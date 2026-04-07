

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Mon Jan 18 19:14:07 2038
 */
/* Compiler settings for E:\SolimarLicensing\SolimarLicensingCode\common\_SSILicenseInterfaces.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0628 
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
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef ___SSILicenseInterfaces_h__
#define ___SSILicenseInterfaces_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if defined(_CONTROL_FLOW_GUARD_XFG)
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

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


#ifndef __ISolimarLicenseMgr8_FWD_DEFINED__
#define __ISolimarLicenseMgr8_FWD_DEFINED__
typedef interface ISolimarLicenseMgr8 ISolimarLicenseMgr8;

#endif 	/* __ISolimarLicenseMgr8_FWD_DEFINED__ */


#ifndef __ISolimarLicenseSvr_FWD_DEFINED__
#define __ISolimarLicenseSvr_FWD_DEFINED__
typedef interface ISolimarLicenseSvr ISolimarLicenseSvr;

#endif 	/* __ISolimarLicenseSvr_FWD_DEFINED__ */


#ifndef __ISolimarLicenseSvr2_FWD_DEFINED__
#define __ISolimarLicenseSvr2_FWD_DEFINED__
typedef interface ISolimarLicenseSvr2 ISolimarLicenseSvr2;

#endif 	/* __ISolimarLicenseSvr2_FWD_DEFINED__ */


#ifndef __ISolimarLicenseSvr3_FWD_DEFINED__
#define __ISolimarLicenseSvr3_FWD_DEFINED__
typedef interface ISolimarLicenseSvr3 ISolimarLicenseSvr3;

#endif 	/* __ISolimarLicenseSvr3_FWD_DEFINED__ */


#ifndef __ISolimarLicenseSvr4_FWD_DEFINED__
#define __ISolimarLicenseSvr4_FWD_DEFINED__
typedef interface ISolimarLicenseSvr4 ISolimarLicenseSvr4;

#endif 	/* __ISolimarLicenseSvr4_FWD_DEFINED__ */


#ifndef __ISolimarSoftwareLicenseSvr_FWD_DEFINED__
#define __ISolimarSoftwareLicenseSvr_FWD_DEFINED__
typedef interface ISolimarSoftwareLicenseSvr ISolimarSoftwareLicenseSvr;

#endif 	/* __ISolimarSoftwareLicenseSvr_FWD_DEFINED__ */


#ifndef __ISolimarSoftwareLicenseSvr2_FWD_DEFINED__
#define __ISolimarSoftwareLicenseSvr2_FWD_DEFINED__
typedef interface ISolimarSoftwareLicenseSvr2 ISolimarSoftwareLicenseSvr2;

#endif 	/* __ISolimarSoftwareLicenseSvr2_FWD_DEFINED__ */


#ifndef __ISolimarSoftwareLicenseSvr3_FWD_DEFINED__
#define __ISolimarSoftwareLicenseSvr3_FWD_DEFINED__
typedef interface ISolimarSoftwareLicenseSvr3 ISolimarSoftwareLicenseSvr3;

#endif 	/* __ISolimarSoftwareLicenseSvr3_FWD_DEFINED__ */


#ifndef __ISoftwareLicensingMessage_FWD_DEFINED__
#define __ISoftwareLicensingMessage_FWD_DEFINED__
typedef interface ISoftwareLicensingMessage ISoftwareLicensingMessage;

#endif 	/* __ISoftwareLicensingMessage_FWD_DEFINED__ */


#ifndef __ISolimarLicenseEmailAlert_FWD_DEFINED__
#define __ISolimarLicenseEmailAlert_FWD_DEFINED__
typedef interface ISolimarLicenseEmailAlert ISolimarLicenseEmailAlert;

#endif 	/* __ISolimarLicenseEmailAlert_FWD_DEFINED__ */


#ifndef __IObjectAuthentication_FWD_DEFINED__
#define __IObjectAuthentication_FWD_DEFINED__
typedef interface IObjectAuthentication IObjectAuthentication;

#endif 	/* __IObjectAuthentication_FWD_DEFINED__ */


#ifndef __ICoAllLicenseInterfaces_FWD_DEFINED__
#define __ICoAllLicenseInterfaces_FWD_DEFINED__
typedef interface ICoAllLicenseInterfaces ICoAllLicenseInterfaces;

#endif 	/* __ICoAllLicenseInterfaces_FWD_DEFINED__ */


#ifndef __CCoAllLicenseInterfaces_FWD_DEFINED__
#define __CCoAllLicenseInterfaces_FWD_DEFINED__

#ifdef __cplusplus
typedef class CCoAllLicenseInterfaces CCoAllLicenseInterfaces;
#else
typedef struct CCoAllLicenseInterfaces CCoAllLicenseInterfaces;
#endif /* __cplusplus */

#endif 	/* __CCoAllLicenseInterfaces_FWD_DEFINED__ */


/* header files for imported files */
#include "prsht.h"
#include "MsHTML.h"
#include "MsHtmHst.h"
#include "ExDisp.h"
#include "ObjSafe.h"
#include "comadmin.h"
#include "transact.h"
#include "txcoord.h"

#ifdef __cplusplus
extern "C"{
#endif 


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
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILicensingMessage * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILicensingMessage * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILicensingMessage * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ILicensingMessage * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ILicensingMessage * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ILicensingMessage * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ILicensingMessage * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(ILicensingMessage, GetLicenseMessageList)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetLicenseMessageList )( 
            ILicensingMessage * This,
            /* [in] */ VARIANT_BOOL clear_messages,
            /* [retval][out] */ VARIANT *pvtMessageList);
        
        DECLSPEC_XFGVIRT(ILicensingMessage, DispatchLicenseMessageList)
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
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarLicenseMgr * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarLicenseMgr * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarLicenseMgr * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarLicenseMgr * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarLicenseMgr * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarLicenseMgr * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarLicenseMgr * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Connect)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            ISolimarLicenseMgr * This,
            /* [in] */ BSTR server);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Disconnect)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            ISolimarLicenseMgr * This);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Initialize)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            ISolimarLicenseMgr * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ValidateLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ValidateLicense )( 
            ISolimarLicenseMgr * This,
            /* [retval][out] */ VARIANT_BOOL *license_valid);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseTotal)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseTotal )( 
            ISolimarLicenseMgr * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseInUse)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseInUse )( 
            ISolimarLicenseMgr * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseObtain)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseObtain )( 
            ISolimarLicenseMgr * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseRelease)
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
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarLicenseMgr2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarLicenseMgr2 * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarLicenseMgr2 * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarLicenseMgr2 * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarLicenseMgr2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarLicenseMgr2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarLicenseMgr2 * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Connect)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            ISolimarLicenseMgr2 * This,
            /* [in] */ BSTR server);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Disconnect)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            ISolimarLicenseMgr2 * This);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Initialize)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            ISolimarLicenseMgr2 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ValidateLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ValidateLicense )( 
            ISolimarLicenseMgr2 * This,
            /* [retval][out] */ VARIANT_BOOL *license_valid);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseTotal)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseTotal )( 
            ISolimarLicenseMgr2 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseInUse)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseInUse )( 
            ISolimarLicenseMgr2 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseObtain)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseObtain )( 
            ISolimarLicenseMgr2 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseRelease)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseRelease )( 
            ISolimarLicenseMgr2 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr2, ModuleLicenseSerialNumbers)
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
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarLicenseMgr3 * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarLicenseMgr3 * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarLicenseMgr3 * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarLicenseMgr3 * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Connect)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ BSTR server);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Disconnect)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            ISolimarLicenseMgr3 * This);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Initialize)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ValidateLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ValidateLicense )( 
            ISolimarLicenseMgr3 * This,
            /* [retval][out] */ VARIANT_BOOL *license_valid);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseTotal)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseTotal )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseInUse)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseInUse )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseObtain)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseObtain )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseRelease)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseRelease )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr2, ModuleLicenseSerialNumbers)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseSerialNumbers )( 
            ISolimarLicenseMgr3 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ VARIANT *pvtSerialNumberList);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr3, ModuleLicenseCounterDecrement)
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
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarLicenseMgr4 * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarLicenseMgr4 * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarLicenseMgr4 * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarLicenseMgr4 * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Connect)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ BSTR server);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Disconnect)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            ISolimarLicenseMgr4 * This);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Initialize)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ValidateLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ValidateLicense )( 
            ISolimarLicenseMgr4 * This,
            /* [retval][out] */ VARIANT_BOOL *license_valid);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseTotal)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseTotal )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseInUse)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseInUse )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseObtain)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseObtain )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseRelease)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseRelease )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr2, ModuleLicenseSerialNumbers)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseSerialNumbers )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ VARIANT *pvtSerialNumberList);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr3, ModuleLicenseCounterDecrement)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseCounterDecrement )( 
            ISolimarLicenseMgr4 * This,
            /* [in] */ long module_id,
            /* [in] */ long license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr4, Initialize2)
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
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarLicenseMgr5 * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarLicenseMgr5 * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarLicenseMgr5 * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarLicenseMgr5 * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Connect)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ BSTR server);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Disconnect)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            ISolimarLicenseMgr5 * This);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Initialize)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ValidateLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ValidateLicense )( 
            ISolimarLicenseMgr5 * This,
            /* [retval][out] */ VARIANT_BOOL *license_valid);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseTotal)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseTotal )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseInUse)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseInUse )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseObtain)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseObtain )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseRelease)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseRelease )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr2, ModuleLicenseSerialNumbers)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseSerialNumbers )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ VARIANT *pvtSerialNumberList);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr3, ModuleLicenseCounterDecrement)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseCounterDecrement )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ long module_id,
            /* [in] */ long license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr4, Initialize2)
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
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, Connect2)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect2 )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ BSTR server,
            /* [in] */ VARIANT_BOOL use_only_shared_licenses,
            /* [in] */ VARIANT_BOOL use_as_backup);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, Initialize3)
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
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, GetVersionLicenseManager)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVersionLicenseManager )( 
            ISolimarLicenseMgr5 * This,
            /* [out] */ long *p_ver_major,
            /* [out] */ long *p_ver_minor,
            /* [out] */ long *p_ver_build);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, GetVersionLicenseServer)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVersionLicenseServer )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ BSTR server,
            /* [out] */ long *p_ver_major,
            /* [out] */ long *p_ver_minor,
            /* [out] */ long *p_ver_build);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, ConnectByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ConnectByProduct )( 
            ISolimarLicenseMgr5 * This,
            /* [in] */ long product,
            /* [in] */ VARIANT_BOOL use_shared_licenses_servers);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, KeyProductExists)
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
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarLicenseMgr6 * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarLicenseMgr6 * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarLicenseMgr6 * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarLicenseMgr6 * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Connect)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ BSTR server);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Disconnect)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            ISolimarLicenseMgr6 * This);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Initialize)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ValidateLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ValidateLicense )( 
            ISolimarLicenseMgr6 * This,
            /* [retval][out] */ VARIANT_BOOL *license_valid);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseTotal)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseTotal )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseInUse)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseInUse )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseObtain)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseObtain )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseRelease)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseRelease )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr2, ModuleLicenseSerialNumbers)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseSerialNumbers )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ VARIANT *pvtSerialNumberList);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr3, ModuleLicenseCounterDecrement)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseCounterDecrement )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ long module_id,
            /* [in] */ long license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr4, Initialize2)
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
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, Connect2)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect2 )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ BSTR server,
            /* [in] */ VARIANT_BOOL use_only_shared_licenses,
            /* [in] */ VARIANT_BOOL use_as_backup);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, Initialize3)
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
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, GetVersionLicenseManager)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVersionLicenseManager )( 
            ISolimarLicenseMgr6 * This,
            /* [out] */ long *p_ver_major,
            /* [out] */ long *p_ver_minor,
            /* [out] */ long *p_ver_build);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, GetVersionLicenseServer)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVersionLicenseServer )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ BSTR server,
            /* [out] */ long *p_ver_major,
            /* [out] */ long *p_ver_minor,
            /* [out] */ long *p_ver_build);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, ConnectByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ConnectByProduct )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ long product,
            /* [in] */ VARIANT_BOOL use_shared_licenses_servers);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, KeyProductExists)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyProductExists )( 
            ISolimarLicenseMgr6 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [out] */ VARIANT_BOOL *p_bool_key_product_exists);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr6, ModuleLicenseInUse_ByApp)
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
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarLicenseMgr7 * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarLicenseMgr7 * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarLicenseMgr7 * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarLicenseMgr7 * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Connect)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ BSTR server);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Disconnect)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            ISolimarLicenseMgr7 * This);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Initialize)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ValidateLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ValidateLicense )( 
            ISolimarLicenseMgr7 * This,
            /* [retval][out] */ VARIANT_BOOL *license_valid);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseTotal)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseTotal )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseInUse)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseInUse )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseObtain)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseObtain )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseRelease)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseRelease )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr2, ModuleLicenseSerialNumbers)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseSerialNumbers )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ VARIANT *pvtSerialNumberList);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr3, ModuleLicenseCounterDecrement)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseCounterDecrement )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long module_id,
            /* [in] */ long license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr4, Initialize2)
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
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, Connect2)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect2 )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ BSTR server,
            /* [in] */ VARIANT_BOOL use_only_shared_licenses,
            /* [in] */ VARIANT_BOOL use_as_backup);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, Initialize3)
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
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, GetVersionLicenseManager)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVersionLicenseManager )( 
            ISolimarLicenseMgr7 * This,
            /* [out] */ long *p_ver_major,
            /* [out] */ long *p_ver_minor,
            /* [out] */ long *p_ver_build);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, GetVersionLicenseServer)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVersionLicenseServer )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ BSTR server,
            /* [out] */ long *p_ver_major,
            /* [out] */ long *p_ver_minor,
            /* [out] */ long *p_ver_build);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, ConnectByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ConnectByProduct )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long product,
            /* [in] */ VARIANT_BOOL use_shared_licenses_servers);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, KeyProductExists)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyProductExists )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [out] */ VARIANT_BOOL *p_bool_key_product_exists);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr6, ModuleLicenseInUse_ByApp)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseInUse_ByApp )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr7, Connect3)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect3 )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ BSTR server,
            /* [in] */ long connectionFlags);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr7, SoftwareLicense_Initialize)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareLicense_Initialize )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ BSTR application_instance,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ long auto_ui_level,
            /* [in] */ unsigned long grace_period_minutes);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr7, SoftwareLicense_InitializeViewOnly)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareLicense_InitializeViewOnly )( 
            ISolimarLicenseMgr7 * This,
            /* [in] */ BSTR application_instance,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr7, GetInfoByProduct)
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


#ifndef __ISolimarLicenseMgr8_INTERFACE_DEFINED__
#define __ISolimarLicenseMgr8_INTERFACE_DEFINED__

/* interface ISolimarLicenseMgr8 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISolimarLicenseMgr8;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A575B758-4C33-4DF4-A110-F8BB712AB9ED")
    ISolimarLicenseMgr8 : public ISolimarLicenseMgr7
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE InitializeAuthInfo( 
            /* [in] */ BSTR domain,
            /* [in] */ BSTR username,
            /* [in] */ BSTR password,
            /* [in] */ long authenticationLevel,
            /* [in] */ long impersonationLevel) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ISolimarLicenseMgr8Vtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarLicenseMgr8 * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarLicenseMgr8 * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarLicenseMgr8 * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarLicenseMgr8 * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Connect)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ BSTR server);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Disconnect)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            ISolimarLicenseMgr8 * This);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, Initialize)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ValidateLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ValidateLicense )( 
            ISolimarLicenseMgr8 * This,
            /* [retval][out] */ VARIANT_BOOL *license_valid);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseTotal)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseTotal )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseInUse)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseInUse )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseObtain)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseObtain )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr, ModuleLicenseRelease)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseRelease )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ long module_id,
            /* [in] */ long count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr2, ModuleLicenseSerialNumbers)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseSerialNumbers )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ VARIANT *pvtSerialNumberList);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr3, ModuleLicenseCounterDecrement)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseCounterDecrement )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ long module_id,
            /* [in] */ long license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr4, Initialize2)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize2 )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ VARIANT_BOOL single_key,
            /* [in] */ BSTR specific_single_key_ident,
            /* [in] */ VARIANT_BOOL lock_keys,
            /* [in] */ long auto_ui_level,
            /* [in] */ unsigned long grace_period_minutes);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, Connect2)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect2 )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ BSTR server,
            /* [in] */ VARIANT_BOOL use_only_shared_licenses,
            /* [in] */ VARIANT_BOOL use_as_backup);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, Initialize3)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Initialize3 )( 
            ISolimarLicenseMgr8 * This,
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
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, GetVersionLicenseManager)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVersionLicenseManager )( 
            ISolimarLicenseMgr8 * This,
            /* [out] */ long *p_ver_major,
            /* [out] */ long *p_ver_minor,
            /* [out] */ long *p_ver_build);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, GetVersionLicenseServer)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVersionLicenseServer )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ BSTR server,
            /* [out] */ long *p_ver_major,
            /* [out] */ long *p_ver_minor,
            /* [out] */ long *p_ver_build);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, ConnectByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ConnectByProduct )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ long product,
            /* [in] */ VARIANT_BOOL use_shared_licenses_servers);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr5, KeyProductExists)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyProductExists )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [out] */ VARIANT_BOOL *p_bool_key_product_exists);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr6, ModuleLicenseInUse_ByApp)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModuleLicenseInUse_ByApp )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ long module_id,
            /* [retval][out] */ long *count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr7, Connect3)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect3 )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ BSTR server,
            /* [in] */ long connectionFlags);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr7, SoftwareLicense_Initialize)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareLicense_Initialize )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ BSTR application_instance,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor,
            /* [in] */ long auto_ui_level,
            /* [in] */ unsigned long grace_period_minutes);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr7, SoftwareLicense_InitializeViewOnly)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareLicense_InitializeViewOnly )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ BSTR application_instance,
            /* [in] */ long product,
            /* [in] */ long prod_ver_major,
            /* [in] */ long prod_ver_minor);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr7, GetInfoByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetInfoByProduct )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ long product,
            /* [in] */ VARIANT_BOOL use_shared_licenses_servers,
            /* [out] */ BSTR *p_server,
            /* [out] */ BSTR *p_backup_server,
            /* [out] */ VARIANT_BOOL *p_bool_use_test_dev_licensing);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseMgr8, InitializeAuthInfo)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *InitializeAuthInfo )( 
            ISolimarLicenseMgr8 * This,
            /* [in] */ BSTR domain,
            /* [in] */ BSTR username,
            /* [in] */ BSTR password,
            /* [in] */ long authenticationLevel,
            /* [in] */ long impersonationLevel);
        
        END_INTERFACE
    } ISolimarLicenseMgr8Vtbl;

    interface ISolimarLicenseMgr8
    {
        CONST_VTBL struct ISolimarLicenseMgr8Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISolimarLicenseMgr8_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISolimarLicenseMgr8_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISolimarLicenseMgr8_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISolimarLicenseMgr8_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISolimarLicenseMgr8_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISolimarLicenseMgr8_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISolimarLicenseMgr8_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISolimarLicenseMgr8_Connect(This,server)	\
    ( (This)->lpVtbl -> Connect(This,server) ) 

#define ISolimarLicenseMgr8_Disconnect(This)	\
    ( (This)->lpVtbl -> Disconnect(This) ) 

#define ISolimarLicenseMgr8_Initialize(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level)	\
    ( (This)->lpVtbl -> Initialize(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level) ) 

#define ISolimarLicenseMgr8_ValidateLicense(This,license_valid)	\
    ( (This)->lpVtbl -> ValidateLicense(This,license_valid) ) 

#define ISolimarLicenseMgr8_ModuleLicenseTotal(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseTotal(This,module_id,count) ) 

#define ISolimarLicenseMgr8_ModuleLicenseInUse(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseInUse(This,module_id,count) ) 

#define ISolimarLicenseMgr8_ModuleLicenseObtain(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseObtain(This,module_id,count) ) 

#define ISolimarLicenseMgr8_ModuleLicenseRelease(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseRelease(This,module_id,count) ) 


#define ISolimarLicenseMgr8_ModuleLicenseSerialNumbers(This,module_id,pvtSerialNumberList)	\
    ( (This)->lpVtbl -> ModuleLicenseSerialNumbers(This,module_id,pvtSerialNumberList) ) 


#define ISolimarLicenseMgr8_ModuleLicenseCounterDecrement(This,module_id,license_count)	\
    ( (This)->lpVtbl -> ModuleLicenseCounterDecrement(This,module_id,license_count) ) 


#define ISolimarLicenseMgr8_Initialize2(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level,grace_period_minutes)	\
    ( (This)->lpVtbl -> Initialize2(This,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level,grace_period_minutes) ) 


#define ISolimarLicenseMgr8_Connect2(This,server,use_only_shared_licenses,use_as_backup)	\
    ( (This)->lpVtbl -> Connect2(This,server,use_only_shared_licenses,use_as_backup) ) 

#define ISolimarLicenseMgr8_Initialize3(This,application_instance,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level,grace_period_minutes,app_instance_lock_key,b_bypass_remote_key_restriction)	\
    ( (This)->lpVtbl -> Initialize3(This,application_instance,product,prod_ver_major,prod_ver_minor,single_key,specific_single_key_ident,lock_keys,auto_ui_level,grace_period_minutes,app_instance_lock_key,b_bypass_remote_key_restriction) ) 

#define ISolimarLicenseMgr8_GetVersionLicenseManager(This,p_ver_major,p_ver_minor,p_ver_build)	\
    ( (This)->lpVtbl -> GetVersionLicenseManager(This,p_ver_major,p_ver_minor,p_ver_build) ) 

#define ISolimarLicenseMgr8_GetVersionLicenseServer(This,server,p_ver_major,p_ver_minor,p_ver_build)	\
    ( (This)->lpVtbl -> GetVersionLicenseServer(This,server,p_ver_major,p_ver_minor,p_ver_build) ) 

#define ISolimarLicenseMgr8_ConnectByProduct(This,product,use_shared_licenses_servers)	\
    ( (This)->lpVtbl -> ConnectByProduct(This,product,use_shared_licenses_servers) ) 

#define ISolimarLicenseMgr8_KeyProductExists(This,product,prod_ver_major,prod_ver_minor,p_bool_key_product_exists)	\
    ( (This)->lpVtbl -> KeyProductExists(This,product,prod_ver_major,prod_ver_minor,p_bool_key_product_exists) ) 


#define ISolimarLicenseMgr8_ModuleLicenseInUse_ByApp(This,module_id,count)	\
    ( (This)->lpVtbl -> ModuleLicenseInUse_ByApp(This,module_id,count) ) 


#define ISolimarLicenseMgr8_Connect3(This,server,connectionFlags)	\
    ( (This)->lpVtbl -> Connect3(This,server,connectionFlags) ) 

#define ISolimarLicenseMgr8_SoftwareLicense_Initialize(This,application_instance,product,prod_ver_major,prod_ver_minor,auto_ui_level,grace_period_minutes)	\
    ( (This)->lpVtbl -> SoftwareLicense_Initialize(This,application_instance,product,prod_ver_major,prod_ver_minor,auto_ui_level,grace_period_minutes) ) 

#define ISolimarLicenseMgr8_SoftwareLicense_InitializeViewOnly(This,application_instance,product,prod_ver_major,prod_ver_minor)	\
    ( (This)->lpVtbl -> SoftwareLicense_InitializeViewOnly(This,application_instance,product,prod_ver_major,prod_ver_minor) ) 

#define ISolimarLicenseMgr8_GetInfoByProduct(This,product,use_shared_licenses_servers,p_server,p_backup_server,p_bool_use_test_dev_licensing)	\
    ( (This)->lpVtbl -> GetInfoByProduct(This,product,use_shared_licenses_servers,p_server,p_backup_server,p_bool_use_test_dev_licensing) ) 


#define ISolimarLicenseMgr8_InitializeAuthInfo(This,domain,username,password,authenticationLevel,impersonationLevel)	\
    ( (This)->lpVtbl -> InitializeAuthInfo(This,domain,username,password,authenticationLevel,impersonationLevel) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISolimarLicenseMgr8_INTERFACE_DEFINED__ */


#ifndef __ISolimarLicenseSvr_INTERFACE_DEFINED__
#define __ISolimarLicenseSvr_INTERFACE_DEFINED__

/* interface ISolimarLicenseSvr */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISolimarLicenseSvr;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C6CCE857-1A24-41D5-A44C-4DC629CD370D")
    ISolimarLicenseSvr : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Heartbeat( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyEnumerate( 
            /* [retval][out] */ VARIANT *keylist) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnterPassword( 
            /* [in] */ BSTR password) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnterPasswordPacket( 
            /* [in] */ VARIANT vtPasswordPacket,
            /* [retval][out] */ BSTR *verification_code) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenerateBasePassword( 
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [retval][out] */ BSTR *password) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenerateVersionPassword( 
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long ver_major,
            /* [in] */ long ver_minor,
            /* [retval][out] */ BSTR *password) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenerateExtensionPassword( 
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long extend_days,
            /* [in] */ long extension_num,
            /* [retval][out] */ BSTR *password) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenerateModulePassword( 
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long product_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count,
            /* [retval][out] */ BSTR *password) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetLicenseServerTime( 
            /* [retval][out] */ VARIANT *pvtSystemTime) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenerateModulePassword2( 
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long product_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count,
            /* [in] */ long password_number,
            /* [retval][out] */ BSTR *password) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PasswordPacketInitialize( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PasswordPacketAppendPassword( 
            /* [in] */ VARIANT vtExpires,
            /* [in] */ BSTR password) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PasswordPacketFinalize( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PasswordPacketGetPacket( 
            /* [retval][out] */ VARIANT *pvtPacketData) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PasswordPacketGetVerificationCode( 
            /* [retval][out] */ BSTR *verification_code) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyTrialExpires( 
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT *expire_date) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyTrialHours( 
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ long *trial_hours) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyIsActive( 
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT_BOOL *key_active) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyIsProgrammed( 
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT_BOOL *key_programmed) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyIsPresent( 
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT_BOOL *key_present) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyHeaderQuery( 
            /* [in] */ BSTR key_ident,
            /* [in] */ long header_ident,
            /* [retval][out] */ VARIANT *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyLock( 
            /* [in] */ BSTR key_ident) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyUnlock( 
            /* [in] */ BSTR key_ident) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyObtain( 
            /* [in] */ BSTR key_ident) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyRelease( 
            /* [in] */ BSTR key_ident) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyValidateLicense( 
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT_BOOL *license_valid) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyModuleEnumerate( 
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT *key_module_list) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyModuleQuery( 
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_identifier,
            /* [retval][out] */ VARIANT *vtValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyModuleLicenseTotal( 
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [retval][out] */ long *license_count) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyModuleLicenseInUse( 
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [retval][out] */ long *license_count) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyModuleLicenseObtain( 
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyModuleLicenseRelease( 
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyModuleLicenseCounterDecrement( 
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyFormat( 
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ BSTR *new_key_ident) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyProgram( 
            /* [in] */ BSTR key_ident,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long product_ident,
            /* [in] */ long ver_major,
            /* [in] */ long ver_minor,
            /* [in] */ long key_type,
            /* [in] */ long days,
            /* [in] */ VARIANT module_value_list,
            /* [retval][out] */ BSTR *new_key_ident) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyReadRaw( 
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT *pvtKeyData) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ISolimarLicenseSvrVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarLicenseSvr * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarLicenseSvr * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarLicenseSvr * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarLicenseSvr * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarLicenseSvr * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarLicenseSvr * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarLicenseSvr * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, Heartbeat)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Heartbeat )( 
            ISolimarLicenseSvr * This);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyEnumerate)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyEnumerate )( 
            ISolimarLicenseSvr * This,
            /* [retval][out] */ VARIANT *keylist);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, EnterPassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnterPassword )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, EnterPasswordPacket)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnterPasswordPacket )( 
            ISolimarLicenseSvr * This,
            /* [in] */ VARIANT vtPasswordPacket,
            /* [retval][out] */ BSTR *verification_code);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GenerateBasePassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateBasePassword )( 
            ISolimarLicenseSvr * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GenerateVersionPassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateVersionPassword )( 
            ISolimarLicenseSvr * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long ver_major,
            /* [in] */ long ver_minor,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GenerateExtensionPassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateExtensionPassword )( 
            ISolimarLicenseSvr * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long extend_days,
            /* [in] */ long extension_num,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GenerateModulePassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateModulePassword )( 
            ISolimarLicenseSvr * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long product_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GetLicenseServerTime)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetLicenseServerTime )( 
            ISolimarLicenseSvr * This,
            /* [retval][out] */ VARIANT *pvtSystemTime);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GenerateModulePassword2)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateModulePassword2 )( 
            ISolimarLicenseSvr * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long product_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count,
            /* [in] */ long password_number,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, PasswordPacketInitialize)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PasswordPacketInitialize )( 
            ISolimarLicenseSvr * This);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, PasswordPacketAppendPassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PasswordPacketAppendPassword )( 
            ISolimarLicenseSvr * This,
            /* [in] */ VARIANT vtExpires,
            /* [in] */ BSTR password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, PasswordPacketFinalize)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PasswordPacketFinalize )( 
            ISolimarLicenseSvr * This);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, PasswordPacketGetPacket)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PasswordPacketGetPacket )( 
            ISolimarLicenseSvr * This,
            /* [retval][out] */ VARIANT *pvtPacketData);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, PasswordPacketGetVerificationCode)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PasswordPacketGetVerificationCode )( 
            ISolimarLicenseSvr * This,
            /* [retval][out] */ BSTR *verification_code);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyTrialExpires)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyTrialExpires )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT *expire_date);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyTrialHours)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyTrialHours )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ long *trial_hours);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyIsActive)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyIsActive )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT_BOOL *key_active);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyIsProgrammed)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyIsProgrammed )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT_BOOL *key_programmed);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyIsPresent)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyIsPresent )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT_BOOL *key_present);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyHeaderQuery)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyHeaderQuery )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long header_ident,
            /* [retval][out] */ VARIANT *value);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyLock)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyLock )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyUnlock)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyUnlock )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyObtain)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyObtain )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyRelease)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyRelease )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyValidateLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyValidateLicense )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT_BOOL *license_valid);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleEnumerate)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleEnumerate )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT *key_module_list);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleQuery)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleQuery )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_identifier,
            /* [retval][out] */ VARIANT *vtValue);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleLicenseTotal)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseTotal )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [retval][out] */ long *license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleLicenseInUse)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseInUse )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [retval][out] */ long *license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleLicenseObtain)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseObtain )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleLicenseRelease)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseRelease )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleLicenseCounterDecrement)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseCounterDecrement )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyFormat)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyFormat )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ BSTR *new_key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyProgram)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyProgram )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long product_ident,
            /* [in] */ long ver_major,
            /* [in] */ long ver_minor,
            /* [in] */ long key_type,
            /* [in] */ long days,
            /* [in] */ VARIANT module_value_list,
            /* [retval][out] */ BSTR *new_key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyReadRaw)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyReadRaw )( 
            ISolimarLicenseSvr * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT *pvtKeyData);
        
        END_INTERFACE
    } ISolimarLicenseSvrVtbl;

    interface ISolimarLicenseSvr
    {
        CONST_VTBL struct ISolimarLicenseSvrVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISolimarLicenseSvr_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISolimarLicenseSvr_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISolimarLicenseSvr_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISolimarLicenseSvr_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISolimarLicenseSvr_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISolimarLicenseSvr_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISolimarLicenseSvr_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISolimarLicenseSvr_Heartbeat(This)	\
    ( (This)->lpVtbl -> Heartbeat(This) ) 

#define ISolimarLicenseSvr_KeyEnumerate(This,keylist)	\
    ( (This)->lpVtbl -> KeyEnumerate(This,keylist) ) 

#define ISolimarLicenseSvr_EnterPassword(This,password)	\
    ( (This)->lpVtbl -> EnterPassword(This,password) ) 

#define ISolimarLicenseSvr_EnterPasswordPacket(This,vtPasswordPacket,verification_code)	\
    ( (This)->lpVtbl -> EnterPasswordPacket(This,vtPasswordPacket,verification_code) ) 

#define ISolimarLicenseSvr_GenerateBasePassword(This,customer_number,key_number,password)	\
    ( (This)->lpVtbl -> GenerateBasePassword(This,customer_number,key_number,password) ) 

#define ISolimarLicenseSvr_GenerateVersionPassword(This,customer_number,key_number,ver_major,ver_minor,password)	\
    ( (This)->lpVtbl -> GenerateVersionPassword(This,customer_number,key_number,ver_major,ver_minor,password) ) 

#define ISolimarLicenseSvr_GenerateExtensionPassword(This,customer_number,key_number,extend_days,extension_num,password)	\
    ( (This)->lpVtbl -> GenerateExtensionPassword(This,customer_number,key_number,extend_days,extension_num,password) ) 

#define ISolimarLicenseSvr_GenerateModulePassword(This,customer_number,key_number,product_ident,module_ident,license_count,password)	\
    ( (This)->lpVtbl -> GenerateModulePassword(This,customer_number,key_number,product_ident,module_ident,license_count,password) ) 

#define ISolimarLicenseSvr_GetLicenseServerTime(This,pvtSystemTime)	\
    ( (This)->lpVtbl -> GetLicenseServerTime(This,pvtSystemTime) ) 

#define ISolimarLicenseSvr_GenerateModulePassword2(This,customer_number,key_number,product_ident,module_ident,license_count,password_number,password)	\
    ( (This)->lpVtbl -> GenerateModulePassword2(This,customer_number,key_number,product_ident,module_ident,license_count,password_number,password) ) 

#define ISolimarLicenseSvr_PasswordPacketInitialize(This)	\
    ( (This)->lpVtbl -> PasswordPacketInitialize(This) ) 

#define ISolimarLicenseSvr_PasswordPacketAppendPassword(This,vtExpires,password)	\
    ( (This)->lpVtbl -> PasswordPacketAppendPassword(This,vtExpires,password) ) 

#define ISolimarLicenseSvr_PasswordPacketFinalize(This)	\
    ( (This)->lpVtbl -> PasswordPacketFinalize(This) ) 

#define ISolimarLicenseSvr_PasswordPacketGetPacket(This,pvtPacketData)	\
    ( (This)->lpVtbl -> PasswordPacketGetPacket(This,pvtPacketData) ) 

#define ISolimarLicenseSvr_PasswordPacketGetVerificationCode(This,verification_code)	\
    ( (This)->lpVtbl -> PasswordPacketGetVerificationCode(This,verification_code) ) 

#define ISolimarLicenseSvr_KeyTrialExpires(This,key_ident,expire_date)	\
    ( (This)->lpVtbl -> KeyTrialExpires(This,key_ident,expire_date) ) 

#define ISolimarLicenseSvr_KeyTrialHours(This,key_ident,trial_hours)	\
    ( (This)->lpVtbl -> KeyTrialHours(This,key_ident,trial_hours) ) 

#define ISolimarLicenseSvr_KeyIsActive(This,key_ident,key_active)	\
    ( (This)->lpVtbl -> KeyIsActive(This,key_ident,key_active) ) 

#define ISolimarLicenseSvr_KeyIsProgrammed(This,key_ident,key_programmed)	\
    ( (This)->lpVtbl -> KeyIsProgrammed(This,key_ident,key_programmed) ) 

#define ISolimarLicenseSvr_KeyIsPresent(This,key_ident,key_present)	\
    ( (This)->lpVtbl -> KeyIsPresent(This,key_ident,key_present) ) 

#define ISolimarLicenseSvr_KeyHeaderQuery(This,key_ident,header_ident,value)	\
    ( (This)->lpVtbl -> KeyHeaderQuery(This,key_ident,header_ident,value) ) 

#define ISolimarLicenseSvr_KeyLock(This,key_ident)	\
    ( (This)->lpVtbl -> KeyLock(This,key_ident) ) 

#define ISolimarLicenseSvr_KeyUnlock(This,key_ident)	\
    ( (This)->lpVtbl -> KeyUnlock(This,key_ident) ) 

#define ISolimarLicenseSvr_KeyObtain(This,key_ident)	\
    ( (This)->lpVtbl -> KeyObtain(This,key_ident) ) 

#define ISolimarLicenseSvr_KeyRelease(This,key_ident)	\
    ( (This)->lpVtbl -> KeyRelease(This,key_ident) ) 

#define ISolimarLicenseSvr_KeyValidateLicense(This,key_ident,license_valid)	\
    ( (This)->lpVtbl -> KeyValidateLicense(This,key_ident,license_valid) ) 

#define ISolimarLicenseSvr_KeyModuleEnumerate(This,key_ident,key_module_list)	\
    ( (This)->lpVtbl -> KeyModuleEnumerate(This,key_ident,key_module_list) ) 

#define ISolimarLicenseSvr_KeyModuleQuery(This,key_ident,module_identifier,vtValue)	\
    ( (This)->lpVtbl -> KeyModuleQuery(This,key_ident,module_identifier,vtValue) ) 

#define ISolimarLicenseSvr_KeyModuleLicenseTotal(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseTotal(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr_KeyModuleLicenseInUse(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseInUse(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr_KeyModuleLicenseObtain(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseObtain(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr_KeyModuleLicenseRelease(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseRelease(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr_KeyModuleLicenseCounterDecrement(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseCounterDecrement(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr_KeyFormat(This,key_ident,new_key_ident)	\
    ( (This)->lpVtbl -> KeyFormat(This,key_ident,new_key_ident) ) 

#define ISolimarLicenseSvr_KeyProgram(This,key_ident,customer_number,key_number,product_ident,ver_major,ver_minor,key_type,days,module_value_list,new_key_ident)	\
    ( (This)->lpVtbl -> KeyProgram(This,key_ident,customer_number,key_number,product_ident,ver_major,ver_minor,key_type,days,module_value_list,new_key_ident) ) 

#define ISolimarLicenseSvr_KeyReadRaw(This,key_ident,pvtKeyData)	\
    ( (This)->lpVtbl -> KeyReadRaw(This,key_ident,pvtKeyData) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISolimarLicenseSvr_INTERFACE_DEFINED__ */


#ifndef __ISolimarLicenseSvr2_INTERFACE_DEFINED__
#define __ISolimarLicenseSvr2_INTERFACE_DEFINED__

/* interface ISolimarLicenseSvr2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISolimarLicenseSvr2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4852B206-97BA-4934-AFD8-35E85F7FC855")
    ISolimarLicenseSvr2 : public ISolimarLicenseSvr
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyModuleLicenseInUse2( 
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [retval][out] */ long *license_count) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyModuleInUse( 
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [retval][out] */ long *license_count) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ISolimarLicenseSvr2Vtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarLicenseSvr2 * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarLicenseSvr2 * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarLicenseSvr2 * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarLicenseSvr2 * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, Heartbeat)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Heartbeat )( 
            ISolimarLicenseSvr2 * This);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyEnumerate)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyEnumerate )( 
            ISolimarLicenseSvr2 * This,
            /* [retval][out] */ VARIANT *keylist);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, EnterPassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnterPassword )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, EnterPasswordPacket)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnterPasswordPacket )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ VARIANT vtPasswordPacket,
            /* [retval][out] */ BSTR *verification_code);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GenerateBasePassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateBasePassword )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GenerateVersionPassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateVersionPassword )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long ver_major,
            /* [in] */ long ver_minor,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GenerateExtensionPassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateExtensionPassword )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long extend_days,
            /* [in] */ long extension_num,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GenerateModulePassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateModulePassword )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long product_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GetLicenseServerTime)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetLicenseServerTime )( 
            ISolimarLicenseSvr2 * This,
            /* [retval][out] */ VARIANT *pvtSystemTime);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GenerateModulePassword2)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateModulePassword2 )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long product_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count,
            /* [in] */ long password_number,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, PasswordPacketInitialize)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PasswordPacketInitialize )( 
            ISolimarLicenseSvr2 * This);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, PasswordPacketAppendPassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PasswordPacketAppendPassword )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ VARIANT vtExpires,
            /* [in] */ BSTR password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, PasswordPacketFinalize)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PasswordPacketFinalize )( 
            ISolimarLicenseSvr2 * This);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, PasswordPacketGetPacket)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PasswordPacketGetPacket )( 
            ISolimarLicenseSvr2 * This,
            /* [retval][out] */ VARIANT *pvtPacketData);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, PasswordPacketGetVerificationCode)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PasswordPacketGetVerificationCode )( 
            ISolimarLicenseSvr2 * This,
            /* [retval][out] */ BSTR *verification_code);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyTrialExpires)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyTrialExpires )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT *expire_date);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyTrialHours)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyTrialHours )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ long *trial_hours);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyIsActive)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyIsActive )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT_BOOL *key_active);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyIsProgrammed)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyIsProgrammed )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT_BOOL *key_programmed);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyIsPresent)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyIsPresent )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT_BOOL *key_present);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyHeaderQuery)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyHeaderQuery )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long header_ident,
            /* [retval][out] */ VARIANT *value);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyLock)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyLock )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyUnlock)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyUnlock )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyObtain)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyObtain )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyRelease)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyRelease )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyValidateLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyValidateLicense )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT_BOOL *license_valid);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleEnumerate)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleEnumerate )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT *key_module_list);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleQuery)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleQuery )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_identifier,
            /* [retval][out] */ VARIANT *vtValue);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleLicenseTotal)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseTotal )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [retval][out] */ long *license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleLicenseInUse)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseInUse )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [retval][out] */ long *license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleLicenseObtain)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseObtain )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleLicenseRelease)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseRelease )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleLicenseCounterDecrement)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseCounterDecrement )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyFormat)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyFormat )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ BSTR *new_key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyProgram)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyProgram )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long product_ident,
            /* [in] */ long ver_major,
            /* [in] */ long ver_minor,
            /* [in] */ long key_type,
            /* [in] */ long days,
            /* [in] */ VARIANT module_value_list,
            /* [retval][out] */ BSTR *new_key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyReadRaw)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyReadRaw )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT *pvtKeyData);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr2, KeyModuleLicenseInUse2)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseInUse2 )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [retval][out] */ long *license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr2, KeyModuleInUse)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleInUse )( 
            ISolimarLicenseSvr2 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [retval][out] */ long *license_count);
        
        END_INTERFACE
    } ISolimarLicenseSvr2Vtbl;

    interface ISolimarLicenseSvr2
    {
        CONST_VTBL struct ISolimarLicenseSvr2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISolimarLicenseSvr2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISolimarLicenseSvr2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISolimarLicenseSvr2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISolimarLicenseSvr2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISolimarLicenseSvr2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISolimarLicenseSvr2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISolimarLicenseSvr2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISolimarLicenseSvr2_Heartbeat(This)	\
    ( (This)->lpVtbl -> Heartbeat(This) ) 

#define ISolimarLicenseSvr2_KeyEnumerate(This,keylist)	\
    ( (This)->lpVtbl -> KeyEnumerate(This,keylist) ) 

#define ISolimarLicenseSvr2_EnterPassword(This,password)	\
    ( (This)->lpVtbl -> EnterPassword(This,password) ) 

#define ISolimarLicenseSvr2_EnterPasswordPacket(This,vtPasswordPacket,verification_code)	\
    ( (This)->lpVtbl -> EnterPasswordPacket(This,vtPasswordPacket,verification_code) ) 

#define ISolimarLicenseSvr2_GenerateBasePassword(This,customer_number,key_number,password)	\
    ( (This)->lpVtbl -> GenerateBasePassword(This,customer_number,key_number,password) ) 

#define ISolimarLicenseSvr2_GenerateVersionPassword(This,customer_number,key_number,ver_major,ver_minor,password)	\
    ( (This)->lpVtbl -> GenerateVersionPassword(This,customer_number,key_number,ver_major,ver_minor,password) ) 

#define ISolimarLicenseSvr2_GenerateExtensionPassword(This,customer_number,key_number,extend_days,extension_num,password)	\
    ( (This)->lpVtbl -> GenerateExtensionPassword(This,customer_number,key_number,extend_days,extension_num,password) ) 

#define ISolimarLicenseSvr2_GenerateModulePassword(This,customer_number,key_number,product_ident,module_ident,license_count,password)	\
    ( (This)->lpVtbl -> GenerateModulePassword(This,customer_number,key_number,product_ident,module_ident,license_count,password) ) 

#define ISolimarLicenseSvr2_GetLicenseServerTime(This,pvtSystemTime)	\
    ( (This)->lpVtbl -> GetLicenseServerTime(This,pvtSystemTime) ) 

#define ISolimarLicenseSvr2_GenerateModulePassword2(This,customer_number,key_number,product_ident,module_ident,license_count,password_number,password)	\
    ( (This)->lpVtbl -> GenerateModulePassword2(This,customer_number,key_number,product_ident,module_ident,license_count,password_number,password) ) 

#define ISolimarLicenseSvr2_PasswordPacketInitialize(This)	\
    ( (This)->lpVtbl -> PasswordPacketInitialize(This) ) 

#define ISolimarLicenseSvr2_PasswordPacketAppendPassword(This,vtExpires,password)	\
    ( (This)->lpVtbl -> PasswordPacketAppendPassword(This,vtExpires,password) ) 

#define ISolimarLicenseSvr2_PasswordPacketFinalize(This)	\
    ( (This)->lpVtbl -> PasswordPacketFinalize(This) ) 

#define ISolimarLicenseSvr2_PasswordPacketGetPacket(This,pvtPacketData)	\
    ( (This)->lpVtbl -> PasswordPacketGetPacket(This,pvtPacketData) ) 

#define ISolimarLicenseSvr2_PasswordPacketGetVerificationCode(This,verification_code)	\
    ( (This)->lpVtbl -> PasswordPacketGetVerificationCode(This,verification_code) ) 

#define ISolimarLicenseSvr2_KeyTrialExpires(This,key_ident,expire_date)	\
    ( (This)->lpVtbl -> KeyTrialExpires(This,key_ident,expire_date) ) 

#define ISolimarLicenseSvr2_KeyTrialHours(This,key_ident,trial_hours)	\
    ( (This)->lpVtbl -> KeyTrialHours(This,key_ident,trial_hours) ) 

#define ISolimarLicenseSvr2_KeyIsActive(This,key_ident,key_active)	\
    ( (This)->lpVtbl -> KeyIsActive(This,key_ident,key_active) ) 

#define ISolimarLicenseSvr2_KeyIsProgrammed(This,key_ident,key_programmed)	\
    ( (This)->lpVtbl -> KeyIsProgrammed(This,key_ident,key_programmed) ) 

#define ISolimarLicenseSvr2_KeyIsPresent(This,key_ident,key_present)	\
    ( (This)->lpVtbl -> KeyIsPresent(This,key_ident,key_present) ) 

#define ISolimarLicenseSvr2_KeyHeaderQuery(This,key_ident,header_ident,value)	\
    ( (This)->lpVtbl -> KeyHeaderQuery(This,key_ident,header_ident,value) ) 

#define ISolimarLicenseSvr2_KeyLock(This,key_ident)	\
    ( (This)->lpVtbl -> KeyLock(This,key_ident) ) 

#define ISolimarLicenseSvr2_KeyUnlock(This,key_ident)	\
    ( (This)->lpVtbl -> KeyUnlock(This,key_ident) ) 

#define ISolimarLicenseSvr2_KeyObtain(This,key_ident)	\
    ( (This)->lpVtbl -> KeyObtain(This,key_ident) ) 

#define ISolimarLicenseSvr2_KeyRelease(This,key_ident)	\
    ( (This)->lpVtbl -> KeyRelease(This,key_ident) ) 

#define ISolimarLicenseSvr2_KeyValidateLicense(This,key_ident,license_valid)	\
    ( (This)->lpVtbl -> KeyValidateLicense(This,key_ident,license_valid) ) 

#define ISolimarLicenseSvr2_KeyModuleEnumerate(This,key_ident,key_module_list)	\
    ( (This)->lpVtbl -> KeyModuleEnumerate(This,key_ident,key_module_list) ) 

#define ISolimarLicenseSvr2_KeyModuleQuery(This,key_ident,module_identifier,vtValue)	\
    ( (This)->lpVtbl -> KeyModuleQuery(This,key_ident,module_identifier,vtValue) ) 

#define ISolimarLicenseSvr2_KeyModuleLicenseTotal(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseTotal(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr2_KeyModuleLicenseInUse(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseInUse(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr2_KeyModuleLicenseObtain(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseObtain(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr2_KeyModuleLicenseRelease(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseRelease(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr2_KeyModuleLicenseCounterDecrement(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseCounterDecrement(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr2_KeyFormat(This,key_ident,new_key_ident)	\
    ( (This)->lpVtbl -> KeyFormat(This,key_ident,new_key_ident) ) 

#define ISolimarLicenseSvr2_KeyProgram(This,key_ident,customer_number,key_number,product_ident,ver_major,ver_minor,key_type,days,module_value_list,new_key_ident)	\
    ( (This)->lpVtbl -> KeyProgram(This,key_ident,customer_number,key_number,product_ident,ver_major,ver_minor,key_type,days,module_value_list,new_key_ident) ) 

#define ISolimarLicenseSvr2_KeyReadRaw(This,key_ident,pvtKeyData)	\
    ( (This)->lpVtbl -> KeyReadRaw(This,key_ident,pvtKeyData) ) 


#define ISolimarLicenseSvr2_KeyModuleLicenseInUse2(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseInUse2(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr2_KeyModuleInUse(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleInUse(This,key_ident,module_ident,license_count) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISolimarLicenseSvr2_INTERFACE_DEFINED__ */


#ifndef __ISolimarLicenseSvr3_INTERFACE_DEFINED__
#define __ISolimarLicenseSvr3_INTERFACE_DEFINED__

/* interface ISolimarLicenseSvr3 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISolimarLicenseSvr3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BDA195D0-FC1A-4814-9DE0-364394D8D810")
    ISolimarLicenseSvr3 : public ISolimarLicenseSvr2
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenerateApplicationInstancePassword( 
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long license_count,
            /* [in] */ long password_number,
            /* [retval][out] */ BSTR *password) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyProgram2( 
            /* [in] */ BSTR key_ident,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long product_ident,
            /* [in] */ long ver_major,
            /* [in] */ long ver_minor,
            /* [in] */ long key_type,
            /* [in] */ long application_instances,
            /* [in] */ long days,
            /* [in] */ VARIANT module_value_list,
            /* [retval][out] */ BSTR *new_key_ident) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetVersionLicenseServer( 
            /* [out] */ long *p_ver_major,
            /* [out] */ long *p_ver_minor,
            /* [out] */ long *p_ver_build) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddApplicationInstance( 
            /* [in] */ BSTR key_ident,
            /* [in] */ BSTR application_instance,
            /* [in] */ VARIANT_BOOL b_app_instance_lock_key) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemoveApplicationInstance( 
            /* [in] */ BSTR key_ident,
            /* [in] */ BSTR application_instance) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetApplicationInstanceList( 
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT *pvtAppInstanceList) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyModuleLicenseUnlimited( 
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [in] */ VARIANT_BOOL b_module_is_unlimited) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ISolimarLicenseSvr3Vtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarLicenseSvr3 * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarLicenseSvr3 * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarLicenseSvr3 * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarLicenseSvr3 * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, Heartbeat)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Heartbeat )( 
            ISolimarLicenseSvr3 * This);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyEnumerate)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyEnumerate )( 
            ISolimarLicenseSvr3 * This,
            /* [retval][out] */ VARIANT *keylist);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, EnterPassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnterPassword )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, EnterPasswordPacket)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnterPasswordPacket )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ VARIANT vtPasswordPacket,
            /* [retval][out] */ BSTR *verification_code);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GenerateBasePassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateBasePassword )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GenerateVersionPassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateVersionPassword )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long ver_major,
            /* [in] */ long ver_minor,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GenerateExtensionPassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateExtensionPassword )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long extend_days,
            /* [in] */ long extension_num,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GenerateModulePassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateModulePassword )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long product_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GetLicenseServerTime)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetLicenseServerTime )( 
            ISolimarLicenseSvr3 * This,
            /* [retval][out] */ VARIANT *pvtSystemTime);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GenerateModulePassword2)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateModulePassword2 )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long product_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count,
            /* [in] */ long password_number,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, PasswordPacketInitialize)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PasswordPacketInitialize )( 
            ISolimarLicenseSvr3 * This);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, PasswordPacketAppendPassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PasswordPacketAppendPassword )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ VARIANT vtExpires,
            /* [in] */ BSTR password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, PasswordPacketFinalize)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PasswordPacketFinalize )( 
            ISolimarLicenseSvr3 * This);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, PasswordPacketGetPacket)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PasswordPacketGetPacket )( 
            ISolimarLicenseSvr3 * This,
            /* [retval][out] */ VARIANT *pvtPacketData);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, PasswordPacketGetVerificationCode)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PasswordPacketGetVerificationCode )( 
            ISolimarLicenseSvr3 * This,
            /* [retval][out] */ BSTR *verification_code);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyTrialExpires)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyTrialExpires )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT *expire_date);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyTrialHours)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyTrialHours )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ long *trial_hours);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyIsActive)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyIsActive )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT_BOOL *key_active);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyIsProgrammed)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyIsProgrammed )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT_BOOL *key_programmed);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyIsPresent)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyIsPresent )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT_BOOL *key_present);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyHeaderQuery)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyHeaderQuery )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long header_ident,
            /* [retval][out] */ VARIANT *value);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyLock)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyLock )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyUnlock)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyUnlock )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyObtain)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyObtain )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyRelease)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyRelease )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyValidateLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyValidateLicense )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT_BOOL *license_valid);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleEnumerate)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleEnumerate )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT *key_module_list);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleQuery)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleQuery )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_identifier,
            /* [retval][out] */ VARIANT *vtValue);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleLicenseTotal)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseTotal )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [retval][out] */ long *license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleLicenseInUse)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseInUse )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [retval][out] */ long *license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleLicenseObtain)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseObtain )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleLicenseRelease)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseRelease )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleLicenseCounterDecrement)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseCounterDecrement )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyFormat)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyFormat )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ BSTR *new_key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyProgram)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyProgram )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long product_ident,
            /* [in] */ long ver_major,
            /* [in] */ long ver_minor,
            /* [in] */ long key_type,
            /* [in] */ long days,
            /* [in] */ VARIANT module_value_list,
            /* [retval][out] */ BSTR *new_key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyReadRaw)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyReadRaw )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT *pvtKeyData);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr2, KeyModuleLicenseInUse2)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseInUse2 )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [retval][out] */ long *license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr2, KeyModuleInUse)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleInUse )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [retval][out] */ long *license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr3, GenerateApplicationInstancePassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateApplicationInstancePassword )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long license_count,
            /* [in] */ long password_number,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr3, KeyProgram2)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyProgram2 )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long product_ident,
            /* [in] */ long ver_major,
            /* [in] */ long ver_minor,
            /* [in] */ long key_type,
            /* [in] */ long application_instances,
            /* [in] */ long days,
            /* [in] */ VARIANT module_value_list,
            /* [retval][out] */ BSTR *new_key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr3, GetVersionLicenseServer)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVersionLicenseServer )( 
            ISolimarLicenseSvr3 * This,
            /* [out] */ long *p_ver_major,
            /* [out] */ long *p_ver_minor,
            /* [out] */ long *p_ver_build);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr3, AddApplicationInstance)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddApplicationInstance )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ BSTR application_instance,
            /* [in] */ VARIANT_BOOL b_app_instance_lock_key);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr3, RemoveApplicationInstance)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RemoveApplicationInstance )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ BSTR application_instance);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr3, GetApplicationInstanceList)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetApplicationInstanceList )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT *pvtAppInstanceList);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr3, KeyModuleLicenseUnlimited)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseUnlimited )( 
            ISolimarLicenseSvr3 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [in] */ VARIANT_BOOL b_module_is_unlimited);
        
        END_INTERFACE
    } ISolimarLicenseSvr3Vtbl;

    interface ISolimarLicenseSvr3
    {
        CONST_VTBL struct ISolimarLicenseSvr3Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISolimarLicenseSvr3_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISolimarLicenseSvr3_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISolimarLicenseSvr3_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISolimarLicenseSvr3_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISolimarLicenseSvr3_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISolimarLicenseSvr3_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISolimarLicenseSvr3_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISolimarLicenseSvr3_Heartbeat(This)	\
    ( (This)->lpVtbl -> Heartbeat(This) ) 

#define ISolimarLicenseSvr3_KeyEnumerate(This,keylist)	\
    ( (This)->lpVtbl -> KeyEnumerate(This,keylist) ) 

#define ISolimarLicenseSvr3_EnterPassword(This,password)	\
    ( (This)->lpVtbl -> EnterPassword(This,password) ) 

#define ISolimarLicenseSvr3_EnterPasswordPacket(This,vtPasswordPacket,verification_code)	\
    ( (This)->lpVtbl -> EnterPasswordPacket(This,vtPasswordPacket,verification_code) ) 

#define ISolimarLicenseSvr3_GenerateBasePassword(This,customer_number,key_number,password)	\
    ( (This)->lpVtbl -> GenerateBasePassword(This,customer_number,key_number,password) ) 

#define ISolimarLicenseSvr3_GenerateVersionPassword(This,customer_number,key_number,ver_major,ver_minor,password)	\
    ( (This)->lpVtbl -> GenerateVersionPassword(This,customer_number,key_number,ver_major,ver_minor,password) ) 

#define ISolimarLicenseSvr3_GenerateExtensionPassword(This,customer_number,key_number,extend_days,extension_num,password)	\
    ( (This)->lpVtbl -> GenerateExtensionPassword(This,customer_number,key_number,extend_days,extension_num,password) ) 

#define ISolimarLicenseSvr3_GenerateModulePassword(This,customer_number,key_number,product_ident,module_ident,license_count,password)	\
    ( (This)->lpVtbl -> GenerateModulePassword(This,customer_number,key_number,product_ident,module_ident,license_count,password) ) 

#define ISolimarLicenseSvr3_GetLicenseServerTime(This,pvtSystemTime)	\
    ( (This)->lpVtbl -> GetLicenseServerTime(This,pvtSystemTime) ) 

#define ISolimarLicenseSvr3_GenerateModulePassword2(This,customer_number,key_number,product_ident,module_ident,license_count,password_number,password)	\
    ( (This)->lpVtbl -> GenerateModulePassword2(This,customer_number,key_number,product_ident,module_ident,license_count,password_number,password) ) 

#define ISolimarLicenseSvr3_PasswordPacketInitialize(This)	\
    ( (This)->lpVtbl -> PasswordPacketInitialize(This) ) 

#define ISolimarLicenseSvr3_PasswordPacketAppendPassword(This,vtExpires,password)	\
    ( (This)->lpVtbl -> PasswordPacketAppendPassword(This,vtExpires,password) ) 

#define ISolimarLicenseSvr3_PasswordPacketFinalize(This)	\
    ( (This)->lpVtbl -> PasswordPacketFinalize(This) ) 

#define ISolimarLicenseSvr3_PasswordPacketGetPacket(This,pvtPacketData)	\
    ( (This)->lpVtbl -> PasswordPacketGetPacket(This,pvtPacketData) ) 

#define ISolimarLicenseSvr3_PasswordPacketGetVerificationCode(This,verification_code)	\
    ( (This)->lpVtbl -> PasswordPacketGetVerificationCode(This,verification_code) ) 

#define ISolimarLicenseSvr3_KeyTrialExpires(This,key_ident,expire_date)	\
    ( (This)->lpVtbl -> KeyTrialExpires(This,key_ident,expire_date) ) 

#define ISolimarLicenseSvr3_KeyTrialHours(This,key_ident,trial_hours)	\
    ( (This)->lpVtbl -> KeyTrialHours(This,key_ident,trial_hours) ) 

#define ISolimarLicenseSvr3_KeyIsActive(This,key_ident,key_active)	\
    ( (This)->lpVtbl -> KeyIsActive(This,key_ident,key_active) ) 

#define ISolimarLicenseSvr3_KeyIsProgrammed(This,key_ident,key_programmed)	\
    ( (This)->lpVtbl -> KeyIsProgrammed(This,key_ident,key_programmed) ) 

#define ISolimarLicenseSvr3_KeyIsPresent(This,key_ident,key_present)	\
    ( (This)->lpVtbl -> KeyIsPresent(This,key_ident,key_present) ) 

#define ISolimarLicenseSvr3_KeyHeaderQuery(This,key_ident,header_ident,value)	\
    ( (This)->lpVtbl -> KeyHeaderQuery(This,key_ident,header_ident,value) ) 

#define ISolimarLicenseSvr3_KeyLock(This,key_ident)	\
    ( (This)->lpVtbl -> KeyLock(This,key_ident) ) 

#define ISolimarLicenseSvr3_KeyUnlock(This,key_ident)	\
    ( (This)->lpVtbl -> KeyUnlock(This,key_ident) ) 

#define ISolimarLicenseSvr3_KeyObtain(This,key_ident)	\
    ( (This)->lpVtbl -> KeyObtain(This,key_ident) ) 

#define ISolimarLicenseSvr3_KeyRelease(This,key_ident)	\
    ( (This)->lpVtbl -> KeyRelease(This,key_ident) ) 

#define ISolimarLicenseSvr3_KeyValidateLicense(This,key_ident,license_valid)	\
    ( (This)->lpVtbl -> KeyValidateLicense(This,key_ident,license_valid) ) 

#define ISolimarLicenseSvr3_KeyModuleEnumerate(This,key_ident,key_module_list)	\
    ( (This)->lpVtbl -> KeyModuleEnumerate(This,key_ident,key_module_list) ) 

#define ISolimarLicenseSvr3_KeyModuleQuery(This,key_ident,module_identifier,vtValue)	\
    ( (This)->lpVtbl -> KeyModuleQuery(This,key_ident,module_identifier,vtValue) ) 

#define ISolimarLicenseSvr3_KeyModuleLicenseTotal(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseTotal(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr3_KeyModuleLicenseInUse(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseInUse(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr3_KeyModuleLicenseObtain(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseObtain(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr3_KeyModuleLicenseRelease(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseRelease(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr3_KeyModuleLicenseCounterDecrement(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseCounterDecrement(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr3_KeyFormat(This,key_ident,new_key_ident)	\
    ( (This)->lpVtbl -> KeyFormat(This,key_ident,new_key_ident) ) 

#define ISolimarLicenseSvr3_KeyProgram(This,key_ident,customer_number,key_number,product_ident,ver_major,ver_minor,key_type,days,module_value_list,new_key_ident)	\
    ( (This)->lpVtbl -> KeyProgram(This,key_ident,customer_number,key_number,product_ident,ver_major,ver_minor,key_type,days,module_value_list,new_key_ident) ) 

#define ISolimarLicenseSvr3_KeyReadRaw(This,key_ident,pvtKeyData)	\
    ( (This)->lpVtbl -> KeyReadRaw(This,key_ident,pvtKeyData) ) 


#define ISolimarLicenseSvr3_KeyModuleLicenseInUse2(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseInUse2(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr3_KeyModuleInUse(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleInUse(This,key_ident,module_ident,license_count) ) 


#define ISolimarLicenseSvr3_GenerateApplicationInstancePassword(This,customer_number,key_number,license_count,password_number,password)	\
    ( (This)->lpVtbl -> GenerateApplicationInstancePassword(This,customer_number,key_number,license_count,password_number,password) ) 

#define ISolimarLicenseSvr3_KeyProgram2(This,key_ident,customer_number,key_number,product_ident,ver_major,ver_minor,key_type,application_instances,days,module_value_list,new_key_ident)	\
    ( (This)->lpVtbl -> KeyProgram2(This,key_ident,customer_number,key_number,product_ident,ver_major,ver_minor,key_type,application_instances,days,module_value_list,new_key_ident) ) 

#define ISolimarLicenseSvr3_GetVersionLicenseServer(This,p_ver_major,p_ver_minor,p_ver_build)	\
    ( (This)->lpVtbl -> GetVersionLicenseServer(This,p_ver_major,p_ver_minor,p_ver_build) ) 

#define ISolimarLicenseSvr3_AddApplicationInstance(This,key_ident,application_instance,b_app_instance_lock_key)	\
    ( (This)->lpVtbl -> AddApplicationInstance(This,key_ident,application_instance,b_app_instance_lock_key) ) 

#define ISolimarLicenseSvr3_RemoveApplicationInstance(This,key_ident,application_instance)	\
    ( (This)->lpVtbl -> RemoveApplicationInstance(This,key_ident,application_instance) ) 

#define ISolimarLicenseSvr3_GetApplicationInstanceList(This,key_ident,pvtAppInstanceList)	\
    ( (This)->lpVtbl -> GetApplicationInstanceList(This,key_ident,pvtAppInstanceList) ) 

#define ISolimarLicenseSvr3_KeyModuleLicenseUnlimited(This,key_ident,module_ident,b_module_is_unlimited)	\
    ( (This)->lpVtbl -> KeyModuleLicenseUnlimited(This,key_ident,module_ident,b_module_is_unlimited) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISolimarLicenseSvr3_INTERFACE_DEFINED__ */


#ifndef __ISolimarLicenseSvr4_INTERFACE_DEFINED__
#define __ISolimarLicenseSvr4_INTERFACE_DEFINED__

/* interface ISolimarLicenseSvr4 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISolimarLicenseSvr4;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("70C717AC-46ED-44d8-B0EF-AFCCC63DC98E")
    ISolimarLicenseSvr4 : public ISolimarLicenseSvr3
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyModuleLicenseInUse_ByApp( 
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [retval][out] */ long *license_count) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ISolimarLicenseSvr4Vtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarLicenseSvr4 * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarLicenseSvr4 * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarLicenseSvr4 * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarLicenseSvr4 * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, Heartbeat)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Heartbeat )( 
            ISolimarLicenseSvr4 * This);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyEnumerate)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyEnumerate )( 
            ISolimarLicenseSvr4 * This,
            /* [retval][out] */ VARIANT *keylist);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, EnterPassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnterPassword )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, EnterPasswordPacket)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnterPasswordPacket )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ VARIANT vtPasswordPacket,
            /* [retval][out] */ BSTR *verification_code);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GenerateBasePassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateBasePassword )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GenerateVersionPassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateVersionPassword )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long ver_major,
            /* [in] */ long ver_minor,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GenerateExtensionPassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateExtensionPassword )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long extend_days,
            /* [in] */ long extension_num,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GenerateModulePassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateModulePassword )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long product_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GetLicenseServerTime)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetLicenseServerTime )( 
            ISolimarLicenseSvr4 * This,
            /* [retval][out] */ VARIANT *pvtSystemTime);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, GenerateModulePassword2)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateModulePassword2 )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long product_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count,
            /* [in] */ long password_number,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, PasswordPacketInitialize)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PasswordPacketInitialize )( 
            ISolimarLicenseSvr4 * This);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, PasswordPacketAppendPassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PasswordPacketAppendPassword )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ VARIANT vtExpires,
            /* [in] */ BSTR password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, PasswordPacketFinalize)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PasswordPacketFinalize )( 
            ISolimarLicenseSvr4 * This);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, PasswordPacketGetPacket)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PasswordPacketGetPacket )( 
            ISolimarLicenseSvr4 * This,
            /* [retval][out] */ VARIANT *pvtPacketData);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, PasswordPacketGetVerificationCode)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PasswordPacketGetVerificationCode )( 
            ISolimarLicenseSvr4 * This,
            /* [retval][out] */ BSTR *verification_code);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyTrialExpires)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyTrialExpires )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT *expire_date);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyTrialHours)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyTrialHours )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ long *trial_hours);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyIsActive)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyIsActive )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT_BOOL *key_active);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyIsProgrammed)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyIsProgrammed )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT_BOOL *key_programmed);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyIsPresent)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyIsPresent )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT_BOOL *key_present);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyHeaderQuery)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyHeaderQuery )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long header_ident,
            /* [retval][out] */ VARIANT *value);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyLock)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyLock )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyUnlock)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyUnlock )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyObtain)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyObtain )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyRelease)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyRelease )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyValidateLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyValidateLicense )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT_BOOL *license_valid);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleEnumerate)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleEnumerate )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT *key_module_list);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleQuery)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleQuery )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_identifier,
            /* [retval][out] */ VARIANT *vtValue);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleLicenseTotal)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseTotal )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [retval][out] */ long *license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleLicenseInUse)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseInUse )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [retval][out] */ long *license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleLicenseObtain)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseObtain )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleLicenseRelease)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseRelease )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyModuleLicenseCounterDecrement)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseCounterDecrement )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [in] */ long license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyFormat)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyFormat )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ BSTR *new_key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyProgram)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyProgram )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long product_ident,
            /* [in] */ long ver_major,
            /* [in] */ long ver_minor,
            /* [in] */ long key_type,
            /* [in] */ long days,
            /* [in] */ VARIANT module_value_list,
            /* [retval][out] */ BSTR *new_key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr, KeyReadRaw)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyReadRaw )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT *pvtKeyData);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr2, KeyModuleLicenseInUse2)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseInUse2 )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [retval][out] */ long *license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr2, KeyModuleInUse)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleInUse )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [retval][out] */ long *license_count);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr3, GenerateApplicationInstancePassword)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateApplicationInstancePassword )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long license_count,
            /* [in] */ long password_number,
            /* [retval][out] */ BSTR *password);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr3, KeyProgram2)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyProgram2 )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long customer_number,
            /* [in] */ long key_number,
            /* [in] */ long product_ident,
            /* [in] */ long ver_major,
            /* [in] */ long ver_minor,
            /* [in] */ long key_type,
            /* [in] */ long application_instances,
            /* [in] */ long days,
            /* [in] */ VARIANT module_value_list,
            /* [retval][out] */ BSTR *new_key_ident);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr3, GetVersionLicenseServer)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVersionLicenseServer )( 
            ISolimarLicenseSvr4 * This,
            /* [out] */ long *p_ver_major,
            /* [out] */ long *p_ver_minor,
            /* [out] */ long *p_ver_build);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr3, AddApplicationInstance)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddApplicationInstance )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ BSTR application_instance,
            /* [in] */ VARIANT_BOOL b_app_instance_lock_key);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr3, RemoveApplicationInstance)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RemoveApplicationInstance )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ BSTR application_instance);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr3, GetApplicationInstanceList)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetApplicationInstanceList )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [retval][out] */ VARIANT *pvtAppInstanceList);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr3, KeyModuleLicenseUnlimited)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseUnlimited )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [in] */ VARIANT_BOOL b_module_is_unlimited);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseSvr4, KeyModuleLicenseInUse_ByApp)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyModuleLicenseInUse_ByApp )( 
            ISolimarLicenseSvr4 * This,
            /* [in] */ BSTR key_ident,
            /* [in] */ long module_ident,
            /* [retval][out] */ long *license_count);
        
        END_INTERFACE
    } ISolimarLicenseSvr4Vtbl;

    interface ISolimarLicenseSvr4
    {
        CONST_VTBL struct ISolimarLicenseSvr4Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISolimarLicenseSvr4_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISolimarLicenseSvr4_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISolimarLicenseSvr4_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISolimarLicenseSvr4_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISolimarLicenseSvr4_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISolimarLicenseSvr4_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISolimarLicenseSvr4_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISolimarLicenseSvr4_Heartbeat(This)	\
    ( (This)->lpVtbl -> Heartbeat(This) ) 

#define ISolimarLicenseSvr4_KeyEnumerate(This,keylist)	\
    ( (This)->lpVtbl -> KeyEnumerate(This,keylist) ) 

#define ISolimarLicenseSvr4_EnterPassword(This,password)	\
    ( (This)->lpVtbl -> EnterPassword(This,password) ) 

#define ISolimarLicenseSvr4_EnterPasswordPacket(This,vtPasswordPacket,verification_code)	\
    ( (This)->lpVtbl -> EnterPasswordPacket(This,vtPasswordPacket,verification_code) ) 

#define ISolimarLicenseSvr4_GenerateBasePassword(This,customer_number,key_number,password)	\
    ( (This)->lpVtbl -> GenerateBasePassword(This,customer_number,key_number,password) ) 

#define ISolimarLicenseSvr4_GenerateVersionPassword(This,customer_number,key_number,ver_major,ver_minor,password)	\
    ( (This)->lpVtbl -> GenerateVersionPassword(This,customer_number,key_number,ver_major,ver_minor,password) ) 

#define ISolimarLicenseSvr4_GenerateExtensionPassword(This,customer_number,key_number,extend_days,extension_num,password)	\
    ( (This)->lpVtbl -> GenerateExtensionPassword(This,customer_number,key_number,extend_days,extension_num,password) ) 

#define ISolimarLicenseSvr4_GenerateModulePassword(This,customer_number,key_number,product_ident,module_ident,license_count,password)	\
    ( (This)->lpVtbl -> GenerateModulePassword(This,customer_number,key_number,product_ident,module_ident,license_count,password) ) 

#define ISolimarLicenseSvr4_GetLicenseServerTime(This,pvtSystemTime)	\
    ( (This)->lpVtbl -> GetLicenseServerTime(This,pvtSystemTime) ) 

#define ISolimarLicenseSvr4_GenerateModulePassword2(This,customer_number,key_number,product_ident,module_ident,license_count,password_number,password)	\
    ( (This)->lpVtbl -> GenerateModulePassword2(This,customer_number,key_number,product_ident,module_ident,license_count,password_number,password) ) 

#define ISolimarLicenseSvr4_PasswordPacketInitialize(This)	\
    ( (This)->lpVtbl -> PasswordPacketInitialize(This) ) 

#define ISolimarLicenseSvr4_PasswordPacketAppendPassword(This,vtExpires,password)	\
    ( (This)->lpVtbl -> PasswordPacketAppendPassword(This,vtExpires,password) ) 

#define ISolimarLicenseSvr4_PasswordPacketFinalize(This)	\
    ( (This)->lpVtbl -> PasswordPacketFinalize(This) ) 

#define ISolimarLicenseSvr4_PasswordPacketGetPacket(This,pvtPacketData)	\
    ( (This)->lpVtbl -> PasswordPacketGetPacket(This,pvtPacketData) ) 

#define ISolimarLicenseSvr4_PasswordPacketGetVerificationCode(This,verification_code)	\
    ( (This)->lpVtbl -> PasswordPacketGetVerificationCode(This,verification_code) ) 

#define ISolimarLicenseSvr4_KeyTrialExpires(This,key_ident,expire_date)	\
    ( (This)->lpVtbl -> KeyTrialExpires(This,key_ident,expire_date) ) 

#define ISolimarLicenseSvr4_KeyTrialHours(This,key_ident,trial_hours)	\
    ( (This)->lpVtbl -> KeyTrialHours(This,key_ident,trial_hours) ) 

#define ISolimarLicenseSvr4_KeyIsActive(This,key_ident,key_active)	\
    ( (This)->lpVtbl -> KeyIsActive(This,key_ident,key_active) ) 

#define ISolimarLicenseSvr4_KeyIsProgrammed(This,key_ident,key_programmed)	\
    ( (This)->lpVtbl -> KeyIsProgrammed(This,key_ident,key_programmed) ) 

#define ISolimarLicenseSvr4_KeyIsPresent(This,key_ident,key_present)	\
    ( (This)->lpVtbl -> KeyIsPresent(This,key_ident,key_present) ) 

#define ISolimarLicenseSvr4_KeyHeaderQuery(This,key_ident,header_ident,value)	\
    ( (This)->lpVtbl -> KeyHeaderQuery(This,key_ident,header_ident,value) ) 

#define ISolimarLicenseSvr4_KeyLock(This,key_ident)	\
    ( (This)->lpVtbl -> KeyLock(This,key_ident) ) 

#define ISolimarLicenseSvr4_KeyUnlock(This,key_ident)	\
    ( (This)->lpVtbl -> KeyUnlock(This,key_ident) ) 

#define ISolimarLicenseSvr4_KeyObtain(This,key_ident)	\
    ( (This)->lpVtbl -> KeyObtain(This,key_ident) ) 

#define ISolimarLicenseSvr4_KeyRelease(This,key_ident)	\
    ( (This)->lpVtbl -> KeyRelease(This,key_ident) ) 

#define ISolimarLicenseSvr4_KeyValidateLicense(This,key_ident,license_valid)	\
    ( (This)->lpVtbl -> KeyValidateLicense(This,key_ident,license_valid) ) 

#define ISolimarLicenseSvr4_KeyModuleEnumerate(This,key_ident,key_module_list)	\
    ( (This)->lpVtbl -> KeyModuleEnumerate(This,key_ident,key_module_list) ) 

#define ISolimarLicenseSvr4_KeyModuleQuery(This,key_ident,module_identifier,vtValue)	\
    ( (This)->lpVtbl -> KeyModuleQuery(This,key_ident,module_identifier,vtValue) ) 

#define ISolimarLicenseSvr4_KeyModuleLicenseTotal(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseTotal(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr4_KeyModuleLicenseInUse(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseInUse(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr4_KeyModuleLicenseObtain(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseObtain(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr4_KeyModuleLicenseRelease(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseRelease(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr4_KeyModuleLicenseCounterDecrement(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseCounterDecrement(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr4_KeyFormat(This,key_ident,new_key_ident)	\
    ( (This)->lpVtbl -> KeyFormat(This,key_ident,new_key_ident) ) 

#define ISolimarLicenseSvr4_KeyProgram(This,key_ident,customer_number,key_number,product_ident,ver_major,ver_minor,key_type,days,module_value_list,new_key_ident)	\
    ( (This)->lpVtbl -> KeyProgram(This,key_ident,customer_number,key_number,product_ident,ver_major,ver_minor,key_type,days,module_value_list,new_key_ident) ) 

#define ISolimarLicenseSvr4_KeyReadRaw(This,key_ident,pvtKeyData)	\
    ( (This)->lpVtbl -> KeyReadRaw(This,key_ident,pvtKeyData) ) 


#define ISolimarLicenseSvr4_KeyModuleLicenseInUse2(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseInUse2(This,key_ident,module_ident,license_count) ) 

#define ISolimarLicenseSvr4_KeyModuleInUse(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleInUse(This,key_ident,module_ident,license_count) ) 


#define ISolimarLicenseSvr4_GenerateApplicationInstancePassword(This,customer_number,key_number,license_count,password_number,password)	\
    ( (This)->lpVtbl -> GenerateApplicationInstancePassword(This,customer_number,key_number,license_count,password_number,password) ) 

#define ISolimarLicenseSvr4_KeyProgram2(This,key_ident,customer_number,key_number,product_ident,ver_major,ver_minor,key_type,application_instances,days,module_value_list,new_key_ident)	\
    ( (This)->lpVtbl -> KeyProgram2(This,key_ident,customer_number,key_number,product_ident,ver_major,ver_minor,key_type,application_instances,days,module_value_list,new_key_ident) ) 

#define ISolimarLicenseSvr4_GetVersionLicenseServer(This,p_ver_major,p_ver_minor,p_ver_build)	\
    ( (This)->lpVtbl -> GetVersionLicenseServer(This,p_ver_major,p_ver_minor,p_ver_build) ) 

#define ISolimarLicenseSvr4_AddApplicationInstance(This,key_ident,application_instance,b_app_instance_lock_key)	\
    ( (This)->lpVtbl -> AddApplicationInstance(This,key_ident,application_instance,b_app_instance_lock_key) ) 

#define ISolimarLicenseSvr4_RemoveApplicationInstance(This,key_ident,application_instance)	\
    ( (This)->lpVtbl -> RemoveApplicationInstance(This,key_ident,application_instance) ) 

#define ISolimarLicenseSvr4_GetApplicationInstanceList(This,key_ident,pvtAppInstanceList)	\
    ( (This)->lpVtbl -> GetApplicationInstanceList(This,key_ident,pvtAppInstanceList) ) 

#define ISolimarLicenseSvr4_KeyModuleLicenseUnlimited(This,key_ident,module_ident,b_module_is_unlimited)	\
    ( (This)->lpVtbl -> KeyModuleLicenseUnlimited(This,key_ident,module_ident,b_module_is_unlimited) ) 


#define ISolimarLicenseSvr4_KeyModuleLicenseInUse_ByApp(This,key_ident,module_ident,license_count)	\
    ( (This)->lpVtbl -> KeyModuleLicenseInUse_ByApp(This,key_ident,module_ident,license_count) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISolimarLicenseSvr4_INTERFACE_DEFINED__ */


#ifndef __ISolimarSoftwareLicenseSvr_INTERFACE_DEFINED__
#define __ISolimarSoftwareLicenseSvr_INTERFACE_DEFINED__

/* interface ISolimarSoftwareLicenseSvr */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISolimarSoftwareLicenseSvr;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7FFD4EEB-DC7A-43f0-9117-667D77EF93A2")
    ISolimarSoftwareLicenseSvr : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SoftwareModuleLicenseTotalForAll_ByProduct( 
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [retval][out] */ long *pLicenseCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SoftwareModuleLicenseInUseForAll_ByProduct( 
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [retval][out] */ long *pLicenseCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SoftwareValidateLicenseApp_ByProduct( 
            /* [in] */ long productID,
            /* [retval][out] */ VARIANT_BOOL *pbLicenseValid) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SoftwareModuleLicenseInUseByApp_ByProduct( 
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [retval][out] */ long *pLicenseCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SoftwareModuleLicenseInUseByConnection_ByProduct( 
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [retval][out] */ long *pLicenseCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SoftwareModuleLicenseObtainByApp_ByProduct( 
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [in] */ long licenseCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SoftwareModuleLicenseReleaseByApp_ByProduct( 
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [in] */ long licenseCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SoftwareModuleLicenseCounterDecrementByApp_ByProduct( 
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [in] */ long licenseCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSoftwareLicenseInfoByProduct_ForAll( 
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrProductInfoAttribsStream) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSoftwareLicenseInfo_ForAll( 
            /* [retval][out] */ BSTR *pBstrLicenseInfoAttribsStream) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAllSoftwareLicenses( 
            /* [retval][out] */ BSTR *pBstrListAllLicensesStream) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSoftwareLicenseInfoByProduct_ByLicense( 
            /* [in] */ BSTR softwareLicense,
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrProductInfoAttribsStream) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSoftwareLicenseInfo_ByLicense( 
            /* [in] */ BSTR softwareLicense,
            /* [retval][out] */ BSTR *pBstrLicenseInfoAttribsStream) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSoftwareLicenseStatus_ByProduct( 
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrStringToDwordMap) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSoftwareLicenseStatus_ByLicense( 
            /* [in] */ BSTR softwareLicense) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSoftwareSpecByProduct( 
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrProductSoftwareSpecAttribs) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSoftwareSpec( 
            /* [retval][out] */ BSTR *pBstrSoftwareSpecAttribsStream) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SoftwareAddApplicationInstanceByProduct( 
            /* [in] */ long productID,
            /* [in] */ BSTR applicationInstance) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SoftwareRemoveApplicationInstanceByProduct( 
            /* [in] */ long productID,
            /* [in] */ BSTR applicationInstance) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SoftwareGetApplicationInstanceListByProduct( 
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrListAppInstStream) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenerateSoftwareLicPacket( 
            /* [in] */ BSTR bstrLicPackageAttribsStream,
            /* [in] */ VARIANT vtExpires,
            /* [out] */ BSTR *pBstrVerificationCode,
            /* [retval][out] */ VARIANT *pVtLicensePacket) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnterSoftwareLicPacket( 
            /* [in] */ VARIANT vtLicensePacket,
            /* [retval][out] */ BSTR *pBstrVerificationCode) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ValidateToken_ByLicense( 
            /* [in] */ BSTR softwareLicense,
            /* [in] */ long validationTokenType,
            /* [in] */ BSTR validationValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber( 
            /* [in] */ BSTR softwareLicense,
            /* [in] */ BSTR contractNumber) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenerateVerifyDataWithVerCode_ByLicense( 
            /* [in] */ BSTR softwareLicense,
            /* [retval][out] */ VARIANT *pVtLicensePacket) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenerateVerifyDataWithLicInfo_ByLicense( 
            /* [in] */ BSTR softwareLicense,
            /* [retval][out] */ VARIANT *pVtLicensePacket) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenerateSoftwareLicArchive_ByLicense( 
            /* [in] */ BSTR softwareLicense,
            /* [retval][out] */ VARIANT *pVtLicenseArchive) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenerateLicPackage_ByVerifyData( 
            /* [in] */ VARIANT vtLicensePacket,
            /* [retval][out] */ BSTR *pBstrLicensePackageAttribsStream) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenerateLicPackage_BySoftwareLicArchive( 
            /* [in] */ VARIANT vtLicenseArchive,
            /* [retval][out] */ BSTR *pBstrLicensePackageAttribsStream) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenerateLicPackage_BySoftwareLicPacket( 
            /* [in] */ VARIANT vtLicensePacket,
            /* [retval][out] */ BSTR *pBstrLicensePackageAttribsStream) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnterSoftwareLicArchive( 
            /* [in] */ VARIANT vtLicenseArchive) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenerateLicenseSystemData( 
            /* [retval][out] */ VARIANT *pVtLicSysDataPacket) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenerateStream_ByLicenseSystemData( 
            /* [in] */ VARIANT vtLicSysDataPacket,
            /* [retval][out] */ BSTR *pBstrLicSysDataAttribsStream) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenerateStreamData_ByLicenseSystemData( 
            /* [in] */ VARIANT vtLicSysDataPacket,
            /* [out] */ BSTR *pBstrCreatedDateStreamed,
            /* [out] */ BSTR *pBstrKeyAttribsListStream,
            /* [out] */ BSTR *pBstrLicUsageDataAttribsStream,
            /* [out] */ BSTR *pBstrConnectionAttribsListStream,
            /* [out] */ BSTR *pBstrEventLogAttribsListStream,
            /* [retval][out] */ BSTR *pBstrLicInfoDataAttribsListStream) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetEventLogList_ForLicenseServer( 
            /* [retval][out] */ BSTR *pBstrEventLogAttribsListStream) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ISolimarSoftwareLicenseSvrVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarSoftwareLicenseSvr * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarSoftwareLicenseSvr * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareModuleLicenseTotalForAll_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareModuleLicenseTotalForAll_ByProduct )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [retval][out] */ long *pLicenseCount);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareModuleLicenseInUseForAll_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareModuleLicenseInUseForAll_ByProduct )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [retval][out] */ long *pLicenseCount);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareValidateLicenseApp_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareValidateLicenseApp_ByProduct )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ long productID,
            /* [retval][out] */ VARIANT_BOOL *pbLicenseValid);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareModuleLicenseInUseByApp_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareModuleLicenseInUseByApp_ByProduct )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [retval][out] */ long *pLicenseCount);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareModuleLicenseInUseByConnection_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareModuleLicenseInUseByConnection_ByProduct )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [retval][out] */ long *pLicenseCount);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareModuleLicenseObtainByApp_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareModuleLicenseObtainByApp_ByProduct )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [in] */ long licenseCount);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareModuleLicenseReleaseByApp_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareModuleLicenseReleaseByApp_ByProduct )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [in] */ long licenseCount);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareModuleLicenseCounterDecrementByApp_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareModuleLicenseCounterDecrementByApp_ByProduct )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [in] */ long licenseCount);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareLicenseInfoByProduct_ForAll)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareLicenseInfoByProduct_ForAll )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrProductInfoAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareLicenseInfo_ForAll)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareLicenseInfo_ForAll )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [retval][out] */ BSTR *pBstrLicenseInfoAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetAllSoftwareLicenses)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAllSoftwareLicenses )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [retval][out] */ BSTR *pBstrListAllLicensesStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareLicenseInfoByProduct_ByLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareLicenseInfoByProduct_ByLicense )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ BSTR softwareLicense,
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrProductInfoAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareLicenseInfo_ByLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareLicenseInfo_ByLicense )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ BSTR softwareLicense,
            /* [retval][out] */ BSTR *pBstrLicenseInfoAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareLicenseStatus_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareLicenseStatus_ByProduct )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrStringToDwordMap);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareLicenseStatus_ByLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareLicenseStatus_ByLicense )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ BSTR softwareLicense);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareSpecByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareSpecByProduct )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrProductSoftwareSpecAttribs);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareSpec)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareSpec )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [retval][out] */ BSTR *pBstrSoftwareSpecAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareAddApplicationInstanceByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareAddApplicationInstanceByProduct )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ long productID,
            /* [in] */ BSTR applicationInstance);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareRemoveApplicationInstanceByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareRemoveApplicationInstanceByProduct )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ long productID,
            /* [in] */ BSTR applicationInstance);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareGetApplicationInstanceListByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareGetApplicationInstanceListByProduct )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrListAppInstStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateSoftwareLicPacket)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateSoftwareLicPacket )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ BSTR bstrLicPackageAttribsStream,
            /* [in] */ VARIANT vtExpires,
            /* [out] */ BSTR *pBstrVerificationCode,
            /* [retval][out] */ VARIANT *pVtLicensePacket);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, EnterSoftwareLicPacket)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnterSoftwareLicPacket )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ VARIANT vtLicensePacket,
            /* [retval][out] */ BSTR *pBstrVerificationCode);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, ValidateToken_ByLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ValidateToken_ByLicense )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ BSTR softwareLicense,
            /* [in] */ long validationTokenType,
            /* [in] */ BSTR validationValue);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ BSTR softwareLicense,
            /* [in] */ BSTR contractNumber);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateVerifyDataWithVerCode_ByLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateVerifyDataWithVerCode_ByLicense )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ BSTR softwareLicense,
            /* [retval][out] */ VARIANT *pVtLicensePacket);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateVerifyDataWithLicInfo_ByLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateVerifyDataWithLicInfo_ByLicense )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ BSTR softwareLicense,
            /* [retval][out] */ VARIANT *pVtLicensePacket);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateSoftwareLicArchive_ByLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateSoftwareLicArchive_ByLicense )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ BSTR softwareLicense,
            /* [retval][out] */ VARIANT *pVtLicenseArchive);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateLicPackage_ByVerifyData)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateLicPackage_ByVerifyData )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ VARIANT vtLicensePacket,
            /* [retval][out] */ BSTR *pBstrLicensePackageAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateLicPackage_BySoftwareLicArchive)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateLicPackage_BySoftwareLicArchive )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ VARIANT vtLicenseArchive,
            /* [retval][out] */ BSTR *pBstrLicensePackageAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateLicPackage_BySoftwareLicPacket)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateLicPackage_BySoftwareLicPacket )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ VARIANT vtLicensePacket,
            /* [retval][out] */ BSTR *pBstrLicensePackageAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, EnterSoftwareLicArchive)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnterSoftwareLicArchive )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ VARIANT vtLicenseArchive);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateLicenseSystemData)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateLicenseSystemData )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [retval][out] */ VARIANT *pVtLicSysDataPacket);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateStream_ByLicenseSystemData)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateStream_ByLicenseSystemData )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ VARIANT vtLicSysDataPacket,
            /* [retval][out] */ BSTR *pBstrLicSysDataAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateStreamData_ByLicenseSystemData)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateStreamData_ByLicenseSystemData )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [in] */ VARIANT vtLicSysDataPacket,
            /* [out] */ BSTR *pBstrCreatedDateStreamed,
            /* [out] */ BSTR *pBstrKeyAttribsListStream,
            /* [out] */ BSTR *pBstrLicUsageDataAttribsStream,
            /* [out] */ BSTR *pBstrConnectionAttribsListStream,
            /* [out] */ BSTR *pBstrEventLogAttribsListStream,
            /* [retval][out] */ BSTR *pBstrLicInfoDataAttribsListStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetEventLogList_ForLicenseServer)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetEventLogList_ForLicenseServer )( 
            ISolimarSoftwareLicenseSvr * This,
            /* [retval][out] */ BSTR *pBstrEventLogAttribsListStream);
        
        END_INTERFACE
    } ISolimarSoftwareLicenseSvrVtbl;

    interface ISolimarSoftwareLicenseSvr
    {
        CONST_VTBL struct ISolimarSoftwareLicenseSvrVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISolimarSoftwareLicenseSvr_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISolimarSoftwareLicenseSvr_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISolimarSoftwareLicenseSvr_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISolimarSoftwareLicenseSvr_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISolimarSoftwareLicenseSvr_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISolimarSoftwareLicenseSvr_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISolimarSoftwareLicenseSvr_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISolimarSoftwareLicenseSvr_SoftwareModuleLicenseTotalForAll_ByProduct(This,productID,moduleIdent,pLicenseCount)	\
    ( (This)->lpVtbl -> SoftwareModuleLicenseTotalForAll_ByProduct(This,productID,moduleIdent,pLicenseCount) ) 

#define ISolimarSoftwareLicenseSvr_SoftwareModuleLicenseInUseForAll_ByProduct(This,productID,moduleIdent,pLicenseCount)	\
    ( (This)->lpVtbl -> SoftwareModuleLicenseInUseForAll_ByProduct(This,productID,moduleIdent,pLicenseCount) ) 

#define ISolimarSoftwareLicenseSvr_SoftwareValidateLicenseApp_ByProduct(This,productID,pbLicenseValid)	\
    ( (This)->lpVtbl -> SoftwareValidateLicenseApp_ByProduct(This,productID,pbLicenseValid) ) 

#define ISolimarSoftwareLicenseSvr_SoftwareModuleLicenseInUseByApp_ByProduct(This,productID,moduleIdent,pLicenseCount)	\
    ( (This)->lpVtbl -> SoftwareModuleLicenseInUseByApp_ByProduct(This,productID,moduleIdent,pLicenseCount) ) 

#define ISolimarSoftwareLicenseSvr_SoftwareModuleLicenseInUseByConnection_ByProduct(This,productID,moduleIdent,pLicenseCount)	\
    ( (This)->lpVtbl -> SoftwareModuleLicenseInUseByConnection_ByProduct(This,productID,moduleIdent,pLicenseCount) ) 

#define ISolimarSoftwareLicenseSvr_SoftwareModuleLicenseObtainByApp_ByProduct(This,productID,moduleIdent,licenseCount)	\
    ( (This)->lpVtbl -> SoftwareModuleLicenseObtainByApp_ByProduct(This,productID,moduleIdent,licenseCount) ) 

#define ISolimarSoftwareLicenseSvr_SoftwareModuleLicenseReleaseByApp_ByProduct(This,productID,moduleIdent,licenseCount)	\
    ( (This)->lpVtbl -> SoftwareModuleLicenseReleaseByApp_ByProduct(This,productID,moduleIdent,licenseCount) ) 

#define ISolimarSoftwareLicenseSvr_SoftwareModuleLicenseCounterDecrementByApp_ByProduct(This,productID,moduleIdent,licenseCount)	\
    ( (This)->lpVtbl -> SoftwareModuleLicenseCounterDecrementByApp_ByProduct(This,productID,moduleIdent,licenseCount) ) 

#define ISolimarSoftwareLicenseSvr_GetSoftwareLicenseInfoByProduct_ForAll(This,productID,pBstrProductInfoAttribsStream)	\
    ( (This)->lpVtbl -> GetSoftwareLicenseInfoByProduct_ForAll(This,productID,pBstrProductInfoAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr_GetSoftwareLicenseInfo_ForAll(This,pBstrLicenseInfoAttribsStream)	\
    ( (This)->lpVtbl -> GetSoftwareLicenseInfo_ForAll(This,pBstrLicenseInfoAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr_GetAllSoftwareLicenses(This,pBstrListAllLicensesStream)	\
    ( (This)->lpVtbl -> GetAllSoftwareLicenses(This,pBstrListAllLicensesStream) ) 

#define ISolimarSoftwareLicenseSvr_GetSoftwareLicenseInfoByProduct_ByLicense(This,softwareLicense,productID,pBstrProductInfoAttribsStream)	\
    ( (This)->lpVtbl -> GetSoftwareLicenseInfoByProduct_ByLicense(This,softwareLicense,productID,pBstrProductInfoAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr_GetSoftwareLicenseInfo_ByLicense(This,softwareLicense,pBstrLicenseInfoAttribsStream)	\
    ( (This)->lpVtbl -> GetSoftwareLicenseInfo_ByLicense(This,softwareLicense,pBstrLicenseInfoAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr_GetSoftwareLicenseStatus_ByProduct(This,productID,pBstrStringToDwordMap)	\
    ( (This)->lpVtbl -> GetSoftwareLicenseStatus_ByProduct(This,productID,pBstrStringToDwordMap) ) 

#define ISolimarSoftwareLicenseSvr_GetSoftwareLicenseStatus_ByLicense(This,softwareLicense)	\
    ( (This)->lpVtbl -> GetSoftwareLicenseStatus_ByLicense(This,softwareLicense) ) 

#define ISolimarSoftwareLicenseSvr_GetSoftwareSpecByProduct(This,productID,pBstrProductSoftwareSpecAttribs)	\
    ( (This)->lpVtbl -> GetSoftwareSpecByProduct(This,productID,pBstrProductSoftwareSpecAttribs) ) 

#define ISolimarSoftwareLicenseSvr_GetSoftwareSpec(This,pBstrSoftwareSpecAttribsStream)	\
    ( (This)->lpVtbl -> GetSoftwareSpec(This,pBstrSoftwareSpecAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr_SoftwareAddApplicationInstanceByProduct(This,productID,applicationInstance)	\
    ( (This)->lpVtbl -> SoftwareAddApplicationInstanceByProduct(This,productID,applicationInstance) ) 

#define ISolimarSoftwareLicenseSvr_SoftwareRemoveApplicationInstanceByProduct(This,productID,applicationInstance)	\
    ( (This)->lpVtbl -> SoftwareRemoveApplicationInstanceByProduct(This,productID,applicationInstance) ) 

#define ISolimarSoftwareLicenseSvr_SoftwareGetApplicationInstanceListByProduct(This,productID,pBstrListAppInstStream)	\
    ( (This)->lpVtbl -> SoftwareGetApplicationInstanceListByProduct(This,productID,pBstrListAppInstStream) ) 

#define ISolimarSoftwareLicenseSvr_GenerateSoftwareLicPacket(This,bstrLicPackageAttribsStream,vtExpires,pBstrVerificationCode,pVtLicensePacket)	\
    ( (This)->lpVtbl -> GenerateSoftwareLicPacket(This,bstrLicPackageAttribsStream,vtExpires,pBstrVerificationCode,pVtLicensePacket) ) 

#define ISolimarSoftwareLicenseSvr_EnterSoftwareLicPacket(This,vtLicensePacket,pBstrVerificationCode)	\
    ( (This)->lpVtbl -> EnterSoftwareLicPacket(This,vtLicensePacket,pBstrVerificationCode) ) 

#define ISolimarSoftwareLicenseSvr_ValidateToken_ByLicense(This,softwareLicense,validationTokenType,validationValue)	\
    ( (This)->lpVtbl -> ValidateToken_ByLicense(This,softwareLicense,validationTokenType,validationValue) ) 

#define ISolimarSoftwareLicenseSvr_SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber(This,softwareLicense,contractNumber)	\
    ( (This)->lpVtbl -> SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber(This,softwareLicense,contractNumber) ) 

#define ISolimarSoftwareLicenseSvr_GenerateVerifyDataWithVerCode_ByLicense(This,softwareLicense,pVtLicensePacket)	\
    ( (This)->lpVtbl -> GenerateVerifyDataWithVerCode_ByLicense(This,softwareLicense,pVtLicensePacket) ) 

#define ISolimarSoftwareLicenseSvr_GenerateVerifyDataWithLicInfo_ByLicense(This,softwareLicense,pVtLicensePacket)	\
    ( (This)->lpVtbl -> GenerateVerifyDataWithLicInfo_ByLicense(This,softwareLicense,pVtLicensePacket) ) 

#define ISolimarSoftwareLicenseSvr_GenerateSoftwareLicArchive_ByLicense(This,softwareLicense,pVtLicenseArchive)	\
    ( (This)->lpVtbl -> GenerateSoftwareLicArchive_ByLicense(This,softwareLicense,pVtLicenseArchive) ) 

#define ISolimarSoftwareLicenseSvr_GenerateLicPackage_ByVerifyData(This,vtLicensePacket,pBstrLicensePackageAttribsStream)	\
    ( (This)->lpVtbl -> GenerateLicPackage_ByVerifyData(This,vtLicensePacket,pBstrLicensePackageAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr_GenerateLicPackage_BySoftwareLicArchive(This,vtLicenseArchive,pBstrLicensePackageAttribsStream)	\
    ( (This)->lpVtbl -> GenerateLicPackage_BySoftwareLicArchive(This,vtLicenseArchive,pBstrLicensePackageAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr_GenerateLicPackage_BySoftwareLicPacket(This,vtLicensePacket,pBstrLicensePackageAttribsStream)	\
    ( (This)->lpVtbl -> GenerateLicPackage_BySoftwareLicPacket(This,vtLicensePacket,pBstrLicensePackageAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr_EnterSoftwareLicArchive(This,vtLicenseArchive)	\
    ( (This)->lpVtbl -> EnterSoftwareLicArchive(This,vtLicenseArchive) ) 

#define ISolimarSoftwareLicenseSvr_GenerateLicenseSystemData(This,pVtLicSysDataPacket)	\
    ( (This)->lpVtbl -> GenerateLicenseSystemData(This,pVtLicSysDataPacket) ) 

#define ISolimarSoftwareLicenseSvr_GenerateStream_ByLicenseSystemData(This,vtLicSysDataPacket,pBstrLicSysDataAttribsStream)	\
    ( (This)->lpVtbl -> GenerateStream_ByLicenseSystemData(This,vtLicSysDataPacket,pBstrLicSysDataAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr_GenerateStreamData_ByLicenseSystemData(This,vtLicSysDataPacket,pBstrCreatedDateStreamed,pBstrKeyAttribsListStream,pBstrLicUsageDataAttribsStream,pBstrConnectionAttribsListStream,pBstrEventLogAttribsListStream,pBstrLicInfoDataAttribsListStream)	\
    ( (This)->lpVtbl -> GenerateStreamData_ByLicenseSystemData(This,vtLicSysDataPacket,pBstrCreatedDateStreamed,pBstrKeyAttribsListStream,pBstrLicUsageDataAttribsStream,pBstrConnectionAttribsListStream,pBstrEventLogAttribsListStream,pBstrLicInfoDataAttribsListStream) ) 

#define ISolimarSoftwareLicenseSvr_GetEventLogList_ForLicenseServer(This,pBstrEventLogAttribsListStream)	\
    ( (This)->lpVtbl -> GetEventLogList_ForLicenseServer(This,pBstrEventLogAttribsListStream) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISolimarSoftwareLicenseSvr_INTERFACE_DEFINED__ */


#ifndef __ISolimarSoftwareLicenseSvr2_INTERFACE_DEFINED__
#define __ISolimarSoftwareLicenseSvr2_INTERFACE_DEFINED__

/* interface ISolimarSoftwareLicenseSvr2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISolimarSoftwareLicenseSvr2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3295BF3C-D692-435a-86AF-DC559094FD86")
    ISolimarSoftwareLicenseSvr2 : public ISolimarSoftwareLicenseSvr
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SoftwareAddApplicationInstanceByProduct2( 
            /* [in] */ long productID,
            /* [in] */ BSTR applicationInstance,
            /* [in] */ long flags) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SoftwareGetApplicationInstanceListByProduct2( 
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrListUsAppInstInfoAttribs) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ISolimarSoftwareLicenseSvr2Vtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarSoftwareLicenseSvr2 * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarSoftwareLicenseSvr2 * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareModuleLicenseTotalForAll_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareModuleLicenseTotalForAll_ByProduct )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [retval][out] */ long *pLicenseCount);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareModuleLicenseInUseForAll_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareModuleLicenseInUseForAll_ByProduct )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [retval][out] */ long *pLicenseCount);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareValidateLicenseApp_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareValidateLicenseApp_ByProduct )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ long productID,
            /* [retval][out] */ VARIANT_BOOL *pbLicenseValid);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareModuleLicenseInUseByApp_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareModuleLicenseInUseByApp_ByProduct )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [retval][out] */ long *pLicenseCount);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareModuleLicenseInUseByConnection_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareModuleLicenseInUseByConnection_ByProduct )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [retval][out] */ long *pLicenseCount);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareModuleLicenseObtainByApp_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareModuleLicenseObtainByApp_ByProduct )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [in] */ long licenseCount);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareModuleLicenseReleaseByApp_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareModuleLicenseReleaseByApp_ByProduct )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [in] */ long licenseCount);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareModuleLicenseCounterDecrementByApp_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareModuleLicenseCounterDecrementByApp_ByProduct )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [in] */ long licenseCount);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareLicenseInfoByProduct_ForAll)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareLicenseInfoByProduct_ForAll )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrProductInfoAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareLicenseInfo_ForAll)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareLicenseInfo_ForAll )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [retval][out] */ BSTR *pBstrLicenseInfoAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetAllSoftwareLicenses)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAllSoftwareLicenses )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [retval][out] */ BSTR *pBstrListAllLicensesStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareLicenseInfoByProduct_ByLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareLicenseInfoByProduct_ByLicense )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ BSTR softwareLicense,
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrProductInfoAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareLicenseInfo_ByLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareLicenseInfo_ByLicense )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ BSTR softwareLicense,
            /* [retval][out] */ BSTR *pBstrLicenseInfoAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareLicenseStatus_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareLicenseStatus_ByProduct )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrStringToDwordMap);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareLicenseStatus_ByLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareLicenseStatus_ByLicense )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ BSTR softwareLicense);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareSpecByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareSpecByProduct )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrProductSoftwareSpecAttribs);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareSpec)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareSpec )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [retval][out] */ BSTR *pBstrSoftwareSpecAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareAddApplicationInstanceByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareAddApplicationInstanceByProduct )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ long productID,
            /* [in] */ BSTR applicationInstance);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareRemoveApplicationInstanceByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareRemoveApplicationInstanceByProduct )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ long productID,
            /* [in] */ BSTR applicationInstance);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareGetApplicationInstanceListByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareGetApplicationInstanceListByProduct )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrListAppInstStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateSoftwareLicPacket)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateSoftwareLicPacket )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ BSTR bstrLicPackageAttribsStream,
            /* [in] */ VARIANT vtExpires,
            /* [out] */ BSTR *pBstrVerificationCode,
            /* [retval][out] */ VARIANT *pVtLicensePacket);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, EnterSoftwareLicPacket)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnterSoftwareLicPacket )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ VARIANT vtLicensePacket,
            /* [retval][out] */ BSTR *pBstrVerificationCode);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, ValidateToken_ByLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ValidateToken_ByLicense )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ BSTR softwareLicense,
            /* [in] */ long validationTokenType,
            /* [in] */ BSTR validationValue);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ BSTR softwareLicense,
            /* [in] */ BSTR contractNumber);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateVerifyDataWithVerCode_ByLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateVerifyDataWithVerCode_ByLicense )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ BSTR softwareLicense,
            /* [retval][out] */ VARIANT *pVtLicensePacket);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateVerifyDataWithLicInfo_ByLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateVerifyDataWithLicInfo_ByLicense )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ BSTR softwareLicense,
            /* [retval][out] */ VARIANT *pVtLicensePacket);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateSoftwareLicArchive_ByLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateSoftwareLicArchive_ByLicense )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ BSTR softwareLicense,
            /* [retval][out] */ VARIANT *pVtLicenseArchive);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateLicPackage_ByVerifyData)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateLicPackage_ByVerifyData )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ VARIANT vtLicensePacket,
            /* [retval][out] */ BSTR *pBstrLicensePackageAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateLicPackage_BySoftwareLicArchive)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateLicPackage_BySoftwareLicArchive )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ VARIANT vtLicenseArchive,
            /* [retval][out] */ BSTR *pBstrLicensePackageAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateLicPackage_BySoftwareLicPacket)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateLicPackage_BySoftwareLicPacket )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ VARIANT vtLicensePacket,
            /* [retval][out] */ BSTR *pBstrLicensePackageAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, EnterSoftwareLicArchive)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnterSoftwareLicArchive )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ VARIANT vtLicenseArchive);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateLicenseSystemData)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateLicenseSystemData )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [retval][out] */ VARIANT *pVtLicSysDataPacket);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateStream_ByLicenseSystemData)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateStream_ByLicenseSystemData )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ VARIANT vtLicSysDataPacket,
            /* [retval][out] */ BSTR *pBstrLicSysDataAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateStreamData_ByLicenseSystemData)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateStreamData_ByLicenseSystemData )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ VARIANT vtLicSysDataPacket,
            /* [out] */ BSTR *pBstrCreatedDateStreamed,
            /* [out] */ BSTR *pBstrKeyAttribsListStream,
            /* [out] */ BSTR *pBstrLicUsageDataAttribsStream,
            /* [out] */ BSTR *pBstrConnectionAttribsListStream,
            /* [out] */ BSTR *pBstrEventLogAttribsListStream,
            /* [retval][out] */ BSTR *pBstrLicInfoDataAttribsListStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetEventLogList_ForLicenseServer)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetEventLogList_ForLicenseServer )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [retval][out] */ BSTR *pBstrEventLogAttribsListStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr2, SoftwareAddApplicationInstanceByProduct2)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareAddApplicationInstanceByProduct2 )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ long productID,
            /* [in] */ BSTR applicationInstance,
            /* [in] */ long flags);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr2, SoftwareGetApplicationInstanceListByProduct2)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareGetApplicationInstanceListByProduct2 )( 
            ISolimarSoftwareLicenseSvr2 * This,
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrListUsAppInstInfoAttribs);
        
        END_INTERFACE
    } ISolimarSoftwareLicenseSvr2Vtbl;

    interface ISolimarSoftwareLicenseSvr2
    {
        CONST_VTBL struct ISolimarSoftwareLicenseSvr2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISolimarSoftwareLicenseSvr2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISolimarSoftwareLicenseSvr2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISolimarSoftwareLicenseSvr2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISolimarSoftwareLicenseSvr2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISolimarSoftwareLicenseSvr2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISolimarSoftwareLicenseSvr2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISolimarSoftwareLicenseSvr2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISolimarSoftwareLicenseSvr2_SoftwareModuleLicenseTotalForAll_ByProduct(This,productID,moduleIdent,pLicenseCount)	\
    ( (This)->lpVtbl -> SoftwareModuleLicenseTotalForAll_ByProduct(This,productID,moduleIdent,pLicenseCount) ) 

#define ISolimarSoftwareLicenseSvr2_SoftwareModuleLicenseInUseForAll_ByProduct(This,productID,moduleIdent,pLicenseCount)	\
    ( (This)->lpVtbl -> SoftwareModuleLicenseInUseForAll_ByProduct(This,productID,moduleIdent,pLicenseCount) ) 

#define ISolimarSoftwareLicenseSvr2_SoftwareValidateLicenseApp_ByProduct(This,productID,pbLicenseValid)	\
    ( (This)->lpVtbl -> SoftwareValidateLicenseApp_ByProduct(This,productID,pbLicenseValid) ) 

#define ISolimarSoftwareLicenseSvr2_SoftwareModuleLicenseInUseByApp_ByProduct(This,productID,moduleIdent,pLicenseCount)	\
    ( (This)->lpVtbl -> SoftwareModuleLicenseInUseByApp_ByProduct(This,productID,moduleIdent,pLicenseCount) ) 

#define ISolimarSoftwareLicenseSvr2_SoftwareModuleLicenseInUseByConnection_ByProduct(This,productID,moduleIdent,pLicenseCount)	\
    ( (This)->lpVtbl -> SoftwareModuleLicenseInUseByConnection_ByProduct(This,productID,moduleIdent,pLicenseCount) ) 

#define ISolimarSoftwareLicenseSvr2_SoftwareModuleLicenseObtainByApp_ByProduct(This,productID,moduleIdent,licenseCount)	\
    ( (This)->lpVtbl -> SoftwareModuleLicenseObtainByApp_ByProduct(This,productID,moduleIdent,licenseCount) ) 

#define ISolimarSoftwareLicenseSvr2_SoftwareModuleLicenseReleaseByApp_ByProduct(This,productID,moduleIdent,licenseCount)	\
    ( (This)->lpVtbl -> SoftwareModuleLicenseReleaseByApp_ByProduct(This,productID,moduleIdent,licenseCount) ) 

#define ISolimarSoftwareLicenseSvr2_SoftwareModuleLicenseCounterDecrementByApp_ByProduct(This,productID,moduleIdent,licenseCount)	\
    ( (This)->lpVtbl -> SoftwareModuleLicenseCounterDecrementByApp_ByProduct(This,productID,moduleIdent,licenseCount) ) 

#define ISolimarSoftwareLicenseSvr2_GetSoftwareLicenseInfoByProduct_ForAll(This,productID,pBstrProductInfoAttribsStream)	\
    ( (This)->lpVtbl -> GetSoftwareLicenseInfoByProduct_ForAll(This,productID,pBstrProductInfoAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr2_GetSoftwareLicenseInfo_ForAll(This,pBstrLicenseInfoAttribsStream)	\
    ( (This)->lpVtbl -> GetSoftwareLicenseInfo_ForAll(This,pBstrLicenseInfoAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr2_GetAllSoftwareLicenses(This,pBstrListAllLicensesStream)	\
    ( (This)->lpVtbl -> GetAllSoftwareLicenses(This,pBstrListAllLicensesStream) ) 

#define ISolimarSoftwareLicenseSvr2_GetSoftwareLicenseInfoByProduct_ByLicense(This,softwareLicense,productID,pBstrProductInfoAttribsStream)	\
    ( (This)->lpVtbl -> GetSoftwareLicenseInfoByProduct_ByLicense(This,softwareLicense,productID,pBstrProductInfoAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr2_GetSoftwareLicenseInfo_ByLicense(This,softwareLicense,pBstrLicenseInfoAttribsStream)	\
    ( (This)->lpVtbl -> GetSoftwareLicenseInfo_ByLicense(This,softwareLicense,pBstrLicenseInfoAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr2_GetSoftwareLicenseStatus_ByProduct(This,productID,pBstrStringToDwordMap)	\
    ( (This)->lpVtbl -> GetSoftwareLicenseStatus_ByProduct(This,productID,pBstrStringToDwordMap) ) 

#define ISolimarSoftwareLicenseSvr2_GetSoftwareLicenseStatus_ByLicense(This,softwareLicense)	\
    ( (This)->lpVtbl -> GetSoftwareLicenseStatus_ByLicense(This,softwareLicense) ) 

#define ISolimarSoftwareLicenseSvr2_GetSoftwareSpecByProduct(This,productID,pBstrProductSoftwareSpecAttribs)	\
    ( (This)->lpVtbl -> GetSoftwareSpecByProduct(This,productID,pBstrProductSoftwareSpecAttribs) ) 

#define ISolimarSoftwareLicenseSvr2_GetSoftwareSpec(This,pBstrSoftwareSpecAttribsStream)	\
    ( (This)->lpVtbl -> GetSoftwareSpec(This,pBstrSoftwareSpecAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr2_SoftwareAddApplicationInstanceByProduct(This,productID,applicationInstance)	\
    ( (This)->lpVtbl -> SoftwareAddApplicationInstanceByProduct(This,productID,applicationInstance) ) 

#define ISolimarSoftwareLicenseSvr2_SoftwareRemoveApplicationInstanceByProduct(This,productID,applicationInstance)	\
    ( (This)->lpVtbl -> SoftwareRemoveApplicationInstanceByProduct(This,productID,applicationInstance) ) 

#define ISolimarSoftwareLicenseSvr2_SoftwareGetApplicationInstanceListByProduct(This,productID,pBstrListAppInstStream)	\
    ( (This)->lpVtbl -> SoftwareGetApplicationInstanceListByProduct(This,productID,pBstrListAppInstStream) ) 

#define ISolimarSoftwareLicenseSvr2_GenerateSoftwareLicPacket(This,bstrLicPackageAttribsStream,vtExpires,pBstrVerificationCode,pVtLicensePacket)	\
    ( (This)->lpVtbl -> GenerateSoftwareLicPacket(This,bstrLicPackageAttribsStream,vtExpires,pBstrVerificationCode,pVtLicensePacket) ) 

#define ISolimarSoftwareLicenseSvr2_EnterSoftwareLicPacket(This,vtLicensePacket,pBstrVerificationCode)	\
    ( (This)->lpVtbl -> EnterSoftwareLicPacket(This,vtLicensePacket,pBstrVerificationCode) ) 

#define ISolimarSoftwareLicenseSvr2_ValidateToken_ByLicense(This,softwareLicense,validationTokenType,validationValue)	\
    ( (This)->lpVtbl -> ValidateToken_ByLicense(This,softwareLicense,validationTokenType,validationValue) ) 

#define ISolimarSoftwareLicenseSvr2_SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber(This,softwareLicense,contractNumber)	\
    ( (This)->lpVtbl -> SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber(This,softwareLicense,contractNumber) ) 

#define ISolimarSoftwareLicenseSvr2_GenerateVerifyDataWithVerCode_ByLicense(This,softwareLicense,pVtLicensePacket)	\
    ( (This)->lpVtbl -> GenerateVerifyDataWithVerCode_ByLicense(This,softwareLicense,pVtLicensePacket) ) 

#define ISolimarSoftwareLicenseSvr2_GenerateVerifyDataWithLicInfo_ByLicense(This,softwareLicense,pVtLicensePacket)	\
    ( (This)->lpVtbl -> GenerateVerifyDataWithLicInfo_ByLicense(This,softwareLicense,pVtLicensePacket) ) 

#define ISolimarSoftwareLicenseSvr2_GenerateSoftwareLicArchive_ByLicense(This,softwareLicense,pVtLicenseArchive)	\
    ( (This)->lpVtbl -> GenerateSoftwareLicArchive_ByLicense(This,softwareLicense,pVtLicenseArchive) ) 

#define ISolimarSoftwareLicenseSvr2_GenerateLicPackage_ByVerifyData(This,vtLicensePacket,pBstrLicensePackageAttribsStream)	\
    ( (This)->lpVtbl -> GenerateLicPackage_ByVerifyData(This,vtLicensePacket,pBstrLicensePackageAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr2_GenerateLicPackage_BySoftwareLicArchive(This,vtLicenseArchive,pBstrLicensePackageAttribsStream)	\
    ( (This)->lpVtbl -> GenerateLicPackage_BySoftwareLicArchive(This,vtLicenseArchive,pBstrLicensePackageAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr2_GenerateLicPackage_BySoftwareLicPacket(This,vtLicensePacket,pBstrLicensePackageAttribsStream)	\
    ( (This)->lpVtbl -> GenerateLicPackage_BySoftwareLicPacket(This,vtLicensePacket,pBstrLicensePackageAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr2_EnterSoftwareLicArchive(This,vtLicenseArchive)	\
    ( (This)->lpVtbl -> EnterSoftwareLicArchive(This,vtLicenseArchive) ) 

#define ISolimarSoftwareLicenseSvr2_GenerateLicenseSystemData(This,pVtLicSysDataPacket)	\
    ( (This)->lpVtbl -> GenerateLicenseSystemData(This,pVtLicSysDataPacket) ) 

#define ISolimarSoftwareLicenseSvr2_GenerateStream_ByLicenseSystemData(This,vtLicSysDataPacket,pBstrLicSysDataAttribsStream)	\
    ( (This)->lpVtbl -> GenerateStream_ByLicenseSystemData(This,vtLicSysDataPacket,pBstrLicSysDataAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr2_GenerateStreamData_ByLicenseSystemData(This,vtLicSysDataPacket,pBstrCreatedDateStreamed,pBstrKeyAttribsListStream,pBstrLicUsageDataAttribsStream,pBstrConnectionAttribsListStream,pBstrEventLogAttribsListStream,pBstrLicInfoDataAttribsListStream)	\
    ( (This)->lpVtbl -> GenerateStreamData_ByLicenseSystemData(This,vtLicSysDataPacket,pBstrCreatedDateStreamed,pBstrKeyAttribsListStream,pBstrLicUsageDataAttribsStream,pBstrConnectionAttribsListStream,pBstrEventLogAttribsListStream,pBstrLicInfoDataAttribsListStream) ) 

#define ISolimarSoftwareLicenseSvr2_GetEventLogList_ForLicenseServer(This,pBstrEventLogAttribsListStream)	\
    ( (This)->lpVtbl -> GetEventLogList_ForLicenseServer(This,pBstrEventLogAttribsListStream) ) 


#define ISolimarSoftwareLicenseSvr2_SoftwareAddApplicationInstanceByProduct2(This,productID,applicationInstance,flags)	\
    ( (This)->lpVtbl -> SoftwareAddApplicationInstanceByProduct2(This,productID,applicationInstance,flags) ) 

#define ISolimarSoftwareLicenseSvr2_SoftwareGetApplicationInstanceListByProduct2(This,productID,pBstrListUsAppInstInfoAttribs)	\
    ( (This)->lpVtbl -> SoftwareGetApplicationInstanceListByProduct2(This,productID,pBstrListUsAppInstInfoAttribs) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISolimarSoftwareLicenseSvr2_INTERFACE_DEFINED__ */


#ifndef __ISolimarSoftwareLicenseSvr3_INTERFACE_DEFINED__
#define __ISolimarSoftwareLicenseSvr3_INTERFACE_DEFINED__

/* interface ISolimarSoftwareLicenseSvr3 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISolimarSoftwareLicenseSvr3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0C0E18D9-DB58-49bf-8049-491601DFD84E")
    ISolimarSoftwareLicenseSvr3 : public ISolimarSoftwareLicenseSvr2
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenerateStreamData_ByLicenseSystemData2( 
            /* [in] */ VARIANT vtLicSysDataPacket,
            /* [out] */ BSTR *pBstrCreatedDateStreamed,
            /* [out] */ BSTR *pBstrKeyAttribsListStream,
            /* [out] */ BSTR *pBstrLicUsageDataAttribsStream,
            /* [out] */ BSTR *pBstrConnectionAttribsListStream,
            /* [out] */ BSTR *pBstrEventLogAttribsListStream,
            /* [out] */ BSTR *pBstrLicInfoDataAttribsListStream,
            /* [retval][out] */ BSTR *pBstrLicAlertInfoAttribs) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenerateLicenseSystemDataForSolimar( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ISolimarSoftwareLicenseSvr3Vtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarSoftwareLicenseSvr3 * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarSoftwareLicenseSvr3 * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareModuleLicenseTotalForAll_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareModuleLicenseTotalForAll_ByProduct )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [retval][out] */ long *pLicenseCount);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareModuleLicenseInUseForAll_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareModuleLicenseInUseForAll_ByProduct )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [retval][out] */ long *pLicenseCount);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareValidateLicenseApp_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareValidateLicenseApp_ByProduct )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ long productID,
            /* [retval][out] */ VARIANT_BOOL *pbLicenseValid);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareModuleLicenseInUseByApp_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareModuleLicenseInUseByApp_ByProduct )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [retval][out] */ long *pLicenseCount);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareModuleLicenseInUseByConnection_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareModuleLicenseInUseByConnection_ByProduct )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [retval][out] */ long *pLicenseCount);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareModuleLicenseObtainByApp_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareModuleLicenseObtainByApp_ByProduct )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [in] */ long licenseCount);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareModuleLicenseReleaseByApp_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareModuleLicenseReleaseByApp_ByProduct )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [in] */ long licenseCount);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareModuleLicenseCounterDecrementByApp_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareModuleLicenseCounterDecrementByApp_ByProduct )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ long productID,
            /* [in] */ long moduleIdent,
            /* [in] */ long licenseCount);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareLicenseInfoByProduct_ForAll)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareLicenseInfoByProduct_ForAll )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrProductInfoAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareLicenseInfo_ForAll)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareLicenseInfo_ForAll )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [retval][out] */ BSTR *pBstrLicenseInfoAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetAllSoftwareLicenses)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAllSoftwareLicenses )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [retval][out] */ BSTR *pBstrListAllLicensesStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareLicenseInfoByProduct_ByLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareLicenseInfoByProduct_ByLicense )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ BSTR softwareLicense,
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrProductInfoAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareLicenseInfo_ByLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareLicenseInfo_ByLicense )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ BSTR softwareLicense,
            /* [retval][out] */ BSTR *pBstrLicenseInfoAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareLicenseStatus_ByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareLicenseStatus_ByProduct )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrStringToDwordMap);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareLicenseStatus_ByLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareLicenseStatus_ByLicense )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ BSTR softwareLicense);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareSpecByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareSpecByProduct )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrProductSoftwareSpecAttribs);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetSoftwareSpec)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareSpec )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [retval][out] */ BSTR *pBstrSoftwareSpecAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareAddApplicationInstanceByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareAddApplicationInstanceByProduct )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ long productID,
            /* [in] */ BSTR applicationInstance);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareRemoveApplicationInstanceByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareRemoveApplicationInstanceByProduct )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ long productID,
            /* [in] */ BSTR applicationInstance);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareGetApplicationInstanceListByProduct)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareGetApplicationInstanceListByProduct )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrListAppInstStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateSoftwareLicPacket)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateSoftwareLicPacket )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ BSTR bstrLicPackageAttribsStream,
            /* [in] */ VARIANT vtExpires,
            /* [out] */ BSTR *pBstrVerificationCode,
            /* [retval][out] */ VARIANT *pVtLicensePacket);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, EnterSoftwareLicPacket)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnterSoftwareLicPacket )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ VARIANT vtLicensePacket,
            /* [retval][out] */ BSTR *pBstrVerificationCode);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, ValidateToken_ByLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ValidateToken_ByLicense )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ BSTR softwareLicense,
            /* [in] */ long validationTokenType,
            /* [in] */ BSTR validationValue);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ BSTR softwareLicense,
            /* [in] */ BSTR contractNumber);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateVerifyDataWithVerCode_ByLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateVerifyDataWithVerCode_ByLicense )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ BSTR softwareLicense,
            /* [retval][out] */ VARIANT *pVtLicensePacket);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateVerifyDataWithLicInfo_ByLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateVerifyDataWithLicInfo_ByLicense )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ BSTR softwareLicense,
            /* [retval][out] */ VARIANT *pVtLicensePacket);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateSoftwareLicArchive_ByLicense)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateSoftwareLicArchive_ByLicense )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ BSTR softwareLicense,
            /* [retval][out] */ VARIANT *pVtLicenseArchive);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateLicPackage_ByVerifyData)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateLicPackage_ByVerifyData )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ VARIANT vtLicensePacket,
            /* [retval][out] */ BSTR *pBstrLicensePackageAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateLicPackage_BySoftwareLicArchive)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateLicPackage_BySoftwareLicArchive )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ VARIANT vtLicenseArchive,
            /* [retval][out] */ BSTR *pBstrLicensePackageAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateLicPackage_BySoftwareLicPacket)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateLicPackage_BySoftwareLicPacket )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ VARIANT vtLicensePacket,
            /* [retval][out] */ BSTR *pBstrLicensePackageAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, EnterSoftwareLicArchive)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnterSoftwareLicArchive )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ VARIANT vtLicenseArchive);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateLicenseSystemData)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateLicenseSystemData )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [retval][out] */ VARIANT *pVtLicSysDataPacket);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateStream_ByLicenseSystemData)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateStream_ByLicenseSystemData )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ VARIANT vtLicSysDataPacket,
            /* [retval][out] */ BSTR *pBstrLicSysDataAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GenerateStreamData_ByLicenseSystemData)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateStreamData_ByLicenseSystemData )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ VARIANT vtLicSysDataPacket,
            /* [out] */ BSTR *pBstrCreatedDateStreamed,
            /* [out] */ BSTR *pBstrKeyAttribsListStream,
            /* [out] */ BSTR *pBstrLicUsageDataAttribsStream,
            /* [out] */ BSTR *pBstrConnectionAttribsListStream,
            /* [out] */ BSTR *pBstrEventLogAttribsListStream,
            /* [retval][out] */ BSTR *pBstrLicInfoDataAttribsListStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr, GetEventLogList_ForLicenseServer)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetEventLogList_ForLicenseServer )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [retval][out] */ BSTR *pBstrEventLogAttribsListStream);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr2, SoftwareAddApplicationInstanceByProduct2)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareAddApplicationInstanceByProduct2 )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ long productID,
            /* [in] */ BSTR applicationInstance,
            /* [in] */ long flags);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr2, SoftwareGetApplicationInstanceListByProduct2)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoftwareGetApplicationInstanceListByProduct2 )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ long productID,
            /* [retval][out] */ BSTR *pBstrListUsAppInstInfoAttribs);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr3, GenerateStreamData_ByLicenseSystemData2)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateStreamData_ByLicenseSystemData2 )( 
            ISolimarSoftwareLicenseSvr3 * This,
            /* [in] */ VARIANT vtLicSysDataPacket,
            /* [out] */ BSTR *pBstrCreatedDateStreamed,
            /* [out] */ BSTR *pBstrKeyAttribsListStream,
            /* [out] */ BSTR *pBstrLicUsageDataAttribsStream,
            /* [out] */ BSTR *pBstrConnectionAttribsListStream,
            /* [out] */ BSTR *pBstrEventLogAttribsListStream,
            /* [out] */ BSTR *pBstrLicInfoDataAttribsListStream,
            /* [retval][out] */ BSTR *pBstrLicAlertInfoAttribs);
        
        DECLSPEC_XFGVIRT(ISolimarSoftwareLicenseSvr3, GenerateLicenseSystemDataForSolimar)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateLicenseSystemDataForSolimar )( 
            ISolimarSoftwareLicenseSvr3 * This);
        
        END_INTERFACE
    } ISolimarSoftwareLicenseSvr3Vtbl;

    interface ISolimarSoftwareLicenseSvr3
    {
        CONST_VTBL struct ISolimarSoftwareLicenseSvr3Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISolimarSoftwareLicenseSvr3_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISolimarSoftwareLicenseSvr3_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISolimarSoftwareLicenseSvr3_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISolimarSoftwareLicenseSvr3_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISolimarSoftwareLicenseSvr3_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISolimarSoftwareLicenseSvr3_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISolimarSoftwareLicenseSvr3_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISolimarSoftwareLicenseSvr3_SoftwareModuleLicenseTotalForAll_ByProduct(This,productID,moduleIdent,pLicenseCount)	\
    ( (This)->lpVtbl -> SoftwareModuleLicenseTotalForAll_ByProduct(This,productID,moduleIdent,pLicenseCount) ) 

#define ISolimarSoftwareLicenseSvr3_SoftwareModuleLicenseInUseForAll_ByProduct(This,productID,moduleIdent,pLicenseCount)	\
    ( (This)->lpVtbl -> SoftwareModuleLicenseInUseForAll_ByProduct(This,productID,moduleIdent,pLicenseCount) ) 

#define ISolimarSoftwareLicenseSvr3_SoftwareValidateLicenseApp_ByProduct(This,productID,pbLicenseValid)	\
    ( (This)->lpVtbl -> SoftwareValidateLicenseApp_ByProduct(This,productID,pbLicenseValid) ) 

#define ISolimarSoftwareLicenseSvr3_SoftwareModuleLicenseInUseByApp_ByProduct(This,productID,moduleIdent,pLicenseCount)	\
    ( (This)->lpVtbl -> SoftwareModuleLicenseInUseByApp_ByProduct(This,productID,moduleIdent,pLicenseCount) ) 

#define ISolimarSoftwareLicenseSvr3_SoftwareModuleLicenseInUseByConnection_ByProduct(This,productID,moduleIdent,pLicenseCount)	\
    ( (This)->lpVtbl -> SoftwareModuleLicenseInUseByConnection_ByProduct(This,productID,moduleIdent,pLicenseCount) ) 

#define ISolimarSoftwareLicenseSvr3_SoftwareModuleLicenseObtainByApp_ByProduct(This,productID,moduleIdent,licenseCount)	\
    ( (This)->lpVtbl -> SoftwareModuleLicenseObtainByApp_ByProduct(This,productID,moduleIdent,licenseCount) ) 

#define ISolimarSoftwareLicenseSvr3_SoftwareModuleLicenseReleaseByApp_ByProduct(This,productID,moduleIdent,licenseCount)	\
    ( (This)->lpVtbl -> SoftwareModuleLicenseReleaseByApp_ByProduct(This,productID,moduleIdent,licenseCount) ) 

#define ISolimarSoftwareLicenseSvr3_SoftwareModuleLicenseCounterDecrementByApp_ByProduct(This,productID,moduleIdent,licenseCount)	\
    ( (This)->lpVtbl -> SoftwareModuleLicenseCounterDecrementByApp_ByProduct(This,productID,moduleIdent,licenseCount) ) 

#define ISolimarSoftwareLicenseSvr3_GetSoftwareLicenseInfoByProduct_ForAll(This,productID,pBstrProductInfoAttribsStream)	\
    ( (This)->lpVtbl -> GetSoftwareLicenseInfoByProduct_ForAll(This,productID,pBstrProductInfoAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr3_GetSoftwareLicenseInfo_ForAll(This,pBstrLicenseInfoAttribsStream)	\
    ( (This)->lpVtbl -> GetSoftwareLicenseInfo_ForAll(This,pBstrLicenseInfoAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr3_GetAllSoftwareLicenses(This,pBstrListAllLicensesStream)	\
    ( (This)->lpVtbl -> GetAllSoftwareLicenses(This,pBstrListAllLicensesStream) ) 

#define ISolimarSoftwareLicenseSvr3_GetSoftwareLicenseInfoByProduct_ByLicense(This,softwareLicense,productID,pBstrProductInfoAttribsStream)	\
    ( (This)->lpVtbl -> GetSoftwareLicenseInfoByProduct_ByLicense(This,softwareLicense,productID,pBstrProductInfoAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr3_GetSoftwareLicenseInfo_ByLicense(This,softwareLicense,pBstrLicenseInfoAttribsStream)	\
    ( (This)->lpVtbl -> GetSoftwareLicenseInfo_ByLicense(This,softwareLicense,pBstrLicenseInfoAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr3_GetSoftwareLicenseStatus_ByProduct(This,productID,pBstrStringToDwordMap)	\
    ( (This)->lpVtbl -> GetSoftwareLicenseStatus_ByProduct(This,productID,pBstrStringToDwordMap) ) 

#define ISolimarSoftwareLicenseSvr3_GetSoftwareLicenseStatus_ByLicense(This,softwareLicense)	\
    ( (This)->lpVtbl -> GetSoftwareLicenseStatus_ByLicense(This,softwareLicense) ) 

#define ISolimarSoftwareLicenseSvr3_GetSoftwareSpecByProduct(This,productID,pBstrProductSoftwareSpecAttribs)	\
    ( (This)->lpVtbl -> GetSoftwareSpecByProduct(This,productID,pBstrProductSoftwareSpecAttribs) ) 

#define ISolimarSoftwareLicenseSvr3_GetSoftwareSpec(This,pBstrSoftwareSpecAttribsStream)	\
    ( (This)->lpVtbl -> GetSoftwareSpec(This,pBstrSoftwareSpecAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr3_SoftwareAddApplicationInstanceByProduct(This,productID,applicationInstance)	\
    ( (This)->lpVtbl -> SoftwareAddApplicationInstanceByProduct(This,productID,applicationInstance) ) 

#define ISolimarSoftwareLicenseSvr3_SoftwareRemoveApplicationInstanceByProduct(This,productID,applicationInstance)	\
    ( (This)->lpVtbl -> SoftwareRemoveApplicationInstanceByProduct(This,productID,applicationInstance) ) 

#define ISolimarSoftwareLicenseSvr3_SoftwareGetApplicationInstanceListByProduct(This,productID,pBstrListAppInstStream)	\
    ( (This)->lpVtbl -> SoftwareGetApplicationInstanceListByProduct(This,productID,pBstrListAppInstStream) ) 

#define ISolimarSoftwareLicenseSvr3_GenerateSoftwareLicPacket(This,bstrLicPackageAttribsStream,vtExpires,pBstrVerificationCode,pVtLicensePacket)	\
    ( (This)->lpVtbl -> GenerateSoftwareLicPacket(This,bstrLicPackageAttribsStream,vtExpires,pBstrVerificationCode,pVtLicensePacket) ) 

#define ISolimarSoftwareLicenseSvr3_EnterSoftwareLicPacket(This,vtLicensePacket,pBstrVerificationCode)	\
    ( (This)->lpVtbl -> EnterSoftwareLicPacket(This,vtLicensePacket,pBstrVerificationCode) ) 

#define ISolimarSoftwareLicenseSvr3_ValidateToken_ByLicense(This,softwareLicense,validationTokenType,validationValue)	\
    ( (This)->lpVtbl -> ValidateToken_ByLicense(This,softwareLicense,validationTokenType,validationValue) ) 

#define ISolimarSoftwareLicenseSvr3_SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber(This,softwareLicense,contractNumber)	\
    ( (This)->lpVtbl -> SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber(This,softwareLicense,contractNumber) ) 

#define ISolimarSoftwareLicenseSvr3_GenerateVerifyDataWithVerCode_ByLicense(This,softwareLicense,pVtLicensePacket)	\
    ( (This)->lpVtbl -> GenerateVerifyDataWithVerCode_ByLicense(This,softwareLicense,pVtLicensePacket) ) 

#define ISolimarSoftwareLicenseSvr3_GenerateVerifyDataWithLicInfo_ByLicense(This,softwareLicense,pVtLicensePacket)	\
    ( (This)->lpVtbl -> GenerateVerifyDataWithLicInfo_ByLicense(This,softwareLicense,pVtLicensePacket) ) 

#define ISolimarSoftwareLicenseSvr3_GenerateSoftwareLicArchive_ByLicense(This,softwareLicense,pVtLicenseArchive)	\
    ( (This)->lpVtbl -> GenerateSoftwareLicArchive_ByLicense(This,softwareLicense,pVtLicenseArchive) ) 

#define ISolimarSoftwareLicenseSvr3_GenerateLicPackage_ByVerifyData(This,vtLicensePacket,pBstrLicensePackageAttribsStream)	\
    ( (This)->lpVtbl -> GenerateLicPackage_ByVerifyData(This,vtLicensePacket,pBstrLicensePackageAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr3_GenerateLicPackage_BySoftwareLicArchive(This,vtLicenseArchive,pBstrLicensePackageAttribsStream)	\
    ( (This)->lpVtbl -> GenerateLicPackage_BySoftwareLicArchive(This,vtLicenseArchive,pBstrLicensePackageAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr3_GenerateLicPackage_BySoftwareLicPacket(This,vtLicensePacket,pBstrLicensePackageAttribsStream)	\
    ( (This)->lpVtbl -> GenerateLicPackage_BySoftwareLicPacket(This,vtLicensePacket,pBstrLicensePackageAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr3_EnterSoftwareLicArchive(This,vtLicenseArchive)	\
    ( (This)->lpVtbl -> EnterSoftwareLicArchive(This,vtLicenseArchive) ) 

#define ISolimarSoftwareLicenseSvr3_GenerateLicenseSystemData(This,pVtLicSysDataPacket)	\
    ( (This)->lpVtbl -> GenerateLicenseSystemData(This,pVtLicSysDataPacket) ) 

#define ISolimarSoftwareLicenseSvr3_GenerateStream_ByLicenseSystemData(This,vtLicSysDataPacket,pBstrLicSysDataAttribsStream)	\
    ( (This)->lpVtbl -> GenerateStream_ByLicenseSystemData(This,vtLicSysDataPacket,pBstrLicSysDataAttribsStream) ) 

#define ISolimarSoftwareLicenseSvr3_GenerateStreamData_ByLicenseSystemData(This,vtLicSysDataPacket,pBstrCreatedDateStreamed,pBstrKeyAttribsListStream,pBstrLicUsageDataAttribsStream,pBstrConnectionAttribsListStream,pBstrEventLogAttribsListStream,pBstrLicInfoDataAttribsListStream)	\
    ( (This)->lpVtbl -> GenerateStreamData_ByLicenseSystemData(This,vtLicSysDataPacket,pBstrCreatedDateStreamed,pBstrKeyAttribsListStream,pBstrLicUsageDataAttribsStream,pBstrConnectionAttribsListStream,pBstrEventLogAttribsListStream,pBstrLicInfoDataAttribsListStream) ) 

#define ISolimarSoftwareLicenseSvr3_GetEventLogList_ForLicenseServer(This,pBstrEventLogAttribsListStream)	\
    ( (This)->lpVtbl -> GetEventLogList_ForLicenseServer(This,pBstrEventLogAttribsListStream) ) 


#define ISolimarSoftwareLicenseSvr3_SoftwareAddApplicationInstanceByProduct2(This,productID,applicationInstance,flags)	\
    ( (This)->lpVtbl -> SoftwareAddApplicationInstanceByProduct2(This,productID,applicationInstance,flags) ) 

#define ISolimarSoftwareLicenseSvr3_SoftwareGetApplicationInstanceListByProduct2(This,productID,pBstrListUsAppInstInfoAttribs)	\
    ( (This)->lpVtbl -> SoftwareGetApplicationInstanceListByProduct2(This,productID,pBstrListUsAppInstInfoAttribs) ) 


#define ISolimarSoftwareLicenseSvr3_GenerateStreamData_ByLicenseSystemData2(This,vtLicSysDataPacket,pBstrCreatedDateStreamed,pBstrKeyAttribsListStream,pBstrLicUsageDataAttribsStream,pBstrConnectionAttribsListStream,pBstrEventLogAttribsListStream,pBstrLicInfoDataAttribsListStream,pBstrLicAlertInfoAttribs)	\
    ( (This)->lpVtbl -> GenerateStreamData_ByLicenseSystemData2(This,vtLicSysDataPacket,pBstrCreatedDateStreamed,pBstrKeyAttribsListStream,pBstrLicUsageDataAttribsStream,pBstrConnectionAttribsListStream,pBstrEventLogAttribsListStream,pBstrLicInfoDataAttribsListStream,pBstrLicAlertInfoAttribs) ) 

#define ISolimarSoftwareLicenseSvr3_GenerateLicenseSystemDataForSolimar(This)	\
    ( (This)->lpVtbl -> GenerateLicenseSystemDataForSolimar(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISolimarSoftwareLicenseSvr3_INTERFACE_DEFINED__ */


#ifndef __ISoftwareLicensingMessage_INTERFACE_DEFINED__
#define __ISoftwareLicensingMessage_INTERFACE_DEFINED__

/* interface ISoftwareLicensingMessage */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISoftwareLicensingMessage;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("817EA560-B5F3-4f63-9AD5-3AE239849C68")
    ISoftwareLicensingMessage : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSoftwareLicenseMessageList( 
            /* [in] */ VARIANT_BOOL clear_messages,
            /* [retval][out] */ VARIANT *pvtMessageList) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DispatchSoftwareLicenseMessageList( 
            /* [in] */ VARIANT_BOOL clear_messages) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ISoftwareLicensingMessageVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISoftwareLicensingMessage * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISoftwareLicensingMessage * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISoftwareLicensingMessage * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISoftwareLicensingMessage * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISoftwareLicensingMessage * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISoftwareLicensingMessage * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISoftwareLicensingMessage * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(ISoftwareLicensingMessage, GetSoftwareLicenseMessageList)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSoftwareLicenseMessageList )( 
            ISoftwareLicensingMessage * This,
            /* [in] */ VARIANT_BOOL clear_messages,
            /* [retval][out] */ VARIANT *pvtMessageList);
        
        DECLSPEC_XFGVIRT(ISoftwareLicensingMessage, DispatchSoftwareLicenseMessageList)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DispatchSoftwareLicenseMessageList )( 
            ISoftwareLicensingMessage * This,
            /* [in] */ VARIANT_BOOL clear_messages);
        
        END_INTERFACE
    } ISoftwareLicensingMessageVtbl;

    interface ISoftwareLicensingMessage
    {
        CONST_VTBL struct ISoftwareLicensingMessageVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISoftwareLicensingMessage_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISoftwareLicensingMessage_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISoftwareLicensingMessage_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISoftwareLicensingMessage_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISoftwareLicensingMessage_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISoftwareLicensingMessage_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISoftwareLicensingMessage_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISoftwareLicensingMessage_GetSoftwareLicenseMessageList(This,clear_messages,pvtMessageList)	\
    ( (This)->lpVtbl -> GetSoftwareLicenseMessageList(This,clear_messages,pvtMessageList) ) 

#define ISoftwareLicensingMessage_DispatchSoftwareLicenseMessageList(This,clear_messages)	\
    ( (This)->lpVtbl -> DispatchSoftwareLicenseMessageList(This,clear_messages) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISoftwareLicensingMessage_INTERFACE_DEFINED__ */


#ifndef __ISolimarLicenseEmailAlert_INTERFACE_DEFINED__
#define __ISolimarLicenseEmailAlert_INTERFACE_DEFINED__

/* interface ISolimarLicenseEmailAlert */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISolimarLicenseEmailAlert;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("64BF79C5-F816-439d-80D6-EBB7296BC803")
    ISolimarLicenseEmailAlert : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMailServerInfo( 
            /* [retval][out] */ BSTR *pBstrAlertMailServerAttribsStream) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMailServerInfo( 
            /* [in] */ BSTR bstrAlertMailServerAttribsStream) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TestMailServerInfo( 
            /* [in] */ BSTR bstrTestMailServerAttribsStream) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAllEmailAlerts( 
            /* [retval][out] */ BSTR *pBstrEmailAlertMailAttribsListStream) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetEmailAlert( 
            /* [in] */ BSTR bstrEmailAlertId,
            /* [retval][out] */ BSTR *pBstrEmailAlertMailAttribsStream) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetEmailAlert( 
            /* [in] */ BSTR bstrEmailAlertId,
            /* [in] */ BSTR bstrEmailAlertMailAttribsStream) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddEmailAlert( 
            /* [in] */ BSTR bstrEmailAlertMailAttribsStream,
            /* [retval][out] */ BSTR *pBstrEmailAlertId) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteEmailAlert( 
            /* [in] */ BSTR bstrEmailAlertId) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ISolimarLicenseEmailAlertVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISolimarLicenseEmailAlert * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISolimarLicenseEmailAlert * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISolimarLicenseEmailAlert * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISolimarLicenseEmailAlert * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISolimarLicenseEmailAlert * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISolimarLicenseEmailAlert * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISolimarLicenseEmailAlert * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseEmailAlert, GetMailServerInfo)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetMailServerInfo )( 
            ISolimarLicenseEmailAlert * This,
            /* [retval][out] */ BSTR *pBstrAlertMailServerAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseEmailAlert, SetMailServerInfo)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetMailServerInfo )( 
            ISolimarLicenseEmailAlert * This,
            /* [in] */ BSTR bstrAlertMailServerAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseEmailAlert, TestMailServerInfo)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TestMailServerInfo )( 
            ISolimarLicenseEmailAlert * This,
            /* [in] */ BSTR bstrTestMailServerAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseEmailAlert, GetAllEmailAlerts)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAllEmailAlerts )( 
            ISolimarLicenseEmailAlert * This,
            /* [retval][out] */ BSTR *pBstrEmailAlertMailAttribsListStream);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseEmailAlert, GetEmailAlert)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetEmailAlert )( 
            ISolimarLicenseEmailAlert * This,
            /* [in] */ BSTR bstrEmailAlertId,
            /* [retval][out] */ BSTR *pBstrEmailAlertMailAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseEmailAlert, SetEmailAlert)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetEmailAlert )( 
            ISolimarLicenseEmailAlert * This,
            /* [in] */ BSTR bstrEmailAlertId,
            /* [in] */ BSTR bstrEmailAlertMailAttribsStream);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseEmailAlert, AddEmailAlert)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddEmailAlert )( 
            ISolimarLicenseEmailAlert * This,
            /* [in] */ BSTR bstrEmailAlertMailAttribsStream,
            /* [retval][out] */ BSTR *pBstrEmailAlertId);
        
        DECLSPEC_XFGVIRT(ISolimarLicenseEmailAlert, DeleteEmailAlert)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteEmailAlert )( 
            ISolimarLicenseEmailAlert * This,
            /* [in] */ BSTR bstrEmailAlertId);
        
        END_INTERFACE
    } ISolimarLicenseEmailAlertVtbl;

    interface ISolimarLicenseEmailAlert
    {
        CONST_VTBL struct ISolimarLicenseEmailAlertVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISolimarLicenseEmailAlert_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISolimarLicenseEmailAlert_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISolimarLicenseEmailAlert_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISolimarLicenseEmailAlert_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISolimarLicenseEmailAlert_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISolimarLicenseEmailAlert_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISolimarLicenseEmailAlert_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISolimarLicenseEmailAlert_GetMailServerInfo(This,pBstrAlertMailServerAttribsStream)	\
    ( (This)->lpVtbl -> GetMailServerInfo(This,pBstrAlertMailServerAttribsStream) ) 

#define ISolimarLicenseEmailAlert_SetMailServerInfo(This,bstrAlertMailServerAttribsStream)	\
    ( (This)->lpVtbl -> SetMailServerInfo(This,bstrAlertMailServerAttribsStream) ) 

#define ISolimarLicenseEmailAlert_TestMailServerInfo(This,bstrTestMailServerAttribsStream)	\
    ( (This)->lpVtbl -> TestMailServerInfo(This,bstrTestMailServerAttribsStream) ) 

#define ISolimarLicenseEmailAlert_GetAllEmailAlerts(This,pBstrEmailAlertMailAttribsListStream)	\
    ( (This)->lpVtbl -> GetAllEmailAlerts(This,pBstrEmailAlertMailAttribsListStream) ) 

#define ISolimarLicenseEmailAlert_GetEmailAlert(This,bstrEmailAlertId,pBstrEmailAlertMailAttribsStream)	\
    ( (This)->lpVtbl -> GetEmailAlert(This,bstrEmailAlertId,pBstrEmailAlertMailAttribsStream) ) 

#define ISolimarLicenseEmailAlert_SetEmailAlert(This,bstrEmailAlertId,bstrEmailAlertMailAttribsStream)	\
    ( (This)->lpVtbl -> SetEmailAlert(This,bstrEmailAlertId,bstrEmailAlertMailAttribsStream) ) 

#define ISolimarLicenseEmailAlert_AddEmailAlert(This,bstrEmailAlertMailAttribsStream,pBstrEmailAlertId)	\
    ( (This)->lpVtbl -> AddEmailAlert(This,bstrEmailAlertMailAttribsStream,pBstrEmailAlertId) ) 

#define ISolimarLicenseEmailAlert_DeleteEmailAlert(This,bstrEmailAlertId)	\
    ( (This)->lpVtbl -> DeleteEmailAlert(This,bstrEmailAlertId) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISolimarLicenseEmailAlert_INTERFACE_DEFINED__ */


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
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IObjectAuthentication * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IObjectAuthentication * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IObjectAuthentication * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IObjectAuthentication * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IObjectAuthentication * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IObjectAuthentication * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IObjectAuthentication * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        DECLSPEC_XFGVIRT(IObjectAuthentication, Challenge)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Challenge )( 
            IObjectAuthentication * This,
            /* [in] */ VARIANT vtChallenge,
            /* [retval][out] */ VARIANT *pvtResponse);
        
        DECLSPEC_XFGVIRT(IObjectAuthentication, GetChallenge)
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetChallenge )( 
            IObjectAuthentication * This,
            /* [retval][out] */ VARIANT *pvtChallenge);
        
        DECLSPEC_XFGVIRT(IObjectAuthentication, PutResponse)
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


#ifndef __ICoAllLicenseInterfaces_INTERFACE_DEFINED__
#define __ICoAllLicenseInterfaces_INTERFACE_DEFINED__

/* interface ICoAllLicenseInterfaces */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_ICoAllLicenseInterfaces;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("f0f29021-010f-45a1-8cc7-272d205f95a0")
    ICoAllLicenseInterfaces : public IUnknown
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct ICoAllLicenseInterfacesVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoAllLicenseInterfaces * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoAllLicenseInterfaces * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoAllLicenseInterfaces * This);
        
        END_INTERFACE
    } ICoAllLicenseInterfacesVtbl;

    interface ICoAllLicenseInterfaces
    {
        CONST_VTBL struct ICoAllLicenseInterfacesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoAllLicenseInterfaces_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoAllLicenseInterfaces_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoAllLicenseInterfaces_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoAllLicenseInterfaces_INTERFACE_DEFINED__ */



#ifndef __SSILicenseInterfaces_LIBRARY_DEFINED__
#define __SSILicenseInterfaces_LIBRARY_DEFINED__

/* library SSILicenseInterfaces */
/* [helpstring][uuid][version] */ 


EXTERN_C const IID LIBID_SSILicenseInterfaces;

EXTERN_C const CLSID CLSID_CCoAllLicenseInterfaces;

#ifdef __cplusplus

class DECLSPEC_UUID("1cad935d-43c6-4429-9004-b6faf274620b")
CCoAllLicenseInterfaces;
#endif
#endif /* __SSILicenseInterfaces_LIBRARY_DEFINED__ */

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


