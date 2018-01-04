//--------------------------------------------------------------------------------------------------
//
//   Module Name: PSTOBEXCommon.h
//
//   General Description: Declaration of OBEX common type definitions and constants.
//
//--------------------------------------------------------------------------------------------------
//
//                            Motorola Confidential Proprietary
//                               Worldwide Software Division
//                    (c) Copyright Motorola 2002-2005, All Rights Reserved
//
//
// Revision History:
//                            Modification  Tracking
// Author                     Date          Number      Description of Changes
// -------------------------  ------------  ----------  ----------------------------------------
// Daniel Imamura             03/30/05      LIBff59902  File creation
// Zhongwei Qiu                5/11/05      LIBgg09439  Fix the byte definition
// Daniel Imamura             06/03/05      LIBgg23012  Removing stdafx.h include
// Daniel Imamura             06/15/05      LIBgg11057  Removing context structure for Put and Get operations
//                                                      and adding members to structures SPUT and SGET
// Daniel Imamura             09/08/05      LIBgg08171  Added constant to support SyncML development
// Dindo Jao                  03/02/06      LIBhh93979  Change the level 8 logging error
// Daniel Imamura             01/26/06      LIBhh80257  OBEX workaround for SyncML (packet size, timeout)
// Dindo Jao                  03/02/06      LIBhh93979  Change the level 8 logging error
// Daniel Imamura             03/08/06      LIBii14868  Increasing value of CPSTOBEX_PSTREADWRITE_RETRIES
// Leandro Cardoso            03/29/07      LIBll59281  Add support to OBEX communication with LJ/EzX phones
//
//--------------------------------------------------------------------------------------------------
//                                       INCLUDE FILES
//--------------------------------------------------------------------------------------------------
#ifndef __PSTOBEXCommon_h__
#define __PSTOBEXCommon_h__

#include <afxcoll.h>

//--------------------------------------------------------------------------------------------------
//                                       EXTERNAL TYPES
//--------------------------------------------------------------------------------------------------
class CPSTOBEXHeader;

//--------------------------------------------------------------------------------------------------
//                                       DEFINES
//--------------------------------------------------------------------------------------------------
// Maximum OBEX packet size
#define CPSTOBEX_MAX_ALLOWED_PACKSIZE   4096

// This is the usual overhead size for an OBEX packet or header, described as follow:
// - Usual header overhead: 1 header opcode + 2 header length = 3 bytes
// or
// - OBEX packet overhead: 1 opcode + 2 total packet length = 3 bytes
#define CPSTOBEX_HDRS_OVERHEAD          3

// Header overhead for Get packet. Note that this is is minimum amount of bytes to
// be removed from the OBEX packet size and occurs when retrieving only the body header
// 6 = 1 byte opcode + 2 bytes packet size
//   + 1 byte Body opcode + 2 bytes Body size
#define CPSTOBEX_GET_HDRS_OVERHEAD      (2 * CPSTOBEX_HDRS_OVERHEAD)

// Maximum OBEX Get body size. Note that this is is minimum amount of bytes to
// be removed from the OBEX packet size as the name is considered to be null in this
// definition.
#define CPSTOBEX_MAX_GET_BODY_SIZE      CPSTOBEX_MAX_ALLOWED_PACKSIZE - CPSTOBEX_GET_HDRS_OVERHEAD

// Header overhead for the first Put packet. Note that this is is minimum amount
// of bytes to be removed from the OBEX packet size as the name is considered to
// be null in this definition.
// 21 = 1 byte opcode + 2 bytes packet size
//    + 5 bytes connection ID
//    + 5 bytes length header (1 + 4)
//    + 1 byte Body opcode + 2 bytes Body size
//    + 5 bytes for Name header opcode, length (2) and unicode \0 (0x00 0x00)
#define CPSTOBEX_PUT_HDRS_OVERHEAD      21

// Header overhead for the first Put packet with no Name header included.
// 16 = 1 byte opcode + 2 bytes packet size
//    + 5 bytes connection ID
//    + 5 bytes length header (1 + 4)
//    + 1 byte Body opcode + 2 bytes Body size
#define CPSTOBEX_PUT_HDRS_NONAME_OVERHEAD 16

// Maximum OBEX Put body size. Note that this is is minimum amount of bytes to
// be removed from the OBEX packet size as the name is considered to be null in this
// definition.
#define CPSTOBEX_MAX_PUT_BODY_SIZE      CPSTOBEX_MAX_ALLOWED_PACKSIZE - CPSTOBEX_PUT_HDRS_OVERHEAD

// Maximum OBEX Put body size when sending PUT packet with only 
// (OBEX opcode/total pack size) + (Body header opcode/size)
#define CPSTOBEX_MAX_PUTDATA_BODY_SIZE  (CPSTOBEX_MAX_ALLOWED_PACKSIZE - (2 * CPSTOBEX_HDRS_OVERHEAD))

// Defining a NULL value for connection ID.
#define CPSTOBEX_UNDEFINED_CONNID       0xFFFFFFFF

// Small string size
#define CPSTOBEX_SMALLSTRING_SIZE       0xFF

// Medium string size
#define CPSTOBEX_MEDIUMSTRING_SIZE      0x200

// String to store the TimeUTC characters
#define CPSTOBEX_TIMEUTC_SIZE           18

// Version of OBEX protocol
#define CPSTOBEX_VERSION                0x12

// Default Connect operation flag
#define CPSTOBEX_CONNECTFLAG            0x00

// SetPath Constants byte. From the OBEX specification:
// "3.3.6.2 The constants byte is entirely reserved at this time,
// and must be set to zero by sender and ignored by receiver."
#define CPSTOBEX_SETPATH_CONSTANTS      0x00

// Default timeout value for PSTRead/Write operations (in ms)
#define CPSTOBEX_PSTREADWRITE_TIMEOUT   5000

// Default number of retries for PSTRead/Write operations
#define CPSTOBEX_PSTREADWRITE_RETRIES   50

// Default timeout value for FindInterfaceHandle operations: 30 secs
#define CPSTOBEX_PSTFINDIHANDLE_TIMEOUT 30000

// -- Packet information
// Minimum size of a Connect response packet
#define CPSTOBEX_RESPPACK_CONNECT_MIN   7

// Minimum size of a Connect response packet
#define CPSTOBEX_RESPPACK_GET_MIN       3

// String defining the modem interface for PST
#define CPSTOBEX_STR_MOTOROLAMDM        "Motorola_MDM"

// String defining the obex interface for PST
// Please note that this will be changed in the future to something
// like Motorola_OBEX. In the meantime, we use the same as the modem.
#define CPSTOBEX_STR_MOTOROLAOBEX       "Motorola_MDM"

// Default initialization string for modem interface and its size
//                                         A   T   +   M   O   D   E   =   2   2
#define CPSTOBEX_STR_DEFAULTINIT        "\x41\x54\x2B\x4D\x4F\x44\x45\x3D\x32\x32\x0d"
#define CPSTOBEX_LEN_DEFAULTINIT        11
//                                         A   T   +   C   P   R   O   T   =   0   ,   1   .   0   ,   1   7
#define CPSTOBEX_STR_DEFAULTINIT_LJ     "\x41\x54\x2B\x43\x50\x52\x4F\x54\x3D\x30\x2C\x31\x2E\x30\x2C\x31\x37\x0d"
#define CPSTOBEX_LEN_DEFAULTINIT_LJ     18

// Sleep time after "AT+MODE=22" ot "AT+CPROT=1,1.0,17" is issued to the phone to ensure that
// OBEX mode has been initialized
#define CPSTOBEX_SLEEP_OBEXINIT         500

// Default target for folder browser service
#define CPSTOBEX_STR_FOLDERBROWSE       "\xF9\xEC\x7B\xC4\x95\x3C\x11\xD2\x98\x4E\x52\x54\x00\xDC\x9E\x09"
#define CPSTOBEX_LEN_FOLDERBROWSE       16

// File type to retrieve directory listing
//                                         x   -   o   b   e   x   /   f   o   l   d   e   r   -   l   i   s   t   i   n   g  \0
#define CPSTOBEX_STR_DIRLISTTYPE        "\x78\x2d\x6f\x62\x65\x78\x2f\x66\x6f\x6c\x64\x65\x72\x2d\x6c\x69\x73\x74\x69\x6e\x67\x00 "
#define CPSTOBEX_LEN_DIRLISTTYPE        22

// Defining the size in bytes for the attribute
#define CPSTOBEX_HVSIZE_CONNID          4
#define CPSTOBEX_HVSIZE_NAME            2

//--------------------------------------------------------------------------------------------------
//                                       MACROS
//--------------------------------------------------------------------------------------------------
// Swaping bytes in UNICODE chars. It is used to convert little endian (desktop)
// to big endian (OBEX protocol)
#define OBEX_UNICODE_CHAR_SWAP(dw) dw = ((dw & 0xFF) << 8) | ((dw & 0xFF00) >> 8)

#define OBEX_SWAP32_ENDIANESS(dw)  dw = ((dw & 0xFF) << 24) | ((dw & 0xFF00) << 8) | ((dw & 0xFF0000) >> 8) | ((dw & 0xFF000000) >> 24)

// Logs messages
#define OBEXErrorMsg(sLogMessage) \
   LogPSTMessage(NO_DEVICE_ID,LEVEL_1, sLogMessage); \
   TRACE(sLogMessage);

//--------------------------------------------------------------------------------------------------
//                                       TYPES
//--------------------------------------------------------------------------------------------------
// Collection of OBEX headers to be sent to the phone
typedef CPtrList       CPSTOBEXHeaderList;

// Connection Id returned by Phone
typedef unsigned int   OBEXCONNECTIONID;

// 8 bit unsigned value
typedef unsigned char  UINT8;

// 16 bit unsigned value
typedef unsigned short UINT16;


// -- Enumerations
// OBEX supported headers' HI
enum EPSTObexHI
{
     PSTOBEXHD_NULL     = 0xFF // "Null" HI: indicates that no valid id was assigned
   , PSTOBEXHD_NAME     = 0x01 // Name: name of the object (often a file name)
   , PSTOBEXHD_TYPE     = 0x42 // Type: type of object - e.g. text, html, binary
   , PSTOBEXHD_LENGTH   = 0xC3 // Length: the length of the object in bytes
   , PSTOBEXHD_TIME     = 0x44 // Time: date/time stamp - ISO 8601 version - preferred
   , PSTOBEXHD_TIMEDP   = 0xC4 // Time: date/time stamp - 4 byte version (deprecated, for compatibility only)
   , PSTOBEXHD_DESCRIP  = 0x05 // Description: text description of the object
   , PSTOBEXHD_TARGET   = 0x46 // Target: name of service that operation is targeted to
   , PSTOBEXHD_BODY     = 0x48 // Body: a chunk of the object body.
   , PSTOBEXHD_BODYEND  = 0x49 // End of: Body: the final chunk of the object body
   , PSTOBEXHD_WHO      = 0x4A // Who : identifies the OBEX application, used to tell if talking to a peer
   , PSTOBEXHD_OBJCLASS = 0x51 // Object: Class: OBEX Object class of object
   , PSTOBEXHD_CONNID   = 0xCB // Connection: Id: an identifier used for OBEX connection multiplexing

   // OBEX headers modifiers: High bits (bits 7 and 8).
   , PSTOBEXHD_UNICODE  = 0X00 // null terminated Unicode text, length prefixed with 2 byte unsigned integer
   , PSTOBEXHD_BYTESEQ  = 0X40 // byte sequence, length prefixed with 2 byte unsigned integer
   , PSTOBEXHD_1BYTE    = 0X80 // 1 byte quantity
   , PSTOBEXHD_4BYTES   = 0XC0 // 4 byte quantity - transmitted in network byte order (high byte first)
};

// OBEX supported operation codes (opcodes)
enum EPSTObexOpcode
{
     PSTOBEXOPCD_NULL       = 0x00 // Undefined Operation
   , PSTOBEXOPCD_CONNECT    = 0x80 // Connect: Choose your partner, negotiate capabilities
   , PSTOBEXOPCD_DISCONNECT = 0x81 // Disconnect: Signal the end of the session
   , PSTOBEXOPCD_PUT        = 0x02 // Put: Send an object
   , PSTOBEXOPCD_GET        = 0x03 // Get: Get an object
   , PSTOBEXOPCD_SETPATH    = 0x85 // SetPath: Modifies the current path on the receiving side
   , PSTOBEXOPCD_ABORT      = 0xFF // Abort: Abort the current operation
   , PSTOBEXOPCD_FINALBIT   = 0x80 // Used to sinalize that the Final bit is set by bitwise AND
                                   // with the required opcode. Please note that Connect, Disconnect
                                   // SetPath and Abort is always used with the final bit set
};

// OBEX packet type
enum EPSTObexPacketType
{
   // Response packets
     PSTOBEXPKTYPE_CONNECT = 01 // Response for Connect
   , PSTOBEXPKTYPE_DISCONNECT   // Response for Disconnect
   , PSTOBEXPKTYPE_PUT          // Response for Put
   , PSTOBEXPKTYPE_GET          // Response for Get
   , PSTOBEXPKTYPE_GET_FINFO    // Response for Get: File info
   , PSTOBEXPKTYPE_GET_CONT     // Response for Get: Continue
   , PSTOBEXPKTYPE_SETPATH      // Response for SetPath
   , PSTOBEXPKTYPE_ABORT        // Response for Abort
};

// OBEX response available codes
enum EPSTObexResponse
{
     PSTOBEXRESP_CONTINUE           = 0x90 // Continue
   , PSTOBEXRESP_OK                 = 0xA0 // OK, Success
   , PSTOBEXRESP_CREATED            = 0xA1 // Created
   , PSTOBEXRESP_ACCEPTED           = 0xA2 // Accepted
   , PSTOBEXRESP_NONAUTHINFO        = 0xA3 // Non-Authoritative Information
   , PSTOBEXRESP_NOCONTENT          = 0xA4 // No Content
   , PSTOBEXRESP_RESETCONTENT       = 0xA5 // Reset Content
   , PSTOBEXRESP_PARTIALCONTENT     = 0xA6 // Partial Content
   , PSTOBEXRESP_MULTICHOICE        = 0xB0 // Multiple Choices
   , PSTOBEXRESP_MOVEDPERM          = 0xB1 // Moved Permanently
   , PSTOBEXRESP_MOVEDTEMP          = 0xB2 // Moved temporarily
   , PSTOBEXRESP_SEEOTHER           = 0xB3 // See Other
   , PSTOBEXRESP_NOTMODIF           = 0xB4 // Not modified
   , PSTOBEXRESP_USEPROXY           = 0xB5 // Use Proxy
   , PSTOBEXRESP_BADREQUST          = 0xC0 // Bad Request - server couldn’t understand request
   , PSTOBEXRESP_UNAUTHORIZED       = 0xC1 // Unauthorized
   , PSTOBEXRESP_PAYREQUIRED        = 0xC2 // Payment required
   , PSTOBEXRESP_FORBID             = 0xC3 // Forbidden - operation is understood but refused
   , PSTOBEXRESP_NOTFOUND           = 0xC4 // Not Found
   , PSTOBEXRESP_METHODNOTALLOW     = 0xC5 // Method not allowed
   , PSTOBEXRESP_NOTACCEPT          = 0xC6 // Not Acceptable
   , PSTOBEXRESP_PROXYAUTH          = 0xC7 // Proxy Authentication required
   , PSTOBEXRESP_REQUESTTIMEOUT     = 0xC8 // Request Time Out
   , PSTOBEXRESP_CONFLICT           = 0xC9 // Conflict
   , PSTOBEXRESP_GONE               = 0xCA // Gone
   , PSTOBEXRESP_LENGTHREQUIRED     = 0xCB // Length Required
   , PSTOBEXRESP_PRECONDFAIL        = 0xCC // Precondition failed
   , PSTOBEXRESP_REQUESTENTTOOBIG   = 0xCD // Requested entity too large
   , PSTOBEXRESP_URLTOOLARGE        = 0xCE // Request URL too large
   , PSTOBEXRESP_UNSUPPORTEDMEDIA   = 0xCF // Unsupported media type
   , PSTOBEXRESP_INTERNALSRVERROR   = 0xD0 // Internal Server Error
   , PSTOBEXRESP_NOTIMPLEM          = 0xD1 // Not Implemented
   , PSTOBEXRESP_BADGW              = 0xD2 // Bad Gateway
   , PSTOBEXRESP_SERVICEUNAVAIL     = 0xD3 // Service Unavailable
   , PSTOBEXRESP_GWTIMEOUT          = 0xD4 // Gateway Timeout
   , PSTOBEXRESP_HTTPVERSIONNOTSUPP = 0xD5 // HTTP version not supported
   , PSTOBEXRESP_DBFULL             = 0xE0 // Database Full
   , PSTOBEXRESP_DBLOCK             = 0xE1 // Database Locked
};

// OBEX Operation class state.
// These states are defined in the natural order of the operation. Note that they are defined
// with the possibility of future states inclusions between them. Do not change the order of
// the states as this may lead to mal-function in the CPSTOBEXOperation class and subclasses.
enum EPSTObexOpState
{
     PSTOBEXOPSTATE_INITIAL         = 00 // Initial state, object only created
   , PSTOBEXOPSTATE_FLAGSET         = 10 // Flags for the operation have been set
   , PSTOBEXOPSTATE_HEADERSET       = 20 // Header list for the op. have been set
   , PSTOBEXOPSTATE_READYTOSEND     = 30 // Request packet is ready to be sent
   , PSTOBEXOPSTATE_REQSENT         = 40 // Request packet sent
   , PSTOBEXOPSTATE_READYTORETRIEVE = 50 // Ready to retrieve response packet
   , PSTOBEXOPSTATE_RESPRETRIEVED   = 60 // Response packet retrieved
   , PSTOBEXOPSTATE_CONTINUE        = 70 // Operation still continues
   , PSTOBEXOPSTATE_COMPLETED       = 80 // Operation completed
};

// OBEX SetPath operation flags
enum CPSTOBEX_SETPATHFLAG
{
     PSTOBEXPATH_BACKUPLEVEL   = 0x01 // Backup one directory level (same as "..")
   , PSTOBEXPATH_DONTCREATEDIR = 0x02 // Dont create the directory if it does not exist
};

// -- Structures
typedef struct _PSTOBEX_SCONNECT
{
   UINT16 m_iTargetLength;                         // 2 bytes with the target name length
   BYTE m_TargetName[CPSTOBEX_SMALLSTRING_SIZE];   // Byte sequence that identifies the target of the operation.
   UINT16 m_iMaxPackLength;                        // Maximum OBEX packet size
} PSTOBEX_SCONNECT;

typedef struct _PSTOBEX_SABORT
{
   UINT32 m_iConnId;                               // 4-byte Connection Id
} PSTOBEX_SABORT;

typedef struct _PSTOBEX_SSETPATH
{
   UINT32 m_iConnId;                               // 4 bytes specifying the connect id.

   UCHAR m_PathName[CPSTOBEX_MEDIUMSTRING_SIZE];   // Null terminated, Unicode for relative path name
   UINT8 m_bAutoCreate;                            // Byte acting like a boolean
} PSTOBEX_SSETPATH;

typedef struct _PSTOBEX_SGET
{
   BYTE m_Name[CPSTOBEX_MEDIUMSTRING_SIZE];        // Null terminated, Unicode complete
                                                   // file name. Max length of 512
   BYTE m_Type[CPSTOBEX_SMALLSTRING_SIZE];         // Null terminated, ASCII string
                                                   // for object type. Max length of 255
   UINT32 m_iLength;                               // Maximum OBEX packet size
   BYTE m_Description[CPSTOBEX_MEDIUMSTRING_SIZE]; // Null terminated, Unicode for
                                                   // object description. Max length of 512
   BYTE m_TimeUTC[CPSTOBEX_TIMEUTC_SIZE];          // Null terminated, ASCII for UTC time.
                                                   // E.g., YYYYMMDDTHHMMSSZ
   UINT32 m_iTimeSeconds;                          // 4 bytes specifying seconds from Jan 1st, 1970.
   UINT32 m_iConnId;                               // 4 bytes specifying the connect id.
   BYTE m_DataBlock[CPSTOBEX_MAX_GET_BODY_SIZE];   // Maximum OBEX Get body size
   UINT32 m_iDataBlockLength;                      // Size of data stored in m_DataBlock in bytes
   int m_iRetries;                                 // Number of retries to retrieve phone response
} PSTOBEX_SGET;

typedef struct _PSTOBEX_SPUT
{
   BYTE m_Name[CPSTOBEX_MEDIUMSTRING_SIZE];          // Null terminated, Unicode complete file name.
                                                     // Max length of 512
   BYTE m_Type[CPSTOBEX_SMALLSTRING_SIZE];           // Null terminated, ASCII string for object type.
                                                     // Max length of 255
   UINT32 m_iLength;                                 // File lenght
   UINT16 m_iMaxPackLength;                          // Maximum OBEX packet size
   BYTE m_Description[CPSTOBEX_MEDIUMSTRING_SIZE];   // Null terminated, Unicode for object description.
                                                     // Max length of 512
   BYTE m_TimeUTC[CPSTOBEX_TIMEUTC_SIZE];            // Null terminated, ASCII for UTC time.
                                                     // E.g., YYYYMMDDTHHMMSSZ
   UINT32 m_iTimeSeconds;                            // 4 bytes specifying seconds from Jan 1st, 1970.
   UINT32 m_iConnId;                                 // 4 bytes specifying the connect id.
   BYTE m_DataBlock[CPSTOBEX_MAX_PUTDATA_BODY_SIZE]; // Maximum OBEX Put body size
   UINT32 m_iDataBlockLength;                        // Size of data stored in m_DataBlock in bytes
} PSTOBEX_SPUT;

typedef struct _PSTOBEX_SDEL
{
   UINT32 m_iConnId;                              // 4 bytes specifying the connect id.
   UCHAR m_PathName[CPSTOBEX_MEDIUMSTRING_SIZE];  // Null terminated, Unicode for
                                                  // relative path name. Max length of
                                                  // CPSTOBEX_MEDIUMSTRING_SIZE (512)
   UCHAR m_Name[CPSTOBEX_MEDIUMSTRING_SIZE];      // Null terminated, Unicode complete
                                                  // file name. Max length of
                                                  // CPSTOBEX_MEDIUMSTRING_SIZE (512)
} PSTOBEX_SDEL;

typedef struct _PSTOBEX_SDIRLIST
{
   UINT32 m_iConnId;                              // 4 bytes specifying the connect id.
   UCHAR m_PathName[CPSTOBEX_MEDIUMSTRING_SIZE];  // Null terminated, Unicode for
                                                  // relative path name. Max length of
                                                  // CPSTOBEX_MEDIUMSTRING_SIZE (512)
} PSTOBEX_SDIRLIST;



#endif // #ifndef __PSTOBEXCommon_h__
