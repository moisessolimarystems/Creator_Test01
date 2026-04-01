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

//#define read _read
//#define write _write
//#define open _open
//#define close _close
#define lseek _lseek
#define strupr _strupr
#define stricmp _stricmp
#define chsize _chsize
#define swab _swab
#define tell _tell

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

///#ifndef TRUE
///#  define TRUE    1
///#  define FALSE   0
///#endif

//min copied from windef.h
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

#ifndef SPACE
#define SPACE      32
#endif

#ifndef LFEED
#define LFEED      10
#endif

#ifndef CARRET
#define CARRET     13
#endif

#ifndef APOSTROPHE
#define APOSTROPHE 39
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

