// SolimarLicenseServer.cpp : Implementation of WinMain

#include "stdafx.h"
#include "resource.h"

// The module attribute causes WinMain to be automatically implemented for you
[ module(SERVICE, uuid = "{F73FBF52-C028-483A-9818-F895243B6E63}", 
		 name = "SolimarLicenseServer", 
		 helpstring = "SolimarLicenseServer 1.0 Type Library", 
		 resource_name="IDS_SERVICENAME") ]
class CSolimarLicenseServerModule
{
public:
	HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the appropriate security settings for 
		// your service
		// Suggested - PKT Level Authentication, 
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY 
		// and an appropiate Non NULL Security Descriptor.
		HRESULT hr = S_OK;
		
		// if running from the debugger, it seems that calling CoInitializeSecurity 
		// causes the service to shut down with error 1063. When debugging, a command
		// line parameter is passed which skips the call.
		if (!wcsstr(GetCommandLineW(),L"-debugger"))
		{
			hr = CoInitializeSecurity(
				NULL,
				-1,
				NULL, 
				NULL,
				RPC_C_AUTHN_LEVEL_NONE,
				RPC_C_IMP_LEVEL_IMPERSONATE,
				NULL,
				EOAC_NONE,
				0);   //Reserved
			if (FAILED(hr))
			{
				OutputDebugStringW(L"CSolimarLicenseServerModule::InitializeSecurity() CoInitializeSecurity fails");
			}
		}

		return hr;
	}
};
