//----------------------------------------------------------------------------
#ifndef ConnectionEditorH
#define ConnectionEditorH
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
class TConnectionEditorDlg : public TForm
{
__published:
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
        TLabel *Label1;
        TEdit *KeyServerNameEB;
        void __fastcall OKBtnClick(TObject *Sender);

private:
public:
	virtual __fastcall TConnectionEditorDlg(TComponent* AOwner);
        bool IsValidServer(AnsiString serverName);
};
//----------------------------------------------------------------------------
extern PACKAGE TConnectionEditorDlg *ConnectionEditorDlg;
//----------------------------------------------------------------------------
#endif
