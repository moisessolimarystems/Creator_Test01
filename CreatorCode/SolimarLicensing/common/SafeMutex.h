#ifndef __SafeMutex_H
#define __SafeMutex_H

#include <windows.h>

class SafeMutex
{
public:
	SafeMutex(HANDLE mutexHandle);
	~SafeMutex();
private:
	HANDLE m_mutex;
};

#endif