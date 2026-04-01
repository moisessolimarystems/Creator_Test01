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
	m_pRainbowKeyDriver(NULL)
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

HRESULT SoftwareLicenseMgr::Initialize(_bstr_t bstrLicenseFile, RainbowDriver* pRainbowKeyDriver, SoftwareServerDataMgr* pLicServerDataMgr)
{
	SafeMutex mutex(softwareLicenseMgrLock);
	m_bstrLicenseFile = bstrLicenseFile;
//wchar_t debug_buf[1024];
//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::Initialize() m_bstrLicenseFile: %s, m_pRainbowKeyDriver: 0x%08x", (wchar_t*)m_bstrLicenseFile, m_pRainbowKeyDriver);
//OutputDebugStringW(debug_buf);
	m_pRainbowKeyDriver = pRainbowKeyDriver;
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
				SYSTEMTIME activationExpirationDateSystime;
				if(!TimeHelper::StringToSystemTime(std::wstring(SpdAttribs::WStringObj(m_licenseFileAttribs.licLicenseInfoAttribs.activationExpirationDate)).c_str(), activationExpirationDateSystime))
					throw(E_FAIL);
				
				_variant_t vtActivationExpirationDate(NULL);	
				if (!SystemTimeToVariantTime(&activationExpirationDateSystime, &vtActivationExpirationDate.date)) 
					throw(E_FAIL);
				time_t activationExpirationDateTimeT = TimeHelper::VariantToTimeT(vtActivationExpirationDate, false);
				time_t emptyExpiresDateTimeT = time_t(-1);

				// ignore expiration date if it equals 1900/1/1
				if(activationExpirationDateTimeT != emptyExpiresDateTimeT)
				{
					// only add licensing to cache if the current date is below the expiration date
					time_t currentTimeDateTimeT = time(NULL);	//Retrieves Universal Time
					if(currentTimeDateTimeT > activationExpirationDateTimeT)
					{
						throw(LicenseServerError::EHR_LIC_SOFTWARE_LIC_FILE_EXPIRED);
					}
				}


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
						case Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttCompuerName:
							//OutputDebugStringW(L"    Validate - ttCompuerName");
							hr = ValidateHardwareCompuerName(_bstr_t(valTokenIT->tokenValue->c_str()));
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
//wchar_t debug_buf[1024];

	if(m_pRainbowKeyDriver != NULL)
	{
		SafeMutex mutex1(m_pRainbowKeyDriver->keys_lock);
		m_pRainbowKeyDriver->RefreshKeyList();

		KeySpec keyspec;

		for (RainbowDriver::KeyList::iterator keyIt = m_pRainbowKeyDriver->keys.begin(); keyIt!=m_pRainbowKeyDriver->keys.end(); keyIt++)
		{
//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::VerifyHardwareKeyID() keyIt->first: %s, bstrValidationValue: %s", (wchar_t*)keyIt->first, (wchar_t*)bstrValidationValue);
//OutputDebugStringW(debug_buf);
			if(_wcsicmp(keyIt->first, bstrValidationValue) == 0)
			{
				hr = S_OK;
				break;
			}
		}
	}
	return hr;
}
HRESULT SoftwareLicenseMgr::ValidateLicenseCode(_bstr_t bstrValidationValue, bool bCheckProtectionKey)
{
	HRESULT hr(LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_LICENSE_CODE);
//wchar_t debug_buf[1024];
	SafeMutex mutex(softwareLicenseMgrLock);

	//Determine if the license code should be on the protection key, or on the system.
	bool bVerifyOnProtectionKey(false);
	for(	std::vector<Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs>::iterator valTokenIT = m_licenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->begin();
			valTokenIT != m_licenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->end();
			valTokenIT++)
	{
		if((Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::TTokenType)valTokenIT->tokenType == Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttHardwareKeyID)
		{
			bVerifyOnProtectionKey = true;
			break;
		}
	}

	if(bVerifyOnProtectionKey || bCheckProtectionKey)	//Look for license code on protection key
	{
		if(m_pRainbowKeyDriver != NULL)
		{
			SafeMutex mutex1(m_pRainbowKeyDriver->keys_lock);
			m_pRainbowKeyDriver->RefreshKeyList();

			for (RainbowDriver::KeyList::iterator keyIt = m_pRainbowKeyDriver->keys.begin(); keyIt!=m_pRainbowKeyDriver->keys.end(); keyIt++)
			{
				//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::VerifyLicenseCode() keyIt->first: %s", (wchar_t*)keyIt->first);
				//OutputDebugStringW(debug_buf);

				BSTR bstrKeyCode;
				hr = m_pRainbowKeyDriver->GetSoftwareKeyCode(keyIt->first, &bstrKeyCode);
				if(SUCCEEDED(hr))
				{
					//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::VerifyLicenseCode() bstrKeyCode: %s, bstrValidationValue: %s", (wchar_t*)bstrKeyCode, (wchar_t*)bstrValidationValue);
					//OutputDebugStringW(debug_buf);
					if(_wcsicmp(bstrKeyCode, bstrValidationValue) == 0)
						hr = S_OK;
					else
						hr = LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_LICENSE_CODE;

					SysFreeString(bstrKeyCode);
				}
				if(SUCCEEDED(hr))	//Found key code, leave...
					break;
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
				if(_wcsicmp(bstr_t(tmpFileInfo.LicFileVerificationCode->c_str()), bstrValidationValue) == 0)
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
	HRESULT hr(LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_BIOS_SERIALNUMBER);
	SafeMutex mutex(softwareLicenseMgrLock);

//wchar_t debug_buf[1024];
//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::VerifyHardwareBiosSerialNumber() bstrValidationValue: %s, ThreadID: %d", (wchar_t*)bstrValidationValue, GetCurrentThreadId());
//OutputDebugStringW(debug_buf);
	WMIHelper::ResultsType BiosList;
	WMIHelper wmi(L"\\\\localhost\\root\\cimv2");
	hr = wmi.Connect();

	if(SUCCEEDED(hr))
	{
		// gather processor information
		hr = wmi.ExecuteQuery(L"SELECT SerialNumber FROM Win32_BIOS", BiosList);
		if(SUCCEEDED(hr))
		{
			if (BiosList.size()>0)
			{
				for (unsigned int idx=0; idx<BiosList.size(); ++idx)
				{
					VARIANT* pvtBiosSerialNumber = &BiosList[idx][L"SerialNumber"];
					if (pvtBiosSerialNumber->vt == VT_BSTR)
					{
						if(_wcsicmp(pvtBiosSerialNumber->bstrVal, bstrValidationValue) == 0)
							hr = S_OK;
						else
							hr = LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_BIOS_SERIALNUMBER;
//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::VerifyHardwareBiosSerialNumber() biosSerialNumber: %s", (wchar_t*)pvtBiosSerialNumber->bstrVal);
//OutputDebugStringW(debug_buf);
					}
				}
				WMIHelper::Clear(BiosList);
				BiosList.clear();
			}
		}
	}

	if (FAILED(hr))
	{
		
		//Maybe set hr to LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_PROCESSOR_ID, think about
		//If do reset error, log existing one before changing it...
	}
	

	return hr;
}
HRESULT SoftwareLicenseMgr::ValidateHardwareMacAddress(_bstr_t bstrValidationValue)
{
	HRESULT hr(LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_MAC_ADDRESS);
	SafeMutex mutex(softwareLicenseMgrLock);

//wchar_t debug_buf[1024];
	WMIHelper::ResultsType NetworkAdaptersList;
	WMIHelper wmi(L"\\\\localhost\\root\\cimv2");
	hr = wmi.Connect();

	if(SUCCEEDED(hr))
	{
		// gather processor information
		hr = wmi.ExecuteQuery(L"SELECT MACAddress FROM Win32_NetworkAdapterConfiguration Where IPEnabled = True", NetworkAdaptersList);
		if(SUCCEEDED(hr))
		{
			if (NetworkAdaptersList.size()>0)
			{
				for (unsigned int idx=0; idx<NetworkAdaptersList.size(); ++idx)
				{
					VARIANT* pvtMacAddress = &NetworkAdaptersList[idx][L"MACAddress"];
					if (pvtMacAddress->vt == VT_BSTR)
					{
						if(_wcsicmp(pvtMacAddress->bstrVal, bstrValidationValue) == 0)
							hr = S_OK;
						else
							hr = LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_MAC_ADDRESS;

						if(SUCCEEDED(hr))
							break;
//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::VerifyHardwareBiosSerialNumber() pvtMacAddress: %s", (wchar_t*)pvtMacAddress->bstrVal);
//OutputDebugStringW(debug_buf);
					}
				}
				WMIHelper::Clear(NetworkAdaptersList);
				NetworkAdaptersList.clear();
			}
		}
	}

	if (FAILED(hr))
	{
		
		//Maybe set hr to LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_MAC_ADDRESS, think about
		//If do reset error, log existing one before changing it...
	}

	return hr;
}
HRESULT SoftwareLicenseMgr::ValidateHardwareCompuerName(_bstr_t bstrValidationValue)
{
	HRESULT hr(LicenseServerError::EHR_LIC_SOFTWARE_VALIDATION_FAILED_COMPUTER_NAME);
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
	tmpVerTokenAttribs3.tokenType = Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttCompuerName;
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

static const time_t SECONDS_PER_DAY = (time_t)(24*60*60); //24hours * 60 mins * 60seconds
Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribsAddOn1;
tmpModAttribsAddOn1.moduleID = 400;	//ConcurrentOperator
tmpModAttribsAddOn1.moduleValue = 10;	//Unlimited @ 255
tmpModAttribsAddOn1.moduleAppInstance = 4;
time_t curTimeT = time(NULL);	//Retrieves Universal Time
VARIANT vtCurTime = TimeHelper::TimeTToVariant(curTimeT);
SYSTEMTIME curTimeSystime;
VariantTimeToSystemTime(vtCurTime.date, &curTimeSystime);
wchar_t curTimeStamp[256];
TimeHelper::SystemTimeToString(curTimeStamp, sizeof(curTimeStamp)/sizeof(wchar_t), curTimeSystime);
tmpModAttribsAddOn1.moduleExpirationDate = std::wstring(curTimeStamp);
tmpProdAttribs.moduleList->push_back(tmpModAttribsAddOn1);

Lic_PackageAttribs::Lic_ModuleInfoAttribs tmpModAttribsAddOn2;
tmpModAttribsAddOn2.moduleID = 400;	//ConcurrentOperator
tmpModAttribsAddOn2.moduleValue = 100;	//Unlimited @ 255
tmpModAttribsAddOn2.moduleAppInstance = 4;
curTimeT = time(NULL);	//Retrieves Universal Time
VARIANT vtAddOnTime = TimeHelper::TimeTToVariant(curTimeT+(1*SECONDS_PER_DAY));
SYSTEMTIME addOnTimeSystime;
VariantTimeToSystemTime(vtAddOnTime.date, &addOnTimeSystime);
wchar_t addOnTimeStamp[256];
TimeHelper::SystemTimeToString(addOnTimeStamp, sizeof(addOnTimeStamp)/sizeof(wchar_t), addOnTimeSystime);
tmpModAttribsAddOn2.moduleExpirationDate = std::wstring(addOnTimeStamp);
tmpProdAttribs.moduleList->push_back(tmpModAttribsAddOn2);




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
HRESULT SoftwareLicenseMgr::GetProtectionKeyModifiedDate(time_t* pTimeTModifiedDate)
{
	SafeMutex mutex(softwareLicenseMgrLock);
	return GetProtectionKeyModifiedDate(pTimeTModifiedDate, &m_licenseFileAttribs);
}
HRESULT SoftwareLicenseMgr::GetProtectionKeyModifiedDate(time_t* pTimeTModifiedDate, Lic_PackageAttribs* pLicenseFileAttribs)
{
//wchar_t debug_buf[1024];
//_snwprintf_s(debug_buf, _countof(debug_buf), L"SoftwareLicenseMgr::GetProtectionKeyModifiedDate()");
//OutputDebugStringW(debug_buf);
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
		//Find the key, query for the modified date
		if(m_pRainbowKeyDriver != NULL)
		{
			hr = m_pRainbowKeyDriver->GetSoftwareModifiedDate(keyID, pTimeTModifiedDate);
		}
	}
	return hr;
}

HRESULT SoftwareLicenseMgr::GetProtectionKeyCurrentActivations(int* pCurrentActivations, Lic_PackageAttribs* pLicenseFileAttribs)
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
		//Find the key, query for the modified date
		if(m_pRainbowKeyDriver != NULL)
		{
			hr = m_pRainbowKeyDriver->GetSoftwareCurrentActivations(keyID, (unsigned short *)pCurrentActivations);
		}
	}
//wchar_t debug_buf[1024];
//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::GetProtectionKeyCurrentActivations() - leave pCurrentActivations: %d, hr: 0x%08x, %s", *pCurrentActivations, hr, LicenseServerError::GetErrorMessage(hr).c_str());
//OutputDebugStringW(debug_buf);
	return hr;
}
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
			//find modified date from key
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
		Lic_PackageAttribs tmpLicenseFileAttribs;
		tmpLicenseFileAttribs.InitFromString(bstrLicPacketStream);

		hr = VerifyForSoftwareLicenseUpgrade(&tmpLicenseFileAttribs, true, false);
		if(FAILED(hr))
			throw hr;

		unsigned short currentActivations(0);

		currentActivations = (int)tmpLicenseFileAttribs.licLicenseInfoAttribs.activationCurrent;

		//If license packet activations current is not 0, use it, else use value from the current system.
		if(currentActivations == 0)	
		{
			hr = GetProtectionKeyCurrentActivations((int*)&currentActivations, &tmpLicenseFileAttribs);

			//hr == E_INVALIDARG happens if there is no protection key in verification tokens, use activations from license server data mgr
			if(hr == E_INVALIDARG)
			{
				Lic_ServerDataAttribs::Lic_ServerDataFileInfoAttribs tmpFileInfo;
				hr = m_pLicServerDataMgr->GetFileInfoFor(std::wstring(SpdAttribs::WStringObj(Lic_PackageAttribsHelper::GetDisplayLabel(&m_licenseFileAttribs.licLicenseInfoAttribs))).c_str(), &tmpFileInfo);
				if(SUCCEEDED(hr))
					currentActivations = (int)tmpFileInfo.LicCurrentActivations;
			}
		}

		tmpLicenseFileAttribs.licLicenseInfoAttribs.activationCurrent = currentActivations;

		_bstr_t bstrHardwareKeyID = _bstr_t(L"");
		_bstr_t bstrLicenseCode = _bstr_t(L"");
		//Remove the Validation Token:ttTypePackageOnly
		for(	std::vector<Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs>::iterator valTokenIT = tmpLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->begin();
				valTokenIT != tmpLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->end();
				valTokenIT++)
		{
			if(valTokenIT->tokenType == Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs::ttTypePackageOnly)
			{
				tmpLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->erase(valTokenIT);
				break;
			}
		}

		for(	std::vector<Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs>::iterator valTokenIT = tmpLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->begin();
				valTokenIT != tmpLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->end();
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
		tmpLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.verificationCodeHistoryList = m_licenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.verificationCodeHistoryList;

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
		tmpLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.verificationCodeHistoryList->push_back(verCode);

		//SysFreeString(bstrLicPacketStream);
		//bstrLicPacketStream = SysAllocString(tmpLicenseFileAttribs.ToString().c_str());


		//if there is a newGUID, set it in the verification tokens before saving to file
		if(_wcsicmp(newLicenseGUID, L"")!=0)
		{
			for(	std::vector<Lic_PackageAttribs::Lic_LicenseInfoAttribs::Lic_ValidationTokenAttribs>::iterator valTokenIT = tmpLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->begin();
					valTokenIT != tmpLicenseFileAttribs.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList->end();
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
		bstrLicPacketStream = SysAllocString(tmpLicenseFileAttribs.ToString().c_str());
		
//wchar_t debug_buf[1024];		
//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::ApplyLicensePacket() - %x-%x, %d", cI, sL, (int)pLicPacket->licLicenseInfoAttribs.softwareLicType);
//OutputDebugStringW(debug_buf);
		
		hr = SoftwareLicenseFile::SaveToLicenseFile(m_bstrLicenseFile, bstrLicPacketStream);
		if(FAILED(hr))
			throw hr;

		//On success, see if there is a hardware key, if so find right one and update its ttLicenseCode & Modified Date
		if(_wcsicmp(newLicenseGUID, L"")!=0)
		{
			SafeMutex mutex1(m_pRainbowKeyDriver->keys_lock);
			m_pRainbowKeyDriver->RefreshKeyList();
			for (RainbowDriver::KeyList::iterator keyIt = m_pRainbowKeyDriver->keys.begin(); keyIt!=m_pRainbowKeyDriver->keys.end(); keyIt++)
			{
				if(_wcsicmp(keyIt->first, bstrHardwareKeyID) == 0)
				{
					hr = m_pRainbowKeyDriver->SetSoftwareKeyCode(keyIt->first, newLicenseGUID);

					if(SUCCEEDED(hr))
					{
						SYSTEMTIME modifiedDateSystime;
						if(!TimeHelper::StringToSystemTime(std::wstring(SpdAttribs::WStringObj(tmpLicenseFileAttribs.licLicenseInfoAttribs.modifiedDate)).c_str(), modifiedDateSystime))
							throw(E_FAIL);

						_variant_t vtModifiedDate(NULL);	
						if (!SystemTimeToVariantTime(&modifiedDateSystime, &vtModifiedDate.date)) 
							throw(E_FAIL);

						time_t modTimeT = TimeHelper::VariantToTimeT(vtModifiedDate, false);
						hr = m_pRainbowKeyDriver->SetSoftwareModifiedDate(keyIt->first, modTimeT);

						hr = m_pRainbowKeyDriver->SetSoftwareCurrentActivations(keyIt->first, (unsigned short)tmpLicenseFileAttribs.licLicenseInfoAttribs.activationCurrent);
					}
					break;
				}
			}
		}
		if(FAILED(hr))
			throw hr;
	
		m_licenseFileAttribs.InitFromString(bstrLicPacketStream);
		m_bstrLicenseName = bstr_t(Lic_PackageAttribsHelper::GetDisplayLabel(&m_licenseFileAttribs.licLicenseInfoAttribs).c_str());
		if(m_pLicServerDataMgr!=NULL)		//Update LicenseServerDataMgr with new verification code...
		{
			Lic_ServerDataAttribs::Lic_ServerDataFileInfoAttribs tmpFileInfo;
			tmpFileInfo.LicFileName = std::wstring(SpdAttribs::WStringObj(m_bstrLicenseFile));
			tmpFileInfo.LicFileVerificationCode = std::wstring(SpdAttribs::WStringObj(newLicenseGUID));
			tmpFileInfo.LicName = std::wstring(SpdAttribs::WStringObj(Lic_PackageAttribsHelper::GetDisplayLabel(&m_licenseFileAttribs.licLicenseInfoAttribs)));
			tmpFileInfo.LicModifiedDate = m_licenseFileAttribs.licLicenseInfoAttribs.modifiedDate;
			tmpFileInfo.LicCurrentActivations = m_licenseFileAttribs.licLicenseInfoAttribs.activationCurrent;
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

		hr = VerifyForSoftwareLicenseUpgrade(&tmpLicPacket, true, false);
		if(FAILED(hr))
			throw hr;

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
		
		//
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
			tmpFileInfo.LicFileVerificationCode = tmpLicCode;
			tmpFileInfo.LicName = std::wstring(SpdAttribs::WStringObj(Lic_PackageAttribsHelper::GetDisplayLabel(&m_licenseFileAttribs.licLicenseInfoAttribs)));
			tmpFileInfo.LicModifiedDate = m_licenseFileAttribs.licLicenseInfoAttribs.modifiedDate;
			tmpFileInfo.LicCurrentActivations = m_licenseFileAttribs.licLicenseInfoAttribs.activationCurrent;
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
			tmpFileInfo.LicFileVerificationCode = std::wstring(SpdAttribs::WStringObj(_newLicenseGUID));
			tmpFileInfo.LicName = std::wstring(SpdAttribs::WStringObj(Lic_PackageAttribsHelper::GetDisplayLabel(&m_licenseFileAttribs.licLicenseInfoAttribs)));
			tmpFileInfo.LicModifiedDate = m_licenseFileAttribs.licLicenseInfoAttribs.modifiedDate;
			tmpFileInfo.LicCurrentActivations = m_licenseFileAttribs.licLicenseInfoAttribs.activationCurrent;
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

HRESULT SoftwareLicenseMgr::UseActivationToExtendTime()
{
//wchar_t debug_buf[1024];		
//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseMgr::UseActivationToExtendTime() - Enter");
//OutputDebugStringW(debug_buf);
	SafeMutex mutex(softwareLicenseMgrLock);
	HRESULT hr(S_OK);
	
	try
	{
		//Removed restriction of ActivationTimes only for Licenses of type sltDisasterRecovery or sltTestDev
		//if((m_licenseFileAttribs.licLicenseInfoAttribs.softwareLicType != Lic_PackageAttribs::Lic_LicenseInfoAttribs::sltDisasterRecovery)
		//	&& (m_licenseFileAttribs.licLicenseInfoAttribs.softwareLicType != Lic_PackageAttribs::Lic_LicenseInfoAttribs::sltTestDev))
		//	throw E_INVALIDARG;	//Can only call this function on Licenses of type sltDisasterRecovery or sltTestDev

		if((int)m_licenseFileAttribs.licLicenseInfoAttribs.activationTotal <= (int)m_licenseFileAttribs.licLicenseInfoAttribs.activationCurrent)
			throw LicenseServerError::EHR_LICENSE_INSUFFICIENT;	//Not enough licenses


		hr = VerifyForSoftwareLicenseUpgrade(&m_licenseFileAttribs, false, true);
		if(FAILED(hr))
			throw hr;

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

		time_t curTimeT = time(NULL);	//Retrieves Universal Time

		SYSTEMTIME currentExpiresDateSystime;
		if(!TimeHelper::StringToSystemTime(std::wstring(SpdAttribs::WStringObj(m_licenseFileAttribs.licLicenseInfoAttribs.activationExpirationDate)).c_str(), currentExpiresDateSystime))
			throw(E_FAIL);

//_snwprintf_s(debug_buf, 1024, L"       CurrentExpirationDate: %04d-%02d-%02d %02d:%02d:%02d.%04d", 
//				currentExpiresDateSystime.wYear, 
//				currentExpiresDateSystime.wMonth, 
//				currentExpiresDateSystime.wDay, 
//				currentExpiresDateSystime.wHour, 
//				currentExpiresDateSystime.wMinute,
//				currentExpiresDateSystime.wSecond, 
//				currentExpiresDateSystime.wMilliseconds);
//OutputDebugStringW(debug_buf);

		_variant_t vtCurrentExpiresDate(NULL);	
		if (!SystemTimeToVariantTime(&currentExpiresDateSystime, &vtCurrentExpiresDate.date)) 
			throw(E_FAIL);
		time_t curExpiresDateTimeT = TimeHelper::VariantToTimeT(vtCurrentExpiresDate, false);

		// Base new expiration date off of the larger or newExpiresDateTimeT & curTimeT
		time_t newExpiresDateTimeT = (difftime(curExpiresDateTimeT, curTimeT) > 0) ? curExpiresDateTimeT : curTimeT;

		static const time_t SECONDS_PER_DAY = (time_t)(24*60*60); //24hours * 60 mins * 60seconds
		newExpiresDateTimeT += (int)m_licenseFileAttribs.licLicenseInfoAttribs.activationAmountInDays * SECONDS_PER_DAY;

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

		
		//SystemTime is in universal time, change to local before writing to file...
		//struct tm localTimeTM;
		//errno_t err = _localtime64_s( &localTimeTM, &newExpiresDateTimeT ); 
		//if (err)
		//	throw(E_FAIL);
	
		//wchar_t timestamp[256];
		//_snwprintf(timestamp, sizeof(timestamp)/sizeof(wchar_t),
		//	L"%04d-%02d-%02d %02d:%02d:%02d.%04d",
		//	localTimeTM.tm_year + 1900, 
		//	localTimeTM.tm_mon + 1, 
		//	localTimeTM.tm_mday, 
		//	localTimeTM.tm_hour, 
		//	localTimeTM.tm_min,
		//	localTimeTM.tm_sec, 
		//	0000);

		
//_snwprintf_s(debug_buf, 1024, L"    NewExpirationDate: %s", (wchar_t*)timestamp);
//OutputDebugStringW(debug_buf);

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


		m_licenseFileAttribs.licLicenseInfoAttribs.activationExpirationDate = std::wstring(timestamp);
		m_licenseFileAttribs.licLicenseInfoAttribs.activationCurrent = (int)m_licenseFileAttribs.licLicenseInfoAttribs.activationCurrent + 1;

		//Update the modified date
		wchar_t wchCurrentTimestamp[256];
		TimeHelper::SystemTimeToString(wchCurrentTimestamp, _countof(wchCurrentTimestamp), currentSystime);
		m_licenseFileAttribs.licLicenseInfoAttribs.modifiedDate = std::wstring(wchCurrentTimestamp);

		BSTR bstrLicPacketStream = SysAllocString(m_licenseFileAttribs.ToString().c_str());
		hr = SoftwareLicenseFile::SaveToLicenseFile(m_bstrLicenseFile, bstrLicPacketStream);
		SysFreeString(bstrLicPacketStream);
		if(FAILED(hr))
			throw hr;

		//On success, see if there is a hardware key, if so find right one and update its ttLicenseCode.
		if(_wcsicmp(wstrHardwareKeyID.c_str(), L"")!=0)
		{
			SafeMutex mutex1(m_pRainbowKeyDriver->keys_lock);
			m_pRainbowKeyDriver->RefreshKeyList();
			for (RainbowDriver::KeyList::iterator keyIt = m_pRainbowKeyDriver->keys.begin(); keyIt!=m_pRainbowKeyDriver->keys.end(); keyIt++)
			{
				if(_wcsicmp(keyIt->first, wstrHardwareKeyID.c_str()) == 0)
				{
					hr = m_pRainbowKeyDriver->SetSoftwareKeyCode(keyIt->first, newLicenseGUID);
					if(SUCCEEDED(hr))
					{
						SYSTEMTIME modifiedDateSystime;
						if(!TimeHelper::StringToSystemTime(std::wstring(SpdAttribs::WStringObj(m_licenseFileAttribs.licLicenseInfoAttribs.modifiedDate)).c_str(), modifiedDateSystime))
							throw(E_FAIL);

						_variant_t vtModifiedDate(NULL);	
						if (!SystemTimeToVariantTime(&modifiedDateSystime, &vtModifiedDate.date)) 
							throw(E_FAIL);

						time_t modTimeT = TimeHelper::VariantToTimeT(vtModifiedDate, false);
						hr = m_pRainbowKeyDriver->SetSoftwareModifiedDate(keyIt->first, modTimeT);

						hr = m_pRainbowKeyDriver->SetSoftwareCurrentActivations(keyIt->first, (unsigned short)m_licenseFileAttribs.licLicenseInfoAttribs.activationCurrent);
					}
					break;
				}
			}
		}
		if(FAILED(hr))
			throw hr;

		if(m_pLicServerDataMgr!=NULL)		//Update LicenseServerDataMgr with new verification code...
		{
			Lic_ServerDataAttribs::Lic_ServerDataFileInfoAttribs tmpFileInfo;
			tmpFileInfo.LicFileName = std::wstring(SpdAttribs::WStringObj(m_bstrLicenseFile));
			tmpFileInfo.LicFileVerificationCode = std::wstring(SpdAttribs::WStringObj(newLicenseGUID));
			tmpFileInfo.LicName = std::wstring(SpdAttribs::WStringObj(Lic_PackageAttribsHelper::GetDisplayLabel(&m_licenseFileAttribs.licLicenseInfoAttribs)));
			tmpFileInfo.LicModifiedDate = m_licenseFileAttribs.licLicenseInfoAttribs.modifiedDate;
			tmpFileInfo.LicCurrentActivations = m_licenseFileAttribs.licLicenseInfoAttribs.activationCurrent;
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
/*
HRESULT SoftwareLicenseMgr::GetLicenseStream(BSTR* bstrLicenseStream)
{
	return E_NOTIMPL;
}
HRESULT SoftwareLicenseMgr::GetSoftwareSpecStream(BSTR* bstrSoftwareStream)
{
	return E_NOTIMPL;
}


HRESULT ApplyLicensePacket(LicensePacket* pLicPacket, BSTR *pBstrVerificationCode)
{
	return E_NOTIMPL;
}
*/


//static
//HANDLE SoftwareLicenseMgr::SoftwareLicenseFile::softwareLicenseFileLock = CreateMutex(0,0,0);
//static




