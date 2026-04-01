//----------------------------------------------------------------------------*
//    filename - SpdBaseDll.h                                               *
//                                                                            *
//----------------------------------------------------------------------------*
//----------------------------------------------------------------------------*
//                                                                            *
//    Solimar Print/Director                                                  *
//                                                                            *
//    Copyright 1992-2004 Solimar Systems, Inc.  All rights reserved.         *
//                                                                            *
//----------------------------------------------------------------------------*
#pragma once

#include <windows.h>
#include <string>

#ifndef SPDBASE_EXPORTS
   #ifndef SPDUTILS_NOT_A_DLL
      #define SPDBASE_IMP_EXP __declspec(dllimport)
   #else
      #define SPDBASE_IMP_EXP  
   #endif
#else
   #define SPDBASE_IMP_EXP  __declspec(dllexport)
#endif

// Ignore dll-interface warning for template instantiations.
#pragma warning (disable : 4251)



