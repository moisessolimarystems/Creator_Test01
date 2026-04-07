#ifndef _Time_Helper_22D9F564EC584e29B699FD3C300D8042_h__
#define _Time_Helper_22D9F564EC584e29B699FD3C300D8042_h__

#include <windows.h>
#include <time.h>
#include <comdef.h>
#include "FromSpd\L_SpdStreamable4.h"																// CR.32662.V06; added.

using namespace SpdStreamable;																		// CR.32662.V06; added.


class TimeHelper
{
public:
	static const time_t ONE_HOUR_IN_SECONDS = (time_t)(60*60);
	static const time_t ONE_DAY_IN_SECONDS = (time_t)(24*ONE_HOUR_IN_SECONDS);
	static const time_t ONE_DAY_IN_HOURS = (time_t)(24);

	static time_t VariantToTimeT(VARIANT &timestamp, bool bConvertToLocalTime=false)
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
		return bConvertToLocalTime? mktime(&tm_struct) : _mkgmtime(&tm_struct);
	}

	static VARIANT TimeTToVariant(time_t timestamp, bool bConvertToLocalTime=false)
	{
		// convert the time_t in to a variant date
		VARIANT vtTimestamp;
		vtTimestamp.vt = VT_DATE;
		vtTimestamp.dblVal = 0.0;
		SYSTEMTIME systimestamp;
		memset(&systimestamp, 0, sizeof(systimestamp));
		tm tm_struct;
		if (bConvertToLocalTime)
			localtime_s(&tm_struct, &timestamp);
		else
			gmtime_s(&tm_struct, &timestamp);
		systimestamp.wSecond = tm_struct.tm_sec;
		systimestamp.wMinute = tm_struct.tm_min;
		systimestamp.wHour = tm_struct.tm_hour;
		systimestamp.wDay = tm_struct.tm_mday;
		systimestamp.wMonth = tm_struct.tm_mon + 1;
		systimestamp.wYear = tm_struct.tm_year + 1900;
		systimestamp.wDayOfWeek = tm_struct.tm_wday;
		SystemTimeToVariantTime(&systimestamp, &vtTimestamp.dblVal);
		return vtTimestamp;
	}

	//	Converts string in the Structure "YYYY-MM-DD HH-MM-SS-UUUU" where in order:
	//	Y - Year
	//	M - Month
	//	D - Day
	//	H - Hour
	//	M - Minute
	//	S - Second
	// U - Millisecond
	static bool StringToSystemTime(const wchar_t *pwcBuf, SYSTEMTIME &sysTmOutVal) 
	{
		bool bRetVal(true) ;
		wchar_t wcCpy[25] ;
		wchar_t *pwcNull(NULL) ;
		//
		// Make a working copy of the const input buffer.
		//
		wmemcpy(wcCpy, pwcBuf, 24) ;
		//
		// Set intermediate delimiters to NULL.
		//
		wcCpy[4] = wcCpy[7] = wcCpy[10] = wcCpy[13] = wcCpy[16] = wcCpy[19] = wcCpy[24] = 0 ;
		//
		// Now convert known fixed offsets to their appropriate values.
		//
		sysTmOutVal.wYear =     (WORD)_wcstoi64(wcCpy+0 , &pwcNull, 10) ;
		sysTmOutVal.wMonth =    (WORD)_wcstoi64(wcCpy+5 , &pwcNull, 10) ;
		sysTmOutVal.wDay =      (WORD)_wcstoi64(wcCpy+8 , &pwcNull, 10) ;
		sysTmOutVal.wHour =     (WORD)_wcstoi64(wcCpy+11, &pwcNull, 10) ;
		sysTmOutVal.wMinute =   (WORD)_wcstoi64(wcCpy+14, &pwcNull, 10) ;
		sysTmOutVal.wSecond =   (WORD)_wcstoi64(wcCpy+17, &pwcNull, 10) ;
		sysTmOutVal.wMilliseconds = (WORD)_wcstoi64(wcCpy+20, &pwcNull, 10) % 1000 ;
		sysTmOutVal.wDayOfWeek = 0 ;

		return bRetVal ;
	}

	/* TimeToString() - Overload which uses attribs time and string types.
	 * SLB note: I opted to add a flag for conversion to local time vs.
	 * changing actual time object to local time... This way all time 
	 * calculations will remain in UTC. 
	 *
	 * SLB 13.mar.2025 CR.32662.V06; Added.
	 --------------------------------------------------------------------------------*/ 
	static CSpdCString TimeToString(
		const CFILETIME& ft, 
		bool bShowMilliSeconds = true,
		bool bConvertToLocalTime = false)
	{
		SYSTEMTIME stTime;
		BOOL bRES;
		bRES = FileTimeToSystemTime(&ft, &stTime);
		if (bRES == FALSE) return L"Invalid time object.";

		return TimeToString(stTime, bShowMilliSeconds, bConvertToLocalTime);
	}

	//	Converts SYSTEMTIME to a string in the Structure "YYYY-MM-DD HH-MM-SS-UUUU" or "YYYY-MM-DD HH-MM-SS"
	//
	// SLB 13.mar.2025 CR.32662.V06; modified to return a string vs pass it a buffer. 
	// The return bool was never used either. Simplifies usage code considerably.
	static CSpdCString TimeToString(/*wchar_t *pwcBuf, size_t count,*/
		const SYSTEMTIME &stTime, 
		bool bShowMilliSeconds = true,
		bool bConvertToLocalTime = false)
	{
		SYSTEMTIME st(stTime);
		if (bConvertToLocalTime) GetLocalTime(&st);

		wchar_t wcsTimeBuf[32];
		if (bShowMilliSeconds)
		{
			_snwprintf_s(wcsTimeBuf, _TRUNCATE, L"%04d-%02d-%02d %02d:%02d:%02d.%04d", // CR.32662.V06; modified.
				st.wYear, 
				st.wMonth, 
				st.wDay, 
				st.wHour, 
				st.wMinute,
				st.wSecond, 
				st.wMilliseconds);
		}
		else
		{
			_snwprintf_s(wcsTimeBuf, _TRUNCATE, L"%04d-%02d-%02d %02d:%02d:%02d",		// CR.32662.V06; modified.
				st.wYear, 
				st.wMonth, 
				st.wDay, 
				st.wHour, 
				st.wMinute,
				st.wSecond);
		}
		return wcsTimeBuf;
	}
};

#endif 
