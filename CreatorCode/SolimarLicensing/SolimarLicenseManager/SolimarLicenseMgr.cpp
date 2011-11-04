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
#include "..\common\LicenseSettings.h"
#include "..\SolimarLicenseServer\KeyMessages.h"
#include "..\Common\LicAttribsCPP\Lic_PackageAttribs.h"
#include "..\common\LicAttribsCPP\Lic_GenericAttribs.h"
#include "..\Common\SoftwareSpecInstance.h"	//ForGlobalSoftwareSpec::GlobalSoftwareSpec & g_pSoftwareSpec
#include "resource.h"
#include <string>
#include "list"
#include <time.h>
#include <comdef.h>
#include <math.h>
#include <stdio.h>
#define ENSURE_INITIALIZED if (!m_initialized) return LicenseServerError::EHR_MANAGER_NOT_INITIALIZED

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

CSolimarLicenseMgr::SoftwareLicenseInfo::SoftwareLicenseInfo() :
	SoftwareLicenseValid(false)
{
}
CSolimarLicenseMgr::SoftwareLicenseInfo::SoftwareLicenseInfo(const SoftwareLicenseInfo &s):
	licenses_total(s.licenses_total),
	licenses_allocated(s.licenses_allocated),
	SoftwareLicenseValid(s.SoftwareLicenseValid)
{
}


CSolimarLicenseMgr::ServerInfo::ServerInfo() :
	LicenseServer(NULL),
	bValidLicensing(false),
	bUseSoftwareLicensing(false),
	bUseOnlySharedLicenses(false)
{
	;
}

CSolimarLicenseMgr::ServerInfo::ServerInfo(const ServerInfo &s) : 
	LicenseServer(s.LicenseServer)
{
	;
}

CSolimarLicenseMgr::ServerInfo::ServerInfo(
	_bstr_t servername, 
	bool useOnlySharedLicenses, 
	bool useSoftwareLicensing,
	bool useOnlyLicenseViewer,
	GITPtr<ISolimarLicenseSvr4> pILicenseServer
) : 
	name(servername), 
	bValidLicensing(false),
	bUseOnlySharedLicenses(useOnlySharedLicenses),
	bUseSoftwareLicensing(useSoftwareLicensing),
	bUseOnlyLicenseViewer(useOnlyLicenseViewer),
	LicenseServer(pILicenseServer)
{
	;
}

CSolimarLicenseMgr::ServerInfo::~ServerInfo()
{
	;
}
HRESULT CSolimarLicenseMgr::ServerInfo::Reconnect()
{
	//OutputFormattedDebugString(L"ServerInfo::Reconnect() - %s", (wchar_t*)this->name);
	// Try to create an ISolimarLicenseServer proxy to the server
	COSERVERINFO	serverInfo	= {0, this->name, NULL, 0};
	MULTI_QI		multiQI		= {&__uuidof(ISolimarLicenseSvr4), NULL, NOERROR};
	HRESULT hr = S_OK;
	ISolimarLicenseSvr4 *pILicenseServer = NULL;
	try
	{
		hr = CoCreateInstanceEx(
			__uuidof(CSolimarLicenseSvr), 
			NULL, 
			CLSCTX_REMOTE_SERVER | CLSCTX_LOCAL_SERVER, 
			&serverInfo, 
			1, 
			&multiQI);
		if(FAILED(hr))
			throw hr;


		pILicenseServer = (ISolimarLicenseSvr4*)multiQI.pItf;
		ChallengeResponseHelper CR(	challenge_key_server_thisauthuser_private, 
									sizeof(challenge_key_server_thisauthuser_private)/sizeof(BYTE), 
									challenge_key_server_userauththis_public, 
									sizeof(challenge_key_server_userauththis_public)/sizeof(BYTE));
		// try to authenticate the license server
		hr = CR.AuthenticateServer(pILicenseServer);
		if(FAILED(hr))
			throw hr;

		// let the license server authenticate this manager
		hr = CR.AuthenticateToServer(pILicenseServer);
		if(FAILED(hr))
			throw hr;
				
		hr = this->LicenseServer.Attach(pILicenseServer);
		if(FAILED(hr))
			throw hr;	
	}
	catch(HRESULT &ehr)
	{
		hr = ehr;
	}
	if (FAILED(hr) && pILicenseServer!=NULL)
		pILicenseServer->Release();
		
	return hr;
}




CSolimarLicenseMgr::CSolimarLicenseMgr() : 
	m_initialized(false), 
	ServerListLock(CreateMutex(0,0,0)),
	GracePeriodLock(CreateMutex(0,0,0)),
	MessageListLock(CreateMutex(0,0,0)),
	m_ui_level(UI_LEVEL_RESPONSE | UI_STYLE_EVENT_LOG),	// Set to these so that failure to connect will log in event log.
	m_dtGracePeriodStart(0),
	m_dtGracePeriod(0),
	m_dtRefreshLicenses(0),
	//m_bConfiguredForSoftwareLicense(false),
	m_bUsingBackupServers(false),
	m_bInViolationPeriod(false),
	m_headerInformation(_bstr_t(L"")),
	m_applicationInstance(_bstr_t(L"")),
	m_applicationInstanceKey(_bstr_t(L"")),
	m_product(-1),
	m_productKeyID(-1),
	ChallengeResponseHelper(challenge_key_manager_userauththis_private, 
							sizeof(challenge_key_manager_userauththis_private)/sizeof(BYTE), 
							challenge_key_manager_thisauthuser_public, 
							sizeof(challenge_key_manager_thisauthuser_public)/sizeof(BYTE))
{
	try
	{
		HeartbeatThread = new APCTimer(HeartbeatThreadFunction, this, 3*1000 /* 3 seconds */, InitTimerThreadCB, this);

		if(g_pSoftwareSpec == NULL)
			g_pSoftwareSpec = new GlobalSoftwareSpec();
		m_softwareSpec = g_pSoftwareSpec->GetSoftwareSpec();
	}
	catch(...)
	{
OutputDebugString(L"CSolimarLicenseMgr::CSolimarLicenseMgr() - caught general exception");	
	}
}

CSolimarLicenseMgr::~CSolimarLicenseMgr()
{
//OutputDebugString(L"CSolimarLicenseMgr::~CSolimarLicenseMgr() - Enter");	
	Disconnect();
	if (HeartbeatThread)
		delete HeartbeatThread;
	if (ServerListLock!=INVALID_HANDLE_VALUE)
		CloseHandle(ServerListLock);
	if (GracePeriodLock!=INVALID_HANDLE_VALUE)
		CloseHandle(GracePeriodLock);
	if (MessageListLock!=INVALID_HANDLE_VALUE)
		CloseHandle(MessageListLock);
//OutputDebugString(L"CSolimarLicenseMgr::~CSolimarLicenseMgr() - Leave");	
}

void CSolimarLicenseMgr::HeartbeatThreadFunction(void* pvThis)
{
	HRESULT hr = S_OK;
	CSolimarLicenseMgr *pThis = (CSolimarLicenseMgr*)pvThis;
//OutputDebugString(L"CSolimarLicenseMgr::HeartbeatThreadFunction() - Enter");
	pThis->SendHeartbeat();
//OutputDebugString(L"CSolimarLicenseMgr::HeartbeatThreadFunction() - Leave");
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
	long connectionFlags = 0;
	if(bUseOnlySharedLicenses == VARIANT_TRUE)
		connectionFlags += CF_ONLY_SHARED_LICENSE;
	if(bUseAsBackup == VARIANT_TRUE)
		connectionFlags += CF_BACKUP_SERVER;
	return Connect3(server, connectionFlags);
}


// ISolimarLicenseMgr6
STDMETHODIMP CSolimarLicenseMgr::Connect3(BSTR server, long connectionFlags)
{
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::Connect3 (server: %s, bUseOnlySharedLicenses: %s, bUseAsBackup: %s, bUseSoftwareLicense: %s)", //, bUseTestDev: %s, bUseOnlyLicViewer: %s)", 
//			  (wchar_t*)server, 
//			  connectionFlags & CF_ONLY_SHARED_LICENSE ? L"true" : L"false", 
//			  connectionFlags & CF_BACKUP_SERVER ? L"true" : L"false",
//			  connectionFlags & CF_SOFTWARE_LICENSING ? L"true" : L"false"/*,
//			  connectionFlags & CF_ONLY_LICENSE_VIEWER ? L"true" : L"false"*/
//			  );
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
	MULTI_QI		multiQI		= {&__uuidof(ISolimarLicenseSvr4), NULL, NOERROR};
//swprintf_s(tmpbuf, 1024, L"    CSolimarLicenseMgr::Connect2 () - CoCreateInstanceEx() - Start");
//OutputDebugString(tmpbuf); 
	hr = CoCreateInstanceEx(
		__uuidof(CSolimarLicenseSvr), 
		NULL, 
		CLSCTX_REMOTE_SERVER | CLSCTX_LOCAL_SERVER,
		&serverInfo, 
		1, 
		&multiQI);
//swprintf_s(tmpbuf, 1024, L"    CSolimarLicenseMgr::Connect2 () - CoCreateInstanceEx() - End");
//OutputDebugString(tmpbuf); 
	if (SUCCEEDED(hr))
	{	
		//Any licensing server that supports remote licensing will have interface ISolimarLicenseSvr4.
		ISolimarLicenseSvr4 *pILicenseServer = (ISolimarLicenseSvr4*)multiQI.pItf;
		ChallengeResponseHelper CR(	challenge_key_server_thisauthuser_private, 
									sizeof(challenge_key_server_thisauthuser_private)/sizeof(BYTE), 
									challenge_key_server_userauththis_public, 
									sizeof(challenge_key_server_userauththis_public)/sizeof(BYTE));
		// try to authenticate the license server
//swprintf_s(tmpbuf, 1024, L"    CSolimarLicenseMgr::Connect2 () - CR.AuthenticateServer() - Start");
//OutputDebugString(tmpbuf);
		hr = CR.AuthenticateServer(pILicenseServer);
//swprintf_s(tmpbuf, 1024, L"    CSolimarLicenseMgr::Connect2 () - CR.AuthenticateServer() - End");
//OutputDebugString(tmpbuf);
		if (SUCCEEDED(hr))
		{
			// let the license server authenticate this manager
//swprintf_s(tmpbuf, 1024, L"    CSolimarLicenseMgr::Connect2 () - CR.AuthenticateToServer() - Start");
//OutputDebugString(tmpbuf);
			hr = CR.AuthenticateToServer(pILicenseServer);
//swprintf_s(tmpbuf, 1024, L"    CSolimarLicenseMgr::Connect2 () - CR.AuthenticateToServer() - End");
//OutputDebugString(tmpbuf);
			if (SUCCEEDED(hr))
			{
				//if((connectionFlags & CF_SOFTWARE_LICENSING) != 0)
				if((connectionFlags & CF_ONLY_LICENSE_VIEWER) == 0)
				{
					//Any licensing server that supports remote licensing will have interface ISolimarSoftwareLicenseSvr.
					ISolimarSoftwareLicenseSvr *pLocalSoftwareLicSvr;
					hr = pILicenseServer->QueryInterface(__uuidof(ISolimarSoftwareLicenseSvr), (void**)&pLocalSoftwareLicSvr);
					if(SUCCEEDED(hr))
					{
						pLocalSoftwareLicSvr->Release();
					}
					//else
					//{
					//	//The license server does not support software licensing, continue
					//	hr = S_OK;
					//}
					//if((connectionFlags & CF_BACKUP_SERVER) != 0)
					//{
					//}

					//if connectionFlags & CF_BACKUP_SERVER
					//		verify there are backup software licenses on the server

					//if connectionFlags & CF_BACKUP_SERVER == 0
					//		verify there are standard licenses 
				}

				if(SUCCEEDED(hr))
				{
					SafeMutex mutex(ServerListLock);
					//if(bUseAsBackup == VARIANT_TRUE)
					if((connectionFlags & CF_BACKUP_SERVER) != 0)
					{
						m_backupServers[server] = ServerInfo(
							server, 
							((connectionFlags & CF_ONLY_SHARED_LICENSE) != 0), 
							((connectionFlags & CF_SOFTWARE_LICENSING) != 0), 
							((connectionFlags & CF_ONLY_LICENSE_VIEWER) != 0), 
							pILicenseServer);
					}
					else	//if(bUseAsBackup == VARIANT_FALSE)
					{
						m_servers[server] = ServerInfo(
							server, 
							((connectionFlags & CF_ONLY_SHARED_LICENSE) != 0),  
							((connectionFlags & CF_SOFTWARE_LICENSING) != 0), 
							((connectionFlags & CF_ONLY_LICENSE_VIEWER) != 0), 
							pILicenseServer);
					}
				}
			}
		}
		if (FAILED(hr))
		{
			pILicenseServer->Release();
		}
	}
	else	//CoCreateInstanceEx() Failed
	{
		//See if remote licensing is not supported - in this scenarios ISolimarLicenseSvr3 is not on the system, but ISolimarLicenseSvr2 is.
		HRESULT tmpHr = S_OK;
		COSERVERINFO	serverInfo	= {0, server, NULL, 0};
		MULTI_QI		multiQI3		= {&__uuidof(ISolimarLicenseSvr3), NULL, NOERROR};
		tmpHr = CoCreateInstanceEx(
			__uuidof(CSolimarLicenseSvr), 
			NULL, 
			CLSCTX_REMOTE_SERVER | CLSCTX_LOCAL_SERVER,
			&serverInfo, 
			1, 
			&multiQI3);
		if(FAILED(tmpHr))
		{
			//Try to call CoCreateInstanceEx() for ISolimarLicenseSvr2, if this succeeds then the machine has a license server installed,
			//but the version of the license server doesn't support remote connections.
			COSERVERINFO	serverInfoLegacy	= {0, server, NULL, 0};
			MULTI_QI		multiQILegacy		= {&__uuidof(ISolimarLicenseSvr2), NULL, NOERROR};
			tmpHr = CoCreateInstanceEx(
				__uuidof(CSolimarLicenseSvr), 
				NULL, 
				CLSCTX_REMOTE_SERVER | CLSCTX_LOCAL_SERVER,
				&serverInfoLegacy, 
				1, 
				&multiQILegacy);
			if (SUCCEEDED(tmpHr))
			{	
				//Log a message that the license server you are connecting to is at a version that doesn't allow remote connections.
				hr = LicenseServerError::EHR_KEY_NO_REMOTE_VERSION_KEY_SERVER;
				//hr = S_FALSE;
			}
		}
	}

	m_initialized = false;


	
	if(FAILED(hr))
	{
		//CR.FIX.10112 - Return cleaner error messages for ClassNotRegistered and RpcError
		if(hr == 0x80040154/*ClassNotRegistered*/)
			hr = LicenseServerError::EHR_LIC_MGR_NO_LIC_SERVER;
		else if(hr == 0x800706ba)	//RpcErrror
			hr = LicenseServerError::EHR_LIC_MGR_NO_COMPUTER;
	}
	wchar_t errorBuf[1024];
	if(FAILED(hr))
	{
		long lmMajor = 0, lmMinor = 0, lmBuild = 0;
		long lsMajor = 0, lsMinor = 0, lsBuild = 0;
		HRESULT hrResult = E_FAIL;
		if ((unsigned int)hr == 0x80004002)
		{
			hrResult = GetVersionLicenseManager(&lmMajor, &lmMinor, &lmBuild);
			if(SUCCEEDED(hrResult))
				hrResult = GetVersionLicenseServer(server, &lsMajor, &lsMinor, &lsBuild);
		}

		if(SUCCEEDED(hrResult))	//hrResult will only have a value in case of hr == 0x80004002/*NoInterfaceSupported*/
		{
			// Override No-Interface with more detailed
			if (lmMajor >= 3 && lsMajor < 3)
			{
				swprintf_s(errorBuf, _countof(errorBuf), L"Failed to connect to License Server: %s%s%s - License Server must be atleast 3.0.0, License Server is at version: %d.%d.%d\r\n - %s",
					(wchar_t*)server, 
					connectionFlags & CF_BACKUP_SERVER ? L", Backup: true" : L"",
					connectionFlags & CF_ONLY_SHARED_LICENSE ? L", Shared: true" : L"",
					lsMajor, 
					lsMinor, 
					lsBuild, 
					LicenseServerError::GenerateErrorMessage(hr).c_str());
			}
			else
			{
				swprintf_s(errorBuf, 1024, L"Failed to connect to License Server: %s%s%s - License Manager on localhost is at version: %d.%d.%d, while License Server is at version: %d.%d.%d\r\n - %s",
					(wchar_t*)server, 
					connectionFlags & CF_BACKUP_SERVER ? L", Backup: true" : L"",
					connectionFlags & CF_ONLY_SHARED_LICENSE ? L", Shared: true" : L"",
					lmMajor, 
					lmMinor, 
					lmBuild,
					lsMajor, 
					lsMinor, 
					lsBuild, 
					LicenseServerError::GenerateErrorMessage(hr).c_str());
			}
		}
		else if(FAILED(hrResult))
		{
			swprintf_s(errorBuf, 1024, L"Failed to connect to License Server: %s%s%s - %s", //, bUseTestDev: %s)", 
			  (wchar_t*)server, 
			  connectionFlags & CF_BACKUP_SERVER ? L", Backup: true" : L"",
			  connectionFlags & CF_ONLY_SHARED_LICENSE ? L", Shared: true" : L"",
			  LicenseServerError::GenerateErrorMessage(hr).c_str()
			  );
		}
	}

	LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(hr, LicenseServerError::GenerateErrorMessage(-1, std::wstring(errorBuf)), __uuidof(CSolimarLicenseMgr), __uuidof(ISolimarLicenseMgr7));
	if(_wcsicmp(m_headerInformation, L"") != 0)
		LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(hr, LicenseServerError::GenerateErrorMessage(-1, std::wstring(m_headerInformation)), __uuidof(0), __uuidof(0));	// Call this second here so product is on top of error message.
	if(FAILED(hr))
		SS_GENERATE_AND_DISPATCH_MESSAGE(L"", ((connectionFlags & CF_BACKUP_SERVER) != 0) ? MT_WARNING: MT_ERROR, hr, MessageGenericError);	//Log connection errors as warnings for backups

	if((connectionFlags & CF_BACKUP_SERVER) != 0)
		hr = S_OK; //Connection to backup server should not error out
	
	return hr;
}
STDMETHODIMP CSolimarLicenseMgr::ConnectByProduct(long product, VARIANT_BOOL bUseSharedLicenseServers)
{
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::ConnectByProduct (product: %d, bUseSharedLicenseServers: %s) - Enter", product, bUseSharedLicenseServers==VARIANT_TRUE ? L"true" : L"false");
//OutputDebugString(tmpbuf); 

	HRESULT hr(E_FAIL);
	bool bConnectedToAtleastOneComputer = false;
	try
	{
		long calProdID = product;
		if(bUseSharedLicenseServers == VARIANT_TRUE)
		{
			if(product == Lic_PackageAttribs::pid_SolIndexer)
				calProdID = Lic_PackageAttribs::pid_SdxDesigner;
			else if(product == Lic_PackageAttribs::pid_Rubika)
				calProdID = Lic_PackageAttribs::pid_RubikaProcessBuilder;
			else if(product == Lic_PackageAttribs::pid_Spde)
				calProdID = Lic_PackageAttribs::pid_SpdeQueueManager;
		}
		m_product = calProdID;

		LicenseSettings::LicenseServerSettingsTwoPointZero licenseServerSettings;
		try
		{
			LicenseSettings licSettings;
			hr = licSettings.Initialize();
			if(FAILED(hr))
				throw S_OK;	//set to ok so try to connect to localhost
			hr = licSettings.GetLiceseServerByProduct(calProdID, &licenseServerSettings);
			if(FAILED(hr))
				throw hr;
		}
		catch(HRESULT &eHr)
		{
			if(SUCCEEDED(eHr))
			{
				licenseServerSettings.serverName = L"localhost";
				licenseServerSettings.backupServerName = L"";
				licenseServerSettings.bIsTestDevLicensing = false;
			}
			else if(FAILED(eHr))
				throw eHr;
		}

		long connectionFlags = CF_SOFTWARE_LICENSING;

		wchar_t wstrBuff[1024];
		std::wstring wstrProductName = std::wstring(L"Unknown Product");
		Lic_PackageAttribs::Lic_SoftwareSpecAttribs::TMap_Lic_ProductSoftwareSpecAttribsMap::iterator prodSpecIt = m_softwareSpec.productSpecMap->find(calProdID);
		if(prodSpecIt != m_softwareSpec.productSpecMap->end())
			wstrProductName = prodSpecIt->second.productName;

		swprintf_s(wstrBuff, _countof(wstrBuff), L"Product: %s", wstrProductName.c_str());
		m_headerInformation = _bstr_t(wstrBuff);
		hr = Connect3(((licenseServerSettings.serverName.length() !=0) ? licenseServerSettings.serverName : L"localhost"), connectionFlags);	//Try to connect to primary server
		if(FAILED(hr))
			throw hr;

		if(_wcsicmp(licenseServerSettings.backupServerName, L"") != 0)	//Backup license server is set
		{
			connectionFlags += CF_BACKUP_SERVER;
			hr = Connect3(licenseServerSettings.backupServerName, connectionFlags);	//Try to connect to backup server
			if(FAILED(hr))
				throw hr;
		}

		
		if(licenseServerSettings.bIsTestDevLicensing == true)
		{
			//Successfully connected, use a different product ID for the Test/Dev licensing
			if(m_product == Lic_PackageAttribs::pid_Iconvert)
				m_product = Lic_PackageAttribs::pid_TestDevIconvert;
			else if(m_product == Lic_PackageAttribs::pid_Rubika)
				m_product = Lic_PackageAttribs::pid_TestDevRubika;
			else if(m_product == Lic_PackageAttribs::pid_RubikaProcessBuilder)
				m_product = Lic_PackageAttribs::pid_TestDevRubikaProcessBuilder;
			else if(m_product == Lic_PackageAttribs::pid_SdxDesigner)
				m_product = Lic_PackageAttribs::pid_TestDevSdxDesigner;
			else if(m_product == Lic_PackageAttribs::pid_SolIndexer)
				m_product = Lic_PackageAttribs::pid_TestDevSolIndexer;
			else if(m_product == Lic_PackageAttribs::pid_SolFusion)
				m_product = Lic_PackageAttribs::pid_TestDevSolfusionSp;
			else if(m_product == Lic_PackageAttribs::pid_SolScript)
				m_product = Lic_PackageAttribs::pid_TestDevSolScript;
			else if(m_product == Lic_PackageAttribs::pid_SolsearcherEnt)
				m_product = Lic_PackageAttribs::pid_TestDevSolsearcherEp;
			else if(m_product == Lic_PackageAttribs::pid_Spde)
				m_product = Lic_PackageAttribs::pid_TestDevSpde;
			else if(m_product == Lic_PackageAttribs::pid_SpdeQueueManager)
				m_product = Lic_PackageAttribs::pid_TestDevSpdeQueueManager;
			else if(m_product == Lic_PackageAttribs::pid_SolsearcherSp)
				m_product = Lic_PackageAttribs::pid_TestDevSseSp;
			else if(m_product == Lic_PackageAttribs::pid_SOLitrack)	//CR.14543 - Add Test/Dev for SOLitrack
				m_product = Lic_PackageAttribs::pid_TestDevSOLitrack;
			else if(m_product == Lic_PackageAttribs::pid_LibraryServices)
				m_product = Lic_PackageAttribs::pid_TestDevLibraryServices;
			
		}
		bConnectedToAtleastOneComputer = true;
	}
	catch(HRESULT &ehr)
	{
		hr = ehr;
	}

	if(SUCCEEDED(hr) && bConnectedToAtleastOneComputer == false)
	{
		//Didn't connect to a PC, try to connect to local host.
		hr = Connect2(L"localhost", VARIANT_FALSE, VARIANT_FALSE);
	}
	if(FAILED(hr))	//if failed any connect call, disconnect from any possible connections made.
	{
		m_product = -1;
		SAVE_ERRORINFO
		Disconnect();
		RESTORE_ERRORINFO
	}

//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::ConnectByProduct leave - m_product: %d, hr: 0x%08x", m_product, hr);
//OutputDebugString(tmpbuf); 	
	return hr;
}


STDMETHODIMP CSolimarLicenseMgr::GetInfoByProduct(	
	long product, 
	VARIANT_BOOL bUseSharedLicenseServers, 
	BSTR* p_server, 
	BSTR* p_backup_server, 
	VARIANT_BOOL* p_bool_use_test_dev_licensing)
{
	HRESULT hr(E_FAIL);
	try
	{
		LicenseSettings licSettings;
		hr = licSettings.Initialize();
		if(FAILED(hr))
			throw S_OK;	//set to ok so try to connect to localhost

		long calProdID = product;
		if(bUseSharedLicenseServers == VARIANT_TRUE)
		{
			if(product == Lic_PackageAttribs::pid_SolIndexer)
				calProdID = Lic_PackageAttribs::pid_SdxDesigner;
			else if(product == Lic_PackageAttribs::pid_Rubika)
				calProdID = Lic_PackageAttribs::pid_RubikaProcessBuilder;
			else if(product == Lic_PackageAttribs::pid_Spde)
				calProdID = Lic_PackageAttribs::pid_SpdeQueueManager;
		}

		LicenseSettings::LicenseServerSettingsTwoPointZero licenseServerSettings;
		hr = licSettings.GetLiceseServerByProduct(calProdID, &licenseServerSettings);
		if(FAILED(hr))
			throw hr;

		*p_server = SysAllocString(licenseServerSettings.serverName);
		*p_backup_server = SysAllocString(licenseServerSettings.backupServerName);
		*p_bool_use_test_dev_licensing = (licenseServerSettings.bIsTestDevLicensing==true) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	catch(HRESULT &ehr)
	{
		hr = ehr;
	}
	return hr;
}
//Call after an RPC error, clear all allocated licenses so they will have to be re-obtained
HRESULT CSolimarLicenseMgr::ReConnect(ServerInfo* pSrvInfoObj)
{
	SafeMutex mutex(ServerListLock);
	HRESULT hr = S_OK;
	try
	{
		//clear all allocated licenses...
		pSrvInfoObj->software_license.licenses_allocated.clear();
	}
	catch(HRESULT &ehr)
	{
		hr = ehr;
	}

	return hr;
}

STDMETHODIMP CSolimarLicenseMgr::Disconnect()
{
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::Disconnect() - Enter - AppID: %s", (wchar_t*)m_applicationInstance);
//OutputDebugString(tmpbuf);
	SafeMutex mutex(ServerListLock);

	//Free up all the licenses
//swprintf_s(tmpbuf, 1024, L"    CSolimarLicenseMgr::Disconnect() - Loop ModuleLicenseRelease()");
//OutputDebugString(tmpbuf);
	for (ModuleLicenseMap::iterator m = m_allocated_licenses.begin(); m != m_allocated_licenses.end(); ++m)
	{
		if(m->second != 0)
			ModuleLicenseRelease(m->first, m->second);
	}
//swprintf_s(tmpbuf, 1024, L"    CSolimarLicenseMgr::Disconnect() - Loop m_productkeys.erase()");
//OutputDebugString(tmpbuf);
	while(!m_productkeys.empty())
		m_productkeys.erase(m_productkeys.begin());

	VARIANT vtKeyList;
	HRESULT hr(S_OK);
	try
	{
		// for each primary server
		for (ServerList::iterator server = m_servers.begin(); server != m_servers.end(); ++server)
		{
			//FreeAllSoftwareLicenseFromCache
			if(server->second.bUseSoftwareLicensing)
			{
				if(SUCCEEDED(hr))
					hr = FreeAllSoftwareLicenseFromCache(&(server->second), false);
			}
			else
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

		// for each backup server
		for (ServerList::iterator server = m_backupServers.begin(); server != m_backupServers.end(); ++server)
		{
			//FreeAllSoftwareLicenseFromCache
			if(server->second.bUseSoftwareLicensing)
			{
				//software_license
				if(SUCCEEDED(hr))
					hr = FreeAllSoftwareLicenseFromCache(&(server->second), false);
			}
			else
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
	}
	catch (_com_error &e) 
	{
//swprintf_s(tmpbuf, 1024, L"    CSolimarLicenseMgr::Disconnect() - COM Error");
//OutputDebugString(tmpbuf);
		if(SS_RPC_FAILED(e.Error()))
		{
			//Log Message about RPC failure to Key
			SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::Disconnect() - RPC Error", MT_INFO, LicenseServerError::EC_UNKNOWN, MessageGenericError);
			SS_GENERATE_AND_DISPATCH_MESSAGE(LicensingMessageStringTable[MessageClientTimeout], MT_INFO, LicenseServerError::EC_CLIENT_TIMEOUT, MessageClientTimeout);
		}
	}
	catch (...)
	{
//swprintf_s(tmpbuf, 1024, L"    CSolimarLicenseMgr::Disconnect() - Unknown Exception");
//OutputDebugString(tmpbuf);
	}

	while(!m_servers.empty())
	{
		//m_servers.begin()->second.LicenseServer;
		m_servers.erase(m_servers.begin());
	}

	while(!m_backupServers.empty())
		m_backupServers.erase(m_backupServers.begin());

	m_ui_level = UI_LEVEL_RESPONSE | UI_STYLE_EVENT_LOG;
	m_initialized = false;
	m_bUsingBackupServers = false;
	m_bInViolationPeriod = false;
	m_product = -1;
	m_productKeyID = -1;
	m_applicationInstance = _bstr_t(L"");
	m_applicationInstanceKey = _bstr_t(L"");
	m_headerInformation = _bstr_t(L"");

	// CR.FIX.11491 - Force refresh of keys on disconnect. 
	m_dtRefreshLicenses = 0;
//OutputDebugString(L"CSolimarLicenseMgr::Disconnect() - Leave");	
	return S_OK;
}


STDMETHODIMP CSolimarLicenseMgr::KeyProductExists(long product, long prod_ver_major, long prod_ver_minor, VARIANT_BOOL *p_bool_key_product_exists)
{
	// get the key list form the server
	HRESULT hr = S_OK;
	
	*p_bool_key_product_exists = VARIANT_FALSE;
	for(int loopIdx=0; loopIdx<2; loopIdx++)
	{
		ServerList::iterator server = loopIdx==0 ? m_servers.begin() : m_backupServers.begin();
		ServerList::iterator serverEnd = loopIdx==0 ? m_servers.end() : m_backupServers.end();
		// foreach server
		for (;server != serverEnd; ++server)
		{
			if(server->second.bUseSoftwareLicensing == true)
			{
				try
				{
					BSTR bstrLicenseStream;
					SS_SLSERVER_ON_INTERFACE_FTCALL_HR(ISolimarSoftwareLicenseSvr, server->second, GetSoftwareLicenseInfoByProduct_ForAll, (m_product, &bstrLicenseStream), hr);
					if(FAILED(hr))
						throw hr;

					// might need to check for NULL or empty

					Lic_PackageAttribs::Lic_ProductInfoAttribs prodAttribs;
					prodAttribs.InitFromString(bstrLicenseStream);
					SysFreeString(bstrLicenseStream);
					
					// if the product id and product version requirements are satisfied
					if (Version::TinyVersion(Version::ModuleVersion(prodAttribs.product_Major, prodAttribs.product_Minor, 0, 0)) >= Version::TinyVersion(Version::ModuleVersion(prod_ver_major, prod_ver_minor, 0, 0)) && product==prodAttribs.productID)
					{
						*p_bool_key_product_exists = VARIANT_TRUE;
					}
				}
				catch(HRESULT &ehr)
				{
					hr = ehr;
				}

				if(*p_bool_key_product_exists == VARIANT_TRUE)
					break;
			}
			else
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
							for (unsigned int i = 0; i<vtKeyList.parray->rgsabound[0].cElements; ++i)	//for each key on the server
							{
								_bstr_t key_ident = pvtKeyIdent[i].bstrVal;
								VARIANT vtKeyProductID, vtKeyProductVersion;

								// check that the key has the requisite product version and etc.
								// CR.11272 - Moved check up higher to reduce remote calls
								hr = server->second.LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product ID"].id, &vtKeyProductID);
								if (FAILED(hr) || m_keyspec.products[m_productKeyID].id!=vtKeyProductID.uiVal) {hr = S_OK; continue;}
								
								hr = server->second.LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product Version"].id, &vtKeyProductVersion);
								if (FAILED(hr) || Version::TinyVersion(vtKeyProductVersion.uiVal,0) < Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0))) {hr = S_OK; continue;}
								
								// check that the key is present and valid
								VARIANT_BOOL key_license_valid(VARIANT_FALSE);
								//KeyValidateLicense Calls KeyIsPresent() & KeyIsActive()
								hr = server->second.LicenseServer->KeyValidateLicense(key_ident, &key_license_valid);
								if (FAILED(hr) || key_license_valid==VARIANT_FALSE) {hr = S_OK; continue;}
								
								
								// CR.11272 - Moved check up higher to reduce remote calls
								// if the product id and product version requirements are satisfied
								//if (Version::TinyVersion(vtKeyProductVersion.uiVal,0) >= Version::TinyVersion(Version::ModuleVersion(prod_ver_major, prod_ver_minor, 0, 0)) && product==vtKeyProductID.uiVal)
								//{
								*p_bool_key_product_exists = VARIANT_TRUE;
								break;
								//}
							}	//End for each key on the server
						}
					}
				}
				catch (_com_error &e) 
				{
					if(SS_RPC_FAILED(e.Error()))
					{
						//Log Message about RPC failure to Key
						SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::KeyProductExists() - RPC Error", MT_INFO, LicenseServerError::EC_UNKNOWN, MessageGenericError);
						SS_GENERATE_AND_DISPATCH_MESSAGE(LicensingMessageStringTable[MessageClientTimeout], MT_INFO, LicenseServerError::EC_CLIENT_TIMEOUT, MessageClientTimeout);
					}
				}
				if(*p_bool_key_product_exists == VARIANT_TRUE)
					break;	//Leave Loop
			}
		}	//End for (;server != serverEnd; ++server)
	}	//End for(int loopIdx=0; loopIdx<2; loopIdx++)
	return hr;
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
	return Initialize_Internal(
		VARIANT_FALSE,		//Configure for Protection Key Licensing
		application_instance, 
		product, 
		prod_ver_major, 
		prod_ver_minor, 
		single_key, 
		specific_single_key_ident, 
		lock_keys, 
		auto_ui_level, 
		grace_period_minutes, 
		app_instance_lock_key, 
		bypass_remote_key_restriction, 
		VARIANT_FALSE);
}

STDMETHODIMP CSolimarLicenseMgr::SoftwareLicense_Initialize(
		BSTR application_instance, 
		long product, 
		long prod_ver_major, 
		long prod_ver_minor, 
		long auto_ui_level, 
		unsigned long grace_period_minutes)
{
	return Initialize_Internal(
		VARIANT_TRUE, //Configure for Sofware file Licensing
		application_instance, 
		product, 
		prod_ver_major, 
		prod_ver_minor, 
		VARIANT_FALSE, //single_key
		VARIANT_FALSE, //specific_single_key_ident,
		VARIANT_FALSE, //lock_keys
		auto_ui_level, 
		grace_period_minutes, 
		VARIANT_FALSE,	//app_instance_lock_key
		VARIANT_FALSE, //bypass_remote_key_restriction
		VARIANT_FALSE);//not view only licensing
}

STDMETHODIMP CSolimarLicenseMgr::SoftwareLicense_InitializeViewOnly(
		BSTR application_instance, 
		long product, 
		long prod_ver_major, 
		long prod_ver_minor) 
{
	return Initialize_Internal(
		VARIANT_TRUE, //Configure for Sofware file Licensing
		application_instance, 
		product, 
		prod_ver_major, 
		prod_ver_minor, 
		VARIANT_FALSE, //single_key
		VARIANT_FALSE, //specific_single_key_ident,
		VARIANT_FALSE, //lock_keys
		0, 
		0, 
		VARIANT_FALSE,	//app_instance_lock_key
		VARIANT_FALSE,	//bypass_remote_key_restriction
		VARIANT_TRUE);	//for view only licensing
}

// S_OK if some keys were found
// S_FALSE if no keys are found
// Error HRESULT if there was an error
HRESULT CSolimarLicenseMgr::Initialize_Internal(
	VARIANT_BOOL b_configure_for_software_licenses, 
	BSTR application_instance, 
	long product, 
	long prod_ver_major, 
	long prod_ver_minor, 
	VARIANT_BOOL single_key, 
	BSTR specific_single_key_ident, 
	VARIANT_BOOL lock_keys, 
	long auto_ui_level, 
	unsigned long grace_period_minutes, 
	VARIANT_BOOL b_app_instance_lock_key, 
	VARIANT_BOOL b_bypass_remote_key_restriction, 
	VARIANT_BOOL b_view_licenses_only)
{
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::Initialize_Internal(application_instance: %s, product: %d, version: %d.%d, app_instance_lock_key: %s, b_view_licenses_only: %s, auto_ui_level: 0x%x) - Enter", (wchar_t*)application_instance, product, prod_ver_major, prod_ver_minor, b_app_instance_lock_key==VARIANT_TRUE ? L"true" : L"false", b_view_licenses_only==VARIANT_TRUE ? L"true" : L"false", auto_ui_level);
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
	
	m_specific_single_key_ident = _bstr_t(specific_single_key_ident, true);
	m_single_key = (single_key==VARIANT_TRUE);
	m_lock_keys = (lock_keys==VARIANT_TRUE);
	m_bViewLicenseOnly = (b_view_licenses_only==VARIANT_TRUE);
	if(m_product == -1 || m_bViewLicenseOnly /*for viewOnlyLicensing, ignore any m_product set by previous Connect call*/)
		m_product = product;

	m_prod_ver_major = prod_ver_major;
	m_prod_ver_minor = prod_ver_minor;

	// JWL - HACK - Add special case for Rubika 2.2 and below. Rubika 2.2 and below incoreectly register for licensing
	// message to be displayed via message boxes, but this causes problems when SOLfusion (a service) kicks off Rubika and there 
	// are any licensing messages, SOLfusion hangs forever.
	if((m_product == Lic_PackageAttribs::pid_Rubika) && ((m_prod_ver_major == 2 && m_prod_ver_minor <=2) || m_prod_ver_major==1))
		m_ui_level = UI_LEVEL_CRITICAL | UI_STYLE_EVENT_LOG;
	else	// for all other products
		m_ui_level = auto_ui_level;

	m_current_single_key = _bstr_t(L"");
	m_initialized = true;
	m_dtRefreshLicenses = 0;	//Force refresh on call to Initialize
	m_dtGracePeriod = grace_period_minutes;
	m_bLockKeyByAppInstance = b_app_instance_lock_key == VARIANT_TRUE ? true : false;
	m_bBypassRemoteKeyRestrictions = b_bypass_remote_key_restriction == VARIANT_TRUE ? true : false;

	// CR.13548 - To force all m_applicationInstance to be the upper case
	CComBSTR tmpBstr = application_instance;
	tmpBstr.ToUpper();
	m_applicationInstance = tmpBstr;

	InternalCalculateLegacyProtectionKeyInfo(m_product);	//Side effect: sets m_productKeyID & m_applicationInstanceKey & bUseOnlySharedLicenses in ServerInfo

//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::Initialize_Internal() m_product: %d, m_productKeyID: %d, m_applicationInstance: %s, m_applicationInstanceKey: %s", m_product, m_productKeyID, (wchar_t*)m_applicationInstance, (wchar_t*)m_applicationInstanceKey);
//OutputDebugString(tmpbuf);

	m_headerInformation = _bstr_t(L"");

	//if(m_product != -1)	//XXX - Think I can remove this check
	{
		//m_bConfiguredForSoftwareLicense = b_configure_for_software_licenses == VARIANT_TRUE ? true : false;;
		SafeMutex mutex(ServerListLock);

//OutputDebugString(L"CSolimarLicenseMgr::Initialize3 - g_pSoftwareSpec->GetSoftwareSpec - Pre");
		//m_softwareSpec = g_pSoftwareSpec->GetSoftwareSpec();
//OutputDebugString(L"CSolimarLicenseMgr::Initialize3 - g_pSoftwareSpec->GetSoftwareSpec - Post");
		//RefreshSoftwareSpec(&m_softwareSpec);

//OutputDebugString(L"CSolimarLicenseMgr::Initialize3 - RefreshLicenses() - Pre");
		hr = RefreshLicenses();
		if(FAILED(hr) && m_backupServers.size() > 0)
		{
			m_bUsingBackupServers = true;
			hr = RefreshLicenses(); //try to use backup license server.
			if(FAILED(hr))	//if backup failed, use primary and log the error.
				m_bUsingBackupServers = false;
		}
//OutputDebugString(L"CSolimarLicenseMgr::Initialize3 - RefreshLicenses() - Post");

		//if(!m_bConfiguredForSoftwareLicense)

		// CR.11272 - Why is there two RefreshKeyList() calls?
		//   RefreshLicenses() calls RefreshLicensingFromLicServers(), then RefreshLicensingFromLicServers(true) is called.
		// Maybe to log errors?
		if(hr != S_OK)	//Log S_FALSE and FAILED(hr), don't call RefreshKeyList() during success cases
		{
			SAVE_ERRORINFO
			hr = RefreshLicensingFromLicServers(true/*Log Error*/) ;
			RESTORE_ERRORINFO
		}
	}

	// S_OK if some keys were found
	// S_FALSE if no keys are found
	// Error HRESULT if there was an error
	std::wstring wstrPrimaryLicenseServer = L"";
	if(SUCCEEDED(hr) && !m_bViewLicenseOnly)	//Don't log connections of license viewer
	{
		//Send a message that connection connects
		//Move variable wstrBuff to a class variable and use it for all messages with LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE()
		wchar_t wstrBuff[1024];
		std::wstring wstrProductName = std::wstring(L"Unknown Product");
		Lic_PackageAttribs::Lic_SoftwareSpecAttribs::TMap_Lic_ProductSoftwareSpecAttribsMap::iterator prodSpecIt = m_softwareSpec.productSpecMap->find(m_product);
		if(prodSpecIt != m_softwareSpec.productSpecMap->end())
			wstrProductName = prodSpecIt->second.productName;

		// This assumes at most only one License Server and one Backup License Server
		wstrPrimaryLicenseServer = m_servers.begin()!=m_servers.end() ? m_servers.begin()->second.name : L"";
		std::wstring wstrBackupLicenseServer = m_backupServers.begin()!=m_backupServers.end() ? m_backupServers.begin()->second.name : L"";
		
		wchar_t wstrTmpBuf[10];
		swprintf_s(wstrTmpBuf, _countof(wstrTmpBuf), L"%d.%d", m_prod_ver_major, m_prod_ver_minor);
		std::wstring wstrVersion = (m_prod_ver_major!=0 && m_prod_ver_minor!=0) ? std::wstring(wstrTmpBuf) : std::wstring(L"");

		swprintf_s(wstrBuff, _countof(wstrBuff), L"Product: %s%s%s, Product Connection: %s, License Server: %s%s%s",
			wstrProductName.c_str(),
			wstrVersion.length() != 0 ? L", Version: " : L"",
			wstrVersion.length() != 0 ? wstrVersion.c_str() : L"",
			std::wstring(m_applicationInstance).c_str(),
			wstrPrimaryLicenseServer.c_str(),
			wstrBackupLicenseServer.length() != 0 ? L", Backup License Server: " : L"",
			wstrBackupLicenseServer.length() != 0 ? wstrBackupLicenseServer.c_str() : L""
			);
		m_headerInformation = _bstr_t(wstrBuff);
	}
	else if(FAILED(hr))
	{
		//Send a message that connection connects
		wchar_t wstrBuff[1024];
		std::wstring wstrProductName = std::wstring(L"Unknown Product");
		Lic_PackageAttribs::Lic_SoftwareSpecAttribs::TMap_Lic_ProductSoftwareSpecAttribsMap::iterator prodSpecIt = m_softwareSpec.productSpecMap->find(m_product);
		if(prodSpecIt != m_softwareSpec.productSpecMap->end())
			wstrProductName = prodSpecIt->second.productName;

		// This assumes at most only one License Server and one Backup License Server
		std::wstring wstrPrimaryLicenseServer = m_servers.begin()!=m_servers.end() ? m_servers.begin()->second.name : L"";
		std::wstring wstrBackupLicenseServer = m_backupServers.begin()!=m_backupServers.end() ? m_backupServers.begin()->second.name : L"";
		
		wchar_t wstrTmpBuf[10];
		swprintf_s(wstrTmpBuf, _countof(wstrTmpBuf), L"%d.%d", m_prod_ver_major, m_prod_ver_minor);
		std::wstring wstrVersion = (m_prod_ver_major!=0 && m_prod_ver_minor!=0) ? std::wstring(wstrTmpBuf) : std::wstring(L"");

		swprintf_s(wstrBuff, _countof(wstrBuff), L"Product: %s%s%s, Product Connection: %s, License Server: %s%s%s",
			wstrProductName.c_str(),
			wstrVersion.length() != 0 ? L", Version: " : L"",
			wstrVersion.length() != 0 ? wstrVersion.c_str() : L"",
			std::wstring(m_applicationInstance).c_str(),
			wstrPrimaryLicenseServer.c_str(),
			wstrBackupLicenseServer.length() != 0 ? L", Backup License Server: " : L"",
			wstrBackupLicenseServer.length() != 0 ? wstrBackupLicenseServer.c_str() : L""
			);
		m_headerInformation = _bstr_t(wstrBuff);
	}

	LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(hr, LicenseServerError::GenerateErrorMessage(hr, L"Initialization Failed"), __uuidof(0), __uuidof(0));
	if(_wcsicmp(m_headerInformation, L"") != 0)
		LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(hr, LicenseServerError::GenerateErrorMessage(-1, std::wstring(m_headerInformation)), __uuidof(0), __uuidof(0));	// Call this second here so product is on top of error message.
	if(hr != S_OK)
	{
//OutputDebugString(L"CSolimarLicenseMgr::Initialize_Internal() - MessageViolationEnter()");
		m_bInViolationPeriod = true;
		SS_GENERATE_AND_DISPATCH_MESSAGE(L"", MT_ERROR, hr, MessageGenericError);
	}
	else
	{
		m_bInViolationPeriod = false;
		SS_GENERATE_AND_DISPATCH_MESSAGE_WITH_SERVER(wstrPrimaryLicenseServer.c_str(), std::wstring(m_headerInformation + _bstr_t(L"\r\n - Initialization Succeeded")).c_str(), MT_INFO, hr, MessageValidInitialize);
	}

//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::Initialize3 leave 1 - hr: 0x%08x, m_bInViolationPeriod:%s", hr, m_bInViolationPeriod ? L"true" : L"false");
//OutputDebugString(tmpbuf); 
	return hr;
}





//Private
//Side effect: sets m_productKeyID & m_applicationInstanceKey & bUseOnlySharedLicenses in ServerInfo
void CSolimarLicenseMgr::InternalCalculateLegacyProtectionKeyInfo(long _productID)
{
	bool bConfigureForSharedLicensingKey(false);
	m_applicationInstanceKey = m_applicationInstance;
	switch(_productID)
	{
		case Lic_PackageAttribs::pid_SdxDesigner:
		case Lic_PackageAttribs::pid_TestDevSdxDesigner:
		case Lic_PackageAttribs::pid_TestDevSolIndexer:
			m_productKeyID = Lic_PackageAttribs::pid_SolIndexer;
			bConfigureForSharedLicensingKey = (_productID!=Lic_PackageAttribs::pid_TestDevSolIndexer);
			break;
		case Lic_PackageAttribs::pid_RubikaProcessBuilder:
		case Lic_PackageAttribs::pid_TestDevRubikaProcessBuilder:
		case Lic_PackageAttribs::pid_TestDevRubika:
			m_productKeyID = Lic_PackageAttribs::pid_Rubika;
			bConfigureForSharedLicensingKey = (_productID!=Lic_PackageAttribs::pid_TestDevRubika);
			break;
		case Lic_PackageAttribs::pid_SpdeQueueManager:
		case Lic_PackageAttribs::pid_TestDevSpdeQueueManager:
		case Lic_PackageAttribs::pid_TestDevSpde:
			m_productKeyID = Lic_PackageAttribs::pid_Spde;
			bConfigureForSharedLicensingKey = (_productID!=Lic_PackageAttribs::pid_TestDevSpde);
			break;
		case Lic_PackageAttribs::pid_TestDevIconvert:
			m_productKeyID = Lic_PackageAttribs::pid_Iconvert;
			break;
		case Lic_PackageAttribs::pid_TestDevSolfusionSp:
			m_productKeyID = Lic_PackageAttribs::pid_SolFusion;
			break;
		case Lic_PackageAttribs::pid_TestDevSolScript:
			m_productKeyID = Lic_PackageAttribs::pid_SolScript;
			break;
		case Lic_PackageAttribs::pid_SolsearcherSp:
		case Lic_PackageAttribs::pid_TestDevSolsearcherEp:
		case Lic_PackageAttribs::pid_TestDevSseSp:
			m_productKeyID = Lic_PackageAttribs::pid_SolsearcherEnt;
			break;
		case Lic_PackageAttribs::pid_TestDevSOLitrack://CR.14543 - Add Test/Dev for SOLitrack
			m_productKeyID = Lic_PackageAttribs::pid_SOLitrack;
			break;
		case Lic_PackageAttribs::pid_TestDevLibraryServices:
			m_productKeyID = Lic_PackageAttribs::pid_LibraryServices;
			break;
		default:
			m_productKeyID = _productID;
			break;
	}

	if(bConfigureForSharedLicensingKey)
	{
		m_applicationInstanceKey = _bstr_t(L"");

		////Cycle through all the license servers and set all the keys to be shared
		for (ServerList::iterator serverIt = m_servers.begin();serverIt != m_servers.end(); serverIt++)
			serverIt->second.bUseOnlySharedLicenses = true;

		for (ServerList::iterator serverIt = m_backupServers.begin();serverIt != m_backupServers.end(); serverIt++)
			serverIt->second.bUseOnlySharedLicenses = true;
	}
}
STDMETHODIMP CSolimarLicenseMgr::ValidateLicense(VARIANT_BOOL *license_valid)
{
//OutputDebugString(L"CSolimarLicenseMgr::ValidateLicense()"); 
	HRESULT hr = S_OK;
	try
	{
		hr = ValidateLicenseInternal(license_valid, true, true);
	}
	catch(_com_error&)
	{
		SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::ValidateLicense() - COM Exception", MT_INFO, LicenseServerError::EC_UNKNOWN, MessageGenericError);
	}
	catch(...)
	{
		SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::ValidateLicense() - Unknown Exception", MT_INFO, LicenseServerError::EC_UNKNOWN, MessageGenericError);
	}
	LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(hr, LicenseServerError::GenerateErrorMessage(hr), __uuidof(CSolimarLicenseMgr), __uuidof(ISolimarLicenseMgr7));
	return hr;
}
//Parameter:
//	bool use_back_up_on_error - use backup servers or enter Grace/Violation Period if FAILS
HRESULT CSolimarLicenseMgr::ValidateLicenseInternal(VARIANT_BOOL *license_valid, bool use_back_up_on_error, bool bLogError)
{

	HRESULT hr = S_OK;
	ENSURE_INITIALIZED;
	SafeMutex mutex(ServerListLock);
	bool bAlreadyUsingBackUpServers = m_bUsingBackupServers;
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, _countof(tmpbuf), L"CSolimarLicenseMgr::ValidateLicenseInternal (use_back_up_on_error: %s) - bAlreadyUsingBackUpServers: %s, m_bInViolationPeriod: %s", /*, m_bPrimaryLicenseServerHasBeenSuccessfullyInitialized: %s", */
//			  use_back_up_on_error ? L"true" : L"false", 
//			  bAlreadyUsingBackUpServers ? L"true" : L"false", 
//			  m_bInViolationPeriod ? L"true" : L"false"//,
//			  //m_bPrimaryLicenseServerHasBeenSuccessfullyInitialized ? L"true" : L"false"
//			  );
//OutputDebugString(tmpbuf);

	m_bUsingBackupServers = false;	//Try to Re-use non-Backup Servers.
	HRESULT hrPrimaryServers = S_OK;
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
			break;	// Break out of loop
		}
		else if(*license_valid == VARIANT_FALSE)
		{
			if(hr == LicenseServerError::EHR_LIC_SOFTWARE_PRODUCT_NO_VERSION)
			{
				// case that license file has been removed via license archive, do not allow going to backup server,
				// go into license violation.
				break;
			}
			else if(m_bUsingBackupServers || !use_back_up_on_error)
			{
				//License failed using active server and backup server, fall to grace period section...
				if(bAlreadyUsingBackUpServers)
					m_bUsingBackupServers = bAlreadyUsingBackUpServers;
				break;	// Break out of loop
			}
			else //if(!m_bUsingBackupServers)	
			{
				//try validation again with backup servers
				hrPrimaryServers = hr;
				m_bUsingBackupServers = true;

				// Generates a message about using backup servers.
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
						HRESULT messageHr = LicenseServerError::EHR_KEY_USE_FAILOVER_KEY_SERVER;
						wchar_t licensing_message_buffer[256];
						swprintf_s(	licensing_message_buffer,
							sizeof(licensing_message_buffer)/sizeof(wchar_t),
							LicensingMessageStringTable[messageID],
							(wchar_t*)bstr_KeyServer,
							(wchar_t*)bstr_BackupKeyServer
							);
						LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(messageHr, LicenseServerError::GenerateErrorMessage(hrPrimaryServers, std::wstring(licensing_message_buffer), false), __uuidof(CSolimarLicenseMgr), __uuidof(ISolimarLicenseMgr7));
						if(_wcsicmp(m_headerInformation, L"") != 0)
							LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(messageHr, LicenseServerError::GenerateErrorMessage(-1, std::wstring(m_headerInformation)), __uuidof(0), __uuidof(0));
						if(bLogError)
							SS_GENERATE_AND_DISPATCH_MESSAGE_WITH_SERVER(bstr_KeyServer, LicenseServerError::GetErrorMessage(messageHr).c_str(), MT_WARNING, messageHr, MessageGeneric);
					}
				}
			}
		}
	}	//End For Loop

//swprintf_s(tmpbuf, _countof(tmpbuf), 
//			  L"CSolimarLicenseMgr::ValidateLicenseInternal () - m_bUsingBackupServers: %s, license_valid: %s, m_bPrimaryLicenseServerHasBeenSuccessfullyInitialized: %s", 
//			  m_bUsingBackupServers ? L"true" : L"false",
//			  (*license_valid == VARIANT_TRUE)  ? L"true" : L"false",
//			  m_bPrimaryLicenseServerHasBeenSuccessfullyInitialized ? L"true" : L"false"
//			  );
//OutputDebugString(tmpbuf);
	//Grace Period Section
	if(*license_valid == VARIANT_FALSE)
	{
		if(use_back_up_on_error)
		{
			StartGracePeriod();	//Does nothing if grace period has already been started
			if(!GracePeriodHadEnded())
			{
				*license_valid = VARIANT_TRUE;
				hr = S_OK;
			}
			else if(m_bInViolationPeriod == false)
			{
				//It Violation, see if it entered Violation for the first time...
				_bstr_t bstr_KeyServer = L"";
				_bstr_t bstr_BackupKeyServer = L"";
				if(!m_servers.empty())
					bstr_KeyServer = (*m_servers.begin()).second.name;
				if(!m_backupServers.empty())
					bstr_BackupKeyServer = (*m_backupServers.begin()).second.name;

				HRESULT messageHr = hrPrimaryServers;
				wchar_t licensing_message_buffer[256];
				swprintf_s(	licensing_message_buffer,
								_countof(licensing_message_buffer),
								L"Entering Licensing Violation period. License error with License Server: %s%s%s.",
								(wchar_t*)bstr_KeyServer,
								((_wcsicmp(bstr_BackupKeyServer, L"") != 0) ? L", Backup License Server: " : L""),
								((_wcsicmp(bstr_BackupKeyServer, L"") != 0) ? (wchar_t*)bstr_BackupKeyServer : L"")
						);
				LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(messageHr, LicenseServerError::GenerateErrorMessage(messageHr, std::wstring(licensing_message_buffer), false), __uuidof(CSolimarLicenseMgr), __uuidof(ISolimarLicenseMgr7));
				if(_wcsicmp(m_headerInformation, L"") != 0)
					LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(messageHr, LicenseServerError::GenerateErrorMessage(-1, std::wstring(m_headerInformation)), __uuidof(0), __uuidof(0));
				if(bLogError)
					SS_GENERATE_AND_DISPATCH_MESSAGE_WITH_SERVER(bstr_KeyServer, L"", MT_ERROR, messageHr, MessageViolationEnter);
//OutputDebugString(L"CSolimarLicenseMgr::ValidateLicenseInternal() - MessageViolationEnter()");
				m_bInViolationPeriod = true;
			}
		}
	}
	else if(*license_valid == VARIANT_TRUE)
	{
		if(bAlreadyUsingBackUpServers && !m_bUsingBackupServers)
		{
			//Free all licenses on backup servers
			// for each backup server
			for (ServerList::iterator serverIt = m_backupServers.begin(); serverIt != m_backupServers.end(); ++serverIt)
			{
				//FreeAllSoftwareLicenseFromCache
				if(serverIt->second.bUseSoftwareLicensing)
				{
					//software_license
					if(SUCCEEDED(hr))
						hr = FreeAllSoftwareLicenseFromCache(&(serverIt->second), false);
				}
				else
				{
					VARIANT vtKeyList;
					SS_SLSERVER_FTCALL_HR(serverIt->second, KeyEnumerate, (&vtKeyList), hr);
					if (SUCCEEDED(hr) && (vtKeyList.vt & (VT_ARRAY | VT_VARIANT)))
					{
						VARIANT *pvtKeyIdent;
						if (SUCCEEDED(SafeArrayAccessData(vtKeyList.parray, (void**)&pvtKeyIdent)))
						{
							FreeAllKeys(&(serverIt->second), pvtKeyIdent, vtKeyList.parray->rgsabound[0].cElements, false);
						}
					}
				}
			}

			//Move to Active Servers, log message not using backservers anymore...
			_bstr_t bstr_KeyServer = L"";
			if(!m_servers.empty())
				bstr_KeyServer = (*m_servers.begin()).second.name;

			int messageID = MessageKeysRestoreKeyServer;
			HRESULT messageHr = LicenseServerError::EHR_KEY_RESTORED_KEY_SERVER;
			wchar_t licensing_message_buffer[256];
			swprintf_s(	licensing_message_buffer,
						sizeof(licensing_message_buffer)/sizeof(wchar_t),
						LicensingMessageStringTable[messageID],
						(wchar_t*)bstr_KeyServer);
			LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(messageHr, LicenseServerError::GenerateErrorMessage(-1, std::wstring(licensing_message_buffer), false), __uuidof(CSolimarLicenseMgr), __uuidof(ISolimarLicenseMgr7));
			if(_wcsicmp(m_headerInformation, L"") != 0)
				LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(messageHr, LicenseServerError::GenerateErrorMessage(-1, std::wstring(m_headerInformation)), __uuidof(0), __uuidof(0));
			if(bLogError)
				SS_GENERATE_AND_DISPATCH_MESSAGE_WITH_SERVER(bstr_KeyServer, LicenseServerError::GetErrorMessage(messageHr).c_str(), MT_WARNING, messageHr, MessageViolationLeave);
//OutputDebugString(L"CSolimarLicenseMgr::ValidateLicenseInternal() - MessageViolationLeave()");
		}
		m_bInViolationPeriod = false;
		StopGracePeriod();
	}

	if(FAILED(hrPrimaryServers) && FAILED(hr))	// Means there was a error contacting both primary and backup license servers, return error from primary
		hr = hrPrimaryServers;
//OutputFormattedDebugString(L"CSolimarLicenseMgr::ValidateLicenseInternal return: 0x%08x, license_valid: %d", hr, *license_valid == VARIANT_TRUE);
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
	hr = ModuleLicenseTotalInternal(module_id, count);
	LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(hr, LicenseServerError::GenerateErrorMessage(hr), __uuidof(CSolimarLicenseMgr), __uuidof(ISolimarLicenseMgr7));
	return hr;
}
HRESULT CSolimarLicenseMgr::ModuleLicenseTotalInternal(long module_id, long *count)
{
	HRESULT hr = S_OK;
	
	*count = 0;
	
	ENSURE_INITIALIZED;
	
	try
	{
		SafeMutex mutex(ServerListLock);	
		
		ServerList::iterator server = m_bUsingBackupServers ? m_backupServers.begin() : m_servers.begin();
		ServerList::iterator serverEnd = m_bUsingBackupServers ? m_backupServers.end() : m_servers.end();

		// foreach server
		for (;server != serverEnd; ++server)
		{
			if(server->second.bUseSoftwareLicensing) // software licensing
			{
				long unlimitedNumber = 0;
				Lic_PackageAttribs::Lic_SoftwareSpecAttribs::TMap_Lic_ProductSoftwareSpecAttribsMap::iterator prodSpecIt = m_softwareSpec.productSpecMap->find(m_product);
				
				if(prodSpecIt != m_softwareSpec.productSpecMap->end())
				{
					if(prodSpecIt->second.sameModSpecProductID != 0)	//Test/Dev's share the same mod spec
					{
						Lic_PackageAttribs::Lic_SoftwareSpecAttribs::TMap_Lic_ProductSoftwareSpecAttribsMap::iterator  tmpProdSpecIt = m_softwareSpec.productSpecMap->find((int)prodSpecIt->second.sameModSpecProductID);
						if(tmpProdSpecIt != m_softwareSpec.productSpecMap->end())
							prodSpecIt = tmpProdSpecIt;
					}

					Lic_PackageAttribs::Lic_ProductSoftwareSpecAttribs::TMap_Lic_ModuleSoftwareSpecAttribsMap::iterator modSpecIt = prodSpecIt->second.moduleSpecMap->find(module_id);
					if(modSpecIt != prodSpecIt->second.moduleSpecMap->end())
					{
						unlimitedNumber = modSpecIt->second.modUnlimitedValue;
					}
				}

				if(unlimitedNumber != 0 && unlimitedNumber<= server->second.software_license.licenses_total[module_id])
					*count = 0x7FFFFFFF;
				else
					*count = server->second.software_license.licenses_total[module_id];
			}
			else
			{
				KeySpec::Module &module(m_keyspec.products[m_productKeyID][module_id]);
				bool bModuleIsSharable = module.isSharable;
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
		}
	}
	
	catch(_com_error& e)
	{
		hr = e.Error();
		*count = 0;
	}
	
//wchar_t debug_buf[1024];
//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::ModuleLicenseTotal module_id=%d, count=%d)", module_id, *count);
//debug_buf[1023] = 0;
//OutputDebugStringW(debug_buf);

	return hr;
}

STDMETHODIMP CSolimarLicenseMgr::ModuleLicenseInUse(long module_id, long *count)
{
	HRESULT hr = S_OK;
	
	*count = 0;
	
	ENSURE_INITIALIZED;
	
	SafeMutex mutex(ServerListLock);
	
	hr = ModuleLicenseInUseInternal(module_id, count);
	
	return hr;
}


STDMETHODIMP CSolimarLicenseMgr::ModuleLicenseInUse_ByApp(long module_id, long *count)
{
	HRESULT hr = S_OK;
	
	*count = 0;
	
	ENSURE_INITIALIZED;
	
	SafeMutex mutex(ServerListLock);
	hr = ModuleLicenseInUse_ByAppInternal(module_id, count);
	LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(hr, LicenseServerError::GenerateErrorMessage(hr), __uuidof(CSolimarLicenseMgr), __uuidof(ISolimarLicenseMgr7));
	return hr;
}
HRESULT CSolimarLicenseMgr::ModuleLicenseInUseInternal(long module_id, long *count)
{
	HRESULT hr = S_OK;
	
	*count = 0;
	
	ENSURE_INITIALIZED;
	
	SafeMutex mutex(ServerListLock);	
	
	ServerList::iterator server = m_bUsingBackupServers ? m_backupServers.begin() : m_servers.begin();
	ServerList::iterator serverEnd = m_bUsingBackupServers ? m_backupServers.end() : m_servers.end();

	bool bUnset(true);
	// foreach server
	for (;server != serverEnd; ++server)
	{
		if(server->second.bUseSoftwareLicensing) // software licensing
		{
			try
			{
				long software_licenses_in_use(0);

				if (m_bViewLicenseOnly)
				{
					SS_SLSERVER_ON_INTERFACE_FTCALL(ISolimarSoftwareLicenseSvr, server->second, SoftwareAddApplicationInstanceByProduct, (m_product, m_applicationInstance));
				}

				SS_SLSERVER_ON_INTERFACE_FTCALL_HR(ISolimarSoftwareLicenseSvr, server->second, SoftwareModuleLicenseInUseByConnection_ByProduct, (m_product, module_id, &software_licenses_in_use), hr);

				if (m_bViewLicenseOnly)
				{
					SS_SLSERVER_ON_INTERFACE_FTCALL(ISolimarSoftwareLicenseSvr, server->second, SoftwareRemoveApplicationInstanceByProduct, (m_product, m_applicationInstance));
				}

				if(FAILED(hr)) 
					throw hr;

				*count += software_licenses_in_use; 
				bUnset = false;
			}
			catch(HRESULT &ehr)
			{
				hr = ehr;
			}
		}
	}

	if(bUnset)
	{
		//Will also enter this if the license servers with software licensing are not able to be reached.
		*count = m_allocated_licenses[module_id];
		hr = S_OK;
	}
	
//wchar_t debug_buf[1024];
//_snwprintf_s(debug_buf, _countof(debug_buf), L"CSolimarLicenseMgr::ModuleLicenseInUseInternal module_id=%d, count=%d)", module_id, *count);
//OutputDebugStringW(debug_buf);
	return hr;
}
HRESULT CSolimarLicenseMgr::ModuleLicenseInUse_ByAppInternal(long module_id, long *count)
{
	HRESULT hr = S_OK;
	
	*count = 0;
	
	ENSURE_INITIALIZED;
	
	SafeMutex mutex(ServerListLock);	
	
	ServerList::iterator server = m_bUsingBackupServers ? m_backupServers.begin() : m_servers.begin();
	ServerList::iterator serverEnd = m_bUsingBackupServers ? m_backupServers.end() : m_servers.end();

	bool bUnset(true);
	// foreach server
	for (;server != serverEnd; ++server)
	{
		if(server->second.bUseSoftwareLicensing) // software licensing
		{
			//The old scheme just returned the in use number of instance, didn't take multiple processes with the same 
			//application name into account.  If the new InUse breaks something, look here first
			try
			{
				long software_licenses_in_use(0);

				if (m_bViewLicenseOnly)
				{
					SS_SLSERVER_ON_INTERFACE_FTCALL(ISolimarSoftwareLicenseSvr, server->second, SoftwareAddApplicationInstanceByProduct, (m_product, m_applicationInstance));
				}

				SS_SLSERVER_ON_INTERFACE_FTCALL_HR(ISolimarSoftwareLicenseSvr, server->second, SoftwareModuleLicenseInUseByApp_ByProduct, (m_product, module_id, &software_licenses_in_use), hr);
				//SS_SLSERVER_ON_INTERFACE_FTCALL_HR(ISolimarSoftwareLicenseSvr, server->second, SoftwareModuleLicenseInUseByConnection_ByProduct, (m_product, module_id, &software_licenses_in_use), hr);

				if (m_bViewLicenseOnly)
				{
					SS_SLSERVER_ON_INTERFACE_FTCALL(ISolimarSoftwareLicenseSvr, server->second, SoftwareRemoveApplicationInstanceByProduct, (m_product, m_applicationInstance));
				}

				if(FAILED(hr)) 
					throw hr;

				*count += software_licenses_in_use; 
				bUnset = false;
			}
			catch(HRESULT &ehr)
			{
				hr = ehr;
			}
		}
	}

	if(bUnset)
	{
		//Protection Key model, check the cache of obtained licenses instead of querying keys...
		//This does not take multiple License Servers into account.  Also doesn't take multiple processes
		//with the same ApplicationInstanceID into account.

		//Will also enter this if the license servers with software licensing are not able to be reached.
		*count = appInstanceInUseModuleCacheMap[module_id];
		hr = S_OK;
	}
	
//wchar_t debug_buf[1024];
//_snwprintf_s(debug_buf, _countof(debug_buf), L"CSolimarLicenseMgr::ModuleLicenseInUseInternal module_id=%d, count=%d)", module_id, *count);
//OutputDebugStringW(debug_buf);
	return hr;
}
STDMETHODIMP CSolimarLicenseMgr::ModuleLicenseObtain(long module_id, long count)
{
//wchar_t debug_buf[1024];
//_snwprintf_s(debug_buf, _countof(debug_buf), _countof(debug_buf), L"CSolimarLicenseMgr::ModuleLicenseObtain m_product: %d, AppID: %s, module_id=%d, count=%d)", m_product, (wchar_t*)m_applicationInstance, module_id, count);
//OutputDebugStringW(debug_buf);

	HRESULT hr = S_OK;
	ENSURE_INITIALIZED;
	try
	{
		if (m_bViewLicenseOnly) 
			throw LicenseServerError::EHR_LIC_MGR_VIEW_ONLY;

		if (count<0)
			throw E_INVALIDARG;

		if(GracePeriodHasStarted())
			throw LicenseServerError::EHR_LICENSE_INSUFFICIENT;

		SafeMutex mutex(ServerListLock);

		// record that the licenses were obtained
		m_allocated_licenses[module_id] += count;
		// perform the license allocation
		VARIANT_BOOL licensing_valid = VARIANT_FALSE;
		hr = ValidateLicenseInternal(&licensing_valid, false/*don't use grace period*/, false/*don't log error*/);
		if (FAILED(hr) || licensing_valid==VARIANT_FALSE) 
		{
			m_allocated_licenses[module_id] -= count;
			//Is case of FAILED(hr) && licensing_valid==VARIANT_TRUE a possible case?

			//1st call to validate will cause licensing to enter violation, if error was
			//because of insufficient licensing call again with logging off to get licensing
			//out of violation without a message being logged.
			//if(hr ==LicenseServerError::EHR_LICENSE_INSUFFICIENT)
			ValidateLicenseInternal(&licensing_valid, true, false);
			throw hr;
		}

	}
	catch(HRESULT &eHr)
	{
		hr = eHr;
	}

	LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(hr, LicenseServerError::GenerateErrorMessage(hr), __uuidof(CSolimarLicenseMgr), __uuidof(ISolimarLicenseMgr7));
	if(_wcsicmp(m_headerInformation, L"") != 0)
		LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(hr, LicenseServerError::GenerateErrorMessage(-1, std::wstring(m_headerInformation)), __uuidof(0), __uuidof(0));
	//if(FAILED(hr))
	//	SS_GENERATE_AND_DISPATCH_MESSAGE(L"", MT_ERROR, hr, MessageGenericError);

//OutputFormattedDebugString(L"CSolimarLicenseMgr::ModuleLicenseObtain(%d, %d) - returns: 0x%08x", module_id, count, hr);
	return hr;
}

STDMETHODIMP CSolimarLicenseMgr::ModuleLicenseRelease(long module_id, long count)
{
//wchar_t debug_buf[1024];
//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::ModuleLicenseRelease AppID: %s, module_id: %d, count: %d - Enter)", (wchar_t*)m_applicationInstance, module_id, count);
//OutputDebugStringW(debug_buf);
	//static const MAX_MESSAGE_SIZE = 1024;
	//wchar_t event_log_msg[MAX_MESSAGE_SIZE];	
	//_snwprintf(event_log_msg, MAX_MESSAGE_SIZE, L"ModuleLicenseRelease module_id=%d, count=%d", module_id, count);
	//EventLogHelper::WriteEventLog(L"Solimar License Server", event_log_msg, MT_INFO);
	HRESULT hr = S_OK;
	
	ENSURE_INITIALIZED;
	
	SafeMutex mutex(ServerListLock);

	if (m_bViewLicenseOnly) 
		return LicenseServerError::EHR_LIC_MGR_VIEW_ONLY;

	if (count<0 || count>m_allocated_licenses[module_id])
		return E_INVALIDARG;

	// record that the licenses were released
	m_allocated_licenses[module_id] -= count;

	// perform the license de-allocation
	//m_bUsingBackupServers = false;	//CR.FIX.14599 - Try to Re-use non-Backup Servers.
	//RefreshLicenses();	//don't return the state of the manager

	//CR.FIX.14599 - Incase licenses are freeded, call ValidateLicenseInternal
	VARIANT_BOOL licensing_valid = VARIANT_FALSE;
	ValidateLicenseInternal(&licensing_valid, true, false);

//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::ModuleLicenseRelease AppID: %s, module_id: %d, count: %d - Leave)", (wchar_t*)m_applicationInstance, module_id, count);
//OutputDebugStringW(debug_buf);
	LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(hr, LicenseServerError::GenerateErrorMessage(hr), __uuidof(CSolimarLicenseMgr), __uuidof(ISolimarLicenseMgr7));
//OutputFormattedDebugString(L"CSolimarLicenseMgr::ModuleLicenseRelease(%d, %d) - returns: 0x%08x", module_id, count, hr);
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
						
						// check that the key has the requisite product version and etc.
						// CR.11272 - Moved check up higher to reduce remote calls
						hr = server->second.LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product ID"].id, &vtKeyProductID);
						if (FAILED(hr) || m_keyspec.products[m_productKeyID].id!=vtKeyProductID.uiVal) {hr = S_OK; continue;}
						
						hr = server->second.LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product Version"].id, &vtKeyProductVersion);
						if (FAILED(hr) || Version::TinyVersion(vtKeyProductVersion.uiVal,0) < Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0))) {hr = S_OK; continue;}

						// check that the key is present and valid
						VARIANT_BOOL key_license_valid(VARIANT_FALSE);
						//KeyValidateLicense Calls KeyIsPresent() & KeyIsActive()
						hr = server->second.LicenseServer->KeyValidateLicense(key_ident, &key_license_valid);
						if (FAILED(hr) || key_license_valid==VARIANT_FALSE) {hr = S_OK; continue;}
						
						// if the product id and product version requirements are satisfied
						if (Version::TinyVersion(vtKeyProductVersion.uiVal,0) >= Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0)) && m_keyspec.products[m_productKeyID].id==vtKeyProductID.uiVal)
						{
							// if a specific key is requested, but this one is not it, skip this key
							if (!(m_single_key && m_specific_single_key_ident.length()>0 && m_specific_single_key_ident!=key_ident))
							{
								// refresh the cache of licenses on the key
								// for each module in the key spec for the product
								for (KeySpec::Product::data_list_t::iterator module = m_keyspec.products[m_productKeyID].data.begin(); module != m_keyspec.products[m_productKeyID].data.end(); ++module)
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
				SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::ModuleLicenseSerialNumbers() - RPC Error", MT_INFO, LicenseServerError::EC_UNKNOWN, MessageGenericError);
				SS_GENERATE_AND_DISPATCH_MESSAGE(LicensingMessageStringTable[MessageClientTimeout], MT_INFO, LicenseServerError::EC_CLIENT_TIMEOUT, MessageClientTimeout);
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

	if (m_bViewLicenseOnly) 
		return LicenseServerError::EHR_LIC_MGR_VIEW_ONLY;

	SafeMutex mutex(ServerListLock);
	
	// perform the license allocation
	VARIANT_BOOL licensing_valid = VARIANT_FALSE;
	hr = ValidateLicenseInternal(&licensing_valid, true, false);
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
						if (Version::TinyVersion(vtKeyProductVersion.uiVal,0) >= Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0)) && m_keyspec.products[m_productKeyID].id==vtKeyProductID.uiVal)
						{
							// if a specific key is requested, but this one is not it, skip this key
							if (!(m_single_key && m_specific_single_key_ident.length()>0 && m_specific_single_key_ident!=key_ident))
							{
								for (KeySpec::Product::data_list_t::iterator module = m_keyspec.products[m_productKeyID].data.begin(); module != m_keyspec.products[m_productKeyID].data.end() && licenseToDecrement>0; ++module)
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
		if(server->second.bUseSoftwareLicensing == false)
		{
			// if this server's key list contains the key in question
			if (server->second.keys.find(key_ident)!=server->second.keys.end())
			{
				return true;
			}
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
		SS_SLSERVER_FTCALL_HR(serverIt->second, GetVersionLicenseServer, (p_ver_major, p_ver_minor, p_ver_build), hr);
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
		// ISolimarLicenseSvr3 contains GetVersionLicenseServer()
		COSERVERINFO	serverInfo	= {0, server, NULL, 0};
		MULTI_QI		multiQI		= {&__uuidof(ISolimarLicenseSvr3), NULL, NOERROR};

		hr = CoCreateInstanceEx(
			__uuidof(CSolimarLicenseSvr), 
			NULL, 
			CLSCTX_REMOTE_SERVER | CLSCTX_LOCAL_SERVER,
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
//wchar_t tmpbuf[1024];
	HRESULT hr = S_OK;
	ENSURE_INITIALIZED;
	try
	{
		SafeMutex mutex(ServerListLock);
		{
			SafeMutex mutex_messages(MessageListLock);
			ServerList::iterator server = m_bUsingBackupServers ? m_backupServers.begin() : m_servers.begin();
			ServerList::iterator serverEnd = m_bUsingBackupServers ? m_backupServers.end() : m_servers.end();

			LicensingMessageList server_messages_list;

			for (;server != serverEnd; ++server)
			{
				_variant_t vtServerMessageList;
				//Get all the Protection Key messages...
				try
				{
					SS_SLSERVER_ON_INTERFACE_FTCALL_HR(ILicensingMessage, server->second, GetLicenseMessageList, (VARIANT_TRUE, &vtServerMessageList), hr);
					LicensingMessageList server_messages(vtServerMessageList);
					licensing_message_cache.insert(licensing_message_cache.end(), server_messages.begin(), server_messages.end());
				}
				catch (_com_error &e)
				{
					if(SS_RPC_FAILED(e.Error()))
					{
						//Log Message about RPC failure to Key
						SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::GetLicenseMessageList() - RPC Error", MT_INFO, LicenseServerError::EC_UNKNOWN, MessageGenericError);
						SS_GENERATE_AND_DISPATCH_MESSAGE(LicensingMessageStringTable[MessageClientTimeout], MT_INFO, LicenseServerError::EC_CLIENT_TIMEOUT, MessageClientTimeout);
					}
				}


				//Get all the Software License messages...
				try
				{
					_variant_t vtServerSoftwareMessageList;
					SS_SLSERVER_ON_INTERFACE_FTCALL_HR(ISoftwareLicensingMessage, server->second, GetSoftwareLicenseMessageList, (VARIANT_TRUE, &vtServerSoftwareMessageList), hr);
					if (SUCCEEDED(hr))
					{
						LicensingSoftwareMessageList server_software_messages(vtServerSoftwareMessageList);
						licensing_software_message_cache.insert(licensing_software_message_cache.end(), server_software_messages.begin(), server_software_messages.end());
					}
				}
				catch (_com_error &e) 
				{
					if(SS_RPC_FAILED(e.Error()))
					{
						//Log Message about RPC failure to Key
						SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::GetSoftwareLicenseMessageList() - RPC Error", MT_INFO, LicenseServerError::EC_UNKNOWN, MessageGenericError);
						SS_GENERATE_AND_DISPATCH_MESSAGE(LicensingMessageStringTable[MessageClientTimeout], MT_INFO, LicenseServerError::EC_CLIENT_TIMEOUT, MessageClientTimeout);
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
					server_messages_list.insert(server_messages_list.end(), *m);
					++m;
				}
			}

			//filter the software message list, removing messages that are not relevant to the manager (by product)
			for (LicensingSoftwareMessageList::iterator mIt = licensing_software_message_cache.begin(); mIt != licensing_software_message_cache.end();)
			{
				LicensingSoftwareMessage lsm = LicensingSoftwareMessage(*mIt);
				if(m_product != mIt->productId)
				{
					mIt = licensing_software_message_cache.erase(mIt);
				}
				else
				{
					server_messages_list.insert(server_messages_list.end(), LicensingMessage(mIt->licenseServer, mIt->softwareLicense, mIt->messageTimestamp, mIt->messageType, mIt->messageId, mIt->message, mIt->messageErrorHr, 0, _variant_t(0.0,VT_DATE), 0));
					mIt++;
				}
			}

//swprintf_s(tmpbuf, 1024, L"server_messages_list.size: %d", server_messages_list.size());
//OutputDebugString(tmpbuf);
			*pvtMessageList = server_messages_list;

			//Dispatch all the messages before returning
			//DispatchLicenseMessageList(VARIANT_FALSE);

			if (clear_messages==VARIANT_TRUE)
			{
				licensing_message_cache.clear();
				licensing_software_message_cache.clear();
			}

		}
	}
	catch(_com_error&)
	{
		SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::GetLicenseMessageList() - COM Exception", MT_INFO, LicenseServerError::EC_UNKNOWN, MessageGenericError);
	}
	catch(...)
	{
		SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::GetLicenseMessageList() - Unknown Exception", MT_INFO, LicenseServerError::EC_UNKNOWN, MessageGenericError);
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
			DispatchLicenseMessage(_bstr_t(m->key_ident.c_str()), m->message_type, m->error, m->timestamp, _bstr_t(m->message.c_str()),m->message_id);
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
//OutputFormattedDebugString(L"CSolimarLicenseMgr::SendHeartbeat() - Server: %s, Start", (wchar_t*)server->first);
			SS_SLSERVER_FTCALL_HR(server->second, Heartbeat, (), hr);
//OutputFormattedDebugString(L"CSolimarLicenseMgr::SendHeartbeat() - Server: %s, Stop: 0x%x", (wchar_t*)server->first, hr);
		}
		catch (_com_error &e) 
		{
			if(SS_RPC_FAILED(e.Error()))
			{
				//Log Message about RPC failure to Key
				SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::SendHeartbeat() - RPC Error", MT_INFO, LicenseServerError::EC_UNKNOWN, MessageGenericError);
				SS_GENERATE_AND_DISPATCH_MESSAGE(LicensingMessageStringTable[MessageClientTimeout], MT_INFO, LicenseServerError::EC_CLIENT_TIMEOUT, MessageClientTimeout);
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
		((ui_level & UI_LEVEL_RESPONSE) && ((message_type==MT_WARNING) || (message_type==MT_ERROR)))
		||
		((ui_level & UI_LEVEL_CRITICAL) && (message_type==MT_ERROR))
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

void CSolimarLicenseMgr::DispatchLicenseMessage(BSTR key_ident, long message_type, long error, VARIANT vtTimestamp, BSTR message, long message_id)
{
	DispatchLicenseMessage(L"", key_ident, message_type, error, vtTimestamp, message, message_id);
}
void CSolimarLicenseMgr::DispatchLicenseMessage(BSTR license_server, BSTR key_ident, long message_type, long error, VARIANT vtTimestamp, BSTR message, long message_id)
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
			KeyMessageWriteEventLog(key_ident, message_type, error, vtTimestamp, message, message_id);
		}
	}

	std::wstring str_error_message = (_wcsicmp(L"", message) == 0) ? LicenseServerError::GetErrorMessage(error) : std::wstring(message);
	{
//OutputFormattedDebugString(L"CSolimarLicenseMgr::DispatchLicenseMessage(error: 0x%x, msg: %s)", error, str_error_message.c_str());
		SafeMutex mutex_messages(MessageListLock);
		licensing_software_message_cache.push_back(LicensingSoftwareMessage(std::wstring(license_server), std::wstring(key_ident), m_product, message_type, message_id, vtTimestamp, str_error_message, error));
	}
}

void CSolimarLicenseMgr::KeyMessageShowDialog(BSTR key_ident, unsigned int message_type, HRESULT error, VARIANT vtTimestamp, BSTR message)
{
	std::wstring str_error_message = (_wcsicmp(L"", message) == 0) ? LicenseServerError::GetErrorMessage(error) : std::wstring(message);
	if(str_error_message.length() != 0)
		ShowLicenseDialog(_bstr_t(L"Licensing Information"), _bstr_t(str_error_message.c_str()));
}

void CSolimarLicenseMgr::KeyMessageWriteEventLog(BSTR key_ident, unsigned int message_type, HRESULT error, VARIANT vtTimestamp, BSTR message, long message_id)
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

	std::wstring str_error_message;
	str_error_message = LicenseServerError::GetErrorMessage(error);
	unsigned int event_type = EVENTLOG_INFORMATION_TYPE;
	bool bKeyIdentEmpty = _wcsicmp(L"", key_ident) == 0;
	bool bErrorMessageEmpty = str_error_message.length() == 0;
	bool bMessageEmpty = _wcsicmp(L"", message) == 0;

	switch (message_type)
	{
		case MT_WARNING:
		case MT_INFO:
			_snwprintf_s(event_log_msg, sizeof(event_log_msg)/sizeof(wchar_t), L"Solimar Systems, Inc.\r\nProduct Licensing Status Message:%s%s%s%s",
				bKeyIdentEmpty ? L"" : L"\r\n\r\nLicenseID: ",
				bKeyIdentEmpty ? L"" : key_ident,
				(bMessageEmpty && bErrorMessageEmpty) ? L"" : L"\r\n\r\n",
				(bMessageEmpty && bErrorMessageEmpty) ? L"" : (!bMessageEmpty ? message : str_error_message.c_str())
				);
			event_type = (message_type==MT_WARNING) ? EVENTLOG_WARNING_TYPE : EVENTLOG_INFORMATION_TYPE;
			break;
		case MT_ERROR:
			_snwprintf_s(event_log_msg, sizeof(event_log_msg)/sizeof(wchar_t), L"Solimar Systems, Inc.\r\nProduct Licensing Error Message: 0x%08x%s%s%s%s%s%s",
				error,
				bKeyIdentEmpty ? L"" : L"\r\n\r\nLicenseID: ",
				bKeyIdentEmpty ? L"" : key_ident,
				bErrorMessageEmpty ? L"" : L"\r\n\r\n",
				bErrorMessageEmpty ? L"" : str_error_message.c_str(),
				bMessageEmpty ? L"" : L"\r\n\r\n",
				bMessageEmpty ? L"" : message
				);
			event_type = EVENTLOG_ERROR_TYPE;
			break;
	}

	// Don't flood event log with the same license message, check cache
	bool bCachedMsg = false;
	for(	std::list<std::wstring>::iterator msgCacheIt = m_licenseMsgEventLogCache.begin();
			msgCacheIt != m_licenseMsgEventLogCache.end();
			msgCacheIt++)
	{
		if(_wcsicmp(msgCacheIt->c_str(), event_log_msg) == 0)
		{
			bCachedMsg = true;
			m_licenseMsgEventLogCache.remove(*msgCacheIt);	// Remove item, so it can be re-added to the end of the cache
			break;
		}
	}

	m_licenseMsgEventLogCache.push_back(event_log_msg);	// Add message to the end of the cache
	if(m_licenseMsgEventLogCache.size() > 5)	// Cache the last 5 msg
		m_licenseMsgEventLogCache.pop_front();

	if(!bCachedMsg)
		LicenseServerError::WriteEventLog(event_log_msg, event_type, message_id, m_product);
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
// Refresh the Lic_ProdInfoAttribs object from the License Server it is connected to.
// There is an assumption that only one license server can be connected to
// S_OK if licenses for the product were found
// S_FALSE if licenses for the product were not found
// Error HRESULT if there was an error

HRESULT CSolimarLicenseMgr::RefreshSoftwareLicenseFromLicServers(bool _bLogError)
{
	HRESULT hr = S_OK;
//wchar_t debug_buf[1024];
//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::RefreshSoftwareLicenseFromLicServers() - Enter");
//OutputDebugStringW(debug_buf);	
	ENSURE_INITIALIZED;

	SafeMutex mutex(ServerListLock);

	bool bFoundProductAndVersion = false;
	bool bFoundExpiredProduct = false;
	bool bActivationExpired = false;
	std::wstring expiredDate = L"";
	long prodMajor = 0, prodMinor = 0;
	try
	{
		
		ServerList::iterator serverIt = m_bUsingBackupServers ? m_backupServers.begin() : m_servers.begin();
		ServerList::iterator serverEndIt = m_bUsingBackupServers ? m_backupServers.end() : m_servers.end();

		//There should only be 1 server...
		for(;serverIt != serverEndIt; ++serverIt)	//If there should only be 1 server
		{
			try
			{
				serverIt->second.software_license.licenses_total.clear();	//Clear current values out.
				serverIt->second.software_license.licenses_inuse.clear();	//Clear current values out.
				serverIt->second.software_license.licenses_allocated.clear();	//Clear current values out.

				BSTR bstrLicenseStream;
				SS_SLSERVER_ON_INTERFACE_FTCALL_HR(ISolimarSoftwareLicenseSvr, serverIt->second, GetSoftwareLicenseInfoByProduct_ForAll, (m_product, &bstrLicenseStream), hr);
				if(FAILED(hr))
					throw hr;
				
				// might need to check for NULL or empty
				Lic_PackageAttribs::Lic_ProductInfoAttribs prodAttribs;
				prodAttribs.InitFromString(bstrLicenseStream);
				SysFreeString(bstrLicenseStream);

				// Only add stuff at right version
				// if the product version requirements are satisfied
				//if (Version::TinyVersion(vtKeyProductVersion.uiVal,0) >= Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0))
				prodMajor = prodAttribs.product_Major;
				prodMinor = prodAttribs.product_Minor;

				if (	Version::ModuleVersion(prodAttribs.product_Major, prodAttribs.product_Minor, prodAttribs.product_SubMajor, prodAttribs.product_SubMinor) >= 
						(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0)))
				{
					hr = AssociateAppInstanceToSoftwareServer(&(serverIt->second), m_applicationInstance, _bLogError);
					if(SUCCEEDED(hr))
						bFoundProductAndVersion = true;

					HRESULT tmpHr;
					for(Lic_PackageAttribs::Lic_ProductInfoAttribs::TVector_Lic_ModuleInfoAttribsList::iterator modIt = prodAttribs.moduleList->begin();
						modIt != prodAttribs.moduleList->end();
						modIt++)
					{
						long licenses_allocated(0), licenses_inuse(0);

						// get the number of licenses in use for the key for all connections to it - licenses_inuse
						SS_SLSERVER_ON_INTERFACE_FTCALL_HR(ISolimarSoftwareLicenseSvr, serverIt->second, SoftwareModuleLicenseInUseByApp_ByProduct, (m_product, modIt->moduleID, &licenses_inuse), tmpHr);
						if(FAILED(tmpHr))
							licenses_inuse = 0;

						// get the number of licenses in allocated for this connection - licenses_allocated
						SS_SLSERVER_ON_INTERFACE_FTCALL_HR(ISolimarSoftwareLicenseSvr, serverIt->second, SoftwareModuleLicenseInUseByConnection_ByProduct, (m_product, modIt->moduleID, &licenses_allocated), tmpHr);
						if(FAILED(tmpHr))
							licenses_allocated = 0;

						serverIt->second.software_license.licenses_total[modIt->moduleID] = modIt->moduleValue;
						serverIt->second.software_license.licenses_inuse[modIt->moduleID] = licenses_inuse;
						serverIt->second.software_license.licenses_allocated[modIt->moduleID] = licenses_allocated;
					}

					VARIANT_BOOL server_license_valid(VARIANT_FALSE);
					SS_SLSERVER_ON_INTERFACE_FTCALL_HR(ISolimarSoftwareLicenseSvr, serverIt->second, SoftwareValidateLicenseApp_ByProduct, (m_product, &server_license_valid), tmpHr);
					if(FAILED(tmpHr) || server_license_valid==VARIANT_FALSE)
						serverIt->second.bValidLicensing = false;
					else
						serverIt->second.bValidLicensing = true;
				}
				else if (	Version::ModuleVersion(prodAttribs.product_Major, prodAttribs.product_Minor, prodAttribs.product_SubMajor, prodAttribs.product_SubMinor) == 
						(Version::ModuleVersion(0, 0, 0, 0)))
				{
					// CR.12672 - (prodAttribs.product_Major, prodAttribs.product_Minor) == (0,0,0,0) means no product license, check to see if expired.
					if((prodAttribs.bUseActivations == true) || (prodAttribs.bUseExpirationDate == true))
					{
						bFoundExpiredProduct = true;
						bActivationExpired = prodAttribs.bUseActivations;
						expiredDate = bActivationExpired ? prodAttribs.activationCurrentExpirationDate : prodAttribs.expirationDate;
					}
				}
			}
			catch(HRESULT &ehr)
			{
				hr = ehr;
			}
		}
	}
	catch (_com_error &e) 
	{
		if(SS_RPC_FAILED(e.Error()))
		{
			//Log Message about RPC failure to Key
			SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::RefreshSoftwareLicenseFromLicServers() - RPC Error", MT_INFO, LicenseServerError::EC_UNKNOWN, MessageGenericError);
			SS_GENERATE_AND_DISPATCH_MESSAGE(LicensingMessageStringTable[MessageClientTimeout], MT_INFO, LicenseServerError::EC_CLIENT_TIMEOUT, MessageClientTimeout);
		}
	}
	if(bFoundProductAndVersion == false && SUCCEEDED(hr))
	{
		hr = bFoundExpiredProduct ? LicenseServerError::EHR_LIC_SOFTWARE_PRODUCT_LIC_EXPIRED : LicenseServerError::EHR_LIC_SOFTWARE_PRODUCT_NO_VERSION;
	}

	
	if(FAILED(hr))
	{
		if(hr == LicenseServerError::EHR_LIC_SOFTWARE_PRODUCT_NO_VERSION || hr == LicenseServerError::EHR_LIC_SOFTWARE_PRODUCT_LIC_EXPIRED)
		{
			std::wstring wstrProdName = L"Unknown";
			Lic_PackageAttribs::Lic_SoftwareSpecAttribs::TMap_Lic_ProductSoftwareSpecAttribsMap::iterator prodSpecIt = m_softwareSpec.productSpecMap->find(m_product);
			if(prodSpecIt != m_softwareSpec.productSpecMap->end())
				wstrProdName = prodSpecIt->second.productName;

			wchar_t errorBuf[1024];
			if(hr == LicenseServerError::EHR_LIC_SOFTWARE_PRODUCT_NO_VERSION)
			{
				swprintf_s(errorBuf, _countof(errorBuf), L"Attempting to run Product: %s Version: %d.%d. You are licensed to run only Version: %d.%d or lower (License Server)",
					wstrProdName.c_str(),
					m_prod_ver_major,
					m_prod_ver_minor,
					prodMajor,
					prodMinor
					);
			}
			else if(hr == LicenseServerError::EHR_LIC_SOFTWARE_PRODUCT_LIC_EXPIRED)
			{
				swprintf_s(
					errorBuf, 
					_countof(errorBuf), 
					bActivationExpired ? L"Activation Expired on %s" : L"Expired on %s",
					expiredDate.c_str()
					);
			}
			LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(hr, std::wstring(errorBuf), __uuidof(CSolimarLicenseMgr), __uuidof(ISolimarLicenseMgr7));
		}
	}
	return hr;
}

//
// refresh the list of the keys on the currently connected servers
// S_OK if some keys were found
// S_FALSE if no keys are found
// Error HRESULT if there was an error
HRESULT CSolimarLicenseMgr::RefreshKeyListFromLicServers(bool _bLogError)
{
	HRESULT hr = S_OK;
//wchar_t debug_buf[1024];
//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::RefreshKeyListFromLicServers() - Enter");
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
//_snwprintf(debug_buf, 1024, L"    CSolimarLicenseMgr::RefreshKeyList() - ValidateKeyList() - pre");
//OutputDebugStringW(debug_buf);	
						hr = ValidateKeyList(&(server->second), pvtKeyIdent, vtKeyList.parray->rgsabound[0].cElements);
//_snwprintf(debug_buf, 1024, L"    CSolimarLicenseMgr::RefreshKeyList() - ValidateKeyList() - post");
//OutputDebugStringW(debug_buf);	
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

								SS_GENERATE_AND_DISPATCH_MESSAGE(LicenseServerError::GetErrorMessage(hr).c_str(), MT_INFO, hr, MessageGenericError);
							}
							continue;
						}
					}

					if(m_bLockKeyByAppInstance)
					{
//_snwprintf(debug_buf, 1024, L"    CSolimarLicenseMgr::RefreshKeyList() - LockOneOfEachKeyConfiguration() - pre");
//OutputDebugStringW(debug_buf);	
						hr = LockOneOfEachKeyConfiguration(&(server->second), pvtKeyIdent, vtKeyList.parray->rgsabound[0].cElements, _bLogError);
//_snwprintf(debug_buf, 1024, L"    CSolimarLicenseMgr::RefreshKeyList() - LockOneOfEachKeyConfiguration() - post");
//OutputDebugStringW(debug_buf);	
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
								SS_GENERATE_AND_DISPATCH_MESSAGE(LicenseServerError::GetErrorMessage(hr).c_str(), MT_INFO, hr, MessageGenericError);
							}
						}
					}
					else //if(!m_bLockKeyByAppInstance)
					{
//_snwprintf(debug_buf, 1024, L"    CSolimarLicenseMgr::RefreshKeyList() - AssociateAppInstanceToBaseKey() - pre");
//OutputDebugStringW(debug_buf);
						hr = AssociateAppInstanceToBaseKey(&(server->second), pvtKeyIdent, vtKeyList.parray->rgsabound[0].cElements, _bLogError);
					}


					//Cycle through all the keys, calculate unlimitedness
//_snwprintf(debug_buf, 1024, L"    CSolimarLicenseMgr::RefreshKeyList() - SetUnlimitedModulesOnKeys() - pre");
//OutputDebugStringW(debug_buf);
					SetUnlimitedModulesOnKeys(&(server->second), pvtKeyIdent, vtKeyList.parray->rgsabound[0].cElements, _bLogError);

               RefreshApplicationInstanceInUseCache(&(server->second), _bLogError);
//_snwprintf(debug_buf, 1024, L"    CSolimarLicenseMgr::RefreshKeyList() - SetUnlimitedModulesOnKeys() - post");
//OutputDebugStringW(debug_buf);
					SafeArrayUnaccessData(vtKeyList.parray);
				}
			}
		}
		catch (_com_error &e) 
		{
			if(SS_RPC_FAILED(e.Error()))
			{
				//Log Message about RPC failure to Key
				SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::RefreshKeyListFromLicServers() - RPC Error", MT_INFO, LicenseServerError::EC_UNKNOWN, MessageGenericError);
				SS_GENERATE_AND_DISPATCH_MESSAGE(LicensingMessageStringTable[MessageClientTimeout], MT_INFO, LicenseServerError::EC_CLIENT_TIMEOUT, MessageClientTimeout);
			}
			hr = e.Error();
		}
	}

	found_keys = SUCCEEDED(hr);

	// remove any keys that are not present and have no licenses obtained
	if (FAILED(hr))
		RemoveObsoleteKeysFromCache();
	else
	{
//_snwprintf(debug_buf, 1024, L"    CSolimarLicenseMgr::RefreshKeyList() - RemoveObsoleteKeysFromCache() - pre");
//OutputDebugStringW(debug_buf);
		hr = RemoveObsoleteKeysFromCache();
//_snwprintf(debug_buf, 1024, L"    CSolimarLicenseMgr::RefreshKeyList() - RemoveObsoleteKeysFromCache() - post");
//OutputDebugStringW(debug_buf);
	}



//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::RefreshKeyListFromLicServers() - Leave 0 - hr: 0x%08x, found_keys: %s", hr, found_keys ? L"true" : L"false");
//OutputDebugStringW(debug_buf);	

	if (FAILED(hr)) return hr;
    
	// S_OK if some keys were found
	// S_FALSE if no keys are found
	// Error HRESULT if there was an error
//_snwprintf(debug_buf, 1024, L"    CSolimarLicenseMgr::RefreshKeyList() - Leave");
//OutputDebugStringW(debug_buf);	
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

		// check that the key has the requisite product version and etc.
		// CR.11272 - Moved check up higher to reduce remote calls
		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product ID"].id, &vtKeyProductID);
		if (FAILED(hr) || m_keyspec.products[m_productKeyID].id!=vtKeyProductID.uiVal) {hr = S_OK; continue;}

		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product Version"].id, &vtKeyProductVersion);
		if (FAILED(hr) || Version::TinyVersion(vtKeyProductVersion.uiVal,0) < Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0))) {hr = S_OK; continue;}
		
		//KeyValidateLicense Calls KeyIsPresent() & KeyIsActive()
		hr = pServerInfo->LicenseServer->KeyValidateLicense(key_ident, &key_license_valid);
		if (FAILED(hr) || key_license_valid==VARIANT_FALSE) {hr = S_OK; continue;}

		// if the product id and product version requirements are satisfied
		if (Version::TinyVersion(vtKeyProductVersion.uiVal,0) >= Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0)) && 
			m_keyspec.products[m_productKeyID].id==vtKeyProductID.uiVal)
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
					KEYVerification=0xff,
				} KeyTypeFlagX;
				for (KeySpec::Product::data_list_t::iterator module = m_keyspec.products[m_productKeyID].data.begin(); module != m_keyspec.products[m_productKeyID].data.end(); ++module)
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

		// check that the key has the requisite product version and etc.
		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product ID"].id, &vtKeyProductID);
		if (FAILED(hr) || m_keyspec.products[m_productKeyID].id!=vtKeyProductID.uiVal) {hr = S_OK; continue;}

		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product Version"].id, &vtKeyProductVersion);
		if (FAILED(hr) || Version::TinyVersion(vtKeyProductVersion.uiVal,0) < Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0))) {hr = S_OK; continue;}
		
		// check that the key is present & active, don't check for validity because unlimited has not been called yet.
		VARIANT_BOOL key_active(VARIANT_FALSE);
		hr = pServerInfo->LicenseServer->KeyIsActive(key_ident, &key_active);
		if (FAILED(hr) || key_active==VARIANT_FALSE) {hr = S_OK; continue;}

		VARIANT_BOOL key_present(VARIANT_FALSE);
		hr = pServerInfo->LicenseServer->KeyIsPresent(key_ident, &key_present);
		if (FAILED(hr) || key_present==VARIANT_FALSE) {hr = S_OK; continue;}

		// if the product id and product version requirements are satisfied
		if (Version::TinyVersion(vtKeyProductVersion.uiVal,0) >= Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0)) && 
			m_keyspec.products[m_productKeyID].id==vtKeyProductID.uiVal)
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
				KEYVerification=0xff,
			} KeyTypeFlagX;
			for (KeySpec::Product::data_list_t::iterator module = m_keyspec.products[m_productKeyID].data.begin(); module != m_keyspec.products[m_productKeyID].data.end(); ++module)
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
	//List of need keytype that need to be locked to m_applicationInstanceKey
	std::list<_bstr_t> neededKeyTypeList;
	for(std::list<_bstr_t>::iterator keyTypeIt = keyTypeList.begin(); keyTypeIt != keyTypeList.end(); keyTypeIt++)
		neededKeyTypeList.push_back(_bstr_t(*keyTypeIt, true));

	//Remove the keytypes that are already locked to m_applicationInstanceKey
	_bstr_tMap currentAppInstanceMap = appInstanceToKeyInfoMap[m_applicationInstanceKey];	//Find the map of this app instances
	for(_bstr_tMap::iterator keyTypeIt = currentAppInstanceMap.begin(); keyTypeIt != currentAppInstanceMap.end(); keyTypeIt++)
	{
		hr = pServerInfo->LicenseServer->AddApplicationInstance(keyTypeIt->first, m_applicationInstanceKey, VARIANT_TRUE);

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
					hr = pServerInfo->LicenseServer->AddApplicationInstance(freeKeyIt->first, m_applicationInstanceKey, VARIANT_TRUE);
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
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::FreeAllKeys(%s, %d) - Enter", (wchar_t*)pServerInfo->name, count);
//OutputDebugString(tmpbuf);

	//first cycle through all keys, set all given Application Instances to empty to free
	for (int i = 0; i<count; ++i)
	{
		_bstr_t key_ident = pvtKeyList[i].bstrVal;
		VARIANT vtKeyProductID, vtKeyProductVersion;
		// check that the key is present and valid
		VARIANT_BOOL key_license_valid(VARIANT_FALSE);

		// check that the key has the requisite product version and etc.
		// CR.11272 - Moved check up higher to reduce remote calls
		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product ID"].id, &vtKeyProductID);
		if (FAILED(hr) || m_keyspec.products[m_productKeyID].id!=vtKeyProductID.uiVal) {hr = S_OK; continue;}

		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product Version"].id, &vtKeyProductVersion);
		if (FAILED(hr) || Version::TinyVersion(vtKeyProductVersion.uiVal,0) < Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0))) {hr = S_OK; continue;}
		
		// check that the key is present & active, don't check for validity because validatiy should be unimportant when freeing.
		VARIANT_BOOL key_active(VARIANT_FALSE);
		hr = pServerInfo->LicenseServer->KeyIsActive(key_ident, &key_active);
		if (FAILED(hr) || key_active==VARIANT_FALSE) {hr = S_OK; continue;}

		VARIANT_BOOL key_present(VARIANT_FALSE);
		hr = pServerInfo->LicenseServer->KeyIsPresent(key_ident, &key_present);
		if (FAILED(hr) || key_present==VARIANT_FALSE) {hr = S_OK; continue;}

		// if the product id and product version requirements are satisfied
		if (Version::TinyVersion(vtKeyProductVersion.uiVal,0) >= Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0)) && 
			m_keyspec.products[m_productKeyID].id==vtKeyProductID.uiVal)
		{
			//Try to remove application instance, don't throw error if it was never associated with...
			pServerInfo->LicenseServer->RemoveApplicationInstance(key_ident, m_applicationInstanceKey);
		}
	}

	//Remove all keys from being seen...
	for (ServerInfo::KeyList::iterator k = pServerInfo->keys.begin(); k != pServerInfo->keys.end(); ++k)
	{
		// mark all keys as not being present
		// if they are seen on the server scan, they will be marked as present
		k->second.KeyPresent = false;
	}
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::FreeAllKeys() - Leave");
//OutputDebugString(tmpbuf);
	return hr;
}



// Sets the appropiate modules on keys to be unlimited.  Calculate: #of base keys X unlimited number for modules.
HRESULT CSolimarLicenseMgr::SetUnlimitedModulesOnKeys(ServerInfo* pServerInfo, VARIANT *pvtKeyList, long count, bool bLogError)
{
	std::map<unsigned int, int> moduleCounterMap;
	int baseKeysSeen = 0;
	HRESULT hr = S_OK;

//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"        CSolimarLicenseMgr::SetUnlimitedModulesOnKeys (AppID: %s, app_instance_lock_key: %s) - Enter", (wchar_t*)m_applicationInstanceKey, m_bLockKeyByAppInstance ? L"true" : L"false");
//OutputDebugString(tmpbuf);
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
				KEYVerification=0xff,
			} KeyTypeFlagX;

			//Calculate Base Keys Seen
			if(key_type!=KEYAddon)
				baseKeysSeen++;

			//Calculate Modules Seen
			for (KeySpec::Product::data_list_t::iterator moduleIt = m_keyspec.products[m_productKeyID].data.begin(); moduleIt != m_keyspec.products[m_productKeyID].data.end(); ++moduleIt)
			{
				if (moduleIt->isLicense)
				{
					///if((pServerInfo->bUseOnlySharedLicenses && module->isSharable) || !pServerInfo->bUseOnlySharedLicenses)
					unsigned long newTotal =  moduleCounterMap[moduleIt->id] + keyIt->second.licenses_total[moduleIt->id];
					if ((keyIt->second.licenses_total[moduleIt->id] >= 0x7FFFFFFF) || (newTotal>= 0x7FFFFFFF))
						newTotal = 0x7FFFFFFF;
					moduleCounterMap[moduleIt->id] = newTotal;
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

			// Verify that the key is for this product
			// CR.11272 - Moved check up higher to reduce remote calls
			hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product ID"].id, &vtKeyProductID);
			if (FAILED(hr) || m_keyspec.products[m_productKeyID].id!=vtKeyProductID.uiVal) {hr = S_OK; continue;}

			// check that the key has the requisite product version and etc.
			hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product Version"].id, &vtKeyProductVersion);
			if (FAILED(hr) || Version::TinyVersion(vtKeyProductVersion.uiVal,0) < Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0))) {hr = S_OK; continue;}

			// check that the key is present, don't check for validity because it is being set to unlimited if suppose to.
			VARIANT_BOOL key_active(VARIANT_FALSE);
			hr = pServerInfo->LicenseServer->KeyIsActive(key_ident, &key_active);
			if (FAILED(hr) || key_active==VARIANT_FALSE) {hr = S_OK; continue;}

			VARIANT_BOOL key_present(VARIANT_FALSE);
			hr = pServerInfo->LicenseServer->KeyIsPresent(key_ident, &key_present);
			if (FAILED(hr) || key_present==VARIANT_FALSE) {hr = S_OK; continue;}

			// CR.11272 - Moved check up higher to reduce remote calls = remove if statement
			// if the product id and product version requirements are satisfied
			//if (Version::TinyVersion(vtKeyProductVersion.uiVal,0) >= Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0)) && 
			//	m_keyspec.products[m_productKeyID].id==vtKeyProductID.uiVal)
			//{
//swprintf_s(tmpbuf, 1024, L"        (key_ident: %s, m_applicationInstanceKey: %s)", (wchar_t*)pvtKeyList[i].bstrVal, (wchar_t*)m_applicationInstance);
//OutputDebugString(tmpbuf);

			for(std::map<unsigned int, int>::iterator tmpModIt = moduleCounterMap.begin(); tmpModIt != moduleCounterMap.end(); tmpModIt++)
			{
				KeySpec::Module &module(m_keyspec.products[m_productKeyID][tmpModIt->first]);
				bool bSetUnlimited = false;
				if(module.unlimited > 0)
				{
					if(module.pool != 0)	//Is a pooled module
					{
						//Factor in base keys in unlimited Calculation
						KeySpec::Module &pool(m_keyspec.products[m_productKeyID][module.pool]);
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

				if((pServerInfo->keys[pvtKeyList[i].bstrVal].licenses_total[module.id] != 0x7FFFFFF && bSetUnlimited) ||
				   (pServerInfo->keys[pvtKeyList[i].bstrVal].licenses_total[module.id] == 0x7FFFFFF && !bSetUnlimited))
				{
					hr = pServerInfo->LicenseServer->KeyModuleLicenseUnlimited(pvtKeyList[i].bstrVal, module.id, bSetUnlimited ? VARIANT_TRUE : VARIANT_FALSE);
					if(SUCCEEDED(hr))
					{
						// get the total number of new licenses 
						long licenses_total = 0;
						hr = pServerInfo->LicenseServer->KeyModuleLicenseTotal(pvtKeyList[i].bstrVal, module.id, &licenses_total);
						if (SUCCEEDED(hr))
							pServerInfo->keys[pvtKeyList[i].bstrVal].licenses_total[module.id] = licenses_total;
					}
				}
				//}
			}
		}
//swprintf_s(tmpbuf, 1024, L"        CSolimarLicenseMgr::SetUnlimitedModulesOnKeys - 2nd Loop: End");
//OutputDebugString(tmpbuf);
	}
	return hr;
}


// refresh ApplicationInstance InUse list
HRESULT CSolimarLicenseMgr::RefreshApplicationInstanceInUseCache(ServerInfo* pServerInfo, bool bLogError)
{
	bool bValidKeySeen(false);
	for (ServerInfo::KeyList::iterator keyIt = pServerInfo->keys.begin(); keyIt != pServerInfo->keys.end(); ++keyIt)
	{
		if(keyIt->second.KeyPresent && keyIt->second.KeyValid)
		{
			bValidKeySeen = true;
			break;
		}
	}

	if(bValidKeySeen)
	{
		if(pServerInfo->keys.size() != 0)
			appInstanceInUseModuleCacheMap.clear();
		for (ServerInfo::KeyList::iterator keyIt = pServerInfo->keys.begin(); keyIt != pServerInfo->keys.end(); ++keyIt)
		{
			if(keyIt->second.KeyPresent && keyIt->second.KeyValid)
			{
				for (KeySpec::Product::data_list_t::iterator moduleIt = m_keyspec.products[m_productKeyID].data.begin(); moduleIt != m_keyspec.products[m_productKeyID].data.end(); ++moduleIt)
				{
					if (moduleIt->isLicense)
					{
						appInstanceInUseModuleCacheMap[moduleIt->id] += keyIt->second.licenses_inuse_byapp[moduleIt->id];
					}
				}
			}
		}
	}
	return S_OK;
}
// Side Effect: if can associate App Instance with a base key, will adds all keys to the cache
// Will only add non remote access keys (Application Instance == 0) to License Managers on the same machine as the License Server
HRESULT CSolimarLicenseMgr::AssociateAppInstanceToBaseKey(ServerInfo* pServerInfo, VARIANT *pvtKeyList, long count, bool bLogError)
{
	HRESULT hr(S_OK);
//wchar_t debug_buf[1024];
//_snwprintf(debug_buf, 1024, L"    CSolimarLicenseMgr::AssociateAppInstanceToBaseKey() - Enter");
//OutputDebugStringW(debug_buf);
	if(m_bLockKeyByAppInstance)
		return E_FAIL;	//Only m_bLockKeyByAppInstance must have all keys match

	std::list<_bstr_t> baseKeyWithNoAppInstanceFirstPrefList;	//list of free base keys, first preference
	std::list<_bstr_t> baseKeyWithNoAppInstanceSecondPrefList;	//list of free base keys, second preference
	_bstr_t keysAlreadyAssignedToAnAppInstance;
	std::list<_bstr_t> keyToAddList;	//List of all keys to add to the cache
	bool bIsLocalServer = IsLocalMachine(pServerInfo->name);
	bool bFoundAppInstance = false;

//_snwprintf(debug_buf, 1024, L"    CSolimarLicenseMgr::AssociateAppInstanceToBaseKey() - For Loop() - Start");
//OutputDebugStringW(debug_buf);
	for (int i = 0; i<count; ++i)
	{
		_bstr_t key_ident = pvtKeyList[i].bstrVal;
		VARIANT vtKeyProductID, vtKeyProductVersion;
		// CR.11272 - Moved check up higher to reduce remote calls
		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product ID"].id, &vtKeyProductID);
		if (FAILED(hr) || m_keyspec.products[m_productKeyID].id!=vtKeyProductID.uiVal) {hr = S_OK; continue;}

		// check that the key has the requisite product version and etc.
		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product Version"].id, &vtKeyProductVersion);
		if (FAILED(hr) || Version::TinyVersion(vtKeyProductVersion.uiVal,0) < Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0))) {hr = S_OK; continue;}
		
		// check that the key is present & active, don't check for validity because unlimited has not been called yet.
		VARIANT_BOOL key_active(VARIANT_FALSE);
		hr = pServerInfo->LicenseServer->KeyIsActive(key_ident, &key_active);
		if (FAILED(hr) || key_active==VARIANT_FALSE) {hr = S_OK; continue;}

		VARIANT_BOOL key_present(VARIANT_FALSE);
		hr = pServerInfo->LicenseServer->KeyIsPresent(key_ident, &key_present);
		if (FAILED(hr) || key_present==VARIANT_FALSE) {hr = S_OK; continue;}

		// CR.11272 - Moved check up higher to reduce remote calls - removed if statement
		// if the product id and product version requirements are satisfied
		//if (Version::TinyVersion(vtKeyProductVersion.uiVal,0) >= Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0)) && 
		//	m_keyspec.products[m_productKeyID].id==vtKeyProductID.uiVal)
		//{
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
				if(wcsicmp(bstrApplicationInstance, m_applicationInstanceKey) == 0)
				{
					//Found a key that is already associated with the application instance, leave
					hr = pServerInfo->LicenseServer->AddApplicationInstance(key_ident, m_applicationInstanceKey, VARIANT_FALSE);
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::AssociateAppInstanceToBaseKey - pServerInfo->LicenseServer->AddApplicationInstance(%s, %s, true) = %s", (wchar_t*)key_ident, (wchar_t*)m_applicationInstanceKey, hr==S_OK ? L"true" : L"false");
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
				else if(wcscmp(m_applicationInstanceKey, _bstr_t(L"")) == 0)
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
		//}
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
					m_applicationInstanceKey, 
					VARIANT_FALSE);	//Don't Lock Key
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
						m_applicationInstanceKey, 
						VARIANT_FALSE);	//Don't Lock Key
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
						m_applicationInstanceKey, 
						VARIANT_FALSE);	//Don't Lock Key
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::AssociateAppInstanceToBaseKey - pServerInfo->LicenseServer->AddApplicationInstance(%s, %s, false) = %s", (wchar_t*)*keyIt, (wchar_t*)m_applicationInstanceKey, hr==S_OK ? L"true" : L"false");
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

//_snwprintf(debug_buf, 1024, L"    CSolimarLicenseMgr::AssociateAppInstanceToBaseKey() - Leave");
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
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::GetAppInstanceFromKey() - Enter - pServerInfo->LicenseServer->GetApplicationInstanceList(%s, %s)", (wchar_t*)bstrKeyIdent, (wchar_t*)m_applicationInstanceKey, (wchar_t*)app_instance_key_ident);
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


				//if(pElement[(2*idx)+1].boolVal == VARIANT_TRUE && wcscmp(m_applicationInstanceKey, pElement[idx].bstrVal) == 0)
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
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::AddKeyToCache(serverName: %s, bstrKeyIdent: %s) - AppID: %s", (wchar_t*)pServerInfo->name, (wchar_t*)bstrKeyIdent, (wchar_t*)m_applicationInstanceKey);
//OutputDebugString(tmpbuf);
	HRESULT hr(S_OK);
	_bstr_t key_ident = bstrKeyIdent;
	VARIANT vtKeyProductID, vtKeyProductVersion;
	for(;;)
	{
		// CR.11272 - Moved check up higher to reduce remote calls
		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product ID"].id, &vtKeyProductID);
		if (FAILED(hr) || m_keyspec.products[m_productKeyID].id!=vtKeyProductID.uiVal) {hr = S_OK; continue;}

		// check that the key is present & active, don't check for validity because unlimited has not been called yet.
		VARIANT_BOOL key_active(VARIANT_FALSE);
		hr = pServerInfo->LicenseServer->KeyIsActive(key_ident, &key_active);
		if (FAILED(hr) || key_active==VARIANT_FALSE) {hr = S_OK; continue;}

		VARIANT_BOOL key_present(VARIANT_FALSE);
		hr = pServerInfo->LicenseServer->KeyIsPresent(key_ident, &key_present);
		if (FAILED(hr) || key_present==VARIANT_FALSE) {hr = S_OK; continue;}


		
		// check that the key has the requisite product version and etc.
		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product Version"].id, &vtKeyProductVersion);
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
		if(m_keyspec.products[m_productKeyID].id==vtKeyProductID.uiVal)
		{
			m_productkeys[key_ident] = true;
			if (Version::TinyVersion(vtKeyProductVersion.uiVal,0) >= Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0)))
			{
				// if a specific key is requested, but this one is not it, skip this key
				if (!(m_single_key && m_specific_single_key_ident.length()>0 && m_specific_single_key_ident!=key_ident))
				{
					pServerInfo->keys[key_ident].KeyPresent = true;
					pServerInfo->keys[key_ident].KeyValid = true;
					
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
						EXTENDED_TRIAL5=7, BASE=2, UNUSED=10, DEACTIVATED=11, EXTENDED_TRIAL6=12,
						EXTENDED_TRIAL7=13, EXTENDED_TRIAL8=14, EXTENDED_TRIAL9=15, EXTENDED_TRIAL10=16,
						EXTENDED_TRIAL11=17, EXTENDED_TRIAL12=18, EXTENDED_TRIAL13=19, EXTENDED_TRIAL14=20,
						EXTENDED_TRIAL15=21, EXTENDED_TRIAL16=22
					};

					KeyIsTrial = (
						vtKeyStatus.ulVal == INITIAL_TRIAL ||
						vtKeyStatus.ulVal == EXTENDED_TRIAL ||
						vtKeyStatus.ulVal == EXTENDED_TRIAL2 ||
						vtKeyStatus.ulVal == EXTENDED_TRIAL3 ||
						vtKeyStatus.ulVal == EXTENDED_TRIAL4 ||
						vtKeyStatus.ulVal == EXTENDED_TRIAL5 ||
						vtKeyStatus.ulVal == EXTENDED_TRIAL6 ||
						vtKeyStatus.ulVal == EXTENDED_TRIAL7 ||
						vtKeyStatus.ulVal == EXTENDED_TRIAL8 ||
						vtKeyStatus.ulVal == EXTENDED_TRIAL9 ||
						vtKeyStatus.ulVal == EXTENDED_TRIAL10 ||
						vtKeyStatus.ulVal == EXTENDED_TRIAL11 ||
						vtKeyStatus.ulVal == EXTENDED_TRIAL12 ||
						vtKeyStatus.ulVal == EXTENDED_TRIAL13 ||
						vtKeyStatus.ulVal == EXTENDED_TRIAL14 ||
						vtKeyStatus.ulVal == EXTENDED_TRIAL15 ||
						vtKeyStatus.ulVal == EXTENDED_TRIAL16 ||
						vtKeyStatus.ulVal == UNINITIALIZED_TRIAL
					);
					
					wchar_t licensing_message_buffer[256];
					
					// vtExpirationDate is in universal time, convert to local time for messages.
					_variant_t vtLocalizedExpirationDate = TimeHelper::TimeTToVariant(TimeHelper::VariantToTimeT(vtExpirationDate), true);
					

					// if the key has just become inactive add message
					if (TrialInfoInitialized && !KeyActive && pServerInfo->keys[key_ident].KeyActive)
					{
						swprintf_s(licensing_message_buffer, sizeof(licensing_message_buffer)/sizeof(wchar_t), LicensingMessageStringTable[MessageLMTempKeyExpired], KeyTrialHoursLeft, (wchar_t*)_bstr_t(vtLocalizedExpirationDate));
						AddLicensingMessage(LicensingMessage(std::wstring((wchar_t*)pServerInfo->name), std::wstring((wchar_t*)key_ident), vtServerTime, MT_WARNING, MessageLMTempKeyExpired, std::wstring(licensing_message_buffer), 0, vtKeyStatus.ulVal, vtExpirationDate, KeyTrialHoursLeft));
					}
					// if the key has just become active add message
					if (TrialInfoInitialized && KeyActive && !pServerInfo->keys[key_ident].KeyActive)
					{
						swprintf_s(licensing_message_buffer, sizeof(licensing_message_buffer)/sizeof(wchar_t), LicensingMessageStringTable[MessageLMTempKeyActive], KeyTrialHoursLeft, (wchar_t*)_bstr_t(vtLocalizedExpirationDate));
						AddLicensingMessage(LicensingMessage(std::wstring((wchar_t*)pServerInfo->name), std::wstring((wchar_t*)key_ident), vtServerTime, MT_INFO, MessageLMTempKeyActive, std::wstring(licensing_message_buffer), 0, vtKeyStatus.ulVal, vtExpirationDate, KeyTrialHoursLeft));
					}
					// if the key had not been seen before and is inactive
					if (KeyIsTrial && !KeyActive && !TrialInfoInitialized)
					{
						swprintf_s(licensing_message_buffer, sizeof(licensing_message_buffer)/sizeof(wchar_t), LicensingMessageStringTable[MessageLMTempKeyExpired], KeyTrialHoursLeft, (wchar_t*)_bstr_t(vtLocalizedExpirationDate));
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
						swprintf_s(licensing_message_buffer, sizeof(licensing_message_buffer)/sizeof(wchar_t), LicensingMessageStringTable[MessageLMTempKeyStatus], KeyTrialHoursLeft, (wchar_t*)_bstr_t(vtLocalizedExpirationDate));
						AddLicensingMessage(LicensingMessage(std::wstring((wchar_t*)pServerInfo->name), std::wstring((wchar_t*)key_ident), vtServerTime, MT_INFO, MessageLMTempKeyStatus, std::wstring(licensing_message_buffer), 0, vtKeyStatus.ulVal, vtExpirationDate, KeyTrialHoursLeft));
					}
					
					pServerInfo->keys[key_ident].KeyActive = KeyActive;
					pServerInfo->keys[key_ident].KeyTrialInfoInitialized = true;
					pServerInfo->keys[key_ident].KeyIsTrial = KeyIsTrial;
					pServerInfo->keys[key_ident].KeyTrialHoursLeft = KeyTrialHoursLeft;
					pServerInfo->keys[key_ident].KeyExpiresDaysLeft = KeyExpiresDaysLeft;
					
					// refresh the cache of licenses on the key
					// for each module in the key spec for the product
					for (KeySpec::Product::data_list_t::iterator module = m_keyspec.products[m_productKeyID].data.begin(); module != m_keyspec.products[m_productKeyID].data.end(); ++module)
					{
						if (module->isLicense)
						{
							long module_id = static_cast<long>(module->id);
							long licenses_total(0), licenses_allocated(0), licenses_inuse(0), licenses_inuse_byapp(0);

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

								// get the number of licenses in use for the given application for the key for all connections to it
								hr = pServerInfo->LicenseServer->KeyModuleLicenseInUse_ByApp(key_ident, module_id, &licenses_inuse_byapp);
								if (FAILED(hr)) break;
							}
							
							pServerInfo->keys[key_ident].licenses_total[module_id] = licenses_total;
							pServerInfo->keys[key_ident].licenses_allocated[module_id] = licenses_allocated;
							pServerInfo->keys[key_ident].licenses_inuse[module_id] = licenses_inuse;
							pServerInfo->keys[key_ident].licenses_inuse_byapp[module_id] = licenses_inuse_byapp;
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
				vtTimestamp = TimeHelper::TimeTToVariant(timestamp, false);

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
					_bstr_t(message),
					MessageGenericError
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
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"CSolimarLicenseMgr::ValidateLicenseCache() - Enter - m_bUsingBackupServers: %d", m_bUsingBackupServers);
//OutputDebugString(tmpbuf);
	ENSURE_INITIALIZED;
	SafeMutex mutex(ServerListLock);	
	
	outstanding_licenses = m_allocated_licenses;
	
	ServerList::iterator server = m_bUsingBackupServers ? m_backupServers.begin() : m_servers.begin();
	ServerList::iterator serverEnd = m_bUsingBackupServers ? m_backupServers.end() : m_servers.end();

	// foreach server
	for (;server != serverEnd; ++server)
	{
		if(server->second.bUseSoftwareLicensing)
		{
			// for each module
			for (ModuleLicenseMap::iterator m = outstanding_licenses.begin(); m != outstanding_licenses.end(); ++m)
			{
				//CR.FIX.14599 - add modules to release to the outstanding_licenses, even if not valid licensing
				if(server->second.bValidLicensing || (m->second - server->second.software_license.licenses_allocated[m->first]) < 0)
					m->second -= server->second.software_license.licenses_allocated[m->first];
			}
		}
		else	// use protection key licensing
		{
			// foreach key
			for (ServerInfo::KeyList::iterator k = server->second.keys.begin(); k != server->second.keys.end(); ++k)
			{
				if (	k->second.KeyPresent && 
					k->second.KeyValid && 
					(k->second.KeyObtained || (*server).second.bUseOnlySharedLicenses))
				{
					// for each module
					for (ModuleLicenseMap::iterator m = outstanding_licenses.begin(); m != outstanding_licenses.end(); ++m)
					{
						bool bModuleIsSharable = false;
						if((*server).second.bUseOnlySharedLicenses)
						{
							KeySpec::Module &module(m_keyspec.products[m_productKeyID][m->first]);
							bModuleIsSharable = module.isSharable;
						}

						if(((*server).second.bUseOnlySharedLicenses && bModuleIsSharable) || !(*server).second.bUseOnlySharedLicenses)
							m->second -= k->second.licenses_allocated[m->first];
					}
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
	double timeDiffSeconds = difftime(m_dtRefreshLicenses, timeNow);

	// don't call RefreshKeyListFromLicServers if just made the same call within the last 5 seconds
	if(abs(timeDiffSeconds) > 5.0)	
	{
		hr = RefreshLicensingFromLicServers();
		//hr = m_bConfiguredForSoftwareLicense==true ? RefreshSoftwareLicenseFromLicServers() : RefreshKeyList();
		if(SUCCEEDED(hr))
			m_dtRefreshLicenses = time(NULL);
	}

	if (FAILED(hr)) {	return hr; }
	
	// check to see if there is a difference between 
	// the number of licenses obtained and those backed up by keys
	ModuleLicenseMap outstanding_licenses;
	hr = ValidateLicenseCache(outstanding_licenses);
	if (FAILED(hr)) {	return hr; }
	
	// obtain and release licenses where appropriate
	HRESULT tempHr = hr;
	for (ModuleLicenseMap::iterator module = outstanding_licenses.begin(); module != outstanding_licenses.end(); ++module)
	{
//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::RefreshLicenses() - moduleID: %d, amount: %d", module->first, module->second);
//OutputDebugStringW(debug_buf);
		// CR.FIX.15183 - Use SUCCEEDED(hr) instead of !m_bInViolationPeriod so that when in violation period you have the 
		//chance to get out of the violation period.
		if (module->second>0 && SUCCEEDED(hr))
		{	
			tempHr = ObtainLicensesInternal(module->first, module->second);
			if (FAILED(tempHr) && SUCCEEDED(hr)) 
				hr = tempHr;
		}
		else if (module->second<0)
		{
			tempHr = ReleaseLicensesInternal(module->first, -1*module->second);
			if (FAILED(tempHr) && SUCCEEDED(hr)) 
				hr = tempHr;
		}
	}

	
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
		if(server->second.bUseSoftwareLicensing == false)
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
	}
	return S_OK;
}



//Software server
HRESULT CSolimarLicenseMgr::AssociateAppInstanceToSoftwareServer(ServerInfo* pServerInfo, _bstr_t appInstance, bool bLogError)
{
	HRESULT hr(S_OK);
	if(!m_bViewLicenseOnly && pServerInfo != NULL)
	{
		SS_SLSERVER_ON_INTERFACE_FTCALL_HR(ISolimarSoftwareLicenseSvr, (*pServerInfo), SoftwareAddApplicationInstanceByProduct, (m_product, appInstance), hr);
	}
	return hr;
}

HRESULT CSolimarLicenseMgr::FreeAllSoftwareLicenseFromCache(ServerInfo* pServerInfo, bool bLogError)
{
	HRESULT hr(S_OK);
	if(!m_bViewLicenseOnly && pServerInfo != NULL)
	{
		// SoftwareRemoveApplicationInstanceByProduct should release all modules obtained by the appInstance.
		SS_SLSERVER_ON_INTERFACE_FTCALL_HR(ISolimarSoftwareLicenseSvr, (*pServerInfo), SoftwareRemoveApplicationInstanceByProduct, (m_product, m_applicationInstance), hr);
	}
	return hr;
}


// attempts to allocate licenses on the known-good keys in the cache
HRESULT CSolimarLicenseMgr::ObtainLicensesInternal(long module_id, long license_count)
{
//OutputFormattedDebugString(L"CSolimarLicenseMgr::ObtainLicensesInternal - Enter - moduleID=%d, licenseCount=%d", module_id, license_count);
	HRESULT hr = LicenseServerError::EHR_LICENSE_INSUFFICIENT;
	
	ENSURE_INITIALIZED;

	if (m_bViewLicenseOnly) return LicenseServerError::EHR_LIC_MGR_VIEW_ONLY;
	
	if (license_count<=0) return E_INVALIDARG;
	
	SafeMutex mutex(ServerListLock);
	
	long licenses_to_obtain(license_count);
	long licenses_obtained(0);
	
	//xxx && !(single_key && allocated_licenses_on_a_key)
	//xxx how to ensure that the single key case is handled??
	//xxx need an indicator of which key is the current key (if any) and only use that key
	//xxx need a way to transfer licenses from the single key to a different single key if needed in case the first one fills up
	ServerList::iterator server = m_bUsingBackupServers ? m_backupServers.begin() : m_servers.begin();
	ServerList::iterator serverEnd = m_bUsingBackupServers ? m_backupServers.end() : m_servers.end();
	bool bOnlyOneServer = m_bUsingBackupServers ? (m_backupServers.size()) == 1 : (m_servers.size() == 1);
	
	try
	{
		// foreach server
		for (;server != serverEnd; ++server)
		{
			if(server->second.bUseOnlyLicenseViewer == false)
			{
				if(server->second.bUseSoftwareLicensing) // software licensing
				{
					long totalLicense(0);
					ModuleLicenseTotalInternal(module_id, &totalLicense);
					long software_licenses_available = totalLicense - server->second.software_license.licenses_inuse[module_id];
					//long software_licenses_available = server->second.software_license.licenses_total[module_id] - server->second.software_license.licenses_inuse[module_id];
					long software_licenses_to_obtain = bOnlyOneServer ? license_count : min(licenses_to_obtain - licenses_obtained, software_licenses_available);
		OutputFormattedDebugString(L"CSolimarLicenseMgr::ObtainLicensesInternal totalLicense=%d, software_licenses_available=%d, software_licenses_to_obtain=%d", totalLicense, software_licenses_available, software_licenses_to_obtain);
					if(software_licenses_available == 0 ||
						(bOnlyOneServer && software_licenses_available <software_licenses_to_obtain))
					{
						hr = LicenseServerError::EHR_LICENSE_INSUFFICIENT;

						Lic_PackageAttribs::Lic_SoftwareSpecAttribs::TMap_Lic_ProductSoftwareSpecAttribsMap::iterator prodSpecIt = m_softwareSpec.productSpecMap->find(m_product);
						std::wstring wstrModuleName = std::wstring(L"Unknown Module");
						if(prodSpecIt != m_softwareSpec.productSpecMap->end())
						{
							Lic_PackageAttribs::Lic_ProductSoftwareSpecAttribs::TMap_Lic_ModuleSoftwareSpecAttribsMap::iterator modSpecIt = prodSpecIt->second.moduleSpecMap->find(module_id);
							if(modSpecIt != prodSpecIt->second.moduleSpecMap->end())
								wstrModuleName = modSpecIt->second.moduleName;
						}
						wchar_t errorBuf[1024];
						swprintf_s(errorBuf, _countof(errorBuf), L"Module: %s (%d) - Licensed: %d, In Use: %d, Amount to Obtain: %d",
							wstrModuleName.c_str(),
							module_id,
							totalLicense,
							server->second.software_license.licenses_inuse[module_id],
							software_licenses_to_obtain
							);
						LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(hr, std::wstring(errorBuf), __uuidof(0), __uuidof(0));
					}
					else if(software_licenses_available >= software_licenses_to_obtain)
					{
						try
						{
							SS_SLSERVER_ON_INTERFACE_FTCALL_HR(ISolimarSoftwareLicenseSvr, server->second, SoftwareModuleLicenseObtainByApp_ByProduct, (m_product, module_id, software_licenses_to_obtain), hr);
							if(FAILED(hr)) 
								throw hr;

							licenses_obtained+=software_licenses_to_obtain; 
							server->second.software_license.licenses_allocated[module_id]+=software_licenses_to_obtain;
							server->second.software_license.licenses_inuse[module_id]+=software_licenses_to_obtain;
						}
						catch(HRESULT &ehr)
						{
							hr = ehr;
						}
					}
				}
				else // protection key licensing
				{
					KeySpec::Module &module(m_keyspec.products[m_productKeyID][module_id]);
					bool bModuleIsSharable = module.isSharable;
					if(((*server).second.bUseOnlySharedLicenses && bModuleIsSharable) || !(*server).second.bUseOnlySharedLicenses)
					{
						// foreach key
						for (ServerInfo::KeyList::iterator k = server->second.keys.begin(); k != server->second.keys.end(); ++k)
						{
							
							if (k->second.KeyPresent && k->second.KeyValid)
							{				
								long key_licenses_available = k->second.licenses_total[module_id] - k->second.licenses_inuse[module_id];
								long key_licenses_to_obtain = min(licenses_to_obtain - licenses_obtained, key_licenses_available);
								// if the key has has licenses available and hasn't been obtained yet, obtain it, except for shared licensing
								if (!k->second.KeyObtained && key_licenses_to_obtain>0 && !((*server).second.bUseOnlySharedLicenses))
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
											SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::ObtainLicensesInternal() - RPC Error", MT_INFO, LicenseServerError::EC_UNKNOWN, MessageGenericError);
											SS_GENERATE_AND_DISPATCH_MESSAGE(LicensingMessageStringTable[MessageClientTimeout], MT_INFO, LicenseServerError::EC_CLIENT_TIMEOUT, MessageClientTimeout);
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
										 long licenses_inuse_byapp(0);
										 hr = server->second.LicenseServer->KeyModuleLicenseInUse_ByApp(k->first, module_id, &licenses_inuse_byapp);
										 if(SUCCEEDED(hr))
											 k->second.licenses_inuse_byapp[module_id] = licenses_inuse_byapp;
										 }
								}
							}
						}
					}
					if(licenses_obtained == licenses_to_obtain)
						RefreshApplicationInstanceInUseCache(&(server->second), false);
				}
			}
		}
	}
	catch(_com_error& e)
	{
		hr = e.Error();
	}
	if((licenses_obtained != licenses_to_obtain) || (hr == E_FAIL))
		hr = LicenseServerError::EHR_LICENSE_INSUFFICIENT;
	LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(hr, LicenseServerError::GetErrorMessage(hr), __uuidof(0), __uuidof(0));
//OutputFormattedDebugString(L"CSolimarLicenseMgr::ObtainLicensesInternal - Leave - moduleID=%d, licenseCount=%d, licenses_obtained: %d, licenses_to_obtain: %d, hr: 0x%x", module_id, license_count, licenses_obtained, licenses_to_obtain, hr);
	//return (licenses_obtained == licenses_to_obtain ? S_OK : hr);
	return hr;
}



// attempts to deallocate licenses on keys that have 
HRESULT CSolimarLicenseMgr::ReleaseLicensesInternal(long module_id, long license_count)
{
	HRESULT hr = S_OK;
	
	ENSURE_INITIALIZED;
	if (m_bViewLicenseOnly) return LicenseServerError::EHR_LIC_MGR_VIEW_ONLY;
	if (license_count<=0) return E_INVALIDARG;
	
	SafeMutex mutex(ServerListLock);
	
	long licenses_to_release(license_count);
	long licenses_released(0);


	ServerList::reverse_iterator server = m_bUsingBackupServers ? m_backupServers.rbegin() : m_servers.rbegin();
	ServerList::reverse_iterator serverEnd = m_bUsingBackupServers ? m_backupServers.rend() : m_servers.rend();
	try
	{
		// for each server (backwards)
		for (;server != serverEnd; ++server)
		{
			if(server->second.bUseSoftwareLicensing) // software licensing
			{
				long software_licenses_allocated = server->second.software_license.licenses_allocated[module_id];
				long software_licenses_to_release = min(licenses_to_release-licenses_released,software_licenses_allocated);

				try
				{
					SS_SLSERVER_ON_INTERFACE_FTCALL_HR(ISolimarSoftwareLicenseSvr, server->second, SoftwareModuleLicenseReleaseByApp_ByProduct, (m_product, module_id, software_licenses_to_release), hr);
					if(FAILED(hr)) 
						throw hr;

					licenses_released+=software_licenses_to_release; 
					server->second.software_license.licenses_allocated[module_id]-=software_licenses_to_release;
					server->second.software_license.licenses_inuse[module_id]-=software_licenses_to_release;
				}
				catch(HRESULT &ehr)
				{
					hr = ehr;
				}
			}
			else
			{
				KeySpec::Module &module(m_keyspec.products[m_productKeyID][module_id]);
				bool bModuleIsSharable = module.isSharable;
				// if (server is set to use only sharable licenses and module is sharable ) or (server is set to use all licenses)
				if(((*server).second.bUseOnlySharedLicenses && bModuleIsSharable) || !(*server).second.bUseOnlySharedLicenses)
				{
					// foreach key (backwards)
					for (ServerInfo::KeyList::reverse_iterator k = server->second.keys.rbegin(); k != server->second.keys.rend(); ++k)
					{
						if (k->second.KeyPresent && (k->second.KeyObtained || (*server).second.bUseOnlySharedLicenses))
						{
							// if the key has already been obtained
							if (!k->second.KeyObtained && !((*server).second.bUseOnlySharedLicenses))
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
										SS_GENERATE_AND_DISPATCH_MESSAGE(L"CSolimarLicenseMgr::ReleaseLicensesInternal() - RPC Error", MT_INFO, LicenseServerError::EC_UNKNOWN, MessageGenericError);
										SS_GENERATE_AND_DISPATCH_MESSAGE(LicensingMessageStringTable[MessageClientTimeout], MT_INFO, LicenseServerError::EC_CLIENT_TIMEOUT, MessageClientTimeout);
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
								long licenses_inuse_byapp(0);
								hr = server->second.LicenseServer->KeyModuleLicenseInUse_ByApp(k->first, module_id, &licenses_inuse_byapp);
								if(SUCCEEDED(hr))
									k->second.licenses_inuse_byapp[module_id] = licenses_inuse_byapp;

								}
							
							// if the key no longer has any licenses obtained on it, try to release it and unlock it where necessary
							//xxx
						}
					}
				}
				if(licenses_released == licenses_to_release)
					RefreshApplicationInstanceInUseCache(&(server->second), false);
			}
		}
	}
	catch(_com_error& e)
	{
		hr = e.Error();
	}
//OutputFormattedDebugString(L"CSolimarLicenseMgr::ReleaseLicensesInternal(%d, %d) - licenses_released(%d) == licenses_to_release(%d)", module_id, license_count, licenses_released, licenses_to_release);
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

	SafeMutex mutex_messages(MessageListLock);
	licensing_message_cache.push_back(message);
	return S_OK;
}
bool CSolimarLicenseMgr::GracePeriodHadEnded()
{
	SafeMutex mutex(GracePeriodLock);
	return (GracePeriodHasStarted() && (m_dtGracePeriodStart+(m_dtGracePeriod*60) < (unsigned)time(NULL))) || m_dtGracePeriod==0;
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
		SS_GENERATE_AND_DISPATCH_MESSAGE(LicensingMessageStringTable[MessageGracePeriodStarted], MT_WARNING, LicenseServerError::EC_SP_NO_SERVER_RESPONSE, MessageGracePeriodStarted);
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
		SS_GENERATE_AND_DISPATCH_MESSAGE(LicensingMessageStringTable[MessageGracePeriodEnded], MT_WARNING, 0, MessageGracePeriodEnded);
	}
}





HRESULT CSolimarLicenseMgr::RefreshLicensingFromLicServers(bool _bLogError)
{
//wchar_t debug_buf[1024];
//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::RefreshLicensingFromLicServers() - Enter");
//OutputDebugStringW(debug_buf);
	ENSURE_INITIALIZED;
	
	SafeMutex mutex(ServerListLock);
	HRESULT hr(S_OK);
	HRESULT tempHr(S_OK);
		
	ServerList::iterator serverIt = m_bUsingBackupServers ? m_backupServers.begin() : m_servers.begin();
	ServerList::iterator serverEndIt = m_bUsingBackupServers ? m_backupServers.end() : m_servers.end();

	// foreach server
	if(serverIt != serverEndIt)
	{
		bool bContainsSoftwareLic(false);
		hr = LicenseServerContainsSoftwareLicensingForProduct(m_product, &(serverIt->second), &bContainsSoftwareLic);
//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::RefreshLicensingFromLicServers() - LicenseServerContainsSoftwareLicensingForProduct() - hr: 0x%08x", hr);
//OutputDebugStringW(debug_buf);
		if(SUCCEEDED(hr))
		{
			if(bContainsSoftwareLic)
			{
				hr = RefreshSoftwareLicenseFromLicServers(_bLogError);
//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::RefreshLicensingFromLicServers() - RefreshSoftwareLicenseFromLicServers() - hr: 0x%08x", hr);
//OutputDebugStringW(debug_buf);

				if(SUCCEEDED(hr))
					(serverIt->second).bUseSoftwareLicensing = bContainsSoftwareLic;
			}
		}

		//Failed to use software licensing, just use protection key
		if(!bContainsSoftwareLic || FAILED(hr))	
		{
			(serverIt->second).bUseSoftwareLicensing = false;
			SAVE_ERRORINFO
			tempHr = RefreshKeyListFromLicServers(_bLogError && !bContainsSoftwareLic);
			RESTORE_ERRORINFO
			if(SUCCEEDED(tempHr))	//Don't replace hr if it is FAILED
				hr = tempHr;
//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::RefreshLicensingFromLicServers() - RefreshKeyListFromLicServers() - hr: 0x%08x", hr);
//OutputDebugStringW(debug_buf);
		}

		//Failed both software licensing and protection key
		if(FAILED(hr))
		{
			//look to see if there is a software license error and report that
			SAVE_ERRORINFO
			tempHr = LicenseServerSoftwareLicenseStatusFirstErrorForProduct(m_product, &(serverIt->second));
			RESTORE_ERRORINFO
			if(FAILED(tempHr))
			{
				//Replace error info with latest error
				LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(tempHr, LicenseServerError::GenerateErrorMessage(tempHr), __uuidof(CSolimarLicenseMgr), __uuidof(ISolimarLicenseMgr7));
				LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(hr, LicenseServerError::GenerateErrorMessage(hr), __uuidof(0), __uuidof(0));

				hr = tempHr;
			}
//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::RefreshLicensingFromLicServers() - LicenseServerSoftwareLicenseStatusFirstErrorForProduct() - hr: 0x%08x", hr);
//OutputDebugStringW(debug_buf);
		}

	}
//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::RefreshLicensingFromLicServers() - Leave 0 - hr: 0x%08x", hr);
//OutputDebugStringW(debug_buf);
	return hr;
}


HRESULT CSolimarLicenseMgr::LicenseServerContainsProtectionKeyLicensingForProduct(long product_id, ServerInfo* pServerInfo, bool* pbHasLicensing)
{
	ENSURE_INITIALIZED;
	
	SafeMutex mutex(ServerListLock);
	HRESULT hr(S_OK);
	*pbHasLicensing = true;
	return hr;
}
HRESULT CSolimarLicenseMgr::LicenseServerContainsSoftwareLicensingForProduct(long product_id, ServerInfo* pServerInfo, bool* pbHasLicensing)
{
//wchar_t debug_buf[1024];
//_snwprintf(debug_buf, 1024, L"CSolimarLicenseMgr::LicenseServerContainsSoftwareLicensingForProduct() - Enter");
//OutputDebugStringW(debug_buf);
	ENSURE_INITIALIZED;

	SafeMutex mutex(ServerListLock);
	HRESULT hr(S_OK);
	try
	{
		*pbHasLicensing = false;
		BSTR bstrLicenseStream;
		SS_SLSERVER_ON_INTERFACE_FTCALL_HR(ISolimarSoftwareLicenseSvr, (*pServerInfo), GetSoftwareLicenseInfoByProduct_ForAll, (product_id, &bstrLicenseStream), hr);

//_snwprintf(debug_buf, _countof(debug_buf), L"CSolimarLicenseMgr::LicenseServerContainsSoftwareLicensingForProduct - GetSoftwareLicenseInfoByProduct_ForAll() - hr: 0x%08x", hr);
//OutputDebugStringW(debug_buf);
		if(FAILED(hr))
			throw hr;

		// might need to check for NULL or empty

		Lic_PackageAttribs::Lic_ProductInfoAttribs prodAttribs;
		prodAttribs.InitFromString(bstrLicenseStream);
		SysFreeString(bstrLicenseStream);


		// if licensing is there, products must match.
		*pbHasLicensing = (int)prodAttribs.productID == product_id ;
//_snwprintf(debug_buf, _countof(debug_buf), L"CSolimarLicenseMgr::LicenseServerContainsSoftwareLicensingForProduct() - prodAttribs.productID(%d) == product_id(%d) ", prodAttribs.productID, product_id);
//OutputDebugStringW(debug_buf);

		if(*pbHasLicensing == false)
			hr = LicenseServerError::EHR_LIC_SOFTWARE_NO_PRODUCT;
	}
	catch(HRESULT &ehr)
	{
		hr = ehr;
	}

//_snwprintf(debug_buf, _countof(debug_buf), L"CSolimarLicenseMgr::LicenseServerContainsSoftwareLicensingForProduct() - Leave 0 - hr: 0x%08x", hr);
//OutputDebugStringW(debug_buf);
	return hr;
}

HRESULT CSolimarLicenseMgr::LicenseServerSoftwareLicenseStatusFirstErrorForProduct(long product_id, ServerInfo* pServerInfo)
{
	ENSURE_INITIALIZED;

	SafeMutex mutex(ServerListLock);
	HRESULT hr(S_OK);
	try
	{
		BSTR bstrLicenseStream;
		SS_SLSERVER_ON_INTERFACE_FTCALL_HR(ISolimarSoftwareLicenseSvr, (*pServerInfo), GetSoftwareLicenseStatus_ByProduct, (m_product, &bstrLicenseStream), hr);
		if(FAILED(hr))
			throw hr;

		SpdAttribs::MapStringDwordObj strDwordMapObj;
		SpdAttribs::TMapStringDword strDwordMap;
		strDwordMapObj.StringToValue(bstrLicenseStream, strDwordMap);
		SysFreeString(bstrLicenseStream);
		for(	SpdAttribs::TMapStringDword::iterator strDwordMapIt = strDwordMap.begin();
				strDwordMapIt != strDwordMap.end();
				strDwordMapIt++)
		{
			if(FAILED(strDwordMapIt->second))
			{
				hr = strDwordMapIt->second;
				break;
			}
		}
		// might need to check for NULL or empty
		//Lic_PackageAttribs::Lic_ProductInfoAttribs prodAttribs;
		//prodAttribs.InitFromString(bstrLicenseStream);
		//SysFreeString(bstrLicenseStream);


		// if licensing is there, products must match.
		//*pbHasLicensing = (int)prodAttribs.productID == product_id ;
	}
	catch(HRESULT &eHr)
	{
		hr = eHr;
	}
	catch(_com_error &e)
	{
		hr = e.Error();
	}
	catch(...)
	{
		hr = E_FAIL;
	}
	return hr;
}

//HRESULT CSolimarLicenseMgr::RefreshLicensingSchemeFromLicenseServer(ServerInfo* pServerInfo, VARIANT *pvtKeyList, long count, bool bLogError)
//{
//	//Look for Software License Key
//	for (int i = 0; i<count; ++i)
//	{
//		_bstr_t key_ident = pvtKeyList[i].bstrVal;
//		VARIANT vtKeyProductID, vtKeyProductVersion;
//
//		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product Version"].id, &vtKeyProductVersion);
//		if (FAILED(hr)) {hr = S_OK; continue;}
//
//		// check that the key is present and valid
//		VARIANT_BOOL key_license_valid(VARIANT_FALSE);
//      
//		// check that the key has the requisite product version and etc.
//		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product Version"].id, &vtKeyProductVersion);
//		if (FAILED(hr)) {hr = S_OK; continue;}
//		hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Product ID"].id, &vtKeyProductID);
//		if (FAILED(hr)) {hr = S_OK; continue;}
//
//		// if the product id and product version requirements are satisfied
//		if (Version::TinyVersion(vtKeyProductVersion.uiVal,0) >= Version::TinyVersion(Version::ModuleVersion(m_prod_ver_major, m_prod_ver_minor, 0, 0)) && 
//			m_keyspec.products[m_productKeyID].id==vtKeyProductID.uiVal)
//
//
//	}
//}


// cycles through all servers in server list seeing if they have a newer
//software spec than the currently loaded one.  Should call only once probably...
//Also, only retrieves the Lic_ProductSoftwareSpecAttribs for the product the
//LicenseManager is initialized for...
HRESULT CSolimarLicenseMgr::RefreshSoftwareSpec(Lic_PackageAttribs::Lic_SoftwareSpecAttribs* pSoftwareSpec)
{
	
	HRESULT hr = S_OK;

	//Use your own softwarespec...
	return E_NOTIMPL;

	ENSURE_INITIALIZED;
	
	SafeMutex mutex(ServerListLock);	
	
	ServerList::iterator server = m_bUsingBackupServers ? m_backupServers.begin() : m_servers.begin();
	ServerList::iterator serverEnd = m_bUsingBackupServers ? m_backupServers.end() : m_servers.end();

	// foreach server
	for (;server != serverEnd; ++server)
	{
		//Get version of keyspec, compare it with other versions from m_servers to pSoftwareSpec

		//Server has a newer version, get its Software Spec...

		//software_license
		BSTR bstrSpecStream;
		SS_SLSERVER_ON_INTERFACE_FTCALL_HR(ISolimarSoftwareLicenseSvr, server->second, GetSoftwareSpecByProduct, (m_product, &bstrSpecStream), hr);
		if(SUCCEEDED(hr))
		{
			Lic_PackageAttribs::Lic_ProductSoftwareSpecAttribs tmpProdSpec;
			tmpProdSpec.InitFromString(bstrSpecStream);
			if(SUCCEEDED(hr))
			{
				pSoftwareSpec->productSpecMap->insert(Lic_PackageAttribs::Lic_SoftwareSpecAttribs::TMap_Lic_ProductSoftwareSpecAttribsMap::value_type(m_product, tmpProdSpec));
			}
			SysFreeString(bstrSpecStream);
		}
	}



	return hr;
}
