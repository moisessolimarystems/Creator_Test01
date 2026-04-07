#pragma once

#include "safemutex.h"
#include <objidl.h>

// SLB 10.sep.2025 CR.34426.V01; To prevent conflicts with other target 
// implementations of GIT.h (it is ubiquitous in Solimar products), I put 
// licensing's implementation into a namespace.
namespace SSILicensing																					// CR.34426.V01; added namespace {}
{
	/*
	 * AutoReleasePtr
	 *	Automatically releases the attached interface when
	 *	the object goes out of scope.
	 */
	template<class Interface>
	class AutoReleasePtr
	{
	public:
		AutoReleasePtr(Interface* pItf) :
			m_pItf(pItf)
		{
			;
		}
		~AutoReleasePtr()
		{
			m_pItf->Release();
		}
		Interface* operator->()
		{
			return m_pItf;
		}
	private:
		Interface* m_pItf;
		AutoReleasePtr()
		{
			;
		} // avoid misuse
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
		typedef Interface _Interface;
	private:
		HANDLE m_hXLockMut;
		DWORD m_cookie;

	public:
		GITPtr(Interface* pItf) : m_cookie(0), m_hXLockMut(INVALID_HANDLE_VALUE)
		{
			m_hXLockMut = CreateMutex(NULL, FALSE, NULL);
			Attach(pItf);
		}
		GITPtr(int null) : m_cookie(0), m_hXLockMut(INVALID_HANDLE_VALUE)
		{
			m_hXLockMut = CreateMutex(NULL, FALSE, NULL);
		}
		GITPtr() : m_cookie(0), m_hXLockMut(INVALID_HANDLE_VALUE)
		{
			m_hXLockMut = CreateMutex(NULL, FALSE, NULL);
		}
		GITPtr(const GITPtr& r) : m_cookie(0), m_hXLockMut(INVALID_HANDLE_VALUE)
		{
			m_hXLockMut = CreateMutex(NULL, FALSE, NULL);
			*this = r;
		}

		~GITPtr()
		{
			Revoke();
			CloseHandle(m_hXLockMut);
			m_hXLockMut = INVALID_HANDLE_VALUE;
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
			SafeMutex lock(m_hXLockMut);
			HRESULT hr;
			hr = Revoke();
			if (FAILED(hr))
			{
				OutputDebugStringW(L"\nGITPtr::Attach() - Revoke() failed.");
			}
			else
			{
				IGlobalInterfaceTable* pIGIT = NULL;
				hr = CreateGIT(&pIGIT);
				if (FAILED(hr))
				{
					OutputDebugStringW(L"\nGITPtr::Attach() - CreateGIT() failed.");
				}
				else //if (SUCCEEDED(hr))
				{
					{
						SafeMutex lock(m_hXLockMut);
						hr = pIGIT->RegisterInterfaceInGlobal(
							static_cast<IUnknown*>(pItf),
							__uuidof(Interface),
							&m_cookie);
					}
					pIGIT->Release();
					if (FAILED(hr))
					{
						OutputDebugStringW(L"\nGITPtr::Attach() - pIGIT->RegisterInterfaceInGlobal() failed.");
					}
					else
					{
						pItf->Release();
					}
				}
			}
			return hr;
		}

		/*
		 * CopyTo()
		 * The interface from the GIT is copied to the passed pointer.
		 * The pointer must be released by the caller when it is no
		 * longer required.
		 *
		 * SLB 11.jun.2024 CR.32662.V03; Refactored code to catch errors and prevent
		 * Access Violation exceptions.
		 */
		HRESULT CopyTo(Interface** ppItf) const
		{
			*ppItf = NULL;

			if (!m_cookie)
			{
				// Not a problem: copying an empty GITPtr object.
				// Nothing needs to be done because *ppItf has already been set to NULL.
				return S_FALSE;
			}

			SafeMutex lock(m_hXLockMut);

			wchar_t wcsMsg[128];
			HRESULT hr(S_OK);
			IGlobalInterfaceTable* pIGIT = NULL;

			hr = CreateGIT(&pIGIT);
			if (FAILED(hr))
			{
				_snwprintf_s(wcsMsg, _TRUNCATE, L"\n[%06u] %s() - CreateGIT() failed.",
					GetCurrentThreadId(),
					__WFUNCTION__);
				OutputDebugStringW(wcsMsg);
				return hr;
			}

			try
			{
				hr = pIGIT->GetInterfaceFromGlobal(m_cookie, __uuidof(Interface), (void**)ppItf);
			}
			catch (...)
			{
				_snwprintf_s(wcsMsg, _TRUNCATE, L"\n[%06u] %s() - general exception while calling pIGIT->GetInterfaceFromGlobal() cookie(%08X)",
					GetCurrentThreadId(),
					__WFUNCTION__,
					m_cookie);
				OutputDebugStringW(wcsMsg);
				hr = E_FAIL;
			}
			if (FAILED(hr))
			{
				_snwprintf_s(wcsMsg, _TRUNCATE, L"\n[%06u] %s() - pIGIT->GetInterfaceFromGlobal() failed. hr(%08X), cookie(%08X)",
					GetCurrentThreadId(),
					__WFUNCTION__,
					hr,
					m_cookie);
				OutputDebugStringW(wcsMsg);
				return hr;
			}
			pIGIT->Release();
			return S_OK;
		}

		/*
		 * operator->
		 * Throws com_error exceptions.
		 *
		 * SLB 11.jun.2024 CR.32662.V03; Refactored code to catch errors and prevent
		 * Access Violation exceptions.
		 */
		AutoReleasePtr<Interface> operator->()
		{
			wchar_t wcsMsg[128];

			HRESULT hr(S_OK);
			Interface* pItf = NULL;
			hr = CopyTo(&pItf);
			if (hr == S_FALSE) hr = E_POINTER;
			if (FAILED(hr))
			{
				_snwprintf_s(wcsMsg, _TRUNCATE, L"\n[%06u] %s() -  CopyTo() failed. Exception will be thrown.",
					GetCurrentThreadId(),
					__WFUNCTION__);
				OutputDebugStringW(wcsMsg);
				throw (_com_error(hr));
			}

			// CR.32662.V06; check for pItf == NULL. Don't want to AutoRelease a NULL ptr.
			if (!pItf)
			{
				_snwprintf_s(wcsMsg, _TRUNCATE, L"\n[%06u] %s() - Problem. Dereferencing an empty GIT object.",
					GetCurrentThreadId(),
					__WFUNCTION__);
				OutputDebugStringW(wcsMsg);

				throw (_com_error(E_POINTER));
			}

			return AutoReleasePtr<Interface>(pItf);
		}

		/*
		 * operator=
		 */
		GITPtr& operator=(const GITPtr& r)
		{
			wchar_t wcsMsg[128];

			SafeMutex lock(m_hXLockMut);
			HRESULT hr(S_OK);
			Interface* pItf = NULL;

			hr = r.CopyTo(&pItf);
			if (FAILED(hr))
			{
				_snwprintf_s(wcsMsg, _TRUNCATE, L"\n[%06u] %s() - CopyTo() failed. Exception will be thrown.",
					GetCurrentThreadId(),
					__WFUNCTION__);
				OutputDebugStringW(wcsMsg);
				throw (_com_error(hr));
			}

			// If empty object, then no interface to attach to. exit.
			if (!pItf) return *this;

			hr = Attach(pItf);
			if (FAILED(hr))
			{
				_snwprintf_s(wcsMsg, _TRUNCATE, L"\n[%06u] %s() - Attach() failed. Exception will be thrown.",
					GetCurrentThreadId(),
					__WFUNCTION__);
				OutputDebugStringW(wcsMsg);
				throw (_com_error(hr));
			}

			return *this;
		}

		/*
		 * operator!=
		 * Compare NULL
		 */
		operator bool() const
		{
			return m_cookie != 0;
		}


		/*
		 * CreateInstance()
		 * Creates locally typed interface for the provided CLSID.
		 *
		 * SLB 21.feb.2024 CR.32662; refactored method to immediatley return on error.
		 * Added check in case Attach() fails.
		 */
		HRESULT CreateInstance(
			const CLSID& rclsid,
			IUnknown* pOuter = NULL,
			DWORD dwClsContext = CLSCTX_ALL)
		{
			SafeMutex lock(m_hXLockMut);
			Interface* pItf = 0;
			HRESULT hr = CoCreateInstance(rclsid, pOuter, dwClsContext, __uuidof(Interface), reinterpret_cast<void**>(&pItf));
			if (FAILED(hr)) { OutputDebugStringW(L"\nGITPtr::CreateInstance() - CoCreateInstance() failed."); return hr; }	// CR.32662; modified.

			hr = Attach(pItf);																		// CR.32662; modified.
			if (FAILED(hr)) { pItf->Release(); OutputDebugStringW(L"\nGITPtr::CreateInstance() - Attach() failed."); return hr; } // CR.32662; added.

			return S_OK;
		}

		/* CreateInstanceEx() - provides clean wrapper for CoCreateInstanceEx()
		 * if only one interface is desired.
		 *
		 * SLB 01.feb.2024 CR.32662; created for SolimarLicenseMgr class.
		--------------------------------------------------------------------------------*/
		HRESULT CreateInstanceEx(
			const CLSID& rclsid,
			const BSTR& bstrServer,
			IUnknown* pOuter = NULL,
			COAUTHINFO* pAuthInfo = NULL,
			DWORD dwClsContext = CLSCTX_ALL)
		{
			SafeMutex lock(m_hXLockMut);

			HRESULT hr;
			COSERVERINFO serverInfo = { 0, bstrServer, pAuthInfo, 0 };
			MULTI_QI multiQI = { &__uuidof(Interface), NULL, NOERROR };
			hr = CoCreateInstanceEx(rclsid, pOuter, dwClsContext, &serverInfo, 1, &multiQI);
			if (FAILED(hr)) { OutputDebugStringW(L"\nGITPtr::CreateInstanceEx() - CoCreateInstanceEx() failed."); return hr; }
			if (FAILED(multiQI.hr)) { OutputDebugStringW(L"\nGITPtr::CreateInstanceEx() - CoCreateInstanceEx(2) failed."); return multiQI.hr; }

			hr = Attach(reinterpret_cast<Interface*>(multiQI.pItf));
			if (FAILED(hr)) { OutputDebugStringW(L"\nGITPtr::CreateInstanceEx() - Attach() failed."); return hr; }

			return S_OK;
		}

		/*
		* QueryInterface()
		* Allows for GITPtr output param.
		*/
		template<typename _OtherIIID> HRESULT QueryInterface(const IID& iid, GITPtr<_OtherIIID>& p)
		{
			SafeMutex lock(m_hXLockMut);
			Interface* pItf;
			DWORD dwItfRefCount(0);
			HRESULT hr = CopyTo(&pItf);
			if (FAILED(hr))
			{
				OutputDebugStringW(L"\nGITPtr::CreateInstance() - CopyTo() failed.");
			}
			else if (!pItf)
			{
				hr = E_NOINTERFACE;
				OutputDebugStringW(L"\nGITPtr::CreateInstance() - CopyTo() failed.");
			}
			else //if (SUCCEEDED(hr))
			{
				_OtherIIID* pOtherItf = NULL;
				hr = pItf->QueryInterface(iid, reinterpret_cast<void**>(&pOtherItf));
				dwItfRefCount = pItf->Release();
				if (!dwItfRefCount)
				{
					OutputDebugStringW(L"\nDebug -> Refcount == 0");
				}
				if (FAILED(hr))
				{
					OutputDebugStringW(L"\nGITPtr::CreateInstance() - QueryInterface() failed.");
				}
				else // if (SUCCEEDED(hr))
				{
					hr = p.Attach(pOtherItf);
					if (FAILED(hr))
					{
						OutputDebugStringW(L"\nGITPtr::CreateInstance() - Attach() failed.");
					}
				}
			}
			return hr;
		}

	private:

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

			if (!m_cookie)
			{  //
				// Not a problem.
				// No interface registered to revoke.
				//
			}
			else
			{
				IGlobalInterfaceTable* pIGIT = NULL;

				hr = CreateGIT(&pIGIT);
				if (FAILED(hr))
				{
					OutputDebugStringW(L"\nGITPtr::Revoke() - CreateGIT() failed.");
					SafeMutex lock(m_hXLockMut);
					m_cookie = 0;
				}
				else //if (SUCCEEDED(hr))
				{
					{
						SafeMutex lock(m_hXLockMut);
						hr = pIGIT->RevokeInterfaceFromGlobal(m_cookie);
						if (FAILED(hr))
						{
							OutputDebugStringW(L"\nGITPtr::Revoke() - RevokeInterfaceFromGlobal() failed.");
						}
						m_cookie = 0;
					}
					pIGIT->Release();
				}
			}
			return hr;
		}
	};
}

