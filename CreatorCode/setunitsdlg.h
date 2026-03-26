//----------------------------------------------------------------------------
#ifndef SetUnitsDlgH
#define SetUnitsDlgH
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
//----------------------------------------------------------------------------
class TUnitsDlg : public TForm
{
__published:        
	TButton *CancelBtn;
	TBevel *Bevel1;
	TLabel *Label1;
	TEdit *Edit1;
        TSpeedButton *OkBtn;

	void __fastcall OkBtnClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
        void __fastcall CancelBtnClick(TObject *Sender);
private:
   unsigned short m_Units;
   unsigned short m_MaxUnits;
   unsigned short m_MinUnits;
public:
	virtual __fastcall TUnitsDlg(TComponent* AOwner, unsigned short max);
   unsigned short GetUnits(){return m_Units;}

};
//----------------------------------------------------------------------------
extern TUnitsDlg *UnitsDlg;
//----------------------------------------------------------------------------
#endif    
