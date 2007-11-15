#include "stdafx.h"
#include "PasswordValidation.h"
#include "..\common\LicenseError.h"

using namespace PWDValidation;

PasswordValidation::PasswordValidation(CommunicationLink* CommLink)
{
	TheCommLink = CommLink;
}

PasswordValidation::~PasswordValidation()
{
	if(TheCommLink)	
		TheCommLink = NULL;	
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
	if(hr == LicenseServerError::EHR_PACKET_EXPIRED)
	{
		MessageBox::Show("Expired Packet", "Password Packet Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return hr;
	}
	else if(hr == S_FALSE)
	{
		MessageBox::Show("Invalid Password Found in Packet", "Password Packet Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return hr;
	}
	else if(FAILED(hr))
	{
		MessageBox::Show("Unknown Error", "Password Packet Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	wchar_t buf[256];
	swprintf_s(buf, 
			   sizeof(buf)/sizeof(wchar_t), 
			   L"The Verification Code Is : %s", 
			   verification_code);
	MessageBox::Show(buf);
	//need to only write to eventlog on remote machine
	swprintf_s(buf, 
		       sizeof(buf)/sizeof(wchar_t), 
			   L"Solimar Systems, Inc.\r\nProduct Licensing Status Message\r\n\r\nPacket :\r\n%s\r\n\r\nPassword Packet Correct. Verification Code : %s.", 
			   TheFileName,
			   verification_code);				
	TheCommLink->WriteEventLog(buf, EventLogEntryType::Information); 
	// free up the variant safearray
	VariantClear(&vtPacket);
	SysFreeString(verification_code);
	
	return hr;
}

HRESULT PasswordValidation::CheckPassword(String* InputString)
{
	return TheCommLink->CheckPassword(InputString);
}
