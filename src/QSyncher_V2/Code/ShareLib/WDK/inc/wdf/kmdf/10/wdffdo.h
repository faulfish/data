/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    WdfFdo.h

Abstract:

    This is the interface to the FDO functionality in the framework.  This also
    covers filters.

Environment:

    kernel mode only

Revision History:

--*/

#ifndef _WDFFDO_H_
#define _WDFFDO_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)



typedef
NTSTATUS
(*PFN_WDF_DEVICE_FILTER_RESOURCE_REQUIREMENTS)(
    IN WDFDEVICE Device,
    IN WDFIORESREQLIST IoResourceRequirementsList
    );

typedef
NTSTATUS
(*PFN_WDF_DEVICE_REMOVE_ADDED_RESOURCES)(
    IN WDFDEVICE Device,
    IN  WDFCMRESLIST ResourcesRaw,
    IN  WDFCMRESLIST ResourcesTranslated
    );

typedef struct _WDF_FDO_EVENT_CALLBACKS {
    //
    // Size of this structure in bytes
    //
    ULONG Size;

    PFN_WDF_DEVICE_FILTER_RESOURCE_REQUIREMENTS EvtDeviceFilterAddResourceRequirements;

    PFN_WDF_DEVICE_FILTER_RESOURCE_REQUIREMENTS EvtDeviceFilterRemoveResourceRequirements;

    PFN_WDF_DEVICE_REMOVE_ADDED_RESOURCES EvtDeviceRemoveAddedResources;

} WDF_FDO_EVENT_CALLBACKS, *PWDF_FDO_EVENT_CALLBACKS;

VOID
FORCEINLINE
WDF_FDO_EVENT_CALLBACKS_INIT(
    OUT PWDF_FDO_EVENT_CALLBACKS Callbacks
    )
{
    RtlZeroMemory(Callbacks, sizeof(WDF_FDO_EVENT_CALLBACKS));
    Callbacks->Size = sizeof(WDF_FDO_EVENT_CALLBACKS);
}

//
// WDF Function: WdfFdoInitWdmGetPhysicalDevice
//
typedef
WDFAPI
PDEVICE_OBJECT
(*PFN_WDFFDOINITWDMGETPHYSICALDEVICE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit
    );

PDEVICE_OBJECT
FORCEINLINE
WdfFdoInitWdmGetPhysicalDevice(
    __in
    PWDFDEVICE_INIT DeviceInit
    )
{
    return ((PFN_WDFFDOINITWDMGETPHYSICALDEVICE) WdfFunctions[WdfFdoInitWdmGetPhysicalDeviceTableIndex])(WdfDriverGlobals, DeviceInit);
}

//
// WDF Function: WdfFdoInitOpenRegistryKey
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFFDOINITOPENREGISTRYKEY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    IN ULONG DeviceInstanceKeyType,
    IN ACCESS_MASK DesiredAccess,
    PWDF_OBJECT_ATTRIBUTES KeyAttributes,
    WDFKEY* Key
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfFdoInitOpenRegistryKey(
    __in
    PWDFDEVICE_INIT DeviceInit,
    IN ULONG DeviceInstanceKeyType,
    IN ACCESS_MASK DesiredAccess,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES KeyAttributes,
    __out
    WDFKEY* Key
    )
{
    return ((PFN_WDFFDOINITOPENREGISTRYKEY) WdfFunctions[WdfFdoInitOpenRegistryKeyTableIndex])(WdfDriverGlobals, DeviceInit, DeviceInstanceKeyType, DesiredAccess, KeyAttributes, Key);
}

//
// WDF Function: WdfFdoInitQueryProperty
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFFDOINITQUERYPROPERTY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    IN DEVICE_REGISTRY_PROPERTY DeviceProperty,
    IN ULONG BufferLength,
    PVOID PropertyBuffer,
    PULONG ResultLength
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfFdoInitQueryProperty(
    __in
    PWDFDEVICE_INIT DeviceInit,
    IN DEVICE_REGISTRY_PROPERTY DeviceProperty,
    IN ULONG BufferLength,
    __out_bcount_full(BufferLength)
    PVOID PropertyBuffer,
    __out
    PULONG ResultLength
    )
{
    return ((PFN_WDFFDOINITQUERYPROPERTY) WdfFunctions[WdfFdoInitQueryPropertyTableIndex])(WdfDriverGlobals, DeviceInit, DeviceProperty, BufferLength, PropertyBuffer, ResultLength);
}

//
// WDF Function: WdfFdoInitAllocAndQueryProperty
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFFDOINITALLOCANDQUERYPROPERTY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    IN DEVICE_REGISTRY_PROPERTY DeviceProperty,
    POOL_TYPE PoolType,
    PWDF_OBJECT_ATTRIBUTES PropertyMemoryAttributes,
    WDFMEMORY* PropertyMemory
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfFdoInitAllocAndQueryProperty(
    __in
    PWDFDEVICE_INIT DeviceInit,
    IN DEVICE_REGISTRY_PROPERTY DeviceProperty,
    POOL_TYPE PoolType,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES PropertyMemoryAttributes,
    __out
    WDFMEMORY* PropertyMemory
    )
{
    return ((PFN_WDFFDOINITALLOCANDQUERYPROPERTY) WdfFunctions[WdfFdoInitAllocAndQueryPropertyTableIndex])(WdfDriverGlobals, DeviceInit, DeviceProperty, PoolType, PropertyMemoryAttributes, PropertyMemory);
}

//
// WDF Function: WdfFdoInitSetEventCallbacks
//
typedef
WDFAPI
VOID
(*PFN_WDFFDOINITSETEVENTCALLBACKS)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    PWDF_FDO_EVENT_CALLBACKS FdoEventCallbacks
    );

VOID
FORCEINLINE
WdfFdoInitSetEventCallbacks(
    __in
    PWDFDEVICE_INIT DeviceInit,
    __in
    PWDF_FDO_EVENT_CALLBACKS FdoEventCallbacks
    )
{
    ((PFN_WDFFDOINITSETEVENTCALLBACKS) WdfFunctions[WdfFdoInitSetEventCallbacksTableIndex])(WdfDriverGlobals, DeviceInit, FdoEventCallbacks);
}

//
// WDF Function: WdfFdoInitSetFilter
//
typedef
WDFAPI
VOID
(*PFN_WDFFDOINITSETFILTER)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit
    );

VOID
FORCEINLINE
WdfFdoInitSetFilter(
    __in
    PWDFDEVICE_INIT DeviceInit
    )
{
    ((PFN_WDFFDOINITSETFILTER) WdfFunctions[WdfFdoInitSetFilterTableIndex])(WdfDriverGlobals, DeviceInit);
}

//
// WDF Function: WdfFdoInitSetDefaultChildListConfig
//
typedef
WDFAPI
VOID
(*PFN_WDFFDOINITSETDEFAULTCHILDLISTCONFIG)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    PWDF_CHILD_LIST_CONFIG Config,
    PWDF_OBJECT_ATTRIBUTES DefaultChildListAttributes
    );

VOID
FORCEINLINE
WdfFdoInitSetDefaultChildListConfig(
    __inout
    PWDFDEVICE_INIT DeviceInit,
    __in
    PWDF_CHILD_LIST_CONFIG Config,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES DefaultChildListAttributes
    )
{
    ((PFN_WDFFDOINITSETDEFAULTCHILDLISTCONFIG) WdfFunctions[WdfFdoInitSetDefaultChildListConfigTableIndex])(WdfDriverGlobals, DeviceInit, Config, DefaultChildListAttributes);
}

//
// WDF Function: WdfFdoQueryForInterface
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFFDOQUERYFORINTERFACE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Fdo,
    LPCGUID InterfaceType,
    PINTERFACE Interface,
    IN USHORT Size,
    IN USHORT Version,
    PVOID InterfaceSpecificData
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfFdoQueryForInterface(
    __in
    WDFDEVICE Fdo,
    __in
    LPCGUID InterfaceType,
    __out
    PINTERFACE Interface,
    IN USHORT Size,
    IN USHORT Version,
    __in_opt
    PVOID InterfaceSpecificData
    )
{
    return ((PFN_WDFFDOQUERYFORINTERFACE) WdfFunctions[WdfFdoQueryForInterfaceTableIndex])(WdfDriverGlobals, Fdo, InterfaceType, Interface, Size, Version, InterfaceSpecificData);
}

//
// WDF Function: WdfFdoGetDefaultChildList
//
typedef
WDFAPI
WDFCHILDLIST
(*PFN_WDFFDOGETDEFAULTCHILDLIST)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Fdo
    );

WDFCHILDLIST
FORCEINLINE
WdfFdoGetDefaultChildList(
    __in
    WDFDEVICE Fdo
    )
{
    return ((PFN_WDFFDOGETDEFAULTCHILDLIST) WdfFunctions[WdfFdoGetDefaultChildListTableIndex])(WdfDriverGlobals, Fdo);
}

//
// WDF Function: WdfFdoAddStaticChild
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFFDOADDSTATICCHILD)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Fdo,
    WDFDEVICE Child
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfFdoAddStaticChild(
    __in
    WDFDEVICE Fdo,
    __in
    WDFDEVICE Child
    )
{
    return ((PFN_WDFFDOADDSTATICCHILD) WdfFunctions[WdfFdoAddStaticChildTableIndex])(WdfDriverGlobals, Fdo, Child);
}

//
// WDF Function: WdfFdoLockStaticChildListForIteration
//
typedef
WDFAPI
VOID
(*PFN_WDFFDOLOCKSTATICCHILDLISTFORITERATION)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Fdo
    );

VOID
FORCEINLINE
WdfFdoLockStaticChildListForIteration(
    __in
    WDFDEVICE Fdo
    )
{
    ((PFN_WDFFDOLOCKSTATICCHILDLISTFORITERATION) WdfFunctions[WdfFdoLockStaticChildListForIterationTableIndex])(WdfDriverGlobals, Fdo);
}

//
// WDF Function: WdfFdoRetrieveNextStaticChild
//
typedef
WDFAPI
WDFDEVICE
(*PFN_WDFFDORETRIEVENEXTSTATICCHILD)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Fdo,
    WDFDEVICE PreviousChild,
    IN ULONG Flags
    );

WDFDEVICE
FORCEINLINE
WdfFdoRetrieveNextStaticChild(
    __in
    WDFDEVICE Fdo,
    __in_opt
    WDFDEVICE PreviousChild,
    IN ULONG Flags
    )
{
    return ((PFN_WDFFDORETRIEVENEXTSTATICCHILD) WdfFunctions[WdfFdoRetrieveNextStaticChildTableIndex])(WdfDriverGlobals, Fdo, PreviousChild, Flags);
}

//
// WDF Function: WdfFdoUnlockStaticChildListFromIteration
//
typedef
WDFAPI
VOID
(*PFN_WDFFDOUNLOCKSTATICCHILDLISTFROMITERATION)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Fdo
    );

VOID
FORCEINLINE
WdfFdoUnlockStaticChildListFromIteration(
    __in
    WDFDEVICE Fdo
    )
{
    ((PFN_WDFFDOUNLOCKSTATICCHILDLISTFROMITERATION) WdfFunctions[WdfFdoUnlockStaticChildListFromIterationTableIndex])(WdfDriverGlobals, Fdo);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFFDO_H_

