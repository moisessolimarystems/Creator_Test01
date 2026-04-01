#pragma once
#ifndef __Licensing_Software_Message_Header_
#define __Licensing_Software_Message_Header_

#pragma warning(disable:4786)
#include <deque>
#include <string>
#include <windows.h>

class LicensingSoftwareMessage
{
	public:

		LicensingSoftwareMessage();
		LicensingSoftwareMessage(	std::wstring &_licenseServer,
											std::wstring &_softwareLicense,
											DWORD _productId,
											DWORD _messageType,
											DWORD _messageId,
											VARIANT _messageTimestamp,
											std::wstring &_message,
											HRESULT _messageErrorHr
											);
		LicensingSoftwareMessage(const LicensingSoftwareMessage &sMsg);
		LicensingSoftwareMessage(const VARIANT &vtsMsg);
		
		operator VARIANT();
		operator std::wstring();

		std::wstring licenseServer;
		std::wstring softwareLicense;
		DWORD productId;
		DWORD messageId;
		VARIANT messageTimestamp;
		DWORD messageType;
		std::wstring message;
		HRESULT messageErrorHr;
		
		//// trial info
		//DWORD key_status;
		//VARIANT key_expire_date;
		//DWORD key_trial_hours;
};

class LicensingSoftwareMessageList : public std::deque<LicensingSoftwareMessage>
{
	public:
		LicensingSoftwareMessageList();
		LicensingSoftwareMessageList(const VARIANT &vtMessageSoftwareList);
		operator VARIANT();
};

#endif