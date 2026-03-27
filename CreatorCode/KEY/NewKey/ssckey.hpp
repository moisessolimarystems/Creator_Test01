//----------------------------------------------------------------------------*
//    filename - ssckey.hpp                                                   *
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
#ifndef __SSCKEY_HPP
#define __SSCKEY_HPP


#ifndef min
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#endif   

/*----------------------------------------------------------------------------*
 *                                                                            *
 *    SolScriptProtectionKey Class:                                           *
 *                                                                            *
 *    Derived from SpdProtectionKey, this class adds functionality for        *
 *    SOLscript keys.                                                         *
 *                                                                            *
 *----------------------------------------------------------------------------*/
class SolScriptProtectionKey : public SpdProtectionKey
{
public:
   SolScriptProtectionKey() : SpdProtectionKey() {}
   SolScriptProtectionKey(const SolScriptProtectionKey& pkey) : SpdProtectionKey(pkey) {}

protected:
   virtual ushort keyMessageBox(
      const char* msg, ushort options, bool modal=true, bool display_message=true);
};



#endif

