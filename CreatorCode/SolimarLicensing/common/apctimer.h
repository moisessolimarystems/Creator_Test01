#ifndef __APCTIMER_H
#define __APCTIMER_H

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0403
#endif
#include <windows.h>

enum EInitReason { irStartup, irShutdown };

typedef void (*APCFunc)(void* pArg);
typedef bool (*APCInitFunc)(EInitReason reason, void* pArg);

class APCTimer {
public:
	APCTimer() {} // Hide default constructor.
	APCTimer(APCFunc pAPCFunc, void* pAPCArg, long msAPCInterval, APCInitFunc pAPCInitFunc = NULL, void* pAPCInitArg = NULL);
	~APCTimer();

	bool Initialize(EInitReason reason);

	void Invoke()	{ SetEvent(m_hTimerInvoke); }
	void Stop()		{ SetEvent(m_hTimerCancel); }

	void RevEnable(long revHighIntervalSecs, long revLowIntervalSecs, long revDownRatePercent);
	void RevUp();
	bool RevEnabled();

private:
	APCFunc		m_pAPCFunc;
	APCInitFunc	m_pAPCInitFunc;
	void*		m_pAPCArg;
	void*		m_pAPCInitArg;

	long		m_msAPCInterval;
	long		m_msAPCRevInterval;

	bool		m_bRevEnabled;
	long		m_msRevHighInterval;
	long		m_msRevLowInterval;
	long		m_revDownRatePercent;

	HANDLE		m_hTimer;
	HANDLE		m_hTimerThread;
	HANDLE		m_hTimerCancel;
	HANDLE		m_hTimerInvoke;
	HANDLE		m_hRevMutex;


	static unsigned long CALLBACK TimerThreadFunc(void *pParam);
	static VOID CALLBACK TimerAPCProc(
		LPVOID lpArg,               // Data value.
		DWORD dwTimerLowValue,      // Timer low value.
		DWORD dwTimerHighValue );	// Timer high value.

	void StartTimer();
	void StopTimer();
	BOOL SetTimer();
	bool WaitTimer();

	void RevDown();

	void APCInvoke() { (*m_pAPCFunc)(m_pAPCArg); }
};

#endif
