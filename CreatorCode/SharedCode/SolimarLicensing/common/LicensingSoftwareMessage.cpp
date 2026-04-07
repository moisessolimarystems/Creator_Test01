#include "LicensingSoftwareMessage.h"
#include "TimeHelper.h"																					// CR.32662.V06; added.

LicensingSoftwareMessage::LicensingSoftwareMessage()
{
	;
}

LicensingSoftwareMessage::LicensingSoftwareMessage(
	const CSpdCString &licenseServer, 																// CR.32662; modified.
	const CSpdCString &softwareLicense, 															// CR.32662; modified.
	DWORD productId,
	DWORD messageType,
	DWORD messageId,
	CFILETIME messageTimestamp,																		// CR.32662.V06; modified.
	const CSpdCString &message, 																		// CR.32662; modified.
	HRESULT messageErrorHr) :
	m_licenseServer(licenseServer),
	m_softwareLicense(softwareLicense),
	m_productId(productId),
	m_messageId(messageId),
	m_messageType(messageType),
	m_messageTimestamp(messageTimestamp),
	m_message(message),
	m_messageErrorHr(messageErrorHr)
{
	;
}

LicensingSoftwareMessage::LicensingSoftwareMessage(
	const LicensingSoftwareMessage &m) : 
	m_licenseServer(m.m_licenseServer),
	m_softwareLicense(m.m_softwareLicense),
	m_productId(m.m_productId),
	m_messageId(m.m_messageId),
	m_messageType(m.m_messageType),
	m_messageTimestamp(m.m_messageTimestamp),
	m_message(m.m_message),
	m_messageErrorHr(m.m_messageErrorHr)
{
	;
}

/* LicensingSoftwareMessage() - VARIANT copy constructor over-load
 *
 * SLB 14.mar.2025 CR.32662.V06; Modified to convert time in VARIANT to the 
 * new CFILETIME member type.
 * SLB 27.aug.2025 CR.34403; Refactored code to be flatter. No functional changes
 * intended.
 -----------------------------------------------------------------------------------*/
LicensingSoftwareMessage::LicensingSoftwareMessage(
	const VARIANT &vtSoftwareMsg)
{
	if (!(vtSoftwareMsg.vt & VT_ARRAY)) return;													// CR.34403; modified.
	if (!(vtSoftwareMsg.vt & VT_VARIANT)) return;												// CR.34403; modified.
	if (vtSoftwareMsg.parray->rgsabound[0].cElements < 8) return;							// CR.34403; modified.

	HRESULT hr;
	VARIANT* pElement = 0;
	hr = SafeArrayAccessData(vtSoftwareMsg.parray, (void**)&pElement);
	if (FAILED(hr)) return;

	m_licenseServer = pElement[0].bstrVal;
	m_softwareLicense = pElement[1].bstrVal;
	m_productId = pElement[2].ulVal;
	m_messageId = pElement[3].ulVal;
	m_messageType = pElement[4].ulVal;
	m_messageTimestamp = TimeHelper::VariantToTimeT(pElement[5]);							// CR.32662.V06; modified.
	m_message = pElement[6].bstrVal;
	m_messageErrorHr = pElement[7].ulVal;

	SafeArrayUnaccessData(vtSoftwareMsg.parray);
}

/* operator VARIANT()
 *
 * SLB 14.mar.2025 CR.32662.V06; Modified to convert the new CFILETIME member type
 * into a VARIANT.
 * SLB 27.aug.2025 CR.34403; Refactored code to be flatter. No functional changes
 * intended.
 -----------------------------------------------------------------------------------*/
LicensingSoftwareMessage::operator VARIANT()
{
	VARIANT vt;
	VariantInit(&vt);
	vt.vt = VT_ARRAY | VT_VARIANT;
	vt.parray = SafeArrayCreateVector(VT_VARIANT, 0, 8);

	HRESULT hr;
	VARIANT* pElement;
	hr = SafeArrayAccessData(vt.parray, (void**)&pElement);
	if (FAILED(hr)) return vt;

	CComVariant(m_licenseServer.c_str()).Detach(pElement + 0);
	CComVariant(m_softwareLicense.c_str()).Detach(pElement + 1);
	CComVariant(m_productId).Detach(pElement + 2);
	CComVariant(m_messageId).Detach(pElement + 3);
	CComVariant(m_messageType).Detach(pElement + 4);
	CComVariant(TimeHelper::TimeTToVariant(m_messageTimestamp)).Detach(pElement + 5);
	CComVariant(m_message.c_str()).Detach(pElement + 6);
	CComVariant((DWORD)m_messageErrorHr).Detach(pElement + 7);

	SafeArrayUnaccessData(vt.parray);
	return vt;
}

LicensingSoftwareMessage::operator CSpdCString()
{
	return m_message;
}


LicensingSoftwareMessageList::LicensingSoftwareMessageList()
{
	;
}

/* LicensingSoftwareMessageList() constructor.
 *
 * SLB 27.aug.2025 CR.34403; Refactored code to be flatter. No functional changes
 * intended.
 -----------------------------------------------------------------------------------*/
LicensingSoftwareMessageList::LicensingSoftwareMessageList(
	const VARIANT &vtMsgList)
{
	clear();

	if (!(vtMsgList.vt & VT_ARRAY)) return;
	if (!(vtMsgList.vt & VT_VARIANT)) return;

	HRESULT hr;
	VARIANT* pElement = 0;
	hr = SafeArrayAccessData(vtMsgList.parray, (void**)&pElement);
	if (FAILED(hr)) return;

	for (DWORD i = 0; i < vtMsgList.parray->rgsabound[0].cElements; ++i)
	{
		push_back(pElement[i]);
	}
	SafeArrayUnaccessData(vtMsgList.parray);
}

/* operator VARIANT()
 *
 * SLB 27.aug.2025 CR.34403; Refactored code to be flatter. No functional changes
 * intended.
 -----------------------------------------------------------------------------------*/
LicensingSoftwareMessageList::operator VARIANT()
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