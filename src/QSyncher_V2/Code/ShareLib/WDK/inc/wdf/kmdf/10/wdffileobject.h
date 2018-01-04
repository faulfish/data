/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    wdffileobject.h

Abstract:

    This header containts the Windows Driver Framework file object
    DDIs.

Environment:

    kernel mode only

Revision History:


--*/

#ifndef _WDFFILEOBJECT_H_
#define _WDFFILEOBJECT_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)



//
// WDF Function: WdfFileObjectGetFileName
//
typedef
WDFAPI
PUNICODE_STRING
(*PFN_WDFFILEOBJECTGETFILENAME)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFFILEOBJECT FileObject
    );

PUNICODE_STRING
FORCEINLINE
WdfFileObjectGetFileName(
    __in
    WDFFILEOBJECT FileObject
    )
{
    return ((PFN_WDFFILEOBJECTGETFILENAME) WdfFunctions[WdfFileObjectGetFileNameTableIndex])(WdfDriverGlobals, FileObject);
}

//
// WDF Function: WdfFileObjectGetFlags
//
typedef
WDFAPI
ULONG
(*PFN_WDFFILEOBJECTGETFLAGS)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFFILEOBJECT FileObject
    );

ULONG
FORCEINLINE
WdfFileObjectGetFlags(
    __in
    WDFFILEOBJECT FileObject
    )
{
    return ((PFN_WDFFILEOBJECTGETFLAGS) WdfFunctions[WdfFileObjectGetFlagsTableIndex])(WdfDriverGlobals, FileObject);
}

//
// WDF Function: WdfFileObjectGetDevice
//
typedef
WDFAPI
WDFDEVICE
(*PFN_WDFFILEOBJECTGETDEVICE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFFILEOBJECT FileObject
    );

WDFDEVICE
FORCEINLINE
WdfFileObjectGetDevice(
    __in
    WDFFILEOBJECT FileObject
    )
{
    return ((PFN_WDFFILEOBJECTGETDEVICE) WdfFunctions[WdfFileObjectGetDeviceTableIndex])(WdfDriverGlobals, FileObject);
}

//
// WDF Function: WdfFileObjectWdmGetFileObject
//
typedef
WDFAPI
PFILE_OBJECT
(*PFN_WDFFILEOBJECTWDMGETFILEOBJECT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFFILEOBJECT FileObject
    );

PFILE_OBJECT
FORCEINLINE
WdfFileObjectWdmGetFileObject(
    __in
    WDFFILEOBJECT FileObject
    )
{
    return ((PFN_WDFFILEOBJECTWDMGETFILEOBJECT) WdfFunctions[WdfFileObjectWdmGetFileObjectTableIndex])(WdfDriverGlobals, FileObject);
}




#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFFILEOBJECT_H_

