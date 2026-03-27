//----------------------------------------------------------------------------*
//    filename - pdfkey.cpp                                                   *
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
#include <windows.h>
//#include "3prty\rainbow\winnt\spromeps.h"

#include "keyprod.h"
#include "pkey.h"
#include "spdkey.h"
#include "pdfkey.hpp"



/*----------------------------------------------------------------------------*
 *                                                                            *
 *    PDFUtilityProtectionKey Class:                                           *
 *                                                                            *
 *    Derived from SpdProtectionKey, this class adds functionality for        *
 *    SOLscript keys.                                                         *
 *                                                                            *
 *----------------------------------------------------------------------------*/

/* keyMessageBox()
 *    Display a platform-independent message box with the specified message
 *    and buttons. SOLscript message boxes with just the OK button are always
 *    non-modal, since modal dialogs could disrupt the flow of SPD.
------------------------------------------------------------------------------*/
ushort PDFUtilityProtectionKey::keyMessageBox(
   const char* msg, ushort options, bool modal, bool display_message)
{
   // pkYesNoButton message boxes can be modal, but pkOKButton message boxes
   // must be non-modal.
   if (options&pkOKButton)
      modal = false;

   return SpdProtectionKey::keyMessageBox(msg, options, modal, display_message);
}
//------------------------------------------------------------------------------





