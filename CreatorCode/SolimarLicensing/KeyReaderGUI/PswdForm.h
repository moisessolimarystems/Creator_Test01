#pragma once

#include "PasswordValidation.h"

namespace KeyReaderGUI
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace PWDValidation;

	/// <summary> 
	/// Summary for PswdForm
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	__gc class PswdForm : public System::Windows::Forms::Form
	{
	public: 
		PswdForm(PasswordValidation* ThePasswordValidater);
		~PswdForm();
        
	protected: 
		void Dispose(Boolean disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}
	private: System::ComponentModel::IContainer*  components;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

		PasswordValidation* PasswordValidater;

		System::Windows::Forms::Label *  PswdLabel;
		System::Windows::Forms::Button *  OkBtn;
		System::Windows::Forms::Button *  CancelBtn;
		System::Windows::Forms::GroupBox *  groupBox1;
		System::Windows::Forms::Label *  label1;
		System::Windows::Forms::Label *  PswdFormLabel;
		System::Windows::Forms::ErrorProvider *  PswdErrorProvider;
		System::Windows::Forms::RichTextBox *  PswdTextBox;

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (new System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager*  resources = (new System::ComponentModel::ComponentResourceManager(__typeof(PswdForm)));
			this->PswdLabel = (new System::Windows::Forms::Label());
			this->OkBtn = (new System::Windows::Forms::Button());
			this->CancelBtn = (new System::Windows::Forms::Button());
			this->PswdTextBox = (new System::Windows::Forms::RichTextBox());
			this->groupBox1 = (new System::Windows::Forms::GroupBox());
			this->label1 = (new System::Windows::Forms::Label());
			this->PswdFormLabel = (new System::Windows::Forms::Label());
			this->PswdErrorProvider = (new System::Windows::Forms::ErrorProvider(this->components));
			this->groupBox1->SuspendLayout();
			(__try_cast<System::ComponentModel::ISupportInitialize*  >(this->PswdErrorProvider))->BeginInit();
			this->SuspendLayout();
			// 
			// PswdLabel
			// 
			this->PswdLabel->Font = (new System::Drawing::Font(S"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				(System::Byte)0));
			this->PswdLabel->Location = System::Drawing::Point(16, 96);
			this->PswdLabel->Name = S"PswdLabel";
			this->PswdLabel->Size = System::Drawing::Size(64, 24);
			this->PswdLabel->TabIndex = 6;
			this->PswdLabel->Text = S"Password:";
			// 
			// OkBtn
			// 
			this->OkBtn->Location = System::Drawing::Point(176, 152);
			this->OkBtn->Name = S"OkBtn";
			this->OkBtn->Size = System::Drawing::Size(75, 23);
			this->OkBtn->TabIndex = 1;
			this->OkBtn->Text = S"OK";
			this->OkBtn->Click += new System::EventHandler(this, &PswdForm::OkBtn_Click);
			// 
			// CancelBtn
			// 
			this->CancelBtn->CausesValidation = false;
			this->CancelBtn->Location = System::Drawing::Point(253, 152);
			this->CancelBtn->Name = S"CancelBtn";
			this->CancelBtn->Size = System::Drawing::Size(75, 23);
			this->CancelBtn->TabIndex = 2;
			this->CancelBtn->Text = S"Cancel";
			this->CancelBtn->Click += new System::EventHandler(this, &PswdForm::CancelBtn_Click);
			// 
			// PswdTextBox
			// 
			this->PswdTextBox->Location = System::Drawing::Point(80, 93);
			this->PswdTextBox->MaxLength = 50;
			this->PswdTextBox->Multiline = false;
			this->PswdTextBox->Name = S"PswdTextBox";
			this->PswdTextBox->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::None;
			this->PswdTextBox->Size = System::Drawing::Size(248, 20);
			this->PswdTextBox->TabIndex = 0;
			this->PswdTextBox->Text = S"";
			this->PswdTextBox->Validating += new System::ComponentModel::CancelEventHandler(this, &PswdForm::PswdTextBox_Validating);
			this->PswdTextBox->Validated += new System::EventHandler(this, &PswdForm::PswdTextBox_Validated);
			this->PswdTextBox->KeyDown += new System::Windows::Forms::KeyEventHandler(this, &PswdForm::PswdTextBox_KeyDown);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Location = System::Drawing::Point(16, 136);
			this->groupBox1->Name = S"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(312, 8);
			this->groupBox1->TabIndex = 4;
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
			// PswdFormLabel
			// 
			this->PswdFormLabel->Location = System::Drawing::Point(24, 16);
			this->PswdFormLabel->Name = S"PswdFormLabel";
			this->PswdFormLabel->Size = System::Drawing::Size(304, 64);
			this->PswdFormLabel->TabIndex = 5;
			this->PswdFormLabel->Text = resources->GetString(S"PswdFormLabel.Text");
			// 
			// PswdErrorProvider
			// 
			this->PswdErrorProvider->ContainerControl = this;
			// 
			// PswdForm
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->ClientSize = System::Drawing::Size(346, 182);
			this->Controls->Add(this->PswdFormLabel);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->PswdTextBox);
			this->Controls->Add(this->CancelBtn);
			this->Controls->Add(this->OkBtn);
			this->Controls->Add(this->PswdLabel);
			this->Font = (new System::Drawing::Font(S"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				(System::Byte)0));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = S"PswdForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = S"Please Enter a Password ";
			this->groupBox1->ResumeLayout(false);
			(__try_cast<System::ComponentModel::ISupportInitialize*  >(this->PswdErrorProvider))->EndInit();
			this->ResumeLayout(false);

		}		

		System::Void OkBtn_Click(System::Object *  sender, System::EventArgs *  e);			 			 
		System::Void CancelBtn_Click(System::Object *  sender, System::EventArgs *  e);						 
		System::Void PswdTextBox_Validated(System::Object *  sender, System::EventArgs *  e);
		System::Void PswdTextBox_Validating(System::Object *  sender, System::ComponentModel::CancelEventArgs *  e);
		bool IsValidPassword(String* number, String** errorMessage); 
		System::Void PswdTextBox_KeyDown(System::Object*  sender, System::Windows::Forms::KeyEventArgs*  e);
		
};
}