/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    WdfDmaEnabler.h

Abstract:

    WDF DMA Enabler support

Environment:

    Kernel mode only.

Notes:

Revision History:

--*/

#ifndef _WDFDMAENABLER_H_
#define _WDFDMAENABLER_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)

typedef enum _WDF_DMA_PROFILE {
    WdfDmaProfileInvalid = 0,
    WdfDmaProfilePacket,
    WdfDmaProfileScatterGather,
    WdfDmaProfilePacket64,
    WdfDmaProfileScatterGather64,
    WdfDmaProfileScatterGatherDuplex,
    WdfDmaProfileScatterGather64Duplex,
} WDF_DMA_PROFILE;

typedef enum _WDF_DMA_DIRECTION {
    WdfDmaDirectionReadFromDevice = FALSE,
    WdfDmaDirectionWriteToDevice = TRUE,
} WDF_DMA_DIRECTION;



//
// DMA power event callbacks
//
typedef
NTSTATUS
(*PFN_WDF_DMA_ENABLER_FILL)(
    IN  WDFDMAENABLER DmaEnabler
    );

typedef
NTSTATUS
(*PFN_WDF_DMA_ENABLER_FLUSH)(
    IN  WDFDMAENABLER DmaEnabler
    );

typedef
NTSTATUS
(*PFN_WDF_DMA_ENABLER_ENABLE)(
    IN  WDFDMAENABLER DmaEnabler
    );

typedef
NTSTATUS
(*PFN_WDF_DMA_ENABLER_DISABLE)(
    IN  WDFDMAENABLER DmaEnabler
    );

typedef
NTSTATUS
(*PFN_WDF_DMA_ENABLER_SELFMANAGED_IO_START)(
    IN  WDFDMAENABLER DmaEnabler
    );

typedef
NTSTATUS
(*PFN_WDF_DMA_ENABLER_SELFMANAGED_IO_STOP)(
    IN  WDFDMAENABLER DmaEnabler
    );


#define  WDF_DMA_ENABLER_UNLIMITED_FRAGMENTS ((ULONG) -1)

typedef struct _WDF_DMA_ENABLER_CONFIG {
    //
    // Size of this structure in bytes
    //
    ULONG                Size;

    //
    // One of the above WDF_DMA_PROFILES
    //
    WDF_DMA_PROFILE      Profile;

    //
    // Maximum DMA Transfer handled in bytes.
    //
    size_t               MaximumLength;

    //
    // The various DMA PnP/Power event callbacks
    //
    PFN_WDF_DMA_ENABLER_FILL                  EvtDmaEnablerFill;
    PFN_WDF_DMA_ENABLER_FLUSH                 EvtDmaEnablerFlush;
    PFN_WDF_DMA_ENABLER_DISABLE               EvtDmaEnablerDisable;
    PFN_WDF_DMA_ENABLER_ENABLE                EvtDmaEnablerEnable;
    PFN_WDF_DMA_ENABLER_SELFMANAGED_IO_START  EvtDmaEnablerSelfManagedIoStart;
    PFN_WDF_DMA_ENABLER_SELFMANAGED_IO_STOP   EvtDmaEnablerSelfManagedIoStop;

} WDF_DMA_ENABLER_CONFIG, *PWDF_DMA_ENABLER_CONFIG;

VOID
FORCEINLINE
WDF_DMA_ENABLER_CONFIG_INIT(
    IN  PWDF_DMA_ENABLER_CONFIG  Config,
    IN  WDF_DMA_PROFILE          Profile,
    IN  size_t                   MaximumLength
    )
{
    RtlZeroMemory(Config, sizeof(WDF_DMA_ENABLER_CONFIG));

    Config->Size = sizeof(WDF_DMA_ENABLER_CONFIG);
    Config->Profile = Profile;
    Config->MaximumLength = MaximumLength;
}

//
// WDF Function: WdfDmaEnablerCreate
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDMAENABLERCREATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PWDF_DMA_ENABLER_CONFIG Config,
    PWDF_OBJECT_ATTRIBUTES Attributes,
    WDFDMAENABLER* DmaEnablerHandle
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDmaEnablerCreate(
    __in
    WDFDEVICE Device,
    __in
    PWDF_DMA_ENABLER_CONFIG Config,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES Attributes,
    __out
    WDFDMAENABLER* DmaEnablerHandle
    )
{
    return ((PFN_WDFDMAENABLERCREATE) WdfFunctions[WdfDmaEnablerCreateTableIndex])(WdfDriverGlobals, Device, Config, Attributes, DmaEnablerHandle);
}

//
// WDF Function: WdfDmaEnablerGetMaximumLength
//
typedef
WDFAPI
size_t
(*PFN_WDFDMAENABLERGETMAXIMUMLENGTH)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDMAENABLER DmaEnabler
    );

size_t
FORCEINLINE
WdfDmaEnablerGetMaximumLength(
    __in
    WDFDMAENABLER DmaEnabler
    )
{
    return ((PFN_WDFDMAENABLERGETMAXIMUMLENGTH) WdfFunctions[WdfDmaEnablerGetMaximumLengthTableIndex])(WdfDriverGlobals, DmaEnabler);
}

//
// WDF Function: WdfDmaEnablerGetMaximumScatterGatherElements
//
typedef
WDFAPI
size_t
(*PFN_WDFDMAENABLERGETMAXIMUMSCATTERGATHERELEMENTS)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDMAENABLER DmaEnabler
    );

size_t
FORCEINLINE
WdfDmaEnablerGetMaximumScatterGatherElements(
    __in
    WDFDMAENABLER DmaEnabler
    )
{
    return ((PFN_WDFDMAENABLERGETMAXIMUMSCATTERGATHERELEMENTS) WdfFunctions[WdfDmaEnablerGetMaximumScatterGatherElementsTableIndex])(WdfDriverGlobals, DmaEnabler);
}

//
// WDF Function: WdfDmaEnablerSetMaximumScatterGatherElements
//
typedef
WDFAPI
VOID
(*PFN_WDFDMAENABLERSETMAXIMUMSCATTERGATHERELEMENTS)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDMAENABLER DmaEnabler,
    IN size_t MaximumFragments
    );

VOID
FORCEINLINE
WdfDmaEnablerSetMaximumScatterGatherElements(
    __in
    WDFDMAENABLER DmaEnabler,
    IN size_t MaximumFragments
    )
{
    ((PFN_WDFDMAENABLERSETMAXIMUMSCATTERGATHERELEMENTS) WdfFunctions[WdfDmaEnablerSetMaximumScatterGatherElementsTableIndex])(WdfDriverGlobals, DmaEnabler, MaximumFragments);
}

//
// WDF Function: WdfDmaEnablerGetFragmentLength
//
typedef
WDFAPI
size_t
(*PFN_WDFDMAENABLERGETFRAGMENTLENGTH)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDMAENABLER DmaEnabler,
    IN WDF_DMA_DIRECTION DmaDirection
    );

size_t
FORCEINLINE
WdfDmaEnablerGetFragmentLength(
    __in
    WDFDMAENABLER DmaEnabler,
    IN WDF_DMA_DIRECTION DmaDirection
    )
{
    return ((PFN_WDFDMAENABLERGETFRAGMENTLENGTH) WdfFunctions[WdfDmaEnablerGetFragmentLengthTableIndex])(WdfDriverGlobals, DmaEnabler, DmaDirection);
}

//
// WDF Function: WdfDmaEnablerWdmGetDmaAdapter
//
typedef
WDFAPI
PDMA_ADAPTER
(*PFN_WDFDMAENABLERWDMGETDMAADAPTER)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDMAENABLER DmaEnabler,
    IN WDF_DMA_DIRECTION DmaDirection
    );

PDMA_ADAPTER
FORCEINLINE
WdfDmaEnablerWdmGetDmaAdapter(
    __in
    WDFDMAENABLER DmaEnabler,
    IN WDF_DMA_DIRECTION DmaDirection
    )
{
    return ((PFN_WDFDMAENABLERWDMGETDMAADAPTER) WdfFunctions[WdfDmaEnablerWdmGetDmaAdapterTableIndex])(WdfDriverGlobals, DmaEnabler, DmaDirection);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFDMAENABLER_H_

