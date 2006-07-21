// ISpdRecorder.h : Declaration of the ISpdRecorder

#pragma once
#include "resource.h"       // main symbols

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

#endif
