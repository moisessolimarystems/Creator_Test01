#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#include "LicenseController.h"
#include "..\common\LicenseError.h"
#include "..\common\TimeHelper.h"

void LicenseController::UpdateKeysThreadFunction(void* pvThis)
{
//OutputDebugStringW(L"LicenseController::UpdateKeysThreadFunction() - Enter");
	HRESULT hr = S_OK;
	LicenseController *pThis = (LicenseController*)pvThis;
	hr = pThis->UpdateKeys();
//OutputDebugStringW(L"LicenseController::UpdateKeysThreadFunction() - Leave");
}

void LicenseController::HeartbeatCheckThreadFunction(void* pvThis)
{
//OutputDebugStringW(L"LicenseController::HeartbeatCheckThreadFunction() - Enter");
	HRESULT hr = S_OK;
	LicenseController *pThis = (LicenseController*)pvThis;
	pThis->HeartbeatCheck();
//OutputDebugStringW(L"LicenseController::HeartbeatCheckThreadFunction() - Leave");
}

void LicenseController::TimesUpThreadFunction(void* pvThis)
{
//OutputDebugStringW(L"LicenseController::TimesUpThreadFunction() - Enter");
	HRESULT hr = S_OK;
	LicenseController *pThis = (LicenseController*)pvThis;
	hr = pThis->TimesUp();
//OutputDebugStringW(L"LicenseController::TimesUpThreadFunction() - Leave");
}

LicenseController::LicenseController() : 
	HeartbeatListLock(CreateMutex(0,0,0)), 
	MessageClientListLock(CreateMutex(0,0,0)),
	USBNotification(NULL)//USBNotification(this)
{
	//wchar_t tmpbuf[1024];
	//swprintf_s(tmpbuf, 1024, L"LicenseController::LicenseController() - Enter, ThreadID: %d", GetCurrentThreadId());
	//OutputDebugString(tmpbuf);

	_tzset();

	//keyserver.Initialize(this.d);

	//this.keyserver.Initialize(&this.driver);
	//this.softwareServer.Initialize(&this.driver);
	this->keyserver.Initialize(&this->driver);
	this->softwareServer.Initialize(&this->keyserver, &this->driver);


	//UpdateKeysThread = new APCTimer(UpdateKeysThreadFunction, this, UpdateKeysThreadPeriod/6, InitTimerThreadCB, this);
	UpdateKeysThread = new APCTimer(UpdateKeysThreadFunction, this, UpdateKeysThreadPeriod, InitTimerThreadCB, this);
	HeartbeatCheckThread = new APCTimer(HeartbeatCheckThreadFunction, this, HeartbeatCheckThreadPeriod, InitTimerThreadCB, this);
	TimesUpThread = new APCTimer(TimesUpThreadFunction, this, TrialKeyDecrementCheckPeriod, InitTimerThreadCB, this);
	//UpdateKeysThread = new APCTimer(UpdateKeysThreadFunction, this, UpdateKeysThreadPeriod);
	//HeartbeatCheckThread = new APCTimer(HeartbeatCheckThreadFunction, this, HeartbeatCheckThreadPeriod);
	//TimesUpThread = new APCTimer(TimesUpThreadFunction, this, TrialKeyDecrementCheckPeriod);

	{
	SafeMutex mutex(HeartbeatListLock);
	heartbeats.clear();
	}


	// get the local host name for use in licensing messages
	wchar_t hostname[1024];
	DWORD hostname_size = 0;
	if (GetComputerNameEx(ComputerNameDnsHostname, hostname, &(hostname_size=1024)) || GetComputerNameEx(ComputerNameNetBIOS, hostname, &(hostname_size=1024)))
	{
		server_host_name = hostname;
	}
	
	// trigger a key list resynchronize
	//UpdateKeysThread->RevEnable(UpdateKeysThreadHighPeriodSeconds, UpdateKeysThreadLowPeriodSeconds, 20);
	UpdateKeysThread->RevEnable(10, 30, 20);

	UpdateKeysThread->Invoke();
}

// APCTimerInitialize()
//	
// This function is executed by the same thread that will 
// invoke ActivateInternal.
//
//*******************************************************************
bool LicenseController::InitTimerThreadCB(EInitReason reason, void *pArg) 
{
//wchar_t debug_buf[1024];
//_snwprintf(debug_buf, 1024, L"LicenseController::InitTimerThreadCB() ThreadID: %d", GetCurrentThreadId());
//OutputDebugStringW(debug_buf);
	if (reason == irStartup)
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
	else if (reason == irShutdown)
		CoUninitialize();

	return true;
}

LicenseController::~LicenseController()
{
	//wchar_t tmpbuf[1024];
	//swprintf_s(tmpbuf, 1024, L"LicenseController::~LicenseController() - Enter, ThreadID: %d", GetCurrentThreadId());
	//OutputDebugString(tmpbuf);

	if (UpdateKeysThread)
		delete UpdateKeysThread;
	if (HeartbeatCheckThread)
		delete HeartbeatCheckThread;
	if (TimesUpThread)
		delete TimesUpThread;
	if (MessageClientListLock!=NULL)
		CloseHandle(MessageClientListLock);
}



HRESULT LicenseController::GetLicenseServerTime(VARIANT *pvtSystemTime)
{
	// _variant_t( double dblSrc, VARTYPE vtSrc = VT_R8 );
	double dSysTime;
	SYSTEMTIME stSysTime;

	GetSystemTime(&stSysTime);
	if (SystemTimeToVariantTime(&stSysTime, &dSysTime))
	{
		*pvtSystemTime = _variant_t(dSysTime, VT_DATE).Detach();
		return S_OK;
	}
	else
	{
		VariantInit(pvtSystemTime);
		return E_FAIL;
	}
}

HRESULT LicenseController::GetLicenseMessageList(BSTR license_id, VARIANT *pvtMessageList)
{
	SafeMutex mutex(MessageClientListLock);
	
	*pvtMessageList = message_clients[_bstr_t(license_id, true)];
	message_clients[_bstr_t(license_id, true)].clear();
	
	return S_OK;
}

HRESULT LicenseController::GetSoftwareLicenseMessageList(BSTR license_id, VARIANT *pvtMessageList)
{
	SafeMutex mutex(MessageClientListLock);
	
	*pvtMessageList = message_software_clients[_bstr_t(license_id, true)];
	message_software_clients[_bstr_t(license_id, true)].clear();
	
	return S_OK;
}
void LicenseController::GenerateMessage(const wchar_t* key_ident, EMessageType message_type, HRESULT error, time_t timestamp, const unsigned int MessageLookupID, ...)
{
	static const int MAX_MESSAGE_SIZE = 0x2000;
	wchar_t message[MAX_MESSAGE_SIZE];
	va_list pArg;
	
	va_start(pArg, MessageLookupID);
	_vsnwprintf_s(message, _countof(message), LicensingMessageStringTable[MessageLookupID], pArg);
	va_end(pArg);
	message[_countof(message)-1] = 0;
	
	GenerateMessageInternal(key_ident, false/*Don't Generate Software License Message*/, -1, message_type, error, timestamp, MessageLookupID, message);
}
void LicenseController::GenerateSoftwareLicenseMessage(const wchar_t* softwareLicID, long productID, EMessageType messageType, HRESULT error, time_t timestamp, const unsigned int MessageLookupID, ...)
{
	static const int MAX_MESSAGE_SIZE = 0x2000;
	wchar_t message[MAX_MESSAGE_SIZE];
	va_list pArg;
	
	va_start(pArg, MessageLookupID);
	_vsnwprintf_s(message, _countof(message), LicensingMessageStringTable[MessageLookupID], pArg);
	va_end(pArg);
	message[_countof(message)-1] = 0;
	
	GenerateMessageInternal(softwareLicID, true/*Generate Software License Message*/, productID, messageType, error, timestamp, MessageLookupID, message);
}

void LicenseController::GenerateMessageInternal(const wchar_t* license_source, bool bGenerateSoftwareLicMsg, long productID, EMessageType message_type, HRESULT error, time_t timestamp, const unsigned int MessageLookupID, const wchar_t* message)
{
	static const int MAX_MESSAGE_SIZE = 0x2000;
	wchar_t event_log_msg[MAX_MESSAGE_SIZE];
	
	_variant_t vtTimestamp;
	vtTimestamp =  TimeHelper::TimeTToVariant(timestamp, false);
	
	/*
	_bstr_t str_timestamp;
	_bstr_t str_error_message;	// look in i:\chris r\samplehr.txt
	
	str_error_message = LicenseServerError::GetErrorMessage(error).c_str();
	
	// convert the date in to a string
	char cstr_timestamp[256];
	if (!GetTimeFormatA(LOCALE_USER_DEFAULT, TIME_FORCE24HOURFORMAT, &systimestamp, NULL, cstr_timestamp, 256))
		str_timestamp = cstr_timestamp;
	
	switch (message_type)
	{
	case MT_INFO:
		_snwprintf(event_log_msg, MAX_MESSAGE_SIZE, L"Solimar Systems, Inc.\r\nProduct Licensing Status Message\r\n%s\r\nKey: %s\r\n\r\n%s",
			str_timestamp.GetBSTR(),
			key_ident,
			message);
		break;
	case MT_ERROR:
		_snwprintf(event_log_msg, MAX_MESSAGE_SIZE, L"Solimar Systems, Inc.\r\nProduct Licensing Error Message\r\n%s\r\nKey: %s\r\n%08x %s\r\n\r\n%s",
			str_timestamp.GetBSTR(),
			key_ident,
			error,
			str_error_message,
			message);
		break;
	}
	*/


	//Ignore Timedate stamp for writing to event log, unnecessary because event log
	//keeps track of Timedate anyways.
	_bstr_t str_error_message;	// look in i:\chris r\samplehr.txt
	str_error_message = LicenseServerError::GetErrorMessage(error).c_str();
	
	// convert the date in to a string
	switch (message_type)
	{
	case MT_WARNING:
	case MT_INFO:
		_snwprintf_s(event_log_msg, sizeof(event_log_msg)/sizeof(wchar_t), L"Solimar Systems, Inc.\r\nProduct Licensing Status Message\r\n\r\nLicense: %s\r\n\r\n%s",
			license_source,
			message);
		break;
	case MT_ERROR:
		_snwprintf_s(event_log_msg, sizeof(event_log_msg)/sizeof(wchar_t), MAX_MESSAGE_SIZE, L"Solimar Systems, Inc.\r\nProduct Licensing Error Message\r\n\r\nLicense: %s\r\n%08x %s\r\n\r\n%s",
			license_source,
			error,
			str_error_message,
			message);
		break;
	}
	
	unsigned int event_type = EVENTLOG_INFORMATION_TYPE;
	if (error & 0x8000000) 
		event_type = EVENTLOG_ERROR_TYPE;
	else if(message_type == MT_WARNING)
		event_type = EVENTLOG_WARNING_TYPE;
	LicenseServerError::WriteEventLog(event_log_msg, event_type);
	
	// notify the clients of the message
	SafeMutex mutex(MessageClientListLock);
	
	try
	{
		if(bGenerateSoftwareLicMsg)
		{
			LicensingSoftwareMessage softwareLicMsg(
				std::wstring(server_host_name), 
				std::wstring(license_source), 
				productID,
				message_type,
				MessageLookupID, 
				vtTimestamp, 
				std::wstring(message), 
				error);
			for (SoftwareMessageClientList::iterator clientIt = message_software_clients.begin(); clientIt != message_software_clients.end(); ++clientIt)
			{
				clientIt->second.push_back(softwareLicMsg);
			}
		}
		else	//Generate a standard Licensing Message
		{
			//xxx might need to make this asynchronous
			//yyy client->second->DispatchLicenseMessage(_bstr_t(key_ident), message_type, error, vtTimestamp, _bstr_t(message));
			//zzz add message to the message lists for all clients
			//xxx need to pass in the message id that indicates which message this is (eg. "trial key expired" "cannot write to key", etc.)
			LicensingMessage m(std::wstring(server_host_name), std::wstring(license_source), vtTimestamp, message_type, MessageLookupID, std::wstring(message), error, 0, _variant_t(0.0,VT_DATE), 0);
			for (MessageClientList::iterator c = message_clients.begin(); c != message_clients.end(); ++c)
			{
				c->second.push_back(m);
				//xxx make sure that the message backlog is not excessive.
				//xxx if it is, cull some of the older messages
			}
		}
	}
	catch (_com_error &e)
	{
		e.Error();
	}
}







HRESULT LicenseController::Heartbeat(BSTR license_id)
{
	SafeMutex mutex(HeartbeatListLock);
	
	DWORD cur_time = (DWORD)time(0);	

//wchar_t debug_buf[1024];
//_snwprintf(debug_buf, 1024, L"LicenseController::Heartbeat (%s) cur_time=%d)", (BSTR)license_id, cur_time);
//debug_buf[1023] = 0;
//OutputDebugStringW(debug_buf);

	heartbeats[_bstr_t(license_id,true)]=cur_time;

	return S_OK;
}
HRESULT LicenseController::RemoveHeartbeat(BSTR license_id)
{
	SafeMutex mutex(HeartbeatListLock);
	for (HeartbeatList::iterator heartbeatIt = heartbeats.begin(); heartbeatIt != heartbeats.end(); ++heartbeatIt)
	{
		if(_wcsicmp(heartbeatIt->first, license_id) == 0)
		{
			heartbeats.erase(heartbeatIt);
			break;
		}
	}
	RemoveFromNotification(license_id);
	
	return S_OK;
}
HRESULT LicenseController::RemoveFromNotification(BSTR license_id)
{
	SafeMutex mutex(HeartbeatListLock);
	HRESULT hr = keyserver.LicenseReleaseAll(license_id);
	//Cycle through all keys removing the app instance, passing an empty string will remove the correct app instance...
	for (KeyList::iterator keyIt = keys.begin(); keyIt!=keys.end(); ++keyIt)
		keyIt->second->RemoveApplicationInstance(_bstr_t(license_id, true), L"");

	//Remove the app instance, a productID of -1 will remove all products that match license_id, 
	//passing an empty string will remove the correct app instance...
	softwareServer.RemoveApplicationInstance(-1, _bstr_t(license_id, true), L"");

	// Stop notifying the client of messages
	{
		SafeMutex mutex(MessageClientListLock);
		message_clients[_bstr_t(license_id, true)].clear();	//Erase all existing messages
		message_clients.erase(_bstr_t(license_id, true));		//Remove from notifying list

		message_software_clients[_bstr_t(license_id, true)].clear();	//Erase all existing messages
		message_software_clients.erase(_bstr_t(license_id, true));		//Remove from notifying list
	}
	return hr;
}


//
// Check licenses for softwareServer
void LicenseController::HeartbeatCheck()
{
	HRESULT hr = S_OK;
	SafeMutex mutex(HeartbeatListLock);

	HeartbeatList keepers;

	DWORD cur_time = (DWORD)time(0);
//wchar_t debug_buf1[1024];
//_snwprintf(debug_buf1, 1024, L"KeyServer::HeartbeatCheck Enter - NumberOfHeartBeats = %d", heartbeats.size());
//debug_buf1[1023] = 0;
//OutputDebugStringW(debug_buf1);

	for (HeartbeatList::iterator heartbeat = heartbeats.begin(); heartbeat != heartbeats.end(); ++heartbeat)
	{
//_snwprintf(debug_buf1, 1024, L"KeyServer::HeartbeatCheck (%s) (heartbeat->second %d + HeartbeatKillClientPeriod %d < cur_time %d)", (BSTR)heartbeat->first, heartbeat->second, HeartbeatKillClientPeriod, cur_time);
//debug_buf1[1023] = 0;
//OutputDebugStringW(debug_buf1);

		if (heartbeat->second + HeartbeatKillClientPeriod < cur_time)
		{
			//xxx debug
			wchar_t debug_buf[1024];
			//_snwprintf(debug_buf, 1024, L"LicenseServerError::EHR_CLIENT_TIMEOUT (%s) (heartbeat->second %d + HeartbeatKillClientPeriod %d < cur_time %d)", (BSTR)heartbeat->first, heartbeat->second, HeartbeatKillClientPeriod, cur_time);
			//_snwprintf_s(debug_buf, sizeof(debug_buf)/sizeof(wchar_t), L"LicenseServerError::EHR_CLIENT_TIMEOUT  (heartbeat->second %d + HeartbeatKillClientPeriod %d < cur_time %d)", heartbeat->second, HeartbeatKillClientPeriod, cur_time);
			_snwprintf_s(debug_buf, sizeof(debug_buf)/sizeof(wchar_t), L"LicenseServerError::EHR_CLIENT_TIMEOUT - LicenseID: %s - (heartbeat->second %d + HeartbeatKillClientPeriod %d < cur_time %d)", (wchar_t*)heartbeat->first, heartbeat->second, HeartbeatKillClientPeriod, cur_time);
			debug_buf[1023] = 0;
			OutputDebugStringW(debug_buf);

			GenerateMessage(L"", MT_INFO, LicenseServerError::EHR_CLIENT_TIMEOUT, time(0), MessageClientTimeout);
						
			hr = RemoveFromNotification(heartbeat->first);
		}
		else
		{
			HeartbeatList::iterator next_heartbeat = heartbeat;
			keepers.insert(heartbeat,++next_heartbeat);
		}
	}
	
	heartbeats = keepers;
//_snwprintf(debug_buf1, 1024, L"KeyServer::HeartbeatCheck Leave - NumberOfHeartBeats = %d", heartbeats.size());
//debug_buf1[1023] = 0;
//OutputDebugStringW(debug_buf1);

}

/*
	APCTimer *TimesUpThread;
	static void TimesUpThreadFunction(void* pvThis);
	typedef struct {time_t last_decrement; bool key_obtained;} TrialTimeInfo;
	typedef std::map<_bstr_t, time_t> TrialTimeInfoList;
	// called periodically by the TimesUpThread
	HRESULT TimesUp();
*/


HRESULT LicenseController::UpdateKeys()
{
	HRESULT hr(S_OK);
	{
	SafeMutex mutex1(driver.keys_lock);
	if(driver.AtLeastOneParallelKey())
		UpdateKeysThread->RevUp();	//Kick up how often looking for keys.
	}

	keyserver.ResynchronizeKeys();
	softwareServer.ResynchronizeSoftwareLicenses();
	return hr;
}

HRESULT LicenseController::TimesUp()
{
	//xxx check for expired keys?
	keyserver.TimesUp();

	//Sent out warning messages for close to expired product licenses...
	softwareServer.TimesUp();
	return S_OK;
}

// supports usb device insert/remove notification
void LicenseController::USBEventCallback(LPVOID pContext)
{
//OutputDebugStringW(L"KeyServer::USBEventCallback() - Enter");
//try
//{
//	OutputDebugStringW(L"KeyServer::USBEventCallback() - a");
	keyserver.ResynchronizeKeys(true);
//	OutputDebugStringW(L"KeyServer::USBEventCallback() - b");
	softwareServer.ResynchronizeSoftwareLicenses(true);
//	OutputDebugStringW(L"KeyServer::USBEventCallback() - c");
//}
//catch(...)
//{
//	OutputDebugStringW(L"KeyServer::USBEventCallback() - Unexpected Exception");
//}
//OutputDebugStringW(L"KeyServer::USBEventCallback() - Leave");
}
