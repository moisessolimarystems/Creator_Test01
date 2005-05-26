#include "stdafx.h"
#include "ContactInfo.h"

using namespace ContactInfoForm;

//Constructor. 
//Initializes the member variables and creates a table of appropriate size.
ContactInfo::ContactInfo()
{
	//Initialize the resources
	InitializeComponent();
}

//Initializes the resources
void ContactInfo::InitializeComponent()
{
	this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
	this->ClientSize = System::Drawing::Size(500, 250);
	this->Name = S"ContactInfo";
	this->Text = S"Contact Information";
	
	RichTextBox* richTextBox1 = new RichTextBox();
	richTextBox1->BackColor = System::Drawing::SystemColors::InactiveBorder;
    richTextBox1->Dock = DockStyle::Fill;
	richTextBox1->ReadOnly = true;
	    
	richTextBox1->SelectionFont = new System::Drawing::Font(S"Arial", 18, FontStyle::Bold);
	richTextBox1->SelectedText = S"\n   To Contact Sales Or Technical Support";

	richTextBox1->SelectionFont = new System::Drawing::Font(S"Arial", 12);
	
	richTextBox1->AppendText(S"\n");
	richTextBox1->AppendText(S"\n \t \t  Telephone: \t(619) 849-2800");
	richTextBox1->AppendText(S"\n \t \t  Fax: \t \t(619) 849-2801");
	richTextBox1->AppendText(S"\n \t \t  Web: \t \thttp://www.solimarsystems.com");
	richTextBox1->AppendText(S"\n \t \t  Email: \t \ttech.support@solimarsystems.com");
	richTextBox1->AppendText(S"\n \t \t  \t \tsales@solimarsystems.com");
	richTextBox1->AppendText(S"\n \t \t  FTP: \t \tftp://ftp.solimarsystems.com");
	richTextBox1->AppendText(S"\n \t \t  \t \tor link through Solimar website");

	this->Controls->Add(richTextBox1);

}
ContactInfo::~ContactInfo()
{
}

//Releases all of the resources that this object owns
void ContactInfo::Dispose(bool disposing)
{
	if (disposing && components)
	{
		components->Dispose();
	}
	
	__super::Dispose(disposing);
}