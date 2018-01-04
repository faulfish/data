/*++

Copyright (c) 1990  Microsoft Corporation

Module Name:

    WinSplp.h

Abstract:

    Internal Header file for Print APIs

Author:

Revision History:

--*/

#ifndef _WINSPLP_
#define _WINSPLP_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define PRINTER_NOTIFY_STATUS_ENDPOINT 1
#define PRINTER_NOTIFY_STATUS_POLL     2
#define PRINTER_NOTIFY_STATUS_INFO     4


#define ROUTER_UNKNOWN      0
#define ROUTER_SUCCESS      1
#define ROUTER_STOP_ROUTING 2

#if (NTDDI_VERSION >= NTDDI_WS03)
    #ifndef __ATTRIBUTE_INFO_3__
    #define __ATTRIBUTE_INFO_3__
    typedef struct _ATTRIBUTE_INFO_3 {
        DWORD    dwJobNumberOfPagesPerSide;
        DWORD    dwDrvNumberOfPagesPerSide;
        DWORD    dwNupBorderFlags;
        DWORD    dwJobPageOrderFlags;
        DWORD    dwDrvPageOrderFlags;
        DWORD    dwJobNumberOfCopies;
        DWORD    dwDrvNumberOfCopies;
        DWORD    dwColorOptimization;           // Added for monochrome optimization
        short    dmPrintQuality;                // Added for monochrome optimization
        short    dmYResolution;                 // Added for monochrome optimization
    } ATTRIBUTE_INFO_3, *PATTRIBUTE_INFO_3;

    #endif
#endif // (NTDDI_VERSION >= NTDDI_WS03)

#if (NTDDI_VERSION >= NTDDI_LONGHORN)
    #ifndef __ATTRIBUTE_INFO_4__
    #define __ATTRIBUTE_INFO_4__
    typedef struct _ATTRIBUTE_INFO_4 {
        DWORD    dwJobNumberOfPagesPerSide;
        DWORD    dwDrvNumberOfPagesPerSide;
        DWORD    dwNupBorderFlags;
        DWORD    dwJobPageOrderFlags;
        DWORD    dwDrvPageOrderFlags;
        DWORD    dwJobNumberOfCopies;
        DWORD    dwDrvNumberOfCopies;
        DWORD    dwColorOptimization;          // Added for monochrome optimization
        short    dmPrintQuality;               // Added for monochrome optimization
        short    dmYResolution;                // Added for monochrome optimization

        // _ATTRIBUTE_INFO_4 specific fields.
        DWORD    dwDuplexFlags;
        DWORD    dwNupDirection;
        DWORD    dwBookletFlags;
        DWORD    dwScalingPercentX;            // Scaling percentage in X direction.
        DWORD    dwScalingPercentY;            // Scaling percentage in Y direction.
    } ATTRIBUTE_INFO_4, *PATTRIBUTE_INFO_4;

    //dwDuplexFlags
    // The below flag tells print processor to flip page order
    // while printing reverse duplex.
    // e.g. Instead of 4,3,2,1, pages should be printed in order 3,4,1,2
    #define REVERSE_PAGES_FOR_REVERSE_DUPLEX ( 0x00000001      )
    #define DONT_SEND_EXTRA_PAGES_FOR_DUPLEX ( 0x00000001 << 1 ) // 0x00000002

    //Flags for dwNupDirection.
    #define RIGHT_THEN_DOWN                  ( 0x00000001      ) // 0x00000001
    #define DOWN_THEN_RIGHT                  ( 0x00000001 << 1 ) // 0x00000002
    #define LEFT_THEN_DOWN                   ( 0x00000001 << 2 ) // 0x00000004
    #define DOWN_THEN_LEFT                   ( 0x00000001 << 3 ) // 0x00000008


    //dwBookletFlags
    #define BOOKLET_EDGE_LEFT                ( 0x00000000 )
    #define BOOKLET_EDGE_RIGHT               ( 0x00000001 )

    #endif //__ATTRIBUTE_INFO_4__
#endif // (NTDDI_VERSION >= NTDDI_LONGHORN)



typedef struct _PRINTER_NOTIFY_INIT {
    DWORD Size;
    DWORD Reserved;
    DWORD PollTime;
} PRINTER_NOTIFY_INIT, *PPRINTER_NOTIFY_INIT, *LPPRINTER_NOTIFY_INIT;

typedef struct _SPLCLIENT_INFO_1{
    DWORD       dwSize;
    LPWSTR      pMachineName;
    LPWSTR      pUserName;
    DWORD       dwBuildNum;
    DWORD       dwMajorVersion;
    DWORD       dwMinorVersion;
    WORD        wProcessorArchitecture;
} SPLCLIENT_INFO_1, *PSPLCLIENT_INFO_1, *LPSPLCLIENT_INFO_1;

// This definition is used in the private spooler RPC interface (RpcSplOpenPrinter)
// The handle returned in the struct is the Server Side hPrinter which will used in
// making direct API calls from the client to the server side w/o the overhead of
// RPC. The performance boost is observed mainly in calls to Read/WritePrinter made from
// within the spooler (gdi32.dll during playback)
//
//
typedef struct _SPLCLIENT_INFO_2_V1{

    ULONG_PTR       hSplPrinter;            // Server side handle to be used for direct calls
} SPLCLIENT_INFO_2_W2K;

typedef struct _SPLCLIENT_INFO_2_V2{

#ifdef _WIN64
    DWORD64 hSplPrinter;      // Server side handle to be used for direct calls
#else
    DWORD32 hSplPrinter;      // Server side handle to be used for direct calls
#endif

} SPLCLIENT_INFO_2_WINXP;

typedef struct _SPLCLIENT_INFO_2_V3{

    UINT64          hSplPrinter;            // Server side handle to be used for direct calls
} SPLCLIENT_INFO_2_LONGHORN;

#if (OSVER(NTDDI_VERSION) == NTDDI_W2K)
    typedef SPLCLIENT_INFO_2_W2K SPLCLIENT_INFO_2, *PSPLCLIENT_INFO_2, *LPSPLCLIENT_INFO_2;
#elif ((OSVER(NTDDI_VERSION) == NTDDI_WINXP) || (OSVER(NTDDI_VERSION) == NTDDI_WS03))
    typedef SPLCLIENT_INFO_2_WINXP SPLCLIENT_INFO_2, *PSPLCLIENT_INFO_2, *LPSPLCLIENT_INFO_2;
#else
    typedef SPLCLIENT_INFO_2_LONGHORN SPLCLIENT_INFO_2, *PSPLCLIENT_INFO_2, *LPSPLCLIENT_INFO_2;
#endif


#if (NTDDI_VERSION >= NTDDI_LONGHORN)
    //
    // This structure is a super set of the information in both a
    // splclient_info_1 and splclient_info2 it also contains addtional
    // information needed by the provider.
    //
    typedef struct _SPLCLIENT_INFO_3
    {
        UINT            cbSize;                 // Size in bytes of this structure
        DWORD           dwFlags;                // Open printer additional flags to the provider
        DWORD           dwSize;                 // Reserved, here for complitbility with a info 1 structure
        PWSTR           pMachineName;           // Client machine name
        PWSTR           pUserName;              // Client user name
        DWORD           dwBuildNum;             // Cleint build number
        DWORD           dwMajorVersion;         // Client machine major version
        DWORD           dwMinorVersion;         // Cleint machine minor version
        WORD            wProcessorArchitecture; // Client machine architecture
        UINT64          hSplPrinter;            // Server side handle to be used for direct calls
    } SPLCLIENT_INFO_3, *PSPLCLIENT_INFO_3, *LPSPLCLIENT_INFO_3;
#endif // (NTDDI_VERSION >= NTDDI_LONGHORN)


typedef struct _PRINTPROVIDOR
{

    BOOL (*fpOpenPrinter)(LPWSTR   pPrinterName,
                          LPHANDLE phPrinter,
                          LPPRINTER_DEFAULTS pDefault);

    BOOL (*fpSetJob)(HANDLE hPrinter,
                     DWORD JobId,
                     DWORD Level,
                     LPBYTE pJob,
                     DWORD Command);

    BOOL (*fpGetJob)(HANDLE   hPrinter,
                     DWORD    JobId,
                     DWORD    Level,
                     LPBYTE   pJob,
                     DWORD    cbBuf,
                     LPDWORD  pcbNeeded);

    BOOL (*fpEnumJobs)(HANDLE  hPrinter,
                       DWORD   FirstJob,
                       DWORD   NoJobs,
                       DWORD   Level,
                       LPBYTE  pJob,
                       DWORD   cbBuf,
                       LPDWORD pcbNeeded,
                       LPDWORD pcReturned);

    HANDLE (*fpAddPrinter)(LPWSTR  pName,
                           DWORD   Level,
                           LPBYTE  pPrinter);

    BOOL (*fpDeletePrinter)(HANDLE   hPrinter);

    BOOL (*fpSetPrinter)(HANDLE  hPrinter,
                         DWORD   Level,
                         LPBYTE  pPrinter,
                         DWORD   Command);

    BOOL (*fpGetPrinter)(HANDLE  hPrinter,
                         DWORD   Level,
                         LPBYTE  pPrinter,
                         DWORD   cbBuf,
                         LPDWORD pcbNeeded);

    BOOL (*fpEnumPrinters)(DWORD   Flags,
                           LPWSTR  Name,
                           DWORD   Level,
                           LPBYTE  pPrinterEnum,
                           DWORD   cbBuf,
                           LPDWORD pcbNeeded,
                           LPDWORD pcReturned);

    BOOL (*fpAddPrinterDriver)(LPWSTR  pName,
                               DWORD   Level,
                               LPBYTE  pDriverInfo);

    BOOL (*fpEnumPrinterDrivers)(LPWSTR  pName,
                                 LPWSTR  pEnvironment,
                                 DWORD   Level,
                                 LPBYTE  pDriverInfo,
                                 DWORD   cbBuf,
                                 LPDWORD pcbNeeded,
                                 LPDWORD pcReturned);

    BOOL (*fpGetPrinterDriver)(HANDLE  hPrinter,
                               LPWSTR  pEnvironment,
                               DWORD   Level,
                               LPBYTE  pDriverInfo,
                               DWORD   cbBuf,
                               LPDWORD pcbNeeded);

    BOOL (*fpGetPrinterDriverDirectory)(LPWSTR  pName,
                                        LPWSTR  pEnvironment,
                                        DWORD   Level,
                                        LPBYTE  pDriverDirectory,
                                        DWORD   cbBuf,
                                        LPDWORD pcbNeeded);

    BOOL (*fpDeletePrinterDriver)(LPWSTR   pName,
                                  LPWSTR   pEnvironment,
                                  LPWSTR   pDriverName);

    BOOL (*fpAddPrintProcessor)(LPWSTR  pName,
                                LPWSTR  pEnvironment,
                                LPWSTR  pPathName,
                                LPWSTR  pPrintProcessorName);

    BOOL (*fpEnumPrintProcessors)(LPWSTR  pName,
                                  LPWSTR  pEnvironment,
                                  DWORD   Level,
                                  LPBYTE  pPrintProcessorInfo,
                                  DWORD   cbBuf,
                                  LPDWORD pcbNeeded,
                                  LPDWORD pcReturned);

    BOOL (*fpGetPrintProcessorDirectory)(LPWSTR  pName,
                                         LPWSTR  pEnvironment,
                                         DWORD   Level,
                                         LPBYTE  pPrintProcessorInfo,
                                         DWORD   cbBuf,
                                         LPDWORD pcbNeeded);

    BOOL (*fpDeletePrintProcessor)(LPWSTR  pName,
                                   LPWSTR  pEnvironment,
                                   LPWSTR  pPrintProcessorName);

    BOOL (*fpEnumPrintProcessorDatatypes)(LPWSTR  pName,
                                          LPWSTR  pPrintProcessorName,
                                          DWORD   Level,
                                          LPBYTE  pDataypes,
                                          DWORD   cbBuf,
                                          LPDWORD pcbNeeded,
                                          LPDWORD pcReturned);

    DWORD (*fpStartDocPrinter)(HANDLE  hPrinter,
                               DWORD   Level,
                               LPBYTE  pDocInfo);

    BOOL (*fpStartPagePrinter)(HANDLE  hPrinter);

    BOOL (*fpWritePrinter)(HANDLE  hPrinter,
                           LPVOID  pBuf,
                           DWORD   cbBuf,
                           LPDWORD pcWritten);

    BOOL (*fpEndPagePrinter)(HANDLE   hPrinter);

    BOOL (*fpAbortPrinter)(HANDLE   hPrinter);

    BOOL (*fpReadPrinter)(HANDLE  hPrinter,
                          LPVOID  pBuf,
                          DWORD   cbBuf,
                          LPDWORD pNoBytesRead);

    BOOL (*fpEndDocPrinter)(HANDLE   hPrinter);

    BOOL (*fpAddJob)(HANDLE  hPrinter,
                     DWORD   Level,
                     LPBYTE  pData,
                     DWORD   cbBuf,
                     LPDWORD pcbNeeded);

    BOOL (*fpScheduleJob)(HANDLE  hPrinter,
                          DWORD   JobId);

    DWORD (*fpGetPrinterData)(HANDLE   hPrinter,
                              LPWSTR   pValueName,
                              LPDWORD  pType,
                              LPBYTE   pData,
                              DWORD    nSize,
                              LPDWORD  pcbNeeded);

    DWORD (*fpSetPrinterData)(HANDLE  hPrinter,
                              LPWSTR  pValueName,
                              DWORD   Type,
                              LPBYTE  pData,
                              DWORD   cbData);

    DWORD (*fpWaitForPrinterChange)(HANDLE hPrinter, DWORD Flags);

    BOOL (*fpClosePrinter)(HANDLE hPrinter);

    BOOL (*fpAddForm)(HANDLE  hPrinter,
                      DWORD   Level,
                      LPBYTE  pForm);

    BOOL (*fpDeleteForm)(HANDLE  hPrinter,
                         LPWSTR  pFormName);

    BOOL (*fpGetForm)(HANDLE  hPrinter,
                      LPWSTR  pFormName,
                      DWORD   Level,
                      LPBYTE  pForm,
                      DWORD   cbBuf,
                      LPDWORD pcbNeeded);

    BOOL (*fpSetForm)(HANDLE  hPrinter,
                      LPWSTR  pFormName,
                      DWORD   Level,
                      LPBYTE  pForm);

    BOOL (*fpEnumForms)(HANDLE  hPrinter,
                        DWORD   Level,
                        LPBYTE  pForm,
                        DWORD   cbBuf,
                        LPDWORD pcbNeeded,
                        LPDWORD pcReturned);

    BOOL (*fpEnumMonitors)(LPWSTR  pName,
                           DWORD   Level,
                           LPBYTE  pMonitors,
                           DWORD   cbBuf,
                           LPDWORD pcbNeeded,
                           LPDWORD pcReturned);

    BOOL (*fpEnumPorts)(LPWSTR  pName,
                        DWORD   Level,
                        LPBYTE  pPorts,
                        DWORD   cbBuf,
                        LPDWORD pcbNeeded,
                        LPDWORD pcReturned);

    BOOL (*fpAddPort)(LPWSTR  pName,
                      HWND    hWnd,
                      LPWSTR  pMonitorName);

    BOOL (*fpConfigurePort)(LPWSTR  pName,
                            HWND    hWnd,
                            LPWSTR  pPortName);

    BOOL (*fpDeletePort)(LPWSTR  pName,
                         HWND    hWnd,
                         LPWSTR  pPortName);

    HANDLE (*fpCreatePrinterIC)(HANDLE      hPrinter,
                                LPDEVMODEW  pDevMode);

    BOOL (*fpPlayGdiScriptOnPrinterIC)(HANDLE  hPrinterIC,
                                       LPBYTE  pIn,
                                       DWORD   cIn,
                                       LPBYTE  pOut,
                                       DWORD   cOut,
                                       DWORD   ul);

    BOOL (*fpDeletePrinterIC)(HANDLE  hPrinterIC);

    BOOL (*fpAddPrinterConnection)(LPWSTR  pName);

    BOOL (*fpDeletePrinterConnection)(LPWSTR pName);

    DWORD (*fpPrinterMessageBox)(HANDLE  hPrinter,
                                 DWORD   Error,
                                 HWND    hWnd,
                                 LPWSTR  pText,
                                 LPWSTR  pCaption,
                                 DWORD   dwType);

    BOOL (*fpAddMonitor)(LPWSTR  pName,
                         DWORD   Level,
                         LPBYTE  pMonitorInfo);

    BOOL (*fpDeleteMonitor)(LPWSTR  pName,
                            LPWSTR  pEnvironment,
                            LPWSTR  pMonitorName);

    BOOL (*fpResetPrinter)(HANDLE hPrinter,
                           LPPRINTER_DEFAULTS pDefault);

    BOOL (*fpGetPrinterDriverEx)(HANDLE  hPrinter,
                                 LPWSTR  pEnvironment,
                                 DWORD   Level,
                                 LPBYTE  pDriverInfo,
                                 DWORD   cbBuf,
                                 LPDWORD pcbNeeded,
                                 DWORD   dwClientMajorVersion,
                                 DWORD   dwClientMinorVersion,
                                 PDWORD  pdwServerMajorVersion,
                                 PDWORD  pdwServerMinorVersion);

    BOOL (*fpFindFirstPrinterChangeNotification)(HANDLE hPrinter,
                                                 DWORD  fdwFlags,
                                                 DWORD  fdwOptions,
                                                 HANDLE hNotify,
                                                 PDWORD pfdwStatus,
                                                 PVOID  pPrinterNotifyOptions,
                                                 PVOID  pPrinterNotifyInit);

    BOOL (*fpFindClosePrinterChangeNotification)(HANDLE hPrinter);

    BOOL (*fpAddPortEx)(LPWSTR   pName,
                        DWORD    Level,
                        LPBYTE   lpBuffer,
                        LPWSTR   lpMonitorName);

    BOOL (*fpShutDown)(LPVOID pvReserved);

    BOOL (*fpRefreshPrinterChangeNotification)(HANDLE   hPrinter,
                                               DWORD    Reserved,
                                               PVOID    pvReserved,
                                               PVOID    pPrinterNotifyInfo);

    BOOL (*fpOpenPrinterEx)(LPWSTR              pPrinterName,
                            LPHANDLE            phPrinter,
                            LPPRINTER_DEFAULTS  pDefault,
                            LPBYTE              pClientInfo,
                            DWORD               Level);

    HANDLE (*fpAddPrinterEx)(LPWSTR  pName,
                             DWORD   Level,
                             LPBYTE  pPrinter,
                             LPBYTE  pClientInfo,
                             DWORD   ClientInfoLevel);

    BOOL (*fpSetPort)(LPWSTR     pName,
                      LPWSTR     pPortName,
                      DWORD      Level,
                      LPBYTE     pPortInfo);

    DWORD (*fpEnumPrinterData)(HANDLE   hPrinter,
                               DWORD    dwIndex,
                               LPWSTR   pValueName,
                               DWORD    cbValueName,
                               LPDWORD  pcbValueName,
                               LPDWORD  pType,
                               LPBYTE   pData,
                               DWORD    cbData,
                               LPDWORD  pcbData);

    DWORD (*fpDeletePrinterData)(HANDLE   hPrinter,
                                 LPWSTR   pValueName);

    DWORD (*fpClusterSplOpen)(LPCTSTR pszServer,
                              LPCTSTR pszResource,
                              PHANDLE phSpooler,
                              LPCTSTR pszName,
                              LPCTSTR pszAddress);


    DWORD (*fpClusterSplClose)(HANDLE hSpooler);

    DWORD (*fpClusterSplIsAlive)(HANDLE hSpooler);

    DWORD (*fpSetPrinterDataEx)(HANDLE  hPrinter,
                                LPCWSTR pKeyName,
                                LPCWSTR pValueName,
                                DWORD   Type,
                                LPBYTE  pData,
                                DWORD   cbData);

    DWORD (*fpGetPrinterDataEx)(HANDLE   hPrinter,
                                LPCWSTR  pKeyName,
                                LPCWSTR  pValueName,
                                LPDWORD  pType,
                                LPBYTE   pData,
                                DWORD    nSize,
                                LPDWORD  pcbNeeded);

    DWORD (*fpEnumPrinterDataEx)(HANDLE  hPrinter,
                                 LPCWSTR pKeyName,
                                 LPBYTE  pEnumValues,
                                 DWORD   cbEnumValues,
                                 LPDWORD pcbEnumValues,
                                 LPDWORD pnEnumValues);

    DWORD (*fpEnumPrinterKey)(HANDLE   hPrinter,
                              LPCWSTR  pKeyName,
                              LPWSTR   pSubkey,
                              DWORD    cbSubkey,
                              LPDWORD  pcbSubkey);

    DWORD (*fpDeletePrinterDataEx)(HANDLE  hPrinter,
                                   LPCWSTR pKeyName,
                                   LPCWSTR pValueName);

    DWORD (*fpDeletePrinterKey)(HANDLE  hPrinter,
                                LPCWSTR pKeyName);

    BOOL (*fpSeekPrinter)(HANDLE hPrinter,
                          LARGE_INTEGER liDistanceToMove,
                          PLARGE_INTEGER pliNewPointer,
                          DWORD dwMoveMethod,
                          BOOL bWrite);

    BOOL (*fpDeletePrinterDriverEx)(LPWSTR   pName,
                                    LPWSTR   pEnvironment,
                                    LPWSTR   pDriverName,
                                    DWORD    dwDeleteFlag,
                                    DWORD    dwVersionNum);

    BOOL (*fpAddPerMachineConnection)(LPCWSTR    pServer,
                                      LPCWSTR    pPrinterName,
                                      LPCWSTR    pPrintServer,
                                      LPCWSTR    pProvider);

    BOOL (*fpDeletePerMachineConnection)(LPCWSTR   pServer,
                                         LPCWSTR   pPrinterName);

    BOOL (*fpEnumPerMachineConnections)(LPCWSTR    pServer,
                                        LPBYTE     pPrinterEnum,
                                        DWORD      cbBuf,
                                        LPDWORD    pcbNeeded,
                                        LPDWORD    pcReturned);

    BOOL (*fpXcvData)(HANDLE  hXcv,
                      LPCWSTR pszDataName,
                      PBYTE   pInputData,
                      DWORD   cbInputData,
                      PBYTE   pOutputData,
                      DWORD   cbOutputData,
                      PDWORD  pcbOutputNeeded,
                      PDWORD  pdwStatus);

    BOOL (*fpAddPrinterDriverEx)(LPWSTR  pName,
                                 DWORD   Level,
                                 LPBYTE  pDriverInfo,
                                 DWORD   dwFileCopyFlags);

    BOOL (*fpSplReadPrinter)(HANDLE hPrinter,
                             LPBYTE *pBuf,
                             DWORD  cbBuf);

    BOOL (*fpDriverUnloadComplete)(LPWSTR  pDriverFile);

    BOOL (*fpGetSpoolFileInfo)(HANDLE    hPrinter,
                               LPWSTR    *pSpoolDir,
                               LPHANDLE  phFile,
                               HANDLE    hSpoolerProcess,
                               HANDLE    hAppProcess);

    BOOL (*fpCommitSpoolData)(HANDLE  hPrinter,
                              DWORD   cbCommit);

    BOOL (*fpCloseSpoolFileHandle)(HANDLE  hPrinter);

    BOOL (*fpFlushPrinter)(HANDLE  hPrinter,
                           LPBYTE  pBuf,
                           DWORD   cbBuf,
                           LPDWORD pcWritten,
                           DWORD   cSleep);

   #if (NTDDI_VERSION >= NTDDI_WINXP)
        DWORD (*fpSendRecvBidiData)(HANDLE                    hPrinter,
                                    LPCWSTR                   pAction,
                                    PBIDI_REQUEST_CONTAINER   pReqData,
                                    PBIDI_RESPONSE_CONTAINER* ppResData);
   #endif // (NTDDI_VERSION >= NTDDI_WINXP)

   #if (NTDDI_VERSION >= NTDDI_LONGHORN)
        BOOL (*fpAddPrinterConnection2)(LPCWSTR  pName,
                                        DWORD    dwLevel,
                                        PVOID    pInfo);

        HRESULT (*fpGetPrintClassObject)(PCWSTR,
                                         #ifdef __cplusplus
                                             const IID&,
                                         #else
                                             const IID*,
                                         #endif

                                         VOID**);

        HRESULT (*fpReportJobProcessingProgress)(HANDLE                      hPrinter,
                                                 ULONG                       jobId,
                                                 EPrintXPSJobOperation       jobOperation,
                                                 EPrintXPSJobProgress        jobProgress);

        VOID (*fpEnumAndLogProvidorObjects)(DWORD   dwLevel,
                                            VOID    *pfOut);

        HRESULT (*fpInternalGetPrinterDriver)(HANDLE  hPrinter,
                                              LPWSTR  pEnvironment,
                                              DWORD   Level,
                                              LPBYTE  pDriverInfo,
                                              DWORD   cbBuf,
                                              LPDWORD pcbNeeded,
                                              DWORD   dwClientMajorVersion,
                                              DWORD   dwClientMinorVersion,
                                              PDWORD  pdwServerMajorVersion,
                                              PDWORD  pdwServerMinorVersion);

        HRESULT (*fpFindCompatibleDriver)(LPCWSTR     pcszPnpId,
                                          LPCWSTR     pcszPortName,
                                          LPWSTR      pszManufacturerName,
                                          DWORD       cchManufacturerName,
                                          LPDWORD     pcchRequiredManufacturerNameSize,
                                          LPWSTR      pszModelName,
                                          DWORD       cchModelName,
                                          LPDWORD     pcchRequiredModelNameSize,
                                          LPDWORD     pdwRank0Matches);

    #endif // (NTDDI_VERSION >= NTDDI_LONGHORN)

} PRINTPROVIDOR, *LPPRINTPROVIDOR;

BOOL
InitializePrintProvidor(
   __out_bcount(cbPrintProvidor) LPPRINTPROVIDOR  pPrintProvidor,
   __in DWORD    cbPrintProvidor,
   __in_opt LPWSTR   pFullRegistryPath
);

typedef struct _PRINTPROCESSOROPENDATA {
    PDEVMODE  pDevMode;
    LPWSTR    pDatatype;
    LPWSTR    pParameters;
    LPWSTR    pDocumentName;
    DWORD     JobId;
    LPWSTR    pOutputFile;
    LPWSTR    pPrinterName;
} PRINTPROCESSOROPENDATA, *PPRINTPROCESSOROPENDATA, *LPPRINTPROCESSOROPENDATA;

HANDLE
OpenPrintProcessor(
    __in LPWSTR  pPrinterName,
    __in PPRINTPROCESSOROPENDATA pPrintProcessorOpenData
);

BOOL
PrintDocumentOnPrintProcessor(
    __in HANDLE  hPrintProcessor,
    __in LPWSTR  pDocumentName
);

BOOL
ClosePrintProcessor(
    __inout HANDLE  hPrintProcessor
);

BOOL
ControlPrintProcessor(
    __in HANDLE  hPrintProcessor,
    __in DWORD   Command
);

DWORD
GetPrintProcessorCapabilities(
    __in LPTSTR   pValueName,
    __in DWORD    dwAttributes,
    __out_bcount(nSize) LPBYTE   pData,
    __in DWORD    nSize,
    __out LPDWORD  pcbNeeded
);

#if (NTDDI_VERSION >= NTDDI_WS03)
    BOOL
    GetJobAttributes(
        __in  LPWSTR            pPrinterName,
        __in  LPDEVMODEW        pDevmode,
        __out PATTRIBUTE_INFO_3 pAttributeInfo
        );
#endif // (NTDDI_VERSION >= NTDDI_WS03)


#if (NTDDI_VERSION >= NTDDI_LONGHORN)
    BOOL
    GetJobAttributesEx(
        __in LPWSTR            pPrinterName,
        __in LPDEVMODEW        pDevmode,
        __in DWORD             dwLevel,
        __out_bcount(nSize) LPBYTE            pAttributeInfo,
        __in DWORD             nSize,
        __in DWORD             dwFlags
        );


    // Flags for dwFlags
    #define FILL_WITH_DEFAULTS 0x1

#endif // (NTDDI_VERSION >= NTDDI_WS03)


BOOL
InitializeMonitor(
    LPWSTR  pRegistryRoot
);

BOOL
OpenPort(
    __in    LPWSTR  pName,
    __out   PHANDLE pHandle
);

BOOL
WritePort(
    __in                HANDLE  hPort,
    __in_bcount(cbBuf)  LPBYTE  pBuffer,
                        DWORD   cbBuf,
    __out               LPDWORD pcbWritten
);

BOOL
ReadPort(
    __in                    HANDLE hPort,
    __out_bcount(cbBuffer)  LPBYTE pBuffer,
                            DWORD  cbBuffer,
    __out                   LPDWORD pcbRead
);

BOOL
ClosePort(
    __in    HANDLE  hPort
);


BOOL
XcvOpenPort(
    __in    LPCWSTR     pszObject,
            ACCESS_MASK GrantedAccess,
    __out   PHANDLE     phXcv
);

DWORD
XcvDataPort(
    __in                        HANDLE  hXcv,
    __in                        LPCWSTR pszDataName,
    __in_bcount(cbInputData)    PBYTE   pInputData,
                                DWORD   cbInputData,
    __out_bcount(cbOutputData)  PBYTE   pOutputData,
                                DWORD   cbOutputData,
    __out                       PDWORD  pcbOutputNeeded
);

BOOL
XcvClosePort(
    __in    HANDLE  hXcv
);


BOOL
AddPortUI
(
            __in PCWSTR  pszServer,
    __in    HWND    hWnd,
            __in PCWSTR  pszMonitorNameIn,
    __out   PWSTR  *ppszPortNameOut
);

BOOL
ConfigurePortUI
(
            __in PCWSTR pszServer,
    __in    HWND   hWnd,
            __in PCWSTR pszPortName
);

BOOL
DeletePortUI
(
            __in PCWSTR pszServer,
    __in    HWND   hWnd,
            __in PCWSTR pszPortName
);


BOOL
SplDeleteSpoolerPortStart(
    PCWSTR pPortName
);

BOOL
SplDeleteSpoolerPortEnd(
    PCWSTR pName,
    BOOL   bDeletePort
);

#if (STRICT && (NTDDI_VERSION >= NTDDI_LONGHORN))
    #define HKEYMONITOR HKEY
#else
    #define HKEYMONITOR HANDLE
#endif

typedef struct _MONITORREG {

    DWORD cbSize;

    LONG
    (WINAPI *fpCreateKey)(
        HKEYMONITOR hcKey,
        LPCTSTR pszSubKey,
        DWORD dwOptions,
        REGSAM samDesired,
        PSECURITY_ATTRIBUTES pSecurityAttributes,
        HKEYMONITOR *phckResult,
        PDWORD pdwDisposition,
        HANDLE hSpooler
        );

    LONG
    (WINAPI *fpOpenKey)(
        HKEYMONITOR hcKey,
        LPCTSTR pszSubKey,
        REGSAM samDesired,
        HKEYMONITOR *phkResult,
        HANDLE hSpooler
        );

    LONG
    (WINAPI *fpCloseKey)(
        HKEYMONITOR hcKey,
        HANDLE hSpooler
        );

    LONG
    (WINAPI *fpDeleteKey)(
        HKEYMONITOR hcKey,
        LPCTSTR pszSubKey,
        HANDLE hSpooler
        );

    LONG
    (WINAPI *fpEnumKey)(
        HKEYMONITOR hcKey,
        DWORD dwIndex,
        LPTSTR pszName,
        PDWORD pcchName,
        PFILETIME pftLastWriteTime,
        HANDLE hSpooler
        );

    LONG
    (WINAPI *fpQueryInfoKey)(
        HKEYMONITOR hcKey,
        PDWORD pcSubKeys,
        PDWORD pcbKey,
        PDWORD pcValues,
        PDWORD pcbValue,
        PDWORD pcbData,
        PDWORD pcbSecurityDescriptor,
        PFILETIME pftLastWriteTime,
        HANDLE hSpooler
        );

    LONG
    (WINAPI *fpSetValue)(
        HKEYMONITOR hcKey,
        LPCTSTR pszValue,
        DWORD dwType,
        const BYTE* pData,
        DWORD cbData,
        HANDLE hSpooler
        );

    LONG
    (WINAPI *fpDeleteValue)(
        HKEYMONITOR hcKey,
        LPCTSTR pszValue,
        HANDLE hSpooler
        );

    LONG
    (WINAPI *fpEnumValue)(
        HKEYMONITOR hcKey,
        DWORD dwIndex,
        LPTSTR pszValue,
        PDWORD pcbValue,
        PDWORD pType,
        PBYTE pData,
        PDWORD pcbData,
        HANDLE hSpooler
        );

    LONG
    (WINAPI *fpQueryValue)(
        HKEYMONITOR hcKey,
        LPCTSTR pszValue,
        PDWORD pType,
        PBYTE pData,
        PDWORD pcbData,
        HANDLE hSpooler
        );

} MONITORREG, *PMONITORREG;


typedef struct _MONITORINIT {
    DWORD cbSize;
    HANDLE hSpooler;
    HKEYMONITOR hckRegistryRoot;
    PMONITORREG pMonitorReg;
    BOOL bLocal;
    LPCWSTR pszServerName;
} MONITORINIT, *PMONITORINIT;


typedef struct _MONITOR
{
    BOOL (WINAPI *pfnEnumPorts)
    (
    LPWSTR   pName,
    DWORD   Level,
    LPBYTE  pPorts,
    DWORD   cbBuf,
    LPDWORD pcbNeeded,
    LPDWORD pcReturned
    );

    BOOL (WINAPI *pfnOpenPort)
    (
    LPWSTR  pName,
    PHANDLE pHandle
    );

    BOOL (WINAPI *pfnOpenPortEx)
    (
    LPWSTR  pPortName,
    LPWSTR  pPrinterName,
    PHANDLE pHandle,
    struct _MONITOR FAR *pMonitor
    );


    BOOL (WINAPI *pfnStartDocPort)
    (
    HANDLE  hPort,
    LPWSTR  pPrinterName,
    DWORD   JobId,
    DWORD   Level,
    LPBYTE  pDocInfo
    );

    BOOL (WINAPI *pfnWritePort)
    (
    HANDLE  hPort,
    LPBYTE  pBuffer,
    DWORD   cbBuf,
    LPDWORD pcbWritten
    );

    BOOL (WINAPI *pfnReadPort)
    (
    HANDLE hPort,
    LPBYTE pBuffer,
    DWORD  cbBuffer,
    LPDWORD pcbRead
    );

    BOOL (WINAPI *pfnEndDocPort)
    (
    HANDLE   hPort
    );

    BOOL (WINAPI *pfnClosePort)
    (
        HANDLE  hPort
    );

    BOOL (WINAPI *pfnAddPort)
    (
    LPWSTR   pName,
    HWND    hWnd,
    LPWSTR   pMonitorName
    );

    BOOL (WINAPI *pfnAddPortEx)
    (
    LPWSTR   pName,
    DWORD    Level,
    LPBYTE   lpBuffer,
    LPWSTR   lpMonitorName
    );

    BOOL (WINAPI *pfnConfigurePort)
    (
    LPWSTR   pName,
    HWND  hWnd,
    LPWSTR pPortName
    );

    BOOL (WINAPI *pfnDeletePort)
    (
    LPWSTR  pName,
    HWND    hWnd,
    LPWSTR  pPortName
    );

    BOOL (WINAPI *pfnGetPrinterDataFromPort)
    (
    HANDLE  hPort,
    DWORD   ControlID,
    LPWSTR  pValueName,
    LPWSTR  lpInBuffer,
    DWORD   cbInBuffer,
    LPWSTR  lpOutBuffer,
    DWORD   cbOutBuffer,
    LPDWORD lpcbReturned
    );

    BOOL (WINAPI *pfnSetPortTimeOuts)
    (
    HANDLE  hPort,
    LPCOMMTIMEOUTS lpCTO,
    DWORD   reserved    // must be set to 0
    );

    BOOL (WINAPI *pfnXcvOpenPort)
    (
    LPCWSTR pszObject,
    ACCESS_MASK GrantedAccess,
    PHANDLE phXcv
    );

    DWORD (WINAPI *pfnXcvDataPort)
    (
    HANDLE  hXcv,
    LPCWSTR pszDataName,
    PBYTE   pInputData,
    DWORD   cbInputData,
    PBYTE   pOutputData,
    DWORD   cbOutputData,
    PDWORD  pcbOutputNeeded
    );

    BOOL (WINAPI *pfnXcvClosePort)
    (
    HANDLE  hXcv
    );

} MONITOR, FAR *LPMONITOR;

typedef struct _MONITOREX
{
    DWORD       dwMonitorSize;
    MONITOR     Monitor;

} MONITOREX, FAR *LPMONITOREX;


typedef struct _MONITOR2
{
    DWORD cbSize;
    BOOL (WINAPI *pfnEnumPorts)
    (
    HANDLE  hMonitor,
    LPWSTR  pName,
    DWORD   Level,
    LPBYTE  pPorts,
    DWORD   cbBuf,
    LPDWORD pcbNeeded,
    LPDWORD pcReturned
    );

    BOOL (WINAPI *pfnOpenPort)
    (
    HANDLE  hMonitor,
    LPWSTR  pName,
    PHANDLE pHandle
    );

    BOOL (WINAPI *pfnOpenPortEx)
    (
    HANDLE  hMonitor,
    HANDLE  hMonitorPort,
    LPWSTR  pPortName,
    LPWSTR  pPrinterName,
    PHANDLE pHandle,
    struct _MONITOR2 FAR *pMonitor2
    );


    BOOL (WINAPI *pfnStartDocPort)
    (
    HANDLE  hPort,
    LPWSTR  pPrinterName,
    DWORD   JobId,
    DWORD   Level,
    LPBYTE  pDocInfo
    );

    BOOL (WINAPI *pfnWritePort)
    (
    HANDLE  hPort,
    LPBYTE  pBuffer,
    DWORD   cbBuf,
    LPDWORD pcbWritten
    );

    BOOL (WINAPI *pfnReadPort)
    (
    HANDLE hPort,
    LPBYTE pBuffer,
    DWORD  cbBuffer,
    LPDWORD pcbRead
    );

    BOOL (WINAPI *pfnEndDocPort)
    (
    HANDLE   hPort
    );

    BOOL (WINAPI *pfnClosePort)
    (
    HANDLE  hPort
    );

    BOOL (WINAPI *pfnAddPort)
    (
    HANDLE   hMonitor,
    LPWSTR   pName,
    HWND     hWnd,
    LPWSTR   pMonitorName
    );

    BOOL (WINAPI *pfnAddPortEx)
    (
    HANDLE   hMonitor,
    LPWSTR   pName,
    DWORD    Level,
    LPBYTE   lpBuffer,
    LPWSTR   lpMonitorName
    );

    BOOL (WINAPI *pfnConfigurePort)
    (
    HANDLE  hMonitor,
    LPWSTR  pName,
    HWND    hWnd,
    LPWSTR  pPortName
    );

    BOOL (WINAPI *pfnDeletePort)
    (
    HANDLE  hMonitor,
    LPWSTR  pName,
    HWND    hWnd,
    LPWSTR  pPortName
    );

    BOOL (WINAPI *pfnGetPrinterDataFromPort)
    (
    HANDLE  hPort,
    DWORD   ControlID,
    LPWSTR  pValueName,
    LPWSTR  lpInBuffer,
    DWORD   cbInBuffer,
    LPWSTR  lpOutBuffer,
    DWORD   cbOutBuffer,
    LPDWORD lpcbReturned
    );

    BOOL (WINAPI *pfnSetPortTimeOuts)
    (
    HANDLE  hPort,
    LPCOMMTIMEOUTS lpCTO,
    DWORD   reserved    // must be set to 0
    );

    BOOL (WINAPI *pfnXcvOpenPort)
    (
    HANDLE  hMonitor,
    LPCWSTR pszObject,
    ACCESS_MASK GrantedAccess,
    PHANDLE phXcv
    );

    DWORD (WINAPI *pfnXcvDataPort)
    (
    HANDLE  hXcv,
    LPCWSTR pszDataName,
    PBYTE   pInputData,
    DWORD   cbInputData,
    PBYTE   pOutputData,
    DWORD   cbOutputData,
    PDWORD  pcbOutputNeeded
    );

    BOOL (WINAPI *pfnXcvClosePort)
    (
    HANDLE  hXcv
    );

    VOID (WINAPI *pfnShutdown)
    (
    HANDLE hMonitor
    );

    #if (NTDDI_VERSION >= NTDDI_WINXP)
        DWORD (WINAPI *pfnSendRecvBidiDataFromPort)
        (
            HANDLE                    hPort,
            DWORD                     dwAccessBit,
            LPCWSTR                   pAction,
            PBIDI_REQUEST_CONTAINER   pReqData,
            PBIDI_RESPONSE_CONTAINER* ppResData
        );
    #endif

} MONITOR2, *PMONITOR2, FAR *LPMONITOR2;

#if (NTDDI_VERSION >= NTDDI_WINXP)
    #define MONITOR2_SIZE_WIN2K ( sizeof(DWORD) + (sizeof(PVOID)*18) )
#endif // (NTDDI_VERSION >= NTDDI_WINXP)

typedef struct _MONITORUI
{
    DWORD   dwMonitorUISize;

    BOOL (WINAPI *pfnAddPortUI)
    (
        PCWSTR pszServer,
        HWND   hWnd,
        PCWSTR pszMonitorNameIn,
        PWSTR  *ppszPortNameOut
    );

    BOOL (WINAPI *pfnConfigurePortUI)
    (
        PCWSTR  pName,
        HWND    hWnd,
        PCWSTR  pPortName
    );

    BOOL (WINAPI *pfnDeletePortUI)
    (
        PCWSTR pszServer,
        HWND   hWnd,
        PCWSTR pszPortName
    );

} MONITORUI, FAR *PMONITORUI;


HANDLE
CreatePrinterIC(
__in        HANDLE      hPrinter,
__in_opt    LPDEVMODEW  pDevMode
);

BOOL
PlayGdiScriptOnPrinterIC(
__in                HANDLE  hPrinterIC,
__in_bcount(cIn)    LPBYTE  pIn,
                    DWORD   cIn,
__out_bcount(cOut)  LPBYTE  pOut,
                    DWORD   cOut,
                    DWORD   ul
);

BOOL
DeletePrinterIC(
__in    HANDLE  hPrinterIC
);

BOOL
DevQueryPrint(
__in    HANDLE      hPrinter,
__in    LPDEVMODE   pDevMode,
__out   DWORD*      pResID
);

HANDLE
RevertToPrinterSelf(
    VOID
);

BOOL
ImpersonatePrinterClient(
__in    HANDLE  hToken
);

BOOL
ReplyPrinterChangeNotification(
__in        HANDLE hPrinter,
            DWORD  fdwChangeFlags,
__out_opt   PDWORD pdwResult,
__in_opt    PVOID  pPrinterNotifyInfo
);

BOOL
ReplyPrinterChangeNotificationEx(
__in    HANDLE   hNotify,
        DWORD    dwColor,
        DWORD    fdwFlags,
__out   PDWORD   pdwResult,
__in    PVOID    pPrinterNotifyInfo
    );

BOOL
PartialReplyPrinterChangeNotification(
 __in            HANDLE  hPrinter,
__in_opt        PPRINTER_NOTIFY_INFO_DATA pDataSrc
    );

PPRINTER_NOTIFY_INFO
RouterAllocPrinterNotifyInfo(
    DWORD cPrinterNotifyInfoData
);

BOOL
RouterFreePrinterNotifyInfo(
__in_opt   PPRINTER_NOTIFY_INFO pInfo
);

#if (NTDDI_VERSION >= NTDDI_WINXP)
    PBIDI_RESPONSE_CONTAINER
    RouterAllocBidiResponseContainer(
        DWORD Count
    );

    PVOID
    RouterAllocBidiMem (
        IN size_t NumBytes
    );

    DWORD
    RouterFreeBidiResponseContainer(
        IN PBIDI_RESPONSE_CONTAINER pData
        );

    VOID
    RouterFreeBidiMem (
        IN PVOID pMemPointer
    );
#endif // (NTDDI_VERSION >= NTDDI_WINXP)

#define PRINTER_NOTIFY_INFO_DATA_COMPACT 1

BOOL
AppendPrinterNotifyInfoData(
__in            PPRINTER_NOTIFY_INFO      pInfoDest,
__in_opt        PPRINTER_NOTIFY_INFO_DATA pDataSrc,
                DWORD                     fdwFlags
);

#if (NTDDI_VERSION >= NTDDI_LONGHORN)
    typedef BOOL (CALLBACK *ROUTER_NOTIFY_CALLBACK)(
        IN      DWORD                   dwCommand,
        IN      PVOID                   pContext,
        IN      DWORD                   dwColor,
        IN      PPRINTER_NOTIFY_INFO    pNofityInfo,
        IN      DWORD                   fdwFlags,
            OUT PDWORD                  pdwResult
        );

    typedef enum _NOTIFICATION_CALLBACK_COMMANDS
    {
        NOTIFICATION_COMMAND_NOTIFY,
        NOTIFICATION_COMMAND_CONTEXT_ACQUIRE,
        NOTIFICATION_COMMAND_CONTEXT_RELEASE
    } NOTIFICATION_CALLBACK_COMMANDS;

    typedef struct _NOTIFICATION_CONFIG_1
    {
        UINT                    cbSize;
        DWORD                   fdwFlags;
        ROUTER_NOTIFY_CALLBACK  pfnNotifyCallback;
        PVOID                   pContext;
    } NOTIFICATION_CONFIG_1, *PNOTIFICATION_CONFIG_1;

    typedef enum _NOTIFICATION_CONFIG_FLAGS
    {
        NOTIFICATION_CONFIG_CREATE_EVENT      = 1 << 0,
        NOTIFICATION_CONFIG_REGISTER_CALLBACK = 1 << 1,
        NOTIFICATION_CONFIG_EVENT_TRIGGER     = 1 << 2,
        NOTIFICATION_CONFIG_ASYNC_CHANNEL     = 1 << 3
    } NOTIFICATION_CONFIG_FLAGS;
#endif // (NTDDI_VERSION >= NTDDI_LONGHORN)

DWORD
CallRouterFindFirstPrinterChangeNotification(
__in    HANDLE                  hPrinterRPC,
        DWORD                   fdwFilterFlags,
        DWORD                   fdwOptions,
__in    HANDLE                  hNotify,
__in    PPRINTER_NOTIFY_OPTIONS pPrinterNotifyOptions
    );

BOOL
ProvidorFindFirstPrinterChangeNotification(
__in        HANDLE                  hPrinter,
            DWORD                   fdwFlags,
            DWORD                   fdwOptions,
__in        HANDLE                  hNotify,
__in_opt    PVOID                   pPrinterNotifyOptions,
__out_opt   PVOID                   pvReserved1
    );

BOOL
ProvidorFindClosePrinterChangeNotification(
__in    HANDLE                  hPrinter
    );

BOOL
SpoolerFindFirstPrinterChangeNotification(
 __in       HANDLE                  hPrinter,
            DWORD                   fdwFilterFlags,
            DWORD                   fdwOptions,
__in        PVOID                   pPrinterNotifyOptions,
__in_opt    PVOID                   pvReserved,
__in        PVOID                   pNotificationConfig,
__out_opt   PHANDLE                 phNotify,
__out_opt   PHANDLE                 phEvent
    );

BOOL
SpoolerFindNextPrinterChangeNotification(
__in            HANDLE    hPrinter,
__out           LPDWORD   pfdwChange,
__in_opt        LPVOID    pPrinterNotifyOptions,
__inout_opt     LPVOID    *ppPrinterNotifyInfo
    );

#if (NTDDI_VERSION >= NTDDI_LONGHORN)
    BOOL
    SpoolerRefreshPrinterChangeNotification(
    __in            HANDLE                   hPrinter,
                    DWORD                    dwColor,
    __in            PPRINTER_NOTIFY_OPTIONS  pOptions,
    __inout_opt     PPRINTER_NOTIFY_INFO     *ppInfo
        );
#endif // (NTDDI_VERSION >= NTDDI_LONGHORN)

VOID
SpoolerFreePrinterNotifyInfo(
__in    PPRINTER_NOTIFY_INFO    pInfo
    );

BOOL
SpoolerFindClosePrinterChangeNotification(
__in    HANDLE                  hPrinter
    );

LPMONITOR2
WINAPI
InitializePrintMonitor2(
    __in    PMONITORINIT    pMonitorInit,
    __out   PHANDLE         phMonitor
);

BOOL
WINAPI
InitializeMonitorEx(
    __in    LPWSTR      pRegistryRoot,
    __out   LPMONITOR   pMonitor
);

LPMONITOREX
WINAPI
InitializePrintMonitor(
    __in    LPWSTR      pRegistryRoot
);

PMONITORUI
WINAPI
InitializePrintMonitorUI(
    VOID
);


//
//  The following is added for new point-and-print support which allows
//  specific files to be associated with a print queue (instead of a printer
//  driver) using SetPrinterDataEx under the key "CopyFiles"
//
#define COPYFILE_EVENT_SET_PRINTER_DATAEX           1
#define COPYFILE_EVENT_DELETE_PRINTER               2
#define COPYFILE_EVENT_ADD_PRINTER_CONNECTION       3
#define COPYFILE_EVENT_DELETE_PRINTER_CONNECTION    4
#define COPYFILE_EVENT_FILES_CHANGED                5


BOOL
WINAPI
SpoolerCopyFileEvent(
    __in LPWSTR  pszPrinterName,
    __in LPWSTR  pszKey,
    __in DWORD   dwCopyFileEvent
    );

#define COPYFILE_FLAG_CLIENT_SPOOLER            0x00000001
#define COPYFILE_FLAG_SERVER_SPOOLER            0x00000002


DWORD
WINAPI
GenerateCopyFilePaths(
    __in IN      LPCWSTR     pszPrinterName,
    __in IN      LPCWSTR     pszDirectory,
    __in IN      LPBYTE      pSplClientInfo,
    __in IN      DWORD       dwLevel,
    __inout_ecount(*pcchSourceDirSize) IN OUT  LPWSTR      pszSourceDir,
    __inout IN OUT  LPDWORD     pcchSourceDirSize,
    __inout_ecount(*pcchTargetDirSize) IN OUT  LPWSTR      pszTargetDir,
    __inout IN OUT  LPDWORD     pcchTargetDirSize,
    __in IN      DWORD       dwFlags
    );

#if (NTDDI_VERSION >= NTDDI_WINXP)
    typedef enum {
        kMessageBox = 0
    } UI_TYPE;

    typedef struct {
        DWORD       cbSize;     // sizeof(MESSAGEBOX_PARAMS)
        LPWSTR      pTitle;     // Pointer to a null-terminated string for the title bar of the message box.
        LPWSTR      pMessage;   // Pointer to a null-terminated string containing the message to display.
        DWORD       Style;      // Specifies the contents and behavior of the message box
        DWORD       dwTimeout;  // If bWait is TRUE, Timeout specifies the time, in seconds, that the function waits for the user's response.
        BOOL        bWait;      // If TRUE, SplPromptUIInUsersSession does not return until the user responds or the time-out interval elapses.
                                // If Timeout is zero, SplPromptUIInUsersSession doesn't return until the user responds.
                                // If FALSE, the function returns immediately and pResponse returns IDASYNC.

    } MESSAGEBOX_PARAMS, *PMESSAGEBOX_PARAMS;

    typedef struct {

        UI_TYPE  UIType;

        union {

            MESSAGEBOX_PARAMS   MessageBoxParams;
        };

    } SHOWUIPARAMS, *PSHOWUIPARAMS;

    BOOL
    SplPromptUIInUsersSession(
    __in                                HANDLE          hPrinter,
                                        DWORD           JobId,
    __in_bcount(sizeof(SHOWUIPARAMS))   PSHOWUIPARAMS   pUIParams,
    __out                               DWORD           *pResponse
    );

    DWORD
    SplIsSessionZero(
    __in    HANDLE  hPrinter,
            DWORD   JobId,
    __out   BOOL    *pIsSessionZero
    );
#endif // (NTDDI_VERSION >= NTDDI_WINXP)


#ifdef __cplusplus
}                   /* End of extern "C" { */
#endif              /* __cplusplus */

#endif      // _WINSPLP_



