// CClockTime.h
#pragma once

#ifndef _CClockTime_h																					// CR.32662.V04; added.
#define _CClockTime_h																					// CR.32662.V04; added.

#include "L_SSILib1Dll.h"

#pragma pack(push, 8)                                                               // CR.30115; added.
#include <windows.h>
#include "L_quadword.h"
#include <time.h>
#include <string>

//
// class: CClockTimeException
//
// SLB 06.sep.2011; CR.15133; added.
//
class SSILIB1_EXPORT CClockTimeException
{
public:
   wchar_t m_wcsMsg[200] ;
   QUADWORD m_qwStoredClockTime ;
   QUADWORD m_qwCurrentClockTime ;
public:
   CClockTimeException() : 
      m_qwStoredClockTime(0),
      m_qwCurrentClockTime(0)
   {
      m_wcsMsg[0] = 0 ;
   }
   CClockTimeException(
      const wchar_t *pwcsMsg, 
      const wchar_t *pwcsFunc) 
      :
      m_qwStoredClockTime(0),
      m_qwCurrentClockTime(0)
   { 
      _snwprintf_s(m_wcsMsg, _TRUNCATE, L"%s() Internal error. %s",
         pwcsFunc,
         pwcsMsg);
   }
} ;

// SLB 16.feb.2009 CR.11099; I threw this class together to help manage time tracking and comparing with
// clock_t type. Fixes issue where clock() can return negative  numbers after about 26 days.
const QUADWORD qwOverflowValue(MAKEQUADWORD(0xFFFFFFFF, 0xEFFFFFFF));
/* 
 * Class: CClockTime
 *
 * SLB 09.sep.2011 CR.15133; Moved some function defintions into a source file. Source file is part of SSILIB1, so now
 * class may be exported. Also, added two new methods as well: SetValMilliSecs(), SetValSecs().
 * SLB 24.oct.2011 CR.15133.V02; Care must be taken when initializing a QUADWORD with clock_t. If clock_t is negative,
 * QUADWORD will end up with upper DWORD of 0xFFFFFFFF because the compiler is first casting clock_t to a 64 bit
 * signed integer then assigning this to the unsigned QUADWORD. To solve this problem, the solution used is to 
 * fisrt cast clock_t to a DWORD then assign this to the QUADWORD.
 * SLB 27.oct.2011 CR.14349; added operators for +() and -(). Used by CSpdDBSpoolCachedMonitor.cpp
 * SLB 17.feb.2017 CR.22592.V01; Now updated members SubtractWithNoUnderFlow() and AddWithNoOverFlow(). Both methods
 * now return a reference to this object.
 * SLB 27.jul.2018 CR.25648; Changed so that clock() is NEVER used, because in new C runtime (VS 2015+), once 32K clicks is reached, the return value stops 
 * incrementing. Now have implemented a workaround using the Win32 QueryPerformanceCounter() method (see definition for Now()).
 * Removed comparison operators and replaced with QUADWORD casting operator.
 * All clock_t methods are private so noone can accidently use clock() or clock_t to operate with this class.
 * SLB 02.may.2019 CR.27141; Modifications to adjust QueryPerformanceCounter() results with QueryPerformanceFrequency() info. 
 * SLB 18.jan.2023 CR.32082; changes in initialization of members by constructor to
 * eliminate compiler warning msg 26495.
 */
class CClockTime																							// CR.15133; modified.
{
   // Note: m_qwTime is the timestamp is milli-seconds.
   QUADWORD m_qwTime ;

   // Note: s_qwTicksPerSecond is used to convert QueryPerformanceCounter() result to determine time in milliseconds.
   static const QUADWORD s_qwTicksPerSecond;                                        // CR.27141; added.

public:
   static QUADWORD Clock();

   CClockTime() : m_qwTime(0) {}
   CClockTime(QUADWORD qwTime) : m_qwTime(qwTime) {}
   CClockTime(const CClockTime &cpy)
   {
      copy(cpy) ;
   }
   CClockTime & operator = (QUADWORD qw)
   { 
      copy(qw);
      return *this;
   }
   CClockTime operator + (const CClockTime &ct) const                               // CR.14349; added.
   {
      CClockTime ctRes(*this) ;
         
      ctRes.add(ct) ;
      return ctRes ;
   }
   CClockTime operator - (const CClockTime &ct) const                               // CR.14349; added.
   {
      CClockTime ctRes(*this) ;
      
      ctRes.sub(ct) ;
      return ctRes ;
   }
   CClockTime & operator = (const CClockTime &ctCpy)
   {
      copy(ctCpy) ;
      return *this ;
   }
   CClockTime & operator += (const CClockTime &ct)
   {
      add(ct) ;
      return *this ;
   }
   CClockTime & operator -= (const CClockTime &ct)
   {
      sub(ct) ;
      return *this ;
   }

#if 0
   // CR.
   bool operator < (const CClockTime &ct) const
   {
      if (compare(ct)<0)
         return true ;
      else 
         return false ;
   }
   bool operator <= (const CClockTime &ct) const
   {
      if (compare(ct)<=0)
         return true ;
      else 
         return false ;
   }
   bool operator == (const CClockTime &ct) const
   {
      if (compare(ct)==0)
         return true ;
      else 
         return false ;
   }
   bool operator > (const CClockTime &ct) const
   {
      if (compare(ct)>0)
         return true ;
      else 
         return false ;
   }
   bool operator >= (const CClockTime &ct) const
   {
      if (compare(ct)>=0)
         return true ;
      else 
         return false ;
   }
#else
   operator QUADWORD() const { return Val(); }
#endif
   CClockTime &Now() { m_qwTime = Clock(); return *this; }
   void Clear() { m_qwTime = 0; }
   QUADWORD Val() const { return m_qwTime; }
   QUADWORD MilliSecs() const { return Val(); }                                     // CR.15133; modified.
   QUADWORD Secs() const { return MilliSecs() / 1000 ; }                            // CR.15133; modified.
   void SetValMilliSecs(DWORD dwMs) ;                                               // CR.15133; added.
   void SetValSecs(DWORD dwSec) ;                                                   // CR.15133; added.
   CClockTime & SubtractWithNoUnderFlow(const CClockTime &ct) ;                     // CR.22592.V01; modified. CR.15133; modified.
   CClockTime & AddWithNoOverFlow(const CClockTime &ct) ;                           // CR.22592.V01; modified. CR.15133; modified.
   QUADWORD ElapsedMilliSecs() const                                                // CR.25648; added.
   {
      //
      // Don't allow returning what should be a negative value.
      //
      QUADWORD qwNow(Clock());
      if (m_qwTime > qwNow) throw (CClockTimeException(L"Internal error calculating elapsed time. CClockTime is in the future.", __WFUNCTION__));

      return qwNow-m_qwTime;
   }
   QUADWORD ElapsedSecs() const { return ElapsedMilliSecs()/1000; }                 // CR.25648; added.

private:
   static QUADWORD TicksPerMillisecond();                                           // CR.27141; added.

   void copy(const CClockTime &cpy)
   {
      copy(cpy.m_qwTime) ;
   }
   void copy(QUADWORD qw)
   {
      m_qwTime = qw;
   }
   void add(const CClockTime &ct)
   {
      add(ct.m_qwTime) ;
   }
   void add(QUADWORD qw)
   {
      m_qwTime += qw ;                                                              // CR.15133.V02; modified
   }
   void sub(const CClockTime &ct)                                                   // CR.15133; modified.
   {
      sub(ct.m_qwTime);
   }
   void sub(QUADWORD qw) ;                                                          // CR.15133; modified.
   int compare(const CClockTime &ctCmp) const ;                                     // CR.15133; modified.
   //
   // CR.25648; make all clock_t operations private so they cannot be used.
   //
   CClockTime(const clock_t &ctTime) : m_qwTime(0) { }                              // CR.32082; modified. CR.25648; modified.
   CClockTime & operator = (const clock_t &ct)  { return *this; }                   // CR.25648; modified.
   CClockTime operator + (clock_t ct) const { return *this; }                       // CR.25648; modified.
   CClockTime operator - (clock_t ct) const  { return *this; }                      // CR.25648; modified.
   CClockTime & operator += (clock_t ct)  { return *this; }                         // CR.25648; modified.
   CClockTime & operator -= (clock_t ct) { return *this; }                          // CR.25648; modified.
} ;

#pragma pack(pop)																							// CR.30115; added.

#endif