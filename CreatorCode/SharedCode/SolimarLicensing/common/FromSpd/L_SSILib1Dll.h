//----------------------------------------------------------------------------*
//    filename - SSILib1Dll.h                                                 *
//                                                                            *
//----------------------------------------------------------------------------*
//----------------------------------------------------------------------------*
//                                                                            *
//    Solimar Print/Director                                                  *
//                                                                            *
//    Copyright 1992-2010 Solimar Systems, Inc.  All rights reserved.         *
//                                                                            *
//----------------------------------------------------------------------------*
#pragma once

#ifndef _SSILib1Dll_h																					// CR.32662.V04; added.
#define _SSILib1Dll_h																					// CR.32662.V04; added.

#ifdef _WIN32_WINNT 
	#if _WIN32_WINNT < 0x0601
	#define _WIN32_WINNT  0x0601
	#endif
#endif
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#pragma pack(push, 8)                                                               // CR.30115; added.
#include <windows.h>

#include <string>

#pragma warning(disable: 4091)  /* 'typedef ': ignored when no variable is declared */ // CR.24131; added.

// CR.27366; If GetMessage macro is not defined, define it.
//    needed by ssilib wrapper project at a minimum.
#ifndef GetMessage                                                                  // CR.27366; added.
   #ifdef UNICODE
      #define GetMessage  GetMessageW
   #else
      #define GetMessage  GetMessageA
   #endif // !UNICODE
#endif
//
// CR.13870; Created these helper macros to help define linkage when using methods from the 
// SSILib1 library.
//
#ifdef SSILIB1_DLL
   //
   // SSILIB1 will be added to a DLL.
   //
   #define SSILIB1_IMPORT __declspec(dllimport)
   #define SSILIB1_EXPORT __declspec(dllexport)
#else
   #ifdef NO_SSILIB1_DLL
      //
      // NO SSILIB1 DLL is being used. 
      // Case where SSILIB1 DLL source files are included directly into a project (vs. referencing the SSILIB1 DLL)
      // 
      #define SSILIB1_IMPORT 
      #define SSILIB1_EXPORT
   #else
      //
      // SPDBASE DLL is being used (vs source files being included in project directly)
      //
      #define SSILIB1_IMPORT __declspec(dllexport)
      #define SSILIB1_EXPORT __declspec(dllimport)
   #endif
#endif

// Ignore dll-interface warning for template instantiations.
#pragma warning (disable : 4251)

#ifndef sol_assert
#define sol_assert(x) if (!x) { _ASSERTE(x); _CrtDbgBreak(); }
#endif
//
// Forward declarations needed for Interface methods.
//
class CSpdUserMessageIf; 
class CSpdJobManagerConsumerIF; 
class CSpdJobPropertyIF; 
class CSpdJobNameIf;                                                                // CR.17124.Item37; added.


/*
 * namespace SSILib1InterfaceDefs:
 *
 * SLB 07.aug.2013 CR.17124.Item37; SSILib is now a DLL. Completely changed how projects can over-ride the default functionality
 * needed by this library. Removed old interface methods, IFGetxxxxx(), and replaced with new ones IFSetxxxx(). The idea before is that
 * each product just "defined" a correseponding IFGetxxxx() method and, at link time, it would automatically over-ride the default 
 * behavior.
 * This does not work that well when SSILib1 is a DLL. So, the library has been modified so that it tracks pointers to the services which 
 * can be over-ridden by the project using it. Now, if the project desires a different behavior, it calls one of the new IFSetxxxx() methods
 * to replace the handler for that service.
 *
 * IFSetUserMessageHandler() - Project/Product uses this method to over-ride how user messages are handled within
 * SSILib_XXX.DLL 
 * 
 * IFSetJobManagerConsumer() - Project/Product uses this method to over-ride how Job and Report Ids are generated within 
 * SSILib_XXX.DLL 
 *
 * IFSetJobNameHandler() - Project/Product uses this method to over-ride how SPDE type job names are generated within 
 * SSILib_XXX.DLL 
 *
 */
namespace SSILib1InterfaceDefs
{
   SSILIB1_EXPORT void IFSetUserMessageHandler(                                     // CR.17124.Item37; added.
      CSpdUserMessageIf *pNewHandler);  
   SSILIB1_EXPORT void IFSetJobManagerConsumer(                                     // CR.17124.Item37; added.
      CSpdJobManagerConsumerIF *pNewHandler);    
   SSILIB1_EXPORT void IFSetJobNameHandler(                                         // CR.17124.Item37; added.
      CSpdJobNameIf *pNewHandler);  
   SSILIB1_EXPORT void IFNewJobName(                                                // CR.17124.Item37; added.
      const CSpdJobPropertyIF *pjmJobMsg,
      std::wstring &wsOutput,
      const std::wstring &wsWildCard,
      int nNumIndex,
      int nNumIndexRadix,
      DWORD dwOsFlags,
      bool bFixStringVarsOnly,
      bool bUseOnlyFileNameInStringVar);                                            // CR.24077; added.
   SSILIB1_EXPORT void IFNewJobName2(                                                // CR.17124.Item37; added.
      const CSpdJobPropertyIF *pjmJobMsg,
      std::wstring &wsOutput,
      const std::wstring &wsWildCard,
      int nNumIndex,
      int nNumIndexRadix,
      DWORD dwOsFlags,
      bool bFixStringVarsOnly,
      bool bUseOnlyFileNameInStringVar);                                            // CR.24077; added.
}

#pragma pack(pop)                                                                   // CR.30115; added.

#endif