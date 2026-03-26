//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "CustQueryDlg.h"

//---------------------------------------------------------------------------
#ifdef __BORLAND_CPP__
#pragma resource "..\\creator\\*.dfm"
#else
#pragma resource "*.dfm"
#endif
TQueryDialog *QueryDialog;
//---------------------------------------------------------------------------
__fastcall TQueryDialog::TQueryDialog(TComponent* Owner, QueryByType state)
	: TForm(Owner)
{
   QUERY_STATE = state;
   if( state == ByNUMBER ){//by number
   	Caption = "Search by number";
   	NumLabel->Visible = true;
      NameLabel->Visible = false;
      EditSearch->Left = 111;
      EditSearch->Width = 35;
      EditSearch->MaxLength = 3;
   }
   else{ // by name
   	Caption = "Search by name";
   	NumLabel->Visible = false;
      NameLabel->Visible = true;
      EditSearch->Left = 45;
      EditSearch->Width = 145;
      EditSearch->MaxLength = 40;
   }
}
//---------------------------------------------------------------------------
void __fastcall TQueryDialog::AcceptButtonClick(TObject *Sender)
{
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TQueryDialog::CancelButtonClick(TObject *Sender)
{
   ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TQueryDialog::EditSearchKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift)
{
   if( Key == VK_RETURN )
   	AcceptButtonClick(this);

   //if( QUERY_STATE == ByNUMBER ){ //if my number, mask out any non-hex chars
	//	if( !(Key >= 0x30 && Key <= 0x39) || (Key >= 0x41 && Key <= 0x47) ){
   //		Key = 0;
   //	}
   //}
}
//---------------------------------------------------------------------------
