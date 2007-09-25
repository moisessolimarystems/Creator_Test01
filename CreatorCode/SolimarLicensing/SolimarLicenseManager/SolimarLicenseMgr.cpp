// SolimarLicenseMgr.cpp : Implementation of CSolimarLicenseMgr

#include "stdafx.h"
#include "SolimarLicenseMgr.h"
#include "..\common\ChallengeResponse.h"
#include "..\common\Version.h"
#include "..\common\SafeMutex.h"
#include "..\common\TimeHelper.h"
#include "..\common\EventLogHelper.h"
#include "..\common\LicensingMessage.h"
#include "..\common\LicenseError.h"
#include "..\common\VersionInfo.h"
#include "..\common\NetworkUtils.h"
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

void OutputFormattedDebugString(const wchar_t *fmt, ...)
{
	va_list list;
	va_start(list, fmt);
	wchar_t buf[1024];
	memset(buf, 0, sizeof(buf));
	_vsnwprintf_s(buf, 1024, fmt, list);
	OutputDebugStringW(buf);
}


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
	LicenseServer(NULL),
	bUseOnlySharedLicenses(false)
{
	;
}

CSolimarLicenseMgr::ServerInfo::ServerInfo(const ServerInfo &s) : 
	LicenseServer(s.LicenseServer)
{
	;
}

CSolimarLicenseMgr::ServerInfo::ServerInfo(_bstr_t servername, bool useOnlySharedLicenses, GITPtr<ISolimarLicenseSvr3> pILicenseServer) : 
	name(servername), 
	bUseOnlySharedLicenses(useOnlySharedLicenses),
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
	MULTI_QI		multiQI		= {&__uuidof(ISolimarLicenseSvr3), NULL, NOERROR};
	
	HRESULT hr = CoCreateInstanceEx(
		__uuidof(CSolimarLicenseSvr), 
		NULL, 
		CLSCTX_REMOTE_SERVER, 
		&serverInfo, 
		1, 
		&multiQI);
	if (SUCCEEDED(hr))
	{
		ISolimarLicenseSvr3 *pILicenseServer = (ISolimarLicenseSvr3*)multiQI.pItf;
		ChallengeResponseHelper CR(	challenge_key_server_thisauthuser_private, 
									sizeof(challenge_key_server_thisauthuser_private)/sizeof(BYTE), 
									challenge_key_server_userauththis_public, 
									sizeof(challenge_key_server_userauththis_public)/sizeof(BYTE));
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
	m_bUsingBackupServers(false),
	ChallengeResponseHelper(challenge_key_manager_userauththis_private, 
							sizeof(challenge_key_manager_userauththis_private)/sizeof(BYTE), 
							challenge_key_manager_thisauthuser_public, 
							sizeof(challenge_key_manager_thisauthuser_public)/sizeof(BYTE))
{
//OutputDebugString(L"CSolimarLicenseMgr::CSolimarLicenseMgr()");	
	HeartbeatThread = new APCTimer(HeartbeatThreadFunction, this, 3*1000 /* 3 seconds */, InitTimerThreadCB, this);
}

CSolimarLicenseMgr::~CSolimarLicenseMgr()
{
	Disconnect();
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
	ChallengeResponse CR(challenge_key_manager_userauththis_private, 
						 sizeof(challenge_key_manager_userauththis_private)/sizeof(BYTE));

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
	return Connect2(server, VARIANT_FALSE, VARIANT_FALSE);
}
STDMETHODIMP CSolimarLicenseMgr::Connect2(BSTR server, VARIANT_BOOL bUseOnlySharedLicenses, VARIANT_BOOL bUseAsBackup)
{
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::Connect2 (server: %s, bUseOnlySharedLicenses: %s, bUseAsBackup: %s) - Enter", (wchar_t*)server, bUseOnlySharedLicenses ? L"true" : L"false", bUseAsBackup ? L"true" : L"false");
//OutputDebugString(tmpbuf); 
	HRESULT hr = S_OK;
	{
	SafeMutex mutex(ServerListLock);
	ServerList::iterator serverIt = m_servers.find(server);
	
	if(serverIt != m_servers.end())
		return hr;	//Return is already connected to the server
	
	serverIt = m_backupServers.find(server);
	if(serverIt != m_backupServers.end())
		return hr;	//Return is already connected to the server
	}
	

	// Try to create an ISolimarLicenseServer proxy to the server
	COSERVERINFO	serverInfo	= {0, server, NULL, 0};
	MULTI_QI		multiQI		= {&__uuidof(ISolimarLicenseSvr3), NULL, NOERROR};
	hr = CoCreateInstanceEx(
		__uuidof(CSolimarLicenseSvr), 
		NULL, 
		CLSCTX_REMOTE_SERVER, 
		&serverInfo, 
		1, 
		&multiQI);
	if (SUCCEEDED(hr))
	{	
		ISolimarLicenseSvr3 *pILicenseServer = (ISolimarLicenseSvr3*)multiQI.pItf;
		ChallengeResponseHelper CR(	challenge_key_server_thisauthuser_private, 
									sizeof(challenge_key_server_thisauthuser_private)/sizeof(BYTE), 
									challenge_key_server_userauththis_public, 
									sizeof(challenge_key_server_userauththis_public)/sizeof(BYTE));
		// try to authenticate the license server
		hr = CR.AuthenticateServer(pILicenseServer);
		if (SUCCEEDED(hr))
		{
			// let the license server authenticate this manager
			hr = CR.AuthenticateToServer(pILicenseServer);
			if (SUCCEEDED(hr))
			{
				SafeMutex mutex(ServerListLock);
				if(bUseAsBackup == VARIANT_TRUE)
					m_backupServers[server] = ServerInfo(server, bUseOnlySharedLicenses==VARIANT_TRUE, pILicenseServer);
				else	//if(bUseAsBackup == VARIANT_FALSE)
					m_servers[server] = ServerInfo(server, bUseOnlySharedLicenses==VARIANT_TRUE, pILicenseServer);
			}
		}
		if (FAILED(hr))
		{
			pILicenseServer->Release();
		}
	}

	m_initialized = false;

//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::Connect2 leave - hr: 0x%08x", hr);
//OutputDebugString(tmpbuf); 
	return hr;
}

STDMETHODIMP CSolimarLicenseMgr::Disconnect()
{
//OutputDebugString(L"CSolimarLicenseMgr::Disconnect() - Enter");
	SafeMutex mutex(ServerListLock);

	//Free up all the licenses
	for (ModuleLicenseMap::iterator m = m_allocated_licenses.begin(); m != m_allocated_licenses.end(); ++m)
		ModuleLicenseRelease(m->first, m->second);

	while(!m_productkeys.empty())
		m_productkeys.erase(m_productkeys.begin());

	VARIANT vtKeyList;
	HRESULT hr(S_OK);
	try
	{
		// for each server
		for (ServerList::iterator server = m_servers.begin(); server != m_servers.end(); ++server)
		{
			SS_SLSERVER_FTCALL_HR(server->second, KeyEnumerate, (&vtKeyList), hr);
			if (SUCCEEDED(hr) && (vtKeyList.vt & (VT_ARRAY | VT_VARIANT)))
			{
				VARIANT *pvtKeyIdent;
				if (SUCCEEDED(SafeArrayAccessData(vtKeyList.parray, (void**)&pvtKeyIdent)))
				{
					FreeAllKeys(&(server->second), pvtKeyIdent, vtKeyList.parray->rgsabound[0].cElements, false);
				}
			}
		}

		// for each server
		for (ServerList::iterator server = m_backupServers.begin(); server != m_backupServers.end(); ++server)
		{
			SS_SLSERVER_FTCALL_HR(server->second, KeyEnumerate, (&vtKeyList), hr);
			if (SUCCEEDED(hr) && (vtKeyList.vt & (VT_ARRAY | VT_VARIANT)))
			{
				VARIANT *pvtKeyIdent;
				if (SUCCEEDED(SafeArrayAccessData(vtKeyList.parray, (void**)&pvtKeyIdent)))
				{
					FreeAllKeys(&(server->second), pvtKeyIdent, vtKeyList.parray->rgsabound[0].cElements, false);
				}
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

	while(!m_servers.empty())
		m_servers.erase(m_servers.begin());

	while(!m_backupServers.empty())
		m_backupServers.erase(m_backupServers.begin());
	
	m_ui_level = UI_IGNORE;
	m_initialized = false;
//OutputDebugString(L"CSolimarLicenseMgr::Disconnect() - Leave");	
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
	return Initialize3(L"", product, prod_ver_major, prod_ver_minor, single_key, specific_single_key_ident, lock_keys, auto_ui_level, grace_period_minutes, VARIANT_FALSE, VARIANT_FALSE);
}


//unsigned long grace_period_minutes - How long license can still be validated after there is a violation because of no keys on the system.
//VARIANT_BOOL app_instance_lock_key - Will lock the first base on each license server.  Will lock all add-on key, all bases keys on the system must match.
//VARIANT_BOOL bypass_remote_key_restriction - True means the restriction of remote license managers using non-remote keys has been lifted.
STDMETHODIMP CSolimarLicenseMgr::Initialize3(	BSTR application_instance, 
												long product, 
												long prod_ver_major, 
												long prod_ver_minor, 
												VARIANT_BOOL single_key, 
												BSTR specific_single_key_ident, 
												VARIANT_BOOL lock_keys, 
												long auto_ui_level, 
												unsigned long grace_period_minutes,
												VARIANT_BOOL app_instance_lock_key,
												VARIANT_BOOL bypass_remote_key_restriction)
{
//OutputDebugString(L"CSolimarLicenseMgr::Initialize3() - Enter");
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::Initialize3 (application_instance: %s, app_instance_lock_key: %s) - Enter", (wchar_t*)application_instance, app_instance_lock_key ? L"true" : L"false");
//OutputDebugString(tmpbuf);
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
	m_bLockKeyByAppInstance = app_instance_lock_key == VARIANT_TRUE ? true : false;
	m_bBypassRemoteKeyRestrictions = bypass_remote_key_restriction == VARIANT_TRUE ? true : false;
	m_applicationInstance = application_instance;
	
	SafeMutex mutex(ServerListLock);
	
	hr = RefreshLicenses();
	
	if (FAILED(hr)) 
	{
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::Initialize3 leave 0 - hr: 0x%08x", hr);
//OutputDebugString(tmpbuf); 
		return hr;
	}

	return RefreshKeyList(true/*Log Error*/);
	//FOR TESTING
//hr = RefreshKeyList(true);
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::Initialize3 leave 1 - hr: 0x%08x", hr);
//OutputDebugString(tmpbuf); 
//return hr;
}

STDMETHODIMP CSolimarLicenseMgr::ValidateLicense(VARIANT_BOOL *license_valid)
{
//OutputDebugString(L"<CSolimarLicenseMgr::ValidateLicense() - Enter");
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
//OutputDebugString(L">CSolimarLicenseMgr::ValidateLicense() - Leave");
	return hr;
}
HRESULT CSolimarLicenseMgr::ValidateLicenseInternal(VARIANT_BOOL *license_valid, bool use_back_up_on_error)
{
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::ValidateLicenseInternal (use_back_up_on_error: %s)", use_back_up_on_error ? L"true" : L"false");
//OutputDebugString(tmpbuf);

	HRESULT hr = S_OK;
	ENSURE_INITIALIZED;
	SafeMutex mutex(ServerListLock);
	bool bAlreadyUsingBackUpServers = m_bUsingBackupServers;
	m_bUsingBackupServers = false;	//Try to Re-use non-Backup Servers.
	for(;;)
	{
		//Try to reuse non-backup servers...

		*license_valid = VARIANT_FALSE; 
		hr = RefreshLicenses();
		if (SUCCEEDED(hr)) 
		{
			ModuleLicenseMap outstanding_licenses;
			hr = ValidateLicenseCache(outstanding_licenses);
			if (SUCCEEDED(hr)) 
			{
				// determine if any licenses aren't obtained by keys
				*license_valid = VARIANT_TRUE;
				for (ModuleLicenseMap::iterator module = outstanding_licenses.begin(); module != outstanding_licenses.end(); ++module)
				{
					if (module->second>0)
					{
						*license_valid = VARIANT_FALSE;
						break;
					}
				}
			}
		}

		if(*license_valid == VARIANT_TRUE)
		{
			break;
		}
		else if(*license_valid == VARIANT_FALSE)
		{
			if(m_bUsingBackupServers || !use_back_up_on_error)
			{
				//License failed using active server and backup server, fall to grace period section...
				break;
			}
			else //if(!m_bUsingBackupServers)	
			{
				//try validation again with backup servers
				m_bUsingBackupServers = true;

				if(!bAlreadyUsingBackUpServers)
				{
					//Check to see if backup server is specified...
					if(!m_backupServers.empty())
					{
						//
						//Move to Backup Servers, log message

						//currently there is a limit of only one Key Server & Backup Server
						_bstr_t bstr_KeyServer = L"";
						_bstr_t bstr_BackupKeyServer = L"";

						if(!m_servers.empty())
							bstr_KeyServer = (*m_servers.begin()).second.name;
						if(!m_backupServers.empty())
							bstr_BackupKeyServer = (*m_backupServers.begin()).second.name;

						int messageID = MessageKeysUseBackupKeyServer;
						HRESULT messageHr = LicenseServerError::EHR_KEY_USE_BACKUP_KEY_SERVER;
						wchar_t licensing_message_buffer[256];
						swprintf_s(	licensing_message_buffer,
									sizeof(licensing_message_buffer)/sizeof(wchar_t),
									LicensingMessageStringTable[messageID],
									(wchar_t*)bstr_KeyServer, 
									(wchar_t*)bstr_BackupKeyServer);
						_variant_t vtTimestamp;
						_variant_t vtExpirationDate;
						long lTrialHours = 0, lKeyStatus=0;
						SS_GENERATE_NOW_TO_VARIANT_TIME_DATE(vtTimestamp);
						SS_GENERATE_NOW_TO_VARIANT_TIME_DATE(vtExpirationDate);

						// Add messages to License Server queued list
						AddLicensingMessage(
							LicensingMessage(
								std::wstring((wchar_t*)bstr_KeyServer),	//std::wstring &server
								std::wstring(L""),		//std::wstring &key
								vtTimestamp,	//VARIANT msg_time
								MT_WARNING,								//DWORD msg_type
								messageID,				//DWORD msg_id
								std::wstring(licensing_message_buffer),  //std::wstring &msg
								messageHr,										//HRESULT hr
								lKeyStatus,						//DWORD status
								vtExpirationDate,						//VARIANT expire_date
								lTrialHours								//DWORD trial_hours
							)
						);

						SS_GENERATE_AND_DISPATCH_MESSAGE(LicenseServerError::GetErrorMessage(messageHr).c_str(), MT_INFO, messageHr);
					}
				}
			}
		}
	}

	//Grace Period Section
	if(*license_valid == VARIANT_FALSE)
	{
		if(use_back_up_on_error)
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
		if(bAlreadyUsingBackUpServers && !m_bUsingBackupServers)
		{
			//Move to Active Servers, log message not using backservers anymore...
			_bstr_t bstr_KeyServer = L"";
			_bstr_t bstr_BackupKeyServer = L"";

			if(!m_servers.empty())
				bstr_KeyServer = (*m_servers.begin()).second.name;
			if(!m_backupServers.empty())
				bstr_BackupKeyServer = (*m_backupServers.begin()).second.name;

			int messageID = MessageKeysRestoreKeyServer;
			HRESULT messageHr = LicenseServerError::EHR_KEY_RESTORED_KEY_SERVER;
			wchar_t licensing_message_buffer[256];
			swprintf_s(	licensing_message_buffer,
						sizeof(licensing_message_buffer)/sizeof(wchar_t),
						LicensingMessageStringTable[messageID],
						(wchar_t*)bstr_KeyServer, 
						(wchar_t*)bstr_BackupKeyServer);
			_variant_t vtTimestamp;
			_variant_t vtExpirationDate;
			long lTrialHours = 0, lKeyStatus=0;
			SS_GENERATE_NOW_TO_VARIANT_TIME_DATE(vtTimestamp);
			SS_GENERATE_NOW_TO_VARIANT_TIME_DATE(vtExpirationDate);

			// Add messages to License Server queued list
			AddLicensingMessage(
				LicensingMessage(
					std::wstring((wchar_t*)bstr_KeyServer),	//std::wstring &server
					std::wstring(L""),		//std::wstring &key
					vtTimestamp,	//VARIANT msg_time
					MT_WARNING,								//DWORD msg_type
					messageID,				//DWORD msg_id
					std::wstring(licensing_message_buffer),  //std::wstring &msg
					messageHr,										//HRESULT hr
					lKeyStatus,						//DWORD status
					vtExpirationDate,						//VARIANT expire_date
					lTrialHours								//DWORD trial_hours
				)
			);

			SS_GENERATE_AND_DISPATCH_MESSAGE(LicenseServerError::GetErrorMessage(messageHr).c_str(), MT_INFO, messageHr);
		}
		StopGracePeriod();
	}
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::ValidateLicenseInternal return: 0x%08x", hr);
//OutputDebugString(tmpbuf);
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
	
	KeySpec::Module &module(m_keyspec.products[m_product][module_id]);
	bool bModuleIsSharable = module.isSharable;

	ServerList::iterator server = m_bUsingBackupServers ? m_backupServers.begin() : m_servers.begin();
	ServerList::iterator serverEnd = m_bUsingBackupServers ? m_backupServers.end() : m_servers.end();

	// foreach server
	for (;server != serverEnd; ++server)
	{
		// if (server is set to use only sharable licenses and module is sharable ) or (server is set to use all licenses)
		if(((*server).second.bUseOnlySharedLicenses && bModuleIsSharable) || !(*server).second.bUseOnlySharedLicenses)
		{
			// foreach key
			for (ServerInfo::KeyList::iterator key = server->second.keys.begin(); key != server->second.keys.end(); ++key)
			{
				if (key->second.KeyPresent)
				{
					long c = key->second.licenses_total[module_id];
					// if one of the licenses is set to 'unlimited', retain this fact
					if (*count>=0x7FFFFFFF || c>=0x7FFFFFFF || *count+c>=0x7FFFFFFF)
						*count = 0x7FFFFFFF;
					else
						*count += c;
				}
			}
		}
	}
	
//wchar_t debug_buf[1024];
//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::ModuleLicenseTotal module_id=%d, count=%d)", module_id, *count);
//debug_buf[1023] = 0;
//OutputDebugStringW(debug_buf);

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
		return LicenseServerError::EHR_LICENSE_INSUFFICIENT;

	SafeMutex mutex(ServerListLock);
	
	// record that the licenses were obtained
	m_allocated_licenses[module_id] += count;
	// perform the license allocation
	VARIANT_BOOL licensing_valid = VARIANT_FALSE;
	hr = ValidateLicenseInternal(&licensing_valid, false);
	if (FAILED(hr) || licensing_valid==VARIANT_FALSE) 
		m_allocated_licenses[module_id] -= count;

	return (licensing_valid == VARIANT_TRUE ? S_OK : hr);
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
	ServerList::iterator server = m_bUsingBackupServers ? m_backupServers.begin() : m_servers.begin();
	ServerList::iterator serverEnd = m_bUsingBackupServers ? m_backupServers.end() : m_servers.end();

	// foreach server
	for (;server != serverEnd; ++server)
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
						VARIANT_BOOL key_license_valid(VARIANT_FALSE);
						//KeyValidateLicense Calls KeyIsPresent() & KeyIsActive()
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
		return hr;

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
		return LicenseServerError::EHR_LICENSE_INSUFFICIENT;
	}

	SafeMutex mutex(ServerListLock);
	
	// perform the license allocation
	VARIANT_BOOL licensing_valid = VARIANT_FALSE;
	hr = ValidateLicenseInternal(&licensing_valid, false);
	if (SUCCEEDED(hr) && licensing_valid==VARIANT_FALSE) 
		hr = E_FAIL;

	long licenseToDecrement = license_count;
	if(SUCCEEDED(hr))
	{
		ServerList::iterator server = m_bUsingBackupServers ? m_backupServers.begin() : m_servers.begin();
		ServerList::iterator serverEnd = m_bUsingBackupServers ? m_backupServers.end() : m_servers.end();

		// foreach server
		for (;server != serverEnd && licenseToDecrement>0; ++server)
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

	ServerList::iterator server = m_bUsingBackupServers ? m_backupServers.begin() : m_servers.begin();
	ServerList::iterator serverEnd = m_bUsingBackupServers ? m_backupServers.end() : m_servers.end();

	// foreach server
	for (;server != serverEnd; ++server)
	{
		// if this server's key list contains the key in question
		if (server->second.keys.find(key_ident)!=server->second.keys.end())
		{
			return true;
		}
	}
	return false;
}
bool CSolimarLicenseMgr::ProductKey(_bstr_t key_ident)
{
	if (key_ident.length()==0) 
		return true;

	return m_productkeys.find(key_ident) != m_productkeys.end();
}

STDMETHODIMP CSolimarLicenseMgr::GetVersionLicenseManager(long* p_ver_major, long* p_ver_minor, long* p_ver_build)
{
	*p_ver_major = MAJOR_REVISION_NUMBER;
	*p_ver_minor = MINOR_REVISION_NUMBER;
	*p_ver_build = BUILD_NUMBER;
	return S_OK;
}
STDMETHODIMP CSolimarLicenseMgr::GetVersionLicenseServer(BSTR server, long* p_ver_major, long* p_ver_minor, long* p_ver_build)
{
	HRESULT hr = S_OK;

	ServerList::iterator serverIt = m_servers.find(server);
	bool bFoundServer = serverIt != m_servers.end();
	if(!bFoundServer)
	{
		serverIt = m_backupServers.find(server);
		bFoundServer = serverIt != m_backupServers.end();
	}
	if(bFoundServer)
	{
		//Use the existing connection to the machine and retrieve the version.
		hr = serverIt->second.LicenseServer->GetVersionLicenseServer(p_ver_major, p_ver_minor, p_ver_build);
		if(FAILED(hr))
		{
			*p_ver_major = 0;
			*p_ver_minor = 0;
			*p_ver_build = 0;
		}
	}
	else
	{
		//Try to contact the machine and retrieve the version.

		// Try to create an ISolimarLicenseServer proxy to the server
		COSERVERINFO	serverInfo	= {0, server, NULL, 0};
		MULTI_QI		multiQI		= {&__uuidof(ISolimarLicenseSvr3), NULL, NOERROR};

		hr = CoCreateInstanceEx(
			__uuidof(CSolimarLicenseSvr), 
			NULL, 
			CLSCTX_REMOTE_SERVER, 
			&serverInfo, 
			1, 
			&multiQI);
		if (SUCCEEDED(hr))
		{	
			ISolimarLicenseSvr3 *pILicenseServer = (ISolimarLicenseSvr3*)multiQI.pItf;
			ChallengeResponseHelper CR(	challenge_key_server_thisauthuser_private, 
										sizeof(challenge_key_server_thisauthuser_private)/sizeof(BYTE), 
										challenge_key_server_userauththis_public, 
										sizeof(challenge_key_server_userauththis_public)/sizeof(BYTE));
			// try to authenticate the license server
			hr = CR.AuthenticateServer(pILicenseServer);
			if (SUCCEEDED(hr))
			{
				// let the license server authenticate this manager
				hr = CR.AuthenticateToServer(pILicenseServer);
				if (SUCCEEDED(hr))
				{
					hr = pILicenseServer->GetVersionLicenseServer(p_ver_major, p_ver_minor, p_ver_build);
					if(FAILED(hr))
					{
						*p_ver_major = 0;
						*p_ver_minor = 0;
						*p_ver_build = 0;
					}
				}
			}
			if (FAILED(hr))
				pILicenseServer->Release();
		}
	}
	return hr;
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
			ServerList::iterator server = m_bUsingBackupServers ? m_backupServers.begin() : m_servers.begin();
			ServerList::iterator serverEnd = m_bUsingBackupServers ? m_backupServers.end() : m_servers.end();

			// foreach server
			for (;server != serverEnd; ++server)
			{
				ILicensingMessage *pServerLicensingMessage = 0;
				_variant_t vtServerMessageList;
				try
				{
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
				std::wstring keyName = m->key_ident;
				if (!ProductKey(m->key_ident.c_str()))
				{
					m = licensing_message_cache.erase(m);
				}
				else
				{
					++m;
				}
			}
			*pvtMessageList = licensing_message_cache;

			//Dispatch all the messages before returning
			//DispatchLicenseMessageList(VARIANT_FALSE);

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
	ServerList::iterator server = m_bUsingBackupServers ? m_backupServers.begin() : m_servers.begin();
	ServerList::iterator serverEnd = m_bUsingBackupServers ? m_backupServers.end() : m_servers.end();

	// foreach server
	for (;server != serverEnd; ++server)
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

	//Ignore Timedate stamp for writing to event log, unnecessary because event log
	//keeps track of Timedate anyways.
	/*
	_variant_t vtStrTimestamp = vtTimestamp;
	vtStrTimestamp.ChangeType(VT_BSTR);
	_bstr_t str_timestamp = vtStrTimestamp.bstrVal;
	_bstr_t str_error_message;	// look in i:\chris r\samplehr.txt
	//xxx implement GetErrorMessage such that it returns the right error message?
	str_error_message = LicenseServerError::GetErrorMessage(error).c_str();
	switch (message_type)
	{
	case MT_INFO:
		_snwprintf(event_log_msg, MAX_MESSAGE_SIZE, L"Solimar Systems, Inc.\r\nProduct Licensing Status Message\r\n%s\r\nKey: %s\r\n\r\n%s",
			str_timestamp.GetBSTR(),
			key_ident,
			message);
		break;
	case MT_ERROR:
		_snwprintf(event_log_msg, MAX_MESSAGE_SIZE, L"Solimar Systems, Inc.\r\nProduct Licensing Error Message\r\n%s\r\nKey: %s\r\n%08x %s\r\n\r\n%s",
			str_timestamp.GetBSTR(),
			key_ident,
			error,
			str_error_message,
			message);
		break;
	}
	*/

	_bstr_t str_error_message;
	//xxx implement GetErrorMessage such that it returns the right error message?
	str_error_message = LicenseServerError::GetErrorMessage(error).c_str();
	switch (message_type)
	{
	case MT_INFO:
		_snwprintf_s(event_log_msg, sizeof(event_log_msg)/sizeof(wchar_t), L"Solimar Systems, Inc.\r\nProduct Licensing Status Message\r\n\r\nKey: %s\r\n\r\n%s",
			key_ident,
			message);
		break;
	case MT_ERROR:
		_snwprintf_s(event_log_msg, sizeof(event_log_msg)/sizeof(wchar_t), L"Solimar Systems, Inc.\r\nProduct Licensing Error Message\r\n\r\nKey: %s\r\n%08x %s\r\n\r\n%s",
			key_ident,
			error,
			str_error_message,
			message);
		break;
	}
	
	unsigned int event_type = EVENTLOG_INFORMATION_TYPE;
	if (error & 0x8000000) event_type = EVENTLOG_ERROR_TYPE;
	//EventLogHelper::WriteEventLog(L"Solimar License Server", event_log_msg, message_type);
	LicenseServerError::WriteEventLog(event_log_msg, message_type);
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
//wchar_t debug_buf[1024];
//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::RefreshKeyList() - Enter");
//OutputDebugStringW(debug_buf);	
	ENSURE_INITIALIZED;
	
	SafeMutex mutex(ServerListLock);
		
	bool found_keys = false;
	bool bGracePeriodSeen = false;

	ServerList::iterator server = m_bUsingBackupServers ? m_backupServers.begin() : m_servers.begin();
	ServerList::iterator serverEnd = m_bUsingBackupServers ? m_backupServers.end() : m_servers.end();

	// foreach server
	for (;server != serverEnd; ++server)
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
					if(m_bLockKeyByAppInstance)
					{
						//Validates that all the keys "match"
						//	All the base keys match module by module...
						//	All the different Key configurations (Add-on & Base) have the same number of Application Instances
						hr = ValidateKeyList(&(server->second), pvtKeyIdent, vtKeyList.parray->rgsabound[0].cElements);
						if(FAILED(hr))
						{
							if(_bLogError)
							{
								//Log error, continue to next server...
								wchar_t licensing_message_buffer[256];
								int licMessage = 0;
								SS_CONVERT_LICENSE_HR_TO_LICENSE_MESSAGE(hr, licensing_message_buffer, licMessage, server->first);

								_variant_t vtTimestamp;
								_variant_t vtExpirationDate;
								long lTrialHours = 0, lKeyStatus=0;
								SS_GENERATE_NOW_TO_VARIANT_TIME_DATE(vtTimestamp);
								SS_GENERATE_NOW_TO_VARIANT_TIME_DATE(vtExpirationDate);

								// Add messages to queued list
								AddLicensingMessage(
									LicensingMessage(
										std::wstring((wchar_t*)server->first),	//std::wstring &server
										std::wstring(L""),		//std::wstring &key
										vtTimestamp,	//VARIANT msg_time
										MT_WARNING,								//DWORD msg_type
										licMessage,				//DWORD msg_id
										std::wstring(licensing_message_buffer),  //std::wstring &msg
										hr,										//HRESULT hr
										lKeyStatus,						//DWORD status
										vtExpirationDate,						//VARIANT expire_date
										lTrialHours								//DWORD trial_hours
									)
								);

								SS_GENERATE_AND_DISPATCH_MESSAGE(LicenseServerError::GetErrorMessage(hr).c_str(), MT_INFO, hr);
							}
							continue;
						}
					}


					if(m_bLockKeyByAppInstance)
					{
						hr = LockOneOfEachKeyConfiguration(&(server->second), pvtKeyIdent, vtKeyList.parray->rgsabound[0].cElements, _bLogError);
						if(FAILED(hr))
						{
							if(_bLogError)
							{
								//Log error, continue to next server...
								wchar_t licensing_message_buffer[256];
								int licMessage = 0;
								SS_CONVERT_LICENSE_HR_TO_LICENSE_MESSAGE(hr, licensing_message_buffer, licMessage, server->first);
								_variant_t vtTimestamp;
								_variant_t vtExpirationDate;
								long lTrialHours = 0, lKeyStatus=0;
								SS_GENERATE_NOW_TO_VARIANT_TIME_DATE(vtTimestamp);
								SS_GENERATE_NOW_TO_VARIANT_TIME_DATE(vtExpirationDate);

								// Add messages to queued list
								AddLicensingMessage(
									LicensingMessage(
										std::wstring((wchar_t*)server->first),	//std::wstring &server
										std::wstring(L""),		//std::wstring &key
										vtTimestamp,	//VARIANT msg_time
										MT_WARNING,								//DWORD msg_type
										licMessage,				//DWORD msg_id
										std::wstring(licensing_message_buffer),  //std::wstring &msg
										hr,										//HRESULT hr
										lKeyStatus,						//DWORD status
										vtExpirationDate,						//VARIANT expire_date
										lTrialHours								//DWORD trial_hours
									)
								);
								SS_GENERATE_AND_DISPATCH_MESSAGE(LicenseServerError::GetErrorMessage(hr).c_str(), MT_INFO, hr);
							}
						}
					}
					else //if(!m_bLockKeyByAppInstance)
					{
						hr = AssociateAppInstanceToBaseKey(&(server->second), pvtKeyIdent, vtKeyList.parray->rgsabound[0].cElements, _bLogError);
						if(FAILED(hr))
						{
							if(_bLogError)
							{
								//Log error, continue to next server...
								wchar_t licensing_message_buffer[256];
								int licMessage = 0;
								SS_CONVERT_LICENSE_HR_TO_LICENSE_MESSAGE(hr, licensing_message_buffer, licMessage, server->first);
								_variant_t vtTimestamp;
								_variant_t vtExpirationDate;
								long lTrialHours = 0, lKeyStatus=0;
								SS_GENERATE_NOW_TO_VARIANT_TIME_DATE(vtTimestamp);
								SS_GENERATE_NOW_TO_VARIANT_TIME_DATE(vtExpirationDate);

								// Add messages to queued list
								AddLicensingMessage(
									LicensingMessage(
										std::wstring((wchar_t*)server->first),	//std::wstring &server
										std::wstring(L""),		//std::wstring &key
										vtTimestamp,	//VARIANT msg_time
										MT_WARNING,								//DWORD msg_type
										licMessage,				//DWORD msg_id
										std::wstring(licensing_message_buffer),  //std::wstring &msg
										hr,										//HRESULT hr
										lKeyStatus,						//DWORD status
										vtExpirationDate,						//VARIANT expire_date
										lTrialHours								//DWORD trial_hours
									)
								);
								SS_GENERATE_AND_DISPATCH_MESSAGE(LicenseServerError::GetErrorMessage(hr).c_str(), MT_INFO, hr);
							}
						}
					}

					//Cycle through all the keys, calculate unlimitedness
					SetUnlimitedModulesOnKeys(&(server->second), pvtKeyIdent, vtKeyList.parray->rgsabound[0].cElements, _bLogError);
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

	found_keys = SUCCEEDED(hr);

	// remove any keys that are not present and have no licenses obtained
	if (FAILED(hr))
		RemoveObsoleteKeysFromCache();
	else
		hr = RemoveObsoleteKeysFromCache();


	if (FAILED(hr)) return hr;
    
	// S_OK if some keys were found
	// S_FALSE if no keys are found
	// Error HRESULT if there was an error
	return (SUCCEEDED(hr) ? (found_keys ? S_OK : S_FALSE) : hr);
}

//if m_bLockKeyByAppInstance then 
//	-all base keys on the system must match module by module
HRESULT  CSolimarLicenseMgr::ValidateKeyList(ServerInfo* pServerInfo, VARIANT *pvtKeyList, long count)
{
	HRESULT hr(S_OK);

//wchar_t debug_buf[1024];
//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::ValidateKeyList() - Enter");
//OutputDebugStringW(debug_buf);	

	if(!m_bLockKeyByAppInstance)
		return hr;	//Only m_bLockKeyByAppInstance must have all keys match

	//First Pass
	_bstr_t app_instance_key_ident = L"";
	bool bAssociatedAppInstanceToBaseKey = false;
	bool bAppInstanceAlreadyAssociatedToBaseKey = false;
	std::map<unsigned int, long> lockKeyModuleByModuleMap;	//This map stores all modules that all base keys must match
	std::map<_bstr_t, long> refCountOfAddonsModuleByModuleMap;
	_bstr_t refCountOfBaseModuleByModule = L"";
	long refCountOfBaseModuleByModuleCount = 0;
	bool bUnInitializedLockKeyModuleByModuleMap = true;
	//first cycle through all keys, find a base key and associate app instance with the base key if possible.
	for (int i = 0; i<count; ++i)
	{
		_bstr_t key_ident = pvtKeyList[i].bstrVal;
		VARIANT vtKeyProductID, vtKeyProductVersion;
		// check that the key is present and valid
		VARIANT_BOOL key_license_valid(VARIANT_FALSE);
		
		//KeyValidateLicense Calls KeyIsPresent() & KeyIsActive()
		hr = pServerInfo->LicenseServer->KeyValidateLicense(key_ident, &key_license_valid);
		if (FAILED(hr) || key_license_valid==VARIANT_FALSE) {hr = S_OK; continue;}

		// check that the key has the requisite product version and etc.
		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product Version"].id, &vtKeyProductVersion);
		if (FAILED(hr)) {hr = S_OK; continue;}
		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product ID"].id, &vtKeyProductID);
		if (FAILED(hr)) {hr = S_OK; continue;}

		// if the product id and product version requirements are satisfied
		if (Version::TinyVersion(vtKeyProductVersion.uiVal,0) >= Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0)) && 
			m_keyspec.products[m_product].id==vtKeyProductID.uiVal)
		{
			VARIANT vtKeyType;
			hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Key Type"].id, &vtKeyType);
			if (FAILED(hr)) {hr = S_OK; continue;}
			unsigned short key_type = vtKeyType.uiVal;
			if(m_bLockKeyByAppInstance)
			{
				typedef enum {
					KEYNone			=0x00000000,  //reserved state
					KEYAddon		   =1,
					KEYBase			=2,
					KEYReplacement	=3,
					KEYRelicense	=4,
					KEYInventory	=5,
					KEYBackup		=6,
					KEYEmergency	=7,
					//KEYSolimar	=8,
					KEYCustom		=9,
					KEYLoan			=10,
					KEYDemo			=11,
					KEYRental		=12,
					KEYDevelopment	=13,
					KEYReserved		=14,
				} KeyTypeFlagX;
				for (KeySpec::Product::data_list_t::iterator module = m_keyspec.products[m_product].data.begin(); module != m_keyspec.products[m_product].data.end(); ++module)
				{
					if (module->isLicense)
					{
						long module_id = static_cast<long>(module->id);
						long licenses_total(0), licenses_allocated(0), licenses_inuse(0);

						if(!module->isSharable)
						{
							// get the total number of licenses
							long licenses_total;
							hr = pServerInfo->LicenseServer->KeyModuleLicenseTotal(key_ident, module_id, &licenses_total);
							lockKeyModuleByModuleMap[module->id] = licenses_total;
						}
					}
				}

				//Convert to a string of modules to compare
				wchar_t wModuleString[1024];
				wchar_t wTmpBuf[64];

				//Treate all non addon keys the same
				//	key_type == KEYDevelopment || key_type == KEYBase || key_type == KEYBackup
				_snwprintf_s(wModuleString, sizeof(wModuleString)/sizeof(wchar_t), L"%d", (key_type==KEYAddon) ? 1 : 0);
				wModuleString[1023] = 0;
				
				for(std::map<unsigned int, long>::iterator modIt=lockKeyModuleByModuleMap.begin();
					modIt!=lockKeyModuleByModuleMap.end();
					modIt++)
				{
					_snwprintf_s(wTmpBuf, sizeof(wTmpBuf)/sizeof(wchar_t), L"|%d", modIt->second==0x7FFFFFFF ? -1 : modIt->second);
					wTmpBuf[63] = 0;
					wcscat_s(wModuleString, sizeof(wModuleString)/sizeof(wchar_t), wTmpBuf);
					wModuleString[1023] = 0;
				}

				if(key_type==KEYAddon)
					refCountOfAddonsModuleByModuleMap[wModuleString]++;
				else	//Treate all other keys types as base
				{
					if(refCountOfBaseModuleByModuleCount == 0)
					{
						refCountOfBaseModuleByModule = wModuleString;
						refCountOfBaseModuleByModuleCount++;
					}
					else if(wcscmp(refCountOfBaseModuleByModule, wModuleString) == 0)
					{
						refCountOfBaseModuleByModuleCount++;
					}
					else
					{
						hr = LicenseServerError::EHR_KEY_BASE_NOT_MATCHING;
						//hr = E_FAIL; //BASE KEYS DO NOT MATCH - Throw custom hr
						break;
					}

				}
			}
		}
	}

	if(SUCCEEDED(hr))
	{
		//Verify that all keys are the way they are suppose to be; All reference counts should be equal for Base and Addon Keys...
		for(std::map<_bstr_t, long>::iterator refIt=refCountOfAddonsModuleByModuleMap.begin();
			refIt!=refCountOfAddonsModuleByModuleMap.end();
			refIt++)
		{
			if(refCountOfBaseModuleByModuleCount != 0 && refCountOfBaseModuleByModuleCount != refIt->second)
			{
				hr = LicenseServerError::EHR_KEY_NOT_MATCHING_INSTANCES;
				break;
			}
		}
	}

	if(FAILED(hr))
	{
		//go through and clear all caches..
		FreeAllKeys(pServerInfo, pvtKeyList, count, false);
	}

//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::ValidateKeyList() - Leave");
//OutputDebugStringW(debug_buf);
	return hr;
}



// Side Effect: if can lock an application instance with each keys configuration (unique by the modules the keys are licensed for), will
//				adds all locked keys to the cache
// Gives preferences that a License Manager connecting to a Local Server (same computer) will try to lock a key with no remote access (Application Instance == 0)
// and a Remote License Manager connecting to a Remote Server (different computer) will try to lock a key with remote access (Application Instance >= 1)
HRESULT CSolimarLicenseMgr::LockOneOfEachKeyConfiguration(ServerInfo* pServerInfo, VARIANT *pvtKeyList, long count, bool bLogError)
{
	HRESULT hr(S_OK);
//wchar_t debug_buf[1024];
//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::LockOneOfEachKeyConfiguration() - Enter");
//OutputDebugStringW(debug_buf);
	if(!m_bLockKeyByAppInstance)
		return E_FAIL;	//Only m_bLockKeyByAppInstance must have all keys match

	//Assume ValidateKeyList has been called and succeeded
	//	Therefore: Only one type of key configuration for base keys...
	//	Therefore: An equal number application instance per key configuration for addon keys...

	//First loop - cycle through all keys, and note which application instance is tied to all base keys and application instance keys

	typedef std::map<_bstr_t, _bstr_t> _bstr_tMap;	//KeyInfo is a map<[keyname], [keytype]>
	
	//A keytype is a module by module look at the key taking Add-On/Base into account.
	std::map<_bstr_t, _bstr_tMap> appInstanceToKeyInfoMap;	//map<[applicationInstance], _bstr_tMap>
	_bstr_tMap freeAppInstanceKeyFirstPrefList;	//list of free keys, first preference
	_bstr_tMap freeAppInstanceKeySecondtPrefList;	//list of free keys, second preference
	std::list<_bstr_t> keyTypeList;	//List of all known keytypes
	bool bLockedBaseKey = false;
	_bstr_t baseKeyType = _bstr_t(L"");	//there should only be one base keytype

	bool bIsLocalServer = IsLocalMachine(pServerInfo->name);

	//first cycle through all keys, find a base key and associate app instance with the base key if possible.
	for (int i = 0; i<count; ++i)
	{
		_bstr_t key_ident = pvtKeyList[i].bstrVal;
		VARIANT vtKeyProductID, vtKeyProductVersion;
		// check that the key is present and valid
		VARIANT_BOOL key_license_valid(VARIANT_FALSE);
		hr = pServerInfo->LicenseServer->KeyValidateLicense(key_ident, &key_license_valid);
		if (FAILED(hr) || key_license_valid==VARIANT_FALSE) {hr = S_OK; continue;}

		// check that the key has the requisite product version and etc.
		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product Version"].id, &vtKeyProductVersion);
		if (FAILED(hr)) {hr = S_OK; continue;}
		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product ID"].id, &vtKeyProductID);
		if (FAILED(hr)) {hr = S_OK; continue;}

		// if the product id and product version requirements are satisfied
		if (Version::TinyVersion(vtKeyProductVersion.uiVal,0) >= Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0)) && 
			m_keyspec.products[m_product].id==vtKeyProductID.uiVal)
		{
			_bstr_t bstrApplicationInstance;
			std::map<unsigned int, long> lockKeyModuleByModuleMap;	//This map stores all modules that all base keys must match
			bstrApplicationInstance = GetAppInstanceFromKey(pServerInfo, key_ident, bLogError);

			VARIANT vtKeyNumberOfAppInstances;
			hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Application Instances"].id, &vtKeyNumberOfAppInstances);
			if (FAILED(hr)) {hr = S_OK; continue;}
			

			VARIANT vtKeyType;
			hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Key Type"].id, &vtKeyType);
			if (FAILED(hr)) {hr = S_OK; continue;}
			unsigned short key_type = vtKeyType.uiVal;
			typedef enum {
				KEYNone			=0x00000000,  //reserved state
				KEYAddon		   =1,
				KEYBase			=2,
				KEYReplacement	=3,
				KEYRelicense	=4,
				KEYInventory	=5,
				KEYBackup		=6,
				KEYEmergency	=7,
				//KEYSolimar	=8,
				KEYCustom		=9,
				KEYLoan			=10,
				KEYDemo			=11,
				KEYRental		=12,
				KEYDevelopment	=13,
				KEYReserved		=14,
			} KeyTypeFlagX;
			for (KeySpec::Product::data_list_t::iterator module = m_keyspec.products[m_product].data.begin(); module != m_keyspec.products[m_product].data.end(); ++module)
			{
				if (module->isLicense)
				{
					long module_id = static_cast<long>(module->id);
					long licenses_total(0), licenses_allocated(0), licenses_inuse(0);

					if(!module->isSharable)
					{
						// get the total number of licenses
						long licenses_total;
						hr = pServerInfo->LicenseServer->KeyModuleLicenseTotal(key_ident, module_id, &licenses_total);
						lockKeyModuleByModuleMap[module->id] = licenses_total;
					}
				}
			}

			//Convert to a string of modules to compare
			wchar_t wModuleString[1024];
			wchar_t wTmpBuf[64];
			

			//Treate all non addon keys the same
			//	key_type == KEYDevelopment || key_type == KEYBase || key_type == KEYBackup
			_snwprintf_s(wModuleString, sizeof(wModuleString)/sizeof(wchar_t), L"%d", (key_type==KEYAddon) ? 1 : 0);
			wModuleString[1023] = 0;
			
			for(std::map<unsigned int, long>::iterator modIt=lockKeyModuleByModuleMap.begin();
				modIt!=lockKeyModuleByModuleMap.end();
				modIt++)
			{
				_snwprintf_s(wTmpBuf, sizeof(wTmpBuf)/sizeof(wchar_t), L"|%d", modIt->second==0x7FFFFFFF ? -1 : modIt->second);
				wTmpBuf[63] = 0;
				wcscat_s(wModuleString, sizeof(wModuleString)/sizeof(wchar_t), wTmpBuf);
				wModuleString[1023] = 0;
			}

			//See if non-Addon Key is locked...
			if(key_type!=KEYAddon && !bLockedBaseKey)
			{
				if(wcscmp(bstrApplicationInstance, _bstr_t(L""))!=0)
					bLockedBaseKey = true;
				//baseKeyType = _bstr_t(wModuleString, true);
				baseKeyType = wModuleString;
			}

			bool bAddToList = true;
			//See if it is a list of known keytypes
			for(std::list<_bstr_t>::iterator keyIt = keyTypeList.begin();
				keyIt != keyTypeList.end();
				keyIt++)
			{
				if(wcscmp(*keyIt, wModuleString) == 0)
				{
					bAddToList = false;
					break;
				}
			}

			//Add to list of known keytypes
			if(bAddToList == true)
				keyTypeList.push_back(wModuleString);


			if(wcscmp(L"", bstrApplicationInstance) == 0)
			{
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::LockOneOfEachKeyConfiguration NumOfAppInstances: %d, bIsLocalServer: %s", vtKeyNumberOfAppInstances.uiVal, bIsLocalServer ? L"true" : L"false");
//OutputDebugString(tmpbuf);
				//Put in correct map based on preference.
				if((vtKeyNumberOfAppInstances.uiVal == 0 && bIsLocalServer) ||
					(vtKeyNumberOfAppInstances.uiVal > 0 && !bIsLocalServer))
				{
					freeAppInstanceKeyFirstPrefList[key_ident] = wModuleString;
				}
				else if((vtKeyNumberOfAppInstances.uiVal > 0 && bIsLocalServer)	|| m_bBypassRemoteKeyRestrictions)	
				{
					//Allow local license manager to lock a remote key, OR 
					//Allow remote license manager to lock a non-remote key only if m_bBypassRemoteKeyRestrictions==true
					freeAppInstanceKeySecondtPrefList[key_ident] = wModuleString;
				}
				else
				{
					//Don't allow remote license manager to lock a non-remote key.
				}
			}
			else	//Assign to the spot in the map
			{
				//Add to map...
				_bstr_tMap tmpMap = appInstanceToKeyInfoMap[bstrApplicationInstance];
				tmpMap[key_ident] = wModuleString;
				appInstanceToKeyInfoMap[bstrApplicationInstance] = tmpMap;
			}
		}
	}	//End for loop
	//Second Part - Cycle through and make sure 1 of each keytype is grabbed, else grab.
	//List of need keytype that need to be locked to m_applicationInstance
	std::list<_bstr_t> neededKeyTypeList;
	for(std::list<_bstr_t>::iterator keyTypeIt = keyTypeList.begin(); keyTypeIt != keyTypeList.end(); keyTypeIt++)
		neededKeyTypeList.push_back(_bstr_t(*keyTypeIt, true));

	//Remove the keytypes that are already locked to m_applicationInstance
	_bstr_tMap currentAppInstanceMap = appInstanceToKeyInfoMap[m_applicationInstance];	//Find the map of this app instances
	for(_bstr_tMap::iterator keyTypeIt = currentAppInstanceMap.begin(); keyTypeIt != currentAppInstanceMap.end(); keyTypeIt++)
	{
		hr = pServerInfo->LicenseServer->AddApplicationInstance(keyTypeIt->first, m_applicationInstance, VARIANT_TRUE);

		neededKeyTypeList.remove(_bstr_t(keyTypeIt->second, true));

		//Add key to the cache
		hr = AddKeyToCache(pServerInfo, keyTypeIt->first, bLogError);
	}
		
	//_bstr_tMap freeAppInstanceMap = appInstanceToKeyInfoMap[L""];	//Free app instances

	//Cycle through the needed list, cross reference with appInstanceToKeyInfoMap[""]
	for(std::list<_bstr_t>::iterator keyTypeIt = neededKeyTypeList.begin(); keyTypeIt != neededKeyTypeList.end(); keyTypeIt++)
	{
		bool bLockedAppInstance(false);
		for(int counter=1; counter<3 && bLockedAppInstance==false; counter++)
		{
			//Cycle through twice if need be, first time try to find one from primary list, then look in secondary list.
			_bstr_tMap freeAppInstanceMap = counter==1 ? freeAppInstanceKeyFirstPrefList : freeAppInstanceKeySecondtPrefList;

			//Cycle through the map finding a free keytype...
			for(_bstr_tMap::iterator freeKeyIt = freeAppInstanceMap.begin(); freeKeyIt != freeAppInstanceMap.end(); freeKeyIt++)
			{
				//find a match
				if(wcscmp(*keyTypeIt, freeKeyIt->second) == 0)
				{
					hr = pServerInfo->LicenseServer->AddApplicationInstance(freeKeyIt->first, m_applicationInstance, VARIANT_TRUE);
					if(SUCCEEDED(hr))	//Successfully locked key...
					{
						bLockedAppInstance = true;
						hr = AddKeyToCache(pServerInfo, freeKeyIt->first, bLogError);

						if(wcscmp(*keyTypeIt, baseKeyType) == 0)
							bLockedBaseKey = true;
						break;
					}
				}
			}
		}
		if(!bLockedAppInstance)
		{
			//throw error
			hr = LicenseServerError::EHR_KEY_NO_FREE_APP_INSTANCE;
			break;
		}
	}
	
	if(!bLockedBaseKey)
	{
		//throw custom error about no base key...
		hr = LicenseServerError::EHR_KEY_NO_BASE_KEY;
	}

	if(FAILED(hr))
	{
		//go through and clear all caches..
		FreeAllKeys(pServerInfo, pvtKeyList, count, bLogError);
	}

	//clean up
	freeAppInstanceKeyFirstPrefList.clear();
	freeAppInstanceKeySecondtPrefList.clear();
	neededKeyTypeList.clear();
	keyTypeList.clear();
	for(std::map<_bstr_t, _bstr_tMap>::iterator appIt=appInstanceToKeyInfoMap.begin(); appIt!=appInstanceToKeyInfoMap.end(); appIt++)
		appIt->second.clear();
	appInstanceToKeyInfoMap.clear();

//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::LockOneOfEachKeyConfiguration() - Leave");
//OutputDebugStringW(debug_buf);
	return hr;
}

// Returns true if the server is the same name as the local machine.  Will verify matching Domain Names or IP addresses.
bool CSolimarLicenseMgr::IsLocalMachine(_bstr_t serverName)
{
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::IsLocalMachine(serverName: %s)", (wchar_t*)serverName);
//OutputDebugString(tmpbuf);
	bool bRetVal = false;

	//Cycle through all the various checks to see if server is the "local server", break if one check passes.
	for(;;)
	{
		if(_wcsicmp(L"localhost", serverName) == 0)
		{
			bRetVal = true;
			break;
		}

		WCHAR localComputerName[1024];
		DWORD localComputerNameSize = 1024;
		if (GetComputerNameW(localComputerName, &localComputerNameSize))
		{
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::IsLocalMachine localComputerName: %s", (wchar_t*)localComputerName);
//OutputDebugString(tmpbuf);

			//
			if(_wcsicmp(localComputerName, serverName) == 0)
			{
				bRetVal = true;
				break;
			}

			//compare against IP addresses
			std::vector<_bstr_t> ipList;
			if(SUCCEEDED(NetworkUtils::SolimarNetworkHelper::GetIPAddresses(localComputerName, ipList)))
			{
				for(std::vector<_bstr_t>::iterator ipIt = ipList.begin(); ipIt != ipList.end(); ipIt++)
				{
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::IsLocalMachine ipAddress: %s", (wchar_t*)*ipIt);
//OutputDebugString(tmpbuf);

					if(_wcsicmp(*ipIt, serverName) == 0)
					{
						bRetVal = true;
						break;
					}
				}
			}
		}

		break;	//Unconditional break;
	}
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::IsLocalMachine return: %s", bRetVal ? L"true" : L"false");
//OutputDebugString(tmpbuf);
	return bRetVal;
}

HRESULT CSolimarLicenseMgr::FreeAllKeys(ServerInfo* pServerInfo, VARIANT *pvtKeyList, long count, bool bLogError)
{
	HRESULT hr = S_OK;
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::FreeAllKeys(%s, %d)", (wchar_t*)pServerInfo->name, count);
//OutputDebugString(tmpbuf);

	//first cycle through all keys, set all given Application Instances to empty to free
	for (int i = 0; i<count; ++i)
	{
		_bstr_t key_ident = pvtKeyList[i].bstrVal;
		VARIANT vtKeyProductID, vtKeyProductVersion;
		// check that the key is present and valid
		VARIANT_BOOL key_license_valid(VARIANT_FALSE);

		//KeyValidateLicense Calls KeyIsPresent() & KeyIsActive()
		hr = pServerInfo->LicenseServer->KeyValidateLicense(key_ident, &key_license_valid);
		if (FAILED(hr) || key_license_valid==VARIANT_FALSE) {hr = S_OK; continue;}

		// check that the key has the requisite product version and etc.
		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product Version"].id, &vtKeyProductVersion);
		if (FAILED(hr)) {hr = S_OK; continue;}
		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product ID"].id, &vtKeyProductID);
		if (FAILED(hr)) {hr = S_OK; continue;}

		// if the product id and product version requirements are satisfied
		if (Version::TinyVersion(vtKeyProductVersion.uiVal,0) >= Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0)) && 
			m_keyspec.products[m_product].id==vtKeyProductID.uiVal)
		{
			//Try to remove application instance, don't throw error if it was never associated with...
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"    pServerInfo->LicenseServer->RemoveApplicationInstance(%s, %s)", (wchar_t*)key_ident, (wchar_t*)m_applicationInstance);
//OutputDebugString(tmpbuf);
			pServerInfo->LicenseServer->RemoveApplicationInstance(key_ident, m_applicationInstance);
		}
	}

	//Remove all keys from being seen...
	for (ServerInfo::KeyList::iterator k = pServerInfo->keys.begin(); k != pServerInfo->keys.end(); ++k)
	{
		// mark all keys as not being present
		// if they are seen on the server scan, they will be marked as present
		k->second.KeyPresent = false;
	}

	return hr;
}



// Sets the appropiate modules on keys to be unlimited.  Calculate: #of base keys X unlimited number for modules.
HRESULT CSolimarLicenseMgr::SetUnlimitedModulesOnKeys(ServerInfo* pServerInfo, VARIANT *pvtKeyList, long count, bool bLogError)
{
	std::map<unsigned int, int> moduleCounterMap;
	int baseKeysSeen = 0;
	HRESULT hr = S_OK;

	for (ServerInfo::KeyList::iterator keyIt = pServerInfo->keys.begin(); keyIt != pServerInfo->keys.end(); ++keyIt)
	{
		KeyInfo tmpSvrInfo = keyIt->second;
		if(keyIt->second.KeyPresent && keyIt->second.KeyValid)
		{
			VARIANT vtKeyType;
			hr = pServerInfo->LicenseServer->KeyHeaderQuery(keyIt->first, m_keyspec.headers[L"Key Type"].id, &vtKeyType);
			if (FAILED(hr)) {hr = S_OK; continue;}
			unsigned short key_type = vtKeyType.uiVal;
			typedef enum {
				KEYNone			=0x00000000,  //reserved state
				KEYAddon		   =1,
				KEYBase			=2,
				KEYReplacement	=3,
				KEYRelicense	=4,
				KEYInventory	=5,
				KEYBackup		=6,
				KEYEmergency	=7,
				//KEYSolimar	=8,
				KEYCustom		=9,
				KEYLoan			=10,
				KEYDemo			=11,
				KEYRental		=12,
				KEYDevelopment	=13,
				KEYReserved		=14,
			} KeyTypeFlagX;

			//Calculate Base Keys Seen
			if(key_type!=KEYAddon)
				baseKeysSeen++;

			//Calculate Modules Seen
			for (KeySpec::Product::data_list_t::iterator moduleIt = m_keyspec.products[m_product].data.begin(); moduleIt != m_keyspec.products[m_product].data.end(); ++moduleIt)
			{
				if (moduleIt->isLicense)
				{
					///if((pServerInfo->bUseOnlySharedLicenses && module->isSharable) || !pServerInfo->bUseOnlySharedLicenses)
					moduleCounterMap[moduleIt->id] += keyIt->second.licenses_total[moduleIt->id];
				}
			}

		}
	}

   //Calculate Unlimitedness
	if(baseKeysSeen > 0)
	{
		for (int i = 0; i<count; ++i)
		{
			_bstr_t key_ident = pvtKeyList[i].bstrVal;
			VARIANT vtKeyProductID, vtKeyProductVersion;
			// check that the key is present and valid
			VARIANT_BOOL key_license_valid(VARIANT_FALSE);

			//KeyValidateLicense Calls KeyIsPresent() & KeyIsActive()
			hr = pServerInfo->LicenseServer->KeyValidateLicense(key_ident, &key_license_valid);
			if (FAILED(hr) || key_license_valid==VARIANT_FALSE) {hr = S_OK; continue;}

			// check that the key has the requisite product version and etc.
			hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product Version"].id, &vtKeyProductVersion);
			if (FAILED(hr)) {hr = S_OK; continue;}

			hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product ID"].id, &vtKeyProductID);
			if (FAILED(hr)) {hr = S_OK; continue;}

			// if the product id and product version requirements are satisfied
			if (Version::TinyVersion(vtKeyProductVersion.uiVal,0) >= Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0)) && 
				m_keyspec.products[m_product].id==vtKeyProductID.uiVal)
			{
				for(std::map<unsigned int, int>::iterator tmpModIt = moduleCounterMap.begin(); tmpModIt != moduleCounterMap.end(); tmpModIt++)
				{
					KeySpec::Module &module(m_keyspec.products[m_product][tmpModIt->first]);
					bool bSetUnlimited = false;
					if(module.unlimited > 0)
					{
						if(module.pool != 0)	//Is a pooled module
						{
							//Factor in base keys in unlimited Calculation
							KeySpec::Module &pool(m_keyspec.products[m_product][module.pool]);
							bSetUnlimited = baseKeysSeen * (int)module.unlimited <= moduleCounterMap[module.id] && baseKeysSeen * (int)pool.unlimited <= moduleCounterMap[pool.id];
						}
						else if(module.isSharable != 0)	//Is a shared module
						{
							//Don't factor in base keys in unlimited Calculation
							bSetUnlimited = (int)module.unlimited <= moduleCounterMap[module.id];
						}
						else if(module.unlimited == 1)
						{
							//Don't factor in base keys in unlimited Calculation
							bSetUnlimited = (int)module.unlimited <= moduleCounterMap[module.id];
						}
						else	//non-on/off Modules
						{
							//Factor in base keys in unlimited Calculation
							bSetUnlimited = baseKeysSeen * (int)module.unlimited <= moduleCounterMap[module.id];
						}
					}

					//Only set for the right product key
					pServerInfo->LicenseServer->KeyModuleLicenseUnlimited(pvtKeyList[i].bstrVal, module.id, bSetUnlimited ? VARIANT_TRUE : VARIANT_FALSE);
				}
			}
		}
	}
	return hr;
}

// Side Effect: if can associate App Instance with a base key, will adds all keys to the cache
// Will only add non remote access keys (Application Instance == 0) to License Managers on the same machine as the License Server
HRESULT CSolimarLicenseMgr::AssociateAppInstanceToBaseKey(ServerInfo* pServerInfo, VARIANT *pvtKeyList, long count, bool bLogError)
{
	HRESULT hr(S_OK);
//wchar_t debug_buf[1024];
//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::AssociateAppInstanceToBaseKey() - Enter");
//OutputDebugStringW(debug_buf);
	if(m_bLockKeyByAppInstance)
		return E_FAIL;	//Only m_bLockKeyByAppInstance must have all keys match

	std::list<_bstr_t> baseKeyWithNoAppInstanceFirstPrefList;	//list of free base keys, first preference
	std::list<_bstr_t> baseKeyWithNoAppInstanceSecondPrefList;	//list of free base keys, second preference
	_bstr_t keysAlreadyAssignedToAnAppInstance;
	std::list<_bstr_t> keyToAddList;	//List of all keys to add to the cache
	bool bIsLocalServer = IsLocalMachine(pServerInfo->name);
	bool bFoundAppInstance = false;
	for (int i = 0; i<count; ++i)
	{
		_bstr_t key_ident = pvtKeyList[i].bstrVal;
		VARIANT vtKeyProductID, vtKeyProductVersion;
		// check that the key is present and valid
		VARIANT_BOOL key_license_valid(VARIANT_FALSE);
      
		//KeyValidateLicense Calls KeyIsPresent() & KeyIsActive()
		hr = pServerInfo->LicenseServer->KeyValidateLicense(key_ident, &key_license_valid);
		if (FAILED(hr) || key_license_valid==VARIANT_FALSE) {hr = S_OK; continue;}

		// check that the key has the requisite product version and etc.
		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product Version"].id, &vtKeyProductVersion);
		if (FAILED(hr)) {hr = S_OK; continue;}
		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product ID"].id, &vtKeyProductID);
		if (FAILED(hr)) {hr = S_OK; continue;}

		// if the product id and product version requirements are satisfied
		if (Version::TinyVersion(vtKeyProductVersion.uiVal,0) >= Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0)) && 
			m_keyspec.products[m_product].id==vtKeyProductID.uiVal)
		{
			VARIANT vtKeyType, vtKeyNumberOfAppInstances;
			hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Key Type"].id, &vtKeyType);
			if (FAILED(hr)) {hr = S_OK; continue;}

			hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Application Instances"].id, &vtKeyNumberOfAppInstances);
			if (FAILED(hr)) {hr = S_OK; continue;}

			unsigned short key_type = vtKeyType.uiVal;
			typedef enum {
				KEYNone			=0x00000000,  //reserved state
				KEYAddon		   =1,
				KEYBase			=2,
				KEYReplacement	=3,
				KEYRelicense	=4,
				KEYInventory	=5,
				KEYBackup		=6,
				KEYEmergency	=7,
				//KEYSolimar	=8,
				KEYCustom		=9,
				KEYLoan			=10,
				KEYDemo			=11,
				KEYRental		=12,
				KEYDevelopment	=13,
				KEYReserved		=14,
			} KeyTypeFlagX;
			if(key_type != KEYAddon)
			{
				_bstr_t bstrApplicationInstance;
				bstrApplicationInstance = GetAppInstanceFromKey(pServerInfo, key_ident, bLogError);
				if(!bFoundAppInstance)
				{
					if(wcscmp(bstrApplicationInstance, m_applicationInstance) == 0)
					{
						//Found a key that is already associated with the application instance, leave
						hr = pServerInfo->LicenseServer->AddApplicationInstance(key_ident, m_applicationInstance, VARIANT_TRUE);
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::AssociateAppInstanceToBaseKey - pServerInfo->LicenseServer->AddApplicationInstance(%s, %s, true) = %s", (wchar_t*)key_ident, (wchar_t*)m_applicationInstance, hr==S_OK ? L"true" : L"false");
//OutputDebugString(tmpbuf);
						keysAlreadyAssignedToAnAppInstance = _bstr_t(key_ident, true);
						bFoundAppInstance = true;
					}
					else if(wcscmp(bstrApplicationInstance, _bstr_t(L"")) == 0)
					{
						//Found a non-addon key that is not associated, add to list of possible keys to add for association if necessary.
						//Put in correct map based on preference.
						if((vtKeyNumberOfAppInstances.uiVal == 0 && bIsLocalServer) ||
							(vtKeyNumberOfAppInstances.uiVal > 0 && !bIsLocalServer))
						{
							baseKeyWithNoAppInstanceFirstPrefList.push_back(_bstr_t(key_ident, true));
						}
						else if((vtKeyNumberOfAppInstances.uiVal > 0 && bIsLocalServer)	|| m_bBypassRemoteKeyRestrictions)
						{
							//Allow local license manager to be associated to a remote key.
							//Allow remote license manager to be associated to a non-remote key only if m_bBypassRemoteKeyRestrictions==true
							baseKeyWithNoAppInstanceSecondPrefList.push_back(_bstr_t(key_ident, true));
						}
						else
						{
							//Don't allow remote license manager to be associated to a non-remote key.
						}
					}
					else if(wcscmp(m_applicationInstance, _bstr_t(L"")) == 0)
					{
						//Special case for empty application Instance, add to list of possible keys to add for association if necessary.
						baseKeyWithNoAppInstanceFirstPrefList.push_back(_bstr_t(key_ident, true));
					}
				}
			}

			//Don't add non-remote keys to be added the pool of remote license manager
			if(!(vtKeyNumberOfAppInstances.uiVal == 0 && !bIsLocalServer) || m_bBypassRemoteKeyRestrictions)
			{
				keyToAddList.push_back(_bstr_t(key_ident, true));
			}
		}
	}	//End For Loop

	//Try to find an key to associate app instance to.
	if(!bFoundAppInstance)
	{
		//Still not found, call on the first list
		for(std::list<_bstr_t>::iterator keyIt = baseKeyWithNoAppInstanceFirstPrefList.begin();
			keyIt != baseKeyWithNoAppInstanceFirstPrefList.end();
			keyIt++)
		{
			hr = pServerInfo->LicenseServer->AddApplicationInstance(
					*keyIt, 
					m_applicationInstance, 
					VARIANT_TRUE);	//Don't Lock Key
			if(SUCCEEDED(hr))	//Successfully locked key...
			{
				keysAlreadyAssignedToAnAppInstance = _bstr_t(*keyIt, true);
				bFoundAppInstance = true;
				break;
			}
		}
		if(!bFoundAppInstance)
		{
			//Still not found, call on the secondary list
			for(std::list<_bstr_t>::iterator keyIt = baseKeyWithNoAppInstanceSecondPrefList.begin();
				keyIt != baseKeyWithNoAppInstanceSecondPrefList.end();
				keyIt++)
			{
				hr = pServerInfo->LicenseServer->AddApplicationInstance(
						*keyIt, 
						m_applicationInstance, 
						VARIANT_TRUE);	//Don't Lock Key
				if(SUCCEEDED(hr))	//Successfully locked key...
				{
					keysAlreadyAssignedToAnAppInstance = _bstr_t(*keyIt, true);
					bFoundAppInstance = true;
					break;
				}
			}
		}
	}

	//Application Instance Should be found by now...
	if(!bFoundAppInstance)
	{
		hr = LicenseServerError::EHR_KEY_NO_FREE_APP_INSTANCE;
	}
	else
	{
		//Success case
		// - Add all keys to the cache
		for(std::list<_bstr_t>::iterator keyIt = keyToAddList.begin();
			keyIt != keyToAddList.end();
			keyIt++)
		{
			//Maybe only call it on keys not locked to an application instance...
			if(wcscmp(keysAlreadyAssignedToAnAppInstance, *keyIt) != 0)
			{
				hr = pServerInfo->LicenseServer->AddApplicationInstance(
						*keyIt, 
						m_applicationInstance, 
						VARIANT_FALSE);	//Don't Lock Key
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::AssociateAppInstanceToBaseKey - pServerInfo->LicenseServer->AddApplicationInstance(%s, %s, false) = %s", (wchar_t*)*keyIt, (wchar_t*)m_applicationInstance, hr==S_OK ? L"true" : L"false");
//OutputDebugString(tmpbuf);
			}
			hr = AddKeyToCache(pServerInfo, *keyIt, bLogError);
		}
	}

	if(FAILED(hr))
	{
		//go through and clear all caches..
		FreeAllKeys(pServerInfo, pvtKeyList, count, bLogError);
	}

	
	//clean up
	baseKeyWithNoAppInstanceFirstPrefList.clear();
	baseKeyWithNoAppInstanceSecondPrefList.clear();
	keyToAddList.clear();

//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::AssociateAppInstanceToBaseKey() - Leave");
//OutputDebugStringW(debug_buf);

	return hr;
}

_bstr_t CSolimarLicenseMgr::GetAppInstanceFromKey(ServerInfo* pServerInfo, BSTR bstrKeyIdent, bool bLogError)
{
	_bstr_t app_instance_key_ident = L"";
	//Find if there is an application instance locking the key...
	VARIANT vtAppInstanceList;
	//SS_SLSERVER_FTCALL_HR(&pServerInfo, GetApplicationInstanceList, (key_ident, &pvtAppInstanceList), hr);
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::GetAppInstanceFromKey() - Enter - pServerInfo->LicenseServer->GetApplicationInstanceList(%s, %s)", (wchar_t*)bstrKeyIdent, (wchar_t*)m_applicationInstance, (wchar_t*)app_instance_key_ident);
//OutputDebugString(tmpbuf);

	HRESULT hr = pServerInfo->LicenseServer->GetApplicationInstanceList(bstrKeyIdent, &vtAppInstanceList);
	if(SUCCEEDED(hr) && (vtAppInstanceList.vt & (VT_ARRAY | VT_VARIANT)))
	{
		VARIANT *pElement = 0;	//Vector returned with BSTR name, VARIANT_BOOL bLock
		if (SUCCEEDED(SafeArrayAccessData(vtAppInstanceList.parray, (void**)&pElement)))
		{
			for(unsigned int idx=0; idx<(vtAppInstanceList.parray->rgsabound[0].cElements/2); idx++)
			{
//swprintf_s(tmpbuf, 1024, L"			Keys [%d](%s, %s)", 2*idx, (wchar_t*)pElement[2*idx].bstrVal, pElement[(2*idx)+1].boolVal == VARIANT_TRUE ? L"true" : L"false");
//OutputDebugString(tmpbuf);
//swprintf_s(tmpbuf, 1024, L"			[%d] = %s", 2*idx, (wchar_t*)pElement[2*idx].bstrVal);
//OutputDebugString(tmpbuf);
//swprintf_s(tmpbuf, 1024, L"			[%d] = %s", 2*idx+1, pElement[(2*idx)+1].boolVal == VARIANT_TRUE ? L"true" : L"false");
//OutputDebugString(tmpbuf);


				//if(pElement[(2*idx)+1].boolVal == VARIANT_TRUE && wcscmp(m_applicationInstance, pElement[idx].bstrVal) == 0)
				if(pElement[(2*idx)+1].boolVal == VARIANT_TRUE)
				{
					app_instance_key_ident = pElement[2*idx].bstrVal;
					//Comment out for testing
					//break;	//There should only be one locked
				}
			}
			SafeArrayUnaccessData(vtAppInstanceList.parray);
		}
	}
//swprintf_s(tmpbuf, 1024, L"			Returns %s", (wchar_t*)app_instance_key_ident);
//OutputDebugString(tmpbuf);
	return app_instance_key_ident;
}


// adds an individual key to the cache
HRESULT CSolimarLicenseMgr::AddKeyToCache(ServerInfo* pServerInfo, BSTR bstrKeyIdent, bool bLogError)
{
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::AddKeyToCache(serverName: %s, bstrKeyIdent: %s)", (wchar_t*)pServerInfo->name, (wchar_t*)bstrKeyIdent);
//OutputDebugString(tmpbuf);
	HRESULT hr(S_OK);
	_bstr_t key_ident = bstrKeyIdent;
	VARIANT vtKeyProductID, vtKeyProductVersion;
	for(;;)
	{
		// check that the key is present and valid
		VARIANT_BOOL key_license_valid(VARIANT_FALSE), key_active(VARIANT_FALSE);
		
		//Need result of KeyIsActive below
		hr = pServerInfo->LicenseServer->KeyIsActive(key_ident, &key_active);
		if (FAILED(hr)) {hr = S_OK; break;}

		//KeyValidateLicense Calls KeyIsPresent() & KeyIsActive()
		hr = pServerInfo->LicenseServer->KeyValidateLicense(key_ident, &key_license_valid);
		if (FAILED(hr)) {hr = S_OK; break;}
		
		// check that the key has the requisite product version and etc.
		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product Version"].id, &vtKeyProductVersion);
		if (FAILED(hr)) {hr = S_OK; break;}
		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product ID"].id, &vtKeyProductID);
		if (FAILED(hr)) {hr = S_OK; break;}
		
		// get information about trial keys
		_variant_t vtKeyStatus, vtExpirationDate, vtServerTime;
		long lTrialHours(0);
		hr = pServerInfo->LicenseServer->GetLicenseServerTime(&vtServerTime);
		if (FAILED(hr)) {hr = S_OK; break;}
		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Status"].id, &vtKeyStatus);
		if (FAILED(hr)) {hr = S_OK; break;}
		hr = pServerInfo->LicenseServer->KeyTrialExpires(key_ident, &vtExpirationDate);
		if (FAILED(hr)) {hr = S_OK; break;}
		hr = pServerInfo->LicenseServer->KeyTrialHours(key_ident, &lTrialHours);
		if (FAILED(hr)) {hr = S_OK; break;}

		// if the product id and product version requirements are satisfied
		if(m_keyspec.products[m_product].id==vtKeyProductID.uiVal)
		{
			m_productkeys[key_ident] = true;
			if (Version::TinyVersion(vtKeyProductVersion.uiVal,0) >= Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0)))
			{
				// if a specific key is requested, but this one is not it, skip this key
				if (!(m_single_key && m_specific_single_key_ident.length()>0 && m_specific_single_key_ident!=key_ident))
				{
					pServerInfo->keys[key_ident].KeyPresent = true;
					pServerInfo->keys[key_ident].KeyValid = (key_license_valid != VARIANT_FALSE);
					
					// trial information and messages
					bool TrialInfoInitialized = pServerInfo->keys[key_ident].KeyTrialInfoInitialized;
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
					if (TrialInfoInitialized && !KeyActive && pServerInfo->keys[key_ident].KeyActive)
					{
						swprintf_s(licensing_message_buffer, sizeof(licensing_message_buffer)/sizeof(wchar_t), LicensingMessageStringTable[MessageLMTempKeyExpired], KeyTrialHoursLeft, (wchar_t*)_bstr_t(vtExpirationDate));
						AddLicensingMessage(LicensingMessage(std::wstring((wchar_t*)pServerInfo->name), std::wstring((wchar_t*)key_ident), vtServerTime, MT_WARNING, MessageLMTempKeyExpired, std::wstring(licensing_message_buffer), 0, vtKeyStatus.ulVal, vtExpirationDate, KeyTrialHoursLeft));
					}
					// if the key has just become active add message
					if (TrialInfoInitialized && KeyActive && !pServerInfo->keys[key_ident].KeyActive)
					{
						swprintf_s(licensing_message_buffer, sizeof(licensing_message_buffer)/sizeof(wchar_t), LicensingMessageStringTable[MessageLMTempKeyActive], KeyTrialHoursLeft, (wchar_t*)_bstr_t(vtExpirationDate));
						AddLicensingMessage(LicensingMessage(std::wstring((wchar_t*)pServerInfo->name), std::wstring((wchar_t*)key_ident), vtServerTime, MT_INFO, MessageLMTempKeyActive, std::wstring(licensing_message_buffer), 0, vtKeyStatus.ulVal, vtExpirationDate, KeyTrialHoursLeft));
					}
					// if the key had not been seen before and is inactive
					if (KeyIsTrial && !KeyActive && !TrialInfoInitialized)
					{
						swprintf_s(licensing_message_buffer, sizeof(licensing_message_buffer)/sizeof(wchar_t), LicensingMessageStringTable[MessageLMTempKeyExpired], KeyTrialHoursLeft, (wchar_t*)_bstr_t(vtExpirationDate));
						AddLicensingMessage(LicensingMessage(std::wstring((wchar_t*)pServerInfo->name), std::wstring((wchar_t*)key_ident), vtServerTime, MT_WARNING, MessageLMTempKeyExpired, std::wstring(licensing_message_buffer), 0, vtKeyStatus.ulVal, vtExpirationDate, KeyTrialHoursLeft));
					}
					if (KeyIsTrial && KeyActive && 
						(
							// if the key is seen for the first time or the number of trial hours left has changed
							(!TrialInfoInitialized || (TrialInfoInitialized && pServerInfo->keys[key_ident].KeyTrialHoursLeft/24!=KeyTrialHoursLeft/24 && KeyTrialHoursLeft/24 < 15))
							||
							// if the key is seen for the first time or the number of expiration days left has changed
							(!TrialInfoInitialized || (TrialInfoInitialized && pServerInfo->keys[key_ident].KeyExpiresDaysLeft!=KeyExpiresDaysLeft && KeyExpiresDaysLeft < 15))
						)
					)
					{
						swprintf_s(licensing_message_buffer, sizeof(licensing_message_buffer)/sizeof(wchar_t), LicensingMessageStringTable[MessageLMTempKeyStatus], KeyTrialHoursLeft, (wchar_t*)_bstr_t(vtExpirationDate));
						AddLicensingMessage(LicensingMessage(std::wstring((wchar_t*)pServerInfo->name), std::wstring((wchar_t*)key_ident), vtServerTime, MT_INFO, MessageLMTempKeyStatus, std::wstring(licensing_message_buffer), 0, vtKeyStatus.ulVal, vtExpirationDate, KeyTrialHoursLeft));
					}
					
					pServerInfo->keys[key_ident].KeyActive = KeyActive;
					pServerInfo->keys[key_ident].KeyTrialInfoInitialized = true;
					pServerInfo->keys[key_ident].KeyIsTrial = KeyIsTrial;
					pServerInfo->keys[key_ident].KeyTrialHoursLeft = KeyTrialHoursLeft;
					pServerInfo->keys[key_ident].KeyExpiresDaysLeft = KeyExpiresDaysLeft;
					
					// refresh the cache of licenses on the key
					// for each module in the key spec for the product
					for (KeySpec::Product::data_list_t::iterator module = m_keyspec.products[m_product].data.begin(); module != m_keyspec.products[m_product].data.end(); ++module)
					{
						if (module->isLicense)
						{
							long module_id = static_cast<long>(module->id);
							long licenses_total(0), licenses_allocated(0), licenses_inuse(0);

							// if (server is set to use only sharable licenses and module is sharable ) or (server is set to use all licenses)
							if((pServerInfo->bUseOnlySharedLicenses && module->isSharable) || !pServerInfo->bUseOnlySharedLicenses)
							{
								// get the total number of licenses
								hr = pServerInfo->LicenseServer->KeyModuleLicenseTotal(key_ident, module_id, &licenses_total);
								if (FAILED(hr)) break;
								// get the number of licenses in allocated for this connection
								hr = pServerInfo->LicenseServer->KeyModuleLicenseInUse2(key_ident, module_id, &licenses_allocated);
								if (FAILED(hr)) break;
								
								// get the number of licenses in use for the key for all connections to it
								hr = pServerInfo->LicenseServer->KeyModuleInUse(key_ident, module_id, &licenses_inuse);
								if (FAILED(hr)) break;
							}
							
							pServerInfo->keys[key_ident].licenses_total[module_id] = licenses_total;
							pServerInfo->keys[key_ident].licenses_allocated[module_id] = licenses_allocated;
							pServerInfo->keys[key_ident].licenses_inuse[module_id] = licenses_inuse;
						}
					}
					
					if (pServerInfo->keys[key_ident].KeyObtained)
					{
						// ensure that the licensing on this key is still valid
						VARIANT_BOOL this_key_license_valid = VARIANT_FALSE;
						HRESULT hr = pServerInfo->LicenseServer->KeyValidateLicense(key_ident, &this_key_license_valid);
						pServerInfo->keys[key_ident].KeyValid = (SUCCEEDED(hr) && this_key_license_valid==VARIANT_TRUE);
					}
				}
			}
			else if(bLogError && Version::TinyVersion(vtKeyProductVersion.uiVal,0) < Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0)))
			{
				static const int MAX_MESSAGE_SIZE = 1024;
				wchar_t message[MAX_MESSAGE_SIZE];
				_snwprintf_s(message, MAX_MESSAGE_SIZE, LicensingMessageStringTable[MessageViolationVersion], m_prod_ver_major, m_prod_ver_minor);
				message[MAX_MESSAGE_SIZE-1] = 0;
				
				// convert the time_t in to a variant date
				time_t timestamp = time(0);
				_variant_t vtTimestamp;
				double vtimestamp;
				SYSTEMTIME systimestamp;
				memset(&systimestamp, 0, sizeof(systimestamp));
				//tm * tm_struct = gmtime(&timestamp);
				tm tm_struct;
				gmtime_s(&tm_struct, &timestamp);
				systimestamp.wSecond = tm_struct.tm_sec;
				systimestamp.wMinute = tm_struct.tm_min;
				systimestamp.wHour = tm_struct.tm_hour;
				systimestamp.wDay = tm_struct.tm_mday;
				systimestamp.wMonth = tm_struct.tm_mon + 1;
				systimestamp.wYear = tm_struct.tm_year + 1900;
				systimestamp.wDayOfWeek = tm_struct.tm_wday;
				if (SystemTimeToVariantTime(&systimestamp, &vtimestamp))
					vtTimestamp = _variant_t(vtimestamp, VT_DATE);
				else
					vtTimestamp = _variant_t(0.0, VT_DATE);

				// Add messages to queued list
				AddLicensingMessage(
					LicensingMessage(
						std::wstring((wchar_t*)pServerInfo->name),	//std::wstring &server
						std::wstring((wchar_t*)key_ident),		//std::wstring &key
						vtTimestamp,							//VARIANT msg_time
						MT_WARNING,								//DWORD msg_type
						MessageViolationVersion,				//DWORD msg_id
						std::wstring(message),					//std::wstring &msg
						0,										//HRESULT hr
						vtKeyStatus.ulVal,						//DWORD status
						vtExpirationDate,						//VARIANT expire_date
						lTrialHours								//DWORD trial_hours
					)
				);

				// Dispatch message to UI if needed.
				DispatchLicenseMessage(
					key_ident, 
					MT_INFO, 
					LicenseServerError::EC_SP_VERSION_NOT_SUPPORTED, 
					vtTimestamp, 
					_bstr_t(message)
				);
			}
		}
		break;	//Unconditional break of for loop.
	}
	return hr;
}


// checks that all licenses checked out are accounted for by some key
HRESULT CSolimarLicenseMgr::ValidateLicenseCache(ModuleLicenseMap &outstanding_licenses)
{
	ENSURE_INITIALIZED;
	SafeMutex mutex(ServerListLock);	
	
	outstanding_licenses = m_allocated_licenses;
	
	ServerList::iterator server = m_bUsingBackupServers ? m_backupServers.begin() : m_servers.begin();
	ServerList::iterator serverEnd = m_bUsingBackupServers ? m_backupServers.end() : m_servers.end();

	// foreach server
	for (;server != serverEnd; ++server)
	{
		// foreach key
		for (ServerInfo::KeyList::iterator k = server->second.keys.begin(); k != server->second.keys.end(); ++k)
		{
			if (k->second.KeyPresent && k->second.KeyObtained && k->second.KeyValid)
			{
				// for each module
				for (ModuleLicenseMap::iterator m = outstanding_licenses.begin(); m != outstanding_licenses.end(); ++m)
				{
					bool bModuleIsSharable = false;
					if((*server).second.bUseOnlySharedLicenses)
					{
						KeySpec::Module &module(m_keyspec.products[m_product][m->first]);
						bModuleIsSharable = module.isSharable;
					}

					if(((*server).second.bUseOnlySharedLicenses && bModuleIsSharable) || !(*server).second.bUseOnlySharedLicenses)
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
//wchar_t debug_buf[1024];
//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::RefreshLicenses() - Enter");
//OutputDebugStringW(debug_buf);

	ENSURE_INITIALIZED;
	
	SafeMutex mutex(ServerListLock);
	
	// refresh the cache
	time_t timeNow = time(NULL);
	double timeDiffSeconds = difftime(m_dtRefreshKeyList, timeNow);

	// don't call RefreshKeyList if just made the same call within the last 5 seconds
	if(abs(timeDiffSeconds) > 5.0)	
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
	
//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::RefreshLicenses() - Leave");
//OutputDebugStringW(debug_buf);
	return hr;
}

HRESULT CSolimarLicenseMgr::RemoveObsoleteKeysFromCache()
{
	ENSURE_INITIALIZED;
	
	SafeMutex mutex(ServerListLock);
	
	ServerList::iterator server = m_bUsingBackupServers ? m_backupServers.begin() : m_servers.begin();
	ServerList::iterator serverEnd = m_bUsingBackupServers ? m_backupServers.end() : m_servers.end();

	// foreach server
	for (;server != serverEnd; ++server)
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
	//OutputFormattedDebugString(L"CSolimarLicenseMgr::ObtainLicensesInternal moduleID=%d, licenseCount=%d", module_id, license_count);
	HRESULT hr = S_OK;
	
	ENSURE_INITIALIZED;
	
	if (license_count<=0) return E_INVALIDARG;
	
	SafeMutex mutex(ServerListLock);
	
	long licenses_to_obtain(license_count);
	long licenses_obtained(0);
	KeySpec::Module &module(m_keyspec.products[m_product][module_id]);
	bool bModuleIsSharable = module.isSharable;
	
	
	//xxx && !(single_key && allocated_licenses_on_a_key)
	//xxx how to ensure that the single key case is handled??
	//xxx need an indicator of which key is the current key (if any) and only use that key
	//xxx need a way to transfer licenses from the single key to a different single key if needed in case the first one fills up

	ServerList::iterator server = m_bUsingBackupServers ? m_backupServers.begin() : m_servers.begin();
	ServerList::iterator serverEnd = m_bUsingBackupServers ? m_backupServers.end() : m_servers.end();

	// foreach server
	for (;server != serverEnd; ++server)
	{
		if(((*server).second.bUseOnlySharedLicenses && bModuleIsSharable) || !(*server).second.bUseOnlySharedLicenses)
		{
			// foreach key
			for (ServerInfo::KeyList::iterator k = server->second.keys.begin(); k != server->second.keys.end(); ++k)
			{
				
				if (k->second.KeyPresent && k->second.KeyValid)
				{				
					long key_licenses_available = k->second.licenses_total[module_id] - k->second.licenses_inuse[module_id];
					long key_licenses_to_obtain = min(licenses_to_obtain - licenses_obtained, key_licenses_available);

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
						if (SUCCEEDED(hr)) 
						{
							licenses_obtained+=key_licenses_to_obtain; 
							k->second.licenses_allocated[module_id]+=key_licenses_to_obtain;
							k->second.licenses_inuse[module_id]+=key_licenses_to_obtain;
						}
					}
				}
			}
		}
	}
	return (licenses_obtained == licenses_to_obtain ? S_OK : LicenseServerError::EHR_LICENSE_INSUFFICIENT);
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
	KeySpec::Module &module(m_keyspec.products[m_product][module_id]);
	bool bModuleIsSharable = module.isSharable;

	ServerList::reverse_iterator server = m_bUsingBackupServers ? m_backupServers.rbegin() : m_servers.rbegin();
	ServerList::reverse_iterator serverEnd = m_bUsingBackupServers ? m_backupServers.rend() : m_servers.rend();
	// for each server (backwards)
	for (;server != serverEnd; ++server)
	{
		// if (server is set to use only sharable licenses and module is sharable ) or (server is set to use all licenses)
		if(((*server).second.bUseOnlySharedLicenses && bModuleIsSharable) || !(*server).second.bUseOnlySharedLicenses)
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
						k->second.licenses_inuse[module_id]-=key_licenses_to_release;

					}
					
					// if the key no longer has any licenses obtained on it, try to release it and unlock it where necessary
					//xxx
				}
			}
		}
	}
	return (licenses_released == licenses_to_release ? S_OK : E_FAIL);
}

// adds a message to the list of unretrieved messages
HRESULT CSolimarLicenseMgr::AddLicensingMessage(LicensingMessage &message)
{
//_variant_t vtStrExpire = message.key_expire_date;
//vtStrExpire.ChangeType(VT_BSTR);
//_bstr_t str_expire = vtStrExpire.bstrVal;
//_variant_t vtStrTimestamp = message.timestamp;
//vtStrTimestamp.ChangeType(VT_BSTR);
//_bstr_t str_timestamp = vtStrTimestamp.bstrVal;
//OutputFormattedDebugString(L"AddLicensingMessage message: %s | %d | %s", message.message, message.message_type, str_timestamp);
//
//wchar_t tmpbuf[2048];
//swprintf_s(tmpbuf, 2048, L"AddLicensingMessage keyIdent:%s | keyServer:%s | keyHours:%d | expire:%s | messageType:%d | messageID:%d | timestamp:%s | message:%s", 
//				message.key_ident.c_str(),
//				message.key_server.c_str(),
//				message.key_trial_hours,	//int
//				(wchar_t*)str_expire,
//				message.message_type,	//int
//				message.message_id, //int
//				(wchar_t*)str_timestamp, 
//				message.message.c_str());
//swprintf_s(tmpbuf, 2048, L"AddLicensingMessage message:");
//OutputDebugString(tmpbuf);


	//message.
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
	bool bViolation = (GracePeriodHasStarted() && (m_dtGracePeriodStart+(m_dtGracePeriod*60) < (unsigned)timeNow)) || m_dtGracePeriod==0;
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
   //Only do if there is a grace period
	if(m_dtGracePeriod > 0 && GracePeriodHasStarted() == false)	//If not in grace period already, start grace period
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
   //Only do if there is a grace period
	if(m_dtGracePeriod > 0 && GracePeriodHasStarted() == true)
	{
	   {
	   SafeMutex mutex(GracePeriodLock);
	   m_dtGracePeriodStart = 0;
	   }
		SS_GENERATE_AND_DISPATCH_MESSAGE(LicensingMessageStringTable[MessageGracePeriodEnded], MT_INFO, 0);
	}
}
