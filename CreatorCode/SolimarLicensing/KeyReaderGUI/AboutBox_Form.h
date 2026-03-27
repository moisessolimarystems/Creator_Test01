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
	private: System::Windows::Forms::Label *  labelAddress;
	private: System::Windows::Forms::LinkLabel *  linkFtp;
	private: System::Windows::Forms::LinkLabel *  linkWebsite;
	private: System::Windows::Forms::Label *  labelPhoneNumbers;
	private: System::Windows::Forms::Label *  labelWebsite;
	private: System::Windows::Forms::LinkLabel *  linkTechSupport;
	private: System::Windows::Forms::LinkLabel *  linkSales;
	private: System::Windows::Forms::Label *  labelFtp;
	private: System::Windows::Forms::Label *  labelOr;
	private: System::Windows::Forms::Label *  labelEmail;
	private: System::Windows::Forms::Label *  labelLicensing;
	private: System::Windows::Forms::Label *  versionLicensing;
	private: System::Windows::Forms::Label *  labelCopyright;
	private: System::Windows::Forms::Label *  labelRightsReserved;

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
			this->labelCopyright = new System::Windows::Forms::Label();
			this->labelRightsReserved = new System::Windows::Forms::Label();
			this->labelLicensing = new System::Windows::Forms::Label();
			this->versionLicensing = new System::Windows::Forms::Label();
			this->linkFtp = new System::Windows::Forms::LinkLabel();
			this->linkWebsite = new System::Windows::Forms::LinkLabel();
			this->labelPhoneNumbers = new System::Windows::Forms::Label();
			this->labelWebsite = new System::Windows::Forms::Label();
			this->linkTechSupport = new System::Windows::Forms::LinkLabel();
			this->linkSales = new System::Windows::Forms::LinkLabel();
			this->labelFtp = new System::Windows::Forms::Label();
			this->labelOr = new System::Windows::Forms::Label();
			this->labelEmail = new System::Windows::Forms::Label();
			this->labelAddress = new System::Windows::Forms::Label();
			this->AboutInfoPanel->SuspendLayout();
			this->SuspendLayout();
			// 
			// AboutInfoPanel
			// 
			this->AboutInfoPanel->Controls->Add(this->labelCopyright);
			this->AboutInfoPanel->Controls->Add(this->labelRightsReserved);
			this->AboutInfoPanel->Controls->Add(this->labelLicensing);
			this->AboutInfoPanel->Controls->Add(this->versionLicensing);
			this->AboutInfoPanel->Controls->Add(this->linkFtp);
			this->AboutInfoPanel->Controls->Add(this->linkWebsite);
			this->AboutInfoPanel->Controls->Add(this->labelPhoneNumbers);
			this->AboutInfoPanel->Controls->Add(this->labelWebsite);
			this->AboutInfoPanel->Controls->Add(this->linkTechSupport);
			this->AboutInfoPanel->Controls->Add(this->linkSales);
			this->AboutInfoPanel->Controls->Add(this->labelFtp);
			this->AboutInfoPanel->Controls->Add(this->labelOr);
			this->AboutInfoPanel->Controls->Add(this->labelEmail);
			this->AboutInfoPanel->Controls->Add(this->labelAddress);
			this->AboutInfoPanel->Location = System::Drawing::Point(0, 0);
			this->AboutInfoPanel->Name = S"AboutInfoPanel";
			this->AboutInfoPanel->Size = System::Drawing::Size(432, 272);
			this->AboutInfoPanel->TabIndex = 0;
			// 
			// labelCopyright
			// 
			this->labelCopyright->BackColor = System::Drawing::Color::Transparent;
			this->labelCopyright->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->labelCopyright->Location = System::Drawing::Point(8, 232);
			this->labelCopyright->Name = S"labelCopyright";
			this->labelCopyright->Size = System::Drawing::Size(424, 16);
			this->labelCopyright->TabIndex = 15;
			this->labelCopyright->Text = S"Copyright 1997-XXXX Solimar Systems Incorporated.";
			// 
			// labelRightsReserved
			// 
			this->labelRightsReserved->BackColor = System::Drawing::Color::Transparent;
			this->labelRightsReserved->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->labelRightsReserved->Location = System::Drawing::Point(8, 248);
			this->labelRightsReserved->Name = S"labelRightsReserved";
			this->labelRightsReserved->Size = System::Drawing::Size(424, 20);
			this->labelRightsReserved->TabIndex = 14;
			this->labelRightsReserved->Text = S"All rights reserved.  All trademarks are property of their respective owners.";
			// 
			// labelLicensing
			// 
			this->labelLicensing->BackColor = System::Drawing::Color::Transparent;
			this->labelLicensing->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->labelLicensing->Location = System::Drawing::Point(8, 208);
			this->labelLicensing->Name = S"labelLicensing";
			this->labelLicensing->Size = System::Drawing::Size(60, 18);
			this->labelLicensing->TabIndex = 13;
			this->labelLicensing->Text = S"Licensing:";
			// 
			// versionLicensing
			// 
			this->versionLicensing->BackColor = System::Drawing::Color::Transparent;
			this->versionLicensing->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->versionLicensing->Location = System::Drawing::Point(72, 208);
			this->versionLicensing->Name = S"versionLicensing";
			this->versionLicensing->Size = System::Drawing::Size(88, 18);
			this->versionLicensing->TabIndex = 12;
			this->versionLicensing->Text = S"XX.XX.XXXXXX";
			// 
			// linkFtp
			// 
			this->linkFtp->BackColor = System::Drawing::Color::Transparent;
			this->linkFtp->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->linkFtp->Location = System::Drawing::Point(280, 136);
			this->linkFtp->Name = S"linkFtp";
			this->linkFtp->Size = System::Drawing::Size(152, 20);
			this->linkFtp->TabIndex = 11;
			this->linkFtp->TabStop = true;
			this->linkFtp->Text = S"ftp://ftp.solimarsystems.com";
			this->linkFtp->LinkClicked += new System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, LinkLabel_Click);
			// 
			// linkWebsite
			// 
			this->linkWebsite->BackColor = System::Drawing::Color::Transparent;
			this->linkWebsite->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->linkWebsite->Location = System::Drawing::Point(56, 136);
			this->linkWebsite->Name = S"linkWebsite";
			this->linkWebsite->Size = System::Drawing::Size(188, 20);
			this->linkWebsite->TabIndex = 9;
			this->linkWebsite->TabStop = true;
			this->linkWebsite->Text = S"http://www.solimarsystems.com";
			this->linkWebsite->LinkClicked += new System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, LinkLabel_Click);
			// 
			// labelPhoneNumbers
			// 
			this->labelPhoneNumbers->BackColor = System::Drawing::Color::Transparent;
			this->labelPhoneNumbers->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->labelPhoneNumbers->Location = System::Drawing::Point(8, 184);
			this->labelPhoneNumbers->Name = S"labelPhoneNumbers";
			this->labelPhoneNumbers->Size = System::Drawing::Size(424, 20);
			this->labelPhoneNumbers->TabIndex = 5;
			this->labelPhoneNumbers->Text = S"Telephone: (619) 849-2800                                   Fax:     (619) 849-28" 
				S"01";
			// 
			// labelWebsite
			// 
			this->labelWebsite->BackColor = System::Drawing::Color::Transparent;
			this->labelWebsite->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->labelWebsite->Location = System::Drawing::Point(8, 136);
			this->labelWebsite->Name = S"labelWebsite";
			this->labelWebsite->Size = System::Drawing::Size(56, 20);
			this->labelWebsite->TabIndex = 3;
			this->labelWebsite->Text = S"Website:";
			// 
			// linkTechSupport
			// 
			this->linkTechSupport->BackColor = System::Drawing::Color::Transparent;
			this->linkTechSupport->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->linkTechSupport->Location = System::Drawing::Point(56, 160);
			this->linkTechSupport->Name = S"linkTechSupport";
			this->linkTechSupport->Size = System::Drawing::Size(196, 20);
			this->linkTechSupport->TabIndex = 10;
			this->linkTechSupport->TabStop = true;
			this->linkTechSupport->Text = S"tech.support@solimarsystems.com";
			this->linkTechSupport->LinkClicked += new System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, EmailLink_Click);
			// 
			// linkSales
			// 
			this->linkSales->BackColor = System::Drawing::Color::Transparent;
			this->linkSales->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->linkSales->Location = System::Drawing::Point(280, 160);
			this->linkSales->Name = S"linkSales";
			this->linkSales->Size = System::Drawing::Size(152, 20);
			this->linkSales->TabIndex = 8;
			this->linkSales->TabStop = true;
			this->linkSales->Text = S"sales@solimarsystems.com";
			this->linkSales->LinkClicked += new System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, EmailLink_Click);
			// 
			// labelFtp
			// 
			this->labelFtp->BackColor = System::Drawing::Color::Transparent;
			this->labelFtp->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->labelFtp->Location = System::Drawing::Point(248, 136);
			this->labelFtp->Name = S"labelFtp";
			this->labelFtp->Size = System::Drawing::Size(36, 20);
			this->labelFtp->TabIndex = 4;
			this->labelFtp->Text = S"FTP:";
			// 
			// labelOr
			// 
			this->labelOr->BackColor = System::Drawing::Color::Transparent;
			this->labelOr->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->labelOr->Location = System::Drawing::Point(256, 160);
			this->labelOr->Name = S"labelOr";
			this->labelOr->Size = System::Drawing::Size(28, 20);
			this->labelOr->TabIndex = 7;
			this->labelOr->Text = S"or";
			// 
			// labelEmail
			// 
			this->labelEmail->BackColor = System::Drawing::Color::Transparent;
			this->labelEmail->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->labelEmail->Location = System::Drawing::Point(8, 160);
			this->labelEmail->Name = S"labelEmail";
			this->labelEmail->Size = System::Drawing::Size(56, 20);
			this->labelEmail->TabIndex = 6;
			this->labelEmail->Text = S"E-Mail:";
			// 
			// labelAddress
			// 
			this->labelAddress->BackColor = System::Drawing::Color::Transparent;
			this->labelAddress->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->labelAddress->Location = System::Drawing::Point(8, 112);
			this->labelAddress->Name = S"labelAddress";
			this->labelAddress->Size = System::Drawing::Size(336, 20);
			this->labelAddress->TabIndex = 1;
			this->labelAddress->Text = S"Solimar Systems Incorporated.  1515 Second Ave. San Diego, CA";
			// 
			// AboutBox_Form
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->ClientSize = System::Drawing::Size(432, 268);
			this->Controls->Add(this->AboutInfoPanel);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Icon = (__try_cast<System::Drawing::Icon *  >(resources->GetObject(S"$this.Icon")));
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
		void LinkLabel_Click(Object* sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs* e)
		{
			System::Diagnostics::Process::Start(__try_cast<LinkLabel*>(sender)->Text);			
		}

		void EmailLink_Click(Object* sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs* e)
		{
			System::Diagnostics::Process::Start(String::Concat(S"Mailto:", __try_cast<LinkLabel*>(sender)->Text));	
		}
	};
}