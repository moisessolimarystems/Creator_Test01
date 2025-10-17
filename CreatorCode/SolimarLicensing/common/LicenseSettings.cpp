

#include "LicenseSettings.h"
#include "..\common\SafeMutex.h"
#include "..\common\LicAttribsCPP\Lic_PackageAttribs.h"
#include <shlwapi.h>	//For PathAppend


HRESULT LicenseSettings::Initialize()
{

	HRESULT hr(E_FAIL);
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"LicenseSettings::Initialize() - Enter");
//OutputDebugString(tmpbuf); 

	for(;;)
	{
		SafeMutex mutex(m_xmlReaderMutex);
		//Find CSIDL_COMMON_APPDATA on local system
		WCHAR szPath[MAX_PATH];
		hr = SHGetFolderPath( NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath );
		if(FAILED(hr))	
			break;

		// CR.FIX.20830 - Use "CSIDL_COMMON_APPDATA\\Solimar Systems\Solimar Licensing\SolimarLicenseServer" instead of "CSIDL_COMMON_APPDATA\\Solimar\SolimarLicenseServer".
		// Check for "CSIDL_COMMON_APPDATA\\Solimar Systems\Solimar Licensing\SolimarLicenseServer", if it doesn't exist, check for "CSIDL_COMMON_APPDATA\\Solimar\SolimarLicenseServer" and move
		// all of its contents to "CSIDL_COMMON_APPDATA\\Solimar Systems\Solimar Licensing\SolimarLicenseServer".  This will break any backward compatibly once the files are moved.

		WCHAR szPathCr20830Post[MAX_PATH];
		wmemcpy_s(&szPathCr20830Post[0], MAX_PATH, &szPath[0], MAX_PATH);
		PathAppend(szPathCr20830Post, L"Solimar Systems");

		// Create [ProgramData]\Solimar Systems if neccessary
		if(PathFileExists(szPathCr20830Post) == false)
			CreateDirectory(szPathCr20830Post, 0);

		PathAppend(szPathCr20830Post, L"Solimar Licensing");
		// Create [ProgramData]\Solimar Systems\Solimar Licensing if neccessary
		if(PathFileExists(szPathCr20830Post) == false)
			CreateDirectory(szPathCr20830Post, 0);

		PathAppend(szPathCr20830Post, L"SolimarLicenseServer");

		WCHAR szPathCr20830Pre[MAX_PATH];
		wmemcpy_s(&szPathCr20830Pre[0], MAX_PATH, &szPath[0], MAX_PATH);
		PathAppend(szPathCr20830Pre, L"Solimar\\SolimarLicenseServer");

		if(PathFileExists(szPathCr20830Pre) == true)
		{
			if(PathFileExists(szPathCr20830Post) == true)
				SpdUtils::DeleteFolderAndFiles(std::wstring(szPathCr20830Post));

			if (MoveFile(szPathCr20830Pre, szPathCr20830Post) == false)
			{
//_snwprintf_s(debug_buf, 1024, L"SoftwareServerDataMgr::LoadFromFile() - Move Failed, Error Code: %d", GetLastError());
//OutputDebugStringW(debug_buf);
			}
		}

		//Append config file
		if(!PathAppend(szPath, L"Solimar Systems\\Solimar Licensing\\SolimarLicenseServer\\LocalConnectionSettings.xml")) 
			break;	
	
		//Open config file
		hr = m_xmlReader.Load(szPath);
		if(FAILED(hr))	
			break;

		// Get root.
		MSXML2::IXMLDOMNodePtr pParentNode;
		MSXML2::IXMLDOMNodePtr pChildNode;
		hr = m_xmlReader.GetRoot(pParentNode);
		if (FAILED(hr))
			break;

		hr = m_xmlReader.GetChildByElement(pParentNode, L"Version", pChildNode);
		if (FAILED(hr))
			break;

		hr = m_xmlReader.GetAttribute(pChildNode, L"value", m_bstrXmlSettingsVersion);
		if (FAILED(hr))
			break;

		break;	//Unconditional break
	}

//swprintf_s(tmpbuf, 1024, L"LicenseSettings::Initialize returns 0x%08x", hr);
//OutputDebugString(tmpbuf); 
	return hr;
}

/*
//Version 1.0 of XML layout
<LicensingStructure>
	<Version value="1.0" />
	<ServerStructure>
		<Product id="">
			<Server name="[string]" backup="[0/1]" shared="[0/1]" />
			.
			.
			.
			.
		</Product>
		.
		.
		.
	</ServerStructure>
</LicensingStructure>

//Version 2.0 of XML layout
<LicensingStructure>
	<Version value="2.0" />
	<ServerStructure>
		<Product id="">
			<Server name="[string]" backup="[string]" testDev="[0/1]"/>
		</Product>
		.
		.
		.
	</ServerStructure>
</LicensingStructure>
*/
HRESULT LicenseSettings::GetLiceseServerByProduct(long productID, LicenseServerSettingsTwoPointZero* pLicenseServer)
{
//wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"LicenseSettings::GetLiceseServerByProduct (productID: %d", productID);
//OutputDebugString(tmpbuf); 
	HRESULT hr(E_FAIL);
	try
	{
		if(pLicenseServer == NULL)
			throw E_FAIL;

		SafeMutex mutex(m_xmlReaderMutex);

//swprintf_s(tmpbuf, 1024, L"LicenseSettings::GetLiceseServerByProduct - m_bstrXmlSettingsVersion: %s", (wchar_t*)m_bstrXmlSettingsVersion);
//OutputDebugString(tmpbuf); 
		if(wcscmp(m_bstrXmlSettingsVersion, L"1.0") == 0)	//Version 1.0 settings
		{
			std::list<LicenseSettings::LicenseServerSettingsOnePointZero*> licServerSettingsList;
			licServerSettingsList.clear();

			//find correct ProdID
			long calProdID = productID;
			if(productID == Lic_PackageAttribs::pid_SdxDesigner)
				calProdID = Lic_PackageAttribs::pid_SolIndexer;
			else if(productID == Lic_PackageAttribs::pid_RubikaProcessBuilder)
				calProdID = Lic_PackageAttribs::pid_Rubika;
			else if(productID == Lic_PackageAttribs::pid_SpdeQueueManager)
				calProdID = Lic_PackageAttribs::pid_Spde;

			hr = LoadVersionOnePointZeroXML(calProdID, &licServerSettingsList);
			if(FAILED(hr))
				throw hr;

			bool bUseShared = (calProdID != productID);
//swprintf_s(tmpbuf, 1024, L"LicenseSettings::GetLiceseServerByProduct calProdID: %d, bUseShared:%d", calProdID, bUseShared);
//OutputDebugString(tmpbuf); 

			//convert LicenseServerSettingsOnePointZero to LicenseServerSettingsTwoPointZero
			pLicenseServer->bIsTestDevLicensing = false;
			for(	std::list<LicenseSettings::LicenseServerSettingsOnePointZero*>::iterator licServerSettingsIt = licServerSettingsList.begin();
					licServerSettingsIt != licServerSettingsList.end();
					licServerSettingsIt++)
			{
				if((*licServerSettingsIt)->bIsSharedServer == bUseShared)
				{
					if((*licServerSettingsIt)->bIsBackupServer)
						pLicenseServer->backupServerName = (*licServerSettingsIt)->serverName;
					else
						pLicenseServer->serverName = (*licServerSettingsIt)->serverName;
				}
			}
		}
		else if(wcscmp(m_bstrXmlSettingsVersion, L"2.0") == 0)	//Version 2.0 settings
		{
			hr = LoadVersionTwoPointZeroXML(productID, pLicenseServer);
		}
		
	}
	catch(HRESULT &ehr)
	{
		hr = ehr;
	}

//swprintf_s(tmpbuf, 1024, L"LicenseSettings::GetLiceseServerByProduct returns 0x%08x", hr);
//OutputDebugString(tmpbuf); 
	return hr;
}

HRESULT LicenseSettings::LoadVersionOnePointZeroXML(long productID, std::list<LicenseServerSettingsOnePointZero*>* pLicenseServerList)
{
//	wchar_t tmpbuf[1024];
//swprintf_s(tmpbuf, 1024, L"LicenseSettings::LoadVersionOnePointZeroXML(productID: %d)", productID);
//OutputDebugString(tmpbuf); 
	
	HRESULT hr(S_OK);
	for(;;)
	{
		//Get root
		MSXML2::IXMLDOMNodePtr pParentNode;
		MSXML2::IXMLDOMNodePtr pChildNode;
		hr = m_xmlReader.GetRoot(pParentNode);
		if (FAILED(hr))
			break;

		hr = m_xmlReader.GetChildByElement(pParentNode, L"ServerStructure", pChildNode);
		if (FAILED(hr))
			break;

		int childCount = 0;
		hr = m_xmlReader.GetChildCount(pChildNode, childCount);
		if (FAILED(hr))
			break;

		bool bFoundNode = false;
		MSXML2::IXMLDOMNodePtr pProdNode;
		for(int idx=0; idx<childCount; idx++)
		{
			hr = m_xmlReader.GetChildByIndex(pChildNode, idx, pProdNode);
			if (FAILED(hr))
				continue;

			//Look for ProductID
			_bstr_t prodId = L"";
			hr = m_xmlReader.GetAttribute(pProdNode, L"id", prodId);
			if (FAILED(hr))
				continue;

			wchar_t wcstrProdId[256];
			wsprintf(wcstrProdId, L"%d", productID);
			if(_wcsicmp(wcstrProdId, prodId) == 0)
			{
				bFoundNode = true;	//Found Correct ProductID Node
				break;
			}
		}

		if(bFoundNode)
		{
			int prodServerChildCount = 0;
			MSXML2::IXMLDOMNodePtr pProdServerNode;
			hr = m_xmlReader.GetChildCount(pProdNode, prodServerChildCount);
			if (FAILED(hr))
				break;

			for(int idx=0; idx<prodServerChildCount; idx++)
			{
				hr = m_xmlReader.GetChildByIndex(pProdNode, idx, pProdServerNode);
				if (FAILED(hr))	continue;
				
				_bstr_t serverName = L"";
				hr = m_xmlReader.GetAttribute(pProdServerNode, L"name", serverName);
				if (FAILED(hr))	continue;

				_bstr_t backupServer = L"";
				hr = m_xmlReader.GetAttribute(pProdServerNode, L"backup", backupServer);
				if (FAILED(hr))	continue;

				_bstr_t sharedServer = L"";
				hr = m_xmlReader.GetAttribute(pProdServerNode, L"shared", sharedServer);
				if (FAILED(hr))	continue;

//swprintf_s(tmpbuf, 1024, L"LicenseSettings::LoadVersionOnePointZeroXML(name: %s, backup: %s, sharedServer: %s", (wchar_t*)serverName, (wchar_t*)backupServer, (wchar_t*)sharedServer);
//OutputDebugString(tmpbuf); 
				LicenseServerSettingsOnePointZero* tmpLicSetting = new LicenseServerSettingsOnePointZero();
				tmpLicSetting->serverName = serverName;
				tmpLicSetting->bIsBackupServer = _wcsicmp(backupServer, L"0") == 0 ? false : true;
				tmpLicSetting->bIsSharedServer = _wcsicmp(sharedServer, L"0") == 0 ? false : true;
				pLicenseServerList->insert(pLicenseServerList->end(), tmpLicSetting);
			}
		}
		break; //Unconditional break
	}
	return hr;
}

HRESULT LicenseSettings::LoadVersionTwoPointZeroXML(long productID, LicenseServerSettingsTwoPointZero* pLicenseServer)
{
//	wchar_t tmpbuf[1024];
	HRESULT hr(S_OK);
	for(;;)
	{
		//Get root
		MSXML2::IXMLDOMNodePtr pParentNode;
		MSXML2::IXMLDOMNodePtr pChildNode;
		hr = m_xmlReader.GetRoot(pParentNode);
		if (FAILED(hr))
			break;
		hr = m_xmlReader.GetChildByElement(pParentNode, L"ServerStructure", pChildNode);
		if (FAILED(hr))
			break;

		int childCount = 0;
		hr = m_xmlReader.GetChildCount(pChildNode, childCount);
		if (FAILED(hr))
			break;
		bool bFoundNode = false;
		MSXML2::IXMLDOMNodePtr pProdNode;
		for(int idx=0; idx<childCount; idx++)
		{
			hr = m_xmlReader.GetChildByIndex(pChildNode, idx, pProdNode);
			if (FAILED(hr))
				continue;

			//Look for ProductID
			_bstr_t prodId = L"";
			hr = m_xmlReader.GetAttribute(pProdNode, L"id", prodId);
			if (FAILED(hr))
				continue;

			wchar_t wcstrProdId[256];
			wsprintf(wcstrProdId, L"%d", productID);
			if(_wcsicmp(wcstrProdId, prodId) == 0)
			{
				bFoundNode = true;	//Found Correct ProductID Node
				break;
			}
		}
		if(bFoundNode)
		{
			int prodServerChildCount = 0;
			MSXML2::IXMLDOMNodePtr pProdServerNode;
			hr = m_xmlReader.GetChildCount(pProdNode, prodServerChildCount);
			if (FAILED(hr))
				break;

			for(int idx=0; idx<prodServerChildCount; idx++)
			{
				hr = m_xmlReader.GetChildByIndex(pProdNode, idx, pProdServerNode);
				if (FAILED(hr))	continue;
				
				_bstr_t serverName = L"";
				hr = m_xmlReader.GetAttribute(pProdServerNode, L"name", serverName);
				if (FAILED(hr))	continue;

				_bstr_t backupServer = L"0";
				hr = m_xmlReader.GetAttribute(pProdServerNode, L"backup", backupServer);

				_bstr_t testDevLic = L"0";
				hr = m_xmlReader.GetAttribute(pProdServerNode, L"testDev", testDevLic);

//swprintf_s(tmpbuf, 1024, L"LicenseSettings::GetLiceseServerByProduct (name: %s, backup: %s, testDev: %s)", (wchar_t*)serverName, (wchar_t*)backupServer, (wchar_t*)testDevLic);
//OutputDebugString(tmpbuf); 
				pLicenseServer->serverName = serverName;
				pLicenseServer->backupServerName = backupServer;
				pLicenseServer->bIsTestDevLicensing = _wcsicmp(testDevLic, L"0") == 0 ? false : true;
				hr = S_OK;
			}
		}
		break; //Unconditional break
	}
	return hr;
}
