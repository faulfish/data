/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    wdfresource.h

Abstract:

    This defines the DDIs for hardware resources

Environment:

    kernel mode only

Revision History:

--*/

#ifndef _WDFRESOURCE_H_
#define _WDFRESOURCE_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)

#define WDF_INSERT_AT_END ((ULONG) -1)



//
// WDF Function: WdfIoResourceRequirementsListSetSlotNumber
//
typedef
WDFAPI
VOID
(*PFN_WDFIORESOURCEREQUIREMENTSLISTSETSLOTNUMBER)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIORESREQLIST RequirementsList,
    IN ULONG SlotNumber
    );

VOID
FORCEINLINE
WdfIoResourceRequirementsListSetSlotNumber(
    __in
    WDFIORESREQLIST RequirementsList,
    IN ULONG SlotNumber
    )
{
    ((PFN_WDFIORESOURCEREQUIREMENTSLISTSETSLOTNUMBER) WdfFunctions[WdfIoResourceRequirementsListSetSlotNumberTableIndex])(WdfDriverGlobals, RequirementsList, SlotNumber);
}

//
// WDF Function: WdfIoResourceRequirementsListSetInterfaceType
//
typedef
WDFAPI
VOID
(*PFN_WDFIORESOURCEREQUIREMENTSLISTSETINTERFACETYPE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIORESREQLIST RequirementsList,
    INTERFACE_TYPE InterfaceType
    );

VOID
FORCEINLINE
WdfIoResourceRequirementsListSetInterfaceType(
    __in
    WDFIORESREQLIST RequirementsList,
    INTERFACE_TYPE InterfaceType
    )
{
    ((PFN_WDFIORESOURCEREQUIREMENTSLISTSETINTERFACETYPE) WdfFunctions[WdfIoResourceRequirementsListSetInterfaceTypeTableIndex])(WdfDriverGlobals, RequirementsList, InterfaceType);
}

//
// WDF Function: WdfIoResourceRequirementsListAppendIoResList
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIORESOURCEREQUIREMENTSLISTAPPENDIORESLIST)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIORESREQLIST RequirementsList,
    WDFIORESLIST IoResList
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoResourceRequirementsListAppendIoResList(
    __in
    WDFIORESREQLIST RequirementsList,
    __in
    WDFIORESLIST IoResList
    )
{
    return ((PFN_WDFIORESOURCEREQUIREMENTSLISTAPPENDIORESLIST) WdfFunctions[WdfIoResourceRequirementsListAppendIoResListTableIndex])(WdfDriverGlobals, RequirementsList, IoResList);
}

//
// WDF Function: WdfIoResourceRequirementsListInsertIoResList
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIORESOURCEREQUIREMENTSLISTINSERTIORESLIST)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIORESREQLIST RequirementsList,
    WDFIORESLIST IoResList,
    IN ULONG Index
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoResourceRequirementsListInsertIoResList(
    __in
    WDFIORESREQLIST RequirementsList,
    __in
    WDFIORESLIST IoResList,
    IN ULONG Index
    )
{
    return ((PFN_WDFIORESOURCEREQUIREMENTSLISTINSERTIORESLIST) WdfFunctions[WdfIoResourceRequirementsListInsertIoResListTableIndex])(WdfDriverGlobals, RequirementsList, IoResList, Index);
}

//
// WDF Function: WdfIoResourceRequirementsListGetCount
//
typedef
WDFAPI
ULONG
(*PFN_WDFIORESOURCEREQUIREMENTSLISTGETCOUNT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIORESREQLIST RequirementsList
    );

ULONG
FORCEINLINE
WdfIoResourceRequirementsListGetCount(
    __in
    WDFIORESREQLIST RequirementsList
    )
{
    return ((PFN_WDFIORESOURCEREQUIREMENTSLISTGETCOUNT) WdfFunctions[WdfIoResourceRequirementsListGetCountTableIndex])(WdfDriverGlobals, RequirementsList);
}

//
// WDF Function: WdfIoResourceRequirementsListGetIoResList
//
typedef
WDFAPI
WDFIORESLIST
(*PFN_WDFIORESOURCEREQUIREMENTSLISTGETIORESLIST)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIORESREQLIST RequirementsList,
    IN ULONG Index
    );

WDFIORESLIST
FORCEINLINE
WdfIoResourceRequirementsListGetIoResList(
    __in
    WDFIORESREQLIST RequirementsList,
    IN ULONG Index
    )
{
    return ((PFN_WDFIORESOURCEREQUIREMENTSLISTGETIORESLIST) WdfFunctions[WdfIoResourceRequirementsListGetIoResListTableIndex])(WdfDriverGlobals, RequirementsList, Index);
}

//
// WDF Function: WdfIoResourceRequirementsListRemove
//
typedef
WDFAPI
VOID
(*PFN_WDFIORESOURCEREQUIREMENTSLISTREMOVE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIORESREQLIST RequirementsList,
    IN ULONG Index
    );

VOID
FORCEINLINE
WdfIoResourceRequirementsListRemove(
    __in
    WDFIORESREQLIST RequirementsList,
    IN ULONG Index
    )
{
    ((PFN_WDFIORESOURCEREQUIREMENTSLISTREMOVE) WdfFunctions[WdfIoResourceRequirementsListRemoveTableIndex])(WdfDriverGlobals, RequirementsList, Index);
}

//
// WDF Function: WdfIoResourceRequirementsListRemoveByIoResList
//
typedef
WDFAPI
VOID
(*PFN_WDFIORESOURCEREQUIREMENTSLISTREMOVEBYIORESLIST)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIORESREQLIST RequirementsList,
    WDFIORESLIST IoResList
    );

VOID
FORCEINLINE
WdfIoResourceRequirementsListRemoveByIoResList(
    __in
    WDFIORESREQLIST RequirementsList,
    __in
    WDFIORESLIST IoResList
    )
{
    ((PFN_WDFIORESOURCEREQUIREMENTSLISTREMOVEBYIORESLIST) WdfFunctions[WdfIoResourceRequirementsListRemoveByIoResListTableIndex])(WdfDriverGlobals, RequirementsList, IoResList);
}

//
// WDF Function: WdfIoResourceListCreate
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIORESOURCELISTCREATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIORESREQLIST RequirementsList,
    PWDF_OBJECT_ATTRIBUTES Attributes,
    WDFIORESLIST* ResourceList
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoResourceListCreate(
    __in
    WDFIORESREQLIST RequirementsList,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES Attributes,
    __out
    WDFIORESLIST* ResourceList
    )
{
    return ((PFN_WDFIORESOURCELISTCREATE) WdfFunctions[WdfIoResourceListCreateTableIndex])(WdfDriverGlobals, RequirementsList, Attributes, ResourceList);
}

//
// WDF Function: WdfIoResourceListAppendDescriptor
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIORESOURCELISTAPPENDDESCRIPTOR)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIORESLIST ResourceList,
    PIO_RESOURCE_DESCRIPTOR Descriptor
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoResourceListAppendDescriptor(
    __in
    WDFIORESLIST ResourceList,
    __in
    PIO_RESOURCE_DESCRIPTOR Descriptor
    )
{
    return ((PFN_WDFIORESOURCELISTAPPENDDESCRIPTOR) WdfFunctions[WdfIoResourceListAppendDescriptorTableIndex])(WdfDriverGlobals, ResourceList, Descriptor);
}

//
// WDF Function: WdfIoResourceListInsertDescriptor
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFIORESOURCELISTINSERTDESCRIPTOR)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIORESLIST ResourceList,
    PIO_RESOURCE_DESCRIPTOR Descriptor,
    IN ULONG Index
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfIoResourceListInsertDescriptor(
    __in
    WDFIORESLIST ResourceList,
    __in
    PIO_RESOURCE_DESCRIPTOR Descriptor,
    IN ULONG Index
    )
{
    return ((PFN_WDFIORESOURCELISTINSERTDESCRIPTOR) WdfFunctions[WdfIoResourceListInsertDescriptorTableIndex])(WdfDriverGlobals, ResourceList, Descriptor, Index);
}

//
// WDF Function: WdfIoResourceListUpdateDescriptor
//
typedef
WDFAPI
VOID
(*PFN_WDFIORESOURCELISTUPDATEDESCRIPTOR)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIORESLIST ResourceList,
    PIO_RESOURCE_DESCRIPTOR Descriptor,
    IN ULONG Index
    );

__checkReturn
VOID
FORCEINLINE
WdfIoResourceListUpdateDescriptor(
    __in
    WDFIORESLIST ResourceList,
    __in
    PIO_RESOURCE_DESCRIPTOR Descriptor,
    IN ULONG Index
    )
{
    ((PFN_WDFIORESOURCELISTUPDATEDESCRIPTOR) WdfFunctions[WdfIoResourceListUpdateDescriptorTableIndex])(WdfDriverGlobals, ResourceList, Descriptor, Index);
}

//
// WDF Function: WdfIoResourceListGetCount
//
typedef
WDFAPI
ULONG
(*PFN_WDFIORESOURCELISTGETCOUNT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIORESLIST ResourceList
    );

ULONG
FORCEINLINE
WdfIoResourceListGetCount(
    __in
    WDFIORESLIST ResourceList
    )
{
    return ((PFN_WDFIORESOURCELISTGETCOUNT) WdfFunctions[WdfIoResourceListGetCountTableIndex])(WdfDriverGlobals, ResourceList);
}

//
// WDF Function: WdfIoResourceListGetDescriptor
//
typedef
WDFAPI
PIO_RESOURCE_DESCRIPTOR
(*PFN_WDFIORESOURCELISTGETDESCRIPTOR)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIORESLIST ResourceList,
    IN ULONG Index
    );

PIO_RESOURCE_DESCRIPTOR
FORCEINLINE
WdfIoResourceListGetDescriptor(
    __in
    WDFIORESLIST ResourceList,
    IN ULONG Index
    )
{
    return ((PFN_WDFIORESOURCELISTGETDESCRIPTOR) WdfFunctions[WdfIoResourceListGetDescriptorTableIndex])(WdfDriverGlobals, ResourceList, Index);
}

//
// WDF Function: WdfIoResourceListRemove
//
typedef
WDFAPI
VOID
(*PFN_WDFIORESOURCELISTREMOVE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIORESLIST ResourceList,
    IN ULONG Index
    );

VOID
FORCEINLINE
WdfIoResourceListRemove(
    __in
    WDFIORESLIST ResourceList,
    IN ULONG Index
    )
{
    ((PFN_WDFIORESOURCELISTREMOVE) WdfFunctions[WdfIoResourceListRemoveTableIndex])(WdfDriverGlobals, ResourceList, Index);
}

//
// WDF Function: WdfIoResourceListRemoveByDescriptor
//
typedef
WDFAPI
VOID
(*PFN_WDFIORESOURCELISTREMOVEBYDESCRIPTOR)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFIORESLIST ResourceList,
    PIO_RESOURCE_DESCRIPTOR Descriptor
    );

VOID
FORCEINLINE
WdfIoResourceListRemoveByDescriptor(
    __in
    WDFIORESLIST ResourceList,
    __in
    PIO_RESOURCE_DESCRIPTOR Descriptor
    )
{
    ((PFN_WDFIORESOURCELISTREMOVEBYDESCRIPTOR) WdfFunctions[WdfIoResourceListRemoveByDescriptorTableIndex])(WdfDriverGlobals, ResourceList, Descriptor);
}

//
// WDF Function: WdfCmResourceListAppendDescriptor
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFCMRESOURCELISTAPPENDDESCRIPTOR)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCMRESLIST List,
    PCM_PARTIAL_RESOURCE_DESCRIPTOR Descriptor
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfCmResourceListAppendDescriptor(
    __in
    WDFCMRESLIST List,
    __in
    PCM_PARTIAL_RESOURCE_DESCRIPTOR Descriptor
    )
{
    return ((PFN_WDFCMRESOURCELISTAPPENDDESCRIPTOR) WdfFunctions[WdfCmResourceListAppendDescriptorTableIndex])(WdfDriverGlobals, List, Descriptor);
}

//
// WDF Function: WdfCmResourceListInsertDescriptor
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFCMRESOURCELISTINSERTDESCRIPTOR)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCMRESLIST List,
    PCM_PARTIAL_RESOURCE_DESCRIPTOR Descriptor,
    IN ULONG Index
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfCmResourceListInsertDescriptor(
    __in
    WDFCMRESLIST List,
    __in
    PCM_PARTIAL_RESOURCE_DESCRIPTOR Descriptor,
    IN ULONG Index
    )
{
    return ((PFN_WDFCMRESOURCELISTINSERTDESCRIPTOR) WdfFunctions[WdfCmResourceListInsertDescriptorTableIndex])(WdfDriverGlobals, List, Descriptor, Index);
}

//
// WDF Function: WdfCmResourceListGetCount
//
typedef
WDFAPI
ULONG
(*PFN_WDFCMRESOURCELISTGETCOUNT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCMRESLIST List
    );

ULONG
FORCEINLINE
WdfCmResourceListGetCount(
    __in
    WDFCMRESLIST List
    )
{
    return ((PFN_WDFCMRESOURCELISTGETCOUNT) WdfFunctions[WdfCmResourceListGetCountTableIndex])(WdfDriverGlobals, List);
}

//
// WDF Function: WdfCmResourceListGetDescriptor
//
typedef
WDFAPI
PCM_PARTIAL_RESOURCE_DESCRIPTOR
(*PFN_WDFCMRESOURCELISTGETDESCRIPTOR)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCMRESLIST List,
    IN ULONG Index
    );

PCM_PARTIAL_RESOURCE_DESCRIPTOR
FORCEINLINE
WdfCmResourceListGetDescriptor(
    __in
    WDFCMRESLIST List,
    IN ULONG Index
    )
{
    return ((PFN_WDFCMRESOURCELISTGETDESCRIPTOR) WdfFunctions[WdfCmResourceListGetDescriptorTableIndex])(WdfDriverGlobals, List, Index);
}

//
// WDF Function: WdfCmResourceListRemove
//
typedef
WDFAPI
VOID
(*PFN_WDFCMRESOURCELISTREMOVE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCMRESLIST List,
    IN ULONG Index
    );

VOID
FORCEINLINE
WdfCmResourceListRemove(
    __in
    WDFCMRESLIST List,
    IN ULONG Index
    )
{
    ((PFN_WDFCMRESOURCELISTREMOVE) WdfFunctions[WdfCmResourceListRemoveTableIndex])(WdfDriverGlobals, List, Index);
}

//
// WDF Function: WdfCmResourceListRemoveByDescriptor
//
typedef
WDFAPI
VOID
(*PFN_WDFCMRESOURCELISTREMOVEBYDESCRIPTOR)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFCMRESLIST List,
    PCM_PARTIAL_RESOURCE_DESCRIPTOR Descriptor
    );

VOID
FORCEINLINE
WdfCmResourceListRemoveByDescriptor(
    __in
    WDFCMRESLIST List,
    __in
    PCM_PARTIAL_RESOURCE_DESCRIPTOR Descriptor
    )
{
    ((PFN_WDFCMRESOURCELISTREMOVEBYDESCRIPTOR) WdfFunctions[WdfCmResourceListRemoveByDescriptorTableIndex])(WdfDriverGlobals, List, Descriptor);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFRESOURCE_H_

