#include "stdafx.h"
#include "AboutBoxForm.h"

using namespace AboutBox;

//Constructor. 
//Initializes the member variables and creates a table of appropriate size.
AboutBox::AboutBoxForm::AboutBoxForm()
{
	//Initialize the resources
	InitializeComponent();
}

//Initializes the resources
void AboutBoxForm::InitializeComponent()
{
	this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
	this->ClientSize = System::Drawing::Size(500, 250);
	this->Name = S"AboutSolimar";
	this->Text = S"About Solimar";
	
	RichTextBox* richTextBox1 = new RichTextBox();
	richTextBox1->BackColor = System::Drawing::SystemColors::InactiveBorder;
    richTextBox1->Dock = DockStyle::Fill;
	richTextBox1->ReadOnly = true;
	    
	richTextBox1->SelectionFont = new System::Drawing::Font(S"Arial", 16, FontStyle::Bold);
	richTextBox1->SelectionAlignment = HorizontalAlignment::Center;
	richTextBox1->SelectedText = S"\n \n Key License Manager";

	richTextBox1->SelectionFont = new System::Drawing::Font(S"Arial", 8);
	richTextBox1->SelectionAlignment = HorizontalAlignment::Center;

	richTextBox1->AppendText(S"\n Copyright 1997-2004 Solimar Systems, Inc");
	richTextBox1->AppendText(S"\n All rights reserved");
	richTextBox1->AppendText(S"\n All trademark properties of their respective owners");
	richTextBox1->AppendText(S"\n \n Solimar Systems, Inc");
	richTextBox1->AppendText(S"\n 1515 Second Avenue");
	richTextBox1->AppendText(S"\n San Diego, CA 92101-3005");
	richTextBox1->AppendText(S"\n 92101-3005");

	this->Controls->Add(richTextBox1);

}
AboutBoxForm::~AboutBoxForm()
{
}

//Releases all of the resources that this object owns
void AboutBoxForm::Dispose(bool disposing)
{
	if (disposing && components)
	{
		components->Dispose();
	}
	
	__super::Dispose(disposing);
}