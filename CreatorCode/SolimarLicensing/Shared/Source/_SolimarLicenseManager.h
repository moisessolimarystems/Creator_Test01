

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Nov 17 16:25:04 2017
 */
/* Compiler settings for _SolimarLicenseManager.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

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

#ifndef ___SolimarLicenseManager_h__
#define ___SolimarLicenseManager_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IObjectAuthentication_FWD_DEFINED__
#define __IObjectAuthentication_FWD_DEFINED__
typedef interface IObjectAuthentication IObjectAuthentication;
#endif 	/* __IObjectAuthentication_FWD_DEFINED__ */


#ifndef __ILicensingMessage_FWD_DEFINED__
#define __ILicensingMessage_FWD_DEFINED__
typedef interface ILicensingMessage ILicensingMessage;
#endif 	/* __ILicensingMessage_FWD_DEFINED__ */


#ifndef __ISolimarLicenseMgr_FWD_DEFINED__
#define __ISolimarLicenseMgr_FWD_DEFINED__
typedef interface ISolimarLicenseMgr ISolimarLicenseMgr;
#endif 	/* __ISolimarLicenseMgr_FWD_DEFINED__ */


#ifndef __ISolimarLicenseMgr2_FWD_DEFINED__
#define __ISolimarLicenseMgr2_FWD_DEFINED__
typedef interface ISolimarLicenseMgr2 ISolimarLicenseMgr2;
#endif 	/* __ISolimarLicenseMgr2_FWD_DEFINED__ */


#ifndef __ISolimarLicenseMgr3_FWD_DEFINED__
#define __ISolimarLicenseMgr3_FWD_DEFINED__
typedef interface ISolimarLicenseMgr3 ISolimarLicenseMgr3;
#endif 	/* __ISolimarLicenseMgr3_FWD_DEFINED__ */


#ifndef __ISolimarLicenseMgr4_FWD_DEFINED__
#define __ISolimarLicenseMgr4_FWD_DEFINED__
typedef interface ISolimarLicenseMgr4 ISolimarLicenseMgr4;
#endif 	/* __ISolimarLicenseMgr4_FWD_DEFINED__ */


#ifndef __ISolimarLicenseMgr5_FWD_DEFINED__
#define __ISolimarLicenseMgr5_FWD_DEFINED__
typedef interface ISolimarLicenseMgr5 ISolimarLicenseMgr5;
#endif 	/* __ISolimarLicenseMgr5_FWD_DEFINED__ */


#ifndef __ISolimarLicenseMgr6_FWD_DEFINED__
#define __ISolimarLicenseMgr6_FWD_DEFINED__
typedef interface ISolimarLicenseMgr6 ISolimarLicenseMgr6;
#endif 	/* __ISolimarLicenseMgr6_FWD_DEFINED__ */


#ifndef __ISolimarLicenseMgr7_FWD_DEFINED__
#define __ISolimarLicenseMgr7_FWD_DEFINED__
typedef interface ISolimarLicenseMgr7 ISolimarLicenseMgr7;
#endif 	/* __ISolimarLicenseMgr7_FWD_DEFINED__ */


#ifndef __CSolimarLicenseMgr_FWD_DEFINED__
#define __CSolimarLicenseMgr_FWD_DEFINED__

#ifdef __cplusplus
typedef class CSolimarLicenseMgr CSolimarLicenseMgr;
#else
typedef struct CSolimarLicenseMgr CSolimarLicenseMgr;
#endif /* __cplusplus */

#endif 	/* __CSolimarLicenseMgr_FWD_DEFINED__ */


/* header files for imported files */
#include "prsht.h"
#include "mshtml.h"
#include "mshtmhst.h"
#include "exdisp.h"
#include "objsafe.h"
#include "shldisp.h"
//#include "shobjidl.h" //Comment out this file, causes problem with the SDK Visual Studio 6.0 uses - JWL.

#ifdef __cplusplus
extern "C"{
#endif 


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
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
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
            /* [range][in] */ UINT cNames,
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
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IObjectAuthentication_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IObjectAuthentication_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IObjectAuthentication_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IObjectAuthentication_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IObjectAuthentication_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IObjectAuthentication_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IObjectAuthentication_Challenge(This,vtChallenge,pvtResponse)	\
    ( (This)->lpVtbl -> Challenge(This,vtChallenge,pvtResponse) ) 

#define IObjectAuthentication_GetChallenge(This,pvtChallenge)	\
    ( (This)->lpVtbl -> GetChallenge(This,pvtChallenge) ) 

#define IObjectAuthentication_PutResponse(This,vtResponse)	\
    ( (This)->lpVtbl -> PutResponse(This,vtResponse) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IObjectAuthentication_INTERFACE_DEFINED__ */


#ifndef __ILicensingMessage_INTERFACE_DEFINED__
#define __ILicensingMessage_INTERFACE_DEFINED__

/* interface ILicensingMessage */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ILicensingMessage;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("025336D2-0EF1-4dab-9126-E8ACD750FFE5")
    ILicensingMessage : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetLicenseMessageList( 
            /* [in] */ VARIANT_BOOL clear_messages,
            /* [retval][out] */ VARIANT *pvtMessageList) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DispatchLicenseMessageList( 
            /* [in] */ VARIANT_BOOL clear_messages) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILicensingMessageVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILicensingMessage * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILicensingMessage * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILicensingMessage * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ILicensingMessage * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ILicensingMessage * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ILicensingMessage * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ILicensingMessage * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetLicenseMessageList )( 
            ILicensingMessage * This,
            /* [in] */ VARIANT_BOOL clear_messages,
            /* [retval][out] */ VARIANT *pvtMessageList);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DispatchLicenseMessageList )( 
            ILicensingMessage * This,
            /* [in] */ VARIANT_BOOL clear_messages);
        
        END_INTERFACE
    } ILicensingMessageVtbl;

    interface ILicensingMessage
    {
        CONST_VTBL struct ILicensingMessageVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILicensingMessage_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILicensingMessage_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILicensingMessage_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILicensingMessage_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ILicensingMessage_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ILicensingMessage_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ILicensingMessage_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ILicensingMessage_GetLicenseMessageList(This,clear_messages,pvtMessageList)	\
    ( (This)->lpVtbl -> GetLicenseMessageList(This,clear_messages,pvtMessageList) ) 

#define ILicensingMessage_DispatchLicenseMessageList(This,clear_messages)	\
    ( (This)->lpVtbl -> DispatchLicenseMessageList(This,clear_messages) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILicensingMessage_INTERFACE_DEFINED__ */


#ifndef __ISolimarLicenseMgr_INTERFACE_DEFINED__
#define __ISolimarLicenseMgr_INTERFACE_DEFINED__

/* interface ISolimarLicenseMgr */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISolimarLicenseMgr;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E76FF720-125E-4FE3-AAF4-05E8CBF6040A")
    ISolimarLicenseMgr : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Connect( 
            /* [in] */ BSTR server) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Disconnect( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ValidateLicense( 
            /* [retval][out] */ VARIANT_BOOL *license_valid) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ModuleLicenseTotal( 
            /* [in] */ long module_id,
            /* [retval][out] */ long *count) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ModuleLicenseInUse( 
            /* [in] */ long module_id,
            /* [retval][out] */ long *count) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ModuleLicenseObtain( 
            /* [in] */ long module_id,
            /* [in] */ long count) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ModuleLicenseRelease( 
            /* [in] */ long module_id,
            /* [in] */ long count) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISolimarLicenseMgrVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarLicenseMgr * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarLicenseMgr * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarLicenseMgr * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarLicenseMgr * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarLicenseMgr * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarLicenseMgr * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarLicenseMgr * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            ISolimarLicenseMgr * This,
            /* [in] */ BSTR server);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            ISolimarLicenseMgr * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            ISolimarLicenseMgr * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ValidateLicense )( 
            ISolimarLicenseMgr * This,
            /* [retval][out] */ VARIANT_BOOL *license_valid);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseTotal )( 
            ISolimarLicenseMgr * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseInUse )( 
            ISolimarLicenseMgr * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseObtain )( 
            ISolimarLicenseMgr * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseRelease )( 
            ISolimarLicenseMgr * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        END_INTERFACE
    } ISolimarLicenseMgrVtbl;

    interface ISolimarLicenseMgr
    {
        CONST_VTBL struct ISolimarLicenseMgrVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISolimarLicenseMgr_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISolimarLicenseMgr_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISolimarLicenseMgr_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISolimarLicenseMgr_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISolimarLicenseMgr_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISolimarLicenseMgr_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISolimarLicenseMgr_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISolimarLicenseMgr_Connect(This,server)	\
    ( (This)->lpVtbl -> Connect(This,server) ) 

#define ISolimarLicenseMgr_Disconnect(This)	\
    ( (This)->lpVtbl -> Disconnect(This) ) 

#define ISolimarLicenseMgr_Initialize(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level)	\
    ( (This)->lpVtbl -> Initialize(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level) ) 

#define ISolimarLicenseMgr_ValidateLicense(This,license_valid)	\
    ( (This)->lpVtbl -> ValidateLicense(This,license_valid) ) 

#define ISolimarLicenseMgr_ModuleLicenseTotal(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseTotal(This,module_id,count) ) 

#define ISolimarLicenseMgr_ModuleLicenseInUse(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseInUse(This,module_id,count) ) 

#define ISolimarLicenseMgr_ModuleLicenseObtain(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseObtain(This,module_id,count) ) 

#define ISolimarLicenseMgr_ModuleLicenseRelease(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseRelease(This,module_id,count) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISolimarLicenseMgr_INTERFACE_DEFINED__ */


#ifndef __ISolimarLicenseMgr2_INTERFACE_DEFINED__
#define __ISolimarLicenseMgr2_INTERFACE_DEFINED__

/* interface ISolimarLicenseMgr2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISolimarLicenseMgr2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EA2ADA7A-67BB-45f6-AF57-40C4E2CE8595")
    ISolimarLicenseMgr2 : public ISolimarLicenseMgr
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ModuleLicenseSerialNumbers( 
            /* [in] */ long module_id,
            /* [retval][out] */ VARIANT *pvtSerialNumberList) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISolimarLicenseMgr2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarLicenseMgr2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarLicenseMgr2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarLicenseMgr2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarLicenseMgr2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarLicenseMgr2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarLicenseMgr2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarLicenseMgr2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            ISolimarLicenseMgr2 * This,
            /* [in] */ BSTR server);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            ISolimarLicenseMgr2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            ISolimarLicenseMgr2 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ValidateLicense )( 
            ISolimarLicenseMgr2 * This,
            /* [retval][out] */ VARIANT_BOOL *license_valid);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseTotal )( 
            ISolimarLicenseMgr2 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseInUse )( 
            ISolimarLicenseMgr2 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseObtain )( 
            ISolimarLicenseMgr2 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseRelease )( 
            ISolimarLicenseMgr2 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseSerialNumbers )( 
            ISolimarLicenseMgr2 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ VARIANT *pvtSerialNumberList);
        
        END_INTERFACE
    } ISolimarLicenseMgr2Vtbl;

    interface ISolimarLicenseMgr2
    {
        CONST_VTBL struct ISolimarLicenseMgr2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISolimarLicenseMgr2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISolimarLicenseMgr2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISolimarLicenseMgr2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISolimarLicenseMgr2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISolimarLicenseMgr2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISolimarLicenseMgr2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISolimarLicenseMgr2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISolimarLicenseMgr2_Connect(This,server)	\
    ( (This)->lpVtbl -> Connect(This,server) ) 

#define ISolimarLicenseMgr2_Disconnect(This)	\
    ( (This)->lpVtbl -> Disconnect(This) ) 

#define ISolimarLicenseMgr2_Initialize(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level)	\
    ( (This)->lpVtbl -> Initialize(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level) ) 

#define ISolimarLicenseMgr2_ValidateLicense(This,license_valid)	\
    ( (This)->lpVtbl -> ValidateLicense(This,license_valid) ) 

#define ISolimarLicenseMgr2_ModuleLicenseTotal(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseTotal(This,module_id,count) ) 

#define ISolimarLicenseMgr2_ModuleLicenseInUse(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseInUse(This,module_id,count) ) 

#define ISolimarLicenseMgr2_ModuleLicenseObtain(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseObtain(This,module_id,count) ) 

#define ISolimarLicenseMgr2_ModuleLicenseRelease(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseRelease(This,module_id,count) ) 


#define ISolimarLicenseMgr2_ModuleLicenseSerialNumbers(This,module_id,pvtSerialNumberList)	\
    ( (This)->lpVtbl -> ModuleLicenseSerialNumbers(This,module_id,pvtSerialNumberList) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISolimarLicenseMgr2_INTERFACE_DEFINED__ */


#ifndef __ISolimarLicenseMgr3_INTERFACE_DEFINED__
#define __ISolimarLicenseMgr3_INTERFACE_DEFINED__

/* interface ISolimarLicenseMgr3 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISolimarLicenseMgr3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CF8B391B-286E-4d72-9087-C8720C32D25C")
    ISolimarLicenseMgr3 : public ISolimarLicenseMgr2
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ModuleLicenseCounterDecrement( 
            /* [in] */ long module_id,
            /* [in] */ long license_count) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISolimarLicenseMgr3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarLicenseMgr3 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarLicenseMgr3 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarLicenseMgr3 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ BSTR server);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            ISolimarLicenseMgr3 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ValidateLicense )( 
            ISolimarLicenseMgr3 * This,
            /* [retval][out] */ VARIANT_BOOL *license_valid);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseTotal )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseInUse )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseObtain )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseRelease )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseSerialNumbers )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ VARIANT *pvtSerialNumberList);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseCounterDecrement )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ long module_id,
            /* [in] */ long license_count);
        
        END_INTERFACE
    } ISolimarLicenseMgr3Vtbl;

    interface ISolimarLicenseMgr3
    {
        CONST_VTBL struct ISolimarLicenseMgr3Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISolimarLicenseMgr3_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISolimarLicenseMgr3_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISolimarLicenseMgr3_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISolimarLicenseMgr3_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISolimarLicenseMgr3_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISolimarLicenseMgr3_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISolimarLicenseMgr3_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISolimarLicenseMgr3_Connect(This,server)	\
    ( (This)->lpVtbl -> Connect(This,server) ) 

#define ISolimarLicenseMgr3_Disconnect(This)	\
    ( (This)->lpVtbl -> Disconnect(This) ) 

#define ISolimarLicenseMgr3_Initialize(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level)	\
    ( (This)->lpVtbl -> Initialize(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level) ) 

#define ISolimarLicenseMgr3_ValidateLicense(This,license_valid)	\
    ( (This)->lpVtbl -> ValidateLicense(This,license_valid) ) 

#define ISolimarLicenseMgr3_ModuleLicenseTotal(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseTotal(This,module_id,count) ) 

#define ISolimarLicenseMgr3_ModuleLicenseInUse(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseInUse(This,module_id,count) ) 

#define ISolimarLicenseMgr3_ModuleLicenseObtain(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseObtain(This,module_id,count) ) 

#define ISolimarLicenseMgr3_ModuleLicenseRelease(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseRelease(This,module_id,count) ) 


#define ISolimarLicenseMgr3_ModuleLicenseSerialNumbers(This,module_id,pvtSerialNumberList)	\
    ( (This)->lpVtbl -> ModuleLicenseSerialNumbers(This,module_id,pvtSerialNumberList) ) 


#define ISolimarLicenseMgr3_ModuleLicenseCounterDecrement(This,module_id,license_count)	\
    ( (This)->lpVtbl -> ModuleLicenseCounterDecrement(This,module_id,license_count) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISolimarLicenseMgr3_INTERFACE_DEFINED__ */


#ifndef __ISolimarLicenseMgr4_INTERFACE_DEFINED__
#define __ISolimarLicenseMgr4_INTERFACE_DEFINED__

/* interface ISolimarLicenseMgr4 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISolimarLicenseMgr4;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3DD3F19F-B523-4c1b-9AC3-0840C90B78E3")
    ISolimarLicenseMgr4 : public ISolimarLicenseMgr3
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Initialize2( 
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level,
            /* [in] */ unsigned long grace_period_minutes) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISolimarLicenseMgr4Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarLicenseMgr4 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarLicenseMgr4 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarLicenseMgr4 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ BSTR server);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            ISolimarLicenseMgr4 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ValidateLicense )( 
            ISolimarLicenseMgr4 * This,
            /* [retval][out] */ VARIANT_BOOL *license_valid);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseTotal )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseInUse )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseObtain )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseRelease )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseSerialNumbers )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ VARIANT *pvtSerialNumberList);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseCounterDecrement )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ long module_id,
            /* [in] */ long license_count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize2 )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level,
            /* [in] */ unsigned long grace_period_minutes);
        
        END_INTERFACE
    } ISolimarLicenseMgr4Vtbl;

    interface ISolimarLicenseMgr4
    {
        CONST_VTBL struct ISolimarLicenseMgr4Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISolimarLicenseMgr4_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISolimarLicenseMgr4_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISolimarLicenseMgr4_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISolimarLicenseMgr4_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISolimarLicenseMgr4_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISolimarLicenseMgr4_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISolimarLicenseMgr4_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISolimarLicenseMgr4_Connect(This,server)	\
    ( (This)->lpVtbl -> Connect(This,server) ) 

#define ISolimarLicenseMgr4_Disconnect(This)	\
    ( (This)->lpVtbl -> Disconnect(This) ) 

#define ISolimarLicenseMgr4_Initialize(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level)	\
    ( (This)->lpVtbl -> Initialize(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level) ) 

#define ISolimarLicenseMgr4_ValidateLicense(This,license_valid)	\
    ( (This)->lpVtbl -> ValidateLicense(This,license_valid) ) 

#define ISolimarLicenseMgr4_ModuleLicenseTotal(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseTotal(This,module_id,count) ) 

#define ISolimarLicenseMgr4_ModuleLicenseInUse(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseInUse(This,module_id,count) ) 

#define ISolimarLicenseMgr4_ModuleLicenseObtain(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseObtain(This,module_id,count) ) 

#define ISolimarLicenseMgr4_ModuleLicenseRelease(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseRelease(This,module_id,count) ) 


#define ISolimarLicenseMgr4_ModuleLicenseSerialNumbers(This,module_id,pvtSerialNumberList)	\
    ( (This)->lpVtbl -> ModuleLicenseSerialNumbers(This,module_id,pvtSerialNumberList) ) 


#define ISolimarLicenseMgr4_ModuleLicenseCounterDecrement(This,module_id,license_count)	\
    ( (This)->lpVtbl -> ModuleLicenseCounterDecrement(This,module_id,license_count) ) 


#define ISolimarLicenseMgr4_Initialize2(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level,grace_period_minutes)	\
    ( (This)->lpVtbl -> Initialize2(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level,grace_period_minutes) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISolimarLicenseMgr4_INTERFACE_DEFINED__ */


#ifndef __ISolimarLicenseMgr5_INTERFACE_DEFINED__
#define __ISolimarLicenseMgr5_INTERFACE_DEFINED__

/* interface ISolimarLicenseMgr5 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISolimarLicenseMgr5;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9A0AF885-9C33-4f42-BFAA-648400429209")
    ISolimarLicenseMgr5 : public ISolimarLicenseMgr4
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Connect2( 
            /* [in] */ BSTR server,
            /* [in] */ VARIANT_BOOL use_only_shared_licenses,
            /* [in] */ VARIANT_BOOL use_as_backup) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Initialize3( 
            /* [in] */ BSTR application_instance,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level,
            /* [in] */ unsigned long grace_period_minutes,
            /* [in] */ VARIANT_BOOL app_instance_lock_key,
            /* [in] */ VARIANT_BOOL b_bypass_remote_key_restriction) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetVersionLicenseManager( 
            /* [out] */ long *p_ver_major,
            /* [out] */ long *p_ver_minor,
            /* [out] */ long *p_ver_build) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetVersionLicenseServer( 
            /* [in] */ BSTR server,
            /* [out] */ long *p_ver_major,
            /* [out] */ long *p_ver_minor,
            /* [out] */ long *p_ver_build) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ConnectByProduct( 
            /* [in] */ long product,
            /* [in] */ VARIANT_BOOL use_shared_licenses_servers) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyProductExists( 
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [out] */ VARIANT_BOOL *p_bool_key_product_exists) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISolimarLicenseMgr5Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarLicenseMgr5 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarLicenseMgr5 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarLicenseMgr5 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ BSTR server);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            ISolimarLicenseMgr5 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ValidateLicense )( 
            ISolimarLicenseMgr5 * This,
            /* [retval][out] */ VARIANT_BOOL *license_valid);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseTotal )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseInUse )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseObtain )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseRelease )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseSerialNumbers )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ VARIANT *pvtSerialNumberList);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseCounterDecrement )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ long module_id,
            /* [in] */ long license_count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize2 )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level,
            /* [in] */ unsigned long grace_period_minutes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect2 )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ BSTR server,
            /* [in] */ VARIANT_BOOL use_only_shared_licenses,
            /* [in] */ VARIANT_BOOL use_as_backup);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize3 )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ BSTR application_instance,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level,
            /* [in] */ unsigned long grace_period_minutes,
            /* [in] */ VARIANT_BOOL app_instance_lock_key,
            /* [in] */ VARIANT_BOOL b_bypass_remote_key_restriction);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVersionLicenseManager )( 
            ISolimarLicenseMgr5 * This,
            /* [out] */ long *p_ver_major,
            /* [out] */ long *p_ver_minor,
            /* [out] */ long *p_ver_build);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVersionLicenseServer )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ BSTR server,
            /* [out] */ long *p_ver_major,
            /* [out] */ long *p_ver_minor,
            /* [out] */ long *p_ver_build);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ConnectByProduct )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ long product,
            /* [in] */ VARIANT_BOOL use_shared_licenses_servers);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyProductExists )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [out] */ VARIANT_BOOL *p_bool_key_product_exists);
        
        END_INTERFACE
    } ISolimarLicenseMgr5Vtbl;

    interface ISolimarLicenseMgr5
    {
        CONST_VTBL struct ISolimarLicenseMgr5Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISolimarLicenseMgr5_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISolimarLicenseMgr5_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISolimarLicenseMgr5_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISolimarLicenseMgr5_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISolimarLicenseMgr5_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISolimarLicenseMgr5_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISolimarLicenseMgr5_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISolimarLicenseMgr5_Connect(This,server)	\
    ( (This)->lpVtbl -> Connect(This,server) ) 

#define ISolimarLicenseMgr5_Disconnect(This)	\
    ( (This)->lpVtbl -> Disconnect(This) ) 

#define ISolimarLicenseMgr5_Initialize(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level)	\
    ( (This)->lpVtbl -> Initialize(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level) ) 

#define ISolimarLicenseMgr5_ValidateLicense(This,license_valid)	\
    ( (This)->lpVtbl -> ValidateLicense(This,license_valid) ) 

#define ISolimarLicenseMgr5_ModuleLicenseTotal(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseTotal(This,module_id,count) ) 

#define ISolimarLicenseMgr5_ModuleLicenseInUse(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseInUse(This,module_id,count) ) 

#define ISolimarLicenseMgr5_ModuleLicenseObtain(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseObtain(This,module_id,count) ) 

#define ISolimarLicenseMgr5_ModuleLicenseRelease(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseRelease(This,module_id,count) ) 


#define ISolimarLicenseMgr5_ModuleLicenseSerialNumbers(This,module_id,pvtSerialNumberList)	\
    ( (This)->lpVtbl -> ModuleLicenseSerialNumbers(This,module_id,pvtSerialNumberList) ) 


#define ISolimarLicenseMgr5_ModuleLicenseCounterDecrement(This,module_id,license_count)	\
    ( (This)->lpVtbl -> ModuleLicenseCounterDecrement(This,module_id,license_count) ) 


#define ISolimarLicenseMgr5_Initialize2(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level,grace_period_minutes)	\
    ( (This)->lpVtbl -> Initialize2(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level,grace_period_minutes) ) 


#define ISolimarLicenseMgr5_Connect2(This,server,use_only_shared_licenses,use_as_backup)	\
    ( (This)->lpVtbl -> Connect2(This,server,use_only_shared_licenses,use_as_backup) ) 

#define ISolimarLicenseMgr5_Initialize3(This,application_instance,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level,grace_period_minutes,app_instance_lock_key,b_bypass_remote_key_restriction)	\
    ( (This)->lpVtbl -> Initialize3(This,application_instance,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level,grace_period_minutes,app_instance_lock_key,b_bypass_remote_key_restriction) ) 

#define ISolimarLicenseMgr5_GetVersionLicenseManager(This,p_ver_major,p_ver_minor,p_ver_build)	\
    ( (This)->lpVtbl -> GetVersionLicenseManager(This,p_ver_major,p_ver_minor,p_ver_build) ) 

#define ISolimarLicenseMgr5_GetVersionLicenseServer(This,server,p_ver_major,p_ver_minor,p_ver_build)	\
    ( (This)->lpVtbl -> GetVersionLicenseServer(This,server,p_ver_major,p_ver_minor,p_ver_build) ) 

#define ISolimarLicenseMgr5_ConnectByProduct(This,product,use_shared_licenses_servers)	\
    ( (This)->lpVtbl -> ConnectByProduct(This,product,use_shared_licenses_servers) ) 

#define ISolimarLicenseMgr5_KeyProductExists(This,product,prod_ver_major,prod_ver_minor,p_bool_key_product_exists)	\
    ( (This)->lpVtbl -> KeyProductExists(This,product,prod_ver_major,prod_ver_minor,p_bool_key_product_exists) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISolimarLicenseMgr5_INTERFACE_DEFINED__ */


#ifndef __ISolimarLicenseMgr6_INTERFACE_DEFINED__
#define __ISolimarLicenseMgr6_INTERFACE_DEFINED__

/* interface ISolimarLicenseMgr6 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISolimarLicenseMgr6;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("39B8A524-0193-4bf3-BF83-8728413B0DA7")
    ISolimarLicenseMgr6 : public ISolimarLicenseMgr5
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ModuleLicenseInUse_ByApp( 
            /* [in] */ long module_id,
            /* [retval][out] */ long *count) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISolimarLicenseMgr6Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarLicenseMgr6 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarLicenseMgr6 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarLicenseMgr6 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ BSTR server);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            ISolimarLicenseMgr6 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ValidateLicense )( 
            ISolimarLicenseMgr6 * This,
            /* [retval][out] */ VARIANT_BOOL *license_valid);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseTotal )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseInUse )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseObtain )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseRelease )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseSerialNumbers )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ VARIANT *pvtSerialNumberList);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseCounterDecrement )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ long module_id,
            /* [in] */ long license_count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize2 )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level,
            /* [in] */ unsigned long grace_period_minutes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect2 )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ BSTR server,
            /* [in] */ VARIANT_BOOL use_only_shared_licenses,
            /* [in] */ VARIANT_BOOL use_as_backup);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize3 )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ BSTR application_instance,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level,
            /* [in] */ unsigned long grace_period_minutes,
            /* [in] */ VARIANT_BOOL app_instance_lock_key,
            /* [in] */ VARIANT_BOOL b_bypass_remote_key_restriction);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVersionLicenseManager )( 
            ISolimarLicenseMgr6 * This,
            /* [out] */ long *p_ver_major,
            /* [out] */ long *p_ver_minor,
            /* [out] */ long *p_ver_build);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVersionLicenseServer )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ BSTR server,
            /* [out] */ long *p_ver_major,
            /* [out] */ long *p_ver_minor,
            /* [out] */ long *p_ver_build);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ConnectByProduct )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ long product,
            /* [in] */ VARIANT_BOOL use_shared_licenses_servers);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyProductExists )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [out] */ VARIANT_BOOL *p_bool_key_product_exists);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseInUse_ByApp )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        END_INTERFACE
    } ISolimarLicenseMgr6Vtbl;

    interface ISolimarLicenseMgr6
    {
        CONST_VTBL struct ISolimarLicenseMgr6Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISolimarLicenseMgr6_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISolimarLicenseMgr6_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISolimarLicenseMgr6_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISolimarLicenseMgr6_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISolimarLicenseMgr6_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISolimarLicenseMgr6_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISolimarLicenseMgr6_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISolimarLicenseMgr6_Connect(This,server)	\
    ( (This)->lpVtbl -> Connect(This,server) ) 

#define ISolimarLicenseMgr6_Disconnect(This)	\
    ( (This)->lpVtbl -> Disconnect(This) ) 

#define ISolimarLicenseMgr6_Initialize(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level)	\
    ( (This)->lpVtbl -> Initialize(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level) ) 

#define ISolimarLicenseMgr6_ValidateLicense(This,license_valid)	\
    ( (This)->lpVtbl -> ValidateLicense(This,license_valid) ) 

#define ISolimarLicenseMgr6_ModuleLicenseTotal(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseTotal(This,module_id,count) ) 

#define ISolimarLicenseMgr6_ModuleLicenseInUse(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseInUse(This,module_id,count) ) 

#define ISolimarLicenseMgr6_ModuleLicenseObtain(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseObtain(This,module_id,count) ) 

#define ISolimarLicenseMgr6_ModuleLicenseRelease(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseRelease(This,module_id,count) ) 


#define ISolimarLicenseMgr6_ModuleLicenseSerialNumbers(This,module_id,pvtSerialNumberList)	\
    ( (This)->lpVtbl -> ModuleLicenseSerialNumbers(This,module_id,pvtSerialNumberList) ) 


#define ISolimarLicenseMgr6_ModuleLicenseCounterDecrement(This,module_id,license_count)	\
    ( (This)->lpVtbl -> ModuleLicenseCounterDecrement(This,module_id,license_count) ) 


#define ISolimarLicenseMgr6_Initialize2(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level,grace_period_minutes)	\
    ( (This)->lpVtbl -> Initialize2(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level,grace_period_minutes) ) 


#define ISolimarLicenseMgr6_Connect2(This,server,use_only_shared_licenses,use_as_backup)	\
    ( (This)->lpVtbl -> Connect2(This,server,use_only_shared_licenses,use_as_backup) ) 

#define ISolimarLicenseMgr6_Initialize3(This,application_instance,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level,grace_period_minutes,app_instance_lock_key,b_bypass_remote_key_restriction)	\
    ( (This)->lpVtbl -> Initialize3(This,application_instance,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level,grace_period_minutes,app_instance_lock_key,b_bypass_remote_key_restriction) ) 

#define ISolimarLicenseMgr6_GetVersionLicenseManager(This,p_ver_major,p_ver_minor,p_ver_build)	\
    ( (This)->lpVtbl -> GetVersionLicenseManager(This,p_ver_major,p_ver_minor,p_ver_build) ) 

#define ISolimarLicenseMgr6_GetVersionLicenseServer(This,server,p_ver_major,p_ver_minor,p_ver_build)	\
    ( (This)->lpVtbl -> GetVersionLicenseServer(This,server,p_ver_major,p_ver_minor,p_ver_build) ) 

#define ISolimarLicenseMgr6_ConnectByProduct(This,product,use_shared_licenses_servers)	\
    ( (This)->lpVtbl -> ConnectByProduct(This,product,use_shared_licenses_servers) ) 

#define ISolimarLicenseMgr6_KeyProductExists(This,product,prod_ver_major,prod_ver_minor,p_bool_key_product_exists)	\
    ( (This)->lpVtbl -> KeyProductExists(This,product,prod_ver_major,prod_ver_minor,p_bool_key_product_exists) ) 


#define ISolimarLicenseMgr6_ModuleLicenseInUse_ByApp(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseInUse_ByApp(This,module_id,count) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISolimarLicenseMgr6_INTERFACE_DEFINED__ */


#ifndef __ISolimarLicenseMgr7_INTERFACE_DEFINED__
#define __ISolimarLicenseMgr7_INTERFACE_DEFINED__

/* interface ISolimarLicenseMgr7 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISolimarLicenseMgr7;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("60D07071-CE52-462b-B457-C22A5D23F66B")
    ISolimarLicenseMgr7 : public ISolimarLicenseMgr6
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Connect3( 
            /* [in] */ BSTR server,
            /* [in] */ long connectionFlags) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SoftwareLicense_Initialize( 
            /* [in] */ BSTR application_instance,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ long auto_ui_level,
            /* [in] */ unsigned long grace_period_minutes) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SoftwareLicense_InitializeViewOnly( 
            /* [in] */ BSTR application_instance,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetInfoByProduct( 
            /* [in] */ long product,
            /* [in] */ VARIANT_BOOL use_shared_licenses_servers,
            /* [out] */ BSTR *p_server,
            /* [out] */ BSTR *p_backup_server,
            /* [out] */ VARIANT_BOOL *p_bool_use_test_dev_licensing) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISolimarLicenseMgr7Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarLicenseMgr7 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarLicenseMgr7 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarLicenseMgr7 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ BSTR server);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            ISolimarLicenseMgr7 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ValidateLicense )( 
            ISolimarLicenseMgr7 * This,
            /* [retval][out] */ VARIANT_BOOL *license_valid);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseTotal )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseInUse )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseObtain )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseRelease )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseSerialNumbers )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ VARIANT *pvtSerialNumberList);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseCounterDecrement )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long module_id,
            /* [in] */ long license_count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize2 )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level,
            /* [in] */ unsigned long grace_period_minutes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect2 )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ BSTR server,
            /* [in] */ VARIANT_BOOL use_only_shared_licenses,
            /* [in] */ VARIANT_BOOL use_as_backup);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize3 )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ BSTR application_instance,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level,
            /* [in] */ unsigned long grace_period_minutes,
            /* [in] */ VARIANT_BOOL app_instance_lock_key,
            /* [in] */ VARIANT_BOOL b_bypass_remote_key_restriction);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVersionLicenseManager )( 
            ISolimarLicenseMgr7 * This,
            /* [out] */ long *p_ver_major,
            /* [out] */ long *p_ver_minor,
            /* [out] */ long *p_ver_build);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVersionLicenseServer )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ BSTR server,
            /* [out] */ long *p_ver_major,
            /* [out] */ long *p_ver_minor,
            /* [out] */ long *p_ver_build);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ConnectByProduct )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long product,
            /* [in] */ VARIANT_BOOL use_shared_licenses_servers);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyProductExists )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [out] */ VARIANT_BOOL *p_bool_key_product_exists);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseInUse_ByApp )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect3 )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ BSTR server,
            /* [in] */ long connectionFlags);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareLicense_Initialize )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ BSTR application_instance,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ long auto_ui_level,
            /* [in] */ unsigned long grace_period_minutes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareLicense_InitializeViewOnly )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ BSTR application_instance,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetInfoByProduct )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long product,
            /* [in] */ VARIANT_BOOL use_shared_licenses_servers,
            /* [out] */ BSTR *p_server,
            /* [out] */ BSTR *p_backup_server,
            /* [out] */ VARIANT_BOOL *p_bool_use_test_dev_licensing);
        
        END_INTERFACE
    } ISolimarLicenseMgr7Vtbl;

    interface ISolimarLicenseMgr7
    {
        CONST_VTBL struct ISolimarLicenseMgr7Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISolimarLicenseMgr7_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISolimarLicenseMgr7_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISolimarLicenseMgr7_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISolimarLicenseMgr7_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISolimarLicenseMgr7_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISolimarLicenseMgr7_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISolimarLicenseMgr7_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISolimarLicenseMgr7_Connect(This,server)	\
    ( (This)->lpVtbl -> Connect(This,server) ) 

#define ISolimarLicenseMgr7_Disconnect(This)	\
    ( (This)->lpVtbl -> Disconnect(This) ) 

#define ISolimarLicenseMgr7_Initialize(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level)	\
    ( (This)->lpVtbl -> Initialize(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level) ) 

#define ISolimarLicenseMgr7_ValidateLicense(This,license_valid)	\
    ( (This)->lpVtbl -> ValidateLicense(This,license_valid) ) 

#define ISolimarLicenseMgr7_ModuleLicenseTotal(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseTotal(This,module_id,count) ) 

#define ISolimarLicenseMgr7_ModuleLicenseInUse(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseInUse(This,module_id,count) ) 

#define ISolimarLicenseMgr7_ModuleLicenseObtain(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseObtain(This,module_id,count) ) 

#define ISolimarLicenseMgr7_ModuleLicenseRelease(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseRelease(This,module_id,count) ) 


#define ISolimarLicenseMgr7_ModuleLicenseSerialNumbers(This,module_id,pvtSerialNumberList)	\
    ( (This)->lpVtbl -> ModuleLicenseSerialNumbers(This,module_id,pvtSerialNumberList) ) 


#define ISolimarLicenseMgr7_ModuleLicenseCounterDecrement(This,module_id,license_count)	\
    ( (This)->lpVtbl -> ModuleLicenseCounterDecrement(This,module_id,license_count) ) 


#define ISolimarLicenseMgr7_Initialize2(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level,grace_period_minutes)	\
    ( (This)->lpVtbl -> Initialize2(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level,grace_period_minutes) ) 


#define ISolimarLicenseMgr7_Connect2(This,server,use_only_shared_licenses,use_as_backup)	\
    ( (This)->lpVtbl -> Connect2(This,server,use_only_shared_licenses,use_as_backup) ) 

#define ISolimarLicenseMgr7_Initialize3(This,application_instance,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level,grace_period_minutes,app_instance_lock_key,b_bypass_remote_key_restriction)	\
    ( (This)->lpVtbl -> Initialize3(This,application_instance,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level,grace_period_minutes,app_instance_lock_key,b_bypass_remote_key_restriction) ) 

#define ISolimarLicenseMgr7_GetVersionLicenseManager(This,p_ver_major,p_ver_minor,p_ver_build)	\
    ( (This)->lpVtbl -> GetVersionLicenseManager(This,p_ver_major,p_ver_minor,p_ver_build) ) 

#define ISolimarLicenseMgr7_GetVersionLicenseServer(This,server,p_ver_major,p_ver_minor,p_ver_build)	\
    ( (This)->lpVtbl -> GetVersionLicenseServer(This,server,p_ver_major,p_ver_minor,p_ver_build) ) 

#define ISolimarLicenseMgr7_ConnectByProduct(This,product,use_shared_licenses_servers)	\
    ( (This)->lpVtbl -> ConnectByProduct(This,product,use_shared_licenses_servers) ) 

#define ISolimarLicenseMgr7_KeyProductExists(This,product,prod_ver_major,prod_ver_minor,p_bool_key_product_exists)	\
    ( (This)->lpVtbl -> KeyProductExists(This,product,prod_ver_major,prod_ver_minor,p_bool_key_product_exists) ) 


#define ISolimarLicenseMgr7_ModuleLicenseInUse_ByApp(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseInUse_ByApp(This,module_id,count) ) 


#define ISolimarLicenseMgr7_Connect3(This,server,connectionFlags)	\
    ( (This)->lpVtbl -> Connect3(This,server,connectionFlags) ) 

#define ISolimarLicenseMgr7_SoftwareLicense_Initialize(This,application_instance,product,prod_ver_major,prod_ver_minor,auto_ui_level,grace_period_minutes)	\
    ( (This)->lpVtbl -> SoftwareLicense_Initialize(This,application_instance,product,prod_ver_major,prod_ver_minor,auto_ui_level,grace_period_minutes) ) 

#define ISolimarLicenseMgr7_SoftwareLicense_InitializeViewOnly(This,application_instance,product,prod_ver_major,prod_ver_minor)	\
    ( (This)->lpVtbl -> SoftwareLicense_InitializeViewOnly(This,application_instance,product,prod_ver_major,prod_ver_minor) ) 

#define ISolimarLicenseMgr7_GetInfoByProduct(This,product,use_shared_licenses_servers,p_server,p_backup_server,p_bool_use_test_dev_licensing)	\
    ( (This)->lpVtbl -> GetInfoByProduct(This,product,use_shared_licenses_servers,p_server,p_backup_server,p_bool_use_test_dev_licensing) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISolimarLicenseMgr7_INTERFACE_DEFINED__ */



#ifndef __SolimarLicenseManager_LIBRARY_DEFINED__
#define __SolimarLicenseManager_LIBRARY_DEFINED__

/* library SolimarLicenseManager */
/* [helpstring][uuid][version] */ 


EXTERN_C const IID LIBID_SolimarLicenseManager;

EXTERN_C const CLSID CLSID_CSolimarLicenseMgr;

#ifdef __cplusplus

class DECLSPEC_UUID("986A969C-F4C2-4EB7-85E6-F049201AEBB9")
CSolimarLicenseMgr;
#endif
#endif /* __SolimarLicenseManager_LIBRARY_DEFINED__ */

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


