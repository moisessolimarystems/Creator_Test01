//-------------------------------------------------------------------------*
//    filename - keyprod.h                                                 *
//                                                                         *
//-------------------------------------------------------------------------*
//-------------------------------------------------------------------------*
//                                                                         *
//    Solimar Print/Director                                               *
//                                                                         *
//    Copyright (c) 1992-1996 by Solimar Systems, Inc.                     *
//    All Rights Reserved.                                                 *
//                                                                         *
//-------------------------------------------------------------------------*
#ifndef __KEYPROD_H
#define __KEYPROD_H

enum ProductLength {LONG_PRODUCT_NAME, MEDIUM_PRODUCT_NAME, SHORT_PRODUCT_NAME};
enum ProductId {
   SPD_PRODUCT=0, XIMAGE_PRODUCT=1, XIMAGENT_PRODUCT=2, SOLPCLNT_PRODUCT=3,
   NOT_AVAILABLE=4, INDEX_PLUGIN=5, CONNECT_PRODUCT=6, ICONVERT_PRODUCT=7,
   SOLSEARCHER_ENTERPRISE_PRODUCT=8, SOLSCRIPT_PRODUCT=9,
   SDX_DESIGNER_PRODUCT=10, QUANTUM_PRODUCT=11, RUBIKA_PRODUCT=12, SOLFUSION_PRODUCT=13, SPDE_PRODUCT=14, SOLITRACK_PRODUCT=16, NUMBER_OF_PRODUCTS=16,
   UNKNOWN_PRODUCT=0xFFFF};

extern const char* ProductText[];
extern const char* mediumProductText[];
extern const char* longProductText[];

enum QueryType {PRIMARY_1, PRIMARY_2, SECONDARY_1, SECONDARY_2};


typedef unsigned short ushort;
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

#ifndef __WIN32__
//enum bool {false, true};
#endif



#endif
