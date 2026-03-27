//-------------------------------------------------------------------------*
//    filename - pkey.cpp                                                  *
//                                                                         *
//    Class(es):                                                           *
//       ProtectionKey                                                     *
//                                                                         *
//-------------------------------------------------------------------------*
//-------------------------------------------------------------------------*
//                                                                         *
//    Solimar Print/Director                                               *
//                                                                         *
//    Copyright (c) 1992-1996 by Solimar Systems, Inc.                     *
//    All Rights Reserved.                                                 *
//                                                                         *
//-------------------------------------------------------------------------*
//-------------------------------------------------------------------------*
//                                                                         *
//    Change history:                                                      *
//    Nm    Date        Version  Description                               *
//    DCM   16.sep.93            replaced pow() functions with shift       *
//                               operations, fixed POW messages w/ega      *
//                               and mono displays                         *
//    SLB   19.sep.93   1.01     Added copyright notice and version.       *
//    DCM   22.sep.93   1.02     replaced timer.h hour counting with       *
//                               time.h hour counting to eliminate         *
//                               hardware/OS dependency.                   *
//    DCM   27.sep.93   1.03     checks the hour counter even if one       *
//                               hour hasn't gone by.                      *
//    DCM   21.jan.94   2.00     Multiple key ability! Restructuring       *
//                               of functionality. Compatability with      *
//                               Creator v2.00.                            *
//    DCM   11.feb.94   2.01     Warning message compatibility with        *
//                               old keys.                                 *
//    DCM   18.feb.94   2.02     Created keytext.cpp.                      *
//    DCM   27.jun.94   2.10     reformatted to work w/SPD 3.xx            *
//    DCM   16.sep.94   2.11     changed warning messages a bit            *
//    DCM   05.oct.94   2.12     max extension days = 90                   *
//    DCM   26.oct.94   2.13     Added uninitialized status.               *
//    DCM   01.mar.95   2.20     Added SP/D version info.                  *
//    DCM   13.may.96   3.00     Changed class hierarchy: incorporated     *
//                               KeyData into ProtectionKey.  Added        *
//                               SpdProtectionKey which is derived from    *
//                               ProtectionKey.  Added SpdKeyManager which *
//                               is derived from KeyManager.  The SP/D     *
//                               specific info was moved into these new    *
//                               classes.  The base classes ProtectionKey  *
//                               and KeyManager are intended to work for   *
//                               any Solimar product, TV or Windows based. *
//    BRD   14.may.96            Made windows message box System Modal.    *
//                               Some applications would override the      *
//                               messagebox, and add a shade style. This   *
//                               would cause GP faults in USER.EXE. By     *
//                               making the message box System Modal, the  *
//                               application couldn't override it.         *
//                               This error may only occcur in DLL's, but  *
//                               I'm not sure.                             *
//    DCM   07.feb.97   3.01     Added support for the Win32 Rainbow API,  *
//                               which has a slightly different interface  *
//                               than the DOS/Windows 16-bit API. Changes  *
//                               were made through the use of macros and   *
//                               should have no effect on the 16-bit code. *
//                               Moved all UI code into KEYUI.CPP. Created *
//                               functions to access arrays of strings and *
//                               do bounds checking to avoid GP faults.    *
//                               Added support for XImageNT and SolPclNT   *
//                               products.                                 *
//    DCM   02.dec.97   3.10     Modified for SPDNT. Removed all SPD DOS   *
//                               code. Added multiple extension support.   *
//                               Added capability to specify version when  *
//                               entering product version passwords.       *
//                               Deleted file keyui.cpp. Borland Win32     *
//                               apps can use keypassworddlg.cpp and       *
//                               keymessagebox.cpp, other apps will need   *
//                               to define keyPasswordDlg() and            *
//                               keyMessageBox() within their own files    *
//                               and include those in their key library.   *
//                               When a product version password is        *
//                               entered, maxlicensed is not written to    *
//                               the key for every spddefault module.      *
//    DCM   18.jan.98   3.20     A day before first release of SPDNT, took *
//                               out all that ugly subtract 0x3000 from    *
//                               SPDNT version stuff. The first version of *
//                               SPDNT is 0x4000, and it shares the same   *
//                               product ID as SPD for DOS. Simple.        *
//    DCM   22.jun.98            Fixed a potential infinite loop created   *
//                               when Bert changed MAX_EXTENSION_DAYS to   *
//                               255.                                      *
//    DCM   20.jul.98            Fixed a sscanf() related bug that would   *
//                               occur if a user entered a password with   *
//                               a '-' in it.                              *
//    WAJ   28.jan.99				Changed Initial and Extension Trial       *
//                               functionality.  Counters are now          *
//                               initialized on startup and then           *
//                               decremented based on time running.        *
//                                                                         *
//-------------------------------------------------------------------------*
#include <stdio.h>
#include <string.h>
#include <stdlib.h>     // rand()
#include <time.h>
#include <classes.hpp>

//
// Windows NT or 32-Bit Windows 95 Application
//

#if defined(__WIN32__)

#include <windows.h>
//#include "3prty\rainbow\winnt\spromeps.h"

//
// Windows 3.1 or 16-Bit Windows 95 Application
//

#else

#include <windows.h>

#endif // Windows 3.1 or 16-Bit Windows 95 Application

#include "keyprod.h"
#include "pkey.h"
#include "spdkey.h"
#include "SSKey.h"
#include "ssckey.hpp"

// protection key passwords and developer ID
const ushort WRITE_PASSWORD = 0x4AFC;
const ushort ACTIVATE_PASSWORD1 = 0xFED1;
const ushort ACTIVATE_PASSWORD2 = 0x1A00;
const ushort DEVELOPER_ID = 0xFEA8;

// miscellaneous constants
#define DAYS_PER_YEAR            365
#define HOURS_PER_DAY            24
#define SECONDS_PER_HOUR         3600
#define SECONDS_PER_DAY          86400L
#define TWO_HRS                  7200L

//IDs that are used for the license server
const long ProductID = 10021;
const long ProductVersionID = 10022;
const long LicenseStatusID = 10025;


/* ProductText[]
 *    ProductText[] is an array of Solimar product names indexed by the
 *    ProductId enumerated type. Max length of strings is 9 for Creator
 *    dialog boxes.
 *-------------------------------------------------------------------------*/
const char* ProductText[] =
{
   "SP/D",                                                      // SPD_PRODUCT = 0
   "XImage",                                                    // XIMAGE_PRODUCT = 1
   "XImage NT",                                                 // XIMAGENT_PRODUCT = 2
   "SolPcl",                                                    // SOLPCLNT_PRODUCT = 3
   "???",             //SP/D NT                                 // Not Used,
   "SOLsearcher",                                               // INDEX_PLUGIN = 5
   "Connectivity",                                              // CONNECT_PRODUCT = 6
   "iCONVERT",                                                  // ICONVERT_PRODUCT = 7
   "SOLsearcher Ent.",                                          // SOLSEARCHER_ENTERPRISE_PRODUCT = 8
   "SOLscript",                                                 // SOLSCRIPT_PRODUCT = 9
   "SDX Designer",                                              // SDX_DESIGNER_PRODUCT = 10
   "Quantum Server",															 // QUANTUM_PRODUCT = 11
// "123456789" <-- Maximum product name length is 9
};

/* productText[]
 *    longProductText[] is an array of product labels.  Array should
 *    not be accessed directly but through helper function getProductText()
---------------------------------------------------------------------------*/
const char* longProductText[] =
{

   "Solimar Print/Director, Platinum Edition ",                 // SPD_PRODUCT = 0
   "Solimar XImage 3.1/95 ",                                    // XIMAGE_PRODUCT = 1
   "Solimar XImage NT ",                                        // XIMAGENT_PRODUCT = 2
   "Solimar PCL Driver ",                                       // SOLPCLNT_PRODUCT = 3
   "???",                                                       // Not Used,
   "SOLsearcher ",                                              // INDEX_PLUGIN = 5
   "Solimar Print/Director, Connectivity Server Edition ",      // CONNECT_PRODUCT = 6
   "Solimar Print/Director, iCONVERT Edition ",                 // ICONVERT_PRODUCT = 7
   "SOLsearcher Enterprise ",                                   // SOLSEARCHER_ENTERPRISE_PRODUCT = 8
   "SOLscript ",                                                // SOLSCRIPT_PRODUCT = 9
   "SDX Designer ",                                             // SDX_DESIGNER_PRODUCT = 10
	"Solimar Print/Director, Quantum Server Edition ",           // QUANTUM_PRODUCT = 11
};

/* productText[]
 *    mediumProductText[] is an array of product labels.  Array should
 *    not be accessed directly but through helper function getProductText()
---------------------------------------------------------------------------*/
const char* mediumProductText[] =
{
   "SP/D, Platinum Edition ",                // SPD_PRODUCT = 0
   "XImage 3.1/95 ",                         // XIMAGE_PRODUCT = 1
   "XImage NT ",                             // XIMAGENT_PRODUCT = 2
   "PCL Driver ",                            // SOLPCLNT_PRODUCT = 3
   "???",                                    // Not Used,
   "SOLsearcher ",                           // INDEX_PLUGIN = 5
   "SP/D, Connectivity Server Edition ",     // CONNECT_PRODUCT = 6
   "SP/D, iCONVERT Edition",                 // ICONVERT_PRODUCT = 7
   "SOLsearcher Enterprise ",                // SOLSEARCHER_ENTERPRISE_PRODUCT = 8
   "SOLscript ",                             // SOLSCRIPT_PRODUCT = 9
   "SDX Designer ",                          // SDX_DESIGNER_PRODUCT = 10
   "SP/D, Quantum Server",							// QUANTUM_PRODUCT = 11
};

/* KDPasswordText[]
 *    This is the text for Test/Dev key types which can be any key.
---------------------------------------------------------------------------*/
const char* KDPasswordText[] =
{                        			// Extension Unit Value
   "30 days/85 hrs - TL",        // 1 Elements #1-#12 are used to deliver prorated Test Licenses (30 days/85 hour increments)
   "60 days/170 hrs - TL",       // 2
   "90 days/255 hrs - TL",       // 3
   "120 days/340 hrs - TL",      // 4
   "150 days/425 hrs - TL",      // 5
   "180 days/510 hrs - TL",      // 6
   "210 days/595 hrs - TL",      // 7
   "240 days/680 hrs - TL",      // 8
   "270 days/765 hrs - TL",      // 9
   "300 days/850 hrs - TL",      // 10
   "330 days/935 hrs - TL",      // 11
   "365 days/1000 hrs - TL",     // 12
   "365 days/9000 hrs - DL",     // 13 Element #13 is the Development License
   "365 days/250 hrs - PDSL",    // 14 Elements #14-#17 are Partner Demo and Support Licenses
   "365 days/375 hrs - PDSL",    // 15
   "365 days/500 hrs - PDSL",    // 16
   "365 days/750 hrs - PDSL",    // 17
   "730 days/18000 hrs - MYDL",  // 18 Elements #18-#21 are multi-year prepaid Development Licenses
   "1095 days/27000 hrs - MYDL", // 19
   "1460 days/36000 hrs - MYDL", // 20
   "1825 days/45000 hrs - MYDL", // 21
   "730 days/2000 hrs - MYTL",   // 22 Elements #22-#25 are multi-year prepaid Test License
   "1095 days/3000 hrs - MYTL",  // 23
   "1460 days/4000 hrs - MYTL",  // 24
   "1825 days/5000 hrs - MYTL",  // 25

};
/* KDPasswordDays[]
 *    This is the text for Test/Dev key types which can be any key.
---------------------------------------------------------------------------*/
const unsigned short KDPasswordDays[] =
{        // Extension Unit Value
   30,  	// 1 Elements #1-#12 are used to deliver prorated Test Licenses (30 days/85 hour increments)
   60,  	// 2
   90, 	// 3
   120, 	// 4
   150, 	// 5
   180, 	// 6
   210, 	// 7
   240, 	// 8
   270, 	// 9
   300, 	// 10
   330, 	// 11
   365, 	// 12
   365, 	// 13 Element #13 is the Development License
   365, 	// 14 Elements #14-#17 are Partner Demo and Support Licenses
   365, 	// 15
   365, 	// 16
   365, 	// 17
   730, 	// 18 Elements #18-#21 are multi-year prepaid Development Licenses
   1095, // 19
   1460, // 20
   1825, // 21
   730,  // 22 Elements #22-#25 are multi-year prepaid Test License
   1095, // 23
   1460, // 24
   1825, // 25
};

/* KDPasswordHours[]
 *    This is the text for Test/Dev key types which can be any key.
---------------------------------------------------------------------------*/
const unsigned short KDPasswordHours[] =
{           // Extension Unit Value
   85,      // 1 Elements #1-#12 are used to deliver prorated Test Licenses (30 days/85 hour increments)
   170,     // 2
   255,     // 3
   340,     // 4
   425,     // 5
   510,     // 6
   595,     // 7
   680,     // 8
   765,     // 9
   850,     // 10
   935,     // 11
   1000,    // 12
   9000,    // 13 Element #13 is the Development License
   250,    	// 14 Elements #14-#17 are Partner Demo and Support Licenses
   375,    	// 15
   500,    	// 16
   750,    	// 17
   18000,  	// 18 Elements #18-#21 are multi-year prepaid Development Licenses
   27000, 	// 19
   36000, 	// 20
   45000, 	// 21
   2000,  	// 22 Elements #22-#25 are multi-year prepaid Test License
   3000,  	// 23
   4000,   	// 24
   5000, 	// 25
};



/*-------------------------------------------------------------------------*
 *                                                                         *
 *    ProtectionKey Class:                                                 *
 *                                                                         *
 *    ProtectionKey contains all pertinent information for one protection  *
 *    key:  the handle needed for calls to the Rainbow API, and all data   *
 *    for the key.  ProtectionKey also contains numerous functions for     *
 *    interfacing with the protection key and accessing the key data.      *
 *                                                                         *
 *-------------------------------------------------------------------------*/

/* constructor
---------------------------------------------------------------------------*/
ProtectionKey::ProtectionKey() :
   warningDay(START_OF_PROGRAM),
   initialCounter(keyDataBlock.data[INITIAL_COUNTER_CELL]),
   extendedCounter(keyDataBlock.data[EXTENDED_COUNTER_CELL]),
   primaryDescriptor(*(ulong*)(&keyDataBlock.data[PRIMARY_ALG_CELL])),
   primaryPassword(*(ulong*)(&keyDataBlock.data[PRIMARY_PASSWD_CELL])),
   expirationDate(*(ulong*)(&keyDataBlock.data[EXPIRATION_DATE_CELL])),
   secondaryDescriptor(*(ulong*)(&keyDataBlock.data[SECONDARY_ALG_CELL])),
   productId(keyDataBlock.data[PRODUCT_ID_CELL]),
   productVersion(keyDataBlock.data[PRODUCT_VERSION_CELL]),
   keyVersion(keyDataBlock.data[KEY_VERSION_CELL]),
   uses(keyDataBlock.data[KEY_USES_CELL]),
   status(keyDataBlock.data[KEY_STATUS_CELL]),
   customerNumber(keyDataBlock.data[CUSTOMER_NUMBER_CELL]),
   keyNumber(keyDataBlock.data[KEY_NUMBER_CELL]),
   keyType(keyDataBlock.data[KEY_TYPE_CELL])
{

}



/* copy constructor
---------------------------------------------------------------------------*/
ProtectionKey::ProtectionKey(const ProtectionKey& pkey) :
   warningDay(pkey.warningDay),
   initialCounter(keyDataBlock.data[INITIAL_COUNTER_CELL]),
   extendedCounter(keyDataBlock.data[EXTENDED_COUNTER_CELL]),
   primaryDescriptor(*(ulong*)(&keyDataBlock.data[PRIMARY_ALG_CELL])),
   primaryPassword(*(ulong*)(&keyDataBlock.data[PRIMARY_PASSWD_CELL])),
   expirationDate(*(ulong*)(&keyDataBlock.data[EXPIRATION_DATE_CELL])),
   secondaryDescriptor(*(ulong*)(&keyDataBlock.data[SECONDARY_ALG_CELL])),
   productId(keyDataBlock.data[PRODUCT_ID_CELL]),
   productVersion(keyDataBlock.data[PRODUCT_VERSION_CELL]),
   keyVersion(keyDataBlock.data[KEY_VERSION_CELL]),
   uses(keyDataBlock.data[KEY_USES_CELL]),
   status(keyDataBlock.data[KEY_STATUS_CELL]),
   customerNumber(keyDataBlock.data[CUSTOMER_NUMBER_CELL]),
   keyNumber(keyDataBlock.data[KEY_NUMBER_CELL]),
   keyType(keyDataBlock.data[KEY_TYPE_CELL])
{
	keyDataBlock = pkey.keyDataBlock;
}


/* destructor
---------------------------------------------------------------------------*/
ProtectionKey::~ProtectionKey()
{
}



/* badProductVersion()
 *    Check product version licensing.  If a key is not licensed for the
 *    current product version, display a message box to that effect and
 *    return true.  Otherwise, return false.
---------------------------------------------------------------------------*/
bool ProtectionKey::badProductVersion(ushort active_product_version)
{
   if (productVersion < active_product_version)
   {
      sprintf(
         scratchBuf,
         "Key #%03hX-%02hX is not licensed for product version %hx.%03hx. "
         "The key will be inactive until licensing is modified.",
         customerNumber,
         keyNumber,
         ushort(active_product_version>>12),
         ushort(active_product_version&0xFFF)
      );
      keyMessageBox(scratchBuf, pkWarning|pkOKButton);
      return true;
   }
   return false;
}



/* getProductVersion()
 *    Return the product version.
---------------------------------------------------------------------------*/
ushort ProtectionKey::getProductVersion() const
{
   return productVersion;
}


/* clearKeyData()
 *    Initialize all cells of the protection key to 0.  Requires that
 *    overwrite passwords be passed as parameters.  Returns SUCCESS or
 *    FAILURE.
---------------------------------------------------------------------------*/
HRESULT ProtectionKey::clearKeyData(BSTR* TheKeyID, ISolimarLicenseSvr* pServer)
{
   if(pServer)
      return (pServer->KeyFormat(*TheKeyID, TheKeyID));
   else
      return E_FAIL;
}




/* keyMessageBox()
 *    Display a platform-independent message box with the specified
 *    message and buttons. Added ability to suppress messagebox.
---------------------------------------------------------------------------*/
ushort ProtectionKey::keyMessageBox(
   const char* msg, ushort options, bool modal, bool display_message)
{
   if (!display_message)
      return 0;

   return ::keyMessageBox(msg, getProductText(), options, modal);
}



/* inactiveWarningBox()
 *    Put up a warning box to give a warning that a key is about to become
 *    inactive.
---------------------------------------------------------------------------*/
void ProtectionKey::inactiveWarningBox()
{
   warningDay = 0;
   getWarningText(scratchBuf, true);
   keyMessageBox(scratchBuf, pkWarning|pkOKButton);
}



/* getBasePassword()
 *    Calculate and return the password to change to key's status to base
 *    by querying the secondary algorithm of the protection key.  If
 *    successful, returns the base password, otherwise returns 0.
---------------------------------------------------------------------------*/
void ProtectionKey::getBasePassword(ISolimarLicenseSvr* pServer, char* Password_String)
{
   if(pServer)
   {
      BSTR password;
      if(SUCCEEDED(pServer->GenerateBasePassword(customerNumber, keyNumber, &password)))
      {
         AnsiString* pwd = new AnsiString(password);
         strcpy(Password_String, pwd->c_str());
         delete pwd;
      }
      SysFreeString(password);
   }
}

/* ProgramsKey
 *    Programs the key
---------------------------------------------------------------------------*/
ulong ProtectionKey::ProgramKey(ISolimarLicenseSvr* pServer,
                                BSTR* pTheKeyID,
                                int num_days,
                                VARIANT* pModSafeArray
                                )
{
   if(!pServer)
      return 1;

   long ProductVersion = keyDataBlock.data[PRODUCT_VERSION_CELL];

	//extract the first byte
	//12 = 3 Bytes * sizeof(BYTE) = 3*4
	int MajorVersion = ((int)ProductVersion) >> 12;

	//extract the last three bytes
	int MinorVersion = (int)((((ProductVersion & 0x0F00) >> 8) * 10) + (((ProductVersion & 0x00F0) >> 4) * 1));

   HRESULT hr = 0;

   hr = pServer->KeyProgram(*pTheKeyID,         //the current key ID
                            keyDataBlock.data[CUSTOMER_NUMBER_CELL],
                            keyDataBlock.data[KEY_NUMBER_CELL],
                            keyDataBlock.data[PRODUCT_ID_CELL],
                            MajorVersion,
                            MinorVersion,
                            keyDataBlock.data[KEY_TYPE_CELL],
                            num_days,
                            *pModSafeArray,
                            pTheKeyID       //the key id after being programmed
                           );

   if(SUCCEEDED(hr))
      return 0;

   return 1;
}


/* getCounter() - Return value of appropriate counter.
---------------------------------------------------------------------------*/
short ProtectionKey::getCounter() const
{
   short result = 0;
   switch(status) {
      case UNINITIALIZED_TRIAL:
      case INITIAL_TRIAL:
         result = initialCounter;
         break;
      case EXTENDED_TRIAL:
      case EXTENDED_TRIAL2:
      case EXTENDED_TRIAL3:
      case EXTENDED_TRIAL4:
      case EXTENDED_TRIAL5:
         result = extendedCounter;
   }
   return result;
}



/* getCounterDays()
 *    Determine the number of days left in the temporary license based on
 *    the appropriate counter.
---------------------------------------------------------------------------*/
short ProtectionKey::getCounterDays() const
{
   short result = 0;
   switch(status) {
      case UNINITIALIZED_TRIAL:
      case INITIAL_TRIAL:
      	result = (keyType==KEYDevelopment) ? short(((short)initialCounter)/HOURS_PER_DAY) : short(((short)initialCounter)/HOURS_PER_DAY - EXTRA_DAY_BUFFER);
         break;
      case EXTENDED_TRIAL:
      case EXTENDED_TRIAL2:
      case EXTENDED_TRIAL3:
      case EXTENDED_TRIAL4:
      case EXTENDED_TRIAL5:
         result = short(extendedCounter/HOURS_PER_DAY);
   }
   return result;
}



/* getExpirationDays()
 *    Determine the number of days left in the temporary license based on
 *    the expirationDate member.
---------------------------------------------------------------------------*/
short ProtectionKey::getExpirationDays() const
{
   short trial_days =
      (short)(((long)expirationDate - time(NULL)) / (long)SECONDS_PER_DAY);
   if (status == INITIAL_TRIAL || status == UNINITIALIZED_TRIAL)
      trial_days -= EXTRA_DAY_BUFFER;
   if (trial_days > 365)   trial_days = 365;
   if (trial_days < -100)  trial_days = -100;
   return trial_days;
}



/* getExtensionPassword()
 *    Calculate and return the password for the specified number of
 *    extension_days by querying the secondary algorithm of the protection
 *    key.  If successful, returns the password for this extension,
 *    otherwise returns 0.
 *
---------------------------------------------------------------------------*/
void ProtectionKey::getExtensionPassword(uchar extension_days,
                                          ushort extension_num,
                                          ISolimarLicenseSvr* pServer,
                                          char* password
                                          )
{
   if(pServer)
   {
      BSTR TempPassword;
      if(SUCCEEDED(pServer->GenerateExtensionPassword(customerNumber,
                                                      keyNumber,
                                                      (long)extension_days,
                                                      (long)extension_num,
                                                      &TempPassword
                                                     )))
      {
         AnsiString* pwd = new AnsiString(TempPassword);
         strcpy(password, pwd->c_str());
         delete pwd;
      }
      SysFreeString(TempPassword);
   }
}



/* getKeyStatusText()
 *    Return a text string for this key's status.
---------------------------------------------------------------------------*/
const char* ProtectionKey::getKeyStatusText() const
{
   return (status <= DEACTIVATED) ? keyStatusText[status] : "?";
}



/* getProductText()
 *    Return a text string for this key's product ID.
---------------------------------------------------------------------------*/
const char* ProtectionKey::getProductText() const
{
   return (productId <= NUMBER_OF_PRODUCTS) ? ProductText[productId] : "?";
}



/* getKeyStatusText()
 *    Return a text string for the specified key_status.
 *    This is a STATIC function.
---------------------------------------------------------------------------*/
const char* ProtectionKey::getKeyStatusText(KeyStatus key_status)
{
   return (key_status <= DEACTIVATED) ? keyStatusText[key_status] : "?";
}



/* getProductText()
 *    Return a text string for the specified product_id.
 *    This is a STATIC function.
---------------------------------------------------------------------------*/
const char* ProtectionKey::getProductText(ProductId id, ProductLength length)
{
   //return (product_id < NUMBER_OF_PRODUCTS) ? ProductText[product_id] : "?";
   if( id > (sizeof(longProductText)/sizeof(char*)) )
   	return "???";
   else
   {
      if(length == LONG_PRODUCT_NAME)
   		return longProductText[id];
      else if(length == MEDIUM_PRODUCT_NAME)
      	return mediumProductText[id];
      else
      	return ProductText[id];
   }
}

//==============================================================================
// Function:    getKDPasswordText
// Purpose:     Returns the formatted password text for the development keys.
// Parameters:  None
// Returns:     const char*
//==============================================================================
const char* ProtectionKey::getKDPasswordText(unsigned short index) const
{
    return KDPasswordText[index];
}

//==============================================================================
// Function:    getKDPasswordDays
// Purpose:     Returns the formatted password days for the development keys.
// Parameters:  None
// Returns:     const char*
//==============================================================================
const unsigned short ProtectionKey::getKDPasswordDays(unsigned short index) const
{
    return KDPasswordDays[index];
}

//==============================================================================
// Function:    getKDPasswordHours
// Purpose:     Returns the formatted password hours for the development keys.
// Parameters:  None
// Returns:     const char*
//==============================================================================
const unsigned short ProtectionKey::getKDPasswordHours(unsigned short index) const
{
    return KDPasswordHours[index];
}


/* getProductVersionPassword()
 *    Calculate and return the password for the specified product version
 *    by querying the secondary algorithm of the protection key.  If
 *    successful, returns the password for this version, otherwise
 *    returns 0.
 *
---------------------------------------------------------------------------*/
void ProtectionKey::getProductVersionPassword(ushort product_version,
                                              ISolimarLicenseSvr* pServer,
                                              char* Password_String
                                             )
{
   if(pServer)
   {
      BSTR password;

      //extract the first byte
		//12 = 3 Bytes * sizeof(BYTE) = 3*4
		int MajorVersion = product_version >> 12;

		//extract the last three bytes
//		int MinorVersion = product_version & 0x0fff;
      int MinorVersion = (int)((((product_version & 0x0F00) >> 8) * 10) + (((product_version & 0x00F0) >> 4) * 1));

      if(SUCCEEDED(pServer->GenerateVersionPassword(customerNumber,
                                                    keyNumber,
                                                    MajorVersion,
                                                    MinorVersion,
                                                    &password
                                                   )))
      {
         //convert the password from BSTR to and int
         AnsiString* pwd = new AnsiString(password);
         strcpy(Password_String, pwd->c_str());
         delete pwd;

         SysFreeString(password);
      }
   }
}



/* getWarningText() - Gets text to display.
---------------------------------------------------------------------------*/
void ProtectionKey::getWarningText(char *dest, bool inactive_key)
{
   short trial_days = min(getExpirationDays(), getCounterDays());
   if (trial_days > 0)   // haven't reached end of trial
      sprintf(dest,
         "Your temporary license for key #%03hX-%02hX has %hd day(s) remaining. ",
         customerNumber,
         keyNumber,
         trial_days
      );
   else if (!trial_days) // today is end of trial
      sprintf(dest,
         "Your temporary license for key #%03hX-%02hX expires today. ",
         customerNumber,
         keyNumber
      );
   else                  // past end of trial
   {
      sprintf(dest,
         "Your temporary license for key #%03hX-%02hX has expired. ",
         customerNumber,
         keyNumber
      );
      if (trial_days > -60)
	      sprintf(&dest[strlen(dest)],
	         "You have been in violation for %hd day(s). ", -trial_days);
   }

   if (inactive_key)
      strcat(dest, "The key will be inactive until licensing is modified.");
   else if (((status == INITIAL_TRIAL) ?
	   (trial_days+EXTRA_DAY_BUFFER) : trial_days) <= 1)
      strcat(dest, "The key will soon be inactive until licensing is modified.");
}



/* newKey()
 *    Create a key object based on the product_id.  This is a STATIC
 *    function.
---------------------------------------------------------------------------*/
ProtectionKey* ProtectionKey::newKey(ProductId product_id)
{
   ProtectionKey* new_key;
   switch (product_id)
   {
      case SPD_PRODUCT:
      case CONNECT_PRODUCT:
		case ICONVERT_PRODUCT:
      case QUANTUM_PRODUCT:
         new_key = new SpdProtectionKey();
         break;
      case XIMAGE_PRODUCT:
      case XIMAGENT_PRODUCT:
      case SOLPCLNT_PRODUCT:
      case INDEX_PLUGIN:
      case SDX_DESIGNER_PRODUCT:
      default:
         new_key = new ProtectionKey();
         break;
      case SOLSEARCHER_ENTERPRISE_PRODUCT:
         new_key = new SSProtectionKey();
         break;
#ifndef __NO_SOLSCRIPT_KEY__
      case SOLSCRIPT_PRODUCT:
         new_key = new SolScriptProtectionKey();
         break;
#endif
   }
   return new_key;
}

/* newKey()
 *    Create a key object based on the product_id which is figured out by the KeyID.  This is a STATIC
 *    function.
---------------------------------------------------------------------------*/
ProtectionKey* ProtectionKey::newKey(BSTR* KeyID, ISolimarLicenseSvr* pServer)
{
//NEED TO TEST RH
	if(!pServer)
		return NULL;

	HRESULT hr = 0;
	ProtectionKey* new_key;
	VARIANT TheProductID;

	//using the solimar license server, query the key header for the specified info
	hr = pServer->KeyHeaderQuery(*KeyID,
                                ProductID,
                                &TheProductID
       							    );
   if(!SUCCEEDED(hr))
      return NULL;

	ProductId product_id = (ProductId)TheProductID.iVal;
	VariantClear(&TheProductID);

	switch (product_id)
	{
      case SPD_PRODUCT:
      case CONNECT_PRODUCT:
		case ICONVERT_PRODUCT:
      case QUANTUM_PRODUCT:
         new_key = new SpdProtectionKey();
         break;
      case XIMAGE_PRODUCT:
      case XIMAGENT_PRODUCT:
      case SOLPCLNT_PRODUCT:
      case INDEX_PLUGIN:
      case SDX_DESIGNER_PRODUCT:
      default:
         new_key = new ProtectionKey();
         break;
      case SOLSEARCHER_ENTERPRISE_PRODUCT:
         new_key = new SSProtectionKey();
         break;
#ifndef __NO_SOLSCRIPT_KEY__
      case SOLSCRIPT_PRODUCT:
         new_key = new SolScriptProtectionKey();
         break;
#endif
   }
   return new_key;
}

/* newKey()
 *    Duplicate the key object defined by parameter pkey.  This is a
 *    STATIC function.
---------------------------------------------------------------------------*/
ProtectionKey* ProtectionKey::newKey(const ProtectionKey* pkey)
{
   if (!pkey)
      return NULL;

   ProtectionKey* new_key;
   switch (pkey->productId)
   {
      case SPD_PRODUCT:
      case CONNECT_PRODUCT:
      case ICONVERT_PRODUCT:
      case QUANTUM_PRODUCT:
         new_key = new SpdProtectionKey(*(SpdProtectionKey*)pkey);
         break;
      case XIMAGE_PRODUCT:
      case XIMAGENT_PRODUCT:
      case SOLPCLNT_PRODUCT:
      case INDEX_PLUGIN:
      case SDX_DESIGNER_PRODUCT:
      default:
         new_key = new ProtectionKey(*pkey);
         break;
      case SOLSEARCHER_ENTERPRISE_PRODUCT:
         new_key = new SSProtectionKey(*(SSProtectionKey*)pkey);
         break;
#ifndef __NO_SOLSCRIPT_KEY__
      case SOLSCRIPT_PRODUCT:
         new_key = new SolScriptProtectionKey(*(SolScriptProtectionKey*)pkey);
         break;
#endif
   }
   return new_key;
}

/* readKeyData()
 *    Read the non-algorithm protection key data members from the
 *    protection key.  Returns SUCCESS or FAILURE.
---------------------------------------------------------------------------*/
const short OFFSET_INDEX = 2;
const short NUM_CELLS_INDEX = 1;
short ProtectionKey::readKeyData(ISolimarLicenseSvr* pServer, BSTR* KeyID)
{
   if(!pServer)
      return FAILURE;

   VARIANT* pKeyData = new VARIANT();
   BYTE* pCellValue;
   DWORD FirstDataCell;
   DWORD NumberOfCells;

   //reads the raw data from the key and returns a safe array of bytes
   if(!SUCCEEDED(pServer->KeyReadRaw(*KeyID, pKeyData)))
      return FAILURE;

   // SafeArrayAccessData and SafeArrayUnaccessData are win32 functions
	//needed to access safe array data
	if(SUCCEEDED(SafeArrayAccessData(pKeyData->parray, (void**)&pCellValue)))
   {
      FirstDataCell = ((DWORD*)pCellValue)[OFFSET_INDEX];
      NumberOfCells = ((DWORD*)pCellValue)[NUM_CELLS_INDEX];

      //i=8 because we need to skip over the first 8 cells after the header
      for(DWORD i = 8; i < NumberOfCells; i++)
         keyDataBlock.data[i-8] = ((ushort*)(pCellValue+FirstDataCell))[i];
   }
   else
      return FAILURE;

   if (SUCCEEDED(SafeArrayUnaccessData(pKeyData->parray)))
         return SUCCESS;

   return FAILURE;
}




/* setCounterDays() - Set the appropriate counter to trial_days.
---------------------------------------------------------------------------*/
void ProtectionKey::setCounterDays(ushort trial_days)
{
   switch(status) {
      case UNINITIALIZED_TRIAL:
      case INITIAL_TRIAL:
         initialCounter =
            ushort((trial_days+EXTRA_DAY_BUFFER)*(ushort)HOURS_PER_DAY);
         break;
      case EXTENDED_TRIAL:
      case EXTENDED_TRIAL2:
      case EXTENDED_TRIAL3:
      case EXTENDED_TRIAL4:
      case EXTENDED_TRIAL5:
         extendedCounter = ushort(trial_days*(ushort)HOURS_PER_DAY);
   }
}

/* setCounterDays() - Set the appropriate counter to trial_days.
---------------------------------------------------------------------------*/
void ProtectionKey::setCounterDaysByHours(ushort trial_hours)
{
   switch(status) {
      case UNINITIALIZED_TRIAL:
      case INITIAL_TRIAL:
         initialCounter = trial_hours;
         break;
      case EXTENDED_TRIAL:
      case EXTENDED_TRIAL2:
      case EXTENDED_TRIAL3:
      case EXTENDED_TRIAL4:
      case EXTENDED_TRIAL5:
         extendedCounter = trial_hours;
   }
}



/* setExpirationDays()
 *    Set the expiration date to trial_days + the current day.
---------------------------------------------------------------------------*/
void ProtectionKey::setExpirationDays(ushort trial_days)
{
   if ((status == INITIAL_TRIAL || status == UNINITIALIZED_TRIAL) && keyType!=KEYDevelopment)
      trial_days += EXTRA_DAY_BUFFER;
   expirationDate = time(NULL) + (long)trial_days * (long)SECONDS_PER_DAY;
}

/* warningBox()
 *    Put up a warning box to give a warning that a trial period is about
 *    to run out.
---------------------------------------------------------------------------*/
void ProtectionKey::warningBox()
{
   short trial_days = min(getExpirationDays(), getCounterDays());

   if (trial_days == warningDay) // already warned user today
      return;

   if ((warningDay == START_OF_PROGRAM) || // always warn at start of program
       (trial_days == 7) ||
       (trial_days == 5) ||
       (trial_days <= 3))
   {
      getWarningText(scratchBuf);
      if (trial_days > 0)
         keyMessageBox(scratchBuf, pkReminder|pkOKButton, false);
      else
         keyMessageBox(scratchBuf, pkWarning|pkOKButton, false);
      warningDay = trial_days;
   }
}

/* writeKeyData()
 *    Write the protection key data members to the protection key.
 *    This function is used exclusively by the Creator to program a
 *    protection key.  Returns SUCCESS or FAILURE.
---------------------------------------------------------------------------*/
short ProtectionKey::writeKeyData()
{
   return SUCCESS;
}

/* writeProductVersion()
 *    Set the productVersion for both the protection key and the productVersion
 *    data member.  Returns SUCCESS or FAILURE.
---------------------------------------------------------------------------*/
short ProtectionKey::writeProductVersion(ushort product_version)
{
   return 0;
}



/* writeStatus()
 *    Change the status of the key to key_status for both the protection
 *    key and the local data member.  Returns SUCCESS or FAILURE.
---------------------------------------------------------------------------*/
short ProtectionKey::writeStatus(ushort key_status)
{
   return 0;
}



/* accessCode[]
 *    The accessCode array tells the access code of each cell of the
 *    protection key.
---------------------------------------------------------------------------*/
ProtectionKey::AccessCode ProtectionKey::accessCode[TOTAL_CELLS] =
{
	// FIRST ROW OF CELLS
	READWRITE,     // Protection Key Address:  0x08
	READWRITE,     // 0x09
	READWRITE,     // 0x0A
	READWRITE,     // 0x0B
	READWRITE,     // 0x0C
	READWRITE,     // 0x0D
	READWRITE,     // 0x0E
	READWRITE,     // 0x0F
	// SECOND ROW OF CELLS
	READWRITE,     // 0x10
	READWRITE,     // 0x11
	READWRITE,     // 0x12
	READWRITE,     // 0x13
	READWRITE,     // 0x14
	READWRITE,     // 0x15
	READWRITE,     // 0x16
	READWRITE,     // 0x17
	// THIRD ROW OF CELLS
	READWRITE,     // 0x18
	READWRITE,     // 0x19
	READWRITE,     // 0x1A
	READWRITE,     // 0x1B
	ALGORITHM,     // 0x1C
	ALGORITHM,     // 0x1D
	ALGORITHM,     // 0x1E
	ALGORITHM,     // 0x1F
	// FOURTH ROW OF CELLS
	READWRITE,     // 0x20
	READWRITE,     // 0x21
	READWRITE,     // 0x22
	READWRITE,     // 0x23
	READWRITE,     // 0x24
	READWRITE,     // 0x25
	READWRITE,     // 0x26
	READWRITE,     // 0x27
	// FIFTH ROW OF CELLS
	READWRITE,     // 0x28
	READWRITE,     // 0x29
	ALGORITHM,     // 0x2A
	ALGORITHM,     // 0x2B
	READWRITE,     // 0x2C
	READWRITE,     // 0x2D
	READWRITE,     // 0x2E
	READWRITE,     // 0x2F
	// SIXTH ROW OF CELLS
	READWRITE,     // 0x30
	READWRITE,     // 0x31
	READWRITE,     // 0x32
	READWRITE,     // 0x33
	READWRITE,     // 0x34
	READWRITE,     // 0x35
	READWRITE,     // 0x36
	READWRITE,     // 0x37
	// LAST ROW OF CELLS
	READWRITE,     // 0x38
	READWRITE,     // 0x39
	READWRITE,     // 0x3A
	READWRITE,     // 0x3B
	READWRITE,     // 0x3C
	READWRITE,     // 0x3D
	READONLY,      // 0x3E
	READONLY       // 0x3F
};



/* queries
 *    queries is an instance of the Query class, containing queries
 *    and the expected responses from the protection key for those
 *    queries.
---------------------------------------------------------------------------*/
Query ProtectionKey::queries =
{
   {0x14, 0xD0, 0x15, 0x20, 0x30}, // primary1
   {0xA3, 0xBF, 0xD2, 0xA0, 0xF1}, // primout1
   {0xA1, 0x4C, 0xDB, 0x15, 0x23}, // primary2
   {0x2B, 0xD4, 0x29, 0x8D, 0xA7}, // primout2
   {0xFF, 0xAB, 0xCE, 0xDD, 0x84}, // secondary1
   {0x92, 0x08, 0x36, 0x2F, 0x3D}, // secondout1
   {0xF2, 0xFA, 0xAB, 0xEC, 0x14}, // secondary2
   {0x75, 0x2E, 0xB1, 0xBF, 0x2E}  // secondout2
};



/* keyStatusText[]
 *    keyStatusText gives a text string for each key status.
---------------------------------------------------------------------------*/
const char* ProtectionKey::keyStatusText[] =
{
   "Initial",       // 0 = INITIAL_TRIAL
   "Extended",      // 1 = EXTENDED_TRIAL
   "Permanent",     // 2 = BASE
   "Uninitialized", // 3 = UNINITIALIZED
   "Extended 2",    // 4 = EXTENDED_TRIAL2
   "Extended 3",    // 5 = EXTENDED_TRIAL3
   "Extended 4",    // 6 = EXTENDED_TRIAL4
   "Extended 5",    // 7 = EXTENDED_TRIAL5
   NULL,            // 8
   NULL,            // 9
   "Unused",        // 10 = UNUSED
   "Deactivated"    // 11 = DEACTIVATED
};




