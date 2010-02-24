#ifndef _Event_Log_Helper_406C35857AD74193875B222F371A5641_h__
#define _Event_Log_Helper_406C35857AD74193875B222F371A5641_h__

#include <comdef.h>
#include <tchar.h>

class EventLogHelper
{
public:
    static HRESULT WriteEventLog(wchar_t* event_source, wchar_t *event_log_msg, unsigned int event_type, long event_id);
    static HRESULT ReadEventLog_Helper();
//	 static LPWSTR GetMessageString(DWORD MessageId, DWORD argc, LPWSTR argv);
private:
static DWORD DumpRecordsInBuffer(PBYTE pBuffer, DWORD dwBytesRead);
static DWORD GetEventTypeName(DWORD EventType);
static LPWSTR GetMessageString(DWORD Id, DWORD argc, LPWSTR args);
static void GetTimestamp(const DWORD Time, WCHAR DisplayString[]);
static DWORD ApplyParameterStringsToMessage(CONST LPCWSTR pMessage, LPWSTR & pFinalMessage);

};

#endif
