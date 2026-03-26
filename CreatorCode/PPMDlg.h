//---------------------------------------------------------------------------

#ifndef PPMDlgH
#define PPMDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TDlgPPM : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TBevel *Bevel1;
        TMaskEdit *PagesEditBox;
        TButton *CancelBtn;
        TButton *okBtn;
        void __fastcall OkBtnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TDlgPPM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDlgPPM *DlgPPM;
//---------------------------------------------------------------------------
#endif
