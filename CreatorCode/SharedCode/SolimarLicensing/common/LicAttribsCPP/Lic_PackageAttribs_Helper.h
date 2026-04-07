// Lic_PackageAttribsHelper.h

#pragma once
#include "Lic_PackageAttribs.h"

// CR.32662.V03; Added typedefs for Lic_XXX attribs classes to simplify code.
typedef Lic_PackageAttribs::Lic_LicenseInfoAttribs TLic_LicenseInfoAttribs;			// CR.34184; added.
typedef Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_VerificationAttribs TLic_VerificationAttribs;
typedef Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs TLic_ValidationTokenAttribs;
typedef Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotHistoryInfoAttribs TLic_ActivitySlotHistoryInfoAttribs;
typedef Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs TLic_ActivitySlotChangeInfoAttribs;

typedef Lic_PackageAttribs::Lic_ProductSoftwareSpecAttribs TLic_ProductSoftwareSpecAttribs;

class Lic_PackageAttribsHelper
{
public:
	static CSpdCString GetDisplayLabel(
		const Lic_PackageAttribs::Lic_LicenseInfoAttribs &licInfoAttribs)
	{
		wchar_t wcsDiplayLabel[1024];
		wchar_t wcsDestinationID[5];
		wchar_t wcsSwGroupID[5];

		//wchar_t wLetter = L'U';
		//switch(_pLicInfoAttribs->softwareLicType)
		//{
		//	case Lic_PackageAttribs::Lic_LicenseInfoAttribs::sltFailover:
		//		wLetter = L'F';
		//		break;
		//	//case Lic_PackageAttribs::Lic_LicenseInfoAttribs::sltDisasterRecovery:
		//	//	wLetter = L'D';
		//	//	break;
		//	//case Lic_PackageAttribs::Lic_LicenseInfoAttribs::sltPerpetual:
		//	//	wLetter = L'P';
		//	//	break;
		//	//case Lic_PackageAttribs::Lic_LicenseInfoAttribs::sltSubscription:
		//	//	wLetter = L'S';
		//	//	break;
		//	//case Lic_PackageAttribs::Lic_LicenseInfoAttribs::sltTestDev:
		//	//	wLetter = L'T';
		//	//	break;
		//	//default:
		//	//	wLetter = L'U';
		//	//	break;
		//	default:
		//		wLetter = L'S';
		//		break;
		//}

		_itow_s(licInfoAttribs.GetdestinationID(), wcsDestinationID, 36);
		_itow_s(licInfoAttribs.GetsoftwareGroupLicenseID(), wcsSwGroupID, 36);
		for (int idx=0;idx<_countof(wcsDestinationID);idx++)
			wcsDestinationID[idx] = towupper(wcsDestinationID[idx]);
		for(int idx=0;idx<_countof(wcsSwGroupID);idx++)
			wcsSwGroupID[idx] = towupper(wcsSwGroupID[idx]);
		_snwprintf_s(wcsDiplayLabel, _TRUNCATE, L"%03x-%02s-%03s",
			(int)licInfoAttribs.GetcustomerID(),
			wcsDestinationID,
			wcsSwGroupID);			
		//_snwprintf_s(
		//	wcstrDiplayLabel,
		//	sizeof(wcstrDiplayLabel)/sizeof(wchar_t),
		//	sizeof(wcstrDiplayLabel)/sizeof(wchar_t),
		//	L"%03x-%02s-%02s-%c",
		//	(int)_pLicInfoAttribs->customerID,
		//	wcstrDestinationID,
		//	wcstrSwGroupID,
		//	wLetter
		//	);
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

		return wcsDiplayLabel;
	}


	static CSpdCString GetValidationValue(
		const TLic_VerificationAttribs::Lic_ValidationTokenAttribsList &licVerTokAttribsList, 
		TLic_ValidationTokenAttribs::TTokenType _tokenType)
	{
		for (TLic_VerificationAttribs::TItcLic_ValidationTokenAttribsList valTokenIT = licVerTokAttribsList.begin();
			valTokenIT != licVerTokAttribsList.end();
			valTokenIT++)
		//for (	std::vector<Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs>::iterator valTokenIT = (*_pLicVerTokAttribsList)->begin();
		//	valTokenIT != (*_pLicVerTokAttribsList)->end();
		//	valTokenIT++)
		{
			if (valTokenIT->GettokenType() != _tokenType) continue;

			return valTokenIT->GettokenValue();
		}

		return L"";
	}


	static CSpdCString GetProductName(
		Lic_PackageAttribs::Lic_SoftwareSpecAttribs* param_pSoftwareSpec, 
		int paramProductID)
	{
		CSpdCString wsProductName = L"Unknown Product";
		for (Lic_PackageAttribs::Lic_SoftwareSpecAttribs::TItcLic_ProductSoftwareSpecAttribsMap prodSpecIt = param_pSoftwareSpec->productSpecMap.begin();
			prodSpecIt != param_pSoftwareSpec->productSpecMap.end();
			prodSpecIt++)
		{
			if((int)prodSpecIt->first == paramProductID)
			{
				wsProductName = prodSpecIt->second.GetproductName();
				break;
			}
		}
		return wsProductName;
	}

	static CSpdCString GetModuleName(
		const Lic_PackageAttribs::Lic_SoftwareSpecAttribs &param_softwareSpec, 
		int paramProductID, 
		int paramModuleID)
	{
		CSpdCString wsModuleName = L"Unknown Module";
		for (Lic_PackageAttribs::Lic_SoftwareSpecAttribs::TItcLic_ProductSoftwareSpecAttribsMap prodSpecIt = param_softwareSpec.productSpecMap.begin();
			prodSpecIt != param_softwareSpec.productSpecMap.end();
			prodSpecIt++)
		{
			if((int)prodSpecIt->first == paramProductID)
			{
				TLic_ProductSoftwareSpecAttribs::Lic_ModuleSoftwareSpecAttribsMap moduleSpecMap = GetModuleList(param_softwareSpec, paramProductID);
				for (TLic_ProductSoftwareSpecAttribs::TItcLic_ModuleSoftwareSpecAttribsMap modSpecIt = moduleSpecMap.begin();
					modSpecIt != moduleSpecMap.end();
					modSpecIt++)
				//for (TLic_ProductSoftwareSpecAttribs::TMap_Lic_ModuleSoftwareSpecAttribsMap::iterator modSpecIt = moduleSpecMap.begin();
				//		modSpecIt != moduleSpecMap.end();
				//		modSpecIt++)
				{
					if((int)modSpecIt->first == paramModuleID)
					{
						wsModuleName = modSpecIt->second.GetmoduleName();
						break;
					}
				}
				break;
			}
		}
		return wsModuleName;
	}

	// Returns the ModuleMap, takes Test/Dev Products into account
	static TLic_ProductSoftwareSpecAttribs::Lic_ModuleSoftwareSpecAttribsMap GetModuleList(
		const Lic_PackageAttribs::Lic_SoftwareSpecAttribs &param_softwareSpec,
		int paramProductID)
	{
		// can't construct the module Spec Map outside of its parent class, 
		// so generate an empty attribs structure in order to initialize
		// our reference.
		TLic_ProductSoftwareSpecAttribs emptyAttribs;
		TLic_ProductSoftwareSpecAttribs::Lic_ModuleSoftwareSpecAttribsMap &moduleSpecMap(emptyAttribs.moduleSpecMap);
		TLic_ProductSoftwareSpecAttribs prodSpec;

		for (Lic_PackageAttribs::Lic_SoftwareSpecAttribs::TItcLic_ProductSoftwareSpecAttribsMap prodSpecIt = param_softwareSpec.productSpecMap.begin();
			prodSpecIt != param_softwareSpec.productSpecMap.end();
			prodSpecIt++)
		{
			if (prodSpecIt->first == (DWORD)paramProductID)
			{
				prodSpec = prodSpecIt->second;
				break;
			}
		}

		if (prodSpec.GetsameModSpecProductID() != 0)
		{
			for (Lic_PackageAttribs::Lic_SoftwareSpecAttribs::TItcLic_ProductSoftwareSpecAttribsMap prodSpecIt = param_softwareSpec.productSpecMap.begin();
				prodSpecIt != param_softwareSpec.productSpecMap.end();
				prodSpecIt++)
			{
				if (prodSpecIt->first == prodSpec.GetsameModSpecProductID())
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

		// makes a copy of the map and returns that.
		return moduleSpecMap;
	}
};


