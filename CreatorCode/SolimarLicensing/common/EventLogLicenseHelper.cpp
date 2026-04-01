#include "EventLogLicenseHelper.h"
#include "..\common\LicAttribsCPP\Lic_GenericAttribs.h"
#include "..\common\LicAttribsCPP\Lic_KeyAttribs.h"
#include "..\common\LicAttribsCPP\Lic_LicenseSystemAttribs.h"
#include "..\common\LicAttribsCPP\Lic_SystemInfoAttribs.h"
#include "..\common\LicAttribsCPP\Lic_UsageInfoAttribs.h"
#include "..\common\LicAttribsCPP\Sys_EventLogInfoAttribs.h"
#include "..\common\TimeHelper.h"	//For TimeHelper
#include "StringUtils.h"				//For WstringToString
#include <errno.h>
const int BUFFER_SIZE = 1024*64*2;
HRESULT EventLogLicenseHelper::ReadEventLog(BSTR *pBstrEventLogAttribsListStream)
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

				tmpEventLogEntryInfo.eventID = pevlr->EventID;
				tmpEventLogEntryInfo.instanceId = pevlr->EventID;
				tmpEventLogEntryInfo.index = pevlr->RecordNumber;

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

				time_t curTimeT = pevlr->TimeGenerated;
				VARIANT currentTimeVT = TimeHelper::TimeTToVariant(curTimeT, true);
				SYSTEMTIME currentSystime;
				VariantTimeToSystemTime(currentTimeVT.date, &currentSystime);
				wchar_t currentTimestamp[256];
				TimeHelper::SystemTimeToString(currentTimestamp, _countof(currentTimestamp), currentSystime);
				tmpEventLogEntryInfo.timeGenerated = std::wstring(currentTimestamp);

				UINT uOffset = 0;
				size_t dataSize = 0;
				LPBYTE pTmpString = 0;

				// Message
				uOffset = pevlr->StringOffset;
				dataSize = pevlr->DataOffset - pevlr->StringOffset;
				pTmpString = (LPBYTE)GlobalAlloc(GPTR, dataSize * sizeof(BYTE));
				if(pTmpString != 0)
				{
					memcpy(pTmpString, (LPBYTE)pevlr + uOffset, dataSize);
					std::wstring wTmpString = std::wstring((wchar_t*)pTmpString);
					GlobalFree(pTmpString);
					pTmpString = 0;

					// CR.FIX.13703.v2 - It is possible for binary data to be in the msg.  In those cases, can not 
					//get proper msg from event log.
					long errorNo = 0;
					StringUtils::WstringToString(wTmpString, &errorNo);
					if(errorNo == EILSEQ)	//Cannot convert wide string to a multibyte character, assume binary data
						tmpEventLogEntryInfo.message = std::wstring(L"Can not retrieve Event Log Message");
					else
						tmpEventLogEntryInfo.message = SpdUtils::XmlEscapeString(SpdUtils::ConvertNonPrintableToHex(wTmpString));
				}


				// Source
				uOffset	= sizeof(EVENTLOGRECORD);
				dataSize = (_MAX_PATH + 1);
				pTmpString = (LPBYTE)GlobalAlloc(GPTR, dataSize * sizeof(wchar_t));
				if(pTmpString != 0)
				{
					memcpy(pTmpString, (LPBYTE)pevlr + uOffset, dataSize);
					tmpEventLogEntryInfo.source = std::wstring((wchar_t*)pTmpString);

					uOffset += wcslen((wchar_t*)pTmpString) * sizeof(wchar_t);
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
					tmpEventLogEntryInfo.machineName = std::wstring((wchar_t*)pTmpString);

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



				curTimeT = pevlr->TimeWritten;
				currentTimeVT = TimeHelper::TimeTToVariant(curTimeT, true);
				VariantTimeToSystemTime(currentTimeVT.date, &currentSystime);
				TimeHelper::SystemTimeToString(currentTimestamp, _countof(currentTimestamp), currentSystime);
				tmpEventLogEntryInfo.timeWritten = std::wstring(currentTimestamp);

				tmpEventLogEntryInfo.categoryNumber = pevlr->EventCategory;

				//tmpEventLogEntryInfo.category = std::wstring(GetMessageString(pevlr->EventCategory, 0, NULL));

				tmpEventLogEntryInfo.entryType = pevlr->EventType;
				sysEventLogInfo.entryList->push_back(tmpEventLogEntryInfo);
				currentCount++;
				
				//Get Next Record
				dwRead -= pevlr->Length;
				pevlr = (EVENTLOGRECORD *) ((LPBYTE) pevlr + pevlr->Length);
				
			}
			pevlr = (EVENTLOGRECORD *) &bBuffer;
		}
		CloseEventLog(hEventLog);
	}

	//OutputDebugStringW(L"EventLogHelper::ReadEventLog() - SysAllocString() - Start");
	*pBstrEventLogAttribsListStream = SysAllocString(sysEventLogInfo.ToString().c_str());
	//OutputDebugStringW(L"EventLogHelper::ReadEventLog() - SysAllocString() - End");
	//OutputDebugStringW(L"EventLogHelper::ReadEventLog() - End");
	return hr;
}
