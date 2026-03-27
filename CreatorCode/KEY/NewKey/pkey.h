//-------------------------------------------------------------------------*
//    filename - pkey.h                                                    *
//                                                                         *
//    Class(es):                                                           *
//       Query                                                             *
//       KeyDataBlock                                                      *
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
#ifndef __PKEY_H
#define __PKEY_H

#include <time.h>
// function return values
#define SUCCESS                  1
#define FAILURE                 -1
#define NO_MATCH                 0
#define CLOCK_MISMATCH          -2
#define USER_CANCELED           -3

// key cell specifications
#define TOTAL_CELLS              56

// key cell numbers
#define KEY_START_ADDRESS        0x08
#define INITIAL_COUNTER_CELL     0x12
#define EXTENDED_COUNTER_CELL    0x13
#define PRIMARY_ALG_CELL         0x14
#define PRIMARY_PASSWD_CELL      0x16
#define EXPIRATION_DATE_CELL     0x18
#define SECONDARY_ALG_CELL       0x22
#define KEY_TYPE_CELL				0x30
#define PRODUCT_ID_CELL          0x31
#define PRODUCT_VERSION_CELL     0x32
#define KEY_VERSION_CELL         0x33
#define KEY_USES_CELL            0x34
#define KEY_STATUS_CELL          0x35
#define CUSTOMER_NUMBER_CELL     0x36
#define KEY_NUMBER_CELL          0x37

// trial period constants
const short INITIAL_TRIAL_DAYS = 30;
const short EXTRA_DAY_BUFFER = 10;
const short MAX_EXTENSION_DAYS = 255;  //before 3/17/98 MAX_EXTENSION_DAYS = 90;

// special constants used for warning customer
#define START_OF_PROGRAM         100
#define DEACTIVATION_DAY         101

// version of SP/D coinciding with key library/Creator v3.00
#define KEYv300_SPD_VERSION      0x3310

#ifndef __WIN32__ // not the Win32 rainbow library
// Global API Command Packet defined in Spromeps.c
extern APIPACKET ApiPacket;
#endif


#include "..\SolimarLicensing\SolimarLicenseServer\_SolimarLicenseServer.h"


enum KeyTypeFlagX {
	KEYNone       =0x00000000,  //reserved state
	KEYAddon       =1,
	KEYBase        =2,
	KEYReplacement =3,
	KEYRelicense   =4,
	KEYInventory   =5,
	KEYBackup      =6,
	KEYEmergency   =7,
        //KEYSolimar    =8,
	KEYCustom      =9,
	KEYLoan        =10,
	KEYDemo        =11,
	KEYRental      =12,
        KEYDevelopment =13,
	KEYReserved    =14,
};

enum KeyStatus  {UNINITIALIZED_TRIAL=3, INITIAL_TRIAL=0, EXTENDED_TRIAL=1,
                 EXTENDED_TRIAL2=4, EXTENDED_TRIAL3=5, EXTENDED_TRIAL4=6,
                 EXTENDED_TRIAL5=7, BASE=2, UNUSED=10, DEACTIVATED=11};

/* pkXXXX
 *    The pkXXXX constants are ORed together to produce the options
 *    parameter for keyMessageBox().  The return value for keyMessageBox()
 *    is pkOK, pkYes, or pkNo.
---------------------------------------------------------------------------*/
enum {pkWarning=0, pkError=1, pkInformation=2, pkReminder=3, // titles
      pkYesNoButton=0x0300, pkOKButton=0x0400,               // buttons
      pkOK=10, pkYes=12, pkNo=13};                           // return values


/*-------------------------------------------------------------------------*
 *                                                                         *
 *    Query Class:                                                         *
 *                                                                         *
 *    Query is a class that contains the queries and the expected          *
 *    responses from the protection key for those queries.                 *
 *                                                                         *
 *-------------------------------------------------------------------------*/
#define QUERY_LENGTH 5
class Query
{
public:
   uchar primary1[QUERY_LENGTH];
   uchar primout1[QUERY_LENGTH];
   uchar primary2[QUERY_LENGTH];
   uchar primout2[QUERY_LENGTH];
   uchar secondary1[QUERY_LENGTH];
   uchar secondout1[QUERY_LENGTH];
   uchar secondary2[QUERY_LENGTH];
   uchar secondout2[QUERY_LENGTH];
};



/*-------------------------------------------------------------------------*
 *                                                                         *
 *    KeyDataBlock Class:                                                  *
 *                                                                         *
 *    This class contains a ushort array that mirrors the data on the      *
 *    protection key.                                                      *
 *                                                                         *
 *-------------------------------------------------------------------------*/
class KeyDataBlock
{
public:
   KeyDataBlock() {memset(data, 0, sizeof(data));}
   ushort data[TOTAL_CELLS];
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
class ProtectionKey
{
public:
   ProtectionKey();
   ProtectionKey(const ProtectionKey&);
   virtual ~ProtectionKey();

   // static functions used to create key objects
   static ProtectionKey* newKey(ProductId product_id);
   static ProtectionKey* newKey(BSTR* KeyID, ISolimarLicenseSvr* pServer);
   static ProtectionKey* newKey(const ProtectionKey*); // copy key

   // function to display platform-independent message box
   friend ushort keyMessageBox(
      const char* msg, const char* product_text, ushort options, bool modal);

   // functions that do not perform key I/O
   bool badProductVersion(ushort);
   ushort getProductVersion() const;
   bool isDeactivated() const;
   bool isOnTrial() const;
   bool isProgrammed() const;
   short getCounter() const;
   short getCounterDays() const;
   short getExpirationDays() const;
   void setCounterDays(ushort);
   void setCounterDaysByHours(ushort);
   void setExpirationDays(ushort);
   const char* getKeyStatusText() const;
   const char* getProductText() const;
   ulong ProtectionKey::ProgramKey(ISolimarLicenseSvr*, BSTR*, int, VARIANT*);
   static const char* getKeyStatusText(KeyStatus key_status);
   static const char* getProductText(ProductId product_id, ProductLength product_length=SHORT_PRODUCT_NAME);


   // For the test/development key types.
   const char* getKDPasswordText(unsigned short index) const;
   const unsigned short getKDPasswordDays(unsigned short index) const;
   const unsigned short getKDPasswordHours(unsigned short index) const;

public:

   // simple key I/O functions
   HRESULT clearKeyData(BSTR* TheKeyID, ISolimarLicenseSvr* pServer);

   void getBasePassword(ISolimarLicenseSvr* pServer, char* Password_String);
   enum {MAX_EXTENSION_NUM = 4};

   void getExtensionPassword(uchar extension_days,
                              ushort extension_num,
                              ISolimarLicenseSvr* pServer,
                              char* password);

   void getProductVersionPassword(ushort product_version,
                                   ISolimarLicenseSvr* pServer,
                                   char* Password_String
                                   );
   short readKeyData(ISolimarLicenseSvr* pServer, BSTR* KeyID);

   short writeKeyData();
   short writeStatus(ushort);
   short writeProductVersion(ushort);

   // GUI-related functions
   virtual ushort keyMessageBox(
      const char* msg, ushort options, bool modal=true, bool display_message=true);
   void inactiveWarningBox();
   void getWarningText(char *dest, bool inactive_key=false);
   void warningBox();

   short warningDay;
   KeyDataBlock keyDataBlock;

   // protection key data (references into keyDataBlock)
   ushort& initialCounter;
   ushort& extendedCounter;
   ulong& primaryDescriptor;
   ulong& primaryPassword;
   ulong& expirationDate;
   ulong& secondaryDescriptor;
   ushort& productId;
   ushort& productVersion; // use set/get functions for SPDNT compatibility
   ushort& keyVersion;
   ushort& uses;
   ushort& status;
   ushort& customerNumber;
   ushort& keyNumber;
   ushort& keyType;
   // end protection key data

protected:
   char scratchBuf[512];
   clock_t check_point; //clock cycles count when key was last decremented

   // static data
   enum AccessCode {READWRITE, READONLY, COUNTER, ALGORITHM};
   static AccessCode accessCode[TOTAL_CELLS];
   static Query queries;
   static const char* keyStatusText[];
};



/* isDeactivated()
 *    Return true if this key has been deactivated by the Creator, false
 *    otherwise.
---------------------------------------------------------------------------*/
inline bool ProtectionKey::isDeactivated() const
{
   return bool(status==DEACTIVATED);
}



/* isOnTrial()
 *    Return true if this key is on a trial period, false otherwise.
---------------------------------------------------------------------------*/
inline bool ProtectionKey::isOnTrial() const
{
   return bool (
      status==INITIAL_TRIAL ||
      status==EXTENDED_TRIAL ||
      status==EXTENDED_TRIAL2 ||
      status==EXTENDED_TRIAL3 ||
      status==EXTENDED_TRIAL4 ||
      status==EXTENDED_TRIAL5 ||
      status==UNINITIALIZED_TRIAL
   );
}



/* isProgrammed()
 *    Return true if this key is programmed, false if it has been
 *    deactivated or never before programmed.
---------------------------------------------------------------------------*/
inline bool ProtectionKey::isProgrammed() const
{
   return bool(status < UNUSED && customerNumber);
}



#endif

