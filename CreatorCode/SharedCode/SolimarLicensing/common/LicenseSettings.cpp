#include "LicenseSettings.h"

#include "..\common\SafeMutex.h"
#include "..\common\LicAttribsCPP\Lic_PackageAttribs.h"
#include "..\common\FromSpd\L_SpdException.h"													// CR.32662.V01; added.

/* Initialize()
 * SLB 29.feb.2024 - I don't understand why this is in a for () loop, when it just has
 * an unconditional break at the end AND no continue statements....
 * 
 * SLB 24.jan.2024 CR.32662; Simple changes to reduce compiler warnings.
 * Replaced calls to deprecated method SHGetFolderPath() with calls to 
 * SHGetKnownFolderPath().
 * SLB 13.mar.2024 CR.32662.V01; Found issues with use of SHGetKnownFolderPath(). 
 * Ended up re-coding entire section related to retrieving the ProgramData folder 
 * and handling any pre CR.20830 conversions. 
 * Removed the for (;;) loop, it seemed pointless, and added exception handling.
---------------------------------------------------------------------------------------*/
HRESULT LicenseSettings::Initialize()
{

	try
	{
		SafeMutex mutex(m_xmlReaderMutex);
		CNtfsFolderName fn(GetConfigSettingsFile());
		CNtfsFolderName fnPreCR20830(GetConfigSettingsFile(true));

		// CR.FIX.20830 - Use "CSIDL_COMMON_APPDATA\\Solimar Systems\Solimar Licensing\SolimarLicenseServer" instead of "CSIDL_COMMON_APPDATA\\Solimar\SolimarLicenseServer".
		// Check for "CSIDL_COMMON_APPDATA\\Solimar Systems\Solimar Licensing\SolimarLicenseServer", if it doesn't exist, check for "CSIDL_COMMON_APPDATA\\Solimar\SolimarLicenseServer" and move
		// all of its contents to "CSIDL_COMMON_APPDATA\\Solimar Systems\Solimar Licensing\SolimarLicenseServer".  This will break any backward compatibly once the files are moved.

		// Check to see if pre CR.20830 folders exist. If they do, then they must 
		// be transferred into the current location (and removed from previous).
		DWORD dwNtErr(0);
		bool bRes(false);
		bRes = fnPreCR20830.DoesFolderExist(dwNtErr, false);
		if (bRes)
		{
			// Pre CR.20830 config exists... Move old folder to new folder location.
			// Move operation will fail if destination folder exists, so delete it.
			fn.DeleteFilesAndFolders(true);
			bRes = fnPreCR20830.Ntfs_MoveFile(fn.GetFullFolderName(false).c_str(), 0);
			if (!bRes) ThrowHr(HRESULT_FROM_WIN32(GetLastError()), __WFUNCTION__, L"Error converting ProgramData folder from Legacy format.");

			// Now CR.20830 folder compatibility is normalized to expected location.
		}

		bRes = fn.DoesFolderExist(dwNtErr, false);
		if (!bRes) 
			bRes = fn.CreateFolder(dwNtErr, NULL);
		if (!bRes) ThrowHr(HRESULT_FROM_WIN32(dwNtErr), __WFUNCTION__, L"Error creating the Solimar License Server's ProgramData folder."); 


		// Load the XML config file
		HRESULT hr;
		m_xmlReader.Load(fn.GetFullFileName().c_str());
		//if (FAILED(hr)) ThrowHr(hr, __WFUNCTION__, L"Error loading config file into XML reader.");

		// Get root.
		MSXML2::IXMLDOMNodePtr pParentNode;
		MSXML2::IXMLDOMNodePtr pChildNode;
		hr = m_xmlReader.GetRoot(pParentNode);
		if (FAILED(hr)) ThrowHr(hr, __WFUNCTION__, L"Error navigating to config file root node.");

		hr = m_xmlReader.GetChildByElement(pParentNode, L"Version", pChildNode);
		if (FAILED(hr)) ThrowHr(hr, __WFUNCTION__, L"Expected Version info not found in config file");

		_bstr_t bstrRetVal;																				// CR.32662.V01; added.
		hr = m_xmlReader.GetAttribute(pChildNode, L"value", bstrRetVal);
		if (FAILED(hr)) ThrowHr(hr, __WFUNCTION__, L"Expected Version value not found in config file");

		m_wsXmlSettingsVersion = (wchar_t *)bstrRetVal;														// CR.32662.V01; added.
	}
	catch (SpdException::hr_exception e)
	{
		CEnterExitMsg::TraceMsg(L"(%08X) Exception in %s() handled in %s(). %s", e.GetHr(), e.GetMethod().c_str(), __WFUNCTION__, e.GetMessage().c_str());
		return e.GetHr();
	}

	return S_OK;
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
HRESULT LicenseSettings::GetLiceseServerByProduct(
	long productID, 
	LicenseServerSettingsTwoPointZero* pLicenseServer)
{
	HRESULT hr(E_FAIL);
	try
	{
		if (pLicenseServer == NULL) throw E_FAIL;

		SafeMutex mutex(m_xmlReaderMutex);

		if (m_wsXmlSettingsVersion == L"1.0") // Version 1.0 settings
		{
			std::list<LicenseSettings::LicenseServerSettingsOnePointZero*> licServerSettingsList;
			licServerSettingsList.clear();

			//find correct ProdID
			long calProdID = productID;
			if (productID == Lic_PackageAttribs::pid_SdxDesigner)
				calProdID = Lic_PackageAttribs::pid_SolIndexer;
			else if (productID == Lic_PackageAttribs::pid_RubikaProcessBuilder)
				calProdID = Lic_PackageAttribs::pid_Rubika;
			else if (productID == Lic_PackageAttribs::pid_SpdeRemoteClients)				// CR.33933; modified.
				calProdID = Lic_PackageAttribs::pid_Spde;

			hr = LoadVersionOnePointZeroXML(calProdID, &licServerSettingsList);
			if (FAILED(hr)) throw hr;

			bool bUseShared = (calProdID != productID);

			//convert LicenseServerSettingsOnePointZero to LicenseServerSettingsTwoPointZero
			pLicenseServer->m_bIsTestDevLicensing = false;
			for (	std::list<LicenseSettings::LicenseServerSettingsOnePointZero*>::iterator licServerSettingsIt = licServerSettingsList.begin();
					licServerSettingsIt != licServerSettingsList.end();
					licServerSettingsIt++)
			{
				if ((*licServerSettingsIt)->bIsSharedServer == bUseShared)
				{
					if ((*licServerSettingsIt)->bIsBackupServer)
						pLicenseServer->m_wsBackupServerName = (*licServerSettingsIt)->m_wsServerName;
					else
						pLicenseServer->m_wsServerName = (*licServerSettingsIt)->m_wsServerName;
				}
			}
		}
		else if (m_wsXmlSettingsVersion == L"2.0")	// Version 2.0 settings
		{
			hr = LoadVersionTwoPointZeroXML(productID, pLicenseServer);
		}
		
	}
	catch (HRESULT &ehr)
	{
		hr = ehr;
	}

	return hr;
}

HRESULT LicenseSettings::LoadVersionOnePointZeroXML(long productID, std::list<LicenseServerSettingsOnePointZero*>* pLicenseServerList)
{
	HRESULT hr(S_OK);
	for (;;)
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
		for (int idx=0; idx<childCount; idx++)
		{
			hr = m_xmlReader.GetChildByIndex(pChildNode, idx, pProdNode);
			if (FAILED(hr)) continue;

			//Look for ProductID
			_bstr_t prodId = L"";
			hr = m_xmlReader.GetAttribute(pProdNode, L"id", prodId);
			if (FAILED(hr)) continue;

			wchar_t wcstrProdId[256];
			wsprintf(wcstrProdId, L"%d", productID);
			if (_wcsicmp(wcstrProdId, prodId) == 0)
			{
				bFoundNode = true;	//Found Correct ProductID Node
				break;
			}
		}

		if (bFoundNode)
		{
			int prodServerChildCount = 0;
			MSXML2::IXMLDOMNodePtr pProdServerNode;
			hr = m_xmlReader.GetChildCount(pProdNode, prodServerChildCount);
			if (FAILED(hr)) break;

			for (int idx=0; idx<prodServerChildCount; idx++)
			{
				hr = m_xmlReader.GetChildByIndex(pProdNode, idx, pProdServerNode);
				if (FAILED(hr)) continue;
				
				_bstr_t serverName = L"";
				hr = m_xmlReader.GetAttribute(pProdServerNode, L"name", serverName);
				if (FAILED(hr)) continue;

				_bstr_t backupServer = L"";
				hr = m_xmlReader.GetAttribute(pProdServerNode, L"backup", backupServer);
				if (FAILED(hr)) continue;

				_bstr_t sharedServer = L"";
				hr = m_xmlReader.GetAttribute(pProdServerNode, L"shared", sharedServer);
				if (FAILED(hr)) continue;

				LicenseServerSettingsOnePointZero* tmpLicSetting = new LicenseServerSettingsOnePointZero();
				tmpLicSetting->m_wsServerName = (wchar_t *)serverName;
				tmpLicSetting->bIsBackupServer = _wcsicmp(backupServer, L"0") == 0 ? false : true;
				tmpLicSetting->bIsSharedServer = _wcsicmp(sharedServer, L"0") == 0 ? false : true;
				pLicenseServerList->insert(pLicenseServerList->end(), tmpLicSetting);
			}
		}
		break; //Unconditional break
	}
	return hr;
}

HRESULT LicenseSettings::LoadVersionTwoPointZeroXML(
	long productID, 
	LicenseServerSettingsTwoPointZero* pLicenseServer)
{
	HRESULT hr(S_OK);
	for (;;)
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
		for (int idx=0; idx<childCount; idx++)
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
			if (_wcsicmp(wcstrProdId, prodId) == 0)
			{
				bFoundNode = true;	//Found Correct ProductID Node
				break;
			}
		}
		if (bFoundNode)
		{
			int prodServerChildCount = 0;
			MSXML2::IXMLDOMNodePtr pProdServerNode;
			hr = m_xmlReader.GetChildCount(pProdNode, prodServerChildCount);
			if (FAILED(hr))
				break;

			for (int idx=0; idx<prodServerChildCount; idx++)
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

				pLicenseServer->m_wsServerName = (wchar_t *)serverName;
				pLicenseServer->m_wsBackupServerName = (wchar_t*)backupServer;
				pLicenseServer->m_bIsTestDevLicensing = _wcsicmp(testDevLic, L"0") == 0 ? false : true;
				hr = S_OK;
			}
		}
		break; //Unconditional break
	}
	return hr;
}
