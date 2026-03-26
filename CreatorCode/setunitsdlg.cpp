//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "SetUnitsDlg.h"
//--------------------------------------------------------------------- 
#ifdef __BORLAND_CPP__
#pragma resource "..\\creator\\*.dfm"
#else
#pragma resource "*.dfm"
#endif
TUnitsDlg *UnitsDlg;
//---------------------------------------------------------------------
__fastcall TUnitsDlg::TUnitsDlg(TComponent* AOwner, unsigned short max)
	: TForm(AOwner), m_MaxUnits(max)
{
   char maxString[32];
   sprintf(maxString, "%d\0", m_MaxUnits);
   Edit1->MaxLength = strlen(maxString);
   m_MinUnits = 0;
}
//---------------------------------------------------------------------
void __fastcall TUnitsDlg::OkBtnClick(TObject *Sender)
{
   try {
      m_Units = Edit1->Text.ToInt();
      if (m_Units > m_MaxUnits) {
   	   //Application->MessageBox("Invalid Number of Units"/*message*/ , "Information", MB_OK|MB_ICONWARNING );
         Edit1->Text = m_MaxUnits;
         Edit1->SetFocus();
      }
      else
         ModalResult = mrYes;

   }
   catch (...) {
      //invalid entry
      m_Units = m_MinUnits;
      Edit1->Text = m_MinUnits;
      Edit1->SetFocus();
   }
}
//---------------------------------------------------------------------------
void __fastcall TUnitsDlg::FormShow(TObject *Sender)
{
	Edit1->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TUnitsDlg::CancelBtnClick(TObject *Sender)
{
   ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

