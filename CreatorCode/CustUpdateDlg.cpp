//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CustUpdateDlg.h"
//---------------------------------------------------------------------
#ifdef __BORLAND_CPP__
#pragma resource "..\\creator\\*.dfm"
#else
#pragma resource "*.dfm"
#endif
TUpdateDlg *UpdateDlg;

//---------------------------------------------------------------------
__fastcall TUpdateDlg::TUpdateDlg(TComponent* AOwner, UDState state)
	: TForm(AOwner)
{
	if( state == INSERT ){
   	Caption = "Insert Company";
      AcceptButton->Caption = "Insert";
   }
   else{
   	AcceptButton->Caption = "Update";
   	Caption = "Modify Company";
   }
}
//==============================================================================
//  AcceptButtonClick
//  Check company name string.  If NULL display an error message and return to
//  dialog, else return IDOK and close the dialog.
//==============================================================================
void __fastcall TUpdateDlg::AcceptButtonClick(TObject *Sender)
{
	AnsiString tmp = EditCompany->Text;

 	if(EditCompany->Text.IsEmpty() )// if edit field is blank
   	Application->MessageBox("Name field can not be empty.", "Update Error", MB_OK);
   else
   	ModalResult = IDOK;
}
//==============================================================================
//
//==============================================================================
void __fastcall TUpdateDlg::FormActivate(TObject *Sender)
{
	EditCompany->SetFocus();	
}
//==============================================================================
//
//==============================================================================
void __fastcall TUpdateDlg::EditCompanyKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift)
{
	if( Key == VK_RETURN )  // if user hits ENTER while editing the company name
   	AcceptButtonClick( this );  // then assume they want to insert or modify with the current Text value
}
//--------------------------------------------------------------------------- 
