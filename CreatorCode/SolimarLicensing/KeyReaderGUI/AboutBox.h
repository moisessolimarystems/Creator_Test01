#include "..\common\VersionInfo.h"

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
	/// Summary for AboutBox
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public __gc class AboutBox : public System::Windows::Forms::Form
	{
	public: 
		AboutBox(void)
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
		void InitializeDynamicData()
		{
			String* year = System::DateTime::Now.Year.ToString();
			int index = (labelCopyright->Text)->IndexOf("XXXX");
			labelCopyright->Text = labelCopyright->Text->Remove(index, 4)->Insert(index, year);
			versionLicensing->Text = String::Concat(MAJOR_REVISION_NUMBER.ToString(),S".",MINOR_REVISION_NUMBER.ToString(), S".", BUILD_NUMBER.ToString());
		}
		System::Void AboutBox_Click(System::Object *  sender, System::EventArgs *  e)
		{
			this->Close();
		}
		System::Void AboutBox_Load(System::Object *  sender, System::EventArgs *  e)
		{
			this->Refresh();
		}
		System::Void LinkLabel_Click(System::Object *  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs *  e)
		{
			System::Diagnostics::Process::Start((dynamic_cast<LinkLabel*>(sender))->Text);
		}

		System::Void EmailLink_Click(System::Object *  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs *  e)
		{
			System::Diagnostics::Process::Start(String::Concat("Mailto:", (dynamic_cast<LinkLabel*>(sender))->Text));
		}

	protected: System::Windows::Forms::LinkLabel *  linkLabel1;
	protected: System::Windows::Forms::LinkLabel *  linkWebsite;
	protected: System::Windows::Forms::Label *  labelCopyright;
	protected: System::Windows::Forms::Label *  labelRightsReserved;
	protected: System::Windows::Forms::Label *  labelPhoneNumbers;
	protected: System::Windows::Forms::Label *  labelAddress;
	protected: System::Windows::Forms::Label *  labelWebsite;
	protected: System::Windows::Forms::LinkLabel *  linkTechSupport;
	protected: System::Windows::Forms::LinkLabel *  linkSales;
	protected: System::Windows::Forms::Label *  labelFtp;
	protected: System::Windows::Forms::Label *  labelEmail;
	protected: System::Windows::Forms::Label *  labelLicensing;
	protected: System::Windows::Forms::Label *  versionLicensing;
	protected: System::Windows::Forms::Label *  label1;
	protected: System::Windows::Forms::Label *  label2;
	protected: System::Windows::Forms::Label *  label3;


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
			System::Resources::ResourceManager *  resources = new System::Resources::ResourceManager(__typeof(KeyReaderGUI::AboutBox));
			this->linkLabel1 = new System::Windows::Forms::LinkLabel();
			this->linkWebsite = new System::Windows::Forms::LinkLabel();
			this->labelCopyright = new System::Windows::Forms::Label();
			this->labelRightsReserved = new System::Windows::Forms::Label();
			this->labelPhoneNumbers = new System::Windows::Forms::Label();
			this->labelAddress = new System::Windows::Forms::Label();
			this->labelWebsite = new System::Windows::Forms::Label();
			this->linkTechSupport = new System::Windows::Forms::LinkLabel();
			this->linkSales = new System::Windows::Forms::LinkLabel();
			this->labelFtp = new System::Windows::Forms::Label();
			this->labelEmail = new System::Windows::Forms::Label();
			this->labelLicensing = new System::Windows::Forms::Label();
			this->versionLicensing = new System::Windows::Forms::Label();
			this->label1 = new System::Windows::Forms::Label();
			this->label2 = new System::Windows::Forms::Label();
			this->label3 = new System::Windows::Forms::Label();
			this->SuspendLayout();
			// 
			// linkLabel1
			// 
			this->linkLabel1->BackColor = System::Drawing::Color::Transparent;
			this->linkLabel1->Font = new System::Drawing::Font(S"Arial", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->linkLabel1->LinkColor = System::Drawing::Color::FromArgb((System::Byte)51, (System::Byte)102, (System::Byte)153);
			this->linkLabel1->Location = System::Drawing::Point(416, 376);
			this->linkLabel1->Name = S"linkLabel1";
			this->linkLabel1->Size = System::Drawing::Size(208, 16);
			this->linkLabel1->TabIndex = 22;
			this->linkLabel1->TabStop = true;
			this->linkLabel1->Text = S"ftp://downloads.solimarsystems.com";
			this->linkLabel1->LinkClicked += new System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, LinkLabel_Click);
			// 
			// linkWebsite
			// 
			this->linkWebsite->BackColor = System::Drawing::Color::Transparent;
			this->linkWebsite->Font = new System::Drawing::Font(S"Arial", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->linkWebsite->LinkColor = System::Drawing::Color::FromArgb((System::Byte)51, (System::Byte)102, (System::Byte)153);
			this->linkWebsite->Location = System::Drawing::Point(416, 360);
			this->linkWebsite->Name = S"linkWebsite";
			this->linkWebsite->Size = System::Drawing::Size(208, 16);
			this->linkWebsite->TabIndex = 19;
			this->linkWebsite->TabStop = true;
			this->linkWebsite->Text = S"http://www.solimarsystems.com";
			this->linkWebsite->LinkClicked += new System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, LinkLabel_Click);
			// 
			// labelCopyright
			// 
			this->labelCopyright->BackColor = System::Drawing::Color::Transparent;
			this->labelCopyright->Font = new System::Drawing::Font(S"Arial", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->labelCopyright->Location = System::Drawing::Point(208, 484);
			this->labelCopyright->Name = S"labelCopyright";
			this->labelCopyright->Size = System::Drawing::Size(384, 14);
			this->labelCopyright->TabIndex = 12;
			this->labelCopyright->Text = S"© 1997-XXXX. Solimar Systems, Inc. All rights reserved.";
			// 
			// labelRightsReserved
			// 
			this->labelRightsReserved->BackColor = System::Drawing::Color::Transparent;
			this->labelRightsReserved->Font = new System::Drawing::Font(S"Arial", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->labelRightsReserved->Location = System::Drawing::Point(208, 500);
			this->labelRightsReserved->Name = S"labelRightsReserved";
			this->labelRightsReserved->Size = System::Drawing::Size(384, 16);
			this->labelRightsReserved->TabIndex = 17;
			this->labelRightsReserved->Text = S"All trademarks are the property of their respective owners.";
			// 
			// labelPhoneNumbers
			// 
			this->labelPhoneNumbers->BackColor = System::Drawing::Color::Transparent;
			this->labelPhoneNumbers->Font = new System::Drawing::Font(S"Arial", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->labelPhoneNumbers->Location = System::Drawing::Point(208, 392);
			this->labelPhoneNumbers->Name = S"labelPhoneNumbers";
			this->labelPhoneNumbers->Size = System::Drawing::Size(148, 16);
			this->labelPhoneNumbers->TabIndex = 18;
			this->labelPhoneNumbers->Text = S"Tel:   +1.619.849.2800";
			// 
			// labelAddress
			// 
			this->labelAddress->BackColor = System::Drawing::Color::Transparent;
			this->labelAddress->Font = new System::Drawing::Font(S"Arial", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->labelAddress->Location = System::Drawing::Point(208, 344);
			this->labelAddress->Name = S"labelAddress";
			this->labelAddress->Size = System::Drawing::Size(148, 16);
			this->labelAddress->TabIndex = 15;
			this->labelAddress->Text = S"Solimar Systems, Inc.";
			// 
			// labelWebsite
			// 
			this->labelWebsite->BackColor = System::Drawing::Color::Transparent;
			this->labelWebsite->Font = new System::Drawing::Font(S"Arial", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->labelWebsite->Location = System::Drawing::Point(372, 360);
			this->labelWebsite->Name = S"labelWebsite";
			this->labelWebsite->Size = System::Drawing::Size(42, 16);
			this->labelWebsite->TabIndex = 16;
			this->labelWebsite->Text = S"Web:";
			// 
			// linkTechSupport
			// 
			this->linkTechSupport->BackColor = System::Drawing::Color::Transparent;
			this->linkTechSupport->Font = new System::Drawing::Font(S"Arial", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->linkTechSupport->LinkColor = System::Drawing::Color::FromArgb((System::Byte)51, (System::Byte)102, (System::Byte)153);
			this->linkTechSupport->Location = System::Drawing::Point(416, 392);
			this->linkTechSupport->Name = S"linkTechSupport";
			this->linkTechSupport->Size = System::Drawing::Size(208, 16);
			this->linkTechSupport->TabIndex = 20;
			this->linkTechSupport->TabStop = true;
			this->linkTechSupport->Text = S"tech.support@solimarsystems.com";
			this->linkTechSupport->LinkClicked += new System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, EmailLink_Click);
			// 
			// linkSales
			// 
			this->linkSales->BackColor = System::Drawing::Color::Transparent;
			this->linkSales->Font = new System::Drawing::Font(S"Arial", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->linkSales->LinkColor = System::Drawing::Color::FromArgb((System::Byte)51, (System::Byte)102, (System::Byte)153);
			this->linkSales->Location = System::Drawing::Point(416, 408);
			this->linkSales->Name = S"linkSales";
			this->linkSales->Size = System::Drawing::Size(208, 16);
			this->linkSales->TabIndex = 21;
			this->linkSales->TabStop = true;
			this->linkSales->Text = S"sales@solimarsystems.com";
			this->linkSales->LinkClicked += new System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, EmailLink_Click);
			// 
			// labelFtp
			// 
			this->labelFtp->BackColor = System::Drawing::Color::Transparent;
			this->labelFtp->Font = new System::Drawing::Font(S"Arial", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->labelFtp->Location = System::Drawing::Point(372, 376);
			this->labelFtp->Name = S"labelFtp";
			this->labelFtp->Size = System::Drawing::Size(42, 16);
			this->labelFtp->TabIndex = 3;
			this->labelFtp->Text = S"FTP:";
			// 
			// labelEmail
			// 
			this->labelEmail->BackColor = System::Drawing::Color::Transparent;
			this->labelEmail->Font = new System::Drawing::Font(S"Arial", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->labelEmail->Location = System::Drawing::Point(372, 392);
			this->labelEmail->Name = S"labelEmail";
			this->labelEmail->Size = System::Drawing::Size(42, 16);
			this->labelEmail->TabIndex = 4;
			this->labelEmail->Text = S"EMail:";
			// 
			// labelLicensing
			// 
			this->labelLicensing->BackColor = System::Drawing::Color::Transparent;
			this->labelLicensing->Font = new System::Drawing::Font(S"Arial", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->labelLicensing->Location = System::Drawing::Point(208, 436);
			this->labelLicensing->Name = S"labelLicensing";
			this->labelLicensing->Size = System::Drawing::Size(108, 16);
			this->labelLicensing->TabIndex = 7;
			this->labelLicensing->Text = S"Licensing:";
			// 
			// versionLicensing
			// 
			this->versionLicensing->BackColor = System::Drawing::Color::Transparent;
			this->versionLicensing->Font = new System::Drawing::Font(S"Arial", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->versionLicensing->Location = System::Drawing::Point(208, 452);
			this->versionLicensing->Name = S"versionLicensing";
			this->versionLicensing->Size = System::Drawing::Size(108, 16);
			this->versionLicensing->TabIndex = 8;
			this->versionLicensing->Text = S"XX.XX.XXXXXX";
			// 
			// label1
			// 
			this->label1->BackColor = System::Drawing::Color::Transparent;
			this->label1->Font = new System::Drawing::Font(S"Arial", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->label1->Location = System::Drawing::Point(208, 360);
			this->label1->Name = S"label1";
			this->label1->Size = System::Drawing::Size(148, 16);
			this->label1->TabIndex = 5;
			this->label1->Text = S"1515 Second Ave.";
			// 
			// label2
			// 
			this->label2->BackColor = System::Drawing::Color::Transparent;
			this->label2->Font = new System::Drawing::Font(S"Arial", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->label2->Location = System::Drawing::Point(208, 376);
			this->label2->Name = S"label2";
			this->label2->Size = System::Drawing::Size(148, 16);
			this->label2->TabIndex = 11;
			this->label2->Text = S"San Diego, CA 92101";
			// 
			// label3
			// 
			this->label3->BackColor = System::Drawing::Color::Transparent;
			this->label3->Font = new System::Drawing::Font(S"Arial", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->label3->Location = System::Drawing::Point(208, 408);
			this->label3->Name = S"label3";
			this->label3->Size = System::Drawing::Size(148, 16);
			this->label3->TabIndex = 14;
			this->label3->Text = S"Fax:  +1.619.849.2801";
			// 
			// AboutBox
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->BackgroundImage = (__try_cast<System::Drawing::Image *  >(resources->GetObject(S"$this.BackgroundImage")));
			this->ClientSize = System::Drawing::Size(736, 560);
			this->Controls->Add(this->linkLabel1);
			this->Controls->Add(this->linkWebsite);
			this->Controls->Add(this->labelCopyright);
			this->Controls->Add(this->labelRightsReserved);
			this->Controls->Add(this->labelPhoneNumbers);
			this->Controls->Add(this->labelAddress);
			this->Controls->Add(this->labelWebsite);
			this->Controls->Add(this->linkTechSupport);
			this->Controls->Add(this->linkSales);
			this->Controls->Add(this->labelFtp);
			this->Controls->Add(this->labelEmail);
			this->Controls->Add(this->labelLicensing);
			this->Controls->Add(this->versionLicensing);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label3);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Name = S"AboutBox";
			this->Text = S"AboutBox";
			this->Click += new System::EventHandler(this, AboutBox_Click);
			this->Load += new System::EventHandler(this, AboutBox_Load);
			this->ResumeLayout(false);

		}		









};
}