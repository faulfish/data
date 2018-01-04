/* ---- WKTOOLS.H ------------------------------------------------------------
 * WIBU-KEY Tools/Setup ANSI/MS-C Header File.
 * (C) Copyright 1996-1999 by WIBU-SYSTEMS AG
 * D-76137 Karlsruhe, Federal Republic of Germany
 * Version 2.52 of 99Aug20
 * ---------------------------------------------------------------------------
 */

/* ============================================================
 * WIBU-KEY system types and macros for the WIBUKEY.CPL control
 * Compatible to Microsoft's Windows-API declarations
 * ============================================================
 */

#if !defined(_WKTOOLS_INCLUDED)
#define _WKTOOLS_INCLUDED

#include <wk.h>

#if defined(__cplusplus)
extern "C" {            /* WKTOOLS.H contains only C declarations */
#endif

/* flags for <ulFlags> of WktShowControlPanel() */
#define WKT_PAGE_CONTENTS   0x00000001
#define WKT_PAGE_TEST       0x00000002
#define WKT_PAGE_CONTEXT    0x00000004
#define WKT_PAGE_UPDATE     0x00000008
#define WKT_PAGE_SETTINGS   0x00000010
#define WKT_PAGE_INSTALL    0x00000020
#define WKT_PAGE_DIAGNOSIS  0x00000040
#define WKT_PAGE_INFO       0x00000080
#define WKT_PAGE_SERVER     0x00000100
#define WKT_PAGE_CLIENT     0x00000200
#define WKT_PAGE_ALL        0x80000000

/* flags for <ulFlags> of WktInstall() */
#define WKTI_NOREMOVE      0x000001
#define WKTI_WIN16         0x010000
#define WKTI_WIN32         0x020000
#define WKTI_DOS           0x040000
#define WKTI_NO_MESSAGES   0x080000


/* error codes, returned by WktGetLastError() */
#define WKT_NO_ERROR                      0
#define WKT_ERROR_ILLEGAL_CHARACTER      13
#define WKT_ERROR_PROFILE_WRITE          14
#define WKT_ERROR_PROFILE_READ           15
#define WKT_ERROR_NOT_SUPPORTED          16
#define WKT_ERROR_VALUE_NOT_FOUND        17
#define WKT_ERROR_RTCWRITE               21
#define WKT_ERROR_RTUREAD                22
#define WKT_ERROR_NOUSERRIGHT            30
#define WKT_ERROR_INTERNAL_ERROR        100
#define WKT_ERROR_REBOOT                104
#define WKT_ERROR_WRONG_SYSTEM          106
#define WKT_ERROR_INVALID_PARAMETER     107
#define WKT_ERROR_NOT_IMPLEMENTED       108
#define WKT_ERROR_ABORT                 109
#define WKT_ERROR_BUFFER_TOO_SMALL      110
#define WKT_ERROR_REG_READ              111
#define WKT_ERROR_REG_WRITE             112
#define WKT_ERROR_NOTEMPFOLDER          113
#define WKT_ERROR_FILEINUSE             200
#define WKT_ERROR_READONLY              201
#define WKT_ERROR_NOACCESS              202
#define WKT_ERROR_NORENAME              203
#define WKT_ERROR_NOCOPY                204
#define WKT_ERROR_NODELETE              205
#define WKT_ERROR_NOMOVE                206
#define WKT_ERROR_FILE_NOT_FOUND        208
#define WKT_ERROR_NO_ACCESS_DRIVER      209
#define WKT_ERROR_NOCREATE              210
#define WKT_ERROR_NOINSTALL             211
#define WKT_ERROR_NODEINSTALL           212
#define WKT_ERROR_FILE_CORRUPT          213
#define WKT_ERROR_WRONG_ARCHITECTURE    214
#define WKT_ERROR_OLDVER                230
#define WKT_ERROR_OLDLANGVER            231
#define WKT_ERROR_UNKNOWN               400


/*****************************************************************************
WKT_CTX...                        Flags for <ulFlags> of WktGetRemoteContext()
==============================================================================

WKT_CTX_PORT ..... pszPortSpec specifies a valid WIBU-KEY portname
// not yet implemented
WKT_CTX_FIRMCODE   pszPortSpec specifies a Firm Code
WKT_CTX_WKUSPEC .. pszPortSpec specifies a WIBU-BOX/Entry specification
                   like WKU

*****************************************************************************/

#define WKT_CTX_PORT      0x01
#define WKT_CTX_FIRMCODE  0x02
#define WKT_CTX_WKUSPEC   0x04


/*****************************************************************************
WKTI_...                                  Commands for <ulCmd> of WktInstall()
==============================================================================

WKTI_CMD_INSTALL .. install specific parts of WIBU-KEY
WKTI_CMD_DEINSTALL  deinstall specifid parts of WIBU-KEY
WKTI_CMD_UPDATE ... update specifid parts of WIBU-KEY

*****************************************************************************/

enum {
 WKTI_CMD_INSTALL = 0, WKTI_CMD_DEINSTALL, WKTI_CMD_UPDATE
 };

/* ===========================================
 * Entry functions in WIBUKEY.CPL/WIBUKE32.CPL
 * ===========================================
 */

BOOL WKAPIENTRY WktInstallA(const TCHAR *pszSource,
 ULONG ulCmd, ULONG ulFlags);
#if defined(_WIN32)
BOOL WKAPIENTRY WktInstallW(const TCHAR *pszSource,
  ULONG ulCmd, ULONG ulFlags);
#endif

BOOL WKAPIENTRY WktShowControlPanel(ULONG flFlags, INT iStartPage);

BOOL WKAPIENTRY WktSetProfileStringA(const TCHAR *pszPath,
 const TCHAR *pszValueName, const TCHAR *pszValue);
#if defined(_WIN32)
BOOL WKAPIENTRY WktSetProfileStringW(const TCHAR *pszPath,
 const TCHAR *pszValueName, const TCHAR *pszValue);
#endif

BOOL WKAPIENTRY WktGetProfileStringA(const TCHAR *pszPath,
 const TCHAR *pszValueName, TCHAR *pszBuf, UINT cchBuf);
#if defined(_WIN32)
BOOL WKAPIENTRY WktGetProfileStringW(const TCHAR *pszPath,
 const TCHAR *pszValueName, TCHAR *pszBuf, UINT cchBuf);
#endif

BOOL WKAPIENTRY WktGetRemoteContextA(const TCHAR *pszPortSpec, ULONG flCtrl,
 UINT fuEntries, BOOL fOnlyEmpty, BOOL fAppend, const TCHAR *pszRtcFile);
#if defined(_WIN32)
BOOL WKAPIENTRY WktGetRemoteContextW(const TCHAR *pszPortSpec, ULONG flCtrl,
 UINT fuEntries, BOOL fOnlyEmpty, BOOL fAppend, const TCHAR *pszRtcFile);
#endif

BOOL WKAPIENTRY WktSetRemoteUpdateA(const TCHAR *pszRtuFile, BOOL fDelete);
#if defined(_WIN32)
BOOL WKAPIENTRY WktSetRemoteUpdateW(const TCHAR *pszRtuFile, BOOL fDelete);
#endif

INT WKAPIENTRY WktGetLastError();

/* Replacement of standard names to Unicode and ASCII functions */

#if defined(_WIN32) && defined(_UNICODE)
  #define WktInstall WktInstallW
  #define WktSetProfileString WktSetProfileStringW
  #define WktGetProfileString WktGetProfileStringW
  #define WktGetRemoteContext WktGetRemoteContextW
  #define WktSetRemoteUpdate WktSetRemoteUpdateW
#else
  #define WktInstall WktInstallA
  #define WktSetProfileString WktSetProfileStringA
  #define WktGetProfileString WktGetProfileStringA
  #define WktGetRemoteContext WktGetRemoteContextA
  #define WktSetRemoteUpdate WktSetRemoteUpdateA
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* !defined(WKTOOLS_INCLUDED) */
