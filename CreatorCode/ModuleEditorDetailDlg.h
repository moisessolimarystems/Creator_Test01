//---------------------------------------------------------------------------

#ifndef ModuleEditorDetailDlgH
#define ModuleEditorDetailDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TModuleEditorDetailsDlg : public TForm
{
__published:	// IDE-managed Components
        TButton *OK;
        TButton *Cancel;
        TGroupBox *GroupBox1;
        TLabel *ModIDLabel;
        TLabel *ModNameLabel;
        TLabel *ModTypeLabel;
        TLabel *ModEngineerLabel;
        TLabel *ModVersionAddedLabel;
        TButton *UpdateButton;
        TEdit *ModNameEdit;
        TEdit *ModVersionAddedEdit;
        TComboBox *ModTypeCombo;
        TComboBox *ModEngineerCombo;
        TLabel *ModVersionRemovedLabel;
        TEdit *ModVersionRemovedEdit;
        TLabel *ModIDNumberLabel;
        TLabel *ModulePartNumberLabel;
        TEdit *ModPartNumberEdit;
        TLabel *ModMaxLabel;
        TEdit *ModMaxEdit;
        TCheckBox *ModSpdDefaultCheckBox;
        TCheckBox *ModConnectivityDefaultCheckBox;
        TCheckBox *ModConnectivityModuleCheckBox;
        TLabel *ProductIDLabel;
        TCheckBox *ModiConvertDefaultCheckBox;
        TCheckBox *ModiConvertModuleCheckBox;
        void __fastcall UpdateButtonClick(TObject *Sender);
        void __fastcall ModConnectivityDefaultCheckBoxClick(
          TObject *Sender);
        void __fastcall ModiConvertDefaultCheckBoxClick(TObject *Sender);
private:	// User declarations
        
public:		// User declarations
        __fastcall TModuleEditorDetailsDlg(TComponent* Owner);
        
        // member variables
        unsigned short product_id;
        unsigned short mod_id;
        unsigned short mod_max;
        unsigned short mod_version_added;
        unsigned short mod_version_removed;
        AnsiString mod_name;
        AnsiString mod_engineer;
        AnsiString mod_type;
        AnsiString mod_partnumber;
        bool mod_spd_def;
        bool mod_connect_def;
        bool mod_connect_mod;
        bool mod_iConvert_def;
        bool mod_iConvert_mod;
        bool bExistingMember;    

};
//---------------------------------------------------------------------------
extern PACKAGE TModuleEditorDetailsDlg *ModuleEditorDetailsDlg;
//---------------------------------------------------------------------------
#endif
