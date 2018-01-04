/*++

Copyright (c) Microsoft Corporation. All rights reserved.

Module Name:

    iphlpapi.h

Abstract:
    Header file for functions to interact with the IP Stack for MIB-II and
    related functionality

--*/

#ifndef __IPHLPAPI_H__
#define __IPHLPAPI_H__

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// IPRTRMIB.H has the definitions of the structures used to set and get     //
// information                                                              //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#include <iprtrmib.h>
#include <ipexport.h>
#include <iptypes.h>
#include <tcpestats.h>

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// The GetXXXTable APIs take a buffer and a size of buffer.  If the buffer  //
// is not large enough, the APIs return ERROR_INSUFFICIENT_BUFFER  and      //
// *pdwSize is the required buffer size                                     //
// The bOrder is a BOOLEAN, which if TRUE sorts the table according to      //
// MIB-II (RFC XXXX)                                                        //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Retrieves the number of interfaces in the system. These include LAN and  //
// WAN interfaces                                                           //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


DWORD
WINAPI
GetNumberOfInterfaces(
    OUT PDWORD  pdwNumIf
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Gets the MIB-II ifEntry                                                  //
// The dwIndex field of the MIB_IFROW should be set to the index of the     //
// interface being queried                                                  //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
GetIfEntry(
    IN OUT PMIB_IFROW   pIfRow
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Gets the MIB-II IfTable                                                  //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
GetIfTable(
    OUT    PMIB_IFTABLE pIfTable,
    IN OUT PULONG       pdwSize,
    IN     BOOL         bOrder
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Gets the Interface to IP Address mapping                                 //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
GetIpAddrTable(
    OUT    PMIB_IPADDRTABLE pIpAddrTable,
    IN OUT PULONG           pdwSize,
    IN     BOOL             bOrder
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Gets the current IP Address to Physical Address (ARP) mapping            //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

ULONG
WINAPI
GetIpNetTable(
    OUT PMIB_IPNETTABLE IpNetTable,
    IN OUT PULONG SizePointer,
    IN BOOL Order
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Gets the IP Routing Table  (RFX XXXX)                                    //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
GetIpForwardTable(
    OUT    PMIB_IPFORWARDTABLE pIpForwardTable,
    IN OUT PULONG              pdwSize,
    IN     BOOL                bOrder
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Gets TCP Connection/UDP Listener Table                                   //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

ULONG
WINAPI
GetTcpTable(
    OUT PMIB_TCPTABLE TcpTable,
    IN OUT PULONG SizePointer,
    IN BOOL Order
    );

DWORD
WINAPI
GetExtendedTcpTable(
    OUT    PVOID           pTcpTable,
    IN OUT PDWORD          pdwSize,
    IN     BOOL            bOrder,
    IN     ULONG           ulAf,
    IN     TCP_TABLE_CLASS TableClass,
    IN     ULONG           Reserved
    );    

DWORD
WINAPI
GetOwnerModuleFromTcpEntry(
    IN     PMIB_TCPROW_OWNER_MODULE      pTcpEntry,
    IN     TCPIP_OWNER_MODULE_INFO_CLASS Class,
    OUT    PVOID                         pBuffer,
    IN OUT PDWORD                        pdwSize
    );

ULONG
WINAPI
GetUdpTable(
    OUT PMIB_UDPTABLE UdpTable,
    IN OUT PULONG SizePointer,
    IN BOOL Order
    );

DWORD
WINAPI
GetExtendedUdpTable(
    OUT    PVOID           pUdpTable,
    IN OUT PDWORD          pdwSize,
    IN     BOOL            bOrder,
    IN     ULONG           ulAf,
    IN     UDP_TABLE_CLASS TableClass,
    IN     ULONG           Reserved
    );

DWORD
WINAPI
GetOwnerModuleFromUdpEntry(
    IN     PMIB_UDPROW_OWNER_MODULE      pUdpEntry,
    IN     TCPIP_OWNER_MODULE_INFO_CLASS Class,
    OUT    PVOID                         pBuffer,
    IN OUT PDWORD                        pdwSize
    );
    
#if (NTDDI_VERSION >= NTDDI_LONGHORN)
ULONG
WINAPI
GetTcpTable2(
    OUT PMIB_TCPTABLE2 TcpTable,
    IN OUT PULONG SizePointer,
    IN BOOL Order
    );
#endif // (NTDDI_VERSION >= NTDDI_LONGHORN)  

#if (NTDDI_VERSION < NTDDI_LONGHORN)
//
// Deprecated APIs, Added for documentation.
//

DWORD
AllocateAndGetTcpExTableFromStack(
    OUT PVOID         *ppTcpTable,
    IN  BOOL          bOrder,
    IN  HANDLE        hHeap,
    IN  DWORD         dwFlags,
    IN  DWORD         dwFamily
    );

DWORD
AllocateAndGetUdpExTableFromStack(
    OUT PVOID         *ppUdpTable,
    IN  BOOL          bOrder,
    IN  HANDLE        hHeap,
    IN  DWORD         dwFlags,
    IN  DWORD         dwFamily
    );

#endif // (NTDDI_VERSION < NTDDI_LONGHORN) 

#ifdef _WS2IPDEF_
//
// The following definitions require Winsock2.
//

#if (NTDDI_VERSION >= NTDDI_LONGHORN)
ULONG
WINAPI
GetTcp6Table(
    OUT PMIB_TCP6TABLE TcpTable,
    IN OUT PULONG SizePointer,
    IN BOOL Order
    );

ULONG
WINAPI
GetTcp6Table2(
    OUT PMIB_TCP6TABLE2 TcpTable,
    IN OUT PULONG SizePointer,
    IN BOOL Order
    );

#endif // (NTDDI_VERSION >= NTDDI_LONGHORN)  

#ifdef WINAPI

ULONG
WINAPI
GetPerTcpConnectionEStats(
    PMIB_TCPROW Row,
    TCP_ESTATS_TYPE EstatsType,
    __out_bcount_opt(RwSize) PUCHAR Rw,
    ULONG RwVersion,
    ULONG RwSize,
    __out_bcount_opt(RosSize) PUCHAR Ros,
    ULONG RosVersion,
    ULONG RosSize,
    __out_bcount_opt(RodSize) PUCHAR Rod,
    ULONG RodVersion,
    ULONG RodSize
    );

ULONG
WINAPI
SetPerTcpConnectionEStats(
    PMIB_TCPROW Row,
    TCP_ESTATS_TYPE EstatsType,
    __in_bcount(RwSize) PUCHAR Rw,
    ULONG RwVersion,
    ULONG RwSize,
    ULONG Offset
    );

#ifdef _WS2IPDEF_

ULONG
WINAPI
GetPerTcp6ConnectionEStats(
    PMIB_TCP6ROW Row,
    TCP_ESTATS_TYPE EstatsType,
    __out_bcount_opt(RwSize) PUCHAR Rw,
    ULONG RwVersion,
    ULONG RwSize,
    __out_bcount_opt(RosSize) PUCHAR Ros,
    ULONG RosVersion,
    ULONG RosSize,
    __out_bcount_opt(RodSize) PUCHAR Rod,
    ULONG RodVersion,
    ULONG RodSize
    );

ULONG
WINAPI
SetPerTcp6ConnectionEStats(
    PMIB_TCP6ROW Row,
    TCP_ESTATS_TYPE EstatsType,
    __in_bcount(RwSize) PUCHAR Rw,
    ULONG RwVersion,
    ULONG RwSize,
    ULONG Offset
    );	

#endif // _WS2IPDEF_

#endif // WINAPI    

DWORD
WINAPI
GetOwnerModuleFromTcp6Entry(
    IN     PMIB_TCP6ROW_OWNER_MODULE     pTcpEntry,
    IN     TCPIP_OWNER_MODULE_INFO_CLASS Class,
    OUT    PVOID                         pBuffer,
    IN OUT PDWORD                        pdwSize
    );    

ULONG
WINAPI
GetUdp6Table(
    OUT PMIB_UDP6TABLE Udp6Table,
    IN OUT PULONG SizePointer,
    IN BOOL Order
    );

DWORD
WINAPI
GetOwnerModuleFromUdp6Entry(
    IN     PMIB_UDP6ROW_OWNER_MODULE     pUdpEntry,
    IN     TCPIP_OWNER_MODULE_INFO_CLASS Class,
    OUT    PVOID                         pBuffer,
    IN OUT PDWORD                        pdwSize
    );    

#endif // _WS2IPDEF_

DWORD
GetOwnerModuleFromPidAndInfo(
    IN ULONG                         ulPid,
    IN ULONGLONG                     *pInfo,
    IN TCPIP_OWNER_MODULE_INFO_CLASS Class,
    OUT PVOID                        pBuffer,
    IN OUT PDWORD                    pdwSize
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Gets IP/ICMP/TCP/UDP Statistics                                          //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#if (NTDDI_VERSION >= NTDDI_WIN2K)
ULONG
WINAPI
GetIpStatistics(
    OUT PMIB_IPSTATS Statistics
    );


ULONG
WINAPI
GetIpStatisticsEx(
    OUT PMIB_IPSTATS Statistics,
    IN ULONG Family
    );

ULONG
WINAPI
SetIpStatisticsEx(
    IN PMIB_IPSTATS Statistics,
    IN ULONG Family
    );

#endif
ULONG
WINAPI
GetIcmpStatistics(
    OUT PMIB_ICMP Statistics
    );

#if (NTDDI_VERSION >= NTDDI_XPSP1)
ULONG
WINAPI
GetIcmpStatisticsEx(
    OUT PMIB_ICMP_EX Statistics,
    IN ULONG Family
    );
#endif

#if (NTDDI_VERSION >= NTDDI_WIN2K)
ULONG
WINAPI
GetTcpStatistics(
    OUT PMIB_TCPSTATS Statistics
    );

ULONG
WINAPI
GetTcpStatisticsEx(
    OUT PMIB_TCPSTATS Statistics,
    IN ULONG Family
    );
#endif

ULONG
WINAPI
GetUdpStatistics(
    OUT PMIB_UDPSTATS Stats
    );

ULONG
WINAPI
GetUdpStatisticsEx(
    OUT PMIB_UDPSTATS Statistics,
    IN ULONG Family
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Used to set the ifAdminStatus on an interface.  The only fields of the   //
// MIB_IFROW that are relevant are the dwIndex (index of the interface      //
// whose status needs to be set) and the dwAdminStatus which can be either  //
// MIB_IF_ADMIN_STATUS_UP or MIB_IF_ADMIN_STATUS_DOWN                       //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
SetIfEntry(
    IN PMIB_IFROW pIfRow
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Used to create, modify or delete a route.  In all cases the              //
// dwForwardIfIndex, dwForwardDest, dwForwardMask, dwForwardNextHop and     //
// dwForwardPolicy MUST BE SPECIFIED. Currently dwForwardPolicy is unused   //
// and MUST BE 0.                                                           //
// For a set, the complete MIB_IPFORWARDROW structure must be specified     //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
CreateIpForwardEntry(
    IN PMIB_IPFORWARDROW pRoute
    );

DWORD
WINAPI
SetIpForwardEntry(
    IN PMIB_IPFORWARDROW pRoute
    );

DWORD
WINAPI
DeleteIpForwardEntry(
    IN PMIB_IPFORWARDROW pRoute
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Used to set the ipForwarding to ON or OFF (currently only ON->OFF is     //
// allowed) and to set the defaultTTL.  If only one of the fields needs to  //
// be modified and the other needs to be the same as before the other field //
// needs to be set to MIB_USE_CURRENT_TTL or MIB_USE_CURRENT_FORWARDING as  //
// the case may be                                                          //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#if (NTDDI_VERSION >= NTDDI_WIN2K)
DWORD
WINAPI
SetIpStatistics(
    IN PMIB_IPSTATS pIpStats
    );
#endif

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Used to set the defaultTTL.                                              //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
SetIpTTL(
    UINT nTTL
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Used to create, modify or delete an ARP entry.  In all cases the dwIndex //
// dwAddr field MUST BE SPECIFIED.                                          //
// For a set, the complete MIB_IPNETROW structure must be specified         //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
CreateIpNetEntry(
    IN PMIB_IPNETROW    pArpEntry
    );

DWORD
WINAPI
SetIpNetEntry(
    IN PMIB_IPNETROW    pArpEntry
    );

DWORD
WINAPI
DeleteIpNetEntry(
    IN PMIB_IPNETROW    pArpEntry
    );

DWORD
WINAPI
FlushIpNetTable(
    IN DWORD   dwIfIndex
    );


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Used to create or delete a Proxy ARP entry. The dwIndex is the index of  //
// the interface on which to PARP for the dwAddress.  If the interface is   //
// of a type that doesnt support ARP, e.g. PPP, then the call will fail     //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
CreateProxyArpEntry(
    IN  DWORD   dwAddress,
    IN  DWORD   dwMask,
    IN  DWORD   dwIfIndex
    );

DWORD
WINAPI
DeleteProxyArpEntry(
    IN  DWORD   dwAddress,
    IN  DWORD   dwMask,
    IN  DWORD   dwIfIndex
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Used to set the state of a TCP Connection. The only state that it can be //
// set to is MIB_TCP_STATE_DELETE_TCB.  The complete MIB_TCPROW structure   //
// MUST BE SPECIFIED                                                        //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
SetTcpEntry(
    IN PMIB_TCPROW pTcpRow
    );


DWORD
WINAPI
GetInterfaceInfo(
    IN PIP_INTERFACE_INFO pIfTable,
    OUT PULONG            dwOutBufLen
    );

DWORD
WINAPI
GetUniDirectionalAdapterInfo(OUT PIP_UNIDIRECTIONAL_ADAPTER_ADDRESS pIPIfInfo,
                 OUT PULONG dwOutBufLen
                 );

#if (NTDDI_VERSION >= NTDDI_WIN2KSP1)
#ifndef NHPALLOCATEANDGETINTERFACEINFOFROMSTACK_DEFINED
#define NHPALLOCATEANDGETINTERFACEINFOFROMSTACK_DEFINED

DWORD
WINAPI
NhpAllocateAndGetInterfaceInfoFromStack(
    OUT IP_INTERFACE_NAME_INFO **ppTable,
    OUT PDWORD                 pdwCount,
    IN BOOL                    bOrder,
    IN HANDLE                  hHeap,
    IN DWORD                   dwFlags
    );

#endif
#endif // (NTDDI_VERSION >= NTDDI_WIN2KSP1)

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Gets the "best" outgoing interface for the specified destination address //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
GetBestInterface(
    IN  IPAddr  dwDestAddr,
    OUT PDWORD  pdwBestIfIndex
    );

#pragma warning(push)
#pragma warning(disable:4115)
DWORD
WINAPI
GetBestInterfaceEx(
    IN  struct sockaddr *pDestAddr,
    OUT PDWORD           pdwBestIfIndex
    );
#pragma warning(pop)

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Gets the best (longest matching prefix) route for the given destination  //
// If the source address is also specified (i.e. is not 0x00000000), and    //
// there are multiple "best" routes to the given destination, the returned  //
// route will be one that goes out over the interface which has an address  //
// that matches the source address                                          //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
GetBestRoute(
    IN  DWORD               dwDestAddr,
    IN  DWORD               dwSourceAddr, OPTIONAL
    OUT PMIB_IPFORWARDROW   pBestRoute
    );

DWORD
WINAPI
NotifyAddrChange(
    OUT PHANDLE      Handle,
    IN  LPOVERLAPPED overlapped
    );


DWORD
WINAPI
NotifyRouteChange(
    OUT PHANDLE      Handle,
    IN  LPOVERLAPPED overlapped
    );

BOOL
WINAPI
CancelIPChangeNotify(
    IN  LPOVERLAPPED notifyOverlapped
    );

DWORD
WINAPI
GetAdapterIndex(
    __in IN LPWSTR  AdapterName,
    __inout OUT PULONG IfIndex
    );

DWORD
WINAPI
AddIPAddress(
    IPAddr  Address,
    IPMask  IpMask,
    DWORD   IfIndex,
    PULONG  NTEContext,
    PULONG  NTEInstance
    );

DWORD
WINAPI
DeleteIPAddress(
    ULONG NTEContext
    );

#if (NTDDI_VERSION >= NTDDI_WIN2KSP1)
DWORD
WINAPI
GetNetworkParams(
    PFIXED_INFO pFixedInfo, PULONG pOutBufLen
    );
#endif

ULONG
WINAPI
GetAdaptersInfo(
    IN PIP_ADAPTER_INFO AdapterInfo, 
    IN OUT PULONG SizePointer
    );

PIP_ADAPTER_ORDER_MAP 
WINAPI
GetAdapterOrderMap(
    VOID
    );

#ifdef _WINSOCK2API_

//
// The following functions require Winsock2.
//

ULONG
WINAPI
GetAdaptersAddresses(
    IN ULONG Family,
    IN ULONG Flags,
    IN PVOID Reserved,
    __out_bcount_opt(*SizePointer) PIP_ADAPTER_ADDRESSES AdapterAddresses, 
    IN OUT PULONG SizePointer
    );

#endif

#if (NTDDI_VERSION >= NTDDI_WIN2KSP1)
DWORD
WINAPI
GetPerAdapterInfo(
    ULONG IfIndex, PIP_PER_ADAPTER_INFO pPerAdapterInfo, PULONG pOutBufLen
    );
#endif

DWORD
WINAPI
IpReleaseAddress(
    PIP_ADAPTER_INDEX_MAP  AdapterInfo
    );


DWORD
WINAPI
IpRenewAddress(
    PIP_ADAPTER_INDEX_MAP  AdapterInfo
    );

DWORD
WINAPI
SendARP(
    IPAddr DestIP,
    IPAddr SrcIP,
    PVOID pMacAddr,
    PULONG  PhyAddrLen
    );

BOOL
WINAPI
GetRTTAndHopCount(
    IPAddr DestIpAddress,
    PULONG HopCount,
    ULONG  MaxHops,
    PULONG RTT
    );

DWORD
WINAPI
GetFriendlyIfIndex(
    DWORD IfIndex
    );

DWORD
WINAPI
EnableRouter(
    HANDLE* pHandle,
    OVERLAPPED* pOverlapped
    );

DWORD
WINAPI
UnenableRouter(
    OVERLAPPED* pOverlapped,
    LPDWORD lpdwEnableCount OPTIONAL
    );
DWORD
WINAPI
DisableMediaSense(
    HANDLE *pHandle,
    OVERLAPPED *pOverLapped
    );

DWORD
WINAPI
RestoreMediaSense(
    OVERLAPPED* pOverlapped,
    LPDWORD lpdwEnableCount OPTIONAL
    );

#if (NTDDI_VERSION >= NTDDI_LONGHORN)

DWORD
WINAPI
GetIpErrorString(
    IN IP_STATUS ErrorCode,
    __out_ecount_opt(*Size + 1) PWSTR Buffer,
    __inout PDWORD Size
    );

#if (NTDDI_VERSION >= NTDDI_LONGHORN)
#ifdef _WS2DEF_
ULONG
WINAPI
ResolveNeighbor(
    IN SOCKADDR *NetworkAddress,
    OUT PVOID PhysicalAddress,
    IN OUT PULONG PhysicalAddressLength
    );
#endif
#endif  
//
// Port reservation API routines.
//

ULONG
WINAPI
CreatePersistentTcpPortReservation(
    IN USHORT StartPort,
    IN USHORT NumberOfPorts,
    OUT PULONG64 Token
    );

ULONG
WINAPI
CreatePersistentUdpPortReservation(
    IN USHORT StartPort,
    IN USHORT NumberOfPorts,
    OUT PULONG64 Token
    );

ULONG
WINAPI
DeletePersistentTcpPortReservation(
    IN USHORT StartPort,
    IN USHORT NumberOfPorts
    );

ULONG
WINAPI
DeletePersistentUdpPortReservation(
    IN USHORT StartPort,
    IN USHORT NumberOfPorts
    );

ULONG
WINAPI
LookupPersistentTcpPortReservation(
    IN USHORT StartPort,
    IN USHORT NumberOfPorts,
    OUT PULONG64 Token
    );

ULONG
WINAPI
LookupPersistentUdpPortReservation(
    IN USHORT StartPort,
    IN USHORT NumberOfPorts,
    OUT PULONG64 Token
    );


//
// Network String parsing API
//

#define NET_STRING_IPV4_ADDRESS           0x00000001
   // The string identifies an IPv4 Host/router using literal address.
   // (port or prefix not allowed) 
#define NET_STRING_IPV4_SERVICE           0x00000002
   // The string identifies an IPv4 service using literal address.
   // (port required; prefix not allowed) 
#define NET_STRING_IPV4_NETWORK           0x00000004
   // The string identifies an IPv4 network.
   // (prefix required; port not allowed) 
#define NET_STRING_IPV6_ADDRESS           0x00000008
   // The string identifies an IPv6 Host/router using literal address.
   // (port or prefix not allowed; scope-id allowed) 
#define NET_STRING_IPV6_ADDRESS_NO_SCOPE  0x00000010
   // The string identifies an IPv6 Host/router using literal address
   // where the interface context is already known.
   // (port or prefix not allowed; scope-id not allowed) 
#define NET_STRING_IPV6_SERVICE           0x00000020
   // The string identifies an IPv6 service using literal address.
   // (port required; prefix not allowed; scope-id allowed) 
#define NET_STRING_IPV6_SERVICE_NO_SCOPE  0x00000040
   // The string identifies an IPv6 service using literal address
   // where the interface context is already known.
   // (port required; prefix not allowed; scope-id not allowed) 
#define NET_STRING_IPV6_NETWORK           0x00000080
   // The string identifies an IPv6 network.
   // (prefix required; port or scope-id not allowed) 
#define NET_STRING_NAMED_ADDRESS          0x00000100
   // The string identifies an Internet Host using DNS.
   // (port or prefix or scope-id not allowed) 
#define NET_STRING_NAMED_SERVICE          0x00000200
   // The string identifies an Internet service using DNS.
   // (port required; prefix or scope-id not allowed)

#define NET_STRING_IP_ADDRESS             (NET_STRING_IPV4_ADDRESS   | \
                                           NET_STRING_IPV6_ADDRESS)

#define NET_STRING_IP_ADDRESS_NO_SCOPE    (NET_STRING_IPV4_ADDRESS   | \
                                           NET_STRING_IPV6_ADDRESS_NO_SCOPE)

#define NET_STRING_IP_SERVICE             (NET_STRING_IPV4_SERVICE   | \
                                           NET_STRING_IPV6_SERVICE)

#define NET_STRING_IP_SERVICE_NO_SCOPE    (NET_STRING_IPV4_SERVICE   | \
                                           NET_STRING_IPV6_SERVICE_NO_SCOPE)

#define NET_STRING_IP_NETWORK             (NET_STRING_IPV4_NETWORK   | \
                                           NET_STRING_IPV6_NETWORK)

#define NET_STRING_ANY_ADDRESS            (NET_STRING_NAMED_ADDRESS  | \
                                           NET_STRING_IP_ADDRESS)

#define NET_STRING_ANY_ADDRESS_NO_SCOPE   (NET_STRING_NAMED_ADDRESS  | \
                                           NET_STRING_IP_ADDRESS_NO_SCOPE)

#define NET_STRING_ANY_SERVICE            (NET_STRING_NAMED_SERVICE  | \
                                           NET_STRING_IP_SERVICE)

#define NET_STRING_ANY_SERVICE_NO_SCOPE   (NET_STRING_NAMED_SERVICE  | \
                                           NET_STRING_IP_SERVICE_NO_SCOPE)

typedef enum NET_ADDRESS_FORMAT_
{
   NET_ADDRESS_FORMAT_UNSPECIFIED = 0,

   NET_ADDRESS_DNS_NAME,
   NET_ADDRESS_IPV4,
   NET_ADDRESS_IPV6

} NET_ADDRESS_FORMAT;

#if defined (_WS2DEF_) && defined (_WS2IPDEF_) && defined(_WINDNS_INCLUDED_)
	// app must include winsock2.h, ws2ipdef.h, and windns.h to use this API

typedef struct NET_ADDRESS_INFO_
{
   NET_ADDRESS_FORMAT Format;

   union
   {
      struct {
         WCHAR Address[DNS_MAX_NAME_BUFFER_LENGTH];   
         WCHAR Port[6];
      } NamedAddress;

      SOCKADDR_IN Ipv4Address;
      SOCKADDR_IN6 Ipv6Address;
      SOCKADDR IpAddress;
   };

} NET_ADDRESS_INFO, *PNET_ADDRESS_INFO;

DWORD
WINAPI
ParseNetworkString(
   IN CONST WCHAR* NetworkString,
   IN DWORD Types,
   OUT PNET_ADDRESS_INFO AddressInfo OPTIONAL,
   OUT USHORT* PortNumber OPTIONAL,
   OUT BYTE* PrefixLength OPTIONAL
   );

#endif

#include <netioapi.h>

#endif // (NTDDI_VERSION >= NTDDI_LONGHORN)

#ifdef __cplusplus
}
#endif

#endif //__IPHLPAPI_H__

