#ifndef __SafeMutex_H
#define __SafeMutex_H

// smart pointer class which handles mutex blocking/releasing.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vector>
//#include <assert.h>																					// CR.32662.V06; commented out.

// SLB 13.mar.2025 CR.32662.V06; Added definition of __WFUNCTION__ macro, to be 
// used in debug message macros mostly. Added here because this include file is
// ubiquitous through out the project.
#ifndef WIDEN
#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define __WFUNCTION__ WIDEN(__FUNCTION__)
#endif

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
   ~SafeMutex();
   bool Locked() const ;
   
private:
	// Protect default constructor from usage.
	SafeMutex() {}																							// CR.32662.V06; moved here.
   void _init(const THandleList &hlMutexList, DWORD dwTo=INFINITE);
} ;

#endif
