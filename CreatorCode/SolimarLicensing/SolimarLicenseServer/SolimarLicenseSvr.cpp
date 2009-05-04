// SolimarLicenseSvr.cpp : Implementation of CSolimarLicenseSvr

#include "SolimarLicenseSvr.h"
#include "KeyServer.h"
#include "..\common\ChallengeResponse.h"
#include "..\common\VersionInfo.h"

// The macro SOLIMAR_INTERNAL_PROTECTED_FUNCTION will return from a function immediately for release builds.
// The purpose of this is to protect some licensing functions from being accessable to the customer.
#if !defined(_SOLIMAR_INTERNAL) && !defined(_DEBUG)
#define SOLIMAR_INTERNAL_PROTECTED_FUNCTION return HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED)
#else
#define SOLIMAR_INTERNAL_PROTECTED_FUNCTION 
#endif

// if the client hasn't authenticated, return an error
#define CHECK_CLIENT_AUTHENTICATION  if (!ChallengePassedByClient()) return HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED);
//#define CHECK_CLIENT_AUTHENTICATION {}

BYTE CSolimarLicenseSvr::challenge_key_server_thisauthuser_public[] = {
#include "..\common\keys\SolimarLicenseServer.ThisAuthUser.public.key.txt"
};

BYTE CSolimarLicenseSvr::challenge_key_server_userauththis_private[] = {
#include "..\common\keys\SolimarLicenseServer.UserAuthThis.private.key.txt"
};


//KeyServer CSolimarLicenseSvr::keyserver;

CSolimarLicenseSvr::CSolimarLicenseSvr() : 
	ChallengeResponseHelper(challenge_key_server_userauththis_private, sizeof(challenge_key_server_userauththis_private)/sizeof(BYTE), challenge_key_server_thisauthuser_public, sizeof(challenge_key_server_thisauthuser_public)/sizeof(BYTE))
{
//wchar_t debug_buf[1024];
//_snwprintf_s(debug_buf, _countof(debug_buf, L"CSolimarLicenseSvr::CSolimarLicenseSvr() - Enter, m_licenseId: %s", (wchar_t*)m_licenseId);
//OutputDebugStringW(debug_buf);
	//keyserver.ResynchronizeKeys();
	//keyserver.Initialize(&driver);
	g_licenseController.keyserver.Initialize(&g_licenseController.driver);
	g_licenseController.softwareServer.Initialize(&g_licenseController.driver);
	//g_licenseController.softwareServer.Initialize(&keyserver.driver);
//_snwprintf_s(debug_buf, _countof(debug_buf, L"CSolimarLicenseSvr::CSolimarLicenseSvr() - Leave, m_licenseId: %s", (wchar_t*)m_licenseId);
//OutputDebugStringW(debug_buf);
}

CSolimarLicenseSvr::~CSolimarLicenseSvr()
{
//wchar_t debug_buf[1024];
//_snwprintf_s(debug_buf, _countof(debug_buf, L"CSolimarLicenseSvr::~CSolimarLicenseSvr() - Enter, m_licenseId: %s", (wchar_t*)m_licenseId);
//OutputDebugStringW(debug_buf);

	g_licenseController.keyserver.LicenseReleaseAll(m_licenseId);
	g_licenseController.RemoveHeartbeat(m_licenseId);
	
//_snwprintf_s(debug_buf, _countof(debug_buf, L"CSolimarLicenseSvr::~CSolimarLicenseSvr() - Leave, m_licenseId: %s", (wchar_t*)m_licenseId);
//OutputDebugStringW(debug_buf);
}


STDMETHODIMP CSolimarLicenseSvr::AddApplicationInstance(BSTR key_ident, BSTR application_instance, VARIANT_BOOL b_app_instance_lock_key)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.AddApplicationInstance(m_licenseId, key_ident, application_instance, b_app_instance_lock_key);
}
STDMETHODIMP CSolimarLicenseSvr::RemoveApplicationInstance(BSTR key_ident, BSTR application_instance)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.RemoveApplicationInstance(m_licenseId, key_ident, application_instance);
}
STDMETHODIMP CSolimarLicenseSvr::GetApplicationInstanceList(BSTR key_ident, VARIANT *pvtAppInstanceList)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.GetApplicationInstanceList(m_licenseId, key_ident, pvtAppInstanceList);
}



STDMETHODIMP CSolimarLicenseSvr::GetVersionLicenseServer(long* p_ver_major, long* p_ver_minor, long* p_ver_build)
{
	*p_ver_major = MAJOR_REVISION_NUMBER;
	*p_ver_minor = MINOR_REVISION_NUMBER;
	*p_ver_build = BUILD_NUMBER;
	return S_OK;
}

STDMETHODIMP CSolimarLicenseSvr::Challenge(VARIANT vtChallenge, VARIANT *pvtResponse)
{
/*
const size_t debugStringSize = 1024;
wchar_t debug_buf[debugStringSize];
_snwprintf_s(debug_buf, debugStringSize, L"CSolimarLicenseSvr::Challenge 1)");
debug_buf[debugStringSize-1] = 0;
OutputDebugStringW(debug_buf);
*/
	HRESULT hr = S_OK;
	
	VariantClear(pvtResponse);
	
	// Initialize the ChallengeResponse class with the private key, to decrypt the message
	ChallengeResponse CR(challenge_key_server_userauththis_private, sizeof(challenge_key_server_userauththis_private)/sizeof(BYTE));
	ChallengeResponse::Message challenge(vtChallenge), response;
	
	hr = CR.GenerateResponse(challenge,response);
	
	if (SUCCEEDED(hr))
	{
		*pvtResponse = response;
	}
	
	return hr;
}

STDMETHODIMP CSolimarLicenseSvr::GetChallenge(VARIANT *pvtChallenge)
{
	return GetChallengeInternal(pvtChallenge);
}

STDMETHODIMP CSolimarLicenseSvr::PutResponse(VARIANT vtResponse)
{
	return PutResponseInternal(vtResponse);
}

STDMETHODIMP CSolimarLicenseSvr::Heartbeat()
{
	return g_licenseController.Heartbeat(m_licenseId);
}

STDMETHODIMP CSolimarLicenseSvr::KeyEnumerate(VARIANT *keylist)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyEnumerate(keylist);
}

STDMETHODIMP CSolimarLicenseSvr::EnterPassword(BSTR password)
{
	CHECK_CLIENT_AUTHENTICATION;
	HRESULT hr = g_licenseController.keyserver.EnterPassword(password);
	g_licenseController.keyserver.ResynchronizeKeys(true);
	return hr;
}

HRESULT CSolimarLicenseSvr::EnterPasswordPacket(VARIANT vtPasswordPacket, BSTR *verification_code)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.EnterPasswordPacket(vtPasswordPacket,verification_code);
}


STDMETHODIMP CSolimarLicenseSvr::GenerateBasePassword(long customer_number, long key_number, BSTR *password)
{
	SOLIMAR_INTERNAL_PROTECTED_FUNCTION;
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.GenerateBasePassword(customer_number, key_number, password);
}

STDMETHODIMP CSolimarLicenseSvr::GenerateApplicationInstancePassword(long customer_number, long key_number, long license_count, long password_number, BSTR *password)
{
	SOLIMAR_INTERNAL_PROTECTED_FUNCTION;
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.GenerateApplicationInstancePassword(customer_number, key_number, license_count, password_number, password);
}
STDMETHODIMP CSolimarLicenseSvr::GenerateVersionPassword(long customer_number, long key_number, long ver_major, long ver_minor, BSTR *password)
{
	SOLIMAR_INTERNAL_PROTECTED_FUNCTION;
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.GenerateVersionPassword(customer_number, key_number, ver_major, ver_minor, password);
}

STDMETHODIMP CSolimarLicenseSvr::GenerateExtensionPassword(long customer_number, long key_number, long extend_days, long extension_num, BSTR *password)
{
	SOLIMAR_INTERNAL_PROTECTED_FUNCTION;
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.GenerateExtensionPassword(customer_number, key_number, extend_days, extension_num, password);
}

STDMETHODIMP CSolimarLicenseSvr::GenerateModulePassword(long customer_number, long key_number, long product_ident, long module_ident, long license_count, BSTR *password)
{
	SOLIMAR_INTERNAL_PROTECTED_FUNCTION;
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.GenerateModulePassword(customer_number, key_number, product_ident, module_ident, license_count, password);
}
STDMETHODIMP CSolimarLicenseSvr::GenerateModulePassword2(long customer_number, long key_number, long product_ident, long module_ident, long license_count, long password_number, BSTR *password)
{
	SOLIMAR_INTERNAL_PROTECTED_FUNCTION;
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.GenerateModulePassword(customer_number, key_number, product_ident, module_ident, license_count, password, password_number);
}

STDMETHODIMP CSolimarLicenseSvr::GetLicenseServerTime(VARIANT *pvtSystemTime)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.GetLicenseServerTime(pvtSystemTime);
}

// Password packet management
STDMETHODIMP CSolimarLicenseSvr::PasswordPacketInitialize()
{
	SOLIMAR_INTERNAL_PROTECTED_FUNCTION;
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.PasswordPacketInitialize(m_licenseId);
}

STDMETHODIMP CSolimarLicenseSvr::PasswordPacketAppendPassword(VARIANT vtExpires, BSTR password)
{
	SOLIMAR_INTERNAL_PROTECTED_FUNCTION;
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.PasswordPacketAppendPassword(m_licenseId, vtExpires, password);
}

STDMETHODIMP CSolimarLicenseSvr::PasswordPacketFinalize()
{
	SOLIMAR_INTERNAL_PROTECTED_FUNCTION;
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.PasswordPacketFinalize(m_licenseId);
}	

STDMETHODIMP CSolimarLicenseSvr::PasswordPacketGetPacket(VARIANT *pvtPacketData)
{
	SOLIMAR_INTERNAL_PROTECTED_FUNCTION;
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.PasswordPacketGetPacket(m_licenseId, pvtPacketData);
}

STDMETHODIMP CSolimarLicenseSvr::PasswordPacketGetVerificationCode(BSTR *verification_code)
{
	SOLIMAR_INTERNAL_PROTECTED_FUNCTION;
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.PasswordPacketGetVerificationCode(m_licenseId, verification_code);
}


STDMETHODIMP CSolimarLicenseSvr::KeyTrialExpires(BSTR key_ident, VARIANT *expire_date)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyTrialExpires(key_ident, expire_date);
}

STDMETHODIMP CSolimarLicenseSvr::KeyTrialHours(BSTR key_ident, long *trial_hours)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyTrialHours(key_ident, trial_hours);
}

STDMETHODIMP CSolimarLicenseSvr::KeyIsActive(BSTR key_ident, VARIANT_BOOL *key_active)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyIsActive(key_ident, key_active);
}

STDMETHODIMP CSolimarLicenseSvr::KeyIsProgrammed(BSTR key_ident, VARIANT_BOOL *key_programmed)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyIsProgrammed(key_ident, key_programmed);
}

STDMETHODIMP CSolimarLicenseSvr::KeyHeaderQuery(BSTR key_ident, long header_ident, VARIANT *value)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyHeaderQuery(key_ident, header_ident, value);
}

STDMETHODIMP CSolimarLicenseSvr::KeyIsPresent(BSTR key_ident, VARIANT_BOOL *key_present)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyIsPresent(key_ident, key_present);
}

STDMETHODIMP CSolimarLicenseSvr::KeyLock(BSTR key_ident)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyLock(m_licenseId, key_ident);
}

STDMETHODIMP CSolimarLicenseSvr::KeyUnlock(BSTR key_ident)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyUnlock(m_licenseId, key_ident);
}

STDMETHODIMP CSolimarLicenseSvr::KeyObtain(BSTR key_ident)
{
	CHECK_CLIENT_AUTHENTICATION;

	//This starts the heart beat. The calling program must maintain the heart beat
	//to keep the resource obtained.
	g_licenseController.Heartbeat(m_licenseId);
	return g_licenseController.keyserver.KeyObtain(m_licenseId, key_ident);
}

STDMETHODIMP CSolimarLicenseSvr::KeyRelease(BSTR key_ident)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyRelease(m_licenseId, key_ident);
}

STDMETHODIMP CSolimarLicenseSvr::KeyValidateLicense(BSTR key_ident, VARIANT_BOOL *license_valid)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyValidateLicense(m_licenseId, key_ident, license_valid);
}

STDMETHODIMP CSolimarLicenseSvr::KeyModuleEnumerate(BSTR key_ident, VARIANT *key_module_list)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyModuleEnumerate(key_ident, key_module_list);
}

STDMETHODIMP CSolimarLicenseSvr::KeyModuleQuery(BSTR key_ident, long module_ident, VARIANT *vtValue)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyModuleQuery(key_ident, module_ident, vtValue);
}

STDMETHODIMP CSolimarLicenseSvr::KeyModuleLicenseTotal(BSTR key_ident, long module_ident, long* license_count)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyModuleLicenseTotal(m_licenseId, key_ident, module_ident, license_count);
}

STDMETHODIMP CSolimarLicenseSvr::KeyModuleLicenseInUse(BSTR key_ident, long module_ident, long* license_count)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyModuleInUse(key_ident, module_ident, license_count);	//For Backward Compatibility
}

STDMETHODIMP CSolimarLicenseSvr::KeyModuleLicenseObtain(BSTR key_ident, long module_ident, long license_count)
{
	CHECK_CLIENT_AUTHENTICATION;

	//This starts the heart beat. The calling program must maintain the heart beat
	//to keep the resource obtained.
	g_licenseController.Heartbeat(m_licenseId);
	return g_licenseController.keyserver.KeyModuleLicenseObtain(m_licenseId, key_ident, module_ident, license_count);
}

STDMETHODIMP CSolimarLicenseSvr::KeyModuleLicenseRelease(BSTR key_ident, long module_ident, long license_count)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyModuleLicenseRelease(m_licenseId, key_ident, module_ident, license_count);
}

STDMETHODIMP CSolimarLicenseSvr::KeyModuleLicenseCounterDecrement(BSTR key_ident, long module_ident, long license_count)
{
	CHECK_CLIENT_AUTHENTICATION;

	//This starts the heart beat. The calling program must maintain the heart beat
	//to keep the resource obtained.
	g_licenseController.Heartbeat(m_licenseId);
	return g_licenseController.keyserver.KeyModuleLicenseCounterDecrement(m_licenseId, key_ident, module_ident, license_count);
}

STDMETHODIMP CSolimarLicenseSvr::KeyModuleLicenseInUse2(BSTR key_ident, long module_ident, long* license_count)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyModuleLicenseInUse(m_licenseId, key_ident, module_ident, license_count);
}

STDMETHODIMP CSolimarLicenseSvr::KeyModuleInUse(BSTR key_ident, long module_ident, long* license_count)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyModuleInUse(key_ident, module_ident, license_count);
}

STDMETHODIMP CSolimarLicenseSvr::KeyModuleLicenseInUse_ByApp(BSTR key_ident, long module_ident, long* license_count)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyModuleLicenseInUse_ByApp(m_licenseId, key_ident, module_ident, license_count);
}

STDMETHODIMP CSolimarLicenseSvr::KeyModuleLicenseUnlimited(BSTR key_ident, long module_ident, VARIANT_BOOL b_module_is_unlimited)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyModuleLicenseUnlimited(m_licenseId, key_ident, module_ident, b_module_is_unlimited);
}
// Sets all writable cells on a key to zero
STDMETHODIMP CSolimarLicenseSvr::KeyFormat(BSTR key_ident, BSTR *new_key_ident)
{
	SOLIMAR_INTERNAL_PROTECTED_FUNCTION;
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyFormat(key_ident, new_key_ident);
}

// Programs a key
STDMETHODIMP CSolimarLicenseSvr::KeyProgram(BSTR key_ident, long customer_number, long key_number, long product_ident, long ver_major, long ver_minor, long key_type, long days, VARIANT module_value_list, BSTR *new_key_ident)
{
	SOLIMAR_INTERNAL_PROTECTED_FUNCTION;
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyProgram(key_ident, customer_number, key_number, product_ident, ver_major, ver_minor, key_type, 1/*application_instances*/, days, module_value_list, new_key_ident);
}
STDMETHODIMP CSolimarLicenseSvr::KeyProgram2(BSTR key_ident, long customer_number, long key_number, long product_ident, long ver_major, long ver_minor, long key_type, long application_instances, long days, VARIANT module_value_list, BSTR *new_key_ident)
{
	SOLIMAR_INTERNAL_PROTECTED_FUNCTION;
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyProgram(key_ident, customer_number, key_number, product_ident, ver_major, ver_minor, key_type, application_instances, days, module_value_list, new_key_ident);
}


// Reads raw data off of the key
STDMETHODIMP CSolimarLicenseSvr::KeyReadRaw(BSTR key_ident, VARIANT *pvtKeyData)
{
	SOLIMAR_INTERNAL_PROTECTED_FUNCTION;
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.keyserver.KeyReadRaw(key_ident, pvtKeyData);
}

STDMETHODIMP CSolimarLicenseSvr::GetLicenseMessageList(VARIANT_BOOL clear_messages, VARIANT *pvtMessageList)
{
	CHECK_CLIENT_AUTHENTICATION;
	if (clear_messages!=VARIANT_TRUE) return E_INVALIDARG;
	return g_licenseController.GetLicenseMessageList(m_licenseId, pvtMessageList);
}

STDMETHODIMP CSolimarLicenseSvr::DispatchLicenseMessageList(VARIANT_BOOL clear_messages)
{
	return E_NOTIMPL;
}




//
// ISoftwareLicensingMessage
STDMETHODIMP CSolimarLicenseSvr::GetSoftwareLicenseMessageList(VARIANT_BOOL clear_messages, VARIANT *pvtMessageList)
{
	CHECK_CLIENT_AUTHENTICATION;
	if (clear_messages!=VARIANT_TRUE) return E_INVALIDARG;
	return g_licenseController.GetSoftwareLicenseMessageList(m_licenseId, pvtMessageList);
}
STDMETHODIMP CSolimarLicenseSvr::DispatchSoftwareLicenseMessageList(VARIANT_BOOL clear_messages)
{
	return E_NOTIMPL;
}
//
// ISolimarSoftwareLicenseSvr
STDMETHODIMP CSolimarLicenseSvr::SoftwareModuleLicenseTotalForAll_ByProduct(long productID, long moduleIdent, long* pLicenseCount)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.ModuleLicenseTotalForAll(productID, moduleIdent, pLicenseCount);
}

STDMETHODIMP CSolimarLicenseSvr::SoftwareModuleLicenseInUseForAll_ByProduct(long productID, long moduleIdent, long* pLicenseCount)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.ModuleLicenseInUseForAll(productID, moduleIdent, pLicenseCount);
}

STDMETHODIMP CSolimarLicenseSvr::SoftwareValidateLicenseApp_ByProduct(long productID, VARIANT_BOOL *pbLicenseValid)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.ValidateLicense(productID, m_licenseId, pbLicenseValid);
}


//
//Retrieves the sum of the InUse count for all the LicenseInstances that are connected with the same Application Instance name, not just the current LicenseInstances.
STDMETHODIMP CSolimarLicenseSvr::SoftwareModuleLicenseInUseByApp_ByProduct(long productID, long moduleIdent, long* pLicenseCount)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.ModuleLicenseInUseByApp(productID, m_licenseId, moduleIdent, pLicenseCount);
}
STDMETHODIMP CSolimarLicenseSvr::SoftwareModuleLicenseInUseByConnection_ByProduct(long productID, long moduleIdent, long* pLicenseCount)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.ModuleLicenseInUseByLicenseID(productID, m_licenseId, moduleIdent, pLicenseCount);
}

STDMETHODIMP CSolimarLicenseSvr::SoftwareModuleLicenseObtainByApp_ByProduct(long productID, long moduleIdent, long licenseCount)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.ModuleLicenseObtainByApp(productID, m_licenseId, moduleIdent, licenseCount);
}

STDMETHODIMP CSolimarLicenseSvr::SoftwareModuleLicenseReleaseByApp_ByProduct(long productID, long moduleIdent, long licenseCount)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.ModuleLicenseReleaseByApp(productID, m_licenseId, moduleIdent, licenseCount);
}

STDMETHODIMP CSolimarLicenseSvr::SoftwareModuleLicenseCounterDecrementByApp_ByProduct(long productID, long moduleIdent, long licenseCount)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.ModuleLicenseDecrementCounterByApp(productID, m_licenseId, moduleIdent, licenseCount);
}



STDMETHODIMP CSolimarLicenseSvr::GetSoftwareLicenseInfoByProduct_ForAll(long productID, BSTR *pBstrProductInfoAttribsStream)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.GetSoftwareLicenseInfoByProduct_ForAll(productID, pBstrProductInfoAttribsStream);
}

STDMETHODIMP CSolimarLicenseSvr::GetSoftwareLicenseInfo_ForAll(BSTR *pBstrLicenseInfoAttribsStream)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.GetSoftwareLicenseInfo_ForAll(pBstrLicenseInfoAttribsStream);
}

STDMETHODIMP CSolimarLicenseSvr::GetAllSoftwareLicenses(BSTR *pBstrLicenseListStream)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.GetAllSoftwareLicenses(pBstrLicenseListStream);
}

STDMETHODIMP CSolimarLicenseSvr::GetSoftwareLicenseInfoByProduct_ByLicense(BSTR softwareLicense, long productID, BSTR *pBstrProductInfoAttribsStream)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.GetSoftwareLicenseInfoByProduct_ByLicense(softwareLicense, productID, pBstrProductInfoAttribsStream);
}

STDMETHODIMP CSolimarLicenseSvr::GetSoftwareLicenseInfo_ByLicense(BSTR softwareLicense, BSTR *pBstrLicenseInfoAttribsStream)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.GetSoftwareLicenseInfo_ByLicense(softwareLicense, pBstrLicenseInfoAttribsStream);
}
STDMETHODIMP CSolimarLicenseSvr::GetSoftwareLicenseStatus_ByProduct(long productID, BSTR *pBstrStringToDwordMap)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.GetSoftwareLicenseStatus_ByProduct(productID, pBstrStringToDwordMap);
}
STDMETHODIMP CSolimarLicenseSvr::GetSoftwareLicenseStatus_ByLicense(BSTR softwareLicense)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.GetSoftwareLicenseStatus_ByLicense(softwareLicense);
}

STDMETHODIMP CSolimarLicenseSvr::GetSoftwareSpecByProduct(long productID, BSTR *pBstrProductSoftwareSpecAttribs)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.GetSoftwareSpecByProduct(productID, pBstrProductSoftwareSpecAttribs);
}
STDMETHODIMP CSolimarLicenseSvr::GetSoftwareSpec(BSTR *pBstrSoftwareSpecAttribsStream)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.GetSoftwareSpec(pBstrSoftwareSpecAttribsStream);
}



STDMETHODIMP CSolimarLicenseSvr::SoftwareAddApplicationInstanceByProduct(long productID, BSTR applicationInstance)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.AddApplicationInstance(productID, m_licenseId, applicationInstance);
}

STDMETHODIMP CSolimarLicenseSvr::SoftwareRemoveApplicationInstanceByProduct(long productID, BSTR applicationInstance)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.RemoveApplicationInstance(productID, m_licenseId, applicationInstance);
}

STDMETHODIMP CSolimarLicenseSvr::SoftwareGetApplicationInstanceListByProduct(long productID, BSTR *pBstrListAppInstStream)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.GetApplicationInstanceList(productID, m_licenseId, pBstrListAppInstStream);
}

STDMETHODIMP CSolimarLicenseSvr::GenerateSoftwareLicPacket(BSTR bstrLicPackageAttribsStream, VARIANT vtExpires, BSTR *pBstrVerificationCode, VARIANT* pVtLicensePacket)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.GenerateSoftwareLicPacket(bstrLicPackageAttribsStream, vtExpires, pBstrVerificationCode, pVtLicensePacket);
}
STDMETHODIMP CSolimarLicenseSvr::EnterSoftwareLicPacket(VARIANT vtLicensePacket, BSTR *pBstrVerificationCode)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.EnterSoftwareLicPacket(vtLicensePacket, pBstrVerificationCode);
}
STDMETHODIMP CSolimarLicenseSvr::GenerateSoftwareLicArchive_ByLicense(BSTR softwareLicense, VARIANT* pVtLicenseArchive)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.GenerateSoftwareLicArchive_ByLicense(softwareLicense, pVtLicenseArchive);
}

STDMETHODIMP CSolimarLicenseSvr::EnterSoftwareLicArchive(VARIANT vtLicenseArchive)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.EnterSoftwareLicArchive(vtLicenseArchive);
}


STDMETHODIMP CSolimarLicenseSvr::GenerateVerifyDataWithVerCode_ByLicense(BSTR softwareLicense, VARIANT* pVtLicensePacket)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.GenerateVerifyDataWithVerCode_ByLicense(softwareLicense, pVtLicensePacket);
}
STDMETHODIMP CSolimarLicenseSvr::GenerateVerifyDataWithLicInfo_ByLicense(BSTR softwareLicense, VARIANT* pVtLicensePacket)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.GenerateVerifyDataWithLicInfo_ByLicense(softwareLicense, pVtLicensePacket);
}
STDMETHODIMP CSolimarLicenseSvr::GenerateLicPackage_ByVerifyData(VARIANT vtVerifyData, BSTR *pBstrLicensePackageAttribsStream)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.GenerateLicPackage_ByVerifyData(vtVerifyData, pBstrLicensePackageAttribsStream);
}


STDMETHODIMP CSolimarLicenseSvr::GenerateLicPackage_BySoftwareLicArchive(VARIANT vtLicenseArchive, BSTR *pBstrLicensePackageAttribsStream)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.GenerateLicPackage_BySoftwareLicArchive(vtLicenseArchive, pBstrLicensePackageAttribsStream);
}
STDMETHODIMP CSolimarLicenseSvr::GenerateLicPackage_BySoftwareLicPacket(VARIANT vtLicensePacket, BSTR *pBstrLicensePackageAttribsStream)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.GenerateLicPackage_BySoftwareLicPacket(vtLicensePacket, pBstrLicensePackageAttribsStream);
}
STDMETHODIMP CSolimarLicenseSvr::GenerateLicenseSystemData(VARIANT* pVtLicSysDataPacket)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.GenerateLicenseSystemData(pVtLicSysDataPacket);
}
STDMETHODIMP CSolimarLicenseSvr::GenerateStream_ByLicenseSystemData(VARIANT vtLicSysDataPacket, BSTR *pBstrLicSysDataAttribsStream)
{
	// Comment Out for testing
	SOLIMAR_INTERNAL_PROTECTED_FUNCTION;
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.GenerateStream_ByLicenseSystemData(vtLicSysDataPacket, pBstrLicSysDataAttribsStream);
}

STDMETHODIMP CSolimarLicenseSvr::ValidateToken_ByLicense(BSTR softwareLicense, long validationTokenType, BSTR validationValue)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.ValidateToken_ByLicense(softwareLicense, validationTokenType, validationValue);
}
STDMETHODIMP CSolimarLicenseSvr::SoftwareLicenseUseActivationToExtendTime_ByLicense(BSTR softwareLicense)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.SoftwareLicenseUseActivationToExtendTime_ByLicense(softwareLicense);
}

//
// ISolimarConversionToSoftwareLicenseSvr
STDMETHODIMP CSolimarLicenseSvr::ConvertProtectionKeyToSoftwareLicense(BSTR softwareLicense, BSTR keyIdent)
{
	CHECK_CLIENT_AUTHENTICATION;
	return g_licenseController.softwareServer.ConvertProtectionKeyToSoftwareLicense(softwareLicense, keyIdent);	
}

