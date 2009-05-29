//----------------------------------------------------------------------------
#ifndef CustUpdateDlgH
#define CustUpdateDlgH

#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//----------------------------------------------------------------------------


enum UDState{ INSERT, MODIFY };
//----------------------------------------------------------------------------
class TUpdateDlg : public TForm
{
__published:        
	TButton *CancelBtn;
	TBevel *Bevel1;
	TLabel *Label1;
	TSpeedButton *AcceptButton;
	TEdit *EditCompany;
	void __fastcall AcceptButtonClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall EditCompanyKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift);
private:
public:
	virtual __fastcall TUpdateDlg(TComponent* AOwner):TForm(AOwner){}
	virtual __fastcall TUpdateDlg(TComponent* AOwner, UDState);
};
//----------------------------------------------------------------------------
extern TUpdateDlg *UpdateDlg;
//----------------------------------------------------------------------------
#endif    
