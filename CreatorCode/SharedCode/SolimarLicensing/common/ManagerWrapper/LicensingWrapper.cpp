#ifndef _WIN32_DCOM
#define _WIN32_DCOM // For CoInitializeEx
#endif

#include "LicensingWrapper.h"

//#include "..\common\InProcPtr.h"																	// CR.32662; commented.
#include "..\ChallengeResponseHelper.h"
#include "..\IObjectAuthentication.h"																// CR.32662; modified.
#include "..\SafeMutex.h"
#include "..\TimeHelper.h"																				// CR.32662.V06; added.

#include <assert.h>		// For ASSERT
#include <string>

BYTE SolimarLicenseManagerWrapper::LicensingWrapper::challenge_key_manager_userauththis_public[] = {
#include "..\keys\SolimarLicenseManager.UserAuthThis.public.key.txt"
};

BYTE SolimarLicenseManagerWrapper::LicensingWrapper::challenge_key_manager_thisauthuser_private[] = {
#include "..\keys\SolimarLicenseManager.ThisAuthUser.private.key.txt"
};


KeySpec SolimarLicenseManagerWrapper::LicensingWrapper::keyspec;

SolimarLicenseManagerWrapper::LicensingWrapper::LicensingWrapper() :
	ChallengeResponseHelper(challenge_key_manager_thisauthuser_private, sizeof(challenge_key_manager_thisauthuser_private) / sizeof(BYTE), challenge_key_manager_userauththis_public, sizeof(challenge_key_manager_userauththis_public) / sizeof(BYTE)),
	m_license_message_callback_context(0),
	m_license_message_callback(0),
	m_license_invalid_callback_context(0),
	m_license_invalid_callback(0),
	m_MessageDispatchThread(0),
	m_LicenseValidityThread(0),
	m_validityCheck_LastHr(S_OK),
	m_constructorHR(S_OK),
	m_MemberLock(CreateMutex(0, FALSE, 0)),
	m_ThreadKillEvent(CreateEvent(0, TRUE, FALSE, 0))
{
	Initialize(L"", L"", L"", 0, 0);
}

SolimarLicenseManagerWrapper::LicensingWrapper::LicensingWrapper(
	const std::wstring& domain, 																		// CR.32662.V06; modified.
	const std::wstring& username,																		// CR.32662.V06; modified.
	const std::wstring& password,																		// CR.32662.V06; modified.
	long authenticationLevel,
	long impersonationLevel) :
	ChallengeResponseHelper(
		challenge_key_manager_thisauthuser_private, sizeof(challenge_key_manager_thisauthuser_private)/sizeof(BYTE), 
		challenge_key_manager_userauththis_public, sizeof(challenge_key_manager_userauththis_public)/sizeof(BYTE)),
	m_license_message_callback_context(0),
	m_license_message_callback(0),
	m_license_invalid_callback_context(0),
	m_license_invalid_callback(0),
	m_MessageDispatchThread(0),
	m_LicenseValidityThread(0),
	m_validityCheck_LastHr(S_OK),
	m_constructorHR(S_OK),
	m_MemberLock(CreateMutex(0, FALSE, 0)),
	m_ThreadKillEvent(CreateEvent(0, TRUE, FALSE, 0))
{
	Initialize(
		domain,
		username,
		password,
		authenticationLevel,
		impersonationLevel);
}

/*
 * SLB 31.jan.2024 CR.32662; Minor restructuring of initialization/error messaging 
 * for the LicenseManager interfaces.
------------------------------------------------------------------------------------*/
void SolimarLicenseManagerWrapper::LicensingWrapper::Initialize(
	const std::wstring &domain,
	const std::wstring &username,
	const std::wstring &password,
	long authenticationLevel,
	long impersonationLevel)
{
	//ISolimarLicenseMgr8* pLocalLicenseMgr = NULL;
	//HRESULT hr = CoCreateInstance(__uuidof(CSolimarLicenseMgr), NULL, CLSCTX_INPROC_SERVER, __uuidof(ISolimarLicenseMgr8), (void**)&pLocalLicenseMgr);
	m_constructorHR = m_licenseManagerPtr.CreateInstance(CLSID_CSolimarLicenseMgr, NULL, CLSCTX_INPROC_SERVER);
	if (FAILED(m_constructorHR)) { LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::Initialize() Problem creating CSolimarLicenseMgr instance.", m_constructorHR); return; }
	
	if (username.length() > 0)
	{
		m_constructorHR = m_licenseManagerPtr->InitializeAuthInfo(
			CComBSTR(domain.c_str()),
			CComBSTR(username.c_str()),
			CComBSTR(password.c_str()),
			authenticationLevel, impersonationLevel);
		// CR.32662 note; If error occurred, only log it... Don't exit (to preserve 
		// previous functionality)
		if (FAILED(m_constructorHR)) { LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::Initialize() Problem returned by call to InitializeAuthInfo().", m_constructorHR); }
	}

	//ILicensingMessage* pLocalLicenseMessage = NULL;
	//hr = m_licenseManagerPtr->QueryInterface(__uuidof(ILicensingMessage), (void**)&pLocalLicenseMessage);
	m_constructorHR = m_licenseManagerPtr.QueryInterface(__uuidof(ILicensingMessage), m_licenseMessagePtr);
	if (FAILED(m_constructorHR)) { LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::Initialize() Problem initializing ILicensingMessage interface.", m_constructorHR); return; }
	
	m_MessageDispatchThread = CreateThread(0, 0, MessageDispatchThreadProc, this, CREATE_SUSPENDED, 0);
	m_LicenseValidityThread = CreateThread(0, 0, LicenseValidityThreadProc, this, CREATE_SUSPENDED, 0);
}

SolimarLicenseManagerWrapper::LicensingWrapper::LicensingWrapper(const LicensingWrapper &o) : 
	ChallengeResponseHelper(challenge_key_manager_thisauthuser_private, sizeof(challenge_key_manager_thisauthuser_private)/sizeof(BYTE), challenge_key_manager_userauththis_public, sizeof(challenge_key_manager_userauththis_public)/sizeof(BYTE)),
	m_license_message_callback_context(0),
	m_license_message_callback(0),
	m_license_invalid_callback_context(0),
	m_license_invalid_callback(0),
	m_MessageDispatchThread(0),
	m_LicenseValidityThread(0),
	m_MemberLock(CreateMutex(0, FALSE, 0)),
	m_validityCheck_LastHr(S_OK),
	m_constructorHR(S_OK),
	m_ThreadKillEvent(CreateEvent(0, TRUE, FALSE, 0))
{
	*this = o;
	
	m_MessageDispatchThread = CreateThread(0, 0, MessageDispatchThreadProc, this, 0, 0);
	m_LicenseValidityThread = CreateThread(0, 0, LicenseValidityThreadProc, this, 0, 0);
}

SolimarLicenseManagerWrapper::LicensingWrapper& SolimarLicenseManagerWrapper::LicensingWrapper::operator=(const LicensingWrapper &o)
{
	SafeMutex mutex(m_MemberLock);

	m_license_message_callback_context = o.m_license_message_callback_context;
	m_license_message_callback = o.m_license_message_callback;	
	m_license_invalid_callback_context = o.m_license_invalid_callback_context;
	m_license_invalid_callback = o.m_license_invalid_callback;
	m_licenseManagerPtr = o.m_licenseManagerPtr;
	return *this;
}

SolimarLicenseManagerWrapper::LicensingWrapper::~LicensingWrapper()
{
	// signal the threads to terminate

	SetEvent(m_ThreadKillEvent);

	try
	{
		// wait for the threads to terminate
		HANDLE threads[2] = {m_MessageDispatchThread, m_LicenseValidityThread};
		int retVal = WaitForMultipleObjects(sizeof(threads)/sizeof(HANDLE), threads, TRUE, 2000);
	}
	catch (...)
	{
	}
   
	{
	SafeMutex mutex(m_MemberLock);
	while (!m_session_obtained_map.empty())
	{
		//Side Effect: Erases the session_id in the m_session_obtained_map if it exists
		EndLicensingSessionEx(m_session_obtained_map.begin()->first);
	}
	}

   if (m_MessageDispatchThread != NULL)
	{
	   TerminateThread(m_MessageDispatchThread,0);
	}
   if (m_LicenseValidityThread != NULL)
	{
	   TerminateThread(m_LicenseValidityThread,0);
	}
	
	CloseHandle(m_MessageDispatchThread);
	CloseHandle(m_LicenseValidityThread);
	
	CloseHandle(m_ThreadKillEvent);
	CloseHandle(m_MemberLock);
}

/* ConnectEx()
 * 
 * SLB 29.feb.2024 CR.32662.V01; Refactored to return void and throw ComError
 * for problems.
------------------------------------------------------------------------------------*/
void SolimarLicenseManagerWrapper::LicensingWrapper::ConnectEx(
	const std::wstring &server)
{
	// UseOnlySharedLicenses = false
	// UseAsBackup = false
	ConnectEx(server, false, false);
}

/* ConnectEx()
 *
 * SLB 29.feb.2024 CR.32662.V01; Refactored to return void and throw ComError
 * for problems.
------------------------------------------------------------------------------------*/
void SolimarLicenseManagerWrapper::LicensingWrapper::ConnectEx(
	const std::wstring &server, 
	bool bUseOnlySharedLicenses, 
	bool bUseAsBackup, 
	bool bUseOnlyLicenseViewer)
{
	//CEnterExitMsg eem(__WFUNCTION__, server.c_str());

	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

	// authenticate
	ISolimarLicenseMgr8* pItf = NULL;
	HRESULT hr = m_licenseManagerPtr.CopyTo(&pItf);
	if (FAILED(hr)) { throw ComError(hr, L"SolimarLicenseManagerWrapper::LicensingWrapper::ConnectEx() Problem creating copy of the ISolimarLicenseMgr8 interface."); }
	
	// try to authenticate the license manager
	hr = AuthenticateServer(reinterpret_cast<IDispatch*>(pItf));
	if (FAILED(hr)) { pItf->Release(); throw ComError(hr, L"SolimarLicenseManagerWrapper::LicensingWrapper::ConnectEx() Problem... Manager did not authenticate correctly."); }

	// let the license manager authenticate this client
	hr = AuthenticateToServer(reinterpret_cast<IDispatch*>(pItf));
	if (FAILED(hr)) { pItf->Release(); throw ComError(hr, L"SolimarLicenseManagerWrapper::LicensingWrapper::ConnectEx() Problem... This client did not authenticate by Manager."); }
	
	// No longer need the raw interface for authentication. Release it.
	pItf->Release();

	// make a connection to the license manager
	hr = m_licenseManagerPtr->Connect3(
		CComBSTR(server.c_str()),									//Key Server
		(bUseOnlySharedLicenses * CF_SOFTWARE_LICENSING) |	//Use only Shared Licensing
		(bUseAsBackup * CF_BACKUP_SERVER) |						//Use as Backup
		(bUseOnlyLicenseViewer * CF_ONLY_LICENSE_VIEWER));	//Use only License Viewer
	if (FAILED(hr)) { throw ComError(hr, L"SolimarLicenseManagerWrapper::LicensingWrapper::ConnectEx() Problem... This client failed to connect to the Manager."); }
}

/* ConnectByProductEx()
 *
 * SLB 29.feb.2024 CR.32662.V01; Refactored to return void and throw ComError
 * for problems.
------------------------------------------------------------------------------------*/
void SolimarLicenseManagerWrapper::LicensingWrapper::ConnectByProductEx(
	long product, 
	bool bUseSharedLicenseServers)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)
	
	// authenticate
	ISolimarLicenseMgr8* pItf = NULL;
	HRESULT hr = m_licenseManagerPtr.CopyTo(&pItf);
	if (FAILED(hr)) { throw ComError(hr, L"SolimarLicenseManagerWrapper::LicensingWrapper::ConnectEx() Problem creating copy of the ISolimarLicenseMgr8 interface."); }

	// try to authenticate the license manager
	hr = AuthenticateServer(reinterpret_cast<IDispatch*>(pItf));
	if (FAILED(hr)) { pItf->Release(); throw ComError(hr, L"SolimarLicenseManagerWrapper::LicensingWrapper::ConnectEx() Problem... Manager did not authenticate correctly."); }

	// let the license manager authenticate this client
	hr = AuthenticateToServer(reinterpret_cast<IDispatch*>(pItf));
	if (FAILED(hr)) { pItf->Release(); throw ComError(hr, L"SolimarLicenseManagerWrapper::LicensingWrapper::ConnectEx() Problem... This client did not authenticate by Manager."); }

	// No longer need the raw interface for authentication. Release it.
	pItf->Release();

	// make a connection to the license manager
	hr = m_licenseManagerPtr->ConnectByProduct(
		product, 
		bUseSharedLicenseServers ? VARIANT_TRUE : VARIANT_FALSE);

	if (FAILED(hr)) { throw ComError(hr, L"SolimarLicenseManagerWrapper::LicensingWrapper::ConnectByProductEx() Problem... This client failed to connect to the Manager."); }
}

void SolimarLicenseManagerWrapper::LicensingWrapper::GetInfoByProductEx(
	long product, 
	std::wstring& primary_server, 
	std::wstring& backup_server,
	bool& bTestDev, 
	bool bUseSharedLicenseServers)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)
	
	HRESULT hr;
	CComBSTR bstrPrimary;
	CComBSTR bstrBackup;
	VARIANT_BOOL vbIsTestDev = VARIANT_TRUE;
	hr = m_licenseManagerPtr->GetInfoByProduct(product, bUseSharedLicenseServers ? VARIANT_TRUE : VARIANT_FALSE, &bstrPrimary, &bstrBackup, &vbIsTestDev);
	if (FAILED(hr)) { throw ComError(hr, L"SolimarLicenseManagerWrapper::LicensingWrapper::GetInfoByProductEx(). COM call licenseManager::GetInfoByProduct() failed."); }

	//if (FAILED(hr)) { LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::GetInfoByProductEx()", hr); return hr; }

	// success. assign output values.
	primary_server = bstrPrimary;
	backup_server = bstrBackup;
	bTestDev = vbIsTestDev == VARIANT_TRUE;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::DisconnectEx()
{
	//CEnterExitMsg eem(__WFUNCTION__);

	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)
	
	HRESULT hr = m_licenseManagerPtr->Disconnect();
	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::DisconnectEx()", hr);

	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::InitializeEx(
	long product, 
	long prod_ver_major, 
	long prod_ver_minor, 
	bool single_key, 
	const std::wstring &specific_single_key_ident, 
	bool lock_keys, 
	DWORD ui_level, 
	unsigned long grace_period_minutes)
{
	//CEnterExitMsg eem(__WFUNCTION__);
	
	return InitializeEx(L"", product, prod_ver_major, prod_ver_minor, single_key, specific_single_key_ident, lock_keys, ui_level, grace_period_minutes);
}
HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::InitializeEx(
	const std::wstring &app_instance, 
	long product, 
	long prod_ver_major, 
	long prod_ver_minor, 
	bool single_key, 
	const std::wstring &specific_single_key_ident, 
	bool lock_keys, 
	DWORD ui_level, 
	unsigned long grace_period_minutes, 
	bool app_instance_lock_keys, 
	bool bypass_remote_key_restriction)
{
	//CEnterExitMsg eem(__WFUNCTION__, app_instance.c_str());

   SafeMutex mutex(m_MemberLock);
   ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

   HRESULT hr = m_licenseManagerPtr->Initialize3(
		CComBSTR(app_instance.c_str()),
		product, 
		prod_ver_major, 
		prod_ver_minor, 
		(single_key ? VARIANT_TRUE : VARIANT_FALSE), 
		CComBSTR(specific_single_key_ident.c_str()),
		(lock_keys ? VARIANT_TRUE : VARIANT_FALSE), 
		ui_level, 
		grace_period_minutes, 
		app_instance_lock_keys ? VARIANT_TRUE : VARIANT_FALSE,
		bypass_remote_key_restriction ? VARIANT_TRUE : VARIANT_FALSE);

	ResumeThread(m_MessageDispatchThread);
	ResumeThread(m_LicenseValidityThread);

   LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::InitializeEx()", hr);
   return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::InitializeViewOnly(
	const std::wstring& app_instance,
	long product,
	long prod_ver_major, 
	long prod_ver_minor)
{
	//CEnterExitMsg eem(__WFUNCTION__, app_instance.c_str());

   SafeMutex mutex(m_MemberLock);

	HRESULT hr;
	hr = m_licenseManagerPtr->SoftwareLicense_InitializeViewOnly(
		CComBSTR(app_instance.c_str()),
		product,
		prod_ver_major, 
		prod_ver_minor);

	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::InitializeViewOnly()", hr);
   return hr;
}



HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::KeyProductExistsEx(
	long product, 
	long prod_ver_major, 
	long prod_ver_minor, 
	bool* bKeyExists)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

	VARIANT_BOOL bVtKeyExist = VARIANT_FALSE;
	HRESULT hr = m_licenseManagerPtr->KeyProductExists(product, prod_ver_major, prod_ver_minor, &bVtKeyExist);
	if (SUCCEEDED(hr))
		*bKeyExists = bVtKeyExist==VARIANT_TRUE;

	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::KeyProductExistsEx()", hr);
   return hr;
}

bool SolimarLicenseManagerWrapper::LicensingWrapper::RegisterMessageCallback(
	void* pContext, 
	LicenseMessageCallbackPtr LicenseMessageCallback)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	SafeMutex mutex(m_MemberLock);

	m_license_message_callback_context = pContext;
	m_license_message_callback = LicenseMessageCallback;
	return true;
}

bool SolimarLicenseManagerWrapper::LicensingWrapper::UnregisterMessageCallback()
{
	//CEnterExitMsg eem(__WFUNCTION__);

	SafeMutex mutex(m_MemberLock);

	m_license_message_callback_context = 0;
	m_license_message_callback = 0;
	return true;
}
bool SolimarLicenseManagerWrapper::LicensingWrapper::RegisterInvalidLicenseCallback(
	void* pContext, 
	LicenseInvalidCallbackPtr LicenseInvalidCallback)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	SafeMutex mutex(m_MemberLock);

	m_license_invalid_callback_context = pContext;
	m_license_invalid_callback = LicenseInvalidCallback;
	return true;
}

bool SolimarLicenseManagerWrapper::LicensingWrapper::UnregisterInvalidLicenseCallback()
{
	//CEnterExitMsg eem(__WFUNCTION__);

	SafeMutex mutex(m_MemberLock);
	m_license_invalid_callback_context = 0;
	m_license_invalid_callback = 0;
	return true;
}


HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseTotalEx(
	long module, 
	long* license_count)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)
	
	HRESULT hr = m_licenseManagerPtr->ModuleLicenseTotal(module, license_count);

	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseTotalEx()", hr);
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseInUseEx(
	long module, 
	long* license_count)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)
	
	HRESULT hr = m_licenseManagerPtr->ModuleLicenseInUse(module, license_count);
	
	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseInUseEx()", hr);
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseInUse_ByAppEx(
	long module, 
	long* license_count)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

	HRESULT hr = m_licenseManagerPtr->ModuleLicenseInUse_ByApp(module, license_count);

	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseInUse_ByAppEx()", hr);
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseObtainEx(
	long module, 
	long license_count)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)
	
	HRESULT hr = m_licenseManagerPtr->ModuleLicenseObtain(module, license_count);

	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseObtainEx()", hr);
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseCounterDecrementEx(
	long module, 
	long license_count)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)
	
	HRESULT hr = m_licenseManagerPtr->ModuleLicenseCounterDecrement(module, license_count);
	
	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseCounterDecrementEx()", hr);
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseReleaseEx(
	long module, 
	long license_count)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)
	
	HRESULT hr = m_licenseManagerPtr->ModuleLicenseRelease(module, license_count);
	
	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseReleaseEx()", hr);
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ValidateLicenseEx(
	bool* b_valid)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)
	
	VARIANT_BOOL license_valid;
	HRESULT hr = m_licenseManagerPtr->ValidateLicense(&license_valid);
	
	*b_valid = license_valid == VARIANT_TRUE;
	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ValidateLicenseEx()", hr);
	return hr;
}

DWORD WINAPI SolimarLicenseManagerWrapper::LicensingWrapper::MessageDispatchThreadProc(
	LPVOID pWrapper)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	CoInitializeEx(0, COINIT_MULTITHREADED);
	DWORD thread_kill(0);

	//pLicenseMessages is not used anymore, to get the messages, the ILicensingMessage* must be from the same 
	//object that had initialized called against it.
	ILicensingMessage* pLicenseMessages = NULL;
	HRESULT hr = S_OK;

	int count(0);
	int invokeTime(150); //in 100 MilliSeconds //150 == 15000 milliseconds
	while (1)
	{
		// check whether the thread should terminate
		if (WAIT_OBJECT_0 == WaitForSingleObject(((LicensingWrapper*)pWrapper)->m_ThreadKillEvent,0))
		{
			break;
		}
		
		if (count % invokeTime == 0)
		{
			//Only do once every invokeTime
			hr = ((LicensingWrapper*)pWrapper)->MessageDispatchThreadProc(pLicenseMessages);
			count = 0;
		}
		count++;
		
		Sleep(100);	//100 MilliSeconds
	}

	// Clean up
	CoUninitialize();

	return 0;
}

/*  MessageDispatchThreadProc()
 *
 * SLB 15.mar.2025 CR.32662.V06; Changed to convert the message object's CFILETIME into 
 * the VARIANT DATE_TIME  that the call back needs.
------------------------------------------------------------------------------------*/
HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::MessageDispatchThreadProc(
	ILicensingMessage* pLicenseMessages)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	
	HRESULT hr = S_OK;
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

	if (m_licenseMessagePtr == NULL)
		return S_FALSE;

	if (m_license_message_callback == 0)
		return S_FALSE;
	
	LicensingMessageList message_list;
	VARIANT vtMessageList;
	VariantInit(&vtMessageList);
	//hr = pLicenseMessages->GetLicenseMessageList(VARIANT_TRUE, &vtMessageList);
	hr = m_licenseMessagePtr->GetLicenseMessageList(VARIANT_TRUE, &vtMessageList);

LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::MessageDispatchThreadProc() - GetLicenseMessageList()", hr);
	if (FAILED(hr)) return hr;
	if (vtMessageList.vt & VT_ARRAY)
		message_list = vtMessageList;
	VariantClear(&vtMessageList);

	if (m_license_message_callback)
	{
		for (LicensingMessageList::iterator m = message_list.begin(); m != message_list.end(); ++m)
		{
			
			m_license_message_callback(
				m_license_message_callback_context, 
				m->m_key_ident.c_str(), 
				m->m_message_type, 
				m->m_error, 
				&TimeHelper::TimeTToVariant(m->m_timestamp),										// CR.32662.V06; modified.
				m->m_message.c_str());
		}
	}
	return S_OK;
}

DWORD WINAPI SolimarLicenseManagerWrapper::LicensingWrapper::LicenseValidityThreadProc(LPVOID pWrapper)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	CoInitializeEx(0, COINIT_MULTITHREADED);
	HRESULT hr = S_OK;
	DWORD thread_kill(0);
	int count(0);
	int invokeTime(600); //in 100 MilliSeconds //600 == 60000 milliseconds = 60 seconds
	while (1)
	{
		// check whether the thread should terminate
		if (WAIT_OBJECT_0 == WaitForSingleObject(((LicensingWrapper*)pWrapper)->m_ThreadKillEvent,0))
		{
			break;
		}
		
		if (count % invokeTime == 0)
		{
			//Only do once every invokeTime
			hr = ((LicensingWrapper*)pWrapper)->LicenseValidityThreadProc();
			count = 0;
		}
		count++;
		
		Sleep(100);	//100 MilliSeconds
	}
	CoUninitialize();

	return 0;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::LicenseValidityThreadProc()
{
	//CEnterExitMsg eem(__WFUNCTION__);

	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

	if (m_license_invalid_callback == 0)
		return S_FALSE;

	bool bValid = false;
	HRESULT hr = ValidateLicenseEx(&bValid);
	if (SUCCEEDED(hr))	//If succeeded, look at bValid
		hr = bValid ? S_OK : E_FAIL;

	if (m_license_invalid_callback)
	{
		if (m_validityCheck_LastHr != hr)
		{
			m_license_invalid_callback(m_license_invalid_callback_context, hr, LicenseServerError::GetErrorMessage(hr).c_str());
			m_validityCheck_LastHr = hr;
		}
		//if (FAILED(hr))
		//	m_license_invalid_callback(m_license_invalid_callback_context, hr, LicenseServerError::GetErrorMessage(hr).c_str());
	}
	return hr;
	//return E_NOTIMPL;
}

std::wstring SolimarLicenseManagerWrapper::LicensingWrapper::StringToWstring(const std::string &s)
{
	size_t characters;
	#if _MSC_VER >= 1400	
		mbstowcs_s(&characters, 0, 0, s.c_str(), 256);
	#else
		characters = mbstowcs(0, s.c_str(), 256);
	#endif

	wchar_t* ws = new wchar_t[characters+1];
	ws[0]=0;
	
	#if _MSC_VER >= 1400	
		size_t tmpValue;
		mbstowcs_s(
			&tmpValue,
			ws, 
			characters,	//size in words
			s.c_str(),
			characters);
	#else
		mbstowcs(ws,s.c_str(),characters);
	#endif
	ws[characters]=0;
	std::wstring ret = ws;
	delete [] ws;
	return ret;
}


long SolimarLicenseManagerWrapper::LicensingWrapper::LookupProductID(
	const std::wstring &product)																		// CR.32662.V06; modified.
{
	try
	{
		return keyspec.products[product.c_str()].id;
	}
	catch (...)
	{
		return -1;
	}
}

long SolimarLicenseManagerWrapper::LicensingWrapper::LookupProductID(
	const std::string &product)																		// CR.32662.V06; modified.
{
	return LookupProductID(StringToWstring(product));
}

long SolimarLicenseManagerWrapper::LicensingWrapper::LookupModuleID(
	long product_id, 
	const std::wstring &module)																		// CR.32662.V06; modified.
{
	try
	{
		return keyspec.products[product_id][module.c_str()].id;
	}
	catch (...)
	{
		return -1;
	}
}

long SolimarLicenseManagerWrapper::LicensingWrapper::LookupModuleID(
	long product_id, 
	const std::string &module)																			// CR.32662.V06; modified.
{
	return LookupModuleID(product_id, StringToWstring(module));
}

long SolimarLicenseManagerWrapper::LicensingWrapper::LookupHeaderID(
	const std::wstring &header)																		// CR.32662.V06; modified.
{
	try
	{
		return keyspec.headers[header.c_str()].id;
	}
	catch (...)
	{
		return -1;
	}
}

long SolimarLicenseManagerWrapper::LicensingWrapper::LookupHeaderID(
	const std::string &header)																			// CR.32662.V06; modified.
{
	return LookupHeaderID(StringToWstring(header));
}


HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::GetVersionLicenseManagerEx(
	long* p_ver_major, 
	long* p_ver_minor, 
	long* p_ver_build)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)
	
	HRESULT hr = m_licenseManagerPtr->GetVersionLicenseManager(p_ver_major, p_ver_minor, p_ver_build);

	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::GetVersionLicenseManagerEx()", hr);
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::GetVersionLicenseServerEx(
	const std::wstring &server,																		// CR.32662.V06; modified.
	long* p_ver_major, 
	long* p_ver_minor, 
	long* p_ver_build)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)
	
	CComBSTR bstrServer(server.c_str());
	HRESULT hr = m_licenseManagerPtr->GetVersionLicenseServer(bstrServer, p_ver_major, p_ver_minor, p_ver_build);

	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::GetVersionLicenseServerEx()", hr);
	return hr;
}


HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::StartLicensingSessionEx(
	long* pSessionID)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	HRESULT hr(S_OK);
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

	int newSessionID = 1;
	while (m_session_obtained_map.find(newSessionID) != m_session_obtained_map.end())
		newSessionID++;

	std::map<int, int>* pNewMap = new std::map<int, int>();
	pNewMap->clear();
	m_session_obtained_map.insert(std::map<int, std::map<int, int>*>::value_type(newSessionID, pNewMap));
	*pSessionID = newSessionID;
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseObtainLicensingSessionEx(
	long session_id, 
	long module, 
	long license_count)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	HRESULT hr(E_INVALIDARG);
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

	std::map<int, std::map<int, int>*>::iterator sessionMapIt = m_session_obtained_map.find(session_id);
	if (sessionMapIt != m_session_obtained_map.end())
	{
		hr = ModuleLicenseObtainEx(module, license_count);
		if (SUCCEEDED(hr))
		{
			std::map<int, int>* pNewMap = sessionMapIt->second;
			if (pNewMap != NULL)
			{
				std::map<int, int>::iterator mapIt = pNewMap->find(module);
				if (mapIt == pNewMap->end())	//New item
					pNewMap->insert(std::map<int, int>::value_type(module, license_count));
				else	//Update existing item
					mapIt->second += license_count;
			}
		}
	}
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseReleaseLicensingSessionEx(
	long session_id, 
	long module, 
	long license_count)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	HRESULT hr(E_INVALIDARG);
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

	std::map<int, std::map<int, int>*>::iterator sessionMapIt = m_session_obtained_map.find(session_id);
	if (sessionMapIt != m_session_obtained_map.end())
	{
		std::map<int, int>* pNewMap = sessionMapIt->second;
		std::map<int, int>::iterator mapIt = pNewMap->find(module);
		if (mapIt != pNewMap->end())
		{
			//Verify that this sessionID has obtained enough licenses to release
			if ((long)mapIt->second >= license_count)
			{
				hr = ModuleLicenseReleaseEx(module, license_count);
				if (SUCCEEDED(hr))
				{
					mapIt->second -= license_count;
				}
			}
		}
	}
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseInUseLicensingSessionEx(
	long session_id, 
	long module, 
	long* plicenseCount)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	HRESULT hr(E_INVALIDARG);
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

	std::map<int, std::map<int, int>*>::iterator sessionMapIt = m_session_obtained_map.find(session_id);
	if (sessionMapIt != m_session_obtained_map.end())
	{
		std::map<int, int>* pNewMap = sessionMapIt->second;
		std::map<int, int>::iterator mapIt = pNewMap->find(module);
		if (mapIt != pNewMap->end())
			*plicenseCount = (long)mapIt->second;
		else
			*plicenseCount = 0;
		hr = S_OK;
	}
	return hr;
}
//Side Effect: Erases the session_id in the m_session_obtained_map if it exists
HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::EndLicensingSessionEx(
	long session_id)
{
	//CEnterExitMsg eem(__WFUNCTION__);

	HRESULT hr(E_INVALIDARG);
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

	std::map<int, std::map<int, int>*>::iterator sessionMapIt = m_session_obtained_map.find(session_id);
	if (sessionMapIt != m_session_obtained_map.end())
	{
		std::map<int, int>* pNewMap = sessionMapIt->second;
		if (pNewMap != NULL)
		{
			while (!pNewMap->empty())
			{
				//ignore errors releasing...
				ModuleLicenseReleaseEx(pNewMap->begin()->first, pNewMap->begin()->second);
				pNewMap->erase(pNewMap->begin());
			}
			delete pNewMap;
			m_session_obtained_map.erase(sessionMapIt);
		}
		hr = S_OK;
	}
	return hr;
}


/*
 * Namespace: AliasedResourceIf
 *
 * SLB 11.jun.2024 CR.32662.V03; Added this namespace as it is required by the Attribs v4 code, but it
 * is never used/exercised by licensing.
 *
 * The methods in the namespace are required to be implemented by the code that uses the
 * AliasedResourceIf namespace. At this time primarily the attribs methods in CAliasableResourceString.h
 */
namespace AliasedResourceIf
{
	/* IFADTGetResourceName() - This method resolves the resource name and returns it. It uses the
	 * CSpdBaseObject::GetResourceNameOfAlias() method to retrieve the resourcename.
	 *
	 * SLB 29.mar.2013 CR.15891.V03; created.
	-----------------------------------------------------------------------------------------------------*/
	HRESULT IFADTGetResourceName(
		const CSpdCString& wsAliasName,
		CSpdCString& wsResourceName)
	{
		wsResourceName = wsAliasName;
		return S_OK;
	}
}  // end of namespace
