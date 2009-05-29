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
#ifndef __SOLFUSIONKEY_H
#define __SOLFUSIONKEY_H


#ifndef min
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#endif   

/*----------------------------------------------------------------------------*
 *                                                                            *
 *    SOLfusionProtectionKey Class:                                           *
 *                                                                            *
 *    Derived from SpdProtectionKey, this class adds functionality for        *
 *    SOLfusion keys.                                                         *
 *                                                                            *
 *----------------------------------------------------------------------------*/
class SOLfusionProtectionKey : public SpdProtectionKey
{
public:
   SOLfusionProtectionKey() :  SpdProtectionKey() {}
   ~SOLfusionProtectionKey() {}
   SOLfusionProtectionKey(const SOLfusionProtectionKey& pkey) : SpdProtectionKey(pkey) {}

protected:
   virtual ushort keyMessageBox(
      const char* msg, ushort options, bool modal=true, bool display_message=true);
};



#endif

