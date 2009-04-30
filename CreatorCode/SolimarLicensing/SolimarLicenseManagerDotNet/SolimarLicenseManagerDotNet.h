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
			
			~SolimarLicenseWrapper();	//Destructor for cleaning up managed resources
			!SolimarLicenseWrapper();	//Finalizer for cleaning up unmanaged resources
			bool Connect(String^ server);
			bool Connect(String^ server, bool bUseOnlySharedLicenses, bool bUseAsBackup);
			bool ConnectByProduct(long product, bool bUseSharedLicenseServers);
			bool Disconnect();
			bool Initialize(long product, long prodVerMajor, long prodVerMinor, bool singleKey, String^ specificSingleKeyIdent, bool lockKeys, LICENSE_LEVEL uiLevel, unsigned long gracePeriodMinutes);
			bool Initialize(String^ applicationInstance, long product, long prodVerMajor, long prodVerMinor, bool singleKey, String^  specificSingleKeyIdent, bool lockKeys, LICENSE_LEVEL uiLevel, unsigned long gracePeriodMinutes, bool applicationInstanceLockKeys, bool bypassRemoteKeyRestriction);

			bool KeyProductExists(long product, long prodVerMajor, long prodVerMinor, bool% bRefKeyExists);
			bool ModuleLicenseTotal(long module, long% refLicenseCount);
			bool ModuleLicenseInUse(long module, long% refLicenseCount);
			bool ModuleLicenseInUse_ByApp(long module, long% refLicenseCount);
			bool ModuleLicenseObtain(long module, long licenseCount);
			bool ModuleLicenseRelease(long module, long licenseCount);
			bool ModuleLicenseCounterDecrement(long module, long licenseCount);
			bool ValidateLicense();
			bool GetVersionLicenseManager(long% refVerMajor, long% refVerMinor, long% refVerBuild);
			bool GetVersionLicenseServer(String^ server, long% refVerMajor, long% refVerMinor, long% refVerBuild);

			void ConnectEx(String^ server);
			void ConnectEx(String^ server, bool bUseOnlySharedLicenses, bool bUseAsBackup);
			void ConnectByProductEx(long product, bool bUseSharedLicenseServers);
			void DisconnectEx();
			// Returns true if there is any licensing for the given products, else false.  If no licensing was present initially,
			// but licensing is added afterwards, than the initialized connection will be able to have modules obtained against it
			// and validated.
			bool InitializeEx(long product, long prodVerMajor, long prodVerMinor, bool singleKey, String^ specificSingleKeyIdent, bool lockKeys, LICENSE_LEVEL uiLevel, unsigned long gracePeriodMinutes);
			bool InitializeEx(String^ applicationInstance, long product, long prodVerMajor, long prodVerMinor, bool singleKey, String^  specificSingleKeyIdent, bool lockKeys, LICENSE_LEVEL uiLevel, unsigned long gracePeriodMinutes, bool applicationInstanceLockKeys, bool bypassRemoteKeyRestriction);
			bool KeyProductExistsEx(int product, int prodVerMajor, int prodVerMinor);
			long ModuleLicenseTotalEx(long module);
			long ModuleLicenseInUseEx(long module);
			long ModuleLicenseInUse_ByAppEx(long module);
			void ModuleLicenseObtainEx(long module, long licenseCount);
			void ModuleLicenseReleaseEx(long module, long licenseCount);
			void ModuleLicenseCounterDecrementEx(long module, long licenseCount);
			bool ValidateLicenseEx();
			void GetVersionLicenseManagerEx(long% refVerMajor, long% refVerMinor, long% refVerBuild);
			void GetVersionLicenseServerEx(String^ server, long% refVerMajor, long% refVerMinor, long% refVerBuild);

			long StartLicensingSessionEx();
			void ModuleLicenseObtainLicensingSessionEx(long sessionID, long module, long licenseCount);
			void ModuleLicenseReleaseLicensingSessionEx(long sessionID, long module, long licenseCount);
			long ModuleLicenseInUseLicensingSessionEx(long sessionID, long module);
			void EndLicensingSessionEx(long sessionID);

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
			bool m_bFinalized;

			delegate void DelegateInvalidLicenseCallbackUnmanaged(void* pContext, HRESULT hr, const wchar_t* message);
			DelegateInvalidLicenseCallbackUnmanaged^ m_invalidLicenseCallbackUnmanaged;
			DelegateInvalidLicenseCallbackManaged^ m_invalidLicenseCallbackManaged;
			bool m_bRegisteredInvalidLicenseCallback;
			
			void MessageCallback(void* pContext, const wchar_t* key_ident, unsigned int message_type, HRESULT error, VARIANT *pvtTimestamp, const wchar_t* message);
			void InvalidLicenseCallback(void* pContext, HRESULT hr, const wchar_t* message);
	};
}


