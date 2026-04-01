//----------------------------------------------------------------------------*
//    filename - sdxkey.hpp                                                   *
//                                                                            *
//    Class(es):                                                              *
//       SolScriptProtectionKey                                               *
//                                                                            *
//----------------------------------------------------------------------------*
//----------------------------------------------------------------------------*
//                                                                            *
//    Solimar Print/Director                                                  *
//                                                                            *
//    Copyright 1992-2003 Solimar Systems, Inc.  All rights reserved.         *
//                                                                            *
//----------------------------------------------------------------------------*
#ifndef __SOLITRACKKEY_H
#define __SOLITRACKKEY_H


#ifndef min
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#endif   

/*----------------------------------------------------------------------------*
 *                                                                            *
 *    SOLitrackProtectionKey Class:                                           *
 *                                                                            *
 *    Derived from SpdProtectionKey, this class adds functionality for        *
 *    SOLitrack keys.                                                         *
 *                                                                            *
 *----------------------------------------------------------------------------*/
class SOLitrackProtectionKey : public SpdProtectionKey
{
public:
   SOLitrackProtectionKey() :  SpdProtectionKey() {}
   ~SOLitrackProtectionKey() {}
   SOLitrackProtectionKey(const SOLitrackProtectionKey& pkey) : SpdProtectionKey(pkey) {}

protected:
   virtual ushort keyMessageBox(
      const char* msg, ushort options, bool modal=true, bool display_message=true);
};



#endif

