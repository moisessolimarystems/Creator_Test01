#ifndef __MODULELICENSELISTVIEWMANAGER_H__
#define __MODULELICENSELISTVIEWMANAGER_H__

#include "CommunicationLink.h"

const long UNLIMITED_LICENSES = 0xFFFFFFFF / 2;
const int  SPD_PRODUCT = 0;
const int  ICONVERT_PRODUCT = 7;
const int  SOLSCRIPT_PRODUCT = 9;
const int  SDX_DESIGNER_PRODUCT = 10;

const unsigned short SPD_MAX_NUMBER_MODULES[] =
{        // Maximum number of modules that can be licensed. The max number denotes unlimited. 
         //The index into this array represents the module id
   0,    // 0
   0,  	// 1
   1,  	// 2
   1, 	// 3
   1, 	// 4
   1, 	// 5
   4, 	// 6
   4,  	// 7
   4,  	// 8
   1, 	// 9
   1, 	// 10
   1, 	// 11
   2, 	// 12
   2,  	// 13
   1,  	// 14
   4, 	// 15
   4, 	// 16
   1, 	// 17
   1, 	// 18
   1,  	// 19
   1,  	// 20
   2, 	// 21
   1, 	// 22
   1, 	// 23
   1, 	// 24
   2,  	// 25
   1,  	// 26
   1, 	// 27
   1, 	// 28
   1, 	// 29
   1, 	// 30
   1,  	// 31
   0,  	// 32
   1, 	// 33
   1, 	// 34
   1, 	// 35
   1, 	// 36
   4,  	// 37
   4,  	// 38
   1, 	// 39
   1, 	// 40
   1, 	// 41
   1, 	// 42
   0,  	// 43
   1,  	// 44
   1, 	// 45
   2, 	// 46
   1, 	// 47
   1,    // 48
   1, 	// 49
   1, 	// 50
   1,    // 51
   2, 	// 52
   2, 	// 53
   1,    // 54
   1, 	// 55
   1, 	// 56
   0,    // 57
   0, 	// 58
   0, 	// 59
   0,    // 60
   0, 	// 61
   1,    // 62
   1     // 63
};

const unsigned short ICONVERT_MAX_NUMBER_MODULES[] =
{        // Maximum number of modules that can be licensed. The max number denotes unlimited. 
         //The index into this array represents the module id
   1,    // 0
   1,  	// 1
   1,  	// 2
   0, 	// 3
   0, 	// 4
   0, 	// 5
   0, 	// 6
   0,  	// 7
   1,  	// 8
   1, 	// 9
   0, 	// 10
   0, 	// 11
   1, 	// 12
   1,  	// 13
   1,  	// 14
   1, 	// 15
   0, 	// 16
   0, 	// 17
   0, 	// 18
   0,  	// 19
   0,  	// 20
   0, 	// 21
   0, 	// 22
   0, 	// 23
   0, 	// 24
   0,  	// 25
   0,  	// 26
   0, 	// 27
   0, 	// 28
   1, 	// 29
   1, 	// 30
   0,  	// 31
   1,  	// 32
   1, 	// 33
   1, 	// 34
   1, 	// 35
   1, 	// 36
   0,  	// 37
   0,  	// 38
   0, 	// 39
   1, 	// 40
   1, 	// 41
   1, 	// 42
   1,  	// 43
   0,  	// 44
   0, 	// 45
   0, 	// 46
   0, 	// 47
   1,    // 48
   1, 	// 49
   1, 	// 50
   1,    // 51
   0, 	// 52
   0, 	// 53
   0,    // 54
   0, 	// 55
   1  	// 56
};

const unsigned short SOLSCRIPT_MAX_NUMBER_MODULES[] =
{        // Maximum number of modules that can be licensed. The max number denotes unlimited. 
         //The index into this array represents the module id
   1,    // 0
   1,  	// 1
   0,  	// 2
   0, 	// 3
   0, 	// 4
   0, 	// 5
   0, 	// 6
   0,  	// 7
   0,  	// 8
   0, 	// 9
   0, 	// 10
   0, 	// 11
   1, 	// 12
   0,  	// 13
   0,  	// 14
   0, 	// 15
   0, 	// 16
   0, 	// 17
   0, 	// 18
   0,  	// 19
   0,  	// 20
   1, 	// 21
   0, 	// 22
   0, 	// 23
   0, 	// 24
   1,  	// 25
   0,  	// 26
   0, 	// 27
   0, 	// 28
   1, 	// 29
   1, 	// 30
   0,  	// 31
   1,  	// 32
   1, 	// 33
   1, 	// 34
   1, 	// 35
   1, 	// 36
   0,  	// 37
   0,  	// 38
   0, 	// 39
   1, 	// 40
   1, 	// 41
   1, 	// 42
   1,  	// 43
   0,  	// 44
   0, 	// 45
   1, 	// 46
   0, 	// 47
   1,    // 48
   1, 	// 49
   1, 	// 50
   1,    // 51
   1, 	// 52
   1 	// 53
};

const unsigned short SDX_DESIGNER_MAX_NUMBER_MODULES[] =
{        // Maximum number of modules that can be licensed. The max number denotes unlimited. 
         //The index into this array represents the module id
   1,    // 0
   1  	// 1
};
namespace ModuleViewManager
{
	using namespace System;
	using namespace System::IO;
	using namespace System::Data;
	using namespace System::Windows::Forms;


	__gc class ModuleLicenseListViewManager
	{
		public:
			ModuleLicenseListViewManager(ListView* TheModViewManager);
			~ModuleLicenseListViewManager();
			void PopulateView();
			void SetKeyID(Object* NewKeyNumber);
			bool IsInitialized();

	private: 
		CommunicationLink* OurCommLink;
		ListView* TheModView;
		Object* KeyNum;

		void FillRow(ModuleLicensingStructure TheModuleLicStructure);
	};
}

#endif //__MODULELICENSELISTVIEWMANAGER_H__