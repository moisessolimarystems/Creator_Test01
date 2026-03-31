// SolimarLicenseManagerDotNet.h

#pragma once

#include "..\manager_wrapper\LicensingWrapper.h"

using namespace System;

namespace SolimarLicenseManagerDotNet {

	//This wrapper exposes all the functionality in License Wrapper.cpp/.h for managed code
	public ref class SolimarLicenseWrapper
	{
		public:

			enum class LICENSE_LEVEL
			{
				UI_IGNORE =				0x00,
				UI_LEVEL_ALL =			0x01,
				UI_LEVEL_RESPONSE =		0x02,
				UI_LEVEL_CRITICAL =		0x04,
				UI_STYLE_DIALOG =		0x10,
				UI_STYLE_EVENT_LOG =	0x20,
			};	

			SolimarLicenseWrapper();
			~SolimarLicenseWrapper();
			bool Connect(String^ server);
			bool Connect(String^ server, bool bUseOnlySharedLicenses, bool bUseAsBackup);
			bool ConnectByProduct(long product, bool bUseSharedLicenseServers);
			bool Initialize(long product, long prodVerMajor, long prodVerMinor, bool singleKey, String^ specificSingleKeyIdent, bool lockKeys, LICENSE_LEVEL uiLevel, unsigned long gracePeriodMinutes);
			bool Initialize(String^ applicationInstance, long product, long prodVerMajor, long prodVerMinor, bool singleKey, String^  specificSingleKeyIdent, bool lockKeys, LICENSE_LEVEL uiLevel, unsigned long gracePeriodMinutes, bool applicationInstanceLockKeys, bool bypassRemoteKeyRestriction);

			bool KeyProductExists(long product, long prodVerMajor, long prodVerMinor, bool% bRefKeyExists);
			bool ModuleLicenseTotal(long module, long% refLicenseCount);
			bool ModuleLicenseInUse(long module, long% refLicenseCount);
			bool ModuleLicenseObtain(long module, long licenseCount);
			bool ModuleLicenseRelease(long module, long licenseCount);
			bool ModuleLicenseCounterDecrement(long module, long licenseCount);
			bool ValidateLicense();
			bool GetVersionLicenseManager(long% refVerMajor, long% refVerMinor, long% refVerBuild);
			bool GetVersionLicenseServer(String^ server, long% refVerMajor, long% refVerMinor, long% refVerBuild);

			HRESULT ConnectEx(String^ server);
			HRESULT ConnectEx(String^ server, bool bUseOnlySharedLicenses, bool bUseAsBackup);
			HRESULT ConnectByProductEx(long product, bool bUseSharedLicenseServers);
			HRESULT InitializeEx(long product, long prodVerMajor, long prodVerMinor, bool singleKey, String^ specificSingleKeyIdent, bool lockKeys, LICENSE_LEVEL uiLevel, unsigned long gracePeriodMinutes);
			HRESULT InitializeEx(String^ applicationInstance, long product, long prodVerMajor, long prodVerMinor, bool singleKey, String^  specificSingleKeyIdent, bool lockKeys, LICENSE_LEVEL uiLevel, unsigned long gracePeriodMinutes, bool applicationInstanceLockKeys, bool bypassRemoteKeyRestriction);
			HRESULT KeyProductExistsEx(int product, int prodVerMajor, int prodVerMinor, bool% bRefKeyExists);
			HRESULT ModuleLicenseTotalEx(long module, long% refLicenseCount);
			HRESULT ModuleLicenseInUseEx(long module, long% refLicenseCount);
			HRESULT ModuleLicenseObtainEx(long module, long licenseCount);
			HRESULT ModuleLicenseReleaseEx(long module, long licenseCount);
			HRESULT ModuleLicenseCounterDecrementEx(long module, long licenseCount);
			HRESULT ValidateLicenseEx(bool% bRefValid);
			HRESULT GetVersionLicenseManagerEx(long% refVerMajor, long% refVerMinor, long% refVerBuild);
			HRESULT GetVersionLicenseServerEx(String^ server, long% refVerMajor, long% refVerMinor, long% refVerBuild);

			long LookupProductID(String^ product);
			long LookupModuleID(long productId, String^ module);
			long LookupHeaderID(String^ header);	
			
			/*
			typedef void (*LicenseMessageCallbackPtr)(void* pContext, const wchar_t* key_ident, unsigned int message_type, HRESULT error, VARIANT *pvtTimestamp, const wchar_t* message);
			bool RegisterMessageCallback(void* pContext, LicenseMessageCallbackPtr LicenseMessageCallback);
			bool UnregisterMessageCallback();
			*/
			delegate void DelegateMessageCallbackManaged(IntPtr pContext, String^ keyIdent, unsigned int messageType, HRESULT hrError, DateTime^ pvtTimestamp, String^ message);
			delegate void DelegateInvalidLicenseCallbackManaged(IntPtr pContext, HRESULT hr, String^ message);
			
			bool RegisterMessageCallback(IntPtr pContext, DelegateMessageCallbackManaged^ licenseMessageCallback);
			bool UnregisterMessageCallback();
			
			bool RegisterInvalidLicenseCallback(IntPtr pContext, DelegateInvalidLicenseCallbackManaged^ licenseInvalidCallback);
			bool UnregisterInvalidLicenseCallback();

		private:
			SolimarLicenseManagerWrapper::LicensingWrapper* m_pLicenseWrapper;

			delegate void DelegateMessageCallbackUnmanaged(void* pContext, const wchar_t* key_ident, unsigned int message_type, HRESULT error, VARIANT *pvtTimestamp, const wchar_t* message);
			DelegateMessageCallbackUnmanaged^ m_messageCallbackUnmanaged;
			DelegateMessageCallbackManaged^ m_messageCallbackManaged;
			bool m_bRegisteredMessageCallback;

			delegate void DelegateInvalidLicenseCallbackUnmanaged(void* pContext, HRESULT hr, const wchar_t* message);
			DelegateInvalidLicenseCallbackUnmanaged^ m_invalidLicenseCallbackUnmanaged;
			DelegateInvalidLicenseCallbackManaged^ m_invalidLicenseCallbackManaged;
			bool m_bRegisteredInvalidLicenseCallback;
			
			void MessageCallback(void* pContext, const wchar_t* key_ident, unsigned int message_type, HRESULT error, VARIANT *pvtTimestamp, const wchar_t* message);
			void InvalidLicenseCallback(void* pContext, HRESULT hr, const wchar_t* message);
	};
}


