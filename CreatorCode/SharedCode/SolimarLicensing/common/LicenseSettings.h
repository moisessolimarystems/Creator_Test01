#pragma once

//#include <comutil.h>			//For _bstr_t														// CR.32662.V01; commented.
#include <list>
#include "..\common\XMLConfig.h"	//For XMLConfig
#include "..\common\CLicenseFolders.h"																// CR.32662.V01; added.

/* class LicenseSettings
 *
 * SLB 29.feb.2024 CR.32662.V01; converted all _bstr_t to CComBSTR.
 * SLB 03.apr.2024 CR.32662.V02; converted all CComBSTR to CSpdCString.
 */
class LicenseSettings : 
	public CLicenseFolders																				// CR.32662.V01; added.
{
	public:
		class LicenseServerSettingsOnePointZero
		{
			public :
				LicenseServerSettingsOnePointZero(): 
					 bIsBackupServer(false), 
					 bIsSharedServer(false),
					 bIsSoftwareLicensing(false),
					 bIsTestDevLicensing(false),
					 bIsDisasterRecoveryServer(false)
				{}

				CSpdCString m_wsServerName;
				bool bIsBackupServer;
				bool bIsSharedServer;
				bool bIsSoftwareLicensing;
				bool bIsTestDevLicensing;
				bool bIsDisasterRecoveryServer;
		};

		class LicenseServerSettingsTwoPointZero
		{
			public :
				LicenseServerSettingsTwoPointZero(): 
					m_bIsTestDevLicensing(false)
				{}

				CSpdCString m_wsServerName;
				CSpdCString m_wsBackupServerName;
				bool m_bIsTestDevLicensing;
		};

		LicenseSettings(): m_xmlReaderMutex(CreateMutex(0, 0, L"")){}
		~LicenseSettings()
		{
			if (m_xmlReaderMutex)
				CloseHandle(m_xmlReaderMutex);
		}
		HRESULT Initialize();
		HRESULT GetLiceseServerByProduct(long productID, LicenseServerSettingsTwoPointZero* pLicenseServer);

	private:
		HANDLE m_xmlReaderMutex; 
		XMLConfig m_xmlReader;
		CSpdCString m_wsXmlSettingsVersion;

		HRESULT LoadVersionOnePointZeroXML(long productID, std::list<LicenseServerSettingsOnePointZero*>* pLicenseServerList);
		HRESULT LoadVersionTwoPointZeroXML(long productID, LicenseServerSettingsTwoPointZero* pLicenseServer);
};
