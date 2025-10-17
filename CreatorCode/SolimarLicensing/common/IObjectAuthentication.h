// IObjectAuthentication.h : Declaration of the IObjectAuthentication interface

/* This interface is used to facilitate verifying the integrity and authenticity of the code behind a COM proxy */

#pragma once

#include "resource.h"       // main symbols

// IObjectAuthentication
[
	object,
	uuid("779FE092-038A-43c2-8F99-5F4974D14997"),
	dual,	helpstring("IObjectAuthentication Interface"),
	pointer_default(unique)
]
__interface IObjectAuthentication : IDispatch
{
	// Client authenticates Server.
	[id(1),helpstring("method Challenge")] HRESULT Challenge([in] VARIANT vtChallenge, [out,retval] VARIANT *pvtResponse);
	// Server authenticates Client.
	[id(2),helpstring("method GetChallenge")] HRESULT GetChallenge([out,retval] VARIANT *pvtChallenge);
	[id(3),helpstring("method PutResponse")] HRESULT PutResponse([in] VARIANT vtResponse);
};
