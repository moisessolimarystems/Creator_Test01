//---------------------------------------------------------------------------
#ifndef CustQueryDlgH
#define CustQueryDlgH

#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------


enum QueryByType{ ByNUMBER, ByNAME };
//---------------------------------------------------------------------------
class TQueryDialog : public TForm
{
__published:	// IDE-managed Components
	TEdit *EditSearch;
	TButton *AcceptButton;
	TButton *CancelButton;
	TBevel *Bevel1;
	TLabel *NumLabel;
	TLabel *NameLabel;
	void __fastcall AcceptButtonClick(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);
	void __fastcall EditSearchKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift);
private:	// User declarations
public:		// User declarations
	__fastcall TQueryDialog(TComponent* Owner ): TForm( Owner ){}
	__fastcall TQueryDialog(TComponent* Owner, QueryByType);
   QueryByType QUERY_STATE;

};
//---------------------------------------------------------------------------
extern TQueryDialog *QueryDialog;
//---------------------------------------------------------------------------
#endif
