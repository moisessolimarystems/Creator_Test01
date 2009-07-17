//----------------------------------------------------------------------------
#ifndef VersionDlgH
#define VersionDlgH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Mask.hpp>
//----------------------------------------------------------------------------
class TDlgVersion : public TForm
{
__published:
	TButton *CancelBtn;
	TBevel *Bevel1;
   TMaskEdit *VersionEdit;
	TBitBtn *OKBtn;
   TComboBox *ProductComboBox;
   TButton *ApplyBtn;
	void __fastcall OKBtnClick(TObject *Sender);
   void __fastcall ProductComboBoxChange(TObject *Sender);
   void __fastcall ApplyBtnClick(TObject *Sender);

private:
	bool DOS_to_NT;
   bool ALL_PRODUCTS;
   unsigned short version;
   unsigned short init_version;

   static const int convertProductToIndex[];
   static const int convertIndexToProduct[];

public:
	//virtual __fastcall TDlgVersion(TComponent* AOwner):TForm(AOwner){}
   virtual __fastcall TDlgVersion(TComponent* AOwner);

   unsigned short getVersion(){return version;}
   void setDialog(unsigned short, unsigned short, bool);

BEGIN_MESSAGE_MAP
END_MESSAGE_MAP( TForm )
};
//----------------------------------------------------------------------------
extern TDlgVersion *DlgVersion;
//----------------------------------------------------------------------------
#endif
