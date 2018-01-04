/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    WdfMemory.h

Abstract:

    Contains prototypes for managing memory objects in the driver frameworks.

Author:

Environment:

    kernel mode only

Revision History:

--*/

#ifndef _WDFMEMORY_H_
#define _WDFMEMORY_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)

typedef enum _WDF_MEMORY_DESCRIPTOR_TYPE {
    WdfMemoryDescriptorTypeInvalid = 0,
    WdfMemoryDescriptorTypeBuffer,
    WdfMemoryDescriptorTypeMdl,
    WdfMemoryDescriptorTypeHandle,
} WDF_MEMORY_DESCRIPTOR_TYPE;



typedef struct _WDFMEMORY_OFFSET {
    //
    // Offset into the WDFMEMORY that the operation should start at.
    //
    size_t BufferOffset;

    //
    // Number of bytes that the operation should access.  If 0, the entire
    // length of the WDFMEMORY buffer will be used in the operation or ignored
    // depending on the API.
    //
    size_t BufferLength;

} WDFMEMORY_OFFSET, *PWDFMEMORY_OFFSET;

typedef struct _WDF_MEMORY_DESCRIPTOR {
    WDF_MEMORY_DESCRIPTOR_TYPE Type;

    union {
        struct {
            PVOID Buffer;

            ULONG Length;
        } BufferType;

        struct {
            PMDL Mdl;

            ULONG BufferLength;
        } MdlType;

        struct {
            WDFMEMORY Memory;
            PWDFMEMORY_OFFSET Offsets;
        } HandleType;
    } u;

} WDF_MEMORY_DESCRIPTOR, *PWDF_MEMORY_DESCRIPTOR;

VOID
FORCEINLINE
WDF_MEMORY_DESCRIPTOR_INIT_BUFFER(
    OUT PWDF_MEMORY_DESCRIPTOR Descriptor,
    IN PVOID Buffer,
    IN ULONG BufferLength
    )
{
    RtlZeroMemory(Descriptor, sizeof(WDF_MEMORY_DESCRIPTOR));

    Descriptor->Type = WdfMemoryDescriptorTypeBuffer;
    Descriptor->u.BufferType.Buffer = Buffer;
    Descriptor->u.BufferType.Length = BufferLength;
}

VOID
FORCEINLINE
WDF_MEMORY_DESCRIPTOR_INIT_MDL(
    OUT PWDF_MEMORY_DESCRIPTOR Descriptor,
    IN PMDL Mdl,
    IN ULONG BufferLength
    )
{
    RtlZeroMemory(Descriptor, sizeof(WDF_MEMORY_DESCRIPTOR));

    Descriptor->Type = WdfMemoryDescriptorTypeMdl;
    Descriptor->u.MdlType.Mdl = Mdl;
    Descriptor->u.MdlType.BufferLength = BufferLength;
}

VOID
FORCEINLINE
WDF_MEMORY_DESCRIPTOR_INIT_HANDLE(
    OUT PWDF_MEMORY_DESCRIPTOR Descriptor,
    IN WDFMEMORY Memory,
    IN OPTIONAL PWDFMEMORY_OFFSET Offsets
    )
{
    RtlZeroMemory(Descriptor, sizeof(WDF_MEMORY_DESCRIPTOR));

    Descriptor->Type = WdfMemoryDescriptorTypeHandle;
    Descriptor->u.HandleType.Memory = Memory;
    Descriptor->u.HandleType.Offsets = Offsets;
}

//
// WDF Function: WdfMemoryCreate
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFMEMORYCREATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDF_OBJECT_ATTRIBUTES Attributes,
    IN POOL_TYPE PoolType,
    IN OPTIONAL ULONG PoolTag,
    IN size_t BufferSize,
    WDFMEMORY* Memory,
    PVOID* Buffer
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfMemoryCreate(
    __in_opt
    PWDF_OBJECT_ATTRIBUTES Attributes,
    IN POOL_TYPE PoolType,
    IN OPTIONAL ULONG PoolTag,
    IN size_t BufferSize,
    __out
    WDFMEMORY* Memory,
    __out_opt
    PVOID* Buffer
    )
{
    return ((PFN_WDFMEMORYCREATE) WdfFunctions[WdfMemoryCreateTableIndex])(WdfDriverGlobals, Attributes, PoolType, PoolTag, BufferSize, Memory, Buffer);
}

//
// WDF Function: WdfMemoryCreatePreallocated
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFMEMORYCREATEPREALLOCATED)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDF_OBJECT_ATTRIBUTES Attributes,
    PVOID Buffer,
    IN size_t BufferSize,
    WDFMEMORY* Memory
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfMemoryCreatePreallocated(
    __in_opt
    PWDF_OBJECT_ATTRIBUTES Attributes,
    __in
    PVOID Buffer,
    IN size_t BufferSize,
    __out
    WDFMEMORY* Memory
    )
{
    return ((PFN_WDFMEMORYCREATEPREALLOCATED) WdfFunctions[WdfMemoryCreatePreallocatedTableIndex])(WdfDriverGlobals, Attributes, Buffer, BufferSize, Memory);
}

//
// WDF Function: WdfMemoryGetBuffer
//
typedef
WDFAPI
PVOID
(*PFN_WDFMEMORYGETBUFFER)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFMEMORY Memory,
    size_t* BufferSize
    );

PVOID
FORCEINLINE
WdfMemoryGetBuffer(
    __in
    WDFMEMORY Memory,
    __out_opt
    size_t* BufferSize
    )
{
    return ((PFN_WDFMEMORYGETBUFFER) WdfFunctions[WdfMemoryGetBufferTableIndex])(WdfDriverGlobals, Memory, BufferSize);
}

//
// WDF Function: WdfMemoryAssignBuffer
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFMEMORYASSIGNBUFFER)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFMEMORY Memory,
    PVOID Buffer,
    IN size_t BufferSize
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfMemoryAssignBuffer(
    __in
    WDFMEMORY Memory,
    __in
    PVOID Buffer,
    IN size_t BufferSize
    )
{
    return ((PFN_WDFMEMORYASSIGNBUFFER) WdfFunctions[WdfMemoryAssignBufferTableIndex])(WdfDriverGlobals, Memory, Buffer, BufferSize);
}

//
// WDF Function: WdfMemoryCopyToBuffer
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFMEMORYCOPYTOBUFFER)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFMEMORY SourceMemory,
    IN size_t SourceOffset,
    PVOID Buffer,
    IN size_t NumBytesToCopyTo
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfMemoryCopyToBuffer(
    __in
    WDFMEMORY SourceMemory,
    IN size_t SourceOffset,
    __out_bcount( NumBytesToCopyTo )
    PVOID Buffer,
    IN size_t NumBytesToCopyTo
    )
{
    return ((PFN_WDFMEMORYCOPYTOBUFFER) WdfFunctions[WdfMemoryCopyToBufferTableIndex])(WdfDriverGlobals, SourceMemory, SourceOffset, Buffer, NumBytesToCopyTo);
}

//
// WDF Function: WdfMemoryCopyFromBuffer
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFMEMORYCOPYFROMBUFFER)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFMEMORY DestinationMemory,
    IN size_t DestinationOffset,
    PVOID Buffer,
    IN size_t NumBytesToCopyFrom
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfMemoryCopyFromBuffer(
    __in
    WDFMEMORY DestinationMemory,
    IN size_t DestinationOffset,
    __in
    PVOID Buffer,
    IN size_t NumBytesToCopyFrom
    )
{
    return ((PFN_WDFMEMORYCOPYFROMBUFFER) WdfFunctions[WdfMemoryCopyFromBufferTableIndex])(WdfDriverGlobals, DestinationMemory, DestinationOffset, Buffer, NumBytesToCopyFrom);
}

//
// WDF Function: WdfLookasideListCreate
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFLOOKASIDELISTCREATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDF_OBJECT_ATTRIBUTES LookasideAttributes,
    IN size_t BufferSize,
    IN POOL_TYPE PoolType,
    PWDF_OBJECT_ATTRIBUTES MemoryAttributes,
    IN OPTIONAL ULONG PoolTag,
    WDFLOOKASIDE* Lookaside
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfLookasideListCreate(
    __in_opt
    PWDF_OBJECT_ATTRIBUTES LookasideAttributes,
    IN size_t BufferSize,
    IN POOL_TYPE PoolType,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES MemoryAttributes,
    IN OPTIONAL ULONG PoolTag,
    __out
    WDFLOOKASIDE* Lookaside
    )
{
    return ((PFN_WDFLOOKASIDELISTCREATE) WdfFunctions[WdfLookasideListCreateTableIndex])(WdfDriverGlobals, LookasideAttributes, BufferSize, PoolType, MemoryAttributes, PoolTag, Lookaside);
}

//
// WDF Function: WdfMemoryCreateFromLookaside
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFMEMORYCREATEFROMLOOKASIDE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFLOOKASIDE Lookaside,
    WDFMEMORY* Memory
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfMemoryCreateFromLookaside(
    __in
    WDFLOOKASIDE Lookaside,
    __out
    WDFMEMORY* Memory
    )
{
    return ((PFN_WDFMEMORYCREATEFROMLOOKASIDE) WdfFunctions[WdfMemoryCreateFromLookasideTableIndex])(WdfDriverGlobals, Lookaside, Memory);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFMEMORY_H_

