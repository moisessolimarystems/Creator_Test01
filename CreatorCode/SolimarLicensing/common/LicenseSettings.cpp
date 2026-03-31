

#include "LicenseSettings.h"
#include "..\common\SafeMutex.h"
#include <shlwapi.h>	//For PathAppend


HRESULT LicenseSettings::Initialize()
{

	HRESULT hr(E_FAIL);
wchar_t tmpbuf[1024];
swprintf_s(tmpbuf, 1024, L"LicenseSettings::Initialize() - Enter");
OutputDebugString(tmpbuf); 

	for(;;)
	{
		SafeMutex mutex(m_xmlReaderMutex);
		//Find CSIDL_COMMON_APPDATA on local system
		WCHAR szPath[MAX_PATH];
		hr = SHGetFolderPath( NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath );
		if(FAILED(hr))	
			break;

		//Append config file
		if(!PathAppend(szPath, L"Solimar\\SolimarLicenseServer\\LocalConnectionSettings.xml")) 
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

swprintf_s(tmpbuf, 1024, L"LicenseSettings::Initialize returns 0x%08x", hr);
OutputDebugString(tmpbuf); 
	return hr;
}

/*
//Version 1.0 of XML layout
<LicensingStructure>
	<Version value="" />
	<ServerStructure>
		<Product id="">
			<Server name="" backup="" shared="" />
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
*/
HRESULT LicenseSettings::GetLiceseServerByProduct(long productID, std::list<LicenseServerSettings*>* pLicenseServerList)
{
wchar_t tmpbuf[1024];
swprintf_s(tmpbuf, 1024, L"LicenseSettings::GetLiceseServerByProduct (productID: %d", productID);
OutputDebugString(tmpbuf); 
	HRESULT hr(E_FAIL);
	if(pLicenseServerList == NULL)
		return hr;

	for(;;)
	{
		SafeMutex mutex(m_xmlReaderMutex);

		pLicenseServerList->clear();

		if(wcscmp(m_bstrXmlSettingsVersion, L"1.0") == 0)	//Version 1.0 settings
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

swprintf_s(tmpbuf, 1024, L"LicenseSettings::GetLiceseServerByProduct (name: %s, backup: %s, sharedServer: %s", (wchar_t*)serverName, (wchar_t*)backupServer, (wchar_t*)sharedServer);
OutputDebugString(tmpbuf); 
					LicenseServerSettings* tmpLicSetting = new LicenseServerSettings();
					tmpLicSetting->serverName = serverName;
					tmpLicSetting->bIsBackupServer = _wcsicmp(backupServer, L"0") == 0 ? false : true;
					tmpLicSetting->bIsSharedServer = _wcsicmp(sharedServer, L"0") == 0 ? false : true;
					pLicenseServerList->insert(pLicenseServerList->end(), tmpLicSetting);
				}
			}
		}
		break; //Unconditional break
	}

swprintf_s(tmpbuf, 1024, L"LicenseSettings::GetLiceseServerByProduct returns 0x%08x", hr);
OutputDebugString(tmpbuf); 
	return hr;
}