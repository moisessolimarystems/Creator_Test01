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
__fastcall TUnitsDlg::TUnitsDlg(TComponent* AOwner, unsigned int max)
	: TForm(AOwner), m_MaxUnits(max)
{
   char maxString[32];
   sprintf(maxString, "%d\0", m_MaxUnits);
   Edit1->MaxLength = strlen(maxString);
   m_MinUnits = 1;
}
//---------------------------------------------------------------------
void __fastcall TUnitsDlg::OkBtnClick(TObject *Sender)
{
   AnsiString WarnMsg("");
   try {
      m_Units = Edit1->Text.ToInt();

      if (m_Units > m_MaxUnits) {
         Application->MessageBox("Invalid Number of Units" , "Information", MB_OK|MB_ICONWARNING );
         Edit1->Text = m_MaxUnits;
         Edit1->SetFocus();
      }
      else if((m_Units%m_MinUnits) != 0) {       //checks if units are acceptable
         Application->MessageBox(WarnMsg.sprintf("Number of Units Must Be a Multiple of %i.", m_MinUnits).c_str() , "Information", MB_OK|MB_ICONWARNING );
         if(m_Units < m_MinUnits)                //set units to minimum
            Edit1->Text = m_MinUnits;
         else                                    //set units to a valid number(last valid number)
            Edit1->Text = m_Units - (m_Units%m_MinUnits);
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

void __fastcall TUnitsDlg::UpDownBtnClick(TObject *Sender,
      TUDBtnType Button)
{
      Edit1->Text = m_CounterUnits * (UpDownBtn->Position + 1);
}
//---------------------------------------------------------------------------

