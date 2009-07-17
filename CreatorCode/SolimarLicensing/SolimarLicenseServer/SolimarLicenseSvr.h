// SolimarLicenseSvr.h : Declaration of the CSolimarLicenseSvr

#pragma once

#include "stdafx.h"
#include "resource.h"       // main symbols
#include "..\common\IObjectAuthentication.h"
#include "..\common\ILicensingMessage.h"
#include "..\common\ISoftwareLicensingMessage.h"
#include "..\common\ChallengeResponseHelper.h"
#include "KeyServer.h"
#include "KeyServerInstance.h"	// creates a global keyserver
#include "SoftwareServer.h"
#include "SoftwareServerInstance.h"	// creates a global softwareServer
#include <comutil.h>

// ISolimarLicenseSvr
[
	object,
	uuid("C6CCE857-1A24-41D5-A44C-4DC629CD370D"),
	dual,	helpstring("ISolimarLicenseSvr Interface"),
	pointer_default(unique)
]
__interface ISolimarLicenseSvr : IDispatch
{
	// Top level functions
	[id(1),helpstring("method Heartbeat")] HRESULT Heartbeat();
	[id(2),helpstring("method KeyEnumerate")] HRESULT KeyEnumerate([out,retval] VARIANT *keylist);
	[id(3),helpstring("method EnterPassword")] HRESULT EnterPassword([in] BSTR password);
	[id(4),helpstring("method EnterPasswordPacket")] HRESULT EnterPasswordPacket([in] VARIANT vtPasswordPacket, [out,retval] BSTR *verification_code);
	[id(5),helpstring("method GenerateBasePassword")] HRESULT GenerateBasePassword([in] long customer_number, [in] long key_number, [out,retval] BSTR *password);
	[id(6),helpstring("method GenerateVersionPassword")] HRESULT GenerateVersionPassword([in] long customer_number, [in] long key_number, [in] long ver_major, [in] long ver_minor, [out,retval] BSTR *password);
	[id(7),helpstring("method GenerateExtensionPassword")] HRESULT GenerateExtensionPassword([in] long customer_number, [in] long key_number, [in] long extend_days, [in] long extension_num, [out,retval] BSTR *password);
	[id(8),helpstring("method GenerateModulePassword")] HRESULT GenerateModulePassword([in] long customer_number, [in] long key_number, [in] long product_ident, [in] long module_ident, [in] long license_count, [out,retval] BSTR *password);
	[id(9),helpstring("method GetLicenseServerTime")] HRESULT GetLicenseServerTime([out,retval] VARIANT *pvtSystemTime);
	[id(10),helpstring("method GenerateModulePassword2")] HRESULT GenerateModulePassword2([in] long customer_number, [in] long key_number, [in] long product_ident, [in] long module_ident, [in] long license_count, [in] long password_number, [out,retval] BSTR *password);
	
	// Password packet management
	[id(20),helpstring("method PasswordPacketInitialize")] HRESULT PasswordPacketInitialize();
	[id(21),helpstring("method PasswordPacketAppendPassword")] HRESULT PasswordPacketAppendPassword([in] VARIANT vtExpires, [in] BSTR password);
	[id(22),helpstring("method PasswordPacketFinalize")] HRESULT PasswordPacketFinalize();
	[id(23),helpstring("method PasswordPacketGetPacket")] HRESULT PasswordPacketGetPacket([out,retval] VARIANT *pvtPacketData);
	[id(24),helpstring("method PasswordPacketGetVerificationCode")] HRESULT PasswordPacketGetVerificationCode([out,retval] BSTR *verification_code);
	
	// Key specific functions
	[id(32),helpstring("method KeyTrialExpires")] HRESULT KeyTrialExpires([in] BSTR key_ident, [out,retval] VARIANT *expire_date);
	[id(33),helpstring("method KeyTrialHours")] HRESULT KeyTrialHours([in] BSTR key_ident, [out,retval] long *trial_hours);
	[id(34),helpstring("method KeyIsActive")] HRESULT KeyIsActive([in] BSTR key_ident, [out,retval] VARIANT_BOOL *key_active);
	[id(35),helpstring("method KeyIsProgrammed")] HRESULT KeyIsProgrammed([in] BSTR key_ident, [out,retval] VARIANT_BOOL *key_programmed);
	[id(36),helpstring("method KeyIsPresent")] HRESULT KeyIsPresent([in] BSTR key_ident, [out,retval] VARIANT_BOOL *key_present);
	[id(37),helpstring("method KeyHeaderQuery")] HRESULT KeyHeaderQuery([in] BSTR key_ident, [in] long header_ident, [out,retval] VARIANT *value);
	
	// License locking support
	// locking a key prevents it from being used by others until it is unlocked
	[id(41),helpstring("method KeyLock")] HRESULT KeyLock([in] BSTR key_ident);
	[id(42),helpstring("method KeyUnlock")] HRESULT KeyUnlock([in] BSTR key_ident);
	// indicates that if the key is trial, to start using up hours until it is released
	[id(43),helpstring("method KeyObtain")] HRESULT KeyObtain([in] BSTR key_ident);
	[id(44),helpstring("method KeyRelease")] HRESULT KeyRelease([in] BSTR key_ident);
	
	// Module information and locking
	[id(50),helpstring("method KeyValidateLicense")] HRESULT KeyValidateLicense([in] BSTR key_ident, [out,retval] VARIANT_BOOL *license_valid);
	[id(51),helpstring("method KeyModuleEnumerate")] HRESULT KeyModuleEnumerate([in] BSTR key_ident, [out,retval] VARIANT *key_module_list);
	[id(52),helpstring("method KeyModuleQuery")] HRESULT KeyModuleQuery([in] BSTR key_ident, [in] long module_identifier, [out,retval] VARIANT *vtValue);
	[id(53),helpstring("method KeyModuleLicenseTotal")] HRESULT KeyModuleLicenseTotal([in] BSTR key_ident, [in] long module_ident, [out,retval] long* license_count);
	[id(54),helpstring("method KeyModuleLicenseInUse")] HRESULT KeyModuleLicenseInUse([in] BSTR key_ident, [in] long module_ident, [out,retval] long* license_count);
	[id(55),helpstring("method KeyModuleLicenseObtain")] HRESULT KeyModuleLicenseObtain([in] BSTR key_ident, [in] long module_ident, [in] long license_count);
	[id(56),helpstring("method KeyModuleLicenseRelease")] HRESULT KeyModuleLicenseRelease([in] BSTR key_ident, [in] long module_ident, [in] long license_count);
	[id(57),helpstring("method KeyModuleLicenseCounterDecrement")] HRESULT KeyModuleLicenseCounterDecrement([in] BSTR key_ident, [in] long module_ident, [in] long license_count);
	
	// Key Programming/Creator support
	// Sets all writable cells on a key to zero
	[id(60),helpstring("method KeyFormat")] HRESULT KeyFormat([in] BSTR key_ident, [out,retval] BSTR *new_key_ident);
	// Programs a key
	[id(61),helpstring("method KeyProgram")] HRESULT KeyProgram([in] BSTR key_ident, [in] long customer_number, [in] long key_number, [in] long product_ident, [in] long ver_major, [in] long ver_minor, [in] long key_type, [in] long days, [in] VARIANT module_value_list, [out,retval] BSTR *new_key_ident);
	// Reads raw data off the key
	[id(62),helpstring("method KeyReadRaw")] HRESULT KeyReadRaw([in] BSTR key_ident, [out,retval] VARIANT *pvtKeyData);

};

// ISolimarLicenseSvr2
[
	object,
	uuid("4852B206-97BA-4934-AFD8-35E85F7FC855"),
	dual,	helpstring("ISolimarLicenseSvr2 Interface"),
	pointer_default(unique)
]
__interface ISolimarLicenseSvr2 : ISolimarLicenseSvr
{
	[id(70),helpstring("method KeyModuleLicenseInUse2")] HRESULT KeyModuleLicenseInUse2([in] BSTR key_ident, [in] long module_ident, [out,retval] long* license_count);
	[id(71),helpstring("method KeyModuleInUse")] HRESULT KeyModuleInUse([in] BSTR key_ident, [in] long module_ident, [out,retval] long* license_count);
};

// ISolimarLicenseSvr3
[
	object,
	uuid("BDA195D0-FC1A-4814-9DE0-364394D8D810"),
	dual,	helpstring("ISolimarLicenseSvr3 Interface"),
	pointer_default(unique)
]
__interface ISolimarLicenseSvr3 : ISolimarLicenseSvr2
{
	[id(72),helpstring("method GenerateApplicationInstancePassword")] HRESULT GenerateApplicationInstancePassword([in] long customer_number, [in] long key_number, [in] long license_count, [in] long password_number, [out,retval] BSTR *password);
	// Programs a key
	[id(73),helpstring("method KeyProgram2")] HRESULT KeyProgram2([in] BSTR key_ident, [in] long customer_number, [in] long key_number, [in] long product_ident, [in] long ver_major, [in] long ver_minor, [in] long key_type, [in] long application_instances, [in] long days, [in] VARIANT module_value_list, [out,retval] BSTR *new_key_ident);
	// Gets the Version of the License Server
	[id(74),helpstring("method GetVersionLicenseServer")] HRESULT GetVersionLicenseServer([out] long* p_ver_major, [out] long* p_ver_minor, [out] long* p_ver_build);
	[id(75),helpstring("method AddApplicationInstance")] HRESULT AddApplicationInstance([in] BSTR key_ident, [in] BSTR application_instance, [in] VARIANT_BOOL b_app_instance_lock_key);
	[id(76),helpstring("method RemoveApplicationInstance")] HRESULT RemoveApplicationInstance([in] BSTR key_ident, [in] BSTR application_instance);
	[id(77),helpstring("method GetApplicationInstanceList")] HRESULT GetApplicationInstanceList([in] BSTR key_ident, [out,retval] VARIANT *pvtAppInstanceList);

	// Sets to treat the Module License as Unlimited
	[id(78),helpstring("method KeyModuleLicenseUnlimited")] HRESULT KeyModuleLicenseUnlimited([in] BSTR key_ident, [in] long module_ident, [in] VARIANT_BOOL b_module_is_unlimited);
};

// ISolimarLicenseSvr4
[
	object,
	uuid("70C717AC-46ED-44d8-B0EF-AFCCC63DC98E"),
	dual,	helpstring("ISolimarLicenseSvr4 Interface"),
	pointer_default(unique)
]
__interface ISolimarLicenseSvr4 : ISolimarLicenseSvr3
{
	[id(79),helpstring("method KeyModuleLicenseInUse_ByApp")] HRESULT KeyModuleLicenseInUse_ByApp([in] BSTR key_ident, [in] long module_ident, [out,retval] long* license_count);
};

// ISolimarSoftwareLicenseSvr
[
	object,
	uuid("7FFD4EEB-DC7A-43f0-9117-667D77EF93A2"),
	dual,	helpstring("ISolimarSoftwareLicenseSvr Interface"),
	pointer_default(unique)
]
__interface ISolimarSoftwareLicenseSvr : IDispatch
{
	[id(80),helpstring("method SoftwareModuleLicenseTotalForAll_ByProduct")] HRESULT SoftwareModuleLicenseTotalForAll_ByProduct([in] long productID, [in] long moduleIdent, [out,retval] long* pLicenseCount);
	[id(81),helpstring("method SoftwareModuleLicenseInUseForAll_ByProduct")] HRESULT SoftwareModuleLicenseInUseForAll_ByProduct([in] long productID, [in] long moduleIdent, [out,retval] long* pLicenseCount);
	[id(82),helpstring("method SoftwareValidateLicenseApp_ByProduct")] HRESULT SoftwareValidateLicenseApp_ByProduct([in] long productID, [out,retval] VARIANT_BOOL *pbLicenseValid);
	[id(83),helpstring("method SoftwareModuleLicenseInUseByApp_ByProduct")] HRESULT SoftwareModuleLicenseInUseByApp_ByProduct([in] long productID, [in] long moduleIdent, [out,retval] long* pLicenseCount);
	[id(84),helpstring("method SoftwareModuleLicenseInUseByConnection_ByProduct")] HRESULT SoftwareModuleLicenseInUseByConnection_ByProduct([in] long productID, [in] long moduleIdent, [out,retval] long* pLicenseCount);
	[id(85),helpstring("method SoftwareModuleLicenseObtainByApp_ByProduct")] HRESULT SoftwareModuleLicenseObtainByApp_ByProduct([in] long productID, [in] long moduleIdent, [in] long licenseCount);
	[id(86),helpstring("method SoftwareModuleLicenseReleaseByApp_ByProduct")] HRESULT SoftwareModuleLicenseReleaseByApp_ByProduct([in] long productID, [in] long moduleIdent, [in] long licenseCount);
	[id(87),helpstring("method SoftwareModuleLicenseCounterDecrementByApp_ByProduct")] HRESULT SoftwareModuleLicenseCounterDecrementByApp_ByProduct([in] long productID, [in] long moduleIdent, [in] long licenseCount);

	[id(88),helpstring("method GetSoftwareLicenseInfoByProduct_ForAll")] HRESULT GetSoftwareLicenseInfoByProduct_ForAll([in] long productID, [out,retval] BSTR *pBstrProductInfoAttribsStream);
	[id(89),helpstring("method GetSoftwareLicenseInfo_ForAll")] HRESULT GetSoftwareLicenseInfo_ForAll([out,retval] BSTR *pBstrLicenseInfoAttribsStream);

	[id(90),helpstring("method GetAllSoftwareLicenses")] HRESULT GetAllSoftwareLicenses([out,retval] BSTR *pBstrListAllLicensesStream);
	[id(91),helpstring("method GetSoftwareLicenseInfoByProduct_ByLicense")] HRESULT GetSoftwareLicenseInfoByProduct_ByLicense([in] BSTR softwareLicense, [in] long productID, [out,retval] BSTR *pBstrProductInfoAttribsStream);
	[id(92),helpstring("method GetSoftwareLicenseInfo_ByLicense")] HRESULT GetSoftwareLicenseInfo_ByLicense([in] BSTR softwareLicense, [out,retval] BSTR *pBstrLicenseInfoAttribsStream);

	[id(93),helpstring("method GetSoftwareLicenseStatus_ByProduct")] HRESULT GetSoftwareLicenseStatus_ByProduct([in] long productID, [out,retval] BSTR *pBstrStringToDwordMap);
	[id(94),helpstring("method GetSoftwareLicenseStatus_ByLicense")] HRESULT GetSoftwareLicenseStatus_ByLicense([in] BSTR softwareLicense);

	[id(95),helpstring("method GetSoftwareSpecByProduct")] HRESULT GetSoftwareSpecByProduct([in] long productID, [out,retval] BSTR *pBstrProductSoftwareSpecAttribs);
	[id(96),helpstring("method GetSoftwareSpec")] HRESULT GetSoftwareSpec([out,retval] BSTR *pBstrSoftwareSpecAttribsStream);

	[id(97),helpstring("method SoftwareAddApplicationInstanceByProduct")] HRESULT SoftwareAddApplicationInstanceByProduct([in] long productID, [in] BSTR applicationInstance);
	[id(98),helpstring("method SoftwareRemoveApplicationInstanceByProduct")] HRESULT SoftwareRemoveApplicationInstanceByProduct([in] long productID, [in] BSTR applicationInstance);
	[id(99),helpstring("method SoftwareGetApplicationInstanceListByProduct")] HRESULT SoftwareGetApplicationInstanceListByProduct([in] long productID, [out,retval] BSTR *pBstrListAppInstStream);

	[id(100),helpstring("method GenerateSoftwareLicPacket")] HRESULT GenerateSoftwareLicPacket([in] BSTR bstrLicPackageAttribsStream, [in] VARIANT vtExpires, [out] BSTR *pBstrVerificationCode, [out,retval] VARIANT* pVtLicensePacket);
	[id(101),helpstring("method EnterSoftwareLicPacket")] HRESULT EnterSoftwareLicPacket([in] VARIANT vtLicensePacket, [out,retval] BSTR *pBstrVerificationCode);

	[id(102),helpstring("method ValidateToken_ByLicense")] HRESULT ValidateToken_ByLicense([in] BSTR softwareLicense, [in] long validationTokenType, [in] BSTR validationValue);

	[id(103),helpstring("method SoftwareLicenseUseActivationToExtendTime_ByLicense")] HRESULT SoftwareLicenseUseActivationToExtendTime_ByLicense([in] BSTR softwareLicense);

	[id(104),helpstring("method GenerateVerifyDataWithVerCode_ByLicense")] HRESULT GenerateVerifyDataWithVerCode_ByLicense([in] BSTR softwareLicense, [out,retval] VARIANT* pVtLicensePacket);
	[id(105),helpstring("method GenerateVerifyDataWithLicInfo_ByLicense")] HRESULT GenerateVerifyDataWithLicInfo_ByLicense([in] BSTR softwareLicense, [out,retval] VARIANT* pVtLicensePacket);
	[id(106),helpstring("method GenerateSoftwareLicArchive_ByLicense")] HRESULT GenerateSoftwareLicArchive_ByLicense([in] BSTR softwareLicense, [out,retval] VARIANT* pVtLicenseArchive);
	[id(107),helpstring("method GenerateLicPackage_ByVerifyData")] HRESULT GenerateLicPackage_ByVerifyData([in] VARIANT vtLicensePacket, [out,retval] BSTR *pBstrLicensePackageAttribsStream);
	[id(108),helpstring("method GenerateLicPackage_BySoftwareLicArchive")] HRESULT GenerateLicPackage_BySoftwareLicArchive([in] VARIANT vtLicenseArchive, [out,retval] BSTR *pBstrLicensePackageAttribsStream);
	[id(109),helpstring("method GenerateLicPackage_BySoftwareLicPacket")] HRESULT GenerateLicPackage_BySoftwareLicPacket([in] VARIANT vtLicensePacket, [out,retval] BSTR *pBstrLicensePackageAttribsStream);
	[id(110),helpstring("method EnterSoftwareLicArchive")] HRESULT EnterSoftwareLicArchive([in] VARIANT vtLicenseArchive);
	[id(111),helpstring("method GenerateLicenseSystemData")] HRESULT GenerateLicenseSystemData([out,retval] VARIANT* pVtLicSysDataPacket);
	[id(112),helpstring("method GenerateStream_ByLicenseSystemData")] HRESULT GenerateStream_ByLicenseSystemData([in] VARIANT vtLicSysDataPacket, [out,retval] BSTR *pBstrLicSysDataAttribsStream);
};

[
	object,
	uuid("A275E4D8-B8D6-4fd7-A11A-CE3340F1E0DF"),
	dual,	helpstring("ISolimarConversionToSoftwareLicenseSvr Interface"),
	pointer_default(unique)
]
__interface ISolimarConversionToSoftwareLicenseSvr : IDispatch
{
	[id(113),helpstring("method ConvertProtectionKeyToSoftwareLicense")] HRESULT ConvertProtectionKeyToSoftwareLicense([in] BSTR softwareLicense, [in] BSTR keyIdent);
};


// CSolimarLicenseSvr

[
	coclass,
	threading(free),
	support_error_info("ISolimarLicenseSvr"),
	vi_progid("SolimarLicenseServer.SolimarLicenseSvr"),
	progid("SolimarLicenseServer.SolimarLicenseSv.1"),
	version(1.0),
	uuid("4735A522-8C07-46D0-BA08-68BFF2B1B3A6"),
	helpstring("SolimarLicenseSvr Class")
]
class ATL_NO_VTABLE CSolimarLicenseSvr : 
	public ISolimarLicenseSvr4, 
	public ISolimarSoftwareLicenseSvr,
	public ISolimarConversionToSoftwareLicenseSvr,
	public IObjectAuthentication,
	public ILicensingMessage,
	public ISoftwareLicensingMessage,
	public ChallengeResponseHelper
{
public:

	CSolimarLicenseSvr();
	virtual ~CSolimarLicenseSvr();
	
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
//wchar_t debug_buf[1024];
//_snwprintf(debug_buf, 1024, L"CSolimarLicenseSvr::FinalConstruct() ThreadID: %d", GetCurrentThreadId());
//OutputDebugStringW(debug_buf);

		OLECHAR buffer[128];
		GUID guid;
		HRESULT hr = CoCreateGuid(&guid);
		if (FAILED(hr)) return hr;
		
		// translate GUID in to an OLESTR
		buffer[0]=0;
		StringFromGUID2(guid, buffer, 128);
		m_licenseId = buffer;

//wchar_t debug_buf[1024];
//_snwprintf(debug_buf, _countof(debug_buf), L"CSolimarLicenseSvr::FinalConstruct() m_licenseId: %s", (wchar_t*)m_licenseId);
//OutputDebugStringW(debug_buf);
		//keyserver.LicenseSessionInitialize(m_licenseId);

		//No longer start heart beat requirement until a key or license is obtained.
		//keyserver.Heartbeat(m_licenseId);
		
		return S_OK;
	}
	
	void FinalRelease() 
	{
//wchar_t debug_buf[1024];
//_snwprintf_s(debug_buf, 1024, L"CSolimarLicenseSvr::FinalRelease() m_licenseId: %s", (wchar_t*)m_licenseId);
//OutputDebugStringW(debug_buf);
	}
public:
	// IObjectAuthentication
	STDMETHOD(Challenge)(VARIANT vtChallenge, VARIANT *pvtResponse);
	STDMETHOD(GetChallenge)(VARIANT *pvtChallenge);
	STDMETHOD(PutResponse)(VARIANT vtResponse);
	
	// ISolimarLicenseMgr
	// Top level functions
	STDMETHOD(AddApplicationInstance)(BSTR key_ident, BSTR application_instance, VARIANT_BOOL b_app_instance_lock_key);
	STDMETHOD(RemoveApplicationInstance)(BSTR key_ident, BSTR application_instance);
	STDMETHOD(GetApplicationInstanceList)(BSTR key_ident, VARIANT *pvtAppInstanceList);

	STDMETHOD(GetVersionLicenseServer) (long* p_ver_major, long* p_ver_minor, long* p_ver_build);
	STDMETHOD(Heartbeat)();	//Any function that obtains or checks out licensing function needs to maintain a heart beat
	STDMETHOD(KeyEnumerate)(VARIANT *keylist);
	STDMETHOD(EnterPassword)(BSTR password);
	STDMETHOD(EnterPasswordPacket)(VARIANT vtPasswordPacket, BSTR *verification_code);
	STDMETHOD(GenerateBasePassword)(long customer_number, long key_number, BSTR *password);
	STDMETHOD(GenerateApplicationInstancePassword)(long customer_number, long key_number, long license_count, long password_number, BSTR *password);
	STDMETHOD(GenerateVersionPassword)(long customer_number, long key_number, long ver_major, long ver_minor, BSTR *password);
	STDMETHOD(GenerateExtensionPassword)(long customer_number, long key_number, long extend_days, long extension_num, BSTR *password);
	STDMETHOD(GenerateModulePassword)(long customer_number, long key_number, long product_ident, long module_ident, long license_count, BSTR *password);
	STDMETHOD(GetLicenseServerTime)(VARIANT *pvtSystemTime);
	STDMETHOD(GenerateModulePassword2)(long customer_number, long key_number, long product_ident, long module_ident, long license_count, long password_number, BSTR *password);
	
	// Password packet management
	STDMETHOD(PasswordPacketInitialize)();
	STDMETHOD(PasswordPacketAppendPassword)(VARIANT vtExpires, BSTR password);
	STDMETHOD(PasswordPacketFinalize)();
	STDMETHOD(PasswordPacketGetPacket)(VARIANT *pvtPacketData);
	STDMETHOD(PasswordPacketGetVerificationCode)(BSTR *verification_code);

	// Key specific functions
	STDMETHOD(KeyTrialExpires)(BSTR key_ident, VARIANT *expire_date);
	STDMETHOD(KeyTrialHours)(BSTR key_ident, long *trial_hours);
	STDMETHOD(KeyIsActive)(BSTR key_ident, VARIANT_BOOL *key_active);
	STDMETHOD(KeyIsProgrammed)(BSTR key_ident, VARIANT_BOOL *key_programmed);
	STDMETHOD(KeyHeaderQuery)(BSTR key_ident, long header_ident, VARIANT *value);
	STDMETHOD(KeyIsPresent)(BSTR key_ident, VARIANT_BOOL *key_present);
	
	// License locking support
	// locking a key prevents it from being used by others until it is unlocked
	STDMETHOD(KeyLock)(BSTR key_ident);
	STDMETHOD(KeyUnlock)(BSTR key_ident);
	// indicates that if the key is trial, to start using up hours until it is released
	STDMETHOD(KeyObtain)(BSTR key_ident);
	STDMETHOD(KeyRelease)(BSTR key_ident);
	
	// Module information and license locking
	STDMETHOD(KeyValidateLicense)(BSTR key_ident, VARIANT_BOOL *license_valid);
	STDMETHOD(KeyModuleEnumerate)(BSTR key_ident, VARIANT *key_module_list);
	STDMETHOD(KeyModuleQuery)(BSTR key_ident, long module_ident, VARIANT *vtValue);
	STDMETHOD(KeyModuleLicenseTotal)(BSTR key_ident, long module_ident, long* license_count);
	STDMETHOD(KeyModuleLicenseInUse)(BSTR key_ident, long module_ident, long* license_count);
	STDMETHOD(KeyModuleLicenseInUse_ByApp)(BSTR key_ident, long module_ident, long* license_count);
	STDMETHOD(KeyModuleLicenseObtain)(BSTR key_ident, long module_ident, long license_count);
	STDMETHOD(KeyModuleLicenseRelease)(BSTR key_ident, long module_ident, long license_count);
	STDMETHOD(KeyModuleLicenseCounterDecrement)(BSTR key_ident, long module_ident, long license_count);
	STDMETHOD(KeyModuleLicenseInUse2)(BSTR key_ident, long module_ident, long* license_count);
	STDMETHOD(KeyModuleInUse)(BSTR key_ident, long module_ident, long* license_count);
	STDMETHOD(KeyModuleLicenseUnlimited)(BSTR key_ident, long module_ident, VARIANT_BOOL b_module_is_unlimited);
	
	// Sets all writable cells on a key to zero
	STDMETHOD(KeyFormat)(BSTR key_ident, BSTR *new_key_ident);
	// Programs a key
	STDMETHOD(KeyProgram)(BSTR key_ident, long customer_number, long key_number, long product_ident, long ver_major, long ver_minor, long key_type, long days, VARIANT module_value_list, BSTR *new_key_ident);
	STDMETHOD(KeyProgram2)(BSTR key_ident, long customer_number, long key_number, long product_ident, long ver_major, long ver_minor, long key_type, long application_instances, long days, VARIANT module_value_list, BSTR *new_key_ident);
	// Reads raw data off of the key
	STDMETHOD(KeyReadRaw)(BSTR key_ident, VARIANT *pvtKeyData);

	// ILicensingMessage
	STDMETHOD(GetLicenseMessageList)(VARIANT_BOOL clear_messages, VARIANT *pvtMessageList);
	STDMETHOD(DispatchLicenseMessageList)(VARIANT_BOOL clear_messages);

	// ISoftwareLicensingMessage
	STDMETHOD(GetSoftwareLicenseMessageList)(VARIANT_BOOL clear_messages, VARIANT *pvtMessageList);
	STDMETHOD(DispatchSoftwareLicenseMessageList)(VARIANT_BOOL clear_messages);

	// ISolimarSoftwareLicenseSvr
	STDMETHOD(SoftwareModuleLicenseTotalForAll_ByProduct)(long productID, long moduleIdent, long* pLicenseCount);
	STDMETHOD(SoftwareModuleLicenseInUseForAll_ByProduct)(long productID, long moduleIdent, long* pLicenseCount);
	STDMETHOD(SoftwareValidateLicenseApp_ByProduct)(long productID, VARIANT_BOOL *pbLicenseValid);
	STDMETHOD(SoftwareModuleLicenseInUseByApp_ByProduct)(long productID, long moduleIdent, long* pLicenseCount);
	STDMETHOD(SoftwareModuleLicenseInUseByConnection_ByProduct)(long productID, long moduleIdent, long* pLicenseCount);
	STDMETHOD(SoftwareModuleLicenseObtainByApp_ByProduct)(long productID, long moduleIdent, long licenseCount);
	STDMETHOD(SoftwareModuleLicenseReleaseByApp_ByProduct)(long productID, long moduleIdent, long licenseCount);
	STDMETHOD(SoftwareModuleLicenseCounterDecrementByApp_ByProduct)(long productID, long moduleIdent, long licenseCount);

	STDMETHOD(GetSoftwareLicenseInfoByProduct_ForAll)(long productID, BSTR *pBstrProductInfoAttribsStream);
	STDMETHOD(GetSoftwareLicenseInfo_ForAll)(BSTR *pBstrLicenseInfoAttribsStream);
	
	STDMETHOD(GetAllSoftwareLicenses)(BSTR *pBstrLicenseListStream);
	STDMETHOD(GetSoftwareLicenseInfoByProduct_ByLicense)(BSTR bstrSoftwareLicense, long productID, BSTR *pBstrProductInfoAttribsStream);
	STDMETHOD(GetSoftwareLicenseInfo_ByLicense)(BSTR bstrSoftwareLicense, BSTR *pBstrLicenseInfoAttribsStream);

	STDMETHOD(GetSoftwareLicenseStatus_ByProduct)(long productID, BSTR *pBstrStringToDwordMap);
	STDMETHOD(GetSoftwareLicenseStatus_ByLicense)(BSTR softwareLicense);

	STDMETHOD(GetSoftwareSpecByProduct)(long productID, BSTR *pBstrProductSoftwareSpecAttribs);
	STDMETHOD(GetSoftwareSpec)(BSTR *pBstrSoftwareSpecAttribsStream);
	STDMETHOD(SoftwareAddApplicationInstanceByProduct)(long productID, BSTR applicationInstance);
	STDMETHOD(SoftwareRemoveApplicationInstanceByProduct)(long productID, BSTR applicationInstance);
	STDMETHOD(SoftwareGetApplicationInstanceListByProduct)(long productID, BSTR *pBstrListAppInstStream);
	
	STDMETHOD(GenerateSoftwareLicPacket)(BSTR bstrLicPackageAttribsStream, VARIANT vtExpires, BSTR *pBstrVerificationCode, VARIANT* pVtLicensePacket);
	STDMETHOD(EnterSoftwareLicPacket)(VARIANT vtLicensePacket, BSTR *pBstrVerificationCode);
	STDMETHOD(GenerateSoftwareLicArchive_ByLicense)(BSTR softwareLicense, VARIANT* pVtLicenseArchive);
	STDMETHOD(EnterSoftwareLicArchive)(VARIANT vtLicenseArchive);
	STDMETHOD(GenerateVerifyDataWithVerCode_ByLicense)(BSTR softwareLicense, VARIANT* pVtLicensePacket);
	STDMETHOD(GenerateVerifyDataWithLicInfo_ByLicense)(BSTR softwareLicense, VARIANT* pVtLicensePacket);
	STDMETHOD(GenerateLicPackage_ByVerifyData)(VARIANT vtVerifyData, BSTR *pBstrLicensePackageAttribsStream);
	STDMETHOD(GenerateLicPackage_BySoftwareLicArchive)(VARIANT vtLicenseArchive, BSTR *pBstrLicensePackageAttribsStream);
	STDMETHOD(GenerateLicPackage_BySoftwareLicPacket)(VARIANT vtLicensePacket, BSTR *pBstrLicensePackageAttribsStream);
	STDMETHOD(GenerateLicenseSystemData)(VARIANT* pVtLicSysDataPacket);
	STDMETHOD(GenerateStream_ByLicenseSystemData)(VARIANT vtLicSysDataPacket, BSTR *pBstrLicSysDataAttribsStream);	// Only for Interal License Servers

	STDMETHOD(ValidateToken_ByLicense)(BSTR softwareLicense, long validationTokenType, BSTR validationValue);

	STDMETHOD(SoftwareLicenseUseActivationToExtendTime_ByLicense)(BSTR softwareLicense);

	// ISolimarConversionToSoftwareLicenseSvr
	//if softwareLicense is L"", will try to add to first license file it finds, if it can't find one then will create new license file.
	STDMETHOD(ConvertProtectionKeyToSoftwareLicense)(BSTR softwareLicense, BSTR keyIdent);
private:
	_bstr_t m_licenseId;
	static BYTE challenge_key_server_thisauthuser_public[];
	static BYTE challenge_key_server_userauththis_private[];
};

