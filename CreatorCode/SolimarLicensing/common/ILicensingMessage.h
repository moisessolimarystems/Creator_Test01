// ILicensingMessage.h : Declaration of the ILicensingMessage interface

/* This interface is used to facilitate verifying the integrity and authenticity of the code behind a COM proxy */

#pragma once
#include "resource.h"       // main symbols
#include "InProcPtr.h"

// IObjectAuthentication
[
	object,
	uuid("025336D2-0EF1-4dab-9126-E8ACD750FFE5"),
	dual,	helpstring("IObjectAuthentication Interface"),
	pointer_default(unique)
]
__interface ILicensingMessage : IDispatch
{
	// Message/Event support
	[id(3),helpstring("method GetLicenseMessageList")] HRESULT GetLicenseMessageList([out,retval] VARIANT *pvtMessageList);
};
