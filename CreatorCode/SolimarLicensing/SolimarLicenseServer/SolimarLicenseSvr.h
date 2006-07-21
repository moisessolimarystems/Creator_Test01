// SolimarLicenseSvr.h : Declaration of the CSolimarLicenseSvr

#pragma once

#include "stdafx.h"
#include "resource.h"       // main symbols
#include "..\common\IObjectAuthentication.h"
#include "..\common\ILicensingMessage.h"
#include "..\common\ChallengeResponseHelper.h"
#include "KeyServer.h"
#include "KeyServerInstance.h"
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



// CSolimarLicenseSvr

[
	coclass,
	threading("free"),
	support_error_info("ISolimarLicenseSvr"),
	vi_progid("SolimarLicenseServer.SolimarLicenseSvr"),
	progid("SolimarLicenseServer.SolimarLicenseSv.1"),
	version(1.0),
	uuid("4735A522-8C07-46D0-BA08-68BFF2B1B3A6"),
	helpstring("SolimarLicenseSvr Class")
]
class ATL_NO_VTABLE CSolimarLicenseSvr : 
	public ISolimarLicenseSvr, 
	public IObjectAuthentication,
	public ILicensingMessage,
	public ChallengeResponseHelper
{
public:

	CSolimarLicenseSvr();
	virtual ~CSolimarLicenseSvr();
	
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		OLECHAR buffer[128];
		GUID guid;
		HRESULT hr = CoCreateGuid(&guid);
		if (FAILED(hr)) return hr;
		
		// translate GUID in to an OLESTR
		buffer[0]=0;
		StringFromGUID2(guid, buffer, 128);
		m_licenseId = buffer;
		
		keyserver.LicenseSessionInitialize(m_licenseId);
		keyserver.Heartbeat(m_licenseId);
		
		return S_OK;
	}
	
	void FinalRelease() 
	{
		keyserver.LicenseSessionUninitialize(m_licenseId);
	}

public:
	// IObjectAuthentication
	STDMETHOD(Challenge)(VARIANT vtChallenge, VARIANT *pvtResponse);
	STDMETHOD(GetChallenge)(VARIANT *pvtChallenge);
	STDMETHOD(PutResponse)(VARIANT vtResponse);
	
	// ISolimarLicenseMgr
	// Top level functions
	STDMETHOD(Heartbeat)();
	STDMETHOD(KeyEnumerate)(VARIANT *keylist);
	STDMETHOD(EnterPassword)(BSTR password);
	STDMETHOD(EnterPasswordPacket)(VARIANT vtPasswordPacket, BSTR *verification_code);
	STDMETHOD(GenerateBasePassword)(long customer_number, long key_number, BSTR *password);
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
	STDMETHOD(KeyModuleLicenseObtain)(BSTR key_ident, long module_ident, long license_count);
	STDMETHOD(KeyModuleLicenseRelease)(BSTR key_ident, long module_ident, long license_count);
	STDMETHOD(KeyModuleLicenseCounterDecrement)(BSTR key_ident, long module_ident, long license_count);
	
	// Sets all writable cells on a key to zero
	STDMETHOD(KeyFormat)(BSTR key_ident, BSTR *new_key_ident);
	// Programs a key
	STDMETHOD(KeyProgram)(BSTR key_ident, long customer_number, long key_number, long product_ident, long ver_major, long ver_minor, long key_type, long days, VARIANT module_value_list, BSTR *new_key_ident);
	// Reads raw data off of the key
	STDMETHOD(KeyReadRaw)(BSTR key_ident, VARIANT *pvtKeyData);

	// ILicensingMessage
	STDMETHOD(GetLicenseMessageList)(VARIANT_BOOL clear_messages, VARIANT *pvtMessageList);
	STDMETHOD(DispatchLicenseMessageList)(VARIANT_BOOL clear_messages);
	
	
private:
	_bstr_t m_licenseId;
	static BYTE challenge_key_server_thisauthuser_public[];
	static BYTE challenge_key_server_userauththis_private[];
};

