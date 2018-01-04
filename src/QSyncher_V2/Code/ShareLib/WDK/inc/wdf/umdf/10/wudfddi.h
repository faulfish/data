

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0499 */
/* Compiler settings for wudfddi.idl:
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

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __wudfddi_h__
#define __wudfddi_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWDFObject_FWD_DEFINED__
#define __IWDFObject_FWD_DEFINED__
typedef interface IWDFObject IWDFObject;
#endif 	/* __IWDFObject_FWD_DEFINED__ */


#ifndef __IWDFDriver_FWD_DEFINED__
#define __IWDFDriver_FWD_DEFINED__
typedef interface IWDFDriver IWDFDriver;
#endif 	/* __IWDFDriver_FWD_DEFINED__ */


#ifndef __IWDFDeviceInitialize_FWD_DEFINED__
#define __IWDFDeviceInitialize_FWD_DEFINED__
typedef interface IWDFDeviceInitialize IWDFDeviceInitialize;
#endif 	/* __IWDFDeviceInitialize_FWD_DEFINED__ */


#ifndef __IWDFDevice_FWD_DEFINED__
#define __IWDFDevice_FWD_DEFINED__
typedef interface IWDFDevice IWDFDevice;
#endif 	/* __IWDFDevice_FWD_DEFINED__ */


#ifndef __IWDFNamedPropertyStore_FWD_DEFINED__
#define __IWDFNamedPropertyStore_FWD_DEFINED__
typedef interface IWDFNamedPropertyStore IWDFNamedPropertyStore;
#endif 	/* __IWDFNamedPropertyStore_FWD_DEFINED__ */


#ifndef __IWDFIoQueue_FWD_DEFINED__
#define __IWDFIoQueue_FWD_DEFINED__
typedef interface IWDFIoQueue IWDFIoQueue;
#endif 	/* __IWDFIoQueue_FWD_DEFINED__ */


#ifndef __IWDFIoRequest_FWD_DEFINED__
#define __IWDFIoRequest_FWD_DEFINED__
typedef interface IWDFIoRequest IWDFIoRequest;
#endif 	/* __IWDFIoRequest_FWD_DEFINED__ */


#ifndef __IWDFRequestCompletionParams_FWD_DEFINED__
#define __IWDFRequestCompletionParams_FWD_DEFINED__
typedef interface IWDFRequestCompletionParams IWDFRequestCompletionParams;
#endif 	/* __IWDFRequestCompletionParams_FWD_DEFINED__ */


#ifndef __IWDFIoRequestCompletionParams_FWD_DEFINED__
#define __IWDFIoRequestCompletionParams_FWD_DEFINED__
typedef interface IWDFIoRequestCompletionParams IWDFIoRequestCompletionParams;
#endif 	/* __IWDFIoRequestCompletionParams_FWD_DEFINED__ */


#ifndef __IWDFFile_FWD_DEFINED__
#define __IWDFFile_FWD_DEFINED__
typedef interface IWDFFile IWDFFile;
#endif 	/* __IWDFFile_FWD_DEFINED__ */


#ifndef __IWDFDriverCreatedFile_FWD_DEFINED__
#define __IWDFDriverCreatedFile_FWD_DEFINED__
typedef interface IWDFDriverCreatedFile IWDFDriverCreatedFile;
#endif 	/* __IWDFDriverCreatedFile_FWD_DEFINED__ */


#ifndef __IWDFIoTarget_FWD_DEFINED__
#define __IWDFIoTarget_FWD_DEFINED__
typedef interface IWDFIoTarget IWDFIoTarget;
#endif 	/* __IWDFIoTarget_FWD_DEFINED__ */


#ifndef __IWDFIoTargetStateManagement_FWD_DEFINED__
#define __IWDFIoTargetStateManagement_FWD_DEFINED__
typedef interface IWDFIoTargetStateManagement IWDFIoTargetStateManagement;
#endif 	/* __IWDFIoTargetStateManagement_FWD_DEFINED__ */


#ifndef __IWDFFileHandleTargetFactory_FWD_DEFINED__
#define __IWDFFileHandleTargetFactory_FWD_DEFINED__
typedef interface IWDFFileHandleTargetFactory IWDFFileHandleTargetFactory;
#endif 	/* __IWDFFileHandleTargetFactory_FWD_DEFINED__ */


#ifndef __IWDFMemory_FWD_DEFINED__
#define __IWDFMemory_FWD_DEFINED__
typedef interface IWDFMemory IWDFMemory;
#endif 	/* __IWDFMemory_FWD_DEFINED__ */


#ifndef __IObjectCleanup_FWD_DEFINED__
#define __IObjectCleanup_FWD_DEFINED__
typedef interface IObjectCleanup IObjectCleanup;
#endif 	/* __IObjectCleanup_FWD_DEFINED__ */


#ifndef __IDriverEntry_FWD_DEFINED__
#define __IDriverEntry_FWD_DEFINED__
typedef interface IDriverEntry IDriverEntry;
#endif 	/* __IDriverEntry_FWD_DEFINED__ */


#ifndef __IPnpCallback_FWD_DEFINED__
#define __IPnpCallback_FWD_DEFINED__
typedef interface IPnpCallback IPnpCallback;
#endif 	/* __IPnpCallback_FWD_DEFINED__ */


#ifndef __IPnpCallbackSelfManagedIo_FWD_DEFINED__
#define __IPnpCallbackSelfManagedIo_FWD_DEFINED__
typedef interface IPnpCallbackSelfManagedIo IPnpCallbackSelfManagedIo;
#endif 	/* __IPnpCallbackSelfManagedIo_FWD_DEFINED__ */


#ifndef __IPnpCallbackHardware_FWD_DEFINED__
#define __IPnpCallbackHardware_FWD_DEFINED__
typedef interface IPnpCallbackHardware IPnpCallbackHardware;
#endif 	/* __IPnpCallbackHardware_FWD_DEFINED__ */


#ifndef __IQueueCallbackCreate_FWD_DEFINED__
#define __IQueueCallbackCreate_FWD_DEFINED__
typedef interface IQueueCallbackCreate IQueueCallbackCreate;
#endif 	/* __IQueueCallbackCreate_FWD_DEFINED__ */


#ifndef __IQueueCallbackRead_FWD_DEFINED__
#define __IQueueCallbackRead_FWD_DEFINED__
typedef interface IQueueCallbackRead IQueueCallbackRead;
#endif 	/* __IQueueCallbackRead_FWD_DEFINED__ */


#ifndef __IQueueCallbackWrite_FWD_DEFINED__
#define __IQueueCallbackWrite_FWD_DEFINED__
typedef interface IQueueCallbackWrite IQueueCallbackWrite;
#endif 	/* __IQueueCallbackWrite_FWD_DEFINED__ */


#ifndef __IQueueCallbackDeviceIoControl_FWD_DEFINED__
#define __IQueueCallbackDeviceIoControl_FWD_DEFINED__
typedef interface IQueueCallbackDeviceIoControl IQueueCallbackDeviceIoControl;
#endif 	/* __IQueueCallbackDeviceIoControl_FWD_DEFINED__ */


#ifndef __IQueueCallbackDefaultIoHandler_FWD_DEFINED__
#define __IQueueCallbackDefaultIoHandler_FWD_DEFINED__
typedef interface IQueueCallbackDefaultIoHandler IQueueCallbackDefaultIoHandler;
#endif 	/* __IQueueCallbackDefaultIoHandler_FWD_DEFINED__ */


#ifndef __IQueueCallbackStateChange_FWD_DEFINED__
#define __IQueueCallbackStateChange_FWD_DEFINED__
typedef interface IQueueCallbackStateChange IQueueCallbackStateChange;
#endif 	/* __IQueueCallbackStateChange_FWD_DEFINED__ */


#ifndef __IQueueCallbackIoStop_FWD_DEFINED__
#define __IQueueCallbackIoStop_FWD_DEFINED__
typedef interface IQueueCallbackIoStop IQueueCallbackIoStop;
#endif 	/* __IQueueCallbackIoStop_FWD_DEFINED__ */


#ifndef __IQueueCallbackIoResume_FWD_DEFINED__
#define __IQueueCallbackIoResume_FWD_DEFINED__
typedef interface IQueueCallbackIoResume IQueueCallbackIoResume;
#endif 	/* __IQueueCallbackIoResume_FWD_DEFINED__ */


#ifndef __IRequestCallbackCancel_FWD_DEFINED__
#define __IRequestCallbackCancel_FWD_DEFINED__
typedef interface IRequestCallbackCancel IRequestCallbackCancel;
#endif 	/* __IRequestCallbackCancel_FWD_DEFINED__ */


#ifndef __IRequestCallbackRequestCompletion_FWD_DEFINED__
#define __IRequestCallbackRequestCompletion_FWD_DEFINED__
typedef interface IRequestCallbackRequestCompletion IRequestCallbackRequestCompletion;
#endif 	/* __IRequestCallbackRequestCompletion_FWD_DEFINED__ */


#ifndef __IFileCallbackCleanup_FWD_DEFINED__
#define __IFileCallbackCleanup_FWD_DEFINED__
typedef interface IFileCallbackCleanup IFileCallbackCleanup;
#endif 	/* __IFileCallbackCleanup_FWD_DEFINED__ */


#ifndef __IFileCallbackClose_FWD_DEFINED__
#define __IFileCallbackClose_FWD_DEFINED__
typedef interface IFileCallbackClose IFileCallbackClose;
#endif 	/* __IFileCallbackClose_FWD_DEFINED__ */


#ifndef __IImpersonateCallback_FWD_DEFINED__
#define __IImpersonateCallback_FWD_DEFINED__
typedef interface IImpersonateCallback IImpersonateCallback;
#endif 	/* __IImpersonateCallback_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_wudfddi_0000_0000 */
/* [local] */ 

typedef WCHAR *PWSTR;

typedef const WCHAR *PCWSTR;

typedef struct UMDF_VERSION_DATA
    {
    ULONG MajorNumber;
    ULONG MinorNumber;
    ULONG ServiceNumber;
    } 	UMDF_VERSION_DATA;

typedef struct UMDF_VERSION_DATA *PUMDF_VERSION_DATA;


__declspec(dllexport) 
__declspec(selectany) 
UMDF_VERSION_DATA Microsoft_WDF_UMDF_Version = {1, 
                                                5, 
                                                0};

#ifdef MIDL_PASS
typedef 
enum _SECURITY_IMPERSONATION_LEVEL
    {	SecurityAnonymous	= 0,
	SecurityIdentification	= ( SecurityAnonymous + 1 ) ,
	SecurityImpersonation	= ( SecurityIdentification + 1 ) ,
	SecurityDelegation	= ( SecurityImpersonation + 1 ) 
    } 	SECURITY_IMPERSONATION_LEVEL;

#endif
#ifndef STATUS_WDF_PAUSED
#define STATUS_WDF_PAUSED       ((NTSTATUS)0xC0200203L)
#endif
#ifndef STATUS_WDF_BUSY
#define STATUS_WDF_BUSY         ((NTSTATUS)0xC0200204L)
#endif
typedef 
enum _WDF_CALLBACK_CONSTRAINT
    {	None	= 0,
	WdfDeviceLevel	= 1,
	WdfLevelReserved	= 2,
	WdfLevelMaximum	= ( WdfLevelReserved + 1 ) 
    } 	WDF_CALLBACK_CONSTRAINT;

typedef 
enum _WDF_IO_QUEUE_DISPATCH_TYPE
    {	WdfIoQueueDispatchSequential	= 1,
	WdfIoQueueDispatchParallel	= 2,
	WdfIoQueueDispatchManual	= 3,
	WdfIoQueueDispatchMaximum	= ( WdfIoQueueDispatchManual + 1 ) 
    } 	WDF_IO_QUEUE_DISPATCH_TYPE;

typedef 
enum _WDF_IO_QUEUE_STATE
    {	WdfIoQueueAcceptRequests	= 0x1,
	WdfIoQueueDispatchRequests	= 0x2,
	WdfIoQueueNoRequests	= 0x4,
	WdfIoQueueDriverNoRequests	= 0x8,
	WdfIoQueuePnpHeld	= 0x10
    } 	WDF_IO_QUEUE_STATE;

typedef enum _WDF_IO_QUEUE_STATE *PWDF_IO_QUEUE_STATE;

typedef 
enum _WDF_REQUEST_TYPE
    {	WdfRequestUndefined	= 0,
	WdfRequestCreate	= 1,
	WdfRequestCleanup	= 2,
	WdfRequestRead	= 3,
	WdfRequestWrite	= 4,
	WdfRequestDeviceIoControl	= 5,
	WdfRequestClose	= 6,
	WdfRequestUsb	= 7,
	WdfRequestOther	= 8,
	WdfRequestInternalIoctl	= 9,
	WdfRequestTypeNoFormat	= 10,
	WdfRequestMaximum	= ( WdfRequestTypeNoFormat + 1 ) 
    } 	WDF_REQUEST_TYPE;

typedef enum _WDF_REQUEST_TYPE *PWDF_REQUEST_TYPE;

typedef 
enum _WDF_REQUEST_STOP_ACTION_FLAGS
    {	WdfRequestStopActionInvalid	= 0,
	WdfRequestStopActionSuspend	= 0x1,
	WdfRequestStopActionPurge	= 0x2,
	WdfRequestStopRequestCancelable	= 0x10000000
    } 	WDF_REQUEST_STOP_ACTION_FLAGS;

typedef 
enum _WDF_REQUEST_SEND_OPTIONS_FLAGS
    {	WDF_REQUEST_SEND_OPTION_TIMEOUT	= 0x1,
	WDF_REQUEST_SEND_OPTION_SYNCHRONOUS	= 0x2,
	WDF_REQUEST_SEND_OPTION_IGNORE_TARGET_STATE	= 0x4,
	WDF_REQUEST_SEND_OPTION_SEND_AND_FORGET	= 0x8
    } 	WDF_REQUEST_SEND_OPTIONS_FLAGS;

typedef 
enum _WDF_IO_TARGET_STATE
    {	WdfIoTargetStateUndefined	= 0,
	WdfIoTargetStarted	= 1,
	WdfIoTargetStopped	= 2,
	WdfIoTargetClosedForQueryRemove	= 3,
	WdfIoTargetClosed	= 4,
	WdfIoTargetDeleted	= 5,
	WdfIoTargetStateMaximum	= ( WdfIoTargetDeleted + 1 ) 
    } 	WDF_IO_TARGET_STATE;

typedef enum _WDF_IO_TARGET_STATE *PWDF_IO_TARGET_STATE;

typedef 
enum _WDF_IO_TARGET_SENT_IO_ACTION
    {	WdfIoTargetSentIoUndefined	= 0,
	WdfIoTargetCancelSentIo	= 1,
	WdfIoTargetWaitForSentIoToComplete	= 2,
	WdfIoTargetLeaveSentIoPending	= 3,
	WdfIoTargetSentIoMaximum	= ( WdfIoTargetLeaveSentIoPending + 1 ) 
    } 	WDF_IO_TARGET_SENT_IO_ACTION;

typedef struct _WDFMEMORY_OFFSET
    {
    ULONG_PTR BufferOffset;
    ULONG_PTR BufferLength;
    } 	WDFMEMORY_OFFSET;

typedef struct _WDFMEMORY_OFFSET *PWDFMEMORY_OFFSET;

typedef 
enum _WDF_TRI_STATE
    {	WdfUseDefault	= 0,
	WdfFalse	= 1,
	WdfTrue	= 2
    } 	WDF_TRI_STATE;

typedef enum _WDF_TRI_STATE *PWDF_TRI_STATE;

typedef 
enum _WDF_PNP_STATE
    {	WdfPnpStateInvalid	= 0,
	WdfPnpStateDisabled	= 1,
	WdfPnpStateFailed	= 2,
	WdfPnpStateRemoved	= 3,
	WdfPnpStateResourcesChanged	= 4,
	WdfPnpStateDontDisplayInUI	= 5,
	WdfPnpStateNotDisableable	= 6,
	WdfPnpStateMaximum	= ( WdfPnpStateNotDisableable + 1 ) 
    } 	WDF_PNP_STATE;

typedef 
enum _WDF_PNP_CAPABILITY
    {	WdfPnpCapInvalid	= 0,
	WdfPnpCapLockSupported	= 1,
	WdfPnpCapEjectSupported	= 2,
	WdfPnpCapRemovable	= 3,
	WdfPnpCapDockDevice	= 4,
	WdfPnpCapSurpriseRemovalOk	= 5,
	WdfPnpCapNoDisplayInUI	= 6,
	WdfPnpCapMaximum	= ( WdfPnpCapNoDisplayInUI + 1 ) 
    } 	WDF_PNP_CAPABILITY;

typedef 
enum _WDF_POWER_DEVICE_STATE
    {	WdfPowerDeviceInvalid	= 0,
	WdfPowerDeviceD0	= 1,
	WdfPowerDeviceD1	= 2,
	WdfPowerDeviceD2	= 3,
	WdfPowerDeviceD3	= 4,
	WdfPowerDeviceD3Final	= 5,
	WdfPowerDevicePrepareForHibernation	= 6,
	WdfPowerDeviceMaximum	= ( WdfPowerDevicePrepareForHibernation + 1 ) 
    } 	WDF_POWER_DEVICE_STATE;

typedef enum _WDF_POWER_DEVICE_STATE *PWDF_POWER_DEVICE_STATE;

typedef 
enum _WDF_EVENT_TYPE
    {	WdfEventReserved	= 0,
	WdfEventBroadcast	= 1,
	WdfEventMaximum	= ( WdfEventBroadcast + 1 ) 
    } 	WDF_EVENT_TYPE;

typedef 
enum _WDF_PROPERTY_STORE_RETRIEVE_FLAGS
    {	WdfPropertyStoreNormal	= 0,
	WdfPropertyStoreCreateIfMissing	= 0x1
    } 	WDF_PROPERTY_STORE_RETRIEVE_FLAGS;

typedef 
enum _WDF_PROPERTY_STORE_DISPOSITION
    {	CreatedNewStore	= 0x1,
	OpenedExistingStore	= 0x2
    } 	WDF_PROPERTY_STORE_DISPOSITION;




































extern RPC_IF_HANDLE __MIDL_itf_wudfddi_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wudfddi_0000_0000_v0_0_s_ifspec;

#ifndef __IWDFObject_INTERFACE_DEFINED__
#define __IWDFObject_INTERFACE_DEFINED__

/* interface IWDFObject */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWDFObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("64275C66-2E71-4060-B5F4-3A76DF96ED3C")
    IWDFObject : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE DeleteWdfObject( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AssignContext( 
            /* [unique][in] */ 
            __in_opt  IObjectCleanup *pCleanupCallback,
            /* [unique][in] */ 
            __in_opt  void *pContext) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RetrieveContext( 
            /* [out] */ 
            __out  void **ppvContext) = 0;
        
        virtual void STDMETHODCALLTYPE AcquireLock( void) = 0;
        
        virtual void STDMETHODCALLTYPE ReleaseLock( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWDFObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWDFObject * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWDFObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWDFObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteWdfObject )( 
            IWDFObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *AssignContext )( 
            IWDFObject * This,
            /* [unique][in] */ 
            __in_opt  IObjectCleanup *pCleanupCallback,
            /* [unique][in] */ 
            __in_opt  void *pContext);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveContext )( 
            IWDFObject * This,
            /* [out] */ 
            __out  void **ppvContext);
        
        void ( STDMETHODCALLTYPE *AcquireLock )( 
            IWDFObject * This);
        
        void ( STDMETHODCALLTYPE *ReleaseLock )( 
            IWDFObject * This);
        
        END_INTERFACE
    } IWDFObjectVtbl;

    interface IWDFObject
    {
        CONST_VTBL struct IWDFObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWDFObject_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWDFObject_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWDFObject_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWDFObject_DeleteWdfObject(This)	\
    ( (This)->lpVtbl -> DeleteWdfObject(This) ) 

#define IWDFObject_AssignContext(This,pCleanupCallback,pContext)	\
    ( (This)->lpVtbl -> AssignContext(This,pCleanupCallback,pContext) ) 

#define IWDFObject_RetrieveContext(This,ppvContext)	\
    ( (This)->lpVtbl -> RetrieveContext(This,ppvContext) ) 

#define IWDFObject_AcquireLock(This)	\
    ( (This)->lpVtbl -> AcquireLock(This) ) 

#define IWDFObject_ReleaseLock(This)	\
    ( (This)->lpVtbl -> ReleaseLock(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWDFObject_INTERFACE_DEFINED__ */


#ifndef __IWDFDriver_INTERFACE_DEFINED__
#define __IWDFDriver_INTERFACE_DEFINED__

/* interface IWDFDriver */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWDFDriver;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("AD368EBE-4139-43e3-A875-69B266A9139C")
    IWDFDriver : public IWDFObject
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateDevice( 
            /* [in] */ 
            __in  IWDFDeviceInitialize *pDeviceInit,
            /* [in] */ 
            __in  IUnknown *pCallbackInterface,
            /* [out] */ 
            __out  IWDFDevice **ppDevice) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateWdfObject( 
            /* [unique][in] */ 
            __in_opt  IUnknown *pCallbackInterface,
            /* [unique][in] */ 
            __in_opt  IWDFObject *pParentObject,
            /* [out] */ 
            __out  IWDFObject **ppWdfObject) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreatePreallocatedWdfMemory( 
            /* [size_is][in] */ 
            __in_bcount(BufferSize)  BYTE *pBuff,
            /* [in] */ 
            __in  SIZE_T BufferSize,
            /* [unique][in] */ 
            __in_opt  IUnknown *pCallbackInterface,
            /* [unique][in] */ 
            __in_opt  IWDFObject *pParentObject,
            /* [out] */ 
            __out  IWDFMemory **ppWdfMemory) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateWdfMemory( 
            /* [in] */ 
            __in  SIZE_T BufferSize,
            /* [unique][in] */ 
            __in_opt  IUnknown *pCallbackInterface,
            /* [unique][in] */ 
            __in_opt  IWDFObject *pParentObject,
            /* [out] */ 
            __out  IWDFMemory **ppWdfMemory) = 0;
        
        virtual BOOL STDMETHODCALLTYPE IsVersionAvailable( 
            /* [in] */ 
            __in  UMDF_VERSION_DATA *pMinimumVersion) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RetrieveVersionString( 
            /* [unique][out][string] */ 
            __out_ecount_part_opt(*pdwVersionLength, *pdwVersionLength)  PWSTR pVersion,
            /* [out][in] */ 
            __inout  DWORD *pdwVersionLength) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWDFDriverVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWDFDriver * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWDFDriver * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWDFDriver * This);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteWdfObject )( 
            IWDFDriver * This);
        
        HRESULT ( STDMETHODCALLTYPE *AssignContext )( 
            IWDFDriver * This,
            /* [unique][in] */ 
            __in_opt  IObjectCleanup *pCleanupCallback,
            /* [unique][in] */ 
            __in_opt  void *pContext);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveContext )( 
            IWDFDriver * This,
            /* [out] */ 
            __out  void **ppvContext);
        
        void ( STDMETHODCALLTYPE *AcquireLock )( 
            IWDFDriver * This);
        
        void ( STDMETHODCALLTYPE *ReleaseLock )( 
            IWDFDriver * This);
        
        HRESULT ( STDMETHODCALLTYPE *CreateDevice )( 
            IWDFDriver * This,
            /* [in] */ 
            __in  IWDFDeviceInitialize *pDeviceInit,
            /* [in] */ 
            __in  IUnknown *pCallbackInterface,
            /* [out] */ 
            __out  IWDFDevice **ppDevice);
        
        HRESULT ( STDMETHODCALLTYPE *CreateWdfObject )( 
            IWDFDriver * This,
            /* [unique][in] */ 
            __in_opt  IUnknown *pCallbackInterface,
            /* [unique][in] */ 
            __in_opt  IWDFObject *pParentObject,
            /* [out] */ 
            __out  IWDFObject **ppWdfObject);
        
        HRESULT ( STDMETHODCALLTYPE *CreatePreallocatedWdfMemory )( 
            IWDFDriver * This,
            /* [size_is][in] */ 
            __in_bcount(BufferSize)  BYTE *pBuff,
            /* [in] */ 
            __in  SIZE_T BufferSize,
            /* [unique][in] */ 
            __in_opt  IUnknown *pCallbackInterface,
            /* [unique][in] */ 
            __in_opt  IWDFObject *pParentObject,
            /* [out] */ 
            __out  IWDFMemory **ppWdfMemory);
        
        HRESULT ( STDMETHODCALLTYPE *CreateWdfMemory )( 
            IWDFDriver * This,
            /* [in] */ 
            __in  SIZE_T BufferSize,
            /* [unique][in] */ 
            __in_opt  IUnknown *pCallbackInterface,
            /* [unique][in] */ 
            __in_opt  IWDFObject *pParentObject,
            /* [out] */ 
            __out  IWDFMemory **ppWdfMemory);
        
        BOOL ( STDMETHODCALLTYPE *IsVersionAvailable )( 
            IWDFDriver * This,
            /* [in] */ 
            __in  UMDF_VERSION_DATA *pMinimumVersion);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveVersionString )( 
            IWDFDriver * This,
            /* [unique][out][string] */ 
            __out_ecount_part_opt(*pdwVersionLength, *pdwVersionLength)  PWSTR pVersion,
            /* [out][in] */ 
            __inout  DWORD *pdwVersionLength);
        
        END_INTERFACE
    } IWDFDriverVtbl;

    interface IWDFDriver
    {
        CONST_VTBL struct IWDFDriverVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWDFDriver_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWDFDriver_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWDFDriver_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWDFDriver_DeleteWdfObject(This)	\
    ( (This)->lpVtbl -> DeleteWdfObject(This) ) 

#define IWDFDriver_AssignContext(This,pCleanupCallback,pContext)	\
    ( (This)->lpVtbl -> AssignContext(This,pCleanupCallback,pContext) ) 

#define IWDFDriver_RetrieveContext(This,ppvContext)	\
    ( (This)->lpVtbl -> RetrieveContext(This,ppvContext) ) 

#define IWDFDriver_AcquireLock(This)	\
    ( (This)->lpVtbl -> AcquireLock(This) ) 

#define IWDFDriver_ReleaseLock(This)	\
    ( (This)->lpVtbl -> ReleaseLock(This) ) 


#define IWDFDriver_CreateDevice(This,pDeviceInit,pCallbackInterface,ppDevice)	\
    ( (This)->lpVtbl -> CreateDevice(This,pDeviceInit,pCallbackInterface,ppDevice) ) 

#define IWDFDriver_CreateWdfObject(This,pCallbackInterface,pParentObject,ppWdfObject)	\
    ( (This)->lpVtbl -> CreateWdfObject(This,pCallbackInterface,pParentObject,ppWdfObject) ) 

#define IWDFDriver_CreatePreallocatedWdfMemory(This,pBuff,BufferSize,pCallbackInterface,pParentObject,ppWdfMemory)	\
    ( (This)->lpVtbl -> CreatePreallocatedWdfMemory(This,pBuff,BufferSize,pCallbackInterface,pParentObject,ppWdfMemory) ) 

#define IWDFDriver_CreateWdfMemory(This,BufferSize,pCallbackInterface,pParentObject,ppWdfMemory)	\
    ( (This)->lpVtbl -> CreateWdfMemory(This,BufferSize,pCallbackInterface,pParentObject,ppWdfMemory) ) 

#define IWDFDriver_IsVersionAvailable(This,pMinimumVersion)	\
    ( (This)->lpVtbl -> IsVersionAvailable(This,pMinimumVersion) ) 

#define IWDFDriver_RetrieveVersionString(This,pVersion,pdwVersionLength)	\
    ( (This)->lpVtbl -> RetrieveVersionString(This,pVersion,pdwVersionLength) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWDFDriver_INTERFACE_DEFINED__ */


#ifndef __IWDFDeviceInitialize_INTERFACE_DEFINED__
#define __IWDFDeviceInitialize_INTERFACE_DEFINED__

/* interface IWDFDeviceInitialize */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWDFDeviceInitialize;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("74CC381C-0871-43c1-878B-3F7C9D16933D")
    IWDFDeviceInitialize : public IUnknown
    {
    public:
        virtual void STDMETHODCALLTYPE SetFilter( void) = 0;
        
        virtual void STDMETHODCALLTYPE SetLockingConstraint( 
            /* [in] */ 
            __in  WDF_CALLBACK_CONSTRAINT LockType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RetrieveDevicePropertyStore( 
            /* [unique][in] */ 
            __in_opt  PCWSTR pcwszServiceName,
            /* [in] */ 
            __in  WDF_PROPERTY_STORE_RETRIEVE_FLAGS Flags,
            /* [out] */ 
            __out  IWDFNamedPropertyStore **ppPropStore,
            /* [unique][out] */ 
            __out_opt  WDF_PROPERTY_STORE_DISPOSITION *pDisposition) = 0;
        
        virtual void STDMETHODCALLTYPE SetPowerPolicyOwnership( 
            /* [in] */ 
            __in  BOOL fTrue) = 0;
        
        virtual void STDMETHODCALLTYPE AutoForwardCreateCleanupClose( 
            /* [in] */ 
            __in  WDF_TRI_STATE State) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RetrieveDeviceInstanceId( 
            /* [unique][out][string] */ 
            __out_opt  PWSTR Buffer,
            /* [out][in] */ 
            __inout  DWORD *pdwSizeInChars) = 0;
        
        virtual void STDMETHODCALLTYPE SetPnpCapability( 
            /* [in] */ 
            __in  WDF_PNP_CAPABILITY Capability,
            /* [in] */ 
            __in  WDF_TRI_STATE Value) = 0;
        
        virtual WDF_TRI_STATE STDMETHODCALLTYPE GetPnpCapability( 
            /* [in] */ 
            __in  WDF_PNP_CAPABILITY Capability) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWDFDeviceInitializeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWDFDeviceInitialize * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWDFDeviceInitialize * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWDFDeviceInitialize * This);
        
        void ( STDMETHODCALLTYPE *SetFilter )( 
            IWDFDeviceInitialize * This);
        
        void ( STDMETHODCALLTYPE *SetLockingConstraint )( 
            IWDFDeviceInitialize * This,
            /* [in] */ 
            __in  WDF_CALLBACK_CONSTRAINT LockType);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveDevicePropertyStore )( 
            IWDFDeviceInitialize * This,
            /* [unique][in] */ 
            __in_opt  PCWSTR pcwszServiceName,
            /* [in] */ 
            __in  WDF_PROPERTY_STORE_RETRIEVE_FLAGS Flags,
            /* [out] */ 
            __out  IWDFNamedPropertyStore **ppPropStore,
            /* [unique][out] */ 
            __out_opt  WDF_PROPERTY_STORE_DISPOSITION *pDisposition);
        
        void ( STDMETHODCALLTYPE *SetPowerPolicyOwnership )( 
            IWDFDeviceInitialize * This,
            /* [in] */ 
            __in  BOOL fTrue);
        
        void ( STDMETHODCALLTYPE *AutoForwardCreateCleanupClose )( 
            IWDFDeviceInitialize * This,
            /* [in] */ 
            __in  WDF_TRI_STATE State);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveDeviceInstanceId )( 
            IWDFDeviceInitialize * This,
            /* [unique][out][string] */ 
            __out_opt  PWSTR Buffer,
            /* [out][in] */ 
            __inout  DWORD *pdwSizeInChars);
        
        void ( STDMETHODCALLTYPE *SetPnpCapability )( 
            IWDFDeviceInitialize * This,
            /* [in] */ 
            __in  WDF_PNP_CAPABILITY Capability,
            /* [in] */ 
            __in  WDF_TRI_STATE Value);
        
        WDF_TRI_STATE ( STDMETHODCALLTYPE *GetPnpCapability )( 
            IWDFDeviceInitialize * This,
            /* [in] */ 
            __in  WDF_PNP_CAPABILITY Capability);
        
        END_INTERFACE
    } IWDFDeviceInitializeVtbl;

    interface IWDFDeviceInitialize
    {
        CONST_VTBL struct IWDFDeviceInitializeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWDFDeviceInitialize_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWDFDeviceInitialize_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWDFDeviceInitialize_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWDFDeviceInitialize_SetFilter(This)	\
    ( (This)->lpVtbl -> SetFilter(This) ) 

#define IWDFDeviceInitialize_SetLockingConstraint(This,LockType)	\
    ( (This)->lpVtbl -> SetLockingConstraint(This,LockType) ) 

#define IWDFDeviceInitialize_RetrieveDevicePropertyStore(This,pcwszServiceName,Flags,ppPropStore,pDisposition)	\
    ( (This)->lpVtbl -> RetrieveDevicePropertyStore(This,pcwszServiceName,Flags,ppPropStore,pDisposition) ) 

#define IWDFDeviceInitialize_SetPowerPolicyOwnership(This,fTrue)	\
    ( (This)->lpVtbl -> SetPowerPolicyOwnership(This,fTrue) ) 

#define IWDFDeviceInitialize_AutoForwardCreateCleanupClose(This,State)	\
    ( (This)->lpVtbl -> AutoForwardCreateCleanupClose(This,State) ) 

#define IWDFDeviceInitialize_RetrieveDeviceInstanceId(This,Buffer,pdwSizeInChars)	\
    ( (This)->lpVtbl -> RetrieveDeviceInstanceId(This,Buffer,pdwSizeInChars) ) 

#define IWDFDeviceInitialize_SetPnpCapability(This,Capability,Value)	\
    ( (This)->lpVtbl -> SetPnpCapability(This,Capability,Value) ) 

#define IWDFDeviceInitialize_GetPnpCapability(This,Capability)	\
    ( (This)->lpVtbl -> GetPnpCapability(This,Capability) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWDFDeviceInitialize_INTERFACE_DEFINED__ */


#ifndef __IWDFDevice_INTERFACE_DEFINED__
#define __IWDFDevice_INTERFACE_DEFINED__

/* interface IWDFDevice */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWDFDevice;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D657FE45-460A-49c3-8219-766AE8032A80")
    IWDFDevice : public IWDFObject
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE RetrieveDevicePropertyStore( 
            /* [unique][in] */ 
            __in_opt  PCWSTR pcwszServiceName,
            /* [in] */ 
            __in  WDF_PROPERTY_STORE_RETRIEVE_FLAGS Flags,
            /* [out] */ 
            __out  IWDFNamedPropertyStore **ppPropStore,
            /* [unique][out] */ 
            __out_opt  WDF_PROPERTY_STORE_DISPOSITION *pDisposition) = 0;
        
        virtual void STDMETHODCALLTYPE GetDriver( 
            /* [out] */ 
            __out  IWDFDriver **ppWdfDriver) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RetrieveDeviceInstanceId( 
            /* [unique][out][string] */ 
            __out_opt  PWSTR Buffer,
            /* [out][in] */ 
            __inout  DWORD *pdwSizeInChars) = 0;
        
        virtual void STDMETHODCALLTYPE GetDefaultIoTarget( 
            /* [out] */ 
            __out  IWDFIoTarget **ppWdfIoTarget) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateWdfFile( 
            /* [string][unique][in] */ 
            __in_opt  LPCWSTR pcwszFileName,
            /* [out] */ 
            __out  IWDFDriverCreatedFile **ppFile) = 0;
        
        virtual void STDMETHODCALLTYPE GetDefaultIoQueue( 
            /* [out] */ 
            __out  IWDFIoQueue **ppWdfIoQueue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateIoQueue( 
            /* [in] */ 
            __in  IUnknown *pCallbackInterface,
            /* [in] */ 
            __in  BOOL bDefaultQueue,
            /* [in] */ 
            __in  WDF_IO_QUEUE_DISPATCH_TYPE DispatchType,
            /* [in] */ 
            __in  BOOL bPowerManaged,
            /* [in] */ 
            __in  BOOL bAllowZeroLengthRequests,
            /* [out] */ 
            __out  IWDFIoQueue **ppIoQueue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateDeviceInterface( 
            /* [in] */ 
            __in  LPCGUID pDeviceInterfaceGuid,
            /* [unique][string][in] */ 
            __in_opt  PCWSTR pReferenceString) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AssignDeviceInterfaceState( 
            /* [in] */ 
            __in  LPCGUID pDeviceInterfaceGuid,
            /* [unique][string][in] */ 
            __in_opt  PCWSTR pReferenceString,
            /* [in] */ 
            __in  BOOL Enable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RetrieveDeviceName( 
            /* [unique][out][string] */ 
            __out_ecount_part_opt(*pdwDeviceNameLength, *pdwDeviceNameLength)  PWSTR pDeviceName,
            /* [out][in] */ 
            __inout  DWORD *pdwDeviceNameLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PostEvent( 
            /* [in] */ 
            __in  REFGUID EventGuid,
            /* [in] */ 
            __in  WDF_EVENT_TYPE EventType,
            /* [size_is][in] */ 
            __in_bcount(cbDataSize)  BYTE *pbData,
            /* [in] */ 
            __in  DWORD cbDataSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ConfigureRequestDispatching( 
            /* [in] */ 
            __in  IWDFIoQueue *pQueue,
            /* [in] */ 
            __in  WDF_REQUEST_TYPE RequestType,
            /* [in] */ 
            __in  BOOL Forward) = 0;
        
        virtual void STDMETHODCALLTYPE SetPnpState( 
            /* [in] */ 
            __in  WDF_PNP_STATE State,
            /* [in] */ 
            __in  WDF_TRI_STATE Value) = 0;
        
        virtual WDF_TRI_STATE STDMETHODCALLTYPE GetPnpState( 
            /* [in] */ 
            __in  WDF_PNP_STATE State) = 0;
        
        virtual void STDMETHODCALLTYPE CommitPnpState( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateRequest( 
            /* [unique][in] */ 
            __in_opt  IUnknown *pCallbackInterface,
            /* [unique][in] */ 
            __in_opt  IWDFObject *pParentObject,
            /* [out] */ 
            __out  IWDFIoRequest **ppRequest) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateSymbolicLink( 
            /* [unique][string][in] */ 
            __in  PCWSTR pSymbolicLink) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWDFDeviceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWDFDevice * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWDFDevice * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWDFDevice * This);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteWdfObject )( 
            IWDFDevice * This);
        
        HRESULT ( STDMETHODCALLTYPE *AssignContext )( 
            IWDFDevice * This,
            /* [unique][in] */ 
            __in_opt  IObjectCleanup *pCleanupCallback,
            /* [unique][in] */ 
            __in_opt  void *pContext);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveContext )( 
            IWDFDevice * This,
            /* [out] */ 
            __out  void **ppvContext);
        
        void ( STDMETHODCALLTYPE *AcquireLock )( 
            IWDFDevice * This);
        
        void ( STDMETHODCALLTYPE *ReleaseLock )( 
            IWDFDevice * This);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveDevicePropertyStore )( 
            IWDFDevice * This,
            /* [unique][in] */ 
            __in_opt  PCWSTR pcwszServiceName,
            /* [in] */ 
            __in  WDF_PROPERTY_STORE_RETRIEVE_FLAGS Flags,
            /* [out] */ 
            __out  IWDFNamedPropertyStore **ppPropStore,
            /* [unique][out] */ 
            __out_opt  WDF_PROPERTY_STORE_DISPOSITION *pDisposition);
        
        void ( STDMETHODCALLTYPE *GetDriver )( 
            IWDFDevice * This,
            /* [out] */ 
            __out  IWDFDriver **ppWdfDriver);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveDeviceInstanceId )( 
            IWDFDevice * This,
            /* [unique][out][string] */ 
            __out_opt  PWSTR Buffer,
            /* [out][in] */ 
            __inout  DWORD *pdwSizeInChars);
        
        void ( STDMETHODCALLTYPE *GetDefaultIoTarget )( 
            IWDFDevice * This,
            /* [out] */ 
            __out  IWDFIoTarget **ppWdfIoTarget);
        
        HRESULT ( STDMETHODCALLTYPE *CreateWdfFile )( 
            IWDFDevice * This,
            /* [string][unique][in] */ 
            __in_opt  LPCWSTR pcwszFileName,
            /* [out] */ 
            __out  IWDFDriverCreatedFile **ppFile);
        
        void ( STDMETHODCALLTYPE *GetDefaultIoQueue )( 
            IWDFDevice * This,
            /* [out] */ 
            __out  IWDFIoQueue **ppWdfIoQueue);
        
        HRESULT ( STDMETHODCALLTYPE *CreateIoQueue )( 
            IWDFDevice * This,
            /* [in] */ 
            __in  IUnknown *pCallbackInterface,
            /* [in] */ 
            __in  BOOL bDefaultQueue,
            /* [in] */ 
            __in  WDF_IO_QUEUE_DISPATCH_TYPE DispatchType,
            /* [in] */ 
            __in  BOOL bPowerManaged,
            /* [in] */ 
            __in  BOOL bAllowZeroLengthRequests,
            /* [out] */ 
            __out  IWDFIoQueue **ppIoQueue);
        
        HRESULT ( STDMETHODCALLTYPE *CreateDeviceInterface )( 
            IWDFDevice * This,
            /* [in] */ 
            __in  LPCGUID pDeviceInterfaceGuid,
            /* [unique][string][in] */ 
            __in_opt  PCWSTR pReferenceString);
        
        HRESULT ( STDMETHODCALLTYPE *AssignDeviceInterfaceState )( 
            IWDFDevice * This,
            /* [in] */ 
            __in  LPCGUID pDeviceInterfaceGuid,
            /* [unique][string][in] */ 
            __in_opt  PCWSTR pReferenceString,
            /* [in] */ 
            __in  BOOL Enable);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveDeviceName )( 
            IWDFDevice * This,
            /* [unique][out][string] */ 
            __out_ecount_part_opt(*pdwDeviceNameLength, *pdwDeviceNameLength)  PWSTR pDeviceName,
            /* [out][in] */ 
            __inout  DWORD *pdwDeviceNameLength);
        
        HRESULT ( STDMETHODCALLTYPE *PostEvent )( 
            IWDFDevice * This,
            /* [in] */ 
            __in  REFGUID EventGuid,
            /* [in] */ 
            __in  WDF_EVENT_TYPE EventType,
            /* [size_is][in] */ 
            __in_bcount(cbDataSize)  BYTE *pbData,
            /* [in] */ 
            __in  DWORD cbDataSize);
        
        HRESULT ( STDMETHODCALLTYPE *ConfigureRequestDispatching )( 
            IWDFDevice * This,
            /* [in] */ 
            __in  IWDFIoQueue *pQueue,
            /* [in] */ 
            __in  WDF_REQUEST_TYPE RequestType,
            /* [in] */ 
            __in  BOOL Forward);
        
        void ( STDMETHODCALLTYPE *SetPnpState )( 
            IWDFDevice * This,
            /* [in] */ 
            __in  WDF_PNP_STATE State,
            /* [in] */ 
            __in  WDF_TRI_STATE Value);
        
        WDF_TRI_STATE ( STDMETHODCALLTYPE *GetPnpState )( 
            IWDFDevice * This,
            /* [in] */ 
            __in  WDF_PNP_STATE State);
        
        void ( STDMETHODCALLTYPE *CommitPnpState )( 
            IWDFDevice * This);
        
        HRESULT ( STDMETHODCALLTYPE *CreateRequest )( 
            IWDFDevice * This,
            /* [unique][in] */ 
            __in_opt  IUnknown *pCallbackInterface,
            /* [unique][in] */ 
            __in_opt  IWDFObject *pParentObject,
            /* [out] */ 
            __out  IWDFIoRequest **ppRequest);
        
        HRESULT ( STDMETHODCALLTYPE *CreateSymbolicLink )( 
            IWDFDevice * This,
            /* [unique][string][in] */ 
            __in  PCWSTR pSymbolicLink);
        
        END_INTERFACE
    } IWDFDeviceVtbl;

    interface IWDFDevice
    {
        CONST_VTBL struct IWDFDeviceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWDFDevice_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWDFDevice_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWDFDevice_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWDFDevice_DeleteWdfObject(This)	\
    ( (This)->lpVtbl -> DeleteWdfObject(This) ) 

#define IWDFDevice_AssignContext(This,pCleanupCallback,pContext)	\
    ( (This)->lpVtbl -> AssignContext(This,pCleanupCallback,pContext) ) 

#define IWDFDevice_RetrieveContext(This,ppvContext)	\
    ( (This)->lpVtbl -> RetrieveContext(This,ppvContext) ) 

#define IWDFDevice_AcquireLock(This)	\
    ( (This)->lpVtbl -> AcquireLock(This) ) 

#define IWDFDevice_ReleaseLock(This)	\
    ( (This)->lpVtbl -> ReleaseLock(This) ) 


#define IWDFDevice_RetrieveDevicePropertyStore(This,pcwszServiceName,Flags,ppPropStore,pDisposition)	\
    ( (This)->lpVtbl -> RetrieveDevicePropertyStore(This,pcwszServiceName,Flags,ppPropStore,pDisposition) ) 

#define IWDFDevice_GetDriver(This,ppWdfDriver)	\
    ( (This)->lpVtbl -> GetDriver(This,ppWdfDriver) ) 

#define IWDFDevice_RetrieveDeviceInstanceId(This,Buffer,pdwSizeInChars)	\
    ( (This)->lpVtbl -> RetrieveDeviceInstanceId(This,Buffer,pdwSizeInChars) ) 

#define IWDFDevice_GetDefaultIoTarget(This,ppWdfIoTarget)	\
    ( (This)->lpVtbl -> GetDefaultIoTarget(This,ppWdfIoTarget) ) 

#define IWDFDevice_CreateWdfFile(This,pcwszFileName,ppFile)	\
    ( (This)->lpVtbl -> CreateWdfFile(This,pcwszFileName,ppFile) ) 

#define IWDFDevice_GetDefaultIoQueue(This,ppWdfIoQueue)	\
    ( (This)->lpVtbl -> GetDefaultIoQueue(This,ppWdfIoQueue) ) 

#define IWDFDevice_CreateIoQueue(This,pCallbackInterface,bDefaultQueue,DispatchType,bPowerManaged,bAllowZeroLengthRequests,ppIoQueue)	\
    ( (This)->lpVtbl -> CreateIoQueue(This,pCallbackInterface,bDefaultQueue,DispatchType,bPowerManaged,bAllowZeroLengthRequests,ppIoQueue) ) 

#define IWDFDevice_CreateDeviceInterface(This,pDeviceInterfaceGuid,pReferenceString)	\
    ( (This)->lpVtbl -> CreateDeviceInterface(This,pDeviceInterfaceGuid,pReferenceString) ) 

#define IWDFDevice_AssignDeviceInterfaceState(This,pDeviceInterfaceGuid,pReferenceString,Enable)	\
    ( (This)->lpVtbl -> AssignDeviceInterfaceState(This,pDeviceInterfaceGuid,pReferenceString,Enable) ) 

#define IWDFDevice_RetrieveDeviceName(This,pDeviceName,pdwDeviceNameLength)	\
    ( (This)->lpVtbl -> RetrieveDeviceName(This,pDeviceName,pdwDeviceNameLength) ) 

#define IWDFDevice_PostEvent(This,EventGuid,EventType,pbData,cbDataSize)	\
    ( (This)->lpVtbl -> PostEvent(This,EventGuid,EventType,pbData,cbDataSize) ) 

#define IWDFDevice_ConfigureRequestDispatching(This,pQueue,RequestType,Forward)	\
    ( (This)->lpVtbl -> ConfigureRequestDispatching(This,pQueue,RequestType,Forward) ) 

#define IWDFDevice_SetPnpState(This,State,Value)	\
    ( (This)->lpVtbl -> SetPnpState(This,State,Value) ) 

#define IWDFDevice_GetPnpState(This,State)	\
    ( (This)->lpVtbl -> GetPnpState(This,State) ) 

#define IWDFDevice_CommitPnpState(This)	\
    ( (This)->lpVtbl -> CommitPnpState(This) ) 

#define IWDFDevice_CreateRequest(This,pCallbackInterface,pParentObject,ppRequest)	\
    ( (This)->lpVtbl -> CreateRequest(This,pCallbackInterface,pParentObject,ppRequest) ) 

#define IWDFDevice_CreateSymbolicLink(This,pSymbolicLink)	\
    ( (This)->lpVtbl -> CreateSymbolicLink(This,pSymbolicLink) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWDFDevice_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_wudfddi_0000_0004 */
/* [local] */ 

#ifdef MIDL_PASS
typedef void *PROPVARIANT;

#else
typedef struct tagPROPVARIANT PROPVARIANT;
#endif


extern RPC_IF_HANDLE __MIDL_itf_wudfddi_0000_0004_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wudfddi_0000_0004_v0_0_s_ifspec;

#ifndef __IWDFNamedPropertyStore_INTERFACE_DEFINED__
#define __IWDFNamedPropertyStore_INTERFACE_DEFINED__

/* interface IWDFNamedPropertyStore */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_IWDFNamedPropertyStore;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("394b48c9-bca0-498f-8e2c-01225464a932")
    IWDFNamedPropertyStore : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetNamedValue( 
            /* [string][in] */ 
            __in  LPCWSTR pszName,
            /* [out] */ 
            __out  PROPVARIANT *pv) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetNamedValue( 
            /* [string][in] */ 
            __in  LPCWSTR pszName,
            /* [in] */ 
            __in  const PROPVARIANT *pv) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNameCount( 
            /* [out] */ 
            __out  DWORD *pdwCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNameAt( 
            /* [in] */ 
            __in  DWORD iProp,
            /* [string][out] */ 
            __out  PWSTR *ppwszName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWDFNamedPropertyStoreVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWDFNamedPropertyStore * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWDFNamedPropertyStore * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWDFNamedPropertyStore * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetNamedValue )( 
            IWDFNamedPropertyStore * This,
            /* [string][in] */ 
            __in  LPCWSTR pszName,
            /* [out] */ 
            __out  PROPVARIANT *pv);
        
        HRESULT ( STDMETHODCALLTYPE *SetNamedValue )( 
            IWDFNamedPropertyStore * This,
            /* [string][in] */ 
            __in  LPCWSTR pszName,
            /* [in] */ 
            __in  const PROPVARIANT *pv);
        
        HRESULT ( STDMETHODCALLTYPE *GetNameCount )( 
            IWDFNamedPropertyStore * This,
            /* [out] */ 
            __out  DWORD *pdwCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetNameAt )( 
            IWDFNamedPropertyStore * This,
            /* [in] */ 
            __in  DWORD iProp,
            /* [string][out] */ 
            __out  PWSTR *ppwszName);
        
        END_INTERFACE
    } IWDFNamedPropertyStoreVtbl;

    interface IWDFNamedPropertyStore
    {
        CONST_VTBL struct IWDFNamedPropertyStoreVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWDFNamedPropertyStore_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWDFNamedPropertyStore_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWDFNamedPropertyStore_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWDFNamedPropertyStore_GetNamedValue(This,pszName,pv)	\
    ( (This)->lpVtbl -> GetNamedValue(This,pszName,pv) ) 

#define IWDFNamedPropertyStore_SetNamedValue(This,pszName,pv)	\
    ( (This)->lpVtbl -> SetNamedValue(This,pszName,pv) ) 

#define IWDFNamedPropertyStore_GetNameCount(This,pdwCount)	\
    ( (This)->lpVtbl -> GetNameCount(This,pdwCount) ) 

#define IWDFNamedPropertyStore_GetNameAt(This,iProp,ppwszName)	\
    ( (This)->lpVtbl -> GetNameAt(This,iProp,ppwszName) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWDFNamedPropertyStore_INTERFACE_DEFINED__ */


#ifndef __IWDFIoQueue_INTERFACE_DEFINED__
#define __IWDFIoQueue_INTERFACE_DEFINED__

/* interface IWDFIoQueue */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWDFIoQueue;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("AE1162B9-8B11-4714-993D-93DC48CC9E8A")
    IWDFIoQueue : public IWDFObject
    {
    public:
        virtual void STDMETHODCALLTYPE GetDevice( 
            /* [out] */ 
            __out  IWDFDevice **ppWdfDevice) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ConfigureRequestDispatching( 
            /* [in] */ 
            __in  WDF_REQUEST_TYPE RequestType,
            /* [in] */ 
            __in  BOOL Forward) = 0;
        
        virtual WDF_IO_QUEUE_STATE STDMETHODCALLTYPE GetState( 
            /* [out] */ 
            __out  ULONG *pulNumOfRequestsInQueue,
            /* [out] */ 
            __out  ULONG *pulNumOfRequestsInDriver) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RetrieveNextRequest( 
            /* [out] */ 
            __out  IWDFIoRequest **ppRequest) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RetrieveNextRequestByFileObject( 
            /* [in] */ 
            __in  IWDFFile *pFile,
            /* [out] */ 
            __out  IWDFIoRequest **ppRequest) = 0;
        
        virtual void STDMETHODCALLTYPE Start( void) = 0;
        
        virtual void STDMETHODCALLTYPE Stop( 
            /* [unique][in] */ 
            __in_opt  IQueueCallbackStateChange *pStopComplete) = 0;
        
        virtual void STDMETHODCALLTYPE StopSynchronously( void) = 0;
        
        virtual void STDMETHODCALLTYPE Drain( 
            /* [unique][in] */ 
            __in_opt  IQueueCallbackStateChange *pDrainComplete) = 0;
        
        virtual void STDMETHODCALLTYPE DrainSynchronously( void) = 0;
        
        virtual void STDMETHODCALLTYPE Purge( 
            /* [unique][in] */ 
            __in_opt  IQueueCallbackStateChange *pPurgeComplete) = 0;
        
        virtual void STDMETHODCALLTYPE PurgeSynchronously( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWDFIoQueueVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWDFIoQueue * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWDFIoQueue * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWDFIoQueue * This);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteWdfObject )( 
            IWDFIoQueue * This);
        
        HRESULT ( STDMETHODCALLTYPE *AssignContext )( 
            IWDFIoQueue * This,
            /* [unique][in] */ 
            __in_opt  IObjectCleanup *pCleanupCallback,
            /* [unique][in] */ 
            __in_opt  void *pContext);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveContext )( 
            IWDFIoQueue * This,
            /* [out] */ 
            __out  void **ppvContext);
        
        void ( STDMETHODCALLTYPE *AcquireLock )( 
            IWDFIoQueue * This);
        
        void ( STDMETHODCALLTYPE *ReleaseLock )( 
            IWDFIoQueue * This);
        
        void ( STDMETHODCALLTYPE *GetDevice )( 
            IWDFIoQueue * This,
            /* [out] */ 
            __out  IWDFDevice **ppWdfDevice);
        
        HRESULT ( STDMETHODCALLTYPE *ConfigureRequestDispatching )( 
            IWDFIoQueue * This,
            /* [in] */ 
            __in  WDF_REQUEST_TYPE RequestType,
            /* [in] */ 
            __in  BOOL Forward);
        
        WDF_IO_QUEUE_STATE ( STDMETHODCALLTYPE *GetState )( 
            IWDFIoQueue * This,
            /* [out] */ 
            __out  ULONG *pulNumOfRequestsInQueue,
            /* [out] */ 
            __out  ULONG *pulNumOfRequestsInDriver);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveNextRequest )( 
            IWDFIoQueue * This,
            /* [out] */ 
            __out  IWDFIoRequest **ppRequest);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveNextRequestByFileObject )( 
            IWDFIoQueue * This,
            /* [in] */ 
            __in  IWDFFile *pFile,
            /* [out] */ 
            __out  IWDFIoRequest **ppRequest);
        
        void ( STDMETHODCALLTYPE *Start )( 
            IWDFIoQueue * This);
        
        void ( STDMETHODCALLTYPE *Stop )( 
            IWDFIoQueue * This,
            /* [unique][in] */ 
            __in_opt  IQueueCallbackStateChange *pStopComplete);
        
        void ( STDMETHODCALLTYPE *StopSynchronously )( 
            IWDFIoQueue * This);
        
        void ( STDMETHODCALLTYPE *Drain )( 
            IWDFIoQueue * This,
            /* [unique][in] */ 
            __in_opt  IQueueCallbackStateChange *pDrainComplete);
        
        void ( STDMETHODCALLTYPE *DrainSynchronously )( 
            IWDFIoQueue * This);
        
        void ( STDMETHODCALLTYPE *Purge )( 
            IWDFIoQueue * This,
            /* [unique][in] */ 
            __in_opt  IQueueCallbackStateChange *pPurgeComplete);
        
        void ( STDMETHODCALLTYPE *PurgeSynchronously )( 
            IWDFIoQueue * This);
        
        END_INTERFACE
    } IWDFIoQueueVtbl;

    interface IWDFIoQueue
    {
        CONST_VTBL struct IWDFIoQueueVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWDFIoQueue_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWDFIoQueue_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWDFIoQueue_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWDFIoQueue_DeleteWdfObject(This)	\
    ( (This)->lpVtbl -> DeleteWdfObject(This) ) 

#define IWDFIoQueue_AssignContext(This,pCleanupCallback,pContext)	\
    ( (This)->lpVtbl -> AssignContext(This,pCleanupCallback,pContext) ) 

#define IWDFIoQueue_RetrieveContext(This,ppvContext)	\
    ( (This)->lpVtbl -> RetrieveContext(This,ppvContext) ) 

#define IWDFIoQueue_AcquireLock(This)	\
    ( (This)->lpVtbl -> AcquireLock(This) ) 

#define IWDFIoQueue_ReleaseLock(This)	\
    ( (This)->lpVtbl -> ReleaseLock(This) ) 


#define IWDFIoQueue_GetDevice(This,ppWdfDevice)	\
    ( (This)->lpVtbl -> GetDevice(This,ppWdfDevice) ) 

#define IWDFIoQueue_ConfigureRequestDispatching(This,RequestType,Forward)	\
    ( (This)->lpVtbl -> ConfigureRequestDispatching(This,RequestType,Forward) ) 

#define IWDFIoQueue_GetState(This,pulNumOfRequestsInQueue,pulNumOfRequestsInDriver)	\
    ( (This)->lpVtbl -> GetState(This,pulNumOfRequestsInQueue,pulNumOfRequestsInDriver) ) 

#define IWDFIoQueue_RetrieveNextRequest(This,ppRequest)	\
    ( (This)->lpVtbl -> RetrieveNextRequest(This,ppRequest) ) 

#define IWDFIoQueue_RetrieveNextRequestByFileObject(This,pFile,ppRequest)	\
    ( (This)->lpVtbl -> RetrieveNextRequestByFileObject(This,pFile,ppRequest) ) 

#define IWDFIoQueue_Start(This)	\
    ( (This)->lpVtbl -> Start(This) ) 

#define IWDFIoQueue_Stop(This,pStopComplete)	\
    ( (This)->lpVtbl -> Stop(This,pStopComplete) ) 

#define IWDFIoQueue_StopSynchronously(This)	\
    ( (This)->lpVtbl -> StopSynchronously(This) ) 

#define IWDFIoQueue_Drain(This,pDrainComplete)	\
    ( (This)->lpVtbl -> Drain(This,pDrainComplete) ) 

#define IWDFIoQueue_DrainSynchronously(This)	\
    ( (This)->lpVtbl -> DrainSynchronously(This) ) 

#define IWDFIoQueue_Purge(This,pPurgeComplete)	\
    ( (This)->lpVtbl -> Purge(This,pPurgeComplete) ) 

#define IWDFIoQueue_PurgeSynchronously(This)	\
    ( (This)->lpVtbl -> PurgeSynchronously(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWDFIoQueue_INTERFACE_DEFINED__ */


#ifndef __IWDFIoRequest_INTERFACE_DEFINED__
#define __IWDFIoRequest_INTERFACE_DEFINED__

/* interface IWDFIoRequest */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWDFIoRequest;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("896DF312-22B4-4a9d-95DD-A364AAF59769")
    IWDFIoRequest : public IWDFObject
    {
    public:
        virtual void STDMETHODCALLTYPE CompleteWithInformation( 
            /* [in] */ 
            __in  HRESULT CompletionStatus,
            /* [in] */ 
            __in  SIZE_T Information) = 0;
        
        virtual void STDMETHODCALLTYPE SetInformation( 
            /* [in] */ 
            __in  ULONG_PTR Information) = 0;
        
        virtual void STDMETHODCALLTYPE Complete( 
            /* [in] */ 
            __in  HRESULT CompletionStatus) = 0;
        
        virtual void STDMETHODCALLTYPE SetCompletionCallback( 
            /* [in] */ 
            __in  IRequestCallbackRequestCompletion *pCompletionCallback,
            /* [unique][in] */ 
            __in_opt  void *pContext) = 0;
        
        virtual WDF_REQUEST_TYPE STDMETHODCALLTYPE GetType( void) = 0;
        
        virtual void STDMETHODCALLTYPE GetCreateParameters( 
            /* [unique][out] */ 
            __out_opt  ULONG *pOptions,
            /* [unique][out] */ 
            __out_opt  USHORT *pFileAttributes,
            /* [unique][out] */ 
            __out_opt  USHORT *pShareAccess) = 0;
        
        virtual void STDMETHODCALLTYPE GetReadParameters( 
            /* [unique][out] */ 
            __out_opt  SIZE_T *pSizeInBytes,
            /* [unique][out] */ 
            __out_opt  LONGLONG *pullOffset,
            /* [unique][out] */ 
            __out_opt  ULONG *pulKey) = 0;
        
        virtual void STDMETHODCALLTYPE GetWriteParameters( 
            /* [unique][out] */ 
            __out_opt  SIZE_T *pSizeInBytes,
            /* [unique][out] */ 
            __out_opt  LONGLONG *pullOffset,
            /* [unique][out] */ 
            __out_opt  ULONG *pulKey) = 0;
        
        virtual void STDMETHODCALLTYPE GetDeviceIoControlParameters( 
            /* [unique][out] */ 
            __out_opt  ULONG *pControlCode,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pInBufferSize,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pOutBufferSize) = 0;
        
        virtual void STDMETHODCALLTYPE GetOutputMemory( 
            /* [out] */ 
            __out  IWDFMemory **ppWdfMemory) = 0;
        
        virtual void STDMETHODCALLTYPE GetInputMemory( 
            /* [out] */ 
            __out  IWDFMemory **ppWdfMemory) = 0;
        
        virtual void STDMETHODCALLTYPE MarkCancelable( 
            /* [in] */ 
            __in  IRequestCallbackCancel *pCancelCallback) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnmarkCancelable( void) = 0;
        
        virtual BOOL STDMETHODCALLTYPE CancelSentRequest( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ForwardToIoQueue( 
            /* [in] */ 
            __in  IWDFIoQueue *pDestination) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Send( 
            /* [in] */ 
            __in  IWDFIoTarget *pIoTarget,
            /* [in] */ 
            __in  ULONG Flags,
            /* [in] */ 
            __in  LONGLONG Timeout) = 0;
        
        virtual void STDMETHODCALLTYPE GetFileObject( 
            /* [out] */ 
            __out  IWDFFile **ppFileObject) = 0;
        
        virtual void STDMETHODCALLTYPE FormatUsingCurrentType( void) = 0;
        
        virtual ULONG STDMETHODCALLTYPE GetRequestorProcessId( void) = 0;
        
        virtual void STDMETHODCALLTYPE GetIoQueue( 
            /* [out] */ 
            __out  IWDFIoQueue **ppWdfIoQueue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Impersonate( 
            /* [in] */ 
            __in  SECURITY_IMPERSONATION_LEVEL ImpersonationLevel,
            /* [in] */ 
            __in  IImpersonateCallback *pCallback,
            /* [unique][in] */ 
            __in_opt  void *pvCallbackContext) = 0;
        
        virtual BOOL STDMETHODCALLTYPE IsFrom32BitProcess( void) = 0;
        
        virtual void STDMETHODCALLTYPE GetCompletionParams( 
            /* [out] */ 
            __out  IWDFRequestCompletionParams **ppCompletionParams) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWDFIoRequestVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWDFIoRequest * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWDFIoRequest * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWDFIoRequest * This);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteWdfObject )( 
            IWDFIoRequest * This);
        
        HRESULT ( STDMETHODCALLTYPE *AssignContext )( 
            IWDFIoRequest * This,
            /* [unique][in] */ 
            __in_opt  IObjectCleanup *pCleanupCallback,
            /* [unique][in] */ 
            __in_opt  void *pContext);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveContext )( 
            IWDFIoRequest * This,
            /* [out] */ 
            __out  void **ppvContext);
        
        void ( STDMETHODCALLTYPE *AcquireLock )( 
            IWDFIoRequest * This);
        
        void ( STDMETHODCALLTYPE *ReleaseLock )( 
            IWDFIoRequest * This);
        
        void ( STDMETHODCALLTYPE *CompleteWithInformation )( 
            IWDFIoRequest * This,
            /* [in] */ 
            __in  HRESULT CompletionStatus,
            /* [in] */ 
            __in  SIZE_T Information);
        
        void ( STDMETHODCALLTYPE *SetInformation )( 
            IWDFIoRequest * This,
            /* [in] */ 
            __in  ULONG_PTR Information);
        
        void ( STDMETHODCALLTYPE *Complete )( 
            IWDFIoRequest * This,
            /* [in] */ 
            __in  HRESULT CompletionStatus);
        
        void ( STDMETHODCALLTYPE *SetCompletionCallback )( 
            IWDFIoRequest * This,
            /* [in] */ 
            __in  IRequestCallbackRequestCompletion *pCompletionCallback,
            /* [unique][in] */ 
            __in_opt  void *pContext);
        
        WDF_REQUEST_TYPE ( STDMETHODCALLTYPE *GetType )( 
            IWDFIoRequest * This);
        
        void ( STDMETHODCALLTYPE *GetCreateParameters )( 
            IWDFIoRequest * This,
            /* [unique][out] */ 
            __out_opt  ULONG *pOptions,
            /* [unique][out] */ 
            __out_opt  USHORT *pFileAttributes,
            /* [unique][out] */ 
            __out_opt  USHORT *pShareAccess);
        
        void ( STDMETHODCALLTYPE *GetReadParameters )( 
            IWDFIoRequest * This,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pSizeInBytes,
            /* [unique][out] */ 
            __out_opt  LONGLONG *pullOffset,
            /* [unique][out] */ 
            __out_opt  ULONG *pulKey);
        
        void ( STDMETHODCALLTYPE *GetWriteParameters )( 
            IWDFIoRequest * This,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pSizeInBytes,
            /* [unique][out] */ 
            __out_opt  LONGLONG *pullOffset,
            /* [unique][out] */ 
            __out_opt  ULONG *pulKey);
        
        void ( STDMETHODCALLTYPE *GetDeviceIoControlParameters )( 
            IWDFIoRequest * This,
            /* [unique][out] */ 
            __out_opt  ULONG *pControlCode,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pInBufferSize,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pOutBufferSize);
        
        void ( STDMETHODCALLTYPE *GetOutputMemory )( 
            IWDFIoRequest * This,
            /* [out] */ 
            __out  IWDFMemory **ppWdfMemory);
        
        void ( STDMETHODCALLTYPE *GetInputMemory )( 
            IWDFIoRequest * This,
            /* [out] */ 
            __out  IWDFMemory **ppWdfMemory);
        
        void ( STDMETHODCALLTYPE *MarkCancelable )( 
            IWDFIoRequest * This,
            /* [in] */ 
            __in  IRequestCallbackCancel *pCancelCallback);
        
        HRESULT ( STDMETHODCALLTYPE *UnmarkCancelable )( 
            IWDFIoRequest * This);
        
        BOOL ( STDMETHODCALLTYPE *CancelSentRequest )( 
            IWDFIoRequest * This);
        
        HRESULT ( STDMETHODCALLTYPE *ForwardToIoQueue )( 
            IWDFIoRequest * This,
            /* [in] */ 
            __in  IWDFIoQueue *pDestination);
        
        HRESULT ( STDMETHODCALLTYPE *Send )( 
            IWDFIoRequest * This,
            /* [in] */ 
            __in  IWDFIoTarget *pIoTarget,
            /* [in] */ 
            __in  ULONG Flags,
            /* [in] */ 
            __in  LONGLONG Timeout);
        
        void ( STDMETHODCALLTYPE *GetFileObject )( 
            IWDFIoRequest * This,
            /* [out] */ 
            __out  IWDFFile **ppFileObject);
        
        void ( STDMETHODCALLTYPE *FormatUsingCurrentType )( 
            IWDFIoRequest * This);
        
        ULONG ( STDMETHODCALLTYPE *GetRequestorProcessId )( 
            IWDFIoRequest * This);
        
        void ( STDMETHODCALLTYPE *GetIoQueue )( 
            IWDFIoRequest * This,
            /* [out] */ 
            __out  IWDFIoQueue **ppWdfIoQueue);
        
        HRESULT ( STDMETHODCALLTYPE *Impersonate )( 
            IWDFIoRequest * This,
            /* [in] */ 
            __in  SECURITY_IMPERSONATION_LEVEL ImpersonationLevel,
            /* [in] */ 
            __in  IImpersonateCallback *pCallback,
            /* [unique][in] */ 
            __in_opt  void *pvCallbackContext);
        
        BOOL ( STDMETHODCALLTYPE *IsFrom32BitProcess )( 
            IWDFIoRequest * This);
        
        void ( STDMETHODCALLTYPE *GetCompletionParams )( 
            IWDFIoRequest * This,
            /* [out] */ 
            __out  IWDFRequestCompletionParams **ppCompletionParams);
        
        END_INTERFACE
    } IWDFIoRequestVtbl;

    interface IWDFIoRequest
    {
        CONST_VTBL struct IWDFIoRequestVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWDFIoRequest_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWDFIoRequest_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWDFIoRequest_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWDFIoRequest_DeleteWdfObject(This)	\
    ( (This)->lpVtbl -> DeleteWdfObject(This) ) 

#define IWDFIoRequest_AssignContext(This,pCleanupCallback,pContext)	\
    ( (This)->lpVtbl -> AssignContext(This,pCleanupCallback,pContext) ) 

#define IWDFIoRequest_RetrieveContext(This,ppvContext)	\
    ( (This)->lpVtbl -> RetrieveContext(This,ppvContext) ) 

#define IWDFIoRequest_AcquireLock(This)	\
    ( (This)->lpVtbl -> AcquireLock(This) ) 

#define IWDFIoRequest_ReleaseLock(This)	\
    ( (This)->lpVtbl -> ReleaseLock(This) ) 


#define IWDFIoRequest_CompleteWithInformation(This,CompletionStatus,Information)	\
    ( (This)->lpVtbl -> CompleteWithInformation(This,CompletionStatus,Information) ) 

#define IWDFIoRequest_SetInformation(This,Information)	\
    ( (This)->lpVtbl -> SetInformation(This,Information) ) 

#define IWDFIoRequest_Complete(This,CompletionStatus)	\
    ( (This)->lpVtbl -> Complete(This,CompletionStatus) ) 

#define IWDFIoRequest_SetCompletionCallback(This,pCompletionCallback,pContext)	\
    ( (This)->lpVtbl -> SetCompletionCallback(This,pCompletionCallback,pContext) ) 

#define IWDFIoRequest_GetType(This)	\
    ( (This)->lpVtbl -> GetType(This) ) 

#define IWDFIoRequest_GetCreateParameters(This,pOptions,pFileAttributes,pShareAccess)	\
    ( (This)->lpVtbl -> GetCreateParameters(This,pOptions,pFileAttributes,pShareAccess) ) 

#define IWDFIoRequest_GetReadParameters(This,pSizeInBytes,pullOffset,pulKey)	\
    ( (This)->lpVtbl -> GetReadParameters(This,pSizeInBytes,pullOffset,pulKey) ) 

#define IWDFIoRequest_GetWriteParameters(This,pSizeInBytes,pullOffset,pulKey)	\
    ( (This)->lpVtbl -> GetWriteParameters(This,pSizeInBytes,pullOffset,pulKey) ) 

#define IWDFIoRequest_GetDeviceIoControlParameters(This,pControlCode,pInBufferSize,pOutBufferSize)	\
    ( (This)->lpVtbl -> GetDeviceIoControlParameters(This,pControlCode,pInBufferSize,pOutBufferSize) ) 

#define IWDFIoRequest_GetOutputMemory(This,ppWdfMemory)	\
    ( (This)->lpVtbl -> GetOutputMemory(This,ppWdfMemory) ) 

#define IWDFIoRequest_GetInputMemory(This,ppWdfMemory)	\
    ( (This)->lpVtbl -> GetInputMemory(This,ppWdfMemory) ) 

#define IWDFIoRequest_MarkCancelable(This,pCancelCallback)	\
    ( (This)->lpVtbl -> MarkCancelable(This,pCancelCallback) ) 

#define IWDFIoRequest_UnmarkCancelable(This)	\
    ( (This)->lpVtbl -> UnmarkCancelable(This) ) 

#define IWDFIoRequest_CancelSentRequest(This)	\
    ( (This)->lpVtbl -> CancelSentRequest(This) ) 

#define IWDFIoRequest_ForwardToIoQueue(This,pDestination)	\
    ( (This)->lpVtbl -> ForwardToIoQueue(This,pDestination) ) 

#define IWDFIoRequest_Send(This,pIoTarget,Flags,Timeout)	\
    ( (This)->lpVtbl -> Send(This,pIoTarget,Flags,Timeout) ) 

#define IWDFIoRequest_GetFileObject(This,ppFileObject)	\
    ( (This)->lpVtbl -> GetFileObject(This,ppFileObject) ) 

#define IWDFIoRequest_FormatUsingCurrentType(This)	\
    ( (This)->lpVtbl -> FormatUsingCurrentType(This) ) 

#define IWDFIoRequest_GetRequestorProcessId(This)	\
    ( (This)->lpVtbl -> GetRequestorProcessId(This) ) 

#define IWDFIoRequest_GetIoQueue(This,ppWdfIoQueue)	\
    ( (This)->lpVtbl -> GetIoQueue(This,ppWdfIoQueue) ) 

#define IWDFIoRequest_Impersonate(This,ImpersonationLevel,pCallback,pvCallbackContext)	\
    ( (This)->lpVtbl -> Impersonate(This,ImpersonationLevel,pCallback,pvCallbackContext) ) 

#define IWDFIoRequest_IsFrom32BitProcess(This)	\
    ( (This)->lpVtbl -> IsFrom32BitProcess(This) ) 

#define IWDFIoRequest_GetCompletionParams(This,ppCompletionParams)	\
    ( (This)->lpVtbl -> GetCompletionParams(This,ppCompletionParams) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWDFIoRequest_INTERFACE_DEFINED__ */


#ifndef __IWDFRequestCompletionParams_INTERFACE_DEFINED__
#define __IWDFRequestCompletionParams_INTERFACE_DEFINED__

/* interface IWDFRequestCompletionParams */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWDFRequestCompletionParams;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("707a2b42-69b8-4971-a49c-4031861e7aff")
    IWDFRequestCompletionParams : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetCompletionStatus( void) = 0;
        
        virtual ULONG_PTR STDMETHODCALLTYPE GetInformation( void) = 0;
        
        virtual WDF_REQUEST_TYPE STDMETHODCALLTYPE GetCompletedRequestType( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWDFRequestCompletionParamsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWDFRequestCompletionParams * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWDFRequestCompletionParams * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWDFRequestCompletionParams * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCompletionStatus )( 
            IWDFRequestCompletionParams * This);
        
        ULONG_PTR ( STDMETHODCALLTYPE *GetInformation )( 
            IWDFRequestCompletionParams * This);
        
        WDF_REQUEST_TYPE ( STDMETHODCALLTYPE *GetCompletedRequestType )( 
            IWDFRequestCompletionParams * This);
        
        END_INTERFACE
    } IWDFRequestCompletionParamsVtbl;

    interface IWDFRequestCompletionParams
    {
        CONST_VTBL struct IWDFRequestCompletionParamsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWDFRequestCompletionParams_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWDFRequestCompletionParams_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWDFRequestCompletionParams_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWDFRequestCompletionParams_GetCompletionStatus(This)	\
    ( (This)->lpVtbl -> GetCompletionStatus(This) ) 

#define IWDFRequestCompletionParams_GetInformation(This)	\
    ( (This)->lpVtbl -> GetInformation(This) ) 

#define IWDFRequestCompletionParams_GetCompletedRequestType(This)	\
    ( (This)->lpVtbl -> GetCompletedRequestType(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWDFRequestCompletionParams_INTERFACE_DEFINED__ */


#ifndef __IWDFIoRequestCompletionParams_INTERFACE_DEFINED__
#define __IWDFIoRequestCompletionParams_INTERFACE_DEFINED__

/* interface IWDFIoRequestCompletionParams */
/* [unique][restricted][local][uuid][object] */ 


EXTERN_C const IID IID_IWDFIoRequestCompletionParams;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("723d4f3c-7722-4c49-99ae-195937bd9582")
    IWDFIoRequestCompletionParams : public IWDFRequestCompletionParams
    {
    public:
        virtual void STDMETHODCALLTYPE GetWriteParameters( 
            /* [unique][out] */ 
            __out_opt  IWDFMemory **ppWriteMemory,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pBytesWritten,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pWriteMemoryOffset) = 0;
        
        virtual void STDMETHODCALLTYPE GetReadParameters( 
            /* [unique][out] */ 
            __out_opt  IWDFMemory **ppReadMemory,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pBytesRead,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pReadMemoryOffset) = 0;
        
        virtual void STDMETHODCALLTYPE GetIoctlParameters( 
            /* [unique][out] */ 
            __out_opt  ULONG *pIoControlCode,
            /* [unique][out] */ 
            __out_opt  IWDFMemory **ppInputMemory,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pInputMemoryOffset,
            /* [unique][out] */ 
            __out_opt  IWDFMemory **ppOutputMemory,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pOutputMemoryOffset,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pOutBytes) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWDFIoRequestCompletionParamsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWDFIoRequestCompletionParams * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWDFIoRequestCompletionParams * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWDFIoRequestCompletionParams * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCompletionStatus )( 
            IWDFIoRequestCompletionParams * This);
        
        ULONG_PTR ( STDMETHODCALLTYPE *GetInformation )( 
            IWDFIoRequestCompletionParams * This);
        
        WDF_REQUEST_TYPE ( STDMETHODCALLTYPE *GetCompletedRequestType )( 
            IWDFIoRequestCompletionParams * This);
        
        void ( STDMETHODCALLTYPE *GetWriteParameters )( 
            IWDFIoRequestCompletionParams * This,
            /* [unique][out] */ 
            __out_opt  IWDFMemory **ppWriteMemory,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pBytesWritten,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pWriteMemoryOffset);
        
        void ( STDMETHODCALLTYPE *GetReadParameters )( 
            IWDFIoRequestCompletionParams * This,
            /* [unique][out] */ 
            __out_opt  IWDFMemory **ppReadMemory,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pBytesRead,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pReadMemoryOffset);
        
        void ( STDMETHODCALLTYPE *GetIoctlParameters )( 
            IWDFIoRequestCompletionParams * This,
            /* [unique][out] */ 
            __out_opt  ULONG *pIoControlCode,
            /* [unique][out] */ 
            __out_opt  IWDFMemory **ppInputMemory,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pInputMemoryOffset,
            /* [unique][out] */ 
            __out_opt  IWDFMemory **ppOutputMemory,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pOutputMemoryOffset,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pOutBytes);
        
        END_INTERFACE
    } IWDFIoRequestCompletionParamsVtbl;

    interface IWDFIoRequestCompletionParams
    {
        CONST_VTBL struct IWDFIoRequestCompletionParamsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWDFIoRequestCompletionParams_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWDFIoRequestCompletionParams_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWDFIoRequestCompletionParams_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWDFIoRequestCompletionParams_GetCompletionStatus(This)	\
    ( (This)->lpVtbl -> GetCompletionStatus(This) ) 

#define IWDFIoRequestCompletionParams_GetInformation(This)	\
    ( (This)->lpVtbl -> GetInformation(This) ) 

#define IWDFIoRequestCompletionParams_GetCompletedRequestType(This)	\
    ( (This)->lpVtbl -> GetCompletedRequestType(This) ) 


#define IWDFIoRequestCompletionParams_GetWriteParameters(This,ppWriteMemory,pBytesWritten,pWriteMemoryOffset)	\
    ( (This)->lpVtbl -> GetWriteParameters(This,ppWriteMemory,pBytesWritten,pWriteMemoryOffset) ) 

#define IWDFIoRequestCompletionParams_GetReadParameters(This,ppReadMemory,pBytesRead,pReadMemoryOffset)	\
    ( (This)->lpVtbl -> GetReadParameters(This,ppReadMemory,pBytesRead,pReadMemoryOffset) ) 

#define IWDFIoRequestCompletionParams_GetIoctlParameters(This,pIoControlCode,ppInputMemory,pInputMemoryOffset,ppOutputMemory,pOutputMemoryOffset,pOutBytes)	\
    ( (This)->lpVtbl -> GetIoctlParameters(This,pIoControlCode,ppInputMemory,pInputMemoryOffset,ppOutputMemory,pOutputMemoryOffset,pOutBytes) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWDFIoRequestCompletionParams_INTERFACE_DEFINED__ */


#ifndef __IWDFFile_INTERFACE_DEFINED__
#define __IWDFFile_INTERFACE_DEFINED__

/* interface IWDFFile */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWDFFile;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CC8FE04B-FE8B-4245-AFD6-C31BC830C791")
    IWDFFile : public IWDFObject
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE RetrieveFileName( 
            /* [unique][out][string] */ 
            __out_opt  PWSTR pFileName,
            /* [out][in] */ 
            __inout  DWORD *pdwFileNameLengthInChars) = 0;
        
        virtual void STDMETHODCALLTYPE GetDevice( 
            /* [out] */ 
            __out  IWDFDevice **ppWdfDevice) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWDFFileVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWDFFile * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWDFFile * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWDFFile * This);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteWdfObject )( 
            IWDFFile * This);
        
        HRESULT ( STDMETHODCALLTYPE *AssignContext )( 
            IWDFFile * This,
            /* [unique][in] */ 
            __in_opt  IObjectCleanup *pCleanupCallback,
            /* [unique][in] */ 
            __in_opt  void *pContext);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveContext )( 
            IWDFFile * This,
            /* [out] */ 
            __out  void **ppvContext);
        
        void ( STDMETHODCALLTYPE *AcquireLock )( 
            IWDFFile * This);
        
        void ( STDMETHODCALLTYPE *ReleaseLock )( 
            IWDFFile * This);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveFileName )( 
            IWDFFile * This,
            /* [unique][out][string] */ 
            __out_opt  PWSTR pFileName,
            /* [out][in] */ 
            __inout  DWORD *pdwFileNameLengthInChars);
        
        void ( STDMETHODCALLTYPE *GetDevice )( 
            IWDFFile * This,
            /* [out] */ 
            __out  IWDFDevice **ppWdfDevice);
        
        END_INTERFACE
    } IWDFFileVtbl;

    interface IWDFFile
    {
        CONST_VTBL struct IWDFFileVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWDFFile_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWDFFile_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWDFFile_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWDFFile_DeleteWdfObject(This)	\
    ( (This)->lpVtbl -> DeleteWdfObject(This) ) 

#define IWDFFile_AssignContext(This,pCleanupCallback,pContext)	\
    ( (This)->lpVtbl -> AssignContext(This,pCleanupCallback,pContext) ) 

#define IWDFFile_RetrieveContext(This,ppvContext)	\
    ( (This)->lpVtbl -> RetrieveContext(This,ppvContext) ) 

#define IWDFFile_AcquireLock(This)	\
    ( (This)->lpVtbl -> AcquireLock(This) ) 

#define IWDFFile_ReleaseLock(This)	\
    ( (This)->lpVtbl -> ReleaseLock(This) ) 


#define IWDFFile_RetrieveFileName(This,pFileName,pdwFileNameLengthInChars)	\
    ( (This)->lpVtbl -> RetrieveFileName(This,pFileName,pdwFileNameLengthInChars) ) 

#define IWDFFile_GetDevice(This,ppWdfDevice)	\
    ( (This)->lpVtbl -> GetDevice(This,ppWdfDevice) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWDFFile_INTERFACE_DEFINED__ */


#ifndef __IWDFDriverCreatedFile_INTERFACE_DEFINED__
#define __IWDFDriverCreatedFile_INTERFACE_DEFINED__

/* interface IWDFDriverCreatedFile */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWDFDriverCreatedFile;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("b7615d26-494b-47a6-b4cf-0271bcc3da4b")
    IWDFDriverCreatedFile : public IWDFFile
    {
    public:
        virtual void STDMETHODCALLTYPE Close( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWDFDriverCreatedFileVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWDFDriverCreatedFile * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWDFDriverCreatedFile * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWDFDriverCreatedFile * This);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteWdfObject )( 
            IWDFDriverCreatedFile * This);
        
        HRESULT ( STDMETHODCALLTYPE *AssignContext )( 
            IWDFDriverCreatedFile * This,
            /* [unique][in] */ 
            __in_opt  IObjectCleanup *pCleanupCallback,
            /* [unique][in] */ 
            __in_opt  void *pContext);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveContext )( 
            IWDFDriverCreatedFile * This,
            /* [out] */ 
            __out  void **ppvContext);
        
        void ( STDMETHODCALLTYPE *AcquireLock )( 
            IWDFDriverCreatedFile * This);
        
        void ( STDMETHODCALLTYPE *ReleaseLock )( 
            IWDFDriverCreatedFile * This);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveFileName )( 
            IWDFDriverCreatedFile * This,
            /* [unique][out][string] */ 
            __out_opt  PWSTR pFileName,
            /* [out][in] */ 
            __inout  DWORD *pdwFileNameLengthInChars);
        
        void ( STDMETHODCALLTYPE *GetDevice )( 
            IWDFDriverCreatedFile * This,
            /* [out] */ 
            __out  IWDFDevice **ppWdfDevice);
        
        void ( STDMETHODCALLTYPE *Close )( 
            IWDFDriverCreatedFile * This);
        
        END_INTERFACE
    } IWDFDriverCreatedFileVtbl;

    interface IWDFDriverCreatedFile
    {
        CONST_VTBL struct IWDFDriverCreatedFileVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWDFDriverCreatedFile_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWDFDriverCreatedFile_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWDFDriverCreatedFile_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWDFDriverCreatedFile_DeleteWdfObject(This)	\
    ( (This)->lpVtbl -> DeleteWdfObject(This) ) 

#define IWDFDriverCreatedFile_AssignContext(This,pCleanupCallback,pContext)	\
    ( (This)->lpVtbl -> AssignContext(This,pCleanupCallback,pContext) ) 

#define IWDFDriverCreatedFile_RetrieveContext(This,ppvContext)	\
    ( (This)->lpVtbl -> RetrieveContext(This,ppvContext) ) 

#define IWDFDriverCreatedFile_AcquireLock(This)	\
    ( (This)->lpVtbl -> AcquireLock(This) ) 

#define IWDFDriverCreatedFile_ReleaseLock(This)	\
    ( (This)->lpVtbl -> ReleaseLock(This) ) 


#define IWDFDriverCreatedFile_RetrieveFileName(This,pFileName,pdwFileNameLengthInChars)	\
    ( (This)->lpVtbl -> RetrieveFileName(This,pFileName,pdwFileNameLengthInChars) ) 

#define IWDFDriverCreatedFile_GetDevice(This,ppWdfDevice)	\
    ( (This)->lpVtbl -> GetDevice(This,ppWdfDevice) ) 


#define IWDFDriverCreatedFile_Close(This)	\
    ( (This)->lpVtbl -> Close(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWDFDriverCreatedFile_INTERFACE_DEFINED__ */


#ifndef __IWDFIoTarget_INTERFACE_DEFINED__
#define __IWDFIoTarget_INTERFACE_DEFINED__

/* interface IWDFIoTarget */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWDFIoTarget;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CC060D79-C0C2-407f-8B10-A5E900FC3474")
    IWDFIoTarget : public IWDFObject
    {
    public:
        virtual void STDMETHODCALLTYPE GetTargetFile( 
            /* [out] */ 
            __out  IWDFFile **ppWdfFile) = 0;
        
        virtual void STDMETHODCALLTYPE CancelSentRequestsForFile( 
            /* [in] */ 
            __in  IWDFFile *pFile) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FormatRequestForRead( 
            /* [in] */ 
            __in  IWDFIoRequest *pRequest,
            /* [unique][in] */ 
            __in_opt  IWDFFile *pFile,
            /* [unique][in] */ 
            __in_opt  IWDFMemory *pOutputMemory,
            /* [unique][in] */ 
            __in_opt  PWDFMEMORY_OFFSET pOutputMemoryOffset,
            /* [unique][in] */ 
            __in_opt  PLONGLONG DeviceOffset) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FormatRequestForWrite( 
            /* [in] */ 
            __in  IWDFIoRequest *pRequest,
            /* [unique][in] */ 
            __in_opt  IWDFFile *pFile,
            /* [unique][in] */ 
            __in_opt  IWDFMemory *pInputMemory,
            /* [unique][in] */ 
            __in_opt  PWDFMEMORY_OFFSET pInputMemoryOffset,
            /* [unique][in] */ 
            __in_opt  PLONGLONG DeviceOffset) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FormatRequestForIoctl( 
            /* [in] */ 
            __in  IWDFIoRequest *pRequest,
            /* [in] */ 
            __in  ULONG IoctlCode,
            /* [unique][in] */ 
            __in_opt  IWDFFile *pFile,
            /* [unique][in] */ 
            __in_opt  IWDFMemory *pInputMemory,
            /* [unique][in] */ 
            __in_opt  PWDFMEMORY_OFFSET pInputMemoryOffset,
            /* [unique][in] */ 
            __in_opt  IWDFMemory *pOutputMemory,
            /* [unique][in] */ 
            __in_opt  PWDFMEMORY_OFFSET pOutputMemoryOffset) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWDFIoTargetVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWDFIoTarget * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWDFIoTarget * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWDFIoTarget * This);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteWdfObject )( 
            IWDFIoTarget * This);
        
        HRESULT ( STDMETHODCALLTYPE *AssignContext )( 
            IWDFIoTarget * This,
            /* [unique][in] */ 
            __in_opt  IObjectCleanup *pCleanupCallback,
            /* [unique][in] */ 
            __in_opt  void *pContext);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveContext )( 
            IWDFIoTarget * This,
            /* [out] */ 
            __out  void **ppvContext);
        
        void ( STDMETHODCALLTYPE *AcquireLock )( 
            IWDFIoTarget * This);
        
        void ( STDMETHODCALLTYPE *ReleaseLock )( 
            IWDFIoTarget * This);
        
        void ( STDMETHODCALLTYPE *GetTargetFile )( 
            IWDFIoTarget * This,
            /* [out] */ 
            __out  IWDFFile **ppWdfFile);
        
        void ( STDMETHODCALLTYPE *CancelSentRequestsForFile )( 
            IWDFIoTarget * This,
            /* [in] */ 
            __in  IWDFFile *pFile);
        
        HRESULT ( STDMETHODCALLTYPE *FormatRequestForRead )( 
            IWDFIoTarget * This,
            /* [in] */ 
            __in  IWDFIoRequest *pRequest,
            /* [unique][in] */ 
            __in_opt  IWDFFile *pFile,
            /* [unique][in] */ 
            __in_opt  IWDFMemory *pOutputMemory,
            /* [unique][in] */ 
            __in_opt  PWDFMEMORY_OFFSET pOutputMemoryOffset,
            /* [unique][in] */ 
            __in_opt  PLONGLONG DeviceOffset);
        
        HRESULT ( STDMETHODCALLTYPE *FormatRequestForWrite )( 
            IWDFIoTarget * This,
            /* [in] */ 
            __in  IWDFIoRequest *pRequest,
            /* [unique][in] */ 
            __in_opt  IWDFFile *pFile,
            /* [unique][in] */ 
            __in_opt  IWDFMemory *pInputMemory,
            /* [unique][in] */ 
            __in_opt  PWDFMEMORY_OFFSET pInputMemoryOffset,
            /* [unique][in] */ 
            __in_opt  PLONGLONG DeviceOffset);
        
        HRESULT ( STDMETHODCALLTYPE *FormatRequestForIoctl )( 
            IWDFIoTarget * This,
            /* [in] */ 
            __in  IWDFIoRequest *pRequest,
            /* [in] */ 
            __in  ULONG IoctlCode,
            /* [unique][in] */ 
            __in_opt  IWDFFile *pFile,
            /* [unique][in] */ 
            __in_opt  IWDFMemory *pInputMemory,
            /* [unique][in] */ 
            __in_opt  PWDFMEMORY_OFFSET pInputMemoryOffset,
            /* [unique][in] */ 
            __in_opt  IWDFMemory *pOutputMemory,
            /* [unique][in] */ 
            __in_opt  PWDFMEMORY_OFFSET pOutputMemoryOffset);
        
        END_INTERFACE
    } IWDFIoTargetVtbl;

    interface IWDFIoTarget
    {
        CONST_VTBL struct IWDFIoTargetVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWDFIoTarget_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWDFIoTarget_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWDFIoTarget_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWDFIoTarget_DeleteWdfObject(This)	\
    ( (This)->lpVtbl -> DeleteWdfObject(This) ) 

#define IWDFIoTarget_AssignContext(This,pCleanupCallback,pContext)	\
    ( (This)->lpVtbl -> AssignContext(This,pCleanupCallback,pContext) ) 

#define IWDFIoTarget_RetrieveContext(This,ppvContext)	\
    ( (This)->lpVtbl -> RetrieveContext(This,ppvContext) ) 

#define IWDFIoTarget_AcquireLock(This)	\
    ( (This)->lpVtbl -> AcquireLock(This) ) 

#define IWDFIoTarget_ReleaseLock(This)	\
    ( (This)->lpVtbl -> ReleaseLock(This) ) 


#define IWDFIoTarget_GetTargetFile(This,ppWdfFile)	\
    ( (This)->lpVtbl -> GetTargetFile(This,ppWdfFile) ) 

#define IWDFIoTarget_CancelSentRequestsForFile(This,pFile)	\
    ( (This)->lpVtbl -> CancelSentRequestsForFile(This,pFile) ) 

#define IWDFIoTarget_FormatRequestForRead(This,pRequest,pFile,pOutputMemory,pOutputMemoryOffset,DeviceOffset)	\
    ( (This)->lpVtbl -> FormatRequestForRead(This,pRequest,pFile,pOutputMemory,pOutputMemoryOffset,DeviceOffset) ) 

#define IWDFIoTarget_FormatRequestForWrite(This,pRequest,pFile,pInputMemory,pInputMemoryOffset,DeviceOffset)	\
    ( (This)->lpVtbl -> FormatRequestForWrite(This,pRequest,pFile,pInputMemory,pInputMemoryOffset,DeviceOffset) ) 

#define IWDFIoTarget_FormatRequestForIoctl(This,pRequest,IoctlCode,pFile,pInputMemory,pInputMemoryOffset,pOutputMemory,pOutputMemoryOffset)	\
    ( (This)->lpVtbl -> FormatRequestForIoctl(This,pRequest,IoctlCode,pFile,pInputMemory,pInputMemoryOffset,pOutputMemory,pOutputMemoryOffset) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWDFIoTarget_INTERFACE_DEFINED__ */


#ifndef __IWDFIoTargetStateManagement_INTERFACE_DEFINED__
#define __IWDFIoTargetStateManagement_INTERFACE_DEFINED__

/* interface IWDFIoTargetStateManagement */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWDFIoTargetStateManagement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("88b077aa-eff6-4eba-9e4d-064c70822b98")
    IWDFIoTargetStateManagement : public IUnknown
    {
    public:
        virtual WDF_IO_TARGET_STATE STDMETHODCALLTYPE GetState( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Start( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Stop( 
            /* [in] */ 
            __in  WDF_IO_TARGET_SENT_IO_ACTION Action) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ 
            __in  BOOL bIsSurpriseRemove) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWDFIoTargetStateManagementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWDFIoTargetStateManagement * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWDFIoTargetStateManagement * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWDFIoTargetStateManagement * This);
        
        WDF_IO_TARGET_STATE ( STDMETHODCALLTYPE *GetState )( 
            IWDFIoTargetStateManagement * This);
        
        HRESULT ( STDMETHODCALLTYPE *Start )( 
            IWDFIoTargetStateManagement * This);
        
        HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IWDFIoTargetStateManagement * This,
            /* [in] */ 
            __in  WDF_IO_TARGET_SENT_IO_ACTION Action);
        
        HRESULT ( STDMETHODCALLTYPE *Remove )( 
            IWDFIoTargetStateManagement * This,
            /* [in] */ 
            __in  BOOL bIsSurpriseRemove);
        
        END_INTERFACE
    } IWDFIoTargetStateManagementVtbl;

    interface IWDFIoTargetStateManagement
    {
        CONST_VTBL struct IWDFIoTargetStateManagementVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWDFIoTargetStateManagement_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWDFIoTargetStateManagement_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWDFIoTargetStateManagement_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWDFIoTargetStateManagement_GetState(This)	\
    ( (This)->lpVtbl -> GetState(This) ) 

#define IWDFIoTargetStateManagement_Start(This)	\
    ( (This)->lpVtbl -> Start(This) ) 

#define IWDFIoTargetStateManagement_Stop(This,Action)	\
    ( (This)->lpVtbl -> Stop(This,Action) ) 

#define IWDFIoTargetStateManagement_Remove(This,bIsSurpriseRemove)	\
    ( (This)->lpVtbl -> Remove(This,bIsSurpriseRemove) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWDFIoTargetStateManagement_INTERFACE_DEFINED__ */


#ifndef __IWDFFileHandleTargetFactory_INTERFACE_DEFINED__
#define __IWDFFileHandleTargetFactory_INTERFACE_DEFINED__

/* interface IWDFFileHandleTargetFactory */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_IWDFFileHandleTargetFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1427b7e8-e365-400b-9b92-5ace55168e25")
    IWDFFileHandleTargetFactory : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateFileHandleTarget( 
            /* [in] */ 
            __in  HANDLE hTarget,
            /* [out] */ 
            __out  IWDFIoTarget **ppTarget) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWDFFileHandleTargetFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWDFFileHandleTargetFactory * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWDFFileHandleTargetFactory * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWDFFileHandleTargetFactory * This);
        
        HRESULT ( STDMETHODCALLTYPE *CreateFileHandleTarget )( 
            IWDFFileHandleTargetFactory * This,
            /* [in] */ 
            __in  HANDLE hTarget,
            /* [out] */ 
            __out  IWDFIoTarget **ppTarget);
        
        END_INTERFACE
    } IWDFFileHandleTargetFactoryVtbl;

    interface IWDFFileHandleTargetFactory
    {
        CONST_VTBL struct IWDFFileHandleTargetFactoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWDFFileHandleTargetFactory_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWDFFileHandleTargetFactory_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWDFFileHandleTargetFactory_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWDFFileHandleTargetFactory_CreateFileHandleTarget(This,hTarget,ppTarget)	\
    ( (This)->lpVtbl -> CreateFileHandleTarget(This,hTarget,ppTarget) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWDFFileHandleTargetFactory_INTERFACE_DEFINED__ */


#ifndef __IWDFMemory_INTERFACE_DEFINED__
#define __IWDFMemory_INTERFACE_DEFINED__

/* interface IWDFMemory */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWDFMemory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("AB098F88-8F16-472a-B0BC-ECA46486C102")
    IWDFMemory : public IWDFObject
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CopyFromMemory( 
            /* [in] */ 
            __in  IWDFMemory *Source,
            /* [unique][in] */ 
            __in_opt  PWDFMEMORY_OFFSET SourceOffset) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CopyToBuffer( 
            /* [in] */ 
            __in  ULONG_PTR SourceOffset,
            /* [size_is][in] */ 
            __in_bcount(NumOfBytesToCopyTo)  void *TargetBuffer,
            /* [in] */ 
            __in  SIZE_T NumOfBytesToCopyTo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CopyFromBuffer( 
            /* [in] */ 
            __in  ULONG_PTR DestOffset,
            /* [size_is][in] */ 
            __in_bcount(NumOfBytesToCopyFrom)  void *SourceBuffer,
            /* [in] */ 
            __in  SIZE_T NumOfBytesToCopyFrom) = 0;
        
        virtual SIZE_T STDMETHODCALLTYPE GetSize( void) = 0;
        
        virtual void *STDMETHODCALLTYPE GetDataBuffer( 
            /* [unique][out] */ 
            __out_opt  SIZE_T *BufferSize) = 0;
        
        virtual void STDMETHODCALLTYPE SetBuffer( 
            /* [size_is][in] */ 
            __in_bcount(BufferSize)  void *Buffer,
            /* [in] */ 
            __in  SIZE_T BufferSize) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWDFMemoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWDFMemory * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWDFMemory * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWDFMemory * This);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteWdfObject )( 
            IWDFMemory * This);
        
        HRESULT ( STDMETHODCALLTYPE *AssignContext )( 
            IWDFMemory * This,
            /* [unique][in] */ 
            __in_opt  IObjectCleanup *pCleanupCallback,
            /* [unique][in] */ 
            __in_opt  void *pContext);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveContext )( 
            IWDFMemory * This,
            /* [out] */ 
            __out  void **ppvContext);
        
        void ( STDMETHODCALLTYPE *AcquireLock )( 
            IWDFMemory * This);
        
        void ( STDMETHODCALLTYPE *ReleaseLock )( 
            IWDFMemory * This);
        
        HRESULT ( STDMETHODCALLTYPE *CopyFromMemory )( 
            IWDFMemory * This,
            /* [in] */ 
            __in  IWDFMemory *Source,
            /* [unique][in] */ 
            __in_opt  PWDFMEMORY_OFFSET SourceOffset);
        
        HRESULT ( STDMETHODCALLTYPE *CopyToBuffer )( 
            IWDFMemory * This,
            /* [in] */ 
            __in  ULONG_PTR SourceOffset,
            /* [size_is][in] */ 
            __in_bcount(NumOfBytesToCopyTo)  void *TargetBuffer,
            /* [in] */ 
            __in  SIZE_T NumOfBytesToCopyTo);
        
        HRESULT ( STDMETHODCALLTYPE *CopyFromBuffer )( 
            IWDFMemory * This,
            /* [in] */ 
            __in  ULONG_PTR DestOffset,
            /* [size_is][in] */ 
            __in_bcount(NumOfBytesToCopyFrom)  void *SourceBuffer,
            /* [in] */ 
            __in  SIZE_T NumOfBytesToCopyFrom);
        
        SIZE_T ( STDMETHODCALLTYPE *GetSize )( 
            IWDFMemory * This);
        
        void *( STDMETHODCALLTYPE *GetDataBuffer )( 
            IWDFMemory * This,
            /* [unique][out] */ 
            __out_opt  SIZE_T *BufferSize);
        
        void ( STDMETHODCALLTYPE *SetBuffer )( 
            IWDFMemory * This,
            /* [size_is][in] */ 
            __in_bcount(BufferSize)  void *Buffer,
            /* [in] */ 
            __in  SIZE_T BufferSize);
        
        END_INTERFACE
    } IWDFMemoryVtbl;

    interface IWDFMemory
    {
        CONST_VTBL struct IWDFMemoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWDFMemory_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWDFMemory_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWDFMemory_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWDFMemory_DeleteWdfObject(This)	\
    ( (This)->lpVtbl -> DeleteWdfObject(This) ) 

#define IWDFMemory_AssignContext(This,pCleanupCallback,pContext)	\
    ( (This)->lpVtbl -> AssignContext(This,pCleanupCallback,pContext) ) 

#define IWDFMemory_RetrieveContext(This,ppvContext)	\
    ( (This)->lpVtbl -> RetrieveContext(This,ppvContext) ) 

#define IWDFMemory_AcquireLock(This)	\
    ( (This)->lpVtbl -> AcquireLock(This) ) 

#define IWDFMemory_ReleaseLock(This)	\
    ( (This)->lpVtbl -> ReleaseLock(This) ) 


#define IWDFMemory_CopyFromMemory(This,Source,SourceOffset)	\
    ( (This)->lpVtbl -> CopyFromMemory(This,Source,SourceOffset) ) 

#define IWDFMemory_CopyToBuffer(This,SourceOffset,TargetBuffer,NumOfBytesToCopyTo)	\
    ( (This)->lpVtbl -> CopyToBuffer(This,SourceOffset,TargetBuffer,NumOfBytesToCopyTo) ) 

#define IWDFMemory_CopyFromBuffer(This,DestOffset,SourceBuffer,NumOfBytesToCopyFrom)	\
    ( (This)->lpVtbl -> CopyFromBuffer(This,DestOffset,SourceBuffer,NumOfBytesToCopyFrom) ) 

#define IWDFMemory_GetSize(This)	\
    ( (This)->lpVtbl -> GetSize(This) ) 

#define IWDFMemory_GetDataBuffer(This,BufferSize)	\
    ( (This)->lpVtbl -> GetDataBuffer(This,BufferSize) ) 

#define IWDFMemory_SetBuffer(This,Buffer,BufferSize)	\
    ( (This)->lpVtbl -> SetBuffer(This,Buffer,BufferSize) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWDFMemory_INTERFACE_DEFINED__ */


#ifndef __IObjectCleanup_INTERFACE_DEFINED__
#define __IObjectCleanup_INTERFACE_DEFINED__

/* interface IObjectCleanup */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IObjectCleanup;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("244ABE3A-ABBC-43b1-A877-F00077550E6A")
    IObjectCleanup : public IUnknown
    {
    public:
        virtual void STDMETHODCALLTYPE OnCleanup( 
            /* [in] */ 
            __in  IWDFObject *pWdfObject) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IObjectCleanupVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IObjectCleanup * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IObjectCleanup * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IObjectCleanup * This);
        
        void ( STDMETHODCALLTYPE *OnCleanup )( 
            IObjectCleanup * This,
            /* [in] */ 
            __in  IWDFObject *pWdfObject);
        
        END_INTERFACE
    } IObjectCleanupVtbl;

    interface IObjectCleanup
    {
        CONST_VTBL struct IObjectCleanupVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IObjectCleanup_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IObjectCleanup_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IObjectCleanup_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IObjectCleanup_OnCleanup(This,pWdfObject)	\
    ( (This)->lpVtbl -> OnCleanup(This,pWdfObject) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IObjectCleanup_INTERFACE_DEFINED__ */


#ifndef __IDriverEntry_INTERFACE_DEFINED__
#define __IDriverEntry_INTERFACE_DEFINED__

/* interface IDriverEntry */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDriverEntry;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1bec7499-8881-4f2b-b01c-a1a907304afc")
    IDriverEntry : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnInitialize( 
            /* [in] */ 
            __in  IWDFDriver *pWdfDriver) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnDeviceAdd( 
            /* [in] */ 
            __in  IWDFDriver *pWdfDriver,
            /* [in] */ 
            __in  IWDFDeviceInitialize *pWdfDeviceInit) = 0;
        
        virtual void STDMETHODCALLTYPE OnDeinitialize( 
            /* [in] */ 
            __in  IWDFDriver *pWdfDriver) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDriverEntryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDriverEntry * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDriverEntry * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDriverEntry * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnInitialize )( 
            IDriverEntry * This,
            /* [in] */ 
            __in  IWDFDriver *pWdfDriver);
        
        HRESULT ( STDMETHODCALLTYPE *OnDeviceAdd )( 
            IDriverEntry * This,
            /* [in] */ 
            __in  IWDFDriver *pWdfDriver,
            /* [in] */ 
            __in  IWDFDeviceInitialize *pWdfDeviceInit);
        
        void ( STDMETHODCALLTYPE *OnDeinitialize )( 
            IDriverEntry * This,
            /* [in] */ 
            __in  IWDFDriver *pWdfDriver);
        
        END_INTERFACE
    } IDriverEntryVtbl;

    interface IDriverEntry
    {
        CONST_VTBL struct IDriverEntryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDriverEntry_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDriverEntry_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDriverEntry_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDriverEntry_OnInitialize(This,pWdfDriver)	\
    ( (This)->lpVtbl -> OnInitialize(This,pWdfDriver) ) 

#define IDriverEntry_OnDeviceAdd(This,pWdfDriver,pWdfDeviceInit)	\
    ( (This)->lpVtbl -> OnDeviceAdd(This,pWdfDriver,pWdfDeviceInit) ) 

#define IDriverEntry_OnDeinitialize(This,pWdfDriver)	\
    ( (This)->lpVtbl -> OnDeinitialize(This,pWdfDriver) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDriverEntry_INTERFACE_DEFINED__ */


#ifndef __IPnpCallback_INTERFACE_DEFINED__
#define __IPnpCallback_INTERFACE_DEFINED__

/* interface IPnpCallback */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IPnpCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("27C32374-CC45-4840-857E-8E5EF7C0EBFF")
    IPnpCallback : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnD0Entry( 
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice,
            /* [in] */ 
            __in  WDF_POWER_DEVICE_STATE previousState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnD0Exit( 
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice,
            /* [in] */ 
            __in  WDF_POWER_DEVICE_STATE newState) = 0;
        
        virtual void STDMETHODCALLTYPE OnSurpriseRemoval( 
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnQueryRemove( 
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnQueryStop( 
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPnpCallbackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPnpCallback * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPnpCallback * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPnpCallback * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnD0Entry )( 
            IPnpCallback * This,
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice,
            /* [in] */ 
            __in  WDF_POWER_DEVICE_STATE previousState);
        
        HRESULT ( STDMETHODCALLTYPE *OnD0Exit )( 
            IPnpCallback * This,
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice,
            /* [in] */ 
            __in  WDF_POWER_DEVICE_STATE newState);
        
        void ( STDMETHODCALLTYPE *OnSurpriseRemoval )( 
            IPnpCallback * This,
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice);
        
        HRESULT ( STDMETHODCALLTYPE *OnQueryRemove )( 
            IPnpCallback * This,
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice);
        
        HRESULT ( STDMETHODCALLTYPE *OnQueryStop )( 
            IPnpCallback * This,
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice);
        
        END_INTERFACE
    } IPnpCallbackVtbl;

    interface IPnpCallback
    {
        CONST_VTBL struct IPnpCallbackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPnpCallback_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPnpCallback_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPnpCallback_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPnpCallback_OnD0Entry(This,pWdfDevice,previousState)	\
    ( (This)->lpVtbl -> OnD0Entry(This,pWdfDevice,previousState) ) 

#define IPnpCallback_OnD0Exit(This,pWdfDevice,newState)	\
    ( (This)->lpVtbl -> OnD0Exit(This,pWdfDevice,newState) ) 

#define IPnpCallback_OnSurpriseRemoval(This,pWdfDevice)	\
    ( (This)->lpVtbl -> OnSurpriseRemoval(This,pWdfDevice) ) 

#define IPnpCallback_OnQueryRemove(This,pWdfDevice)	\
    ( (This)->lpVtbl -> OnQueryRemove(This,pWdfDevice) ) 

#define IPnpCallback_OnQueryStop(This,pWdfDevice)	\
    ( (This)->lpVtbl -> OnQueryStop(This,pWdfDevice) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPnpCallback_INTERFACE_DEFINED__ */


#ifndef __IPnpCallbackSelfManagedIo_INTERFACE_DEFINED__
#define __IPnpCallbackSelfManagedIo_INTERFACE_DEFINED__

/* interface IPnpCallbackSelfManagedIo */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IPnpCallbackSelfManagedIo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D1C9CA4C-46AA-46f8-BD8C-6CA9DEB13340")
    IPnpCallbackSelfManagedIo : public IUnknown
    {
    public:
        virtual void STDMETHODCALLTYPE OnSelfManagedIoCleanup( 
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice) = 0;
        
        virtual void STDMETHODCALLTYPE OnSelfManagedIoFlush( 
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnSelfManagedIoInit( 
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnSelfManagedIoSuspend( 
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnSelfManagedIoRestart( 
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnSelfManagedIoStop( 
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPnpCallbackSelfManagedIoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPnpCallbackSelfManagedIo * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPnpCallbackSelfManagedIo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPnpCallbackSelfManagedIo * This);
        
        void ( STDMETHODCALLTYPE *OnSelfManagedIoCleanup )( 
            IPnpCallbackSelfManagedIo * This,
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice);
        
        void ( STDMETHODCALLTYPE *OnSelfManagedIoFlush )( 
            IPnpCallbackSelfManagedIo * This,
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice);
        
        HRESULT ( STDMETHODCALLTYPE *OnSelfManagedIoInit )( 
            IPnpCallbackSelfManagedIo * This,
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice);
        
        HRESULT ( STDMETHODCALLTYPE *OnSelfManagedIoSuspend )( 
            IPnpCallbackSelfManagedIo * This,
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice);
        
        HRESULT ( STDMETHODCALLTYPE *OnSelfManagedIoRestart )( 
            IPnpCallbackSelfManagedIo * This,
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice);
        
        HRESULT ( STDMETHODCALLTYPE *OnSelfManagedIoStop )( 
            IPnpCallbackSelfManagedIo * This,
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice);
        
        END_INTERFACE
    } IPnpCallbackSelfManagedIoVtbl;

    interface IPnpCallbackSelfManagedIo
    {
        CONST_VTBL struct IPnpCallbackSelfManagedIoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPnpCallbackSelfManagedIo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPnpCallbackSelfManagedIo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPnpCallbackSelfManagedIo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPnpCallbackSelfManagedIo_OnSelfManagedIoCleanup(This,pWdfDevice)	\
    ( (This)->lpVtbl -> OnSelfManagedIoCleanup(This,pWdfDevice) ) 

#define IPnpCallbackSelfManagedIo_OnSelfManagedIoFlush(This,pWdfDevice)	\
    ( (This)->lpVtbl -> OnSelfManagedIoFlush(This,pWdfDevice) ) 

#define IPnpCallbackSelfManagedIo_OnSelfManagedIoInit(This,pWdfDevice)	\
    ( (This)->lpVtbl -> OnSelfManagedIoInit(This,pWdfDevice) ) 

#define IPnpCallbackSelfManagedIo_OnSelfManagedIoSuspend(This,pWdfDevice)	\
    ( (This)->lpVtbl -> OnSelfManagedIoSuspend(This,pWdfDevice) ) 

#define IPnpCallbackSelfManagedIo_OnSelfManagedIoRestart(This,pWdfDevice)	\
    ( (This)->lpVtbl -> OnSelfManagedIoRestart(This,pWdfDevice) ) 

#define IPnpCallbackSelfManagedIo_OnSelfManagedIoStop(This,pWdfDevice)	\
    ( (This)->lpVtbl -> OnSelfManagedIoStop(This,pWdfDevice) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPnpCallbackSelfManagedIo_INTERFACE_DEFINED__ */


#ifndef __IPnpCallbackHardware_INTERFACE_DEFINED__
#define __IPnpCallbackHardware_INTERFACE_DEFINED__

/* interface IPnpCallbackHardware */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IPnpCallbackHardware;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51433BD3-C7C1-4bd8-B4C1-AB1E034626CC")
    IPnpCallbackHardware : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnPrepareHardware( 
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnReleaseHardware( 
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPnpCallbackHardwareVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPnpCallbackHardware * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPnpCallbackHardware * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPnpCallbackHardware * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnPrepareHardware )( 
            IPnpCallbackHardware * This,
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice);
        
        HRESULT ( STDMETHODCALLTYPE *OnReleaseHardware )( 
            IPnpCallbackHardware * This,
            /* [in] */ 
            __in  IWDFDevice *pWdfDevice);
        
        END_INTERFACE
    } IPnpCallbackHardwareVtbl;

    interface IPnpCallbackHardware
    {
        CONST_VTBL struct IPnpCallbackHardwareVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPnpCallbackHardware_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPnpCallbackHardware_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPnpCallbackHardware_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPnpCallbackHardware_OnPrepareHardware(This,pWdfDevice)	\
    ( (This)->lpVtbl -> OnPrepareHardware(This,pWdfDevice) ) 

#define IPnpCallbackHardware_OnReleaseHardware(This,pWdfDevice)	\
    ( (This)->lpVtbl -> OnReleaseHardware(This,pWdfDevice) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPnpCallbackHardware_INTERFACE_DEFINED__ */


#ifndef __IQueueCallbackCreate_INTERFACE_DEFINED__
#define __IQueueCallbackCreate_INTERFACE_DEFINED__

/* interface IQueueCallbackCreate */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IQueueCallbackCreate;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7D8899F0-8DA9-435f-983C-7E17D4356997")
    IQueueCallbackCreate : public IUnknown
    {
    public:
        virtual void STDMETHODCALLTYPE OnCreateFile( 
            /* [in] */ 
            __in  IWDFIoQueue *pWdfQueue,
            /* [in] */ 
            __in  IWDFIoRequest *pWDFRequest,
            /* [in] */ 
            __in  IWDFFile *pWdfFileObject) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IQueueCallbackCreateVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IQueueCallbackCreate * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IQueueCallbackCreate * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IQueueCallbackCreate * This);
        
        void ( STDMETHODCALLTYPE *OnCreateFile )( 
            IQueueCallbackCreate * This,
            /* [in] */ 
            __in  IWDFIoQueue *pWdfQueue,
            /* [in] */ 
            __in  IWDFIoRequest *pWDFRequest,
            /* [in] */ 
            __in  IWDFFile *pWdfFileObject);
        
        END_INTERFACE
    } IQueueCallbackCreateVtbl;

    interface IQueueCallbackCreate
    {
        CONST_VTBL struct IQueueCallbackCreateVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IQueueCallbackCreate_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IQueueCallbackCreate_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IQueueCallbackCreate_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IQueueCallbackCreate_OnCreateFile(This,pWdfQueue,pWDFRequest,pWdfFileObject)	\
    ( (This)->lpVtbl -> OnCreateFile(This,pWdfQueue,pWDFRequest,pWdfFileObject) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IQueueCallbackCreate_INTERFACE_DEFINED__ */


#ifndef __IQueueCallbackRead_INTERFACE_DEFINED__
#define __IQueueCallbackRead_INTERFACE_DEFINED__

/* interface IQueueCallbackRead */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IQueueCallbackRead;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C3072620-1DA6-48fb-B4FE-ED652B16AEB2")
    IQueueCallbackRead : public IUnknown
    {
    public:
        virtual void STDMETHODCALLTYPE OnRead( 
            /* [in] */ 
            __in  IWDFIoQueue *pWdfQueue,
            /* [in] */ 
            __in  IWDFIoRequest *pWdfRequest,
            /* [in] */ 
            __in  SIZE_T NumOfBytesToRead) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IQueueCallbackReadVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IQueueCallbackRead * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IQueueCallbackRead * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IQueueCallbackRead * This);
        
        void ( STDMETHODCALLTYPE *OnRead )( 
            IQueueCallbackRead * This,
            /* [in] */ 
            __in  IWDFIoQueue *pWdfQueue,
            /* [in] */ 
            __in  IWDFIoRequest *pWdfRequest,
            /* [in] */ 
            __in  SIZE_T NumOfBytesToRead);
        
        END_INTERFACE
    } IQueueCallbackReadVtbl;

    interface IQueueCallbackRead
    {
        CONST_VTBL struct IQueueCallbackReadVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IQueueCallbackRead_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IQueueCallbackRead_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IQueueCallbackRead_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IQueueCallbackRead_OnRead(This,pWdfQueue,pWdfRequest,NumOfBytesToRead)	\
    ( (This)->lpVtbl -> OnRead(This,pWdfQueue,pWdfRequest,NumOfBytesToRead) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IQueueCallbackRead_INTERFACE_DEFINED__ */


#ifndef __IQueueCallbackWrite_INTERFACE_DEFINED__
#define __IQueueCallbackWrite_INTERFACE_DEFINED__

/* interface IQueueCallbackWrite */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IQueueCallbackWrite;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4888CBC5-3FA2-4ea3-89A3-C20ED6E04778")
    IQueueCallbackWrite : public IUnknown
    {
    public:
        virtual void STDMETHODCALLTYPE OnWrite( 
            /* [in] */ 
            __in  IWDFIoQueue *pWdfQueue,
            /* [in] */ 
            __in  IWDFIoRequest *pWdfRequest,
            /* [in] */ 
            __in  SIZE_T NumOfBytesToWrite) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IQueueCallbackWriteVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IQueueCallbackWrite * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IQueueCallbackWrite * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IQueueCallbackWrite * This);
        
        void ( STDMETHODCALLTYPE *OnWrite )( 
            IQueueCallbackWrite * This,
            /* [in] */ 
            __in  IWDFIoQueue *pWdfQueue,
            /* [in] */ 
            __in  IWDFIoRequest *pWdfRequest,
            /* [in] */ 
            __in  SIZE_T NumOfBytesToWrite);
        
        END_INTERFACE
    } IQueueCallbackWriteVtbl;

    interface IQueueCallbackWrite
    {
        CONST_VTBL struct IQueueCallbackWriteVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IQueueCallbackWrite_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IQueueCallbackWrite_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IQueueCallbackWrite_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IQueueCallbackWrite_OnWrite(This,pWdfQueue,pWdfRequest,NumOfBytesToWrite)	\
    ( (This)->lpVtbl -> OnWrite(This,pWdfQueue,pWdfRequest,NumOfBytesToWrite) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IQueueCallbackWrite_INTERFACE_DEFINED__ */


#ifndef __IQueueCallbackDeviceIoControl_INTERFACE_DEFINED__
#define __IQueueCallbackDeviceIoControl_INTERFACE_DEFINED__

/* interface IQueueCallbackDeviceIoControl */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IQueueCallbackDeviceIoControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C5411408-0F1E-4ed6-A412-36DD15EEE707")
    IQueueCallbackDeviceIoControl : public IUnknown
    {
    public:
        virtual void STDMETHODCALLTYPE OnDeviceIoControl( 
            /* [in] */ 
            __in  IWDFIoQueue *pWdfQueue,
            /* [in] */ 
            __in  IWDFIoRequest *pWdfRequest,
            /* [in] */ 
            __in  ULONG ControlCode,
            /* [in] */ 
            __in  SIZE_T InputBufferSizeInBytes,
            /* [in] */ 
            __in  SIZE_T OutputBufferSizeInBytes) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IQueueCallbackDeviceIoControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IQueueCallbackDeviceIoControl * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IQueueCallbackDeviceIoControl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IQueueCallbackDeviceIoControl * This);
        
        void ( STDMETHODCALLTYPE *OnDeviceIoControl )( 
            IQueueCallbackDeviceIoControl * This,
            /* [in] */ 
            __in  IWDFIoQueue *pWdfQueue,
            /* [in] */ 
            __in  IWDFIoRequest *pWdfRequest,
            /* [in] */ 
            __in  ULONG ControlCode,
            /* [in] */ 
            __in  SIZE_T InputBufferSizeInBytes,
            /* [in] */ 
            __in  SIZE_T OutputBufferSizeInBytes);
        
        END_INTERFACE
    } IQueueCallbackDeviceIoControlVtbl;

    interface IQueueCallbackDeviceIoControl
    {
        CONST_VTBL struct IQueueCallbackDeviceIoControlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IQueueCallbackDeviceIoControl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IQueueCallbackDeviceIoControl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IQueueCallbackDeviceIoControl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IQueueCallbackDeviceIoControl_OnDeviceIoControl(This,pWdfQueue,pWdfRequest,ControlCode,InputBufferSizeInBytes,OutputBufferSizeInBytes)	\
    ( (This)->lpVtbl -> OnDeviceIoControl(This,pWdfQueue,pWdfRequest,ControlCode,InputBufferSizeInBytes,OutputBufferSizeInBytes) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IQueueCallbackDeviceIoControl_INTERFACE_DEFINED__ */


#ifndef __IQueueCallbackDefaultIoHandler_INTERFACE_DEFINED__
#define __IQueueCallbackDefaultIoHandler_INTERFACE_DEFINED__

/* interface IQueueCallbackDefaultIoHandler */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IQueueCallbackDefaultIoHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("15FB6D38-FDD8-4005-8E55-EB0B98313499")
    IQueueCallbackDefaultIoHandler : public IUnknown
    {
    public:
        virtual void STDMETHODCALLTYPE OnDefaultIoHandler( 
            /* [in] */ 
            __in  IWDFIoQueue *pWdfQueue,
            /* [in] */ 
            __in  IWDFIoRequest *pWdfRequest) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IQueueCallbackDefaultIoHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IQueueCallbackDefaultIoHandler * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IQueueCallbackDefaultIoHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IQueueCallbackDefaultIoHandler * This);
        
        void ( STDMETHODCALLTYPE *OnDefaultIoHandler )( 
            IQueueCallbackDefaultIoHandler * This,
            /* [in] */ 
            __in  IWDFIoQueue *pWdfQueue,
            /* [in] */ 
            __in  IWDFIoRequest *pWdfRequest);
        
        END_INTERFACE
    } IQueueCallbackDefaultIoHandlerVtbl;

    interface IQueueCallbackDefaultIoHandler
    {
        CONST_VTBL struct IQueueCallbackDefaultIoHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IQueueCallbackDefaultIoHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IQueueCallbackDefaultIoHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IQueueCallbackDefaultIoHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IQueueCallbackDefaultIoHandler_OnDefaultIoHandler(This,pWdfQueue,pWdfRequest)	\
    ( (This)->lpVtbl -> OnDefaultIoHandler(This,pWdfQueue,pWdfRequest) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IQueueCallbackDefaultIoHandler_INTERFACE_DEFINED__ */


#ifndef __IQueueCallbackStateChange_INTERFACE_DEFINED__
#define __IQueueCallbackStateChange_INTERFACE_DEFINED__

/* interface IQueueCallbackStateChange */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IQueueCallbackStateChange;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E7ECE381-7CB1-468a-BC43-ABD5948FFC75")
    IQueueCallbackStateChange : public IUnknown
    {
    public:
        virtual void STDMETHODCALLTYPE OnStateChange( 
            /* [in] */ 
            __in  IWDFIoQueue *pWdfQueue,
            /* [in] */ 
            __in  WDF_IO_QUEUE_STATE QueueState) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IQueueCallbackStateChangeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IQueueCallbackStateChange * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IQueueCallbackStateChange * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IQueueCallbackStateChange * This);
        
        void ( STDMETHODCALLTYPE *OnStateChange )( 
            IQueueCallbackStateChange * This,
            /* [in] */ 
            __in  IWDFIoQueue *pWdfQueue,
            /* [in] */ 
            __in  WDF_IO_QUEUE_STATE QueueState);
        
        END_INTERFACE
    } IQueueCallbackStateChangeVtbl;

    interface IQueueCallbackStateChange
    {
        CONST_VTBL struct IQueueCallbackStateChangeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IQueueCallbackStateChange_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IQueueCallbackStateChange_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IQueueCallbackStateChange_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IQueueCallbackStateChange_OnStateChange(This,pWdfQueue,QueueState)	\
    ( (This)->lpVtbl -> OnStateChange(This,pWdfQueue,QueueState) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IQueueCallbackStateChange_INTERFACE_DEFINED__ */


#ifndef __IQueueCallbackIoStop_INTERFACE_DEFINED__
#define __IQueueCallbackIoStop_INTERFACE_DEFINED__

/* interface IQueueCallbackIoStop */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IQueueCallbackIoStop;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3DE3B961-88FB-435a-9C14-0868FB4FC623")
    IQueueCallbackIoStop : public IUnknown
    {
    public:
        virtual void STDMETHODCALLTYPE OnIoStop( 
            /* [in] */ 
            __in  IWDFIoQueue *pWdfQueue,
            /* [in] */ 
            __in  IWDFIoRequest *pWdfRequest,
            /* [in] */ 
            __in  ULONG ActionFlags) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IQueueCallbackIoStopVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IQueueCallbackIoStop * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IQueueCallbackIoStop * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IQueueCallbackIoStop * This);
        
        void ( STDMETHODCALLTYPE *OnIoStop )( 
            IQueueCallbackIoStop * This,
            /* [in] */ 
            __in  IWDFIoQueue *pWdfQueue,
            /* [in] */ 
            __in  IWDFIoRequest *pWdfRequest,
            /* [in] */ 
            __in  ULONG ActionFlags);
        
        END_INTERFACE
    } IQueueCallbackIoStopVtbl;

    interface IQueueCallbackIoStop
    {
        CONST_VTBL struct IQueueCallbackIoStopVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IQueueCallbackIoStop_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IQueueCallbackIoStop_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IQueueCallbackIoStop_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IQueueCallbackIoStop_OnIoStop(This,pWdfQueue,pWdfRequest,ActionFlags)	\
    ( (This)->lpVtbl -> OnIoStop(This,pWdfQueue,pWdfRequest,ActionFlags) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IQueueCallbackIoStop_INTERFACE_DEFINED__ */


#ifndef __IQueueCallbackIoResume_INTERFACE_DEFINED__
#define __IQueueCallbackIoResume_INTERFACE_DEFINED__

/* interface IQueueCallbackIoResume */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IQueueCallbackIoResume;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BEE9FD3C-3DDA-4d9d-A8FC-02C38D79F663")
    IQueueCallbackIoResume : public IUnknown
    {
    public:
        virtual void STDMETHODCALLTYPE OnIoResume( 
            /* [in] */ 
            __in  IWDFIoQueue *pWdfQueue,
            /* [in] */ 
            __in  IWDFIoRequest *pWdfRequest) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IQueueCallbackIoResumeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IQueueCallbackIoResume * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IQueueCallbackIoResume * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IQueueCallbackIoResume * This);
        
        void ( STDMETHODCALLTYPE *OnIoResume )( 
            IQueueCallbackIoResume * This,
            /* [in] */ 
            __in  IWDFIoQueue *pWdfQueue,
            /* [in] */ 
            __in  IWDFIoRequest *pWdfRequest);
        
        END_INTERFACE
    } IQueueCallbackIoResumeVtbl;

    interface IQueueCallbackIoResume
    {
        CONST_VTBL struct IQueueCallbackIoResumeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IQueueCallbackIoResume_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IQueueCallbackIoResume_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IQueueCallbackIoResume_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IQueueCallbackIoResume_OnIoResume(This,pWdfQueue,pWdfRequest)	\
    ( (This)->lpVtbl -> OnIoResume(This,pWdfQueue,pWdfRequest) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IQueueCallbackIoResume_INTERFACE_DEFINED__ */


#ifndef __IRequestCallbackCancel_INTERFACE_DEFINED__
#define __IRequestCallbackCancel_INTERFACE_DEFINED__

/* interface IRequestCallbackCancel */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IRequestCallbackCancel;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4E9F1A77-4587-4235-81C4-E6D24545A656")
    IRequestCallbackCancel : public IUnknown
    {
    public:
        virtual void STDMETHODCALLTYPE OnCancel( 
            /* [in] */ 
            __in  IWDFIoRequest *pWdfRequest) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRequestCallbackCancelVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRequestCallbackCancel * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRequestCallbackCancel * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRequestCallbackCancel * This);
        
        void ( STDMETHODCALLTYPE *OnCancel )( 
            IRequestCallbackCancel * This,
            /* [in] */ 
            __in  IWDFIoRequest *pWdfRequest);
        
        END_INTERFACE
    } IRequestCallbackCancelVtbl;

    interface IRequestCallbackCancel
    {
        CONST_VTBL struct IRequestCallbackCancelVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRequestCallbackCancel_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRequestCallbackCancel_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRequestCallbackCancel_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRequestCallbackCancel_OnCancel(This,pWdfRequest)	\
    ( (This)->lpVtbl -> OnCancel(This,pWdfRequest) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IRequestCallbackCancel_INTERFACE_DEFINED__ */


#ifndef __IRequestCallbackRequestCompletion_INTERFACE_DEFINED__
#define __IRequestCallbackRequestCompletion_INTERFACE_DEFINED__

/* interface IRequestCallbackRequestCompletion */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IRequestCallbackRequestCompletion;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8A7CC8CB-CEB2-46f6-9851-77BD347A15C2")
    IRequestCallbackRequestCompletion : public IUnknown
    {
    public:
        virtual void STDMETHODCALLTYPE OnCompletion( 
            /* [in] */ 
            __in  IWDFIoRequest *pWdfRequest,
            /* [in] */ 
            __in  IWDFIoTarget *pIoTarget,
            /* [in] */ 
            __in  IWDFRequestCompletionParams *pParams,
            /* [in] */ 
            __in  void *pContext) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRequestCallbackRequestCompletionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRequestCallbackRequestCompletion * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRequestCallbackRequestCompletion * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRequestCallbackRequestCompletion * This);
        
        void ( STDMETHODCALLTYPE *OnCompletion )( 
            IRequestCallbackRequestCompletion * This,
            /* [in] */ 
            __in  IWDFIoRequest *pWdfRequest,
            /* [in] */ 
            __in  IWDFIoTarget *pIoTarget,
            /* [in] */ 
            __in  IWDFRequestCompletionParams *pParams,
            /* [in] */ 
            __in  void *pContext);
        
        END_INTERFACE
    } IRequestCallbackRequestCompletionVtbl;

    interface IRequestCallbackRequestCompletion
    {
        CONST_VTBL struct IRequestCallbackRequestCompletionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRequestCallbackRequestCompletion_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRequestCallbackRequestCompletion_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRequestCallbackRequestCompletion_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRequestCallbackRequestCompletion_OnCompletion(This,pWdfRequest,pIoTarget,pParams,pContext)	\
    ( (This)->lpVtbl -> OnCompletion(This,pWdfRequest,pIoTarget,pParams,pContext) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IRequestCallbackRequestCompletion_INTERFACE_DEFINED__ */


#ifndef __IFileCallbackCleanup_INTERFACE_DEFINED__
#define __IFileCallbackCleanup_INTERFACE_DEFINED__

/* interface IFileCallbackCleanup */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IFileCallbackCleanup;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("35C5F79C-6D81-4819-AA64-D6A8939D94D6")
    IFileCallbackCleanup : public IUnknown
    {
    public:
        virtual void STDMETHODCALLTYPE OnCleanupFile( 
            /* [in] */ 
            __in  IWDFFile *pWdfFileObject) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFileCallbackCleanupVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFileCallbackCleanup * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFileCallbackCleanup * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFileCallbackCleanup * This);
        
        void ( STDMETHODCALLTYPE *OnCleanupFile )( 
            IFileCallbackCleanup * This,
            /* [in] */ 
            __in  IWDFFile *pWdfFileObject);
        
        END_INTERFACE
    } IFileCallbackCleanupVtbl;

    interface IFileCallbackCleanup
    {
        CONST_VTBL struct IFileCallbackCleanupVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFileCallbackCleanup_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IFileCallbackCleanup_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IFileCallbackCleanup_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IFileCallbackCleanup_OnCleanupFile(This,pWdfFileObject)	\
    ( (This)->lpVtbl -> OnCleanupFile(This,pWdfFileObject) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IFileCallbackCleanup_INTERFACE_DEFINED__ */


#ifndef __IFileCallbackClose_INTERFACE_DEFINED__
#define __IFileCallbackClose_INTERFACE_DEFINED__

/* interface IFileCallbackClose */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IFileCallbackClose;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C28FD731-F3F1-4573-97DC-620F72D013A6")
    IFileCallbackClose : public IUnknown
    {
    public:
        virtual void STDMETHODCALLTYPE OnCloseFile( 
            /* [in] */ 
            __in  IWDFFile *pWdfFileObject) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFileCallbackCloseVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFileCallbackClose * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFileCallbackClose * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFileCallbackClose * This);
        
        void ( STDMETHODCALLTYPE *OnCloseFile )( 
            IFileCallbackClose * This,
            /* [in] */ 
            __in  IWDFFile *pWdfFileObject);
        
        END_INTERFACE
    } IFileCallbackCloseVtbl;

    interface IFileCallbackClose
    {
        CONST_VTBL struct IFileCallbackCloseVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFileCallbackClose_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IFileCallbackClose_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IFileCallbackClose_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IFileCallbackClose_OnCloseFile(This,pWdfFileObject)	\
    ( (This)->lpVtbl -> OnCloseFile(This,pWdfFileObject) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IFileCallbackClose_INTERFACE_DEFINED__ */


#ifndef __IImpersonateCallback_INTERFACE_DEFINED__
#define __IImpersonateCallback_INTERFACE_DEFINED__

/* interface IImpersonateCallback */
/* [unique][restricted][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IImpersonateCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("99B01D17-9FBD-4aa6-B16D-82DAB6A4107F")
    IImpersonateCallback : public IUnknown
    {
    public:
        virtual void STDMETHODCALLTYPE OnImpersonate( 
            /* [unique][in] */ 
            __in_opt  void *Context) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IImpersonateCallbackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IImpersonateCallback * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IImpersonateCallback * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IImpersonateCallback * This);
        
        void ( STDMETHODCALLTYPE *OnImpersonate )( 
            IImpersonateCallback * This,
            /* [unique][in] */ 
            __in_opt  void *Context);
        
        END_INTERFACE
    } IImpersonateCallbackVtbl;

    interface IImpersonateCallback
    {
        CONST_VTBL struct IImpersonateCallbackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IImpersonateCallback_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IImpersonateCallback_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IImpersonateCallback_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IImpersonateCallback_OnImpersonate(This,Context)	\
    ( (This)->lpVtbl -> OnImpersonate(This,Context) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IImpersonateCallback_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



