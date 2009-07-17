//----------------------------------------------------------------------------
#ifndef ExtensionDlgH
#define ExtensionDlgH

#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include <StdCtrls.hpp>
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
class TDlgExtension : public TForm
{
__published:
	TButton *CancelBtn;
	TBevel *Bevel1;
	TLabel *Label1;
   TSpeedButton *OkBtn;
	TMaskEdit *Days;
	void __fastcall OkBtnClick(TObject *Sender);
private:
public:
	virtual __fastcall TDlgExtension(TComponent* AOwner):TForm(AOwner){}
   virtual __fastcall TDlgExtension(TComponent* AOwner, SKeyRecord* );
   //void __fastcall OnKeyDown( TWMKeyDown Message);

   //unsigned short extNum;
   //KeyStatus new_status;
   SKeyRecord *key_record;
   //long password;
   //char pswdString[9];
   //int product;

};
//----------------------------------------------------------------------------
extern TDlgExtension *DlgExtension;
//----------------------------------------------------------------------------
#endif    
