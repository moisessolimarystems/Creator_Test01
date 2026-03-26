//---------------------------------------------------------------------------

#ifndef keyDateSearchDlgH
#define keyDateSearchDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TdsSearchDlg : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TLabel *MinDate;
        TEdit *lessThanEditBox;
        TLabel *MaxDate;
        TEdit *greaterThanEditBox;
        TButton *dsSearchButton;
        TButton *dsCancelButton;
        TLabel *dsMainLabel;
        TLabel *Label2;
        TLabel *Label3;
        void __fastcall dsSearchButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TdsSearchDlg(TComponent* Owner);

        String greaterThanDate;
        String lessThanDate;
};
//---------------------------------------------------------------------------
extern PACKAGE TdsSearchDlg *dsSearchDlg;
//---------------------------------------------------------------------------
#endif
