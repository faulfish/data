/*++

Copyright(c) Microsoft Corporation. All rights reserved.

Module Name:

    ws2sdp.h

Abstract:

    This module contains definitions for Sockets Direct Protocol (SDP) support.

Revision History:

--*/

#ifndef _WS2SDP_H_
#define _WS2SDP_H_

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

//
// Connect data structure
//
// Bufs - The number of receive buffers posted, this maps to BSDH->Bufs field.
// 
// MaxAdverts - The max number of RDMA advertisements expected from the remote 
// peer, this maps to BSDH->MaxAdverts field.
//
// RcvSize1 - This maps to "Desired Remote Receive Size" of the Hello message, 
// or "Actual Receive Size" of the HelloAck message.
//
// RcvSize2 - Local receive size. This field is only significant for CallerData. 
// SDP-SAN provider should use this value to fill in the LocalRcvSz field 
// of the SDP Hello message. For CalleeData, this field should be ignored.
//
// IRD - the iWARP IRD value. This field maps to the "IRD" field of the Hello 
// message and the HelloAck message. For CallerData, SDP-SAN provider should 
// use this value to fill in the IRD field of the Hello message and set it as
// local RNICs IRD value. For CalleeData, SDP-SAN provider should use the 
// IRD field of the incoming SDP HelloAck message to fill in this field.
//
// ORD - the iWARP ORD value. This field maps to the "ORD" field of the Hello 
// message and the HelloAck message. For CallerData, SDP-SAN provider should use 
// this value to fill in the ORD field of the Hello message and set it as the 
// local RNIC's ORD value. For CalleeData, SDP-SAN provider should use the ORD 
// field of the incoming SDP HelloAck message to fill in this field.
//

typedef struct _WSASDPCONNECTDATA {
    USHORT Bufs;
    UCHAR  MaxAdverts;
    DWORD  RcvSize1;
    DWORD  RcvSize2;
    DWORD  IRD;
    DWORD  ORD;        
} WSASDPCONNECTDATA, FAR * LPWSASDPCONNECTDATA;


//
// Following are flags that can be specified in a WSPSend() call.
//

//
// Flag to set Solicited Event bit
//

#define MSG_SOLICITED   0x10000

//
// Flag to tell provider that this send contains remote invalidate information.
//

#define MSG_SDP_REMOTE_INVALIDATE 0x20000

#ifdef __cplusplus
}
#endif

#endif // _WS2SDP_H_

