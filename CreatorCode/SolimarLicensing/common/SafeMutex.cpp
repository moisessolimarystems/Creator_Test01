#include "SafeMutex.h"

SafeMutex::SafeMutex(HANDLE mutexHandle)
{
	m_mutex=mutexHandle;
	WaitForSingleObject(m_mutex, INFINITE);
}

SafeMutex::~SafeMutex()
{
	ReleaseMutex(m_mutex);
}
