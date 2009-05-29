//---------------------------------------------------------------------------

#ifndef KeySearchDlgH
#define KeySearchDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TksdSearchDlg : public TForm
{
public:		// User declarations
        __fastcall TksdSearchDlg(TComponent* Owner);

                // Member Variables
        String keyNumber;
        String versionNumber;
        String customerNumber;
        String license;
        String keyType;

__published:	// IDE-managed Components
        TPanel *ksdPanel;
        TButton *ksdSearchButton;
        TButton *ksdCancelButton;
        TLabel *ksdKeyNumberLabel;
        TMaskEdit *ksdKeyMaskEdit;
        TLabel *ksdVersionNumberLabel;
        TMaskEdit *ksdVersionMaskEdit;
        TLabel *ksdLicenseLabel;
        TComboBox *ksdLicenseComboBox;
        TLabel *ksdKeyTypeLabel;
        TComboBox *ksdKeyTypeComboBox;
        TLabel *ksdCustomerNumberLabel;
        TMaskEdit *ksdCustomerMaskEdit;
        void __fastcall ksdKeyMaskEditChange(TObject *Sender);
        void __fastcall ksdVersionMaskEditChange(TObject *Sender);
        void __fastcall ksdCustomerMaskEditChange(TObject *Sender);
        void __fastcall ksdLicenseComboBoxChange(TObject *Sender);
        void __fastcall ksdKeyTypeComboBoxChange(TObject *Sender);
private:	// User declarations

};
//---------------------------------------------------------------------------
extern PACKAGE TksdSearchDlg *ksdSearchDlg;
//---------------------------------------------------------------------------
#endif
