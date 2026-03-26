//------------------------------------------------------------*
// filename -      managers.h                                 *
//                                                            *
//        Contains Key, 												  *
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
#ifndef __MANAGERS
#define __MANAGERS


#include <fstream.h>             // file i/o functionality
#include "key\key\keyprod.h"
#include "key\3prty\rainbow\winnt\Spromeps.h"
#include "key\key\pkey.h"
#include "key\key\spdkey.h"
#include "key\key\sskey.h"
#include "moduleDetail.h"

//#include "keymaster.h"


#define MAXCOMMAND               256


class SKeyRecord;
class ProtectionKey;
class SpdProtectionKey;
class SSProtectionKey;



/*--------------------------------------------------------------------*
 *                                                                    *
 *   KeyMaster Class:                                                 *
 *                                                                    *
 *   This class provides the Protection Key interface for the Creator.*
 *                                                                    *
 *--------------------------------------------------------------------*/
class KeyMaster {
   // packet member must come before everything else so it is DWORD aligned
   RB_SPRO_APIPACKET packet;
   const char* server_name;
public:
   KeyMaster() : scratchkey(&packet) {server_name = NULL;}

   //operate on ProtectionKey passed in
   ulong getPermanentPassword(ProtectionKey*);
   ulong getProductVersionPassword(ProtectionKey*, ushort);
   ulong getExtensionPassword(ProtectionKey*, uchar, ProductId, ushort, ushort);

   ulong getModulePassword(SpdProtectionKey*, uchar, ProductId, ushort, ushort);
   //ulong getModZeroPassword(SpdProtectionKey*, uchar, ProductId, ushort);
   ulong getOutputPassword(SpdProtectionKey*, ushort);
   ulong getPagesPerMinutePassword(SpdProtectionKey*, ushort);

   ulong getIndexServersPassword(SSProtectionKey*, ushort);
   ulong getReportServersPassword(SSProtectionKey*, ushort);
   ulong getConcurrentUsersPassword(SSProtectionKey*, ushort);
   ulong getApplicationServerPassword(SSProtectionKey*, ushort);

   //simulate applying password to ProtectionKey passed in
   void applyModZeroPassword(SKeyRecord* key_record, unsigned short mod_id, unsigned short units);
   void applyModPassword(SKeyRecord* keyrec, unsigned char module_id, unsigned short units);
   void applyOutputPassword(SKeyRecord* keyrec, int output_units);
   void applyPermanentPassword(SKeyRecord* key_record);
   void applyVersionPassword(SKeyRecord* keyrec, unsigned short version);
   void applyExtensionPassword(SKeyRecord* keyrec, unsigned char days);
   void applyPagesPerMinutePassword(SKeyRecord* keyrec);

   void applyIndexServersPassword(SKeyRecord* keyrec, unsigned short units_licensed);
   void applyReportServersPassword(SKeyRecord* keyrec, unsigned short units_licensed);
   void applyConcurrentUsersPassword(SKeyRecord* keyrec, unsigned short units_licensed);
   void applyApplicationPassword(SKeyRecord* keyrec, unsigned short units_licensed);

   //operate on scratchKey
   short clear();
   short deactivate();
   bool found();
   short initDriver();
   ProtectionKey* newKey(KeyDataBlock*);
   ProtectionKey* newKey(ProductId product_id) {return ProtectionKey::newKey(product_id, &packet);}
   void resetKey(SKeyRecord*, ProductId);
   short program(ProtectionKey*);
   bool programmed();

   //operate on specificed SKeyRecord
   void initializeMaxModules(SKeyRecord*);
   void initializeMinModules(SKeyRecord*);
   void initializeMaxValues(SKeyRecord*);
   void initializeMinValues(SKeyRecord*);
   bool hasModules(SKeyRecord* keyrec);
   short read(SKeyRecord*);
   short read(SKeyRecord*, ProductId);

protected:
   ProtectionKey* newKey();
   short setHandle(ProtectionKey*);

   ProtectionKey scratchkey;
};

extern KeyMaster* keyMaster;

#endif
