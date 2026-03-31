#pragma once

namespace KeyReaderGUI
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	/// <summary> 
	/// Summary for ConnectToForm
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public __gc class ConnectToForm : public System::Windows::Forms::Form
	{
	public: 
		//list of past valid connections passed in
		ConnectToForm(ArrayList* _ServerList)
		{
			InitializeComponent();
			for(int i=0; i<_ServerList->Count; i++)
			{
				CB_ServerList->Items->Insert(i,_ServerList->Item[i]);
			}	
			if(_ServerList->Count > 0)
				CB_ServerList->SelectedIndex = 0;
		}
		~ConnectToForm(){this->Dispose(true);}
        
	protected: 
		void Dispose(Boolean disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}
	private: System::Windows::Forms::Panel *  TopPanel;
	private: System::Windows::Forms::Panel *  BottomPanel;
	private: System::Windows::Forms::Button *  BTN_OK;
	private: System::Windows::Forms::Button *  BTN_Cancel;
	private: System::Windows::Forms::ComboBox *  CB_ServerList;
	private: System::Windows::Forms::Label *  Lbl_ServerName;
	private: System::Windows::Forms::GroupBox*  groupBox1;
	private: System::Windows::Forms::Label*  label1;
	private: System::Windows::Forms::ErrorProvider*  ConnectErrorProvider;

	private: System::ComponentModel::IContainer*  components;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (new System::ComponentModel::Container());
			this->TopPanel = (new System::Windows::Forms::Panel());
			this->groupBox1 = (new System::Windows::Forms::GroupBox());
			this->label1 = (new System::Windows::Forms::Label());
			this->BottomPanel = (new System::Windows::Forms::Panel());
			this->BTN_Cancel = (new System::Windows::Forms::Button());
			this->BTN_OK = (new System::Windows::Forms::Button());
			this->Lbl_ServerName = (new System::Windows::Forms::Label());
			this->CB_ServerList = (new System::Windows::Forms::ComboBox());
			this->ConnectErrorProvider = (new System::Windows::Forms::ErrorProvider(this->components));
			this->TopPanel->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->BottomPanel->SuspendLayout();
			(__try_cast<System::ComponentModel::ISupportInitialize*  >(this->ConnectErrorProvider))->BeginInit();
			this->SuspendLayout();
			// 
			// TopPanel
			// 
			this->TopPanel->Controls->Add(this->groupBox1);
			this->TopPanel->Controls->Add(this->BottomPanel);
			this->TopPanel->Controls->Add(this->Lbl_ServerName);
			this->TopPanel->Controls->Add(this->CB_ServerList);
			this->TopPanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->TopPanel->Location = System::Drawing::Point(0, 0);
			this->TopPanel->Name = S"TopPanel";
			this->TopPanel->Size = System::Drawing::Size(288, 94);
			this->TopPanel->TabIndex = 0;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Location = System::Drawing::Point(11, 54);
			this->groupBox1->Name = S"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(261, 8);
			this->groupBox1->TabIndex = 5;
			this->groupBox1->TabStop = false;
			// 
			// label1
			// 
			this->label1->Location = System::Drawing::Point(3, 16);
			this->label1->Name = S"label1";
			this->label1->Size = System::Drawing::Size(100, 23);
			this->label1->TabIndex = 0;
			this->label1->Text = S"label1";
			// 
			// BottomPanel
			// 
			this->BottomPanel->AutoSize = true;
			this->BottomPanel->Controls->Add(this->BTN_Cancel);
			this->BottomPanel->Controls->Add(this->BTN_OK);
			this->BottomPanel->Location = System::Drawing::Point(0, 59);
			this->BottomPanel->Name = S"BottomPanel";
			this->BottomPanel->Size = System::Drawing::Size(288, 34);
			this->BottomPanel->TabIndex = 1;
			// 
			// BTN_Cancel
			// 
			this->BTN_Cancel->Location = System::Drawing::Point(197, 8);
			this->BTN_Cancel->Name = S"BTN_Cancel";
			this->BTN_Cancel->Size = System::Drawing::Size(75, 23);
			this->BTN_Cancel->TabIndex = 1;
			this->BTN_Cancel->Text = S"Cancel";
			this->BTN_Cancel->Click += new System::EventHandler(this, &ConnectToForm::BTN_Cancel_Click);
			// 
			// BTN_OK
			// 
			this->BTN_OK->Location = System::Drawing::Point(112, 8);
			this->BTN_OK->Name = S"BTN_OK";
			this->BTN_OK->Size = System::Drawing::Size(75, 23);
			this->BTN_OK->TabIndex = 0;
			this->BTN_OK->Text = S"Ok";
			this->BTN_OK->Click += new System::EventHandler(this, &ConnectToForm::BTN_OK_Click);
			// 
			// Lbl_ServerName
			// 
			this->Lbl_ServerName->Location = System::Drawing::Point(8, 20);
			this->Lbl_ServerName->Name = S"Lbl_ServerName";
			this->Lbl_ServerName->Size = System::Drawing::Size(72, 24);
			this->Lbl_ServerName->TabIndex = 1;
			this->Lbl_ServerName->Text = S"Server Name";
			this->Lbl_ServerName->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// CB_ServerList
			// 
			this->CB_ServerList->Location = System::Drawing::Point(88, 24);
			this->CB_ServerList->Name = S"CB_ServerList";
			this->CB_ServerList->Size = System::Drawing::Size(184, 21);
			this->CB_ServerList->TabIndex = 0;
			this->CB_ServerList->Text = S"localhost";
			this->CB_ServerList->Validating += new System::ComponentModel::CancelEventHandler(this, &ConnectToForm::CB_ServerList_Validating);
			this->CB_ServerList->Validated += new System::EventHandler(this, &ConnectToForm::CB_ServerList_Validated);
			this->CB_ServerList->KeyDown += new System::Windows::Forms::KeyEventHandler(this, &ConnectToForm::CB_ServerList_KeyDown);
			// 
			// ConnectErrorProvider
			// 
			this->ConnectErrorProvider->ContainerControl = this;
			// 
			// ConnectToForm
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->ClientSize = System::Drawing::Size(288, 94);
			this->Controls->Add(this->TopPanel);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = S"ConnectToForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = S"Server Connection";
			this->Closing += new System::ComponentModel::CancelEventHandler(this, &ConnectToForm::ConnectToForm_Closing);
			this->Load += new System::EventHandler(this, &ConnectToForm::ConnectToForm_Load);
			this->TopPanel->ResumeLayout(false);
			this->TopPanel->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->BottomPanel->ResumeLayout(false);
			(__try_cast<System::ComponentModel::ISupportInitialize*  >(this->ConnectErrorProvider))->EndInit();
			this->ResumeLayout(false);

		}		
		//Button Events
		System::Void BTN_OK_Click(System::Object *  sender, System::EventArgs *  e);
		System::Void BTN_Cancel_Click(System::Object *  sender, System::EventArgs *  e);
		//Form Events
		System::Void ConnectToForm_Load(System::Object *  sender, System::EventArgs *  e);
		System::Void ConnectToForm_Closing(System::Object *  sender, System::ComponentModel::CancelEventArgs *  e);
		System::Void CB_ServerList_KeyDown(System::Object*  sender, System::Windows::Forms::KeyEventArgs*  e);
		System::Void CB_ServerList_Validated(System::Object*  sender, System::EventArgs*  e);
		System::Void CB_ServerList_Validating(System::Object*  sender, System::ComponentModel::CancelEventArgs*  e); 
		//
	private :
		bool IsValidServer(String*);

	public :
		String* GetServerName(){return this->CB_ServerList->Text;}
};
}