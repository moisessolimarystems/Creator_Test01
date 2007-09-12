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
	m_MessageDispatchThread(0),
	m_LicenseValidityThread(0),
	m_MemberLock(CreateMutex(0, FALSE, 0)),
	m_ThreadKillEvent(CreateEvent(0, TRUE, FALSE, 0))
{
	ISolimarLicenseMgr5* pLocalLicenseMgr = NULL;
	HRESULT hr = CoCreateInstance(__uuidof(CSolimarLicenseMgr), NULL, CLSCTX_INPROC_SERVER, __uuidof(ISolimarLicenseMgr5), (void**)&pLocalLicenseMgr);
	if (SUCCEEDED(hr))
	{
		hr = m_licenseManagerPtr.Attach(pLocalLicenseMgr);
		if (SUCCEEDED(hr))
		{
			m_MessageDispatchThread = CreateThread(0, 0, MessageDispatchThreadProc, this, 0, 0);
			m_LicenseValidityThread = CreateThread(0, 0, LicenseValidityThreadProc, this, 0, 0);
		}
		else
		{
			pLocalLicenseMgr->Release();
		}
	}
	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::LicensingWrapper()", hr);
}

SolimarLicenseManagerWrapper::LicensingWrapper::LicensingWrapper(const LicensingWrapper &o) : 
	ChallengeResponseHelper(challenge_key_manager_thisauthuser_private, sizeof(challenge_key_manager_thisauthuser_private)/sizeof(BYTE), challenge_key_manager_userauththis_public, sizeof(challenge_key_manager_userauththis_public)/sizeof(BYTE)),
	m_license_message_callback_context(0),
	m_license_message_callback(0),
	m_MessageDispatchThread(0),
	m_LicenseValidityThread(0),
	m_MemberLock(CreateMutex(0, FALSE, 0)),
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
	m_licenseManagerPtr = o.m_licenseManagerPtr;

	return *this;
}

SolimarLicenseManagerWrapper::LicensingWrapper::~LicensingWrapper()
{
	// signal the threads to terminate
	SetEvent(m_ThreadKillEvent);
	
	// wait for the threads to terminate
	HANDLE threads[2] = {m_MessageDispatchThread, m_LicenseValidityThread};
	WaitForMultipleObjects(sizeof(threads)/sizeof(HANDLE), threads, TRUE, 1000);
	
   if(m_MessageDispatchThread != NULL)
	   TerminateThread(m_MessageDispatchThread,0);
   if(m_LicenseValidityThread != NULL)
	   TerminateThread(m_LicenseValidityThread,0);
	
	CloseHandle(m_MessageDispatchThread);
	CloseHandle(m_LicenseValidityThread);
	
	CloseHandle(m_ThreadKillEvent);
	CloseHandle(m_MemberLock);
}

bool SolimarLicenseManagerWrapper::LicensingWrapper::Connect(std::wstring server)
{
	return Connect(	server, 
					false,	//bUseOnlySharedLicenses
					false);	//bUseAsBackup
}
bool SolimarLicenseManagerWrapper::LicensingWrapper::Connect(std::wstring server, bool bUseOnlySharedLicenses, bool bUseAsBackup)
{
	SafeMutex mutex(m_MemberLock);
	
	// authenticate
	ISolimarLicenseMgr5* pISolimarLicenseMgr = NULL;
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
	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::Connect()", hr);
	return SUCCEEDED(hr);
}

bool SolimarLicenseManagerWrapper::LicensingWrapper::Initialize(long product, long prod_ver_major, long prod_ver_minor, bool single_key, std::wstring &specific_single_key_ident, bool lock_keys, DWORD ui_level, unsigned long grace_period_minutes)
{
   return Initialize(L"", product, prod_ver_major, prod_ver_minor, single_key, specific_single_key_ident, lock_keys, ui_level, grace_period_minutes);
}
bool SolimarLicenseManagerWrapper::LicensingWrapper::Initialize(std::wstring application_instance, long product, long prod_ver_major, long prod_ver_minor, bool single_key, std::wstring &specific_single_key_ident, bool lock_keys, DWORD ui_level, unsigned long grace_period_minutes, bool application_instance_lock_keys, bool bypass_remote_key_restriction)
{
   SafeMutex mutex(m_MemberLock);

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

   LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::Initialize()", hr);
   return SUCCEEDED(hr);
}

bool SolimarLicenseManagerWrapper::LicensingWrapper::RegisterMessageCallback(void* pContext, LicenseMessageCallbackPtr LicenseMessageCallback)
{
	SafeMutex mutex(m_MemberLock);
	m_license_message_callback_context = pContext;
	m_license_message_callback = LicenseMessageCallback;
	return true;
}

bool SolimarLicenseManagerWrapper::LicensingWrapper::UnregisterMessageCallback()
{
	SafeMutex mutex(m_MemberLock);
	m_license_message_callback_context = 0;
	m_license_message_callback = 0;
	return true;
}

bool SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseTotal(long module, long* license_count)
{
	SafeMutex mutex(m_MemberLock);
	
	HRESULT hr = m_licenseManagerPtr->ModuleLicenseTotal(module, license_count);

	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseTotal()", hr);
	return SUCCEEDED(hr);
}
bool SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseInUse(long module, long* license_count)
{
	SafeMutex mutex(m_MemberLock);
	
	HRESULT hr = m_licenseManagerPtr->ModuleLicenseInUse(module, license_count);
	
	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseInUse()", hr);
	return SUCCEEDED(hr);
}

bool SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseObtain(long module, long license_count)
{
	SafeMutex mutex(m_MemberLock);
	
	HRESULT hr = m_licenseManagerPtr->ModuleLicenseObtain(module, license_count);

	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseObtain()", hr);
	return SUCCEEDED(hr);
}

bool SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseCounterDecrement(long module, long license_count)
{
	SafeMutex mutex(m_MemberLock);
	
	HRESULT hr = m_licenseManagerPtr->ModuleLicenseCounterDecrement(module, license_count);
	
	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseCounterDecrement()", hr);
	return SUCCEEDED(hr);
}

bool SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseRelease(long module, long license_count)
{
	SafeMutex mutex(m_MemberLock);
	
	HRESULT hr = m_licenseManagerPtr->ModuleLicenseRelease(module, license_count);
	
	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseRelease()", hr);
	return SUCCEEDED(hr);
}

bool SolimarLicenseManagerWrapper::LicensingWrapper::ValidateLicense()
{
	SafeMutex mutex(m_MemberLock);
	
	VARIANT_BOOL license_valid;
	HRESULT hr = m_licenseManagerPtr->ValidateLicense(&license_valid);
	
	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::ValidateLicense()", hr);
	return SUCCEEDED(hr) && (license_valid==VARIANT_TRUE);
}

DWORD WINAPI SolimarLicenseManagerWrapper::LicensingWrapper::MessageDispatchThreadProc(LPVOID pWrapper)
{
	CoInitializeEx(0, COINIT_MULTITHREADED);
	DWORD thread_kill(0);

	ILicensingMessage* pLicenseMessages = NULL;
	HRESULT hr = CoCreateInstance(__uuidof(CSolimarLicenseMgr), NULL, CLSCTX_INPROC_SERVER, __uuidof(ILicensingMessage), (void**)&pLicenseMessages);
	assert(SUCCEEDED(hr));
	if (FAILED(hr))
		return 0;

	while(1)
	{
		// check whether the thread should terminate
		if (WAIT_OBJECT_0 == WaitForSingleObject(((LicensingWrapper*)pWrapper)->m_ThreadKillEvent,0))
			break;
		
		hr = ((LicensingWrapper*)pWrapper)->MessageDispatchThreadProc(pLicenseMessages);
		
		Sleep(5000);
	}

	// Clean up
	pLicenseMessages->Release();

	CoUninitialize();
	return 0;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::MessageDispatchThreadProc(ILicensingMessage* pLicenseMessages)
{
	HRESULT hr = S_OK;
	
	SafeMutex mutex(m_MemberLock);
	
	LicensingMessageList message_list;
	VARIANT vtMessageList;
	VariantInit(&vtMessageList);
	hr = pLicenseMessages->GetLicenseMessageList(VARIANT_TRUE, &vtMessageList);
	if (FAILED(hr)) return hr;
	if (vtMessageList.vt & VT_ARRAY)
		message_list = vtMessageList;
	VariantClear(&vtMessageList);
	
	if (m_license_message_callback)
	{
		for (LicensingMessageList::iterator m = message_list.begin(); m != message_list.end(); ++m)
		{
			m_license_message_callback(m_license_message_callback_context, m->key_ident.c_str(), m->message_type, m->error, &m->timestamp, m->message.c_str());
		}
	}
	return S_OK;
}

DWORD WINAPI SolimarLicenseManagerWrapper::LicensingWrapper::LicenseValidityThreadProc(LPVOID pWrapper)
{
	CoInitializeEx(0, COINIT_MULTITHREADED);
	HRESULT hr = S_OK;
	DWORD thread_kill(0);
	
	while(1)
	{
		// check whether the thread should terminate
		if (WAIT_OBJECT_0 == WaitForSingleObject(((LicensingWrapper*)pWrapper)->m_ThreadKillEvent,0))
			break;
		
		hr = ((LicensingWrapper*)pWrapper)->LicenseValidityThreadProc();
		
		Sleep(10000);
	}
	CoUninitialize();
	return 0;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::LicenseValidityThreadProc()
{
	// dispatch messages
	return E_NOTIMPL;
}

std::wstring SolimarLicenseManagerWrapper::LicensingWrapper::StringToWstring(const std::string &s)
{
	//size_t characters = mbstowcs(0, s.c_str(), 256);
	size_t characters;
	mbstowcs_s(&characters, 0, 0, s.c_str(), 256);
	wchar_t* ws = new wchar_t[characters];
	ws[0]=0;
	size_t tmpValue;
	mbstowcs_s(	&tmpValue,
				ws, 
				wcslen(ws) + 1,	//size in words
				s.c_str(),
				characters);
	ws[255]=0;
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


bool SolimarLicenseManagerWrapper::LicensingWrapper::GetVersionLicenseManager(long* p_ver_major, long* p_ver_minor, long* p_ver_build)
{
	SafeMutex mutex(m_MemberLock);
	
	HRESULT hr = m_licenseManagerPtr->GetVersionLicenseManager(p_ver_major, p_ver_minor, p_ver_build);

	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::GetVersionLicenseManager()", hr);
	return SUCCEEDED(hr);
}
bool SolimarLicenseManagerWrapper::LicensingWrapper::GetVersionLicenseServer(std::wstring server, long* p_ver_major, long* p_ver_minor, long* p_ver_build)
{
	SafeMutex mutex(m_MemberLock);
	
	BSTR bstrServer = SysAllocString(server.c_str());
	HRESULT hr = m_licenseManagerPtr->GetVersionLicenseServer(bstrServer, p_ver_major, p_ver_minor, p_ver_build);
	SysFreeString(bstrServer);

	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingWrapper::GetVersionLicenseServer()", hr);
	return SUCCEEDED(hr);
}