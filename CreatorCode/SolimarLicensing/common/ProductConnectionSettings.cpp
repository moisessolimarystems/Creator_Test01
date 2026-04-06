#include "ProductConnectionSettings.h"
#include "..\common\StringUtils.h"
#include <WinDef.h>
#include <shlobj.h> //For SHGetFolderPath
#include <shlwapi.h> //For PathAppend
#include <map>
#include <list>
//
// Same logic from ConnectionSettingsHelper.cs to retrive the installed products and read the config file.
// code is converted from C# to C++

// Hide the string so it doesn't appear in the strings of the image - "Software\\Solimar"
const byte ProductConnectionSettings::RegKey_SolimarByte[] = { 0x53, 0x00, 0x6f, 0x00, 0x66, 0x00, 0x74, 0x00, 0x77, 0x00, 0x61, 0x00, 0x72, 0x00, 0x65, 0x00, 0x5c, 0x00, 0x5c, 0x00, 0x53, 0x00, 0x6f, 0x00, 0x6c, 0x00, 0x69, 0x00, 0x6d, 0x00, 0x61, 0x00, 0x72, 0x00, 0x00, 0x00 };

// Hide the string so it doesn't appear in the strings of the image - "Software\\Wow6432Node\\Solimar"
const byte ProductConnectionSettings::RegKey_Solimar32Byte[] = { 0x53, 0x00, 0x6f, 0x00, 0x66, 0x00, 0x74, 0x00, 0x77, 0x00, 0x61, 0x00, 0x72, 0x00, 0x65, 0x00, 0x5c, 0x00, 0x5c, 0x00, 0x57, 0x00, 0x6f, 0x00, 0x77, 0x00, 0x36, 0x00, 0x34, 0x00, 0x33, 0x00, 
0x32, 0x00, 0x4e, 0x00, 0x6f, 0x00, 0x64, 0x00, 0x65, 0x00, 0x5c, 0x00, 0x5c, 0x00, 0x53, 0x00, 0x6f, 0x00, 0x6c, 0x00, 0x69, 0x00, 0x6d, 0x00, 0x61, 0x00, 0x72, 0x00, 0x00, 0x00 };

// Hide the string so it doesn't appear in the strings of the image - "Solimar Licensing"
const byte ProductConnectionSettings::RegKey_ProductLicensingByte[] = { 0x53, 0x00, 0x6f, 0x00, 0x6c, 0x00, 0x69, 0x00, 0x6d, 0x00, 0x61, 0x00, 0x72, 0x00, 0x20, 0x00, 0x4c, 0x00, 0x69, 0x00, 0x63, 0x00, 0x65, 0x00, 0x6e, 0x00, 0x73, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x67, 0x00, 0x00, 0x00 };

// Hide the string so it doesn't appear in the strings of the image - "SOLSearcher"
const byte ProductConnectionSettings::RegKey_SseByte[] = { 0x53, 0x00, 0x4f, 0x00, 0x4c, 0x00, 0x53, 0x00, 0x65, 0x00, 0x61, 0x00, 0x72, 0x00, 0x63, 0x00, 0x68, 0x00, 0x65, 0x00, 0x72, 0x00, 0x00, 0x00 };

// Hide the string so it doesn't appear in the strings of the image - "InstallInfo"
const byte ProductConnectionSettings::RegKey_SseInstallInfoByte[] = { 0x49, 0x00, 0x6e, 0x00, 0x73, 0x00, 0x74, 0x00, 0x61, 0x00, 0x6c, 0x00, 0x6c, 0x00, 0x49, 0x00, 0x6e, 0x00, 0x66, 0x00, 0x6f, 0x00, 0x00, 0x00 };

// Hide the string so it doesn't appear in the strings of the image - "ProductCode_4"
const byte ProductConnectionSettings::RegKey_SseInstallInfoSseByte[] = { 0x50, 0x00, 0x72, 0x00, 0x6f, 0x00, 0x64, 0x00, 0x75, 0x00, 0x63, 0x00, 0x74, 0x00, 0x43, 0x00, 0x6f, 0x00, 0x64, 0x00, 0x65, 0x00, 0x5f, 0x00, 0x34, 0x00, 0x00, 0x00 };

// Hide the string so it doesn't appear in the strings of the image - "ProductCode_7"
const byte ProductConnectionSettings::RegKey_SseInstallInfoSseSpByte[] = { 0x50, 0x00, 0x72, 0x00, 0x6f, 0x00, 0x64, 0x00, 0x75, 0x00, 0x63, 0x00, 0x74, 0x00, 0x43, 0x00, 0x6f, 0x00, 0x64, 0x00, 0x65, 0x00, 0x5f, 0x00, 0x37, 0x00, 0x00, 0x00 };

// Hide the string so it doesn't appear in the strings of the image - "ProductCode_54"
const byte ProductConnectionSettings::RegKey_SseInstallInfoSse64Byte[] = { 0x50, 0x00, 0x72, 0x00, 0x6f, 0x00, 0x64, 0x00, 0x75, 0x00, 0x63, 0x00, 0x74, 0x00, 0x43, 0x00, 0x6f, 0x00, 0x64, 0x00, 0x65, 0x00, 0x5f, 0x00, 0x35, 0x00, 0x34, 0x00, 0x00, 0x00 };

// Hide the string so it doesn't appear in the strings of the image - "ProductCode_57"
const byte ProductConnectionSettings::RegKey_SseInstallInfoSseSp64Byte[] = { 0x50, 0x00, 0x72, 0x00, 0x6f, 0x00, 0x64, 0x00, 0x75, 0x00, 0x63, 0x00, 0x74, 0x00, 0x43, 0x00, 0x6f, 0x00, 0x64, 0x00, 0x65, 0x00, 0x5f, 0x00, 0x35, 0x00, 0x37, 0x00, 0x00, 0x00 };

// static
HRESULT ProductConnectionSettings::GetClientSettingsAttribs(Lic_ClientSettingsAttribs* pClientSettingsAttrib, Lic_PackageAttribs::Lic_SoftwareSpecAttribs* pSoftwareSpec)
{
OutputDebugStringW(L"ProductConnectionSettings::GetClientSettingsAttribs()");
	HRESULT hr(S_OK);

	HKEY root = HKEY_LOCAL_MACHINE;

	std::map<int, bool> installedProductIdMap;
	HKEY regkey;
	long result;
	wchar_t wcKey[MAX_PATH];
	
	std::list<_bstr_t> regKeyList;
	regKeyList.push_back((wchar_t*)ProductConnectionSettings::RegKey_SolimarByte);
	regKeyList.push_back((wchar_t*)ProductConnectionSettings::RegKey_Solimar32Byte);

	for(	std::list<_bstr_t>::iterator regKeyListIt = regKeyList.begin();
			regKeyListIt != regKeyList.end();
			regKeyListIt++)
	{
		result = RegOpenKeyExW(
			root,
			*regKeyListIt,
			0,
			KEY_READ | KEY_EXECUTE | KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS,
			&regkey);

		if (result==ERROR_SUCCESS)
		{
			int index = 0;
			while (RegEnumKeyW(regkey, index, wcKey, MAX_PATH) == (DWORD)ERROR_SUCCESS) 
			{
				if (wcscmp(wcKey, (wchar_t*)ProductConnectionSettings::RegKey_ProductLicensingByte) != 0)
				{
					int productId = -1;
					if (SUCCEEDED(GetProductID(wcKey, &productId, pSoftwareSpec)))
					{
						installedProductIdMap[productId] = true;
						if (productId == Lic_PackageAttribs::pid_Rubika)
							installedProductIdMap[Lic_PackageAttribs::pid_RubikaProcessBuilder] = true;
						else if (productId == Lic_PackageAttribs::pid_Spde)
							installedProductIdMap[Lic_PackageAttribs::pid_SpdeQueueManager] = true;
						else if (productId == Lic_PackageAttribs::pid_SdxDesigner)
							installedProductIdMap[Lic_PackageAttribs::pid_SolIndexer] = true;
					}
					else if (wcscmp(wcKey, (wchar_t*)ProductConnectionSettings::RegKey_SseByte) == 0)
					{
						if (IsSseSystemManagerInstalled(*regKeyListIt))
							installedProductIdMap[Lic_PackageAttribs::pid_SdxDesigner] = true;
					}
				}
				index++;
			}
			RegCloseKey(regkey);
		}
	}

	// 
	// installedProductIdMap is a map of the installed products on the system
	WCHAR szPathAndFile[MAX_PATH];
	try
	{
		//Find CSIDL_COMMON_APPDATA on local system
		hr = SHGetFolderPath( NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPathAndFile );
		if(FAILED(hr))
			throw hr;

		//Append solimar licensing location.
		// Hide the string so it doesn't appear in the strings of the image - "Solimar Systems\\Solimar Licensing\\SolimarLicenseServer\\LocalConnectionSettings.xml"
		BYTE fileLocByte[] = {
			0x53, 0x00, 0x6f, 0x00, 0x6c, 0x00, 0x69, 0x00, 0x6d, 0x00, 0x61, 0x00, 0x72, 0x00, 0x20, 0x00, 0x53, 0x00, 0x79, 0x00, 0x73, 0x00, 0x74, 0x00, 0x65, 0x00, 0x6d, 0x00, 0x73, 0x00, 0x5c, 0x00,
			0x5c, 0x00, 0x53, 0x00, 0x6f, 0x00, 0x6c, 0x00, 0x69, 0x00, 0x6d, 0x00, 0x61, 0x00, 0x72, 0x00, 0x20, 0x00, 0x4c, 0x00, 0x69, 0x00, 0x63, 0x00, 0x65, 0x00, 0x6e, 0x00, 0x73, 0x00, 0x69, 0x00,
			0x6e, 0x00, 0x67, 0x00, 0x5c, 0x00, 0x5c, 0x00, 0x53, 0x00, 0x6f, 0x00, 0x6c, 0x00, 0x69, 0x00, 0x6d, 0x00, 0x61, 0x00, 0x72, 0x00, 0x4c, 0x00, 0x69, 0x00, 0x63, 0x00, 0x65, 0x00, 0x6e, 0x00,
			0x73, 0x00, 0x65, 0x00, 0x53, 0x00, 0x65, 0x00, 0x72, 0x00, 0x76, 0x00, 0x65, 0x00, 0x72, 0x00, 0x5c, 0x00, 0x5c, 0x00, 0x4c, 0x00, 0x6f, 0x00, 0x63, 0x00, 0x61, 0x00, 0x6c, 0x00, 0x43, 0x00,
			0x6f, 0x00, 0x6e, 0x00, 0x6e, 0x00, 0x65, 0x00, 0x63, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x53, 0x00, 0x65, 0x00, 0x74, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x67, 0x00,
			0x73, 0x00, 0x2e, 0x00, 0x78, 0x00, 0x6d, 0x00, 0x6c, 0x00, 0x00, 0x00
		};
		if(!PathAppend(szPathAndFile, (wchar_t*)fileLocByte))
			throw E_FAIL;

		HANDLE hFile(INVALID_HANDLE_VALUE);
		hFile = CreateFileW(
			szPathAndFile, 
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

		// Hide the string so it doesn't appear in the strings of the image - "Version"
		byte tagVersionByte[] = { 0x56, 0x00, 0x65, 0x00, 0x72, 0x00, 0x73, 0x00, 0x69, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x00, 0x00 };

		// Hide the string so it doesn't appear in the strings of the image - "2.0"
		byte dataVersionByte[] = { 0x32, 0x00, 0x2e, 0x00, 0x30, 0x00, 0x00, 0x00 };
		std::wstring wsVersionData;

		// Hide the string so it doesn't appear in the strings of the image - "value"
		byte elementValueByte[] = { 0x76, 0x00, 0x61, 0x00, 0x6c, 0x00, 0x75, 0x00, 0x65, 0x00, 0x00, 0x00 };
		std::wstring wsElementValueData;

		std::wstring wsBuffer = StringUtils::StringToWstring(buffer);
		delete [] buffer;

		bool bFoundVersion2 = false;
		if (FindDataForTags((wchar_t*)tagVersionByte, wsBuffer, wsVersionData) != std::wstring::npos)
		{
			if (FindValueInData((wchar_t*)elementValueByte, wsVersionData, wsElementValueData) != std::wstring::npos)
				bFoundVersion2 = wcscmp(wsElementValueData.c_str(), (wchar_t*)dataVersionByte) == 0;
		}

		if (bFoundVersion2)
		{
			// XML looks like this, repeated
			//	<Product id="18">
			//		<Server name="qalicense" backup="" testDev="0" />
			//	</Product>

			// Hide the string so it doesn't appear in the strings of the image - "Product"
			byte tagProductByte[] = { 0x50, 0x00, 0x72, 0x00, 0x6f, 0x00, 0x64, 0x00, 0x75, 0x00, 0x63, 0x00, 0x74, 0x00, 0x00, 0x00 };

			// Hide the string so it doesn't appear in the strings of the image - "id"
			byte elementIdByte[] = { 0x69, 0x00, 0x64, 0x00, 0x00, 0x00 };

			// Hide the string so it doesn't appear in the strings of the image - "Server"
			byte tagServerByte[] = { 0x53, 0x00, 0x65, 0x00, 0x72, 0x00, 0x76, 0x00, 0x65, 0x00, 0x72, 0x00, 0x00, 0x00 };

			// Hide the string so it doesn't appear in the strings of the image - "name"
			byte elementNameByte[] = { 0x6e, 0x00, 0x61, 0x00, 0x6d, 0x00, 0x65, 0x00, 0x00, 0x00 };

			// Hide the string so it doesn't appear in the strings of the image - "backup"
			byte elementBackupByte[] = { 0x62, 0x00, 0x61, 0x00, 0x63, 0x00, 0x6b, 0x00, 0x75, 0x00, 0x70, 0x00, 0x00, 0x00 };

			// Hide the string so it doesn't appear in the strings of the image - "testDev"
			byte elementTestDevByte[] = { 0x74, 0x00, 0x65, 0x00, 0x73, 0x00, 0x74, 0x00, 0x44, 0x00, 0x65, 0x00, 0x76, 0x00, 0x00, 0x00 };

			std::wstring wsProductData;
			std::wstring wsElementData;

			size_t stCurrentPos(0);
			std::wstring wsTagProduct = std::wstring((wchar_t*)tagProductByte);
			stCurrentPos = FindDataForTags(wsTagProduct, wsBuffer, wsProductData);
			if (stCurrentPos != std::wstring::npos)
				stCurrentPos += wsTagProduct.length();
			while (stCurrentPos != std::wstring::npos)
			{
				Lic_ClientSettingsAttribs::Lic_ProductConnectionAttribs prodConnectionAttrib;

				// Found a product, get all the info out of the xml in wsProductData
				if (FindValueInData((wchar_t*)elementIdByte, wsProductData, wsElementData) != std::wstring::npos)
				{
					if (wsElementData.length() > 0)
						prodConnectionAttrib.productID= _wtoi(wsElementData.c_str());
					OutputDebugStringW(wsElementData.c_str());
				}
				if (FindValueInData((wchar_t*)elementNameByte, wsProductData, wsElementData) != std::wstring::npos)
				{
					prodConnectionAttrib.serverName = wsElementData;
					OutputDebugStringW(wsElementData.c_str());
				}
				if (FindValueInData((wchar_t*)elementBackupByte, wsProductData, wsElementData) != std::wstring::npos)
				{
					prodConnectionAttrib.backupServerName = wsElementData;
					OutputDebugStringW(wsElementData.c_str());
				}
				if (FindValueInData((wchar_t*)elementTestDevByte, wsProductData, wsElementData) != std::wstring::npos)
				{
					prodConnectionAttrib.bTestDev = (wcscmp(wsElementData.c_str(), L"1") == 0);
					OutputDebugStringW(wsElementData.c_str());
				}

				if (prodConnectionAttrib.productID != 0)
					prodConnectionAttrib.bInstalled = installedProductIdMap[prodConnectionAttrib.productID];

				pClientSettingsAttrib->prodConnList->push_back(prodConnectionAttrib);

				// Cycle to the next product
				stCurrentPos = FindDataForTags(wsTagProduct, wsBuffer, wsProductData, false, stCurrentPos);
				if (stCurrentPos != std::wstring::npos)
					stCurrentPos += wsTagProduct.length();
			}

			//size_t stEndPos(0);
		}
	}
	catch (HRESULT)
	{
	}


	return S_OK;
}


// static
bool ProductConnectionSettings::IsSseSystemManagerInstalled(_bstr_t regKey)
{
	// CR.13130 - Detect in SSE or SSE Single Plateform is on the system, add SDX Designer to be configured
	bool bSseInstalled = false;
	wchar_t regKeyQuery[1024];
	swprintf_s(regKeyQuery, 
		L"%s\\%s\\%s",
		regKey,
		(wchar_t*)ProductConnectionSettings::RegKey_SseByte,
		(wchar_t*)ProductConnectionSettings::RegKey_SseInstallInfoByte
		);
	HKEY regkey;
	if (RegOpenKeyExW(
		HKEY_LOCAL_MACHINE,
		regKey,
		0,
		KEY_READ | KEY_EXECUTE | KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS,
		&regkey)==ERROR_SUCCESS)
	{
		wchar_t wcKey[MAX_PATH];
		int index = 0;
		while (!bSseInstalled && RegEnumKeyW(regkey, index, wcKey, MAX_PATH) == (DWORD)ERROR_SUCCESS) 
		{
			if (wcscmp(wcKey, (wchar_t*)ProductConnectionSettings::RegKey_SseInstallInfoSseByte) == 0)
				bSseInstalled = true;
			else if (wcscmp(wcKey, (wchar_t*)ProductConnectionSettings::RegKey_SseInstallInfoSseSpByte) == 0)
				bSseInstalled = true;
			else if (wcscmp(wcKey, (wchar_t*)ProductConnectionSettings::RegKey_SseInstallInfoSse64Byte) == 0)
				bSseInstalled = true;
			else if (wcscmp(wcKey, (wchar_t*)ProductConnectionSettings::RegKey_SseInstallInfoSseSp64Byte) == 0)
				bSseInstalled = true;
			index++;
		}
		RegCloseKey(regkey);
	}
	return bSseInstalled;
}

// static
HRESULT ProductConnectionSettings::GetProductID(_bstr_t productName, int* pProductId, Lic_PackageAttribs::Lic_SoftwareSpecAttribs* pSoftwareSpec)
{
	HRESULT hr(E_INVALIDARG);
	*pProductId = -1;
	for(Lic_PackageAttribs::Lic_SoftwareSpecAttribs::TMap_Lic_ProductSoftwareSpecAttribsMap::iterator prodSpecMapIt = pSoftwareSpec->productSpecMap->begin();
		prodSpecMapIt != pSoftwareSpec->productSpecMap->end();
		prodSpecMapIt++)
	{
		if (wcscmp(prodSpecMapIt->second.productName->c_str(), productName) == 0)
		{
			hr = S_OK;
			*pProductId = prodSpecMapIt->second.productID;
			break;
		}
	}
	return hr;
}

size_t ProductConnectionSettings::FindDataForTags(
	const std::wstring &wsTagId,
	const std::wstring &wsData,
	std::wstring &wsOutVal,
	bool bKeepTags,
	size_t stSearchStartPos
	)
{
	size_t stRetVal(std::wstring::npos);
	std::wstring wsTokenTag(wsTagId + std::wstring(L">"));
	std::wstring wsSingleElementTokenTag(std::wstring(L"/>"));
	//std::wstring wsOpenTag(std::wstring(L"<") + wsTokenTag);
	std::wstring wsOpenTag(std::wstring(L"<") + wsTagId + std::wstring(L" "));
	size_t stStartPos(stSearchStartPos);
	size_t stEndPos(stSearchStartPos);
	bool bSingleElement = false;
	DWORD dwTagCount(0);
	//
	// clear output
	//
	wsOutVal.clear();

	stStartPos = stRetVal = wsData.find(wsOpenTag, stSearchStartPos );

	if (stStartPos != std::wstring::npos)
	{  //
		// found the open tag.
		// Adjust the search position and increment the tag count semaphore.
		//
		stEndPos = stStartPos + 1 ; // add one  because wsOpenTag.length() == wsTokenTag.length() + 1
		dwTagCount++ ;

		while (dwTagCount)
		{
			stEndPos = wsData.find(wsTokenTag, stEndPos + wsTokenTag.length());
			if (stEndPos == std::wstring::npos)
			{
				if (dwTagCount == 1)
				{
					// No ending tags, look for '/>'
					stEndPos = wsData.find(wsSingleElementTokenTag, stEndPos + wsSingleElementTokenTag.length());
					bSingleElement = true;
					dwTagCount--;
				}
				else
				{  //
					// no more tags found.
					// break out of the loop.
					//
					break ;
				}
			}
			else
			{
				if (wsData[stEndPos-1] == L'<' )
				{  //
					// this is an opening tag, increment semaphore
					//
					dwTagCount ++ ;
				}
				else if (wsData[stEndPos-1] == L'/')
				{  //
					// This is a closing tag, decrement semaphore
					//
					dwTagCount--;
				}
				else
				{  //
					// This is just data that looks like a tag!
					//
				}
			}
		} // end of tag search loop.

		if (dwTagCount)
		{  //
			// problem: xml is not clean.  could not match all opening tags with
			// closing tags.
			//
			stRetVal = std::wstring::npos;
		}
		else
		{  //
			// semaphore is zero.
			// A matching close tag has been found for the first open tag.
			// Copy data between tags to output buffer.
			//
			if (bKeepTags)
			{  //
				// keeping tags...
				// add the tag length to the end position.
				//
				stEndPos += wsTokenTag.length();
			}
			else
			{  //
				// not keeping tags...
				// Add the tag length to the start position.
				//
				stStartPos += wsOpenTag.length();
				if (!bSingleElement)
					stEndPos -= 2 ; // sub 2 because token tag length == close tag length - 2
			}
			//
			// populate output buffer.
			// 
			wsOutVal = wsData.substr(stStartPos, stEndPos - stStartPos);
		}
	}
	return stRetVal ;
}

// static
size_t ProductConnectionSettings::FindValueInData(
	const std::wstring &wsElement,
	const std::wstring &wsData,
	std::wstring &wsOutVal
	)
{
	size_t stRetVal(std::wstring::npos);

	size_t stStartPos(0);
	size_t stEndPos(0);
	std::wstring wsFindElementStart(wsElement + std::wstring(L"=\""));
	std::wstring wsFindElementEnd(L"\"");

	//
	// clear output
	//
	wsOutVal.clear();

	stStartPos = wsData.find(wsFindElementStart, 0 );
	if (stStartPos != std::wstring::npos)
	{
		stStartPos += wsFindElementStart.length();
		stEndPos = wsData.find(wsFindElementEnd, stStartPos);
		if (stStartPos != std::wstring::npos)
		{
			stRetVal = stStartPos;
			wsOutVal = wsData.substr(stStartPos, stEndPos - stStartPos);
		}

	}

	return stRetVal;
}
