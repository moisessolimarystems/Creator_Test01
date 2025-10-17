#pragma once

#include "..\Common\LicAttribsCPP\Lic_PackageAttribs.h"
#include "..\common\SafeMutex.h"

class GlobalSoftwareSpec
{
	public: 
		GlobalSoftwareSpec();
		~GlobalSoftwareSpec();

		Lic_PackageAttribs::Lic_SoftwareSpecAttribs GetSoftwareSpec();
		void SetSoftwareSpec(Lic_PackageAttribs::Lic_SoftwareSpecAttribs newSoftwareSpec);
		
	private:
		HANDLE GlobalSoftwareSpecLock;
		Lic_PackageAttribs::Lic_SoftwareSpecAttribs softwareSpec;

		#include "SoftwareProductSpecInitialize.h"
};


//extern GlobalSoftwareSpec g_softwareSpec;
extern GlobalSoftwareSpec* g_pSoftwareSpec;




