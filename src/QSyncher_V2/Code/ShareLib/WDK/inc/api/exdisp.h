

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0499 */
/* Compiler settings for exdisp.idl:
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

/* verify that the <rpcsal.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCSAL_H_VERSION__
#define __REQUIRED_RPCSAL_H_VERSION__ 100
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __exdisp_h__
#define __exdisp_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWebBrowser_FWD_DEFINED__
#define __IWebBrowser_FWD_DEFINED__
typedef interface IWebBrowser IWebBrowser;
#endif 	/* __IWebBrowser_FWD_DEFINED__ */


#ifndef __DWebBrowserEvents_FWD_DEFINED__
#define __DWebBrowserEvents_FWD_DEFINED__
typedef interface DWebBrowserEvents DWebBrowserEvents;
#endif 	/* __DWebBrowserEvents_FWD_DEFINED__ */


#ifndef __IWebBrowserApp_FWD_DEFINED__
#define __IWebBrowserApp_FWD_DEFINED__
typedef interface IWebBrowserApp IWebBrowserApp;
#endif 	/* __IWebBrowserApp_FWD_DEFINED__ */


#ifndef __IWebBrowser2_FWD_DEFINED__
#define __IWebBrowser2_FWD_DEFINED__
typedef interface IWebBrowser2 IWebBrowser2;
#endif 	/* __IWebBrowser2_FWD_DEFINED__ */


#ifndef __DWebBrowserEvents2_FWD_DEFINED__
#define __DWebBrowserEvents2_FWD_DEFINED__
typedef interface DWebBrowserEvents2 DWebBrowserEvents2;
#endif 	/* __DWebBrowserEvents2_FWD_DEFINED__ */


#ifndef __WebBrowser_V1_FWD_DEFINED__
#define __WebBrowser_V1_FWD_DEFINED__

#ifdef __cplusplus
typedef class WebBrowser_V1 WebBrowser_V1;
#else
typedef struct WebBrowser_V1 WebBrowser_V1;
#endif /* __cplusplus */

#endif 	/* __WebBrowser_V1_FWD_DEFINED__ */


#ifndef __WebBrowser_FWD_DEFINED__
#define __WebBrowser_FWD_DEFINED__

#ifdef __cplusplus
typedef class WebBrowser WebBrowser;
#else
typedef struct WebBrowser WebBrowser;
#endif /* __cplusplus */

#endif 	/* __WebBrowser_FWD_DEFINED__ */


#ifndef __InternetExplorer_FWD_DEFINED__
#define __InternetExplorer_FWD_DEFINED__

#ifdef __cplusplus
typedef class InternetExplorer InternetExplorer;
#else
typedef struct InternetExplorer InternetExplorer;
#endif /* __cplusplus */

#endif 	/* __InternetExplorer_FWD_DEFINED__ */


#ifndef __ShellBrowserWindow_FWD_DEFINED__
#define __ShellBrowserWindow_FWD_DEFINED__

#ifdef __cplusplus
typedef class ShellBrowserWindow ShellBrowserWindow;
#else
typedef struct ShellBrowserWindow ShellBrowserWindow;
#endif /* __cplusplus */

#endif 	/* __ShellBrowserWindow_FWD_DEFINED__ */


#ifndef __DShellWindowsEvents_FWD_DEFINED__
#define __DShellWindowsEvents_FWD_DEFINED__
typedef interface DShellWindowsEvents DShellWindowsEvents;
#endif 	/* __DShellWindowsEvents_FWD_DEFINED__ */


#ifndef __IShellWindows_FWD_DEFINED__
#define __IShellWindows_FWD_DEFINED__
typedef interface IShellWindows IShellWindows;
#endif 	/* __IShellWindows_FWD_DEFINED__ */


#ifndef __ShellWindows_FWD_DEFINED__
#define __ShellWindows_FWD_DEFINED__

#ifdef __cplusplus
typedef class ShellWindows ShellWindows;
#else
typedef struct ShellWindows ShellWindows;
#endif /* __cplusplus */

#endif 	/* __ShellWindows_FWD_DEFINED__ */


#ifndef __IShellUIHelper_FWD_DEFINED__
#define __IShellUIHelper_FWD_DEFINED__
typedef interface IShellUIHelper IShellUIHelper;
#endif 	/* __IShellUIHelper_FWD_DEFINED__ */


#ifndef __IShellUIHelper2_FWD_DEFINED__
#define __IShellUIHelper2_FWD_DEFINED__
typedef interface IShellUIHelper2 IShellUIHelper2;
#endif 	/* __IShellUIHelper2_FWD_DEFINED__ */


#ifndef __ShellUIHelper_FWD_DEFINED__
#define __ShellUIHelper_FWD_DEFINED__

#ifdef __cplusplus
typedef class ShellUIHelper ShellUIHelper;
#else
typedef struct ShellUIHelper ShellUIHelper;
#endif /* __cplusplus */

#endif 	/* __ShellUIHelper_FWD_DEFINED__ */


#ifndef __DShellNameSpaceEvents_FWD_DEFINED__
#define __DShellNameSpaceEvents_FWD_DEFINED__
typedef interface DShellNameSpaceEvents DShellNameSpaceEvents;
#endif 	/* __DShellNameSpaceEvents_FWD_DEFINED__ */


#ifndef __IShellFavoritesNameSpace_FWD_DEFINED__
#define __IShellFavoritesNameSpace_FWD_DEFINED__
typedef interface IShellFavoritesNameSpace IShellFavoritesNameSpace;
#endif 	/* __IShellFavoritesNameSpace_FWD_DEFINED__ */


#ifndef __IShellNameSpace_FWD_DEFINED__
#define __IShellNameSpace_FWD_DEFINED__
typedef interface IShellNameSpace IShellNameSpace;
#endif 	/* __IShellNameSpace_FWD_DEFINED__ */


#ifndef __ShellNameSpace_FWD_DEFINED__
#define __ShellNameSpace_FWD_DEFINED__

#ifdef __cplusplus
typedef class ShellNameSpace ShellNameSpace;
#else
typedef struct ShellNameSpace ShellNameSpace;
#endif /* __cplusplus */

#endif 	/* __ShellNameSpace_FWD_DEFINED__ */


#ifndef __ShellShellNameSpace_FWD_DEFINED__
#define __ShellShellNameSpace_FWD_DEFINED__

#ifdef __cplusplus
typedef class ShellShellNameSpace ShellShellNameSpace;
#else
typedef struct ShellShellNameSpace ShellShellNameSpace;
#endif /* __cplusplus */

#endif 	/* __ShellShellNameSpace_FWD_DEFINED__ */


#ifndef __IScriptErrorList_FWD_DEFINED__
#define __IScriptErrorList_FWD_DEFINED__
typedef interface IScriptErrorList IScriptErrorList;
#endif 	/* __IScriptErrorList_FWD_DEFINED__ */


#ifndef __CScriptErrorList_FWD_DEFINED__
#define __CScriptErrorList_FWD_DEFINED__

#ifdef __cplusplus
typedef class CScriptErrorList CScriptErrorList;
#else
typedef struct CScriptErrorList CScriptErrorList;
#endif /* __cplusplus */

#endif 	/* __CScriptErrorList_FWD_DEFINED__ */


/* header files for imported files */
#include "ocidl.h"
#include "docobj.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_exdisp_0000_0000 */
/* [local] */ 

#define SID_SkipHung IID_IEnumVARIANT


extern RPC_IF_HANDLE __MIDL_itf_exdisp_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_exdisp_0000_0000_v0_0_s_ifspec;


#ifndef __SHDocVw_LIBRARY_DEFINED__
#define __SHDocVw_LIBRARY_DEFINED__

/* library SHDocVw */
/* [version][lcid][helpstring][uuid] */ 

typedef /* [helpstring][uuid] */  DECLSPEC_UUID("34A226E0-DF30-11CF-89A9-00A0C9054129") 
enum CommandStateChangeConstants
    {	CSC_UPDATECOMMANDS	= 0xffffffff,
	CSC_NAVIGATEFORWARD	= 0x1,
	CSC_NAVIGATEBACK	= 0x2
    } 	CommandStateChangeConstants;

typedef /* [helpstring][uuid] */  DECLSPEC_UUID("65507BE0-91A8-11d3-A845-009027220E6D") 
enum SecureLockIconConstants
    {	secureLockIconUnsecure	= 0,
	secureLockIconMixed	= 0x1,
	secureLockIconSecureUnknownBits	= 0x2,
	secureLockIconSecure40Bit	= 0x3,
	secureLockIconSecure56Bit	= 0x4,
	secureLockIconSecureFortezza	= 0x5,
	secureLockIconSecure128Bit	= 0x6
    } 	SecureLockIconConstants;

typedef /* [helpstring][uuid] */  DECLSPEC_UUID("F41E6981-28E5-11d0-82B4-00A0C90C29C5") 
enum ShellWindowTypeConstants
    {	SWC_EXPLORER	= 0,
	SWC_BROWSER	= 0x1,
	SWC_3RDPARTY	= 0x2,
	SWC_CALLBACK	= 0x4,
	SWC_DESKTOP	= 0x8
    } 	ShellWindowTypeConstants;

typedef /* [hidden][helpstring][uuid] */  DECLSPEC_UUID("7716a370-38ca-11d0-a48b-00a0c90a8f39") 
enum ShellWindowFindWindowOptions
    {	SWFO_NEEDDISPATCH	= 0x1,
	SWFO_INCLUDEPENDING	= 0x2,
	SWFO_COOKIEPASSED	= 0x4
    } 	ShellWindowFindWindowOptions;


EXTERN_C const IID LIBID_SHDocVw;

#ifndef __IWebBrowser_INTERFACE_DEFINED__
#define __IWebBrowser_INTERFACE_DEFINED__

/* interface IWebBrowser */
/* [object][oleautomation][dual][hidden][helpcontext][helpstring][uuid] */ 

typedef /* [helpstring][uuid] */  DECLSPEC_UUID("14EE5380-A378-11cf-A731-00A0C9082637") 
enum BrowserNavConstants
    {	navOpenInNewWindow	= 0x1,
	navNoHistory	= 0x2,
	navNoReadFromCache	= 0x4,
	navNoWriteToCache	= 0x8,
	navAllowAutosearch	= 0x10,
	navBrowserBar	= 0x20,
	navHyperlink	= 0x40,
	navEnforceRestricted	= 0x80,
	navNewWindowsManaged	= 0x100,
	navUntrustedForDownload	= 0x200,
	navTrustedForActiveX	= 0x400,
	navOpenInNewTab	= 0x800,
	navOpenInBackgroundTab	= 0x1000,
	navKeepWordWheelText	= 0x2000
    } 	BrowserNavConstants;

typedef /* [helpstring][uuid] */  DECLSPEC_UUID("C317C261-A991-11cf-A731-00A0C9082637") 
enum RefreshConstants
    {	REFRESH_NORMAL	= 0,
	REFRESH_IFEXPIRED	= 1,
	REFRESH_COMPLETELY	= 3
    } 	RefreshConstants;


EXTERN_C const IID IID_IWebBrowser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EAB22AC1-30C1-11CF-A7EB-0000C05BAE0B")
    IWebBrowser : public IDispatch
    {
    public:
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE GoBack( void) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE GoForward( void) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE GoHome( void) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE GoSearch( void) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE Navigate( 
            /* [in] */ __RPC__in BSTR URL,
            /* [optional][in] */ __RPC__in VARIANT *Flags,
            /* [optional][in] */ __RPC__in VARIANT *TargetFrameName,
            /* [optional][in] */ __RPC__in VARIANT *PostData,
            /* [optional][in] */ __RPC__in VARIANT *Headers) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE Refresh( void) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE Refresh2( 
            /* [optional][in] */ __RPC__in VARIANT *Level) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Application( 
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppDisp) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Parent( 
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppDisp) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Container( 
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppDisp) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Document( 
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppDisp) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_TopLevelContainer( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *Type) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Left( 
            /* [retval][out] */ __RPC__out long *pl) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Left( 
            /* [in] */ long Left) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Top( 
            /* [retval][out] */ __RPC__out long *pl) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Top( 
            /* [in] */ long Top) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ __RPC__out long *pl) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Width( 
            /* [in] */ long Width) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ __RPC__out long *pl) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Height( 
            /* [in] */ long Height) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_LocationName( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *LocationName) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_LocationURL( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *LocationURL) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Busy( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWebBrowserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWebBrowser * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWebBrowser * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWebBrowser * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWebBrowser * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWebBrowser * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWebBrowser * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWebBrowser * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GoBack )( 
            IWebBrowser * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GoForward )( 
            IWebBrowser * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GoHome )( 
            IWebBrowser * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GoSearch )( 
            IWebBrowser * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Navigate )( 
            IWebBrowser * This,
            /* [in] */ __RPC__in BSTR URL,
            /* [optional][in] */ __RPC__in VARIANT *Flags,
            /* [optional][in] */ __RPC__in VARIANT *TargetFrameName,
            /* [optional][in] */ __RPC__in VARIANT *PostData,
            /* [optional][in] */ __RPC__in VARIANT *Headers);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Refresh )( 
            IWebBrowser * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Refresh2 )( 
            IWebBrowser * This,
            /* [optional][in] */ __RPC__in VARIANT *Level);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IWebBrowser * This);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Application )( 
            IWebBrowser * This,
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppDisp);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Parent )( 
            IWebBrowser * This,
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppDisp);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Container )( 
            IWebBrowser * This,
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppDisp);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Document )( 
            IWebBrowser * This,
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppDisp);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_TopLevelContainer )( 
            IWebBrowser * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IWebBrowser * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *Type);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IWebBrowser * This,
            /* [retval][out] */ __RPC__out long *pl);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Left )( 
            IWebBrowser * This,
            /* [in] */ long Left);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IWebBrowser * This,
            /* [retval][out] */ __RPC__out long *pl);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Top )( 
            IWebBrowser * This,
            /* [in] */ long Top);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IWebBrowser * This,
            /* [retval][out] */ __RPC__out long *pl);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Width )( 
            IWebBrowser * This,
            /* [in] */ long Width);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IWebBrowser * This,
            /* [retval][out] */ __RPC__out long *pl);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Height )( 
            IWebBrowser * This,
            /* [in] */ long Height);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_LocationName )( 
            IWebBrowser * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *LocationName);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_LocationURL )( 
            IWebBrowser * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *LocationURL);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Busy )( 
            IWebBrowser * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool);
        
        END_INTERFACE
    } IWebBrowserVtbl;

    interface IWebBrowser
    {
        CONST_VTBL struct IWebBrowserVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWebBrowser_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWebBrowser_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWebBrowser_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWebBrowser_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IWebBrowser_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IWebBrowser_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IWebBrowser_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IWebBrowser_GoBack(This)	\
    ( (This)->lpVtbl -> GoBack(This) ) 

#define IWebBrowser_GoForward(This)	\
    ( (This)->lpVtbl -> GoForward(This) ) 

#define IWebBrowser_GoHome(This)	\
    ( (This)->lpVtbl -> GoHome(This) ) 

#define IWebBrowser_GoSearch(This)	\
    ( (This)->lpVtbl -> GoSearch(This) ) 

#define IWebBrowser_Navigate(This,URL,Flags,TargetFrameName,PostData,Headers)	\
    ( (This)->lpVtbl -> Navigate(This,URL,Flags,TargetFrameName,PostData,Headers) ) 

#define IWebBrowser_Refresh(This)	\
    ( (This)->lpVtbl -> Refresh(This) ) 

#define IWebBrowser_Refresh2(This,Level)	\
    ( (This)->lpVtbl -> Refresh2(This,Level) ) 

#define IWebBrowser_Stop(This)	\
    ( (This)->lpVtbl -> Stop(This) ) 

#define IWebBrowser_get_Application(This,ppDisp)	\
    ( (This)->lpVtbl -> get_Application(This,ppDisp) ) 

#define IWebBrowser_get_Parent(This,ppDisp)	\
    ( (This)->lpVtbl -> get_Parent(This,ppDisp) ) 

#define IWebBrowser_get_Container(This,ppDisp)	\
    ( (This)->lpVtbl -> get_Container(This,ppDisp) ) 

#define IWebBrowser_get_Document(This,ppDisp)	\
    ( (This)->lpVtbl -> get_Document(This,ppDisp) ) 

#define IWebBrowser_get_TopLevelContainer(This,pBool)	\
    ( (This)->lpVtbl -> get_TopLevelContainer(This,pBool) ) 

#define IWebBrowser_get_Type(This,Type)	\
    ( (This)->lpVtbl -> get_Type(This,Type) ) 

#define IWebBrowser_get_Left(This,pl)	\
    ( (This)->lpVtbl -> get_Left(This,pl) ) 

#define IWebBrowser_put_Left(This,Left)	\
    ( (This)->lpVtbl -> put_Left(This,Left) ) 

#define IWebBrowser_get_Top(This,pl)	\
    ( (This)->lpVtbl -> get_Top(This,pl) ) 

#define IWebBrowser_put_Top(This,Top)	\
    ( (This)->lpVtbl -> put_Top(This,Top) ) 

#define IWebBrowser_get_Width(This,pl)	\
    ( (This)->lpVtbl -> get_Width(This,pl) ) 

#define IWebBrowser_put_Width(This,Width)	\
    ( (This)->lpVtbl -> put_Width(This,Width) ) 

#define IWebBrowser_get_Height(This,pl)	\
    ( (This)->lpVtbl -> get_Height(This,pl) ) 

#define IWebBrowser_put_Height(This,Height)	\
    ( (This)->lpVtbl -> put_Height(This,Height) ) 

#define IWebBrowser_get_LocationName(This,LocationName)	\
    ( (This)->lpVtbl -> get_LocationName(This,LocationName) ) 

#define IWebBrowser_get_LocationURL(This,LocationURL)	\
    ( (This)->lpVtbl -> get_LocationURL(This,LocationURL) ) 

#define IWebBrowser_get_Busy(This,pBool)	\
    ( (This)->lpVtbl -> get_Busy(This,pBool) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWebBrowser_INTERFACE_DEFINED__ */


#ifndef __DWebBrowserEvents_DISPINTERFACE_DEFINED__
#define __DWebBrowserEvents_DISPINTERFACE_DEFINED__

/* dispinterface DWebBrowserEvents */
/* [hidden][helpstring][uuid] */ 


EXTERN_C const IID DIID_DWebBrowserEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("EAB22AC2-30C1-11CF-A7EB-0000C05BAE0B")
    DWebBrowserEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct DWebBrowserEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            DWebBrowserEvents * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            DWebBrowserEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            DWebBrowserEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            DWebBrowserEvents * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            DWebBrowserEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            DWebBrowserEvents * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            DWebBrowserEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } DWebBrowserEventsVtbl;

    interface DWebBrowserEvents
    {
        CONST_VTBL struct DWebBrowserEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define DWebBrowserEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define DWebBrowserEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define DWebBrowserEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define DWebBrowserEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define DWebBrowserEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define DWebBrowserEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define DWebBrowserEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __DWebBrowserEvents_DISPINTERFACE_DEFINED__ */


#ifndef __IWebBrowserApp_INTERFACE_DEFINED__
#define __IWebBrowserApp_INTERFACE_DEFINED__

/* interface IWebBrowserApp */
/* [object][dual][oleautomation][hidden][helpcontext][helpstring][uuid] */ 


EXTERN_C const IID IID_IWebBrowserApp;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0002DF05-0000-0000-C000-000000000046")
    IWebBrowserApp : public IWebBrowser
    {
    public:
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE Quit( void) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE ClientToWindow( 
            /* [out][in] */ __RPC__inout int *pcx,
            /* [out][in] */ __RPC__inout int *pcy) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE PutProperty( 
            /* [in] */ __RPC__in BSTR Property,
            /* [in] */ VARIANT vtValue) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE GetProperty( 
            /* [in] */ __RPC__in BSTR Property,
            /* [retval][out] */ __RPC__out VARIANT *pvtValue) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *Name) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_HWND( 
            /* [retval][out] */ __RPC__out SHANDLE_PTR *pHWND) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_FullName( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *FullName) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Path( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *Path) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool) = 0;
        
        virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL Value) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_StatusBar( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool) = 0;
        
        virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_StatusBar( 
            /* [in] */ VARIANT_BOOL Value) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_StatusText( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *StatusText) = 0;
        
        virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_StatusText( 
            /* [in] */ __RPC__in BSTR StatusText) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_ToolBar( 
            /* [retval][out] */ __RPC__out int *Value) = 0;
        
        virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_ToolBar( 
            /* [in] */ int Value) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_MenuBar( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *Value) = 0;
        
        virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_MenuBar( 
            /* [in] */ VARIANT_BOOL Value) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_FullScreen( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pbFullScreen) = 0;
        
        virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_FullScreen( 
            /* [in] */ VARIANT_BOOL bFullScreen) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWebBrowserAppVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWebBrowserApp * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWebBrowserApp * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWebBrowserApp * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWebBrowserApp * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWebBrowserApp * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWebBrowserApp * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWebBrowserApp * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GoBack )( 
            IWebBrowserApp * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GoForward )( 
            IWebBrowserApp * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GoHome )( 
            IWebBrowserApp * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GoSearch )( 
            IWebBrowserApp * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Navigate )( 
            IWebBrowserApp * This,
            /* [in] */ __RPC__in BSTR URL,
            /* [optional][in] */ __RPC__in VARIANT *Flags,
            /* [optional][in] */ __RPC__in VARIANT *TargetFrameName,
            /* [optional][in] */ __RPC__in VARIANT *PostData,
            /* [optional][in] */ __RPC__in VARIANT *Headers);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Refresh )( 
            IWebBrowserApp * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Refresh2 )( 
            IWebBrowserApp * This,
            /* [optional][in] */ __RPC__in VARIANT *Level);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IWebBrowserApp * This);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Application )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppDisp);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Parent )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppDisp);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Container )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppDisp);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Document )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppDisp);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_TopLevelContainer )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *Type);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__out long *pl);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Left )( 
            IWebBrowserApp * This,
            /* [in] */ long Left);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__out long *pl);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Top )( 
            IWebBrowserApp * This,
            /* [in] */ long Top);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__out long *pl);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Width )( 
            IWebBrowserApp * This,
            /* [in] */ long Width);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__out long *pl);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Height )( 
            IWebBrowserApp * This,
            /* [in] */ long Height);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_LocationName )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *LocationName);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_LocationURL )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *LocationURL);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Busy )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Quit )( 
            IWebBrowserApp * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClientToWindow )( 
            IWebBrowserApp * This,
            /* [out][in] */ __RPC__inout int *pcx,
            /* [out][in] */ __RPC__inout int *pcy);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PutProperty )( 
            IWebBrowserApp * This,
            /* [in] */ __RPC__in BSTR Property,
            /* [in] */ VARIANT vtValue);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetProperty )( 
            IWebBrowserApp * This,
            /* [in] */ __RPC__in BSTR Property,
            /* [retval][out] */ __RPC__out VARIANT *pvtValue);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *Name);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_HWND )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__out SHANDLE_PTR *pHWND);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_FullName )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *FullName);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Path )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *Path);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool);
        
        /* [helpcontext][helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IWebBrowserApp * This,
            /* [in] */ VARIANT_BOOL Value);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StatusBar )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool);
        
        /* [helpcontext][helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StatusBar )( 
            IWebBrowserApp * This,
            /* [in] */ VARIANT_BOOL Value);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StatusText )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *StatusText);
        
        /* [helpcontext][helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StatusText )( 
            IWebBrowserApp * This,
            /* [in] */ __RPC__in BSTR StatusText);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ToolBar )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__out int *Value);
        
        /* [helpcontext][helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ToolBar )( 
            IWebBrowserApp * This,
            /* [in] */ int Value);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_MenuBar )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *Value);
        
        /* [helpcontext][helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_MenuBar )( 
            IWebBrowserApp * This,
            /* [in] */ VARIANT_BOOL Value);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_FullScreen )( 
            IWebBrowserApp * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pbFullScreen);
        
        /* [helpcontext][helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FullScreen )( 
            IWebBrowserApp * This,
            /* [in] */ VARIANT_BOOL bFullScreen);
        
        END_INTERFACE
    } IWebBrowserAppVtbl;

    interface IWebBrowserApp
    {
        CONST_VTBL struct IWebBrowserAppVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWebBrowserApp_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWebBrowserApp_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWebBrowserApp_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWebBrowserApp_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IWebBrowserApp_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IWebBrowserApp_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IWebBrowserApp_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IWebBrowserApp_GoBack(This)	\
    ( (This)->lpVtbl -> GoBack(This) ) 

#define IWebBrowserApp_GoForward(This)	\
    ( (This)->lpVtbl -> GoForward(This) ) 

#define IWebBrowserApp_GoHome(This)	\
    ( (This)->lpVtbl -> GoHome(This) ) 

#define IWebBrowserApp_GoSearch(This)	\
    ( (This)->lpVtbl -> GoSearch(This) ) 

#define IWebBrowserApp_Navigate(This,URL,Flags,TargetFrameName,PostData,Headers)	\
    ( (This)->lpVtbl -> Navigate(This,URL,Flags,TargetFrameName,PostData,Headers) ) 

#define IWebBrowserApp_Refresh(This)	\
    ( (This)->lpVtbl -> Refresh(This) ) 

#define IWebBrowserApp_Refresh2(This,Level)	\
    ( (This)->lpVtbl -> Refresh2(This,Level) ) 

#define IWebBrowserApp_Stop(This)	\
    ( (This)->lpVtbl -> Stop(This) ) 

#define IWebBrowserApp_get_Application(This,ppDisp)	\
    ( (This)->lpVtbl -> get_Application(This,ppDisp) ) 

#define IWebBrowserApp_get_Parent(This,ppDisp)	\
    ( (This)->lpVtbl -> get_Parent(This,ppDisp) ) 

#define IWebBrowserApp_get_Container(This,ppDisp)	\
    ( (This)->lpVtbl -> get_Container(This,ppDisp) ) 

#define IWebBrowserApp_get_Document(This,ppDisp)	\
    ( (This)->lpVtbl -> get_Document(This,ppDisp) ) 

#define IWebBrowserApp_get_TopLevelContainer(This,pBool)	\
    ( (This)->lpVtbl -> get_TopLevelContainer(This,pBool) ) 

#define IWebBrowserApp_get_Type(This,Type)	\
    ( (This)->lpVtbl -> get_Type(This,Type) ) 

#define IWebBrowserApp_get_Left(This,pl)	\
    ( (This)->lpVtbl -> get_Left(This,pl) ) 

#define IWebBrowserApp_put_Left(This,Left)	\
    ( (This)->lpVtbl -> put_Left(This,Left) ) 

#define IWebBrowserApp_get_Top(This,pl)	\
    ( (This)->lpVtbl -> get_Top(This,pl) ) 

#define IWebBrowserApp_put_Top(This,Top)	\
    ( (This)->lpVtbl -> put_Top(This,Top) ) 

#define IWebBrowserApp_get_Width(This,pl)	\
    ( (This)->lpVtbl -> get_Width(This,pl) ) 

#define IWebBrowserApp_put_Width(This,Width)	\
    ( (This)->lpVtbl -> put_Width(This,Width) ) 

#define IWebBrowserApp_get_Height(This,pl)	\
    ( (This)->lpVtbl -> get_Height(This,pl) ) 

#define IWebBrowserApp_put_Height(This,Height)	\
    ( (This)->lpVtbl -> put_Height(This,Height) ) 

#define IWebBrowserApp_get_LocationName(This,LocationName)	\
    ( (This)->lpVtbl -> get_LocationName(This,LocationName) ) 

#define IWebBrowserApp_get_LocationURL(This,LocationURL)	\
    ( (This)->lpVtbl -> get_LocationURL(This,LocationURL) ) 

#define IWebBrowserApp_get_Busy(This,pBool)	\
    ( (This)->lpVtbl -> get_Busy(This,pBool) ) 


#define IWebBrowserApp_Quit(This)	\
    ( (This)->lpVtbl -> Quit(This) ) 

#define IWebBrowserApp_ClientToWindow(This,pcx,pcy)	\
    ( (This)->lpVtbl -> ClientToWindow(This,pcx,pcy) ) 

#define IWebBrowserApp_PutProperty(This,Property,vtValue)	\
    ( (This)->lpVtbl -> PutProperty(This,Property,vtValue) ) 

#define IWebBrowserApp_GetProperty(This,Property,pvtValue)	\
    ( (This)->lpVtbl -> GetProperty(This,Property,pvtValue) ) 

#define IWebBrowserApp_get_Name(This,Name)	\
    ( (This)->lpVtbl -> get_Name(This,Name) ) 

#define IWebBrowserApp_get_HWND(This,pHWND)	\
    ( (This)->lpVtbl -> get_HWND(This,pHWND) ) 

#define IWebBrowserApp_get_FullName(This,FullName)	\
    ( (This)->lpVtbl -> get_FullName(This,FullName) ) 

#define IWebBrowserApp_get_Path(This,Path)	\
    ( (This)->lpVtbl -> get_Path(This,Path) ) 

#define IWebBrowserApp_get_Visible(This,pBool)	\
    ( (This)->lpVtbl -> get_Visible(This,pBool) ) 

#define IWebBrowserApp_put_Visible(This,Value)	\
    ( (This)->lpVtbl -> put_Visible(This,Value) ) 

#define IWebBrowserApp_get_StatusBar(This,pBool)	\
    ( (This)->lpVtbl -> get_StatusBar(This,pBool) ) 

#define IWebBrowserApp_put_StatusBar(This,Value)	\
    ( (This)->lpVtbl -> put_StatusBar(This,Value) ) 

#define IWebBrowserApp_get_StatusText(This,StatusText)	\
    ( (This)->lpVtbl -> get_StatusText(This,StatusText) ) 

#define IWebBrowserApp_put_StatusText(This,StatusText)	\
    ( (This)->lpVtbl -> put_StatusText(This,StatusText) ) 

#define IWebBrowserApp_get_ToolBar(This,Value)	\
    ( (This)->lpVtbl -> get_ToolBar(This,Value) ) 

#define IWebBrowserApp_put_ToolBar(This,Value)	\
    ( (This)->lpVtbl -> put_ToolBar(This,Value) ) 

#define IWebBrowserApp_get_MenuBar(This,Value)	\
    ( (This)->lpVtbl -> get_MenuBar(This,Value) ) 

#define IWebBrowserApp_put_MenuBar(This,Value)	\
    ( (This)->lpVtbl -> put_MenuBar(This,Value) ) 

#define IWebBrowserApp_get_FullScreen(This,pbFullScreen)	\
    ( (This)->lpVtbl -> get_FullScreen(This,pbFullScreen) ) 

#define IWebBrowserApp_put_FullScreen(This,bFullScreen)	\
    ( (This)->lpVtbl -> put_FullScreen(This,bFullScreen) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWebBrowserApp_INTERFACE_DEFINED__ */


#ifndef __IWebBrowser2_INTERFACE_DEFINED__
#define __IWebBrowser2_INTERFACE_DEFINED__

/* interface IWebBrowser2 */
/* [object][dual][oleautomation][hidden][helpcontext][helpstring][uuid] */ 


EXTERN_C const IID IID_IWebBrowser2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D30C1661-CDAF-11d0-8A3E-00C04FC9E26E")
    IWebBrowser2 : public IWebBrowserApp
    {
    public:
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE Navigate2( 
            /* [in] */ __RPC__in VARIANT *URL,
            /* [optional][in] */ __RPC__in VARIANT *Flags,
            /* [optional][in] */ __RPC__in VARIANT *TargetFrameName,
            /* [optional][in] */ __RPC__in VARIANT *PostData,
            /* [optional][in] */ __RPC__in VARIANT *Headers) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE QueryStatusWB( 
            /* [in] */ OLECMDID cmdID,
            /* [retval][out] */ __RPC__out OLECMDF *pcmdf) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE ExecWB( 
            /* [in] */ OLECMDID cmdID,
            /* [in] */ OLECMDEXECOPT cmdexecopt,
            /* [optional][in] */ __RPC__in VARIANT *pvaIn,
            /* [optional][in][out] */ __RPC__inout VARIANT *pvaOut) = 0;
        
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE ShowBrowserBar( 
            /* [in] */ __RPC__in VARIANT *pvaClsid,
            /* [optional][in] */ __RPC__in VARIANT *pvarShow,
            /* [optional][in] */ __RPC__in VARIANT *pvarSize) = 0;
        
        virtual /* [bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_ReadyState( 
            /* [out][retval] */ __RPC__out READYSTATE *plReadyState) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Offline( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pbOffline) = 0;
        
        virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Offline( 
            /* [in] */ VARIANT_BOOL bOffline) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Silent( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pbSilent) = 0;
        
        virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Silent( 
            /* [in] */ VARIANT_BOOL bSilent) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_RegisterAsBrowser( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pbRegister) = 0;
        
        virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_RegisterAsBrowser( 
            /* [in] */ VARIANT_BOOL bRegister) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_RegisterAsDropTarget( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pbRegister) = 0;
        
        virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_RegisterAsDropTarget( 
            /* [in] */ VARIANT_BOOL bRegister) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_TheaterMode( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pbRegister) = 0;
        
        virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_TheaterMode( 
            /* [in] */ VARIANT_BOOL bRegister) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_AddressBar( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *Value) = 0;
        
        virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_AddressBar( 
            /* [in] */ VARIANT_BOOL Value) = 0;
        
        virtual /* [helpcontext][helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Resizable( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *Value) = 0;
        
        virtual /* [helpcontext][helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Resizable( 
            /* [in] */ VARIANT_BOOL Value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWebBrowser2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWebBrowser2 * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWebBrowser2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWebBrowser2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWebBrowser2 * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWebBrowser2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWebBrowser2 * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWebBrowser2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GoBack )( 
            IWebBrowser2 * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GoForward )( 
            IWebBrowser2 * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GoHome )( 
            IWebBrowser2 * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GoSearch )( 
            IWebBrowser2 * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Navigate )( 
            IWebBrowser2 * This,
            /* [in] */ __RPC__in BSTR URL,
            /* [optional][in] */ __RPC__in VARIANT *Flags,
            /* [optional][in] */ __RPC__in VARIANT *TargetFrameName,
            /* [optional][in] */ __RPC__in VARIANT *PostData,
            /* [optional][in] */ __RPC__in VARIANT *Headers);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Refresh )( 
            IWebBrowser2 * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Refresh2 )( 
            IWebBrowser2 * This,
            /* [optional][in] */ __RPC__in VARIANT *Level);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IWebBrowser2 * This);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Application )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppDisp);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Parent )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppDisp);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Container )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppDisp);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Document )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppDisp);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_TopLevelContainer )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *Type);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__out long *pl);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Left )( 
            IWebBrowser2 * This,
            /* [in] */ long Left);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__out long *pl);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Top )( 
            IWebBrowser2 * This,
            /* [in] */ long Top);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__out long *pl);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Width )( 
            IWebBrowser2 * This,
            /* [in] */ long Width);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__out long *pl);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Height )( 
            IWebBrowser2 * This,
            /* [in] */ long Height);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_LocationName )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *LocationName);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_LocationURL )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *LocationURL);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Busy )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Quit )( 
            IWebBrowser2 * This);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClientToWindow )( 
            IWebBrowser2 * This,
            /* [out][in] */ __RPC__inout int *pcx,
            /* [out][in] */ __RPC__inout int *pcy);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PutProperty )( 
            IWebBrowser2 * This,
            /* [in] */ __RPC__in BSTR Property,
            /* [in] */ VARIANT vtValue);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetProperty )( 
            IWebBrowser2 * This,
            /* [in] */ __RPC__in BSTR Property,
            /* [retval][out] */ __RPC__out VARIANT *pvtValue);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *Name);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_HWND )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__out SHANDLE_PTR *pHWND);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_FullName )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *FullName);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Path )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *Path);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool);
        
        /* [helpcontext][helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IWebBrowser2 * This,
            /* [in] */ VARIANT_BOOL Value);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StatusBar )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool);
        
        /* [helpcontext][helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StatusBar )( 
            IWebBrowser2 * This,
            /* [in] */ VARIANT_BOOL Value);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StatusText )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *StatusText);
        
        /* [helpcontext][helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StatusText )( 
            IWebBrowser2 * This,
            /* [in] */ __RPC__in BSTR StatusText);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ToolBar )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__out int *Value);
        
        /* [helpcontext][helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ToolBar )( 
            IWebBrowser2 * This,
            /* [in] */ int Value);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_MenuBar )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *Value);
        
        /* [helpcontext][helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_MenuBar )( 
            IWebBrowser2 * This,
            /* [in] */ VARIANT_BOOL Value);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_FullScreen )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pbFullScreen);
        
        /* [helpcontext][helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FullScreen )( 
            IWebBrowser2 * This,
            /* [in] */ VARIANT_BOOL bFullScreen);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Navigate2 )( 
            IWebBrowser2 * This,
            /* [in] */ __RPC__in VARIANT *URL,
            /* [optional][in] */ __RPC__in VARIANT *Flags,
            /* [optional][in] */ __RPC__in VARIANT *TargetFrameName,
            /* [optional][in] */ __RPC__in VARIANT *PostData,
            /* [optional][in] */ __RPC__in VARIANT *Headers);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *QueryStatusWB )( 
            IWebBrowser2 * This,
            /* [in] */ OLECMDID cmdID,
            /* [retval][out] */ __RPC__out OLECMDF *pcmdf);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ExecWB )( 
            IWebBrowser2 * This,
            /* [in] */ OLECMDID cmdID,
            /* [in] */ OLECMDEXECOPT cmdexecopt,
            /* [optional][in] */ __RPC__in VARIANT *pvaIn,
            /* [optional][in][out] */ __RPC__inout VARIANT *pvaOut);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ShowBrowserBar )( 
            IWebBrowser2 * This,
            /* [in] */ __RPC__in VARIANT *pvaClsid,
            /* [optional][in] */ __RPC__in VARIANT *pvarShow,
            /* [optional][in] */ __RPC__in VARIANT *pvarSize);
        
        /* [bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ReadyState )( 
            IWebBrowser2 * This,
            /* [out][retval] */ __RPC__out READYSTATE *plReadyState);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Offline )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pbOffline);
        
        /* [helpcontext][helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Offline )( 
            IWebBrowser2 * This,
            /* [in] */ VARIANT_BOOL bOffline);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Silent )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pbSilent);
        
        /* [helpcontext][helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Silent )( 
            IWebBrowser2 * This,
            /* [in] */ VARIANT_BOOL bSilent);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_RegisterAsBrowser )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pbRegister);
        
        /* [helpcontext][helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_RegisterAsBrowser )( 
            IWebBrowser2 * This,
            /* [in] */ VARIANT_BOOL bRegister);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_RegisterAsDropTarget )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pbRegister);
        
        /* [helpcontext][helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_RegisterAsDropTarget )( 
            IWebBrowser2 * This,
            /* [in] */ VARIANT_BOOL bRegister);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_TheaterMode )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pbRegister);
        
        /* [helpcontext][helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_TheaterMode )( 
            IWebBrowser2 * This,
            /* [in] */ VARIANT_BOOL bRegister);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_AddressBar )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *Value);
        
        /* [helpcontext][helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_AddressBar )( 
            IWebBrowser2 * This,
            /* [in] */ VARIANT_BOOL Value);
        
        /* [helpcontext][helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Resizable )( 
            IWebBrowser2 * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *Value);
        
        /* [helpcontext][helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Resizable )( 
            IWebBrowser2 * This,
            /* [in] */ VARIANT_BOOL Value);
        
        END_INTERFACE
    } IWebBrowser2Vtbl;

    interface IWebBrowser2
    {
        CONST_VTBL struct IWebBrowser2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWebBrowser2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWebBrowser2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWebBrowser2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWebBrowser2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IWebBrowser2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IWebBrowser2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IWebBrowser2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IWebBrowser2_GoBack(This)	\
    ( (This)->lpVtbl -> GoBack(This) ) 

#define IWebBrowser2_GoForward(This)	\
    ( (This)->lpVtbl -> GoForward(This) ) 

#define IWebBrowser2_GoHome(This)	\
    ( (This)->lpVtbl -> GoHome(This) ) 

#define IWebBrowser2_GoSearch(This)	\
    ( (This)->lpVtbl -> GoSearch(This) ) 

#define IWebBrowser2_Navigate(This,URL,Flags,TargetFrameName,PostData,Headers)	\
    ( (This)->lpVtbl -> Navigate(This,URL,Flags,TargetFrameName,PostData,Headers) ) 

#define IWebBrowser2_Refresh(This)	\
    ( (This)->lpVtbl -> Refresh(This) ) 

#define IWebBrowser2_Refresh2(This,Level)	\
    ( (This)->lpVtbl -> Refresh2(This,Level) ) 

#define IWebBrowser2_Stop(This)	\
    ( (This)->lpVtbl -> Stop(This) ) 

#define IWebBrowser2_get_Application(This,ppDisp)	\
    ( (This)->lpVtbl -> get_Application(This,ppDisp) ) 

#define IWebBrowser2_get_Parent(This,ppDisp)	\
    ( (This)->lpVtbl -> get_Parent(This,ppDisp) ) 

#define IWebBrowser2_get_Container(This,ppDisp)	\
    ( (This)->lpVtbl -> get_Container(This,ppDisp) ) 

#define IWebBrowser2_get_Document(This,ppDisp)	\
    ( (This)->lpVtbl -> get_Document(This,ppDisp) ) 

#define IWebBrowser2_get_TopLevelContainer(This,pBool)	\
    ( (This)->lpVtbl -> get_TopLevelContainer(This,pBool) ) 

#define IWebBrowser2_get_Type(This,Type)	\
    ( (This)->lpVtbl -> get_Type(This,Type) ) 

#define IWebBrowser2_get_Left(This,pl)	\
    ( (This)->lpVtbl -> get_Left(This,pl) ) 

#define IWebBrowser2_put_Left(This,Left)	\
    ( (This)->lpVtbl -> put_Left(This,Left) ) 

#define IWebBrowser2_get_Top(This,pl)	\
    ( (This)->lpVtbl -> get_Top(This,pl) ) 

#define IWebBrowser2_put_Top(This,Top)	\
    ( (This)->lpVtbl -> put_Top(This,Top) ) 

#define IWebBrowser2_get_Width(This,pl)	\
    ( (This)->lpVtbl -> get_Width(This,pl) ) 

#define IWebBrowser2_put_Width(This,Width)	\
    ( (This)->lpVtbl -> put_Width(This,Width) ) 

#define IWebBrowser2_get_Height(This,pl)	\
    ( (This)->lpVtbl -> get_Height(This,pl) ) 

#define IWebBrowser2_put_Height(This,Height)	\
    ( (This)->lpVtbl -> put_Height(This,Height) ) 

#define IWebBrowser2_get_LocationName(This,LocationName)	\
    ( (This)->lpVtbl -> get_LocationName(This,LocationName) ) 

#define IWebBrowser2_get_LocationURL(This,LocationURL)	\
    ( (This)->lpVtbl -> get_LocationURL(This,LocationURL) ) 

#define IWebBrowser2_get_Busy(This,pBool)	\
    ( (This)->lpVtbl -> get_Busy(This,pBool) ) 


#define IWebBrowser2_Quit(This)	\
    ( (This)->lpVtbl -> Quit(This) ) 

#define IWebBrowser2_ClientToWindow(This,pcx,pcy)	\
    ( (This)->lpVtbl -> ClientToWindow(This,pcx,pcy) ) 

#define IWebBrowser2_PutProperty(This,Property,vtValue)	\
    ( (This)->lpVtbl -> PutProperty(This,Property,vtValue) ) 

#define IWebBrowser2_GetProperty(This,Property,pvtValue)	\
    ( (This)->lpVtbl -> GetProperty(This,Property,pvtValue) ) 

#define IWebBrowser2_get_Name(This,Name)	\
    ( (This)->lpVtbl -> get_Name(This,Name) ) 

#define IWebBrowser2_get_HWND(This,pHWND)	\
    ( (This)->lpVtbl -> get_HWND(This,pHWND) ) 

#define IWebBrowser2_get_FullName(This,FullName)	\
    ( (This)->lpVtbl -> get_FullName(This,FullName) ) 

#define IWebBrowser2_get_Path(This,Path)	\
    ( (This)->lpVtbl -> get_Path(This,Path) ) 

#define IWebBrowser2_get_Visible(This,pBool)	\
    ( (This)->lpVtbl -> get_Visible(This,pBool) ) 

#define IWebBrowser2_put_Visible(This,Value)	\
    ( (This)->lpVtbl -> put_Visible(This,Value) ) 

#define IWebBrowser2_get_StatusBar(This,pBool)	\
    ( (This)->lpVtbl -> get_StatusBar(This,pBool) ) 

#define IWebBrowser2_put_StatusBar(This,Value)	\
    ( (This)->lpVtbl -> put_StatusBar(This,Value) ) 

#define IWebBrowser2_get_StatusText(This,StatusText)	\
    ( (This)->lpVtbl -> get_StatusText(This,StatusText) ) 

#define IWebBrowser2_put_StatusText(This,StatusText)	\
    ( (This)->lpVtbl -> put_StatusText(This,StatusText) ) 

#define IWebBrowser2_get_ToolBar(This,Value)	\
    ( (This)->lpVtbl -> get_ToolBar(This,Value) ) 

#define IWebBrowser2_put_ToolBar(This,Value)	\
    ( (This)->lpVtbl -> put_ToolBar(This,Value) ) 

#define IWebBrowser2_get_MenuBar(This,Value)	\
    ( (This)->lpVtbl -> get_MenuBar(This,Value) ) 

#define IWebBrowser2_put_MenuBar(This,Value)	\
    ( (This)->lpVtbl -> put_MenuBar(This,Value) ) 

#define IWebBrowser2_get_FullScreen(This,pbFullScreen)	\
    ( (This)->lpVtbl -> get_FullScreen(This,pbFullScreen) ) 

#define IWebBrowser2_put_FullScreen(This,bFullScreen)	\
    ( (This)->lpVtbl -> put_FullScreen(This,bFullScreen) ) 


#define IWebBrowser2_Navigate2(This,URL,Flags,TargetFrameName,PostData,Headers)	\
    ( (This)->lpVtbl -> Navigate2(This,URL,Flags,TargetFrameName,PostData,Headers) ) 

#define IWebBrowser2_QueryStatusWB(This,cmdID,pcmdf)	\
    ( (This)->lpVtbl -> QueryStatusWB(This,cmdID,pcmdf) ) 

#define IWebBrowser2_ExecWB(This,cmdID,cmdexecopt,pvaIn,pvaOut)	\
    ( (This)->lpVtbl -> ExecWB(This,cmdID,cmdexecopt,pvaIn,pvaOut) ) 

#define IWebBrowser2_ShowBrowserBar(This,pvaClsid,pvarShow,pvarSize)	\
    ( (This)->lpVtbl -> ShowBrowserBar(This,pvaClsid,pvarShow,pvarSize) ) 

#define IWebBrowser2_get_ReadyState(This,plReadyState)	\
    ( (This)->lpVtbl -> get_ReadyState(This,plReadyState) ) 

#define IWebBrowser2_get_Offline(This,pbOffline)	\
    ( (This)->lpVtbl -> get_Offline(This,pbOffline) ) 

#define IWebBrowser2_put_Offline(This,bOffline)	\
    ( (This)->lpVtbl -> put_Offline(This,bOffline) ) 

#define IWebBrowser2_get_Silent(This,pbSilent)	\
    ( (This)->lpVtbl -> get_Silent(This,pbSilent) ) 

#define IWebBrowser2_put_Silent(This,bSilent)	\
    ( (This)->lpVtbl -> put_Silent(This,bSilent) ) 

#define IWebBrowser2_get_RegisterAsBrowser(This,pbRegister)	\
    ( (This)->lpVtbl -> get_RegisterAsBrowser(This,pbRegister) ) 

#define IWebBrowser2_put_RegisterAsBrowser(This,bRegister)	\
    ( (This)->lpVtbl -> put_RegisterAsBrowser(This,bRegister) ) 

#define IWebBrowser2_get_RegisterAsDropTarget(This,pbRegister)	\
    ( (This)->lpVtbl -> get_RegisterAsDropTarget(This,pbRegister) ) 

#define IWebBrowser2_put_RegisterAsDropTarget(This,bRegister)	\
    ( (This)->lpVtbl -> put_RegisterAsDropTarget(This,bRegister) ) 

#define IWebBrowser2_get_TheaterMode(This,pbRegister)	\
    ( (This)->lpVtbl -> get_TheaterMode(This,pbRegister) ) 

#define IWebBrowser2_put_TheaterMode(This,bRegister)	\
    ( (This)->lpVtbl -> put_TheaterMode(This,bRegister) ) 

#define IWebBrowser2_get_AddressBar(This,Value)	\
    ( (This)->lpVtbl -> get_AddressBar(This,Value) ) 

#define IWebBrowser2_put_AddressBar(This,Value)	\
    ( (This)->lpVtbl -> put_AddressBar(This,Value) ) 

#define IWebBrowser2_get_Resizable(This,Value)	\
    ( (This)->lpVtbl -> get_Resizable(This,Value) ) 

#define IWebBrowser2_put_Resizable(This,Value)	\
    ( (This)->lpVtbl -> put_Resizable(This,Value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWebBrowser2_INTERFACE_DEFINED__ */


#ifndef __DWebBrowserEvents2_DISPINTERFACE_DEFINED__
#define __DWebBrowserEvents2_DISPINTERFACE_DEFINED__

/* dispinterface DWebBrowserEvents2 */
/* [hidden][helpstring][uuid] */ 


EXTERN_C const IID DIID_DWebBrowserEvents2;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("34A715A0-6587-11D0-924A-0020AFC7AC4D")
    DWebBrowserEvents2 : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct DWebBrowserEvents2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            DWebBrowserEvents2 * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            DWebBrowserEvents2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            DWebBrowserEvents2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            DWebBrowserEvents2 * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            DWebBrowserEvents2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            DWebBrowserEvents2 * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            DWebBrowserEvents2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } DWebBrowserEvents2Vtbl;

    interface DWebBrowserEvents2
    {
        CONST_VTBL struct DWebBrowserEvents2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define DWebBrowserEvents2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define DWebBrowserEvents2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define DWebBrowserEvents2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define DWebBrowserEvents2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define DWebBrowserEvents2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define DWebBrowserEvents2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define DWebBrowserEvents2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __DWebBrowserEvents2_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_WebBrowser_V1;

#ifdef __cplusplus

class DECLSPEC_UUID("EAB22AC3-30C1-11CF-A7EB-0000C05BAE0B")
WebBrowser_V1;
#endif

EXTERN_C const CLSID CLSID_WebBrowser;

#ifdef __cplusplus

class DECLSPEC_UUID("8856F961-340A-11D0-A96B-00C04FD705A2")
WebBrowser;
#endif

EXTERN_C const CLSID CLSID_InternetExplorer;

#ifdef __cplusplus

class DECLSPEC_UUID("0002DF01-0000-0000-C000-000000000046")
InternetExplorer;
#endif

EXTERN_C const CLSID CLSID_ShellBrowserWindow;

#ifdef __cplusplus

class DECLSPEC_UUID("c08afd90-f2a1-11d1-8455-00a0c91f3880")
ShellBrowserWindow;
#endif

#ifndef __DShellWindowsEvents_DISPINTERFACE_DEFINED__
#define __DShellWindowsEvents_DISPINTERFACE_DEFINED__

/* dispinterface DShellWindowsEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID_DShellWindowsEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("fe4106e0-399a-11d0-a48c-00a0c90a8f39")
    DShellWindowsEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct DShellWindowsEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            DShellWindowsEvents * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            DShellWindowsEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            DShellWindowsEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            DShellWindowsEvents * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            DShellWindowsEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            DShellWindowsEvents * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            DShellWindowsEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } DShellWindowsEventsVtbl;

    interface DShellWindowsEvents
    {
        CONST_VTBL struct DShellWindowsEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define DShellWindowsEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define DShellWindowsEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define DShellWindowsEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define DShellWindowsEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define DShellWindowsEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define DShellWindowsEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define DShellWindowsEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __DShellWindowsEvents_DISPINTERFACE_DEFINED__ */


#ifndef __IShellWindows_INTERFACE_DEFINED__
#define __IShellWindows_INTERFACE_DEFINED__

/* interface IShellWindows */
/* [object][dual][oleautomation][helpstring][uuid] */ 


EXTERN_C const IID IID_IShellWindows;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("85CB6900-4D95-11CF-960C-0080C7F4EE85")
    IShellWindows : public IDispatch
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ __RPC__out long *Count) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Item( 
            /* [optional][in] */ VARIANT index,
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **Folder) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE _NewEnum( 
            /* [retval][out] */ __RPC__deref_out_opt IUnknown **ppunk) = 0;
        
        virtual /* [hidden][helpstring] */ HRESULT STDMETHODCALLTYPE Register( 
            /* [in] */ __RPC__in_opt IDispatch *pid,
            /* [in] */ long hwnd,
            /* [in] */ int swClass,
            /* [out] */ __RPC__out long *plCookie) = 0;
        
        virtual /* [hidden][helpstring] */ HRESULT STDMETHODCALLTYPE RegisterPending( 
            /* [in] */ long lThreadId,
            /* [in] */ __RPC__in VARIANT *pvarloc,
            /* [in] */ __RPC__in VARIANT *pvarlocRoot,
            /* [in] */ int swClass,
            /* [out] */ __RPC__out long *plCookie) = 0;
        
        virtual /* [hidden][helpstring] */ HRESULT STDMETHODCALLTYPE Revoke( 
            /* [in] */ long lCookie) = 0;
        
        virtual /* [hidden][helpstring] */ HRESULT STDMETHODCALLTYPE OnNavigate( 
            /* [in] */ long lCookie,
            /* [in] */ __RPC__in VARIANT *pvarLoc) = 0;
        
        virtual /* [hidden][helpstring] */ HRESULT STDMETHODCALLTYPE OnActivated( 
            /* [in] */ long lCookie,
            /* [in] */ VARIANT_BOOL fActive) = 0;
        
        virtual /* [hidden][helpstring] */ HRESULT STDMETHODCALLTYPE FindWindowSW( 
            /* [in] */ __RPC__in VARIANT *pvarLoc,
            /* [in] */ __RPC__in VARIANT *pvarLocRoot,
            /* [in] */ int swClass,
            /* [out] */ __RPC__out long *phwnd,
            /* [in] */ int swfwOptions,
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppdispOut) = 0;
        
        virtual /* [hidden][helpstring] */ HRESULT STDMETHODCALLTYPE OnCreated( 
            /* [in] */ long lCookie,
            /* [in] */ __RPC__in_opt IUnknown *punk) = 0;
        
        virtual /* [hidden][helpstring] */ HRESULT STDMETHODCALLTYPE ProcessAttachDetach( 
            /* [in] */ VARIANT_BOOL fAttach) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IShellWindowsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IShellWindows * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IShellWindows * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IShellWindows * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IShellWindows * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IShellWindows * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IShellWindows * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IShellWindows * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IShellWindows * This,
            /* [retval][out] */ __RPC__out long *Count);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Item )( 
            IShellWindows * This,
            /* [optional][in] */ VARIANT index,
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **Folder);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *_NewEnum )( 
            IShellWindows * This,
            /* [retval][out] */ __RPC__deref_out_opt IUnknown **ppunk);
        
        /* [hidden][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Register )( 
            IShellWindows * This,
            /* [in] */ __RPC__in_opt IDispatch *pid,
            /* [in] */ long hwnd,
            /* [in] */ int swClass,
            /* [out] */ __RPC__out long *plCookie);
        
        /* [hidden][helpstring] */ HRESULT ( STDMETHODCALLTYPE *RegisterPending )( 
            IShellWindows * This,
            /* [in] */ long lThreadId,
            /* [in] */ __RPC__in VARIANT *pvarloc,
            /* [in] */ __RPC__in VARIANT *pvarlocRoot,
            /* [in] */ int swClass,
            /* [out] */ __RPC__out long *plCookie);
        
        /* [hidden][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Revoke )( 
            IShellWindows * This,
            /* [in] */ long lCookie);
        
        /* [hidden][helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnNavigate )( 
            IShellWindows * This,
            /* [in] */ long lCookie,
            /* [in] */ __RPC__in VARIANT *pvarLoc);
        
        /* [hidden][helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnActivated )( 
            IShellWindows * This,
            /* [in] */ long lCookie,
            /* [in] */ VARIANT_BOOL fActive);
        
        /* [hidden][helpstring] */ HRESULT ( STDMETHODCALLTYPE *FindWindowSW )( 
            IShellWindows * This,
            /* [in] */ __RPC__in VARIANT *pvarLoc,
            /* [in] */ __RPC__in VARIANT *pvarLocRoot,
            /* [in] */ int swClass,
            /* [out] */ __RPC__out long *phwnd,
            /* [in] */ int swfwOptions,
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppdispOut);
        
        /* [hidden][helpstring] */ HRESULT ( STDMETHODCALLTYPE *OnCreated )( 
            IShellWindows * This,
            /* [in] */ long lCookie,
            /* [in] */ __RPC__in_opt IUnknown *punk);
        
        /* [hidden][helpstring] */ HRESULT ( STDMETHODCALLTYPE *ProcessAttachDetach )( 
            IShellWindows * This,
            /* [in] */ VARIANT_BOOL fAttach);
        
        END_INTERFACE
    } IShellWindowsVtbl;

    interface IShellWindows
    {
        CONST_VTBL struct IShellWindowsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IShellWindows_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IShellWindows_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IShellWindows_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IShellWindows_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IShellWindows_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IShellWindows_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IShellWindows_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IShellWindows_get_Count(This,Count)	\
    ( (This)->lpVtbl -> get_Count(This,Count) ) 

#define IShellWindows_Item(This,index,Folder)	\
    ( (This)->lpVtbl -> Item(This,index,Folder) ) 

#define IShellWindows__NewEnum(This,ppunk)	\
    ( (This)->lpVtbl -> _NewEnum(This,ppunk) ) 

#define IShellWindows_Register(This,pid,hwnd,swClass,plCookie)	\
    ( (This)->lpVtbl -> Register(This,pid,hwnd,swClass,plCookie) ) 

#define IShellWindows_RegisterPending(This,lThreadId,pvarloc,pvarlocRoot,swClass,plCookie)	\
    ( (This)->lpVtbl -> RegisterPending(This,lThreadId,pvarloc,pvarlocRoot,swClass,plCookie) ) 

#define IShellWindows_Revoke(This,lCookie)	\
    ( (This)->lpVtbl -> Revoke(This,lCookie) ) 

#define IShellWindows_OnNavigate(This,lCookie,pvarLoc)	\
    ( (This)->lpVtbl -> OnNavigate(This,lCookie,pvarLoc) ) 

#define IShellWindows_OnActivated(This,lCookie,fActive)	\
    ( (This)->lpVtbl -> OnActivated(This,lCookie,fActive) ) 

#define IShellWindows_FindWindowSW(This,pvarLoc,pvarLocRoot,swClass,phwnd,swfwOptions,ppdispOut)	\
    ( (This)->lpVtbl -> FindWindowSW(This,pvarLoc,pvarLocRoot,swClass,phwnd,swfwOptions,ppdispOut) ) 

#define IShellWindows_OnCreated(This,lCookie,punk)	\
    ( (This)->lpVtbl -> OnCreated(This,lCookie,punk) ) 

#define IShellWindows_ProcessAttachDetach(This,fAttach)	\
    ( (This)->lpVtbl -> ProcessAttachDetach(This,fAttach) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IShellWindows_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ShellWindows;

#ifdef __cplusplus

class DECLSPEC_UUID("9BA05972-F6A8-11CF-A442-00A0C90A8F39")
ShellWindows;
#endif

#ifndef __IShellUIHelper_INTERFACE_DEFINED__
#define __IShellUIHelper_INTERFACE_DEFINED__

/* interface IShellUIHelper */
/* [object][dual][oleautomation][helpstring][uuid] */ 


EXTERN_C const IID IID_IShellUIHelper;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("729FE2F8-1EA8-11d1-8F85-00C04FC2FBE1")
    IShellUIHelper : public IDispatch
    {
    public:
        virtual /* [id][hidden] */ HRESULT STDMETHODCALLTYPE ResetFirstBootMode( void) = 0;
        
        virtual /* [id][hidden] */ HRESULT STDMETHODCALLTYPE ResetSafeMode( void) = 0;
        
        virtual /* [id][hidden] */ HRESULT STDMETHODCALLTYPE RefreshOfflineDesktop( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddFavorite( 
            /* [in] */ __RPC__in BSTR URL,
            /* [in][optional] */ __RPC__in VARIANT *Title) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddChannel( 
            /* [in] */ __RPC__in BSTR URL) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddDesktopComponent( 
            /* [in] */ __RPC__in BSTR URL,
            /* [in] */ __RPC__in BSTR Type,
            /* [in][optional] */ __RPC__in VARIANT *Left,
            /* [in][optional] */ __RPC__in VARIANT *Top,
            /* [in][optional] */ __RPC__in VARIANT *Width,
            /* [in][optional] */ __RPC__in VARIANT *Height) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE IsSubscribed( 
            /* [in] */ __RPC__in BSTR URL,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE NavigateAndFind( 
            /* [in] */ __RPC__in BSTR URL,
            /* [in] */ __RPC__in BSTR strQuery,
            /* [in] */ __RPC__in VARIANT *varTargetFrame) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ImportExportFavorites( 
            /* [in] */ VARIANT_BOOL fImport,
            /* [in] */ __RPC__in BSTR strImpExpPath) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AutoCompleteSaveForm( 
            /* [in][optional] */ __RPC__in VARIANT *Form) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AutoScan( 
            /* [in] */ __RPC__in BSTR strSearch,
            /* [in] */ __RPC__in BSTR strFailureUrl,
            /* [in][optional] */ __RPC__in VARIANT *pvarTargetFrame) = 0;
        
        virtual /* [id][hidden] */ HRESULT STDMETHODCALLTYPE AutoCompleteAttach( 
            /* [in][optional] */ __RPC__in VARIANT *Reserved) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ShowBrowserUI( 
            /* [in] */ __RPC__in BSTR bstrName,
            /* [in] */ __RPC__in VARIANT *pvarIn,
            /* [retval][out] */ __RPC__out VARIANT *pvarOut) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IShellUIHelperVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IShellUIHelper * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IShellUIHelper * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IShellUIHelper * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IShellUIHelper * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IShellUIHelper * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IShellUIHelper * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IShellUIHelper * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][hidden] */ HRESULT ( STDMETHODCALLTYPE *ResetFirstBootMode )( 
            IShellUIHelper * This);
        
        /* [id][hidden] */ HRESULT ( STDMETHODCALLTYPE *ResetSafeMode )( 
            IShellUIHelper * This);
        
        /* [id][hidden] */ HRESULT ( STDMETHODCALLTYPE *RefreshOfflineDesktop )( 
            IShellUIHelper * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AddFavorite )( 
            IShellUIHelper * This,
            /* [in] */ __RPC__in BSTR URL,
            /* [in][optional] */ __RPC__in VARIANT *Title);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AddChannel )( 
            IShellUIHelper * This,
            /* [in] */ __RPC__in BSTR URL);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AddDesktopComponent )( 
            IShellUIHelper * This,
            /* [in] */ __RPC__in BSTR URL,
            /* [in] */ __RPC__in BSTR Type,
            /* [in][optional] */ __RPC__in VARIANT *Left,
            /* [in][optional] */ __RPC__in VARIANT *Top,
            /* [in][optional] */ __RPC__in VARIANT *Width,
            /* [in][optional] */ __RPC__in VARIANT *Height);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *IsSubscribed )( 
            IShellUIHelper * This,
            /* [in] */ __RPC__in BSTR URL,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *NavigateAndFind )( 
            IShellUIHelper * This,
            /* [in] */ __RPC__in BSTR URL,
            /* [in] */ __RPC__in BSTR strQuery,
            /* [in] */ __RPC__in VARIANT *varTargetFrame);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ImportExportFavorites )( 
            IShellUIHelper * This,
            /* [in] */ VARIANT_BOOL fImport,
            /* [in] */ __RPC__in BSTR strImpExpPath);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AutoCompleteSaveForm )( 
            IShellUIHelper * This,
            /* [in][optional] */ __RPC__in VARIANT *Form);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AutoScan )( 
            IShellUIHelper * This,
            /* [in] */ __RPC__in BSTR strSearch,
            /* [in] */ __RPC__in BSTR strFailureUrl,
            /* [in][optional] */ __RPC__in VARIANT *pvarTargetFrame);
        
        /* [id][hidden] */ HRESULT ( STDMETHODCALLTYPE *AutoCompleteAttach )( 
            IShellUIHelper * This,
            /* [in][optional] */ __RPC__in VARIANT *Reserved);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ShowBrowserUI )( 
            IShellUIHelper * This,
            /* [in] */ __RPC__in BSTR bstrName,
            /* [in] */ __RPC__in VARIANT *pvarIn,
            /* [retval][out] */ __RPC__out VARIANT *pvarOut);
        
        END_INTERFACE
    } IShellUIHelperVtbl;

    interface IShellUIHelper
    {
        CONST_VTBL struct IShellUIHelperVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IShellUIHelper_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IShellUIHelper_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IShellUIHelper_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IShellUIHelper_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IShellUIHelper_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IShellUIHelper_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IShellUIHelper_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IShellUIHelper_ResetFirstBootMode(This)	\
    ( (This)->lpVtbl -> ResetFirstBootMode(This) ) 

#define IShellUIHelper_ResetSafeMode(This)	\
    ( (This)->lpVtbl -> ResetSafeMode(This) ) 

#define IShellUIHelper_RefreshOfflineDesktop(This)	\
    ( (This)->lpVtbl -> RefreshOfflineDesktop(This) ) 

#define IShellUIHelper_AddFavorite(This,URL,Title)	\
    ( (This)->lpVtbl -> AddFavorite(This,URL,Title) ) 

#define IShellUIHelper_AddChannel(This,URL)	\
    ( (This)->lpVtbl -> AddChannel(This,URL) ) 

#define IShellUIHelper_AddDesktopComponent(This,URL,Type,Left,Top,Width,Height)	\
    ( (This)->lpVtbl -> AddDesktopComponent(This,URL,Type,Left,Top,Width,Height) ) 

#define IShellUIHelper_IsSubscribed(This,URL,pBool)	\
    ( (This)->lpVtbl -> IsSubscribed(This,URL,pBool) ) 

#define IShellUIHelper_NavigateAndFind(This,URL,strQuery,varTargetFrame)	\
    ( (This)->lpVtbl -> NavigateAndFind(This,URL,strQuery,varTargetFrame) ) 

#define IShellUIHelper_ImportExportFavorites(This,fImport,strImpExpPath)	\
    ( (This)->lpVtbl -> ImportExportFavorites(This,fImport,strImpExpPath) ) 

#define IShellUIHelper_AutoCompleteSaveForm(This,Form)	\
    ( (This)->lpVtbl -> AutoCompleteSaveForm(This,Form) ) 

#define IShellUIHelper_AutoScan(This,strSearch,strFailureUrl,pvarTargetFrame)	\
    ( (This)->lpVtbl -> AutoScan(This,strSearch,strFailureUrl,pvarTargetFrame) ) 

#define IShellUIHelper_AutoCompleteAttach(This,Reserved)	\
    ( (This)->lpVtbl -> AutoCompleteAttach(This,Reserved) ) 

#define IShellUIHelper_ShowBrowserUI(This,bstrName,pvarIn,pvarOut)	\
    ( (This)->lpVtbl -> ShowBrowserUI(This,bstrName,pvarIn,pvarOut) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IShellUIHelper_INTERFACE_DEFINED__ */


#ifndef __IShellUIHelper2_INTERFACE_DEFINED__
#define __IShellUIHelper2_INTERFACE_DEFINED__

/* interface IShellUIHelper2 */
/* [object][dual][oleautomation][helpstring][uuid] */ 


EXTERN_C const IID IID_IShellUIHelper2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("a7fe6eda-1932-4281-b881-87b31b8bc52c")
    IShellUIHelper2 : public IShellUIHelper
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddSearchProvider( 
            /* [in] */ __RPC__in BSTR URL) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RunOnceShown( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SkipRunOnce( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CustomizeSettings( 
            /* [in] */ VARIANT_BOOL fSQM,
            /* [in] */ VARIANT_BOOL fPhishing,
            /* [in] */ __RPC__in BSTR bstrLocale) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SqmEnabled( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pfEnabled) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE PhishingEnabled( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pfEnabled) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE BrandImageUri( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pbstrUri) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SkipTabsWelcome( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DiagnoseConnection( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CustomizeClearType( 
            /* [in] */ VARIANT_BOOL fSet) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE IsSearchProviderInstalled( 
            /* [in] */ __RPC__in BSTR URL,
            /* [retval][out] */ __RPC__out DWORD *pdwResult) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE IsSearchMigrated( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pfMigrated) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DefaultSearchProvider( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pbstrName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RunOnceRequiredSettingsComplete( 
            /* [in] */ VARIANT_BOOL fComplete) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RunOnceHasShown( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pfShown) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SearchGuideUrl( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pbstrUrl) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IShellUIHelper2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IShellUIHelper2 * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IShellUIHelper2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IShellUIHelper2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IShellUIHelper2 * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IShellUIHelper2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IShellUIHelper2 * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IShellUIHelper2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][hidden] */ HRESULT ( STDMETHODCALLTYPE *ResetFirstBootMode )( 
            IShellUIHelper2 * This);
        
        /* [id][hidden] */ HRESULT ( STDMETHODCALLTYPE *ResetSafeMode )( 
            IShellUIHelper2 * This);
        
        /* [id][hidden] */ HRESULT ( STDMETHODCALLTYPE *RefreshOfflineDesktop )( 
            IShellUIHelper2 * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AddFavorite )( 
            IShellUIHelper2 * This,
            /* [in] */ __RPC__in BSTR URL,
            /* [in][optional] */ __RPC__in VARIANT *Title);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AddChannel )( 
            IShellUIHelper2 * This,
            /* [in] */ __RPC__in BSTR URL);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AddDesktopComponent )( 
            IShellUIHelper2 * This,
            /* [in] */ __RPC__in BSTR URL,
            /* [in] */ __RPC__in BSTR Type,
            /* [in][optional] */ __RPC__in VARIANT *Left,
            /* [in][optional] */ __RPC__in VARIANT *Top,
            /* [in][optional] */ __RPC__in VARIANT *Width,
            /* [in][optional] */ __RPC__in VARIANT *Height);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *IsSubscribed )( 
            IShellUIHelper2 * This,
            /* [in] */ __RPC__in BSTR URL,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *NavigateAndFind )( 
            IShellUIHelper2 * This,
            /* [in] */ __RPC__in BSTR URL,
            /* [in] */ __RPC__in BSTR strQuery,
            /* [in] */ __RPC__in VARIANT *varTargetFrame);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ImportExportFavorites )( 
            IShellUIHelper2 * This,
            /* [in] */ VARIANT_BOOL fImport,
            /* [in] */ __RPC__in BSTR strImpExpPath);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AutoCompleteSaveForm )( 
            IShellUIHelper2 * This,
            /* [in][optional] */ __RPC__in VARIANT *Form);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AutoScan )( 
            IShellUIHelper2 * This,
            /* [in] */ __RPC__in BSTR strSearch,
            /* [in] */ __RPC__in BSTR strFailureUrl,
            /* [in][optional] */ __RPC__in VARIANT *pvarTargetFrame);
        
        /* [id][hidden] */ HRESULT ( STDMETHODCALLTYPE *AutoCompleteAttach )( 
            IShellUIHelper2 * This,
            /* [in][optional] */ __RPC__in VARIANT *Reserved);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ShowBrowserUI )( 
            IShellUIHelper2 * This,
            /* [in] */ __RPC__in BSTR bstrName,
            /* [in] */ __RPC__in VARIANT *pvarIn,
            /* [retval][out] */ __RPC__out VARIANT *pvarOut);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AddSearchProvider )( 
            IShellUIHelper2 * This,
            /* [in] */ __RPC__in BSTR URL);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RunOnceShown )( 
            IShellUIHelper2 * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SkipRunOnce )( 
            IShellUIHelper2 * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CustomizeSettings )( 
            IShellUIHelper2 * This,
            /* [in] */ VARIANT_BOOL fSQM,
            /* [in] */ VARIANT_BOOL fPhishing,
            /* [in] */ __RPC__in BSTR bstrLocale);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SqmEnabled )( 
            IShellUIHelper2 * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pfEnabled);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *PhishingEnabled )( 
            IShellUIHelper2 * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pfEnabled);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *BrandImageUri )( 
            IShellUIHelper2 * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pbstrUri);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SkipTabsWelcome )( 
            IShellUIHelper2 * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DiagnoseConnection )( 
            IShellUIHelper2 * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CustomizeClearType )( 
            IShellUIHelper2 * This,
            /* [in] */ VARIANT_BOOL fSet);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *IsSearchProviderInstalled )( 
            IShellUIHelper2 * This,
            /* [in] */ __RPC__in BSTR URL,
            /* [retval][out] */ __RPC__out DWORD *pdwResult);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *IsSearchMigrated )( 
            IShellUIHelper2 * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pfMigrated);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DefaultSearchProvider )( 
            IShellUIHelper2 * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pbstrName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RunOnceRequiredSettingsComplete )( 
            IShellUIHelper2 * This,
            /* [in] */ VARIANT_BOOL fComplete);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RunOnceHasShown )( 
            IShellUIHelper2 * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pfShown);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SearchGuideUrl )( 
            IShellUIHelper2 * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pbstrUrl);
        
        END_INTERFACE
    } IShellUIHelper2Vtbl;

    interface IShellUIHelper2
    {
        CONST_VTBL struct IShellUIHelper2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IShellUIHelper2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IShellUIHelper2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IShellUIHelper2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IShellUIHelper2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IShellUIHelper2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IShellUIHelper2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IShellUIHelper2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IShellUIHelper2_ResetFirstBootMode(This)	\
    ( (This)->lpVtbl -> ResetFirstBootMode(This) ) 

#define IShellUIHelper2_ResetSafeMode(This)	\
    ( (This)->lpVtbl -> ResetSafeMode(This) ) 

#define IShellUIHelper2_RefreshOfflineDesktop(This)	\
    ( (This)->lpVtbl -> RefreshOfflineDesktop(This) ) 

#define IShellUIHelper2_AddFavorite(This,URL,Title)	\
    ( (This)->lpVtbl -> AddFavorite(This,URL,Title) ) 

#define IShellUIHelper2_AddChannel(This,URL)	\
    ( (This)->lpVtbl -> AddChannel(This,URL) ) 

#define IShellUIHelper2_AddDesktopComponent(This,URL,Type,Left,Top,Width,Height)	\
    ( (This)->lpVtbl -> AddDesktopComponent(This,URL,Type,Left,Top,Width,Height) ) 

#define IShellUIHelper2_IsSubscribed(This,URL,pBool)	\
    ( (This)->lpVtbl -> IsSubscribed(This,URL,pBool) ) 

#define IShellUIHelper2_NavigateAndFind(This,URL,strQuery,varTargetFrame)	\
    ( (This)->lpVtbl -> NavigateAndFind(This,URL,strQuery,varTargetFrame) ) 

#define IShellUIHelper2_ImportExportFavorites(This,fImport,strImpExpPath)	\
    ( (This)->lpVtbl -> ImportExportFavorites(This,fImport,strImpExpPath) ) 

#define IShellUIHelper2_AutoCompleteSaveForm(This,Form)	\
    ( (This)->lpVtbl -> AutoCompleteSaveForm(This,Form) ) 

#define IShellUIHelper2_AutoScan(This,strSearch,strFailureUrl,pvarTargetFrame)	\
    ( (This)->lpVtbl -> AutoScan(This,strSearch,strFailureUrl,pvarTargetFrame) ) 

#define IShellUIHelper2_AutoCompleteAttach(This,Reserved)	\
    ( (This)->lpVtbl -> AutoCompleteAttach(This,Reserved) ) 

#define IShellUIHelper2_ShowBrowserUI(This,bstrName,pvarIn,pvarOut)	\
    ( (This)->lpVtbl -> ShowBrowserUI(This,bstrName,pvarIn,pvarOut) ) 


#define IShellUIHelper2_AddSearchProvider(This,URL)	\
    ( (This)->lpVtbl -> AddSearchProvider(This,URL) ) 

#define IShellUIHelper2_RunOnceShown(This)	\
    ( (This)->lpVtbl -> RunOnceShown(This) ) 

#define IShellUIHelper2_SkipRunOnce(This)	\
    ( (This)->lpVtbl -> SkipRunOnce(This) ) 

#define IShellUIHelper2_CustomizeSettings(This,fSQM,fPhishing,bstrLocale)	\
    ( (This)->lpVtbl -> CustomizeSettings(This,fSQM,fPhishing,bstrLocale) ) 

#define IShellUIHelper2_SqmEnabled(This,pfEnabled)	\
    ( (This)->lpVtbl -> SqmEnabled(This,pfEnabled) ) 

#define IShellUIHelper2_PhishingEnabled(This,pfEnabled)	\
    ( (This)->lpVtbl -> PhishingEnabled(This,pfEnabled) ) 

#define IShellUIHelper2_BrandImageUri(This,pbstrUri)	\
    ( (This)->lpVtbl -> BrandImageUri(This,pbstrUri) ) 

#define IShellUIHelper2_SkipTabsWelcome(This)	\
    ( (This)->lpVtbl -> SkipTabsWelcome(This) ) 

#define IShellUIHelper2_DiagnoseConnection(This)	\
    ( (This)->lpVtbl -> DiagnoseConnection(This) ) 

#define IShellUIHelper2_CustomizeClearType(This,fSet)	\
    ( (This)->lpVtbl -> CustomizeClearType(This,fSet) ) 

#define IShellUIHelper2_IsSearchProviderInstalled(This,URL,pdwResult)	\
    ( (This)->lpVtbl -> IsSearchProviderInstalled(This,URL,pdwResult) ) 

#define IShellUIHelper2_IsSearchMigrated(This,pfMigrated)	\
    ( (This)->lpVtbl -> IsSearchMigrated(This,pfMigrated) ) 

#define IShellUIHelper2_DefaultSearchProvider(This,pbstrName)	\
    ( (This)->lpVtbl -> DefaultSearchProvider(This,pbstrName) ) 

#define IShellUIHelper2_RunOnceRequiredSettingsComplete(This,fComplete)	\
    ( (This)->lpVtbl -> RunOnceRequiredSettingsComplete(This,fComplete) ) 

#define IShellUIHelper2_RunOnceHasShown(This,pfShown)	\
    ( (This)->lpVtbl -> RunOnceHasShown(This,pfShown) ) 

#define IShellUIHelper2_SearchGuideUrl(This,pbstrUrl)	\
    ( (This)->lpVtbl -> SearchGuideUrl(This,pbstrUrl) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IShellUIHelper2_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ShellUIHelper;

#ifdef __cplusplus

class DECLSPEC_UUID("64AB4BB7-111E-11d1-8F79-00C04FC2FBE1")
ShellUIHelper;
#endif

#ifndef __DShellNameSpaceEvents_DISPINTERFACE_DEFINED__
#define __DShellNameSpaceEvents_DISPINTERFACE_DEFINED__

/* dispinterface DShellNameSpaceEvents */
/* [uuid] */ 


EXTERN_C const IID DIID_DShellNameSpaceEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("55136806-B2DE-11D1-B9F2-00A0C98BC547")
    DShellNameSpaceEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct DShellNameSpaceEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            DShellNameSpaceEvents * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            DShellNameSpaceEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            DShellNameSpaceEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            DShellNameSpaceEvents * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            DShellNameSpaceEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            DShellNameSpaceEvents * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            DShellNameSpaceEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } DShellNameSpaceEventsVtbl;

    interface DShellNameSpaceEvents
    {
        CONST_VTBL struct DShellNameSpaceEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define DShellNameSpaceEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define DShellNameSpaceEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define DShellNameSpaceEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define DShellNameSpaceEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define DShellNameSpaceEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define DShellNameSpaceEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define DShellNameSpaceEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __DShellNameSpaceEvents_DISPINTERFACE_DEFINED__ */


#ifndef __IShellFavoritesNameSpace_INTERFACE_DEFINED__
#define __IShellFavoritesNameSpace_INTERFACE_DEFINED__

/* interface IShellFavoritesNameSpace */
/* [hidden][unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IShellFavoritesNameSpace;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("55136804-B2DE-11D1-B9F2-00A0C98BC547")
    IShellFavoritesNameSpace : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveSelectionUp( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveSelectionDown( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ResetSort( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE NewFolder( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Synchronize( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Import( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Export( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE InvokeContextMenuCommand( 
            /* [in] */ __RPC__in BSTR strCommand) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveSelectionTo( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SubscriptionsEnabled( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateSubscriptionForSelection( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteSubscriptionForSelection( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetRoot( 
            /* [in] */ __RPC__in BSTR bstrFullPath) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IShellFavoritesNameSpaceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IShellFavoritesNameSpace * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IShellFavoritesNameSpace * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IShellFavoritesNameSpace * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IShellFavoritesNameSpace * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IShellFavoritesNameSpace * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IShellFavoritesNameSpace * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IShellFavoritesNameSpace * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveSelectionUp )( 
            IShellFavoritesNameSpace * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveSelectionDown )( 
            IShellFavoritesNameSpace * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSort )( 
            IShellFavoritesNameSpace * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *NewFolder )( 
            IShellFavoritesNameSpace * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Synchronize )( 
            IShellFavoritesNameSpace * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Import )( 
            IShellFavoritesNameSpace * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Export )( 
            IShellFavoritesNameSpace * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *InvokeContextMenuCommand )( 
            IShellFavoritesNameSpace * This,
            /* [in] */ __RPC__in BSTR strCommand);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveSelectionTo )( 
            IShellFavoritesNameSpace * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SubscriptionsEnabled )( 
            IShellFavoritesNameSpace * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateSubscriptionForSelection )( 
            IShellFavoritesNameSpace * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteSubscriptionForSelection )( 
            IShellFavoritesNameSpace * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetRoot )( 
            IShellFavoritesNameSpace * This,
            /* [in] */ __RPC__in BSTR bstrFullPath);
        
        END_INTERFACE
    } IShellFavoritesNameSpaceVtbl;

    interface IShellFavoritesNameSpace
    {
        CONST_VTBL struct IShellFavoritesNameSpaceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IShellFavoritesNameSpace_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IShellFavoritesNameSpace_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IShellFavoritesNameSpace_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IShellFavoritesNameSpace_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IShellFavoritesNameSpace_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IShellFavoritesNameSpace_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IShellFavoritesNameSpace_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IShellFavoritesNameSpace_MoveSelectionUp(This)	\
    ( (This)->lpVtbl -> MoveSelectionUp(This) ) 

#define IShellFavoritesNameSpace_MoveSelectionDown(This)	\
    ( (This)->lpVtbl -> MoveSelectionDown(This) ) 

#define IShellFavoritesNameSpace_ResetSort(This)	\
    ( (This)->lpVtbl -> ResetSort(This) ) 

#define IShellFavoritesNameSpace_NewFolder(This)	\
    ( (This)->lpVtbl -> NewFolder(This) ) 

#define IShellFavoritesNameSpace_Synchronize(This)	\
    ( (This)->lpVtbl -> Synchronize(This) ) 

#define IShellFavoritesNameSpace_Import(This)	\
    ( (This)->lpVtbl -> Import(This) ) 

#define IShellFavoritesNameSpace_Export(This)	\
    ( (This)->lpVtbl -> Export(This) ) 

#define IShellFavoritesNameSpace_InvokeContextMenuCommand(This,strCommand)	\
    ( (This)->lpVtbl -> InvokeContextMenuCommand(This,strCommand) ) 

#define IShellFavoritesNameSpace_MoveSelectionTo(This)	\
    ( (This)->lpVtbl -> MoveSelectionTo(This) ) 

#define IShellFavoritesNameSpace_get_SubscriptionsEnabled(This,pBool)	\
    ( (This)->lpVtbl -> get_SubscriptionsEnabled(This,pBool) ) 

#define IShellFavoritesNameSpace_CreateSubscriptionForSelection(This,pBool)	\
    ( (This)->lpVtbl -> CreateSubscriptionForSelection(This,pBool) ) 

#define IShellFavoritesNameSpace_DeleteSubscriptionForSelection(This,pBool)	\
    ( (This)->lpVtbl -> DeleteSubscriptionForSelection(This,pBool) ) 

#define IShellFavoritesNameSpace_SetRoot(This,bstrFullPath)	\
    ( (This)->lpVtbl -> SetRoot(This,bstrFullPath) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IShellFavoritesNameSpace_INTERFACE_DEFINED__ */


#ifndef __IShellNameSpace_INTERFACE_DEFINED__
#define __IShellNameSpace_INTERFACE_DEFINED__

/* interface IShellNameSpace */
/* [hidden][unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IShellNameSpace;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e572d3c9-37be-4ae2-825d-d521763e3108")
    IShellNameSpace : public IShellFavoritesNameSpace
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_EnumOptions( 
            /* [retval][out] */ __RPC__out LONG *pgrfEnumFlags) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_EnumOptions( 
            /* [in] */ LONG lVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SelectedItem( 
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **pItem) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SelectedItem( 
            /* [in] */ __RPC__in_opt IDispatch *pItem) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Root( 
            /* [retval][out] */ __RPC__out VARIANT *pvar) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Root( 
            /* [in] */ VARIANT var) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Depth( 
            /* [retval][out] */ __RPC__out int *piDepth) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Depth( 
            /* [in] */ int iDepth) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Mode( 
            /* [retval][out] */ __RPC__out UINT *puMode) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Mode( 
            /* [in] */ UINT uMode) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Flags( 
            /* [retval][out] */ __RPC__out DWORD *pdwFlags) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Flags( 
            /* [in] */ DWORD dwFlags) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TVFlags( 
            /* [in] */ DWORD dwFlags) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TVFlags( 
            /* [retval][out] */ __RPC__out DWORD *dwFlags) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Columns( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *bstrColumns) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Columns( 
            /* [in] */ __RPC__in BSTR bstrColumns) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CountViewTypes( 
            /* [retval][out] */ __RPC__out int *piTypes) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetViewType( 
            /* [in] */ int iType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SelectedItems( 
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppid) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Expand( 
            /* [in] */ VARIANT var,
            int iDepth) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UnselectAll( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IShellNameSpaceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IShellNameSpace * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IShellNameSpace * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IShellNameSpace * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IShellNameSpace * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IShellNameSpace * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IShellNameSpace * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IShellNameSpace * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveSelectionUp )( 
            IShellNameSpace * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveSelectionDown )( 
            IShellNameSpace * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSort )( 
            IShellNameSpace * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *NewFolder )( 
            IShellNameSpace * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Synchronize )( 
            IShellNameSpace * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Import )( 
            IShellNameSpace * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Export )( 
            IShellNameSpace * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *InvokeContextMenuCommand )( 
            IShellNameSpace * This,
            /* [in] */ __RPC__in BSTR strCommand);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveSelectionTo )( 
            IShellNameSpace * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SubscriptionsEnabled )( 
            IShellNameSpace * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateSubscriptionForSelection )( 
            IShellNameSpace * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteSubscriptionForSelection )( 
            IShellNameSpace * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pBool);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetRoot )( 
            IShellNameSpace * This,
            /* [in] */ __RPC__in BSTR bstrFullPath);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_EnumOptions )( 
            IShellNameSpace * This,
            /* [retval][out] */ __RPC__out LONG *pgrfEnumFlags);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_EnumOptions )( 
            IShellNameSpace * This,
            /* [in] */ LONG lVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SelectedItem )( 
            IShellNameSpace * This,
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **pItem);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SelectedItem )( 
            IShellNameSpace * This,
            /* [in] */ __RPC__in_opt IDispatch *pItem);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Root )( 
            IShellNameSpace * This,
            /* [retval][out] */ __RPC__out VARIANT *pvar);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Root )( 
            IShellNameSpace * This,
            /* [in] */ VARIANT var);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Depth )( 
            IShellNameSpace * This,
            /* [retval][out] */ __RPC__out int *piDepth);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Depth )( 
            IShellNameSpace * This,
            /* [in] */ int iDepth);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Mode )( 
            IShellNameSpace * This,
            /* [retval][out] */ __RPC__out UINT *puMode);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Mode )( 
            IShellNameSpace * This,
            /* [in] */ UINT uMode);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Flags )( 
            IShellNameSpace * This,
            /* [retval][out] */ __RPC__out DWORD *pdwFlags);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Flags )( 
            IShellNameSpace * This,
            /* [in] */ DWORD dwFlags);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TVFlags )( 
            IShellNameSpace * This,
            /* [in] */ DWORD dwFlags);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TVFlags )( 
            IShellNameSpace * This,
            /* [retval][out] */ __RPC__out DWORD *dwFlags);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Columns )( 
            IShellNameSpace * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *bstrColumns);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Columns )( 
            IShellNameSpace * This,
            /* [in] */ __RPC__in BSTR bstrColumns);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CountViewTypes )( 
            IShellNameSpace * This,
            /* [retval][out] */ __RPC__out int *piTypes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetViewType )( 
            IShellNameSpace * This,
            /* [in] */ int iType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SelectedItems )( 
            IShellNameSpace * This,
            /* [retval][out] */ __RPC__deref_out_opt IDispatch **ppid);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Expand )( 
            IShellNameSpace * This,
            /* [in] */ VARIANT var,
            int iDepth);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *UnselectAll )( 
            IShellNameSpace * This);
        
        END_INTERFACE
    } IShellNameSpaceVtbl;

    interface IShellNameSpace
    {
        CONST_VTBL struct IShellNameSpaceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IShellNameSpace_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IShellNameSpace_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IShellNameSpace_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IShellNameSpace_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IShellNameSpace_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IShellNameSpace_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IShellNameSpace_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IShellNameSpace_MoveSelectionUp(This)	\
    ( (This)->lpVtbl -> MoveSelectionUp(This) ) 

#define IShellNameSpace_MoveSelectionDown(This)	\
    ( (This)->lpVtbl -> MoveSelectionDown(This) ) 

#define IShellNameSpace_ResetSort(This)	\
    ( (This)->lpVtbl -> ResetSort(This) ) 

#define IShellNameSpace_NewFolder(This)	\
    ( (This)->lpVtbl -> NewFolder(This) ) 

#define IShellNameSpace_Synchronize(This)	\
    ( (This)->lpVtbl -> Synchronize(This) ) 

#define IShellNameSpace_Import(This)	\
    ( (This)->lpVtbl -> Import(This) ) 

#define IShellNameSpace_Export(This)	\
    ( (This)->lpVtbl -> Export(This) ) 

#define IShellNameSpace_InvokeContextMenuCommand(This,strCommand)	\
    ( (This)->lpVtbl -> InvokeContextMenuCommand(This,strCommand) ) 

#define IShellNameSpace_MoveSelectionTo(This)	\
    ( (This)->lpVtbl -> MoveSelectionTo(This) ) 

#define IShellNameSpace_get_SubscriptionsEnabled(This,pBool)	\
    ( (This)->lpVtbl -> get_SubscriptionsEnabled(This,pBool) ) 

#define IShellNameSpace_CreateSubscriptionForSelection(This,pBool)	\
    ( (This)->lpVtbl -> CreateSubscriptionForSelection(This,pBool) ) 

#define IShellNameSpace_DeleteSubscriptionForSelection(This,pBool)	\
    ( (This)->lpVtbl -> DeleteSubscriptionForSelection(This,pBool) ) 

#define IShellNameSpace_SetRoot(This,bstrFullPath)	\
    ( (This)->lpVtbl -> SetRoot(This,bstrFullPath) ) 


#define IShellNameSpace_get_EnumOptions(This,pgrfEnumFlags)	\
    ( (This)->lpVtbl -> get_EnumOptions(This,pgrfEnumFlags) ) 

#define IShellNameSpace_put_EnumOptions(This,lVal)	\
    ( (This)->lpVtbl -> put_EnumOptions(This,lVal) ) 

#define IShellNameSpace_get_SelectedItem(This,pItem)	\
    ( (This)->lpVtbl -> get_SelectedItem(This,pItem) ) 

#define IShellNameSpace_put_SelectedItem(This,pItem)	\
    ( (This)->lpVtbl -> put_SelectedItem(This,pItem) ) 

#define IShellNameSpace_get_Root(This,pvar)	\
    ( (This)->lpVtbl -> get_Root(This,pvar) ) 

#define IShellNameSpace_put_Root(This,var)	\
    ( (This)->lpVtbl -> put_Root(This,var) ) 

#define IShellNameSpace_get_Depth(This,piDepth)	\
    ( (This)->lpVtbl -> get_Depth(This,piDepth) ) 

#define IShellNameSpace_put_Depth(This,iDepth)	\
    ( (This)->lpVtbl -> put_Depth(This,iDepth) ) 

#define IShellNameSpace_get_Mode(This,puMode)	\
    ( (This)->lpVtbl -> get_Mode(This,puMode) ) 

#define IShellNameSpace_put_Mode(This,uMode)	\
    ( (This)->lpVtbl -> put_Mode(This,uMode) ) 

#define IShellNameSpace_get_Flags(This,pdwFlags)	\
    ( (This)->lpVtbl -> get_Flags(This,pdwFlags) ) 

#define IShellNameSpace_put_Flags(This,dwFlags)	\
    ( (This)->lpVtbl -> put_Flags(This,dwFlags) ) 

#define IShellNameSpace_put_TVFlags(This,dwFlags)	\
    ( (This)->lpVtbl -> put_TVFlags(This,dwFlags) ) 

#define IShellNameSpace_get_TVFlags(This,dwFlags)	\
    ( (This)->lpVtbl -> get_TVFlags(This,dwFlags) ) 

#define IShellNameSpace_get_Columns(This,bstrColumns)	\
    ( (This)->lpVtbl -> get_Columns(This,bstrColumns) ) 

#define IShellNameSpace_put_Columns(This,bstrColumns)	\
    ( (This)->lpVtbl -> put_Columns(This,bstrColumns) ) 

#define IShellNameSpace_get_CountViewTypes(This,piTypes)	\
    ( (This)->lpVtbl -> get_CountViewTypes(This,piTypes) ) 

#define IShellNameSpace_SetViewType(This,iType)	\
    ( (This)->lpVtbl -> SetViewType(This,iType) ) 

#define IShellNameSpace_SelectedItems(This,ppid)	\
    ( (This)->lpVtbl -> SelectedItems(This,ppid) ) 

#define IShellNameSpace_Expand(This,var,iDepth)	\
    ( (This)->lpVtbl -> Expand(This,var,iDepth) ) 

#define IShellNameSpace_UnselectAll(This)	\
    ( (This)->lpVtbl -> UnselectAll(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IShellNameSpace_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ShellNameSpace;

#ifdef __cplusplus

class DECLSPEC_UUID("55136805-B2DE-11D1-B9F2-00A0C98BC547")
ShellNameSpace;
#endif

EXTERN_C const CLSID CLSID_ShellShellNameSpace;

#ifdef __cplusplus

class DECLSPEC_UUID("2F2F1F96-2BC1-4b1c-BE28-EA3774F4676A")
ShellShellNameSpace;
#endif

#ifndef __IScriptErrorList_INTERFACE_DEFINED__
#define __IScriptErrorList_INTERFACE_DEFINED__

/* interface IScriptErrorList */
/* [object][dual][hidden][oleautomation][helpstring][uuid] */ 


EXTERN_C const IID IID_IScriptErrorList;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F3470F24-15FD-11d2-BB2E-00805FF7EFCA")
    IScriptErrorList : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE advanceError( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE retreatError( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE canAdvanceError( 
            /* [retval][out] */ __RPC__out BOOL *pfCanAdvance) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE canRetreatError( 
            /* [retval][out] */ __RPC__out BOOL *pfCanRetreat) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getErrorLine( 
            /* [retval][out] */ __RPC__out LONG *plLine) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getErrorChar( 
            /* [retval][out] */ __RPC__out LONG *plChar) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getErrorCode( 
            /* [retval][out] */ __RPC__out LONG *plCode) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getErrorMsg( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pstr) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getErrorUrl( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pstr) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getAlwaysShowLockState( 
            /* [retval][out] */ __RPC__out BOOL *pfAlwaysShowLocked) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getDetailsPaneOpen( 
            /* [retval][out] */ __RPC__out BOOL *pfDetailsPaneOpen) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE setDetailsPaneOpen( 
            BOOL fDetailsPaneOpen) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getPerErrorDisplay( 
            /* [retval][out] */ __RPC__out BOOL *pfPerErrorDisplay) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE setPerErrorDisplay( 
            BOOL fPerErrorDisplay) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IScriptErrorListVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IScriptErrorList * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IScriptErrorList * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IScriptErrorList * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IScriptErrorList * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IScriptErrorList * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IScriptErrorList * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IScriptErrorList * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *advanceError )( 
            IScriptErrorList * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *retreatError )( 
            IScriptErrorList * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *canAdvanceError )( 
            IScriptErrorList * This,
            /* [retval][out] */ __RPC__out BOOL *pfCanAdvance);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *canRetreatError )( 
            IScriptErrorList * This,
            /* [retval][out] */ __RPC__out BOOL *pfCanRetreat);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *getErrorLine )( 
            IScriptErrorList * This,
            /* [retval][out] */ __RPC__out LONG *plLine);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *getErrorChar )( 
            IScriptErrorList * This,
            /* [retval][out] */ __RPC__out LONG *plChar);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *getErrorCode )( 
            IScriptErrorList * This,
            /* [retval][out] */ __RPC__out LONG *plCode);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *getErrorMsg )( 
            IScriptErrorList * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pstr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *getErrorUrl )( 
            IScriptErrorList * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pstr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *getAlwaysShowLockState )( 
            IScriptErrorList * This,
            /* [retval][out] */ __RPC__out BOOL *pfAlwaysShowLocked);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *getDetailsPaneOpen )( 
            IScriptErrorList * This,
            /* [retval][out] */ __RPC__out BOOL *pfDetailsPaneOpen);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *setDetailsPaneOpen )( 
            IScriptErrorList * This,
            BOOL fDetailsPaneOpen);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *getPerErrorDisplay )( 
            IScriptErrorList * This,
            /* [retval][out] */ __RPC__out BOOL *pfPerErrorDisplay);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *setPerErrorDisplay )( 
            IScriptErrorList * This,
            BOOL fPerErrorDisplay);
        
        END_INTERFACE
    } IScriptErrorListVtbl;

    interface IScriptErrorList
    {
        CONST_VTBL struct IScriptErrorListVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IScriptErrorList_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IScriptErrorList_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IScriptErrorList_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IScriptErrorList_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IScriptErrorList_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IScriptErrorList_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IScriptErrorList_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IScriptErrorList_advanceError(This)	\
    ( (This)->lpVtbl -> advanceError(This) ) 

#define IScriptErrorList_retreatError(This)	\
    ( (This)->lpVtbl -> retreatError(This) ) 

#define IScriptErrorList_canAdvanceError(This,pfCanAdvance)	\
    ( (This)->lpVtbl -> canAdvanceError(This,pfCanAdvance) ) 

#define IScriptErrorList_canRetreatError(This,pfCanRetreat)	\
    ( (This)->lpVtbl -> canRetreatError(This,pfCanRetreat) ) 

#define IScriptErrorList_getErrorLine(This,plLine)	\
    ( (This)->lpVtbl -> getErrorLine(This,plLine) ) 

#define IScriptErrorList_getErrorChar(This,plChar)	\
    ( (This)->lpVtbl -> getErrorChar(This,plChar) ) 

#define IScriptErrorList_getErrorCode(This,plCode)	\
    ( (This)->lpVtbl -> getErrorCode(This,plCode) ) 

#define IScriptErrorList_getErrorMsg(This,pstr)	\
    ( (This)->lpVtbl -> getErrorMsg(This,pstr) ) 

#define IScriptErrorList_getErrorUrl(This,pstr)	\
    ( (This)->lpVtbl -> getErrorUrl(This,pstr) ) 

#define IScriptErrorList_getAlwaysShowLockState(This,pfAlwaysShowLocked)	\
    ( (This)->lpVtbl -> getAlwaysShowLockState(This,pfAlwaysShowLocked) ) 

#define IScriptErrorList_getDetailsPaneOpen(This,pfDetailsPaneOpen)	\
    ( (This)->lpVtbl -> getDetailsPaneOpen(This,pfDetailsPaneOpen) ) 

#define IScriptErrorList_setDetailsPaneOpen(This,fDetailsPaneOpen)	\
    ( (This)->lpVtbl -> setDetailsPaneOpen(This,fDetailsPaneOpen) ) 

#define IScriptErrorList_getPerErrorDisplay(This,pfPerErrorDisplay)	\
    ( (This)->lpVtbl -> getPerErrorDisplay(This,pfPerErrorDisplay) ) 

#define IScriptErrorList_setPerErrorDisplay(This,fPerErrorDisplay)	\
    ( (This)->lpVtbl -> setPerErrorDisplay(This,fPerErrorDisplay) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IScriptErrorList_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CScriptErrorList;

#ifdef __cplusplus

class DECLSPEC_UUID("EFD01300-160F-11d2-BB2E-00805FF7EFCA")
CScriptErrorList;
#endif
#endif /* __SHDocVw_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



