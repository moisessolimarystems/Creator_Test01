#pragma once																								// CR.32662.V03; modified.
#include "EventLogHelper.h"
class EventLogLicenseHelper : EventLogHelper
{
	public:
		static HRESULT ReadEventLog(
			CSpdCString &wsEventLogAttribsListStream);											// CR.32662.V03; modified.
};
