//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SetUnitsDlg.h"
#include "SolSearcherEnterpriseDetails.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSolSearcherDetails *SolSearcherDetails;
//---------------------------------------------------------------------------
__fastcall TSolSearcherDetails::TSolSearcherDetails(TComponent* Owner)
   : TFrame(Owner)
{
}
//---------------------------------------------------------------------------

/* GeneratePasswordMenuItemClick()
 *----------------------------------------------------------------------------*/
void __fastcall TSolSearcherDetails::GeneratePasswordMenuItemClick(
      TObject *Sender)
{
   Sender;
   TListItem *pItem(SolSearcherPropertyList->Selected);
   if (!pItem)
      return;

   GeneratePassword(reinterpret_cast<int>(pItem->Data));
}

/* SetValueMenuItemClick
 *----------------------------------------------------------------------------*/
void __fastcall TSolSearcherDetails::SetValueMenuItemClick(TObject *Sender)
{
   Sender;
   TListItem *pItem(SolSearcherPropertyList->Selected);
   if (!pItem)
      return;

   SetValue(pItem);
}

/* SetValue() - Changes the value for a give item without generating a password.
 *----------------------------------------------------------------------------*/
void TSolSearcherDetails::SetValue(TListItem *pItem)//int type)
{
   int units(0);
   int type(reinterpret_cast<int>(pItem->Data));

   //
   // If for some reason you're in this function and not a SSKey
   if (m_pKey->productId != SOLSEARCHER_ENTERPRISE_PRODUCT)
        return;
   unsigned short min;
   unsigned short max;
   void (__closure *SetUnits)(unsigned short);

   switch (type) {
      case  INDEX_SERVERS_CELL:
         min = m_pKey->getIndexServers();
         max = MAX_INDEX_SERVERS;
         SetUnits = m_pKey->setIndexServers;
      break;
      case  REPORT_SERVERS_CELL:
         min = m_pKey->getReportServers();
         max = MAX_REPORT_SERVERS;
         SetUnits = m_pKey->setReportServers;
      break;
      case  CONCURRENT_USERS_25_CELL:
         min = m_pKey->getConcurrentUsers();
         max = MAX_CONCURRENT_USERS_25;
         SetUnits = m_pKey->setConcurrentUsers;
      break;
      case  APPLICATIONS_CELL:
         min =  m_pKey->getApplications();
         max = MAX_APPLICATIONS;
         SetUnits = m_pKey->setApplications;
      break;
   }

   if (min>=max && m_mode!=3)
      return;

   //
   // get new total of units licensed.
   TUnitsDlg *dlg = new TUnitsDlg(this, max);
   dlg->Edit1->Text = min;

   if( dlg->ShowModal() == IDYES ) {
      units = (unsigned short)dlg->Edit1->Text.ToInt();

      SetUnits(units);
      pItem->SubItems->Strings[0] = units;
      pItem->Update();
   }
   delete dlg;
   dlg = NULL;
}

/* GeneratePassword()
 *----------------------------------------------------------------------------*/
void TSolSearcherDetails::GeneratePassword(int type)
{
   int units(0);
   int descriptionId;
   char password_string[128];
   AnsiString fieldName;

   //
   // If for some reason you're in this function and not a SSKey
   if (m_pKey->productId != SOLSEARCHER_ENTERPRISE_PRODUCT)
        return;
   unsigned short min(0);
   unsigned short max;
//   unsigned long ( __closure *CreatePassword)(SSProtectionKey*, unsigned short);
AnsiString ( __closure *CreatePassword)(SSProtectionKey*, unsigned short);
   void ( __closure *ApplyPassword)(SKeyRecord* keyrec, unsigned short units_licensed);

   switch (type) {
      case  INDEX_SERVERS_CELL:
         if (!m_pKey->isOnTrial())
            min = m_pKey->getIndexServers();
         max = MAX_INDEX_SERVERS;
         fieldName = "SKRindexServers";
         descriptionId = 600;
         CreatePassword = keyMaster->getIndexServersPassword;
         ApplyPassword =  keyMaster->applyIndexServersPassword;
      break;
      case  REPORT_SERVERS_CELL:
         if (!m_pKey->isOnTrial())
            min = m_pKey->getReportServers();
         max = MAX_REPORT_SERVERS;
         fieldName = "SKRreportServers";
         descriptionId = 601;
         CreatePassword = keyMaster->getReportServersPassword;
         ApplyPassword =  keyMaster->applyReportServersPassword;
      break;
      case  CONCURRENT_USERS_25_CELL:
         if (!m_pKey->isOnTrial())
            min = m_pKey->getConcurrentUsers();
         //max = MAX_CONCURRENT_USERS_25;  RH CR 3742
         max = MAX_CONCURRENT_USERS;
         fieldName = "SKRconcurrentUsers";
         descriptionId = 602;
         CreatePassword = keyMaster->getConcurrentUsersPassword;
         ApplyPassword =  keyMaster->applyConcurrentUsersPassword;
      break;
      case  APPLICATIONS_CELL:
         if (!m_pKey->isOnTrial())
            min =  m_pKey->getApplications();
         max = MAX_APPLICATIONS;
         descriptionId = 603;
         fieldName = "SKRapplications";
         CreatePassword = keyMaster->getApplicationServerPassword;
         ApplyPassword =  keyMaster->applyApplicationPassword;
      break;
   }

   if (min>=max)
      return;

   //
   // get new total of units licensed.
   TUnitsDlg *dlg = new TUnitsDlg(this, max);
   dlg->Edit1->Text = min;

   if( dlg->ShowModal() == IDYES ) {
      units = (unsigned short)dlg->Edit1->Text.ToInt();

      //check if key is attached
      if( keyMaster->found() == false ) {
         Application->MessageBox("No key attached.", "Information Message", MB_OK);
         return;
      }
      //check if attached key is programmed
      if( keyMaster->programmed() == false ) {
         Application->MessageBox("Attached key must be programmed.", "Information Message", MB_OK);
         return;
      }

      //generate password
      AnsiString password = CreatePassword(m_pKey, units);
      if(!password.c_str()) {
         Application->MessageBox("Unable to generate password. Check to see if the key is a slave key.", "Key Message", MB_OK|MB_ICONERROR );
         return;
      }

      //apply password to key
      ApplyPassword(m_pKeyRecord, units);

      //format password
//      sprintf( password_string, "%8X-%1d", password.c_str();, units);
        sprintf( password_string, "%8s", password.c_str());
      //Update database
      try
      {
         m_pDatabase->StartTransaction();

         DetailSQL->Close();
         DetailSQL->SQL->Clear();

         if (m_pKeyRecord->non_perm_ktf == true)
            DetailSQL->SQL->Add(Format("UPDATE SKeyRecord SET %s = :units WHERE SKRid = :keyId", OPENARRAY(TVarRec,(fieldName))));
         else
            DetailSQL->SQL->Add(Format("UPDATE SKeyRecord SET SKRstatus = 2, %s = :units WHERE SKRid = :keyId", OPENARRAY(TVarRec,(fieldName))));

         DetailSQL->ParamByName("units")->AsInteger = m_pKey->getIndexServers();
         DetailSQL->ParamByName("keyId")->AsInteger = m_pKeyRecord->skr_id;
         DetailSQL->ExecSQL();

         DetailSQL->SQL->Add("INSERT INTO sTransactionDetail (SKRid, TDpassword, SDRid, TDunits, TDrow_id) values (:key_id, :password, :descriptionid, :units, 0)");
         DetailSQL->ParamByName("key_id")->AsInteger = m_pKeyRecord->skr_id;
         DetailSQL->ParamByName("password")->AsString = password_string;
         DetailSQL->ParamByName("units")->AsInteger = units;
         DetailSQL->ParamByName("descriptionid")->AsInteger = descriptionId;
         DetailSQL->ExecSQL();

         m_pDatabase->Commit();
         OnDetailUpdate(true); //update key form also
      }
      catch( Exception *e ) {
         Application->MessageBox( e->Message.c_str(), "Database Failure", MB_OK);
         m_pDatabase->Rollback();
      }
//////
   }

   //
   // clean up the heap
   delete(dlg);
   dlg = NULL;
}

void TSolSearcherDetails::Load(SKeyRecord* pKeyRecord)
{
   //set members

   if (pKeyRecord->pkey->productId != 8)
      return;
   m_pKeyRecord = pKeyRecord;
   m_pKey = reinterpret_cast<SSProtectionKey *>(m_pKeyRecord->pkey);

    TListItem *pItem;
   //clear list
   SolSearcherPropertyList->Items->Clear();

   //INDEX_SERVERS_CELL = 0x00;
   pItem = SolSearcherPropertyList->Items->Add();
   pItem->Caption = "Index Server";
   pItem->Data = (void*)INDEX_SERVERS_CELL;
   pItem->SubItems->Add(m_pKey->getIndexServers());

   //REPORT_SERVERS_CELL = 0x01;
   pItem = SolSearcherPropertyList->Items->Add();
   pItem->Caption = "Document Server";
   pItem->Data = (void*)REPORT_SERVERS_CELL;
   pItem->SubItems->Add(m_pKey->getReportServers());

   //CONCURRENT_USERS_25_CELL = 0x02;
   pItem = SolSearcherPropertyList->Items->Add();
   pItem->Caption = "Concurrent Users";
   pItem->Data = (void*)CONCURRENT_USERS_25_CELL;
   pItem->SubItems->Add(m_pKey->getConcurrentUsers());

   //APPLICATIONS_CELL = 0x03;
   pItem = SolSearcherPropertyList->Items->Add();
   pItem->Caption = "Application Databases";
   pItem->Data = (void*)APPLICATIONS_CELL;
   pItem->SubItems->Add(m_pKey->getApplications());

}
/* Initialize
 * Mode: 2 - Viewing Info
 * Mode: 3 - Wizard
 *----------------------------------------------------------------------------*/
void TSolSearcherDetails::Initialize(int mode, void( __closure *fp_OnDetailUpdate)(bool))
{
   if (m_bInitialized)
      return;
   OnDetailUpdate = fp_OnDetailUpdate;

   m_pDatabase = Session->FindDatabase("CustDB");
   m_mode = mode;
   PropertyPopup->Items->Items[0]->Visible = false;
   PropertyPopup->Items->Items[1]->Visible = false;

   if (m_mode == 2) {
      PropertyPopup->Items->Items[0]->Visible = true;
   }
   else if (m_mode == 3) {
      PropertyPopup->Items->Items[1]->Visible = true;
   }

   m_bInitialized = true;
}
/*
//==============================================================================
// Function:    createIndexServersPassword()
// Purpose:     creates an index server password, and updates the information in the
//              SKeyRecord and STransactionDetail tables based on the key record
//              id number.
// Parameters:  ( int ) - units_licensed
// Returns:     None
//==============================================================================
void TSolSearcherDetails::CreateIndexServersPassword(unsigned short units_licensed)
{
   char password_string[14];

   //check if key is attached
   if( keyMaster->found() == false )
   {
      Application->MessageBox("No key attached.", "Information Message", MB_OK);
      return;
   }
   //check if attached key is programmed
   if( keyMaster->programmed() == false )
   {
      Application->MessageBox("Attached key must be programmed.", "Information Message", MB_OK);
      return;
   }

   //
   //generate password
   unsigned long password = keyMaster->getIndexServersPassword(m_pKey, units_licensed);
   if(!password)
   {
      Application->MessageBox("Unable to generate password. Check to see if the key is a slave key.", "Key Message", MB_OK|MB_ICONERROR );
      return;
   }

   //
   //apply password to key
   keyMaster->applyIndexServersPassword(m_pKeyRecord, units_licensed);

   //
   //format password
   sprintf( password_string, "%8X-%1d", password, units_licensed);

   //
   //Update database
   try
   {
      m_pDatabase->StartTransaction();

      DetailSQL->Close();
      DetailSQL->SQL->Clear();

      if (key_record->non_perm_ktf == true)
         DetailSQL->SQL->Add("UPDATE SKeyRecord SET SKRindexServers = :units WHERE SKRid = :keyId ");
      else
         DetailSQL->SQL->Add("UPDATE SKeyRecord SET SKRstatus = 2, SKRindexServers = :units WHERE SKRid = :keyId ");

      DetailSQL->ParamByName("units")->AsInteger = ss_key->getIndexServers();
      DetailSQL->ParamByName("keyId")->AsInteger = m_pKeyRecord->skr_id;
      DetailSQL->ExecSQL();

      DetailSQL->SQL->Add("INSERT INTO sTransactionDetail (SKRid, TDpassword, SDRid, TDunits, TDrow_id) values (:key_id, :password, :descriptionid, :units, 0)");
      DetailSQL->ParamByName("key_id")->AsInteger = m_pKeyRecord->skr_id;
      DetailSQL->ParamByName("password")->AsString = password_string;
      DetailSQL->ParamByName("units")->AsInteger = units_licensed;
      DetailSQL->ParamByName("descriptionid")->AsInteger = 600;  // 600 = Index Servers
      DetailSQL->ExecSQL();

      m_pDatabase->Commit();
      //RefreshKeyPage(0);
      //RefreshKeyPage();
   }
   catch( Exception *e )
   {
      Application->MessageBox( e->Message.c_str(), "Database Failure", MB_OK);
      m_pDatabase->Rollback();
   }
}

//==============================================================================
// Function:    createReportServersPassword()
// Purpose:     creates an index server password, and updates the information in the
//              SKeyRecord and STransactionDetail tables based on the key record
//              id number.
// Parameters:  ( int ) - units_licensed
// Returns:     None
//==============================================================================
void TSolSearcherDetails::CreateReportServersPassword(unsigned short units_licensed)
{
   char password_string[14];

   //
   // If for some reason you're in this function and not a SSKey
   if (key_record->pkey->productId != SOLSEARCHER_ENTERPRISE_PRODUCT)
        return;

   //
   // check attached key status - need to have a programmed key attached
   // pkey should always be SSProtectionKey
   SSProtectionKey* ss_key = ((SSProtectionKey*)(key_record->pkey));

   //
   // check if key is attached
   if(!isAttachedKeyReady())
      return;

   //
   // generate password
   unsigned long password = keyMaster->getReportServersPassword(ss_key, units_licensed);
   if(!password)
   {
      Application->MessageBox("Unable to generate password. Check to see if the key is a slave key.", "Key Message", MB_OK|MB_ICONERROR );
      return;
   }

   //
   // apply password to key
   keyMaster->applyReportServersPassword(key_record, units_licensed);

   //
   // format password
   sprintf( password_string, "%8X-%1d", password, units_licensed);

   //
   // Update database
   try
   {
      m_pDatabase->StartTransaction();

      DetailSQL->Close();
      DetailSQL->SQL->Clear();

      if (key_record->non_perm_ktf == true)
         DetailSQL->SQL->Add("UPDATE SKeyRecord SET SKRreportServers = :units WHERE SKRid = :keyId ");
      else
         DetailSQL->SQL->Add("UPDATE SKeyRecord SET SKRstatus = 2, SKRreportServers = :units WHERE SKRid = :keyId ");

      DetailSQL->ParamByName("units")->AsInteger = ss_key->getReportServers();
      DetailSQL->ParamByName("keyId")->AsInteger = key_record->skr_id;
      DetailSQL->ExecSQL();

      DetailSQL->SQL->Add("INSERT INTO sTransactionDetail (SKRid, TDpassword, SDRid, TDunits, TDrow_id) values (:key_id, :password, :descriptionid, :units, 0)");
      DetailSQL->ParamByName("key_id")->AsInteger = key_record->skr_id;
      DetailSQL->ParamByName("password")->AsString = password_string;
      DetailSQL->ParamByName("units")->AsInteger = units_licensed;
      DetailSQL->ParamByName("descriptionid")->AsInteger = 601;  // 601 = Report Servers
      DetailSQL->ExecSQL();

      m_pDatabase->Commit();
      ///RefreshKeyPage(0);
      ///RefreshKeyPage();
   }
   catch( Exception *e )
   {
      Application->MessageBox( e->Message.c_str(), "Database Failure", MB_OK);
      m_pDatabase->Rollback();
   }
}


//==============================================================================
// Function:    createConcurrentUsersPassword()
// Purpose:     creates an index server password, and updates the information in
//              the SKeyRecord and STransactionDetail tables based on the key
//              record id number.
// Parameters:  ( int ) - units_licensed
// Returns:     None
//==============================================================================
void TSolSearcherDetails::CreateConcurrentUsersPassword(unsigned short units_licensed)
{
   char password_string[14];

   //
   // If for some reason you're in this function and not a SSKey
   if (key_record->pkey->productId != SOLSEARCHER_ENTERPRISE_PRODUCT)
        return;

   //
   // check attached key status - need to have a programmed key attached
   // pkey should always be SSProtectionKey
   SSProtectionKey* ss_key = ((SSProtectionKey*)(key_record->pkey));

   //
   // check if key is attached
   if(!isAttachedKeyReady())
      return;

   //
   // generate password
   unsigned long password = keyMaster->getConcurrentUsersPassword(ss_key, units_licensed);
   if(!password)
   {
      Application->MessageBox("Unable to generate password. Check to see if the key is a slave key.", "Key Message", MB_OK|MB_ICONERROR );
      return;
   }

   //
   // apply password to key
   keyMaster->applyConcurrentUsersPassword(key_record, units_licensed);

   //
   // format password
   sprintf( password_string, "%8X-%1d", password, units_licensed);

   //
   // Update database
   try
   {
      m_pDatabase->StartTransaction();

      DetailSQL->Close();
      DetailSQL->SQL->Clear();

      if (key_record->non_perm_ktf == true)
         DetailSQL->SQL->Add("UPDATE SKeyRecord SET SKRconcurrentUsers = :units WHERE SKRid = :keyId ");
      else
         DetailSQL->SQL->Add("UPDATE SKeyRecord SET SKRstatus = 2, SKRconcurrentUsers = :units WHERE SKRid = :keyId ");

      DetailSQL->ParamByName("units")->AsInteger = ss_key->getConcurrentUsers();
      DetailSQL->ParamByName("keyId")->AsInteger = key_record->skr_id;
      DetailSQL->ExecSQL();

      DetailSQL->SQL->Add("INSERT INTO sTransactionDetail (SKRid, TDpassword, SDRid, TDunits, TDrow_id) values (:key_id, :password, :descriptionid, :units, 0)");
      DetailSQL->ParamByName("key_id")->AsInteger = key_record->skr_id;
      DetailSQL->ParamByName("password")->AsString = password_string;
      DetailSQL->ParamByName("units")->AsInteger = units_licensed;
      DetailSQL->ParamByName("descriptionid")->AsInteger = 602;  // 602 = Concurrent Users
      DetailSQL->ExecSQL();

      m_pDatabase->Commit();
      RefreshKeyPage(0);
      RefreshKeyPage();
   }
   catch( Exception *e )
   {
      Application->MessageBox( e->Message.c_str(), "Database Failure", MB_OK);
      m_pDatabase->Rollback();
   }
}

//==============================================================================
// Function:    createApplicationPassword()
// Purpose:     creates an application server password, and updates the information in
//              the SKeyRecord and STransactionDetail tables based on the key
//              record id number.
// Parameters:  ( int ) - units_licensed
// Returns:     None
//==============================================================================
void TSolSearcherDetails::CreateApplicationPassword(unsigned short units_licensed)
{
   char password_string[14];

   //
   // If for some reason you're in this function and not a SSKey
   if (key_record->pkey->productId != SOLSEARCHER_ENTERPRISE_PRODUCT)
        return;

   //
   // check attached key status - need to have a programmed key attached
   // pkey should always be SSProtectionKey
   SSProtectionKey* ss_key = ((SSProtectionKey*)(key_record->pkey));

   //
   // check if key is attached
   if(!isAttachedKeyReady())
      return;

   //
   // generate password
   unsigned long password = keyMaster->getApplicationServerPassword(ss_key, units_licensed);
   if(!password)
   {
      Application->MessageBox("Unable to generate password. Check to see if the key is a slave key.", "Key Message", MB_OK|MB_ICONERROR );
      return;
   }

   //
   // apply password to key
   keyMaster->applyApplicationPassword(key_record, units_licensed);

   //
   // format password
   sprintf( password_string, "%8X-%1d", password, units_licensed);

   //
   // Update database
   try
   {
      m_pDatabase->StartTransaction();

      DetailSQL->Close();
      DetailSQL->SQL->Clear();

      if (key_record->non_perm_ktf == true)
         DetailSQL->SQL->Add("UPDATE SKeyRecord SET SKRconcurrentUsers = :units WHERE SKRid = :keyId ");
      else
         DetailSQL->SQL->Add("UPDATE SKeyRecord SET SKRstatus = 2, SKRapplications = :units WHERE SKRid = :keyId ");

      DetailSQL->ParamByName("units")->AsInteger = ss_key->getApplications();
      DetailSQL->ParamByName("keyId")->AsInteger = key_record->skr_id;
      DetailSQL->ExecSQL();

      DetailSQL->SQL->Add("INSERT INTO sTransactionDetail (SKRid, TDpassword, SDRid, TDunits, TDrow_id) values (:key_id, :password, :descriptionid, :units, 0)");
      DetailSQL->ParamByName("key_id")->AsInteger = key_record->skr_id;
      DetailSQL->ParamByName("password")->AsString = password_string;
      DetailSQL->ParamByName("units")->AsInteger = units_licensed;
      DetailSQL->ParamByName("descriptionid")->AsInteger = 603;  // 603 = Application Servers
      DetailSQL->ExecSQL();

      m_pDatabase->Commit();
      RefreshKeyPage(0);
      RefreshKeyPage();
   }
   catch( Exception *e )
   {
      Application->MessageBox( e->Message.c_str(), "Database Failure", MB_OK);
      m_pDatabase->Rollback();
   }
}
*/




