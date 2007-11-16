//------------------------------------------------------------*
// filename -       kyslevel.cpp                              *
//                                                            *
//        TV User Interface for the Keys Level of the Creator.*
//                                                            *
// Class(es):                                                 *
//        SDate                                               *
//        SKeyRecord                                          *
//------------------------------------------------------------*
//------------------------------------------------------------*
//                                                            *
//    The Creator                                             *
//                                                            *
//    Copyright (c) 1993-2001 by Solimar Systems, Inc.        *
//    All Rights Reserved.                                    *
//                                                            *
//------------------------------------------------------------*
#include <time.h>
#include <string.h>     // strXXX()
#include <stdio.h>      // sprintf()

#include "keyform.h"
#include "kyslevel.h"
#include "KeyWizard.h"

/*--------------------------------------------------------------------*
 *                                                                    *
 *   SKeyRecord Class:                                                *
 *                                                                    *
 *   Contains database data for one protection key.                   *
 *                                                                    *
 *--------------------------------------------------------------------*/
 /* static members
---------------------------------------------------------------------------*/
char* SKeyRecord::text = new char[255];

const char* status_text[] =
{
   "Initial",
   "Extension 1",
   "Permanent",
   "Unitialized",
   "Extension 2",
   "Extension 3",
   "Extension 4",
   "Extension 5",
   " ",
   " ",
   "Unused",
   "Deactivated",
   "Extension 6",
   "Extension 7",
   "Extension 8",
   "Extension 9",
   "Extension 10",
   "Extension 11",
   "Extension 12",
   "Extension 13",
   "Extension 14",
   "Extension 15",
};


//==============================================================================
// Function:    SKeyRecord
// Purpose:     constructor- initializes members.
//==============================================================================
SKeyRecord::SKeyRecord() :
   pkey(NULL), parent_skr_id(0), skr_id(0)
{
   memset(description, 0, sizeof(description));
   activationDate = time(NULL);//set to current time
   num_days = 0;
   memset(module_id_array, -1, sizeof(module_id_array));
   memset(license_count_array, -1, sizeof(license_count_array));
}


//==============================================================================
// Function:    SKeyRecord
// Purpose:     copy constructor
// Parameters:  const SKeyRecord& keyrec
//==============================================================================
SKeyRecord::SKeyRecord(const SKeyRecord& keyrec)
{
   pkey = keyrec.pkey ? ProtectionKey::newKey(keyrec.pkey) : NULL;
   memcpy(description, keyrec.description, sizeof(description));
}


//==============================================================================
// Function:    operator=()
// Purpose:
// Parameters:  const SKeyRecord& - keyrec
// Returns:     SKeyRecord&
//==============================================================================
SKeyRecord& SKeyRecord::operator=(const SKeyRecord& keyrec)
{
   if (this != &keyrec)  // beware of x=x
   {
      if (pkey)
         delete pkey;
      pkey = keyrec.pkey ? ProtectionKey::newKey(keyrec.pkey) : NULL;
      memcpy(description, keyrec.description, sizeof(description));
   }
   return *this;
}


//==============================================================================
// Function:    ~SKeyRecord()
// Purpose:     destructor
//==============================================================================
SKeyRecord::~SKeyRecord()
{
   if (pkey)
      delete pkey;
}


//==============================================================================
// Function:    getKeyNumber
// Purpose:     returns the Key Number for this Key.
// Parameters:  None
// Returns:     ushort
//==============================================================================
ushort SKeyRecord::getKeyNumber()
{
   return(pkey->keyNumber);
}


//==============================================================================
// Function:    setKeyNumber
// Purpose:     sets the Key Number for this Key.
// Parameters:  ushort - key_number
// Returns:     None
//==============================================================================
void SKeyRecord::setKeyNumber(ushort key_number)
{
   pkey->keyNumber = key_number;
}


//==============================================================================
// Function:    getCustomerNumber
// Purpose:     returns the Customer Number for this Key.
// Parameters:  None
// Returns:     ushort
//==============================================================================
ushort SKeyRecord::getCustomerNumber()
{
   return(pkey->customerNumber);
}

//==============================================================================
// Function:    getKeyTypeEnumValue
// Purpose:     returns the Enumerated value for the key type for the corresponding
//              integer type. The value stored in the database are integer values.
//              ex. KEYNone = enumerated type
//                  ktfNone = integer type
// Parameters:  None
// Returns:     ushort
//==============================================================================
ushort SKeyRecord::getKeyTypeEnumValue(int iIntegerValue)
{
   switch(iIntegerValue)
   {
        case ktfNone:
           return KEYNone;
        case ktfAddon:
           return KEYAddon;
        case ktfBase:
           return KEYBase;
        case ktfReplacement:
           return KEYReplacement;
        case ktfRelicense:
           return KEYRelicense;
        case ktfInventory:
           return KEYInventory;
        case ktfBackup:
           return KEYBackup;
        case ktfEmergency:
           return KEYEmergency;
        case ktfCustom:
           return KEYCustom;
        case ktfLoan:
           return KEYLoan;
        case ktfDemo:
           return KEYDemo;
        case ktfRental:
           return KEYRental;
        case ktfDevelopment:
           return KEYDevelopment;
        case ktfReserved:
           return KEYReserved;
        default:
           return ktfNone;
   }  // end switch
}

//==============================================================================
// Function:    getKeyTypeIntegerValue
// Purpose:     returns the Integer value for the key type from the corresponding
//              enumerated type. The enumerated types are defined in PKey.h
//              ex. KEYNone = enumerated type
//                  ktfNone = integer type
// Parameters:  None
// Returns:     ushort
//==============================================================================
int SKeyRecord::getKeyTypeIntegerValue(ushort iEnumValue)
{
   switch(iEnumValue)
   {
        case KEYNone:
           return ktfNone;
        case KEYAddon:
           return ktfAddon;
        case KEYBase:
           return ktfBase;
        case KEYReplacement:
           return ktfReplacement;
        case KEYRelicense:
           return ktfRelicense;
        case KEYInventory:
           return ktfInventory;
        case KEYBackup:
           return ktfBackup;
        case KEYEmergency:
           return ktfEmergency;
        //case KEYSolimar
        //   return ktfSolimar;
        case KEYCustom:
           return ktfCustom;
        case KEYLoan:
           return ktfLoan;
        case KEYDemo:
           return ktfDemo;
        case KEYRental:
           return ktfRental;
        case KEYDevelopment:
           return ktfDevelopment;
        case KEYReserved:
           return ktfReserved;
        default:
           return ktfNone;
   } // end switch
}


//==============================================================================
// Function:    setCustomerNumber
// Purpose:     sets the Customer Number for this Key.
// Parameters:  ushort - customer_number
// Returns:     None
//==============================================================================
void SKeyRecord::setCustomerNumber(ushort customer_number)
{
   pkey->customerNumber = customer_number;
}


//==============================================================================
// Function:    getUses
// Purpose:     returns the uses for this Key.
// Parameters:  None
// Returns:     ushort
//==============================================================================
ushort SKeyRecord::getUses()
{
   return(pkey->uses);
}


//==============================================================================
// Function:    getStatus
// Purpose:     returns the status of this Key.
// Parameters:  None
// Returns:     ushort
//==============================================================================
ushort SKeyRecord::getStatus()
{
   return(pkey->status);
}

//==============================================================================
// Function:    getProductId
// Purpose:     returns the productId of this Key.
// Parameters:  None
// Returns:     ushort
//==============================================================================
ushort SKeyRecord::getProductId()
{
    return(pkey->productId);
}

//==============================================================================
// Function:    setStatus
// Purpose:     sets the status of this Key.
// Parameters:  ushort status
// Returns:     None
// Note:        setCustomerNumber() - sets the status of this Key.
//==============================================================================
void SKeyRecord::setStatus(ushort status)
{
   pkey->status = status;
}

//==============================================================================
// Function:    getNextExtensionP
// Purpose:     returns status for next trial, returns UNUSED if fails.
// Parameters:  None
// Returns:     unsigned short
// Note:        getNextTrialStatus()
//==============================================================================
unsigned short SKeyRecord::getNextExtensionP()
{
   unsigned short extNum(0);
   switch (pkey->status)
   {
      case INITIAL_TRIAL:
         extNum = 0;
         break;
      case EXTENDED_TRIAL:
         extNum = 1;
         break;
      case EXTENDED_TRIAL2:
      case EXTENDED_TRIAL3:
      case EXTENDED_TRIAL4:
      case EXTENDED_TRIAL5:
         extNum = pkey->status - LEGACY_EXTENSION_OFFSET;
         break;
      case EXTENDED_TRIAL6:
      case EXTENDED_TRIAL7:
      case EXTENDED_TRIAL8:
      case EXTENDED_TRIAL9:
      case EXTENDED_TRIAL10:
      case EXTENDED_TRIAL11:
      case EXTENDED_TRIAL12:
      case EXTENDED_TRIAL13:
      case EXTENDED_TRIAL14:
      case EXTENDED_TRIAL15:
      case EXTENDED_TRIAL16:
         extNum = pkey->status - EXTENSION_OFFSET;
         break;
   }
   return extNum;
}

//==============================================================================
// Function:    getNextExtension
// Purpose:
// Parameters:  None
// Returns:     unsigned short
//==============================================================================
unsigned short SKeyRecord::getNextExtension()
{
   unsigned short next_status(UNUSED);

   switch (pkey->status)
   {
      case EXTENDED_TRIAL:
         next_status = EXTENDED_TRIAL2;
         break;
      case EXTENDED_TRIAL5 :
         next_status = EXTENDED_TRIAL6;
         break;
      case INITIAL_TRIAL:
      case EXTENDED_TRIAL2:
      case EXTENDED_TRIAL3:
      case EXTENDED_TRIAL4:
      case EXTENDED_TRIAL6:
      case EXTENDED_TRIAL7:
      case EXTENDED_TRIAL8:
      case EXTENDED_TRIAL9:
      case EXTENDED_TRIAL10:
      case EXTENDED_TRIAL11:
      case EXTENDED_TRIAL12:
      case EXTENDED_TRIAL13:
      case EXTENDED_TRIAL14:
      case EXTENDED_TRIAL15:
      case EXTENDED_TRIAL16:
         next_status = ++pkey->status;
         break;
   }
   return next_status;
}
//==============================================================================
// Function:    getKeyTypeText
// Purpose:     Returns the key type text value for each corresponding enumerated
//              type value defined in pkey.h
// Parameters:  None
// Returns:     ushort
//==============================================================================
const char* SKeyRecord::getKeyTypeText(ushort iEnumValue)
{
   switch(iEnumValue)
   {
        case KEYNone:
           return "None";
        case KEYAddon:
           return "Add-on";
        case KEYBase:
           return "Base";
        case KEYReplacement:
           return "Replacement";
        case KEYRelicense:
           return "Relicense";
        case KEYInventory:
           return "Inventory";
        case KEYBackup:
           return "Backup";
        case KEYEmergency:
           return "Emergency";
        //case KEYSolimar:
        //   return "Solimar";
        case KEYCustom:
           return "Custom";
        case KEYLoan:
           return "Loan";
        case KEYDemo:
           return "Demo";
        case KEYRental:
           return "Rental";
        case KEYDevelopment:
           return "Test/Dev";
        case KEYReserved:
           return "Reserved";
        default:
           return 0;
   }
}

//==============================================================================
// Function:    getVersionText
// Purpose:     Returns the formatted verion text.
// Parameters:  None
// Returns:     const char*
//==============================================================================
const char* SKeyRecord::getVersionText()
{
   int version( pkey->productVersion );
   sprintf( text, "%X.%03X", version/0x1000, version&0x0fff );
   return( text );
}

//==============================================================================
// Function:    getProductText
// Purpose:     calls pkey->getProductText() which returns text based on the
//              product id of the key.
// Parameters:  None
// Returns:     const char*
//==============================================================================
const char* SKeyRecord::getProductText()
{
   return pkey->getProductText();
}

//==============================================================================
// Function:    getStatusText()
// Purpose:     Returns licensing text base on the status parameter.  The status
//              parameter defaults to the status of the ProtectionKey::pkey
//              member of SKeyRecord if not status is passed when calling the
//              function.
// Parameters:  unsigned short - status
// Returns:     const char*
//==============================================================================
const char* SKeyRecord::getStatusText(unsigned short status)
{
   //check if return status has been '&' over key's origninal status
   if( status & S_RETURNED )
   	return "RETURNED";
   else if( status & S_LOST )
   	return "LOST";
   else if( status < sizeof(status_text) )
   	return status_text[status];
   else
      return "Unknown";
}

void SKeyRecord::setAppInstances(ushort value)
{
    appInstances = value;
}

void SKeyRecord::setPasswordNumber(ushort value)
{
    passwordNumber = value;
}

unsigned short SKeyRecord::incrementPasswordNumber()
{

      switch(getProductId())
      {
          case SPDE_PRODUCT :
          case RUBIKA_PRODUCT :
          case SDX_DESIGNER_PRODUCT :
          case SOLSEARCHER_ENTERPRISE_PRODUCT :
               passwordNumber++;
               break;
          case ICONVERT_PRODUCT :
               if(pkey->productVersion > 0x9050)
                  passwordNumber++;
               break;
          default :
               break;
      }
      return passwordNumber;
}

unsigned short SKeyRecord::decrementPasswordNumber()
{
      switch(getProductId())
      {
          case SPDE_PRODUCT :
          case RUBIKA_PRODUCT :
          case SDX_DESIGNER_PRODUCT :
          case SOLSEARCHER_ENTERPRISE_PRODUCT :
               passwordNumber--;
               break;
          case ICONVERT_PRODUCT :
               if(pkey->productVersion > 0x9050)
                  passwordNumber--;
               break;
          default :
               break;
      }
      return passwordNumber;
}



