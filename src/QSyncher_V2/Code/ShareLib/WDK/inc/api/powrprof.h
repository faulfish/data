
/*****************************************************************************\
*                                                                             *
* powrprof.h - - Interface for powrprof.dll, the power policy applicator      *
*                                                                             *
* Version 1.0                                                                 *
*                                                                             *
* Copyright (c) Microsoft Corporation. All rights reserved.                   *
*                                                                             *
\*****************************************************************************/

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Registry storage structures for the GLOBAL_POWER_POLICY data. There are two
// structures, GLOBAL_MACHINE_POWER_POLICY and GLOBAL_USER_POWER_POLICY. the
// GLOBAL_MACHINE_POWER_POLICY stores per machine data for which there is no UI.
// GLOBAL_USER_POWER_POLICY stores the per user data.

typedef struct _GLOBAL_MACHINE_POWER_POLICY{
    ULONG                   Revision;
    SYSTEM_POWER_STATE      LidOpenWakeAc;
    SYSTEM_POWER_STATE      LidOpenWakeDc;
    ULONG                   BroadcastCapacityResolution;
} GLOBAL_MACHINE_POWER_POLICY, *PGLOBAL_MACHINE_POWER_POLICY;

typedef struct _GLOBAL_USER_POWER_POLICY{
    ULONG                   Revision;
    POWER_ACTION_POLICY     PowerButtonAc;
    POWER_ACTION_POLICY     PowerButtonDc;
    POWER_ACTION_POLICY     SleepButtonAc;
    POWER_ACTION_POLICY     SleepButtonDc;
    POWER_ACTION_POLICY     LidCloseAc;
    POWER_ACTION_POLICY     LidCloseDc;
    SYSTEM_POWER_LEVEL      DischargePolicy[NUM_DISCHARGE_POLICIES];
    ULONG                   GlobalFlags;
} GLOBAL_USER_POWER_POLICY, *PGLOBAL_USER_POWER_POLICY;

// Structure to manage global power policies at the user level. This structure
// contains data which is common across all power policy profiles.

typedef struct _GLOBAL_POWER_POLICY{
    GLOBAL_USER_POWER_POLICY    user;
    GLOBAL_MACHINE_POWER_POLICY mach;
} GLOBAL_POWER_POLICY, *PGLOBAL_POWER_POLICY;


// Registry storage structures for the POWER_POLICY data. There are three
// structures, MACHINE_POWER_POLICY, MACHINE_PROCESSOR_POWER_POLICY and
// USER_POWER_POLICY. the MACHINE_POWER_POLICY stores per machine data for 
// which there is no UI.  USER_POWER_POLICY stores the per user data.

typedef struct _MACHINE_POWER_POLICY{
    ULONG                   Revision;       // 1

    // meaning of power action "sleep"
    SYSTEM_POWER_STATE      MinSleepAc;
    SYSTEM_POWER_STATE      MinSleepDc;
    SYSTEM_POWER_STATE      ReducedLatencySleepAc;
    SYSTEM_POWER_STATE      ReducedLatencySleepDc;

    // parameters for dozing
    ULONG                   DozeTimeoutAc;
    ULONG                   DozeTimeoutDc;
    ULONG                   DozeS4TimeoutAc;
    ULONG                   DozeS4TimeoutDc;

    // processor policies
    UCHAR                   MinThrottleAc;
    UCHAR                   MinThrottleDc;
    UCHAR                   pad1[2];
    POWER_ACTION_POLICY     OverThrottledAc;
    POWER_ACTION_POLICY     OverThrottledDc;

} MACHINE_POWER_POLICY, *PMACHINE_POWER_POLICY;


#if (NTDDI_VERSION >= NTDDI_WINXP) // Windows XP Version
//
// deprecated
//
typedef struct _MACHINE_PROCESSOR_POWER_POLICY {
    ULONG                   Revision;       // 1
    
    PROCESSOR_POWER_POLICY  ProcessorPolicyAc;    
    PROCESSOR_POWER_POLICY  ProcessorPolicyDc;    

} MACHINE_PROCESSOR_POWER_POLICY, *PMACHINE_PROCESSOR_POWER_POLICY;
#endif

typedef struct _USER_POWER_POLICY{
    ULONG                   Revision;       // 1


    // "system idle" detection
    POWER_ACTION_POLICY     IdleAc;
    POWER_ACTION_POLICY     IdleDc;
    ULONG                   IdleTimeoutAc;
    ULONG                   IdleTimeoutDc;
    UCHAR                   IdleSensitivityAc;
    UCHAR                   IdleSensitivityDc;
    
    // Throttling Policy
    UCHAR                   ThrottlePolicyAc;
    UCHAR                   ThrottlePolicyDc;

    // meaning of power action "sleep"
    SYSTEM_POWER_STATE      MaxSleepAc;
    SYSTEM_POWER_STATE      MaxSleepDc;

    // For future use
    ULONG                   Reserved[2];

    // video policies
    ULONG                   VideoTimeoutAc;
    ULONG                   VideoTimeoutDc;

    // hard disk policies
    ULONG                   SpindownTimeoutAc;
    ULONG                   SpindownTimeoutDc;

    // processor policies
    BOOLEAN                 OptimizeForPowerAc;
    BOOLEAN                 OptimizeForPowerDc;
    UCHAR                   FanThrottleToleranceAc;
    UCHAR                   FanThrottleToleranceDc;
    UCHAR                   ForcedThrottleAc;
    UCHAR                   ForcedThrottleDc;
    
} USER_POWER_POLICY, *PUSER_POWER_POLICY;

// Structure to manage power policies at the user level. This structure
// contains data which is unique across power policy profiles.

typedef struct _POWER_POLICY{
    USER_POWER_POLICY       user;
    MACHINE_POWER_POLICY    mach;
} POWER_POLICY, *PPOWER_POLICY;


// Constants for GlobalFlags

#define EnableSysTrayBatteryMeter   0x01
#define EnableMultiBatteryDisplay   0x02
#define EnablePasswordLogon         0x04
#define EnableWakeOnRing            0x08
#define EnableVideoDimDisplay       0x10

//
// Power setting attribute flags
//

#define POWER_ATTRIBUTE_HIDE        0x00000001

// This constant is passed as a uiID to WritePwrScheme.
#define NEWSCHEME (UINT)-1

// Prototype for EnumPwrSchemes callback proceedures.

#if (NTDDI_VERSION >= NTDDI_LONGHORN)
typedef BOOLEAN (CALLBACK* PWRSCHEMESENUMPROC)(UINT, DWORD, LPWSTR, DWORD, LPWSTR, PPOWER_POLICY, LPARAM);
#else
typedef BOOLEAN (CALLBACK* PWRSCHEMESENUMPROC)(UINT, DWORD, LPTSTR, DWORD, LPTSTR, PPOWER_POLICY, LPARAM);
#endif
typedef BOOLEAN (CALLBACK* PFNNTINITIATEPWRACTION)(POWER_ACTION, SYSTEM_POWER_STATE, ULONG, BOOLEAN);

// Public function prototypes

BOOLEAN
WINAPI
GetPwrDiskSpindownRange(
    __out PUINT puiMax,
    __out PUINT puiMin
    );   

BOOLEAN 
WINAPI
EnumPwrSchemes(
    __in PWRSCHEMESENUMPROC lpfn,
    __in LPARAM lParam
    );

BOOLEAN 
WINAPI ReadGlobalPwrPolicy(
        __in PGLOBAL_POWER_POLICY pGlobalPowerPolicy
    );
        
BOOLEAN 
WINAPI 
ReadPwrScheme(
        __in UINT uiID,
    __out PPOWER_POLICY pPowerPolicy
    );
    
BOOLEAN
WINAPI
WritePwrScheme(
    __in PUINT puiID,
    __in LPCWSTR lpszSchemeName,
    __in_opt LPCWSTR lpszDescription,
    __in PPOWER_POLICY lpScheme
    );

BOOLEAN 
WINAPI 
WriteGlobalPwrPolicy(
        __in PGLOBAL_POWER_POLICY pGlobalPowerPolicy
    );
        
BOOLEAN 
WINAPI 
DeletePwrScheme(
        __in UINT uiID
        );
        
BOOLEAN 
WINAPI 
GetActivePwrScheme(
        __out PUINT puiID
    );
        
BOOLEAN
WINAPI
SetActivePwrScheme(
    __in UINT uiID,
    __in_opt PGLOBAL_POWER_POLICY pGlobalPowerPolicy,
    __in_opt PPOWER_POLICY pPowerPolicy
    );
    
BOOLEAN 
WINAPI 
GetPwrCapabilities(
        __in PSYSTEM_POWER_CAPABILITIES lpspc
    );

BOOLEAN 
WINAPI
IsPwrSuspendAllowed(
        VOID
        );

BOOLEAN 
WINAPI
IsPwrHibernateAllowed(
        VOID
        );
        
BOOLEAN 
WINAPI
IsPwrShutdownAllowed(
        VOID
        );

BOOLEAN 
WINAPI
IsAdminOverrideActive(
    __in PADMINISTRATOR_POWER_POLICY papp
    );
    
BOOLEAN 
WINAPI 
SetSuspendState(
        __in BOOLEAN bHibernate,
    __in BOOLEAN bForce,
    __in BOOLEAN bWakeupEventsDisabled
    );
    
BOOLEAN 
WINAPI 
GetCurrentPowerPolicies(
        __out PGLOBAL_POWER_POLICY pGlobalPowerPolicy,
    __out PPOWER_POLICY pPowerPolicy
    );

BOOLEAN 
WINAPI 
CanUserWritePwrScheme(
        VOID
        );


#if (NTDDI_VERSION >= NTDDI_WINXP) // Windows XP Version
//
// deprecated.
//
BOOLEAN 
WINAPI 
ReadProcessorPwrScheme(
        __in UINT uiID,
    __out PMACHINE_PROCESSOR_POWER_POLICY pMachineProcessorPowerPolicy
    );

//
// deprecated.
//    
BOOLEAN 
WINAPI 
WriteProcessorPwrScheme(
        __in UINT uiID,
    __in PMACHINE_PROCESSOR_POWER_POLICY pMachineProcessorPowerPolicy
    );
#endif // (NTDDI_VERSION >= NTDDI_WINXP) // Windows XP Version

BOOLEAN 
WINAPI 
ValidatePowerPolicies(
        __inout_opt PGLOBAL_POWER_POLICY pGlobalPowerPolicy,
    __inout_opt PPOWER_POLICY pPowerPolicy
    );
    
#ifndef NT_SUCCESS
#define NTSTATUS LONG
#define _OVERRIDE_NTSTATUS_
#endif

NTSTATUS WINAPI CallNtPowerInformation(POWER_INFORMATION_LEVEL, __in_bcount_opt(cbin) PVOID pvin, ULONG cbin, __out_bcount_opt(cbout) PVOID pvout, ULONG cbout);

#ifdef _OVERRIDE_NTSTATUS_
#undef NTSTATUS
#endif

#if (NTDDI_VERSION >= NTDDI_LONGHORN)
//
// Enum which defines which field inside of a
// power setting is being accessed.
//
typedef enum _POWER_DATA_ACCESSOR {
        
        //
        // Used by read/write and enumeration engines
        //
        ACCESS_AC_POWER_SETTING_INDEX = 0,
        ACCESS_DC_POWER_SETTING_INDEX,
        ACCESS_FRIENDLY_NAME,
        ACCESS_DESCRIPTION,
        ACCESS_POSSIBLE_POWER_SETTING,
        ACCESS_POSSIBLE_POWER_SETTING_FRIENDLY_NAME,
        ACCESS_POSSIBLE_POWER_SETTING_DESCRIPTION,
        ACCESS_DEFAULT_AC_POWER_SETTING,
        ACCESS_DEFAULT_DC_POWER_SETTING,
        ACCESS_POSSIBLE_VALUE_MIN,
        ACCESS_POSSIBLE_VALUE_MAX,
        ACCESS_POSSIBLE_VALUE_INCREMENT,
        ACCESS_POSSIBLE_VALUE_UNITS,
        ACCESS_ICON_RESOURCE,
        ACCESS_DEFAULT_SECURITY_DESCRIPTOR,
        ACCESS_ATTRIBUTES,
        
        //
        // Used by enumeration engine.
        //
        ACCESS_SCHEME,
        ACCESS_SUBGROUP,
        ACCESS_INDIVIDUAL_SETTING,
        
        //
        // Used by access check
        //
        ACCESS_ACTIVE_SCHEME,
        ACCESS_CREATE_SCHEME
        
} POWER_DATA_ACCESSOR, *PPOWER_DATA_ACCESSOR;

//
// =========================================
// Power Scheme APIs
// =========================================
//

DWORD
WINAPI
PowerSettingAccessCheck (
    __in POWER_DATA_ACCESSOR AccessFlags,
    __in_opt CONST GUID *PowerGuid
    );

//
// Read functions.
//
DWORD
WINAPI
PowerReadACValueIndex (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SchemeGuid,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __out LPDWORD AcValueIndex
    );

DWORD
WINAPI
PowerReadDCValueIndex (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SchemeGuid,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __out LPDWORD DcValueIndex
    );

DWORD
WINAPI
PowerReadACValue (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SchemeGuid,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __out_opt PULONG Type,
    __out_bcount_opt(*BufferSize) LPBYTE Buffer,
    __inout_opt LPDWORD BufferSize
    );

DWORD
WINAPI
PowerReadDCValue (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SchemeGuid,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __out_opt PULONG Type,
    __out_bcount_opt(*BufferSize) PUCHAR Buffer,
    __inout LPDWORD BufferSize
    );

DWORD
WINAPI
PowerReadFriendlyName (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SchemeGuid,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __out_bcount_opt(*BufferSize) PUCHAR Buffer,
    __inout LPDWORD BufferSize
    );

DWORD
WINAPI
PowerReadDescription (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SchemeGuid,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __out_bcount_opt(*BufferSize) PUCHAR Buffer,
    __inout LPDWORD BufferSize
    );

DWORD
WINAPI
PowerReadPossibleValue (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __out_opt PULONG Type,
    __in ULONG PossibleSettingIndex,
    __out_bcount_opt(*BufferSize) PUCHAR Buffer,
    __inout LPDWORD BufferSize
    );

DWORD
WINAPI
PowerReadPossibleFriendlyName (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __in ULONG PossibleSettingIndex,
    __out_bcount_opt(*BufferSize) PUCHAR Buffer,
    __inout LPDWORD BufferSize
    );

DWORD
WINAPI
PowerReadPossibleDescription (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __in ULONG PossibleSettingIndex,
    __out_bcount_opt(*BufferSize) PUCHAR Buffer,
    __inout LPDWORD BufferSize
    );

DWORD
WINAPI
PowerReadValueMin (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __out LPDWORD ValueMinimum
    );

DWORD
WINAPI
PowerReadValueMax (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __out LPDWORD ValueMaximum
    );

DWORD
WINAPI
PowerReadValueIncrement (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __out LPDWORD ValueIncrement
    );

DWORD
WINAPI
PowerReadValueUnitsSpecifier (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __out_bcount_opt(*BufferSize) UCHAR *Buffer,
    __inout LPDWORD BufferSize
    );

DWORD
WINAPI
PowerReadACDefaultIndex (
    __in_opt HKEY RootPowerKey,
    __in CONST GUID *SchemePersonalityGuid,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid OPTIONAL,
    __in CONST GUID *PowerSettingGuid,
    __out LPDWORD AcDefaultIndex
    );

DWORD
WINAPI
PowerReadDCDefaultIndex (
    __in_opt HKEY RootPowerKey,
    __in CONST GUID *SchemePersonalityGuid,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in CONST GUID *PowerSettingGuid,
    __out LPDWORD DcDefaultIndex
    );

DWORD
WINAPI
PowerReadIconResourceSpecifier (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SchemeGuid,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __out_bcount_opt(*BufferSize) PUCHAR Buffer,
    __inout LPDWORD BufferSize
    );
    
DWORD
WINAPI
PowerReadSettingAttributes (
    __in_opt CONST GUID *SubGroupGuid,
    __in_opt CONST GUID *PowerSettingGuid
    );

//
// Write functions.
//
DWORD
WINAPI
PowerWriteACValueIndex (
    __in_opt HKEY RootPowerKey,
    __in CONST GUID *SchemeGuid,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __in DWORD AcValueIndex
    );

DWORD
WINAPI
PowerWriteDCValueIndex (
    __in_opt HKEY RootPowerKey,
    __in CONST GUID *SchemeGuid,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __in DWORD DcValueIndex
    );

DWORD
WINAPI
PowerWriteFriendlyName (
    __in_opt HKEY RootPowerKey,
    __in CONST GUID *SchemeGuid,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __in UCHAR *Buffer,
    __in DWORD BufferSize
    );

DWORD
WINAPI
PowerWriteDescription (
    __in_opt HKEY RootPowerKey,
    __in CONST GUID *SchemeGuid,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __in_bcount(BufferSize) UCHAR *Buffer,
    __in DWORD BufferSize
    );

DWORD
WINAPI
PowerWritePossibleValue (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __in ULONG Type,
    __in ULONG PossibleSettingIndex,
    __in_bcount(BufferSize) UCHAR *Buffer,
    __in DWORD BufferSize
    );

DWORD
WINAPI
PowerWritePossibleFriendlyName (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __in ULONG PossibleSettingIndex,
    __in_bcount(BufferSize) UCHAR *Buffer,
    __in DWORD BufferSize
    );

DWORD
WINAPI
PowerWritePossibleDescription (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __in ULONG PossibleSettingIndex,
    __in_bcount(BufferSize) UCHAR *Buffer,
    __in DWORD BufferSize
    );

DWORD
WINAPI
PowerWriteValueMin (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __in DWORD ValueMinimum
    );

DWORD
WINAPI
PowerWriteValueMax (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __in DWORD ValueMaximum
    );
    
DWORD
WINAPI
PowerWriteValueIncrement (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __in DWORD ValueIncrement
    );

DWORD
WINAPI
PowerWriteValueUnitsSpecifier (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __in_bcount(BufferSize) UCHAR *Buffer,
    __in DWORD BufferSize
    );

DWORD
WINAPI
PowerWriteACDefaultIndex (
    __in_opt HKEY RootSystemPowerKey,
    __in CONST GUID *SchemePersonalityGuid,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in CONST GUID *PowerSettingGuid,
    __in DWORD DefaultAcIndex
    );

DWORD
WINAPI
PowerWriteDCDefaultIndex (
    __in_opt HKEY RootSystemPowerKey,
    __in CONST GUID *SchemePersonalityGuid,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in CONST GUID *PowerSettingGuid,
    __in DWORD DefaultDcIndex
    );

DWORD
WINAPI
PowerWriteIconResourceSpecifier (
    __in_opt HKEY RootPowerKey,
    __in CONST GUID *SchemeGuid,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __in_bcount(BufferSize) UCHAR *Buffer,
    __in DWORD BufferSize
    );
    
DWORD
PowerWriteSettingAttributes (
    __in_opt CONST GUID *SubGroupGuid,
    __in_opt CONST GUID *PowerSettingGuid,
    __in DWORD Attributes
    );

DWORD
WINAPI
PowerDuplicateScheme (
    __in_opt HKEY RootPowerKey,
    __in CONST GUID *SourceSchemeGuid,
    __in GUID **DestinationSchemeGuid
    );
    
DWORD
PowerImportPowerScheme (
    __in_opt HKEY RootPowerKey,
    __in LPCWSTR ImportFileNamePath,
    __deref_inout GUID **DestinationSchemeGuid
    );

DWORD
WINAPI
PowerDeleteScheme (
    __in_opt HKEY RootPowerKey,
    __in CONST GUID *SchemeGuid
    );

DWORD 
WINAPI
PowerRemovePowerSetting (
   __in CONST GUID *PowerSettingSubKeyGuid,
   __in CONST GUID *PowerSettingGuid
   );

DWORD
WINAPI
PowerCreateSetting (
    __in_opt HKEY RootSystemPowerKey,
    __in CONST GUID *SubGroupOfPowerSettingsGuid,
    __in CONST GUID *PowerSettingGuid
    );

DWORD
WINAPI
PowerCreatePossibleSetting (
    __in_opt HKEY RootSystemPowerKey,
    __in CONST GUID *SubGroupOfPowerSettingsGuid,
    __in CONST GUID *PowerSettingGuid,
    __in ULONG PossibleSettingIndex 
    );

//
// Enumerate Functions.
//
DWORD
WINAPI
PowerEnumerate (
    __in_opt HKEY RootPowerKey,
    __in_opt CONST GUID *SchemeGuid,
    __in_opt CONST GUID *SubGroupOfPowerSettingsGuid,
    __in POWER_DATA_ACCESSOR AccessFlags,
    __in ULONG Index,
    __out_bcount_opt(*BufferSize) UCHAR *Buffer,
    __inout DWORD *BufferSize
    );

//
// Activate Functions.
//
DWORD
WINAPI
PowerSetActiveScheme (
    __in_opt HKEY UserRootPowerKey,
    __in_opt CONST GUID *SchemeGuid
    );

DWORD
WINAPI
PowerGetActiveScheme (
    __in_opt HKEY UserRootPowerKey,
    __deref_out GUID **ActivePolicyGuid
    );
    
DWORD
PowerOpenUserPowerKey (
    __out HKEY *phUserPowerKey,
    __in REGSAM Access,
    __in BOOL OpenExisting
    );

DWORD
PowerOpenSystemPowerKey (
    __out HKEY *phSystemPowerKey,
    __in REGSAM Access,
    __in BOOL OpenExisting
    );
        
DWORD
WINAPI
PowerCanRestoreIndividualDefaultPowerScheme (
    __in CONST GUID *SchemeGuid
    );

DWORD
WINAPI
PowerRestoreIndividualDefaultPowerScheme (
    __in CONST GUID *SchemeGuid
    );

DWORD
WINAPI
PowerRestoreDefaultPowerSchemes(
    VOID
    );

DWORD
WINAPI
PowerReplaceDefaultPowerSchemes(
    VOID
    );

POWER_PLATFORM_ROLE
WINAPI
PowerDeterminePlatformRole(
    VOID
    );

#endif // #if (NTDDI_VERSION >= NTDDI_LONGHORN)




#if (NTDDI_VERSION >= NTDDI_WS03) // Windows server03

//
// =========================================
// Device Power APIs
// =========================================
//
//
// ========================================================
// These flags tell us how to interpret a query of
// device power.  Use these (or a combination of these)
// for the QueryInterpretationFlags parameter sent into
// DevicePowerEnumDevices().
//
// They'll ask us for some devices that supports some
// D state.  These flags help us determine what the
// user really wants.  I.e. does he want the device
// name, or the hardware ID.  Does he want some device
// that supports some D state, or does he want us to
// go see if that devices supports some S state? ...
// ========================================================
//

//
// Return HardwareID instead of the friendly device name, which
// is the default.
//
#define DEVICEPOWER_HARDWAREID             (0x80000000)

//
//
// 'and' the requested power states.  I.e. if the user requested
// that we query devices supporting D1 and D3, this flag says
// "give me devices that support D1 *and* D3".  Without this
// flag, the query would be interpreted as "give me devices
// that support D1 *or* D3".
//
#define DEVICEPOWER_AND_OPERATION          (0x40000000)

// Only preform the query on devices that are present in the system.
//
#define DEVICEPOWER_FILTER_DEVICES_PRESENT (0x20000000)

//
// Only preform the query on devices that are actual hardware.
//
#define DEVICEPOWER_FILTER_HARDWARE        (0x10000000)

//
// Only preform the query on devices that are armed to wake the
// system from a sleep state.
//
#define DEVICEPOWER_FILTER_WAKEENABLED     (0x08000000)

//
// Only preform the query on devices that are capable of being programmed
// to wake the system from a sleep state.
//
#define DEVICEPOWER_FILTER_WAKEPROGRAMMABLE (0x04000000)

//
// Go find the device who's name is specifed by an input
// parameter, then see if it's got the capabilities specified
// in QueryFlags.
//
#define DEVICEPOWER_FILTER_ON_NAME         (0x02000000)



//
// Define flags to set/clear capabilities in the devices.
//

//
// Enable the device to wake the system from a sleep state.
//
#define DEVICEPOWER_SET_WAKEENABLED (0x00000001)

//
// Disable the device from waking the system from a sleep
// state.
//
#define DEVICEPOWER_CLEAR_WAKEENABLED (0x00000002)




//
// For the QueryFlags parameter, the user should send in
// one of the PDCAP_D*_SUPPORTED flags found in ntpoapi.h.
//
// #define PDCAP_D0_SUPPORTED              0x00000001
// #define PDCAP_D1_SUPPORTED              0x00000002
// #define PDCAP_D2_SUPPORTED              0x00000004
// #define PDCAP_D3_SUPPORTED              0x00000008
// #define PDCAP_WAKE_FROM_D0_SUPPORTED    0x00000010
// #define PDCAP_WAKE_FROM_D1_SUPPORTED    0x00000020
// #define PDCAP_WAKE_FROM_D2_SUPPORTED    0x00000040
// #define PDCAP_WAKE_FROM_D3_SUPPORTED    0x00000080
// #define PDCAP_WARM_EJECT_SUPPORTED      0x00000100
//
// The user can also send in a combination of these values
// to do queries that match either or both of the specified
// flags.  E.g. if the user used (PDCAP_D1_SUPPORTED | PDCAP_D3_SUPPORTED),
// then the query would return a device that supported *either*
// D1 or D3.  The user could specify QUERY_AND_OPERATION in
// QueryInterpretationFlags to get a device that supported
// D1 *and* D3.
//

//
// We also support querying on S states.  Although devices don't
// really understand system states, we can use the D-to-S state
// mappings to derive this.  For example, if we ask a device if
// he supports S2, we can derive that by looking at his D-to-S
// mappings.  Suppose the device has a D-to-S state mapping
// that looks like this:
// S0 -> D0
// S1 -> D3
// S2 -> D3
// S3 -> D3
// S4 -> D3
// S5 -> D3
//
// We see that S2 maps to D3 on this device.  Does this device
// actually support D3?  We can find that out.  If so, then we
// say that this device supports S2.
//
// We have to make up the S-state vectors because there's no
// existing values like the PDCAP_D* values.
//
#define PDCAP_S0_SUPPORTED              0x00010000
#define PDCAP_S1_SUPPORTED              0x00020000
#define PDCAP_S2_SUPPORTED              0x00040000
#define PDCAP_S3_SUPPORTED              0x00080000
#define PDCAP_WAKE_FROM_S0_SUPPORTED    0x00100000
#define PDCAP_WAKE_FROM_S1_SUPPORTED    0x00200000
#define PDCAP_WAKE_FROM_S2_SUPPORTED    0x00400000
#define PDCAP_WAKE_FROM_S3_SUPPORTED    0x00800000
#define PDCAP_S4_SUPPORTED              0x01000000
#define PDCAP_S5_SUPPORTED              0x02000000


//
// Function prototypes
//
extern
BOOLEAN    
DevicePowerEnumDevices(
    __in ULONG  QueryIndex,
    __in ULONG  QueryInterpretationFlags,
    __in ULONG  QueryFlags,
    __out_bcount_opt(*pBufferSize) PBYTE  pReturnBuffer,
    __inout PULONG pBufferSize
    );

extern
DWORD
DevicePowerSetDeviceState(
    __in LPCWSTR DeviceDescription,
    __in ULONG SetFlags,
    __in_opt PVOID SetData
    );

BOOLEAN
DevicePowerOpen(
    __in_opt ULONG DebugMask
    );

BOOLEAN
DevicePowerClose(
    VOID
    );
#endif  // (NTDDI_VERSION >= NTDDI_WS03) Windows server03 version



#ifdef __cplusplus
}
#endif


