/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    WdfCollection.h

Abstract:

    This is the interface to the collection object

Environment:

    kernel mode only

Revision History:

--*/

#ifndef _WDFCOLLECTION_H_
#define _WDFCOLLECTION_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)



//
// WDF Function: WdfCollectionCreate
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFCOLLECTIONCREATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDF_OBJECT_ATTRIBUTES CollectionAttributes,
    WDFCOLLECTION* Collection
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfCollectionCreate(
    __in_opt
    PWDF_OBJECT_ATTRIBUTES CollectionAttributes,
    __out
    WDFCOLLECTION* Collection
    )
{
    return ((PFN_WDFCOLLECTIONCREATE) WdfFunctions[WdfCollectionCreateTableIndex])(WdfDriverGlobals, CollectionAttributes, Collection);
}

//
// WDF Function: WdfCollectionGetCount
//
typedef
WDFAPI
ULONG
(*PFN_WDFCOLLECTIONGETCOUNT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCOLLECTION Collection
    );

ULONG
FORCEINLINE
WdfCollectionGetCount(
    __in
    WDFCOLLECTION Collection
    )
{
    return ((PFN_WDFCOLLECTIONGETCOUNT) WdfFunctions[WdfCollectionGetCountTableIndex])(WdfDriverGlobals, Collection);
}

//
// WDF Function: WdfCollectionAdd
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFCOLLECTIONADD)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCOLLECTION Collection,
    WDFOBJECT Object
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfCollectionAdd(
    __in
    WDFCOLLECTION Collection,
    __in
    WDFOBJECT Object
    )
{
    return ((PFN_WDFCOLLECTIONADD) WdfFunctions[WdfCollectionAddTableIndex])(WdfDriverGlobals, Collection, Object);
}

//
// WDF Function: WdfCollectionRemove
//
typedef
WDFAPI
VOID
(*PFN_WDFCOLLECTIONREMOVE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCOLLECTION Collection,
    WDFOBJECT Item
    );

VOID
FORCEINLINE
WdfCollectionRemove(
    __in
    WDFCOLLECTION Collection,
    __in
    WDFOBJECT Item
    )
{
    ((PFN_WDFCOLLECTIONREMOVE) WdfFunctions[WdfCollectionRemoveTableIndex])(WdfDriverGlobals, Collection, Item);
}

//
// WDF Function: WdfCollectionRemoveItem
//
typedef
WDFAPI
VOID
(*PFN_WDFCOLLECTIONREMOVEITEM)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCOLLECTION Collection,
    IN ULONG Index
    );

VOID
FORCEINLINE
WdfCollectionRemoveItem(
    __in
    WDFCOLLECTION Collection,
    IN ULONG Index
    )
{
    ((PFN_WDFCOLLECTIONREMOVEITEM) WdfFunctions[WdfCollectionRemoveItemTableIndex])(WdfDriverGlobals, Collection, Index);
}

//
// WDF Function: WdfCollectionGetItem
//
typedef
WDFAPI
WDFOBJECT
(*PFN_WDFCOLLECTIONGETITEM)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCOLLECTION Collection,
    IN ULONG Index
    );

WDFOBJECT
FORCEINLINE
WdfCollectionGetItem(
    __in
    WDFCOLLECTION Collection,
    IN ULONG Index
    )
{
    return ((PFN_WDFCOLLECTIONGETITEM) WdfFunctions[WdfCollectionGetItemTableIndex])(WdfDriverGlobals, Collection, Index);
}

//
// WDF Function: WdfCollectionGetFirstItem
//
typedef
WDFAPI
WDFOBJECT
(*PFN_WDFCOLLECTIONGETFIRSTITEM)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCOLLECTION Collection
    );

WDFOBJECT
FORCEINLINE
WdfCollectionGetFirstItem(
    __in
    WDFCOLLECTION Collection
    )
{
    return ((PFN_WDFCOLLECTIONGETFIRSTITEM) WdfFunctions[WdfCollectionGetFirstItemTableIndex])(WdfDriverGlobals, Collection);
}

//
// WDF Function: WdfCollectionGetLastItem
//
typedef
WDFAPI
WDFOBJECT
(*PFN_WDFCOLLECTIONGETLASTITEM)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCOLLECTION Collection
    );

WDFOBJECT
FORCEINLINE
WdfCollectionGetLastItem(
    __in
    WDFCOLLECTION Collection
    )
{
    return ((PFN_WDFCOLLECTIONGETLASTITEM) WdfFunctions[WdfCollectionGetLastItemTableIndex])(WdfDriverGlobals, Collection);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFCOLLECTION_H_

