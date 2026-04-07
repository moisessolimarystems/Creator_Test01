// CSpdBlock.h
// A smart pointer class which handles mutex blocking/releasing.
//

#pragma once

#ifndef _CSpdBlock_h																						// CR.32662.V04; added.
#define _CSpdBlock_h																						// CR.32662.V04; added.

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vector>
#include <assert.h>
#include <time.h>
#include <string>                                                                   // CR.30115; added.

#ifndef sol_assert
#define sol_assert(x) if (!x) { _ASSERTE(x) ; _CrtDbgBreak() ; }
#endif

#ifndef WIDEN
#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define __WFUNCTION__ WIDEN(__FUNCTION__)
#endif

/*
 * class CMutexLock
 *
 * A smart pointer class to obtain access to a mute, or list of mutex, and ensure it/they gets released
 * when this object goes out of scope. 
 *
 * Default behavior is to wait forever, however the caller may supply both a timeout and a context string.
 * The timeout is just passed directly to the WaitForSingleObject() call. If the timeout is specified, then 
 * the caller should ALWAYS use the Locked() method to determine if the mutex was obtained or not.
 *
 * The context string is useful for debugging purposes. It will be added to the Debug Messages generated when 
 * there is an error obtaining ownership of a mutex (note: a timeout is not treated as an error).
 *
 * The Locked() method can be used to ensure that the mutex was obtained successfully.
 * SLB 24.jan.2014 CR.17610; replaced THandleList definition. It was just a standard typedef std::vector<>,
 * Now it is a class, derived from std::vector.
 * SLB 16.mar.2015 CR.18725; Added new parameter to prevent assert errors when a bad handle is passed in. This is for
 * code which uses the Locked() method to determine, not only if a lock was achieved, but if the object is in fact 
 * valid (i.e. it know that the HANDLE may be invalid, so it expects this to occur). 
 * SLB 02.jul.2015 CR.19071; Found potential problem whereby, if some locks are successful, but one is not,
 * then the locks obtained will not get released until the object is deleted (usually when it goes out of scope).
 * Fixed so that if any lock fails, then all of the locks successfully obtained get released immediately.
 * SLB 27.sep.2017 CR.24527; Modified to handle single mutex case differently for performance reasons. Cumulative effect
 * of blocking on a single mutex multiple times was contributing to significant performance issues when constructing a list 
 * which contains a single mutex.
 * SLB 18.jan.2023 CR.32082; changes in "protected" constructors to eliminate 
 * compiler warning msg 26495.
 */
class CMutexLock
{
public:
   /*
    * class THandleList: This used to be just a straight forward type definition, but a contructor which takes 
    * a single HANDLE was needed.
    *
    * SLB 24.jan.2014 CR.17610; created.
    * SLB 18.jan.2023 CR.32082; changes in initialization of members by constructor to
    * eliminate compiler warning msg 26495.
    */
   typedef std::vector<HANDLE> THandleList;
   typedef THandleList::iterator TItHandleList ;
   typedef THandleList::const_iterator TItcHandleList ;

private:
   THandleList m_hlMutexList ;
   HANDLE m_hMutex;                                                                 // CR.24527; added.
   bool m_bSuccess ;
   const std::wstring m_wsContext ;
   DWORD m_dwNtErr;

public:
   CMutexLock(
      HANDLE hMutex, 
      DWORD dwTo = INFINITE, 
      const std::wstring &wsContext = L"",
      bool bUseBadHandleAssert = true                                               // CR.18725; added.
      ) :
      m_hMutex(INVALID_HANDLE_VALUE),                                               // CR.24527; added.
      m_wsContext(wsContext),
      m_dwNtErr(0), 
      m_bSuccess(false)                                                             // CR.32082; added.
   {
      // constructor for a single mutex.
      _init(hMutex, dwTo, bUseBadHandleAssert) ;                                    // CR.18725; modified.
   }
   CMutexLock(
      const THandleList &hlMutexList, 
      DWORD dwTo = INFINITE, 
      bool bUseBadHandleAssert = true) :                                            // CR.18725; added.
      m_hMutex(INVALID_HANDLE_VALUE),                                               // CR.24527; added.
      m_dwNtErr(0),                                                                 // CR.32082; added.
      m_bSuccess(false)                                                             // CR.32082; added.
   {  
      // constructor for a list of mutexes.
      _init(hlMutexList, dwTo, bUseBadHandleAssert) ;                               // CR.18725; modified.

      // Don't wait for destructor, if unsuccessful, make sure that all 
      // successfully locked mutexes are released...
      if (!Locked()) release() ;                                                    // CR.19071; added.
   }
   virtual ~CMutexLock()
   {
      release() ;                                                                   // CR.19071; added.
   }

   bool Locked() const
   {
      return m_bSuccess ; 
   }
   DWORD GetError() const
   {
      return m_dwNtErr;
   }
private:
   CMutexLock() : m_hMutex(0), m_bSuccess(false), m_dwNtErr(0)                      // CR.32082; modified.
   {  
      // prevent mis-use
      assert(false) ;
   }

   /* _init()
    *
    * SLB 04.mar.2009 CR.11092.V01; Modified so that the latest mutex owned gets inserted at the 
    * beginning of the saved list. Hence, it will get released before the other mutexi in the list.
    * This makes this behave in a LIFO fashion, which is optimal.
    * SLB 16.mar.2015 CR.18725; if bUseBadHandleAssert==false, no assert is thrown for a bad handle.
    * SLB 06.jul.2015 CR.19071; Restructured code a bit. Now checks result from WaitForXXX() using a 
    * switch() statement.
    * SLB 27.sep.2017 CR.24527; Moved mutex locking code from this method into new helper method 
    * ObtainMutex(). This just calls the new method now.
    *------------------------------------------------------------------------------------------*/
   void _init(
      const THandleList &hlMutexList, 
      DWORD dwTo, 
      bool bUseBadHandleAssert)                                                     // CR.18725; added.
   {
      // initialize state members
      m_bSuccess = false ;

      // gain ownership of mutexi
      for (TItcHandleList ithMutex(hlMutexList.begin()) ; ithMutex != hlMutexList.end() ; ithMutex++)
      {
         HANDLE hMutex(*ithMutex) ;
         bool bRes;
         bRes = ObtainMutex(hMutex, dwTo, bUseBadHandleAssert) ;                    // CR.24527; added.
         if (!bRes) return ;

         // ownership obtained, add mutex to the list to be released.
         m_hlMutexList.push_back(hMutex) ;
      }

      // Loop exited cleanly.
      m_bSuccess = true ;
   }

   /* _init() - Called by constructor, this form of _init() is sued to handle single mutex case. This 
    * performs significantly better than creating a temp list with a single handle and calling
    * the multi handle version of _init().
    *
    * SLB 27.sep.2017 CR.24527; Created to improve performance for single mutex handle case.
    *------------------------------------------------------------------------------------------*/
   void _init(
      HANDLE &hMutex, 
      DWORD dwTo, 
      bool bUseBadHandleAssert) 
   {
      m_bSuccess = ObtainMutex(hMutex, dwTo, bUseBadHandleAssert) ;
      if (m_bSuccess) m_hMutex = hMutex ;
   }

   /* ObtainMutex() - Obtains ownership of single mutex. Generates errors according to passed in parms.
    *
    * SLB 27.sep.2017 CR.24527; Created since there are two forms of _init() now.
    * SLB 02.may.2019 CR.27765; Throws hr_exception instead of throwing assert().
    *------------------------------------------------------------------------------------------*/
   bool ObtainMutex(HANDLE hMutex, DWORD dwTo, bool bUseBadHandleAssert) const
   {
      // gain ownership of mutexi
      wchar_t wcsMsg[256] ;                                                         // CR.11092.V01
      if (hMutex==INVALID_HANDLE_VALUE)
      {  
         if (!bUseBadHandleAssert) return false ;

         // problem: not a valid mutex handle.
         // Note,  WaitForSingleObject() does not treat INVALID_HANDLE_VALUE as invalid... 
         // it will wait for ever...  Hence catch it here.
#if _MSC_VER >=1400
         _snwprintf_s(                                                              // CR.11092.V01
            wcsMsg, _TRUNCATE,                                                      // CR.13870; modified.
            L"[%06u]SPDE - %s Invalid Mutex. Context(%s)",
            GetCurrentThreadId(),
            __WFUNCTION__,
            m_wsContext.c_str()) ;
         OutputDebugStringW(wcsMsg);
#endif
#ifdef _DEBUG
         throw (E_HANDLE);                                                          // CR.27765; modified.
#endif
         return false ;
      }

      ULONG ulWfsoResult ;   
      ulWfsoResult = WaitForSingleObject(hMutex, dwTo) ;
      DWORD m_dwNtErr(GetLastError()) ;
      switch (ulWfsoResult)
      {
      case WAIT_OBJECT_0:
         // Successfully obtained ownership of the mutex. return true!
         return true ;
      case WAIT_FAILED:
         // problem: WaitForSingleObject() failed. Generate debug message.
#if _MSC_VER >=1400
         _snwprintf_s(                                                              // CR.11092.V01
            wcsMsg, _TRUNCATE,                                                      // CR.13870; modified.
            L"[%06u] %s() WAIT_FAILED NT(%06u) Context(%s) hMutex(%08p)",           // CR.24131; modified.
            GetCurrentThreadId(),
            __WFUNCTION__,
            m_dwNtErr,
            m_wsContext.c_str(),
            hMutex) ;
         OutputDebugStringW(wcsMsg);
#endif
      }
      return false ;
   }

   /* release() - Releases all obtained sync objects.
    *
    * SLB 02.jul.2015 CR.19071; Created.
    * SLB 27.sep.2017 CR.24527; Modified to handle single mutex case. 
    *------------------------------------------------------------------------------------------*/
   void release()
   {
      // If single mutex case, release it and return.
      if (m_hMutex!=INVALID_HANDLE_VALUE) { ReleaseMutex(m_hMutex); return ; }      // CR.24527; added.

      // Managing a list of mutexes. Release them all.
      for (TItHandleList ithMutex(m_hlMutexList.begin()) ; ithMutex != m_hlMutexList.end() ; ithMutex++)
      {
         ReleaseMutex(*ithMutex) ;
      }
   }
} ;
typedef CMutexLock CSpdBlock; // DCM 19.jan.98 - Replaced SBlock with CMutexLock.

/*
 * class CSmartCriticalSection - This class manages creating and destroying of a CriticalSection object via its
 * constructors/destructor.
 *
 * Note: Special consideration. This class does not implement the typical copy constructor or
 * assignment operator. It presumes that the mutex handle should NOT be shared across object (because
 * once an object closes the handle, then it is invalid for all other objects). There fore, it makes sure
 * to use its own Mutex (assignment operator)and creates a new one if required (copy constructor).
 *
 * SLB 27.aug.2020 CR.29294.V04; Created.
 * SLB 18.jan.2023 CR.32082; changes in initialization of members by constructor to 
 * eliminate compiler warning msg 26495.
 */
class CSmartCriticalSection
{
   CRITICAL_SECTION m_cs;
   // for help trouble shooting.
   DWORD m_dwOwnerThreadId;                                                         // CR.30450; added.

public:
   CSmartCriticalSection() : m_dwOwnerThreadId(0) { InitializeCriticalSection(&m_cs); } // CR.32082; modified.
   virtual ~CSmartCriticalSection() { DeleteCriticalSection(&m_cs); }

   // NOTE: Copy constructor and assignment operator DO NOT copy the Critical Section. These definitions 
   // allow for a containing class to use this object as a member and not have to explicitly
   // initialize or destroy it.
   // This is not compatible/problematic with a Named mutex.
   CSmartCriticalSection(const CSmartCriticalSection &c) { InitializeCriticalSection(&m_cs); m_dwOwnerThreadId = 0; }
   CSmartCriticalSection & operator = (const CSmartCriticalSection &c) { return *this; }

   DWORD SetOwnerThreadId() { return m_dwOwnerThreadId = GetCurrentThreadId(); }    // CR.30450; added.
   void ResetOwnerThreadId() { m_dwOwnerThreadId = 0; }                             // CR.30450; added.

   operator const CRITICAL_SECTION &() const { return m_cs; }
   operator LPCRITICAL_SECTION () { return & m_cs; }
};
typedef CSmartCriticalSection CSpdSmartLock;


/*
 * class CCriticalSectionLock - This class manages Entering and Leaving a CriticalSection via its
 * constructors/destructor.
 * This has the equivalent functionality for CriticalSection objects that CMutexLock 
 * has for Mutex objects.
 *
 * SLB 08.aug.2020 CR.29294.V04; Removed handling of dead lock exception.
 * SLB 28.apr.2021 CR.30450; Moved here from SpdUtils.h. Has some minor enhancements.
 * SLB 18.jan.2023 CR.32082; changes in "protected" constructors to eliminate compiler warning msg 26495.
 */
class CCriticalSectionLock
{
private:

   CSmartCriticalSection & m_cs;                                                    // CR.30450; modified.
   bool m_bSuccess;

private:
   // copy constructor use is not allowed.
   CCriticalSectionLock(const CCriticalSectionLock &c) : m_cs(c.m_cs), m_bSuccess(false) {} // CR.32082; modified.

public:
   CCriticalSectionLock(const CSmartCriticalSection &cs) :
      m_bSuccess(false),
      m_cs((CSmartCriticalSection &)cs)
   {
      // Obtain ownership of Critical Section. Note: this can throw an exception
      // From MSDN:  Do not handle a possible deadlock exception; instead, debug the application.
      EnterCriticalSection((LPCRITICAL_SECTION)m_cs);
      m_cs.SetOwnerThreadId();                                                      // CR.30450; added.

      // Success. 
      m_bSuccess = true;
   }
   ~CCriticalSectionLock()
   {
      // If Ownership was previously aquired, release now.
      m_cs.ResetOwnerThreadId();                                                    // CR.30450; added.
      if (m_bSuccess) LeaveCriticalSection((LPCRITICAL_SECTION)m_cs);

      m_bSuccess = false;
   }
   bool Locked() const
   {
      return m_bSuccess;
   }
};
typedef CCriticalSectionLock CSpdLock;

#endif