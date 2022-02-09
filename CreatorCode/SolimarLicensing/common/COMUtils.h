#ifndef __comutils_39809_12983798_198273987_h
#define __comutils_39809_12983798_198273987_h

#pragma warning(disable : 4786)
#include <deque>
#include <comsvcs.h>     // Specifies COM+ methods and functions

class COMUtils
{
public:
	
	/** Specifies dimensions of a multidimensional safe array */
	typedef std::deque<unsigned int> DimensionsType;
	
	/** Creates a multidimensional array of arrays
	  * where each array is a variant safearray, 
	  * and the non-array elements are variants
	  * Arguments:
	  *		array		provide pointer to a variant 
	  *					object that will receive the array
	  *		dimensions	listing of the size of each dimension
	  */
	static HRESULT CreateMultidimensionalSafearray(VARIANT* array, DimensionsType &dimensions);
	
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
	static HRESULT AccessMultidimensionalSafearray(VARIANT* array, DimensionsType &indices, VARIANT** element);

	/** Provides releases the element obtained with a call to Access...
	  * Decrements reference counts, etc.
	  */
	static HRESULT UnaccessMultidimensionalSafearray(VARIANT* array, DimensionsType &indices);
	
	/** Calls the SafeArrayDelete on all the internal arrays recursively */
	static HRESULT DeleteMultidimensionalSafearray(VARIANT* array);

	/** Gets the dimensions of a multidimensional safearray. *** indices must be empty/clear() ed *** */
	static HRESULT SizeofMultidimensionalSafearray(VARIANT* array, DimensionsType &indices);
	
	/** Sets a value in the safe array using VariantCopy */
	static HRESULT SetMultidimensionalSafearrayValue(VARIANT* array, DimensionsType &indices, VARIANT *value);
	
	/** Reads raw data from a file in to a safe array */
	static HRESULT CreateSafeArrayFromFile(VARIANT* pvArray, BSTR path);

	/** Reads data from a buffer in to a safe array */
	static HRESULT CreateSafeArrayFromBuffer(VARIANT* pvArray, void* buffer, unsigned int buffersize);
	
	/** Generates and returns a BSTR representing a GUID in the format: "{a978f...}" */
	static BSTR CreateGUID();
	
	/** Checks if the caller is in the specified role */
	static HRESULT IsCallerInRole(BSTR role);

	static HRESULT SetProxyBlanket(IUnknown* pItf, COAUTHINFO* pAuthInfo, bool setIUnknown = false);

private:
};

/*
 * MarshalInterface
 */
template<class TObject, const IID* TObjectIID, DWORD TMshCtx>
class MarshalInterface {
public:
	/* Pass the interface to be marshalled */
	MarshalInterface(IUnknown* pItf) : m_hGlobal(NULL)
	{
		m_hr = Marshal(pItf);
	}
	/* Cleans up the marshalled data packet */
	~MarshalInterface()
	{
		ReleaseMarshal();
	}
	/* 
	 * Returns an unmarshalled interface pointer. Caller is
	 * responsible for calling Release().
	 */
	HRESULT QueryInterface(REFIID riid, LPVOID* pObject)
	{
		HRESULT hr;
		if (m_hr != S_OK)
			return m_hr;
		TObject* _pObject = NULL;
		hr = UnMarshal(&_pObject);
		if (FAILED(hr))
			return hr;
		/* If the riid is different then the one marshalled, query for new itf */
		if (riid == *TObjectIID) 
			*pObject = _pObject;
		else { 
			hr = _pObject->QueryInterface(riid, pObject);
			_pObject->Release();
		}
		return hr;
	}
private:
	HRESULT		m_hr;
	HGLOBAL		m_hGlobal;

	/*
	 * Writes into a stream the data required to initialize a proxy 
	 * object in some client process.
	 */
	HRESULT Marshal(IUnknown* pItf)
	{
		HRESULT hr;
		IStream *pStm = NULL;
		hr = CreateStreamOnHGlobal(0, FALSE, &pStm);
		if (SUCCEEDED(hr)) {
			hr = CoMarshalInterface(
					pStm,
					*TObjectIID,
					pItf,
					TMshCtx,
					0,
					MSHLFLAGS_TABLESTRONG);
			if (SUCCEEDED(hr)) 
				hr = GetHGlobalFromStream(pStm, &m_hGlobal);
			pStm->Release();
		}
		return hr;
	}
	/* 
	 * Initializes a newly created proxy using data written into the 
	 * stream by a previous call to Marshal.
	 */
	HRESULT UnMarshal(TObject** ppObject)
	{
		HRESULT hr;
		IStream *pStm = NULL;
		if (m_hGlobal) {
			hr = CreateStreamOnHGlobal(m_hGlobal, FALSE, &pStm);
			if (SUCCEEDED(hr)) {
				hr = CoUnmarshalInterface(pStm, *TObjectIID, (void**)ppObject);
				pStm->Release();
			}
		}
		return hr;
	}
	/* Destroys a previously marshaled data packet */
	void ReleaseMarshal()
	{
		HRESULT hr;
		if (m_hGlobal) {
			IStream *pStm = 0;
			hr = CreateStreamOnHGlobal(m_hGlobal, FALSE, &pStm);
			if (SUCCEEDED(hr)) {
				CoReleaseMarshalData(pStm);
				pStm->Release();
			}
			m_hGlobal = NULL;
		}
	}
};

#endif

