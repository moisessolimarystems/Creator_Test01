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
#include "spdkey.h"
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
   SpdProtectionKey(),
   sessionUnits(keyDataBlock.data[SPDE_SESSION_CELL])
{
}
#else // not the Win32 rainbow library
SpdeProtectionKey::SpdeProtectionKey() :
   SpdProtectionKey(),
   sessionUnits(keyDataBlock.data[SPDE_SESSION_CELL])
{
}
#endif

/* copy constructor
---------------------------------------------------------------------------*/
SpdeProtectionKey::SpdeProtectionKey(const SpdeProtectionKey& pkey) :
   SpdProtectionKey(pkey),
   sessionUnits(keyDataBlock.data[SPDE_SESSION_CELL])
{
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
                                          ISolimarLicenseSvr3* pServer,
                                          ushort password_number,
                                          char* Password_String)
 {
   if(pServer)
   {
      BSTR password;
      if(SUCCEEDED(pServer->GenerateModulePassword2(customerNumber,
                                                   keyNumber,
                                                   productId,
                                                   OUTPUT_POOL_MODULE_ID,
                                                   output_units,
                                                   password_number,
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
                                          ISolimarLicenseSvr3* pServer,
                                          ushort password_number,
                                          char* Password_String)
 {
   if(pServer)
   {
      BSTR password;
      if(SUCCEEDED(pServer->GenerateModulePassword2(customerNumber,
                                                   keyNumber,
                                                   productId,
                                                   USER_SESSION_MODULE_ID,
                                                   user_session_units,
                                                   password_number,
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
                                          ISolimarLicenseSvr3* pServer,
                                          ushort password_number,
                                          char* Password_String)
 {
   if(pServer)
   {
      BSTR password;
      if(SUCCEEDED(pServer->GenerateModulePassword2(customerNumber,
                                                   keyNumber,
                                                   productId,
                                                   OPERATOR_SESSION_MODULE_ID,
                                                   operator_session_units,
                                                   password_number,
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

// getOutputUnits - returns the number of outputs licensed for the key
ushort SpdeProtectionKey::getOperatorSessionUnits()
{
    return sessionUnits & 0x00FF;  //return bottom 8 bits
}

// getOutputUnits - returns the number of outputs licensed for the key
ushort SpdeProtectionKey::getUserSessionUnits()
{
    return sessionUnits >> 8;      //return top 8 bits
}

void SpdeProtectionKey::setOperatorSessionUnits(ushort units)
{   //Session units always less then 256( 0xFF )
    //clear portion of sessionUnits then set
    sessionUnits &= 0xFF00;   //operatorSession units found in bottom 8 bits
    sessionUnits |= units;
}

// getOutputUnits - returns the number of outputs licensed for the key
void SpdeProtectionKey::setUserSessionUnits(ushort units)
{   //Session units always less then 256( 0xFF )
    sessionUnits &= 0x00FF;   //UserSession units are found in top 8 bits
    sessionUnits |= (units << 8);
}

