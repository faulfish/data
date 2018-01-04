/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    wdfio.h

Abstract:

    This module contains contains the Windows Driver Framework I/O
    interfaces.

Environment:

    kernel mode only

Revision History:


--*/

#ifndef _WDFIO_H_
#define _WDFIO_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)

// 
// Types
// 

// 
// This defines the dispatch type of the queue. This controls how
// the queue raises I/O events to the driver through the registered
// callbacks.
// 
// Sequential allows the driver to have the queue automatically dispatch
// one request at a time, and will hold requests until a current request
// is completed.
// 
// Parallel has the queue dispatch requests to the driver as they arrive
// at the queue, and the queue is in a processing state. The driver can
// look at the requests in real time, and decide to service them, forward them
// to another queue, pend them, or return a status to have the queue held
// due to a hardware or other resource limit.
// 
// Manual allows the driver to create multiple queues for requests, and control
// when it wants to retrieve requests from the queue by calling the queues
// WdfIoQueueRetrieveNextRequest() API.
// 

typedef enum _WDF_IO_QUEUE_DISPATCH_TYPE {
    WdfIoQueueDispatchInvalid = 0,
    WdfIoQueueDispatchSequential,
    WdfIoQueueDispatchParallel,
    WdfIoQueueDispatchManual,
    WdfIoQueueDispatchMax,
} WDF_IO_QUEUE_DISPATCH_TYPE;

// 
// This defines the status of the queue.
// 
// WdfIoQueueAcceptRequests   - If TRUE, Queue will accept requests from WDM
// dispatch through WdfDeviceConfigureRequestDispatching,
// or from WdfRequestForwardToIoQueue.
// 
// If FALSE, Queue will complete requests with
// STATUS_CANCELLED from WdfDeviceConfigureRequestDispatching,
// and fail WdfRequestForwardToIoQueue with
// STATUS_WDF_BUSY.
// 
// 
// WdfIoQueueDispatchRequests - If TRUE, and the Queue is configured for
// automatic dispatch as either
// WdfIoQueueDispatchSequential,
// or WdfIoQueueDispatchParallel, the Queue will
// present the requests to the driver according
// to the drivers configuration.
// 
// If FALSE, requests are not automatically
// presented to the device driver.
// 
// This has no effect on the drivers ability to
// retrieve requests with WdfIoQueueRetrieveNextRequest.
// 
// WdfIoQueueNoRequests       - If TRUE, the Queue has no requests to present
// or return to the device driver.
// 
// WdfIoQueueDriverNoRequests - If TRUE, the device driver is not operating
// on any requests retrieved from or presented
// by this Queue.
// 
// WdfIoQueuePnpHeld          - The Framework PnP stage has requested that
// the device driver stop receiving new requests.
// 
// Automatic request dispatch stops, and
// WdfIoQueueRetrieveNextRequest returns STATUS_WDF_BUSY.
// 

typedef enum _WDF_IO_QUEUE_STATE {
    WdfIoQueueAcceptRequests = 0x01,
    WdfIoQueueDispatchRequests = 0x02,
    WdfIoQueueNoRequests = 0x04,
    WdfIoQueueDriverNoRequests = 0x08,
    WdfIoQueuePnpHeld = 0x10,
} WDF_IO_QUEUE_STATE;



//
// These macros represent some common Queue states
//

//
// A Queue is idle if it has no requests, and the driver
// is not operating on any.
//

BOOLEAN
FORCEINLINE
WDF_IO_QUEUE_IDLE(
    WDF_IO_QUEUE_STATE State
    )
{
    return ((State & WdfIoQueueNoRequests) &&
            (State & WdfIoQueueDriverNoRequests)) ? TRUE: FALSE;
}

//
// A Queue is ready if it can accept and dispatch requests and
// queue is not held by PNP
//
BOOLEAN
FORCEINLINE
WDF_IO_QUEUE_READY(
    WDF_IO_QUEUE_STATE State
    )
{
   return ((State & WdfIoQueueDispatchRequests) &&
        (State & WdfIoQueueAcceptRequests) &&
        ((State & WdfIoQueuePnpHeld)==0)) ? TRUE: FALSE;
}

//
// A Queue is stopped if it can accept new requests, but
// is not automatically delivering them to the device driver,
// and the queue is idle.
//
BOOLEAN
FORCEINLINE
WDF_IO_QUEUE_STOPPED(
    WDF_IO_QUEUE_STATE State
    )
{
    return (((State & WdfIoQueueDispatchRequests) == 0) &&
        (State & WdfIoQueueAcceptRequests) &&
        (State & WdfIoQueueDriverNoRequests)) ? TRUE: FALSE;

}

//
// A Queue is drained if it can not accept new requests but
// can dispatch existing requests, and there are no requests
// either in the Queue or the device driver.
//

BOOLEAN
FORCEINLINE
WDF_IO_QUEUE_DRAINED(
    WDF_IO_QUEUE_STATE State
    )
{
    return ( ((State & WdfIoQueueAcceptRequests)==0) &&
          (State & WdfIoQueueDispatchRequests) &&
          (State & WdfIoQueueNoRequests)  &&
          (State & WdfIoQueueDriverNoRequests) ) ? TRUE: FALSE;

}

//
// A Queue is purged if it can not accept new requests
// and there are no requests either in the Queue or
// the device driver.
//
BOOLEAN
FORCEINLINE
WDF_IO_QUEUE_PURGED(
    WDF_IO_QUEUE_STATE State
    )
{
    return ( ((State & WdfIoQueueAcceptRequests)==0) &&
          (State & WdfIoQueueNoRequests) &&
          (State & WdfIoQueueDriverNoRequests) ) ? TRUE: FALSE;

}

//
// Event callback definitions
//

typedef
VOID
(*PFN_WDF_IO_QUEUE_IO_DEFAULT) (
    IN WDFQUEUE                Queue,
    IN WDFREQUEST              Request
    );


typedef
VOID
(*PFN_WDF_IO_QUEUE_IO_STOP) (
    IN WDFQUEUE                 Queue,
    IN WDFREQUEST               Request,
    IN ULONG ActionFlags
    );

typedef
VOID
(*PFN_WDF_IO_QUEUE_IO_RESUME) (
    IN WDFQUEUE   Queue,
    IN WDFREQUEST Request
    );

typedef
VOID
(*PFN_WDF_IO_QUEUE_IO_READ) (
    IN WDFQUEUE      Queue,
    IN WDFREQUEST    Request,
    IN size_t         Length
    );

typedef
VOID
(*PFN_WDF_IO_QUEUE_IO_WRITE) (
    IN WDFQUEUE      Queue,
    IN WDFREQUEST    Request,
    IN size_t         Length
    );

typedef
VOID
(*PFN_WDF_IO_QUEUE_IO_DEVICE_CONTROL) (
    IN WDFQUEUE      Queue,
    IN WDFREQUEST    Request,
    IN size_t         OutputBufferLength,
    IN size_t         InputBufferLength,
    IN ULONG         IoControlCode
    );

typedef
VOID
(*PFN_WDF_IO_QUEUE_IO_INTERNAL_DEVICE_CONTROL) (
    IN WDFQUEUE      Queue,
    IN WDFREQUEST    Request,
    IN size_t         OutputBufferLength,
    IN size_t         InputBufferLength,
    IN ULONG         IoControlCode
    );


typedef
VOID
(*PFN_WDF_IO_QUEUE_IO_CANCELED_ON_QUEUE) (
    IN WDFQUEUE                Queue,
    IN WDFREQUEST              Request
    );


typedef
VOID
(*PFN_WDF_IO_QUEUE_STATE) (
    IN WDFQUEUE            Queue,
    IN WDFCONTEXT          Context
    );

//
// This is the structure used to configure an IoQueue and
// register callback events to it.
//

typedef struct _WDF_IO_QUEUE_CONFIG {

    ULONG                                       Size;

    WDF_IO_QUEUE_DISPATCH_TYPE                  DispatchType;

    WDF_TRI_STATE                               PowerManaged;

    BOOLEAN                                     AllowZeroLengthRequests;

    BOOLEAN                                     DefaultQueue;

    PFN_WDF_IO_QUEUE_IO_DEFAULT                 EvtIoDefault;

    PFN_WDF_IO_QUEUE_IO_READ                    EvtIoRead;

    PFN_WDF_IO_QUEUE_IO_WRITE                   EvtIoWrite;

    PFN_WDF_IO_QUEUE_IO_DEVICE_CONTROL          EvtIoDeviceControl;

    PFN_WDF_IO_QUEUE_IO_INTERNAL_DEVICE_CONTROL EvtIoInternalDeviceControl;

    PFN_WDF_IO_QUEUE_IO_STOP                    EvtIoStop;

    PFN_WDF_IO_QUEUE_IO_RESUME                  EvtIoResume;

    PFN_WDF_IO_QUEUE_IO_CANCELED_ON_QUEUE       EvtIoCanceledOnQueue;

} WDF_IO_QUEUE_CONFIG, *PWDF_IO_QUEUE_CONFIG;

VOID
FORCEINLINE
WDF_IO_QUEUE_CONFIG_INIT(
    OUT PWDF_IO_QUEUE_CONFIG      Config,
    IN WDF_IO_QUEUE_DISPATCH_TYPE DispatchType
    )
{
    RtlZeroMemory(Config, sizeof(WDF_IO_QUEUE_CONFIG));

    Config->Size = sizeof(WDF_IO_QUEUE_CONFIG);
    Config->PowerManaged = WdfUseDefault;
    Config->DispatchType = DispatchType;
}

VOID
FORCEINLINE
WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(
    OUT PWDF_IO_QUEUE_CONFIG      Config,
    IN WDF_IO_QUEUE_DISPATCH_TYPE DispatchType
    )
{
    RtlZeroMemory(Config, sizeof(WDF_IO_QUEUE_CONFIG));

    Config->Size = sizeof(WDF_IO_QUEUE_CONFIG);
    Config->PowerManaged = WdfUseDefault;
    Config->DefaultQueue = TRUE;
    Config->DispatchType = DispatchType;
}

//
// WDF Function: WdfIoQueueCreate
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOQUEUECREATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PWDF_IO_QUEUE_CONFIG Config,
    PWDF_OBJECT_ATTRIBUTES QueueAttributes,
    WDFQUEUE* Queue
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoQueueCreate(
    __in
    WDFDEVICE Device,
    __in
    PWDF_IO_QUEUE_CONFIG Config,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES QueueAttributes,
    __out_opt
    WDFQUEUE* Queue
    )
{
    return ((PFN_WDFIOQUEUECREATE) WdfFunctions[WdfIoQueueCreateTableIndex])(WdfDriverGlobals, Device, Config, QueueAttributes, Queue);
}

//
// WDF Function: WdfIoQueueGetState
//
typedef
WDFAPI
WDF_IO_QUEUE_STATE
(*PFN_WDFIOQUEUEGETSTATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFQUEUE Queue,
    PULONG QueueRequests,
    PULONG DriverRequests
    );

WDF_IO_QUEUE_STATE
FORCEINLINE
WdfIoQueueGetState(
    __in
    WDFQUEUE Queue,
    __out_opt
    PULONG QueueRequests,
    __out_opt
    PULONG DriverRequests
    )
{
    return ((PFN_WDFIOQUEUEGETSTATE) WdfFunctions[WdfIoQueueGetStateTableIndex])(WdfDriverGlobals, Queue, QueueRequests, DriverRequests);
}

//
// WDF Function: WdfIoQueueStart
//
typedef
WDFAPI
VOID
(*PFN_WDFIOQUEUESTART)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFQUEUE Queue
    );

VOID
FORCEINLINE
WdfIoQueueStart(
    __in
    WDFQUEUE Queue
    )
{
    ((PFN_WDFIOQUEUESTART) WdfFunctions[WdfIoQueueStartTableIndex])(WdfDriverGlobals, Queue);
}

//
// WDF Function: WdfIoQueueStop
//
typedef
WDFAPI
VOID
(*PFN_WDFIOQUEUESTOP)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFQUEUE Queue,
    PFN_WDF_IO_QUEUE_STATE StopComplete,
    WDFCONTEXT Context
    );

VOID
FORCEINLINE
WdfIoQueueStop(
    __in
    WDFQUEUE Queue,
    __in_opt
    PFN_WDF_IO_QUEUE_STATE StopComplete,
    __in_opt
    WDFCONTEXT Context
    )
{
    ((PFN_WDFIOQUEUESTOP) WdfFunctions[WdfIoQueueStopTableIndex])(WdfDriverGlobals, Queue, StopComplete, Context);
}

//
// WDF Function: WdfIoQueueStopSynchronously
//
typedef
WDFAPI
VOID
(*PFN_WDFIOQUEUESTOPSYNCHRONOUSLY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFQUEUE Queue
    );

VOID
FORCEINLINE
WdfIoQueueStopSynchronously(
    __in
    WDFQUEUE Queue
    )
{
    ((PFN_WDFIOQUEUESTOPSYNCHRONOUSLY) WdfFunctions[WdfIoQueueStopSynchronouslyTableIndex])(WdfDriverGlobals, Queue);
}

//
// WDF Function: WdfIoQueueGetDevice
//
typedef
WDFAPI
WDFDEVICE
(*PFN_WDFIOQUEUEGETDEVICE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFQUEUE Queue
    );

WDFDEVICE
FORCEINLINE
WdfIoQueueGetDevice(
    __in
    WDFQUEUE Queue
    )
{
    return ((PFN_WDFIOQUEUEGETDEVICE) WdfFunctions[WdfIoQueueGetDeviceTableIndex])(WdfDriverGlobals, Queue);
}

//
// WDF Function: WdfIoQueueRetrieveNextRequest
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOQUEUERETRIEVENEXTREQUEST)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFQUEUE Queue,
    WDFREQUEST* OutRequest
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoQueueRetrieveNextRequest(
    __in
    WDFQUEUE Queue,
    __out
    WDFREQUEST* OutRequest
    )
{
    return ((PFN_WDFIOQUEUERETRIEVENEXTREQUEST) WdfFunctions[WdfIoQueueRetrieveNextRequestTableIndex])(WdfDriverGlobals, Queue, OutRequest);
}

//
// WDF Function: WdfIoQueueRetrieveRequestByFileObject
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOQUEUERETRIEVEREQUESTBYFILEOBJECT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFQUEUE Queue,
    WDFFILEOBJECT FileObject,
    WDFREQUEST* OutRequest
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoQueueRetrieveRequestByFileObject(
    __in
    WDFQUEUE Queue,
    __in
    WDFFILEOBJECT FileObject,
    __out
    WDFREQUEST* OutRequest
    )
{
    return ((PFN_WDFIOQUEUERETRIEVEREQUESTBYFILEOBJECT) WdfFunctions[WdfIoQueueRetrieveRequestByFileObjectTableIndex])(WdfDriverGlobals, Queue, FileObject, OutRequest);
}

//
// WDF Function: WdfIoQueueFindRequest
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOQUEUEFINDREQUEST)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFQUEUE Queue,
    WDFREQUEST FoundRequest,
    WDFFILEOBJECT FileObject,
    PWDF_REQUEST_PARAMETERS Parameters,
    WDFREQUEST* OutRequest
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoQueueFindRequest(
    __in
    WDFQUEUE Queue,
    __in_opt
    WDFREQUEST FoundRequest,
    __in_opt
    WDFFILEOBJECT FileObject,
    __out_opt
    PWDF_REQUEST_PARAMETERS Parameters,
    __out
    WDFREQUEST* OutRequest
    )
{
    return ((PFN_WDFIOQUEUEFINDREQUEST) WdfFunctions[WdfIoQueueFindRequestTableIndex])(WdfDriverGlobals, Queue, FoundRequest, FileObject, Parameters, OutRequest);
}

//
// WDF Function: WdfIoQueueRetrieveFoundRequest
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOQUEUERETRIEVEFOUNDREQUEST)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFQUEUE Queue,
    WDFREQUEST FoundRequest,
    WDFREQUEST* OutRequest
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoQueueRetrieveFoundRequest(
    __in
    WDFQUEUE Queue,
    __in
    WDFREQUEST FoundRequest,
    __out
    WDFREQUEST* OutRequest
    )
{
    return ((PFN_WDFIOQUEUERETRIEVEFOUNDREQUEST) WdfFunctions[WdfIoQueueRetrieveFoundRequestTableIndex])(WdfDriverGlobals, Queue, FoundRequest, OutRequest);
}

//
// WDF Function: WdfIoQueueDrainSynchronously
//
typedef
WDFAPI
VOID
(*PFN_WDFIOQUEUEDRAINSYNCHRONOUSLY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFQUEUE Queue
    );

VOID
FORCEINLINE
WdfIoQueueDrainSynchronously(
    __in
    WDFQUEUE Queue
    )
{
    ((PFN_WDFIOQUEUEDRAINSYNCHRONOUSLY) WdfFunctions[WdfIoQueueDrainSynchronouslyTableIndex])(WdfDriverGlobals, Queue);
}

//
// WDF Function: WdfIoQueueDrain
//
typedef
WDFAPI
VOID
(*PFN_WDFIOQUEUEDRAIN)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFQUEUE Queue,
    PFN_WDF_IO_QUEUE_STATE DrainComplete,
    WDFCONTEXT Context
    );

VOID
FORCEINLINE
WdfIoQueueDrain(
    __in
    WDFQUEUE Queue,
    __in_opt
    PFN_WDF_IO_QUEUE_STATE DrainComplete,
    __in_opt
    WDFCONTEXT Context
    )
{
    ((PFN_WDFIOQUEUEDRAIN) WdfFunctions[WdfIoQueueDrainTableIndex])(WdfDriverGlobals, Queue, DrainComplete, Context);
}

//
// WDF Function: WdfIoQueuePurgeSynchronously
//
typedef
WDFAPI
VOID
(*PFN_WDFIOQUEUEPURGESYNCHRONOUSLY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFQUEUE Queue
    );

VOID
FORCEINLINE
WdfIoQueuePurgeSynchronously(
    __in
    WDFQUEUE Queue
    )
{
    ((PFN_WDFIOQUEUEPURGESYNCHRONOUSLY) WdfFunctions[WdfIoQueuePurgeSynchronouslyTableIndex])(WdfDriverGlobals, Queue);
}

//
// WDF Function: WdfIoQueuePurge
//
typedef
WDFAPI
VOID
(*PFN_WDFIOQUEUEPURGE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFQUEUE Queue,
    PFN_WDF_IO_QUEUE_STATE PurgeComplete,
    WDFCONTEXT Context
    );

VOID
FORCEINLINE
WdfIoQueuePurge(
    __in
    WDFQUEUE Queue,
    __in_opt
    PFN_WDF_IO_QUEUE_STATE PurgeComplete,
    __in_opt
    WDFCONTEXT Context
    )
{
    ((PFN_WDFIOQUEUEPURGE) WdfFunctions[WdfIoQueuePurgeTableIndex])(WdfDriverGlobals, Queue, PurgeComplete, Context);
}

//
// WDF Function: WdfIoQueueReadyNotify
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIOQUEUEREADYNOTIFY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFQUEUE Queue,
    PFN_WDF_IO_QUEUE_STATE QueueReady,
    WDFCONTEXT Context
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoQueueReadyNotify(
    __in
    WDFQUEUE Queue,
    __in_opt
    PFN_WDF_IO_QUEUE_STATE QueueReady,
    __in_opt
    WDFCONTEXT Context
    )
{
    return ((PFN_WDFIOQUEUEREADYNOTIFY) WdfFunctions[WdfIoQueueReadyNotifyTableIndex])(WdfDriverGlobals, Queue, QueueReady, Context);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFIO_H_

