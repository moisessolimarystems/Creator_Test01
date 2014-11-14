// SolimarLicenseManagerDotNet.h

#pragma once

#include "..\manager_wrapper\LicensingWrapper.h"
#include "LicensingServerWrapper.h"

using namespace System;

namespace Solimar {	namespace Licensing {		namespace LicenseManagerWrapper 
{
	///<summary>
	///This wrapper exposes all the functionality in License Wrapper.cpp/.h for managed code
	///</summary>
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

			enum class CONNECTION_FLAGS
			{
				CF_NONE =	0x00,
				CF_BACKUP_SERVER = 0x01,
				CF_ONLY_SHARED_LICENSE = 0x02,
				CF_SOFTWARE_LICENSING = 0x04,
				CF_ONLY_LICENSE_VIEWER = 0x08,
			};

			SolimarLicenseWrapper();
			
			~SolimarLicenseWrapper();	//Destructor for cleaning up managed resources
			!SolimarLicenseWrapper();	//Finalizer for cleaning up unmanaged resources

			///<summary>
			///Connect to the License Server on the given Machine.
			///</summary>
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
			void ConnectLicenseViewerEx(String^ server);
			void ConnectByProductEx(long product, bool bUseSharedLicenseServers);
			void GetInfoByProductEx(long product, bool bUseSharedLicenseServers, String^% refLicServer, String^% refBackupLicServer, bool% refBUseTestDev);
			void DisconnectEx();
			// Returns true if there is any licensing for the given products, else false.  If no licensing was present initially,
			// but licensing is added afterwards, than the initialized connection will be able to have modules obtained against it
			// and validated.
			bool InitializeEx(long product, long prodVerMajor, long prodVerMinor, bool singleKey, String^ specificSingleKeyIdent, bool lockKeys, LICENSE_LEVEL uiLevel, unsigned long gracePeriodMinutes);
			bool InitializeEx(String^ applicationInstance, long product, long prodVerMajor, long prodVerMinor, bool singleKey, String^  specificSingleKeyIdent, bool lockKeys, LICENSE_LEVEL uiLevel, unsigned long gracePeriodMinutes, bool applicationInstanceLockKeys, bool bypassRemoteKeyRestriction);
			bool InitializeViewOnly(String^ applicationInstance, long product, long prodVerMajor, long prodVerMinor);
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
			
			delegate void DelegateMessageCallbackManaged(IntPtr pContext, String^ keyIdent, unsigned int messageType, HRESULT hrError, DateTime^ pvtTimestamp, String^ message);
			delegate void DelegateInvalidLicenseCallbackManaged(IntPtr pContext, HRESULT hr, String^ message);
			
			bool RegisterMessageCallback(IntPtr pContext, DelegateMessageCallbackManaged^ licenseMessageCallback);
			bool UnregisterMessageCallback();
			
			bool RegisterInvalidLicenseCallback(IntPtr pContext, DelegateInvalidLicenseCallbackManaged^ licenseInvalidCallback);
			bool UnregisterInvalidLicenseCallback();
		
		private:
			SolimarLicenseManagerWrapper::LicensingWrapper* m_pLicenseWrapper;

			String^ GetLicenseServerVersion(String^ _server);
			String^ GetLicenseManagerVersion();
			System::Runtime::InteropServices::COMException^ GenerateException(HRESULT _hr);

			delegate void DelegateMessageCallbackUnmanaged(void* pContext, const wchar_t* key_ident, unsigned int message_type, HRESULT error, VARIANT *pvtTimestamp, const wchar_t* message);
			DelegateMessageCallbackUnmanaged^ m_messageCallbackUnmanaged;
			DelegateMessageCallbackManaged^ m_messageCallbackManaged;
			bool m_bRegisteredMessageCallback;
			bool m_bFinalized;

			bool m_bUseSharedLicenseServers;
			long m_productID;
			String^ m_licServer;

			delegate void DelegateInvalidLicenseCallbackUnmanaged(void* pContext, HRESULT hr, const wchar_t* message);
			DelegateInvalidLicenseCallbackUnmanaged^ m_invalidLicenseCallbackUnmanaged;
			DelegateInvalidLicenseCallbackManaged^ m_invalidLicenseCallbackManaged;
			bool m_bRegisteredInvalidLicenseCallback;
			
			void MessageCallback(void* pContext, const wchar_t* key_ident, unsigned int message_type, HRESULT error, VARIANT *pvtTimestamp, const wchar_t* message);
			void InvalidLicenseCallback(void* pContext, HRESULT hr, const wchar_t* message);
	};







	public ref class SolimarLicenseProtectionKeyInfo
	{
		public:
			SolimarLicenseProtectionKeyInfo()
			{
				keyName = "";
				productID = -1;
				productName = "";
				version = "0.0";
				hoursLeft = 0;
				expirationDate = DateTime(0);
				keyTypeName = "";
				licenseTypeName = "";
				keyTypeID = 0;
				bIsActive = false;
				bIsProgrammed = false;
				applicationInstance = 0;
			}
			String^ keyName;
			long productID;
			String^ productName;
			String^ version;
			long hoursLeft;
			DateTime expirationDate;
			String^ keyTypeName;
			long keyTypeID;
			String^ licenseTypeName;
			bool bIsActive;
			bool bIsProgrammed;
			long applicationInstance;

			String^ GetKeyTypeString()		{return GetKeyTypeString(keyTypeID);}
			bool IsKeyTypeUninitialized()	{return IsKeyTypeUninitialized(keyTypeID);}
			bool IsKeyTypeVerification()	{return IsKeyTypeVerification(keyTypeID);}
			bool IsKeyTypeDevelopment()	{return IsKeyTypeDevelopment(keyTypeID);}
			//String^ GetLicenseIdString(licenseID)	{return GetLicenseIdString(licenseID);}

			static String^ GetKeyTypeString(int keyType)
			{
				String^ keyTypeName = "";
				if (keyType == 0)
					keyTypeName = "Uninitialized Trial";
				else if (keyType == 1)
					keyTypeName = "Addon";
				else if (keyType == 2)
					keyTypeName = "Base";
				else if (keyType == 3)
					keyTypeName = "Replacement";
				else if (keyType == 4)
					keyTypeName = "Relicense";
				else if (keyType == 5)
					keyTypeName = "Inventory";
				else if (keyType == 7)
					keyTypeName = "Emergency";
				else if (keyType == 9)
					keyTypeName = "Custom";
				else if (keyType == 10)
					keyTypeName = "Loan";
				else if (keyType == 11)
					keyTypeName = "Demo";
				else if (keyType == 12)
					keyTypeName = "Rental";
				else if (keyType == 13)
					keyTypeName = "Development";
				else if (keyType == 14)
					keyTypeName = "Reserved";
				else if (keyType == 255)
					keyTypeName = "Verification";
				else
					keyTypeName = "Unknown";
				return keyTypeName;
			}
			static bool IsKeyTypeUninitialized(int keyType){return (keyType == 0);}
			static bool IsKeyTypeVerification(int keyType){return (keyType == 255);}
			static bool IsKeyTypeDevelopment(int keyType){return (keyType == 13);}
			static String^ GetLicenseIdString(int licenseID)
			{
				// CR.FIX.15648 - Fixed Extension 5 not displaying correctly.
				String^ licenseType = "Unknown License";
				if (licenseID == 0)
					licenseType = "Initial Trial";
				else if (licenseID == 1)
					licenseType = "Extended Trial";
				else if (licenseID == 2)
					licenseType = "Base";
				else if (licenseID == 3)
					licenseType = "Uninitialized Trial";
				else if (4 <= licenseID && licenseID <= 7)
					licenseType = "Extended Trial " + (licenseID-2).ToString();
				else if (licenseID == 10)
					licenseType = "Unused";
				else if (licenseID == 11)
					licenseType = "Deactivated";
				else if (12 <= licenseID && licenseID <= 22)
					licenseType = "Extended Trial " + (licenseID - 6).ToString();
				return licenseType;
			}
	};
	public ref class SolimarLicenseProtectionKeyModuleInfo
	{
		public:
			SolimarLicenseProtectionKeyModuleInfo()
			{
				moduleID = 0;
				moduleName = "";
				moduleUnlimited = 0;
				modulePool = 0;
				moduleShared = 0;
				moduleInUse = 0;
				moduleTotal = 0;
			}
			long moduleID;
			String^ moduleName;
			long moduleUnlimited;
			long modulePool;
			long moduleShared;
			long moduleInUse;
			long moduleTotal;
	};
	public ref class SolimarLicenseServerWrapper
	{
		public:
			SolimarLicenseServerWrapper();
			~SolimarLicenseServerWrapper();

			void Connect(String^ server);
			void Disconnect();

			long SoftwareModuleLicenseTotalForAll_ByProduct(long productID, long moduleIdent);
			long SoftwareModuleLicenseInUseForAll_ByProduct(long productID, long moduleIdent);
			//HRESULT SoftwareModuleLicenseTotalForAll_ByProduct(long productID, long moduleIdent, long% refLicenseCount);
			//HRESULT SoftwareModuleLicenseInUseForAll_ByProduct(long productID, long moduleIdent, long% refLicenseCount);
			String^ GetSoftwareLicenseInfoByProduct_ForAll(long productID);

			//returns licenseInfoAttribsStream
			String^ GetSoftwareLicenseInfo_ForAll();

			//returns licenseListStream
			String^ GetAllSoftwareLicenses();

			//returns productInfoAttribsStream
			String^ GetSoftwareLicenseInfoByProduct_ByLicense(String^ softwareLicense, long productID);

			//returns licenseInfoAttribsStream
			String^ GetSoftwareLicenseInfo_ByLicense(String^ softwareLicense);

			//returns map<string, string> as a stream where the first string is the sofware license name, and
			//the second string is the status
			String^ GetSoftwareLicenseStatus_ByProduct(long productID);

			String^ GetSoftwareLicenseStatus_ByLicense(String^ softwareLicense, bool% refBSucess);

			//throws an exception if fails to verify with reason why failure occurred
			void ValidateToken_ByLicense(String^ softwareLicense, long validationTokenType, String^ validationValue);

			//HRESULT GetSoftwareSpecByProduct(long productID, String^% refProductSoftwareSpecAttribs);
			//HRESULT GetSoftwareSpec(String^% refSoftwareSpecAttribsStream);
			String^ GetSoftwareSpecByProduct(long productID);
			String^ GetSoftwareSpec();

			//returns appInstListStream
			String^ SoftwareGetApplicationInstanceListByProduct(long productID);

			//returns a streamed Lic_UsProductInfoAttribs
			String^ SoftwareGetApplicationInstanceListByProduct2(long productID);

			//throws an exception on a failure
			void GenerateSoftwareLicPacket(String^ licPackageAttribsStream, DateTime expirationDate, String^% refVerificationCode, array<Byte>^% refByteLicensePacket);
			//returns VerificationCode
			String^ EnterSoftwareLicPacket(array<Byte>^ byteLicensePacket);
			String^ EnterProtectionKeyPasswordPacket(array<Byte>^ byteLicensePacket);
			//throws an exception on a failure
			bool EnterProtectionKeyPassword(String^ password);
			
			//throws an exception on a failure
			void GenerateSoftwareLicArchive_ByLicense(String^ softwareLicense, array<Byte>^% refByteLicenseArchive);
			//returns VerificationCode
			void EnterSoftwareLicArchive(array<Byte>^ byteLicenseArchive);

			//throws an exception on a failure
			void GenerateVerifyDataWithVerCode_ByLicense(String^ softwareLicense, array<Byte>^% refByteLicensePacket);
			//throws an exception on a failure
			void GenerateVerifyDataWithLicInfo_ByLicense(String^ softwareLicense, array<Byte>^% refByteLicensePacket);

			//throws an exception on a failure, returns licenseInfoAttribsStream on success.
			String^ GenerateLicInfo_ByVerifyData(array<Byte>^ byteVerifyData);

			//throws an exception on a failure, returns licenseInfoAttribsStream on success.
			String^ GenerateLicPackage_BySoftwareLicArchive(array<Byte>^ byteLicenseArchive);

			//throws an exception on a failure, returns licenseInfoAttribsStream on success.
			String^ GenerateLicPackage_BySoftwareLicPacket(array<Byte>^ byteLicensePacket);

			//throws an exception on a failure
			void GenerateLicenseSystemData(array<Byte>^% refByteLicSysDataPacket);

			void GenerateLicenseSystemDataForSolimar();
			
			//throws an exception on a failure, returns licenseInfoAttribsStream on success.
			String^ GenerateStreamData_ByLicenseSystemData(array<Byte>^ byteLicSysDataPacket, String^% refModifiedDateStreamed, String^% refKeyAttribsListStream, String^% refLicUsageDataAttribsStream, String^% refEventLogAttribsListStream, String^% refConnectionAttribsListStream);

			//throws an exception on a failure, returns LicAlertInfoAttribs on success.
			String^ GenerateStreamData_ByLicenseSystemData2(array<Byte>^ byteLicSysDataPacket, String^% refModifiedDateStreamed, String^% refKeyAttribsListStream, String^% refLicUsageDataAttribsStream, String^% refEventLogAttribsListStream, String^% refConnectionAttribsListStream, String^% refLicInfoDataAttribsListStream);

			//throws an exception on a failure, returns licenseInfoAttribsStream on success.
			String^ GenerateStream_ByLicenseSystemData(array<Byte>^ byteLicSysDataPacket);

			//throws an exception on a failure, 
			String^ GetEventLogList_ForLicenseServer();

			String^ GetMailServerInfo();
			void SetMailServerInfo(String^ alertMailServerAttribsStream);
			void TestMailServerInfo(String^ testMailServerAttribsStream);
			String^ GetAllEmailAlerts();
			String^ GetEmailAlert(String^ emailAlertId);
			void SetEmailAlert(String^ emailAlertId, String^ emailAlertMailAttribsStream);
			String^ AddEmailAlert(String^ emailAlertMailAttribsStream);
			void DeleteEmailAlert(String^ emailAlertId);

			bool CanDeleteSoftwareLicense(String^ softwareLicense);
			void DeleteSoftwareLicense(String^ softwareLicense);

			void SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber(String^ softwareLicense, String^ contractNumber);

			System::Collections::Generic::List<SolimarLicenseProtectionKeyInfo^>^ KeyEnumerate();

			System::Collections::Generic::List<SolimarLicenseProtectionKeyModuleInfo^>^ KeyModuleEnumerate(String^ key);

			String^ KeyFormat(String^ key);
			String^ KeyProgramVerification(String^ key, long customerNumber, long keyNumber);

		private:

			long LookupHeaderID(std::wstring header);
			long KeyHeaderQuery(BSTR bstrKey, long headerIdent);
			DateTime KeyTrialExpires(BSTR bstrKey);
			SolimarLicenseManagerWrapper::LicensingServerWrapper* m_pLicenseServerWrapper;
	};

}}}


