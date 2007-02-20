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

#include <windows.h>
#include <fstream.h>             // file i/o functionality
#include "key\newkey\keyprod.h"
//#include "key\3prty\rainbow\winnt\Spromeps.h"
#include "key\newkey\pkey.h"
#include "key\newkey\spdkey.h"
#include "key\newkey\sskey.h"
#include "key\newkey\spdekey.h"
#include "moduleDetail.h"
#include "SolimarLicensing\SolimarLicenseServer\_SolimarLicenseServer.h"
#include "MultidimensionalSafeArray.h"

#define MAXCOMMAND               256

class SKeyRecord;
class ProtectionKey;
class SpdProtectionKey;
class SSProtectionKey;
class SpdeProtectionKey;
/*--------------------------------------------------------------------*
 *                                                                    *
 *   KeyMaster Class:                                                 *
 *                                                                    *
 *   This class provides the Protection Key interface for the Creator.*
 *                                                                    *
 *--------------------------------------------------------------------*/
class KeyMaster {

public:
   KeyMaster();
   ~KeyMaster();

   //operate on ProtectionKey passed in
   void getPermanentPassword(ProtectionKey*, char*);
   void getProductVersionPassword(ProtectionKey*, ushort, char*);
   void getExtensionPassword(ProtectionKey*,
                             ushort,
                             ProductId,
                             ushort,
                             ushort,
                             char*
                            );
   void getModulePassword(ProtectionKey*, uchar, ProductId, ushort, unsigned int, ushort, char*);
   void getOutputPassword(SpdProtectionKey*, ushort, ushort, char*);
   void getPagesPerMinutePassword(SpdProtectionKey*, ushort, ushort, char*, ushort, long);
   //void getOutputPassword(SpdeProtectionKey*, ushort, ushort, char*);
   void getOperatorSessionPassword(SpdeProtectionKey*, ushort, ushort, char*);
   void getUserSessionPassword(SpdeProtectionKey*, ushort, ushort, char*);
   void getPagesPerMinutePassword(SpdeProtectionKey*, ushort, ushort, char*, ushort, long);

   auto_ptr<AnsiString> getIndexServersPassword(SSProtectionKey*, ushort);
   auto_ptr<AnsiString> getReportServersPassword(SSProtectionKey*, ushort);
   auto_ptr<AnsiString> getConcurrentUsersPassword(SSProtectionKey*, ushort);
   auto_ptr<AnsiString> getApplicationServerPassword(SSProtectionKey*, ushort);
   auto_ptr<AnsiString> getDocumentAssemblerPassword(SSProtectionKey*, ushort);

   //simulate applying password to ProtectionKey passed in
   void applyModZeroPassword(SKeyRecord* key_record, ModuleDetail* detail, unsigned short units);
   void applyModPassword(SKeyRecord* keyrec, /*unsigned char module_id,*/ ModuleDetail* detail, unsigned short units);
   void applyOutputPassword(SKeyRecord* keyrec, int output_units);
   void applyOperatorSessionPassword(SKeyRecord* keyrec, int operator_session);
   void applyUserSessionPassword(SKeyRecord* keyrec, int user_session);
   void applyPermanentPassword(SKeyRecord* key_record);
   void applyVersionPassword(SKeyRecord* keyrec, unsigned short version);
   void applyExtensionPassword(SKeyRecord* keyrec, unsigned short days);
   void applyPagesPerMinutePassword(SKeyRecord* keyrec);

   void applyIndexServersPassword(SKeyRecord* keyrec, unsigned short units_licensed);
   void applyReportServersPassword(SKeyRecord* keyrec, unsigned short units_licensed);
   void applyConcurrentUsersPassword(SKeyRecord* keyrec, unsigned short units_licensed);
   void applyApplicationPassword(SKeyRecord* keyrec, unsigned short units_licensed);
   void applyDocumentAssemblerPassword(SKeyRecord* keyrec, unsigned short units_licensed);

   //operate on scratchKey
   short clear();
   short deactivate();
   bool found();
   HRESULT initDriver();
   ProtectionKey* newKey(KeyDataBlock*);
   ProtectionKey* newKey(ProductId product_id) {return ProtectionKey::newKey(product_id);}
   void resetKey(SKeyRecord*, ProductId);
   short program(SKeyRecord* key_record);
   bool programmed();

   //operate on specificed SKeyRecord
   void initializeMaxModules(SKeyRecord*);
   void initializeMinModules(SKeyRecord*);
   short read(SKeyRecord*);
   short read(SKeyRecord*, ProductId);

   HRESULT InitPasswordPacket();
   HRESULT FinalizePasswordPacket();
   HRESULT AppendPasswordToPacket(VARIANT vtExpires, BSTR password);
   HRESULT GetPasswordPacket(VARIANT* pvtPacketData);
   HRESULT GetVerificationCode(BSTR* VerificationCode);
protected:
   ProtectionKey* newKey();
   HRESULT KeyMaster::setHandle();

   //ptr to the server object. Used to access the lower layer code.
   ISolimarLicenseSvr* pTheServer;

   //The list of keys returned from the lower layer app
   VARIANT* pKeyList;

   ProtectionKey* CurrentKey;
   BSTR*   	      CurrentKeyID;
};

extern KeyMaster* keyMaster;

#endif
