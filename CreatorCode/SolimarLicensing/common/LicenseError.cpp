#include "LicenseError.h"
#include "EventLogHelper.h"
namespace LicenseServerError
{
/*
 * SLErrors[]
 *
 * custom errors.
 */
const SL_ERROR SLErrors[] =
{
	//----------------------------------------------------------------------------------------------
	// Error Code						HRESULT						Message
	//----------------------------------------------------------------------------------------------
	
	// License server errors
	{EC_UNKNOWN,						EHR_UNKNOWN,						L"Unknown error (License server)"},
	{EC_DRIVER_INIT_FAILURE,						EHR_DRIVER_INIT_FAILURE,						L"Driver initialization failure (License server)"},
	{EC_PASSWORD_NO_MATCH,						EHR_PASSWORD_NO_MATCH,						L"Passwords do not match (License server)"},
	{EC_PASSWORD_INVALID_FORMAT,						EHR_PASSWORD_INVALID_FORMAT,						L"Invalid password format (License server)"},
	{EC_LICENSE_INSUFFICIENT,						EHR_LICENSE_INSUFFICIENT,						L"Insufficient license (License server)"},
	{EC_LICENSE_INVALID,						EHR_LICENSE_INVALID,						L"Invalid license (License server)"},
	{EC_LICENSE_EXPRIED,						EHR_LICENSE_EXPRIED,						L"License expired (License server)"},
	{EC_KEY_TRIAL_EXPIRED,						EHR_KEY_TRIAL_EXPIRED,						L"Trial Key expired (License server)"},
	{EC_KEY_NOT_PRESENT,						EHR_KEY_NOT_PRESENT,						L"Key not present (License server)"},
	{EC_CLOCK_ERROR,						EHR_CLOCK_ERROR,						L"Clock error (License server)"},
	{EC_KEY_WRITE_FAILURE,						EHR_KEY_WRITE_FAILURE,						L"Failed writing to the key (License server)"},
	{EC_KEY_WRITE_ACCESS_DENIED,						EHR_KEY_WRITE_ACCESS_DENIED,						L"Failed writing to the key, access is denied (License server)"},
	{EC_KEY_NOT_ACTIVATED,						EHR_KEY_NOT_ACTIVATED,						L"Key not active (License server)"},
	{EC_AUTHENTICATION_FAILURE,						EHR_AUTHENTICATION_FAILURE,						L"Authentication failure (License server)"},
	{EC_MANAGER_NOT_INITIALIZED,						EHR_MANAGER_NOT_INITIALIZED,						L"Manager not initialized (License server)"},
	{EC_KEY_NO_SUITABLE_KEY,						EHR_KEY_NO_SUITABLE_KEY,						L"No suitable key found (License server)"},
	{EC_KEY_LOCKED,						EHR_KEY_LOCKED,						L"Key locked (License server)"},
	{EC_CLIENT_TIMEOUT,						EHR_CLIENT_TIMEOUT,						L"Client timeout (License server)"},
	{EC_PACKET_EXPIRED,						EHR_PACKET_EXPIRED,						L"Packet expired (License server)"},
	{EC_KEY_BASE_NOT_MATCHING,						EHR_KEY_BASE_NOT_MATCHING,						L"Base Keys do not match module for module (License server)"},
	{EC_KEY_NOT_MATCHING_INSTANCES,						EHR_KEY_NOT_MATCHING_INSTANCES,						L"Keys do not match by Application Instances for all the Key on the server (License server)"},
	{EC_KEY_NO_FREE_APP_INSTANCE,						EHR_KEY_NO_FREE_APP_INSTANCE,						L"Insufficient licensing, unable to obtain a Product Connection on the server (License server)"},
	{EC_KEY_NO_BASE_KEY,						EHR_KEY_NO_BASE_KEY,						L"Insufficient licensing, unable to locate a Base Key on the server (License server)"},
	{EC_KEY_RESTORED_KEY_SERVER,						EHR_KEY_RESTORED_KEY_SERVER,						L"Licensing on the License server has been restored (License server)"},
	{EC_KEY_USE_FAILOVER_KEY_SERVER,						EHR_KEY_USE_FAILOVER_KEY_SERVER,						L"Unable to validate Licensing on the License server, switching to Failover License server (License server)"},
	{EC_KEY_NO_REMOTE_VERSION_KEY_SERVER,						EHR_KEY_NO_REMOTE_VERSION_KEY_SERVER,						L"License Server is at a version that does not support remote connectivity.  Please upgrade the License Server (License server)"},
	{EC_LIC_PROD_NO_FREE_APP_INSTANCE,						EHR_LIC_PROD_NO_FREE_APP_INSTANCE,						L"Unable to obtain a Product Connection on the server for the given product (License server)"},
	{EC_LIC_MOD_NO_FREE_APP_INSTANCE,						EHR_LIC_MOD_NO_FREE_APP_INSTANCE,						L"Unable to obtain a Product Connection on the server for the given module (License server)"},
	{EC_LIC_SOFTWARE_VALIDATION_FAILED_KEY_ID,						EHR_LIC_SOFTWARE_VALIDATION_FAILED_KEY_ID,						L"Failed to validate software license with Protection Key ID (License server)"},
	{EC_LIC_SOFTWARE_VALIDATION_FAILED_LICENSE_CODE,						EHR_LIC_SOFTWARE_VALIDATION_FAILED_LICENSE_CODE,						L"Failed to validate software license with License Code (License server)"},
	{EC_LIC_SOFTWARE_VALIDATION_BIOS_SERIALNUMBER,						EHR_LIC_SOFTWARE_VALIDATION_FAILED_BIOS_SERIALNUMBER,						L"Failed to validate software license with Bios Serial Number (License server)"},
	{EC_LIC_SOFTWARE_VALIDATION_FAILED_MAC_ADDRESS,						EHR_LIC_SOFTWARE_VALIDATION_FAILED_MAC_ADDRESS,						L"Failed to validate software license with MAC address (License server)"},
	{EC_LIC_SOFTWARE_VALIDATION_FAILED_COMPUTER_NAME,						EHR_LIC_SOFTWARE_VALIDATION_FAILED_COMPUTER_NAME,						L"Failed to validate software license with Computer Name (License server)"},
	{EC_LIC_SOFTWARE_VALIDATION_FAILED_CUSTOMER_COPY,						EHR_LIC_SOFTWARE_VALIDATION_FAILED_CUSTOMER_COPY,						L"Can not validate against a Customer Copy (License server)"},
	{EC_LIC_SOFTWARE_VALIDATION_FAILED_ARCHIVE,						EHR_LIC_SOFTWARE_VALIDATION_FAILED_ARCHIVE,						L"Can not validate against a License Archive (License server)"},
	{EC_LIC_SOFTWARE_VALIDATION_FAILED_PACKET,						EHR_LIC_SOFTWARE_VALIDATION_FAILED_PACKET,						L"Can not validate against a License Packet (License server)"},
	{EC_LIC_SOFTWARE_CONNECT_FAILOVER_ONLY,						EHR_LIC_SOFTWARE_CONNECT_FAILOVER_ONLY,						L"Failed to connect.  This License Server is configured as a Failover License Server and can only be connected to as a failover (License server)"},
	{EC_LIC_SOFTWARE_LIC_PACKET_ALREADY_ENTERED,						EHR_LIC_SOFTWARE_LIC_PACKET_ALREADY_ENTERED,						L"Failed to apply Software License Packet.  This Packet has already been applied (License server)"},
	{EC_LIC_SOFTWARE_CANT_GENERATE_LIC_PACKET,						EHR_LIC_SOFTWARE_CANT_GENERATE_LIC_PACKET,						L"License Server does not support generating License Packets (License server)"},
	{EC_LIC_SOFTWARE_CANT_GENERATE_LIC_ARCHIVE_NO_KEY,						EHR_LIC_SOFTWARE_CANT_GENERATE_LIC_ARCHIVE_NO_KEY,						L"Can not generate License Archive that does not validate against a Protection Key (License server)"},
	{EC_LIC_SOFTWARE_LIC_ARCHIVE_CODE_NO_MATCH,						EHR_LIC_SOFTWARE_LIC_ARCHIVE_CODE_NO_MATCH,						L"Failed to apply License Archive. License Codes do not match, probably trying to use an old License Archive (License server)"},
	{EC_LIC_SOFTWARE_KEY_ID_REQUIRED,						EHR_LIC_SOFTWARE_KEY_ID_REQUIRED,						L"Failed to update software license, appropriate Protection Key is required to be on the License Server for this action (License server)"},
	{EC_LIC_SOFTWARE_LIC_FILE_EXPIRED,						EHR_LIC_SOFTWARE_LIC_FILE_EXPIRED,						L"Software License Expired (License server)"},
	{EC_LIC_SOFTWARE_NO_PRODUCT,						EHR_LIC_SOFTWARE_NO_PRODUCT,						L"Software Licensing does not contain licensing for the given product (License Server)"},
	{EC_LIC_SOFTWARE_PRODUCT_NO_VERSION,						EHR_LIC_SOFTWARE_PRODUCT_NO_VERSION,						L"Software Licensing contains licensing for the given product, but not at the desired version (License Server)"},
	{EC_LIC_MOD_NO_OBTAIN_USING_FAILOVER,						EHR_LIC_MOD_NO_OBTAIN_USING_FAILOVER,						L"Unable to obtain module when using Backup Server (License Server)"},
	{EC_LIC_CLOCK_LIC_PACKET,						EHR_LIC_CLOCK_LIC_PACKET,						L"The System Time does not seem to be set correctly (License Server)"},
	{EC_LIC_CLOCK_LIC_ARCHIVE,						EHR_LIC_CLOCK_LIC_ARCHIVE,						L"The System Time does not seem to be set correctly (License Server)"},
	{EC_LIC_SOFTWARE_VALIDATION_FAILED_GENERIC,						EHR_LIC_SOFTWARE_VALIDATION_FAILED_GENERIC,						L"Failed to validate software license with a Validation Token (License server)"},
	{EC_LIC_SOFTWARE_LIC_PACKET_LIC_SERVER_UPGRADE,						EHR_LIC_SOFTWARE_LIC_PACKET_LIC_SERVER_UPGRADE,						L"Unable to use Software License Packet.  The License Server has to be upgraded before this Software License Packet can be used (License server)"},
	{EC_LIC_SOFTWARE_PRODUCT_LIC_EXPIRED,						EHR_LIC_SOFTWARE_PRODUCT_LIC_EXPIRED,						L"Product License Expired (License server)"},
	{EC_LIC_MAIL_SERVER,						EHR_LIC_MAIL_SERVER,						L"Dynamic Mail Server Error (License server)"},
	{EC_DONGLE_EMULATOR,						EHR_DONGLE_EMULATOR,						L"Second 1515 error verifying License Server, please contact Solimar Systems, Inc. (License server)"},

	{EC_SP_INVALID_FUNCTION_CODE,						EHR_SP_INVALID_FUNCTION_CODE,						L"Invalid function code (Rainbow driver)"},
	{EC_SP_INVALID_PACKET,						EHR_SP_INVALID_PACKET,						L"Invalid packet (Rainbow driver)"},
	{EC_SP_UNIT_NOT_FOUND,						EHR_SP_UNIT_NOT_FOUND,						L"Unit not found (Rainbow driver)"},
	{EC_SP_ACCESS_DENIED,						EHR_SP_ACCESS_DENIED,						L"Access is denied (Rainbow driver)"},
	{EC_SP_INVALID_MEMORY_ADDRESS,						EHR_SP_INVALID_MEMORY_ADDRESS,						L"Invalid memory address (Rainbow driver)"},
	{EC_SP_INVALID_ACCESS_CODE,						EHR_SP_INVALID_ACCESS_CODE,						L"Invalid access code (Rainbow driver)"},
	{EC_SP_PORT_IS_BUSY,						EHR_SP_PORT_IS_BUSY,						L"Port is busy (Rainbow driver)"},
	{EC_SP_WRITE_NOT_READY,						EHR_SP_WRITE_NOT_READY,						L"Write not ready (Rainbow driver)"},
	{EC_SP_NO_PORT_FOUND,						EHR_SP_NO_PORT_FOUND,						L"No parallel ports found (Rainbow driver)"},
	{EC_SP_ALREADY_ZERO,						EHR_SP_ALREADY_ZERO,						L"Counter already zero (Rainbow driver)"},
	{EC_SP_DRIVER_NOT_INSTALLED,						EHR_SP_DRIVER_NOT_INSTALLED,						L"System device driver not installed or detected (Rainbow driver)"},
	{EC_SP_VERSION_NOT_SUPPORTED,						EHR_SP_VERSION_NOT_SUPPORTED,						L"System device driver is outdated, update the driver (Rainbow driver)"},
	{EC_SP_QUERY_TOO_LONG,						EHR_SP_QUERY_TOO_LONG,						L"Query to long (Rainbow driver)"},
	{EC_SP_DRIVER_IS_BUSY,						EHR_SP_DRIVER_IS_BUSY,						L"System driver is busy (Rainbow driver)"},
	{EC_SP_PORT_ALLOCATION_FAILURE,						EHR_SP_PORT_ALLOCATION_FAILURE,						L"Failure to allocate parallel port (Rainbow driver)"},
	{EC_SP_PORT_RELEASE_FAILURE,						EHR_SP_PORT_RELEASE_FAILURE,						L"Failure to release previously allocated parallel port (Rainbow driver)"},
	{EC_SP_ACQUIRE_PORT_TIMEOUT,						EHR_SP_ACQUIRE_PORT_TIMEOUT,						L"Failure to aquire access to parallel port with time-out period (Rainbow driver)"},
	{EC_SP_SIGNAL_NOT_SUPPORTED,						EHR_SP_SIGNAL_NOT_SUPPORTED,						L"Machine does not support a signal line (Rainbow driver)"},
	{EC_SP_INIT_NOT_CALLED,						EHR_SP_INIT_NOT_CALLED,						L"Key is not initialized (Rainbow driver)"},
	{EC_SP_FAIL_ON_DRIVER_COMM,						EHR_SP_FAIL_ON_DRIVER_COMM,						L"Client library failed on communication with the Rainbow system driver (Rainbow driver)"},
	{EC_SP_SERVER_PROBABLY_NOT_UP,						EHR_SP_SERVER_PROBABLY_NOT_UP,						L"The server is not responding, the client has timed out (Rainbow driver)"},
	{EC_SP_UNKNOWN_HOST,						EHR_SP_UNKNOWN_HOST,						L"The server host is unknown; the server is not on the network or an invalid host name was specified (Rainbow driver)"},
	{EC_SP_SENDTO_FAILED,						EHR_SP_SENDTO_FAILED,						L"Unable to send a message to the server (Rainbow driver)"},
	{EC_SP_SOCKET_CREATION_FAILED,						EHR_SP_SOCKET_CREATION_FAILED,						L"Unable able to open a network socket (Rainbow driver)"},
	{EC_SP_NORESOURCES,						EHR_SP_NORESOURCES,						L"Insufficient resources (such as memory) to complete the request (Rainbow driver)"},
	{EC_SP_BROADCAST_NOT_SUPPORTED,						EHR_SP_BROADCAST_NOT_SUPPORTED,						L"Broadcast is not supported by the network interface on the system (Rainbow driver)"},
	{EC_SP_BAD_SERVER_MESSAGE,						EHR_SP_BAD_SERVER_MESSAGE,						L"Could not understand a message received from the server (Rainbow driver)"},
	{EC_SP_NO_SERVER_RUNNING,						EHR_SP_NO_SERVER_RUNNING,						L"Could not communicate with the server; verify the server is running (Rainbow driver)"},
	{EC_SP_NO_NETWORK,						EHR_SP_NO_NETWORK,						L"Unable to talk to the specified host; network communication problems encountered (Rainbow driver)"},
	{EC_SP_NO_SERVER_RESPONSE,						EHR_SP_NO_SERVER_RESPONSE,						L"No server responded to the client broadcast. There is no server running in the subnet (Rainbow driver)"},
	{EC_SP_NO_LICENSE_AVAILABLE,						EHR_SP_NO_LICENSE_AVAILABLE,						L"All licenses are currently in use (Rainbow driver)"},
	{EC_SP_INVALID_LICENSE,						EHR_SP_INVALID_LICENSE,						L"License is no longer valid (Rainbow driver)"},
	{EC_SP_INVALID_OPERATION,						EHR_SP_INVALID_OPERATION,						L"Specified operation cannot be performed (Rainbow driver)"},
	{EC_SP_BUFFER_TOO_SMALL,						EHR_SP_BUFFER_TOO_SMALL,						L"The size of the buffer is not sufficient to hold the expected data (Rainbow driver)"},
	{EC_SP_INTERNAL_ERROR,						EHR_SP_INTERNAL_ERROR,						L"An internal error (Rainbow driver)"},
	{EC_SP_PACKET_ALREADY_INITIALIZED,						EHR_SP_PACKET_ALREADY_INITIALIZED,						L"The packet being initialized was already initialized (Rainbow driver)"},
	{EC_SP_PROTOCOL_NOT_INSTALLED,						EHR_SP_PROTOCOL_NOT_INSTALLED,						L"The specified protocol is not installed (Rainbow driver)"},
	{EC_SP_NO_LEASE_FEATURE,						EHR_SP_NO_LEASE_FEATURE,						L"The element does not contain any lease period. Probably, this is not a full licenseelement (Rainbow driver)"},
	{EC_SP_LEASE_EXPIRED,						EHR_SP_LEASE_EXPIRED,						L"The trial version expiration date has reached before the application was first executed (Rainbow driver)"},
	{EC_SP_COUNTER_LIMIT_REACHED,						EHR_SP_COUNTER_LIMIT_REACHED,						L"The value requested for the decrement operation exceeds the current counter value (Rainbow driver)"},
	{EC_SP_NO_DIGITAL_SIGNATURE,						EHR_SP_NO_DIGITAL_SIGNATURE,						L"The Sentinel driver binary is not signed by a valid authority (Rainbow driver)"},
	{EC_SP_SYS_FILE_CORRUPTED,						EHR_SP_SYS_FILE_CORRUPTED,						L"The digital certificate of the Sentinel driver is not valid (Rainbow driver)"},
	{EC_SP_INVALID_DEV_CODE,						EHR_SP_INVALID_DEV_CODE,						L"You specified an invalid Developer Code (Rainbow driver)"},
	{EC_SP_DEVID_DOES_NOT_MATCH,						EHR_SP_DEVID_DOES_NOT_MATCH,						L"The specified developer ID doesn't match the one in Developer Code (Rainbow driver)"},
	{EC_SP_DEVICE_SHARING_DETECTED,						EHR_SP_DEVICE_SHARING_DETECTED,						L"Device is shared by more than one system or shared in virtual machine environment (Rainbow driver)"},
	{EC_SP_SERVER_VERSION_NOT_SUPPORTED,						EHR_SP_SERVER_VERSION_NOT_SUPPORTED,						L"The Sentinel Protection Server from which you are trying to obtain a license is outdated (Rainbow driver)"},
	{EC_SP_FILE_NOT_FOUND,						EHR_SP_FILE_NOT_FOUND,						L"Specified file is not found (Rainbow driver)"},
	{EC_SP_PATH_TOO_LONG,						EHR_SP_PATH_TOO_LONG,						L"Specified path is too long (Rainbow driver)"},
	{EC_SP_SOFT_DB_CORRUPT,						EHR_SP_SOFT_DB_CORRUPT,						L"The software key database is in corrupt state (Rainbow driver)"},
	{EC_SP_SOFT_DB_RESTORE_DETECTED,						EHR_SP_SOFT_DB_RESTORE_DETECTED,						L"The software key database has been illegally restored to an earlier version (Rainbow driver)"},
	{EC_SP_PRST_DATA_CORRUPT,						EHR_SP_PRST_DATA_CORRUPT,						L"The SuperPro persistence data is in corrupt state (Rainbow driver)"},
	{EC_SP_SECURITY_RUNTIME_NOT_DETECTED,						EHR_SP_SECURITY_RUNTIME_NOT_DETECTED,						L"The Sentinel Security Runtime was not installed or detected (Rainbow driver)"},
	{EC_SP_TIME_TAMPER_DETECTED,						EHR_SP_TIME_TAMPER_DETECTED,						L"A time tampering condition is detected because the LKD (Last Known good Date) stored in the key is ahead of the system's current date (Rainbow driver)"},
	{EC_SP_END_DATE_REACHED,						EHR_SP_END_DATE_REACHED,						L"The software key is invalid because the key end date has reached (Rainbow driver)"},
	{EC_SP_START_DATE_NOT_REACHED,						EHR_SP_START_DATE_NOT_REACHED,						L"The software key is invalid because the system's current date is ahead of the key start date (Rainbow driver)"},
	{EC_SP_HOST_ID_ERROR,						EHR_SP_HOST_ID_ERROR,						L"The fingerprint of the machine has been changed or the license is enforced with the user name and you are using a different user name (Rainbow driver)"},
	{EC_SP_LIC_INVALID,						EHR_SP_LIC_INVALID,						L"SuperPro Software Key is invalid (Rainbow driver)"},
	{EC_SP_LIC_RUN_TIME_ERROR,						EHR_SP_LIC_RUN_TIME_ERROR,						L"License runtime failed to work due to some error (Rainbow driver)"},
	{EC_SP_LIC_MEMORY_CORRUPTED,						EHR_SP_LIC_MEMORY_CORRUPTED,						L"License is corrupted (like invalid cell format) (Rainbow driver)"},
	{EC_SP_NOT_ENOUGH_MEMORY,						EHR_SP_NOT_ENOUGH_MEMORY,						L"Not enough storage is available to process this command (Rainbow driver)"},
	{EC_SP_IP_ADDRESS_BLOCKED,						EHR_SP_IP_ADDRESS_BLOCKED,						L"Cannot communicate with the Sentinel Protection Server because the IP address of the client is blocked by the server (Rainbow driver)"},
	{EC_SP_SERVER_OUT_OF_WORKING_TIME,						EHR_SP_SERVER_OUT_OF_WORKING_TIME,						L"Cannot communicate with the Sentinel Protection Server because the server is out of the working time (Rainbow driver)"},
	{EC_SP_SECURITY_RUNTIME_VERSION_MISMATCH,						EHR_SP_SECURITY_RUNTIME_VERSION_MISMATCH,						L"SuperPro persistence component versions mismatch (Rainbow driver)"},
	{EC_SP_INVALID_STATUS,						EHR_SP_INVALID_STATUS,						L"An invalid status code was returned (Rainbow driver)"},

	{EC_WRPPR_SVR_NOT_CONNECTED,						EHR_WRPPR_SVR_NOT_CONNECTED,						L"Not Connected (License Server Wrapper)"},

	{EC_LIC_MGR_NO_COMPUTER,						EHR_LIC_MGR_NO_COMPUTER,						L"Could not communicate with the computer, verify computer name is correct (License Manager)"},
	{EC_LIC_MGR_NO_LIC_SERVER,						EHR_LIC_MGR_NO_LIC_SERVER,						L"Unable to contact License Server on the computer, verify License Server is running and is at proper version (License Manager)"},
	{EC_LIC_MGR_VIEW_ONLY,						EHR_LIC_MGR_VIEW_ONLY,						L"Unable to perform the given action, initialized for view only (License Manager)"},
};

const unsigned long SL_ERROR_COUNT = sizeof(SLErrors) / sizeof(SL_ERROR);


HRESULT WriteEventLog(wchar_t *event_log_msg, unsigned int event_type, long event_id, long product_id)
{
	return WriteEventLog(event_log_msg, event_type, (product_id!=-1) ? (product_id * 1000) + event_id : event_id);
}

HRESULT WriteEventLog(wchar_t *event_log_msg, unsigned int event_type, long event_id)
{
	//TReadEventLog();
	return EventLogHelper::WriteEventLog(L"Solimar License Server", event_log_msg, event_type, event_id);
}

//#define MAX_RECORD_BUFFER_SIZE  0x10000  // 64K
HRESULT TReadEventLog()
{
	return E_NOTIMPL;
	//return EventLogHelper::ReadEventLog_Helper();
}

// Tries to look up error by looking up IErrorInfo first...
std::wstring GetErrorMessage(HRESULT hr)
{
	wchar_t hrErrMsg[256];
	#if _MSC_VER >= 1400
		swprintf_s(hrErrMsg, sizeof(hrErrMsg)/sizeof(wchar_t), L" [0x%08X]", hr);
	#else
		swprintf(hrErrMsg, L" [0x%08X]", hr);
	#endif
	std::wstring retVal(L"Unknown Error");

	IErrorInfo* pIErrorInfo = NULL;
	if (GetErrorInfo(0, &pIErrorInfo) == S_OK)
	{
		BSTR des;
		if (SUCCEEDED(pIErrorInfo->GetDescription(&des)))
		{
			retVal = std::wstring(des);
			SetErrorInfo(0, pIErrorInfo);
			SysFreeString(des);
		}
		pIErrorInfo->Release();
	}
	else if(SL_IS_LIC_HR(hr))
	{
		//Detect a Solimar License error
		retVal = GetECMessage(SL_EC_FROM_EHR(hr));
	}
	else
	{
		HRESULT tempHr = ((hr & 0x80070000) == 0x80070000) ? (hr & 0x0000FFFF) : hr;
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
		while(tempMsg[index-1]==L'\n' || tempMsg[index-1]==L'\r')
		{
			tempMsg[index] = L'\0';
			index--;
		}
		retVal = std::wstring(tempMsg.substr(0, index));
		// Free the buffer.
		LocalFree(lpMsgBuf);
	}

	//TRACE(AnsiString(retVal + AnsiString(hrErrMsg)).c_str());
	return retVal + ((retVal.find_last_of(L"]") == std::wstring::npos) ? std::wstring(hrErrMsg) : std::wstring(L""));
}

// Tries to look up error by looking up hr in map...
std::wstring GenerateErrorMessage(HRESULT hr, std::wstring wstrHeaderMsg, bool bAppendExistingErrorInfo)
{
	wchar_t hrErrMsg[256];
	#if _MSC_VER >= 1400
		swprintf_s(hrErrMsg, sizeof(hrErrMsg)/sizeof(wchar_t), L" [0x%08X]", hr);
	#else
		swprintf(hrErrMsg, L" [0x%08X]", hr);
	#endif
	std::wstring wstrRetVal(L"Unknown Error");
	std::wstring wstrAppendErrorMsg(L"");
	std::wstring wstrHeaderErrorMsg(wstrHeaderMsg);

	if(hr == -1)
	{
		wstrHeaderErrorMsg = L"";
		wstrRetVal = wstrHeaderMsg;
	}
	else
	{
		if(SL_IS_LIC_HR(hr))
		{
			//Detect a Solimar License error
			wstrRetVal = GetECMessage(SL_EC_FROM_EHR(hr));
		}
		else
		{
			HRESULT tempHr = ((hr & 0x80070000) == 0x80070000) ? (hr & 0x0000FFFF) : hr;
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
			while(tempMsg[index-1]==L'\n' || tempMsg[index-1]==L'\r')
			{
				tempMsg[index] = L'\0';
				index--;
			}
			wstrRetVal = std::wstring(tempMsg.substr(0, index));
			// Free the buffer.
			LocalFree(lpMsgBuf);
		}
	}

	bool bUseForMainMessage = (_wcsicmp(wstrRetVal.c_str(), L"Unknown Error")==0);
	if(bUseForMainMessage || bAppendExistingErrorInfo)
	{
		IErrorInfo* pIErrorInfo = NULL;
		if (GetErrorInfo(0, &pIErrorInfo) == S_OK)
		{
			BSTR des;
			if (SUCCEEDED(pIErrorInfo->GetDescription(&des)))
			{
				if(bUseForMainMessage)
					wstrRetVal = std::wstring(des);
				else if(bAppendExistingErrorInfo)
				{
					if(wcsstr(std::wstring(des).c_str(), wstrRetVal.c_str())== NULL)
						wstrAppendErrorMsg = std::wstring(des); // Only append if message is different.
					else
						wstrRetVal = std::wstring(des); // Use the longer message
				}
				else if(bAppendExistingErrorInfo && (_wcsicmp(wstrRetVal.c_str(), std::wstring(des).c_str()) != 0))
					wstrAppendErrorMsg = std::wstring(des); // Only append if message is different.
				SetErrorInfo(0, pIErrorInfo);
				SysFreeString(des);
			}
			pIErrorInfo->Release();
		}
	}

	std::wstring tmpStr = ((wstrHeaderErrorMsg.length() == 0) ? L"" : wstrHeaderErrorMsg + L"\r\n - ") + 
		std::wstring(wstrRetVal) + 
		((wstrAppendErrorMsg.length() == 0) ? L"" : L"\r\n - " + wstrAppendErrorMsg)
		;
	//OutputDebugString(std::wstring(std::wstring(L"GenerateErrorMessage(): ") + tmpStr).c_str());
	return tmpStr;
}

/*
 *
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
	return std::wstring(SLErrors[0].message); //Unknown
}
};
