// ISpdRecorder.h : Declaration of the ISpdRecorder

#pragma once
#include "resource.h"       // main symbols

//#ifndef UI_IGNORE
enum
{
	UI_IGNORE =				0x00,
	UI_LEVEL_ALL =			0x01,
	UI_LEVEL_RESPONSE =		0x02,
	UI_LEVEL_CRITICAL =		0x04,
	UI_STYLE_DIALOG =		0x10,
	UI_STYLE_EVENT_LOG =	0x20,
};

enum 
{
	CF_NONE =	0x00,
	CF_BACKUP_SERVER = 0x01,
	CF_ONLY_SHARED_LICENSE = 0x02,
	CF_SOFTWARE_LICENSING = 0x04,
	CF_ONLY_LICENSE_VIEWER = 0x08,
};

//#endif

#ifndef __ISolimarLicenseMgr_INTERFACE_DEFINED__
#define __ISolimarLicenseMgr_INTERFACE_DEFINED__





// ISolimarLicenseMgr
[
	object,
	uuid("E76FF720-125E-4FE3-AAF4-05E8CBF6040A"),
	dual,	helpstring("ISolimarLicenseMgr Interface"),
	pointer_default(unique)
]

__interface ISolimarLicenseMgr : IDispatch
{
	// Connects to a key monitor server.
	// Can be called multiple times in succession to connect to and aggregate multiple key servers
    [id(1),helpstring("method Connect")] HRESULT Connect([in] BSTR server);
	// Disconnects from all servers, and releases all licenses
	[id(2),helpstring("method Disconnect")] HRESULT Disconnect();
	
	// Sets the product and product version required. Call this function after calling Connect()
	[id(3),helpstring("method Initialize")] HRESULT Initialize([in] long product, [in] long prod_ver_major, [in] long prod_ver_minor, [in] VARIANT_BOOL single_key, [in] BSTR specific_single_key_ident, [in] VARIANT_BOOL lock_keys, [in] long auto_ui_level);	
	// Checks whether the licenses obtained so far are still valid
	[id(4),helpstring("method ValidateLicense")] HRESULT ValidateLicense([out,retval] VARIANT_BOOL *license_valid);
	
	// Licensing info
	[id(6),helpstring("method ModuleLicenseTotal")] HRESULT ModuleLicenseTotal([in] long module_id, [out,retval] long *count);
	[id(7),helpstring("method ModuleLicenseInUse")] HRESULT ModuleLicenseInUse([in] long module_id, [out,retval] long *count);
	[id(8),helpstring("method ModuleLicenseObtain")] HRESULT ModuleLicenseObtain([in] long module_id, [in] long count);
	[id(9),helpstring("method ModuleLicenseRelease")] HRESULT ModuleLicenseRelease([in] long module_id, [in] long count);
	
};

// ISolimarLicenseMgr2
[
	object,
	uuid("EA2ADA7A-67BB-45f6-AF57-40C4E2CE8595"),
	dual,	helpstring("ISolimarLicenseMgr2 Interface"),
	pointer_default(unique)
]
__interface ISolimarLicenseMgr2 : ISolimarLicenseMgr
{
	[id(10),helpstring("method ModuleLicenseSerialNumbers")] HRESULT ModuleLicenseSerialNumbers([in] long module_id, [out,retval] VARIANT *pvtSerialNumberList);
};

// ISolimarLicenseMgr3
[
	object,
	uuid("CF8B391B-286E-4d72-9087-C8720C32D25C"),
	dual,	helpstring("ISolimarLicenseMgr3 Interface"),
	pointer_default(unique)
]
__interface ISolimarLicenseMgr3 : ISolimarLicenseMgr2
{
	[id(11),helpstring("method ModuleLicenseCounterDecrement")] HRESULT ModuleLicenseCounterDecrement([in] long module_id, [in] long license_count);
};


// ISolimarLicenseMgr4
[
	object,
	uuid("3DD3F19F-B523-4c1b-9AC3-0840C90B78E3"),
	dual,	helpstring("ISolimarLicenseMgr4 Interface"),
	pointer_default(unique)
]
__interface ISolimarLicenseMgr4 : ISolimarLicenseMgr3
{
	// Sets the product and product version required. Call this function after calling Connect()
	[id(12),helpstring("method Initialize2")] HRESULT Initialize2([in] long product, [in] long prod_ver_major, [in] long prod_ver_minor, [in] VARIANT_BOOL single_key, [in] BSTR specific_single_key_ident, [in] VARIANT_BOOL lock_keys, [in] long auto_ui_level, [in] unsigned long grace_period_minutes);
};

// ISolimarLicenseMgr5
[
	object,
	uuid("9A0AF885-9C33-4f42-BFAA-648400429209"),
	dual,	helpstring("ISolimarLicenseMgr5 Interface"),
	pointer_default(unique)
]
__interface ISolimarLicenseMgr5 : ISolimarLicenseMgr4
{
	// Connects to a key monitor server.
	// Can be called multiple times in succession to connect to and aggregate multiple key servers
	//	if bUseOnlySharedLicenses is set to true, will only use modules with the property SharedUse set.
	[id(13),helpstring("method Connect2")] HRESULT Connect2([in] BSTR server, [in] VARIANT_BOOL use_only_shared_licenses, [in] VARIANT_BOOL use_as_backup);
	[id(14),helpstring("method Initialize3")] HRESULT Initialize3([in] BSTR application_instance, [in] long product, [in] long prod_ver_major, [in] long prod_ver_minor, [in] VARIANT_BOOL single_key, [in] BSTR specific_single_key_ident, [in] VARIANT_BOOL lock_keys, [in] long auto_ui_level, [in] unsigned long grace_period_minutes, [in] VARIANT_BOOL app_instance_lock_key, [in] VARIANT_BOOL b_bypass_remote_key_restriction);
	[id(15),helpstring("method GetVersionLicenseManager")] HRESULT GetVersionLicenseManager([out] long* p_ver_major, [out] long* p_ver_minor, [out] long* p_ver_build);
	[id(16),helpstring("method GetVersionLicenseServer")] HRESULT GetVersionLicenseServer([in] BSTR server, [out] long* p_ver_major, [out] long* p_ver_minor, [out] long* p_ver_build);
	[id(17),helpstring("method ConnectByProduct")] HRESULT ConnectByProduct([in] long product, [in] VARIANT_BOOL use_shared_licenses_servers);
	[id(18),helpstring("method KeyProductExists")] HRESULT KeyProductExists([in] long product, [in] long prod_ver_major, [in] long prod_ver_minor, [out] VARIANT_BOOL *p_bool_key_product_exists);

};

// ISolimarLicenseMgr6
[
	object,
	uuid("39B8A524-0193-4bf3-BF83-8728413B0DA7"),
	dual,	helpstring("ISolimarLicenseMgr6 Interface"),
	pointer_default(unique)
]
__interface ISolimarLicenseMgr6 : ISolimarLicenseMgr5
{
	[id(19),helpstring("method ModuleLicenseInUse_ByApp")] HRESULT ModuleLicenseInUse_ByApp([in] long module_id, [out,retval] long *count);
};

// ISolimarLicenseMgr7
[
	object,
	uuid("60D07071-CE52-462b-B457-C22A5D23F66B"),
	dual,	helpstring("ISolimarLicenseMgr7 Interface"),
	pointer_default(unique)
]
__interface ISolimarLicenseMgr7 : ISolimarLicenseMgr6
{
	// Methods for Software license server
	[id(20),helpstring("method Connect3")] HRESULT Connect3([in] BSTR server, [in] long connectionFlags);
	[id(21),helpstring("method SoftwareLicense_Initialize")] HRESULT SoftwareLicense_Initialize([in] BSTR application_instance, [in] long product, [in] long prod_ver_major, [in] long prod_ver_minor, [in] long auto_ui_level, [in] unsigned long grace_period_minutes);
	[id(22),helpstring("method SoftwareLicense_InitializeViewOnly")] HRESULT SoftwareLicense_InitializeViewOnly([in] BSTR application_instance, [in] long product, [in] long prod_ver_major, [in] long prod_ver_minor);
	[id(23),helpstring("method GetInfoByProduct")] HRESULT GetInfoByProduct([in] long product, [in] VARIANT_BOOL use_shared_licenses_servers, [out] BSTR* p_server, [out] BSTR* p_backup_server, [out] VARIANT_BOOL* p_bool_use_test_dev_licensing);
};

// ISolimarLicenseMgr8
[
	object,
	uuid("A575B758-4C33-4DF4-A110-F8BB712AB9ED"),
	dual, helpstring("ISolimarLicenseMgr8 Interface"),
	pointer_default(unique)
]
__interface ISolimarLicenseMgr8 : ISolimarLicenseMgr7
{
	[id(24), helpstring("method InitializeAuthInfo")] HRESULT InitializeAuthInfo([in] BSTR domain, [in] BSTR username, [in] BSTR password, [in] long authenticationLevel, [in] long impersonationLevel);
};

#endif
