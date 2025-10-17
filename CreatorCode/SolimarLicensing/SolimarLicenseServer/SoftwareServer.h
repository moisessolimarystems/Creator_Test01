#pragma once
//#include "..\Common\LicAttribsCPP\SoftwareSpec.h"
#include "LicenseCache.h"
#include "SolimarSoftwareLicenseMgr.h"
//#include "ProtectionKey.h"
#include <list>
#include <map>
#include "KeyServer.h"
//#include "RainbowDriver.h"

class SoftwareServer //: public USBNotification //Derive to get USB calls
{
	public:
		SoftwareServer();
		~SoftwareServer();

		HRESULT Initialize(KeyServer* pKeyServer, RainbowDriver* pDriver);
	
		HRESULT AddApplicationInstance(long productID, BSTR license_id, BSTR application_instance, long flags);
		HRESULT RemoveApplicationInstance(long productID, BSTR license_id, BSTR application_instance);
		HRESULT GetApplicationInstanceList(long productID, BSTR license_id, BSTR *pBstrListAppInstStream);
		HRESULT GetApplicationInstanceList2(long productID, BSTR license_id, BSTR *pBstrListUsAppInstInfoAttribs);

		HRESULT ResynchronizeSoftwareLicenses(bool bForceRefresh = false);


		HRESULT ApplyLicensePacket(Lic_PackageAttribs* pLicPacket, BSTR *pBstrVerificationCode);
		HRESULT EnterLicensePacket(VARIANT vtPasswordPacket, BSTR *pBstrVerificationCode);

		//Licensing functions - Ensure that the inuse to the total values are correct
		HRESULT ValidateLicense(long productID, BSTR licenseID, VARIANT_BOOL *pbLicenseValid);
		
		HRESULT ModuleLicenseTotalForAll(long productID, long moduleIdent, long* pLicenseCount);
		HRESULT ModuleLicenseInUseForAll(long productID, long moduleIdent, long* pLicenseCount);

		//Results based on the application instance based on the license_id 
		HRESULT ModuleLicenseInUseByApp(long productID, BSTR licenseID, long moduleIdent, long* pLicenseCount);
		//Results based on the license_id.
		HRESULT ModuleLicenseInUseByLicenseID(long productID, BSTR licenseID, long moduleIdent, long* pLicenseCount);
		HRESULT ModuleLicenseObtainByApp(long productID, BSTR licenseID, long moduleIdent, long licenseCount);
		HRESULT ModuleLicenseReleaseByApp(long productID, BSTR licenseID, long moduleIdent, long licenseCount);
		HRESULT ModuleLicenseDecrementCounterByApp(long productID, BSTR licenseID, long moduleIdent, long licenseCount);
		//HRESULT ModuleLicenseUnlimited(long product_ident, BSTR license_id, long module_ident, VARIANT_BOOL b_module_is_unlimited);

		HRESULT GetSoftwareLicenseInfoByProduct_ForAll(long productID, BSTR *pBstrProductInfoAttribsStream);
		HRESULT GetSoftwareLicenseInfo_ForAll(BSTR *pBstrLicenseInfoAttribsStream);

		HRESULT GetAllSoftwareLicenses(BSTR *pBstrLicenseListStream);
		HRESULT GetSoftwareLicenseInfoByProduct_ByLicense(BSTR softwareLicense, long productID, BSTR *pBstrProductInfoAttribsStream);
		HRESULT GetSoftwareLicenseInfo_ByLicense(BSTR softwareLicense, BSTR *pBstrLicenseInfoAttribsStream);

		HRESULT GetSoftwareLicenseStatus_ByProduct(long productID, BSTR *pBstrStringToDwordMap);
		HRESULT GetSoftwareLicenseStatus_ByLicense(BSTR softwareLicense);

		HRESULT GetSoftwareSpecByProduct(long productID, BSTR *pBstrProductSoftwareSpecAttribs);
		HRESULT GetSoftwareSpec(BSTR *pBstrSoftwareSpecAttribsStream);

		HRESULT GenerateSoftwareLicPacket(BSTR bstrLicPackageAttribsStream, VARIANT vtExpires, BSTR *pBstrVerificationCode, VARIANT* pVtLicensePacket);
		HRESULT EnterSoftwareLicPacket(VARIANT vtLicensePacket, BSTR *pBstrVerificationCode);

		HRESULT GenerateSoftwareLicArchive_ByLicense(BSTR softwareLicense, VARIANT* pVtLicArchive);
		HRESULT EnterSoftwareLicArchive(VARIANT vtLicArchive);

		HRESULT GenerateVerifyDataWithVerCode_ByLicense(BSTR softwareLicense, VARIANT* pVtLicensePacket);
		HRESULT GenerateVerifyDataWithLicInfo_ByLicense(BSTR softwareLicense, VARIANT* pVtLicensePacket);
		HRESULT GenerateLicPackage_ByVerifyData(VARIANT vtVerifyData, BSTR *pBstrLicensePackageAttribsStream);
		HRESULT GenerateLicPackage_BySoftwareLicArchive(VARIANT vtLicenseArchive, BSTR *pBstrLicensePackageAttribsStream);
		HRESULT GenerateLicPackage_BySoftwareLicPacket(VARIANT vtLicensePacket, BSTR *pBstrLicensePackageAttribsStream);
		HRESULT GenerateLicenseSystemData(VARIANT* pVtLicSysDataPacket);

		// Will generate Diagnostic Data and Mail to Solimar, only allow once per 5 minutes.
		HRESULT GenerateLicenseSystemDataForSolimar();

		HRESULT GenerateStreamData_ByLicenseSystemData(
			VARIANT vtLicSysDataPacket, 
			BSTR *pBstrCreatedDateStreamed, 
			BSTR *pBstrKeyAttribsListStream, 
			BSTR *pBstrLicUsageDataAttribsStream, 
			BSTR *pBstrConnectionAttribsListStream, 
			BSTR *pBstrEventLogAttribsListStream, 
			BSTR *pBstrLicInfoDataAttribsListStream
			);

		HRESULT GenerateStreamData_ByLicenseSystemData2(
			VARIANT vtLicSysDataPacket, 
			BSTR *pBstrCreatedDateStreamed, 
			BSTR *pBstrKeyAttribsListStream, 
			BSTR *pBstrLicUsageDataAttribsStream, 
			BSTR *pBstrConnectionAttribsListStream, 
			BSTR *pBstrEventLogAttribsListStream, 
			BSTR *pBstrLicInfoDataAttribsListStream,
			BSTR *pBstrLicAlertInfoAttribs
			);
		// Only for Interal License Servers
		HRESULT GenerateStream_ByLicenseSystemData(VARIANT vtLicSysDataPacket, BSTR *pBstrLicSysDataAttribsStream);
		HRESULT GetEventLogList_ForLicenseServer(BSTR *pBstrEventLogAttribsListStream);

		HRESULT ValidateToken_ByLicense(BSTR softwareLicense, long validationTokenType, BSTR validationValue);

		HRESULT SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber(BSTR softwareLicense, BSTR contractNumber);

		//if softwareLicense is L"", will try to add to first license file it finds, if it can't find one will create new license file.
		HRESULT ConvertProtectionKeyToSoftwareLicense(BSTR softwareLicense, BSTR keyIdent);

		// CR.17907
		HRESULT GetMailServerInfo(BSTR *pBstrAlertMailServerAttribsStream);
		HRESULT SetMailServerInfo(BSTR bstrAlertMailServerAttribsStream);
		HRESULT TestMailServerInfo(BSTR bstrTestMailServerAttribsStream);
		HRESULT GetAllEmailAlerts(BSTR *pBstrEmailAlertMailAttribsListStream);
		HRESULT GetEmailAlert(BSTR bstrEmailAlertId, BSTR *pBstrEmailAlertMailAttribsStream);
		HRESULT SetEmailAlert(BSTR bstrEmailAlertId, BSTR bstrEmailAlertMailAttribsStream);
		HRESULT AddEmailAlert(BSTR bstrEmailAlertMailAttribsStream, BSTR *pBstrEmailAlertId);
		HRESULT DeleteEmailAlert(BSTR bstrEmailAlertId);
		HRESULT SendAlertEmailIfNeeded(long productId, unsigned int eventId, std::wstring message);
		
		HRESULT GetProductIdAndApplicationInstanceByLicenseID(BSTR licenseID, int* pIntProductID, BSTR *pBstrApplicationInstance);

		HRESULT TimesUp();
		HRESULT CheckHealth(unsigned int timeout);
	private:
		HANDLE SoftwareLicenseLock;
		HANDLE SendMailLock;

		Lic_PackageAttribs::Lic_SoftwareSpecAttribs* pSoftwareSpec;		//Contains information to find out details of all modules.
		LicenseCache licCache;	//Contains the license cache of all the products
		SoftwareServerDataMgr licServerDataMgr;

		//Use a cache of the streamed version of the License Info because large License Infos take a long time to convert from an object to a stream
		std::map<std::wstring/*SW Lic Name*/, std::pair<std::wstring/*Last Touched Date*/, std::wstring/*SW Lic Streamed*/>> swLicStreamedCacheMap;

		//Use to determine if the cache sent a violation license info object or not
		std::map<std::wstring/*SW Lic Name*/, bool/*bClockViolation*/> swLicClockViolationMap;

		// CR.17907
		std::map<unsigned int, std::list<std::wstring>> eventLogIdToListofAlertIdsMap;
		HRESULT UpdateAlertEventLogIdCache(Lic_ServerDataAttribs::Lic_AlertInfoAttribs::Lic_EmailAlertMailAttribsList);
		HRESULT SendAlertEMail(SpdAttribs::VectorStringAttrib recipentsList, std::wstring subject, std::wstring body, std::wstring attachmentFile = L"");

		RainbowDriver* pRainbowDriver;
		KeyServer* pKeyServer;
		bool bFirstTime;
		time_t lastTimeEmailToSolimar;

		//variables to help with sending out messages
		//Pair used to store module ID and expiration date
		typedef std::pair<int/*ModuleID*/, time_t> ModuleIdToExpirationDate;
		typedef std::vector<ModuleIdToExpirationDate> ModuleIdToExpirationDateList;
		struct ProductReminderClass
		{
			ProductReminderClass(): closestExpDate(-1), lastReminderSent(-1), swExpDate(-1){}
			time_t closestExpDate;
			time_t lastReminderSent;
			time_t swExpDate;
			ModuleIdToExpirationDateList modIdToExpDateList;
		};

		//Map used to store expiration dates of a Product License, index by ProductID
		typedef std::map<int/*ProductID*/, ProductReminderClass> ProductReminderMap;
		struct SoftwareLicReminderClass
		{
			SoftwareLicReminderClass(): softwareLicReminderClassLastRefresh(-1){}
			ProductReminderMap prodReminderMap;
			time_t softwareLicReminderClassLastRefresh;
		};

		//Map used to store expiration dates of a Software License, index by Software License Name
		std::map<std::wstring/*SW Lic Name*/, SoftwareLicReminderClass> swLicReminderMap;
		

		//Populate productReminderMap from a given Lic_PackageAttribs::Lic_LicenseInfoAttribs
		HRESULT PopulateProductReminderMap(Lic_PackageAttribs::Lic_LicenseInfoAttribs* pLicInfoAttribs, ProductReminderMap* pProdReminderMap);
		
		//std::list<SoftwareLicenseMgr*> softwareLicMgrList;
		typedef std::map<_bstr_t, SoftwareLicenseMgr*> SoftwareLicList; //map<_bstr_t(licenseFileName, SoftwareLicenseMgr*>

		//std::map<_bstr_t, SoftwareLicenseMgr*>
		SoftwareLicList softwareLicMgrMap; 

		HRESULT ResynchronizeSoftwareLicensesInternal(bool bForceRefresh = false);

		HRESULT ApplyLicensePacketInternal(BSTR bstrLicPackageAttribsStream, _bstr_t bstrVerificationCode);
		HRESULT ConvertProtectionKeyToLicInfoAttribsInternal(ProtectionKey* pKey, _bstr_t bstrVerificationKey, Lic_PackageAttribs::Lic_LicenseInfoAttribs *pLicenseInfoAttribs);
		HRESULT MergeLicInfoAttribsInternal(Lic_PackageAttribs::Lic_LicenseInfoAttribs *pMergeFromLicenseInfoAttribs, Lic_PackageAttribs::Lic_LicenseInfoAttribs *pMergeToLicenseInfoAttribs);

		HRESULT StringToCustomerAndKeyNumberInternal(std::wstring _inputString, int* _pCustomerNumber, int* _pKeyNumber);
		HRESULT GetSoftwareLicenseInfo_ByLicenseInternal(BSTR softwareLicense, Lic_PackageAttribs::Lic_LicenseInfoAttribs *pLicenseInfoAttribs);

		//returns NULL when not found...
		SoftwareLicenseMgr* GetSoftwareLicenseMgr_ByLicenseInternal(BSTR softwareLicense);

		//HRESULT RemoveSoftwareLicense_ByLicenseInternal(BSTR softwareLicense);


		// Matches same variables in KeyServer.h
		// Maybe should change to be different values, something to think about.
		static BYTE crypto_key_password_packet_private[];
		static BYTE crypto_key_password_packet_public[];
		static BYTE crypto_key_password_packet_password[];

		static BYTE crypto_key_verify_data_private[];
		static BYTE crypto_key_verify_data_public[];
		static BYTE crypto_key_verify_data_password[];

		static BYTE crypto_key_license_archive_private[];
		static BYTE crypto_key_license_archive_public[];
		static BYTE crypto_key_license_archive_password[];

		static unsigned int license_packet_code_int[];
		static unsigned int license_archive_code_int[];
		static unsigned int license_verify_data_code_int[];
};
