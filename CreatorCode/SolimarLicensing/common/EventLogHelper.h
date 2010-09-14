#ifndef _Event_Log_Helper_406C35857AD74193875B222F371A5641_h__
#define _Event_Log_Helper_406C35857AD74193875B222F371A5641_h__

#include <comdef.h>
#include <tchar.h>
// vc6 = 1200, vc7 = 1300, vc7.1 = 1310, vc8 = 1400
#if _MSC_VER < 1400
#include <stdio.h>
#define swprintf_s _snwprintf
#endif

class EventLogHelper
{
public:
    static HRESULT WriteEventLog(wchar_t* event_source, wchar_t *event_log_msg, unsigned int event_type, long event_id);
    static HRESULT ReadEventLog_Helper();
	 static HRESULT ReadEventLog(BSTR *pBstrEventLogAttribsListStream);
//	 static LPWSTR GetMessageString(DWORD MessageId, DWORD argc, LPWSTR argv);
protected:
static DWORD DumpRecordsInBuffer(PBYTE pBuffer, DWORD dwBytesRead);
static DWORD GetEventTypeName(DWORD EventType);
static LPWSTR GetMessageString(DWORD Id, DWORD argc, LPWSTR args);
static void GetTimestamp(const DWORD Time, WCHAR DisplayString[]);
static DWORD ApplyParameterStringsToMessage(CONST LPCWSTR pMessage, LPWSTR & pFinalMessage);

};

#endif
