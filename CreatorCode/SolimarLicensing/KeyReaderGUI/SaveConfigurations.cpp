#include "stdafx.h"
#include "SaveConfigurations.h"

using namespace SaveConfig;
//Constructor. 
//Initializes the member variables and creates a table of appropriate size.
SaveConfigurations::SaveConfigurations(Form* TheForm) 
									  : MainForm(TheForm)	
{
	settingsFileName = "SolimarLicenseManager.config";
	formConfig = new FormSettings();
}

//Initializes the resources
SaveConfigurations::~SaveConfigurations(){}

void SaveConfigurations::SaveFormConfigs()
{
	formConfig->FormSize = MainForm->Size;
	formConfig->FormLocation = MainForm->Location;
}

void SaveConfigurations::LoadFormConfigs(Point* FormLocation, Size* FormSize)
{
	//if the return value is -1 then set width = -1 so the calling function knows that 
	//this is the first run through
	
	if(formConfig->FormSize.Width < 0) //figure out how to detect first time
	{
		FormSize->Width = -1;
		return;
	}
	
	*FormLocation = formConfig->FormLocation;
	*FormSize = formConfig->FormSize;
}


void SaveConfigurations::SaveModListConfig(ListView* TheModListView)
{
	formConfig->ModListSize = TheModListView->Size;
	formConfig->ModListLocation = TheModListView->Location;

	//get the collection of headers that belong to the mod list view
	ListView::ColumnHeaderCollection* TheColHeaders = new ListView::
														ColumnHeaderCollection(TheModListView);
	//enumerate through the column headers
	System::Collections::IEnumerator* myEnum = TheColHeaders->GetEnumerator();
	int j = 0;
	while (myEnum->MoveNext())
	{
		//extract the headers one at a time
		ColumnHeader* CurrentHeader = __try_cast<ColumnHeader*>(myEnum->Current);
		formConfig->ModColumnWidth[j] = CurrentHeader->Width;
		j++;
	}

	//get the current column order
	int LCount = TheModListView->Columns->Count;
	int* LOrder = new int[NUM_MOD_COLS];
	IntPtr ModViewHandle = (TheModListView->Handle);
	HWND m_HWND = reinterpret_cast<HWND> (ModViewHandle.ToPointer());
	ListView_GetColumnOrderArray(m_HWND, LCount, LOrder); 

	for(int i=0; i < NUM_MOD_COLS; i++)
	{
		formConfig->ModColOrder[i] = LOrder[i];
	}
	
	delete [] LOrder;
}

void SaveConfigurations::LoadModListConfig(ModuleGUIConfigurationStructPtr TheModConfigStructPtr)
{
	//if the return value is -1 then set width = -1 so the calling function knows that 
	//this is the first run through
	if(formConfig->ModListSize.Width < 0)
	{
		TheModConfigStructPtr->width = -1;
		return;
	}

	TheModConfigStructPtr->width = formConfig->ModListSize.Width;
	TheModConfigStructPtr->height = formConfig->ModListSize.Height;
	TheModConfigStructPtr->x_coord = formConfig->ModListLocation.X;
	TheModConfigStructPtr->y_coord = formConfig->ModListLocation.Y;

	//next group of data deals with the column headers
	//next group deals with the column ordering. i.e. the user moves the columns around.
	for(int count = 0; count < NUM_MOD_COLS; count++)
	{
		TheModConfigStructPtr->ColumnWidth[count] = formConfig->ModColumnWidth[count];
		TheModConfigStructPtr->ColOrder[count] = formConfig->ModColOrder[count];
	}
}

void SaveConfigurations::SaveKeyListConfig(ListView* TheKeyListView)
{
	formConfig->KeyListSize = TheKeyListView->Size;
	formConfig->KeyListLocation = TheKeyListView->Location;
	

	ListView::ColumnHeaderCollection* TheColHeaders = new ListView::
														ColumnHeaderCollection(TheKeyListView);
	//enumerate through the column headers
	System::Collections::IEnumerator* myEnum = TheColHeaders->GetEnumerator();
	int j = 0;
	while (myEnum->MoveNext())
	{
		ColumnHeader* CurrentHeader = __try_cast<ColumnHeader*>(myEnum->Current);
		formConfig->KeyColWidth[j] = CurrentHeader->Width;	
		j++;
	}

	//get the current column order
	int LCount = TheKeyListView->Columns->Count;
	int* LOrder = new int[NUM_KEY_COLS];
	IntPtr KeyViewHandle = (TheKeyListView->Handle);
	HWND m_HWND = reinterpret_cast<HWND> (KeyViewHandle.ToPointer());
	ListView_GetColumnOrderArray(m_HWND, LCount, LOrder); 

	for(int i=0; i < NUM_KEY_COLS; i++)
	{
		formConfig->KeyColOrder[i] = LOrder[i];
	}
	
	delete [] LOrder;
}

#undef MessageBox
void SaveConfigurations::LoadKeyListConfig(KeyGUIConfigurationStructPtr TheKeyConfigStruct)
{
	//if the return value is -1 then set width = -1 so the calling function knows that 
	//this is the first run through
	if(formConfig->KeyListSize.Width < 0)
	{
		TheKeyConfigStruct->width = -1;
		return;
	}
	
	TheKeyConfigStruct->width = formConfig->KeyListSize.Width;
	TheKeyConfigStruct->height = formConfig->KeyListSize.Height;
	TheKeyConfigStruct->x_coord = formConfig->KeyListLocation.X;
	TheKeyConfigStruct->y_coord =  formConfig->KeyListLocation.Y;

	//next group of data deals with the column headers
	for(int count = 0; count < NUM_KEY_COLS; count++)
	{
		TheKeyConfigStruct->ColWidths[count] =  formConfig->KeyColWidth[count];
		TheKeyConfigStruct->ColOrder[count] = formConfig->KeyColOrder[count];
	}
}

#undef MessageBox
void SaveConfigurations::LoadSettings()
{
	try
	{
		IsolatedStorageFile* isoStore = IsolatedStorageFile::GetUserStoreForDomain();
		//open settings file
		if (isoStore->GetFileNames(settingsFileName)->Length == 0 )
		{
			// File not exists. Let us NOT try to DeSerialize it.
			return;
		}

		// Read the stream from Isolated Storage.
		Stream* stream = new IsolatedStorageFileStream(settingsFileName, FileMode::OpenOrCreate, isoStore );
		if ( stream != NULL )
		{
			try
			{
				// DeSerialize the FormSettings from stream.
				IFormatter* formatter = new BinaryFormatter();
				formConfig = __try_cast<FormSettings*>(formatter->Deserialize(stream));
			}
			catch(SerializationException* e)		
			{
				MessageBox::Show(MainForm, System::String::Concat("Failed to Load Settings : ", e->Message), "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			stream->Close();
		}
	}
	catch(Exception*)
	{
		//Exception thrown trying to retrieve IsolatedStorage. Skip loading settings.
	}
}

void SaveConfigurations::SaveSettings()
{
	// Open the stream from the IsolatedStorage.
	try
	{
		IsolatedStorageFile* isoStore = IsolatedStorageFile::GetUserStoreForDomain();

		Stream* stream = new IsolatedStorageFileStream(settingsFileName, FileMode::Create, isoStore );

		if ( stream != NULL )
		{
			try
			{
				// Serialize the FormSettings into the IsolatedStorage.
				IFormatter* formatter = new BinaryFormatter();
				formatter->Serialize(stream, formConfig);
			}
			catch(SerializationException* e)
			{
				MessageBox::Show(MainForm, System::String::Concat("Failed to Save Settings : ",e->Message), "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			stream->Close();
		}
	}
	catch(Exception*)
	{
		//Exception thrown trying to retrieve IsolatedStorage. Skip saving settings.
	}
}
