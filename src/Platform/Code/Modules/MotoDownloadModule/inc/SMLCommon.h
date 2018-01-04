//--------------------------------------------------------------------------------------------------
//
//   Module Name: SMLCommon.h
//
//   General Description: Declaration of SyncML common type definitions and constants.
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
// Daniel Imamura             07/21/05      LIBgg08171  File creation
// Daniel Imamura             01/26/06      LIBhh80257  OBEX workaround(packet size, timeout) and SyncML packet split
// Daniel Imamura             03/10/06      LIBii14868  Adding definition of SML_GLOBALFLEXTABLE_SYNCML
// Daniel Imamura             03/10/06      LIBii14868  Adding definition of SML_GLOBALFLEXTABLE_SYNCML
// Rodrigo Oshiro             04/18/06      LIBii25156  Added vObjects handling support 
// Daniel Imamura             04/13/06      LIBii15022  Including new ESMLStatus
// Tiago Armani               06/02/06      LIBii54372  Support for Obex Server
// Renan Battaglin            08/08/06      LIBjj44971  Added field m_iLocURI to SML_VOBJECT structure
//
//--------------------------------------------------------------------------------------------------
//                                       INCLUDE FILES
//--------------------------------------------------------------------------------------------------
#ifndef __SMLCommon_h__
#define __SMLCommon_h__

#include <afxcoll.h>
#include "PSTCommon.h"
#include <vector>

//--------------------------------------------------------------------------------------------------
//                                       EXTERNAL TYPES
//--------------------------------------------------------------------------------------------------
class CSMLDList;
class CSMLMessageLogger;

//--------------------------------------------------------------------------------------------------
//                                       LIBRARY RELATED
//--------------------------------------------------------------------------------------------------
#define SML_CORE_API extern "C" __declspec(dllexport)

//--------------------------------------------------------------------------------------------------
//                                       CONSTANTS
//--------------------------------------------------------------------------------------------------
// Availability bit in SEEM_GLOBAL_FLEX_TABLE for feat.8009 SyncML-DS support
// named DL_DB_FEATURE_ID_LOCAL_SYNC_FEATURE_AVAILABLE: 
// bit 1409
// SEEM element: id=50/0x32; Record=1/0x01; offset=176/0xB0; length=1/0x01
#define SML_GLOBALFLEXTABLE_SYNCML 1409 

#define SML_WBXML_ITOA_MAXSIZE 35    // Max number of digits that itoa converts (33) + terminators (2)
#define SML_WBXML_MAXDIGITS 11        // Max digits of an integer for the SyncML (32bits as defined
                                     // in MultiByte - 4 continuation bits = 28bits = 268.435.455
#define SML_WBXML_STRTERMINATOR 0x00 // string terminator character

// -- Default string sizes
#define SML_SIZE_STRINGLONG   255 // Long string
#define SML_SIZE_STRINGMEDIUM 128 // Medium string
#define SML_SIZE_STRINGSHORT  64  // Short string

// -- Default initialization constants
#define SML_UINT32_UNDEFINED 0xFFFFFFFF // Undefined value for 4-byte unsigned integers


// -- OBEX service for SyncML
// SML_OBEX_TARGET_LENGTH define does not consider the null terminator as it is used in 
// the OBEX connect operation. In this case, the null termination character is not added 
// to the OBEX packet.
#define SML_OBEX_TARGET_NAME   "SYNCML-SYNC"
#define SML_OBEX_TARGET_LENGTH 11

// Note that SML_OBEX_MIMETYPE_LEN must have null terminator added since it is used in 
// other OBEX operations where the null termination must be sent
#define SML_OBEX_MIMETYPE_STR  "application/vnd.syncml+wbxml"
#define SML_OBEX_MIMETYPE_LEN  29 // actual 28 chars + null terminator
#define SML_OBEX_TYPEHEADER_OVERHEAD 3 // 1 byte = header opcode; 2 bytes = length

// -- Used as undefined Id when the ID is supposed to be a positive integer
#define SML_INTID_UNDEFINED -1

// Default SyncML header values
#define SML_SMLPROT_VERDTD "1.1"
#define SML_SMLPROT_VERPROTO "SyncML/1.1"
#define SML_SMLPROT_SMLDLL "SDIRSD"

// Name of the specialized class from CSMLObject
#define SML_CLASSNAME_BODYALERT    "CSMLBodyAlert"
#define SML_CLASSNAME_BODYCRED     "CSMLBodyCred"
#define SML_CLASSNAME_BODYMAP      "CSMLBodyMap"
#define SML_CLASSNAME_BODYMAPITEM  "CSMLBodyMapItem"
#define SML_CLASSNAME_BODYPART     "CSMLBodyPart"
#define SML_CLASSNAME_BODYPUT      "CSMLBodyPut"
#define SML_CLASSNAME_BODYSTATUS   "CSMLBodyStatus"
#define SML_CLASSNAME_BODYSYNC     "CSMLBodySync"
#define SML_CLASSNAME_BODYSYNCCMD  "CSMLBodySyncCmd"
#define SML_CLASSNAME_BODYDATAITEM "CSMLDataItem"
#define SML_CLASSNAME_BODYMETAINFO "CSMLMetaInfo"
#define SML_CLASSNAME_COMM         "CSMLComm"
#define SML_CLASSNAME_PACK         "CSMLPack"
#define SML_CLASSNAME_PACKBODY     "CSMLPackBody"
#define SML_CLASSNAME_PACKHDR      "CSMLPackHdr"
#define SML_CLASSNAME_ENGWBXML     "CSMLEngineWBXML"
#define SML_CLASSNAME_PACK2WB      "CSMLPack2WBXML"
#define SML_CLASSNAME_WB2PACK      "CSMLWBXML2Pack"
#define SML_CLASSNAME_WBINFO       "CSMLWBXMLInfo"

// Name of the SyncML command class from CSMLObject
#define SML_SMLCMD_ADD          "Add"
#define SML_SMLCMD_ALERT        "Alert"
#define SML_SMLCMD_ARCHIVE      "Archive"
#define SML_SMLCMD_ATOMIC       "Atomic"
#define SML_SMLCMD_COPY         "Copy"
#define SML_SMLCMD_CRED         "Cred"
#define SML_SMLCMD_DELETE       "Delete"
#define SML_SMLCMD_EXEC         "Exec"
#define SML_SMLCMD_GET          "Get"
#define SML_SMLCMD_MAP          "Map"
#define SML_SMLCMD_PUT          "Put"
#define SML_SMLCMD_REPLACE      "Replace"
#define SML_SMLCMD_SEARCH       "Search"
#define SML_SMLCMD_SFTDEL       "SftDel"
#define SML_SMLCMD_SYNC         "Sync"
#define SML_SMLCMD_SYNCHDR      "SyncHdr"

//--------------------------------------------------------------------------------------------------
//                                       CHARSET CONSTANTS
//--------------------------------------------------------------------------------------------------
// IANA charset list can be found at
// http://www.iana.org/assignments/ianacharset-mib
#define SML_CHARSET_OTHER                    1 // other
#define SML_CHARSET_UNKNOWN                  2 // unknown
#define SML_CHARSET_ASCII                    3 // csASCII
#define SML_CHARSET_SHIFTJIS                17 // csShiftJIS
#define SML_CHARSET_UTF8                   106 // csUTF8

//--------------------------------------------------------------------------------------------------
//                                       LOGGING CONSTANTS
//--------------------------------------------------------------------------------------------------
// -- SyncML log levels
#define CSML_LOGLEVEL_NONE   0x00 // Do not log anything
#define CSML_LOGLEVEL_IMPGAP 0x02 // Implementation gap - Must always log!
#define CSML_LOGLEVEL_ERROR  0x04 // Errors
#define CSML_LOGLEVEL_WARN   0x08 // Warnings
#define CSML_LOGLEVEL_TRACE  0x10 // Traces
#define CSML_LOGLEVEL_DEBUG  0x20 // Debugging information. Only valid in debug versions
#define CSML_LOGLEVEL_ALL    0xFF // Log all messages

// -- SyncML modules log
#define CSML_LOGMODULE_NONE         0x00000000 // Do not log anything
#define CSML_LOGMODULE_COMM         0x00000100 // Communication related
#define CSML_LOGMODULE_XML          0x00000200 // XML related
#define CSML_LOGMODULE_PACKET       0x00000400 // Packet related
#define CSML_LOGMODULE_SESSION      0x00000800 // SyncML Session related
#define CSML_LOGMODULE_DS           0x00001000 // SyncML-DS related
#define CSML_LOGMODULE_API          0x00002000 // API exposure in SML.dll
#define CSML_LOGMODULE_MISC         0x00004000 // Misc. classes, e.g., Doubly linked list, tag contents
#define CSML_LOGMODULE_OBEX_SERVER  0x00008000 // Obex server class
#define CSML_LOGMODULE_OS_CONTEXT   0x00010000 // Obex server context classes
#define CSML_LOGMODULE_OS_OPERATION 0x00020000 // Obex server operation classes
#define CSML_LOGMODULE_OS_COMM      0x00040000 // Obex communication classes
#define CSML_LOGMODULE_ALL          0x00FFFF00 // Log all messages

//--------------------------------------------------------------------------------------------------
//                                       Messages
//--------------------------------------------------------------------------------------------------
typedef struct _SML_MESSAGING
{
	WORD	 wDeviceID;
	DWORD	 dwLevels;
	DWORD	 dwModules;
	LPCSTR strMessage;
      
} SML_MESSAGING, *PSML_MESSAGING;

const UINT WM_SML_LOG_MESSAGE =::RegisterWindowMessage(_T("WM_SML_LOG_MESSAGE"));

//--------------------------------------------------------------------------------------------------
//                                       Structures
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                       TYPES
//--------------------------------------------------------------------------------------------------
// -- Enumerations
enum ESMLWBXML_CONTENT_TYPE
{
     SML_CONTENT_OPAQUE = 0x00  // The content is specified as an opaque data
   , SML_CONTENT_INLINE = 0x01  // The content is defined as an inline string
   , SML_CONTENT_STABLE = 0x02  // The content is defined as a reference in string table
   , SML_CONTENT_UNKNOWN = 0xFF // The content of the string is unknown
};

// SyncML Operation status
enum ESMLStatus
{
     SML_OK                     = 0x00  // Operation completed successfully
   , SML_ERR_INTERNAL           = 0x100 // Unexpected error occurred. Internal error during processing

   // General
   , SML_ERR_INVALID_PARAM      = 0x01  // Specified parameter is invalid
   , SML_ERR_DLL_LOAD           = 0x02  // SML cannot load a required DLL (most likely PST.dll)
   , SML_ERR_PRECOND_FAIL       = 0x03  // Pre-condition for the operation failed
   , SML_ERR_XMLPART_TOOBIG     = 0x04  // XML part to be serialized too big to fit in current byte array
   , SML_ERR_ARRAY_OUTOFBOUNDS  = 0x05  // Trying to access array in an out of bounds position
   , SML_ERR_ARRAY_NODATA       = 0x06  // Trying to access array in a position that does not have data set
   , SML_ERR_INSUFFICIENT_MEMORY = 0x07 // Error allocating memory

   // OBEX
   , SML_ERR_OBEX_INIT          = 0x10  // Error initializating OBEX support in the phone
   , SML_ERR_OBEX_CONNECT       = 0x11  // Error trying to connect to the OBEX Service
   , SML_ERR_OBEX_DISCONNECT    = 0x12  // Error trying to disconnect from the OBEX Service
   , SML_ERR_OBEX_PUT           = 0x13  // Error trying to put object
   , SML_ERR_OBEX_GET           = 0x14  // Error trying to get object
   , SML_ERR_OBEX_TIMEOUT       = 0x15  // Timeout when performing an OBEX operation (e.g., GET)

   // XML
   , SML_ERR_XML_ERRORCONVERT   = 0x20  // Unspecified error while converting WBXML to CSMLPack or vice-versa
   , SML_ERR_XML_WBXMLINFO      = 0x21  // Error while parsing WBXML format-related information
   , SML_ERR_XML_WBXMLHEADER    = 0x22  // Error while parsing WBXML header information
   , SML_ERR_XML_NOTSUPPORTED   = 0x23  // Error occurred because a non-supported tag/request was found
   , SML_ERR_XML_WBXMLMETINF    = 0x24  // Error while parsing WBXML meta information
   , SML_ERR_XML_WBXMLALERT     = 0x25  // Error while parsing WBXML Body Alert information
   , SML_ERR_XML_WBXMLDATAITEM  = 0x26  // Error while parsing WBXML Data Item
   , SML_ERR_XML_WBXMLMETANCH   = 0x27  // Error while parsing WBXML meta Anchor
   , SML_ERR_XML_WBXMLMETMEM    = 0x28  // Error while parsing WBXML meta memory information
   , SML_ERR_XML_WBXMLBODY      = 0x29  // Error while parsing WBXML Body information
   , SML_ERR_XML_WBXMLMAP       = 0x2A  // Error while parsing WBXML Body Map information
   , SML_ERR_XML_WBXMLMAPITEM   = 0x2B  // Error while parsing WBXML Body Map Item information
   , SML_ERR_XML_WBXMLSYNC      = 0x2C  // Error while parsing WBXML Body Sync information
   , SML_ERR_XML_WBXMLSYNCCMD   = 0x2D  // Error while parsing WBXML Body Sync Command information
   , SML_ERR_XML_WBXMLSTATUS    = 0x2E  // Error while parsing WBXML Body Status information
   , SML_ERR_XML_WBXMLPUT       = 0x2F  // Error while parsing WBXML Body Put information
   , SML_ERR_XML_WBXMLCRED      = 0x30  // Error while parsing WBXML Body Cred information
   , SML_ERR_XML_WBXMLROUTEINF  = 0x31  // Error while parsing WBXML routing information (target/source related)

   // Session
   , SML_ERR_SESSION_ISINIT     = 0x40  // Session is already initialized
   , SML_ERR_SESSION_RESPONSE   = 0x41  // Error while processing respnse packet
   , SML_ERR_SESSION_RETCODE    = 0x42  // Return code from the phone contains error
   , SML_ERR_SESSION_CREATERESP = 0x43  // Error while creating response to phone
};

// Which type of session is needed
enum ESMLSessionType
{
     ESML_SESSTYPE_UNDEFINED = 0x00 // Unknown: will not create any
   , ESML_SESSTYPE_BAK_PBDB  = 0x01 // Create SyncML session to perform backup operation (PB, DB, ToDos)
   , ESML_SESSTYPE_RES_PBDB  = 0x02 // Create restore session to perform restore operation (PB, DB, ToDos)
};

// Which kind of XML format SyncML protocol will be performing
enum ESMLSessionFormat
{
     ESML_SESSFMT_UNDEFINED = 0
   , ESML_SESSFMT_WBXML         // WBXML
};

// Which connection protocol will be used
enum ESMLConnProtocol
{
     ESML_CONN_UNDEFINED = 0
   , ESML_CONN_OBEX_USB         // OBEX over USB
};

// SyncML valid commands
enum ESMLCommand
{
     ESML_CMD_ADD      = 0x00
   , ESML_CMD_ALERT    = 0x01
   , ESML_CMD_ATOMIC   = 0x02
   , ESML_CMD_COPY     = 0x03
   , ESML_CMD_DELETE   = 0x04
   , ESML_CMD_EXEC     = 0x05
   , ESML_CMD_GET      = 0x06
   , ESML_CMD_MAP      = 0x07
   , ESML_CMD_PUT      = 0x08
   , ESML_CMD_REPLACE  = 0x09
   , ESML_CMD_RESULTS  = 0x0a
   , ESML_CMD_SEARCH   = 0x0b
   , ESML_CMD_SEQUENCE = 0x0c
   , ESML_CMD_STATUS   = 0x0d
   , ESML_CMD_SYNC     = 0x0e
};


enum ESMLSyncMLType
{
     ESML_SYNC_NONE
   , ESML_SYNC_CLIENT_INITIATED
   , ESML_SYNC_SERVER_ALERTED
};


// SyncML return codes
// 101 In progress. The specified SyncML command is being carried out, but has not yet completed.
//
// Successful 2xx
// 200 OK. The SyncML command completed successfully.
// 201 Item added. The requested item was added.
// 202 Accepted for processing. The request to either run a remote execution of an application or to alert a user or application was successfully performed.
// 203 Non-authoritative response. The request is being responded to by an entity other than the one targeted. The response is only to be returned when the request would have been resulted in a 200 response code from the authoritative target.
// 204 No content. The request was successfully completed but no data is being returned. The response code is also returned in response to a Get when the target has no content.
// 205 Reset content. The source should update their content. The originator of the request is being told that their content should be synchronized to get an up to date version.
// 206 Partial content. The response indicates that only part of the command was completed. If the remainder of the command can be completed later, then when completed another appropriate completion request status code SHOULD be created.
// 207 Conflict resolved with merge. The response indicates that the request created a conflict; which was resolved with a merge of the client and server instances of the data. The response includes both the Target and Source URLs in the Item of the Status. In addition, a Replace command is returned with the merged data.
// 208 Conflict resolved with client’s command "winning". The response indicates that there was an update conflict; which was resolved by the client command winning.
// 209 Conflict resolved with duplicate. The response indicates that the request created an update conflict; which was resolved with a duplication of the client’s data being created in the server database. The response includes both the target URI of the duplicate in the Item of the Status. In addition, in the case of a two-way synchronization, an Add command is returned with the duplicate data definition.
// 210 Delete without archive. The response indicates that the requested data was successfully deleted, but that it was not archived prior to deletion because this optional feature was not supported by the implementation.
// 211 Item not deleted. The requested item was not found. It may have been previously deleted.
// 212 Authentication accepted. No further authentication is needed for the remainder of the synchronization session. This response code can only be used in response to a request in which the credentials were provided.
// 213 Chunked item accepted and buffered.
// 214 Operation cancelled. The SyncML command completed successfully, but no more commands will be processed within the session.
// 215 Not executed. A command was not executed, as a result of user interaction and user chose not to accept the choice.
// 216 Atomic roll back OK. A command was inside Atomic element and Atomic failed. This command was rolled back successfully.
//
// Redirection 3xx
// 300 Multiple choices. The requested target is one of a number of multiple alternatives requested target. The alternative SHOULD also be returned in the Item element type in the Status.
// 301 Moved permanently. The requested target has a new URI. The new URI SHOULD also be returned in the Item element type in the Status.
// 302 Found. The requested target has temporarily moved to a different URI. The original URI SHOULD continue to be used. The URI of the temporary location SHOULD also be returned in the Item element type in the Status. The requestor SHOULD confirm the identity and authority of the temporary URI to act on behalf of the original target URI.
// 303 See other. The requested target can be found at another URI. The other URI SHOULD be returned in the Item element type in the Status.
// 304 Not modified. The requested SyncML command was not executed on the target. This is an additional response that can be added to any of the other Redirection response codes.
// 305 Use proxy. The requested target MUST be accessed through the specified proxy URI. The proxy URI SHOULD also be returned in the Item element type in the Status. Originator Exceptions 4xx
// 400 Bad request. The requested command could not be performed because of malformed syntax in the command. The malformed command MAY also be returned in the Item element type in the Status.
// 401 Invalid credentials. The requested command failed because the requestor MUST provide proper authentication. If the property type of authentication was presented in the original request, then the response code indicates that the requested command has been refused for those credentials.
// 402 Payment required. The requested command failed because proper payment is required. This version of SyncML does not standardize the payment mechanism.
// 403 Forbidden. The requested command failed, but the recipient understood the requested command. Authentication will not help and the request SHOULD NOT be repeated. If the recipient wishes to make public why the request was denied, then a description MAY be specified in the Item element type in the Status. If the recipient does not wish to make public why the request was denied then the response code 404 MAY be used instead.
// 404 Not found. The requested target was not found. No indication is given as to whether this is a temporary or permanent condition. The response code 410 SHOULD be used when the condition is permanent and the recipient wishes to make this fact public. This response code is also used when the recipient does not want to make public the reason for why a requested command is not allowed or when no other response code is appropriate.
// 405 Command not allowed. The requested command is not allowed on the target. The recipient SHOULD return the allowed command for the target in the Item element type in the Status.
// 406 Optional feature not supported. The requested command failed because an optional feature in the request was not supported. The unsupported feature SHOULD be specified by the Item element type in the Status.
// 407 Missing credentials. This response code is similar to 401 except that the response code indicates that the originator MUST first authenticate with the recipient. The recipient SHOULD also return the suitable challenge in the Chal element type in the Status.
// 408 Request timeout. An expected message was not received within the required period of time. The request can be repeated at another time. The RespURI can be used to specify the URI and optionally the date/time after which the originator can repeat the request. See RespURI for details.
// 409 Conflict. The requested failed because of an update conflict between the client and server versions of the data. Setting of the conflict resolution policy is outside the scope of this version of SyncML. However, identification of conflict resolution performed, if any, is within the scope.
// 410 Gone. The requested target is no longer on the recipient and no forwarding URI is known.
// 411 Size required. The requested command MUST be accompanied by byte size or length information in the Meta element type.
// 412 Incomplete command. The requested command failed on the recipient because it was incomplete or incorrectly formed. The recipient SHOULD specify the portion of the command that was incomplete or incorrect in the Item element type in the Status.
// 413 Request entity too large. The recipient is refusing to perform the requested command because the requested item is larger than the recipient is able or willing to process. If the condition is temporary, the recipient SHOULD also include a Status with the response code 418 and specify a RespURI with the response URI and optionally the date/time that the command SHOULD be repeated.
// 414 URI too long. The requested command failed because the target URI is too long for what the recipient is able or willing to process. This response code is seldom encountered, but is used when a recipient perceives that an intruder may be attempting to exploit security holes or other defects in order to threaten the recipient.
// 415 Unsupported media type or format. The unsupported content type or format SHOULD also be identified in the Item element type in the Status.
// 416 Requested size too big. The request failed because the specified byte size in the request was too big.
// 417 Retry later. The request failed at this time and the originator should retry the request later. The recipient SHOULD specify a RespURI with the response URI and the date/time that the command SHOULD be repeated.
// 418 Already exists. The requested Put or Add command failed because the target already exists.
// 419 Conflict resolved with server data. The response indicates that the client request created a conflict; which was resolved by the server command winning. The normal information in the Status should be sufficient for the client to "undo" the resolution, if it is desired.
// 420 Device full. The response indicates that the recipient has no more storage space for the remaining synchronization data. The response includes the remaining number of data that could not be returned to the originator in the Item of the Status.
// 421 Unknown search grammar. The requested command failed on the server because the specified search grammar was not known. The client SHOULD re-specify the search using a known search grammar.
// 422 Bad CGI Script. The requested command failed on the server because the CGI scripting in the LocURI was incorrectly formed. The client SHOULD re-specify the portion of the command that was incorrect in the Item element type in the Status.
// 423 Soft-delete conflict. The requested command failed because the "Soft Deleted" item was previously "Hard Deleted" on the server.
// 424 Size mismatch. The chunked object was received, but the size of the received object did not match the size declared within the first chunk.
//
// Recipient Exception 5xx
// 500 Command failed. The recipient encountered an unexpected condition which prevented it from fulfilling the request
// 501 Command not implemented. The recipient does not support the command required to fulfill the request. This is the appropriate response when the recipient does not recognize the requested command and is not capable of supporting it for any resource.
// 502 Bad gateway. The recipient, while acting as a gateway or proxy, received an invalid response from the upstream recipient it accessed in attempting to fulfill the request.
// 503 Service unavailable. The recipient is currently unable to handle the request due to a temporary overloading or maintenance of the recipient. The implication is that this is a temporary condition; which will be alleviated after some delay. The recipient SHOULD specify the URI and date/time after which the originator should retry in the RespURI in the response.
// 504 Gateway timeout. The recipient, while acting as a gateway or proxy, did not receive a timely response from the upstream recipient specified by the URI (e.g. HTTP, FTP, LDAP) or some other auxiliary recipient (e.g. DNS) it needed to access in attempting to complete the request.
// 505 DTD Version not supported. The recipient does not support or refuses to support the specified version of SyncML DTD used in the request SyncML Message. The recipient MUST include the versions it does support in the Item element type in the Status.
// 506 Processing error. An application error occurred while processing the request. The originator should retry the request. The RespURI can contain the URI and date/time after which the originator can retry the request.
// 507 Atomic failed. The error caused all SyncML commands within an Atomic element type to fail. 508 Refresh required. An error occurred that necessitates a refresh of the current synchronization state of the client with the server. Client is requested to initiate a slow sync with the server.
// 509 Reserved for future use.
// 510 Data store failure. An error occurred while processing the request. The error is related to a failure in the recipient data store.
// 511 Server failure. A severe error occurred in the server while processing the request. The originator SHOULD NOT retry the request.
// 512 Synchronization failed. An application error occurred during the synchronization session. The originator should restart the synchronization session from the beginning.
// 513 Protocol Version not supported. The recipient does not support or refuses to support the specified version of the SyncML Synchronization Protocol used in the request SyncML Message. The recipient MUST include the versions it does support in the Item element type in the Status.
// 514 Operation cancelled. The SyncML command was not completed successfully, since the operation was already cancelled before processing the command. The originator should repeat the command in the next session.
// 516 Atomic roll back failed. Command was inside Atomic element and Atomic failed. This command was not rolled back successfully. Server should take action to try to recover client back into original state.
enum ESMLRetCode
{
    SML_RETCODE_OK        = 200		// OK
  , SML_RETCODE_ADDED	  = 201		// Item added. The requested item was added.
  , SML_RETCODE_NOT_FOUND = 404		// Not found. The requested target was not found.
};

typedef byte SML_WBXML_TAG; // Tag in WBXML is a byte long

typedef UINT32 SML_WBXML_CHARSET; // Tag in WBXML is a byte long

typedef struct _SML_VOBJECT{
    int m_iLocURI;
    int m_iLength;
    BYTE* m_pbData;
} SML_VOBJECT;
        
//--------------------------------------------------------------------------------------------------
//                                       APIs
//--------------------------------------------------------------------------------------------------
SML_CORE_API bool SMLSuccess(ESMLStatus Status);
SML_CORE_API bool SMLFailure(ESMLStatus Status);
SML_CORE_API ESMLStatus SMLIsSyncMLSupported(DEVICEID deviceID, HANDLE hInfac, BYTE *pData, DWORD dwDataLength);

SML_CORE_API ESMLStatus SMLInitialize(HANDLE hAppHandle = NULL, DWORD dwLogLevel = CSML_LOGLEVEL_ERROR, DWORD dwLogModule = CSML_LOGMODULE_ALL);
SML_CORE_API ESMLStatus SMLUninitialize();
SML_CORE_API ESMLStatus SMLBackup(DEVICEID deviceID, HANDLE hInfac, LPCTSTR pIMEI, CSMLDList **pPBData, CSMLDList **pDBData);
SML_CORE_API ESMLStatus SMLRestore(DEVICEID deviceID, HANDLE hInfac, LPCTSTR pIMEI, CSMLDList *pPBData, CSMLDList *pDBData);
SML_CORE_API ESMLStatus SMLDataFree(CSMLDList **pData);
SML_CORE_API ESMLStatus SMLGetObjectList(CSMLDList *pData, std::vector<SML_VOBJECT> &arrVObject);
SML_CORE_API ESMLStatus SMLSetObjectList(CSMLDList **pData, std::vector<SML_VOBJECT> &arrVObject);

#ifdef _SML_INT_TEST_ON_
  // Code included here exposes APIs used specifically for Integration Testing (use 
  // "Win32 Debug for Integration Test" configuration) and is NOT intended to be exposed 
  // in regular debug/release builds
  SML_CORE_API ESMLStatus SMLIntTestWBXML(LPCTSTR pWBXMLFileName, LPCTSTR pDestinationFileName);
  SML_CORE_API ESMLStatus SMLIntTestOBEX(DEVICEID deviceID, HANDLE hInfac);
#endif // _SML_INT_TEST_ON_

#endif // #ifndef __SMLCommon_h__
