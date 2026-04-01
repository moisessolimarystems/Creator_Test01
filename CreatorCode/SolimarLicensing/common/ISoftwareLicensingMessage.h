#pragma once
#ifndef __ISoftwareLicensingMessage_Header__
#define __ISoftwareLicensingMessage_Header__

#include "resource.h"       // main symbols
#include "InProcPtr.h"


// ISoftwareLicensingMessage
[
	object,
	uuid("817EA560-B5F3-4f63-9AD5-3AE239849C68"),
	dual,	helpstring("ISoftwareLicensingMessage Interface"),
	pointer_default(unique)
]
__interface ISoftwareLicensingMessage : IDispatch
{
	// Software License Message/Event support
	[id(4),helpstring("method GetSoftwareLicenseMessageList")] HRESULT GetSoftwareLicenseMessageList([in] VARIANT_BOOL clear_messages, [out,retval] VARIANT *pvtMessageList);
	[id(5),helpstring("method DispatchSoftwareLicenseMessageList")] HRESULT DispatchSoftwareLicenseMessageList([in] VARIANT_BOOL clear_messages);
};


#endif