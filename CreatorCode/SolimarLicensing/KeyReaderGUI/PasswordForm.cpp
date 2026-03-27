#include "stdafx.h"
#include "PasswordForm.h"

using namespace PWDForm;

//Constructor. 
//Initializes the member variables and creates a table of appropriate size.
PasswordForm::PasswordForm(PasswordValidation* ThePasswordValidater)
					: PasswordValidater(ThePasswordValidater)
{
	//Initialize the resources
	InitializeComponent();
}

PasswordForm::~PasswordForm()
{
}

//Initializes the resources
void PasswordForm::InitializeComponent()
{
	//Initializes the module license form.
	components = new System::ComponentModel::Container();
	AutoScaleBaseSize = System::Drawing::Size(5, 13);
	ClientSize = System::Drawing::Size(400, 100);
	Text = S"Password";

	// Create a new TextBox control using this constructor.
    TextBox* textBox1 = new TextBox();

	textBox1->Location = System::Drawing::Point(14, 40);
    textBox1->Size = System::Drawing::Size(60, 24);
	textBox1->Text = S"Password: ";
	textBox1->Multiline = false;
    textBox1->ReadOnly = true;
	textBox1->BorderStyle = System::Windows::Forms::BorderStyle::None;

	// Create a new TextBox control using this constructor.
    TextBox* textBox2 = new TextBox();

	textBox2->Location = System::Drawing::Point(77, 36);
    textBox2->Size = System::Drawing::Size(260, 24);
	textBox2->Multiline = false;
    textBox2->ReadOnly = false;
	Controls->Add(textBox2);
	Controls->Add(textBox1);

	//create button
	Button* button1 = new Button ();

	button1->Location = System::Drawing::Point(300, 75);
    button1->Size = System::Drawing::Size(40, 25);
    button1->Text = S"OK";
	this->AcceptButton = button1;
    button1->Click += new System::EventHandler(this, &PasswordForm::button1_Click);
	
	//add the button to the form
	Controls->Add(button1);
}

//event handler for when the module license button is clicked
System::Void PWDForm::PasswordForm::button1_Click(System::Object*  sender, System::EventArgs *  e)
{
	String* InputString = this->Controls->get_Item(0)->Text;
	HRESULT hr = PasswordValidater->CheckPassword(InputString);

#undef MessageBox
	//The protocol is S_FALSE signifies invalid password
	if(hr == S_FALSE)
	{
		System::Windows::Forms::MessageBox::Show("Invalid Password");
	}
	//S_OK signifies correct password
	else if(hr == S_OK)
	{
		//refresh both windows
	}
	//else the function failed
	else
	{
		System::Windows::Forms::MessageBox::Show("Function Call Failed");
	}
	this->Close();
}
//Releases all of the resources that this object owns
void PasswordForm::Dispose(bool disposing)
{
	if (disposing && components)
	{
		components->Dispose();
	}
	
	__super::Dispose(disposing);
}
