#ifndef _Event_Log_Helper_406C35857AD74193875B222F371A5641_h__
#define _Event_Log_Helper_406C35857AD74193875B222F371A5641_h__

#include <comdef.h>
#include <tchar.h>

class EventLogHelper
{
public:
    static HRESULT WriteEventLog(wchar_t* event_source, wchar_t *event_log_msg, unsigned int event_type);
};

#endif
