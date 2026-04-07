//----------------------------------------------------------------------------*
//    filename - AliasedResourceIF.h                                          *
//                                                                            *
//----------------------------------------------------------------------------*
//----------------------------------------------------------------------------*
//                                                                            *
//    Solimar Print/Director                                                  *
//                                                                            *
//    Copyright 1992-2013 Solimar Systems, Inc.  All rights reserved.         *
//                                                                            *
// SLB 29.mar.2013 CR.15891.V03; Created.
//----------------------------------------------------------------------------*
#pragma once
#ifndef _AliasedResourceIF_h																			// CR.33897; added.
#define _AliasedResourceIF_h																			// CR.33897; added.

#include "L_SSILib1Dll.h"
#include "L_SpdStreamable4.h"

using namespace SpdStreamable;

#ifdef ALIASED_RESOURCEIF_DEFINED
//
// Aliased Resource interface methods are defined in this target.
//
#  ifdef ALIASED_RESOURCEIF_INDLL
   //
   // Case where Aliased Resource interface methods are in a DLL 
   // Methods must be exported in this case.
   //
#     define ALIASED_RESOURCE_LINKAGE __declspec(dllexport)
#  else
   //
   // Case wehre Aliased Resource interface methods are directly linked
   // into .exe (current target is an .exe)
   //
#     define ALIASED_RESOURCE_LINKAGE 
#  endif
#else
//
// Aliased Resource interface methods are not defined in this target. 
//
#  define ALIASED_RESOURCE_LINKAGE __declspec(dllimport)
#endif

#ifndef sol_assert
#define sol_assert(x) if (!x) { _ASSERTE(x) ; _CrtDbgBreak() ; }
#endif
#ifndef WIDEN
#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define __WFUNCTION__ WIDEN(__FUNCTION__)
#endif

//
// Forward declarations needed for Interface methods.
//

//
// Framework Interface methods 
//
namespace AliasedResourceIf
{  //
   // Defines access methods to static SPDE Framework methods that may be useful.
   //
   ALIASED_RESOURCE_LINKAGE HRESULT IFADTGetResourceName(
      const CSpdCString &wsAliasName,
      CSpdCString &wsResourceName);
}

#endif