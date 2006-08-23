//------------------------------------------------------------------------------
// Filename:    KeyWizard.h
//------------------------------------------------------------------------------


#ifndef KeyWizardH
#define KeyWizardH
//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>

#include "clookups.h"
#include "key\newkey\keyprod.h"
#include "kyslevel.h"
#include "SftTree_OCX.h"
#include <Db.hpp>
#include <DBTables.hpp>
#include <OleCtrls.hpp>
#include "ModuleDetail.h"
#include <ToolWin.hpp>
#include <ImgList.hpp>
#include "SolSearcherEnterpriseDetails.h"
#include <DB.hpp>
#include <stack>

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
#define MAX_MODULES 64     // 16 cells * 4 modules per cell
#define SUGGESTED_USES 10  //times a key should be reused
#define PRIMARY_DESCRIPTOR          0x9AAB6B01L
#define PRIMARY_PASSWORD            0x1A00FED1L
#define SECONDARY_DESCRIPTOR        0xBF0A1143L

#define PARALLEL_KEY                0
#define USB_KEY                     1

// since these are defined in Pkey.h, we should use ifndef
#ifndef DAYS_PER_YEAR
#define DAYS_PER_YEAR               365
#endif
#ifndef HOURS_PER_DAY
#define HOURS_PER_DAY               24
#endif

//wizard states
const int wsStart = 1;
const int wsModuleSelection = 5;
const int wsSSESelection   = 7;
const int wsOrderSelection = 10;
const int wsKeySelection = 15;
const int wsFinal = 100;

//module initialize states
const int msNone       =0;
const int msMax        =1;
const int msMin        =2;
const int msClear      =3;

const int MAX_DAYS     =90;
const int DEFAULT_DAYS =32;
const int EXPIRED_DAYS =0;

const int ktfNone       =0;  //reserved state
const int ktfAddon      =1;
const int ktfBase       =2;
const int ktfReplacement=4;
const int ktfRelicense  =8;
const int ktfInventory  =16;
const int ktfBackup     =32;
const int ktfEmergency  =64;
//const int ktfSolimar =128;
const int ktfCustom     =256;
const int ktfLoan       =512;
const int ktfDemo       =1024;
const int ktfRental     =2048;
const int ktfDevelopment=4096;
const int ktfReserved   =0x80000000;


const int ktfAll = (ktfAddon|ktfBase|ktfReplacement|ktfRelicense|ktfInventory|ktfBackup|ktfEmergency|ktfCustom|ktfLoan|ktfDemo|ktfRental|ktfDevelopment);
const int ktfStandAlone = (ktfBase|ktfInventory|ktfBackup|ktfEmergency|ktfCustom|ktfLoan|ktfDemo|ktfRental);
const int ktfSPD = ktfAll;
const int ktfSPDlimited = ktfAll;
const int ktfProductWithoutModules = (ktfBase|ktfReplacement|ktfInventory|ktfBackup|ktfEmergency|ktfCustom|ktfLoan|ktfDemo|ktfRental);

//------------------------------------------------------------------------------
// Class(es)
//------------------------------------------------------------------------------

//==============================================================================
// class:       TKeyWizardFrm
//==============================================================================
class TEMP_BSTR
{
   BSTR bStr;
   wchar_t tmp_wstring[33];// _itow max return value
public:
   TEMP_BSTR(){ bStr = NULL; }
   TEMP_BSTR( wchar_t* string ){ bStr = SysAllocString(string);}
   ~TEMP_BSTR()
   {

      SysFreeString( bStr );
      bStr = NULL;
   }
   operator wchar_t*(){return(bStr);}

   BSTR operator=(wchar_t* string)
   {
      if(bStr==NULL)
      {
         bStr = SysAllocString(string);
      }
      else
      {
         SysFreeString(bStr);
         bStr = SysAllocString(string);
      }
      return(bStr);
   }
};


//==============================================================================
// class:       TKeyWizardFrm
//==============================================================================
class TKeyWizardFrm : public TForm
{
__published:	// IDE-managed Components
        TPanel *StartPanel;
        TLabel *Label5;
        TComboBox *ProductComboBox;
        TLabel *Label6;
        TComboBox *keyTypeComboBox;
        TLabel *Label1;
        TMaskEdit *ui_version;
        TLabel *DaysLabel;
        TMaskEdit *ui_days;
        TUpDown *DayUpDown;
        TPanel *ButtonPanel;
        TButton *NextBtn;
        TButton *BackBtn;
        TButton *CancelBtn;
        TBevel *Bevel1;
        TPanel *Panel1;
        TPanel *FinalPanel;
        TQuery *Query1;
        TDataSource *DataSource1;
        TDatabase *backoffice;
        TPanel *ModuleProgramPanel;
        TQuery *OrderListQuery;
        TImageList *ImageList1;
        TCheckBox *SelectModules;
        TModuleFrame *WizardModuleFrame;
        TButton *ProgramBtn;
        TMaskEdit *OutputEdit;
        TUpDown *OutputUnits;
        TLabel *OutputLabel;
        TEdit *SystemEdit;
        TLabel *SystemEditLabel;
        TLabel *Label7;
        TMemo *KeySummary;
        TLabel *eBONumberLabel;
        TEdit *eBONumberEditBox;
        TComboBox *keyClassComboBox;
        TLabel *keyClassLabel;
        TLabel *passwordComboBoxLabel;
        TComboBox *passwordComboBox;
        TQuery *cQuery;
        TDataSource *PasswordDataSource;
   TPanel *SolSearcherEnterprisePanel;
   TSolSearcherDetails *TSolSearcherDetails1;
        void __fastcall BackBtnClick(TObject *Sender);
        void __fastcall NextBtnClick(TObject *Sender);
        void __fastcall ProductComboBoxChange(TObject *Sender);
        void __fastcall keyTypeComboBoxChange(TObject *Sender);
        void __fastcall ui_daysChange(TObject *Sender);
        void __fastcall ProgramBtnClick(TObject *Sender);
        void __fastcall OutputEditChange(TObject *Sender);
        void __fastcall SelectModulesClick(TObject *Sender);
        void __fastcall SystemEditChange(TObject *Sender);
        void __fastcall eBONumberEditBoxChange(TObject *Sender);
        void __fastcall keyClassComboBoxChange(TObject *Sender);
        void __fastcall passwordComboBoxChange(TObject *Sender);
   void __fastcall ui_versionChange(TObject *Sender);
private:
        //
        // User declarations
        __fastcall TKeyWizardFrm(TComponent* Owner): TForm(Owner), key_record(NULL)/*, key(NULL)*/{}

        //
        // Initialization
        void initializeWizard();
        void setDefaults();
        void setStartPanelMembers();
        void setModulePanelMembers();
        void SetSSEMembers(){}
        bool hasModuleLicenses(){if(key_record->pkey->productId==SPD_PRODUCT||key_record->pkey->productId==CONNECT_PRODUCT||key_record->pkey->productId==QUANTUM_PRODUCT||key_record->pkey->productId==ICONVERT_PRODUCT||key_record->pkey->productId==SDX_DESIGNER_PRODUCT||key_record->pkey->productId==SOLSCRIPT_PRODUCT||key_record->pkey->productId==RUBIKA_PRODUCT||key_record->pkey->productId==SPDE_PRODUCT)return true;else return false;}

        //
        // START PANEL
        String getSummary();
        AnsiString geteBONumber();
        bool SPD_LEGACY;
        bool isSpdLegacy(SPD_LEGACY);
        int getSystemId();
        int getKeyTypeComboBoxIndex(Classes::TStrings* combobox,AnsiString search_string);
        void refreshKeyTypeList();
        void updateKeyTypeLookup();
        void updateVersionLookup();
        void updateDays();
        void updateVersion();
        void updatePasswordExp();
        void setPasswordComboBox();
        void setProductComboBox(ProductId _product);
        void setKeyClassComboBox();
        void showPasswordComboBox(bool bTurnOn);
        static const int convertProductToIndex[];
        static const int convertIndexToProduct[];
        unsigned short getOutput();
        unsigned short getSelectedVersion();
        unsigned short getSelectedProduct(){return convertIndexToProduct[ProductComboBox->ItemIndex];}

        //
        // utility variables
        char *endptr;
        char scratch[256]; //scratch buffer
        wchar_t w_scratch[33];

      //
      // Wizard States
      int wizard_state;
      int previous_wizard_state;
      int m_selectedProduct;
      /* cleaning wizard ---*/
      bool m_bNonPermanentKey;  //does key have ability to become permanent
      bool m_bModules;          //true if key contains modules
      /*--------------------*/


        //
        // Member Variables
        int key_type_flag;
        int customer_number;
        int edited_system_id;
        int edited_days_value;
        int edited_hours_value;
        bool EDIT_SYSTEMID;
        bool edited_version;
        bool edited_days;
        bool RESET_MODULE_PANEL;
        bool MODULE_STATE_CHANGE;
        AnsiString eBONumber;

        TPanel* active_panel;
        //ProtectionKey* key;            // pointer to SKeyRecord::pkey
        //ProtectionKey* pkey;
        SKeyRecord* const key_record;

        SpdProtectionKey *m_pModuleKey;
        SSProtectionKey  *m_pSolSearchKey;

        //std::list<int> wizard_list;
        std::stack<int> state; //current state of wizard

        //--temporary members--should be encapsulated-------//-
        //KeyTypeInfo key_type_list[12]; //key_type_count     //-
        //bool key_with_modules;                              //-
        //int selected_key_type_index;                        //-
        //setKeyTypeComboBox();                               //-
        KeyType getKeyType();                               //-
        //--------------------------------------------------//-
public:		// User declarations
         __fastcall TKeyWizardFrm(SKeyRecord* key_record, TComponent* Owner);


};
//---------------------------------------------------------------------------
extern PACKAGE TKeyWizardFrm *KeyWizardFrm;
//---------------------------------------------------------------------------
#endif
