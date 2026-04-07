//----------------------------------------------------------------------------------------*
// filename - SpdUtils.h                                             
//                                                                          
//             This module contains some helper functions dealing with DOS file
//             naming conventions and other misc. routines                     
//                                                                             
// Namespace: SpdUtils.
//                                                                             
// Release Info:                                                               
// Nm     Date         Discription                                             
// SLB   07.jan.98   Added SCreateDirectory()                                  
// SLB   28.oct.97   SPD Nt port.                                              
// SLB 17.jul.2020 CR.29523; cleaned up for sharing with iCONVERT 10.3+.   
// SLB 28.apr.2021 CR.30450; Moved definition of class CSmartCriticalSection into CSpdBlock.h
// SLB 12.sep.2023 CR.32574.V02; FixFileNameNtfs() gets new param bSlashesOk.
//----------------------------------------------------------------------------------------*
#pragma once

#ifndef _SpdUtils_h																						// CR.32662.V04; added.
#define _SpdUtils_h																						// CR.32662.V04; added.

#include "L_SSILib1Dll.h"
#include <string>
#include "L_quadword.h"
#include "L_SpdStreamable4.h"
#include <exception>

#ifndef BIT
#define BIT(x) (((QUADWORD)1) << x)
#endif

#define fnsplit(a,b,c,d,e) SpdUtils::wsplitpath(a,b,c,d,e)
#define fnmerge(a,b,c,d,e) SpdUtils::wmakepath(a,b,c,d,e)
//
using namespace SpdStreamable;

namespace SpdUtils
{
   enum OsTypeFlags
   {
      ostNoRestricts = 0x1,
      ostDOS = 0x2,
      ostNTFS = 0x4,
      ostUNIX = 0x8,
      ostNovQueue = 0x10,
      ostXeroxDJDE = 0x20,
      ostWildcardPassthrough = 0x8000,                                              // CR.26999.Item27.V02; added.
   }; 

   SSILIB1_EXPORT void GetCurrentUserAndDomain(                                     // CR.28872.V02; added.
      CSpdCString &wsUserName,
      CSpdCString &wsDomainName,
      CSpdCString &wsSid);
   SSILIB1_EXPORT CSpdCString SidToString(                                          // CR.28872.V02; added.
      PSID pSid);
   SSILIB1_EXPORT CSpdCString GetComSessionsSid(                                    // CR.28872.V01; added.
      bool bUsesLogonAuthentication,
      const CSpdCString &wsLogonAccountName,
      const CSpdCString &wsLogonDomainName,
      const CSpdCString &wsLogonPassword,
      CSpdCString &wsAccountName);

   SSILIB1_EXPORT const CSpdCString &GetComputerNameW();                            // CR.29058; modified. CR.26999.Item82; added.
	SSILIB1_EXPORT const CSpdCString& GetComputerNameBIOS();                         // CR.34040; added. 
   SSILIB1_EXPORT CSpdCString GenerateGUID();                                       // CR.26999.Item82; added.

   SSILIB1_EXPORT bool VerifyFolder(                                                // CR.13870; Now uses SSILIB1_EXPORT to define linkage.
      const std::wstring &wsDir, 
      bool bWriteRights = false);  
   SSILIB1_EXPORT bool VerifyFolderA(                                               // CR.13870; Now uses SSILIB1_EXPORT to define linkage.
      const char *strDir, 
      bool bWriteRights = false); 
   SSILIB1_EXPORT void FixFileChar(                                                 // CR.13870; Now uses SSILIB1_EXPORT to define linkage.
      wchar_t &c, 
      DWORD os_type_flags,
      bool bSlashesOk = false);                                                     // CR.7698; added.
   SSILIB1_EXPORT std::wstring FixDosName(                                          // CR.13870; Now uses SSILIB1_EXPORT to define linkage.
      const std::wstring &wsFileName, 
      bool bLongFileNamesOk = false);  
   SSILIB1_EXPORT std::wstring FixFileName(                                         // CR.13870; Now uses SSILIB1_EXPORT to define linkage.
      const std::wstring &wsFileName, 
      DWORD dwOsTypes);  
   SSILIB1_EXPORT void FixFileNameNtfs(                                             // CR.13870; Now uses SSILIB1_EXPORT to define linkage.
      const SpdStreamable::CSpdCString &wsInputFileName, 
		const SpdStreamable::CSpdCString &wsReplacementString,								// CR.32574; added.
      SpdStreamable::CSpdCString &wsOutputFileName,
		bool bSlashesOk = false);																		//CR.32574.V02; added.
   SSILIB1_EXPORT char * newStr(                                                    // CR.13870; Now uses SSILIB1_EXPORT to define linkage.
      const char *szString); 
   //SSILIB1_EXPORT int  xatoi(const char *s); 
   SSILIB1_EXPORT bool CreateFolder(                                                // CR.13870; Now uses SSILIB1_EXPORT to define linkage.
      const std::wstring &wsNewDir, 
      LPSECURITY_ATTRIBUTES lpsecattribs = NULL);  
   SSILIB1_EXPORT void GetFullPath(                                                 // CR.13870; Now uses SSILIB1_EXPORT to define linkage.
      std::wstring &wsNewPath);  
   SSILIB1_EXPORT void wsplitpath(                                                  // CR.13870; Now uses SSILIB1_EXPORT to define linkage.
      const std::wstring &wsPath, 
      std::wstring &wsDrive,
      std::wstring &wsDir,
      std::wstring &wsFName,
      std::wstring &wsExt); 
   SSILIB1_EXPORT void wmakepath(                                                   // CR.13870; Now uses SSILIB1_EXPORT to define linkage.
      std::wstring &wsPath,
      const std::wstring &wsDrive,
      const std::wstring &wsDir,
      const std::wstring &wsFName,
      const std::wstring &wsExt); 
   SSILIB1_EXPORT std::wstring ExtractFileDrive(                                    // CR.13870; Now uses SSILIB1_EXPORT to define linkage.
      const std::wstring &wsPathName); 
   SSILIB1_EXPORT DWORD MaxFileNameLength(                                          // CR.13870; Now uses SSILIB1_EXPORT to define linkage.
      const std::wstring &wsDriveString); 
   SSILIB1_EXPORT std::wstring wstrupr(                                             // CR.13870; Now uses SSILIB1_EXPORT to define linkage.
      std::wstring& ws);
   SSILIB1_EXPORT bool WildcardCompare(                                             // CR.13870; Now uses SSILIB1_EXPORT to define linkage.
      const SpdStreamable::CSpdCString& wsStr, 
      const SpdStreamable::CSpdCString& wsMask, 
      bool case_sensitive = false); 
   SSILIB1_EXPORT bool XmlEscapeString(                                             // CR.13870; Now uses SSILIB1_EXPORT to define linkage.
      const wchar_t *pwcsIn, 
      wchar_t *pwcsOut, 
      size_t stOutBufLen, 
      size_t &stOutStringLen); 
   SSILIB1_EXPORT bool XmlUnescapeString(                                           // CR.13870; Now uses SSILIB1_EXPORT to define linkage.
      const wchar_t *pwcsIn, 
      wchar_t *pwcsOut, 
      size_t stOutBufLen, 
      size_t &stOutStringLen); 
   SSILIB1_EXPORT bool XmlEscapeString(                                             // CR.21572; modified.
      const SpdStreamable::CSpdCString &wsUnescaped,
      SpdStreamable::CSpdCString &wsEscaped,
      bool bOnlySetOutputIfEscaped = false);                                        // CR.21572; added.
   SSILIB1_EXPORT void XmlUnescapeString(
      const SpdStreamable::CSpdCString &wsEscaped, 
      SpdStreamable::CSpdCString &wsUnescaped); 
   SSILIB1_EXPORT SpdStreamable::CSpdCString XmlEscapeString(
      const SpdStreamable::CSpdCString &wsUnescaped) ; 
   SSILIB1_EXPORT SpdStreamable::CSpdCString XmlUnescapeString(
      const SpdStreamable::CSpdCString &wsEscaped); 
   SSILIB1_EXPORT void GetWin32ErrorMessage(                                        // CR.13870; Now uses SSILIB1_EXPORT to define linkage.
      std::wstring &wsMsg, 
      DWORD dwNtErr); 
   SSILIB1_EXPORT void GetWin32ErrorMessage(                                        // CR.13870; Now uses SSILIB1_EXPORT to define linkage.
      wchar_t *pwcsMsg, 
      DWORD dwStringLen,
      DWORD dwNtErr); 
   SSILIB1_EXPORT SpdStreamable::CSpdCString GetWin32ErrorMessage(                  //CR.26999.Item55; added.
      DWORD dwNtErr);
   SSILIB1_EXPORT SpdStreamable::CSpdCString GetHRErrorMessage(                     //CR.26999.Item55; added.
      HRESULT hr);

   SSILIB1_EXPORT std::wstring tolower(                                             // CR.17124.Item46; added.                    
      const std::wstring &ws) ; 
   SSILIB1_EXPORT void ThrowHr(                                                     // CR.18557.V02; added.
      HRESULT hr, 
      const wchar_t *pwcsFunc,
      const wchar_t *pwcsFormat, 
      ...); 

	SSILIB1_EXPORT std::wstring __stdcall ConvertNonPrintableToHex(						// CR.33662.V01; added.
		const std::wstring& wsRawString);
} // end of namespace 


/*
 * class CSmartMutex - This class manages creating and destroying of a mutex object via its
 * constructors/destructor.
 *
 * Note: Special consideration. This class does not implement the typical copy constructor or
 * assignment operator. It presumes that the mutex handle should NOT be shared across object (because
 * once an object closes the handle, then it is invalid for all other objects). There fore, it makes sure
 * to use its own Mutex (assignment operator)and creates a new one if required (copy constructor).
 *
 * SLB 27.apr.2020 CR.29294; Created to help reduce handle leaks.
 * SLB 29.may.2020 CR.29392; Removed blocking on mutex in destructor. This could/is cause problems when an object
 * contains multiple mutexes and there is an order dependency on how they are acquired. If an object needs this kind of
 * functionality, then it must implement it in its own destructor.
 */
class CSmartMutex
{
   HANDLE m_h;

public:
   CSmartMutex(const CSpdCString wsName = L"") : m_h(CreateMutexW(NULL, FALSE, wsName.empty() ? NULL : wsName.c_str())) {}
   virtual ~CSmartMutex() { CloseHandle(m_h);  m_h = INVALID_HANDLE_VALUE; }        // CR.29392; modified.

   // NOTE: Copy constructor and assignment operator DO NOT copy HANDLE. These definitions 
   // allow for a containing class to use this object as a member and not have to explicitly
   // initialize or destroy it.
   // This is not compatible/problematic with a Named mutex.
   CSmartMutex(const CSmartMutex &c) : m_h(CreateMutex(NULL, FALSE, NULL)) {}
   CSmartMutex & operator = (const CSmartMutex &c) { return *this; }

   operator HANDLE () const { return m_h; }
};


/*
 * class CSmartEvent - This class manages creating and destroying of a event object via its
 * constructors/destructor.
 *
 * Note: Special consideration. This class does not implement the typical copy constructor or
 * assignment operator. It presumes that the mutex handle should NOT be shared across object (because
 * once an object closes the handle, then it is invalid for all other objects). There fore, it makes sure
 * to use its own Mutex (assignment operator)and creates a new one if required (copy constructor).
 *
 * SLB 27.apr.2020 CR.29294; Created to help reduce handle leaks.
 * SLB 01.jun.2020 CR.29294.V01; Added support for specifying reset mode (auto or manual) and initial state.
 * SLB 08.jun.2020 CR.29294.V02; removed default initializers from constructor. Force these to always be
 * specified... too many issues when defaults are accidentally used.
 */
class CSmartEvent
{
   HANDLE m_h;
   // CreateEvent() parms, Saved in case copy constructor is called.
   BOOL m_bManual;                                                                  // CR.29294.V01; added.
   BOOL m_bInitial;                                                                 // CR.29294.V01; added.

public:
   CSmartEvent(BOOL bManual, BOOL bInitial, CSpdCString wsName = L"") :             // CR.29294.V02; modified.
      m_h(CreateEventW(NULL, bManual, bInitial, wsName.empty() ? NULL : wsName.c_str())),
      m_bManual(bManual),                                                           // CR.29294.V01; added.
      m_bInitial(bInitial)                                                          // CR.29294.V01; added.
   {}
   virtual ~CSmartEvent() { CloseHandle(m_h); m_h = INVALID_HANDLE_VALUE; }
   // NOTE: Copy constructor and assignment operator DO NOT copy HANDLE. These definitions 
   // allow for a containing class to use this object as a member and not have to explicitly
   // initialize or destroy it.
   CSmartEvent(const CSmartEvent &c) :
      m_h(CreateEvent(NULL, c.m_bManual, c.m_bInitial, NULL)),                      // CR.29294.V01; modified.
      m_bManual(c.m_bManual),                                                       // CR.29294.V01; added.
      m_bInitial(c.m_bInitial)                                                      // CR.29294.V01; added.
   {}
   CSmartEvent & operator = (const CSmartEvent &c) { return *this; }

   operator HANDLE () const { return m_h; }
};

#endif