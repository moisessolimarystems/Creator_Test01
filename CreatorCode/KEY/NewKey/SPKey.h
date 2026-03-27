//----------------------------------------------------------------------------*
//    filename - SPKey.h                                                      *
//                                                                            *
//    Class(es):                                                              *
//       SPProtectionKey                                                      *
//                                                                            *
//----------------------------------------------------------------------------*
//----------------------------------------------------------------------------*
//                                                                            *
//    SS Single Platform                                                      *
//                                                                            *
//    Copyright 1992-2003 Solimar Systems, Inc.  All rights reserved.         *
//                                                                            *
//----------------------------------------------------------------------------*
#ifndef __SPKEY_H
#define __SPKEY_H


#ifndef min
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#endif

/*----------------------------------------------------------------------------*
 *                                                                            *
 *    SPProtectionKey Class:                                           *
 *                                                                            *
 *    Derived from SpdProtectionKey, this class adds functionality for        *
 *    SOLscript keys.                                                         *
 *                                                                            *
 *----------------------------------------------------------------------------*/
class SPProtectionKey : public SpdProtectionKey
{
public:
   SPProtectionKey() : SpdProtectionKey() {}
   SPProtectionKey(const SPProtectionKey& pkey) : SpdProtectionKey(pkey) {}

protected:
   virtual ushort keyMessageBox(
      const char* msg, ushort options, bool modal=true, bool display_message=true);
};



#endif