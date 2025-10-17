#include "KeyError.h"

HRESULT WriteEventLog(wchar_t *event_log_msg, unsigned int event_type)
{
	HRESULT hr = S_OK;
    HANDLE h;
	
    h = RegisterEventSource(NULL, TEXT("Solimar License Server"));
    if (h == NULL) return HRESULT_FROM_WIN32(GetLastError());
 
    if (!ReportEventW(h,           // event log handle 
            (WORD)(event_type),   // event type 
            0,                    // category zero 
            0,					  // event identifier 
            NULL,                 // no user security identifier 
            1,                    // one substitution string 
            0,                    // no data 
            (LPCWSTR*)&event_log_msg,       // pointer to string array 
            NULL))                // pointer to data 
		hr = HRESULT_FROM_WIN32(GetLastError());
    
    DeregisterEventSource(h); 
	
	return hr;
}


std::wstring GetErrorMessage(HRESULT hr)
{
	wchar_t hrErrMsg[256];
	swprintf(hrErrMsg, L" [0x%08X]", hr);
	std::wstring retVal(L"Unknown Error");
	
	//Trap General Installer Error
	if(hr == 0x80070643)
	{
		//0x80070643 translates to Fatal Installer Error, not pretty.
		retVal = L"An error was encountered while installing.";
	}
	//Detect if it is a Win32 error
	else if((hr & 0x80070000) == 0x80070000)  //that location is where win32 errors set their value
	{
		HRESULT tempHr(hr & 0x0000FFFF); //Convert, take the lower 4
		
		LPVOID lpMsgBuf;
		FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			tempHr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(wchar_t*)&lpMsgBuf,
			0,
			NULL);
		std::wstring tempMsg = (wchar_t*)lpMsgBuf;
		size_t index(tempMsg.length());
		while(tempMsg[index]==L'\n' || tempMsg[index]==L'\r')
		{
			tempMsg[index] = L'\0';
			index--;
		}
		retVal = std::wstring(tempMsg.c_str());
		
		// Free the buffer.
		LocalFree(lpMsgBuf);
	}
	else
	{
		IErrorInfo* pIErrorInfo = NULL;
		GetErrorInfo(0, &pIErrorInfo);
		if (pIErrorInfo)
		{
			BSTR des;
			if (SUCCEEDED(pIErrorInfo->GetDescription(&des)))
			{
				retVal = std::wstring(des);
				SysFreeString(des);
			}
			pIErrorInfo->Release();
		}
	}
	//TRACE(AnsiString(retVal + AnsiString(hrErrMsg)).c_str());
	return retVal + std::wstring(hrErrMsg);
}
