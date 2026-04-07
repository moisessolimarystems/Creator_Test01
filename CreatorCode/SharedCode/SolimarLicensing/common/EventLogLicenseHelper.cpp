#include "EventLogLicenseHelper.h"
//#include "..\common\LicAttribsCPP\Lic_GenericAttribs.h"									// CR.32662.V03; commented out.
#include "..\common\LicAttribsCPP\Lic_KeyAttribs.h"
#include "..\common\LicAttribsCPP\Lic_LicenseSystemAttribs.h"
#include "..\common\LicAttribsCPP\Lic_SystemInfoAttribs.h"
#include "..\common\LicAttribsCPP\Lic_UsageInfoAttribs.h"
#include "..\common\LicAttribsCPP\Sys_EventLogInfoAttribs.h"
#include "..\common\TimeHelper.h"	//For TimeHelper
#include "StringUtils.h"				//For WstringToString
#include <errno.h>

const int BUFFER_SIZE = 1024*64*2;

// SLB 11.jun.2024 CR.32662.V03; Modifications made to work with Attribs v4 code.
// These include setting/getting attribs members using the SetXXX()/GetXXX() methods
// vs. having access to members directly.
// Time members are now based on FILETIME (vs time_t). Changes made accordingly,
// CFILTIME is used to help. 
// No longer use BSTR as param. Use CSpdString instead (consistent with other attribs
// code).
HRESULT EventLogLicenseHelper::ReadEventLog(
	CSpdCString &wsEventLogListAttribs)																// CR.32662.V03; modified.
{
	//OutputDebugStringW(L"EventLogHelper::ReadEventLog() - Start");
	HANDLE hEventLog(NULL);
	HRESULT hr(S_OK);
	Sys_EventLogInfoAttribs sysEventLogInfo;

	// Open the Application event log.
	hEventLog = OpenEventLog(	NULL, // use local computer
										L"Application"); // source name

	if (hEventLog == NULL)
	{
		hr = HRESULT_FROM_WIN32(::GetLastError());
	}
	else	//Got Event Log Handle
	{
		EVENTLOGRECORD *pevlr;
		BYTE bBuffer[BUFFER_SIZE];
		DWORD dwRead, dwNeeded, dwThisRecord;

		pevlr = (EVENTLOGRECORD *)&bBuffer;

		// Get the record number of the oldest event log record.
		GetOldestEventLogRecord(hEventLog, &dwThisRecord);

		// Opening the event log positions the file pointer for this
		// handle at the end (newest item) of the log. Read the event log records
		// sequentially until the first record has been read.
		const int MAXSIZE = 100;
      int currentCount = 0; 
//wchar_t buf[1024];
		while (	::ReadEventLog(	hEventLog, // event log handle
										EVENTLOG_BACKWARDS_READ | // reads forward
										EVENTLOG_SEQUENTIAL_READ, // sequential read
										0, // ignored for sequential reads
										pevlr, // pointer to buffer
										BUFFER_SIZE, // size of buffer
										&dwRead, // number of bytes read
										&dwNeeded) // bytes in next record
					&& (currentCount < MAXSIZE)) 
		{
			
			while ((dwRead > 0) && (currentCount < MAXSIZE))
			{

				//Convert current Event Log Entry to a Licensing Event Entry
				Sys_EventLogInfoAttribs::Sys_EventLogEntriesInfoAttribs tmpEventLogEntryInfo;

//_snwprintf(buf, 
//			  sizeof(buf)/sizeof(wchar_t), 
//			  L"eventID: %d, index: %d, ClosingRecordNumber: %d, EventCategory: %d, ReservedFlags: %d, Reserved: %d", 
//			  pevlr->EventID, 
//			  pevlr->RecordNumber,
//			  pevlr->ClosingRecordNumber,
//			  pevlr->EventCategory,
//			  pevlr->ReservedFlags,
//			  pevlr->Reserved
//			  );
//
//OutputDebugStringW(buf);

				tmpEventLogEntryInfo.SeteventID(pevlr->EventID);								// CR.32662.V03; modified.
				tmpEventLogEntryInfo.SetinstanceId(pevlr->EventID);							// CR.32662.V03; modified.
				tmpEventLogEntryInfo.Setindex(pevlr->RecordNumber);							// CR.32662.V03; modified.

//_snwprintf(buf, 
//			  sizeof(buf)/sizeof(wchar_t), 
//			  L"pevlr->EventID: %d, tmpEventLogEntryInfo.eventID: %d, pevlr->RecordNumber: %d, tmpEventLogEntryInfo.index: %d", 
//			  pevlr->EventID, 
//			  (int)tmpEventLogEntryInfo.eventID, 
//			  pevlr->RecordNumber,
//			  (int)tmpEventLogEntryInfo.index
//			  );
//
//OutputDebugStringW(buf);

#if 1
				CFILETIME cftEventTime((time_t)pevlr->TimeGenerated);							// CR.32662.V03; use SPDE way to handle time.
				tmpEventLogEntryInfo.SettimeGenerated(cftEventTime);							// CR.32662.V03; use SPDE way to handle time.
#else
				time_t curTimeT = pevlr->TimeGenerated;
				VARIANT currentTimeVT = TimeHelper::TimeTToVariant(curTimeT, true);
				SYSTEMTIME currentSystime;
				VariantTimeToSystemTime(currentTimeVT.date, &currentSystime);
				wchar_t currentTimestamp[256];
				TimeHelper::SystemTimeToString(currentTimestamp, _countof(currentTimestamp), currentSystime);
				tmpEventLogEntryInfo.SettimeGenerated(currentSystime);
#endif		
				

				UINT uOffset = 0;
				size_t dataSize = 0;
				LPBYTE pTmpString = 0;

				// Message
				uOffset = pevlr->StringOffset;
				dataSize = pevlr->DataOffset - pevlr->StringOffset;
				// CR.FIX.18075 - Fixed an issue when event log message was empty (dataSize==0), uninitialized data was trying to 
				// be accessed, thus causing License Server to crash if uninitialized data was invalid data for StringUtils::WstringToString
				std::wstring wTmpString = L"";
				if (dataSize > 0)
				{
					pTmpString = (LPBYTE)GlobalAlloc(GPTR, dataSize * sizeof(BYTE));
					if(pTmpString != 0)
					{
						memcpy(pTmpString, (LPBYTE)pevlr + uOffset, dataSize);
						wTmpString = std::wstring((wchar_t*)pTmpString);
						GlobalFree(pTmpString);
						pTmpString = 0;
					}
				}
				// CR.FIX.13703.v2 - It is possible for binary data to be in the msg.  In those cases, can not 
				// get proper msg from event log.
				long errorNo = 0;
				StringUtils::WstringToStringErrorOnBinary(wTmpString, &errorNo);
				if(errorNo == EILSEQ)	//Cannot convert wide string to a multibyte character, assume binary data
					tmpEventLogEntryInfo.Setmessage(L"Can not retrieve Event Log Message");
				else if(!StringUtils::DetectMultiByteCharacters(wTmpString))
					tmpEventLogEntryInfo.Setmessage(SpdUtils::XmlEscapeString(SpdUtils::ConvertNonPrintableToHex(wTmpString)));
				else
					tmpEventLogEntryInfo.Setmessage(L"-");

				// Source
				uOffset	= sizeof(EVENTLOGRECORD);
				dataSize = (_MAX_PATH + 1);
				pTmpString = (LPBYTE)GlobalAlloc(GPTR, dataSize * sizeof(wchar_t));
				if(pTmpString != 0)
				{
					memcpy(pTmpString, (LPBYTE)pevlr + uOffset, dataSize);
					tmpEventLogEntryInfo.Setsource((wchar_t*)pTmpString);

					uOffset += (UINT) wcslen((wchar_t*)pTmpString) * sizeof(wchar_t);		// CR.32662; modified.
					GlobalFree(pTmpString);
					pTmpString = 0;
				}

				// Computer Name
				uOffset += sizeof(wchar_t);
				dataSize = (_MAX_PATH + 1);
				pTmpString = (LPBYTE)GlobalAlloc(GPTR, dataSize * sizeof(wchar_t));
				if(pTmpString != 0)
				{
					memcpy(pTmpString, (LPBYTE)pevlr + uOffset, dataSize);
					tmpEventLogEntryInfo.SetmachineName((wchar_t*)pTmpString);

					GlobalFree(pTmpString);
					pTmpString = 0;
				}


				// CR.13703 - getting the SID is causing an error when streaming to xml.  SID does us
				// to return to Solimar because we can't look it up.  Don't get it.
				// User SID
				//if(pevlr->UserSidLength > 0)
				//{
				//	uOffset = pevlr->UserSidOffset;
				//	dataSize = pevlr->UserSidLength;
				//	pTmpString = (LPBYTE)GlobalAlloc(GPTR, dataSize * sizeof(wchar_t));
				//	if(pTmpString != 0)
				//	{
				//		memcpy(pTmpString, (LPBYTE)pevlr + uOffset, dataSize);

				//		// For testing, this will put the SID in the value
				//		tmpEventLogEntryInfo.userName = std::wstring((wchar_t*)pTmpString);

				//		GlobalFree(pTmpString);
				//		pTmpString = 0;
				//	}
				//}

				/*strcpy(lpszComputerName, (LPTSTR)((LPBYTE)pELR + uOffset));
				dwComputerNameLen = strlen(lpszComputerName);*/


				#if 1
				cftEventTime = (time_t)pevlr->TimeWritten;										// CR.32662.V03; use SPDE way to handle time.
				tmpEventLogEntryInfo.SettimeWritten(cftEventTime);								// CR.32662.V03; use SPDE way to handle time.
				#else
				curTimeT = pevlr->TimeWritten;
				currentTimeVT = TimeHelper::TimeTToVariant(curTimeT, true);
				VariantTimeToSystemTime(currentTimeVT.date, &currentSystime);
				TimeHelper::SystemTimeToString(currentTimestamp, _countof(currentTimestamp), currentSystime);
				tmpEventLogEntryInfo.SettimeWritten(currentTimestamp);
				#endif
				tmpEventLogEntryInfo.SetcategoryNumber(pevlr->EventCategory);

				//tmpEventLogEntryInfo.category = std::wstring(GetMessageString(pevlr->EventCategory, 0, NULL));

				tmpEventLogEntryInfo.SetentryType(pevlr->EventType);
				sysEventLogInfo.entryList.push_back(tmpEventLogEntryInfo);
				currentCount++;
				
				//Get Next Record
				dwRead -= pevlr->Length;
				pevlr = (EVENTLOGRECORD *) ((LPBYTE) pevlr + pevlr->Length);
				
			}
			pevlr = (EVENTLOGRECORD *) &bBuffer;
		}
		CloseEventLog(hEventLog);
	}

	wsEventLogListAttribs = sysEventLogInfo.Serialize();										// CR.32662.V03; modified.
	return hr;
}
