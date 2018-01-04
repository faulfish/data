/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    WdfRegistry.h

Abstract:

    This is the interface to registry access.

Environment:

    kernel mode only

Revision History:

--*/

#ifndef _WDFREGISTRY_H_
#define _WDFREGISTRY_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)



//
// WDF Function: WdfRegistryOpenKey
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREGISTRYOPENKEY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFKEY ParentKey,
    PCUNICODE_STRING KeyName,
    IN ACCESS_MASK DesiredAccess,
    PWDF_OBJECT_ATTRIBUTES KeyAttributes,
    WDFKEY* Key
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRegistryOpenKey(
    __in_opt
    WDFKEY ParentKey,
    __in
    PCUNICODE_STRING KeyName,
    IN ACCESS_MASK DesiredAccess,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES KeyAttributes,
    __out
    WDFKEY* Key
    )
{
    return ((PFN_WDFREGISTRYOPENKEY) WdfFunctions[WdfRegistryOpenKeyTableIndex])(WdfDriverGlobals, ParentKey, KeyName, DesiredAccess, KeyAttributes, Key);
}

//
// WDF Function: WdfRegistryCreateKey
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREGISTRYCREATEKEY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFKEY ParentKey,
    PCUNICODE_STRING KeyName,
    IN ACCESS_MASK DesiredAccess,
    IN ULONG CreateOptions,
    PULONG CreateDisposition,
    PWDF_OBJECT_ATTRIBUTES KeyAttributes,
    WDFKEY* Key
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRegistryCreateKey(
    __in_opt
    WDFKEY ParentKey,
    __in
    PCUNICODE_STRING KeyName,
    IN ACCESS_MASK DesiredAccess,
    IN ULONG CreateOptions,
    __out_opt
    PULONG CreateDisposition,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES KeyAttributes,
    __out
    WDFKEY* Key
    )
{
    return ((PFN_WDFREGISTRYCREATEKEY) WdfFunctions[WdfRegistryCreateKeyTableIndex])(WdfDriverGlobals, ParentKey, KeyName, DesiredAccess, CreateOptions, CreateDisposition, KeyAttributes, Key);
}

//
// WDF Function: WdfRegistryClose
//
typedef
WDFAPI
VOID
(*PFN_WDFREGISTRYCLOSE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFKEY Key
    );

VOID
FORCEINLINE
WdfRegistryClose(
    __in
    WDFKEY Key
    )
{
    ((PFN_WDFREGISTRYCLOSE) WdfFunctions[WdfRegistryCloseTableIndex])(WdfDriverGlobals, Key);
}

//
// WDF Function: WdfRegistryWdmGetHandle
//
typedef
WDFAPI
HANDLE
(*PFN_WDFREGISTRYWDMGETHANDLE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFKEY Key
    );

HANDLE
FORCEINLINE
WdfRegistryWdmGetHandle(
    __in
    WDFKEY Key
    )
{
    return ((PFN_WDFREGISTRYWDMGETHANDLE) WdfFunctions[WdfRegistryWdmGetHandleTableIndex])(WdfDriverGlobals, Key);
}

//
// WDF Function: WdfRegistryRemoveKey
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREGISTRYREMOVEKEY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFKEY Key
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRegistryRemoveKey(
    __in
    WDFKEY Key
    )
{
    return ((PFN_WDFREGISTRYREMOVEKEY) WdfFunctions[WdfRegistryRemoveKeyTableIndex])(WdfDriverGlobals, Key);
}

//
// WDF Function: WdfRegistryRemoveValue
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREGISTRYREMOVEVALUE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFKEY Key,
    PCUNICODE_STRING ValueName
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRegistryRemoveValue(
    __in
    WDFKEY Key,
    __in
    PCUNICODE_STRING ValueName
    )
{
    return ((PFN_WDFREGISTRYREMOVEVALUE) WdfFunctions[WdfRegistryRemoveValueTableIndex])(WdfDriverGlobals, Key, ValueName);
}

//
// WDF Function: WdfRegistryQueryValue
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREGISTRYQUERYVALUE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFKEY Key,
    PCUNICODE_STRING ValueName,
    IN ULONG ValueLength,
    PVOID Value,
    PULONG ValueLengthQueried,
    PULONG ValueType
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRegistryQueryValue(
    __in
    WDFKEY Key,
    __in
    PCUNICODE_STRING ValueName,
    IN ULONG ValueLength,
    __out_bcount_opt( ValueLength)
    PVOID Value,
    __out_opt
    PULONG ValueLengthQueried,
    __out_opt
    PULONG ValueType
    )
{
    return ((PFN_WDFREGISTRYQUERYVALUE) WdfFunctions[WdfRegistryQueryValueTableIndex])(WdfDriverGlobals, Key, ValueName, ValueLength, Value, ValueLengthQueried, ValueType);
}

//
// WDF Function: WdfRegistryQueryMemory
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREGISTRYQUERYMEMORY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFKEY Key,
    PCUNICODE_STRING ValueName,
    IN POOL_TYPE PoolType,
    PWDF_OBJECT_ATTRIBUTES MemoryAttributes,
    WDFMEMORY* Memory,
    PULONG ValueType
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRegistryQueryMemory(
    __in
    WDFKEY Key,
    __in
    PCUNICODE_STRING ValueName,
    IN POOL_TYPE PoolType,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES MemoryAttributes,
    __out
    WDFMEMORY* Memory,
    __out_opt
    PULONG ValueType
    )
{
    return ((PFN_WDFREGISTRYQUERYMEMORY) WdfFunctions[WdfRegistryQueryMemoryTableIndex])(WdfDriverGlobals, Key, ValueName, PoolType, MemoryAttributes, Memory, ValueType);
}

//
// WDF Function: WdfRegistryQueryMultiString
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREGISTRYQUERYMULTISTRING)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFKEY Key,
    PCUNICODE_STRING ValueName,
    PWDF_OBJECT_ATTRIBUTES StringsAttributes,
    WDFCOLLECTION Collection
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRegistryQueryMultiString(
    __in
    WDFKEY Key,
    __in
    PCUNICODE_STRING ValueName,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES StringsAttributes,
    __in
    WDFCOLLECTION Collection
    )
{
    return ((PFN_WDFREGISTRYQUERYMULTISTRING) WdfFunctions[WdfRegistryQueryMultiStringTableIndex])(WdfDriverGlobals, Key, ValueName, StringsAttributes, Collection);
}

//
// WDF Function: WdfRegistryQueryUnicodeString
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREGISTRYQUERYUNICODESTRING)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFKEY Key,
    PCUNICODE_STRING ValueName,
    PUSHORT ValueByteLength,
    PUNICODE_STRING Value
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRegistryQueryUnicodeString(
    __in
    WDFKEY Key,
    __in
    PCUNICODE_STRING ValueName,
    __out_opt
    PUSHORT ValueByteLength,
    __inout_opt
    PUNICODE_STRING Value
    )
{
    return ((PFN_WDFREGISTRYQUERYUNICODESTRING) WdfFunctions[WdfRegistryQueryUnicodeStringTableIndex])(WdfDriverGlobals, Key, ValueName, ValueByteLength, Value);
}

//
// WDF Function: WdfRegistryQueryString
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREGISTRYQUERYSTRING)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFKEY Key,
    PCUNICODE_STRING ValueName,
    IN WDFSTRING String
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRegistryQueryString(
    __in
    WDFKEY Key,
    __in
    PCUNICODE_STRING ValueName,
    IN WDFSTRING String
    )
{
    return ((PFN_WDFREGISTRYQUERYSTRING) WdfFunctions[WdfRegistryQueryStringTableIndex])(WdfDriverGlobals, Key, ValueName, String);
}

//
// WDF Function: WdfRegistryQueryULong
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREGISTRYQUERYULONG)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFKEY Key,
    PCUNICODE_STRING ValueName,
    PULONG Value
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRegistryQueryULong(
    __in
    WDFKEY Key,
    __in
    PCUNICODE_STRING ValueName,
    __out
    PULONG Value
    )
{
    return ((PFN_WDFREGISTRYQUERYULONG) WdfFunctions[WdfRegistryQueryULongTableIndex])(WdfDriverGlobals, Key, ValueName, Value);
}

//
// WDF Function: WdfRegistryAssignValue
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREGISTRYASSIGNVALUE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFKEY Key,
    PCUNICODE_STRING ValueName,
    IN ULONG ValueType,
    IN ULONG ValueLength,
    PVOID Value
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRegistryAssignValue(
    __in
    WDFKEY Key,
    __in
    PCUNICODE_STRING ValueName,
    IN ULONG ValueType,
    IN ULONG ValueLength,
    __in_ecount( ValueLength)
    PVOID Value
    )
{
    return ((PFN_WDFREGISTRYASSIGNVALUE) WdfFunctions[WdfRegistryAssignValueTableIndex])(WdfDriverGlobals, Key, ValueName, ValueType, ValueLength, Value);
}

//
// WDF Function: WdfRegistryAssignMemory
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREGISTRYASSIGNMEMORY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFKEY Key,
    PCUNICODE_STRING ValueName,
    IN ULONG ValueType,
    IN WDFMEMORY Memory,
    PWDFMEMORY_OFFSET MemoryOffsets
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRegistryAssignMemory(
    __in
    WDFKEY Key,
    __in
    PCUNICODE_STRING ValueName,
    IN ULONG ValueType,
    IN WDFMEMORY Memory,
    __in_opt
    PWDFMEMORY_OFFSET MemoryOffsets
    )
{
    return ((PFN_WDFREGISTRYASSIGNMEMORY) WdfFunctions[WdfRegistryAssignMemoryTableIndex])(WdfDriverGlobals, Key, ValueName, ValueType, Memory, MemoryOffsets);
}

//
// WDF Function: WdfRegistryAssignMultiString
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREGISTRYASSIGNMULTISTRING)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFKEY Key,
    PCUNICODE_STRING ValueName,
    IN WDFCOLLECTION StringsCollection
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRegistryAssignMultiString(
    __in
    WDFKEY Key,
    __in
    PCUNICODE_STRING ValueName,
    IN WDFCOLLECTION StringsCollection
    )
{
    return ((PFN_WDFREGISTRYASSIGNMULTISTRING) WdfFunctions[WdfRegistryAssignMultiStringTableIndex])(WdfDriverGlobals, Key, ValueName, StringsCollection);
}

//
// WDF Function: WdfRegistryAssignUnicodeString
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREGISTRYASSIGNUNICODESTRING)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFKEY Key,
    PCUNICODE_STRING ValueName,
    PCUNICODE_STRING Value
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRegistryAssignUnicodeString(
    __in
    WDFKEY Key,
    __in
    PCUNICODE_STRING ValueName,
    __in
    PCUNICODE_STRING Value
    )
{
    return ((PFN_WDFREGISTRYASSIGNUNICODESTRING) WdfFunctions[WdfRegistryAssignUnicodeStringTableIndex])(WdfDriverGlobals, Key, ValueName, Value);
}

//
// WDF Function: WdfRegistryAssignString
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREGISTRYASSIGNSTRING)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFKEY Key,
    PCUNICODE_STRING ValueName,
    IN WDFSTRING String
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRegistryAssignString(
    __in
    WDFKEY Key,
    __in
    PCUNICODE_STRING ValueName,
    IN WDFSTRING String
    )
{
    return ((PFN_WDFREGISTRYASSIGNSTRING) WdfFunctions[WdfRegistryAssignStringTableIndex])(WdfDriverGlobals, Key, ValueName, String);
}

//
// WDF Function: WdfRegistryAssignULong
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFREGISTRYASSIGNULONG)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFKEY Key,
    PCUNICODE_STRING ValueName,
    IN ULONG Value
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfRegistryAssignULong(
    __in
    WDFKEY Key,
    __in
    PCUNICODE_STRING ValueName,
    IN ULONG Value
    )
{
    return ((PFN_WDFREGISTRYASSIGNULONG) WdfFunctions[WdfRegistryAssignULongTableIndex])(WdfDriverGlobals, Key, ValueName, Value);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFREGISTRY_H_

