#include "stdafx.h"
#include "PswdForm.h"

using namespace KeyReaderGUI;

//Constructor. 
//Initializes the member variables and creates a table of appropriate size.
PswdForm::PswdForm(PasswordValidation* ThePasswordValidater)
					: PasswordValidater(ThePasswordValidater)
{
	//Initialize the resources
	InitializeComponent();
}

PswdForm::~PswdForm()
{
	this->Dispose(true);
}

System::Void PswdForm::OkBtn_Click(System::Object *  sender, System::EventArgs *  e)
{
	String* InputString = this->PswdTextBox->Text; //this->Controls->get_Item(0)->Text;
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

System::Void PswdForm::CancelBtn_Click(System::Object *  sender, System::EventArgs *  e)
{
	this->Close();
}

System::Void PswdForm::PswdTextBox_Validated(System::Object *  sender, System::EventArgs *  e)
{
	// If all conditions have been met, clear the ErrorProvider of errors.
	PswdErrorProvider->SetError(this->PswdTextBox, S"");
}

System::Void PswdForm::PswdTextBox_Validating(System::Object *  sender, System::ComponentModel::CancelEventArgs *  e)
{
	String* errorMsg;
	if (!IsValidPassword(this->PswdTextBox->Text, &errorMsg)) 
	{
                // Cancel the event and select the text to be corrected by the user.
		e->Cancel = true;
		this->PswdTextBox->Select(0, this->PswdTextBox->Text->Length);
		
                // Set the ErrorProvider error with the text to display.
	    PswdErrorProvider->SetError(this->PswdTextBox, errorMsg);
    }
}

System::Void PswdForm::PswdTextBox_KeyDown(System::Object*  sender, System::Windows::Forms::KeyEventArgs*  e)
{
	if (e->KeyCode == Keys::Enter || e->KeyCode == Keys::Return)
		OkBtn_Click(sender,e);	
	else if (e->KeyCode == Keys::Escape)
		DialogResult = DialogResult::Cancel;
}

bool PswdForm::IsValidPassword(String* password,  String** errorMessage) 
{
        if (password->Length == 0) 
		{
            *errorMessage = S"Please enter a password.";
            return false;
        }
		/*
		if(!System::Text::RegularExpressions::Regex::IsMatch(number, "[^0-9]"))
		{
			    *errorMessage = S"";
                return true;
		}
        *errorMessage = S"Please enter a valid number.";
		*/
        return true;
}