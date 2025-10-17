#ifndef __WMI_Helper_57908296812637134781638947_h__
#define __WMI_Helper_57908296812637134781638947_h__

#pragma warning(disable : 4786)
#include <vector>
#include <string>
#include <map>

#ifndef _WIN32_DCOM
#define _WIN32_DCOM
#endif

#include <ole2.h>
#include <Wbemidl.h>
#include <objidl.h>
#include "SecurityHelper.h"


class WMIHelper
{
public:
	/** Specifies the connection string to connect to the WMI locator object, (eg.: "\\localhost\root\cimv2") */
	WMIHelper(std::wstring connectionstring);
	virtual ~WMIHelper();
	
	typedef std::vector<VARIANT> VariantListType;
	typedef std::vector<std::wstring> StringListType;
	typedef std::map<std::wstring, VARIANT> ArgumentsType;
	typedef std::vector<ArgumentsType> ResultsType;
	
	class WMIObject
	{
	public:
		WMIObject()								: m_pInterface(0) { ; }
		WMIObject(WMIObject &r)					: m_pInterface(r.m_pInterface) 
		{ 
			if (m_pInterface) m_pInterface->AddRef();
		}
		WMIObject(IWbemClassObject *pInterface)	: m_pInterface(pInterface) { ; }

		virtual ~WMIObject()
		{
			if (m_pInterface) m_pInterface->Release();
		}

		HRESULT Put(std::wstring property, VARIANT *pvtVal);
		HRESULT Get(std::wstring property, VARIANT *pvtVal);
		
		//cast to encapsulated object
		operator IWbemClassObject* &() {return m_pInterface;}
		WMIObject& operator=(const WMIObject& r)
		{ 
			m_pInterface=r.m_pInterface; 
			if (m_pInterface)
				m_pInterface->AddRef();
			return *this;
		}
		
	private:
		IWbemClassObject *m_pInterface;
	};
	
	HRESULT Connect();
	HRESULT Connect(std::wstring domain, std::wstring username, std::wstring password);

	HRESULT SpawnObject(std::wstring path, WMIObject &object);
	HRESULT GetObject(std::wstring path, WMIObject &object);
	HRESULT ExecuteQuery(std::wstring query, ResultsType &results);
	HRESULT ExecuteMethod(std::wstring path, std::wstring instancepath, std::wstring method, ArgumentsType &args, ArgumentsType &results);
	HRESULT ExecuteMethod(WMIObject &object, std::wstring path, std::wstring method, ArgumentsType &args, ArgumentsType &results);
	
	/** Each of these removes all elements, calling VariantClear on any owned variants */
	static void Clear(VariantListType &list);
	static void Clear(ArgumentsType &list);
	static void Clear(ResultsType &list);
	
	static void WMIHelper::Print(ArgumentsType &list);
	static void WMIHelper::Print(ResultsType &list);

protected:
	IWbemServices *m_pServices;
	SecurityHelper::ProxyBlanket *m_pBlanket;
	
	// retained for setting up proxy blankets on all the proxies.
	std::wstring m_domain, m_username, m_password;
	std::wstring m_connectionstring;

};


#endif
