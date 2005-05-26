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
	LicensingMessage(std::wstring &key, DWORD msg_type, HRESULT hr, VARIANT msg_time, std::wstring &msg);
	LicensingMessage(const LicensingMessage &m);
	LicensingMessage(const VARIANT &vtMessage);
	
	operator VARIANT();
	operator std::wstring();
	
	std::wstring key_ident;
	DWORD message_type;
	HRESULT error;
	VARIANT timestamp;
	std::wstring message;
};

class LicensingMessageList : public std::deque<LicensingMessage>
{
public:
	LicensingMessageList();
	LicensingMessageList(const VARIANT &vtMessageList);
	operator VARIANT();
};

#endif
