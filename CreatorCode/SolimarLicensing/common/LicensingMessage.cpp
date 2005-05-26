#include "LicensingMessage.h"


LicensingMessage::LicensingMessage()
{
	;
}

LicensingMessage::LicensingMessage(std::wstring &key, DWORD msg_type, HRESULT hr, VARIANT msg_time, std::wstring &msg) : 
	key_ident(key),
	message_type(msg_type),
	error(hr),
	timestamp(msg_time),
	message(msg)
{
	;
}

LicensingMessage::LicensingMessage(const LicensingMessage &m) : 
	key_ident(m.key_ident),
	message_type(m.message_type),
	error(m.error),
	timestamp(m.timestamp),
	message(m.message)
{
	;
}

LicensingMessage::LicensingMessage(const VARIANT &vtMessage)
{
	if ((vtMessage.vt & VT_ARRAY) && (vtMessage.vt & VT_VARIANT))
	{
		if (vtMessage.parray->rgsabound[0].cElements==5)
		{
			VARIANT *pElement = 0;
			if (SUCCEEDED(SafeArrayAccessData(vtMessage.parray, (void**)&pElement)))
			{
				key_ident = pElement[0].bstrVal;
				message_type = pElement[1].ulVal;
				error = pElement[2].ulVal;
				timestamp = pElement[3];
				message = pElement[4].bstrVal;
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
	vt.parray = SafeArrayCreateVector(VT_VARIANT, 0, 5);
	if (SUCCEEDED(SafeArrayAccessData(vt.parray, (void**)&pElement)))
	{
		VariantClear(&pElement[0]);
		pElement[0].vt = VT_BSTR;
		pElement[0].bstrVal = SysAllocString(key_ident.c_str());
		VariantClear(&pElement[1]);
		pElement[1].vt = VT_UI4;
		pElement[1].ulVal = (unsigned long)(message_type);
		VariantClear(&pElement[2]);
		pElement[2].vt = VT_UI4;
		pElement[2].ulVal = (unsigned long)(error);
		VariantCopy(&pElement[3], &timestamp);
		VariantClear(&pElement[4]);
		pElement[4].vt = VT_BSTR;
		pElement[4].bstrVal = SysAllocString(message.c_str());
		
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
	//xxx make this a bit better
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

