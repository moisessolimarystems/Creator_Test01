#pragma once

#include <shlobj.h>			//For SHGetFolderPath
#include <comutil.h>			//For _bstr_t
#include <list>
#include "..\common\XMLConfig.h"	//For XMLConfig


class LicenseSettings
{
	public:
		class LicenseServerSettingsOnePointZero
		{
			public :
				LicenseServerSettingsOnePointZero(): 
					 serverName(L""), 
					 bIsBackupServer(false), 
					 bIsSharedServer(false),
					 bIsSoftwareLicensing(false),
					 bIsTestDevLicensing(false),
					 bIsDisasterRecoveryServer(false)
					 {}

				_bstr_t serverName;
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
					 serverName(L""), 
					 backupServerName(L""), 
					 bIsTestDevLicensing(false)
					 {}

				_bstr_t serverName;
				_bstr_t backupServerName;
				bool bIsTestDevLicensing;
		};

		LicenseSettings(): m_bstrXmlSettingsVersion(L""), m_xmlReaderMutex(CreateMutex(0, 0, L"")){}
		~LicenseSettings()
		{
			if(m_xmlReaderMutex)
				CloseHandle(m_xmlReaderMutex);
		}
		HRESULT Initialize();
		HRESULT GetLiceseServerByProduct(long productID, LicenseServerSettingsTwoPointZero* pLicenseServer);

	private:
		HANDLE m_xmlReaderMutex; 
		XMLConfig m_xmlReader;
		_bstr_t m_bstrXmlSettingsVersion;

		HRESULT LoadVersionOnePointZeroXML(long productID, std::list<LicenseServerSettingsOnePointZero*>* pLicenseServerList);
		HRESULT LoadVersionTwoPointZeroXML(long productID, LicenseServerSettingsTwoPointZero* pLicenseServer);
};
