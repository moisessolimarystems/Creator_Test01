#pragma once
#include <comdef.h>
#include "RainbowDriver.h"
#include "..\Common\Version.h"
#include "..\Common\LicAttribsCPP\Lic_PackageAttribs.h"
#include "..\Common\SoftwareSpecInstance.h"	//For GlobalSoftwareSpec::GlobalSoftwareSpec g_pSoftwareSpec
#include "SoftwareServerDataMgr.h"	//For SoftwareServerDataMgr & SoftwareLicenseFile


class SoftwareLicenseMgr
{
	public:
		SoftwareLicenseMgr();
		~SoftwareLicenseMgr();

		//
		//Will return an different error HRESULTs if 
		//	1) There is no License File for the Key
		//	2) LicenseFile and Key don't have matching "special codes"
		//
		HRESULT Initialize(_bstr_t bstrLicenseFile, RainbowDriver* pRainbowKeyDriver, SoftwareServerDataMgr* pLicServerDataMgr);
		HRESULT Verify(bool* pBValid, bool bForceRefresh);

		HRESULT GetSoftwareLicenseName(BSTR *pBstrLicenseName);
		HRESULT GetVerificationAttribs(Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_VerificationAttribs* pVerificationAttrib);
		HRESULT GetLicenseInfo(Lic_PackageAttribs::Lic_LicenseInfoAttribs* pLicenseInfo);

		HRESULT GetProtectionKeyModifiedDate(time_t* pVtModifiedDate);
		HRESULT GetProtectionKeyModifiedDate(time_t* pVtModifiedDate, Lic_PackageAttribs* pLicenseFileAttribs);
		HRESULT GetProtectionKeyCurrentActivations(int* pCurrentActivations, Lic_PackageAttribs* pLicenseFileAttribs);

		HRESULT DeleteLicenseFile();
		HRESULT ContainsLicenseForProduct(long _productID);
		//HRESULT GetSoftwareSpec(Lic_PackageAttribs::Lic_SoftwareSpecAttribs* pSoftwareSpec);
		HRESULT ApplyLicensePacket(Lic_PackageAttribs* pLicPacket, _bstr_t bstrVerificationCode);
		HRESULT EnterLicenseArchive(Lic_PackageAttribs* pLicInfo);

		HRESULT SetVerificationAttribs(Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_VerificationAttribs* pVerificationAttrib);
		HRESULT UpdateLicenseCode( _bstr_t bstrLicenseCode);

		HRESULT UseActivationToExtendTime();

		//HRESULT EnterLicensePacket(VARIANT vtPasswordPacket, BSTR *pBstrVerificationCode);
		/*
		HRESULT GetLicenseStreamVersion(Version::ModuleVersion* pLicenseStreamVersion);
		HRESULT GetLicenseStream(BSTR* pBstrLicenseStream);
		HRESULT GetSoftwareSpecVersion(Version::ModuleVersion* pSoftwareSpecVersion);
		HRESULT GetSoftwareSpecStream(BSTR* pBstrSoftwareStream);

		//HRESULT ApplyLicensePacket(BSTR *pBstrVerificationCode);
		HRESULT ApplyLicensePacket(LicensePacket* pLicPacket, BSTR *pBstrVerificationCode);
		*/

		HRESULT ValidateHardwareKeyID(_bstr_t bstrValidationValue);
		HRESULT ValidateLicenseCode(_bstr_t bstrValidationValue, bool bCheckProtectionKey = false);
		HRESULT ValidateHardwareBiosSerialNumber(_bstr_t bstrValidationValue);
		HRESULT ValidateHardwareMacAddress(_bstr_t bstrValidationValue);
		HRESULT ValidateHardwareCompuerName(_bstr_t bstrValidationValue);
	private:
		time_t m_lastTimeCheck;
		RainbowDriver* m_pRainbowKeyDriver;
		SoftwareServerDataMgr* m_pLicServerDataMgr;
		_bstr_t m_bstrLicenseFile;
		_bstr_t m_bstrLicenseName;
		

		Lic_PackageAttribs m_licenseFileAttribs;

		HRESULT RefreshLicenseFileAttribs();
		HANDLE softwareLicenseMgrLock;

		HRESULT InternalUpdate(Lic_PackageAttribs* _pLicPacAttribs, _bstr_t _newLicenseGUID);

		HRESULT VerifyForSoftwareLicenseUpgrade(Lic_PackageAttribs* _pLicPackageAttribs, bool _bIgnoreLicenseCode, bool _bIgnoreModifiedDate);


		Lic_PackageAttribs::Lic_ProductInfoAttribs TestingOnly_GenerateLicense_Rubika();
		Lic_PackageAttribs::Lic_ProductInfoAttribs TestingOnly_GenerateLicense_SPDE();
		Lic_PackageAttribs::Lic_ProductInfoAttribs TestingOnly_GenerateLicense_SSE();
		Lic_PackageAttribs::Lic_ProductInfoAttribs TestingOnly_GenerateLicense_SDX();
		Lic_PackageAttribs::Lic_ProductInfoAttribs TestingOnly_GenerateLicense_iCONVERT();
		Lic_PackageAttribs::Lic_ProductInfoAttribs TestingOnly_GenerateLicense_SOLfusion();
		Lic_PackageAttribs::Lic_ProductInfoAttribs TestingOnly_GenerateLicense_RubikaProcessBuilder();
		Lic_PackageAttribs::Lic_ProductInfoAttribs TestingOnly_GenerateLicense_SOLindexer();
		Lic_PackageAttribs::Lic_ProductInfoAttribs TestingOnly_GenerateLicense_SPDE_QueueManager();

};