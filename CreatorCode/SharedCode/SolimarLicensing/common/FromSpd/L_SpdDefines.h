//----------------------------------------------------------------------------------------*
// filename -       SpdDefines.h                                                             *
//                                                                                        *
// Class(es):                                                                             *
//                                                                                        *
//                                                                                        *
//    Copyright (c) 1995-97 by Solimar Systems, Inc.                                      *
//    All Rights Reserved.                                                                *
//                                                                                        *
//----------------------------------------------------------------------------------------*
#pragma once

#ifndef _SpdDefines_h																					// CR.33897; added.
#define _SpdDefines_h																					// CR.33897; added.

#include "L_quadword.h"																					// CR.13870; added.
#ifndef Boolean
   //typedef bool Boolean;
   #define Boolean bool
   #define True true
   #define False false
#endif

#define SWAPBYTES(pw)      (*pw = (ushort)( ((*((ushort *)pw)<<8)) | (*((ushort *)pw)>>8)) )
#define SWAPWORDS(pl)      (*pl = (ulong)( ((*((ulong *)pl)<<16)) | (*((ulong *)pl)>>16)) )
#define MAKEWORD2(hi,lo)    MAKEWORD(lo,hi)
#define MAKELONG2(hi,lo)    MAKELONG(lo,hi)

#define strupr _strupr
#define stricmp _stricmp
#define unlink _unlink  // defined to reduce warning messages.

typedef unsigned char BYTE ;
typedef unsigned char uchar ;
typedef unsigned long ulong ;
typedef unsigned short ushort ;

///#define _RTLENTRY __cdecl

// HBAD - Handle is invalid
//			 used by:
//							Scsi32.dll
#ifndef HBAD
#define HBAD  0xFFFFFFFFL
#endif

// min copied from windef.h
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef YES
#  define YES 1
#  define NO  0
#endif

#ifndef MSB
#  define MSB(w)  ( (BYTE)(((w)>>8) & 0xff) )
#  define LSB(w)  ( (BYTE)((w) & 0xff) )
#endif

#ifndef A_SPACE            // CR.13870; renamed SPACE to A_SPACE because of conflicts.
#define A_SPACE      32    
#endif

#ifndef LFEED
#define LFEED      10
#endif

#ifndef CARRET
#define CARRET     13
#endif

#ifndef A_APOSTROPHE       // CR.13870; renamed APOSTROPHE to A_APOSTROPHE because of conflicts.
#define A_APOSTROPHE 39       
#endif

#ifndef ESCAPE
#define ESCAPE     27
#endif

#define XLATE_EBC	0
#define XLATE_ASC 1
#define XLATE_OCT 2
#define XLATE_HEX 3
#define XLATE_H2  4
#define XLATE_H6  5

#ifndef WIDEN
#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define __WFUNCTION__ WIDEN(__FUNCTION__)
#endif

#ifndef sol_assert         // CR.13870; added.
#define sol_assert(x) if (!x) { _ASSERTE(x) ; _CrtDbgBreak() ; }
#endif

#endif