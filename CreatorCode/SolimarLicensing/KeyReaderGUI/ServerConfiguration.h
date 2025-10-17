#pragma once

#include "ConnectionSettings.h"

namespace KeyReaderGUI
{
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace Microsoft::Win32;
using namespace System::Xml;
using namespace System::IO;
using namespace System::Security::AccessControl;


//initial key created when installed
#define SOLIMAR_KEY						L"SOFTWARE\\Solimar"
#define SOLIMAR_KEY_32					L"SOFTWARE\\Wow6432Node\\Solimar"
#define LICENSE_PRODUCT					L"Solimar Licensing"
//XML Elements & Attributes
#define LICENSING_STRUCTURE_ELEMENT		L"LicensingStructure"
#define SERVER_STRUCTURE_ELEMENT		L"ServerStructure"
#define VERSION_ELEMENT					L"Version"
#define VALUE_ATTRIB					L"value"
#define SERVER_ELEMENT					L"Server"
#define PRODUCT_ELEMENT					L"Product"
#define ID_ATTRIB						L"id"
#define NAME_ATTRIB						L"name"
#define BACKUP_ATTRIB					L"backup"
#define SHARED_ATTRIB					L"shared"
//XML File/Folder Path
#define LOCAL_CONNECTION_FILE_PATH		L"\\Solimar\\SolimarLicenseServer\\"
#define LOCAL_CONNECTION_FILE			L"LocalConnectionSettings.xml"
	/// <summary> 
	/// Summary for ServerConfiguration
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public __gc class ServerConfiguration : public System::Windows::Forms::Form
	{
	public: 
		ServerConfiguration(void)
		{
			InitializeComponent();
			m_ValueChanged = false;
		}
		~ServerConfiguration(){this->Dispose(true);}
        
	protected: 
		void Dispose(Boolean disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}
	private: System::Windows::Forms::Panel*  TopPanel;
	private: System::Windows::Forms::Panel*  BottomPanel;


	private: System::Windows::Forms::Button*  CancelBtn;
	private: System::Windows::Forms::Button*  OKBtn;

	private: System::Windows::Forms::Panel*  MiddlePanel;
	private: System::Windows::Forms::PropertyGrid*  ServerPropertyGrid;
	private: System::Windows::Forms::ComboBox*  ProductComboBox;



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container* components;

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->TopPanel = (new System::Windows::Forms::Panel());
			this->ProductComboBox = (new System::Windows::Forms::ComboBox());
			this->BottomPanel = (new System::Windows::Forms::Panel());
			this->CancelBtn = (new System::Windows::Forms::Button());
			this->OKBtn = (new System::Windows::Forms::Button());
			this->MiddlePanel = (new System::Windows::Forms::Panel());
			this->ServerPropertyGrid = (new System::Windows::Forms::PropertyGrid());
			this->TopPanel->SuspendLayout();
			this->BottomPanel->SuspendLayout();
			this->MiddlePanel->SuspendLayout();
			this->SuspendLayout();
			// 
			// TopPanel
			// 
			this->TopPanel->Controls->Add(this->ProductComboBox);
			this->TopPanel->Dock = System::Windows::Forms::DockStyle::Top;
			this->TopPanel->Location = System::Drawing::Point(0, 0);
			this->TopPanel->Name = S"TopPanel";
			this->TopPanel->Size = System::Drawing::Size(356, 21);
			this->TopPanel->TabIndex = 0;
			// 
			// ProductComboBox
			// 
			this->ProductComboBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ProductComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->ProductComboBox->FormattingEnabled = true;
			this->ProductComboBox->Location = System::Drawing::Point(0, 0);
			this->ProductComboBox->Name = S"ProductComboBox";
			this->ProductComboBox->Size = System::Drawing::Size(356, 21);
			this->ProductComboBox->TabIndex = 1;
			this->ProductComboBox->SelectedIndexChanged += new System::EventHandler(this, &ServerConfiguration::ProductComboBox_SelectedIndexChanged);
			// 
			// BottomPanel
			// 
			this->BottomPanel->Controls->Add(this->CancelBtn);
			this->BottomPanel->Controls->Add(this->OKBtn);
			this->BottomPanel->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->BottomPanel->Location = System::Drawing::Point(0, 268);
			this->BottomPanel->Name = S"BottomPanel";
			this->BottomPanel->Size = System::Drawing::Size(356, 42);
			this->BottomPanel->TabIndex = 1;
			// 
			// CancelBtn
			// 
			this->CancelBtn->Location = System::Drawing::Point(281, 11);
			this->CancelBtn->Name = S"CancelBtn";
			this->CancelBtn->Size = System::Drawing::Size(75, 23);
			this->CancelBtn->TabIndex = 2;
			this->CancelBtn->Text = S"Cancel";
			this->CancelBtn->UseVisualStyleBackColor = true;
			this->CancelBtn->Click += new System::EventHandler(this, &ServerConfiguration::CancelBtn_Click);
			// 
			// OKBtn
			// 
			this->OKBtn->Location = System::Drawing::Point(200, 11);
			this->OKBtn->Name = S"OKBtn";
			this->OKBtn->Size = System::Drawing::Size(75, 23);
			this->OKBtn->TabIndex = 1;
			this->OKBtn->Text = S"OK";
			this->OKBtn->UseVisualStyleBackColor = true;
			this->OKBtn->Click += new System::EventHandler(this, &ServerConfiguration::OKBtn_Click);
			// 
			// MiddlePanel
			// 
			this->MiddlePanel->BackColor = System::Drawing::SystemColors::Control;
			this->MiddlePanel->Controls->Add(this->ServerPropertyGrid);
			this->MiddlePanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->MiddlePanel->Location = System::Drawing::Point(0, 21);
			this->MiddlePanel->Name = S"MiddlePanel";
			this->MiddlePanel->Size = System::Drawing::Size(356, 247);
			this->MiddlePanel->TabIndex = 2;
			// 
			// ServerPropertyGrid
			// 
			this->ServerPropertyGrid->BackColor = System::Drawing::SystemColors::Control;
			this->ServerPropertyGrid->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ServerPropertyGrid->Location = System::Drawing::Point(0, 0);
			this->ServerPropertyGrid->Name = S"ServerPropertyGrid";
			this->ServerPropertyGrid->Size = System::Drawing::Size(356, 247);
			this->ServerPropertyGrid->TabIndex = 0;
			this->ServerPropertyGrid->PropertyValueChanged += new System::Windows::Forms::PropertyValueChangedEventHandler(this, &ServerConfiguration::ServerPropertyGrid_PropertyValueChanged);
			// 
			// ServerConfiguration
			// 
			this->ClientSize = System::Drawing::Size(356, 310);
			this->Controls->Add(this->MiddlePanel);
			this->Controls->Add(this->BottomPanel);
			this->Controls->Add(this->TopPanel);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->HelpButton = true;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = S"ServerConfiguration";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = S"Local Connection Settings";
			this->Load += new System::EventHandler(this, &ServerConfiguration::ServerConfiguration_Load);
			this->TopPanel->ResumeLayout(false);
			this->BottomPanel->ResumeLayout(false);
			this->MiddlePanel->ResumeLayout(false);
			this->ResumeLayout(false);

		}		


		//Form Events
		System::Void ServerConfiguration_Load(System::Object*  sender, System::EventArgs*  e);
		System::Void OKBtn_Click(System::Object*  sender, System::EventArgs*  e);
		System::Void CancelBtn_Click(System::Object*  sender, System::EventArgs*  e);			
		System::Void ProductComboBox_SelectedIndexChanged(System::Object*  sender, System::EventArgs*  e);
		System::Void ServerPropertyGrid_PropertyValueChanged(System::Object*  s, System::Windows::Forms::PropertyValueChangedEventArgs*  e);

		//Helper Methods
		bool LoadSettings(int ProductID);
		bool SaveSettings(int ProductID);
		int MapProductName(String* ProductName);
		bool CreateDefaultXML();
		void AddFileSecurity(String* fileName, String* account, 
                        FileSystemRights rights, AccessControlType controlType);
	
	private:
		void ConvertSettingsToXml_1_0();
		ConnectionSettings* TheConnectionSetting;
		bool m_ValueChanged;
		String* m_LastSelectedProduct;
};
}