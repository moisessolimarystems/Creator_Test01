//----------------------------------------------------------------------------------------*
// filename - SpdException.h                                                              *
//                                                                                        *
//----------------------------------------------------------------------------------------*
#pragma once

#ifndef _SpdException_h																					// CR.32662.V04; added.
#define _SpdException_h																					// CR.32662.V04; added.

#include "L_CSpdBlock.h"
#include "L_CClockTime.h"                                                             // CR.25648; added.
//#include <windows.h>
#include <eh.h>
#include <exception>                                                                // needed for std::exception
#include <string>                                                                   // CR.17862; added.
#include <time.h>
#include <array>
#include <map>


// CR.30450; Handy macro to use when no exception should escape at all.
#define EAT_EXCEPTION(a) try { a; } catch(...) {}                                   // CR.30450; added.

namespace SpdException
{

#pragma warning(push)                                                               // CR.24131; added.
#pragma warning(disable: 4275)  /* non dll-interface class 'std::exception' used as base class */ // CR.24131; added.
   //
   // Note: SLB 23.jul.2017; for this case I believe it is ok to disable this warning becaue the base class
   // is an inline class.

   /*
    * class win32_exception
    *
    * SLB - Added these classes to help catch win32 exceptions.
    *
    * SLB 10.feb.2014 CR.17498.V01; Moved data member declarations to top of the class.
    */
   class SSILIB1_EXPORT win32_exception: public std::exception
   {
      friend class access_violation;
   public:
      typedef const void* Address; // OK on Win32 platform
   private:
      const char* mWhat;
      Address mWhere;
      unsigned mCode;
    public:
      static void install_handler();
      static void uninstall_handler();
      virtual const char* what() const { return mWhat; };
      Address where() const { return mWhere; };
      unsigned code() const { return mCode; };
   private:
      static void translate(unsigned code, EXCEPTION_POINTERS* info);
      static _se_translator_function s_old_handler; 
   protected:
      win32_exception(const EXCEPTION_RECORD& info);
   }; 
#pragma warning(pop)                                                                // CR.24131; added.

   /*
    * class access_violation
    *
    * SLB 10.feb.2014 CR.17498.V01; Moved data member declarations to top of the class.
    */
   class SSILIB1_EXPORT access_violation: public win32_exception                    // CR.24131; modified.
   {
   private:
      bool mIsWrite;
      Address mBadAddress;
   public:
      bool isWrite() const { return mIsWrite; };
      Address badAddress() const { return mBadAddress; };
   private:
      access_violation(const EXCEPTION_RECORD& info);
      friend void win32_exception::translate(unsigned code, EXCEPTION_POINTERS* info);
   }; 

   /*
    *
    * class hr_exception
    *
    * Class used to throw HR exceptions within SPDE engine code.
    *
    * SLB 05.feb.2014 CR.17498.V01; created.
    * SLB 06.mar.2014 CR.17478; Updated class constructor. 
    * SLB 27.mar.2014 CR.17829; Added ability to track if "message was displayed".
    * SLB 03.feb.2019 CR.27366; Add "W" to GetMessage() so that the method will be compatible with
    * the windows.h macro which adds the "W" or "A" based upon whether the project is a UNICODE or Multibyte target.
    * Note: Multibyte targets will need to explicitly use GetMessageW() while UNICODE targets can use GetMessage().
    */ 
   class SSILIB1_EXPORT hr_exception
   {
   private:
      HRESULT m_hr; 
      std::wstring m_wsMessage; 
      std::wstring m_wsMethod; 
      bool m_bMessageWasDisplayed;                                                  // CR.17829; added.
   public:
      hr_exception(
         HRESULT hr,
         const std::wstring &wsMessage,                                             // CR.17478; modified.
         const std::wstring &wsMethod);                                             // CR.17478; modified.
      hr_exception(
         const hr_exception &cpy); 
      hr_exception & operator = (const hr_exception &cpy); 
      HRESULT GetHr() const; 
      std::wstring GetMessageW() const;                                             // CR.27366; modified.
      std::wstring GetMethod() const; 
      bool WasMessageDisplayed() const;                                             // CR.17829; added.
      void SetMessageWasDisplayed(                                                  // CR.17829; added.
         bool bMessageWasDisplayed);   
   }; 


   /*
    *
    * class win_exception
    *
    * Class used to throw HR exceptions within SPDE engine code.
    *
    * SLB 05.feb.2014 CR.17498.V01; created.
    */ 
   class SSILIB1_EXPORT win_exception
   {
   private:
      DWORD m_dwNtErr; 
      std::wstring m_wsMessage; 
      std::wstring m_wsMethod; 
   public:
      win_exception(
         HRESULT hr,
         const std::wstring &wsMessage=L"",
         const std::wstring &wsMethod=L""); 
      win_exception(
         const win_exception &cpy); 
      win_exception & operator = (const win_exception &cpy); 
      DWORD GetNtErr() const; 
      std::wstring GetMessage() const; 
      std::wstring GetMethod() const; 
   }; 

} // end of namespace

/*
 * class CEnterExitMsg - A smart pointer class to help with debugging threading issues.
 *
 * SLB 20.may.2015 CR.19048; Moved here to be available to general code base.
 * SLB 02.jul.2015 CR.19071; Realized the m_hrRes was not getting initialized. Fixed.
 * SLB 24.feb.2016 CR.18485; Improving debug messages to help troubleshoot Security Manager. Messages will now indent at
 * each function entry level. 
 * Additionally, messages can be added outside of constructor/destructor context using TraceMsg()... This to will honor
 * the current indent level for the thread.
 * SLB 18.may.2017 CR.24219; Changed from using std::array<> to std::vector<> as array<> is not supported by 
 * VS.2008, which is what iCONVERT uses to build the SSI_LIB projects.
 */
class SSILIB1_EXPORT CEnterExitMsg
{ 
protected:
   CClockTime m_ctEntryTS; 
   std::wstring m_wsMsg;
   std::wstring m_wsMethodName;

   // m_bDestructorWasCalled: Derived class destructors set this to true if they
   // don't want the base class destructor to generate a message.
   bool m_bDestructorWasCalled;

   static HANDLE s_hMutex; 
   static std::map<DWORD, DWORD> s_ThreadEnterIndices;  
   static std::vector<std::wstring> s_SpaceStringTable;                             // CR.24219; modified.

public:
   std::wstring m_wsCodeBlockDescription; 
   HRESULT m_hrRes; 

protected:

   // defualt contstructor is protected. Only to be called by derived classes.
   CEnterExitMsg() :
      m_hrRes(E_FAIL),
      m_ctEntryTS(CClockTime::Clock()),
      m_bDestructorWasCalled(false)
   {}

public:
#pragma warning( push )                                                             // CR.32082.V00.01; added.
#pragma warning( disable:4793 )                                                     // CR.32082.V00.01; added.

   CEnterExitMsg(const wchar_t *pwcsMethod, const wchar_t* pwcsFormatString = NULL, ...) :
      m_wsMethodName(pwcsMethod),
      m_hrRes(E_FAIL),                                                               // CR.19071; added.
      m_ctEntryTS(CClockTime::Clock()),
      m_bDestructorWasCalled(false)
   {
      if (pwcsFormatString && pwcsFormatString[0]!=0)
      {
         wchar_t wcsMsg[2048];
         int nSize;
         // Format the message.
         va_list pArg;
         va_start(pArg, pwcsFormatString);
         nSize = _vsnwprintf_s(wcsMsg, _TRUNCATE, pwcsFormatString, pArg);
         va_end(pArg);

         if (nSize < 0) OutputDebugStringW(L"CEnterExitMsg::constructor() error occurred formatting string.");
         m_wsMsg = wcsMsg;
      }

      TraceMsg(L"%s(): %s ENTER", m_wsMethodName.c_str(), m_wsMsg.c_str());
      //
      // Update the space offset index, for this thread, in the static table.
      //
      IncrementThreadSpaceIndex(GetCurrentThreadId());
   }
   virtual ~CEnterExitMsg()
   {
      if (m_bDestructorWasCalled) return; 
      
      m_bDestructorWasCalled = true;

      // Update the space offset index, for this thread, in the static table.
      DecrementThreadSpaceIndex(GetCurrentThreadId()); 
      
      TraceMsg(L"%s(): %s EXIT(%llu ms)  result(%08X)", m_wsMethodName.c_str(), m_wsMsg.c_str(), CClockTime::Clock() - m_ctEntryTS, m_hrRes);
   }

   HRESULT SetHr(HRESULT hr = S_OK) { return m_hrRes = hr; }
   static void GetFormattedTraceMsg(
      const wchar_t *pwcsMsg,
      wchar_t *pwcsFormatted,
      size_t stFormattedBufLen);
#if 0
   static void TraceMsg(const wchar_t *pwcsMsg)
   {
      // Format message and output it.
      wchar_t wcsMsg[2048]; 
      GetFormattedTraceMsg(pwcsMsg, wcsMsg, 2048);
      OutputDebugStringW(wcsMsg); 
   }
#else
   static void TraceMsg(const wchar_t* pwcsFormatString, ...);
#endif
   static void IncrementThreadSpaceIndex(DWORD dwThreadId);
   static void DecrementThreadSpaceIndex(DWORD dwThreadId);
   static DWORD GetThreadSpaceIndex(DWORD dwThreadId);
#pragma warning ( pop )                                                             // CR.32082.V00.01; added.
}; 

#endif