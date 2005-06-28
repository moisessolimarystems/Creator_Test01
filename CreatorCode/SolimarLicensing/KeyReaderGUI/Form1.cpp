#include "stdafx.h"
#include "Form1.h"

#define TIMER_INTERVAL					   7000

#define NUM_KEY_COLUMNS					   7
#define NUM_MOD_COLUMNS					   3

#define KEY_NUMBER_COL_WIDTH			   72
#define PRODUCT_ID_COL_WIDTH			   120
#define PRODUCT_VERSION_COL_WIDTH		   87
#define LICENSES_COL_WIDTH				   49
#define ACTIVE_COL_WIDTH				   42
#define HOURS_LEFT_COL_WIDTH			   71
#define EXPIRATION_COL_WIDTH			   150

#define MODULE_NAME_COL_WIDTH			   160
#define IN_USE_COL_WIDTH				   48
#define TOTAL_LICENSES_COL_WIDTH		   81

using namespace KeyReaderGUI;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	System::Threading::Thread::CurrentThread->ApartmentState = System::Threading::ApartmentState::STA;
	Application::Run(new Form1());
	return 0;
}

//Form Constructor. Creates a new form and initializes all of the components.
//The Form contains two vertically aligned panels, which each consist of a listview
//that displays licensing information.
Form1::Form1()
{
	CurrentKeySelected = NULL;

	//The Sizing Manager scales the size of the form with respect to the monitor resolution
	this->TheSizingManager = new ControlSizing(this);

	//SaveConfig is used to save the form/control sizes into the registry
	SaveConfig = new SaveConfigurations(this);
	
	
	//add all of the components to the form
	InitializeComponent();

	//Initialize the major components on the form
	InitializeKeyInfoListView();
	InitializeModListView();
	InitializeMainMenu();
	InitializeForm();
    InitializeModPanel();
	InitializeGUITimer();

	TheAboutBox = new AboutBox_Form();

	//new up a key view manager to manage the key list view
	this->TheKeyViewManager = new KeyInfoListViewManager(this->KeyInfoListView);

    //fill in the key list view with the appropriate values
	TheKeyViewManager->PopulateView();

	this->KeyInfoPanel->ResumeLayout(false);
	this->ModuleLicensePanel->ResumeLayout(false);
	this->ResumeLayout(false);

	//create a new password validator
	PasswordValidater = new PasswordValidation();

	//create the comparer for the key list view
	lvwColumnSorter = new ListViewItemComparer();
	this->KeyInfoListView->ListViewItemSorter = lvwColumnSorter;

	//now create the comparer for the mod list view
	mlvwColumnSorter = new ModListViewComparer();
	this->ModLicenseListView->ListViewItemSorter = mlvwColumnSorter;

    UpdateKeyListView();
}

void Form1::Dispose(Boolean disposing)
{
	//removes all of the components
	if (disposing && components)
	{
		components->Dispose();
	}

	//deletes all of the unmanaged resources
	if(TheKeyViewManager)
	{
		delete TheKeyViewManager;
		TheKeyViewManager = NULL;
	}

	if(this->TheModViewManager)
	{
		delete TheModViewManager;
		TheModViewManager = NULL;
	}
	if(PasswordValidater)
	{
		delete PasswordValidater;
		PasswordValidater = NULL;
	}
	if(this->TheSizingManager)
	{
		delete TheSizingManager;
		TheSizingManager = NULL;
	}
	if(this->SaveConfig)
	{
		delete SaveConfig;
		SaveConfig = NULL;
	}
	__super::Dispose(disposing);
}

//Initializes the form. This function loads the form size and location from registry
void Form1::InitializeGUITimer()
{
	//creates a new UI timer
	myTimer = new System::Windows::Forms::Timer();
	exitFlag = true;

	// Set the timer interval to 7 seconds.
	myTimer->Interval = TIMER_INTERVAL;
	myTimer->Start();

	/* Adds the event and the event handler for the method that will 
	process the timer event to the timer. */
	myTimer->Tick += new EventHandler(this, &Form1::TimerEventProcessor);


	//raises the event.
	while(exitFlag == false) 
	{
		// Processes all the events in the queue.
		Application::DoEvents();
	}
}

//Initializes the form. This function loads the form size and location from registry
void Form1::InitializeForm()
{
	//init form size and location
	int width, height, x_coord, y_coord;

	//get the saved form configurations from the registry
	SaveConfig->LoadFormConfigs(&width, &height, &x_coord, &y_coord);
	
	//if one of those values is -1 than we know that this is the first time the app has ever run
	if(width == -1)
	{
		this->TheSizingManager->InitializeFormSize();
		this->TheSizingManager->InitializeFormLocation();
	}

	else
	{
		// Set the size of the form to be 3/4 the size of working rectangle.
		this->Size = *(__nogc new System::Drawing::Size(width, height));
		this->Location = *(__nogc new System::Drawing::Point(x_coord, y_coord));
	}

	//declare a form closing even handler that will be used to save the user's configs in reg.
	this->Closed += new System::EventHandler(this, &Form1::Form1_Closing);
}

//The ModPanel is the panel on the right of the form. It contains a list that displays the module
//license information. When the form loads we want to intialize it to the same size it was when
//user last closed the application. In order to do this we need to set the size of the mod panel 
//and the entire form and every other control will follow because of how everything is docked. 
void Form1::InitializeModPanel()
{
   	//init form size and location
	int width, height, x_coord, y_coord;

	//get the saved form configurations from the registry
	SaveConfig->LoadModPanelConfig(&width, &height, &x_coord, &y_coord);
	
	//if one of those values is -1 than we know that this is the first time the app has ever run
	if(width != -1)
	{
      this->ModuleLicensePanel->Height = height;
      this->ModuleLicensePanel->Width = width;
	}
}

//The Key info list view is located on the left side of the form and it displays all of
//the key info.
void Form1::InitializeKeyInfoListView()
{
	this->KeyNumber->Text = S"Key Number";
	this->ProductID->Text = S"Product ID";
	this->ProductVersion->Text = S"Product Version";
	this->License->Text = S"License";
	this->Active->Text = S"Active";
	this->HoursLeft->Text = S"Days Left";
	this->ExpirationDate->Text = S"Expiration Date";


	//set the list view attributes
	this->KeyInfoListView->Alignment = ListViewAlignment::SnapToGrid;
	this->KeyInfoListView->AllowColumnReorder = true;
	Windows::Forms::ColumnHeader* __mcTemp__1[] = new ColumnHeader*[NUM_KEY_COLUMNS];
	this->KeyInfoListView->Dock = DockStyle::Fill;
	this->KeyInfoListView->FullRowSelect = true;
	this->KeyInfoListView->Name = S"KeyInfoListView";
	this->KeyInfoListView->TabIndex = 0;
	this->KeyInfoListView->Tag = S"";
	this->KeyInfoListView->View = System::Windows::Forms::View::Details;
	this->KeyInfoListView->MultiSelect = false;

	// Set the initial sorting type for the ListView.
   this->KeyInfoListView->Sorting = SortOrder::None;

	KeyGUIConfigurationStruct KeyConfigStruct;
	memset(&KeyConfigStruct, 0 , sizeof(KeyConfigStruct));

	//Load the saved configurations from the registry.
	this->SaveConfig->LoadKeyListConfig(&KeyConfigStruct);
	
	// -1 represents the first time running this app
	if(KeyConfigStruct.width == -1)
	{
		this->KeyNumber->Width = KEY_NUMBER_COL_WIDTH;
		this->ProductID->Width = PRODUCT_ID_COL_WIDTH;
		this->ProductVersion->Width = PRODUCT_VERSION_COL_WIDTH;
		this->License->Width = LICENSES_COL_WIDTH;
		this->Active->Width = ACTIVE_COL_WIDTH;
		this->HoursLeft->Width = HOURS_LEFT_COL_WIDTH;
		this->ExpirationDate->Width = EXPIRATION_COL_WIDTH;
	}

	else
	{
		this->KeyNumber->Width = KeyConfigStruct.ColWidths[0];
		this->ProductID->Width = KeyConfigStruct.ColWidths[1];
		this->ProductVersion->Width = KeyConfigStruct.ColWidths[2];
		this->License->Width = KeyConfigStruct.ColWidths[3];
		this->Active->Width = KeyConfigStruct.ColWidths[4];
		this->HoursLeft->Width = KeyConfigStruct.ColWidths[5];
		this->ExpirationDate->Width = KeyConfigStruct.ColWidths[6];


		//set the order of the columns in the view
		//get the current column order
		int LCount = KeyInfoListView->Columns->Count;
		IntPtr KeyViewHandle = (KeyInfoListView->Handle);
		HWND m_HWND = reinterpret_cast<HWND> (KeyViewHandle.ToPointer());
		ListView_SetColumnOrderArray(m_HWND, LCount, KeyConfigStruct.ColOrder); 
	}


	//add the event handlers
	this->KeyInfoListView->SelectedIndexChanged += new System::
														EventHandler(this, 
																	 &Form1::KeyList_RowChange
																	);
	this->KeyInfoListView->ColumnClick += new 
											ColumnClickEventHandler(this, 
																	&Form1::KeyList_ColumnClick
																   );

	this->KeyInfoListView->MouseUp += new MouseEventHandler(this, &Form1::KeyList_MouseUp);


	this->KeyInfoListView->MouseDown += new MouseEventHandler(this, &Form1::KeyList_MouseDown);
}

void Form1::InitializeModListView()
{
	//intialize the list view column header names
	this->ModuleName->Text = "Module Name";
	this->LicensesInUse->Text = "In Use";
	this->TotalLicenses->Text = "Total Licenses";
	

	//add the column headers
	ColumnHeader* __mcTemp__3[] = new ColumnHeader*[NUM_MOD_COLUMNS];
	__mcTemp__3[0] = this->ModuleName;
	__mcTemp__3[1] = this->TotalLicenses;
	__mcTemp__3[2] = this->LicensesInUse;
	this->ModLicenseListView->Columns->AddRange(__mcTemp__3);

	ModuleGUIConfigurationStruct ModConfigStruct;
	memset(&ModConfigStruct, 0 , sizeof(ModConfigStruct));

	//load the mod list view info from registry
	this->SaveConfig->LoadModListConfig(&ModConfigStruct);

	//-1 represents the first time running this app
	if(ModConfigStruct.width == -1)
	{
		this->ModuleName->Width = MODULE_NAME_COL_WIDTH;
		this->LicensesInUse->Width = IN_USE_COL_WIDTH;
		this->TotalLicenses->Width = TOTAL_LICENSES_COL_WIDTH;
	}

	//load the init values from registry
	else
	{
		this->ModuleName->Width = ModConfigStruct.FirstModColumnWidth;
		this->TotalLicenses->Width = ModConfigStruct.SecondModColumnWidth;
		this->LicensesInUse->Width = ModConfigStruct.ThirdModColumnWidth;
		
      //set the order of the columns in the view
		//get the current column order
		int LCount = ModLicenseListView->Columns->Count;
		IntPtr ModViewHandle = (ModLicenseListView->Handle);
		HWND m_HWND = reinterpret_cast<HWND> (ModViewHandle.ToPointer());
		ListView_SetColumnOrderArray(m_HWND, LCount, ModConfigStruct.ColOrder); 
	}

	//New up a mod view manager to manage the list view
	this->TheModViewManager = new ModuleLicenseListViewManager(this->ModLicenseListView);

	//add the event handlers for the mod list view
	this->ModLicenseListView->MouseUp += new MouseEventHandler(this, 
															  &Form1::ModList_MouseUp
														      );

	this->ModLicenseListView->MouseUp += new MouseEventHandler(this, 
															  &Form1::ModList_MouseDown
														      );

	this->ModLicenseListView->ColumnClick += new ColumnClickEventHandler(this, 
																	 &Form1::ModList_ColumnClick
																	  );

	this->ModLicenseListView->SelectedIndexChanged += new EventHandler(this, 
																	   &Form1::ModList_RowChange
																	  );
}
System::Void Form1::ExitMenuItem_Click(System::Object *  sender, System::EventArgs *  e)
{
	Form1_Closing(sender, e);
	Application::Exit();
}

void Form1::InitializeMainMenu()
{
   this->refreshMenuItem->Click += new System::EventHandler(this, Form1::refreshMenuItem_Click);
   this->EnterPasswordMenuItem->Click += new System::EventHandler(this, Form1::EnterPasswordMenuItem_Click);
   this->AboutSolimar->Click += new System::EventHandler(this, Form1::AboutSolimar_Click);
}

void Form1::AboutSolimar_Click(Object* sender, System::EventArgs* e)
{
	TheAboutBox->ShowDialog();
}


System::Void Form1::PasswordMenuItem_Select(System::Object *  sender, System::EventArgs *  e)
{
	if(this->KeyInfoListView->Items->Count > 0)
	{
		EnterPasswordMenuItem->Enabled = true;
		AddPasswordPacketMenuItem->Enabled = true;
	}
	else
	{
		EnterPasswordMenuItem->Enabled = false;
		AddPasswordPacketMenuItem->Enabled = false;
	}
}

void Form1::EnterPasswordMenuItem_Click(Object* sender, System::EventArgs* e) 
{
	ThePasswordForm = new PasswordForm(PasswordValidater);
	ThePasswordForm->ShowDialog();
	UpdateViews();
	ThePasswordForm->Dispose(true);
	delete ThePasswordForm;
	ThePasswordForm = NULL;
}

// Bring up a dialog to open a file.
System::Void Form1::AddPasswordPacketMenuItem_Click(System::Object *  sender, System::EventArgs *  e)
{
		// Show the FolderBrowserDialog.
    System::Windows::Forms::DialogResult result = openFileDialog1->ShowDialog();

	
	//if OK button was pressed
	if (result == DialogResult::OK) 
	{
		
			//get the file name
			String* TheFileName = openFileDialog1->FileName;
			BSTR bstr_FileName;
		
			//convert the string* to a BSTR
			System::IntPtr ptr(System::Runtime::InteropServices::Marshal::StringToBSTR(TheFileName));
			bstr_FileName = (static_cast<BSTR>(static_cast<void *>(ptr)));
			
			//process the file
			PasswordValidater->ProcessPasswordFile(bstr_FileName);
			System::Runtime::InteropServices::Marshal::FreeBSTR(ptr);
    }
}



// Refresh the window
void Form1::refreshMenuItem_Click(Object* sender, System::EventArgs* e) 
{
	//updates the views
 	UpdateViews();
}

void Form1::KeyList_RowChange(Object* sender, System::EventArgs* e)
{
	//stop the timer
	exitFlag = true;
	myTimer->Enabled = false;

	this->ModLicenseListView->BeginUpdate();

	//get the list of the items selected in the key list view
	ListView::SelectedListViewItemCollection* RowItems = this->KeyInfoListView->SelectedItems;

	//get a list of all of the items in the mod license list view
	ListView::ListViewItemCollection* EveryItem = new ListView::
												ListViewItemCollection(this->ModLicenseListView);
	//clear every row in the mod license list view
	EveryItem->Clear();
	
	//enumerate through the selected rows
	System::Collections::IEnumerator* myEnum = RowItems->GetEnumerator();
	while (myEnum->MoveNext())
	{
		//grab the key number from the selected row  and populate the 
		//mod license list view based on the new key value
		ListViewItem* CurrentItem = __try_cast<ListViewItem*>(myEnum->Current);
		CurrentKeySelected = CurrentItem->Text;
		this->TheModViewManager->SetKeyID(CurrentKeySelected);
		TheModViewManager->PopulateView();
	}
	this->ModLicenseListView->EndUpdate();

	//restart the timer
	myTimer->Enabled = true;
	exitFlag = false;
}

void Form1::ModList_RowChange(Object* sender, System::EventArgs* e)
{
	//stop the timer
	exitFlag = true;
	myTimer->Enabled = false;

	//get the selected item in the mod license view and unselect it
	ListView::SelectedListViewItemCollection* RowItems = this->ModLicenseListView->SelectedItems;
	
	System::Collections::IEnumerator* myEnum = RowItems->GetEnumerator();
	while (myEnum->MoveNext())
	{
		ListViewItem* CurrentItem = __try_cast<ListViewItem*>(myEnum->Current);
		CurrentItem->Selected = false;
		CurrentItem->Focused = false;
	}

	//restart the timer
	myTimer->Enabled = true;
	exitFlag = false;

	//instead, continue selecting the current key in the key list view
	this->KeyInfoListView->Focus();
	SelectCurrentKey();
}

//handles the key list sorting
void Form1::KeyList_ColumnClick(Object* /*o*/, ColumnClickEventArgs* e) 
{
	// Determine if the clicked column is already the column that is being sorted.
	if ( e->Column == lvwColumnSorter->SortColumn )
	{
		// Reverse the current sort direction for this column.
		if (lvwColumnSorter->Order == SortOrder::Ascending)
		{
			lvwColumnSorter->Order = SortOrder::Descending;
		}
		else
		{
			lvwColumnSorter->Order = SortOrder::Ascending;
		}
	}
	else
	{
		// Set the column number that is to be sorted. By default, this is in ascending order.
		lvwColumnSorter->SortColumn = e->Column;
		lvwColumnSorter->Order = SortOrder::Ascending;
	}

	// Perform the sort with these new sort options.
	this->KeyInfoListView->Sort();
}

//sorts the mod list based on the column clicked on by the user
void Form1::ModList_ColumnClick(Object* /*o*/, ColumnClickEventArgs* e) 
{
	// Determine if the clicked column is already the column that is being sorted.
	if ( e->Column == mlvwColumnSorter->SortColumn )
	{
		// Reverse the current sort direction for this column.
		if (mlvwColumnSorter->Order == SortOrder::Ascending)
		{
			mlvwColumnSorter->Order = SortOrder::Descending;
		}
		else
		{
			mlvwColumnSorter->Order = SortOrder::Ascending;
		}
	}
	else
	{
		// Set the column number that is to be sorted. By default, this is in ascending order.
		mlvwColumnSorter->SortColumn = e->Column;
		mlvwColumnSorter->Order = SortOrder::Ascending;
	}

	// Perform the sort with these new sort options.
	this->ModLicenseListView->Sort();
	SelectCurrentKey();
}

//gets called when the UI timer goes off
void Form1::TimerEventProcessor(Object* myObject,  EventArgs* myEventArgs)
{
	//update the key listviews
//	UpdateKeyListView();
}

void Form1::StartTimer()
{
   //restart the timer
	myTimer->Enabled = true;
	exitFlag = false;
}

void Form1::StopTimer()
{
	//stop the timer
	exitFlag = true;
	myTimer->Enabled = false;
}

//updates the key list view
void Form1::UpdateKeyListView()
{
   StopTimer();
   ListView::ListViewItemCollection* EveryKeyItem = new ListView::
												ListViewItemCollection(this->KeyInfoListView);

	//clear the key view; populate with the new values and select the current key
	EveryKeyItem->Clear();
	if(!this->TheKeyViewManager->PopulateView())
		return;

	SelectCurrentKey();
   StartTimer();
}

//updates the key list and mod license views
void Form1::UpdateViews()
{
   StopTimer();
   ListView::ListViewItemCollection* EveryKeyItem = new ListView::
												ListViewItemCollection(this->KeyInfoListView);

	//clear the key view; populate with the new values and select the current key
	EveryKeyItem->Clear();
	if(!this->TheKeyViewManager->PopulateView())
		return;

	SelectCurrentKey();
	
	//if there is a mod view; clear it and update with updated values
	if(TheModViewManager->IsInitialized())
	{
		this->ModLicenseListView->BeginUpdate();
		ListView::ListViewItemCollection* EveryItem = new ListView::
												ListViewItemCollection(this->ModLicenseListView);

		EveryItem->Clear();
		TheModViewManager->PopulateView();
		this->ModLicenseListView->EndUpdate();
	}
   StartTimer();
}
void Form1::SelectCurrentKey()
{
	this->KeyInfoListView->Focus();

	if(CurrentKeySelected)
	{
		ListView::ListViewItemCollection* EveryViewItem = new ListView::
													ListViewItemCollection(this->KeyInfoListView);
		
		System::Collections::IEnumerator* myEnum = EveryViewItem->GetEnumerator();
		while (myEnum->MoveNext())
		{
			CurrentItem = __try_cast<ListViewItem*>(myEnum->Current);
			if(CurrentItem->Text->Equals(CurrentKeySelected))
			{
				CurrentItem->Selected = true;
				CurrentItem->Focused = true;
				break;
			}
		}
	}
}

//Highlights the entire row when the user clicks on an individual cell
void Form1::KeyList_MouseUp(Object* sender, MouseEventArgs* e)
{
	//restart the timer
   StartTimer();
}

void Form1::KeyList_MouseDown(Object* sender, MouseEventArgs* e)
{
	//stop the timer
	StopTimer();
}

//Highlights the entire row when the user clicks on an individual cell
void Form1::ModList_MouseUp(Object* sender, MouseEventArgs* e)
{
	SelectCurrentKey();
}

//Highlights the entire row when the user clicks on an individual cell
void Form1::ModList_MouseDown(Object* sender, MouseEventArgs* e)
{
	SelectCurrentKey();
}

void Form1::Form1_Closing(Object* sender, EventArgs* e)
{
	SaveConfig->SaveFormConfigs();
	SaveConfig->SaveKeyPanelConfig(this->KeyInfoPanel);
	SaveConfig->SaveModPanelConfig(this->ModuleLicensePanel);
	SaveConfig->SaveKeyListConfig(this->KeyInfoListView);
	SaveConfig->SaveModListConfig(this->ModLicenseListView);
	SaveConfig->SaveSplitterConfig(this->splitter1);
}
