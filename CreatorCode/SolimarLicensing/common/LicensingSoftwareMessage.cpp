#include "LicensingSoftwareMessage.h"


LicensingSoftwareMessage::LicensingSoftwareMessage()
{
	;
}

LicensingSoftwareMessage::LicensingSoftwareMessage(std::wstring &_licenseServer,
																	std::wstring &_softwareLicense,
																	DWORD _productId,
																	DWORD _messageId,
																	DWORD _messageType,
																	VARIANT _messageTimestamp,
																	std::wstring &_message,
																	HRESULT _messageErrorHr
																	) :
	licenseServer(_licenseServer),
	softwareLicense(_softwareLicense),
	productId(_productId),
	messageId(_messageId),
	messageType(_messageType),
	messageTimestamp(_messageTimestamp),
	message(_message),
	messageErrorHr(_messageErrorHr)
{
	;
}

LicensingSoftwareMessage::LicensingSoftwareMessage(const LicensingSoftwareMessage &m) : 
	licenseServer(m.licenseServer),
	softwareLicense(m.softwareLicense),
	productId(m.productId),
	messageId(m.messageId),
	messageType(m.messageType),
	messageTimestamp(m.messageTimestamp),
	message(m.message),
	messageErrorHr(m.messageErrorHr)
{
	;
}

LicensingSoftwareMessage::LicensingSoftwareMessage(const VARIANT &vtSoftwareMessage)
{
	if ((vtSoftwareMessage.vt & VT_ARRAY) && (vtSoftwareMessage.vt & VT_VARIANT))
	{
		if (vtSoftwareMessage.parray->rgsabound[0].cElements>=8)
		{
			VARIANT *pElement = 0;
			if (SUCCEEDED(SafeArrayAccessData(vtSoftwareMessage.parray, (void**)&pElement)))
			{
				licenseServer = pElement[0].bstrVal;
				softwareLicense = pElement[1].bstrVal;
				productId = pElement[2].ulVal;
				messageId = pElement[3].ulVal;
				messageType = pElement[4].ulVal;
				messageTimestamp = pElement[5];
				message = pElement[6].bstrVal;
				messageErrorHr = pElement[7].ulVal;
				SafeArrayUnaccessData(vtSoftwareMessage.parray);
			}			
		}
	}
}

LicensingSoftwareMessage::operator VARIANT()
{
	VARIANT vt, *pElement = 0;
	VariantInit(&vt);
	vt.vt = VT_ARRAY | VT_VARIANT;
	vt.parray = SafeArrayCreateVector(VT_VARIANT, 0, 8);
	if (SUCCEEDED(SafeArrayAccessData(vt.parray, (void**)&pElement)))
	{
		VariantClear(&pElement[0]);
		pElement[0].vt = VT_BSTR;
		pElement[0].bstrVal = SysAllocString(licenseServer.c_str());
		VariantClear(&pElement[1]);
		pElement[1].vt = VT_BSTR;
		pElement[1].bstrVal = SysAllocString(softwareLicense.c_str());
		VariantClear(&pElement[2]);
		pElement[2].vt = VT_UI4;
		pElement[2].ulVal = (unsigned long)(productId);
		VariantClear(&pElement[3]);
		pElement[3].vt = VT_UI4;
		pElement[3].ulVal = (unsigned long)(messageId);
		VariantClear(&pElement[4]);
		pElement[4].vt = VT_UI4;
		pElement[4].ulVal = (unsigned long)(messageType);
		VariantCopy(&pElement[5], &messageTimestamp);
		VariantClear(&pElement[6]);
		pElement[6].vt = VT_BSTR;
		pElement[6].bstrVal = SysAllocString(message.c_str());
		VariantClear(&pElement[7]);
		pElement[7].vt = VT_UI4;
		pElement[7].ulVal = (unsigned long)(messageErrorHr);
		SafeArrayUnaccessData(vt.parray);
	}
	else
	{
		VariantClear(&vt);
		VariantInit(&vt);
	}
	return vt;
}

LicensingSoftwareMessage::operator std::wstring()
{
	return message;
}


LicensingSoftwareMessageList::LicensingSoftwareMessageList()
{
	;
}

LicensingSoftwareMessageList::LicensingSoftwareMessageList(const VARIANT &vtSoftwareMessageList)
{
	clear();

	if ((vtSoftwareMessageList.vt & VT_ARRAY) && (vtSoftwareMessageList.vt & VT_VARIANT))
	{
		VARIANT *pElement = 0;
		if (SUCCEEDED(SafeArrayAccessData(vtSoftwareMessageList.parray, (void**)&pElement)))
		{
			for (DWORD i=0; i<vtSoftwareMessageList.parray->rgsabound[0].cElements; ++i)
			{
				push_back(pElement[i]);
			}
			SafeArrayUnaccessData(vtSoftwareMessageList.parray);
		}
	}
}

LicensingSoftwareMessageList::operator VARIANT()
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