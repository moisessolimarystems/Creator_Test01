//----------------------------------------------------------------------------*
//    filename - ssckey.hpp                                                   *
//                                                                            *
//    Class(es):                                                              *
//       PDFUtilityProtectionKey                                               *
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
 *    PDFUtilityProtectionKey Class:                                           *
 *                                                                            *
 *    Derived from SpdProtectionKey, this class adds functionality for        *
 *    SOLscript keys.                                                         *
 *                                                                            *
 *----------------------------------------------------------------------------*/
class PDFUtilityProtectionKey : public SpdProtectionKey
{
public:
   PDFUtilityProtectionKey() : SpdProtectionKey() {}
   PDFUtilityProtectionKey(const PDFUtilityProtectionKey& pkey) : SpdProtectionKey(pkey) {}

protected:
   virtual ushort keyMessageBox(
      const char* msg, ushort options, bool modal=true, bool display_message=true);
};



#endif

