//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PPMDlg.h"
#include <stdio.h>     //sprintf
#include "kyslevel.h"  //SKeyRecord

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDlgPPM *DlgPPM;
//---------------------------------------------------------------------------
__fastcall TDlgPPM::TDlgPPM(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDlgPPM::OkBtnClick(TObject *Sender)
{
    int ppm = PagesEditBox->Text.ToInt();
    if (ppm > 500)
    {
       Application->MessageBox("Invalid entry, number must \n\rrange from 1 to 500. \n\rAnything greater than 500 is unlimited", "Warning", MB_OK|MB_ICONWARNING );
       PagesEditBox->Text = 500;
       PagesEditBox->SetFocus();
    }
    return;
}



