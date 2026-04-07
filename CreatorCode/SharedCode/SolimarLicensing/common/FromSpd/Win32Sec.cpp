
// Win32Sec.cpp
//
// SLB 06.feb.2020 CASE.723895; Refactoring and exception handling changes for Win32Sec calls. 

#include "SpdException.h"                                                           // CASE.723895; modified.
#include "SpdUtils.h"                                                               // CASE.723895; added.
#include "Win32Sec.h"


namespace Win32Sec
{

   // WIN32_FROM_HRESULT()
   // Helper method to convert HRESULT back into Win32 error value.
   //
   // SLB 14.apr.2008 CR.9374; Added.
   // SLB 20.feb.2014 CR.17478; Moved out of class and made this a Win32Sec namespace
   // method.
   bool WIN32_FROM_HRESULT(HRESULT hr, DWORD *pdwWin32) 
   {
      if ((hr & 0xFFFF0000) == MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, 0)) 
      {
         // Could have come from many values, but we choose this one
         *pdwWin32 = HRESULT_CODE(hr);
         return true ;
      }
      if (hr == S_OK) 
      {
         *pdwWin32 = HRESULT_CODE(hr);
         return true ;
      }
      //
      // generate a debug warning message.
      CEnterExitMsg::TraceMsg(L"\n%s() Warning, failed to convert hr(%08X) to a win32 error code.", __WFUNCTION__, hr) ;

      //sol_assert(false) ; // SLB 25.apr.2008 - this does not compile in 2003 gui code.
      // otherwise, we got an impossible value
      return false;
   }

   //
   // Helper method to convert HRESULT back into Win32 error value.
   //
   // SLB 20.feb.2014 CR.17478; added.
   //
   DWORD WIN32_FROM_HRESULT(HRESULT hr) 
   {
      DWORD dwRes(0) ;

      WIN32_FROM_HRESULT(hr, &dwRes) ;
      return dwRes ;
   }
}

