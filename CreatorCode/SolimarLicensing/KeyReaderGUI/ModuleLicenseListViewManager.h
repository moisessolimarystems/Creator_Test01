#ifndef __MODULELICENSELISTVIEWMANAGER_H__
#define __MODULELICENSELISTVIEWMANAGER_H__

#include "CommunicationLink.h"

const long UnlimitedLicenses = 0xFFFFFFFF / 2;

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