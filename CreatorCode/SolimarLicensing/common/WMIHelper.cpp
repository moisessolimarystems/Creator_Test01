#include "WMIHelper.h"
#include "SecurityNT.h"

/** Specifies the connection string to connect to the WMI locator object, (eg.: "\\localhost\root\cimv2") */
WMIHelper::WMIHelper(std::wstring connectionstring) : 
	m_connectionstring(connectionstring), m_pServices(NULL), m_pBlanket(NULL)
{
	;
}

HRESULT WMIHelper::WMIObject::Put(std::wstring property, VARIANT *pvtVal)
{
	HRESULT hr;

	if (!m_pInterface)
		return E_FAIL;
	
	BSTR bstrProperty=SysAllocString(property.c_str());
	hr = m_pInterface->Put(bstrProperty, 0, pvtVal, 0);
	SysFreeString(bstrProperty);
	return hr;
}

HRESULT WMIHelper::WMIObject::Get(std::wstring property, VARIANT *pvtVal)
{
	HRESULT hr;
	
	if (!m_pInterface)
		return E_FAIL;
	
	BSTR bstrProperty=SysAllocString(property.c_str());
	hr = m_pInterface->Get(bstrProperty, 0, pvtVal, NULL, NULL);
	SysFreeString(bstrProperty);
	return hr;
}

HRESULT WMIHelper::Connect()
{
	return Connect(L"",L"", L"");
}

HRESULT WMIHelper::Connect(std::wstring domain, std::wstring username, std::wstring password)
{
	HRESULT hr;
	IWbemLocator *pLocator;
	
	m_domain=domain;
	m_username=username;
	m_password=password;

	BSTR bstrConnectionString = SysAllocString(m_connectionstring.c_str());
	BSTR bstrUsername = SysAllocString((m_domain.length()>0 ? (m_domain + std::wstring(L"\\") + m_username).c_str() : m_username.c_str()));
	BSTR bstrPassword = SysAllocString(m_password.c_str());
		
	// create the locator object
	hr = CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER, IID_IWbemLocator , (void**)&pLocator);
	if (SUCCEEDED(hr))
	{
		// create the services object
		if (m_username!=std::wstring(L"") && m_password!=std::wstring(L""))
		{
			hr = pLocator->ConnectServer(bstrConnectionString, bstrUsername, bstrPassword, NULL, 0, NULL, NULL, &m_pServices);
		}
		else
		{
			hr = pLocator->ConnectServer(bstrConnectionString, NULL, NULL, NULL, 0, NULL, NULL, &m_pServices);
		}
		pLocator->Release();

		// set the proxy blanket for the services object if it has been obtained
		if (SUCCEEDED(hr))
		{
			//m_pBlanket = new SecurityHelper::ProxyBlanket(m_pServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL,RPC_C_IMP_LEVEL_IMPERSONATE, /*NULL,*/ EOAC_NONE);
			m_pBlanket=new SecurityHelper::ProxyBlanket(m_pServices, RPC_C_AUTHN_LEVEL_PKT_PRIVACY, RPC_C_IMP_LEVEL_IMPERSONATE, (m_domain.length()>0 ? m_domain.c_str() : NULL), (m_username.length()>0 ? m_username.c_str() : NULL), (m_password.length()>0 ? m_password.c_str() : NULL));
			hr = m_pBlanket->GetStatus();
		}
		else
		{
			m_pServices=NULL;
			m_pBlanket=NULL;
		}
	}
	
	SysFreeString(bstrPassword);
	SysFreeString(bstrUsername);
	SysFreeString(bstrConnectionString);
	
	return hr;
}

WMIHelper::~WMIHelper()
{
	m_connectionstring.erase();
	m_domain.erase();
	m_username.erase();
	m_password.erase();

	if (m_pBlanket)
		delete m_pBlanket;
	
	if (m_pServices)
		m_pServices->Release();
}

/*
typedef std::vector<VARIANT> VariantListType;
typedef std::vector<std::wstring> StringListType;
typedef std::map<std::wstring, VARIANT> ArgumentsType;
typedef std::vector<ArgumentsType> ResultsType;
*/

HRESULT WMIHelper::SpawnObject(std::wstring path, WMIObject &object)
{
	HRESULT hr;
	BSTR bstrPath = SysAllocString(path.c_str());
	
    IWbemClassObject *pClass = 0;
    hr = m_pServices->GetObject(bstrPath, 0, NULL, &pClass, NULL);
    SysFreeString(bstrPath);
	
	if (FAILED(hr))
	{
		if (pClass)
			pClass->Release();
		return hr;
	}
	
    IWbemClassObject *pInstance = 0;
    hr = pClass->SpawnInstance(0, &pInstance);
	
	if (pInstance)
		object=WMIObject(pInstance);
	
	if (pClass)
		pClass->Release();
	
	return hr;
}

HRESULT WMIHelper::GetObject(std::wstring path, WMIObject &object)
{
	HRESULT hr;
	BSTR bstrPath = SysAllocString(path.c_str());
	
    IWbemClassObject *pClass = 0;
    hr = m_pServices->GetObject(bstrPath, 0, NULL, &pClass, NULL);
    SysFreeString(bstrPath);
	
	if (FAILED(hr))
	{
		if (pClass)
			pClass->Release();
		return hr;
	}
	
	if (pClass)
		object=WMIObject(pClass);
	
	return hr;
}

HRESULT WMIHelper::ExecuteQuery(std::wstring query, ResultsType &results)
{
	HRESULT hr;

	// init the results structure
	Clear(results);
	
	if (m_pServices==NULL)
		return E_ACCESSDENIED;

	// set up params for the query
	BSTR bstrWQL=SysAllocString(L"WQL");
	BSTR bstrQuery=SysAllocString(query.c_str());

	// Execute the query
	IEnumWbemClassObject *pEnumClassObject=0;
	//
	hr=m_pServices->ExecQuery(bstrWQL, bstrQuery, WBEM_FLAG_FORWARD_ONLY, NULL, &pEnumClassObject);
	
	// cleanup
	SysFreeString(bstrQuery);
	SysFreeString(bstrWQL);
	
	// loop through and package the query results
	if (SUCCEEDED(hr))
	{
		// Define the object interface
		IWbemClassObject *pObj;
		ULONG uReturned = 0; // Used to check the number of objects returned
		
		VARIANT vtArg;
		
		SAFEARRAY *psaNames=NULL;
		BSTR* pData=NULL;
		
		{// set a proxy blanket on the enum
			SecurityHelper::ProxyBlanket blanket(pEnumClassObject, RPC_C_AUTHN_LEVEL_PKT_PRIVACY, RPC_C_IMP_LEVEL_IMPERSONATE, (m_domain.length()>0 ? m_domain.c_str() : NULL), (m_username.length()>0 ? m_username.c_str() : NULL), (m_password.length()>0 ? m_password.c_str() : NULL));
			
			// loop for each row returned
			while(true)
			{
				pObj=0;
				pEnumClassObject->Next(0, 1, &pObj, &uReturned);

				if (uReturned==0 || FAILED(hr))
					break;
				
				// add a new row to the results
				results.push_back(ResultsType::value_type());
				
				psaNames=NULL;
				pData=NULL;
				
				// get all the property names
				hr = pObj->GetNames(NULL, WBEM_FLAG_ALWAYS, NULL, &psaNames);
				if (SUCCEEDED(hr) && psaNames!=NULL)
				{
					// loop through each property name, getting its value
					SafeArrayAccessData(psaNames, (void**)&pData);
					for (unsigned int i=0; i<psaNames->rgsabound[0].cElements; ++i)
					{
						hr = pObj->Get(pData[i], 0, &vtArg, NULL, NULL);
						if (SUCCEEDED(hr))
						{
							std::wstring propertyname(pData[i]);
							//if (propertyname.size()<=2 || (propertyname[0]!=L'_' && propertyname[1]!=L'_'))
								results.back()[propertyname]=vtArg;
						}
					}
					SafeArrayUnaccessData(psaNames);
				}
				SafeArrayDestroy(psaNames);
				
				pObj->Release();
			}
		} // close the proxy blanket
		pEnumClassObject->Release();
	}
	
	return hr;
}

HRESULT WMIHelper::ExecuteMethod(WMIObject &object, std::wstring path, std::wstring method, ArgumentsType &args, ArgumentsType &results)
{
	/*
	BSTR bstrArgname;
	VARIANT vtArg;
	HRESULT hr;
	
	if (m_pServices==NULL)
		return E_ACCESSDENIED;

	// alloc strings
	BSTR bstrPath, bstrMethod, bstrInstancepath;
	bstrPath=SysAllocString(path.c_str());
	bstrInstancepath=SysAllocString(instancepath.c_str());
	bstrMethod=SysAllocString(method.c_str());
	
	IWbemClassObject *pPath=0, *pMethod=0, *pInparams=0, *pOutparams=0;
	
	// Get the method and its parameters
	hr = m_pServices->GetObject(bstrPath, 0, NULL, &pPath, NULL);
	if (SUCCEEDED(hr))
	{
		hr = pPath->GetMethod(bstrMethod, 0, &pMethod, NULL);
		if (SUCCEEDED(hr) && pMethod!=NULL)
		{
			hr = pMethod->SpawnInstance(0, &pInparams);
			if (FAILED(hr))
			{
				pMethod->Release();
				pMethod=0;
				pPath->Release();
				pPath=0;
			}
		}
		else
		{
			pPath->Release();
			pPath=0;

			if(pMethod)
				pMethod->Release();

			pMethod=0;
		}
	}
	
	if (FAILED(hr))
	{
		SysFreeString(bstrPath);
		SysFreeString(bstrInstancepath);
		SysFreeString(bstrMethod);
		return hr;
	}
	
	// set up the arguments
	ArgumentsType::iterator i;
	for (i=args.begin(); i!=args.end(); ++i)
	{
		bstrArgname=SysAllocString((i->first).c_str());
		hr = pInparams->Put(bstrArgname, 0, &(i->second), 0);
		SysFreeString(bstrArgname);
	}
	
	// execute the method
	hr = m_pServices->ExecMethod((instancepath.length()>0 ? bstrInstancepath : bstrPath), bstrMethod, 0, NULL, (pInparams!=NULL ? pInparams : NULL), &pOutparams, NULL);
	
	SysFreeString(bstrPath);
	SysFreeString(bstrInstancepath);
	SysFreeString(bstrMethod);
	
	// package outgoing parameters
	Clear(results);
	
	if (SUCCEEDED(hr))
	{
		SAFEARRAY *psaNames=NULL;
		BSTR* pData=NULL;
		hr = pOutparams->GetNames(NULL, WBEM_FLAG_ALWAYS, NULL, &psaNames);
		if (SUCCEEDED(hr) && psaNames!=NULL)
		{
			SafeArrayAccessData(psaNames, (void**)&pData);
			for (unsigned int i=0; i<psaNames->rgsabound[0].cElements; ++i)
			{
				hr = pOutparams->Get(pData[i], 0, &vtArg, NULL, NULL);
				if (SUCCEEDED(hr))
				{
					std::wstring propertyname(pData[i]);
					//if (propertyname.size()<=2 || (propertyname[0]!=L'_' && propertyname[1]!=L'_'))
						results[propertyname]=vtArg;
				}
			}
			SafeArrayUnaccessData(psaNames);
		}
		SafeArrayDestroy(psaNames);
	}
	
	if (pInparams)
		pInparams->Release();
	if (pOutparams)
		pOutparams->Release();
	if (pMethod)
		pMethod->Release();
	if (pPath)
		pPath->Release();
	
	return hr;
	*/
	return S_OK;
}


HRESULT WMIHelper::ExecuteMethod(std::wstring path, std::wstring instancepath, std::wstring method, ArgumentsType &args, ArgumentsType &results)
{
	BSTR bstrArgname;
	VARIANT vtArg;
	HRESULT hr;
	
	if (m_pServices==NULL)
		return E_ACCESSDENIED;

	// alloc strings
	BSTR bstrPath, bstrMethod, bstrInstancepath;
	bstrPath=SysAllocString(path.c_str());
	bstrInstancepath=SysAllocString(instancepath.c_str());
	bstrMethod=SysAllocString(method.c_str());
	
	IWbemClassObject *pPath=0, *pMethod=0, *pInparams=0, *pOutparams=0;
	
	// Get the method and its parameters
	hr = m_pServices->GetObject(bstrPath, 0, NULL, &pPath, NULL);
	if (SUCCEEDED(hr))
	{
		hr = pPath->GetMethod(bstrMethod, 0, &pMethod, NULL);
		if (SUCCEEDED(hr) && pMethod!=NULL)
		{
			hr = pMethod->SpawnInstance(0, &pInparams);
			if (FAILED(hr))
			{
				pMethod->Release();
				pMethod=0;
				pPath->Release();
				pPath=0;
			}
		}
		else
		{
			pPath->Release();
			pPath=0;

			if(pMethod)
				pMethod->Release();

			pMethod=0;
		}
	}
	
	if (FAILED(hr))
	{
		SysFreeString(bstrPath);
		SysFreeString(bstrInstancepath);
		SysFreeString(bstrMethod);
		return hr;
	}
	
	// set up the arguments
	ArgumentsType::iterator i;
	for (i=args.begin(); i!=args.end(); ++i)
	{
		bstrArgname=SysAllocString((i->first).c_str());
		hr = pInparams->Put(bstrArgname, 0, &(i->second), 0);
		SysFreeString(bstrArgname);
	}
	
	// execute the method
	hr = m_pServices->ExecMethod((instancepath.length()>0 ? bstrInstancepath : bstrPath), bstrMethod, 0, NULL, (pInparams!=NULL ? pInparams : NULL), &pOutparams, NULL);
	
	SysFreeString(bstrPath);
	SysFreeString(bstrInstancepath);
	SysFreeString(bstrMethod);
	
	// package outgoing parameters
	Clear(results);
	
	if (SUCCEEDED(hr))
	{
		SAFEARRAY *psaNames=NULL;
		BSTR* pData=NULL;
		hr = pOutparams->GetNames(NULL, WBEM_FLAG_ALWAYS, NULL, &psaNames);
		if (SUCCEEDED(hr) && psaNames!=NULL)
		{
			SafeArrayAccessData(psaNames, (void**)&pData);
			for (unsigned int i=0; i<psaNames->rgsabound[0].cElements; ++i)
			{
				hr = pOutparams->Get(pData[i], 0, &vtArg, NULL, NULL);
				if (SUCCEEDED(hr))
				{
					std::wstring propertyname(pData[i]);
					//if (propertyname.size()<=2 || (propertyname[0]!=L'_' && propertyname[1]!=L'_'))
						results[propertyname]=vtArg;
				}
			}
			SafeArrayUnaccessData(psaNames);
		}
		SafeArrayDestroy(psaNames);
	}
	
	if (pInparams)
		pInparams->Release();
	if (pOutparams)
		pOutparams->Release();
	if (pMethod)
		pMethod->Release();
	if (pPath)
		pPath->Release();
	
	return hr;
}

/** Each of these removes all elements, calling VariantClear on any owned variants */
void WMIHelper::Clear(VariantListType &list)
{
	VARIANT vtTmp;
	while(!list.empty())
	{
		vtTmp=list.back();
		VariantClear(&vtTmp);
		list.pop_back();
	}
}

void WMIHelper::Clear(ArgumentsType &list)
{
	VARIANT vtTmp;
	ArgumentsType::iterator i;
	for (i=list.begin(); i!=list.end(); ++i)
	{
		vtTmp=i->second;
		VariantClear(&vtTmp);
	}
	list.clear();
}

void WMIHelper::Clear(ResultsType &list)
{
	ResultsType::iterator i;
	for (i=list.begin(); i!=list.end(); ++i)
	{
		Clear(*i);
	}
	list.clear();
}


void WMIHelper::Print(ArgumentsType &list)
{
	VARIANT vtTmp;
	ArgumentsType::iterator i;
	for (i=list.begin(); i!=list.end(); ++i)
	{
		OutputDebugString(_T("*"));
		OutputDebugStringW(i->first.c_str());
		OutputDebugString(_T(" "));
		vtTmp=i->second;
		if ((vtTmp.vt & VT_TYPEMASK)==VT_BSTR)
		{
			OutputDebugStringW(vtTmp.bstrVal);
		}
		else
		{
			TCHAR buffer[100];
			_stprintf_s(buffer, 100, _T("%08x"),vtTmp.ulVal);
			OutputDebugString(buffer);
		}
		OutputDebugString(_T("*\r\n"));
	}
}


void WMIHelper::Print(ResultsType &list)
{
	ResultsType::iterator i;
	for (i=list.begin(); i!=list.end(); ++i)
	{
		Print(*i);
	}
}

