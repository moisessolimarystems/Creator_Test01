#include "SafeMutex.h"
#include <stdio.h>

SafeMutex::SafeMutex(HANDLE hMutex, DWORD dwTo)
{
   THandleList hlMutexList ;

   hlMutexList.push_back(hMutex) ;
   _init(hlMutexList, dwTo) ;
}
SafeMutex::SafeMutex(const THandleList &hlMutexList, DWORD dwTo)
{
   _init(hlMutexList, dwTo) ;
}
SafeMutex::~SafeMutex()
{
   TItHandleList ithMutex ;

   for (
      ithMutex = m_hlMutexList.begin() ;
      ithMutex != m_hlMutexList.end() ;
      ithMutex++
      )
   {
      HANDLE hMutex(*ithMutex) ;

      ReleaseMutex(hMutex) ;
   }
}

bool SafeMutex::Locked() 
{
   return m_bSuccess ; 
}

void SafeMutex::_init(const THandleList &hlMutexList, DWORD dwTo) 
{
   TItcHandleList ithMutex ;
   ULONG ulWfsoResult(WAIT_OBJECT_0) ;   
   //
   // initialize state members
   //
   m_bSuccess = false ;
   //
   // gain ownership of mutexi
   //
   for (
      ithMutex = hlMutexList.begin() ;
      ithMutex != hlMutexList.end() && ulWfsoResult==WAIT_OBJECT_0;
      ithMutex++
      )
   {
      HANDLE hMutex(*ithMutex) ;

      if (hMutex==INVALID_HANDLE_VALUE)
      {  //
         // problem: not a valid mutex handle.
         // Note,  WaitForSingleObject() does not treat INVALID_HANDLE_VALUE as invalid... 
         // it will wait for ever...  Hence catch it here.
         //
         assert(hMutex!=INVALID_HANDLE_VALUE) ;
      }
      else
      {  
         ulWfsoResult = WaitForSingleObject(hMutex, dwTo) ;
         if (ulWfsoResult==WAIT_FAILED)
         {  
            DWORD dwNtErr(ERROR_SUCCESS) ;
            //
            // problem: WaitForSingleObject() failed.
            //
            dwNtErr = GetLastError() ;
            assert(ulWfsoResult!=WAIT_FAILED) ;
         }
         else if (ulWfsoResult==WAIT_TIMEOUT)
         {  //
            // problem: did not get ownership of object.
            //
            break ;
         }
         else if (ulWfsoResult==WAIT_OBJECT_0)
         {  //
            // Successfully obtained ownership of the mutex.
            // Insert the mutex into the list that the destructor
            // will release.
            //
            m_hlMutexList.push_back(hMutex) ;
         }
      }
   }
   if (ithMutex == hlMutexList.end())
   {
      m_bSuccess = true ;
   }
}
