#ifndef __Lic_PackageAttribsHelperAttribs_h__
#define __Lic_PackageAttribsHelperAttribs_h__

#include "Lic_PackageAttribs.h"

class Lic_PackageAttribsHelper
{
	public:
	static std::wstring GetDisplayLabel(Lic_PackageAttribs::Lic_LicenseInfoAttribs* _pLicInfoAttribs)
	{
		wchar_t wcstrDiplayLabel[1024];
		wchar_t wcstrDestinationID[5];
		wchar_t wcstrSwGroupID[5];
		if(_pLicInfoAttribs)
		{
			wchar_t wLetter = L'U';
			switch(_pLicInfoAttribs->softwareLicType)
			{
				case Lic_PackageAttribs::Lic_LicenseInfoAttribs::sltFailover:
					wLetter = L'F';
					break;
				//case Lic_PackageAttribs::Lic_LicenseInfoAttribs::sltDisasterRecovery:
				//	wLetter = L'D';
				//	break;
				//case Lic_PackageAttribs::Lic_LicenseInfoAttribs::sltPerpetual:
				//	wLetter = L'P';
				//	break;
				//case Lic_PackageAttribs::Lic_LicenseInfoAttribs::sltSubscription:
				//	wLetter = L'S';
				//	break;
				//case Lic_PackageAttribs::Lic_LicenseInfoAttribs::sltTestDev:
				//	wLetter = L'T';
				//	break;
				//default:
				//	wLetter = L'U';
				//	break;
				default:
					wLetter = L'S';
					break;
			}

			_itow_s(_pLicInfoAttribs->destinationID, wcstrDestinationID, 36);
			_itow_s(_pLicInfoAttribs->softwareGroupLicenseID, wcstrSwGroupID, 36);
			for(int idx=0;idx<_countof(wcstrDestinationID);idx++)
				wcstrDestinationID[idx] = towupper(wcstrDestinationID[idx]);
			for(int idx=0;idx<_countof(wcstrSwGroupID);idx++)
				wcstrSwGroupID[idx] = towupper(wcstrSwGroupID[idx]);
			
			_snwprintf_s(
				wcstrDiplayLabel,
				sizeof(wcstrDiplayLabel)/sizeof(wchar_t),
				sizeof(wcstrDiplayLabel)/sizeof(wchar_t),
				L"%03x-%02s-%02s-%c",
				(int)_pLicInfoAttribs->customerID,
				wcstrDestinationID,
				wcstrSwGroupID,
				wLetter
				);
			//_snwprintf_s(
			//	wcstrDiplayLabel,
			//	sizeof(wcstrDiplayLabel)/sizeof(wchar_t),
			//	sizeof(wcstrDiplayLabel)/sizeof(wchar_t),
			//	L"%03x-%02s-%02s-%c%d",
			//	(int)_pLicInfoAttribs->customerID,
			//	wcstrDestinationID,
			//	wcstrSwGroupID,
			//	wLetter,
			//	(int)_pLicInfoAttribs->softwareLicTypeIndex
			//	);
			//_snwprintf_s(
			//	wcstrDiplayLabel,
			//	sizeof(wcstrDiplayLabel)/sizeof(wchar_t),
			//	sizeof(wcstrDiplayLabel)/sizeof(wchar_t),
			//	L"%04x-%03x-%04x-%c%02d",
			//	(int)_pLicInfoAttribs->customerID,
			//	(int)_pLicInfoAttribs->destinationID,
			//	(int)_pLicInfoAttribs->softwareGroupLicenseID,
			//	wLetter,
			//	(int)_pLicInfoAttribs->softwareLicTypeIndex
			//	);
		}
		return std::wstring(wcstrDiplayLabel);
	}


	static std::wstring GetValidationValue(Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_VerificationAttribs::Lic_ValidationTokenAttribsList* _pLicVerTokAttribsList, Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::TTokenType _tokenType)
	{
		std::wstring wTokenValue = std::wstring(L"");
		if(_pLicVerTokAttribsList)
		{
			for(	std::vector<Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs>::iterator valTokenIT = (*_pLicVerTokAttribsList)->begin();
						valTokenIT != (*_pLicVerTokAttribsList)->end();
						valTokenIT++)
			{
				if((Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::TTokenType)valTokenIT->tokenType == _tokenType)
				{
					wTokenValue = std::wstring(valTokenIT->tokenValue->c_str());
					break;
				}
			}
		}
		return wTokenValue;
	}


	static std::wstring GetProductName(Lic_PackageAttribs::Lic_SoftwareSpecAttribs* param_pSoftwareSpec, int paramProductID)
	{
		std::wstring wProductName = std::wstring(L"Unknown Product");
		for(	Lic_PackageAttribs::Lic_SoftwareSpecAttribs::TMap_Lic_ProductSoftwareSpecAttribsMap::iterator prodSpecIt = param_pSoftwareSpec->productSpecMap->begin();
				prodSpecIt != param_pSoftwareSpec->productSpecMap->end();
				prodSpecIt++)
		{
			if((int)prodSpecIt->first == paramProductID)
			{
				wProductName = (prodSpecIt->second).productName;
				break;
			}
		}
		return wProductName;
	}

	static std::wstring GetModuleName(Lic_PackageAttribs::Lic_SoftwareSpecAttribs* param_pSoftwareSpec, int paramProductID, int paramModuleID)
	{
		std::wstring wModuleName = std::wstring(L"Unknown Module");
		for(	Lic_PackageAttribs::Lic_SoftwareSpecAttribs::TMap_Lic_ProductSoftwareSpecAttribsMap::iterator prodSpecIt = param_pSoftwareSpec->productSpecMap->begin();
				prodSpecIt != param_pSoftwareSpec->productSpecMap->end();
				prodSpecIt++)
		{
			if((int)prodSpecIt->first == paramProductID)
			{
				Lic_PackageAttribs::Lic_ProductSoftwareSpecAttribs::TMap_Lic_ModuleSoftwareSpecAttribsMap moduleSpecMap = GetModuleList(param_pSoftwareSpec, paramProductID);
				for(	Lic_PackageAttribs::Lic_ProductSoftwareSpecAttribs::TMap_Lic_ModuleSoftwareSpecAttribsMap::iterator modSpecIt = moduleSpecMap.begin();
						modSpecIt != moduleSpecMap.end();
						modSpecIt++)
				{
					if((int)modSpecIt->first == paramModuleID)
					{
						wModuleName = (modSpecIt->second).moduleName;
						break;
					}
				}
				break;
			}
		}
		return wModuleName;
	}

	//Returns the ModuleMap, takes Test/Dev Products into account
	static Lic_PackageAttribs::Lic_ProductSoftwareSpecAttribs::TMap_Lic_ModuleSoftwareSpecAttribsMap GetModuleList(Lic_PackageAttribs::Lic_SoftwareSpecAttribs* param_pSoftwareSpec, int paramProductID)
	{
		Lic_PackageAttribs::Lic_ProductSoftwareSpecAttribs::TMap_Lic_ModuleSoftwareSpecAttribsMap moduleSpecMap;
		Lic_PackageAttribs::Lic_ProductSoftwareSpecAttribs prodSpec;
		for(	Lic_PackageAttribs::Lic_SoftwareSpecAttribs::TMap_Lic_ProductSoftwareSpecAttribsMap::iterator prodSpecIt = param_pSoftwareSpec->productSpecMap->begin();
				prodSpecIt != param_pSoftwareSpec->productSpecMap->end();
				prodSpecIt++)
		{
			if((int)prodSpecIt->first == paramProductID)
			{
				prodSpec = prodSpecIt->second;
				break;
			}
		}

		if(prodSpec.sameModSpecProductID != 0)
		{
			for(	Lic_PackageAttribs::Lic_SoftwareSpecAttribs::TMap_Lic_ProductSoftwareSpecAttribsMap::iterator prodSpecIt = param_pSoftwareSpec->productSpecMap->begin();
				prodSpecIt != param_pSoftwareSpec->productSpecMap->end();
				prodSpecIt++)
			{
				if((int)prodSpecIt->first == prodSpec.sameModSpecProductID)
				{
					moduleSpecMap = prodSpecIt->second.moduleSpecMap;
					break;
				}
			}
		}
		else
		{
			moduleSpecMap = prodSpec.moduleSpecMap;
		}
		return moduleSpecMap;
	}
};


#endif //__Lic_PackageAttribsHelperAttribs_h__