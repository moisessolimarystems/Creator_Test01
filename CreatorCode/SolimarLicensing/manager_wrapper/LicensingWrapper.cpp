#ifndef _WIN32_DCOM
#define _WIN32_DCOM // For CoInitializeEx
#endif

#include "LicensingWrapper.h"

#include "..\common\InProcPtr.h"
#include "..\common\ChallengeResponseHelper.h"
#include "..\common\_IObjectAuthentication.h"
#include "..\common\SafeMutex.h"

#include <assert.h>		// For ASSERT
#include <string>

BYTE SolimarLicenseManagerWrapper::LicensingWrapper::challenge_key_manager_userauththis_public[] = {
#include "..\common\keys\SolimarLicenseManager.UserAuthThis.public.key.txt"
};

BYTE SolimarLicenseManagerWrapper::LicensingWrapper::challenge_key_manager_thisauthuser_private[] = {
#include "..\common\keys\SolimarLicenseManager.ThisAuthUser.private.key.txt"
};

KeySpec SolimarLicenseManagerWrapper::LicensingWrapper::keyspec;

SolimarLicenseManagerWrapper::LicensingWrapper::LicensingWrapper() : 
	ChallengeResponseHelper(challenge_key_manager_thisauthuser_private, sizeof(challenge_key_manager_thisauthuser_private)/sizeof(BYTE), challenge_key_manager_userauththis_public, sizeof(challenge_key_manager_userauththis_public)/sizeof(BYTE)),
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
//OutputDebugStringW(L"LicensingWrapper::LicensingWrapper() - Enter");
	ISolimarLicenseMgr6* pLocalLicenseMgr = NULL;
	HRESULT hr = CoCreateInstance(__uuidof(CSolimarLicenseMgr), NULL, CLSCTX_INPROC_SERVER, __uuidof(ISolimarLicenseMgr6), (void**)&pLocalLicenseMgr);
	if (SUCCEEDED(hr))
	{
		hr = m_licenseManagerPtr.Attach(pLocalLicenseMgr);
		if (SUCCEEDED(hr))
		{
			ILicensingMessage* pLocalLicenseMessage = NULL;
			hr = m_licenseManagerPtr->QueryInterface(__uuidof(ILicensingMessage), (void**)&pLocalLicenseMessage);
			if (SUCCEEDED(hr))
			{
				hr = m_licenseMessagePtr.Attach(pLocalLicenseMessage);
				if (SUCCEEDED(hr))
				{
					m_MessageDispatchThread = CreateThread(0, 0, MessageDispatchThreadProc, this, CREATE_SUSPENDED, 0);
					m_LicenseValidityThread = CreateThread(0, 0, LicenseValidityThreadProc, this, CREATE_SUSPENDED, 0);
				}
			}
		}
		else
		{
			pLocalLicenseMgr->Release();
		}
	}
//OutputDebugStringW(L"LicensingWrapper::LicensingWrapper() - Leave");
	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::LicensingWrapper()", hr);
	m_constructorHR = hr;
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
//OutputDebugStringW(L"LicensingWrapper::LicensingWrapper(const) - Enter");
	*this = o;
	
	m_MessageDispatchThread = CreateThread(0, 0, MessageDispatchThreadProc, this, 0, 0);
	m_LicenseValidityThread = CreateThread(0, 0, LicenseValidityThreadProc, this, 0, 0);
//OutputDebugStringW(L"LicensingWrapper::LicensingWrapper(const) - Leave");
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
//OutputDebugStringW(L"LicensingWrapper::~LicensingWrapper() - Enter");
	// signal the threads to terminate

	SetEvent(m_ThreadKillEvent);

	try
	{
		// wait for the threads to terminate
		HANDLE threads[2] = {m_MessageDispatchThread, m_LicenseValidityThread};
		int retVal = WaitForMultipleObjects(sizeof(threads)/sizeof(HANDLE), threads, TRUE, 2000);
	}
	catch(...)
	{
	}
   
	{
	SafeMutex mutex(m_MemberLock);
	while(!m_session_obtained_map.empty())
	{
		//Side Effect: Erases the session_id in the m_session_obtained_map if it exists
		EndLicensingSessionEx(m_session_obtained_map.begin()->first);
	}
	}

   if(m_MessageDispatchThread != NULL)
	{
	   TerminateThread(m_MessageDispatchThread,0);
	}
   if(m_LicenseValidityThread != NULL)
	{
	   TerminateThread(m_LicenseValidityThread,0);
	}
	
	CloseHandle(m_MessageDispatchThread);
	CloseHandle(m_LicenseValidityThread);
	
	CloseHandle(m_ThreadKillEvent);
	CloseHandle(m_MemberLock);
//OutputDebugStringW(L"LicensingWrapper::~LicensingWrapper() - Leave");
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ConnectEx(std::wstring server)
{
	return ConnectEx(	server, 
					false,	//bUseOnlySharedLicenses
					false);	//bUseAsBackup
}
HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ConnectEx(std::wstring server, bool bUseOnlySharedLicenses, bool bUseAsBackup)
{
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

	// authenticate
	ISolimarLicenseMgr6* pISolimarLicenseMgr = NULL;
	HRESULT hr = m_licenseManagerPtr.CopyTo(&pISolimarLicenseMgr);
	if (SUCCEEDED(hr))
	{
		// try to authenticate the license manager
		hr = AuthenticateServer(reinterpret_cast<IDispatch*>(pISolimarLicenseMgr));
		if (SUCCEEDED(hr))
		{ // let the license manager authenticate this client
			hr = AuthenticateToServer(reinterpret_cast<IDispatch*>(pISolimarLicenseMgr));
			if (SUCCEEDED(hr))
			{
				// make a connection to the license manager
				BSTR bstrServer = SysAllocString(server.c_str());
				hr = pISolimarLicenseMgr->Connect2(	bstrServer,													//Key Server
													(bUseOnlySharedLicenses ? VARIANT_TRUE : VARIANT_FALSE),	//Use only Shared Licensing
													(bUseAsBackup ? VARIANT_TRUE : VARIANT_FALSE));				//Use as Backup
				SysFreeString(bstrServer);
			}
		}	
		pISolimarLicenseMgr->Release();
	}
	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ConnectEx()", hr);
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ConnectByProductEx(long product, bool bUseSharedLicenseServers)
{
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)
	
	// authenticate
	ISolimarLicenseMgr6* pISolimarLicenseMgr = NULL;
	HRESULT hr = m_licenseManagerPtr.CopyTo(&pISolimarLicenseMgr);
	if (SUCCEEDED(hr))
	{
		// try to authenticate the license manager
		hr = AuthenticateServer(reinterpret_cast<IDispatch*>(pISolimarLicenseMgr));
		if (SUCCEEDED(hr))
		{ // let the license manager authenticate this client
			hr = AuthenticateToServer(reinterpret_cast<IDispatch*>(pISolimarLicenseMgr));
			if (SUCCEEDED(hr))
			{
				// make a connection to the license manager
				hr = pISolimarLicenseMgr->ConnectByProduct(product, bUseSharedLicenseServers ? VARIANT_TRUE : VARIANT_FALSE);
			}
		}	
		pISolimarLicenseMgr->Release();
	}
	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ConnectByProductEx()", hr);
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::DisconnectEx()
{
//OutputDebugStringW(L"LicensingWrapper::DisconnectEx() - Enter");
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)
	
	HRESULT hr = m_licenseManagerPtr->Disconnect();

	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::DisconnectEx()", hr);
//OutputDebugStringW(L"LicensingWrapper::DisconnectEx() - Leave");
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::InitializeEx(long product, long prod_ver_major, long prod_ver_minor, bool single_key, std::wstring specific_single_key_ident, bool lock_keys, DWORD ui_level, unsigned long grace_period_minutes)
{
   return InitializeEx(L"", product, prod_ver_major, prod_ver_minor, single_key, specific_single_key_ident, lock_keys, ui_level, grace_period_minutes);
}
HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::InitializeEx(std::wstring application_instance, long product, long prod_ver_major, long prod_ver_minor, bool single_key, std::wstring specific_single_key_ident, bool lock_keys, DWORD ui_level, unsigned long grace_period_minutes, bool application_instance_lock_keys, bool bypass_remote_key_restriction)
{
   SafeMutex mutex(m_MemberLock);
   ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

   BSTR bstrSpecificSingleKey = SysAllocString(specific_single_key_ident.c_str());
   BSTR bstrApplication_instance = SysAllocString(application_instance.c_str());
   HRESULT hr = m_licenseManagerPtr->Initialize3(	bstrApplication_instance, 
													product, 
													prod_ver_major, 
													prod_ver_minor, 
													(single_key ? VARIANT_TRUE : VARIANT_FALSE), 
													bstrSpecificSingleKey, 
													(lock_keys ? VARIANT_TRUE : VARIANT_FALSE), 
													ui_level, grace_period_minutes, 
													application_instance_lock_keys ? VARIANT_TRUE : VARIANT_FALSE,
													bypass_remote_key_restriction ? VARIANT_TRUE : VARIANT_FALSE);
   SysFreeString(bstrSpecificSingleKey);
   SysFreeString(bstrApplication_instance);


	ResumeThread(m_MessageDispatchThread);
	ResumeThread(m_LicenseValidityThread);


   LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::InitializeEx()", hr);
   return hr;
}


HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::KeyProductExistsEx(long product, long prod_ver_major, long prod_ver_minor, bool* bKeyExists)
{
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

	VARIANT_BOOL bVtKeyExist = VARIANT_FALSE;
	HRESULT hr = m_licenseManagerPtr->KeyProductExists(product, prod_ver_major, prod_ver_minor, &bVtKeyExist);
	if(SUCCEEDED(hr))
		*bKeyExists = bVtKeyExist==VARIANT_TRUE;

	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::KeyProductExistsEx()", hr);
   return hr;
}

bool SolimarLicenseManagerWrapper::LicensingWrapper::RegisterMessageCallback(void* pContext, LicenseMessageCallbackPtr LicenseMessageCallback)
{
//OutputDebugStringW(L"LicensingWrapper::RegisterMessageCallback() - Enter");
	SafeMutex mutex(m_MemberLock);

	m_license_message_callback_context = pContext;
	m_license_message_callback = LicenseMessageCallback;
//OutputDebugStringW(L"LicensingWrapper::RegisterMessageCallback() - Leave");
	return true;
}

bool SolimarLicenseManagerWrapper::LicensingWrapper::UnregisterMessageCallback()
{
//OutputDebugStringW(L"LicensingWrapper::UnregisterMessageCallback() - Enter");
	SafeMutex mutex(m_MemberLock);

	m_license_message_callback_context = 0;
	m_license_message_callback = 0;
//OutputDebugStringW(L"LicensingWrapper::UnregisterMessageCallback() - Leave");
	return true;
}
bool SolimarLicenseManagerWrapper::LicensingWrapper::RegisterInvalidLicenseCallback(void* pContext, LicenseInvalidCallbackPtr LicenseInvalidCallback)
{
//OutputDebugStringW(L"LicensingWrapper::RegisterInvalidLicenseCallback() - Enter");
	SafeMutex mutex(m_MemberLock);

	m_license_invalid_callback_context = pContext;
	m_license_invalid_callback = LicenseInvalidCallback;
//OutputDebugStringW(L"LicensingWrapper::RegisterInvalidLicenseCallback() - Leave");
	return true;
}

bool SolimarLicenseManagerWrapper::LicensingWrapper::UnregisterInvalidLicenseCallback()
{
//OutputDebugStringW(L"LicensingWrapper::UnregisterInvalidLicenseCallback() - Enter");
	SafeMutex mutex(m_MemberLock);
	m_license_invalid_callback_context = 0;
	m_license_invalid_callback = 0;
//OutputDebugStringW(L"LicensingWrapper::UnregisterInvalidLicenseCallback() - Leave");
	return true;
}


HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseTotalEx(long module, long* license_count)
{
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)
	
	HRESULT hr = m_licenseManagerPtr->ModuleLicenseTotal(module, license_count);

	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseTotalEx()", hr);
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseInUseEx(long module, long* license_count)
{
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)
	
	HRESULT hr = m_licenseManagerPtr->ModuleLicenseInUse(module, license_count);
	
	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseInUseEx()", hr);
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseInUse_ByAppEx(long module, long* license_count)
{
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

	HRESULT hr = m_licenseManagerPtr->ModuleLicenseInUse_ByApp(module, license_count);

	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseInUse_ByAppEx()", hr);
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseObtainEx(long module, long license_count)
{
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)
	
	HRESULT hr = m_licenseManagerPtr->ModuleLicenseObtain(module, license_count);

	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseObtainEx()", hr);
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseCounterDecrementEx(long module, long license_count)
{
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)
	
	HRESULT hr = m_licenseManagerPtr->ModuleLicenseCounterDecrement(module, license_count);
	
	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseCounterDecrementEx()", hr);
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseReleaseEx(long module, long license_count)
{
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)
	
	HRESULT hr = m_licenseManagerPtr->ModuleLicenseRelease(module, license_count);
	
	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseReleaseEx()", hr);
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ValidateLicenseEx(bool* b_valid)
{
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)
	
	VARIANT_BOOL license_valid;
	HRESULT hr = m_licenseManagerPtr->ValidateLicense(&license_valid);
	
	*b_valid = license_valid == VARIANT_TRUE;
	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ValidateLicenseEx()", hr);
	return hr;
}

DWORD WINAPI SolimarLicenseManagerWrapper::LicensingWrapper::MessageDispatchThreadProc(LPVOID pWrapper)
{
	CoInitializeEx(0, COINIT_MULTITHREADED);
//OutputDebugStringW(L"WINAPI LicensingWrapper::MessageDispatchThreadProc(LPVOID pWrapper) - Enter"); 
	DWORD thread_kill(0);

	//pLicenseMessages is not used anymore, to get the messages, the ILicensingMessage* must be from the same 
	//object that had initialized called against it.
	ILicensingMessage* pLicenseMessages = NULL;
	HRESULT hr = S_OK;

	int count(0);
	int invokeTime(150); //in 100 MilliSeconds //150 == 15000 milliseconds
	while(1)
	{
		// check whether the thread should terminate
		if (WAIT_OBJECT_0 == WaitForSingleObject(((LicensingWrapper*)pWrapper)->m_ThreadKillEvent,0))
		{
			break;
		}
		
		if(count % invokeTime == 0)
		{
			//Only do once every invokeTime
			hr = ((LicensingWrapper*)pWrapper)->MessageDispatchThreadProc(pLicenseMessages);
			count = 0;
		}
		count++;
		
		Sleep(100);	//100 MilliSeconds
	}

	// Clean up
//OutputDebugStringW(L"WINAPI LicensingWrapper::MessageDispatchThreadProc(LPVOID pWrapper) - Leave"); 
	CoUninitialize();

	return 0;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::MessageDispatchThreadProc(ILicensingMessage* pLicenseMessages)
{
//OutputDebugStringW(L"HRESULT LicensingWrapper::MessageDispatchThreadProc(ILicensingMessage* pLicenseMessages) - Enter"); 

	
//wchar_t tmpbuf[1024];
	HRESULT hr = S_OK;
	
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

	if(m_licenseMessagePtr == NULL)
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
//swprintf_s(tmpbuf, 1024, L"LicensingWrapper::MessageDispatchThreadProc message: %s", m->message.c_str());
//OutputDebugStringW(tmpbuf); 

			m_license_message_callback(m_license_message_callback_context, m->key_ident.c_str(), m->message_type, m->error, &m->timestamp, m->message.c_str());
		}
	}
//OutputDebugStringW(L"HRESULT LicensingWrapper::MessageDispatchThreadProc(ILicensingMessage* pLicenseMessages) - Leave"); 
	return S_OK;
}

DWORD WINAPI SolimarLicenseManagerWrapper::LicensingWrapper::LicenseValidityThreadProc(LPVOID pWrapper)
{
	CoInitializeEx(0, COINIT_MULTITHREADED);
//OutputDebugStringW(L"WINAPI LicensingWrapper::LicenseValidityThreadProc(LPVOID pWrapper) - Enter"); 
	HRESULT hr = S_OK;
	DWORD thread_kill(0);
	int count(0);
	int invokeTime(200); //in 100 MilliSeconds //200 == 20000 milliseconds
	while(1)
	{
		// check whether the thread should terminate
		if (WAIT_OBJECT_0 == WaitForSingleObject(((LicensingWrapper*)pWrapper)->m_ThreadKillEvent,0))
		{
			break;
		}
		
		if(count % invokeTime == 0)
		{
			//Only do once every invokeTime
			hr = ((LicensingWrapper*)pWrapper)->LicenseValidityThreadProc();
			count = 0;
		}
		count++;
		
		Sleep(100);	//100 MilliSeconds
	}
//OutputDebugStringW(L"WINAPI LicensingWrapper::LicenseValidityThreadProc(LPVOID pWrapper) - Leave"); 
	CoUninitialize();

	return 0;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::LicenseValidityThreadProc()
{
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

	bool bValid = false;
	HRESULT hr = ValidateLicenseEx(&bValid);
	if(SUCCEEDED(hr))	//If succeeded, look at bValid
		hr = bValid ? S_OK : E_FAIL;

	if(m_license_invalid_callback)
	{
		if(m_validityCheck_LastHr != hr)
		{
			m_license_invalid_callback(m_license_invalid_callback_context, hr, LicenseServerError::GetErrorMessage(hr).c_str());
			m_validityCheck_LastHr = hr;
		}
		//if(FAILED(hr))
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
		mbstowcs_s(	&tmpValue,
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


long SolimarLicenseManagerWrapper::LicensingWrapper::LookupProductID(std::wstring product)
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

long SolimarLicenseManagerWrapper::LicensingWrapper::LookupProductID(std::string product)
{
	return LookupProductID(StringToWstring(product));
}

long SolimarLicenseManagerWrapper::LicensingWrapper::LookupModuleID(long product_id, std::wstring module)
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

long SolimarLicenseManagerWrapper::LicensingWrapper::LookupModuleID(long product_id, std::string module)
{
	return LookupModuleID(product_id, StringToWstring(module));
}

long SolimarLicenseManagerWrapper::LicensingWrapper::LookupHeaderID(std::wstring header)
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

long SolimarLicenseManagerWrapper::LicensingWrapper::LookupHeaderID(std::string header)
{
	return LookupHeaderID(StringToWstring(header));
}


HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::GetVersionLicenseManagerEx(long* p_ver_major, long* p_ver_minor, long* p_ver_build)
{
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)
	
	HRESULT hr = m_licenseManagerPtr->GetVersionLicenseManager(p_ver_major, p_ver_minor, p_ver_build);

	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::GetVersionLicenseManagerEx()", hr);
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::GetVersionLicenseServerEx(std::wstring server, long* p_ver_major, long* p_ver_minor, long* p_ver_build)
{
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)
	
	BSTR bstrServer = SysAllocString(server.c_str());
	HRESULT hr = m_licenseManagerPtr->GetVersionLicenseServer(bstrServer, p_ver_major, p_ver_minor, p_ver_build);
	SysFreeString(bstrServer);

	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::GetVersionLicenseServerEx()", hr);
	return hr;
}


HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::StartLicensingSessionEx(long* pSessionID)
{
	HRESULT hr(S_OK);
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

	int newSessionID = 1;
	while(m_session_obtained_map.find(newSessionID) != m_session_obtained_map.end())
		newSessionID++;

	std::map<int, int>* pNewMap = new std::map<int, int>();
	pNewMap->clear();
	m_session_obtained_map.insert(std::map<int, std::map<int, int>*>::value_type(newSessionID, pNewMap));
	*pSessionID = newSessionID;
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseObtainLicensingSessionEx(long session_id, long module, long license_count)
{
	HRESULT hr(E_INVALIDARG);
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

	std::map<int, std::map<int, int>*>::iterator sessionMapIt = m_session_obtained_map.find(session_id);
	if(sessionMapIt != m_session_obtained_map.end())
	{
		hr = ModuleLicenseObtainEx(module, license_count);
		if(SUCCEEDED(hr))
		{
			std::map<int, int>* pNewMap = sessionMapIt->second;
			if(pNewMap != NULL)
			{
				std::map<int, int>::iterator mapIt = pNewMap->find(module);
				if(mapIt == pNewMap->end())	//New item
					pNewMap->insert(std::map<int, int>::value_type(module, license_count));
				else	//Update existing item
					mapIt->second += license_count;
			}
		}
	}
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseReleaseLicensingSessionEx(long session_id, long module, long license_count)
{
	HRESULT hr(E_INVALIDARG);
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

	std::map<int, std::map<int, int>*>::iterator sessionMapIt = m_session_obtained_map.find(session_id);
	if(sessionMapIt != m_session_obtained_map.end())
	{
		std::map<int, int>* pNewMap = sessionMapIt->second;
		std::map<int, int>::iterator mapIt = pNewMap->find(module);
		if(mapIt != pNewMap->end())
		{
			//Verify that this sessionID has obtained enough licenses to release
			if((long)mapIt->second >= license_count)
			{
				hr = ModuleLicenseReleaseEx(module, license_count);
				if(SUCCEEDED(hr))
				{
					mapIt->second -= license_count;
				}
			}
		}
	}
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseInUseLicensingSessionEx(long session_id, long module, long* plicenseCount)
{
	HRESULT hr(E_INVALIDARG);
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

	std::map<int, std::map<int, int>*>::iterator sessionMapIt = m_session_obtained_map.find(session_id);
	if(sessionMapIt != m_session_obtained_map.end())
	{
		std::map<int, int>* pNewMap = sessionMapIt->second;
		std::map<int, int>::iterator mapIt = pNewMap->find(module);
		if(mapIt != pNewMap->end())
			*plicenseCount = (long)mapIt->second;
		else
			*plicenseCount = 0;
		hr = S_OK;
	}
	return hr;
}
//Side Effect: Erases the session_id in the m_session_obtained_map if it exists
HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::EndLicensingSessionEx(long session_id)
{
	HRESULT hr(E_INVALIDARG);
	SafeMutex mutex(m_MemberLock);
	ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(m_constructorHR)

	std::map<int, std::map<int, int>*>::iterator sessionMapIt = m_session_obtained_map.find(session_id);
	if(sessionMapIt != m_session_obtained_map.end())
	{
		std::map<int, int>* pNewMap = sessionMapIt->second;
		if(pNewMap != NULL)
		{
			while(!pNewMap->empty())
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
