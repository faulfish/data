/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    WdfString.h

Abstract:

    This is the DDI for string handles.

Environment:

    kernel mode only

Revision History:

--*/

#ifndef _WDFSTRING_H_
#define _WDFSTRING_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)



//
// WDF Function: WdfStringCreate
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFSTRINGCREATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PCUNICODE_STRING UnicodeString,
    PWDF_OBJECT_ATTRIBUTES StringAttributes,
    WDFSTRING* String
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfStringCreate(
    __in_opt
    PCUNICODE_STRING UnicodeString,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES StringAttributes,
    __out
    WDFSTRING* String
    )
{
    return ((PFN_WDFSTRINGCREATE) WdfFunctions[WdfStringCreateTableIndex])(WdfDriverGlobals, UnicodeString, StringAttributes, String);
}

//
// WDF Function: WdfStringGetUnicodeString
//
typedef
WDFAPI
VOID
(*PFN_WDFSTRINGGETUNICODESTRING)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFSTRING String,
    PUNICODE_STRING UnicodeString
    );

VOID
FORCEINLINE
WdfStringGetUnicodeString(
    __in
    WDFSTRING String,
    PUNICODE_STRING UnicodeString
    )
{
    ((PFN_WDFSTRINGGETUNICODESTRING) WdfFunctions[WdfStringGetUnicodeStringTableIndex])(WdfDriverGlobals, String, UnicodeString);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFSTRING_H_

