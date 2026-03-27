#ifndef _Time_Helper_22D9F564EC584e29B699FD3C300D8042_h__
#define _Time_Helper_22D9F564EC584e29B699FD3C300D8042_h__

#include <windows.h>
#include <time.h>
#include <comdef.h>

class TimeHelper
{
public:
	static time_t VariantToTimeT(VARIANT &timestamp)
	{
		SYSTEMTIME systimestamp;
		VariantTimeToSystemTime(timestamp.dblVal, &systimestamp);
		tm tm_struct;
		memset(&tm_struct,0,sizeof(tm_struct));
		tm_struct.tm_sec = systimestamp.wSecond;
		tm_struct.tm_min = systimestamp.wMinute;
		tm_struct.tm_hour = systimestamp.wHour;
		tm_struct.tm_mday = systimestamp.wDay;
		tm_struct.tm_mon = systimestamp.wMonth - 1;
		tm_struct.tm_year = systimestamp.wYear - 1900;
		tm_struct.tm_wday = systimestamp.wDayOfWeek;
		return mktime(&tm_struct);
	}

	static VARIANT TimeTToVariant(time_t timestamp)
	{
		// convert the time_t in to a variant date
		VARIANT vtTimestamp;
		vtTimestamp.vt = VT_DATE;
		vtTimestamp.dblVal = 0.0;
		SYSTEMTIME systimestamp;
		memset(&systimestamp, 0, sizeof(systimestamp));
		tm * tm_struct = gmtime(&timestamp);
		systimestamp.wSecond = tm_struct->tm_sec;
		systimestamp.wMinute = tm_struct->tm_min;
		systimestamp.wHour = tm_struct->tm_hour;
		systimestamp.wDay = tm_struct->tm_mday;
		systimestamp.wMonth = tm_struct->tm_mon + 1;
		systimestamp.wYear = tm_struct->tm_year + 1900;
		systimestamp.wDayOfWeek = tm_struct->tm_wday;
		SystemTimeToVariantTime(&systimestamp, &vtTimestamp.dblVal);
		return vtTimestamp;
	}
};

#endif 
