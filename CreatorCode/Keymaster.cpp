//------------------------------------------------------------*
// filename -       keymastr.cpp                              *
//                                                            *
//        Protection Key Interface for the Creator.           *
//                                                            *
// Class(es):                                                 *
//        KeyMaster                                           *
//                                                            *
//------------------------------------------------------------*
//------------------------------------------------------------*
//                                                            *
//    The Creator                                             *
//                                                            *
//    Copyright (c) 1993-2001 by Solimar Systems, Inc.        *
//    All Rights Reserved.                                    *
//                                                            *
//------------------------------------------------------------*


#include "kyslevel.h"
#include "Keymaster.h"
#include "SolimarLicensing\SolimarLicenseServer\_SolimarLicenseServer_i.c"
#include <windows.h>
#include "SolimarLicensing\common\ChallengeResponseHelper.h"

#define OVERPASSWORD1			0xC822
#define OVERPASSWORD2			0x85A2

// statusLineText codes
#define OPERATING_ON_KEY            1

BYTE challenge_key_manager_userauththis_public[] = {
#include "..\..\SolimarLicensing\common\keys\SolimarLicenseServer.UserAuthThis.public.key.txt"
};

BYTE challenge_key_manager_thisauthuser_private[] = {
#include "..\..\SolimarLicensing\common\keys\SolimarLicenseServer.ThisAuthUser.private.key.txt"
};

/*--------------------------------------------------------------------*
 *                                                                    *
 *   KeyMaster Class:                                                 *
 *                                                                    *
 *   This class provides the Protection Key interface for the Creator.*
 *                                                                    *
 *--------------------------------------------------------------------*/
//==============================================================================
// Function:    Constructor()
// Purpose:     Initialize all members.
// Parameters:  None
// Returns:     nothing
// Note:
//==============================================================================
KeyMaster::KeyMaster()
{
        pKeyList = new VARIANT();
        pTheServer = NULL;
        CurrentKey = new ProtectionKey;
}

//==============================================================================
// Function:    Destructor()
// Purpose:     Uninitialize all members.
// Parameters:  None
// Returns:     nothing
// Note:
//==============================================================================
KeyMaster::~KeyMaster()
{
   VariantClear(pKeyList);
   delete pKeyList;

   delete CurrentKey;
   CurrentKey = NULL;
}

//==============================================================================
// Function:    clear()
// Purpose:     Initialize all cells to 0.  Returns 0 for success, 1 otherwise.
// Parameters:  None
// Returns:     short
// Note:
//==============================================================================
short KeyMaster::clear()
{
   if (SUCCEEDED(setHandle()))
   {
      if(pTheServer)
      {
         if(SUCCEEDED(CurrentKey->clearKeyData(CurrentKeyID, pTheServer)))
            return 0;
      }
   }
   return 1;
}


//==============================================================================
// Function:    deactivate()
// Purpose:     Initializes key.  Returns 0 for success,
//              1 otherwise.
// Parameters:  None
// Returns:     short
// Note:
//==============================================================================
short KeyMaster::deactivate()
{
   return clear();
}


//==============================================================================
// Function:    found()
// Purpose:     Finds the protection key.  Returns True if key is found, False
//              otherwise.
// Parameters:  None
// Returns:     bool
// Note:
//==============================================================================
bool KeyMaster::found()
{
        VARIANT_BOOL IsPresent;

        if (SUCCEEDED(setHandle()))
        {
                if(pTheServer)
                {
                        pTheServer->KeyIsPresent(*CurrentKeyID, &IsPresent);
                        return (IsPresent == VARIANT_TRUE ? true : false);
                }
        }
        return false;
}

//==============================================================================
// Function:    getPermanentPassword()
// Purpose:     Get the permanent password for the key.  Returns password for
//              success, 0 otherwise.
// Parameters:  ProtectionKey* - key
// Returns:     ulong
// Note:
//==============================================================================
void KeyMaster::getPermanentPassword(ProtectionKey* key, char* Password_String)
{
   if(pTheServer)
      key->getBasePassword(pTheServer, Password_String);
}


//==============================================================================
// Function:    getExtensionPassword()
// Purpose:     Get extension password for extension_days.  Returns password for
//              success, 0 otherwise.
// Parameters:  ProtectionKey* - key
//              uchar - extension_days
//              ProductId - product_id
//              ushort - product_version
// Returns:     ulong
// Note:
//==============================================================================
void KeyMaster::getExtensionPassword(ProtectionKey* key,
                                      uchar extension_days,
                                      ProductId product_id,
                                      ushort product_version,
                                      ushort extension_num,
                                      char* Password_String)
{

   if(pTheServer)
      key->getExtensionPassword(extension_days, extension_num, pTheServer, Password_String);
}


//==============================================================================
// Function:    getModulePassword()
// Purpose:     Get password for module mod_id.  Returns password for success, 0
//              otherwise.
// Parameters:  ProtectionKey* - key
//              uchar - mod_id
//              ProductId - product_id
//              ushort - product_version
// Returns:     ulong
// Note:
//==============================================================================
void  KeyMaster::getModulePassword(SpdProtectionKey* key,
                                   uchar mod_id,
                                   ProductId product_id,
                                   ushort product_version,
                                   ushort units,
                                   char* Password_String)
{
   if(pTheServer)
      key->getModulePassword(mod_id, units,
                             product_id,
                             product_version,
                             pTheServer,
                             Password_String
                            );
}


//==============================================================================
// Function:    getOutputPassword()
// Purpose:
// Parameters:  ProtectionKey* - key
//              ushort - output_units
// Returns:     ulong
// Note:
//==============================================================================
void KeyMaster::getOutputPassword(SpdProtectionKey* key,
                                   ushort output_units,
                                   char* Password_String )
{
   if(pTheServer)
     key->getOutputPassword(output_units, pTheServer, Password_String);
}

//==============================================================================
// Function:    getPagesPerMinutePassword()
// Purpose:
// Parameters:  ProtectionKey* - key
//              ushort - output_units
// Returns:     ulong
// Note:
//==============================================================================
void KeyMaster::getPagesPerMinutePassword(SpdProtectionKey* key,
                                           ushort ext,
                                           ushort pages,
                                           char* Password_String,
                                           long ModID )
{
   if(pTheServer)
      key->getPagesPerMinutePassword(ext, pages, pTheServer, Password_String, ModID);
}

//==============================================================================
// Function:    getProductVersionPassword()
// Purpose:     Get product version password for prodcut_version.  Returns password
//              for success, 0 otherwise.
// Parameters:  ProtectionKey* - key
//              ushort - product_version
// Returns:     ulong
// Note:
//==============================================================================
void KeyMaster::getProductVersionPassword(ProtectionKey* key,
                                          ushort product_version,
                                          char* Password_String
                                          )
{
   if(pTheServer)
      key->getProductVersionPassword(product_version,
                                     pTheServer,
                                     Password_String);

}

//==============================================================================
// Function:    getIndexServerPassword()
// Purpose:     Gets the index server password used for the SOL searcher enterprise
//              keys.
// Parameters:  SSProtectionKey* - key
//              ushort - units_licensed
// Returns:     ulong
// Note:
//==============================================================================
AnsiString KeyMaster::getIndexServersPassword(SSProtectionKey* key,
                                         ushort units_licensed)
{
   if(pTheServer)
      return key->getIndexServersPassword(units_licensed, pTheServer);

   return 0;
}

//==============================================================================
// Function:    getReportServerPassword()
// Purpose:     Gets the report server password used for the SOL searcher enterprise
//              keys.
// Parameters:  SSProtectionKey* - key
//              ushort - units_licensed
// Returns:     ulong
// Note:
//==============================================================================
AnsiString KeyMaster::getReportServersPassword(SSProtectionKey* key,
                                          ushort units_licensed)
{
   if(pTheServer)
      return key->getReportServersPassword(units_licensed, pTheServer);

   return 0;
}

//==============================================================================
// Function:    getConcurrentUsersPassword()
// Purpose:     Gets the index server password used for the SOL searcher enterprise
//              keys.
// Parameters:  SSProtectionKey* - key
//              ushort - units_licensed
// Returns:     ulong
// Note:
//==============================================================================
AnsiString KeyMaster::getConcurrentUsersPassword(SSProtectionKey* key,
                                                 ushort units_licensed)
{
   if(pTheServer)
      return key->getConcurrentUsersPassword(units_licensed, pTheServer);

   return 0;
}

AnsiString KeyMaster::getApplicationServerPassword(SSProtectionKey* key,
                                              ushort units_licensed)
{
   if(pTheServer)
         return key->getApplicationsPassword(units_licensed, pTheServer);

   return 0;
}

//==============================================================================
// Function:    initDriver()
// Purpose:     Initialize SuperPro driver.  Returns 0 for success, 1 otherwise.
// Parameters:  None
// Returns:     short
// Note:
//==============================================================================
HRESULT KeyMaster::initDriver()
{
   HRESULT hr = 0;

   //connects to the solimar license server
	hr = CoCreateInstance(CLSID_CSolimarLicenseSvr,
  			                NULL,
    			             CLSCTX_LOCAL_SERVER,
      			          IID_ISolimarLicenseSvr,
			                (void**)&pTheServer
	  		               );

   if(!SUCCEEDED(hr))
   {
      Application->MessageBox("The Solimar License Server Is Not Running", "Key Message", MB_OK|MB_ICONERROR )
      Application->Terminate();
      return hr;
   }


   ChallengeResponseHelper cr(challenge_key_manager_thisauthuser_private, sizeof(challenge_key_manager_thisauthuser_private), challenge_key_manager_userauththis_public, sizeof(challenge_key_manager_userauththis_public));

	hr = cr.AuthenticateServer(pTheServer);
	if(!SUCCEEDED(hr))
      return hr;

	// let the license server authenticate this client
	hr = cr.AuthenticateToServer(pTheServer);
	if(!SUCCEEDED(hr))
      return hr;

   //get the list of keys
   hr = pTheServer->KeyEnumerate(pKeyList);

   return hr;
}


//==============================================================================
// Function:    newKey()
// Purpose:     Find a Solimar key and create and return a new key object based on
//              the product ID found on the key.
// Parameters:  None
// Returns:     ProtectionKey*
// Note:
//==============================================================================
ProtectionKey* KeyMaster::newKey()
{
   if(pTheServer)
      if(SUCCEEDED(setHandle()))
      	   return ProtectionKey::newKey(CurrentKeyID, pTheServer);

   return NULL;
}

//==============================================================================
// Function:    program()
// Purpose:     Programs the protection key.   Returns 0 for success, 1 otherwise.
// Parameters:  None
// Returns:     short
// Note:
//==============================================================================
short KeyMaster::program(SKeyRecord* key_record)
{
   ProtectionKey* key = key_record->pkey;
   short ret = 1;
   HRESULT hr = 0;
   VARIANT ModuleArray;
   MultidimensionalSafeArray::DimensionsType dims, index;

   //returned from AccessMultiDimensionalSafearray and is used to stuff the variants
   //into the safe array
   VARIANT* pVarArray;

   if(!SUCCEEDED(setHandle()))
      return 1;

   ModuleDetail** module_detail = lookup->getModuleList(key_record->pkey->productId);
   if(key_record->pkey->productId != SOLSEARCHER_ENTERPRISE_PRODUCT)
   {
      SpdProtectionKey* spd_key((SpdProtectionKey*)(key_record->pkey));
      int NumModules = 0;
      int ModuleIds[64];

      for(int mod_id =0; mod_id<64;mod_id++)
      {
         if ((module_detail[mod_id]->bExistingMember))
         {

            ModuleIds[NumModules] = mod_id;
            NumModules++;
         }
      }

      //set up the dimenstions of the multi dim safe array. 64 mods and each mod has
      //2 fields (mod id and licesne count)
      dims.push_back(NumModules);
      dims.push_back(2);

      //create the multi dim safe array to look like Module_Array [NumModules][2]
      hr = MultidimensionalSafeArray::CreateMultidimensionalSafearray(&ModuleArray, dims);
char Buff[256];
      for(int i=0; i<NumModules; i++)
      {
         AnsiString mod_name = module_detail[ModuleIds[i]]->name;

         index.push_back(i);
         index.push_back(0);

         hr = MultidimensionalSafeArray::AccessMultidimensionalSafearray(&ModuleArray,
                                                                                 index,
                                                                                 &pVarArray
                                                                                );
         if(SUCCEEDED(hr))
         {
            //set the type of the variant
            pVarArray->vt = VT_UI4;
            pVarArray->ulVal = module_detail[ModuleIds[i]]->id;
            hr = MultidimensionalSafeArray::UnaccessMultidimensionalSafearray(&ModuleArray, index);
            ltoa(hr, Buff, 10);
           	OutputDebugString("**HR from first unaccess ");
         	OutputDebugString(Buff);
         	OutputDebugString("**\r\n");
         }

         index.pop_back();
         index.push_back(1);

         if(SUCCEEDED(MultidimensionalSafeArray::AccessMultidimensionalSafearray(&ModuleArray,
                                                                                 index,
                                                                                 &pVarArray
                                                                                 )))
         {
            //set the type of the variant
            pVarArray->vt = VT_UI4;
            pVarArray->ulVal = spd_key->getLicense(ModuleIds[i]);
            hr = MultidimensionalSafeArray::UnaccessMultidimensionalSafearray(&ModuleArray, index);
            ltoa(hr, Buff, 10);
           	OutputDebugString("**HR from second unaccess ");
         	OutputDebugString(Buff);
         	OutputDebugString("**\r\n");
         }
         index.pop_back();
         index.pop_back();
      }
   }
   
   //its an SSE key
   else
   {
      //set up the dimenstions of the multi dim safe array. 64 mods and each mod has
      //2 fields (mod id and licesne count)
      dims.push_back(4);
      dims.push_back(2);

      //create the multi dim safe array to look like Module_Array [NumModules][2]
      MultidimensionalSafeArray::CreateMultidimensionalSafearray(&ModuleArray, dims);

      for( int mod_id=0; mod_id<4; mod_id++ )
      {
         SSProtectionKey* ss_key = ((SSProtectionKey*)(key_record->pkey));
         index.push_back(mod_id);
         index.push_back(0);

         if(SUCCEEDED(MultidimensionalSafeArray::AccessMultidimensionalSafearray(&ModuleArray,
                                                                                 index,
                                                                                 &pVarArray
                                                                                )))
         {
            //set the type of the variant
            pVarArray->vt = VT_UI4;
            pVarArray->ulVal = mod_id;
            MultidimensionalSafeArray::UnaccessMultidimensionalSafearray(&ModuleArray, index);
         }

         index.pop_back();
         index.push_back(1);
         if(SUCCEEDED(MultidimensionalSafeArray::AccessMultidimensionalSafearray(&ModuleArray,
                                                                                 index,
                                                                                 &pVarArray
                                                                                 )))
         {
            //set the type of the variant
            pVarArray->vt = VT_UI4;

            if(mod_id == 0)
              pVarArray->ulVal = ss_key->getIndexServers();
            else if(mod_id == 1)
               pVarArray->ulVal = ss_key->getReportServers();
            else if(mod_id == 2)
               pVarArray->ulVal = ss_key->getConcurrentUsers();
            else
               pVarArray->ulVal = ss_key->getApplications();

            MultidimensionalSafeArray::UnaccessMultidimensionalSafearray(&ModuleArray, index);
         }
         index.pop_back();
         index.pop_back();
      }
   }
   if(pTheServer)
      ret = key->ProgramKey(pTheServer, CurrentKeyID, key_record->num_days, &ModuleArray);

  	OutputDebugString("**Before Deleting multi dim");
   hr = MultidimensionalSafeArray::DeleteMultidimensionalSafearray(&ModuleArray);
   char Buffer[256];
   ltoa(hr, Buffer, 10);
  	OutputDebugString("**HR from deleting the multi dim array is ");
	OutputDebugString(Buffer);
	OutputDebugString("**\r\n");

   if(!SUCCEEDED(hr))
      ret = 1;

   VariantClear(&ModuleArray);

   return ret;
}

//==============================================================================
// Function:    programmed()
// Purpose:     Determines if key is programmed. Returns True if key is programmed,
//              False otherwise or for key error.
// Parameters:  None
// Returns:     bool
// Note:
//==============================================================================
bool KeyMaster::programmed()
{
   VARIANT_BOOL IsProgrammed;
   HRESULT hr = 0;

   if (SUCCEEDED(setHandle()))
   {
      if(pTheServer)
      {
         hr = pTheServer->KeyIsProgrammed(*CurrentKeyID, &IsProgrammed);
         if(SUCCEEDED(hr))
            return (IsProgrammed == VARIANT_TRUE ? true : false);
      }
   }
   return false;
}


//==============================================================================
// Function:    read()
// Purpose:     Finds the protection key and reads its data.
//              Returns 0 for success, 1 otherwise.
// Parameters:  SKeyRecord* - keyrec
// Returns:     short
// Note:        This read has only one parameter...there is another read function
//              that has two parameters.
//==============================================================================
short KeyMaster::read(SKeyRecord* keyrec)
{
   if(!pTheServer)
      return 1;

   if (keyrec->pkey)
      delete keyrec->pkey;
   keyrec->pkey = newKey();
   if (!keyrec->pkey || keyrec->pkey->readKeyData(pTheServer, CurrentKeyID) == FAILURE)
      return 1; // FAILURE

   return 0;    // SUCCESS
}

//==============================================================================
// Function:    resetKey
// Purpose:     resets the key to it's initial state. This does not work for the
//              SOLsearcher keys, and we will therefore have to call ProtectionKey::newKey()
//              again later to ensure that the constructor gets called which will
//              initialize Pkey with the SOLSEARCHER_ENTERPRISE_PRODUCT product Id.
// Parameters:  SKeyRecord* - keyrec
// Returns:     None
//==============================================================================
void KeyMaster::resetKey(SKeyRecord* keyrec, ProductId pid)
{
   if(keyrec->pkey)
      delete keyrec->pkey;
   //keyrec->pkey = newKey(SPD_PRODUCT);
   keyrec->pkey = newKey(pid);
}

//==============================================================================
// Function:    read()
// Purpose:     Finds the protection key and reads its data.
//              Returns 0 for success, 1 otherwise.
// Parameters:  SKeyRecord* - keyrec
//              ProductId - productId
// Returns:     short
//==============================================================================
short KeyMaster::read(SKeyRecord* keyrec, ProductId productId)
{
   if (keyrec->pkey)
      delete keyrec->pkey;
   keyrec->pkey = newKey(productId);
   if (!keyrec->pkey || keyrec->pkey->readKeyData(pTheServer, CurrentKeyID) == FAILURE)
      return 1; // FAILURE

   return 0;    // SUCCESS
}

//==============================================================================
// Function:    setHandle()
// Purpose:     Sets the handle of key to that of the first protection key found.
//              Returns 0 for success, 1 otherwise.
// Parameters:  ProtectionKey* key
// Returns:     short
//==============================================================================
HRESULT KeyMaster::setHandle()
{
   if(pTheServer)
   {
      VARIANT* pKeyName;

      HRESULT hr = pTheServer->KeyEnumerate(pKeyList);

      // SafeArrayAccessData and SafeArrayUnaccessData are win32 functions
		//needed to access safe array data
		if (SUCCEEDED(SafeArrayAccessData(pKeyList->parray, (void**)&pKeyName)))
		{
		   CurrentKeyID = &((BSTR)(pKeyName[0].bstrVal));

			if (SUCCEEDED(SafeArrayUnaccessData(pKeyList->parray)))
				return SUCCESS;
		}
   }

   return E_FAIL;
}

//==============================================================================
// Function:    initializeMaxModules
// Purpose:     Set the module values for all key types that support modules to
//              their maximum values.
// Parameters:  SKeyRecord* - keyrec
// Returns:     None
//==============================================================================
void KeyMaster::initializeMaxModules( SKeyRecord* keyrec )
{
   //
   // SP/D or CONNECTIVITY
   if( keyrec->pkey->productId == SPD_PRODUCT ||
       keyrec->pkey->productId == CONNECT_PRODUCT ||
       keyrec->pkey->productId == QUANTUM_PRODUCT)
   {
      ModuleDetail** module_detail = lookup->getModuleList(SPD_PRODUCT);
      SpdProtectionKey* spd_key((SpdProtectionKey*)(keyrec->pkey));

      for( int mod_id=0; mod_id<64; mod_id++ )
      {
         if( module_detail[mod_id]->isAvailableForVersion(spd_key->productVersion) &&
                module_detail[mod_id]->isAvailableForProduct(spd_key->productId) )
         {
            spd_key->setLicense(mod_id, module_detail[mod_id]->max);
            spd_key->getLicense(mod_id);
         }
         else
         {
            spd_key->setLicense(mod_id, 0);
         }
      }

      if(keyrec->pkey->productVersion > 0x4000)
         spd_key->outputUnits = 4095;
      else
         spd_key->outputUnits = 0;

      if(keyrec->pkey->productVersion > 0x5110)
      {
        keyrec->xch_ipds_ppm = MAX_PAGES_PER_MINUTE;
        keyrec->xch_ps_ppm = MAX_PAGES_PER_MINUTE;
        keyrec->xch_ps_dbcs_ppm = MAX_PAGES_PER_MINUTE;
        keyrec->xch_pcl_ppm = MAX_PAGES_PER_MINUTE;
        keyrec->afpds_ps_ppm = MAX_PAGES_PER_MINUTE;
      }
      else
      {
        keyrec->xch_ipds_ppm = 0;
        keyrec->xch_ps_ppm = 0;
        keyrec->xch_ps_dbcs_ppm = 0;
        keyrec->xch_pcl_ppm = 0;
        keyrec->afpds_ps_ppm = 0;
      }

   }

   //
   // ICONVERT -
   if( keyrec->pkey->productId == ICONVERT_PRODUCT )
   {
      ModuleDetail** iConvert_module_detail = lookup->getModuleList(ICONVERT_PRODUCT);
      SpdProtectionKey* iConvert_key((SpdProtectionKey*)(keyrec->pkey));

      for( int mod_id=0; mod_id<64; mod_id++ )
      {
         if( iConvert_module_detail[mod_id]->isAvailableForVersion(iConvert_key->productVersion) &&
                iConvert_module_detail[mod_id]->isAvailableForProduct(iConvert_key->productId) )
         {
            iConvert_key->setLicense(mod_id, iConvert_module_detail[mod_id]->max);
         }
         else
         {
            iConvert_key->setLicense(mod_id, 0);
         }
      }

      if(keyrec->pkey->productVersion > 0x0000)
         iConvert_key->outputUnits = 4095;
      else
         iConvert_key->outputUnits = 0;

      keyrec->xch_ipds_ppm = 0;
      keyrec->xch_ps_ppm = 0;
      keyrec->xch_ps_dbcs_ppm = 0;
      keyrec->xch_pcl_ppm = 0;
      keyrec->afpds_ps_ppm = 0;
   }

   //
   // SOLSCRIPT_PRODUCT -
   if( keyrec->pkey->productId == SOLSCRIPT_PRODUCT ||
       keyrec->pkey->productId == SDX_DESIGNER_PRODUCT)
   {
      ModuleDetail** ppModuleDetail = lookup->getModuleList(keyrec->pkey->productId);
      SpdProtectionKey* spdKey((SpdProtectionKey*)(keyrec->pkey));

      for (int mod_id=0; mod_id<64; mod_id++) {
         if (ppModuleDetail[mod_id]->isAvailableForVersion(spdKey->productVersion) &&
                ppModuleDetail[mod_id]->isAvailableForProduct(spdKey->productId) ) {
            spdKey->setLicense(mod_id, ppModuleDetail[mod_id]->max);
         }
         else {
            spdKey->setLicense(mod_id, 0);
         }
      }

      ///if(spdKey->productVersion > 0x0000)
      ///   spdKey->outputUnits = 4095;
      ///else
         spdKey->outputUnits = 0;

      keyrec->xch_ipds_ppm = 0;
      keyrec->xch_ps_ppm = 0;
      keyrec->xch_ps_dbcs_ppm = 0;
      keyrec->xch_pcl_ppm = 0;
      keyrec->afpds_ps_ppm = 0;

   }  // end of SOLSCRIPT_PRODUCT code added

}

//==============================================================================
// Function:    initializeMinModules(SKeyRecord*, ModuleDetail**)
// Purpose:     Set the module values to their minimum values
// Parameters:  SKeyRecord* - keyrec
// Returns:     None
//==============================================================================
void KeyMaster::initializeMinModules( SKeyRecord* keyrec)
{

   //
   // SP/D or CONNECTIVITY -
   if( keyrec->pkey->productId == SPD_PRODUCT ||
       keyrec->pkey->productId == CONNECT_PRODUCT ||
       keyrec->pkey->productId == QUANTUM_PRODUCT)
   {
      ModuleDetail** module_detail = lookup->getModuleList(SPD_PRODUCT);
      SpdProtectionKey* spd_key((SpdProtectionKey*)(keyrec->pkey));

      for( int mod_id=0; mod_id<64; mod_id++ )
      {
         if( module_detail[mod_id]->isAvailableForVersion(spd_key->productVersion) &&
                module_detail[mod_id]->isDefaultForProduct(spd_key->productId) )
         {
            spd_key->setLicense(mod_id, module_detail[mod_id]->max);
         }
         else
         {
            spd_key->setLicense(mod_id, 0);
         }
      }
      if(keyrec->pkey->productVersion > 0x4000)
         spd_key->outputUnits = 1;
      else
         spd_key->outputUnits = 0;

      keyrec->xch_ipds_ppm = 0;
      keyrec->xch_ps_ppm = 0;
      keyrec->xch_ps_dbcs_ppm = 0;
      keyrec->xch_pcl_ppm = 0;
      keyrec->afpds_ps_ppm = 0;

   }

   //
   // ICONVERT -
   if( keyrec->pkey->productId == ICONVERT_PRODUCT )
   {
      ModuleDetail** iConvert_module_detail = lookup->getModuleList(ICONVERT_PRODUCT);
      SpdProtectionKey* iConvert_key((SpdProtectionKey*)(keyrec->pkey));

      for( int mod_id=0; mod_id<64; mod_id++ )
      {
         if( iConvert_module_detail[mod_id]->isAvailableForVersion(iConvert_key->productVersion) &&
                iConvert_module_detail[mod_id]->isDefaultForProduct(iConvert_key->productId) )
         {
            iConvert_key->setLicense(mod_id, iConvert_module_detail[mod_id]->max);
         }
         else
         {
            iConvert_key->setLicense(mod_id, 0);
         }
      }

      if(keyrec->pkey->productVersion > 0x0000)
         iConvert_key->outputUnits = 1;
      else
         iConvert_key->outputUnits = 0;

      keyrec->xch_ipds_ppm = 0;
      keyrec->xch_ps_ppm = 0;
      keyrec->xch_ps_dbcs_ppm = 0;
      keyrec->xch_pcl_ppm = 0;
      keyrec->afpds_ps_ppm = 0;

   }  // end of iConvert code added

   //
   // SOLSCRIPT_PRODUCT -
   if( keyrec->pkey->productId == SOLSCRIPT_PRODUCT ||
       keyrec->pkey->productId == SDX_DESIGNER_PRODUCT)
   {
      ModuleDetail** ppModuleDetail = lookup->getModuleList(keyrec->pkey->productId);
      SpdProtectionKey* spdKey((SpdProtectionKey*)(keyrec->pkey));

      for (int mod_id=0; mod_id<64; mod_id++) {
         if (ppModuleDetail[mod_id]->isAvailableForVersion(spdKey->productVersion) &&
                ppModuleDetail[mod_id]->isDefaultForProduct(spdKey->productId) ) {
            spdKey->setLicense(mod_id, ppModuleDetail[mod_id]->max);
         }
         else {
            spdKey->setLicense(mod_id, 0);
         }
      }

      if(spdKey->productVersion > 0x0000)
         spdKey->outputUnits = 1;
      else
         spdKey->outputUnits = 0;

      keyrec->xch_ipds_ppm = 0;
      keyrec->xch_ps_ppm = 0;
      keyrec->xch_ps_dbcs_ppm = 0;
      keyrec->xch_pcl_ppm = 0;
      keyrec->afpds_ps_ppm = 0;

   }  // end of SOLSCRIPT_PRODUCT code added
}

//==============================================================================
// Function:    initializeMaxValues
// Purpose:     Initializes the index servers, reports servers & consurrent users
//              to their max values.
// Parameters:  SKeyRecord* - keyrec
// Returns:     None
//==============================================================================
void KeyMaster::initializeMaxValues( SKeyRecord* keyrec )
{
   //
   // SOLsearcher Enterprise Product
   if( keyrec->pkey->productId == SOLSEARCHER_ENTERPRISE_PRODUCT )
   {
      // reinterpret cast to SSProtectionKey to access functions of that class
      SSProtectionKey* ss_key = ((SSProtectionKey*)(keyrec->pkey));

      ss_key->setIndexServers(MAX_INDEX_SERVERS);
      ss_key->setReportServers(MAX_REPORT_SERVERS);
      ss_key->setConcurrentUsers(MAX_CONCURRENT_USERS);
      ss_key->setApplications(MAX_APPLICATIONS);
   }
}

//==============================================================================
// Function:    initializeMinValues
// Purpose:     Initializes the index servers, reports servers & consurrent users
//              to their minimum values.
// Parameters:  SKeyRecord* - keyrec
// Returns:     None
//==============================================================================
void KeyMaster::initializeMinValues( SKeyRecord* keyrec )
{
   //
   // SOLsearcher Enterprise Product
   if( keyrec->pkey->productId == SOLSEARCHER_ENTERPRISE_PRODUCT )
   {
      // cast to SSProtectionKey to access functions of that class
      SSProtectionKey* ss_key = ((SSProtectionKey*)(keyrec->pkey));

      ss_key->setIndexServers(0);
      ss_key->setReportServers(0);
      ss_key->setConcurrentUsers(0);
      ss_key->setApplications(0);
   }

}
//==============================================================================
// Function:    applyVersionPassword()
// Purpose:
// Parameters:  SKeyRecord* - keyrec
//              unsigned short - version
// Returns:     None
//==============================================================================
void KeyMaster::applyVersionPassword(SKeyRecord* keyrec, unsigned short version)
{
   //
   // Set version
   keyrec->pkey->productVersion = version;


   //
   // Check if key product type supports module licensing
   if( keyrec->pkey->productId == SPD_PRODUCT ||
       keyrec->pkey->productId == CONNECT_PRODUCT ||
       keyrec->pkey->productId == QUANTUM_PRODUCT ||
       keyrec->pkey->productId == SOLSCRIPT_PRODUCT ||
       keyrec->pkey->productId == SDX_DESIGNER_PRODUCT)
   {
      ModuleDetail** ppModuleDetail = lookup->getModuleList(keyrec->pkey->productId);
      SpdProtectionKey* pSpdKey((SpdProtectionKey*)(keyrec->pkey));

      // Zero out all modules except those that are default
      for (int mod_idx(0);mod_idx<64; mod_idx++) {
         //if available default set to max value
         if( ppModuleDetail[mod_idx]->isAvailableForVersion(pSpdKey->productVersion) &&
             ppModuleDetail[mod_idx]->isDefaultForProduct(pSpdKey->productId) )
         {
            pSpdKey->setLicense(mod_idx, ppModuleDetail[mod_idx]->max);
         }
      }
   }
}
//==============================================================================
// Function:    applyExtensionPassword()
// Purpose:
// Parameters:  SKeyRecord* - keyrec
//              unsigned char - days
// Returns:     None
// Note:        Must new apply status before calling setCounterDays().
//==============================================================================
void KeyMaster::applyExtensionPassword(SKeyRecord* keyrec, unsigned char days )
{
   // set new status, set counter
   keyrec->pkey->status = keyrec->getNextExtension();
   keyrec->pkey->setCounterDays(days);
}

//==============================================================================
// Function:    applyModZeroPassword()
// Purpose:
// Parameters:  SKeyRecord* keyrec,
//              unsigned short module_id
// Returns:     None
//==============================================================================
void KeyMaster::applyModZeroPassword(SKeyRecord* keyrec, unsigned short module_id, unsigned short unit)
{
   //make key permanent
   applyPermanentPassword(keyrec);

   //increment module to 1
   SpdProtectionKey* spd_key((SpdProtectionKey*)(keyrec->pkey));
   spd_key->setLicense(module_id, ushort(unit));

}

//==============================================================================
// Function:    applyPagesPerMinutePassword()
// Purpose:
// Parameters:  SKeyRecord* keyrec,
// Returns:     None
//==============================================================================
void KeyMaster::applyPagesPerMinutePassword(SKeyRecord* keyrec)
{
   // make key permanent
   SpdProtectionKey* spd_key((SpdProtectionKey*)(keyrec->pkey));

   if( spd_key->isOnTrial() )
   {
      applyPermanentPassword(keyrec);
   }
}

//==============================================================================
// Function:    applyOutputPassword
// Purpose:
// Parameters:  (SKeyRecord* key_record, int output_units)
// Returns:     None
//==============================================================================
void KeyMaster::applyOutputPassword(SKeyRecord* key_record, int output_units)
{
   // int mod_id_idx;
   SpdProtectionKey* spd_key((SpdProtectionKey*)(key_record->pkey));

   if( spd_key->isOnTrial() )
   {
      applyPermanentPassword(key_record);
   }

   // set total output units
   spd_key->outputUnits = output_units;
}

//==============================================================================
// Function:    applyModZeroPassword()
// Purpose:     Apply a module password.
// Parameters:
// Returns:
//==============================================================================
void KeyMaster::applyModPassword(SKeyRecord* keyrec, unsigned char module_id, unsigned short units)
{
   SpdProtectionKey* spd_key((SpdProtectionKey*)(keyrec->pkey));

   // increment module
   //spd_key->setLicense(module_id, spd_key->getLicense(module_id)+1);
   spd_key->setLicense(module_id, units+1);
}

//==============================================================================
// Function:    applyIndexServersPassword()
// Purpose:
// Parameters:  SKeyRecord* keyrec,
// Returns:     None
//==============================================================================
void KeyMaster::applyIndexServersPassword(SKeyRecord* keyrec,
                                          unsigned short units_licensed)
{
   // make key permanent
   SSProtectionKey* ss_key = ((SSProtectionKey*)(keyrec->pkey));

   if( ss_key->isOnTrial() )
   {
      applyPermanentPassword(keyrec);
   }

   ss_key->setIndexServers(units_licensed);
}

//==============================================================================
// Function:    applyReportServersPassword()
// Purpose:
// Parameters:  SKeyRecord* keyrec,
// Returns:     None
//==============================================================================
void KeyMaster::applyReportServersPassword(SKeyRecord* keyrec,
                                           unsigned short units_licensed)
{
   // make key permanent
   SSProtectionKey* ss_key = ((SSProtectionKey*)(keyrec->pkey));
   if( ss_key->isOnTrial() )
   {
      applyPermanentPassword(keyrec);
   }

   ss_key->setReportServers(units_licensed);
}

//==============================================================================
// Function:    applyConcurrentUsersPassword()
// Purpose:
// Parameters:  SKeyRecord* keyrec,
// Returns:     None
//==============================================================================
void KeyMaster::applyConcurrentUsersPassword(SKeyRecord* keyrec,
                                             unsigned short units_licensed)
{
   //
   // cast ss_key
   SSProtectionKey* ss_key = ((SSProtectionKey*)(keyrec->pkey));
   if( ss_key->isOnTrial() )
   {
      applyPermanentPassword(keyrec);
   }

   ss_key->setConcurrentUsers(units_licensed);
}


//==============================================================================
// Function:    applyApplicationPassword()
// Purpose:
// Parameters:  SKeyRecord* keyrec,
// Returns:     None
//==============================================================================
void KeyMaster::applyApplicationPassword(SKeyRecord* keyrec,
                                             unsigned short units_licensed)
{
   //
   // cast ss_key
   SSProtectionKey* ss_key = ((SSProtectionKey*)(keyrec->pkey));
   if( ss_key->isOnTrial() )
   {
      applyPermanentPassword(keyrec);
   }

   ss_key->setApplications(units_licensed);
}


//==============================================================================
// Function:    applyPermanentPassword
// Purpose:     Sets key_record to look like a permanent password had been
//              applied.  In reality, permanent password zero out all modules,
//              default modules are set when a version password is applied.
// Parameters:  SKeyRecord* key_record
// Returns:     None
//==============================================================================
void KeyMaster::applyPermanentPassword(SKeyRecord* key_record)
{
   //
   // If it is a Non-Permanent Key Type, then return
   if (key_record->non_perm_ktf == true)
        return;

   //
   // check if key product type supports module licensing
   if (key_record->pkey->productId == SPD_PRODUCT ||
       key_record->pkey->productId == CONNECT_PRODUCT ||
       key_record->pkey->productId == QUANTUM_PRODUCT ||
       key_record->pkey->productId == ICONVERT_PRODUCT) {

      SpdProtectionKey* spd_key((SpdProtectionKey*)(key_record->pkey));

      //
      // set outputUnits = 1 /*new_licensing*/
      spd_key->outputUnits = 1;
      key_record->xch_ipds_ppm = 0;
      key_record->xch_ps_ppm = 0;
      key_record->xch_ps_dbcs_ppm = 0;
      key_record->xch_pcl_ppm = 0;
      key_record->afpds_ps_ppm = 0;

      if (key_record->pkey->productId != ICONVERT_PRODUCT) {
         //
         // CREATES the pages per minute values in the registry.
         spd_key->setPagesPerMinute(XCH_PS_ID, 0);
         spd_key->setPagesPerMinute(XCH_PCL_ID, 0);
         spd_key->setPagesPerMinute(XCH_PS_DBCS_ID, 0);
         spd_key->setPagesPerMinute(XCH_IPDS_ID, 0);
         spd_key->setPagesPerMinute(AFPDS_PS_ID, 0);
      }

      //
      // zero out all modules except those that are default
      ModuleDetail** ppModuleDetail = lookup->getModuleList(key_record->pkey->productId);
      for (int mod_idx(0);mod_idx<64; mod_idx++) {
         if( ppModuleDetail[mod_idx]->isAvailableForVersion(spd_key->productVersion) &&
             ppModuleDetail[mod_idx]->isDefaultForProduct(spd_key->productId) ) {
            spd_key->setLicense(mod_idx, ppModuleDetail[mod_idx]->max);
         }
         else
            spd_key->setLicense(mod_idx, 0);
      }
   }
   else if (key_record->pkey->productId == SOLSCRIPT_PRODUCT ||
            key_record->pkey->productId == SDX_DESIGNER_PRODUCT) {

      SpdProtectionKey* spd_key((SpdProtectionKey*)(key_record->pkey));

      spd_key->outputUnits = 0; //does not belong to SOLSCRIPT
      key_record->xch_ipds_ppm = 0;
      key_record->xch_ps_ppm = 0;
      key_record->xch_ps_dbcs_ppm = 0;
      key_record->xch_pcl_ppm = 0;
      key_record->afpds_ps_ppm = 0;

      //
      // zero out all modules except those that are default
      ModuleDetail** ppModuleDetail = lookup->getModuleList(key_record->pkey->productId);
      for (int mod_idx(0);mod_idx<64; mod_idx++) {
         if( ppModuleDetail[mod_idx]->isAvailableForVersion(spd_key->productVersion) &&
             ppModuleDetail[mod_idx]->isDefaultForProduct(spd_key->productId) ) {
            spd_key->setLicense(mod_idx, ppModuleDetail[mod_idx]->max);
         }
         else
            spd_key->setLicense(mod_idx, 0);
      }
   }

   //
   // SOLSEARCHER ENTERPRISE
   //
   if( key_record->pkey->productId == SOLSEARCHER_ENTERPRISE_PRODUCT )
   {
      SSProtectionKey* ss_key;
      ss_key = ((SSProtectionKey*)(key_record->pkey));

      ss_key->setIndexServers(0);
      ss_key->setReportServers(0);
      ss_key->setConcurrentUsers(0);
      ss_key->setApplications(0);
   }

   //set status to permanent
   key_record->pkey->status = 2;
}

HRESULT KeyMaster::InitPasswordPacket()
{
   return pTheServer->PasswordPacketInitialize();
}

HRESULT KeyMaster::FinalizePasswordPacket()
{
    return pTheServer->PasswordPacketFinalize();
}

HRESULT KeyMaster::AppendPasswordToPacket(VARIANT vtExpires, BSTR password)
{
   return pTheServer->PasswordPacketAppendPassword(vtExpires, password);
}

HRESULT KeyMaster::GetPasswordPacket(VARIANT* pvtPacketData)
{
   return pTheServer->PasswordPacketGetPacket(pvtPacketData);
}
HRESULT KeyMaster::GetVerificationCode(BSTR* VerificationCode)
{
   return pTheServer->PasswordPacketGetVerificationCode(VerificationCode);
}

