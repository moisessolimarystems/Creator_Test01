
#include "EventLogHelper.h"


HRESULT EventLogHelper::WriteEventLog(
	const wchar_t* event_source,																		// CR.32662; modified. 
	const wchar_t *event_log_msg, 																	// CR.32662; modified. 
	unsigned int event_type, 
	long event_id)
{
	HRESULT hr = S_OK;
    HANDLE h;

    h = RegisterEventSourceW(NULL, event_source);
    if (h == NULL) return HRESULT_FROM_WIN32(GetLastError());

    if (!ReportEventW(h,           // event log handle 
            (WORD)(event_type),   // event type 
            0,                    // category zero 
            event_id,             // event identifier 
            NULL,                 // no user security identifier 
            1,                    // one substitution string 
            0,							// no data 
            (const wchar_t**)&event_log_msg,	// pointer to string array 
            NULL))                // pointer to data 
		hr = HRESULT_FROM_WIN32(GetLastError());
    
    DeregisterEventSource(h); 
	
	return hr;
}

#define MAX_RECORD_BUFFER_SIZE  0x10000  // 64K
#define MAX_TIMESTAMP_LEN       23 + 1   // mm/dd/yyyy hh:mm:ss.mmm
HRESULT EventLogHelper::ReadEventLog_Helper()
{
	return E_NOTIMPL;
	//HRESULT hr(E_FAIL);
	//HANDLE hEventLog(NULL);
	//DWORD status(ERROR_SUCCESS);
	//DWORD dwBytesToRead(0);
	//DWORD dwBytesRead(0);
	//DWORD dwMinimumBytesToRead(0);
	//PBYTE pBuffer(NULL);
	//PBYTE pTemp(NULL);
	//try
	//{
	//	hEventLog = OpenEventLog(NULL, TEXT("Solimar License Server"));
	//	if(hEventLog == NULL)
	//		throw HRESULT_FROM_WIN32(GetLastError());

	//	// Allocate an initial block of memory used to read event records. The number 
	//	// of records read into the buffer will vary depending on the size of each event.
	//	// The size of each event will vary based on the size of the user-defined
	//	// data included with each event, the number and length of insertion 
	//	// strings, and other data appended to the end of the event record.
	//	dwBytesToRead = MAX_RECORD_BUFFER_SIZE;
	//	pBuffer = (PBYTE)malloc(dwBytesToRead);
	//	if (NULL == pBuffer)
	//		throw HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);

	//	// Read blocks of records until you reach the end of the log or an 
	//	// error occurs. The records are read from newest to oldest. If the buffer
	//	// is not big enough to hold a complete event record, reallocate the buffer.
	//	while (ERROR_SUCCESS == status)
	//	{
	//		if (!ReadEventLogW(hEventLog, 
	//			EVENTLOG_SEQUENTIAL_READ | EVENTLOG_BACKWARDS_READ,
	//			0, 
	//			pBuffer,
	//			dwBytesToRead,
	//			&dwBytesRead,
	//			&dwMinimumBytesToRead))
	//		{
	//			status = GetLastError();
	//			if (ERROR_INSUFFICIENT_BUFFER == status)
	//			{
	//				status = ERROR_SUCCESS;

	//				pTemp = (PBYTE)realloc(pBuffer, dwMinimumBytesToRead);
	//				if (NULL == pTemp)
	//					throw HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);

	//				pBuffer = pTemp;
	//				dwBytesToRead = dwMinimumBytesToRead;
	//			}
	//			else 
	//			{
	//				if (ERROR_HANDLE_EOF != status)
	//				{
	//					//wprintf(L"ReadEventLog failed with %lu.\n", status);
	//					//goto cleanup;
	//					throw E_FAIL;
	//				}
	//			}
	//		}
	//		else
	//		{
	//			// Print the contents of each record in the buffer.
	//			DumpRecordsInBuffer(pBuffer, dwBytesRead);
	//			//Success case
	//			int x=0;
	//			x++;
	//			x++;
	//			//x = x+dwBytesToRead;
	//			//GetMessageString(
	//		}
	//	}
	//}
	//catch(HRESULT &eHr)
	//{
	//	hr = eHr;
	//}
	//if (hEventLog)
	//	CloseEventLog(hEventLog);
	//if (pBuffer)
	//	free(pBuffer);
	//return hr;
}

//const int BUFFER_SIZE = 1024*64;
HRESULT EventLogHelper::ReadEventLog(CSpdCString &ws)
{
	return E_NOTIMPL;
	//OutputDebugStringW(L"EventLogHelper::ReadEventLog() - Start");
//	HANDLE hEventLog(NULL);
//	HRESULT hr(S_OK);
//	Sys_EventLogInfoAttribs sysEventLogInfo;
//
//	// Open the Application event log.
//	hEventLog = OpenEventLog(	NULL, // use local computer
//										L"Application"); // source name
//
//	if (hEventLog == NULL)
//	{
//		hr = HRESULT_FROM_WIN32(::GetLastError());
//	}
//	else	//Got Event Log Handle
//	{
//		EVENTLOGRECORD *pevlr;
//		BYTE bBuffer[BUFFER_SIZE];
//		DWORD dwRead, dwNeeded, dwThisRecord;
//
//		pevlr = (EVENTLOGRECORD *)&bBuffer;
//
//		// Get the record number of the oldest event log record.
//		GetOldestEventLogRecord(hEventLog, &dwThisRecord);
//
//		// Opening the event log positions the file pointer for this
//		// handle at the end (newest item) of the log. Read the event log records
//		// sequentially until the first record has been read.
//		const int MAXSIZE = 100;
//      int currentCount = 0; 
////wchar_t buf[1024];
//		while (	::ReadEventLog(	hEventLog, // event log handle
//										EVENTLOG_BACKWARDS_READ | // reads forward
//										EVENTLOG_SEQUENTIAL_READ, // sequential read
//										0, // ignored for sequential reads
//										pevlr, // pointer to buffer
//										BUFFER_SIZE, // size of buffer
//										&dwRead, // number of bytes read
//										&dwNeeded) // bytes in next record
//					&& (currentCount < MAXSIZE)) 
//		{
//			
//			while ((dwRead > 0) && (currentCount < MAXSIZE))
//			{
//
//				//Convert current Event Log Entry to a Licensing Event Entry
//				Sys_EventLogInfoAttribs::Sys_EventLogEntriesInfoAttribs tmpEventLogEntryInfo;
//
////_snwprintf(buf, 
////			  sizeof(buf)/sizeof(wchar_t), 
////			  L"eventID: %d, index: %d, ClosingRecordNumber: %d, EventCategory: %d, ReservedFlags: %d, Reserved: %d", 
////			  pevlr->EventID, 
////			  pevlr->RecordNumber,
////			  pevlr->ClosingRecordNumber,
////			  pevlr->EventCategory,
////			  pevlr->ReservedFlags,
////			  pevlr->Reserved
////			  );
////
////OutputDebugStringW(buf);
//
//				tmpEventLogEntryInfo.eventID = pevlr->EventID;
//				tmpEventLogEntryInfo.instanceId = pevlr->EventID;
//				tmpEventLogEntryInfo.index = pevlr->RecordNumber;
//
////_snwprintf(buf, 
////			  sizeof(buf)/sizeof(wchar_t), 
////			  L"pevlr->EventID: %d, tmpEventLogEntryInfo.eventID: %d, pevlr->RecordNumber: %d, tmpEventLogEntryInfo.index: %d", 
////			  pevlr->EventID, 
////			  (int)tmpEventLogEntryInfo.eventID, 
////			  pevlr->RecordNumber,
////			  (int)tmpEventLogEntryInfo.index
////			  );
////
////OutputDebugStringW(buf);
//
//				time_t curTimeT = pevlr->TimeGenerated;
//				VARIANT currentTimeVT = TimeHelper::TimeTToVariant(curTimeT, true);
//				SYSTEMTIME currentSystime;
//				VariantTimeToSystemTime(currentTimeVT.date, &currentSystime);
//				wchar_t currentTimestamp[256];
//				TimeHelper::SystemTimeToString(currentTimestamp, _countof(currentTimestamp), currentSystime);
//				tmpEventLogEntryInfo.timeGenerated = std::wstring(currentTimestamp);
//
//				UINT uOffset = 0;
//				size_t dataSize = 0;
//				LPBYTE pTmpString = 0;
//
//				// Message
//				uOffset = pevlr->StringOffset;
//				dataSize = pevlr->DataOffset - pevlr->StringOffset;
//				pTmpString = (LPBYTE)GlobalAlloc(GPTR, dataSize * sizeof(BYTE));
//				if(pTmpString != 0)
//				{
//					memcpy(pTmpString, (LPBYTE)pevlr + uOffset, dataSize);
//					tmpEventLogEntryInfo.message = std::wstring((wchar_t*)pTmpString);
//					GlobalFree(pTmpString);
//					pTmpString = 0;
//				}
//
//
//				// Source
//				uOffset	= sizeof(EVENTLOGRECORD);
//				dataSize = (_MAX_PATH + 1);
//				pTmpString = (LPBYTE)GlobalAlloc(GPTR, dataSize * sizeof(wchar_t));
//				if(pTmpString != 0)
//				{
//					memcpy(pTmpString, (LPBYTE)pevlr + uOffset, dataSize);
//					tmpEventLogEntryInfo.source = std::wstring((wchar_t*)pTmpString);
//
//					uOffset += wcslen((wchar_t*)pTmpString) * sizeof(wchar_t);
//					GlobalFree(pTmpString);
//					pTmpString = 0;
//				}
//
//				// Computer Name
//				uOffset += sizeof(wchar_t);
//				dataSize = (_MAX_PATH + 1);
//				pTmpString = (LPBYTE)GlobalAlloc(GPTR, dataSize * sizeof(wchar_t));
//				if(pTmpString != 0)
//				{
//					memcpy(pTmpString, (LPBYTE)pevlr + uOffset, dataSize);
//					tmpEventLogEntryInfo.machineName = std::wstring((wchar_t*)pTmpString);
//
//					GlobalFree(pTmpString);
//					pTmpString = 0;
//				}
//
//	
//				// User SID
//				if(pevlr->UserSidLength > 0)
//				{
//					uOffset = pevlr->UserSidOffset;
//					dataSize = pevlr->UserSidLength;
//					pTmpString = (LPBYTE)GlobalAlloc(GPTR, dataSize * sizeof(wchar_t));
//					if(pTmpString != 0)
//					{
//						memcpy(pTmpString, (LPBYTE)pevlr + uOffset, dataSize);
//
//						// For testing, this will put the SID in the value
//						tmpEventLogEntryInfo.userName = std::wstring((wchar_t*)pTmpString);
//
//						GlobalFree(pTmpString);
//						pTmpString = 0;
//					}
//				}
//
//				/*strcpy(lpszComputerName, (LPTSTR)((LPBYTE)pELR + uOffset));
//				dwComputerNameLen = strlen(lpszComputerName);*/
//
//
//
//				curTimeT = pevlr->TimeWritten;
//				currentTimeVT = TimeHelper::TimeTToVariant(curTimeT, true);
//				VariantTimeToSystemTime(currentTimeVT.date, &currentSystime);
//				TimeHelper::SystemTimeToString(currentTimestamp, _countof(currentTimestamp), currentSystime);
//				tmpEventLogEntryInfo.timeWritten = std::wstring(currentTimestamp);
//
//				tmpEventLogEntryInfo.categoryNumber = pevlr->EventCategory;
//
//				//tmpEventLogEntryInfo.category = std::wstring(GetMessageString(pevlr->EventCategory, 0, NULL));
//
//				tmpEventLogEntryInfo.entryType = pevlr->EventType;
//				sysEventLogInfo.entryList->push_back(tmpEventLogEntryInfo);
//				currentCount++;
//				
//				//Get Next Record
//				dwRead -= pevlr->Length;
//				pevlr = (EVENTLOGRECORD *) ((LPBYTE) pevlr + pevlr->Length);
//				
//			}
//			pevlr = (EVENTLOGRECORD *) &bBuffer;
//		}
//		CloseEventLog(hEventLog);
//	}
//
//	//OutputDebugStringW(L"EventLogHelper::ReadEventLog() - SysAllocString() - Start");
//	*pBstrEventLogAttribsListStream = SysAllocString(sysEventLogInfo.ToString().c_str());
//	//OutputDebugStringW(L"EventLogHelper::ReadEventLog() - SysAllocString() - End");
//	//OutputDebugStringW(L"EventLogHelper::ReadEventLog() - End");
//	return hr;
}

// Loop through the buffer and print the contents of each record 
	// in the buffer.
// Loop through the buffer and print the contents of each record 
// in the buffer.
DWORD EventLogHelper::DumpRecordsInBuffer(PBYTE pBuffer, DWORD dwBytesRead)
{
    DWORD status = ERROR_SUCCESS;
//    PBYTE pRecord = pBuffer;
//    PBYTE pEndOfRecords = pBuffer + dwBytesRead;
//    LPWSTR pMessage = NULL;
//    LPWSTR pFinalMessage = NULL;
//    WCHAR TimeStamp[MAX_TIMESTAMP_LEN];
//wchar_t tmpbuf[1024];
//    while (pRecord < pEndOfRecords)
//    {
//        // If the event was written by our provider, write the contents of the event.
//        if (0 == wcscmp(TEXT("Solimar License Server"), (LPWSTR)(pRecord + sizeof(EVENTLOGRECORD))))
//        {
//
//
//            GetTimestamp(((PEVENTLOGRECORD)pRecord)->TimeGenerated, TimeStamp);
//            wprintf(L"Time stamp: %s\n", TimeStamp);
//            wprintf(L"record number: %lu\n", ((PEVENTLOGRECORD)pRecord)->RecordNumber);
//            wprintf(L"status code: %d\n", ((PEVENTLOGRECORD)pRecord)->EventID & 0xFFFF);
//            //wprintf(L"event type: %s\n", pEventTypeNames[GetEventTypeName(((PEVENTLOGRECORD)pRecord)->EventType)]);
//OutputDebugString(L"--------------------------------------------"); 
//swprintf_s(tmpbuf, _countof(tmpbuf), L"Time stamp: %s", TimeStamp);
//OutputDebugString(tmpbuf); 
//swprintf_s(tmpbuf, _countof(tmpbuf), L"record number: %lu", ((PEVENTLOGRECORD)pRecord)->RecordNumber);
//OutputDebugString(tmpbuf); 
//swprintf_s(tmpbuf, _countof(tmpbuf), L"status code: %d", ((PEVENTLOGRECORD)pRecord)->EventID & 0xFFFF);
//OutputDebugString(tmpbuf); 
//
//            pMessage = GetMessageString(((PEVENTLOGRECORD)pRecord)->EventCategory, 0, NULL);
//
//            if (pMessage)
//            {
//                wprintf(L"event category: %s", pMessage);
//swprintf_s(tmpbuf, _countof(tmpbuf), L"event category: %s", pMessage);
//OutputDebugString(tmpbuf); 
//                LocalFree(pMessage);
//                pMessage = NULL;
//            }
//
//            pMessage = GetMessageString(((PEVENTLOGRECORD)pRecord)->EventID, 
//                ((PEVENTLOGRECORD)pRecord)->NumStrings, (LPWSTR)(pRecord + ((PEVENTLOGRECORD)pRecord)->StringOffset));
//
//            if (pMessage)
//            {
//                status = ApplyParameterStringsToMessage(pMessage, pFinalMessage);
//
//                wprintf(L"event message: %s", (pFinalMessage) ? pFinalMessage : pMessage);
//swprintf_s(tmpbuf, _countof(tmpbuf), L"event message: %s", (pFinalMessage) ? pFinalMessage : pMessage);
//OutputDebugString(tmpbuf); 
//                LocalFree(pMessage);
//                pMessage = NULL;
//
//                if (pFinalMessage)
//                {
//                    free(pFinalMessage);
//                    pFinalMessage = NULL;
//                }
//            }
//
//
//            // To write the event data, you need to know the format of the data. In
//            // this example, we know that the event data is a null-terminated string.
//            if (((PEVENTLOGRECORD)pRecord)->DataLength > 0)
//            {
//                wprintf(L"event data: %s\n", (LPWSTR)(pRecord + ((PEVENTLOGRECORD)pRecord)->DataOffset));
//swprintf_s(tmpbuf, _countof(tmpbuf), L"event data: %s", (LPWSTR)(pRecord + ((PEVENTLOGRECORD)pRecord)->DataOffset));
//OutputDebugString(tmpbuf); 
//            }
//
//            wprintf(L"\n");
//        }
//
//        pRecord += ((PEVENTLOGRECORD)pRecord)->Length;
//    }

    return status;
}
//
//
//
//// Get an index value to the pEventTypeNames array based on 
//// the event type value.
DWORD EventLogHelper::GetEventTypeName(DWORD EventType)
{
    DWORD index = 0;

    switch (EventType)
    {
        case EVENTLOG_ERROR_TYPE:
            index = 0;
            break;
        case EVENTLOG_WARNING_TYPE:
            index = 1;
            break;
        case EVENTLOG_INFORMATION_TYPE:
            index = 2;
            break;
        case EVENTLOG_AUDIT_SUCCESS:
            index = 3;
            break;
        case EVENTLOG_AUDIT_FAILURE:
            index = 4;
            break;
    }

    return index;
}



// Formats the specified message. If the message uses inserts, build
// the argument list to pass to FormatMessage.
LPWSTR EventLogHelper::GetMessageString(DWORD MessageId, DWORD argc, LPWSTR argv)
{
    LPWSTR pMessage = NULL;
//    DWORD dwFormatFlags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ALLOCATE_BUFFER;
//    DWORD_PTR* pArgs = NULL;
//    LPWSTR pString = argv;
//wchar_t tmpbuf[1024];
//    // The insertion strings appended to the end of the event record
//    // are an array of strings; however, FormatMessage requires
//    // an array of addresses. Create an array of DWORD_PTRs based on
//    // the count of strings. Assign the address of each string
//    // to an element in the array (maintaining the same order).
//    if (argc > 0)
//    {
//        pArgs = (DWORD_PTR*)malloc(sizeof(DWORD_PTR) * argc);
//        if (pArgs)
//        {
//            dwFormatFlags |= FORMAT_MESSAGE_ARGUMENT_ARRAY;
//
//            for (DWORD i = 0; i < argc; i++)
//            {
//                pArgs[i] = (DWORD_PTR)pString;
//                pString += wcslen(pString) + 1;
//            }
//        }
//        else
//        {
//            dwFormatFlags |= FORMAT_MESSAGE_IGNORE_INSERTS;
//            wprintf(L"Failed to allocate memory for the insert string array.\n");
//swprintf_s(tmpbuf, _countof(tmpbuf), L"Failed to allocate memory for the insert string array.");
//OutputDebugString(tmpbuf); 
//        }
//    }
//
//    if (!FormatMessage(dwFormatFlags,
//                       //g_hResources,
//							  NULL,
//                       MessageId,
//                       0,  
//                       (LPWSTR)&pMessage, 
//                       0, 
//                       (va_list*)pArgs))
//    {
//        //wprintf(L"Format message failed with %lu\n", GetLastError());
//swprintf_s(tmpbuf, _countof(tmpbuf), L"Format message failed with %lu\n", GetLastError());
//OutputDebugString(tmpbuf); 
//			wprintf(tmpbuf);
//    }
//
//    if (pArgs)
//        free(pArgs);

    return pMessage;
}

// If the message string contains parameter insertion strings (for example, %%4096),
// you must perform the parameter substitution yourself. To get the parameter message 
// string, call FormatMessage with the message identifier found in the parameter insertion 
// string (for example, 4096 is the message identifier if the parameter insertion string
// is %%4096). You then substitute the parameter insertion string in the message 
// string with the actual parameter message string. 
DWORD EventLogHelper::ApplyParameterStringsToMessage(CONST LPCWSTR pMessage, LPWSTR & pFinalMessage)
{
    DWORD status = ERROR_SUCCESS;
//    DWORD dwParameterCount = 0;  // Number of insertion strings found in pMessage
//    DWORD dwBufferSize = 0;      // Size of the buffer in bytes
//    DWORD cchBuffer = 0;         // Size of the buffer in characters
//    DWORD cchParameters = 0;     // Number of characters in all the parameter strings
//    DWORD cch = 0;
//    DWORD i = 0;
//    LPWSTR* pStartingAddresses = NULL;  // Array of pointers to the beginning of each parameter string in pMessage
//    LPWSTR* pEndingAddresses = NULL;    // Array of pointers to the end of each parameter string in pMessage
//    DWORD* pParameterIDs = NULL;        // Array of parameter identifiers found in pMessage
//    LPWSTR* pParameters = NULL;         // Array of the actual parameter strings
//    LPWSTR pTempMessage = (LPWSTR)pMessage;
//    LPWSTR pTempFinalMessage = NULL;
//wchar_t tmpbuf[1024];
//    // Determine the number of parameter insertion strings in pMessage.
//    while (pTempMessage = wcschr(pTempMessage, L'%'))
//    {
//        dwParameterCount++;
//        pTempMessage++;
//    }
//
//    // If there are no parameter insertion strings in pMessage, return.
//    if (0 == dwParameterCount)
//    {
//        pFinalMessage = NULL;
//        goto cleanup;
//    }
//
//    // Allocate an array of pointers that will contain the beginning address 
//    // of each parameter insertion string.
//    dwBufferSize = sizeof(LPWSTR) * dwParameterCount;
//    pStartingAddresses = (LPWSTR*)malloc(dwBufferSize);
//    if (NULL == pStartingAddresses)
//    {
//        wprintf(L"Failed to allocate memory for pStartingAddresses.\n");
//swprintf_s(tmpbuf, _countof(tmpbuf), L"Failed to allocate memory for pStartingAddresses.");
//OutputDebugString(tmpbuf); 
//        status = ERROR_OUTOFMEMORY;
//        goto cleanup;
//    }
//
//    RtlZeroMemory(pStartingAddresses, dwBufferSize);
//
//    // Allocate an array of pointers that will contain the ending address (one
//    // character past the of the identifier) of the each parameter insertion string.
//    pEndingAddresses = (LPWSTR*)malloc(dwBufferSize);
//    if (NULL == pEndingAddresses)
//    {
//        wprintf(L"Failed to allocate memory for pEndingAddresses.\n");
//swprintf_s(tmpbuf, _countof(tmpbuf), L"Failed to allocate memory for pEndingAddresses.");
//OutputDebugString(tmpbuf); 
//        status = ERROR_OUTOFMEMORY;
//        goto cleanup;
//    }
//
//    RtlZeroMemory(pEndingAddresses, dwBufferSize);
//
//    // Allocate an array of pointers that will contain pointers to the actual
//    // parameter strings.
//    pParameters = (LPWSTR*)malloc(dwBufferSize);
//    if (NULL == pParameters)
//    {
//        //wprintf(L"Failed to allocate memory for pEndingAddresses.\n");
//swprintf_s(tmpbuf, _countof(tmpbuf), L"Failed to allocate memory for pEndingAddresses.");
//OutputDebugString(tmpbuf); 
//        status = ERROR_OUTOFMEMORY;
//        goto cleanup;
//    }
//
//    RtlZeroMemory(pParameters, dwBufferSize);
//
//    // Allocate an array of DWORDs that will contain the message identifier
//    // for each parameter.
//    pParameterIDs = (DWORD*)malloc(dwBufferSize);
//    if (NULL == pParameterIDs)
//    {
//        wprintf(L"Failed to allocate memory for pParameterIDs.\n");
//swprintf_s(tmpbuf, _countof(tmpbuf), L"Failed to allocate memory for pParameterIDs.");
//OutputDebugString(tmpbuf); 
//        status = ERROR_OUTOFMEMORY;
//        goto cleanup;
//    }
//
//    RtlZeroMemory(pParameterIDs, dwBufferSize);
//
//    // Find each parameter in pMessage and get the pointer to the
//    // beginning of the insertion string, the end of the insertion string,
//    // and the message identifier of the parameter.
//    pTempMessage = (LPWSTR)pMessage;
//    while (pTempMessage = wcschr(pTempMessage, L'%'))
//    {
//        if (isdigit(*(pTempMessage+1)))
//        {
//            pStartingAddresses[i] = pTempMessage;
//
//            pTempMessage++;
//            pParameterIDs[i] = (DWORD)_wtoi(pTempMessage);
//
//            while (isdigit(*++pTempMessage))
//                ;
//
//            pEndingAddresses[i] = pTempMessage;
//
//            i++;
//        }
//    }
//
//    // For each parameter, use the message identifier to get the
//    // actual parameter string.
//    for (DWORD i = 0; i < dwParameterCount; i++)
//    {
//        pParameters[i] = GetMessageString(pParameterIDs[i], 0, NULL);
//        if (NULL == pParameters[i])
//        {
//            wprintf(L"GetMessageString could not find parameter string for insert %lu.\n", i);
//swprintf_s(tmpbuf, _countof(tmpbuf), L"GetMessageString could not find parameter string for insert %lu.", i);
//OutputDebugString(tmpbuf); 
//            status = ERROR_INVALID_PARAMETER;
//            goto cleanup;
//        }
//
//        cchParameters += wcslen(pParameters[i]);
//    }
//
//    // Allocate enough memory for pFinalMessage based on the length of pMessage
//    // and the length of each parameter string. The pFinalMessage buffer will contain 
//    // the completed parameter substitution.
//    pTempMessage = (LPWSTR)pMessage;
//    dwBufferSize = (wcslen(pMessage) + cchParameters + 1) * sizeof(WCHAR);
//    pFinalMessage = (LPWSTR)malloc(dwBufferSize);
//    if (NULL == pFinalMessage)
//    {
//        wprintf(L"Failed to allocate memory for pFinalMessage.\n");
//swprintf_s(tmpbuf, _countof(tmpbuf), L"Failed to allocate memory for pFinalMessage.");
//OutputDebugString(tmpbuf); 
//
//        status = ERROR_OUTOFMEMORY;
//        goto cleanup;
//    }
//
//    RtlZeroMemory(pFinalMessage, dwBufferSize);
//    cchBuffer = dwBufferSize / sizeof(WCHAR);
//    pTempFinalMessage = pFinalMessage;
//
//    // Build the final message string.
//    for (DWORD i = 0; i < dwParameterCount; i++)
//    {
//        // Append the segment from pMessage. In the first iteration, this is "8 " and in the
//        // second iteration, this is " = 2 ".
//        wcsncpy_s(pTempFinalMessage, cchBuffer, pTempMessage, cch = (pStartingAddresses[i] - pTempMessage));
//        pTempMessage = pEndingAddresses[i];
//        cchBuffer -= cch;
//
//        // Append the parameter string. In the first iteration, this is "quarts" and in the
//        // second iteration, this is "gallons"
//        pTempFinalMessage += cch;
//        wcscpy_s(pTempFinalMessage, cchBuffer, pParameters[i]);
//        cchBuffer -= cch = wcslen(pParameters[i]);
//
//        pTempFinalMessage += cch;
//    }
//
//    // Append the last segment from pMessage, which is ".".
//    wcscpy_s(pTempFinalMessage, cchBuffer, pTempMessage);
//
//cleanup:
//
//    if (ERROR_SUCCESS != status)
//        pFinalMessage = (LPWSTR)pMessage;
//
//    if (pStartingAddresses)
//        free(pStartingAddresses);
//
//    if (pEndingAddresses)
//        free(pEndingAddresses);
//
//    if (pParameterIDs)
//        free(pParameterIDs);
//
//    for (DWORD i = 0; i < dwParameterCount; i++)
//    {
//        if (pParameters[i])
//            LocalFree(pParameters[i]);
//    }

    return status;
}



// Get a string that contains the time stamp of when the event 
// was generated.
void EventLogHelper::GetTimestamp(const DWORD Time, WCHAR DisplayString[])
{
    ULONGLONG ullTimeStamp = 0;
    ULONGLONG SecsTo1970 = 116444736000000000;
    SYSTEMTIME st;
    FILETIME ft, ftLocal;

    ullTimeStamp = Int32x32To64(Time, 10000000) + SecsTo1970;
    ft.dwHighDateTime = (DWORD)((ullTimeStamp >> 32) & 0xFFFFFFFF);
    ft.dwLowDateTime = (DWORD)(ullTimeStamp & 0xFFFFFFFF);
    
    FileTimeToLocalFileTime(&ft, &ftLocal);
    FileTimeToSystemTime(&ftLocal, &st);
    //StringCchPrintf(DisplayString, MAX_TIMESTAMP_LEN, L"%d/%d/%d %.2d:%.2d:%.2d", 
	 //wsprintf(DisplayString, MAX_TIMESTAMP_LEN, L"%d/%d/%d %.2d:%.2d:%.2d", 
	 swprintf_s(DisplayString, MAX_TIMESTAMP_LEN, L"%d/%d/%d %.2d:%.2d:%.2d", 
        st.wMonth, st.wDay, st.wYear, st.wHour, st.wMinute, st.wSecond);
}