
#include "SoftwareServerDataMgr.h"
#include "..\common\StringUtils.h"
#include "..\common\FlateHelper.h"
#include "..\common\CryptoHelper.h"
#include "..\common\CryptoAndFlateHelper.h"
#include "..\common\SafeMutex.h"
#include "..\common\TimeHelper.h"
#include "SoftwareServerInstance.h"	// For g_licenseController
#include <shlobj.h> //For SHGetFolderPath
#include <shlwapi.h> //For PathAppend
#include <math.h>			//For abs


#include <sys/stat.h>	//For _S_IWRITE that _wchmod uses



BYTE SoftwareLicenseFile::crypto_license_file_password[] = {
#include "..\common\keys\SolimarLicenseFile.password.txt"
};
BYTE SoftwareLicenseFile::crypto_license_file_public[] = {
#include "..\common\keys\SolimarLicenseFile.public.key.txt"
};
BYTE SoftwareLicenseFile::crypto_license_file_private[] = {
#include "..\common\keys\SolimarLicenseFile.private.key.txt"
};

//C9379DA6077646588738518B3EBD67AF
unsigned int SoftwareLicenseFile::license_file_code_int[] = {
	0x43, 0x00, 0x39, 0x00, 0x33, 0x00, 0x37, 0x00,	//C937
	0x39, 0x00, 0x44, 0x00, 0x41, 0x00, 0x36, 0x00, //9DA6
	0x30, 0x00, 0x37, 0x00, 0x37, 0x00, 0x36, 0x00,	//0776
	0x34, 0x00, 0x36, 0x00, 0x35, 0x00, 0x38, 0x00,	//4658
	0x38, 0x00, 0x37, 0x00, 0x33, 0x00, 0x38, 0x00,	//8738
	0x35, 0x00, 0x31, 0x00, 0x38, 0x00, 0x42, 0x00,	//518B
	0x33, 0x00, 0x45, 0x00, 0x42, 0x00, 0x44, 0x00,	//3EBD
	0x36, 0x00, 0x37, 0x00, 0x41, 0x00, 0x46, 0x00,	//67AF
	0x00, 0x00
};
//static BYTE crypto_license_file_public[];
//static BYTE crypto_license_file_private[];
//
//SoftwareServerDataMgr
//

//Public
SoftwareServerDataMgr::SoftwareServerDataMgr()
 :	pServerDataAttribs(NULL),
	softwareServerDataMgrLock(CreateMutex(0,0,0)),
	lastTouchDateTimeT(NULL)
{
}

SoftwareServerDataMgr::~SoftwareServerDataMgr()
{
	if(pServerDataAttribs)
		delete pServerDataAttribs;
	
	if (softwareServerDataMgrLock!=NULL)
		CloseHandle(softwareServerDataMgrLock);
}

HRESULT SoftwareServerDataMgr::GetAllFileInfoList(Lic_ServerDataAttribs::Lic_ServerDataFileInfoAttribsList* pFileInfoList)
{
	HRESULT hr(S_OK);
	try
	{
		SafeMutex mutex(softwareServerDataMgrLock);

		if(pServerDataAttribs == NULL)
			hr = LoadFromFile();
		if(FAILED(hr))
			throw hr;
		//pFileInfoList->InitFromString(pServerDataAttribs->fileInfoList.ToString().c_str());
		*pFileInfoList = pServerDataAttribs->fileInfoList;
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

HRESULT SoftwareServerDataMgr::GetFileInfoFor(_bstr_t bstrLicenseNameValue, Lic_ServerDataAttribs::Lic_ServerDataFileInfoAttribs* pFileInfo)
{
	HRESULT hr(S_OK);
	try
	{
		SafeMutex mutex(softwareServerDataMgrLock);
		if(pServerDataAttribs == NULL)
			hr = LoadFromFile();
		if(FAILED(hr))
			throw hr;

		hr = E_INVALIDARG;
		for(	Lic_ServerDataAttribs::TVector_Lic_ServerDataFileInfoAttribsList::iterator licSrvFileInfoListIt = pServerDataAttribs->fileInfoList->begin();
				licSrvFileInfoListIt != pServerDataAttribs->fileInfoList->end();
				licSrvFileInfoListIt++)
		{
			if(_wcsicmp(licSrvFileInfoListIt->LicName->c_str(), bstrLicenseNameValue) == 0)
			{
				pFileInfo->InitFromString(licSrvFileInfoListIt->ToString().c_str());
				hr = S_OK;
				break;
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

//Will add bstrLicenseNameValue if it doesn't already exist.
HRESULT SoftwareServerDataMgr::SetFileInfoFor(_bstr_t bstrLicenseNameValue, Lic_ServerDataAttribs::Lic_ServerDataFileInfoAttribs* pFileInfo)
{
	HRESULT hr(S_OK);
	try
	{
		SafeMutex mutex(softwareServerDataMgrLock);
		if(pServerDataAttribs == NULL)
			hr = LoadFromFile();
		if(FAILED(hr))
			throw hr;

		//Look for existing bstrLicenseNameValue to update
		bool bFound = false;
		for(	Lic_ServerDataAttribs::TVector_Lic_ServerDataFileInfoAttribsList::iterator licSrvFileInfoListIt = pServerDataAttribs->fileInfoList->begin();
				licSrvFileInfoListIt != pServerDataAttribs->fileInfoList->end();
				licSrvFileInfoListIt++)
		{
			if(_wcsicmp(licSrvFileInfoListIt->LicName->c_str(), bstrLicenseNameValue) == 0)
			{
				bFound = true;
				licSrvFileInfoListIt->InitFromString(pFileInfo->ToString().c_str());
				//	pFileInfo->InitFromString(licSrvFileInfoListIt->ToString().c_str());
				break;
			}
		}

		if(!bFound)
		{
			pServerDataAttribs->fileInfoList->push_back(*pFileInfo);
		}
		SaveToFile();
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

HRESULT SoftwareServerDataMgr::Touch()
{
	HRESULT hr(S_OK);
	try
	{
		SafeMutex mutex(softwareServerDataMgrLock);

		if(pServerDataAttribs == NULL)
			hr = LoadFromFile();
		if(FAILED(hr))
			throw hr;

		time_t timeNowTimeT = time(NULL);	//Retrieves Universal Time
		double timeDiffSeconds = difftime(timeNowTimeT, lastTouchDateTimeT);
		bool bUpdateLastTouch(false);
		bool bEnterClockViolation(false);
		bool bLeaveClockViolation(false);
		bool bLogWarning_ClockViolation(false);
		bool bSaveToFile(true);

		if(timeDiffSeconds > (60.0 * 5))
		{
			//Don't touch if have touched within last 5 minutes...
			bUpdateLastTouch = true;
			if(pServerDataAttribs->bInClockViol == true)
				bLeaveClockViolation = true;

			if(pServerDataAttribs->clockViolCount > 0)
			{
				//double timeDiffSeconds = difftime(timeNowTimeT, lastTouchDateTimeT);
				SYSTEMTIME lastClockViolationDateSystime;
				if(!TimeHelper::StringToSystemTime(std::wstring(SpdAttribs::WStringObj(pServerDataAttribs->clockViolLastDate)).c_str(), lastClockViolationDateSystime))
					throw(E_FAIL);
				_variant_t lastClockViolationDateVt(NULL);	
				if (!SystemTimeToVariantTime(&lastClockViolationDateSystime, &lastClockViolationDateVt.date)) 
					throw(E_FAIL);
				time_t lastClockViolationDateTimeT = TimeHelper::VariantToTimeT(lastClockViolationDateVt, false);
				if(difftime(timeNowTimeT, lastClockViolationDateTimeT) >= (60.0 * 60 * 24 * 60))
				{
					//if it has been 60 days since last clock violation, clear all clockViolationWarnings
					pServerDataAttribs->clockViolCount = 0;
					pServerDataAttribs->clockViolHistoryList->clear();
				}
			}
		}
		else if(timeDiffSeconds < (60.0 * -30) && timeDiffSeconds > (60.0 * 60.0 * -6))
		{
			//timeDiffSeconds is in the range greater than -6 hours, but less than -30 minutes
			bUpdateLastTouch = true;
			
			if(pServerDataAttribs->bInClockViol == false)
			{
				pServerDataAttribs->clockViolCount = pServerDataAttribs->clockViolCount + 1;

				//if current violation warnings counter is >= 3, go into violation
				if(pServerDataAttribs->clockViolCount >= 3)
					bEnterClockViolation = true;
				else
					bLogWarning_ClockViolation = true;
			}
			else if(pServerDataAttribs->bInClockViol == true)
				bLeaveClockViolation = true;
		}
		else if(timeDiffSeconds < (60.0 * 60.0 * -6))
		{
			//timeDiffSeconds is in the range less than -6 hours
			if(pServerDataAttribs->bInClockViol == false)
				bEnterClockViolation = true;

			//if not in violation, go into violation
			bUpdateLastTouch = false;
		}
		else
		{
			bSaveToFile = false;
		}


		if(bEnterClockViolation == true || bLogWarning_ClockViolation == true)
		{
			bSaveToFile = true;
			Lic_ServerDataAttribs::Lic_ClockViolationInfoAttribs clockViolationInfoAttribs;

			clockViolationInfoAttribs.fileDate = pServerDataAttribs->lastTouchDate;
			_variant_t vtCurTime;
			vtCurTime = TimeHelper::TimeTToVariant(timeNowTimeT, false);
			SYSTEMTIME curTimeSystime;
			VariantTimeToSystemTime(vtCurTime.date, &curTimeSystime);
			wchar_t curTimeStamp[256];
			TimeHelper::SystemTimeToString(curTimeStamp, sizeof(curTimeStamp)/sizeof(wchar_t), curTimeSystime);
			clockViolationInfoAttribs.systemDate = std::wstring(curTimeStamp);

			pServerDataAttribs->clockViolLastDate = std::wstring(curTimeStamp);
			pServerDataAttribs->clockViolHistoryList->push_back(clockViolationInfoAttribs);
		}
		if(bEnterClockViolation == true)
		{
			bSaveToFile = true;
			pServerDataAttribs->bInClockViol = true;
			//Cycle through all the license info objects, update so they are in violation

			g_licenseController.GenerateMessage(L"License Server", MT_ERROR, S_OK, time(0), MessageGeneric, L"Error - License Server has entered Clock Violation Period, please contact Solimar Systems, Inc. for assistance.");
		}
		else if(bLeaveClockViolation == true)
		{
			bSaveToFile = true;
			pServerDataAttribs->bInClockViol = false;

			//Cycle through all the license info objects, update so they are out of violation

			g_licenseController.GenerateMessage(L"License Server", MT_INFO, S_OK, time(0), MessageGeneric, L"Information - License Server has left Clock Violation Period");
		}

		if(bLogWarning_ClockViolation == true)	//Log to message scheme
		{
			g_licenseController.GenerateMessage(L"License Server", MT_WARNING, S_OK, time(0), MessageGeneric, L"Warning - License Server has detected a Clock Violation Warning");
		}

		if(bUpdateLastTouch == true)
		{
			_variant_t vtCurTime;
			//vtCurTime = TimeHelper::TimeTToVariant(timeNowTimeT, true);
			vtCurTime = TimeHelper::TimeTToVariant(timeNowTimeT, false);
			SYSTEMTIME curTimeSystime;
			VariantTimeToSystemTime(vtCurTime.date, &curTimeSystime);
			wchar_t curTimeStamp[256];
			TimeHelper::SystemTimeToString(curTimeStamp, sizeof(curTimeStamp)/sizeof(wchar_t), curTimeSystime);
			pServerDataAttribs->lastTouchDate = std::wstring(curTimeStamp);
			lastTouchDateTimeT = timeNowTimeT;
			bSaveToFile = true;
		}
		if(bSaveToFile == true)
		{
			hr = SaveToFile();
			if(FAILED(hr))
				throw hr;
		}

		//SYSTEMTIME currentSystime;
		//GetSystemTime(&currentSystime);
		//wchar_t timestamp[256];
		//TimeHelper::SystemTimeToString(timestamp, sizeof(timestamp)/sizeof(wchar_t), currentSystime);
		//pServerDataAttribs->lastTouchDate = std::wstring(timestamp);

		////hr = SaveToFile();
		////if(FAILED(hr))
		////	throw hr;
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



// Up to caller to free pBstrSoftwareStream
HRESULT SoftwareServerDataMgr::GetLicServerDataAttrbsStream(BSTR* pBstrSoftwareStream)
{
	HRESULT hr(S_OK);
	try
	{
		SafeMutex mutex(softwareServerDataMgrLock);
		if(pServerDataAttribs != NULL)
			*pBstrSoftwareStream = SysAllocString(pServerDataAttribs->ToString().c_str());
		else
			*pBstrSoftwareStream = SysAllocString(L"File not Found");

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
//#define GENERATE_LIC
#ifdef GENERATE_LIC
#include "SolimarSoftwareLicenseMgr.h"
HRESULT CreateNew_Lic_ServerDataFileInfoAttribs(Lic_ServerDataAttribs::Lic_ServerDataFileInfoAttribs* pLicServerData, SoftwareServerDataMgr* pSoftwareServerDataMgr)
{
	SoftwareLicenseMgr* pNewSwLicMgr;
	pNewSwLicMgr = new SoftwareLicenseMgr();
	WCHAR szPath[MAX_PATH];
	
	//Find CSIDL_COMMON_APPDATA on local system
	HRESULT hr = SHGetFolderPath( NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath );
	PathAppend(szPath, L"Solimar\\Licensing\\");

	_bstr_t newLicenseFile = _bstr_t(L"");
	GUID key_guid;
	hr = CoCreateGuid(&key_guid);
	if (FAILED(hr))
		throw hr;
	
	wchar_t tmp_code[128];
	StringFromGUID2(key_guid, tmp_code, 128);
	tmp_code[127]=0;
	newLicenseFile = _bstr_t(tmp_code).Detach();

	pNewSwLicMgr->Initialize(_bstr_t(szPath) + newLicenseFile, NULL, pSoftwareServerDataMgr);
	pLicServerData->LicFileName = std::wstring(SpdAttribs::WStringObj(_bstr_t(szPath) + newLicenseFile));
	pLicServerData->LicFileVerificationCode = std::wstring(L"{D98A3C30-02DB-43D8-AF5F-CAEC61EC076B}");
	//pLicServerData->LicFileVerificationCode = pNewSwLicMgr->
	pLicServerData->LicName = std::wstring(SpdAttribs::WStringObj(newLicenseFile));
	//softwareLicMgrMap.insert(SoftwareLicList::value_type(_bstr_t(szPath) + newLicenseFile, pSoftwareLicMgr));


	return S_OK;
}
#endif
//Private
HRESULT SoftwareServerDataMgr::LoadFromFile()
{
	HRESULT hr(S_OK);
	try
	{
		SafeMutex mutex(softwareServerDataMgrLock);
		WCHAR szPathAndFile[MAX_PATH];

		//Find CSIDL_COMMON_APPDATA on local system
		hr = SHGetFolderPath( NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPathAndFile );
		if(FAILED(hr))
			throw hr;

		//Append solimar licensing location.
		// Hide the string so it doesn't appear in the strings of the image - "Solimar\\Licensing\\SolimarLicenseServer.dat"
		BYTE fileLocByte[] = {
			0x53, 0x00, 0x6f, 0x00, 0x6c, 0x00, 0x69, 0x00, 0x6d, 0x00, 0x61, 0x00, 0x72, 0x00, 0x5c, 0x00, 0x5c, 0x00, 0x4c, 0x00, 0x69, 0x00, 0x63, 0x00, 0x65, 0x00, 0x6e, 0x00, 0x73, 0x00, 0x69, 0x00,
			0x6e, 0x00, 0x67, 0x00, 0x5c, 0x00, 0x5c, 0x00, 0x53, 0x00, 0x6f, 0x00, 0x6c, 0x00, 0x69, 0x00, 0x6d, 0x00, 0x61, 0x00, 0x72, 0x00, 0x4c, 0x00, 0x69, 0x00, 0x63, 0x00, 0x65, 0x00, 0x6e, 0x00,
			0x73, 0x00, 0x65, 0x00, 0x53, 0x00, 0x65, 0x00, 0x72, 0x00, 0x76, 0x00, 0x65, 0x00, 0x72, 0x00, 0x2e, 0x00, 0x64, 0x00, 0x61, 0x00, 0x74, 0x00, 0x00, 0x00
		};
		if(!PathAppend(szPathAndFile, (wchar_t*)fileLocByte)) 
			throw E_FAIL;

		BSTR bstrSoftwareStream = NULL;
		bool bNoFile(false);
		//bool bNoDataInFile(false);
		bool bNoPath(false);
		hr = SoftwareLicenseFile::LoadFromLicenseFile(szPathAndFile, &bstrSoftwareStream);
		if(hr == 0x80070002)	//File not Found
			bNoFile = true;
		if(hr == 0x80070003)	//Path not Found
		{
			bNoFile = true;
			bNoPath = true;
		}
		else if(FAILED(hr))
			throw hr;

		if(bNoPath)
		{
			WCHAR szCreatePathAndFile[MAX_PATH];
			hr = SHGetFolderPath( NULL, CSIDL_COMMON_APPDATA, NULL, 0, szCreatePathAndFile );
			if(FAILED(hr))
				throw hr;
			PathAppend(szCreatePathAndFile, L"Solimar");
			if(PathFileExists(szCreatePathAndFile) == false)
				CreateDirectory(szCreatePathAndFile, 0);
			PathAppend(szCreatePathAndFile, L"Licensing");
			if(PathFileExists(szCreatePathAndFile) == false)
				CreateDirectory(szCreatePathAndFile, 0);
		}

		if(pServerDataAttribs == NULL)
			pServerDataAttribs = new Lic_ServerDataAttribs();

		if(bstrSoftwareStream != NULL)
		{
			pServerDataAttribs->InitFromString(bstrSoftwareStream);
			
			SYSTEMTIME lastTouchDateSystime;
			if(!TimeHelper::StringToSystemTime(std::wstring(SpdAttribs::WStringObj(pServerDataAttribs->lastTouchDate)).c_str(), lastTouchDateSystime))
				throw(E_FAIL);
			_variant_t vtLastTouchExpiresDate(NULL);	
			if (!SystemTimeToVariantTime(&lastTouchDateSystime, &vtLastTouchExpiresDate.date)) 
				throw(E_FAIL);
			lastTouchDateTimeT = TimeHelper::VariantToTimeT(vtLastTouchExpiresDate, false);

			SysFreeString(bstrSoftwareStream);	//Clean up
		}

		if(bNoFile)
		{
			#ifdef GENERATE_LIC
			//Initialize For Testing...
			Lic_ServerDataAttribs::Lic_ServerDataFileInfoAttribs newLicServerData;
			if(SUCCEEDED(CreateNew_Lic_ServerDataFileInfoAttribs(&newLicServerData, NULL)))
				pServerDataAttribs->fileInfoList->push_back(newLicServerData);
			#endif

			SaveToFile();
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
HRESULT SoftwareServerDataMgr::SaveToFile()
{
	HRESULT hr(S_OK);
	try
	{
		SafeMutex mutex(softwareServerDataMgrLock);
		if(pServerDataAttribs == NULL)
			throw E_FAIL;

		WCHAR szPathAndFile[MAX_PATH];

		//Find CSIDL_COMMON_APPDATA on local system
		hr = SHGetFolderPath( NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPathAndFile );
		if(FAILED(hr))
			throw hr;

		//Append solimar licensing location.
		// Hide the string so it doesn't appear in the strings of the image - "Solimar\\Licensing\\SolimarLicenseServer.dat"
		BYTE fileLocByte[] = {
			0x53, 0x00, 0x6f, 0x00, 0x6c, 0x00, 0x69, 0x00, 0x6d, 0x00, 0x61, 0x00, 0x72, 0x00, 0x5c, 0x00, 0x5c, 0x00, 0x4c, 0x00, 0x69, 0x00, 0x63, 0x00, 0x65, 0x00, 0x6e, 0x00, 0x73, 0x00, 0x69, 0x00,
			0x6e, 0x00, 0x67, 0x00, 0x5c, 0x00, 0x5c, 0x00, 0x53, 0x00, 0x6f, 0x00, 0x6c, 0x00, 0x69, 0x00, 0x6d, 0x00, 0x61, 0x00, 0x72, 0x00, 0x4c, 0x00, 0x69, 0x00, 0x63, 0x00, 0x65, 0x00, 0x6e, 0x00,
			0x73, 0x00, 0x65, 0x00, 0x53, 0x00, 0x65, 0x00, 0x72, 0x00, 0x76, 0x00, 0x65, 0x00, 0x72, 0x00, 0x2e, 0x00, 0x64, 0x00, 0x61, 0x00, 0x74, 0x00, 0x00, 0x00
		};
		if(!PathAppend(szPathAndFile, (wchar_t*)fileLocByte)) 
			throw E_FAIL;

		BSTR bstrLicPacketStream = SysAllocString(pServerDataAttribs->ToString().c_str());
		hr = SoftwareLicenseFile::SaveToLicenseFile(szPathAndFile, bstrLicPacketStream);
		SysFreeString(bstrLicPacketStream);
		if(FAILED(hr))
			throw hr;
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



//
// class SoftwareLicenseFile
//


//static
HRESULT SoftwareLicenseFile::SaveToLicenseFile(_bstr_t licenseFileName, BSTR bstrSoftwareStream)
{
//	SafeMutex mutex(softwareLicenseFileLock);
	HRESULT hr(S_OK);
	HANDLE hFile(INVALID_HANDLE_VALUE); 
	
	try
	{
		//change from read-only file mode
		//		returns -1 and errno == ENOENT if file can't be found, doesn't matter, continue
      _wchmod(licenseFileName, _S_IWRITE);
		
		hFile = CreateFileW(
			licenseFileName, 
			GENERIC_WRITE | GENERIC_READ,	//DWORD dwDesiredAccess,
			0,	//DWORD dwShareMode,
			NULL,				//LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			OPEN_ALWAYS,	//DWORD dwCreationDisposition,
			FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_READONLY,	//DWORD dwFlagsAndAttributes,
			NULL);			//HANDLE hTemplateFile
		if (hFile == INVALID_HANDLE_VALUE) 
		{
			throw HRESULT_FROM_WIN32(GetLastError());
		}


		//New Code
		VARIANT vtByteArray;
		VariantInit(&vtByteArray);
		_bstr_t bstr_license_data_code;
		bstr_license_data_code = (wchar_t*)(&license_file_code_int[0]);
		for(int idx=1; idx<32; idx++)
			bstr_license_data_code += (wchar_t*)(&license_file_code_int[2*idx]);
		hr = CryptoAndFlateHelper::StreamToEncryptCompressByteArray(bstr_license_data_code, crypto_license_file_private, sizeof(crypto_license_file_private)/sizeof(BYTE), crypto_license_file_password, sizeof(crypto_license_file_password)/sizeof(BYTE), bstrSoftwareStream, &vtByteArray);
		if(FAILED(hr))
			throw hr;

		BYTE *pSAData=0;
		if (vtByteArray.vt!=(VT_ARRAY | VT_UI1) || vtByteArray.parray==0)
			throw E_INVALIDARG;

		hr = SafeArrayAccessData(vtByteArray.parray, (void**)&pSAData);
		if(FAILED(hr))
			throw hr;

		DWORD bytesWritten;
		if( !WriteFile(
							hFile,
							pSAData,
							vtByteArray.parray->rgsabound[0].cElements,
							&bytesWritten,
							NULL) )
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
			CloseHandle(hFile);
			SafeArrayUnaccessData(vtByteArray.parray);
			throw hr;
		}

		//truncate whatever data was aready in the file.
		if( !SetEndOfFile(hFile))
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
			CloseHandle(hFile);
			throw hr;
		}

		SafeArrayUnaccessData(vtByteArray.parray);
		VariantClear(&vtByteArray);
		CloseHandle(hFile);
	}
	catch(HRESULT &eHr)
	{
		hr = eHr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	//change to read-only file mode
	//		returns -1 and errno == ENOENT if file can't be found, doesn't matter, continue
   _wchmod(licenseFileName, _S_IREAD);


	return hr;
}

//static
HRESULT SoftwareLicenseFile::LoadFromLicenseFile(_bstr_t licenseFileName, BSTR* pBstrSoftwareStream)
{
//	GetLastAccessTime
//wchar_t debug_buf[1024];
//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseFile::LoadFromLicenseFile() - Enter - licenseFileName: %s", (wchar_t*)licenseFileName);
//OutputDebugStringW(debug_buf);	


//	SafeMutex mutex(softwareLicenseFileLock);
	HRESULT hr(S_OK);
	HANDLE hFile(INVALID_HANDLE_VALUE);

	try
	{
		hFile = CreateFileW(
			licenseFileName, 
			GENERIC_READ,	//DWORD dwDesiredAccess,
			0,	//DWORD dwShareMode,
			NULL,				//LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			OPEN_EXISTING,	//DWORD dwCreationDisposition,
			0,					//DWORD dwFlagsAndAttributes,
			NULL);			//HANDLE hTemplateFile
		if (hFile == INVALID_HANDLE_VALUE) 
		{
			throw HRESULT_FROM_WIN32(GetLastError());
		}

		char *buffer;
		DWORD bw, buffersize;
		buffersize = GetFileSize(hFile, NULL);
		if (buffersize==INVALID_FILE_SIZE)
		{
			CloseHandle(hFile);
			throw E_ACCESSDENIED;
		}

		if (buffersize==0)
		{
			CloseHandle(hFile);
			throw S_OK;
		}
		buffer=new char[buffersize+1];

		if (!ReadFile(hFile, buffer, buffersize, &bw, NULL))
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
			CloseHandle(hFile);
			delete [] buffer;
			throw hr;
		}
		CloseHandle(hFile);
		buffer[buffersize]='\0';


		VARIANT vtByteArray;
		VariantInit(&vtByteArray);

		// package the string in to a variant
		vtByteArray.vt = VT_ARRAY | VT_UI1;
		vtByteArray.parray = SafeArrayCreateVector(VT_UI1, 0, (buffersize)*sizeof(char));

		BYTE *pPacketData = 0;
		if (FAILED(hr = SafeArrayAccessData(vtByteArray.parray, (void**)&pPacketData))) 
		{
			VariantClear(&vtByteArray);
			delete [] buffer;
			throw hr;
		}
		memcpy(pPacketData, buffer, (buffersize)*sizeof(char));
		SafeArrayUnaccessData(vtByteArray.parray);


		_bstr_t bstr_license_data_code;
		bstr_license_data_code = (wchar_t*)(&license_file_code_int[0]);
		for(int idx=1; idx<32; idx++)
			bstr_license_data_code += (wchar_t*)(&license_file_code_int[2*idx]);
		hr = CryptoAndFlateHelper::EncryptCompressByteArrayToStream(bstr_license_data_code, crypto_license_file_public, sizeof(crypto_license_file_public)/sizeof(BYTE), crypto_license_file_password, sizeof(crypto_license_file_password)/sizeof(BYTE), vtByteArray, pBstrSoftwareStream);
		if(FAILED(hr))
		{
			VariantClear(&vtByteArray);
			delete [] buffer;
			throw hr;
		}

		VariantClear(&vtByteArray);
		delete [] buffer;
	}
	catch(HRESULT &eHr)
	{
		hr = eHr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
//_snwprintf_s(debug_buf, 1024, L"SoftwareLicenseFile::LoadFromLicenseFile() - Leave - licenseFileName: %s", (wchar_t*)*pBstrSoftwareStream);
//OutputDebugStringW(debug_buf);	
	return hr;
}

//static
HRESULT SoftwareLicenseFile::DeleteLicenseFile(_bstr_t licenseFileName)
{
	HRESULT hr(S_OK);
	try
	{
		//Remove readonly attribute or will get access denied.
		_wchmod(licenseFileName, _S_IWRITE);
		if(!DeleteFile(licenseFileName))
			throw HRESULT_FROM_WIN32(GetLastError());
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
