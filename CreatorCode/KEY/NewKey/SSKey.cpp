//----------------------------------------------------------------------------*
//    filename - SSKey.cpp                                                    *
//                                                                            *
//    Class(es):                                                              *
//       SSProtectionKey                                                      *
//                                                                            *
//----------------------------------------------------------------------------*
//----------------------------------------------------------------------------*
//                                                                            *
//    Solimar SOLsearcher                                                     *
//                                                                            *
//    Copyright 2002-2003 Solimar Systems, Inc. All rights reserved.          *
//                                                                            *
//----------------------------------------------------------------------------*
#include <stdio.h>

#include <windows.h>
#include <classes.hpp>
#include "keyprod.h"
#include "pkey.h"
#include "spdkey.h"
#include "SSKey.h"

/*----------------------------------------------------------------------------*
 *                                                                            *
 *    SSProtectionKey Class:                                                  *
 *                                                                            *
 *    Derived from ProtectionKey, this class provides the interface for       *
 *    accessing a SOLsearcher protection key.                                 *
 *                                                                            *
 *----------------------------------------------------------------------------*/

/* constructor
------------------------------------------------------------------------------*/
SSProtectionKey::SSProtectionKey() :
   SpdProtectionKey(),
   indexServers(keyDataBlock.data[INDEX_SERVERS_CELL]),  //0x00
   reportServers(keyDataBlock.data[REPORT_SERVERS_CELL]),//0x01
   concurrentUsers25(keyDataBlock.data[CONCURRENT_USERS_25_CELL]), //0x02
   applications(keyDataBlock.data[APPLICATIONS_CELL]), //0x03
   documentAssembler(keyDataBlock.data[DOCUMENT_ASSEMBLER_CELL])
{
}



/* copy constructor
------------------------------------------------------------------------------*/
SSProtectionKey::SSProtectionKey(const SSProtectionKey& pkey) :
   SpdProtectionKey(pkey),
   indexServers(keyDataBlock.data[INDEX_SERVERS_CELL]),
   reportServers(keyDataBlock.data[REPORT_SERVERS_CELL]),
   concurrentUsers25(keyDataBlock.data[CONCURRENT_USERS_25_CELL]),
   applications(keyDataBlock.data[APPLICATIONS_CELL]),
   documentAssembler(keyDataBlock.data[DOCUMENT_ASSEMBLER_CELL])
{
}

/* getIndexServers()
 *    Return the number of index servers licensed on the key.
------------------------------------------------------------------------------*/
ushort SSProtectionKey::getIndexServers() const
{
   return indexServers;
}

/* getReportServers()
 *    Return the number of report servers licensed on the key.
------------------------------------------------------------------------------*/
ushort SSProtectionKey::getReportServers() const
{
   return reportServers;
}

/* getConcurrentUsers()
 *    Return the number of concurrent users licensed on the key.
------------------------------------------------------------------------------*/
ushort SSProtectionKey::getConcurrentUsers() const
{
   return concurrentUsers25 * _25_UNITS_LICENSED_PER_CONCURRENT_USERS_25;
}

/* getApplications()
 *    Return the number of applications licensed on the key.
------------------------------------------------------------------------------*/
ushort SSProtectionKey::getApplications() const
{
   return applications;
}

/* getDocumentAssembler()
 *    Return the number of applications licensed on the key.
------------------------------------------------------------------------------*/
ushort SSProtectionKey::getDocumentAssembler() const
{
   return documentAssembler;
}
/* setIndexServers()
 *    Set the number of index servers licensed on the key.
------------------------------------------------------------------------------*/
void SSProtectionKey::setIndexServers(ushort units_licensed)
{
   indexServers = units_licensed;
}

/* setReportServers()
 *    Set the number of report servers licensed on the key.
------------------------------------------------------------------------------*/
void SSProtectionKey::setReportServers(ushort units_licensed)
{
   reportServers = units_licensed;
}

/* setConcurrentUsers()
 *    Set the number of concurrent users licensed on the key. Since the
 *    units_licensed passed to this function is always rounded down to a
 *    multiple of 25, the caller should know to only set units_licensed
 *    to multiples of 25.
------------------------------------------------------------------------------*/
void SSProtectionKey::setConcurrentUsers(ushort units_licensed)
{
   concurrentUsers25 =
      units_licensed / _25_UNITS_LICENSED_PER_CONCURRENT_USERS_25;
}

/* setApplications()
 *    Set the number of applications licensed on the key.
------------------------------------------------------------------------------*/
void SSProtectionKey::setApplications(ushort units_licensed)
{
   applications = units_licensed;
}

/* setDocumentAssembler()
 *    Set the number of applications licensed on the key.
------------------------------------------------------------------------------*/
void SSProtectionKey::setDocumentAssembler(ushort units_licensed)
{
   documentAssembler = units_licensed;
}

/* getIndexServersPassword()
 *    Calculate and return the password for the number of index servers
 *    specified by units_licensed (0 through 255) by querying the secondary
 *    algorithm of the protection key. If successful, returns the password,
 *    otherwise returns 0.
 *
 *    This query will therefore produce a password that is unique for this
 *    customer, with this key, and with this many index servers licensed.
------------------------------------------------------------------------------*/
const long INDEX_SERVERS_MODULE_ID = 0;
auto_ptr<AnsiString> SSProtectionKey::getIndexServersPassword(ushort units_licensed, ISolimarLicenseSvr3* pServer)
{
   //AnsiString* pwd  = NULL;
   auto_ptr<AnsiString> pwd;

   if(pServer)
   {
      BSTR password;

      if(SUCCEEDED(pServer->GenerateModulePassword(customerNumber,
                                                  keyNumber,
                                                  productId,
                                                  INDEX_SERVERS_MODULE_ID,
                                                  units_licensed,
                                                  &password
                                                 )))
      {
         //convert the password from BSTR to and int
         pwd = auto_ptr<AnsiString>(new AnsiString(password));

         SysFreeString(password);
      }
   }
   return pwd;
}

/* getReportServersPassword()
 *    Calculate and return the password for the number of report servers
 *    specified by units_licensed (0 through 255) by querying the secondary
 *    algorithm of the protection key. If successful, returns the password,
 *    otherwise returns 0.
 *
 *
 *    This query will therefore produce a password that is unique for this
 *    customer, with this key, and with this many report servers licensed.
------------------------------------------------------------------------------*/
const long REPORT_SERVERS_MODULE_ID = 1;
auto_ptr<AnsiString> SSProtectionKey::getReportServersPassword(ushort units_licensed, ISolimarLicenseSvr3* pServer)
{
   //AnsiString* pwd = NULL;
   auto_ptr<AnsiString> pwd;

   if(pServer)
   {
      BSTR password;

      if(SUCCEEDED(pServer->GenerateModulePassword(customerNumber,
                                                  keyNumber,
                                                  productId,
                                                  REPORT_SERVERS_MODULE_ID,
                                                  units_licensed,
                                                  &password
                                                 )))
      {
         //convert the password from BSTR to and int
         pwd = auto_ptr<AnsiString>(new AnsiString(password));
         SysFreeString(password);
      }
   }
   return pwd;
}

/* getConcurrentUsersPassword()
 *    Calculate and return the password for the number of concurrent users
 *    specified by units_licensed (0 through 51175) by querying the secondary
 *    algorithm of the protection key. If successful, returns the password,
 *    otherwise returns 0. Since the units_licensed passed to this function
 *    is always rounded down to a multiple of 25, the caller should know to
 *    only set units_licensed to multiples of 25.
 *
 *    This query will therefore produce a password that is unique for this
 *    customer, with this key, and with this many concurrent users licensed.
 *
 *    We place a maximum limit of 0x7FF on concurrent users / 25 so that there
 *    is room for several other types of passwords without risking password
 *    overlap.
------------------------------------------------------------------------------*/
const long CONCURRENT_USERS_MODULE_ID = 2;
auto_ptr<AnsiString> SSProtectionKey::getConcurrentUsersPassword(ushort units_licensed, ISolimarLicenseSvr3* pServer)
{
   //AnsiString* pwd = 0;
   auto_ptr<AnsiString> pwd;
   if(pServer)
   {
      BSTR password;

      if(SUCCEEDED(pServer->GenerateModulePassword(customerNumber,
                                                  keyNumber,
                                                  productId,
                                                  CONCURRENT_USERS_MODULE_ID,
                                                  units_licensed,
                                                  &password
                                                 )))
      {
         //convert the password from BSTR to and int
         pwd = auto_ptr<AnsiString>(new AnsiString(password));

         SysFreeString(password);
      }
   }
   return pwd;
}

/* getApplicationsPassword()
 *    Calculate and return the password for the number of applications
 *    specified by units_licensed (0 through 255) by querying the secondary
 *    algorithm of the protection key. If successful, returns the password,
 *    otherwise returns 0.
 *
 *    This query will therefore produce a password that is unique for this
 *    customer, with this key, and with this many applications licensed.
------------------------------------------------------------------------------*/
const uchar APPLICATION_DATABASES_MODULE_ID = 3;
auto_ptr<AnsiString> SSProtectionKey::getApplicationsPassword(ushort units_licensed, ISolimarLicenseSvr3* pServer)
{
   //AnsiString* pwd = 0;
   auto_ptr<AnsiString> pwd;
   if(pServer)
   {
      BSTR password;

      if(SUCCEEDED(pServer->GenerateModulePassword(customerNumber,
                                                  keyNumber,
                                                  productId,
                                                  APPLICATION_DATABASES_MODULE_ID,
                                                  units_licensed,
                                                  &password
                                                 )))
      {
         //convert the password from BSTR to and int
         pwd = auto_ptr<AnsiString>(new AnsiString(password));
         SysFreeString(password);
      }
   }
   return pwd;
}

/* getApplicationsPassword()
 *    Calculate and return the password for the number of applications
 *    specified by units_licensed (0 through 255) by querying the secondary
 *    algorithm of the protection key. If successful, returns the password,
 *    otherwise returns 0.
 *
 *    This query will therefore produce a password that is unique for this
 *    customer, with this key, and with this many applications licensed.
------------------------------------------------------------------------------*/
const uchar DOCUMENT_ASSEMBLER_MODULE_ID = 4;
auto_ptr<AnsiString> SSProtectionKey::getDocumentAssemblerPassword(ushort units_licensed, ISolimarLicenseSvr3* pServer)
{
   //AnsiString* pwd = 0;
   auto_ptr<AnsiString> pwd;
   if(pServer)
   {
      BSTR password;

      if(SUCCEEDED(pServer->GenerateModulePassword(customerNumber,
                                                  keyNumber,
                                                  productId,
                                                  DOCUMENT_ASSEMBLER_MODULE_ID,
                                                  units_licensed,
                                                  &password
                                                 )))
      {
         //convert the password from BSTR to and int
         pwd = auto_ptr<AnsiString>(new AnsiString(password));
         SysFreeString(password);
      }
   }
   return pwd;
}
