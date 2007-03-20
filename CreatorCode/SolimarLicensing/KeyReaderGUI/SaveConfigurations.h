#ifndef __SAVINGCONFIGURATIONS_H__
#define __SAVINGCONFIGURATIONS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#using <mscorlib.dll>

#define NUM_MOD_COLS 3
#define NUM_KEY_COLS 7

namespace SaveConfig
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;
	using namespace System::IO;
	using namespace System::IO::IsolatedStorage;
	using namespace System::Runtime::Serialization;
	using namespace System::Runtime::Serialization::Formatters::Binary;
	using namespace Microsoft::Win32; 

	[Serializable]
	public __gc class FormSettings
	{
		public :
			//Form Settings
			Point FormLocation;
			Size FormSize;

			//Module List Settings
			Point ModListLocation;
			Size ModListSize;
			int ModColumnWidth __gc[];
			int ModColOrder __gc[];

			//Key List Settings
			Point KeyListLocation;
			Size KeyListSize;
			int KeyColWidth __gc[];
			int KeyColOrder __gc[];		
		public:
			FormSettings()
			{
				FormSize.Width = -1;
				ModListSize.Width = -1;
				KeyListSize.Width = -1;
				ModColumnWidth = new int __gc[NUM_MOD_COLS];
				ModColOrder = new int __gc[NUM_MOD_COLS];
				KeyColWidth = new int __gc[NUM_KEY_COLS];
				KeyColOrder = new int __gc[NUM_KEY_COLS];
			}
	};
	
	//this class handles the sizing of controls upon the form load. 
    public __gc class SaveConfigurations
	{
		public:
		//constructor
		SaveConfigurations(Form* TheForm);
		~SaveConfigurations();

		//initializes the size of the form
		void LoadFormConfigs(Point* FormLocation, Size* FormSize);
		void SaveFormConfigs();
		void SaveModListConfig(ListView* TheModListView);
		void LoadModListConfig(ModuleGUIConfigurationStructPtr TheModConfigStructPtr);
		void SaveKeyListConfig(ListView* TheKeyListView);
		void LoadKeyListConfig(KeyGUIConfigurationStructPtr TheKeyConfigStruct);

		//Load/Save Settings to IsolatedStorage
		void LoadSettings();
		void SaveSettings();
	private:
		//Members
		String* settingsFileName;
		FormSettings* formConfig;
		Form* MainForm;
	};
}
#endif //__CONTACTINFO_H__
