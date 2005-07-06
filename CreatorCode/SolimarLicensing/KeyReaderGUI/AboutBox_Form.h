#include "VersionInfo.h"

#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace KeyReaderGUI
{
	/// <summary> 
	/// Summary for AboutBox_Form
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public __gc class AboutBox_Form : public System::Windows::Forms::Form
	{
	public: 
		AboutBox_Form(void)
		{
#pragma push_macro("GetObject") 
#undef GetObject 
			InitializeComponent();
#pragma pop_macro("GetObject") 
			InitializeDynamicData();
		}
        
	protected: 
		void Dispose(Boolean disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}
	private: System::Windows::Forms::Panel *  AboutInfoPanel;
	private: System::Windows::Forms::Label *  labelLicensing;
	private: System::Windows::Forms::Label *  versionLicensing;
	private: System::Windows::Forms::Label *  labelCopyright;
	private: System::Windows::Forms::Label *  labelRightsReserved;
	private: System::Windows::Forms::PictureBox *  LogoPictureBox;
	private: System::Windows::Forms::Button *  AboutOKBtn;


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
			System::Resources::ResourceManager *  resources = new System::Resources::ResourceManager(__typeof(KeyReaderGUI::AboutBox_Form));
			this->AboutInfoPanel = new System::Windows::Forms::Panel();
			this->AboutOKBtn = new System::Windows::Forms::Button();
			this->LogoPictureBox = new System::Windows::Forms::PictureBox();
			this->labelCopyright = new System::Windows::Forms::Label();
			this->labelRightsReserved = new System::Windows::Forms::Label();
			this->labelLicensing = new System::Windows::Forms::Label();
			this->versionLicensing = new System::Windows::Forms::Label();
			this->AboutInfoPanel->SuspendLayout();
			this->SuspendLayout();
			// 
			// AboutInfoPanel
			// 
			this->AboutInfoPanel->Controls->Add(this->AboutOKBtn);
			this->AboutInfoPanel->Controls->Add(this->LogoPictureBox);
			this->AboutInfoPanel->Controls->Add(this->labelCopyright);
			this->AboutInfoPanel->Controls->Add(this->labelRightsReserved);
			this->AboutInfoPanel->Controls->Add(this->labelLicensing);
			this->AboutInfoPanel->Controls->Add(this->versionLicensing);
			this->AboutInfoPanel->Location = System::Drawing::Point(0, 0);
			this->AboutInfoPanel->Name = S"AboutInfoPanel";
			this->AboutInfoPanel->Size = System::Drawing::Size(450, 310);
			this->AboutInfoPanel->TabIndex = 0;
			// 
			// AboutOKBtn
			// 
			this->AboutOKBtn->Location = System::Drawing::Point(352, 248);
			this->AboutOKBtn->Name = S"AboutOKBtn";
			this->AboutOKBtn->TabIndex = 17;
			this->AboutOKBtn->Text = S"OK";
			this->AboutOKBtn->Click += new System::EventHandler(this, AboutOKBtn_Click);
			// 
			// LogoPictureBox
			// 
			this->LogoPictureBox->BackgroundImage = (__try_cast<System::Drawing::Image *  >(resources->GetObject(S"LogoPictureBox.BackgroundImage")));
			this->LogoPictureBox->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->LogoPictureBox->Location = System::Drawing::Point(8, 8);
			this->LogoPictureBox->Name = S"LogoPictureBox";
			this->LogoPictureBox->Size = System::Drawing::Size(234, 259);
			this->LogoPictureBox->TabIndex = 16;
			this->LogoPictureBox->TabStop = false;
			// 
			// labelCopyright
			// 
			this->labelCopyright->BackColor = System::Drawing::Color::Transparent;
			this->labelCopyright->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->labelCopyright->Location = System::Drawing::Point(248, 40);
			this->labelCopyright->Name = S"labelCopyright";
			this->labelCopyright->Size = System::Drawing::Size(208, 32);
			this->labelCopyright->TabIndex = 15;
			this->labelCopyright->Text = S"Copyright 1997-XXXX Solimar Systems Incorporated.";
			// 
			// labelRightsReserved
			// 
			this->labelRightsReserved->BackColor = System::Drawing::Color::Transparent;
			this->labelRightsReserved->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->labelRightsReserved->Location = System::Drawing::Point(248, 80);
			this->labelRightsReserved->Name = S"labelRightsReserved";
			this->labelRightsReserved->Size = System::Drawing::Size(200, 40);
			this->labelRightsReserved->TabIndex = 14;
			this->labelRightsReserved->Text = S"All rights reserved.  All trademarks are property of their respective owners.";
			// 
			// labelLicensing
			// 
			this->labelLicensing->BackColor = System::Drawing::Color::Transparent;
			this->labelLicensing->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->labelLicensing->Location = System::Drawing::Point(248, 16);
			this->labelLicensing->Name = S"labelLicensing";
			this->labelLicensing->Size = System::Drawing::Size(56, 16);
			this->labelLicensing->TabIndex = 13;
			this->labelLicensing->Text = S"Version :";
			// 
			// versionLicensing
			// 
			this->versionLicensing->BackColor = System::Drawing::Color::Transparent;
			this->versionLicensing->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->versionLicensing->Location = System::Drawing::Point(352, 16);
			this->versionLicensing->Name = S"versionLicensing";
			this->versionLicensing->Size = System::Drawing::Size(88, 16);
			this->versionLicensing->TabIndex = 12;
			this->versionLicensing->Text = S"XX.XX.XXXXXX";
			// 
			// AboutBox_Form
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->ClientSize = System::Drawing::Size(444, 284);
			this->Controls->Add(this->AboutInfoPanel);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (__try_cast<System::Drawing::Icon *  >(resources->GetObject(S"$this.Icon")));
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = S"AboutBox_Form";
			this->Text = S"About License Manager";
			this->AboutInfoPanel->ResumeLayout(false);
			this->ResumeLayout(false);

		}		

		void InitializeDynamicData()
		{
			String* year = System::DateTime::Now.Year.ToString();
			int index = (labelCopyright->Text)->IndexOf("XXXX");
			labelCopyright->Text = labelCopyright->Text->Remove(index, 4)->Insert(index, year);
			versionLicensing->Text = String::Concat(MAJOR_REVISION_NUMBER.ToString(),S".",MINOR_REVISION_NUMBER.ToString(), S".", BUILD_NUMBER.ToString());
		}
	    System::Void AboutOKBtn_Click(System::Object *  sender, System::EventArgs *  e)
	    {
			this->Close();
	    }

};
}