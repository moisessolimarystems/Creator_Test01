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

#define OVERPASSWORD1			0xC822
#define OVERPASSWORD2			0x85A2

// statusLineText codes
#define OPERATING_ON_KEY            1



/*--------------------------------------------------------------------*
 *                                                                    *
 *   KeyMaster Class:                                                 *
 *                                                                    *
 *   This class provides the Protection Key interface for the Creator.*
 *                                                                    *
 *--------------------------------------------------------------------*/

//==============================================================================
// Function:    clear()
// Purpose:     Initialize all cells to 0.  Returns 0 for success, 1 otherwise.
// Parameters:  None
// Returns:     short
// Note:
//==============================================================================
short KeyMaster::clear()
{
   if (setHandle(&scratchkey) ||
         scratchkey.clearKeyData(OVERPASSWORD1, OVERPASSWORD2) == FAILURE)
   return 1;

   return 0;
}


//==============================================================================
// Function:    deactivate()
// Purpose:     Initializes key and bump key uses variable.  Returns 0 for success,
//              1 otherwise.
// Parameters:  None
// Returns:     short
// Note:
//==============================================================================
short KeyMaster::deactivate()
{
   if (setHandle(&scratchkey))
      return 1;
   if (scratchkey.readUses() == FAILURE)
      return 1;
   if (scratchkey.clearKeyData(OVERPASSWORD1, OVERPASSWORD2) == FAILURE)
      return 1;
   scratchkey.uses++;
   if (scratchkey.writeUses() == FAILURE)
      return 1;
   return 0;
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
   ProtectionKey::sproInitialize(&packet, server_name);
   return ProtectionKey::sproFindFirstUnit(&packet);
}

//==============================================================================
// Function:    hasModules()
// Purpose:     Returns true or false depending on whether the product has
//              Modules or not.  SPD, CONNECTIVITY SERVER, or ICONVERT
// Parameters:  SKeyRecord* - keyrec
// Returns:     bool
// Note:
//==============================================================================
bool KeyMaster::hasModules(SKeyRecord* keyrec)
{
   if(keyrec->pkey->productId==SPD_PRODUCT ||
      keyrec->pkey->productId==CONNECT_PRODUCT ||
      keyrec->pkey->productId==QUANTUM_PRODUCT ||
      keyrec->pkey->productId==ICONVERT_PRODUCT ||
      keyrec->pkey->productId==SOLSCRIPT_PRODUCT)
      return true;
   else
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
ulong KeyMaster::getPermanentPassword(ProtectionKey* key)
{
   if (setHandle(key))
      return 0;
   return key->getBasePassword();
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
ulong KeyMaster::getExtensionPassword(ProtectionKey* key,
                                      uchar extension_days,
                                      ProductId product_id,
                                      ushort product_version,
                                      ushort extension_num)
{
   if (setHandle(key))
      return 0;
   return key->getExtensionPassword(extension_days, product_id, product_version, extension_num);
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
ulong KeyMaster::getModulePassword(SpdProtectionKey* key,
                                   uchar mod_id,
                                   ProductId product_id,
                                   ushort product_version,
                                   ushort units)
{
   if (setHandle(key))
      return 0;
   //return key->getModulePassword(mod_id, key->getLicense(mod_id), product_id, product_version);
   return key->getModulePassword(mod_id, units, product_id, product_version);
}


//==============================================================================
// Function:    getModZeroPassword()
// Purpose:     Get zero password for module mod_id.  Returns password for success,
//              0 otherwise.
// Parameters:  ProtectionKey* - key
//              uchar - mod_id
//              ProductId - product_id
//              ushort - product_version
// Returns:     ulong
// Note:
//==============================================================================
/*ulong KeyMaster::getModZeroPassword(SpdProtectionKey* key,
                                    uchar mod_id,
                                    ProductId product_id,
                                    ushort product_version)
{
   if (setHandle(key))
      return 0;
   return key->getModZeroPassword(mod_id, product_id, product_version);
}*/


//==============================================================================
// Function:    getOutputPassword()
// Purpose:
// Parameters:  ProtectionKey* - key
//              ushort - output_units
// Returns:     ulong
// Note:
//==============================================================================
ulong KeyMaster::getOutputPassword(SpdProtectionKey* key,
                                   ushort output_units )
{
   if(setHandle(key))
   	return 0;
   return key->getOutputPassword(output_units);
}

//==============================================================================
// Function:    getPagesPerMinutePassword()
// Purpose:
// Parameters:  ProtectionKey* - key
//              ushort - output_units
// Returns:     ulong
// Note:
//==============================================================================
ulong KeyMaster::getPagesPerMinutePassword(SpdProtectionKey* key,
                                           ushort ext )
{
   if(setHandle(key))
   	return 0;
   return key->getPagesPerMinutePassword(ext);
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
ulong KeyMaster::getProductVersionPassword(ProtectionKey* key,
                                           ushort product_version)
{
   if (setHandle(key))
      return 0;
   return key->getProductVersionPassword(product_version);
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
ulong KeyMaster::getIndexServersPassword(SSProtectionKey* key,
                                         ushort units_licensed)
{
   if (setHandle(key))
      return 0;
   return key->getIndexServersPassword(units_licensed);
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
ulong KeyMaster::getReportServersPassword(SSProtectionKey* key,
                                          ushort units_licensed)
{
   if (setHandle(key))
      return 0;
   return key->getReportServersPassword(units_licensed);
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
ulong KeyMaster::getConcurrentUsersPassword(SSProtectionKey* key,
                                            ushort units_licensed)
{
   if (setHandle(key))
      return 0;
   return key->getConcurrentUsersPassword(units_licensed);
}

ulong KeyMaster::getApplicationServerPassword(SSProtectionKey* key,
                                              ushort units_licensed)
{
   if (setHandle(key))
      return 0;
   return key->getApplicationsPassword(units_licensed);
}

//==============================================================================
// Function:    initDriver()
// Purpose:     Initialize SuperPro driver.  Returns 0 for success, 1 otherwise.
// Parameters:  None
// Returns:     short
// Note:
//==============================================================================
short KeyMaster::initDriver()
{
   return ProtectionKey::sproInitialize(&packet, server_name);
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
   ProtectionKey::sproInitialize(&packet, server_name);
   return ProtectionKey::allocFirstUnit(&packet);
}

//==============================================================================
// Function:    newKey()
// Purpose:     Return a new key object based on the product ID found in the
//              KeyDataBlock passed as a parameter.
// Parameters:  KeyDataBlock* - key_data_block
// Returns:     ProtectionKey*
// Note:
//==============================================================================
ProtectionKey* KeyMaster::newKey(KeyDataBlock* key_data_block)
{
   scratchkey.keyDataBlock = *key_data_block;
   return ProtectionKey::newKey(ProductId(scratchkey.productId), &packet);
   // Not here
}

//==============================================================================
// Function:    program()
// Purpose:     Programs the protection key.   Returns 0 for success, 1 otherwise.
// Parameters:  None
// Returns:     short
// Note:
//==============================================================================
short KeyMaster::program(ProtectionKey* key)
{
   if (setHandle(key) ||
         key->clearKeyData(OVERPASSWORD1, OVERPASSWORD2) == FAILURE ||
         key->writeKeyData() == FAILURE)
      return 1;

   return 0;
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
   //
   // Tries to find the first protection key & sets the handle
   if (setHandle(&scratchkey) || scratchkey.readCustomerNumber() == FAILURE)
      return false;

   return bool(scratchkey.customerNumber);
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
   if (keyrec->pkey)
      delete keyrec->pkey;
   keyrec->pkey = newKey();
   if (!keyrec->pkey || keyrec->pkey->readKeyData() == FAILURE)
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
   if (!keyrec->pkey || keyrec->pkey->readKeyData() == FAILURE)
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
short KeyMaster::setHandle(ProtectionKey* key)
{
   // call sproFindFirstUnit() to obtain packet handle (FAILURE)
   ProtectionKey::sproInitialize(&packet, server_name);
   if (!ProtectionKey::sproFindFirstUnit(&packet))
      return 1;

   //
   // set key's packet handle (SUCCESS)
   key->setPacket(&packet);

   return 0;
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
   if( keyrec->pkey->productId == SOLSCRIPT_PRODUCT )
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
   if( keyrec->pkey->productId == SOLSCRIPT_PRODUCT )
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
       keyrec->pkey->productId == SOLSCRIPT_PRODUCT)
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
///void KeyMaster::applyOutputPassword(SKeyRecord* key_record, int output_units, ModuleDetail** module_detail)
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

   // key status changed to permanent
   //spd_key->status = 2; //applyModPassword should only be called if key status is already 2

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
   else if (key_record->pkey->productId == SOLSCRIPT_PRODUCT) {

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



