//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "KeySearchDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TksdSearchDlg *ksdSearchDlg;
//---------------------------------------------------------------------------
__fastcall TksdSearchDlg::TksdSearchDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TksdSearchDlg::ksdKeyMaskEditChange(TObject *Sender)
{
        keyNumber = ksdKeyMaskEdit->Text.c_str();
}
//---------------------------------------------------------------------------

void __fastcall TksdSearchDlg::ksdVersionMaskEditChange(TObject *Sender)
{
        versionNumber = ksdVersionMaskEdit->Text.c_str();
}
//---------------------------------------------------------------------------

void __fastcall TksdSearchDlg::ksdCustomerMaskEditChange(
      TObject *Sender)
{
        customerNumber = ksdCustomerMaskEdit->Text.c_str();
}
//---------------------------------------------------------------------------

void __fastcall TksdSearchDlg::ksdLicenseComboBoxChange(TObject *Sender)
{
        license = ksdLicenseComboBox->Text.c_str();        
}
//---------------------------------------------------------------------------

void __fastcall TksdSearchDlg::ksdKeyTypeComboBoxChange(TObject *Sender)
{
        keyType = ksdKeyTypeComboBox->Text.c_str();        
}
//---------------------------------------------------------------------------

