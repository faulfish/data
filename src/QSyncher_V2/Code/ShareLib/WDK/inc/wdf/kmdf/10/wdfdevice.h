/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    wdfdevice.h

Abstract:


Environment:

    kernel mode only

Revision History:

--*/

#ifndef _WDFDEVICE_H_
#define _WDFDEVICE_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)


// begin_wpp enum
typedef enum _WDF_DEVICE_STATE_FLAGS {
    WdfDevStateNP = 0x8000,
} WDF_DEVICE_STATE_FLAGS;

// end_wpp

// begin_wpp enum
typedef enum _WDF_DEVICE_PNP_STATE {
    WdfDevStatePnpInvalid = 0x00,
    WdfDevStatePnpObjectCreated = 0x100,
    WdfDevStatePnpCheckForDevicePresence = 0x101,
    WdfDevStatePnpEjectFailed = 0x102,
    WdfDevStatePnpEjectHardware = 0x103,
    WdfDevStatePnpEjectedWaitingForRemove = 0x104,
    WdfDevStatePnpInit = 0x105,
    WdfDevStatePnpInitStarting = 0x106,
    WdfDevStatePnpInitSurpriseRemoved = 0x107,
    WdfDevStatePnpHardwareAvailable = 0x108,
    WdfDevStatePnpEnableInterfaces = 0x109,
    WdfDevStatePnpHardwareAvailablePowerPolicyFailed = 0x10A,
    WdfDevStatePnpQueryRemoveAskDriver = 0x10B,
    WdfDevStatePnpQueryRemovePending = 0x10C,
    WdfDevStatePnpQueryRemoveStaticCheck = 0x10D,
    WdfDevStatePnpQueriedRemoving = 0x10E,
    WdfDevStatePnpQueryStopAskDriver = 0x10F,
    WdfDevStatePnpQueryStopPending = 0x110,
    WdfDevStatePnpQueryStopStaticCheck = 0x111,
    WdfDevStatePnpQueryCanceled = 0x112,
    WdfDevStatePnpRemoved = 0x113,
    WdfDevStatePnpPdoRemoved = 0x114,
    WdfDevStatePnpRemovedPdoWait = 0x115,
    WdfDevStatePnpRemovedPdoSurpriseRemoved = 0x116,
    WdfDevStatePnpRemovingDisableInterfaces = 0x117,
    WdfDevStatePnpRestarting = 0x118,
    WdfDevStatePnpStarted = 0x119,
    WdfDevStatePnpStartedCancelStop = 0x11A,
    WdfDevStatePnpStartedCancelRemove = 0x11B,
    WdfDevStatePnpStartedRemoving = 0x11C,
    WdfDevStatePnpStartingFromStopped = 0x11D,
    WdfDevStatePnpStopped = 0x11E,
    WdfDevStatePnpStoppedWaitForStartCompletion = 0x11F,
    WdfDevStatePnpStartedStopping = 0x120,
    WdfDevStatePnpSurpriseRemove = 0x121,
    WdfDevStatePnpInitQueryRemove = 0x122,
    WdfDevStatePnpInitQueryRemoveCanceled = 0x123,
    WdfDevStatePnpFdoRemoved = 0x124,
    WdfDevStatePnpRemovedWaitForChildren = 0x125,
    WdfDevStatePnpQueriedSurpriseRemove = 0x126,
    WdfDevStatePnpSurpriseRemoveIoStarted = 0x127,
    WdfDevStatePnpFailedPowerDown = 0x128,
    WdfDevStatePnpFailedIoStarting = 0x129,
    WdfDevStatePnpFailedOwnHardware = 0x12A,
    WdfDevStatePnpFailed = 0x12B,
    WdfDevStatePnpFailedSurpriseRemoved = 0x12C,
    WdfDevStatePnpFailedStarted = 0x12D,
    WdfDevStatePnpFailedWaitForRemove = 0x12E,
    WdfDevStatePnpFailedInit = 0x12F,
    WdfDevStatePnpPdoInitFailed = 0x130,
    WdfDevStatePnpRestart = 0x131,
    WdfDevStatePnpRestartReleaseHardware = 0x132,
    WdfDevStatePnpRestartHardwareAvailable = 0x133,
    WdfDevStatePnpPdoRestart = 0x134,
    WdfDevStatePnpFinal = 0x135,
    WdfDevStatePnpNull = 0x136,
} WDF_DEVICE_PNP_STATE, *PWDF_DEVICE_PNP_STATE;

// end_wpp

// begin_wpp enum
typedef enum _WDF_DEVICE_POWER_STATE {
    WdfDevStatePowerInvalid = 0x00,
    WdfDevStatePowerObjectCreated = 0x300,
    WdfDevStatePowerCheckDeviceType = 0x301,
    WdfDevStatePowerCheckDeviceTypeNP = 0x302 | WdfDevStateNP,
    WdfDevStatePowerCheckParentState = 0x303,
    WdfDevStatePowerCheckParentStateNP = 0x304 | WdfDevStateNP,
    WdfDevStatePowerEnablingWakeAtBus = 0x305,
    WdfDevStatePowerEnablingWakeAtBusNP = 0x306 | WdfDevStateNP,
    WdfDevStatePowerD0 = 0x307,
    WdfDevStatePowerD0NP = 0x308 | WdfDevStateNP,
    WdfDevStatePowerD0BusWakeOwner = 0x309,
    WdfDevStatePowerD0BusWakeOwnerNP = 0x30A | WdfDevStateNP,
    WdfDevStatePowerD0ArmedForWake = 0x30B,
    WdfDevStatePowerD0ArmedForWakeNP = 0x30C | WdfDevStateNP,
    WdfDevStatePowerD0DisarmingWakeAtBus = 0x30D,
    WdfDevStatePowerD0DisarmingWakeAtBusNP = 0x30E | WdfDevStateNP,
    WdfDevStatePowerD0Starting = 0x30F,
    WdfDevStatePowerD0StartingConnectInterrupt = 0x310,
    WdfDevStatePowerD0StartingDmaEnable = 0x311,
    WdfDevStatePowerD0StartingStartSelfManagedIo = 0x312,
    WdfDevStatePowerDecideD0State = 0x313,
    WdfDevStatePowerGotoD3Stopped = 0x314,
    WdfDevStatePowerStopped = 0x315,
    WdfDevStatePowerStartingCheckDeviceType = 0x316,
    WdfDevStatePowerStartingChild = 0x317,
    WdfDevStatePowerDxDisablingWakeAtBus = 0x318,
    WdfDevStatePowerDxDisablingWakeAtBusNP = 0x319 | WdfDevStateNP,
    WdfDevStatePowerGotoDx = 0x31A,
    WdfDevStatePowerGotoDxNP = 0x31B | WdfDevStateNP,
    WdfDevStatePowerGotoDxIoStopped = 0x31C,
    WdfDevStatePowerGotoDxIoStoppedNP = 0x31D | WdfDevStateNP,
    WdfDevStatePowerGotoDxNPFailed = 0x31E | WdfDevStateNP,
    WdfDevStatePowerDx = 0x31F,
    WdfDevStatePowerDxNP = 0x320 | WdfDevStateNP,
    WdfDevStatePowerGotoDxArmedForWake = 0x321,
    WdfDevStatePowerGotoDxArmedForWakeNP = 0x322 | WdfDevStateNP,
    WdfDevStatePowerGotoDxIoStoppedArmedForWake = 0x323,
    WdfDevStatePowerGotoDxIoStoppedArmedForWakeNP = 0x324 | WdfDevStateNP,
    WdfDevStatePowerDxArmedForWake = 0x325,
    WdfDevStatePowerDxArmedForWakeNP = 0x326 | WdfDevStateNP,
    WdfDevStatePowerCheckParentStateArmedForWake = 0x327,
    WdfDevStatePowerCheckParentStateArmedForWakeNP = 0x328 | WdfDevStateNP,
    WdfDevStatePowerWaitForParentArmedForWake = 0x329,
    WdfDevStatePowerWaitForParentArmedForWakeNP = 0x32A | WdfDevStateNP,
    WdfDevStatePowerStartSelfManagedIo = 0x32B,
    WdfDevStatePowerStartSelfManagedIoNP = 0x32C | WdfDevStateNP,
    WdfDevStatePowerStartSelfManagedIoFailed = 0x32D,
    WdfDevStatePowerStartSelfManagedIoFailedNP = 0x32E | WdfDevStateNP,
    WdfDevStatePowerWaitForParent = 0x32F,
    WdfDevStatePowerWaitForParentNP = 0x330 | WdfDevStateNP,
    WdfDevStatePowerWakePending = 0x331,
    WdfDevStatePowerWakePendingNP = 0x332 | WdfDevStateNP,
    WdfDevStatePowerWaking = 0x333,
    WdfDevStatePowerWakingNP = 0x334 | WdfDevStateNP,
    WdfDevStatePowerWakingConnectInterrupt = 0x335,
    WdfDevStatePowerWakingConnectInterruptNP = 0x336 | WdfDevStateNP,
    WdfDevStatePowerWakingConnectInterruptFailed = 0x337,
    WdfDevStatePowerWakingConnectInterruptFailedNP = 0x338 | WdfDevStateNP,
    WdfDevStatePowerWakingDmaEnable = 0x339,
    WdfDevStatePowerWakingDmaEnableNP = 0x33A | WdfDevStateNP,
    WdfDevStatePowerWakingDmaEnableFailed = 0x33B,
    WdfDevStatePowerWakingDmaEnableFailedNP = 0x33C | WdfDevStateNP,
    WdfDevStatePowerReportPowerUpFailedDerefParent = 0x33D,
    WdfDevStatePowerReportPowerUpFailed = 0x33E,
    WdfDevStatePowerPowerFailedPowerDown = 0x33F,
    WdfDevStatePowerReportPowerDownFailed = 0x340,
    WdfDevStatePowerInitialConnectInterruptFailed = 0x341,
    WdfDevStatePowerInitialDmaEnableFailed = 0x342,
    WdfDevStatePowerInitialSelfManagedIoFailed = 0x343,
    WdfDevStatePowerInitialPowerUpFailedDerefParent = 0x344,
    WdfDevStatePowerInitialPowerUpFailed = 0x345,
    WdfDevStatePowerDxStoppedDisarmWake = 0x346,
    WdfDevStatePowerDxStoppedDisarmWakeNP = 0x347 | WdfDevStateNP,
    WdfDevStatePowerGotoDxStoppedDisableInterruptNP = 0x348 | WdfDevStateNP,
    WdfDevStatePowerGotoDxStopped = 0x349,
    WdfDevStatePowerDxStopped = 0x34A,
    WdfDevStatePowerGotoStopped = 0x34B,
    WdfDevStatePowerStoppedCompleteDx = 0x34C,
    WdfDevStatePowerDxStoppedDecideDxState = 0x34D,
    WdfDevStatePowerDxStoppedArmForWake = 0x34E,
    WdfDevStatePowerDxStoppedArmForWakeNP = 0x34F | WdfDevStateNP,
    WdfDevStatePowerFinalPowerDownFailed = 0x350,
    WdfDevStatePowerFinal = 0x351,
    WdfDevStatePowerNull = 0x352,
} WDF_DEVICE_POWER_STATE, *PWDF_DEVICE_POWER_STATE;

// end_wpp


// begin_wpp enum
typedef enum _WDF_DEVICE_POWER_POLICY_STATE {
    WdfDevStatePwrPolInvalid =  0x00,
    WdfDevStatePwrPolObjectCreated = 0x500,
    WdfDevStatePwrPolStarting = 0x501,
    WdfDevStatePwrPolStartingSucceeded = 0x502,
    WdfDevStatePwrPolStartingFailed = 0x503,
    WdfDevStatePwrPolStartingDecideS0Wake = 0x504,
    WdfDevStatePwrPolStartedIdleCapable = 0x505,
    WdfDevStatePwrPolTimerExpiredNoWake = 0x506,
    WdfDevStatePwrPolTimerExpiredNoWakeCompletePowerDown = 0x507,
    WdfDevStatePwrPolWaitingUnarmed = 0x508,
    WdfDevStatePwrPolWaitingUnarmedQueryIdle = 0x509,
    WdfDevStatePwrPolS0NoWakePowerUp = 0x50A,
    WdfDevStatePwrPolS0NoWakeCompletePowerUp = 0x50B,
    WdfDevStatePwrPolSystemSleepFromDeviceWaitingUnarmed = 0x50C,
    WdfDevStatePwrPolSystemSleepNeedWake = 0x50D,
    WdfDevStatePwrPolSystemSleepNeedWakeCompletePowerUp = 0x50E,
    WdfDevStatePwrPolSystemSleepPowerRequestFailed = 0x50F,
    WdfDevStatePwrPolCheckPowerPageable = 0x510,
    WdfDevStatePwrPolSleepingWakeWakeArrived = 0x511,
    WdfDevStatePwrPolSleepingWakeRevertArmWake = 0x512,
    WdfDevStatePwrPolSystemAsleepWakeArmed = 0x513,
    WdfDevStatePwrPolSystemWakeDeviceWakeEnabled = 0x514,
    WdfDevStatePwrPolSystemWakeDeviceWakeEnabledWakeCanceled = 0x515,
    WdfDevStatePwrPolSystemWakeDeviceWakeDisarm = 0x516,
    WdfDevStatePwrPolSystemWakeDeviceWakeTriggered = 0x517,
    WdfDevStatePwrPolSystemWakeDeviceWakeTriggeredS0 = 0x518,
    WdfDevStatePwrPolSystemWakeDeviceWokeDisarm = 0x519,
    WdfDevStatePwrPolSleepingWakeWakeArrivedNP =  0x51A | WdfDevStateNP,
    WdfDevStatePwrPolSleepingWakeRevertArmWakeNP =  0x51B | WdfDevStateNP,
    WdfDevStatePwrPolSleepingWakePowerDownFailed = 0x51C,
    WdfDevStatePwrPolSleepingWakePowerDownFailedWakeCanceled = 0x51D,
    WdfDevStatePwrPolSystemAsleepWakeArmedNP =  0x51E | WdfDevStateNP,
    WdfDevStatePwrPolSystemWakeDeviceWakeEnabledNP =  0x51F | WdfDevStateNP,
    WdfDevStatePwrPolSystemWakeDeviceWakeEnabledWakeCanceledNP =  0x520 | WdfDevStateNP,
    WdfDevStatePwrPolSystemWakeDeviceWakeDisarmNP =  0x521 | WdfDevStateNP,
    WdfDevStatePwrPolSystemWakeDeviceWakeTriggeredNP =  0x522 | WdfDevStateNP,
    WdfDevStatePwrPolSystemWakeDeviceWakeTriggeredS0NP =  0x523 | WdfDevStateNP,
    WdfDevStatePwrPolSystemWakeDeviceWokeDisarmNP =  0x524 | WdfDevStateNP,
    WdfDevStatePwrPolSystemWakeDeviceWakeCompletePowerUp = 0x525,
    WdfDevStatePwrPolSleeping = 0x526,
    WdfDevStatePwrPolSleepingNoWakePowerDown = 0x527,
    WdfDevStatePwrPolSleepingNoWakeCompletePowerDown = 0x528,
    WdfDevStatePwrPolSleepingNoWakeDxRequestFailed = 0x529,
    WdfDevStatePwrPolSleepingWakePowerDown = 0x52A,
    WdfDevStatePwrPolSleepingSendWake = 0x52B,
    WdfDevStatePwrPolSystemAsleepNoWake = 0x52C,
    WdfDevStatePwrPolSystemWakeDeviceWakeDisabled = 0x52D,
    WdfDevStatePwrPolSystemWakeDeviceToD0 = 0x52E,
    WdfDevStatePwrPolSystemWakeDeviceToD0CompletePowerUp = 0x52F,
    WdfDevStatePwrPolSystemWakeQueryIdle = 0x530,
    WdfDevStatePwrPolStartedWakeCapable = 0x531,
    WdfDevStatePwrPolTimerExpiredDecideUsbSS = 0x532,
    WdfDevStatePwrPolTimerExpiredWakeCapablePowerDown = 0x533,
    WdfDevStatePwrPolTimerExpiredWakeCapableSendWake = 0x534,
    WdfDevStatePwrPolTimerExpiredWakeCapableUsbSS = 0x535,
    WdfDevStatePwrPolTimerExpiredWakeCapableWakeArrived = 0x536,
    WdfDevStatePwrPolTimerExpiredWakeCapableCancelWake = 0x537,
    WdfDevStatePwrPolTimerExpiredWakeCapableWakeCanceled = 0x538,
    WdfDevStatePwrPolTimerExpiredWakeCapableCleanup = 0x539,
    WdfDevStatePwrPolTimerExpiredWakeCapableDxAllocFailed = 0x53A,
    WdfDevStatePwrPolTimerExpiredWakeCompletedPowerDown = 0x53B,
    WdfDevStatePwrPolTimerExpiredWakeCompletedPowerUp = 0x53C,
    WdfDevStatePwrPolWaitingArmedUsbSS = 0x53D,
    WdfDevStatePwrPolWaitingArmed = 0x53E,
    WdfDevStatePwrPolWaitingArmedQueryIdle = 0x53F,
    WdfDevStatePwrPolIoPresentArmed = 0x540,
    WdfDevStatePwrPolIoPresentArmedWakeCanceled = 0x541,
    WdfDevStatePwrPolS0WakeDisarm = 0x542,
    WdfDevStatePwrPolS0WakeCompletePowerUp = 0x543,
    WdfDevStatePwrPolTimerExpiredWakeSucceeded = 0x544,
    WdfDevStatePwrPolTimerExpiredWakeCompletedDisarm = 0x545,
    WdfDevStatePwrPolTimerExpiredWakeCapableWakeSucceeded = 0x546,
    WdfDevStatePwrPolTimerExpiredWakeCapableWakeFailed = 0x547,
    WdfDevStatePwrPolWakeFailedUsbSS = 0x548,
    WdfDevStatePwrPolTimerExpiredWakeCapablePowerDownFailedCancelWake = 0x549,
    WdfDevStatePwrPolTimerExpiredWakeCapablePowerDownFailedWakeCanceled = 0x54A,
    WdfDevStatePwrPolTimerExpiredWakeCapablePowerDownFailedUsbSS = 0x54B,
    WdfDevStatePwrPolCancelingWakeForSystemSleep = 0x54C,
    WdfDevStatePwrPolCancelingWakeForSystemSleepWakeCanceled = 0x54D,
    WdfDevStatePwrPolDisarmingWakeForSystemSleepCompletePowerUp = 0x54E,
    WdfDevStatePwrPolPowerUpForSystemSleepFailed = 0x54F,
    WdfDevStatePwrPolWokeFromS0UsbSS = 0x550,
    WdfDevStatePwrPolWokeFromS0 = 0x551,
    WdfDevStatePwrPolWokeFromS0NotifyDriver = 0x552,
    WdfDevStatePwrPolStoppingResetDevice = 0x553,
    WdfDevStatePwrPolStoppingResetDeviceCompletePowerUp = 0x554,
    WdfDevStatePwrPolStoppingResetDeviceFailed = 0x555,
    WdfDevStatePwrPolStoppingD0 = 0x556,
    WdfDevStatePwrPolStoppingD0Failed = 0x557,
    WdfDevStatePwrPolStoppingDisarmWake = 0x558,
    WdfDevStatePwrPolStoppingDisarmWakeCancelWake = 0x559,
    WdfDevStatePwrPolStoppingDisarmWakeWakeCanceled = 0x55A,
    WdfDevStatePwrPolStopping = 0x55B,
    WdfDevStatePwrPolStoppingFailed = 0x55C,
    WdfDevStatePwrPolStoppingSendStatus = 0x55D,
    WdfDevStatePwrPolStoppingCancelTimer = 0x55E,
    WdfDevStatePwrPolStoppingWaitForIdleTimeout = 0x55F,
    WdfDevStatePwrPolStoppingCancelUsbSS = 0x560,
    WdfDevStatePwrPolStoppingWaitForUsbSSCompletion = 0x561,
    WdfDevStatePwrPolStoppingCancelWake = 0x562,
    WdfDevStatePwrPolStopped = 0x563,
    WdfDevStatePwrPolCancelUsbSS = 0x564,
    WdfDevStatePwrPolStarted = 0x565,
    WdfDevStatePwrPolStartedCancelTimer = 0x566,
    WdfDevStatePwrPolStartedWaitForIdleTimeout = 0x567,
    WdfDevStatePwrPolStartedWakeCapableCancelTimerForSleep = 0x568,
    WdfDevStatePwrPolStartedWakeCapableWaitForIdleTimeout = 0x569,
    WdfDevStatePwrPolStartedWakeCapableSleepingUsbSS = 0x56A,
    WdfDevStatePwrPolStartedIdleCapableCancelTimerForSleep = 0x56B,
    WdfDevStatePwrPolStartedIdleCapableWaitForIdleTimeout = 0x56C,
    WdfDevStatePwrPolDeviceD0PowerRequestFailed = 0x56D,
    WdfDevStatePwrPolDevicePowerRequestFailed = 0x56E,
    WdfDevStatePwrPolGotoDx = 0x56F,
    WdfDevStatePwrPolGotoDxInDx = 0x570,
    WdfDevStatePwrPolDx = 0x571,
    WdfDevStatePwrPolGotoD0 = 0x572,
    WdfDevStatePwrPolGotoD0InD0 = 0x573,
    WdfDevStatePwrPolFinal = 0x574,
    WdfDevStatePwrPolSleepingPowerDownNotProcessed = 0x575,
    WdfDevStatePwrPolTimerExpiredWakeCapablePowerDownNotProcessed = 0x576,
    WdfDevStatePwrPolTimerExpiredNoWakePowerDownNotProcessed = 0x577,
    WdfDevStatePwrPolTimerExpiredNoWakePoweredDownDisableIdleTimer = 0x578,
    WdfDevStatePwrPolStoppingWaitingForImplicitPowerDown = 0x579,
    WdfDevStatePwrPolStoppingPoweringUp = 0x57A,
    WdfDevStatePwrPolStoppingPoweringDown = 0x57B,
    WdfDevStatePwrPolNull = 0x57C,
} WDF_DEVICE_POWER_POLICY_STATE, *PWDF_DEVICE_POWER_POLICY_STATE;

// end_wpp

typedef enum _WDF_STATE_NOTIFICATION_TYPE {
    StateNotificationInvalid =       0x0000,
    StateNotificationEnterState =       0x0001,
    StateNotificationPostProcessState = 0x0002,
    StateNotificationLeaveState =       0x0004,
    StateNotificationAllStates =       StateNotificationEnterState | StateNotificationPostProcessState | StateNotificationLeaveState,
} WDF_STATE_NOTIFICATION_TYPE;

typedef enum _WDF_POWER_POLICY_S0_IDLE_CAPABILITIES {
    IdleCapsInvalid = 0,
    IdleCannotWakeFromS0,
    IdleCanWakeFromS0,
    IdleUsbSelectiveSuspend,
} WDF_POWER_POLICY_S0_IDLE_CAPABILITIES;

typedef enum _WDF_POWER_POLICY_S0_IDLE_USER_CONTROL {
    IdleUserControlInvalid = 0,
    IdleDoNotAllowUserControl,
    IdleAllowUserControl,
} WDF_POWER_POLICY_S0_IDLE_USER_CONTROL;

typedef enum _WDF_POWER_POLICY_IDLE_TIMEOUT_CONSTANTS {
    IdleTimeoutDefaultConstant = 0,
} WDF_POWER_POLICY_IDLE_TIMEOUT_CONSTANTS;

typedef enum _WDF_POWER_POLICY_SX_WAKE_USER_CONTROL {
    WakeUserControlInvalid = 0,
    WakeDoNotAllowUserControl,
    WakeAllowUserControl,
} WDF_POWER_POLICY_SX_WAKE_USER_CONTROL;

typedef enum _WDF_POWER_DEVICE_STATE {
    WdfPowerDeviceInvalid = 0,
    WdfPowerDeviceD0,
    WdfPowerDeviceD1,
    WdfPowerDeviceD2,
    WdfPowerDeviceD3,
    WdfPowerDeviceD3Final,
    WdfPowerDevicePrepareForHibernation,
    WdfPowerDeviceMaximum,
} WDF_POWER_DEVICE_STATE, *PWDF_POWER_DEVICE_STATE;

typedef enum _WDF_SPECIAL_FILE_TYPE {
    WdfSpecialFileUndefined = 0,
    WdfSpecialFilePaging = 1,
    WdfSpecialFileHibernation,
    WdfSpecialFileDump,
    WdfSpecialFileMax,
} WDF_SPECIAL_FILE_TYPE, *PWDF_SPECIAL_FILE_TYPE;

typedef enum _WDF_DEVICE_IO_TYPE {
    WdfDeviceIoUndefined = 0,
    WdfDeviceIoNeither,
    WdfDeviceIoBuffered,
    WdfDeviceIoDirect,
} WDF_DEVICE_IO_TYPE, *PWDF_DEVICE_IO_TYPE;

typedef enum _WDF_FILEOBJECT_CLASS {
    WdfFileObjectInvalid = 0,
    WdfFileObjectNotRequired,
    WdfFileObjectWdfCanUseFsContext,
    WdfFileObjectWdfCanUseFsContext2,
    WdfFileObjectWdfCannotUseFsContexts,
} WDF_FILEOBJECT_CLASS, *PWDF_FILEOBJECT_CLASS;

typedef enum _WDF_DEVICE_FAILED_ACTION {
    WdfDeviceFailedUndefined = 0,
    WdfDeviceFailedAttemptRestart,
    WdfDeviceFailedNoRestart,
} WDF_DEVICE_FAILED_ACTION;


//
// If you do not want the auto inclusion of wdmsec.h, define
// WDF_DEVICE_NO_WDMSEC_H before including this file (or wdf.h)
//
#ifndef WDF_DEVICE_NO_WDMSEC_H
#include <wdmsec.h>
#endif // WDF_DEVICE_NO_WDMSEC_H

#define WDF_S0_IDLE_ENABLED_VALUE_NAME      L"IdleInWorkingState"
#define WDF_SX_WAKE_ENABLED_VALUE_NAME      L"WakeFromSleepState"

//
// Declare empty definitions so that they may be referenced by
// routines before they are defined.
//
typedef enum _WDF_REQUEST_TYPE WDF_REQUEST_TYPE;


typedef
VOID
(*PFN_WDF_DEVICE_FILE_CREATE)(
    IN WDFDEVICE Device,
    IN WDFREQUEST Request,
    IN WDFFILEOBJECT FileObject
    );


typedef
VOID
(*PFN_WDF_FILE_CLOSE)(
    IN WDFFILEOBJECT FileObject
    );

typedef
VOID
(*PFN_WDF_FILE_CLEANUP)(
    IN WDFFILEOBJECT FileObject
    );

typedef struct _WDF_FILEOBJECT_CONFIG {
    //
    // Size of this structure in bytes
    //
    ULONG Size;

    //
    // Event callback for create requests
    //
    PFN_WDF_DEVICE_FILE_CREATE  EvtDeviceFileCreate;

    //
    // Event callback for close requests
    //
    PFN_WDF_FILE_CLOSE   EvtFileClose;

    //
    // Event callback for cleanup requests
    //
    PFN_WDF_FILE_CLEANUP EvtFileCleanup;

    //
    // If WdfTrue, create/cleanup/close file object related requests will be
    //      sent down the stack.
    //
    // If WdfFalse, create/cleanup/close will be completed at this location in
    //      the device stack.
    //
    // If WdfDefault, behavior depends on device type
    //      FDO, PDO, Control:  use the WdfFalse behavior
    //      Filter:  use the WdfTrue behavior
    //
    WDF_TRI_STATE AutoForwardCleanupClose;

    //
    // Specify whether framework should create WDFFILEOBJECT and also
    // whether it can FsContexts fields in the WDM fileobject to store
    // WDFFILEOBJECT so that it can avoid table look up and improve perf.
    //
    WDF_FILEOBJECT_CLASS FileObjectClass;

} WDF_FILEOBJECT_CONFIG, *PWDF_FILEOBJECT_CONFIG;

VOID
FORCEINLINE
WDF_FILEOBJECT_CONFIG_INIT(
    OUT PWDF_FILEOBJECT_CONFIG FileEventCallbacks,
    IN OPTIONAL PFN_WDF_DEVICE_FILE_CREATE EvtDeviceFileCreate,
    IN OPTIONAL PFN_WDF_FILE_CLOSE EvtFileClose,
    IN OPTIONAL PFN_WDF_FILE_CLEANUP EvtFileCleanup
    )
{
    FileEventCallbacks->Size = sizeof(WDF_FILEOBJECT_CONFIG);

    FileEventCallbacks->EvtDeviceFileCreate  = EvtDeviceFileCreate;
    FileEventCallbacks->EvtFileClose   = EvtFileClose;
    FileEventCallbacks->EvtFileCleanup = EvtFileCleanup;

    FileEventCallbacks->FileObjectClass = WdfFileObjectWdfCannotUseFsContexts;
    FileEventCallbacks->AutoForwardCleanupClose = WdfUseDefault;
}

typedef struct _WDF_DEVICE_PNP_NOTIFICATION_DATA {
    //
    // Type of data
    //
    WDF_STATE_NOTIFICATION_TYPE Type;

    union {
        struct {
            //
            // The current state that is about to be exited
            //
            WDF_DEVICE_PNP_STATE CurrentState;

            //
            // The new state that is about to be entered
            //
            WDF_DEVICE_PNP_STATE NewState;
        } EnterState;

        struct {
            //
            // The current state
            //
            WDF_DEVICE_PNP_STATE CurrentState;
        } PostProcessState;


        struct {
            //
            // The current state that is about to be exitted
            //
            WDF_DEVICE_PNP_STATE CurrentState;

            //
            // The state that is about to be entered
            //
            WDF_DEVICE_PNP_STATE NewState;
        } LeaveState;
    } Data;
} WDF_DEVICE_PNP_NOTIFICATION_DATA;

typedef const WDF_DEVICE_PNP_NOTIFICATION_DATA* PCWDF_DEVICE_PNP_NOTIFICATION_DATA;

typedef struct _WDF_DEVICE_POWER_NOTIFICATION_DATA {
    //
    // Type of data
    //
    WDF_STATE_NOTIFICATION_TYPE Type;

    union {
        struct {
            //
            // The current state that is about to be exitted
            //
            WDF_DEVICE_POWER_STATE CurrentState;

            //
            // The new state that is about to be entered
            //
            WDF_DEVICE_POWER_STATE NewState;
        } EnterState;

        struct {
            //
            // The current state
            //
            WDF_DEVICE_POWER_STATE CurrentState;
        } PostProcessState;


        struct {
            //
            // The current state that is about to be exitted
            //
            WDF_DEVICE_POWER_STATE CurrentState;

            //
            // The state that is about to be entered
            //
            WDF_DEVICE_POWER_STATE NewState;
        } LeaveState;
    } Data;
} WDF_DEVICE_POWER_NOTIFICATION_DATA;

typedef const WDF_DEVICE_POWER_NOTIFICATION_DATA* PCWDF_DEVICE_POWER_NOTIFICATION_DATA;

typedef struct _WDF_DEVICE_POWER_POLICY_NOTIFICATION_DATA {
    //
    // Type of data
    //
    WDF_STATE_NOTIFICATION_TYPE Type;

    union {
        struct {
            //
            // The current state that is about to be exitted
            //
            WDF_DEVICE_POWER_POLICY_STATE CurrentState;

            //
            // The new state that is about to be entered
            //
            WDF_DEVICE_POWER_POLICY_STATE NewState;
        } EnterState;

        struct {
            //
            // The current state
            //
            WDF_DEVICE_POWER_POLICY_STATE CurrentState;
        } PostProcessState;


        struct {
            //
            // The current state that is about to be exitted
            //
            WDF_DEVICE_POWER_POLICY_STATE CurrentState;

            //
            // The state that is about to be entered
            //
            WDF_DEVICE_POWER_POLICY_STATE NewState;
        } LeaveState;
    } Data;
} WDF_DEVICE_POWER_POLICY_NOTIFICATION_DATA;

typedef const WDF_DEVICE_POWER_POLICY_NOTIFICATION_DATA* PCWDF_DEVICE_POWER_POLICY_NOTIFICATION_DATA;

typedef
VOID
(*PFN_WDF_DEVICE_PNP_STATE_CHANGE_NOTIFICATION)(
    WDFDEVICE Device,
    PCWDF_DEVICE_PNP_NOTIFICATION_DATA NotificationData
    );

typedef
VOID
(*PFN_WDF_DEVICE_POWER_STATE_CHANGE_NOTIFICATION)(
    WDFDEVICE Device,
    PCWDF_DEVICE_POWER_NOTIFICATION_DATA NotificationData
    );

typedef
VOID
(*PFN_WDF_DEVICE_POWER_POLICY_STATE_CHANGE_NOTIFICATION)(
    WDFDEVICE Device,
    PCWDF_DEVICE_POWER_POLICY_NOTIFICATION_DATA NotificationData
    );

#define IdleTimeoutDefaultValue ((ULONG) IdleTimeoutDefaultConstant)

typedef
NTSTATUS
(*PFN_WDF_DEVICE_D0_ENTRY)(
    IN  WDFDEVICE Device,
    IN  WDF_POWER_DEVICE_STATE PreviousState
    );

typedef
NTSTATUS
(*PFN_WDF_DEVICE_D0_ENTRY_POST_INTERRUPTS_ENABLED)(
    IN  WDFDEVICE Device,
    IN  WDF_POWER_DEVICE_STATE PreviousState
    );

typedef
NTSTATUS
(*PFN_WDF_DEVICE_D0_EXIT)(
    IN  WDFDEVICE Device,
    IN  WDF_POWER_DEVICE_STATE TargetState
    );

typedef
NTSTATUS
(*PFN_WDF_DEVICE_D0_EXIT_PRE_INTERRUPTS_DISABLED)(
    IN  WDFDEVICE Device,
    IN  WDF_POWER_DEVICE_STATE TargetState
    );


typedef
NTSTATUS
(*PFN_WDF_DEVICE_PREPARE_HARDWARE)(
    IN  WDFDEVICE Device,
    IN  WDFCMRESLIST ResourcesRaw,
    IN  WDFCMRESLIST ResourcesTranslated
    );

typedef
NTSTATUS
(*PFN_WDF_DEVICE_RELEASE_HARDWARE)(
    IN  WDFDEVICE Device,
    IN  WDFCMRESLIST ResourcesTranslated
    );

typedef
VOID
(*PFN_WDF_DEVICE_SELF_MANAGED_IO_CLEANUP)(
    IN  WDFDEVICE Device
    );

typedef
VOID
(*PFN_WDF_DEVICE_SELF_MANAGED_IO_FLUSH)(
    IN  WDFDEVICE Device
    );

typedef
NTSTATUS
(*PFN_WDF_DEVICE_SELF_MANAGED_IO_INIT)(
    IN  WDFDEVICE Device
    );

typedef
NTSTATUS
(*PFN_WDF_DEVICE_SELF_MANAGED_IO_SUSPEND)(
    IN  WDFDEVICE Device
    );

typedef
NTSTATUS
(*PFN_WDF_DEVICE_SELF_MANAGED_IO_RESTART)(
    IN  WDFDEVICE Device
    );

typedef
NTSTATUS
(*PFN_WDF_DEVICE_QUERY_STOP)(
    IN WDFDEVICE Device
    );

typedef
NTSTATUS
(*PFN_WDF_DEVICE_QUERY_REMOVE)(
    IN WDFDEVICE Device
    );

typedef
VOID
(*PFN_WDF_DEVICE_SURPRISE_REMOVAL)(
    IN WDFDEVICE Device
    );

typedef
VOID
(*PFN_WDF_DEVICE_USAGE_NOTIFICATION)(
    IN WDFDEVICE Device,
    IN WDF_SPECIAL_FILE_TYPE NotificationType,
    IN BOOLEAN IsInNotificationPath
    );

typedef
VOID
(*PFN_WDF_DEVICE_RELATIONS_QUERY)(
    IN WDFDEVICE Device,
    IN DEVICE_RELATION_TYPE RelationType
    );

typedef
NTSTATUS
(*PFN_WDF_DEVICE_ARM_WAKE_FROM_S0)(
    IN WDFDEVICE Device
    );

typedef
NTSTATUS
(*PFN_WDF_DEVICE_ARM_WAKE_FROM_SX)(
    IN WDFDEVICE Device
    );

typedef
VOID
(*PFN_WDF_DEVICE_DISARM_WAKE_FROM_S0)(
    IN WDFDEVICE Device
    );

typedef
VOID
(*PFN_WDF_DEVICE_DISARM_WAKE_FROM_SX)(
    IN WDFDEVICE Device
    );

typedef
VOID
(*PFN_WDF_DEVICE_WAKE_FROM_S0_TRIGGERED)(
    IN WDFDEVICE Device
    );

typedef
VOID
(*PFN_WDF_DEVICE_WAKE_FROM_SX_TRIGGERED)(
    IN WDFDEVICE Device
    );

typedef struct _WDF_PNPPOWER_EVENT_CALLBACKS {
    //
    // Size of this structure in bytes
    //
    ULONG Size;

    PFN_WDF_DEVICE_D0_ENTRY                 EvtDeviceD0Entry;
    PFN_WDF_DEVICE_D0_ENTRY_POST_INTERRUPTS_ENABLED EvtDeviceD0EntryPostInterruptsEnabled;
    PFN_WDF_DEVICE_D0_EXIT                  EvtDeviceD0Exit;
    PFN_WDF_DEVICE_D0_EXIT_PRE_INTERRUPTS_DISABLED EvtDeviceD0ExitPreInterruptsDisabled;
    PFN_WDF_DEVICE_PREPARE_HARDWARE         EvtDevicePrepareHardware;
    PFN_WDF_DEVICE_RELEASE_HARDWARE         EvtDeviceReleaseHardware;
    PFN_WDF_DEVICE_SELF_MANAGED_IO_CLEANUP  EvtDeviceSelfManagedIoCleanup;
    PFN_WDF_DEVICE_SELF_MANAGED_IO_FLUSH    EvtDeviceSelfManagedIoFlush;
    PFN_WDF_DEVICE_SELF_MANAGED_IO_INIT     EvtDeviceSelfManagedIoInit;
    PFN_WDF_DEVICE_SELF_MANAGED_IO_SUSPEND  EvtDeviceSelfManagedIoSuspend;
    PFN_WDF_DEVICE_SELF_MANAGED_IO_RESTART  EvtDeviceSelfManagedIoRestart;
    PFN_WDF_DEVICE_SURPRISE_REMOVAL         EvtDeviceSurpriseRemoval;
    PFN_WDF_DEVICE_QUERY_REMOVE             EvtDeviceQueryRemove;
    PFN_WDF_DEVICE_QUERY_STOP               EvtDeviceQueryStop;
    PFN_WDF_DEVICE_USAGE_NOTIFICATION       EvtDeviceUsageNotification;
    PFN_WDF_DEVICE_RELATIONS_QUERY          EvtDeviceRelationsQuery;
} WDF_PNPPOWER_EVENT_CALLBACKS, *PWDF_PNPPOWER_EVENT_CALLBACKS;

typedef struct _WDF_POWER_POLICY_EVENT_CALLBACKS {
    //
    // Size of this structure in bytes
    //
    ULONG Size;

    PFN_WDF_DEVICE_ARM_WAKE_FROM_S0         EvtDeviceArmWakeFromS0;
    PFN_WDF_DEVICE_DISARM_WAKE_FROM_S0      EvtDeviceDisarmWakeFromS0;
    PFN_WDF_DEVICE_WAKE_FROM_S0_TRIGGERED   EvtDeviceWakeFromS0Triggered;

    PFN_WDF_DEVICE_ARM_WAKE_FROM_SX         EvtDeviceArmWakeFromSx;
    PFN_WDF_DEVICE_DISARM_WAKE_FROM_SX      EvtDeviceDisarmWakeFromSx;
    PFN_WDF_DEVICE_WAKE_FROM_SX_TRIGGERED   EvtDeviceWakeFromSxTriggered;

} WDF_POWER_POLICY_EVENT_CALLBACKS, *PWDF_POWER_POLICY_EVENT_CALLBACKS;

VOID
FORCEINLINE
WDF_POWER_POLICY_EVENT_CALLBACKS_INIT(
    PWDF_POWER_POLICY_EVENT_CALLBACKS Callbacks
    )
{
    RtlZeroMemory(Callbacks, sizeof(WDF_POWER_POLICY_EVENT_CALLBACKS));

    Callbacks->Size = sizeof(WDF_POWER_POLICY_EVENT_CALLBACKS);
}

VOID
FORCEINLINE
WDF_PNPPOWER_EVENT_CALLBACKS_INIT(
    OUT PWDF_PNPPOWER_EVENT_CALLBACKS Callbacks
    )
{
    RtlZeroMemory(Callbacks, sizeof(WDF_PNPPOWER_EVENT_CALLBACKS));
    Callbacks->Size = sizeof(WDF_PNPPOWER_EVENT_CALLBACKS);
}


ULONG
FORCEINLINE
WdfDevStateNormalize(
    ULONG State
    )
{
    return State & ~WdfDevStateNP;
}


BOOLEAN
FORCEINLINE
WdfDevStateIsNP(
    ULONG State
    )
{
    return (State & WdfDevStateNP) ? TRUE : FALSE;
}

typedef struct _WDF_DEVICE_POWER_POLICY_IDLE_SETTINGS {
    //
    // Size of this structure in bytes
    //
    ULONG Size;

    //
    // Indicates whether the device can wake itself up while the machine is in
    // S0.
    //
    WDF_POWER_POLICY_S0_IDLE_CAPABILITIES IdleCaps;

    //
    // The low power state in which the device will be placed when it is idled
    // out while the machine is in S0.
    //
    DEVICE_POWER_STATE DxState;

    //
    // Amount of time the device must be idle before idling out.  Timeout is in
    // milliseconds.
    //
    ULONG IdleTimeout;

    //
    // Inidcates whether a user can control the idle policy of the device.
    // By default, a user is allowed to change the policy.
    //
    WDF_POWER_POLICY_S0_IDLE_USER_CONTROL UserControlOfIdleSettings;

    //
    // If WdfTrue, idling out while the machine is in S0 will be enabled.
    //
    // If WdfFalse, idling out will be disabled.
    //
    // If WdfUseDefault, the idling out will be enabled.  If
    // UserControlOfIdleSettings is set to IdleAllowUserControl, the user's
    // settings will override the default.
    //
    WDF_TRI_STATE Enabled;

} WDF_DEVICE_POWER_POLICY_IDLE_SETTINGS, *PWDF_DEVICE_POWER_POLICY_IDLE_SETTINGS;

VOID
FORCEINLINE
WDF_DEVICE_POWER_POLICY_IDLE_SETTINGS_INIT(
    PWDF_DEVICE_POWER_POLICY_IDLE_SETTINGS Settings,
    WDF_POWER_POLICY_S0_IDLE_CAPABILITIES IdleCaps
    )
{
    RtlZeroMemory(Settings, sizeof(WDF_DEVICE_POWER_POLICY_IDLE_SETTINGS));

    Settings->Size = sizeof(WDF_DEVICE_POWER_POLICY_IDLE_SETTINGS);

    Settings->IdleTimeout = IdleTimeoutDefaultValue;
    Settings->UserControlOfIdleSettings = IdleAllowUserControl;
    Settings->Enabled = WdfUseDefault;

    Settings->IdleCaps = IdleCaps;

    switch (IdleCaps) {
    case IdleUsbSelectiveSuspend:
    case IdleCanWakeFromS0:
        Settings->DxState = PowerDeviceMaximum;
        break;

    case IdleCannotWakeFromS0:
        Settings->DxState = PowerDeviceD3;
        break;
    }
}

typedef struct _WDF_DEVICE_POWER_POLICY_WAKE_SETTINGS {
    //
    // Size of this structure in bytes
    //
    ULONG Size;

    //
    // The low power state in which the device will be placed when it is armed
    // for wake from Sx.
    //
    DEVICE_POWER_STATE DxState;

    //
    // Inidcates whether a user can control the idle policy of the device.
    // By default, a user is allowed to change the policy.
    //
    WDF_POWER_POLICY_SX_WAKE_USER_CONTROL UserControlOfWakeSettings;

    //
    // If WdfTrue, arming the device for wake while the machine is in Sx is
    // enabled.
    //
    // If WdfFalse, arming the device for wake while the machine is in Sx is
    // disabled.
    //
    // If WdfUseDefault, arming will be enabled.  If UserControlOfWakeSettings
    // is set to WakeAllowUserControl, the user's settings will override the
    // default.
    //
    WDF_TRI_STATE Enabled;

} WDF_DEVICE_POWER_POLICY_WAKE_SETTINGS, *PWDF_DEVICE_POWER_POLICY_WAKE_SETTINGS;

VOID
FORCEINLINE
WDF_DEVICE_POWER_POLICY_WAKE_SETTINGS_INIT(
    PWDF_DEVICE_POWER_POLICY_WAKE_SETTINGS Settings
    )
{
    RtlZeroMemory(Settings, sizeof(WDF_DEVICE_POWER_POLICY_WAKE_SETTINGS));

    Settings->Size = sizeof(WDF_DEVICE_POWER_POLICY_WAKE_SETTINGS);

    Settings->Enabled = WdfUseDefault;
    Settings->DxState = PowerDeviceMaximum;
    Settings->UserControlOfWakeSettings = WakeAllowUserControl;
}

typedef struct _WDF_DEVICE_STATE {
    //
    // Size of this structure in bytes
    //
    ULONG Size;

    //
    // If set to WdfTrue, the device will be disabled
    //
    WDF_TRI_STATE Disabled;

    //
    // If set to WdfTrue, the device will not be displayed in device manager.
    // Once hidden, the device cannot be unhidden.
    //
    WDF_TRI_STATE DontDisplayInUI;

    //
    // If set to WdfTrue, the device is reporting itself as failed.  If set
    // in conjuction with ResourcesChanged to WdfTrue, the device will receive
    // a PnP stop and then a new PnP start device.
    //
    WDF_TRI_STATE Failed;

    //
    // If set to WdfTrue, the device cannot be subsequently disabled.
    //
    WDF_TRI_STATE NotDisableable;


    //
    // If set to WdfTrue, the device stack will be torn down.
    //
    WDF_TRI_STATE Removed;

    //
    // If set to WdfTrue, the device will be sent another PnP start.  If the
    // Failed field is set to WdfTrue as well, a PnP stop will be sent before
    // the start.
    //
    WDF_TRI_STATE ResourcesChanged;

} WDF_DEVICE_STATE, *PWDF_DEVICE_STATE;

VOID
FORCEINLINE
WDF_DEVICE_STATE_INIT(
    OUT PWDF_DEVICE_STATE PnpDeviceState
    )
{
    RtlZeroMemory(PnpDeviceState, sizeof(WDF_DEVICE_STATE));

    PnpDeviceState->Size = sizeof(WDF_DEVICE_STATE);

    //
    // Initializes all of the fields to the WdfUseDefault enum value
    //
    PnpDeviceState->Disabled = WdfUseDefault;
    PnpDeviceState->DontDisplayInUI = WdfUseDefault;
    PnpDeviceState->Failed = WdfUseDefault;
    PnpDeviceState->NotDisableable = WdfUseDefault;
    PnpDeviceState->Removed = WdfUseDefault;
    PnpDeviceState->ResourcesChanged = WdfUseDefault;
}

typedef struct _WDF_DEVICE_PNP_CAPABILITIES {
    //
    // Size of the structure in bytes
    //
    ULONG Size;

    //
    // NOTE: To mark a PDO as raw, call WdfPdoInitAssignRawDevice
    //

    WDF_TRI_STATE LockSupported;
    WDF_TRI_STATE EjectSupported;
    WDF_TRI_STATE Removable;
    WDF_TRI_STATE DockDevice;
    WDF_TRI_STATE UniqueID;
    WDF_TRI_STATE SilentInstall;
    WDF_TRI_STATE SurpriseRemovalOK;
    WDF_TRI_STATE HardwareDisabled;
    WDF_TRI_STATE NoDisplayInUI;

    //
    // Default values of -1 indicate not to set this value
    //
    ULONG Address;
    ULONG UINumber;

} WDF_DEVICE_PNP_CAPABILITIES, *PWDF_DEVICE_PNP_CAPABILITIES;

VOID
FORCEINLINE
WDF_DEVICE_PNP_CAPABILITIES_INIT(
    PWDF_DEVICE_PNP_CAPABILITIES Caps
    )
{
    RtlZeroMemory(Caps, sizeof(WDF_DEVICE_PNP_CAPABILITIES));

    Caps->Size = sizeof(WDF_DEVICE_PNP_CAPABILITIES);

    Caps->LockSupported = WdfUseDefault;
    Caps->EjectSupported = WdfUseDefault;
    Caps->Removable = WdfUseDefault;
    Caps->DockDevice = WdfUseDefault;
    Caps->UniqueID = WdfUseDefault;
    Caps->SilentInstall = WdfUseDefault;
    Caps->SurpriseRemovalOK = WdfUseDefault;
    Caps->HardwareDisabled = WdfUseDefault;
    Caps->NoDisplayInUI = WdfUseDefault;

    Caps->Address = (ULONG) -1;
    Caps->UINumber = (ULONG) -1;
}

typedef struct _WDF_DEVICE_POWER_CAPABILITIES {
    //
    // Size of the structure in bytes
    //
    ULONG Size;

    WDF_TRI_STATE DeviceD1;
    WDF_TRI_STATE DeviceD2;

    WDF_TRI_STATE WakeFromD0;
    WDF_TRI_STATE WakeFromD1;
    WDF_TRI_STATE WakeFromD2;
    WDF_TRI_STATE WakeFromD3;

    //
    // Default value PowerDeviceMaximum indicates not to set this value
    //
    DEVICE_POWER_STATE DeviceState[PowerSystemMaximum];

    //
    // Default value PowerDeviceMaximum, PowerSystemMaximum indicates not to
    // set this value.
    //
    DEVICE_POWER_STATE DeviceWake;
    SYSTEM_POWER_STATE SystemWake;

    //
    // Default values of -1 indicate not to set this value
    //
    ULONG D1Latency;
    ULONG D2Latency;
    ULONG D3Latency;

    //
    // Ideal Dx state for the device to be put into when the machine moves into
    // Sx and the device is not armed for wake.  By default, the default will be
    // placed into D3.  If IdealDxStateForSx is lighter then
    // DeviceState[Sx], then DeviceState[Sx] will be used as the Dx state.
    //
    DEVICE_POWER_STATE IdealDxStateForSx;

} WDF_DEVICE_POWER_CAPABILITIES, *PWDF_DEVICE_POWER_CAPABILITIES;

VOID
FORCEINLINE
WDF_DEVICE_POWER_CAPABILITIES_INIT(
    PWDF_DEVICE_POWER_CAPABILITIES Caps
    )
{
    ULONG i;

    RtlZeroMemory(Caps, sizeof(WDF_DEVICE_POWER_CAPABILITIES));

    Caps->Size = sizeof(WDF_DEVICE_POWER_CAPABILITIES);

    Caps->DeviceD1 = WdfUseDefault;
    Caps->DeviceD2 = WdfUseDefault;

    Caps->WakeFromD0 = WdfUseDefault;
    Caps->WakeFromD1 = WdfUseDefault;
    Caps->WakeFromD2 = WdfUseDefault;
    Caps->WakeFromD3 = WdfUseDefault;

    for (i = 0; i < sizeof(Caps->DeviceState)/sizeof(Caps->DeviceState[1]); i++) {
        Caps->DeviceState[i] = PowerDeviceMaximum;
    }

    Caps->DeviceWake = PowerDeviceMaximum;
    Caps->SystemWake = PowerSystemMaximum;

    Caps->D1Latency = (ULONG) -1;
    Caps->D2Latency = (ULONG) -1;
    Caps->D3Latency = (ULONG) -1;

    Caps->IdealDxStateForSx = PowerDeviceMaximum;
}

typedef
NTSTATUS
(*PFN_WDFDEVICE_WDM_IRP_PREPROCESS)(
    WDFDEVICE Device,
    PIRP Irp
    );

//
// This is called to pre-process a request using METHOD_NEITHER
//
typedef
VOID
(*PFN_WDF_IO_IN_CALLER_CONTEXT) (
    IN WDFDEVICE  Device,
    IN WDFREQUEST Request
    );

//
// WDF Function: WdfDeviceGetDeviceState
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICEGETDEVICESTATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PWDF_DEVICE_STATE DeviceState
    );

VOID
FORCEINLINE
WdfDeviceGetDeviceState(
    __in
    WDFDEVICE Device,
    __out
    PWDF_DEVICE_STATE DeviceState
    )
{
    ((PFN_WDFDEVICEGETDEVICESTATE) WdfFunctions[WdfDeviceGetDeviceStateTableIndex])(WdfDriverGlobals, Device, DeviceState);
}

//
// WDF Function: WdfDeviceSetDeviceState
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICESETDEVICESTATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PWDF_DEVICE_STATE DeviceState
    );

VOID
FORCEINLINE
WdfDeviceSetDeviceState(
    __in
    WDFDEVICE Device,
    __in
    PWDF_DEVICE_STATE DeviceState
    )
{
    ((PFN_WDFDEVICESETDEVICESTATE) WdfFunctions[WdfDeviceSetDeviceStateTableIndex])(WdfDriverGlobals, Device, DeviceState);
}

//
// WDF Function: WdfWdmDeviceGetWdfDeviceHandle
//
typedef
WDFAPI
WDFDEVICE
(*PFN_WDFWDMDEVICEGETWDFDEVICEHANDLE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PDEVICE_OBJECT DeviceObject
    );

WDFDEVICE
FORCEINLINE
WdfWdmDeviceGetWdfDeviceHandle(
    __in
    PDEVICE_OBJECT DeviceObject
    )
{
    return ((PFN_WDFWDMDEVICEGETWDFDEVICEHANDLE) WdfFunctions[WdfWdmDeviceGetWdfDeviceHandleTableIndex])(WdfDriverGlobals, DeviceObject);
}

//
// WDF Function: WdfDeviceWdmGetDeviceObject
//
typedef
WDFAPI
PDEVICE_OBJECT
(*PFN_WDFDEVICEWDMGETDEVICEOBJECT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device
    );

PDEVICE_OBJECT
FORCEINLINE
WdfDeviceWdmGetDeviceObject(
    __in
    WDFDEVICE Device
    )
{
    return ((PFN_WDFDEVICEWDMGETDEVICEOBJECT) WdfFunctions[WdfDeviceWdmGetDeviceObjectTableIndex])(WdfDriverGlobals, Device);
}

//
// WDF Function: WdfDeviceWdmGetAttachedDevice
//
typedef
WDFAPI
PDEVICE_OBJECT
(*PFN_WDFDEVICEWDMGETATTACHEDDEVICE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device
    );

PDEVICE_OBJECT
FORCEINLINE
WdfDeviceWdmGetAttachedDevice(
    __in
    WDFDEVICE Device
    )
{
    return ((PFN_WDFDEVICEWDMGETATTACHEDDEVICE) WdfFunctions[WdfDeviceWdmGetAttachedDeviceTableIndex])(WdfDriverGlobals, Device);
}

//
// WDF Function: WdfDeviceWdmGetPhysicalDevice
//
typedef
WDFAPI
PDEVICE_OBJECT
(*PFN_WDFDEVICEWDMGETPHYSICALDEVICE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device
    );

PDEVICE_OBJECT
FORCEINLINE
WdfDeviceWdmGetPhysicalDevice(
    __in
    WDFDEVICE Device
    )
{
    return ((PFN_WDFDEVICEWDMGETPHYSICALDEVICE) WdfFunctions[WdfDeviceWdmGetPhysicalDeviceTableIndex])(WdfDriverGlobals, Device);
}

//
// WDF Function: WdfDeviceWdmDispatchPreprocessedIrp
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICEWDMDISPATCHPREPROCESSEDIRP)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PIRP Irp
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceWdmDispatchPreprocessedIrp(
    __in
    WDFDEVICE Device,
    __in
    PIRP Irp
    )
{
    return ((PFN_WDFDEVICEWDMDISPATCHPREPROCESSEDIRP) WdfFunctions[WdfDeviceWdmDispatchPreprocessedIrpTableIndex])(WdfDriverGlobals, Device, Irp);
}

//
// WDF Function: WdfDeviceAddDependentUsageDeviceObject
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICEADDDEPENDENTUSAGEDEVICEOBJECT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PDEVICE_OBJECT DependentDevice
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceAddDependentUsageDeviceObject(
    __in
    WDFDEVICE Device,
    __in
    PDEVICE_OBJECT DependentDevice
    )
{
    return ((PFN_WDFDEVICEADDDEPENDENTUSAGEDEVICEOBJECT) WdfFunctions[WdfDeviceAddDependentUsageDeviceObjectTableIndex])(WdfDriverGlobals, Device, DependentDevice);
}

//
// WDF Function: WdfDeviceAddRemovalRelationsPhysicalDevice
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICEADDREMOVALRELATIONSPHYSICALDEVICE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PDEVICE_OBJECT PhysicalDevice
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceAddRemovalRelationsPhysicalDevice(
    __in
    WDFDEVICE Device,
    __in
    PDEVICE_OBJECT PhysicalDevice
    )
{
    return ((PFN_WDFDEVICEADDREMOVALRELATIONSPHYSICALDEVICE) WdfFunctions[WdfDeviceAddRemovalRelationsPhysicalDeviceTableIndex])(WdfDriverGlobals, Device, PhysicalDevice);
}

//
// WDF Function: WdfDeviceRemoveRemovalRelationsPhysicalDevice
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICEREMOVEREMOVALRELATIONSPHYSICALDEVICE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PDEVICE_OBJECT PhysicalDevice
    );

VOID
FORCEINLINE
WdfDeviceRemoveRemovalRelationsPhysicalDevice(
    __in
    WDFDEVICE Device,
    __in
    PDEVICE_OBJECT PhysicalDevice
    )
{
    ((PFN_WDFDEVICEREMOVEREMOVALRELATIONSPHYSICALDEVICE) WdfFunctions[WdfDeviceRemoveRemovalRelationsPhysicalDeviceTableIndex])(WdfDriverGlobals, Device, PhysicalDevice);
}

//
// WDF Function: WdfDeviceClearRemovalRelationsDevices
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICECLEARREMOVALRELATIONSDEVICES)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device
    );

VOID
FORCEINLINE
WdfDeviceClearRemovalRelationsDevices(
    __in
    WDFDEVICE Device
    )
{
    ((PFN_WDFDEVICECLEARREMOVALRELATIONSDEVICES) WdfFunctions[WdfDeviceClearRemovalRelationsDevicesTableIndex])(WdfDriverGlobals, Device);
}

//
// WDF Function: WdfDeviceGetDriver
//
typedef
WDFAPI
WDFDRIVER
(*PFN_WDFDEVICEGETDRIVER)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device
    );

WDFDRIVER
FORCEINLINE
WdfDeviceGetDriver(
    __in
    WDFDEVICE Device
    )
{
    return ((PFN_WDFDEVICEGETDRIVER) WdfFunctions[WdfDeviceGetDriverTableIndex])(WdfDriverGlobals, Device);
}

//
// WDF Function: WdfDeviceRetrieveDeviceName
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICERETRIEVEDEVICENAME)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    WDFSTRING String
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceRetrieveDeviceName(
    __in
    WDFDEVICE Device,
    __in
    WDFSTRING String
    )
{
    return ((PFN_WDFDEVICERETRIEVEDEVICENAME) WdfFunctions[WdfDeviceRetrieveDeviceNameTableIndex])(WdfDriverGlobals, Device, String);
}

//
// WDF Function: WdfDeviceAssignMofResourceName
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICEASSIGNMOFRESOURCENAME)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PCUNICODE_STRING MofResourceName
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceAssignMofResourceName(
    __in
    WDFDEVICE Device,
    __in
    PCUNICODE_STRING MofResourceName
    )
{
    return ((PFN_WDFDEVICEASSIGNMOFRESOURCENAME) WdfFunctions[WdfDeviceAssignMofResourceNameTableIndex])(WdfDriverGlobals, Device, MofResourceName);
}

//
// WDF Function: WdfDeviceGetIoTarget
//
typedef
WDFAPI
WDFIOTARGET
(*PFN_WDFDEVICEGETIOTARGET)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device
    );

WDFIOTARGET
FORCEINLINE
WdfDeviceGetIoTarget(
    __in
    WDFDEVICE Device
    )
{
    return ((PFN_WDFDEVICEGETIOTARGET) WdfFunctions[WdfDeviceGetIoTargetTableIndex])(WdfDriverGlobals, Device);
}

//
// WDF Function: WdfDeviceGetDevicePnpState
//
typedef
WDFAPI
WDF_DEVICE_PNP_STATE
(*PFN_WDFDEVICEGETDEVICEPNPSTATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device
    );

WDF_DEVICE_PNP_STATE
FORCEINLINE
WdfDeviceGetDevicePnpState(
    __in
    WDFDEVICE Device
    )
{
    return ((PFN_WDFDEVICEGETDEVICEPNPSTATE) WdfFunctions[WdfDeviceGetDevicePnpStateTableIndex])(WdfDriverGlobals, Device);
}

//
// WDF Function: WdfDeviceGetDevicePowerState
//
typedef
WDFAPI
WDF_DEVICE_POWER_STATE
(*PFN_WDFDEVICEGETDEVICEPOWERSTATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device
    );

WDF_DEVICE_POWER_STATE
FORCEINLINE
WdfDeviceGetDevicePowerState(
    __in
    WDFDEVICE Device
    )
{
    return ((PFN_WDFDEVICEGETDEVICEPOWERSTATE) WdfFunctions[WdfDeviceGetDevicePowerStateTableIndex])(WdfDriverGlobals, Device);
}

//
// WDF Function: WdfDeviceGetDevicePowerPolicyState
//
typedef
WDFAPI
WDF_DEVICE_POWER_POLICY_STATE
(*PFN_WDFDEVICEGETDEVICEPOWERPOLICYSTATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device
    );

WDF_DEVICE_POWER_POLICY_STATE
FORCEINLINE
WdfDeviceGetDevicePowerPolicyState(
    __in
    WDFDEVICE Device
    )
{
    return ((PFN_WDFDEVICEGETDEVICEPOWERPOLICYSTATE) WdfFunctions[WdfDeviceGetDevicePowerPolicyStateTableIndex])(WdfDriverGlobals, Device);
}

//
// WDF Function: WdfDeviceAssignS0IdleSettings
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICEASSIGNS0IDLESETTINGS)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PWDF_DEVICE_POWER_POLICY_IDLE_SETTINGS Settings
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceAssignS0IdleSettings(
    __in
    WDFDEVICE Device,
    __in
    PWDF_DEVICE_POWER_POLICY_IDLE_SETTINGS Settings
    )
{
    return ((PFN_WDFDEVICEASSIGNS0IDLESETTINGS) WdfFunctions[WdfDeviceAssignS0IdleSettingsTableIndex])(WdfDriverGlobals, Device, Settings);
}

//
// WDF Function: WdfDeviceAssignSxWakeSettings
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICEASSIGNSXWAKESETTINGS)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PWDF_DEVICE_POWER_POLICY_WAKE_SETTINGS Settings
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceAssignSxWakeSettings(
    __in
    WDFDEVICE Device,
    __in
    PWDF_DEVICE_POWER_POLICY_WAKE_SETTINGS Settings
    )
{
    return ((PFN_WDFDEVICEASSIGNSXWAKESETTINGS) WdfFunctions[WdfDeviceAssignSxWakeSettingsTableIndex])(WdfDriverGlobals, Device, Settings);
}

//
// WDF Function: WdfDeviceOpenRegistryKey
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICEOPENREGISTRYKEY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    IN ULONG DeviceInstanceKeyType,
    IN ACCESS_MASK DesiredAccess,
    PWDF_OBJECT_ATTRIBUTES KeyAttributes,
    WDFKEY* Key
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceOpenRegistryKey(
    __in
    WDFDEVICE Device,
    IN ULONG DeviceInstanceKeyType,
    IN ACCESS_MASK DesiredAccess,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES KeyAttributes,
    __out
    WDFKEY* Key
    )
{
    return ((PFN_WDFDEVICEOPENREGISTRYKEY) WdfFunctions[WdfDeviceOpenRegistryKeyTableIndex])(WdfDriverGlobals, Device, DeviceInstanceKeyType, DesiredAccess, KeyAttributes, Key);
}

//
// WDF Function: WdfDeviceSetSpecialFileSupport
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICESETSPECIALFILESUPPORT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    IN WDF_SPECIAL_FILE_TYPE FileType,
    IN BOOLEAN FileTypeIsSupported
    );

VOID
FORCEINLINE
WdfDeviceSetSpecialFileSupport(
    __in
    WDFDEVICE Device,
    IN WDF_SPECIAL_FILE_TYPE FileType,
    IN BOOLEAN FileTypeIsSupported
    )
{
    ((PFN_WDFDEVICESETSPECIALFILESUPPORT) WdfFunctions[WdfDeviceSetSpecialFileSupportTableIndex])(WdfDriverGlobals, Device, FileType, FileTypeIsSupported);
}

//
// WDF Function: WdfDeviceSetCharacteristics
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICESETCHARACTERISTICS)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    IN ULONG DeviceCharacteristics
    );

VOID
FORCEINLINE
WdfDeviceSetCharacteristics(
    __in
    WDFDEVICE Device,
    IN ULONG DeviceCharacteristics
    )
{
    ((PFN_WDFDEVICESETCHARACTERISTICS) WdfFunctions[WdfDeviceSetCharacteristicsTableIndex])(WdfDriverGlobals, Device, DeviceCharacteristics);
}

//
// WDF Function: WdfDeviceGetCharacteristics
//
typedef
WDFAPI
ULONG
(*PFN_WDFDEVICEGETCHARACTERISTICS)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device
    );

ULONG
FORCEINLINE
WdfDeviceGetCharacteristics(
    __in
    WDFDEVICE Device
    )
{
    return ((PFN_WDFDEVICEGETCHARACTERISTICS) WdfFunctions[WdfDeviceGetCharacteristicsTableIndex])(WdfDriverGlobals, Device);
}

//
// WDF Function: WdfDeviceGetAlignmentRequirement
//
typedef
WDFAPI
ULONG
(*PFN_WDFDEVICEGETALIGNMENTREQUIREMENT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device
    );

ULONG
FORCEINLINE
WdfDeviceGetAlignmentRequirement(
    __in
    WDFDEVICE Device
    )
{
    return ((PFN_WDFDEVICEGETALIGNMENTREQUIREMENT) WdfFunctions[WdfDeviceGetAlignmentRequirementTableIndex])(WdfDriverGlobals, Device);
}

//
// WDF Function: WdfDeviceSetAlignmentRequirement
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICESETALIGNMENTREQUIREMENT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    IN ULONG AlignmentRequirement
    );

VOID
FORCEINLINE
WdfDeviceSetAlignmentRequirement(
    __in
    WDFDEVICE Device,
    IN ULONG AlignmentRequirement
    )
{
    ((PFN_WDFDEVICESETALIGNMENTREQUIREMENT) WdfFunctions[WdfDeviceSetAlignmentRequirementTableIndex])(WdfDriverGlobals, Device, AlignmentRequirement);
}

//
// WDF Function: WdfDeviceInitFree
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICEINITFREE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit
    );

VOID
FORCEINLINE
WdfDeviceInitFree(
    __in
    PWDFDEVICE_INIT DeviceInit
    )
{
    ((PFN_WDFDEVICEINITFREE) WdfFunctions[WdfDeviceInitFreeTableIndex])(WdfDriverGlobals, DeviceInit);
}

//
// WDF Function: WdfDeviceInitSetPnpPowerEventCallbacks
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICEINITSETPNPPOWEREVENTCALLBACKS)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    PWDF_PNPPOWER_EVENT_CALLBACKS PnpPowerEventCallbacks
    );

VOID
FORCEINLINE
WdfDeviceInitSetPnpPowerEventCallbacks(
    __in
    PWDFDEVICE_INIT DeviceInit,
    __in
    PWDF_PNPPOWER_EVENT_CALLBACKS PnpPowerEventCallbacks
    )
{
    ((PFN_WDFDEVICEINITSETPNPPOWEREVENTCALLBACKS) WdfFunctions[WdfDeviceInitSetPnpPowerEventCallbacksTableIndex])(WdfDriverGlobals, DeviceInit, PnpPowerEventCallbacks);
}

//
// WDF Function: WdfDeviceInitSetPowerPolicyEventCallbacks
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICEINITSETPOWERPOLICYEVENTCALLBACKS)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    PWDF_POWER_POLICY_EVENT_CALLBACKS PowerPolicyEventCallbacks
    );

VOID
FORCEINLINE
WdfDeviceInitSetPowerPolicyEventCallbacks(
    __in
    PWDFDEVICE_INIT DeviceInit,
    __in
    PWDF_POWER_POLICY_EVENT_CALLBACKS PowerPolicyEventCallbacks
    )
{
    ((PFN_WDFDEVICEINITSETPOWERPOLICYEVENTCALLBACKS) WdfFunctions[WdfDeviceInitSetPowerPolicyEventCallbacksTableIndex])(WdfDriverGlobals, DeviceInit, PowerPolicyEventCallbacks);
}

//
// WDF Function: WdfDeviceInitSetPowerPolicyOwnership
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICEINITSETPOWERPOLICYOWNERSHIP)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    IN BOOLEAN IsPowerPolicyOwner
    );

VOID
FORCEINLINE
WdfDeviceInitSetPowerPolicyOwnership(
    __in
    PWDFDEVICE_INIT DeviceInit,
    IN BOOLEAN IsPowerPolicyOwner
    )
{
    ((PFN_WDFDEVICEINITSETPOWERPOLICYOWNERSHIP) WdfFunctions[WdfDeviceInitSetPowerPolicyOwnershipTableIndex])(WdfDriverGlobals, DeviceInit, IsPowerPolicyOwner);
}

//
// WDF Function: WdfDeviceInitRegisterPnpStateChangeCallback
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICEINITREGISTERPNPSTATECHANGECALLBACK)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    WDF_DEVICE_PNP_STATE PnpState,
    PFN_WDF_DEVICE_PNP_STATE_CHANGE_NOTIFICATION EvtDevicePnpStateChange,
    ULONG CallbackTypes
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceInitRegisterPnpStateChangeCallback(
    __in
    PWDFDEVICE_INIT DeviceInit,
    WDF_DEVICE_PNP_STATE PnpState,
    __in
    PFN_WDF_DEVICE_PNP_STATE_CHANGE_NOTIFICATION EvtDevicePnpStateChange,
    ULONG CallbackTypes
    )
{
    return ((PFN_WDFDEVICEINITREGISTERPNPSTATECHANGECALLBACK) WdfFunctions[WdfDeviceInitRegisterPnpStateChangeCallbackTableIndex])(WdfDriverGlobals, DeviceInit, PnpState, EvtDevicePnpStateChange, CallbackTypes);
}

//
// WDF Function: WdfDeviceInitRegisterPowerStateChangeCallback
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICEINITREGISTERPOWERSTATECHANGECALLBACK)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    WDF_DEVICE_POWER_STATE PowerState,
    PFN_WDF_DEVICE_POWER_STATE_CHANGE_NOTIFICATION EvtDevicePowerStateChange,
    ULONG CallbackTypes
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceInitRegisterPowerStateChangeCallback(
    __in
    PWDFDEVICE_INIT DeviceInit,
    WDF_DEVICE_POWER_STATE PowerState,
    __in
    PFN_WDF_DEVICE_POWER_STATE_CHANGE_NOTIFICATION EvtDevicePowerStateChange,
    ULONG CallbackTypes
    )
{
    return ((PFN_WDFDEVICEINITREGISTERPOWERSTATECHANGECALLBACK) WdfFunctions[WdfDeviceInitRegisterPowerStateChangeCallbackTableIndex])(WdfDriverGlobals, DeviceInit, PowerState, EvtDevicePowerStateChange, CallbackTypes);
}

//
// WDF Function: WdfDeviceInitRegisterPowerPolicyStateChangeCallback
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICEINITREGISTERPOWERPOLICYSTATECHANGECALLBACK)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    WDF_DEVICE_POWER_POLICY_STATE PowerPolicyState,
    PFN_WDF_DEVICE_POWER_POLICY_STATE_CHANGE_NOTIFICATION EvtDevicePowerPolicyStateChange,
    ULONG CallbackTypes
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceInitRegisterPowerPolicyStateChangeCallback(
    __in
    PWDFDEVICE_INIT DeviceInit,
    WDF_DEVICE_POWER_POLICY_STATE PowerPolicyState,
    __in
    PFN_WDF_DEVICE_POWER_POLICY_STATE_CHANGE_NOTIFICATION EvtDevicePowerPolicyStateChange,
    ULONG CallbackTypes
    )
{
    return ((PFN_WDFDEVICEINITREGISTERPOWERPOLICYSTATECHANGECALLBACK) WdfFunctions[WdfDeviceInitRegisterPowerPolicyStateChangeCallbackTableIndex])(WdfDriverGlobals, DeviceInit, PowerPolicyState, EvtDevicePowerPolicyStateChange, CallbackTypes);
}

//
// WDF Function: WdfDeviceInitSetIoType
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICEINITSETIOTYPE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    IN WDF_DEVICE_IO_TYPE IoType
    );

VOID
FORCEINLINE
WdfDeviceInitSetIoType(
    __in
    PWDFDEVICE_INIT DeviceInit,
    IN WDF_DEVICE_IO_TYPE IoType
    )
{
    ((PFN_WDFDEVICEINITSETIOTYPE) WdfFunctions[WdfDeviceInitSetIoTypeTableIndex])(WdfDriverGlobals, DeviceInit, IoType);
}

//
// WDF Function: WdfDeviceInitSetExclusive
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICEINITSETEXCLUSIVE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    IN BOOLEAN IsExclusive
    );

VOID
FORCEINLINE
WdfDeviceInitSetExclusive(
    __in
    PWDFDEVICE_INIT DeviceInit,
    IN BOOLEAN IsExclusive
    )
{
    ((PFN_WDFDEVICEINITSETEXCLUSIVE) WdfFunctions[WdfDeviceInitSetExclusiveTableIndex])(WdfDriverGlobals, DeviceInit, IsExclusive);
}

//
// WDF Function: WdfDeviceInitSetPowerNotPageable
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICEINITSETPOWERNOTPAGEABLE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit
    );

VOID
FORCEINLINE
WdfDeviceInitSetPowerNotPageable(
    __in
    PWDFDEVICE_INIT DeviceInit
    )
{
    ((PFN_WDFDEVICEINITSETPOWERNOTPAGEABLE) WdfFunctions[WdfDeviceInitSetPowerNotPageableTableIndex])(WdfDriverGlobals, DeviceInit);
}

//
// WDF Function: WdfDeviceInitSetPowerPageable
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICEINITSETPOWERPAGEABLE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit
    );

VOID
FORCEINLINE
WdfDeviceInitSetPowerPageable(
    __in
    PWDFDEVICE_INIT DeviceInit
    )
{
    ((PFN_WDFDEVICEINITSETPOWERPAGEABLE) WdfFunctions[WdfDeviceInitSetPowerPageableTableIndex])(WdfDriverGlobals, DeviceInit);
}

//
// WDF Function: WdfDeviceInitSetPowerInrush
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICEINITSETPOWERINRUSH)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit
    );

VOID
FORCEINLINE
WdfDeviceInitSetPowerInrush(
    __in
    PWDFDEVICE_INIT DeviceInit
    )
{
    ((PFN_WDFDEVICEINITSETPOWERINRUSH) WdfFunctions[WdfDeviceInitSetPowerInrushTableIndex])(WdfDriverGlobals, DeviceInit);
}

//
// WDF Function: WdfDeviceInitSetDeviceType
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICEINITSETDEVICETYPE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    IN DEVICE_TYPE DeviceType
    );

VOID
FORCEINLINE
WdfDeviceInitSetDeviceType(
    __in
    PWDFDEVICE_INIT DeviceInit,
    IN DEVICE_TYPE DeviceType
    )
{
    ((PFN_WDFDEVICEINITSETDEVICETYPE) WdfFunctions[WdfDeviceInitSetDeviceTypeTableIndex])(WdfDriverGlobals, DeviceInit, DeviceType);
}

//
// WDF Function: WdfDeviceInitAssignName
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICEINITASSIGNNAME)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    PCUNICODE_STRING DeviceName
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceInitAssignName(
    __in
    PWDFDEVICE_INIT DeviceInit,
    __in_opt
    PCUNICODE_STRING DeviceName
    )
{
    return ((PFN_WDFDEVICEINITASSIGNNAME) WdfFunctions[WdfDeviceInitAssignNameTableIndex])(WdfDriverGlobals, DeviceInit, DeviceName);
}

//
// WDF Function: WdfDeviceInitAssignSDDLString
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICEINITASSIGNSDDLSTRING)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    PCUNICODE_STRING SDDLString
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceInitAssignSDDLString(
    __in
    PWDFDEVICE_INIT DeviceInit,
    __in
    PCUNICODE_STRING SDDLString
    )
{
    return ((PFN_WDFDEVICEINITASSIGNSDDLSTRING) WdfFunctions[WdfDeviceInitAssignSDDLStringTableIndex])(WdfDriverGlobals, DeviceInit, SDDLString);
}

//
// WDF Function: WdfDeviceInitSetDeviceClass
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICEINITSETDEVICECLASS)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    CONST GUID* DeviceClassGuid
    );

VOID
FORCEINLINE
WdfDeviceInitSetDeviceClass(
    __in
    PWDFDEVICE_INIT DeviceInit,
    __in
    CONST GUID* DeviceClassGuid
    )
{
    ((PFN_WDFDEVICEINITSETDEVICECLASS) WdfFunctions[WdfDeviceInitSetDeviceClassTableIndex])(WdfDriverGlobals, DeviceInit, DeviceClassGuid);
}

//
// WDF Function: WdfDeviceInitSetCharacteristics
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICEINITSETCHARACTERISTICS)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    IN ULONG DeviceCharacteristics,
    IN BOOLEAN OrInValues
    );

VOID
FORCEINLINE
WdfDeviceInitSetCharacteristics(
    __in
    PWDFDEVICE_INIT DeviceInit,
    IN ULONG DeviceCharacteristics,
    IN BOOLEAN OrInValues
    )
{
    ((PFN_WDFDEVICEINITSETCHARACTERISTICS) WdfFunctions[WdfDeviceInitSetCharacteristicsTableIndex])(WdfDriverGlobals, DeviceInit, DeviceCharacteristics, OrInValues);
}

//
// WDF Function: WdfDeviceInitSetFileObjectConfig
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICEINITSETFILEOBJECTCONFIG)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    PWDF_FILEOBJECT_CONFIG FileObjectConfig,
    PWDF_OBJECT_ATTRIBUTES FileObjectAttributes
    );

VOID
FORCEINLINE
WdfDeviceInitSetFileObjectConfig(
    __in
    PWDFDEVICE_INIT DeviceInit,
    __in
    PWDF_FILEOBJECT_CONFIG FileObjectConfig,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES FileObjectAttributes
    )
{
    ((PFN_WDFDEVICEINITSETFILEOBJECTCONFIG) WdfFunctions[WdfDeviceInitSetFileObjectConfigTableIndex])(WdfDriverGlobals, DeviceInit, FileObjectConfig, FileObjectAttributes);
}

//
// WDF Function: WdfDeviceInitSetRequestAttributes
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICEINITSETREQUESTATTRIBUTES)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    PWDF_OBJECT_ATTRIBUTES RequestAttributes
    );

VOID
FORCEINLINE
WdfDeviceInitSetRequestAttributes(
    __in
    PWDFDEVICE_INIT DeviceInit,
    __in
    PWDF_OBJECT_ATTRIBUTES RequestAttributes
    )
{
    ((PFN_WDFDEVICEINITSETREQUESTATTRIBUTES) WdfFunctions[WdfDeviceInitSetRequestAttributesTableIndex])(WdfDriverGlobals, DeviceInit, RequestAttributes);
}

//
// WDF Function: WdfDeviceInitAssignWdmIrpPreprocessCallback
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICEINITASSIGNWDMIRPPREPROCESSCALLBACK)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    PFN_WDFDEVICE_WDM_IRP_PREPROCESS EvtDevicePreprocessWdmIrp,
    UCHAR MajorFunction,
    PUCHAR MinorFunctions,
    ULONG NumMinorFunctions
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceInitAssignWdmIrpPreprocessCallback(
    __in
    PWDFDEVICE_INIT DeviceInit,
    __in
    PFN_WDFDEVICE_WDM_IRP_PREPROCESS EvtDevicePreprocessWdmIrp,
    UCHAR MajorFunction,
    __in_opt
    PUCHAR MinorFunctions,
    ULONG NumMinorFunctions
    )
{
    return ((PFN_WDFDEVICEINITASSIGNWDMIRPPREPROCESSCALLBACK) WdfFunctions[WdfDeviceInitAssignWdmIrpPreprocessCallbackTableIndex])(WdfDriverGlobals, DeviceInit, EvtDevicePreprocessWdmIrp, MajorFunction, MinorFunctions, NumMinorFunctions);
}

//
// WDF Function: WdfDeviceInitSetIoInCallerContextCallback
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICEINITSETIOINCALLERCONTEXTCALLBACK)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT DeviceInit,
    PFN_WDF_IO_IN_CALLER_CONTEXT EvtIoInCallerContext
    );

VOID
FORCEINLINE
WdfDeviceInitSetIoInCallerContextCallback(
    __in
    PWDFDEVICE_INIT DeviceInit,
    __in
    PFN_WDF_IO_IN_CALLER_CONTEXT EvtIoInCallerContext
    )
{
    ((PFN_WDFDEVICEINITSETIOINCALLERCONTEXTCALLBACK) WdfFunctions[WdfDeviceInitSetIoInCallerContextCallbackTableIndex])(WdfDriverGlobals, DeviceInit, EvtIoInCallerContext);
}

//
// WDF Function: WdfDeviceCreate
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICECREATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    PWDFDEVICE_INIT* DeviceInit,
    PWDF_OBJECT_ATTRIBUTES DeviceAttributes,
    WDFDEVICE* Device
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceCreate(
    PWDFDEVICE_INIT* DeviceInit,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES DeviceAttributes,
    __out
    WDFDEVICE* Device
    )
{
    return ((PFN_WDFDEVICECREATE) WdfFunctions[WdfDeviceCreateTableIndex])(WdfDriverGlobals, DeviceInit, DeviceAttributes, Device);
}

//
// WDF Function: WdfDeviceSetStaticStopRemove
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICESETSTATICSTOPREMOVE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    IN BOOLEAN Stoppable
    );

VOID
FORCEINLINE
WdfDeviceSetStaticStopRemove(
    __in
    WDFDEVICE Device,
    IN BOOLEAN Stoppable
    )
{
    ((PFN_WDFDEVICESETSTATICSTOPREMOVE) WdfFunctions[WdfDeviceSetStaticStopRemoveTableIndex])(WdfDriverGlobals, Device, Stoppable);
}

//
// WDF Function: WdfDeviceCreateDeviceInterface
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICECREATEDEVICEINTERFACE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    CONST GUID* InterfaceClassGUID,
    PCUNICODE_STRING ReferenceString
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceCreateDeviceInterface(
    __in
    WDFDEVICE Device,
    __in
    CONST GUID* InterfaceClassGUID,
    __in_opt
    PCUNICODE_STRING ReferenceString
    )
{
    return ((PFN_WDFDEVICECREATEDEVICEINTERFACE) WdfFunctions[WdfDeviceCreateDeviceInterfaceTableIndex])(WdfDriverGlobals, Device, InterfaceClassGUID, ReferenceString);
}

//
// WDF Function: WdfDeviceSetDeviceInterfaceState
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICESETDEVICEINTERFACESTATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    CONST GUID* InterfaceClassGUID,
    PCUNICODE_STRING ReferenceString,
    IN BOOLEAN IsInterfaceEnabled
    );

VOID
FORCEINLINE
WdfDeviceSetDeviceInterfaceState(
    __in
    WDFDEVICE Device,
    __in
    CONST GUID* InterfaceClassGUID,
    __in_opt
    PCUNICODE_STRING ReferenceString,
    IN BOOLEAN IsInterfaceEnabled
    )
{
    ((PFN_WDFDEVICESETDEVICEINTERFACESTATE) WdfFunctions[WdfDeviceSetDeviceInterfaceStateTableIndex])(WdfDriverGlobals, Device, InterfaceClassGUID, ReferenceString, IsInterfaceEnabled);
}

//
// WDF Function: WdfDeviceRetrieveDeviceInterfaceString
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICERETRIEVEDEVICEINTERFACESTRING)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    CONST GUID* InterfaceClassGUID,
    PCUNICODE_STRING ReferenceString,
    WDFSTRING String
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceRetrieveDeviceInterfaceString(
    __in
    WDFDEVICE Device,
    __in
    CONST GUID* InterfaceClassGUID,
    __in_opt
    PCUNICODE_STRING ReferenceString,
    __in
    WDFSTRING String
    )
{
    return ((PFN_WDFDEVICERETRIEVEDEVICEINTERFACESTRING) WdfFunctions[WdfDeviceRetrieveDeviceInterfaceStringTableIndex])(WdfDriverGlobals, Device, InterfaceClassGUID, ReferenceString, String);
}

//
// WDF Function: WdfDeviceCreateSymbolicLink
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICECREATESYMBOLICLINK)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PCUNICODE_STRING SymbolicLinkName
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceCreateSymbolicLink(
    __in
    WDFDEVICE Device,
    __in
    PCUNICODE_STRING SymbolicLinkName
    )
{
    return ((PFN_WDFDEVICECREATESYMBOLICLINK) WdfFunctions[WdfDeviceCreateSymbolicLinkTableIndex])(WdfDriverGlobals, Device, SymbolicLinkName);
}

//
// WDF Function: WdfDeviceQueryProperty
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICEQUERYPROPERTY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    IN DEVICE_REGISTRY_PROPERTY DeviceProperty,
    IN ULONG BufferLength,
    PVOID PropertyBuffer,
    OUT PULONG ResultLength
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceQueryProperty(
    __in
    WDFDEVICE Device,
    IN DEVICE_REGISTRY_PROPERTY DeviceProperty,
    IN ULONG BufferLength,
    __out_bcount_full(BufferLength)
    PVOID PropertyBuffer,
    __out
    OUT PULONG ResultLength
    )
{
    return ((PFN_WDFDEVICEQUERYPROPERTY) WdfFunctions[WdfDeviceQueryPropertyTableIndex])(WdfDriverGlobals, Device, DeviceProperty, BufferLength, PropertyBuffer, ResultLength);
}

//
// WDF Function: WdfDeviceAllocAndQueryProperty
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICEALLOCANDQUERYPROPERTY)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    IN DEVICE_REGISTRY_PROPERTY DeviceProperty,
    IN POOL_TYPE PoolType,
    PWDF_OBJECT_ATTRIBUTES PropertyMemoryAttributes,
    WDFMEMORY* PropertyMemory
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceAllocAndQueryProperty(
    __in
    WDFDEVICE Device,
    IN DEVICE_REGISTRY_PROPERTY DeviceProperty,
    IN POOL_TYPE PoolType,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES PropertyMemoryAttributes,
    __out
    WDFMEMORY* PropertyMemory
    )
{
    return ((PFN_WDFDEVICEALLOCANDQUERYPROPERTY) WdfFunctions[WdfDeviceAllocAndQueryPropertyTableIndex])(WdfDriverGlobals, Device, DeviceProperty, PoolType, PropertyMemoryAttributes, PropertyMemory);
}

//
// WDF Function: WdfDeviceSetPnpCapabilities
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICESETPNPCAPABILITIES)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PWDF_DEVICE_PNP_CAPABILITIES PnpCapabilities
    );

VOID
FORCEINLINE
WdfDeviceSetPnpCapabilities(
    __in
    WDFDEVICE Device,
    __in
    PWDF_DEVICE_PNP_CAPABILITIES PnpCapabilities
    )
{
    ((PFN_WDFDEVICESETPNPCAPABILITIES) WdfFunctions[WdfDeviceSetPnpCapabilitiesTableIndex])(WdfDriverGlobals, Device, PnpCapabilities);
}

//
// WDF Function: WdfDeviceSetPowerCapabilities
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICESETPOWERCAPABILITIES)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PWDF_DEVICE_POWER_CAPABILITIES PowerCapabilities
    );

VOID
FORCEINLINE
WdfDeviceSetPowerCapabilities(
    __in
    WDFDEVICE Device,
    __in
    PWDF_DEVICE_POWER_CAPABILITIES PowerCapabilities
    )
{
    ((PFN_WDFDEVICESETPOWERCAPABILITIES) WdfFunctions[WdfDeviceSetPowerCapabilitiesTableIndex])(WdfDriverGlobals, Device, PowerCapabilities);
}

//
// WDF Function: WdfDeviceSetBusInformationForChildren
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICESETBUSINFORMATIONFORCHILDREN)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PPNP_BUS_INFORMATION BusInformation
    );

VOID
FORCEINLINE
WdfDeviceSetBusInformationForChildren(
    __in
    WDFDEVICE Device,
    __in
    PPNP_BUS_INFORMATION BusInformation
    )
{
    ((PFN_WDFDEVICESETBUSINFORMATIONFORCHILDREN) WdfFunctions[WdfDeviceSetBusInformationForChildrenTableIndex])(WdfDriverGlobals, Device, BusInformation);
}

//
// WDF Function: WdfDeviceIndicateWakeStatus
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICEINDICATEWAKESTATUS)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    IN NTSTATUS WaitWakeStatus
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceIndicateWakeStatus(
    __in
    WDFDEVICE Device,
    IN NTSTATUS WaitWakeStatus
    )
{
    return ((PFN_WDFDEVICEINDICATEWAKESTATUS) WdfFunctions[WdfDeviceIndicateWakeStatusTableIndex])(WdfDriverGlobals, Device, WaitWakeStatus);
}

//
// WDF Function: WdfDeviceSetFailed
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICESETFAILED)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    IN WDF_DEVICE_FAILED_ACTION FailedAction
    );

VOID
FORCEINLINE
WdfDeviceSetFailed(
    __in
    WDFDEVICE Device,
    IN WDF_DEVICE_FAILED_ACTION FailedAction
    )
{
    ((PFN_WDFDEVICESETFAILED) WdfFunctions[WdfDeviceSetFailedTableIndex])(WdfDriverGlobals, Device, FailedAction);
}

//
// WDF Function: WdfDeviceStopIdle
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICESTOPIDLE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    IN BOOLEAN WaitForD0
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceStopIdle(
    __in
    WDFDEVICE Device,
    IN BOOLEAN WaitForD0
    )
{
    return ((PFN_WDFDEVICESTOPIDLE) WdfFunctions[WdfDeviceStopIdleTableIndex])(WdfDriverGlobals, Device, WaitForD0);
}

//
// WDF Function: WdfDeviceResumeIdle
//
typedef
WDFAPI
VOID
(*PFN_WDFDEVICERESUMEIDLE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device
    );

VOID
FORCEINLINE
WdfDeviceResumeIdle(
    __in
    WDFDEVICE Device
    )
{
    ((PFN_WDFDEVICERESUMEIDLE) WdfFunctions[WdfDeviceResumeIdleTableIndex])(WdfDriverGlobals, Device);
}

//
// WDF Function: WdfDeviceGetFileObject
//
typedef
WDFAPI
WDFFILEOBJECT
(*PFN_WDFDEVICEGETFILEOBJECT)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    PFILE_OBJECT FileObject
    );

WDFFILEOBJECT
FORCEINLINE
WdfDeviceGetFileObject(
    __in
    WDFDEVICE Device,
    __in
    PFILE_OBJECT FileObject
    )
{
    return ((PFN_WDFDEVICEGETFILEOBJECT) WdfFunctions[WdfDeviceGetFileObjectTableIndex])(WdfDriverGlobals, Device, FileObject);
}

//
// WDF Function: WdfDeviceEnqueueRequest
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICEENQUEUEREQUEST)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    WDFREQUEST Request
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceEnqueueRequest(
    __in
    WDFDEVICE Device,
    __in
    WDFREQUEST Request
    )
{
    return ((PFN_WDFDEVICEENQUEUEREQUEST) WdfFunctions[WdfDeviceEnqueueRequestTableIndex])(WdfDriverGlobals, Device, Request);
}

//
// WDF Function: WdfDeviceGetDefaultQueue
//
typedef
WDFAPI
WDFQUEUE
(*PFN_WDFDEVICEGETDEFAULTQUEUE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device
    );

WDFQUEUE
FORCEINLINE
WdfDeviceGetDefaultQueue(
    __in
    WDFDEVICE Device
    )
{
    return ((PFN_WDFDEVICEGETDEFAULTQUEUE) WdfFunctions[WdfDeviceGetDefaultQueueTableIndex])(WdfDriverGlobals, Device);
}

//
// WDF Function: WdfDeviceConfigureRequestDispatching
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDEVICECONFIGUREREQUESTDISPATCHING)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDEVICE Device,
    WDFQUEUE Queue,
    IN WDF_REQUEST_TYPE RequestType
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDeviceConfigureRequestDispatching(
    __in
    WDFDEVICE Device,
    __in
    WDFQUEUE Queue,
    IN WDF_REQUEST_TYPE RequestType
    )
{
    return ((PFN_WDFDEVICECONFIGUREREQUESTDISPATCHING) WdfFunctions[WdfDeviceConfigureRequestDispatchingTableIndex])(WdfDriverGlobals, Device, Queue, RequestType);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFDEVICE_H_

