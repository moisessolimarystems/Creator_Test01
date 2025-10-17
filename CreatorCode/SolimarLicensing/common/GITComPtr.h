#ifndef __GIT_Com_Ptr_EB27BF47_8DC1_452c_B007_CD2636610ACC__
#define __GIT_Com_Ptr_EB27BF47_8DC1_452c_B007_CD2636610ACC__

// This class is styled after the _com_ptr_t class 

#include <comdef.h>
#include <comutil.h>
//#pragma warning(push)
//#pragma warning(disable: 4290)

#define _GIT_COM_SMARTPTR_TYPEDEF(Interface, IID) typedef GITComPtr< _com_IIID<Interface, &IID> > Interface ## Ptr

template <typename _IIID> class GITComPtr
{
public:
    // Declare interface type so that the type may be available outside
    // the scope of this template.
    //
    typedef _IIID ThisIIID;
    typedef typename _IIID::Interface Interface;
	
    // When the compiler supports references in template parameters,
    // _CLSID will be changed to a reference.  To avoid conversion
    // difficulties this function should be used to obtain the
    // CLSID.
    //
    static const IID& GetIID() throw()
    { 
        return ThisIIID::GetIID(); 
    }

    // Constructs a smart-pointer from any other smart pointer.
    //
    template<typename _OtherIIID> GITComPtr(const GITComPtr<_OtherIIID>& p) 
        : m_dwCookie(0)
    {
        HRESULT hr = _QueryInterface(p);
		
        if (FAILED(hr) && (hr != E_NOINTERFACE)) {
            _com_issue_error(hr);
        }
    }

    // Constructs a smart-pointer from any IUnknown-based interface pointer.
    //
    template<typename _InterfaceType> GITComPtr(_InterfaceType* p) 
        : m_dwCookie(0)
    {
        HRESULT hr = _QueryInterface(p);

        if (FAILED(hr) && (hr != E_NOINTERFACE)) {
            _com_issue_error(hr);
        }
    }

    // Disable conversion using GITComPtr* specialization of
    // template<typename _InterfaceType> GITComPtr(_InterfaceType* p)
    //
    template<> explicit GITComPtr(GITComPtr* p) 
        : m_dwCookie(0)
    {
        if (p == NULL) {
            _com_issue_error(E_POINTER);
        }
        else {
			// extract the interface pointer from p, and add it to the GIT
			InsertInterfacePointer(p->ExtractInterfacePointer());
        }
    }

    // Default constructor.
    //
    GITComPtr() throw()
        : m_dwCookie(0)
    {
		;
    }

    // This constructor is provided to allow NULL assignment. It will issue
    // an error if any value other than null is assigned to the object.
    //
    GITComPtr(int null) 
        : m_dwCookie(0)
    {
        if (null != 0) {
            _com_issue_error(E_POINTER);
        }
    }

    // Copy the pointer and AddRef().
    //
    //template<> GITComPtr(const GITComPtr& cp) throw()	//C2299 changes made to build in visual studios 2005
	GITComPtr(const GITComPtr& cp) throw()
        : m_dwCookie(0)
    {
		InsertInterfacePointer(cp.ExtractInterfacePointer());
    }

    // Saves the interface.
    //
    template<> GITComPtr(Interface* pInterface) throw()
        : m_dwCookie(0)
    {
		InsertInterfacePointer(pInterface);
    }

    // Copies the pointer. If fAddRef is TRUE, the interface will
    // be AddRef()ed.
    //
    GITComPtr(Interface* pInterface, bool fAddRef) throw()
        : m_dwCookie(0)
    {
		InsertInterfacePointer(pInterface);
        if (fAddRef) {
            _AddRef();
        }
    }

    // Construct a pointer for a _variant_t object.
    //
    GITComPtr(const _variant_t& varSrc) 
        : m_dwCookie(0)
    {
//yyy
        HRESULT hr = QueryStdInterfaces(varSrc);

        if (FAILED(hr) && (hr != E_NOINTERFACE)) {
            _com_issue_error(hr);
        }
    }

    // Calls CoCreateClass with the provided CLSID.
    //
    explicit GITComPtr(const CLSID& clsid, IUnknown* pOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) 
        : m_dwCookie(0)
    {
        HRESULT hr = CreateInstance(clsid, pOuter, dwClsContext);

        if (FAILED(hr) && (hr != E_NOINTERFACE)) {
            _com_issue_error(hr);
        }
    }

    // Calls CoCreateClass with the provided CLSID retrieved from
    // the string.
    //
    explicit GITComPtr(LPCWSTR str, IUnknown* pOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) 
        : m_dwCookie(0)
    {
        HRESULT hr = CreateInstance(str, pOuter, dwClsContext);

        if (FAILED(hr) && (hr != E_NOINTERFACE)) {
            _com_issue_error(hr);
        }
    }

    // Calls CoCreateClass with the provided SBCS CLSID retrieved from
    // the string.
    //
    explicit GITComPtr(LPCSTR str, IUnknown* pOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) 
        : m_dwCookie(0)
    {
        HRESULT hr = CreateInstance(str, pOuter, dwClsContext);

        if (FAILED(hr) && (hr != E_NOINTERFACE)) {
            _com_issue_error(hr);
        }
    }

    // Queries for interface.
    //
    template<typename _OtherIIID> GITComPtr& operator=(const GITComPtr<_OtherIIID>& p) 
    {
        HRESULT hr = _QueryInterface(p);

        if (FAILED(hr) && (hr != E_NOINTERFACE)) {
            _com_issue_error(hr);
        }

        return *this;
    }

    // Queries for interface.
    //
    template<typename _InterfaceType> GITComPtr& operator=(_InterfaceType* p) 
    {
        HRESULT hr = _QueryInterface(p);
    
        if (FAILED(hr) && (hr != E_NOINTERFACE)) {
            _com_issue_error(hr);
        }

        return *this;
    }

    // Saves the interface.
    //
    template<> GITComPtr& operator=(Interface* pInterface) throw()
    {
		InsertInterfacePointer(pInterface);
        return *this;
    }
	
    // Copies and AddRef()'s the interface.
    //
    //template<> GITComPtr& operator=(const GITComPtr& cp) throw()//C2299 changes made to build in visual studios 2005
	GITComPtr& operator=(const GITComPtr& cp) throw()
    { 
        return operator=(cp.ExtractInterfacePointer());
    }

    // This operator is provided to permit the assignment of NULL to the class.
    // It will issue an error if any value other than NULL is assigned to it.
    //
    GITComPtr& operator=(int null) 
    {
        if (null != 0) {
            _com_issue_error(E_POINTER);
        }

        return operator=(reinterpret_cast<Interface*>(NULL));
    }

    // Construct a pointer for a _variant_t object.
    //
    GITComPtr& operator=(const _variant_t& varSrc) 
    {
        HRESULT hr = QueryStdInterfaces(varSrc);

        if (FAILED(hr) && (hr != E_NOINTERFACE)) {
            _com_issue_error(hr);
        }

        return *this;
    }

    // If we still have an interface then Release() it. The interface
    // may be NULL if Detach() has previously been called, or if it was
    // never set.
    //
    ~GITComPtr() throw()
    {
		RemoveInterfacePointer();
        //xxx release here? Does the remove from GIT affect the reference count?
		//xxx _Release();
    }
	
    // Saves/sets the interface without AddRef()ing. This call
    // will release any previously acquired interface.
    //
    void Attach(Interface* pInterface) throw()
    {
		Attach(pInterface, false);
    }

    // Saves/sets the interface only AddRef()ing if fAddRef is TRUE.
    // This call will release any previously acquired interface.
    //
    void Attach(Interface* pInterface, bool fAddRef) throw()
    {
		//xxx this affects refcount strategy
		//xxx InsertInterfacePointer does take ownership of interface - probably calls AddRef
		InsertInterfacePointer(pInterface);

		if (pInterface == NULL)
			_com_issue_error(E_POINTER);
		if (!fAddRef)
			pInterface->Release();
    }

    // Simply NULL the interface pointer so that it isn't Released()'ed.
    //
    Interface* Detach() throw()
    {
		Interface* const old = ExtractInterfacePointer();
		old->AddRef();
		RemoveInterfacePointer();
		return old;
    }

    // Return the interface. This value may be NULL.
    //
    operator Interface*() const throw()
    { 
        return ExtractInterfacePointer(); 
    }

    // Queries for the unknown and return it
    // Provides minimal level error checking before use.
    //
    operator Interface&() const 
    {
        Interface* pInterface = ExtractInterfacePointer();
        if (pInterface == NULL) {
            _com_issue_error(E_POINTER);
        }

        return *pInterface; 
    }

    // Allows an instance of this class to act as though it were the
    // actual interface. Also provides minimal error checking.
    //
    Interface& operator*() const 
    { 
        Interface* pInterface = ExtractInterfacePointer();
        if (pInterface == NULL) {
            _com_issue_error(E_POINTER);
        }

        return *pInterface; 
    }
	
	/*
    // Returns the address of the interface pointer contained in this
    // class. This is useful when using the COM/OLE interfaces to create
    // this interface.
    //
    Interface** operator&() throw()
    {
        _Release();
        m_pInterface = NULL;
        return &m_pInterface;
    }
	*/

    // Allows this class to be used as the interface itself.
    // Also provides simple error checking.
    //
    Interface* operator->() const 
    { 
        Interface* pInterface = ExtractInterfacePointer();
        if (pInterface == NULL) {
            _com_issue_error(E_POINTER);
        }

        return pInterface; 
    }

    // This operator is provided so that simple boolean expressions will
    // work.  For example: "if (p) ...".
    // Returns TRUE if the pointer is not NULL.
    //
    operator bool() const throw()
    { 
        return m_dwCookie != 0; 
    }

    // Compare two smart pointers
    //
    template<typename _OtherIIID> bool operator==(const GITComPtr<_OtherIIID>& p) 
    {
        return _CompareUnknown(p) == 0;
    }

    // Compare two smart pointers
    //
    template<typename _OtherIIID> bool operator==(GITComPtr<_OtherIIID>& p) 
    {
        return _CompareUnknown(p) == 0;
    }

    // Compare two pointers
    //
    template<typename _InterfaceType> bool operator==(_InterfaceType* p) 
    {
        return _CompareUnknown(p) == 0;
    }

    // Compare with other interface
    //
    template<> bool operator==(Interface* p) 
    {
        return (ExtractInterfacePointer() == p) 
                    ? true
                    : _CompareUnknown(p) == 0;
    }

    // Compare two smart pointers
    //
    template<> bool operator==(const GITComPtr& p) throw()
    {
        return operator==(p.ExtractInterfacePointer());
    }

    // Compare two smart pointers
    //
    template<> bool operator==(GITComPtr& p) throw()
    {
        return operator==(p.ExtractInterfacePointer());
    }

    // For comparison to NULL
    //
    bool operator==(int null) 
    {
        if (null != 0) {
            _com_issue_error(E_POINTER);
        }

        return m_dwCookie == NULL || ExtractInterfacePointer() == NULL;
    }

    // Compare two smart pointers
    //
    template<typename _OtherIIID> bool operator!=(const GITComPtr<_OtherIIID>& p) 
    {
        return !(operator==(p));
    }

    // Compare two smart pointers
    //
    template<typename _OtherIIID> bool operator!=(GITComPtr<_OtherIIID>& p) 
    {
        return !(operator==(p));
    }

    // Compare two pointers
    //
    template<typename _InterfaceType> bool operator!=(_InterfaceType* p) 
    {
        return !(operator==(p));
    }

    // For comparison to NULL
    //
    bool operator!=(int null) 
    {
        return !(operator==(null));
    }

    // Compare two smart pointers
    //
    template<typename _OtherIIID> bool operator<(const GITComPtr<_OtherIIID>& p) 
    {
        return _CompareUnknown(p) < 0;
    }

    // Compare two smart pointers
    //
    template<typename _OtherIIID> bool operator<(GITComPtr<_OtherIIID>& p) 
    {
        return _CompareUnknown(p) < 0;
    }

    // Compare two pointers
    //
    template<typename _InterfaceType> bool operator<(_InterfaceType* p) 
    {
        return _CompareUnknown(p) < 0;
    }

    // Compare two smart pointers
    //
    template<typename _OtherIIID> bool operator>(const GITComPtr<_OtherIIID>& p) 
    {
        return _CompareUnknown(p) > 0;
    }

    // Compare two smart pointers
    //
    template<typename _OtherIIID> bool operator>(GITComPtr<_OtherIIID>& p) 
    {
        return _CompareUnknown(p) > 0;
    }

    // Compare two pointers
    //
    template<typename _InterfaceType> bool operator>(_InterfaceType* p) 
    {
        return _CompareUnknown(p) > 0;
    }

    // Compare two smart pointers
    //
    template<typename _OtherIIID> bool operator<=(const GITComPtr<_OtherIIID>& p) 
    {
        return _CompareUnknown(p) <= 0;
    }

    // Compare two smart pointers
    //
    template<typename _OtherIIID> bool operator<=(GITComPtr<_OtherIIID>& p) 
    {
        return _CompareUnknown(p) <= 0;
    }

    // Compare two pointers
    //
    template<typename _InterfaceType> bool operator<=(_InterfaceType* p) 
    {
        return _CompareUnknown(p) <= 0;
    }

    // Compare two smart pointers
    //
    template<typename _OtherIIID> bool operator>=(const GITComPtr<_OtherIIID>& p) 
    {
        return _CompareUnknown(p) >= 0;
    }

    // Compare two smart pointers
    //
    template<typename _OtherIIID> bool operator>=(GITComPtr<_OtherIIID>& p) 
    {
        return _CompareUnknown(p) >= 0;
    }

    // Compare two pointers
    //
    template<typename _InterfaceType> bool operator>=(_InterfaceType* p) 
    {
        return _CompareUnknown(p) >= 0;
    }

    // Provides error-checking Release()ing of this interface.
    //
    void Release() 
    {
		Interface* pInterface = ExtractInterfacePointer();
        if (pInterface == NULL) {
            _com_issue_error(E_POINTER);
        }
        else {
            pInterface->Release();
            pInterface = NULL;
        }
    }

    // Provides error-checking AddRef()ing of this interface.
    //
    void AddRef() 
    { 
		Interface* pInterface = ExtractInterfacePointer();
        if (pInterface == NULL) {
            _com_issue_error(E_POINTER);
        }
        else {
            pInterface->AddRef();
        }
    }

    // Another way to get the interface pointer without casting.
    //
    Interface* GetInterfacePtr() const throw()
    { 
        return ExtractInterfacePointer(); 
    }

    // Another way to get the interface pointer without casting.
    // Use for [in, out] parameter passing
    Interface*& GetInterfacePtr() throw()
    { 
        return ExtractInterfacePointer(); 
    }

    // Loads an interface for the provided CLSID.
    // Returns an HRESULT.  Any previous interface is unconditionally released.
    //
    HRESULT CreateInstance(const CLSID& rclsid, IUnknown* pOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) throw()
    {
        HRESULT hr = S_OK;
		
		RemoveInterfacePointer();
		
		Interface* pInterface = 0;
		
		hr = CoCreateInstance(rclsid, pOuter, dwClsContext, GetIID(), reinterpret_cast<void**>(&pInterface));
		if (FAILED(hr)) return hr;
		
		InsertInterfacePointer(pInterface);
		
		pInterface->Release();
		
		return hr;
    }
	
    // Creates the class specified by clsidString.  clsidString may
    // contain a class id, or a prog id string.
    //
    HRESULT CreateInstance(LPCWSTR clsidString, IUnknown* pOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) throw()
    {
        if (clsidString == NULL) {
            return E_INVALIDARG;
        }
		
        CLSID clsid;
        HRESULT hr;
		
        if (clsidString[0] == L'{') {
            hr = CLSIDFromString(const_cast<LPWSTR> (clsidString), &clsid);
        }
        else {
            hr = CLSIDFromProgID(const_cast<LPWSTR> (clsidString), &clsid);
        }
		
        if (FAILED(hr)) {
            return hr;
        }
		
        return CreateInstance(clsid, pOuter, dwClsContext);
    }

    // Creates the class specified by SBCS clsidString.  clsidString may
    // contain a class id, or a prog id string.
    //
    HRESULT CreateInstance(LPCSTR clsidStringA, IUnknown* pOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) throw()
    {
        if (clsidStringA == NULL) {
            return E_INVALIDARG;
        }
		
        int size = lstrlenA(clsidStringA) + 1;
        int destSize = MultiByteToWideChar(CP_ACP, 0, clsidStringA, size, NULL, 0);

        if (destSize == 0) {
            return HRESULT_FROM_WIN32(GetLastError());
        }

        LPWSTR clsidStringW;
        __try {
            clsidStringW = static_cast<LPWSTR>(_alloca(destSize * sizeof(WCHAR)));
        }
        __except (1) {
            clsidStringW = NULL;
        }

        if (clsidStringW == NULL) {
            return E_OUTOFMEMORY;
        }

        if (MultiByteToWideChar(CP_ACP, 0, clsidStringA, size, clsidStringW, destSize) == 0) {
            return HRESULT_FROM_WIN32(GetLastError());
        }
		
        return CreateInstance(clsidStringW, pOuter, dwClsContext);
    }
	
    template<typename _OtherIIID> HRESULT QueryInterface(const IID& iid, GITComPtr<_OtherIIID>& p)
	{
		HRESULT hr = S_OK;
		
		Interface* pInterface = ExtractInterfacePointer();
		GITComPtr<_OtherIIID>::Interface* pQI = 0;
		
		if (pInterface != 0)
		{
			hr = pInterface->QueryInterface(iid, reinterpret_cast<void**>(&pQI));
			if (FAILED(hr)) return hr;
			p = pQI;
			pQI->Release();
		}
		else
		{
			return E_POINTER;
		}
		
		return hr;
	}
	
    template<typename _OtherIIID> HRESULT QueryInterface(const IID& iid, GITComPtr<_OtherIIID>* p)
	{
		return QueryInterface(iid, *p);
	}
	
	// Performs the QI for the specified IID and returns it in p.
    // As with all QIs, the interface will be AddRef'd.
    //
    template<typename _InterfaceType> HRESULT QueryInterface(const IID& iid, _InterfaceType*& p) throw ()
    {
		Interface* pInterface = ExtractInterfacePointer();
        if (pInterface != NULL) {
            return pInterface->QueryInterface(iid, reinterpret_cast<void**>(&p));
        }

        return E_POINTER;
    }
	
    // Performs the QI for the specified IID and returns it in p.
    // As with all QIs, the interface will be AddRef'd.
    //
    template<typename _InterfaceType> HRESULT QueryInterface(const IID& iid, _InterfaceType** p) throw()
    {
        return QueryInterface(iid, *p);
    }
	
protected:
	
	DWORD m_dwCookie;
	
	// Adds the interface pointer to the GIT 
	void InsertInterfacePointer(Interface *pInterface)
	{
		if (m_dwCookie!=0)
		{
			RemoveInterfacePointer();
		}
		
		if (pInterface != 0)
		{
			IGlobalInterfaceTablePtr git(CLSID_StdGlobalInterfaceTable);
			HRESULT hr = git->RegisterInterfaceInGlobal(pInterface, GetIID(), &m_dwCookie);
			if (FAILED(hr)) _com_issue_error(hr);

			//xxx git takes ownership of interface* in RegisterInterfaceInGlobal() 
			//    must be calling addref() 
		}
	}
	
	// extracts the interface pointer from the GIT
	Interface* ExtractInterfacePointer() const
	{
		if (m_dwCookie!=0)
		{
			IGlobalInterfaceTablePtr git(CLSID_StdGlobalInterfaceTable);
			
			Interface* pInterface = 0;
			HRESULT hr = git->GetInterfaceFromGlobal(m_dwCookie, GetIID(), reinterpret_cast<void**>(&pInterface));
			
			if (FAILED(hr)) _com_issue_error(E_POINTER);
			
			// GetInterfaceFromGlobal calls addref, but this 
			// screws up the refcounting implemented in this class
			pInterface->Release();
			
			return pInterface;
		}
		return 0;
	}
	
	// removes the interface pointer from the GIT
	void RemoveInterfacePointer()
	{
		if (m_dwCookie!=0)
		{
			IGlobalInterfaceTablePtr git(CLSID_StdGlobalInterfaceTable);
			git->RevokeInterfaceFromGlobal(m_dwCookie);

			//xxx git releases ownership of interface* in RevokeInterfaceFromGlobal() 
			//    must be calling release() 
		}
		m_dwCookie = 0;
	}
	
    // Releases only if the interface is not null.
    // The interface is not set to NULL.
    //
    void _Release() throw()
    {
		Interface* pInterface = ExtractInterfacePointer();
        if (pInterface != NULL) {
            pInterface->Release();
        }
    }

    // AddRefs only if the interface is not NULL
    //
    void _AddRef() throw()
    {
		Interface* pInterface = ExtractInterfacePointer();
        if (pInterface != NULL) {
            pInterface->AddRef();
        }
    }

    // Performs a QI on pUnknown for the interface type returned
    // for this class.  The interface is stored.  If pUnknown is
    // NULL, or the QI fails, E_NOINTERFACE is returned and
    // _pInterface is set to NULL.
    //
    template<typename _InterfacePtr> HRESULT _QueryInterface(_InterfacePtr p) throw()
    {
        HRESULT hr;

        // Can't QI NULL
        //
        if (p != NULL) {
            // Query for this interface
            //
            Interface* pInterface;
            hr = p->QueryInterface(GetIID(), reinterpret_cast<void**>(&pInterface));
			
            // Save the interface without AddRef()ing.
            //
            Attach(SUCCEEDED(hr)? pInterface : NULL);
        }
        else {
            operator=(static_cast<Interface*>(NULL));
            hr = E_NOINTERFACE;
        }

        return hr;
    }

    // Compares the provided pointer with this by obtaining IUnknown interfaces
    // for each pointer and then returning the difference.
    //
    template<typename _InterfacePtr> int _CompareUnknown(_InterfacePtr p) 
    {
		IUnknown* pu1, *pu2;
		
		Interface* pInterface = ExtractInterfacePointer();
		if (pInterface != NULL) {
			HRESULT hr = pInterface->QueryInterface(__uuidof(IUnknown), reinterpret_cast<void**>(&pu1));
			
			if (FAILED(hr)) {
                _com_issue_error(hr);
                pu1 = NULL;
            }
            else {
                pu1->Release();
            }
        }
        else {
            pu1 = NULL;
        }

        if (p != NULL) {
            HRESULT hr = p->QueryInterface(__uuidof(IUnknown), reinterpret_cast<void**>(&pu2));

            if (FAILED(hr)) {
                _com_issue_error(hr);
                pu2 = NULL;
            }
            else {
                pu2->Release();
            }
        }
        else {
            pu2 = NULL;
        }
		
		return ((pu1 > pu2) ? 1 : ((pu1 < pu2) ? -1 : 0));
    }

    // Try to extract either IDispatch* or an IUnknown* from
    // the VARIANT
    //
    HRESULT QueryStdInterfaces(const _variant_t& varSrc) throw()
    {
        if (V_VT(&varSrc) == VT_DISPATCH) {
            return _QueryInterface(V_DISPATCH(&varSrc));
        }

        if (V_VT(&varSrc) == VT_UNKNOWN) {
            return _QueryInterface(V_UNKNOWN(&varSrc));
        }

        // We have something other than an IUnknown or an IDispatch.
        // Can we convert it to either one of these?
        // Try IDispatch first
        //
        VARIANT varDest;
        VariantInit(&varDest);

        HRESULT hr = VariantChangeType(&varDest, const_cast<VARIANT*>(static_cast<const VARIANT*>(&varSrc)), 0, VT_DISPATCH);
        if (SUCCEEDED(hr)) {
            hr = _QueryInterface(V_DISPATCH(&varSrc));
        }

        if (hr == E_NOINTERFACE) {
            // That failed ... so try IUnknown
            //
            VariantInit(&varDest);
            hr = VariantChangeType(&varDest, const_cast<VARIANT*>(static_cast<const VARIANT*>(&varSrc)), 0, VT_UNKNOWN);
            if (SUCCEEDED(hr)) {
                hr = _QueryInterface(V_UNKNOWN(&varSrc));
            }
        }

        VariantClear(&varDest);
        return hr;
    }
};

/*
// Reverse comparison operators for GITComPtr
//
template<typename _InterfaceType> bool operator==(int null, GITComPtr<_InterfaceType>& p) 
{
    if (null != 0) {
        _com_issue_error(E_POINTER);
    }

    return p == NULL;
}

template<typename _Interface, typename _InterfacePtr> bool operator==(_Interface* i, GITComPtr<_InterfacePtr>& p) 
{
    return p == i;
}

template<typename _Interface> bool operator!=(int null, GITComPtr<_Interface>& p) 
{
    if (null != 0) {
        _com_issue_error(E_POINTER);
    }

    return p != NULL;
}

template<typename _Interface, typename _InterfacePtr> bool operator!=(_Interface* i, GITComPtr<_InterfacePtr>& p) 
{
    return p != i;
}

template<typename _Interface> bool operator<(int null, GITComPtr<_Interface>& p) 
{
    if (null != 0) {
        _com_issue_error(E_POINTER);
    }

    return p > NULL;
}

template<typename _Interface, typename _InterfacePtr> bool operator<(_Interface* i, GITComPtr<_InterfacePtr>& p) 
{
    return p > i;
}

template<typename _Interface> bool operator>(int null, GITComPtr<_Interface>& p) 
{
    if (null != 0) {
        _com_issue_error(E_POINTER);
    }

    return p < NULL;
}

template<typename _Interface, typename _InterfacePtr> bool operator>(_Interface* i, GITComPtr<_InterfacePtr>& p) 
{
    return p < i;
}

template<typename _Interface> bool operator<=(int null, GITComPtr<_Interface>& p) 
{
    if (null != 0) {
        _com_issue_error(E_POINTER);
    }

    return p >= NULL;
}

template<typename _Interface, typename _InterfacePtr> bool operator<=(_Interface* i, GITComPtr<_InterfacePtr>& p) 
{
    return p >= i;
}

template<typename _Interface> bool operator>=(int null, GITComPtr<_Interface>& p) 
{
    if (null != 0) {
        _com_issue_error(E_POINTER);
    }

    return p <= NULL;
}

template<typename _Interface, typename _InterfacePtr> bool operator>=(_Interface* i, GITComPtr<_InterfacePtr>& p) 
{
    return p <= i;
}

*/

//#pragma pop_macro("new")
//#pragma warning(pop)

#endif // __GIT_Com_Ptr_EB27BF47_8DC1_452c_B007_CD2636610ACC__



