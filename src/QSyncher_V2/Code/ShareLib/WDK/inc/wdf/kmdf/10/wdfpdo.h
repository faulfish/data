/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    WdfPdo.h

Abstract:

    This is the interface to the PDO WDFDEVICE handle.

Environment:

    kernel mode only

Revision History:

--*/

#ifndef _WDFPDO_H_
#define _WDFPDO_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)



typedef
NTSTATUS
(*PFN_WDF_DEVICE_RESOURCES_QUERY)(
    IN WDFDEVICE Device,
    IN WDFCMRESLIST Resources
    );

typedef
NTSTATUS
(*PFN_WDF_DEVICE_RESOURCE_REQUIREMENTS_QUERY)(
    IN WDFDEVICE Device,
    IN WDFIORESREQLIST IoResourceRequirementsList
    );

typedef
NTSTATUS
(*PFN_WDF_DEVICE_EJECT)(
    IN WDFDEVICE Device
    );

typedef
NTSTATUS
(*PFN_WDF_DEVICE_SET_LOCK)(
    IN WDFDEVICE Device,
    IN BOOLEAN IsLocked
    );

typedef
NTSTATUS
(*PFN_WDF_DEVICE_ENABLE_WAKE_AT_BUS)(
    IN WDFDEVICE Device,
    IN SYSTEM_POWER_STATE PowerState
    );

typedef
VOID
(*PFN_WDF_DEVICE_DISABLE_WAKE_AT_BUS)(
    IN WDFDEVICE Device
    );

typedef struct _WDF_PDO_EVENT_CALLBACKS {
    //
    // The size of this structure in bytes
    //
    ULONG Size;

    //
    // Called in response to IRP_MN_QUERY_RESOURCES
    //
    PFN_WDF_DEVICE_RESOURCES_QUERY EvtDeviceResourcesQuery;

    //
    // Called in response to IRP_MN_QUERY_RESOURCE_REQUIREMENTS
    //
    PFN_WDF_DEVICE_RESOURCE_REQUIREMENTS_QUERY EvtDeviceResourceRequirementsQuery;

    //
    // Called in response to IRP_MN_EJECT
    //
    PFN_WDF_DEVICE_EJECT EvtDeviceEject;

    //
    // Called in response to IRP_MN_SET_LOCK
    //
    PFN_WDF_DEVICE_SET_LOCK EvtDeviceSetLock;

    //
    // Called in response to the power policy owner sending a wait wake to the
    // PDO.  Bus generic arming shoulding occur here.
    //
    PFN_WDF_DEVICE_ENABLE_WAKE_AT_BUS       EvtDeviceEnableWakeAtBus;

    //
    // Called in response to the power policy owner sending a wait wake to the
    // PDO.  Bus generic disarming shoulding occur here.
    //
    PFN_WDF_DEVICE_DISABLE_WAKE_AT_BUS      EvtDeviceDisableWakeAtBus;

} WDF_PDO_EVENT_CALLBACKS, *PWDF_PDO_EVENT_CALLBACKS;

VOID
FORCEINLINE
WDF_PDO_EVENT_CALLBACKS_INIT(
    OUT PWDF_PDO_EVENT_CALLBACKS Callbacks
    )
{
    RtlZeroMemory(Callbacks, sizeof(WDF_PDO_EVENT_CALLBACKS));
    Callbacks->Size = sizeof(WDF_PDO_EVENT_CALLBACKS);
}

//
// WDF Function: WdfPdoInitAllocate
//
typedef
WDFAPI
PWDFDEVICE_INIT
(*PFN_WDFPDOINITALLOCATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE ParentDevice
    );

__checkReturn
PWDFDEVICE_INIT
FORCEINLINE
WdfPdoInitAllocate(
    __in
    WDFDEVICE ParentDevice
    )
{
    return ((PFN_WDFPDOINITALLOCATE) WdfFunctions[WdfPdoInitAllocateTableIndex])(WdfDriverGlobals, ParentDevice);
}

//
// WDF Function: WdfPdoInitSetEventCallbacks
//
typedef
WDFAPI
VOID
(*PFN_WDFPDOINITSETEVENTCALLBACKS)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    PWDF_PDO_EVENT_CALLBACKS DispatchTable
    );

VOID
FORCEINLINE
WdfPdoInitSetEventCallbacks(
    __in
    PWDFDEVICE_INIT DeviceInit,
    __in
    PWDF_PDO_EVENT_CALLBACKS DispatchTable
    )
{
    ((PFN_WDFPDOINITSETEVENTCALLBACKS) WdfFunctions[WdfPdoInitSetEventCallbacksTableIndex])(WdfDriverGlobals, DeviceInit, DispatchTable);
}

//
// WDF Function: WdfPdoInitAssignDeviceID
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFPDOINITASSIGNDEVICEID)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    PCUNICODE_STRING DeviceID
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfPdoInitAssignDeviceID(
    __in
    PWDFDEVICE_INIT DeviceInit,
    __in
    PCUNICODE_STRING DeviceID
    )
{
    return ((PFN_WDFPDOINITASSIGNDEVICEID) WdfFunctions[WdfPdoInitAssignDeviceIDTableIndex])(WdfDriverGlobals, DeviceInit, DeviceID);
}

//
// WDF Function: WdfPdoInitAssignInstanceID
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFPDOINITASSIGNINSTANCEID)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    PCUNICODE_STRING InstanceID
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfPdoInitAssignInstanceID(
    __in
    PWDFDEVICE_INIT DeviceInit,
    __in
    PCUNICODE_STRING InstanceID
    )
{
    return ((PFN_WDFPDOINITASSIGNINSTANCEID) WdfFunctions[WdfPdoInitAssignInstanceIDTableIndex])(WdfDriverGlobals, DeviceInit, InstanceID);
}

//
// WDF Function: WdfPdoInitAddHardwareID
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFPDOINITADDHARDWAREID)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    PCUNICODE_STRING HardwareID
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfPdoInitAddHardwareID(
    __in
    PWDFDEVICE_INIT DeviceInit,
    __in
    PCUNICODE_STRING HardwareID
    )
{
    return ((PFN_WDFPDOINITADDHARDWAREID) WdfFunctions[WdfPdoInitAddHardwareIDTableIndex])(WdfDriverGlobals, DeviceInit, HardwareID);
}

//
// WDF Function: WdfPdoInitAddCompatibleID
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFPDOINITADDCOMPATIBLEID)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    PCUNICODE_STRING CompatibleID
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfPdoInitAddCompatibleID(
    __in
    PWDFDEVICE_INIT DeviceInit,
    __in
    PCUNICODE_STRING CompatibleID
    )
{
    return ((PFN_WDFPDOINITADDCOMPATIBLEID) WdfFunctions[WdfPdoInitAddCompatibleIDTableIndex])(WdfDriverGlobals, DeviceInit, CompatibleID);
}

//
// WDF Function: WdfPdoInitAddDeviceText
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFPDOINITADDDEVICETEXT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    PCUNICODE_STRING DeviceDescription,
    PCUNICODE_STRING DeviceLocation,
    IN LCID LocaleId
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfPdoInitAddDeviceText(
    __in
    PWDFDEVICE_INIT DeviceInit,
    __in
    PCUNICODE_STRING DeviceDescription,
    __in
    PCUNICODE_STRING DeviceLocation,
    IN LCID LocaleId
    )
{
    return ((PFN_WDFPDOINITADDDEVICETEXT) WdfFunctions[WdfPdoInitAddDeviceTextTableIndex])(WdfDriverGlobals, DeviceInit, DeviceDescription, DeviceLocation, LocaleId);
}

//
// WDF Function: WdfPdoInitSetDefaultLocale
//
typedef
WDFAPI
VOID
(*PFN_WDFPDOINITSETDEFAULTLOCALE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    IN LCID LocaleId
    );

VOID
FORCEINLINE
WdfPdoInitSetDefaultLocale(
    __in
    PWDFDEVICE_INIT DeviceInit,
    IN LCID LocaleId
    )
{
    ((PFN_WDFPDOINITSETDEFAULTLOCALE) WdfFunctions[WdfPdoInitSetDefaultLocaleTableIndex])(WdfDriverGlobals, DeviceInit, LocaleId);
}

//
// WDF Function: WdfPdoInitAssignRawDevice
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFPDOINITASSIGNRAWDEVICE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    CONST GUID* DeviceClassGuid
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfPdoInitAssignRawDevice(
    __in
    PWDFDEVICE_INIT DeviceInit,
    __in
    CONST GUID* DeviceClassGuid
    )
{
    return ((PFN_WDFPDOINITASSIGNRAWDEVICE) WdfFunctions[WdfPdoInitAssignRawDeviceTableIndex])(WdfDriverGlobals, DeviceInit, DeviceClassGuid);
}

//
// WDF Function: WdfPdoMarkMissing
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFPDOMARKMISSING)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfPdoMarkMissing(
    __in
    WDFDEVICE Device
    )
{
    return ((PFN_WDFPDOMARKMISSING) WdfFunctions[WdfPdoMarkMissingTableIndex])(WdfDriverGlobals, Device);
}

//
// WDF Function: WdfPdoRequestEject
//
typedef
WDFAPI
VOID
(*PFN_WDFPDOREQUESTEJECT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device
    );

VOID
FORCEINLINE
WdfPdoRequestEject(
    __in
    WDFDEVICE Device
    )
{
    ((PFN_WDFPDOREQUESTEJECT) WdfFunctions[WdfPdoRequestEjectTableIndex])(WdfDriverGlobals, Device);
}

//
// WDF Function: WdfPdoGetParent
//
typedef
WDFAPI
WDFDEVICE
(*PFN_WDFPDOGETPARENT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device
    );

WDFDEVICE
FORCEINLINE
WdfPdoGetParent(
    __in
    WDFDEVICE Device
    )
{
    return ((PFN_WDFPDOGETPARENT) WdfFunctions[WdfPdoGetParentTableIndex])(WdfDriverGlobals, Device);
}

//
// WDF Function: WdfPdoRetrieveIdentificationDescription
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFPDORETRIEVEIDENTIFICATIONDESCRIPTION)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER IdentificationDescription
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfPdoRetrieveIdentificationDescription(
    __in
    WDFDEVICE Device,
    __inout
    PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER IdentificationDescription
    )
{
    return ((PFN_WDFPDORETRIEVEIDENTIFICATIONDESCRIPTION) WdfFunctions[WdfPdoRetrieveIdentificationDescriptionTableIndex])(WdfDriverGlobals, Device, IdentificationDescription);
}

//
// WDF Function: WdfPdoRetrieveAddressDescription
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFPDORETRIEVEADDRESSDESCRIPTION)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PWDF_CHILD_ADDRESS_DESCRIPTION_HEADER AddressDescription
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfPdoRetrieveAddressDescription(
    __in
    WDFDEVICE Device,
    __inout
    PWDF_CHILD_ADDRESS_DESCRIPTION_HEADER AddressDescription
    )
{
    return ((PFN_WDFPDORETRIEVEADDRESSDESCRIPTION) WdfFunctions[WdfPdoRetrieveAddressDescriptionTableIndex])(WdfDriverGlobals, Device, AddressDescription);
}

//
// WDF Function: WdfPdoUpdateAddressDescription
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFPDOUPDATEADDRESSDESCRIPTION)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PWDF_CHILD_ADDRESS_DESCRIPTION_HEADER AddressDescription
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfPdoUpdateAddressDescription(
    __in
    WDFDEVICE Device,
    __inout
    PWDF_CHILD_ADDRESS_DESCRIPTION_HEADER AddressDescription
    )
{
    return ((PFN_WDFPDOUPDATEADDRESSDESCRIPTION) WdfFunctions[WdfPdoUpdateAddressDescriptionTableIndex])(WdfDriverGlobals, Device, AddressDescription);
}

//
// WDF Function: WdfPdoAddEjectionRelationsPhysicalDevice
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFPDOADDEJECTIONRELATIONSPHYSICALDEVICE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PDEVICE_OBJECT PhysicalDevice
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfPdoAddEjectionRelationsPhysicalDevice(
    __in
    WDFDEVICE Device,
    __in
    PDEVICE_OBJECT PhysicalDevice
    )
{
    return ((PFN_WDFPDOADDEJECTIONRELATIONSPHYSICALDEVICE) WdfFunctions[WdfPdoAddEjectionRelationsPhysicalDeviceTableIndex])(WdfDriverGlobals, Device, PhysicalDevice);
}

//
// WDF Function: WdfPdoRemoveEjectionRelationsPhysicalDevice
//
typedef
WDFAPI
VOID
(*PFN_WDFPDOREMOVEEJECTIONRELATIONSPHYSICALDEVICE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PDEVICE_OBJECT PhysicalDevice
    );

VOID
FORCEINLINE
WdfPdoRemoveEjectionRelationsPhysicalDevice(
    __in
    WDFDEVICE Device,
    __in
    PDEVICE_OBJECT PhysicalDevice
    )
{
    ((PFN_WDFPDOREMOVEEJECTIONRELATIONSPHYSICALDEVICE) WdfFunctions[WdfPdoRemoveEjectionRelationsPhysicalDeviceTableIndex])(WdfDriverGlobals, Device, PhysicalDevice);
}

//
// WDF Function: WdfPdoClearEjectionRelationsDevices
//
typedef
WDFAPI
VOID
(*PFN_WDFPDOCLEAREJECTIONRELATIONSDEVICES)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device
    );

VOID
FORCEINLINE
WdfPdoClearEjectionRelationsDevices(
    __in
    WDFDEVICE Device
    )
{
    ((PFN_WDFPDOCLEAREJECTIONRELATIONSDEVICES) WdfFunctions[WdfPdoClearEjectionRelationsDevicesTableIndex])(WdfDriverGlobals, Device);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFPDO_H_

