#ifndef __SafeMutex_H
#define __SafeMutex_H

// smart pointer class which handles mutex blocking/releasing.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vector>
#include <assert.h>

class SafeMutex
{
public:
   typedef std::vector<HANDLE> THandleList ;
   typedef THandleList::iterator TItHandleList ;
   typedef THandleList::const_iterator TItcHandleList ;
private:
   THandleList m_hlMutexList ;
   bool m_bSuccess ;
public:
   SafeMutex(HANDLE hMutex, DWORD dwTo=INFINITE);
   SafeMutex(const THandleList &hlMutexList, DWORD dwTo=INFINITE);
   SafeMutex() ;
   ~SafeMutex();
   bool Locked() const ;
   
private:
   void _init(const THandleList &hlMutexList, DWORD dwTo=INFINITE);
} ;

#endif
