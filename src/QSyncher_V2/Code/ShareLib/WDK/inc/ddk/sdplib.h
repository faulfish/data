#ifndef __SDPLIB_H__
#define __SDPLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "sdpnode.h"

#ifndef NTSTATUS
typedef LONG NTSTATUS;
#endif

#ifdef _NTDDK_
#define SDPLIB_KERNEL
#endif

#ifdef SDPLIB_KERNEL
PSDP_TREE_ROOT_NODE
SdpCreateNodeTree(
    ULONG tag
    );

PSDP_NODE
SdpCreateNodeNil(
    ULONG tag
    );

PSDP_NODE
SdpCreateNodeUInt128(
    PSDP_ULARGE_INTEGER_16 puli16Val,
    ULONG tag
    );

PSDP_NODE
SdpCreateNodeUInt64(
    ULONGLONG ullVal,
    ULONG tag
    );

PSDP_NODE
SdpCreateNodeUInt32(
    ULONG ulVal,
    ULONG tag
    );

PSDP_NODE
SdpCreateNodeUInt16(
    USHORT usVal,
    ULONG tag
    );

PSDP_NODE
SdpCreateNodeUInt8(
    UCHAR ucVal,
    ULONG tag
    );

PSDP_NODE
SdpCreateNodeInt128(
    PSDP_LARGE_INTEGER_16 uil16Val,
    ULONG tag
    );

PSDP_NODE
SdpCreateNodeInt64(
    LONGLONG llVal,
    ULONG tag
    );

PSDP_NODE
SdpCreateNodeInt32(
    LONG lVal,
    ULONG tag
    );

PSDP_NODE
SdpCreateNodeInt16(
    SHORT sVal,
    ULONG tag
    );

PSDP_NODE
SdpCreateNodeInt8(
    CHAR cVal,
    ULONG tag
    );

PSDP_NODE
SdpCreateNodeUUID128(
    const GUID *uuid,
    ULONG tag
    );

PSDP_NODE
SdpCreateNodeUUID32(
    ULONG uuidVal4,
    ULONG tag
    );

PSDP_NODE
SdpCreateNodeUUID16(
    USHORT uuidVal2,
    ULONG tag
    );

PSDP_NODE
SdpCreateNodeBoolean(
    SDP_BOOLEAN  bVal,
    ULONG tag
    );

PSDP_NODE
SdpCreateNodeSequence(
    ULONG tag
    );

PSDP_NODE
SdpCreateNodeAlternative(
    ULONG tag
    );

PSDP_NODE
SdpCreateNodeUrl(
    __in_bcount(UrlLength) PCHAR url,
    __in_bound ULONG urlLength,
    ULONG tag
    );

NTSTATUS
SdpAddAttributeToTree(
    PSDP_TREE_ROOT_NODE Tree,
    USHORT AttribId,
    PSDP_NODE AttribValue,
    ULONG tag
    );

PSDP_NODE
SdpCreateNodeString(
    __in_bcount(StringLength) PCHAR string, 
    __in_bound ULONG stringLength, 
    ULONG tag
    );

#else //SDPLIB_KERNEL

PSDP_TREE_ROOT_NODE
SdpCreateNodeTree();

PSDP_NODE
SdpCreateNodeNil();

PSDP_NODE
SdpCreateNodeUInt128(
    PSDP_ULARGE_INTEGER_16 puli16Val
    );

PSDP_NODE
SdpCreateNodeUInt64(
    ULONGLONG ullVal
    );

PSDP_NODE
SdpCreateNodeUInt32(
    ULONG ulVal
    );

PSDP_NODE
SdpCreateNodeUInt16(
    USHORT usVal
    );

PSDP_NODE
SdpCreateNodeUInt8(
    UCHAR ucVal
    );

PSDP_NODE
SdpCreateNodeInt128(
    PSDP_LARGE_INTEGER_16 uil16Val
    );

PSDP_NODE
SdpCreateNodeInt64(
    LONGLONG llVal
    );

PSDP_NODE
SdpCreateNodeInt32(
    LONG lVal
    );

PSDP_NODE
SdpCreateNodeInt16(
    SHORT sVal
    );

PSDP_NODE
SdpCreateNodeInt8(
    CHAR cVal
    );

PSDP_NODE
SdpCreateNodeUUID128(
    const GUID *uuid
    );

PSDP_NODE
SdpCreateNodeUUID32(
    ULONG uuidVal4
    );

PSDP_NODE
SdpCreateNodeUUID16(
    USHORT uuidVal2
    );

PSDP_NODE
SdpCreateNodeBoolean(
    SDP_BOOLEAN  bVal
    );

PSDP_NODE
SdpCreateNodeSequence();

PSDP_NODE
SdpCreateNodeAlternative();

PSDP_NODE
SdpCreateNodeUrl(
    __in_bcount(UrlLength) PCHAR url,
    __in_bound ULONG urlLength
    );

NTSTATUS
SdpAddAttributeToTree(
    PSDP_TREE_ROOT_NODE Tree,
    USHORT AttribId,
    PSDP_NODE AttribValue
    );

PSDP_NODE
SdpCreateNodeString(
    __in_bcount(StringLength) PCHAR string, 
    __in_bound ULONG stringLength
    );

#endif //SDPLIB_KERNEL

NTSTATUS
SdpFreeTree(
    PSDP_TREE_ROOT_NODE Tree
    );
    
#define   SdpCreateNodeUUID SdpCreateNodeUUID128

NTSTATUS
SdpAppendNodeToContainerNode(
    PSDP_NODE Parent,
    PSDP_NODE Node
    );

NTSTATUS
SdpFindAttributeInTree(
    PSDP_TREE_ROOT_NODE Tree,
    USHORT AttribId,
    PSDP_NODE *Attribute
    );

#ifdef __cplusplus
};
#endif


#endif


