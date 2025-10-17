#pragma once
#include <comdef.h>
#include <map>
#include "..\Common\Version.h"

//
// This class may need to be serializable
//

class ModuleObject
{
	public:
		int moduleID;
		int value;
		int applicationInstances;
};
class ProductObject
{
	public:
		int productID;
		Version::ModuleVersion productVersion;
		int licenseType;	//Maybe an enum instead
		std::map<int, ModuleObject> moduleMap; //<int(moduleID), ModuleObject>
};



