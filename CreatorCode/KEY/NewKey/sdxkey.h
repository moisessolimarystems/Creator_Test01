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
#ifndef __SDXKEY_H
#define __SDXKEY_H


#ifndef min
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#endif   

/*----------------------------------------------------------------------------*
 *                                                                            *
 *    SDXDesignerProtectionKey Class:                                           *
 *                                                                            *
 *    Derived from SpdProtectionKey, this class adds functionality for        *
 *    SDXDesigner keys.                                                         *
 *                                                                            *
 *----------------------------------------------------------------------------*/
class SDXDesignerProtectionKey : public SpdProtectionKey
{
public:
   SDXDesignerProtectionKey() :  SpdProtectionKey() {}
   ~SDXDesignerProtectionKey() {}   
   SDXDesignerProtectionKey(const SDXDesignerProtectionKey& pkey) : SpdProtectionKey(pkey) {}

protected:
   virtual ushort keyMessageBox(
      const char* msg, ushort options, bool modal=true, bool display_message=true);
};



#endif

