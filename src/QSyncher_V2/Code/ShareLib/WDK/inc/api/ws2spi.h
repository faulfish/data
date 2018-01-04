//$TAG BIZDEV
//  $IPCategory:     
//  $DealPointID:    118736
//  $AgreementName:  berkeley software distribution license
//  $AgreementType:  oss license
//  $ExternalOrigin: regents of the university of california
//$ENDTAG

//$TAG ENGR 
//  $Owner:    vadime
//  $Module:   published_inc
//
//$ENDTAG

/* WS2SPI.H -- definitions to be used with the WinSock service provider.
 *
 * Copyright (c) Microsoft Corporation. All rights reserved.
 *
 * This header file corresponds to version 2.2.x of the WinSock SPI
 * specification.
 *
 * This file includes parts which are Copyright (c) 1982-1986 Regents
 * of the University of California.  All rights reserved.  The
 * Berkeley Software License Agreement specifies the terms and
 * conditions for redistribution.
 */

#ifndef _WINSOCK2SPI_
#define _WINSOCK2SPI_

#if _MSC_VER > 1000
#pragma once
#endif

/*
 * Pull in WINSOCK2.H if necessary
 */

#ifndef _WINSOCK2API_
#include <winsock2.h>
#endif /* _WINSOCK2API_ */

/*
 * Ensure structures are packed consistently.
 */

#if !defined(_WIN64)
#include <pshpack4.h>
#endif

#define WSPDESCRIPTION_LEN 255

#define WSS_OPERATION_IN_PROGRESS 0x00000103L

typedef struct WSPData {
    WORD         wVersion;
    WORD         wHighVersion;
    WCHAR        szDescription[WSPDESCRIPTION_LEN+1];
} WSPDATA, FAR * LPWSPDATA;

typedef struct _WSATHREADID {
    HANDLE      ThreadHandle;
    DWORD_PTR    Reserved;
} WSATHREADID, FAR * LPWSATHREADID;

/*
 * SPI function linkage.
 */

#define WSPAPI WSAAPI


#ifdef __cplusplus
extern "C" {
#endif

/*
 * Pointer to a blocking callback. A pointer to a blocking callback is
 * returned from the WPUQueryBlockingCallback() upcall. Note that this
 * function's signature is not identical to an application's blocking
 * hook function.
 */

typedef
BOOL
(CALLBACK FAR * LPBLOCKINGCALLBACK)(
    DWORD_PTR dwContext
    );

/*
 * Pointer to a user APC function. This is used as a parameter to the
 * WPUQueueUserApc() upcall. Note that this function's signature is not
 * identical to an application's completion routine.
 */

typedef
VOID
(CALLBACK FAR * LPWSAUSERAPC)(
    DWORD_PTR dwContext
    );

/*
 * Pointers to the individual entries in a service provider's proc table.
 */

typedef
SOCKET
(WSPAPI * LPWSPACCEPT)(
    SOCKET s,
    struct sockaddr FAR * addr,
    LPINT addrlen,
    LPCONDITIONPROC lpfnCondition,
    DWORD_PTR dwCallbackData,
    LPINT lpErrno
    );

typedef
INT
(WSPAPI * LPWSPADDRESSTOSTRING)(
    LPSOCKADDR lpsaAddress,
    DWORD dwAddressLength,
    LPWSAPROTOCOL_INFOW lpProtocolInfo,
    LPWSTR lpszAddressString,
    LPDWORD lpdwAddressStringLength,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPASYNCSELECT)(
    SOCKET s,
    HWND hWnd,
    unsigned int wMsg,
    long lEvent,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPBIND)(
    SOCKET s,
    const struct sockaddr FAR * name,
    int namelen,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPCANCELBLOCKINGCALL)(
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPCLEANUP)(
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPCLOSESOCKET)(
    SOCKET s,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPCONNECT)(
    SOCKET s,
    const struct sockaddr FAR * name,
    int namelen,
    LPWSABUF lpCallerData,
    LPWSABUF lpCalleeData,
    LPQOS lpSQOS,
    LPQOS lpGQOS,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPDUPLICATESOCKET)(
    SOCKET s,
    DWORD dwProcessId,
    LPWSAPROTOCOL_INFOW lpProtocolInfo,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPENUMNETWORKEVENTS)(
    SOCKET s,
    WSAEVENT hEventObject,
    LPWSANETWORKEVENTS lpNetworkEvents,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPEVENTSELECT)(
    SOCKET s,
    WSAEVENT hEventObject,
    long lNetworkEvents,
    LPINT lpErrno
    );

typedef
BOOL
(WSPAPI * LPWSPGETOVERLAPPEDRESULT)(
    SOCKET s,
    LPWSAOVERLAPPED lpOverlapped,
    LPDWORD lpcbTransfer,
    BOOL fWait,
    LPDWORD lpdwFlags,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPGETPEERNAME)(
    SOCKET s,
    struct sockaddr FAR * name,
    LPINT namelen,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPGETSOCKNAME)(
    SOCKET s,
    struct sockaddr FAR * name,
    LPINT namelen,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPGETSOCKOPT)(
    SOCKET s,
    int level,
    int optname,
    char FAR * optval,
    LPINT optlen,
    LPINT lpErrno
    );

typedef
BOOL
(WSPAPI * LPWSPGETQOSBYNAME)(
    SOCKET s,
    LPWSABUF lpQOSName,
    LPQOS lpQOS,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPIOCTL)(
    SOCKET s,
    DWORD dwIoControlCode,
    LPVOID lpvInBuffer,
    DWORD cbInBuffer,
    LPVOID lpvOutBuffer,
    DWORD cbOutBuffer,
    LPDWORD lpcbBytesReturned,
    LPWSAOVERLAPPED lpOverlapped,
    LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
    LPWSATHREADID lpThreadId,
    LPINT lpErrno
    );

typedef
SOCKET
(WSPAPI * LPWSPJOINLEAF)(
    SOCKET s,
    const struct sockaddr FAR * name,
    int namelen,
    LPWSABUF lpCallerData,
    LPWSABUF lpCalleeData,
    LPQOS lpSQOS,
    LPQOS lpGQOS,
    DWORD dwFlags,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPLISTEN)(
    SOCKET s,
    int backlog,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPRECV)(
    SOCKET s,
    LPWSABUF lpBuffers,
    DWORD dwBufferCount,
    LPDWORD lpNumberOfBytesRecvd,
    LPDWORD lpFlags,
    LPWSAOVERLAPPED lpOverlapped,
    LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
    LPWSATHREADID lpThreadId,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPRECVDISCONNECT)(
    SOCKET s,
    LPWSABUF lpInboundDisconnectData,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPRECVFROM)(
    SOCKET s,
    LPWSABUF lpBuffers,
    DWORD dwBufferCount,
    LPDWORD lpNumberOfBytesRecvd,
    LPDWORD lpFlags,
    struct sockaddr FAR * lpFrom,
    LPINT lpFromlen,
    LPWSAOVERLAPPED lpOverlapped,
    LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
    LPWSATHREADID lpThreadId,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPSELECT)(
    int nfds,
    fd_set FAR * readfds,
    fd_set FAR * writefds,
    fd_set FAR * exceptfds,
    const struct timeval FAR * timeout,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPSEND)(
    SOCKET s,
    LPWSABUF lpBuffers,
    DWORD dwBufferCount,
    LPDWORD lpNumberOfBytesSent,
    DWORD dwFlags,
    LPWSAOVERLAPPED lpOverlapped,
    LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
    LPWSATHREADID lpThreadId,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPSENDDISCONNECT)(
    SOCKET s,
    LPWSABUF lpOutboundDisconnectData,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPSENDTO)(
    SOCKET s,
    LPWSABUF lpBuffers,
    DWORD dwBufferCount,
    LPDWORD lpNumberOfBytesSent,
    DWORD dwFlags,
    const struct sockaddr FAR * lpTo,
    int iTolen,
    LPWSAOVERLAPPED lpOverlapped,
    LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
    LPWSATHREADID lpThreadId,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPSETSOCKOPT)(
    SOCKET s,
    int level,
    int optname,
    const char FAR * optval,
    int optlen,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSPSHUTDOWN)(
    SOCKET s,
    int how,
    LPINT lpErrno
    );

typedef
SOCKET
(WSPAPI * LPWSPSOCKET)(
    int af,
    int type,
    int protocol,
    LPWSAPROTOCOL_INFOW lpProtocolInfo,
    GROUP g,
    DWORD dwFlags,
    LPINT lpErrno
    );

typedef
INT
(WSPAPI * LPWSPSTRINGTOADDRESS)(
    LPWSTR AddressString,
    INT AddressFamily,
    LPWSAPROTOCOL_INFOW lpProtocolInfo,
    LPSOCKADDR lpAddress,
    LPINT lpAddressLength,
    LPINT lpErrno
    );

/*
 * A service provider proc table. This structure is returned by value
 * from the service provider's WSPStartup() entrypoint.
 */

typedef struct _WSPPROC_TABLE {

    LPWSPACCEPT              lpWSPAccept;
    LPWSPADDRESSTOSTRING     lpWSPAddressToString;
    LPWSPASYNCSELECT         lpWSPAsyncSelect;
    LPWSPBIND                lpWSPBind;
    LPWSPCANCELBLOCKINGCALL  lpWSPCancelBlockingCall;
    LPWSPCLEANUP             lpWSPCleanup;
    LPWSPCLOSESOCKET         lpWSPCloseSocket;
    LPWSPCONNECT             lpWSPConnect;
    LPWSPDUPLICATESOCKET     lpWSPDuplicateSocket;
    LPWSPENUMNETWORKEVENTS   lpWSPEnumNetworkEvents;
    LPWSPEVENTSELECT         lpWSPEventSelect;
    LPWSPGETOVERLAPPEDRESULT lpWSPGetOverlappedResult;
    LPWSPGETPEERNAME         lpWSPGetPeerName;
    LPWSPGETSOCKNAME         lpWSPGetSockName;
    LPWSPGETSOCKOPT          lpWSPGetSockOpt;
    LPWSPGETQOSBYNAME        lpWSPGetQOSByName;
    LPWSPIOCTL               lpWSPIoctl;
    LPWSPJOINLEAF            lpWSPJoinLeaf;
    LPWSPLISTEN              lpWSPListen;
    LPWSPRECV                lpWSPRecv;
    LPWSPRECVDISCONNECT      lpWSPRecvDisconnect;
    LPWSPRECVFROM            lpWSPRecvFrom;
    LPWSPSELECT              lpWSPSelect;
    LPWSPSEND                lpWSPSend;
    LPWSPSENDDISCONNECT      lpWSPSendDisconnect;
    LPWSPSENDTO              lpWSPSendTo;
    LPWSPSETSOCKOPT          lpWSPSetSockOpt;
    LPWSPSHUTDOWN            lpWSPShutdown;
    LPWSPSOCKET              lpWSPSocket;
    LPWSPSTRINGTOADDRESS     lpWSPStringToAddress;

} WSPPROC_TABLE, FAR * LPWSPPROC_TABLE;

/*
 * Pointers to the individual entries in the upcall table.
 */

typedef
BOOL
(WSPAPI * LPWPUCLOSEEVENT)(
    WSAEVENT hEvent,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWPUCLOSESOCKETHANDLE)(
    SOCKET s,
    LPINT lpErrno
    );

typedef
WSAEVENT
(WSPAPI * LPWPUCREATEEVENT)(
    LPINT lpErrno
    );

typedef
SOCKET
(WSPAPI * LPWPUCREATESOCKETHANDLE)(
    DWORD dwCatalogEntryId,
    DWORD_PTR dwContext,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWPUFDISSET)(
    SOCKET s,
    fd_set FAR * fdset
    );

typedef
int
(WSPAPI * LPWPUGETPROVIDERPATH)(
    LPGUID lpProviderId,
    WCHAR FAR * lpszProviderDllPath,
    LPINT lpProviderDllPathLen,
    LPINT lpErrno
    );

typedef
SOCKET
(WSPAPI * LPWPUMODIFYIFSHANDLE)(
    DWORD dwCatalogEntryId,
    SOCKET ProposedHandle,
    LPINT lpErrno
    );

typedef
BOOL
(WSPAPI * LPWPUPOSTMESSAGE)(
    HWND hWnd,
    UINT Msg,
    WPARAM wParam,
    LPARAM lParam
    );

typedef
int
(WSPAPI * LPWPUQUERYBLOCKINGCALLBACK)(
    DWORD dwCatalogEntryId,
    LPBLOCKINGCALLBACK FAR * lplpfnCallback,
    PDWORD_PTR lpdwContext,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWPUQUERYSOCKETHANDLECONTEXT)(
    SOCKET s,
    PDWORD_PTR lpContext,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWPUQUEUEAPC)(
    LPWSATHREADID lpThreadId,
    LPWSAUSERAPC lpfnUserApc,
    DWORD_PTR dwContext,
    LPINT lpErrno
    );

typedef
BOOL
(WSPAPI * LPWPURESETEVENT)(
    WSAEVENT hEvent,
    LPINT lpErrno
    );

typedef
BOOL
(WSPAPI * LPWPUSETEVENT)(
    WSAEVENT hEvent,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWPUOPENCURRENTTHREAD)(
    LPWSATHREADID lpThreadId,
    LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWPUCLOSETHREAD)(
    LPWSATHREADID lpThreadId,
    LPINT lpErrno
    );


// Available only directly from ws2_32.dll
typedef
int
(WSPAPI * LPWPUCOMPLETEOVERLAPPEDREQUEST) (
    SOCKET s,
    LPWSAOVERLAPPED lpOverlapped,
    DWORD dwError,
    DWORD cbTransferred,
    LPINT lpErrno
);

/*
 * The upcall table. This structure is passed by value to the service
 * provider's WSPStartup() entrypoint.
 */

typedef struct _WSPUPCALLTABLE {

    LPWPUCLOSEEVENT               lpWPUCloseEvent;
    LPWPUCLOSESOCKETHANDLE        lpWPUCloseSocketHandle;
    LPWPUCREATEEVENT              lpWPUCreateEvent;
    LPWPUCREATESOCKETHANDLE       lpWPUCreateSocketHandle;
    LPWPUFDISSET                  lpWPUFDIsSet;
    LPWPUGETPROVIDERPATH          lpWPUGetProviderPath;
    LPWPUMODIFYIFSHANDLE          lpWPUModifyIFSHandle;
    LPWPUPOSTMESSAGE              lpWPUPostMessage;
    LPWPUQUERYBLOCKINGCALLBACK    lpWPUQueryBlockingCallback;
    LPWPUQUERYSOCKETHANDLECONTEXT lpWPUQuerySocketHandleContext;
    LPWPUQUEUEAPC                 lpWPUQueueApc;
    LPWPURESETEVENT               lpWPUResetEvent;
    LPWPUSETEVENT                 lpWPUSetEvent;
    LPWPUOPENCURRENTTHREAD        lpWPUOpenCurrentThread;
    LPWPUCLOSETHREAD              lpWPUCloseThread;

} WSPUPCALLTABLE, FAR * LPWSPUPCALLTABLE;

/*
 *  WinSock 2 SPI socket function prototypes
 */

__control_entrypoint(DllExport)
int
WSPAPI
WSPStartup(
    IN WORD wVersionRequested,
    __in OUT LPWSPDATA lpWSPData,
    __in IN LPWSAPROTOCOL_INFOW lpProtocolInfo,
    __in IN WSPUPCALLTABLE UpcallTable,
    __inout OUT LPWSPPROC_TABLE lpProcTable
    );

typedef
int
(WSPAPI * LPWSPSTARTUP)(
    WORD wVersionRequested,
    LPWSPDATA lpWSPData,
    LPWSAPROTOCOL_INFOW lpProtocolInfo,
    WSPUPCALLTABLE UpcallTable,
    LPWSPPROC_TABLE lpProcTable
    );

/*
 * Installation and configuration entrypoints.
 */

__control_entrypoint(DllExport)
int
WSPAPI
WSCEnumProtocols(
    IN LPINT lpiProtocols,
    __out_bcount_opt(*lpdwBufferLength) LPWSAPROTOCOL_INFOW lpProtocolBuffer,
    IN OUT LPDWORD lpdwBufferLength,
    OUT LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSCENUMPROTOCOLS)(
    LPINT lpiProtocols,
    LPWSAPROTOCOL_INFOW lpProtocolBuffer,
    LPDWORD lpdwBufferLength,
    LPINT lpErrno
    );

#if(defined(_WIN64) && (_WIN32_WINNT >= 0x0501))

/*
 * 64-bit architectures capable of running 32-bit code have
 * separate 64-bit and 32-bit catalogs.  API with '32' prefix
 * allow 32 bit catalog manipulations by 64 bit process.
 */
__control_entrypoint(DllExport)
int
WSPAPI
WSCEnumProtocols32(
    IN LPINT lpiProtocols,
    __out_bcount(*lpdwBufferLength) LPWSAPROTOCOL_INFOW lpProtocolBuffer,
    IN OUT LPDWORD lpdwBufferLength,
    OUT LPINT lpErrno
    );
#endif //(defined(_WIN64) && _WIN32_WINNT >= 0x0501)

__control_entrypoint(DllExport)
int
WSPAPI
WSCDeinstallProvider(
    IN LPGUID lpProviderId,
    OUT LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSCDEINSTALLPROVIDER)(
    LPGUID lpProviderId,
    LPINT lpErrno
    );

#if(defined(_WIN64) && (_WIN32_WINNT >= 0x0501))

__control_entrypoint(DllExport)
int
WSPAPI
WSCDeinstallProvider32(
    IN LPGUID lpProviderId,
    OUT LPINT lpErrno
    );
#endif //(defined(_WIN64) && _WIN32_WINNT >= 0x0501)

__control_entrypoint(DllExport)
int
WSPAPI
WSCInstallProvider(
    IN LPGUID lpProviderId,
    IN const WCHAR FAR * lpszProviderDllPath,
    __in_ecount(dwNumberOfEntries) const LPWSAPROTOCOL_INFOW lpProtocolInfoList,
    IN DWORD dwNumberOfEntries,
    OUT LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSCINSTALLPROVIDER)(
    LPGUID lpProviderId,
    const WCHAR FAR * lpszProviderDllPath,
    const LPWSAPROTOCOL_INFOW lpProtocolInfoList,
    DWORD dwNumberOfEntries,
    LPINT lpErrno
    );

#if(defined(_WIN64) && (_WIN32_WINNT >= 0x0501))

/*
 * This API manipulates 64-bit and 32-bit catalogs simulteneously.
 * It is needed to guarantee the same catalog IDs for provider catalog
 * entries in both 64-bit and 32-bit catalogs.
 */
__control_entrypoint(DllExport)
int
WSPAPI
WSCInstallProvider64_32(
    IN LPGUID lpProviderId,
    IN const WCHAR FAR * lpszProviderDllPath,
    __in_ecount(dwNumberOfEntries) const LPWSAPROTOCOL_INFOW lpProtocolInfoList,
    IN DWORD dwNumberOfEntries,
    OUT LPINT lpErrno
    );
#endif //(defined(_WIN64) && (_WIN32_WINNT >= 0x0501))

__control_entrypoint(DllExport)
int
WSPAPI
WSCGetProviderPath(
    IN LPGUID lpProviderId,
    __out_ecount(*lpProviderDllPathLen) WCHAR FAR * lpszProviderDllPath,
    IN OUT LPINT lpProviderDllPathLen,
    OUT LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSCGETPROVIDERPATH)(
    LPGUID lpProviderId,
    WCHAR FAR * lpszProviderDllPath,
    LPINT lpProviderDllPathLen,
    LPINT lpErrno
    );

#if(_WIN32_WINNT >= 0x0501)
#if defined(_WIN64)
__control_entrypoint(DllExport)
int
WSPAPI
WSCGetProviderPath32(
    IN LPGUID lpProviderId,
    __out_ecount(*lpProviderDllPathLen) WCHAR FAR * lpszProviderDllPath,
    IN OUT LPINT lpProviderDllPathLen,
    OUT LPINT lpErrno
    );
#endif

__control_entrypoint(DllExport)
int
WSPAPI
WSCUpdateProvider(
    IN  LPGUID lpProviderId,
    IN  const WCHAR FAR * lpszProviderDllPath,
    __in_ecount(dwNumberOfEntries) const LPWSAPROTOCOL_INFOW lpProtocolInfoList,
    IN  DWORD dwNumberOfEntries,
    OUT LPINT lpErrno
    );

typedef
int
(WSPAPI * LPWSCUPDATEPROVIDER)(
    LPGUID lpProviderId,
    const WCHAR FAR * lpszProviderDllPath,
    const LPWSAPROTOCOL_INFOW lpProtocolInfoList,
    DWORD dwNumberOfEntries,
    LPINT lpErrno
    );

#if defined(_WIN64)
__control_entrypoint(DllExport)
int
WSPAPI
WSCUpdateProvider32(
    IN  LPGUID lpProviderId,
    IN  const WCHAR FAR * lpszProviderDllPath,
    __in_ecount(dwNumberOfEntries) const LPWSAPROTOCOL_INFOW lpProtocolInfoList,
    IN  DWORD dwNumberOfEntries,
    OUT LPINT lpErrno
    );
#endif
#endif //(_WIN32_WINNT >= 0x0501)

#if (_WIN32_WINNT < 0x0600)
int
WSPAPI
WSCInstallQOSTemplate (
    IN  const LPGUID    Guid,
    IN  LPWSABUF        QosName,
    IN  LPQOS           Qos
    );

typedef
int
(WSPAPI * LPWSCINSTALLQOSTEMPLATE)(
    const LPGUID    Guid,
    LPWSABUF        QosName,
    LPQOS           Qos
    );

int
WSPAPI
WSCRemoveQOSTemplate (
    IN  const LPGUID    Guid,
    IN  LPWSABUF        QosName
    );

typedef
int
(WSPAPI * LPWSCREMOVEQOSTEMPLATE)(
    const LPGUID    Guid,
    LPWSABUF        QosName
    );

#endif //(_WIN32_WINNT < 0x0600)

#if(_WIN32_WINNT >= 0x0600)

//
// LSP Categorization
//

#define LSP_SYSTEM             0x80000000
#define LSP_INSPECTOR          0x00000001
#define LSP_REDIRECTOR         0x00000002
#define LSP_PROXY              0x00000004
#define LSP_FIREWALL           0x00000008
#define LSP_INBOUND_MODIFY     0x00000010
#define LSP_OUTBOUND_MODIFY    0x00000020
#define LSP_CRYPTO_COMPRESS    0x00000040
#define LSP_LOCAL_CACHE        0x00000080

//
// Provider Information APIs
//

typedef enum _WSC_PROVIDER_INFO_TYPE {
//  InfoType is:                  Info points to:
    ProviderInfoLspCategories, // DWORD (LspCategories)
    ProviderInfoAudit,         // struct WSC_PROVIDER_AUDIT_INFO
} WSC_PROVIDER_INFO_TYPE ;

typedef struct _WSC_PROVIDER_AUDIT_INFO {
    DWORD   RecordSize;     // Size of this audit record, in bytes (includes this field)
    PVOID   Reserved;       // Reserved
    // Install Time (GMT)
    // User SID of account performing install
    // Length (in characters) of Full Path
    // Full Path of EXE performing install
} WSC_PROVIDER_AUDIT_INFO;

__control_entrypoint(DllExport)
int
WSPAPI
WSCSetProviderInfo( 
    __in LPGUID lpProviderId,
    __in WSC_PROVIDER_INFO_TYPE InfoType,
    __in_bcount(InfoSize) PBYTE Info,
    __in size_t InfoSize,
    __in DWORD Flags,
    __out LPINT lpErrno
    );

__control_entrypoint(DllExport)
int
WSPAPI
WSCGetProviderInfo( 
    __in LPGUID lpProviderId,
    __in WSC_PROVIDER_INFO_TYPE InfoType,
    __out_bcount_part(*InfoSize, *InfoSize) PBYTE Info,
    __inout size_t *InfoSize,
    __in DWORD Flags,
    __out LPINT lpErrno
    );

#if defined(_WIN64)

__control_entrypoint(DllExport)
int
WSPAPI
WSCSetProviderInfo32( 
    __in LPGUID lpProviderId,
    __in WSC_PROVIDER_INFO_TYPE InfoType,
    __in_bcount(InfoSize) PBYTE Info,
    __in size_t InfoSize,
    __in DWORD Flags,
    __out LPINT lpErrno
    );

__control_entrypoint(DllExport)
int
WSPAPI
WSCGetProviderInfo32( 
    __in LPGUID lpProviderId,
    __in WSC_PROVIDER_INFO_TYPE InfoType,
    __out_bcount_part(*InfoSize, *InfoSize) PBYTE Info,
    __inout size_t *InfoSize,
    __in DWORD Flags,
    __out LPINT lpErrno
    );

#endif // (_WIN64)

//
// App Permitted LSP Categorization APIs
//

__control_entrypoint(DllExport)
int
WSPAPI
WSCSetApplicationCategory(
    __in_ecount(PathLength) LPCWSTR Path,
    __in DWORD PathLength,
    __in_ecount_opt(ExtraLength) LPCWSTR Extra,
    __in DWORD ExtraLength,
    __in DWORD PermittedLspCategories,
    __out_opt DWORD * pPrevPermLspCat,
    __out LPINT lpErrno
    );

__control_entrypoint(DllExport)
int
WSPAPI
WSCGetApplicationCategory(
    __in_ecount(PathLength) LPCWSTR Path,
    __in DWORD PathLength,
    __in_ecount_opt(ExtraLength) LPCWSTR Extra,
    __in DWORD ExtraLength,
    __out DWORD * pPermittedLspCategories,
    __out LPINT lpErrno
    );

#endif //(_WIN32_WINNT >= 0x0600)


/*
 *  The following upcall function prototypes are only used by WinSock 2 DLL and
 *  should not be used by any service providers.
 */

BOOL
WSPAPI
WPUCloseEvent(
    IN WSAEVENT hEvent,
    OUT LPINT lpErrno
    );

int
WSPAPI
WPUCloseSocketHandle(
    IN SOCKET s,
    OUT LPINT lpErrno
    );

WSAEVENT
WSPAPI
WPUCreateEvent(
    OUT LPINT lpErrno
    );

SOCKET
WSPAPI
WPUCreateSocketHandle(
    IN DWORD dwCatalogEntryId,
    IN DWORD_PTR dwContext,
    OUT LPINT lpErrno
    );

__control_entrypoint(DispatchTable)
int
WSPAPI
WPUFDIsSet(
    IN SOCKET s,
    IN fd_set FAR * fdset
    );

int
WSPAPI
WPUGetProviderPath(
    IN LPGUID lpProviderId,
    __out_ecount(*lpProviderDllPathLen) WCHAR FAR * lpszProviderDllPath,
    __inout LPINT lpProviderDllPathLen,
    OUT LPINT lpErrno
    );

SOCKET
WSPAPI
WPUModifyIFSHandle(
    IN DWORD dwCatalogEntryId,
    IN SOCKET ProposedHandle,
    OUT LPINT lpErrno
    );

__control_entrypoint(DispatchTable)
BOOL
WSPAPI
WPUPostMessage(
    IN HWND hWnd,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam
    );

int
WSPAPI
WPUQueryBlockingCallback(
    IN DWORD dwCatalogEntryId,
    OUT LPBLOCKINGCALLBACK FAR * lplpfnCallback,
    OUT PDWORD_PTR lpdwContext,
    OUT LPINT lpErrno
    );

int
WSPAPI
WPUQuerySocketHandleContext(
    IN SOCKET s,
    OUT PDWORD_PTR lpContext,
    OUT LPINT lpErrno
    );

__control_entrypoint(DispatchTable)
int
WSPAPI
WPUQueueApc(
    IN LPWSATHREADID lpThreadId,
    IN LPWSAUSERAPC lpfnUserApc,
    IN DWORD_PTR dwContext,
    OUT LPINT lpErrno
    );

BOOL
WSPAPI
WPUResetEvent(
    IN WSAEVENT hEvent,
    OUT LPINT lpErrno
    );

BOOL
WSPAPI
WPUSetEvent(
    IN WSAEVENT hEvent,
    OUT LPINT lpErrno
    );

__control_entrypoint(DispatchTable)
int
WSPAPI
WPUCompleteOverlappedRequest (
    SOCKET s, 	
    LPWSAOVERLAPPED lpOverlapped, 	
    DWORD dwError, 	
    DWORD cbTransferred, 	
    LPINT lpErrno
);

#if(_WIN32_WINNT >= 0x0501)
__control_entrypoint(DispatchTable)
int
WSPAPI
WPUOpenCurrentThread(
    LPWSATHREADID lpThreadId,
    LPINT lpErrno
    );

__control_entrypoint(DispatchTable)
int
WSPAPI
WPUCloseThread(
    LPWSATHREADID lpThreadId,
    LPINT lpErrno
    );
#endif //(_WIN32_WINNT >= 0x0501)

/*
 * Installing and uninstalling name space providers.
 */

#if(_WIN32_WINNT >= 0x0501)
/*
 * SPI and API for enumerating name space providers are
 * currently equivalent since there is no concept of a hidden
 * name space provider
 */
#define WSCEnumNameSpaceProviders WSAEnumNameSpaceProvidersW
#define LPFN_WSCENUMNAMESPACEPROVIDERS LPFN_WSAENUMNAMESPACEPROVIDERSW

#if defined(_WIN64)
INT
WSAAPI
WSCEnumNameSpaceProviders32(
    __inout LPDWORD             lpdwBufferLength,
    __out_bcount(*lpdwBufferLength) LPWSANAMESPACE_INFOW lpnspBuffer
    );
#endif
#endif //(_WIN32_WINNT >= 0x0501)

#if(_WIN32_WINNT >= 0x0600)
#define WSCEnumNameSpaceProvidersEx WSAEnumNameSpaceProvidersExW
#define LPFN_WSCENUMNAMESPACEPROVIDERSEX LPFN_WSAENUMNAMESPACEPROVIDERSEXW

#if defined(_WIN64)
INT
WSAAPI
WSCEnumNameSpaceProvidersEx32(
    __inout LPDWORD     lpdwBufferLength,
    __out_bcount(*lpdwBufferLength) LPWSANAMESPACE_INFOEXW lpnspBuffer
    );
#endif //(_WIN64)
#endif //(_WIN32_WINNT >= 0x0600)

INT
WSPAPI
WSCInstallNameSpace (
    __in LPWSTR lpszIdentifier,
    __in LPWSTR lpszPathName,
    IN DWORD dwNameSpace,
    IN DWORD dwVersion,
    __in IN LPGUID lpProviderId
    );

typedef
INT
(WSPAPI * LPWSCINSTALLNAMESPACE)(
    LPWSTR lpszIdentifier,
    LPWSTR lpszPathName,
    DWORD dwNameSpace,
    DWORD dwVersion,
    LPGUID lpProviderId
    );

#if(defined(_WIN64) && (_WIN32_WINNT >= 0x0501))
INT
WSPAPI
WSCInstallNameSpace32 (
    __in LPWSTR lpszIdentifier,
    __in LPWSTR lpszPathName,
    IN DWORD dwNameSpace,
    IN DWORD dwVersion,
    IN LPGUID lpProviderId
    );
#endif //(defined(_WIN64) && (_WIN32_WINNT >= 0x0501))

INT
WSPAPI
WSCUnInstallNameSpace (
    IN LPGUID lpProviderId
    );

typedef
INT
(WSPAPI * LPWSCUNINSTALLNAMESPACE)(
    LPGUID lpProviderId
    );

#if (_WIN32_WINNT >= 0x0600 )
INT
WSPAPI
WSCInstallNameSpaceEx(
    __in LPWSTR lpszIdentifier,
    __in LPWSTR lpszPathName,
    IN DWORD dwNameSpace,
    IN DWORD dwVersion,
    __in LPGUID lpProviderId,
    __in LPBLOB lpProviderSpecific
    );
#endif //(defined(_WIN32_WINNT >= 0x0600)

#if(defined(_WIN64) && (_WIN32_WINNT >= 0x0600))
INT
WSPAPI
WSCInstallNameSpaceEx32(
    __in LPWSTR lpszIdentifier,
    __in LPWSTR lpszPathName,
    IN DWORD dwNameSpace,
    IN DWORD dwVersion,
    IN LPGUID lpProviderId,
    IN LPBLOB lpProviderSpecific
    );
#endif //(defined(_WIN64) && (_WIN32_WINNT >= 0x0600))

#if(defined(_WIN64) && (_WIN32_WINNT >= 0x0501))
INT
WSPAPI
WSCUnInstallNameSpace32 (
    IN LPGUID lpProviderId
    );
#endif //(defined(_WIN64) && (_WIN32_WINNT >= 0x0501))

INT
WSPAPI
WSCEnableNSProvider (
    IN LPGUID lpProviderId,
    IN BOOL fEnable
    );

typedef
INT
(WSPAPI * LPWSCENABLENSPROVIDER)(
    LPGUID lpProviderId,
    BOOL fEnable
    );

#if(defined(_WIN64) && (_WIN32_WINNT >= 0x0501))
INT
WSPAPI
WSCEnableNSProvider32 (
    IN LPGUID lpProviderId,
    IN BOOL fEnable
    );
#endif //(defined(_WIN64) && (_WIN32_WINNT >= 0x0501))

#if (_WIN32_WINNT >= 0x0600)

#if defined(_WIN64)
int WSPAPI WSCInstallProviderAndChains64_32(
#else
int WSPAPI WSCInstallProviderAndChains(
#endif
    IN LPGUID lpProviderId,
    __in const LPWSTR lpszProviderDllPath,
#if defined(_WIN64)
    __in const LPWSTR lpszProviderDllPath32,
#endif
    __in const LPWSTR lpszLspName,
    IN DWORD dwServiceFlags,
    __inout_ecount(dwNumberOfEntries) LPWSAPROTOCOL_INFOW lpProtocolInfoList,
    IN DWORD dwNumberOfEntries,
    OUT LPDWORD lpdwCatalogEntryId OPTIONAL,
    OUT LPINT lpErrno);

#endif //(_WIN32_WINNT >= 0x0600)


/*
 * Pointers to the individual entries in the namespace proc table.
 */

typedef
INT
(WSAAPI * LPNSPCLEANUP)(
    LPGUID lpProviderId
    );

typedef
INT
(WSAAPI * LPNSPLOOKUPSERVICEBEGIN)(
    LPGUID lpProviderId,
    LPWSAQUERYSETW lpqsRestrictions,
    LPWSASERVICECLASSINFOW lpServiceClassInfo,
    DWORD dwControlFlags,
    LPHANDLE lphLookup
    );

typedef
INT
(WSAAPI * LPNSPLOOKUPSERVICENEXT)(
    HANDLE hLookup,
    DWORD dwControlFlags,
    LPDWORD lpdwBufferLength,
    LPWSAQUERYSETW lpqsResults
    );

#if(_WIN32_WINNT >= 0x0501)
typedef
INT
(WSAAPI * LPNSPIOCTL)(
    HANDLE          hLookup,
    DWORD           dwControlCode,
    LPVOID          lpvInBuffer,
    DWORD           cbInBuffer,
    LPVOID          lpvOutBuffer,
    DWORD           cbOutBuffer,
    LPDWORD         lpcbBytesReturned,
    LPWSACOMPLETION lpCompletion,
    LPWSATHREADID   lpThreadId
    );
#endif //(_WIN32_WINNT >= 0x0501)

typedef
INT
(WSAAPI * LPNSPLOOKUPSERVICEEND)(
    HANDLE hLookup
    );

typedef
INT
(WSAAPI * LPNSPSETSERVICE)(
    LPGUID lpProviderId,
    LPWSASERVICECLASSINFOW lpServiceClassInfo,
    LPWSAQUERYSETW lpqsRegInfo,
    WSAESETSERVICEOP essOperation,
    DWORD dwControlFlags
    );

typedef
INT
(WSAAPI * LPNSPINSTALLSERVICECLASS)(
    LPGUID lpProviderId,
    LPWSASERVICECLASSINFOW lpServiceClassInfo
    );

typedef
INT
(WSAAPI * LPNSPREMOVESERVICECLASS)(
    LPGUID lpProviderId,
    LPGUID lpServiceClassId
    );

typedef
INT
(WSAAPI * LPNSPGETSERVICECLASSINFO)(
    LPGUID lpProviderId,
    LPDWORD lpdwBufSize,
    LPWSASERVICECLASSINFOW lpServiceClassInfo
    );

/*
 * The name space service provider procedure table.
 */

typedef struct _NSP_ROUTINE {

    /* Structure version information: */
    DWORD           cbSize;
    DWORD           dwMajorVersion;
    DWORD           dwMinorVersion;

    /* Procedure-pointer table: */

    LPNSPCLEANUP             NSPCleanup;
    LPNSPLOOKUPSERVICEBEGIN  NSPLookupServiceBegin;
    LPNSPLOOKUPSERVICENEXT   NSPLookupServiceNext;
    LPNSPLOOKUPSERVICEEND    NSPLookupServiceEnd;
    LPNSPSETSERVICE          NSPSetService;
    LPNSPINSTALLSERVICECLASS NSPInstallServiceClass;
    LPNSPREMOVESERVICECLASS  NSPRemoveServiceClass;
    LPNSPGETSERVICECLASSINFO NSPGetServiceClassInfo;

#if(_WIN32_WINNT >= 0x0501)
    // These APIs were added later, so must appear here
    // to keep the pointers in the structure in order.
    // Namespaces unaware of these APIs will set cbSize
    // to match the size of FIELD_OFFSET(NSP_ROUTINE, NSPIoctl).
    LPNSPIOCTL               NSPIoctl;
#endif //(_WIN32_WINNT >= 0x0501)

} NSP_ROUTINE, FAR * LPNSP_ROUTINE;

/*
 * Startup procedures.
 */

INT
WSAAPI
NSPStartup(
    __in LPGUID lpProviderId,
    __inout LPNSP_ROUTINE lpnspRoutines
    );

typedef
INT
(WSAAPI * LPNSPSTARTUP)(
    LPGUID lpProviderId,
    LPNSP_ROUTINE lpnspRoutines
    );

typedef
INT
(WSAAPI * LPNSPV2STARTUP)(
    LPGUID lpProviderId,
    LPVOID * ppvClientSessionArg
    );

typedef
INT
(WSAAPI * LPNSPV2CLEANUP)(
    LPGUID lpProviderId,
    LPVOID pvClientSessionArg
    );

typedef
INT
(WSAAPI * LPNSPV2LOOKUPSERVICEBEGIN)(
    LPGUID lpProviderId,
    LPWSAQUERYSET2W lpqsRestrictions,
    DWORD dwControlFlags,
    LPVOID lpvClientSessionArg,
    LPHANDLE lphLookup
    );

typedef
VOID
(WSAAPI * LPNSPV2LOOKUPSERVICENEXTEX)(
    HANDLE hAsyncCall,
    HANDLE hLookup,
    DWORD dwControlFlags,
    LPDWORD lpdwBufferLength,
    LPWSAQUERYSET2W lpqsResults
    );

typedef
INT
(WSAAPI * LPNSPV2LOOKUPSERVICEEND)(
    HANDLE hLookup
    );

typedef
VOID
(WSAAPI * LPNSPV2SETSERVICEEX)(
    HANDLE hAsyncCall,
    LPGUID lpProviderId,
    LPWSAQUERYSET2W lpqsRegInfo,
    WSAESETSERVICEOP essOperation,
    DWORD dwControlFlags,
    LPVOID lpvClientSessionArg 
    );

typedef
VOID
(WSAAPI * LPNSPV2CLIENTSESSIONRUNDOWN)(
    LPGUID lpProviderId,
    LPVOID pvClientSessionArg
    );

typedef struct _NSPV2_ROUTINE {  
    DWORD cbSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    LPNSPV2STARTUP NSPv2Startup;
    LPNSPV2CLEANUP NSPv2Cleanup;
    LPNSPV2LOOKUPSERVICEBEGIN NSPv2LookupServiceBegin;  
    LPNSPV2LOOKUPSERVICENEXTEX NSPv2LookupServiceNextEx;
    LPNSPV2LOOKUPSERVICEEND NSPv2LookupServiceEnd;  
    LPNSPV2SETSERVICEEX NSPv2SetServiceEx;
    LPNSPV2CLIENTSESSIONRUNDOWN NSPv2ClientSessionRundown;
} NSPV2_ROUTINE, *PNSPV2_ROUTINE, *LPNSPV2_ROUTINE;
typedef const NSPV2_ROUTINE * PCNSPV2_ROUTINE, * LPCNSPV2_ROUTINE;

#if(_WIN32_WINNT >= 0x0600)

INT WSAAPI WSAAdvertiseProvider(
        __in                const GUID *                    puuidProviderId,
        __in                const LPCNSPV2_ROUTINE          pNSPv2Routine);

INT WSAAPI WSAUnadvertiseProvider(
        __in                const GUID *                    puuidProviderId);

INT WSAAPI WSAProviderCompleteAsyncCall(
        __in                HANDLE                          hAsyncCall,
        __in                INT                             iRetCode);

#endif //(_WIN32_WINNT >= 0x0600)

#ifdef __cplusplus
}
#endif

#if !defined(_WIN64)
#include <poppack.h>
#endif

#endif  /* _WINSOCK2SPI_ */


