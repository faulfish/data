/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    wdfdpc.h

Abstract:

    This is the C header for driver frameworks DPC object

Revision History:


--*/

#ifndef _WDFDPC_H_
#define _WDFDPC_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)



//
// This is the function that gets called back into the driver
// when the DPC fires.
//
typedef
VOID
(*PFN_WDF_DPC) (
    IN WDFDPC  Dpc
    );

typedef struct _WDF_DPC_CONFIG {
    ULONG       Size;
    PFN_WDF_DPC EvtDpcFunc;

    //
    // If this is TRUE, the DPC will automatically serialize
    // with the event callback handlers of its Parent Object.
    //
    // Parent Object's callback constraints should be compatible
    // with the DPC (DISPATCH_LEVEL), or the request will fail.
    //
    BOOLEAN     AutomaticSerialization;

} WDF_DPC_CONFIG, *PWDF_DPC_CONFIG;

VOID
FORCEINLINE
WDF_DPC_CONFIG_INIT(
    IN PWDF_DPC_CONFIG Config,
    IN PFN_WDF_DPC     EvtDpcFunc
    )
{
    RtlZeroMemory(Config, sizeof(WDF_DPC_CONFIG));
    Config->Size = sizeof(WDF_DPC_CONFIG);
    Config->EvtDpcFunc = EvtDpcFunc;

    Config->AutomaticSerialization = TRUE;
}

//
// WDF Function: WdfDpcCreate
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDPCCREATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDF_DPC_CONFIG Config,
    PWDF_OBJECT_ATTRIBUTES Attributes,
    WDFDPC* Dpc
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDpcCreate(
    __in
    PWDF_DPC_CONFIG Config,
    __in
    PWDF_OBJECT_ATTRIBUTES Attributes,
    __out
    WDFDPC* Dpc
    )
{
    return ((PFN_WDFDPCCREATE) WdfFunctions[WdfDpcCreateTableIndex])(WdfDriverGlobals, Config, Attributes, Dpc);
}

//
// WDF Function: WdfDpcEnqueue
//
typedef
WDFAPI
BOOLEAN
(*PFN_WDFDPCENQUEUE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDPC Dpc
    );

BOOLEAN
FORCEINLINE
WdfDpcEnqueue(
    __in
    WDFDPC Dpc
    )
{
    return ((PFN_WDFDPCENQUEUE) WdfFunctions[WdfDpcEnqueueTableIndex])(WdfDriverGlobals, Dpc);
}

//
// WDF Function: WdfDpcCancel
//
typedef
WDFAPI
BOOLEAN
(*PFN_WDFDPCCANCEL)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDPC Dpc,
    IN BOOLEAN Wait
    );

BOOLEAN
FORCEINLINE
WdfDpcCancel(
    __in
    WDFDPC Dpc,
    IN BOOLEAN Wait
    )
{
    return ((PFN_WDFDPCCANCEL) WdfFunctions[WdfDpcCancelTableIndex])(WdfDriverGlobals, Dpc, Wait);
}

//
// WDF Function: WdfDpcGetParentObject
//
typedef
WDFAPI
WDFOBJECT
(*PFN_WDFDPCGETPARENTOBJECT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDPC Dpc
    );

WDFOBJECT
FORCEINLINE
WdfDpcGetParentObject(
    __in
    WDFDPC Dpc
    )
{
    return ((PFN_WDFDPCGETPARENTOBJECT) WdfFunctions[WdfDpcGetParentObjectTableIndex])(WdfDriverGlobals, Dpc);
}

//
// WDF Function: WdfDpcWdmGetDpc
//
typedef
WDFAPI
PKDPC
(*PFN_WDFDPCWDMGETDPC)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDPC Dpc
    );

PKDPC
FORCEINLINE
WdfDpcWdmGetDpc(
    __in
    WDFDPC Dpc
    )
{
    return ((PFN_WDFDPCWDMGETDPC) WdfFunctions[WdfDpcWdmGetDpcTableIndex])(WdfDriverGlobals, Dpc);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFDPC_H_

