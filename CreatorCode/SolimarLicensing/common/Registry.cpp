#include "Registry.h"
#include "COMUtils.h"

#pragma warning(disable : 4786)
#include <vector>

long Registry::GetValueDWORD(HKEY root, const wchar_t* key, const wchar_t* valuename, LPDWORD value /*out*/)
{
	HKEY regkey;
	long result;

	result = RegOpenKeyExW(
		root,
		key,
		0,
		KEY_READ | KEY_EXECUTE | KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS,
		&regkey);

	if (result!=ERROR_SUCCESS)
		return result;

	DWORD buffersize=sizeof(DWORD);
	result = RegQueryValueExW(
		regkey,
		valuename,
		0,
		NULL,
		(LPBYTE)(void*)value,
		&buffersize);

	if (result!=ERROR_SUCCESS)
		return result;
	
	result = RegCloseKey(regkey);
	return result;
}

long Registry::GetValueString(HKEY root, const wchar_t* key, const wchar_t* valuename, LPBYTE* ppBuffer, DWORD initialBufferSize)
{
	HKEY regkey;
	long result;
	
	*ppBuffer=NULL;

	result = RegOpenKeyExW(
		root,
		key,
		0,
		KEY_READ | KEY_EXECUTE | KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS,
		&regkey);

	if (result!=ERROR_SUCCESS)
		return result;

	DWORD bufsize=initialBufferSize;
	while(true)
	{
		// allocate a buffer
		*ppBuffer=new unsigned char[bufsize];

		// leave if there was an error allocating memory
		if (*ppBuffer==NULL)
		{
			result = ERROR_NOT_ENOUGH_MEMORY;
			break;
		}
		
		result = RegQueryValueExW(
			regkey,
			valuename,
			0,
			NULL,
			*ppBuffer,
			&bufsize);
		
		// if successful, leave
		if (result==ERROR_SUCCESS)
			break;
		
		// the buffer was too small or there was an error, kill the buffer
		delete [] *ppBuffer;
		
		// if there was an error, break out
		if (result!=ERROR_MORE_DATA)
			break;
		
		// try again with a larger buffer
		bufsize=bufsize*2;
	}

	if (result!=ERROR_SUCCESS)
	{
		*ppBuffer=NULL;
		return result;
	}
	
	result = RegCloseKey(regkey);
	return result;
}


HRESULT Registry::GetValue(HKEY root, _bstr_t key, _bstr_t valuename, _variant_t &value, REGSAM samDesired)
{
	HRESULT hr;
	HKEY regkey;
	long result;
	
	value.Clear();

	result = RegOpenKeyExW(
		root,
		key,
		0,
		KEY_READ | KEY_EXECUTE | KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | samDesired,
		&regkey);

	if (result!=ERROR_SUCCESS)
		return HRESULT_FROM_WIN32(result);
	
	LPBYTE pData;
	DWORD datasize, datatype;
	
	datasize=0;
	result = RegQueryValueExW(
		regkey,
		valuename,
		NULL,
		&datatype,
		NULL,
		&datasize);
	
	if (result!=ERROR_SUCCESS)
		return HRESULT_FROM_WIN32(result);

	// allocate a buffer to receive the data
	pData=new BYTE[datasize];
	
	result = RegQueryValueExW(
		regkey,
		valuename,
		NULL,
		NULL,
		pData,
		&datasize);

	hr = HRESULT_FROM_WIN32(result);
	
	if (SUCCEEDED(hr))
	{
		// shove the result in to a _variant_t using the type info
		switch(datatype)
		{
			case REG_BINARY:
			{
				//xxx not implemented (safearray)
				break;
			}
			case REG_DWORD:
			//case REG_DWORD_LITTLE_ENDIAN:
			//case REG_DWORD_BIG_ENDIAN:
			{
				value.vt=VT_UI4;
				value.ulVal=(unsigned long)(*((DWORD*)pData));
				break;
			}
			case REG_MULTI_SZ:
			{
				COMUtils::DimensionsType dim;
				unsigned int stringcount=0;
				VARIANT *vt;

				// count the number of strings in the data
				wchar_t* pCursor;
				bool prevnull=false;
				pCursor=(wchar_t*)pData;
				while(pCursor<(wchar_t*)(pData+datasize))
				{
					if (*pCursor==L'\0')
					{
						if (!prevnull)
							stringcount++;
						else
							break;
					}
				}

				dim.push_back(stringcount);
				COMUtils::CreateMultidimensionalSafearray(&value, dim);
				dim.pop_back();

				pCursor=(wchar_t*)pData;

				// populate each string
				for (unsigned int index=0; index<stringcount; ++index)
				{
					dim.push_back(index);
					COMUtils::AccessMultidimensionalSafearray(&value, dim, &vt);
					
					*vt=_variant_t(_bstr_t(pCursor)).Detach();
					pCursor+=wcslen(pCursor)+1;
					
					COMUtils::UnaccessMultidimensionalSafearray(&value, dim);
					dim.pop_back();
				}
				
				break;
			}
			case REG_SZ:
			{
				value=_bstr_t((wchar_t*)pData);
				break;
			}
			default:
			{
				//xxx not implemented
				break;
			}
		}
	}

	delete [] pData;
	
	result = RegCloseKey(regkey);

	if (FAILED(hr))
	{
		return hr;
	}
	return HRESULT_FROM_WIN32(result);
}

/*
 * EnumKey()
 *
 * Index should be set to 0 for the first call. All subsequent calls
 * should continue incrementing index until the return value != S_OK.
 *
 * While an application is using the RegEnumKey function, it should 
 * not make calls to any registration functions that might change the 
 * key being queried.
 *
 * Returns S_FALSE when there are no more sub keys to enumerate.
 */
HRESULT Registry::EnumKey(HKEY root, BSTR key, DWORD index, _bstr_t &subKey, REGSAM samDesired)
{
	HRESULT hr = S_FALSE;
	HKEY regkey;
	long result;
	
	result = RegOpenKeyExW(
		root,
		key,
		0,
		KEY_READ | KEY_EXECUTE | KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | samDesired,
		&regkey);

	if (result!=ERROR_SUCCESS)
	{
		if(HRESULT_FROM_WIN32(result) == 0x80070002)	//Cannot find Key
			return S_FALSE;
		else
			return HRESULT_FROM_WIN32(result);
	}

	wchar_t wcKey[MAX_PATH];
	result = RegEnumKeyW(regkey, index, wcKey, MAX_PATH); 
	if (result == (DWORD)ERROR_SUCCESS) {
		subKey = wcKey;
		hr = S_OK;
	}
	else
		hr = S_FALSE;

	RegCloseKey(regkey);

	return hr;
}

// creates the key if it does not exist, and overwrites existing values if they do.
HRESULT Registry::SetValue(HKEY root, _bstr_t key, _bstr_t valuename, _variant_t value, REGSAM samDesired)
{
	HRESULT hr;
	HKEY regkey;
	long result;
	
	// open/create the key
	result = RegCreateKeyExW(root, key, 0, NULL, 0, KEY_WRITE | samDesired, NULL, &regkey, NULL);	// optionally the last param recieves whehter a key was created or opened
	if (FAILED(hr=HRESULT_FROM_WIN32(result)))
		return hr;
	
	// check that the type of any existing value
	DWORD datatype;
	result = RegQueryValueExW(
		regkey,
		valuename,
		NULL,
		&datatype,
		NULL,
		NULL);
	
	// if the key was found, make sure the provided value is of the same type
	if (!result)
	{
		switch (datatype)
		{
			case REG_BINARY:
				// if this is not binary data
				if (!(value.vt & VT_ARRAY) || !((value.vt & VT_UI1) || (value.vt & VT_I1)))
					hr = E_INVALIDARG;
				break;
			case REG_DWORD:
				// if this is not a 4 byte integer
				if (!(value.vt & VT_I4) || !(value.vt & VT_UI4)) 
					hr = E_INVALIDARG;
				break;
			case REG_MULTI_SZ:
				// if this isn't an array of variants  (necessary but not sufficient check)
				if (!(value.vt & (VT_ARRAY | VT_VARIANT)))
					hr = E_INVALIDARG;
				break;
			case REG_SZ:
				// if this wasn't a string
				if (!(value.vt & (VT_BSTR)))
					hr = E_INVALIDARG;
				break;
		}
	}

	if (FAILED(hr))
	{
		RegCloseKey(regkey);
		return hr;
	}
	
	// write the value to the registry
	result=0;
	unsigned char* pData;
	switch(value.vt)
	{
		// binary data
		case VT_ARRAY | VT_UI1:
		case VT_ARRAY | VT_I1:
			hr = SafeArrayAccessData(value.parray, (void**)&pData);
			if (FAILED(hr))
				break;
			result = RegSetValueExW(regkey, valuename, 0, REG_BINARY, pData, value.parray->rgsabound[0].cElements);
			hr = HRESULT_FROM_WIN32(result);
			SafeArrayUnaccessData(value.parray);
			break;
		// string
		case VT_BSTR:
			result = RegSetValueExW(regkey, valuename, 0, REG_SZ, (unsigned char*)value.bstrVal, (DWORD)sizeof(wchar_t)*(wcslen(value.bstrVal)+1));
			hr = HRESULT_FROM_WIN32(result);
			break;
		// multi string
		case VT_ARRAY | VT_VARIANT:
			//xxx not implemented (use the multidimensional bits in COMUtils to 
			//xxx access the array, and create a buffer holding their contents)
			break;
		// dword
		case VT_I4:
			result = RegSetValueExW(regkey, valuename, 0, REG_DWORD, (unsigned char*)&value.lVal, sizeof(value.lVal));
			hr = HRESULT_FROM_WIN32(result);
			break;
		case VT_UI4:
			result = RegSetValueExW(regkey, valuename, 0, REG_DWORD, (unsigned char*)&value.ulVal, sizeof(value.ulVal));
			hr = HRESULT_FROM_WIN32(result);
			break;
		default:
			hr = E_INVALIDARG;
			break;
	}
	
	RegCloseKey(regkey);
	
	return hr;
}


/* Enumerates the values of a given registry key. Returns S_FALSE when no more values are found
 *
 * Index should be set to 0 for the first call. All subsequent calls
 * should continue incrementing index until the return value != S_OK.
 *
 * While an application is using the RegEnumKeyValues function, it should 
 * not make calls to any registration functions that might change the 
 * key being queried.
 *
 * Returns S_FALSE when there are no more sub keys to enumerate or an error occurs.
 */
HRESULT Registry::EnumKeyValues(HKEY root, BSTR key, DWORD index, _bstr_t &subKey, REGSAM samDesired)
{
	HRESULT hr = S_FALSE;
	HKEY regkey;
	long result;
	
	result = RegOpenKeyExW(
		root,
		key,
		0,
		KEY_READ | KEY_EXECUTE | KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | samDesired,
		&regkey);

	if (result!=ERROR_SUCCESS)
	{
		if(HRESULT_FROM_WIN32(result) == 0x80070002)	//Cannot find Key
			return S_FALSE;
		else
			return HRESULT_FROM_WIN32(result);
	}
	
	DWORD valuenamesize=MAX_PATH;
	wchar_t *pValuename= new wchar_t[valuenamesize];
	
	result = RegEnumValueW(regkey, index, pValuename, &valuenamesize, NULL, NULL, NULL, NULL);
	if (result == (DWORD)ERROR_SUCCESS) {
		subKey = pValuename;
		hr = S_OK;
	}
	else
		hr = S_FALSE;
	
	delete [] pValuename;

	RegCloseKey(regkey);
	
	return hr;
}

/* Deletes a key and all of its subkeys and values */
HRESULT Registry::DeleteKey(HKEY root, BSTR parentKey, BSTR keyname, REGSAM samDesired)
{
	return E_NOTIMPL;
	//See reason below for reason why this function is not implemented
	/*
	
	typedef std::vector<_bstr_t> stringlist;

	HRESULT hr = S_FALSE;
	HKEY regkey;
	long result;
	DWORD index;
	_bstr_t keypath=_bstr_t(parentKey)+_bstr_t(L"\\")+_bstr_t(keyname);
	_bstr_t valuename;
	_bstr_t childkeyname;
	stringlist names;
	
	result = RegOpenKeyExW(
		root,
		parentKey,
		0,
		KEY_READ | KEY_EXECUTE | KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | samDesired,
		&regkey);

	if (result!=ERROR_SUCCESS)
		return HRESULT_FROM_WIN32(result);
	
	// delete all the values under key key
	hr = S_OK;
	for(index=0; SUCCEEDED(hr); ++index)
	{
		hr = EnumKeyValues(root, keypath, index, valuename);
		if (hr==S_FALSE)
		{
			hr = S_OK;
			break;
		}
		if (SUCCEEDED(hr))
		{
			names.push_back(valuename);
		}
	}
	
	// delete all the values in the names collection
	while(!names.empty() && SUCCEEDED(hr))
	{
		valuename=names.back();
		names.pop_back();
		hr = DeleteValue(root, keypath, valuename);
	}
	
	if (FAILED(hr))
	{
		RegCloseKey(regkey);
		return hr;
	}
	
	
	
	// delete all of the keys, recursively, under the key
	for (index=0; SUCCEEDED(hr); ++index)
	{
		hr = EnumKey(root, keypath, index, childkeyname);
		if (hr==S_FALSE)
		{
			hr = S_OK;
			break;
		}
		if (SUCCEEDED(hr))
		{
			names.push_back(childkeyname);
		}
	}
	// delete all the values in the names collection
	while(!names.empty() && SUCCEEDED(hr))
	{
		childkeyname=names.back();
		names.pop_back();
		hr = DeleteKey(root, keypath, childkeyname, samDesired);
	}
	
	if (FAILED(hr))
	{
		RegCloseKey(regkey);
		return hr;
	}
	
	
	// delete the key itself
	result = RegDeleteKeyExW(regkey, keyname, samDesired, 0);	//Problem Here

	//The problem lay specifically with a dependency on RegDeleteKeyEx() from ADVAPI32.DLL which for some 
	//reason is shown as unresolvable in Dependency Walker and also gives a registration problem.  This 
	//registration issue only appears on a debug build.  Must use RegDeleteKeyExW instead of RegDeleteKey because
	//only RegDeleteKeyExW accepts a parameter to delete registry keys on a 64-bit machine.


	hr = HRESULT_FROM_WIN32(result);

	RegCloseKey(regkey);

	return hr;
	*/
}

/* Deletes a registry value */
HRESULT Registry::DeleteValue(HKEY root, BSTR parentKey, BSTR valuename, REGSAM samDesired)
{
	HRESULT hr;
	HKEY regkey;
	long result;
	
	result = RegOpenKeyExW(
		root,
		parentKey,
		0,
		KEY_READ | KEY_EXECUTE | KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | KEY_WRITE | samDesired,
		&regkey);

	if (result!=ERROR_SUCCESS)
		return HRESULT_FROM_WIN32(result);
	
	result = RegDeleteValueW(regkey, valuename);
	hr = HRESULT_FROM_WIN32(result);

	RegCloseKey(regkey);

	return hr;
}

