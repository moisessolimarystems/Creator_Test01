#include "LicenseCache.h"
#include "..\common\LicenseError.h"
#include "..\common\LicAttribsCPP\Lic_GenericAttribs.h"
#include "..\common\LicAttribsCPP\Lic_PackageAttribsHelper.h"
#include "..\common\TimeHelper.h"


//
// Class LicenseCacheByProduct
//

LicenseCacheByProduct::LicenseCacheByProduct() :
	productID(0),
	productMaxAppInstance(0),
	productMajorVersion(0),
	productMinorVersion(0),
	pProductSpec(NULL),
	licenseUseCacheByProductLock(CreateMutex(0,0,0))
{
}
LicenseCacheByProduct::~LicenseCacheByProduct()
{
	{
	SafeMutex mutex(licenseUseCacheByProductLock);
	}
	if (licenseUseCacheByProductLock!=NULL)
	{
		CloseHandle(licenseUseCacheByProductLock);
		licenseUseCacheByProductLock = NULL;
	}
	if(pProductSpec != NULL)
		delete pProductSpec;
}

HRESULT LicenseCacheByProduct::GetCache(Lic_PackageAttribs::Lic_ProductInfoAttribs* pProdInfo)
{
	HRESULT hr(S_OK);
	SafeMutex mutex(licenseUseCacheByProductLock);

	pProdInfo->productID = productID;
	pProdInfo->productAppInstance = productMaxAppInstance;
	pProdInfo->product_Major = productMajorVersion;
	pProdInfo->product_Minor = productMinorVersion;
	for(ModuleLicenseMap::iterator modLicMapIt = licensesTotalMap.begin();
		modLicMapIt != licensesTotalMap.end();
		modLicMapIt++)
	{
		//Lic_ProductInfoAttribs::TVector_Lic_ModuleInfoAttribsList::iterator modIt = pProdInfo->moduleList->find(modLicMapIt->first);
		//if(modIt != pProdInfo->moduleList->end())
		//{
		//	modIt->moduleValue += licensesTotalMap[modIt->moduleID];
		//	modIt->moduleAppInstance += licensesAppInstanceTotalMap[modIt->moduleID];
		//}
		//else
		{
			Lic_PackageAttribs::Lic_ModuleInfoAttribs licModAttrib;
			licModAttrib.moduleID = modLicMapIt->first;
			licModAttrib.moduleValue = licensesTotalMap[modLicMapIt->first];
			licModAttrib.moduleAppInstance = licensesAppInstanceTotalMap[modLicMapIt->first];
			pProdInfo->moduleList->push_back(licModAttrib);
		}
		

	//pProdInfo.moduleList

	}
	return hr;
}
HRESULT LicenseCacheByProduct::RefreshCache(Lic_PackageAttribs::Lic_ProductInfoAttribs* pProdInfo, bool bLicSvrClockViolation)
{
	HRESULT hr(S_OK);
	SafeMutex mutex(licenseUseCacheByProductLock);

	//Should only be 1 pProdInfo for productMaxAppInstance
	//Cycle all the modules in the Lic_ProductInfoAttribs
	productMaxAppInstance = pProdInfo->productAppInstance;
	productID = pProdInfo->productID;

	//	There should only be 1 pProdInfo for product version, if not set to the lowest value seen.
	//	To implment this option properly the version should really be set on the module itself, not
	//		at the product.
	//	Another option would be to have a list of these Lic_PackageAttribs::Lic_ProductInfoAttribs* all
	//		at different versions, think about...

	if((unsigned)productMajorVersion == 0 || 
		(unsigned)productMajorVersion > pProdInfo->product_Major || 
		((unsigned)productMajorVersion == pProdInfo->product_Major && (unsigned)productMinorVersion > pProdInfo->product_Minor))
	{
		productMajorVersion = pProdInfo->product_Major;
		productMinorVersion = pProdInfo->product_Minor;
	}


	
	for(Lic_PackageAttribs::Lic_ProductInfoAttribs::TVector_Lic_ModuleInfoAttribsList::iterator modIt = pProdInfo->moduleList->begin();
		modIt != pProdInfo->moduleList->end();
		modIt++)
	{
		
		hr = S_OK;
		bool bAddToCache(true);
		try
		{
			SYSTEMTIME moduleExpiresDateSystime;
			if(!TimeHelper::StringToSystemTime(std::wstring(SpdAttribs::WStringObj(modIt->moduleExpirationDate)).c_str(), moduleExpiresDateSystime))
				throw(E_FAIL);
			
			_variant_t vtModuleExpiresDate(NULL);	
			if (!SystemTimeToVariantTime(&moduleExpiresDateSystime, &vtModuleExpiresDate.date)) 
				throw(E_FAIL);
			time_t moduleExpiresDateTimeT = TimeHelper::VariantToTimeT(vtModuleExpiresDate, false);
			time_t emptyExpiresDateTimeT = time_t(-1);

			// ignore expiration date if it equals 1900/1/1
			if(moduleExpiresDateTimeT != emptyExpiresDateTimeT)
			{
				// only add licensing to cache if the current date is below the expiration date and no lic svr clock violation
				time_t currentTimeDateTimeT = time(NULL);	//Retrieves Universal Time
				if(bLicSvrClockViolation || (currentTimeDateTimeT > moduleExpiresDateTimeT))
				{
					bAddToCache = false;
				}
			}
		}
		catch(HRESULT &eHr)
		{
			hr = eHr;
		}
		catch (...)
		{
			hr = E_FAIL;
		}

		


		//modIt->moduleExpirationDate
		if(SUCCEEDED(hr) && bAddToCache)
		{
			licensesTotalMap[modIt->moduleID] += modIt->moduleValue;
			licensesAppInstanceTotalMap[modIt->moduleID] += modIt->moduleAppInstance;
		}
	}
	return hr;
}
HRESULT LicenseCacheByProduct::RefreshSoftwareSpec(Lic_PackageAttribs::Lic_ProductSoftwareSpecAttribs* param_pProductSpec)
{


	HRESULT hr(S_OK);
	SafeMutex mutex(licenseUseCacheByProductLock);

	//Make a copy of the software product spec
	BSTR bstrStream = SysAllocString(param_pProductSpec->ToString().c_str());
	if(pProductSpec == NULL)
		pProductSpec = new Lic_PackageAttribs::Lic_ProductSoftwareSpecAttribs();
	pProductSpec->InitFromString(bstrStream);
	SysFreeString(bstrStream);

	return hr;
}
HRESULT LicenseCacheByProduct::ClearCache_Totals()
{
	HRESULT hr(S_OK);
	SafeMutex mutex(licenseUseCacheByProductLock);
	productMaxAppInstance = 0;
	licensesTotalMap.clear();
	licensesAppInstanceTotalMap.clear();
	productMajorVersion = 0;
	productMinorVersion = 0;
	return hr;
}
HRESULT LicenseCacheByProduct::AddApplicationInstance(BSTR licenseID, BSTR applicationInstance)
{
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, _countof(tmpbuf), L" LicenseCacheByProduct::AddApplicationInstance() - LicenseID: %s, AppInstance: %s", (wchar_t*)licenseID, (wchar_t*)applicationInstance);
//OutputDebugString(tmpbuf);
	HRESULT hr(LicenseServerError::EHR_LIC_PROD_NO_FREE_APP_INSTANCE);
	SafeMutex mutex(licenseUseCacheByProductLock);

	bool bNewAppInstance(true);
	bool bAddEntry(false);
	std::map<_bstr_t, int> numberOfUniqueAppInstMap;	//key is appInst, value is nothing...
	// See if appInstance is already in the list... also count unique appInstances seen
	for(LicenseToApplicationInstanceList::iterator licAppIt = licenseToAppInstList.begin();
		licAppIt != licenseToAppInstList.end();
		licAppIt++)
	{
		if(_wcsicmp(applicationInstance, licAppIt->second) == 0)
		{
			// found applicationInstance already in list
			bNewAppInstance = false;
			bAddEntry = true;
		}

		// this structure will store number of unique appInstances seen
		numberOfUniqueAppInstMap[_bstr_t(licAppIt->second, true)] = 1;
	}
//swprintf_s(tmpbuf, _countof(tmpbuf), L" LicenseCacheByProduct::AddApplicationInstance() - productMaxAppInstance: %d, numberOfUniqueAppInstMap.size(): %d", productMaxAppInstance, numberOfUniqueAppInstMap.size());
//OutputDebugString(tmpbuf);


	// See if there are enough application instances to allow this new one, if it is a new one...
	if(bNewAppInstance)
		bAddEntry = ((unsigned)productMaxAppInstance >= (numberOfUniqueAppInstMap.size() + 1));

	if(bAddEntry)
	{
		licenseToAppInstList[_bstr_t(licenseID,true)] = _bstr_t(applicationInstance,true);
		hr = S_OK;
	}

	if(SUCCEEDED(hr))	//Verfiy that all existing licenses are still good.
		hr = ((unsigned)productMaxAppInstance >= numberOfUniqueAppInstMap.size()) ? S_OK : LicenseServerError::EHR_LIC_PROD_NO_FREE_APP_INSTANCE;

	if(FAILED(hr))
	{
		wchar_t errorBuf[1024];
		swprintf_s(errorBuf, _countof(errorBuf), L"Application Instances - Licensed: %d, In Use: %d",
			productMaxAppInstance,
			numberOfUniqueAppInstMap.size()
			);
		LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(hr, std::wstring(errorBuf), __uuidof(0), __uuidof(0));
	}

	// cleanup
	numberOfUniqueAppInstMap.clear();

//swprintf_s(tmpbuf, _countof(tmpbuf), L" LicenseCacheByProduct::AddApplicationInstance() - hr: 0x%08x", hr);
//OutputDebugString(tmpbuf);
	return hr;
}
HRESULT LicenseCacheByProduct::RemoveApplicationInstance(BSTR licenseID, BSTR applicationInstance)
{
	HRESULT hr(S_OK);
	SafeMutex mutex(licenseUseCacheByProductLock);

	
	//Release all licenses that are held by this licenseID.
	//Cycle through modInUseMap and release all modules 
	//  call ModuleLicenseReleaseByApp(BSTR licenseID, long moduleIdent, long licenseCount)
	//  because it remove app instance from module if neccessary
	LicenseToModuleUseList::iterator licToModListIt = licenseToModUseList.find(_bstr_t(licenseID, true));
	if(licToModListIt != licenseToModUseList.end())
	{
		ModuleLicenseMap modInUseMap = licToModListIt->second;
		for(	ModuleLicenseMap::iterator modMapIt = modInUseMap.begin();
				modMapIt != modInUseMap.end();
				modMapIt++)
		{
			ModuleLicenseReleaseByApp(licenseID, modMapIt->first, modMapIt->second);
		}
	}

	//Remove licenseID from licenseToAppInstList...
	LicenseToApplicationInstanceList::iterator licAppIt = licenseToAppInstList.find(_bstr_t(licenseID,true));
	if(licAppIt != licenseToAppInstList.end())
		licenseToAppInstList.erase(licAppIt);


	return hr;
}
HRESULT LicenseCacheByProduct::GetApplicationInstanceList(BSTR licenseID, BSTR *pBstrListAppInstStream)
{
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L" LicenseCacheByProduct::GetApplicationInstanceList() - LicenseID: %s", (wchar_t*)licenseID);
//OutputDebugString(tmpbuf);
	HRESULT hr(S_OK);
	SafeMutex mutex(licenseUseCacheByProductLock);

	std::map<_bstr_t, int> numberOfUniqueAppInstMap;	//key is appInst, value is nothing...
	// See if appInstance is already in the list... also count unique appInstances seen
	for(LicenseToApplicationInstanceList::iterator licAppIt = licenseToAppInstList.begin();
		licAppIt != licenseToAppInstList.end();
		licAppIt++)
	{
		numberOfUniqueAppInstMap[_bstr_t(licAppIt->second, true)] = 1;	// this structure will store number of unique appInstances seen
	}

	Lic_StringListAttribs list;
	for(std::map<_bstr_t, int>::iterator uniqueLicAppIt = numberOfUniqueAppInstMap.begin();
		uniqueLicAppIt != numberOfUniqueAppInstMap.end();
		uniqueLicAppIt++)
	{
//swprintf_s(tmpbuf, 1024, L" LicenseCacheByProduct::GetApplicationInstanceList() - AppID: %s", (wchar_t*)uniqueLicAppIt->first);
//OutputDebugString(tmpbuf);

		list.strList->push_back(SpdAttribs::WStringObj(uniqueLicAppIt->first));
	}
	
	*pBstrListAppInstStream = SysAllocString(list.strList.ToString().c_str());
	return hr;
}

HRESULT LicenseCacheByProduct::ModuleLicenseTotalForAll(long moduleIdent, long* pLicenseCount)
{
	HRESULT hr(S_OK);
	SafeMutex mutex(licenseUseCacheByProductLock);

	ModuleLicenseMap::iterator modMapIt = licensesTotalMap.find(moduleIdent);
	if(modMapIt != licensesTotalMap.end())
		*pLicenseCount = modMapIt->second;
	else	// no module for total
		*pLicenseCount = 0;
	return hr;
}
HRESULT LicenseCacheByProduct::ModuleLicenseInUseForAll(long moduleIdent, long* pLicenseCount)
{
	HRESULT hr(S_OK);
	SafeMutex mutex(licenseUseCacheByProductLock);
	ModuleLicenseMap::iterator modMapIt = licensesInuseMap.find(moduleIdent);
	if(modMapIt != licensesInuseMap.end())
		*pLicenseCount = modMapIt->second;
	else	// module not inuse yet
		*pLicenseCount = 0;
	return hr;
}

//Results based on the licenseID.
//HRESULT LicenseCacheByProduct::ValidateLicense(BSTR licenseID, VARIANT_BOOL *pBLicenseValid)
//{
//	HRESULT hr(S_OK);
//	SafeMutex mutex(licenseUseCacheByProductLock);
//	return hr;
//}

//Returns the number of module licenses inuse by the ApplicationInstance the licenseID is associate with
HRESULT LicenseCacheByProduct::ModuleLicenseInUseByApp(BSTR licenseID, long moduleIdent, long* pLicenseCount)
{
	HRESULT hr(S_OK);
	SafeMutex mutex(licenseUseCacheByProductLock);

	std::list<_bstr_t> licenseIdList;
	*pLicenseCount = 0;
	try
	{
		// From a given licenseID, find its appInstanceName
		LicenseToApplicationInstanceList::iterator licAppIt = licenseToAppInstList.find(_bstr_t(licenseID,true));
		if(licAppIt == licenseToAppInstList.end())
		{
				//Unknown Application Instance...
				throw E_INVALIDARG;	//Maybe change to custom error...
		}

		_bstr_t appInst = licAppIt!=licenseToAppInstList.end() ? _bstr_t(licAppIt->second,true) : _bstr_t(L"",true);

		// Generate a list of all the licenseID with the appInstanceName
		//std::map<_bstr_t(licenseID), _bstr_t(appInstanceID)> licenseToAppInstList
		for(licAppIt = licenseToAppInstList.begin(); licAppIt!=licenseToAppInstList.end(); licAppIt++)
		{
			if(_wcsicmp(appInst, licAppIt->second) == 0)
				licenseIdList.push_back(licAppIt->first);
		}
		
		// Cycle through the list of licenseIDs, total its moduleInUse by moduleIdent	
		for(std::list<_bstr_t>::iterator licenseIdIt = licenseIdList.begin(); licenseIdIt!=licenseIdList.end(); licenseIdIt++)
		{
			LicenseToModuleUseList::iterator licToModListIt = licenseToModUseList.find(_bstr_t(*licenseIdIt, true));
			if(licToModListIt != licenseToModUseList.end())
			{
				ModuleLicenseMap::iterator modMapIt = licToModListIt->second.find(moduleIdent);
				if(modMapIt != licToModListIt->second.end())
				{
					*pLicenseCount += modMapIt->second;
				}
			}
		}
	}
	catch(HRESULT &eHr)
	{
		hr = eHr;
	}
	catch(...)
	{
		hr = E_FAIL;
	}
	licenseIdList.clear();
	return hr;
}
//Returns the number of module licenses inuse by the licenseID it is associated with
HRESULT LicenseCacheByProduct::ModuleLicenseInUseByLicenseID(BSTR licenseID, long moduleIdent, long* pLicenseCount)
{
	HRESULT hr(S_OK);
	SafeMutex mutex(licenseUseCacheByProductLock);

	*pLicenseCount = 0;
	try
	{
		LicenseToModuleUseList::iterator licToModListIt = licenseToModUseList.find(_bstr_t(licenseID, true));
		if(licToModListIt != licenseToModUseList.end())
		{
			ModuleLicenseMap::iterator modMapIt = licToModListIt->second.find(moduleIdent);
			if(modMapIt != licToModListIt->second.end())
			{
				*pLicenseCount = modMapIt->second;
			}
			else
			{
				//Unknown moduleID...
				throw E_INVALIDARG;	//Maybe change to custom error...
			}
		}
		else
		{
			//Unknown licenseID...
			throw E_INVALIDARG;	//Maybe change to custom error...
		}
	}
	catch(HRESULT &eHr)
	{
		hr = eHr;
	}
	catch(...)
	{
		hr = E_FAIL;
	}
	return hr;
}
HRESULT LicenseCacheByProduct::ModuleLicenseObtainByApp(BSTR licenseID, long moduleIdent, long licenseCount)
{
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L" LicenseCacheByProduct::ModuleLicenseObtainByApp() - LicenseID: %s, ProdID: %d, ModID: %d, Amount: %d", (wchar_t*)licenseID, (int)pProductSpec->productID, moduleIdent, licenseCount);
//OutputDebugString(tmpbuf);
	HRESULT hr(S_OK);
	SafeMutex mutex(licenseUseCacheByProductLock);

	for(;;)
	{
		LicenseToApplicationInstanceList::iterator licAppIt = licenseToAppInstList.find(_bstr_t(licenseID,true));
		if(licAppIt == licenseToAppInstList.end())
		{
			//Unknown Application Instance...
			hr = E_FAIL;	//Change to custom error...
			break;
		}

		_bstr_t appInst = licAppIt!=licenseToAppInstList.end() ? _bstr_t(licAppIt->second,true) : _bstr_t(L"",true);

//swprintf_s(tmpbuf, 1024, L" LicenseCacheByProduct::ModuleLicenseObtainByApp() - appInst: %s", (wchar_t*)appInst);
//OutputDebugString(tmpbuf);

		bool bAddToModAppInUseList(true);
		int modInUseByLicAppInst(0);

		// look to see if this appInstance is already associated with the module.
		for(ApplicationList::iterator appIt = moduleAppInstUseList[moduleIdent].begin();
			appIt != moduleAppInstUseList[moduleIdent].end();
			appIt++)
		{
			if(_wcsicmp(appInst, *appIt) == 0)
			{
				bAddToModAppInUseList = false;
				break;
			}
		}

				int licObtainAmount = licenseCount;

		//if module has an unlimited amount
		int modSpecUnlimitedValue = 0;
		std::wstring wstrModuleName = std::wstring(L"Unknown Module");
		Lic_PackageAttribs::Lic_ProductSoftwareSpecAttribs::TMap_Lic_ModuleSoftwareSpecAttribsMap::iterator modSpecIt = pProductSpec->moduleSpecMap->find(moduleIdent);
		if(modSpecIt != pProductSpec->moduleSpecMap->end())
		{
			modSpecUnlimitedValue = modSpecIt->second.modUnlimitedValue;
			wstrModuleName = modSpecIt->second.moduleName;
		}

		// need to add appInstance to the module, see if there are enough appInstance license for the module.
		if(bAddToModAppInUseList)
		{
			//NOTE: yy/mm/dd: 08/01/02 - Not valid test, rewrite this...
			//if(licensesAppInstanceTotalMap[moduleIdent] < licensesAppInstanceTotalMap.size()+1)

			if(licensesAppInstanceTotalMap[moduleIdent] < (long)(moduleAppInstUseList[moduleIdent].size()+1))
			{
				//Not enough app instances for the module
				hr = LicenseServerError::EHR_LIC_MOD_NO_FREE_APP_INSTANCE;
				//hr = E_FAIL;	//Change to custom error...
				wchar_t errorBuf[1024];
				swprintf_s(errorBuf, _countof(errorBuf), L"Module: %s (%d) - Application Instances - Licensed: %d, In Use: %d",
					wstrModuleName.c_str(),
					moduleIdent,
					licensesAppInstanceTotalMap[moduleIdent],
					moduleAppInstUseList[moduleIdent].size()
					);
				LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(hr, std::wstring(errorBuf), __uuidof(0), __uuidof(0));
				break;
			}
		}

		if(modSpecUnlimitedValue != 0)
		{
			// calculate the module inuse amount of all the licID with the given appID, 
			// value might near Unlimited.
			int currentModInuseByAppInst(0);
			long tmpLicenseCount = 0;
			for(LicenseToApplicationInstanceList::iterator licIdToAppIdIt = licenseToAppInstList.begin();
				licIdToAppIdIt != licenseToAppInstList.end();
				licIdToAppIdIt++)
			{
				if(_wcsicmp(appInst, licIdToAppIdIt->second) == 0)
				{
					hr = ModuleLicenseInUseByApp(licIdToAppIdIt->first, moduleIdent, &tmpLicenseCount);
					if(SUCCEEDED(hr))
						currentModInuseByAppInst += tmpLicenseCount;
					else	//Returns FAILED(hr) if nothing is found
						hr = S_OK;
				}
			}

			// look at software spec, if currentModInuseByAppInst is at or near unlimited amount
			licObtainAmount = min( max(modSpecUnlimitedValue-currentModInuseByAppInst, 0), licObtainAmount);
		}
		

		// check to see if there are enough licenses
		if(licensesTotalMap[moduleIdent] < licensesInuseMap[moduleIdent]+licObtainAmount)
		{
			//Not enough licenses for the module
			hr = LicenseServerError::EHR_LICENSE_INSUFFICIENT;
			wchar_t errorBuf[1024];
			swprintf_s(errorBuf, _countof(errorBuf), L"Module: %s (%d) - Licensed: %d, In Use: %d, Amount to Obtain: %d ",
					wstrModuleName.c_str(),
					moduleIdent,
					licensesTotalMap[moduleIdent],
					licensesInuseMap[moduleIdent],
					licObtainAmount
					);
			LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(hr, std::wstring(errorBuf), __uuidof(0), __uuidof(0));
			break;
		}

		//
		// enough licenses, obtain from all the maps
		if(bAddToModAppInUseList)	//add to appInst for given modules.
			moduleAppInstUseList[moduleIdent].push_back(ApplicationList::value_type(appInst));
		licensesInuseMap[moduleIdent] += licObtainAmount;
		licenseToModUseList[_bstr_t(licenseID, true)][moduleIdent] += licenseCount;

//swprintf_s(tmpbuf, 1024, L" LicenseCacheByProduct::ModuleLicenseObtainByApp() - bAddToModAppInUseList: %d, licObtainAmount: %d, licensesInuseMap[moduleIdent]: %d, licenseCount: %d, licenseToModUseList[_bstr_t(licenseID, true)][moduleIdent]: %d", bAddToModAppInUseList, licObtainAmount, licensesInuseMap[moduleIdent], licenseCount, licenseToModUseList[_bstr_t(licenseID, true)][moduleIdent]);
//OutputDebugString(tmpbuf);


		break;	//Unconditional Break
	}
//swprintf_s(tmpbuf, 1024, L" LicenseCacheByProduct::ModuleLicenseObtainByApp() - hr: 0x%08x", hr);
//OutputDebugString(tmpbuf);
	return hr;
}
HRESULT LicenseCacheByProduct::ModuleLicenseReleaseByApp(BSTR licenseID, long moduleIdent, long licenseCount)
{
	HRESULT hr(S_OK);
	SafeMutex mutex(licenseUseCacheByProductLock);
	for(;;)
	{
		LicenseToApplicationInstanceList::iterator licAppIt = licenseToAppInstList.find(_bstr_t(licenseID,true));
		if(licAppIt == licenseToAppInstList.end())
		{
			//Unknown Application Instance...
			hr = E_FAIL;	//Change to custom error...
			break;
		}

		_bstr_t appInst = licAppIt!=licenseToAppInstList.end() ? _bstr_t(licAppIt->second,true) : _bstr_t(L"",true);

		//Don't release more licenses than this licenseID has obtained
		int modReleaseAmount_ByLicID = min(licenseToModUseList[_bstr_t(licenseID, true)][moduleIdent], licenseCount);
		int modReleaseAmount = modReleaseAmount_ByLicID;

		//if module has an unlimited amount
		int modSpecUnlimitedValue = 0;
		Lic_PackageAttribs::Lic_ProductSoftwareSpecAttribs::TMap_Lic_ModuleSoftwareSpecAttribsMap::iterator modSpecIt = pProductSpec->moduleSpecMap->find(moduleIdent);
		if(modSpecIt != pProductSpec->moduleSpecMap->end())
			modSpecUnlimitedValue = modSpecIt->second.modUnlimitedValue;

		if(modSpecUnlimitedValue != 0)
		{
			// CR.FIX.11869.Item14 - calculate the module inuse amount of all the licID with the given appID, value might near Unlimited.
			long currentModInuseByAppInst(0);
			ModuleLicenseInUseByApp(licenseID, moduleIdent, &currentModInuseByAppInst);

			// look at software spec, if currentModInuseByAppInst is at or near unlimited amount
			if(currentModInuseByAppInst == 0)
			{
				//Nothing is obtained
				modReleaseAmount = 0;
			}
			else if(currentModInuseByAppInst<modReleaseAmount)
			{
				modReleaseAmount = min(currentModInuseByAppInst, modSpecUnlimitedValue);
			}
			else if((currentModInuseByAppInst-modReleaseAmount) > modSpecUnlimitedValue)
			{
				//module is still has enough licenses to be unlimited, release nothing...
				modReleaseAmount = 0;
			}
			else if	( (currentModInuseByAppInst>modSpecUnlimitedValue) && 
						((currentModInuseByAppInst-modReleaseAmount) <= modSpecUnlimitedValue))
			{
				//current amount is above unlimited, but after release amount will be less than unlimited
				modReleaseAmount = modSpecUnlimitedValue - (currentModInuseByAppInst - modReleaseAmount);
			}
			else //if( (currentModInuseByAppInst>modReleaseAmount) && [(currentModInuseByAppInst-modReleaseAmount) <= modSpecUnlimitedValue]
			{
				//I don't know what case this is...
				//modReleaseAmount = modSpecUnlimitedValue - (currentModInuseByAppInst - modReleaseAmount);
			}
		}

		//Update all lists
		licensesInuseMap[moduleIdent] -= modReleaseAmount;
		licenseToModUseList[_bstr_t(licenseID, true)][moduleIdent] -= modReleaseAmount_ByLicID;
		RefreshApplicationInstancesUsedByModule(moduleIdent);
		break;	//Unconditional Break
	}
	return hr;
}


HRESULT LicenseCacheByProduct::RefreshApplicationInstancesUsedByModule(long moduleIdent)
{
	HRESULT hr(S_OK);

	

	ModuleApplicationUseList::iterator modAppInstIt = moduleAppInstUseList.find(moduleIdent);
	if(modAppInstIt != moduleAppInstUseList.end())
	{
		ApplicationList appInstToRemoveList;

		//walk through all appInst associated with the module.
		for(ApplicationList::iterator appInstIt = modAppInstIt->second.begin();
			appInstIt != modAppInstIt->second.end();
			appInstIt++)
		{
			std::list<_bstr_t> licenseIdList;
			hr = GetListOfLicenseID(*appInstIt, &licenseIdList);
			if(SUCCEEDED(hr))
			{
				long moduleInUseByAppInst(0);
				for(std::list<_bstr_t>::iterator licIdIt = licenseIdList.begin();
					licIdIt != licenseIdList.end();
					licIdIt++)
				{
					LicenseToModuleUseList::iterator licToModListIt = licenseToModUseList.find(*licIdIt);
					if(licToModListIt != licenseToModUseList.end())
					{
						ModuleLicenseMap::iterator modMapIt = licToModListIt->second.find(moduleIdent);
						if(modMapIt != licToModListIt->second.end())
							moduleInUseByAppInst += modMapIt->second;
					}
				}

				if(moduleInUseByAppInst == 0)
				{
					//Remove the appInst from moduleAppInstUseList
					appInstToRemoveList.push_back(*appInstIt);
				}
			}
		}

		//remove all unused app instances with the module.
		for(ApplicationList::iterator appToRemoveInstIt = appInstToRemoveList.begin();
			appToRemoveInstIt != appInstToRemoveList.end();
			appToRemoveInstIt++)
		{
			for(ApplicationList::iterator appInstIt = modAppInstIt->second.begin();
				appInstIt != modAppInstIt->second.end();
				appInstIt++)
			{
				if(_wcsicmp(*appInstIt, *appToRemoveInstIt) == 0)
				{
					modAppInstIt->second.erase(appInstIt);
					break;
				}
			}
		}
	}
	return hr;
}

HRESULT LicenseCacheByProduct::GetListOfLicenseID(_bstr_t applicationInstance, std::list<_bstr_t>* pLicenseList)
{
	HRESULT hr(S_OK);
	for(LicenseToApplicationInstanceList::iterator licAppIt = licenseToAppInstList.begin();
		licAppIt != licenseToAppInstList.end();
		licAppIt++)
	{
		if(_wcsicmp(applicationInstance, licAppIt->second) == 0)
			pLicenseList->push_back(licAppIt->first);
	}
	return hr;
}

HRESULT LicenseCacheByProduct::ModuleLicenseDecrementCounterByApp(BSTR licenseID, long moduleIdent, long licenseCount)
{
	HRESULT hr(S_OK);
	SafeMutex mutex(licenseUseCacheByProductLock);
	return hr;
}
//
// Class LicenseCache
//
LicenseCache::LicenseCache():
	licenseUseCacheLock(CreateMutex(0,0,0))
{
}
LicenseCache::~LicenseCache()
{
	if (licenseUseCacheLock!=NULL)
	{
		CloseHandle(licenseUseCacheLock);
		licenseUseCacheLock = NULL;
	}
}

HRESULT LicenseCache::AddApplicationInstance(long productID, BSTR licenseID, BSTR applicationInstance)
{
	SafeMutex mutex(licenseUseCacheLock);
	HRESULT hr(S_OK);
	LicenseCacheByProductMap::iterator prodCacheMapIt = productCacheMap.find(productID);
	if(prodCacheMapIt != productCacheMap.end())
		hr = prodCacheMapIt->second->AddApplicationInstance(licenseID, applicationInstance);
	else
		hr = E_INVALIDARG;

	return hr;
}

//if productID = -1, remove licenseID from all products
HRESULT LicenseCache::RemoveApplicationInstance(long productID, BSTR licenseID, BSTR applicationInstance)
{
	SafeMutex mutex(licenseUseCacheLock);
	HRESULT hr(S_OK);

	if(productID == -1)
	{
		for(	LicenseCacheByProductMap::iterator prodCacheMapIt= productCacheMap.begin();
				prodCacheMapIt != productCacheMap.end();
				prodCacheMapIt++)
		{
			hr = prodCacheMapIt->second->RemoveApplicationInstance(licenseID, applicationInstance);
		}
		//Don't use hr, chance is licenseID probably will not be in the cache maps.
		hr = S_OK;
	}
	else
	{
		LicenseCacheByProductMap::iterator prodCacheMapIt = productCacheMap.find(productID);
		if(prodCacheMapIt != productCacheMap.end())
			hr = prodCacheMapIt->second->RemoveApplicationInstance(licenseID, applicationInstance);
		else
			hr = E_INVALIDARG;
	}
	return hr;
}
HRESULT LicenseCache::GetApplicationInstanceList(long productID, BSTR licenseID, BSTR *pBstrListAppInstStream)
{
	SafeMutex mutex(licenseUseCacheLock);
	HRESULT hr(S_OK);
	LicenseCacheByProductMap::iterator prodCacheMapIt = productCacheMap.find(productID);
	if(prodCacheMapIt != productCacheMap.end())
		hr = prodCacheMapIt->second->GetApplicationInstanceList(licenseID, pBstrListAppInstStream);
	else
		hr = E_INVALIDARG;
	return hr;
}


HRESULT LicenseCache::RefreshCache(std::list<Lic_PackageAttribs::Lic_LicenseInfoAttribs*>* pLicInfoList, bool bLicSvrClockViolation)
{
	HRESULT hr(S_OK);

	SafeMutex mutex(licenseUseCacheLock);

	//Clear totals for current cache...
	for(LicenseCacheByProductMap::iterator prodCacheMapIt = productCacheMap.begin();
		prodCacheMapIt != productCacheMap.end();
		prodCacheMapIt++)
	{
		prodCacheMapIt->second->ClearCache_Totals();
	}

	for(	std::list<Lic_PackageAttribs::Lic_LicenseInfoAttribs*>::iterator licInfoAttribIt = pLicInfoList->begin();
			licInfoAttribIt != pLicInfoList->end();
			licInfoAttribIt++)
	{
		SYSTEMTIME activationExpirationDateSystime;
		if(!TimeHelper::StringToSystemTime(std::wstring(SpdAttribs::WStringObj((*licInfoAttribIt)->activationExpirationDate)).c_str(), activationExpirationDateSystime))
			continue;
		_variant_t vtActivationExpirationDate(NULL);	
		if (!SystemTimeToVariantTime(&activationExpirationDateSystime, &vtActivationExpirationDate.date)) 
			continue;
		time_t activationExpirationDateTimeT = TimeHelper::VariantToTimeT(vtActivationExpirationDate, false);
		time_t emptyExpiresDateTimeT = time_t(-1);

		// if expiration date == 1900/1/1 && total == 0 && activation in days == 0, this is standard licensing, non licensing activation or expiring type
		bool bExpiringLicense = (!((*licInfoAttribIt)->activationTotal == 0 && 
			(*licInfoAttribIt)->activationAmountInDays == 0 &&
			activationExpirationDateTimeT == emptyExpiresDateTimeT));

		//If in bLicSvrClockViolation, only add product licenses to cache if the license info is a non-expiring type
		if(!bLicSvrClockViolation || (bLicSvrClockViolation && !bExpiringLicense))
		{
			for(Lic_PackageAttribs::Lic_LicenseInfoAttribs::TVector_Lic_ProductInfoAttribsList::iterator prodIt = (*licInfoAttribIt)->productList->begin();
				prodIt != (*licInfoAttribIt)->productList->end();
				prodIt++)
			{
				//Add new item if needed.
				if(productCacheMap.find((*prodIt).productID) == productCacheMap.end())
					productCacheMap[(*prodIt).productID] = new LicenseCacheByProduct();
					//productCacheMap.insert(std::map<int,LicenseCacheByProduct*>::value_type((*prodIt).productID), new LicenseCacheByProduct()));

				productCacheMap[(*prodIt).productID]->RefreshCache(&(*prodIt), bLicSvrClockViolation);
				//productCacheMap[(*prodIt).productID].RefreshCache(&(*prodIt));

				//LicenseCacheByProductMap::iterator prodCacheMapIt = productCacheMap.find((*prodIt).productID);
				//if(prodCacheMapIt != productCacheMap.end())	//Found existing product
				//{
				//	hr = prodCacheMapIt->second->RefreshCache(*prodIt);
				//}
				//else	//add new product
				//{
				//}
				
				//int x = (*prodIt).productID;
				//productCacheMap[(*prodIt).productID].RefreshCache(*prodIt);
			}
		}

		//Lic_LicenseInfoAttribs* pTmp = *licInfoAttribIt;
		//pTmp->productList->size();
		//pTmp->
		//(*licInfoAttribIt)->productList
		//for(TVector_Lic_ProductInfoAttribsList prodIt = (*licInfoAttribIt)->productList.begin();
		//	prodIt != (*licInfoAttribIt)->productList.end();
		//	prodIt++)
		//{
		//}
		//RefreshCache
		//look up (*licInfoAttribIt)->prodID in the map for the LicenseCacheByProduct,
		//call [LicenseCacheByProduct]->RefreshCache(<Lic_ProductInfoAttribs*>)
	}

	return hr;
}

HRESULT LicenseCache::GetCache_ByProduct(long productID, Lic_PackageAttribs::Lic_ProductInfoAttribs* pProdInfo)
{
	HRESULT hr(S_OK);
	SafeMutex mutex(licenseUseCacheLock);

	//LicenseCacheByProductMap::iterator prodCacheMapIt = productCacheMap.begin();
	LicenseCacheByProductMap::iterator prodCacheMapIt = productCacheMap.find(productID);
	if(prodCacheMapIt != productCacheMap.end())
		hr = prodCacheMapIt->second->GetCache(pProdInfo);

	return hr;
}
HRESULT LicenseCache::GetCache(Lic_PackageAttribs::Lic_LicenseInfoAttribs* pLicInfo)
{
	HRESULT hr(S_OK);
	SafeMutex mutex(licenseUseCacheLock);

	for(LicenseCacheByProductMap::iterator prodCacheMapIt = productCacheMap.begin();
		prodCacheMapIt != productCacheMap.end();
		prodCacheMapIt++)
	{
		Lic_PackageAttribs::Lic_ProductInfoAttribs prodInfoAttibs;
		hr = prodCacheMapIt->second->GetCache(&prodInfoAttibs);
		if(FAILED(hr))
			continue;
		pLicInfo->productList->push_back(prodInfoAttibs);
	}
	return hr;
}

HRESULT LicenseCache::RefreshSoftwareSpec(Lic_PackageAttribs::Lic_SoftwareSpecAttribs* pSoftwareSpec)
{
	HRESULT hr(S_OK);

	SafeMutex mutex(licenseUseCacheLock);
	for(Lic_PackageAttribs::Lic_SoftwareSpecAttribs::TMap_Lic_ProductSoftwareSpecAttribsMap::iterator prodSpecMapIt = pSoftwareSpec->productSpecMap->begin();
		prodSpecMapIt != pSoftwareSpec->productSpecMap->end();
		prodSpecMapIt++)
	{
		if(productCacheMap.find((int)prodSpecMapIt->first) == productCacheMap.end()) //Create new one if neccessary
			productCacheMap[(int)prodSpecMapIt->first] = new LicenseCacheByProduct();

		//Test/Dev products don't have a module list, so they must be set manually.
		Lic_PackageAttribs::Lic_ProductSoftwareSpecAttribs tmpProdSpec;
		tmpProdSpec = prodSpecMapIt->second;
		tmpProdSpec.moduleSpecMap = Lic_PackageAttribsHelper::GetModuleList(pSoftwareSpec, (int)prodSpecMapIt->first);
		productCacheMap[(int)prodSpecMapIt->first]->RefreshSoftwareSpec(&tmpProdSpec);
	}

	//for(LicenseCacheByProductMap::iterator prodCacheMapIt = productCacheMap.begin();
	//	prodCacheMapIt != productCacheMap.end();
	//	prodCacheMapIt++)
	//{
	//	Lic_SoftwareSpecAttribs::TMap_Lic_ProductSoftwareSpecAttribsMap::iterator prodSpecIt = pSoftwareSpec->productSpecMap->find(prodCacheMapIt->first);

	//	if(productCacheMap.find((*prodIt).productID) == productCacheMap.end()) //Create new one if neccessary
	//		productCacheMap[(*prodIt).productID] = new LicenseCacheByProduct();

	//	productCacheMap[(*prodIt).productID]->RefreshSoftwareSpec(&(*prodIt));

	//	//if(prodSpecIt != pSoftwareSpec->productSpecMap->end())
	//	//	prodCacheMapIt->second->RefreshSoftwareSpec(&prodSpecIt->second);

	//	//prodCacheMapIt->second->RefreshCache();
	////productCacheMap
	//}


	return hr;
}


HRESULT LicenseCache::ModuleEnumerate(long productID, VARIANT *pvtModuleList)
{
	return E_NOTIMPL;
}
HRESULT LicenseCache::ModuleQuery(long productID, long module_ident, VARIANT *vtValue)
{
	return E_NOTIMPL;
}

HRESULT LicenseCache::ModuleLicenseTotalForAll(long productID, long moduleIdent, long* pLicenseCount)
{
	SafeMutex mutex(licenseUseCacheLock);
	HRESULT hr(S_OK);
	LicenseCacheByProductMap::iterator prodCacheMapIt = productCacheMap.find(productID);
	if(prodCacheMapIt != productCacheMap.end())
		hr = prodCacheMapIt->second->ModuleLicenseTotalForAll(moduleIdent, pLicenseCount);
	else
		hr = E_INVALIDARG;
	return hr;
}
HRESULT LicenseCache::ModuleLicenseInUseForAll(long productID, long moduleIdent, long* pLicenseCount)
{
	SafeMutex mutex(licenseUseCacheLock);
	HRESULT hr(S_OK);
	LicenseCacheByProductMap::iterator prodCacheMapIt = productCacheMap.find(productID);
	if(prodCacheMapIt != productCacheMap.end())
		hr = prodCacheMapIt->second->ModuleLicenseInUseForAll(moduleIdent, pLicenseCount);
	else
		hr = E_INVALIDARG;
	return hr;
}

//Results based on the licenseID.
//HRESULT LicenseCache::ValidateLicense(long productID, BSTR licenseID, VARIANT_BOOL *pBLicenseValid)
//{
//	SafeMutex mutex(licenseUseCacheLock);
//	HRESULT hr(S_OK);
//	LicenseCacheByProductMap::iterator prodCacheMapIt = productCacheMap.find(productID);
//	if(prodCacheMapIt != productCacheMap.end())
//		hr = prodCacheMapIt->second->ValidateLicense(licenseID, pBLicenseValid);
//	else
//		hr = E_INVALIDARG;
//	return hr;
//}
HRESULT LicenseCache::ModuleLicenseInUseByApp(long productID, BSTR licenseID, long moduleIdent, long* pLicenseCount)
{
	SafeMutex mutex(licenseUseCacheLock);
	HRESULT hr(S_OK);
	LicenseCacheByProductMap::iterator prodCacheMapIt = productCacheMap.find(productID);
	if(prodCacheMapIt != productCacheMap.end())
		hr = prodCacheMapIt->second->ModuleLicenseInUseByApp(licenseID, moduleIdent, pLicenseCount);
	else
		hr = E_INVALIDARG;
	return hr;
}
HRESULT LicenseCache::ModuleLicenseInUseByLicenseID(long productID, BSTR licenseID, long moduleIdent, long* pLicenseCount)
{
	SafeMutex mutex(licenseUseCacheLock);
	HRESULT hr(S_OK);
	LicenseCacheByProductMap::iterator prodCacheMapIt = productCacheMap.find(productID);
	if(prodCacheMapIt != productCacheMap.end())
		hr = prodCacheMapIt->second->ModuleLicenseInUseByLicenseID(licenseID, moduleIdent, pLicenseCount);
	else
		hr = E_INVALIDARG;
	return hr;
}
HRESULT LicenseCache::ModuleLicenseObtainByApp(long productID, BSTR licenseID, long moduleIdent, long licenseCount)
{
	SafeMutex mutex(licenseUseCacheLock);
	HRESULT hr(S_OK);
	LicenseCacheByProductMap::iterator prodCacheMapIt = productCacheMap.find(productID);
	if(prodCacheMapIt != productCacheMap.end())
		hr = prodCacheMapIt->second->ModuleLicenseObtainByApp(licenseID, moduleIdent, licenseCount);
	else
		hr = E_INVALIDARG;
	return hr;

}
HRESULT LicenseCache::ModuleLicenseReleaseByApp(long productID, BSTR licenseID, long moduleIdent, long licenseCount)
{
	SafeMutex mutex(licenseUseCacheLock);
	HRESULT hr(S_OK);
	LicenseCacheByProductMap::iterator prodCacheMapIt = productCacheMap.find(productID);
	if(prodCacheMapIt != productCacheMap.end())
		hr = prodCacheMapIt->second->ModuleLicenseReleaseByApp(licenseID, moduleIdent, licenseCount);
	else
		hr = E_INVALIDARG;
	return hr;
}
HRESULT LicenseCache::ModuleLicenseDecrementCounterByApp(long productID, BSTR licenseID, long moduleIdent, long licenseCount)
{
	SafeMutex mutex(licenseUseCacheLock);
	HRESULT hr(S_OK);
	LicenseCacheByProductMap::iterator prodCacheMapIt = productCacheMap.find(productID);
	if(prodCacheMapIt != productCacheMap.end())
		hr = prodCacheMapIt->second->ModuleLicenseDecrementCounterByApp(licenseID, moduleIdent, licenseCount);
	else
		hr = E_INVALIDARG;
	return hr;
}
