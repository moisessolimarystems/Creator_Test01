// This is the main DLL file.

#define _WIN32_DCOM
#define _WIN32_WINNT 0x0500

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <comutil.h>
#include <comdef.h>

#include "SolimarLicenseManagerDotNet.h"

#include "..\manager_wrapper\LicensingWrapper.h"
#include "..\common\LicenseError.h"

/*
BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    return TRUE;
}
*/
/*
struct __declspec(dllexport) A {
   A() {
      System::Console::WriteLine("Module ctor initializing based on global instance of class.\n");
   }

   void Test() {
      printf_s("Test called so linker does not throw away unused object.\n");
   }
};
*/
 
#pragma unmanaged
// Global instance of object
//A obj;
 
extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {
   // Remove all managed code from here and put it in constructor of A.
   return true;
}

//__declspec(dllexport) bool Connect(std::wstring server, bool bUseOnlySharedLicenses, bool bUseAsBackup)
//{
//	return false;
//}

#pragma managed

namespace SolimarLicenseManagerDotNet
{


	SolimarLicenseWrapper::SolimarLicenseWrapper()
	{
//OutputDebugStringW(L"SolimarLicenseWrapper::SolimarLicenseWrapper() - Enter");
		m_pLicenseWrapper = new SolimarLicenseManagerWrapper::LicensingWrapper();
		m_bRegisteredMessageCallback = false;
		m_bRegisteredInvalidLicenseCallback = false;
		m_messageCallbackUnmanaged = gcnew DelegateMessageCallbackUnmanaged(this, &SolimarLicenseWrapper::MessageCallback);
		m_invalidLicenseCallbackUnmanaged = gcnew DelegateInvalidLicenseCallbackUnmanaged(this, &SolimarLicenseWrapper::InvalidLicenseCallback);
//OutputDebugStringW(L"SolimarLicenseWrapper::SolimarLicenseWrapper() - Leave");
	}
	SolimarLicenseWrapper::~SolimarLicenseWrapper()
	{
//OutputDebugStringW(L"SolimarLicenseWrapper::~SolimarLicenseWrapper() - Enter");
		delete m_pLicenseWrapper;
		m_pLicenseWrapper = NULL;
//OutputDebugStringW(L"SolimarLicenseWrapper::~SolimarLicenseWrapper() - Leave");
	}

	bool SolimarLicenseWrapper::Connect(String^ server)
	{
		BSTR bstrServer;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(server));
		bstrServer = (static_cast<BSTR>(static_cast<void *>(ptr)));

		bool bResult = m_pLicenseWrapper->Connect(bstrServer);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		return bResult;
	}
	bool SolimarLicenseWrapper::Connect(String^ server, bool bUseOnlySharedLicenses, bool bUseAsBackup)
	{
		BSTR bstrServer;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(server));
		bstrServer = (static_cast<BSTR>(static_cast<void *>(ptr)));

		bool bResult = m_pLicenseWrapper->Connect(bstrServer, bUseOnlySharedLicenses, bUseAsBackup);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		return bResult;
	}

	bool SolimarLicenseWrapper::ConnectByProduct(long product, bool bUseSharedLicenseServers)
	{
		return m_pLicenseWrapper->ConnectByProduct(product, bUseSharedLicenseServers);
	}
	bool SolimarLicenseWrapper::Disconnect()
	{
//OutputDebugStringW(L"SolimarLicenseWrapper::Disconnect() - Enter");
		return m_pLicenseWrapper->Disconnect();
	}
	bool SolimarLicenseWrapper::Initialize(long product, long prodVerMajor, long prodVerMinor, bool singleKey, String^ specificSingleKeyIdent, bool lockKeys, LICENSE_LEVEL uiLevel, unsigned long gracePeriodMinutes)
	{
		BSTR bstrSpecificSingleKeyIdent;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(specificSingleKeyIdent));
		bstrSpecificSingleKeyIdent = (static_cast<BSTR>(static_cast<void *>(ptr)));


		bool bResult = m_pLicenseWrapper->Initialize(product, prodVerMajor, prodVerMinor, singleKey, bstrSpecificSingleKeyIdent, lockKeys, (DWORD)uiLevel, gracePeriodMinutes);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		return bResult;
	}

	bool SolimarLicenseWrapper::Initialize(String^ applicationInstance, long product, long prodVerMajor, long prodVerMinor, bool singleKey, String^ specificSingleKeyIdent, bool lockKeys, LICENSE_LEVEL uiLevel, unsigned long gracePeriodMinutes, bool applicationInstanceLockKeys, bool bypassRemoteKeyRestriction)
	{
		BSTR bstrSpecificSingleKeyIdent, bstrApplicationInstance;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(specificSingleKeyIdent));
		bstrSpecificSingleKeyIdent = (static_cast<BSTR>(static_cast<void *>(ptr)));

		System::IntPtr ptr2(System::Runtime::InteropServices::Marshal::StringToBSTR(applicationInstance));
		bstrApplicationInstance = (static_cast<BSTR>(static_cast<void *>(ptr2)));

		bool bResult = m_pLicenseWrapper->Initialize(bstrApplicationInstance, product, prodVerMajor, prodVerMinor, singleKey, bstrSpecificSingleKeyIdent, lockKeys, (DWORD)uiLevel, gracePeriodMinutes, applicationInstanceLockKeys, bypassRemoteKeyRestriction);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr2);

		return bResult;
	}

	bool SolimarLicenseWrapper::KeyProductExists(long product, long prodVerMajor, long prodVerMinor, bool% bRefKeyExists) 
	{
		bool bTmpKeyExists, bResult;
		bResult = m_pLicenseWrapper->KeyProductExists(product, prodVerMajor, prodVerMinor, &bTmpKeyExists);
		bRefKeyExists = bTmpKeyExists;
		return bResult;
	}
	bool SolimarLicenseWrapper::ModuleLicenseTotal(long module, long% refLicenseCount) 
	{
		long tmpLicenseCount;
		bool bResult;
		bResult = m_pLicenseWrapper->ModuleLicenseTotal(module, &tmpLicenseCount);
		refLicenseCount = tmpLicenseCount;
		return bResult;
	}
	bool SolimarLicenseWrapper::ModuleLicenseInUse(long module, long% refLicenseCount) 
	{
		long tmpLicenseCount;
		bool bResult;
		bResult = m_pLicenseWrapper->ModuleLicenseInUse(module, &tmpLicenseCount);
		refLicenseCount = tmpLicenseCount;
		return bResult;
	}
	bool SolimarLicenseWrapper::ModuleLicenseObtain(long module, long license_count) 
	{
		return m_pLicenseWrapper->ModuleLicenseObtain(module, license_count);
	}
	bool SolimarLicenseWrapper::ModuleLicenseRelease(long module, long license_count) 
	{
		return m_pLicenseWrapper->ModuleLicenseRelease(module, license_count);
	}
	bool SolimarLicenseWrapper::ModuleLicenseCounterDecrement(long module, long license_count) 
	{
		return m_pLicenseWrapper->ModuleLicenseCounterDecrement(module, license_count);
	}
	bool SolimarLicenseWrapper::ValidateLicense()
	{
		return m_pLicenseWrapper->ValidateLicense();
	}
	bool SolimarLicenseWrapper::GetVersionLicenseManager(long% refVerMajor, long% refVerMinor, long% refVerBuild) 
	{
		long tmpVerMajor, tmpVerMinor, tmpVerBuild;
		bool bResult = m_pLicenseWrapper->GetVersionLicenseManager(&tmpVerMajor, &tmpVerMinor, &tmpVerBuild);
		refVerMajor = tmpVerMajor;
		refVerMinor = tmpVerMinor;
		refVerBuild = tmpVerBuild;
		return bResult;
	}
	bool SolimarLicenseWrapper::GetVersionLicenseServer(String^ server, long% refVerMajor, long% refVerMinor, long% refVerBuild) 
	{
		BSTR bstrServer;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(server));
		bstrServer = (static_cast<BSTR>(static_cast<void *>(ptr)));

		//bool bResult = m_pLicenseWrapper->GetVersionLicenseServer(bstrServer, p_ver_major, p_ver_minor, p_ver_build);
		long tmpVerMajor, tmpVerMinor, tmpVerBuild;
		bool bResult = m_pLicenseWrapper->GetVersionLicenseServer(bstrServer, &tmpVerMajor, &tmpVerMinor, &tmpVerBuild);
		refVerMajor = tmpVerMajor;
		refVerMinor = tmpVerMinor;
		refVerBuild = tmpVerBuild;
		return bResult;

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		return bResult;
	}

	



	HRESULT SolimarLicenseWrapper::ConnectEx(String^ server) 
	{
		BSTR bstrServer;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(server));
		bstrServer = (static_cast<BSTR>(static_cast<void *>(ptr)));

		HRESULT hrResult = m_pLicenseWrapper->ConnectEx(bstrServer);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		return hrResult;
	}
	HRESULT SolimarLicenseWrapper::ConnectEx(String^ server, bool bUseOnlySharedLicenses, bool bUseAsBackup)
	{
		BSTR bstrServer;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(server));
		bstrServer = (static_cast<BSTR>(static_cast<void *>(ptr)));

		HRESULT hrResult = m_pLicenseWrapper->ConnectEx(bstrServer, bUseOnlySharedLicenses, bUseAsBackup);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		return hrResult;
	}
	HRESULT SolimarLicenseWrapper::ConnectByProductEx(long product, bool bUseSharedLicenseServers)
	{
		return m_pLicenseWrapper->ConnectByProductEx(product, bUseSharedLicenseServers);
	}
	HRESULT SolimarLicenseWrapper::DisconnectEx()
	{
		return m_pLicenseWrapper->DisconnectEx();
	}

	HRESULT SolimarLicenseWrapper::InitializeEx(long product, long prodVerMajor, long prodVerMinor, bool singleKey, String^ specificSingleKeyIdent, bool lockKeys, LICENSE_LEVEL uiLevel, unsigned long gracePeriodMinutes)
	{
		BSTR bstrSpecificSingleKeyIdent;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(specificSingleKeyIdent));
		bstrSpecificSingleKeyIdent = (static_cast<BSTR>(static_cast<void *>(ptr)));


		HRESULT hrResult = m_pLicenseWrapper->InitializeEx(product, prodVerMajor, prodVerMinor, singleKey, bstrSpecificSingleKeyIdent, lockKeys, (DWORD)uiLevel, gracePeriodMinutes);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		return hrResult;
	}

	HRESULT SolimarLicenseWrapper::InitializeEx(String^ applicationInstance, long product, long prodVerMajor, long prodVerMinor, bool singleKey, String^ specificSingleKeyIdent, bool lockKeys, LICENSE_LEVEL uiLevel, unsigned long gracePeriodMinutes, bool applicationInstanceLockKeys, bool bypassRemoteKeyRestriction)
	{
		BSTR bstrSpecificSingleKeyIdent, bstrApplicationInstance;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(specificSingleKeyIdent));
		bstrSpecificSingleKeyIdent = (static_cast<BSTR>(static_cast<void *>(ptr)));

		System::IntPtr ptr2(System::Runtime::InteropServices::Marshal::StringToBSTR(applicationInstance));
		bstrApplicationInstance = (static_cast<BSTR>(static_cast<void *>(ptr2)));

		HRESULT hrResult = m_pLicenseWrapper->InitializeEx(bstrApplicationInstance, product, prodVerMajor, prodVerMinor, singleKey, bstrSpecificSingleKeyIdent, lockKeys, (DWORD)uiLevel, gracePeriodMinutes, applicationInstanceLockKeys, bypassRemoteKeyRestriction);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr2);

		return hrResult;
	}

	HRESULT SolimarLicenseWrapper::KeyProductExistsEx(int product, int prodVerMajor, int prodVerMinor, bool% bRefKeyExists)
	{
		bool bTmpKeyExists;
		HRESULT hrResult = m_pLicenseWrapper->KeyProductExistsEx(product, prodVerMajor, prodVerMinor, &bTmpKeyExists);
		bRefKeyExists = bTmpKeyExists;
		return hrResult;
	}
	HRESULT SolimarLicenseWrapper::ModuleLicenseTotalEx(long module, long% refLicenseCount)
	{
		long tmpLicenseCount;
		HRESULT hrResult = m_pLicenseWrapper->ModuleLicenseTotalEx(module, &tmpLicenseCount);
		refLicenseCount = tmpLicenseCount;
		return hrResult;
	}
	HRESULT SolimarLicenseWrapper::ModuleLicenseInUseEx(long module, long% refLicenseCount)
	{
		long tmpLicenseCount;
		HRESULT hrResult = m_pLicenseWrapper->ModuleLicenseInUseEx(module, &tmpLicenseCount);
		refLicenseCount = tmpLicenseCount;
		return hrResult;

	}
	HRESULT SolimarLicenseWrapper::ModuleLicenseObtainEx(long module, long licenseCount)
	{
		return m_pLicenseWrapper->ModuleLicenseObtainEx(module, licenseCount);
	}
	HRESULT SolimarLicenseWrapper::ModuleLicenseReleaseEx(long module, long licenseCount)
	{
		return m_pLicenseWrapper->ModuleLicenseReleaseEx(module, licenseCount);
	}
	HRESULT SolimarLicenseWrapper::ModuleLicenseCounterDecrementEx(long module, long licenseCount)
	{
		return m_pLicenseWrapper->ModuleLicenseCounterDecrementEx(module, licenseCount);
	}
	HRESULT SolimarLicenseWrapper::ValidateLicenseEx(bool% bRefValid)
	{
		bool bTmpValid;
		HRESULT hrResult = m_pLicenseWrapper->ValidateLicenseEx(&bTmpValid);
		bRefValid = bTmpValid;
		return hrResult;
	}
	HRESULT SolimarLicenseWrapper::GetVersionLicenseManagerEx(long% refVerMajor, long% refVerMinor, long% refVerBuild)
	{
		long tmpVerMajor, tmpVerMinor, tmpVerBuild;
		HRESULT hrResult = m_pLicenseWrapper->GetVersionLicenseManagerEx(&tmpVerMajor, &tmpVerMinor, &tmpVerBuild);
		refVerMajor = tmpVerMajor;
		refVerMinor = tmpVerMinor;
		refVerBuild = tmpVerBuild;
		return hrResult;
	}
	HRESULT SolimarLicenseWrapper::GetVersionLicenseServerEx(String^ server, long% refVerMajor, long% refVerMinor, long% refVerBuild)
	{
		BSTR bstrServer;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(server));
		bstrServer = (static_cast<BSTR>(static_cast<void *>(ptr)));

		long tmpVerMajor, tmpVerMinor, tmpVerBuild;
		HRESULT hrResult = m_pLicenseWrapper->GetVersionLicenseServerEx(bstrServer, &tmpVerMajor, &tmpVerMinor, &tmpVerBuild);
		refVerMajor = tmpVerMajor;
		refVerMinor = tmpVerMinor;
		refVerBuild = tmpVerBuild;
		return hrResult;

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		return hrResult;
	}


	long SolimarLicenseWrapper::LookupProductID(String^ product)
	{
		BSTR bstrProduct;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(product));
		bstrProduct = (static_cast<BSTR>(static_cast<void *>(ptr)));

		long lResult = m_pLicenseWrapper->LookupProductID(bstrProduct);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		return lResult;

	}
	long SolimarLicenseWrapper::LookupModuleID(long productId, String^ module)
	{
		BSTR bstrModule;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(module));
		bstrModule = (static_cast<BSTR>(static_cast<void *>(ptr)));

		long lResult = m_pLicenseWrapper->LookupModuleID(productId, bstrModule);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		return lResult;
	}

	long SolimarLicenseWrapper::LookupHeaderID(String^ header)
	{
		BSTR bstrHeadere;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(header));
		bstrHeadere = (static_cast<BSTR>(static_cast<void *>(ptr)));

		long lResult = m_pLicenseWrapper->LookupHeaderID(bstrHeadere);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		return lResult;
	}





	bool SolimarLicenseWrapper::RegisterMessageCallback(IntPtr pContext, DelegateMessageCallbackManaged^ licenseMessageCallback)
	{
		if(!m_bRegisteredMessageCallback)	//Uninitialize
		{
			m_messageCallbackManaged = licenseMessageCallback;

			pin_ptr<DelegateMessageCallbackUnmanaged^> tmpPinPtr = &m_messageCallbackUnmanaged;
			m_pLicenseWrapper->RegisterMessageCallback(
					(void*)m_pLicenseWrapper, 
					(SolimarLicenseManagerWrapper::LicensingWrapper::LicenseMessageCallbackPtr) System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(m_messageCallbackUnmanaged).ToPointer());

			m_bRegisteredMessageCallback = true;
		}
		else	//Already Initialized
		{
			//Add to multi-cast delegate...
			m_messageCallbackManaged = static_cast<DelegateMessageCallbackManaged^> (Delegate::Combine(m_messageCallbackManaged, licenseMessageCallback));
		}
		
		return true;
	}
	bool SolimarLicenseWrapper::UnregisterMessageCallback()
	{
		m_bRegisteredMessageCallback = false;
		Delegate::RemoveAll(m_messageCallbackManaged, m_messageCallbackManaged);
		return m_pLicenseWrapper->UnregisterMessageCallback();
	}

	void SolimarLicenseWrapper::MessageCallback(void* pContext, const wchar_t* key_ident, unsigned int message_type, HRESULT error, VARIANT *pvtTimestamp, const wchar_t* message)
	{
		//Invoke to call all delegates...

		//Convert from UTC time to Local time.
		SYSTEMTIME st;
		FILETIME ft, dft;
		LARGE_INTEGER li;  
		VariantTimeToSystemTime(pvtTimestamp->date,&st);
		SystemTimeToFileTime(&st, &ft);
		FileTimeToLocalFileTime(&ft, &dft);
		li.LowPart = dft.dwLowDateTime;
		li.HighPart = dft.dwHighDateTime;    
		DateTime utc_t = System::DateTime::FromFileTimeUtc(li.QuadPart);

		m_messageCallbackManaged->Invoke((System::IntPtr)pContext, %String(key_ident), message_type, error, %utc_t, %String(message));
	}



	bool SolimarLicenseWrapper::RegisterInvalidLicenseCallback(IntPtr pContext, DelegateInvalidLicenseCallbackManaged^ licenseInvalidCallback)
	{
		if(!m_bRegisteredInvalidLicenseCallback)	//Uninitialize
		{
			m_invalidLicenseCallbackManaged = licenseInvalidCallback;

			pin_ptr<DelegateInvalidLicenseCallbackUnmanaged^> tmpPinPtr = &m_invalidLicenseCallbackUnmanaged;
			m_pLicenseWrapper->RegisterInvalidLicenseCallback(
					(void*)m_pLicenseWrapper, 
					(SolimarLicenseManagerWrapper::LicensingWrapper::LicenseInvalidCallbackPtr) System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(m_invalidLicenseCallbackUnmanaged).ToPointer());

			m_bRegisteredInvalidLicenseCallback = true;
		}
		else	//Already Initialized
		{
			//Add to multi-cast delegate...
			m_invalidLicenseCallbackManaged = static_cast<DelegateInvalidLicenseCallbackManaged^> (Delegate::Combine(m_invalidLicenseCallbackManaged, licenseInvalidCallback));
		}
		
		return true;
	}
	bool SolimarLicenseWrapper::UnregisterInvalidLicenseCallback()
	{
		m_bRegisteredInvalidLicenseCallback = false;
		Delegate::RemoveAll(m_invalidLicenseCallbackManaged, m_invalidLicenseCallbackManaged);
		return m_pLicenseWrapper->UnregisterInvalidLicenseCallback();
	}

	void SolimarLicenseWrapper::InvalidLicenseCallback(void* pContext, HRESULT hr, const wchar_t* message)
	{
		//Invoke to call all delegates...
		m_invalidLicenseCallbackManaged->Invoke((System::IntPtr)pContext, hr, %String(message));
	}
}