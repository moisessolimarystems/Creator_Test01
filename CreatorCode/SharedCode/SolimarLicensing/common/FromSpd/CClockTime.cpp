//
// CClockTime.cpp
//
#include "L_CClockTime.h"

/* GetTicksPerSecond() - returns units being used by QueryPerformanceCounter() in ticks/sec.
 *
 * Used to initialize static member of CClockTime.
 *
 * SLB 02.may.2019 CR.27141; created.
 ---------------------------------------------------------------------------------------------------------*/
QUADWORD GetTicksPerSecond()
{
   LARGE_INTEGER li;
   li.QuadPart = 1000;

   QueryPerformanceFrequency(&li);
   return li.QuadPart;
}

//
// Initialize statics.
//
const QUADWORD CClockTime::s_qwTicksPerSecond(GetTicksPerSecond());                 // CR.27141; added.

/* Clock() - static method.
 *
 * SLB 27.jul.2018 CR.25648; Created because in VS 2015+, clock() no longer works the same and is useless after the first
 * 32K ticks have occurred in the process.
 * SLB 02.may.2019 CR.27141; Fixed to take into account tick frequency on system/OS.
 ---------------------------------------------------------------------------------------------------------*/
QUADWORD CClockTime::Clock()
{
   LARGE_INTEGER liNow;
   BOOL bRES;
   bRES = QueryPerformanceCounter(&liNow);
   if (bRES == FALSE) throw (CClockTimeException(L"Problem retrieving Performance Counter.", __WFUNCTION__));
   //
   // QueryPerformanceCounter() returns value in ticks. Divide by TicksPerMillisecond() to return milli-seconds (traditional 
   // operation of the crt clock() function).
   //
   return (liNow.QuadPart/TicksPerMillisecond());                                   // CR.27141; modified.
}

/* TicksPerMillisecond() - static method.
 *
 * SLB 06.sep.2011 CR.27141; Added.
---------------------------------------------------------------------------------------------------------*/
QUADWORD CClockTime::TicksPerMillisecond()
{
   return s_qwTicksPerSecond / 1000;
}


/* SetValMilliSecs()
 *
 * SLB 06.sep.2011 CR.15133; Added.
---------------------------------------------------------------------------------------------------------*/
void CClockTime::SetValMilliSecs(DWORD dwMs) 
{
   m_qwTime = dwMs;
}


/* SetValSecs()
 *
 * SLB 06.sep.2011 CR.15133; Added.
---------------------------------------------------------------------------------------------------------*/
void CClockTime::SetValSecs(DWORD dwSec) 
{
   m_qwTime = dwSec * 1000;
}


/* SubtractWithNoUnderFlow()
 *
 * SLB 06.sep.2011 CR.15133; modified to throw a new exception type if saved time is greater than the over
 * flow value.
 * SLB 09.sep.2011 CR.15133.V01; Moved definition of this method out of header file and into source file.
 * SLB 24.oct.2011 CR.15133.V02; Make sure to cast clock_t var to DWORD before assigning it to a QUADWORD var.
 * SLB 17.feb.2017 CR.22592.V01; Updated to return a reference to this object.
---------------------------------------------------------------------------------------------------------*/
CClockTime & CClockTime::SubtractWithNoUnderFlow(const CClockTime &ct)              // CR.22592.V01; modified.
{  
   //
   // sub() requires that current value is greater than subtracting value.
   // 
   if (m_qwTime<ct.m_qwTime)
   {  
      //
      // not a problem: clock() has rolled over.
      // fix up current value by adding overflow value to it.
      //
      m_qwTime+=qwOverflowValue ;
   }
   sub(ct) ;
   //
   // After sub() operation, current value should be under the over flow value.
   // Verify that it is.
   //
   if (m_qwTime/qwOverflowValue!=0)                                                 // CR.15133; modified.
   {  
      //
      // problem: this should never occur.
      // initialize exception object.
      //
      CClockTimeException e ;
      e.m_qwStoredClockTime = m_qwTime ;
      e.m_qwCurrentClockTime = Clock() ;                                            // CR.15133.V02; modified.
      _snwprintf_s(
         e.m_wcsMsg, _TRUNCATE,
         L"CClockTime::SubtactWithNoUnderFlow() - saved time (%08I64X) is greater than over flow val. current time (%08I64X)",
         e.m_qwStoredClockTime,
         e.m_qwCurrentClockTime) ;
      throw(e) ; // throw the exception.
   }
   return *this ;                                                                   // CR.22592.V01; added.
}

/* AddWithNoOverFlow()
 *
 * SLB 06.sep.2011 CR.15133; After add() is complete, modified to throw out all bits above the exception value. 
 * this forces the saved time to "wrap".
 * SLB 09.sep.2011 CR.15133.V01; Moved definition of this method out of header file and into source file.
 * SLB 17.feb.2017 CR.22592.V01; Updated to return a reference to this object.
---------------------------------------------------------------------------------------------------------*/
CClockTime & CClockTime::AddWithNoOverFlow(const CClockTime &ct)                    // CR.22592.V01; modified.
{
   add(ct) ;
   //
   // After add(), it is possible that m_qwTime has gone past overflow value.
   // Ensure that its value gets "wrapped" around. This will match functionality of 
   // clock().
   // Note: this means that differences in clock_t greater than the overflow value (32bit,unsigned value)
   // can not be tracked. This should not be a problem as clock_t values are supposed to be
   // used to measure time in terms of milli-seconds or less. Overflow value does not happen 
   // until 49 days (roughly).
   //
   m_qwTime %= qwOverflowValue ;                                                    // CR.15133; added.
   return *this ;                                                                   // CR.22592.V01; added.
}

/* sub()
 *
 * SLB 06.sep.2011 CR.15133; Modified exception thrown when current time is less than time to be subtracted
 * (the under-flow condition).
 * SLB 09.sep.2011 CR.15133.V01; Moved definition of this method out of header file and into source file.
 * SLB 24.oct.2011 CR.15133.V02; Make sure to cast clock_t var to DWORD before assigning it to a QUADWORD var.
 ---------------------------------------------------------------------------------------------------------*/
void CClockTime::sub(QUADWORD qw)
{
   if (m_qwTime<qw)
   {  //
      // problem: this should never occur.
      // initialize exception object. CR.15133; added
      // CR.15133; added
      //
      CClockTimeException e ;
      e.m_qwStoredClockTime = m_qwTime ;
      e.m_qwCurrentClockTime = Clock() ;                                            // CR.15133.V02; modified.
      _snwprintf_s(
         e.m_wcsMsg, _TRUNCATE,
         L"CClockTime::sub1() - saved time (%08I64X) is less than comparison object's((%08I64X)",
         e.m_qwStoredClockTime,
         qw) ;
      throw(e) ; // throw the exception.
   }
   m_qwTime-=qw ;
}



