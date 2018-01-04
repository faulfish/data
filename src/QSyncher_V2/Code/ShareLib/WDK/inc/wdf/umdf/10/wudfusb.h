

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0499 */
/* Compiler settings for wudfusb.idl:
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

#ifndef __wudfusb_h__
#define __wudfusb_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWDFUsbTargetFactory_FWD_DEFINED__
#define __IWDFUsbTargetFactory_FWD_DEFINED__
typedef interface IWDFUsbTargetFactory IWDFUsbTargetFactory;
#endif 	/* __IWDFUsbTargetFactory_FWD_DEFINED__ */


#ifndef __IWDFUsbTargetDevice_FWD_DEFINED__
#define __IWDFUsbTargetDevice_FWD_DEFINED__
typedef interface IWDFUsbTargetDevice IWDFUsbTargetDevice;
#endif 	/* __IWDFUsbTargetDevice_FWD_DEFINED__ */


#ifndef __IWDFUsbInterface_FWD_DEFINED__
#define __IWDFUsbInterface_FWD_DEFINED__
typedef interface IWDFUsbInterface IWDFUsbInterface;
#endif 	/* __IWDFUsbInterface_FWD_DEFINED__ */


#ifndef __IWDFUsbTargetPipe_FWD_DEFINED__
#define __IWDFUsbTargetPipe_FWD_DEFINED__
typedef interface IWDFUsbTargetPipe IWDFUsbTargetPipe;
#endif 	/* __IWDFUsbTargetPipe_FWD_DEFINED__ */


#ifndef __IWDFUsbRequestCompletionParams_FWD_DEFINED__
#define __IWDFUsbRequestCompletionParams_FWD_DEFINED__
typedef interface IWDFUsbRequestCompletionParams IWDFUsbRequestCompletionParams;
#endif 	/* __IWDFUsbRequestCompletionParams_FWD_DEFINED__ */


/* header files for imported files */
#include "wudfddi.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_wudfusb_0000_0000 */
/* [local] */ 

#ifdef MIDL_PASS
typedef PVOID WINUSB_INTERFACE_HANDLE;

typedef WINUSB_INTERFACE_HANDLE *PWINUSB_INTERFACE_HANDLE;

typedef PVOID PUSB_INTERFACE_DESCRIPTOR;

typedef PVOID PWINUSB_PIPE_INFORMATION;

typedef PVOID PWINUSB_SETUP_PACKET;

typedef ULONG USBD_PIPE_TYPE;

#else
#include <WinUsb.h>
#endif



typedef 
enum _WDF_USB_REQUEST_TYPE
    {	WdfUsbRequestTypeInvalid	= 0,
	WdfUsbRequestTypeNoFormat	= ( WdfUsbRequestTypeInvalid + 1 ) ,
	WdfUsbRequestTypeDeviceControlTransfer	= ( WdfUsbRequestTypeNoFormat + 1 ) ,
	WdfUsbRequestTypePipeWrite	= ( WdfUsbRequestTypeDeviceControlTransfer + 1 ) ,
	WdfUsbRequestTypePipeRead	= ( WdfUsbRequestTypePipeWrite + 1 ) 
    } 	WDF_USB_REQUEST_TYPE;

typedef enum _WDF_USB_REQUEST_TYPE *PWDF_USB_REQUEST_TYPE;



extern RPC_IF_HANDLE __MIDL_itf_wudfusb_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wudfusb_0000_0000_v0_0_s_ifspec;

#ifndef __IWDFUsbTargetFactory_INTERFACE_DEFINED__
#define __IWDFUsbTargetFactory_INTERFACE_DEFINED__

/* interface IWDFUsbTargetFactory */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_IWDFUsbTargetFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3f7becf9-3a65-4348-a4f3-339d5734a9c6")
    IWDFUsbTargetFactory : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateUsbTargetDevice( 
            /* [out] */ 
            __out  IWDFUsbTargetDevice **ppDevice) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWDFUsbTargetFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWDFUsbTargetFactory * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWDFUsbTargetFactory * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWDFUsbTargetFactory * This);
        
        HRESULT ( STDMETHODCALLTYPE *CreateUsbTargetDevice )( 
            IWDFUsbTargetFactory * This,
            /* [out] */ 
            __out  IWDFUsbTargetDevice **ppDevice);
        
        END_INTERFACE
    } IWDFUsbTargetFactoryVtbl;

    interface IWDFUsbTargetFactory
    {
        CONST_VTBL struct IWDFUsbTargetFactoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWDFUsbTargetFactory_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWDFUsbTargetFactory_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWDFUsbTargetFactory_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWDFUsbTargetFactory_CreateUsbTargetDevice(This,ppDevice)	\
    ( (This)->lpVtbl -> CreateUsbTargetDevice(This,ppDevice) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWDFUsbTargetFactory_INTERFACE_DEFINED__ */


#ifndef __IWDFUsbTargetDevice_INTERFACE_DEFINED__
#define __IWDFUsbTargetDevice_INTERFACE_DEFINED__

/* interface IWDFUsbTargetDevice */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_IWDFUsbTargetDevice;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4cd12e96-900a-44c3-a1b7-05b8954dab76")
    IWDFUsbTargetDevice : public IWDFIoTarget
    {
    public:
        virtual WINUSB_INTERFACE_HANDLE STDMETHODCALLTYPE GetWinUsbHandle( void) = 0;
        
        virtual UCHAR STDMETHODCALLTYPE GetNumInterfaces( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RetrieveUsbInterface( 
            /* [in] */ 
            __in  UCHAR InterfaceIndex,
            /* [out] */ 
            __out  IWDFUsbInterface **ppUsbInterface) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FormatRequestForControlTransfer( 
            /* [in] */ 
            __in  IWDFIoRequest *pRequest,
            /* [in] */ 
            __in  PWINUSB_SETUP_PACKET SetupPacket,
            /* [unique][in] */ 
            __in_opt  IWDFMemory *pMemory,
            /* [unique][in] */ 
            __in_opt  PWDFMEMORY_OFFSET TransferOffset) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RetrieveDeviceInformation( 
            /* [in] */ 
            __in  ULONG InformationType,
            /* [out][in] */ 
            __inout  ULONG *BufferLength,
            /* [out] */ 
            __out  PVOID Buffer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RetrieveDescriptor( 
            /* [in] */ 
            __in  UCHAR DescriptorType,
            /* [in] */ 
            __in  UCHAR Index,
            /* [in] */ 
            __in  USHORT LanguageID,
            /* [out][in] */ 
            __inout  ULONG *BufferLength,
            /* [out] */ 
            __out  PVOID Buffer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RetrievePowerPolicy( 
            /* [in] */ 
            __in  ULONG PolicyType,
            /* [out][in] */ 
            __inout  ULONG *ValueLength,
            /* [out] */ 
            __out  PVOID Value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetPowerPolicy( 
            /* [in] */ 
            __in  ULONG PolicyType,
            /* [in] */ 
            __in  ULONG ValueLength,
            /* [in] */ 
            __in  PVOID Value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWDFUsbTargetDeviceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWDFUsbTargetDevice * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWDFUsbTargetDevice * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWDFUsbTargetDevice * This);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteWdfObject )( 
            IWDFUsbTargetDevice * This);
        
        HRESULT ( STDMETHODCALLTYPE *AssignContext )( 
            IWDFUsbTargetDevice * This,
            /* [unique][in] */ 
            __in_opt  IObjectCleanup *pCleanupCallback,
            /* [unique][in] */ 
            __in_opt  void *pContext);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveContext )( 
            IWDFUsbTargetDevice * This,
            /* [out] */ 
            __out  void **ppvContext);
        
        void ( STDMETHODCALLTYPE *AcquireLock )( 
            IWDFUsbTargetDevice * This);
        
        void ( STDMETHODCALLTYPE *ReleaseLock )( 
            IWDFUsbTargetDevice * This);
        
        void ( STDMETHODCALLTYPE *GetTargetFile )( 
            IWDFUsbTargetDevice * This,
            /* [out] */ 
            __out  IWDFFile **ppWdfFile);
        
        void ( STDMETHODCALLTYPE *CancelSentRequestsForFile )( 
            IWDFUsbTargetDevice * This,
            /* [in] */ 
            __in  IWDFFile *pFile);
        
        HRESULT ( STDMETHODCALLTYPE *FormatRequestForRead )( 
            IWDFUsbTargetDevice * This,
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
            IWDFUsbTargetDevice * This,
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
            IWDFUsbTargetDevice * This,
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
        
        WINUSB_INTERFACE_HANDLE ( STDMETHODCALLTYPE *GetWinUsbHandle )( 
            IWDFUsbTargetDevice * This);
        
        UCHAR ( STDMETHODCALLTYPE *GetNumInterfaces )( 
            IWDFUsbTargetDevice * This);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveUsbInterface )( 
            IWDFUsbTargetDevice * This,
            /* [in] */ 
            __in  UCHAR InterfaceIndex,
            /* [out] */ 
            __out  IWDFUsbInterface **ppUsbInterface);
        
        HRESULT ( STDMETHODCALLTYPE *FormatRequestForControlTransfer )( 
            IWDFUsbTargetDevice * This,
            /* [in] */ 
            __in  IWDFIoRequest *pRequest,
            /* [in] */ 
            __in  PWINUSB_SETUP_PACKET SetupPacket,
            /* [unique][in] */ 
            __in_opt  IWDFMemory *pMemory,
            /* [unique][in] */ 
            __in_opt  PWDFMEMORY_OFFSET TransferOffset);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveDeviceInformation )( 
            IWDFUsbTargetDevice * This,
            /* [in] */ 
            __in  ULONG InformationType,
            /* [out][in] */ 
            __inout  ULONG *BufferLength,
            /* [out] */ 
            __out  PVOID Buffer);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveDescriptor )( 
            IWDFUsbTargetDevice * This,
            /* [in] */ 
            __in  UCHAR DescriptorType,
            /* [in] */ 
            __in  UCHAR Index,
            /* [in] */ 
            __in  USHORT LanguageID,
            /* [out][in] */ 
            __inout  ULONG *BufferLength,
            /* [out] */ 
            __out  PVOID Buffer);
        
        HRESULT ( STDMETHODCALLTYPE *RetrievePowerPolicy )( 
            IWDFUsbTargetDevice * This,
            /* [in] */ 
            __in  ULONG PolicyType,
            /* [out][in] */ 
            __inout  ULONG *ValueLength,
            /* [out] */ 
            __out  PVOID Value);
        
        HRESULT ( STDMETHODCALLTYPE *SetPowerPolicy )( 
            IWDFUsbTargetDevice * This,
            /* [in] */ 
            __in  ULONG PolicyType,
            /* [in] */ 
            __in  ULONG ValueLength,
            /* [in] */ 
            __in  PVOID Value);
        
        END_INTERFACE
    } IWDFUsbTargetDeviceVtbl;

    interface IWDFUsbTargetDevice
    {
        CONST_VTBL struct IWDFUsbTargetDeviceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWDFUsbTargetDevice_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWDFUsbTargetDevice_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWDFUsbTargetDevice_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWDFUsbTargetDevice_DeleteWdfObject(This)	\
    ( (This)->lpVtbl -> DeleteWdfObject(This) ) 

#define IWDFUsbTargetDevice_AssignContext(This,pCleanupCallback,pContext)	\
    ( (This)->lpVtbl -> AssignContext(This,pCleanupCallback,pContext) ) 

#define IWDFUsbTargetDevice_RetrieveContext(This,ppvContext)	\
    ( (This)->lpVtbl -> RetrieveContext(This,ppvContext) ) 

#define IWDFUsbTargetDevice_AcquireLock(This)	\
    ( (This)->lpVtbl -> AcquireLock(This) ) 

#define IWDFUsbTargetDevice_ReleaseLock(This)	\
    ( (This)->lpVtbl -> ReleaseLock(This) ) 


#define IWDFUsbTargetDevice_GetTargetFile(This,ppWdfFile)	\
    ( (This)->lpVtbl -> GetTargetFile(This,ppWdfFile) ) 

#define IWDFUsbTargetDevice_CancelSentRequestsForFile(This,pFile)	\
    ( (This)->lpVtbl -> CancelSentRequestsForFile(This,pFile) ) 

#define IWDFUsbTargetDevice_FormatRequestForRead(This,pRequest,pFile,pOutputMemory,pOutputMemoryOffset,DeviceOffset)	\
    ( (This)->lpVtbl -> FormatRequestForRead(This,pRequest,pFile,pOutputMemory,pOutputMemoryOffset,DeviceOffset) ) 

#define IWDFUsbTargetDevice_FormatRequestForWrite(This,pRequest,pFile,pInputMemory,pInputMemoryOffset,DeviceOffset)	\
    ( (This)->lpVtbl -> FormatRequestForWrite(This,pRequest,pFile,pInputMemory,pInputMemoryOffset,DeviceOffset) ) 

#define IWDFUsbTargetDevice_FormatRequestForIoctl(This,pRequest,IoctlCode,pFile,pInputMemory,pInputMemoryOffset,pOutputMemory,pOutputMemoryOffset)	\
    ( (This)->lpVtbl -> FormatRequestForIoctl(This,pRequest,IoctlCode,pFile,pInputMemory,pInputMemoryOffset,pOutputMemory,pOutputMemoryOffset) ) 


#define IWDFUsbTargetDevice_GetWinUsbHandle(This)	\
    ( (This)->lpVtbl -> GetWinUsbHandle(This) ) 

#define IWDFUsbTargetDevice_GetNumInterfaces(This)	\
    ( (This)->lpVtbl -> GetNumInterfaces(This) ) 

#define IWDFUsbTargetDevice_RetrieveUsbInterface(This,InterfaceIndex,ppUsbInterface)	\
    ( (This)->lpVtbl -> RetrieveUsbInterface(This,InterfaceIndex,ppUsbInterface) ) 

#define IWDFUsbTargetDevice_FormatRequestForControlTransfer(This,pRequest,SetupPacket,pMemory,TransferOffset)	\
    ( (This)->lpVtbl -> FormatRequestForControlTransfer(This,pRequest,SetupPacket,pMemory,TransferOffset) ) 

#define IWDFUsbTargetDevice_RetrieveDeviceInformation(This,InformationType,BufferLength,Buffer)	\
    ( (This)->lpVtbl -> RetrieveDeviceInformation(This,InformationType,BufferLength,Buffer) ) 

#define IWDFUsbTargetDevice_RetrieveDescriptor(This,DescriptorType,Index,LanguageID,BufferLength,Buffer)	\
    ( (This)->lpVtbl -> RetrieveDescriptor(This,DescriptorType,Index,LanguageID,BufferLength,Buffer) ) 

#define IWDFUsbTargetDevice_RetrievePowerPolicy(This,PolicyType,ValueLength,Value)	\
    ( (This)->lpVtbl -> RetrievePowerPolicy(This,PolicyType,ValueLength,Value) ) 

#define IWDFUsbTargetDevice_SetPowerPolicy(This,PolicyType,ValueLength,Value)	\
    ( (This)->lpVtbl -> SetPowerPolicy(This,PolicyType,ValueLength,Value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWDFUsbTargetDevice_INTERFACE_DEFINED__ */


#ifndef __IWDFUsbInterface_INTERFACE_DEFINED__
#define __IWDFUsbInterface_INTERFACE_DEFINED__

/* interface IWDFUsbInterface */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_IWDFUsbInterface;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("a6714a4d-c764-498f-b6d2-032f10ca4e36")
    IWDFUsbInterface : public IWDFObject
    {
    public:
        virtual void STDMETHODCALLTYPE GetInterfaceDescriptor( 
            /* [out] */ 
            __out  PUSB_INTERFACE_DESCRIPTOR UsbAltInterfaceDescriptor) = 0;
        
        virtual UCHAR STDMETHODCALLTYPE GetInterfaceNumber( void) = 0;
        
        virtual UCHAR STDMETHODCALLTYPE GetNumEndPoints( void) = 0;
        
        virtual UCHAR STDMETHODCALLTYPE GetConfiguredSettingIndex( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelectSetting( 
            /* [in] */ 
            __in  UCHAR SettingNumber) = 0;
        
        virtual WINUSB_INTERFACE_HANDLE STDMETHODCALLTYPE GetWinUsbHandle( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RetrieveUsbPipeObject( 
            /* [in] */ 
            __in  UCHAR PipeIndex,
            /* [out] */ 
            __out  IWDFUsbTargetPipe **ppPipe) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWDFUsbInterfaceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWDFUsbInterface * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWDFUsbInterface * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWDFUsbInterface * This);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteWdfObject )( 
            IWDFUsbInterface * This);
        
        HRESULT ( STDMETHODCALLTYPE *AssignContext )( 
            IWDFUsbInterface * This,
            /* [unique][in] */ 
            __in_opt  IObjectCleanup *pCleanupCallback,
            /* [unique][in] */ 
            __in_opt  void *pContext);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveContext )( 
            IWDFUsbInterface * This,
            /* [out] */ 
            __out  void **ppvContext);
        
        void ( STDMETHODCALLTYPE *AcquireLock )( 
            IWDFUsbInterface * This);
        
        void ( STDMETHODCALLTYPE *ReleaseLock )( 
            IWDFUsbInterface * This);
        
        void ( STDMETHODCALLTYPE *GetInterfaceDescriptor )( 
            IWDFUsbInterface * This,
            /* [out] */ 
            __out  PUSB_INTERFACE_DESCRIPTOR UsbAltInterfaceDescriptor);
        
        UCHAR ( STDMETHODCALLTYPE *GetInterfaceNumber )( 
            IWDFUsbInterface * This);
        
        UCHAR ( STDMETHODCALLTYPE *GetNumEndPoints )( 
            IWDFUsbInterface * This);
        
        UCHAR ( STDMETHODCALLTYPE *GetConfiguredSettingIndex )( 
            IWDFUsbInterface * This);
        
        HRESULT ( STDMETHODCALLTYPE *SelectSetting )( 
            IWDFUsbInterface * This,
            /* [in] */ 
            __in  UCHAR SettingNumber);
        
        WINUSB_INTERFACE_HANDLE ( STDMETHODCALLTYPE *GetWinUsbHandle )( 
            IWDFUsbInterface * This);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveUsbPipeObject )( 
            IWDFUsbInterface * This,
            /* [in] */ 
            __in  UCHAR PipeIndex,
            /* [out] */ 
            __out  IWDFUsbTargetPipe **ppPipe);
        
        END_INTERFACE
    } IWDFUsbInterfaceVtbl;

    interface IWDFUsbInterface
    {
        CONST_VTBL struct IWDFUsbInterfaceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWDFUsbInterface_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWDFUsbInterface_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWDFUsbInterface_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWDFUsbInterface_DeleteWdfObject(This)	\
    ( (This)->lpVtbl -> DeleteWdfObject(This) ) 

#define IWDFUsbInterface_AssignContext(This,pCleanupCallback,pContext)	\
    ( (This)->lpVtbl -> AssignContext(This,pCleanupCallback,pContext) ) 

#define IWDFUsbInterface_RetrieveContext(This,ppvContext)	\
    ( (This)->lpVtbl -> RetrieveContext(This,ppvContext) ) 

#define IWDFUsbInterface_AcquireLock(This)	\
    ( (This)->lpVtbl -> AcquireLock(This) ) 

#define IWDFUsbInterface_ReleaseLock(This)	\
    ( (This)->lpVtbl -> ReleaseLock(This) ) 


#define IWDFUsbInterface_GetInterfaceDescriptor(This,UsbAltInterfaceDescriptor)	\
    ( (This)->lpVtbl -> GetInterfaceDescriptor(This,UsbAltInterfaceDescriptor) ) 

#define IWDFUsbInterface_GetInterfaceNumber(This)	\
    ( (This)->lpVtbl -> GetInterfaceNumber(This) ) 

#define IWDFUsbInterface_GetNumEndPoints(This)	\
    ( (This)->lpVtbl -> GetNumEndPoints(This) ) 

#define IWDFUsbInterface_GetConfiguredSettingIndex(This)	\
    ( (This)->lpVtbl -> GetConfiguredSettingIndex(This) ) 

#define IWDFUsbInterface_SelectSetting(This,SettingNumber)	\
    ( (This)->lpVtbl -> SelectSetting(This,SettingNumber) ) 

#define IWDFUsbInterface_GetWinUsbHandle(This)	\
    ( (This)->lpVtbl -> GetWinUsbHandle(This) ) 

#define IWDFUsbInterface_RetrieveUsbPipeObject(This,PipeIndex,ppPipe)	\
    ( (This)->lpVtbl -> RetrieveUsbPipeObject(This,PipeIndex,ppPipe) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWDFUsbInterface_INTERFACE_DEFINED__ */


#ifndef __IWDFUsbTargetPipe_INTERFACE_DEFINED__
#define __IWDFUsbTargetPipe_INTERFACE_DEFINED__

/* interface IWDFUsbTargetPipe */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_IWDFUsbTargetPipe;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("f373216e-8c42-4c5c-95b4-d2a40b99c0fa")
    IWDFUsbTargetPipe : public IWDFIoTarget
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Abort( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Flush( void) = 0;
        
        virtual void STDMETHODCALLTYPE GetInformation( 
            /* [out] */ 
            __out  PWINUSB_PIPE_INFORMATION pInfo) = 0;
        
        virtual BOOL STDMETHODCALLTYPE IsInEndPoint( void) = 0;
        
        virtual BOOL STDMETHODCALLTYPE IsOutEndPoint( void) = 0;
        
        virtual USBD_PIPE_TYPE STDMETHODCALLTYPE GetType( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RetrievePipePolicy( 
            /* [in] */ 
            __in  ULONG PolicyType,
            /* [out][in] */ 
            __inout  ULONG *ValueLength,
            /* [out] */ 
            __out  PVOID Value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetPipePolicy( 
            /* [in] */ 
            __in  ULONG PolicyType,
            /* [in] */ 
            __in  ULONG ValueLength,
            /* [in] */ 
            __in  PVOID Value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWDFUsbTargetPipeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWDFUsbTargetPipe * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWDFUsbTargetPipe * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWDFUsbTargetPipe * This);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteWdfObject )( 
            IWDFUsbTargetPipe * This);
        
        HRESULT ( STDMETHODCALLTYPE *AssignContext )( 
            IWDFUsbTargetPipe * This,
            /* [unique][in] */ 
            __in_opt  IObjectCleanup *pCleanupCallback,
            /* [unique][in] */ 
            __in_opt  void *pContext);
        
        HRESULT ( STDMETHODCALLTYPE *RetrieveContext )( 
            IWDFUsbTargetPipe * This,
            /* [out] */ 
            __out  void **ppvContext);
        
        void ( STDMETHODCALLTYPE *AcquireLock )( 
            IWDFUsbTargetPipe * This);
        
        void ( STDMETHODCALLTYPE *ReleaseLock )( 
            IWDFUsbTargetPipe * This);
        
        void ( STDMETHODCALLTYPE *GetTargetFile )( 
            IWDFUsbTargetPipe * This,
            /* [out] */ 
            __out  IWDFFile **ppWdfFile);
        
        void ( STDMETHODCALLTYPE *CancelSentRequestsForFile )( 
            IWDFUsbTargetPipe * This,
            /* [in] */ 
            __in  IWDFFile *pFile);
        
        HRESULT ( STDMETHODCALLTYPE *FormatRequestForRead )( 
            IWDFUsbTargetPipe * This,
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
            IWDFUsbTargetPipe * This,
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
            IWDFUsbTargetPipe * This,
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
        
        HRESULT ( STDMETHODCALLTYPE *Abort )( 
            IWDFUsbTargetPipe * This);
        
        HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IWDFUsbTargetPipe * This);
        
        HRESULT ( STDMETHODCALLTYPE *Flush )( 
            IWDFUsbTargetPipe * This);
        
        void ( STDMETHODCALLTYPE *GetInformation )( 
            IWDFUsbTargetPipe * This,
            /* [out] */ 
            __out  PWINUSB_PIPE_INFORMATION pInfo);
        
        BOOL ( STDMETHODCALLTYPE *IsInEndPoint )( 
            IWDFUsbTargetPipe * This);
        
        BOOL ( STDMETHODCALLTYPE *IsOutEndPoint )( 
            IWDFUsbTargetPipe * This);
        
        USBD_PIPE_TYPE ( STDMETHODCALLTYPE *GetType )( 
            IWDFUsbTargetPipe * This);
        
        HRESULT ( STDMETHODCALLTYPE *RetrievePipePolicy )( 
            IWDFUsbTargetPipe * This,
            /* [in] */ 
            __in  ULONG PolicyType,
            /* [out][in] */ 
            __inout  ULONG *ValueLength,
            /* [out] */ 
            __out  PVOID Value);
        
        HRESULT ( STDMETHODCALLTYPE *SetPipePolicy )( 
            IWDFUsbTargetPipe * This,
            /* [in] */ 
            __in  ULONG PolicyType,
            /* [in] */ 
            __in  ULONG ValueLength,
            /* [in] */ 
            __in  PVOID Value);
        
        END_INTERFACE
    } IWDFUsbTargetPipeVtbl;

    interface IWDFUsbTargetPipe
    {
        CONST_VTBL struct IWDFUsbTargetPipeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWDFUsbTargetPipe_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWDFUsbTargetPipe_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWDFUsbTargetPipe_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWDFUsbTargetPipe_DeleteWdfObject(This)	\
    ( (This)->lpVtbl -> DeleteWdfObject(This) ) 

#define IWDFUsbTargetPipe_AssignContext(This,pCleanupCallback,pContext)	\
    ( (This)->lpVtbl -> AssignContext(This,pCleanupCallback,pContext) ) 

#define IWDFUsbTargetPipe_RetrieveContext(This,ppvContext)	\
    ( (This)->lpVtbl -> RetrieveContext(This,ppvContext) ) 

#define IWDFUsbTargetPipe_AcquireLock(This)	\
    ( (This)->lpVtbl -> AcquireLock(This) ) 

#define IWDFUsbTargetPipe_ReleaseLock(This)	\
    ( (This)->lpVtbl -> ReleaseLock(This) ) 


#define IWDFUsbTargetPipe_GetTargetFile(This,ppWdfFile)	\
    ( (This)->lpVtbl -> GetTargetFile(This,ppWdfFile) ) 

#define IWDFUsbTargetPipe_CancelSentRequestsForFile(This,pFile)	\
    ( (This)->lpVtbl -> CancelSentRequestsForFile(This,pFile) ) 

#define IWDFUsbTargetPipe_FormatRequestForRead(This,pRequest,pFile,pOutputMemory,pOutputMemoryOffset,DeviceOffset)	\
    ( (This)->lpVtbl -> FormatRequestForRead(This,pRequest,pFile,pOutputMemory,pOutputMemoryOffset,DeviceOffset) ) 

#define IWDFUsbTargetPipe_FormatRequestForWrite(This,pRequest,pFile,pInputMemory,pInputMemoryOffset,DeviceOffset)	\
    ( (This)->lpVtbl -> FormatRequestForWrite(This,pRequest,pFile,pInputMemory,pInputMemoryOffset,DeviceOffset) ) 

#define IWDFUsbTargetPipe_FormatRequestForIoctl(This,pRequest,IoctlCode,pFile,pInputMemory,pInputMemoryOffset,pOutputMemory,pOutputMemoryOffset)	\
    ( (This)->lpVtbl -> FormatRequestForIoctl(This,pRequest,IoctlCode,pFile,pInputMemory,pInputMemoryOffset,pOutputMemory,pOutputMemoryOffset) ) 


#define IWDFUsbTargetPipe_Abort(This)	\
    ( (This)->lpVtbl -> Abort(This) ) 

#define IWDFUsbTargetPipe_Reset(This)	\
    ( (This)->lpVtbl -> Reset(This) ) 

#define IWDFUsbTargetPipe_Flush(This)	\
    ( (This)->lpVtbl -> Flush(This) ) 

#define IWDFUsbTargetPipe_GetInformation(This,pInfo)	\
    ( (This)->lpVtbl -> GetInformation(This,pInfo) ) 

#define IWDFUsbTargetPipe_IsInEndPoint(This)	\
    ( (This)->lpVtbl -> IsInEndPoint(This) ) 

#define IWDFUsbTargetPipe_IsOutEndPoint(This)	\
    ( (This)->lpVtbl -> IsOutEndPoint(This) ) 

#define IWDFUsbTargetPipe_GetType(This)	\
    ( (This)->lpVtbl -> GetType(This) ) 

#define IWDFUsbTargetPipe_RetrievePipePolicy(This,PolicyType,ValueLength,Value)	\
    ( (This)->lpVtbl -> RetrievePipePolicy(This,PolicyType,ValueLength,Value) ) 

#define IWDFUsbTargetPipe_SetPipePolicy(This,PolicyType,ValueLength,Value)	\
    ( (This)->lpVtbl -> SetPipePolicy(This,PolicyType,ValueLength,Value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWDFUsbTargetPipe_INTERFACE_DEFINED__ */


#ifndef __IWDFUsbRequestCompletionParams_INTERFACE_DEFINED__
#define __IWDFUsbRequestCompletionParams_INTERFACE_DEFINED__

/* interface IWDFUsbRequestCompletionParams */
/* [unique][local][object][uuid] */ 


EXTERN_C const IID IID_IWDFUsbRequestCompletionParams;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5cb1d859-f7ee-411e-9fa5-0159cc118324")
    IWDFUsbRequestCompletionParams : public IWDFRequestCompletionParams
    {
    public:
        virtual WDF_USB_REQUEST_TYPE STDMETHODCALLTYPE GetCompletedUsbRequestType( void) = 0;
        
        virtual void STDMETHODCALLTYPE GetDeviceControlTransferParameters( 
            /* [unique][out] */ 
            __out_opt  IWDFMemory **ppMemory,
            /* [unique][out] */ 
            __out_opt  ULONG *pLengthTransferred,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pOffset,
            /* [unique][out] */ 
            __out_opt  PWINUSB_SETUP_PACKET pSetupPacket) = 0;
        
        virtual void STDMETHODCALLTYPE GetPipeWriteParameters( 
            /* [unique][out] */ 
            __out_opt  IWDFMemory **ppWriteMemory,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pBytesWritten,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pWriteMemoryOffset) = 0;
        
        virtual void STDMETHODCALLTYPE GetPipeReadParameters( 
            /* [unique][out] */ 
            __out_opt  IWDFMemory **ppReadMemory,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pBytesRead,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pReadMemoryOffset) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWDFUsbRequestCompletionParamsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWDFUsbRequestCompletionParams * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWDFUsbRequestCompletionParams * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWDFUsbRequestCompletionParams * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCompletionStatus )( 
            IWDFUsbRequestCompletionParams * This);
        
        ULONG_PTR ( STDMETHODCALLTYPE *GetInformation )( 
            IWDFUsbRequestCompletionParams * This);
        
        WDF_REQUEST_TYPE ( STDMETHODCALLTYPE *GetCompletedRequestType )( 
            IWDFUsbRequestCompletionParams * This);
        
        WDF_USB_REQUEST_TYPE ( STDMETHODCALLTYPE *GetCompletedUsbRequestType )( 
            IWDFUsbRequestCompletionParams * This);
        
        void ( STDMETHODCALLTYPE *GetDeviceControlTransferParameters )( 
            IWDFUsbRequestCompletionParams * This,
            /* [unique][out] */ 
            __out_opt  IWDFMemory **ppMemory,
            /* [unique][out] */ 
            __out_opt  ULONG *pLengthTransferred,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pOffset,
            /* [unique][out] */ 
            __out_opt  PWINUSB_SETUP_PACKET pSetupPacket);
        
        void ( STDMETHODCALLTYPE *GetPipeWriteParameters )( 
            IWDFUsbRequestCompletionParams * This,
            /* [unique][out] */ 
            __out_opt  IWDFMemory **ppWriteMemory,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pBytesWritten,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pWriteMemoryOffset);
        
        void ( STDMETHODCALLTYPE *GetPipeReadParameters )( 
            IWDFUsbRequestCompletionParams * This,
            /* [unique][out] */ 
            __out_opt  IWDFMemory **ppReadMemory,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pBytesRead,
            /* [unique][out] */ 
            __out_opt  SIZE_T *pReadMemoryOffset);
        
        END_INTERFACE
    } IWDFUsbRequestCompletionParamsVtbl;

    interface IWDFUsbRequestCompletionParams
    {
        CONST_VTBL struct IWDFUsbRequestCompletionParamsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWDFUsbRequestCompletionParams_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWDFUsbRequestCompletionParams_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWDFUsbRequestCompletionParams_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWDFUsbRequestCompletionParams_GetCompletionStatus(This)	\
    ( (This)->lpVtbl -> GetCompletionStatus(This) ) 

#define IWDFUsbRequestCompletionParams_GetInformation(This)	\
    ( (This)->lpVtbl -> GetInformation(This) ) 

#define IWDFUsbRequestCompletionParams_GetCompletedRequestType(This)	\
    ( (This)->lpVtbl -> GetCompletedRequestType(This) ) 


#define IWDFUsbRequestCompletionParams_GetCompletedUsbRequestType(This)	\
    ( (This)->lpVtbl -> GetCompletedUsbRequestType(This) ) 

#define IWDFUsbRequestCompletionParams_GetDeviceControlTransferParameters(This,ppMemory,pLengthTransferred,pOffset,pSetupPacket)	\
    ( (This)->lpVtbl -> GetDeviceControlTransferParameters(This,ppMemory,pLengthTransferred,pOffset,pSetupPacket) ) 

#define IWDFUsbRequestCompletionParams_GetPipeWriteParameters(This,ppWriteMemory,pBytesWritten,pWriteMemoryOffset)	\
    ( (This)->lpVtbl -> GetPipeWriteParameters(This,ppWriteMemory,pBytesWritten,pWriteMemoryOffset) ) 

#define IWDFUsbRequestCompletionParams_GetPipeReadParameters(This,ppReadMemory,pBytesRead,pReadMemoryOffset)	\
    ( (This)->lpVtbl -> GetPipeReadParameters(This,ppReadMemory,pBytesRead,pReadMemoryOffset) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWDFUsbRequestCompletionParams_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



