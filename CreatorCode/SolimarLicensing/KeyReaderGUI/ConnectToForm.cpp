#include "stdafx.h"
#include "ConnectToForm.h"

using namespace KeyReaderGUI;
using namespace System::Runtime::InteropServices; // for class Marshal

#undef MessageBox
System::Void ConnectToForm::BTN_OK_Click(System::Object *  sender, System::EventArgs *  e)
{
	 bool bRetVal = true;
	 System::Windows::Forms::Cursor* storedCursor = this->Cursor;
	 this->Cursor = Cursors::WaitCursor;
	 if(!IsValidServer(CB_ServerList->Text->Trim()))
	 { 
			MessageBox::Show(this, "Failed to connect to specified License Server.", "Connection Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			ConnectToForm::CB_ServerList->Focused;	
			bRetVal = false;
	 }
	 
	 this->Cursor = storedCursor;
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

bool ConnectToForm::IsValidServer(String* ServerName)
{
	HRESULT hr = 0;
	int length;
	if(!(ServerName->Length > 0))
		ServerName = L"localhost";
	
	//include null terminating char
	length = ServerName->Length+1;
	wchar_t* lpwServerName = new WCHAR[length];
	const char* lpcServerName = (char*)(void*)Marshal::StringToHGlobalAnsi(ServerName);
	MultiByteToWideChar(CP_ACP, 0, lpcServerName, length, lpwServerName, length);
	Marshal::FreeHGlobal((System::IntPtr)(void*)lpcServerName);
	
	COSERVERINFO	serverInfo	= {0, lpwServerName, NULL, 0};
    MULTI_QI        multiQI     = {&IID_ISolimarLicenseSvr3, NULL, NOERROR};   
	hr = CoCreateInstanceEx(
						     __uuidof(CSolimarLicenseSvr),
							 NULL,
							 CLSCTX_REMOTE_SERVER | CLSCTX_LOCAL_SERVER,
							 &serverInfo,
							 1,
							 &multiQI);
	static_cast<ISolimarLicenseSvr3*>(multiQI.pItf)->Release();
	if(!SUCCEEDED(hr))
	{
		return false;
	}	
	return true;
}