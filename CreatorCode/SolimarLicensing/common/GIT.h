#ifndef __GIT_H
#define __GIT_H

#include "safemutex.h"

/*
 * AutoReleasePtr
 *	Automatically releases the attached interface when
 *	the object goes out of scope.
 */
template<class Interface>
class AutoReleasePtr
{
public:
	AutoReleasePtr(Interface* pItf) : m_pItf(pItf) {}
	~AutoReleasePtr() { m_pItf->Release(); }
	Interface* operator->() { return m_pItf; }
private:
	Interface* m_pItf;
	AutoReleasePtr() {} // avoid misuse
};

/*
 * GITPtr
 *	This class provides methods for dealing with interface pointers 
 *	and the global interface table (GIT).
 *	NOTE: This class is thread safe.
 */
template<class Interface>
class GITPtr 
{
public:
	GITPtr(Interface* pItf) : m_cookie(0) 
	{ 
		m_hXLockMut = CreateMutex(NULL, FALSE, NULL);
		Attach(pItf); 
	}
	GITPtr(int null) : m_cookie(0) 
	{
		m_hXLockMut = CreateMutex(NULL, FALSE, NULL);
	}
	GITPtr() : m_cookie(0) 
	{ 
		m_hXLockMut = CreateMutex(NULL, FALSE, NULL); 
	}
	GITPtr(const GITPtr& r) : m_cookie(0)
	{
		m_hXLockMut = CreateMutex(NULL, FALSE, NULL); 
		*this = r;
	}

	~GITPtr()
	{
		Revoke();
		CloseHandle(m_hXLockMut);
	}

	/*
	 * Attach()
	 *	Call this method to register the interface pointer 
	 *	in the global interface table (GIT). 
	 *
	 *	Takes ownership of the interface. Because RegisterInterfaceInGlobal
	 *	AddRefs the interface, this function will immediately call Release
	 *  on the interface once it has been successfully registered in the GIT.
	 */
	HRESULT Attach(Interface* pItf)
	{
		HRESULT hr;
		hr = Revoke();
		if (FAILED(hr))
			return hr;

		IGlobalInterfaceTable* pIGIT = NULL;
		hr = CreateGIT(&pIGIT);
		if (SUCCEEDED(hr))
		{
			{
				SafeMutex lock(m_hXLockMut);
				hr = pIGIT->RegisterInterfaceInGlobal(
					static_cast<IUnknown*>(pItf),
					__uuidof(Interface),
					&m_cookie);
			}
			pIGIT->Release();
			if (SUCCEEDED(hr))
				pItf->Release();
		}

		return hr;
	}

	/*
	 * CopyTo()
	 * The interface from the GIT is copied to the passed pointer. 
	 * The pointer must be released by the caller when it is no 
	 * longer required.
	 */
	HRESULT CopyTo(Interface** ppItf) const
	{
		*ppItf = NULL;

		IGlobalInterfaceTable* pIGIT = NULL;
		HRESULT hr = CreateGIT(&pIGIT);
		if (SUCCEEDED(hr))
		{
			{
				SafeMutex lock(m_hXLockMut);
				if (m_cookie)
				{
					hr = pIGIT->GetInterfaceFromGlobal(
						m_cookie,
						__uuidof(Interface),
						(void**)ppItf);
				}
				else
				{
					hr = E_NOINTERFACE;
				}
			}
			pIGIT->Release();
		}

		return hr;
	}

	/*
	 * operator->
	 */
	AutoReleasePtr<Interface> operator->()
	{
		Interface* pItf = NULL;
		CopyTo(&pItf);
		return AutoReleasePtr<Interface>(pItf);
	}

	/*
	 * operator=
	 */
	GITPtr& operator=(const GITPtr& r) 
	{
		Interface* pItf = NULL;
		if (SUCCEEDED(r.CopyTo(&pItf)))
		{
			Attach(pItf);
		}
		return *this;
	}

private:

	HANDLE m_hXLockMut;
	DWORD m_cookie;

	/*
	 * CreateGIT()
	 *	Creates a GIT object.
	 */
	HRESULT CreateGIT(IGlobalInterfaceTable** ppIGIT) const
	{
		return CoCreateInstance(
			CLSID_StdGlobalInterfaceTable,
			0,
			CLSCTX_INPROC_SERVER,
			IID_IGlobalInterfaceTable,
			(void**)ppIGIT);
	}
	
	/*
	 * Revoke()
	 *	Call this method to remove the current interface from the 
	 *	global interface table (GIT).
	 */
	HRESULT Revoke()
	{
		HRESULT hr = S_OK;
		IGlobalInterfaceTable* pIGIT;
		hr = CreateGIT(&pIGIT);
		if (SUCCEEDED(hr))
		{
			{
				SafeMutex lock(m_hXLockMut);
				if (m_cookie)
				{
					hr = pIGIT->RevokeInterfaceFromGlobal(m_cookie);
				}
				m_cookie = 0;
			}
			pIGIT->Release();
		}
		return hr;
	}
};

#endif
