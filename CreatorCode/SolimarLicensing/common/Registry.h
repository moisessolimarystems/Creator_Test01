#ifndef __Registry6923976079082_H
#define __Registry6923976079082_H

#include <windows.h>
#include <comdef.h>

/* A class to encapsulate basic registry operations */

/* By default, a 32-bit application running on WOW64 accesses the 32-bit registry view and a 64-bit application accesses 
	the 64-bit registry view. The following flags enable 32-bit applications to access reflected keys in the 64-bit 
	registry view and 64-bit applications to access reflected keys in the 32-bit registry view.*/

class Registry
{
public:
	// old interfaces using byte buffers
	static long GetValueDWORD(HKEY root, const wchar_t* key, const wchar_t* valuename, LPDWORD value /*out*/);
	static long GetValueString(HKEY root, const wchar_t* key, const wchar_t* valuename, LPBYTE* ppBuffer, DWORD initialBufferSize);
	//static long GetValueSize(HKEY root, LPCSTR key, LPCSTR valuename, LPDWORD size /*out*/);
	
	// new interfaces using _variant_t and returning HRESULT
	static HRESULT GetValue(HKEY root, _bstr_t key, _bstr_t valuename, _variant_t &value, REGSAM samDesired=0);

	// creates the key if it does not exist, and overwrites existing values if they do.
	static HRESULT SetValue(HKEY root, _bstr_t key, _bstr_t valuename, _variant_t value, REGSAM samDesired=0);
	/* Enumerates sub keys of a given registry key. Returns S_FALSE when no more sub keys are found. */
	static HRESULT EnumKey(HKEY root, BSTR key, DWORD index, _bstr_t &subKey, REGSAM samDesired=0);

	/* Enumerates the values of a given registry key. Returns S_FALSE when no more values are found */
	static HRESULT EnumKeyValues(HKEY root, BSTR key, DWORD index, _bstr_t &subKey, REGSAM samDesired=0);

	/* Deletes a key and all of its subkeys and values */
	static HRESULT DeleteKey(HKEY root, BSTR parentKey, BSTR keyname, REGSAM samDesired=0);
	/* Deletes a registry value */
	static HRESULT DeleteValue(HKEY root, BSTR parentKey, BSTR valuename, REGSAM samDesired=0);
};


#endif 


