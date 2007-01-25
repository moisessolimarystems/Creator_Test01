#include "LicenseError.h"
namespace LicenseServerError
{
/*
 * SLErrors[]
 *
 * SOLsearcher custom errors.
 */

const SL_ERROR SLErrors[] = 
{
	//----------------------------------------------------------------------------------------------
	// Error Code						HRESULT								Message
	//----------------------------------------------------------------------------------------------

	// key server errors
	{EC_UNKNOWN,						EHR_UNKNOWN,						L"Unknown error (Key server)"},
	{EC_DRIVER_INIT_FAILURE,			EHR_DRIVER_INIT_FAILURE,			L"Driver initialization failure (Key server)"},
	{EC_PASSWORD_NO_MATCH,				EHR_PASSWORD_NO_MATCH,				L"Passwords do not match (Key server)"},
	{EC_PASSWORD_INVALID_FORMAT,		EHR_PASSWORD_INVALID_FORMAT,		L"Invalid password format (Key server)"},
	{EC_LICENSE_INSUFFICIENT,			EHR_LICENSE_INSUFFICIENT,			L"Insufficient license (Key server)"},
	{EC_LICENSE_INVALID,				EHR_LICENSE_INVALID,				L"Invalid license (Key server)"},
	{EC_LICENSE_EXPRIED,				EHR_LICENSE_EXPRIED,				L"License expired (Key server)"},
	{EC_KEY_TRIAL_EXPIRED,				EHR_KEY_TRIAL_EXPIRED,				L"Trial Key expired (Key server)"},
	{EC_KEY_NOT_PRESENT,				EHR_KEY_NOT_PRESENT,				L"Key not present (Key server)"},
	{EC_CLOCK_ERROR,					EHR_CLOCK_ERROR,					L"Clock error (Key server)"},
	{EC_KEY_WRITE_FAILURE,				EHR_KEY_WRITE_FAILURE,				L"Failed writing to the key (Key server)"},
	{EC_KEY_WRITE_ACCESS_DENIED,		EHR_KEY_WRITE_ACCESS_DENIED,		L"Failed writing to the key, access is denied (Key server)"},
	{EC_KEY_NOT_ACTIVATED,				EHR_KEY_NOT_ACTIVATED,				L"Key not active (Key server)"},
	{EC_AUTHENTICATION_FAILURE,			EHR_AUTHENTICATION_FAILURE,			L"Authentication failure (Key server)"},
	{EC_MANAGER_NOT_INITIALIZED,		EHR_MANAGER_NOT_INITIALIZED,		L"Manager not initialized (Key server)"},
	{EC_KEY_NO_SUITABLE_KEY,			EHR_KEY_NO_SUITABLE_KEY,			L"No suitable key found (Key server)"},
	{EC_KEY_LOCKED,						EHR_KEY_LOCKED,						L"Key locked (Key server)"},
	{EC_CLIENT_TIMEOUT,					EHR_CLIENT_TIMEOUT,					L"Client timeout (Key server)"},
	{EC_PACKET_EXPIRED,					EHR_PACKET_EXPIRED,					L"Packet expired (Key server)"},

	// rainbox driver errors 
	{EC_SP_INVALID_FUNCTION_CODE,		EHR_SP_INVALID_FUNCTION_CODE,		L"Invalid function code (Rainbow driver)"},
	{EC_SP_INVALID_PACKET,				EHR_SP_INVALID_PACKET,				L"Invalid packet (Rainbow driver)"},	
	{EC_SP_UNIT_NOT_FOUND,				EHR_SP_UNIT_NOT_FOUND,				L"Unit not found (Rainbow driver)"},
	{EC_SP_ACCESS_DENIED,				EHR_SP_ACCESS_DENIED,				L"Access is denied (Rainbow driver)"},
	{EC_SP_INVALID_MEMORY_ADDRESS,		EHR_SP_INVALID_MEMORY_ADDRESS,		L"Invalid memory address (Rainbow driver)"},		
	{EC_SP_INVALID_ACCESS_CODE,			EHR_SP_INVALID_ACCESS_CODE,			L"Invalid access code (Rainbow driver)"},
	{EC_SP_PORT_IS_BUSY,				EHR_SP_PORT_IS_BUSY,				L"Port is busy (Rainbow driver)"},
	{EC_SP_WRITE_NOT_READY,				EHR_SP_WRITE_NOT_READY,				L"Write not ready (Rainbow driver)"},
	{EC_SP_NO_PORT_FOUND,				EHR_SP_NO_PORT_FOUND,				L"No parallel ports found (Rainbow driver)"},
	{EC_SP_ALREADY_ZERO,				EHR_SP_ALREADY_ZERO,				L"Counter already zero (Rainbow driver)"},
	{EC_SP_DRIVER_NOT_INSTALLED,		EHR_SP_DRIVER_NOT_INSTALLED,		L"System device driver not installed or detected (Rainbow driver)"},
	{EC_SP_VERSION_NOT_SUPPORTED,		EHR_SP_VERSION_NOT_SUPPORTED,		L"System device driver is outdated, update the driver (Rainbow driver)"},
	{EC_SP_QUERY_TOO_LONG,				EHR_SP_QUERY_TOO_LONG,				L"Query to long (Rainbow driver)"},
	{EC_SP_DRIVER_IS_BUSY,				EHR_SP_DRIVER_IS_BUSY,				L"System driver is busy (Rainbow driver)"},
	{EC_SP_PORT_ALLOCATION_FAILURE,		EHR_SP_PORT_ALLOCATION_FAILURE,		L"Failure to allocate parallel port (Rainbow driver)"},
	{EC_SP_PORT_RELEASE_FAILURE,		EHR_SP_PORT_RELEASE_FAILURE,		L"Failure to release previously allocated parallel port (Rainbow driver)"},
	{EC_SP_ACQUIRE_PORT_TIMEOUT,		EHR_SP_ACQUIRE_PORT_TIMEOUT,		L"Failure to aquire access to parallel port with time-out period (Rainbow driver)"},
	{EC_SP_SIGNAL_NOT_SUPPORTED,		EHR_SP_SIGNAL_NOT_SUPPORTED,		L"Machine does not support a signal line (Rainbow driver)"},
	{EC_SP_INIT_NOT_CALLED,				EHR_SP_INIT_NOT_CALLED,				L"Key is not initialized (Rainbow driver)"},
	{EC_SP_FAIL_ON_DRIVER_COMM,			EHR_SP_FAIL_ON_DRIVER_COMM,			L"Client library failed on communication with the Rainbow system driver (Rainbow driver)"},
	{EC_SP_SERVER_PROBABLY_NOT_UP,		EHR_SP_SERVER_PROBABLY_NOT_UP,		L"The server is not responding, the client has timed out (Rainbow driver)"},
	{EC_SP_UNKNOWN_HOST,				EHR_SP_UNKNOWN_HOST,				L"The server host is unknown; the server is not on the network or an invalid host name was specified (Rainbow driver)"},
	{EC_SP_SENDTO_FAILED,				EHR_SP_SENDTO_FAILED,				L"Unable to send a message to the server (Rainbow driver)"},
	{EC_SP_SOCKET_CREATION_FAILED,		EHR_SP_SOCKET_CREATION_FAILED,		L"Unable able to open a network socket (Rainbow driver)"},
	{EC_SP_NORESOURCES,					EHR_SP_NORESOURCES,					L"Insufficient resources (such as memory) to complete the request (Rainbow driver)"},
	{EC_SP_BROADCAST_NOT_SUPPORTED,		EHR_SP_BROADCAST_NOT_SUPPORTED,		L"Broadcast is not supported by the network interface on the system (Rainbow driver)"},
	{EC_SP_BAD_SERVER_MESSAGE,			EHR_SP_BAD_SERVER_MESSAGE,			L"Could not understand a message received from the server (Rainbow driver)"},
	{EC_SP_NO_SERVER_RUNNING,			EHR_SP_NO_SERVER_RUNNING,			L"Could not communicate with the server; verify the server is running (Rainbow driver)"},
	{EC_SP_NO_NETWORK,					EHR_SP_NO_NETWORK,					L"Unable to talk to the specified host; network communication problems encountered (Rainbow driver)"},
	{EC_SP_NO_SERVER_RESPONSE,			EHR_SP_NO_SERVER_RESPONSE,			L"No server responded to the client broadcast. There is no server running in the subnet (Rainbow driver)"},
	{EC_SP_NO_LICENSE_AVAILABLE,		EHR_SP_NO_LICENSE_AVAILABLE,		L"All licenses are currently in use (Rainbow driver)"},
	{EC_SP_INVALID_LICENSE,				EHR_SP_INVALID_LICENSE,				L"License is no longer valid (Rainbow driver)"},
	{EC_SP_INVALID_OPERATION,			EHR_SP_INVALID_OPERATION,			L"Specified operation cannot be performed (Rainbow driver)"},
	{EC_SP_BUFFER_TOO_SMALL,			EHR_SP_BUFFER_TOO_SMALL,			L"The size of the buffer is not sufficient to hold the expected data (Rainbow driver)"},
	{EC_SP_INTERNAL_ERROR,				EHR_SP_INTERNAL_ERROR,				L"An internal error (Rainbow driver)"},
	{EC_SP_PACKET_ALREADY_INITIALIZED,	EHR_SP_PACKET_ALREADY_INITIALIZED,	L"The packet being initialized was already initialized (Rainbow driver)"},
	{EC_SP_INVALID_STATUS,				EHR_SP_INVALID_STATUS,				L"An invalid status code was returned (Rainbow driver)"},
};

const unsigned long SL_ERROR_COUNT = sizeof(SLErrors) / sizeof(SL_ERROR);


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
	
	if(SL_IS_LIC_HR(hr))
	{
		//Detect a Solimar License error
		retVal = GetECMessage(SL_EC_FROM_EHR(hr));
	}
	else if((hr & 0x80070000) == 0x80070000)  //that location is where win32 errors set their value
	{
		//Detect if it is a Win32 error
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


/*
 * GetECMessage()
 *
 * Looks up a custom Solimar License Error message given the HR error code.
 */
std::wstring GetECMessage(unsigned long echr)
{
	unsigned long ec = echr & 0x0000FFFF;
	for (int i=0; i<SL_ERROR_COUNT; i++) {
		if (SLErrors[i].EC == ec)
			return std::wstring(SLErrors[i].message);
	}
	return std::wstring(SLErrors[0].message); // Unknown
}
};