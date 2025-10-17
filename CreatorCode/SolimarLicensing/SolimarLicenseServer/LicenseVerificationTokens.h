#pragma once
#include <comdef.h>


enum eVerificationType
{
	evt_None = 0,
	evt_ProtectionKeyID = 1,
	evt_ProtectionKeyCode = 2,
	evt_ProcessorID = 3,
	evt_MacAddress = 4,
};

class LicenseVerificationToken
{
	public:
		eVerificationType verificationType;
		BSTR verificationValue;
};
