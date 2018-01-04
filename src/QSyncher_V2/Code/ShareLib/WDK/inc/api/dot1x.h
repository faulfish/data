#ifndef __DOT1X_H_
#define __DOT1X_H_

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <l2cmn.h>

#if __midl
#define V1_ENUM [v1_enum]
#else
#define V1_ENUM
#endif

/*
    The identity that is being used by the 1X module. This is a function of the
    onex auth mode and system triggers (e.g. user logon/logoff)
    */  
typedef V1_ENUM enum _ONEX_AUTH_IDENTITY 
{
    OneXAuthIdentityNone,
    OneXAuthIdentityMachine,
    OneXAuthIdentityUser,
    OneXAuthIdentityExplicitUser,
    OneXAuthIdentityGuest,
    OneXAuthIdentityInvalid
} ONEX_AUTH_IDENTITY, *PONEX_AUTH_IDENTITY;

/*
    The 1X authentication status. Each of the auth status corresponds to one or
    more of the internal 1X states. 
    */
typedef V1_ENUM enum _ONEX_AUTH_STATUS
{
    OneXAuthNotStarted,
    OneXAuthInProgress,
    OneXAuthNoAuthenticatorFound,
    OneXAuthSuccess,
    OneXAuthFailure,
    OneXAuthInvalid
} ONEX_AUTH_STATUS, *PONEX_AUTH_STATUS;

/*
    1X specific reason codes
    */
typedef V1_ENUM enum _ONEX_REASON_CODE
{
    ONEX_REASON_CODE_SUCCESS = 0,
    ONEX_REASON_START = L2_REASON_CODE_ONEX_BASE,
    ONEX_UNABLE_TO_IDENTIFY_USER,
    ONEX_IDENTITY_NOT_FOUND,
    ONEX_UI_DISABLED,
    ONEX_UI_FAILURE,
    ONEX_EAP_FAILURE_RECEIVED,
    ONEX_AUTHENTICATOR_NO_LONGER_PRESENT,
    ONEX_NO_RESPONSE_TO_IDENTITY,
    ONEX_PROFILE_VERSION_NOT_SUPPORTED,
    ONEX_PROFILE_INVALID_LENGTH,
    ONEX_PROFILE_DISALLOWED_EAP_TYPE,
    ONEX_PROFILE_INVALID_EAP_TYPE_OR_FLAG,
    ONEX_PROFILE_INVALID_ONEX_FLAGS,
    ONEX_PROFILE_INVALID_TIMER_VALUE,
    ONEX_PROFILE_INVALID_SUPPLICANT_MODE,
    ONEX_PROFILE_INVALID_AUTH_MODE,
    ONEX_PROFILE_INVALID_EAP_CONNECTION_PROPERTIES,
} ONEX_REASON_CODE, *PONEX_REASON_CODE;

#ifdef __cplusplus
}
#endif

#endif 

