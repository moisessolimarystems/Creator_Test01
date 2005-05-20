//----------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "PasswordReportFrm.h"
//----------------------------------------------------------------------------
#pragma resource "*.dfm"
TPasswordReport *PasswordReport;
//----------------------------------------------------------------------------
__fastcall TPasswordReport::TPasswordReport(TComponent* Owner, AnsiString where_clause)
    : TQuickRep(Owner)
{
   try
   {
      ReportQuery->SQL->Add(where_clause);
      ReportQuery->Open();

      //build key description string
      AnsiString key_label( Format("%X-%X %s v%X.%03X", OPENARRAY(TVarRec,(
                           ReportQuery->FieldByName("SCRnumber")->AsInteger,
                           ReportQuery->FieldByName("SKRnumber")->AsInteger,
                           ReportQuery->FieldByName("SPLvalue")->AsString,
                           ReportQuery->FieldByName("SKRversion")->AsInteger/0x1000,
                           ReportQuery->FieldByName("SKRversion")->AsInteger&0xFFF))));
      StaticKeyLabel->Caption = key_label;
      StaticKeyPassword->Caption = (ReportQuery->FieldByName("TDpassword")->AsString).SubString(PASSWORD_START, PASSWORD_LENGTH);
   }
   catch(EDBEngineError &e)
   {
      Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
   }
   catch(...)
   {
      StaticKeyLabel->Caption =  "Information not available";
   }
}
//----------------------------------------------------------------------------
