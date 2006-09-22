//------------------------------------------------------------------------------
// Filename:    keyform.h
// Note:        Copyright (c) 1993-2001 by Solimar Systems, Inc.
//              All Rights Reserved.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#ifndef KeyFormH
#define KeyFormH

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "kyslevel.h"
#include "clookups.h"
#include "KeyMaster.h"
#include "KeyWizard.h"

#include <Db.hpp>
#include <DBGrids.hpp>
#include <DBTables.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include "ModuleDetail.h"
#include <Forms.hpp>
#include "SolSearcherEnterpriseDetails.h"
#include <DB.hpp>

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
#define S_RETURNED            0x0100
#define S_LOST                0x0200

#define create_modify_key     0x1
#define extension_pwd	      0x2
#define version_pwd	      0x4
#define permanent_pwd	      0x8
#define module_pwd	      0x10

#define prod_spd_dos 	      4

//values of dirty flag - specifies if information on tab needs to be updated.
#define key_tab               0x0001
#define module_tab            0x0002
#define password_tab          0x0004
#define solsearcher_tab       0x0008
#define all_tabs              (key_tab | module_tab | solsearcher_tab | password_tab)

#define SORT_CUSTOMER         0
#define SORT_KEY              1
#define SORT_PRODUCT          2
#define SORT_VERSION          3
#define SORT_EXPIRATION       4
#define SORT_ACTIVATION       5
#define SORT_DEACTIVATION     6
#define SORT_KEYTYPE          7
#define SORT_STATUS           8
#define SORT_SYSTEM           9



// PhysicalFlag -
//==============================================================================
enum PhysicalFlag
{
   pf_PROGRAM = 1,
   pf_EXTENSION = 2,
   pf_MODULE = 4,
   pf_VERSION = 8,
   pf_PERMANENT = 0x10,
   pf_REMOVE = 0x20,
   pf_DEACTIVATE = 0x40,
   pf_REPROGRAM = 0x80,
   pf_MODIFY_STATUS = 0x100,
   pf_RESERVE = 0x200,
   pf_NONPERMANENT = 0x300,
//   pf_SSKEY_SLAVE = 0x400,
//   pf_SSKEY_MASTER = 0x500,
};

// sPanel - display fields on status panel of key form
//==============================================================================
enum sPanel{ CUSTOMER, KEY, PRODUCT, VERSION, LICENSE };


//------------------------------------------------------------------------------
//              class TFCustomerKeys
//------------------------------------------------------------------------------
class TFCustomerKeys : public TForm
{
__published:	// IDE-managed Components
	TDataSource *KeyDataSource;
	TDataSource *HistoryDataSource;
	TQuery *PasswordQuery;
	TQuery *UtilityQuery;
	TDataSource *DataSource1;
	TQuery *UtilityQuery2;
	TPageControl *PageControl1;
	TTabSheet *TabSheetKeys;
	TTabSheet *TabSheetModules;
	TTabSheet *TabSheetPasswords;
	TDBGrid *KeyGrid;
	TPanel *KeyPanel;
	TLabel *Label4;
	TDBGrid *PswdGrid;
	TDBEdit *DBEdit1;
	TStatusBar *StatusBar1;
	TBevel *Bevel1;
        TSpeedButton *PrintBtn;
        TSpeedButton *ArchiveBtn;
        TGroupBox *Licensing;
        TLabel *StatusLabel;
        TLabel *PoolLabel;
        TEdit *output_devices;
        TPopupMenu *OrderMenu;
        TMenuItem *Set1;
        TMenuItem *Clear1;
        TMenuItem *N4;
        TMenuItem *GeneratePassword1;
        TMainMenu *MainMenu1;
        TMenuItem *mmKey;
        TMenuItem *mmPassword;
        TMenuItem *mmDeactivate;
        TMenuItem *mmProgram;
        TMenuItem *mmReprogram;
        TMenuItem *mmRemove;
        TMenuItem *mmLost;
        TMenuItem *mmReturned;
        TMenuItem *N1;
        TStoredProc *StoredProc1;
        TMenuItem *mmReserve;
        TModuleFrame *KeyFormModuleFrame;
        TMenuItem *mmExtension;
        TMenuItem *mmPermanent;
        TMenuItem *mmSPDOutput;
        TMenuItem *N2;
        TMenuItem *mmVersion;
        TMenuItem *mmTools;
        TMenuItem *mmToolUpdateModules;
        TMenuItem *mmUpdateVersion;
        TStoredProc *sp_GenerateSystemId;
        TQuery *QueryKey;
        TMenuItem *mmToBase;
        TMenuItem *N5;
        TMenuItem *mmExit;
        TLabel *OutputDescription;
        TMemo *DescriptionMemo;
        TEdit *PKNumberEditBox;
        TLabel *Label1;
        TPopupMenu *EditPopupMenu;
        TMenuItem *UpdatePKNumber;
        TMenuItem *mmMaxLU;
        TEdit *eBONumberEditBox;
        TLabel *eBOLabel;
        TLabel *keyClassLabelDescription;
        TLabel *keyClassLabel;
        TEdit *afpds_ps_ppmEdit;
        TEdit *xch_ps_dbcs_ppmEdit;
        TEdit *xch_ps_ppmEdit;
        TEdit *xch_pcl_ppmEdit;
        TEdit *xch_ipds_ppmEdit;
        TGroupBox *PPMGroupBox;
        TLabel *xch_ps_ppm_label;
        TLabel *xch_pcl_ppm_label;
        TLabel *xch_ps_dbcs_ppm_label;
        TLabel *xch_ipds_ppm_label;
        TLabel *afpds_ps_ppm_label;
        TDataSource *PasswordDataSource;
        TQuery *cQuery;
        TLabel *Label2;
        TLabel *keyTypeLabel;
        TMenuItem *Search1;
        TMenuItem *mmSearchByType;
        TMenuItem *mmSearchByCustomerNumber;
        TMenuItem *mmSearchByKeyNumber;
        TMenuItem *mmSearchByLicense;
        TLabel *KeyInfoCustomerLabel;
        TLabel *KeyInfoCustomerNameLabel;
        TMenuItem *mmSearchByVersion;
        TMenuItem *Date1;
        TMenuItem *mmExpirationDate;
        TMenuItem *mmActivationDate;
        TMenuItem *mmDeactivationDate;
        TMenuItem *UpdateeBONumber;
        TMenuItem *UpdateDescription;
   TTabSheet *SolSearcherEnterpriseProperties;
   TSolSearcherDetails *SolSearcherDetails;
        TGroupBox *SessionGroupBox;
        TEdit *operator_sessions;
        TEdit *user_sessions;
        TLabel *OperatorSessionLabel;
        TLabel *UserSessionLabel;
        TMenuItem *mmSPDEOperatorSessions;
        TMenuItem *mmSPDEUserSessions;
	void __fastcall OnKeyRowChange(TObject *Sender);
        void __fastcall mmExtensionClick(TObject* Sender);
	void __fastcall mmProgramClick(TObject *Sender);
	void __fastcall KeyGridDrawColumnCell(TObject *Sender, const TRect &Rect,
									int DataCol, TColumn *Column, TGridDrawState State);

	void __fastcall mmVersionClick(TObject *Sender);
	void __fastcall mmPermanentClick(TObject *Sender);
	void __fastcall OnTabChange(TObject *Sender);
	void __fastcall PrintBtnClick(TObject *Sender);
   void __fastcall ArchiveBtnClick(TObject *Sender);
	void __fastcall Exit1Click(TObject *Sender);
	void __fastcall mmSPDOutputClick(TObject *Sender);

        void __fastcall mmLostClick(TObject *Sender);
        void __fastcall mmReturnedClick(TObject *Sender);
        void __fastcall mmDeactivateClick(TObject *Sender);
        void __fastcall mmReprogramClick(TObject *Sender);
        void __fastcall mmRemoveClick(TObject *Sender);
        void __fastcall mmReserveClick(TObject *Sender);
        void __fastcall mmUpdateVersionClick(TObject *Sender);
        void __fastcall ToBaseClick(TObject *Sender);
        void __fastcall PswdGridKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall UpdatePKNumberClick(TObject *Sender);
        void __fastcall KeyFormModuleFramemmIncrementModuleClick(
          TObject *Sender);
        void __fastcall TabSheetModulesEnter(TObject *Sender);
        void __fastcall mmMaxLUClick(TObject *Sender);
        void __fastcall TabSheetModulesExit(TObject *Sender);
        void __fastcall KeyFormModuleFramePagesPerMinute1Click(
          TObject *Sender);
        void __fastcall KeyFormModuleFramemmPagesPerMinuteClick(
          TObject *Sender);
        void __fastcall KeyGridTitleClick(TColumn *Column);
        void __fastcall PswdGridDrawColumnCell(TObject *Sender,
          const TRect &Rect, int DataCol, TColumn *Column,
          TGridDrawState State);
        void __fastcall mmSearchByTypeClick(TObject *Sender);
        void __fastcall mmSearchByCustomerNumberClick(TObject *Sender);
        void __fastcall mmSearchByLicenseClick(TObject *Sender);
        void __fastcall mmSearchByKeyNumberClick(TObject *Sender);
        void __fastcall mmSearchByVersionClick(TObject *Sender);
        void __fastcall mmActivationDateClick(TObject *Sender);
        void __fastcall mmExpirationDateClick(TObject *Sender);
        void __fastcall mmDeactivationDateClick(TObject *Sender);
        void __fastcall UpdateeBONumberClick(TObject *Sender);
        void __fastcall UpdateDescriptionClick(TObject *Sender);
        void __fastcall PKNumberEditBoxChange(TObject *Sender);
        void __fastcall eBONumberEditBoxChange(TObject *Sender);
        void __fastcall DescriptionMemoChange(TObject *Sender);
        void __fastcall mmSPDEOperatorSessionsClick(TObject *Sender);
        void __fastcall mmSPDEUserSessionsClick(TObject *Sender);
        void __fastcall KeyFormModuleFramemmSetModuleClick(
          TObject *Sender);

private:
        // User declarations
        AnsiString QueryKeySQL;         // base SQL string for the key form
        AnsiString sql_statement;       // used to remember the last query.
        char *endptr;
        char scratch[256];              // scratch buffer
        static char* key_sort_type[];   // list of sort types for sql statement
        SKeyRecord* key_record;
        TDatabase* Database1;
        TStringList* PktList;
        unsigned int dirty_tab;
        unsigned int PHYSICAL_FLAG;
        unsigned short tmp_modules[TOTAL_MODULE_CELLS];
        unsigned short tmp_spde_modules[TOTAL_MODULE_CELLS+2];  //16 + 2 needed for spde
        //
        // GUI Options
        bool isAttachedKeyReady();
        void __fastcall clearPermissions();
        void __fastcall resetPhysicalFlag();
        void __fastcall resetSqlStatements();
        void __fastcall setGUIOptions();
        void __fastcall setKeyInfoValues();
        void __fastcall setOutputUnitsDisplay(unsigned short outputs);
        void __fastcall setStatusValues();
        void __fastcall showLicenseInfo(bool bTurnOn);
        void __fastcall showModuleInfo(bool bTurnOn);
        void __fastcall showPagesPerMinuteInfo(bool bTurnOn);
        void __fastcall showSessionsInfo(bool bTurnOn) ;
        ///void __fastcall showSSKeyInfo(bool bTurnOn);
        //
        // GUI functions
        void __fastcall LoadPswdGrid();
        void __fastcall RefreshKeyPage(int _index = -1);
        void __fastcall resetKey(ProductId);
        void __fastcall SetModFilter();
        void TabDataOnChange(bool);
        void setOperatorSessionUnitsDisplay(unsigned short units);
        void setUserSessionUnitsDisplay(unsigned short units);

        //
        // Search functions
        void searchCustomerNumber(String customerNumberSearchValue);
        void searchDate(int dateType, String greaterThanValue, String lessThanValue);
        void searchKeyNumber(String keyNumberSearchValue);
        void searchKeyType(String customerKeyTypeSearchValue);
        void searchLicense(String customerLicenseSearchValue);
        void searchVersionNumber(String versionSearchValue);


public:
        //constructor(s) and destructor(s)
        __fastcall TFCustomerKeys(TComponent* Owner):TForm(Owner){}
        __fastcall TFCustomerKeys(TComponent* Owner, AnsiString, int, int searchKey = -1 );
        virtual __fastcall ~TFCustomerKeys(){
                                              delete PktList;
                                              delete key_record;
                                            }

        //
        // Member variables
        static unsigned int PERMISSION_FLAG;
        String customerKeyNumber;
        String customerStatus;
        String keyStatus;
        String licenseStatus;
        String productStatus;
        String versionStatus;



        //
        // Password Functions
        void createExtensionPassword(unsigned short days);
        void createMaxLUValuePassword(int iMaxLUValue);
        void createOutputPassword(int output_units);
        void createOperatorSessionPassword(int operator_sessions);
        void createUserSessionPassword(int user_sessions);
        void createVersionPassword(unsigned short version);
        void modulePasswordCreated();

        //Packet Functions
        void createExtensionPackets(unsigned short days);
        void createPasswordPackets(unsigned short days);

        void createConcurrentUsersPassword(unsigned short units_licensed);
        void createIndexServersPassword(unsigned short units_licensed);
        void createReportServersPassword(unsigned short units_licensed);
        void createApplicationPassword(unsigned short units_licensed);
        void createDocumentAssemblerPassword(unsigned short units_licensed);
        //
        // Utility functions
        AnsiString getKeyTypeText(int key_type);
        bool dbSaveKey(SKeyRecord*,TKeyWizardFrm*);
        bool CheckDateFormat(AnsiString dateString);
        BSTR AsciiToBSTR(const char* pStr);

        //
        // Used to update the private member variable sql_statement for queries/searches
        AnsiString getQueryKeySQLStatement();
        void __fastcall setQueryKeySQLStatement(AnsiString new_statement);

};
//---------------------------------------------------------------------------
extern TFCustomerKeys *FCustomerKeys;
//---------------------------------------------------------------------------
#endif
