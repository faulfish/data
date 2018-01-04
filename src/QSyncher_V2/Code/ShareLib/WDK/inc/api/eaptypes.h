////////////////////////////////////////////////////////////
//
// Copyright (c) Microsoft Corporation.
//
// SYNOPSIS
//
//    IDL source for interaction with EAPHost supplicants.
//
////////////////////////////////////////////////////////////

#ifndef EAPTYPES_H
#define EAPTYPES_H
#pragma once

#include <guiddef.h>  // Used by EAP_ERROR structure.


// Properties that EAP Methods support
#define eapPropCipherSuiteNegotiation 0x00000001
#define eapPropMutualAuth             0x00000002
#define eapPropIntegrity              0x00000004
#define eapPropReplayProtection       0x00000008
#define eapPropConfidentiality        0x00000010
#define eapPropKeyDerivation          0x00000020
#define eapPropKeyStrength64          0x00000040
#define eapPropKeyStrength128         0x00000080
#define eapPropKeyStrength256         0x00000100
#define eapPropKeyStrength512         0x00000200
#define eapPropKeyStrength1024        0x00000400
#define eapPropDictionaryAttackResistance 0x00000800
#define eapPropFastReconnect          0x00001000
#define eapPropCryptoBinding          0x00002000
#define eapPropSessionIndependence    0x00004000
#define eapPropFragmentation          0x00008000
#define eapPropChannelBinding         0x00010000
#define eapPropNap                    0x00020000
#define eapPropStandalone             0x00040000
#define eapPropMppeEncryption         0x00080000
#define eapPropTunnelMethod           0x00100000
#define eapPropSupportsConfig         0x00200000
#define eapPropCertifiedMethod        0x00400000
#define eapPropReserved               0x80000000

// Registry NameValue that stores Properties supported by EAP Method
#define EAP_VALUENAME_PROPERTIES L"Properties"

typedef DWORD EAP_SESSIONID;


typedef struct _EAP_TYPE
{
   BYTE type;
   DWORD dwVendorId;
   DWORD dwVendorType;   
} EAP_TYPE;
   

typedef struct _EAP_METHOD_TYPE
{
   EAP_TYPE eapType;
   DWORD dwAuthorId;
} EAP_METHOD_TYPE;


typedef struct _EAP_METHOD_INFO 
{
   EAP_METHOD_TYPE eaptype;
   LPWSTR pwszAuthorName;
   LPWSTR pwszFriendlyName;
   DWORD eapProperties;
   struct _EAP_METHOD_INFO* pInnerMethodInfo;
} EAP_METHOD_INFO;

typedef struct _EAP_METHOD_INFO_ARRAY
{
   DWORD dwNumberOfMethods;
   EAP_METHOD_INFO* pEapMethods;
} EAP_METHOD_INFO_ARRAY;

typedef struct _EAP_ERROR
{
   DWORD dwWinError;
   EAP_METHOD_TYPE type;
   DWORD dwReasonCode;

   GUID   rootCauseGuid;
   GUID   repairGuid;
   GUID   helpLinkGuid;

   LPWSTR pRootCauseString;
   LPWSTR pRepairString;
} EAP_ERROR;


//TODO: Fix the GUID declarations, and delete these.  (This was a trivial workaround, to let me continue working.)
EXTERN_C const CLSID GUID_EapHost_Default;

EXTERN_C const CLSID GUID_EapHost_Cause_MethodDLLNotFound;
EXTERN_C const CLSID GUID_EapHost_Cause_EapNegotiationFailed;    //*!*
EXTERN_C const CLSID GUID_EapHost_Cause_ThirdPartyMethod_Host_Reset;
EXTERN_C const CLSID GUID_EapHost_Cause_XmlMalformed;
EXTERN_C const CLSID GUID_EapHost_Cause_MethodDoesNotSupportOperation;
EXTERN_C const CLSID GUID_EapHost_Cause_EapQecInaccessible;      //*!*

EXTERN_C const CLSID GUID_EapHost_Cause_Generic_AuthFailure;
EXTERN_C const CLSID GUID_EapHost_Cause_IdentityUnknown;

EXTERN_C const CLSID GUID_EapHost_Cause_CertStoreInaccessible;

EXTERN_C const CLSID GUID_EapHost_Cause_User_CertExpired;
EXTERN_C const CLSID GUID_EapHost_Cause_User_CertInvalid;
EXTERN_C const CLSID GUID_EapHost_Cause_User_CertNotFound;
EXTERN_C const CLSID GUID_EapHost_Cause_User_CertOtherError;
EXTERN_C const CLSID GUID_EapHost_Cause_User_CertRejected;
EXTERN_C const CLSID GUID_EapHost_Cause_User_CertRevoked;
EXTERN_C const CLSID GUID_EapHost_Cause_User_Account_OtherProblem;
EXTERN_C const CLSID GUID_EapHost_Cause_User_CredsRejected;

EXTERN_C const CLSID GUID_EapHost_Cause_Server_CertExpired;
EXTERN_C const CLSID GUID_EapHost_Cause_Server_CertInvalid;
EXTERN_C const CLSID GUID_EapHost_Cause_Server_CertNotFound;
EXTERN_C const CLSID GUID_EapHost_Cause_Server_CertOtherError;
EXTERN_C const CLSID GUID_EapHost_Cause_Server_CertRevoked;

EXTERN_C const CLSID GUID_EapHost_Cause_User_Root_CertExpired;
EXTERN_C const CLSID GUID_EapHost_Cause_User_Root_CertInvalid;
EXTERN_C const CLSID GUID_EapHost_Cause_User_Root_CertNotFound;

EXTERN_C const CLSID GUID_EapHost_Cause_Server_Root_CertNotFound;
EXTERN_C const CLSID GUID_EapHost_Cause_Server_Root_CertNameRequired;

EXTERN_C const CLSID GUID_EapHost_Repair_ContactSysadmin;
EXTERN_C const CLSID GUID_EapHost_Repair_Retry_Authentication;

EXTERN_C const CLSID GUID_EapHost_Repair_User_AuthFailure;
EXTERN_C const CLSID GUID_EapHost_Repair_User_GetNewCert;
EXTERN_C const CLSID GUID_EapHost_Repair_User_SelectValidCert;

EXTERN_C const CLSID GUID_EapHost_Repair_Server_ClientSelectServerCert;

EXTERN_C const CLSID GUID_EapHost_Help_EapConfigureTypes;
EXTERN_C const CLSID GUID_EapHost_Help_FailedAuth;
EXTERN_C const CLSID GUID_EapHost_Help_ObtainingCerts;
EXTERN_C const CLSID GUID_EapHost_Help_SelectingCerts;
EXTERN_C const CLSID GUID_EapHost_Help_SetupEapServer;
EXTERN_C const CLSID GUID_EapHost_Help_Troubleshooting;

//--------------------------


typedef
#ifdef __midl
   [v1_enum]
#endif
enum _EAP_ATTRIBUTE_TYPE
{
    eatMinimum = 0,                // Undefined
    eatUserName,                   // Value field is a Pointer
    eatUserPassword,               // Value field is a Pointer
    eatMD5CHAPPassword,            // Value field is a Pointer
    eatNASIPAddress,               // Value field is a 32 bit integral value
    eatNASPort,                    // Value field is a 32 bit integral value
    eatServiceType,                // Value field is a 32 bit integral value
    eatFramedProtocol,             // Value field is a 32 bit integral value
    eatFramedIPAddress,            // Value field is a 32 bit integral value
    eatFramedIPNetmask,            // Value field is a 32 bit integral value
    eatFramedRouting = 10,         // Value field is a 32 bit integral value
    eatFilterId,                   // Value field is a Pointer
    eatFramedMTU,                  // Value field is a 32 bit integral value
    eatFramedCompression,          // Value field is a 32 bit integral value
    eatLoginIPHost,                // Value field is a 32 bit integral value
    eatLoginService,               // Value field is a 32 bit integral value
    eatLoginTCPPort,               // Value field is a 32 bit integral value
    eatUnassigned17,               // Undefined
    eatReplyMessage,               // Value field is a Pointer
    eatCallbackNumber,             // Value field is a Pointer
    eatCallbackId =20,             // Value field is a Pointer
    eatUnassigned21,               // Undefined
    eatFramedRoute,                // Value field is a Pointer
    eatFramedIPXNetwork,           // Value field is a 32 bit integral value
    eatState,                      // Value field is a Pointer
    eatClass,                      // Value field is a Pointer
    eatVendorSpecific,             // Value field is a Pointer
    eatSessionTimeout,             // Value field is a 32 bit integral value
    eatIdleTimeout,                // Value field is a 32 bit integral value
    eatTerminationAction,          // Value field is a 32 bit integral value
    eatCalledStationId = 30,       // Value field is a Pointer
    eatCallingStationId,           // Value field is a Pointer
    eatNASIdentifier,              // Value field is a Pointer
    eatProxyState,                 // Value field is a Pointer
    eatLoginLATService,            // Value field is a Pointer
    eatLoginLATNode,               // Value field is a Pointer
    eatLoginLATGroup,              // Value field is a Pointer
    eatFramedAppleTalkLink,        // Value field is a 32 bit integral value
    eatFramedAppleTalkNetwork,     // Value field is a 32 bit integral value
    eatFramedAppleTalkZone,        // Value field is a Pointer
    eatAcctStatusType = 40,        // Value field is a 32 bit integral value
    eatAcctDelayTime,              // Value field is a 32 bit integral value
    eatAcctInputOctets,            // Value field is a 32 bit integral value
    eatAcctOutputOctets,           // Value field is a 32 bit integral value
    eatAcctSessionId,              // Value field is a Pointer
    eatAcctAuthentic,              // Value field is a 32 bit integral value
    eatAcctSessionTime,            // Value field is a 32 bit integral value
    eatAcctInputPackets,           // Value field is a 32 bit integral value
    eatAcctOutputPackets,          // Value field is a 32 bit integral value
    eatAcctTerminateCause,         // Value field is a 32 bit integral value
    eatAcctMultiSessionId = 50,    // Value field is a Pointer
    eatAcctLinkCount,              // Value field is a 32 bit integral value
    eatAcctEventTimeStamp = 55,    // Value field is a 32 bit integral value
    eatMD5CHAPChallenge = 60,      // Value field is a Pointer
    eatNASPortType,                // Value field is a 32 bit integral value
    eatPortLimit,                  // Value field is a 32 bit integral value
    eatLoginLATPort,               // Value field is a Pointer
    eatTunnelType,                 // Value field is a 32 bit integral value
    eatTunnelMediumType,           // Value field is a 32 bit integral value
    eatTunnelClientEndpoint,       // Value field is a Pointer
    eatTunnelServerEndpoint,       // Value field is a Pointer
    eatARAPPassword = 70,          // Value field is a Pointer
    eatARAPFeatures,               // Value field is a Pointer
    eatARAPZoneAccess,             // Value field is a 32 bit integral value
    eatARAPSecurity,               // Value field is a 32 bit integral value
    eatARAPSecurityData,           // Value field is a Pointer
    eatPasswordRetry,              // Value field is a 32 bit integral value
    eatPrompt,                     // Value field is a 32 bit integral value
    eatConnectInfo,                // Value field is a Pointer
    eatConfigurationToken,         // Value field is a Pointer
    eatEAPMessage,                 // Value field is a Pointer
    eatSignature = 80,             // Value field is a Pointer
    eatARAPChallengeResponse = 84, // Value field is a Pointer
    eatAcctInterimInterval = 85,   // Value field is a 32 bit integral value
    eatNASIPv6Address = 95,  // Value field is a Pointer
    eatFramedInterfaceId, // Value field is a Pointer
    eatFramedIPv6Prefix, // Value field is a Pointer
    eatLoginIPv6Host, // Value field is a Pointer
    eatFramedIPv6Route, // Value field is a Pointer
    eatFramedIPv6Pool, // Value field is a Pointer
    eatARAPGuestLogon = 8096,      // Value field is a 32 bit integral value
    eatCertificateOID,             // Value field is a Pointer
    eatEAPConfiguration,           // Value field is a Pointer
    eatPEAPEmbeddedEAPTypeId,      // Value field is a 32 bit integral value
    eatPEAPFastRoamedSession,      // Value field is a 32 bit integral value
    eatEAPTLV = 8102,              // Value field is a Pointer
    eatCredentialsChanged,        // Value field is a Integer with boolean 
                                   // semantics
    eatInnerEapMethodType,         // Value field is a pointer
                                   // EapMethodType of the inner method in 
                                   // case of Tunnelled method                             
    eatClearTextPassword = 8107,   // Value field is a pointer
                                   // Clear text password that can be used
                                   // by certain password based eap methods
                                   // to authenticate a user.
    eatQuarantineSoH     = 8150, // Value field is a pointer
    eatPeerId                 = 9000,  // Value field is a pointer
    eatServerId,                          // Value field is a pointer
    eatMethodId,                          // Value field is a pointer
    eatEMSK,                               // Value field is a pointer
    eatReserved = 0xFFFFFFFF       // Undefined
} EAP_ATTRIBUTE_TYPE, EapAttributeType;


typedef struct _EAP_ATTRIBUTE 
{  
        EAP_ATTRIBUTE_TYPE eaType;  
        DWORD dwLength;
#ifdef __midl   
        [size_is(dwLength)] BYTE* pValue;
#else
    BYTE *pValue;
#endif
} EAP_ATTRIBUTE, EapAttribute;


typedef struct _EAP_ATTRIBUTES 
{  
        DWORD dwNumberOfAttributes;
#ifdef __midl   
   //FIXME: Should this be size_is or length_is
        [size_is(dwNumberOfAttributes)] EAP_ATTRIBUTE* pAttribs;  
#else
    EAP_ATTRIBUTE *pAttribs;
#endif
} EAP_ATTRIBUTES, EapAttributes;


// No UI should be displayed
#define EAP_FLAG_Reserved1              0x00000001
#define EAP_FLAG_NON_INTERACTIVE        0x00000002  

// The user data was obtained from Winlogon
#define EAP_FLAG_LOGON                  0x00000004 

// Show the Credentails UI before authenticating even if cached creds are present
#define EAP_FLAG_PREVIEW                0x00000008

#define EAP_FLAG_Reserved2              0x00000010

// Authentication is for machine, i.e. not for user. Not setting this flag means
// it is user authentication.
#define EAP_FLAG_MACHINE_AUTH           0x00000020  

// Request to provide guest access
#define EAP_FLAG_GUEST_ACCESS           0x00000040  

#define EAP_FLAG_Reserved3              0x00000080

#define EAP_FLAG_Reserved4              0x00000100

// Indicates this is the first call after m/c resumed from hibernation.
#define EAP_FLAG_RESUME_FROM_HIBERNATE  0x00000200

#define EAP_FLAG_Reserved5              0x00000400

#define EAP_FLAG_Reserved6              0x00000800

// Should have this or make it reserved?
#define EAP_FLAG_FULL_AUTH              0x00001000

///   New EapHost Flags                       

// Prefer Credentails passed in BeginSession instead of other form of
// credentials, even if config says use certain way of cred retrieval. If this fails, fallback to 
// Method specific credentail retrieval
#define EAP_FLAG_PREFER_ALT_CREDENTIALS     0x00002000

#define EAP_FLAG_Reserved7                  0x00004000

// To inform the cause of reauth is a NAP callback
#define EAP_PEER_FLAG_HEALTH_STATE_CHANGE   0x00008000 

// Continue authentication with information available. If can not proceed, fail
// the authentication
#define EAP_FLAG_SUPRESS_UI                 0x00010000

// Pre logon scenario like PLAP
#define EAP_FLAG_PRE_LOGON                  0x00020000

// User Authentication. For legacy Methods not setting EAP_FLAG_MACHINE_AUTH
// also means user authentication.
#define EAP_FLAG_USER_AUTH                  0x00040000

// Config can be viewed but not updated
#define EAP_FLAG_CONFG_READONLY             0x00080000

#define EAP_FLAG_Reserved8                  0x00100000

// the various states of the input fields
#define EAP_CONFIG_INPUT_FIELD_PROPS_DEFAULT             0
#define EAP_CONFIG_INPUT_FIELD_PROPS_NON_DISPLAYABLE     1

// the types of fileds that eap can request to show
typedef enum _EAP_CONFIG_INPUT_FIELD_TYPE {
    EapConfigInputUsername,
    EapConfigInputPassword,
    EapConfigInputNetworkUsername,
    EapConfigInputNetworkPassword,
    EapConfigInputPin, 
    EapConfigInputPSK, 
    EapConfigInputEdit
} EAP_CONFIG_INPUT_FIELD_TYPE, *PEAP_CONFIG_INPUT_FIELD_TYPE;

#define EAP_CREDENTIAL_VERSION                     1
#define MAX_EAP_CONFIG_INPUT_FIELD_LENGTH          256
#define MAX_EAP_CONFIG_INPUT_FIELD_VALUE_LENGTH    1024

typedef struct _EAP_CONFIG_INPUT_FIELD_DATA {
    DWORD dwSize;
    EAP_CONFIG_INPUT_FIELD_TYPE Type;
    DWORD dwFlagProps;
    LPWSTR pwszLabel;
    LPWSTR pwszData;
    DWORD dwMinDataLength;
    DWORD dwMaxDataLength;
} EAP_CONFIG_INPUT_FIELD_DATA, *PEAP_CONFIG_INPUT_FIELD_DATA;

typedef struct _EAP_CONFIG_INPUT_FIELD_ARRAY {
    DWORD dwVersion; // used for versioning
    DWORD dwNumberOfFields;
#ifdef __midl   
   //FIXME: Should this be size_is or length_is
    [size_is(dwNumberOfFields)] EAP_CONFIG_INPUT_FIELD_DATA* pFields;
#else
    EAP_CONFIG_INPUT_FIELD_DATA* pFields;
#endif
} EAP_CONFIG_INPUT_FIELD_ARRAY, *PEAP_CONFIG_INPUT_FIELD_ARRAY;


#endif //EAPTYPES_H

