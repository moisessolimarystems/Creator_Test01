#pragma once
#include "..\Common\LicAttribsCPP\Lic_ClientSettingsAttribs.h"
#include "..\Common\LicAttribsCPP\Lic_PackageAttribs.h"

#include <comutil.h>	//_bstr_t
class ProductConnectionSettings
{
	public:
		// ProductConnectionSettings();

		static HRESULT GetClientSettingsAttribs(Lic_ClientSettingsAttribs* pClientSettingsAttrib, Lic_PackageAttribs::Lic_SoftwareSpecAttribs* pSoftwareSpec);

		
	private:
		static const byte RegKey_SolimarByte[];
		static const byte RegKey_Solimar32Byte[];
		static const byte RegKey_ProductLicensingByte[];
		static const byte RegKey_SseByte[];
		static const byte RegKey_SseInstallInfoByte[];
		static const byte RegKey_SseInstallInfoSseByte[];
		static const byte RegKey_SseInstallInfoSseSpByte[];
		static const byte RegKey_SseInstallInfoSse64Byte[];
		static const byte RegKey_SseInstallInfoSseSp64Byte[];

		static HRESULT GetProductID(	_bstr_t productName, 
												int* pProductId, 
												Lic_PackageAttribs::Lic_SoftwareSpecAttribs* pSoftwareSpec
												);

		static bool IsSseSystemManagerInstalled(_bstr_t regKey);

		static size_t FindDataForTags(const std::wstring &wsTagId, 
												const std::wstring &wsData,
												std::wstring &wsOutVal,
												bool bKeepTags = false,
												size_t stSearchStartPos = 0 
												);
		static size_t FindValueInData(const std::wstring &wsElement,
												const std::wstring &wsData,
												std::wstring &wsOutVal
												);

};
