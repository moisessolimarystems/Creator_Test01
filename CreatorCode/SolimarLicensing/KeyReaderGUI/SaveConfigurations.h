#ifndef __SAVINGCONFIGURATIONS_H__
#define __SAVINGCONFIGURATIONS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#using <mscorlib.dll>

namespace SaveConfig
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;
	using namespace Microsoft::Win32; 

	//this class handles the sizing of controls upon the form load. 
	__gc class SaveConfigurations
	{
		public:

		//constructor
		SaveConfigurations(Form* TheForm);
		~SaveConfigurations();

		//initializes the size of the form
		void LoadFormConfigs(int* width, int* height, int* x_coord, int* y_coord);
		void SaveFormConfigs();
		void SaveModPanelConfig(Panel* TheModPanel);
		void LoadModPanelConfig(int* width, int* height, int* x_coord, int* y_coord);
		void SaveKeyPanelConfig(Panel* TheKeyPanel);
		void LoadKeyPanelConfig(int* width, int* height, int* x_coord, int* y_coord);
		void SaveModListConfig(ListView* TheModListView);
		void LoadModListConfig(ModuleGUIConfigurationStructPtr TheModConfigStructPtr);
		void SaveKeyListConfig(ListView* TheKeyListView);
		void LoadKeyListConfig(KeyGUIConfigurationStructPtr TheKeyConfigStruct);
		void SaveSplitterConfig(Splitter* TheSplitter);
		void LoadSplitterConfig(int* width, int* height, int* x_coord, int* y_coord);

	private:
		//Members
		Form* MainForm;
		RegistryKey * pRegKey;

		//private/internal functions
		void InitRegKeyEditing();
		void WriteRegKey(String* name, Object* value);
		Object* ReadRegKey(String* name);
	};
}
#endif //__CONTACTINFO_H__