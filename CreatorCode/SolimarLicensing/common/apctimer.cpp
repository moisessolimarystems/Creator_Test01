#include "apctimer.h"
void TRACE(LPCTSTR  pFormat, ...) {}
#include "SafeMutex.h"

//
// APCTimer()
//
//*******************************************************************
APCTimer::APCTimer(
	APCFunc		pAPCFunc, 
	void*		pAPCArg, 
	long		msAPCInterval,		// In milliseconds
	APCInitFunc pAPCInitFunc,		// Optional initialization func.
	void*		pAPCInitArg) 		// Optional initialization arg.
	:
	m_pAPCFunc(pAPCFunc),
	m_pAPCArg(pAPCArg),
	m_msAPCInterval(msAPCInterval),
	m_msAPCRevInterval(msAPCInterval),
	m_pAPCInitFunc(pAPCInitFunc),
	m_pAPCInitArg(pAPCInitArg),
	m_bRevEnabled(false),
	m_hRevMutex(NULL)
{ 
	m_hRevMutex	= CreateMutex(NULL, false, NULL);

	StartTimer(); 
}

//
// ~APCTimer()
//
//*******************************************************************
APCTimer::~APCTimer() 
{ 
	StopTimer(); 

	if (m_hRevMutex) 
		CloseHandle(m_hRevMutex);
}

// 
// Initialize()
// 
// If the constructor was passed an optional APCInit function,
// Initialize will call it and return the results, otherwise
// Initialize just returns true,  assuming no initialization is
// required. This optional APCInit function is a good place
// to call functions like CoInitialize, as the function is
// always called with the APCThread, not the thread that
// instantiated this class.
//
//*******************************************************************
bool APCTimer::Initialize(EInitReason reason)
{
	if (m_pAPCInitFunc) 
		return (*m_pAPCInitFunc)(reason, m_pAPCInitArg);
	else
		return true;
}

//
// RevEnabled()
//
//*******************************************************************
bool APCTimer::RevEnabled() 
{ 
	bool rv; 

	SafeMutex revBlock(m_hRevMutex);

	rv = m_bRevEnabled;

	return rv; 
}

//
// RevEnable()
//
//*******************************************************************
void APCTimer::RevEnable(long revHighIntervalSecs, long revLowIntervalSecs, long revDownRatePercent)
{
	{ // begin mutex scope.

	SafeMutex revBlock(m_hRevMutex);

	m_msRevHighInterval = revHighIntervalSecs * 1000;
	m_msRevLowInterval = revLowIntervalSecs * 1000;
	m_revDownRatePercent = revDownRatePercent;

	m_msAPCInterval = m_msRevHighInterval;
	m_msAPCRevInterval = m_msAPCInterval;

	m_bRevEnabled = true;

	} // end mutex scope.

	RevUp();
}

//
// RevUp()
//
//*******************************************************************
void APCTimer::RevUp() 
{ 
	{ // begin mutex scope.

	SafeMutex revBlock(m_hRevMutex);

	m_msAPCRevInterval = m_msAPCInterval;

	} // end mutex scope.

	SetTimer();
	//Invoke();  //Do not call invoke when RevUp is called
}

//
// RevDown()
//
//*******************************************************************
void APCTimer::RevDown() 
{ 
	{ // begin mutex scope.

	SafeMutex revBlock(m_hRevMutex);
	if (m_msAPCRevInterval < m_msRevLowInterval) 
	{
		m_msAPCRevInterval = m_msAPCRevInterval + (long)(float(m_revDownRatePercent) / float(100) * float(m_msAPCRevInterval));
		if (m_msAPCRevInterval > m_msRevLowInterval)
			m_msAPCRevInterval = m_msRevLowInterval;
	}

	} // end mutex scope.



	SetTimer();
}

//
// TimerThreadFunc()
//
// Static
//
//*******************************************************************
unsigned long CALLBACK APCTimer::TimerThreadFunc(void *pParam)
{
	int result = 1;

	APCTimer* pThis = static_cast<APCTimer*>(pParam);

	if (!pThis->Initialize(irStartup))
		return 0; // Startup failed.

	if (pThis->SetTimer()) {
		while (pThis->WaitTimer());
	}
	else {
		result = GetLastError();
		TRACE(TEXT("Error setting timer (%d)"), result); 
	}
	
	if (!pThis->Initialize(irShutdown))
		return 0; // Shutdown failed.

	return result;
}	

//
// TimerAPCProc()
//
// Static
//
//*******************************************************************
VOID CALLBACK APCTimer::TimerAPCProc(
	LPVOID lpArg,               // Data value.
	DWORD dwTimerLowValue,      // Timer low value.
	DWORD dwTimerHighValue )	// Timer high value.
{
	APCTimer* pThis = static_cast<APCTimer*>(lpArg);
	if (pThis->RevEnabled())
		pThis->RevDown();
	pThis->APCInvoke();
}

//
// StartTimer()
//
//*******************************************************************
void APCTimer::StartTimer()
{
	m_hTimer			= CreateWaitableTimer(NULL, false, NULL);
	m_hTimerCancel		= CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hTimerInvoke		= CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hTimerThread		= CreateThread(NULL, 0, TimerThreadFunc, this, 0, NULL);
}


//
// StopTimer()
//
//*******************************************************************
void APCTimer::StopTimer()
{
	CancelWaitableTimer(m_hTimer);
	SetEvent(m_hTimerCancel);
	WaitForSingleObject(m_hTimerThread, INFINITE);
	CloseHandle(m_hTimer);
	CloseHandle(m_hTimerThread);
	CloseHandle(m_hTimerCancel);
	CloseHandle(m_hTimerInvoke);
}

//
// SetTimer()
//
//*******************************************************************
BOOL APCTimer::SetTimer()
{
	__int64         qwDueTime;
	LARGE_INTEGER   liDueTime;

	//
	// Create a negative 64-bit integer that will be used to
	// signal the timer m_APCInterval nano-seconds from now.
	//

	qwDueTime = (__int64)-m_msAPCRevInterval * (__int64)10000; // 100 nano second intervals.

	//
	// Copy the relative time into a LARGE_INTEGER.
	//

	liDueTime.LowPart  = (DWORD) ( qwDueTime & 0xFFFFFFFF );
	liDueTime.HighPart = (LONG)  ( qwDueTime >> 32 );

	return SetWaitableTimer(
		m_hTimer,				// Handle to the timer object.
		&liDueTime,				// When timer will become signaled.
		m_msAPCRevInterval,		// Periodic timer interval.
		NULL,//		TimerAPCProc,			// Completion routine.
		this,					// Argument to the completion routine.
		false);					// Do not restore a suspended system.
}

//
// WaitTimer()
//
// Waits for either the timer to go off, or cancel.
// Returns false if cancel mutex has been signaled.
//
//*******************************************************************
bool APCTimer::WaitTimer() 
{
	HANDLE hArray[3];
	hArray[0] = m_hTimer;
	hArray[1] = m_hTimerCancel;
	hArray[2] = m_hTimerInvoke;
	DWORD retVal = WaitForMultipleObjectsEx(
		3, 
		hArray,
		false,		// Wait for any object to become signaled.
		INFINITE,
		true);		// IMPORTANT!!! The thread must be in an
					// alertable state to process the APC.
	
	//
	// Check if the cancel mutex has been signaled.
	//
	if (retVal - WAIT_OBJECT_0 == 1) 
		return false;
	
	ResetEvent(m_hTimerInvoke);

	TimerAPCProc(this, 0, 0);

	return true;
}
