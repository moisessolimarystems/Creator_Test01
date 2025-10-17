
/* This file was hand generated using the auto generated files _ISolimarLicenseSvr.h and _ISolimarLicenseSvr_i.c */

/* Compiler settings for _SolimarLicenseServer.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef ___IObjectAuthentication__5BA7923C_90A1_4edd_A6EB_003980EFC1B1_h__
#define ___IObjectAuthentication__5BA7923C_90A1_4edd_A6EB_003980EFC1B1_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IObjectAuthentication_FWD_DEFINED__
#define __IObjectAuthentication_FWD_DEFINED__
typedef interface IObjectAuthentication IObjectAuthentication;
#endif 	/* __IObjectAuthentication_FWD_DEFINED__ */


/* header files for imported files */
#include "docobj.h"
#include "prsht.h"
#include "mshtml.h"
#include "mshtmhst.h"
#include "exdisp.h"
#include "objsafe.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __IObjectAuthentication_INTERFACE_DEFINED__
#define __IObjectAuthentication_INTERFACE_DEFINED__

/* interface IObjectAuthentication */
/* [unique][helpstring][dual][uuid][object] */ 

EXTERN_C const IID IID_IObjectAuthentication;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("779FE092-038A-43c2-8F99-5F4974D14997")
    IObjectAuthentication : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Challenge( 
            /* [in] */ VARIANT vtChallenge,
            /* [retval][out] */ VARIANT *pvtResponse) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetChallenge( 
            /* [retval][out] */ VARIANT *pvtChallenge) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PutResponse( 
            /* [in] */ VARIANT vtResponse) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IObjectAuthenticationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IObjectAuthentication * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IObjectAuthentication * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IObjectAuthentication * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IObjectAuthentication * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IObjectAuthentication * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IObjectAuthentication * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IObjectAuthentication * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Challenge )( 
            IObjectAuthentication * This,
            /* [in] */ VARIANT vtChallenge,
            /* [retval][out] */ VARIANT *pvtResponse);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetChallenge )( 
            IObjectAuthentication * This,
            /* [retval][out] */ VARIANT *pvtChallenge);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PutResponse )( 
            IObjectAuthentication * This,
            /* [in] */ VARIANT vtResponse);
        
        END_INTERFACE
    } IObjectAuthenticationVtbl;

    interface IObjectAuthentication
    {
        CONST_VTBL struct IObjectAuthenticationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IObjectAuthentication_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IObjectAuthentication_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IObjectAuthentication_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IObjectAuthentication_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IObjectAuthentication_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IObjectAuthentication_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IObjectAuthentication_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IObjectAuthentication_Challenge(This,vtChallenge,pvtResponse)	\
    (This)->lpVtbl -> Challenge(This,vtChallenge,pvtResponse)

#define IObjectAuthentication_GetChallenge(This,pvtChallenge)	\
    (This)->lpVtbl -> GetChallenge(This,pvtChallenge)

#define IObjectAuthentication_PutResponse(This,vtResponse)	\
    (This)->lpVtbl -> PutResponse(This,vtResponse)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IObjectAuthentication_Challenge_Proxy( 
    IObjectAuthentication * This,
    /* [in] */ VARIANT vtChallenge,
    /* [retval][out] */ VARIANT *pvtResponse);


void __RPC_STUB IObjectAuthentication_Challenge_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IObjectAuthentication_GetChallenge_Proxy( 
    IObjectAuthentication * This,
    /* [retval][out] */ VARIANT *pvtChallenge);


void __RPC_STUB IObjectAuthentication_GetChallenge_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IObjectAuthentication_PutResponse_Proxy( 
    IObjectAuthentication * This,
    /* [in] */ VARIANT vtResponse);


void __RPC_STUB IObjectAuthentication_PutResponse_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IObjectAuthentication_INTERFACE_DEFINED__ */


	/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


