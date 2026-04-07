
#ifndef _WIN32_DCOM
#define _WIN32_DCOM
#endif
#ifndef WIN32_DCOM
#define WIN32_DCOM
#endif

#include "COMUtils.h"
#include <oleauto.h>
#include <objbase.h>

/*
 * Sets the proxy blanket for the given itf. Also allows you to set the proxy blanket
 * on the IUnknown of that itf (The first param isn't the true IUnknown of the interface).
 * See article https://docs.microsoft.com/en-us/windows/win32/wmisdk/setting-authentication-using-c-
 * for an explanation of why the IUnknown of the Itf might need to be set. Usually you will
 * need to set the extra setIUnknown param for interfaces returned from CoCreateInstanceEx calls, and 
 * not set it for interfaces returned from QueryInterface calls.
 */
HRESULT COMUtils::SetProxyBlanket(IUnknown* pItf, COAUTHINFO* pAuthInfo, bool setIUnknown)
{
	HRESULT hr = S_OK;

	if (pAuthInfo)
	{
		hr = CoSetProxyBlanket(
			pItf,
			pAuthInfo->dwAuthnSvc,
			pAuthInfo->dwAuthzSvc,
			pAuthInfo->pwszServerPrincName,
			pAuthInfo->dwAuthnLevel,
			pAuthInfo->dwImpersonationLevel,
			pAuthInfo->pAuthIdentityData,
			pAuthInfo->dwCapabilities);

		if (SUCCEEDED(hr) && setIUnknown)
		{
			// Set the proxy blanket on IUnkown to avoid ACCESS DENIED. Itf can be released immediately after.
			// See the following microsoft article for an explanation: https://docs.microsoft.com/en-us/windows/win32/wmisdk/setting-authentication-using-c-
			IUnknown* pIUnk = NULL;
			hr = pItf->QueryInterface(__uuidof(IUnknown), (void**)&pIUnk);
			if (SUCCEEDED(hr))
			{
				hr = COMUtils::SetProxyBlanket(pIUnk, pAuthInfo);
				pIUnk->Release();
			}
		}
	}

	return hr;
}

/** Creates a multidimensional array of 
  * variants using variant safearrays of 
  * variant safearrays.
  * Arguments:
  *		array		provide pointer to a variant 
  *					object that will receive the array
  *		dimensions	listing of the size of each dimension
  */
HRESULT COMUtils::CreateMultidimensionalSafearray(VARIANT* array, DimensionsType &dimensions)
{
	HRESULT hr;
	
	
	// base case
	if (dimensions.empty())
	{
		VariantInit(array);
		return S_OK;
	}
	else
	{
		// figure out how big the first dimension is
		unsigned int uiArraySize=dimensions.front();

		// create the inner array dimensions argument for recursive calls
		DimensionsType innerDimensions(dimensions);
		innerDimensions.pop_front();
		

		//
		// Create a safearray for use as the 'array' variant
		//
		
		VARIANT *pVariant;
		SAFEARRAY *pDataSA = SafeArrayCreateVector(VT_VARIANT, 0, uiArraySize);
		if (!pDataSA) {
			//TRACE(" COMUtils::CreateMultidimensionalSafearray SafeArrayCreateVector failed");
			//SS_REPORT_CUSTOM_ERROR(CLSID_SSDocLink, IID_ISSDocLink, EC_SS_NOMEMORY)
			return E_OUTOFMEMORY;
		}
			
		//
		// Lock the array, so we have direct access to the safearray buffer.
		//
		
		hr = SafeArrayAccessData(pDataSA, (void**)&pVariant);
		if (FAILED(hr)) {
			//TRACEhr(hr, " COMUtils::CreateMultidimensionalSafearray SafeArrayAccessData failed");
			//SafeArrayDestroy(pDataSA);
			return hr;
		}
		
		
		// iterate through the list of modules
		for (unsigned int i=0; i<uiArraySize; ++i)
		{
			// recurse
			hr = CreateMultidimensionalSafearray(&pVariant[i], innerDimensions);
			if (FAILED(hr))
				break;
		}

		
		// 
		// Unlock the array.
		//
		
		SafeArrayUnaccessData(pDataSA);
		
		// 
		// Set up the variant to hold an array
		// 
		
		VariantInit(array);
		array->vt = VT_ARRAY | VT_VARIANT;
		array->parray = pDataSA;
	
	}

	return hr;
}



/** Provides access to an element or array in the 
  * Arguments:
  *		array		Pointer to an object created
  *					using a call to CreateMultidimensionalSafearray
  *		indices		Indices in to the multidimensional array.
  *					If fewer indices are provided than dimensions
  *					exist in the array, the array specified by
  *					the indices will be returned. 
  *					Eg.: for a 2 dimensional array, specifiying one index, n,
  *					will cause the function to return a single dimensional
  *					array, that array at the nth position of the 1st dimension.
  *		element		Element recieves a pointer to the element
  *					at the specified indices
  */
HRESULT COMUtils::AccessMultidimensionalSafearray(VARIANT* array, DimensionsType &indices, VARIANT** element)
{
	HRESULT hr;
	
	// base case
	if (indices.empty())
	{
		*element=array;
		return S_OK;
	}
	// Recursive case
	else
	{
		// figure out how big the first dimension is
		unsigned int uiArrayIndex=indices.front();
		
		// create the inner array dimensions argument for recursive calls
		DimensionsType innerIndices(indices);
		innerIndices.pop_front();
		
		VARIANT* pVariant;
		hr = SafeArrayAccessData(array->parray, (void**)&pVariant);
		if (FAILED(hr)) {
			//TRACEhr(hr, " COMUtils::CreateMultidimensionalSafearray SafeArrayAccessData failed");
			//SafeArrayDestroy(pDataSA);
			return hr;
		}
		
		// bounds check
		if (uiArrayIndex>array->parray->rgsabound[0].cElements)
			hr=E_INVALIDARG;
		
		// recurse
		if (SUCCEEDED(hr))
			hr = AccessMultidimensionalSafearray(&pVariant[uiArrayIndex], innerIndices, element);
		
		// on failure, de-allocate resources
		if (FAILED(hr))
		{
			SafeArrayUnaccessData(array->parray);
			*element=NULL;
		}
		
		return hr;
	}

	return S_OK;
}


/** Provides releases the element obtained with a call to Access...
  * Decrements reference counts, etc.
  */
HRESULT COMUtils::UnaccessMultidimensionalSafearray(VARIANT* array, DimensionsType &indices)
{
	HRESULT hr;
	
	// base case
	if (indices.empty())
	{
		return S_OK;
	}
	// Recursive case
	else
	{
		// figure out how big the first dimension is
		unsigned int uiArrayIndex=indices.front();
		
		// create the inner array dimensions argument for recursive calls
		DimensionsType innerIndices(indices);
		innerIndices.pop_front();
		
		VARIANT* pVariant;
		hr = SafeArrayAccessData(array->parray, (void**)&pVariant);
		if (FAILED(hr)) {
			//TRACEhr(hr, " COMUtils::CreateMultidimensionalSafearray SafeArrayAccessData failed");
			//SafeArrayDestroy(pDataSA);
			return hr;
		}

		// recurse
		hr = UnaccessMultidimensionalSafearray(&pVariant[uiArrayIndex], innerIndices);
		if (FAILED(hr))
		{
			return hr;
		}
		
		// 
		// Unlock the array.
		//
		
		hr = SafeArrayUnaccessData(array->parray);
		if (FAILED(hr))
		{
			return hr;
		}
		
		hr = SafeArrayUnaccessData(array->parray);
		
		return hr;
	}

	return S_OK;
}

/** Calls the SafeArrayDelete on all the internal arrays recursively */
HRESULT COMUtils::DeleteMultidimensionalSafearray(VARIANT* array)
{
	HRESULT hr = S_OK;
	hr = VariantClear(array);
	if (FAILED(hr)) return hr;
	VariantInit(array);
	return hr;
}


/** Gets the dimensions of a multidimensional safearray */
HRESULT COMUtils::SizeofMultidimensionalSafearray(VARIANT* array, DimensionsType &indices)
{
	HRESULT hr=S_OK;
	VARIANT* pVariant;
	
	if (!array)
		return E_INVALIDARG;
	
	// base case
	if (!(array->vt & VT_ARRAY) ||  !(array->vt & VT_VARIANT))
	{
		indices.clear();
		return S_OK;
	}
	// recursive case
	else
	{
		// lock the array
		hr = SafeArrayAccessData(array->parray, (void**)&pVariant);
		if (FAILED(hr))
		{
			return hr;
		}
		
		// recurse if there are elements
		if (array->parray->rgsabound[0].cElements>0)
		{
			hr = SizeofMultidimensionalSafearray(pVariant, indices);
		}
		
		indices.push_front(array->parray->rgsabound[0].cElements);

		// Unlock the array 
		hr = SafeArrayUnaccessData(array->parray);

		return hr;
	}
}

/** Sets a value in the safe array using VariantCopy */
HRESULT COMUtils::SetMultidimensionalSafearrayValue(VARIANT* array, DimensionsType &indices, VARIANT *value)
{
	HRESULT hr = S_OK;
	VARIANT *pvtDest(0);

	hr = AccessMultidimensionalSafearray(array, indices, &pvtDest);
	if (FAILED(hr)) return hr;
	hr = VariantCopy(pvtDest,value);
	if (FAILED(hr))
	{
		UnaccessMultidimensionalSafearray(array, indices);
		return hr;
	}
	else
	{
		hr = UnaccessMultidimensionalSafearray(array, indices);
		if (FAILED(hr)) return hr;
	}
	
	return S_OK;
}


/** Reads raw data from a file in to a safe array */
HRESULT COMUtils::CreateSafeArrayFromFile(VARIANT* pvArray, BSTR path)
{
	void *pData;
	SAFEARRAY *pDataArray;
	HRESULT hr;

	// initialize the in variable
	VariantInit(pvArray);

	// load the file to send in to a variant
	HANDLE sourcefile = CreateFileW(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if (sourcefile==INVALID_HANDLE_VALUE)
		return HRESULT_FROM_WIN32(GetLastError());
	
	DWORD filesize = GetFileSize(sourcefile,NULL);
	if (filesize==INVALID_FILE_SIZE)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		CloseHandle(sourcefile);
		return hr;
	}
	
	SAFEARRAYBOUND rgsabound[1];
	rgsabound[0].lLbound = 0;
	rgsabound[0].cElements = filesize;
	
	// create an array of unsigned bytes
	pDataArray = SafeArrayCreate(VT_UI1, 1, rgsabound);
	
	hr = SafeArrayAccessData(pDataArray, (void**)&pData);
	if (FAILED(hr))
	{
		CloseHandle(sourcefile);
		return hr;
	}

	DWORD bytesread;
	if (!ReadFile(sourcefile, pData, rgsabound[0].cElements, &bytesread, NULL))
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		CloseHandle(sourcefile);
		SafeArrayUnaccessData(pDataArray);
		return hr;
	}
	CloseHandle(sourcefile);
	
	hr = SafeArrayUnaccessData(pDataArray);
	
	pvArray->vt=VT_ARRAY | VT_UI1;
	pvArray->parray=pDataArray;

	return hr;
}

/** Reads data from a buffer in to a safe array */
HRESULT COMUtils::CreateSafeArrayFromBuffer(
	VARIANT* pvArray, 
	void* buffer, 
	unsigned int buffersize)
{

	void *pData;
	SAFEARRAY *pDataArray;
	HRESULT hr;

	// initialize the in variable
	VariantInit(pvArray);
	
	SAFEARRAYBOUND rgsabound[1];
	rgsabound[0].lLbound = 0;
	rgsabound[0].cElements = buffersize;
	
	// create an array of unsigned bytes
	pDataArray = SafeArrayCreate(VT_UI1, 1, rgsabound);
	
	hr = SafeArrayAccessData(pDataArray, (void**)&pData);
	if (FAILED(hr))
		return hr;
	
	// copy bytes from the buffer in to the safearray
	memcpy(pData, buffer, buffersize);
	
	hr = SafeArrayUnaccessData(pDataArray);
	
	pvArray->vt=VT_ARRAY | VT_UI1;
	pvArray->parray=pDataArray;

	return hr;
}

/** Generates and returns a BSTR representing a GUID in the format: "{a978f...}" */
BSTR COMUtils::CreateGUID()
{
	// as the os for a guid structure
	GUID guid;
	CoCreateGuid(&guid);

	// convert it to a char buffer
	OLECHAR buffer[100];
	StringFromGUID2(guid, buffer, 100);
	
	return SysAllocString(buffer);
}


/** Checks if the caller is in the specified role */
HRESULT COMUtils::IsCallerInRole(BSTR role)
{
	//
	// Don't TRACE the entrance into this function, in order to 
	// better hide the security implementation details.
	//

	HRESULT hr;

	ISecurityCallContext *pISCC;
	hr = CoGetCallContext(IID_ISecurityCallContext, (void**)&pISCC);
	if (FAILED(hr)) {
		//TRACEhr(hr, " CoGetCallContext failed");
		return hr;
	}

	//
	// Make sure role-based security is enabled.
	//
	
	VARIANT_BOOL bIsSecurityEnabled;
	if (SUCCEEDED(hr))
		hr = pISCC->IsSecurityEnabled(&bIsSecurityEnabled);
	if (SUCCEEDED(hr))
		if (!bIsSecurityEnabled)
			hr = E_ACCESSDENIED;

	//
	// Make sure the caller is a member of the specified role.
	//
	
	VARIANT_BOOL bIsCallerInRole;
	if (SUCCEEDED(hr))
		hr = pISCC->IsCallerInRole(role, &bIsCallerInRole);
	if (SUCCEEDED(hr))
		if (!bIsCallerInRole)
			hr = E_ACCESSDENIED;		

	pISCC->Release();

	return hr;
}