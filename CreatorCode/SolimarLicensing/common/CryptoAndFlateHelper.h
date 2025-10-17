#pragma once

#include "CryptoHelper.h"
#include "FlateHelper.h"
#include <comutil.h>	//For _bstr_t


class CryptoAndFlateHelper
{
public:
	static HRESULT StreamToEncryptCompressByteArray(BSTR bstrLicCode, const BYTE *pByteListHashCode, const DWORD byteListHashCodeLength, BYTE *pByteListEncrypKey, DWORD byteListEncrypKeyLength, BSTR bstrStream, VARIANT* pVtByteArray);
	static HRESULT EncryptCompressByteArrayToStream(BSTR bstrLicCode, const BYTE *pByteListHashCode, const DWORD byteListHashCodeLength, BYTE *pByteListEncrypKey, DWORD byteListEncrypKeyLength, VARIANT vtByteArray, BSTR *pBstrStream, bool bExpectDateInByteArray=false);
private:
	
};
