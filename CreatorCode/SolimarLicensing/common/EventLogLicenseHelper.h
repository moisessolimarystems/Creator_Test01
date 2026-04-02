#ifndef _Event_Log_License_Helper_067D701D6B0E4e63830FC470CF3562B4_h__
#define _Event_Log_License_Helper_067D701D6B0E4e63830FC470CF3562B4_h__

#include "EventLogHelper.h"

class EventLogLicenseHelper : EventLogHelper
{
	public:
		static HRESULT ReadEventLog(BSTR *pBstrEventLogAttribsListStream);
};
#endif