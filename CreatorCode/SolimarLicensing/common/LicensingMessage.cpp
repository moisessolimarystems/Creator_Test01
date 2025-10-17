#include "LicensingMessage.h"


LicensingMessage::LicensingMessage()
{
	;
}

LicensingMessage::LicensingMessage(std::wstring &server, std::wstring &key, VARIANT msg_time, DWORD msg_type, DWORD msg_id, std::wstring &msg, HRESULT hr, DWORD status, VARIANT expire_date, DWORD trial_hours) : 
	key_ident(key),
	key_server(server),
	timestamp(msg_time),
	message_type(msg_type),
	message_id(msg_id),
	message(msg),
	error(hr),
	key_status(status),
	key_expire_date(expire_date),
	key_trial_hours(trial_hours)
{
	;
}

LicensingMessage::LicensingMessage(const LicensingMessage &m) : 
	key_ident(m.key_ident),
	key_server(m.key_server),
	timestamp(m.timestamp),
	message_type(m.message_type),
	message_id(m.message_id),
	message(m.message),
	error(m.error),
	key_status(m.key_status),
	key_expire_date(m.key_expire_date),
	key_trial_hours(m.key_trial_hours)
{
	;
}

LicensingMessage::LicensingMessage(const VARIANT &vtMessage)
{
	if ((vtMessage.vt & VT_ARRAY) && (vtMessage.vt & VT_VARIANT))
	{
		if (vtMessage.parray->rgsabound[0].cElements>=10)
		{
			VARIANT *pElement = 0;
			if (SUCCEEDED(SafeArrayAccessData(vtMessage.parray, (void**)&pElement)))
			{
				key_server = pElement[0].bstrVal;
				key_ident = pElement[1].bstrVal;
				timestamp = pElement[2];
				message_type = pElement[3].ulVal;
				message_id = pElement[4].ulVal;
				message = pElement[5].bstrVal;
				error = pElement[6].ulVal;
				key_status = pElement[7].ulVal;
				key_expire_date = pElement[8];
				key_trial_hours = pElement[9].ulVal;
				SafeArrayUnaccessData(vtMessage.parray);
			}			
		}
	}
}

LicensingMessage::operator VARIANT()
{
	VARIANT vt, *pElement = 0;
	VariantInit(&vt);
	vt.vt = VT_ARRAY | VT_VARIANT;
	vt.parray = SafeArrayCreateVector(VT_VARIANT, 0, 10);
	if (SUCCEEDED(SafeArrayAccessData(vt.parray, (void**)&pElement)))
	{
		VariantClear(&pElement[0]);
		pElement[0].vt = VT_BSTR;
		pElement[0].bstrVal = SysAllocString(key_server.c_str());
		VariantClear(&pElement[1]);
		pElement[1].vt = VT_BSTR;
		pElement[1].bstrVal = SysAllocString(key_ident.c_str());
		VariantCopy(&pElement[2], &timestamp);
		VariantClear(&pElement[3]);
		pElement[3].vt = VT_UI4;
		pElement[3].ulVal = (unsigned long)(message_type);
		VariantClear(&pElement[4]);
		pElement[4].vt = VT_UI4;
		pElement[4].ulVal = (unsigned long)(message_id);
		VariantClear(&pElement[5]);
		pElement[5].vt = VT_BSTR;
		pElement[5].bstrVal = SysAllocString(message.c_str());
		VariantClear(&pElement[6]);
		pElement[6].vt = VT_UI4;
		pElement[6].ulVal = (unsigned long)(error);
		VariantClear(&pElement[7]);
		pElement[7].vt = VT_UI4;
		pElement[7].ulVal = (unsigned long)(key_status);
		VariantCopy(&pElement[8], &key_expire_date);
		VariantClear(&pElement[9]);
		pElement[9].vt = VT_UI4;
		pElement[9].ulVal = (unsigned long)(key_trial_hours);
		
		SafeArrayUnaccessData(vt.parray);
	}
	else
	{
		VariantClear(&vt);
		VariantInit(&vt);
	}
	return vt;
}

LicensingMessage::operator std::wstring()
{
	return message;
}


LicensingMessageList::LicensingMessageList()
{
	;
}

LicensingMessageList::LicensingMessageList(const VARIANT &vtMessageList)
{
	clear();

	if ((vtMessageList.vt & VT_ARRAY) && (vtMessageList.vt & VT_VARIANT))
	{
		VARIANT *pElement = 0;
		if (SUCCEEDED(SafeArrayAccessData(vtMessageList.parray, (void**)&pElement)))
		{
			for (DWORD i=0; i<vtMessageList.parray->rgsabound[0].cElements; ++i)
			{
				push_back(pElement[i]);
			}
			SafeArrayUnaccessData(vtMessageList.parray);
		}
	}
}

LicensingMessageList::operator VARIANT()
{
	VARIANT vt, *pElement = 0;
	VariantInit(&vt);
	vt.vt = VT_ARRAY | VT_VARIANT;
	vt.parray = SafeArrayCreateVector(VT_VARIANT, 0, (ULONG)size());

	DWORD i=0;
	if (SUCCEEDED(SafeArrayAccessData(vt.parray, (void**)&pElement)))
	{
		for (iterator it = begin(); it != end(); ++it, ++i)
		{
			pElement[i] = (VARIANT)(*it);
		}
		SafeArrayUnaccessData(vt.parray);
	}
	return vt;
}

