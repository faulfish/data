/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    WdfChildList.hpp

Abstract:

    This module defines the set of APIs to manipulate a WDFCHILDLIST handle.  A
    WDFCHILDLIST handle maintains a list of descriptions representing
    dynamically enumerated child devices.

Environment:

    kernel mode only

Revision History:

--*/

#ifndef _WDFCHILDLIST_H_
#define _WDFCHILDLIST_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)

typedef enum _WDF_CHILD_LIST_RETRIEVE_DEVICE_STATUS {
    WdfChildListRetrieveDeviceUndefined = 0,
    WdfChildListRetrieveDeviceSuccess,
    WdfChildListRetrieveDeviceNotYetCreated,
    WdfChildListRetrieveDeviceNoSuchDevice,
} WDF_CHILD_LIST_RETRIEVE_DEVICE_STATUS, *PWDF_CHILD_LIST_RETRIEVE_DEVICE_STATUS;

typedef enum _WDF_RETRIEVE_CHILD_FLAGS {
    WdfRetrieveUnspecified = 0x0000,
    WdfRetrievePresentChildren = 0x0001,
    WdfRetrieveMissingChildren = 0x0002,
    WdfRetrievePendingChildren = 0x0004,
    WdfRetrieveAddedChildren = (WdfRetrievePresentChildren | WdfRetrievePendingChildren),
    WdfRetrieveAllChildren = (WdfRetrievePresentChildren | WdfRetrievePendingChildren | WdfRetrieveMissingChildren),
} WDF_RETRIEVE_CHILD_FLAGS;



typedef struct _WDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER {
    //
    // Size in bytes of the entire description, including this header.
    //
    // Same value as WDF_CHILD_LIST_CONFIG::IdentificationDescriptionSize
    // Used as a sanity check.
    //
    ULONG IdentificationDescriptionSize;
}   WDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER,
  *PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER;

VOID
FORCEINLINE
WDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER_INIT(
    PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER Header,
    ULONG IdentificationDescriptionSize
    )
{
    RtlZeroMemory(Header, IdentificationDescriptionSize);
    Header->IdentificationDescriptionSize = IdentificationDescriptionSize;
}

typedef struct _WDF_CHILD_ADDRESS_DESCRIPTION_HEADER {
    //
    // Size in bytes of the entire description, including this header.
    //
    // Same value as WDF_CHILD_LIST_CONFIG::AddressDescriptionSize
    // Used as a sanity check.
    //
    ULONG AddressDescriptionSize;
}   WDF_CHILD_ADDRESS_DESCRIPTION_HEADER,
  *PWDF_CHILD_ADDRESS_DESCRIPTION_HEADER;

VOID
FORCEINLINE
WDF_CHILD_ADDRESS_DESCRIPTION_HEADER_INIT(
    PWDF_CHILD_ADDRESS_DESCRIPTION_HEADER Header,
    ULONG AddressDescriptionSize
    )
{
    RtlZeroMemory(Header, AddressDescriptionSize);
    Header->AddressDescriptionSize = AddressDescriptionSize;
}

typedef
NTSTATUS
(*PFN_WDF_CHILD_LIST_CREATE_DEVICE)(
    WDFCHILDLIST ChildList,
    PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER IdentificationDescription,
    PWDFDEVICE_INIT ChildInit
    );

typedef
VOID
(*PFN_WDF_CHILD_LIST_SCAN_FOR_CHILDREN)(
    WDFCHILDLIST ChildList
    );

typedef
VOID
(*PFN_WDF_CHILD_LIST_IDENTIFICATION_DESCRIPTION_COPY)(
    WDFCHILDLIST ChildList,
    PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER SourceIdentificationDescription,
    PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER DestinationIdentificationDescription
    );

typedef
NTSTATUS
(*PFN_WDF_CHILD_LIST_IDENTIFICATION_DESCRIPTION_DUPLICATE)(
    WDFCHILDLIST ChildList,
    PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER SourceIdentificationDescription,
    PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER DestinationIdentificationDescription
    );

typedef
BOOLEAN
(*PFN_WDF_CHILD_LIST_IDENTIFICATION_DESCRIPTION_COMPARE)(
    WDFCHILDLIST ChildList,
    PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER FirstIdentificationDescription,
    PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER SecondIdentificationDescription
    );

typedef
VOID
(*PFN_WDF_CHILD_LIST_IDENTIFICATION_DESCRIPTION_CLEANUP)(
    WDFCHILDLIST ChildList,
    PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER IdentificationDescription
    );

typedef
VOID
(*PFN_WDF_CHILD_LIST_ADDRESS_DESCRIPTION_COPY)(
    WDFCHILDLIST ChildList,
    PWDF_CHILD_ADDRESS_DESCRIPTION_HEADER SourceAddressDescription,
    PWDF_CHILD_ADDRESS_DESCRIPTION_HEADER DestinationAddressDescription
    );

typedef
NTSTATUS
(*PFN_WDF_CHILD_LIST_ADDRESS_DESCRIPTION_DUPLICATE)(
    WDFCHILDLIST ChildList,
    PWDF_CHILD_ADDRESS_DESCRIPTION_HEADER SourceAddressDescription,
    PWDF_CHILD_ADDRESS_DESCRIPTION_HEADER DestinationAddressDescription
    );

typedef
VOID
(*PFN_WDF_CHILD_LIST_ADDRESS_DESCRIPTION_CLEANUP)(
    WDFCHILDLIST ChildList,
    PWDF_CHILD_ADDRESS_DESCRIPTION_HEADER AddressDescription
    );

typedef
BOOLEAN
(*PFN_WDF_CHILD_LIST_DEVICE_REENUMERATED)(
    WDFCHILDLIST ChildList,
    WDFDEVICE OldDevice,
    PWDF_CHILD_ADDRESS_DESCRIPTION_HEADER OldAddressDescription,
    PWDF_CHILD_ADDRESS_DESCRIPTION_HEADER NewAddressDescription
    );

typedef struct _WDF_CHILD_RETRIEVE_INFO {
    //
    // Size of the structure in bytes
    //
    ULONG Size;

    //
    // Must be a valid pointer when passed in, copied into upon success
    //
    PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER IdentificationDescription;

    //
    // Optional pointer when passed in, copied into upon success
    //
    PWDF_CHILD_ADDRESS_DESCRIPTION_HEADER AddressDescription;

    //
    // Status of the returned device
    //
    WDF_CHILD_LIST_RETRIEVE_DEVICE_STATUS Status;

    //
    // If provided, will be used for searching through the list of devices
    // instead of the default list ID compare function
    //
    PFN_WDF_CHILD_LIST_IDENTIFICATION_DESCRIPTION_COMPARE EvtChildListIdentificationDescriptionCompare;

} WDF_CHILD_RETRIEVE_INFO, *PWDF_CHILD_RETRIEVE_INFO;

VOID
FORCEINLINE
WDF_CHILD_RETRIEVE_INFO_INIT(
    PWDF_CHILD_RETRIEVE_INFO Info,
    PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER IdentificationDescription
    )
{
    RtlZeroMemory(Info, sizeof(WDF_CHILD_RETRIEVE_INFO));

    Info->Size = sizeof(WDF_CHILD_RETRIEVE_INFO);
    Info->IdentificationDescription = IdentificationDescription;
}

typedef struct _WDF_CHILD_LIST_CONFIG {
    //
    // Size of this structure in bytes
    //
    ULONG Size;

    //
    // The size in bytes of an identificaiton description to be used with the
    // created WDFCHILDLIST handle
    //
    ULONG IdentificationDescriptionSize;

    //
    // Optional size in bytes of an address description to be used with the
    // created WDFCHILDLIST handle.
    //
    ULONG AddressDescriptionSize;

    //
    // Required callback to be invoked when a description on the device list
    // needs to be converted into a real WDFDEVICE handle.
    //
    PFN_WDF_CHILD_LIST_CREATE_DEVICE EvtChildListCreateDevice;

    //
    // Optional callback to be invoked when the device list needs to be
    // rescanned.  This function will be called after the device has entered D0
    // and been fully initialized but before I/O has started.
    //
    PFN_WDF_CHILD_LIST_SCAN_FOR_CHILDREN EvtChildListScanForChildren;

    //
    // Optional callback to be invoked when an identification description needs
    // to be copied from one location to another.
    //
    // If left NULL, RtlCopyMemory will be used to copy the description.
    //
    PFN_WDF_CHILD_LIST_IDENTIFICATION_DESCRIPTION_COPY EvtChildListIdentificationDescriptionCopy;

    //
    // Optional callback to be invoked when an identification description needs
    // to be duplicated.  As opposed to EvtChildListIdentificationDescriptionCopy,
    // EvtChildListIdentificationDescriptionDuplicate can fail.
    //
    PFN_WDF_CHILD_LIST_IDENTIFICATION_DESCRIPTION_DUPLICATE EvtChildListIdentificationDescriptionDuplicate;

    //
    // Optional callback to be invoked when an identification description needs
    // to be cleaned up.  This function should *NOT* free the description passed
    // to it, just free any associated resources.
    //
    PFN_WDF_CHILD_LIST_IDENTIFICATION_DESCRIPTION_CLEANUP EvtChildListIdentificationDescriptionCleanup;

    //
    // Optional callback to be invoked when an identification description needs
    // to be compared with another identificaiton description.
    //
    // If left NULL, RtlCompareMemory will be used to compare the two
    // descriptions.
    //
    PFN_WDF_CHILD_LIST_IDENTIFICATION_DESCRIPTION_COMPARE EvtChildListIdentificationDescriptionCompare;

    //
    // Optional callback to be invoked when an address description needs
    // to be copied from one location to another.
    //
    // If left NULL, RtlCopyMemory will be used to copy the description.
    //
    PFN_WDF_CHILD_LIST_ADDRESS_DESCRIPTION_COPY EvtChildListAddressDescriptionCopy;

    //
    // Optional callback to be invoked when an address description needs to be
    // duplicated.  As opposed to EvtChildListAddressDescriptionCopy,
    // EvtChildListAddressDescriptionDuplicate can fail.
    //
    PFN_WDF_CHILD_LIST_ADDRESS_DESCRIPTION_DUPLICATE EvtChildListAddressDescriptionDuplicate;

    //
    // Optional callback to be invoked when an address description needs to be
    // cleaned up.  This function should *NOT* free the description passed to
    // it, just free any associated resources.
    //
    PFN_WDF_CHILD_LIST_ADDRESS_DESCRIPTION_CLEANUP EvtChildListAddressDescriptionCleanup;

    //
    // If provided, will be called when the child's stack requests that the
    // child be reenumerated.  Returning TRUE allows for the reenumeration to
    // proceed.  FALSE will no reenumerate the stack.
    //
    PFN_WDF_CHILD_LIST_DEVICE_REENUMERATED EvtChildListDeviceReenumerated;

} WDF_CHILD_LIST_CONFIG, *PWDF_CHILD_LIST_CONFIG;

VOID
FORCEINLINE
WDF_CHILD_LIST_CONFIG_INIT(
    OUT PWDF_CHILD_LIST_CONFIG Config,
    IN ULONG IdentificationDescriptionSize,
    IN PFN_WDF_CHILD_LIST_CREATE_DEVICE EvtChildListCreateDevice
    )
{
    RtlZeroMemory(Config, sizeof(WDF_CHILD_LIST_CONFIG));

    Config->Size = sizeof(WDF_CHILD_LIST_CONFIG);
    Config->IdentificationDescriptionSize = IdentificationDescriptionSize;
    Config->EvtChildListCreateDevice = EvtChildListCreateDevice;
}

typedef struct _WDF_CHILD_LIST_ITERATOR {
    //
    // Size of this structure in bytes
    //
    ULONG Size;

    //
    // What type of devices to return, see WDF_RETRIEVE_CHILD_FLAGS for
    // flag values
    //
    //
    ULONG Flags;

    //
    // For internal use, treat this field as opaque
    //
    PVOID Reserved[4];

} WDF_CHILD_LIST_ITERATOR, *PWDF_CHILD_LIST_ITERATOR;

VOID
FORCEINLINE
WDF_CHILD_LIST_ITERATOR_INIT(
    PWDF_CHILD_LIST_ITERATOR Iterator,
    ULONG Flags
    )
{
    RtlZeroMemory(Iterator, sizeof(WDF_CHILD_LIST_ITERATOR));

    Iterator->Size = sizeof(WDF_CHILD_LIST_ITERATOR);
    Iterator->Flags = Flags;
}

//
// WDF Function: WdfChildListCreate
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFCHILDLISTCREATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PWDF_CHILD_LIST_CONFIG Config,
    PWDF_OBJECT_ATTRIBUTES ChildListAttributes,
    WDFCHILDLIST* ChildList
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfChildListCreate(
    __in
    WDFDEVICE Device,
    __in
    PWDF_CHILD_LIST_CONFIG Config,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES ChildListAttributes,
    __out
    WDFCHILDLIST* ChildList
    )
{
    return ((PFN_WDFCHILDLISTCREATE) WdfFunctions[WdfChildListCreateTableIndex])(WdfDriverGlobals, Device, Config, ChildListAttributes, ChildList);
}

//
// WDF Function: WdfChildListGetDevice
//
typedef
WDFAPI
WDFDEVICE
(*PFN_WDFCHILDLISTGETDEVICE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCHILDLIST ChildList
    );

WDFDEVICE
FORCEINLINE
WdfChildListGetDevice(
    __in
    WDFCHILDLIST ChildList
    )
{
    return ((PFN_WDFCHILDLISTGETDEVICE) WdfFunctions[WdfChildListGetDeviceTableIndex])(WdfDriverGlobals, ChildList);
}

//
// WDF Function: WdfChildListRetrievePdo
//
typedef
WDFAPI
WDFDEVICE
(*PFN_WDFCHILDLISTRETRIEVEPDO)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCHILDLIST ChildList,
    PWDF_CHILD_RETRIEVE_INFO RetrieveInfo
    );

WDFDEVICE
FORCEINLINE
WdfChildListRetrievePdo(
    __in
    WDFCHILDLIST ChildList,
    __in
    PWDF_CHILD_RETRIEVE_INFO RetrieveInfo
    )
{
    return ((PFN_WDFCHILDLISTRETRIEVEPDO) WdfFunctions[WdfChildListRetrievePdoTableIndex])(WdfDriverGlobals, ChildList, RetrieveInfo);
}

//
// WDF Function: WdfChildListRetrieveAddressDescription
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFCHILDLISTRETRIEVEADDRESSDESCRIPTION)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCHILDLIST ChildList,
    PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER IdentificationDescription,
    PWDF_CHILD_ADDRESS_DESCRIPTION_HEADER AddressDescription
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfChildListRetrieveAddressDescription(
    __in
    WDFCHILDLIST ChildList,
    __in
    PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER IdentificationDescription,
    __inout
    PWDF_CHILD_ADDRESS_DESCRIPTION_HEADER AddressDescription
    )
{
    return ((PFN_WDFCHILDLISTRETRIEVEADDRESSDESCRIPTION) WdfFunctions[WdfChildListRetrieveAddressDescriptionTableIndex])(WdfDriverGlobals, ChildList, IdentificationDescription, AddressDescription);
}

//
// WDF Function: WdfChildListBeginScan
//
typedef
WDFAPI
VOID
(*PFN_WDFCHILDLISTBEGINSCAN)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCHILDLIST ChildList
    );

VOID
FORCEINLINE
WdfChildListBeginScan(
    __in
    WDFCHILDLIST ChildList
    )
{
    ((PFN_WDFCHILDLISTBEGINSCAN) WdfFunctions[WdfChildListBeginScanTableIndex])(WdfDriverGlobals, ChildList);
}

//
// WDF Function: WdfChildListEndScan
//
typedef
WDFAPI
VOID
(*PFN_WDFCHILDLISTENDSCAN)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCHILDLIST ChildList
    );

VOID
FORCEINLINE
WdfChildListEndScan(
    __in
    WDFCHILDLIST ChildList
    )
{
    ((PFN_WDFCHILDLISTENDSCAN) WdfFunctions[WdfChildListEndScanTableIndex])(WdfDriverGlobals, ChildList);
}

//
// WDF Function: WdfChildListBeginIteration
//
typedef
WDFAPI
VOID
(*PFN_WDFCHILDLISTBEGINITERATION)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCHILDLIST ChildList,
    PWDF_CHILD_LIST_ITERATOR Iterator
    );

VOID
FORCEINLINE
WdfChildListBeginIteration(
    __in
    WDFCHILDLIST ChildList,
    __in
    PWDF_CHILD_LIST_ITERATOR Iterator
    )
{
    ((PFN_WDFCHILDLISTBEGINITERATION) WdfFunctions[WdfChildListBeginIterationTableIndex])(WdfDriverGlobals, ChildList, Iterator);
}

//
// WDF Function: WdfChildListRetrieveNextDevice
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFCHILDLISTRETRIEVENEXTDEVICE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCHILDLIST ChildList,
    PWDF_CHILD_LIST_ITERATOR Iterator,
    WDFDEVICE* Device,
    PWDF_CHILD_RETRIEVE_INFO Info
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfChildListRetrieveNextDevice(
    __in
    WDFCHILDLIST ChildList,
    __in
    PWDF_CHILD_LIST_ITERATOR Iterator,
    __out
    WDFDEVICE* Device,
    __in_opt
    PWDF_CHILD_RETRIEVE_INFO Info
    )
{
    return ((PFN_WDFCHILDLISTRETRIEVENEXTDEVICE) WdfFunctions[WdfChildListRetrieveNextDeviceTableIndex])(WdfDriverGlobals, ChildList, Iterator, Device, Info);
}

//
// WDF Function: WdfChildListEndIteration
//
typedef
WDFAPI
VOID
(*PFN_WDFCHILDLISTENDITERATION)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCHILDLIST ChildList,
    PWDF_CHILD_LIST_ITERATOR Iterator
    );

VOID
FORCEINLINE
WdfChildListEndIteration(
    __in
    WDFCHILDLIST ChildList,
    __in
    PWDF_CHILD_LIST_ITERATOR Iterator
    )
{
    ((PFN_WDFCHILDLISTENDITERATION) WdfFunctions[WdfChildListEndIterationTableIndex])(WdfDriverGlobals, ChildList, Iterator);
}

//
// WDF Function: WdfChildListAddOrUpdateChildDescriptionAsPresent
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFCHILDLISTADDORUPDATECHILDDESCRIPTIONASPRESENT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCHILDLIST ChildList,
    PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER IdentificationDescription,
    PWDF_CHILD_ADDRESS_DESCRIPTION_HEADER AddressDescription
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfChildListAddOrUpdateChildDescriptionAsPresent(
    __in
    WDFCHILDLIST ChildList,
    __in
    PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER IdentificationDescription,
    __in_opt
    PWDF_CHILD_ADDRESS_DESCRIPTION_HEADER AddressDescription
    )
{
    return ((PFN_WDFCHILDLISTADDORUPDATECHILDDESCRIPTIONASPRESENT) WdfFunctions[WdfChildListAddOrUpdateChildDescriptionAsPresentTableIndex])(WdfDriverGlobals, ChildList, IdentificationDescription, AddressDescription);
}

//
// WDF Function: WdfChildListUpdateChildDescriptionAsMissing
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFCHILDLISTUPDATECHILDDESCRIPTIONASMISSING)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCHILDLIST ChildList,
    PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER IdentificationDescription
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfChildListUpdateChildDescriptionAsMissing(
    __in
    WDFCHILDLIST ChildList,
    __in
    PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER IdentificationDescription
    )
{
    return ((PFN_WDFCHILDLISTUPDATECHILDDESCRIPTIONASMISSING) WdfFunctions[WdfChildListUpdateChildDescriptionAsMissingTableIndex])(WdfDriverGlobals, ChildList, IdentificationDescription);
}

//
// WDF Function: WdfChildListUpdateAllChildDescriptionsAsPresent
//
typedef
WDFAPI
VOID
(*PFN_WDFCHILDLISTUPDATEALLCHILDDESCRIPTIONSASPRESENT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCHILDLIST ChildList
    );

VOID
FORCEINLINE
WdfChildListUpdateAllChildDescriptionsAsPresent(
    __in
    WDFCHILDLIST ChildList
    )
{
    ((PFN_WDFCHILDLISTUPDATEALLCHILDDESCRIPTIONSASPRESENT) WdfFunctions[WdfChildListUpdateAllChildDescriptionsAsPresentTableIndex])(WdfDriverGlobals, ChildList);
}

//
// WDF Function: WdfChildListRequestChildEject
//
typedef
WDFAPI
BOOLEAN
(*PFN_WDFCHILDLISTREQUESTCHILDEJECT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCHILDLIST ChildList,
    PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER IdentificationDescription
    );

BOOLEAN
FORCEINLINE
WdfChildListRequestChildEject(
    __in
    WDFCHILDLIST ChildList,
    __in
    PWDF_CHILD_IDENTIFICATION_DESCRIPTION_HEADER IdentificationDescription
    )
{
    return ((PFN_WDFCHILDLISTREQUESTCHILDEJECT) WdfFunctions[WdfChildListRequestChildEjectTableIndex])(WdfDriverGlobals, ChildList, IdentificationDescription);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFCHILDLIST_H_

