#ifndef _LicensingServerWrapper_h_
#define _LicensingServerWrapper_h_

#pragma warning(disable:4786)

#include <string>
//#include "..\SolimarLicenseServer\KeySpec.h"

#include "..\SolimarLicenseServer\_SolimarLicenseServer.h"
#include "..\common\ChallengeResponseHelper.h"
#include "..\common\LicensingMessage.h"
#include "..\Common\GIT.h"
#include "..\common\LicenseError.h"
#include "..\common\ss_rpc_failed.h"


namespace SolimarLicenseManagerWrapper
{
	/*
	 * LIC_SSLSERVER_FTCALL 
	 *	Fault tolerant call for ISolimarSoftwareLicenseSvr
	 */
	#define LIC_SSLSERVER_FTCALL(sslSever, func, plist) \
	{ /* begin hr scope */ \
	HRESULT hr; \
	LIC_SSLSERVER_FTCALL_HR(sslSever, func, plist, hr) \
	if(FAILED(hr)) \
		throw _com_error(hr); \
	} /* end hr scope */ \

	/*
	 * LIC_SSLSERVER_FTCALL_HR 
	 *	Fault tolerant call for ISolimarSoftwareLicenseSvr
	 */
	#define LIC_SSLSERVER_FTCALL_HR(sslSever, func, plist, hr) \
	{ /* begin hr scope */ \
		bool __retry; \
		bool __connected = true; \
		do { \
			try \
			{ \
				__retry = false; \
				HRESULT connectHR = LicenseServerError::EHR_CLIENT_TIMEOUT; \
				if (!__connected ) \
				{ \
					connectHR = Connect(licServerName); \
					/*ReConnect(&srvInfoObj);*/ \
				} \
				if (__connected || SUCCEEDED(connectHR)) \
				{ \
					hr = sslSever->##func##plist; \
					if(FAILED(hr)) throw _com_error(hr); \
				} \
				else \
					throw _com_error(connectHR); \
			} \
			catch (_com_error& e) \
			{ \
				__connected = false; \
				if (SS_RPC_FAILED(e.Error())) \
				{ \
					if (SUCCEEDED(Connect(licServerName))) \
					{ \
						__retry = true; \
						__connected = true; \
					} \
				} \
				if (!__retry) \
					hr = e.Error(); \
			} \
		} while (__retry); \
	} /* end scope */ \

	class LicensingServerWrapper : public ChallengeResponseHelper
	{
		public:
			LicensingServerWrapper();
			LicensingServerWrapper(const LicensingServerWrapper &o);
			LicensingServerWrapper& operator=(const LicensingServerWrapper &o);
			virtual ~LicensingServerWrapper();

			HRESULT Connect(std::wstring server);
			HRESULT Disconnect();
			HRESULT SoftwareModuleLicenseTotalForAll_ByProduct(long productID, long moduleIdent, long* pLicenseCount);
			HRESULT SoftwareModuleLicenseInUseForAll_ByProduct(long productID, long moduleIdent, long* pLicenseCount);
			//HRESULT SoftwareModuleLicenseInUseByApp_ByProduct(BSTR bstrApplication, long productID, long moduleIdent, long* pLicenseCount);
			//HRESULT SoftwareModuleLicenseInUseByConnection_ByProduct(long productID, long moduleIdent, long* pLicenseCount);	//Don't think I need to expose this function
			HRESULT GetSoftwareLicenseInfoByProduct_ForAll(long productID, BSTR *pBstrProductInfoAttribsStream);
			HRESULT GetSoftwareLicenseInfo_ForAll(BSTR *pBstrLicenseInfoAttribsStream);
			HRESULT GetAllSoftwareLicenses(BSTR *pBstrAllLicensesStream);
			HRESULT GetSoftwareLicenseInfoByProduct_ByLicense(BSTR bstrSoftwareLicense, long productID, BSTR *pBstrProductInfoAttribsStream);
			HRESULT GetSoftwareLicenseInfo_ByLicense(BSTR bstrSoftwareLicense, BSTR *pBstrLicenseInfoAttribsStream);
			HRESULT GetSoftwareLicenseStatus_ByProduct(long productID, BSTR *pBstrStringToDwordMap);
			HRESULT GetSoftwareLicenseStatus_ByLicense(BSTR bstrSoftwareLicense);
			HRESULT ValidateToken_ByLicense(BSTR bstrSoftwareLicense, long validationTokenType, BSTR bstrValidationValue);
			HRESULT GetSoftwareSpecByProduct(long productID, BSTR *pBstrProductSoftwareSpecAttribs);
			HRESULT GetSoftwareSpec(BSTR *pBstrSoftwareSpecAttribsStream);
			HRESULT SoftwareGetApplicationInstanceListByProduct(long productID, BSTR *pBstrListAppInstStream);

			HRESULT GenerateSoftwareLicPacket(BSTR bstrLicPackageAttribsStream, VARIANT vtExpires, BSTR *pBstrVerificationCode, VARIANT* pVtLicensePacket);
			HRESULT EnterSoftwareLicPacket(VARIANT vtLicensePacket, BSTR *pBstrVerificationCode);
			HRESULT EnterProtectionKeyPassword(BSTR bstrPassword);
			HRESULT EnterProtectionKeyPasswordPacket(VARIANT vtLicensePacket, BSTR *pBstrVerificationCode);

			HRESULT GenerateSoftwareLicArchive_ByLicense(BSTR softwareLicense, VARIANT* pVtLicenseArchive);
			HRESULT EnterSoftwareLicArchive(VARIANT vtLicenseArchive);
			HRESULT GenerateVerifyDataWithVerCode_ByLicense(BSTR bstrSoftwareLicense, VARIANT* pVtLicensePacket);
			HRESULT GenerateVerifyDataWithLicInfo_ByLicense(BSTR bstrSoftwareLicense, VARIANT* pVtLicensePacket);
			HRESULT GenerateLicPackage_ByVerifyData(VARIANT vtVerifyData, BSTR *pBstrLicensePackageAttribsStream);
			HRESULT GenerateLicPackage_BySoftwareLicArchive(VARIANT vtLicenseArchive, BSTR *pBstrLicensePackageAttribsStream);
			HRESULT GenerateLicPackage_BySoftwareLicPacket(VARIANT vtLicensePacket, BSTR *pBstrLicensePackageAttribsStream);
			HRESULT GenerateLicenseSystemData(VARIANT* pVtLicSysDataPacket);
			HRESULT GenerateStreamData_ByLicenseSystemData(VARIANT vtLicSysDataPacket, BSTR *pBstrCreatedDateStreamed, BSTR *pBstrKeyAttribsListStream, BSTR *pBstrLicUsageDataAttribsStream, BSTR *pBstrConnectionAttribsListStream, BSTR *pBstrEventLogAttribsListStream, BSTR *pBstrLicInfoDataAttribsListStream);
			HRESULT GenerateStream_ByLicenseSystemData(VARIANT vtLicSysDataPacket, BSTR *pBstrLicSysDataAttribsStream);

			HRESULT SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber(BSTR softwareLicense, BSTR contractNumber);

			HRESULT GetEventLogList_ForLicenseServer(BSTR *pBstrEventLogAttribsListStream);

			HRESULT KeyEnumerate(VARIANT *pVtKeyList);
			HRESULT KeyHeaderQuery(BSTR bstrKey, long headerIdent, VARIANT *pVtValue);
			HRESULT KeyTrialHours(BSTR bstrKey, long *plTrialHours);
			HRESULT KeyIsActive(BSTR bstrKey, VARIANT_BOOL *pVtKeyActive);
			HRESULT KeyIsProgrammed(BSTR bstrKey, VARIANT_BOOL *pVtKeyProgrammed);
			HRESULT KeyTrialExpires(BSTR bstrKey, VARIANT *pVtExpireDate);

			HRESULT KeyModuleEnumerate(BSTR bstrKey, VARIANT *pVtKeyModuleList);
			HRESULT KeyModuleLicenseTotal(BSTR bstrKey, long moduleIdent, long* pLicenseTotal);
			HRESULT KeyModuleInUse(BSTR bstrKey, long moduleIdent, long* pLicenseInUse);

			HRESULT KeyFormat(BSTR bstrKey, BSTR *pBstrNewKey);
			HRESULT KeyProgram2(BSTR bstrKey, long customerNumber, long keyNumber, long productIdent, long verMajor, long verMinor, long keyType, long application_instances, long days, VARIANT module_value_list, BSTR *pBstrNewKey);

		private:
			
			static BYTE challenge_key_manager_userauththis_public[];
			static BYTE challenge_key_manager_thisauthuser_private[];
			static BYTE challenge_key_server_thisauthuser_private[];
			static BYTE challenge_key_server_userauththis_public[];

			GITPtr<ISolimarLicenseSvr4> licenseServer;
			GITPtr<ISolimarSoftwareLicenseSvr> licenseSoftwareServer;
			bool bConnected;
			std::wstring licServerName;
	};
}
#endif