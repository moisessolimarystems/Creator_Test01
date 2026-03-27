
#include "EventLogHelper.h"

HRESULT EventLogHelper::WriteEventLog(wchar_t* event_source, wchar_t *event_log_msg, unsigned int event_type)
{
	HRESULT hr = S_OK;
    HANDLE h;
	
    h = RegisterEventSourceW(NULL, event_source);
    if (h == NULL) return HRESULT_FROM_WIN32(GetLastError());
 
    if (!ReportEventW(h,           // event log handle 
            (WORD)(event_type),   // event type 
            0,                    // category zero 
            0,					  // event identifier 
            NULL,                 // no user security identifier 
            1,                    // one substitution string 
            0,							// no data 
            (const wchar_t**)&event_log_msg,	// pointer to string array 
            NULL))                // pointer to data 
		hr = HRESULT_FROM_WIN32(GetLastError());
    
    DeregisterEventSource(h); 
	
	return hr;
}