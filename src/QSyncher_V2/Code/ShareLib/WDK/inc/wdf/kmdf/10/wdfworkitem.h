/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    wdfworkitem.h

Abstract:

    This is the Windows Driver Framework work item DDIs

Revision History:


--*/

#ifndef _WDFWORKITEM_H_
#define _WDFWORKITEM_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)



//
// This is the function that gets called back into the driver
// when the WorkItem fires.
//
typedef
VOID
(*PFN_WDF_WORKITEM) (
    IN WDFWORKITEM WorkItem
    );

typedef struct _WDF_WORKITEM_CONFIG {

    ULONG            Size;

    PFN_WDF_WORKITEM EvtWorkItemFunc;

    //
    // If this is TRUE, the workitem will automatically serialize
    // with the event callback handlers of its Parent Object.
    //
    // Parent Object's callback constraints should be compatible
    // with the work item (PASSIVE_LEVEL), or the request will fail.
    //
    BOOLEAN       AutomaticSerialization;

} WDF_WORKITEM_CONFIG, *PWDF_WORKITEM_CONFIG;


VOID
FORCEINLINE
WDF_WORKITEM_CONFIG_INIT(
    OUT PWDF_WORKITEM_CONFIG Config,
    IN PFN_WDF_WORKITEM     EvtWorkItemFunc
    )
{
    RtlZeroMemory(Config, sizeof(WDF_WORKITEM_CONFIG));
    Config->Size = sizeof(WDF_WORKITEM_CONFIG);
    Config->EvtWorkItemFunc = EvtWorkItemFunc;

    Config->AutomaticSerialization = TRUE;
}


//
// WDF Function: WdfWorkItemCreate
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFWORKITEMCREATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDF_WORKITEM_CONFIG Config,
    PWDF_OBJECT_ATTRIBUTES Attributes,
    WDFWORKITEM* WorkItem
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfWorkItemCreate(
    __in
    PWDF_WORKITEM_CONFIG Config,
    __in
    PWDF_OBJECT_ATTRIBUTES Attributes,
    __out
    WDFWORKITEM* WorkItem
    )
{
    return ((PFN_WDFWORKITEMCREATE) WdfFunctions[WdfWorkItemCreateTableIndex])(WdfDriverGlobals, Config, Attributes, WorkItem);
}

//
// WDF Function: WdfWorkItemEnqueue
//
typedef
WDFAPI
VOID
(*PFN_WDFWORKITEMENQUEUE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFWORKITEM WorkItem
    );

VOID
FORCEINLINE
WdfWorkItemEnqueue(
    __in
    WDFWORKITEM WorkItem
    )
{
    ((PFN_WDFWORKITEMENQUEUE) WdfFunctions[WdfWorkItemEnqueueTableIndex])(WdfDriverGlobals, WorkItem);
}

//
// WDF Function: WdfWorkItemGetParentObject
//
typedef
WDFAPI
WDFOBJECT
(*PFN_WDFWORKITEMGETPARENTOBJECT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFWORKITEM WorkItem
    );

WDFOBJECT
FORCEINLINE
WdfWorkItemGetParentObject(
    __in
    WDFWORKITEM WorkItem
    )
{
    return ((PFN_WDFWORKITEMGETPARENTOBJECT) WdfFunctions[WdfWorkItemGetParentObjectTableIndex])(WdfDriverGlobals, WorkItem);
}

//
// WDF Function: WdfWorkItemFlush
//
typedef
WDFAPI
VOID
(*PFN_WDFWORKITEMFLUSH)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFWORKITEM WorkItem
    );

VOID
FORCEINLINE
WdfWorkItemFlush(
    __in
    WDFWORKITEM WorkItem
    )
{
    ((PFN_WDFWORKITEMFLUSH) WdfFunctions[WdfWorkItemFlushTableIndex])(WdfDriverGlobals, WorkItem);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFWORKITEM_H_

