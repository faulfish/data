/* ---- wkext.h --------------------------------------------------------------
 * WIBU-KEY Tools/Setup ANSI-C Header File.
 * (C) Copyright 2004-2006 by WIBU-SYSTEMS AG
 * D-76137 Karlsruhe, Federal Republic of Germany
 * Version 5.20 of 2006Nov13
 * ---------------------------------------------------------------------------
 */

/* ============================================================
 * WIBU-KEY system types and macros for the WkExt control
 * ============================================================
 */

#if !defined(_WKEXT_INCLUDED)
#define _WKEXT_INCLUDED

#include <wk.h>

#if !defined(_WKEXT_NODEFLIB)
  /* link dynamic lib/dll */
  #if defined(_WIN64)
    #pragma comment(lib, "WkExt64.lib")
  #elif defined(_WIN32)
    #pragma comment(lib, "WkExt32.lib")
  #endif
#endif /* #if !defined(_WKEXT_NODEFLIB) */

#if defined(__cplusplus)
extern "C" {            /* wkext.h contains only C declarations */
#endif

/* error codes, returned by WkxGetLastError() */
#define WKX_NO_ERROR                      0
#define WKX_ERROR_RTCWRITE               21
#define WKX_ERROR_RTUREAD                22

#define WKX_ERROR_INVALID_PARAMETER     107
#define WKX_ERROR_NOT_IMPLEMENTED       108
#define WKX_ERROR_ABORT                 109
#define WKX_ERROR_BUFFER_TOO_SMALL      110
#define WKX_ERROR_NO_ACCESS_DRIVER      209
#define WKX_ERROR_UNKNOWN               400

#if defined(USE_OBSOLETE_WKX)
  #define WKX_ERROR_ILLEGAL_CHARACTER      13
  #define WKX_ERROR_PROFILE_WRITE          14
  #define WKX_ERROR_PROFILE_READ           15
  #define WKX_ERROR_NOT_SUPPORTED          16
  #define WKX_ERROR_VALUE_NOT_FOUND        17
  #define WKX_ERROR_NOUSERRIGHT            30
  #define WKX_ERROR_INTERNAL_ERROR        100
  #define WKX_ERROR_REBOOT                104
  #define WKX_ERROR_WRONG_SYSTEM          106
  #define WKX_ERROR_BUFFER_TOO_SMALL      110
  #define WKX_ERROR_REG_READ              111
  #define WKX_ERROR_REG_WRITE             112
  #define WKX_ERROR_NOTEMPFOLDER          113
  #define WKX_ERROR_FILEINUSE             200
  #define WKX_ERROR_READONLY              201
  #define WKX_ERROR_NOACCESS              202
  #define WKX_ERROR_NORENAME              203
  #define WKX_ERROR_NOCOPY                204
  #define WKX_ERROR_NODELETE              205
  #define WKX_ERROR_NOMOVE                206
  #define WKX_ERROR_FILE_NOT_FOUND        208
  #define WKX_ERROR_NOCREATE              210
  #define WKX_ERROR_NOINSTALL             211
  #define WKX_ERROR_NODEINSTALL           212
  #define WKX_ERROR_FILE_CORRUPT          213
  #define WKX_ERROR_WRONG_ARCHITECTURE    214
  #define WKX_ERROR_OLDVER                230
  #define WKX_ERROR_OLDLANGVER            231
#endif


/*****************************************************************************
WKX_CTX...                        Flags for <ulFlags> of WkxGetRemoteContext()
==============================================================================

WKX_CTX_PORT ..... pszPortSpec specifies a valid WIBU-KEY portname
// not yet implemented
WKX_CTX_FIRMCODE   pszPortSpec specifies a Firm Code
WKX_CTX_WKUSPEC .. pszPortSpec specifies a WIBU-BOX/Entry specification
                   like WKU

*****************************************************************************/

#define WKX_CTX_PORT      0x01
#define WKX_CTX_FIRMCODE  0x02
#define WKX_CTX_WKUSPEC   0x04


/*****************************************************************************
WKX_RMTPGM_                                         Remote Programming 2 Flags
==============================================================================

These contants are used in the <mflCtrl> parameter of the WkxSetRemoteUpdate2()
and WkxGetRemoteContext2() method. They define several Remote Programm object
and execution.

Type of Destination
-------------------
WKX_RMTPGM_MEMORY
    (0x000, default) The Remote Programming object is managed in a memory
    block; the address of this block is specified in the <pvDest> res. <pvSrc>
    of both functions in ASCII res. Unicode.
WKX_RMTPGM_FILE
    (0x010) The Remote Programming object is managed in a file; the complete
    path of this file is specified in the <pvDest> res. <pvSrc> of both
    functions in ASCII res. Unicode.
WKX_RMTPGM_FILE_DELETE
    (0x018) The Remote Programming object is managed in a file like
    WKX_RMTPGM_FILE, but this file is deleted after use. This option is useful
    only for Remote Update files.

Write Mode
----------
WKX_RMTPGM_OVERWRITE
    (0x000, default) The newly created update or modified context information
    is stored at the begin of the file; existing data is overwritten.
WKX_RMTPGM_APPEND
    (0x020) The newly created update or modified context information is stored
    at the end of the file; existing data is not overwritten and not
    interpreted.

Type of Data Contents
---------------------
WKX_RMTPGM_TEXT
    (0x000, default) The Remote Programming information is stored as text in
    several lines; binary information is managed with 32 different characters.
WKX_RMTPGM_BINARY
    (0x040) The Remote Programming information is stored in original
    binary information; append mode, adding of comments or different text
    styles are not supported.

* This option is ignored if the Remote Programming data is managed in a file.
  Remote Programming files are always stored in text.

Comment Addition
----------------
WKX_RMTPGM_ADD_COMMENT
    (0x000, default) In front of the packed Remote Programming data several
    lines of text explain which WIBU-BOX is described.
WKX_RMTPGM_NO_COMMENT
    (0x080) The Remote Programming information text is created
    without leading comment lines.

* This option is ignored if the Remote Programming data is stored in binary
  form.

Style of Text Data
------------------
WKX_RMTPGM_WBC_STYLE
    (0x000, default) The created Remote Programming text is stored in the WBC
    style with a unique, GUID based header description.
WKX_RMTPGM_PLAIN_STYLE
    (0x100) The created Remote Programming text is stored without a WBC style
    header. The proper file format must be detected manually by the client.
    This format is obsolete is not recommended to use when files are created.

* This option is ignored if the Remote Programming data is stored in binary
  form.

*****************************************************************************/

enum __WKX_Constances {
  WKX_RMTPGM_MEMORY = 0x000,
  WKX_RMTPGM_FILE = 0x010,
  WKX_RMTPGM_FILE_DELETE = 0x018,
  WKX_RMTPGM_OVERWRITE = 0x000,
  WKX_RMTPGM_APPEND = 0x020,
  WKX_RMTPGM_TEXT = 0x000,
  WKX_RMTPGM_BINARY = 0x040,
  WKX_RMTPGM_ADD_COMMENT = 0x000,
  WKX_RMTPGM_NO_COMMENT = 0x080,
  WKX_RMTPGM_WBC_STYLE = 0x000,
  WKX_RMTPGM_PLAIN_STYLE = 0x100
};


/*****************************************************************************
WKXSTATUS                                              Status Return Structure
==============================================================================

This structure reports the current status of the last operation.

mcbStructSize
    specifies the size of this structure.
mcBox
    contains the number of WIBU-BOXes the last operation accessed to.
mcbTransferSize
    contains the number of bytes transfered to or from the cuurent operation.

*****************************************************************************/

typedef struct {
  int mcbStructSize;
  int mcBox;
  int mcbTransferSize;
} WKXSTATUS;


/*****************************************************************************
WKXEXTMEMACCESS
==============================================================================

This structure reports the current status of the last operation.

mfsCtrl
    [tba].
miPage
    [tba].
mabSignature
    [tba].

*****************************************************************************/

typedef struct {
  unsigned short mfsCtrl;
  unsigned short miPage;
  unsigned char mabSignature[8];
} WKXEXTMEMACCESS;


/*****************************************************************************
WKXRMT_CONTEXT                                Getting Remote Context Parameter
==============================================================================

This structure sets the parameter needed for a WkxGetRemoteContext2 call.

mcbStructSize
    specifies the size of this structure.
mpszSubSystem
    points to a name of the subsystem, which is used to handle
    the Remote Programming Context Reading. If this parameter is
    NULL, the local subsystem is used.
mpszPortSpec
    points to the string with of the port where the first WIBU-BOX
    is searched. If this pointer is NULL, the first available port
    is used.
mpwkbsrFirst
    points to the Serial Number of the first WIBU-BOX which is
    searched. If this pointer is NULL, the first avaialable
    WIBU-BOX at the specified port is used.
mcBox 
    contains the number of WIBU-BOXes for which the Remote Context
    information is returned. If the number of WIBU-BOXes exceeds
    the specified port, it the list is continued at the next port.
    If more WIBU-BOXes are specified than available at the used
    subsystem, the information return stops at the last WIBU-BOX
    of the subsystem.
mflCtrl
    contains a collection of flags which control the operation.
    The flags are specified in wkext.h and starts with the
    WKX_RMTPGM_ prefix.
mfuEntries
    contains a bitmap which entry contents is stored into the
    Remote Programming context. If 0 is specified only the "empty
    entry" information is stored. If (UInt)-1 is specified, all
    available entry information is returned.
mpvDest
    points to a memory buffer which is interpreted in dependence
    of the flags in <flCtrl>: It may point to a read-only file
    name or to a buffer of <cbDest> bytes which receives the
    context information in binary or text form.
mcbDest 
    contains the number of bytes at <pvDest>. This parameter is
    ignored if <pvDest> points to a file name.
mcExtMemAccess
    [tba].
mpxmacc
    [tba].

*****************************************************************************/

typedef struct {
  int mcbStructSize;
  const char* mpszSubSystem;
  const char* mpszPortSpec;
  WKBSERIAL* mpwkbsrFirst;
  unsigned int mcBox;
  unsigned int mflCtrl;
  unsigned int mfuEntries;
  void* mpvDest;
  unsigned int mcbDest;
  unsigned int mcExtMemAccess;
  WKXEXTMEMACCESS* mpxmacc;
} WKXRMT_CONTEXT;


/*****************************************************************************
WKXRMT_UPDATE                                  Setting Remote Update Parameter
==============================================================================

This structure sets the parameter needed for a WkxSetRemoteUpdate2 call.

mcbStructSize
    specifies the size of this structure.
mpszSubSystem
    points to a name of the subsystem, which is used to handle
    the Remote Programming Update operation. If this parameter is
    NULL, the local subsystem is used.
mpszPortSpec
    points to the string with of the port where the first WIBU-BOX
    is searched for update. If this pointer is NULL, the first
    available port is used.
mpwkbsrFirst
    points to the Serial Number of the first WIBU-BOX which is
    searched. If this pointer is NULL, the first avaialable
    WIBU-BOX at the specified port is used.
mcBox
    contains the number of WIBU-BOXes for which the Remote
    Programming Update is executed. If the number of WIBU-BOXes
    exceeds the specified port, it the list is continued at the
    next port. If more WIBU-BOXes are specified than available at
    the used subsystem, the Update process stops at the last
    WIBU-BOX of the subsystem.
mflCtrl
    contains a collection of flags which control the operation.
    The flags are specified in wkext.h and starts with the
    WKX_RMTPGM_ prefix.
mpvSrc
    points to a memory buffer which is interpreted in dependence
    of the flags in <flCtrl>: It may point to a file name or to
    a buffer of <cbSrc> bytes which conaints the Remote
    Programming Update information in binary or text form.
mcbSrc 
    contains the number of bytes at <pvSrc>. This parameter is
    ignored if <pvSrc> points to a file name.

*****************************************************************************/

typedef struct {
  int mcbStructSize;
  const char* mpszSubSystem;
  const char* mpszPortSpec;
  WKBSERIAL* mpwkbsrFirst;
  unsigned int mcBox;
  unsigned int mflCtrl;
  const void* mpvSrc;
  unsigned int mcbSrc;
} WKXRMT_UPDATE;


/*****
 define of WkxGetRemoteContext
*****/
int WKAPIENTRY WkxGetRemoteContext(const char *pszPortSpec, unsigned int flCtrl,
    unsigned int fuEntries, int fOnlyEmpty, int fAppend, const char *pszRtcFile);

/*****
 define of WkxGetRemoteContext2
*****/
int WKAPIENTRY WkxGetRemoteContext2(WKXRMT_CONTEXT* pwkxRtc, WKXSTATUS* pwkxStatus);

/*****
 define of WkxSetRemoteUpdate
*****/
int WKAPIENTRY WkxSetRemoteUpdate(const char *pszRtuFile, int fDelete);

/*****
 define of WkxSetRemoteUpdate2
*****/
int WKAPIENTRY WkxSetRemoteUpdate2(WKXRMT_UPDATE* pwkxRtu, WKXSTATUS* pwkxStatus);

/*****
 define of WkxGetLastError
*****/
int WKAPIENTRY WkxGetLastError();

/*****
 define of WkxSetLastError
*****/
void WKAPIENTRY WkxSetLastError(int idError);

/*****
 define of WkxGetLastErrorText
*****/
int WKAPIENTRY WkxGetLastErrorText(ULONG flCtrl, char *pszErrorText, unsigned int cbErrorText);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* #if !defined(_WKEXT_INCLUDED) */
