#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

//#include "stdafx.h"
#include "SoftwareServer.h"
#include "KeySpec.h"	//For keySpec
#include "SoftwareServerInstance.h"
#include "..\common\LicenseError.h"
#include "..\common\LicAttribsCPP\Lic_GenericAttribs.h"
#include "..\common\LicAttribsCPP\Lic_KeyAttribs.h"
#include "..\common\LicAttribsCPP\Lic_LicenseSystemAttribs.h"
#include "..\common\LicAttribsCPP\Lic_SystemInfoAttribs.h"

#include <shlobj.h> //For SHGetFolderPath
#include <shlwapi.h> //For PathAppend
#include "..\common\CryptoHelper.h"	//For CryptoHelper
#include "..\common\TimeHelper.h"	//For TimeHelper
#include "..\common\NetworkUtils.h"	//For NetworkUtils::SolimarNetworkHelper::GetIPAddresses
#include "..\common\WMIHelper.h"	//For WMIHelper

#include "..\common\FlateHelper.h"
#include "..\Common\StringUtils.h"
#include "..\common\CryptoAndFlateHelper.h"
#include "..\common\VersionInfo.h"	//For Version Information

// only include the password packet signing PRIVATE key on debug and solimar internal builds
BYTE SoftwareServer::crypto_key_password_packet_private[] = {
#if defined(_SOLIMAR_INTERNAL) || defined(_DEBUG)
#include "..\common\keys\SolimarLicensingPacket.private.key.txt"
#else
#include "..\common\keys\SolimarLicensingPacket.public.key.txt"
#endif
};
BYTE SoftwareServer::crypto_key_password_packet_public[] = {
#include "..\common\keys\SolimarLicensingPacket.public.key.txt"
};
BYTE SoftwareServer::crypto_key_password_packet_password[] = {
#include "..\common\keys\SolimarLicensingPacket.password.txt"
};

BYTE SoftwareServer::crypto_key_verify_data_private[] = {
#include "..\common\keys\SolimarVerifyData.private.key.txt"
};
BYTE SoftwareServer::crypto_key_verify_data_public[] = {
#include "..\common\keys\SolimarVerifyData.public.key.txt"
};
BYTE SoftwareServer::crypto_key_verify_data_password[] = {
#include "..\common\keys\SolimarVerifyData.password.txt"
};


BYTE SoftwareServer::crypto_key_license_archive_private[] = {
#include "..\common\keys\SolimarLicensingArchive.private.key.txt"
};
BYTE SoftwareServer::crypto_key_license_archive_public[] = {
#include "..\common\keys\SolimarLicensingArchive.public.key.txt"
};
BYTE SoftwareServer::crypto_key_license_archive_password[] = {
#include "..\common\keys\SolimarLicensingPacket.password.txt"
};

//B83AB367E7194b808F05441273A7A64B
unsigned int SoftwareServer::license_packet_code_int[] = {
	0x42, 0x00, 0x38, 0x00, 0x33, 0x00, 0x41, 0x00,	// B83A
	0x42, 0x00, 0x33, 0x00, 0x36, 0x00, 0x37, 0x00,	// B367
	0x45, 0x00, 0x37, 0x00, 0x31, 0x00, 0x39, 0x00,	// E719
	0x34, 0x00, 0x42, 0x00, 0x38, 0x00, 0x30, 0x00,	// 4B80 
	0x38, 0x00, 0x46, 0x00, 0x30, 0x00, 0x35, 0x00,	// 8F05
	0x34, 0x00, 0x34, 0x00, 0x31, 0x00, 0x32, 0x00,	// 4412
	0x37, 0x00, 0x33, 0x00, 0x41, 0x00, 0x37, 0x00,	// 73A7
	0x41, 0x00, 0x36, 0x00, 0x34, 0x00, 0x42, 0x00,	// A64B
	0x00, 0x00
};

//8F93139804E94368B3D6C2F96F772A62
unsigned int SoftwareServer::license_archive_code_int[] = {
	0x38, 0x00, 0x46, 0x00, 0x39, 0x00, 0x33, 0x00,	// 8F93
	0x31, 0x00, 0x33, 0x00, 0x39, 0x00, 0x38, 0x00,	// 1398
	0x30, 0x00, 0x34, 0x00, 0x45, 0x00, 0x39, 0x00,	// 04E9
	0x34, 0x00, 0x33, 0x00, 0x36, 0x00, 0x38, 0x00,	// 4368 
	0x42, 0x00, 0x33, 0x00, 0x44, 0x00, 0x36, 0x00,	// B3D6
	0x43, 0x00, 0x32, 0x00, 0x46, 0x00, 0x39, 0x00,	// C2F9
	0x36, 0x00, 0x46, 0x00, 0x37, 0x00, 0x37, 0x00,	// 6F77
	0x32, 0x00, 0x41, 0x00, 0x36, 0x00, 0x32, 0x00,	// 2A62
	0x00, 0x00
};

//5C6A795104CF4364AE2E180E33DADB9A
unsigned int SoftwareServer::license_verify_data_code_int[] = {
	0x35, 0x00, 0x43, 0x00, 0x36, 0x00, 0x41, 0x00,	// 5C6A
	0x37, 0x00, 0x39, 0x00, 0x35, 0x00, 0x31, 0x00,	// 7951
	0x30, 0x00, 0x34, 0x00, 0x43, 0x00, 0x46, 0x00,	// 04CF
	0x34, 0x00, 0x33, 0x00, 0x36, 0x00, 0x34, 0x00,	// 4364 
	0x41, 0x00, 0x45, 0x00, 0x32, 0x00, 0x45, 0x00,	// AE2E
	0x31, 0x00, 0x38, 0x00, 0x30, 0x00, 0x45, 0x00,	// 180E
	0x33, 0x00, 0x33, 0x00, 0x44, 0x00, 0x41, 0x00,	// 33DA
	0x44, 0x00, 0x42, 0x00, 0x39, 0x00, 0x41, 0x00,	// DB9A
	0x00, 0x00
};


SoftwareServer::SoftwareServer():
	pSoftwareSpec(NULL),
	SoftwareLicenseLock(CreateMutex(0,0,0)),
	bFirstTime(true)
{
//BYTE serialByte[] = {
//						0x53, 0x00, 0x65, 0x00, 0x72, 0x00, 0x69, 0x00, 0x61, 0x00, 0x6c, 0x00, 0x4e, 0x00, 0x75, 0x00, 0x6d, 0x00, 0x62, 0x00, 0x65, 0x00, 0x72, 0x00, 0x00, 0x00
//					};
//BYTE wmiQuery_SerialByte[] = {
//0x43, 0x00, 0x39, 0x00, 0x33, 0x00, 0x37, 0x00,	//C937
//0x39, 0x00, 0x44, 0x00, 0x41, 0x00, 0x36, 0x00, //9DA6
//0x30, 0x00, 0x37, 0x00, 0x37, 0x00, 0x36, 0x00,	//0776
//0x34, 0x00, 0x36, 0x00, 0x35, 0x00, 0x38, 0x00,	//4658
//0x38, 0x00, 0x37, 0x00, 0x33, 0x00, 0x38, 0x00,	//8738
//0x35, 0x00, 0x31, 0x00, 0x38, 0x00, 0x42, 0x00,	//518B
//0x33, 0x00, 0x45, 0x00, 0x42, 0x00, 0x44, 0x00,	//3EBD
//0x36, 0x00, 0x37, 0x00, 0x41, 0x00, 0x46, 0x00,	//67AF
//0x00, 0x00
//};
//OutputDebugString((wchar_t*)wmiQuery_SerialByte);
//OutputDebugString(L"C9379DA6077646588738518B3EBD67AF");
	if(g_pSoftwareSpec == NULL)
		g_pSoftwareSpec = new GlobalSoftwareSpec();
	//Use Global SoftwareSpec - Only need to do once
	licCache.RefreshSoftwareSpec(&(g_pSoftwareSpec->GetSoftwareSpec()));
	;	
}

SoftwareServer::~SoftwareServer()
{
	if (SoftwareLicenseLock!=NULL)
	{
		CloseHandle(SoftwareLicenseLock);
		SoftwareLicenseLock = NULL;
	}
	if(pSoftwareSpec != NULL)
	{
		delete pSoftwareSpec;
		pSoftwareSpec = NULL;
	}
}

HRESULT SoftwareServer::Initialize(RainbowDriver* pDriver)
{
	SafeMutex mutex(SoftwareLicenseLock);
	pRainbowDriver = pDriver;
	HRESULT hr(S_OK);

	//hr = ResynchronizeSoftwareLicenses();
	return hr;
}

HRESULT SoftwareServer::AddApplicationInstance(long productID, BSTR license_id, BSTR application_instance)
{
	SafeMutex mutex(SoftwareLicenseLock);
	return licCache.AddApplicationInstance(productID, license_id, application_instance);
}
HRESULT SoftwareServer::RemoveApplicationInstance(long productID, BSTR license_id, BSTR application_instance)
{
	SafeMutex mutex(SoftwareLicenseLock);
	return licCache.RemoveApplicationInstance(productID, license_id, application_instance);
}
HRESULT SoftwareServer::GetApplicationInstanceList(long productID, BSTR license_id, BSTR *pBstrListAppInstStream)
{
	SafeMutex mutex(SoftwareLicenseLock);
	return licCache.GetApplicationInstanceList(productID, license_id, pBstrListAppInstStream);
}




HRESULT SoftwareServer::ResynchronizeSoftwareLicenses(bool bForceRefresh)
{
const int BUF_SIZE = 1024;
wchar_t tmpbuf[BUF_SIZE];
//if(bFirstTime)
//{
//	swprintf_s(tmpbuf, BUF_SIZE, L"SoftwareServer::ResynchronizeSoftwareLicenses() - Enter");
//	OutputDebugString(tmpbuf);
//}
//OutputDebugString(L"SoftwareServer::ResynchronizeSoftwareLicenses() - Enter");
	HRESULT hr(E_FAIL);
	DWORD dwNtErr(ERROR_SUCCESS);

	try
	{
		hr = licServerDataMgr.Touch();

		std::list<Lic_PackageAttribs::Lic_LicenseInfoAttribs*> tmpLicInfoList;
		{
		SafeMutex mutex(SoftwareLicenseLock);
		// retrieve license files by looking in licServerDataMgr
		Lic_ServerDataAttribs::Lic_ServerDataFileInfoAttribsList tmpFileInfoList;
		hr = licServerDataMgr.GetAllFileInfoList(&tmpFileInfoList);
		if(FAILED(hr))
			throw hr;

		SoftwareLicenseMgr* pNewSwLicMgr;
//		if(bFirstTime)
//			OutputDebugString(L"SoftwareServer::ResynchronizeSoftwareLicenses()");
		for(	Lic_ServerDataAttribs::TVector_Lic_ServerDataFileInfoAttribsList::iterator licSrvFileInfoListIt = tmpFileInfoList->begin();
				licSrvFileInfoListIt != tmpFileInfoList->end();
				licSrvFileInfoListIt++)
		{
			if(softwareLicMgrMap.find(_bstr_t(licSrvFileInfoListIt->LicFileName->c_str())) == softwareLicMgrMap.end())
			{
				pNewSwLicMgr = new SoftwareLicenseMgr();
				hr = pNewSwLicMgr->Initialize(_bstr_t(licSrvFileInfoListIt->LicFileName->c_str()), pRainbowDriver, &licServerDataMgr);

if(bFirstTime)
{
	swprintf_s(tmpbuf, _countof(tmpbuf), L"%s, %s, %s, %08x, %s, %d", (wchar_t*)bstr_t(licSrvFileInfoListIt->LicFileName->c_str()), (wchar_t*)bstr_t(licSrvFileInfoListIt->LicName->c_str()), (wchar_t*)bstr_t(licSrvFileInfoListIt->LicFileVerificationCode->c_str()), hr, std::wstring(SpdAttribs::WStringObj(licSrvFileInfoListIt->LicModifiedDate)).c_str(), (int)licSrvFileInfoListIt->LicCurrentActivations);
	OutputDebugString(tmpbuf);
}
				if(FAILED(hr))
				{
					delete pNewSwLicMgr;
					pNewSwLicMgr = NULL;
					continue;
				}

				bool bValid(true);
	//OutputDebugString(L"SoftwareServer::ResynchronizeSoftwareLicenses() - pNewSwLicMgr->Verify(&bValid)");
				hr = pNewSwLicMgr->Verify(&bValid, bForceRefresh);

				//if(SUCCEEDED(hr))
				softwareLicMgrMap.insert(SoftwareLicList::value_type(_bstr_t(licSrvFileInfoListIt->LicFileName->c_str()), pNewSwLicMgr));
			}
		}

		for(SoftwareLicList::iterator swLicIt=softwareLicMgrMap.begin(); swLicIt!=softwareLicMgrMap.end(); swLicIt++)
		{
			bool bValid(true);
			hr = (*swLicIt).second->Verify(&bValid, bForceRefresh);
			if(FAILED(hr) || (bValid==false))
			{
				//Maybe break and return fatal error
				//Shoud log in event log and propugate a licensing message the software license and reason why verification failed.
				continue;	
			}

			Lic_PackageAttribs::Lic_LicenseInfoAttribs tmpLicInfoAttribs;
			hr = (*swLicIt).second->GetLicenseInfo(&tmpLicInfoAttribs);
			if(SUCCEEDED(hr))
				tmpLicInfoList.insert(tmpLicInfoList.end(), new Lic_PackageAttribs::Lic_LicenseInfoAttribs(tmpLicInfoAttribs));
		}
		
//OutputDebugString(L"SoftwareServer::ResynchronizeSoftwareLicenses() - licCache.RefreshCache(&tmpLicInfoList)");
		licCache.RefreshCache(&tmpLicInfoList);
		if(bFirstTime) 
			bFirstTime = false;
		}

		//Clean up
		while(!tmpLicInfoList.empty())
		{
			if(*(tmpLicInfoList.begin()) != NULL)
				delete *(tmpLicInfoList.begin());

			tmpLicInfoList.erase(tmpLicInfoList.begin());
		}
	}
	catch(HRESULT &ehr)
	{
		hr = ehr;
	}
	catch(...)
	{
OutputDebugString(L"SoftwareServer::ResynchronizeSoftwareLicenses() - Caught unexpected Exception");
	}

//OutputDebugString(L"SoftwareServer::ResynchronizeSoftwareLicenses() - Leave");
	return hr;
}


//HRESULT SoftwareServer::ValidateLicense(long productID, BSTR licenseID, VARIANT_BOOL *pbLicenseValid)
//{
////OutputDebugString(L"SoftwareServer::ValidateLicense()");
//	SafeMutex mutex(SoftwareLicenseLock);
//	return licCache.ValidateLicense(productID, licenseID, pbLicenseValid);
//}
HRESULT SoftwareServer::ModuleLicenseTotalForAll(long productID, long moduleIdent, long* pLicenseCount)
{
	SafeMutex mutex(SoftwareLicenseLock);
	return licCache.ModuleLicenseTotalForAll(productID, moduleIdent, pLicenseCount);
}
HRESULT SoftwareServer::ModuleLicenseInUseForAll(long productID, long moduleIdent, long* pLicenseCount)
{
	SafeMutex mutex(SoftwareLicenseLock);
	return licCache.ModuleLicenseInUseForAll(productID, moduleIdent, pLicenseCount);
}

//Results based on the application instance based on the license_id 
HRESULT SoftwareServer::ModuleLicenseInUseByApp(long productID, BSTR licenseID, long moduleIdent, long* pLicenseCount)
{
	SafeMutex mutex(SoftwareLicenseLock);
	return licCache.ModuleLicenseInUseByApp(productID, licenseID, moduleIdent, pLicenseCount);
}
//Results based on the license_id.
HRESULT SoftwareServer::ModuleLicenseInUseByLicenseID(long productID, BSTR licenseID, long moduleIdent, long* pLicenseCount)
{
	SafeMutex mutex(SoftwareLicenseLock);
	return licCache.ModuleLicenseInUseByLicenseID(productID, licenseID, moduleIdent, pLicenseCount);
}

HRESULT SoftwareServer::ModuleLicenseObtainByApp(long productID, BSTR licenseID, long moduleIdent, long licenseCount)
{
	SafeMutex mutex(SoftwareLicenseLock);
//Testing handling messages callbacks...
//g_licenseController.GenerateSoftwareLicenseMessage(L"TestSoftwareLicense", 12, MT_WARNING, S_OK, time(0), MessageSoftwareModuleExpiration, L"TestProduct", 12, L"TestMessage");

	return licCache.ModuleLicenseObtainByApp(productID, licenseID, moduleIdent, licenseCount);
}
HRESULT SoftwareServer::ModuleLicenseReleaseByApp(long productID, BSTR licenseID, long moduleIdent, long licenseCount)
{
	SafeMutex mutex(SoftwareLicenseLock);
	return licCache.ModuleLicenseReleaseByApp(productID, licenseID, moduleIdent, licenseCount);
}
HRESULT SoftwareServer::ModuleLicenseDecrementCounterByApp(long productID, BSTR licenseID, long moduleIdent, long licenseCount)
{
	SafeMutex mutex(SoftwareLicenseLock);
	return licCache.ModuleLicenseDecrementCounterByApp(productID, licenseID, moduleIdent, licenseCount);
}


HRESULT SoftwareServer::GetSoftwareLicenseInfoByProduct_ForAll(long productID, BSTR *pBstrProductInfoAttribsStream)
{
	SafeMutex mutex(SoftwareLicenseLock);
	HRESULT hr(S_OK);
	Lic_PackageAttribs::Lic_ProductInfoAttribs tmpProdInfoAttribs;
	hr = licCache.GetCache_ByProduct(productID, &tmpProdInfoAttribs);
	if(SUCCEEDED(hr))
	{
		if((int)tmpProdInfoAttribs.productID != productID )
			hr = LicenseServerError::EHR_LIC_SOFTWARE_NO_PRODUCT;

		if(SUCCEEDED(hr))
			*pBstrProductInfoAttribsStream = SysAllocString(tmpProdInfoAttribs.ToString().c_str());
	}
	return hr;
}
HRESULT SoftwareServer::GetSoftwareLicenseInfo_ForAll(BSTR *pBstrLicenseInfoAttribsStream)
{
	SafeMutex mutex(SoftwareLicenseLock);
	HRESULT hr(S_OK);
	Lic_PackageAttribs::Lic_LicenseInfoAttribs tmpLicInfoAttribs;
	hr = licCache.GetCache(&tmpLicInfoAttribs);
	if(SUCCEEDED(hr))
		*pBstrLicenseInfoAttribsStream = SysAllocString(tmpLicInfoAttribs.ToString().c_str());
	return hr;
}

HRESULT SoftwareServer::GetAllSoftwareLicenses(BSTR *pBstrLicenseListStream)
{
	SafeMutex mutex(SoftwareLicenseLock);
	HRESULT hr(S_OK);
	Lic_StringListAttribs list;
	for(SoftwareLicList::iterator swLicMgrIt = softwareLicMgrMap.begin();
		swLicMgrIt != softwareLicMgrMap.end();
		swLicMgrIt++
		)
	{
		BSTR tmpSoftwareLicense;
		swLicMgrIt->second->GetSoftwareLicenseName(&tmpSoftwareLicense);
		list.strList->push_back(tmpSoftwareLicense);
		SysFreeString(tmpSoftwareLicense);
	}
	*pBstrLicenseListStream = SysAllocString(list.strList.ToString().c_str());
	
	return hr;
}
HRESULT SoftwareServer::GetSoftwareLicenseInfoByProduct_ByLicense(BSTR softwareLicense, long productID, BSTR *pBstrProductInfoAttribsStream)
{
	SafeMutex mutex(SoftwareLicenseLock);

	//Temp, only return the first item as a string, not as a list, change later...
	//SoftwareLicList softwareLicMgrMap; 
	HRESULT hr(E_NOTIMPL);
	//THIS FUNCTION WAS NEVER FINISHED!
	//SoftwareLicList::iterator swLicMgrIt = softwareLicMgrMap.begin();
	//if(swLicMgrIt != softwareLicMgrMap.end())
	//{
	//	//swLicMgrIt->second->
	//	*pBstrProductInfoAttribsStream = SysAllocString(L"Hello");
	//	hr = S_OK;
	//}
	return hr;
}
HRESULT SoftwareServer::GetSoftwareLicenseInfo_ByLicense(BSTR softwareLicense, BSTR *pBstrLicenseInfoAttribsStream)
{
	SafeMutex mutex(SoftwareLicenseLock);
	HRESULT hr = S_OK;

	Lic_PackageAttribs::Lic_LicenseInfoAttribs tmpLicInfoAttribs;
	hr = GetSoftwareLicenseInfo_ByLicenseInternal(softwareLicense, &tmpLicInfoAttribs);
	if(SUCCEEDED(hr))
	{
		//Update Cache if the Software License is not in cache, or the modified date is different
		if(swLicStreamedCacheMap.find(softwareLicense) == swLicStreamedCacheMap.end() || 
			_wcsicmp(swLicStreamedCacheMap[softwareLicense].first.c_str(), std::wstring(tmpLicInfoAttribs.modifiedDate).c_str()) != 0)
		{
			swLicStreamedCacheMap[softwareLicense].first = tmpLicInfoAttribs.modifiedDate;
			swLicStreamedCacheMap[softwareLicense].second = tmpLicInfoAttribs.ToString().c_str();	//This call will take a long time when the License Info object is big.
		}
	}
	*pBstrLicenseInfoAttribsStream = SysAllocString(swLicStreamedCacheMap.find(softwareLicense)->second.second.c_str());

	return hr;
}

HRESULT SoftwareServer::GetSoftwareLicenseStatus_ByProduct(long productID, BSTR *pBstrStringToDwordMap)
{
	HRESULT hr = E_FAIL;
	SafeMutex mutex(SoftwareLicenseLock);
	try
	{
		Lic_StringDwordMapAttribs stringDwordMap;
		for(SoftwareLicList::iterator swLicMgrIt = softwareLicMgrMap.begin();
			swLicMgrIt != softwareLicMgrMap.end();
			swLicMgrIt++
			)
		{
			hr = swLicMgrIt->second->ContainsLicenseForProduct(productID);
			if(SUCCEEDED(hr))
			{
				bool bValid;
				hr = swLicMgrIt->second->Verify(&bValid, false);

				//Add the software license name and hr to the list
				BSTR tmpSoftwareLicense;
				swLicMgrIt->second->GetSoftwareLicenseName(&tmpSoftwareLicense);
				stringDwordMap.strDwordMap->insert(SpdAttribs::MapStringDwordAttrib::TypeT::value_type(tmpSoftwareLicense, hr));
				SysFreeString(tmpSoftwareLicense);
			}
		}
		*pBstrStringToDwordMap = SysAllocString(stringDwordMap.strDwordMap.ToString().c_str());
		hr = S_OK;
	}
	catch(HRESULT &eHr)
	{
		hr = eHr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}

	return hr;
}
HRESULT SoftwareServer::GetSoftwareLicenseStatus_ByLicense(BSTR softwareLicense)
{
	HRESULT hr(E_INVALIDARG);
	SafeMutex mutex(SoftwareLicenseLock);
	try
	{
		for(SoftwareLicList::iterator swLicMgrIt = softwareLicMgrMap.begin();
			swLicMgrIt != softwareLicMgrMap.end();
			swLicMgrIt++)
		{
			BSTR tmpSoftwareLicense;
			swLicMgrIt->second->GetSoftwareLicenseName(&tmpSoftwareLicense);
			if(_wcsicmp(softwareLicense, tmpSoftwareLicense) == 0)
			{
				bool bValid;
				hr = swLicMgrIt->second->Verify(&bValid, true);
				SysFreeString(tmpSoftwareLicense);
				break;
			}
			SysFreeString(tmpSoftwareLicense);
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
	return hr;
}

HRESULT SoftwareServer::GetSoftwareSpecByProduct(long productID, BSTR *pBstrProductSoftwareSpecAttribs)
{
	HRESULT hr(E_NOTIMPL);
	/*
//OutputDebugString(L"SoftwareServer::16() - SafeMutex mutex(SoftwareLicenseLock) - Wait");
wchar_t tmpbuf[1024];
swprintf_s(tmpbuf, 1024, L"SoftwareServer::GetSoftwareSpecByProduct() - SafeMutex mutex(SoftwareLicenseLock) - Wait, SoftwareLicenseLock: 0x%08x, this: 0x%08x, ThreadID: %d", SoftwareLicenseLock, this, GetCurrentThreadId());
OutputDebugString(tmpbuf);
	{
	SafeMutex mutex(SoftwareLicenseLock);
	Lic_PackageAttribs::Lic_SoftwareSpecAttribs::TMap_Lic_ProductSoftwareSpecAttribsMap::iterator prodSpecIt = pSoftwareSpec->productSpecMap->find(productID);
	
	if(prodSpecIt != pSoftwareSpec->productSpecMap->end())
	{
		*pBstrProductSoftwareSpecAttribs = SysAllocString(prodSpecIt->second.ToString().c_str());
		hr = S_OK;
	}
OutputDebugString(L"SoftwareServer::GetSoftwareSpecByProduct() - SafeMutex mutex(SoftwareLicenseLock) - Leave");
	}
	*/
	return hr;
}
HRESULT SoftwareServer::GetSoftwareSpec(BSTR *pBstrSoftwareSpecAttribsStream)
{
	SafeMutex mutex(SoftwareLicenseLock);
	//int c = softwareLicMgrMap.size();
	*pBstrSoftwareSpecAttribsStream = SysAllocString(pSoftwareSpec->ToString().c_str());
	//*pBstrSoftwareSpecAttribsStream = SysAllocString(L"Hello");
	return S_OK;
}

HRESULT SoftwareServer::GenerateSoftwareLicPacket(BSTR bstrLicPackageAttribsStream, VARIANT vtExpires, BSTR *pBstrVerificationCode, VARIANT* pVtLicensePacket)
{
//const int BUF_SIZE = 1024;
//wchar_t tmpbuf[BUF_SIZE];
//swprintf_s(tmpbuf, BUF_SIZE, L"SoftwareServer::GenerateSoftwareLicPacket() - Enter");
//OutputDebugString(tmpbuf);
//swprintf_s(tmpbuf, BUF_SIZE, L"SoftwareServer::GenerateSoftwareLicPacket() - bstrLicPackageAttribsStream: %s", (wchar_t*)bstrLicPackageAttribsStream);
//OutputDebugString(tmpbuf);

	SafeMutex mutex(SoftwareLicenseLock);
	HRESULT hr(S_OK);


	Lic_PackageAttribs licensePackageAttribs;
	licensePackageAttribs.InitFromString(bstrLicPackageAttribsStream);
	if(FAILED(hr))
		throw hr;

	//Populate the modified date
	wchar_t timestamp[256];
	SYSTEMTIME currentSystime;
	GetSystemTime(&currentSystime);	//Retrieves Universal Time
	TimeHelper::SystemTimeToString(timestamp, _countof(timestamp), currentSystime);
	licensePackageAttribs.licLicenseInfoAttribs.modifiedDate = std::wstring(timestamp);

	//
	// Set a variable in licenseInfoAttribs to Package Only
	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs tmpValTokenAttribs;
	tmpValTokenAttribs.tokenType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttTypePackageOnly;
	tmpValTokenAttribs.tokenValue = std::wstring(L"true");
	licensePackageAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->push_back(tmpValTokenAttribs);

	BSTR bstrLicAttribsStream = SysAllocString(licensePackageAttribs.ToString().c_str());

	wchar_t buffer[64];	
	_bstr_t packet_string;

	//Can't use "\r\n" as a delimiter, c++ ToString() for license attribs adds "\n" for xml separation, the c# ToString uses " "
	wchar_t delimiter[] = L"\1";

	// write the magic number
	_bstr_t bstr_license_packet_code;
	bstr_license_packet_code = (wchar_t*)(&license_packet_code_int[0]);
	for(int idx=1; idx<32; idx++)
		bstr_license_packet_code += (wchar_t*)(&license_packet_code_int[2*idx]);
	packet_string += bstr_license_packet_code;
	packet_string += delimiter;

	// write the expiration date
	swprintf_s(buffer,L"%f",(double)vtExpires.date);
	packet_string += buffer;
	packet_string += delimiter;

	// write the streamed license package attribs
	packet_string += bstrLicAttribsStream;
	packet_string += delimiter;
	SysFreeString(bstrLicAttribsStream);

	// compute a hash of the file
	CryptoHelper context;
	CryptoHelper::Digest digest;
	CryptoHelper::Key packet_sign_key(context, crypto_key_password_packet_private, sizeof(crypto_key_password_packet_private)/sizeof(BYTE));
	_variant_t vtSignature;
	//hr = context.HashData((BYTE*)(wchar_t*)packet_string, (packet_string.length()+1)*sizeof(wchar_t), digest);
	hr = context.HashData((BYTE*)(wchar_t*)packet_string, (packet_string.length())*sizeof(wchar_t), digest);
	if (FAILED(hr)) return hr;

	// sign the hash
	hr = context.SignHash(digest, &vtSignature);
	//hr==NTE_BAD_KEYSET means that license server is release, not debug or internal release.
	if (FAILED(hr)) return (hr==NTE_BAD_KEYSET) ? LicenseServerError::EHR_LIC_SOFTWARE_CANT_GENERATE_LIC_PACKET : hr;
	
   // add the hash and the signature to the string
	packet_string += StringUtils::BinaryToString(digest.m_digest, digest.DIGEST_SIZE);
	packet_string += delimiter;
	


	BYTE *pSignature = 0;
	if (FAILED(hr = SafeArrayAccessData(vtSignature.parray, (void**)&pSignature))) return hr;
	packet_string += StringUtils::BinaryToString(pSignature, vtSignature.parray->rgsabound[0].cElements);
	packet_string += delimiter;


//swprintf_s(tmpbuf, BUF_SIZE, L"SoftwareServer::GenerateSoftwareLicPacket() - packet_string: %s", (wchar_t*)packet_string);
//OutputDebugString(tmpbuf);

	SafeArrayUnaccessData(vtSignature.parray);

	// compute the verification code using the hash of the signature

//swprintf_s(tmpbuf, BUF_SIZE, L"SoftwareServer::GenerateSoftwareLicPacket() - packet_string.length(): %d, (packet_string.length()+1)*sizeof(wchar_t): %d, sizeof(wchar_t): %d", packet_string.length(), (packet_string.length()+1)*sizeof(wchar_t), sizeof(wchar_t));
//OutputDebugString(tmpbuf);
	if (FAILED(hr = context.HashData((BYTE*)(wchar_t*)packet_string, (packet_string.length()+1)*sizeof(wchar_t), digest))) return hr;

	// take the first 8 characters from the hex representation of the digest as the verification code
	*pBstrVerificationCode = SysAllocString(std::wstring((wchar_t*)StringUtils::BinaryToString(digest.m_digest, digest.DIGEST_SIZE)).substr(0,8).c_str());

//swprintf_s(tmpbuf, BUF_SIZE, L"SoftwareServer::GenerateSoftwareLicPacket() - pBstrVerificationCode: %s", (wchar_t*)*pBstrVerificationCode);
//OutputDebugString(tmpbuf);

	//Flate string first
	std::string data = StringUtils::WstringToString(std::wstring(packet_string));
	long compressedStreamSize(0);
	//Up to caller to free value from CompressStream
//swprintf_s(tmpbuf, BUF_SIZE, L"SoftwareServer::GenerateSoftwareLicPacket() - Pre FlateHelper::CompressStream - data.length(): %d", data.length());
//OutputDebugString(tmpbuf);

	unsigned char* pCompressedStream = FlateHelper::CompressStream((char*)(data.c_str()), (long)data.length(), &compressedStreamSize);

	//long compressedStreamSize(0);
	//unsigned char* pCompressedStream = FlateHelper::CompressStream((char*)(std::wstring(packet_string).c_str()), packet_string.length()*sizeof(wchar_t), &compressedStreamSize);
	if(pCompressedStream != NULL)
	{
		// package the string in to a variant
		pVtLicensePacket->vt = VT_ARRAY | VT_UI1;
		pVtLicensePacket->parray = SafeArrayCreateVector(VT_UI1, 0, (compressedStreamSize+1)*sizeof(char));

		BYTE *pPacketData = 0;
		if (FAILED(hr = SafeArrayAccessData(pVtLicensePacket->parray, (void**)&pPacketData))) return hr;
		memcpy(pPacketData, pCompressedStream, (compressedStreamSize+1)*sizeof(char));
		
		// encrypt the password packet
		hr = context.EncryptData(	crypto_key_password_packet_password, 
									sizeof(crypto_key_password_packet_password), 
									pPacketData, 
									(compressedStreamSize+1)*sizeof(char));
//swprintf_s(tmpbuf, BUF_SIZE, L"SoftwareServer::GenerateSoftwareLicPacket() - Post context.EncryptData - compressedStreamSize: %d, pVtLicensePacket->parray->rgsabound[0].cElements: %d", compressedStreamSize+1, pVtLicensePacket->parray->rgsabound[0].cElements);
//OutputDebugString(tmpbuf);
		delete [] pCompressedStream;
	}

	SafeArrayUnaccessData(pVtLicensePacket->parray);

	return hr;
}

HRESULT SoftwareServer::EnterSoftwareLicPacket(VARIANT vtLicensePacket, BSTR *pBstrVerificationCode)
{
//const int BUF_SIZE = 1024;
//wchar_t tmpbuf[BUF_SIZE];
//swprintf_s(tmpbuf, BUF_SIZE, L"SoftwareServer::EnterSoftwareLicPacket() - Enter");
//OutputDebugString(tmpbuf);

	SafeMutex mutex(SoftwareLicenseLock);
	HRESULT hr(E_NOTIMPL);

	_bstr_t verification;
	wchar_t *pPacketString1 = 0, *pPacketString2 = 0;
	try
	{
		// sanity check
		//if (!(vtLicensePacket.vt & VT_ARRAY) || !(vtLicensePacket.vt & VT_UI1) || vtLicensePacket.parray->rgsabound[0].cElements>=512*1024)
		if (!(vtLicensePacket.vt & VT_ARRAY) || !(vtLicensePacket.vt & VT_UI1))
			throw E_INVALIDARG;

		// create a copy of the password packet for decrypting
		BYTE *pPacket = 0;
		
		if (FAILED(hr = SafeArrayAccessData(vtLicensePacket.parray, (void**)&pPacket)))
			throw hr;

		//Must inflate first...
		unsigned char* uncompressedBuf = NULL;
		long uncompressedBufSize(0);
		DWORD password_packet_length = vtLicensePacket.parray->rgsabound[0].cElements;

	//swprintf_s(tmpbuf, BUF_SIZE, L"SoftwareServer::EnterSoftwareLicPacket() - Pre context.DecryptData - password_packet_length: %d", password_packet_length);
	//OutputDebugString(tmpbuf);

		{
			CryptoHelper context;
			hr = context.DecryptData(crypto_key_password_packet_password, sizeof(crypto_key_password_packet_password)/sizeof(BYTE), (BYTE*)pPacket, password_packet_length);
			if(FAILED(hr))
				throw E_INVALIDARG;
		}

		////Up to caller to free value from UnCompressStream
		try
		{
			uncompressedBuf = FlateHelper::UnCompressStream((char*)pPacket, password_packet_length, &uncompressedBufSize);
		}
		catch(...)
		{
			// we're done with the source variant
			SafeArrayUnaccessData(vtLicensePacket.parray);
			throw E_INVALIDARG;
		}

	//swprintf_s(tmpbuf, BUF_SIZE, L"SoftwareServer::EnterSoftwareLicPacket() - Post FlateHelper::UnCompressStream - uncompressedBufSize: %d", uncompressedBufSize);
	//OutputDebugString(tmpbuf);

		// we're done with the source variant
		pPacket = 0;
		SafeArrayUnaccessData(vtLicensePacket.parray);

		if(uncompressedBuf == NULL)
			throw E_INVALIDARG;

		////
		//// decrypt the password packet (two copies)
		//// this copy is used for strtok
		//pPacketString1 = (wchar_t*)new BYTE[uncompressedBufSize+2];
		//// this copy is used to verify the hash of the packet data stored in the password packet
		//pPacketString2 = (wchar_t*)new BYTE[uncompressedBufSize+2];
		//memset(pPacketString1, 0, uncompressedBufSize+2);
		//memset(pPacketString2, 0, uncompressedBufSize+2);
		//memcpy(pPacketString1, uncompressedBuf, uncompressedBufSize);
		//memcpy(pPacketString2, uncompressedBuf, uncompressedBufSize);
		//pPacket = 0;
		//delete [] uncompressedBuf;

		std::wstring tmpString = StringUtils::StringToWstring((char*)uncompressedBuf).c_str();
		//
		// decrypt the password packet (two copies)
		// this copy is used for strtok
		pPacketString1 = (wchar_t*)new BYTE[(uncompressedBufSize+1)*sizeof(wchar_t)];
		// this copy is used to verify the hash of the packet data stored in the password packet
		pPacketString2 = (wchar_t*)new BYTE[(uncompressedBufSize+1)*sizeof(wchar_t)];
		memset(pPacketString1, 0, (uncompressedBufSize+1)*sizeof(wchar_t));
		memset(pPacketString2, 0, (uncompressedBufSize+1)*sizeof(wchar_t));
		memcpy(pPacketString1, tmpString.c_str(), (uncompressedBufSize)*sizeof(wchar_t));
		memcpy(pPacketString2, tmpString.c_str(), (uncompressedBufSize)*sizeof(wchar_t));
		delete [] uncompressedBuf;
		
		// compute the verification code
		{
			CryptoHelper context;
			CryptoHelper::Digest digest;

//swprintf_s(tmpbuf, BUF_SIZE, L"SoftwareServer::EnterSoftwareLicPacket() - uncompressedBufSize+2: %d, (uncompressedBufSize+1)*sizeof(wchar_t): %d", uncompressedBufSize+2, (uncompressedBufSize+2)*sizeof(wchar_t));
//OutputDebugString(tmpbuf);

			if (FAILED(hr = context.HashData((BYTE*)(wchar_t*)pPacketString1, (uncompressedBufSize+1)*sizeof(wchar_t), digest))) throw hr;

			verification = std::wstring((wchar_t*)StringUtils::BinaryToString(digest.m_digest, digest.DIGEST_SIZE)).substr(0,8).c_str();
//swprintf_s(tmpbuf, BUF_SIZE, L"SoftwareServer::EnterSoftwareLicPacket() - verification: %s", (wchar_t*)verification);
//OutputDebugString(tmpbuf);
		}
		
		// parse the password packet
		wchar_t *pNextToken;
		wchar_t* pToken(0);
		//Can't use "\r\n" as a delimiter, c++ ToString() for license attribs adds "\n" for xml separation, the c# ToString uses " "
		wchar_t delimiter[] = L"\1";

		_bstr_t bstr_license_packet_code;
		bstr_license_packet_code = (wchar_t*)(&license_packet_code_int[0]);
		for(int idx=1; idx<32; idx++)
			bstr_license_packet_code += (wchar_t*)(&license_packet_code_int[2*idx]);
		if (!(pToken = wcstok_s(pPacketString1, delimiter, &pNextToken)) || wcscmp(pToken,bstr_license_packet_code)!=0) 
			throw E_INVALIDARG;


		wchar_t *expiresLicensePacket = NULL;
		wchar_t *licensePacketAttribsStream = NULL;

		if (!(expiresLicensePacket = wcstok_s(NULL, delimiter, &pNextToken))) 
			throw E_INVALIDARG;
		if (!(licensePacketAttribsStream = wcstok_s(NULL, delimiter, &pNextToken))) 
			throw E_INVALIDARG;

		// convert expires in to a floating point
		double dexpires = 1.0;
		dexpires = _wtof(expiresLicensePacket);
		_variant_t vtExpiresDate;
		VariantInit(&vtExpiresDate);
		vtExpiresDate = _variant_t(dexpires, VT_DATE);

		// check for expired license packages
		// get the current date/time
		_variant_t current_time(0.0,VT_DATE);
		SYSTEMTIME systime;
		GetSystemTime(&systime);
		if (!SystemTimeToVariantTime(&systime, &current_time.date)) throw(E_FAIL);

		if (vtExpiresDate.date!=0 && vtExpiresDate.date<current_time.date) throw LicenseServerError::EHR_PACKET_EXPIRED;


		// grab the hash and the hash signature
		wchar_t *hash = NULL;
		wchar_t *signature = NULL;
		if (!(hash = wcstok_s(NULL, delimiter, &pNextToken))) throw(E_FAIL);
		if (!(signature = wcstok_s(NULL, delimiter, &pNextToken))) throw(E_FAIL);
		
		// calculate the number of bytes to used to calculate the hash
		DWORD hashed_data_size = (DWORD)((BYTE*)hash - (BYTE*)pPacketString1);

		// hash the string
		CryptoHelper context;
		CryptoHelper::Digest computed_digest, provided_digest;
		CryptoHelper::Key signature_verify_key(context, crypto_key_password_packet_public, sizeof(crypto_key_password_packet_public)/sizeof(BYTE));
		hr = context.HashData((BYTE*)pPacketString2, hashed_data_size, computed_digest);
		if (FAILED(hr)) throw(hr);
		
		// compare the signed hash and the computed hash
		if (StringUtils::StringToBinaryLength(hash)!=provided_digest.DIGEST_SIZE) throw(E_INVALIDARG);
		StringUtils::StringToBinary(hash, provided_digest.m_digest, provided_digest.DIGEST_SIZE);
		if (computed_digest!=provided_digest) throw(E_INVALIDARG);
		
		// verify the hash signature
		_variant_t vtSignature;
		VariantInit(&vtSignature);
		vtSignature.vt = VT_ARRAY | VT_UI1;
		vtSignature.parray = SafeArrayCreateVector(VT_UI1, 0, (DWORD)wcslen(signature)/2);
		BYTE *pSignatureData = 0;
		if (SUCCEEDED(hr = SafeArrayAccessData(vtSignature.parray, (void**)&pSignatureData)))
		{
			StringUtils::StringToBinary(signature, pSignatureData, (DWORD)wcslen(signature)/2);
			SafeArrayUnaccessData(vtSignature.parray);
		}
		hr = context.VerifySignature(computed_digest, &vtSignature, signature_verify_key);
		if (FAILED(hr)) throw(hr);

		// passed all validatity checks of the license packet.  Apply License Packet
		hr = ApplyLicensePacketInternal(licensePacketAttribsStream, verification);
		if (FAILED(hr)) throw(hr);

		// password entry succeeded, provide the verification code to the user
		*pBstrVerificationCode = verification.Detach();

//swprintf_s(tmpbuf, BUF_SIZE, L"SoftwareServer::EnterSoftwareLicPacket() - pBstrVerificationCode: %s", (wchar_t*)*pBstrVerificationCode);
//OutputDebugString(tmpbuf);

		// need to dispatch message, not sure how at this level...
		g_licenseController.GenerateMessage(L"", MT_INFO, S_OK, time(0), MessagePasswordPacketVerificationCode, (wchar_t*)*pBstrVerificationCode);
	}
	catch (HRESULT &ehr)
	{
		hr = ehr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	

	if (pPacketString1) delete [] pPacketString1;
	if (pPacketString2) delete [] pPacketString2;

//swprintf_s(tmpbuf, BUF_SIZE, L"SoftwareServer::EnterSoftwareLicPacket() - Leave");
//OutputDebugString(tmpbuf);
	return hr;
}

HRESULT SoftwareServer::GenerateVerifyDataWithVerCode_ByLicense(BSTR softwareLicense, VARIANT* pVtLicensePacket)
{
	HRESULT hr(E_INVALIDARG);
	try
	{
		Lic_PackageAttribs::Lic_LicenseInfoAttribs origLicenseInfoAttribs;
		hr = GetSoftwareLicenseInfo_ByLicenseInternal(softwareLicense, &origLicenseInfoAttribs);
		if(FAILED(hr))
			throw hr;

		Lic_PackageAttribs licensePackageAttribs;
		licensePackageAttribs.licLicenseInfoAttribs.customerID = origLicenseInfoAttribs.customerID;
		licensePackageAttribs.licLicenseInfoAttribs.destinationID = origLicenseInfoAttribs.destinationID;
		licensePackageAttribs.licLicenseInfoAttribs.softwareGroupLicenseID = origLicenseInfoAttribs.softwareGroupLicenseID;
		licensePackageAttribs.licLicenseInfoAttribs.softwareLicType = origLicenseInfoAttribs.softwareLicType;
		licensePackageAttribs.licLicenseInfoAttribs.softwareLicTypeIndex = origLicenseInfoAttribs.softwareLicTypeIndex;
		licensePackageAttribs.licLicenseInfoAttribs.activationAmountInDays = origLicenseInfoAttribs.activationAmountInDays;
		licensePackageAttribs.licLicenseInfoAttribs.activationCurrent = origLicenseInfoAttribs.activationCurrent;
		licensePackageAttribs.licLicenseInfoAttribs.activationExpirationDate = origLicenseInfoAttribs.activationExpirationDate;
		licensePackageAttribs.licLicenseInfoAttribs.activationTotal = origLicenseInfoAttribs.activationTotal;

		// Set a variable in licenseInfoAttribs to customer copy
		Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs tmpValTokenAttribs;
		tmpValTokenAttribs.tokenType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttTypeCopyFromCustomerOnly;
		tmpValTokenAttribs.tokenValue = std::wstring(L"true");
		licensePackageAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->push_back(tmpValTokenAttribs);

		//Get the last verification code
		if(origLicenseInfoAttribs.licVerificationAttribs.verificationCodeHistoryList->size() != 0)
			licensePackageAttribs.licLicenseInfoAttribs.licVerificationAttribs.verificationCodeHistoryList->push_back(origLicenseInfoAttribs.licVerificationAttribs.verificationCodeHistoryList->back());

		//Populate the modified date
		wchar_t timestamp[256];
		SYSTEMTIME currentSystime;
		GetSystemTime(&currentSystime);	//Retrieves Universal Time
		TimeHelper::SystemTimeToString(timestamp, _countof(timestamp), currentSystime);
		licensePackageAttribs.licLicenseInfoAttribs.modifiedDate = std::wstring(timestamp);
	
		BSTR bstrLicAttribsStream = SysAllocString(licensePackageAttribs.ToString().c_str());
		_bstr_t bstr_license_verify_data_code;
		bstr_license_verify_data_code = (wchar_t*)(&license_verify_data_code_int[0]);
		for(int idx=1; idx<32; idx++)
			bstr_license_verify_data_code += (wchar_t*)(&license_verify_data_code_int[2*idx]);
		hr = CryptoAndFlateHelper::StreamToEncryptCompressByteArray(bstr_license_verify_data_code, crypto_key_verify_data_private, sizeof(crypto_key_verify_data_private)/sizeof(BYTE), crypto_key_verify_data_password, sizeof(crypto_key_verify_data_password)/sizeof(BYTE), bstrLicAttribsStream, pVtLicensePacket);
		SysFreeString(bstrLicAttribsStream);
		if(FAILED(hr))
			throw hr;
	}
	catch (HRESULT &ehr)
	{
		hr = ehr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	return hr;
}
HRESULT SoftwareServer::GenerateVerifyDataWithLicInfo_ByLicense(BSTR softwareLicense, VARIANT* pVtLicensePacket)
{
	HRESULT hr(E_INVALIDARG);
	try
	{
		SafeMutex mutex(SoftwareLicenseLock);
		Lic_PackageAttribs licensePackageAttribs;
		hr = GetSoftwareLicenseInfo_ByLicenseInternal(softwareLicense, &licensePackageAttribs.licLicenseInfoAttribs);
		if(FAILED(hr))
			throw hr;

		//
		// Set a variable in licenseInfoAttribs to customer copy
		Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs tmpValTokenAttribs;
		tmpValTokenAttribs.tokenType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttTypeCopyFromCustomerOnly;
		tmpValTokenAttribs.tokenValue = std::wstring(L"true");
		licensePackageAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->push_back(tmpValTokenAttribs);

		//Populate the modified date
		wchar_t timestamp[256];
		SYSTEMTIME currentSystime;
		GetSystemTime(&currentSystime);	//Retrieves Universal Time
		TimeHelper::SystemTimeToString(timestamp, _countof(timestamp), currentSystime);
		licensePackageAttribs.licLicenseInfoAttribs.modifiedDate = std::wstring(timestamp);

		BSTR bstrLicAttribsStream = SysAllocString(licensePackageAttribs.ToString().c_str());
		_bstr_t bstr_license_verify_data_code;
		bstr_license_verify_data_code = (wchar_t*)(&license_verify_data_code_int[0]);
		for(int idx=1; idx<32; idx++)
			bstr_license_verify_data_code += (wchar_t*)(&license_verify_data_code_int[2*idx]);
		hr = CryptoAndFlateHelper::StreamToEncryptCompressByteArray(bstr_license_verify_data_code, crypto_key_verify_data_private, sizeof(crypto_key_verify_data_private)/sizeof(BYTE), crypto_key_verify_data_password, sizeof(crypto_key_verify_data_password)/sizeof(BYTE), bstrLicAttribsStream, pVtLicensePacket);
		SysFreeString(bstrLicAttribsStream);
		if(FAILED(hr))
			throw hr;
	}
	catch (HRESULT &ehr)
	{
		hr = ehr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	return hr;
}

HRESULT SoftwareServer::GenerateLicenseSystemData(VARIANT* pVtLicSysDataPacket)
{
	HRESULT hr(E_INVALIDARG);
	try
	{
		SafeMutex mutex(SoftwareLicenseLock);
		Lic_LicenseSystemAttribs licSystemAttribs;
		
		SpdAttribs::BufferObj bufObj;
		ProtectionKey* pTmpProKey = NULL;
		KeySpec keyspec;
		{
			SafeMutex mutex1(pRainbowDriver->keys_lock);
			pRainbowDriver->RefreshKeyList();
			
			//wchar_t tmpbuf[1024];
			unsigned short shortValue;
			byte valueByte[16];
			int arrayIdx = 0;

			// Cycle through all the protection keys
			for (RainbowDriver::KeyList::iterator dkey = pRainbowDriver->keys.begin(); dkey!=pRainbowDriver->keys.end(); ++dkey)
			{
				Lic_KeyAttribs licKeyAttribs;


				unsigned short curActivations;
				hr = pRainbowDriver->GetSoftwareCurrentActivations(dkey->first, &curActivations);
				licKeyAttribs.currentActivations = curActivations;

				BSTR bstrKeyCode;
				hr = pRainbowDriver->GetSoftwareKeyCode(dkey->first, &bstrKeyCode);
				if(SUCCEEDED(hr))
				{
					licKeyAttribs.verificationCode = std::wstring(bstrKeyCode);
					SysFreeString(bstrKeyCode);
				}

				//find modified date from key
				time_t modifiedTimeT = -1;
				pRainbowDriver->GetSoftwareModifiedDate(dkey->first, &modifiedTimeT);
				if(SUCCEEDED(hr) && modifiedTimeT != -1)
				{
					SYSTEMTIME modifiedSystime;
					VARIANT vtModified;
					vtModified = TimeHelper::TimeTToVariant(modifiedTimeT);
					VariantTimeToSystemTime(vtModified.date, &modifiedSystime);
					wchar_t timestamp[256];
					TimeHelper::SystemTimeToString(timestamp, _countof(timestamp), modifiedSystime);
					licKeyAttribs.modifiedDate = std::wstring(timestamp);
				}
				licKeyAttribs.keyName = std::wstring(dkey->first);

				// Cycle through all the cells of a given key
				for(unsigned short cell = 0; cell<64; cell++)
				{
					shortValue = 0;
					hr = pRainbowDriver->ReadCell(dkey->first, cell, &shortValue);
					arrayIdx = (2 * cell) % 16;

					valueByte[arrayIdx] = SUCCEEDED(hr) ? shortValue>>8 : 0;
					valueByte[arrayIdx+1] = SUCCEEDED(hr) ? (byte)shortValue : 0;

//swprintf_s(tmpbuf, 1024, L" SoftwareServer::GenerateLicenseSystemData() - key: %s, cell: 0x%x, shortValue: 0x%x, valueByte[0x%x]: 0x%x, valueByte[0x%x]: 0x%x", (wchar_t*)dkey->first, cell, shortValue, arrayIdx, valueByte[arrayIdx], arrayIdx+1, valueByte[arrayIdx+1]);
//OutputDebugString(tmpbuf);
					if(cell % 8 == 7)
					{
						SpdAttribs::CBuffer cBuffer;
						cBuffer.SetBuffer((byte*)&valueByte, 16);
						licKeyAttribs.layout->push_back(cBuffer);
					}
				}

				//add key to list
				licSystemAttribs.ListOfStreamed_KeyAttribs->push_back(licKeyAttribs.ToString());
			}
		}	// End of scope for SafeMutex mutex1(pRainbowDriver->keys_lock);
		
		// Add Server Data
		BSTR bstrServerDataAttrbsStream = NULL;
		hr = licServerDataMgr.GetLicServerDataAttrbsStream(&bstrServerDataAttrbsStream);
		if(SUCCEEDED(hr))
		{
			licSystemAttribs.Streamed_ServerDataAttribs = std::wstring(bstrServerDataAttrbsStream);
			SysFreeString(bstrServerDataAttrbsStream);	//Clean up
		}

		//Populate the created date
		wchar_t timestamp[256];
		SYSTEMTIME currentSystime;
		GetSystemTime(&currentSystime);	//Retrieves Universal Time
		TimeHelper::SystemTimeToString(timestamp, _countof(timestamp), currentSystime);
		licSystemAttribs.createdDate = std::wstring(timestamp);

		// Cycle through all the Software Licenses
		Lic_PackageAttribs::Lic_LicenseInfoAttribs licInfoAttribs;
		for(SoftwareLicList::iterator swLicIt=softwareLicMgrMap.begin(); swLicIt!=softwareLicMgrMap.end(); swLicIt++)
		{
			hr = (*swLicIt).second->GetLicenseInfo(&licInfoAttribs);
			BSTR bstrTmpAttribsStream = SysAllocString(licInfoAttribs.ToString().c_str());
			SysFreeString(bstrTmpAttribsStream);

			BSTR bstrSoftwareLicenseName;
			hr = (*swLicIt).second->GetSoftwareLicenseName(&bstrSoftwareLicenseName);
			if(SUCCEEDED(hr))
				licSystemAttribs.ListOfStreamed_InfoAttribs->push_back(licInfoAttribs.ToString());
			SysFreeString(bstrSoftwareLicenseName);
		}	

		//
		// Get System Info
		Lic_SystemInfoAttribs sysInfoAttribs;

		wchar_t wLicSvrVersion[256];
		wsprintf(wLicSvrVersion, L"%d.%d.%d", MAJOR_REVISION_NUMBER, MINOR_REVISION_NUMBER, BUILD_NUMBER);
		sysInfoAttribs.licenseServerVersion = std::wstring(wLicSvrVersion);

		WCHAR localComputerName[1024];
		DWORD localComputerNameSize = 1024;
		if (GetComputerNameW(localComputerName, &localComputerNameSize))
			sysInfoAttribs.computerNameList->push_back(localComputerName);
		//Get IPs also
		std::vector<_bstr_t> ipList;
		if(SUCCEEDED(NetworkUtils::SolimarNetworkHelper::GetIPAddresses(localComputerName, ipList)))
		{
			for(std::vector<_bstr_t>::iterator ipIt = ipList.begin(); ipIt != ipList.end(); ipIt++)
				sysInfoAttribs.computerNameList->push_back(std::wstring(*ipIt));
		}

		WMIHelper::ResultsType NetworkAdaptersList;
		// Hide the string so it doesn't appear in the strings of the image - "\\localhost\root\cimv2"
		BYTE wmiLocation_Byte[] = {
			0x5c, 0x00, 0x5c, 0x00, 0x6c, 0x00, 0x6f, 0x00, 0x63, 0x00, 0x61, 0x00, 0x6c, 0x00, 0x68, 0x00, 0x6f, 0x00, 0x73, 0x00, 0x74, 0x00, 0x5c, 0x00, 0x72, 0x00,
			0x6f, 0x00, 0x6f, 0x00, 0x74, 0x00, 0x5c, 0x00, 0x63, 0x00, 0x69, 0x00, 0x6d, 0x00, 0x76, 0x00, 0x32, 0x00, 0x00, 0x00
		};
		WMIHelper wmi((wchar_t*)wmiLocation_Byte);
		hr = wmi.Connect();
		if(SUCCEEDED(hr))
		{
			// gather processor information
			// Hide the string so it doesn't appear in the strings of the image - "SELECT MACAddress FROM Win32_NetworkAdapterConfiguration Where IPEnabled = True
			BYTE wmiQuery_MacAddressByte[] = {
				0x53, 0x00, 0x45, 0x00, 0x4c, 0x00, 0x45, 0x00, 0x43, 0x00, 0x54, 0x00, 0x20, 0x00, 0x4d, 0x00, 0x41, 0x00, 0x43, 0x00, 0x41, 0x00, 0x64, 0x00, 0x64, 0x00, 0x72, 0x00, 0x65, 0x00, 0x73, 0x00,
				0x73, 0x00, 0x20, 0x00, 0x46, 0x00, 0x52, 0x00, 0x4f, 0x00, 0x4d, 0x00, 0x20, 0x00, 0x57, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x33, 0x00, 0x32, 0x00, 0x5f, 0x00, 0x4e, 0x00, 0x65, 0x00, 0x74, 0x00,
				0x77, 0x00, 0x6f, 0x00, 0x72, 0x00, 0x6b, 0x00, 0x41, 0x00, 0x64, 0x00, 0x61, 0x00, 0x70, 0x00, 0x74, 0x00, 0x65, 0x00, 0x72, 0x00, 0x43, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x66, 0x00, 0x69, 0x00,
				0x67, 0x00, 0x75, 0x00, 0x72, 0x00, 0x61, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x20, 0x00, 0x57, 0x00, 0x68, 0x00, 0x65, 0x00, 0x72, 0x00, 0x65, 0x00, 0x20, 0x00, 0x49, 0x00,
				0x50, 0x00, 0x45, 0x00, 0x6e, 0x00, 0x61, 0x00, 0x62, 0x00, 0x6c, 0x00, 0x65, 0x00, 0x64, 0x00, 0x20, 0x00, 0x3d, 0x00, 0x20, 0x00, 0x54, 0x00, 0x72, 0x00, 0x75, 0x00, 0x65, 0x00, 0x00, 0x00
			};
			hr = wmi.ExecuteQuery((wchar_t*)wmiQuery_MacAddressByte, NetworkAdaptersList);
			if(SUCCEEDED(hr) && NetworkAdaptersList.size()>0)
			{
				for (unsigned int idx=0; idx<NetworkAdaptersList.size(); ++idx)
				{
					// Hide the string so it doesn't appear in the strings of the image - "MACAddress"
					BYTE macAddressByte[] = {
						0x4d, 0x00, 0x41, 0x00, 0x43, 0x00, 0x41, 0x00, 0x64, 0x00, 0x64, 0x00, 0x72, 0x00, 0x65, 0x00, 0x73, 0x00, 0x73, 0x00, 0x00, 0x00
					};
					VARIANT* pvtMacAddress = &NetworkAdaptersList[idx][(wchar_t*)macAddressByte];
					if (pvtMacAddress->vt == VT_BSTR)
						sysInfoAttribs.macAddressList->push_back(std::wstring(pvtMacAddress->bstrVal));
				}
				WMIHelper::Clear(NetworkAdaptersList);
				NetworkAdaptersList.clear();
			}
			hr = S_OK;
		}

		if(SUCCEEDED(hr))
		{
			// gather processor information
			WMIHelper::ResultsType BiosList;
			// Hide the string so it doesn't appear in the strings of the image - "SELECT SerialNumber FROM Win32_BIOS"
			BYTE wmiQuery_SerialByte[] = {
				0x53, 0x00, 0x45, 0x00, 0x4c, 0x00, 0x45, 0x00, 0x43, 0x00, 0x54, 0x00, 0x20, 0x00, 0x53, 0x00, 0x65, 0x00, 0x72, 0x00, 0x69, 0x00, 0x61, 0x00, 0x6c, 0x00, 0x4e, 0x00, 0x75, 0x00, 0x6d, 0x00,
				0x62, 0x00, 0x65, 0x00, 0x72, 0x00, 0x20, 0x00, 0x46, 0x00, 0x52, 0x00, 0x4f, 0x00, 0x4d, 0x00, 0x20, 0x00, 0x57, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x33, 0x00, 0x32, 0x00, 0x5f, 0x00, 0x42, 0x00,
				0x49, 0x00, 0x4f, 0x00, 0x53, 0x00, 0x00
			};
			hr = wmi.ExecuteQuery((wchar_t*)wmiQuery_SerialByte, BiosList);
			if(SUCCEEDED(hr) && BiosList.size()>0)
			{
				for (unsigned int idx=0; idx<BiosList.size(); ++idx)
				{
					// Hide the string so it doesn't appear in the strings of the image - "SerialNumber"
					BYTE serialByte[] = {
						0x53, 0x00, 0x65, 0x00, 0x72, 0x00, 0x69, 0x00, 0x61, 0x00, 0x6c, 0x00, 0x4e, 0x00, 0x75, 0x00, 0x6d, 0x00, 0x62, 0x00, 0x65, 0x00, 0x72, 0x00, 0x00, 0x00
					};
					VARIANT* pvtBiosSerialNumber = &BiosList[idx][(wchar_t*)serialByte];
					if (pvtBiosSerialNumber->vt == VT_BSTR)
					{
						sysInfoAttribs.biosSerialNumberList->push_back(std::wstring(pvtBiosSerialNumber->bstrVal));
					}
				}
				WMIHelper::Clear(BiosList);
				BiosList.clear();
			}
		}
		licSystemAttribs.Streamed_SoftwareSpecAttribs = std::wstring(g_pSoftwareSpec->GetSoftwareSpec().ToString());
		licSystemAttribs.Streamed_SystemInfoAttribs = std::wstring(sysInfoAttribs.ToString());

		BSTR bstrLicAttribsStream = SysAllocString(licSystemAttribs.ToString().c_str());
//OutputDebugString(bstrLicAttribsStream);

		_bstr_t bstr_license_verify_data_code;
		bstr_license_verify_data_code = (wchar_t*)(&license_verify_data_code_int[0]);
		for(int idx=1; idx<32; idx++)
			bstr_license_verify_data_code += (wchar_t*)(&license_verify_data_code_int[2*idx]);
		hr = CryptoAndFlateHelper::StreamToEncryptCompressByteArray(bstr_license_verify_data_code, crypto_key_verify_data_private, sizeof(crypto_key_verify_data_private)/sizeof(BYTE), crypto_key_verify_data_password, sizeof(crypto_key_verify_data_password)/sizeof(BYTE), bstrLicAttribsStream, pVtLicSysDataPacket);
		SysFreeString(bstrLicAttribsStream);

		if(FAILED(hr))
			throw hr;
	}
	catch (HRESULT &ehr)
	{
		hr = ehr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	return hr;
}
HRESULT SoftwareServer::GenerateStream_ByLicenseSystemData(VARIANT vtLicSysDataPacket, BSTR *pBstrLicSysDataAttribsStream)
{
	HRESULT hr(E_INVALIDARG);
	try
	{
		SafeMutex mutex(SoftwareLicenseLock);
		_bstr_t bstr_license_verify_data_code;
		bstr_license_verify_data_code = (wchar_t*)(&license_verify_data_code_int[0]);
		for(int idx=1; idx<32; idx++)
			bstr_license_verify_data_code += (wchar_t*)(&license_verify_data_code_int[2*idx]);
		hr = CryptoAndFlateHelper::EncryptCompressByteArrayToStream(bstr_license_verify_data_code, crypto_key_verify_data_public, sizeof(crypto_key_verify_data_public)/sizeof(BYTE), crypto_key_verify_data_password, sizeof(crypto_key_verify_data_password)/sizeof(BYTE), vtLicSysDataPacket, pBstrLicSysDataAttribsStream);
	}
	catch (HRESULT &ehr)
	{
		hr = ehr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	return hr;
}
HRESULT SoftwareServer::GenerateLicPackage_ByVerifyData(VARIANT vtVerifyData, BSTR *pBstrLicensePackageAttribsStream)
{
	HRESULT hr(E_INVALIDARG);
	try
	{
		SafeMutex mutex(SoftwareLicenseLock);
		_bstr_t bstr_license_verify_data_code;
		bstr_license_verify_data_code = (wchar_t*)(&license_verify_data_code_int[0]);
		for(int idx=1; idx<32; idx++)
			bstr_license_verify_data_code += (wchar_t*)(&license_verify_data_code_int[2*idx]);
		hr = CryptoAndFlateHelper::EncryptCompressByteArrayToStream(bstr_license_verify_data_code, crypto_key_verify_data_public, sizeof(crypto_key_verify_data_public)/sizeof(BYTE), crypto_key_verify_data_password, sizeof(crypto_key_verify_data_password)/sizeof(BYTE), vtVerifyData, pBstrLicensePackageAttribsStream);
	}
	catch (HRESULT &ehr)
	{
		hr = ehr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	return hr;
	
}
HRESULT SoftwareServer::GenerateLicPackage_BySoftwareLicArchive(VARIANT vtLicenseArchive, BSTR *pBstrLicensePackageAttribsStream)
{
	HRESULT hr(E_INVALIDARG);
	try
	{
		SafeMutex mutex(SoftwareLicenseLock);
		_bstr_t bstr_license_archive_code;
		bstr_license_archive_code = (wchar_t*)(&license_archive_code_int[0]);
		for(int idx=1; idx<32; idx++)
			bstr_license_archive_code += (wchar_t*)(&license_archive_code_int[2*idx]);
		hr = CryptoAndFlateHelper::EncryptCompressByteArrayToStream(bstr_license_archive_code, crypto_key_license_archive_public, sizeof(crypto_key_license_archive_public)/sizeof(BYTE), crypto_key_license_archive_password, sizeof(crypto_key_license_archive_password)/sizeof(BYTE), vtLicenseArchive, pBstrLicensePackageAttribsStream);
	}
	catch (HRESULT &ehr)
	{
		hr = ehr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	return hr;
}
HRESULT SoftwareServer::GenerateLicPackage_BySoftwareLicPacket(VARIANT vtLicensePacket, BSTR *pBstrLicensePackageAttribsStream)
{
	HRESULT hr(E_FAIL);
	try
	{
		SafeMutex mutex(SoftwareLicenseLock);
		_bstr_t bstr_license_packet_code;
		bstr_license_packet_code = (wchar_t*)(&license_packet_code_int[0]);
		for(int idx=1; idx<32; idx++)
			bstr_license_packet_code += (wchar_t*)(&license_packet_code_int[2*idx]);
		hr = CryptoAndFlateHelper::EncryptCompressByteArrayToStream(bstr_license_packet_code, crypto_key_password_packet_public, sizeof(crypto_key_password_packet_public)/sizeof(BYTE), crypto_key_password_packet_password, sizeof(crypto_key_password_packet_password)/sizeof(BYTE), vtLicensePacket, pBstrLicensePackageAttribsStream, true/*bExpectDateInByteArray*/);
	}
	catch (HRESULT &ehr)
	{
		hr = ehr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	return hr;
}
HRESULT GeneratePasswordFile()
{
	HRESULT hr(S_OK);
	//wchar_t tmpbuf[1024];
	try
	{
		DWORD lpNumberOfBytesWritten;
		std::wstring wStrFullBuffer = std::wstring(L"");
		for(int idx=0; idx<64; idx++)
		{
			GUID guid;
			CoCreateGuid(&guid);
			// convert it to a char buffer
			OLECHAR buffer[100];
			int sizeOfBuffer = StringFromGUID2(guid, buffer, 100);
			std::wstring wStrBuffer = std::wstring(buffer);
			size_t chPos = wStrBuffer.find(L"-");
			while(chPos != std::wstring::npos)
			{
				wStrBuffer = wStrBuffer.erase(chPos, 1);
				chPos = wStrBuffer.find(L"-");
			}
			chPos = wStrBuffer.find(L"{");
			if(chPos != std::wstring::npos)
				wStrBuffer = wStrBuffer.erase(chPos, 1);
			chPos = wStrBuffer.find(L"}");
			if(chPos != std::wstring::npos)
				wStrBuffer = wStrBuffer.erase(chPos, 1);
			wStrFullBuffer.append(wStrBuffer);
		}
		int sizeOfByteBuffer = StringUtils::StringToBinaryLength(wStrFullBuffer.c_str());
		BYTE byteBuffer[2024];
		StringUtils::StringToBinary(wStrFullBuffer.c_str(), &byteBuffer[0], sizeOfByteBuffer);
		HANDLE hPrivateKeyFile = CreateFileW(L"raw.password", GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, 0, 0);
		if (hPrivateKeyFile != INVALID_HANDLE_VALUE)
		{
			bool bWriteSucceeded = !WriteFile(
								hPrivateKeyFile, 
								(LPCVOID)byteBuffer,
								sizeOfByteBuffer, 
								&lpNumberOfBytesWritten, 
								NULL
						);
			CloseHandle(hPrivateKeyFile);
		}

		HANDLE hPrivateKeyTxtFile = CreateFileW(L"raw.password.txt", GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, 0, 0);
		if (hPrivateKeyTxtFile != INVALID_HANDLE_VALUE)
		{
			char tmpBuff[10];
			int tmpBuffLength = 0;
			for(int idx = 0; idx < sizeOfByteBuffer; idx++)
			{
				if(idx == sizeOfByteBuffer-1)
					tmpBuffLength = _snprintf_s(tmpBuff, sizeof(tmpBuff)/sizeof(char), "0x%02x", byteBuffer[idx]);
				else
					tmpBuffLength = _snprintf_s(tmpBuff, sizeof(tmpBuff)/sizeof(char), "0x%02x, ", byteBuffer[idx]);
				WriteFile(
								hPrivateKeyTxtFile, 
								(LPCVOID)tmpBuff,
								tmpBuffLength,
								&lpNumberOfBytesWritten, 
								NULL
						);
				if((idx+1)%16==0)
				{
					tmpBuffLength = _snprintf_s(tmpBuff, sizeof(tmpBuff)/sizeof(char), "\r\n");
					WriteFile(
								hPrivateKeyTxtFile, 
								(LPCVOID)tmpBuff,
								tmpBuffLength,
								&lpNumberOfBytesWritten, 
								NULL
						);
				}
			}
			CloseHandle(hPrivateKeyTxtFile);
		}
	}
	catch (HRESULT &ehr)
	{
		hr = ehr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	return hr;

}
HRESULT GenerateNewKeys()
{
	HRESULT hr(S_OK);
	//wchar_t tmpbuf[1024];
	try
	{
		//For Testing - trying to generate new keys...
		CryptoHelper tmpContext;
		//CryptoHelper::Key* tmpKey = new CryptoHelper::Key(tmpContext, 2048);
		CryptoHelper::Key* tmpKey = new CryptoHelper::Key(tmpContext);

		DWORD lpNumberOfBytesWritten;
		DWORD publicBufferSize(0);
		BYTE *pPublicBuffer = 0;

		DWORD privateBufferSize(0);
		BYTE *pPrivateBuffer = 0;
		hr = tmpKey->ExportPrivateKeySize(&privateBufferSize);
		if(FAILED(hr)) 
			throw hr;
		//swprintf_s(tmpbuf, 1024, L"GenerateNewKeys() - privateBufferSize: %d", privateBufferSize);
		//OutputDebugString(tmpbuf);
		pPrivateBuffer = new BYTE[privateBufferSize];
		//memcpy(pPrivateBuffer, 0, (privateBufferSize)*sizeof(char));
		hr = tmpKey->ExportPrivateKey(pPrivateBuffer, privateBufferSize);

		HANDLE hPrivateKeyFile = CreateFileW(L"private.key", GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, 0, 0);
		if (hPrivateKeyFile != INVALID_HANDLE_VALUE)
		{
			WriteFile(
								hPrivateKeyFile, 
								(LPCVOID)pPrivateBuffer, 
								privateBufferSize, 
								&lpNumberOfBytesWritten, 
								NULL
						);
			CloseHandle(hPrivateKeyFile);
		}
		HANDLE hPrivateKeyTxtFile = CreateFileW(L"private.key.txt", GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, 0, 0);
		if (hPrivateKeyTxtFile != INVALID_HANDLE_VALUE)
		{
			char tmpBuff[10];
			int tmpBuffLength = 0;
			for(unsigned int idx = 0; idx < privateBufferSize; idx++)
			{
				if(idx == privateBufferSize-1)
					tmpBuffLength = _snprintf_s(tmpBuff, sizeof(tmpBuff)/sizeof(char), "0x%02x", pPrivateBuffer[idx]);
				else
					tmpBuffLength = _snprintf_s(tmpBuff, sizeof(tmpBuff)/sizeof(char), "0x%02x, ", pPrivateBuffer[idx]);
				WriteFile(
								hPrivateKeyTxtFile, 
								(LPCVOID)tmpBuff,
								tmpBuffLength,
								&lpNumberOfBytesWritten, 
								NULL
						);
				if((idx+1)%16==0)
				{
					tmpBuffLength = _snprintf_s(tmpBuff, sizeof(tmpBuff)/sizeof(char), "\r\n");
					WriteFile(
								hPrivateKeyTxtFile, 
								(LPCVOID)tmpBuff,
								tmpBuffLength,
								&lpNumberOfBytesWritten, 
								NULL
						);
				}
			}
			CloseHandle(hPrivateKeyTxtFile);
		}
		delete pPrivateBuffer;

		hr = tmpKey->ExportPublicKeySize(&publicBufferSize);
		if(FAILED(hr)) 
			throw hr;
		//swprintf_s(tmpbuf, 1024, L"GenerateNewKeys() - publicBufferSize: %d", publicBufferSize);
		//OutputDebugString(tmpbuf);
		pPublicBuffer = new BYTE[publicBufferSize];
		hr = tmpKey->ExportPublicKey(pPublicBuffer, publicBufferSize);
		HANDLE hPublicKeyFile = CreateFileW(L"public.key", GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, 0, 0);
		if (hPublicKeyFile != INVALID_HANDLE_VALUE)
		{
			WriteFile(
								hPublicKeyFile, 
								(LPCVOID)pPublicBuffer, 
								publicBufferSize, 
								&lpNumberOfBytesWritten, 
								NULL
						);
			CloseHandle(hPublicKeyFile);
		}
		HANDLE hPublicKeyTxtFile = CreateFileW(L"public.key.txt", GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, 0, 0);
		if (hPublicKeyFile != INVALID_HANDLE_VALUE)
		{
			char tmpBuff[10];
			int tmpBuffLength = 0;
			for(unsigned int idx = 0; idx < publicBufferSize; idx++)
			{
				if(idx == publicBufferSize-1)
					tmpBuffLength = _snprintf_s(tmpBuff, sizeof(tmpBuff)/sizeof(char), "0x%02x", pPublicBuffer[idx]);
				else
					tmpBuffLength = _snprintf_s(tmpBuff, sizeof(tmpBuff)/sizeof(char), "0x%02x, ", pPublicBuffer[idx]);
				WriteFile(
								hPublicKeyTxtFile, 
								(LPCVOID)tmpBuff,
								tmpBuffLength,
								&lpNumberOfBytesWritten, 
								NULL
						);
				if((idx+1)%16==0)
				{
					tmpBuffLength = _snprintf_s(tmpBuff, sizeof(tmpBuff)/sizeof(char), "\r\n");
					WriteFile(
								hPublicKeyTxtFile, 
								(LPCVOID)tmpBuff,
								tmpBuffLength,
								&lpNumberOfBytesWritten, 
								NULL
						);
				}
			}
			CloseHandle(hPublicKeyTxtFile);
		}
		delete pPublicBuffer;
	}
	catch (HRESULT &ehr)
	{
		hr = ehr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	return hr;
}
HRESULT SoftwareServer::GenerateSoftwareLicArchive_ByLicense(BSTR softwareLicense, VARIANT* pVtLicArchive)
{
	HRESULT hr(E_INVALIDARG);
	try
	{
		SafeMutex mutex(SoftwareLicenseLock);
		Lic_PackageAttribs licensePackageAttribs;
		hr = GetSoftwareLicenseInfo_ByLicenseInternal(softwareLicense, &licensePackageAttribs.licLicenseInfoAttribs);
		if(FAILED(hr))
			throw hr;

		std::wstring newKeyID = Lic_PackageAttribsHelper::GetValidationValue(&(licensePackageAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList), Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttHardwareKeyID);
		if(newKeyID.length() == 0)
			throw LicenseServerError::EHR_LIC_SOFTWARE_CANT_GENERATE_LIC_ARCHIVE_NO_KEY;

		//make sure that if there is a licenseID, it must match because if it doesn't match that means that the license file might have been replaced with an older one
		std::wstring tmpLicenseCode = Lic_PackageAttribsHelper::GetValidationValue(&(licensePackageAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList), Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttLicenseCode);
		if(tmpLicenseCode.length() != 0)
		{
			SoftwareLicenseMgr* pSwLicMgr = GetSoftwareLicenseMgr_ByLicenseInternal(softwareLicense);
			if(pSwLicMgr != NULL)
			{
				hr = pSwLicMgr->ValidateLicenseCode(tmpLicenseCode.c_str());
				if(FAILED(hr))
					throw hr;
			}
		}

		// Set a variable in licenseInfoAttribs to license archive
		Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs tmpVerTokenAttribs;
		tmpVerTokenAttribs.tokenType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttTypeArchiveOnly;
		tmpVerTokenAttribs.tokenValue = std::wstring(L"true");
		licensePackageAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->push_back(tmpVerTokenAttribs);

		//Populate the modified date
		wchar_t timestamp[256];
		SYSTEMTIME currentSystime;
		GetSystemTime(&currentSystime);	//Retrieves Universal Time
		TimeHelper::SystemTimeToString(timestamp, _countof(timestamp), currentSystime);
		licensePackageAttribs.licLicenseInfoAttribs.modifiedDate = std::wstring(timestamp);

		BSTR bstrLicAttribsStream = SysAllocString(licensePackageAttribs.ToString().c_str());
		_bstr_t bstr_license_archive_code;
		bstr_license_archive_code = (wchar_t*)(&license_archive_code_int[0]);
		for(int idx=1; idx<32; idx++)
			bstr_license_archive_code += (wchar_t*)(&license_archive_code_int[2*idx]);
		hr = CryptoAndFlateHelper::StreamToEncryptCompressByteArray(bstr_license_archive_code, crypto_key_license_archive_private, sizeof(crypto_key_license_archive_private)/sizeof(BYTE), crypto_key_license_archive_password, sizeof(crypto_key_license_archive_password)/sizeof(BYTE), bstrLicAttribsStream, pVtLicArchive);
		SysFreeString(bstrLicAttribsStream);
		if(FAILED(hr))
			throw hr;

		//Delete old license object and license file from the system...
		for(	SoftwareLicList::iterator swLicMgrIt = softwareLicMgrMap.begin();
				swLicMgrIt != softwareLicMgrMap.end();
				swLicMgrIt++)
		{
			BSTR tmpBstrLicName;
			hr = swLicMgrIt->second->GetSoftwareLicenseName(&tmpBstrLicName);
			if(SUCCEEDED(hr))
			{
				if(_wcsicmp(tmpBstrLicName, softwareLicense) == 0)
				{
					hr = swLicMgrIt->second->DeleteLicenseFile();
					SysFreeString(tmpBstrLicName);
					if(FAILED(hr))
						throw hr;

					delete swLicMgrIt->second;
					swLicMgrIt->second = NULL;
					softwareLicMgrMap.erase(swLicMgrIt);

					g_licenseController.GenerateMessage(tmpBstrLicName, MT_INFO, S_OK, time(0), MessageGeneric, L"Successfully generated License Archive from Software License. The original Software License has been removed. This License Archive can be transferred to another system.");
					SysFreeString(tmpBstrLicName);
					break;
				}
				SysFreeString(tmpBstrLicName);
			}
		}
	}
	catch (HRESULT &ehr)
	{
		hr = ehr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	return hr;
}
//

HRESULT SoftwareServer::EnterSoftwareLicArchive(VARIANT vtLicArchive)
{
	HRESULT hr(E_INVALIDARG);
	SoftwareLicenseMgr* pSoftwareLicMgr = NULL;
	bool bCreatedPSoftwareLicMgr = false;
	try
	{
		SafeMutex mutex(SoftwareLicenseLock);
		
		BSTR bstrLicensePackageAttribsStream = 0;
		_bstr_t bstr_license_archive_code;
		bstr_license_archive_code = (wchar_t*)(&license_archive_code_int[0]);
		for(int idx=1; idx<32; idx++)
			bstr_license_archive_code += (wchar_t*)(&license_archive_code_int[2*idx]);
		hr = CryptoAndFlateHelper::EncryptCompressByteArrayToStream(bstr_license_archive_code, crypto_key_license_archive_public, sizeof(crypto_key_license_archive_public)/sizeof(BYTE), crypto_key_license_archive_password, sizeof(crypto_key_license_archive_password)/sizeof(BYTE), vtLicArchive, &bstrLicensePackageAttribsStream);
		if(FAILED(hr))
			throw hr;


		//Convert bstrLicenseInfoAttribsStream to 
		Lic_PackageAttribs licensePackageAttribs;
		licensePackageAttribs.InitFromString(bstrLicensePackageAttribsStream);
		SysFreeString(bstrLicensePackageAttribsStream);
		
		//Find the software license and replace it with bstrLicenseInfoAttribsStream.
		pSoftwareLicMgr = GetSoftwareLicenseMgr_ByLicenseInternal(_bstr_t(Lic_PackageAttribsHelper::GetDisplayLabel(&licensePackageAttribs.licLicenseInfoAttribs).c_str()));


		WCHAR szPath[MAX_PATH];
		_bstr_t newLicenseFile = _bstr_t(L"");

		// if no match is found, add a new SoftwareLicenseMgr 
		if(pSoftwareLicMgr == NULL)
		{
			pSoftwareLicMgr = new SoftwareLicenseMgr();
			bCreatedPSoftwareLicMgr = true;

			//Find CSIDL_COMMON_APPDATA on local system
			hr = SHGetFolderPath( NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath );
			PathAppend(szPath, L"Solimar\\Licensing\\");

			GUID key_guid;
			hr = CoCreateGuid(&key_guid);
			if (FAILED(hr))
				throw hr;
			
			wchar_t tmp_code[128];
			StringFromGUID2(key_guid, tmp_code, 128);
			tmp_code[127]=0;
			newLicenseFile = _bstr_t(tmp_code).Detach();
			pSoftwareLicMgr->Initialize(_bstr_t(szPath) + newLicenseFile, pRainbowDriver, &licServerDataMgr);
		}



		//The Protection Key in the License Archive must be present on the License Server to apply the License Archive
		std::wstring tmpKeyID = Lic_PackageAttribsHelper::GetValidationValue(&(licensePackageAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList), Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttHardwareKeyID);
		hr = pSoftwareLicMgr->ValidateHardwareKeyID(tmpKeyID.c_str());
		if(FAILED(hr))
		{
			if(hr == LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_KEY_ID)
				hr = LicenseServerError::EHR_LIC_SOFTWARE_KEY_ID_REQUIRED;
			throw hr;
		}

		
		//The License Code in the License Archive must match the License Code from the Protection Key
		std::wstring tmpLicCode = Lic_PackageAttribsHelper::GetValidationValue(&(licensePackageAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList), Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttLicenseCode);
		hr = pSoftwareLicMgr->ValidateLicenseCode(tmpLicCode.c_str(), true/*bCheckProtectionKey*/);
		if(FAILED(hr))
		{
			if(hr == LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_LICENSE_CODE)
				hr = LicenseServerError::EHR_LIC_SOFTWARE_LIC_ARCHIVE_CODE_NO_MATCH;
			throw hr;
		}

		//Can only call EnterLicenseArchive() on a license that has a Protection Key, and if the
		//License Code on the Key must match the License Code in the pLicPacket
		hr = pSoftwareLicMgr->EnterLicenseArchive(&licensePackageAttribs);
		if(FAILED(hr))
			throw hr;

		//Successfully imported licenseArchive.
		if(bCreatedPSoftwareLicMgr)
		{
			softwareLicMgrMap.insert(SoftwareLicList::value_type(_bstr_t(szPath) + newLicenseFile, pSoftwareLicMgr));
			BSTR tmpBstrLicName;
			hr = pSoftwareLicMgr->GetSoftwareLicenseName(&tmpBstrLicName);
			g_licenseController.GenerateMessage(SUCCEEDED(hr) ? tmpBstrLicName : L"", MT_INFO, S_OK, time(0), MessageGeneric, L"Successfully generated Software License from License Archive.  License transfer complete.");
			if(SUCCEEDED(hr))
			SysFreeString(tmpBstrLicName);
		}
	}
	catch (HRESULT &ehr)
	{
		hr = ehr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	if(FAILED(hr) && bCreatedPSoftwareLicMgr==true && pSoftwareLicMgr!=NULL)
	{
		delete pSoftwareLicMgr;
	}

		
	return hr;
}
HRESULT SoftwareServer::ApplyLicensePacketInternal(BSTR bstrLicPackageAttribsStream, _bstr_t bstrVerificationCode)
{
	HRESULT hr(S_OK);


	try
	{
		// use licensePacketAttribsStream and pass to SolimarSoftwareLicenseMgr
		Lic_PackageAttribs tmpLicPackageAttribs;
		tmpLicPackageAttribs.InitFromString(bstrLicPackageAttribsStream);

		SoftwareLicenseMgr* pSoftwareLicMgr = GetSoftwareLicenseMgr_ByLicenseInternal(_bstr_t(Lic_PackageAttribsHelper::GetDisplayLabel(&tmpLicPackageAttribs.licLicenseInfoAttribs).c_str()));

		WCHAR szPath[MAX_PATH];
		_bstr_t newLicenseFile = _bstr_t(L"");

		// if no match is found, add a new SoftwareLicenseMgr 
		if(pSoftwareLicMgr == NULL)
		{
			pSoftwareLicMgr = new SoftwareLicenseMgr();

			//Find CSIDL_COMMON_APPDATA on local system
			hr = SHGetFolderPath( NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath );
			PathAppend(szPath, L"Solimar\\Licensing\\");
			
			GUID key_guid;
			hr = CoCreateGuid(&key_guid);
			if (FAILED(hr))
				throw hr;
			
			wchar_t tmp_code[128];
			StringFromGUID2(key_guid, tmp_code, 128);
			tmp_code[127]=0;
			newLicenseFile = _bstr_t(tmp_code).Detach();

			pSoftwareLicMgr->Initialize(_bstr_t(szPath) + newLicenseFile, pRainbowDriver, &licServerDataMgr);
		}

		//
		hr = pSoftwareLicMgr->ApplyLicensePacket(&tmpLicPackageAttribs, bstrVerificationCode);
		if(FAILED(hr))
			throw hr;
		//Determine if pSoftwareLicMgr has a protection key as the verification token, if so then cycle through
		//all the softwareLicMgrMap and update the new license code from pSoftwareLicMgr to all the entries
		//in softwareLicMgrMap  that also have the same protection key as a verification token
		Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_VerificationAttribs licVerAttribs;
		hr = pSoftwareLicMgr->GetVerificationAttribs(&licVerAttribs);
		if(FAILED(hr))
			throw hr;

		if(_wcsicmp(newLicenseFile, L"") != 0)	//This is a new license, add to map
		{
			softwareLicMgrMap.insert(SoftwareLicList::value_type(_bstr_t(szPath) + newLicenseFile, pSoftwareLicMgr));
		}

		std::wstring newKeyID = Lic_PackageAttribsHelper::GetValidationValue(&(licVerAttribs.validationTokenList), Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttHardwareKeyID);
		if(_wcsicmp(newKeyID.c_str(), L"") != 0)	//There is a protection key ID
		{
			//Find new License Code
			std::wstring newLicenseCode = Lic_PackageAttribsHelper::GetValidationValue(&(licVerAttribs.validationTokenList), Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttLicenseCode);

			//Cycle through softwareLicMgrMap, updating all the entries that have the same protection key ID with the new License Code
			for(	SoftwareLicList::iterator swLicIt = softwareLicMgrMap.begin();
					swLicIt!=softwareLicMgrMap.end(); 
					swLicIt++)
			{
				//No need to update the same item.
				if(pSoftwareLicMgr != swLicIt->second)	
				{
					Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_VerificationAttribs tmpLicVerAttribs;
					hr = swLicIt->second->GetVerificationAttribs(&tmpLicVerAttribs);
					if(FAILED(hr))
						continue;

					std::wstring tmpKeyID = Lic_PackageAttribsHelper::GetValidationValue(&(tmpLicVerAttribs.validationTokenList), Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttHardwareKeyID);
					std::wstring tmpLicCode = Lic_PackageAttribsHelper::GetValidationValue(&(tmpLicVerAttribs.validationTokenList), Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttLicenseCode);
					if(_wcsicmp(newKeyID.c_str(), tmpKeyID.c_str()) == 0)
					{
						hr = swLicIt->second->UpdateLicenseCode(newLicenseCode.c_str());
						if(FAILED(hr))
							continue;
					}
				}
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
	return hr;
}



HRESULT SoftwareServer::ValidateToken_ByLicense(BSTR softwareLicense, long validationTokenType, BSTR validationValue)
{
	HRESULT hr(S_OK);
	SafeMutex mutex(SoftwareLicenseLock);
	SoftwareLicenseMgr* pSwLicMgr = GetSoftwareLicenseMgr_ByLicenseInternal(softwareLicense);
	if(pSwLicMgr)
	{
		switch(validationTokenType)
		{
			case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttHardwareKeyID:
				hr = pSwLicMgr->ValidateHardwareKeyID(validationValue);
				break;
			case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttLicenseCode:
				hr = pSwLicMgr->ValidateLicenseCode(validationValue);
				break;
			case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttBiosSerialNumber:
				hr = pSwLicMgr->ValidateHardwareBiosSerialNumber(validationValue);
				break;
			case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttMacAddress:
				hr = pSwLicMgr->ValidateHardwareMacAddress(validationValue);
				break;
			case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttComputerName:
				hr = pSwLicMgr->ValidateHardwareCompuerName(validationValue);
				break;
			case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttTypeCopyFromCustomerOnly:
				hr = LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_CUSTOMER_COPY;
				break;
			case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttTypeArchiveOnly:
				hr = LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_ARCHIVE;
				break;
			case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttTypePackageOnly:
				hr = LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_PACKET;
				break;
			default:
				break;
		};
	}
	else	//Not Found
	{
		hr = E_INVALIDARG;
	}
	return hr;
}



HRESULT SoftwareServer::SoftwareLicenseUseActivationToExtendTime_ByLicense(BSTR softwareLicense)
{
	HRESULT hr(E_INVALIDARG);
	SafeMutex mutex(SoftwareLicenseLock);
	try
	{
		SoftwareLicenseMgr* pSwLicMgr = GetSoftwareLicenseMgr_ByLicenseInternal(softwareLicense);
		if(!pSwLicMgr)
			throw E_INVALIDARG;

		hr = pSwLicMgr->UseActivationToExtendTime();
		if(FAILED(hr))
			throw hr;
	
		//Determine if pSwLicMgr has a protection key as the verification token, if so then cycle through
		//all the softwareLicMgrMap and update the new license code from pSoftwareLicMgr to all the entries
		//in softwareLicMgrMap  that also have the same protection key as a verification token
		Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_VerificationAttribs licVerAttribs;
		hr = pSwLicMgr->GetVerificationAttribs(&licVerAttribs);
		if(FAILED(hr))
			throw hr;

		std::wstring newKeyID = Lic_PackageAttribsHelper::GetValidationValue(&(licVerAttribs.validationTokenList), Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttHardwareKeyID);
		if(_wcsicmp(newKeyID.c_str(), L"") != 0)	//There is a protection key ID
		{
			//Find new License Code
			std::wstring newLicenseCode = Lic_PackageAttribsHelper::GetValidationValue(&(licVerAttribs.validationTokenList), Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttLicenseCode);

			//Cycle through softwareLicMgrMap, updating all the entries that have the same protection key ID with the new License Code
			for(SoftwareLicList::iterator swLicIt = softwareLicMgrMap.begin(); swLicIt!=softwareLicMgrMap.end(); swLicIt++)
			{
				//No need to update the same item.
				if(pSwLicMgr != swLicIt->second)	
				{
					Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_VerificationAttribs tmpLicVerAttribs;
					hr = swLicIt->second->GetVerificationAttribs(&tmpLicVerAttribs);
					if(FAILED(hr))
						continue;

					std::wstring tmpKeyID = Lic_PackageAttribsHelper::GetValidationValue(&(tmpLicVerAttribs.validationTokenList), Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttHardwareKeyID);
					std::wstring tmpLicCode = Lic_PackageAttribsHelper::GetValidationValue(&(tmpLicVerAttribs.validationTokenList), Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttLicenseCode);
					if(_wcsicmp(newKeyID.c_str(), tmpKeyID.c_str()) == 0)
					{
						hr = swLicIt->second->UpdateLicenseCode(newLicenseCode.c_str());
						if(FAILED(hr))
							continue;
					}
				}
			}
		}

		//Log message about activation
		wchar_t tmpbuf[1024];
		Lic_PackageAttribs::Lic_LicenseInfoAttribs tmpLicInfoAttribs;
		hr = pSwLicMgr->GetLicenseInfo(&tmpLicInfoAttribs);
		if(FAILED(hr))
			throw hr;
		
		swprintf_s(tmpbuf, 
			L"Successfully applied License Activation. Expiration Date: %s, Current Activation: %d, Total Activations: %d, Activation Amount in Days: %d", 
			std::wstring(SpdAttribs::WStringObj(tmpLicInfoAttribs.activationExpirationDate)).c_str(),
			(int)tmpLicInfoAttribs.activationCurrent, 
			(int)tmpLicInfoAttribs.activationTotal, 
			(int)tmpLicInfoAttribs.activationAmountInDays);

		g_licenseController.GenerateMessage((wchar_t*)softwareLicense, MT_INFO, S_OK, time(0), MessageGeneric, tmpbuf);
	}
	catch(HRESULT &eHr)
	{
		hr = eHr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	return hr;
}



HRESULT SoftwareServer::GetSoftwareLicenseInfo_ByLicenseInternal(BSTR softwareLicense, Lic_PackageAttribs::Lic_LicenseInfoAttribs *pLicenseInfoAttribs)
{
	HRESULT hr(S_OK);
	SoftwareLicenseMgr* pSwLicMgr = GetSoftwareLicenseMgr_ByLicenseInternal(softwareLicense);
	if(pSwLicMgr)
		hr = pSwLicMgr->GetLicenseInfo(pLicenseInfoAttribs);
	else
		hr = E_INVALIDARG;
	return hr;
}
SoftwareLicenseMgr* SoftwareServer::GetSoftwareLicenseMgr_ByLicenseInternal(BSTR softwareLicense)
{
	HRESULT hr(S_OK);
	SoftwareLicenseMgr* retVal = NULL;
	bool bFoundLicense(false);
	//Indexed by license filename, not be softwareLicense
	for(SoftwareLicList::iterator swLicMgrIt = softwareLicMgrMap.begin();
		swLicMgrIt != softwareLicMgrMap.end();
		swLicMgrIt++)
	{
		BSTR tmpBstrLicName;
		hr = swLicMgrIt->second->GetSoftwareLicenseName(&tmpBstrLicName);
		if(SUCCEEDED(hr))
		{
			if(_wcsicmp(tmpBstrLicName, softwareLicense) == 0)
			{
				retVal = swLicMgrIt->second;
				bFoundLicense = true;
				SysFreeString(tmpBstrLicName);
				break;
			}
			SysFreeString(tmpBstrLicName);
		}
	}
	return retVal;
}

//HRESULT SoftwareServer::RemoveSoftwareLicense_ByLicenseInternal(BSTR softwareLicense)
//{
//	HRESULT hr(S_OK);
//	try
//	{
//		SoftwareLicenseMgr* pSwLicMgr = GetSoftwareLicenseMgr_ByLicenseInternal(softwareLicense);
//		if(pSwLicMgr == NULL)
//			throw E_INVALIDARG;
//	}
//	catch(HRESULT &eHr)
//	{
//		hr = eHr;
//	}
//	catch (...)
//	{
//		hr = E_FAIL;
//	}
//	return hr;
//}

int m_uniqueValue = 0;

//if softwareLicense is L"", will try to add to first license file it finds, if it can't find one then will create new license file.
HRESULT SoftwareServer::ConvertProtectionKeyToSoftwareLicense(BSTR softwareLicense, BSTR keyIdent)
{
	HRESULT hr(E_INVALIDARG);
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"SoftwareServer::ConvertProtectionKeyToSoftwareLicense() - softwareLicense: %s, keyIdent: %s", softwareLicense, keyIdent);
//OutputDebugString(tmpbuf);

	SafeMutex mutex(SoftwareLicenseLock);
	//bool bFoundMatchingLicense(false);
	//bool bFoundAnyLicense(softwareLicMgrMap.size > 0);

	//SoftwareLicenseMgr* pTmpSoftwareLicMgr = NULL;

	//if(_wcsicmp(softwareLicense, BSTR(L"") != 0)
	//{
	//	//Indexed by license filename, not be softwareLicense
	//	for(SoftwareLicList::iterator swLicMgrIt = softwareLicMgrMap.begin();
	//		swLicMgrIt != softwareLicMgrMap.end();
	//		swLicMgrIt++)
	//	{
	//		BSTR tmpBstrLicName;
	//		hr = swLicMgrIt->second->GetSoftwareLicenseName(&tmpBstrLicName);
	//		if(SUCCEEDED(hr))
	//		{
	//			//Find License File
	//			if(_wcsicmp(tmpBstrLicName, softwareLicense) == 0)
	//			{
	//				pTmpSoftwareLicMgr = swLicMgrIt->second;
	//				//bFoundMatchingLicense = true;
	//				break;
	//			}
	//		}
	//	}
	//}
	//

	////if matching license is not founs, and there are any licenses, use first one found.
	//if((pTmpSoftwareLicMgr == NULL) && (softwareLicMgrMap.size > 0))
	//{
	//	pTmpSoftwareLicMgr = *(softwareLicMgrMap.begin());
	//}
	//else	//none found, use a new one...
	//{
	//	pTmpSoftwareLicMgr = new SoftwareLicenseMgr();
	//}

	try
	{
		ProtectionKey* pTmpProKey = NULL;
		KeySpec keyspec;
		{
			SafeMutex mutex1(pRainbowDriver->keys_lock);
			pRainbowDriver->RefreshKeyList();
			
			for (RainbowDriver::KeyList::iterator dkey = pRainbowDriver->keys.begin(); dkey!=pRainbowDriver->keys.end(); ++dkey)
			{
				if(_wcsicmp(dkey->first, keyIdent) == 0)
				{
					_bstr_t keyName = dkey->first;
					pTmpProKey = new ProtectionKey(dkey->first, dkey->first, &keyspec,pRainbowDriver, true/*Use Share Licensing*/);
					break;
				}
			}
		}
		if(pTmpProKey == NULL) //Failed to find key, throw exception...
			throw E_INVALIDARG;

		Lic_PackageAttribs::Lic_LicenseInfoAttribs tmpExistingLicInfoAttribs;
		bool bFoundExistingLicense = false;

		if(_wcsicmp(L"", softwareLicense) == 0)	//if the string is empty, grab the first 
		{
			if(softwareLicMgrMap.size() != 0)
			{
				softwareLicMgrMap.begin()->second->GetLicenseInfo(&tmpExistingLicInfoAttribs);
				bFoundExistingLicense = true;
			}
		}
		else
		{
			hr = GetSoftwareLicenseInfo_ByLicenseInternal(softwareLicense, &tmpExistingLicInfoAttribs);
			if(SUCCEEDED(hr))
			{
				//will only return SUCCESS if softwareLicense exists...
				bFoundExistingLicense = true;
			}
		}


		BSTR bstrLicPackageStream;
		//hr = ConvertProtectionKeyToLicPackageAttribsStreamInternal(softwareLicense, pTmpProKey, &bstrLicPackageStream);
		//if(FAILED(hr))
		//	throw hr;
		Lic_PackageAttribs::Lic_LicenseInfoAttribs tmpNewLicInfoAttribs;
		hr = ConvertProtectionKeyToLicInfoAttribsInternal(pTmpProKey, softwareLicense, &tmpNewLicInfoAttribs);
		if(FAILED(hr))
			throw hr;

		Lic_PackageAttribs tmpLicPackage;
		if(bFoundExistingLicense)
		{
			hr = MergeLicInfoAttribsInternal(&tmpNewLicInfoAttribs, &tmpExistingLicInfoAttribs);
			if(FAILED(hr))
				throw hr;
			tmpLicPackage.licLicenseInfoAttribs = tmpExistingLicInfoAttribs;
		}
		else
		{
			tmpLicPackage.licLicenseInfoAttribs = tmpNewLicInfoAttribs;
			int customerNumber;
			int keyNumber;
			hr = StringToCustomerAndKeyNumberInternal(std::wstring(softwareLicense), &customerNumber, &keyNumber);
			if(SUCCEEDED(hr))
			{
				tmpLicPackage.licLicenseInfoAttribs.customerID = customerNumber;
				tmpLicPackage.licLicenseInfoAttribs.softwareGroupLicenseID = keyNumber;
			}
		}
		bstrLicPackageStream = SysAllocString(tmpLicPackage.ToString().c_str());

		//Convert protection key to a Lic_PackageAttribs (basically a Lic_LicenseInfoAttribs)
		//Call to add protection key

		wchar_t wVerCodeBuf[256];
		wsprintf(wVerCodeBuf, L"Convert Protection Key %s - %d", keyIdent, m_uniqueValue++);	//Add so can add the same key multiple times
		

		//_bstr_t bstrVerificationCode = _bstr_t(L"");
		_bstr_t bstrVerificationCode = _bstr_t(wVerCodeBuf);
		hr = ApplyLicensePacketInternal(bstrLicPackageStream, bstrVerificationCode);
		SysFreeString(bstrLicPackageStream);
		if(FAILED(hr))
			throw hr;

	}
	catch(HRESULT eHr)
	{
		hr = eHr;
	}
	//pRainbowDriver->
	//licInfo->
	

	//bFoundAnyLicense


	//if(SUCCEEDED(hr) && !bFoundMatchingLicense)
	//	hr = E_INVALIDARG;
	return hr;
}


HRESULT SoftwareServer::ConvertProtectionKeyToLicInfoAttribsInternal(ProtectionKey* pKey, _bstr_t bstrValidationKey, Lic_PackageAttribs::Lic_LicenseInfoAttribs *pLicenseInfoAttribs)
{
	HRESULT hr(E_INVALIDARG);

	try
	{
		if(pKey == NULL)
			throw E_INVALIDARG;

		KeySpec keyspec;
		Lic_PackageAttribs::Lic_LicenseInfoAttribs *pTmpLicenseInfoAttribs = new Lic_PackageAttribs::Lic_LicenseInfoAttribs();

		//If the key is a base key on trial, or an add-on key, have the module be add-on modules that expire at the expiration date of the key

		//If the key is a testDev key, change the Product to the Test/Dev version of the product

		//If the key is a backup key, maybe don't convert, think about this case some more

		//If the key is a base key that has become permanent, give it a permanent module

		//Get Key Type
		VARIANT vtKeyType;
		hr = pKey->HeaderQuery(keyspec.headers[L"Key Type"].id, &vtKeyType);
		if (FAILED(hr)) 
			throw hr;
		unsigned short key_type = vtKeyType.uiVal;
		VariantClear(&vtKeyType);
		//typedef enum 
		//{
		//		KEYNone			=0x00000000,  //reserved state
		//		KEYAddon		   =1,
		//		KEYBase			=2,
		//		KEYReplacement	=3,
		//		KEYRelicense	=4,
		//		KEYInventory	=5,
		//		KEYBackup		=6,
		//		KEYEmergency	=7,
		//		//KEYSolimar	=8,
		//		KEYCustom		=9,
		//		KEYLoan			=10,
		//		KEYDemo			=11,
		//		KEYRental		=12,
		//		KEYDevelopment	=13,
		//		KEYReserved		=14,
		//		KEYVerification=0xff,
		//} KeyTypeFlagX;
		bool bCalculateExpirationDate (false);
		Lic_PackageAttribs::Lic_LicenseInfoAttribs::TSoftwareLicenseType tLicType;
		if(key_type == 1)	//KEYAddon
		{
			bCalculateExpirationDate = true;
			tLicType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::sltStandard;
		}
		else if(key_type == 2)	//KEYBase
		{
			enum KeyStatuses
			{
				UNINITIALIZED_TRIAL=3, INITIAL_TRIAL=0, EXTENDED_TRIAL=1,
				EXTENDED_TRIAL2=4, EXTENDED_TRIAL3=5, EXTENDED_TRIAL4=6,
				EXTENDED_TRIAL5=7, BASE=2, UNUSED=10, DEACTIVATED=11, EXTENDED_TRIAL6=12,
				EXTENDED_TRIAL7=13, EXTENDED_TRIAL8=14, EXTENDED_TRIAL9=15, EXTENDED_TRIAL10=16,
				EXTENDED_TRIAL11=17, EXTENDED_TRIAL12=18, EXTENDED_TRIAL13=19, EXTENDED_TRIAL14=20,
				EXTENDED_TRIAL15=21, EXTENDED_TRIAL16=22
			};

			//Get Key Status
			VARIANT vtKeyStatus;
			VariantInit(&vtKeyStatus);
			hr = pKey->HeaderQuery(keyspec.headers[L"Status"].id, &vtKeyStatus);
			if (FAILED(hr)) 
				throw hr;
			KeyStatuses keyStatus = (KeyStatuses)vtKeyStatus.uiVal;
			VariantClear(&vtKeyStatus);
			bCalculateExpirationDate = (
				keyStatus == INITIAL_TRIAL ||
				keyStatus == EXTENDED_TRIAL ||
				keyStatus == EXTENDED_TRIAL2 ||
				keyStatus == EXTENDED_TRIAL3 ||
				keyStatus == EXTENDED_TRIAL4 ||
				keyStatus == EXTENDED_TRIAL5 ||
				keyStatus == EXTENDED_TRIAL6 ||
				keyStatus == EXTENDED_TRIAL7 ||
				keyStatus == EXTENDED_TRIAL8 ||
				keyStatus == EXTENDED_TRIAL9 ||
				keyStatus == EXTENDED_TRIAL10 ||
				keyStatus == EXTENDED_TRIAL11 ||
				keyStatus == EXTENDED_TRIAL12 ||
				keyStatus == EXTENDED_TRIAL13 ||
				keyStatus == EXTENDED_TRIAL14 ||
				keyStatus == EXTENDED_TRIAL15 ||
				keyStatus == EXTENDED_TRIAL16 ||
				keyStatus == UNINITIALIZED_TRIAL
			);


			tLicType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::sltStandard;
			//Determine if key is permanent or not
//hr = pServerInfo->LicenseServer->KeyHeaderQuery(key_ident, m_keyspec.headers[L"Status"].id, &vtKeyStatus);

//
//KeyIsTrial = 

			//if key is not permanent
			//bCalculateExpirationDate = true;
			//else if key is permanent
			//bCalculateExpirationDate = false;
		}
		else if(key_type == 13)	//KEYDevelopment
		{
			bCalculateExpirationDate = true;
			tLicType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::sltTestDev;
		}
		else if(key_type == 6)	//KEYBackup
		{
			tLicType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::sltFailover;
		}
		else	//All other
		{
			//Don't convert these unknown key type, throw error
			throw E_INVALIDARG;
		}
		pTmpLicenseInfoAttribs->softwareLicType = tLicType;

		SYSTEMTIME expTimeSystime;
		if(bCalculateExpirationDate)
		{
			VARIANT vtRawExpirationDate;
			VariantInit(&vtRawExpirationDate);
			hr = pKey->HeaderQuery(keyspec.headers[L"Expiration Date"].id, &vtRawExpirationDate);
			time_t expirationDateTimeT = (time_t)vtRawExpirationDate.ulVal;
			
			VARIANT vtExpirationDate;
			vtExpirationDate = TimeHelper::TimeTToVariant(expirationDateTimeT);

			VariantTimeToSystemTime(vtExpirationDate.date, &expTimeSystime);
			if (FAILED(hr)) 
				throw hr;

			VariantClear(&vtExpirationDate);
			VariantClear(&vtRawExpirationDate);
		}


		//Get Key Number
		VARIANT vtKeyNum;
		VariantInit(&vtKeyNum);
		hr = pKey->HeaderQuery(keyspec.headers[L"Key Number"].id, &vtKeyNum);
		if (FAILED(hr)) 
			throw hr;
		pTmpLicenseInfoAttribs->softwareGroupLicenseID = vtKeyNum.uiVal;
		VariantClear(&vtKeyNum);

		//Get Customer Number
		VARIANT vtCustNum;
		VariantInit(&vtCustNum);
		hr = pKey->HeaderQuery(keyspec.headers[L"Customer Number"].id, &vtCustNum);
		if (FAILED(hr)) 
			throw hr;
		pTmpLicenseInfoAttribs->customerID = vtCustNum.uiVal;
		VariantClear(&vtCustNum);


		//Get the Product Number
		VARIANT vtProductID;
		VariantInit(&vtCustNum);
		hr = pKey->HeaderQuery(keyspec.headers[L"Product ID"].id, &vtProductID);
		if (FAILED(hr)) 
			throw hr;
		int origProductID = vtProductID.uiVal;
		VariantClear(&vtProductID);
		
		//Some Products from protection keys get changed to be multiple products
		std::map<int, bool> newProductList; //map<ProductID(int), UseSharedLicensing(bool)
		switch(origProductID)
		{
			case Lic_PackageAttribs::pid_Spde:
				newProductList.insert(std::map<int, bool>::value_type(Lic_PackageAttribs::pid_Spde, false));
				newProductList.insert(std::map<int, bool>::value_type(Lic_PackageAttribs::pid_SpdeQueueManager, true));
				break;
			case Lic_PackageAttribs::pid_Rubika:
				newProductList.insert(std::map<int, bool>::value_type(Lic_PackageAttribs::pid_Rubika, false));
				newProductList.insert(std::map<int, bool>::value_type(Lic_PackageAttribs::pid_RubikaProcessBuilder, true));
				break;
			case Lic_PackageAttribs::pid_SolIndexer:
				newProductList.insert(std::map<int, bool>::value_type(Lic_PackageAttribs::pid_SolIndexer, false));
				newProductList.insert(std::map<int, bool>::value_type(Lic_PackageAttribs::pid_SdxDesigner, true));
				break;
			default:
				newProductList.insert(std::map<int, bool>::value_type(origProductID, false));
				break;
		}

		for(	std::map<int, bool>::iterator newProductListIt = newProductList.begin();
				newProductListIt != newProductList.end();
				newProductListIt++)
		{
			int productID = newProductListIt->first;
			if(key_type == 13)	//KEYDevelopment
			{
				//Recalculate ProductID for test/dev to new productIDs
				switch(newProductListIt->first)
				{
					case Lic_PackageAttribs::pid_Spde:
						productID = Lic_PackageAttribs::pid_TestDevSpde;
						break;
					case Lic_PackageAttribs::pid_Iconvert:
						productID = Lic_PackageAttribs::pid_TestDevIconvert;
						break;
					case Lic_PackageAttribs::pid_SolsearcherEnt:
						productID = Lic_PackageAttribs::pid_TestDevSolsearcherEp;
						break;
					case Lic_PackageAttribs::pid_SolsearcherSp:
						productID = Lic_PackageAttribs::pid_TestDevSseSp;
						break;
					case Lic_PackageAttribs::pid_Rubika:
						productID = Lic_PackageAttribs::pid_TestDevRubika;
						break;
					case Lic_PackageAttribs::pid_SolScript:
						productID = Lic_PackageAttribs::pid_TestDevSolScript;
						break;
					case Lic_PackageAttribs::pid_SolIndexer:
						productID = Lic_PackageAttribs::pid_TestDevSolIndexer;
						break;
					case Lic_PackageAttribs::pid_SolFusion:
						productID = Lic_PackageAttribs::pid_TestDevSolfusionSp;
						break;
					case Lic_PackageAttribs::pid_SpdeQueueManager:
						productID = Lic_PackageAttribs::pid_TestDevSpdeQueueManager;
						break;
					case Lic_PackageAttribs::pid_RubikaProcessBuilder:
						productID = Lic_PackageAttribs::pid_TestDevRubikaProcessBuilder;
						break;
					case Lic_PackageAttribs::pid_SdxDesigner:
						productID = Lic_PackageAttribs::pid_TestDevSdxDesigner;
						break;
					default:
						productID = newProductListIt->first;
						break;
				}
			}
			Lic_PackageAttribs::Lic_ProductInfoAttribs tmpProdAttribs;
			tmpProdAttribs.productID = productID;
			
			

			//Get the Version
			VARIANT vtProductVersion;
			VariantInit(&vtProductVersion);
			hr = pKey->HeaderQuery(keyspec.headers[L"Product Version"].id, &vtProductVersion);
			if (FAILED(hr)) 
				throw hr;
			tmpProdAttribs.product_Major = vtProductVersion.uintVal>>12;
			wchar_t tmpBuf[32];
			_snwprintf_s(tmpBuf, sizeof(tmpBuf)/sizeof(wchar_t), L"%x", (vtProductVersion.uintVal & 0x0FFF) >> 4);
			tmpBuf[31]=0;
			tmpProdAttribs.product_Minor = _wtoi(tmpBuf);
			VariantClear(&vtProductVersion);

			//Get the Application Instance
			VARIANT vtApplicationInstance;
			VariantInit(&vtProductVersion);
			hr = pKey->HeaderQuery(keyspec.headers[L"Application Instances"].id, &vtApplicationInstance);
			if (FAILED(hr)) 
				throw hr;
			if(key_type == 1)	//Is an add-on key, don't increment product application instance
				tmpProdAttribs.productAppInstance = 0;
			else					//Use whatever application instance is on the key, 0&1 are both 1...
				tmpProdAttribs.productAppInstance = vtApplicationInstance.uiVal<=1 ? 1 : vtApplicationInstance.uiVal;
			VariantClear(&vtProductID);

			KeySpec::Product &keyProductSpec(keyspec.products[origProductID]);
			for (KeySpec::Product::data_list_t::iterator keyModuleSpecIt = keyProductSpec.data.begin(); keyModuleSpecIt != keyProductSpec.data.end(); keyModuleSpecIt++)
			{
				//newProductListIt->first	//Use SharedLicensing
				//Only add sharable Modules to sharable ProductIDs
				if(keyModuleSpecIt->isSharable == newProductListIt->second)
				{
					VARIANT vtModuleValue;
					VariantInit(&vtModuleValue);
					hr = pKey->ModuleQuery(keyModuleSpecIt->id, &vtModuleValue);
					if(FAILED(hr))
						continue;	//Skip and go to next module
					int moduleValue = vtModuleValue.uiVal;
					VariantClear(&vtModuleValue);
					if(moduleValue != 0)
					{
						Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs;
						tmpModAttribs.moduleID = keyModuleSpecIt->id;
						tmpModAttribs.moduleValue = moduleValue;
						tmpModAttribs.moduleAppInstance = vtApplicationInstance.uiVal<=1 ? 1 : vtApplicationInstance.uiVal;
						if(bCalculateExpirationDate == true)
						{
							wchar_t wcharExpTimeStamp[256];
							TimeHelper::SystemTimeToString(wcharExpTimeStamp, sizeof(wcharExpTimeStamp)/sizeof(wchar_t), expTimeSystime);
							tmpModAttribs.moduleExpirationDate = std::wstring(wcharExpTimeStamp);
						}
						tmpProdAttribs.moduleList->push_back(tmpModAttribs);
					}
				}
			}
			pTmpLicenseInfoAttribs->productList->push_back(tmpProdAttribs);
		}

		//Generate Validation Token based on [Customer Number]-[Key Number]
		Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_VerificationAttribs tmpVerAttribs;
		Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs tmpValTokenAttribs;
		tmpValTokenAttribs.tokenType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttHardwareKeyID;
		tmpValTokenAttribs.tokenValue = std::wstring(bstrValidationKey);
		pTmpLicenseInfoAttribs->licVerificationAttribs.validationTokenList->push_back(tmpValTokenAttribs);

		//Generate a place for the keyCode
		Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs tmpValTokenAttribs2;
		tmpValTokenAttribs2.tokenType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttLicenseCode;
		tmpValTokenAttribs2.tokenValue = std::wstring(L"");
		pTmpLicenseInfoAttribs->licVerificationAttribs.validationTokenList->push_back(tmpValTokenAttribs2);

		*pLicenseInfoAttribs = *pTmpLicenseInfoAttribs;
	}
	catch(HRESULT eHr)
	{
		hr = eHr;
	}
	return hr;
}
//
HRESULT SoftwareServer::MergeLicInfoAttribsInternal(Lic_PackageAttribs::Lic_LicenseInfoAttribs *pMergeFromLicenseInfoAttribs, Lic_PackageAttribs::Lic_LicenseInfoAttribs *pMergeToLicenseInfoAttribs)
{
	HRESULT hr(S_OK);
	Lic_PackageAttribs::Lic_LicenseInfoAttribs tmpMergeToLicenseInfoAttribs = *pMergeToLicenseInfoAttribs;
	for(	Lic_PackageAttribs::Lic_LicenseInfoAttribs::TVector_Lic_ProductInfoAttribsList::iterator prodMergeFromIt = pMergeFromLicenseInfoAttribs->productList->begin();
			prodMergeFromIt != pMergeFromLicenseInfoAttribs->productList->end();
			prodMergeFromIt++)
	{
		bool bMergedProduct = false;
		for(	Lic_PackageAttribs::Lic_LicenseInfoAttribs::TVector_Lic_ProductInfoAttribsList::iterator prodMergeToIt = tmpMergeToLicenseInfoAttribs.productList->begin();
				prodMergeToIt != tmpMergeToLicenseInfoAttribs.productList->end();
				prodMergeToIt++)
		{
			//Find matching product
			if((int)prodMergeFromIt->productID == (int)prodMergeToIt->productID)
			{
				bMergedProduct = true;

				//Update the Application Instance
				prodMergeToIt->productAppInstance = (int)prodMergeToIt->productAppInstance + (int)prodMergeFromIt->productAppInstance;

				//Set product version to the largest version.
				if(((int)prodMergeFromIt->product_Major > (int)prodMergeToIt->product_Major) ||
					(((int)prodMergeFromIt->product_Major == (int)prodMergeToIt->product_Major) && ((int)prodMergeFromIt->product_Minor > (int)prodMergeToIt->product_Minor)))
				{
					prodMergeToIt->product_Major = (int)prodMergeFromIt->product_Major;
					prodMergeToIt->product_Minor = (int)prodMergeFromIt->product_Minor;
					prodMergeToIt->product_SubMajor = (int)prodMergeFromIt->product_SubMajor;
					prodMergeToIt->product_SubMinor = (int)prodMergeFromIt->product_SubMinor;
				}

				//Merge module
				for(	Lic_PackageAttribs::Lic_ProductInfoAttribs::TVector_Lic_ModuleInfoAttribsList::iterator modMergeFromIt = prodMergeFromIt->moduleList->begin();
						modMergeFromIt != prodMergeFromIt->moduleList->end();
						modMergeFromIt++)
				{
					bool bUpdatedModule = false;

					//Only try to merge if the from module is not an add-on
					if(_wcsicmp(modMergeFromIt->moduleExpirationDate->c_str(), L"1900-01-01 00:00:00.0000") == 0)
					{
						for(	Lic_PackageAttribs::Lic_ProductInfoAttribs::TVector_Lic_ModuleInfoAttribsList::iterator modMergeToIt = prodMergeToIt->moduleList->begin();
								modMergeToIt != prodMergeToIt->moduleList->end();
								modMergeToIt++)
						{
							//Find matching module and both are permanent modules...
							if(((int)modMergeFromIt->moduleID == (int)modMergeToIt->moduleID) && 
								(_wcsicmp(modMergeToIt->moduleExpirationDate->c_str(), L"1900-01-01 00:00:00.0000") == 0))
							{
								//TODO: check and make sure expiration day is not set
								modMergeToIt->moduleAppInstance = (int)modMergeToIt->moduleAppInstance + (int)modMergeFromIt->moduleAppInstance;
								modMergeToIt->moduleValue = (int)modMergeToIt->moduleValue + (int)modMergeFromIt->moduleValue;
								bUpdatedModule = true;
								break;
							}
						}
					}
					if(bUpdatedModule == false)	//Didn't find module to update, add it...
					{
						prodMergeToIt->moduleList->push_back(*modMergeFromIt);
					}
				}
			}
		}

		if(bMergedProduct == false)
		{
			tmpMergeToLicenseInfoAttribs.productList->push_back(*prodMergeFromIt);
		}
	}
	*pMergeToLicenseInfoAttribs = tmpMergeToLicenseInfoAttribs;

	return hr;
}

//Convert 100-055 to 2 ints each having 100 and 55 as its value
HRESULT SoftwareServer::StringToCustomerAndKeyNumberInternal(std::wstring _inputString, int* _pCustomerNumber, int* _pKeyNumber)
{
	//long output[2]={0,0};
	wchar_t *pToken;
	wchar_t *pNextToken;
	size_t sizeOfBuffer = _inputString.length()+1;
	wchar_t *pBuffer=new wchar_t[sizeOfBuffer];
	
	
	// copy the string so we can modify the copy using strtok
	wcscpy_s(pBuffer, sizeOfBuffer, _inputString.c_str());
	
	// parse the string
	unsigned int uiCount=0;

	//pToken=wcstok_s(pBuffer, L"-", &pNextToken);
	//while (pToken!=NULL && uiCount<2)
	//{
	//	output[uiCount]= _wtol(pToken);
	//	++uiCount;
	//	pToken=wcstok_s(NULL,L"-", &pNextToken);
	//}
	//*_pCustomerNumber = output[0];
	//*_pKeyNumber = output[1];

	unsigned int tmpCN(0), tmpKN(0);
	pToken=wcstok_s(pBuffer, L"-", &pNextToken);
	swscanf_s(pToken, L"%hx", &tmpCN);
	pToken=wcstok_s(NULL, L"-", &pNextToken);
	swscanf_s(pToken, L"%hx", &tmpKN);

	*_pCustomerNumber = tmpCN;
	*_pKeyNumber = tmpKN;
	delete [] pBuffer;

	return S_OK;
}

//Populate productReminderMap from a given Lic_PackageAttribs::Lic_LicenseInfoAttribs
HRESULT SoftwareServer::PopulateProductReminderMap(Lic_PackageAttribs::Lic_LicenseInfoAttribs* pLicInfoAttribs, ProductReminderMap* pProdReminderMap)
{
	SafeMutex mutex(SoftwareLicenseLock);
	HRESULT hr(S_OK);
	try
	{
		(*pProdReminderMap).clear();
		if(pLicInfoAttribs != NULL)
		{
			//Convert SoftwareLicense ExpirationDate to time_t
			SYSTEMTIME swLicExpiresDateSystime;
			if(!TimeHelper::StringToSystemTime(std::wstring(SpdAttribs::WStringObj(pLicInfoAttribs->activationExpirationDate)).c_str(), swLicExpiresDateSystime))
				throw E_FAIL;
			_variant_t vtSwLicExpiresDate(NULL);	
			if (!SystemTimeToVariantTime(&swLicExpiresDateSystime, &vtSwLicExpiresDate.date)) 
				throw E_FAIL;
			time_t swLicExpiresDateTimeT = TimeHelper::VariantToTimeT(vtSwLicExpiresDate, false);

			//Cycle through all Products
			for(	Lic_PackageAttribs::Lic_LicenseInfoAttribs::TVector_Lic_ProductInfoAttribsList::iterator prodIt = pLicInfoAttribs->productList->begin();
					prodIt != pLicInfoAttribs->productList->end();
					prodIt++)
			{
				//Set the lowest expiration date...
				if(swLicExpiresDateTimeT != time_t(-1) && ((*pProdReminderMap)[(*prodIt).productID].closestExpDate == -1 || swLicExpiresDateTimeT < (*pProdReminderMap)[(*prodIt).productID].closestExpDate))
					(*pProdReminderMap)[(*prodIt).productID].closestExpDate = swLicExpiresDateTimeT;

				if(swLicExpiresDateTimeT != time_t(-1))
					(*pProdReminderMap)[(*prodIt).productID].swExpDate = swLicExpiresDateTimeT;

				ModuleIdToExpirationDateList tmpModToExpDateList = (*pProdReminderMap)[(*prodIt).productID].modIdToExpDateList;
				for(	Lic_PackageAttribs::Lic_ProductInfoAttribs::TVector_Lic_ModuleInfoAttribsList::iterator modIt = prodIt->moduleList->begin();
						modIt != prodIt->moduleList->end();
						modIt++)
				{
					SYSTEMTIME moduleExpiresDateSystime;
					if(!TimeHelper::StringToSystemTime(std::wstring(SpdAttribs::WStringObj(modIt->moduleExpirationDate)).c_str(), moduleExpiresDateSystime))
						continue;
					
					_variant_t vtModuleExpiresDate(NULL);	
					if (!SystemTimeToVariantTime(&moduleExpiresDateSystime, &vtModuleExpiresDate.date)) 
						continue;
					time_t moduleExpiresDateTimeT = TimeHelper::VariantToTimeT(vtModuleExpiresDate, false);
					time_t emptyExpiresDateTimeT = time_t(-1);

					// ignore expiration date if it equals 1900/1/1
					if(moduleExpiresDateTimeT != emptyExpiresDateTimeT)
					{
						tmpModToExpDateList.push_back(ModuleIdToExpirationDate(modIt->moduleID, moduleExpiresDateTimeT));

						//Set the lowest expiration date...
						if((*pProdReminderMap)[(*prodIt).productID].closestExpDate == -1 || moduleExpiresDateTimeT < (*pProdReminderMap)[(*prodIt).productID].closestExpDate)
							(*pProdReminderMap)[(*prodIt).productID].closestExpDate = moduleExpiresDateTimeT;
					}
				}
				(*pProdReminderMap)[(*prodIt).productID].modIdToExpDateList = tmpModToExpDateList;
			}
		}
	}
	catch (HRESULT &ehr)
	{
		hr = ehr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	return hr;
}

//Generates a Warning Message for all expiring modules.
HRESULT SoftwareServer::TimesUp()
{
	SafeMutex mutex(SoftwareLicenseLock);
	HRESULT hr(S_OK);
	try
	{
		static const time_t ONE_HOUR = (time_t)(60*60);
		static const time_t ONE_DAY = (time_t)(24*ONE_HOUR);
		time_t curTime = time(NULL);

		//Cycle through all SoftwareLicenseManagers
		for(	SoftwareLicList::iterator swLicMgrIt = softwareLicMgrMap.begin();
				swLicMgrIt != softwareLicMgrMap.end();
				swLicMgrIt++)
		{
			Lic_PackageAttribs::Lic_LicenseInfoAttribs tmpLicInfoAttribs;
			hr = swLicMgrIt->second->GetLicenseInfo(&tmpLicInfoAttribs);
			if(FAILED(hr))
				continue;
			
			std::wstring wstrSoftwareLicenseName;
			BSTR bstrSoftwareLicenseName;
			hr = swLicMgrIt->second->GetSoftwareLicenseName(&bstrSoftwareLicenseName);
			if(FAILED(hr))
				continue;
			wstrSoftwareLicenseName = (wchar_t*)bstrSoftwareLicenseName;
			SysFreeString(bstrSoftwareLicenseName);

			//See if need to refresh view maps..
			if(swLicReminderMap[wstrSoftwareLicenseName].softwareLicReminderClassLastRefresh == -1)
			{
				hr = PopulateProductReminderMap(&tmpLicInfoAttribs, &swLicReminderMap[wstrSoftwareLicenseName].prodReminderMap);
				if(FAILED(hr))
					continue;
				swLicReminderMap[wstrSoftwareLicenseName].softwareLicReminderClassLastRefresh = curTime;
			}

			const int TMP_BUFF_SIZE = 1024;
			for(	ProductReminderMap::iterator prodReminderMapIt = swLicReminderMap[wstrSoftwareLicenseName].prodReminderMap.begin();
					prodReminderMapIt != swLicReminderMap[wstrSoftwareLicenseName].prodReminderMap.end();
					prodReminderMapIt++)
			{
				//Check on the closest expiration date, 
				//14 days to expiration, send reminder every 12 hours
				//7 days to expiration, send reminder every 4 hours
				//3 days to expiration, send reminder every 1 hours

				time_t sendReminderTimePeriod = 0;
				if(prodReminderMapIt->second.closestExpDate == -1)
					sendReminderTimePeriod = ONE_HOUR;
				else if(prodReminderMapIt->second.closestExpDate < curTime)
					sendReminderTimePeriod = ONE_HOUR;
				else	//if(prodReminderMapIt->second.closestExpDate > curTime)
				{
					double timeTillExpiration = difftime(prodReminderMapIt->second.closestExpDate, curTime);
					if(timeTillExpiration > ONE_DAY * 14)
						sendReminderTimePeriod = 0;
					else if(ONE_DAY * 14 >= timeTillExpiration && timeTillExpiration >ONE_DAY * 7)
						sendReminderTimePeriod = ONE_HOUR * 12;
					else if(ONE_DAY * 7 >= timeTillExpiration && timeTillExpiration >ONE_DAY * 3)
						sendReminderTimePeriod = ONE_HOUR * 4;
					else if(ONE_DAY * 3 >= timeTillExpiration && timeTillExpiration > 0)
						sendReminderTimePeriod = ONE_HOUR;
				}

				bool bSendReminder = false;
				bSendReminder = prodReminderMapIt->second.lastReminderSent == -1;
				if(!bSendReminder && sendReminderTimePeriod!=0)
				{
					//difftime returns the elapsed time in seconds, from timer0 to timer1 - double difftime(time_t timer1, time_t timer0 );
					double timeSinceLastReminder = difftime(curTime, prodReminderMapIt->second.lastReminderSent);
					bSendReminder = timeSinceLastReminder >= sendReminderTimePeriod;
				}

				if(bSendReminder)
				{
					//Generate Message to send
					static const int MAX_MESSAGE_SIZE = 0x2000;
					static const int MAX_MESSAGE_SIZE_TO_CUT = 0x1700;
					wchar_t wMsg[MAX_MESSAGE_SIZE];
					wMsg[0] = 0;
					size_t stCurrentBufferSize(0);
					size_t stAdditionBufferSize(0);
					bool bShowMessage(true);
					int tmpProductID = prodReminderMapIt->first;
					for(	ModuleIdToExpirationDateList::iterator modIt = prodReminderMapIt->second.modIdToExpDateList.begin();
							modIt != prodReminderMapIt->second.modIdToExpDateList.end();
							modIt++)
					{
						_variant_t vtTimestamp;
						vtTimestamp = TimeHelper::TimeTToVariant(modIt->second, true);
						SYSTEMTIME timeSystime;
						VariantTimeToSystemTime(vtTimestamp.date, &timeSystime);
						wchar_t wTimeStamp[256];
						TimeHelper::SystemTimeToString(wTimeStamp, _countof(wTimeStamp), timeSystime, false);
						time_t currentTimeDateTimeT = time(NULL);	//Retrieves Universal Time
						
						if(bShowMessage)
						{
							wchar_t wTmpBuf[256];
							wTmpBuf[0] = 0;
							if(currentTimeDateTimeT < modIt->second)	//Module will expire soon
								stAdditionBufferSize = _snwprintf_s(wTmpBuf, _countof(wTmpBuf), L"  Module: %s (%0d) - Will Expire at: %s\n", Lic_PackageAttribsHelper::GetModuleName(&(g_pSoftwareSpec->GetSoftwareSpec()), prodReminderMapIt->first, modIt->first).c_str(), modIt->first, wTimeStamp);
							else	//Module has expired already
								stAdditionBufferSize = _snwprintf_s(wTmpBuf, _countof(wTmpBuf), L"  Module: %s (%0d) - Expired On: %s\n", Lic_PackageAttribsHelper::GetModuleName(&(g_pSoftwareSpec->GetSoftwareSpec()), prodReminderMapIt->first, modIt->first).c_str(), modIt->first, wTimeStamp);
							if(stCurrentBufferSize + stAdditionBufferSize > MAX_MESSAGE_SIZE_TO_CUT)
							{
								_snwprintf_s(wTmpBuf, _countof(wTmpBuf), L"  Use License Viewer for the rest of the modules that are Expired or close to Expiring");
								bShowMessage = false;
							}
							stCurrentBufferSize += stAdditionBufferSize;
							wcscat_s(wMsg, wTmpBuf);
						}
					}

					//Set Message for exp date
					if(prodReminderMapIt->second.swExpDate != -1)
					{
						_variant_t vtTimestamp;
						vtTimestamp = TimeHelper::TimeTToVariant(prodReminderMapIt->second.swExpDate, true);
						SYSTEMTIME timeSystime;
						VariantTimeToSystemTime(vtTimestamp.date, &timeSystime);
						wchar_t wTimeStamp[256];
						TimeHelper::SystemTimeToString(wTimeStamp, _countof(wTimeStamp), timeSystime, false);

						time_t currentTimeDateTimeT = time(NULL);	//Retrieves Universal Time
						wchar_t wTmpBuf[256];
						BSTR bstrTmpSoftwareLicense;
						swLicMgrIt->second->GetSoftwareLicenseName(&bstrTmpSoftwareLicense);
						if(currentTimeDateTimeT < prodReminderMapIt->second.swExpDate)	//Software License is valid
							_snwprintf_s(wTmpBuf, _countof(wTmpBuf), L"\nLicense: %s is valid until: %s\n", bstrTmpSoftwareLicense, wTimeStamp);
						else	//Software License is invalid
							_snwprintf_s(wTmpBuf, _countof(wTmpBuf), L"\nLicense: %s has been invalid since: %s\n", bstrTmpSoftwareLicense, wTimeStamp);
						SysFreeString(bstrTmpSoftwareLicense);
						wcscat_s(wMsg, wTmpBuf);
					}
					if(prodReminderMapIt->second.modIdToExpDateList.size() > 0)
					{
						BSTR bstrTmpSoftwareLicense;
						swLicMgrIt->second->GetSoftwareLicenseName(&bstrTmpSoftwareLicense);
						g_licenseController.GenerateSoftwareLicenseMessage(bstrTmpSoftwareLicense, prodReminderMapIt->first, MT_WARNING, S_OK, time(0), MessageSoftwareModuleExpiration , Lic_PackageAttribsHelper::GetProductName(&(g_pSoftwareSpec->GetSoftwareSpec()), prodReminderMapIt->first).c_str(), prodReminderMapIt->first, wMsg);
						SysFreeString(bstrTmpSoftwareLicense);
					}
					prodReminderMapIt->second.lastReminderSent = curTime;
				}
			}
		}
	}
	catch (HRESULT &ehr)
	{
		hr = ehr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	return hr;
}