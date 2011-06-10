#include "SolimarSoftwareLicenseMgr.h"
#include "..\common\NetworkUtils.h"	//For NetworkUtils::SolimarNetworkHelper::GetIPAddresses
#include "..\common\WMIHelper.h"		//For WMIHelper
#include "..\common\LicenseError.h"
#include "ProtectionKey.h"
#include "KeySpec.h"	//For Global keyspec
#include <time.h>			//For time & difftime
#include <wchar.h>		//For _wchmod
#include <math.h>			//For abs

#include "..\common\TimeHelper.h"


#include "..\common\LicAttribsCPP\Lic_PackageAttribs.h"
#include "..\common\LicAttribsCPP\Lic_PackageAttribsHelper.h"

SoftwareLicenseMgr::SoftwareLicenseMgr():
	softwareLicenseMgrLock(CreateMutex(0,0,0)),
	m_bstrLicenseName(L""),
	m_lastTimeCheck(0),
	m_pKeyServer(NULL)
{
	;
}

SoftwareLicenseMgr::~SoftwareLicenseMgr()
{
	if (softwareLicenseMgrLock!=NULL)
	{
		CloseHandle(softwareLicenseMgrLock);
		softwareLicenseMgrLock = NULL;
	}
}

HRESULT SoftwareLicenseMgr::Initialize(_bstr_t bstrLicenseFile, KeyServer* pKeyServer, SoftwareServerDataMgr* pLicServerDataMgr)
{
	SafeMutex mutex(softwareLicenseMgrLock);
	m_bstrLicenseFile = bstrLicenseFile;
//wchar_t debug_buf[1024];
//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::Initialize() m_bstrLicenseFile: %s", (wchar_t*)m_bstrLicenseFile);
//OutputDebugStringW(debug_buf);
	m_pKeyServer = pKeyServer;
	m_pLicServerDataMgr = pLicServerDataMgr;
	return RefreshLicenseFileAttribs();
}

HRESULT SoftwareLicenseMgr::Verify(bool* pBValid, bool bForceRefresh)
{
	HRESULT hr(S_OK);

	SafeMutex mutex(softwareLicenseMgrLock);

	//maybe only really look in verify if it is dirty, or some time period has
	//passed, such as 5 minutes if last verify returned success...

//wchar_t debug_buf[1024];
//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::Verify()");
//OutputDebugStringW(debug_buf);

	if(bForceRefresh)	
		m_lastTimeCheck = 0;	//Force a refresh

	time_t cur_time = time(NULL);
	static const time_t UPDATE_TIME_PERIOD_SECONDS = (time_t)(5*60);//5 minutes - Don't check more than once every 5 minutes.
	if(m_lastTimeCheck == 0 || abs(difftime(cur_time, m_lastTimeCheck)) > UPDATE_TIME_PERIOD_SECONDS)
	{
//wchar_t debug_buf[1024];		
//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::Verify() - call RefreshLicenseFileAttribs()");
//OutputDebugStringW(debug_buf);
		try
		{
			hr = RefreshLicenseFileAttribs();
			if(SUCCEEDED(hr))
			{
				//maybe only really look in file if it is dirty, or some time period has
				//passed, such as 5 minutes

				//foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs verToken in param_verificationAttribs.validationTokenList.TVal)
				//this maybe the best spot to do the verification that the tokens are all valid
				//Cycle through all the verification tokens, stop if any fail...
				for(	std::vector<Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs>::iterator valTokenIT = m_licenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->begin();
						valTokenIT != m_licenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->end();
						valTokenIT++)
				{
//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::Verify() - TokenType:%d, TokenValue:%s", (int)valTokenIT->tokenType, valTokenIT->tokenValue->c_str());
//OutputDebugStringW(debug_buf);
					switch((Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::TTokenType)valTokenIT->tokenType)
					{
						case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttHardwareKeyID:
							//OutputDebugStringW(L"    Validate - ttHardwareKeyID");
							hr = ValidateHardwareKeyID(_bstr_t(valTokenIT->tokenValue->c_str()));
							break;
						case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttLicenseCode:
							//OutputDebugStringW(L"    Validate - ttLicenseCode");
							hr = ValidateLicenseCode(_bstr_t(valTokenIT->tokenValue->c_str()));
							break;
						case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttBiosSerialNumber:
							//OutputDebugStringW(L"    Validate - ttBiosSerialNumber");
							hr = ValidateHardwareBiosSerialNumber(_bstr_t(valTokenIT->tokenValue->c_str()));
							break;
						case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttMacAddress:
							//OutputDebugStringW(L"    Validate - ttMacAddress");
							hr = ValidateHardwareMacAddress(_bstr_t(valTokenIT->tokenValue->c_str()));
							break;
						case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttComputerName:
							//OutputDebugStringW(L"    Validate - ttComputerName");
							hr = ValidateHardwareCompuerName(_bstr_t(valTokenIT->tokenValue->c_str()));
							break;
						case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttDomainName:
							//OutputDebugStringW(L"    Validate - ttDomainName");
							hr = ValidateHardwareDomainName(_bstr_t(valTokenIT->tokenValue->c_str()));
							break;
						case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttPartOfDomain:
							//OutputDebugStringW(L"    Validate - ttPartOfDomain");
							hr = ValidateHardwarePartOfDomain(_wcsicmp(valTokenIT->tokenValue->c_str(), L"true") == 0);
							break;
						case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttOperatingSystem:
							//OutputDebugStringW(L"    Validate - ttOperatingSystem");
							hr = ValidateHardwareOperatingSystem(_bstr_t(valTokenIT->tokenValue->c_str()));
							break;
						case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttSystemManufacturer:
							//OutputDebugStringW(L"    Validate - ttSystemManufacturer");
							hr = ValidateHardwareSystemManufacturer(_bstr_t(valTokenIT->tokenValue->c_str()));
							break;
						case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttSystemModel:
							//OutputDebugStringW(L"    Validate - ttSystemModel");
							hr = ValidateHardwareSystemModel(_bstr_t(valTokenIT->tokenValue->c_str()));
							break;
						case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttSystemType:
							//OutputDebugStringW(L"    Validate - ttSystemType");
							hr = ValidateHardwareSystemType(_bstr_t(valTokenIT->tokenValue->c_str()));
							break;
						case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttSystemUuid:
							//OutputDebugStringW(L"    Validate - ttSystemUuid");
							hr = ValidateHardwareSystemUUID(_bstr_t(valTokenIT->tokenValue->c_str()));
							break;
						default:
							//OutputDebugStringW(L"    Validate - Unknown");
							break;
					}

					//Break on any error...
					if(FAILED(hr))
						throw hr;
				}	
			}

		}
		catch(HRESULT &eHr)
		{
			hr = eHr;
		}
		catch (...)
		{
			hr = E_FAIL;
		}

//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::Verify() - hr: 0x%08x, %s", hr, LicenseServerError::GetErrorMessage(hr).c_str());
//OutputDebugStringW(debug_buf);

		if(FAILED(hr))
			m_lastTimeCheck = 0;	//Force a check next time...
		else
			m_lastTimeCheck = cur_time;
	}

//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::Verify() - leave hr: 0x%08x, %s", hr, LicenseServerError::GetErrorMessage(hr).c_str());
//OutputDebugStringW(debug_buf);
	return hr;
}

HRESULT SoftwareLicenseMgr::ValidateHardwareKeyID(_bstr_t bstrValidationValue)
{
	HRESULT hr(LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_KEY_ID);
	SafeMutex mutex(softwareLicenseMgrLock);
	if(m_pKeyServer != NULL)
	{
		Lic_KeyAttribs licKeyAttribs;
		hr = m_pKeyServer->GetKeyInfoAttribs(bstrValidationValue, &licKeyAttribs);
		if(hr == E_INVALIDARG)	//No Key attached
			hr = LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_KEY_ID;
	}
	return hr;
}
HRESULT SoftwareLicenseMgr::ValidateLicenseCode(_bstr_t bstrValidationValue, _bstr_t bstrKeyID)
{
	HRESULT hr(LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_LICENSE_CODE);
//wchar_t debug_buf[1024];
	SafeMutex mutex(softwareLicenseMgrLock);

	//Determine if the license code should be on the protection key, or on the system.
	bool bVerifyOnProtectionKey = _wcsicmp(bstrKeyID, L"") != 0;
	_bstr_t keyID = bstrKeyID;
	
	if(bVerifyOnProtectionKey == false)
	{
		for(	std::vector<Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs>::iterator valTokenIT = m_licenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->begin();
				valTokenIT != m_licenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->end();
				valTokenIT++)
		{
			if((Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::TTokenType)valTokenIT->tokenType == Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttHardwareKeyID)
			{
				keyID = _bstr_t(valTokenIT->tokenValue->c_str());
				bVerifyOnProtectionKey = true;
				break;
			}
		}
	}

	if(bVerifyOnProtectionKey)	//Look for license code on protection key
	{
		if(_wcsicmp(keyID, L"") == 0)
		{
			hr = LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_KEY_ID;
		}
		else if(m_pKeyServer != NULL)
		{
			Lic_KeyAttribs licKeyAttribs;
			hr = m_pKeyServer->GetKeyInfoAttribs(keyID, &licKeyAttribs);
			if(hr == E_INVALIDARG)	//No Key attached
				hr = LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_KEY_ID;
			else
			{
				if(_wcsicmp(bstrValidationValue, std::wstring(licKeyAttribs.licenseCode).c_str()) == 0)
					hr = S_OK;
				else
					hr = LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_LICENSE_CODE;
			}
		}
	}
	else	//Look for license code through m_pLicServerDataMgr
	{
		if(hr == LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_LICENSE_CODE)
		{
			Lic_ServerDataAttribs::Lic_ServerDataFileInfoAttribs tmpFileInfo;
			hr = m_pLicServerDataMgr->GetFileInfoFor(std::wstring(SpdAttribs::WStringObj(Lic_PackageAttribsHelper::GetDisplayLabel(&m_licenseFileAttribs.licLicenseInfoAttribs))).c_str(), &tmpFileInfo);
			if(SUCCEEDED(hr))
			{
				if(_wcsicmp(bstr_t(tmpFileInfo.LicFileLicenseCode->c_str()), bstrValidationValue) == 0)
					hr = S_OK;
				else
					hr = LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_LICENSE_CODE;
			}
		}
	}

	return hr;
}
//HRESULT SoftwareLicenseMgr::VerifyHardwareProcessorID(_bstr_t bstrValidationValue)
//{
//	HRESULT hr(LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_PROCESSOR_ID);
//	SafeMutex mutex(softwareLicenseMgrLock);
//
//	
//
//	return hr;
//}
HRESULT SoftwareLicenseMgr::ValidateHardwareBiosSerialNumber(_bstr_t bstrValidationValue)
{
//wchar_t debug_buf[1024];
//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::VerifyHardwareBiosSerialNumber() bstrValidationValue: %s", (wchar_t*)bstrValidationValue);
//OutputDebugStringW(debug_buf);

	// Hide the string so it doesn't appear in the strings of the image - "SELECT SerialNumber FROM Win32_BIOS"
	BYTE wmiQuery_Byte[] = {
		0x53, 0x00, 0x45, 0x00, 0x4c, 0x00, 0x45, 0x00, 0x43, 0x00, 0x54, 0x00, 0x20, 0x00, 0x53, 0x00, 0x65, 0x00, 0x72, 0x00, 0x69, 0x00, 0x61, 0x00, 0x6c, 0x00, 0x4e, 0x00, 0x75, 0x00, 0x6d, 0x00,
		0x62, 0x00, 0x65, 0x00, 0x72, 0x00, 0x20, 0x00, 0x46, 0x00, 0x52, 0x00, 0x4f, 0x00, 0x4d, 0x00, 0x20, 0x00, 0x57, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x33, 0x00, 0x32, 0x00, 0x5f, 0x00, 0x42, 0x00,
		0x49, 0x00, 0x4f, 0x00, 0x53, 0x00, 0x00, 0x00
	};
	// Hide the string so it doesn't appear in the strings of the image - "SerialNumber"
	BYTE fieldByte[] = {
		0x53, 0x00, 0x65, 0x00, 0x72, 0x00, 0x69, 0x00, 0x61, 0x00, 0x6c, 0x00, 0x4e, 0x00, 0x75, 0x00, 0x6d, 0x00, 0x62, 0x00, 0x65, 0x00, 0x72, 0x00, 0x00, 0x00
	};
	return WmiQueryContainsValue((wchar_t*)wmiQuery_Byte, (wchar_t*)fieldByte, bstrValidationValue);
}
HRESULT SoftwareLicenseMgr::ValidateHardwareMacAddress(_bstr_t bstrValidationValue)
{
	// Ignore Loopback Adapter, servicename='msloop' for loopback adapter
	// Hide the string so it doesn't appear in the strings of the image - SELECT MACAddress FROM Win32_NetworkAdapterConfiguration Where IPEnabled = True AND ServiceName <> 'msloop'
	BYTE wmiQuery_Byte[] = {
		0x53, 0x00, 0x45, 0x00, 0x4c, 0x00, 0x45, 0x00, 0x43, 0x00, 0x54, 0x00, 0x20, 0x00, 0x4d, 0x00, 0x41, 0x00, 0x43, 0x00, 0x41, 0x00, 0x64, 0x00, 0x64, 0x00, 0x72, 0x00, 0x65, 0x00, 0x73, 0x00,
		0x73, 0x00, 0x20, 0x00, 0x46, 0x00, 0x52, 0x00, 0x4f, 0x00, 0x4d, 0x00, 0x20, 0x00, 0x57, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x33, 0x00, 0x32, 0x00, 0x5f, 0x00, 0x4e, 0x00, 0x65, 0x00, 0x74, 0x00,
		0x77, 0x00, 0x6f, 0x00, 0x72, 0x00, 0x6b, 0x00, 0x41, 0x00, 0x64, 0x00, 0x61, 0x00, 0x70, 0x00, 0x74, 0x00, 0x65, 0x00, 0x72, 0x00, 0x43, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x66, 0x00, 0x69, 0x00,
		0x67, 0x00, 0x75, 0x00, 0x72, 0x00, 0x61, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x20, 0x00, 0x57, 0x00, 0x68, 0x00, 0x65, 0x00, 0x72, 0x00, 0x65, 0x00, 0x20, 0x00, 0x49, 0x00,
		0x50, 0x00, 0x45, 0x00, 0x6e, 0x00, 0x61, 0x00, 0x62, 0x00, 0x6c, 0x00, 0x65, 0x00, 0x64, 0x00, 0x20, 0x00, 0x3d, 0x00, 0x20, 0x00, 0x54, 0x00, 0x72, 0x00, 0x75, 0x00, 0x65, 0x00, 0x20, 0x00,
		0x41, 0x00, 0x4e, 0x00, 0x44, 0x00, 0x20, 0x00, 0x53, 0x00, 0x65, 0x00, 0x72, 0x00, 0x76, 0x00, 0x69, 0x00, 0x63, 0x00, 0x65, 0x00, 0x4e, 0x00, 0x61, 0x00, 0x6d, 0x00, 0x65, 0x00, 0x20, 0x00,
		0x3c, 0x00, 0x3e, 0x00, 0x20, 0x00, 0x27, 0x00, 0x6d, 0x00, 0x73, 0x00, 0x6c, 0x00, 0x6f, 0x00, 0x6f, 0x00, 0x70, 0x00, 0x27, 0x00, 0x00, 0x00
	};

	// Hide the string so it doesn't appear in the strings of the image - "MACAddress"
	BYTE fieldByte[] = {
			0x4d, 0x00, 0x41, 0x00, 0x43, 0x00, 0x41, 0x00, 0x64, 0x00, 0x64, 0x00, 0x72, 0x00, 0x65, 0x00, 0x73, 0x00, 0x73, 0x00, 0x00, 0x00
		};
	return WmiQueryContainsValue((wchar_t*)wmiQuery_Byte, (wchar_t*)fieldByte, bstrValidationValue);
}
HRESULT SoftwareLicenseMgr::ValidateHardwareCompuerName(_bstr_t bstrValidationValue)
{
	HRESULT hr(LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_GENERIC);
	SafeMutex mutex(softwareLicenseMgrLock);

	WCHAR localComputerName[1024];
	DWORD localComputerNameSize = 1024;

//wchar_t debug_buf[1024];
	bool bMatchedComputerName(false);
	if (GetComputerNameW(localComputerName, &localComputerNameSize))
	{
//swprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::VerifyHardwareCompuerName localComputerName: %s, verify: %s", (wchar_t*)localComputerName, (wchar_t*)bstrValidationValue);
//OutputDebugString(debug_buf);
		
		if(_wcsicmp(localComputerName, bstrValidationValue) == 0)
		{
			bMatchedComputerName = true;
			hr = S_OK;
		}

		if(!bMatchedComputerName)
		{
			//compare against IP addresses
			std::vector<_bstr_t> ipList;
			if(SUCCEEDED(NetworkUtils::SolimarNetworkHelper::GetIPAddresses(localComputerName, ipList)))
			{
				for(std::vector<_bstr_t>::iterator ipIt = ipList.begin(); ipIt != ipList.end(); ipIt++)
				{
//swprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::VerifyHardwareCompuerName ipAddress: %s", (wchar_t*)*ipIt);
//OutputDebugString(debug_buf);

					if(_wcsicmp(*ipIt, bstrValidationValue) == 0)
					{
						bMatchedComputerName = true;
						hr = S_OK;
					}
				}
			}
		}
	}
	

	return hr;
}


HRESULT SoftwareLicenseMgr::ValidateHardwareDomainName(_bstr_t bstrValidationValue)
{
	// Hide the string so it doesn't appear in the strings of the image - "SELECT Domain FROM Win32_ComputerSystem"
	BYTE wmiQuery_Byte[] = {
		0x53, 0x00, 0x45, 0x00, 0x4c, 0x00, 0x45, 0x00, 0x43, 0x00, 0x54, 0x00, 0x20, 0x00, 0x44, 0x00, 0x6f, 0x00, 0x6d, 0x00, 0x61, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x20, 0x00, 0x46, 0x00, 0x52, 0x00, 
		0x4f, 0x00, 0x4d, 0x00, 0x20, 0x00, 0x57, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x33, 0x00, 0x32, 0x00, 0x5f, 0x00, 0x43, 0x00, 0x6f, 0x00, 0x6d, 0x00, 0x70, 0x00, 0x75, 0x00, 0x74, 0x00, 0x65, 0x00,
		0x72, 0x00, 0x53, 0x00, 0x79, 0x00, 0x73, 0x00, 0x74, 0x00, 0x65, 0x00, 0x6d, 0x00, 0x00, 0x00
	};

	// Hide the string so it doesn't appear in the strings of the image - "Domain"
	BYTE fieldByte[] = {
			0x44, 0x00, 0x6f, 0x00, 0x6d, 0x00, 0x61, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x00, 0x00
		};
	return WmiQueryContainsValue((wchar_t*)wmiQuery_Byte, (wchar_t*)fieldByte, bstrValidationValue);
}
HRESULT SoftwareLicenseMgr::ValidateHardwareOperatingSystem(_bstr_t bstrValidationValue)
{
	// Hide the string so it doesn't appear in the strings of the image - "SELECT Caption FROM Win32_OperatingSystem"
	BYTE wmiQuery_Byte[] = {
		0x53, 0x00, 0x45, 0x00, 0x4c, 0x00, 0x45, 0x00, 0x43, 0x00, 0x54, 0x00, 0x20, 0x00, 0x43, 0x00, 0x61, 0x00, 0x70, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x20, 0x00, 0x46, 0x00,
		0x52, 0x00, 0x4f, 0x00, 0x4d, 0x00, 0x20, 0x00, 0x57, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x33, 0x00, 0x32, 0x00, 0x5f, 0x00, 0x4f, 0x00, 0x70, 0x00, 0x65, 0x00, 0x72, 0x00, 0x61, 0x00, 0x74, 0x00,
		0x69, 0x00, 0x6e, 0x00, 0x67, 0x00, 0x53, 0x00, 0x79, 0x00, 0x73, 0x00, 0x74, 0x00, 0x65, 0x00, 0x6d, 0x00, 0x00, 0x00
	};

	// Hide the string so it doesn't appear in the strings of the image - "Caption"
	BYTE fieldByte[] = {
		0x43, 0x00, 0x61, 0x00, 0x70, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x00, 0x00
	};
	return WmiQueryContainsValue((wchar_t*)wmiQuery_Byte, (wchar_t*)fieldByte, bstrValidationValue);
}
HRESULT SoftwareLicenseMgr::ValidateHardwarePartOfDomain(bool bValidationValue)
{
	// Hide the string so it doesn't appear in the strings of the image - "SELECT PartOfDomain FROM Win32_ComputerSystem"
	BYTE wmiQuery_Byte[] = {
		0x53, 0x00, 0x45, 0x00, 0x4c, 0x00, 0x45, 0x00, 0x43, 0x00, 0x54, 0x00, 0x20, 0x00, 0x50, 0x00, 0x61, 0x00, 0x72, 0x00, 0x74, 0x00, 0x4f, 0x00, 0x66, 0x00, 0x44, 0x00, 0x6f, 0x00, 0x6d, 0x00, 
		0x61, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x20, 0x00, 0x46, 0x00, 0x52, 0x00, 0x4f, 0x00, 0x4d, 0x00, 0x20, 0x00, 0x57, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x33, 0x00, 0x32, 0x00, 0x5f, 0x00, 0x43, 0x00, 
		0x6f, 0x00, 0x6d, 0x00, 0x70, 0x00, 0x75, 0x00, 0x74, 0x00, 0x65, 0x00, 0x72, 0x00, 0x53, 0x00, 0x79, 0x00, 0x73, 0x00, 0x74, 0x00, 0x65, 0x00, 0x6d, 0x00, 0x00, 0x00
	};

	// Hide the string so it doesn't appear in the strings of the image - "PartOfDomain"
	BYTE fieldByte[] = {
		0x50, 0x00, 0x61, 0x00, 0x72, 0x00, 0x74, 0x00, 0x4f, 0x00, 0x66, 0x00, 0x44, 0x00, 0x6f, 0x00, 0x6d, 0x00, 0x61, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x00, 0x00
	};
	return WmiQueryContainsValue((wchar_t*)wmiQuery_Byte, (wchar_t*)fieldByte, bValidationValue);
}
HRESULT SoftwareLicenseMgr::ValidateHardwareSystemManufacturer(_bstr_t bstrValidationValue)
{
	// Hide the string so it doesn't appear in the strings of the image - "SELECT Manufacturer FROM Win32_ComputerSystem"
	BYTE wmiQuery_Byte[] = {
		0x53, 0x00, 0x45, 0x00, 0x4c, 0x00, 0x45, 0x00, 0x43, 0x00, 0x54, 0x00, 0x20, 0x00, 0x4d, 0x00, 0x61, 0x00, 0x6e, 0x00, 0x75, 0x00, 0x66, 0x00, 0x61, 0x00, 0x63, 0x00, 0x74, 0x00, 0x75, 0x00, 
		0x72, 0x00, 0x65, 0x00, 0x72, 0x00, 0x20, 0x00, 0x46, 0x00, 0x52, 0x00, 0x4f, 0x00, 0x4d, 0x00, 0x20, 0x00, 0x57, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x33, 0x00, 0x32, 0x00, 0x5f, 0x00, 0x43, 0x00, 
		0x6f, 0x00, 0x6d, 0x00, 0x70, 0x00, 0x75, 0x00, 0x74, 0x00, 0x65, 0x00, 0x72, 0x00, 0x53, 0x00, 0x79, 0x00, 0x73, 0x00, 0x74, 0x00, 0x65, 0x00, 0x6d, 0x00, 0x00, 0x00
	};

	// Hide the string so it doesn't appear in the strings of the image - "Manufacturer"
	BYTE fieldByte[] = {
		0x4d, 0x00, 0x61, 0x00, 0x6e, 0x00, 0x75, 0x00, 0x66, 0x00, 0x61, 0x00, 0x63, 0x00, 0x74, 0x00, 0x75, 0x00, 0x72, 0x00, 0x65, 0x00, 0x72, 0x00, 0x00, 0x00
	};
	return WmiQueryContainsValue((wchar_t*)wmiQuery_Byte, (wchar_t*)fieldByte, bstrValidationValue);
}
HRESULT SoftwareLicenseMgr::ValidateHardwareSystemModel(_bstr_t bstrValidationValue)
{
	// Hide the string so it doesn't appear in the strings of the image - "SELECT Model FROM Win32_ComputerSystem"
	BYTE wmiQuery_Byte[] = {
		0x53, 0x00, 0x45, 0x00, 0x4c, 0x00, 0x45, 0x00, 0x43, 0x00, 0x54, 0x00, 0x20, 0x00, 0x4d, 0x00, 0x6f, 0x00, 0x64, 0x00, 0x65, 0x00, 0x6c, 0x00, 0x20, 0x00, 0x46, 0x00, 0x52, 0x00, 0x4f, 0x00,
		0x4d, 0x00, 0x20, 0x00, 0x57, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x33, 0x00, 0x32, 0x00, 0x5f, 0x00, 0x43, 0x00, 0x6f, 0x00, 0x6d, 0x00, 0x70, 0x00, 0x75, 0x00, 0x74, 0x00, 0x65, 0x00, 0x72, 0x00, 
		0x53, 0x00, 0x79, 0x00, 0x73, 0x00, 0x74, 0x00, 0x65, 0x00, 0x6d, 0x00, 0x00, 0x00
	};

	// Hide the string so it doesn't appear in the strings of the image - "Model"
	BYTE fieldByte[] = {
		0x4d, 0x00, 0x6f, 0x00, 0x64, 0x00, 0x65, 0x00, 0x6c, 0x00, 0x00, 0x00
	};
	return WmiQueryContainsValue((wchar_t*)wmiQuery_Byte, (wchar_t*)fieldByte, bstrValidationValue);
}
HRESULT SoftwareLicenseMgr::ValidateHardwareSystemType(_bstr_t bstrValidationValue)
{
	// Hide the string so it doesn't appear in the strings of the image - "SELECT SystemType FROM Win32_ComputerSystem"
	BYTE wmiQuery_Byte[] = {
		0x53, 0x00, 0x45, 0x00, 0x4c, 0x00, 0x45, 0x00, 0x43, 0x00, 0x54, 0x00, 0x20, 0x00, 0x53, 0x00, 0x79, 0x00, 0x73, 0x00, 0x74, 0x00, 0x65, 0x00, 0x6d, 0x00, 0x54, 0x00, 0x79, 0x00, 0x70, 0x00,
		0x65, 0x00, 0x20, 0x00, 0x46, 0x00, 0x52, 0x00, 0x4f, 0x00, 0x4d, 0x00, 0x20, 0x00, 0x57, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x33, 0x00, 0x32, 0x00, 0x5f, 0x00, 0x43, 0x00, 0x6f, 0x00, 0x6d, 0x00,
		0x70, 0x00, 0x75, 0x00, 0x74, 0x00, 0x65, 0x00, 0x72, 0x00, 0x53, 0x00, 0x79, 0x00, 0x73, 0x00, 0x74, 0x00, 0x65, 0x00, 0x6d, 0x00, 0x00, 0x00
	};

	// Hide the string so it doesn't appear in the strings of the image - "SystemType"
	BYTE fieldByte[] = {
		0x53, 0x00, 0x79, 0x00, 0x73, 0x00, 0x74, 0x00, 0x65, 0x00, 0x6d, 0x00, 0x54, 0x00, 0x79, 0x00, 0x70, 0x00, 0x65, 0x00, 0x00, 0x00
	};
	return WmiQueryContainsValue((wchar_t*)wmiQuery_Byte, (wchar_t*)fieldByte, bstrValidationValue);
}
HRESULT SoftwareLicenseMgr::ValidateHardwareSystemUUID(_bstr_t bstrValidationValue)
{
	// Hide the string so it doesn't appear in the strings of the image - "SELECT UUID FROM Win32_ComputerSystem"
	BYTE wmiQuery_Byte[] = {
		0x53, 0x00, 0x45, 0x00, 0x4c, 0x00, 0x45, 0x00, 0x43, 0x00, 0x54, 0x00, 0x20, 0x00, 0x55, 0x00, 0x55, 0x00, 0x49, 0x00, 0x44, 0x00, 0x20, 0x00, 0x46, 0x00, 0x52, 0x00, 0x4f, 0x00, 0x4d, 0x00, 
		0x20, 0x00, 0x57, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x33, 0x00, 0x32, 0x00, 0x5f, 0x00, 0x43, 0x00, 0x6f, 0x00, 0x6d, 0x00, 0x70, 0x00, 0x75, 0x00, 0x74, 0x00, 0x65, 0x00, 0x72, 0x00, 0x53, 0x00, 
		0x79, 0x00, 0x73, 0x00, 0x74, 0x00, 0x65, 0x00, 0x6d, 0x00, 0x50, 0x00, 0x72, 0x00, 0x6f, 0x00, 0x64, 0x00, 0x75, 0x00, 0x63, 0x00, 0x74, 0x00, 0x00, 0x00
	};

	// Hide the string so it doesn't appear in the strings of the image - "UUID"
	BYTE fieldByte[] = {
		0x55, 0x00, 0x55, 0x00, 0x49, 0x00, 0x44, 0x00, 0x00, 0x00
	};
	return WmiQueryContainsValue((wchar_t*)wmiQuery_Byte, (wchar_t*)fieldByte, bstrValidationValue);
}
//Private
HRESULT SoftwareLicenseMgr::WmiQueryContainsValue(wchar_t* _wQuery, wchar_t* _wColumn, bool _boolValue)
{
	HRESULT hr(LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_GENERIC);	// Use Generic Error Message)
	WMIHelper::ResultsType rList;

	// Hide the string so it doesn't appear in the strings of the image - "\\localhost\root\cimv2"
	BYTE wmiLocation_Byte[] = {
		0x5c, 0x00, 0x5c, 0x00, 0x6c, 0x00, 0x6f, 0x00, 0x63, 0x00, 0x61, 0x00, 0x6c, 0x00, 0x68, 0x00, 0x6f, 0x00, 0x73, 0x00, 0x74, 0x00, 0x5c, 0x00, 0x72, 0x00,
		0x6f, 0x00, 0x6f, 0x00, 0x74, 0x00, 0x5c, 0x00, 0x63, 0x00, 0x69, 0x00, 0x6d, 0x00, 0x76, 0x00, 0x32, 0x00, 0x00, 0x00
	};
	WMIHelper wmi((wchar_t*)wmiLocation_Byte);
	if(SUCCEEDED(wmi.Connect()))
	{
		if(SUCCEEDED(wmi.ExecuteQuery(_wQuery, rList)) && rList.size()>0)
		{
			for (unsigned int idx=0; idx<rList.size(); ++idx)
			{
				VARIANT* pvtTmp = &rList[idx][_wColumn];
				if (pvtTmp->vt == VT_BOOL)
					hr = (((pvtTmp->boolVal == VARIANT_TRUE) ? true : false) == _boolValue) ? S_OK : LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_GENERIC;
			}
			WMIHelper::Clear(rList);
			rList.clear();
		}
	}
	return hr;
}
HRESULT SoftwareLicenseMgr::WmiQueryContainsValue(wchar_t* _wQuery, wchar_t* _wColumn, _bstr_t _bstrValue)
{
	HRESULT hr(LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_GENERIC);	// Use Generic Error Message)
	WMIHelper::ResultsType rList;

	// Hide the string so it doesn't appear in the strings of the image - "\\localhost\root\cimv2"
	BYTE wmiLocation_Byte[] = {
		0x5c, 0x00, 0x5c, 0x00, 0x6c, 0x00, 0x6f, 0x00, 0x63, 0x00, 0x61, 0x00, 0x6c, 0x00, 0x68, 0x00, 0x6f, 0x00, 0x73, 0x00, 0x74, 0x00, 0x5c, 0x00, 0x72, 0x00,
		0x6f, 0x00, 0x6f, 0x00, 0x74, 0x00, 0x5c, 0x00, 0x63, 0x00, 0x69, 0x00, 0x6d, 0x00, 0x76, 0x00, 0x32, 0x00, 0x00, 0x00
	};
	WMIHelper wmi((wchar_t*)wmiLocation_Byte);
	if(SUCCEEDED(wmi.Connect()))
	{
		if(SUCCEEDED(wmi.ExecuteQuery(_wQuery, rList)) && rList.size()>0)
		{
			for (unsigned int idx=0; idx<rList.size(); ++idx)
			{
				VARIANT* pvtTmp = &rList[idx][_wColumn];
				if (pvtTmp->vt == VT_BSTR)
				{
					// The '™' character causes the encrypting of the xml to fail so it has been removed from teh token already, remove it from queried value
					std::wstring wsOS = std::wstring(pvtTmp->bstrVal);
					size_t stEscapePos = wsOS.find(L"™");
					if(stEscapePos != std::wstring::npos)
						wsOS.replace(stEscapePos, 1, L" ") ;

					if(_wcsicmp(wsOS.c_str(), _bstrValue) == 0)
					{
						hr = S_OK;
						break;
					}
				}
			}
			WMIHelper::Clear(rList);
			rList.clear();
		}
	}
	return hr;
}
HRESULT SoftwareLicenseMgr::RefreshLicenseFileAttribs()
{
//wchar_t debug_buf[1024];
//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::RefreshLicenseFileAttribs()");
//OutputDebugStringW(debug_buf);



	HRESULT hr(S_OK);
	SafeMutex mutex(softwareLicenseMgrLock);
	//Pull the license info from m_bstrLicenseFile
//#define GENERATE_LIC
	#ifndef GENERATE_LIC
	BSTR bstrSoftwareStream;
	hr = SoftwareLicenseFile::LoadFromLicenseFile(m_bstrLicenseFile, &bstrSoftwareStream);

	if(SUCCEEDED(hr))
	{
		m_licenseFileAttribs.InitFromString(bstrSoftwareStream);

		m_bstrLicenseName = bstr_t(Lic_PackageAttribsHelper::GetDisplayLabel(&m_licenseFileAttribs.licLicenseInfoAttribs).c_str());

//wchar_t debug_buf[1024];		
//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::RefreshLicenseFileAttribs() - %x-%x, %d", cI, sL, (int)m_licenseFileAttribs.licLicenseInfoAttribs.softwareLicType);
//OutputDebugStringW(debug_buf);

		
		//XXX - Development - Add to licenseFile
		//Lic_ServerDataAttribs::Lic_ServerDataFileInfoAttribs tmpFileInfo;
		//tmpFileInfo.LicFileName = std::wstring(SpdAttribs::WStringObj(m_bstrLicenseFile));
		//tmpFileInfo.LicFileVerificationCode = std::wstring(L"{D98A3C30-02DB-43D8-AF5F-CAEC61EC076B}");
		//tmpFileInfo.LicName = std::wstring(SpdAttribs::WStringObj(Lic_PackageAttribsHelper::GetDisplayLabel(&m_licenseFileAttribs.licLicenseInfoAttribs)));
		//hr = m_pLicServerDataMgr->SetFileInfoFor(std::wstring(SpdAttribs::WStringObj(tmpFileInfo.LicName)).c_str(), &tmpFileInfo);

		//Clean up
		SysFreeString(bstrSoftwareStream);
	}
	#endif

	///*
	#ifdef GENERATE_LIC
	//Initialize license file
	Lic_PackageAttribs tmpLicAttribs;
	//tmpLicAttribs.licLicenseInfoAttribs.customerID = 0x1000;
	tmpLicAttribs.licLicenseInfoAttribs.customerID = 0x0100;
	tmpLicAttribs.licLicenseInfoAttribs.destinationID = 0x000;
	//tmpLicAttribs.licLicenseInfoAttribs.softwareLicenseID = 0x1000;
	//tmpLicAttribs.licLicenseInfoAttribs.softwareLicenseID = 0x1cba;
	//tmpLicAttribs.licLicenseInfoAttribs.softwareGroupLicenseID = 0x1cba;
	tmpLicAttribs.licLicenseInfoAttribs.softwareGroupLicenseID = 0x1000;
	//tmpLicAttribs.licLicenseInfoAttribs.softwareLicenseID = 0xffff;

	//Initialize VerificationTokens
	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_VerificationAttribs tmpVerAttribs;
	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs tmpVerTokenAttribs;
	tmpVerTokenAttribs.tokenType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttHardwareKeyID;
	tmpVerTokenAttribs.tokenValue = std::wstring(L"100-1000");
	tmpLicAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->push_back(tmpVerTokenAttribs);

	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs tmpVerTokenAttribs2;
	tmpVerTokenAttribs2.tokenType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttLicenseCode;
	//tmpVerTokenAttribs2.tokenValue = std::wstring(L"{27AA5F32-E89F-4A91-B454-6C28563C6A46}");
	tmpVerTokenAttribs2.tokenValue = std::wstring(L"{D98A3C30-02DB-43D8-AF5F-CAEC61EC076B}");
	tmpLicAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->push_back(tmpVerTokenAttribs2);

	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs tmpVerTokenAttribs3;
	tmpVerTokenAttribs3.tokenType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttComputerName;
	tmpVerTokenAttribs3.tokenValue = std::wstring(L"jlan5");
	//tmpVerTokenAttribs3.tokenValue = std::wstring(L"vmec1");
	tmpLicAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->push_back(tmpVerTokenAttribs3);

	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs tmpVerTokenAttribs4;
	tmpVerTokenAttribs4.tokenType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttMacAddress;
	tmpVerTokenAttribs4.tokenValue = std::wstring(L"00:18:8b:8c:43:a1");
	//tmpVerTokenAttribs4.tokenValue = std::wstring(L"08:00:27:41:3E:4C");
	tmpLicAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->push_back(tmpVerTokenAttribs4);

	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs tmpVerTokenAttribs5;
	tmpVerTokenAttribs5.tokenType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttBiosSerialNumber;
	tmpVerTokenAttribs5.tokenValue = std::wstring(L"B7CHFC1");
	tmpLicAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->push_back(tmpVerTokenAttribs5);


	tmpLicAttribs.licLicenseInfoAttribs.productList->push_back(TestingOnly_GenerateLicense_Rubika());
	tmpLicAttribs.licLicenseInfoAttribs.productList->push_back(TestingOnly_GenerateLicense_SPDE());
	tmpLicAttribs.licLicenseInfoAttribs.productList->push_back(TestingOnly_GenerateLicense_SSE());
	tmpLicAttribs.licLicenseInfoAttribs.productList->push_back(TestingOnly_GenerateLicense_SDX());
	tmpLicAttribs.licLicenseInfoAttribs.productList->push_back(TestingOnly_GenerateLicense_iCONVERT());
	tmpLicAttribs.licLicenseInfoAttribs.productList->push_back(TestingOnly_GenerateLicense_SOLfusion());
	tmpLicAttribs.licLicenseInfoAttribs.productList->push_back(TestingOnly_GenerateLicense_RubikaProcessBuilder());
	tmpLicAttribs.licLicenseInfoAttribs.productList->push_back(TestingOnly_GenerateLicense_SOLindexer());
	tmpLicAttribs.licLicenseInfoAttribs.productList->push_back(TestingOnly_GenerateLicense_SPDE_QueueManager());
	
	
	BSTR bstrSoftwareStream2 = SysAllocString(tmpLicAttribs.ToString().c_str());
	hr = SoftwareLicenseFile::SaveToLicenseFile(m_bstrLicenseFile, bstrSoftwareStream2);
	SysFreeString(bstrSoftwareStream2);
	#endif
	//*/

	

	return hr;
}

Lic_PackageAttribs::Lic_ProductInfoAttribs SoftwareLicenseMgr::TestingOnly_GenerateLicense_Rubika()
{
	int appInstance = 8;
	Lic_PackageAttribs::Lic_ProductInfoAttribs tmpProdAttribs;
	tmpProdAttribs.productID = 12;
	tmpProdAttribs.product_Major = 2;
	tmpProdAttribs.product_Minor = 2;
	tmpProdAttribs.productAppInstance = appInstance;
	
	//The Modules 0 thru 19 are all on/off modules...
	for(int idx=0; idx<=19; idx++)
	{
		Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs1;
		tmpModAttribs1.moduleID = idx;
		tmpModAttribs1.moduleValue = appInstance;
		tmpModAttribs1.moduleAppInstance = appInstance;
		//tmpModAttribs1.moduleExpirationDate
		tmpProdAttribs.moduleList->push_back(tmpModAttribs1);
	}

	Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs2;
	tmpModAttribs2.moduleID = 30;
	tmpModAttribs2.moduleValue = 255;
	tmpModAttribs2.moduleAppInstance = appInstance;
	tmpProdAttribs.moduleList->push_back(tmpModAttribs2);

	Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs3;
	tmpModAttribs3.moduleID = 31;
	tmpModAttribs3.moduleValue = 255;
	tmpModAttribs3.moduleAppInstance = appInstance;
	tmpProdAttribs.moduleList->push_back(tmpModAttribs3);


	return tmpProdAttribs;
}

Lic_PackageAttribs::Lic_ProductInfoAttribs SoftwareLicenseMgr::TestingOnly_GenerateLicense_SPDE()
{
	int appInstance = 8;
	Lic_PackageAttribs::Lic_ProductInfoAttribs tmpProdAttribs;
	tmpProdAttribs.productID = 14;
	tmpProdAttribs.product_Major = 6;
	tmpProdAttribs.product_Minor = 1;
	tmpProdAttribs.productAppInstance = appInstance;


	//The Modules 0 thru 50 are all on/off modules...
	for(int idx=0; idx<=50; idx++)
	{
		Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs1;
		tmpModAttribs1.moduleID = idx;
		tmpModAttribs1.moduleValue = appInstance;
		tmpModAttribs1.moduleAppInstance = appInstance;
		tmpProdAttribs.moduleList->push_back(tmpModAttribs1);
	}

	for(int idx=101; idx<=105; idx++)
	{
		Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs1;
		tmpModAttribs1.moduleID = idx;
		tmpModAttribs1.moduleValue = appInstance * 15;
		tmpModAttribs1.moduleAppInstance = appInstance;
		tmpProdAttribs.moduleList->push_back(tmpModAttribs1);
	}

	for(int idx=200; idx<=202; idx++)
	{
		Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs1;
		tmpModAttribs1.moduleID = idx;
		tmpModAttribs1.moduleValue = appInstance * 15;
		tmpModAttribs1.moduleAppInstance = appInstance;
		tmpProdAttribs.moduleList->push_back(tmpModAttribs1);
	}

	Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs1;
	tmpModAttribs1.moduleID = 240;	//Output Pool
	tmpModAttribs1.moduleValue = appInstance * 10;	//Unlimited @ 10
	tmpModAttribs1.moduleAppInstance = appInstance;
	tmpProdAttribs.moduleList->push_back(tmpModAttribs1);

	//PPM section...
	for(int idx=300; idx<=305; idx++)
	{
		Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs1;
		tmpModAttribs1.moduleID = idx;
		tmpModAttribs1.moduleValue = appInstance * 500;
		tmpModAttribs1.moduleAppInstance = appInstance;
		tmpProdAttribs.moduleList->push_back(tmpModAttribs1);
	}

	Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs3;
	tmpModAttribs3.moduleID = 400;	//ConcurrentOperator
	//tmpModAttribs3.moduleValue = 255;	//Unlimited @ 255
	tmpModAttribs3.moduleValue = 1;	//Unlimited @ 255
	tmpModAttribs3.moduleAppInstance = appInstance;
	tmpProdAttribs.moduleList->push_back(tmpModAttribs3);

//Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribsAddOn1;
//tmpModAttribsAddOn1.moduleID = 400;	//ConcurrentOperator
//tmpModAttribsAddOn1.moduleValue = 10;	//Unlimited @ 255
//tmpModAttribsAddOn1.moduleAppInstance = 4;
//time_t curTimeT = time(NULL);	//Retrieves Universal Time
//VARIANT vtCurTime = TimeHelper::TimeTToVariant(curTimeT);
//SYSTEMTIME curTimeSystime;
//VariantTimeToSystemTime(vtCurTime.date, &curTimeSystime);
//wchar_t curTimeStamp[256];
//TimeHelper::SystemTimeToString(curTimeStamp, sizeof(curTimeStamp)/sizeof(wchar_t), curTimeSystime);
//tmpModAttribsAddOn1.moduleExpirationDate = std::wstring(curTimeStamp);
//tmpProdAttribs.moduleList->push_back(tmpModAttribsAddOn1);
//
//Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribsAddOn2;
//tmpModAttribsAddOn2.moduleID = 400;	//ConcurrentOperator
//tmpModAttribsAddOn2.moduleValue = 100;	//Unlimited @ 255
//tmpModAttribsAddOn2.moduleAppInstance = 4;
//curTimeT = time(NULL);	//Retrieves Universal Time
//VARIANT vtAddOnTime = TimeHelper::TimeTToVariant(curTimeT+(1*TimeHelper::ONE_DAY_IN_SECONDS));
//SYSTEMTIME addOnTimeSystime;
//VariantTimeToSystemTime(vtAddOnTime.date, &addOnTimeSystime);
//wchar_t addOnTimeStamp[256];
//TimeHelper::SystemTimeToString(addOnTimeStamp, sizeof(addOnTimeStamp)/sizeof(wchar_t), addOnTimeSystime);
//tmpModAttribsAddOn2.moduleExpirationDate = std::wstring(addOnTimeStamp);
//tmpProdAttribs.moduleList->push_back(tmpModAttribsAddOn2);




	Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs4;
	tmpModAttribs4.moduleID = 401;	//Named User Sessions
	tmpModAttribs4.moduleValue = 50;	//Unlimited @ 255
	tmpModAttribs4.moduleAppInstance = appInstance;
	tmpProdAttribs.moduleList->push_back(tmpModAttribs4);




	return tmpProdAttribs;
}





Lic_PackageAttribs::Lic_ProductInfoAttribs SoftwareLicenseMgr::TestingOnly_GenerateLicense_SSE()
{
	int appInstance = 8;
	Lic_PackageAttribs::Lic_ProductInfoAttribs tmpProdAttribs;

	tmpProdAttribs.productID = 8;
	tmpProdAttribs.product_Major = 3;
	tmpProdAttribs.product_Minor = 2;
	tmpProdAttribs.productAppInstance = appInstance;

	//General SSE Licensing
	for(int idx=0; idx<=7; idx++)
	{
		Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs1;
		tmpModAttribs1.moduleID = idx;
		if(idx == 2)	//Concurrent Users
			tmpModAttribs1.moduleValue = 255;
		else if(idx == 3)	//App DB
			tmpModAttribs1.moduleValue = 255;
		else if(idx == 4)	//DocAssembler
			tmpModAttribs1.moduleValue = appInstance;
		else
			tmpModAttribs1.moduleValue = appInstance * 10;

		tmpModAttribs1.moduleAppInstance = appInstance;
		tmpProdAttribs.moduleList->push_back(tmpModAttribs1);
	}


	//On-Off Section
	for(int idx=128; idx<=133; idx++)
	{
		Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs1;
		tmpModAttribs1.moduleID = idx;
		tmpModAttribs1.moduleValue = appInstance;
		tmpModAttribs1.moduleAppInstance = appInstance;
		tmpProdAttribs.moduleList->push_back(tmpModAttribs1);
	}

	return tmpProdAttribs;
}
Lic_PackageAttribs::Lic_ProductInfoAttribs SoftwareLicenseMgr::TestingOnly_GenerateLicense_SDX()
{
	int appInstance = 8;
	Lic_PackageAttribs::Lic_ProductInfoAttribs tmpProdAttribs;

	tmpProdAttribs.productID = 10;
	tmpProdAttribs.product_Major = 3;
	tmpProdAttribs.product_Minor = 2;
	tmpProdAttribs.productAppInstance = appInstance;

	Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs1;
	tmpModAttribs1.moduleID = 0;
	tmpModAttribs1.moduleValue = appInstance * 5;
	tmpModAttribs1.moduleAppInstance = appInstance;
	tmpProdAttribs.moduleList->push_back(tmpModAttribs1);

	Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs2;
	tmpModAttribs2.moduleID = 1;
	tmpModAttribs2.moduleValue = appInstance * 5;
	tmpModAttribs2.moduleAppInstance = appInstance;
	tmpProdAttribs.moduleList->push_back(tmpModAttribs2);

	return tmpProdAttribs;

}
Lic_PackageAttribs::Lic_ProductInfoAttribs SoftwareLicenseMgr::TestingOnly_GenerateLicense_SOLindexer()
{
	int appInstance = 8;
	Lic_PackageAttribs::Lic_ProductInfoAttribs tmpProdAttribs;

	tmpProdAttribs.productID = 7;
	tmpProdAttribs.product_Major = 9;
	tmpProdAttribs.product_Minor = 9;
	tmpProdAttribs.productAppInstance = appInstance;

	for(int idx=0; idx<=2; idx++)
	{
		Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs1;
		tmpModAttribs1.moduleID = idx;
		tmpModAttribs1.moduleValue = appInstance * 50;
		tmpModAttribs1.moduleAppInstance = appInstance;
		tmpProdAttribs.moduleList->push_back(tmpModAttribs1);
	}

	for(int idx=8; idx<=56; idx++)
	{
		if(
			(idx>=8 && idx<=9) ||
			(idx>=12 && idx<=15) ||
			(idx>=29 && idx<=30) ||
			(idx>=32 && idx<=35) ||
			(idx>=40 && idx<=43) ||
			(idx>=48 && idx<=51) ||
			(idx==56)
			)
		{
			Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs1;
			tmpModAttribs1.moduleID = idx;
			tmpModAttribs1.moduleValue = appInstance * 10;
			tmpModAttribs1.moduleAppInstance = appInstance;
			tmpProdAttribs.moduleList->push_back(tmpModAttribs1);
		}
	}



	Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs4;
	tmpModAttribs4.moduleID = 128;	//Max LU
	tmpModAttribs4.moduleValue = appInstance * 255;	
	tmpModAttribs4.moduleAppInstance = appInstance;
	tmpProdAttribs.moduleList->push_back(tmpModAttribs4);

	return tmpProdAttribs;

}

Lic_PackageAttribs::Lic_ProductInfoAttribs SoftwareLicenseMgr::TestingOnly_GenerateLicense_SOLfusion()
{
		int appInstance = 8;
	Lic_PackageAttribs::Lic_ProductInfoAttribs tmpProdAttribs;

	tmpProdAttribs.productID = 13;
	tmpProdAttribs.product_Major = 1;
	tmpProdAttribs.product_Minor = 0;
	tmpProdAttribs.productAppInstance = appInstance;

	Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs4;
	tmpModAttribs4.moduleID = 0;
	tmpModAttribs4.moduleValue = appInstance * 10;	
	tmpModAttribs4.moduleAppInstance = appInstance;
	tmpProdAttribs.moduleList->push_back(tmpModAttribs4);

	return tmpProdAttribs;
}
Lic_PackageAttribs::Lic_ProductInfoAttribs SoftwareLicenseMgr::TestingOnly_GenerateLicense_RubikaProcessBuilder()
{
	int appInstance = 8;
	Lic_PackageAttribs::Lic_ProductInfoAttribs tmpProdAttribs;

	tmpProdAttribs.productID = 16;
	tmpProdAttribs.product_Major = 2;
	tmpProdAttribs.product_Minor = 2;
	tmpProdAttribs.productAppInstance = appInstance;

	Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs4;
	tmpModAttribs4.moduleID = 31;
	tmpModAttribs4.moduleValue = appInstance * 10;	
	tmpModAttribs4.moduleAppInstance = appInstance;
	tmpProdAttribs.moduleList->push_back(tmpModAttribs4);

	return tmpProdAttribs;
}
Lic_PackageAttribs::Lic_ProductInfoAttribs SoftwareLicenseMgr::TestingOnly_GenerateLicense_iCONVERT()
{
	int appInstance = 8;
	Lic_PackageAttribs::Lic_ProductInfoAttribs tmpProdAttribs;

	tmpProdAttribs.productID = 17;
	tmpProdAttribs.product_Major = 3;
	tmpProdAttribs.product_Minor = 2;
	tmpProdAttribs.productAppInstance = appInstance;

	Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs4;
	tmpModAttribs4.moduleID = 0;
	tmpModAttribs4.moduleValue = appInstance * 10;	
	tmpModAttribs4.moduleAppInstance = appInstance;
	tmpProdAttribs.moduleList->push_back(tmpModAttribs4);

	return tmpProdAttribs;
}
Lic_PackageAttribs::Lic_ProductInfoAttribs SoftwareLicenseMgr::TestingOnly_GenerateLicense_SPDE_QueueManager()
{
	int appInstance = 8;
	Lic_PackageAttribs::Lic_ProductInfoAttribs tmpProdAttribs;

	tmpProdAttribs.productID = 18;
	tmpProdAttribs.product_Major = 6;
	tmpProdAttribs.product_Minor = 1;
	tmpProdAttribs.productAppInstance = appInstance;

	Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribs4;
	tmpModAttribs4.moduleID = 400;
	tmpModAttribs4.moduleValue = appInstance * 10;	
	tmpModAttribs4.moduleAppInstance = appInstance;
	tmpProdAttribs.moduleList->push_back(tmpModAttribs4);

	return tmpProdAttribs;
}


HRESULT SoftwareLicenseMgr::GetSoftwareLicenseName(BSTR *pBstrLicenseName)
{
	SafeMutex mutex(softwareLicenseMgrLock);
	//*pBstrLicenseName = SysAllocString(m_bstrLicenseFile);
	*pBstrLicenseName = SysAllocString(m_bstrLicenseName);
	return S_OK;
}

HRESULT SoftwareLicenseMgr::GetVerificationAttribs(Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_VerificationAttribs* pVerificationAttrib)
{
	SafeMutex mutex(softwareLicenseMgrLock);
	*pVerificationAttrib = m_licenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs;
	return S_OK;
}


HRESULT SoftwareLicenseMgr::GetLicenseInfo(Lic_PackageAttribs::Lic_LicenseInfoAttribs* pLicenseInfo)
{
	SafeMutex mutex(softwareLicenseMgrLock);
	*pLicenseInfo = m_licenseFileAttribs.licLicenseInfoAttribs;
	return S_OK;
}
HRESULT SoftwareLicenseMgr::GetSoftwareSpec(Lic_PackageAttribs::Lic_SoftwareSpecAttribs* pSoftwareSpec)
{
	SafeMutex mutex(softwareLicenseMgrLock);
	*pSoftwareSpec = m_licenseFileAttribs.licSoftwareSpecAttribs;
	return S_OK;
}


HRESULT SoftwareLicenseMgr::GetProtectionKeyModifiedDate(time_t* pTimeTModifiedDate, Lic_PackageAttribs* pLicenseFileAttribs)
{
//wchar_t debug_buf[1024];
//_snwprintf_s(debug_buf, _countof(debug_buf), L"SoftwareLicenseMgr::GetProtectionKeyModifiedDate()");
//OutputDebugStringW(debug_buf);
	HRESULT hr(E_INVALIDARG);
	*pTimeTModifiedDate = -1;
	//Determine if the Software License Manager has a protection key as a verification token
	_bstr_t keyID = L"";
	for(	std::vector<Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs>::iterator valTokenIT = pLicenseFileAttribs->licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->begin();
						valTokenIT != pLicenseFileAttribs->licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->end();
						valTokenIT++)
	{
		if(Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttHardwareKeyID == (Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::TTokenType)valTokenIT->tokenType)
		{
			keyID = _bstr_t(valTokenIT->tokenValue->c_str());
			break;
		}
	}

	if(_wcsicmp(keyID, L"") != 0)
	{
		if(m_pKeyServer != NULL)
		{
			Lic_KeyAttribs licKeyAttribs;
			hr = m_pKeyServer->GetKeyInfoAttribs(keyID, &licKeyAttribs);
			if(SUCCEEDED(hr))
			{
				SYSTEMTIME tmpDateSystime;
				if(!TimeHelper::StringToSystemTime(std::wstring(SpdAttribs::WStringObj(licKeyAttribs.packetCreationDate)).c_str(), tmpDateSystime))
					throw(E_FAIL);

				_variant_t tmpDateVt(NULL);	
				if (!SystemTimeToVariantTime(&tmpDateSystime, &tmpDateVt.date)) 
					throw(E_FAIL);

				*pTimeTModifiedDate = TimeHelper::VariantToTimeT(tmpDateVt, false);
			}
		}
	}
	return hr;
}

HRESULT SoftwareLicenseMgr::GetKeyInfoAttribs_FromKey(Lic_KeyAttribs* pKeyAttribs, Lic_PackageAttribs* pLicenseFileAttribs)
{
	HRESULT hr(E_INVALIDARG);
	//Determine if the Software License Manager has a protection key as a verification token
	_bstr_t keyID = L"";
	for(	std::vector<Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs>::iterator valTokenIT = pLicenseFileAttribs->licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->begin();
						valTokenIT != pLicenseFileAttribs->licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->end();
						valTokenIT++)
	{
		if(Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttHardwareKeyID == (Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::TTokenType)valTokenIT->tokenType)
		{
			keyID = _bstr_t(valTokenIT->tokenValue->c_str());
			break;
		}
	}
	if(_wcsicmp(keyID, L"") != 0)
	{
		if(m_pKeyServer != NULL)
			hr = m_pKeyServer->GetKeyInfoAttribs(keyID, pKeyAttribs);
	}
	return hr;
}
//HRESULT SoftwareLicenseMgr::GetKeyInfoAttribs_FromSystem(Lic_KeyAttribs* pKeyAttribs)
//{
//	return m_pLicServerDataMgr->GetKeyInfoFor(m_bstrLicenseName, pKeyAttribs);
//}
HRESULT SoftwareLicenseMgr::DeleteLicenseFile()
{
	SafeMutex mutex(softwareLicenseMgrLock);
	return SoftwareLicenseFile::DeleteLicenseFile(m_bstrLicenseFile);
}
//Returns S_OK if the _productID is in the LicenseInfo structure, else E_FAIL
HRESULT SoftwareLicenseMgr::ContainsLicenseForProduct(long _productID)
{
	HRESULT hr(E_FAIL);
	try
	{
		for(Lic_PackageAttribs::Lic_LicenseInfoAttribs::TVector_Lic_ProductInfoAttribsList::iterator prodIt = m_licenseFileAttribs.licLicenseInfoAttribs.productList->begin();
			prodIt != m_licenseFileAttribs.licLicenseInfoAttribs.productList->end();
			prodIt++)
		{
			if((int)prodIt->productID == (int)_productID)
			{
				hr = S_OK;	
				break;	//Match is found, leave
			}
		}
	}
	catch(HRESULT &eHr)
	{
		hr = eHr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}

	return hr;
}
//HRESULT SoftwareLicenseMgr::GetSoftwareSpec(Lic_PackageAttribs::Lic_SoftwareSpecAttribs* pSoftwareSpec)
//{
//	SafeMutex mutex(softwareLicenseMgrLock);
//
//	//!!!For testing
//	*pSoftwareSpec = g_pSoftwareSpec->GetSoftwareSpec();
//	//pSoftwareSpec = &g_softwareSpec.softwareSpec;
//
//	//Comment in later...
//	//*pSoftwareSpec = m_licenseFileAttribs.licSoftwareSpecAttribs;
//	return S_OK;
//}
//


//private 
HRESULT SoftwareLicenseMgr::VerifyForSoftwareLicenseUpgrade(Lic_PackageAttribs* _pLicPackageAttribs, bool _bIgnoreLicenseCode, bool _bIgnoreModifiedDate)
{
//wchar_t debug_buf[1024];
	HRESULT hr(S_OK);
	SafeMutex mutex(softwareLicenseMgrLock);
	try
	{
		if(_pLicPackageAttribs == NULL)
			throw E_INVALIDARG;

		//if there is a hardware key in the verification attributes, makes sure that the key is plugged in the system 
		// before applying the new license packet or else can't write new hardware key code.

		std::wstring tmpKeyID = Lic_PackageAttribsHelper::GetValidationValue(&(_pLicPackageAttribs->licLicenseInfoAttribs.licVerificationAttribs.validationTokenList), Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttHardwareKeyID);
		if(tmpKeyID.length() != 0)
		{
			hr = ValidateHardwareKeyID(tmpKeyID.c_str());
			if(FAILED(hr))
			{
				if(hr == LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_KEY_ID)
					hr = LicenseServerError::EHR_LIC_SOFTWARE_KEY_ID_REQUIRED;
				throw hr;
			}
		}

		if(!_bIgnoreLicenseCode)
		{
			//make sure that if there is a licenseID, it must match because if it doesn't match that means that the license file might have been 
			//replaced with an older one
			std::wstring tmpLicenseCode = Lic_PackageAttribsHelper::GetValidationValue(&(_pLicPackageAttribs->licLicenseInfoAttribs.licVerificationAttribs.validationTokenList), Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttLicenseCode);
			if(tmpLicenseCode.length() != 0)
			{
				hr = ValidateLicenseCode(tmpLicenseCode.c_str());
				if(FAILED(hr))
					throw hr;
			}
		}


		if(!_bIgnoreModifiedDate)
		{
			//find packet creation date from key
			time_t modifiedTimeT = -1;
			hr = GetProtectionKeyModifiedDate(&modifiedTimeT, _pLicPackageAttribs);
			if(FAILED(hr))
			{
				//failed to find modified date from key, find modified date from ServerDataMgr
				Lic_ServerDataAttribs::Lic_ServerDataFileInfoAttribs tmpFileInfo;
				hr = m_pLicServerDataMgr->GetFileInfoFor(std::wstring(SpdAttribs::WStringObj(Lic_PackageAttribsHelper::GetDisplayLabel(&(_pLicPackageAttribs->licLicenseInfoAttribs)))).c_str(), &tmpFileInfo);

				if(SUCCEEDED(hr))
				{
					_variant_t vtPackageModifiedDate(NULL);	
					SYSTEMTIME packageModifiedDateSystime;
					SystemTimeToVariantTime(&packageModifiedDateSystime, &vtPackageModifiedDate.date);
					modifiedTimeT = TimeHelper::VariantToTimeT(vtPackageModifiedDate, false);
				}
			}

			if(modifiedTimeT != -1)	//if found an existing package for the software license
			{
				SYSTEMTIME newPackageModifiedDateSystime;

				if(TimeHelper::StringToSystemTime(std::wstring(SpdAttribs::WStringObj(_pLicPackageAttribs->licLicenseInfoAttribs.modifiedDate)).c_str(), newPackageModifiedDateSystime))
				{
					_variant_t vtNewPackageModifiedDate(NULL);	
					SystemTimeToVariantTime(&newPackageModifiedDateSystime, &vtNewPackageModifiedDate.date);
					time_t newPackageModifiedDateTimeT = TimeHelper::VariantToTimeT(vtNewPackageModifiedDate, false);
					if(modifiedTimeT >= newPackageModifiedDateTimeT)
						throw LicenseServerError::EHR_LIC_SOFTWARE_LIC_PACKET_ALREADY_ENTERED;
				}
			}
			hr = S_OK;
		}
	}
	catch(HRESULT &eHr)
	{
		hr = eHr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	return hr;
}

//public
HRESULT SoftwareLicenseMgr::ApplyLicensePacket(Lic_PackageAttribs* pLicPacket, _bstr_t bstrVerificationCode)
{
	HRESULT hr(S_OK);
	SafeMutex mutex(softwareLicenseMgrLock);
	BSTR bstrLicPacketStream = SysAllocString(pLicPacket->ToString().c_str());	//Use a copy of...
	try
	{
		bool bResetCurrentDateToNow = false; //To reset clock violations, who accidently set their clock ahead, then back 
		Lic_PackageAttribs tmpPacketNewLicenseFileAttribs;
		tmpPacketNewLicenseFileAttribs.InitFromString(bstrLicPacketStream);

		//
		//YYY - HARD CODE PACKAGE INFORMATION - START
		//{
		//	bool bFoundHardwareKey = false;
		//	std::vector<Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs>::iterator valTokenIT = tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->begin();
		//	while(valTokenIT != tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->end())
		//	{
		//		switch((Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::TTokenType)valTokenIT->tokenType)
		//		{
		//			case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttHardwareKeyID:
		//				valTokenIT->tokenValue = std::wstring(L"0100-1001");
		//				bFoundHardwareKey = true;
		//				valTokenIT++;
		//				break;
		//			case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttLicenseCode:
		//				valTokenIT++;
		//				break;
		//			default:
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->erase(valTokenIT);
		//				valTokenIT = tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->begin();
		//				break;
		//		}
		//	}
		//	if(bFoundHardwareKey == false) //YYY - add verification token of usb key
		//	{
		//		Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs valToken;
		//		valToken.tokenType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttHardwareKeyID;
		//		valToken.tokenValue = std::wstring(L"0100-1001");
		//		tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->push_back(valToken);
		//	}

		//	//tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.customerID = 1;
		//	//tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.destinationID = 1;
		//	{
		//	///*
		//	//Step 1
		//	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotInfoAttribs actSlotInfo0;
		//	actSlotInfo0.activitySlotID = 0;
		//	actSlotInfo0.contractNumber = std::wstring(L"100-00-00-T1-0");
		//	tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.activitySlotList->push_back(actSlotInfo0);

		//	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotInfoAttribs actSlotInfo1;
		//	actSlotInfo1.activitySlotID = 1;
		//	actSlotInfo1.contractNumber = std::wstring(L"100-00-00-T1-1");
		//	tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.activitySlotList->push_back(actSlotInfo1);

		//	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotInfoAttribs actSlotInfo2;
		//	actSlotInfo2.activitySlotID = 2;
		//	actSlotInfo2.contractNumber = std::wstring(L"100-00-00-T1-2");
		//	tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.activitySlotList->push_back(actSlotInfo2);

		//	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotInfoAttribs actSlotInfo3;
		//	actSlotInfo3.activitySlotID = 3;
		//	actSlotInfo3.contractNumber = std::wstring(L"100-00-00-T1-3");
		//	tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.activitySlotList->push_back(actSlotInfo3);
		//	//*/

		//	//History 1)
		//	//add 100-00-00-T1-0 to slot 0
		//	//add 100-00-00-T1-1 to slot 1
		//	//add 100-00-00-T1-2 to slot 2
		//	//add 100-00-00-T1-3 to slot 3
		//	
		//	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotHistoryInfoAttribs slotHistoryAttribs;
		//	slotHistoryAttribs.historyNumber = 1;

		//	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs changeInfo0Attribs;
		//	changeInfo0Attribs.actionType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs::ascaAdd;
		//	changeInfo0Attribs.contractNumber = std::wstring(L"100-00-00-T1-0");
		//	changeInfo0Attribs.param1 = 0;
		//	slotHistoryAttribs.activitySlotChangeInfoList->push_back(changeInfo0Attribs);

		//	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs changeInfo1Attribs;
		//	changeInfo1Attribs.actionType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs::ascaAdd;
		//	changeInfo1Attribs.contractNumber = std::wstring(L"100-00-00-T1-1");
		//	changeInfo1Attribs.param1 = 1;
		//	slotHistoryAttribs.activitySlotChangeInfoList->push_back(changeInfo1Attribs);

		//	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs changeInfo2Attribs;
		//	changeInfo2Attribs.actionType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs::ascaAdd;
		//	changeInfo2Attribs.contractNumber = std::wstring(L"100-00-00-T1-2");
		//	changeInfo2Attribs.param1 = 2;
		//	slotHistoryAttribs.activitySlotChangeInfoList->push_back(changeInfo2Attribs);

		//	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs changeInfo3Attribs;
		//	changeInfo3Attribs.actionType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs::ascaAdd;
		//	changeInfo3Attribs.contractNumber = std::wstring(L"100-00-00-T1-3");
		//	changeInfo3Attribs.param1 = 3;
		//	slotHistoryAttribs.activitySlotChangeInfoList->push_back(changeInfo3Attribs);

		//	tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.activitySlotHistoryList->push_back(slotHistoryAttribs);
		//	}

		//	/*
		//	{
		//	//Step 2
		//	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotInfoAttribs actSlotInfo0;
		//	actSlotInfo0.activitySlotID = 0;
		//	actSlotInfo0.contractNumber = std::wstring(L"100-00-00-T1-0");
		//	tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.activitySlotList->push_back(actSlotInfo0);

		//	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotInfoAttribs actSlotInfo3;
		//	actSlotInfo3.activitySlotID = 1;
		//	actSlotInfo3.contractNumber = std::wstring(L"100-00-00-T1-3");
		//	tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.activitySlotList->push_back(actSlotInfo3);

		//	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotInfoAttribs actSlotInfo2;
		//	actSlotInfo2.activitySlotID = 2;
		//	actSlotInfo2.contractNumber = std::wstring(L"100-00-00-T1-2");
		//	tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.activitySlotList->push_back(actSlotInfo2);

		//	//YYY - remove L"100-00-00-T1-1"
		//	//History, 2)
		//	//delete 100-00-00-T1-1 from slot 1
		//	//move contents of slot 3 to slot 1
		//	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotHistoryInfoAttribs slotHistoryAttribs;
		//	slotHistoryAttribs.historyNumber = 2;
		//	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs changeInfo1Attribs;
		//	changeInfo1Attribs.actionType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs::ascaDelete;
		//	changeInfo1Attribs.contractNumber = std::wstring(L"100-00-00-T1-1");
		//	changeInfo1Attribs.param1 = 1;
		//	slotHistoryAttribs.activitySlotChangeInfoList->push_back(changeInfo1Attribs);

		//	Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs changeInfo2Attribs;
		//	changeInfo2Attribs.actionType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs::ascaMove;
		//	changeInfo2Attribs.contractNumber = std::wstring(L"100-00-00-T1-3");
		//	changeInfo2Attribs.param1 = 3;
		//	changeInfo2Attribs.param2 = 1;
		//	slotHistoryAttribs.activitySlotChangeInfoList->push_back(changeInfo2Attribs);

		//	tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.activitySlotHistoryList->push_back(slotHistoryAttribs);
		//	}
		//	//*/

		//	{
		//		for(int idx = 0;
		//			idx < tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->size();
		//			idx++)
		//		{
		//			tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).productAppInstance = 1;
		//			if(idx == 0)
		//			{
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).contractNumber = std::wstring(L"100-00-00-T1-0");
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).activationTotal = 15;
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).activationAmountInDays = 2;
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).bUseActivations = true;
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).bUseExpirationDate = true;
		//			}
		//			else if(idx == 1)
		//			{
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).contractNumber = std::wstring(L"100-00-00-T1-1");
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).activationTotal = 45;
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).activationAmountInDays = 1;
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).bUseActivations = true;
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).bUseExpirationDate = true;
		//			}
		//			else if(idx == 3)
		//			{
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).contractNumber = std::wstring(L"100-00-00-T1-2");
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).activationTotal = 3;
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).activationAmountInDays = 20;
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).bUseActivations = true;
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).bUseExpirationDate = true;
		//			}
		//			else if(idx == 2)
		//			{
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).contractNumber = std::wstring(L"100-00-00-T1-3");
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).activationTotal = 7;
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).activationAmountInDays = 3;
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).bUseActivations = true;
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).bUseExpirationDate = true;
		//			}
		//			else if(idx == 4)
		//			{
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).contractNumber = std::wstring(L"100-00-00-T1-4");
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).bUseActivations = false;
		//				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).bUseExpirationDate = true;
		//			}

		//			//Lic_PackageAttribs::Lic_ProductInfoAttribs::Lic_ModuleInfoAttribsList::iter
		//			std::vector<Lic_PackageAttribs::Lic_ModuleInfoAttribs>::iterator modIt = tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).moduleList->begin();
		//			while(modIt != tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).moduleList->end())
		//			{
		//				if(_wcsicmp(std::wstring(modIt->contractNumber).c_str(), std::wstring(tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).contractNumber).c_str()) != 0)
		//				{
		//					tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).moduleList->erase(modIt);
		//					modIt != tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).moduleList->begin();
		//				}
		//				else
		//				{
		//					tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).expirationDate = modIt->moduleExpirationDate;
		//					modIt++;
		//				}
		//			}
		//		}

		//		//YYY - remove L"100-00-00-T1-1", assuming is 1 spot from begin()
		//		//Lic_PackageAttribs::Lic_LicenseInfoAttribs::TVector_Lic_ProductInfoAttribsList::iterator prodIt = tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->begin();
		//		//prodIt++; //go to first item
		//		//tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->erase(prodIt);
		//	}
		//}
		//YYY - HARD CODE PACKAGE INFORMATION - END
		
		hr = VerifyForSoftwareLicenseUpgrade(&tmpPacketNewLicenseFileAttribs, true, false);
		if(FAILED(hr))
			throw hr;

		//
		//For the incoming license package, for "Current Activations" & "Current Activation Expiration Date" use current values from the 
		//system unless override is specified.
		//To determine correct values to get:
		//	If there is a existing file info on system, get values from key or system in that order
		// Else look at package and retrieve values from its key
		// This will satisfy case where package has a different validation token key then the system

		//YYY check for override later
		Lic_KeyAttribs keyAttrib;
		//customerID==0 && destinationID==0 for uninitialize LicenseFileAttribs
		bool bInitializedLicenseFileAttribs = (int(m_licenseFileAttribs.licLicenseInfoAttribs.customerID)!=0) || (int(m_licenseFileAttribs.licLicenseInfoAttribs.destinationID)!=0);
		bool bFoundKeyInfo(false);

		//try to get keyAttribs from the system
		if(bInitializedLicenseFileAttribs)
			hr = InternalGetKeyAttribs(&keyAttrib, &m_licenseFileAttribs, &bFoundKeyInfo);
		else
			hr = InternalGetKeyAttribs(&keyAttrib, bstr_t(Lic_PackageAttribsHelper::GetDisplayLabel(&tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs).c_str()), &bFoundKeyInfo);

		if(FAILED(hr) || bFoundKeyInfo == false)
		{
			hr = InternalGetKeyAttribs(&keyAttrib, &tmpPacketNewLicenseFileAttribs, &bFoundKeyInfo);
			if(FAILED(hr))
				bFoundKeyInfo = false;
		}

		//
		//CR.FIX.14842
		//For customers who enter a clock violation because of putting the system clock ahead in time (accidently in the future), then reseting the system clock back 
		//to the current time. 
		//If the License Packet has a licenseID, and it matches the License ID for a given softwareLicMgr, then reset its LastTouchDate to now, and also reset
		//the LastTouchDate on the SoftwareServerDataMgr
		//Calculate and set bResetCurrentDateToNow

		std::wstring packetLicenseCode = Lic_PackageAttribsHelper::GetValidationValue(&(tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList), Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttLicenseCode);
		std::wstring currentLicenseCode = Lic_PackageAttribsHelper::GetValidationValue(&(m_licenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList), Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttLicenseCode);

		bResetCurrentDateToNow = (_wcsicmp(packetLicenseCode.c_str(), currentLicenseCode.c_str()) == 0);
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L" SoftwareLicenseMgr::ApplyLicensePacket - packetLicenseCode: %s, currentLicenseCode: %s", packetLicenseCode.c_str(), currentLicenseCode.c_str());
//OutputDebugString(tmpbuf);
//swprintf_s(tmpbuf, 1024, L" SoftwareLicenseMgr::ApplyLicensePacket - bResetCurrentDateToNow: %s", bResetCurrentDateToNow ? L"TRUE" : L"FALSE");
//OutputDebugString(tmpbuf);

		if(bResetCurrentDateToNow)
		{
			//Set the current date to NOW, rounding down to the nearest hour.
			time_t currentTimeT = int(time(NULL) / TimeHelper::ONE_HOUR_IN_SECONDS) * TimeHelper::ONE_HOUR_IN_SECONDS;
			SYSTEMTIME tmpSystime;
			VARIANT tmpVt;
			tmpVt = TimeHelper::TimeTToVariant(currentTimeT);
			VariantTimeToSystemTime(tmpVt.date, &tmpSystime);
			wchar_t timestamp[256];
			TimeHelper::SystemTimeToString(timestamp, _countof(timestamp), tmpSystime);
			keyAttrib.currentDate = std::wstring(timestamp);
//swprintf_s(tmpbuf, 1024, L" SoftwareLicenseMgr::ApplyLicensePacket - keyAttrib.currentDate: %s", std::wstring(keyAttrib.currentDate).c_str());
//OutputDebugString(tmpbuf);

		}

////Testing...
//throw LicenseServerError::EHR_LIC_CLOCK_LIC_PACKET;

		//if keyAttrib is initialized, convert activity slots to product license expiration.
		if(bFoundKeyInfo)
		{
			Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotInfoAttribsList* pActivitySlotList = NULL;
			Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotInfoAttribsList tmpList;

			if(bInitializedLicenseFileAttribs)
			{
//OutputDebugString(L"SoftwareLicenseMgr::ApplyLicensePacket() - pActivitySlotList = &m_licenseFileAttribs.licLicenseInfoAttribs.activitySlotList - Use From Existing System");
				//Can look to m_licenseFileAttribs for activationSlot to productLicenseNumber relationship
				pActivitySlotList = &m_licenseFileAttribs.licLicenseInfoAttribs.activitySlotList;
			}
			else
			{
				//Cannot look to m_licenseFileAttribs for activationSlot to productLicenseNumber relationship.  Have to do the hard way.
				//Regenerate activationSlot to productLicenseNumber relationship by looking at history.
				//If the keyHistoryNumber and packageHistoryNumber are the same, can use slot relationship from package
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"SoftwareLicenseMgr::ApplyLicensePacket() - bInitializedLicenseFileAttribs: false, keyAttrib.historyNumber:%d, tmpPacketNewLicenseFileAttribs.historyNumber: %d", int(keyAttrib.historyNumber), int(tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.activitySlotHistoryList->size()));
//OutputDebugString(tmpbuf);

				if(keyAttrib.historyNumber == tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.activitySlotHistoryList->size())
				{
//OutputDebugString(L"SoftwareLicenseMgr::ApplyLicensePacket() - pActivitySlotList = &tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.activitySlotList - Use From New Package");
					pActivitySlotList = &tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.activitySlotList;
				}
				else
				{
					//recreate activitySlotList from using tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.activitySlotHistoryList
					//going from 0 to keyAttrib.historyNumber
					//*pActivitySlotList = tmpList;
//OutputDebugString(L"SoftwareLicenseMgr::ApplyLicensePacket() - pActivitySlotList = &tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.activitySlotList - Recreate from Key and New Package");

					//tmpList->insert
					for(int slotNumber=0; slotNumber<20; slotNumber++)
					{
						Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotInfoAttribs slotInfo;
						slotInfo.activitySlotID = slotNumber;
						tmpList->push_back(slotInfo);
					}

					unsigned short keyHistoryNumber = unsigned short(keyAttrib.historyNumber);
					for(	Lic_PackageAttribs::Lic_LicenseInfoAttribs::TVector_Lic_ActivitySlotHistoryInfoAttribsList::iterator histSlotInfoIt = tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.activitySlotHistoryList->begin();
							histSlotInfoIt != tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.activitySlotHistoryList->end();
							histSlotInfoIt++)
					{
						unsigned short slotHistoryNumber = unsigned short(histSlotInfoIt->historyNumber);
						if(slotHistoryNumber <= keyHistoryNumber)
						{
							for(Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotHistoryInfoAttribs::TVector_Lic_ActivitySlotChangeInfoAttribsList::iterator actSlotChangeIt = histSlotInfoIt->activitySlotChangeInfoList->begin();
								actSlotChangeIt != histSlotInfoIt->activitySlotChangeInfoList->end();
								actSlotChangeIt++)
							{
								switch(actSlotChangeIt->actionType)
								{
								case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs::ascaAdd:
									//Param 1 == activity slot for action ascaAdd
									tmpList->at(int(actSlotChangeIt->param1)).contractNumber = std::wstring(actSlotChangeIt->contractNumber);
									break;
								case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs::ascaDelete:
									//Param 1 == activity slot for action ascaDelete
									tmpList->at(int(actSlotChangeIt->param1)).contractNumber = std::wstring(L"");
									break;
								case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs::ascaMove:
									//Param 1 == old activity slot
									//Param 2 == new activity slot
									//Transfer values from old activity slot to the new activity slot
									tmpList->at(int(actSlotChangeIt->param2)).contractNumber = tmpList->at(int(actSlotChangeIt->param1)).contractNumber;
									tmpList->at(int(actSlotChangeIt->param1)).contractNumber = std::wstring(L"");
									break;
								case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs::ascaSetActivations:	//Don't implement
								case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs::ascaSetHoursToExpire://Don't implement
								case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs::ascaIgnore:
								default:
									break;
								}
							}
							keyAttrib.historyNumber = slotHistoryNumber;
						}
					}

					pActivitySlotList = &tmpList;
				}
			}

//YYY - For test, print out list
//			if(pActivitySlotList != NULL)
//			{
//wchar_t tmpbuf[1024];
//				for(Lic_PackageAttribs::Lic_LicenseInfoAttribs::TVector_Lic_ActivitySlotInfoAttribsList::iterator slotIt = (*pActivitySlotList)->begin();
//					slotIt != (*pActivitySlotList)->end();
//					slotIt++)
//				{
//swprintf_s(tmpbuf, 1024, L"SoftwareLicenseMgr::ApplyLicensePacket() - slot: %d, contractNumber: %s", int(slotIt->activitySlotID), std::wstring(slotIt->contractNumber).c_str());
//OutputDebugString(tmpbuf);
//				}
//			}

			unsigned short keyHistoryNumber = unsigned short(keyAttrib.historyNumber);
			//Process history number changes to key  -- if(keyAttrib.historyNumber == tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.activitySlotHistoryList->size())
			for(	Lic_PackageAttribs::Lic_LicenseInfoAttribs::TVector_Lic_ActivitySlotHistoryInfoAttribsList::iterator histSlotInfoIt = tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.activitySlotHistoryList->begin();
					histSlotInfoIt != tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.activitySlotHistoryList->end();
					histSlotInfoIt++)
			{
				unsigned short packageHistoryNumber = unsigned short(histSlotInfoIt->historyNumber);
				if(keyHistoryNumber < packageHistoryNumber)
				{
					//apply history change to keyAttrib
					for(Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotHistoryInfoAttribs::TVector_Lic_ActivitySlotChangeInfoAttribsList::iterator actSlotChangeIt = histSlotInfoIt->activitySlotChangeInfoList->begin();
						actSlotChangeIt != histSlotInfoIt->activitySlotChangeInfoList->end();
						actSlotChangeIt++)
					{
						switch(actSlotChangeIt->actionType)
						{
						case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs::ascaAdd:
						case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs::ascaDelete:
							//Param 1 == activity slot for action ascaAdd or ascaDelete
							//Clear out activity slot
							keyAttrib.activationInfoList->at(int(actSlotChangeIt->param1)).activationSlotCurrentActivation = 0;
							keyAttrib.activationInfoList->at(int(actSlotChangeIt->param1)).activationSlotHoursToExpire = 0;
							break;
						case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs::ascaMove:
							//Param 1 == old activity slot
							//Param 2 == new activity slot
							//Transfer values from old activity slot to the new activity slot
							keyAttrib.activationInfoList->at(int(actSlotChangeIt->param2)).activationSlotCurrentActivation = keyAttrib.activationInfoList->at(int(actSlotChangeIt->param1)).activationSlotCurrentActivation;
							keyAttrib.activationInfoList->at(int(actSlotChangeIt->param2)).activationSlotHoursToExpire = keyAttrib.activationInfoList->at(int(actSlotChangeIt->param1)).activationSlotHoursToExpire;
							//Clear out old activity slot
							keyAttrib.activationInfoList->at(int(actSlotChangeIt->param1)).activationSlotCurrentActivation = 0;
							keyAttrib.activationInfoList->at(int(actSlotChangeIt->param1)).activationSlotHoursToExpire = 0;
							break;
						case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs::ascaSetActivations:
							//Param 1 == activity slot
							//Param 2 == new activations
							keyAttrib.activationInfoList->at(int(actSlotChangeIt->param1)).activationSlotCurrentActivation = int(actSlotChangeIt->param2);
							break;
						case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs::ascaSetHoursToExpire:
							//Param 1 == activity slot
							//Param 2 == new hours to expire
							keyAttrib.activationInfoList->at(int(actSlotChangeIt->param1)).activationSlotHoursToExpire = int(actSlotChangeIt->param2);
							break;
						case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ActivitySlotChangeInfoAttribs::ascaIgnore:
						default:
							break;
						}
					}
					keyAttrib.historyNumber = packageHistoryNumber;
				}
			}


			if(pActivitySlotList != NULL)
			{
				//walk through all the product licenses in tmpPacketNewLicenseFileAttribs
				//	find all that are using activations and set the activations current and activation expiration date based on slot info.
				SYSTEMTIME tmpSystime;
				if(!TimeHelper::StringToSystemTime(std::wstring(SpdAttribs::WStringObj(keyAttrib.currentDate)).c_str(), tmpSystime))
					throw(E_FAIL);
				_variant_t tmpVt(NULL);	
				if (!SystemTimeToVariantTime(&tmpSystime, &tmpVt.date)) 
					throw(E_FAIL);
				time_t keyCurrentTimeT = TimeHelper::VariantToTimeT(tmpVt, false);

				for(int idx = 0;
						idx < tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->size();
						idx++)
				{
					if(tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).bUseActivations == true)
					{
						std::wstring wstrProdLicNum = std::wstring(tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).contractNumber);
						for(Lic_PackageAttribs::Lic_LicenseInfoAttribs::TVector_Lic_ActivitySlotInfoAttribsList::iterator aSlotIt = (*pActivitySlotList)->begin();
							aSlotIt != (*pActivitySlotList)->end();
							aSlotIt++)
						{
							if(_wcsicmp(wstrProdLicNum.c_str(), std::wstring(aSlotIt->contractNumber).c_str()) == 0)
							{
								time_t newExpDateTimeT = keyCurrentTimeT + (keyAttrib.activationInfoList->at(aSlotIt->activitySlotID).activationSlotHoursToExpire * TimeHelper::ONE_HOUR_IN_SECONDS);
								SYSTEMTIME tmpSystime;
								VARIANT tmpVt;
								tmpVt = TimeHelper::TimeTToVariant(newExpDateTimeT);
								VariantTimeToSystemTime(tmpVt.date, &tmpSystime);
								wchar_t timestamp[256];
								TimeHelper::SystemTimeToString(timestamp, _countof(timestamp), tmpSystime);
								if(bool(tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).bActivationOverrideCurrent) == false)
									tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).activationCurrent = keyAttrib.activationInfoList->at(aSlotIt->activitySlotID).activationSlotCurrentActivation;

								tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.productList->at(idx).activationCurrentExpirationDate = std::wstring(timestamp);
								break;
							}
						}
					}
				}
			}	//end if(pActivitySlotList != NULL)
		} //end if(bFoundKeyInfo)

		//if(bFoundKeyInfo)

//YYY - hard break
//throw E_NOTIMPL;		

		
		_bstr_t bstrHardwareKeyID = _bstr_t(L"");
		_bstr_t bstrLicenseCode = _bstr_t(L"");

		//Remove the Validation Token:ttTypePackageOnly
		for(	std::vector<Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs>::iterator valTokenIT = tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->begin();
				valTokenIT != tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->end();
				valTokenIT++)
		{
			if(valTokenIT->tokenType == Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttTypePackageOnly)
			{
				tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->erase(valTokenIT);
				break;
			}
		}

		for(	std::vector<Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs>::iterator valTokenIT = tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->begin();
				valTokenIT != tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->end();
				valTokenIT++)
		{
			switch((Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::TTokenType)valTokenIT->tokenType)
			{
				case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttHardwareKeyID:
						//OutputDebugStringW(L"    Verify - ttHardwareKeyID");
					bstrHardwareKeyID = _bstr_t(valTokenIT->tokenValue->c_str());
						//hr = VerifyHardwareKeyID(_bstr_t(valTokenIT->tokenValue->c_str()));
					break;
				case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttLicenseCode:
					bstrLicenseCode = _bstr_t(valTokenIT->tokenValue->c_str());
					//OutputDebugStringW(L"    Verify - ttLicenseCode");
					//hr = VerifyLicenseCode(_bstr_t(valTokenIT->tokenValue->c_str()));
					break;
				default:
					//OutputDebugStringW(L"    Verify - Unknown");
					break;
			}
		}

		// always make a newLicenseGUID
		_bstr_t newLicenseGUID = _bstr_t(L"");
		GUID key_guid;
		hr = CoCreateGuid(&key_guid);
		if(FAILED(hr))
			throw hr;

		//_snwprintf_s(debug_buf, _countof(debug_buf), L"SoftwareLicenseMgr::ApplyLicensePacket(), GetProtectionKeyModifiedDate(), hr: %s", LicenseServerError::GetErrorMessage(hr).c_str());
		//OutputDebugStringW(debug_buf);
		
		wchar_t tmp_code[128];
		StringFromGUID2(key_guid, tmp_code, 128);
		tmp_code[127]=0;
		newLicenseGUID = _bstr_t(tmp_code).Detach();
		
		for(	std::vector<Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_VerificationCodeAttribs>::iterator verCodeIT = m_licenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.verificationCodeHistoryList->begin();
			verCodeIT != m_licenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.verificationCodeHistoryList->end();
			verCodeIT++)
		{
			_bstr_t t = std::wstring(verCodeIT->verificationValue).c_str();
			if(_wcsicmp(std::wstring(verCodeIT->verificationValue).c_str(), bstrVerificationCode) == 0)
			{
				hr = LicenseServerError::EHR_LIC_SOFTWARE_LIC_PACKET_ALREADY_ENTERED;
				break;
			}
		}
		if(FAILED(hr))
			throw hr;

		//Add verficationCode history from current instance to this new one.
		tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.verificationCodeHistoryList = m_licenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.verificationCodeHistoryList;

		//Add new verification code to history
		Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_VerificationCodeAttribs verCode;
		verCode.verificationValue = std::wstring(SpdAttribs::WStringObj(bstrVerificationCode));
		time_t curTimeT = time(NULL);	//Retrieves Universal Time
		VARIANT vtCurTime = TimeHelper::TimeTToVariant(curTimeT);
		SYSTEMTIME curTimeSystime;
		VariantTimeToSystemTime(vtCurTime.date, &curTimeSystime);
		wchar_t curTimeStamp[256];
		TimeHelper::SystemTimeToString(curTimeStamp, sizeof(curTimeStamp)/sizeof(wchar_t), curTimeSystime);
		verCode.verificationDate = std::wstring(curTimeStamp);
		tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.verificationCodeHistoryList->push_back(verCode);

		//SysFreeString(bstrLicPacketStream);
		//bstrLicPacketStream = SysAllocString(tmpPacketNewLicenseFileAttribs.ToString().c_str());

		//if there is a newGUID, set it in the verification tokens before saving to file
		if(_wcsicmp(newLicenseGUID, L"")!=0)
		{
			for(	std::vector<Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs>::iterator valTokenIT = tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->begin();
					valTokenIT != tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->end();
					valTokenIT++)
			{
				if(valTokenIT->tokenType == Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttLicenseCode)
				{
					valTokenIT->tokenValue = std::wstring(newLicenseGUID);
					break;	//Done, leave for loop
				}
			}
		}

		SysFreeString(bstrLicPacketStream);
		bstrLicPacketStream = SysAllocString(tmpPacketNewLicenseFileAttribs.ToString().c_str());
		
//wchar_t debug_buf[1024];		
//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::ApplyLicensePacket() - %x-%x, %d", cI, sL, (int)pLicPacket->licLicenseInfoAttribs.softwareLicType);
//OutputDebugStringW(debug_buf);

//YYY - hard break
//throw E_NOTIMPL;	
		//Replace on disk
		hr = SoftwareLicenseFile::SaveToLicenseFile(m_bstrLicenseFile, bstrLicPacketStream);
		if(FAILED(hr))
			throw hr;

		//On success, see if there is a hardware key, if so find right one and update its ttLicenseCode & Modified Date
		if(_wcsicmp(newLicenseGUID, L"")!=0)
		{
			// Configure keyAttrib to have all the right info
			if(!bFoundKeyInfo)	//New license, make key info
			{
				//must create keyAttrib
				keyAttrib.historyNumber = tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.activitySlotHistoryList->size();
				keyAttrib.keyVersion = 1;
				for(unsigned short idx=0; idx<20; idx++)
				{
					Lic_KeyAttribs::Lic_ActivationInfoAttribs newLicInfoAttribs;
					newLicInfoAttribs.activationSlotId = idx;
					newLicInfoAttribs.activationSlotCurrentActivation = 0;
					newLicInfoAttribs.activationSlotHoursToExpire = 0;
					keyAttrib.activationInfoList->push_back(newLicInfoAttribs);
				}
			}

			{
			keyAttrib.licenseCode = std::wstring(newLicenseGUID);
			keyAttrib.packetCreationDate = tmpPacketNewLicenseFileAttribs.licLicenseInfoAttribs.modifiedDate;

			//Update the USB Key
			if(_wcsicmp(bstrHardwareKeyID, L"") != 0 )
			{
				m_pKeyServer->SetKeyInfoAttribs(bstrHardwareKeyID, keyAttrib, bResetCurrentDateToNow, false);
			}

			}
		}
		if(FAILED(hr))
			throw hr;
	
		m_licenseFileAttribs.InitFromString(bstrLicPacketStream);
		m_bstrLicenseName = bstr_t(Lic_PackageAttribsHelper::GetDisplayLabel(&m_licenseFileAttribs.licLicenseInfoAttribs).c_str());

//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L" SoftwareLicenseMgr::ApplyLicensePacket - m_bstrLicenseName: %s", (wchar_t*)m_bstrLicenseName);
//OutputDebugString(tmpbuf);

		if(bResetCurrentDateToNow)
		{
			//CR.FIX.14842 - Reset Clock violations
			m_pLicServerDataMgr->Touch(true);
		}

		if(m_pLicServerDataMgr!=NULL)		//Update LicenseServerDataMgr with new verification code...
		{
			Lic_ServerDataAttribs::Lic_ServerDataFileInfoAttribs tmpFileInfo;
			tmpFileInfo.LicFileName = std::wstring(SpdAttribs::WStringObj(m_bstrLicenseFile));
			tmpFileInfo.LicFileLicenseCode = std::wstring(SpdAttribs::WStringObj(newLicenseGUID));
			tmpFileInfo.LicName = std::wstring(SpdAttribs::WStringObj(Lic_PackageAttribsHelper::GetDisplayLabel(&m_licenseFileAttribs.licLicenseInfoAttribs)));
			tmpFileInfo.LicModifiedDate = m_licenseFileAttribs.licLicenseInfoAttribs.modifiedDate;
			keyAttrib.keyName = std::wstring(m_bstrLicenseName);
			tmpFileInfo.Streamed_ActivationAttribs = std::wstring(keyAttrib.ToString());
			hr = m_pLicServerDataMgr->SetFileInfoFor(std::wstring(SpdAttribs::WStringObj(tmpFileInfo.LicName)).c_str(), &tmpFileInfo);
			if(FAILED(hr))
				throw hr;
		}
	}
	catch(HRESULT &eHr)
	{
		hr = eHr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	SysFreeString(bstrLicPacketStream);
	return hr;
}

// Private - Populates the pKeyAttribs either from the USB key or the System info, uses pPackageAttribs
HRESULT SoftwareLicenseMgr::InternalGetKeyAttribs(Lic_KeyAttribs* pKeyAttribs, Lic_PackageAttribs* pPackageAttribs, bool* pBFoundKey)
{
	*pBFoundKey = false;
	HRESULT hr = E_FAIL;
	try
	{
		bool bInitializedLicenseFileAttribs = (int(pPackageAttribs->licLicenseInfoAttribs.customerID)!=0) || (int(pPackageAttribs->licLicenseInfoAttribs.destinationID)!=0);
		bool bFoundKeyInfo(false);
		if(bInitializedLicenseFileAttribs/*pPackageAttribs is INITIALIZED*/)
		{
			//There is file info on system, see if there is a key
			hr = GetKeyInfoAttribs_FromKey(pKeyAttribs, pPackageAttribs);
			//hr == E_INVALIDARG happens if there is no protection key in verification tokens, use info from file info
			*pBFoundKey = SUCCEEDED(hr);
			if(*pBFoundKey == false)
				hr = InternalGetKeyAttribs(pKeyAttribs, std::wstring(SpdAttribs::WStringObj(Lic_PackageAttribsHelper::GetDisplayLabel(&(pPackageAttribs->licLicenseInfoAttribs)))).c_str(), pBFoundKey);
		}
	}
	catch(HRESULT &eHr)
	{
		hr = eHr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	return hr;
}

// Private - Populates the pKeyAttribs either from the USB key or the System info, uses pPackageAttribs
HRESULT SoftwareLicenseMgr::InternalGetKeyAttribs(Lic_KeyAttribs* pKeyAttribs, _bstr_t bstrLicense, bool* pBFoundKey)
{
	*pBFoundKey = false;
	HRESULT hr = E_FAIL;
	try
	{
		Lic_ServerDataAttribs::Lic_ServerDataFileInfoAttribs tmpFileInfo;
		hr = m_pLicServerDataMgr->GetFileInfoFor(bstrLicense, &tmpFileInfo);
		*pBFoundKey = SUCCEEDED(hr);
		if(*pBFoundKey == true)
			pKeyAttribs->InitFromString(std::wstring(tmpFileInfo.Streamed_ActivationAttribs));
	}
	catch(HRESULT &eHr)
	{
		hr = eHr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	return hr;
}

HRESULT SoftwareLicenseMgr::EnterLicenseArchive(Lic_PackageAttribs* pLicPacket)
{
	HRESULT hr(S_OK);
	SafeMutex mutex(softwareLicenseMgrLock);
	
	BSTR bstrLicStream = NULL;
	try
	{
		bstrLicStream = SysAllocString(pLicPacket->ToString().c_str());	//Use a copy of...
		
		Lic_PackageAttribs tmpLicPacket;
		tmpLicPacket.InitFromString(bstrLicStream);
		SysFreeString(bstrLicStream);
		bstrLicStream = NULL;

		// Make sure that the packet creation date is no more than 1 day behind the current system time.
		// In truth, the current system time should always be greater that creation date, but allow 1 day window
		SYSTEMTIME packetCreateDateSystime;
		if(!TimeHelper::StringToSystemTime(std::wstring(SpdAttribs::WStringObj(tmpLicPacket.licLicenseInfoAttribs.modifiedDate)).c_str(), packetCreateDateSystime))
			throw(E_FAIL);
		_variant_t packetCreateDateVt(NULL);	
		if (!SystemTimeToVariantTime(&packetCreateDateSystime, &packetCreateDateVt.date)) 
			throw(E_FAIL);
		time_t packetCreateDateTimeT = TimeHelper::VariantToTimeT(packetCreateDateVt, false);
		time_t currentTimeDateTimeT = time(NULL);	//Retrieves Universal Time
		if(difftime(currentTimeDateTimeT, packetCreateDateTimeT) < -TimeHelper::ONE_DAY_IN_SECONDS)
			throw LicenseServerError::EHR_LIC_CLOCK_LIC_ARCHIVE;

		hr = VerifyForSoftwareLicenseUpgrade(&tmpLicPacket, true, false);
		if(FAILED(hr))
			throw hr;

		Lic_KeyAttribs keyAttrib;
		//customerID==0 && destinationID==0 for uninitialize LicenseFileAttribs
		bool bInitializedLicenseFileAttribs = (int(tmpLicPacket.licLicenseInfoAttribs.customerID)!=0) || (int(tmpLicPacket.licLicenseInfoAttribs.destinationID)!=0);
		bool bFoundKeyInfo(false);
		
		hr = InternalGetKeyAttribs(&keyAttrib, &tmpLicPacket, &bFoundKeyInfo);


		//Remove the Validation Token:ttArchiveOnly
		for(	std::vector<Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs>::iterator valTokenIT = tmpLicPacket.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->begin();
				valTokenIT != tmpLicPacket.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->end();
				valTokenIT++)
		{
			if(valTokenIT->tokenType == Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttTypeArchiveOnly)
			{
				tmpLicPacket.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->erase(valTokenIT);
				break;
			}
		}
		
		bstrLicStream = SysAllocString(tmpLicPacket.ToString().c_str());
		hr = SoftwareLicenseFile::SaveToLicenseFile(m_bstrLicenseFile, bstrLicStream);

		if(FAILED(hr))
			throw hr;

		m_licenseFileAttribs.InitFromString(bstrLicStream);
		m_bstrLicenseName = bstr_t(Lic_PackageAttribsHelper::GetDisplayLabel(&m_licenseFileAttribs.licLicenseInfoAttribs).c_str());
		SysFreeString(bstrLicStream);
		bstrLicStream = NULL;

		if(m_pLicServerDataMgr!=NULL)		//Update LicenseServerDataMgr with new verification code...
		{
			Lic_ServerDataAttribs::Lic_ServerDataFileInfoAttribs tmpFileInfo;
			tmpFileInfo.LicFileName = std::wstring(SpdAttribs::WStringObj(m_bstrLicenseFile));
			std::wstring tmpLicCode = Lic_PackageAttribsHelper::GetValidationValue(&(m_licenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList), Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttLicenseCode);
			tmpFileInfo.LicFileLicenseCode = tmpLicCode;
			tmpFileInfo.LicName = std::wstring(SpdAttribs::WStringObj(Lic_PackageAttribsHelper::GetDisplayLabel(&m_licenseFileAttribs.licLicenseInfoAttribs)));
			tmpFileInfo.LicModifiedDate = m_licenseFileAttribs.licLicenseInfoAttribs.modifiedDate;

			keyAttrib.keyName = std::wstring(m_bstrLicenseName);
			tmpFileInfo.Streamed_ActivationAttribs = std::wstring(keyAttrib.ToString());

			hr = m_pLicServerDataMgr->SetFileInfoFor(std::wstring(SpdAttribs::WStringObj(tmpFileInfo.LicName)).c_str(), &tmpFileInfo);
			if(FAILED(hr))
				throw hr;
		}
	}
	catch(HRESULT &eHr)
	{
		hr = eHr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	if(bstrLicStream)
		SysFreeString(bstrLicStream);
	
	return hr;
}

HRESULT SoftwareLicenseMgr::UpdateLicenseCode( _bstr_t _bstrNewLicenseCode)
{
	SafeMutex mutex(softwareLicenseMgrLock);
	if(_wcsicmp(_bstrNewLicenseCode, L"")!=0)
	{
		for(	std::vector<Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs>::iterator valTokenIT = m_licenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->begin();
				valTokenIT != m_licenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->end();
				valTokenIT++)
		{
			if(valTokenIT->tokenType == Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttLicenseCode)
			{
				valTokenIT->tokenValue = std::wstring(_bstrNewLicenseCode);
				break;	//Done, leave for loop
			}
		}
	}
	return InternalUpdate(&m_licenseFileAttribs, _bstrNewLicenseCode);
}

//private
HRESULT SoftwareLicenseMgr::InternalUpdate(Lic_PackageAttribs* _pLicPacAttribs, _bstr_t _newLicenseGUID)
{
	SafeMutex mutex(softwareLicenseMgrLock);
	HRESULT hr(S_OK);
	BSTR bstrLicPacketStream = NULL; 
	try
	{
		bstrLicPacketStream = SysAllocString(_pLicPacAttribs->ToString().c_str()); 
		hr = SoftwareLicenseFile::SaveToLicenseFile(m_bstrLicenseFile, bstrLicPacketStream);
		if(FAILED(hr))
			throw hr;

		m_licenseFileAttribs.InitFromString(bstrLicPacketStream);
		m_bstrLicenseName = bstr_t(Lic_PackageAttribsHelper::GetDisplayLabel(&m_licenseFileAttribs.licLicenseInfoAttribs).c_str());
		if(m_pLicServerDataMgr!=NULL)		//Update LicenseServerDataMgr with new verification code...
		{
			Lic_ServerDataAttribs::Lic_ServerDataFileInfoAttribs tmpFileInfo;
			tmpFileInfo.LicFileName = std::wstring(SpdAttribs::WStringObj(m_bstrLicenseFile));
			tmpFileInfo.LicFileLicenseCode = std::wstring(SpdAttribs::WStringObj(_newLicenseGUID));
			tmpFileInfo.LicName = std::wstring(SpdAttribs::WStringObj(Lic_PackageAttribsHelper::GetDisplayLabel(&m_licenseFileAttribs.licLicenseInfoAttribs)));
			tmpFileInfo.LicModifiedDate = m_licenseFileAttribs.licLicenseInfoAttribs.modifiedDate;
			Lic_KeyAttribs keyAttribs;
			keyAttribs.InitFromString(std::wstring(tmpFileInfo.Streamed_ActivationAttribs));
			keyAttribs.licenseCode = std::wstring(_newLicenseGUID);
			tmpFileInfo.Streamed_ActivationAttribs = std::wstring(keyAttribs.ToString());
			hr = m_pLicServerDataMgr->SetFileInfoFor(std::wstring(SpdAttribs::WStringObj(tmpFileInfo.LicName)).c_str(), &tmpFileInfo);
			if(FAILED(hr))
				throw hr;
		}
	}
	catch(HRESULT &eHr)
	{
		hr = eHr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	if(bstrLicPacketStream)
		SysFreeString(bstrLicPacketStream);
	return hr;
}

HRESULT SoftwareLicenseMgr::UseActivationToExtendTime(_bstr_t bstrContractNumber)
{
wchar_t debug_buf[1024];		
//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::UseActivationToExtendTime() - Enter");
//OutputDebugStringW(debug_buf);
	SafeMutex mutex(softwareLicenseMgrLock);
	HRESULT hr(S_OK);
	
	try
	{
		bool bInitializedLicenseFileAttribs = (int(m_licenseFileAttribs.licLicenseInfoAttribs.customerID)!=0) || (int(m_licenseFileAttribs.licLicenseInfoAttribs.destinationID)!=0);
		bool bFoundKeyInfo(false);
		Lic_KeyAttribs keyAttribs;
		//Get Keyinfo from the usb key or system
		hr = InternalGetKeyAttribs(&keyAttribs, &m_licenseFileAttribs, &bFoundKeyInfo);
		if(FAILED(hr))
			throw hr;
		if(!bFoundKeyInfo)
			throw E_INVALIDARG;

		//Removed restriction of ActivationTimes only for Licenses of type sltDisasterRecovery or sltTestDev
		//if((m_licenseFileAttribs.licLicenseInfoAttribs.softwareLicType != Lic_PackageAttribs::Lic_LicenseInfoAttribs::sltDisasterRecovery)
		//	&& (m_licenseFileAttribs.licLicenseInfoAttribs.softwareLicType != Lic_PackageAttribs::Lic_LicenseInfoAttribs::sltTestDev))
		//	throw E_INVALIDARG;	//Can only call this function on Licenses of type sltDisasterRecovery or sltTestDev

		Lic_PackageAttribs::Lic_ProductInfoAttribs* pTmpProdInfo = NULL;	//Product License that matches the contract number
		for(	Lic_PackageAttribs::Lic_LicenseInfoAttribs::TVector_Lic_ProductInfoAttribsList::iterator prodIt = m_licenseFileAttribs.licLicenseInfoAttribs.productList->begin();
				prodIt != m_licenseFileAttribs.licLicenseInfoAttribs.productList->end();
				prodIt++)
		{
			if(_wcsicmp(bstrContractNumber, std::wstring(prodIt->contractNumber).c_str()) == 0)
			{
				pTmpProdInfo = &(*prodIt);
				break;
			}
		}
		if(pTmpProdInfo == NULL || (bool)pTmpProdInfo->bUseActivations == false)	//contract number not found, or not using activations
			throw E_INVALIDARG;
		if((int)pTmpProdInfo->activationTotal <= (int)pTmpProdInfo->activationCurrent)
			throw LicenseServerError::EHR_LICENSE_INSUFFICIENT;	//Not enough licenses

		hr = VerifyForSoftwareLicenseUpgrade(&m_licenseFileAttribs, false, true);
		if(FAILED(hr))
			throw hr;

		//find activity slot number for contract number
		unsigned short activitySlot = 0;
		bool bFoundActivitySlot(false);
		for(	Lic_PackageAttribs::Lic_LicenseInfoAttribs::TVector_Lic_ActivitySlotInfoAttribsList::iterator aSlotIt = m_licenseFileAttribs.licLicenseInfoAttribs.activitySlotList->begin();
				aSlotIt != m_licenseFileAttribs.licLicenseInfoAttribs.activitySlotList->end();
				aSlotIt++)
		{
			if(_wcsicmp(bstrContractNumber, std::wstring(aSlotIt->contractNumber).c_str()) == 0)
			{
				activitySlot = unsigned short (aSlotIt->activitySlotID);
				bFoundActivitySlot = true;
				break;
			}
		}
		if(!bFoundActivitySlot)
			throw E_INVALIDARG;
		

		//
		//Added activation time to activationExpirationDate if it hasn't expired yet, else add to current time
		SYSTEMTIME currentSystime;
		GetSystemTime(&currentSystime);
//_snwprintf_s(debug_buf, 1024, L"    currentSystime(Not Used): %04d-%02d-%02d %02d:%02d:%02d.%04d", 
//				currentSystime.wYear, 
//				currentSystime.wMonth, 
//				currentSystime.wDay, 
//				currentSystime.wHour, 
//				currentSystime.wMinute,
//				currentSystime.wSecond, 
//				currentSystime.wMilliseconds);
//OutputDebugStringW(debug_buf);

		Lic_KeyAttribs::Lic_ActivationInfoAttribs* pActInfoAttribs = NULL;
		for(	Lic_KeyAttribs::TVector_Lic_ActivationInfoAttribsList::iterator actSlotIt = keyAttribs.activationInfoList->begin();
				actSlotIt != keyAttribs.activationInfoList->end();
				actSlotIt++)
		{
			if(actSlotIt->activationSlotId == activitySlot)
			{
				pActInfoAttribs = &(*actSlotIt);
				break;
			}
		}

		if(pActInfoAttribs == NULL)
			throw E_INVALIDARG;

		// Base new expiration date off the keys expiration date only
		unsigned short activationSlotHoursToExpire = unsigned short(pActInfoAttribs->activationSlotHoursToExpire);
		time_t curTimeT = time(NULL);	//Retrieves Universal Time
		SYSTEMTIME currentExpiresDateSystime;
		if(!TimeHelper::StringToSystemTime(std::wstring(SpdAttribs::WStringObj(keyAttribs.currentDate)).c_str(), currentExpiresDateSystime))
			throw(E_FAIL);

//_snwprintf_s(debug_buf, 1024, L"       CurrentExpirationDate: %04d-%02d-%02d %02d:%02d:%02d.%04d", 
//		currentExpiresDateSystime.wYear, 
//		currentExpiresDateSystime.wMonth, 
//		currentExpiresDateSystime.wDay, 
//		currentExpiresDateSystime.wHour, 
//		currentExpiresDateSystime.wMinute,
//		currentExpiresDateSystime.wSecond, 
//		currentExpiresDateSystime.wMilliseconds);
//OutputDebugStringW(debug_buf);

		_variant_t vtCurrentExpiresDate(NULL);	
		if (!SystemTimeToVariantTime(&currentExpiresDateSystime, &vtCurrentExpiresDate.date)) 
			throw(E_FAIL);
		time_t curExpiresDateTimeT = TimeHelper::VariantToTimeT(vtCurrentExpiresDate, false);
		time_t newExpiresDateTimeT = curExpiresDateTimeT + (activationSlotHoursToExpire * TimeHelper::ONE_HOUR_IN_SECONDS);

		//if activating for the first time, give an hour
		time_t newKeyExpireDateTimeT = newExpiresDateTimeT + ((int)pTmpProdInfo->activationAmountInDays * TimeHelper::ONE_DAY_IN_SECONDS) + ((activationSlotHoursToExpire==0) ? TimeHelper::ONE_HOUR_IN_SECONDS : 0);
		unsigned short newKeyHoursToExpire = activationSlotHoursToExpire + ((int)pTmpProdInfo->activationAmountInDays * TimeHelper::ONE_DAY_IN_HOURS) + ((activationSlotHoursToExpire==0) ? 1 : 0);

		//Don't let the activation expiration date go beyond the expiration date of the product license.  Calulate the difference in hours of expiration date and 
		//current date, and take the minimum between that and pActInfoAttribs->activationSlotHoursToExpire, use that for activationSlotHoursToExpire
		if(pTmpProdInfo->bUseExpirationDate == true)
		{
			SYSTEMTIME tmpSystime;
			if(!TimeHelper::StringToSystemTime(std::wstring(SpdAttribs::WStringObj(pTmpProdInfo->expirationDate)).c_str(), tmpSystime))
				throw(E_FAIL);
			_variant_t tmpVt(NULL);	
			if (!SystemTimeToVariantTime(&tmpSystime, &tmpVt.date)) 
				throw(E_FAIL);
			time_t expirationDateInSeconds_TimeT = TimeHelper::VariantToTimeT(tmpVt, false);
			if(curExpiresDateTimeT > expirationDateInSeconds_TimeT)
				throw E_FAIL; //Throw expired error

			//product license will expire before activations, force activation to expire at the same time
			if(expirationDateInSeconds_TimeT < newKeyExpireDateTimeT)
			{
				newKeyExpireDateTimeT = expirationDateInSeconds_TimeT;
				newKeyHoursToExpire = (unsigned short)((expirationDateInSeconds_TimeT-curExpiresDateTimeT) / TimeHelper::ONE_HOUR_IN_SECONDS);
			}
		}
		newExpiresDateTimeT = newKeyExpireDateTimeT;
		pActInfoAttribs->activationSlotHoursToExpire = newKeyHoursToExpire;

		VARIANT vtNewExpireTime = TimeHelper::TimeTToVariant(newExpiresDateTimeT);
		SYSTEMTIME newExpireSystime;
		if (!VariantTimeToSystemTime(vtNewExpireTime.date, &newExpireSystime)) 
			throw(E_FAIL);
		wchar_t timestamp[256];
		TimeHelper::SystemTimeToString(timestamp, sizeof(timestamp)/sizeof(wchar_t), newExpireSystime);
		//_snwprintf(timestamp, sizeof(timestamp)/sizeof(wchar_t),
		//	L"%04d-%02d-%02d %02d:%02d:%02d.%04d",
		//	newExpireSystime.wYear, 
		//	newExpireSystime.wMonth, 
		//	newExpireSystime.wDay, 
		//	newExpireSystime.wHour, 
		//	newExpireSystime.wMinute,
		//	newExpireSystime.wSecond, 
		//	newExpireSystime.wMilliseconds);

//_snwprintf_s(debug_buf, 1024, L"            newExpireSystime: %04d-%02d-%02d %02d:%02d:%02d.%04d", 
//				newExpireSystime.wYear, 
//				newExpireSystime.wMonth, 
//				newExpireSystime.wDay, 
//				newExpireSystime.wHour, 
//				newExpireSystime.wMinute,
//				newExpireSystime.wSecond, 
//				newExpireSystime.wMilliseconds);
//OutputDebugStringW(debug_buf);

		pTmpProdInfo->activationCurrentExpirationDate = std::wstring(timestamp);
		pActInfoAttribs->activationSlotCurrentActivation = (unsigned short)pActInfoAttribs->activationSlotCurrentActivation + 1;
		pTmpProdInfo->activationCurrent = (unsigned short)pActInfoAttribs->activationSlotCurrentActivation;

		// always make a newLicenseGUID
		_bstr_t newLicenseGUID = _bstr_t(L"");
		GUID key_guid;
		hr = CoCreateGuid(&key_guid);
		if(FAILED(hr))
			throw hr;
		
		wchar_t tmp_code[128];
		StringFromGUID2(key_guid, tmp_code, 128);
		tmp_code[127]=0;
		newLicenseGUID = _bstr_t(tmp_code).Detach();
		
		std::wstring wstrHardwareKeyID = Lic_PackageAttribsHelper::GetValidationValue(&(m_licenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList), Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttHardwareKeyID);

		//if there is a newGUID, set it in the verification tokens before saving to file
		if(_wcsicmp(newLicenseGUID, L"")!=0)
		{
			for(	std::vector<Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs>::iterator valTokenIT = m_licenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->begin();
					valTokenIT != m_licenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->end();
					valTokenIT++)
			{
				if(valTokenIT->tokenType == Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttLicenseCode)
				{
					valTokenIT->tokenValue = std::wstring(newLicenseGUID);
					break;	//Done, leave for loop
				}
			}
		}

		//Update the modified date
		wchar_t wchCurrentTimestamp[256];
		TimeHelper::SystemTimeToString(wchCurrentTimestamp, _countof(wchCurrentTimestamp), currentSystime);
		m_licenseFileAttribs.licLicenseInfoAttribs.modifiedDate = std::wstring(wchCurrentTimestamp);

		BSTR bstrLicPacketStream = SysAllocString(m_licenseFileAttribs.ToString().c_str());
		hr = SoftwareLicenseFile::SaveToLicenseFile(m_bstrLicenseFile, bstrLicPacketStream);
		SysFreeString(bstrLicPacketStream);
		if(FAILED(hr))
			throw hr;

		//Update the USB Key
		if(_wcsicmp(wstrHardwareKeyID.c_str(), L"") != 0 )
		{
			keyAttribs.licenseCode = std::wstring(SpdAttribs::WStringObj(newLicenseGUID));
			m_pKeyServer->SetKeyInfoAttribs(_bstr_t(wstrHardwareKeyID.c_str()), keyAttribs, true/*force current date update*/, true/*force activity slot update*/);
		}

		if(m_pLicServerDataMgr!=NULL)		//Update LicenseServerDataMgr with new verification code...
		{
			Lic_ServerDataAttribs::Lic_ServerDataFileInfoAttribs tmpFileInfo;
			tmpFileInfo.LicFileName = std::wstring(SpdAttribs::WStringObj(m_bstrLicenseFile));
			tmpFileInfo.LicFileLicenseCode = std::wstring(SpdAttribs::WStringObj(newLicenseGUID));
			tmpFileInfo.LicName = std::wstring(SpdAttribs::WStringObj(Lic_PackageAttribsHelper::GetDisplayLabel(&m_licenseFileAttribs.licLicenseInfoAttribs)));
			tmpFileInfo.LicModifiedDate = m_licenseFileAttribs.licLicenseInfoAttribs.modifiedDate;
			tmpFileInfo.Streamed_ActivationAttribs = std::wstring(keyAttribs.ToString());
			hr = m_pLicServerDataMgr->SetFileInfoFor(std::wstring(SpdAttribs::WStringObj(tmpFileInfo.LicName)).c_str(), &tmpFileInfo);
			if(FAILED(hr))
				throw hr;
		}
	}
	catch(HRESULT &eHr)
	{
		hr = eHr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}

//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::DisasterRecoveryExtendTime() - Leave - hr: 0x%08x", hr);
//OutputDebugStringW(debug_buf);
	return hr;
}
