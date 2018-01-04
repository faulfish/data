#ifndef  __BTHSDPDDI_H__
#define  __BTHSDPDDI_H__

#if (NTDDI_VERSION >= NTDDI_LONGHORN)

#ifdef __cplusplus
extern "C" {
#endif

#define BTHDDI_SDP_PARSE_INTERFACE_VERSION_FOR_QI       (0x0100)
#define BTHDDI_SDP_NODE_INTERFACE_VERSION_FOR_QI        (0x0100)

typedef PSDP_TREE_ROOT_NODE (*PCREATENODETREEROOT)(ULONG tag);
typedef NTSTATUS (*PFREETREE)(PSDP_TREE_ROOT_NODE Tree);

typedef NTSTATUS (*PAPPENDNODETOCONTAINERNODE)(PSDP_NODE Container,
                                               PSDP_NODE Node);
typedef NTSTATUS (*PADDATTRIBUTETOTREEE)(PSDP_TREE_ROOT_NODE Root,
                                         USHORT AttribId,
                                         PSDP_NODE AttribValueNode,
                                         ULONG tag);

typedef PSDP_NODE (*PCREATENODENIL)(ULONG tag);

typedef PSDP_NODE (*PCREATENODEBOOLEAN)(UCHAR bVal, ULONG tag);

typedef PSDP_NODE (*PCREATENODEUINT8)(UCHAR ucVal, ULONG tag);
typedef PSDP_NODE (*PCREATENODEUINT16)(USHORT usVal, ULONG tag);
typedef PSDP_NODE (*PCREATENODEUINT32)(ULONG ulVal, ULONG tag);
typedef PSDP_NODE (*PCREATENODEUINT64)(ULONGLONG ullVal, ULONG tag);
typedef PSDP_NODE (*PCREATENODEUINT128)(PSDP_ULARGE_INTEGER_16 puli16Val, ULONG tag);

typedef PSDP_NODE (*PCREATENODEINT8)(CHAR cVal, ULONG tag);
typedef PSDP_NODE (*PCREATENODEINT16)(SHORT sVal, ULONG tag);
typedef PSDP_NODE (*PCREATENODEINT32)(LONG lVal, ULONG tag);
typedef PSDP_NODE (*PCREATENODEINT64)(LONGLONG llVal, ULONG tag);
typedef PSDP_NODE (*PCREATENODEINT128)(PSDP_LARGE_INTEGER_16 pul16Val, ULONG tag);

typedef PSDP_NODE (*PCREATENODEUUID16)(USHORT usVal, ULONG tag);
typedef PSDP_NODE (*PCREATENODEUUID32)(ULONG ulVal, ULONG tag);
typedef PSDP_NODE (*PCREATENODEUUID128)(const GUID * pUuidVal, ULONG tag);

typedef PSDP_NODE (*PCREATENODESTRING)(PCHAR string, ULONG stringLength, ULONG tag);
typedef PSDP_NODE (*PCREATENODEURL)(PCHAR url, ULONG urlLength, ULONG tag);

typedef PSDP_NODE (*PCREATENODEALTERNATIVE)(ULONG tag);
typedef PSDP_NODE (*PCREATENODESEQUENCE)(ULONG tag);


//
// GUID_BTHDDI_SDP_NODE_INTERFACE
//
typedef struct _BTHDDI_SDP_NODE_INTERFACE {
    INTERFACE           Interface;

    PCREATENODETREEROOT SdpCreateNodeTree;
    PFREETREE           SdpFreeTree;

    PCREATENODENIL      SdpCreateNodeNil;

    PCREATENODEBOOLEAN  SdpCreateNodeBoolean;

    PCREATENODEUINT8    SdpCreateNodeUint8;
    PCREATENODEUINT16   SdpCreateNodeUint16;
    PCREATENODEUINT32   SdpCreateNodeUint32;
    PCREATENODEUINT64   SdpCreateNodeUint64;
    PCREATENODEUINT128  SdpCreateNodeUint128;

    PCREATENODEINT8     SdpCreateNodeInt8;
    PCREATENODEINT16    SdpCreateNodeInt16;
    PCREATENODEINT32    SdpCreateNodeInt32;
    PCREATENODEINT64    SdpCreateNodeInt64;
    PCREATENODEINT128   SdpCreateNodeInt128;

    PCREATENODEUUID16   SdpCreateNodeUuid16;
    PCREATENODEUUID32   SdpCreateNodeUuid32;
    PCREATENODEUUID128  SdpCreateNodeUuid128;

    PCREATENODESTRING   SdpCreateNodeString;

    PCREATENODEURL      SdpCreateNodeUrl;

    PCREATENODEALTERNATIVE SdpCreateNodeAlternative;
    PCREATENODESEQUENCE SdpCreateNodeSequence;

    PADDATTRIBUTETOTREEE SdpAddAttributeToTree;
    PAPPENDNODETOCONTAINERNODE SdpAppendNodeToContainerNode;

} BTHDDI_SDP_NODE_INTERFACE, *PBTHDDI_SDP_NODE_INTERFACE;



typedef void (*PBYTESWAPUUID128)(GUID *pUuidFrom, GUID *pUuiidTo);
typedef void (*PBYTESWAPUINT128)(PSDP_ULARGE_INTEGER_16 pInUint128,
                                 PSDP_ULARGE_INTEGER_16 pOutUint128);
typedef ULONGLONG (*PBYTESWAPUINT64)(ULONGLONG uint64);

typedef void (*PRETRIEVEUUID128)(PUCHAR Stream, GUID *uuid128);
typedef void (*PRETRIEVEUINT128)(PUCHAR Stream,
                                 PSDP_ULARGE_INTEGER_16 pUint128);
typedef void (*PRETRIEVEUINT64)(PUCHAR Stream, PULONGLONG pUint16);

typedef NTSTATUS (*PVALIDATESTREAM)(PUCHAR Stream,
                                    ULONG Size,
                                    PULONG_PTR ErrorByte);

typedef NTSTATUS (*PFINDATTRIBUTEINTREE)(PSDP_TREE_ROOT_NODE Tree,
                                         USHORT AttribId,
                                         PSDP_NODE *AttribValue);

typedef NTSTATUS (*PCONVERTTREETOSTREAM)(PSDP_TREE_ROOT_NODE Root,
                                         PUCHAR *Stream, PULONG Size, ULONG tag);
typedef NTSTATUS (*PCONVERTSTREAMTOTREE)(PUCHAR Stream,
                                         ULONG Size,
                                         PSDP_TREE_ROOT_NODE *Node,
                                         ULONG tag);
typedef VOID (*PGETNEXTELEMENT)(PUCHAR Stream,
                                ULONG StreamSize,
                                PUCHAR CurrentElement,
                                PUCHAR* NextElement,
                                PULONG NextElementSize);
typedef VOID (*pReservedFunction)();


#ifndef __BTHSDPDDIP_H__  

typedef struct _BTHDDI_SDP_PARSE_INTERFACE {
    INTERFACE               Interface;

    PVALIDATESTREAM         SdpValidateStream;

    PCONVERTSTREAMTOTREE    SdpConvertStreamToTree;
    PCONVERTTREETOSTREAM    SdpConvertTreeToStream;
    PFREETREE               SdpFreeTree;
    PBYTESWAPUUID128        SdpByteSwapUuid128;
    PBYTESWAPUINT128        SdpByteSwapUint128;
    PBYTESWAPUINT64         SdpByteSwapUint64;
    PRETRIEVEUUID128        SdpRetrieveUuid128;
    PRETRIEVEUINT128        SdpRetrieveUint128;
    PRETRIEVEUINT64         SdpRetrieveUint64;
    PFINDATTRIBUTEINTREE    SdpFindAttributeInTree;
    PGETNEXTELEMENT         SdpGetNextElement;
    pReservedFunction       Reserved1;
    pReservedFunction       Reserved2;
    pReservedFunction       Reserved3;
    pReservedFunction       Reserved4;
} BTHDDI_SDP_PARSE_INTERFACE, *PBTHDDI_SDP_PARSE_INTERFACE;



#endif


    #ifdef __cplusplus
}
#endif

#endif // (NTDDI_VERSION >= NTDDI_LONGHORN)

#endif //  __BTHSDPDDI_H__

