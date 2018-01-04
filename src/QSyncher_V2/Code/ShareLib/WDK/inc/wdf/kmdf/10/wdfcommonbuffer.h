/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    WdfCommonBuffer.h

Abstract:

    WDF CommonBuffer support

Environment:

    Kernel mode only.

Notes:

Revision History:

--*/

#ifndef _WDFCOMMONBUFFER_H_
#define _WDFCOMMONBUFFER_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)



typedef struct _WDF_COMMON_BUFFER_CONFIG {
    //
    // Size of this structure in bytes
    //
    ULONG   Size;

    //
    // Alignment requirement of the buffer address
    //
    ULONG   AlignmentRequirement;

} WDF_COMMON_BUFFER_CONFIG, *PWDF_COMMON_BUFFER_CONFIG;

VOID
FORCEINLINE
WDF_COMMON_BUFFER_CONFIG_INIT(
    IN  PWDF_COMMON_BUFFER_CONFIG   Config,
    IN  ULONG                       AlignmentRequirement
    )
{
    RtlZeroMemory(Config, sizeof(WDF_COMMON_BUFFER_CONFIG));

    Config->Size = sizeof(WDF_COMMON_BUFFER_CONFIG);
    Config->AlignmentRequirement = AlignmentRequirement;
}

//
// WDF Function: WdfCommonBufferCreate
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFCOMMONBUFFERCREATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDMAENABLER DmaEnabler,
    IN size_t Length,
    PWDF_OBJECT_ATTRIBUTES Attributes,
    WDFCOMMONBUFFER* CommonBuffer
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfCommonBufferCreate(
    __in
    WDFDMAENABLER DmaEnabler,
    IN size_t Length,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES Attributes,
    __out
    WDFCOMMONBUFFER* CommonBuffer
    )
{
    return ((PFN_WDFCOMMONBUFFERCREATE) WdfFunctions[WdfCommonBufferCreateTableIndex])(WdfDriverGlobals, DmaEnabler, Length, Attributes, CommonBuffer);
}

//
// WDF Function: WdfCommonBufferCreateWithConfig
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFCOMMONBUFFERCREATEWITHCONFIG)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDMAENABLER DmaEnabler,
    IN size_t Length,
    PWDF_COMMON_BUFFER_CONFIG Config,
    PWDF_OBJECT_ATTRIBUTES Attributes,
    WDFCOMMONBUFFER* CommonBuffer
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfCommonBufferCreateWithConfig(
    __in
    WDFDMAENABLER DmaEnabler,
    IN size_t Length,
    __in
    PWDF_COMMON_BUFFER_CONFIG Config,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES Attributes,
    __out
    WDFCOMMONBUFFER* CommonBuffer
    )
{
    return ((PFN_WDFCOMMONBUFFERCREATEWITHCONFIG) WdfFunctions[WdfCommonBufferCreateWithConfigTableIndex])(WdfDriverGlobals, DmaEnabler, Length, Config, Attributes, CommonBuffer);
}

//
// WDF Function: WdfCommonBufferGetAlignedVirtualAddress
//
typedef
WDFAPI
PVOID
(*PFN_WDFCOMMONBUFFERGETALIGNEDVIRTUALADDRESS)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCOMMONBUFFER CommonBuffer
    );

PVOID
FORCEINLINE
WdfCommonBufferGetAlignedVirtualAddress(
    __in
    WDFCOMMONBUFFER CommonBuffer
    )
{
    return ((PFN_WDFCOMMONBUFFERGETALIGNEDVIRTUALADDRESS) WdfFunctions[WdfCommonBufferGetAlignedVirtualAddressTableIndex])(WdfDriverGlobals, CommonBuffer);
}

//
// WDF Function: WdfCommonBufferGetAlignedLogicalAddress
//
typedef
WDFAPI
PHYSICAL_ADDRESS
(*PFN_WDFCOMMONBUFFERGETALIGNEDLOGICALADDRESS)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCOMMONBUFFER CommonBuffer
    );

PHYSICAL_ADDRESS
FORCEINLINE
WdfCommonBufferGetAlignedLogicalAddress(
    __in
    WDFCOMMONBUFFER CommonBuffer
    )
{
    return ((PFN_WDFCOMMONBUFFERGETALIGNEDLOGICALADDRESS) WdfFunctions[WdfCommonBufferGetAlignedLogicalAddressTableIndex])(WdfDriverGlobals, CommonBuffer);
}

//
// WDF Function: WdfCommonBufferGetLength
//
typedef
WDFAPI
size_t
(*PFN_WDFCOMMONBUFFERGETLENGTH)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCOMMONBUFFER CommonBuffer
    );

size_t
FORCEINLINE
WdfCommonBufferGetLength(
    __in
    WDFCOMMONBUFFER CommonBuffer
    )
{
    return ((PFN_WDFCOMMONBUFFERGETLENGTH) WdfFunctions[WdfCommonBufferGetLengthTableIndex])(WdfDriverGlobals, CommonBuffer);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFCOMMONBUFFER_H_

