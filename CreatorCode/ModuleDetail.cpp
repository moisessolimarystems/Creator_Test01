//------------------------------------------------------------------------------
// Filename:    ModuleDetail.cpp
// Note:        TModuleFrame expects to have "CustDB" defined outside
//              Copyright (c) 1993-2001 by Solimar Systems, Inc.
//              All Rights Reserved.
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Kyslevel.h"
#include "ModuleDetail.h"
#include "PPMDlg.h"    //pages per minute dialog
#include "SetUnitsDlg.h"

#pragma package(smart_init)

#pragma resource "*.dfm"

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------

TModuleFrame *ModuleFrame;
//==============================================================================
// Function:    TModuleFrame
// Purpose:     Constructor
//==============================================================================
__fastcall TModuleFrame::TModuleFrame(TComponent* Owner)
   : TFrame(Owner), m_sortIdx(1),
                    m_bSortAsc(false),
                    m_sortIdxPrevious(-1),
                    m_mode(1), //just viewing
                    m_bChangeUnits(false)
{
   OnModuleChange = NULL;
}

//==============================================================================
// Function:    setupColumns
// Purpose:     Initializes the columns that will be seen depending on which mode
//              is used and what the user needs to see.
// Parameters:  int - mode
// Returns:     None
// Note:        Should be called after the ModuleFrame is resized so columns will
//              maximize space.
//==============================================================================
void TModuleFrame::SetupColumns()
{
   if (m_bColumnsInit)
       return;
   else
      m_bColumnsInit = true;

   TListColumn  *new_column;
   TListColumn  *name_column;
   int name_width(ModuleList->Width-20/*scrollbar width*/);

   //Setup fixed columns
   new_column = ModuleList->Columns->Add();
   new_column->Caption = "Action";
   new_column->MaxWidth = 100;
   new_column->MinWidth = 60;
   new_column->Width = new_column->MinWidth;
   name_width -= new_column->Width;

   name_column = ModuleList->Columns->Add();
   name_column->Caption = "Name";
   //new_column->AutoSize = true;

   if (m_mode != 4)
   {
        // concerned with details of a particular key, so we need to see what
        // people are licensed for.
        new_column = ModuleList->Columns->Add();
        new_column->Caption = "Units";
        new_column->MaxWidth = 50;
        new_column->MinWidth = 50;
        new_column->Width = new_column->MinWidth;
        name_width -= new_column->Width;
   }
   else // m_mode = 4  ; Module Editor
        // 4 is used for the module editor
   {

        m_bColumnsInit = false;
        // Here we add a column for the module id associated with each module
        new_column = ModuleList->Columns->Add();
        new_column->Caption = "PID";
        new_column->MaxWidth = 35;
        new_column->MinWidth = 25;
        new_column->Width = new_column->MaxWidth;
        name_width -= new_column->Width;

        // Here we add a column for the module id associated with each module
        new_column = ModuleList->Columns->Add();
        new_column->Caption = "ID";
        new_column->MaxWidth = 25;
        new_column->MinWidth = 25;
        new_column->Width = new_column->MaxWidth;
        name_width -= new_column->Width;

        // Module Editor is concerned with details of the module, but not
        // the number of licenses, etc. Here we add a column for the engineer
        // who worked on a particular module.
        new_column = ModuleList->Columns->Add();
        new_column->Caption = "Engineer";
        new_column->MaxWidth = 100;
        new_column->MinWidth = 50;
        new_column->Width = new_column->MaxWidth;
        name_width -= new_column->Width;


        // Here we add a column for the Part Number associated with each module
        new_column = ModuleList->Columns->Add();
        new_column->Caption = "Part Number";
        new_column->MaxWidth = 120;
        new_column->MinWidth = 50;
        new_column->Width = new_column->MaxWidth;
        name_width -= new_column->Width;

        // Here we add a column for the Version Added associated with each module
        new_column = ModuleList->Columns->Add();
        new_column->Caption = "Version Added";
        new_column->MaxWidth = 100;
        new_column->MinWidth = 50;
        new_column->Width = new_column->MaxWidth;
        name_width -= new_column->Width;

        // Here we add a column for the Version Removed associated with each module
        new_column = ModuleList->Columns->Add();
        new_column->Caption = "Version Removed";
        new_column->MaxWidth = 100;
        new_column->MinWidth = 50;
        new_column->Width = new_column->MaxWidth;
        name_width -= new_column->Width;

        // Here we add a column for the Max value associated with each module
        new_column = ModuleList->Columns->Add();
        new_column->Caption = "Max";
        new_column->MaxWidth = 50;
        new_column->MinWidth = 50;
        new_column->Width = new_column->MaxWidth;
        name_width -= new_column->Width;

        // Here we add a column displaying whether it is a Spd Default module
        new_column = ModuleList->Columns->Add();
        new_column->Caption = "SpdDefault";
        new_column->MaxWidth = 75;
        new_column->MinWidth = 50;
        new_column->Width = new_column->MaxWidth;
        name_width -= new_column->Width;

        // Here we add a column displaying whether it is Connectivity Default
        new_column = ModuleList->Columns->Add();
        new_column->Caption = "Connectivity Default";
        new_column->MaxWidth = 100;
        new_column->MinWidth = 50;
        new_column->Width = new_column->MaxWidth;
        name_width -= new_column->Width;

        // Here we add a column displaying whether it is Connectivity Module
        new_column = ModuleList->Columns->Add();
        new_column->Caption = "Connectivity Module";
        new_column->MaxWidth = 100;
        new_column->MinWidth = 50;
        new_column->Width = new_column->MaxWidth;
        name_width -= new_column->Width;
   }
   //set with of Name column base on remaining space
   name_column->Width = name_width;
}


//==============================================================================
// Function:    initialize
// Purpose:     Initializes the TModuleFrame class, which in turn depending
//              on what mode is selected, will have different options.
// Parameters:  int -  mode
//              int -  productID - identifies SPD, Connectivity, or iConvert
//              void(__closure*onModuleUpdate)(void) -
// Returns:     None
// Note:        Why is the onModuleUpdate passed in?  Is it a function pointer
//              to another function in a base class?
//==============================================================================
void TModuleFrame::initialize(int mode, int productID, void(__closure *onModuleChange)(void))
{
   //get a handle to the current database connection
   m_pDatabase = Session->FindDatabase("CustDB");

   //set forms mode
   m_mode = mode;
   m_productId = productID;
   m_moduleDetail = lookup->getModuleList(m_productId);
   OnModuleChange = onModuleChange;

   //add columns and set their width
   SetupColumns();

   // disable the module popup for the key view
   m_bPopup = false;
   ModulePopup->Items->Items[0]->Visible = false;
   ModulePopup->Items->Items[1]->Visible = false;
   
   if (m_mode==2)     // - KeyForm, general uses - //
   {
      m_bPopup = true;

      if (m_productId==SOLSCRIPT_PRODUCT)
         ModulePopup->Items->Items[1]->Visible = true;
      else
         ModulePopup->Items->Items[0]->Visible = true;
   }
   else if (m_mode == 3)  //  - KeyWizard - //
   {
      m_bChangeUnits = true;
   }
}


//==============================================================================
// Function:    load - Expects module_detail list to be initialized.
// Purpose:     loads key_record with the values of skeyrecord.
// Parameters:  SKeyRecord*  skeyrecord
//              bool - all_modules
// Returns:     None
//==============================================================================
void TModuleFrame::load(SKeyRecord* skeyrecord, bool all_modules)
{
   TListItem* new_item;
   key_record = skeyrecord;

   //lookup->getModuleList(m_productId);

   ModuleList->Items->BeginUpdate();
   ModuleList->Items->Clear();

   for( int idx(0); idx<64; idx++ )
   {
      //
      // MODULE EDITOR - Used for the Module Editor so that we can see all
      //    of the modules in the database without the checking the key.
      if (all_modules && (skeyrecord == NULL)  )
      {
          //
          // Convert the version added and removed from decimal values to
          // hexadecimal values.
          char ver_added[256];
          char ver_removed[256];
          ltoa(m_moduleDetail[idx]->version_added, ver_added, 16);
          ltoa(m_moduleDetail[idx]->version_removed, ver_removed, 16);
          for (int i = 0; i < sizeof(ver_added); i++)
                  ver_added[i] = UpCase(ver_added[i]);
          for (int i = 0; i < sizeof(ver_removed); i++)
                  ver_removed[i] = UpCase(ver_removed[i]);

          new_item = ModuleList->Items->Add();
          new_item->Caption = m_moduleDetail[idx]->getActionText();
          new_item->SubItems->Add(m_moduleDetail[idx]->name);
          new_item->SubItems->Add(m_moduleDetail[idx]->pid);
          new_item->SubItems->Add(m_moduleDetail[idx]->id);
          new_item->SubItems->Add(m_moduleDetail[idx]->engineer);
          new_item->SubItems->Add(m_moduleDetail[idx]->partnumber);
          new_item->SubItems->Add(ver_added);
          new_item->SubItems->Add(ver_removed);
          new_item->SubItems->Add(m_moduleDetail[idx]->max);
          new_item->SubItems->Add(m_moduleDetail[idx]->spd_default == true ? "1" : "0");
          new_item->SubItems->Add(m_moduleDetail[idx]->con_default == true ? "1" : "0");
          new_item->SubItems->Add(m_moduleDetail[idx]->con_module == true ? "1" : "0");
          new_item->SubItems->Add(m_moduleDetail[idx]->m_bQuantumDefault == true ? "1" : "0");
          new_item->SubItems->Add(m_moduleDetail[idx]->m_bQuantumModule == true ? "1" : "0");
          new_item->Data = static_cast<void*>(m_moduleDetail[idx]);
      } // end if statement
      else
      {
          //
          // Otherwise check the key for the licensing.
          SpdProtectionKey* spd_key((SpdProtectionKey*)(key_record->pkey));

          if ( m_moduleDetail[idx]->isAvailableForProduct(spd_key->productId) &&
               m_moduleDetail[idx]->isAvailableForVersion(spd_key->productVersion) &&
               (all_modules  || lookup->isAssignedModule(idx)) )
          {
             new_item = ModuleList->Items->Add();
             new_item->Caption = m_moduleDetail[idx]->getActionText();
             new_item->SubItems->Add(m_moduleDetail[idx]->name);
             new_item->SubItems->Add(m_moduleDetail[idx]->getTextForUnits(spd_key->productId, spd_key->getLicense(idx)));
             new_item->Data = static_cast<void*>(m_moduleDetail[idx]);
          } // end if
      }  // end else
   }  // end for loop

   ModuleList->Items->EndUpdate();
}


//==============================================================================
// Function:    ~TModuleFrame
// Purpose:     Destructor
//==============================================================================
__fastcall TModuleFrame::~TModuleFrame()
{

}

//==============================================================================
// Function:    ModuleListCompare
// Purpose:
// Parameters:  TObjec* - Sender
//              TListItem* - Item1
//              TListItem* - Item2
//              int - Data
//              int - &Compare
// Returns:     None
// Note:        everything should be sorted by name if equal
//==============================================================================
void __fastcall TModuleFrame::ModuleListCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
{
   TListItem* my_item_1;
   TListItem* my_item_2;

   if( m_bSortAsc )
   {
      my_item_1 = Item2;
      my_item_2 = Item1;
    }
   else //default is descending sort
   {
      my_item_1 = Item1;
      my_item_2 = Item2;
   }

   ModuleDetail* m_detail_1 = static_cast<ModuleDetail*>(my_item_1->Data);
   ModuleDetail* m_detail_2 = static_cast<ModuleDetail*>(my_item_2->Data);

   if (ModuleList->Column[m_sortIdx]->Caption == AnsiString("Action"))
   {
      Compare = m_detail_1->function - m_detail_2->function;
      if(Compare==0)//match
         Compare = CompareText(Item1->SubItems->Strings[0], Item2->SubItems->Strings[0]);
   }
   else if(ModuleList->Column[m_sortIdx]->Caption == AnsiString("Name"))
   {
      Compare = CompareText(my_item_1->SubItems->Strings[m_sortIdx-1], my_item_2->SubItems->Strings[m_sortIdx-1]);
   }
   else if(ModuleList->Column[m_sortIdx]->Caption == AnsiString("Units"))
   {
      try
      {
         int item_1_unit(my_item_1->SubItems->Strings[m_sortIdx-1].ToInt());
         int item_2_unit(my_item_2->SubItems->Strings[m_sortIdx-1].ToInt());
         Compare = item_1_unit - item_2_unit;
         if(Compare==0)//if match secondary sort on name... no two names should be the same
            Compare = CompareText(Item1->SubItems->Strings[0], Item2->SubItems->Strings[0]);
      }
      catch(...)
      {
         int ix = m_sortIdx - 1;
         Compare = CompareText(my_item_1->SubItems->Strings[ix], my_item_2->SubItems->Strings[ix]);
         if(Compare==0)//if match secondary sort on name... no two names should be the same
           Compare = CompareText(Item1->SubItems->Strings[0], Item2->SubItems->Strings[0]);
      }
   }
   else if( (ModuleList->Column[m_sortIdx]->Caption == AnsiString("ID")) ||
            (ModuleList->Column[m_sortIdx]->Caption == AnsiString("PID")) )
   {
        int item_1_unit(my_item_1->SubItems->Strings[m_sortIdx-1].ToInt());
        int item_2_unit(my_item_2->SubItems->Strings[m_sortIdx-1].ToInt());
        Compare = item_1_unit - item_2_unit;
        if(Compare==0)//if match secondary sort on ID... no two ID's should be the same
                Compare = CompareText(Item1->SubItems->Strings[0], Item2->SubItems->Strings[0]);
   }
   else if(ModuleList->Column[m_sortIdx]->Caption == AnsiString("Max"))
   {
        int item_1_unit(my_item_1->SubItems->Strings[m_sortIdx-1].ToInt());
        int item_2_unit(my_item_2->SubItems->Strings[m_sortIdx-1].ToInt());
        Compare = item_1_unit - item_2_unit;
        if(Compare==0)//if match secondary sort on Max...
                Compare = CompareText(Item1->SubItems->Strings[0], Item2->SubItems->Strings[0]);
   }
   else
   {
        int ix = m_sortIdx - 1;
        Compare = CompareText(my_item_1->SubItems->Strings[ix], my_item_2->SubItems->Strings[ix]);
        if(Compare==0)//if match secondary sort on name... no two names should be the same
           Compare = CompareText(Item1->SubItems->Strings[0], Item2->SubItems->Strings[0]);
   }

}

//==============================================================================
// Function:    ModuleListColumnClick
// Purpose:
// Parameters:  TObject* - Sender
//              TListColumn* - Column
// Returns:     None
//==============================================================================
void __fastcall TModuleFrame::ModuleListColumnClick(TObject *Sender,
      TListColumn *Column)
{
   //set column to sort
   m_sortIdx = Column->Index;

   //if same column being sorted as last time... then reverse the sort order
   if( m_sortIdxPrevious == m_sortIdx )
      m_bSortAsc = !m_bSortAsc;

   //reset m_sortIdxPrevious
   m_sortIdxPrevious = m_sortIdx;

   ModuleList->AlphaSort();
}

//==============================================================================
// Function:    ModuleListKeyPress
// Purpose:
// Parameters:  TObject* - Sender
//              char     - &Key
// Returns:     None
//==============================================================================
void __fastcall TModuleFrame::ModuleListKeyPress(TObject *Sender,
      char &Key)
{
   //get selected LineItem, if there is no item select return
   SpdProtectionKey* spd_key((SpdProtectionKey*)(key_record->pkey));
   TListItem* selected(ModuleList->Selected);
   if(selected==NULL)
      return;

   ModuleDetail* detail = static_cast<ModuleDetail*>(selected->Data);

   if( m_bChangeUnits )
   {
      if(Key == '+')
      {
         //check if module can be increased by one
         unsigned short license(spd_key->getLicense(detail->id));
         if( license < detail->max )
         {
            //change value for key and listview
            spd_key->setLicense(detail->id, license+1);
            selected->SubItems->Strings[1] = license+1;
            selected->Update();
         }
      }
      else if(Key == '-')
      {
         //check if module can be decreased by one
         unsigned short license(spd_key->getLicense(detail->id));
         if( license > 0 )
         {
            //change value for key and listview
            spd_key->setLicense(detail->id, license-1);
            selected->SubItems->Strings[1] = license-1;
            selected->Update();
         }
      }
   }
}

//==============================================================================
// MODE 1 - member functions
//==============================================================================

//==============================================================================
// MODE 2 - member functions
//==============================================================================

//==============================================================================
// Function:    createModulePassword(int module_id, int row_id );
// Purpose:     Creates a module password for a selected module.  This password
//              will be entered into sTransaction detail in the database, and is
//              dependent upon SDRid which is a value in the sDescriptionRecord
//              table.  The password is dependent upon the customer number which
//              will make it unique. Also the key automatically becomes a
//              permanent key when a password is created.
// Parameters:  module_id - identifies module type of password
//              row_id - NULL if legacy password, else a db key for ord_list table
// Returns:     bool
//==============================================================================
bool TModuleFrame::createModulePassword(int units, const bool bPasswordExt)
{
   char password_string[128];

   //get selected list item, from list item get pointer to ModuleDetail structure
   TListItem* selected(ModuleList->Selected);
   if(selected==NULL)
      return false;

   //check attached key status - need to have a programmed key attached
   SpdProtectionKey* spd_key((SpdProtectionKey*)(key_record->pkey));//pkey should always be SpdProtectionKey

   bool keyOnTrial = key_record->pkey->isOnTrial();


   //check if key is attached
   if( keyMaster->found() == false )
   {
      //add message, no key attached
      Application->MessageBox("No key attached.", "Information Message", MB_OK);
      return false;
   }
   //check if attached key is programmed
   if( keyMaster->programmed() == false )
   {
      //add message, attached key must be programmed to generate a password
      Application->MessageBox("Attached key must be programmed.", "Information Message", MB_OK);
      return false;
   }

   //ModuleDetail* detail = m_moduleDetail[modId];
   ModuleDetail* detail = static_cast<ModuleDetail*>(selected->Data);

   //check if module is default
   if( key_record->pkey->isOnTrial() && key_record->non_perm_ktf == false )
   {
      //if the module is a default module, then let the user know that you can't
      //create a password for a default module.
      bool not_available(detail->spd_default||detail->con_default||detail->iConvert_default);

      // Only display the message if the default module applies to the product.
      // Before, any default module would pertain to all products.
      if ( key_record->pkey->productId == SPD_PRODUCT)
          not_available = detail->spd_default;
      else if (key_record->pkey->productId == CONNECT_PRODUCT)
          not_available = detail->con_default;
      else if (key_record->pkey->productId == QUANTUM_PRODUCT)
          not_available = detail->m_bQuantumDefault;
      else if (key_record->pkey->productId == ICONVERT_PRODUCT)
          not_available = detail->iConvert_default;

      if( not_available )
      {
         Application->MessageBox("The word of Bert says you can't create a password for default modules.", "User Error", MB_ICONINFORMATION);
         return false;
      }
      else // ...otherwise create a password
      {
         // SolScript module passwords are not incremental
         if (key_record->pkey->productId != SOLSCRIPT_PRODUCT)
            units = 0;

         keyMaster->getModulePassword(spd_key,
                                      detail->id,
                                      static_cast<ProductId>(spd_key->productId),
                                      spd_key->productVersion,
                                      units,
                                      password_string
                                     );

         //applies module password to key
         keyMaster->applyModZeroPassword(key_record, detail->id, units+1);
      }

   }
   else if( spd_key->status == 2 ||  key_record->non_perm_ktf == true)//key is already permanent
   {
      if (units == -1)
         units = ((SpdProtectionKey*)(key_record->pkey))->getLicense(detail->id);
      //check if license has ability to be incremented
      int available(detail->max - units);
      if( available > 0 )
      {
         keyMaster->getModulePassword(spd_key,
                                      (uchar)detail->id,
                                      static_cast<ProductId>(spd_key->productId),
                                      spd_key->productVersion,
                                      units,
                                      password_string
                                     );
         keyMaster->applyModPassword(key_record, detail->id, units);
      }
      else
      {
         Application->MessageBox("Module already set to its maximum value.", "Module Information", MB_OK);
         return false;
      }
   }
   else //- key_status not valid, do not generate a module password
      return false;

   //execute stored procedure to update database
   try
   {
      m_pDatabase->StartTransaction();
      AnsiString updated_module_list(reinterpret_cast<char*>(spd_key->keyDataBlock.data),static_cast<unsigned char>(32));

      ModuleDetailQuery->Close();
      ModuleDetailQuery->SQL->Clear();
      if (key_record->non_perm_ktf == true)
      {
         ModuleDetailQuery->SQL->Add("Update SKeyRecord set modules = :module_list where SKRid= :skr_id");
      }
      else
      {
         ModuleDetailQuery->SQL->Add("Update SKeyRecord set SKRstatus = 2, SKRoutput = :output, modules = :module_list where SKRid= :skr_id");
         ModuleDetailQuery->ParamByName("output")->AsInteger = (spd_key->isOnTrial() == true) ? 1 : spd_key->outputUnits;
      }

      ModuleDetailQuery->ParamByName("module_list")->AsBlob = updated_module_list;
      ModuleDetailQuery->ParamByName("skr_id")->AsInteger = key_record->skr_id;
      ModuleDetailQuery->ExecSQL();

      if( keyOnTrial  && (key_record->non_perm_ktf == false) )
      {
        ModuleDetailQuery->Close();
        ModuleDetailQuery->SQL->Clear();
        ModuleDetailQuery->SQL->Add("Update SKeyRecord set SKRppmxchipds = :ipds, SKRppmxchpcl = :pcl, SKRppmxchps = :ps, SKRppmxchpsdbcs = :dbcs, SKRppmafpdsps = :afpds where SKRid= :skr_id");
        ModuleDetailQuery->ParamByName("ipds")->AsInteger = 0;
        ModuleDetailQuery->ParamByName("pcl")->AsInteger = 0;
        ModuleDetailQuery->ParamByName("ps")->AsInteger = 0;
        ModuleDetailQuery->ParamByName("dbcs")->AsInteger = 0;
        ModuleDetailQuery->ParamByName("afpds")->AsInteger = 0;
        ModuleDetailQuery->ParamByName("skr_id")->AsInteger = key_record->skr_id;
        ModuleDetailQuery->Prepare();
        ModuleDetailQuery->ExecSQL();
      }

      ModuleDetailQuery->Close();
      ModuleDetailQuery->SQL->Clear();
      ModuleDetailQuery->SQL->Add("INSERT INTO sTransactionDetail (SKRid, TDpassword, SDRid, TDunits, TDrow_id) values (:key_id, :password, :mod_id, :units, 0)");
      ModuleDetailQuery->ParamByName("key_id")->AsInteger = key_record->skr_id;
      ModuleDetailQuery->ParamByName("password")->AsString = password_string;
      ModuleDetailQuery->ParamByName("units")->AsInteger = spd_key->getLicense(detail->id);
      // ICONVERT products have an SDRid of 10XX, and therefore have to check
      // to see if the product is ICONVERT or other.
      ModuleDetailQuery->ParamByName("mod_id")->AsInteger = (key_record->pkey->productId == ICONVERT_PRODUCT) ? detail->id + 1000 : detail->id;


      ModuleDetailQuery->Prepare();
      ModuleDetailQuery->ExecSQL();

      m_pDatabase->Commit();

   }
   catch(EDBEngineError &e)
   {
      Application->MessageBox( e.Message.c_str(), "Database Failure", MB_OK );
      m_pDatabase->Rollback();
      return false;
   }

   return true;
}

//==============================================================================
// Function:    createPagesPerMinutePassword();
// Purpose:     Creates a pages per minute password for a selected module.  This password
//              will be entered into sTransaction detail in the database, and is
//              dependent upon SDRid which is a value in the sDescriptionRecord
//              table.  The password is dependent upon the customer number which
//              will make it unique. Also the key automatically becomes a
//              permanent key when a password is created.
// Parameters:  module_id - identifies module type of password
//              row_id - NULL if legacy password, else a db key for ord_list table
// Returns:     bool
//==============================================================================
bool TModuleFrame::createPagesPerMinutePassword()
{
   char password_string[128];

   //generate password by selecting a module...
   int pages(0);
   int ext(0);

   //get selected list item, from list item get pointer to ModuleDetail structure
   TListItem* selected(ModuleList->Selected);
   if(selected==NULL)
      return false;

   //check attached key status - need to have a programmed key attached
   SpdProtectionKey* spd_key((SpdProtectionKey*)(key_record->pkey));//pkey should always be SpdProtectionKey

   bool keyOnTrial = key_record->pkey->isOnTrial();
   
   //check if key is attached
   if( keyMaster->found() == false )
   {
      //add message, no key attached
      Application->MessageBox("No key attached.", "Information Message", MB_OK);
      return false;
   }
   //check if attached key is programmed
   if( keyMaster->programmed() == false )
   {
      //add message, attached key must be programmed to generate a password
      Application->MessageBox("Attached key must be programmed.", "Information Message", MB_OK);
      return false;
   }

   // Get the module detail data
   ModuleDetail* detail = static_cast<ModuleDetail*>(selected->Data);

   // Check to see if the module can have a pages per minute password
   if (!detail->pages_per_minute)
   {
        Application->MessageBox("You cannot create a Pages Per Minute password for this module.", "Information Message", MB_OK);
        return false;
   }

   // gets the most significant digit of the OutputUnits cell.0xA000
    //execute stored procedure to update database


   //
   // Get the number of extensions, increment it and make sure that we do not go over 15.
   ext = key_record->ppm_extensions;
   ext++;
   if (ext > MAX_PPM_EXTENSIONS)
   {
        Application->MessageBox("Maximum Pages Per Minute Passwords have been issued for this key.", "Warning", MB_OK|MB_ICONWARNING );
        return false;
   }
   
   // If all goes well...Launch a dialog to get the pages per minute
   TDlgPPM *dlg = new TDlgPPM( this );
   if( dlg->ShowModal() == mrCancel )
   {
        delete(dlg);
        return false;
   }
   
   pages = dlg->PagesEditBox->Text.ToInt();
   delete(dlg);

   if (pages > MAX_PAGES_PER_MINUTE)
   {
      Application->MessageBox("Invalid entry, number must \n\rrange from 1 to 500. \n\rAnything greater than 500 is unlimited", "Warning", MB_OK|MB_ICONWARNING );
      return false;
   }
   long ppmModID;

   //detail->pages_per_minute = pages;
   //also translate the mod id to the Pages per minute mod id as specified in the xml file
   switch(detail->id)
   {
        case XCH_IPDS_ID:
                key_record->xch_ipds_ppm = pages;
                ppmModID =  56;
                break;
        case XCH_PCL_ID:
                key_record->xch_pcl_ppm = pages;
                ppmModID = 58;
                break;
        case XCH_PS_ID:
                key_record->xch_ps_ppm = pages;
                ppmModID = 60;
                break;
        case XCH_PS_DBCS_ID:
                key_record->xch_ps_dbcs_ppm = pages;
                ppmModID = 64;
                break;
        case AFPDS_PS_ID:
                key_record->afpds_ps_ppm = pages;
                ppmModID = 66;
                break;
        default:
                break;
   }

   //check if module is default
   if( key_record->pkey->isOnTrial() )
   {
      //if the module is a default module, then let the user know that you can't
      //create a password for a default module or they have exceeded the number of extensions
      //allowed.
      bool not_available(detail->spd_default||detail->con_default||detail->iConvert_default||(ext > MAX_PPM_EXTENSIONS));
      if( not_available )
      {
         Application->MessageBox("You are trying to create a password for a default module OR you have have created the MAX number of passwords for this key.  15 = MAX for Pages Per Minute.", "User Error", MB_ICONINFORMATION);
         return false;
      }
      else // ...otherwise create a password
      {
         keyMaster->getPagesPerMinutePassword(spd_key, ext, pages, password_string, ppmModID);

         //applies module password to key
         keyMaster->applyPagesPerMinutePassword(key_record);
      }

   }
   else if( spd_key->status == 2 )//key is already permanent
   {
      //check if license has ability to be incremented
      int available(ext < MAX_PPM_EXTENSIONS);  
      if( available > 0 )
      {
         keyMaster->getPagesPerMinutePassword(spd_key, ext, pages, password_string, ppmModID);
         keyMaster->applyPagesPerMinutePassword(key_record);
      }
      else
      {
         Application->MessageBox("Maximum Pages Per Minute passwords have been issued.", "Module Information", MB_OK);
         return false;
      }
   }
   else //- key_status not valid, do not generate a module password
      return false;

   // update the module information but in SKeyRecord since it is actually relative
   // to the key.  The modules reflect the key information so we can't just update
   // the modules.


   // generate the last protion of the password
   int code =((ulong(detail->id)&0xFF) << 12) |  (ulong(pages)&0xFFF);

   //execute stored procedure to update database
   try
   {
      m_pDatabase->StartTransaction();
      AnsiString updated_module_list(reinterpret_cast<char*>(spd_key->keyDataBlock.data),static_cast<unsigned char>(32));

      ModuleDetailQuery->Close();
      ModuleDetailQuery->SQL->Clear();
      //ModuleDetailQuery->SQL->Add("Update SKeyRecord set SKRstatus = 2, modules = :module_list, SKRppmextensions = :extensions where SKRid= :skr_id");
      if (key_record->non_perm_ktf == true)
      {
          // This key type never goes permanent
          ModuleDetailQuery->SQL->Add("Update SKeyRecord set modules = :module_list, SKRppmextensions = :extensions where SKRid= :skr_id");
      }
      else
      {
          // If it is a keytype that can go permanent then
          // check to see if the key was a trial key....if so, then we adjust the status and output
          if (keyOnTrial)
          {
                ModuleDetailQuery->SQL->Add("Update SKeyRecord set SKRstatus = 2, SKRoutput = 1, modules = :module_list, SKRppmextensions = :extensions where SKRid= :skr_id");
          }
          // otherwise we only adjust the status. There may have been an output password given for this key
          else
          {
                ModuleDetailQuery->SQL->Add("Update SKeyRecord set SKRstatus = 2, modules = :module_list, SKRppmextensions = :extensions where SKRid= :skr_id");
          }
      }
      ModuleDetailQuery->ParamByName("extensions")->AsInteger = ext;
      ModuleDetailQuery->ParamByName("module_list")->AsBlob = updated_module_list;
      ModuleDetailQuery->ParamByName("skr_id")->AsInteger = key_record->skr_id;
      ModuleDetailQuery->Prepare();
      ModuleDetailQuery->ExecSQL();

      if( keyOnTrial && (key_record->non_perm_ktf == false) )
      {
        ModuleDetailQuery->Close();
        ModuleDetailQuery->SQL->Clear();
        ModuleDetailQuery->SQL->Add("Update SKeyRecord set SKRppmxchipds = :ipds, SKRppmxchpcl = :pcl, SKRppmxchps = :ps, SKRppmxchpsdbcs = :dbcs, SKRppmafpdsps = :afpds where SKRid= :skr_id");
        ModuleDetailQuery->ParamByName("ipds")->AsInteger = 0;
        ModuleDetailQuery->ParamByName("pcl")->AsInteger = 0;
        ModuleDetailQuery->ParamByName("ps")->AsInteger = 0;
        ModuleDetailQuery->ParamByName("dbcs")->AsInteger = 0;
        ModuleDetailQuery->ParamByName("afpds")->AsInteger = 0;
        ModuleDetailQuery->ParamByName("skr_id")->AsInteger = key_record->skr_id;
        ModuleDetailQuery->Prepare();
        ModuleDetailQuery->ExecSQL();
      }

      ModuleDetailQuery->Close();
      ModuleDetailQuery->SQL->Clear();
      switch(detail->id)
      {
        case XCH_IPDS_ID:
              ModuleDetailQuery->SQL->Add("Update SKeyRecord set SKRppmxchipds = :value where SKRid= :skr_id");
              break;
        case XCH_PCL_ID:
              ModuleDetailQuery->SQL->Add("Update SKeyRecord set SKRppmxchpcl = :value where SKRid= :skr_id");
              break;
        case XCH_PS_ID:
              ModuleDetailQuery->SQL->Add("Update SKeyRecord set SKRppmxchps = :value where SKRid= :skr_id");
              break;
        case XCH_PS_DBCS_ID:
              ModuleDetailQuery->SQL->Add("Update SKeyRecord set SKRppmxchpsdbcs = :value where SKRid= :skr_id");
              break;
        case AFPDS_PS_ID:
              ModuleDetailQuery->SQL->Add("Update SKeyRecord set SKRppmafpdsps = :value where SKRid= :skr_id");
              break;
        default:
              // should never happen because we check for the module id's before
              // this point.
              break;
      }
      ModuleDetailQuery->ParamByName("value")->AsInteger = pages;
      ModuleDetailQuery->ParamByName("skr_id")->AsInteger = key_record->skr_id;
      ModuleDetailQuery->Prepare();
      ModuleDetailQuery->ExecSQL();
      

      ModuleDetailQuery->Close();
      ModuleDetailQuery->SQL->Clear();
      ModuleDetailQuery->SQL->Add("INSERT INTO sTransactionDetail (SKRid, TDpassword, SDRid, TDunits, TDrow_id) values (:key_id, :password, :descid, :units, 0)");
      ModuleDetailQuery->ParamByName("key_id")->AsInteger = key_record->skr_id;
      ModuleDetailQuery->ParamByName("password")->AsString = password_string;
      ModuleDetailQuery->ParamByName("units")->AsInteger = pages;

      // ICONVERT products have an SDRid of 10XX, and therefore have to check
      // to see if the product is ICONVERT or other.
      ModuleDetailQuery->ParamByName("descid")->AsInteger = detail->id + 500;
      ModuleDetailQuery->Prepare();
      ModuleDetailQuery->ExecSQL();


      m_pDatabase->Commit();

   }
   catch(EDBEngineError &e)
   {
      Application->MessageBox( e.Message.c_str(), "Database Failure", MB_OK );
      m_pDatabase->Rollback();
      return false;
   }

   return true;
}



//==============================================================================
// MODE 3 - member functions
//==============================================================================

//==============================================================================
// Function:    mmIncrementModuleClick
// Purpose:     This function is an event handler that will create a password
//              for a particular module clicked.  It then refreshes the
//              Module Detail.
// Parameters:  TObject* - Sender
// Returns:     None
//==============================================================================
void __fastcall TModuleFrame::mmIncrementModuleClick(TObject *Sender)
{
   //bool result =
   createModulePassword();

   //call update function
   if(OnModuleChange)
      OnModuleChange();

   //RefreshKeyPage(0);//re-queries database for key information
   //OnKeyRowChange(this);//loads key information into SKeyRecord structure
   //RefreshKeyPage(1);   //reloads modules based on information in SKeyRecord
}

//==============================================================================
// Function:    mmViewAllClick
// Purpose:
// Parameters:  TObject* - Sender
// Returns:     None
//==============================================================================
void __fastcall TModuleFrame::mmViewAllClick(TObject *Sender)
{
   Sender;
   //toggle option
   mmViewAll->Checked = !mmViewAll->Checked;

   load(key_record, mmViewAll->Checked);
}

//==============================================================================
// Function:    PagesPerMinuteClick
// Purpose:     Generates and calls createPagesPerMinutePassword
// Parameters:  TObject *Sender
// Returns:     None
//==============================================================================
void __fastcall TModuleFrame::mmPagesPerMinuteClick(TObject *Sender)
{
   createPagesPerMinutePassword();

   //call update function
   if(OnModuleChange)
       OnModuleChange();

}

/* mmSetModuleClick()
 *----------------------------------------------------------------------------*/
void __fastcall TModuleFrame::mmSetModuleClick(TObject *Sender)
{
   TUnitsDlg *pDlg = new TUnitsDlg(NULL, 0xF);
   if (pDlg->ShowModal()==IDYES)
      createModulePassword(pDlg->GetUnits()-1, true);
   delete pDlg;

   //call update function
   if(OnModuleChange)
      OnModuleChange();
}

/* ModuleListMouseDown
 *----------------------------------------------------------------------------*/
void __fastcall TModuleFrame::ModuleListMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   TPoint pt(X,Y);
   pt = ClientToScreen(pt);
   void* pModuleData(ModuleList->Selected->Data);
   if (pModuleData) {
      ModuleDetail* detail = static_cast<ModuleDetail*>(pModuleData);
      if (m_bPopup && Button==mbRight && detail->name != "{ Not Used }")
         ModulePopup->Popup(pt.x, pt.y);
   }
}


