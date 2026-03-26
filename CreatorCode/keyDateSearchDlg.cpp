//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "keyDateSearchDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TdsSearchDlg *dsSearchDlg;

//==============================================================================
// Constructor
//==============================================================================
__fastcall TdsSearchDlg::TdsSearchDlg(TComponent* Owner)
        : TForm(Owner)
{

}

//==============================================================================
// Function:     dsSearchButtonClick
// Purpose:      Event handler for the search/ok button clicked
// Parameters:   TObject *Sender
// Returns:      None
//==============================================================================
void __fastcall TdsSearchDlg::dsSearchButtonClick(TObject *Sender)
{
    AnsiString min_exp, max_exp;
    int search_param;
    int pos;

    min_exp = lessThanEditBox->Text;
    max_exp = greaterThanEditBox->Text;

    // Simple Checking...
    // Check for the length of the strings to be consistent with the date format
    pos = min_exp.Pos("/");
    if ( (min_exp.Length() != 10) || (pos != 3) )
    {
        lessThanEditBox->Text = "";
        min_exp = "";
    }

    pos = max_exp.Pos("/");
    if ( (max_exp.Length() != 10) || (pos != 3) )
    {
        greaterThanEditBox->Text = "";
        max_exp = "";
    }

    lessThanDate = lessThanEditBox->Text;
    greaterThanDate = greaterThanEditBox->Text;
}
