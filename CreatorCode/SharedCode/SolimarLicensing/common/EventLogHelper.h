#pragma once																								// CR.32662.V03; modified.

#include <comdef.h>
#include <tchar.h>
#include "FromSpd\L_SpdStreamable4.h"																// CR.32662.V03; added.

using namespace SpdStreamable;																		// CR.32662.V03; added.

// CR.32662.V03; Must be using VS.2019+ to build now. Removed code to backward
// compatible with older versions of compiler.

class EventLogHelper
{
public:
    static HRESULT WriteEventLog(
		 const wchar_t* event_source, 																// CR.32662; modified. 
		 const wchar_t *event_log_msg, 																// CR.32662; modified. 
		 unsigned int event_type, 
		 long event_id);
    static HRESULT ReadEventLog_Helper();
	 static HRESULT ReadEventLog(
		 CSpdCString& wsEventLogAttribsListStream);
	 //	 static LPWSTR GetMessageString(DWORD MessageId, DWORD argc, LPWSTR argv);
protected:
	static DWORD DumpRecordsInBuffer(PBYTE pBuffer, DWORD dwBytesRead);
	static DWORD GetEventTypeName(DWORD EventType);
	static LPWSTR GetMessageString(DWORD Id, DWORD argc, LPWSTR args);
	static void GetTimestamp(const DWORD Time, WCHAR DisplayString[]);
	static DWORD ApplyParameterStringsToMessage(CONST LPCWSTR pMessage, LPWSTR & pFinalMessage);
};

