/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    wdfsync.h

Abstract:

    This module contains contains the Windows Driver Framework synchronization
    DDIs.

Environment:

    kernel mode only

Revision History:


--*/

#ifndef _WDFSYNC_H_
#define _WDFSYNC_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)



//
// WDF Function: WdfObjectAcquireLock
//
typedef
WDFAPI
VOID
(*PFN_WDFOBJECTACQUIRELOCK)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFOBJECT Object
    );

VOID
FORCEINLINE
WdfObjectAcquireLock(
    __in
    WDFOBJECT Object
    )
{
    ((PFN_WDFOBJECTACQUIRELOCK) WdfFunctions[WdfObjectAcquireLockTableIndex])(WdfDriverGlobals, Object);
}

//
// WDF Function: WdfObjectReleaseLock
//
typedef
WDFAPI
VOID
(*PFN_WDFOBJECTRELEASELOCK)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFOBJECT Object
    );

VOID
FORCEINLINE
WdfObjectReleaseLock(
    __in
    WDFOBJECT Object
    )
{
    ((PFN_WDFOBJECTRELEASELOCK) WdfFunctions[WdfObjectReleaseLockTableIndex])(WdfDriverGlobals, Object);
}

//
// WDF Function: WdfWaitLockCreate
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFWAITLOCKCREATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDF_OBJECT_ATTRIBUTES LockAttributes,
    WDFWAITLOCK* Lock
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfWaitLockCreate(
    __in_opt
    PWDF_OBJECT_ATTRIBUTES LockAttributes,
    __out
    WDFWAITLOCK* Lock
    )
{
    return ((PFN_WDFWAITLOCKCREATE) WdfFunctions[WdfWaitLockCreateTableIndex])(WdfDriverGlobals, LockAttributes, Lock);
}

//
// WDF Function: WdfWaitLockAcquire
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFWAITLOCKACQUIRE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFWAITLOCK Lock,
    PLONGLONG Timeout
    );

NTSTATUS
FORCEINLINE
WdfWaitLockAcquire(
    __in
    WDFWAITLOCK Lock,
    __in_opt
    PLONGLONG Timeout
    )
{
    return ((PFN_WDFWAITLOCKACQUIRE) WdfFunctions[WdfWaitLockAcquireTableIndex])(WdfDriverGlobals, Lock, Timeout);
}

//
// WDF Function: WdfWaitLockRelease
//
typedef
WDFAPI
VOID
(*PFN_WDFWAITLOCKRELEASE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFWAITLOCK Lock
    );

VOID
FORCEINLINE
WdfWaitLockRelease(
    __in
    WDFWAITLOCK Lock
    )
{
    ((PFN_WDFWAITLOCKRELEASE) WdfFunctions[WdfWaitLockReleaseTableIndex])(WdfDriverGlobals, Lock);
}

//
// WDF Function: WdfSpinLockCreate
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFSPINLOCKCREATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDF_OBJECT_ATTRIBUTES SpinLockAttributes,
    WDFSPINLOCK* SpinLock
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfSpinLockCreate(
    __in_opt
    PWDF_OBJECT_ATTRIBUTES SpinLockAttributes,
    __out
    WDFSPINLOCK* SpinLock
    )
{
    return ((PFN_WDFSPINLOCKCREATE) WdfFunctions[WdfSpinLockCreateTableIndex])(WdfDriverGlobals, SpinLockAttributes, SpinLock);
}

//
// WDF Function: WdfSpinLockAcquire
//
typedef
WDFAPI
VOID
(*PFN_WDFSPINLOCKACQUIRE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFSPINLOCK SpinLock
    );

VOID
FORCEINLINE
WdfSpinLockAcquire(
    __in
    WDFSPINLOCK SpinLock
    )
{
    ((PFN_WDFSPINLOCKACQUIRE) WdfFunctions[WdfSpinLockAcquireTableIndex])(WdfDriverGlobals, SpinLock);
}

//
// WDF Function: WdfSpinLockRelease
//
typedef
WDFAPI
VOID
(*PFN_WDFSPINLOCKRELEASE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFSPINLOCK SpinLock
    );

VOID
FORCEINLINE
WdfSpinLockRelease(
    __in
    WDFSPINLOCK SpinLock
    )
{
    ((PFN_WDFSPINLOCKRELEASE) WdfFunctions[WdfSpinLockReleaseTableIndex])(WdfDriverGlobals, SpinLock);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFSYNC_H_

