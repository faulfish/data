/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    WdfQueryInterface.h

Abstract:

    This file contains the DDIs for handling query interface

Environment:

    kernel mode only

Revision History:

--*/

#ifndef _WDFQUERYINTERFACE_H_
#define _WDFQUERYINTERFACE_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)



typedef
NTSTATUS
(*PFN_WDF_DEVICE_PROCESS_QUERY_INTERFACE_REQUEST)(
    IN WDFDEVICE Device,
    IN LPGUID InterfaceType,
    IN OUT PINTERFACE ExposedInterface,
    IN OUT PVOID ExposedInterfaceSpecificData
    );

typedef struct _WDF_QUERY_INTERFACE_CONFIG {
    //
    // Size of this structure in bytes.
    //
    ULONG Size;

    //
    // Interface to be returned to the caller.  Optional if BehaviorType is set
    // to WdfQueryInterfaceTypePassThrough or ImportInterface is set to TRUE.
    //
    PINTERFACE Interface;

    //
    // The GUID identifying the interface
    //
    CONST GUID * InterfaceType;

    //
    // Valid only for PDOs.  The framework will allocate a new request and
    // forward it down the parent's device stack.
    //
    BOOLEAN SendQueryToParentStack;


    //
    // Driver supplied callback which is called after some basic interface
    // validation has been performed (size, version, and guid checking).  This
    // is an optional parameter and may be NULL unless ImportInterface is
    // specified.
    //
    // If the callback returns !NT_SUCCESS, this error will be returned to the
    // caller and the query interface will fail.
    //
    // In this callback, the caller is free to modify the ExposedInterface in
    // any manner of its choosing.  For instance, the callback may change any
    // field in the interface.  The callback may also alloate a dynamic context
    // to be associated with the interface; the InterfaceReference and
    // InterfaceDereference functions may also be overridden.
    //
    // If ImportInterface is set to TRUE, then this is a required field and the
    // callback must initialize the interface (the framework will leave the
    // ExposedInterface buffer exactly as it received it) since the framework
    // has no way of knowing which fields to fill in and which to leave alone.
    //
    PFN_WDF_DEVICE_PROCESS_QUERY_INTERFACE_REQUEST EvtDeviceProcessQueryInterfaceRequest;

    //
    // If TRUE, the interface provided by the caller contains data that the
    // driver is interested in.  By setting to this field to TRUE, the
    // EvtDeviceProcessQueryInterfaceRequest callback must initialize the
    // ExposedInterface.
    //
    // If FALSE, the entire ExposedInterface is initialized through a memory
    // copy before the EvtDeviceProcessQueryInterfaceRequest is called.
    //
    BOOLEAN ImportInterface;

} WDF_QUERY_INTERFACE_CONFIG, *PWDF_QUERY_INTERFACE_CONFIG;

VOID
FORCEINLINE
WDF_QUERY_INTERFACE_CONFIG_INIT(
    OUT PWDF_QUERY_INTERFACE_CONFIG InterfaceConfig,
    IN PINTERFACE Interface,
    IN CONST GUID* InterfaceType,
    IN OPTIONAL PFN_WDF_DEVICE_PROCESS_QUERY_INTERFACE_REQUEST EvtDeviceProcessQueryInterfaceRequest
    )
{
    RtlZeroMemory(InterfaceConfig, sizeof(WDF_QUERY_INTERFACE_CONFIG));

    InterfaceConfig->Size = sizeof(WDF_QUERY_INTERFACE_CONFIG);
    InterfaceConfig->Interface = Interface;
    InterfaceConfig->InterfaceType = InterfaceType;
    InterfaceConfig->EvtDeviceProcessQueryInterfaceRequest =
        EvtDeviceProcessQueryInterfaceRequest;
}

//
// WDF Function: WdfDeviceAddQueryInterface
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICEADDQUERYINTERFACE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PWDF_QUERY_INTERFACE_CONFIG InterfaceConfig
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceAddQueryInterface(
    __in
    WDFDEVICE Device,
    __in
    PWDF_QUERY_INTERFACE_CONFIG InterfaceConfig
    )
{
    return ((PFN_WDFDEVICEADDQUERYINTERFACE) WdfFunctions[WdfDeviceAddQueryInterfaceTableIndex])(WdfDriverGlobals, Device, InterfaceConfig);
}



//
// The following functions can be assigned to INTERFACE::InterfaceReference
// and INTERFACE::InterfaceDeference
//
// o WdfDeviceInterfaceReferenceNoOp
// o WdfDeviceInterfaceDeReferenceNoOp
//
// Thes 2 functions do nothing when called.  These are appropriate for
// interfaces which can be queried for and used independent of the PNP state of
// the WDFDEVICE the interface was retrieved from.
//
VOID
FORCEINLINE
WdfDeviceInterfaceReferenceNoOp(
    PVOID Context
    )
{
    UNREFERENCED_PARAMETER(Context);
}

VOID
FORCEINLINE
WdfDeviceInterfaceDereferenceNoOp(
    PVOID Context
    )
{
    UNREFERENCED_PARAMETER(Context);
}

#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFQUERYINTERFACE_H_

