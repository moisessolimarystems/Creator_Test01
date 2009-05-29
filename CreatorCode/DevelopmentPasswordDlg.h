//---------------------------------------------------------------------------

#ifndef DevelopmentPasswordDlgH
#define DevelopmentPasswordDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "kyslevel.h"  //SKeyRecord
//---------------------------------------------------------------------------
class TDlgDevelopmentPassword : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *PasswordSelection;
        TComboBox *passwordComboBox;
        TButton *OKButton;
        TButton *CancelButton;
        void __fastcall OKButtonClick(TObject *Sender);
        void __fastcall CancelButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TDlgDevelopmentPassword(TComponent* Owner);
        //virtual __fastcall TDlgDevelopmentPassword(TComponent* Owner );
        SKeyRecord *key_record;
};
//---------------------------------------------------------------------------
extern PACKAGE TDlgDevelopmentPassword *DlgDevelopmentPassword;
//---------------------------------------------------------------------------
#endif
