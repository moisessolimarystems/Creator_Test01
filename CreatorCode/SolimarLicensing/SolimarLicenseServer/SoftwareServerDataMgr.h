#pragma once
#include <comdef.h>
#include "..\Common\LicAttribsCPP\Lic_ServerDataAttribs.h"
class SoftwareServerDataMgr
{
	public:
		SoftwareServerDataMgr();
		~SoftwareServerDataMgr();

		HRESULT GetAllFileInfoList(Lic_ServerDataAttribs::Lic_ServerDataFileInfoAttribsList* pFileInfoList);
		HRESULT GetFileInfoFor(_bstr_t bstrLicenseNameValue, Lic_ServerDataAttribs::Lic_ServerDataFileInfoAttribs* pFileInfo);
		HRESULT SetFileInfoFor(_bstr_t bstrLicenseNameValue, Lic_ServerDataAttribs::Lic_ServerDataFileInfoAttribs* pFileInfo);

		HRESULT Touch();

		// Up to caller to free pBstrSoftwareStream
		HRESULT GetLicServerDataAttrbsStream(BSTR* pBstrSoftwareStream);
	private:
		HRESULT LoadFromFile();
		HRESULT SaveToFile();
		Lic_ServerDataAttribs* pServerDataAttribs;
		HANDLE softwareServerDataMgrLock;
		time_t lastTouchDateTimeT;
};


class SoftwareLicenseFile
{
	public:
		static HRESULT SaveToLicenseFile(_bstr_t licenseFileName, BSTR bstrSoftwareStream);
		static HRESULT LoadFromLicenseFile(_bstr_t licenseFileName, BSTR* pBstrSoftwareStream);		// Up to caller to free pBstrSoftwareStream
		static HRESULT DeleteLicenseFile(_bstr_t licenseFileName);

	private:
		static BYTE crypto_license_file_password[];
		static BYTE crypto_license_file_public[];
		static BYTE crypto_license_file_private[];
		//static HANDLE softwareLicenseFileLock;
};
