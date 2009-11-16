#pragma once
#include <comdef.h>
#include <map>
#include <list>
#include "..\Common\SafeMutex.h"
#include "..\Common\LicAttribsCPP\Lic_PackageAttribs.h"


class LicenseCacheByProduct
{
	public:
		LicenseCacheByProduct();
		~LicenseCacheByProduct();

		HRESULT GetCache(Lic_PackageAttribs::Lic_ProductInfoAttribs* pProdInfo);

		// bLicSvrClockViolation is true when the current system time is before the last touch time, user has set their system clock backwards in time.
		HRESULT RefreshCache(Lic_PackageAttribs::Lic_ProductInfoAttribs* pProdInfo, bool bLicSvrClockViolation);
		HRESULT RefreshSoftwareSpec(Lic_PackageAttribs::Lic_ProductSoftwareSpecAttribs* pSoftwareSpec);
		HRESULT ClearCache_Totals();

		//LicenseCacheByProduct(int productID);

		//int GetProductID() {return m_productID;}
		HRESULT AddApplicationInstance(BSTR licenseID, BSTR applicationInstance);

		//Side Effect: will free all the licenses obtained to the licenseID.
		HRESULT RemoveApplicationInstance(BSTR licenseID, BSTR applicationInstance);
		HRESULT GetApplicationInstanceList(BSTR licenseID, BSTR *pBstrListAppInstStream);

		HRESULT ModuleLicenseTotalForAll(long moduleIdent, long* pLicenseCount);
		HRESULT ModuleLicenseInUseForAll(long moduleIdent, long* pLicenseCount);

		//Results based on the licenseID.
		//HRESULT ValidateLicense(BSTR licenseID, VARIANT_BOOL *pBLicenseValid);
		HRESULT ModuleLicenseObtainByApp(BSTR licenseID, long moduleIdent, long licenseCount);
		HRESULT ModuleLicenseReleaseByApp(BSTR licenseID, long moduleIdent, long licenseCount);
		HRESULT ModuleLicenseDecrementCounterByApp(BSTR licenseID, long moduleIdent, long licenseCount);

		//Returns the number of module licenses inuse by the ApplicationInstance the licenseID is associate with
		HRESULT ModuleLicenseInUseByApp(BSTR licenseID, long moduleIdent, long* pLicenseCount);

		//Returns the number of module licenses inuse by the licenseID it is associated with
		HRESULT ModuleLicenseInUseByLicenseID(BSTR licenseID, long moduleIdent, long* pLicenseCount);
		


	private:
		int productID;
		int productMaxAppInstance;
		int productMajorVersion;
		int productMinorVersion;

		typedef std::map<long, long> ModuleLicenseMap;
		typedef std::list<_bstr_t> ApplicationList;                                // list of application names
		typedef std::map<unsigned int, ApplicationList> ModuleApplicationUseList;	// map of modules and which application instances are associated
		typedef std::map<_bstr_t, _bstr_t> LicenseToApplicationInstanceList;			// maps license identifier guid to application instance names
		typedef std::map<_bstr_t, ModuleLicenseMap> LicenseToModuleUseList;			// maps license identifier guid to modules it is using
		
		ModuleApplicationUseList moduleAppInstUseList;			// map<long[modID], list<appInst>> - List of AppInstances for a given modID
		LicenseToApplicationInstanceList licenseToAppInstList;// map<bstr[licID], bstr[appInst]> - multiple licID can all share the same appInst...
		LicenseToModuleUseList licenseToModUseList;				// map<bstr[licID], map<long[[modID], long[modInUse]>>
		
		ModuleLicenseMap licensesAppInstanceTotalMap;	// map<long[modID], long[appInstForTheModule]>
		ModuleLicenseMap licensesTotalMap;					// map<long[modID], long[modLicTotal]>
		ModuleLicenseMap licensesInuseMap;					// map<long[modID], long[modInUse]>


		//Refreshs moduleAppInstUseList, references licenseToModUseList to determin if an application instance is 
		//still associated with a module.
		HRESULT RefreshApplicationInstancesUsedByModule(long moduleIdent);

		//Given an applicationInstance, returns a list of licenseID that all share that applicationInstance
		HRESULT GetListOfLicenseID(_bstr_t applicationInstance, std::list<_bstr_t>* pLicenseList);

		//ModuleLicenseMap licensesAllocatedMap;				//
		
		Lic_PackageAttribs::Lic_ProductSoftwareSpecAttribs* pProductSpec;
		HANDLE licenseUseCacheByProductLock;
};

class LicenseCache
{
	public:
		LicenseCache();
		~LicenseCache();
		HRESULT AddApplicationInstance(long productID, BSTR licenseID, BSTR applicationInstance);

		
		//Side Effect: will free all the licenses obtained to the licenseID for a given productID
		//if productID = -1, remove licenseID from all products
		HRESULT RemoveApplicationInstance(long productID, BSTR licenseID, BSTR applicationInstance);
		HRESULT GetApplicationInstanceList(long productID, BSTR licenseID, BSTR *pBstrListAppInstStream);
		

		HRESULT RefreshCache(std::list<Lic_PackageAttribs::Lic_LicenseInfoAttribs*>* pLicInfoList, bool bLicSvrClockViolation);
		HRESULT GetCache_ByProduct(long productID, Lic_PackageAttribs::Lic_ProductInfoAttribs* pProdInfo);
		HRESULT GetCache(Lic_PackageAttribs::Lic_LicenseInfoAttribs* pLicInfo);
		HRESULT RefreshSoftwareSpec(Lic_PackageAttribs::Lic_SoftwareSpecAttribs* pSoftwareSpec);

		HRESULT ModuleEnumerate(long productID, VARIANT *pvtModuleList);
		HRESULT ModuleQuery(long productID, long module_ident, VARIANT *vtValue);

		HRESULT ModuleLicenseTotalForAll(long productID, long moduleIdent, long* pLicenseCount);
		HRESULT ModuleLicenseInUseForAll(long productID, long moduleIdent, long* pLicenseCount);

		//Results based on the licenseID.
		//HRESULT ValidateLicense(long productID, BSTR licenseID, VARIANT_BOOL *pBLicenseValid);
		HRESULT ModuleLicenseInUseByApp(long productID, BSTR licenseID, long moduleIdent, long* pLicenseCount);
		HRESULT ModuleLicenseInUseByLicenseID(long productID, BSTR licenseID, long moduleIdent, long* pLicenseCount);
		HRESULT ModuleLicenseObtainByApp(long productID, BSTR licenseID, long moduleIdent, long licenseCount);
		HRESULT ModuleLicenseReleaseByApp(long productID, BSTR licenseID, long moduleIdent, long licenseCount);
		HRESULT ModuleLicenseDecrementCounterByApp(long productID, BSTR licenseID, long moduleIdent, long licenseCount);

	private:
		HANDLE licenseUseCacheLock;
		typedef std::map<int,LicenseCacheByProduct*> LicenseCacheByProductMap; //map<int(productID), LicenseCacheByProduct>

		LicenseCacheByProductMap productCacheMap;
};








