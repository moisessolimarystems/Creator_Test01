//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
#ifndef CUSTOMERFORM_H
#define CUSTOMERFORM_H
//------------------------------------------------------------------------------

#include <stdio.h>
#include <time.h>

#include "CreatorAbout.h"
#include "CustUpdateDlg.h"
#include "CustQueryDlg.h"
#include "KeyForm.h"
#include "ModuleEditorDlg.h"
#include "ConnectionEditor.h"


#include <Db.hpp>
#include <DBTables.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include "ModuleDetail.h"
#include <Forms.hpp>
#include "ModuleEditor.h"
#include <DB.hpp>

#define MAX_KEYS                    0x100
#define HEX_MODE                    16
#define DECIMAL_MODE                10
#define MAX_KEY_NUMBER              0xFF
#define CUST_QUERY_PARAMS_COUNT  	3
#define MAX_ATTEMPTS						3

//------- message codes --------------------------------------------------------
#define DUPLICATE_INPUT             1
#define NO_COMPANY_NAME             2
#define INVALID_CUSTOMER_NUMBER     3
#define DELETE_CUSTOMER_WARNING     4
#define CHANGED_CUSTOMER_NUMBER     5
#define DATABASE_EMPTY              6
#define FILE_ERROR                  7
#define FILE_SUCCESS                8
#define DUPLICATE_KEY_NUMBER        9
#define KEY_NOT_FOUND               10
#define KEY_ERROR                   11
#define PROGRAM_KEY_ERROR           12
#define KEY_SUCCESS                 13
#define DELETE_KEY_WARNING          14
#define KEY_DEACTIVATED             15
#define MOD_MAXED_OUT               16
#define KEY_USED_UP                 17
#define DEACTIVATE_BOX              18
#define CANT_DEACTIVATE_HERE        19
#define CANT_REPROGRAM_HERE         20
#define KEY_NOT_PROGRAMMED          21
#define MAX_KEYS_REACHED            22
#define DELETE_PASSWORD_WARNING     23
#define NO_DEFAULT_LICENSING        24
#define PRINT_ERROR                 25
#define MEMORY_ALLOCATION_ERROR     26
//------------------------------------------------------------------------------
enum SortType
{
   st_by_customer_number,
   st_by_customer_name,
   st_by_customer_keys
};

class TCustForm : public TForm
{
__published:	// IDE-managed Components
	TDataSource *Cust_Source;
	TMainMenu *MainMenu1;
	TMenuItem *SearchMenu;
	TQuery *Cust_Query;
	TMenuItem *New1;
	TMenuItem *mmAll;
	TMenuItem *N1;
	TMenuItem *Close1;
   TDatabase *CFdatabase;
	TMenuItem *Name1;
	TMenuItem *MMCustomer;
	TMenuItem *mmInsert;
	TMenuItem *mmModify;
	TQuery *UtilityQuery1;
        TQuery *ModuleQuery;
	TMenuItem *Key1;
	TMenuItem *View1;
	TMenuItem *Deactivate1;
	TMenuItem *N2;
	TMenuItem *Seek1;
   TMenuItem *MMAbout;
   TDBGrid *CustomerGrid;
   TPageControl *PageControl1;
   TTabSheet *CellDetailTab;
   TPanel *Panel1;
   TBevel *ZoomDetailBevel;
   TStringGrid *KeyCellGrid;
   TStaticText *ZoomDetailTextDescription0;
   TEdit *ZoomDetailValue0;
   TStaticText *ZoomDetailTextDescription1;
   TEdit *ZoomDetailValue1;
   TStaticText *ZoomDetailTextDescription3;
   TEdit *ZoomDetailValue3;
   TStaticText *ZoomDetailTextDescription2;
   TEdit *ZoomDetailValue2;
   TTabSheet *GeneralTab;
   TModuleFrame *AttachedKeyModuleFrame;
   TLabel *keyLabel;
   TLabel *Label2;
   TLabel *Label3;
   TLabel *productLabel;
   TLabel *outputUnits;
   TLabel *Label6;
   TLabel *versionLabel;
   TLabel *Label9;
   TLabel *Label10;
   TLabel *Label11;
   TLabel *licenseLabel;
        TMenuItem *ModuleEditor;
        TListBox *ZoomDetailListBox;
        TScrollBar *CellViewScrollBar;
        TLabel *Label1;
        TLabel *operatorUnits;
        TLabel *Label4;
        TLabel *userUnits;
        TMenuItem *mmConnectionEditor;
	void __fastcall Number1Click(TObject *Sender);
	void __fastcall mmAllClick(TObject *Sender);
	void __fastcall Name1Click(TObject *Sender);
	void __fastcall mmInsertClick(TObject *Sender);
	void __fastcall mmModifyClick(TObject *Sender);
	void __fastcall Deactivate1Click(TObject *Sender);
	void __fastcall Close1Click(TObject *Sender);
	void __fastcall View1Click(TObject *Sender);
	void __fastcall SeekCustomer(TObject *Sender);
	void __fastcall AssignSQL_CustQuery(TObject *Sender, int pick);
	void __fastcall CreateCustomerKeyView(TObject *Sender);
        void __fastcall MMAboutClick(TObject *Sender);
        void __fastcall CustomerGridDrawColumnCell(TObject *Sender,
          const TRect &Rect, int DataCol, TColumn *Column,
          TGridDrawState State);
        void __fastcall CustomerGridDblClick(TObject *Sender);
        void __fastcall KeyCellGridClick(TObject *Sender);
        void __fastcall CustomerGridKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall ModuleEditorClick(TObject *Sender);
        void __fastcall CellViewScrollBarChange(TObject *Sender);
        void __fastcall mmConnectionEditorClick(TObject *Sender);
private:	// User declarations
	void __fastcall SetCustomerForm();
public:		// User declarations
        TFCustomerKeys *KeyGridPtr;
        char scratch[256];
        AnsiString baseSQL;     // base SQL customer form search
        static char* sort_type[];
        AnsiString targetSQL;
        AnsiString condition;  // SQL WHERE clause
        AnsiString custParams[CUST_QUERY_PARAMS_COUNT];

	__fastcall TCustForm(TComponent* Owner);
        __fastcall ~TCustForm();
        void __fastcall CreateQueryDialog( QueryByType state );
        bool __fastcall LoadKeyInfo();
        void __fastcall SeekCustomer();
        bool SetPermissions();

        //keyview utility members
        SKeyRecord *attached_key;
        char cell_text[33]; //33 max return value from itoa
        int cell;
        char* endptr;
        bool hex_view;

private:
        bool resetScroll;
};
//---------------------------------------------------------------------------
extern TCustForm *CustForm;
//---------------------------------------------------------------------------
#endif
