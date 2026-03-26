//------------------------------------------------------------------------------
// Filename:    ModuleDetail.h
// Note:        Copyright (c) 1993-2001 by Solimar Systems, Inc.
//              All Rights Reserved.
//------------------------------------------------------------------------------


#ifndef ModuleDetailH
#define ModuleDetailH
//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "clookups.h"
#include "kyslevel.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <OleCtrls.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
#define MODE_1  1
#define MODE_2  2
#define MODE_3  3
#define MODE_4  4

//------------------------------------------------------------------------------
// Class(es)
//------------------------------------------------------------------------------
class TModuleFrame : public TFrame
{
__published:	// IDE-managed Components
   TQuery *ModuleDetailQuery;
   TListView *ModuleList;
   TPopupMenu *ModulePopup;
   TMenuItem *mmIncrementModule;
   TMenuItem *N1;
   TMenuItem *mmViewAll;
   TMenuItem *mmPagesPerMinute;
        TMenuItem *mmSetModule;
   void __fastcall ModuleListCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
   void __fastcall ModuleListColumnClick(TObject *Sender,
          TListColumn *Column);
   void __fastcall ModuleListKeyPress(TObject *Sender, char &Key);
   void __fastcall mmIncrementModuleClick(TObject *Sender);
   void __fastcall mmViewAllClick(TObject *Sender);
   void __fastcall mmPagesPerMinuteClick(TObject *Sender);
        void __fastcall mmSetModuleClick(TObject *Sender);
   void __fastcall ModuleListMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);

private:	// User declarations
   SKeyRecord*    key_record;

   bool           m_bPopup;
   bool           m_bChangeUnits;//CHANGE_UNITS;
   bool           m_bColumnsInit;//COLUMNS_SETUP;
   bool           m_bSortAsc;//sort_ascending;
   int            m_mode;
   ModuleDetail** m_moduleDetail;
   TDatabase*     m_pDatabase;
   int            m_productId;//PRODUCT_ID;
   int            m_sortIdx;//columnToSort;
   int            m_sortIdxPrevious;//previous_columnToSort;

   void SetupColumns();
   void ( __closure *OnModuleChange )(void);

public:		// User declarations
   __fastcall TModuleFrame(TComponent* Owner);
   __fastcall ~TModuleFrame();
   void load(SKeyRecord* skeyrecord, bool ALL=false);

   void modifyModulePanel(bool can_modify){m_bChangeUnits=can_modify;}
   void initialize(int mode, int productID, void( __closure *onModuleUpdate)(void)=NULL);
   bool createModulePassword(int units = -1, const bool bPasswordExt = false);
   bool createPagesPerMinutePassword();


};
//---------------------------------------------------------------------------
extern PACKAGE TModuleFrame *ModuleFrame;
//---------------------------------------------------------------------------
#endif
