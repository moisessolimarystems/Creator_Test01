//-------------------------------------------------------------------------*
//    filename - spdekey.cpp                                                *
//                                                                         *
//    Class(es):                                                           *
//       ModuleCell                                                        *
//       SpdProtectionKey                                                  *
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
#include <stdio.h>
#include <string.h>
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

//#include "3prty\rainbow\windows\superpro.h"

#endif // Windows 3.1 or 16-Bit Windows 95 Application

#include "keyprod.h"
#include "pkey.h"
#include "spdekey.h"

// These symbols were not defined in Builder 6     - Chris
#if defined(__BORLANDC__)
#ifndef min
#define min(x,y) ((x) < (y) ? (x) : (y))
#endif
#ifndef max
#define max(x,y) ((x) < (y) ? (y) : (x))
#endif
#endif


/*-------------------------------------------------------------------------*
 *                                                                         *
 *    ModuleCell Class:                                                    *
 *                                                                         *
 *    This class corresponds to one cell of the protection key.  Using     *
 *    ModuleCell, you can access a ushort by nibble (1 bit).  ModuleCells *
 *    are used primarily to store the module licensing information from    *
 *    the protection key.                                                  *
 *                                                                         *
 *-------------------------------------------------------------------------*/
enum Nibbles {NIBBLE_A, NIBBLE_B, NIBBLE_C, NIBBLE_D, NIBBLE_E, NIBBLE_F,
              NIBBLE_G, NIBBLE_H, NIBBLE_I, NIBBLE_J, NIBBLE_K, NIBBLE_L,
              NIBBLE_M, NIBBLE_N, NIBBLE_O, NIBBLE_P};

enum Nibbler {NIBBLER_A, NIBBLER_B, NIBBLER_C, NIBBLER_D};

class ModuleCell
{
public:
   ModuleCell() : a(0), b(0), c(0), d(0), e(0), f(0), g(0), h(0), i(0),
                  j(0), k(0), l(0), m(0), n(0), o(0), p(0){};
   operator ushort() const {return *((ushort*)this);}

   enum {NIBBLE=1};
   ushort p:NIBBLE; ushort o:NIBBLE; ushort n:NIBBLE; ushort m:NIBBLE;
   ushort l:NIBBLE; ushort k:NIBBLE; ushort j:NIBBLE; ushort i:NIBBLE;
   ushort h:NIBBLE; ushort g:NIBBLE; ushort f:NIBBLE; ushort e:NIBBLE;
   ushort d:NIBBLE; ushort c:NIBBLE; ushort b:NIBBLE; ushort a:NIBBLE;
};

class LargeModuleCell
{
public:
   LargeModuleCell() : a(0), b(0), c(0), d(0) {};
   operator ushort() const {return *((ushort*)this);}

   enum {NIBBLER=4};
   ushort d:NIBBLER; ushort c:NIBBLER; ushort b:NIBBLER; ushort a:NIBBLER;
};

/*-------------------------------------------------------------------------*
 *                                                                         *
 *    SpdeProtectionKey Class:                                             *
 *                                                                         *
 *    Derived from ProtectionKey, this class expands upon ProtectionKey    *
 *    functionality by adding SP/D module licensing support to the         *
 *    existing ProtectionKey interface.                                    *
 *                                                                         *
 *-------------------------------------------------------------------------*/

 /* constructor
---------------------------------------------------------------------------*/
#ifdef __WIN32__ // the Win32 rainbow library
SpdeProtectionKey::SpdeProtectionKey() :
   ProtectionKey(),
   moduleCells((ModuleCell*)&keyDataBlock.data[SPDE_MODULE_START_CELL]),
   outputUnits(keyDataBlock.data[SPDE_OUTPUT_UNIT_CELL]),
   operatorSessionUnits(keyDataBlock.data[SPDE_OPERATOR_SESSION_CELL]),
   userSessionUnits(keyDataBlock.data[SPDE_USER_SESSION_CELL]),
   hardwareModuleCells((LargeModuleCell*)&keyDataBlock.data[SPDE_HARDWARE_MODULE_START_CELL]),
   resourceModuleCells((LargeModuleCell*)&keyDataBlock.data[SPDE_RESOURCE_MODULE_START_CELL])
{
}
#else // not the Win32 rainbow library
SpdeProtectionKey::SpdeProtectionKey() :
   ProtectionKey(),
   moduleCells((ModuleCell*)&keyDataBlock.data[SPDE_MODULE_START_CELL])
   outputUnits(keyDataBlock.data[SPDE_OUTPUT_UNIT_CELL]),
   operatorSessionUnits(keyDataBlock.data[SPDE_OPERATOR_SESSION_CELL]),
   userSessionUnits(keyDataBlock.data[SPDE_USER_SESSION_CELL]),
   hardwareModuleCells((LargeModuleCell*)&keyDataBlock.data[SPDE_HARDWARE_MODULE_START_CELL]),
   resourceModuleCells((LargeModuleCell*)&keyDataBlock.data[SPDE_RESOURCE_MODULE_START_CELL])
{
}
#endif

/* copy constructor
---------------------------------------------------------------------------*/
SpdeProtectionKey::SpdeProtectionKey(const SpdeProtectionKey& pkey) :
   ProtectionKey(pkey),
   moduleCells((ModuleCell*)&keyDataBlock.data[SPDE_MODULE_START_CELL]),
   outputUnits(keyDataBlock.data[SPDE_OUTPUT_UNIT_CELL]),
   operatorSessionUnits(keyDataBlock.data[SPDE_OPERATOR_SESSION_CELL]),
   userSessionUnits(keyDataBlock.data[SPDE_USER_SESSION_CELL]),
   hardwareModuleCells((LargeModuleCell*)&keyDataBlock.data[SPDE_HARDWARE_MODULE_START_CELL]),
   resourceModuleCells((LargeModuleCell*)&keyDataBlock.data[SPDE_RESOURCE_MODULE_START_CELL])
{
}

/* getLicense()
 *    Get and return the license of module mod_id in the moduleCells[] array.
---------------------------------------------------------------------------*/
uchar SpdeProtectionKey::getLicense(ushort mod_id) const
{        //needs to be able to find bits in cells of (1 - bit) and (4 - bit)
   ushort units_licensed =0;
   ushort mcell;
   Nibbles offset;
   if(mod_id < 100)
   {
    mcell = ushort(mod_id>>4); // >>2 same as /4
    offset = (Nibbles)(mod_id&15); // same as %4
    switch(offset) {
      case NIBBLE_A:
         units_licensed = moduleCells[mcell].m;
         break;
      case NIBBLE_B:
         units_licensed = moduleCells[mcell].n;
         break;
      case NIBBLE_C:
         units_licensed = moduleCells[mcell].o;
         break;
      case NIBBLE_D:
         units_licensed = moduleCells[mcell].p;
         break;
      case NIBBLE_E:
         units_licensed = moduleCells[mcell].i;
         break;
      case NIBBLE_F:
         units_licensed = moduleCells[mcell].j;
         break;
      case NIBBLE_G:
         units_licensed = moduleCells[mcell].k;
         break;
      case NIBBLE_H:
         units_licensed = moduleCells[mcell].l;
         break;
      case NIBBLE_I:
         units_licensed = moduleCells[mcell].e;
         break;
      case NIBBLE_J:
         units_licensed = moduleCells[mcell].f;
         break;
      case NIBBLE_K:
         units_licensed = moduleCells[mcell].g;
         break;
       case NIBBLE_L:
         units_licensed = moduleCells[mcell].h;
         break;
       case NIBBLE_M:
         units_licensed = moduleCells[mcell].a;
         break;
       case NIBBLE_N:
         units_licensed = moduleCells[mcell].b;
         break;
      case NIBBLE_O:
         units_licensed = moduleCells[mcell].c;
         break;
      case NIBBLE_P:
         units_licensed = moduleCells[mcell].d;
         break;
     }
    }
    else
    {
        switch(mod_id) {
              case 101:
                   units_licensed = hardwareModuleCells[1].a;
                   break;
              case 102:
                   units_licensed = hardwareModuleCells[1].b;
                   break;
              case 103:
                   units_licensed = hardwareModuleCells[1].c;
                    break;
              case 104:
                   units_licensed = hardwareModuleCells[1].d;
                   break;
              case 105:
                   units_licensed = hardwareModuleCells[0].a ;
                   break;
              case 200:
                   units_licensed = resourceModuleCells[0].b ;
                   break;
              default: break;
       }
   }
   return (uchar)units_licensed;         //unsigned char truncates ushort to only 1byte instead of 2.

}
/* getOutputPassword()
 *
 *
 *
 *
 *
 *    This query will produce a password that is therefore unique
 *    for this customer, with this key, and with this many output units
 *    licensed.  AMAZING!
 -----------------------------------------------------------------------------*/
 const ushort OUTPUT_POOL_MODULE_ID = 240;
 void SpdeProtectionKey::getOutputPassword(ushort output_units,
                                          ISolimarLicenseSvr* pServer,
                                          char* Password_String)
 {
   if(pServer)
   {
      BSTR password;
      if(SUCCEEDED(pServer->GenerateModulePassword(customerNumber,
                                                   keyNumber,
                                                   productId,
                                                   OUTPUT_POOL_MODULE_ID,
                                                   output_units,
                                                   &password
                                                  )))
      {
         AnsiString* pwd = new AnsiString(password);
         strcpy(Password_String, pwd->c_str());
         delete pwd;
      }
      SysFreeString(password);
   }
 }

 /* getOutputPassword()
 *
 *
 *
 *
 *
 *    This query will produce a password that is therefore unique
 *    for this customer, with this key, and with this many output units
 *    licensed.  AMAZING!
 -----------------------------------------------------------------------------*/
 const ushort USER_SESSION_MODULE_ID = 401;
 void SpdeProtectionKey::getUserSessionPassword(ushort user_session_units,
                                          ISolimarLicenseSvr* pServer,
                                          char* Password_String)
 {
   if(pServer)
   {
      BSTR password;
      if(SUCCEEDED(pServer->GenerateModulePassword(customerNumber,
                                                   keyNumber,
                                                   productId,
                                                   USER_SESSION_MODULE_ID,
                                                   user_session_units,
                                                   &password
                                                  )))
      {
         AnsiString* pwd = new AnsiString(password);
         strcpy(Password_String, pwd->c_str());
         delete pwd;
      }
      SysFreeString(password);
   }
 }

 /* getOutputPassword()
 *
 *
 *
 *
 *
 *    This query will produce a password that is therefore unique
 *    for this customer, with this key, and with this many output units
 *    licensed.  AMAZING!
 -----------------------------------------------------------------------------*/
 const ushort OPERATOR_SESSION_MODULE_ID = 400;
 void SpdeProtectionKey::getOperatorSessionPassword(ushort operator_session_units,
                                          ISolimarLicenseSvr* pServer,
                                          char* Password_String)
 {
   if(pServer)
   {
      BSTR password;
      if(SUCCEEDED(pServer->GenerateModulePassword(customerNumber,
                                                   keyNumber,
                                                   productId,
                                                   OPERATOR_SESSION_MODULE_ID,
                                                   operator_session_units,
                                                   &password
                                                  )))
      {
         AnsiString* pwd = new AnsiString(password);
         strcpy(Password_String, pwd->c_str());
         delete pwd;
      }
      SysFreeString(password);
   }
 }

/* getModulePassword()
 *    Calculate and return the password for module mod_id by querying the
 *    secondary algorithm of the protection key.  If successful, returns
 *    the password for module mod_id, otherwise returns 0. Note that this
 *    function uses the units_licensed parameter to generate a password
 *    for the "next" incremental license. For example, if units_licensed
 *    is 2, the password this function returns should license the user for
 *    3 units.
 *
 *    Query = 01234567, where
 *       012 = Customer Number
 *       34  = Key Number
 *       5   = current units_licensed for module #mod_id, 0 for trial key
 *       67  = mod_id
 *
 *    This query will produce a password that is therefore unique
 *    for this customer, with this key, for this module, and with
 *    this many units licensed.
---------------------------------------------------------------------------*/
void SpdeProtectionKey::getModulePassword(uchar mod_id,
                                          uchar units_licensed,
                                          ProductId product_id,
                                          ushort product_version,
                                          ISolimarLicenseSvr* pServer,
                                          char* Password_String
                                          )
{
   if(pServer)
   {
      BSTR password;
      if(SUCCEEDED(pServer->GenerateModulePassword(customerNumber,
                                                   keyNumber,
                                                   product_id,
                                                   mod_id,
                                                   units_licensed,
                                                   &password
                                                  )))
      {
         AnsiString* pwd = new AnsiString(password);
         strcpy(Password_String, pwd->c_str());
         delete pwd;
      }
      SysFreeString(password);
   }
}

//==============================================================================
// Function:    getPagesPerMinutePassword()
// Purpose:     This function will create and return a password for
//
//              This query will produce a password that is therefore unique
//              for this customer, with this key, and with this many pages
//              licensed.  AMAZING!
// Parameters:  ushort - # of extensions
// Returns:     ulong - password
//==============================================================================
void SpdeProtectionKey::getPagesPerMinutePassword(ushort ext,
                                                 ushort pages,
                                                 ISolimarLicenseSvr* pServer,
                                                 char* Password_String,
                                                 long ModID)
{
   if(pServer)
   {
      BSTR password;
      DWORD ppm_struct = (((pages & 0x0000FFFF) << 16) | (ext & 0x0000FFFF));
      if(SUCCEEDED(pServer->GenerateModulePassword(customerNumber,
                                                   keyNumber,
                                                   productId,
                                                   ModID,
                                                   ppm_struct,
                                                   &password
                                                  )))
      {
         AnsiString* pwd = new AnsiString(password);
         strcpy(Password_String, pwd->c_str());
         delete pwd;
      }
      SysFreeString(password);
   }
}

/* setLicense()
 *    Set the license of module mod_id in the moduleCells[] array to
 *    units_licensed.
---------------------------------------------------------------------------*/
void SpdeProtectionKey::setLicense(/*ushort index,*/ ushort mod_id, ushort units_licensed)
{

   ushort mcell;
   int testVal;
   if(mod_id < 100)   //accounts for (1 - bit per module)
   {
     mcell = ushort(mod_id>>4); // >>4 same as /16;
     Nibbles offset= (Nibbles)(mod_id&15); // same as %16;
     switch(offset) {
        case NIBBLE_A:
           moduleCells[mcell].m = units_licensed;
           break;
        case NIBBLE_B:
           moduleCells[mcell].n = units_licensed;
           break;
        case NIBBLE_C:
           moduleCells[mcell].o = units_licensed;
           break;
       case NIBBLE_D:
           moduleCells[mcell].p = units_licensed;
           break;
        case NIBBLE_E:
           moduleCells[mcell].i = units_licensed;
           break;
        case NIBBLE_F:
           moduleCells[mcell].j = units_licensed;
           break;
        case NIBBLE_G:
           moduleCells[mcell].k = units_licensed;
           break;
        case NIBBLE_H:
           moduleCells[mcell].l = units_licensed;
           break;
        case NIBBLE_I:
           moduleCells[mcell].e = units_licensed;
           break;
        case NIBBLE_J:
           moduleCells[mcell].f = units_licensed;
           break;
        case NIBBLE_K:
           moduleCells[mcell].g = units_licensed;
           break;
        case NIBBLE_L:
           moduleCells[mcell].h = units_licensed;
           break;
         case NIBBLE_M:
           moduleCells[mcell].a = units_licensed;
           break;
         case NIBBLE_N:
           moduleCells[mcell].b = units_licensed;
           break;
        case NIBBLE_O:
           moduleCells[mcell].c = units_licensed;
           break;
        case NIBBLE_P:
           moduleCells[mcell].d = units_licensed;
     }
   }
   else       //accounts for 4 - bits per module, hardware modules
   {
        switch(mod_id) {
              case 101:
                   hardwareModuleCells[1].a = units_licensed;
                   break;
              case 102:
                   hardwareModuleCells[1].b = units_licensed;
                   break;
              case 103:
                   hardwareModuleCells[1].c = units_licensed;
                    break;
              case 104:
                   hardwareModuleCells[1].d = units_licensed;
                   break;
              case 105:
                   hardwareModuleCells[0].a = units_licensed;
                   break;
              case 200:
                   resourceModuleCells[0].b = units_licensed;
                   break;
              default: break;
       }
   }
}

/* setLicenses(ushort* buffer)
 *    Should only be used by creator.
---------------------------------------------------------------------------*/
void SpdeProtectionKey::setLicenses(ushort* buffer, bool hardware)
{
       memcpy(moduleCells, buffer, 36);  //36 = size of moduleblob from database, contains list of modulevalues
}

/* setLicensesToZero()
 *    Initializes all licenses in the moduleCells[] array to 0 and sets all
 *    other cells that need to be zeroed out.   This is performed when the
 *    status of the key changes from trial to base.
---------------------------------------------------------------------------*/
void SpdeProtectionKey::setLicensesToZero()
{
   //zero out total output units
   outputUnits = 0x0000;

   //zero out modules
   ushort total_modules = SPDE_TOTAL_KEY_MODULES;
   for (ushort i = 0; i < total_modules; i++)
      setLicense(i, 0);
   for (ushort i = 101; i < (109); i++)   //8 possible hardware modules
        setLicense(i,0);
   for (ushort i = 200; i < (204); i++)  //4 possible resource modules
        setLicense(i,0);
}

// getOutputUnits - returns the number of outputs licensed for the key
ushort SpdeProtectionKey::getOutputUnits()
{
	return outputUnits;
}

// getOutputUnits - returns the number of outputs licensed for the key
ushort SpdeProtectionKey::getOperatorSessionUnits()
{
	return operatorSessionUnits;
}

// getOutputUnits - returns the number of outputs licensed for the key
ushort SpdeProtectionKey::getUserSessionUnits()
{
	return userSessionUnits;
}

