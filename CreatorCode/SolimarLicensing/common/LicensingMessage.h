#pragma once
#ifndef __Licensing_Message_Header_BFD0F458_211E_4e2f_AC12_78BFADC11BF4__
#define __Licensing_Message_Header_BFD0F458_211E_4e2f_AC12_78BFADC11BF4__

#pragma warning(disable:4786)
#include <deque>
#include <string>
#include <windows.h>

class LicensingMessage
{
public:
	LicensingMessage();
	LicensingMessage(std::wstring &server, std::wstring &key, VARIANT msg_time, DWORD msg_type, DWORD msg_id, std::wstring &msg, HRESULT hr, DWORD status, VARIANT expire_date, DWORD trial_hours);
	LicensingMessage(const LicensingMessage &m);
	LicensingMessage(const VARIANT &vtMessage);
	
	operator VARIANT();
	operator std::wstring();
	
	std::wstring key_server;
	std::wstring key_ident;
	VARIANT timestamp;
	
	DWORD message_type;
	DWORD message_id;
	std::wstring message;
	HRESULT error;
	
	// trial info
	DWORD key_status;
	VARIANT key_expire_date;
	DWORD key_trial_hours;
};

class LicensingMessageList : public std::deque<LicensingMessage>
{
public:
	LicensingMessageList();
	LicensingMessageList(const VARIANT &vtMessageList);
	operator VARIANT();
};

#endif
