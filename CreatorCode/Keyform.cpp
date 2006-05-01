//==============================================================================
// Filename:    keyform.cpp
// Class(es):   TFCustomerKeys
// Note:        Copyright (c) 1993-2001 by Solimar Systems, Inc.
//              All Rights Reserved.
//==============================================================================

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include <time.h>
#include <stdio.h>
#include <dir.h>
#include <clipbrd.hpp>
#include "KeyForm.h"
#include "VersionDlg.h"
#include "ExtensionDlg.h"
#include "DevelopmentPasswordDlg.h"
#include "PasswordReportFrm.h"
#include "SetUnitsDlg.h"
#include "KeyWizard.h"
#include "keySearchDlg.h"
#include "keyDateSearchDlg.h"

#pragma link "ModuleDetail"
#pragma link "SolSearcherEnterpriseDetails"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
// keyPasswordDlg()
//    Display a password dialog box to obtain a password from the user.
//    Returns the password that the user entered.
//---------------------------------------------------------------------------
ushort keyMessageBox( const char* text,
                        const char* text2,
                        unsigned short prodver,
                        bool modal ) {return 0;}
ulong keyPasswordDlg( 	const char* text,
                      	const char* product_text,
                      	ushort& prodver,
   							ushort& units_licensed_decimal,
                        ulong& pages ) { return 0; }
//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
enum TabTypes{ tt_KEY_TAB, tt_MODULE_TAB, tt_PASSWORD_TAB};
enum StatusTypes{ st_TRIAL, st_LAST_TRIAL, st_BASE, st_DEACTIVATED_UNUSED, st_RESERVED };

TFCustomerKeys *FCustomerKeys;

unsigned int TFCustomerKeys::PERMISSION_FLAG = 0;

// sort_type - static member of TFCustomerKeys
//==============================================================================
char* TFCustomerKeys::key_sort_type[] =
{
   " ORDER BY SCustomerRecord.SCRnumber, SKeyRecord.SKRnumber",                         // SORT_CUSTOMER
   " ORDER BY SKeyRecord.SKRnumber",                                                    // SORT_KEY
   " ORDER BY SKRproduct, SCustomerRecord.SCRnumber,SKeyRecord.SKRnumber",              // SORT_PRODUCT
   " ORDER BY SKRversion, SCustomerRecord.SCRnumber,SKeyRecord.SKRnumber",              // SORT_VERSION
   " ORDER BY SKRexpirationdate, SCustomerRecord.SCRnumber,SKeyRecord.SKRnumber",       // SORT_EXPIRATION
   " ORDER BY SKRactivationdate, SCustomerRecord.SCRnumber,SKeyRecord.SKRnumber",       // SORT_ACTIVATION
   " ORDER BY SKRdeactivationdate, SCustomerRecord.SCRnumber,SKeyRecord.SKRnumber",     // SORT_DEACTIVATION
   " ORDER BY KTid, SCustomerRecord.SCRnumber,SKeyRecord.SKRnumber",                    // SORT_KEYTYPE
   " ORDER BY SKRstatus, SCustomerRecord.SCRnumber,SKeyRecord.SKRnumber",               // SORT_STATUS
   " ORDER BY SDid, SCustomerRecord.SCRnumber, SKeyRecord.SKRnumber",                   // SORT_SYSTEM
   NULL,
};


//==============================================================================
// Function:    TFCustomerKeys()
// Purpose:     Constructor
// Note:        once QueryKey is opened it should remain open, with the exception
//              of closing and immediate reopening for refreshes.  Always preform
//              these in a try catch statement, if fails display message and
//              close the form.
//==============================================================================
__fastcall TFCustomerKeys::TFCustomerKeys(TComponent* Owner, AnsiString name, int number, int searchKey )
	: TForm(Owner)
{
    //
    //code should be removed when SKeyRecord is passed to constructor
    //create instance of SKeyRecord, represents selected key
    key_record = new SKeyRecord();

    //
    // set visual properties
    PageControl1->ActivePage = TabSheetKeys;
    KeyFormModuleFrame->ModuleList->Align = alClient;

    //
    // Call resetKey function with SPD_PRODUCT
    resetKey(SPD_PRODUCT);
    resetKey(SPDE_PRODUCT);
    //
    // Set the customer Number
    key_record->pkey->customerNumber = static_cast<unsigned short>(number);

    //
    // get a handle to the current database connection
    Database1 = Session->FindDatabase("CustDB");

    bool KEYFOUND(false);

    //
    // STATUS PANELS - Set captions for status panels
    sprintf( scratch, "Customer: %s", IntToHex(key_record->pkey->customerNumber, 3));//assign here because static
    StatusBar1->Panels->Items[CUSTOMER]->Text = scratch;

    //
    // KEY NUMBER - Set the value for the key number in the form XXX-XX
    sprintf( scratch, "%s-%s", IntToHex(key_record->pkey->customerNumber, 3), IntToHex(key_record->pkey->keyNumber, 2));
    customerKeyNumber = scratch;

    //
    // CUSTOMER LABEL - Set the customer name label initially and the Caption.
    sprintf( scratch, "%s", name);
    KeyInfoCustomerNameLabel->Caption = scratch;
    Caption = name;

    try
    {
        AnsiString my_sql_statement;
        my_sql_statement = "SELECT *, CONVERT(char(12),SKRactivationDate,7)As a_date, CONVERT(char(12),SKRdeactivationDate,7) As d_date, CONVERT(char(12),SKRexpirationDate,7) As e_date  FROM SKeyRecord JOIN SCustomerRecord ON SKeyRecord.SCRnumber = SCustomerRecord.SCRnumber where SKeyRecord.SCRnumber = ";
        my_sql_statement += key_record->pkey->customerNumber;
        //
        // set private member variable with general sql statement.
        setQueryKeySQLStatement(my_sql_statement);
        //
        // set the query with ordering, but we don't want ordering in the member variable
        // in case they want to change the ordering by clicking on the title of each column.
        my_sql_statement += key_sort_type[SORT_KEY];

        QueryKey->Close();
        QueryKey->SQL->Clear();
        QueryKey->SQL->Add(my_sql_statement);

        QueryKey->Open();

        if( searchKey != -1 )
        {
            //
            // search key is valid, need to search for key
   	    QueryKey->DisableControls();
            QueryKey->First();
            while(!QueryKey->Eof && !KEYFOUND)
            {
                if( QueryKey->FieldValues["SKRnumber"] == searchKey )
                {
            	    KEYFOUND = true;
                }
                else
         	    QueryKey->Next();
            }
            //
            // if could not find key, error
            if( !KEYFOUND )
                throw;
            QueryKey->EnableControls();
        }
    }
    catch( Exception &e )
    {
        ModalResult = IDNO; // could not open form
        Application->MessageBox( e.Message.c_str(), "Database Failure", MB_OK );
    }

    //
    // Update the information
    OnKeyRowChange(this);
}

//==============================================================================
// Function:    resetKey
// Purpose:     calls keyMaster->resetKey() which will return a new key with
//              the appropriate key type.
// Parameters:  int - product id.
// Returns:     AnsiString - sql_statement
//==============================================================================
void __fastcall TFCustomerKeys::resetKey(ProductId pid)
{
    keyMaster->resetKey(key_record, pid);
}
//==============================================================================
// Function:    getQueryKeySQLStatement
// Purpose:     Returns the member variable "sql_statement" query that needs to
//              be performed on the keys WITHOUT ordering.
// Parameters:  None
// Returns:     AnsiString - sql_statement
//==============================================================================
AnsiString TFCustomerKeys::getQueryKeySQLStatement()
{
    return sql_statement;
}

//==============================================================================
// Function:    setQueryKeySQLStatement
// Purpose:     Sets the member variable sql_statement used for queries in the
//              database
// Parameters:  AnsiString new_statement
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::setQueryKeySQLStatement(AnsiString new_statement)
{
    sql_statement = new_statement;
}

//==============================================================================
// Function:    setGUIOptions
// Purpose:     Set options that will not change once the key form is open.
//              what is the difference between the PHYSICAL_FLAG and
//              PERMISSION_FLAG??
// Parameters:
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::setGUIOptions()
{
    // set the Program, Remove, Deactivate
    if( (PERMISSION_FLAG & create_modify_key) )
    {
        if(PHYSICAL_FLAG & pf_MODIFY_STATUS)
        {
            mmLost->Enabled = true;
            mmReturned->Enabled = true;
        }
        if(PHYSICAL_FLAG & pf_PROGRAM)
      	    mmProgram->Enabled = true;
        if(PHYSICAL_FLAG & pf_REPROGRAM)
      	    mmReprogram->Enabled = true;
        if(PHYSICAL_FLAG & pf_REMOVE)
      	    mmRemove->Enabled = true;
        if(PHYSICAL_FLAG & pf_DEACTIVATE)
      	    mmDeactivate->Enabled = true;
     }

     //
     // set Extension
     if( (PERMISSION_FLAG & extension_pwd) && (PHYSICAL_FLAG & pf_EXTENSION) )
     {
         mmExtension->Enabled = true;
     }

     //
     // set Permanent
     if( (PERMISSION_FLAG & permanent_pwd) && (PHYSICAL_FLAG & pf_PERMANENT) )
     {
         mmPermanent->Enabled = true;
     }

     //
     // set Module
     bool bool1(PHYSICAL_FLAG & pf_MODULE);
     bool bool2(PERMISSION_FLAG & permanent_pwd);
     bool bool3(PHYSICAL_FLAG & pf_NONPERMANENT);
     if ( bool1 && (bool2 || bool3)) {
     //if ((PHYSICAL_FLAG & pf_MODULE) && (PERMISSION_FLAG & permanent_pwd || PHYSICAL_FLAG & pf_NONPERMANENT)) {
     //if( ( (PERMISSION_FLAG & permanent_pwd) && (PHYSICAL_FLAG & pf_MODULE) ) || (PHYSICAL_FLAG & pf_NONPERMANENT) )
     //{
         mmSPDOutput->Visible = ((key_record->getProductId() != ICONVERT_PRODUCT)     &&
                                 (key_record->getProductId() != RUBIKA_PRODUCT)          &&
                                 (key_record->getProductId() != SOLSCRIPT_PRODUCT))
                                ? true : false;
         mmSPDEOperatorSessions->Visible = (key_record->getProductId() == SPDE_PRODUCT) ? true : false;
         mmSPDEUserSessions->Visible = (key_record->getProductId() == SPDE_PRODUCT) ? true : false;
         mmMaxLU->Visible = (key_record->getProductId() == ICONVERT_PRODUCT) ? true : false;
         KeyFormModuleFrame->mmPagesPerMinute->Enabled = true;
         KeyFormModuleFrame->mmIncrementModule->Enabled = true;
     }

     // set Version
     if( (PERMISSION_FLAG & version_pwd) && (PHYSICAL_FLAG & pf_VERSION) )
     {
         mmVersion->Enabled = true;
     }

     // set exceptions for Add-On and Backup Keys - Special key types
     if( (key_record->pkey->keyType == KEYAddon) || (key_record->pkey->keyType == KEYBackup) )
     {
         //
         // For "ANY" Addon or Backup key, the pages per minute functionality should not be enabled.
         // This applies to Permanent Key types and NON-Permanent Key types
         KeyFormModuleFrame->mmPagesPerMinute->Enabled = false;

         //
         // If not the non permanent key type
         if (key_record->non_perm_ktf == false)
         {
             //then we should disable any functionality that could make the key go permanent.
             mmSPDOutput->Visible = false;
             mmSPDEOperatorSessions->Visible = false;
             mmSPDEUserSessions->Visible = false;
             mmMaxLU->Visible = false;
             KeyFormModuleFrame->mmIncrementModule->Enabled = false;
             mmPermanent->Enabled = false;
             // NOTE: the user can still make the key a BASE key, and the key
             // from that point on can go permanent.
         }
     }

     //
     // set SSKey Permissions
     if ( (PERMISSION_FLAG & permanent_pwd) && (key_record->pkey->productId == SOLSEARCHER_ENTERPRISE_PRODUCT))//(PHYSICAL_FLAG & pf_SSKEY_MASTER) )
     {
        //
        // Allow Master keys to generate these passwords

        mmPermanent->Enabled = true;
       // mmExtension->Enabled = true;

        //
        // Disallow Master keys to generate these passwords
        mmMaxLU->Visible = false;
        mmSPDOutput->Visible = false;
        mmSPDEOperatorSessions->Visible = false;
        mmSPDEUserSessions->Visible = false;

        //
        // Enable ability to mark the key LOST or RETURNED
        mmLost->Enabled = true;
        mmReturned->Enabled = true;
     }

     if ((PERMISSION_FLAG & permanent_pwd) &&
         (key_record->pkey->productId == SOLSCRIPT_PRODUCT ||
         /*key_record->pkey->productId == SDX_DESIGNER_PRODUCT ||*/
         key_record->pkey->productId == RUBIKA_PRODUCT)
         )
     {
        KeyFormModuleFrame->mmViewAll->Enabled = false;
        KeyFormModuleFrame->mmPagesPerMinute->Enabled = false;
     }
}

//==============================================================================
// Function:    showPagesPerMinuteInfo()
// Purpose:     Allows the user to view or not view the information
// Parameters:  BOOL - true - allows the user to see values
//                     false - hides the information from the user.
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::showPagesPerMinuteInfo(bool bTurnOn)
{
    //
    // Group Box
    PPMGroupBox->Visible = bTurnOn;
    //
    // Labels
    xch_ipds_ppm_label->Visible = bTurnOn;
    xch_pcl_ppm_label->Visible = bTurnOn;
    xch_ps_ppm_label->Visible = bTurnOn;
    xch_ps_dbcs_ppm_label->Visible = bTurnOn;
    afpds_ps_ppm_label->Visible = bTurnOn;
    //
    // Edit Boxes
    xch_ipds_ppmEdit->Visible = bTurnOn;
    xch_pcl_ppmEdit->Visible = bTurnOn;
    xch_ps_ppmEdit->Visible = bTurnOn;
    xch_ps_dbcs_ppmEdit->Visible = bTurnOn;
    afpds_ps_ppmEdit->Visible = bTurnOn;
}

//==============================================================================
// Function:    showLicenseInfo()
// Purpose:     Allows the user to view or not view the information
// Parameters:  BOOL - true - allows the user to see values
//                     false - hides the information from the user.
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::showLicenseInfo(bool bTurnOn)
{
    //
    // Group Box
    Licensing->Visible = bTurnOn;

    //
    // Labels
    StatusLabel->Visible = bTurnOn;
    OutputDescription->Visible = bTurnOn;
    PoolLabel->Visible = bTurnOn;

    //
    // Edit Boxes
    output_devices->Visible = bTurnOn;
}

//==============================================================================
// Function:    showSessionsInfo()
// Purpose:     Allows the user to view or not view the information
// Parameters:  BOOL - true - allows the user to see values
//                     false - hides the information from the user.
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::showSessionsInfo(bool bTurnOn)
{
    //
    // Group Box
    SessionGroupBox->Visible = bTurnOn;

}

//==============================================================================
// Function:    showModuleInfo()
// Purpose:     Allows the user to view or not view the information
// Parameters:  BOOL - true - allows the user to see values
//                     false - hides the information from the user.
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::showModuleInfo(bool bTurnOn)
{
    KeyFormModuleFrame->Visible = bTurnOn;
}

//==============================================================================
// Function:    OnKeyRowChange
// Purpose:     This function is called everytime the value in DBEdit1 changes,
//              because the datafield is set to the primary key of the dataSource
//              this function gets called everytime the row changes.
// Parameters:  TObject*
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::OnKeyRowChange(TObject *Sender)
{
    void* buffer;
    //set new key value
    try
    {
        // if the Query is active, try to access it, else do nothing.
        if(QueryKey->Active)
        {
            if(QueryKey->RecordCount)
            {
                //
                // Database ONLY members
                key_record->customerName = QueryKey->FieldByName("SCRname")->AsString;
                key_record->skr_id = QueryKey->FieldByName("SKRid")->AsInteger;
                key_record->system_id = QueryKey->FieldByName("SDid")->AsInteger;
                key_record->po = QueryKey->FieldByName("SKRpo")->AsInteger;
                key_record->eBOnumber = QueryKey->FieldByName("SKReBOnumber")->AsString;
                key_record->summary = QueryKey->FieldByName("SKRdescription")->AsString;
                key_record->key_class = QueryKey->FieldByName("SKRkeyClass")->AsInteger;
                key_record->non_perm_ktf = QueryKey->FieldByName("SKRnonpermktf")->AsBoolean;
                key_record->ppm_extensions = QueryKey->FieldByName("SKRppmextensions")->AsInteger;
                key_record->xch_ps_ppm = QueryKey->FieldByName("SKRppmxchps")->AsInteger;
                key_record->xch_ps_dbcs_ppm = QueryKey->FieldByName("SKRppmxchpsdbcs")->AsInteger;
                key_record->xch_ipds_ppm = QueryKey->FieldByName("SKRppmxchipds")->AsInteger;
                key_record->xch_pcl_ppm = QueryKey->FieldByName("SKRppmxchpcl")->AsInteger;
                key_record->afpds_ps_ppm = QueryKey->FieldByName("SKRppmafpdsps")->AsInteger;

                //
                // ProtectionKey members
                key_record->pkey->keyNumber = QueryKey->FieldByName("SKRnumber")->AsInteger;
                key_record->pkey->customerNumber = QueryKey->FieldByName("SCRnumber")->AsInteger;
                key_record->pkey->productId = QueryKey->FieldByName("SKRproduct")->AsInteger;
                key_record->pkey->productVersion = QueryKey->FieldByName("SKRversion")->AsInteger;
                key_record->pkey->status = QueryKey->FieldByName("SKRstatus")->AsInteger;
                key_record->pkey->keyType = key_record->getKeyTypeEnumValue(QueryKey->FieldByName("KTid")->AsInteger);

                //
                // Initialize the List of Modules for each key.
//                KeyFormModuleFrame->initialize(MODE_2, key_record->pkey->productId, modulePasswordCreated);

                //
                // SpdProtectionKey members
                if( key_record->pkey->productId==SPD_PRODUCT ||
                    key_record->pkey->productId==CONNECT_PRODUCT ||
                    key_record->pkey->productId==QUANTUM_PRODUCT ||
                    key_record->pkey->productId==ICONVERT_PRODUCT  )
                {
                        //
                        // This is important to go between SP/D Keys & SSKeys....DO NOT REMOVE THIS
                        ProtectionKey* temp_key = ProtectionKey::newKey(key_record->pkey);
                        key_record->pkey = temp_key; // Update the key_record with the new pkey
                        temp_key = NULL;             // Clean up
                        delete temp_key;

                        memset(tmp_modules, 0, sizeof(tmp_modules));   //why???
                        QueryKey->Fields->FieldByName("modules")->GetData(tmp_modules, true);
                        ((SpdProtectionKey*)(key_record->pkey))->setLicenses(tmp_modules);
                        ((SpdProtectionKey*)(key_record->pkey))->outputUnits = QueryKey->FieldByName("SKRoutput")->AsInteger;

                        // initialize module tab and make it visible
                        KeyFormModuleFrame->initialize(MODE_2, key_record->pkey->productId, modulePasswordCreated);
                        PageControl1->Pages[1]->TabVisible = true;
                        PageControl1->Pages[2]->TabVisible = false;
                }
                else if (key_record->pkey->productId==SOLSCRIPT_PRODUCT ||
                         /*key_record->pkey->productId == SDX_DESIGNER_PRODUCT ||*/
                         key_record->pkey->productId == RUBIKA_PRODUCT) {
                        //
                        // This is important to go between SP/D Keys & SSKeys....DO NOT REMOVE THIS
                        ProtectionKey* temp_key = ProtectionKey::newKey(key_record->pkey);
                        key_record->pkey = temp_key; // Update the key_record with the new pkey
                        temp_key = NULL;             // Clean up
                        delete temp_key;

                        memset(tmp_modules, 0, sizeof(tmp_modules));       //
                        QueryKey->Fields->FieldByName("modules")->GetData(tmp_modules, true);
                        ((SpdProtectionKey*)(key_record->pkey))->setLicenses(tmp_modules);

                        // initialize module tab and make it visible
                        KeyFormModuleFrame->initialize(MODE_2, key_record->pkey->productId, modulePasswordCreated);
                        PageControl1->Pages[1]->TabVisible = true;
                        PageControl1->Pages[2]->TabVisible = false;
                }
                //
                // SSProtectionKey members
                else if( key_record->pkey->productId==SOLSEARCHER_ENTERPRISE_PRODUCT)
                {
                        //
                        // This is important to go between SP/D Keys & SSKeys....DO NOT REMOVE THIS
                        ProtectionKey* temp_key = ProtectionKey::newKey(key_record->pkey);
                        key_record->pkey = temp_key; // Update the key_record with the new pkey
                        temp_key = NULL;             // Clean up
                        delete temp_key;
                                        //
                        // Now we can safely access the SSKey Members
                        ((SSProtectionKey*)(key_record->pkey))->setIndexServers(QueryKey->FieldByName("SKRindexServers")->AsInteger);
                        ((SSProtectionKey*)(key_record->pkey))->setReportServers(QueryKey->FieldByName("SKRreportServers")->AsInteger);
                        ((SSProtectionKey*)(key_record->pkey))->setConcurrentUsers(QueryKey->FieldByName("SKRconcurrentUsers")->AsInteger);
                        ((SSProtectionKey*)(key_record->pkey))->setApplications(QueryKey->FieldByName("SKRapplications")->AsInteger);
                        ((SSProtectionKey*)(key_record->pkey))->setDocumentAssembler(QueryKey->FieldByName("SKRdocumentAssembler")->AsInteger);
                        // initialize module tab and make it visible
                        SolSearcherDetails->Initialize(2, TabDataOnChange);//keyform mode
                        SolSearcherDetails->Load(key_record);
                        PageControl1->Pages[1]->TabVisible = false;
                        PageControl1->Pages[2]->TabVisible = true;
                }
                else if( key_record->pkey->productId==SPDE_PRODUCT)
                {
                        //
                        // This is important to go between SP/D Keys & SSKeys....DO NOT REMOVE THIS
                        ProtectionKey* temp_key = ProtectionKey::newKey(key_record->pkey);
                        key_record->pkey = temp_key; // Update the key_record with the new pkey
                        temp_key = NULL;             // Clean up
                        delete temp_key;
                        buffer = malloc(QueryKey->Fields->FieldByName("modules")->DataSize);
                                     //spde needs
                        //memset(tmp_spde_modules, 0, sizeof(tmp_spde_modules));  //array of 16 shorts = 32 bytes
                        QueryKey->Fields->FieldByName("modules")->GetData(buffer, true);  // gives me 32 bytes of binary modules
                       ((SpdeProtectionKey*)(key_record->pkey))->setLicenses((unsigned short*)buffer, false);   //need to process 32 bytes of binary modules

                        ((SpdeProtectionKey*)(key_record->pkey))->outputUnits = QueryKey->FieldByName("SKRoutput")->AsInteger;
                        ((SpdeProtectionKey*)(key_record->pkey))->operatorSessionUnits = QueryKey->FieldByName("SKRoperatorSession")->AsInteger;
                        ((SpdeProtectionKey*)(key_record->pkey))->userSessionUnits = QueryKey->FieldByName("SKRuserSession")->AsInteger;
                        // initialize module tab and make it visible
                        KeyFormModuleFrame->initialize(MODE_2, key_record->pkey->productId, modulePasswordCreated);
                        PageControl1->Pages[1]->TabVisible = true;
                        PageControl1->Pages[2]->TabVisible = false;
                        free(buffer);
                }
                else
                {
                  //
                  // This is important to go between SP/D Keys & SSKeys....DO NOT REMOVE THIS
                  ProtectionKey* temp_key = ProtectionKey::newKey(key_record->pkey);
                  key_record->pkey = temp_key; // Update the key_record with the new pkey
                  temp_key = NULL;             // Clean up
                  delete temp_key;

                  PageControl1->Pages[1]->TabVisible = false;
                  PageControl1->Pages[2]->TabVisible = false;
                }
            }
            else //clear key members
            {
                key_record->customerName = 0;
                key_record->skr_id = 0;
                key_record->system_id = 0;
                key_record->po = 0;
                key_record->eBOnumber = 0;
                key_record->summary = 0;
                key_record->key_class = 0;
                key_record->non_perm_ktf = 0;

                key_record->ppm_extensions = 0;
                key_record->xch_ps_ppm = 0;
                key_record->xch_ps_dbcs_ppm = 0;
                key_record->xch_ipds_ppm = 0;
                key_record->xch_pcl_ppm = 0;
                key_record->afpds_ps_ppm = 0;
            }

            //
            //reset SQL statements and mark for refresh
            resetSqlStatements();
            dirty_tab = all_tabs&~key_tab;

            //
            //reset other gui items
            clearPermissions();
            resetPhysicalFlag();
            setGUIOptions();
            setStatusValues();

            //
            //set description,  pk number, and eBO number values
            setKeyInfoValues();

        } // if Query->Active

    }
    catch(...)
    {
        //display message and close form
    }
}


//==============================================================================
// Function:    setKeyInfoValues()
// Purpose:     Sets the values in the purchase order edit box, and description
//              memo boxes.  Both are read-only to prevent errors.  The user has
//              to dbl click on the edit boxes in order to make changes to them.
// Parameters:  None
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::setKeyInfoValues()
{
    //
    // CUSTOMER NAME - Set the customer name
    KeyInfoCustomerNameLabel->Caption = key_record->customerName;
    Caption = key_record->customerName;

    //
    // KEY NUMBER - set key number
    sprintf( scratch, "%s-%s", IntToHex(key_record->pkey->customerNumber, 3), IntToHex(key_record->pkey->keyNumber, 2));
    customerKeyNumber = scratch;

    //
    // PKNUMBER - update the edit box with the pk number currently in the database
    sprintf( scratch, "%d", key_record->po);
    PKNumberEditBox->Text =  scratch;

    //
    // SUMMARY - update the description with the description currently in the database
    sprintf( scratch, "%s", key_record->summary);
    DescriptionMemo->Text = scratch;

    //
    // eBONUMBER - update the epicore Back Office number w/ eBO in the database
    UpdateeBONumber->Enabled = false;
    sprintf( scratch, "%s", key_record->eBOnumber);
    eBONumberEditBox->Text = scratch;

    //
    // KEY CLASS TYPE - Parallel or USB
    keyClassLabel->Caption = (key_record->key_class == 0) ? "Parallel Key" : "USB Key";

    //
    // NON-PERM KEY TYPE - update difference bet old keys & new keys for key types: Addon, WarmBackup, & Test/Dev
    keyTypeLabel->Caption = (key_record->non_perm_ktf ==0) ? "Permanent Key Type" : "NON-Permanent Key Type";

    //
    // PAGES PER MINUTE - update the Pages Per Minute Fields
    xch_ipds_ppmEdit->Text = key_record->xch_ipds_ppm;
    xch_ps_ppmEdit->Text = key_record->xch_ps_ppm;
    xch_ps_dbcs_ppmEdit->Text = key_record->xch_ps_dbcs_ppm;
    xch_pcl_ppmEdit->Text = key_record->xch_pcl_ppm;
    afpds_ps_ppmEdit->Text = key_record->afpds_ps_ppm;

    if ( (key_record->pkey->productId == SPD_PRODUCT) ||
         (key_record->pkey->productId == CONNECT_PRODUCT) ||
         (key_record->pkey->productId == QUANTUM_PRODUCT) ||
         (key_record->pkey->productId == ICONVERT_PRODUCT)  )
    {
        showPagesPerMinuteInfo(true);
        showLicenseInfo(true);
        showModuleInfo(true);
        showSessionsInfo(false);

        //
        // LICENSING - refresh licensing.
        setOutputUnitsDisplay(static_cast<SpdProtectionKey*>(key_record->pkey)->outputUnits);
        PoolLabel->Caption = (key_record->getProductId() == ICONVERT_PRODUCT) ? "Max LU's Licensed:" : "Pool:";
    }
    else if (key_record->pkey->productId == SPDE_PRODUCT)
    {
        showPagesPerMinuteInfo(true);
        showLicenseInfo(true);
        showModuleInfo(true);
        showSessionsInfo(true);

        //
        // LICENSING - refresh licensing.
        setOutputUnitsDisplay(static_cast<SpdeProtectionKey*>(key_record->pkey)->outputUnits);
        setOperatorSessionUnitsDisplay(static_cast<SpdeProtectionKey*>(key_record->pkey)->operatorSessionUnits);
        setUserSessionUnitsDisplay(static_cast<SpdeProtectionKey*>(key_record->pkey)->userSessionUnits);
    }
    else if (key_record->pkey->productId == SOLSCRIPT_PRODUCT ||
             /*key_record->pkey->productId == SDX_DESIGNER_PRODUCT ||*/
             key_record->pkey->productId == RUBIKA_PRODUCT) {
        showPagesPerMinuteInfo(false);
        showLicenseInfo(false);
        showSessionsInfo(false);
        showModuleInfo(true);
        //setOutputUnitsDisplay(static_cast<SpdProtectionKey*>(key_record->pkey)->outputUnits);
        //PoolLabel->Caption = (key_record->getProductId() == ICONVERT_PRODUCT) ? "Max LU's Licensed:" : "Pool:";
    }
    else
    {
        //
        // In addition, for keys not of SP/D or Connectivity types, there is no need to see
        // The licensing dialog or Module information.
        showPagesPerMinuteInfo(false);
        showLicenseInfo(false);
        showModuleInfo(false);
        showSessionsInfo(false);
    }

    //
    // PKNUMBER, EBONUMBER, & DESCRIPTION - set permissions to false until changed
    UpdatePKNumber->Enabled = false;
    UpdateeBONumber->Enabled = false;
    UpdateDescription->Enabled = false;


}

//==============================================================================
// Function:    resetSqlStatements()
// Purpose:     Calls LoadPswdGrid() function.
// Parameters:  None
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::resetSqlStatements()
{
   LoadPswdGrid();
}

//==============================================================================
// Function:    isAttachedKeyReady()
// Purpose:     Check that key is attached and programmed, so that creator can
//              generate password. Display a error message and return false if
//              there is a problem.
// Parameters:  None
// Returns:     None
//==============================================================================
bool TFCustomerKeys::isAttachedKeyReady()
{
   //check if key is attached
   if( keyMaster->found() == false )
   {
      //add message, no key attached
      Application->MessageBox("No key attached to pc.", "Information", MB_OK);
      return false;
   }

   //check if attached key is programmed
   if( keyMaster->programmed() == false )
   {
      //add message, attached key must be programmed to generate a password
      Application->MessageBox("Attached key is not programmed.", "Information", MB_OK);
      return false;
   }

   return true;
}

//==============================================================================
// Function:    resetPhysicalFlag
// Purpose:     Set flag to indicate which options are available to the user
//              based on physical factors, such as; A key exist. Which tab is
//              selected. Status of key highlighed.  Also set visiblity of tab
//              if no key exist.
// Parameters:  None
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::resetPhysicalFlag()
{
   //reset physical flag to not options available
   PHYSICAL_FLAG = 0; //can always reserve a new key number
   //Get selected tab
   TabTypes tab(static_cast<TabTypes>(PageControl1->ActivePage->PageIndex));

   //possible options for key tab
   if( tab == tt_KEY_TAB )
   {
      //can always reserve a key if key tab selectd
      PHYSICAL_FLAG |= pf_RESERVE;

      //set actions that are only avaiable with a key row selected
      if(QueryKey->RecordCount )
      {
         //what is avaliable when key tab is selected and the is a key row
         PHYSICAL_FLAG |= pf_REMOVE;

         //can only program a reserved key
         if (key_record->getKeyTypeIntegerValue(key_record->pkey->keyType) == ktfReserved)
            PHYSICAL_FLAG |= pf_PROGRAM;
         else
         {
            //set base on key status and type
            int key_status(key_record->pkey->status);

            //if key is not deactivated, unsed, returned, or lost
            if( key_status < 8 )//&& key_status != 2)
            {
               //available for all non-dead keys.... some special cases
               PHYSICAL_FLAG |= pf_MODIFY_STATUS;
               PHYSICAL_FLAG |= pf_DEACTIVATE;
               PHYSICAL_FLAG |= pf_VERSION;

               //backup keys do not allow version passwords
               if( key_record->getKeyTypeIntegerValue(key_record->pkey->keyType) == ktfBackup )
                  PHYSICAL_FLAG &= ~pf_VERSION;

               if( key_record->non_perm_ktf == true)
                  PHYSICAL_FLAG |= pf_NONPERMANENT;

               //key is trial
               if( key_status != 2 )
               {
                  //only generate permanent password for base keys
                  if( key_record->getKeyTypeIntegerValue(key_record->pkey->keyType) == ktfBase )
                     PHYSICAL_FLAG |= PHYSICAL_FLAG |= pf_PERMANENT;

                  //if key is on last extension can not generate another extension password
                  if( key_status != 7 )
                     PHYSICAL_FLAG |= pf_EXTENSION;
               }// end key_status != 2
            } // end key_status < 8
         } // end else
      } // end QueryKey->RecordCount
   }  // end if (tab == tt_KEY_TAB)
   //module tab is selected
   else if( tab == tt_MODULE_TAB )
   {
      int key_status;
      //set base on key status and type
      if(QueryKey->RecordCount)
      {
         key_status = key_record->pkey->status;

         //if key is base and not deactived
         if( (key_record->getKeyTypeIntegerValue(key_record->pkey->keyType) == ktfBase && key_status < 8 ) || (key_record->non_perm_ktf == true) )
         {
            PHYSICAL_FLAG |= pf_MODULE;
         }
      } // end QueryKey->RecordCount
   } // end else if
}


//==============================================================================
// Function:    clearPermissions
// Purpose:     Sets all user interface options to false.  Should be called
//              before
// Parameters:  None
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::clearPermissions()
{
   //set all values to false
   mmReprogram->Enabled = false;
   mmProgram->Enabled = false;
   mmRemove->Enabled = false;
   mmDeactivate->Enabled = false;
   mmPermanent->Enabled = false;
   mmExtension->Enabled = false;
   mmVersion->Enabled = false;
   mmSPDOutput->Visible = false;
   mmSPDEOperatorSessions->Visible = false;
   mmSPDEUserSessions->Visible = false;
   mmLost->Enabled = false;
   mmReturned->Enabled = false;
   mmMaxLU->Visible = false;

   //
   // Module Features
   KeyFormModuleFrame->mmPagesPerMinute->Enabled = false;
   KeyFormModuleFrame->mmIncrementModule->Enabled = false;

}


//==============================================================================
// Function:    OnTabChange
// Purpose:     Called everytime a new tab is selected.  Set permissions based
//              on tab selected and recall set GUI functions.
// Parameters:  TObject*
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::OnTabChange(TObject*)
{

   TTabSheet *activeTab = PageControl1->ActivePage;

   switch( activeTab->PageIndex )
   {
      //check if tab becoming active has old information - if so refresh data
   	case 0: //key page
      	if( dirty_tab & key_tab )
         {  //refresh key tab
            RefreshKeyPage();         //refreshTab()
            dirty_tab &~module_tab;
         }
      	break;
      case 1: //module page
      	if( dirty_tab & module_tab )
         {  //refresh module tab
            RefreshKeyPage();         //refreshTab()
            dirty_tab &~module_tab;
         }
        	break;
      case 2: //solsearcher enterprise detail tab
      	if( dirty_tab & solsearcher_tab )
         {  //refresh details tab
            RefreshKeyPage();         //refreshTab()
            dirty_tab &~solsearcher_tab;
         }
        	break;
      case 3: //password history
      	if( dirty_tab & password_tab )
         {  //refresh password tab
            RefreshKeyPage();         //refreshTab()
            dirty_tab &~password_tab;
         }
        	break;
   }

   clearPermissions();
   resetPhysicalFlag();
   setGUIOptions();
}

//==============================================================================
//
// Function:    LoadPswdGrid
// Purpose:     This function will retrieve information and pass it into
//              PasswordQuery. The information includes the date the transaction
//              occurred, the description, units, operator, & password.
//              For keys that are of KEYDevelopment type, we use the
//              extension functionality of normal keys, except the units are the
//              index into the expiration date/counter hours. Ex. an extension
//              for 1 day will be the index into the first row in the table
//              sPasswordDevLookup which gives an expiration date in 30 days
//              and sets the counter hours to 100. Here, the units get the
//              description of on Extended Trial Period for the Test/Dev Key.
// Parameters:  None
// Returns:     None
//
//==============================================================================
void __fastcall TFCustomerKeys::LoadPswdGrid()
{
   /*
     SELECT SDescriptionRecord.SDRname, sTransactionDetail.*, sKeyRecord.SKRstatus,
     CONVERT(char(12),sTransactionDetail.TDdate,107)As t_date FROM sTransactionDetail
     INNER JOIN sDescriptionRecord ON SDescriptionRecord.SDRid = sTransactionDetail.SDRid
     INNER JOIN sKeyRecord ON sTransactionDetail.SKRid = sKeyRecord.SKRid
     WHERE (((sTransactionDetail.SKRid)= :skrid))
   */
   try
   {
       PasswordQuery->Close();
       PasswordQuery->ParamByName("skrId")->AsInteger = key_record->skr_id;
   }
   catch(Exception *e)
   {
       Application->MessageBox( e->Message.c_str(), "Key Message", MB_OK|MB_ICONERROR);
   }
}

//==============================================================================
// Function:    KeyGridDrawColumnCell
// Purpose:     Is called for every cell that needs to be redrawn, however
//              because the DefaultDrawing = true for KeyGrid each cell will
//              already contain the default value.  You only need to redraw a
//              cell if you want it's contents to display differently from the
//              default.
// Parameters:  TObject* - Sender
//              TRect    - &Rect
//              int      - DataCol
//              TColumn* - Column
//              TGridDrawState - State
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::KeyGridDrawColumnCell(TObject *Sender,
	const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State)
{
   int intToHex;

   //
   // there are no keys
   if( !QueryKey->RecordCount )
   	return;

   if( Column->FieldName == "SCRnumber")
   {
       if( Column->Field->DisplayText.Length() != 0 )
       {
          intToHex = Column->Field->DisplayText.ToInt();
          KeyGrid->Canvas->FillRect( Rect );
          sprintf( scratch, "   %s", IntToHex(intToHex, 2));
          KeyGrid->Canvas->TextOut( Rect.Left+8, Rect.Top+2, scratch);
       }
   }
   if( Column->FieldName == "SKRnumber")
   {
       if( Column->Field->DisplayText.Length() != 0 )
       {
          intToHex = Column->Field->DisplayText.ToInt();
          KeyGrid->Canvas->FillRect( Rect );
          sprintf( scratch, "%s", IntToHex(intToHex, 2) );
          KeyGrid->Canvas->TextOut( Rect.Left+8, Rect.Top+2, scratch);
       }
   }
   else if( Column->FieldName == "SKRproduct" )
   {
      try
      {
         if(!Column->Field->IsNull)
         {
            int product = Column->Field->AsInteger;
            int version = KeyGrid->Columns->Items[3]->Field->AsInteger;
            KeyGrid->Canvas->TextOut( Rect.Left+8, Rect.Top+2, lookup->getProductText(product, version));
         }
      }
      catch(...){}//do not reset display value
   }
   else if( Column->FieldName == "SKRversion" )
   {
      try
      {
         if(!Column->Field->IsNull)
         {
            int version = Column->Field->AsInteger;
            sprintf( scratch, "%X.%03X", version/0x1000, version&0x0fff );

            KeyGrid->Canvas->FillRect( Rect );
            KeyGrid->Canvas->TextOut( Rect.Left+8, Rect.Top+2, scratch);
         }
      }
      catch(...){}//do not reset display value
   }
   else if( Column->FieldName == "SKRstatus" )
   {
      try
      {
         KeyGrid->Canvas->FillRect( Rect );
         KeyGrid->Canvas->TextOut( Rect.Left+8, Rect.Top+2, key_record->getStatusText(static_cast<unsigned short>(Column->Field->AsInteger)) );
      }
      catch(...){}//do not modify display text
   }
   else if( Column->FieldName == "KTid" )
   {
      KeyGrid->Canvas->FillRect( Rect );
      KeyGrid->Canvas->TextOut( Rect.Left+8, Rect.Top+2, getKeyTypeText(QueryKey->FieldByName("KTid")->AsInteger) );
   }

}

//==============================================================================
// Function:    mmVersionClick
// Purpose:     Update version information in the database and generates a
//              password to give to the customer.
// Parameters:  TObject* Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::mmVersionClick(TObject *Sender)
{
   unsigned short version(0);

   TDlgVersion *dlg = new TDlgVersion(this);
   //
   // set dialog options
   dlg->setDialog(key_record->pkey->productId, key_record->pkey->productVersion, (PERMISSION_FLAG & permanent_pwd));
   if( dlg->ShowModal() == mrOk )
   {
      version = dlg->getVersion();
      createVersionPassword(version);
   }

   if (dlg)
   {
        delete dlg;
        dlg = NULL;
   }
}

//==============================================================================
// Function:    createVersionPassword()
// Purpose:     This function creates a version password for a user for a certain
//              key.  This function updates the SKeyRecord and STransactionDetail
//              tables in the database.
// Parameters:  unsigned short - version
// Returns:     None
//==============================================================================
void TFCustomerKeys::createVersionPassword(unsigned short version)
{
   char password_string[128];

   if(!isAttachedKeyReady())
      return;

   //
   //generate password
   keyMaster->getProductVersionPassword(key_record->pkey, version, password_string);
   if(!password_string)
   {
      //Application->MessageBox("Unable to generate password.", "Key Message", MB_OK|MB_ICONERROR );
      return;
   }

   //
   //apply password to key
   keyMaster->applyVersionPassword(key_record, version);

   //
   //Update database
   try
   {
      Database1->StartTransaction();

      UtilityQuery->Close();
      UtilityQuery->SQL->Clear();
      UtilityQuery->SQL->Add("UPDATE SKeyRecord SET SKRversion = :version WHERE SKRid = :keyId ");
      UtilityQuery->ParamByName("keyId")->AsInteger = key_record->skr_id;
      UtilityQuery->ParamByName("version")->AsInteger = version;
      UtilityQuery->ExecSQL();

      UtilityQuery->SQL->Add("INSERT INTO sTransactionDetail (SKRid, TDpassword, SDRid, TDunits, TDrow_id) values (:key_id, :password, 254, :units, 0)");
      UtilityQuery->ParamByName("key_id")->AsInteger = key_record->skr_id;
      UtilityQuery->ParamByName("units")->AsString = IntToHex(version, 4);
      UtilityQuery->ParamByName("password")->AsString = password_string;
      UtilityQuery->ExecSQL();

      Database1->Commit();
      RefreshKeyPage(0);
      RefreshKeyPage();
   }
   catch(EDBEngineError &e)
   {
      Database1->Rollback(); //error occurred rollback db changes
      Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
   }
}

//==============================================================================
// Function:    mmExtensionClick
// Purpose:     create an extension password for the key currently selected in
//              the database by calling createExtensionPassword.  This is the
//              event handler. The drop down consists of password text combos but
//              only the days are relevant since the number of hours do not change.
//              Only the days are recorded and updated in the key and database, the
//              number of hours remain the same as when the key was initially programmed.
// Parameters:  ( TObject* ) - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::mmExtensionClick(TObject* Sender)
{
   Sender;

   //
   // Check to see if the key is at the max extension
   switch(key_record->pkey->status)
   {
       case EXTENDED_TRIAL5:
           Application->MessageBox("Unable to generate password. You have reached the maximum number extensions for this key.", "Key Message", MB_OK|MB_ICONERROR );
           return;
       default:
           break;
   }

   //
   // If we are not at the max extension, check the key type to display the appropriate dialog box.
   unsigned short index;
   if (key_record->pkey->keyType == KEYDevelopment)
   {
       TDlgDevelopmentPassword *dlg = new TDlgDevelopmentPassword( this );
       try
       {
           for (int i = 0; i < KD_PASSWORD_MAX; i++)
               dlg->passwordComboBox->Items->Add(key_record->pkey->getKDPasswordText(i));
           dlg->passwordComboBox->ItemIndex = 0;
       }
       catch(EDBEngineError &e)
       {
           dlg->passwordComboBox->Enabled = false;
       }

       if( dlg->ShowModal() == IDYES )
       {   //the index should be the value passed into createExtensionPassword. LicenseServer expects index into KDPasswordHour array
           index = dlg->passwordComboBox->ItemIndex + 1; //+1 so display of units will be at least 1. Subtract 1 when using this value to index.
           createExtensionPassword(index);
       }

       delete dlg;
       dlg = NULL;
   }
   else
   {
       TDlgExtension *dlg = new TDlgExtension( this, key_record );
       if( dlg->ShowModal() == IDYES )
           createExtensionPassword(static_cast<unsigned short>(dlg->Days->Text.ToInt()));

       delete dlg;
       dlg = NULL;
   }
}

//==============================================================================
// Function:    createExtensionPassword
// Purpose:     Creates an extension password for the key currently selected in
//              the database.  This will update the SKeyRecord and STransactionDetail
//              tables in the database.
// Parameters:  ( unsigned char ) - days
// Returns:     None
// Note:        Should not be called if key can not be extended
//==============================================================================
void TFCustomerKeys::createExtensionPassword(unsigned short days)
{
   char password_string[128];
   unsigned short expire_date = days;
   //
   //check if key is attached
   if(!isAttachedKeyReady())
      return;

   //
   //generate password
   keyMaster->getExtensionPassword(key_record->pkey,
                                   days,
                                   static_cast<ProductId>(key_record->pkey->productId),
                                   key_record->pkey->productVersion,
                                   key_record->getNextExtensionP(),
                                   password_string
                                   );
   if(!password_string)
   {
      Application->MessageBox("Unable to generate password.", "Key Message", MB_OK|MB_ICONERROR );
   	return;
   }

   //
   //apply password to key
   keyMaster->applyExtensionPassword(key_record, days);

   //Test/Dev keys store index into units field. So need to translate index to actual number of days.
   if(key_record->pkey->keyType == KEYDevelopment)
        expire_date = key_record->pkey->getKDPasswordDays(days);

   //
   //Update database
   try
   {
      Database1->StartTransaction();

      UtilityQuery->Close();
      UtilityQuery->SQL->Clear();
      UtilityQuery->SQL->Add("UPDATE SKeyRecord SET SKRstatus = :status, SKRexpirationDate = (getdate() + :exp_date) WHERE SKRid = :skr_id ");
      UtilityQuery->ParamByName("skr_id")->AsInteger = key_record->skr_id;
      UtilityQuery->ParamByName("status")->AsInteger = key_record->getStatus();
      UtilityQuery->ParamByName("exp_date")->AsInteger = expire_date;
      UtilityQuery->ExecSQL();

      UtilityQuery->SQL->Add("INSERT INTO sTransactionDetail (SKRid, TDpassword, SDRid, TDunits, TDrow_id) values (:skr_id, :password, 255, :days, 0)");
      UtilityQuery->ParamByName("skr_id")->AsInteger = key_record->skr_id;
      UtilityQuery->ParamByName("password")->AsString = password_string;
      UtilityQuery->ParamByName("days")->AsString = days;
      UtilityQuery->ExecSQL();

      Database1->Commit();
      RefreshKeyPage(0);
      RefreshKeyPage();
   }
   catch( Exception *e )
   {
      Application->MessageBox( e->Message.c_str(), "Database Failure", MB_OK);
      Database1->Rollback();
   }
}


//==============================================================================
// Function:    mmPermanentClick
// Purpose:     Changes the state of a key to permenant.  Function should only
//              be called by key that are currently in a trial state.
// Parameters:  TObject* - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::mmPermanentClick(TObject *Sender)
{
   char password_string[128];

   // Catch just in case the permanent flag is not caught
   if(key_record->non_perm_ktf == true)
   {
      Application->MessageBox("Unable to generate password for this key type.", "Key Message", MB_OK|MB_ICONERROR );
   	return;
   }

   //check if key is attached
   if(!isAttachedKeyReady())
        return;

   // generate password
   keyMaster->getPermanentPassword(key_record->pkey, password_string);
   if(!password_string)
   {
        Application->MessageBox("Unable to generate password.", "Key Message", MB_OK|MB_ICONERROR );
   	return;
   }

   // apply password to key
   keyMaster->applyPermanentPassword(key_record);

   // Update database
   try
   {
      Database1->StartTransaction();
      AnsiString updated_module_list(reinterpret_cast<char*>(key_record->pkey->keyDataBlock.data),static_cast<unsigned char>(36));

      UtilityQuery->Close();
      UtilityQuery->SQL->Clear();
      UtilityQuery->SQL->Add("UPDATE SKeyRecord SET SKRstatus = 2, "
                             "SKRoutput = 1, SKRoperatorSession = 1, SKRuserSession = 1, SKRppmextensions = 0, "
                             "modules = :module_list, SKRppmxchipds = :ipds, "
                             "SKRppmxchpcl = :pcl, SKRppmxchps = :ps, "
                             "SKRppmxchpsdbcs = :dbcs, SKRppmafpdsps = :afpds, "
                             "SKRindexServers =:indexServers, "
                             "SKRreportServers =:reportServers, "
                             "SKRconcurrentUsers =:concurrentUsers, "
                             "SKRapplications =:applications, "
                             "SKRdocumentAssembler =:documentAssembler WHERE SKRid = :keyId "
                             );
      UtilityQuery->ParamByName("module_list")->AsBlob = updated_module_list;
      UtilityQuery->ParamByName("ipds")->AsInteger = 0;
      UtilityQuery->ParamByName("pcl")->AsInteger = 0;
      UtilityQuery->ParamByName("ps")->AsInteger = 0;
      UtilityQuery->ParamByName("dbcs")->AsInteger = 0;
      UtilityQuery->ParamByName("afpds")->AsInteger = 0;
      UtilityQuery->ParamByName("keyId")->AsInteger = key_record->skr_id;
      UtilityQuery->ParamByName("indexServers")->AsInteger = 0;
      UtilityQuery->ParamByName("reportServers")->AsInteger = 0;
      UtilityQuery->ParamByName("concurrentUsers")->AsInteger = 0;
      UtilityQuery->ParamByName("applications")->AsInteger = 0;
      UtilityQuery->ParamByName("documentAssembler")->AsInteger = 0;
      UtilityQuery->ExecSQL();

      UtilityQuery->SQL->Add("INSERT INTO sTransactionDetail (SKRid, TDpassword, SDRid, TDunits, TDrow_id) values (:key_id, :password, 253, 0, 0)");
      UtilityQuery->ParamByName("key_id")->AsInteger = key_record->skr_id;
      UtilityQuery->ParamByName("password")->AsString = password_string;
      UtilityQuery->Prepare();
      UtilityQuery->ExecSQL();

      Database1->Commit();

      RefreshKeyPage(0);
      RefreshKeyPage();
   }
   catch( Exception *e )
   {
      Application->MessageBox( e->Message.c_str(), "Database Failure", MB_OK);
      Database1->Rollback();
   }
}

//==============================================================================
// Function:    RefreshKeyPage
// Purpose:     If called without a parameter, then refreshes the activepage,
// 	        else refreshes page specified by _index.
// Parameters:  ( int ) - _index
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::RefreshKeyPage(int _index)
{
   bool gFOUND = false;
   int skrid;
   int modId;
   TTabSheet *activeTab = PageControl1->ActivePage;

   if( QueryKey->RecordCount )                  // if not empty before refresh
   	skrid = QueryKey->FieldValues["SKRid"];  //save current key
   else
   	skrid = 0; // else assign to zero, zero is not a valid skrid

   if(_index == -1 )
   	_index = activeTab->PageIndex;
   //else user has specified a tab to update
      
   switch( _index )
   {
   	case 0 : //key page refresh
   	    QueryKey->DisableControls();
   	    QueryKey->Close();
            QueryKey->Open();

            if( QueryKey->RecordCount )
      	    	QueryKey->First();
            else
            {
            	//QueryKey->EnableControls();
                break;
            }

            while(!QueryKey->Eof && !gFOUND)
            {
      		if( QueryKey->FieldValues["SKRid"] == skrid )
      		        gFOUND = true;
                else
      			QueryKey->Next();
            }
      	    if( !gFOUND )   // if not found set to start
      	    	QueryKey->First();

      	    QueryKey->EnableControls();
            break;

      case 1 : //module page refresh
            if ( (key_record->pkey->productId == SPD_PRODUCT) ||
                 (key_record->pkey->productId == CONNECT_PRODUCT) ||
                 (key_record->pkey->productId == QUANTUM_PRODUCT) ||
                 (key_record->pkey->productId == ICONVERT_PRODUCT)  )
            {
               KeyFormModuleFrame->load(key_record);
               setOutputUnitsDisplay(static_cast<SpdProtectionKey*>(key_record->pkey)->outputUnits);
               //setOutputUnitsDisplay( ((SpdProtectionKey*)(key_record->pkey))->outputUnits);
            }
            else if ( key_record->pkey->productId == SPDE_PRODUCT)
            {
               KeyFormModuleFrame->load(key_record);  //stupid code since the set functions already call the key_record for units
               setOutputUnitsDisplay(static_cast<SpdeProtectionKey*>(key_record->pkey)->outputUnits);
               setOperatorSessionUnitsDisplay(static_cast<SpdeProtectionKey*>(key_record->pkey)->operatorSessionUnits);
               setUserSessionUnitsDisplay(static_cast<SpdeProtectionKey*>(key_record->pkey)->userSessionUnits);
            }
            else if (key_record->pkey->productId == SOLSCRIPT_PRODUCT ||
                     /*key_record->pkey->productId == SDX_DESIGNER_PRODUCT ||*/
                     key_record->pkey->productId == RUBIKA_PRODUCT) {
               KeyFormModuleFrame->load(key_record);
            }
            break;

      case 2 :  //solsearch details
            SolSearcherDetails->Load(key_record);
         break;
      case 3 :
         PasswordQuery->DisableControls();

         PasswordQuery->Close();
         PasswordQuery->Open();
         PasswordQuery->First();
         PasswordQuery->EnableControls();
         break;
   }

   setStatusValues();
   setKeyInfoValues();

}

//==============================================================================
// Function:    setOutputUnitsDisplay
// Purpose:     Sets the ouput to display whether the key is new or old licensing.
// Parameters:  ( unsigned short ) -  outputs
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::setOutputUnitsDisplay(unsigned short outputs)
{
   unsigned short units;
   if(key_record->pkey->productId == SPDE_PRODUCT)
         units = (static_cast<SpdeProtectionKey*>(key_record->pkey)->outputUnits);
   else
         units = (static_cast<SpdProtectionKey*>(key_record->pkey)->outputUnits);

   output_devices->Text = units;
   if (key_record->pkey->productId != ICONVERT_PRODUCT)
   {
      if(units == 0)
        OutputDescription->Caption = "Old Licensing";
      else
        OutputDescription->Caption = "New Licensing";
   }
   else // is ICONVERT
   {
      if(units == 0)
        OutputDescription->Caption = "NOT Licensed for LU's";
      else
        OutputDescription->Caption = "Licensed for LU's";

   }
}

void TFCustomerKeys::setOperatorSessionUnitsDisplay(unsigned short units)
{
   units = (static_cast<SpdeProtectionKey*>(key_record->pkey)->operatorSessionUnits);
   operator_sessions->Text = units;
}

void TFCustomerKeys::setUserSessionUnitsDisplay(unsigned short units)
{
   units = (static_cast<SpdeProtectionKey*>(key_record->pkey)->userSessionUnits);
   user_sessions->Text = units;
}

//==============================================================================
// Function:    setStatusValues
// Purpose:     Set values displayed in the StatusBar.
// Parameters:  None
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::setStatusValues()
{
   unsigned short version(key_record->pkey->productVersion);

   // set key number
   sprintf( scratch, "Key: %s", IntToHex(key_record->pkey->keyNumber, 2));
	StatusBar1->Panels->Items[KEY]->Text = scratch;

   // set product
   sprintf( scratch, "Product: %s", lookup->getProductText(key_record->pkey->productId, version));
      	StatusBar1->Panels->Items[PRODUCT]->Text = scratch;

   // set version
   sprintf( scratch, "Version: %X.%03X", version/0x1000, version&0x0fff );
	StatusBar1->Panels->Items[VERSION]->Text = scratch;

}

//==============================================================================
// Function:    modulePasswordCreated()
// Purpose:     Once the module password is created, refresh pages and update
//              the SKeyRecord structure.
// Parameters:  None
// Returns:     None
//==============================================================================
void TFCustomerKeys::modulePasswordCreated()
{
   RefreshKeyPage(0);   //re-queries database for key information
   OnKeyRowChange(this);//loads key information into SKeyRecord structure
   RefreshKeyPage(1);   //reloads modules based on information in SKeyRecord
}

//==============================================================================
// Function:    PrintBtnClick
// Purpose:     Prints all selected rows in the password grid.
// Parameters:  ( TObject * ) - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::PrintBtnClick(TObject *Sender)
{
   AnsiString report_list;
   TBookmark tmpMark;

   if (PswdGrid->SelectedRows->Count > 0)
   {
      try
      {
         tmpMark = PasswordQuery->GetBookmark();
         PasswordQuery->DisableControls();

         PasswordQuery->First();
      	 while( !PasswordQuery->Eof )
         {
       		if( PswdGrid->SelectedRows->CurrentRowSelected )
                {
         	    report_list += Format("TDid = %d or ", OPENARRAY(TVarRec,(PasswordQuery->FieldByName("TDid")->AsInteger)));;
         	}
         	PasswordQuery->Next();
      	 }

         report_list.SetLength(report_list.Length()-3); //remove last or statement
         PasswordQuery->GotoBookmark( tmpMark );
      	 PasswordQuery->FreeBookmark( tmpMark );
      	 PasswordQuery->EnableControls();

         TPasswordReport *report = new TPasswordReport( this, report_list );
      	 report->Preview();

         delete( report );
      }
      catch(EDBEngineError &e)
      {
         PasswordQuery->GotoBookmark( tmpMark );
         PasswordQuery->FreeBookmark( tmpMark );
         PasswordQuery->EnableControls();

         Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
      }
   }
}

//==============================================================================
// Function:    ArchiveBtnClick
// Purpose:     Creates a password packet file corresponding to the selected passwords
// Parameters:  ( TObject * ) - Sender
// Returns:     None
//==============================================================================
const char* PasswordPacketDirectory = "L:\\Password Packets\\";
//const char* PasswordPacketDirectory = "R:\\Solimar Password Bundles\\";
void __fastcall TFCustomerKeys::ArchiveBtnClick(TObject *Sender)
{
   TBookmark tmpMark;
   AnsiString AnsiPassword;            //stores password read from grid
   //AnsiString PasswordPacketDirectory;
   wchar_t wcharPassword[256];    //stores password that is converted from ansi to wchar
   BSTR bstrPassword;             //stores the BSTR version of the password
   BSTR VerificationCode;
   int PasswordBufSize = 0;
   AnsiString CurrentDate;
   HRESULT hr = 0;

   SYSTEMTIME CurrentTime;
   VARIANT varTime;
   varTime.vt = VT_DATE;

   GetLocalTime(&CurrentTime);
   SystemTimeToVariantTime(&CurrentTime, &varTime.date);
   varTime.date += 7.0;      //add 7 days to the expiration date.

   int PasswordIDs[1024];
   memset(PasswordIDs, 0, 1024);

   int PasswordIDIndex = 0;

   /*char* buffer = NULL;
   PasswordPacketDirectory = getcwd(buffer, MAXPATH);
   PasswordPacketDirectory =  PasswordPacketDirectory + "\\Password Packets\\";
   */
   //only get the rows selected

   if (PswdGrid->SelectedRows->Count > 0)
   {
      try
      {
         HRESULT hr = keyMaster->InitPasswordPacket();
         tmpMark = PasswordQuery->GetBookmark();
         PasswordQuery->DisableControls();

         PasswordQuery->First();

      	while( !PasswordQuery->Eof )
         {
            //if a row is selected append the password to the packet and remember its ID in order to store it in the DB
       	    if( PswdGrid->SelectedRows->CurrentRowSelected )
            {
               PasswordIDs[PasswordIDIndex] = PasswordQuery->FieldByName("TDid")->AsInteger;
               PasswordIDIndex++;

               AnsiPassword = PswdGrid->Columns->Items[3]->Field->AsString;
               PasswordBufSize = AnsiPassword.WideCharBufSize();
               AnsiPassword.WideChar(wcharPassword, PasswordBufSize);
               bstrPassword = SysAllocString(wcharPassword);
               hr = keyMaster->AppendPasswordToPacket(varTime, bstrPassword);
               SysFreeString(bstrPassword);
             }
             PasswordQuery->Next();
      	}
         hr = keyMaster->FinalizePasswordPacket();

         varTime.date -= 7.0;    //remove the 7 days that was added before

         //create the filename
         char TheFileName[128];
         memset(TheFileName, 0, 128);

         sprintf(TheFileName, " PasswordPacket%s-%s_%d-%d-%d_%d-%d-%d-%d.pkt",
                 IntToHex(key_record->pkey->customerNumber, 3),
                 IntToHex(key_record->pkey->keyNumber, 2),
                 CurrentTime.wMonth, CurrentTime.wDay, CurrentTime.wYear,
                 CurrentTime.wHour, CurrentTime.wMinute, CurrentTime.wSecond, CurrentTime.wMilliseconds);

         //Create the FilePath
         char FilePath[1024];
         memset(FilePath, 0, 1024);
         sprintf(FilePath, "%s%s", PasswordPacketDirectory /*.c_str()*/, TheFileName);

         // Create the password packet file
	      HANDLE hFile = CreateFile(FilePath,
                                    GENERIC_WRITE,
                                    FILE_SHARE_WRITE,
                                    0,
                                    CREATE_NEW, 0, 0);

         hr = HRESULT_FROM_WIN32(::GetLastError());
         if(HRESULT_FROM_WIN32(ERROR_FILE_EXISTS) == hr)
         {
            Application->MessageBox( "Error: File Exists","Error", MB_OK);
            return;
         }

         // create a variant safearray
	 VARIANT vtPacket;
	 VariantInit(&vtPacket);
         BYTE* pData = 0;

         //Get the password packet and write it to the file
         hr = keyMaster->GetPasswordPacket(&vtPacket);
	      if (SUCCEEDED(SafeArrayAccessData(vtPacket.parray, (void**)&pData)))
	      {
		      DWORD bytes_written(0);
		      if (!WriteFile(hFile,
                           pData,
                           vtPacket.parray->rgsabound[0].cElements, //size of array
                           &bytes_written,
                           0))
		      {
			      hr = HRESULT_FROM_WIN32(::GetLastError());
		      }

		      SafeArrayUnaccessData(vtPacket.parray);
		      pData = 0;
	      }

         //Get the verification code and store it in the DB
         hr = keyMaster->GetVerificationCode(&VerificationCode);

         //
         //Update database
         try
         {
            Database1->StartTransaction();

            UtilityQuery->Close();
            UtilityQuery->SQL->Clear();
            UtilityQuery->SQL->Add("INSERT INTO sPasswordPacket (packet_path, packet_verification, packet_created) values (:path, :ver_code, :date_created)");
            UtilityQuery->ParamByName("path")->AsString = TheFileName;
            UtilityQuery->ParamByName("ver_code")->AsString = VerificationCode;
            UtilityQuery->ParamByName("date_created")->AsDateTime = varTime.date;
            UtilityQuery->ExecSQL();

            Database1->Commit();
            RefreshKeyPage();

            //Get the password packet ID
            UtilityQuery->Close();
            UtilityQuery->SQL->Clear();
            UtilityQuery->SQL->Add("SELECT * FROM sPasswordPacket WHERE packet_path = :path AND packet_verification = :ver_code");
            UtilityQuery->ParamByName("path")->AsString = TheFileName;
            UtilityQuery->ParamByName("ver_code")->AsString = VerificationCode;
            UtilityQuery->Prepare();
            UtilityQuery->Open();

            //If the ID was found
            if( UtilityQuery->RecordCount )
            {
               //Store each password in the packet with its corresponding packet ID
               int PasswordPacketID;
               PasswordPacketID = UtilityQuery->FieldValues["packet_id"];

               for(int tempIndex = 0; tempIndex < PasswordIDIndex; tempIndex++)
               {
                  UtilityQuery->Close();
                  UtilityQuery->SQL->Clear();
                  UtilityQuery->SQL->Add("INSERT INTO sPasswordPacketRelationship (packet_id, password_id) values (:pack_id, :pwd_id)");
                  UtilityQuery->ParamByName("pack_id")->AsInteger = PasswordPacketID;
                  UtilityQuery->ParamByName("pwd_id")->AsInteger =  PasswordIDs[tempIndex];
                  UtilityQuery->ExecSQL();
               }
            }
            else
            {
            	Application->MessageBox("Program Failed", "Key Message", MB_OK );
            }
         }
         catch(EDBEngineError &e)
         {
            Database1->Rollback(); //error occurred rollback db changes
            Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
         }
        //free(buffer); //clear the buffer holding the current working directory
        CloseHandle(hFile); //Release handle to the packet file.
        VariantClear(&vtPacket);
        PasswordQuery->GotoBookmark( tmpMark );
     	PasswordQuery->FreeBookmark( tmpMark );
      	PasswordQuery->EnableControls();

      }
      catch(EDBEngineError &e)
      {
         PasswordQuery->GotoBookmark( tmpMark );
         PasswordQuery->FreeBookmark( tmpMark );
         PasswordQuery->EnableControls();

         Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
      }
   }
   VariantClear(&varTime);
}

//==============================================================================
// Function:    Exit1Click
// Purpose:     Closes this form in the application.
// Parameters:  ( TObject* ) - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::Exit1Click(TObject *Sender)
{
   Close();
}

//==============================================================================
// Function:    dbSaveKey
// Purpose:
// Parameters:  ( SKeyRecord* ) - programmed
//              ( TKeyWizardFrm* ) - wizard
// Returns:     bool
//==============================================================================
bool TFCustomerKeys::dbSaveKey(SKeyRecord* programmed, TKeyWizardFrm* wizard)
{
   //attempt to update key after it has been programmed - do not need to update
   //fields that where already saved on reserving the key

   //
   // (SCRnumber, SKRnumber, SKRactivationDate, SKRoperator should already be
   // set when the key is reserved.  SKRdescription will nolonger be used.
   bool result(false);
   try
   {
      //Start a transaction
      Database1->StartTransaction();

      //check if programmed key has a system_id or one needs to be generated
      if(programmed->system_id==0)
      {
         sp_GenerateSystemId->Close();
         sp_GenerateSystemId->Prepare();
         sp_GenerateSystemId->ExecProc();

         programmed->system_id = sp_GenerateSystemId->ParamByName("RETURN_VALUE")->AsInteger;
      }
      int output(0);
      int operatorSessions(0);
      int userSessions(0);
      int ppmExtensions(0);
      int ppmXchIpds(0);
      int ppmXchPsDbcs(0);
      int ppmAfpds(0);
      int indexServers(0);
      int reportServers(0);
      int concurrentUsers(0);
      int applications(0);
      int documentAssembler(0);

      int selectedProduct(programmed->pkey->productId);
      if      (selectedProduct == SPD_PRODUCT       ||
               selectedProduct == CONNECT_PRODUCT   ||
               selectedProduct == QUANTUM_PRODUCT   ||
               selectedProduct == ICONVERT_PRODUCT) {

         output = reinterpret_cast<SpdProtectionKey*>(programmed->pkey)->outputUnits;
      }
      else if (selectedProduct == SOLSEARCHER_ENTERPRISE_PRODUCT) {
         indexServers      = ((SSProtectionKey*)(programmed->pkey))->getIndexServers();
         reportServers     = ((SSProtectionKey*)(programmed->pkey))->getReportServers();
         concurrentUsers   = ((SSProtectionKey*)(programmed->pkey))->getConcurrentUsers();
         applications      = ((SSProtectionKey*)(programmed->pkey))->getApplications();
         documentAssembler = ((SSProtectionKey*)(programmed->pkey))->getDocumentAssembler();
      }
      else if(selectedProduct == SPDE_PRODUCT) {
         output = reinterpret_cast<SpdeProtectionKey*>(programmed->pkey)->outputUnits;
         operatorSessions =   reinterpret_cast<SpdeProtectionKey*>(programmed->pkey)->operatorSessionUnits;
         userSessions =    reinterpret_cast<SpdeProtectionKey*>(programmed->pkey)->userSessionUnits;
      }
      else {

      }

      UtilityQuery->Close();
      UtilityQuery->SQL->Clear();

      if( programmed->getStatus() != 1 ) // if key is not permanent then
      {                                 // expirationDate needs to be set
         UtilityQuery->SQL->Add( "UPDATE SKeyRecord SET "
                                 "SKRactivationDate = getdate(), "
                                 "SKRexpirationDate = :expirationDate, "
                                 "SKRproduct = :product, "
                                 "SKRversion = :version, "
                                 "SKRstatus = :status, "
                                 "modules_initial = CONVERT(binary(72), :module_list), "
                                 "modules = CONVERT(binary(72), :module_list), "
                                 "SKRpo = :po, "
                                 "SKReBOnumber = :eBOnumber, "
                                 "SKRkeyClass = :keyClass, "
                                 "SDid = :SDid, "
                                 "KTid = :KTid, "
                                 "SKRnonpermktf = :ktflag, "
                                 "SKRdescription = :summary, "
                                 "SKRppmextensions = :extensions, "
                                 "SKRppmxchipds = :ipds, "
                                 "SKRppmxchpcl = :pcl, "
                                 "SKRppmxchps = :ps, "
                                 "SKRppmxchpsdbcs = :psdbcs, "
                                 "SKRppmafpdsps = :afpds, "
                                 "SKRindexServers = :index_servers, "
                                 "SKRreportServers = :report_servers, "
                                 "SKRconcurrentUsers = :concurrent_users, "
                                 "SKRapplications = :applications, "
                                 "SKRdocumentAssembler = :document_assembler, "
                                 "SKRoutput = :output, "
                                 "SKRoperatorSession = :operator_sessions, "
                                 "SKRuserSession = :user_sessions WHERE SKRid = :skrid ");

//-----------------------------------------------------------
         time_t test(programmed->pkey->expirationDate);
         struct tm *area(localtime(&test));
         char* tempdate = new char[20];
         strftime(tempdate,20,"%x",area);
         AnsiString test2 = tempdate;//asctime(area);
         UtilityQuery->ParamByName("expirationDate")->AsString = test2;
         delete tempdate;
//-----------------------------------------------------------
      }
      else // key is permanent, do not need to set programmedDays or expirationDate
      {
         UtilityQuery->SQL->Add( "UPDATE SKeyRecord SET "
                                 "SKRactivationDate = getdate(), "
                                 "SKRproduct = :product, "
                                 "SKRversion = :version, "
                                 "SKRstatus = :status, "
                                 "modules_initial = CONVERT(binary(72), :module_list), "
                                 "modules = CONVERT(binary(72), :module_list), "
                                 "SKRpo = :po, "
                                 "SKReBOnumber = :eBOnumber, "
                                 "SKRkeyClass = :keyClass, "
                                 "SDid = :SDid, "
                                 "KTid = :KTid, "
                                 "SKRnonpermktf = :ktflag, "
                                 "SKRdescription = :summary, "
                                 "SKRppmextensions = :extensions, "
                                 "SKRindexServers = :index_servers, "
                                 "SKRreportServers = :report_servers, "
                                 "SKRconcurrentUsers = :concurrent_users, "
                                 "SKRapplications = :applications, "
                                 "SKRdocumentAssembler = :document_assembler, "
                                 "SKRoutput = :output, "
                                 "SKRoperatorSession = :operator_sessions, "
                                 "SKRuserSession = :user_sessions WHERE SKRid = :skrid ");
      }

      UtilityQuery->ParamByName("product")->AsInteger = programmed->pkey->productId;//product;
      UtilityQuery->ParamByName("version")->AsInteger = programmed->pkey->productVersion;//version;
      UtilityQuery->ParamByName("status")->AsInteger = programmed->pkey->status;//dlg->GetSelectedLicense();
      UtilityQuery->ParamByName("KTid")->AsInteger = key_record->getKeyTypeIntegerValue(programmed->pkey->keyType);
      UtilityQuery->ParamByName("po")->AsInteger = programmed->po;//dlg->EditPO->Text.ToInt();
      UtilityQuery->ParamByName("eBOnumber")->AsString = programmed->eBOnumber;
      UtilityQuery->ParamByName("output")->AsInteger = output;
      AnsiString mytest(reinterpret_cast<char*>(programmed->pkey->keyDataBlock.data),static_cast<unsigned char>(36));
      UtilityQuery->ParamByName("module_list")->AsBlob = mytest;
      UtilityQuery->ParamByName("skrid")->AsInteger =  programmed->skr_id;
      UtilityQuery->ParamByName("SDid")->AsInteger = programmed->system_id;
      UtilityQuery->ParamByName("summary")->AsString = programmed->summary;
      UtilityQuery->ParamByName("keyClass")->AsInteger = programmed->key_class;
      UtilityQuery->ParamByName("ktflag")->AsBoolean = programmed->non_perm_ktf;
      UtilityQuery->ParamByName("extensions")->AsInteger = programmed->ppm_extensions;
      UtilityQuery->ParamByName("ipds")->AsInteger = MAX_PAGES_PER_MINUTE;
      UtilityQuery->ParamByName("pcl")->AsInteger = MAX_PAGES_PER_MINUTE;
      UtilityQuery->ParamByName("ps")->AsInteger = MAX_PAGES_PER_MINUTE;
      UtilityQuery->ParamByName("psdbcs")->AsInteger = MAX_PAGES_PER_MINUTE;
      UtilityQuery->ParamByName("afpds")->AsInteger = MAX_PAGES_PER_MINUTE;
      UtilityQuery->ParamByName("index_servers")->AsInteger = indexServers;
      UtilityQuery->ParamByName("report_servers")->AsInteger = reportServers;
      UtilityQuery->ParamByName("concurrent_users")->AsInteger = concurrentUsers;
      UtilityQuery->ParamByName("applications")->AsInteger = applications;
      UtilityQuery->ParamByName("document_assembler")->AsInteger = documentAssembler;
      UtilityQuery->ParamByName("operator_sessions")->AsInteger = operatorSessions;
      UtilityQuery->ParamByName("user_sessions")->AsInteger = userSessions;
      UtilityQuery->Prepare();
      UtilityQuery->ExecSQL();

      Database1->Commit(); // try to commit changes
      result = true;
   }
   catch(Exception& e)      //handles database exceptions thrown by the BDE
   {
      Database1->Rollback();
      Application->MessageBox( e.Message.c_str(), "Database Failure", MB_OK|MB_ICONERROR );

      if( keyMaster->deactivate() ) // clears key data and increments uses
      {  // if failed, display failure message
         Application->MessageBox("Error occurred while trying to clean-up after \n"
                                 "key programming failure, key may contain licensing \n"
                                 "information.", "Key Failure", MB_OK|MB_ICONERROR );
      }
   } //** catch **


   RefreshKeyPage();

   return result;
}

//==============================================================================
// Function:    mmProgramClick()
// Purpose:     Opens a dialog to program a protection.  The program dialog will
//              only return a modale resulte of OK if the program dialog
//              information has been written to the key.  If OK then attempt to
//              update the database.  If database update fails, clear the
//              information written to key and redisplay program dialog.
// Parameters:  ( TObject* ) - Sender
// Returns:     None
// Note:        new functionality - can only program a reserved key
//==============================================================================
void __fastcall TFCustomerKeys::mmProgramClick(TObject *Sender)
{
   //unreferenced variable
   Sender;

   //Check if key is reserved and exit on fail, the UI should block you from this
   //action but check anyways
   if(key_record->getKeyTypeIntegerValue(key_record->pkey->keyType) != ktfReserved)
      return;

   //Create a wizard
   TKeyWizardFrm* wizard = new TKeyWizardFrm(key_record, this);

   while(1)
   {
      // continue until key is programmed or user cancels action
      if( wizard->ShowModal() == mrOk )
      {
         // key_record should contain all information needed to program key
         // if programming is successful update record information in the database
         // create temp. key to represent customer key
//          if( keyMaster->found() )
         if( !keyMaster->programmed() && keyMaster->found() )
         {
            // key is attached and unprogrammed, program key and save action in database
            if( keyMaster->program(key_record) == 0 )
            {
               // key programmed successfully save action in database
               if( dbSaveKey(key_record, wizard) == true )
               {
                  break;
               }
            }
         }
         else if( !keyMaster->found() )
            Application->MessageBox("No key attached to pc.", "Information", MB_OK);
         else
            Application->MessageBox("Attached key is already programmed.", "Information", MB_OK);
      }
      // user canceled action, break out of while
      else
         break;

   }

   delete wizard;
   OnKeyRowChange(this);
}

//==============================================================================
// Function:    mmSPDOutputClick()
// Purpose:
// Parameters:  ( TObject* ) - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::mmSPDOutputClick(TObject *Sender)
{
   Sender;
   int output_units(0);

   //get new total of outputs
   TUnitsDlg *dlg = new TUnitsDlg(this, 0xFFF);
   dlg->Edit1->Text = 1;

   if( dlg->ShowModal() == IDYES )
   {
      output_units = dlg->Edit1->Text.ToInt();
      //
      //check if invalid values returned
      createOutputPassword(output_units);
   }
   //do not need dialog anymore
   delete(dlg);
   dlg = NULL;
}

//---------------------------------------------------------------------------

void __fastcall TFCustomerKeys::mmSPDEOperatorSessionsClick(TObject *Sender)
{
   int operator_sessions(0);

   //get new total of outputs
   TUnitsDlg *dlg = new TUnitsDlg(this, 0xFF);
   dlg->Edit1->Text = 1;

   if( dlg->ShowModal() == IDYES )
   {
      operator_sessions = dlg->Edit1->Text.ToInt();
      //
      //check if invalid values returned
      createOperatorSessionPassword(operator_sessions);
   }
   //do not need dialog anymore
   delete(dlg);
   dlg = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TFCustomerKeys::mmSPDEUserSessionsClick(TObject *Sender)
{
   int user_sessions(0);

   //get new total of outputs
   TUnitsDlg *dlg = new TUnitsDlg(this, 0xFF);
   dlg->Edit1->Text = 1;

   if( dlg->ShowModal() == IDYES )
   {
      user_sessions = dlg->Edit1->Text.ToInt();
      //
      //check if invalid values returned
      createUserSessionPassword(user_sessions);
   }
   //do not need dialog anymore
   delete(dlg);
   dlg = NULL;
}
//---------------------------------------------------------------------------


//==============================================================================
// Function:    createOutputPassword()
// Purpose:     creates an output password, and updates the information in the
//              SKeyRecord and STransactionDetail tables based on the key record
//              id number.
// Parameters:  ( int ) - output_units
// Returns:     None
//==============================================================================
void TFCustomerKeys::createOutputPassword(int output_units)
{
   char password_string[128];

   //create both for now, need to fix copy constructor....
   //check attached key status - need to have a programmed key attached
      SpdeProtectionKey* spde_key((SpdeProtectionKey*)(key_record->pkey));
      SpdProtectionKey* spd_key((SpdProtectionKey*)(key_record->pkey));
   //check if key is attached
   if(!isAttachedKeyReady())
      return;

   //generate password
   if(key_record->pkey->productId == SPDE_PRODUCT)
        keyMaster->getOutputPassword(spde_key, output_units, password_string);
   else
        keyMaster->getOutputPassword(spd_key, output_units, password_string);
   if(!password_string)
   {
      Application->MessageBox("Unable to generate password.", "Key Message", MB_OK|MB_ICONERROR );
      return;
   }
   //apply password to key
   keyMaster->applyOutputPassword(key_record, output_units);

   //Update database
   try
   {
      Database1->StartTransaction();
      AnsiString updated_module_list(reinterpret_cast<char*>(key_record->pkey->keyDataBlock.data),static_cast<unsigned char>(36));

      UtilityQuery->Close();
      UtilityQuery->SQL->Clear();

      if (key_record->non_perm_ktf == true)
         UtilityQuery->SQL->Add("UPDATE SKeyRecord SET SKRoutput = :output_units, modules = :module_list WHERE SKRid = :keyId ");
      else
         UtilityQuery->SQL->Add("UPDATE SKeyRecord SET SKRstatus = 2, SKRoutput = :output_units, modules = :module_list WHERE SKRid = :keyId ");

      UtilityQuery->ParamByName("module_list")->AsBlob = updated_module_list;
      if(key_record->pkey->productId == SPDE_PRODUCT)
              UtilityQuery->ParamByName("output_units")->AsInteger = spde_key->outputUnits;
      else
              UtilityQuery->ParamByName("output_units")->AsInteger = spd_key->outputUnits;
      UtilityQuery->ParamByName("keyId")->AsInteger = key_record->skr_id;
      UtilityQuery->ExecSQL();

      UtilityQuery->SQL->Add("INSERT INTO sTransactionDetail (SKRid, TDpassword, SDRid, TDunits, TDrow_id) values (:key_id, :password, :descriptionid, :units, 0)");
      UtilityQuery->ParamByName("key_id")->AsInteger = key_record->skr_id;
      UtilityQuery->ParamByName("password")->AsString = password_string;
      UtilityQuery->ParamByName("units")->AsInteger = output_units;
      // 300 = Output Pool, 301 = LU Devices
      if(key_record->pkey->productId == SPDE_PRODUCT)
              UtilityQuery->ParamByName("descriptionid")->AsInteger = 5240;
      else
               UtilityQuery->ParamByName("descriptionid")->AsInteger = (key_record->pkey->productId == ICONVERT_PRODUCT) ? 301 : 300;
      UtilityQuery->ExecSQL();

      Database1->Commit();
      RefreshKeyPage(0);
      RefreshKeyPage();
   }
   catch( Exception *e )
   {
      Application->MessageBox( e->Message.c_str(), "Database Failure", MB_OK);
      Database1->Rollback();
   }
}

//==============================================================================
// Function:    createOperatorSessionPassword()
// Purpose:     creates an operator password, and updates the information in the
//              SKeyRecord and STransactionDetail tables based on the key record
//              id number.
// Parameters:  ( int ) - output_units
// Returns:     None
//==============================================================================
void TFCustomerKeys::createOperatorSessionPassword(int operator_sessions)
{
   char password_string[128];

   //create both for now, need to fix copy constructor....
   //check attached key status - need to have a programmed key attached
   SpdeProtectionKey* spde_key((SpdeProtectionKey*)(key_record->pkey));
   //check if key is attached
   if(!isAttachedKeyReady())
      return;

   //generate password
   keyMaster->getOperatorSessionPassword(spde_key, operator_sessions, password_string);

   if(!password_string)
   {
      Application->MessageBox("Unable to generate password.", "Key Message", MB_OK|MB_ICONERROR );
      return;
   }
   //apply password to key
   keyMaster->applyOperatorSessionPassword(key_record, operator_sessions);

   //Update database
   try
   {
      Database1->StartTransaction();

      UtilityQuery->Close();
      UtilityQuery->SQL->Clear();

      if (key_record->non_perm_ktf == true)
         UtilityQuery->SQL->Add("UPDATE SKeyRecord SET SKRoperatorSession = :operator_units WHERE SKRid = :keyId ");
      else
         UtilityQuery->SQL->Add("UPDATE SKeyRecord SET SKRstatus = 2, SKRoperatorSession = :operator_units WHERE SKRid = :keyId ");

      UtilityQuery->ParamByName("operator_units")->AsInteger = spde_key->operatorSessionUnits;
      UtilityQuery->ParamByName("keyId")->AsInteger = key_record->skr_id;
      UtilityQuery->ExecSQL();

      UtilityQuery->SQL->Add("INSERT INTO sTransactionDetail (SKRid, TDpassword, SDRid, TDunits, TDrow_id) values (:key_id, :password, :descriptionid, :units, 0)");
      UtilityQuery->ParamByName("key_id")->AsInteger = key_record->skr_id;
      UtilityQuery->ParamByName("password")->AsString = password_string;
      UtilityQuery->ParamByName("units")->AsInteger = operator_sessions;
      // 400 = Operator Sessions
      UtilityQuery->ParamByName("descriptionid")->AsInteger = 5400;
      UtilityQuery->ExecSQL();

      Database1->Commit();
      RefreshKeyPage(0);
      RefreshKeyPage();
   }
   catch( Exception *e )
   {
      Application->MessageBox( e->Message.c_str(), "Database Failure", MB_OK);
      Database1->Rollback();
   }
}

//==============================================================================
// Function:    createUserSessionsPassword()
// Purpose:     creates an user session password, and updates the information in the
//              SKeyRecord and STransactionDetail tables based on the key record
//              id number.
// Parameters:  ( int ) - output_units
// Returns:     None
//==============================================================================
void TFCustomerKeys::createUserSessionPassword(int user_sessions)
{
   char password_string[128];

   //create both for now, need to fix copy constructor....
   //check attached key status - need to have a programmed key attached
      SpdeProtectionKey* spde_key((SpdeProtectionKey*)(key_record->pkey));
   //check if key is attached
   if(!isAttachedKeyReady())
      return;

   //generate password
   keyMaster->getUserSessionPassword(spde_key, user_sessions, password_string);

   if(!password_string)
   {
      Application->MessageBox("Unable to generate password.", "Key Message", MB_OK|MB_ICONERROR );
      return;
   }
   //apply password to key
   keyMaster->applyUserSessionPassword(key_record, user_sessions);

   //Update database
   try
   {
      Database1->StartTransaction();

      UtilityQuery->Close();
      UtilityQuery->SQL->Clear();

      if (key_record->non_perm_ktf == true)
         UtilityQuery->SQL->Add("UPDATE SKeyRecord SET SKRuserSession = :user_units WHERE SKRid = :keyId ");
      else
         UtilityQuery->SQL->Add("UPDATE SKeyRecord SET SKRstatus = 2, SKRuserSession = :user_units WHERE SKRid = :keyId ");

      UtilityQuery->ParamByName("user_units")->AsInteger = spde_key->userSessionUnits;
      UtilityQuery->ParamByName("keyId")->AsInteger = key_record->skr_id;
      UtilityQuery->ExecSQL();

      UtilityQuery->SQL->Add("INSERT INTO sTransactionDetail (SKRid, TDpassword, SDRid, TDunits, TDrow_id) values (:key_id, :password, :descriptionid, :units, 0)");
      UtilityQuery->ParamByName("key_id")->AsInteger = key_record->skr_id;
      UtilityQuery->ParamByName("password")->AsString = password_string;
      UtilityQuery->ParamByName("units")->AsInteger = user_sessions;
      // 401 = user sessions
      UtilityQuery->ParamByName("descriptionid")->AsInteger = 5401;
      UtilityQuery->ExecSQL();

      Database1->Commit();
      RefreshKeyPage(0);
      RefreshKeyPage();
   }
   catch( Exception *e )
   {
      Application->MessageBox( e->Message.c_str(), "Database Failure", MB_OK);
      Database1->Rollback();
   }
}


//==============================================================================
// Function:    createLUPassword
// Purpose:     This function actually calls createOutputPassword, but for clarity
//              we created another function.
// Parameters:  int - iMaxLU
// Returns:     None
//==============================================================================
void TFCustomerKeys::createMaxLUValuePassword(int iMaxLUValue)
{
   //
   // iMaxLUValue is the output_units passed in.
   createOutputPassword(iMaxLUValue);
}

//==============================================================================
// Function:    mmLostClick
// Purpose:     Event handler for the main menu options Lost.  Will log a key
//              as lost in the database.
// Parameters:  ( TObject* ) - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::mmLostClick(TObject *Sender)
{
   int key_status = QueryKey->FieldValues["SKRstatus"];

   key_status |= S_LOST;

   try
   { //update SKeyRecord
   	Database1->StartTransaction();
   	//--update key with new version number
   	UtilityQuery2->Close();
   	UtilityQuery2->SQL->Clear();
   	UtilityQuery2->SQL->Add("UPDATE SKeyRecord SET SKeyRecord.SKRstatus = :newStatus WHERE SKeyRecord.SKRid = :id");
   	UtilityQuery2->ParamByName("id")->AsInteger = key_record->skr_id;
   	UtilityQuery2->ParamByName("newStatus")->AsInteger = key_status;
   	UtilityQuery2->ExecSQL();

      Database1->Commit();
      RefreshKeyPage(0);

   }
   catch( Exception *e )
   {
   	Application->MessageBox( e->Message.c_str(), "Database Failure", MB_OK);
      Database1->Rollback();
   }
}

//==============================================================================
// Function:    mmReturnedClick
// Purpose:     Event handler for main menu Return option.  Logs a key as returned
//              in the database.
// Parameters:  ( TObject* ) - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::mmReturnedClick(TObject *Sender)
{
   int key_status = QueryKey->FieldValues["SKRstatus"];

   key_status |= S_RETURNED;

   try
   { //update SKeyRecord
   	Database1->StartTransaction();
   	//--update key with new version number
   	UtilityQuery2->Close();
   	UtilityQuery2->SQL->Clear();
   	UtilityQuery2->SQL->Add("UPDATE SKeyRecord SET SKeyRecord.SKRstatus = :newStatus WHERE SKeyRecord.SKRid = :id");
   	UtilityQuery2->ParamByName("id")->AsInteger = key_record->skr_id;
   	UtilityQuery2->ParamByName("newStatus")->AsInteger = key_status;
   	UtilityQuery2->ExecSQL();

        Database1->Commit();
        RefreshKeyPage(0);

   }
   catch( Exception *e )
   {
   	Application->MessageBox( e->Message.c_str(), "Database Failure", MB_OK);
        Database1->Rollback();
   }
}

//==============================================================================
// Function:    mmDeactivateClick()
// Purpose:     Reads the attached key and checks it against selected row, if
//              equal the key is cleared and the database is updated.
// Parameters:  ( TObject* ) - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::mmDeactivateClick(TObject *Sender)
{
   TDateTime curTime = TDateTime::CurrentDate();

   if( !QueryKey->RecordCount ) // there are no keys
   	return;

   SKeyRecord insKeyrec;  // key connected to the pc
   if( keyMaster->read(&insKeyrec))
   {  //try to read info from key
      Application->MessageBox("Can not read attached key.", "Key Messsage", MB_OK|MB_ICONERROR );
      return;
   }
   //setup string
   if( (QueryKey->FieldValues["SCRnumber"] != insKeyrec.getCustomerNumber()) ||
       (QueryKey->FieldValues["SKRnumber"] != insKeyrec.getKeyNumber()) )
   {
      if( insKeyrec.getCustomerNumber() )
      {
         sprintf(scratch, "Key #%3X-%2X must be selected to be deactivated.", insKeyrec.pkey->customerNumber, insKeyrec.pkey->keyNumber );
      	 Application->MessageBox(scratch, "Key Message", MB_OK );
      }
      else
      {
      	Application->MessageBox("Key is not programmed", "Key Message", MB_OK|MB_ICONWARNING );
      }
      return;
   }
   sprintf(scratch, "Are you sure you want to\ndeactivate key #%03X-%02X?", insKeyrec.pkey->customerNumber, insKeyrec.pkey->keyNumber );
   if( IDYES == Application->MessageBox(scratch, "Key Message", MB_YESNO|MB_ICONWARNING) )
   {
      Variant vrInteger( QueryKey->FieldValues["SKRstatus"] );
      if( !keyMaster->deactivate() )
      {
   	 try
         {
      		Database1->StartTransaction();
   		//int recordId = QueryKey->FieldValues["SKRid"];

   		UtilityQuery->Close();
      		UtilityQuery->SQL->Clear();
      		UtilityQuery->SQL->Add("UPDATE SKeyRecord SET SKRdeactivationDate = :today, SKRstatus = 11 WHERE SKRid = :id ");
      		UtilityQuery->ParamByName("today")->AsString = curTime.DateString();
      		UtilityQuery->ParamByName("id")->AsInteger = key_record->skr_id;
      		UtilityQuery->ExecSQL();

		Database1->Commit();
      		RefreshKeyPage();
      	 }
   	 catch( Exception *e )
         {
         	Database1->Rollback();
         	Application->MessageBox(e->Message.c_str(), "Key Message", MB_OK);
   	 }
      }
   }

   RefreshKeyPage(0);//re-queries database for key information
}

//==============================================================================
// Function:    mmReprogramClick
// Purpose:
// Parameters:  ( TObject* ) - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::mmReprogramClick(TObject *Sender)
{
   //verify that same key is attached....
   SKeyRecord *attached_key = new SKeyRecord();  // key connected to the pc

   //try to read info from key
   if( keyMaster->read(attached_key))
   {
      //Application->MessageBox("Could not deactivate the key.", "Key Message", MB_OK|MB_ICONERROR);
      if(attached_key)
   	 delete(attached_key);
      return;
   }
   //verify attached key is selected in creator
   if( key_record->pkey->customerNumber != attached_key->pkey->customerNumber ||
       key_record->pkey->keyNumber != attached_key->pkey->keyNumber )
   {
      //Application->MessageBox("Key is not programmed", "Key Message", MB_OK|MB_ICONWARNING );
      if(attached_key)
   	 delete(attached_key);
      return;
   }

   //
//Create a wizard
   TKeyWizardFrm* wizard = new TKeyWizardFrm(key_record, this);

   while(1)
   {
      //override default behavior for reprogramming
      wizard->ProductComboBox->Enabled = false;
      wizard->keyTypeComboBox->Enabled = false;
      wizard->ui_version->Enabled = false;
      wizard->NextBtn->Enabled = false;
      wizard->ProgramBtn->Enabled = true;

      //continue until key is programmed or user cancels action
      if( wizard->ShowModal() == mrOk )
      {
         //key_record should contain all information needed to program key
         //if programming is successful update record information in the database
         //create temp. key to represent customer key
         if( keyMaster->programmed() && keyMaster->found() )
         {
            //key is attached and unprogrammed, program key and save action in database
            if( keyMaster->program(key_record) == 0 )
            {
               //key programmed successfully save action in database
               if( dbSaveKey(key_record, wizard) == true )
               {
                  break;
               }
            }
         }
         //check a protection key is attached
         else if( !keyMaster->found() )
            Application->MessageBox("No key attached to pc.", "Information", MB_OK);
         else
            Application->MessageBox("Attached key is already programmed.", "Information", MB_OK);
      }
      //user canceled action, break out of while
      else
         break;

   }

   delete wizard;
   RefreshKeyPage();

   //reset how many days the key is programmed for
   //check that same key is attached
   //reset trial counter

   //database -  reset activation date and expiration date
}

//==============================================================================
// Function:    mmRemoveClick()
// Purpose:     This function removes all data from the database.  Note that all
//              information about the key will be lost.  This information is
//              deleted from SKeyRecord and STransactionDetail tables in the
//              database.
// Parameters:  TObject* - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::mmRemoveClick(TObject *Sender)
{
   //should be getting key number string through function...
   sprintf( scratch,"Removing key #%s-%s from the database.  All information will be lost.",
   	    IntToHex(key_record->pkey->customerNumber, 3),
            IntToHex(key_record->pkey->keyNumber, 2) );

   if( Application->MessageBox( scratch, "Warning", MB_OKCANCEL|MB_ICONWARNING) == IDOK )
   {
      try
      {
      	Database1->StartTransaction();


         //delete the corresponding entry in the sPasswordPacketRelationship table
         UtilityQuery->Close();
         UtilityQuery->SQL->Clear();
         UtilityQuery->SQL->Add("DELETE FROM spasswordpacketrelationship WHERE password_id IN (SELECT TDid FROM stransactiondetail WHERE SKRid = :skid)");
         UtilityQuery->ParamByName("skid")->AsInteger = key_record->skr_id;
   	   UtilityQuery->ExecSQL();

         //
         // remove all password related the current key
         UtilityQuery->Close();
   	   UtilityQuery->SQL->Clear();
         UtilityQuery->SQL->Add("DELETE FROM sTransactionDetail WHERE SKRid = :id");
         UtilityQuery->ParamByName("id")->AsInteger = key_record->skr_id;
   	   UtilityQuery->ExecSQL();

         //
         // remove all modules related the current key
         UtilityQuery->Close();
   	 UtilityQuery->SQL->Clear();
   	 UtilityQuery->SQL->Add("DELETE FROM SKeyRecord WHERE SKRid = :id");
         UtilityQuery->ParamByName("id")->AsInteger = key_record->skr_id;
   	 UtilityQuery->ExecSQL();

         Database1->Commit();
      }
      catch( Exception *e )
      {
          //
          //error has occurred while trying to commit() or delete records
          Application->MessageBox( e->Message.c_str(), "Database Failure", MB_OK);
          Database1->Rollback();
      }
      try
      {
          QueryKey->FindPrior();
      }
      catch(...)
      {
      	// prior key does not exist
      }
   }
   RefreshKeyPage();
}

//==============================================================================
// Function:    mmReserveClick()
// Purpose:     Reserves a key number, for a key to be programmed later.  This
//              is necessary for the current product process to work.
// Parameters:  ( TObject* ) - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::mmReserveClick(TObject *Sender)
{
   //
   // this will eventually call a dialog but for now defaults to creating a spdnt base key
   try
   {
      StoredProc1->Close();
      StoredProc1->ParamByName("@customer_id")->AsInteger = key_record->getCustomerNumber();
      StoredProc1->ParamByName("@product_type")->AsInteger = 0; //SPD
      StoredProc1->ParamByName("@key_type")->AsInteger = ktfReserved;     //None - defaults to base
      StoredProc1->Prepare();
      StoredProc1->ExecProc();
   }
   catch(EDBEngineError &e)
   {
      Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
   }

   // refresh key grid to reflect changes...

   RefreshKeyPage(0);
}

//==============================================================================
// Function:    AsciiToBSTR()
// Purpose:     Converts an ascii string to a BSTR. Caller must free
//		BSTR memory with a call to SysFreeString().
// Parameters:  ( const char* ) - pStr
// Returns:     BSTR
//==============================================================================
BSTR TFCustomerKeys::AsciiToBSTR(const char* pStr)
{
	WCHAR pwStr[255];
	::MultiByteToWideChar(CP_ACP, 0, pStr, -1, pwStr, 255/*MAX_PATH*/);
	return SysAllocString(pwStr);
}

//==============================================================================
// Function:    mmUpdateVersionClick()
// Purpose:
// Parameters:  (TObject*) - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::mmUpdateVersionClick(TObject *Sender)
{
   TDlgVersion *dlg = new TDlgVersion(this);
   dlg->setDialog(0,0,true);
   dlg->ShowModal();

   delete dlg;
}

//==============================================================================
// Function:    getKeyTypeText()
// Purpose:
// Parameters:  int - key_type
// Returns:     None
//==============================================================================
AnsiString TFCustomerKeys::getKeyTypeText(int key_type)
{
   switch(key_type)
   {
        case ktfNone:
           return "None";
        case ktfAddon:
           return "Add-on";
        case ktfBase:
           return "Base";
        case ktfReplacement:
           return "Replacement";
        case ktfRelicense:
           return "Relicense";
        case ktfInventory:
           return "Inventory";
        case ktfBackup:
           return "Backup";
        case ktfEmergency:
           return "Emergency";
        case ktfCustom:
           return "Custom";
        case ktfLoan:
           return "Loan";
        case ktfDemo:
           return "Demo";
        case ktfRental:
           return "Rental";
        case ktfDevelopment:
           return "Test/Dev";
        case ktfReserved:
           return "RESERVED";
        default:
           return "Unknown";
   }
}


//==============================================================================
// Function:    ToBaseClick()
// Purpose:
// Parameters:  TObject* - Sender
// Returns:     None
//==============================================================================
//should only be called on a selected row
void __fastcall TFCustomerKeys::ToBaseClick(TObject *Sender)
{
   if(key_record->getKeyTypeIntegerValue(key_record->pkey->keyType) != ktfReserved)
   {
      try
      {
         sp_GenerateSystemId->Close();
         sp_GenerateSystemId->Prepare();
         sp_GenerateSystemId->ExecProc();

         int system_id(sp_GenerateSystemId->ParamByName("RETURN_VALUE")->AsInteger);

         UtilityQuery->Close();
         UtilityQuery->SQL->Clear();
         UtilityQuery->SQL->Add("update sKeyRecord set KTid = :key_type, SDid = :system_id where skrid = :skrid");
         UtilityQuery->ParamByName("key_type")->AsInteger = ktfBase;
         UtilityQuery->ParamByName("system_id")->AsInteger = system_id;
         UtilityQuery->ParamByName("skrid")->AsInteger = key_record->skr_id;
         UtilityQuery->Prepare();
         UtilityQuery->ExecSQL();

         RefreshKeyPage(0);
      }
      catch(EDBEngineError &e)
      {
         Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
      }
   }
}

//==============================================================================
// Function:    PswdGridKeyDown()
// Purpose:
// Parameters:  TObject* - Sender
//              WORD - &Key
//              TShiftState - Shift
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::PswdGridKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   TShiftState shift_state;
   shift_state << ssCtrl;
   //
   //check for copy command
   if( Shift == shift_state && Key == 'C')
   {
      int counter(1); //used to label passwords
      TBookmark tmpMark;
      AnsiString password_list;

      Clipboard()->Open();
      try
      {
         tmpMark = PasswordQuery->GetBookmark();
   		PasswordQuery->DisableControls();

      	 PasswordQuery->First();
      	 while( !PasswordQuery->Eof )
         {
       	    if( PswdGrid->SelectedRows->CurrentRowSelected )
            {
                password_list += Format("\t\tPASSWORD#%d\t%s\t<ENTER>\n",OPENARRAY(TVarRec,(counter, PasswordQuery->FieldByName("TDpassword")->AsString)));
            	counter++;
            }
            PasswordQuery->Next();
      	 }

         if(!password_list.IsEmpty())
            Clipboard()->AsText = password_list;

      	 PasswordQuery->GotoBookmark( tmpMark );
      	 PasswordQuery->FreeBookmark( tmpMark );
      	 PasswordQuery->EnableControls();
      }
      catch (...)
      {
         Clipboard()->Close();
         throw;
      }

      Clipboard()->Close();
   }
}
//==============================================================================
// Function:    PKNumberEditBoxChange()
// Purpose:     Event handler for the PK Number Edit box
// Parameters:  (TObject*) - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::PKNumberEditBoxChange(TObject *Sender)
{
   UpdatePKNumber->Enabled = true;
}
//==============================================================================
// Function:    eBONumberEditBoxChange()
// Purpose:     Event handler for the eBO Number Edit box
// Parameters:  (TObject*) - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::eBONumberEditBoxChange(TObject *Sender)
{
   UpdateeBONumber->Enabled = true;
}
//==============================================================================
// Function:    DescriptionMemoChange()
// Purpose:     Event handler for the Description Edit box
// Parameters:  (TObject*) - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::DescriptionMemoChange(TObject *Sender)
{
   UpdateDescription->Enabled = true;
}

//==============================================================================
// Function:    UpdatePKNumberClick()
// Purpose:     Updates the PK number by calling the lookup public member function
//              updateKeyPKNumber which will update SKeyRecord in the database.
// Parameters:  (TObject*) - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::UpdatePKNumberClick(TObject *Sender)
{
    //get the text currently in the edit box
    key_record->po = PKNumberEditBox->Text.ToInt();

    lookup->updateKeyPKNumber(key_record->skr_id, static_cast<int>(strtol(PKNumberEditBox->Text.c_str(), &endptr, 10)));
    RefreshKeyPage();
    UpdatePKNumber->Enabled = false;
}

//==============================================================================
// Function:    UpdateeBONumberClick
// Purpose:     Updates the epicore back office number related to the key in the
//              database (key_record->eBOnumber) based on the key record id.
// Parameters:  TObject * Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::UpdateeBONumberClick(TObject *Sender)
{
   //update the description in the database

   //get the text currently in the edit box
   key_record->eBOnumber = eBONumberEditBox->Text;

   //update the Description View to the new value given by the user
   lookup->updateKeyeBONumber(key_record->skr_id, key_record->eBOnumber);
   RefreshKeyPage();
   UpdateeBONumber->Enabled = false;
}

//==============================================================================
// Function:    UpdateDescriptionClick()
// Purpose:     Updates the description of the key by calling the lookup public
//              member function updateKeyDescription which will update SKeyRecord
//              in the database.
// Parameters:  (TObject*) - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::UpdateDescriptionClick(TObject *Sender)
{
    //
    //get the text currently in the edit box and update the structure
    key_record->summary = DescriptionMemo->Text;

    //
    //update the Description View to the new value given by the user
    lookup->updateKeyDescription(key_record->skr_id, DescriptionMemo->Text);
    RefreshKeyPage();
    UpdateDescription->Enabled = false;
}

//==============================================================================
// Function:    KeyFormModuleFramemmIncrementModuleClick()
// Purpose:
// Parameters:  TObject* - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::KeyFormModuleFramemmIncrementModuleClick(
      TObject *Sender)
{
    KeyFormModuleFrame->mmIncrementModuleClick(Sender);
}

//==============================================================================
// Function:    TabSheetModulesEnter()
// Purpose:     Sets the PoolLabel to display whether the pool value is an
//              output pool for SPD products or LU counter for ICONVERT products
// Parameters:  TObject* - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::TabSheetModulesEnter(TObject *Sender)
{
   PoolLabel->Caption = (key_record->getProductId() == ICONVERT_PRODUCT) ? "Max LU's Licensed:" : "Pool:";
   mmMaxLU->Visible = (key_record->getProductId() == ICONVERT_PRODUCT) ? true : false;
}

//==============================================================================
// Function:    mmMaxLUClick()
// Purpose:     Allows the user to create a password for the number of LU's.
//              This allows the user to change the values for the customer if
//              they decide they want more/less LU's.
// Parameters:  TObject* - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::mmMaxLUClick(TObject *Sender)
{
   Sender;
   int iMaxLUValue(0);

   //get new total of LU's
   TUnitsDlg *dlg = new TUnitsDlg(this, 0xFFF);
   dlg->Edit1->Text = 1;

   if( dlg->ShowModal() == IDYES )
   {
      iMaxLUValue = dlg->Edit1->Text.ToInt();
      //check if invalid values returned
      createMaxLUValuePassword(iMaxLUValue);
   }

   // clean up off of the heap
   delete(dlg);

}

//==============================================================================
// Function:    TabSheetModulesExit()
// Purpose:     Disables the mmMaxLU option before moving to the next tab.
// Parameters:  TObject* - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::TabSheetModulesExit(TObject *Sender)
{
    mmMaxLU->Visible = false;
}



//==============================================================================
// Function:    KeyFormModuleFramePagesPerMinute1Click()
// Purpose:     calls mmPagesPerMinuteClick
// Parameters:  TObject* - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::KeyFormModuleFramePagesPerMinute1Click(
      TObject *Sender)
{
    KeyFormModuleFrame->mmPagesPerMinuteClick(Sender);
}

//==============================================================================
// Function:    KeyFormModuleFramemmPagesPerMinuteClick()
// Purpose:     calls mmPagesPerMinuteClick
// Parameters:  TObject* - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::KeyFormModuleFramemmPagesPerMinuteClick(
      TObject *Sender)
{
    KeyFormModuleFrame->mmPagesPerMinuteClick(Sender);
}

//==============================================================================
// Function:    KeyGridTitleClick()
// Purpose:     Requeries the key data and sorts according to the column title
//              the user clicks.
// Parameters:  TObject* - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::KeyGridTitleClick(TColumn *Column)
{
    //
    // there are no keys
    if( !QueryKey->RecordCount )
   	return;

    bool gFOUND = false;
    int skrid;

    //
    // Get the current sql_statement member variable.
    AnsiString my_sql_statement = getQueryKeySQLStatement();

    //
    // if not empty before refresh then save current key otherwise, set to 0
    skrid = ( QueryKey->RecordCount ) ? QueryKey->FieldValues["SKRid"] : 0;

    //
    // Add ordering by the column clicked.
    if (Column->FieldName == "SCRnumber")
        my_sql_statement += key_sort_type[SORT_CUSTOMER];
    else if (Column->FieldName ==  "SKRnumber")
        my_sql_statement += key_sort_type[SORT_KEY];
    else if (Column->FieldName ==  "SKRproduct")
        my_sql_statement += key_sort_type[SORT_PRODUCT];
    else if (Column->FieldName == "SKRversion")
        my_sql_statement += key_sort_type[SORT_VERSION];
    else if (Column->FieldName ==  "e_date")
        my_sql_statement += key_sort_type[SORT_EXPIRATION];
    else if (Column->FieldName == "a_date")
        my_sql_statement += key_sort_type[SORT_ACTIVATION];
    else if (Column->FieldName ==  "d_date")
        my_sql_statement += key_sort_type[SORT_DEACTIVATION];
    else if (Column->FieldName ==  "KTid")
        my_sql_statement += key_sort_type[SORT_KEYTYPE];
    else if (Column->FieldName == "SKRstatus")
        my_sql_statement += key_sort_type[SORT_STATUS];
    else if (Column->FieldName == "SDid")
        my_sql_statement += key_sort_type[SORT_SYSTEM];

    try
    {
        QueryKey->DisableControls();
        QueryKey->Close();
        QueryKey->SQL->Clear();
        QueryKey->SQL->Add(my_sql_statement);
        QueryKey->Open();

        if( QueryKey->RecordCount )
            QueryKey->First();

        //
        // Search for the key you were previously on
        while(!QueryKey->Eof && !gFOUND)
        {
            if( QueryKey->FieldValues["SKRid"] == skrid )
      	        gFOUND = true;
            else
                QueryKey->Next();
        }

        //
        //if not found set to first key
        if( !gFOUND )
            QueryKey->First();

        QueryKey->EnableControls();
    }
    catch(...){}

    //
    // Update the information
    OnKeyRowChange(this);
}

//==============================================================================
// Function:    PswdGridDrawColumnCell()
// Purpose:     Draws the column cell event. So far this is only used for the
//              Test/Development keys. Since they add extra values.
// Parameters:
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::PswdGridDrawColumnCell(TObject *Sender,
                const TRect &Rect, int DataCol, TColumn *Column,TGridDrawState State)
{
    //
    // Check if the key is a Test/Dev Key
    int idx;
    if (key_record->pkey->keyType == KEYDevelopment)
    {
        if (Column->FieldName == "SDRname")
        {
            //
            // If it is an extension value (which is what a Test/Dev extension
            // password really is), add the index value
            if( Column->Field->DisplayText == "Extend Trial Period" )
            {
               int index = PswdGrid->Columns->Items[2]->Field->AsInteger - 1; //Subtact 1 to get proper value in the KDPasswordText array.
               PswdGrid->Canvas->FillRect( Rect );
               sprintf( scratch, "Extend Trial Period - %s", key_record->pkey->getKDPasswordText(index)); //display unsigned integer as text in the description.
               PswdGrid->Canvas->TextOut( Rect.Left+2, Rect.Top+2, scratch);
            }
        }
    }
}


//==============================================================================
// Function:    mmSearchbyKeyTypeClick()
// Purpose:     Allows the user to search the database by key type.
// Parameters:  TObject* - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::mmSearchByTypeClick(TObject *Sender)
{
    TksdSearchDlg *dlg = new TksdSearchDlg(this);
    //
    // Set the dialog options
    dlg->ksdKeyTypeComboBox->Enabled = true;
    dlg->ksdKeyTypeComboBox->TabOrder = 0;
    dlg->ksdSearchButton->TabOrder = 1;
    dlg->ksdCancelButton->TabOrder = 2;

    //
    // Disable all other controls
    dlg->ksdCustomerMaskEdit->Enabled = false;
    dlg->ksdKeyMaskEdit->Enabled = false;
    dlg->ksdVersionMaskEdit->Enabled = false;
    dlg->ksdLicenseComboBox->Enabled = false;

    if( dlg->ShowModal() == mrOk )
    {
        searchKeyType(dlg->keyType);
    }

    // Clean up
    delete dlg;
    dlg = NULL;
}
//==============================================================================
// Function:    searchKeyType()
// Purpose:     This will allow the user to search the database by key type
//              This should aide users in house with being able to find customers
//              that might have a certain key type without having to search the
//              database, etc.
// Parameters:  TObject* - Sender
//              String   - customerKeyTypeSearchValue
// Returns:     None
//==============================================================================
void TFCustomerKeys::searchKeyType(String customerKeyTypeSearchValue)
{
    int index(-1);
    bool repaintflag(false);

    //
    // Check the value to indicate what type of search we will be performing
    if (customerKeyTypeSearchValue == "RESERVED")
       index = ktfReserved;
    else if (customerKeyTypeSearchValue == "None")
       index = ktfNone;
    else if (customerKeyTypeSearchValue == "Add-on")
       index = ktfAddon;
    else if (customerKeyTypeSearchValue == "Base")
       index = ktfBase;
    else if (customerKeyTypeSearchValue == "Replacement")
       index = ktfReplacement;
    else if (customerKeyTypeSearchValue == "Relicense")
       index = ktfRelicense;
    else if (customerKeyTypeSearchValue == "Inventory")
       index = ktfInventory;
    else if (customerKeyTypeSearchValue == "Backup")
       index = ktfBackup;
    else if (customerKeyTypeSearchValue == "Emergency")
       index = ktfEmergency;
    else if (customerKeyTypeSearchValue == "Custom")
       index = ktfCustom;
    else if (customerKeyTypeSearchValue == "Loan")
       index = ktfLoan;
    else if (customerKeyTypeSearchValue == "Demo")
       index = ktfDemo;
    else if (customerKeyTypeSearchValue == "Rental")
       index = ktfRental;
    else if (customerKeyTypeSearchValue == "Test/Dev")
       index = ktfDevelopment;
    else
       index = -1;

    try
    {
        AnsiString my_sql_statement;

        QueryKey->DisableControls();
        QueryKey->Close();
        QueryKey->SQL->Clear();
        //
        // Set query w/o ordering
        my_sql_statement = "SELECT *, CONVERT(char(12),SKRactivationDate,7)As a_date, CONVERT(char(12),SKRdeactivationDate,7) As d_date, CONVERT(char(12),SKRexpirationDate,7) As e_date  FROM SKeyRecord JOIN SCustomerRecord ON SKeyRecord.SCRnumber = SCustomerRecord.SCRnumber where SKeyRecord.KTid = ";
        my_sql_statement += index;
        //
        // Set the member variable for future queries
        setQueryKeySQLStatement(my_sql_statement);
        //
        // Set the ordering for the query
        my_sql_statement += " ORDER BY SCustomerRecord.SCRnumber, skrnumber";
        //
        // Perform the query
        QueryKey->SQL->Add(my_sql_statement);
        QueryKey->Open();
        QueryKey->First();
        QueryKey->EnableControls();

        //
        // If there are no records found
        if( !QueryKey->RecordCount )
        {
            //
            // Update the grid to show no information
            Application->MessageBox( "No match found.", "Search", MB_OK|MB_ICONINFORMATION);
            repaintflag = true;
            KeyGrid->Repaint();
        }


    }
    catch( Exception &e )
    {
        Application->MessageBox( e.Message.c_str(), "Database Failure", MB_OK );
    }

    //
    // If records are found, update grid to show information
    if (!repaintflag)
    {
        OnKeyRowChange(this);
        RefreshKeyPage();
    }
}

//==============================================================================
// Function:    mmSearchByCustomerNumberClick()
// Purpose:     Allows the user to search the database by customer number.
// Parameters:  TObject* - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::mmSearchByCustomerNumberClick(
      TObject *Sender)
{
    TksdSearchDlg *dlg = new TksdSearchDlg(this);

    //
    // Set the Dialog Options
    dlg->ksdCustomerMaskEdit->Enabled = true;
    dlg->ksdCustomerMaskEdit->TabOrder = 0;
    dlg->ksdSearchButton->TabOrder = 1;
    dlg->ksdCancelButton->TabOrder = 2;

    //
    // Disable all other fields.
    dlg->ksdKeyMaskEdit->Enabled = false;
    dlg->ksdVersionMaskEdit->Enabled = false;
    dlg->ksdLicenseComboBox->Enabled = false;
    dlg->ksdKeyTypeComboBox->Enabled = false;

    if( dlg->ShowModal() == mrOk )
    {
        searchCustomerNumber(dlg->customerNumber);
    }

    // Clean Up
    delete dlg;
    dlg = NULL;

}

//==============================================================================
// Function:    searchCustomerNumber()
// Purpose:     This will allow the user to search the database by version number
//              This should aide users in house with being able to find customers
//              that might have a certain version without having to search the
//              database, etc.
// Parameters:  TObject* - Sender
//              String   - customerNumberSearchValue
// Returns:     None
//==============================================================================
void TFCustomerKeys::searchCustomerNumber(String customerNumberSearchValue)
{
    bool repaintflag(false);
    AnsiString my_sql_statement;
    unsigned short usCustomerNumber;

    sprintf(scratch, "%s", customerNumberSearchValue);
    usCustomerNumber = static_cast<unsigned short>(strtol(scratch, &endptr, 16));

    try
    {
        QueryKey->Close();
   	QueryKey->SQL->Clear();

        //
        // Set query w/o ordering
        my_sql_statement = "SELECT *, CONVERT(char(12),SKRactivationDate,7)As a_date, CONVERT(char(12),SKRdeactivationDate,7) As d_date, CONVERT(char(12),SKRexpirationDate,7) As e_date  FROM SKeyRecord JOIN SCustomerRecord ON SKeyRecord.SCRnumber = SCustomerRecord.SCRnumber WHERE SKeyRecord.SCRnumber = ";
        my_sql_statement += usCustomerNumber;
        //
        // Set the private member variable for future queries
        setQueryKeySQLStatement(my_sql_statement);
        //
        // Set the ordering for the query
        my_sql_statement += key_sort_type[SORT_KEY];
        //
        // Perform the query
        QueryKey->SQL->Add(my_sql_statement);
        QueryKey->Open();
        QueryKey->First();

        //
        // If there are no records found
        if( !QueryKey->RecordCount )
        {
            //
            // Update the grid to reflect that
      	    Application->MessageBox( "No match found.", "Search", MB_OK|MB_ICONINFORMATION);
            repaintflag = true;
            KeyGrid->Repaint();
        }

        //
        // Otherwise go to the first item
        QueryKey->First();

    }
    catch( Exception &e )
    {
        Application->MessageBox( e.Message.c_str(), "Database Failure", MB_OK );
    }

    //
    // Update the grid to show the information if there are records
    if (!repaintflag)
    {
        OnKeyRowChange(this);
        RefreshKeyPage();
    }
}

//==============================================================================
// Function:    mmSearchByLicenseClick()
// Purpose:     Allows the user to search the database by license.
// Parameters:  TObject* - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::mmSearchByLicenseClick(TObject *Sender)
{
    TksdSearchDlg *dlg = new TksdSearchDlg(this);

    //
    // Set the dialog options
    dlg->ksdLicenseComboBox->Enabled = true;
    dlg->ksdLicenseComboBox->TabOrder = 0;
    dlg->ksdSearchButton->TabOrder = 1;
    dlg->ksdCancelButton->TabOrder = 2;

    //
    // Disable all other fields.
    dlg->ksdKeyMaskEdit->Enabled = false;
    dlg->ksdCustomerMaskEdit->Enabled = false;
    dlg->ksdVersionMaskEdit->Enabled = false;
    dlg->ksdKeyTypeComboBox->Enabled = false;

    if( dlg->ShowModal() == mrOk )
    {
        searchLicense(dlg->license);
    }

    // Clean up
    delete dlg;
    dlg = NULL;
}
//==============================================================================
// Function:    searchLicense()
// Purpose:     This will allow the user to search the database by version number
//              This should aide users in house with being able to find customers
//              that might have a certain version without having to search the
//              database, etc.
// Parameters:  TObject* - Sender
//              String   - customerNumberSearchValue
// Returns:     None
//==============================================================================
void TFCustomerKeys::searchLicense(String customerLicenseSearchValue)
{
    int index(-1);
    bool repaintflag(false);

    //
    // Check the value to see what type of query we will perform.
    if (customerLicenseSearchValue == "Initial")
            index = 0;
    else if (customerLicenseSearchValue == "Extension 1")
            index = 1;
    else if (customerLicenseSearchValue == "Permanent")
            index = 2;
    else if (customerLicenseSearchValue == "Unitialized")
            index = 3;
    else if (customerLicenseSearchValue == "Extension 2")
            index = 4;
    else if (customerLicenseSearchValue == "Extension 3")
            index = 5;
    else if (customerLicenseSearchValue == "Extension 4")
            index = 6;
    else if (customerLicenseSearchValue == "Extension 5")
            index = 7;
    else if (customerLicenseSearchValue == "Unused")
            index = 10;
    else if (customerLicenseSearchValue == "Deactivated")
            index = 11;
    else if (customerLicenseSearchValue == "Returned")
            index = 200;
    else if (customerLicenseSearchValue == "Lost")
            index = 500;
    else
            index = -1;

    try
    {
        //
        // This is the revised sql statement with ordering
        AnsiString my_sql_statement;
        QueryKey->Close();
   	QueryKey->SQL->Clear();
        switch (index)
        {
           // Check for "Returned" keys
           case 200:
                //
                // Set the query without ordering
                my_sql_statement = "SELECT *, CONVERT(char(12),SKRactivationDate,7)As a_date, CONVERT(char(12),SKRdeactivationDate,7) As d_date, CONVERT(char(12),SKRexpirationDate,7) As e_date  FROM SKeyRecord JOIN SCustomerRecord ON SKeyRecord.SCRnumber = SCustomerRecord.SCRnumber where SKeyRecord.SKRstatus > 255 and SKeyRecord.SKRstatus < 268";
                //
                // Set the private member variable for future queries
                setQueryKeySQLStatement(my_sql_statement);
                //
                // Add ordering
                my_sql_statement += key_sort_type[SORT_CUSTOMER];
                //
                // Perform the query
                QueryKey->SQL->Add(my_sql_statement);
                break;

           // Check for "Lost" Keys
           case 500:
                //
                // Set the query w/o ordering
                my_sql_statement = "SELECT *, CONVERT(char(12),SKRactivationDate,7)As a_date, CONVERT(char(12),SKRdeactivationDate,7) As d_date, CONVERT(char(12),SKRexpirationDate,7) As e_date  FROM SKeyRecord JOIN SCustomerRecord ON SKeyRecord.SCRnumber = SCustomerRecord.SCRnumber where SKeyRecord.SKRstatus > 512 and SKeyRecord.SKRstatus < 524";
                //
                // Set the private member variable for future queries
                setQueryKeySQLStatement(my_sql_statement);
                //
                // Add ordering
                my_sql_statement += key_sort_type[SORT_CUSTOMER];
                //
                // Perform the query
                QueryKey->SQL->Add(my_sql_statement);
                break;

           // All other keys
           default:
                //
                // Set the query without ordering
                my_sql_statement = "SELECT *, CONVERT(char(12),SKRactivationDate,7)As a_date, CONVERT(char(12),SKRdeactivationDate,7) As d_date, CONVERT(char(12),SKRexpirationDate,7) As e_date  FROM SKeyRecord JOIN SCustomerRecord ON SKeyRecord.SCRnumber = SCustomerRecord.SCRnumber where SKeyRecord.SKRstatus = ";
                my_sql_statement += index;
                //
                // Set the private member variable for future queries
                setQueryKeySQLStatement(my_sql_statement);
                //
                // Add ordering
                my_sql_statement += key_sort_type[SORT_CUSTOMER];
                //
                // Perform the query
                QueryKey->SQL->Add(my_sql_statement);
                break;
        }

        QueryKey->Open();
        QueryKey->First();

        //
        // If there are no records found
        if( !QueryKey->RecordCount )
        {
                //
                // Repaint the grid with no information
      	        Application->MessageBox( "No match found.", "Search", MB_OK|MB_ICONINFORMATION);
                repaintflag = true;
                KeyGrid->Repaint();
        }

        QueryKey->First();

    }
    catch( Exception &e )
    {
        Application->MessageBox( e.Message.c_str(), "Database Failure", MB_OK );
    }

    //
    // If something was found, update the grid with that information
    if (!repaintflag)
    {
        OnKeyRowChange(this);
        RefreshKeyPage();
    }
}

//==============================================================================
// Function:    mmSearchByKeyNumberClick()
// Purpose:
// Parameters:  TObject* - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::mmSearchByKeyNumberClick(TObject *Sender)
{
    TksdSearchDlg *dlg = new TksdSearchDlg(this);

    //
    // Set the dialog options
    dlg->ksdKeyMaskEdit->Enabled = true;
    dlg->ksdKeyMaskEdit->TabOrder = 0;
    dlg->ksdSearchButton->TabOrder = 1;
    dlg->ksdCancelButton->TabOrder = 2;

    // Disable All Other Fields
    dlg->ksdVersionMaskEdit->Enabled = false;
    dlg->ksdCustomerMaskEdit->Enabled = false;
    dlg->ksdLicenseComboBox->Enabled = false;
    dlg->ksdKeyTypeComboBox->Enabled = false;

    if( dlg->ShowModal() == mrOk )
    {
        customerKeyNumber = dlg->keyNumber;
        searchKeyNumber(dlg->keyNumber);
    }

    // Clean up
    delete dlg;
    dlg = NULL;
}
//==============================================================================
// Function:    searchKeyNumber
// Purpose:     Takes the field from the Key Search Dialog and searches for
//              that specific key number. in the database.
// Parameters:  String keyNumberSearchValue
// Returns:     None
//==============================================================================
void TFCustomerKeys::searchKeyNumber(String keyNumberSearchValue)
{
    char *cpCustomer;
    char *cpKey;
    char tokens[] = "-";
    int iCustomerNum, iCustomerKey;
    bool KEYFOUND(false);
    AnsiString my_sql_statement;

    //
    // Format string with the entire Key Number( Customer Number - Key Number )
    sprintf(scratch, "%s", keyNumberSearchValue);

    //
    // Format "customer" with the customer number and "Key" with key number
    cpCustomer = strtok( scratch, tokens );
    cpKey = strtok(NULL, tokens);

    //
    // if left blank...then take the current customer number and search on that key.
    iCustomerNum = (cpCustomer == NULL) ? key_record->pkey->customerNumber : strtol(cpCustomer, &endptr, 16);

    //
    // if left blank, then search on the 00 key.
    iCustomerKey = (cpKey == NULL) ? 0 : strtol(cpKey, &endptr, 16);

    try
    {
        QueryKey->Close();
        QueryKey->SQL->Clear();
        //
        // Set the initial query w/o ordering
        my_sql_statement = "SELECT *, CONVERT(char(12),SKRactivationDate,7)As a_date, CONVERT(char(12),SKRdeactivationDate,7) As d_date, CONVERT(char(12),SKRexpirationDate,7) As e_date  FROM SKeyRecord JOIN SCustomerRecord ON SKeyRecord.SCRnumber = SCustomerRecord.SCRnumber WHERE SKeyRecord.SCRnumber = ";
        my_sql_statement += iCustomerNum;
        //
        // Set the private member variable for future queries
        setQueryKeySQLStatement(my_sql_statement);
        //
        // Set the query with ordering
        my_sql_statement += key_sort_type[SORT_KEY];
        //
        // Add the query
        QueryKey->SQL->Add(my_sql_statement);
        QueryKey->Open();
        QueryKey->First();

        QueryKey->DisableControls();
        //
        // Try to go back to the key you were previously on
        while(!QueryKey->Eof && !KEYFOUND)
        {
            if( QueryKey->FieldValues["SKRnumber"] == iCustomerKey )
            {
            	KEYFOUND = true;
            }
            else
         	QueryKey->Next();
         }
         //
         // if could not find key, then go to the 00 key
         if( !KEYFOUND )
         	QueryKey->First();
         QueryKey->EnableControls();

     }
     catch( Exception &e )
     {
        Application->MessageBox( e.Message.c_str(), "Database Failure", MB_OK );
     }

     //
     // Update the grid with the current information
     OnKeyRowChange(this);
     RefreshKeyPage();
}

//==============================================================================
// Function:    mmSearchByVersionNumberClick()
// Purpose:     Allows the user to search the database by version number.
// Parameters:  TObject* - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::mmSearchByVersionClick(TObject *Sender)
{
    TksdSearchDlg *dlg = new TksdSearchDlg(this);

    //
    // set the dialog options
    dlg->ksdVersionMaskEdit->Enabled = true;
    dlg->ksdVersionMaskEdit->TabOrder = 0;
    dlg->ksdSearchButton->TabOrder = 1;
    dlg->ksdCancelButton->TabOrder = 2;

    // Disable all other fields
    dlg->ksdKeyMaskEdit->Enabled = false;
    dlg->ksdCustomerMaskEdit->Enabled = false;
    dlg->ksdLicenseComboBox->Enabled = false;
    dlg->ksdKeyTypeComboBox->Enabled = false;

    if( dlg->ShowModal() == mrOk )
    {
        searchVersionNumber(dlg->versionNumber);
    }

    // Clean up
    delete dlg;
    dlg = NULL;
}

//==============================================================================
// Function:    searchVersionNumber()
// Purpose:     This will allow the user to search the database by version number
//              This should aide users in house with being able to find customers
//              that might have a certain version without having to search the
//              database, etc.
// Parameters:  TObject* - Sender
//              String -   versionSearchValue
// Returns:     None
//==============================================================================
void TFCustomerKeys::searchVersionNumber(String versionSearchValue)
{
     unsigned short usVersion;
     bool repaintflag = false;
     AnsiString my_sql_statement;

     sprintf(scratch, "%s", versionSearchValue);
     usVersion = static_cast<unsigned short>(strtol(scratch, &endptr, 16));

     try
     {
        QueryKey->Close();
   	QueryKey->SQL->Clear();
        //
        // Set the initial query statement w/o ordering
        my_sql_statement = "SELECT *, CONVERT(char(12),SKRactivationDate,7)As a_date, CONVERT(char(12),SKRdeactivationDate,7) As d_date, CONVERT(char(12),SKRexpirationDate,7) As e_date  FROM SKeyRecord JOIN SCustomerRecord ON SKeyRecord.SCRnumber = SCustomerRecord.SCRnumber where SKeyRecord.SKRversion = ";
        my_sql_statement += usVersion;
        //
        // Set the member variable for future queries
        setQueryKeySQLStatement(my_sql_statement);
        //
        // Add ordering and query according to that ordering
        my_sql_statement += key_sort_type[SORT_CUSTOMER];
        QueryKey->SQL->Add(my_sql_statement);
        QueryKey->Open();

        QueryKey->First();

        //
        // If there are no records
        if( !QueryKey->RecordCount )
        {
      	        Application->MessageBox( "No match found.", "Search", MB_OK|MB_ICONINFORMATION);
                repaintflag = true;
                KeyGrid->Repaint();
        }

        QueryKey->First();

     }
     catch( Exception &e )
     {
        Application->MessageBox( e.Message.c_str(), "Database Failure", MB_OK );
     }

     if (!repaintflag)
     {
        OnKeyRowChange(this);
        RefreshKeyPage();
     }
}

//==============================================================================
// Function:    mmActivationDateClick()
// Purpose:     Allows the user to search the database by version number.
// Parameters:  TObject* - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::mmActivationDateClick(TObject *Sender)
{
    TdsSearchDlg *dlg = new TdsSearchDlg(this);
    //
    // Set dialog options. TabOrder is already set.
    dlg->dsMainLabel->Caption = "Activation Date Search";

    if( dlg->ShowModal() == mrOk )
    {
        searchDate(SORT_ACTIVATION, dlg->greaterThanDate, dlg->lessThanDate);
    }

    // Clean Up
    delete dlg;
    dlg = NULL;
}

//==============================================================================
// Function:    mmExpirationDateClick()
// Purpose:     Allows the user to search the database by version number.
// Parameters:  TObject* - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::mmExpirationDateClick(TObject *Sender)
{
    TdsSearchDlg *dlg = new TdsSearchDlg(this);
    //
    // Set dialog options. TabOrder is already set.
    dlg->dsMainLabel->Caption = "Expiration Date Search";

    if( dlg->ShowModal() == mrOk )
    {
        searchDate(SORT_EXPIRATION, dlg->greaterThanDate, dlg->lessThanDate);
    }

    // Clean Up
    delete dlg;
    dlg = NULL;
}

//==============================================================================
// Function:    mmDeactivationDateClick()
// Purpose:     Allows the user to search the database by version number.
// Parameters:  TObject* - Sender
// Returns:     None
//==============================================================================
void __fastcall TFCustomerKeys::mmDeactivationDateClick(TObject *Sender)
{
    TdsSearchDlg *dlg = new TdsSearchDlg(this);
    //
    // Set dialog options. TabOrder is already set.
    dlg->dsMainLabel->Caption = "Deactivation Date Search";

    if( dlg->ShowModal() == mrOk )
    {
        searchDate(SORT_DEACTIVATION, dlg->greaterThanDate, dlg->lessThanDate);
    }

    // Clean Up
    delete dlg;
    dlg = NULL;
}

//==============================================================================
// Function:    searchDate()
// Purpose:     This will allow the user to search for the expiration date,
//              the activation date, or the deactivation date.
// Parameters:  TObject* - Sender
//              String -   versionSearchValue
// Returns:     None
//==============================================================================
void TFCustomerKeys::searchDate(int dateType, String greaterThanDate, String lessThanDate)
{
    //
    // Check the date formats to ensure that there are only valid values.
    if ( (!CheckDateFormat(greaterThanDate)) || (!CheckDateFormat(lessThanDate)) )
    {
        MessageBox(NULL, "Invalid Date Format. Please enter a valid search date", "Date Format Error", MB_OK);
        return;
    }

    AnsiString my_sql_statement;
    bool repaintflag(false);
    my_sql_statement = "SELECT *, CONVERT(char(12),SKRactivationDate,7)As a_date, CONVERT(char(12),SKRdeactivationDate,7) As d_date, CONVERT(char(12),SKRexpirationDate,7) As e_date  FROM SKeyRecord JOIN SCustomerRecord ON SKeyRecord.SCRnumber = SCustomerRecord.SCRnumber WHERE ";

    switch(dateType)
    {
        case SORT_ACTIVATION:
            //
            // Set initial query w/o ordering
            my_sql_statement += "SKRactivationDate >= '";
            my_sql_statement += greaterThanDate;
            my_sql_statement += "' and SKRactivationDate < '";
            my_sql_statement += lessThanDate;
            my_sql_statement += "'";
            //
            // Set the member variable for future queries
            setQueryKeySQLStatement(my_sql_statement);
            // Add ordering
            my_sql_statement += key_sort_type[SORT_ACTIVATION];
            break;

        case SORT_EXPIRATION:
            //
            // Set initial query w/o ordering
            my_sql_statement += "SKRexpirationdate >= '";
            my_sql_statement += greaterThanDate;
            my_sql_statement += "' and SKRexpirationdate < '";
            my_sql_statement += lessThanDate;
            my_sql_statement += "'";
            //
            // Set the member variable for future queries
            setQueryKeySQLStatement(my_sql_statement);
            // Add ordering
            my_sql_statement += key_sort_type[SORT_EXPIRATION];
            break;

        case SORT_DEACTIVATION:
            //
            // Set initial query w/o ordering
            my_sql_statement += "SKRdeactivationDate >= '";
            my_sql_statement += greaterThanDate;
            my_sql_statement += "' and SKRdeactivationDate < '";
            my_sql_statement += lessThanDate;
            my_sql_statement += "'";
            //
            // Set the member variable for future queries
            setQueryKeySQLStatement(my_sql_statement);
            //
            // Add ordering
            my_sql_statement += key_sort_type[SORT_DEACTIVATION];
            break;

        default:
            break;
    }

    try
    {
        KeyGrid->Repaint();
        QueryKey->Close();
   	QueryKey->SQL->Clear();
        QueryKey->SQL->Add(my_sql_statement);
        QueryKey->Open();

        QueryKey->First();

        //
        // If there are no records
        if( !QueryKey->RecordCount )
        {
            Application->MessageBox( "No match found.", "Search", MB_OK|MB_ICONINFORMATION);
            repaintflag = true;
        }

        QueryKey->First();
     }
     catch( Exception &e )
     {
         Application->MessageBox( e.Message.c_str(), "Database Failure", MB_OK );
     }

     if (!repaintflag)
     {
         OnKeyRowChange(this);
         RefreshKeyPage();
     }
}

//==============================================================================
// Function:    CheckDateFormat()
// Purpose:     This function checks the string to make sure that there are numbers
//              entered into the string.
// Parameters:  AnsiString - dateString
// Returns:     true if value is valid
//              false if the value is invalid.
//==============================================================================
bool TFCustomerKeys::CheckDateFormat(AnsiString dateString)
{
    //
    // Invalid Character Strings
    AnsiString lowerCase("abcdefghijklmnopqrstuvwxyz.,!@#$%^&*()_-+=[]{}|?~:;`");
    AnsiString upperCase("ABCDEFGHIJKLMNOPQRSTUVWXYZ.,!@#$%^&*()_-+=[]{}|?~:;`");
    AnsiString Month, Day, Year;

    //
    // Initialize check variables
    char *buffer = new char[dateString.Length()+1];
    StrLCopy(buffer, dateString.c_str(), dateString.Length());
    Month = strtok(buffer, "/");
    Day = strtok(NULL, "/");
    Year = strtok(NULL, "/");
    delete[] buffer;
    if (Month.Length() != 2 || Day.Length() != 2 || Year.Length() != 4)
        return false;

    //
    // We need to ensure that the string does not contain letters or other invalid characters.
    // Move throught the string to check for invalid characters
    for ( int j=1; j < dateString.Length(); j++)
    {
        for (int i=1; i < lowerCase.Length(); i++)
        {
            //
            // Compare each value with the values of the invalid character string
            if (dateString.SubString(j, 1) == lowerCase[i])
                return false;

            if (dateString.SubString(j, 1) == upperCase[i])
                return false;
        }
    }

    //
    // Check to see if they are indeed number values, then check the range
    // also check the length of the string, and to see if the 3rd character is a
    // backslash for the format of the dateString.
    if ( (Month.ToInt() > 12) || (Month.ToInt() < 1) ||     // 1 <= Month <= 12
         (Day.ToInt() > 31) || (Day.ToInt() < 1) ||         // 1 <= Day <= 31
         (Year.ToInt() > 3000) || (Year.ToInt() < 1985) ||  // 1985 <= Year <= 3000
         (dateString.Length() != 10) ||                     // MM/DD/YYYY...length = 10 characters
         (dateString[3] != '/') || (dateString[6] != '/') ) // Check for slashes
    {
        return false;
    }
    return true;
}

/* TabDataOnChange
 *----------------------------------------------------------------------------*/
void TFCustomerKeys::TabDataOnChange(bool bKeyTabNeedsRefreshing)
{
   if (bKeyTabNeedsRefreshing)
      RefreshKeyPage(0);
   RefreshKeyPage();
}



