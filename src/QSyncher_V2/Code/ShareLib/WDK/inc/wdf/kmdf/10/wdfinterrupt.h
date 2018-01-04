/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    wdfinterrupt.h

Abstract:

    This is the C header for driver framework Interrupt object

Revision History:


--*/

#ifndef _WDFINTERRUPT_H_
#define _WDFINTERRUPT_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)

// 
// Message Signaled Interrupts (MSI) information structure
// 
typedef enum _WDF_INTERRUPT_POLARITY {
    WdfInterruptPolarityUnknown = 0,
    WdfInterruptActiveHigh,
    WdfInterruptActiveLow,
} WDF_INTERRUPT_POLARITY, *PWDF_INTERRUPT_POLARITY;

typedef enum _WDF_INTERRUPT_POLICY {
    WdfIrqPolicyMachineDefault = 0,
    WdfIrqPolicyAllCloseProcessors,
    WdfIrqPolicyOneCloseProcessor,
    WdfIrqPolicyAllProcessorsInMachine,
    WdfIrqPolicySpecifiedProcessors,
    WdfIrqPolicySpreadMessagesAcrossAllProcessors,
} WDF_INTERRUPT_POLICY, *PWDF_INTERRUPT_POLICY;

typedef enum _WDF_INTERRUPT_PRIORITY {
    WdfIrqPriorityUndefined = 0,
    WdfIrqPriorityLow,
    WdfIrqPriorityNormal,
    WdfIrqPriorityHigh,
} WDF_INTERRUPT_PRIORITY, *PWDF_INTERRUPT_PRIORITY;



//
// This is the function that gets invoked when the hardware ISR occurs.  This
// function is called at the IRQL at which the interrupt is serviced (DIRQL.)
//
typedef
BOOLEAN
(*PFN_WDF_INTERRUPT_ISR) (
    IN WDFINTERRUPT  Interrupt,
    IN ULONG         MessageID
    );

//
// This is the function that gets invoked when a Synchronize execution occurs.
// It will be called at DIRQL.
//
typedef
BOOLEAN
(*PFN_WDF_INTERRUPT_SYNCHRONIZE) (
    IN WDFINTERRUPT  Interrupt,
    IN WDFCONTEXT    Context
    );

//
// This is the function that gets called back into the driver
// when the DpcForIsr fires.  It will be called at DISPATCH_LEVEL.
//
typedef
VOID
(*PFN_WDF_INTERRUPT_DPC) (
    IN WDFINTERRUPT Interrupt,
    IN WDFOBJECT    AssociatedObject
    );

//
// This is the function that gets called back into the driver
// to enable the interrupt in the hardware.  It will be called
// at the same IRQL at which the interrupt will be serviced. (DIRQL)
//
typedef
NTSTATUS
(*PFN_WDF_INTERRUPT_ENABLE) (
    IN WDFINTERRUPT  Interrupt,
    IN WDFDEVICE     AssociatedDevice
    );

//
// This is the function that gets called back into the driver
// to disable the interrupt in the hardware.  It will be called at DIRQL.
//
typedef
NTSTATUS
(*PFN_WDF_INTERRUPT_DISABLE) (
    IN WDFINTERRUPT  Interrupt,
    IN WDFDEVICE     AssociatedDevice
    );

//
// Interrupt Configuration Structure
//
typedef struct _WDF_INTERRUPT_CONFIG {
    ULONG              Size;

    //
    // If this interrupt is to be synchronized with other interrupt(s) assigned
    // to the same WDFDEVICE, create a WDFSPINLOCK and assign it to each of the
    // WDFINTERRUPTs config.
    //
    WDFSPINLOCK        SpinLock;

    WDF_TRI_STATE      ShareVector;

    BOOLEAN            FloatingSave;

    //
    // Automatic Serialization of the DpcForIsr
    //
    BOOLEAN            AutomaticSerialization;

    // Event Callbacks
    PFN_WDF_INTERRUPT_ISR         EvtInterruptIsr;
    PFN_WDF_INTERRUPT_DPC         EvtInterruptDpc;
    PFN_WDF_INTERRUPT_ENABLE      EvtInterruptEnable;
    PFN_WDF_INTERRUPT_DISABLE     EvtInterruptDisable;

} WDF_INTERRUPT_CONFIG, *PWDF_INTERRUPT_CONFIG;


VOID
FORCEINLINE
WDF_INTERRUPT_CONFIG_INIT(
    OUT PWDF_INTERRUPT_CONFIG Configuration,
    IN  PFN_WDF_INTERRUPT_ISR EvtInterruptIsr,
    IN  PFN_WDF_INTERRUPT_DPC EvtInterruptDpc
    )
{
    RtlZeroMemory(Configuration, sizeof(WDF_INTERRUPT_CONFIG));

    Configuration->Size = sizeof(WDF_INTERRUPT_CONFIG);

    Configuration->ShareVector = WdfUseDefault;

    Configuration->EvtInterruptIsr     = EvtInterruptIsr;
    Configuration->EvtInterruptDpc     = EvtInterruptDpc;
}

typedef struct _WDF_INTERRUPT_INFO {
    //
    // Size of this structure in bytes
    //
    ULONG                  Size;
    ULONG64                Reserved1;
    KAFFINITY              TargetProcessorSet;
    ULONG                  Reserved2;
    ULONG                  MessageNumber;
    ULONG                  Vector;
    KIRQL                  Irql;
    KINTERRUPT_MODE        Mode;
    WDF_INTERRUPT_POLARITY Polarity;
    BOOLEAN                MessageSignaled;
    UCHAR                  ShareDisposition; //CM_SHARE_DISPOSITION

} WDF_INTERRUPT_INFO, *PWDF_INTERRUPT_INFO;

VOID
FORCEINLINE
WDF_INTERRUPT_INFO_INIT(
    PWDF_INTERRUPT_INFO Info
    )
{
    RtlZeroMemory(Info, sizeof(WDF_INTERRUPT_INFO));
    Info->Size = sizeof(WDF_INTERRUPT_INFO);
}

//
// WDF Function: WdfInterruptCreate
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFINTERRUPTCREATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PWDF_INTERRUPT_CONFIG Configuration,
    PWDF_OBJECT_ATTRIBUTES Attributes,
    WDFINTERRUPT* Interrupt
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfInterruptCreate(
    __in
    WDFDEVICE Device,
    __in
    PWDF_INTERRUPT_CONFIG Configuration,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES Attributes,
    __out
    WDFINTERRUPT* Interrupt
    )
{
    return ((PFN_WDFINTERRUPTCREATE) WdfFunctions[WdfInterruptCreateTableIndex])(WdfDriverGlobals, Device, Configuration, Attributes, Interrupt);
}

//
// WDF Function: WdfInterruptQueueDpcForIsr
//
typedef
WDFAPI
BOOLEAN
(*PFN_WDFINTERRUPTQUEUEDPCFORISR)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFINTERRUPT Interrupt
    );

BOOLEAN
FORCEINLINE
WdfInterruptQueueDpcForIsr(
    __in
    WDFINTERRUPT Interrupt
    )
{
    return ((PFN_WDFINTERRUPTQUEUEDPCFORISR) WdfFunctions[WdfInterruptQueueDpcForIsrTableIndex])(WdfDriverGlobals, Interrupt);
}

//
// WDF Function: WdfInterruptSynchronize
//
typedef
WDFAPI
BOOLEAN
(*PFN_WDFINTERRUPTSYNCHRONIZE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFINTERRUPT Interrupt,
    PFN_WDF_INTERRUPT_SYNCHRONIZE Callback,
    IN WDFCONTEXT Context
    );

BOOLEAN
FORCEINLINE
WdfInterruptSynchronize(
    __in
    WDFINTERRUPT Interrupt,
    __in
    PFN_WDF_INTERRUPT_SYNCHRONIZE Callback,
    IN WDFCONTEXT Context
    )
{
    return ((PFN_WDFINTERRUPTSYNCHRONIZE) WdfFunctions[WdfInterruptSynchronizeTableIndex])(WdfDriverGlobals, Interrupt, Callback, Context);
}

//
// WDF Function: WdfInterruptAcquireLock
//
typedef
WDFAPI
VOID
(*PFN_WDFINTERRUPTACQUIRELOCK)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFINTERRUPT Interrupt
    );

VOID
FORCEINLINE
WdfInterruptAcquireLock(
    __in
    WDFINTERRUPT Interrupt
    )
{
    ((PFN_WDFINTERRUPTACQUIRELOCK) WdfFunctions[WdfInterruptAcquireLockTableIndex])(WdfDriverGlobals, Interrupt);
}

//
// WDF Function: WdfInterruptReleaseLock
//
typedef
WDFAPI
VOID
(*PFN_WDFINTERRUPTRELEASELOCK)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFINTERRUPT Interrupt
    );

VOID
FORCEINLINE
WdfInterruptReleaseLock(
    __in
    WDFINTERRUPT Interrupt
    )
{
    ((PFN_WDFINTERRUPTRELEASELOCK) WdfFunctions[WdfInterruptReleaseLockTableIndex])(WdfDriverGlobals, Interrupt);
}

//
// WDF Function: WdfInterruptEnable
//
typedef
WDFAPI
VOID
(*PFN_WDFINTERRUPTENABLE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFINTERRUPT Interrupt
    );

VOID
FORCEINLINE
WdfInterruptEnable(
    __in
    WDFINTERRUPT Interrupt
    )
{
    ((PFN_WDFINTERRUPTENABLE) WdfFunctions[WdfInterruptEnableTableIndex])(WdfDriverGlobals, Interrupt);
}

//
// WDF Function: WdfInterruptDisable
//
typedef
WDFAPI
VOID
(*PFN_WDFINTERRUPTDISABLE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFINTERRUPT Interrupt
    );

VOID
FORCEINLINE
WdfInterruptDisable(
    __in
    WDFINTERRUPT Interrupt
    )
{
    ((PFN_WDFINTERRUPTDISABLE) WdfFunctions[WdfInterruptDisableTableIndex])(WdfDriverGlobals, Interrupt);
}

//
// WDF Function: WdfInterruptWdmGetInterrupt
//
typedef
WDFAPI
PKINTERRUPT
(*PFN_WDFINTERRUPTWDMGETINTERRUPT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFINTERRUPT Interrupt
    );

PKINTERRUPT
FORCEINLINE
WdfInterruptWdmGetInterrupt(
    __in
    WDFINTERRUPT Interrupt
    )
{
    return ((PFN_WDFINTERRUPTWDMGETINTERRUPT) WdfFunctions[WdfInterruptWdmGetInterruptTableIndex])(WdfDriverGlobals, Interrupt);
}

//
// WDF Function: WdfInterruptGetInfo
//
typedef
WDFAPI
VOID
(*PFN_WDFINTERRUPTGETINFO)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFINTERRUPT Interrupt,
    PWDF_INTERRUPT_INFO Info
    );

VOID
FORCEINLINE
WdfInterruptGetInfo(
    __in
    WDFINTERRUPT Interrupt,
    __out
    PWDF_INTERRUPT_INFO Info
    )
{
    ((PFN_WDFINTERRUPTGETINFO) WdfFunctions[WdfInterruptGetInfoTableIndex])(WdfDriverGlobals, Interrupt, Info);
}

//
// WDF Function: WdfInterruptSetPolicy
//
typedef
WDFAPI
VOID
(*PFN_WDFINTERRUPTSETPOLICY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFINTERRUPT Interrupt,
    IN WDF_INTERRUPT_POLICY Policy,
    IN WDF_INTERRUPT_PRIORITY Priority,
    IN KAFFINITY TargetProcessorSet
    );

VOID
FORCEINLINE
WdfInterruptSetPolicy(
    __in
    WDFINTERRUPT Interrupt,
    IN WDF_INTERRUPT_POLICY Policy,
    IN WDF_INTERRUPT_PRIORITY Priority,
    IN KAFFINITY TargetProcessorSet
    )
{
    ((PFN_WDFINTERRUPTSETPOLICY) WdfFunctions[WdfInterruptSetPolicyTableIndex])(WdfDriverGlobals, Interrupt, Policy, Priority, TargetProcessorSet);
}

//
// WDF Function: WdfInterruptGetDevice
//
typedef
WDFAPI
WDFDEVICE
(*PFN_WDFINTERRUPTGETDEVICE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFINTERRUPT Interrupt
    );

WDFDEVICE
FORCEINLINE
WdfInterruptGetDevice(
    __in
    WDFINTERRUPT Interrupt
    )
{
    return ((PFN_WDFINTERRUPTGETDEVICE) WdfFunctions[WdfInterruptGetDeviceTableIndex])(WdfDriverGlobals, Interrupt);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFINTERRUPT_H_

