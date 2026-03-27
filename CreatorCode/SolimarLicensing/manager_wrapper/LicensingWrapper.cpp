
#include "LicensingWrapper.h"

#include "..\common\InProcPtr.h"
#include "..\common\ChallengeResponseHelper.h"
#include "..\common\_IObjectAuthentication.h"
#include "..\common\SafeMutex.h"

#include <string>

BYTE SolimarLicenseManagerWrapper::LicensingWrapper::challenge_key_manager_userauththis_public[] = {
#include "..\common\keys\SolimarLicenseManager.UserAuthThis.public.key.txt"
};

BYTE SolimarLicenseManagerWrapper::LicensingWrapper::challenge_key_manager_thisauthuser_private[] = {
#include "..\common\keys\SolimarLicenseManager.ThisAuthUser.private.key.txt"
};

KeySpec SolimarLicenseManagerWrapper::LicensingWrapper::keyspec;

SolimarLicenseManagerWrapper::LicensingWrapper::LicensingWrapper() : 
	pLicenseManager(0), 
	pLicenseManagerMessages(0),
	ChallengeResponseHelper(challenge_key_manager_thisauthuser_private, sizeof(challenge_key_manager_thisauthuser_private), challenge_key_manager_userauththis_public, sizeof(challenge_key_manager_userauththis_public)),
	m_license_message_callback_context(0),
	m_license_message_callback(0),
	m_MessageDispatchThread(0),
	m_LicenseValidityThread(0),
	m_MemberLock(CreateMutex(0, FALSE, 0)),
	m_ThreadKillEvent(CreateEvent(0, TRUE, FALSE, 0))
{
	HRESULT hr = S_OK;
	
	hr = CoCreateInstance(__uuidof(CSolimarLicenseMgr), NULL, CLSCTX_INPROC_SERVER, __uuidof(ISolimarLicenseMgr), (void**)&pLicenseManager);
	if (SUCCEEDED(hr))
	{
		hr = pLicenseManager->QueryInterface(__uuidof(ILicensingMessage), (void**)&pLicenseManagerMessages);
		if (SUCCEEDED(hr))
		{
			m_MessageDispatchThread = CreateThread(0, 0, MessageDispatchThreadProc, this, 0, 0);
			m_LicenseValidityThread = CreateThread(0, 0, LicenseValidityThreadProc, this, 0, 0);
		}
		else
		{
			pLicenseManager->Release();
			pLicenseManager = 0;
			pLicenseManagerMessages = 0;
		}
	}
	else
	{
		pLicenseManager = 0;
	}
}

SolimarLicenseManagerWrapper::LicensingWrapper::LicensingWrapper(const LicensingWrapper &o) : 
	pLicenseManager(0), 
	pLicenseManagerMessages(0),
	ChallengeResponseHelper(challenge_key_manager_thisauthuser_private, sizeof(challenge_key_manager_thisauthuser_private), challenge_key_manager_userauththis_public, sizeof(challenge_key_manager_userauththis_public)),
	m_license_message_callback_context(0),
	m_license_message_callback(0),
	m_MessageDispatchThread(0),
	m_LicenseValidityThread(0),
	m_MemberLock(CreateMutex(0, FALSE, 0)),
	m_ThreadKillEvent(CreateEvent(0, TRUE, FALSE, 0))
{
	{
		SafeMutex mutex(o.m_MemberLock);
		
		pLicenseManager = o.pLicenseManager;
		pLicenseManagerMessages = o.pLicenseManagerMessages;
		m_license_message_callback_context = o.m_license_message_callback_context;
		m_license_message_callback = o.m_license_message_callback;
	}
	
	if (pLicenseManager)
		pLicenseManager->AddRef();
	if (pLicenseManagerMessages)
		pLicenseManagerMessages->AddRef();
	if (pLicenseManager && pLicenseManagerMessages)
	{
		m_MessageDispatchThread = CreateThread(0, 0, MessageDispatchThreadProc, this, 0, 0);
		m_LicenseValidityThread = CreateThread(0, 0, LicenseValidityThreadProc, this, 0, 0);
	}
}

SolimarLicenseManagerWrapper::LicensingWrapper& SolimarLicenseManagerWrapper::LicensingWrapper::operator=(const LicensingWrapper &o)
{
	SafeMutex mutex(m_MemberLock);

	m_license_message_callback_context = o.m_license_message_callback_context;
	m_license_message_callback = o.m_license_message_callback;
	
	if (pLicenseManager)
	{
		pLicenseManager->Release();
		pLicenseManager = 0;
	}
	
	this->pLicenseManager = o.pLicenseManager;

	if (pLicenseManager)
		pLicenseManager->AddRef();

	if (pLicenseManagerMessages )
	{
		pLicenseManagerMessages ->Release();
		pLicenseManagerMessages  = 0;
	}
	
	pLicenseManagerMessages  = o.pLicenseManagerMessages ;

	if (pLicenseManagerMessages )
		pLicenseManagerMessages ->AddRef();
	
	return *this;
}

SolimarLicenseManagerWrapper::LicensingWrapper::~LicensingWrapper()
{
	// signal the threads to terminate
	SetEvent(m_ThreadKillEvent);
	
	// wait for the threads to terminate
	HANDLE threads[2] = {m_MessageDispatchThread, m_LicenseValidityThread};
	WaitForMultipleObjects(sizeof(threads)/sizeof(HANDLE), threads, TRUE, 1000);
	
	TerminateThread(m_MessageDispatchThread,0);
	TerminateThread(m_LicenseValidityThread,0);
	
	CloseHandle(m_MessageDispatchThread);
	CloseHandle(m_LicenseValidityThread);
	
	CloseHandle(m_ThreadKillEvent);
	CloseHandle(m_MemberLock);
	
	if (pLicenseManagerMessages)
	{
		pLicenseManagerMessages->Release();
		pLicenseManagerMessages = 0;
	}
	
	if (pLicenseManager)
	{
		pLicenseManager->Release();
		pLicenseManager = 0;
	}
}

bool SolimarLicenseManagerWrapper::LicensingWrapper::Connect(std::wstring server)
{
	HRESULT hr = S_OK;
	
	SafeMutex mutex(m_MemberLock);
	
	if (!pLicenseManager)
		return false;
	
	// try to authenticate the license manager
	hr = AuthenticateServer(pLicenseManager);
	if (FAILED(hr)) return false;
	
	// let the license manager authenticate this client
	hr = AuthenticateToServer(pLicenseManager);
	if (FAILED(hr)) return false;
	
	// make a connection to the license manager
	BSTR bstrServer = SysAllocString(server.c_str());
	hr = pLicenseManager->Connect(bstrServer);
	SysFreeString(bstrServer);
	
	return SUCCEEDED(hr);
}

bool SolimarLicenseManagerWrapper::LicensingWrapper::Initialize(long product, long prod_ver_major, long prod_ver_minor, bool single_key, std::wstring &specific_single_key_ident, bool lock_keys, DWORD ui_level)
{
	HRESULT hr = S_OK;
	
	SafeMutex mutex(m_MemberLock);
	
	if (!pLicenseManager)
		return false;
	
	BSTR bstrSpecificSingleKey = SysAllocString(specific_single_key_ident.c_str());
	hr = pLicenseManager->Initialize(product, prod_ver_major, prod_ver_minor, (single_key ? VARIANT_TRUE : VARIANT_FALSE), bstrSpecificSingleKey, (lock_keys ? VARIANT_TRUE : VARIANT_FALSE), ui_level);
	SysFreeString(bstrSpecificSingleKey);
	
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

/*
bool LicensingWrapper::GetLicenseMessageList(LicensingMessageList &message_list)
{
	HRESULT hr = S_OK;
	ILicensingMessage *pMgrMessages = 0;
	
	SafeMutex mutex(m_MemberLock);
	
	message_list.clear();
	hr = pLicenseManager->QueryInterface(__uuidof(ILicensingMessage), (void**)&pMgrMessages);
	if (FAILED(hr)) return false;
	VARIANT vtMessageList;
	VariantInit(&vtMessageList);
	hr = pMgrMessages->GetLicenseMessageList(&vtMessageList);
	message_list = vtMessageList;
	VariantClear(&vtMessageList);
	pMgrMessages->Release();
	
	return true;
}
*/

bool SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseObtain(long module, long license_count)
{
	HRESULT hr = S_OK;
	
	SafeMutex mutex(m_MemberLock);
	
	if (!pLicenseManager)
		return false;
	
	hr = pLicenseManager->ModuleLicenseObtain(module, license_count);
	
	return SUCCEEDED(hr);
}

bool SolimarLicenseManagerWrapper::LicensingWrapper::ModuleLicenseRelease(long module, long license_count)
{
	HRESULT hr = S_OK;
	
	SafeMutex mutex(m_MemberLock);
	
	if (!pLicenseManager)
		return false;
	
	hr = pLicenseManager->ModuleLicenseRelease(module, license_count);
	
	return SUCCEEDED(hr);
}

bool SolimarLicenseManagerWrapper::LicensingWrapper::ValidateLicense()
{
	HRESULT hr = S_OK;
	
	SafeMutex mutex(m_MemberLock);
	
	if (!pLicenseManager)
		return false;
	
	VARIANT_BOOL license_valid;
	hr = pLicenseManager->ValidateLicense(&license_valid);
	return SUCCEEDED(hr) && (license_valid==VARIANT_TRUE);
}

DWORD WINAPI SolimarLicenseManagerWrapper::LicensingWrapper::MessageDispatchThreadProc(LPVOID pWrapper)
{
	HRESULT hr = S_OK;
	DWORD thread_kill(0);
	
	while(1)
	{
		// check whether the thread should terminate
		if (WAIT_OBJECT_0 == WaitForSingleObject(((LicensingWrapper*)pWrapper)->m_ThreadKillEvent,0))
			break;
		
		hr = ((LicensingWrapper*)pWrapper)->MessageDispatchThreadProc();
		
		Sleep(5000);
	}

	return 0;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::MessageDispatchThreadProc()
{
	HRESULT hr = S_OK;
	
	SafeMutex mutex(m_MemberLock);
	
	LicensingMessageList message_list;
	VARIANT vtMessageList;
	VariantInit(&vtMessageList);
	hr = pLicenseManagerMessages->GetLicenseMessageList(&vtMessageList);
	if (vtMessageList.vt & VT_ARRAY)
		message_list = vtMessageList;
	VariantClear(&vtMessageList);
	if (FAILED(hr)) return hr;
	
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
	
	return 0;
}

HRESULT SolimarLicenseManagerWrapper::LicensingWrapper::LicenseValidityThreadProc()
{
	// dispatch messages
	return E_NOTIMPL;
}

std::wstring SolimarLicenseManagerWrapper::LicensingWrapper::StringToWstring(const std::string &s)
{
	size_t characters = mbstowcs(0, s.c_str(), 256);
	wchar_t* ws = new wchar_t[characters];
	ws[0]=0;
	mbstowcs(ws,s.c_str(),characters);
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
		return 0;
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
		return 0;
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
		return 0;
	}
}

long SolimarLicenseManagerWrapper::LicensingWrapper::LookupHeaderID(std::string header)
{
	return LookupHeaderID(StringToWstring(header));
}


