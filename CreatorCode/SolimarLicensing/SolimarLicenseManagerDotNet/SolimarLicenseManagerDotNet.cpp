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

#include "..\Common\LicAttribsCPP\Lic_GenericAttribs.h"

#include "..\manager_wrapper\LicensingWrapper.h"
#include "..\common\LicenseError.h"
#include "..\common\MultidimensionalSafeArray.h"
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

//namespace SolimarLicenseManagerDotNet
namespace Solimar{	namespace Licensing{		namespace LicenseManagerWrapper
{


	SolimarLicenseWrapper::SolimarLicenseWrapper()
	{
//OutputDebugStringW(L"SolimarLicenseWrapper::SolimarLicenseWrapper() - Enter");
		m_pLicenseWrapper = new SolimarLicenseManagerWrapper::LicensingWrapper();
		m_bRegisteredMessageCallback = false;
		m_bRegisteredInvalidLicenseCallback = false;
		m_messageCallbackUnmanaged = gcnew DelegateMessageCallbackUnmanaged(this, &SolimarLicenseWrapper::MessageCallback);
		m_invalidLicenseCallbackUnmanaged = gcnew DelegateInvalidLicenseCallbackUnmanaged(this, &SolimarLicenseWrapper::InvalidLicenseCallback);
		
		m_bFinalized = false;

		m_bUseSharedLicenseServers = false;
		m_productID = -1;
		m_licServer = "";
//OutputDebugStringW(L"SolimarLicenseWrapper::SolimarLicenseWrapper() - Leave");

	}
	SolimarLicenseWrapper::~SolimarLicenseWrapper()
	{
//OutputDebugStringW(L"SolimarLicenseWrapper::~SolimarLicenseWrapper() - Enter");
		m_messageCallbackUnmanaged = nullptr;
		m_invalidLicenseCallbackUnmanaged = nullptr;
		if(!m_bFinalized)
		{
			this->!SolimarLicenseWrapper();
		}
//OutputDebugStringW(L"SolimarLicenseWrapper::~SolimarLicenseWrapper() - Leave");
	}
	SolimarLicenseWrapper::!SolimarLicenseWrapper()
	{
//OutputDebugStringW(L"SolimarLicenseWrapper::!SolimarLicenseWrapper() - Enter");
		try
		{
			if(!m_bFinalized)
			{
				delete m_pLicenseWrapper;
				m_pLicenseWrapper = NULL;
				m_bFinalized = true;
			}
		}
		catch(...)
		{
//OutputDebugStringW(L"SolimarLicenseWrapper::!SolimarLicenseWrapper() - Exception");
		}
//OutputDebugStringW(L"SolimarLicenseWrapper::!SolimarLicenseWrapper() - Leave");
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
	bool SolimarLicenseWrapper::ModuleLicenseInUse_ByApp(long module, long% refLicenseCount)
	{
		long tmpLicenseCount;
		bool bResult;
		bResult = m_pLicenseWrapper->ModuleLicenseInUse_ByApp(module, &tmpLicenseCount);
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

	



	void SolimarLicenseWrapper::ConnectEx(String^ server)
	{
		BSTR bstrServer;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(server));
		bstrServer = (static_cast<BSTR>(static_cast<void *>(ptr)));

		m_licServer = gcnew String(bstrServer);
		HRESULT hrResult = m_pLicenseWrapper->ConnectEx(bstrServer);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
			//throw GenerateException(hrResult);

		return ;
	}
	void SolimarLicenseWrapper::ConnectEx(String^ server, bool bUseOnlySharedLicenses, bool bUseAsBackup)
	{
		BSTR bstrServer;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(server));
		bstrServer = (static_cast<BSTR>(static_cast<void *>(ptr)));

		m_licServer = gcnew String(bstrServer);
		HRESULT hrResult = m_pLicenseWrapper->ConnectEx(bstrServer, bUseOnlySharedLicenses, bUseAsBackup);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
			//throw GenerateException(hrResult);

		return ;
	}
	void SolimarLicenseWrapper::ConnectLicenseViewerEx(String^ server)
	{
		BSTR bstrServer;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(server));
		bstrServer = (static_cast<BSTR>(static_cast<void *>(ptr)));

		m_licServer = gcnew String(bstrServer);
		HRESULT hrResult = m_pLicenseWrapper->ConnectEx(bstrServer, false, false, true);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
			//throw GenerateException(hrResult);

		return ;
	}
	void SolimarLicenseWrapper::ConnectByProductEx(long product, bool bUseSharedLicenseServers)
	{
		m_productID = product;
		m_bUseSharedLicenseServers = bUseSharedLicenseServers;
		HRESULT hrResult = m_pLicenseWrapper->ConnectByProductEx(product, bUseSharedLicenseServers);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
			//throw GenerateException(hrResult);

		return ;
	}

	//private
	System::Runtime::InteropServices::COMException^ SolimarLicenseWrapper::GenerateException(HRESULT _hr)
	{
		System::Text::StringBuilder^ errorHeader = gcnew System::Text::StringBuilder();

		//String^ errorHeader = "";
		bool bDisplayHr = true;
		try
		{
			String^ refLicServer = "";
			String^ refBackupLicServer = "";
			bool refBUseTestDev = false;
			if(m_productID != -1)
				GetInfoByProductEx(m_productID, m_bUseSharedLicenseServers, refLicServer, refBackupLicServer, refBUseTestDev);
			else if(m_licServer->Length != 0)
				refLicServer = m_licServer;

			if(refLicServer->Length == 0)
				refLicServer = "localhost";

			String^ tmpVersion = "";
			String^ tmpException = "";
			try
			{
				tmpVersion = GetLicenseServerVersion(refLicServer);
			}
			catch(System::Runtime::InteropServices::COMException^ comEx)
			{
				if((bDisplayHr == true) && comEx->ErrorCode == _hr)
					bDisplayHr = false;
				tmpException = gcnew String(LicenseServerError::GetErrorMessage(comEx->ErrorCode).c_str());
			}

			errorHeader->Append(String::Format("License Server: {0}, {1}: {2}. \r\n", refLicServer, (tmpVersion->Length != 0) ? "Version" : "Error", (tmpVersion->Length != 0) ? tmpVersion : tmpException));
			if(refBackupLicServer->Length != 0)
			{
				tmpVersion = "";
				tmpException = "";
				try
				{
					tmpVersion = GetLicenseServerVersion(refBackupLicServer);
				}
				catch(System::Runtime::InteropServices::COMException^ comEx)
				{
					if((bDisplayHr == true) && comEx->ErrorCode == _hr)
						bDisplayHr = false;
					tmpException = gcnew String(LicenseServerError::GetErrorMessage(comEx->ErrorCode).c_str());
				}

				errorHeader->Append(String::Format("Backup License Server: {0}, {1}: {2}. \r\n", refBackupLicServer, (tmpVersion->Length != 0) ? "Version" : "Error", (tmpVersion->Length != 0) ? tmpVersion : tmpException));
			}
			errorHeader->Append(String::Format("License Manager: localhost, Version: {0}. \r\n", GetLicenseManagerVersion()));
			if(refBUseTestDev == true)
				errorHeader->Append("Use Test/Dev Licensing: true. \r\n");
		}
		catch(...){}

		if(bDisplayHr==true)
			errorHeader->Append("Error: " + gcnew String(LicenseServerError::GetErrorMessage(_hr).c_str()));

		return gcnew System::Runtime::InteropServices::COMException(errorHeader->ToString(), _hr);
	}

	//private
	String^ SolimarLicenseWrapper::GetLicenseServerVersion(String^ _server)
	{
		long lMajor = 0, lMinor = 0, lBuild = 0;
		SolimarLicenseWrapper^ pLicWrapper = gcnew SolimarLicenseWrapper();
		pLicWrapper->ConnectEx(L"localhost");
		pLicWrapper->GetVersionLicenseServerEx(_server, lMajor, lMinor, lBuild);
		return String::Format("{0}.{1}.{2}", lMajor, lMinor, lBuild);
	}

	//private
	String^ SolimarLicenseWrapper::GetLicenseManagerVersion()
	{
		long lMajor = 0, lMinor = 0, lBuild = 0;
		SolimarLicenseWrapper^ pLicWrapper = gcnew SolimarLicenseWrapper();
		pLicWrapper->ConnectEx(L"localhost");
		pLicWrapper->GetVersionLicenseManagerEx(lMajor, lMinor, lBuild);
		return String::Format("{0}.{1}.{2}", lMajor, lMinor, lBuild);
	}


	void SolimarLicenseWrapper::GetInfoByProductEx(long product, bool bUseSharedLicenseServers, String^% refLicServer, String^% refBackupLicServer, bool% refBUseTestDev)
	{
		BSTR bstrRefLicServer, bstrRefBackupLicServer;
		bool bRefTestDev;
		HRESULT hrResult = m_pLicenseWrapper->GetInfoByProductEx(product, &bstrRefLicServer, &bstrRefBackupLicServer, &bRefTestDev, bUseSharedLicenseServers);

		if(SUCCEEDED(hrResult))
		{
			refBUseTestDev = bRefTestDev;
			refLicServer = gcnew String(bstrRefLicServer);
			refBackupLicServer = gcnew String(bstrRefBackupLicServer);
		}
		SysFreeString(bstrRefLicServer);
		SysFreeString(bstrRefBackupLicServer);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
		return ;
	}
	void SolimarLicenseWrapper::DisconnectEx()
	{
		HRESULT hrResult = m_pLicenseWrapper->DisconnectEx();
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
		return ;
	}


	// Returns true if there is any licensing for the given products, else false.
	bool SolimarLicenseWrapper::InitializeEx(long product, long prodVerMajor, long prodVerMinor, bool singleKey, String^ specificSingleKeyIdent, bool lockKeys, LICENSE_LEVEL uiLevel, unsigned long gracePeriodMinutes)
	{
		
		BSTR bstrSpecificSingleKeyIdent;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(specificSingleKeyIdent));
		bstrSpecificSingleKeyIdent = (static_cast<BSTR>(static_cast<void *>(ptr)));


		HRESULT hrResult = m_pLicenseWrapper->InitializeEx(product, prodVerMajor, prodVerMinor, singleKey, bstrSpecificSingleKeyIdent, lockKeys, (DWORD)uiLevel, gracePeriodMinutes);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		//No licensing on license server if hrResult==S_FALSE
		return (hrResult == S_OK) ? true : false;
	}

	// Returns true if there is any licensing for the given products, else false.
	bool SolimarLicenseWrapper::InitializeEx(String^ applicationInstance, long product, long prodVerMajor, long prodVerMinor, bool singleKey, String^ specificSingleKeyIdent, bool lockKeys, LICENSE_LEVEL uiLevel, unsigned long gracePeriodMinutes, bool applicationInstanceLockKeys, bool bypassRemoteKeyRestriction)
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

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		//No licensing on license server if hrResult==S_FALSE
		return (hrResult == S_OK) ? true : false;
	}

	bool SolimarLicenseWrapper::InitializeViewOnly(String^ applicationInstance, long product, long prodVerMajor, long prodVerMinor)
	{
		BSTR bstrApplicationInstance;
		//convert the string* to a BSTR
		System::IntPtr ptr2(System::Runtime::InteropServices::Marshal::StringToBSTR(applicationInstance));
		bstrApplicationInstance = (static_cast<BSTR>(static_cast<void *>(ptr2)));

		HRESULT hrResult = m_pLicenseWrapper->InitializeViewOnly(bstrApplicationInstance, product, prodVerMajor, prodVerMinor);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr2);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		//No licensing on license server if hrResult==S_FALSE
		return (hrResult == S_OK) ? true : false;
	}
	bool SolimarLicenseWrapper::KeyProductExistsEx(int product, int prodVerMajor, int prodVerMinor)
	{
		bool bRetValid(false);
		HRESULT hrResult = m_pLicenseWrapper->KeyProductExistsEx(product, prodVerMajor, prodVerMinor, &bRetValid);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return bRetValid;
	}
	long SolimarLicenseWrapper::ModuleLicenseTotalEx(long module)
	{
		long retLicenseCount;
		HRESULT hrResult = m_pLicenseWrapper->ModuleLicenseTotalEx(module, &retLicenseCount);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
		return retLicenseCount;
	}
	long SolimarLicenseWrapper::ModuleLicenseInUseEx(long module)
	{
		long retLicenseCount;
		HRESULT hrResult = m_pLicenseWrapper->ModuleLicenseInUseEx(module, &retLicenseCount);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
		return retLicenseCount;
	}
	long SolimarLicenseWrapper::ModuleLicenseInUse_ByAppEx(long module)
	{
		long retLicenseCount;
		HRESULT hrResult = m_pLicenseWrapper->ModuleLicenseInUse_ByAppEx(module, &retLicenseCount);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
		return retLicenseCount;
	}
	void SolimarLicenseWrapper::ModuleLicenseObtainEx(long module, long licenseCount)
	{
		HRESULT hrResult = m_pLicenseWrapper->ModuleLicenseObtainEx(module, licenseCount);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
		return ;
	}
	void SolimarLicenseWrapper::ModuleLicenseReleaseEx(long module, long licenseCount)
	{
		HRESULT hrResult = m_pLicenseWrapper->ModuleLicenseReleaseEx(module, licenseCount);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
		return ;
	}
	void SolimarLicenseWrapper::ModuleLicenseCounterDecrementEx(long module, long licenseCount)
	{
		HRESULT hrResult = m_pLicenseWrapper->ModuleLicenseCounterDecrementEx(module, licenseCount);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
		return ;
	}
	bool SolimarLicenseWrapper::ValidateLicenseEx()
	{
		bool bRetValid;
		HRESULT hrResult = m_pLicenseWrapper->ValidateLicenseEx(&bRetValid);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
		return bRetValid;
	}
	void SolimarLicenseWrapper::GetVersionLicenseManagerEx(long% refVerMajor, long% refVerMinor, long% refVerBuild)
	{
		long tmpVerMajor, tmpVerMinor, tmpVerBuild;
		HRESULT hrResult = m_pLicenseWrapper->GetVersionLicenseManagerEx(&tmpVerMajor, &tmpVerMinor, &tmpVerBuild);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
		refVerMajor = tmpVerMajor;
		refVerMinor = tmpVerMinor;
		refVerBuild = tmpVerBuild;
		return ;
	}
	void SolimarLicenseWrapper::GetVersionLicenseServerEx(String^ server, long% refVerMajor, long% refVerMinor, long% refVerBuild)
	{
		BSTR bstrServer;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(server));
		bstrServer = (static_cast<BSTR>(static_cast<void *>(ptr)));

		long tmpVerMajor, tmpVerMinor, tmpVerBuild;
		HRESULT hrResult = m_pLicenseWrapper->GetVersionLicenseServerEx(bstrServer, &tmpVerMajor, &tmpVerMinor, &tmpVerBuild);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		refVerMajor = tmpVerMajor;
		refVerMinor = tmpVerMinor;
		refVerBuild = tmpVerBuild;

		return ;
	}



	long SolimarLicenseWrapper::StartLicensingSessionEx()
	{
		long retNewSessionID;
		HRESULT hrResult = m_pLicenseWrapper->StartLicensingSessionEx(&retNewSessionID);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
		return retNewSessionID;
	}
	void SolimarLicenseWrapper::ModuleLicenseObtainLicensingSessionEx(long sessionID, long module, long licenseCount)
	{
		HRESULT hrResult = m_pLicenseWrapper->ModuleLicenseObtainLicensingSessionEx(sessionID, module, licenseCount);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
		return ;
	}
	void SolimarLicenseWrapper::ModuleLicenseReleaseLicensingSessionEx(long sessionID, long module, long licenseCount)
	{
		HRESULT hrResult = m_pLicenseWrapper->ModuleLicenseReleaseLicensingSessionEx(sessionID, module, licenseCount);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
		return ;
	}
	long SolimarLicenseWrapper::ModuleLicenseInUseLicensingSessionEx(long sessionID, long module)
	{
		long retLicenseCount;
		HRESULT hrResult = m_pLicenseWrapper->ModuleLicenseInUseLicensingSessionEx(sessionID, module, &retLicenseCount);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
		return retLicenseCount;
	}
	void SolimarLicenseWrapper::EndLicensingSessionEx(long sessionID)
	{
		HRESULT hrResult = m_pLicenseWrapper->EndLicensingSessionEx(sessionID);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
		return ;
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
			m_pLicenseWrapper->RegisterMessageCallback(
					(void*)m_pLicenseWrapper, 
					static_cast<SolimarLicenseManagerWrapper::LicensingWrapper::LicenseMessageCallbackPtr> (System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(m_messageCallbackUnmanaged).ToPointer()));

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
//OutputDebugString(L"SolimarLicenseWrapper::MessageCallback() - Enter");
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
//OutputDebugString(L"SolimarLicenseWrapper::MessageCallback() - Leave");
	}



	bool SolimarLicenseWrapper::RegisterInvalidLicenseCallback(IntPtr pContext, DelegateInvalidLicenseCallbackManaged^ licenseInvalidCallback)
	{
		if(!m_bRegisteredInvalidLicenseCallback)	//Uninitialize
		{
			m_invalidLicenseCallbackManaged = licenseInvalidCallback;

			m_pLicenseWrapper->RegisterInvalidLicenseCallback(
					(void*)m_pLicenseWrapper, 
					static_cast<SolimarLicenseManagerWrapper::LicensingWrapper::LicenseInvalidCallbackPtr> (System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(m_invalidLicenseCallbackUnmanaged).ToPointer()));

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
//OutputDebugString(L"SolimarLicenseWrapper::InvalidLicenseCallback() - Enter");
		//Invoke to call all delegates...
		m_invalidLicenseCallbackManaged->Invoke((System::IntPtr)pContext, hr, %String(message));
//OutputDebugString(L"SolimarLicenseWrapper::InvalidLicenseCallback() - Leave");
	}



	//
	// Class SolimarLicenseServerWrapper
	//

	SolimarLicenseServerWrapper::SolimarLicenseServerWrapper()
	{
		m_pLicenseServerWrapper = new SolimarLicenseManagerWrapper::LicensingServerWrapper();
	}

	// destructor cleans up all resources
	SolimarLicenseServerWrapper::~SolimarLicenseServerWrapper()
	{
		// clean up code to release managed resource
      // ...
      // to avoid code duplication 
      // call finalizer to release unmanaged resources
		delete m_pLicenseServerWrapper;
		//this->!SolimarLicenseServerWrapper();
	}

	// finalizer cleans up unmanaged resources
   // destructor or garbage collector will clean up managed resources
	//SolimarLicenseServerWrapper::!SolimarLicenseServerWrapper()
	//{
	//	// clean up code to release unmanaged resource
	//	delete m_pLicenseServerWrapper;
	//}


	


	void SolimarLicenseServerWrapper::Connect(String^ server)
	{
		BSTR bstrServer;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(server));
		bstrServer = (static_cast<BSTR>(static_cast<void *>(ptr)));

		HRESULT hrResult = m_pLicenseServerWrapper->Connect(bstrServer);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

 		if(FAILED(hrResult))
		{
			//throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
			//LicenseServerError::GetErrorMessage(hrResult)

			String^ errorMessage = "";
			long lmMajor = 0, lmMinor = 0, lmBuild = 0;
			long lsMajor = 0, lsMinor = 0, lsBuild = 0;
			try
			{
				if ((unsigned int)hrResult == 0x80004002)
				{
					SolimarLicenseWrapper^ pLicWrapper = gcnew SolimarLicenseWrapper();
					pLicWrapper->ConnectEx(L"localhost");
					pLicWrapper->GetVersionLicenseManagerEx(lmMajor, lmMinor, lmBuild);

					pLicWrapper->GetVersionLicenseServerEx(server, lsMajor, lsMinor, lsBuild);
					if (lmMajor >= 3 && lsMajor < 3)
					{
						errorMessage = String::Format("License Server: {0} - License Server must be atleast 3.0.0\r\nLicense Server on {0} is at version {1}.{2}.{3}", server, lsMajor, lsMinor, lsBuild);
						throw gcnew System::Runtime::InteropServices::COMException(errorMessage);
					}
				}
				else
					throw hrResult;
			}
			catch (HRESULT &ehr)
			{
				throw gcnew System::Runtime::InteropServices::COMException(String::Format("License Server: {0} - ", server) + gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
			}
		}

		return ;
	}
	void SolimarLicenseServerWrapper::Disconnect()
	{
		HRESULT hrResult = m_pLicenseServerWrapper->Disconnect();
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
		return ;
	}
	long SolimarLicenseServerWrapper::SoftwareModuleLicenseTotalForAll_ByProduct(long productID, long moduleIdent)
	{
		long retLicenseTotal(0);
		HRESULT hrResult = m_pLicenseServerWrapper->SoftwareModuleLicenseTotalForAll_ByProduct(productID, moduleIdent, &retLicenseTotal);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return retLicenseTotal;
	}
	long SolimarLicenseServerWrapper::SoftwareModuleLicenseInUseForAll_ByProduct(long productID, long moduleIdent)
	{
		long retLicenseTotal(0);
		HRESULT hrResult = m_pLicenseServerWrapper->SoftwareModuleLicenseInUseForAll_ByProduct(productID, moduleIdent, &retLicenseTotal);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return retLicenseTotal;	
	}


	//returns productInfoAttribsStream
	String^ SolimarLicenseServerWrapper::GetSoftwareLicenseInfoByProduct_ForAll(long productID)
	{
		String^ retProductInfoAttribsStream;
		BSTR bstrStream;
		HRESULT hrResult = m_pLicenseServerWrapper->GetSoftwareLicenseInfoByProduct_ForAll(productID, &bstrStream);
		retProductInfoAttribsStream = gcnew String(bstrStream);
		SysFreeString(bstrStream);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return retProductInfoAttribsStream;	
	}

	//returns licenseInfoAttribsStream
	String^ SolimarLicenseServerWrapper::GetSoftwareLicenseInfo_ForAll()
	{
		String^ retLicenseInfoAttribsStream;
		BSTR bstrStream;
		HRESULT hrResult = m_pLicenseServerWrapper->GetSoftwareLicenseInfo_ForAll(&bstrStream);
		retLicenseInfoAttribsStream = gcnew String(bstrStream);
		SysFreeString(bstrStream);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return retLicenseInfoAttribsStream;	
	}

	//returns licenseListStream
	String^ SolimarLicenseServerWrapper::GetAllSoftwareLicenses()
	{
		BSTR bstrStream;
		String^ retLicenseListStream;
		HRESULT hrResult = m_pLicenseServerWrapper->GetAllSoftwareLicenses(&bstrStream);
		retLicenseListStream = gcnew String(bstrStream);
		SysFreeString(bstrStream);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
		return retLicenseListStream;	
	}

	//returns productInfoAttribsStream
	String^ SolimarLicenseServerWrapper::GetSoftwareLicenseInfoByProduct_ByLicense(String^ softwareLicense, long productID)
	{
		String^ retProductInfoAttribsStream;
		BSTR bstrStream;
		BSTR bstrSoftwareLicense;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(softwareLicense));
		bstrSoftwareLicense = (static_cast<BSTR>(static_cast<void *>(ptr)));
		HRESULT hrResult = m_pLicenseServerWrapper->GetSoftwareLicenseInfoByProduct_ByLicense(bstrSoftwareLicense, productID, &bstrStream);
		retProductInfoAttribsStream = gcnew String(bstrStream);
		SysFreeString(bstrStream);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return retProductInfoAttribsStream;	
	}

	//returns String^% licenseInfoAttribsStream
	String^ SolimarLicenseServerWrapper::GetSoftwareLicenseInfo_ByLicense(String^ softwareLicense)
	{
		String^ retLicenseInfoAttribsStream;
		BSTR bstrStream;
		BSTR bstrSoftwareLicense;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(softwareLicense));
		bstrSoftwareLicense = (static_cast<BSTR>(static_cast<void *>(ptr)));
		HRESULT hrResult = m_pLicenseServerWrapper->GetSoftwareLicenseInfo_ByLicense(bstrSoftwareLicense, &bstrStream);
		retLicenseInfoAttribsStream = gcnew String(bstrStream);

		SysFreeString(bstrStream);
		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
		return retLicenseInfoAttribsStream;	
	}

	//returns map<string, string> as a stream where the first string is the sofware license name, and
	//the second string is the status
	String^ SolimarLicenseServerWrapper::GetSoftwareLicenseStatus_ByProduct(long productID)
	{
		BSTR bstrStream;
		String^ retLicenseListStream;
		HRESULT hrResult = m_pLicenseServerWrapper->GetSoftwareLicenseStatus_ByProduct(productID, &bstrStream);

		SpdAttribs::MapStringDwordObj strDwordMapObj;
		SpdAttribs::TMapStringDword strDwordMap;
		strDwordMapObj.StringToValue(bstrStream, strDwordMap);
		SysFreeString(bstrStream);

		Lic_StringStringMapAttribs licStrStrMap;
		for(	SpdAttribs::TMapStringDword::iterator strDwordMapIt = strDwordMap.begin();
				strDwordMapIt != strDwordMap.end();
				strDwordMapIt++)
		{
			licStrStrMap.strStringMap->insert(SpdAttribs::MapStringStringAttrib::TypeT::value_type(strDwordMapIt->first->c_str(), FAILED(strDwordMapIt->second) ? LicenseServerError::GetErrorMessage(strDwordMapIt->second).c_str() : L"Licensing Verified"));
		}
		retLicenseListStream = gcnew String(licStrStrMap.strStringMap.ToString().c_str());

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
		return retLicenseListStream;	
	}


	String^ SolimarLicenseServerWrapper::GetSoftwareLicenseStatus_ByLicense(String^ softwareLicense, bool% refBSucess)
	{
		BSTR bstrSoftwareLicense;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(softwareLicense));
		bstrSoftwareLicense = (static_cast<BSTR>(static_cast<void *>(ptr)));
		HRESULT hrResult = m_pLicenseServerWrapper->GetSoftwareLicenseStatus_ByLicense(bstrSoftwareLicense);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		refBSucess = FAILED(hrResult) ? false : true;
		return gcnew String(FAILED(hrResult) ? LicenseServerError::GetErrorMessage(hrResult).c_str() : L"Licensing Verified");
	}
	//throws an exception if fails to verify with reason why failure occurred
	void SolimarLicenseServerWrapper::ValidateToken_ByLicense(String^ softwareLicense, long validationTokenType, String^ validationValue)
	{
		BSTR bstrSoftwareLicense;
		BSTR bstrValidationValue;

		//convert the string* to a BSTR
		System::IntPtr ptr1(System::Runtime::InteropServices::Marshal::StringToBSTR(softwareLicense));
		bstrSoftwareLicense = (static_cast<BSTR>(static_cast<void *>(ptr1)));

		System::IntPtr ptr2(System::Runtime::InteropServices::Marshal::StringToBSTR(validationValue));
		bstrValidationValue = (static_cast<BSTR>(static_cast<void *>(ptr2)));

		HRESULT hrResult = m_pLicenseServerWrapper->ValidateToken_ByLicense(bstrSoftwareLicense, validationTokenType, bstrValidationValue);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr1);
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr2);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return ;
	}

	//HRESULT SolimarLicenseServerWrapper::GetSoftwareSpecByProduct(long productID, String^% refProductSoftwareSpecAttribs)
	//{
	//	BSTR bstrStream;
	//	HRESULT hrResult = m_pLicenseServerWrapper->GetSoftwareSpecByProduct(productID, &bstrStream);
	//	refProductSoftwareSpecAttribs = gcnew String(bstrStream);
	//	SysFreeString(bstrStream);
	//	return hrResult;	
	//}
		
	//HRESULT SolimarLicenseServerWrapper::GetSoftwareSpec(String^% refSoftwareSpecAttribsStream)
	//{
	//	BSTR bstrStream;
	//	HRESULT hrResult = m_pLicenseServerWrapper->GetSoftwareSpec(&bstrStream);
	//	refSoftwareSpecAttribsStream = gcnew String(bstrStream);
	//	SysFreeString(bstrStream);
	//	return hrResult;	
	//}


	//returns appInstListStream
	String^ SolimarLicenseServerWrapper::SoftwareGetApplicationInstanceListByProduct(long productID)
	{
		String^ retAppInstListStream;
		BSTR bstrStream;
		HRESULT hrResult = m_pLicenseServerWrapper->SoftwareGetApplicationInstanceListByProduct(productID, &bstrStream);
		retAppInstListStream = gcnew String(bstrStream);
		SysFreeString(bstrStream);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return retAppInstListStream;
	}

	void SolimarLicenseServerWrapper::GenerateSoftwareLicPacket(String^ licPackageAttribsStream, DateTime expirationDate, String^% refVerificationCode, array<Byte>^% refByteLicensePacket)
	{
		BSTR bstrVerificationStream;
		BSTR bstrPackageAttribsStream;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(licPackageAttribsStream));
		bstrPackageAttribsStream = (static_cast<BSTR>(static_cast<void *>(ptr)));

		_variant_t vtExpirationDate;
		_variant_t vtLicensePackage;
		VariantInit(&vtLicensePackage);
		VariantInit(&vtExpirationDate);
		vtExpirationDate.ChangeType(VT_DATE);
		vtExpirationDate.date = expirationDate.ToOADate();

		HRESULT hrResult = m_pLicenseServerWrapper->GenerateSoftwareLicPacket(bstrPackageAttribsStream, vtExpirationDate, &bstrVerificationStream, &vtLicensePackage);
		if(SUCCEEDED(hrResult))
		{
			//Cycle through the SafeArray returned in the variant vtLicensePackage
			if(vtLicensePackage.vt == (VT_ARRAY | VT_UI1))
			{
				BYTE *pPacket = 0;
				wchar_t *pPacketString = 0;
				hrResult = SafeArrayAccessData(vtLicensePackage.parray, (void**)&pPacket);
				if (SUCCEEDED(hrResult))
				{
					
					refByteLicensePacket = gcnew array<Byte>(vtLicensePackage.parray->rgsabound[0].cElements);
					for(unsigned int idx=0; idx<vtLicensePackage.parray->rgsabound[0].cElements; idx++)
					{
						refByteLicensePacket[idx] = pPacket[idx];
					}
					// done with the source variant
					SafeArrayUnaccessData(vtLicensePackage.parray);
				}
			}
			refVerificationCode = gcnew String(bstrVerificationStream);
			SysFreeString(bstrVerificationStream);
		}
		VariantClear(&vtExpirationDate);
		VariantClear(&vtLicensePackage);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return ;
	}
	//returns VerificationCode
	String^ SolimarLicenseServerWrapper::EnterSoftwareLicPacket(array<Byte>^ byteLicensePacket)
	{
		String^ retVerificationCode = "";;
		HRESULT hrResult = S_OK;
		BSTR bstrVerificationStream;

		_variant_t vtLicensePackage;
		VariantInit(&vtLicensePackage);

		try
		{
			// package the string in to a variant
			vtLicensePackage.vt = VT_ARRAY | VT_UI1;
			vtLicensePackage.parray = SafeArrayCreateVector(VT_UI1, 0, (byteLicensePacket->Length+1)*sizeof(wchar_t));

			BYTE *pPacket = 0;
			hrResult = SafeArrayAccessData(vtLicensePackage.parray, (void**)&pPacket);
			if (FAILED(hrResult)) throw hrResult;

			//pPacket = new BYTE[byteLicensePacket->Length+1];
			//for(int idx=0; idx<byteLicensePacket->Length; idx++)
			//	pPacket[idx] = byteLicensePacket[idx];
			//memcpy(pPacket,byteLicensePacket,byteLicensePacket->Length);

			{
			pin_ptr<Byte> pSrcPinned = &byteLicensePacket[0];
			memcpy(pPacket, pSrcPinned, (byteLicensePacket->Length+1)*sizeof(wchar_t));
			}

			SafeArrayUnaccessData(vtLicensePackage.parray);
			

			hrResult = m_pLicenseServerWrapper->EnterSoftwareLicPacket(vtLicensePackage, &bstrVerificationStream);
			if(FAILED(hrResult)) throw hrResult;
			
			retVerificationCode = gcnew String(bstrVerificationStream);
		}
		catch(HRESULT &ehr)
		{
			hrResult = ehr;
		}

		// cleanup section
		VariantClear(&vtLicensePackage);
		SysFreeString(bstrVerificationStream);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return retVerificationCode;
	}

	//returns VerificationCode
	String^ SolimarLicenseServerWrapper::EnterProtectionKeyPasswordPacket(array<Byte>^ byteLicensePacket)
	{
		String^ retVerificationCode = "";;
		HRESULT hrResult = S_OK;
		BSTR bstrVerificationStream;

		_variant_t vtLicensePackage;
		VariantInit(&vtLicensePackage);

		try
		{
			// package the string in to a variant
			vtLicensePackage.vt = VT_ARRAY | VT_UI1;
			vtLicensePackage.parray = SafeArrayCreateVector(VT_UI1, 0, (byteLicensePacket->Length+1)*sizeof(wchar_t));

			BYTE *pPacket = 0;
			hrResult = SafeArrayAccessData(vtLicensePackage.parray, (void**)&pPacket);
			if (FAILED(hrResult)) throw hrResult;

			{
			pin_ptr<Byte> pSrcPinned = &byteLicensePacket[0];
			memcpy(pPacket, pSrcPinned, (byteLicensePacket->Length+1)*sizeof(wchar_t));
			}

			SafeArrayUnaccessData(vtLicensePackage.parray);
			

			hrResult = m_pLicenseServerWrapper->EnterProtectionKeyPasswordPacket(vtLicensePackage, &bstrVerificationStream);
			if(FAILED(hrResult)) throw hrResult;
			
			retVerificationCode = gcnew String(bstrVerificationStream);
		}
		catch(HRESULT &ehr)
		{
			hrResult = ehr;
		}

		// cleanup section
		VariantClear(&vtLicensePackage);
		SysFreeString(bstrVerificationStream);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return retVerificationCode;
	}

	//returns true/false if password is applied
	bool SolimarLicenseServerWrapper::EnterProtectionKeyPassword(String^ password)
	{
		//System::Diagnostics::Trace::Write(String::Format("EnterProtectionKeyPassword() - password: {0}", password));
		BSTR bstrPassword;

		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(password));
		bstrPassword = (static_cast<BSTR>(static_cast<void *>(ptr)));

		HRESULT hrResult = m_pLicenseServerWrapper->EnterProtectionKeyPassword(bstrPassword);	//Returns S_FALSE on failure

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return (hrResult == S_OK);
	}

	//throws an exception on a failure
	void SolimarLicenseServerWrapper::GenerateSoftwareLicArchive_ByLicense(String^ softwareLicense, array<Byte>^% refByteLicenseArchive)
	{
		BSTR bstrSoftwareLicense;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(softwareLicense));
		bstrSoftwareLicense = (static_cast<BSTR>(static_cast<void *>(ptr)));

		_variant_t vtLicenseArchive;
		VariantInit(&vtLicenseArchive);
		HRESULT hrResult = m_pLicenseServerWrapper->GenerateSoftwareLicArchive_ByLicense(bstrSoftwareLicense, &vtLicenseArchive);
		if(SUCCEEDED(hrResult))
		{
			//Cycle through the SafeArray returned in the variant vtLicensePackage
			if(vtLicenseArchive.vt == (VT_ARRAY | VT_UI1))
			{
				BYTE *pPacket = 0;
				wchar_t *pPacketString = 0;
				hrResult = SafeArrayAccessData(vtLicenseArchive.parray, (void**)&pPacket);
				if (SUCCEEDED(hrResult))
				{
					refByteLicenseArchive = gcnew array<Byte>(vtLicenseArchive.parray->rgsabound[0].cElements);
					for(unsigned int idx=0; idx<vtLicenseArchive.parray->rgsabound[0].cElements; idx++)
						refByteLicenseArchive[idx] = pPacket[idx];

					// done with the source variant
					SafeArrayUnaccessData(vtLicenseArchive.parray);
				}
			}
		}
		
		VariantClear(&vtLicenseArchive);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return ;
	}
	//throws an exception on a failure
	void SolimarLicenseServerWrapper::EnterSoftwareLicArchive(array<Byte>^ byteLicenseArchive)
	{
		String^ retVerificationCode = "";;
		HRESULT hrResult = S_OK;
		_variant_t vtLicenseArchive;
		VariantInit(&vtLicenseArchive);
		try
		{
			// package the string in to a variant
			vtLicenseArchive.vt = VT_ARRAY | VT_UI1;
			vtLicenseArchive.parray = SafeArrayCreateVector(VT_UI1, 0, (byteLicenseArchive->Length+1)*sizeof(wchar_t));

			BYTE *pPacket = 0;
			hrResult = SafeArrayAccessData(vtLicenseArchive.parray, (void**)&pPacket);
			if (FAILED(hrResult)) 
				throw hrResult;

			{
			pin_ptr<Byte> pSrcPinned = &byteLicenseArchive[0];
			memcpy(pPacket, pSrcPinned, (byteLicenseArchive->Length+1)*sizeof(wchar_t));
			}

			SafeArrayUnaccessData(vtLicenseArchive.parray);
			

			hrResult = m_pLicenseServerWrapper->EnterSoftwareLicArchive(vtLicenseArchive);
			if(FAILED(hrResult)) 
				throw hrResult;
			
		}
		catch(HRESULT &ehr)
		{
			hrResult = ehr;
		}

		// cleanup section
		VariantClear(&vtLicenseArchive);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return ;
	}
	//throws an exception on a failure
	void SolimarLicenseServerWrapper::GenerateVerifyDataWithVerCode_ByLicense(String^ softwareLicense, array<Byte>^% refByteLicensePacket)
	{
		BSTR bstrSoftwareLicense;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(softwareLicense));
		bstrSoftwareLicense = (static_cast<BSTR>(static_cast<void *>(ptr)));

		_variant_t vtLicensePackage;
		VariantInit(&vtLicensePackage);
		HRESULT hrResult = m_pLicenseServerWrapper->GenerateVerifyDataWithVerCode_ByLicense(bstrSoftwareLicense, &vtLicensePackage);
		if(SUCCEEDED(hrResult))
		{
			//Cycle through the SafeArray returned in the variant vtLicensePackage
			if(vtLicensePackage.vt == (VT_ARRAY | VT_UI1))
			{
				BYTE *pPacket = 0;
				wchar_t *pPacketString = 0;
				hrResult = SafeArrayAccessData(vtLicensePackage.parray, (void**)&pPacket);
				if (SUCCEEDED(hrResult))
				{
					
					refByteLicensePacket = gcnew array<Byte>(vtLicensePackage.parray->rgsabound[0].cElements);
					for(unsigned int idx=0; idx<vtLicensePackage.parray->rgsabound[0].cElements; idx++)
					{
						refByteLicensePacket[idx] = pPacket[idx];
					}
					// done with the source variant
					SafeArrayUnaccessData(vtLicensePackage.parray);
				}
			}
		}
		VariantClear(&vtLicensePackage);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return ;
	}

	//throws an exception on a failure
	void SolimarLicenseServerWrapper::GenerateVerifyDataWithLicInfo_ByLicense(String^ softwareLicense, array<Byte>^% refByteLicensePacket)
	{
		BSTR bstrSoftwareLicense;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(softwareLicense));
		bstrSoftwareLicense = (static_cast<BSTR>(static_cast<void *>(ptr)));

		_variant_t vtLicensePackage;
		VariantInit(&vtLicensePackage);
		HRESULT hrResult = m_pLicenseServerWrapper->GenerateVerifyDataWithLicInfo_ByLicense(bstrSoftwareLicense, &vtLicensePackage);
		if(SUCCEEDED(hrResult))
		{
			//Cycle through the SafeArray returned in the variant vtLicensePackage
			if(vtLicensePackage.vt == (VT_ARRAY | VT_UI1))
			{
				BYTE *pPacket = 0;
				wchar_t *pPacketString = 0;
				hrResult = SafeArrayAccessData(vtLicensePackage.parray, (void**)&pPacket);
				if (SUCCEEDED(hrResult))
				{
					
					refByteLicensePacket = gcnew array<Byte>(vtLicensePackage.parray->rgsabound[0].cElements);
					for(unsigned int idx=0; idx<vtLicensePackage.parray->rgsabound[0].cElements; idx++)
					{
						refByteLicensePacket[idx] = pPacket[idx];
					}
					// done with the source variant
					SafeArrayUnaccessData(vtLicensePackage.parray);
				}
			}
		}
		VariantClear(&vtLicensePackage);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return ;
	}

	//throws an exception on a failure, returns licenseInfoAttribsStream on success.
	String^ SolimarLicenseServerWrapper::GenerateLicInfo_ByVerifyData(array<Byte>^ byteVerifyData)
	{
		String^ retLicenseInfoAttribsStream = "";;
		HRESULT hrResult = S_OK;
		BSTR bstrLicenseInfoAttribsStream;

		_variant_t vtVerifyData;
		VariantInit(&vtVerifyData);

		try
		{
			// package the string in to a variant
			vtVerifyData.vt = VT_ARRAY | VT_UI1;
			vtVerifyData.parray = SafeArrayCreateVector(VT_UI1, 0, (byteVerifyData->Length+1)*sizeof(wchar_t));

			BYTE *pPacket = 0;
			hrResult = SafeArrayAccessData(vtVerifyData.parray, (void**)&pPacket);
			if (FAILED(hrResult)) throw hrResult;

			//pPacket = new BYTE[byteLicensePacket->Length+1];
			//for(int idx=0; idx<byteLicensePacket->Length; idx++)
			//	pPacket[idx] = byteLicensePacket[idx];
			//memcpy(pPacket,byteLicensePacket,byteLicensePacket->Length);

			{
			pin_ptr<Byte> pSrcPinned = &byteVerifyData[0];
			memcpy(pPacket, pSrcPinned, (byteVerifyData->Length+1)*sizeof(wchar_t));
			}

			SafeArrayUnaccessData(vtVerifyData.parray);
			

			hrResult = m_pLicenseServerWrapper->GenerateLicPackage_ByVerifyData(vtVerifyData, &bstrLicenseInfoAttribsStream);
			if(FAILED(hrResult)) throw hrResult;
			
			retLicenseInfoAttribsStream = gcnew String(bstrLicenseInfoAttribsStream);
		}
		catch(HRESULT &ehr)
		{
			hrResult = ehr;
		}

		// cleanup section
		VariantClear(&vtVerifyData);
		SysFreeString(bstrLicenseInfoAttribsStream);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return retLicenseInfoAttribsStream;
	}
	//throws an exception on a failure, returns licenseInfoAttribsStream on success.
	String^ SolimarLicenseServerWrapper::GenerateLicPackage_BySoftwareLicArchive(array<Byte>^ byteLicenseArchive)
	{
		String^ retLicensePackageAttribsStream = "";;
		HRESULT hrResult = S_OK;
		BSTR bstrLicensePackageAttribsStream;

		_variant_t vtLicenseArchive;
		VariantInit(&vtLicenseArchive);

		try
		{
			// package the string in to a variant
			vtLicenseArchive.vt = VT_ARRAY | VT_UI1;
			vtLicenseArchive.parray = SafeArrayCreateVector(VT_UI1, 0, (byteLicenseArchive->Length+1)*sizeof(wchar_t));

			BYTE *pPacket = 0;
			hrResult = SafeArrayAccessData(vtLicenseArchive.parray, (void**)&pPacket);
			if (FAILED(hrResult)) 
				throw hrResult;

			{
			pin_ptr<Byte> pSrcPinned = &byteLicenseArchive[0];
			memcpy(pPacket, pSrcPinned, (byteLicenseArchive->Length+1)*sizeof(wchar_t));
			}

			SafeArrayUnaccessData(vtLicenseArchive.parray);
			

			hrResult = m_pLicenseServerWrapper->GenerateLicPackage_BySoftwareLicArchive(vtLicenseArchive, &bstrLicensePackageAttribsStream);
			if(FAILED(hrResult)) 
				throw hrResult;
			
			retLicensePackageAttribsStream = gcnew String(bstrLicensePackageAttribsStream);
		}
		catch(HRESULT &ehr)
		{
			hrResult = ehr;
		}

		// cleanup section
		VariantClear(&vtLicenseArchive);
		SysFreeString(bstrLicensePackageAttribsStream);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return retLicensePackageAttribsStream;
	}
	//throws an exception on a failure, returns licenseInfoAttribsStream on success.
	String^ SolimarLicenseServerWrapper::GenerateLicPackage_BySoftwareLicPacket(array<Byte>^ byteLicensePacket)
	{
		String^ retLicensePackageAttribsStream = "";;
		HRESULT hrResult = S_OK;
		BSTR bstrLicensePackageAttribsStream;

		_variant_t vtLicensePacket;
		VariantInit(&vtLicensePacket);

		try
		{
			// package the string in to a variant
			vtLicensePacket.vt = VT_ARRAY | VT_UI1;
			vtLicensePacket.parray = SafeArrayCreateVector(VT_UI1, 0, (byteLicensePacket->Length+1)*sizeof(wchar_t));

			BYTE *pPacket = 0;
			hrResult = SafeArrayAccessData(vtLicensePacket.parray, (void**)&pPacket);
			if (FAILED(hrResult)) 
				throw hrResult;

			{
			pin_ptr<Byte> pSrcPinned = &byteLicensePacket[0];
			memcpy(pPacket, pSrcPinned, (byteLicensePacket->Length+1)*sizeof(wchar_t));
			}

			SafeArrayUnaccessData(vtLicensePacket.parray);
			

			hrResult = m_pLicenseServerWrapper->GenerateLicPackage_BySoftwareLicPacket(vtLicensePacket, &bstrLicensePackageAttribsStream);
			if(FAILED(hrResult)) 
				throw hrResult;
			
			retLicensePackageAttribsStream = gcnew String(bstrLicensePackageAttribsStream);
		}
		catch(HRESULT &ehr)
		{
			hrResult = ehr;
		}

		// cleanup section
		VariantClear(&vtLicensePacket);
		SysFreeString(bstrLicensePackageAttribsStream);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return retLicensePackageAttribsStream;
	}

	//throws an exception on a failure, 
	void SolimarLicenseServerWrapper::GenerateLicenseSystemData(array<Byte>^% refByteLicSysDataPacket)
	{
		_variant_t vtLicenseSysData;
		VariantInit(&vtLicenseSysData);
		HRESULT hrResult = m_pLicenseServerWrapper->GenerateLicenseSystemData(&vtLicenseSysData);
		if(SUCCEEDED(hrResult))
		{
			//Cycle through the SafeArray returned in the variant vtLicensePackage
			if(vtLicenseSysData.vt == (VT_ARRAY | VT_UI1))
			{
				BYTE *pPacket = 0;
				wchar_t *pPacketString = 0;
				hrResult = SafeArrayAccessData(vtLicenseSysData.parray, (void**)&pPacket);
				if (SUCCEEDED(hrResult))
				{
					refByteLicSysDataPacket = gcnew array<Byte>(vtLicenseSysData.parray->rgsabound[0].cElements);
					for(unsigned int idx=0; idx<vtLicenseSysData.parray->rgsabound[0].cElements; idx++)
						refByteLicSysDataPacket[idx] = pPacket[idx];

					// done with the source variant
					SafeArrayUnaccessData(vtLicenseSysData.parray);
				}
			}
		}
		
		VariantClear(&vtLicenseSysData);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return ;
	}

	//throws an exception on a failure, 
	String^ SolimarLicenseServerWrapper::GenerateStreamData_ByLicenseSystemData(array<Byte>^ byteLicSysDataPacket, String^% refModifiedDateStreamed, String^% refKeyAttribsListStream, String^% refLicUsageDataAttribsStream, String^% refConnectionAttribsListStream)
	{
		String^ retLicInfoAttribsListStream = "";
		HRESULT hrResult = S_OK;
		BSTR bstrLicInfoAttribsListStream;
		BSTR bstrModifiedDateStreamedStream;
		BSTR bstrKeyAttribsListStream;
		BSTR bstrLicUsageDataAttribsStream;
		BSTR bstrConnectionAttribsListStream;

		_variant_t vtLicInfoAttribsListPacket;
		VariantInit(&vtLicInfoAttribsListPacket);

		try
		{
			// package the string in to a variant
			vtLicInfoAttribsListPacket.vt = VT_ARRAY | VT_UI1;
			vtLicInfoAttribsListPacket.parray = SafeArrayCreateVector(VT_UI1, 0, (byteLicSysDataPacket->Length+1)*sizeof(wchar_t));

			BYTE *pPacket = 0;
			hrResult = SafeArrayAccessData(vtLicInfoAttribsListPacket.parray, (void**)&pPacket);
			if (FAILED(hrResult)) 
				throw hrResult;

			{
			pin_ptr<Byte> pSrcPinned = &byteLicSysDataPacket[0];
			memcpy(pPacket, pSrcPinned, (byteLicSysDataPacket->Length+1)*sizeof(wchar_t));
			}

			SafeArrayUnaccessData(vtLicInfoAttribsListPacket.parray);
			
			hrResult = m_pLicenseServerWrapper->GenerateStreamData_ByLicenseSystemData(vtLicInfoAttribsListPacket, &bstrModifiedDateStreamedStream, &bstrKeyAttribsListStream, &bstrLicUsageDataAttribsStream, &bstrConnectionAttribsListStream, &bstrLicInfoAttribsListStream);
			if(FAILED(hrResult)) 
				throw hrResult;
			
			retLicInfoAttribsListStream = gcnew String(bstrLicInfoAttribsListStream);
			refKeyAttribsListStream = gcnew String(bstrKeyAttribsListStream);
			refLicUsageDataAttribsStream = gcnew String(bstrLicUsageDataAttribsStream);
			refConnectionAttribsListStream = gcnew String(bstrConnectionAttribsListStream);
			refModifiedDateStreamed = gcnew String(bstrModifiedDateStreamedStream);
		}
		catch(HRESULT &ehr)
		{
			hrResult = ehr;
		}

		// cleanup section
		VariantClear(&vtLicInfoAttribsListPacket);
		SysFreeString(bstrConnectionAttribsListStream);
		SysFreeString(bstrKeyAttribsListStream);
		SysFreeString(bstrLicUsageDataAttribsStream);
		SysFreeString(bstrLicInfoAttribsListStream);
		SysFreeString(bstrModifiedDateStreamedStream);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return retLicInfoAttribsListStream;
	}

	//throws an exception on a failure, 
	String^ SolimarLicenseServerWrapper::GenerateStream_ByLicenseSystemData(array<Byte>^ byteLicSysDataPacket)
	{
		String^ retLicSysDataAttribsStream = "";
		HRESULT hrResult = S_OK;
		BSTR bstrLicSysDataAttribsStream;

		_variant_t vtLicSysDataPacket;
		VariantInit(&vtLicSysDataPacket);

		try
		{
			// package the string in to a variant
			vtLicSysDataPacket.vt = VT_ARRAY | VT_UI1;
			vtLicSysDataPacket.parray = SafeArrayCreateVector(VT_UI1, 0, (byteLicSysDataPacket->Length+1)*sizeof(wchar_t));

			BYTE *pPacket = 0;
			hrResult = SafeArrayAccessData(vtLicSysDataPacket.parray, (void**)&pPacket);
			if (FAILED(hrResult)) 
				throw hrResult;

			{
			pin_ptr<Byte> pSrcPinned = &byteLicSysDataPacket[0];
			memcpy(pPacket, pSrcPinned, (byteLicSysDataPacket->Length+1)*sizeof(wchar_t));
			}

			SafeArrayUnaccessData(vtLicSysDataPacket.parray);
			

			hrResult = m_pLicenseServerWrapper->GenerateStream_ByLicenseSystemData(vtLicSysDataPacket, &bstrLicSysDataAttribsStream);
			if(FAILED(hrResult)) 
				throw hrResult;
			
			retLicSysDataAttribsStream = gcnew String(bstrLicSysDataAttribsStream);
		}
		catch(HRESULT &ehr)
		{
			hrResult = ehr;
		}

		// cleanup section
		VariantClear(&vtLicSysDataPacket);
		SysFreeString(bstrLicSysDataAttribsStream);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return retLicSysDataAttribsStream;
	}

	void SolimarLicenseServerWrapper::SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber(String^ softwareLicense, String^ contractNumber)
	{
		BSTR bstrSoftwareLicense;
		BSTR bstrContractNumber;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(softwareLicense));
		bstrSoftwareLicense = (static_cast<BSTR>(static_cast<void *>(ptr)));

		System::IntPtr ptr2(System::Runtime::InteropServices::Marshal::StringToBSTR(contractNumber));
		bstrContractNumber = (static_cast<BSTR>(static_cast<void *>(ptr2)));
		

		HRESULT hrResult = m_pLicenseServerWrapper->SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber(bstrSoftwareLicense, bstrContractNumber);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr2);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		return ;	
	}

	System::Collections::Generic::List<SolimarLicenseProtectionKeyInfo^>^ SolimarLicenseServerWrapper::KeyEnumerate()
	{
		System::Collections::Generic::List<SolimarLicenseProtectionKeyInfo^>^ pRetKeyList = gcnew System::Collections::Generic::List<SolimarLicenseProtectionKeyInfo^>();
		
		VARIANT* pKeyName;
		VARIANT vtKeyList;
		VariantInit(&vtKeyList);
		
		//get the list of keys
		HRESULT hr = m_pLicenseServerWrapper->KeyEnumerate(&vtKeyList);
		if(FAILED(hr))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hr).c_str()), hr);

		// SafeArrayAccessData and SafeArrayUnaccessData are win32 functions needed to access safe array data
		if (SUCCEEDED(SafeArrayAccessData(vtKeyList.parray, (void**)&pKeyName)))
		{
			SolimarLicenseProtectionKeyInfo^ pKeyInfo;
			for (int idx=0; idx<vtKeyList.parray->rgsabound[0].cElements; idx++)
			{
				pKeyInfo = gcnew SolimarLicenseProtectionKeyInfo();

				pKeyInfo->keyName = gcnew String(BSTR(pKeyName[idx].bstrVal));
				long headerID = -1;
				long keyValue = -1;
				headerID = LookupHeaderID(L"Product Version");
				if(headerID != -1)
				{
					keyValue = KeyHeaderQuery(pKeyName[idx].bstrVal, headerID);
					pKeyInfo->version = String::Format("{0:x}.{1:x}", (keyValue & 0xF000) >> 12, (keyValue & 0x0FF0) >> 4);
				}

				headerID = LookupHeaderID(L"Key Type");
				if (headerID != -1)
				{
					pKeyInfo->keyTypeID = KeyHeaderQuery(pKeyName[idx].bstrVal, headerID);
					pKeyInfo->keyTypeName = pKeyInfo->GetKeyTypeString((int)pKeyInfo->keyTypeID);
				}
				headerID = LookupHeaderID(L"Product ID");
				if (headerID != -1)
				{
					pKeyInfo->productID = KeyHeaderQuery(pKeyName[idx].bstrVal, headerID);
					try
					{
						pKeyInfo->productName = gcnew String(SolimarLicenseManagerWrapper::LicensingWrapper::keyspec.products[pKeyInfo->productID].name);
					}
					catch(...)
					{
						pKeyInfo->productName = String::Format("Unknown: {0}", pKeyInfo->productID);
					}

				}

				headerID = LookupHeaderID(L"Status");
				if (headerID != -1)
					pKeyInfo->licenseTypeName = pKeyInfo->GetLicenseIdString(KeyHeaderQuery(pKeyName[idx].bstrVal, headerID));

				headerID = LookupHeaderID(L"Application Instances");
				if (headerID != -1)
					pKeyInfo->applicationInstance = KeyHeaderQuery(pKeyName[idx].bstrVal, headerID);

				hr = m_pLicenseServerWrapper->KeyTrialHours(pKeyName[idx].bstrVal, &keyValue);
				if(SUCCEEDED(hr))
					pKeyInfo->hoursLeft = keyValue;
				pKeyInfo->expirationDate = KeyTrialExpires(pKeyName[idx].bstrVal);

				VARIANT_BOOL vtBoolValue;
				hr = m_pLicenseServerWrapper->KeyIsActive(pKeyName[idx].bstrVal, &vtBoolValue);
				if(SUCCEEDED(hr))
					pKeyInfo->bIsActive = (vtBoolValue==VARIANT_TRUE);

				hr = m_pLicenseServerWrapper->KeyIsProgrammed(pKeyName[idx].bstrVal, &vtBoolValue);
				if(SUCCEEDED(hr))
					pKeyInfo->bIsProgrammed = (vtBoolValue==VARIANT_TRUE);

				//System::Diagnostics::Trace::WriteLine("KeyEnumerate() keyName: " + pKeyInfo->keyName + ", productName:" + pKeyInfo->productName);

				pRetKeyList->Add(pKeyInfo);
			}
			SafeArrayUnaccessData(vtKeyList.parray);
		}

		VariantClear(&vtKeyList);

		return pRetKeyList;
	}

	//should only work on internal releases
	String^ SolimarLicenseServerWrapper::KeyFormat(String^ key)
	{
		//System::Diagnostics::Trace::Write(String::Format("KeyFormat() - key: {0}", key));
		String^ newKey = "";
		BSTR bstrKey;
		BSTR bstrNewKey;

		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(key));
		bstrKey = (static_cast<BSTR>(static_cast<void *>(ptr)));

		HRESULT hrResult = m_pLicenseServerWrapper->KeyFormat(bstrKey, &bstrNewKey);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		newKey = gcnew String(bstrNewKey);
		return newKey;
	}

	//should only work on internal releases
	String^ SolimarLicenseServerWrapper::KeyProgramVerification(String^ key, long customerNumber, long keyNumber)
	{
		//System::Diagnostics::Trace::Write(String::Format("KeyProgramVerification() - key: {0}, newLicense: {1}-{2}", key, customerNumber, keyNumber));
		String^ newKey = "";
		BSTR bstrKey;
		BSTR bstrNewKey;

		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(key));
		bstrKey = (static_cast<BSTR>(static_cast<void *>(ptr)));

		_variant_t vtModList;
		VariantInit(&vtModList);

		HRESULT hrResult = m_pLicenseServerWrapper->KeyProgram2(bstrKey, 
									customerNumber, 
									keyNumber, 
									0xff, //Verification ProdID
									0,	//long verMajor, 
									0,	//long verMinor, 
									0xff, //Verification keyType, 
									0,	//long application_instances
									0,	//long days
									vtModList, 
									&bstrNewKey);

		//free BSTR
		System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);
		VariantClear(&vtModList);

		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		newKey = gcnew String(bstrNewKey);
		return newKey;
	}

	//private
	long SolimarLicenseServerWrapper::KeyHeaderQuery(BSTR bstrKey, long headerIdent)
	{
		long lValue = -1;
		VARIANT vtValue;
		VariantInit(&vtValue);
		HRESULT hr = m_pLicenseServerWrapper->KeyHeaderQuery(bstrKey, headerIdent, &vtValue);
		//if(FAILED(hrResult))
		//	throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);
		if(SUCCEEDED(hr))
		{
			lValue = vtValue.ulVal;
			VariantClear(&vtValue);
		}
		return lValue;
	}

	DateTime SolimarLicenseServerWrapper::KeyTrialExpires(BSTR bstrKey)
	{
		DateTime dtExpires;

		VARIANT vtValue;
		VariantInit(&vtValue);
		//BSTR bstrKey;
		////convert the string* to a BSTR
		//System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(key));
		//bstrKey = (static_cast<BSTR>(static_cast<void *>(ptr)));

		HRESULT hrResult = m_pLicenseServerWrapper->KeyTrialExpires(bstrKey, &vtValue);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		//Convert from UTC time to Local time.
		SYSTEMTIME st;
		FILETIME ft, dft;
		LARGE_INTEGER li;  
		VariantTimeToSystemTime(vtValue.date,&st);
		SystemTimeToFileTime(&st, &ft);
		FileTimeToLocalFileTime(&ft, &dft);
		li.LowPart = dft.dwLowDateTime;
		li.HighPart = dft.dwHighDateTime;    
		dtExpires = System::DateTime::FromFileTimeUtc(li.QuadPart);
		VariantClear(&vtValue);

		return dtExpires;
	}


	long SolimarLicenseServerWrapper::LookupHeaderID(std::wstring header)
	{
		try
		{
			return SolimarLicenseManagerWrapper::LicensingWrapper::keyspec.headers[header.c_str()].id;
		}
		catch (...)
		{
			return -1;
		}
	}



	System::Collections::Generic::List<SolimarLicenseProtectionKeyModuleInfo^>^ SolimarLicenseServerWrapper::KeyModuleEnumerate(String^ key)
	{
		VARIANT vtModList;
		VariantInit(&vtModList);
		BSTR bstrKey;
		//convert the string* to a BSTR
		System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(key));
		bstrKey = (static_cast<BSTR>(static_cast<void *>(ptr)));

		HRESULT hrResult = m_pLicenseServerWrapper->KeyModuleEnumerate(bstrKey, &vtModList);
		if(FAILED(hrResult))
			throw gcnew System::Runtime::InteropServices::COMException(gcnew String(LicenseServerError::GetErrorMessage(hrResult).c_str()), hrResult);

		//void* vp;
		//// SafeArrayAccessData and SafeArrayUnaccessData are win32 functions 
		////needed to access safe array data
		//if (SUCCEEDED(SafeArrayAccessData(ModuleList->parray,&vp)))
		//{
		//	//if we successfully got a ptr to the first module, store it in
		//	//our member variable
		//	Module = reinterpret_cast<VARIANT*>(vp);
		//}
		//VariantClear(&temp);

		SolimarLicenseProtectionKeyModuleInfo^ pkModInfo;
		System::Collections::Generic::List<SolimarLicenseProtectionKeyModuleInfo^>^ modInfoList = gcnew System::Collections::Generic::List<SolimarLicenseProtectionKeyModuleInfo^>();
		if (vtModList.vt==(VT_ARRAY | VT_VARIANT))
		{
			MultidimensionalSafeArray::DimensionsType dims, index;
			hrResult = MultidimensionalSafeArray::SizeofMultidimensionalSafearray(&vtModList, dims);
			if (SUCCEEDED(hrResult) && dims.size()==2)
			{
				VARIANT *pElement = 0;
				int module_id = 0;
				for (unsigned int modIdx=0; modIdx<dims[0]; modIdx++)
				{
					pkModInfo = gcnew SolimarLicenseProtectionKeyModuleInfo();

					index.push_back(modIdx);
					{
						index.push_back(0);
						{
							MultidimensionalSafeArray::AccessMultidimensionalSafearray(&vtModList, index, &pElement);
							pkModInfo->moduleID = (unsigned int)pElement->ulVal;
							MultidimensionalSafeArray::UnaccessMultidimensionalSafearray(&vtModList, index);
						}
						index.pop_back();

						index.push_back(1);
						{
							MultidimensionalSafeArray::AccessMultidimensionalSafearray(&vtModList, index, &pElement);
							pkModInfo->moduleName = gcnew String(BSTR(pElement->bstrVal));
							MultidimensionalSafeArray::UnaccessMultidimensionalSafearray(&vtModList, index);
						}
						index.pop_back();

						index.push_back(2);
						{
							MultidimensionalSafeArray::AccessMultidimensionalSafearray(&vtModList, index, &pElement);
							pkModInfo->moduleUnlimited = (unsigned int)pElement->ulVal;
							MultidimensionalSafeArray::UnaccessMultidimensionalSafearray(&vtModList, index);
						}
						index.pop_back();

						index.push_back(3);
						{
							MultidimensionalSafeArray::AccessMultidimensionalSafearray(&vtModList, index, &pElement);
							pkModInfo->modulePool = (unsigned int)pElement->ulVal;
							MultidimensionalSafeArray::UnaccessMultidimensionalSafearray(&vtModList, index);
						}
						index.pop_back();

						index.push_back(4);
						{
							MultidimensionalSafeArray::AccessMultidimensionalSafearray(&vtModList, index, &pElement);
							pkModInfo->moduleShared = (unsigned int)pElement->ulVal;
							MultidimensionalSafeArray::UnaccessMultidimensionalSafearray(&vtModList, index);
						}
						index.pop_back();
					}
					index.pop_back();
	
					long tmpValue;
					hrResult = m_pLicenseServerWrapper->KeyModuleLicenseTotal(bstrKey, pkModInfo->moduleID, &tmpValue);
					if(SUCCEEDED(hrResult))
						pkModInfo->moduleTotal = tmpValue;
					//System::Diagnostics::Trace::Write(String::Format("KeyModuleEnumerate - modID: {0}, hrResult: {1:x}", pkModInfo->moduleID, hrResult));

					hrResult = m_pLicenseServerWrapper->KeyModuleInUse(bstrKey, pkModInfo->moduleID, &tmpValue);
					if(SUCCEEDED(hrResult))
						pkModInfo->moduleInUse = tmpValue;

					modInfoList->Add(pkModInfo);
				}
			}
		}
		VariantClear(&vtModList);

		//	if (SUCCEEDED(SafeArrayAccessData(Module[ModuleIndex].parray, void**)(&vtModuleInfo))))
		//	{
		//		int nElements = vtModuleList.parray->rgsabound->cElements;
		//		for(int idx=0; idx<nElements; idx++)
		//		{
		//		}
		////if(vtLicensePackage.vt == (VT_ARRAY | VT_UI1))
		////	{
		////		BYTE *pPacket = 0;
		////		wchar_t *pPacketString = 0;
		////		hrResult = SafeArrayAccessData(vtLicensePackage.parray, (void**)&pPacket);
		////SafeArrayUnaccessData(vtLicensePackage.parray);

		////retVal = vtValue.ulVal;
		//	}
			//VariantClear(&vtModuleList);

		return modInfoList;
	}
}}}
