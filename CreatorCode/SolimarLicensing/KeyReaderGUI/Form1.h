#ifndef __FORM1_H__
#define __FORM1_H__

#include "KeyInfoListViewManager.h"
#include "PasswordValidation.h"
#include "PasswordForm.h"
#include "AboutBox_Form.h"
#include "ListViewItemComparer.h"
#include "ModListViewItemComparer.h"
#include "ControlSizing.h"
#include "SaveConfigurations.h"
			
#ifdef GetObject
#undef GetObject
#endif

namespace KeyReaderGUI
{
	using namespace System;
	using namespace System::IO;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace KeyViewManager;
	using namespace PWDValidation;
	using namespace PWDForm;
	using namespace ListViewComparer;
	using namespace ModListViewItemComparer;
	using namespace SizeControl;
	using namespace SaveConfig;

	/// <summary> 
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public __gc class Form1 : public System::Windows::Forms::Form
	{	
	public:
		Form1();
		  
	protected:
		void Dispose(Boolean disposing);

	private:
		//private methods
		void SelectCurrentKey();
      void StartTimer();
      void StopTimer();
      void UpdateKeyListView();
		void UpdateViews();

		//helper methods used to initialize the components on the form
      void InitializeModPanel();
		void InitializeKeyInfoListView();
		void InitializeModListView();
		void InitializeMainMenu();
		void InitializeForm();
		void InitializeGUITimer();

   private: System::Windows::Forms::OpenFileDialog *  openFileDialog1;
   private: System::Windows::Forms::MainMenu *  mainMenu1;
   private: System::Windows::Forms::Panel *  ModuleLicensePanel;
   private: System::Windows::Forms::Splitter *  splitter1;
   private: System::Windows::Forms::Panel *  KeyInfoPanel;
   private: System::Windows::Forms::ListView *  ModLicenseListView;
   private: System::Windows::Forms::ListView *  KeyInfoListView;
   private: System::Windows::Forms::StatusBar *  KeyReaderStatusBar;
   private: System::Windows::Forms::MenuItem *  fileMenuItem;

   private: System::Windows::Forms::MenuItem *  viewMenuItem;
   private: System::Windows::Forms::MenuItem *  refreshMenuItem;
   private: System::Windows::Forms::MenuItem *  PasswordMenuItem;
   private: System::Windows::Forms::MenuItem *  EnterPasswordMenuItem;
   private: System::Windows::Forms::MenuItem *  HelpMenuItem;
   private: System::Windows::Forms::MenuItem *  AboutSolimar;

   private: System::Windows::Forms::ColumnHeader *  newcolumn;
	private: System::Windows::Forms::ColumnHeader*  KeyNumber;
	private: System::Windows::Forms::ColumnHeader*  ProductID;
	private: System::Windows::Forms::ColumnHeader*  ProductVersion;
	private: System::Windows::Forms::ColumnHeader*  License;
	private: System::Windows::Forms::ColumnHeader*  Active;
	private: System::Windows::Forms::ColumnHeader*  HoursLeft;
   private: System::Windows::Forms::ColumnHeader*  ModuleName;
	private: System::Windows::Forms::ColumnHeader*  LicensesInUse;
	private: System::Windows::Forms::ColumnHeader*  TotalLicenses;
	private: System::Windows::Forms::ColumnHeader*  ExpirationDate;
	private: System::Windows::Forms::MenuItem *  ExitMenuItem;
	private: System::Windows::Forms::MenuItem *  AddPasswordPacketMenuItem;
	private:	System::Windows::Forms::Timer    *  myTimer;

   //Event Handler methods
	void Form1::KeyList_RowChange(Object* sender, System::EventArgs* e);
	void Form1::ModList_RowChange(Object* sender, System::EventArgs* e);
	void Form1::KeyList_ColumnClick(Object* o, ColumnClickEventArgs* e);
	void Form1::ModList_ColumnClick(Object* /*o*/, ColumnClickEventArgs* e); 
	void Form1::KeyList_MouseUp(Object* sender, MouseEventArgs* e);
	void Form1::KeyList_MouseDown(Object* sender, MouseEventArgs* e);
	void Form1::ModList_MouseUp(Object* sender, MouseEventArgs* e);
	void Form1::ModList_MouseDown(Object* sender, MouseEventArgs* e);
	void Form1::EnterPasswordMenuItem_Click(Object* sender, System::EventArgs* e);
  	void Form1::refreshMenuItem_Click(Object* sender, System::EventArgs* e);
	void Form1::AboutSolimar_Click(Object* sender, System::EventArgs* e);
	void Form1_Closing(Object* sender, EventArgs* e);

	//method that runs when the Timer is signaled
	void TimerEventProcessor(Object* myObject, EventArgs* myEventArgs);

	//File Menu Item Event Handlers
	System::Void ExitMenuItem_Click(System::Object *  sender, System::EventArgs *  e);

	//Password Menu Item Event Handlers
    System::Void PasswordMenuItem_Select(System::Object *  sender, System::EventArgs *  e);
	System::Void AddPasswordPacketMenuItem_Click(System::Object *  sender, System::EventArgs *  e);

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container * components;

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>


		void InitializeComponent(void)
		{
			System::Resources::ResourceManager *  resources = new System::Resources::ResourceManager(__typeof(KeyReaderGUI::Form1));
			this->KeyReaderStatusBar = new System::Windows::Forms::StatusBar();
			this->openFileDialog1 = new System::Windows::Forms::OpenFileDialog();
			this->mainMenu1 = new System::Windows::Forms::MainMenu();
			this->fileMenuItem = new System::Windows::Forms::MenuItem();
			this->ExitMenuItem = new System::Windows::Forms::MenuItem();
			this->viewMenuItem = new System::Windows::Forms::MenuItem();
			this->refreshMenuItem = new System::Windows::Forms::MenuItem();
			this->PasswordMenuItem = new System::Windows::Forms::MenuItem();
			this->EnterPasswordMenuItem = new System::Windows::Forms::MenuItem();
			this->AddPasswordPacketMenuItem = new System::Windows::Forms::MenuItem();
			this->HelpMenuItem = new System::Windows::Forms::MenuItem();
			this->AboutSolimar = new System::Windows::Forms::MenuItem();
			this->ModuleLicensePanel = new System::Windows::Forms::Panel();
			this->ModLicenseListView = new System::Windows::Forms::ListView();
			this->splitter1 = new System::Windows::Forms::Splitter();
			this->KeyInfoPanel = new System::Windows::Forms::Panel();
			this->KeyInfoListView = new System::Windows::Forms::ListView();
			this->KeyNumber = new System::Windows::Forms::ColumnHeader();
			this->ProductID = new System::Windows::Forms::ColumnHeader();
			this->ProductVersion = new System::Windows::Forms::ColumnHeader();
			this->License = new System::Windows::Forms::ColumnHeader();
			this->Active = new System::Windows::Forms::ColumnHeader();
			this->HoursLeft = new System::Windows::Forms::ColumnHeader();
			this->ExpirationDate = new System::Windows::Forms::ColumnHeader();
			this->ModuleName = new System::Windows::Forms::ColumnHeader();
			this->LicensesInUse = new System::Windows::Forms::ColumnHeader();
			this->TotalLicenses = new System::Windows::Forms::ColumnHeader();
			this->ModuleLicensePanel->SuspendLayout();
			this->KeyInfoPanel->SuspendLayout();
			this->SuspendLayout();
			// 
			// KeyReaderStatusBar
			// 
			this->KeyReaderStatusBar->Location = System::Drawing::Point(0, 373);
			this->KeyReaderStatusBar->Name = S"KeyReaderStatusBar";
			this->KeyReaderStatusBar->Size = System::Drawing::Size(712, 32);
			this->KeyReaderStatusBar->TabIndex = 0;
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->Filter = S"packet files (*.pkt)|*.pkt|All files (*.*)|*.*";
			this->openFileDialog1->InitialDirectory = S"C:\\";
			// 
			// mainMenu1
			// 
			System::Windows::Forms::MenuItem* __mcTemp__1[] = new System::Windows::Forms::MenuItem*[4];
			__mcTemp__1[0] = this->fileMenuItem;
			__mcTemp__1[1] = this->viewMenuItem;
			__mcTemp__1[2] = this->PasswordMenuItem;
			__mcTemp__1[3] = this->HelpMenuItem;
			this->mainMenu1->MenuItems->AddRange(__mcTemp__1);
			// 
			// fileMenuItem
			// 
			this->fileMenuItem->Index = 0;
			System::Windows::Forms::MenuItem* __mcTemp__2[] = new System::Windows::Forms::MenuItem*[1];
			__mcTemp__2[0] = this->ExitMenuItem;
			this->fileMenuItem->MenuItems->AddRange(__mcTemp__2);
			this->fileMenuItem->Text = S"File";
			// 
			// ExitMenuItem
			// 
			this->ExitMenuItem->Index = 0;
			this->ExitMenuItem->Text = S"&Exit";
			this->ExitMenuItem->Click += new System::EventHandler(this, ExitMenuItem_Click);
			// 
			// viewMenuItem
			// 
			this->viewMenuItem->Index = 1;
			System::Windows::Forms::MenuItem* __mcTemp__3[] = new System::Windows::Forms::MenuItem*[1];
			__mcTemp__3[0] = this->refreshMenuItem;
			this->viewMenuItem->MenuItems->AddRange(__mcTemp__3);
			this->viewMenuItem->Text = S"View";
			// 
			// refreshMenuItem
			// 
			this->refreshMenuItem->Index = 0;
			this->refreshMenuItem->Text = S"Refresh";
			// 
			// PasswordMenuItem
			// 
			this->PasswordMenuItem->Index = 2;
			System::Windows::Forms::MenuItem* __mcTemp__4[] = new System::Windows::Forms::MenuItem*[2];
			__mcTemp__4[0] = this->EnterPasswordMenuItem;
			__mcTemp__4[1] = this->AddPasswordPacketMenuItem;
			this->PasswordMenuItem->MenuItems->AddRange(__mcTemp__4);
			this->PasswordMenuItem->Text = S"Password";
			this->PasswordMenuItem->Select += new System::EventHandler(this, PasswordMenuItem_Select);
			// 
			// EnterPasswordMenuItem
			// 
			this->EnterPasswordMenuItem->Index = 0;
			this->EnterPasswordMenuItem->Text = S"Enter Password";
			// 
			// AddPasswordPacketMenuItem
			// 
			this->AddPasswordPacketMenuItem->Index = 1;
			this->AddPasswordPacketMenuItem->Text = S"&Add Password Packet";
			this->AddPasswordPacketMenuItem->Click += new System::EventHandler(this, AddPasswordPacketMenuItem_Click);
			// 
			// HelpMenuItem
			// 
			this->HelpMenuItem->Index = 3;
			System::Windows::Forms::MenuItem* __mcTemp__5[] = new System::Windows::Forms::MenuItem*[1];
			__mcTemp__5[0] = this->AboutSolimar;
			this->HelpMenuItem->MenuItems->AddRange(__mcTemp__5);
			this->HelpMenuItem->Text = S"Help";
			// 
			// AboutSolimar
			// 
			this->AboutSolimar->Index = 0;
			this->AboutSolimar->Text = S"About License Manager";
			// 
			// ModuleLicensePanel
			// 
			this->ModuleLicensePanel->Controls->Add(this->ModLicenseListView);
			this->ModuleLicensePanel->Dock = System::Windows::Forms::DockStyle::Right;
			this->ModuleLicensePanel->Location = System::Drawing::Point(512, 0);
			this->ModuleLicensePanel->Name = S"ModuleLicensePanel";
			this->ModuleLicensePanel->Size = System::Drawing::Size(200, 373);
			this->ModuleLicensePanel->TabIndex = 1;
			// 
			// ModLicenseListView
			// 
			this->ModLicenseListView->BackColor = System::Drawing::SystemColors::InactiveBorder;
			this->ModLicenseListView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ModLicenseListView->Location = System::Drawing::Point(0, 0);
			this->ModLicenseListView->Name = S"ModLicenseListView";
			this->ModLicenseListView->Size = System::Drawing::Size(200, 373);
			this->ModLicenseListView->TabIndex = 0;
			this->ModLicenseListView->View = System::Windows::Forms::View::Details;
			// 
			// splitter1
			// 
			this->splitter1->BackColor = System::Drawing::SystemColors::ActiveBorder;
			this->splitter1->Dock = System::Windows::Forms::DockStyle::Right;
			this->splitter1->Location = System::Drawing::Point(509, 0);
			this->splitter1->Name = S"splitter1";
			this->splitter1->Size = System::Drawing::Size(3, 373);
			this->splitter1->TabIndex = 2;
			this->splitter1->TabStop = false;
			// 
			// KeyInfoPanel
			// 
			this->KeyInfoPanel->Controls->Add(this->KeyInfoListView);
			this->KeyInfoPanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->KeyInfoPanel->Location = System::Drawing::Point(0, 0);
			this->KeyInfoPanel->Name = S"KeyInfoPanel";
			this->KeyInfoPanel->Size = System::Drawing::Size(509, 373);
			this->KeyInfoPanel->TabIndex = 3;
			// 
			// KeyInfoListView
			// 
			System::Windows::Forms::ColumnHeader* __mcTemp__6[] = new System::Windows::Forms::ColumnHeader*[7];
			__mcTemp__6[0] = this->KeyNumber;
			__mcTemp__6[1] = this->ProductID;
			__mcTemp__6[2] = this->ProductVersion;
			__mcTemp__6[3] = this->License;
			__mcTemp__6[4] = this->Active;
			__mcTemp__6[5] = this->HoursLeft;
			__mcTemp__6[6] = this->ExpirationDate;
			this->KeyInfoListView->Columns->AddRange(__mcTemp__6);
			this->KeyInfoListView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->KeyInfoListView->Location = System::Drawing::Point(0, 0);
			this->KeyInfoListView->Name = S"KeyInfoListView";
			this->KeyInfoListView->Size = System::Drawing::Size(509, 373);
			this->KeyInfoListView->TabIndex = 0;
			this->KeyInfoListView->View = System::Windows::Forms::View::Details;
			// 
			// Form1
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->ClientSize = System::Drawing::Size(712, 405);
			this->Controls->Add(this->KeyInfoPanel);
			this->Controls->Add(this->splitter1);
			this->Controls->Add(this->ModuleLicensePanel);
			this->Controls->Add(this->KeyReaderStatusBar);
			this->Icon = (__try_cast<System::Drawing::Icon *  >(resources->GetObject(S"$this.Icon")));
			this->Menu = this->mainMenu1;
			this->Name = S"Form1";
			this->Text = S"Solimar License Manager";
			this->ModuleLicensePanel->ResumeLayout(false);
			this->KeyInfoPanel->ResumeLayout(false);
			this->ResumeLayout(false);

		}

        KeyInfoListViewManager* TheKeyViewManager;
		ModuleLicenseListViewManager* TheModViewManager;
		ListViewItem* CurrentItem;
		bool exitFlag;
		Object* CurrentKeySelected;
		PasswordValidation* PasswordValidater;
		PasswordForm* ThePasswordForm;
		AboutBox_Form* TheAboutBox;
		ListViewItemComparer* lvwColumnSorter;
		ModListViewComparer* mlvwColumnSorter;
		ControlSizing* TheSizingManager;
        SaveConfigurations* SaveConfig;

		 

};
}

#endif //form1.h
