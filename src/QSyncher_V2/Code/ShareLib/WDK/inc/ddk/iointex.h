/*++

Copyright (c) Microsoft Corporation. All rights reserved.

Module Name:

    iointex.h

Abstract:

    This header exposes the new kernel APIs to connect and disconnect
    interrupts in a manner that makes it possible for drivers referencing
    these new APIs to run on downlevel systems.

--*/

#ifndef _IOINTEX_IOINTEX_H_
#define _IOINTEX_IOINTEX_H_
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

//
// Provide definitions for the types and structures which were introduced with
// the new interrupt connection APIs if they haven't already been defined by
// standard headers.  If this header is included in a driver being built for
// a downlevel OS, then it will include the version of ntddk.h or wdm.h that
// was current when that OS shipped.  As a result, we need to manually provide
// the required types in this case.
//
// See DDK documentation and the definition of these structures in the most
// recent standard headers for more information on the meaning of the
// individual types and fields.
//

#if !defined(CONNECT_FULLY_SPECIFIED)

typedef enum _KINTERRUPT_POLARITY {
    InterruptPolarityUnknown,
    InterruptActiveHigh,
    InterruptActiveLow
} KINTERRUPT_POLARITY, *PKINTERRUPT_POLARITY;

typedef
BOOLEAN
(*PKMESSAGE_SERVICE_ROUTINE) (
    IN struct _KINTERRUPT *Interrupt,
    IN PVOID ServiceContext,
    IN ULONG MessageID
    );

//
// Interrupt message information table definition.
//

typedef struct _IO_INTERRUPT_MESSAGE_INFO_ENTRY {
    PHYSICAL_ADDRESS MessageAddress;
    KAFFINITY TargetProcessorSet;
    PKINTERRUPT InterruptObject;
    ULONG MessageData;
    ULONG Vector;
    KIRQL Irql;
    KINTERRUPT_MODE Mode;
    KINTERRUPT_POLARITY Polarity;
} IO_INTERRUPT_MESSAGE_INFO_ENTRY, *PIO_INTERRUPT_MESSAGE_INFO_ENTRY;

typedef struct _IO_INTERRUPT_MESSAGE_INFO {
    KIRQL UnifiedIrql;
    ULONG MessageCount;
    IO_INTERRUPT_MESSAGE_INFO_ENTRY MessageInfo[1];
} IO_INTERRUPT_MESSAGE_INFO, *PIO_INTERRUPT_MESSAGE_INFO;

//
// Fully specified, line based, and message based interrupt connection
// parameter structure definitions.
//

typedef struct _IO_CONNECT_INTERRUPT_FULLY_SPECIFIED_PARAMETERS {
    IN PDEVICE_OBJECT PhysicalDeviceObject;
    OUT PKINTERRUPT *InterruptObject;
    IN PKSERVICE_ROUTINE ServiceRoutine;
    IN PVOID ServiceContext;
    IN PKSPIN_LOCK SpinLock OPTIONAL;
    IN KIRQL SynchronizeIrql;
    IN BOOLEAN FloatingSave;
    IN BOOLEAN ShareVector;
    IN ULONG Vector;
    IN KIRQL Irql;
    IN KINTERRUPT_MODE InterruptMode;
    IN KAFFINITY ProcessorEnableMask;
} IO_CONNECT_INTERRUPT_FULLY_SPECIFIED_PARAMETERS,
  *PIO_CONNECT_INTERRUPT_FULLY_SPECIFIED_PARAMETERS; 

typedef struct _IO_CONNECT_INTERRUPT_LINE_BASED_PARAMETERS {
    IN PDEVICE_OBJECT PhysicalDeviceObject;
    OUT PKINTERRUPT *InterruptObject;
    IN PKSERVICE_ROUTINE ServiceRoutine;
    IN PVOID ServiceContext;
    IN PKSPIN_LOCK SpinLock OPTIONAL;
    IN KIRQL SynchronizeIrql OPTIONAL;
    IN BOOLEAN FloatingSave;
} IO_CONNECT_INTERRUPT_LINE_BASED_PARAMETERS,
  *PIO_CONNECT_INTERRUPT_LINE_BASED_PARAMETERS;

typedef struct _IO_CONNECT_INTERRUPT_MESSAGE_BASED_PARAMETERS {
    IN PDEVICE_OBJECT PhysicalDeviceObject;
    union {
        OUT PVOID *Generic;
        OUT PIO_INTERRUPT_MESSAGE_INFO *InterruptMessageTable;
        OUT PKINTERRUPT *InterruptObject;
    } ConnectionContext;
    IN PKMESSAGE_SERVICE_ROUTINE MessageServiceRoutine;
    IN PVOID ServiceContext;
    IN PKSPIN_LOCK SpinLock OPTIONAL;
    IN KIRQL SynchronizeIrql OPTIONAL;
    IN BOOLEAN FloatingSave;
    IN PKSERVICE_ROUTINE FallBackServiceRoutine OPTIONAL;
} IO_CONNECT_INTERRUPT_MESSAGE_BASED_PARAMETERS,
  *PIO_CONNECT_INTERRUPT_MESSAGE_BASED_PARAMETERS;

//
// Top level interrupt connection parameter definitions.
//

#define CONNECT_FULLY_SPECIFIED     0x1
#define CONNECT_LINE_BASED          0x2
#define CONNECT_MESSAGE_BASED       0x3
#define CONNECT_CURRENT_VERSION     0x3

typedef struct _IO_CONNECT_INTERRUPT_PARAMETERS {
    IN OUT ULONG Version;
    union {
        IO_CONNECT_INTERRUPT_FULLY_SPECIFIED_PARAMETERS FullySpecified;
        IO_CONNECT_INTERRUPT_LINE_BASED_PARAMETERS LineBased;
        IO_CONNECT_INTERRUPT_MESSAGE_BASED_PARAMETERS MessageBased;
    };
} IO_CONNECT_INTERRUPT_PARAMETERS, *PIO_CONNECT_INTERRUPT_PARAMETERS;

//
// Interrupt disconnection parameter structure definition.
//

typedef struct _IO_DISCONNECT_INTERRUPT_PARAMETERS {
    IN ULONG Version;
    union {
        IN PVOID Generic;
        IN PKINTERRUPT InterruptObject;
        IN PIO_INTERRUPT_MESSAGE_INFO InterruptMessageTable;
    } ConnectionContext;
} IO_DISCONNECT_INTERRUPT_PARAMETERS, *PIO_DISCONNECT_INTERRUPT_PARAMETERS;

#endif // !defined(CONNECT_FULLY_SPECIFIED)

//
// Supply an overrideable library implementation of IoConnectInterruptEx.
// See DDK documentation for more details on this API.
//

#undef IoConnectInterruptEx
#define IoConnectInterruptEx WdmlibIoConnectInterruptEx

NTSTATUS
WdmlibIoConnectInterruptEx (
    __inout PIO_CONNECT_INTERRUPT_PARAMETERS Parameters
    );

//
// Supply an overrideable library implementation of IoDisconnectInterruptEx.
// See DDK documentation for more details on this API.
//

#undef IoDisconnectInterruptEx
#define IoDisconnectInterruptEx WdmlibIoDisconnectInterruptEx

NTSTATUS
WdmlibIoDisconnectInterruptEx (
    __inout PIO_DISCONNECT_INTERRUPT_PARAMETERS Parameters
    );


#ifdef __cplusplus
} // extern "C"
#endif

#endif // !defined(_IOINTEX_IOINTEX_H_)

