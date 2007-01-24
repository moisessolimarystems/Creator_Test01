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
      m_hXLockMut = INVALID_HANDLE_VALUE ;
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
         OutputDebugStringW(L"\nGITPtr::Attach() - Revoke() failed.") ;
      }
      else
      {
		   IGlobalInterfaceTable* pIGIT = NULL;
		   hr = CreateGIT(&pIGIT);
         if (FAILED(hr))
         {
            OutputDebugStringW(L"\nGITPtr::Attach() - CreateGIT() failed.") ;
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
               OutputDebugStringW(L"\nGITPtr::Attach() - pIGIT->RegisterInterfaceInGlobal() failed.") ;
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
	 */
	HRESULT CopyTo(Interface** ppItf) const
	{
      SafeMutex lock(m_hXLockMut);
      HRESULT hr(S_OK) ;
		*ppItf = NULL;

		IGlobalInterfaceTable* pIGIT = NULL;
      if (!m_cookie)
      {  //
         // Not a problem: copying an empty GITPtr object.
         // Nothing needs to be done because *ppItf has already been set to NULL.
         //
         hr = S_FALSE ;
      }
      else
      {
		   hr = CreateGIT(&pIGIT);
         if (FAILED(hr))
         {
            OutputDebugStringW(L"\nGITPtr::CopyTo() - CreateGIT() failed.") ;
         }
         else //if (SUCCEEDED(hr))
		   {
            try
			   {
				   SafeMutex lock(m_hXLockMut);

               try
               {
               hr = pIGIT->GetInterfaceFromGlobal(
						m_cookie, 
						__uuidof(Interface),
						(void**)ppItf);
               }
               catch (...)
               {
                  wchar_t wcsMsg[128] ;
                  _snwprintf(
                     wcsMsg, sizeof(wcsMsg),
                     L"\nGITPtr::CopyTo() - general exception while calling pIGIT->GetInterfaceFromGlobal() cookie(%08X)",
                     m_cookie
                     ) ;
                  OutputDebugStringW(wcsMsg) ;
                  hr = E_FAIL ;
               }
               if (FAILED(hr))
               {
                  wchar_t wcsMsg[128] ;
                  _snwprintf(
                     wcsMsg, sizeof(wcsMsg),
                     L"\nGITPtr::CopyTo() - pIGIT->GetInterfaceFromGlobal() failed. hr(%08X), cookie(%08X)",
                     hr,
                     m_cookie
                     ) ;
                  OutputDebugStringW(wcsMsg) ;
                }
			   }
            catch (...)
            {
               wchar_t wcsMsg[128] ;
               _snwprintf(
                  wcsMsg, sizeof(wcsMsg),
                  L"\nGITPtr::CopyTo() - General exception caught hr(%08X) cookie(%08X)",
                  hr,
                  m_cookie
                  ) ;
               OutputDebugStringW(wcsMsg) ;
               hr = E_FAIL ;
            }
			   pIGIT->Release();
		   }
      }
		return hr;
	}

	/*
	 * operator->
	 */
	AutoReleasePtr<Interface> operator->()
	{
      HRESULT hr(S_OK) ;
		Interface* pItf = NULL;
		hr = CopyTo(&pItf);
      if (FAILED(hr))
      {
         OutputDebugStringW(L"\nGITPtr::operator->() - CopyTo() failed. Exception will be thrown.") ;
         _com_error e(hr) ;
         throw(e) ;
      }
      
		return AutoReleasePtr<Interface>(pItf);
	}

	/*
	 * operator=
	 */
	GITPtr& operator=(const GITPtr& r) 
	{
      SafeMutex lock(m_hXLockMut);
      HRESULT hr(S_OK) ;
		Interface* pItf = NULL;

      hr = r.CopyTo(&pItf) ;
      if (FAILED(hr))
      {
         OutputDebugStringW(L"\nGITPtr::operator=() - CopyTo() failed. Exception will be thrown.") ;
      }
      else if (pItf) //if (SUCCEEDED(hr))
		{
			hr = Attach(pItf);
         if (FAILED(hr))
         {
            OutputDebugStringW(L"\nGITPtr::operator=() - Attach() failed. Exception will be thrown.") ;
         }
		}

      if (FAILED(hr))
      {
         _com_error e(hr) ;
         throw(hr) ;
      }
		return *this;
	}

	/*
	 * operator!=
	 * Compare NULL
	 */
    operator bool() const
    {
      return m_cookie!=0 ;
    }


	/* 
	 * CreateInstance()
    * Creates locally typed interface for the provided CLSID.
	 */
    HRESULT CreateInstance(const CLSID& rclsid, IUnknown* pOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
    {
      SafeMutex lock(m_hXLockMut);
		Interface* pInterface = 0;		
		HRESULT hr = CoCreateInstance(
			rclsid, 
			pOuter, 
			dwClsContext, 
			__uuidof(Interface), 
			reinterpret_cast<void**>(&pInterface));
		if (FAILED(hr))
      {  
         OutputDebugStringW(L"\nGITPtr::CreateInstance=() - CoCreateInstance() failed.") ;
      }
      else //if (SUCCEEDED(hr)) 
      {
			Attach(pInterface);		
      }
		return hr;
    }

    /*
	 * QueryInterface()
	 * Allows for GITPtr output param.
	 */
	template<typename _OtherIIID> HRESULT QueryInterface(const IID& iid, GITPtr<_OtherIIID>& p)
	{
      SafeMutex lock(m_hXLockMut);
		Interface* pItf;
      DWORD dwItfRefCount(0) ;
		HRESULT hr = CopyTo(&pItf);
		if (FAILED(hr))
      {  
         OutputDebugStringW(L"\nGITPtr::CreateInstance() - CopyTo() failed.") ;
      }
		else if (!pItf)
      {
         hr = E_NOINTERFACE ;
         OutputDebugStringW(L"\nGITPtr::CreateInstance() - CopyTo() failed.") ;
      }
      else //if (SUCCEEDED(hr))
		{
			_OtherIIID* pOtherItf = NULL;
			hr = pItf->QueryInterface(iid, reinterpret_cast<void**>(&pOtherItf));
			dwItfRefCount = pItf->Release();
         if (!dwItfRefCount)
         {
            OutputDebugStringW(L"\nDebug -> Refcount == 0") ;
         }
         if (FAILED(hr))
         {
            OutputDebugStringW(L"\nGITPtr::CreateInstance() - QueryInterface() failed.") ;
         }
         else // if (SUCCEEDED(hr))
			{
				hr = p.Attach(pOtherItf);
            if (FAILED(hr))
            {
               OutputDebugStringW(L"\nGITPtr::CreateInstance() - Attach() failed.") ;
            }
			}
		}
		return hr;	
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

      if (!m_cookie)
      {  //
         // Not a problem.
         // No interface registered to revoke.
         //
      }
      else
      {
		   IGlobalInterfaceTable* pIGIT(NULL) ;

		   hr = CreateGIT(&pIGIT);
         if (FAILED(hr))
         {
            OutputDebugStringW(L"\nGITPtr::Revoke() - CreateGIT() failed.") ;
            SafeMutex lock(m_hXLockMut);
            m_cookie = 0 ;
         }
         else //if (SUCCEEDED(hr))
		   {
			   {
				   SafeMutex lock(m_hXLockMut);
               hr = pIGIT->RevokeInterfaceFromGlobal(m_cookie);
               if (FAILED(hr))
               {
                  OutputDebugStringW(L"\nGITPtr::Revoke() - RevokeInterfaceFromGlobal() failed.") ;
               }
				   m_cookie = 0;
			   }
			   pIGIT->Release();
		   }
      }
		return hr;
	}
};

#endif
