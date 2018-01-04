/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    wdfrequest.h

Abstract:

    This module contains contains the Windows Driver Framework Request object
    interfaces.

Environment:

    kernel mode only

Revision History:


--*/

#ifndef _WDFREQUEST_H_
#define _WDFREQUEST_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)

//
// Types
//

typedef enum _WDF_REQUEST_TYPE {
    WdfRequestTypeCreate = 0x0,
    WdfRequestTypeCreateNamedPipe = 0x1,
    WdfRequestTypeClose = 0x2,
    WdfRequestTypeRead = 0x3,
    WdfRequestTypeWrite = 0x4,
    WdfRequestTypeQueryInformation = 0x5,
    WdfRequestTypeSetInformation = 0x6,
    WdfRequestTypeQueryEA = 0x7,
    WdfRequestTypeSetEA = 0x8,
    WdfRequestTypeFlushBuffers = 0x9,
    WdfRequestTypeQueryVolumeInformation = 0xa,
    WdfRequestTypeSetVolumeInformation = 0xb,
    WdfRequestTypeDirectoryControl = 0xc,
    WdfRequestTypeFileSystemControl = 0xd,
    WdfRequestTypeDeviceControl = 0xe,
    WdfRequestTypeDeviceControlInternal = 0xf,
    WdfRequestTypeShutdown = 0x10,
    WdfRequestTypeLockControl = 0x11,
    WdfRequestTypeCleanup = 0x12,
    WdfRequestTypeCreateMailSlot = 0x13,
    WdfRequestTypeQuerySecurity = 0x14,
    WdfRequestTypeSetSecurity = 0x15,
    WdfRequestTypePower = 0x16,
    WdfRequestTypeSystemControl = 0x17,
    WdfRequestTypeDeviceChange = 0x18,
    WdfRequestTypeQueryQuota = 0x19,
    WdfRequestTypeSetQuota = 0x1A,
    WdfRequestTypePnp = 0x1B,
    WdfRequestTypeOther =0x1C,
    WdfRequestTypeUsb = 0x40,
    WdfRequestTypeNoFormat = 0xFF,
    WdfRequestTypeMax,
} WDF_REQUEST_TYPE;

typedef enum _WDF_REQUEST_REUSE_FLAGS {
    WDF_REQUEST_REUSE_NO_FLAGS = 0x00000000,
    WDF_REQUEST_REUSE_SET_NEW_IRP = 0x00000001,
} WDF_REQUEST_REUSE_FLAGS;

// 
// This defines the actions to take on a request
// in EvtIoStop.
// 
typedef enum _WDF_REQUEST_STOP_ACTION_FLAGS {
    WdfRequestStopActionInvalid = 0,
    WdfRequestStopActionSuspend = 0x01, //  Device is being suspended
    WdfRequestStopActionPurge = 0x2, //  Device/queue is being removed
    WdfRequestStopRequestCancelable = 0x10000000, //  This bit is set if the request is marked cancelable
} WDF_REQUEST_STOP_ACTION_FLAGS;

typedef enum _WDF_REQUEST_SEND_OPTIONS_FLAGS {
    WDF_REQUEST_SEND_OPTION_TIMEOUT = 0x0000001,
    WDF_REQUEST_SEND_OPTION_SYNCHRONOUS = 0x0000002,
    WDF_REQUEST_SEND_OPTION_IGNORE_TARGET_STATE = 0x0000004,
    WDF_REQUEST_SEND_OPTION_SEND_AND_FORGET = 0x0000008,
} WDF_REQUEST_SEND_OPTIONS_FLAGS;



// Request cancel is called if a request that has been marked cancelable is cancelled
typedef
VOID
(*PFN_WDF_REQUEST_CANCEL) (
    IN WDFREQUEST Request
    );


//
// This parameters structure allows general access to a requests parameters
//
typedef struct _WDF_REQUEST_PARAMETERS {

    USHORT Size;

    UCHAR MinorFunction;

    WDF_REQUEST_TYPE Type;

    //
    // The following user parameters are based on the service that is being
    // invoked.  Drivers and file systems can determine which set to use based
    // on the above major and minor function codes.
    //
    union {

        //
        // System service parameters for:  Create
        //

        struct {
            PIO_SECURITY_CONTEXT SecurityContext;
            ULONG Options;
            USHORT POINTER_ALIGNMENT FileAttributes;
            USHORT ShareAccess;
            ULONG POINTER_ALIGNMENT EaLength;
        } Create;


        //
        // System service parameters for:  Read
        //

        struct {
            size_t Length;
            ULONG POINTER_ALIGNMENT Key;
            LONGLONG DeviceOffset;
        } Read;

        //
        // System service parameters for:  Write
        //

        struct {
            size_t Length;
            ULONG POINTER_ALIGNMENT Key;
            LONGLONG DeviceOffset;
        } Write;

        //
        // System service parameters for:  Device Control
        //
        // Note that the user's output buffer is stored in the UserBuffer field
        // and the user's input buffer is stored in the SystemBuffer field.
        //

        struct {
            size_t OutputBufferLength;
            size_t POINTER_ALIGNMENT InputBufferLength;
            ULONG POINTER_ALIGNMENT IoControlCode;
            PVOID Type3InputBuffer;
        } DeviceIoControl;

        struct {
            PVOID Arg1;
            PVOID  Arg2;
            ULONG POINTER_ALIGNMENT IoControlCode;
            PVOID Arg4;
        } Others;

    } Parameters;

} WDF_REQUEST_PARAMETERS, *PWDF_REQUEST_PARAMETERS;

VOID
FORCEINLINE
WDF_REQUEST_PARAMETERS_INIT(
    OUT PWDF_REQUEST_PARAMETERS Parameters
    )
{
    RtlZeroMemory(Parameters, sizeof(WDF_REQUEST_PARAMETERS));

    Parameters->Size = sizeof(WDF_REQUEST_PARAMETERS);
}

typedef struct _WDF_USB_REQUEST_COMPLETION_PARAMS *PWDF_USB_REQUEST_COMPLETION_PARAMS;

typedef struct _WDF_REQUEST_COMPLETION_PARAMS {
    //
    // Size of the structure in bytes
    //
    ULONG Size;

    WDF_REQUEST_TYPE Type;

    IO_STATUS_BLOCK IoStatus;

    union {
        struct {
            WDFMEMORY Buffer;
            size_t Length;
            size_t Offset;
        } Write;

        struct {
            WDFMEMORY Buffer;
            size_t Length;
            size_t Offset;
        } Read;

        struct {
            ULONG IoControlCode;

            struct {
                WDFMEMORY Buffer;
                size_t Offset;
            } Input;

            struct {
                WDFMEMORY Buffer;
                size_t Offset;
                size_t Length;
            } Output;
        } Ioctl;

        struct {
            union {
                PVOID Ptr;
                ULONG_PTR Value;
            } Argument1;
            union {
                PVOID Ptr;
                ULONG_PTR Value;
            } Argument2;
            union {
                PVOID Ptr;
                ULONG_PTR Value;
            } Argument3;
            union {
                PVOID Ptr;
                ULONG_PTR Value;
            } Argument4;
        } Others;

        struct {
            PWDF_USB_REQUEST_COMPLETION_PARAMS Completion;
        } Usb;
    } Parameters;

} WDF_REQUEST_COMPLETION_PARAMS, *PWDF_REQUEST_COMPLETION_PARAMS;

VOID
FORCEINLINE
WDF_REQUEST_COMPLETION_PARAMS_INIT(
    PWDF_REQUEST_COMPLETION_PARAMS Params
    )
{
    RtlZeroMemory(Params, sizeof(WDF_REQUEST_COMPLETION_PARAMS));
    Params->Size = sizeof(WDF_REQUEST_COMPLETION_PARAMS);
    Params->Type = WdfRequestTypeNoFormat;
}

typedef
VOID
(*PFN_WDF_REQUEST_COMPLETION_ROUTINE)(
    IN WDFREQUEST Request,
    IN WDFIOTARGET Target,
    IN PWDF_REQUEST_COMPLETION_PARAMS Params,
    IN WDFCONTEXT Context
    );

/*++

Routine Description:
    Clears out the internal state of the irp, which includes, but is not limited
    to:
    a)  Any internal allocations for the previously formatted request
    b)  The completion routine and its context
    c)  The request's intended i/o target
    d)  All of the internal IRP's stack locations

Arguments:
    Request - The request to be reused.

    ReuseParams - Parameters controlling the reuse of the request, see comments
        for each field in the structure for usage

Return Value:
    None

  --*/

typedef struct _WDF_REQUEST_REUSE_PARAMS {
    //
    // Size of this structure in bytes
    //
    ULONG Size;

    //
    // Bit field combination of WDF_REQUEST_REUSE_Xxx values
    //
    ULONG Flags;

    //
    // The new status of the request.
    //
    NTSTATUS Status;

    //
    // New PIRP  to be contained in the WDFREQUEST.   Setting a new PIRP value
    // is only valid for WDFREQUESTs created by WdfRequestCreateFromIrp where
    // RequestFreesIrp == FALSE.  No other WDFREQUESTs (presented by the
    // I/O queue for instance) may have their IRPs changed.
    //
    PIRP NewIrp;

} WDF_REQUEST_REUSE_PARAMS, *PWDF_REQUEST_REUSE_PARAMS;

VOID
FORCEINLINE
WDF_REQUEST_REUSE_PARAMS_INIT(
    OUT PWDF_REQUEST_REUSE_PARAMS Params,
    IN ULONG Flags,
    IN NTSTATUS Status
    )
{
    RtlZeroMemory(Params, sizeof(WDF_REQUEST_REUSE_PARAMS));

    Params->Size = sizeof(WDF_REQUEST_REUSE_PARAMS);
    Params->Flags = Flags;
    Params->Status = Status;
}

VOID
FORCEINLINE
WDF_REQUEST_REUSE_PARAMS_SET_NEW_IRP(
    OUT PWDF_REQUEST_REUSE_PARAMS Params,
    IN PIRP NewIrp
    )
{
    Params->Flags |= WDF_REQUEST_REUSE_SET_NEW_IRP;
    Params->NewIrp = NewIrp;
}

typedef struct _WDF_REQUEST_SEND_OPTIONS {
    //
    // Size of the structure in bytes
    //
    ULONG Size;

    //
    // Bit field combination of values from the WDF_REQUEST_SEND_OPTIONS_FLAGS
    // enumeration
    //
    ULONG Flags;

    //
    // Valid when WDF_REQUEST_SEND_OPTION_TIMEOUT is set
    //
    LONGLONG Timeout;

} WDF_REQUEST_SEND_OPTIONS, *PWDF_REQUEST_SEND_OPTIONS;

VOID
FORCEINLINE
WDF_REQUEST_SEND_OPTIONS_INIT(
    OUT PWDF_REQUEST_SEND_OPTIONS Options,
    IN ULONG Flags
    )
{
    RtlZeroMemory(Options, sizeof(WDF_REQUEST_SEND_OPTIONS));
    Options->Size = sizeof(WDF_REQUEST_SEND_OPTIONS);
    Options->Flags = Flags;
}

VOID
FORCEINLINE
WDF_REQUEST_SEND_OPTIONS_SET_TIMEOUT(
    OUT PWDF_REQUEST_SEND_OPTIONS Options,
    IN LONGLONG Timeout
    )
{
    Options->Flags |= WDF_REQUEST_SEND_OPTION_TIMEOUT;
    Options->Timeout = Timeout;
}

//
// WDF Function: WdfRequestCreate
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREQUESTCREATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDF_OBJECT_ATTRIBUTES RequestAttributes,
    WDFIOTARGET IoTarget,
    WDFREQUEST* Request
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRequestCreate(
    __in_opt
    PWDF_OBJECT_ATTRIBUTES RequestAttributes,
    __in_opt
    WDFIOTARGET IoTarget,
    __out
    WDFREQUEST* Request
    )
{
    return ((PFN_WDFREQUESTCREATE) WdfFunctions[WdfRequestCreateTableIndex])(WdfDriverGlobals, RequestAttributes, IoTarget, Request);
}

//
// WDF Function: WdfRequestCreateFromIrp
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREQUESTCREATEFROMIRP)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDF_OBJECT_ATTRIBUTES RequestAttributes,
    PIRP Irp,
    IN BOOLEAN RequestFreesIrp,
    WDFREQUEST* Request
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRequestCreateFromIrp(
    __in_opt
    PWDF_OBJECT_ATTRIBUTES RequestAttributes,
    __in
    PIRP Irp,
    IN BOOLEAN RequestFreesIrp,
    __out
    WDFREQUEST* Request
    )
{
    return ((PFN_WDFREQUESTCREATEFROMIRP) WdfFunctions[WdfRequestCreateFromIrpTableIndex])(WdfDriverGlobals, RequestAttributes, Irp, RequestFreesIrp, Request);
}

//
// WDF Function: WdfRequestReuse
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREQUESTREUSE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    PWDF_REQUEST_REUSE_PARAMS ReuseParams
    );

NTSTATUS
FORCEINLINE
WdfRequestReuse(
    __in
    WDFREQUEST Request,
    __in
    PWDF_REQUEST_REUSE_PARAMS ReuseParams
    )
{
    return ((PFN_WDFREQUESTREUSE) WdfFunctions[WdfRequestReuseTableIndex])(WdfDriverGlobals, Request, ReuseParams);
}

//
// WDF Function: WdfRequestChangeTarget
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREQUESTCHANGETARGET)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    WDFIOTARGET IoTarget
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRequestChangeTarget(
    __in
    WDFREQUEST Request,
    __in
    WDFIOTARGET IoTarget
    )
{
    return ((PFN_WDFREQUESTCHANGETARGET) WdfFunctions[WdfRequestChangeTargetTableIndex])(WdfDriverGlobals, Request, IoTarget);
}

//
// WDF Function: WdfRequestFormatRequestUsingCurrentType
//
typedef
WDFAPI
VOID
(*PFN_WDFREQUESTFORMATREQUESTUSINGCURRENTTYPE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request
    );

VOID
FORCEINLINE
WdfRequestFormatRequestUsingCurrentType(
    __in
    WDFREQUEST Request
    )
{
    ((PFN_WDFREQUESTFORMATREQUESTUSINGCURRENTTYPE) WdfFunctions[WdfRequestFormatRequestUsingCurrentTypeTableIndex])(WdfDriverGlobals, Request);
}

//
// WDF Function: WdfRequestWdmFormatUsingStackLocation
//
typedef
WDFAPI
VOID
(*PFN_WDFREQUESTWDMFORMATUSINGSTACKLOCATION)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    PIO_STACK_LOCATION Stack
    );

VOID
FORCEINLINE
WdfRequestWdmFormatUsingStackLocation(
    __in
    WDFREQUEST Request,
    __in
    PIO_STACK_LOCATION Stack
    )
{
    ((PFN_WDFREQUESTWDMFORMATUSINGSTACKLOCATION) WdfFunctions[WdfRequestWdmFormatUsingStackLocationTableIndex])(WdfDriverGlobals, Request, Stack);
}

//
// WDF Function: WdfRequestSend
//
typedef
WDFAPI
BOOLEAN
(*PFN_WDFREQUESTSEND)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    WDFIOTARGET Target,
    PWDF_REQUEST_SEND_OPTIONS Options
    );

BOOLEAN
FORCEINLINE
WdfRequestSend(
    __in
    WDFREQUEST Request,
    __in
    WDFIOTARGET Target,
    __in_opt
    PWDF_REQUEST_SEND_OPTIONS Options
    )
{
    return ((PFN_WDFREQUESTSEND) WdfFunctions[WdfRequestSendTableIndex])(WdfDriverGlobals, Request, Target, Options);
}

//
// WDF Function: WdfRequestGetStatus
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREQUESTGETSTATUS)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRequestGetStatus(
    __in
    WDFREQUEST Request
    )
{
    return ((PFN_WDFREQUESTGETSTATUS) WdfFunctions[WdfRequestGetStatusTableIndex])(WdfDriverGlobals, Request);
}

//
// WDF Function: WdfRequestMarkCancelable
//
typedef
WDFAPI
VOID
(*PFN_WDFREQUESTMARKCANCELABLE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    PFN_WDF_REQUEST_CANCEL EvtRequestCancel
    );

VOID
FORCEINLINE
WdfRequestMarkCancelable(
    __in
    WDFREQUEST Request,
    __in
    PFN_WDF_REQUEST_CANCEL EvtRequestCancel
    )
{
    ((PFN_WDFREQUESTMARKCANCELABLE) WdfFunctions[WdfRequestMarkCancelableTableIndex])(WdfDriverGlobals, Request, EvtRequestCancel);
}

//
// WDF Function: WdfRequestUnmarkCancelable
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREQUESTUNMARKCANCELABLE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRequestUnmarkCancelable(
    __in
    WDFREQUEST Request
    )
{
    return ((PFN_WDFREQUESTUNMARKCANCELABLE) WdfFunctions[WdfRequestUnmarkCancelableTableIndex])(WdfDriverGlobals, Request);
}

//
// WDF Function: WdfRequestIsCanceled
//
typedef
WDFAPI
BOOLEAN
(*PFN_WDFREQUESTISCANCELED)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request
    );

__checkReturn
BOOLEAN
FORCEINLINE
WdfRequestIsCanceled(
    __in
    WDFREQUEST Request
    )
{
    return ((PFN_WDFREQUESTISCANCELED) WdfFunctions[WdfRequestIsCanceledTableIndex])(WdfDriverGlobals, Request);
}

//
// WDF Function: WdfRequestCancelSentRequest
//
typedef
WDFAPI
BOOLEAN
(*PFN_WDFREQUESTCANCELSENTREQUEST)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request
    );

BOOLEAN
FORCEINLINE
WdfRequestCancelSentRequest(
    __in
    WDFREQUEST Request
    )
{
    return ((PFN_WDFREQUESTCANCELSENTREQUEST) WdfFunctions[WdfRequestCancelSentRequestTableIndex])(WdfDriverGlobals, Request);
}

//
// WDF Function: WdfRequestIsFrom32BitProcess
//
typedef
WDFAPI
BOOLEAN
(*PFN_WDFREQUESTISFROM32BITPROCESS)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request
    );

__checkReturn
BOOLEAN
FORCEINLINE
WdfRequestIsFrom32BitProcess(
    __in
    WDFREQUEST Request
    )
{
    return ((PFN_WDFREQUESTISFROM32BITPROCESS) WdfFunctions[WdfRequestIsFrom32BitProcessTableIndex])(WdfDriverGlobals, Request);
}

//
// WDF Function: WdfRequestSetCompletionRoutine
//
typedef
WDFAPI
VOID
(*PFN_WDFREQUESTSETCOMPLETIONROUTINE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    PFN_WDF_REQUEST_COMPLETION_ROUTINE CompletionRoutine,
    WDFCONTEXT CompletionContext
    );

VOID
FORCEINLINE
WdfRequestSetCompletionRoutine(
    __in
    WDFREQUEST Request,
    __in_opt
    PFN_WDF_REQUEST_COMPLETION_ROUTINE CompletionRoutine,
    __in_opt
    WDFCONTEXT CompletionContext
    )
{
    ((PFN_WDFREQUESTSETCOMPLETIONROUTINE) WdfFunctions[WdfRequestSetCompletionRoutineTableIndex])(WdfDriverGlobals, Request, CompletionRoutine, CompletionContext);
}

//
// WDF Function: WdfRequestGetCompletionParams
//
typedef
WDFAPI
VOID
(*PFN_WDFREQUESTGETCOMPLETIONPARAMS)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    PWDF_REQUEST_COMPLETION_PARAMS Params
    );

VOID
FORCEINLINE
WdfRequestGetCompletionParams(
    __in
    WDFREQUEST Request,
    __out
    PWDF_REQUEST_COMPLETION_PARAMS Params
    )
{
    ((PFN_WDFREQUESTGETCOMPLETIONPARAMS) WdfFunctions[WdfRequestGetCompletionParamsTableIndex])(WdfDriverGlobals, Request, Params);
}

//
// WDF Function: WdfRequestAllocateTimer
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREQUESTALLOCATETIMER)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRequestAllocateTimer(
    __in
    WDFREQUEST Request
    )
{
    return ((PFN_WDFREQUESTALLOCATETIMER) WdfFunctions[WdfRequestAllocateTimerTableIndex])(WdfDriverGlobals, Request);
}

//
// WDF Function: WdfRequestComplete
//
typedef
WDFAPI
VOID
(*PFN_WDFREQUESTCOMPLETE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    IN NTSTATUS Status
    );

VOID
FORCEINLINE
WdfRequestComplete(
    __in
    WDFREQUEST Request,
    IN NTSTATUS Status
    )
{
    ((PFN_WDFREQUESTCOMPLETE) WdfFunctions[WdfRequestCompleteTableIndex])(WdfDriverGlobals, Request, Status);
}

//
// WDF Function: WdfRequestCompleteWithPriorityBoost
//
typedef
WDFAPI
VOID
(*PFN_WDFREQUESTCOMPLETEWITHPRIORITYBOOST)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    IN NTSTATUS Status,
    IN CCHAR PriorityBoost
    );

VOID
FORCEINLINE
WdfRequestCompleteWithPriorityBoost(
    __in
    WDFREQUEST Request,
    IN NTSTATUS Status,
    IN CCHAR PriorityBoost
    )
{
    ((PFN_WDFREQUESTCOMPLETEWITHPRIORITYBOOST) WdfFunctions[WdfRequestCompleteWithPriorityBoostTableIndex])(WdfDriverGlobals, Request, Status, PriorityBoost);
}

//
// WDF Function: WdfRequestCompleteWithInformation
//
typedef
WDFAPI
VOID
(*PFN_WDFREQUESTCOMPLETEWITHINFORMATION)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    IN NTSTATUS Status,
    IN ULONG_PTR Information
    );

VOID
FORCEINLINE
WdfRequestCompleteWithInformation(
    __in
    WDFREQUEST Request,
    IN NTSTATUS Status,
    IN ULONG_PTR Information
    )
{
    ((PFN_WDFREQUESTCOMPLETEWITHINFORMATION) WdfFunctions[WdfRequestCompleteWithInformationTableIndex])(WdfDriverGlobals, Request, Status, Information);
}

//
// WDF Function: WdfRequestGetParameters
//
typedef
WDFAPI
VOID
(*PFN_WDFREQUESTGETPARAMETERS)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    PWDF_REQUEST_PARAMETERS Parameters
    );

VOID
FORCEINLINE
WdfRequestGetParameters(
    __in
    WDFREQUEST Request,
    __out
    PWDF_REQUEST_PARAMETERS Parameters
    )
{
    ((PFN_WDFREQUESTGETPARAMETERS) WdfFunctions[WdfRequestGetParametersTableIndex])(WdfDriverGlobals, Request, Parameters);
}

//
// WDF Function: WdfRequestRetrieveInputMemory
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREQUESTRETRIEVEINPUTMEMORY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    WDFMEMORY* Memory
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRequestRetrieveInputMemory(
    __in
    WDFREQUEST Request,
    __out
    WDFMEMORY* Memory
    )
{
    return ((PFN_WDFREQUESTRETRIEVEINPUTMEMORY) WdfFunctions[WdfRequestRetrieveInputMemoryTableIndex])(WdfDriverGlobals, Request, Memory);
}

//
// WDF Function: WdfRequestRetrieveOutputMemory
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREQUESTRETRIEVEOUTPUTMEMORY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    WDFMEMORY* Memory
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRequestRetrieveOutputMemory(
    __in
    WDFREQUEST Request,
    __out
    WDFMEMORY* Memory
    )
{
    return ((PFN_WDFREQUESTRETRIEVEOUTPUTMEMORY) WdfFunctions[WdfRequestRetrieveOutputMemoryTableIndex])(WdfDriverGlobals, Request, Memory);
}

//
// WDF Function: WdfRequestRetrieveInputBuffer
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREQUESTRETRIEVEINPUTBUFFER)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    IN size_t MinimumRequiredLength,
    PVOID* Buffer,
    size_t* Length
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRequestRetrieveInputBuffer(
    __in
    WDFREQUEST Request,
    IN size_t MinimumRequiredLength,
    __deref_out_bcount(*Length)
    PVOID* Buffer,
    __out_opt
    size_t* Length
    )
{
    return ((PFN_WDFREQUESTRETRIEVEINPUTBUFFER) WdfFunctions[WdfRequestRetrieveInputBufferTableIndex])(WdfDriverGlobals, Request, MinimumRequiredLength, Buffer, Length);
}

//
// WDF Function: WdfRequestRetrieveOutputBuffer
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREQUESTRETRIEVEOUTPUTBUFFER)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    IN size_t MinimumRequiredSize,
    PVOID* Buffer,
    size_t* Length
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRequestRetrieveOutputBuffer(
    __in
    WDFREQUEST Request,
    IN size_t MinimumRequiredSize,
    __deref_out_bcount(*Length)
    PVOID* Buffer,
    __out_opt
    size_t* Length
    )
{
    return ((PFN_WDFREQUESTRETRIEVEOUTPUTBUFFER) WdfFunctions[WdfRequestRetrieveOutputBufferTableIndex])(WdfDriverGlobals, Request, MinimumRequiredSize, Buffer, Length);
}

//
// WDF Function: WdfRequestRetrieveInputWdmMdl
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREQUESTRETRIEVEINPUTWDMMDL)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    PMDL* Mdl
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRequestRetrieveInputWdmMdl(
    __in
    WDFREQUEST Request,
    __deref_out
    PMDL* Mdl
    )
{
    return ((PFN_WDFREQUESTRETRIEVEINPUTWDMMDL) WdfFunctions[WdfRequestRetrieveInputWdmMdlTableIndex])(WdfDriverGlobals, Request, Mdl);
}

//
// WDF Function: WdfRequestRetrieveOutputWdmMdl
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREQUESTRETRIEVEOUTPUTWDMMDL)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    PMDL* Mdl
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRequestRetrieveOutputWdmMdl(
    __in
    WDFREQUEST Request,
    __deref_out
    PMDL* Mdl
    )
{
    return ((PFN_WDFREQUESTRETRIEVEOUTPUTWDMMDL) WdfFunctions[WdfRequestRetrieveOutputWdmMdlTableIndex])(WdfDriverGlobals, Request, Mdl);
}

//
// WDF Function: WdfRequestRetrieveUnsafeUserInputBuffer
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREQUESTRETRIEVEUNSAFEUSERINPUTBUFFER)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    IN size_t MinimumRequiredLength,
    PVOID* InputBuffer,
    size_t* Length
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRequestRetrieveUnsafeUserInputBuffer(
    __in
    WDFREQUEST Request,
    IN size_t MinimumRequiredLength,
    __deref_out_bcount_opt(*Length)
    PVOID* InputBuffer,
    __out_opt
    size_t* Length
    )
{
    return ((PFN_WDFREQUESTRETRIEVEUNSAFEUSERINPUTBUFFER) WdfFunctions[WdfRequestRetrieveUnsafeUserInputBufferTableIndex])(WdfDriverGlobals, Request, MinimumRequiredLength, InputBuffer, Length);
}

//
// WDF Function: WdfRequestRetrieveUnsafeUserOutputBuffer
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREQUESTRETRIEVEUNSAFEUSEROUTPUTBUFFER)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    IN size_t MinimumRequiredLength,
    PVOID* OutputBuffer,
    size_t* Length
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRequestRetrieveUnsafeUserOutputBuffer(
    __in
    WDFREQUEST Request,
    IN size_t MinimumRequiredLength,
    __deref_out_bcount_opt(*Length)
    PVOID* OutputBuffer,
    __out_opt
    size_t* Length
    )
{
    return ((PFN_WDFREQUESTRETRIEVEUNSAFEUSEROUTPUTBUFFER) WdfFunctions[WdfRequestRetrieveUnsafeUserOutputBufferTableIndex])(WdfDriverGlobals, Request, MinimumRequiredLength, OutputBuffer, Length);
}

//
// WDF Function: WdfRequestSetInformation
//
typedef
WDFAPI
VOID
(*PFN_WDFREQUESTSETINFORMATION)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    IN ULONG_PTR Information
    );

VOID
FORCEINLINE
WdfRequestSetInformation(
    __in
    WDFREQUEST Request,
    IN ULONG_PTR Information
    )
{
    ((PFN_WDFREQUESTSETINFORMATION) WdfFunctions[WdfRequestSetInformationTableIndex])(WdfDriverGlobals, Request, Information);
}

//
// WDF Function: WdfRequestGetInformation
//
typedef
WDFAPI
ULONG_PTR
(*PFN_WDFREQUESTGETINFORMATION)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request
    );

ULONG_PTR
FORCEINLINE
WdfRequestGetInformation(
    __in
    WDFREQUEST Request
    )
{
    return ((PFN_WDFREQUESTGETINFORMATION) WdfFunctions[WdfRequestGetInformationTableIndex])(WdfDriverGlobals, Request);
}

//
// WDF Function: WdfRequestGetFileObject
//
typedef
WDFAPI
WDFFILEOBJECT
(*PFN_WDFREQUESTGETFILEOBJECT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request
    );

WDFFILEOBJECT
FORCEINLINE
WdfRequestGetFileObject(
    __in
    WDFREQUEST Request
    )
{
    return ((PFN_WDFREQUESTGETFILEOBJECT) WdfFunctions[WdfRequestGetFileObjectTableIndex])(WdfDriverGlobals, Request);
}

//
// WDF Function: WdfRequestProbeAndLockUserBufferForRead
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREQUESTPROBEANDLOCKUSERBUFFERFORREAD)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    PVOID Buffer,
    IN size_t Length,
    WDFMEMORY* MemoryObject
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRequestProbeAndLockUserBufferForRead(
    __in
    WDFREQUEST Request,
    __in
    PVOID Buffer,
    IN size_t Length,
    __out
    WDFMEMORY* MemoryObject
    )
{
    return ((PFN_WDFREQUESTPROBEANDLOCKUSERBUFFERFORREAD) WdfFunctions[WdfRequestProbeAndLockUserBufferForReadTableIndex])(WdfDriverGlobals, Request, Buffer, Length, MemoryObject);
}

//
// WDF Function: WdfRequestProbeAndLockUserBufferForWrite
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREQUESTPROBEANDLOCKUSERBUFFERFORWRITE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    PVOID Buffer,
    IN size_t Length,
    WDFMEMORY* MemoryObject
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRequestProbeAndLockUserBufferForWrite(
    __in
    WDFREQUEST Request,
    __in
    PVOID Buffer,
    IN size_t Length,
    __out
    WDFMEMORY* MemoryObject
    )
{
    return ((PFN_WDFREQUESTPROBEANDLOCKUSERBUFFERFORWRITE) WdfFunctions[WdfRequestProbeAndLockUserBufferForWriteTableIndex])(WdfDriverGlobals, Request, Buffer, Length, MemoryObject);
}

//
// WDF Function: WdfRequestGetRequestorMode
//
typedef
WDFAPI
KPROCESSOR_MODE
(*PFN_WDFREQUESTGETREQUESTORMODE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request
    );

KPROCESSOR_MODE
FORCEINLINE
WdfRequestGetRequestorMode(
    __in
    WDFREQUEST Request
    )
{
    return ((PFN_WDFREQUESTGETREQUESTORMODE) WdfFunctions[WdfRequestGetRequestorModeTableIndex])(WdfDriverGlobals, Request);
}

//
// WDF Function: WdfRequestForwardToIoQueue
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREQUESTFORWARDTOIOQUEUE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    WDFQUEUE DestinationQueue
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRequestForwardToIoQueue(
    __in
    WDFREQUEST Request,
    __in
    WDFQUEUE DestinationQueue
    )
{
    return ((PFN_WDFREQUESTFORWARDTOIOQUEUE) WdfFunctions[WdfRequestForwardToIoQueueTableIndex])(WdfDriverGlobals, Request, DestinationQueue);
}

//
// WDF Function: WdfRequestGetIoQueue
//
typedef
WDFAPI
WDFQUEUE
(*PFN_WDFREQUESTGETIOQUEUE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request
    );

WDFQUEUE
FORCEINLINE
WdfRequestGetIoQueue(
    __in
    WDFREQUEST Request
    )
{
    return ((PFN_WDFREQUESTGETIOQUEUE) WdfFunctions[WdfRequestGetIoQueueTableIndex])(WdfDriverGlobals, Request);
}

//
// WDF Function: WdfRequestRequeue
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREQUESTREQUEUE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRequestRequeue(
    __in
    WDFREQUEST Request
    )
{
    return ((PFN_WDFREQUESTREQUEUE) WdfFunctions[WdfRequestRequeueTableIndex])(WdfDriverGlobals, Request);
}

//
// WDF Function: WdfRequestStopAcknowledge
//
typedef
WDFAPI
VOID
(*PFN_WDFREQUESTSTOPACKNOWLEDGE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request,
    IN BOOLEAN Requeue
    );

VOID
FORCEINLINE
WdfRequestStopAcknowledge(
    __in
    WDFREQUEST Request,
    IN BOOLEAN Requeue
    )
{
    ((PFN_WDFREQUESTSTOPACKNOWLEDGE) WdfFunctions[WdfRequestStopAcknowledgeTableIndex])(WdfDriverGlobals, Request, Requeue);
}

//
// WDF Function: WdfRequestWdmGetIrp
//
typedef
WDFAPI
PIRP
(*PFN_WDFREQUESTWDMGETIRP)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFREQUEST Request
    );

PIRP
FORCEINLINE
WdfRequestWdmGetIrp(
    __in
    WDFREQUEST Request
    )
{
    return ((PFN_WDFREQUESTWDMGETIRP) WdfFunctions[WdfRequestWdmGetIrpTableIndex])(WdfDriverGlobals, Request);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFREQUEST_H_

