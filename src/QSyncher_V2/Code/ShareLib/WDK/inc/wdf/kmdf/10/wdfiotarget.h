/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    wdfiotarget.h

Abstract:

    Interfaces for WDF IO Targets

Environment:

    kernel mode only

Revision History:

--*/

#ifndef _WDFIOTARGET_H_
#define _WDFIOTARGET_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)

typedef enum _WDF_IO_TARGET_STATE {
    WdfIoTargetStateUndefined = 0,
    WdfIoTargetStarted,
    WdfIoTargetStopped,
    WdfIoTargetClosedForQueryRemove,
    WdfIoTargetClosed,
    WdfIoTargetDeleted,
} WDF_IO_TARGET_STATE, *PWDF_IO_TARGET_STATE;

typedef enum _WDF_IO_TARGET_OPEN_TYPE {
    WdfIoTargetOpenUndefined = 0,
    WdfIoTargetOpenUseExistingDevice = 1,
    WdfIoTargetOpenByName = 2,
    WdfIoTargetOpenReopen = 3,
} WDF_IO_TARGET_OPEN_TYPE;

typedef enum _WDF_IO_TARGET_SENT_IO_ACTION {
    WdfIoTargetSentIoUndefined = 0,
    WdfIoTargetCancelSentIo,
    WdfIoTargetWaitForSentIoToComplete,
    WdfIoTargetLeaveSentIoPending,
} WDF_IO_TARGET_SENT_IO_ACTION;



typedef
NTSTATUS
(*PFN_WDF_IO_TARGET_QUERY_REMOVE)(
    IN WDFIOTARGET IoTarget
    );

typedef
VOID
(*PFN_WDF_IO_TARGET_REMOVE_CANCELED)(
    IN WDFIOTARGET IoTarget
    );

typedef
VOID
(*PFN_WDF_IO_TARGET_REMOVE_COMPLETE)(
    IN WDFIOTARGET IoTarget
    );

typedef struct _WDF_IO_TARGET_OPEN_PARAMS {
    //
    // Size of this structure in bytes
    //
    ULONG Size;

    //
    // Indicates which fields of this structure are going to be used in
    // creating the WDFIOTARGET.
    //
    WDF_IO_TARGET_OPEN_TYPE Type;

    //
    // Notification when the target is being queried for removal.
    // If !NT_SUCCESS is returned, the query will fail and the target will
    // remain opened.
    //
    PFN_WDF_IO_TARGET_QUERY_REMOVE EvtIoTargetQueryRemove;

    //
    // The previous query remove has been canceled and the target can now be
    // reopened.
    //
    PFN_WDF_IO_TARGET_REMOVE_CANCELED EvtIoTargetRemoveCanceled;

    //
    // The query remove has succeeded and the target is now removed from the
    // system.
    //
    PFN_WDF_IO_TARGET_REMOVE_COMPLETE EvtIoTargetRemoveComplete;


    // ========== WdfIoTargetOpenUseExistingDevice begin ==========
    //
    // The device object to send requests to
    //
    PDEVICE_OBJECT TargetDeviceObject;

    //
    // File object representing the TargetDeviceObject.  The PFILE_OBJECT will
    // be passed as a parameter in all requests sent to the resulting
    // WDFIOTARGET.
    //
    PFILE_OBJECT TargetFileObject;
    // ========== WdfIoTargetOpenUseExistingDevice end ==========

    // ========== WdfIoTargetOpenByName begin ==========
    //
    // Name of the device to open.
    //
    UNICODE_STRING TargetDeviceName;

    //
    // The access desired on the device being opened up, ie WDM FILE_XXX_ACCESS
    // such as FILE_ANY_ACCESS, FILE_SPECIAL_ACCESS, FILE_READ_ACCESS, or
    // FILE_WRITE_ACCESS or you can use values such as GENERIC_READ,
    // GENERIC_WRITE, or GENERIC_ALL.
    //
    ACCESS_MASK DesiredAccess;

    //
    // Share access desired on the target being opened, ie WDM FILE_SHARE_XXX
    // values such as FILE_SHARE_READ, FILE_SHARE_WRITE, FILE_SHARE_DELETE.
    //
    // A zero value means exclusive access to the target.
    //
    ULONG ShareAccess;

    //
    // File  attributes, see ZwCreateFile in the DDK for a list of valid
    // values and their meaning.
    //
    ULONG FileAttributes;

    //
    // Create disposition, see ZwCreateFile in the DDK for a list of valid
    // values and their meaning.
    //
    ULONG CreateDisposition;

    //
    // Options for opening the device, see CreateOptions for ZwCreateFile in the
    // DDK for a list of valid values and their meaning.
    //
    ULONG CreateOptions;

    PVOID EaBuffer;

    ULONG EaBufferLength;

    PLONGLONG AllocationSize;

    // ========== WdfIoTargetOpenByName end ==========

    //
    // On return for a create by name, this will contain one of the following
    // values:  FILE_CREATED, FILE_OPENED, FILE_OVERWRITTEN, FILE_SUPERSEDED,
    //          FILE_EXISTS, FILE_DOES_NOT_EXIST
    //
    ULONG FileInformation;

} WDF_IO_TARGET_OPEN_PARAMS, *PWDF_IO_TARGET_OPEN_PARAMS;

VOID
FORCEINLINE
WDF_IO_TARGET_OPEN_PARAMS_INIT_EXISTING_DEVICE(
    OUT PWDF_IO_TARGET_OPEN_PARAMS Params,
    IN PDEVICE_OBJECT DeviceObject
    )
{
    RtlZeroMemory(Params, sizeof(WDF_IO_TARGET_OPEN_PARAMS));

    Params->Size = sizeof(WDF_IO_TARGET_OPEN_PARAMS);
    Params->Type = WdfIoTargetOpenUseExistingDevice;

    Params->TargetDeviceObject = DeviceObject;
}

VOID
FORCEINLINE
WDF_IO_TARGET_OPEN_PARAMS_INIT_CREATE_BY_NAME(
    OUT PWDF_IO_TARGET_OPEN_PARAMS Params,
    IN PUNICODE_STRING TargetDeviceName,
    IN ACCESS_MASK DesiredAccess
    )
{
    RtlZeroMemory(Params, sizeof(WDF_IO_TARGET_OPEN_PARAMS));

    Params->Size = sizeof(WDF_IO_TARGET_OPEN_PARAMS);
    Params->Type = WdfIoTargetOpenByName;

    RtlCopyMemory(&Params->TargetDeviceName,
                  TargetDeviceName,
                  sizeof(UNICODE_STRING));
    Params->DesiredAccess = DesiredAccess;
    Params->CreateOptions = FILE_NON_DIRECTORY_FILE;
}

VOID
FORCEINLINE
WDF_IO_TARGET_OPEN_PARAMS_INIT_OPEN_BY_NAME(
    OUT PWDF_IO_TARGET_OPEN_PARAMS Params,
    IN PUNICODE_STRING TargetDeviceName,
    IN ACCESS_MASK DesiredAccess
    )
{
    WDF_IO_TARGET_OPEN_PARAMS_INIT_CREATE_BY_NAME(Params,
                                                  TargetDeviceName,
                                                  DesiredAccess);
    Params->CreateDisposition = FILE_OPEN;
}

VOID
FORCEINLINE
WDF_IO_TARGET_OPEN_PARAMS_INIT_REOPEN(
    OUT PWDF_IO_TARGET_OPEN_PARAMS Params
    )
{
    RtlZeroMemory(Params, sizeof(WDF_IO_TARGET_OPEN_PARAMS));

    Params->Size = sizeof(WDF_IO_TARGET_OPEN_PARAMS);
    Params->Type = WdfIoTargetOpenReopen;
}

//
// WDF Function: WdfIoTargetCreate
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOTARGETCREATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PWDF_OBJECT_ATTRIBUTES IoTargetAttributes,
    WDFIOTARGET* IoTarget
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoTargetCreate(
    __in
    WDFDEVICE Device,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES IoTargetAttributes,
    __out
    WDFIOTARGET* IoTarget
    )
{
    return ((PFN_WDFIOTARGETCREATE) WdfFunctions[WdfIoTargetCreateTableIndex])(WdfDriverGlobals, Device, IoTargetAttributes, IoTarget);
}

//
// WDF Function: WdfIoTargetOpen
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOTARGETOPEN)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget,
    PWDF_IO_TARGET_OPEN_PARAMS OpenParams
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoTargetOpen(
    __in
    WDFIOTARGET IoTarget,
    __in
    PWDF_IO_TARGET_OPEN_PARAMS OpenParams
    )
{
    return ((PFN_WDFIOTARGETOPEN) WdfFunctions[WdfIoTargetOpenTableIndex])(WdfDriverGlobals, IoTarget, OpenParams);
}

//
// WDF Function: WdfIoTargetCloseForQueryRemove
//
typedef
WDFAPI
VOID
(*PFN_WDFIOTARGETCLOSEFORQUERYREMOVE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget
    );

VOID
FORCEINLINE
WdfIoTargetCloseForQueryRemove(
    __in
    WDFIOTARGET IoTarget
    )
{
    ((PFN_WDFIOTARGETCLOSEFORQUERYREMOVE) WdfFunctions[WdfIoTargetCloseForQueryRemoveTableIndex])(WdfDriverGlobals, IoTarget);
}

//
// WDF Function: WdfIoTargetClose
//
typedef
WDFAPI
VOID
(*PFN_WDFIOTARGETCLOSE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget
    );

VOID
FORCEINLINE
WdfIoTargetClose(
    __in
    WDFIOTARGET IoTarget
    )
{
    ((PFN_WDFIOTARGETCLOSE) WdfFunctions[WdfIoTargetCloseTableIndex])(WdfDriverGlobals, IoTarget);
}

//
// WDF Function: WdfIoTargetStart
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOTARGETSTART)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoTargetStart(
    __in
    WDFIOTARGET IoTarget
    )
{
    return ((PFN_WDFIOTARGETSTART) WdfFunctions[WdfIoTargetStartTableIndex])(WdfDriverGlobals, IoTarget);
}

//
// WDF Function: WdfIoTargetStop
//
typedef
WDFAPI
VOID
(*PFN_WDFIOTARGETSTOP)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget,
    WDF_IO_TARGET_SENT_IO_ACTION Action
    );

VOID
FORCEINLINE
WdfIoTargetStop(
    __in
    WDFIOTARGET IoTarget,
    WDF_IO_TARGET_SENT_IO_ACTION Action
    )
{
    ((PFN_WDFIOTARGETSTOP) WdfFunctions[WdfIoTargetStopTableIndex])(WdfDriverGlobals, IoTarget, Action);
}

//
// WDF Function: WdfIoTargetGetState
//
typedef
WDFAPI
WDF_IO_TARGET_STATE
(*PFN_WDFIOTARGETGETSTATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget
    );

WDF_IO_TARGET_STATE
FORCEINLINE
WdfIoTargetGetState(
    __in
    WDFIOTARGET IoTarget
    )
{
    return ((PFN_WDFIOTARGETGETSTATE) WdfFunctions[WdfIoTargetGetStateTableIndex])(WdfDriverGlobals, IoTarget);
}

//
// WDF Function: WdfIoTargetGetDevice
//
typedef
WDFAPI
WDFDEVICE
(*PFN_WDFIOTARGETGETDEVICE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget
    );

WDFDEVICE
FORCEINLINE
WdfIoTargetGetDevice(
    __in
    WDFIOTARGET IoTarget
    )
{
    return ((PFN_WDFIOTARGETGETDEVICE) WdfFunctions[WdfIoTargetGetDeviceTableIndex])(WdfDriverGlobals, IoTarget);
}

//
// WDF Function: WdfIoTargetQueryTargetProperty
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOTARGETQUERYTARGETPROPERTY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget,
    IN DEVICE_REGISTRY_PROPERTY DeviceProperty,
    IN ULONG BufferLength,
    PVOID PropertyBuffer,
    PULONG ResultLength
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoTargetQueryTargetProperty(
    __in
    WDFIOTARGET IoTarget,
    IN DEVICE_REGISTRY_PROPERTY DeviceProperty,
    IN ULONG BufferLength,
    __out_bcount_full(BufferLength)
    PVOID PropertyBuffer,
    __out
    PULONG ResultLength
    )
{
    return ((PFN_WDFIOTARGETQUERYTARGETPROPERTY) WdfFunctions[WdfIoTargetQueryTargetPropertyTableIndex])(WdfDriverGlobals, IoTarget, DeviceProperty, BufferLength, PropertyBuffer, ResultLength);
}

//
// WDF Function: WdfIoTargetAllocAndQueryTargetProperty
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOTARGETALLOCANDQUERYTARGETPROPERTY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget,
    IN DEVICE_REGISTRY_PROPERTY DeviceProperty,
    IN POOL_TYPE PoolType,
    PWDF_OBJECT_ATTRIBUTES PropertyMemoryAttributes,
    WDFMEMORY* PropertyMemory
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoTargetAllocAndQueryTargetProperty(
    __in
    WDFIOTARGET IoTarget,
    IN DEVICE_REGISTRY_PROPERTY DeviceProperty,
    IN POOL_TYPE PoolType,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES PropertyMemoryAttributes,
    __out
    WDFMEMORY* PropertyMemory
    )
{
    return ((PFN_WDFIOTARGETALLOCANDQUERYTARGETPROPERTY) WdfFunctions[WdfIoTargetAllocAndQueryTargetPropertyTableIndex])(WdfDriverGlobals, IoTarget, DeviceProperty, PoolType, PropertyMemoryAttributes, PropertyMemory);
}

//
// WDF Function: WdfIoTargetQueryForInterface
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOTARGETQUERYFORINTERFACE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget,
    LPCGUID InterfaceType,
    PINTERFACE Interface,
    USHORT Size,
    USHORT Version,
    PVOID InterfaceSpecificData
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoTargetQueryForInterface(
    __in
    WDFIOTARGET IoTarget,
    __in
    LPCGUID InterfaceType,
    __out
    PINTERFACE Interface,
    USHORT Size,
    USHORT Version,
    __in_opt
    PVOID InterfaceSpecificData
    )
{
    return ((PFN_WDFIOTARGETQUERYFORINTERFACE) WdfFunctions[WdfIoTargetQueryForInterfaceTableIndex])(WdfDriverGlobals, IoTarget, InterfaceType, Interface, Size, Version, InterfaceSpecificData);
}

//
// WDF Function: WdfIoTargetWdmGetTargetDeviceObject
//
typedef
WDFAPI
PDEVICE_OBJECT
(*PFN_WDFIOTARGETWDMGETTARGETDEVICEOBJECT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget
    );

PDEVICE_OBJECT
FORCEINLINE
WdfIoTargetWdmGetTargetDeviceObject(
    __in
    WDFIOTARGET IoTarget
    )
{
    return ((PFN_WDFIOTARGETWDMGETTARGETDEVICEOBJECT) WdfFunctions[WdfIoTargetWdmGetTargetDeviceObjectTableIndex])(WdfDriverGlobals, IoTarget);
}

//
// WDF Function: WdfIoTargetWdmGetTargetPhysicalDevice
//
typedef
WDFAPI
PDEVICE_OBJECT
(*PFN_WDFIOTARGETWDMGETTARGETPHYSICALDEVICE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget
    );

PDEVICE_OBJECT
FORCEINLINE
WdfIoTargetWdmGetTargetPhysicalDevice(
    __in
    WDFIOTARGET IoTarget
    )
{
    return ((PFN_WDFIOTARGETWDMGETTARGETPHYSICALDEVICE) WdfFunctions[WdfIoTargetWdmGetTargetPhysicalDeviceTableIndex])(WdfDriverGlobals, IoTarget);
}

//
// WDF Function: WdfIoTargetWdmGetTargetFileObject
//
typedef
WDFAPI
PFILE_OBJECT
(*PFN_WDFIOTARGETWDMGETTARGETFILEOBJECT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget
    );

PFILE_OBJECT
FORCEINLINE
WdfIoTargetWdmGetTargetFileObject(
    __in
    WDFIOTARGET IoTarget
    )
{
    return ((PFN_WDFIOTARGETWDMGETTARGETFILEOBJECT) WdfFunctions[WdfIoTargetWdmGetTargetFileObjectTableIndex])(WdfDriverGlobals, IoTarget);
}

//
// WDF Function: WdfIoTargetWdmGetTargetFileHandle
//
typedef
WDFAPI
HANDLE
(*PFN_WDFIOTARGETWDMGETTARGETFILEHANDLE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget
    );

HANDLE
FORCEINLINE
WdfIoTargetWdmGetTargetFileHandle(
    __in
    WDFIOTARGET IoTarget
    )
{
    return ((PFN_WDFIOTARGETWDMGETTARGETFILEHANDLE) WdfFunctions[WdfIoTargetWdmGetTargetFileHandleTableIndex])(WdfDriverGlobals, IoTarget);
}

//
// WDF Function: WdfIoTargetSendReadSynchronously
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOTARGETSENDREADSYNCHRONOUSLY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget,
    WDFREQUEST Request,
    PWDF_MEMORY_DESCRIPTOR OutputBuffer,
    PLONGLONG DeviceOffset,
    PWDF_REQUEST_SEND_OPTIONS RequestOptions,
    PULONG_PTR BytesRead
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoTargetSendReadSynchronously(
    __in
    WDFIOTARGET IoTarget,
    __in_opt
    WDFREQUEST Request,
    __in_opt
    PWDF_MEMORY_DESCRIPTOR OutputBuffer,
    __in_opt
    PLONGLONG DeviceOffset,
    __in_opt
    PWDF_REQUEST_SEND_OPTIONS RequestOptions,
    __out_opt
    PULONG_PTR BytesRead
    )
{
    return ((PFN_WDFIOTARGETSENDREADSYNCHRONOUSLY) WdfFunctions[WdfIoTargetSendReadSynchronouslyTableIndex])(WdfDriverGlobals, IoTarget, Request, OutputBuffer, DeviceOffset, RequestOptions, BytesRead);
}

//
// WDF Function: WdfIoTargetFormatRequestForRead
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOTARGETFORMATREQUESTFORREAD)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget,
    WDFREQUEST Request,
    WDFMEMORY OutputBuffer,
    PWDFMEMORY_OFFSET OutputBufferOffset,
    PLONGLONG DeviceOffset
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoTargetFormatRequestForRead(
    __in
    WDFIOTARGET IoTarget,
    __in
    WDFREQUEST Request,
    __in_opt
    WDFMEMORY OutputBuffer,
    __in_opt
    PWDFMEMORY_OFFSET OutputBufferOffset,
    __in_opt
    PLONGLONG DeviceOffset
    )
{
    return ((PFN_WDFIOTARGETFORMATREQUESTFORREAD) WdfFunctions[WdfIoTargetFormatRequestForReadTableIndex])(WdfDriverGlobals, IoTarget, Request, OutputBuffer, OutputBufferOffset, DeviceOffset);
}

//
// WDF Function: WdfIoTargetSendWriteSynchronously
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOTARGETSENDWRITESYNCHRONOUSLY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget,
    WDFREQUEST Request,
    PWDF_MEMORY_DESCRIPTOR InputBuffer,
    PLONGLONG DeviceOffset,
    PWDF_REQUEST_SEND_OPTIONS RequestOptions,
    PULONG_PTR BytesWritten
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoTargetSendWriteSynchronously(
    __in
    WDFIOTARGET IoTarget,
    __in_opt
    WDFREQUEST Request,
    __in_opt
    PWDF_MEMORY_DESCRIPTOR InputBuffer,
    __in_opt
    PLONGLONG DeviceOffset,
    __in_opt
    PWDF_REQUEST_SEND_OPTIONS RequestOptions,
    __out_opt
    PULONG_PTR BytesWritten
    )
{
    return ((PFN_WDFIOTARGETSENDWRITESYNCHRONOUSLY) WdfFunctions[WdfIoTargetSendWriteSynchronouslyTableIndex])(WdfDriverGlobals, IoTarget, Request, InputBuffer, DeviceOffset, RequestOptions, BytesWritten);
}

//
// WDF Function: WdfIoTargetFormatRequestForWrite
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOTARGETFORMATREQUESTFORWRITE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget,
    WDFREQUEST Request,
    WDFMEMORY InputBuffer,
    PWDFMEMORY_OFFSET InputBufferOffset,
    PLONGLONG DeviceOffset
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoTargetFormatRequestForWrite(
    __in
    WDFIOTARGET IoTarget,
    __in
    WDFREQUEST Request,
    __in_opt
    WDFMEMORY InputBuffer,
    __in_opt
    PWDFMEMORY_OFFSET InputBufferOffset,
    __in_opt
    PLONGLONG DeviceOffset
    )
{
    return ((PFN_WDFIOTARGETFORMATREQUESTFORWRITE) WdfFunctions[WdfIoTargetFormatRequestForWriteTableIndex])(WdfDriverGlobals, IoTarget, Request, InputBuffer, InputBufferOffset, DeviceOffset);
}

//
// WDF Function: WdfIoTargetSendIoctlSynchronously
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOTARGETSENDIOCTLSYNCHRONOUSLY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget,
    WDFREQUEST Request,
    IN ULONG IoctlCode,
    PWDF_MEMORY_DESCRIPTOR InputBuffer,
    PWDF_MEMORY_DESCRIPTOR OutputBuffer,
    PWDF_REQUEST_SEND_OPTIONS RequestOptions,
    PULONG_PTR BytesReturned
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoTargetSendIoctlSynchronously(
    __in
    WDFIOTARGET IoTarget,
    __in_opt
    WDFREQUEST Request,
    IN ULONG IoctlCode,
    __in_opt
    PWDF_MEMORY_DESCRIPTOR InputBuffer,
    __in_opt
    PWDF_MEMORY_DESCRIPTOR OutputBuffer,
    __in_opt
    PWDF_REQUEST_SEND_OPTIONS RequestOptions,
    __out_opt
    PULONG_PTR BytesReturned
    )
{
    return ((PFN_WDFIOTARGETSENDIOCTLSYNCHRONOUSLY) WdfFunctions[WdfIoTargetSendIoctlSynchronouslyTableIndex])(WdfDriverGlobals, IoTarget, Request, IoctlCode, InputBuffer, OutputBuffer, RequestOptions, BytesReturned);
}

//
// WDF Function: WdfIoTargetFormatRequestForIoctl
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOTARGETFORMATREQUESTFORIOCTL)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget,
    WDFREQUEST Request,
    ULONG IoctlCode,
    WDFMEMORY InputBuffer,
    PWDFMEMORY_OFFSET InputBufferOffset,
    WDFMEMORY OutputBuffer,
    PWDFMEMORY_OFFSET OutputBufferOffset
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoTargetFormatRequestForIoctl(
    __in
    WDFIOTARGET IoTarget,
    __in
    WDFREQUEST Request,
    ULONG IoctlCode,
    __in_opt
    WDFMEMORY InputBuffer,
    __in_opt
    PWDFMEMORY_OFFSET InputBufferOffset,
    __in_opt
    WDFMEMORY OutputBuffer,
    __in_opt
    PWDFMEMORY_OFFSET OutputBufferOffset
    )
{
    return ((PFN_WDFIOTARGETFORMATREQUESTFORIOCTL) WdfFunctions[WdfIoTargetFormatRequestForIoctlTableIndex])(WdfDriverGlobals, IoTarget, Request, IoctlCode, InputBuffer, InputBufferOffset, OutputBuffer, OutputBufferOffset);
}

//
// WDF Function: WdfIoTargetSendInternalIoctlSynchronously
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOTARGETSENDINTERNALIOCTLSYNCHRONOUSLY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget,
    WDFREQUEST Request,
    ULONG IoctlCode,
    PWDF_MEMORY_DESCRIPTOR InputBuffer,
    PWDF_MEMORY_DESCRIPTOR OutputBuffer,
    PWDF_REQUEST_SEND_OPTIONS RequestOptions,
    PULONG_PTR BytesReturned
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoTargetSendInternalIoctlSynchronously(
    __in
    WDFIOTARGET IoTarget,
    __in_opt
    WDFREQUEST Request,
    ULONG IoctlCode,
    __in_opt
    PWDF_MEMORY_DESCRIPTOR InputBuffer,
    __in_opt
    PWDF_MEMORY_DESCRIPTOR OutputBuffer,
    __in_opt
    PWDF_REQUEST_SEND_OPTIONS RequestOptions,
    __out_opt
    PULONG_PTR BytesReturned
    )
{
    return ((PFN_WDFIOTARGETSENDINTERNALIOCTLSYNCHRONOUSLY) WdfFunctions[WdfIoTargetSendInternalIoctlSynchronouslyTableIndex])(WdfDriverGlobals, IoTarget, Request, IoctlCode, InputBuffer, OutputBuffer, RequestOptions, BytesReturned);
}

//
// WDF Function: WdfIoTargetFormatRequestForInternalIoctl
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOTARGETFORMATREQUESTFORINTERNALIOCTL)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget,
    WDFREQUEST Request,
    ULONG IoctlCode,
    WDFMEMORY InputBuffer,
    PWDFMEMORY_OFFSET InputBufferOffset,
    WDFMEMORY OutputBuffer,
    PWDFMEMORY_OFFSET OutputBufferOffset
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoTargetFormatRequestForInternalIoctl(
    __in
    WDFIOTARGET IoTarget,
    __in
    WDFREQUEST Request,
    ULONG IoctlCode,
    __in_opt
    WDFMEMORY InputBuffer,
    __in_opt
    PWDFMEMORY_OFFSET InputBufferOffset,
    __in_opt
    WDFMEMORY OutputBuffer,
    __in_opt
    PWDFMEMORY_OFFSET OutputBufferOffset
    )
{
    return ((PFN_WDFIOTARGETFORMATREQUESTFORINTERNALIOCTL) WdfFunctions[WdfIoTargetFormatRequestForInternalIoctlTableIndex])(WdfDriverGlobals, IoTarget, Request, IoctlCode, InputBuffer, InputBufferOffset, OutputBuffer, OutputBufferOffset);
}

//
// WDF Function: WdfIoTargetSendInternalIoctlOthersSynchronously
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOTARGETSENDINTERNALIOCTLOTHERSSYNCHRONOUSLY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget,
    WDFREQUEST Request,
    ULONG IoctlCode,
    PWDF_MEMORY_DESCRIPTOR OtherArg1,
    PWDF_MEMORY_DESCRIPTOR OtherArg2,
    PWDF_MEMORY_DESCRIPTOR OtherArg4,
    PWDF_REQUEST_SEND_OPTIONS RequestOptions,
    PULONG_PTR BytesReturned
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoTargetSendInternalIoctlOthersSynchronously(
    __in
    WDFIOTARGET IoTarget,
    __in_opt
    WDFREQUEST Request,
    ULONG IoctlCode,
    __in_opt
    PWDF_MEMORY_DESCRIPTOR OtherArg1,
    __in_opt
    PWDF_MEMORY_DESCRIPTOR OtherArg2,
    __in_opt
    PWDF_MEMORY_DESCRIPTOR OtherArg4,
    __in_opt
    PWDF_REQUEST_SEND_OPTIONS RequestOptions,
    __out_opt
    PULONG_PTR BytesReturned
    )
{
    return ((PFN_WDFIOTARGETSENDINTERNALIOCTLOTHERSSYNCHRONOUSLY) WdfFunctions[WdfIoTargetSendInternalIoctlOthersSynchronouslyTableIndex])(WdfDriverGlobals, IoTarget, Request, IoctlCode, OtherArg1, OtherArg2, OtherArg4, RequestOptions, BytesReturned);
}

//
// WDF Function: WdfIoTargetFormatRequestForInternalIoctlOthers
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOTARGETFORMATREQUESTFORINTERNALIOCTLOTHERS)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIOTARGET IoTarget,
    WDFREQUEST Request,
    ULONG IoctlCode,
    WDFMEMORY OtherArg1,
    PWDFMEMORY_OFFSET OtherArg1Offset,
    WDFMEMORY OtherArg2,
    PWDFMEMORY_OFFSET OtherArg2Offset,
    WDFMEMORY OtherArg4,
    PWDFMEMORY_OFFSET OtherArg4Offset
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoTargetFormatRequestForInternalIoctlOthers(
    __in
    WDFIOTARGET IoTarget,
    __in
    WDFREQUEST Request,
    ULONG IoctlCode,
    __in_opt
    WDFMEMORY OtherArg1,
    __in_opt
    PWDFMEMORY_OFFSET OtherArg1Offset,
    __in_opt
    WDFMEMORY OtherArg2,
    __in_opt
    PWDFMEMORY_OFFSET OtherArg2Offset,
    __in_opt
    WDFMEMORY OtherArg4,
    __in_opt
    PWDFMEMORY_OFFSET OtherArg4Offset
    )
{
    return ((PFN_WDFIOTARGETFORMATREQUESTFORINTERNALIOCTLOTHERS) WdfFunctions[WdfIoTargetFormatRequestForInternalIoctlOthersTableIndex])(WdfDriverGlobals, IoTarget, Request, IoctlCode, OtherArg1, OtherArg1Offset, OtherArg2, OtherArg2Offset, OtherArg4, OtherArg4Offset);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFIOTARGET_H_

