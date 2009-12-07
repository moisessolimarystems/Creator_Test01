#ifndef _WIN32_DCOM
#define _WIN32_DCOM // For CoInitializeEx
#endif

#include "LicensingServerWrapper.h"

#include "..\common\InProcPtr.h"
#include "..\common\ChallengeResponseHelper.h"
#include "..\common\_IObjectAuthentication.h"
#include "..\common\SafeMutex.h"
#include "..\manager_wrapper\LicensingWrapper.h"	//LOG_ERROR_HR


#include <assert.h>		// For ASSERT


BYTE SolimarLicenseManagerWrapper::LicensingServerWrapper::challenge_key_manager_userauththis_public[] = {
#include "..\common\keys\SolimarLicenseManager.UserAuthThis.public.key.txt"
};
BYTE SolimarLicenseManagerWrapper::LicensingServerWrapper::challenge_key_manager_thisauthuser_private[] = {
#include "..\common\keys\SolimarLicenseManager.ThisAuthUser.private.key.txt"
};
BYTE SolimarLicenseManagerWrapper::LicensingServerWrapper::challenge_key_server_thisauthuser_private[] = {
#include "..\common\keys\SolimarLicenseServer.ThisAuthUser.private.key.txt"
};
BYTE SolimarLicenseManagerWrapper::LicensingServerWrapper::challenge_key_server_userauththis_public[] = {
#include "..\common\keys\SolimarLicenseServer.UserAuthThis.public.key.txt"
};


SolimarLicenseManagerWrapper::LicensingServerWrapper::LicensingServerWrapper() : 
	bConnected(false),
	ChallengeResponseHelper(challenge_key_manager_thisauthuser_private, sizeof(challenge_key_manager_thisauthuser_private)/sizeof(BYTE), challenge_key_manager_userauththis_public, sizeof(challenge_key_manager_userauththis_public)/sizeof(BYTE))
//	licenseServer(NULL)
{
	return;
}

SolimarLicenseManagerWrapper::LicensingServerWrapper::~LicensingServerWrapper()
{
	//Close any previous connections
	Disconnect();	
//	if(licenseServer != NULL)
//		licenseServer.Release();
}

HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::Connect(std::wstring server)
{
	// Try to create an ISolimarLicenseServer proxy to the server
	COSERVERINFO	serverInfo	= {0, BSTR(server.c_str()), NULL, 0};
	MULTI_QI		multiQI		= {&__uuidof(ISolimarLicenseSvr4), NULL, NOERROR};
	
	//Close any previous connections
	Disconnect();

	HRESULT hr = CoCreateInstanceEx(
		__uuidof(CSolimarLicenseSvr), 
		NULL, 
		CLSCTX_REMOTE_SERVER | CLSCTX_LOCAL_SERVER, 
		&serverInfo, 
		1, 
		&multiQI);
	if (SUCCEEDED(hr))
	{
		ISolimarLicenseSvr4 *pILicenseServer = (ISolimarLicenseSvr4*)multiQI.pItf;
		ChallengeResponseHelper CR(	challenge_key_server_thisauthuser_private, 
									sizeof(challenge_key_server_thisauthuser_private)/sizeof(BYTE), 
									challenge_key_server_userauththis_public, 
									sizeof(challenge_key_server_userauththis_public)/sizeof(BYTE));
		// try to authenticate the license server
		hr = CR.AuthenticateServer(pILicenseServer);
		if (SUCCEEDED(hr))
		{
			// let the license server authenticate this manager
			hr = CR.AuthenticateToServer(pILicenseServer);
			if (SUCCEEDED(hr))
			{
				hr = licenseServer.Attach(pILicenseServer);
				ISolimarSoftwareLicenseSvr *pILicenseSoftwareServer ;
				hr = pILicenseServer->QueryInterface(__uuidof(ISolimarSoftwareLicenseSvr), (void**)&pILicenseSoftwareServer);
				if(SUCCEEDED(hr))
				{
					hr = licenseSoftwareServer.Attach(pILicenseSoftwareServer);
					bConnected = true;
					licServerName = server;
				}
			}
		}
		
		if (FAILED(hr))
			pILicenseServer->Release();
	}
	
	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingServerWrapper::Connect()", hr);
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::Disconnect()
{
	HRESULT hr = S_OK;
	bConnected = false;
	licServerName = L"";
	LOG_ERROR_HR(L"SolimarLicenseManagerWrapper::LicensingServerWrapper::Disconnect()", hr);
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::SoftwareModuleLicenseTotalForAll_ByProduct(long productID, long moduleIdent, long* pLicenseCount)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, SoftwareModuleLicenseTotalForAll_ByProduct, (productID, moduleIdent, pLicenseCount), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::SoftwareModuleLicenseInUseForAll_ByProduct(long productID, long moduleIdent, long* pLicenseCount)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, SoftwareModuleLicenseInUseForAll_ByProduct, (productID, moduleIdent, pLicenseCount), hr);
	}
	return hr;
}

//Think can't accurately get the total
//HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::SoftwareModuleLicenseInUseByApp_ByProduct(BSTR bstrApplication, long productID, long moduleIdent, long* pLicenseCount)
//{
//	HRESULT hr(S_OK);
//	if(bConnected)
//	{
//		hr = licenseSoftwareServer->SoftwareAddApplicationInstanceByProduct(productID, bstrApplication);
//		if(SUCCEEDED(hr))
//		{
//			hr = licenseSoftwareServer->SoftwareModuleLicenseInUseByApp_ByProduct(productID, moduleIdent, pLicenseCount);
//			licenseSoftwareServer->SoftwareRemoveApplicationInstanceByProduct(productID, bstrApplication);	//ignore hr...
//		}
//	}
//	else
//		hr = LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED;
//
//
//	return hr;
//}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::GetSoftwareLicenseInfoByProduct_ForAll(long productID, BSTR *pBstrProductInfoAttribsStream)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, GetSoftwareLicenseInfoByProduct_ForAll, (productID, pBstrProductInfoAttribsStream), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::GetSoftwareLicenseInfo_ForAll(BSTR *pBstrLicenseInfoAttribsStream)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, GetSoftwareLicenseInfo_ForAll, (pBstrLicenseInfoAttribsStream), hr);
	}
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::GetAllSoftwareLicenses(BSTR *pBstrAllLicensesStream)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, GetAllSoftwareLicenses, (pBstrAllLicensesStream), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::GetSoftwareLicenseInfoByProduct_ByLicense(BSTR bstrSoftwareLicense, long productID, BSTR *pBstrProductInfoAttribsStream)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, GetSoftwareLicenseInfoByProduct_ByLicense, (bstrSoftwareLicense, productID, pBstrProductInfoAttribsStream), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::GetSoftwareLicenseInfo_ByLicense(BSTR bstrSoftwareLicense, BSTR *pBstrLicenseInfoAttribsStream)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, GetSoftwareLicenseInfo_ByLicense, (bstrSoftwareLicense, pBstrLicenseInfoAttribsStream), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::GetSoftwareLicenseStatus_ByProduct(long productID, BSTR *pBstrStringToDwordMap)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, GetSoftwareLicenseStatus_ByProduct, (productID, pBstrStringToDwordMap), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::GetSoftwareLicenseStatus_ByLicense(BSTR bstrSoftwareLicense)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, GetSoftwareLicenseStatus_ByLicense, (bstrSoftwareLicense), hr);
	}
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::ValidateToken_ByLicense(BSTR bstrSoftwareLicense, long validationTokenType, BSTR bstrValidationValue)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, ValidateToken_ByLicense, (bstrSoftwareLicense, validationTokenType, bstrValidationValue), hr);
	}
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::GetSoftwareSpecByProduct(long productID, BSTR *pBstrProductSoftwareSpecAttribs)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, GetSoftwareSpecByProduct, (productID, pBstrProductSoftwareSpecAttribs), hr);
	}
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::GetSoftwareSpec(BSTR *pBstrSoftwareSpecAttribsStream)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, GetSoftwareSpec, (pBstrSoftwareSpecAttribsStream), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::SoftwareGetApplicationInstanceListByProduct(long productID, BSTR *pBstrListAppInstStream)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, SoftwareGetApplicationInstanceListByProduct, (productID, pBstrListAppInstStream), hr);
	}
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::GenerateSoftwareLicPacket(BSTR bstrLicPackageAttribsStream, VARIANT vtExpires, BSTR *pBstrVerificationCode, VARIANT* pVtLicensePacket)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, GenerateSoftwareLicPacket, (bstrLicPackageAttribsStream, vtExpires, pBstrVerificationCode, pVtLicensePacket), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::EnterSoftwareLicPacket(VARIANT vtLicensePacket, BSTR *pBstrVerificationCode)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, EnterSoftwareLicPacket, (vtLicensePacket, pBstrVerificationCode), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::EnterProtectionKeyPassword(BSTR bstrPassword)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseServer, EnterPassword, (bstrPassword), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::EnterProtectionKeyPasswordPacket(VARIANT vtLicensePacket, BSTR *pBstrVerificationCode)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseServer, EnterPasswordPacket, (vtLicensePacket, pBstrVerificationCode), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::GenerateSoftwareLicArchive_ByLicense(BSTR softwareLicense, VARIANT* pVtLicenseArchive)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, GenerateSoftwareLicArchive_ByLicense, (softwareLicense, pVtLicenseArchive), hr);
	}
	return hr;

}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::EnterSoftwareLicArchive(VARIANT vtLicenseArchive)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, EnterSoftwareLicArchive, (vtLicenseArchive), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::GenerateVerifyDataWithVerCode_ByLicense(BSTR bstrSoftwareLicense, VARIANT* pVtLicensePacket)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, GenerateVerifyDataWithVerCode_ByLicense, (bstrSoftwareLicense, pVtLicensePacket), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::GenerateVerifyDataWithLicInfo_ByLicense(BSTR bstrSoftwareLicense, VARIANT* pVtLicensePacket)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, GenerateVerifyDataWithLicInfo_ByLicense, (bstrSoftwareLicense, pVtLicensePacket), hr);
	}
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::GenerateLicPackage_ByVerifyData(VARIANT vtVerifyData, BSTR *pBstrLicensePackageAttribsStream)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, GenerateLicPackage_ByVerifyData, (vtVerifyData, pBstrLicensePackageAttribsStream), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::GenerateLicPackage_BySoftwareLicArchive(VARIANT vtLicenseArchive, BSTR *pBstrLicensePackageAttribsStream)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, GenerateLicPackage_BySoftwareLicArchive, (vtLicenseArchive, pBstrLicensePackageAttribsStream), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::GenerateLicPackage_BySoftwareLicPacket(VARIANT vtLicensePacket, BSTR *pBstrLicensePackageAttribsStream)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, GenerateLicPackage_BySoftwareLicPacket, (vtLicensePacket, pBstrLicensePackageAttribsStream), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::GenerateLicenseSystemData(VARIANT* pVtLicSysDataPacket)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, GenerateLicenseSystemData, (pVtLicSysDataPacket), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::GenerateStream_ByLicenseSystemData(VARIANT vtLicSysDataPacket, BSTR *pBstrLicSysDataAttribsStream)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, GenerateStream_ByLicenseSystemData, (vtLicSysDataPacket, pBstrLicSysDataAttribsStream), hr);
	}
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber(BSTR bstrSoftwareLicense, BSTR bstrContractNumber)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseSoftwareServer, SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber, (bstrSoftwareLicense, bstrContractNumber), hr);
	}
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::KeyEnumerate(VARIANT *pVtKeyList)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseServer, KeyEnumerate, (pVtKeyList), hr);
	}
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::KeyHeaderQuery(BSTR bstrKey, long headerIdent, VARIANT *pVtValue)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseServer, KeyHeaderQuery, (bstrKey, headerIdent, pVtValue), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::KeyTrialHours(BSTR bstrKey, long *plTrialHours)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseServer, KeyTrialHours, (bstrKey, plTrialHours), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::KeyIsActive(BSTR bstrKey, VARIANT_BOOL *pVtKeyActive)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseServer, KeyIsActive, (bstrKey, pVtKeyActive), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::KeyIsProgrammed(BSTR bstrKey, VARIANT_BOOL *pVtKeyProgrammed)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseServer, KeyIsProgrammed, (bstrKey, pVtKeyProgrammed), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::KeyTrialExpires(BSTR bstrKey, VARIANT *pVtExpireDate)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseServer, KeyTrialExpires, (bstrKey, pVtExpireDate), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::KeyModuleEnumerate(BSTR bstrKey, VARIANT *pVtKeyModuleList)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseServer, KeyModuleEnumerate, (bstrKey, pVtKeyModuleList), hr);
	}
	return hr;
}

HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::KeyModuleLicenseTotal(BSTR bstrKey, long moduleIdent, long* pLicenseTotal)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseServer, KeyModuleLicenseTotal, (bstrKey, moduleIdent, pLicenseTotal), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::KeyModuleInUse(BSTR bstrKey, long moduleIdent, long* pLicenseInUse)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseServer, KeyModuleInUse, (bstrKey, moduleIdent, pLicenseInUse), hr);
	}
	return hr;
}


HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::KeyFormat(BSTR bstrKey, BSTR *pBstrNewKey)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseServer, KeyFormat, (bstrKey, pBstrNewKey), hr);
	}
	return hr;
}
HRESULT SolimarLicenseManagerWrapper::LicensingServerWrapper::KeyProgram2(BSTR bstrKey, long customerNumber, long keyNumber, long productIdent, long verMajor, long verMinor, long keyType, long application_instances, long days, VARIANT module_value_list, BSTR *pBstrNewKey)
{
	HRESULT hr(LicenseServerError::EHR_WRPPR_SVR_NOT_CONNECTED);
	if(bConnected)
	{
		LIC_SSLSERVER_FTCALL_HR(licenseServer, KeyProgram2, (bstrKey, customerNumber, keyNumber, productIdent, verMajor, verMinor, keyType, application_instances, days, module_value_list, pBstrNewKey), hr);
	}
	return hr;
}