//------------------------------------------------------------------------------
//  Filename:  CustomerForm.cpp
//  Class(es) :
//    TCustForm
//------------------------------------------------------------------------------

#include <vcl\vcl.h>
#pragma hdrstop

const int CREATOR_VERSION = 0x223;

#include "CLookups.h"
#include "CustomerForm.h"

//Globals
KeyMaster *keyMaster;
CLookup *lookup;

//VersionLookup* version_lookup;
//---------------------------------------------------------------------------
#pragma link "Grids"

#pragma link "ModuleDetail"
#pragma resource "*.dfm"


TCustForm *CustForm;

// sort_type -  static member of TCustForm
//==============================================================================
char* TCustForm::sort_type[] =
{
   " ORDER BY SCustomerRecord.SCRnumber",
   " ORDER BY SCustomerRecord.SCRname",
   " ORDER BY SCustomerRecord.SCRnumber",
   NULL,
};


//==============================================================================
// Fuction:     TCustForm
// Purpose:     Constructor
//==============================================================================
__fastcall TCustForm::TCustForm(TComponent* Owner)
	: TForm(Owner), attached_key(NULL)
{
   KeyGridPtr = NULL;
   QueryDialog = NULL;

   keyMaster = new KeyMaster();
   keyMaster->initDriver();

   // base query for all queries executed by the customer form
   baseSQL = "SELECT SCustomerRecord.SCRnumber, SCustomerRecord.SCRname, (select COUNT(*) FROM SKeyRecord WHERE SKeyRecord.SCRnumber = SCustomerRecord.SCRnumber ) AS Expr1 FROM SCustomerRecord";

   // array of params
   custParams[0] = "";             //number
   custParams[1] = "";             //name
   custParams[2] = "";

   bool LOGIN(false);

   try
   {
      CFdatabase->Connected = true;

      //CLookup uses Database1 connection
      lookup = new CLookup();


      //set permissions for currrent user
      SetPermissions();
      SetCustomerForm();
      ///initializeVersionLookup();

      //check version of creator
      UtilityQuery1->Close();
      UtilityQuery1->SQL->Clear();
      UtilityQuery1->SQL->Add("select * from Version");
      UtilityQuery1->Open();

      if(CREATOR_VERSION < UtilityQuery1->FieldByName("Version")->AsInteger)
      {
         Application->MessageBox("Your version of creator is out-of-date.  Please install the latest version.", "The Word of BERT", MB_OK);
      }
      else
         LOGIN = true;
   }
   catch( EDBEngineError &e )
   {
      Application->MessageBox( e.Message.c_str(), "Login Failure", MB_OK|MB_ICONERROR );
   }
   catch(...)
   {
      Application->MessageBox( "General Error.","Error", MB_OK);
   }


   //if login was not successful terminate application
   if(LOGIN == false )
   	Application->Terminate();

   //set pagecontrol size and then initialize
   AttachedKeyModuleFrame->Height = 229;
   AttachedKeyModuleFrame->Width = 392;
   //AttachedKeyModuleFrame->initialize(MODE_1, SPD_PRODUCT); // Default
}


//==============================================================================
// Fuction:     TCustForm
// Purpose:     Destructor
//==============================================================================
__fastcall TCustForm::~TCustForm()
{
  if( keyMaster )
     delete keyMaster;

  if(attached_key)
     delete attached_key;
}


//==============================================================================
// Function:    AssignSQL_CustQuery
// Purpose:     Checks if a base query has been defined, if so adds a sort
//              statement based on the sortIndex ( column ).  Then requeries to
//              update grid.
// Parameters:  TObject*
//              int
// Returns:     None
//==============================================================================
void __fastcall TCustForm::AssignSQL_CustQuery( TObject *Sender, int sortIndex )
{
   if( Cust_Query->RecordCount && !baseSQL.IsEmpty() ) // base string is built
   {
      targetSQL = baseSQL + condition + sort_type[sortIndex];
      try
      {
          Cust_Query->Close();
          Cust_Query->SQL->Clear();
   	  Cust_Query->SQL->Add( targetSQL );
   	  Cust_Query->Open();
      }
      catch( Exception* e )
      {
      	  Application->MessageBox( e->Message.c_str(), "Database Failure", MB_OK );
      }
   }
}


//==============================================================================
// Function:    CreateQueryDialog
// Purpose:     Create and display a query dialog based on which query option
//              was selected (New or Modified).  Then based on the result of the
//              dialog, either perform a query or don't.
// Parameters:  QueryByType - state (enumerated type)
// Returns:     None
//==============================================================================
void __fastcall TCustForm::CreateQueryDialog( QueryByType state )
{
   bool CANCEL = false;
   char *endptr;
   int num;

   if( state == ByNUMBER )
   {
      QueryDialog = new TQueryDialog( Owner, ByNUMBER );
      QueryDialog->EditSearch->Text = custParams[0];
      if( QueryDialog->ShowModal() == mrOk )
      {
      	  custParams[0] = QueryDialog->EditSearch->Text;
          num = strtol(QueryDialog->EditSearch->Text.c_str(), &endptr, 16);
      	  condition = " WHERE SCRnumber = ";
          condition += num;
      }
      else
          CANCEL = true;
   }
   else
   { // search by name
      QueryDialog = new TQueryDialog( Owner, ByNAME );
      QueryDialog->EditSearch->Text = custParams[1];
      if( QueryDialog->ShowModal() == mrOk )
      {
      	  custParams[1] = QueryDialog->EditSearch->Text;
      	  condition = " WHERE SCustomerRecord.SCRname LIKE '%" + custParams[1] + "%'";
      }
      else
          CANCEL = true;
   }

   if( !CANCEL )
   {
      targetSQL = baseSQL + condition + sort_type[0];//sort by number default
      Cust_Query->Close();
      Cust_Query->SQL->Clear();
      Cust_Query->SQL->Add(targetSQL);
      Cust_Query->Open();

      if( !Cust_Query->RecordCount )
      {
      	 Application->MessageBox( "No match found.", "Search", MB_OK|MB_ICONINFORMATION);
         CustomerGrid->Repaint();
      }
   }

   if( QueryDialog )
   {
   	delete QueryDialog;
        QueryDialog = NULL;
   }
}


//==============================================================================
// Function:    mmAllClick
// Purpose:     Query all customers.
// Parameters:  TObject*
// Returns:     None
//==============================================================================
void __fastcall TCustForm::mmAllClick(TObject *Sender)
{
   Cust_Query->Close();
   Cust_Query->SQL->Clear();
   condition = " "; //no condition for all
   targetSQL = baseSQL + condition + sort_type[st_by_customer_number];
   Cust_Query->SQL->Add(targetSQL);
   Cust_Query->Open();
}


//==============================================================================
// Function:    CreateCustomerKeyView()
// Purpose:     Perform a query for all keys that the customer owns based on
//              current selected customer.
// Parameters:  TObject*
// Returns:     None
//==============================================================================
void __fastcall TCustForm::CreateCustomerKeyView(TObject *Sender)
{
   int currentCustomer;
   AnsiString cName;

   if( Cust_Query->Active )
   {
      if( Cust_Query->RecordCount )
      {
         KeyGridPtr = new TFCustomerKeys(Owner, Cust_Query->FieldByName("SCRname")->AsString, Cust_Query->FieldByName("SCRnumber")->AsInteger );
         KeyGridPtr->ShowModal();
      }
   }

   if(KeyGridPtr)
   {
      delete KeyGridPtr;
      KeyGridPtr = NULL;
   }
}


//==============================================================================
// Function:    Number1Click
// Purpose:     Query customer by number.
// Parameters:  TObject*
// Returns:     None
//==============================================================================
void __fastcall TCustForm::Number1Click(TObject *Sender)
{
    CreateQueryDialog( ByNUMBER );
}


//==============================================================================
// Function:    Name1Click()
// Purpose:
// Parameters:  TObject*
// Returns:     None
//==============================================================================
void __fastcall TCustForm::Name1Click(TObject *Sender)
{
    CreateQueryDialog( ByNAME );
}


//==============================================================================
// Function:    mmInsertClick()
// Purpose:     This function is to insert a new customer into the database, and
//              is the event for main menu option "Insert".
// Parameters:  TObject*
// Returns:     None
//==============================================================================
void __fastcall TCustForm::mmInsertClick(TObject *Sender)
{
   TUpdateDlg *dlg = new TUpdateDlg( this, INSERT );
   if( dlg->ShowModal() == IDOK )// try to insert new company into database
   {
      try
      {
         TFCustomerKeys* keyform_ptr;
         UtilityQuery1->Close();
         UtilityQuery1->SQL->Clear();
         UtilityQuery1->SQL->Add("INSERT INTO SCustomerRecord ( SCRname ) VALUES (:company)");
         UtilityQuery1->ParamByName("company")->AsString = dlg->EditCompany->Text.c_str();
         UtilityQuery1->Prepare();
         UtilityQuery1->ExecSQL();

         //Requery, display the record just inserted
         Cust_Query->Close();
         Cust_Query->SQL->Clear();
         Cust_Query->SQL->Add(baseSQL + " WHERE SCustomerRecord.SCRname = '" + dlg->EditCompany->Text + "' " );
         Cust_Query->Open();
      }
      catch(...)
      {
      	Application->MessageBox("Company could not be insert into the database.", "Database Error", mbOK );
      }
   }

   if( dlg )
   {
   	delete dlg;
      dlg = NULL;
   }
}


//==============================================================================
// Function:    mmModifyClick
// Purpose:     Creates a dialog for the user to modify the company name of
//              the current row selected in the grid.
// Parameters:  TObject*
// Returns:     None
//==============================================================================
void __fastcall TCustForm::mmModifyClick(TObject *Sender)
{
   TUpdateDlg *dlg = new TUpdateDlg( this, MODIFY );
   dlg->EditCompany->Text = Cust_Query->FieldValues["SCRname"];
   int company_id = Cust_Query->FieldValues["SCRnumber"];

   if( dlg->ShowModal() == IDOK )
   {// try to insert new company into database
      try
      {
         UtilityQuery1->Close();
         UtilityQuery1->SQL->Clear();
         UtilityQuery1->SQL->Add("UPDATE SCustomerRecord SET SCRname = :new_company WHERE SCRnumber = :company_number");
         UtilityQuery1->ParamByName("new_company")->AsString = dlg->EditCompany->Text;
         UtilityQuery1->ParamByName("company_number")->AsInteger = company_id;
         UtilityQuery1->Prepare();
         UtilityQuery1->ExecSQL();

         //Requery, display the record just inserted
         Cust_Query->DisableControls();
         Cust_Query->Close();
         Cust_Query->Open();

         Cust_Query->First();
         bool FOUND(false);

         while(!Cust_Query->Eof && !FOUND)
         {
            if( Cust_Query->FieldValues["SCRnumber"] == company_id )
            	FOUND = true;
            else
            	Cust_Query->Next();
         }
         if( !FOUND )   //if not found set to start
         	Cust_Query->First();

         Cust_Query->EnableControls();
      }
      catch( Exception &e)
      {
      	Application->MessageBox(e.Message.c_str(), "Error", MB_OK );
      }
   }

   delete dlg;

}


//==============================================================================
// Function:    Deactivate1Click()
// Purpose:     Deactivate key without updating the database.  This should
//              only be used when the key does not exist or does not represent a
//              key in the DB with the same key number.
// Parameters:  TObject*
// Returns:     None
//==============================================================================
void __fastcall TCustForm::Deactivate1Click(TObject *Sender)
{
   SKeyRecord *insKeyrec = new SKeyRecord();  // key connected to the pc

   if( keyMaster->read(insKeyrec))
   {
      //try to read info from key
      Application->MessageBox("Could not deactivate the key.", "Key Message", MB_OK|MB_ICONERROR);
      if(insKeyrec)
         delete(insKeyrec);
      return;
   }
   if( 0 == insKeyrec->getCustomerNumber() )
   {
      Application->MessageBox("Key is not programmed", "Key Message", MB_OK|MB_ICONWARNING );
      if(insKeyrec)
         delete(insKeyrec);
      return;
   }
   if( ID_YES == Application->MessageBox("The database will not be updated\nto reflect the current changes do\nyou want to continue?", "Key Message", MB_YESNO|MB_ICONWARNING) )
   {

      if( keyMaster->deactivate() )
      {
         Application->MessageBox("Could not deactivate the key.", "Key Message", MB_OK|MB_ICONWARNING );
         return;
      }
      else
      {
         // key was deactivated, hide panel displaying key info
         View1->Checked = true; //set to true, because View1Click toggles and then updates
         View1Click(this); // call to update panel visiblity
      }

      if(insKeyrec)
         delete(insKeyrec);
   }
}


//==============================================================================
// Function:    Close1Click
// Purpose:     Terminates the application
// Parameters:  TObject*
// Returns:     None
//==============================================================================
void __fastcall TCustForm::Close1Click(TObject *Sender)
{
   Application->Terminate();
}


//==============================================================================
// Function:    SeekCustomer
// Purpose:     Queries customerNumber of the first attatched key.  If successful,
//              displays the new query results.  If not successful and previous
//              query was successful, return to older view.
// Parameters:  None
// Returns:     None
//==============================================================================
void __fastcall TCustForm::SeekCustomer(TObject *Sender)
{
	SKeyRecord *keyrec = new SKeyRecord();
   TBookmark lastValid;
   AnsiString prevSQL;

   if(!keyrec)
   {
      Application->MessageBox("Out of memory.", "Key Message", MB_OK|MB_ICONERROR );
   }
   else if(keyMaster->read(keyrec))
   {
      Application->MessageBox("No key!", "Key Message", MB_OK|MB_ICONERROR );
   }
   else if( 0 == keyrec->getCustomerNumber() )
   { // 0 is an invalid customer num
   	Application->MessageBox("Key is not programmed", "Key Message", MB_OK|MB_ICONWARNING );
   }
   else
   {
      Cust_Query->DisableControls();

      if( Cust_Query->Active )
      {   //check if query is active
      	if( Cust_Query->RecordCount )
         {
            //check if there is a current query
      	    prevSQL = Cust_Query->SQL->Text;
            lastValid = Cust_Query->GetBookmark();
         }
      }

      Cust_Query->Close();
      Cust_Query->SQL->Clear();
      Cust_Query->SQL->Add(baseSQL +" WHERE SCRnumber = :cNum");
      Cust_Query->ParamByName("cNum")->AsInteger = keyrec->pkey->customerNumber;
      Cust_Query->Open();

      if( Cust_Query->RecordCount )
      { // if a match was found
         if( !prevSQL.IsEmpty() )
         { // and old query was saved before seek
         	Cust_Query->FreeBookmark( lastValid ); // free bookmark
         }
         //check key
         try
         {
      	   UtilityQuery1->Close();
            UtilityQuery1->SQL->Clear();
            UtilityQuery1->SQL->Add("SELECT * FROM SKeyRecord WHERE SKRnumber = :skr_number AND SCRnumber = :scr_number");
            UtilityQuery1->ParamByName("skr_number")->AsInteger = keyrec->pkey->keyNumber;
            UtilityQuery1->ParamByName("scr_number")->AsInteger = keyrec->pkey->customerNumber;
            UtilityQuery1->Prepare();
            UtilityQuery1->Open();

            if( UtilityQuery1->RecordCount )
            {//key found
               KeyGridPtr = new TFCustomerKeys( Owner, Cust_Query->FieldValues["SCRname"], Cust_Query->FieldValues["SCRnumber"], keyrec->pkey->keyNumber);
   				KeyGridPtr->ShowModal();

               if(KeyGridPtr)
               {
                  delete KeyGridPtr;
                  KeyGridPtr = NULL;
               }
            }
            else
            { //key not found
                sprintf( scratch, "Key #%s was not found.", IntToHex(keyrec->pkey->keyNumber, 2));
            	Application->MessageBox( scratch, "Key Message", MB_OK );
            }
      	}
      	catch(...)
        {

      	}
      }
      else if( !prevSQL.IsEmpty() )
      { // no match && previous successful query
         sprintf( scratch, "Unable to locate customer #%s.", IntToHex(keyrec->pkey->customerNumber, 3));
         CustomerGrid->Repaint();
      	 Application->MessageBox(scratch, "Key Message", MB_OK|MB_ICONINFORMATION );
         Cust_Query->Close();
      	 Cust_Query->SQL->Clear();
      	 Cust_Query->SQL->Add(prevSQL);   // restore prevSQL
      	 Cust_Query->Open();
         Cust_Query->GotoBookmark( lastValid );  //place on same record
         Cust_Query->FreeBookmark( lastValid );  //free bookmark
      }
      else{ // not found, no previous query
      	 sprintf( scratch, "Unable to locate customer #%s.", IntToHex(keyrec->pkey->customerNumber, 3));
      	 Application->MessageBox(scratch, "Key Message", MB_OK|MB_ICONINFORMATION );
      }
      Cust_Query->EnableControls();
   }
   if( keyrec )
   	delete( keyrec );

}


//==============================================================================
// Function:    SetPermissions
// Purpose:     Sets a global flag.  Sets permissions for what the user
//              can or can't do in the PermissionsTable in the database.
// Parameters:  None
// Returns:     None
//==============================================================================
bool TCustForm::SetPermissions()
{
   TFCustomerKeys* keyform_ptr;
   bool result(false);

   //set permissions for currrent user
   try
   {
   	UtilityQuery1->Close();
        UtilityQuery1->SQL->Clear();
   	UtilityQuery1->SQL->Add("SELECT PermissionsTable.* FROM PermissionsTable WHERE PermissionsTable.pt_login = user_name()");
   	UtilityQuery1->Prepare();
   	UtilityQuery1->Open();

      //set to first record
      if(UtilityQuery1->RecordCount > 0)
      {
   	   UtilityQuery1->First();

   	   if( (bool)UtilityQuery1->FieldValues["pt_create_modify_key"] )
   		   keyform_ptr->PERMISSION_FLAG |= create_modify_key;
   	   if( (bool)UtilityQuery1->FieldValues["pt_extension_pwd"] )
   		   keyform_ptr->PERMISSION_FLAG |= extension_pwd;
   	   if( (bool)UtilityQuery1->FieldValues["pt_version_pwd"] )
   		   keyform_ptr->PERMISSION_FLAG |= version_pwd;
   	   if( (bool)UtilityQuery1->FieldValues["pt_module_pwd"] )
                   keyform_ptr->PERMISSION_FLAG |= module_pwd;
   	   if( (bool)UtilityQuery1->FieldValues["pt_permanent_pwd"] )
                   keyform_ptr->PERMISSION_FLAG |= permanent_pwd;

         //successfully set permissions
         result = true;
      }
   }
   catch( Exception &e )
   {
   	Application->MessageBox( e.Message.c_str(), "NONE", MB_OK );
   }
   return result;
}


//==============================================================================
// Function:    SetCustomerForm
// Purpose:     Sets a global flag.
// Parameters:  None
// Returns:     None
//==============================================================================
void __fastcall TCustForm::SetCustomerForm()
{
   TFCustomerKeys* keyform_ptr;

   if( keyform_ptr->PERMISSION_FLAG & create_modify_key )
   {
      MMCustomer->Enabled = true;
      Deactivate1->Enabled = true;
      Seek1->Enabled = true;
   }
}

//==============================================================================
// Function:    MMAboutClick
// Purpose:     Displays about dialong with creator version information.
// Parameters:  TObject*
// Returns:     None
//==============================================================================
void __fastcall TCustForm::MMAboutClick(TObject *Sender)
{
   TCreatorAboutBox* aboutbox = new TCreatorAboutBox(CREATOR_VERSION, this);
   aboutbox->ShowModal();
   delete(aboutbox);
}


//==============================================================================
// Function:    CustomerGridDrawColumnCell
// Purpose:
// Parameters:  TObject
//              TRect
//              int
//              TColumn
//              TGridDrawState
// Returns:     None
//==============================================================================
void __fastcall TCustForm::CustomerGridDrawColumnCell(TObject *Sender,
      const TRect &Rect, int DataCol, TColumn *Column,
      TGridDrawState State)
{
   int intToHex;
   if( Column->FieldName == "SCRnumber")
   {
      if( Column->Field->DisplayText.Length() != 0 )
      {
         intToHex = Column->Field->DisplayText.ToInt();
         CustomerGrid->Canvas->FillRect( Rect );
         CustomerGrid->Canvas->TextOut( Rect.Left+8, Rect.Top+2, IntToHex(intToHex, 3));
      }
   }
}

//==============================================================================
// Function:    CustomerGridDblClick
// Purpose:
// Parameters:  ( TObject* ) - Sender
// Returns:     None
//==============================================================================
void __fastcall TCustForm::CustomerGridDblClick(TObject *Sender)
{
   Sender;

   CreateCustomerKeyView(this);
}

//==============================================================================
// Function:    View1Click
// Purpose:     Opens keyview utility.
// Parameters:  ( TObject* ) - Sender
// Returns:     Nont
//==============================================================================
void __fastcall TCustForm::View1Click(TObject *Sender)
{
   Sender;

   if( View1->Checked )
   {
   	View1->Checked = false;
        PageControl1->Visible = false;
   }
   else if( LoadKeyInfo() )
   {
        View1->Checked = true;
        PageControl1->Visible = true;
   }
}

//==============================================================================
// Function:    LoadKeyInfo()
// Purpose:     Returns true if the functions successfully loads the key
//              information and false if it does not.
// Parameters:  None
// Returns:     bool
//==============================================================================
bool __fastcall TCustForm::LoadKeyInfo()
{
   bool LOADED;
   if(attached_key)
      delete attached_key;

   attached_key = new SKeyRecord();
   int product, version, license;

   if(!attached_key)
   {
      LOADED = false;
      Application->MessageBox("Out of memory.", "Key Message", MB_OK|MB_ICONERROR );
   }
   else if(keyMaster->read(attached_key))//, SPD_PRODUCT))//create key with access to module cells
   {
      LOADED = false;
      Application->MessageBox("No key!", "Key Message", MB_OK|MB_ICONERROR );
   }
   else
   {
      LOADED = true;
      //set GeneralTab
      if (attached_key->pkey->productId==SPD_PRODUCT ||
          attached_key->pkey->productId==CONNECT_PRODUCT ||
          attached_key->pkey->productId==QUANTUM_PRODUCT ||
          attached_key->pkey->productId==ICONVERT_PRODUCT)
      {
         AttachedKeyModuleFrame->Visible = true;
         AttachedKeyModuleFrame->initialize(MODE_1, attached_key->pkey->productId);
         AttachedKeyModuleFrame->load(attached_key, true);
      }
      else
      {
         AttachedKeyModuleFrame->Visible = false;
      }


      //set key summary tab
      keyLabel->Caption = Format("%03X-%03X", OPENARRAY(TVarRec,(attached_key->pkey->customerNumber, attached_key->pkey->keyNumber)));
      productLabel->Caption = attached_key->getProductText();
      versionLabel->Caption = attached_key->getVersionText();
      licenseLabel->Caption = attached_key->getStatusText();
      //outputUnits->Text = attached

      //set CellDetailTab
      int col, row;
      for( cell=0, row=0; row<7; row++ )
      {
         for(col=0; col<8; col++, cell++)
         {
            //set value
            if(hex)
               sprintf( cell_text, "0x%04X", attached_key->pkey->keyDataBlock.data[cell] );
            else
               itoa( static_cast<int>(attached_key->pkey->keyDataBlock.data[cell]), cell_text, 10 );
            KeyCellGrid->Cells[col][row] = cell_text;
         }
      }
   }
   KeyCellGridClick(this);
   return( LOADED );
}

//==============================================================================
// Function:    KeyCellGridClick()
// Purpose:
// Parameters:  ( TObject * ) - Sender
// Returns:     None
//==============================================================================
//CellZoom -
void __fastcall TCustForm::KeyCellGridClick(TObject *Sender)
{
   //
   // Passing in the attatched key so that we know which module list to choose from.
   // depending on the productId
   ModuleDetail** module_detail_list = lookup->getModuleList(attached_key->pkey->productId);

   //
   //get selected cell
   TGridRect selected = KeyCellGrid->Selection;
   cell = (selected.Top * 8) + selected.Left;
   int cellNumberValue(0);

   //
   // counter members
   char textValue[50];
   time_t* temp_time;
   char* expirationDate_text;
   struct tm *gmtime_x;
   char my_time[50];

   SpdProtectionKey* spd_key;
   SSProtectionKey* ss_key;


   //Initialize ZoomCells
   ZoomDetailTextDescription0->Visible = false;
   ZoomDetailValue0->Visible = false;
   ZoomDetailTextDescription1->Visible = false;
   ZoomDetailValue1->Visible = false;
   ZoomDetailTextDescription2->Visible = false;
   ZoomDetailValue2->Visible = false;
   ZoomDetailTextDescription3->Visible = false;
   ZoomDetailValue3->Visible = false;

   //
   // cell starts at zero
   // Pertains to all keys, including SOLsearcher keys
   if( cell < TOTAL_MODULE_CELLS ) //16
   {
      //
      // For all keys that are not SOLSEARCHER_ENTERPRISE_PRODUCT keys
      if (attached_key->pkey->productId != SOLSEARCHER_ENTERPRISE_PRODUCT)
      {
         spd_key = reinterpret_cast<SpdProtectionKey*>(attached_key->pkey);
         unsigned short base_cell = cell*4;
         //display module information
         ZoomDetailTextDescription0->Caption = module_detail_list[(base_cell)]->name;
         ZoomDetailTextDescription0->Visible = true;
         uchar test = spd_key->getLicense(base_cell);
         ZoomDetailValue0->Text = test;
         ZoomDetailValue0->Visible = true;

         ZoomDetailTextDescription1->Caption = module_detail_list[(base_cell+1)]->name;
         ZoomDetailTextDescription1->Visible = true;
         ZoomDetailValue1->Text = spd_key->getLicense(base_cell+1);
         ZoomDetailValue1->Visible = true;

         ZoomDetailTextDescription2->Caption = module_detail_list[(base_cell+2)]->name;
         ZoomDetailTextDescription2->Visible = true;
         ZoomDetailValue2->Text = spd_key->getLicense(base_cell+2);
         ZoomDetailValue2->Visible = true;

         ZoomDetailTextDescription3->Caption = module_detail_list[(base_cell+3)]->name;
         ZoomDetailTextDescription3->Visible = true;
         ZoomDetailValue3->Text = spd_key->getLicense(base_cell+3);
         ZoomDetailValue3->Visible = true;
      }
      else
      {
         //
         // SOLsearcher Key Information
         if( cell == INDEX_SERVERS_CELL )
         {
                if (attached_key->pkey->productId == SOLSEARCHER_ENTERPRISE_PRODUCT)
                {
                        SSProtectionKey* ss_key;
                        ss_key = ((SSProtectionKey*)(attached_key->pkey));

                        ZoomDetailTextDescription0->Visible = true;
                        ZoomDetailTextDescription0->Caption = "Index Servers";

                        ZoomDetailValue0->Visible = true;
                        itoa(ss_key->keyDataBlock.data[INDEX_SERVERS_CELL], cell_text, 10 );
                        ZoomDetailValue0->Text = cell_text;
                }
         }
         else if( cell == REPORT_SERVERS_CELL )
         {
                if (attached_key->pkey->productId == SOLSEARCHER_ENTERPRISE_PRODUCT)
                {
                        SSProtectionKey* ss_key;
                        ss_key = ((SSProtectionKey*)(attached_key->pkey));

                        ZoomDetailTextDescription0->Visible = true;
                        ZoomDetailTextDescription0->Caption = "Report Servers";

                        ZoomDetailValue0->Visible = true;
                        itoa(ss_key->keyDataBlock.data[REPORT_SERVERS_CELL], cell_text, 10 );
                        ZoomDetailValue0->Text = cell_text;
                }
         }
         else if( cell == APPLICATIONS_CELL )
         {
                if (attached_key->pkey->productId == SOLSEARCHER_ENTERPRISE_PRODUCT)
                {
                        SSProtectionKey* ss_key;
                        ss_key = ((SSProtectionKey*)(attached_key->pkey));

                        ZoomDetailTextDescription0->Visible = true;
                        ZoomDetailTextDescription0->Caption = "Aplication Databases";

                        ZoomDetailValue0->Visible = true;
                        itoa(ss_key->keyDataBlock.data[APPLICATIONS_CELL], cell_text, 10 );
                        ZoomDetailValue0->Text = cell_text;
                }
         }
         else if( cell == CONCURRENT_USERS_25_CELL )
         {
                if (attached_key->pkey->productId == SOLSEARCHER_ENTERPRISE_PRODUCT)
                {
                        SSProtectionKey* ss_key;
                        ss_key = ((SSProtectionKey*)(attached_key->pkey));

                        ZoomDetailTextDescription0->Visible = true;
                        ZoomDetailTextDescription0->Caption = "Concurrent Users";

                        ZoomDetailValue0->Visible = true;
                        itoa(ss_key->keyDataBlock.data[CONCURRENT_USERS_25_CELL], cell_text, 10 );
                        ZoomDetailValue0->Text = cell_text;
                }
         }
         else{}
      }  // end if (prodID != SOLSEARCHER_ENTERPRISE_PRODUCT)
   }
   else if( cell == INITIAL_COUNTER_CELL )
   {
      //set used description fields to visible
      ZoomDetailTextDescription0->Visible = true;
      ZoomDetailTextDescription0->Caption = "Initial Counter Cell";

      ZoomDetailValue0->Visible = true;

      if(!hex_view)
         cellNumberValue = strtol(KeyCellGrid->Cells[selected.Left][selected.Top].c_str(), &endptr, 16);
      else
         cellNumberValue = atoi( (KeyCellGrid->Cells[selected.Left][selected.Top]).c_str() );

      sprintf(textValue, "Days: %d, Hours: %d", cellNumberValue/24, cellNumberValue%24);
      ZoomDetailValue0->Text = textValue;
   }
   else if( cell == EXTENDED_COUNTER_CELL )
   {
      //set used description fields to visible
      ZoomDetailTextDescription0->Visible = true;
      ZoomDetailTextDescription0->Caption = "Extended Counter Cell";

      ZoomDetailValue0->Visible = true;
      if(!hex_view)
         cellNumberValue = strtol(KeyCellGrid->Cells[selected.Left][selected.Top].c_str(), &endptr, 16);
      else
         cellNumberValue = atoi( (KeyCellGrid->Cells[selected.Left][selected.Top]).c_str() );

      sprintf(textValue, "Days: %d, Hours: %d", cellNumberValue/24, cellNumberValue%24);
      ZoomDetailValue0->Text = textValue;
   }
   else if( cell == EXPIRATION_DATE_CELL)
   {
      ZoomDetailTextDescription0->Visible = true;
      ZoomDetailTextDescription0->Caption = "Expiration Date";

      ZoomDetailValue0->Visible = true;
      temp_time = (time_t*)(&(attached_key->pkey->keyDataBlock.data[EXPIRATION_DATE_CELL]));
      gmtime_x = gmtime(temp_time);
      strftime( my_time, 50, "%x", gmtime_x );
      ZoomDetailValue0->Text = my_time;
   }
   else if( cell == OUTPUT_UNIT_CELL)
   {
      ZoomDetailTextDescription0->Visible = true;
      ZoomDetailTextDescription0->Caption = "Max LU/Output Pool Value";

      ZoomDetailValue0->Visible = true;
      if(!hex_view)
         cellNumberValue = strtol(KeyCellGrid->Cells[selected.Left][selected.Top].c_str(), &endptr, 16);
      else
         cellNumberValue = atoi( (KeyCellGrid->Cells[selected.Left][selected.Top]).c_str() );

      sprintf(textValue, "%d", cellNumberValue);
      ZoomDetailValue0->Text = textValue;

   }
   else if( cell == PRODUCT_ID_CELL)
   {
      ZoomDetailTextDescription0->Visible = true;
      ZoomDetailTextDescription0->Caption = "Product Type";

      ZoomDetailValue0->Visible = true;
      ZoomDetailValue0->Text = attached_key->getProductText();
   }
   else if (cell == KEY_TYPE_CELL)
   {
      ZoomDetailTextDescription0->Visible = true;
      ZoomDetailTextDescription0->Caption = "Key Type";

      ZoomDetailValue0->Visible = true;
      ZoomDetailValue0->Text = attached_key->getKeyTypeText(attached_key->pkey->keyType);

   }
   else if( cell == PRODUCT_VERSION_CELL )
   {
      ZoomDetailTextDescription0->Visible = true;
      ZoomDetailTextDescription0->Caption = "Product Version";

      ZoomDetailValue0->Visible = true;
      ZoomDetailValue0->Text = attached_key->getVersionText();
   }
   else if( cell == KEY_VERSION_CELL )
   {
      ZoomDetailTextDescription0->Visible = true;
      ZoomDetailTextDescription0->Caption = "Key Version";

      ZoomDetailValue0->Visible = false;
      //ZoomDetailValue->Text = attached_key->getVersionText();
   }
   else if( cell == KEY_USES_CELL )
   {
      ZoomDetailTextDescription0->Visible = true;
      ZoomDetailTextDescription0->Caption = "Number of times programmed";

      ZoomDetailValue0->Visible = true;
      ZoomDetailValue0->Text = attached_key->pkey->keyDataBlock.data[KEY_USES_CELL];
   }
   else if( cell == KEY_STATUS_CELL )
   {
      ZoomDetailTextDescription0->Visible = true;
      ZoomDetailValue0->Visible = true;
      ZoomDetailTextDescription0->Caption = "Status";
      ZoomDetailValue0->Text = attached_key->getStatusText();
   }
   else if( cell == CUSTOMER_NUMBER_CELL )
   {
      ZoomDetailTextDescription0->Visible = true;
      ZoomDetailTextDescription0->Caption = "Customer Number";

      ZoomDetailValue0->Visible = true;
      itoa(attached_key->pkey->keyDataBlock.data[CUSTOMER_NUMBER_CELL], cell_text, 16 );
      ZoomDetailValue0->Text = cell_text;
   }
   else if( cell == KEY_NUMBER_CELL )
   {
      ZoomDetailTextDescription0->Visible = true;
      ZoomDetailTextDescription0->Caption = "Key Number";

      ZoomDetailValue0->Visible = true;
      itoa(attached_key->pkey->keyDataBlock.data[KEY_NUMBER_CELL], cell_text, 16 );
      ZoomDetailValue0->Text = cell_text;
   }
   /*else if( cell == MASTER_NOT_FOUND_COUNTER_CELL )
   {
      if (attached_key->pkey->productId == SOLSEARCHER_ENTERPRISE_PRODUCT)
      {
         SSProtectionKey* ss_key;
         ss_key = ((SSProtectionKey*)(attached_key->pkey));

         ZoomDetailTextDescription0->Visible = true;
         ZoomDetailTextDescription0->Caption = "Master Not Found Cell";

         ZoomDetailValue0->Visible = true;
         itoa(ss_key->keyDataBlock.data[MASTER_NOT_FOUND_COUNTER_CELL], cell_text, 16);
         ZoomDetailValue0->Text = cell_text;
      }
   }
   else if( cell == SLAVE_CELL )
   {
      if (attached_key->pkey->productId == SOLSEARCHER_ENTERPRISE_PRODUCT)
      {
         SSProtectionKey* ss_key;
         ss_key = ((SSProtectionKey*)(attached_key->pkey));

         ZoomDetailTextDescription0->Visible = true;
         ZoomDetailTextDescription0->Caption = "Master/Slave Cell";

         ZoomDetailValue0->Visible = true;
         ZoomDetailValue0->Text = (ss_key->isSlave()) ? "Slave" : "Master";
      }
   }*/
   else{}



}

//==============================================================================
// Function:    CustomerGridKeyDown
// Purpose:     Capture key commands when customer grid is in focus.
// Parameters:  TObject* Sender
//              WORD &Key
//              TShiftState Shift
// Returns:     None
//==============================================================================
void __fastcall TCustForm::CustomerGridKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   Sender;

   //if user hits <enter> while customer row selected
   if( VK_RETURN && Cust_Query->RecordCount > 0 )
   {
      CreateCustomerKeyView(this);
   }
}

//==============================================================================
// Function:    ModuleEditorClick
// Purpose:     Launches the module editor dialog which is derived off of
//              ModuleDetail.
// Parameters:  TObject* Sender
// Returns:     None
//==============================================================================
void __fastcall TCustForm::ModuleEditorClick(TObject *Sender)
{
   //
   // If "Module Editor" under Tools in the menu bar is selected
   // We will launch the Module Editor Dialog.
   if( LoadKeyInfo() )
   {
      TModuleEditorForm *modEditorForm = new TModuleEditorForm(this);
      if (modEditorForm->ShowModal() == mrOk)
      {

      }
      delete(modEditorForm);
   }
}
//---------------------------------------------------------------------------



