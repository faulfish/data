/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    wdfdriver.h

Abstract:

    This is the interfaces for the Windows Driver Framework Driver object

Environment:

    kernel mode only

Revision History:

--*/

#ifndef _WDFDRIVER_H_
#define _WDFDRIVER_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)

typedef enum _WDF_DRIVER_INIT_FLAGS {
    WdfDriverInitNonPnpDriver = 0x00000001, //  If set, no Add Device routine is assigned.
    WdfDriverInitNoDispatchOverride = 0x00000002, //  Useful for miniports.
    WdfVerifyOn = 0x00000004, //  Controls whether WDFVERIFY macros are live.
    WdfVerifierOn = 0x00000008, //  Top level verififer flag.
} WDF_DRIVER_INIT_FLAGS;



#define WDF_TRACE_ID ('TRAC')

//
// Callbacks for FxDriver
//

typedef
NTSTATUS
(*PFN_WDF_DRIVER_DEVICE_ADD)(
    IN WDFDRIVER Driver,
    IN PWDFDEVICE_INIT DeviceInit
    );

typedef
VOID
(*PFN_WDF_DRIVER_UNLOAD)(
    IN WDFDRIVER Driver
    );


//
// Used by WPP Tracing (modeled after WPP's WppTraceCallback (in km-init.tpl))
//
typedef
NTSTATUS
(*PFN_WDF_TRACE_CALLBACK)(
    IN  UCHAR   minorFunction,
    IN  PVOID   dataPath,
    IN  ULONG   bufferLength,
    IN  PVOID   buffer,
    IN  PVOID   context,
    OUT PULONG  size
    ) ;

typedef struct _WDF_DRIVER_CONFIG {
    //
    // Size of this structure in bytes
    //
    ULONG Size;

    //
    // Event callbacks
    //
    PFN_WDF_DRIVER_DEVICE_ADD EvtDriverDeviceAdd;

    PFN_WDF_DRIVER_UNLOAD    EvtDriverUnload;

    //
    // Combination of WDF_DRIVER_INIT_FLAGS values
    //
    ULONG DriverInitFlags;

    //
    // Pool tag to use for all allocations made by the framework on behalf of
    // the client driver.
    //
    ULONG DriverPoolTag;

} WDF_DRIVER_CONFIG, *PWDF_DRIVER_CONFIG;

VOID
FORCEINLINE
WDF_DRIVER_CONFIG_INIT(
    OUT PWDF_DRIVER_CONFIG Config,
    IN PFN_WDF_DRIVER_DEVICE_ADD EvtDriverDeviceAdd
    )
{
    RtlZeroMemory(Config, sizeof(WDF_DRIVER_CONFIG));

    Config->Size = sizeof(WDF_DRIVER_CONFIG);
    Config->EvtDriverDeviceAdd = EvtDriverDeviceAdd;
}

typedef struct _WDF_DRIVER_VERSION_AVAILABLE_PARAMS {
    //
    // Size of the structure in bytes
    //
    ULONG Size;

    //
    // Major Version requested
    //
    ULONG MajorVersion;

    //
    // Minor Version requested
    //
    ULONG MinorVersion;

} WDF_DRIVER_VERSION_AVAILABLE_PARAMS, *PWDF_DRIVER_VERSION_AVAILABLE_PARAMS;

VOID
FORCEINLINE
WDF_DRIVER_VERSION_AVAILABLE_PARAMS_INIT(
    PWDF_DRIVER_VERSION_AVAILABLE_PARAMS Params,
    ULONG MajorVersion,
    ULONG MinorVersion
    )
{
    RtlZeroMemory(Params, sizeof(WDF_DRIVER_VERSION_AVAILABLE_PARAMS));

    Params->Size = sizeof(WDF_DRIVER_VERSION_AVAILABLE_PARAMS);
    Params->MajorVersion = MajorVersion;
    Params->MinorVersion = MinorVersion;
}

WDFDRIVER
FORCEINLINE
WdfGetDriver(
    VOID
    )
{
    return WdfDriverGlobals->Driver;
}

//
// WDF Function: WdfDriverCreate
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDRIVERCREATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PDRIVER_OBJECT DriverObject,
    PCUNICODE_STRING RegistryPath,
    PWDF_OBJECT_ATTRIBUTES DriverAttributes,
    PWDF_DRIVER_CONFIG DriverConfig,
    WDFDRIVER* Driver
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDriverCreate(
    __in
    PDRIVER_OBJECT DriverObject,
    __in
    PCUNICODE_STRING RegistryPath,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES DriverAttributes,
    __in
    PWDF_DRIVER_CONFIG DriverConfig,
    __out_opt
    WDFDRIVER* Driver
    )
{
    return ((PFN_WDFDRIVERCREATE) WdfFunctions[WdfDriverCreateTableIndex])(WdfDriverGlobals, DriverObject, RegistryPath, DriverAttributes, DriverConfig, Driver);
}

//
// WDF Function: WdfDriverGetRegistryPath
//
typedef
WDFAPI
PWSTR
(*PFN_WDFDRIVERGETREGISTRYPATH)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDRIVER Driver
    );

PWSTR
FORCEINLINE
WdfDriverGetRegistryPath(
    __in
    WDFDRIVER Driver
    )
{
    return ((PFN_WDFDRIVERGETREGISTRYPATH) WdfFunctions[WdfDriverGetRegistryPathTableIndex])(WdfDriverGlobals, Driver);
}

//
// WDF Function: WdfDriverWdmGetDriverObject
//
typedef
WDFAPI
PDRIVER_OBJECT
(*PFN_WDFDRIVERWDMGETDRIVEROBJECT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDRIVER Driver
    );

PDRIVER_OBJECT
FORCEINLINE
WdfDriverWdmGetDriverObject(
    __in
    WDFDRIVER Driver
    )
{
    return ((PFN_WDFDRIVERWDMGETDRIVEROBJECT) WdfFunctions[WdfDriverWdmGetDriverObjectTableIndex])(WdfDriverGlobals, Driver);
}

//
// WDF Function: WdfDriverOpenParametersRegistryKey
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDRIVEROPENPARAMETERSREGISTRYKEY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDRIVER Driver,
    IN ACCESS_MASK DesiredAccess,
    PWDF_OBJECT_ATTRIBUTES KeyAttributes,
    WDFKEY* Key
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDriverOpenParametersRegistryKey(
    __in
    WDFDRIVER Driver,
    IN ACCESS_MASK DesiredAccess,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES KeyAttributes,
    __out
    WDFKEY* Key
    )
{
    return ((PFN_WDFDRIVEROPENPARAMETERSREGISTRYKEY) WdfFunctions[WdfDriverOpenParametersRegistryKeyTableIndex])(WdfDriverGlobals, Driver, DesiredAccess, KeyAttributes, Key);
}

//
// WDF Function: WdfWdmDriverGetWdfDriverHandle
//
typedef
WDFAPI
WDFDRIVER
(*PFN_WDFWDMDRIVERGETWDFDRIVERHANDLE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PDRIVER_OBJECT DriverObject
    );

WDFDRIVER
FORCEINLINE
WdfWdmDriverGetWdfDriverHandle(
    __in
    PDRIVER_OBJECT DriverObject
    )
{
    return ((PFN_WDFWDMDRIVERGETWDFDRIVERHANDLE) WdfFunctions[WdfWdmDriverGetWdfDriverHandleTableIndex])(WdfDriverGlobals, DriverObject);
}

//
// WDF Function: WdfDriverRegisterTraceInfo
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDRIVERREGISTERTRACEINFO)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PDRIVER_OBJECT DriverObject,
    PFN_WDF_TRACE_CALLBACK EvtTraceCallback,
    PVOID ControlBlock
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDriverRegisterTraceInfo(
    __in
    PDRIVER_OBJECT DriverObject,
    __in
    PFN_WDF_TRACE_CALLBACK EvtTraceCallback,
    __in
    PVOID ControlBlock
    )
{
    return ((PFN_WDFDRIVERREGISTERTRACEINFO) WdfFunctions[WdfDriverRegisterTraceInfoTableIndex])(WdfDriverGlobals, DriverObject, EvtTraceCallback, ControlBlock);
}

//
// WDF Function: WdfDriverRetrieveVersionString
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDRIVERRETRIEVEVERSIONSTRING)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDRIVER Driver,
    WDFSTRING String
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDriverRetrieveVersionString(
    __in
    WDFDRIVER Driver,
    __in
    WDFSTRING String
    )
{
    return ((PFN_WDFDRIVERRETRIEVEVERSIONSTRING) WdfFunctions[WdfDriverRetrieveVersionStringTableIndex])(WdfDriverGlobals, Driver, String);
}

//
// WDF Function: WdfDriverIsVersionAvailable
//
typedef
WDFAPI
BOOLEAN
(*PFN_WDFDRIVERISVERSIONAVAILABLE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDRIVER Driver,
    PWDF_DRIVER_VERSION_AVAILABLE_PARAMS VersionAvailableParams
    );

__checkReturn
BOOLEAN
FORCEINLINE
WdfDriverIsVersionAvailable(
    __in
    WDFDRIVER Driver,
    __in
    PWDF_DRIVER_VERSION_AVAILABLE_PARAMS VersionAvailableParams
    )
{
    return ((PFN_WDFDRIVERISVERSIONAVAILABLE) WdfFunctions[WdfDriverIsVersionAvailableTableIndex])(WdfDriverGlobals, Driver, VersionAvailableParams);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFDRIVER_H_

