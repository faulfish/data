/* ---- wkspdi.h -------------------------------------------------------------
 * WIBU-KEY Development-Kit
 * Structured Parameters Driver Interface (SPDI) Declarations
 * ---------------------------------------------------------------------------
 * (C) Copyright 1989 - 2004 by WIBU-SYSTEMS AG
 * D-76137 Karlsruhe, Federal Republic of Germany
 * Version 4.01 of 2004Jan15
 * ---------------------------------------------------------------------------
 */

#if defined(_WKDOSC) && defined(_WKKRNLC)
  #error _WKDOSC and _WKKRNLC defined at same time
#endif

#if !defined(_WKDOSC) && !defined(_WKKRNLC)
  #error Neither _WKDOSC nor _WKKRNLC defined
#endif

// ===================
// SPDI function codes
// ===================
// The virtual WKDOS calling interface defines:
// * WKD_FNCODE as enumeration type
// * WKD_xxx as constants via the WKSPDIF(xxx) macro
// 
// The kernel driver calling interface defines:
// * WKKRNLC_FNCODE as enumeration type
// * WKKRNLF_xxx as constants via the WKSPDIF(xxx) macro

#if defined(_WKDOSC)
  #define WKSPDI1(id) WKD_##id
  #define WKSPDIF(id, val) WKD_##id = val
#else
  // _WKKRNLC defined
  #define WKSPDI1(id) WKKRNLC_##id
  #define WKSPDIF(id, val) WKKRNLF_##id = val
#endif

typedef enum {
#if defined(_WKDOSC)
  WKSPDIF(CLEANUP, 0x0D),        // no standard WIBU-KEY-API function
  WKSPDIF(GETDRIVERTYPE , 0x0E), // no standard WIBU-KEY-API function
#endif
  WKSPDIF(GETLASTERROR, 0x0F),
#if defined(_WKDOSC)
  WKSPDIF(RESET, 0x10),
  WKSPDIF(GETVER, 0x11),
  // the following two entries are reserved
  WKSPDIF(GETFC, 0x12),  // currently not used
  WKSPDIF(GETUC, 0x13),  // currently not used
  WKSPDIF(OPEN, 0x14),
  WKSPDIF(CLOSE, 0x15),
  WKSPDIF(SELECT, 0x16),
  WKSPDIF(CRYPT, 0x17),
  WKSPDIF(SELECTIND, 0x18),
  WKSPDIF(CRYPTIND, 0x19),
  WKSPDIF(UNSELECT, 0x1A),
  WKSPDIF(LISTPORT, 0x1C),
  WKSPDIF(LISTBOX, 0x1D),
  WKSPDIF(QUERYPORT, 0x1E),
  WKSPDIF(QUERYSTAT, 0x22),
  WKSPDIF(PROGRAM, 0x25),
  WKSPDIF(DEBUG, 0x26),
#endif // #if defined(_WKDOSC)
  // API 2 function variants (return value is no error code)
  WKSPDIF(SETLASTERROR, 0x27),
#if defined(_WKDOSC)
  WKSPDIF(ALLOCMEM, 0x28),
  WKSPDIF(FREEMEM, 0x29),
  WKSPDIF(WRITEMEM, 0x2A),
  WKSPDIF(READMEM, 0x2B),
#endif // #if defined(_WKDOSC)
  WKSPDIF(GETVERSION2, 0x30),
  WKSPDIF(OPEN2, 0x31),
  WKSPDIF(CLOSE2, 0x32),
  WKSPDIF(SELECT2, 0x33),
  WKSPDIF(CRYPT2, 0x34),
  WKSPDIF(LISTPORT2, 0x35),
  WKSPDIF(LISTBOX2, 0x36),
  WKSPDIF(PROGRAM2, 0x37),
  WKSPDIF(UNSELECT2, 0x38),
  WKSPDIF(QUERYPORT2, 0x39),
  WKSPDIF(QUERYSTATUS2, 0x3A),
  WKSPDIF(DIAGNOSE2, 0x3B),
  WKSPDIF(ACCESS2, 0x3C),
  WKSPDIF(RELEASE2, 0x3D),
  WKSPDIF(QUERYENTRY2, 0x3E),
  WKSPDIF(ENUMPORTS2, 0x3F),
  WKSPDIF(SETPORT2, 0x40),
  WKSPDIF(QUERYSYSTEM2, 0x41),
  WKSPDIF(ENUMSERVERS2, 0x42),
  WKSPDIF(CONTROLSYSTEM2, 0x43),
#if defined(_WKDOSC)
  WKSPDIF(ISCOMPLETE, 0x44),
  WKSPDIF(PROGRAMLOW, 0x45),
#endif // #if defined(_WKDOSC)
  WKSPDIF(QUERYINFO2, 0x46),
  WKSPDIF(APPINFO2, 0x47),
  WKSPDIF(GETVERSION3, 0x48)
} WKSPDI1(FNCODE);


// 16 bit handle for DOS communication API
#if !defined(HANDLE16) && defined(_WKDOSC)
  #if defined(WK32)
    #define HANDLE16 USHORT
  #else
    #define HANDLE16 HANDLE
  #endif
#endif

// 32 bit handle for Kernel driver communication API
#if !defined(HANDLE32) && defined(_WKKRNLC)
  #define HANDLE32 HANDLE
#endif

// =========================
// SPDI parameter structures
// =========================
// The virtual WKDOS calling interface defines:
// * WKDOSSTRUCT as union type (prefix "wkds")
// * WKDOS_xxx as constants via the WKSPDIP(xxx) macro
// * WKSPDI_USHORT as USHORT (16 bits)
// * WKSPDI_SHORT as SHORT (16 bits)
// * WKSPDI_ULONG as ULONG (32 bits)
// * WKSPDI_LONG as LONG (32 bits)
// * WKSPDI_HANDLE as HANDLE16 (16 bits)
// 
// The kernel driver calling interface defines:
// * WKKRNLP_STRUCT as union type (prefix "wkkp")
// * WKRRNLP_xxx as constants via the WKSPDIP(xxx) macro
// * WKSPDI_USHORT as ULONG (32 bits)
// * WKSPDI_SHORT as LONG (32 bits)
// * WKSPDI_ULONG as ULONG (32 bits)
// * WKSPDI_LONG as LONG (32 bits)
// * WKSPDI_HANDLE as HANDLE32 (32 bits)

#if defined(_WKDOSC)
  #define WKSPDI2(id) WKDOS##id
  #define WKSPDIP(id) WKDOS_##id
#else
  // _WKKRNLC defined
  #define WKSPDI2(id) WKKRNLP_##id
  #define WKSPDIP(id) WKSPDI2(id)
#endif

#if defined(_WKDOSC)
  #define WKSPDI_USHORT USHORT
  #define WKSPDI_SHORT  SHORT
  #define WKSPDI_ULONG  ULONG
  #define WKSPDI_LONG   LONG
  #define WKSPDI_HANDLE HANDLE16
#else
  // _WKKRNLC defined
  #define WKSPDI_USHORT ULONG
  #define WKSPDI_SHORT  LONG
  #define WKSPDI_ULONG  ULONG
  #define WKSPDI_LONG   LONG
  #define WKSPDI_HANDLE HANDLE32
#endif

#if defined(_WKDOSC)
  #pragma pack(1)
#endif

#if defined(_WKDOSC)

// SPDI-Function 0x10: Reset (API 1)
typedef struct {
  WKSPDI_USHORT fsControl;
} WKSPDIP(RESET);

// SPDI-Function 0x11: Get Version (API 1)
typedef struct {
  WKSPDI_USHORT FAR *pusVersion;
} WKSPDIP(GETVER);

// SPDI-Function 0x14: Open WIBU-BOX Entry (API 1)
typedef struct {
  WKSPDI_USHORT usDestLen;
  BOXENTRY FAR  *pboxentry;
  WKSPDI_HANDLE FAR *phbox;
  WKSPDI_LONG   lUserCode;
  WKSPDI_LONG   lFirmCode;
  WKSPDI_SHORT  usPort;
} WKSPDIP(OPEN);

// SPDI-Function 0x15: Close WIBU-BOX Entry (API 1)
typedef struct {
  WKSPDI_HANDLE hbox;
} WKSPDIP(CLOSE);

// SPDI-Function 0x16: Select Encryption Sequence (API 1)
typedef struct {
  WKSPDI_USHORT usSelectCode;
  WKSPDI_HANDLE hbox;
} WKSPDIP(SELECT);

// SPDI-Function 0x17: Encrypt Memory (API 1)
typedef struct {
  WKSPDI_USHORT FAR *pusDestLen;
  WKSPDI_USHORT usSrcLen;
  VOID FAR *pvSrcBuf;
  VOID FAR *pvDestBuf;
  WKSPDI_HANDLE hbox;
} WKSPDIP(CRYPT);

// SPDI-Function 0x18: Select Indirect Crypt Sequence (API 1)
typedef struct {
  BYTE FAR *pbInitBuf;
  WKSPDI_USHORT usSize;
  WKSPDI_USHORT usSelectCode;
  WKSPDI_HANDLE hbox;
} WKSPDIP(SELECTIND);

// SPDI-Function 0x19: Encrypt Memory Indirectly (API 1)
typedef struct {
  WKSPDI_USHORT FAR *pusDestLen;
  WKSPDI_USHORT usSrcLen;
  VOID FAR *pvSrcBuf;
  VOID FAR *pvDestBuf;
  WKSPDI_HANDLE hbox;
} WKSPDIP(CRYPTIND);

// SPDI-Function 0x1A: UnSelect WIBU-BOX Entry (API 1)
typedef struct {
  WKSPDI_HANDLE hbox;
} WKSPDIP(UNSELECT);

// SPDI-Function 0x1C: List WIBU-BOX Port (API 1)
typedef struct {
  WKSPDI_USHORT usDestBufLen;
  BYTE FAR *pbDestBuf;
  WKSPDI_USHORT usPort;
} WKSPDIP(LISTPORT);

// SPDI-Function 0x1D: List WIBU-BOX Entries (API 1)
typedef struct {
  WKSPDI_USHORT usDestBufLen;
  BYTE FAR *pbDestBuf;
  BYTE FAR *pbSerial;
  WKSPDI_USHORT usPort;
} WKSPDIP(LISTBOX);

// SPDI-Function 0x1E: Query WIBU-BOX port address (API 1)
typedef struct {
  WKBPORT FAR *pDestBuf;
  WKSPDI_USHORT usPort;
  WKSPDI_USHORT usLevel;
  WKSPDI_HANDLE hacc;
} WKSPDIP(QUERYPORT);

// SPDI-Function 0x22: Query WIBU-KEY driver status (API 1)
typedef struct {
  WKBSTATUS FAR *pDestBuf;
  WKSPDI_USHORT usLevel;
} WKSPDIP(QUERYSTAT);

// SPDI-Function 0x25: Program WIBU-BOX (API 1)
typedef struct {
  BYTE FAR *pbStatus;
  BYTE FAR *pbSequence;
  BYTE FAR *pbSerial;
  WKSPDI_USHORT usPort;
} WKSPDIP(PROGRAM);

// SPDI-Function 0x26: Read diagnostic data (API 1)
typedef struct {
  VOID *pvOutData;
  VOID *pvInData;
  WKSPDI_USHORT usData;
  WKSPDI_USHORT fFlags;
} WKSPDIP(DEBUG);

#endif // #if defined(_WKDOSC)

// SPDI-Function 0x27: Set last error code (API 2)
typedef struct {
  WKSPDI_SHORT idErrorCode;
} WKSPDIP(SETLASTERROR);

#if defined(_WKDOSC)

// SPDI-Function 0x28: Allocate driver memory (API 2)
typedef struct {
  WKSPDI_USHORT cbAlloc;
  WKSPDI_HANDLE hwkbsys;
} WKSPDIP(ALLOCMEM);

// SPDI-Function 0x29: Free driver memory (API 2)
typedef struct {
  WKSPDI_HANDLE hwkbmem;
} WKSPDIP(FREEMEM);

// SPDI-Function 0x2A: Write driver memory (API 2)
typedef struct {
  WKSPDI_USHORT cbData;
  const VOID FAR *pvSrc;
  WKSPDI_HANDLE hwkbmem;
} WKSPDIP(WRITEMEM);

// SPDI-Function 0x2B: Read driver memory (API 2)
typedef struct {
  WKSPDI_USHORT cbDest;
  VOID FAR *pvDest;
  WKSPDI_HANDLE hwkbmem;
} WKSPDIP(READMEM);

#endif // #if defined(_WKDOSC)

// SPDI-Function 0x30: Get Version of WIBU-KEY subsystem (API 2)
typedef struct {
  WKSPDI_HANDLE hwkbsys;
} WKSPDIP(GETVERSION2);

// SPDI-Function 0x31: Open WIBU-BOX entry (API 2)
typedef struct {
  VOID FAR *pvCtrl;
  WKSPDI_LONG   lUserCode;
  WKSPDI_LONG   lFirmCode;
  const CHAR FAR *pszPort;
  WKSPDI_ULONG  flCtrl;
  WKSPDI_HANDLE hwkbsys;
} WKSPDIP(OPEN2);

// SPDI-Function 0x32: Close WIBU-BOX entry (API 2)
typedef struct {
  WKSPDI_HANDLE hwkbe;
} WKSPDIP(CLOSE2);

// SPDI-Function 0x33: Select encryption (API 2)
typedef struct {
  VOID FAR *pvCtrl;
  WKSPDI_ULONG  ulSelectCode;
  WKSPDI_ULONG  flCtrl;
  WKSPDI_HANDLE hwkbe;
} WKSPDIP(SELECT2);

// SPDI-Function 0x34: Select encryption (API 2)
typedef struct {
  WKSPDI_SHORT FAR *pcbDest;
  WKSPDI_USHORT cbSrc;
  VOID FAR *pvCtrl;
  VOID FAR *pvDest;
  WKSPDI_ULONG  flCtrl;
  WKSPDI_HANDLE hwkbe;
} WKSPDIP(CRYPT2);

// SPDI-Function 0x35: List WIBU-BOXes at port (API 2)
typedef struct {
  WKSPDI_USHORT cbDest;
  VOID FAR *pvDest;
  const CHAR FAR *pszPort;
  WKSPDI_ULONG  flCtrl;
  WKSPDI_HANDLE hwkbsys;
} WKSPDIP(LISTPORT2);

// SPDI-Function 0x36: List WIBU-BOX contents at port (API 2)
typedef struct {
  WKSPDI_USHORT cbDest;
  VOID FAR *pvDest;
  const WKBSERIAL FAR *pwkbsr;
  const CHAR FAR *pszPort;
  WKSPDI_ULONG  flCtrl;
  WKSPDI_HANDLE hwkbsys;
} WKSPDIP(LISTBOX2);

// SPDI-Function 0x37: Program WIBU-BOX entry (API 2)
typedef struct {
  const VOID FAR *pvPgmData;
  const WKBSERIAL FAR *pwkbsr;
  const CHAR FAR *pszPort;
  WKSPDI_ULONG  flCtrl;
  WKSPDI_HANDLE hwkbsys;
} WKSPDIP(PROGRAM2);

// SPDI-Function 0x38: Unselect encryption (API 2)
typedef struct {
  WKSPDI_HANDLE hwkbe;
} WKSPDIP(UNSELECT2);

// SPDI-Function 0x39: Query WIBU-KEY port attributes (API 2)
typedef struct {
  WKSPDI_USHORT cbDest;
  VOID FAR *pvDest;
  const CHAR FAR *pszPort;
  WKSPDI_ULONG  flCtrl;
  WKSPDI_HANDLE hwkbsys;
} WKSPDIP(QUERYPORT2);

// SPDI-Function 0x3A: Query Status of Subsystem (API 2)
typedef struct {
  WKSPDI_USHORT cbDest;
  WKBSTATUS FAR *pwkbst;
  WKSPDI_ULONG  flCtrl;
  WKSPDI_HANDLE hwkbsys;
} WKSPDIP(QUERYSTATUS2);

// SPDI-Function 0x3B: Read Diagnostic Data (API 2)
typedef struct {
  WKSPDI_ULONG  ulData;
  WKSPDI_USHORT cbData;
  VOID *pvData;
  WKSPDI_ULONG  flCtrl;
  WKSPDI_HANDLE hwkbsys;
} WKSPDIP(DIAGNOSE2);

// SPDI-Function 0x3C: Access WIBU-KEY subsystem driver (API 2)
typedef struct {
  const VOID FAR *pvCtrl;
  WKSPDI_ULONG  flCtrl;
} WKSPDIP(ACCESS2);

// SPDI-Function 0x3D: Release WIBU-KEY subsystem (API 2)
typedef struct {
  WKSPDI_HANDLE hwkbsys;
} WKSPDIP(RELEASE2);

// SPDI-Function 0x3E: Query WIBU-BOX open entry attributes (API 2)
typedef struct {
  WKSPDI_USHORT cbDest;
  VOID FAR *pvDest;
  WKSPDI_ULONG  flCtrl;
  WKSPDI_HANDLE hwkbe;
} WKSPDIP(QUERYENTRY2);

// SPDI-Function 0x3F: Enumate enum ports (API 2)
typedef struct {
  WKSPDI_USHORT cbDest;
  VOID FAR *pszDest;
  WKSPDI_ULONG  flCtrl;
  WKSPDI_HANDLE hwkbsys;
} WKSPDIP(ENUMPORTS2);

// SPDI-Function 0x40: Set WIBU-KEY port specific data (API 2)
typedef struct {
  WKSPDI_USHORT cbSetup;
  const VOID FAR *pvSetup;
  const CHAR FAR *pszPort;
  WKSPDI_ULONG  flCtrl;
  WKSPDI_HANDLE hwkbsys;
} WKSPDIP(SETPORT2);

// SPDI-Function 0x41: Query WIBU-KEY subsystem (API 2)
typedef struct {
  WKSPDI_USHORT cbDest;
  VOID FAR *pvDest;
  WKSPDI_ULONG  flCtrl;
  WKSPDI_HANDLE hwkbsys;
} WKSPDIP(QUERYSYSTEM2);

// SPDI-Function 0x42: Enumerate available servers (API 2)
typedef struct {
  WKSPDI_USHORT cbDest;
  VOID FAR *pszDest;
  WKSPDI_ULONG  flCtrl;
} WKSPDIP(ENUMSERVERS2);

// SPDI-Function 0x43: Control WIBU-KEY subsystem (API 2)
typedef struct {
  WKSPDI_USHORT  cbCtrl;
  VOID FAR *pvCtrl;
  WKSPDI_ULONG  flCtrl;
  WKSPDI_HANDLE hwkbsys;
} WKSPDIP(CONTROLSYSTEM2);

#if defined(_WKDOSC)

// SPDI-Function 0x44: Check if asynchronous call is complete (API 2)
typedef struct {
  WKSPDI_SHORT FAR *pidReturn;
  WKSPDI_SHORT  nActionID;
} WKSPDIP(ISCOMPLETE);

// SPDI-Function 0x45: Internally used program-low function (API 2)
typedef struct {
  USHORT cbData;
  const BYTE FAR *pabData;
  const WKBSERIAL FAR *pwkbsr;
  WKSPDI_SHORT  nPort;
} WKSPDIP(PROGRAMLOW);

#endif // #if defined(_WKDOSC)

// SPDI-Function 0x46: Query WIBU-KEY information (API 2)
typedef struct {
  WKSPDI_USHORT cbDest;
  VOID FAR *pvDest;
  WKSPDI_ULONG  flCtrl;
  WKSPDI_HANDLE hwkbsys;
} WKSPDIP(QUERYINFO2);

// SPDI-Function 0x47: manage WIBU-KEY application info (API 2)
typedef struct {
  WKSPDI_USHORT cbInfo;
  VOID FAR *pvInfo;
  WKSPDI_ULONG  flCtrl;
  WKSPDI_HANDLE hwkbsys;
} WKSPDIP(APPINFO2);

// SPDI-Function 0x48: Get Version of WIBU-KEY subsystem (API 3)
typedef struct {
  WKSPDI_HANDLE hwkbsys;
} WKSPDIP(GETVERSION3);

// union of all WkDos structures
typedef union {
#if defined(_WKDOSC)
  WKSPDIP(RESET) Reset;
  WKSPDIP(GETVER) GetVer;
  WKSPDIP(OPEN) Open;
  WKSPDIP(CLOSE) Close;
  WKSPDIP(SELECT) Select;
  WKSPDIP(CRYPT) Crypt;
  WKSPDIP(SELECTIND) SelectInd;
  WKSPDIP(CRYPTIND) CryptInd;
  WKSPDIP(UNSELECT) UnSelect;
  WKSPDIP(LISTPORT) ListPort;
  WKSPDIP(LISTBOX) ListBox;
  WKSPDIP(QUERYPORT) QueryPort;
  WKSPDIP(QUERYSTAT) QueryStat;
  WKSPDIP(PROGRAM) Program;
  WKSPDIP(DEBUG) Debug;
#endif // #if defined(_WKDOSC)
  WKSPDIP(SETLASTERROR) SetLastError;
#if defined(_WKDOSC)
  WKSPDIP(ALLOCMEM) AllocMem;
  WKSPDIP(FREEMEM) FreeMem;
  WKSPDIP(WRITEMEM) WriteMem;
  WKSPDIP(READMEM) ReadMem;
#endif // #if defined(_WKDOSC)
  WKSPDIP(GETVERSION2) GetVersion2;
  WKSPDIP(OPEN2) Open2;
  WKSPDIP(CLOSE2) Close2;
  WKSPDIP(SELECT2) Select2;
  WKSPDIP(CRYPT2) Crypt2;
  WKSPDIP(LISTPORT2) ListPort2;
  WKSPDIP(LISTBOX2) ListBox2;
  WKSPDIP(PROGRAM2) Program2;
  WKSPDIP(UNSELECT2) UnSelect2;
  WKSPDIP(QUERYPORT2) QueryPort2;
  WKSPDIP(QUERYSTATUS2) QueryStatus2;
  WKSPDIP(DIAGNOSE2) Diagnose2;
  WKSPDIP(ACCESS2) Access2;
  WKSPDIP(RELEASE2) Release2;
  WKSPDIP(QUERYENTRY2) QueryEntry2;
  WKSPDIP(ENUMPORTS2) EnumPorts2;
  WKSPDIP(SETPORT2) SetPort2;
  WKSPDIP(QUERYSYSTEM2) QuerySystem2;
  WKSPDIP(ENUMSERVERS2) EnumServers2;
  WKSPDIP(CONTROLSYSTEM2) ControlSystem2;
#if defined(_WKDOSC)
  WKSPDIP(ISCOMPLETE) IsComplete;
  WKSPDIP(PROGRAMLOW) ProgramLow;
#endif // #if defined(_WKDOSC)
  WKSPDIP(QUERYINFO2) QueryInfo2;
  WKSPDIP(APPINFO2) ApplicationInfo2;
  WKSPDIP(GETVERSION3) GetVersion3;
} WKSPDI2(STRUCT);

#if defined(_WKDOSC)
  #pragma pack()
#endif // if defined(_WKDOSC)

// undefine all local used macros for recalling this header file
#undef WKSPDI1
#undef WKSPDI2
#undef WKSPDIF
#undef WKSPDIP
#undef WKSPDI_USHORT
#undef WKSPDI_SHORT
#undef WKSPDI_ULONG
#undef WKSPDI_LONG
#undef WKSPDI_HANDLE
