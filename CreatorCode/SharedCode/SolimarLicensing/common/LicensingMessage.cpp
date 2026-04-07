#include "LicensingMessage.h"
#include "TimeHelper.h"

LicensingMessage::LicensingMessage()
{
	;
}

/* LicensingMessage()
 *
 * SLB 14.mar.2025 Convert usage of VARIANTs for time to CFILETIME.
------------------------------------------------------------------------------------*/
LicensingMessage::LicensingMessage(
	const CSpdCString& server,																			// CR.32662; modified.
	const CSpdCString& key, 																			// CR.32662; modified.
	const CFILETIME& msg_time, 																		// CR.32662.V06; modified.
	DWORD msg_type, 
	DWORD msg_id, 
	const CSpdCString& msg,																				// CR.32662; modified.
	HRESULT hr, 
	DWORD status, 
	const CFILETIME& expire_date, 																	// CR.32662.V06; modified.
	DWORD trial_hours) : 
	m_key_ident(key),
	m_key_server(server),
	m_timestamp(msg_time),
	m_message_type(msg_type),
	m_message_id(msg_id),
	m_message(msg),
	m_error(hr),
	m_key_status(status),
	m_key_expire_date(expire_date),
	m_key_trial_hours(trial_hours)
{
	;
}

LicensingMessage::LicensingMessage(
	const LicensingMessage &m) : 
	m_key_ident(m.m_key_ident),
	m_key_server(m.m_key_server),
	m_timestamp(m.m_timestamp),
	m_message_type(m.m_message_type),
	m_message_id(m.m_message_id),
	m_message(m.m_message),
	m_error(m.m_error),
	m_key_status(m.m_key_status),
	m_key_expire_date(m.m_key_expire_date),
	m_key_trial_hours(m.m_key_trial_hours)
{
	;
}

/* LicensingMessage() - VARIANT copy constructor over-load
 *
 * SLB 14.mar.2025 CR.32662.V06; Modified to convert time in VARIANT to the
 * new CFILETIME member type.
 * SLB 27.aug.2025 CR.34403; Refactored code to be flatter. No functional changes 
 * intended.
 -----------------------------------------------------------------------------------*/
LicensingMessage::LicensingMessage(
	const VARIANT &vtMessage)
{
	if (!(vtMessage.vt & VT_ARRAY)) return;														// CR.34403; modified.
	if (!(vtMessage.vt & VT_VARIANT)) return;														// CR.34403; modified.
	if (vtMessage.parray->rgsabound[0].cElements < 10) return;								// CR.34403; modified.

	HRESULT hr;
	VARIANT* pElement = 0;
	hr = SafeArrayAccessData(vtMessage.parray, (void**)&pElement);
	if (FAILED(hr)) return;

	m_key_server = pElement[0].bstrVal;
	m_key_ident = pElement[1].bstrVal;
	m_timestamp = TimeHelper::VariantToTimeT(pElement[2]);									// CR.32662.V06; modified.
	m_message_type = pElement[3].ulVal;
	m_message_id = pElement[4].ulVal;
	m_message = pElement[5].bstrVal;
	m_error = pElement[6].ulVal;
	m_key_status = pElement[7].ulVal;
	m_key_expire_date = TimeHelper::VariantToTimeT(pElement[8]);							// CR.32662.V06; modified.
	m_key_trial_hours = pElement[9].ulVal;

	SafeArrayUnaccessData(vtMessage.parray);
}

/* operator VARIANT()
 *
 * SLB 14.mar.2025 CR.32662.V06; Modified to convert the new CFILETIME member type
 * into a VARIANT.
 * SLB 27.aug.2025 CR.34403; Refactored code to be flatter. No functional changes 
 * intended.
 -----------------------------------------------------------------------------------*/
LicensingMessage::operator VARIANT()
{
	// CR.34403 code.
	VARIANT vt;
	VariantInit(&vt);
	vt.vt = VT_ARRAY | VT_VARIANT;
	vt.parray = SafeArrayCreateVector(VT_VARIANT, 0, 10);

	HRESULT hr;
	VARIANT *pElement;
	hr = SafeArrayAccessData(vt.parray, (void**)&pElement);
	if (FAILED(hr)) return vt;

	CComVariant(m_key_server.c_str()).Detach(pElement + 0);
	CComVariant(m_key_ident.c_str()).Detach(pElement + 1);
	CComVariant(TimeHelper::TimeTToVariant(m_timestamp)).Detach(pElement + 2);
	CComVariant(m_message_type).Detach(pElement + 3);
	CComVariant(m_message_id).Detach(pElement + 4);
	CComVariant(m_message.c_str()).Detach(pElement + 5);
	CComVariant((DWORD)m_error).Detach(pElement + 6);
	CComVariant(m_key_status).Detach(pElement + 7);
	CComVariant(TimeHelper::TimeTToVariant(m_key_expire_date)).Detach(pElement + 8);
	CComVariant(m_key_trial_hours).Detach(pElement + 9);

	SafeArrayUnaccessData(vt.parray);
	return vt;
}

LicensingMessage::operator CSpdCString()															// CR.32662.V06; modified.
{
	return m_message;
}


LicensingMessageList::LicensingMessageList()
{
	;
}

/* LicensingMessageList() constructor.
 *
 * SLB 27.aug.2025 CR.34403; Refactored code to be flatter. No functional changes
 * intended.
 -----------------------------------------------------------------------------------*/
LicensingMessageList::LicensingMessageList(
	const VARIANT &vtMessageList)
{
	clear();

	if (!(vtMessageList.vt & VT_ARRAY)) return;
	if (!(vtMessageList.vt & VT_VARIANT)) return;

	HRESULT hr;
	VARIANT* pElement = 0;
	hr = SafeArrayAccessData(vtMessageList.parray, (void**)&pElement);
	if (FAILED(hr)) return;

	for (DWORD i = 0; i < vtMessageList.parray->rgsabound[0].cElements; ++i)
	{
		push_back(pElement[i]);
	}
	SafeArrayUnaccessData(vtMessageList.parray);
}

/* operator VARIANT()
 *
 * SLB 27.aug.2025 CR.34403; Refactored code to be flatter. No functional changes
 * intended.
 -----------------------------------------------------------------------------------*/
LicensingMessageList::operator VARIANT()
{
	VARIANT vt;
	VariantInit(&vt);
	vt.vt = VT_ARRAY | VT_VARIANT;
	vt.parray = SafeArrayCreateVector(VT_VARIANT, 0, (ULONG)size());
	if (empty()) return vt;

	HRESULT hr;
	VARIANT* pElement(NULL);
	hr = SafeArrayAccessData(vt.parray, (void**)&pElement);
	if (FAILED(hr)) return vt;


	DWORD i = 0;
	for (iterator it = begin(); it != end(); ++it, ++i)
	{
		pElement[i] = (VARIANT)(*it);
	}
	SafeArrayUnaccessData(vt.parray);

	return vt;
}

