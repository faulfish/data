/* ---- WK.H -----------------------------------------------------------------
 * WIBU-KEY Development-Kit ANSI/Microsoft C header file.
 * (C) Copyright 1989-2006 by WIBU-SYSTEMS AG
 * D-76137 Karlsruhe, Rueppurrer Strasse 52, Germany/Europe
 * http://www.wibu.com
 * Version 5.20 of 2006Nov22
 * ---------------------------------------------------------------------------
 */

/* ==================================================
 * WIBU-KEY system independent types and macros
 * Compatible to Microsoft's Windows-API declarations
 * ==================================================
 */

#if !defined(_WK_INCLUDED)
#define _WK_INCLUDED

#if !defined(WKAPIVER)
#define WKAPIVER (4*256+0)
#endif

#if defined(__cplusplus)
extern "C" {            /* WK.H contains only C declarations */
#endif

/* set WkAPI 32 bit mode under Win32 */
#if !defined(WK32) && (defined(_WIN32) || defined(_MAC) || \
    defined(_NLM) || defined(__DPMI32__) || defined(_UNIX))
  #define WK32
#endif

/* general types & conversation, partial compatible with OS/2 and Windows */
#if !defined(VOID)
  #define VOID void
#endif

#if !defined(WKAPIENTRY)
  #if defined(_WIN32)
    /* Win32 API */
    #define WKAPIENTRY __stdcall
  #elif defined(_WINDOWS)
    /* Win16 API */
    #define WKAPIENTRY far pascal
  #elif defined(OS2)
    #if defined(WK32)
      /* OS/2 2.x or later: use mixed code call */
      #define WKAPIENTRY __stdcall
    #else
      /* OS/2 16 bits */
      #if defined(APIENTRY16)
        /* OS/2 2.X: assume that 16 bit is used */
        #define WKAPIENTRY APIENTRY16
      #elif defined(APIENTRY)
        /* OS/2 1.X: use standard API calling mechanism */
        #define WKAPIENTRY APIENTRY
      #endif
    #endif
  #elif defined(WK32)
    /* standard 32 bit API for SRCDRV: ANSI C */
    #define WKAPIENTRY
  #else
    /* standard 16 bit API (DOS): Pascal */
    #define WKAPIENTRY far pascal
  #endif
#endif

#if defined(_WIN64)
  /* Win64 API */
  #define WKUNALIGNED UNALIGNED
  #undef DECLARE_HANDLE
  #define DECLARE_HANDLE(name) typedef UINT32 name
#else
  #define WKUNALIGNED
#endif

#if !defined(WKCALLBACK)
  #if defined(_WIN32)
    /* Win32 API */
    #define WKCALLBACK __stdcall
  #elif defined(_WINDOWS)
    /* Win16 API */
    #define WKCALLBACK far pascal
  #elif defined(OS2)
    #if defined(WK32)
      /* OS/2 2.x or later: use mixed code call */
      #define WKCALLBACK __stdcall
    #else
      /* OS/2 16 bits */
      #if defined(APIENTRY16)
        /* OS/2 2.X: assume that 16 bit is used */
        #define WKCALLBACK APIENTRY16
      #elif defined(APIENTRY)
        /* OS/2 1.X: use standard API calling mechanism */
        #define WKCALLBACK APIENTRY
      #endif
    #endif
  #elif defined(WK32)
    /* standard 32 bit API for SRCDRV: ANSI C */
    #define WKCALLBACK
  #else
    /* standard 16 bit API (DOS): Pascal */
    #define WKCALLBACK far pascal
  #endif
#endif

#if !defined(FAR)
  #if defined(WK32)
    #define FAR
  #else
    #define FAR far
  #endif
#endif

#if !defined(NEAR)
  #if defined(WK32)
    #define NEAR
  #else
    #define NEAR near
  #endif
#endif

#if !defined(CONST)
  #define CONST const
#endif

#if !defined(OS2_INCLUDED) && !(defined(_WIN32) && defined(_INC_WINDOWS))

  /* OS/2 or Win32 specific declarations */
  #define INT int
  #define CHAR char
  #define SHORT short
  typedef unsigned char UCHAR;
  typedef unsigned short USHORT;
  typedef unsigned int UINT;
  #if defined(__GNUC__)
    #define ULONG unsigned int
  #else
    typedef unsigned long ULONG;
  #endif // __GNUC__
  typedef VOID FAR *PVOID;
#if !defined(LOUSHORT)
  #define LOUSHORT(l) ((USHORT)(ULONG)(l))
#endif
#if !defined(HIUSHORT)
  #define HIUSHORT(l) ((USHORT)(((ULONG)(l)>>16)))
#endif
#if !defined(MAKEULONG)
  #define MAKEULONG(l,h) ((ULONG)(USHORT)(l)|(ULONG)(USHORT)(h)<<16)
#endif
#if !defined(MAKEUSHORT)
  #define MAKEUSHORT(l, h) ((USHORT)((BYTE)(l)|(BYTE)(h)<<8))
#endif

#endif

/* Standard/Unicode character type */
#if !defined(TCHAR)
  #if defined(_UNICODE)
    #define TCHAR USHORT
  #else
    #define TCHAR CHAR
  #endif
#endif

#if !defined(HANDLE) && !defined(NOBASEDEFS) && !defined(DECLARE_HANDLE)
  /* Windows-compatible handle declarations */
  #if defined(__GNUC__)
  typedef unsigned int HANDLE;
  #else
  typedef CONST void NEAR* HANDLE;
  #endif
  #define WKSTRICT
#endif
#if !defined(DECLARE_HANDLE)
  #if (defined(STRICT) || defined(WKSTRICT)) && !defined(__GNUC__)
    /* Windows-compatible handle declarations */
    #define DECLARE_HANDLE(name) struct name##__ { int unused; };\
       typedef CONST struct name##__ NEAR* name
  #else
    /* non-stricted handle type */
    #define DECLARE_HANDLE(name) typedef HANDLE name
  #endif
#endif

/* Windows and OS/2 compatible declarations */
#if !defined(LONG)
  #if defined(__GNUC__)
    // define long as 32 bit value
    #define LONG int
  #else
    #define LONG long
  #endif // __GNUC__
  typedef unsigned char BYTE;
  typedef int BOOL;
#endif

#if !defined(FALSE)
  #define FALSE 0
#endif
#if !defined(TRUE)
  #define TRUE 1
#endif

#if !defined(LOBYTE)
  #define LOBYTE(us) ((BYTE)((ULONG)(us) & 0xFF))
#endif
#if !defined(HIBYTE)
  #define HIBYTE(us) ((BYTE)(((USHORT)(ULONG)(us) >> 8) & 0xFF))
#endif

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(push, 1)
#endif

typedef struct {
  USHORT twosecs: 5;
  USHORT minutes: 6;
  USHORT hours:   5;
} WKFTIME;

typedef struct {
  USHORT day:   5;
  USHORT month: 4;
  USHORT year:  7;
} WKFDATE;

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(pop)
#endif

#if !defined(MAKEINTRESOURCE)
  #define MAKEINTRESOURCE(i) ((CONST TCHAR FAR*)(ULONG)(USHORT)(i))
#endif

/* convert memory handle into "pointer" */
#if defined(__GNUC__)
  #define WKB_MEMHANDLE(h) ((VOID FAR*)(ULONG)(h))
#else
  #define WKB_MEMHANDLE(h) ((VOID FAR*)(ULONG)(USHORT)(h))
#endif

#if defined(__TURBOC__) &&\
 (defined(__TINY__)||defined(__SMALL__)||defined(__COMPACT__))
 #define MAKECODEPTR(p) ((PVOID)(((unsigned long)_CS<<16)|(USHORT)(p)))
#elif defined(__TURBOC__)
 /* other models of Turbo C */
 #define MAKECODEPTR(p) ((PVOID)((ULONG)(p)))
#else
 /* Microsoft C or other compilers */
 #define MAKECODEPTR(p) ((PVOID)(p))
#endif

#if !defined(_WK_NODEFLIB)

  #if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__WATCOMC__)
    /* set default library names into object file */
    #if defined(_WIN32)
      #if defined(_M_ALPHA)
        #pragma comment(lib, "wkwinDa")
      #elif defined(_M_IX86)
        #if defined(__BORLANDC__)
          #pragma comment(lib, "bcwin32")
        #else
          #if defined(_WIBUKEY_API_STATIC_LIB) && !defined(_DEBUG)
            /* link static lib, only possible when release is wanted */
            #if defined(_MSC_VER)
              #if _MSC_VER >= 1400
                #if defined(_MFC_VER)
                  /* link MFC based variant */ 
                  #pragma comment(lib, "WkWin32StaticV8MD")
                #else
                  /* link Standard C based variant */ 
                  #pragma comment(lib, "WkWin32StaticV8MT")
                #endif
              #elif _MSC_VER >= 1300
                #if defined(_MFC_VER)
                  /* link MFC based variant */ 
                  #pragma comment(lib, "WkWin32StaticV7MD")
                #else
                  /* link Standard C based variant */ 
                  #pragma comment(lib, "WkWin32StaticV7MT")
                #endif
              #elif _MSC_VER >= 1200
                #if defined(_MFC_VER)
                  /* link MFC based variant */ 
                  #pragma comment(lib, "WkWin32StaticV6MD")
                #else
                  /* link Standard C based variant */ 
                  #pragma comment(lib, "WkWin32StaticV6MT")
                #endif
              #else
                /* no fitting VC compiler -> take dll */
                #pragma comment(lib, "WkWin32")
              #endif
            #else
              /* no fitting compiler -> take dll */
              #pragma comment(lib, "WkWin32")
            #endif
          #else
            /* link dynamic lib/dll */
            #pragma comment(lib, "WkWin32")
          #endif
        #endif
      #elif defined(_M_IA64) || defined(_M_AMD64)
          #if defined(_WIBUKEY_API_STATIC_LIB) && !defined(_DEBUG)
            /* link static lib, only possible when release is wanted */
            #if defined(_MFC_VER)
              /* link MFC based variant */ 
              #pragma comment(lib, "WkWin64StaticV8MD")
            #else
              /* link Standard C based variant */ 
              #pragma comment(lib, "WkWin64StaticV8MT")
            #endif
          #else
            /* link dynamic lib/dll */
            #pragma comment(lib, "WkWin64")
          #endif
      #endif
    #else
      /* 16 bit: Windows, DOS extenders & DOS */
      #if defined(_WINDLL) || defined(_WINDOWS) || defined(__WINDOWS__) || defined(_Windows)
        /* Windows */
        #pragma comment(lib, "wkwin")
      #elif defined(__DPMI16__)
        /* 16 bit DOS extender */
        #pragma comment(lib, "wkdpmi16")
      #elif defined(__DPMI32__)
        /* 32 bit DOS extender */
        #pragma comment(lib, "wkdpmi32")
      #else /* DOS */
        #pragma comment(lib, "wkdos")
      #endif
    #endif
  #endif /* #if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__WATCOMC__) */

#endif /* #if !defined(_WK_NODEFLIB) */


/* ===================================================
 * WIBU-KEY API base declarations:
 * Fixed parts of function call and structure controls
 * ===================================================
 */

/* ----------------------------------------------------
 * general structure of <flCtrl> parameter of functions
 * ----------------------------------------------------
 *
 *  31              20          16         8          4          0
 *  +-----------------+--+--+-----+---------+----------+---------+
 *  |<-----flags2---->|PM|XM|AM|UC|  level  | command  |  flags1 |
 *  +-----------------+--+--+-----+---------+----------+---------+
 *
 *  flags1   contains command and function specific flags or other values.
 *  command  contains the command code for the operation via the function.
 *           This is one of the values: 0x010, 0x020, 0x030, ..., 0x0F0
 *  level .. contains the level of the called function. Currently all
 *           functions accept only level 0 (0x0000) and 1 (0x0100).
 *  UC ..... is 1 if UNICODE characters are transferred and 0 otherwise.
 *  AM ..... is 1 if the function is called in asynchronous mode and 0
 *           if the function is called in synchronous mode.
 *  XM ..... is 1 if the standard flags and the level are set individually
 *           in the function call and 0 if the default setting of the
 *           standard flags and the level of the subsystem are used.
 *  PM ..... is 1 if offsets are created in structures and 0 if pointers
 *           are created.
 *  flags2   contains command and function specific flags.
 *
 *  ----------------------------------------------------------
 *  general structure of <flCtrl> parameter of base structures
 *  ----------------------------------------------------------
 *
 *  31              20          16         8          4          0
 *  +-----------------+--+--+--+--+---------+----------+---------+
 *  |<-----flags2---->|PM|XM| 0|UC|<----------flags 1----------->|
 *  +-----------------+--+--+--+--+---------+----------+---------+
 *
 *  UC ..... is 1 if Unicode characters are specified in the structure and
 *           0 if 8-bit-characters are used.
 *  XM ..... is 1 if the standard flags are set individually for this
 *           structure and 0 if the default setting of the
 *           standard flags of the subsystem are used.
 *  PM ..... is 1 if offsets are used in the structure and 0 if pointers
 *           are used.
*/

/* level specification */
#define WKB_LEVELMASK   0xFF00 /* level mask in function parameter <flCtrl> */
#define WKB_LEVEL0      0x0000 /* std. level for version 2.00-2.21 */
#define WKB_LEVEL1      0x0100 /* std. level since version 2.30 */
#define WKB_LEVEL2      0x0200 /* reserved level for future use */

#define WKB_UNICODE       0x10000  /* use Unicode characters in strings */
#define WKB_SYNCHRONOUS   0x00000  /* call function synchronously */
#define WKB_ASYNCHRONOUS  0x20000  /* call function asychronously */
#define WKB_STDMODE       0x00000  /* use standard setting of subsystem */
#define WKB_EXCLMODE      0x40000  /* use exclusive setting for this call */
#define WKB_USEPOINTERS   0x00000  /* use data pointers within structure */
#define WKB_USEOFFSETS    0x80000  /* use data offsets within structure */
#define WKB_STDFLAGMASK   0xF0000  /* mask to access standard control flags */

/* definition of <flCtrl> standard flags */
#if !defined(WKB_STDCTRL)
  #if defined(_UNICODE)
    #define WKB_STDCTRL (WKB_UNICODE|WKB_SYNCHRONOUS|WKB_LEVEL1)
  #else
    #define WKB_STDCTRL (WKB_SYNCHRONOUS|WKB_LEVEL1)
  #endif
#endif
#if !defined(WKB_STDCTRL_STRUCT)
  #if defined(_UNICODE)
    #define WKB_STDCTRL_STRUCT (WKB_UNICODE|WKB_USEPOINTERS)
  #else
    #define WKB_STDCTRL_STRUCT (WKB_USEPOINTERS)
  #endif
#endif

/* version of WIBU-KEY algorithm, used by WkbOpen2() */
#define WKB_VERSION1      0x0000
#define WKB_VERSION2      0x0001
#define WKB_VERSION3      0x0002
#if WKAPIVER >= 4*256+0
#define WKB_VERSION4      0x0003
#define WKB_VERSION5      0x0004
#define WKB_VERSION_MAX   WKB_VERSION5
#else
#define WKB_VERSION_MAX   0x0002
#endif
#if WKAPIVER >= 3*256+0
#define WKB_VERSION_STD   0x0007
#endif
#define WKB_VERSION_RED   0x0006
#define WKB_VERSION_MASK  0x0007

/* dialog box options for Windows driver API */
/* used in WkbQuerySystem2(), WkbQueryInfo2(), and WkbApplicationInfo2() */
#define WKB_DLG_NO_VERSION  0x1000000 /* suppress "view version" button */
#define WKB_DLG_NO_PANEL    0x2000000 /* suppress "CPL panel..." button */
#define WKB_DLG_NO_HELP     0x4000000 /* suppress "WIBU-KEY Help" button */

/* several used <flCtrl> constants */
#define WKB_PORTNUMBER 0x0008  /* port in <pszPort> is number, no string */

/* macro to address offset specified member within structure */
#define WKB_STROFFS(structure, member) \
  ((TCHAR*)((BYTE*)(structure) + (UINT)(ULONG)(structure)->member))


/* ===============================
 * WIBU-KEY API base declarations:
 * handles
 * ===============================
 */

DECLARE_HANDLE(HWKBMEM);           /* handle of allocated memory block */
DECLARE_HANDLE(HWKBENTRY);         /* handle of opened WIBU-BOX entry */
DECLARE_HANDLE(HWKBSYSTEM);        /* handle of WIBU-KEY subsystem */

#define HWKB_LOCAL ((HWKBSYSTEM)1) /* handle value for local system */


/* ======================================
 * WIBU-KEY API base declarations:
 * structures and their control constants
 * ======================================
 */

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(push, 1)
#endif

/* -------------------
 * WKBSERIAL structure
 * -------------------
 * specification of a WIBU-BOX serial number
 * used by WkbListPort(), WkbListBoxPort() and WkbProgram()
 */
typedef struct {
  ULONG  ulSerial;
  USHORT usMask;
  USHORT usReserved;
} WKUNALIGNED WKBSERIAL; /* prefix "wkbsr" */


/* --------------------
 * WKBBOXLIST structure
 * --------------------
 * used by WkbListPort()
 */
typedef struct {
  ULONG  flCtrl;        /* reserved for future use */
  USHORT cBox;          /* number of WIBU-BOXes */
  USHORT usReserved;    /* reserved for future use */
  WKBSERIAL awkbsr[1];  /* serial numbers of found WIBU-BOXes */
} WKUNALIGNED WKBBOXLIST; /* prefix "wkbbxl" */

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(pop)
#endif

/* ---------------------
 * WKBBOXENTRY structure
 * ---------------------
 * specification of a single WIBU-BOX entry
 * used by WkbQueryEntry() and WkbListBox()
 */
#define WKB_BE_INDEXMASK 0x000F /* mask to determine entry index */

#define WKB_BE_EMPTY     0x0000  /* empty entry (all bytes 0) */
#define WKB_BE_FCUC      0x0100  /* standard Firm/User code entry */
#define WKB_BE_MASTER    0x0200  /* master Firm/Match code entry */
#define WKB_BE_DATA      0x0300  /* unbundled data entry */
#define WKB_BE_FILLED    0x0D00  /* filled with any entry (only RmtPgm) */
#define WKB_BE_DEFECT    0x0E00  /* entry is defect */
#define WKB_BE_INVALID   0x0F00  /* entry is invalid */
#define WKB_BE_TYPEMASK  0x0F00

#define WKB_BE_ADDLIMIT    0x1000  /* entry has additional limit entry */
#define WKB_BE_ADDDATA     0x2000  /* entry has additional data entry */
#define WKB_BE_ADDEXPDATE  0x4000  /* extra has additional expiration date */
#define WKB_BE_ADDMASK     0x7000

#define WKB_DATA_SERIALVARY 0x10000 /* modific. varies with serial number */
#define WKB_DATA_COUNTVARY  0x20000 /* modific. varies with program count */
#define WKB_DATA_DATAVARY   0x40000 /* modific. varies with data contents */
#define WKB_DATA_VARYMASK   0x70000

/* used only in WKBBOXENTRY structure within WKBPROGRAM structure */
#define WKB_LIMIT_ABSOLUTE  0x000000 /* replace by specified value */
#define WKB_LIMIT_OFFSET    0x100000 /* add with specified value */

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(push, 1)
#endif

typedef struct {
  ULONG flCtrl;
  ULONG ulFirmCode;
  ULONG ulUserCode;
  ULONG ulData1;
  ULONG ulData2;
} WKUNALIGNED WKBBOXENTRY; /* prefix "wkbbxe */


/* --------------------
 * WKBBOXDATA structure
 * --------------------
 * specification of the entry contents of a WIBU-BOX
 * used by WkbListBox()
 */
typedef struct {
  ULONG flCtrl;            /* reserved for future use */
  USHORT usVersion;        /* version number of WIBU-BOX */
  USHORT cEntry;           /* number of valid entries in <awkbbxe> */
  WKBBOXENTRY awkbbxe[10]; /* entry values of found WIBU-BOXes */
} WKUNALIGNED WKBBOXDATA; /* prefix "wkbbxd" */


/* ----------------------
 * WKBBOXCONFIG structure
 * ----------------------
 * specification of a single WIBU-BOX entry
 * used by WkbListBox() and WkbProgram()
 */
typedef struct {
  ULONG  flCtrl;         /* reserved for future use */
  USHORT usVersion;      /* version number of WIBU-BOX */
  USHORT usReserved;     /* currently not used */
  ULONG  ulProgramCount; /* WIBU-BOX Global Programming Counter register */
  ULONG  ulConfig;       /* WIBU-BOX configuration register */
#if WKAPIVER >= 4*256+0
  USHORT usExtMemSize;   /* number of pages in the ExtMemSize totally */
  USHORT usAsicBuild;    /* WIBU-BOX ASIC build number */
#endif
} WKUNALIGNED WKBBOXCONFIG; /* prefix "wkbbxc" */

/* ---------------------
 * WKBOPENFIND structure
 * ---------------------
 * used by WkbOpenFind()
 */
typedef struct {
  ULONG  flCtrl;         /* version of used algorithm */
  ULONG  ulFirmCode;     /* used Firm Code */
  ULONG  ulUserCode;     /* used User Code */
  ULONG  ulEntrySkip;    /* skip entry bit array (0 => no skip) */
} WKUNALIGNED WKBOPENFIND; /* prefix "wkbof" */

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(pop)
#endif

/* ----------------------
 * WKBOPENENTRY structure
 * ----------------------
 * used by WkbOpen() and WkbQueryEntry()
 */

/* flags in <flCtrl> member of WKBOPENENTRY structure */
#define WKB_OE_MASTER       0x000008 /* opened via master entry */
#define WKB_OE_SELECTED     0x000010 /* opened entry currently selected */
#define WKB_OE_NONETLIMIT   0x100000 /* opened with WKB_OPEN_NONETLIMIT */
#define WKB_OE_GLOBAL       0x200000 /* opened with WKB_OPEN_GLOBAL */
#define WKB_OE_EXCLUSIVE    0x400000 /* opened with WKB_OPEN_EXCLUSIVE */
#if WKAPIVER >= 3*256+0
#define WKB_OE_STATIONSHARE 0x800000 /* opened with WKB_OPEN_STATIONSHARE */
#endif

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(push, 1)
#endif

typedef struct {
  ULONG  flCtrl;
  HWKBSYSTEM hwkbsys;
#if !defined(WK32)
  USHORT usFill; /* fill gap to 32 bit compliance */
#endif
  LONG   lPort;
  TCHAR  FAR *pszPort;
  WKBSERIAL wkbsr;
  USHORT usVersion;
  USHORT usEntry;
  ULONG  ulFirmCode;
  ULONG  ulUserCode;
} WKUNALIGNED WKBOPENENTRY; /* prefix "wkboe" */

#if !defined(WK32)
  /* compatibility with API 1.x */
  typedef struct {
    USHORT usPort;
    BYTE abSerial[5];
    BYTE bEntry;
  } BOXENTRY;
#endif

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(pop)
#endif

/* -------------------
 * WKBSELECT structure
 * -------------------
 * indirect selection control structure for WkbSelect2()
 */
/* options for <flCtrl> of WKBSELECT structure */
#define WKB_SEL_FEAL8     0x0000  /* use FEAL 8 */
#define WKB_SEL_FEAL16    0x0001  /* use FEAL 16 */
#define WKB_SEL_FEAL32    0x0002  /* use FEAL 32 */
#define WKB_SEL_FEALMASK  0x0003  /* mask for FEAL mode */
#define WKB_SEL_CBC_CRYPT 0x0008  /* use CBC block encryption with FEAL */
#define WKB_SEL_OPLEN1    0x0000  /* operand length 1 byte */
#define WKB_SEL_OPLEN2    0x0001  /* operand length 2 bytes */
#define WKB_SEL_OPLEN4    0x0002  /* operand length 4 bytes */
#define WKB_SEL_OPLEN8    0x0003  /* operand length 8 bytes */
#define WKB_SEL_OPLENMASK 0x0003  /* mask for operand length */
#define WKB_SEL_SET_CACHE 0x0010  /* <ulCacheId> is valid */
#define WKB_SEL_CHKCRC    0x0020  /* check 16 bit CRC */
#define WKB_SEL_CALCCRC   0x0040  /* calculate 16 bit CRC */

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(push, 1)
#endif

typedef struct {
  ULONG flCtrl;      /* control flags, data length for permutation,
                        round length of FEAL */
  ULONG ulCacheId;   /* cache identification for WKB_SEL_CACHED */
  ULONG ulCrc;       /* CRC check/calc value for initialization sequence */
  USHORT cbInit;     /* length of valid data in <abInit> */
  BYTE  abInit[20];  /* additional initialization data */
} WKUNALIGNED WKBSELECT; /* prefix "wkbsel" */

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(pop)
#endif

/* -----------------
 * WKBAREA structure
 * -----------------
 * optional control structure for WkbCrypt2()
 */
/* options for <flCtrl> of WkbCrypt() and of structure WKBAREA */
#define WKB_AREA_DATA 0x0000 /* access to data area (standard) */
#define WKB_AREA_CODE 0x0010 /* access to code area */
#define WKB_AREA_MASK 0x0070 /* mask for area type */
#define WKB_AREA_NONE 0x0080 /* access nothing (ignore call of function) */

/* options for <flCtrl> of WKBAREA structure */
#define WKB_AREA_SELECT  0x0001 /* select by <flSelCtrl> & <ulSelectCode> */
#define WKB_AREA_CHKCRC  0x0002 /* check 16 bit CRC of direct WIBU-BOX I/O */
#define WKB_AREA_CALCCRC 0x0004 /* calculate 16 bit CRC of direct I/O */

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(push, 1)
#endif

/* optional control data for <pvCtrl> of WkbCrypt() */
typedef struct {
  ULONG flCtrl;
  ULONG flSelCtrl;     /* same as <flCtrl> of WkbSelect2() */
  ULONG ulSelectCode;  /* same as <ulSelectCode> of WkbSelect2() */
  ULONG ulCrc;         /* 16 bit CRC of checked/calculated WIBU-BOX I/O */
  ULONG ulReserved;    /* expandable for future use */
  CONST BYTE FAR *pbGapMap; /* non-zero: use gap map for skips */
  VOID  FAR *pvCtrl;   /* control data for WKB_AREA_SELECT option */
} WKUNALIGNED WKBAREA; /* prefix "wkbar" */

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(pop)
#endif

/* macro to determine size of gap map in correlation with data size */
#define WKB_GET_MAP_SIZE(cbData) ((cbData + 7) / 8)


/* -----------------
 * WKBPORT structure
 * -----------------
 * used by WkbQueryPort2() and WkbSetPort2()
 */
/* constants for <fsPortCtrl> and <fsDrvCtrl> members of WKBPORT structure */
#define WKB_CTRL_DP      0x0001
#define WKB_CTRL_BD      0x0002
#define WKB_CTRL_UD      0x0004
#define WKB_CTRL_SHARED  0x0008
#define WKB_CTRL_RTSA    0x0002
#define WKB_CTRL_DTRA    0x0004
#define WKB_CTRL_BIN     0x0010
#define WKB_CTRL_CHAR    0x0020
#define WKB_CTRL_NAMC    0x0002
#define WKB_CTRL_NOSRVR  0x0001

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(push, 1)
#endif

typedef struct {
  ULONG ulAddress;    /* physical address of WIBU-KEY port */
  USHORT fsPortCtrl;  /* port class specific control flags */
  USHORT fsDrvCtrl;   /* standard control flags (port class independent) */
  ULONG ulReserved1;  /* reserved for future use */
  BYTE bBusType;      /* not used => 255 */
  BYTE bBusIndex;     /* in range 0..255 */
  USHORT usReserved2; /* reserved for future use */
} WKUNALIGNED WKBPORT; /* prefix "wkbprt" */


/* ---------------------
 * WKBPORTNAME structure
 * ---------------------
 * used by WkbQueryPort2()
 */
typedef struct {
  ULONG flCtrl;       /* control flags */
  LONG lPort;         /* logical port number */
  TCHAR FAR *pszPort; /* name of port */
} WKUNALIGNED WKBPORTNAME; /* prefix "wkbprtn" */

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(pop)
#endif

#if WKAPIVER >= 4*256+0

/* general flags used in WKBEXTMEM */
#define WKB_XM_USER 0    /* user data in ExtMem Page: unprotected */
#define WKB_XM_PROT 1    /* protected data in ExtMem page: Access Signature */

#define WKB_XM_RAW        0
#define WKB_XM_LICENSOR   2
#define WKB_XM_NOSIGN     8

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(push, 1)
#endif

/* -------------------
 * WKBEXTMEM structure
 * -------------------
 * specification to read or write a single ExtMem Page
 * used by WkbProgram2()
 */

typedef struct {
  USHORT fsCtrl;         /* contains WKB_XM_xxx */
  USHORT iPage;          /* contains index of the ExtMem page */
  BYTE   abSignature[8]; /* contains access signature for WKB_XM_PROT */
  BYTE   abData[32];     /* receives or contains data of ExtMem page */
} WKUNALIGNED WKBEXTMEM; /* prefix "wkbxm" */

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(pop)
#endif

#endif // WKAPIVER >= 4*256+0

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(push, 1)
#endif

/* --------------------
 * WKBPROGRAM structure
 * --------------------
 * used by WkbProgram2()
 */
/* constant for <flCtrl> member of WKBPROGRAM structure */
#define WKB_SET_BOXVERSION 0x100 /* lowest byte contains box version number */

typedef struct {
  ULONG flCtrl;         /* contains WIBU-BOX version for programming */
  WKBBOXENTRY wkbbxe;   /* entry which is programmed or modified */
  BYTE  abSignature[8]; /* programming security signature */
} WKUNALIGNED WKBPROGRAM; /* prefix "wkbpgm" */

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(pop)
#endif

/* -------------------
 * WKBSTATUS structure
 * -------------------
 * returned by WkbQueryStatus2()
 */
/* constants for <usFlags> member of WKBSTATUS structure */
#define WKB_ST_INTDRV 1 /* interrupt driver active, number in <usDrvInt> */
#define WKB_ST_KRNL   2 /* system kernel driver is used */
#define WKB_ST_WKDOS  4 /* WkDos Int 2F calling scheme */

/* new status constants (introduced with API V2.32) */
#define WKB_ST_ASYNCHRONOUS  0x0010 /* support of asynchronous mode */
#define WKB_ST_DIAGNOSIS     0x0020 /* support of diagnosis mode */
#define WKB_ST_STDINDCRYPT   0x0040 /* support of indirect encryption */
#define WKB_ST_EXTINDCRYPT   0x0080 /* support of extend. indir. encryption */
#define WKB_ST_AREACRYPT     0x0100 /* support of AREA encryption */
#define WKB_ST_EXPDATE       0x0200 /* support of Expiration Dates */
#define WKB_ST_LOCAL         0x1000 /* support of local WIBU-BOX access */
#define WKB_ST_WKLAN         0x2000 /* support of WkLAN server access */
#define WKB_ST_WKNET         0x4000 /* support of WKNet server access */

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(push, 1)
#endif

typedef struct {
  USHORT usFlags;
  USHORT usDrvInt;
} WKUNALIGNED WKBSTATUS; /* prefix "wkbst" */

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(pop)
#endif

/* --------------------
 * WKBAPPINFO structure
 * --------------------
 * used by WkbApplicationInfo2()
 */
/* constants for <flCtrl> member of WKBAPPINFO structure */
#define WKB_AIS_SEL_NAME     0x0001 /* select <pszAppName> */
#define WKB_AIS_SEL_VERSION  0x0002 /* select <bVersionLow/High> etc. */
#define WKB_AIS_SEL_MENU     0x0004 /* select <hwndMainWnd> etc */
#define WKB_AIS_SEL_LANG     0x0008 /* select <idLanguage> */
#define WKB_AIS_SEL_ALL      0x000F /* select all currently defined parts */

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(push, 1)
#endif

typedef struct {
  ULONG flCtrl;             /* control flags */
  TCHAR FAR *pszAppName;    /* name of application */
  BYTE  bVersionLow;        /* requested minor version of driver/subsystem */
  BYTE  bVersionHigh;       /* requested major version of driver/subsystem */
  USHORT fsAccessMask;      /* mask for version check */
#if defined(_WINDOWS)
  HWND  hwndMainWindow;     /* handle of application window */
#else
  HANDLE hwndMainWindow;    /* handle of application window */
#endif
  ULONG flMenuItemCtrl;     /* flags for application window menu */
  USHORT idLanguage;        /* desired language */
  USHORT usReserved1;       /* reserved for future use */
  ULONG  ulReserved2;       /* reserved for future use */
  ULONG  ulReserved3;       /* reserved for future use */
} WKUNALIGNED WKBAPPINFO; /* prefix "wkbai" */


/* -------------------
 * WKBACCESS structure
 * -------------------
 * transferred to WkbAccess2()
 */
/* constants for <flCtrl> member of WKBACCESS structure */
#define WKB_ACC_AUTOCANCEL 0x0001 /* cancel user with same id as specified */
#define WKB_ACC_NO_DIALOG  0x0002 /* ignore dialog during scanning network */
#define WKB_ACC_NO_CANCEL  0x0004 /* ignore cancel button during scanning */

/* constants for idNetUser */
#define WKB_NETUSER_FIND 0xFFFF /* find free entry */
#define WKB_NETUSER_REG  0xFFFE /* read used idNetUser from system registry */

typedef struct {
  /* general control parameters */
  ULONG flCtrl;            /* control control flags, a WKB_ACC_... constant */
  LONG  lFirmCode;         /* requested subsystem Firm Code */
  LONG  lUserCode;         /* requested subsystem User Code */
  ULONG flStdCtrl;         /* standard control flags for function calls */
  BYTE  bVersionLow;       /* requested minor version of subsystem */
  BYTE  bVersionHigh;      /* requested major version of subsystem */
  /* WkNet & WkLAN specific parameters */
  USHORT idNetUser;         /* index of fixed net-user or WKB_NETUSER_... */
  CONST TCHAR FAR *pszAppName;    /* name of protected application */
  CONST TCHAR FAR *pszServerName; /* optional name of the used server */
  /* WkNet specific parameters */
  CONST TCHAR FAR *pszWkNetFile; /* optional full path of WkNet server file */
  USHORT usRefreshTime;    /* WkNet server file refresh time in seconds */
#if WKAPIVER >= 2*256+50
  USHORT usMaxStartDelta;  /* maximum time difference client/server in min. */
#else
  USHORT usReserved1;      /* reserved until V2.50 */
#endif
  CONST BYTE FAR *pbEncryptSeq; /* pointer to optional WkNet encrypt. seq. */
#if WKAPIVER >= 2*256+52
  CONST WKBAPPINFO FAR *pwkbai; /* ref to application information structure */
#else
  ULONG ulReserved2;       /* reserved until V2.52 */
#endif
  ULONG ulReserved3;       /* reserved for future use */
} WKUNALIGNED WKBACCESS; /* prefix "wkbacc" */


/* --------------------
 * WKBVERSION structure
 * --------------------
 * returned by WkbQueryInfo2()
 */
typedef struct {
  USHORT usVersion;    /* main version, for example 2 for 2.52 */
  USHORT usSubVersion; /* subversion, for example 52 for 2.52 */
  USHORT usBuild;      /* continuous built number, 0 if not used */
  USHORT usCount;      /* count value for state (alpha/beta/gamma/final) */
  USHORT usYear;       /* year of release date, valid range 1999 up to 2099 */
  BYTE   bMonth;       /* month of release date, range 1..12 */
  BYTE   bDay;         /* day of release date, range 1..31 */
  ULONG  ulReserved1;  /* reserved for future use */
} WKUNALIGNED WKBVERSION; /* prefix "wkbver" */

/* -------------------
 * WKBSYSTEM structure
 * -------------------
 * returned by WkbQuerySystem2()
 */
typedef struct {
  ULONG  flCtrl;            /* control & status flags, a WKB_ACC_... flag */
  TCHAR FAR *pszServerName; /* name of the used server station */
  USHORT idNetUser;         /* used net user index */
  USHORT cMaxUser;          /* number of users at most */
  USHORT cFreeUser;         /* number of free entries in user list */
  USHORT cbMaxData;         /* maximum length of an encryption data element */
  /* the following members are WkNet specific */
  USHORT cRefreshError;     /* number of refresh timeout cycles */
  USHORT usReserved1;       /* reserved for future use */
} WKUNALIGNED WKBSYSTEM; /* prefix "wkbsys" */

#if defined(__GNUC__) && defined(__x86_64__)
  #pragma pack(pop)
#endif

/* =====================================
 * WIBU-KEY API base declarations:
 * functions and their control constants
 * =====================================
 */

/* -------------------------------------------------------------------
 * WKbAllocMem(), WkbFreeMem(), WkbReadMem() & WkbWriteMem() functions
 * -------------------------------------------------------------------
 */

HWKBMEM WKAPIENTRY WkbAllocMem(HWKBSYSTEM hwkbsys, UINT cbAlloc);
BOOL WKAPIENTRY WkbFreeMem(HWKBMEM hwkbmem);
UINT WKAPIENTRY WkbWriteMem(HWKBMEM hwkbmem,
  CONST VOID FAR *pvSrc, UINT cbData);
UINT WKAPIENTRY WkbReadMem(HWKBMEM hwkbmem, VOID FAR *pvDest, UINT cbDest);

/* ---------------------------------------------------------------------
 * WkbGetFC(), WkbGetUC() and WkbGetAlgVersion() task specific functions
 * ---------------------------------------------------------------------
 */
INT WKAPIENTRY WkbGetFC(LONG FAR *plFirmCode, ULONG ulReserved);
INT WKAPIENTRY WkbGetUC(LONG FAR *plUserCode, ULONG ulReserved);
#if WKAPIVER >= 4*256+0
INT WKAPIENTRY WkbGetAlgVersion(ULONG FAR *plAlgVersion, ULONG ulReserved);
#endif

/* ------------------------------------
 * WKbAccess() & WkbRelease() functions
 * ------------------------------------
 */

/* command codes in <flCtrl> of WkbAccess2() */
#define WKB_ACC_CREATE  0x0000 /* create new subsystem */
#define WKB_ACC_RESTORE 0x0010 /* access access-freed system again */
#define WKB_ACC_SHELL   0x0020 /* create new subsystem without security */
#define WKB_ACC_GLOBAL  0x0030 /* create local subsystem by specified entry */
#define WKB_ACC_MASK    0x00F0 /* command mask for <flCtrl> */

/* options in <flCtrl> of WkbAccess2() */
#define WKB_ACC_LOCAL         0x000001 /* access local WIBU-BOX */
#define WKB_ACC_WKLAN         0x000002 /* access any WkLAN server */
#define WKB_ACC_WKNET         0x000004 /* access any WKNet server */
#define WKB_ACC_DIRECT        0x100000 /* direct access to WkNet management */
#define WKB_ACC_NONETLIMIT    0x200000 /* no touch of net user limitation */
#define WKB_ACC_EXCLUSIVE     0x400000 /* exclusive access mode */
#if WKAPIVER >= 3*256+0
#define WKB_ACC_STATIONSHARE  0x800000 /* station share access mode */
#endif
#define WKB_ACC_FLAGS         0xF00000

HWKBSYSTEM WKAPIENTRY WkbAccess2(ULONG flCtrl, CONST VOID FAR *pvCtrl);
#if defined(WK32)
  #define WkbAccess(x1, x2) WkbAccess2(x1, x2)
#else
  /* no part of API 1.x */
#endif

INT WKAPIENTRY WkbRelease2(HWKBSYSTEM hwkbsys);
#if defined(WK32)
  #define WkbRelease(x1) WkbRelease2(x1)
#else
  /* no part of API 1.x */
#endif

/* -----------------------------------------------------------
 * Completion message function for asynchronous function calls
 * -----------------------------------------------------------
 */
typedef VOID (WKCALLBACK* PWKBCOMPLFUNC)(INT nActionID, INT nRetValue);

INT WKAPIENTRY WkbGetLastError(VOID);
VOID WKAPIENTRY WkbSetLastError(INT idError);
BOOL WKAPIENTRY WkbIsComplete(INT nActionID, INT FAR *pidReturn);
INT WKAPIENTRY WkbSetCompletionEvent(HWKBSYSTEM hwkbsys, HANDLE hEvent);
HANDLE WKAPIENTRY WkbGetCompletionEvent(HWKBSYSTEM hwkbsys);
INT WKAPIENTRY WkbSetCompletionFunction(HWKBSYSTEM hwkbsys,
  PWKBCOMPLFUNC pfn);
PWKBCOMPLFUNC WKAPIENTRY WkbGetCompletionFunction(HWKBSYSTEM hwkbsys);

/* -----------------------------
 * WkbApplicationInfo() function
 * -----------------------------
 */

/* command codes in <flCtrl> of WkbApplicationInfo2() */
#define WKB_AI_SETDATA      0x0000 /* set WKBAPPINFO structure */
#define WKB_AI_GETDATA      0x0010 /* get WKBAPPINFO structure */
#define WKB_AI_GETNEARLANG  0x0020 /* get nearest supported language ID */
#define WKB_AI_GETLANGLIST  0x0030 /* get list of all supported language ID */
#define WKB_AI_MESSAGE      0x0040 /* displays a message in a dialog box */
#define WKB_AI_MASK         0x00F0 /* command mask for <flCtrl> */

/* options, may be combined with command code */
#define WKB_AI_DIALOG   0x0008 /* display information in dialog box */


INT WKAPIENTRY WkbApplicationInfo2(HWKBSYSTEM hwkbsys, ULONG flCtrl,
  VOID FAR *pvInfo, UINT cbInfo);
#if defined(WK32)
  #define WkbApplicationInfo(x1, x2, x3, x4) \
    WkbApplicationInfo2(x1, x2, x3, x4)
#endif

/* -------------------------
 * WKbEnumServers() function
 * -------------------------
 */
/* command codes in <flCtrl> of WkbEnumServers2() */
#define WKB_ESV_LIST 0x0000 /* return complete list as string */
#define WKB_ESV_NEXT 0x0010 /* return server list step by step */
#define WKB_ESV_MASK 0x00F0 /* command mask for <flCtrl> */

/* options in <flCtrl> of WkbEnumServers2() */
#define WKB_ESV_NAME       0x0000 /* return name of server */
#define WKB_ESV_IPADDR     0x0001 /* return IP address values */
#define WKB_ESV_NO_DIALOG  0x0002 /* ignore dialog during scanning network */
#define WKB_ESV_NO_CANCEL  0x0004 /* ignore cancel button during scanning */
#define WKB_ESV_FLAGS      0x0007


INT WKAPIENTRY WkbEnumServers2(ULONG flCtrl, TCHAR FAR *pszDest,
  UINT cchDest);
#if defined(WK32)
  #define WkbEnumServers(x1, x2, x3) WkbEnumServers2(x1, x2, x3)
#else
  /* no part of API 1.x */
#endif


/* ----------------------------------------
 * WKbControlSystem() & WkbReset() function
 * ----------------------------------------
 */

/* command codes in <flCtrl> of WkbControlSystem2() */
#define WKB_CTS_RESET          0x0000 /* reset specified system */
#define WKB_CTS_CANCEL_USERS   0x0010 /* cancel specified users */
#define WKB_CTS_FREE_ACCESS    0x0020 /* temporary releasing of net access */
#define WKB_CTS_KILL_ACTION    0x0030 /* kill specified action in <cbCtrl> */
#define WKB_CTS_MASK           0x00F0 /* command mask for <flCtrl> */

/* reset flags for WKB_CTS_RESET command in <cbCtrl> */
#define WKB_RESET_SYSTEM      0x01 /* currently not used */
#define WKB_RESET_ADDRESS     0x02
#define WKB_RESET_TIMING      0x04
#define WKB_RESET_BOXSCAN     0x08
#define WKB_RESET_BOXLOCKS    0x10
#define WKB_RESET_FULL        0xFE

/* special value for WKB_CTS_KILL_ACTION */
#define WKB_ALL_ACTIONS   0

INT WKAPIENTRY WkbControlSystem2(HWKBSYSTEM hwkbsys, ULONG flCtrl,
  VOID FAR *pvCtrl, UINT cbCtrl);
#if defined(WK32)
  #define WkbControlSystem(x1, x2, x3, x4) WkbControlSystem2(x1, x2, x3, x4)
#else
  /* compatibility with API 1.x
     WkbControlSystem() is no part of API 1.x
   */
  USHORT WKAPIENTRY WkbReset(USHORT fsControl);
#endif


/* ------------------------------------------------------------
 * WkbGetVersion(), WkbQueryInfo() & WkbQuerySystem() functions
 * ------------------------------------------------------------
 */
INT WKAPIENTRY WkbGetVersion2(HWKBSYSTEM hwkbsys);
#if WKAPIVER >= 3*256+0
INT WKAPIENTRY WkbGetVersion3(HWKBSYSTEM hwkbsys);
#endif
#if defined(WK32)
  #define WkbGetVersion(x1) WkbGetVersion3(x1)
#else
  /* compatibility with API 1.x */
  USHORT WKAPIENTRY WkbGetVersion(USHORT FAR *pusVersion);
#endif

/* command codes in <flCtrl> of WkbQueryInfo2() */
#define WKB_QI_ERRTEXT  0x0000 /* return error text for last error code */
#define WKB_QI_VERVALUE 0x0010 /* query version value in WKBVERSION struct */
#define WKB_QI_VERTEXT  0x0020 /* query version value as text */
#define WKB_QI_MASK     0x00F0 /* command mask for <flCtrl> */

/* options, may be combined with command code */
#define WKB_QI_DRIVER   0x0001 /* return of driver version information */
#define WKB_QI_SERVER   0x0002 /* return of server version information */
#define WKB_QI_DIALOG   0x0008 /* display information in dialog box */

INT WKAPIENTRY WkbQueryInfo2(HWKBSYSTEM hwkbsys, ULONG flCtrl,
  VOID FAR *pvDest, UINT cbDest);
#if defined(WK32)
  #define WkbQueryInfo(x1, x2, x3, x4) WkbQueryInfo2(x1, x2, x3, x4)
#else
  /* no part of API 1.x */
#endif

/* command codes in <flCtrl> of WkbQuerySystem2() */
#define WKB_QSY_SYSTEM    0x0000 /* query WKBSYSTEM structure */
#define WKB_QSY_STATUS    0x0010 /* query complete status for later restore */
#define WKB_QSY_IPADDR    0x0020 /* return string with IP address */
#define WKB_QSY_OPENENTRY 0x0030 /* query WKBOPENENTRY structure */
#define WKB_QSY_MASK   0x00F0 /* command mask for <flCtrl> */

/* options, may be combined with command code */
#define WKB_QSY_UPDATE 0x0001 /* return updated information */
#define WKB_QSY_DIALOG 0x0002 /* display information in dialog box */

INT WKAPIENTRY WkbQuerySystem2(HWKBSYSTEM hwkbsys, ULONG flCtrl,
  VOID FAR *pvDest, UINT cbDest);
#if defined(WK32)
  #define WkbQuerySystem(x1, x2, x3, x4) WkbQuerySystem2(x1, x2, x3, x4)
#else
  /* no part of API 1.x */
#endif


/* -------------------------
 * WkbQueryStatus() function
 * -------------------------
 */
/* command codes in <flCtrl> of WkbQueryStatus2() */
#define WKB_QST_DRIVER         0x0000 /* query status from driver */
#define WKB_QST_MASK           0x00F0 /* command mask for <flCtrl> */

INT WKAPIENTRY WkbQueryStatus2(HWKBSYSTEM hwkbsys, ULONG flCtrl,
  WKBSTATUS FAR *pwkbst, UINT cbDest);
#if defined(WK32)
  #define WkbQueryStatus(x1, x2, x3, x4) WkbQueryStatus2(x1, x2, x3, x4)
#else
  /* compatibility with API 1.x */
  USHORT WKAPIENTRY WkbQueryStatus(USHORT usLevel, WKBSTATUS FAR *pwkbst);
#endif

/* --------------------------------
 * WkbOpen() & WkbClose() functions
 * --------------------------------
 */
/* commands for <flCtrl> of WkbOpen2() */
#define WKB_OPEN_FIND       0x0000
#define WKB_OPEN_ENTRY      0x0010
#define WKB_OPEN_MULTIFIND  0x0020
#define WKB_OPEN_MASK       0x00F0

#define WKB_OPEN_NONETLIMIT    0x100000
#define WKB_OPEN_GLOBAL        0x200000
#define WKB_OPEN_EXCLUSIVE     0x400000
#if WKAPIVER >= 3*256+0
#define WKB_OPEN_STATIONSHARE  0x800000
#endif
#define WKB_OPEN_FLAGS         0xF00000

/* special Firm and User Code values */
#define WKB_ACCESS_CODE (-1)

HWKBENTRY WKAPIENTRY WkbOpen2(HWKBSYSTEM hwkbsys, ULONG flCtrl,
  CONST TCHAR FAR *pszPort, LONG lFirmCode, LONG lUserCode, VOID FAR *pvCtrl);
#if defined(WK32)
  #define WkbOpen(x1, x2, x3, x4, x5, x6) WkbOpen2(x1, x2, x3, x4, x5, x6)
#else
  /* compatibility with API 1.x */
  USHORT WKAPIENTRY WkbOpen(USHORT usPort, LONG lFirmCode, LONG lUserCode,
    HANDLE FAR *phbox, BOXENTRY FAR *pboxentry, USHORT usDestLen);
#endif

INT WKAPIENTRY WkbClose2(HWKBENTRY hwkbe);
#if defined(WK32)
  #define WkbClose(x1) WkbClose2(x1)
#else
  /* compatibility with API 1.x */
  USHORT WKAPIENTRY WkbClose(HANDLE hbox);
#endif


/* ------------------------
 * WkbQueryEntry() function
 * ------------------------
 */
/* command codes in <flCtrl> of WkbQueryEntry2() */
#define WKB_QE_OPEN       0x0000 /* query information about opened entry */
#define WKB_QE_BOX        0x0010 /* query information about used box entry */
#define WKB_QE_MULTIENTRY 0x0020 /* multi-open variant of WKB_QE_BOX */
#define WKB_QE_MASK       0x00F0 /* command mask for <flCtrl> */

INT WKAPIENTRY WkbQueryEntry2(HWKBENTRY hwkbe, ULONG flCtrl,
  VOID FAR *pvDest, UINT cbDest);
#if defined(WK32)
  #define WkbQueryEntry(x1, x2, x3, x4) WkbQueryEntry2(x1, x2, x3, x4)
#else
  /* no part of API 1.x */
#endif

/* -------------------------------------
 * WkbSelect() & WkbUnSelect() functions
 * -------------------------------------
 */
/* command codes in <flCtrl> of WkbSelect2() */
#define WKB_SEL_DIRECT    0x0000 /* direct encryption via WIBU-BOX */
#define WKB_SEL_KNUTH20   0x0010 /* indirect encrypt. by WIBU-BOX 1 algor. */
#define WKB_SEL_DEC_FEAL  0x0020 /* indirect decryption by original FEAL */
#define WKB_SEL_ENC_FEAL  0x0030 /* indirect encryption by original FEAL */
#define WKB_SEL_DEC_PERM  0x0040 /* indirect decryption via permutation */
#define WKB_SEL_ENC_PERM  0x0050 /* indirect encryption via permutation */
#define WKB_SEL_SYMFEAL   0x0060 /* symmetric FEAL indirect encryption */
#define WKB_SEL_USE_CACHE 0x00F0 /* use cached indirect selection data */
#define WKB_SEL_MASK      0x00F0 /* command mask for <flCtrl> */

/* Selection Code Options (bits 28 to 31) */
#define WKB_SC_BOXLIMIT     0x80000000 /* reduce BOX Limit Counter if avail. */
#define WKB_SC_NETLIMIT     0x40000000 /* use Net Limit if available */
#define WKB_SC_EXPDATE      0x20000000 /* check expiration date */
#if WKAPIVER >= 3*256+0
#define WKB_SC_STATIONSHARE 0x10000000 /* check for Station Share */
#endif
#define WKB_SC_EXTMASK      0xF0000000 /* mask for get Option bits */

#define WKB_SC_COREMASK     0x0FFFFFFF /* mask to get Core bits of SC value */

INT WKAPIENTRY WkbSelect2(HWKBENTRY hwkbe, ULONG flCtrl,
  ULONG ulSelectCode, VOID FAR *pvCtrl);
#if defined(WK32)
  #define WkbSelect(x1, x2, x3, x4) WkbSelect2(x1, x2, x3, x4)
#else
  /* compatibility with API 1.x */
  USHORT WKAPIENTRY WkbSelect(HANDLE hbox, USHORT usSelectCode);
  USHORT WKAPIENTRY WkbSelectIndirect(HANDLE hbox, USHORT usSelectCode,
  USHORT usSize, BYTE FAR *pbInitBuf);
#endif

INT WKAPIENTRY WkbUnSelect2(HWKBENTRY hwkbe);
#if defined(WK32)
  #define WkbUnSelect(x1) WkbUnSelect2(x1)
#else
  /* compatibility with API 1.x */
  USHORT WKAPIENTRY WkbUnSelect(HANDLE hbox);
#endif


/* -------------------
 * WkbCrypt() function
 * -------------------
 */
/* options for <flCtrl> of WkbCrypt() */
#define WKB_CRYPT_CTRL     0x0000 /* pvCtrl is NULL or points to area data */
#define WKB_CRYPT_COPY     0x0001 /* pvCtrl points to source data */
#define WKB_CRYPT_BUFFER   0x0002 /* buffer encryption data internally */
#define WKB_CRYPT_NOBUFFER 0x0004 /* don't buffer encryption data */

INT WKAPIENTRY WkbCrypt2(HWKBENTRY hwkbe, ULONG flCtrl, VOID FAR *pvDest,
  VOID FAR *pvCtrl, UINT cbSrc, UINT FAR *pcbDest);
#if defined(WK32)
  #define WkbCrypt(x1, x2, x3, x4, x5, x6) WkbCrypt2(x1, x2, x3, x4, x5, x6)
#else
  /* compatibility with API 1.x */
  USHORT WKAPIENTRY WkbCrypt(HANDLE hbox, PVOID pvDestBuf,
    PVOID pvSrcBuf, USHORT usSrcLen, USHORT FAR *pusDestLen);
  USHORT WKAPIENTRY WkbCryptIndirect(HANDLE hbox, PVOID pvDestBuf,
    PVOID pvSrcBuf, USHORT usSrcLen, USHORT FAR *pusDestLen);
#endif


/* -------------------------------------------------------
 * WkbEnumPorts(), QkbQueryPort() & WkbSetPort() functions
 * -------------------------------------------------------
 */

/* command codes in <flCtrl> of WkbEnumPorts2() */
#define WKB_EPT_LIST 0x0000 /* return complete list as string */
#define WKB_EPT_NEXT 0x0010 /* return server list step by step */
#define WKB_EPT_MASK 0x00F0 /* command mask for <flCtrl> */

INT WKAPIENTRY WkbEnumPorts2(HWKBSYSTEM hwkbsys, ULONG flCtrl,
  TCHAR FAR *pszDest, UINT cchDest);

/* command codes in <flCtrl> of WkbQueryPort2() */
#define WKB_QP_PHYS 0x0000 /* query physical port data/structure WKBPORT */
#define WKB_QP_NAME 0x0010 /* query port name data/structure WKBPORTNAME */
#define WKB_QP_CONFIG_NAMES 0x0020 /* query port configuration names string */
#define WKB_QP_MASK 0x00F0 /* command mask for <flCtrl> */

INT WKAPIENTRY WkbQueryPort2(HWKBSYSTEM hwkbsys, ULONG flCtrl,
  CONST TCHAR FAR *pszPort, VOID FAR *pvDest, UINT cbDest);
#if defined(WK32)
  #define WkbQueryPort(x1, x2, x3, x4, x5) WkbQueryPort2(x1, x2, x3, x4, x5)
#else
  /* compatibility with API 1.x */
  USHORT WKAPIENTRY WkbQueryPort(HANDLE hacc, USHORT usLevel,
    USHORT usPort, WKBPORT FAR *pDestBuf);
#endif

/* command codes in <flCtrl> of WkbSetPort2() */
#define WKB_SP_PHYS 0x0000 /* setup physical port data/structure WKBPORT */
#define WKB_SP_MASK 0x00F0 /* command mask for <flCtrl> */

INT WKAPIENTRY WkbSetPort2(HWKBSYSTEM hwkbsys, ULONG flCtrl,
  CONST TCHAR FAR *pszPort, CONST VOID FAR *pvSetup, UINT cbSetup);
#if defined(WK32)
  #define WkbSetPort(x1, x2, x3, x4, x5) WkbSetPort2(x1, x2, x3, x4, x5)
#endif


/* ----------------------
 * WkbListPort() function
 * ----------------------
 */
INT WKAPIENTRY WkbListPort2(HWKBSYSTEM hwkbsys, ULONG flCtrl,
  CONST TCHAR FAR *pszPort, VOID FAR *pvDest, UINT cbDest);
#if defined(WK32)
  #define WkbListPort(x1, x2, x3, x4, x5) WkbListPort2(x1, x2, x3, x4, x5)
#else
  /* compatibility with API 1.x */
  USHORT WKAPIENTRY WkbListPort(USHORT usPort, BYTE FAR *pbDestBuf,
    USHORT usDestBufLen);
#endif

/* command codes in <flCtrl> of WkbListPort2() */
#define WKB_LPT_SERIAL    0x0000 /* list serial numbers of all functions */
#define WKB_LPT_MASK      0x00F0 /* command mask in <flCtrl> */


/* ---------------------
 * WkbListBox() function
 * ---------------------
 */
/* command codes in <flCtrl> of WkbListBox2() */
#define WKB_LIST_DATA     0x0000 /* list entry registers of WIBU-BOX */
#define WKB_LIST_CONFIG   0x0010 /* list configuration register of WIBU-BOX */
#if WKAPIVER >= 4*256+0
#define WKB_LIST_EXTMEM   0x0020 /* list contents of ExtMem storage pages */
#endif
#define WKB_LIST_MASK     0x00F0 /* command mask in <flCtrl> */

INT WKAPIENTRY WkbListBox2(HWKBSYSTEM hwkbsys, ULONG flCtrl,
  CONST TCHAR FAR *pszPort, CONST WKBSERIAL FAR *pwkbsrl,
  VOID FAR *pvDest, UINT cbDest);
#if defined(WK32)
  #define WkbListBox(x1,x2,x3,x4,x5,x6) WkbListBox2(x1, x2, x3, x4, x5, x6)
#else
  /* compatibility with API 1.x */
  USHORT WKAPIENTRY WkbListBox(USHORT usPort, BYTE FAR *pbSerial,
    BYTE FAR *pbDestBuf, USHORT usDestBufLen);
#endif


/* ---------------------
 * WkbProgram() function
 * ---------------------
 */
/* command codes in <flCtrl> of WkbProgram2() */
#define WKB_PGM_CLEAR        0x0000 /* entry is cleared */
#define WKB_PGM_NEW          0x0010 /* new entry is programmed */
#define WKB_PGM_ADD          0x0020 /* entry is added by data/limit entry */
#define WKB_PGM_MODIFY       0x0030 /* modify user data entry */
#define WKB_PGM_MODIFY_ADDED 0x0040 /* modify added data/limit entry */
#define WKB_PGM_CONFIG       0x0080 /* set WIBU-BOX configuration */
#define WKB_PGM_LOCK         0x0090 /* lock WIBU-BOX for programming */
#define WKB_PGM_UNLOCK       0x00A0 /* unlock locked WIBU-BOX */
#if WKAPIVER >= 4*256+0
#define WKB_PGM_EXTMEM       0x00B0 /* program ExtMem storage pages */
#endif
#define WKB_PGM_MASK         0x00F0 /* command masks in <flCtrl> */


INT WKAPIENTRY WkbProgram2(HWKBSYSTEM hwkbsys, ULONG flCtrl,
  CONST TCHAR FAR *pszPort, CONST WKBSERIAL FAR *pwkbsrl,
  CONST VOID FAR *pvPgmData);
#if defined(WK32)
  #define WkbProgram(x1, x2, x3, x4, x5) WkbProgram2(x1, x2, x3, x4, x5)
#else
  /* compatibility with API 1.x */
  USHORT WKAPIENTRY WkbProgram(USHORT usPort,
    BYTE FAR *pbSerial, BYTE FAR *pbSequence, BYTE FAR *pbStatus);
#endif

#if !defined(WK32)

/* ------------------------------------------------------
 * WkbCheckAndSetPhysPorts & WkbQueryPhysPort() functions
 * ------------------------------------------------------
 */
USHORT WKAPIENTRY WkbCheckAndSetPhysPorts(USHORT usPortBase,
  USHORT *pusCheckAddrTable, USHORT cCheckedPort);
USHORT WKAPIENTRY WkbQueryPhysPort(USHORT usPort, USHORT *pusPortAddr);

#endif /* !defined(WK32) */


/* ==============================
 * Structure Management functions
 * ==============================
 */

UINT WKAPIENTRY WkbStructSizeOf(INT nStructId);
UINT WKAPIENTRY WkbStructCalcSize(INT nStructId, UINT cbAddedArea,
  UINT cbStringArea);
BOOL WKAPIENTRY WkbStructInit(VOID FAR *pvStructBlk, INT nStructId,
  UINT cbAddedArea, UINT cbStringArea);
PVOID WKAPIENTRY WkbStructGetAddr(CONST VOID FAR *pvStructBlk);
UINT WKAPIENTRY WkbStructGetSize(CONST VOID FAR *pvStructBlk);
LONG WKAPIENTRY WkbStructGetPosition(CONST VOID FAR *pvStructBlk,
  LONG ulStructPos, INT nMemberId, INT nIndex);
LONG WKAPIENTRY WkbStructGetValue(CONST VOID FAR *pvStructBlk,
  ULONG ulStructPos, INT nMemberId);
BOOL WKAPIENTRY WkbStructSetValue(VOID FAR *pvStructBlk, ULONG ulStructPos,
  INT nMemberId, LONG lValue);
INT WKAPIENTRY WkbStructGetString(CONST VOID FAR *pvStructBlk,
  ULONG ulStructPos, INT nMemberId, TCHAR FAR *pszDest, UINT cchDest);
BOOL WKAPIENTRY WkbStructSetString(VOID FAR *pvStructBlk, ULONG ulStructPos,
  INT nMemberId, CONST TCHAR FAR *pszSrc);
UINT WKAPIENTRY WkbStructGetData(VOID FAR *pvStructBlk, ULONG ulStructPos,
  INT nMemberId, PVOID pvDest, UINT cbDest);
UINT WKAPIENTRY WkbStructSetData(VOID FAR *pvStructBlk, ULONG ulStructPos,
  INT nMemberId, CONST VOID FAR *pvSrc, UINT cbSrc);
BOOL WKAPIENTRY WkbStructGetStruct(CONST VOID FAR *pvStructBlk,
  ULONG ulStructPos, INT nMemberId, VOID FAR *pvStructBlkDest);
BOOL WKAPIENTRY WkbStructSetStruct(VOID FAR *pvStructBlk, ULONG ulStructPos,
  INT nMemberId, CONST VOID FAR *pvStructBlkSrc);


/* =========================
 * Special support functions
 * =========================
 */

ULONG WKAPIENTRY WkbBitResetSet(ULONG ulSrc, ULONG ulResetMask,
  ULONG ulSetMask);

/* ==========================================
 * Constants for Member & Structure Selection
 * ==========================================
 */

#define WKB_STM_ABCUSTOM1            1
#define WKB_STM_ABINIT               2
#define WKB_STM_RESV1                3
#define WKB_STM_RESV2                4
#define WKB_STM_ABSERIAL             5
#define WKB_STM_ABSIGNATURE          6
#define WKB_STM_AWKBBXE              7
#define WKB_STM_AWKBSR               8
#define WKB_STM_BENTRY               9
#define WKB_STM_BHIGHVERSION         10
#define WKB_STM_BLOWVERSION          11
#define WKB_STM_BSTATUS              12
#define WKB_STM_BUPDTICK             13
#define WKB_STM_BVERHIGH             14
#define WKB_STM_BVERLOW              15
#define WKB_STM_CBBASE               16
#define WKB_STM_CBBLOCK              17
#define WKB_STM_CBCUSTOMAREA         18
#define WKB_STM_CBINIT               19
#define WKB_STM_MAXBLOCK             20
#define WKB_STM_MAXDATA              21
#define WKB_STM_CBOX                 22
#define WKB_STM_CBSLOT               23
#define WKB_STM_CENTRY               24
#define WKB_STM_CFREEUSER            25
#define WKB_STM_CMAXUSER             26
#define WKB_STM_CREFRESHERROR        27
#define WKB_STM_DAY                  28
#define WKB_STM_SDACCESS             29
#define WKB_STM_USDLOGIN             30
#define WKB_STM_USDPROT              31
#define WKB_STM_USDUPD               32
#define WKB_STM_FLCTRL               33
#define WKB_STM_FLSELCTRL            34
#define WKB_STM_FLSTDCTRL            35
#define WKB_STM_FSCTRL               36
#define WKB_STM_HOURS                37
#define WKB_STM_HWKBSYS              38
#define WKB_STM_IDNETUSER            39
#define WKB_STM_LFIRMCODE            40
#define WKB_STM_LPORT                41
#define WKB_STM_LUSERCODE            42
#define WKB_STM_MINUTES              43
#define WKB_STM_MONTH                44
#define WKB_STM_PBENCRYPTSEQ         45
#define WKB_STM_PBGAPMAP             46
#define WKB_STM_PORT                 47
#define WKB_STM_PSZAPPNAME           48
#define WKB_STM_PSZPORT              49
#define WKB_STM_PSZSERVERNAME        50
#define WKB_STM_PSZWKNETFILE         51
#define WKB_STM_PVCTRL               52
#define WKB_STM_SZSYSTEMNAME         53
#define WKB_STM_TWOSECS              54
#define WKB_STM_ULACCSECURITY        55
#define WKB_STM_ULBOXLISTADDR        56
#define WKB_STM_ULCACHEID            57
#define WKB_STM_ULCONFIG             58
#define WKB_STM_ULCRC                59
#define WKB_STM_ULCUSTOMADDR         60
#define WKB_STM_ULD1                 61
#define WKB_STM_ULD2                 62
#define WKB_STM_ULFC                 63
#define WKB_STM_ULFIRMCODE           64
#define WKB_STM_ULPROGRAMCOUNT       65
#define WKB_STM_ULRE                 66
#define WKB_STM_ULRE2                67
#define WKB_STM_ULRE3                68
#define WKB_STM_ULSC                 69
#define WKB_STM_ULSELECTCODE         70
#define WKB_STM_ULUC                 71
#define WKB_STM_ULUPDSECURITY        72
#define WKB_STM_ULUSERCODE           73
#define WKB_STM_USCMAXUSER           74
#define WKB_STM_USCONTROL            75
#define WKB_STM_USDRVINT             76
#define WKB_STM_USENTRY              77
#define WKB_STM_USFILL               78
#define WKB_STM_USFLAGS              79
#define WKB_STM_USLOGINSCRIPT        80
#define WKB_STM_USMASK               81
#define WKB_STM_USMAXUSER            82
#define WKB_STM_USPORT               83
#define WKB_STM_USREFRESHTIME        84
#define WKB_STM_USRE                 85
#define WKB_STM_USRE1                86
#define WKB_STM_STACCESS             87
#define WKB_STM_USTLOGIN             88
#define WKB_STM_USTPROT              89
#define WKB_STM_USTUPD               90
#define WKB_STM_USVERSION            91
#define WKB_STM_WKBBXE               92
#define WKB_STM_YEAR                 93
#define WKB_STM_ULSERIAL             94
#define WKB_STM_WKBSERIAL            95
#define WKB_STM_ULADDRESS            96
#define WKB_STM_ABRE1                97
#define WKB_STM_ABRE2                98
#define WKB_STM_ULENTRYSKIP          99
#define WKB_STM_USMAXSTARTDELTA     100
#define WKB_STM_USSUBVERSION        101
#define WKB_STM_USBUILD             102
#define WKB_STM_USCOUNT             103
#define WKB_STM_USYEAR              104
#define WKB_STM_BMONTH              105
#define WKB_STM_BDAY                106


/* the following constants are the structure enumerators,
   they are also used for the little/big endian converting
*/
#define WKB_STS_BASE             192
#define WKB_STS_ACCESS           (WKB_STS_BASE + 0)
#define WKB_STS_AREA             (WKB_STS_BASE + 1)
#define WKB_STS_BOXCONFIG        (WKB_STS_BASE + 2)
#define WKB_STS_BOXDATA          (WKB_STS_BASE + 3)
#define WKB_STS_BOXENTRY         (WKB_STS_BASE + 4)
#define WKB_STS_BOXLIST          (WKB_STS_BASE + 5)
#define WKB_STS_OPENENTRY        (WKB_STS_BASE + 6)
#define WKB_STS_PORT             (WKB_STS_BASE + 7)
#define WKB_STS_PORTNAME         (WKB_STS_BASE + 8)
#define WKB_STS_PROGRAM          (WKB_STS_BASE + 9)
#define WKB_STS_SELECT           (WKB_STS_BASE + 10)
#define WKB_STS_SERIAL           (WKB_STS_BASE + 11)
#define WKB_STS_STATUS           (WKB_STS_BASE + 12)
#define WKB_STS_SYSTEM           (WKB_STS_BASE + 13)
#define WKB_STS_NET_BASE         (WKB_STS_BASE + 14)
#define WKB_STS_NET_SLOT         (WKB_STS_BASE + 15)
#define WKB_STS_OPENFIND         (WKB_STS_BASE + 16)
#define WKB_STS_VERSION          (WKB_STS_BASE + 17)
#define WKB_STS_VERSION3         (WKB_STS_BASE + 18)
#define WKB_STS_LAST WKB_STS_VERSION


/* ========================
 * WIBU-KEY-API error codes
 * ========================
 */

/* general error codes */
#define WKB_NO_ERROR 0
#define WKB_ERROR_INVALID_HANDLE 1
#define WKB_ERROR_NO_MORE_HANDLES 2
#define WKB_ERROR_INVALID_PORT 3
#define WKB_ERROR_INVALID_PARAMETER 4
#define WKB_ERROR_BUFFER_OVERFLOW 5
#define WKB_ERROR_LOCK_VIOLATION 6
#define WKB_ERROR_NOT_READY 7
#define WKB_ERROR_NOT_CONNECTED 8
#define WKB_ERROR_INVALID_FUNCTION 9
#define WKB_ERROR_NOT_CRYPTIC 10
#define WKB_ERROR_OPTION_NOT_SUPPORTED 11
#define WKB_ERROR_MEMORY_TOO_SMALL 12
#define WKB_ERROR_INVALID_VERSION 13
#define WKB_ERROR_INVALID_LEVEL 14
#define WKB_ERROR_CRC_VERIFY 15

/* WIBU-BOX specific (physical) error codes */
#define WKB_ERROR_NOT_SELECTED 16
#define WKB_ERROR_ENTRY_NOT_FOUND 17
#define WKB_ERROR_SERIAL_NOT_FOUND 18
#define WKB_ERROR_ENTRY_NOT_CHANGED 19
#define WKB_ERROR_INVALID_ENTRY 20
#define WKB_ERROR_BAD_SIGNATURE 21
#define WKB_ERROR_POWER_TOO_SMALL 22
#define WKB_ERROR_ALREADY_SELECTED 23
#define WKB_ERROR_BOXLIMIT_IS_ZERO 24
#define WKB_ERROR_PROGRAM_VERIFY 25
#define WKB_ERROR_ALGCFG_MEMORY 26
#define WKB_ERROR_ALGORITHM_NOT_SUPPORTED 27
#define WKB_ERROR_BAD_EXTMEM 28
#define WKB_ERROR_INVALID_EXTMEM_PAGE 29

/* WIBU-KEY driver specific error codes */
#define WKB_ERROR_PORT_IS_BUSY 32
#define WKB_ERROR_INVALID_SELECTION 33
#define WKB_ERROR_ENTRY_ALREADY_USED 34
#define WKB_ERROR_OUT_OF_DATE 35
#define WKB_ERROR_BAD_ADDRESS 36
#define WKB_ERROR_INVALID_COMMAND 37
#define WKB_ERROR_INVALID_SUBSYSTEM 38
#define WKB_ERROR_DATA_TOO_COMPLEX 39
#define WKB_ERROR_EXCLUSIVE_MODE_CONFLICT 40
#define WKB_ERROR_NO_USER_SET 41

/* asynchronous execution error codes */
#define WKB_ERROR_IN_ACTION 64
#define WKB_ERROR_UNDEFINED_ACTION 65
#define WKB_ERROR_TOO_MANY_ACTIONS 66
#define WKB_ERROR_SUBSYSTEM_IN_ACTION 67

/* WkLAN & WkNet specific error codes */
#define WKB_ERROR_NO_NET_SYSTEM 96
#define WKB_ERROR_NET_SEND_FAULT 97
#define WKB_ERROR_NET_RECEIVE_FAULT 98
#define WKB_ERROR_NET_NOT_READY 99
#define WKB_ERROR_NET_IS_BUSY 100
#define WKB_ERROR_BAD_NET_SYSTEM 101
#define WKB_ERROR_NET_LIMIT_IS_ZERO 102
#define WKB_ERROR_NET_USER_CANCELLED 103
#define WKB_ERROR_NET_DATA_TOO_COMPLEX 104
#define WKB_ERROR_SERVER_NOT_FOUND 105

/* system specific error codes */
#define WKB_ERROR_NO_KRNL_DRIVER 128
#define WKB_ERROR_KRNL_COMMUNICATION 129


/* =========================================
 * WkNet - WIBU-KEY universal network system
 * Declarations for structure of shared file
 * =========================================
 */

/* server file base header: starts at position 0x20 */
typedef struct {
  /* 32 bytes uncryptic static part (file information) */
  BYTE   abSignature[8];  /* 32: contains: 0x1A, 0, 0xFF, "WKNET" */
  BYTE   bHighVersion;    /* 40: contains currently 1 */
  BYTE   bLowVersion;     /* 41: contains currently 0x20=32 */
  USHORT cbBase;          /* 42: number of bytes in WKNET_BASE structure */
  USHORT usMaxUser;       /* 44: max. concurrent users in range 1..255 */ 
  USHORT cbSlot;          /* 46: number of bytes in WKNET_SLOT structure */
  USHORT cbMaxBlock;      /* 48: max. length of encryption block (0..255) */
  USHORT cbCustomArea;    /* 50: length of custom area (only 2.0) */
  ULONG  ulCustomAddr;    /* 52: address of custom specific area (only 2.0) */
  ULONG  ulBoxListAddr;   /* 56: address of WIBU-BOX contents area */
  USHORT usDateUpd;       /* 60: last date, written by resident process */
  USHORT usTimeUpd;       /* 62: last time, written by resident process */
  /* 32 bytes dynamic user and update management (encrypted) */
  USHORT usCMaxUser;      /* 64: number of top concurrent user runs */
  USHORT usDateProt;      /* 66: last date, written by resident process */
  USHORT usTimeProt;      /* 68: last time, written by resident process */
  BYTE   bUpdTick;        /* 70: resident process update tick value */
  BYTE   abReserved2[5];  /* 71: reserved area 2 */
  BYTE   abCustom1[16];   /* 76: custom specific area (encrypted) */
  ULONG  ulUpdSecurity;   /* 92: security data for update/user management */
} WKNET_BASE; /* prefix "wkntb" */

/* user-slot constants */
enum {
  WKNET_CRYPT = 250, /* data available for application to encrypt/decrypt */
  WKNET_WKLAN_USED = 253, /* slot is allocated by WkLAN (new with V2.31) */
  WKNET_LOCK = 254,  /* slot is locked by the maximum-user mechanism */
  WKNET_FREE = 255   /* slot can be used by user-call of application */
};

/* server file slot header */
/* each slot has a length of <cbSlot+cbMaxBlock> bytes */
typedef struct {
  BYTE   szSystemName[16]; /* 0: name of the system which is used */
  USHORT usDateLogIn;    /* 16: date of login */
  USHORT usTimeLogIn;    /* 18: time of login */
  USHORT usDateAccess;   /* 20: date of last access */
  USHORT usTimeAccess;   /* 24: time of last access */
  /* access part (encrypted in dependence of fdateAccess/usTimeAccess) */
  USHORT usLogInScript;  /* 24: login script of this user */
  BYTE   abReserved1[2]; /* 26: reserved area 1 */
  ULONG  ulFC;           /* 28: Firm Code for encryption/decryption */
  ULONG  ulUC;           /* 32: User Code for encryption/decryption */
  ULONG  ulSC;           /* 36: Selection Code for encryption/decryption */
  USHORT cbBlock;        /* 40: length of encryption/decryption block */
  USHORT usControl;      /* 42: control codes for encryption/decryption */
  ULONG  ulAccSecurity;  /* 44: security code for access part */
  /* status part (uncryptic) */
  BYTE   bStatus;        /* 48: encryption/decryption status */
  BYTE   abReserved2[7]; /* 49: reserved area 2 */
} WKNET_SLOT; /* prefix "wknts" */

/* constants for <usControl> */
enum {
  WKNET_CTRL_VERSION2 = 1,  /* use WKB_VERSION2 instead WKB_VERSION1 */
  WKNET_CTRL_MULTIFIND = 2, /* use WKB_OPEN_MULTIFIND instead WKB_OPEN_FIND */
  WKNET_CTRL_MASK = 0x0003
};

#if defined(WK32)

/* ====================================
 * This section contains structures,
 * which describes used structures
 * of the internal WIBU-KEY Network API
 * ====================================
 */

/* level specification */
#define WKM_LEVEL0  0x0000 /* std. level since WIBU-KEY API version 2.40 */
#define WKM_LEVEL1  0x0100 /* reserved level for future use */

/* definition of <flCtrl> standard flags */
#if !defined(WKM_STDCTRL)
  #if defined(_UNICODE)
    #define WKM_STDCTRL (WKB_UNICODE|WKB_SYNCHRONOUS|WKM_LEVEL0)
  #else
    #define WKM_STDCTRL (WKB_SYNCHRONOUS|WKM_LEVEL0)
  #endif
#endif

/* -------------------
 * WKBIPADDR structure
 * -------------------
 * manages a IPv6 compatible 128 bit internet address
 * used by WKMSERVER & WKMUSER
 */
typedef struct  {
  USHORT aus[8]; /* specifies 8 16 bit values which store a 128 bit internet address */
} WKUNALIGNED WKBIPADDR; /* prefix "wkbia" */

/* ----------------------
 * WKBTIMESTAMP structure
 * ----------------------
 * stores a SYSTEMTIME compatible time stamp in little endian
 * used by WKMUSER
 */
typedef struct {
  USHORT wYear;         /* contains the year value in 4 digits */
  USHORT wMonth;        /* contains the month value started with 1 for January */
  USHORT wDayOfWeek;    /* contains the day of the week, started with 0 for Sunday */
  USHORT wDay;          /* contains the day value */
  USHORT wHour;         /* contains the hour value */
  USHORT wMinute;       /* contains the minume value */
  USHORT wSecond;       /* contains the second value */
  USHORT wMilliseconds; /* contains the milliseconds value */
} WKUNALIGNED WKBTIMESTAMP; /* prefix "wkbts" */

/* -------------------
 * WKMSERVER structure
 * -------------------
 * describes the properties of a WIBU-KEY server process
 * returned by WkmQueryNetServer()
 */
typedef struct {
  ULONG ulCtrl;          /* specifies several flags for specific status information */
  ULONG ofszName;        /* contains a byte offset to the server name */
  ULONG ofszDescription; /* contains a byte offset to an additional description name of the server */
  WKBIPADDR wkbia;       /* contains the internet address of the server */
  ULONG idUpdate;        /* contains the current server update count value */
  USHORT cAvailCluster;   /* contains the number of available clusters */
  USHORT cUsedCluster;    /* contains the number of used clusters */
} WKUNALIGNED WKMSERVER; /* prefix "wkmsrv" */

/* -----------------------
 * WKMCLUSTERMAP structure
 * -----------------------
 * describes the properties of a WIBU-KEY network clustermap
 * returned by WkmQueryNetServer()
 */
typedef struct {
  ULONG ulCtrl;   /* specifies several flags for specific status information */
  ULONG idUpdate; /* contains the current server update count value */
  BYTE abMap[4];  /* contains the array which contains the allocation bits, dynamic length */
} WKUNALIGNED WKMCLUSTERMAP; /* prefix "wkmclm" */

/* --------------------
 * WKMCLUSTER structure
 * --------------------
 * describes the properties of a WIBU-KEY network cluster
 * used by WKMCLUSTER
 */
/* options for <ulCtrl> of WKMCLUSTER structure */
#define WKM_CLUSTER_MASTER_CODE  0x0001 /* the specified User Code is a Master Code */
#define WKM_CLUSTER_SHARED       0x0002 /* the cluster is shared with another */
#define WKM_CLUSTER_WKNET_USE    0x0004 /* the cluster is used by WkNet */
#define WKM_CLUSTER_HLM          0x0008 /* the cluster is defined by HLM */
#define WKM_CLUSTER_EXPIRED      0x0010 /*  some or all slots of the HLM are invalidated*/

typedef struct {
  ULONG ulCtrl;          /* specifies several flags, WKM_CLUSTER_... flags used */
  ULONG ofszName;        /* contains a byte offset to the cluster name */
  ULONG ulFirmCode;      /* contains the Firm Code which controls the cluster */
  ULONG ulUserCode;      /* contains the User Code which controls the cluster */
  USHORT idCluster;      /* contains the identification of the cluster */
  USHORT cMaxUser;       /* contains the number of users which are supported by the cluster */
  USHORT cFreeUser;      /* contains the number of users which may be allocated by WkLAN or WkNet */
  USHORT cFreeUserWkNet; /* contains the number of users which may be allocated by WkNet */
} WKUNALIGNED WKMCLUSTER; /* prefix "wkmclu" */

/* ------------------------
 * WKMCLUSTERLIST structure
 * ------------------------
 * describes a list of several WKMCLUSTER structures
 * returned by WkmQueryNetCluster()
 */
typedef struct {
  ULONG ulCtrl;          /* specifies several flags, WKM_CLUSTER_... flags used */
  ULONG idUpdate;        /* contains the current server update count value */
  USHORT cEntry;         /* contains the number of entries which are stored here */
  USHORT cSubsequent;    /* contains the number of clusters */
  WKMCLUSTER awkmclu[1]; /* contains an WKMCLUSTER-array, dynamic length */
} WKUNALIGNED WKMCLUSTERLIST; /* prefix "wkmclul" */

/* --------------------
 * WKMUSERMAP structure
 * --------------------
 * describes a bitmap which specifies the allocated users of a single cluster
 * used by WKMUSERMAPLIST
 */
typedef struct {
  USHORT cbMap;     /* contains the length of the structure */
  USHORT idCluster; /* contains the identification of the cluster */
  BYTE abMap[4];    /* contains the array which contains the allocation bits, dynamic length */
} WKUNALIGNED WKMUSERMAP; /* prefix "wkmusm" */

/* ------------------------
 * WKMUSERMAPLIST structure
 * ------------------------
 * describes a list of several WKMUSERMAP structures
 * returned by WkmQueryNetCluster()
 */
typedef struct {
  ULONG ulCtrl;          /* specifies several flags */
  ULONG idUpdate;        /* contains the current cluster update count value */
  USHORT cEntry;         /* contains the number of entries */
  USHORT cSubsequent;    /* contains the number of clusters */
  WKMUSERMAP awkmusm[1]; /* contains an WKMUSERMAP-array of different, dynamic length */
} WKUNALIGNED WKMUSERMAPLIST; /* prefix "wkmusml" */

/* -----------------
 * WKMUSER structure
 * -----------------
 * describes the properties of a WIBU-KEY network user
 * used by WKMUSERLIST
 */
/* options for <ulCtrl> of WKMCLUSTER structure */
#define WKM_USER_WKNET           0x0001 /* the user has accessed by WkNet */
#define WKM_USER_RESERVED        0x0002 /* the slot is allocated preliminary for WkLAN */
#define WKM_USER_OVERFLOW        0x0004 /* the slot is used as overflow cluster of another slot */
#define WKM_USER_CANCELLED       0x0008 /* the user has been cancelled (delayed) */
#define WKM_USER_EXCLUSIVE       0x0010 /* the user has started the application exclusively */
#define WKM_USER_STATIONSHARE    0x0020 /* the user has started the application stationshared */

typedef struct {
  ULONG ulCtrl;             /* specifies several flags, WKM_USER_...-flags used */
  ULONG ofszUserName;       /* contains a byte offset to the name of the network user */
  ULONG ofszAppName;        /* contains a byte offset to the application */
  ULONG ofszComputer;       /* contains a byte offset to the name of the client computer */
  WKBIPADDR wkbiaComputer;  /* contains the internet address of the computer */
  USHORT idUser;            /* contains the identification of the user */
  USHORT cSharedConnection; /* contains the number of shared connections of this user (slot) */
  ULONG idConnection;       /* contains the connection identification */
  WKBTIMESTAMP wkbtsLogon;  /* contains the logon time of the user at the server */
  WKBTIMESTAMP wkbtsAccess; /* contains the access time of the user at the server */
} WKUNALIGNED WKMUSER; /* prefix "wkmusr" */

/* ---------------------
 * WKMUSERLIST structure
 * ---------------------
 * describes a list of several WKMUSER structures
 * returned by WkmQueryNetUser()
 */
typedef struct {
  ULONG ulCtrl;       /* specifies several flags */
  ULONG idUpdate;     /* contains the current cluster update count value */
  USHORT cEntry;      /* contains the number of entries */
  USHORT cSubsequent; /* contains the number of users */
  WKMUSER awkmusr[1]; /* contains an WKMUSER-array of different, dynamic length */
} WKUNALIGNED WKMUSERLIST; /* prefix "wkmusrl" */

/* =======================================================================
 * WIBU-KEY Network Management Structures, Constants, Macros and Functions
 * =======================================================================
 */
/* options in <flCtrl> of WkmQueryNetCluster & WkmQueryNetUser() */
#define WKM_SINGLE_OBJECT    0x00 /* returns only a single object within a list structure (default) */
#define WKM_LIST_OF_OBJECTS  0x08 /* returns a list of several clusters */

/* ----------------------------
 * WkmQueryNetServer() function
 * ----------------------------
 */
/* command codes in <flCtrl> of WkmQueryNetServer() */
#define WKM_QNS_PROPERTY     0x00 /* returns server properties as WKMSERVER */
#define WKM_QNS_CLUSTERMAP   0x10 /* returns server clusters within a WKMCLUSTERMAP */
#define WKM_QNS_MASK         0xF0 /* command mask for <flCtrl> */

INT WKAPIENTRY WkmQueryNetServer(HWKBSYSTEM hwkbsys, ULONG flCtrl,
  void FAR* pvDest, UINT cbDest);

/* -----------------------------
 * WkmQueryNetCluster() function
 * -----------------------------
 */
/* command codes in <flCtrl> of WkmQueryNetCluster() */
#define WKM_QNC_PROPERTY     0x00 /* returns server properties as WKMCLUSTERLIST */
#define WKM_QNC_USERMAP      0x10 /* returns server clusters within a WKMUSERMAPLIST */
#define WKM_QNC_MASK         0xF0 /* command mask for <flCtrl> */

INT WKAPIENTRY WkmQueryNetCluster(HWKBSYSTEM hwkbsys, ULONG flCtrl,
  UINT idCluster, void FAR* pvDest, UINT cbDest);

/* --------------------------
 * WkmQueryNetUser() function
 * --------------------------
 */
/* command codes in <flCtrl> of WkmQueryNetUser() */
#define WKM_QNU_PROPERTY     0x00 /* returns server properties as WKMUSERLIST */
#define WKM_QNU_MASK         0xF0 /* command mask for <flCtrl> */

INT WKAPIENTRY WkmQueryNetUser(HWKBSYSTEM hwkbsys, ULONG flCtrl,
  UINT idCluster, UINT idUser, void FAR* pvDest, UINT cbDest);

#endif

/* =======================================================
 * WIBU-KEY Automatic EXE Area Encryption Technique (AXAN)
 * Structures, Constants and Macros
 * =======================================================
 */

/* structure to encrypt a code/data area */
typedef struct {
  ULONG ulFlags;           /* flags to control the code encryption */
  WKBAREA FAR *pwkbar;     /* pointer to controlled WKBAREA structure */
  WKBOPENFIND FAR *pwkbof; /* pointer to controlled WKBOPENFIND structure */
  VOID FAR *pvDest;        /* pointer to encrypted data area */
  ULONG cbDest;            /* length of encrypted data area */
} WKUNALIGNED WKBAXANCODE; /* prefix "wkbaxc" */

/* structure to encrypt a Windows resource */
typedef struct {
  ULONG ulFlags;           /* flags to control the resource encryption */
  WKBAREA FAR *pwkbar;     /* pointer to controlled WKBAREA structure */
  WKBOPENFIND FAR *pwkbof; /* pointer to controlled WKBOPENFIND structure */
  TCHAR FAR *pszName;      /* pointer to the name of the ressource */
  TCHAR FAR *pszType;      /* pointer to the type of the ressource */
  ULONG ulLanguage;        /* language type, not used on 16-bit Windows */
  ULONG ulOffset;          /* length of offset from begin of resource data */
  ULONG cbDest;            /* length of encrypted data area, */
                           /*   CB_WKBAXAN_UNSPEC (-1)  <=> maximum */
} WKUNALIGNED WKBAXANRSRC; /* prefix "wkbaxr" */

/* flags for <ulFlags> */
#define WKBAXAN_SC_FCUC    0 /* use FC/UC related Selection Codes */
#define WKBAXAN_SC_FIX     1 /* use specified Selection Codes */
#define WKBAXAN_SC_RANDOM  2 /* use randomized Selection Codes */
#define WKBAXAN_SC_MASK    3 /* mask for Selection Codes */
#define WKBAXAN_NO_CHKCRC  8 /* no WKB_[AREA|SEL]_CHKCRC setting */
#define WKBAXAN_FC_SET     0 /* set FC in WKBOPENFIND structure */
#define WKBAXAN_FC_FIX    16 /* FC in WKBOPENFIND structure specified */
#define WKBAXAN_UC_SET     0 /* set FC in WKBOPENFIND structure */
#define WKBAXAN_UC_FIX    32 /* UC in WKBOPENFIND structure specified */

/* flags for <ulFlags> of WKBAXANRSRC structure */
#define WKBAXAN_RSRC_RAW     0x0000 /* std. resource, interpr. as byte seq. */
#define WKBAXAN_RSRC_ASTRING 0x0100 /* single string for ASCII decryption */
#define WKBAXAN_RSRC_WSTRING 0x0200 /* single string for UNICODE decryption */

/* special value for <cbDest> in the WKBAXANRSRC structure */
#define CB_WKBAXAN_UNSPEC ((ULONG)-1)

typedef struct {
  USHORT cbHeader;        /* length of members until begin of <apwkbaxc> */
  USHORT cbEntry;         /* contains sizeof(WKBAXANCODE) */
  /* WKBAXANCODE awkbaxc[n]; (variable array), end by 0 entry */
} WKUNALIGNED WKBAXANCODETABLE; /* prefix "wkbaxct" */

typedef struct {
  USHORT cbHeader;        /* length of members until begin of <apwkbaxc> */
  USHORT cbEntry;         /* contains sizeof(WKBAXANRSRC) */
  /* WKBAXANRSRC awkbaxr[n]; (variable array), end by 0 entry */
} WKUNALIGNED WKBAXANRSRCTABLE; /* prefix "wkbaxrt" */

typedef struct _WKBAXANHEAD {
  USHORT cbHeader;        /* length of members until begin of <apwkbart> */
  USHORT usReserved;      /* currently not used */
  CHAR achSignature[8];   /* optional signature for finding by WKCRYPT scan */
  ULONG  ulFlags;         /* flags to control the area encryption */
  struct _WKBAXANHEAD FAR *pwkbaxhItself; /* pointer to structure itself */
  /* dynamic length array of pointers to table descriptions follows */
  /* WKBAXAN[CODE|RSRC]TABLE *apwkbar[c|r]t[n]; */
  /* end defined by 0 pointer */
} WKUNALIGNED WKBAXANHEAD; /* prefix "wkbaxh" */

/* support macros to simplify use of AXAN structures */
#define WKB_BEGIN_AXAN_HEAD(Name, Signature, Flags) \
struct {\
  WKBAXANHEAD wkbaxh;\
  union {\
    WKBAXANCODETABLE FAR *pwkbact;\
    WKBAXANRSRCTABLE FAR *pwkbart;\
  } a[];\
} wkbaxh##Name = {\
  {sizeof(WKBAXANHEAD), 0, Signature, Flags, &wkbaxh##Name.wkbaxh},

#define WKB_AXAN_CODE_TABLE(t) &wkbaxct##t.wkbaxct,
#define WKB_AXAN_RSRC_TABLE(t) &wkbaxrt##t.wkbaxrt,

#define WKB_END_AXAN_HEAD() \
  NULL};

#define WKB_BEGIN_AXAN_CODE_TABLE(Name) \
struct {\
  WKBAXANCODETABLE wkbaxct;\
  WKBAXANCODE wkbaxc[];\
} wkbaxct##Name = {\
  {sizeof(WKBAXANCODETABLE), sizeof(WKBAXANCODE)},{

#define WKB_END_AXAN_CODE_TABLE() \
{0, NULL, NULL, NULL, 0}}};

#define WKB_AXAN_CODE(Dest, Len, Area, Open, Flags) \
{Flags, Area, Open, Dest, Len},

#define WKB_BEGIN_AXAN_RSRC_TABLE(Name) \
struct {\
  WKBAXANRSRCTABLE wkbaxrt;\
  WKBAXANRSRC wkbaxr[];\
} wkbaxrt##Name = {\
  {sizeof(WKBAXANRSRCTABLE), sizeof(WKBAXANRSRC)},{

#define WKB_END_AXAN_RSRC_TABLE() \
{0, NULL, NULL, NULL, NULL, 0, 0}}};

#define WKB_AXAN_RSRC(Name, Type, Lang, Offset, Len, Area, Open, Flags) \
{Flags, Area, Open, Name, Type, Lang, Offset, Len},

#if defined(_WINDOWS)

/* ==========================================
 * AXAN functions to load encrypted resources
 * ==========================================
 */

HGLOBAL WKAPIENTRY WkbLoadAxanResource(HWKBENTRY hwkbe, WKBAREA *pwkbar,
  ULONG ulOffset, ULONG cbData, HMODULE hmod, HRSRC hResInfo);

#if defined(_WIN32)

INT WKAPIENTRY WkbLoadAxanStringA(HWKBENTRY hwkbe, WKBAREA *pwkbar,
  HINSTANCE hInstance, UINT uID, CHAR *pszBuffer, INT cchBuffer);
INT WKAPIENTRY WkbLoadAxanStringW(HWKBENTRY hwkbe, WKBAREA *pwkbar,
  HINSTANCE hInstance, UINT uID, WCHAR *pszBuffer, INT cchBuffer);

#if defined(_UNICODE)
  #define WkbLoadAxanString(x1, x2, x3, x4, x5, x6) \
    WkbLoadAxanStringW(x1, x2, x3, x4, x5, x6)
#else
  #define WkbLoadAxanString(x1, x2, x3, x4, x5, x6) \
    WkbLoadAxanStringA(x1, x2, x3, x4, x5, x6)
#endif /* #else #if defined(_UNICODE) */

#else

INT WKAPIENTRY WkbLoadAxanString(HWKBENTRY hwkbe, WKBAREA *pwkbar,
  HINSTANCE hInstance, UINT uID, CHAR *pszBuffer, INT cchBuffer);

#endif /* #else #if defined(_WIN32) */

#endif /* #if defined(_WINDOWS) */

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* !defined(WK_INCLUDED) */
