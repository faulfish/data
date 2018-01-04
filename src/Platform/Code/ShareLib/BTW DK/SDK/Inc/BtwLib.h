/////////////////////////////////////////////////////////////////////////////
//
//  Name        BtwLib.h
//  $Header:
//
//  Function    this file facilitates linking the SDK library and including
//  			other SDK headers
//
//  Date                 Modification
//  ----------------------------------
//  09/11/2003    HZ   Create
//
//  Copyright (c) 2000-2006, Broadcom Corporation, All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////

#pragma once
#if (_MSC_FULL_VER >= 140050727)
    // use this lib for VS2005 wchar definition 
    // Note: using _MSC_VER >= 1400 is not sufficient, MSFT VC++ 6.0 under AMD64 
    // configuration February 2003 Platform SDK, the compiler (incorrectly) identifies
    // itself as 1400, so this is the accurate method.
    //
    // See Programmer's Guide - must redistribute btwapi.dll to use BtWdSdkLib.lib on
    // older BTW stack targets
    #ifdef _WIN64
        #pragma comment(lib, "..\\..\\sdk\\release\\x64\\BtWdSdkLib.lib")
    #else
        #pragma comment(lib, "..\\..\\sdk\\release\\BtWdSdkLib.lib")
    #endif
#else
    // use this for previous compilers
    #ifdef _WIN64
        #pragma comment(lib, "..\\..\\sdk\\release\\amd64\\WidcommSdklib.lib")
    #else
        #pragma comment(lib, "..\\..\\sdk\\release\\WidcommSdklib.lib")
    #endif
#endif

#include "BtIfDefinitions.h"
#include "BtIfClasses.h"
