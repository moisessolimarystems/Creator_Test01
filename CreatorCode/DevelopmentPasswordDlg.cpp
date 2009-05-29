//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DevelopmentPasswordDlg.h"
#include "kyslevel.h"  //SKeyRecord

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDlgDevelopmentPassword *DlgDevelopmentPassword;
//---------------------------------------------------------------------------
__fastcall TDlgDevelopmentPassword::TDlgDevelopmentPassword(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------

void __fastcall TDlgDevelopmentPassword::OKButtonClick(TObject *Sender)
{
   ModalResult = IDYES;
}
//---------------------------------------------------------------------------
void __fastcall TDlgDevelopmentPassword::CancelButtonClick(TObject *Sender)
{
    ModalResult = IDCANCEL;
}
//---------------------------------------------------------------------------
