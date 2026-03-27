//----------------------------------------------------------------------------*
//    filename - SSKey.h                                                      *
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
#ifndef __SSKEY_H
#define __SSKEY_H

// Key cell numbers
const short INDEX_SERVERS_CELL = 0x00;
const short REPORT_SERVERS_CELL = 0x01;
const short CONCURRENT_USERS_25_CELL = 0x02;
const short APPLICATIONS_CELL = 0x03;

// Every unit licensed in the concurrentUsers25 cell represents 25 concurrent
// users. For example, if the concurrentUsers25 cell contains a value of 2,
// then that key is licensed for 50 concurrent users.
const ushort _25_UNITS_LICENSED_PER_CONCURRENT_USERS_25 = 25;

// Max licensing
const ushort MAX_INDEX_SERVERS = 0xFF;
const ushort MAX_REPORT_SERVERS = 0xFF;
const ushort MAX_CONCURRENT_USERS_25 = 0x7FF;
const ushort MAX_CONCURRENT_USERS =
   MAX_CONCURRENT_USERS_25 * _25_UNITS_LICENSED_PER_CONCURRENT_USERS_25;
const ushort MAX_APPLICATIONS = 0xFF;



/*----------------------------------------------------------------------------*
 *                                                                            *
 *    SSProtectionKey Class:                                                  *
 *                                                                            *
 *    Derived from ProtectionKey, this class provides the interface for       *
 *    accessing a SOLsearcher protection key.                                 *
 *                                                                            *
 *----------------------------------------------------------------------------*/
class SSProtectionKey : public ProtectionKey
{
public:
   SSProtectionKey();
   SSProtectionKey(const SSProtectionKey& pkey);

   // Functions that do not perform key I/O.
   ushort getIndexServers() const;
   ushort getReportServers() const;
   ushort getConcurrentUsers() const;
   ushort getApplications() const;
   void setIndexServers(ushort units_licensed);
   void setReportServers(ushort units_licensed);
   void setConcurrentUsers(ushort units_licensed);
   void setApplications(ushort units_licensed);

   // Key I/O functions.
   ulong getIndexServersPassword(ushort units_licensed, ISolimarLicenseSvr* pServer);
   ulong getReportServersPassword(ushort units_licensed, ISolimarLicenseSvr* pServer);
   ulong getConcurrentUsersPassword(ushort units_licensed, ISolimarLicenseSvr* pServer);
   ulong getApplicationsPassword(ushort units_licensed, ISolimarLicenseSvr* pServer);

protected:
   // Protection key data (references into keyDataBlock).
   ushort& indexServers;
   ushort& reportServers;
   ushort& concurrentUsers25;
   ushort& applications;
};



#endif
