#include "stdafx.h"
#include "SaveConfigurations.h"


const char* RegistryAddress = "SOFTWARE\\Solimar\\Solimar Licensing\\Key License Manager";

using namespace SaveConfig;

//Constructor. 
//Initializes the member variables and creates a table of appropriate size.
SaveConfigurations::SaveConfigurations(Form* TheForm)
				: MainForm(TheForm)
{
	InitRegKeyEditing();
}

//Initializes the resources
SaveConfigurations::~SaveConfigurations()
{
}

void SaveConfigurations::SaveFormConfigs()
{
	Size TheFormSize = MainForm->Size;
	Point TheFormLocation = MainForm->Location;
	char  buffer[10];

	//store the size and location into a buffer which will be written into the registry
	sprintf( buffer, "%d,%d,%d,%d", TheFormSize.Width, TheFormSize.Height,
									TheFormLocation.get_X(), TheFormLocation.get_Y());
	WriteRegKey(S"MainFormConfig", (String*)buffer);
}

void SaveConfigurations::LoadFormConfigs(int* width, int* height, int* x_coord, int* y_coord)
{
	String* retval = static_cast<String*>(ReadRegKey(S"MainFormConfig"));
	char* token;
	char* RegValue;
	
	//if the return value is -1 then set width = -1 so the calling function knows that 
	//this is the first run through
	if(!retval)
	{
		*width = -1;
		return;
	}
	
	//convert the string* to a char*
	System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(retval));
	RegValue = (static_cast<char *>(static_cast<void *>(ptr)));
	
	//parse out the string returned from the registry
	token = strtok(RegValue, ",");
	*width = atoi(token);

	token = strtok(NULL, ",");
	*height = atoi(token);

	token = strtok(NULL, ",");
	*x_coord = atoi(token);

	token = strtok(NULL, " ");
	*y_coord = atoi(token);
}

void SaveConfigurations::SaveModPanelConfig(Panel* TheModPanel)
{
	Size TheModPanelSize = TheModPanel->Size;
	Point TheModPanelLocation = TheModPanel->Location;
	char  buffer[10];
	
	//store the size and location of the mod panel in the registry
	sprintf( buffer, "%d,%d,%d,%d", TheModPanelSize.Width, TheModPanelSize.Height,
									TheModPanelLocation.get_X(), TheModPanelLocation.get_Y()
									);
	
	WriteRegKey(S"ModPanelConfig", (String*)buffer);
}

void SaveConfigurations::LoadModPanelConfig(int* width, int* height, int* x_coord, int* y_coord)
{
	String* retval = static_cast<String*> (ReadRegKey(S"ModPanelConfig"));
	char* token;
	char* RegValue;
	
	//if the return value is -1 then set width = -1 so the calling function knows that 
	//this is the first run through
	if(!retval)
	{
		*width = -1;
		return;
	}
	
	//convert the string* to a char*
	System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(retval));
	RegValue = (static_cast<char *>(static_cast<void *>(ptr)));
	
	//parse the string read from the registry 
	token = strtok(RegValue, ",");
	*width = atoi(token);

	token = strtok(NULL, ",");
	*height = atoi(token);

	token = strtok(NULL, ",");
	*x_coord = atoi(token);

	token = strtok(NULL, " ");
	*y_coord = atoi(token);
}

void SaveConfigurations::SaveKeyPanelConfig(Panel* TheKeyPanel)
{
	Size TheKeyPanelSize = TheKeyPanel->Size;
	Point TheKeyPanelLocation = TheKeyPanel->Location;
	char  buffer[10];

	//store the keypanel size and location in the registry
	sprintf( buffer, "%d,%d,%d,%d", TheKeyPanelSize.Width, TheKeyPanelSize.Height,
									TheKeyPanelLocation.get_X(), TheKeyPanelLocation.get_Y()
									);
	WriteRegKey(S"KeyPanelConfig", (String*)buffer);
}

void SaveConfigurations::LoadKeyPanelConfig(int* width, int* height, int* x_coord, int* y_coord)
{
	String* retval = static_cast<String*> (ReadRegKey(S"KeyPanelConfig"));
	char* token;
	char* RegValue;
	
	//if the return value is -1 then set width = -1 so the calling function knows that 
	//this is the first run through
	if(!retval)
	{
		*width = -1;
		return;
	}
	
	//convert the string* to a char*
	System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(retval));
	RegValue = (static_cast<char *>(static_cast<void *>(ptr)));
	
	//parse the 
	token = strtok(RegValue, ",");
	*width = atoi(token);

	token = strtok(NULL, ",");
	*height = atoi(token);

	token = strtok(NULL, ",");
	*x_coord = atoi(token);

	token = strtok(NULL, " ");
	*y_coord = atoi(token);
}

void SaveConfigurations::SaveModListConfig(ListView* TheModListView)
{
	Size TheSize = TheModListView->Size;
	Point TheLocation = TheModListView->Location;
	int j;
	char  buffer[512];

	//store the mod list view's size and location in the registry
	j = sprintf( buffer, "%d,%d,%d,%d,", TheSize.Width, TheSize.Height,
										TheLocation.get_X(), TheLocation.get_Y()
										);

	//get the collection of headers that belong to the mod list view
	ListView::ColumnHeaderCollection* TheColHeaders = new ListView::
														ColumnHeaderCollection(TheModListView);
	//enumerate through the column headers
	System::Collections::IEnumerator* myEnum = TheColHeaders->GetEnumerator();
	while (myEnum->MoveNext())
	{
		//extract the headers one at a time
		ColumnHeader* CurrentHeader = __try_cast<ColumnHeader*>(myEnum->Current);

		//store the header info in the buffer in the form ColumnName(ColumnWidth)
		j += sprintf(buffer + j, "%s(%d)", CurrentHeader->Text, CurrentHeader->Width);
	}

	//get the current column order
	int LCount = TheModListView->Columns->Count;
	int* LOrder = new int[NUM_MOD_COLS];
	IntPtr ModViewHandle = (TheModListView->Handle);
	HWND m_HWND = reinterpret_cast<HWND> (ModViewHandle.ToPointer());
	ListView_GetColumnOrderArray(m_HWND, LCount, LOrder); 

	//concat the column order to the buffer.
	sprintf(buffer + j, ",%d,%d,%d,", LOrder[0], LOrder[1], LOrder[2]);

	WriteRegKey(S"ModListViewConfig", (String*)buffer);
	
	delete [] LOrder;
}

void SaveConfigurations::LoadModListConfig(ModuleGUIConfigurationStructPtr TheModConfigStructPtr)
{
	String* retval = static_cast<String*> (ReadRegKey(S"ModListViewConfig"));
	char* token;
	char* RegValue;
	
	//if the return value is -1 then set width = -1 so the calling function knows that 
	//this is the first run through
	if(!retval)
	{
		TheModConfigStructPtr->width = -1;
		return;
	}
	
	//convert the string* to a char*
	System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(retval));
	RegValue = (static_cast<char *>(static_cast<void *>(ptr)));
	
	//parse the value in the registry
	token = strtok(RegValue, ",");
	TheModConfigStructPtr->width = atoi(token);

	token = strtok(NULL, ",");
	TheModConfigStructPtr->height = atoi(token);

	token = strtok(NULL, ",");
	TheModConfigStructPtr->x_coord = atoi(token);

	token = strtok(NULL, ",");
	TheModConfigStructPtr->y_coord = atoi(token);

	//next group of data deals with the column headers
	token = strtok(NULL, "(");
	TheModConfigStructPtr->FirstModColumnName = token;

	token = strtok(NULL, ")");
	TheModConfigStructPtr->FirstModColumnWidth = atoi(token);

	token = strtok(NULL, "(");
	TheModConfigStructPtr->SecondModColumnName = token;

	token = strtok(NULL, ")");
	TheModConfigStructPtr->SecondModColumnWidth = atoi(token);
	
	token = strtok(NULL, "(");
	TheModConfigStructPtr->ThirdModColumnName = token;

	token = strtok(NULL, ")");
	TheModConfigStructPtr->ThirdModColumnWidth = atoi(token);

	//next group deals with the column ordering. i.e. the user moves the columns around.
	for(int count = 0; count < NUM_MOD_COLS; count++)
	{
		token = strtok(NULL, ",");
		TheModConfigStructPtr->ColOrder[count] = atoi(token);
	}
}

void SaveConfigurations::SaveKeyListConfig(ListView* TheKeyListView)
{
	Size TheSize = TheKeyListView->Size;
	Point TheLocation = TheKeyListView->Location;
	int j;
	char  buffer[512];

	//store the width, height, and location
	j = sprintf( buffer, "%d,%d,%d,%d,", TheSize.Width, TheSize.Height,
										TheLocation.get_X(), TheLocation.get_Y()
										);

	ListView::ColumnHeaderCollection* TheColHeaders = new ListView::
														ColumnHeaderCollection(TheKeyListView);
	//enumerate through the column headers
	System::Collections::IEnumerator* myEnum = TheColHeaders->GetEnumerator();
	while (myEnum->MoveNext())
	{
		ColumnHeader* CurrentHeader = __try_cast<ColumnHeader*>(myEnum->Current);

		//ColumnName(ColumnWidth)
		j += sprintf(buffer + j, "%s(%d)", CurrentHeader->Text, CurrentHeader->Width);
	}

	//get the current column order
	int LCount = TheKeyListView->Columns->Count;
	int* LOrder = new int[NUM_KEY_COLS];
	IntPtr KeyViewHandle = (TheKeyListView->Handle);
	HWND m_HWND = reinterpret_cast<HWND> (KeyViewHandle.ToPointer());
	ListView_GetColumnOrderArray(m_HWND, LCount, LOrder); 

	sprintf(buffer + j, ",%d,%d,%d,%d,%d,%d,%d", LOrder[0], LOrder[1], LOrder[2], 
											LOrder[3], LOrder[4], LOrder[5], LOrder[6]);

	WriteRegKey(S"KeyListViewConfig", (String*)buffer);
	
	delete [] LOrder;
}

void SaveConfigurations::LoadKeyListConfig(KeyGUIConfigurationStructPtr TheKeyConfigStruct)
{
	String* retval = static_cast<String*> (ReadRegKey(S"KeyListViewConfig"));
	char* token;
	char* RegValue;
	
	//if the return value is -1 then set width = -1 so the calling function knows that 
	//this is the first run through
	if(!retval)
	{
		TheKeyConfigStruct->width = -1;
		return;
	}
	
	//convert the string* to a char*
	System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(retval));
	RegValue = (static_cast<char *>(static_cast<void *>(ptr)));
	
	//the key value should be in the form width,height
	token = strtok(RegValue, ",");
	TheKeyConfigStruct->width = atoi(token);

	token = strtok(NULL, ",");
	TheKeyConfigStruct->height = atoi(token);

	token = strtok(NULL, ",");
	TheKeyConfigStruct->x_coord = atoi(token);

	token = strtok(NULL, ",");
	TheKeyConfigStruct->y_coord = atoi(token);

	//next group of data deals with the column headers
	for(int cnt = 0; cnt < NUM_KEY_COLS; cnt++)
	{
		token = strtok(NULL, "(");
		token = strtok(NULL, ")");
		TheKeyConfigStruct->ColWidths[cnt] = atoi(token);
	}

	//next group deals with the column ordering
	for(int count = 0; count < NUM_KEY_COLS; count++)
	{
		token = strtok(NULL, ",");
		TheKeyConfigStruct->ColOrder[count] = atoi(token);
	}
}

void SaveConfigurations::SaveSplitterConfig(Splitter* TheSplitter)
{
	Size TheSize = TheSplitter->Size;
	Point TheLocation = TheSplitter->Location;
	char  buffer[10];
	sprintf( buffer, "%d,%d,%d,%d", TheSize.Width, TheSize.Height,
									TheLocation.get_X(), TheLocation.get_Y()
									);
	WriteRegKey(S"SplitterConfig", (String*)buffer);
}

void SaveConfigurations::LoadSplitterConfig(int* width, int* height,
										   int* x_coord, int* y_coord
										   )
{
	String* retval = static_cast<String*> (ReadRegKey(S"SplitterConfig"));
	char* token;
	char* RegValue;
	
	//if the return value is -1 then set width = -1 so the calling function knows that 
	//this is the first run through
	if(!retval)
	{
		*width = -1;
		return;
	}

	System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(retval));
	RegValue = (static_cast<char *>(static_cast<void *>(ptr)));

	//the key value should be in the form width,height
	token = strtok(RegValue, ",");
	*width = atoi(token);

	token = strtok(NULL, ",");
	*height = atoi(token);

	token = strtok(NULL, ",");
	*x_coord = atoi(token);

	token = strtok(NULL, " ");
	*y_coord = atoi(token);
}

void SaveConfigurations::InitRegKeyEditing()
{
	pRegKey = Registry::CurrentUser->CreateSubKey(RegistryAddress);
}

void SaveConfigurations::WriteRegKey(String* name, Object* value)
{
	pRegKey->SetValue(name, value); 
}

Object* SaveConfigurations::ReadRegKey(String* name)
{
	return pRegKey->GetValue(name);
}