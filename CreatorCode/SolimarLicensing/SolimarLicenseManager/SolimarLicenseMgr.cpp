// SolimarLicenseMgr.cpp : Implementation of CSolimarLicenseMgr

#include "stdafx.h"
#include "SolimarLicenseMgr.h"
#include "..\common\ChallengeResponse.h"
#include "..\common\Version.h"
#include "..\common\SafeMutex.h"
#include "..\common\EventLogHelper.h"
#include "..\common\LicensingMessage.h"
#include "..\SolimarLicenseServer\KeyError.h"
#include "..\SolimarLicenseServer\KeyMessages.h"
#include "resource.h"
#include <string>
#include <time.h>
#include <comdef.h>

#define ENSURE_INITIALIZED if (!m_initialized) return LicenseServerError::EC_MANAGER_NOT_INITIALIZED

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



CSolimarLicenseMgr::ServerInfo::ServerInfo()
{
	;
}

CSolimarLicenseMgr::ServerInfo::ServerInfo(const ServerInfo &s) : 
	LicenseServer(s.LicenseServer)
{
	;
}

CSolimarLicenseMgr::ServerInfo::ServerInfo(_bstr_t servername, ISolimarLicenseSvrPtr pILicenseServer) : 
	name(servername), 
	LicenseServer(pILicenseServer)
{
	;
}

CSolimarLicenseMgr::ServerInfo::~ServerInfo()
{
	;
}

CSolimarLicenseMgr::CSolimarLicenseMgr() : 
	m_initialized(false), 
	ServerListLock(CreateMutex(0,0,0)),
	//m_license_message_callback(0),
	//m_license_message_callback_context(0),
	m_ui_level(UI_IGNORE),
	ChallengeResponseHelper(challenge_key_manager_userauththis_private, sizeof(challenge_key_manager_userauththis_private), challenge_key_manager_thisauthuser_public, sizeof(challenge_key_manager_thisauthuser_public))
{
	HeartbeatThread = new APCTimer(HeartbeatThreadFunction, this, 3*1000);
}

CSolimarLicenseMgr::~CSolimarLicenseMgr()
{
	if (HeartbeatThread)
		delete HeartbeatThread;
	if (ServerListLock!=INVALID_HANDLE_VALUE)
		CloseHandle(ServerListLock);
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
	
	// Try to create an ISolimarLicenseServer proxy to the server
	//ISolimarLicenseServer *pILicenseServer;
	ISolimarLicenseSvr *pILicenseServer = 0;
	COSERVERINFO	serverInfo	= {0, server, NULL, 0};
	MULTI_QI		multiQI		= {&__uuidof(ISolimarLicenseSvr), NULL, NOERROR};
	
	hr = CoCreateInstanceEx(
		__uuidof(CSolimarLicenseSvr), 
		NULL, 
		CLSCTX_REMOTE_SERVER, 
		&serverInfo, 
		1, 
		&multiQI);
	if (FAILED(hr))
		return hr;

	ISolimarLicenseSvrPtr pServer;
	pServer.Attach((ISolimarLicenseSvr*)multiQI.pItf);
	
	ChallengeResponseHelper CR(challenge_key_server_thisauthuser_private, sizeof(challenge_key_server_thisauthuser_private), challenge_key_server_userauththis_public, sizeof(challenge_key_server_userauththis_public));
	// try to authenticate the license server
	hr = CR.AuthenticateServer(pServer);
	if (FAILED(hr)) return hr;
	
	// let the license server authenticate this manager
	hr = CR.AuthenticateToServer(pServer);
	if (FAILED(hr)) return hr;
	
	{
		SafeMutex mutex(ServerListLock);
		m_servers[server]=ServerInfo(server,pServer);
	}

	m_initialized = false;
	
	return S_OK;
}

STDMETHODIMP CSolimarLicenseMgr::Disconnect()
{
	SafeMutex mutex(ServerListLock);

	while(!m_servers.empty())
	{
		m_servers.erase(m_servers.begin());
	}
	
	m_ui_level = UI_IGNORE;
	m_initialized = false;
	
	return S_OK;
}

STDMETHODIMP CSolimarLicenseMgr::Initialize(long product, long prod_ver_major, long prod_ver_minor, VARIANT_BOOL single_key, BSTR specific_single_key_ident, VARIANT_BOOL lock_keys, long auto_ui_level)
{
	HRESULT hr = S_OK;
	
	//xxxyyy
	// make sure that the client has authenticated already
	if (!ChallengePassedByClient())
		return HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED);
	
	m_ui_level = auto_ui_level;
	//m_license_message_callback = 0;
	//m_license_message_callback_context = 0;
	m_specific_single_key_ident = _bstr_t(specific_single_key_ident, true);
	m_single_key = (single_key==VARIANT_TRUE);
	m_lock_keys = (lock_keys==VARIANT_TRUE);
	m_initialized = true;
	
	bool found_keys=false;
	
	SafeMutex mutex(ServerListLock);
	
	// for each server (breaks out if single_key is specified and a key has been found)
	for (ServerList::iterator server = m_servers.begin(); server != m_servers.end() && !(single_key && found_keys); ++server)
	{
		// get the key list form the server
		VARIANT vtKeyList;
		hr = server->second.LicenseServer->KeyEnumerate(&vtKeyList);
		if (SUCCEEDED(hr) && (vtKeyList.vt & (VT_ARRAY | VT_VARIANT)))
		{
			VARIANT *pvtKeyName;
			if (SUCCEEDED(SafeArrayAccessData(vtKeyList.parray, (void**)&pvtKeyName)))
			{
				// for each key on the server
				for (unsigned int i = 0; i<vtKeyList.parray->rgsabound[0].cElements && !(single_key && found_keys); ++i)
				{
					// check that the key has the requisite product version and etc.
					_bstr_t key_name = pvtKeyName[i].bstrVal;
					VARIANT vtKeyProductID, vtKeyProductVersion;
					hr = server->second.LicenseServer->KeyHeaderQuery(key_name, m_keyspec.headers[L"Product Version"].id, &vtKeyProductVersion);
					if (FAILED(hr)) {hr = S_OK; continue;}
					hr = server->second.LicenseServer->KeyHeaderQuery(key_name, m_keyspec.headers[L"Product ID"].id, &vtKeyProductID);
					if (FAILED(hr)) {hr = S_OK; continue;}
					
					// if the key product version version is greater than or equal to the product version requested
					if (Version::TinyVersion(vtKeyProductVersion.uiVal,0) >= Version::TinyVersion(Version::ModuleVersion(prod_ver_major, prod_ver_minor, 0, 0)))
					{
						// if the product matches the product on the key
						if (m_keyspec.products[product].id==vtKeyProductID.uiVal)
						{
							// if a specific key is requested, but this one is not it, skip this key
							if (!(m_single_key && m_specific_single_key_ident.length()>0 && m_specific_single_key_ident!=key_name))
							{
								found_keys=true;
								server->second.keys.push_back(key_name);
								
								hr = server->second.LicenseServer->KeyObtain(key_name);
								//hr = server->second.LicenseServer->MessageCallbackEnrollment((ILicensingMessage*)this);
								
								if (m_lock_keys)
									hr = server->second.LicenseServer->KeyLock(key_name);
								if (m_single_key)
									break;
							}
						}
					}
				}
				SafeArrayUnaccessData(vtKeyList.parray);
			}
		}
	}
	
	// S_OK if some keys were found
	// S_FALSE if no keys are found
	// Error HRESULT if there was an error
	return (SUCCEEDED(hr) ? (found_keys ? S_OK : S_FALSE) : hr);
}

STDMETHODIMP CSolimarLicenseMgr::ValidateLicense(VARIANT_BOOL *license_valid)
{
	HRESULT hr = S_OK;

	ENSURE_INITIALIZED;

	SafeMutex mutex(ServerListLock);
	
	*license_valid = VARIANT_TRUE;
	
	// foreach server
	for (ServerList::iterator server = m_servers.begin(); server != m_servers.end(); ++server)
	{
		// foreach key
		for (ServerInfo::KeyList::iterator key = server->second.keys.begin(); key != server->second.keys.end(); ++key)
		{
			// ensure that the licensing on this key is still valid
			VARIANT_BOOL this_key_license_valid = VARIANT_FALSE;
			hr = server->second.LicenseServer->KeyValidateLicense(*key, &this_key_license_valid);
			if (FAILED(hr))
			{
				*license_valid = VARIANT_FALSE;
				return hr;
			}
			if (this_key_license_valid==VARIANT_FALSE)
			{
				*license_valid = VARIANT_FALSE;
				break;
			}
		}
	}
	
	return S_OK;
}

// count the total licenses in all keys
STDMETHODIMP CSolimarLicenseMgr::ModuleLicenseTotal(long module_id, long *count)
{
	HRESULT hr = S_OK;
	
	*count = 0;
	
	ENSURE_INITIALIZED;
	SafeMutex mutex(ServerListLock);
	
	// foreach server
	for (ServerList::iterator server = m_servers.begin(); server != m_servers.end(); ++server)
	{
		// foreach key
		for (ServerInfo::KeyList::iterator key = server->second.keys.begin(); key != server->second.keys.end(); ++key)
		{
			long c;
			hr = server->second.LicenseServer->KeyModuleLicenseTotal(*key, module_id, &c);
			if (FAILED(hr)) return hr;
			// if one of the licenses is set to 'unlimited', retain this fact
			if (*count==0x7FFFFFFF || c==0x7FFFFFFF)
				*count = 0x7FFFFFFF;
			else
				*count += c;
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

	// foreach server
	for (ServerList::iterator server = m_servers.begin(); server != m_servers.end(); ++server)
	{
		// foreach key
		for (ServerInfo::KeyList::iterator key = server->second.keys.begin(); key != server->second.keys.end(); ++key)
		{
			long c;
			hr = server->second.LicenseServer->KeyModuleLicenseInUse(*key, module_id, &c);
			if (FAILED(hr)) return hr;
			*count += c;
		}
	}
	
	return S_OK;
}

STDMETHODIMP CSolimarLicenseMgr::ModuleLicenseObtain(long module_id, long count)
{
	HRESULT hr = S_OK;
	
	ENSURE_INITIALIZED;
	
	if (count<0)
		return E_INVALIDARG;
	
	long licenses_obtained = 0;
	
	SafeMutex mutex(ServerListLock);

	// foreach server
	for (ServerList::iterator server = m_servers.begin(); server != m_servers.end(); ++server)
	{
		// foreach key
		for (ServerInfo::KeyList::iterator key = server->second.keys.begin(); key != server->second.keys.end(); ++key)
		{
			// query the key for remaining number of licenses
			long key_licenses_total=0, key_licenses_inuse=0, key_licenses_available=0, key_licenses_to_obtain=0;
			hr = server->second.LicenseServer->KeyModuleLicenseTotal(*key, module_id, &key_licenses_total);
			if (FAILED(hr)) continue;
			hr = server->second.LicenseServer->KeyModuleLicenseInUse(*key, module_id, &key_licenses_inuse);
			if (FAILED(hr)) continue;
			
			key_licenses_available = key_licenses_total-key_licenses_inuse;
			key_licenses_to_obtain = min(key_licenses_available,count-licenses_obtained);
			hr = server->second.LicenseServer->KeyModuleLicenseObtain(*key, module_id, key_licenses_to_obtain);

			if (SUCCEEDED(hr))
				licenses_obtained+=key_licenses_to_obtain;
			if (licenses_obtained == count)
				break;
		}
		if (licenses_obtained == count)
			break;
	}
	
	if (licenses_obtained<count)
		return E_INVALIDARG;
	else
        return S_OK;
}

STDMETHODIMP CSolimarLicenseMgr::ModuleLicenseRelease(long module_id, long count)
{
	HRESULT hr = S_OK;
	
	ENSURE_INITIALIZED;
	
	if (count<0)
		return E_INVALIDARG;
	
	long licenses_released = 0;
	
	SafeMutex mutex(ServerListLock);

	// foreach server
	for (ServerList::iterator server = m_servers.begin(); server != m_servers.end(); ++server)
	{
		// foreach key
		for (ServerInfo::KeyList::iterator key = server->second.keys.begin(); key != server->second.keys.end(); ++key)
		{
			// query the key for remaining number of licenses
			long key_licenses_total=0, key_licenses_inuse=0, key_licenses_available=0, key_licenses_to_release=0;
			hr = server->second.LicenseServer->KeyModuleLicenseTotal(*key, module_id, &key_licenses_total);
			if (FAILED(hr)) continue;
			hr = server->second.LicenseServer->KeyModuleLicenseInUse(*key, module_id, &key_licenses_inuse);
			if (FAILED(hr)) continue;
			
			key_licenses_available = key_licenses_total-key_licenses_inuse;
			key_licenses_to_release = min(key_licenses_inuse,count-licenses_released);
			hr = server->second.LicenseServer->KeyModuleLicenseRelease(*key, module_id, key_licenses_to_release);

			if (SUCCEEDED(hr))
				licenses_released+=key_licenses_to_release;
			if (licenses_released == count)
				break;
		}
		if (licenses_released == count)
			break;
	}
	
	if (licenses_released<count)
		return E_INVALIDARG;
	else
        return S_OK;
}

/*
STDMETHODIMP CSolimarLicenseMgr::RegisterMessageCallback(InProcPtr context, InProcPtr callback)
{
	m_license_message_callback_context = AccessInProcPtr(void*,context);
	m_license_message_callback = AccessInProcPtr(LicenseMessageCallbackPtr,callback);
	return S_OK;
}

STDMETHODIMP CSolimarLicenseMgr::DispatchLicenseMessage(BSTR key_ident, long message_type, long error, VARIANT vtTimestamp, BSTR message)
{
	// call the user defined callback
	if (m_license_message_callback)
	{
		m_license_message_callback(m_license_message_callback_context, key_ident, (unsigned int)message_type, (HRESULT)error, vtTimestamp, message);
	}
	
	// perform the auto ui requested (dialogs, event log, etc.)
	switch(m_ui_level)
	{
		//xxx modify the if statements to filter out messages that don't need to be recorded in each case
		// show all messages, warnings and errors
		case UI_ALL_DIALOGS:
			if (true)
			{
				KeyMessageShowDialog(key_ident, message_type, error, vtTimestamp, message);
			}
			break;
		// show only the errors
		case UI_CRITICAL_DIALOGS:
			if (error && true)
			{
				KeyMessageShowDialog(key_ident, message_type, error, vtTimestamp, message);
			}
			break;
		// log all messages, warnings and errors
		case UI_EVENT_LOG:
			if (true)
			{
				KeyMessageWriteEventLog(key_ident, message_type, error, vtTimestamp, message);
			}
			break;
		// take no action at all
		case UI_IGNORE:
			break;
	}
	
	return S_OK;
}
*/

/*
lass LicensingMessage
{
public:
	LicensingMessage();
	LicensingMessage(std::wstring &key, DWORD msg_type, HRESULT hr, VARIANT msg_time, std::wstring &msg);
	LicensingMessage(const LicensingMessage &m);
	LicensingMessage(const VARIANT &vtMessage);
	
	operator VARIANT();
	operator std::wstring();
	
	std::wstring key_ident;
	DWORD message_type;
	HRESULT error;
	VARIANT timestamp;
	std::wstring message;
*/

bool CSolimarLicenseMgr::ManagesKey(_bstr_t key_ident)
{
	if (key_ident.length()==0) return true;

	// foreach server
	for (ServerList::iterator server = m_servers.begin(); server != m_servers.end(); ++server)
	{
		// foreach key
		for (ServerInfo::KeyList::iterator key = server->second.keys.begin(); key != server->second.keys.end(); ++key)
		{
			if ((*key)==key_ident) return true;
		}
	}
	
	return false;
}

STDMETHODIMP CSolimarLicenseMgr::GetLicenseMessageList(VARIANT *pvtMessageList)
{
	HRESULT hr = S_OK;
	
	ENSURE_INITIALIZED;
	
	LicensingMessageList aggregate_messages;

	SafeMutex mutex(ServerListLock);

	// foreach server
	for (ServerList::iterator server = m_servers.begin(); server != m_servers.end(); ++server)
	{
		ILicensingMessage *pServerLicensingMessage = 0;
		_variant_t vtServerMessageList;
		if (SUCCEEDED(server->second.LicenseServer->QueryInterface(__uuidof(ILicensingMessage), (void**)&pServerLicensingMessage)))
		{
			pServerLicensingMessage->GetLicenseMessageList(&vtServerMessageList);
			LicensingMessageList server_messages(vtServerMessageList);
			aggregate_messages.insert(aggregate_messages.end(), server_messages.begin(), server_messages.end());
		}
	}
	
	// filter the message list, removing messages that are not relevant to the manager (by key identifier)
	for (LicensingMessageList::iterator m = aggregate_messages.begin(); m != aggregate_messages.end();)
	{
		if (!ManagesKey(m->key_ident.c_str()))
			aggregate_messages.erase(m);
		else
			++m;
	}
	
	for (LicensingMessageList::iterator m = aggregate_messages.begin(); m != aggregate_messages.end(); ++m)
	{
		DispatchLicenseMessage(_bstr_t(m->key_ident.c_str()), m->message_type, m->error, m->timestamp, _bstr_t(m->message.c_str()));
	}
	
	*pvtMessageList = aggregate_messages;
	
	return S_OK;
}

void CSolimarLicenseMgr::SendHeartbeat()
{
	HRESULT hr = S_OK;

	SafeMutex mutex(ServerListLock);

	for (ServerList::iterator server = m_servers.begin(); server != m_servers.end(); ++server)
	{
		hr = server->second.LicenseServer->Heartbeat();
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
	static const MAX_MESSAGE_SIZE = 1024;
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
	
	EventLogHelper::WriteEventLog(L"Solimar License Server", message, message_type);
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
