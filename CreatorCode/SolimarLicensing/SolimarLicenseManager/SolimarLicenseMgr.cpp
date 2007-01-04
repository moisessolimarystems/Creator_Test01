// SolimarLicenseMgr.cpp : Implementation of CSolimarLicenseMgr

#include "stdafx.h"
#include "SolimarLicenseMgr.h"
#include "..\common\ChallengeResponse.h"
#include "..\common\Version.h"
#include "..\common\SafeMutex.h"
#include "..\common\TimeHelper.h"
#include "..\common\EventLogHelper.h"
#include "..\common\LicensingMessage.h"
#include "..\SolimarLicenseServer\KeyError.h"
#include "..\SolimarLicenseServer\KeyMessages.h"
#include "resource.h"
#include <string>
#include "list"
#include <time.h>
#include <comdef.h>
#include <math.h>
#include <stdio.h>
#define ENSURE_INITIALIZED if (!m_initialized) return LicenseServerError::EHR_KEY_NO_SUITABLE_KEY

BYTE CSolimarLicenseMgr::challenge_key_manager_thisauthuser_public[] = {
#include "..\common\keys\SolimarLicenseManager.ThisAuthUser.public.key.txt"
};

BYTE CSolimarLicenseMgr::challenge_key_manager_userauththis_private[] = {
#include "..\common\keys\SolimarLicenseManager.UserAuthThis.private.key.txt"
};

BYTE CSolimarLicenseMgr::challenge_key_server_thisauthuser_private[] = {
#include "..\common\keys\SolimarLicenseServer.ThisAuthUser.private.key.txt"
};

BYTE CSolimarLicenseMgr::challenge_key_server_userauththis_public[] = {
#include "..\common\keys\SolimarLicenseServer.UserAuthThis.public.key.txt"
};


CSolimarLicenseMgr::KeyInfo::KeyInfo() : 
	KeyPresent(false),
	KeyActive(false),
	KeyObtained(false),
	KeyLocked(false),
	KeyValid(false),
	KeyTrialInfoInitialized(false),
	KeyIsTrial(false),
	KeyTrialHoursLeft(0),
	KeyExpiresDaysLeft(0)
{
	;
}

CSolimarLicenseMgr::KeyInfo::KeyInfo(const KeyInfo &k) : 
	licenses_total(k.licenses_total),
	licenses_allocated(k.licenses_allocated),
	KeyPresent(k.KeyPresent),
	KeyActive(k.KeyActive),
	KeyObtained(k.KeyObtained),
	KeyLocked(k.KeyLocked),
	KeyValid(k.KeyValid),
	KeyTrialInfoInitialized(k.KeyTrialInfoInitialized),
	KeyIsTrial(k.KeyIsTrial),
	KeyTrialHoursLeft(k.KeyTrialHoursLeft),
	KeyExpiresDaysLeft(k.KeyExpiresDaysLeft)
{
	;
}


CSolimarLicenseMgr::ServerInfo::ServerInfo() :
	LicenseServer(NULL)
{
	;
}

CSolimarLicenseMgr::ServerInfo::ServerInfo(const ServerInfo &s) : 
	LicenseServer(s.LicenseServer)
{
	;
}

CSolimarLicenseMgr::ServerInfo::ServerInfo(_bstr_t servername, GITPtr<ISolimarLicenseSvr2> pILicenseServer) : 
	name(servername), 
	LicenseServer(pILicenseServer)
{
	;
}

CSolimarLicenseMgr::ServerInfo::~ServerInfo()
{
	;
}
HRESULT CSolimarLicenseMgr::ServerInfo::Connect()
{
	// Try to create an ISolimarLicenseServer proxy to the server
	COSERVERINFO	serverInfo	= {0, this->name, NULL, 0};
	MULTI_QI		multiQI		= {&__uuidof(ISolimarLicenseSvr2), NULL, NOERROR};
	
	HRESULT hr = CoCreateInstanceEx(
		__uuidof(CSolimarLicenseSvr), 
		NULL, 
		CLSCTX_REMOTE_SERVER, 
		&serverInfo, 
		1, 
		&multiQI);
	if (SUCCEEDED(hr))
	{
		ISolimarLicenseSvr2 *pILicenseServer = (ISolimarLicenseSvr2*)multiQI.pItf;
		ChallengeResponseHelper CR(challenge_key_server_thisauthuser_private, sizeof(challenge_key_server_thisauthuser_private), challenge_key_server_userauththis_public, sizeof(challenge_key_server_userauththis_public));
		// try to authenticate the license server
		hr = CR.AuthenticateServer(pILicenseServer);
		if (SUCCEEDED(hr))
		{
			// let the license server authenticate this manager
			hr = CR.AuthenticateToServer(pILicenseServer);
			if (SUCCEEDED(hr))
			{
				hr = this->LicenseServer.Attach(pILicenseServer);
			}
		}
		if (FAILED(hr))
			pILicenseServer->Release();
	}
		
	return hr;
}



CSolimarLicenseMgr::CSolimarLicenseMgr() : 
	m_initialized(false), 
	ServerListLock(CreateMutex(0,0,0)),
	GracePeriodLock(CreateMutex(0,0,0)),
	MessageListLock(CreateMutex(0,0,0)),
	m_ui_level(UI_IGNORE),
	m_dtGracePeriodStart(0),
	m_dtGracePeriod(0),
	m_dtRefreshKeyList(0),
	ChallengeResponseHelper(challenge_key_manager_userauththis_private, sizeof(challenge_key_manager_userauththis_private), challenge_key_manager_thisauthuser_public, sizeof(challenge_key_manager_thisauthuser_public))
{
	HeartbeatThread = new APCTimer(HeartbeatThreadFunction, this, 3*1000 /* 3 seconds */, InitTimerThreadCB, this);
}

CSolimarLicenseMgr::~CSolimarLicenseMgr()
{
	if (HeartbeatThread)
		delete HeartbeatThread;
	if (ServerListLock!=INVALID_HANDLE_VALUE)
		CloseHandle(ServerListLock);
	if (GracePeriodLock!=INVALID_HANDLE_VALUE)
		CloseHandle(GracePeriodLock);
	if (MessageListLock!=INVALID_HANDLE_VALUE)
		CloseHandle(MessageListLock);
}

void CSolimarLicenseMgr::HeartbeatThreadFunction(void* pvThis)
{
	HRESULT hr = S_OK;
	CSolimarLicenseMgr *pThis = (CSolimarLicenseMgr*)pvThis;
	pThis->SendHeartbeat();
}



// CSolimarLicenseMgr
STDMETHODIMP CSolimarLicenseMgr::Challenge(VARIANT vtChallenge, VARIANT *pvtResponse)
{
	HRESULT hr = S_OK;
	
	VariantClear(pvtResponse);
	
	// Initialize the ChallengeResponse class with the private key, to decrypt the message
	ChallengeResponse CR(challenge_key_manager_userauththis_private, sizeof(challenge_key_manager_userauththis_private));
	ChallengeResponse::Message challenge(vtChallenge), response;
	
	hr = CR.GenerateResponse(challenge,response);
	
	if (SUCCEEDED(hr))
	{
		*pvtResponse = response;
	}
	
	return hr;
}

STDMETHODIMP CSolimarLicenseMgr::GetChallenge(VARIANT *pvtChallenge)
{
	return GetChallengeInternal(pvtChallenge);
}

STDMETHODIMP CSolimarLicenseMgr::PutResponse(VARIANT vtResponse)
{
	return PutResponseInternal(vtResponse);
}



STDMETHODIMP CSolimarLicenseMgr::Connect(BSTR server)
{
	HRESULT hr = S_OK;

	{
	SafeMutex mutex(ServerListLock);
	ServerList::iterator serverIt = m_servers.find(server);
	if(serverIt != m_servers.end())
		return hr;	//Return is already connected to the server
	}
	
	// Try to create an ISolimarLicenseServer proxy to the server
	COSERVERINFO	serverInfo	= {0, server, NULL, 0};
	MULTI_QI		multiQI		= {&__uuidof(ISolimarLicenseSvr2), NULL, NOERROR};
	
	hr = CoCreateInstanceEx(
		__uuidof(CSolimarLicenseSvr), 
		NULL, 
		CLSCTX_REMOTE_SERVER, 
		&serverInfo, 
		1, 
		&multiQI);
	if (SUCCEEDED(hr))
	{	
		ISolimarLicenseSvr2 *pILicenseServer = (ISolimarLicenseSvr2*)multiQI.pItf;
		ChallengeResponseHelper CR(challenge_key_server_thisauthuser_private, sizeof(challenge_key_server_thisauthuser_private), challenge_key_server_userauththis_public, sizeof(challenge_key_server_userauththis_public));
		// try to authenticate the license server
		hr = CR.AuthenticateServer(pILicenseServer);
		if (SUCCEEDED(hr))
		{
			// let the license server authenticate this manager
			hr = CR.AuthenticateToServer(pILicenseServer);
			if (SUCCEEDED(hr))
			{
				SafeMutex mutex(ServerListLock);
				m_servers[server] = ServerInfo(server, pILicenseServer);
			}
		}
		if (FAILED(hr))
			pILicenseServer->Release();
	}

	m_initialized = false;
	
	return hr;
}

STDMETHODIMP CSolimarLicenseMgr::Disconnect()
{
	SafeMutex mutex(ServerListLock);

	//Free up all the licenses
	for (ModuleLicenseMap::iterator m = m_allocated_licenses.begin(); m != m_allocated_licenses.end(); ++m)
		ModuleLicenseRelease(m->first, m->second);

	while(!m_servers.empty())
		m_servers.erase(m_servers.begin());
	
	m_ui_level = UI_IGNORE;
	m_initialized = false;
	
	return S_OK;
}

//
// S_OK if some keys were found
// S_FALSE if no keys are found
// Error HRESULT if there was an error
STDMETHODIMP CSolimarLicenseMgr::Initialize(long product, long prod_ver_major, long prod_ver_minor, VARIANT_BOOL single_key, BSTR specific_single_key_ident, VARIANT_BOOL lock_keys, long auto_ui_level)
{
	return Initialize2(product, prod_ver_major, prod_ver_minor, single_key, specific_single_key_ident, lock_keys, auto_ui_level, 0);
}

STDMETHODIMP CSolimarLicenseMgr::Initialize2(long product, long prod_ver_major, long prod_ver_minor, VARIANT_BOOL single_key, BSTR specific_single_key_ident, VARIANT_BOOL lock_keys, long auto_ui_level, unsigned long grace_period_minutes)
{
	HRESULT hr = S_OK;
	
	// make sure that the client has authenticated already
	if (!ChallengePassedByClient())
		return HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED);

	if(m_initialized)
	{
		SafeMutex mutex(ServerListLock);

		//Free up all the licenses
		for (ModuleLicenseMap::iterator m = m_allocated_licenses.begin(); m != m_allocated_licenses.end(); ++m)
			ModuleLicenseRelease(m->first, m->second);
	}
	
	m_ui_level = auto_ui_level;
	m_specific_single_key_ident = _bstr_t(specific_single_key_ident, true);
	m_single_key = (single_key==VARIANT_TRUE);
	m_lock_keys = (lock_keys==VARIANT_TRUE);
	m_product = product;
	m_prod_ver_major = prod_ver_major;
	m_prod_ver_minor = prod_ver_minor;
	m_current_single_key = _bstr_t(L"");
	m_initialized = true;
	m_dtGracePeriod = grace_period_minutes;
	
	SafeMutex mutex(ServerListLock);
	
	hr = RefreshLicenses();
	if (FAILED(hr)) return hr;
	
	return RefreshKeyList(true/*Log Error*/);
}

STDMETHODIMP CSolimarLicenseMgr::ValidateLicense(VARIANT_BOOL *license_valid)
{
	HRESULT hr = S_OK;
	try
	{
		hr = ValidateLicenseInternal(license_valid, true);
	}
	catch(_com_error&)
	{
		SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::ValidateLicense() - COM Exception", MT_INFO, LicenseServerError::EC_UNKNOWN);
	}
	catch(...)
	{
		SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::ValidateLicense() - Unknown Exception", MT_INFO, LicenseServerError::EC_UNKNOWN);
	}
	return hr;
}
HRESULT CSolimarLicenseMgr::ValidateLicenseInternal(VARIANT_BOOL *license_valid, bool enter_grace_period_on_error)
{
	HRESULT hr = S_OK;
	ENSURE_INITIALIZED;
	SafeMutex mutex(ServerListLock);
	for(;;)
	{
		hr = RefreshLicenses();
		if (FAILED(hr)) 
		{
			*license_valid = VARIANT_FALSE; break;
		}
		ModuleLicenseMap outstanding_licenses;
		hr = ValidateLicenseCache(outstanding_licenses);
		if (FAILED(hr)) 
		{
			*license_valid = VARIANT_FALSE; 
			break;
		}
		
		// determine if any licenses aren't backed up by keys
		*license_valid = VARIANT_TRUE;
		for (ModuleLicenseMap::iterator module = outstanding_licenses.begin(); module != outstanding_licenses.end(); ++module)
		{
			if (module->second>0)
			{
				*license_valid = VARIANT_FALSE;
				break;
			}
		}
		break;
	}
	if(*license_valid == VARIANT_FALSE)
	{
		if(enter_grace_period_on_error)
		{
			StartGracePeriod();	//Does nothing if grace period has already been started
			if(!InViolationPeriod())
			{
				*license_valid = VARIANT_TRUE;
				hr = S_OK;
			}
		}
	}
	else if(*license_valid == VARIANT_TRUE)
	{
		StopGracePeriod();
	}
	return hr;
}

// count the total licenses in all keys
STDMETHODIMP CSolimarLicenseMgr::ModuleLicenseTotal(long module_id, long *count)
{
	HRESULT hr = S_OK;
	
	*count = 0;
	
	ENSURE_INITIALIZED;
	
	SafeMutex mutex(ServerListLock);
	
	// perform the license de-allocation
	RefreshLicenses();	//don't return the state of the manager
	
	// foreach server
	for (ServerList::iterator server = m_servers.begin(); server != m_servers.end(); ++server)
	{
		// foreach key
		for (ServerInfo::KeyList::iterator key = server->second.keys.begin(); key != server->second.keys.end(); ++key)
		{
			if (key->second.KeyPresent)
			{
				long c = key->second.licenses_total[module_id];
				// if one of the licenses is set to 'unlimited', retain this fact
				if (*count>=0x7FFFFFFF || c>=0x7FFFFFFF)
					*count = 0x7FFFFFFF;
				else
					*count += c;
			}
		}
	}
	
	return S_OK;
}

STDMETHODIMP CSolimarLicenseMgr::ModuleLicenseInUse(long module_id, long *count)
{
	HRESULT hr = S_OK;
	
	*count = 0;
	
	ENSURE_INITIALIZED;
	
	SafeMutex mutex(ServerListLock);
	
	// check the cache of obtained licenses
	*count = m_allocated_licenses[module_id];
	
	return S_OK;
}

STDMETHODIMP CSolimarLicenseMgr::ModuleLicenseObtain(long module_id, long count)
{
//wchar_t debug_buf[1024];
//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::ModuleLicenseObtain module_id=%d, count=%d)", module_id, count);
//debug_buf[1023] = 0;
//OutputDebugStringW(debug_buf);
	HRESULT hr = S_OK;
	
	ENSURE_INITIALIZED;
	
	if (count<0)
		return E_INVALIDARG;

	if(GracePeriodHasStarted())
		return E_FAIL;

	SafeMutex mutex(ServerListLock);
	
	// record that the licenses were obtained
	m_allocated_licenses[module_id] += count;
	
	// perform the license allocation
	VARIANT_BOOL licensing_valid = VARIANT_FALSE;
	hr = ValidateLicenseInternal(&licensing_valid, false);
	if (FAILED(hr) || licensing_valid==VARIANT_FALSE) 
		m_allocated_licenses[module_id] -= count;

	if (FAILED(hr)) return hr;
	return (licensing_valid == VARIANT_TRUE ? S_OK : E_FAIL);
}

STDMETHODIMP CSolimarLicenseMgr::ModuleLicenseRelease(long module_id, long count)
{
	//static const MAX_MESSAGE_SIZE = 1024;
	//wchar_t event_log_msg[MAX_MESSAGE_SIZE];	
	//_snwprintf(event_log_msg, MAX_MESSAGE_SIZE, L"ModuleLicenseRelease module_id=%d, count=%d", module_id, count);
	//EventLogHelper::WriteEventLog(L"Solimar License Server", event_log_msg, MT_INFO);
	HRESULT hr = S_OK;
	
	ENSURE_INITIALIZED;
	
	SafeMutex mutex(ServerListLock);
	
	if (count<0 || count>m_allocated_licenses[module_id])
		return E_INVALIDARG;

	// record that the licenses were released
	m_allocated_licenses[module_id] -= count;
	
	// perform the license de-allocation
	RefreshLicenses();	//don't return the state of the manager
	return S_OK;
}

//passing in -1 for the module ID will return a list of all keys serial numbers for the product.
STDMETHODIMP CSolimarLicenseMgr::ModuleLicenseSerialNumbers(long module_id, VARIANT *pvtSerialNumberList)
{
	HRESULT hr = S_OK;
	ENSURE_INITIALIZED;

	SafeMutex mutex(ServerListLock);
	std::list<unsigned long> serialNumberList;// = new std::list<unsigned long>();
	// for each server
	for (ServerList::iterator server = m_servers.begin(); server != m_servers.end(); ++server)
	{
		// get the key list form the server
		VARIANT vtKeyList;
		try 
		{
			SS_SLSERVER_FTCALL_HR(server->second, KeyEnumerate, (&vtKeyList), hr);
			if (SUCCEEDED(hr) && (vtKeyList.vt & (VT_ARRAY | VT_VARIANT)))
			{
				VARIANT *pvtKeyIdent;
				if (SUCCEEDED(SafeArrayAccessData(vtKeyList.parray, (void**)&pvtKeyIdent)))
				{
					// for each key on the server
					for (unsigned int i = 0; i<vtKeyList.parray->rgsabound[0].cElements; ++i)
					{
						_bstr_t key_ident = pvtKeyIdent[i].bstrVal;
						VARIANT vtKeyProductID, vtKeyProductVersion;
						
						// check that the key is present and valid
						VARIANT_BOOL key_present(VARIANT_FALSE), key_license_valid(VARIANT_FALSE), key_active(VARIANT_FALSE);
						hr = server->second.LicenseServer->KeyIsPresent(key_ident, &key_present);
						if (FAILED(hr) || key_present==VARIANT_FALSE) {hr = S_OK; continue;}
						hr = server->second.LicenseServer->KeyIsActive(key_ident, &key_active);
						if (FAILED(hr) || key_active==VARIANT_FALSE) {hr = S_OK; continue;}
						hr = server->second.LicenseServer->KeyValidateLicense(key_ident, &key_license_valid);
						if (FAILED(hr) || key_license_valid==VARIANT_FALSE) {hr = S_OK; continue;}
						
						// check that the key has the requisite product version and etc.
						hr = server->second.LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product Version"].id, &vtKeyProductVersion);
						if (FAILED(hr)) {hr = S_OK; continue;}
						hr = server->second.LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product ID"].id, &vtKeyProductID);
						if (FAILED(hr)) {hr = S_OK; continue;}
						
						// if the product id and product version requirements are satisfied
						if (Version::TinyVersion(vtKeyProductVersion.uiVal,0) >= Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0)) && m_keyspec.products[m_product].id==vtKeyProductID.uiVal)
						{
							// if a specific key is requested, but this one is not it, skip this key
							if (!(m_single_key && m_specific_single_key_ident.length()>0 && m_specific_single_key_ident!=key_ident))
							{
								// refresh the cache of licenses on the key
								// for each module in the key spec for the product
								for (KeySpec::Product::data_list_t::iterator module = m_keyspec.products[m_product].data.begin(); module != m_keyspec.products[m_product].data.end(); ++module)
								{
									//
									//Accept -1 as ignore moduleID
									if ((module_id == static_cast<long>(module->id) && module->isLicense) || module_id == -1)
									{
										VARIANT vtKeyNum, vtCustNum;
										VariantInit(&vtKeyNum);
										VariantInit(&vtCustNum);
										hr = server->second.LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Key Number"].id, &vtKeyNum);
										if (FAILED(hr)) {hr = S_OK; continue;}
										hr = server->second.LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Customer Number"].id, &vtCustNum);
										if (FAILED(hr)) {hr = S_OK; continue;}

										serialNumberList.push_back((vtCustNum.uiVal << 16) + vtKeyNum.uiVal);

										VariantClear(&vtKeyNum);
										VariantClear(&vtCustNum);
										break;
									}
								}						
							}
						}
					}
					SafeArrayUnaccessData(vtKeyList.parray);
				}
			}
		}
		catch (_com_error &e) 
		{
			if(SS_RPC_FAILED(e.Error()))
			{
				//Log Message about RPC failure to Key
				SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::ModuleLicenseSerialNumbers() - RPC Error", MT_INFO, LicenseServerError::EC_UNKNOWN);
				SS_GENERATE_AND_DISPATCH_MESSAGE(LicensingMessageStringTable[MessageClientTimeout], MT_INFO, LicenseServerError::EC_CLIENT_TIMEOUT);
			}
		}
	}

	VariantInit(pvtSerialNumberList);
	pvtSerialNumberList->vt = VT_ARRAY | VT_UI4;

	pvtSerialNumberList->parray = SafeArrayCreateVector(VT_UI4, 0, serialNumberList.size()>0 ? (ULONG)serialNumberList.size() : 1);
	if(!pvtSerialNumberList->parray)
		return E_FAIL;

	long* pSerialNumber;
	hr = SafeArrayAccessData(pvtSerialNumberList->parray, (void**)&pSerialNumber);
	if(FAILED(hr))
		return E_FAIL;

	if(serialNumberList.size())
	{
		while(!serialNumberList.empty())
		{
			*pSerialNumber = (*serialNumberList.begin());
			pSerialNumber++;
			serialNumberList.pop_front();
		}
	}
	else
	{
		//No keys found, return 0xffffffff
		*pSerialNumber = 0xffffffff;
	}

	if (pvtSerialNumberList->parray)
		SafeArrayUnaccessData(pvtSerialNumberList->parray);


	return S_OK;
}

STDMETHODIMP CSolimarLicenseMgr::ModuleLicenseCounterDecrement(long module_id, long license_count)
{
	HRESULT hr = S_OK;
	ENSURE_INITIALIZED;

	if(GracePeriodHasStarted())
	{
		return E_FAIL;
	}

	SafeMutex mutex(ServerListLock);
	
	// perform the license allocation
	VARIANT_BOOL licensing_valid = VARIANT_FALSE;
	hr = ValidateLicenseInternal(&licensing_valid, false);
	if (FAILED(hr) || licensing_valid==VARIANT_FALSE) 
		hr = E_FAIL;

	long licenseToDecrement = license_count;
	if(SUCCEEDED(hr))
	{

		// for each server
		for (ServerList::iterator server = m_servers.begin(); server != m_servers.end() && licenseToDecrement>0; ++server)
		{
			// get the key list form the server
			VARIANT vtKeyList;
			hr = server->second.LicenseServer->KeyEnumerate(&vtKeyList);
			if (SUCCEEDED(hr) && (vtKeyList.vt & (VT_ARRAY | VT_VARIANT)))
			{
				VARIANT *pvtKeyIdent;
				if (SUCCEEDED(SafeArrayAccessData(vtKeyList.parray, (void**)&pvtKeyIdent)))
				{
					// for each key on the server
					for (unsigned int i = 0; i<vtKeyList.parray->rgsabound[0].cElements && licenseToDecrement>0; ++i)
					{
						_bstr_t key_ident = pvtKeyIdent[i].bstrVal;
						VARIANT vtKeyProductID, vtKeyProductVersion;

						// check that the key has the requisite product version and etc.
						hr = server->second.LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product Version"].id, &vtKeyProductVersion);
						if (FAILED(hr)) {hr = S_OK; continue;}
						hr = server->second.LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product ID"].id, &vtKeyProductID);
						if (FAILED(hr)) {hr = S_OK; continue;}
					
						// if the product id and product version requirements are satisfied
						if (Version::TinyVersion(vtKeyProductVersion.uiVal,0) >= Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0)) && m_keyspec.products[m_product].id==vtKeyProductID.uiVal)
						{
							// if a specific key is requested, but this one is not it, skip this key
							if (!(m_single_key && m_specific_single_key_ident.length()>0 && m_specific_single_key_ident!=key_ident))
							{
								for (KeySpec::Product::data_list_t::iterator module = m_keyspec.products[m_product].data.begin(); module != m_keyspec.products[m_product].data.end() && licenseToDecrement>0; ++module)
								{
									if (module->isLicense && module_id == static_cast<long>(module->id))
									{
										long licenses_total(0);

										// get the total number of licenses
										hr = server->second.LicenseServer->KeyModuleLicenseTotal(key_ident, module_id, &licenses_total);
										if(licenses_total >= licenseToDecrement)
										{
											//This key has enough licenses to handle license decrement
											hr = server->second.LicenseServer->KeyModuleLicenseCounterDecrement(key_ident, module_id, license_count);
											if(SUCCEEDED(hr))	//hr == E_FAIL if key is a trial key or can't decrement from key
												licenseToDecrement = 0;
										}
										else //if(licenses_total < licenseToDecrement)
										{
											//This key doesn't have enough licenses to handle full license decrement, 
											//decrement what can be done
											hr = server->second.LicenseServer->KeyModuleLicenseCounterDecrement(key_ident, module_id, licenses_total);
											if(SUCCEEDED(hr))	//hr == E_FAIL if key is a trial key or can't decrement from key
												licenseToDecrement = licenseToDecrement - licenses_total;
										}

										break;	//found the module on this key...
									}
								}
							}
						}
					}
					SafeArrayUnaccessData(vtKeyList.parray);
				}
			}
		}
	}

	// perform the license de-allocation
	if(SUCCEEDED(hr))
	{
		//Need to do if I force obtain to be called before.
		// record that the licenses were released
		//ModuleLicenseRelease(module_id, count);

		//Need to do if I assume obtain was not called before.
		hr = RefreshLicenses();
	}
	return hr;
}
bool CSolimarLicenseMgr::ManagesKey(_bstr_t key_ident)
{
	if (key_ident.length()==0) 
	{
		return true;
	}

	// foreach server
	for (ServerList::iterator server = m_servers.begin(); server != m_servers.end(); ++server)
	{
		// if this server's key list contains the key in question
		if (server->second.keys.find(key_ident)!=server->second.keys.end())
		{
			return true;
		}
	}
	return false;
}

STDMETHODIMP CSolimarLicenseMgr::GetLicenseMessageList(VARIANT_BOOL clear_messages, VARIANT *pvtMessageList)
{
	HRESULT hr = S_OK;
	
	ENSURE_INITIALIZED;
	try
	{
		SafeMutex mutex(ServerListLock);
		{
			SafeMutex mutex_messages(MessageListLock);
			// foreach server append messages from the server to the message cache
			for (ServerList::iterator server = m_servers.begin(); server != m_servers.end(); ++server)
			{
				ILicensingMessage *pServerLicensingMessage = 0;
				_variant_t vtServerMessageList;
				try
				{
					//hr = server->second.LicenseServer->QueryInterface(__uuidof(ILicensingMessage), (void**)&pServerLicensingMessage);
					SS_SLSERVER_FTCALL_HR(server->second, QueryInterface, (__uuidof(ILicensingMessage), (void**)&pServerLicensingMessage), hr);
					if (SUCCEEDED(hr))
					{
						pServerLicensingMessage->GetLicenseMessageList(VARIANT_TRUE, &vtServerMessageList);
						LicensingMessageList server_messages(vtServerMessageList);
						licensing_message_cache.insert(licensing_message_cache.end(), server_messages.begin(), server_messages.end());
						pServerLicensingMessage->Release();
					}
				}
				catch (_com_error &e) 
				{
					if(SS_RPC_FAILED(e.Error()))
					{
						//Log Message about RPC failure to Key
						SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::GetLicenseMessageList() - RPC Error", MT_INFO, LicenseServerError::EC_UNKNOWN);
						SS_GENERATE_AND_DISPATCH_MESSAGE(LicensingMessageStringTable[MessageClientTimeout], MT_INFO, LicenseServerError::EC_CLIENT_TIMEOUT);
					}
				}
			}
			// filter the message list, removing messages that are not relevant to the manager (by key identifier)
			for (LicensingMessageList::iterator m = licensing_message_cache.begin(); m != licensing_message_cache.end();)
			{
				
				if (!ManagesKey(m->key_ident.c_str()))
				{
					m = licensing_message_cache.erase(m);
				}
				else
				{
					++m;
				}
			}
			*pvtMessageList = licensing_message_cache;
			if (clear_messages==VARIANT_TRUE)
				licensing_message_cache.clear();
		}
	}
	catch(_com_error&)
	{
		SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::GetLicenseMessageList() - COM Exception", MT_INFO, LicenseServerError::EC_UNKNOWN);
	}
	catch(...)
	{
		SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::GetLicenseMessageList() - Unknown Exception", MT_INFO, LicenseServerError::EC_UNKNOWN);
	}
	return S_OK;
}

STDMETHODIMP CSolimarLicenseMgr::DispatchLicenseMessageList(VARIANT_BOOL clear_messages)
{
	HRESULT hr = S_OK;
	
	ENSURE_INITIALIZED;
	
	SafeMutex mutex(ServerListLock);
	{
		SafeMutex mutex_messages(MessageListLock);

		for (LicensingMessageList::iterator m = licensing_message_cache.begin(); m != licensing_message_cache.end(); ++m)
		{
			DispatchLicenseMessage(_bstr_t(m->key_ident.c_str()), m->message_type, m->error, m->timestamp, _bstr_t(m->message.c_str()));
		}

		if (clear_messages==VARIANT_TRUE)
			licensing_message_cache.clear();
	}

	return S_OK;
}


void CSolimarLicenseMgr::SendHeartbeat()
{
	HRESULT hr = S_OK;

	SafeMutex mutex(ServerListLock);
	for (ServerList::iterator server = m_servers.begin(); server != m_servers.end(); ++server)
	{
		try 
		{
			SS_SLSERVER_FTCALL_HR(server->second, Heartbeat, (), hr);
		}
		catch (_com_error &e) 
		{
			if(SS_RPC_FAILED(e.Error()))
			{
				//Log Message about RPC failure to Key
				SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::SendHeartbeat() - RPC Error", MT_INFO, LicenseServerError::EC_UNKNOWN);
				SS_GENERATE_AND_DISPATCH_MESSAGE(LicensingMessageStringTable[MessageClientTimeout], MT_INFO, LicenseServerError::EC_CLIENT_TIMEOUT);
			}
		}
	}
}

bool CSolimarLicenseMgr::MessageQualifiesForAutoDispatch(DWORD ui_level, long message_type)
{
	return
	(
		(ui_level & UI_LEVEL_ALL)
		||
		((ui_level & UI_LEVEL_RESPONSE) && (message_type==0))
		||
		((ui_level & UI_LEVEL_CRITICAL) && (message_type==2))
	);
}

bool CSolimarLicenseMgr::isAutoUiStyleDialog(DWORD ui_level)
{
	return (ui_level & UI_STYLE_DIALOG)!=0;
}

bool CSolimarLicenseMgr::isAutoUiStyleEventLog(DWORD ui_level)
{
	return (ui_level & UI_STYLE_EVENT_LOG)!=0;
}

/*
		UI_IGNORE =				0x00,
		UI_LEVEL_ALL =			0x01,
		UI_LEVEL_RESPONSE =		0x02,
		UI_LEVEL_CRITICAL =		0x04,
		UI_STYLE_DIALOG =		0x10,
		UI_STYLE_EVENT_LOG =	0x20,
*/

void CSolimarLicenseMgr::DispatchLicenseMessage(BSTR key_ident, long message_type, long error, VARIANT vtTimestamp, BSTR message)
{
	// perform the auto ui requested (dialogs, event log, etc.)
	if (MessageQualifiesForAutoDispatch(m_ui_level, message_type))
	{
		if (isAutoUiStyleDialog(m_ui_level))
		{
			KeyMessageShowDialog(key_ident, message_type, error, vtTimestamp, message);
		}
		if (isAutoUiStyleEventLog(m_ui_level))
		{
			KeyMessageWriteEventLog(key_ident, message_type, error, vtTimestamp, message);
		}
	}
}

void CSolimarLicenseMgr::KeyMessageShowDialog(BSTR key_ident, unsigned int message_type, HRESULT error, VARIANT vtTimestamp, BSTR message)
{
	ShowLicenseDialog(_bstr_t(L"Licensing Information"), _bstr_t(message,true));
}

void CSolimarLicenseMgr::KeyMessageWriteEventLog(BSTR key_ident, unsigned int message_type, HRESULT error, VARIANT vtTimestamp, BSTR message)
{
	static const int MAX_MESSAGE_SIZE = 1024;
	wchar_t event_log_msg[MAX_MESSAGE_SIZE];	
	_variant_t vtStrTimestamp = vtTimestamp;
	vtStrTimestamp.ChangeType(VT_BSTR);
	_bstr_t str_timestamp = vtStrTimestamp.bstrVal;
	_bstr_t str_error_message;	// look in i:\chris r\samplehr.txt
	//xxx implement GetErrorMessage such that it returns the right error message?
	str_error_message = GetErrorMessage(error).c_str();
	switch (message_type)
	{
	case MT_INFO:
		_snwprintf(event_log_msg, MAX_MESSAGE_SIZE, L"Solimar Systems, Inc.\r\nProduct Licensing Status Message\r\n%s\r\nKey: %s\r\n\r\n%s",
			str_timestamp,
			key_ident,
			message);
		break;
	case MT_ERROR:
		_snwprintf(event_log_msg, MAX_MESSAGE_SIZE, L"Solimar Systems, Inc.\r\nProduct Licensing Error Message\r\n%s\r\nKey: %s\r\n%08x %s\r\n\r\n%s",
			str_timestamp,
			key_ident,
			error,
			str_error_message,
			message);
		break;
	}
	
	unsigned int event_type = EVENTLOG_INFORMATION_TYPE;
	if (error & 0x8000000) event_type = EVENTLOG_ERROR_TYPE;
	EventLogHelper::WriteEventLog(L"Solimar License Server", event_log_msg, message_type);
}

HRESULT CSolimarLicenseMgr::ShowLicenseDialog(_bstr_t caption, _bstr_t message)
{
	HRESULT hr = S_OK;

	// display the dialog box
	LicenseDialogParams params;
	params.caption = caption;
	params.message = message;
	if (-1==DialogBoxParam(GetModuleHandle(L"SolimarLicenseManager.dll"), MAKEINTRESOURCE(IDD_LICENSE_DLG), 0, (DLGPROC)LicenseDlgProc, (LPARAM)&params))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}

BOOL CALLBACK CSolimarLicenseMgr::LicenseDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_INITDIALOG:
		{
			// set the position of the dialog box
			LicenseDialogParams *pParams = (LicenseDialogParams*)lParam;
			SetDlgItemTextW(hDlg, IDC_STATIC_MESSAGE, (wchar_t*)pParams->message);
			SetWindowTextW(hDlg, (wchar_t*)pParams->caption);
			return TRUE;
			break;
		}
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDOK:
				{
					//InvokePackage(GlobalDatabaseDefines::ePT_InstallSrc);
					EndDialog(hDlg,0);
					return TRUE;
				}
			}
			break;
		}
		case WM_CLOSE:
		case WM_DESTROY:
		{
			EndDialog(hDlg,0);
			return TRUE;
			break;
		}
	}
	return FALSE;
}




//
// refresh the list of the keys on the currently connected servers
// S_OK if some keys were found
// S_FALSE if no keys are found
// Error HRESULT if there was an error
HRESULT CSolimarLicenseMgr::RefreshKeyList(bool _bLogError)
{
	HRESULT hr = S_OK;
	
	ENSURE_INITIALIZED;
	
	SafeMutex mutex(ServerListLock);
		
	bool found_keys = false;
	bool bGracePeriodSeen = false;
	// for each server
	for (ServerList::iterator server = m_servers.begin(); server != m_servers.end(); ++server)
	{
		// foreach currently known key
		for (ServerInfo::KeyList::iterator k = server->second.keys.begin(); k != server->second.keys.end(); ++k)
		{
			// mark all keys as not being present
			// if they are seen on the server scan, they will be marked as present
			k->second.KeyPresent = false;
		}
		
		// get the key list form the server
		VARIANT vtKeyList;
		try
		{
			SS_SLSERVER_FTCALL_HR(server->second, KeyEnumerate, (&vtKeyList), hr);
			if (SUCCEEDED(hr) && (vtKeyList.vt & (VT_ARRAY | VT_VARIANT)))
			{
				VARIANT *pvtKeyIdent;
				if (SUCCEEDED(SafeArrayAccessData(vtKeyList.parray, (void**)&pvtKeyIdent)))
				{
					// for each key on the server
					for (unsigned int i = 0; i<vtKeyList.parray->rgsabound[0].cElements; ++i)
					{
						_bstr_t key_ident = pvtKeyIdent[i].bstrVal;
						VARIANT vtKeyProductID, vtKeyProductVersion;

						// check that the key is present and valid
						VARIANT_BOOL key_present(VARIANT_FALSE), key_license_valid(VARIANT_FALSE), key_active(VARIANT_FALSE);
						hr = server->second.LicenseServer->KeyIsPresent(key_ident, &key_present);
						if (FAILED(hr) || key_present==VARIANT_FALSE) {hr = S_OK; continue;}
						hr = server->second.LicenseServer->KeyIsActive(key_ident, &key_active);
						if (FAILED(hr)) {hr = S_OK; continue;}
						hr = server->second.LicenseServer->KeyValidateLicense(key_ident, &key_license_valid);
						if (FAILED(hr)) {hr = S_OK; continue;}
						
						// check that the key has the requisite product version and etc.
						hr = server->second.LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product Version"].id, &vtKeyProductVersion);
						if (FAILED(hr)) {hr = S_OK; continue;}
						hr = server->second.LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product ID"].id, &vtKeyProductID);
						if (FAILED(hr)) {hr = S_OK; continue;}
						
						// get information about trial keys
						_variant_t vtKeyStatus, vtExpirationDate, vtServerTime;
						long lTrialHours(0);
						hr = server->second.LicenseServer->GetLicenseServerTime(&vtServerTime);
						if (FAILED(hr)) {hr = S_OK; continue;}
						hr = server->second.LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Status"].id, &vtKeyStatus);
						if (FAILED(hr)) {hr = S_OK; continue;}
						hr = server->second.LicenseServer->KeyTrialExpires(key_ident, &vtExpirationDate);
						if (FAILED(hr)) {hr = S_OK; continue;}
						hr = server->second.LicenseServer->KeyTrialHours(key_ident, &lTrialHours);
						if (FAILED(hr)) {hr = S_OK; continue;}
						
						// if the product id and product version requirements are satisfied
						if (Version::TinyVersion(vtKeyProductVersion.uiVal,0) >= Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0)) && m_keyspec.products[m_product].id==vtKeyProductID.uiVal)
						{
							// if a specific key is requested, but this one is not it, skip this key
							if (!(m_single_key && m_specific_single_key_ident.length()>0 && m_specific_single_key_ident!=key_ident))
							{
								server->second.keys[key_ident].KeyPresent = true;
								server->second.keys[key_ident].KeyValid = (key_license_valid != VARIANT_FALSE && key_active != VARIANT_FALSE);
								
								// trial information and messages
								bool TrialInfoInitialized = server->second.keys[key_ident].KeyTrialInfoInitialized;
								bool KeyActive = (key_active==VARIANT_TRUE);
								bool KeyIsTrial = false;
								long KeyTrialHoursLeft = lTrialHours;
								long KeyExpiresDaysLeft(0);
								
								_variant_t vtExpirationDaysLeft;
								hr = VarSub(&vtExpirationDate, &vtServerTime, &vtExpirationDaysLeft);
								if (FAILED(hr) || vtExpirationDaysLeft.dblVal < 0.0) {vtExpirationDaysLeft.vt = VT_DATE; vtExpirationDaysLeft.dblVal = 0.0; hr = S_OK;}
								KeyExpiresDaysLeft = static_cast<long>(floor(vtExpirationDaysLeft.dblVal));
								
								enum KeyStatuses
								{
									UNINITIALIZED_TRIAL=3, INITIAL_TRIAL=0, EXTENDED_TRIAL=1,
									EXTENDED_TRIAL2=4, EXTENDED_TRIAL3=5, EXTENDED_TRIAL4=6,
									EXTENDED_TRIAL5=7, BASE=2, UNUSED=10, DEACTIVATED=11
								};

								KeyIsTrial = (
									vtKeyStatus.ulVal == INITIAL_TRIAL ||
									vtKeyStatus.ulVal == EXTENDED_TRIAL ||
									vtKeyStatus.ulVal == EXTENDED_TRIAL2 ||
									vtKeyStatus.ulVal == EXTENDED_TRIAL3 ||
									vtKeyStatus.ulVal == EXTENDED_TRIAL4 ||
									vtKeyStatus.ulVal == EXTENDED_TRIAL5 ||
									vtKeyStatus.ulVal == UNINITIALIZED_TRIAL
								);
								
								wchar_t licensing_message_buffer[256];
								
								// if the key has just become inactive add message
								if (TrialInfoInitialized && !KeyActive && server->second.keys[key_ident].KeyActive)
								{
									swprintf(licensing_message_buffer, LicensingMessageStringTable[MessageLMTempKeyExpired], KeyTrialHoursLeft, (wchar_t*)_bstr_t(vtExpirationDate));
									AddLicensingMessage(LicensingMessage(std::wstring((wchar_t*)server->first), std::wstring((wchar_t*)key_ident), vtServerTime, MT_WARNING, MessageLMTempKeyExpired, std::wstring(licensing_message_buffer), 0, vtKeyStatus.ulVal, vtExpirationDate, KeyTrialHoursLeft));
								}
								// if the key has just become active add message
								if (TrialInfoInitialized && KeyActive && !server->second.keys[key_ident].KeyActive)
								{
									swprintf(licensing_message_buffer, LicensingMessageStringTable[MessageLMTempKeyActive], KeyTrialHoursLeft, (wchar_t*)_bstr_t(vtExpirationDate));
									AddLicensingMessage(LicensingMessage(std::wstring((wchar_t*)server->first), std::wstring((wchar_t*)key_ident), vtServerTime, MT_INFO, MessageLMTempKeyActive, std::wstring(licensing_message_buffer), 0, vtKeyStatus.ulVal, vtExpirationDate, KeyTrialHoursLeft));
								}
								// if the key had not been seen before and is inactive
								if (KeyIsTrial && !KeyActive && !TrialInfoInitialized)
								{
									swprintf(licensing_message_buffer, LicensingMessageStringTable[MessageLMTempKeyExpired], KeyTrialHoursLeft, (wchar_t*)_bstr_t(vtExpirationDate));
									AddLicensingMessage(LicensingMessage(std::wstring((wchar_t*)server->first), std::wstring((wchar_t*)key_ident), vtServerTime, MT_WARNING, MessageLMTempKeyExpired, std::wstring(licensing_message_buffer), 0, vtKeyStatus.ulVal, vtExpirationDate, KeyTrialHoursLeft));
								}
								if (KeyIsTrial && KeyActive && 
									(
										// if the key is seen for the first time or the number of trial hours left has changed
										(!TrialInfoInitialized || (TrialInfoInitialized && server->second.keys[key_ident].KeyTrialHoursLeft/24!=KeyTrialHoursLeft/24 && KeyTrialHoursLeft/24 < 15))
										||
										// if the key is seen for the first time or the number of expiration days left has changed
										(!TrialInfoInitialized || (TrialInfoInitialized && server->second.keys[key_ident].KeyExpiresDaysLeft!=KeyExpiresDaysLeft && KeyExpiresDaysLeft < 15))
									)
								)
								{
									swprintf(licensing_message_buffer, LicensingMessageStringTable[MessageLMTempKeyStatus], KeyTrialHoursLeft, (wchar_t*)_bstr_t(vtExpirationDate));
									AddLicensingMessage(LicensingMessage(std::wstring((wchar_t*)server->first), std::wstring((wchar_t*)key_ident), vtServerTime, MT_INFO, MessageLMTempKeyStatus, std::wstring(licensing_message_buffer), 0, vtKeyStatus.ulVal, vtExpirationDate, KeyTrialHoursLeft));
								}
								
								server->second.keys[key_ident].KeyActive = KeyActive;
								server->second.keys[key_ident].KeyTrialInfoInitialized = true;
								server->second.keys[key_ident].KeyIsTrial = KeyIsTrial;
								server->second.keys[key_ident].KeyTrialHoursLeft = KeyTrialHoursLeft;
								server->second.keys[key_ident].KeyExpiresDaysLeft = KeyExpiresDaysLeft;
								
								// refresh the cache of licenses on the key
								// for each module in the key spec for the product
								for (KeySpec::Product::data_list_t::iterator module = m_keyspec.products[m_product].data.begin(); module != m_keyspec.products[m_product].data.end(); ++module)
								{
									if (module->isLicense)
									{
										long module_id = static_cast<long>(module->id);
										long licenses_total(0), licenses_allocated(0);
										// get the total number of licenses
										hr = server->second.LicenseServer->KeyModuleLicenseTotal(key_ident, module_id, &licenses_total);
										if (FAILED(hr)) break;
										// get the number of licenses in use
										hr = server->second.LicenseServer->KeyModuleLicenseInUse2(key_ident, module_id, &licenses_allocated);
										if (FAILED(hr)) break;
										
										server->second.keys[key_ident].licenses_total[module_id] = licenses_total;
										server->second.keys[key_ident].licenses_allocated[module_id] = licenses_allocated;
									}
								}
								
								if (server->second.keys[key_ident].KeyObtained)
								{
									// ensure that the licensing on this key is still valid
									VARIANT_BOOL this_key_license_valid = VARIANT_FALSE;
									HRESULT hr = server->second.LicenseServer->KeyValidateLicense(key_ident, &this_key_license_valid);
									server->second.keys[key_ident].KeyValid = (SUCCEEDED(hr) && this_key_license_valid==VARIANT_TRUE);
								}

								found_keys = true;
							}
						}
						else if(_bLogError && Version::TinyVersion(vtKeyProductVersion.uiVal,0) < Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0)) && m_keyspec.products[m_product].id==vtKeyProductID.uiVal)
						{
							static const int MAX_MESSAGE_SIZE = 1024;
							wchar_t message[MAX_MESSAGE_SIZE];
							_snwprintf(message, MAX_MESSAGE_SIZE, LicensingMessageStringTable[MessageViolationVersion], m_prod_ver_major, m_prod_ver_minor);
							message[MAX_MESSAGE_SIZE-1] = 0;
							
							// convert the time_t in to a variant date
							time_t timestamp = time(0);
							_variant_t vtTimestamp;
							double vtimestamp;
							SYSTEMTIME systimestamp;
							memset(&systimestamp, 0, sizeof(systimestamp));
							tm * tm_struct = gmtime(&timestamp);
							systimestamp.wSecond = tm_struct->tm_sec;
							systimestamp.wMinute = tm_struct->tm_min;
							systimestamp.wHour = tm_struct->tm_hour;
							systimestamp.wDay = tm_struct->tm_mday;
							systimestamp.wMonth = tm_struct->tm_mon;
							systimestamp.wYear = tm_struct->tm_year;
							systimestamp.wDayOfWeek = tm_struct->tm_wday;
							if (SystemTimeToVariantTime(&systimestamp, &vtimestamp))
								vtTimestamp = _variant_t(vtimestamp, VT_DATE);
							else
								vtTimestamp = _variant_t(0.0, VT_DATE);

							DispatchLicenseMessage(key_ident, MT_INFO, LicenseServerError::EC_SP_VERSION_NOT_SUPPORTED, vtTimestamp, _bstr_t(message));
						}
					}
					SafeArrayUnaccessData(vtKeyList.parray);
				}
			}

		}
		catch (_com_error &e) 
		{
			if(SS_RPC_FAILED(e.Error()))
			{
				//Log Message about RPC failure to Key
				SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::RefreshKeyList() - RPC Error", MT_INFO, LicenseServerError::EC_UNKNOWN);
				SS_GENERATE_AND_DISPATCH_MESSAGE(LicensingMessageStringTable[MessageClientTimeout], MT_INFO, LicenseServerError::EC_CLIENT_TIMEOUT);
			}
		}
	}
	// remove any keys that are not present and have no licenses obtained
	hr = RemoveObsoleteKeysFromCache();
	if (FAILED(hr)) return hr;
    
	// S_OK if some keys were found
	// S_FALSE if no keys are found
	// Error HRESULT if there was an error
	return (SUCCEEDED(hr) ? (found_keys ? S_OK : S_FALSE) : hr);
}

// checks that all licenses checked out are accounted for by some key
HRESULT CSolimarLicenseMgr::ValidateLicenseCache(ModuleLicenseMap &outstanding_licenses)
{
	ENSURE_INITIALIZED;
	
	SafeMutex mutex(ServerListLock);	
	
	outstanding_licenses = m_allocated_licenses;
	
	// for each server
	for (ServerList::iterator server = m_servers.begin(); server != m_servers.end(); ++server)
	{
		// foreach key
		for (ServerInfo::KeyList::iterator k = server->second.keys.begin(); k != server->second.keys.end(); ++k)
		{
			if (k->second.KeyPresent && k->second.KeyObtained && k->second.KeyValid)
			{
				// for each module
				for (ModuleLicenseMap::iterator m = outstanding_licenses.begin(); m != outstanding_licenses.end(); ++m)
				{
					m->second -= k->second.licenses_allocated[m->first];
				}
			}
		}
	}
	
	return S_OK;
}

// checks if licenses are valid, if not, an attempt to reallocate licenses is made
HRESULT CSolimarLicenseMgr::RefreshLicenses()
{
	HRESULT hr = S_OK;
	
	ENSURE_INITIALIZED;
	
	SafeMutex mutex(ServerListLock);
	
	// refresh the cache
	time_t timeNow = time(NULL);
	double timeDiffSeconds = difftime(m_dtRefreshKeyList, timeNow);

	// don't call RefreshKeyList if just made the same call within the last second
	if(abs(timeDiffSeconds) > 1.0)	
	{
		hr = RefreshKeyList();
		if(SUCCEEDED(hr))
			m_dtRefreshKeyList = time(NULL);
	}

	if (FAILED(hr)) return hr;
	
	// check to see if there is a difference between 
	// the number of licenses obtained and those backed up by keys
	ModuleLicenseMap outstanding_licenses;
	hr = ValidateLicenseCache(outstanding_licenses);
	if (FAILED(hr)) return hr;
	
	// obtain and release licenses where appropriate
	for (ModuleLicenseMap::iterator module = outstanding_licenses.begin(); module != outstanding_licenses.end(); ++module)
	{
		if (module->second>0)
		{
			hr = ObtainLicensesInternal(module->first, module->second);
			if (FAILED(hr)) 
			{
				return hr;
			}
		}
		else if (module->second<0)
		{
			hr = ReleaseLicensesInternal(module->first, -1*module->second);
			if (FAILED(hr)) return hr;
		}
	}
	
	return hr;
}

HRESULT CSolimarLicenseMgr::RemoveObsoleteKeysFromCache()
{
	ENSURE_INITIALIZED;
	
	SafeMutex mutex(ServerListLock);
	
	// for each server
	for (ServerList::iterator server = m_servers.begin(); server != m_servers.end(); ++server)
	{
		typedef std::vector<ServerInfo::KeyList::iterator> KeyListIteratorList;
		KeyListIteratorList keys_to_delete;

		// foreach key
		for (ServerInfo::KeyList::iterator k = server->second.keys.begin(); k != server->second.keys.end(); ++k)
		{
			if (!k->second.KeyPresent)
			{
				keys_to_delete.push_back(k);
			}
		}
		
		// remove obsoleted keys from the key list
		for (KeyListIteratorList::iterator ki = keys_to_delete.begin(); ki != keys_to_delete.end(); ++ki)
		{
			server->second.keys.erase(*ki);
		}
	}
	return S_OK;
}

// attempts to allocate licenses on the known-good keys in the cache
HRESULT CSolimarLicenseMgr::ObtainLicensesInternal(long module_id, long license_count)
{
	HRESULT hr = S_OK;
	
	ENSURE_INITIALIZED;
	
	if (license_count<=0) return E_INVALIDARG;
	
	SafeMutex mutex(ServerListLock);
	
	long licenses_to_obtain(license_count);
	long licenses_obtained(0);
	
	//xxx && !(single_key && allocated_licenses_on_a_key)
	//xxx how to ensure that the single key case is handled??
	//xxx need an indicator of which key is the current key (if any) and only use that key
	//xxx need a way to transfer licenses from the single key to a different single key if needed in case the first one fills up

	// for each server
	for (ServerList::iterator server = m_servers.begin(); server != m_servers.end(); ++server)
	{
		// foreach key
		for (ServerInfo::KeyList::iterator k = server->second.keys.begin(); k != server->second.keys.end(); ++k)
		{
			if (k->second.KeyPresent && k->second.KeyValid)
			{				
				long key_licenses_available = k->second.licenses_total[module_id]-k->second.licenses_allocated[module_id];
				long key_licenses_to_obtain = min(licenses_to_obtain-licenses_obtained,key_licenses_available);
				
				// if the key has has licenses available and hasn't been obtained yet, obtain it
				if (!k->second.KeyObtained && key_licenses_to_obtain>0)
				{
					try
					{
						SS_SLSERVER_FTCALL_HR(server->second, KeyObtain, (k->first), hr);
						if (FAILED(hr)) continue;
					
						k->second.KeyObtained = true;
						
						if (m_lock_keys)
						{
							hr = server->second.LicenseServer->KeyLock(k->first);
							if (FAILED(hr))
							{
								hr = server->second.LicenseServer->KeyRelease(k->first);
								k->second.KeyObtained = false;
								continue;
							}
							else
							{
								k->second.KeyLocked = true;
							}
						}
					}
					catch (_com_error &e) 
					{
						if(SS_RPC_FAILED(e.Error()))
						{
							//Log Message about RPC failure to Key
							SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::ObtainLicensesInternal() - RPC Error", MT_INFO, LicenseServerError::EC_UNKNOWN);
							SS_GENERATE_AND_DISPATCH_MESSAGE(LicensingMessageStringTable[MessageClientTimeout], MT_INFO, LicenseServerError::EC_CLIENT_TIMEOUT);
						}
					}
					
				}
				
				if (key_licenses_to_obtain>0)
				{
					hr = server->second.LicenseServer->KeyModuleLicenseObtain(k->first, module_id, key_licenses_to_obtain);
					if (SUCCEEDED(hr)) {licenses_obtained+=key_licenses_to_obtain; k->second.licenses_allocated[module_id]+=key_licenses_to_obtain;}
				}
			}
		}
	}
	return (licenses_obtained == licenses_to_obtain ? S_OK : E_FAIL);
}

// attempts to deallocate licenses on keys that have 
HRESULT CSolimarLicenseMgr::ReleaseLicensesInternal(long module_id, long license_count)
{
	HRESULT hr = S_OK;
	
	ENSURE_INITIALIZED;
	
	if (license_count<=0) return E_INVALIDARG;
	
	SafeMutex mutex(ServerListLock);
	
	long licenses_to_release(license_count);
	long licenses_released(0);
	
	// for each server (backwards)
	for (ServerList::reverse_iterator server = m_servers.rbegin(); server != m_servers.rend(); ++server)
	{
		// foreach key (backwards)
		for (ServerInfo::KeyList::reverse_iterator k = server->second.keys.rbegin(); k != server->second.keys.rend(); ++k)
		{
			if (k->second.KeyPresent && k->second.KeyObtained)
			{
				// if the key has already been obtained
				if (!k->second.KeyObtained)
				{
					try
					{
						hr = server->second.LicenseServer->KeyObtain(k->first);
						if (FAILED(hr)) continue;
						
						k->second.KeyObtained = true;
						
						if (m_lock_keys)
					{
						hr = server->second.LicenseServer->KeyLock(k->first);
						if (FAILED(hr))
						{
							hr = server->second.LicenseServer->KeyRelease(k->first);
							k->second.KeyObtained = false;
							continue;
						}
						else
						{
							k->second.KeyLocked = true;
						}
					}
					}
					catch (_com_error &e) 
					{
						if(SS_RPC_FAILED(e.Error()))
						{
							//Log Message about RPC failure to Key
							SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::ReleaseLicensesInternal() - RPC Error", MT_INFO, LicenseServerError::EC_UNKNOWN);
							SS_GENERATE_AND_DISPATCH_MESSAGE(LicensingMessageStringTable[MessageClientTimeout], MT_INFO, LicenseServerError::EC_CLIENT_TIMEOUT);
						}
					}
				}
				
				long key_licenses_allocated = k->second.licenses_allocated[module_id];
				long key_licenses_to_release = min(licenses_to_release-licenses_released,key_licenses_allocated);
				
				if (key_licenses_to_release>0) 
					hr = server->second.LicenseServer->KeyModuleLicenseRelease(k->first, module_id, key_licenses_to_release);
				if (SUCCEEDED(hr)) 
				{
					licenses_released+=key_licenses_to_release; 
					k->second.licenses_allocated[module_id]-=key_licenses_to_release;
				}
				
				// if the key no longer has any licenses obtained on it, try to release it and unlock it where necessary
				//xxx
			}
		}
	}
	return (licenses_released == licenses_to_release ? S_OK : E_FAIL);
}

// adds a message to the list of unretrieved messages
HRESULT CSolimarLicenseMgr::AddLicensingMessage(LicensingMessage &message)
{
	SafeMutex mutex_messages(MessageListLock);
	licensing_message_cache.push_back(message);
	return S_OK;
}
bool CSolimarLicenseMgr::InViolationPeriod()
{
	SafeMutex mutex(GracePeriodLock);
	time_t timeNow = time(NULL);
	double timeDiff1 = difftime(m_dtGracePeriodStart+(m_dtGracePeriod*60), timeNow);
	double timeDiff2 = difftime(timeNow, m_dtGracePeriodStart+(m_dtGracePeriod*60));
	bool bViolation = GracePeriodHasStarted() && (m_dtGracePeriodStart+(m_dtGracePeriod*60) < (unsigned)timeNow);
	return bViolation;
	//return GracePeriodHasStarted() && (difftime(m_dtGracePeriodStart+m_dtGracePeriod, time(NULL)) > 0);
}
bool CSolimarLicenseMgr::GracePeriodHasStarted()
{
	SafeMutex mutex(GracePeriodLock);
	return m_dtGracePeriodStart != 0;
}
void CSolimarLicenseMgr::StartGracePeriod()
{
	if(GracePeriodHasStarted() == false)	//If not in grace period already, start grace period
	{
		{
		SafeMutex mutex(GracePeriodLock);
		m_dtGracePeriodStart = time(NULL);
		}
		SS_GENERATE_AND_DISPATCH_MESSAGE(LicensingMessageStringTable[MessageGracePeriodStarted], MT_INFO, LicenseServerError::EC_SP_NO_SERVER_RESPONSE);
	}
}
void CSolimarLicenseMgr::StopGracePeriod()
{
	if(GracePeriodHasStarted() == true)
	{
		{
		SafeMutex mutex(GracePeriodLock);
		m_dtGracePeriodStart = 0;
		}
		SS_GENERATE_AND_DISPATCH_MESSAGE(LicensingMessageStringTable[MessageGracePeriodEnded], MT_INFO, 0);
	}
}