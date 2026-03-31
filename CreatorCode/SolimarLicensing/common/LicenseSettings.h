#pragma once

#include <shlobj.h>			//For SHGetFolderPath
#include <comutil.h>			//For _bstr_t
#include <list>
#include "..\common\XMLConfig.h"	//For XMLConfig


class LicenseSettings
{
	public:
		class LicenseServerSettings
		{
			public :
				LicenseServerSettings(): serverName(L""), bIsBackupServer(false), bIsSharedServer(false){}
				_bstr_t serverName;
				bool bIsBackupServer;
				bool bIsSharedServer;
		};

		LicenseSettings(): m_bstrXmlSettingsVersion(L""), m_xmlReaderMutex(CreateMutex(0, 0, L"")){}
		~LicenseSettings()
		{
			if(m_xmlReaderMutex)
				CloseHandle(m_xmlReaderMutex);
		}
		HRESULT Initialize();
		HRESULT GetLiceseServerByProduct(long productID, std::list<LicenseServerSettings*>* pLicenseServerList);

	private:
		HANDLE m_xmlReaderMutex; 
		XMLConfig m_xmlReader;
		_bstr_t m_bstrXmlSettingsVersion;
};