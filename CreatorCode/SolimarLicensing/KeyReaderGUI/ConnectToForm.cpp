#include "stdafx.h"
#include "ConnectToForm.h"

using namespace KeyReaderGUI;
using namespace System::Runtime::InteropServices; // for class Marshal

ConnectToForm::ConnectToForm(CommunicationLink* CommLink, ArrayList* _ServerList)
{
	InitializeComponent();
	TheCommLink = CommLink;
	if (_ServerList != NULL)
	{
		for(int i=0; i<_ServerList->Count; i++)
		{
			CB_ServerList->Items->Insert(i,_ServerList->Item[i]);
		}	
		if(_ServerList->Count > 0)
			CB_ServerList->SelectedIndex = 0;
	}
}
ConnectToForm::~ConnectToForm()
{
	TheCommLink = NULL;
	this->Dispose(true);		
}

#undef MessageBox
System::Void ConnectToForm::BTN_OK_Click(System::Object *  sender, System::EventArgs *  e)
{
	 HRESULT hr;
	 bool bRetVal = true;

	 if(TheCommLink)
	 {
		 System::Windows::Forms::Cursor* storedCursor = this->Cursor;
		 this->Cursor = Cursors::WaitCursor;
		 hr = TheCommLink->Connect(CB_ServerList->Text->Trim());//IsValidServer(CB_ServerList->Text->Trim());
		 if(!SUCCEEDED(hr))
		 { 
				MessageBox::Show(this, "Failed to connect to specified License Server.", "Connection Error", MessageBoxButtons::OK, MessageBoxIcon::Error);	
				TheCommLink->WriteEventLog(String::Format("Solimar Systems, Inc.\r\nProduct Licensing Error Message\r\n\r\nFailed To Connect To Server: {0}\r\n{1:X8}", 
														  CB_ServerList->Text->Trim(),									  
														  hr.ToString("X")),
										   EventLogEntryType::Error); 
				ConnectToForm::CB_ServerList->Focused;	
				bRetVal = false;
		 }
		 else
			 TheCommLink->Disconnect();	//Disconnect after testing successful connection
		 this->Cursor = storedCursor;
	 }
	 if(bRetVal)
	 {
		 DialogResult = DialogResult::OK;
	 }
}
System::Void ConnectToForm::BTN_Cancel_Click(System::Object *  sender, System::EventArgs *  e)
{
	 DialogResult = DialogResult::Cancel;
}

//Form Events
System::Void ConnectToForm::ConnectToForm_Load(System::Object *  sender, System::EventArgs *  e)
{
}
System::Void ConnectToForm::ConnectToForm_Closing(System::Object *  sender, System::ComponentModel::CancelEventArgs *  e)
{
}

System::Void ConnectToForm::CB_ServerList_KeyDown(System::Object*  sender, System::Windows::Forms::KeyEventArgs*  e)
{
	if (e->KeyCode == Keys::Enter || e->KeyCode == Keys::Return)
		BTN_OK_Click(sender, e);	
	else if (e->KeyCode == Keys::Escape)
		DialogResult = DialogResult::Cancel;
}

System::Void ConnectToForm::CB_ServerList_Validated(System::Object*  sender, System::EventArgs*  e) 
{
	// If all conditions have been met, clear the ErrorProvider of errors.
//	ConnectErrorProvider->SetError(this->PswdTextBox, S"");
}
System::Void ConnectToForm::CB_ServerList_Validating(System::Object*  sender, System::ComponentModel::CancelEventArgs*  e)
{
/*	String* errorMsg;
	//if (!IsValidServer(this->PswdTextBox->Text, &errorMsg)) 
	if(!(this->CB_ServerList->Text->Length > 0))
	{
                // Cancel the event and select the text to be corrected by the user.
		e->Cancel = true;
		this->PswdTextBox->Select(0, this->PswdTextBox->Text->Length);
		
                // Set the ErrorProvider error with the text to display.
	    PswdErrorProvider->SetError(this->PswdTextBox, errorMsg);
    }
*/
}
