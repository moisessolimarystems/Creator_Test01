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
#include <ComCtrls.hpp>
//----------------------------------------------------------------------------
class TUnitsDlg : public TForm
{
__published:
	TButton *CancelBtn;
	TBevel *Bevel1;
	TLabel *Label1;
	TEdit *Edit1;
        TSpeedButton *OkBtn;
        TUpDown *UpDownBtn;

	void __fastcall OkBtnClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
        void __fastcall CancelBtnClick(TObject *Sender);
        void __fastcall UpDownBtnClick(TObject *Sender, TUDBtnType Button);
private:
   unsigned int m_Units;
   unsigned int m_MaxUnits;
   unsigned int m_MinUnits;
   unsigned int m_CounterUnits;
   bool m_Counter;
public:
   virtual __fastcall TUnitsDlg(TComponent* AOwner, unsigned int max);
   unsigned int GetUnits() {return m_Units;}
   void SetMinUnits(unsigned int m_Val) { m_MinUnits = m_Val;}
   void SetCounterUnits(unsigned int m_Val, unsigned int m_MaxVal) {
                                             m_CounterUnits = m_Val;
                                             Edit1->Text = m_CounterUnits;
                                             UpDownBtn->Max = ((m_MaxVal/m_Val) - 1);
                                             }
   void EnableCounter(bool m_Val) {UpDownBtn->Visible = m_Val;}
};
//----------------------------------------------------------------------------
extern TUnitsDlg *UnitsDlg;
//----------------------------------------------------------------------------
#endif
