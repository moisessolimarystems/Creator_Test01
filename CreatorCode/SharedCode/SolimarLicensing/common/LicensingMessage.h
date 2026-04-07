#pragma once

#pragma warning(disable:4786)
#include <deque>
#include <string>
#include <windows.h>
#include "FromSpd\L_SpdStreamable4.h"

using namespace SpdStreamable;


class LicensingMessage
{
public:
	CSpdCString m_key_server;
	CSpdCString m_key_ident;
	CFILETIME m_timestamp;  																			// CR.32662.V06; modified.

	DWORD m_message_type;
	DWORD m_message_id;
	CSpdCString m_message;
	HRESULT m_error;

	// trial info
	DWORD m_key_status;
	CFILETIME m_key_expire_date;  																	// CR.32662.V06; modified.
	DWORD m_key_trial_hours; 

public:
	LicensingMessage();
	LicensingMessage(
		const CSpdCString& server, 																	// CR.32662; modified.
		const CSpdCString& key, 																		// CR.32662; modified.
		const CFILETIME& msg_time, 																	// CR.32662.V06; modified.
		DWORD msg_type, 
		DWORD msg_id, 
		const CSpdCString& msg, 																		// CR.32662; modified.
		HRESULT hr, 
		DWORD status, 
		const CFILETIME &expire_date, 																// CR.32662.V06; modified.
		DWORD trial_hours);
	LicensingMessage(
		const LicensingMessage &m);
	LicensingMessage(
		const VARIANT &vtMessage);
	
	operator VARIANT();
	operator CSpdCString();  																			// CR.32662.V06; modified.

};

class LicensingMessageList : public std::deque<LicensingMessage>
{
public:
	LicensingMessageList();
	LicensingMessageList(const VARIANT &vtMessageList);
	operator VARIANT();
};
