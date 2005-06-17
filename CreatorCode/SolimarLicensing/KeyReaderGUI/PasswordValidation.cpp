#include "stdafx.h"
#include "PasswordValidation.h"

using namespace PWDValidation;

PasswordValidation::PasswordValidation()
{
	TheCommLink = new CommunicationLink();
	TheCommLink->Connect();
}

PasswordValidation::~PasswordValidation()
{
	if(TheCommLink)
	{
		delete TheCommLink;
		TheCommLink = NULL;
	}
}

HRESULT PasswordValidation::ProcessPasswordFile(BSTR TheFileName)
{
			
	HRESULT hr;
	
	// open the password packet file
	HANDLE hFile = CreateFileW(TheFileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	hr = HRESULT_FROM_WIN32(::GetLastError());
	DWORD file_size = GetFileSize(hFile, 0);
	
	// create a variant safearray
	VARIANT vtPacket;
	VariantInit(&vtPacket);
	vtPacket.vt = VT_ARRAY | VT_UI1;
	vtPacket.parray = SafeArrayCreateVector(VT_UI1, 0, file_size);

	// access the safearray
	BYTE *pData = 0;
	if (SUCCEEDED(hr = SafeArrayAccessData(vtPacket.parray, (void**)&pData)))
	{
		// read the file in to the safearray
		DWORD bytes_read(0);
		if (!ReadFile(hFile, pData, file_size, &bytes_read, 0))
		{
			hr = HRESULT_FROM_WIN32(::GetLastError());
		}
		SafeArrayUnaccessData(vtPacket.parray);
		pData = 0;
	}
	else
	{
	#undef MessageBox
		MessageBox::Show("Error Accessing File");
		return hr;
	}

	BSTR verification_code;
	
	hr = TheCommLink->EnterPasswordPacket(&vtPacket, &verification_code);

	if(hr == 0x80040213)
	{
		MessageBox::Show("Error: Expired Packet");
		return hr;
	}

	else if(hr == S_FALSE)
	{
		MessageBox::Show("Error: Invalid Password");
		return hr;
	}
	else if(FAILED(hr))
	{
		MessageBox::Show("Error Communicating With SolimarLicenseServer");
	}
	wchar_t buf[256];
	swprintf(buf, L"The Verification Code Is : %s", verification_code);
	MessageBox::Show(buf);

	// free up the variant safearray
	VariantClear(&vtPacket);
	SysFreeString(verification_code);
	
	return hr;
}

HRESULT PasswordValidation::CheckPassword(String* InputString)
{
	return TheCommLink->CheckPassword(InputString);
}
