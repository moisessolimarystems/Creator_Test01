#pragma once

#pragma warning(disable:4786)
#include <deque>
#include <string>
#include <windows.h>
#include "FromSpd\L_SpdStreamable4.h"																// CR.32662.V03; added.

using namespace SpdStreamable;																		// CR.32662.V03; added.

/* class LicensingSoftwareMessage
 * 
 * SLB CR.32662; Converted BSTR members and params to CSpdCString. So calling
 * code does not have to concern itself with COM type handling.
 * SLB 14.mar.2025 CR.32662.V06; converted timestamp member from a VARIANT to 
 * CFILETIME. So calling code does not have to concern itself with COM type 
 * handling.
 */
class LicensingSoftwareMessage
{
	public:

		LicensingSoftwareMessage();
		LicensingSoftwareMessage(	
			const CSpdCString &licenseServer, 														// CR.32662; modified.
			const CSpdCString &softwareLicense, 													// CR.32662; modified.
			DWORD productId,
			DWORD messageType,
			DWORD messageId,
			CFILETIME messageTimestamp,																// CR.32662.V06; modified.
			const CSpdCString &message, 																// CR.32662; modified.
			HRESULT messageErrorHr);
		LicensingSoftwareMessage(
			const LicensingSoftwareMessage &sMsg);
		LicensingSoftwareMessage(
			const VARIANT &vtsMsg);
		
		operator VARIANT();
		operator CSpdCString();

		CSpdCString m_licenseServer;
		CSpdCString m_softwareLicense;
		DWORD m_productId;
		DWORD m_messageId;
		CFILETIME m_messageTimestamp;																	// CR.32662.V06; modified.
		DWORD m_messageType;
		CSpdCString m_message;
		HRESULT m_messageErrorHr;
		
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

