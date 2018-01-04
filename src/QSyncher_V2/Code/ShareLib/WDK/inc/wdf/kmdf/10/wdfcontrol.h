/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    wdfcontrol.h

Abstract:

    Defines functions for controller and creating a "controller" NT4 style
    WDFDEVICE handle.

Environment:

    kernel mode only

Revision History:

--*/

#ifndef _WDFCONTROL_H_
#define _WDFCONTROL_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)

typedef
VOID
(*PFN_WDF_DEVICE_SHUTDOWN_NOTIFICATION)(
    WDFDEVICE Device
    );

typedef enum _WDF_DEVICE_SHUTDOWN_FLAGS {
    WdfDeviceShutdown = 0x01,
    WdfDeviceLastChanceShutdown = 0x02,
} WDF_DEVICE_SHUTDOWN_FLAGS;



//
// WDF Function: WdfControlDeviceInitAllocate
//
typedef
WDFAPI
PWDFDEVICE_INIT
(*PFN_WDFCONTROLDEVICEINITALLOCATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDRIVER Driver,
    CONST UNICODE_STRING* SDDLString
    );

PWDFDEVICE_INIT
FORCEINLINE
WdfControlDeviceInitAllocate(
    __in
    WDFDRIVER Driver,
    __in
    CONST UNICODE_STRING* SDDLString
    )
{
    return ((PFN_WDFCONTROLDEVICEINITALLOCATE) WdfFunctions[WdfControlDeviceInitAllocateTableIndex])(WdfDriverGlobals, Driver, SDDLString);
}

//
// WDF Function: WdfControlDeviceInitSetShutdownNotification
//
typedef
WDFAPI
VOID
(*PFN_WDFCONTROLDEVICEINITSETSHUTDOWNNOTIFICATION)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    PFN_WDF_DEVICE_SHUTDOWN_NOTIFICATION Notification,
    IN UCHAR Flags
    );

VOID
FORCEINLINE
WdfControlDeviceInitSetShutdownNotification(
    __in
    PWDFDEVICE_INIT DeviceInit,
    __in
    PFN_WDF_DEVICE_SHUTDOWN_NOTIFICATION Notification,
    IN UCHAR Flags
    )
{
    ((PFN_WDFCONTROLDEVICEINITSETSHUTDOWNNOTIFICATION) WdfFunctions[WdfControlDeviceInitSetShutdownNotificationTableIndex])(WdfDriverGlobals, DeviceInit, Notification, Flags);
}

//
// WDF Function: WdfControlFinishInitializing
//
typedef
WDFAPI
VOID
(*PFN_WDFCONTROLFINISHINITIALIZING)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device
    );

VOID
FORCEINLINE
WdfControlFinishInitializing(
    __in
    WDFDEVICE Device
    )
{
    ((PFN_WDFCONTROLFINISHINITIALIZING) WdfFunctions[WdfControlFinishInitializingTableIndex])(WdfDriverGlobals, Device);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFCONTROL_H_

