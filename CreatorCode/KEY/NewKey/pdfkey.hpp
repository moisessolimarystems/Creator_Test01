//----------------------------------------------------------------------------*
//    filename - pdfkey.hpp                                                   *
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
#ifndef __PDFKEY_H
#define __PDFKEY_H


#ifndef min
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#endif   

/*----------------------------------------------------------------------------*
 *                                                                            *
 *    PDFUtilityProtectionKey Class:                                           *
 *                                                                            *
 *    Derived from SpdProtectionKey, this class adds functionality for        *
 *    SDXDesigner keys.                                                         *
 *                                                                            *
 *----------------------------------------------------------------------------*/
class PDFUtilityProtectionKey : public SpdProtectionKey
{
public:
   PDFUtilityProtectionKey() : SpdProtectionKey() {}
   ~PDFUtilityProtectionKey() {}
   PDFUtilityProtectionKey(const PDFUtilityProtectionKey& pkey) : SpdProtectionKey(pkey) {}

protected:
   virtual ushort keyMessageBox(
      const char* msg, ushort options, bool modal=true, bool display_message=true);
};



#endif

