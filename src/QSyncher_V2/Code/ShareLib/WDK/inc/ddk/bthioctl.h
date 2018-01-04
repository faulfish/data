/****************************************************************************

Copyright (c) 2000  Microsoft Corporation

Module Name:

    bthioctl.h

Abstract:

    defines the IOCTL codes for the kernel/user calls

Environment:

    Kernel & user mode

Revision History:

    4-4-00 : created by Husni Roukbi
    2-4-05 : split into public and private header files by SandySp

****************************************************************************/
#ifndef __BTHIOCTL_H__
#define __BTHIOCTL_H__

#if (NTDDI_VERSION >= NTDDI_LONGHORN)

#ifndef CTL_CODE
    #pragma message("CTL_CODE undefined. Include winioctl.h or wdm.h")
#endif

//
// IOCTL defines. 
//
#define BTH_IOCTL_BASE      0

#define BTH_CTL(id)         CTL_CODE(FILE_DEVICE_BLUETOOTH,  \
                                     (id), \
                                     METHOD_BUFFERED,  \
                                     FILE_ANY_ACCESS)

#define BTH_KERNEL_CTL(id)  CTL_CODE(FILE_DEVICE_BLUETOOTH,  \
                                     (id), \
                                     METHOD_NEITHER,  \
                                     FILE_ANY_ACCESS)

//
// kernel-level (internal) IOCTLs
//
#define IOCTL_INTERNAL_BTH_SUBMIT_BRB       BTH_KERNEL_CTL(BTH_IOCTL_BASE+0x00)

//
// Input:  none
// Output:  BTH_ENUMERATOR_INFO
//
#define IOCTL_INTERNAL_BTHENUM_GET_ENUMINFO BTH_KERNEL_CTL(BTH_IOCTL_BASE+0x01)

//
// Input:  none
// Output:  BTH_DEVICE_INFO
//
#define IOCTL_INTERNAL_BTHENUM_GET_DEVINFO  BTH_KERNEL_CTL(BTH_IOCTL_BASE+0x02)

//
// IOCTLs 
//
//
// Input:  none
// Output:  BTH_LOCAL_RADIO_INFO
//
#define IOCTL_BTH_GET_LOCAL_INFO            BTH_CTL(BTH_IOCTL_BASE+0x00)

//
// Input:  BTH_ADDR
// Output:  BTH_RADIO_INFO
//
#define IOCTL_BTH_GET_RADIO_INFO            BTH_CTL(BTH_IOCTL_BASE+0x01)

//
// use this ioctl to get a list of cached discovered devices in the port driver.
//
// Input: None
// Output: BTH_DEVICE_INFO_LIST
#define IOCTL_BTH_GET_DEVICE_INFO           BTH_CTL(BTH_IOCTL_BASE+0x02)

//
// Input:  BTH_ADDR
// Output:  none
//
#define IOCTL_BTH_DISCONNECT_DEVICE         BTH_CTL(BTH_IOCTL_BASE+0x03)


// Input:  BTH_SDP_CONNECT
// Output:  BTH_SDP_CONNECT
//
//
#define IOCTL_BTH_SDP_CONNECT               BTH_CTL(BTH_IOCTL_BASE+0x80)

//
// Input:  HANDLE_SDP
// Output:  none
//
#define IOCTL_BTH_SDP_DISCONNECT            BTH_CTL(BTH_IOCTL_BASE+0x81)

//
// Input:  BTH_SDP_SERVICE_SEARCH_REQUEST
// Output:  ULONG * number of handles wanted
//
#define IOCTL_BTH_SDP_SERVICE_SEARCH        BTH_CTL(BTH_IOCTL_BASE+0x82)

//
// Input:  BTH_SDP_ATTRIBUTE_SEARCH_REQUEST
// Output:  BTH_SDP_STREAM_RESPONSE or bigger
//
#define IOCTL_BTH_SDP_ATTRIBUTE_SEARCH      BTH_CTL(BTH_IOCTL_BASE+0x83)

//
// Input:  BTH_SDP_SERVICE_ATTRIBUTE_SEARCH_REQUEST
// Output:  BTH_SDP_STREAM_RESPONSE or bigger
//
#define IOCTL_BTH_SDP_SERVICE_ATTRIBUTE_SEARCH \
                                            BTH_CTL(BTH_IOCTL_BASE+0x84)

//
// Input:  raw SDP stream (at least 2 bytes)
// Ouptut: HANDLE_SDP
//
#define IOCTL_BTH_SDP_SUBMIT_RECORD         BTH_CTL(BTH_IOCTL_BASE+0x85)

//
// Input:  HANDLE_SDP
// Output:  none
//
#define IOCTL_BTH_SDP_REMOVE_RECORD         BTH_CTL(BTH_IOCTL_BASE+0x86)

//
// Input:  BTH_SDP_RECORD + raw SDP record
// Output:  HANDLE_SDP
//
#define IOCTL_BTH_SDP_SUBMIT_RECORD_WITH_INFO BTH_CTL(BTH_IOCTL_BASE+0x87)


#include <PSHPACK1.H>

typedef struct _BTH_DEVICE_INFO_LIST {
    //
    // [IN/OUT] minimum of 1 device required
    //
    ULONG       numOfDevices;

    //
    // Open ended array of devices;
    //
    BTH_DEVICE_INFO   deviceList[1];

} BTH_DEVICE_INFO_LIST, *PBTH_DEVICE_INFO_LIST;

typedef struct _BTH_RADIO_INFO {
    //
    // Supported LMP features of the radio.  Use LMP_XXX() to extract
    // the desired bits.
    //
    ULONGLONG lmpSupportedFeatures;

    //
    // Manufacturer ID (possibly BTH_MFG_XXX)
    //
    USHORT mfg;

    //
    // LMP subversion
    //
    USHORT lmpSubversion;

    //
    // LMP version
    //
    UCHAR lmpVersion;

} BTH_RADIO_INFO, *PBTH_RADIO_INFO;

typedef struct _BTH_LOCAL_RADIO_INFO {
    //
    // Local BTH_ADDR, class of defice, and radio name
    //
    BTH_DEVICE_INFO         localInfo;

    //
    // Combo of LOCAL_RADIO_XXX values
    //
    ULONG flags;

    //
    // HCI revision, see core spec
    //
    USHORT hciRevision;

    //
    // HCI version, see core spec
    //
    UCHAR hciVersion;

    //
    // More information about the local radio (LMP, MFG)
    //
    BTH_RADIO_INFO radioInfo;

} BTH_LOCAL_RADIO_INFO, *PBTH_LOCAL_RADIO_INFO;

#define SDP_CONNECT_CACHE           (0x00000001)
#define SDP_CONNECT_ALLOW_PIN       (0x00000002)

#define SDP_REQUEST_TO_DEFAULT      (0)
#define SDP_REQUEST_TO_MIN          (10)
#define SDP_REQUEST_TO_MAX          (45)

#define SERVICE_OPTION_DO_NOT_PUBLISH       (0x00000002)
#define SERVICE_OPTION_NO_PUBLIC_BROWSE     (0x00000004)

#define SERVICE_SECURITY_USE_DEFAULTS       (0x00000000)
#define SERVICE_SECURITY_NONE               (0x00000001)
#define SERVICE_SECURITY_AUTHORIZE          (0x00000002)
#define SERVICE_SECURITY_AUTHENTICATE       (0x00000004)
#define SERVICE_SECURITY_ENCRYPT_REQUIRED   (0x00000010)
#define SERVICE_SECURITY_ENCRYPT_OPTIONAL   (0x00000020)
#define SERVICE_SECURITY_DISABLED           (0x10000000)
#define SERVICE_SECURITY_NO_ASK             (0x20000000)

//
// Do not attempt to validate that the stream can be parsed
//
#define SDP_SEARCH_NO_PARSE_CHECK   (0x00000001)

//
// Do not check the format of the results.  This includes suppression of both
// the check for a record patten (SEQ of UINT16 + value) and the validation
// of each universal attribute's accordance to the spec.
//
#define SDP_SEARCH_NO_FORMAT_CHECK  (0x00000002)


typedef ULONGLONG HANDLE_SDP, *PHANDLE_SDP;
#define HANDLE_SDP_NULL         ((HANDLE_SDP) 0x0)
#define HANDLE_SDP_LOCAL    ((HANDLE_SDP) -2)

typedef struct _BTH_SDP_CONNECT {
    //
    // Address of the remote SDP server.  Cannot be the local radio.
    //
    BTH_ADDR     bthAddress;

    //
    // Combination of SDP_CONNECT_XXX flags
    //
    ULONG       fSdpConnect;

    //
    // When the connect request returns, this will specify the handle to the
    // SDP connection to the remote server
    //
    HANDLE_SDP   hConnection;

    //
    // Timeout, in seconds, for the requests on ths SDP channel.  If the request
    // times out, the SDP connection represented by the HANDLE_SDP must be
    // closed.  The values for this field are bound by SDP_REQUEST_TO_MIN and
    // SDP_REQUEST_MAX.  If SDP_REQUEST_TO_DEFAULT is specified, the timeout is
    // 30 seconds.
    //
    UCHAR       requestTimeout;

} BTH_SDP_CONNECT,  *PBTH_SDP_CONNECT;

typedef struct _BTH_SDP_DISCONNECT {
    //
    // hConnection returned by BTH_SDP_CONNECT
    //
    HANDLE_SDP   hConnection;

} BTH_SDP_DISCONNECT, *PBTH_SDP_DISCONNECT;


typedef struct _BTH_SDP_RECORD {
    //
    // Combination of SERVICE_SECURITY_XXX flags
    //
    ULONG fSecurity;

    //
    // Combination of SERVICE_OPTION_XXX flags
    //
    ULONG fOptions;

    //
    // combo of COD_SERVICE_XXX flags
    //
    ULONG fCodService;

    //
    // The length of the record array, in bytes.
    //
    ULONG recordLength;

    //
    // The SDP record in its raw format
    //
    UCHAR record[1];

} BTH_SDP_RECORD, *PBTH_SDP_RECORD;

typedef struct _BTH_SDP_SERVICE_SEARCH_REQUEST {
    //
    // Handle returned by the connect request or HANDLE_SDP_LOCAL
    //
    HANDLE_SDP   hConnection;
    
    //
    // Array of UUIDs.  Each entry can be either a 2 byte, 4 byte or 16 byte
    // UUID. SDP spec mandates that a request can have a maximum of 12 UUIDs.
    //
    SdpQueryUuid uuids[MAX_UUIDS_IN_QUERY];

} BTH_SDP_SERVICE_SEARCH_REQUEST, *PBTH_SDP_SERVICE_SEARCH_REQUEST;

typedef struct _BTH_SDP_ATTRIBUTE_SEARCH_REQUEST {
    //
    // Handle returned by the connect request or HANDLE_SDP_LOCAL
    //
    HANDLE_SDP   hConnection;

    //
    // Combo of SDP_SEARCH_Xxx flags
    //
    ULONG searchFlags;

    //
    // Record handle returned by the remote SDP server, most likely from a
    // previous BTH_SDP_SERVICE_SEARCH_RESPONSE.
    //
    ULONG recordHandle;

    //
    // Array of attributes to query for.  Each SdpAttributeRange entry can
    // specify either a single attribute or a range.  To specify a single
    // attribute, minAttribute should be equal to maxAttribute.   The array must
    // be in sorted order, starting with the smallest attribute.  Furthermore,
    // if a range is specified, the minAttribute must be <= maxAttribute.
    //
    SdpAttributeRange range[1];

} BTH_SDP_ATTRIBUTE_SEARCH_REQUEST, *PBTH_SDP_ATTRIBUTE_SEARCH_REQUEST;

typedef struct _BTH_SDP_SERVICE_ATTRIBUTE_SEARCH_REQUEST {
    //
    // Handle returned by the connect request or HANDLE_SDP_LOCAL
    //
    HANDLE_SDP   hConnection;

    //
    // Combo of SDP_SEARCH_Xxx flags
    //
    ULONG searchFlags;

    //
    // See comments in BTH_SDP_SERVICE_SEARCH_REQUEST
    //
    SdpQueryUuid uuids[MAX_UUIDS_IN_QUERY];

    //
    // See comments in BTH_SDP_ATTRIBUTE_SEARCH_REQUEST
    //
    SdpAttributeRange range[1];

} BTH_SDP_SERVICE_ATTRIBUTE_SEARCH_REQUEST,
  *PBTH_SDP_SERVICE_ATTRIBUTE_SEARCH_REQUEST;

typedef struct _BTH_SDP_STREAM_RESPONSE {
    //
    // The required buffer size (not including the first 2 ULONG_PTRs of this
    // data structure) needed to contain the response.
    //
    // If the buffer passed was large enough to contain the entire response,
    // requiredSize will be equal to responseSize.  Otherwise, the caller should
    // resubmit the request with a buffer size equal to
    // sizeof(BTH_SDP_STREAM_RESPONSE) + requiredSize - 1.  (The -1 is because
    // the size of this data structure already includes one byte of the
    // response.)
    //
    // A response cannot exceed 4GB in size.
    //
    ULONG requiredSize;

    //
    // The number of bytes copied into the response array of this data
    // structure.  If there is not enough room for the entire response, the
    // response will be partially copied into the response array.
    //
    ULONG responseSize;

    //
    // The raw SDP response from the serach.
    //
    UCHAR response[1];

} BTH_SDP_STREAM_RESPONSE, *PBTH_SDP_STREAM_RESPONSE;

#include <POPPACK.H>

#endif // (NTDDI_VERSION >= NTDDI_LONGHORN)

#endif // __BTHIOCTL_H__

