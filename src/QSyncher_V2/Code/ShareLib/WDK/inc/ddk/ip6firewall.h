/*++

Copyright (c) 2004-2005  Microsoft Corporation

Module Name:

    ip6firewall.h

Abstract:

    This module contains definitions for the IPv6 firewall hook.

Environment:

    Kernel mode only.

--*/

#ifndef _IP6FIREWALL_
#define _IP6FIREWALL_

#pragma once

#include <ipinfo.h>

#ifdef __cplusplus
extern "C" {
#endif

//
// Types used by the firewall hook.
//

typedef enum {
    DirectionTransmit,
    DirectionReceive,
    DirectionMax
} IPv6Direction;

typedef enum {
    ActionAccept,
    ActionDrop,
    ActionMax
} IPv6Action;

typedef 
IPv6Action
(*IPv6FirewallHookProc) (
    const IPv6Addr *SourceAddress,
    const IPv6Addr *DestinationAddress,
    uint PayloadLength,
    uchar HeaderType,
    const uchar *HeaderData,
    const void *PacketContext,
    uint DataLength,
    uint InterfaceIndex,
    IPv6Direction Direction,
    BOOLEAN IsLoopBack
    );

typedef void
(*IPv6FirewallDeregistrationCompleteProc)(
    );

//
// Exported function declarations.
//

extern NTSTATUS
IPv6EnableFirewallHook(
    IPv6FirewallHookProc FirewallHookFunction
    );

extern void
IPv6DisableFirewallHook(
    IPv6FirewallDeregistrationCompleteProc CompletionRoutine
    );

extern const uchar *
IPv6ObtainPacketData(
    const void *PacketContext,
    uint DataLength,
    uint Alignment
    );

extern IP_STATUS
IPv6GetBestRouteInfo(
    const IPv6Addr *Addr,
    ulong ScopeId,
    ulong Index,
    IP6RouteEntry *Ire
    );
    
#ifdef __cplusplus
}
#endif

#endif // _IP6FIREWALL_

