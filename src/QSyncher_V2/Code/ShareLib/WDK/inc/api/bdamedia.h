//------------------------------------------------------------------------------
// File: BDAMedia.h
//
// Desc: Broadcast Driver Architecture Multimedia Definitions.
//
// Copyright (c) 1996 - 2001, Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#if !defined(_KSMEDIA_)
#error KSMEDIA.H must be included before BDAMEDIA.H
#endif // !defined(_KSMEDIA_)

#if !defined(_BDATYPES_)
#error BDATYPES.H must be included before BDAMEDIA.H
#endif // !defined(_BDATYPES_)

#if !defined(_BDAMEDIA_)
#define _BDAMEDIA_

#if defined(__cplusplus)
extern "C" {
#endif // defined(__cplusplus)



//===========================================================================
//
//  KSProperty Set Structure Definitions for BDA
//
//===========================================================================

typedef struct _KSP_BDA_NODE_PIN {
    KSPROPERTY      Property;
    ULONG           ulNodeType;
    ULONG           ulInputPinId;
    ULONG           ulOutputPinId;
} KSP_BDA_NODE_PIN, *PKSP_BDA_NODE_PIN;


typedef struct _KSM_BDA_PIN
{
    KSMETHOD    Method;
    union
    {
        ULONG       PinId;
        ULONG       PinType;
    };
    ULONG       Reserved;
} KSM_BDA_PIN, * PKSM_BDA_PIN;


typedef struct _KSM_BDA_PIN_PAIR
{
    KSMETHOD    Method;
    union
    {
        ULONG       InputPinId;
        ULONG       InputPinType;
    };
    union
    {
        ULONG       OutputPinId;
        ULONG       OutputPinType;
    };
} KSM_BDA_PIN_PAIR, * PKSM_BDA_PIN_PAIR;


typedef struct {
    KSP_NODE        Property;
    ULONG           EsPid;
} KSP_NODE_ESPID, *PKSP_NODE_ESPID;



//===========================================================================
//
//  BDA Data Range definitions.  Includes specifier definitions.
//
//===========================================================================

//  Antenna Signal Formats
//

typedef struct tagKS_DATARANGE_BDA_ANTENNA {
   KSDATARANGE                  DataRange;

   //   Antenna specifier can go here if required
   //
} KS_DATARANGE_BDA_ANTENNA, *PKS_DATARANGE_BDA_ANTENNA;



//  Transport Formats
//

typedef struct tagBDA_TRANSPORT_INFO {
    ULONG           ulcbPhyiscalPacket; // Size, in bytes, of a physical packet
                                        // (e.g. Satellite link payload size.
    ULONG           ulcbPhyiscalFrame;  // Size, in bytes, of each physical frame
                                        // 0 indicates no HW requirement
    ULONG           ulcbPhyiscalFrameAlignment; // Capture buffer alignment in bytes
                                                // 0 and 1 indicate no alignment requirements
    REFERENCE_TIME  AvgTimePerFrame; // Normal ActiveMovie units (100 nS)

} BDA_TRANSPORT_INFO, *PBDA_TRANSPORT_INFO;

typedef struct tagKS_DATARANGE_BDA_TRANSPORT {
   KSDATARANGE                  DataRange;
   BDA_TRANSPORT_INFO           BdaTransportInfo;

   //   Transport specifier can go here if required
   //
} KS_DATARANGE_BDA_TRANSPORT, *PKS_DATARANGE_BDA_TRANSPORT;


//===========================================================================
//  BDA Event Guids
//
//      These are sent by the IBroadcastEvent service on the graph.
//      To receive,
//          0) Implement IBroadcastEvent in your receiving object - this has one Method on it: Fire()
//          1) QI the graphs service provider for SID_SBroadcastEventService
//                 for the IID_IBroadcastEvent object
//          2) OR create the event service (CLSID_BroadcastEventService) if not already there
//                 and register it
//          3) QI that object for it's IConnectionPoint interface (*pCP)
//          4) Advise your object on *pCP  (e.g. pCP->Advise(static_cast<IBroadCastEvent*>(this), &dwCookie)
//          5) Unadvise when done..
//          6) Implement IBroadcastEvent::Fire(GUID gEventID)
//             Check for relevant event below and deal with it appropriatly...
//===========================================================================

// {83183C03-C09E-45c4-A719-807A94952BF9}
#define STATIC_EVENTID_TuningChanging \
    0x83183c03, 0xc09e, 0x45c4, 0xa7, 0x19, 0x80, 0x7a, 0x94, 0x95, 0x2b, 0xf9
DEFINE_GUIDSTRUCT("83183C03-C09E-45c4-A719-807A94952BF9", EVENTID_TuningChanging);
#define EVENTID_TuningChanging DEFINE_GUIDNAMED(EVENTID_TuningChanging)

// {9D7E6235-4B7D-425d-A6D1-D717C33B9C4C}
#define STATIC_EVENTID_TuningChanged \
    0x9d7e6235, 0x4b7d, 0x425d, 0xa6, 0xd1, 0xd7, 0x17, 0xc3, 0x3b, 0x9c, 0x4c
DEFINE_GUIDSTRUCT("9D7E6235-4B7D-425d-A6D1-D717C33B9C4C", EVENTID_TuningChanged);
#define EVENTID_TuningChanged DEFINE_GUIDNAMED(EVENTID_TuningChanged)

// {2A65C528-2249-4070-AC16-00390CDFB2DD}
#define STATIC_EVENTID_CADenialCountChanged \
    0x2a65c528, 0x2249, 0x4070, 0xac, 0x16, 0x0, 0x39, 0xc, 0xdf, 0xb2, 0xdd
DEFINE_GUIDSTRUCT("2A65C528-2249-4070-AC16-00390CDFB2DD", EVENTID_CADenialCountChanged);
#define EVENTID_CADenialCountChanged DEFINE_GUIDNAMED(EVENTID_CADenialCountChanged)

// {6D9CFAF2-702D-4b01-8DFF-6892AD20D191}
#define STATIC_EVENTID_SignalStatusChanged \
    0x6d9cfaf2, 0x702d, 0x4b01, 0x8d, 0xff, 0x68, 0x92, 0xad, 0x20, 0xd1, 0x91
DEFINE_GUIDSTRUCT("6D9CFAF2-702D-4b01-8DFF-6892AD20D191", EVENTID_SignalStatusChanged);
#define EVENTID_SignalStatusChanged DEFINE_GUIDNAMED(EVENTID_SignalStatusChanged)

// {C87EC52D-CD18-404a-A076-C02A273D3DE7}
#define STATIC_EVENTID_NewSignalAcquired \
    0xc87ec52d, 0xcd18, 0x404a, 0xa0, 0x76, 0xc0, 0x2a, 0x27, 0x3d, 0x3d, 0xe7
DEFINE_GUIDSTRUCT("C87EC52D-CD18-404a-A076-C02A273D3DE7", EVENTID_NewSignalAcquired);
#define EVENTID_NewSignalAcquired DEFINE_GUIDNAMED(EVENTID_NewSignalAcquired)

// {D10DF9D5-C261-4b85-9E8A-517B3299CAB2}
#define STATIC_EVENTID_EASMessageReceived \
    0xd10df9d5, 0xc261, 0x4b85, 0x9e, 0x8a, 0x51, 0x7b, 0x32, 0x99, 0xca, 0xb2
DEFINE_GUIDSTRUCT("D10DF9D5-C261-4b85-9E8A-517B3299CAB2", EVENTID_EASMessageReceived);
#define EVENTID_EASMessageReceived DEFINE_GUIDNAMED(EVENTID_EASMessageReceived)

// This event is broadcasted with DireEx when a table(currently, PAT, PMT, NIT 
// and SDT for DVB; PAT, PMT, MGT and VCT for ATSC). The four parameters are:
// dwPara1 - TSID
// dwPara2 - TID|PID
// dwPara3 - dwHashedVersion
// dwPara4 - program number for PMT but not used for others
// {1B9C3703-D447-4e16-97BB-01799FC031ED}
#define STATIC_EVENTID_PSITable \
    0x1b9c3703, 0xd447, 0x4e16, 0x97, 0xbb, 0x1, 0x79, 0x9f, 0xc0, 0x31, 0xed
DEFINE_GUIDSTRUCT("1B9C3703-D447-4e16-97BB-01799FC031ED", EVENTID_PSITable);
#define EVENTID_PSITable DEFINE_GUIDNAMED(EVENTID_PSITable)

// {A265FAEA-F874-4b38-9FF7-C53D02969996}
#define STATIC_EVENTID_CardStatusChanged\
    0xa265faea, 0xf874, 0x4b38, 0x9f, 0xf7, 0xc5, 0x3d, 0x2, 0x96, 0x99, 0x96
DEFINE_GUIDSTRUCT("A265FAEA-F874-4b38-9FF7-C53D02969996", EVENTID_CardStatusChanged);
#define EVENTID_CardStatusChanged DEFINE_GUIDNAMED(EVENTID_CardStatusChanged)
#define DTV_CardStatus_Inserted      0
#define DTV_CardStatus_Removed       1
#define DTV_CardStatus_Error         2
#define DTV_CardStatus_FirmwareDownload         3

// {000906F5-F0D1-41d6-A7DF-4028697669F6}
#define STATIC_EVENTID_DRMParingStatusChanged \
    0x906f5, 0xf0d1, 0x41d6, 0xa7, 0xdf, 0x40, 0x28, 0x69, 0x76, 0x69, 0xf6
DEFINE_GUIDSTRUCT("000906F5-F0D1-41d6-A7DF-4028697669F6", EVENTID_DRMParingStatusChanged);
#define EVENTID_DRMParingStatusChanged DEFINE_GUIDNAMED(EVENTID_DRMParingStatusChanged)
// The 1st parameter to this event is a BDA_DRMPairingStatus and 2nd is the error code.

// {052C29AF-09A4-4b93-890F-BD6A348968A4}
#define STATIC_EVENTID_MMIMessage \
    0x52c29af, 0x9a4, 0x4b93, 0x89, 0xf, 0xbd, 0x6a, 0x34, 0x89, 0x68, 0xa4
DEFINE_GUIDSTRUCT("052C29AF-09A4-4b93-890F-BD6A348968A4", EVENTID_MMIMessage);
#define EVENTID_MMIMessage DEFINE_GUIDNAMED(EVENTID_MMIMessage)
#define DTV_MMIMessage_Open             0
#define DTV_MMIMessage_Close            1

// {9071AD5D-2359-4c95-8694-AFA81D70BFD5}
#define STATIC_EVENTID_EntitlementChanged \
    0x9071ad5d, 0x2359, 0x4c95, 0x86, 0x94, 0xaf, 0xa8, 0x1d, 0x70, 0xbf, 0xd5
DEFINE_GUIDSTRUCT("9071AD5D-2359-4c95-8694-AFA81D70BFD5", EVENTID_EntitlementChanged);
#define EVENTID_EntitlementChanged DEFINE_GUIDNAMED(EVENTID_EntitlementChanged)
#define DTV_Entitlement_CanDecrypt          0
#define DTV_Entitlement_NotEntitled         1
#define DTV_Entitlement_TechnicalFailure    2


// This FireEx event is fired when tuning to a STB channel number
// the first parameter passed is the channel number the STB has been tuned to
// {17C4D730-D0F0-413a-8C99-500469DE35AD}
#define STATIC_EVENTID_STBChannelNumber\
    0x17c4d730, 0xd0f0, 0x413a, 0x8c, 0x99, 0x50, 0x04, 0x69, 0xde, 0x35, 0xad
DEFINE_GUIDSTRUCT("17C4D730-D0F0-413a-8C99-500469DE35AD", EVENTID_STBChannelNumber);
#define EVENTID_STBChannelNumber DEFINE_GUIDNAMED(EVENTID_STBChannelNumber)

//===========================================================================
//
//  BDA Stream Format GUIDs
//
//===========================================================================

#define STATIC_KSDATAFORMAT_TYPE_BDA_ANTENNA\
    0x71985f41, 0x1ca1, 0x11d3, 0x9c, 0xc8, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0
DEFINE_GUIDSTRUCT("71985F41-1CA1-11d3-9CC8-00C04F7971E0", KSDATAFORMAT_TYPE_BDA_ANTENNA);
#define KSDATAFORMAT_TYPE_BDA_ANTENNA DEFINE_GUIDNAMED(KSDATAFORMAT_TYPE_BDA_ANTENNA)


#define STATIC_KSDATAFORMAT_SUBTYPE_BDA_MPEG2_TRANSPORT\
    0xf4aeb342, 0x0329, 0x4fdd, 0xa8, 0xfd, 0x4a, 0xff, 0x49, 0x26, 0xc9, 0x78
DEFINE_GUIDSTRUCT("F4AEB342-0329-4fdd-A8FD-4AFF4926C978", KSDATAFORMAT_SUBTYPE_BDA_MPEG2_TRANSPORT);
#define KSDATAFORMAT_SUBTYPE_BDA_MPEG2_TRANSPORT DEFINE_GUIDNAMED(KSDATAFORMAT_SUBTYPE_BDA_MPEG2_TRANSPORT)

#define STATIC_KSDATAFORMAT_SPECIFIER_BDA_TRANSPORT\
    0x8deda6fd, 0xac5f, 0x4334, 0x8e, 0xcf, 0xa4, 0xba, 0x8f, 0xa7, 0xd0, 0xf0
DEFINE_GUIDSTRUCT("8DEDA6FD-AC5F-4334-8ECF-A4BA8FA7D0F0", KSDATAFORMAT_SPECIFIER_BDA_TRANSPORT);
#define KSDATAFORMAT_SPECIFIER_BDA_TRANSPORT DEFINE_GUIDNAMED(KSDATAFORMAT_SPECIFIER_BDA_TRANSPORT)


#define STATIC_KSDATAFORMAT_TYPE_BDA_IF_SIGNAL\
    0x61be0b47, 0xa5eb, 0x499b, 0x9a, 0x85, 0x5b, 0x16, 0xc0, 0x7f, 0x12, 0x58
DEFINE_GUIDSTRUCT("61BE0B47-A5EB-499b-9A85-5B16C07F1258", KSDATAFORMAT_TYPE_BDA_IF_SIGNAL);
#define KSDATAFORMAT_TYPE_BDA_IF_SIGNAL DEFINE_GUIDNAMED(KSDATAFORMAT_TYPE_BDA_IF_SIGNAL)


#define STATIC_KSDATAFORMAT_TYPE_MPEG2_SECTIONS\
    0x455f176c, 0x4b06, 0x47ce, 0x9a, 0xef, 0x8c, 0xae, 0xf7, 0x3d, 0xf7, 0xb5
DEFINE_GUIDSTRUCT("455F176C-4B06-47CE-9AEF-8CAEF73DF7B5", KSDATAFORMAT_TYPE_MPEG2_SECTIONS);
#define KSDATAFORMAT_TYPE_MPEG2_SECTIONS DEFINE_GUIDNAMED(KSDATAFORMAT_TYPE_MPEG2_SECTIONS)


#define STATIC_KSDATAFORMAT_SUBTYPE_ATSC_SI\
    0xb3c7397c, 0xd303, 0x414d, 0xb3, 0x3c, 0x4e, 0xd2, 0xc9, 0xd2, 0x97, 0x33
DEFINE_GUIDSTRUCT("B3C7397C-D303-414D-B33C-4ED2C9D29733", KSDATAFORMAT_SUBTYPE_ATSC_SI);
#define KSDATAFORMAT_SUBTYPE_ATSC_SI DEFINE_GUIDNAMED(KSDATAFORMAT_SUBTYPE_ATSC_SI)


#define STATIC_KSDATAFORMAT_SUBTYPE_DVB_SI\
    0xe9dd31a3, 0x221d, 0x4adb, 0x85, 0x32, 0x9a, 0xf3, 0x9, 0xc1, 0xa4, 0x8
DEFINE_GUIDSTRUCT("e9dd31a3-221d-4adb-8532-9af309c1a408", KSDATAFORMAT_SUBTYPE_DVB_SI);
#define KSDATAFORMAT_SUBTYPE_DVB_SI DEFINE_GUIDNAMED(KSDATAFORMAT_SUBTYPE_DVB_SI)


#define STATIC_KSDATAFORMAT_SUBTYPE_BDA_OPENCABLE_PSIP\
    0x762e3f66, 0x336f, 0x48d1, 0xbf, 0x83, 0x2b, 0x0, 0x35, 0x2c, 0x11, 0xf0
DEFINE_GUIDSTRUCT("762E3F66-336F-48d1-BF83-2B00352C11F0", KSDATAFORMAT_SUBTYPE_BDA_OPENCABLE_PSIP);
#define KSDATAFORMAT_SUBTYPE_BDA_OPENCABLE_PSIP DEFINE_GUIDNAMED(KSDATAFORMAT_SUBTYPE_BDA_OPENCABLE_PSIP)

#define STATIC_KSDATAFORMAT_SUBTYPE_BDA_OPENCABLE_OOB_PSIP\
    0x951727db, 0xd2ce, 0x4528, 0x96, 0xf6, 0x33, 0x1, 0xfa, 0xbb, 0x2d, 0xe0
DEFINE_GUIDSTRUCT("951727DB-D2CE-4528-96F6-3301FABB2DE0", KSDATAFORMAT_SUBTYPE_BDA_OPENCABLE_OOB_PSIP);
#define KSDATAFORMAT_SUBTYPE_BDA_OPENCABLE_OOB_PSIP DEFINE_GUIDNAMED(KSDATAFORMAT_SUBTYPE_BDA_OPENCABLE_OOB_PSIP)

//===========================================================================
//
//  KSPinName Definitions for BDA
//
//===========================================================================

//  Pin name for a BDA transport pin
//
// {78216A81-CFA8-493e-9711-36A61C08BD9D}
//
#define STATIC_PINNAME_BDA_TRANSPORT \
    0x78216a81, 0xcfa8, 0x493e, 0x97, 0x11, 0x36, 0xa6, 0x1c, 0x8, 0xbd, 0x9d
DEFINE_GUIDSTRUCT("78216A81-CFA8-493e-9711-36A61C08BD9D", PINNAME_BDA_TRANSPORT);
#define PINNAME_BDA_TRANSPORT DEFINE_GUIDNAMED(PINNAME_BDA_TRANSPORT)


//  Pin name for a BDA analog video pin
//
// {5C0C8281-5667-486c-8482-63E31F01A6E9}
//
#define STATIC_PINNAME_BDA_ANALOG_VIDEO \
    0x5c0c8281, 0x5667, 0x486c, 0x84, 0x82, 0x63, 0xe3, 0x1f, 0x1, 0xa6, 0xe9
DEFINE_GUIDSTRUCT("5C0C8281-5667-486c-8482-63E31F01A6E9", PINNAME_BDA_ANALOG_VIDEO);
#define PINNAME_BDA_ANALOG_VIDEO DEFINE_GUIDNAMED(PINNAME_BDA_ANALOG_VIDEO)


//  Pin name for a BDA analog audio pin
//
// {D28A580A-9B1F-4b0c-9C33-9BF0A8EA636B}
//
#define STATIC_PINNAME_BDA_ANALOG_AUDIO \
    0xd28a580a, 0x9b1f, 0x4b0c, 0x9c, 0x33, 0x9b, 0xf0, 0xa8, 0xea, 0x63, 0x6b
DEFINE_GUIDSTRUCT("D28A580A-9B1F-4b0c-9C33-9BF0A8EA636B", PINNAME_BDA_ANALOG_AUDIO);
#define PINNAME_BDA_ANALOG_AUDIO DEFINE_GUIDNAMED(PINNAME_BDA_ANALOG_AUDIO)


//  Pin name for a BDA FM Radio pin
//
// {D2855FED-B2D3-4eeb-9BD0-193436A2F890}
//
#define STATIC_PINNAME_BDA_FM_RADIO \
    0xd2855fed, 0xb2d3, 0x4eeb, 0x9b, 0xd0, 0x19, 0x34, 0x36, 0xa2, 0xf8, 0x90
DEFINE_GUIDSTRUCT("D2855FED-B2D3-4eeb-9BD0-193436A2F890", PINNAME_BDA_FM_RADIO);
#define PINNAME_BDA_FM_RADIO DEFINE_GUIDNAMED(PINNAME_BDA_FM_RADIO)


//  Pin name for a BDA Intermediate Frequency pin
//
// {1A9D4A42-F3CD-48a1-9AEA-71DE133CBE14}
//
#define STATIC_PINNAME_BDA_IF_PIN \
    0x1a9d4a42, 0xf3cd, 0x48a1, 0x9a, 0xea, 0x71, 0xde, 0x13, 0x3c, 0xbe, 0x14
DEFINE_GUIDSTRUCT("1A9D4A42-F3CD-48a1-9AEA-71DE133CBE14", PINNAME_BDA_IF_PIN);
#define PINNAME_BDA_IF_PIN DEFINE_GUIDNAMED(PINNAME_BDA_IF_PIN)


//  Pin name for a BDA Open Cable PSIP pin
//
// {297BB104-E5C9-4ACE-B123-95C3CBB24D4F}
//
#define STATIC_PINNAME_BDA_OPENCABLE_PSIP_PIN \
    0x297bb104, 0xe5c9, 0x4ace, 0xb1, 0x23, 0x95, 0xc3, 0xcb, 0xb2, 0x4d, 0x4f
DEFINE_GUIDSTRUCT("297BB104-E5C9-4ACE-B123-95C3CBB24D4F", PINNAME_BDA_OPENCABLE_PSIP_PIN);
#define PINNAME_BDA_OPENCABLE_PSIP_PIN DEFINE_GUIDNAMED(PINNAME_BDA_OPENCABLE_PSIP_PIN)


//===========================================================================
//
//  KSProperty Set Definitions for BDA
//
//===========================================================================


//------------------------------------------------------------
//
//  BDA Network Ethernet Filter Property Set
//
// {71985F43-1CA1-11d3-9CC8-00C04F7971E0}
//
#define STATIC_KSPROPSETID_BdaEthernetFilter \
    0x71985f43, 0x1ca1, 0x11d3, 0x9c, 0xc8, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0
DEFINE_GUIDSTRUCT("71985F43-1CA1-11d3-9CC8-00C04F7971E0", KSPROPSETID_BdaEthernetFilter);
#define KSPROPSETID_BdaEthernetFilter DEFINE_GUIDNAMED(KSPROPSETID_BdaEthernetFilter)

typedef enum {
    KSPROPERTY_BDA_ETHERNET_FILTER_MULTICAST_LIST_SIZE = 0,
    KSPROPERTY_BDA_ETHERNET_FILTER_MULTICAST_LIST,
    KSPROPERTY_BDA_ETHERNET_FILTER_MULTICAST_MODE
} KSPROPERTY_BDA_ETHERNET_FILTER;

#define DEFINE_KSPROPERTY_ITEM_BDA_ETHERNET_FILTER_MULTICAST_LIST_SIZE(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_ETHERNET_FILTER_MULTICAST_LIST_SIZE,\
        (GetHandler),\
        sizeof(KSPROPERTY),\
        sizeof(ULONG),\
        FALSE,\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_ETHERNET_FILTER_MULTICAST_LIST(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_ETHERNET_FILTER_MULTICAST_LIST,\
        (GetHandler),\
        sizeof(KSPROPERTY),\
        sizeof(BDA_ETHERNET_ADDRESS_LIST),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_ETHERNET_FILTER_MULTICAST_MODE(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_ETHERNET_FILTER_MULTICAST_MODE,\
        (GetHandler),\
        sizeof(KSPROPERTY),\
        sizeof(BDA_MULTICAST_MODE),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)



//------------------------------------------------------------
//
//  BDA Network IPv4 Filter Property Set
//
// {71985F44-1CA1-11d3-9CC8-00C04F7971E0}
//
#define STATIC_KSPROPSETID_BdaIPv4Filter \
    0x71985f44, 0x1ca1, 0x11d3, 0x9c, 0xc8, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0
DEFINE_GUIDSTRUCT("71985F44-1CA1-11d3-9CC8-00C04F7971E0", KSPROPSETID_BdaIPv4Filter);
#define KSPROPSETID_BdaIPv4Filter DEFINE_GUIDNAMED(KSPROPSETID_BdaIPv4Filter)

typedef enum {
    KSPROPERTY_BDA_IPv4_FILTER_MULTICAST_LIST_SIZE = 0,
    KSPROPERTY_BDA_IPv4_FILTER_MULTICAST_LIST,
    KSPROPERTY_BDA_IPv4_FILTER_MULTICAST_MODE
} KSPROPERTY_BDA_IPv4_FILTER;

#define DEFINE_KSPROPERTY_ITEM_BDA_IPv4_FILTER_MULTICAST_LIST_SIZE(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_IPv4_FILTER_MULTICAST_LIST_SIZE,\
        (GetHandler),\
        sizeof(KSPROPERTY),\
        sizeof(ULONG),\
        FALSE,\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_IPv4_FILTER_MULTICAST_LIST(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_IPv4_FILTER_MULTICAST_LIST,\
        (GetHandler),\
        sizeof(KSPROPERTY),\
        sizeof(BDA_IPv4_ADDRESS_LIST),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_IPv4_FILTER_MULTICAST_MODE(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_IPv4_FILTER_MULTICAST_MODE,\
        (GetHandler),\
        sizeof(KSPROPERTY),\
        sizeof(BDA_MULTICAST_MODE),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)



//------------------------------------------------------------
//
//  BDA Network IPv6 Filter Property Set
//
// {E1785A74-2A23-4fb3-9245-A8F88017EF33}
//
#define STATIC_KSPROPSETID_BdaIPv6Filter \
    0xe1785a74, 0x2a23, 0x4fb3, 0x92, 0x45, 0xa8, 0xf8, 0x80, 0x17, 0xef, 0x33
DEFINE_GUIDSTRUCT("E1785A74-2A23-4fb3-9245-A8F88017EF33", KSPROPSETID_BdaIPv6Filter);
#define KSPROPSETID_BdaIPv6Filter DEFINE_GUIDNAMED(KSPROPSETID_BdaIPv6Filter)

typedef enum {
    KSPROPERTY_BDA_IPv6_FILTER_MULTICAST_LIST_SIZE = 0,
    KSPROPERTY_BDA_IPv6_FILTER_MULTICAST_LIST,
    KSPROPERTY_BDA_IPv6_FILTER_MULTICAST_MODE
} KSPROPERTY_BDA_IPv6_FILTER;

#define DEFINE_KSPROPERTY_ITEM_BDA_IPv6_FILTER_MULTICAST_LIST_SIZE(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_IPv6_FILTER_MULTICAST_LIST_SIZE,\
        (GetHandler),\
        sizeof(KSPROPERTY),\
        sizeof(ULONG),\
        FALSE,\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_IPv6_FILTER_MULTICAST_LIST(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_IPv6_FILTER_MULTICAST_LIST,\
        (GetHandler),\
        sizeof(KSPROPERTY),\
        sizeof(BDA_IPv6_ADDRESS_LIST),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_IPv6_FILTER_MULTICAST_MODE(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_IPv6_FILTER_MULTICAST_MODE,\
        (GetHandler),\
        sizeof(KSPROPERTY),\
        sizeof(BDA_MULTICAST_MODE),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)


//------------------------------------------------------------
//
//
//  BDA Signal Statistics Property Set
//
//  Used to get signal statistics from a control node or a pin.
//  Set NodeId == -1 to get properties from the pin.
//
//  {1347D106-CF3A-428a-A5CB-AC0D9A2A4338}
//
#define STATIC_KSPROPSETID_BdaSignalStats \
    0x1347d106, 0xcf3a, 0x428a, 0xa5, 0xcb, 0xac, 0xd, 0x9a, 0x2a, 0x43, 0x38
DEFINE_GUIDSTRUCT("1347D106-CF3A-428a-A5CB-AC0D9A2A4338", KSPROPSETID_BdaSignalStats);
#define KSPROPSETID_BdaSignalStats DEFINE_GUIDNAMED(KSPROPSETID_BdaSignalStats)

typedef enum {
    KSPROPERTY_BDA_SIGNAL_STRENGTH = 0,
    KSPROPERTY_BDA_SIGNAL_QUALITY,
    KSPROPERTY_BDA_SIGNAL_PRESENT,
    KSPROPERTY_BDA_SIGNAL_LOCKED,
    KSPROPERTY_BDA_SAMPLE_TIME,
    KSPROPERTY_BDA_SIGNAL_LOCK_CAPS,
    KSPROPERTY_BDA_SIGNAL_LOCK_TYPE
} KSPROPERTY_BDA_SIGNAL_STATS;

typedef enum _BdaLockType {
    Bda_LockType_None          = 0x00,
    Bda_LockType_PLL           = 0x01,
    Bda_LockType_DecoderDemod  = 0x02,
    Bda_LockType_Complete      = 0x80
} BDA_LockType;

//  OPTIONAL
//  Carrier strength in mDb (1/1000 of a DB).
//
//  A strength of 0 is nominal strength as expected for the given
//  type of broadcast network.
//
//  Sub-nominal strengths are reported as positive mDb
//
//  Super-nominal strengths are reported as negative mDb
//
#define DEFINE_KSPROPERTY_ITEM_BDA_SIGNAL_STRENGTH(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_SIGNAL_STRENGTH,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(LONG),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

//  OPTIONAL
//  Amount of data successfully extracted from the signal as a percent.
//
//  Signal Quality is usually reported by the demodulation node and is
//  a representation of how much of the original data could be extracted
//  from the signal.
//
//  In the case of Analog Signals, this percentage can be
//  computed by examining the timing of HSync and VSync as will as by
//  looking at information contained in HBlanking and VBlanking intervals.
//
//  In the case of Digital Signals, this percentage can be
//  computed by examining packet CRCs and FEC confidence values.
//
//  100 percent is ideal.
//  95 percent shows very little (almost unnoticable) artifacts when rendered.
//  90 percent contains few enough artifacts as to be easily viewable.
//  80 percent is the minimum level to be viewable.
//  60 percent is the minimum level to expect data services
//  (including EPG) to work.
//  20 percent indicates that the demodulator knows that a properly modulated
//  signal exists but can't produce enough data to be useful.
//
#define DEFINE_KSPROPERTY_ITEM_BDA_SIGNAL_QUALITY(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_SIGNAL_QUALITY,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(LONG),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

//  REQUIRED
//  True if a signal carrier is present.
//
//  Should be returned by the RF tuner node.
//
#define DEFINE_KSPROPERTY_ITEM_BDA_SIGNAL_PRESENT(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_SIGNAL_PRESENT,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(BOOL),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

//  REQUIRED
//  True if the signal can be locked.
//
//  Ususally represents PLL lock when returned by the RF Tuner Node.
//
//  Represents Signal Quality of at least 20% when returned by the
//  demodulator node.
//
#define DEFINE_KSPROPERTY_ITEM_BDA_SIGNAL_LOCKED(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_SIGNAL_LOCKED,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(BOOL),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

//  OPTIONAL
//  Indicates the sample time overwhich signal level and quality are
//  averaged.
//
//  Each time a signal statistics property is requested, the node should
//  report the average value for the last n milliseconds where n is the
//  value set by this property.  If no value is set or if the driver does
//  not support this property, the driver should default to
//  100 millisecond sample times.
//
//  The driver may report values for the most recently completed sample
//  period.
//
#define DEFINE_KSPROPERTY_ITEM_BDA_SAMPLE_TIME(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_SAMPLE_TIME,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(LONG),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)


//  REQUIRED
//  returns a bitmask of supported BDA_LockType_ values
//
//  Should be returned by the RF tuner node.
//
#define DEFINE_KSPROPERTY_ITEM_BDA_SIGNAL_LOCK_CAPS(GetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_SIGNAL_LOCK_CAPS,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(BOOL),\
        NULL,\
        NULL, 0, NULL, NULL, 0)

//  REQUIRED
//  returns current BDA_LockType_ value
//
//  Should be returned by the RF tuner node.
//
#define DEFINE_KSPROPERTY_ITEM_BDA_SIGNAL_LOCK_TYPE(GetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_SIGNAL_LOCK_TYPE,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(BDA_LockType),\
        NULL,\
        NULL, 0, NULL, NULL, 0)

//------------------------------------------------------------
//
//
//  BDA Change Sync Method Set
//
// {FD0A5AF3-B41D-11d2-9C95-00C04F7971E0}
//
#define STATIC_KSMETHODSETID_BdaChangeSync \
    0xfd0a5af3, 0xb41d, 0x11d2, 0x9c, 0x95, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0
DEFINE_GUIDSTRUCT("FD0A5AF3-B41D-11d2-9C95-00C04F7971E0", KSMETHODSETID_BdaChangeSync);
#define KSMETHODSETID_BdaChangeSync DEFINE_GUIDNAMED(KSMETHODSETID_BdaChangeSync)

typedef enum {
    KSMETHOD_BDA_START_CHANGES = 0,
    KSMETHOD_BDA_CHECK_CHANGES,
    KSMETHOD_BDA_COMMIT_CHANGES,
    KSMETHOD_BDA_GET_CHANGE_STATE
} KSMETHOD_BDA_CHANGE_SYNC;

#define DEFINE_KSMETHOD_ITEM_BDA_START_CHANGES(MethodHandler, SupportHandler)\
    DEFINE_KSMETHOD_ITEM(\
        KSMETHOD_BDA_START_CHANGES,\
        KSMETHOD_TYPE_NONE,\
        (MethodHandler),\
        sizeof(KSMETHOD),\
        0,\
        SupportHandler)

#define DEFINE_KSMETHOD_ITEM_BDA_CHECK_CHANGES(MethodHandler, SupportHandler)\
    DEFINE_KSMETHOD_ITEM(\
        KSMETHOD_BDA_CHECK_CHANGES,\
        KSMETHOD_TYPE_NONE,\
        (MethodHandler),\
        sizeof(KSMETHOD),\
        0,\
        SupportHandler)

#define DEFINE_KSMETHOD_ITEM_BDA_COMMIT_CHANGES(MethodHandler, SupportHandler)\
    DEFINE_KSMETHOD_ITEM(\
        KSMETHOD_BDA_COMMIT_CHANGES,\
        KSMETHOD_TYPE_NONE,\
        (MethodHandler),\
        sizeof(KSMETHOD),\
        0,\
        SupportHandler)

#define DEFINE_KSMETHOD_ITEM_BDA_GET_CHANGE_STATE(MethodHandler, SupportHandler)\
    DEFINE_KSMETHOD_ITEM(\
        KSMETHOD_BDA_GET_CHANGE_STATE,\
        KSMETHOD_TYPE_READ,\
        (MethodHandler),\
        sizeof(KSMETHOD),\
        0,\
        SupportHandler)



//------------------------------------------------------------
//
//
//  BDA Device Configuration Method Set
//
// {71985F45-1CA1-11d3-9CC8-00C04F7971E0}
//
#define STATIC_KSMETHODSETID_BdaDeviceConfiguration \
    0x71985f45, 0x1ca1, 0x11d3, 0x9c, 0xc8, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0
DEFINE_GUIDSTRUCT("71985F45-1CA1-11d3-9CC8-00C04F7971E0", KSMETHODSETID_BdaDeviceConfiguration);
#define KSMETHODSETID_BdaDeviceConfiguration DEFINE_GUIDNAMED(KSMETHODSETID_BdaDeviceConfiguration)

typedef enum {
    KSMETHOD_BDA_CREATE_PIN_FACTORY = 0,
    KSMETHOD_BDA_DELETE_PIN_FACTORY,
    KSMETHOD_BDA_CREATE_TOPOLOGY
} KSMETHOD_BDA_DEVICE_CONFIGURATION;

#define DEFINE_KSMETHOD_ITEM_BDA_CREATE_PIN_FACTORY(MethodHandler, SupportHandler)\
    DEFINE_KSMETHOD_ITEM(\
        KSMETHOD_BDA_CREATE_PIN_FACTORY,\
        KSMETHOD_TYPE_READ,\
        (MethodHandler),\
        sizeof(KSM_BDA_PIN),\
        sizeof(ULONG),\
        SupportHandler)

#define DEFINE_KSMETHOD_ITEM_BDA_DELETE_PIN_FACTORY(MethodHandler, SupportHandler)\
    DEFINE_KSMETHOD_ITEM(\
        KSMETHOD_BDA_DELETE_PIN_FACTORY,\
        KSMETHOD_TYPE_NONE,\
        (MethodHandler),\
        sizeof(KSM_BDA_PIN),\
        0,\
        SupportHandler)

#define DEFINE_KSMETHOD_ITEM_BDA_CREATE_TOPOLOGY(MethodHandler, SupportHandler)\
    DEFINE_KSMETHOD_ITEM(\
        KSMETHOD_BDA_CREATE_TOPOLOGY,\
        KSMETHOD_TYPE_WRITE,\
        (MethodHandler),\
        sizeof(KSM_BDA_PIN_PAIR),\
        0,\
        SupportHandler)



//------------------------------------------------------------
//
//
//  BDA Topology Property Set
//
// {A14EE835-0A23-11d3-9CC7-00C04F7971E0}
//
#define STATIC_KSPROPSETID_BdaTopology \
    0xa14ee835, 0x0a23, 0x11d3, 0x9c, 0xc7, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0
DEFINE_GUIDSTRUCT("A14EE835-0A23-11d3-9CC7-00C04F7971E0", KSPROPSETID_BdaTopology);
#define KSPROPSETID_BdaTopology DEFINE_GUIDNAMED(KSPROPSETID_BdaTopology)

typedef enum {
    KSPROPERTY_BDA_NODE_TYPES,
    KSPROPERTY_BDA_PIN_TYPES,
    KSPROPERTY_BDA_TEMPLATE_CONNECTIONS,
    KSPROPERTY_BDA_NODE_METHODS,
    KSPROPERTY_BDA_NODE_PROPERTIES,
    KSPROPERTY_BDA_NODE_EVENTS,
    KSPROPERTY_BDA_CONTROLLING_PIN_ID,
    KSPROPERTY_BDA_NODE_DESCRIPTORS
 }KSPROPERTY_BDA_TOPOLOGY;

#define DEFINE_KSPROPERTY_ITEM_BDA_NODE_TYPES(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_NODE_TYPES,\
        (GetHandler),\
        sizeof(KSPROPERTY),\
        0,\
        FALSE,\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_PIN_TYPES(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_PIN_TYPES,\
        (GetHandler),\
        sizeof(KSPROPERTY),\
        0,\
        FALSE,\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_TEMPLATE_CONNECTIONS(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_TEMPLATE_CONNECTIONS,\
        (GetHandler),\
        sizeof(KSPROPERTY),\
        sizeof( BDA_TEMPLATE_CONNECTION),\
        FALSE,\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_NODE_METHODS(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_NODE_METHODS,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        0,\
        FALSE,\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_NODE_PROPERTIES(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_NODE_PROPERTIES,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        0,\
        FALSE,\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_NODE_EVENTS(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_NODE_EVENTS,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        0,\
        FALSE,\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_CONTROLLING_PIN_ID(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_CONTROLLING_PIN_ID,\
        (GetHandler),\
        sizeof(KSP_BDA_NODE_PIN),\
        sizeof( ULONG),\
        FALSE,\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_NODE_DESCRIPTORS(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_NODE_DESCRIPTORS,\
        (GetHandler),\
        sizeof(KSPROPERTY),\
        0,\
        FALSE,\
        NULL, 0, NULL, NULL, 0)



//------------------------------------------------------------
//
//
//  BDA Pin Control Property Set
//
// {0DED49D5-A8B7-4d5d-97A1-12B0C195874D}
//
#define STATIC_KSPROPSETID_BdaPinControl \
    0xded49d5, 0xa8b7, 0x4d5d, 0x97, 0xa1, 0x12, 0xb0, 0xc1, 0x95, 0x87, 0x4d
DEFINE_GUIDSTRUCT("0DED49D5-A8B7-4d5d-97A1-12B0C195874D", KSPROPSETID_BdaPinControl);
#define KSPROPSETID_BdaPinControl DEFINE_GUIDNAMED(KSPROPSETID_BdaPinControl)

typedef enum {
    KSPROPERTY_BDA_PIN_ID = 0,
    KSPROPERTY_BDA_PIN_TYPE
} KSPROPERTY_BDA_PIN_CONTROL;

#define DEFINE_KSPROPERTY_ITEM_BDA_PIN_ID(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_PIN_ID,\
        (GetHandler),\
        sizeof( KSPROPERTY),\
        sizeof( ULONG),\
        FALSE,\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_PIN_TYPE(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_PIN_TYPE,\
        (GetHandler),\
        sizeof( KSPROPERTY),\
        sizeof( ULONG),\
        FALSE,\
        NULL, 0, NULL, NULL, 0)



//------------------------------------------------------------
//
//
//  BDA Pin Event Set
//
// {104781CD-50BD-40d5-95FB-087E0E86A591}
//
#define STATIC_KSEVENTSETID_BdaPinEvent \
    0x104781cd, 0x50bd, 0x40d5, 0x95, 0xfb, 0x08, 0x7e, 0xe, 0x86, 0xa5, 0x91
DEFINE_GUIDSTRUCT("104781CD-50BD-40d5-95FB-087E0E86A591", KSEVENTSETID_BdaPinEvent);
#define KSEVENTSETID_BdaPinEvent DEFINE_GUIDNAMED(KSEVENTSETID_BdaPinEvent)

typedef enum {
    KSEVENT_BDA_PIN_CONNECTED = 0,
    KSEVENT_BDA_PIN_DISCONNECTED
} KSPROPERTY_BDA_PIN_EVENT;

#define DEFINE_KSEVENT_ITEM_BDA_PIN_CONNECTED(AddHandler, RemoveHandler, SupportHandler)\
    DEFINE_KSEVENT_ITEM(\
        KSEVENT_BDA_PIN_CONNECTED,\
        sizeof( KSEVENTDATA), \
        0, \
        (AddHandler),\
        (RemoveHandler),\
        (SupportHandler)\
        )

#define DEFINE_KSEVENT_ITEM_BDA_PIN_DISCONNECTED(AddHandler, RemoveHandler, SupportHandler)\
    DEFINE_KSEVENT_ITEM(\
        KSEVENT_BDA_PIN_DISCONNECTED,\
        sizeof( KSEVENTDATA), \
        0, \
        (AddHandler),\
        (RemoveHandler),\
        (SupportHandler)\
        )



//------------------------------------------------------------
//
//
//  BDA Void Transform Property Set
//
// {71985F46-1CA1-11d3-9CC8-00C04F7971E0}
//
#define STATIC_KSPROPSETID_BdaVoidTransform \
    0x71985f46, 0x1ca1, 0x11d3, 0x9c, 0xc8, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0
DEFINE_GUIDSTRUCT("71985F46-1CA1-11d3-9CC8-00C04F7971E0", KSPROPSETID_BdaVoidTransform);
#define KSPROPSETID_BdaVoidTransform DEFINE_GUIDNAMED(KSPROPSETID_BdaVoidTransform)

typedef enum {
    KSPROPERTY_BDA_VOID_TRANSFORM_START = 0,
    KSPROPERTY_BDA_VOID_TRANSFORM_STOP
} KSPROPERTY_BDA_VOID_TRANSFORM;

#define DEFINE_KSPROPERTY_ITEM_BDA_VOID_TRANSFORM_START(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_VOID_TRANSFORM_START,\
        FALSE,\
        sizeof(KSPROPERTY),\
        0,\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_VOID_TRANSFORM_STOP(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_VOID_TRANSFORM_STOP,\
        FALSE,\
        sizeof(KSPROPERTY),\
        0,\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)



//------------------------------------------------------------
//
//
//  BDA Null Transform Property Set
//
// {DDF15B0D-BD25-11d2-9CA0-00C04F7971E0}
//
#define STATIC_KSPROPSETID_BdaNullTransform \
    0xddf15b0d, 0xbd25, 0x11d2, 0x9c, 0xa0, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0
DEFINE_GUIDSTRUCT("DDF15B0D-BD25-11d2-9CA0-00C04F7971E0", KSPROPSETID_BdaNullTransform);
#define KSPROPSETID_BdaNullTransform DEFINE_GUIDNAMED(KSPROPSETID_BdaNullTransform)

typedef enum {
    KSPROPERTY_BDA_NULL_TRANSFORM_START = 0,
    KSPROPERTY_BDA_NULL_TRANSFORM_STOP
} KSPROPERTY_BDA_NULL_TRANSFORM;

#define DEFINE_KSPROPERTY_ITEM_BDA_NULL_TRANSFORM_START(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_NULL_TRANSFORM_START,\
        FALSE,\
        sizeof(KSPROPERTY),\
        0,\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_NULL_TRANSFORM_STOP(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_NULL_TRANSFORM_STOP,\
        FALSE,\
        sizeof(KSPROPERTY),\
        0,\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)



//------------------------------------------------------------
//
//
//  BDA Frequency Filter Property Set
//
// {71985F47-1CA1-11d3-9CC8-00C04F7971E0}
//
#define STATIC_KSPROPSETID_BdaFrequencyFilter \
    0x71985f47, 0x1ca1, 0x11d3, 0x9c, 0xc8, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0
DEFINE_GUIDSTRUCT("71985F47-1CA1-11d3-9CC8-00C04F7971E0", KSPROPSETID_BdaFrequencyFilter);
#define KSPROPSETID_BdaFrequencyFilter DEFINE_GUIDNAMED(KSPROPSETID_BdaFrequencyFilter)

typedef enum {
    KSPROPERTY_BDA_RF_TUNER_FREQUENCY = 0,
    KSPROPERTY_BDA_RF_TUNER_POLARITY,
    KSPROPERTY_BDA_RF_TUNER_RANGE,
    KSPROPERTY_BDA_RF_TUNER_TRANSPONDER,
    KSPROPERTY_BDA_RF_TUNER_BANDWIDTH,
    KSPROPERTY_BDA_RF_TUNER_FREQUENCY_MULTIPLIER,
    KSPROPERTY_BDA_RF_TUNER_CAPS,
    KSPROPERTY_BDA_RF_TUNER_SCAN_STATUS,
    KSPROPERTY_BDA_RF_TUNER_STANDARD,
    KSPROPERTY_BDA_RF_TUNER_STANDARD_MODE
} KSPROPERTY_BDA_FREQUENCY_FILTER;

typedef enum _BdaSignalType {
    Bda_SignalType_Unknown =  0,
    Bda_SignalType_Analog  =  1,
    Bda_SignalType_Digital =  2
} BDA_SignalType;

// 
// The BDA_DigitalSignalStandard enumeration is tentatively put out for Beta review
// Based on feedback, this may be updated or entirely removed in a later release
//
typedef enum
{
    Bda_DigitalStandard_None     = 0x00000000,  
    Bda_DigitalStandard_DVB_T    = 0x00000001,
    Bda_DigitalStandard_DVB_S    = 0x00000002,
    Bda_DigitalStandard_DVB_C    = 0x00000004,
    Bda_DigitalStandard_ATSC     = 0x00000008,
    Bda_DigitalStandard_ISDB_T   = 0x00000010,
    Bda_DigitalStandard_ISDB_S   = 0x00000020,
    Bda_DigitalStandard_ISDB_C   = 0x00000040
} BDA_DigitalSignalStandard;

typedef struct {
    KSP_NODE Property;
    ULONG  Mode;                        // IN: KSPROPERTY_TUNER_MODE
    ULONG  AnalogStandardsSupported;    // Bda_AnalogStandard_* (if TV or DSS)
    ULONG  DigitalStandardsSupported;   // Bda_DigitalStandard_*
    ULONG  MinFrequency;                // R - Hz
    ULONG  MaxFrequency;                // R - Hz
    ULONG  SettlingTime;                // R - milliSeconds to settle for any sort of update to the tuner
    ULONG  AnalogSensingRange;          // R - max range (kHz) in which tuner can detect an analog signal 
    ULONG  DigitalSensingRange;         // R - max range (kHz) in which tuner can detect an digital signal
    ULONG  MilliSecondsPerMHz;          // R - max time to lock in to a signal 1MHz away assuming the signal has been detected, but its offset is unknown
} KSPROPERTY_BDA_RF_TUNER_CAPS_S, *PKSPROPERTY_BDA_RF_TUNER_CAPS_S;

typedef struct {
    KSP_NODE Property;
    ULONG CurrentFrequency;          // R - current frequency
    ULONG FrequencyRangeMin;         // R - lower limit of range left to scan
    ULONG FrequencyRangeMax;         // R - upper limit of range left to scan
    ULONG MilliSecondsLeft;          // R - time left to complete scanning
} KSPROPERTY_BDA_RF_TUNER_SCAN_STATUS_S, *PKSPROPERTY_BDA_RF_TUNER_SCAN_STATUS_S;

typedef struct {
    KSP_NODE Property;
    BDA_SignalType SignalType;       // RW - specifies whether the signal is Analog or Digital. this is required to interpret the SignalStandard field
    ULONG  SignalStandard;           // RW - current signal standard (KS_AnalogVideo_* or Bda_DigitalStandard_*) set by the application or detected by the device
} KSPROPERTY_BDA_RF_TUNER_STANDARD_S, *PKSPROPERTY_BDA_RF_TUNER_STANDARD_S;

typedef struct {
    KSP_NODE Property;
    BOOL AutoDetect;                 // RW - specifies whether the driver is in auto-detect mode for the signal standard
} KSPROPERTY_BDA_RF_TUNER_STANDARD_MODE_S, *PKSPROPERTY_BDA_RF_TUNER_STANDARD_MODE_S;

#define DEFINE_KSPROPERTY_ITEM_BDA_RF_TUNER_FREQUENCY(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_RF_TUNER_FREQUENCY,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(ULONG),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_RF_TUNER_POLARITY(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_RF_TUNER_POLARITY,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(ULONG),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_RF_TUNER_RANGE(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_RF_TUNER_RANGE,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(ULONG),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_RF_TUNER_TRANSPONDER(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_RF_TUNER_TRANSPONDER,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(ULONG),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_RF_TUNER_BANDWIDTH(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_RF_TUNER_BANDWIDTH,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(ULONG),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_RF_TUNER_FREQUENCY_MULTIPLIER(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_RF_TUNER_FREQUENCY_MULTIPLIER,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(ULONG),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

//------------------------------------------------------------
//
//
//  BDA Tuner Event Set
//
// {AAB59E17-01C9-4ebf-93F2-FC3B79B46F91}
//
#define STATIC_KSEVENTSETID_BdaTunerEvent \
    0xaab59e17, 0x1c9, 0x4ebf, 0x93, 0xf2, 0xfc, 0x3b, 0x79, 0xb4, 0x6f, 0x91
DEFINE_GUIDSTRUCT("AAB59E17-01C9-4ebf-93F2-FC3B79B46F91", KSEVENTSETID_BdaTunerEvent);
#define KSEVENTSETID_BdaTunerEvent DEFINE_GUIDNAMED(KSEVENTSETID_BdaTunerEvent)

typedef enum {
    KSEVENT_BDA_TUNER_SCAN = 0
} KSEVENT_BDA_TUNER;

typedef struct {
    KSEVENTDATA EventData;
    ULONG StartFrequency;          // W - initial frequency for the scan
    ULONG EndFrequency;            // W - final frequency for the scan, can be less than the initial value indicating a "scan down" is requested
    BDA_LockType LockRequested;    // W - whether the device should look for just a PLL lock, decoder lock, etc. should be a supported lock type.
} KSEVENTDATA_BDA_RF_TUNER_SCAN_S, *PKSEVENTDATA_BDA_RF_TUNER_SCAN_S;

//------------------------------------------------------------
//
//
//  BDA LNB Info Property Set
//
// {992CF102-49F9-4719-A664-C4F23E2408F4}
//
#define STATIC_KSPROPSETID_BdaLNBInfo \
    0x992cf102, 0x49f9, 0x4719, 0xa6, 0x64, 0xc4, 0xf2, 0x3e, 0x24, 0x8, 0xf4
DEFINE_GUIDSTRUCT("992CF102-49F9-4719-A664-C4F23E2408F4", KSPROPSETID_BdaLNBInfo);
#define KSPROPSETID_BdaLNBInfo DEFINE_GUIDNAMED(KSPROPSETID_BdaLNBInfo)

typedef enum {
    KSPROPERTY_BDA_LNB_LOF_LOW_BAND = 0,
    KSPROPERTY_BDA_LNB_LOF_HIGH_BAND,
    KSPROPERTY_BDA_LNB_SWITCH_FREQUENCY
} KSPROPERTY_BDA_LNB_INFO;

#define DEFINE_KSPROPERTY_ITEM_BDA_LNB_LOF_LOW_BAND(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_LNB_LOF_LOW_BAND,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(ULONG),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_LNB_LOF_HIGH_BAND(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_LNB_LOF_HIGH_BAND,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(ULONG),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_LNB_SWITCH_FREQUENCY(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_LNB_SWITCH_FREQUENCY,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(ULONG),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)



//------------------------------------------------------------
//
//
//  BDA Digital Demodulator Property Set
//
// {EF30F379-985B-4d10-B640-A79D5E04E1E0}
//
#define STATIC_KSPROPSETID_BdaDigitalDemodulator \
    0xef30f379, 0x985b, 0x4d10, 0xb6, 0x40, 0xa7, 0x9d, 0x5e, 0x4, 0xe1, 0xe0
DEFINE_GUIDSTRUCT("EF30F379-985B-4d10-B640-A79D5E04E1E0", KSPROPSETID_BdaDigitalDemodulator);
#define KSPROPSETID_BdaDigitalDemodulator DEFINE_GUIDNAMED(KSPROPSETID_BdaDigitalDemodulator)

typedef enum {
    KSPROPERTY_BDA_MODULATION_TYPE = 0,
    KSPROPERTY_BDA_INNER_FEC_TYPE,
    KSPROPERTY_BDA_INNER_FEC_RATE,
    KSPROPERTY_BDA_OUTER_FEC_TYPE,
    KSPROPERTY_BDA_OUTER_FEC_RATE,
    KSPROPERTY_BDA_SYMBOL_RATE,
    KSPROPERTY_BDA_SPECTRAL_INVERSION,
    KSPROPERTY_BDA_GUARD_INTERVAL,
    KSPROPERTY_BDA_TRANSMISSION_MODE,
    KSPROPERTY_BDA_ROLL_OFF,
    KSPROPERTY_BDA_PILOT
} KSPROPERTY_BDA_DIGITAL_DEMODULATOR;

#define DEFINE_KSPROPERTY_ITEM_BDA_MODULATION_TYPE(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_MODULATION_TYPE,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(ModulationType),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_INNER_FEC_TYPE(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_INNER_FEC_TYPE,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(FECMethod),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_INNER_FEC_RATE(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_INNER_FEC_RATE,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(BinaryConvolutionCodeRate),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_OUTER_FEC_TYPE(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_OUTER_FEC_TYPE,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(FECMethod),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_OUTER_FEC_RATE(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_OUTER_FEC_RATE,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(BinaryConvolutionCodeRate),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_SYMBOL_RATE(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_SYMBOL_RATE,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(ULONG),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_SPECTRAL_INVERSION(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_SPECTRAL_INVERSION,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(SpectralInversion),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_GUARD_INTERVAL(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_GUARD_INTERVAL,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(GuardInterval),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_TRANSMISSION_MODE(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_TRANSMISSION_MODE,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(TransmissionMode),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_ROLL_OFF(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_ROLL_OFF,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(RollOff),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_PILOT(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_PILOT,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(Pilot),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)


//------------------------------------------------------------
//
//
//  BDA Autodemodulate Property Set
//
// {DDF15B12-BD25-11d2-9CA0-00C04F7971E0}
//
#define STATIC_KSPROPSETID_BdaAutodemodulate \
    0xddf15b12, 0xbd25, 0x11d2, 0x9c, 0xa0, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0
DEFINE_GUIDSTRUCT("DDF15B12-BD25-11d2-9CA0-00C04F7971E0", KSPROPSETID_BdaAutodemodulate);
#define KSPROPSETID_BdaAutodemodulate DEFINE_GUIDNAMED(KSPROPSETID_BdaAutodemodulate)

typedef enum {
    KSPROPERTY_BDA_AUTODEMODULATE_START = 0,
    KSPROPERTY_BDA_AUTODEMODULATE_STOP
} KSPROPERTY_BDA_AUTODEMODULATE;

#define DEFINE_KSPROPERTY_ITEM_BDA_AUTODEMODULATE_START(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_AUTODEMODULATE_START,\
        FALSE,\
        sizeof(KSP_NODE),\
        0,\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_AUTODEMODULATE_STOP(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_AUTODEMODULATE_STOP,\
        FALSE,\
        sizeof(KSP_NODE),\
        0,\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

//------------------------------------------------------------
//
//
//  BDA Table Section Property Set
//
// {516B99C5-971C-4aaf-B3F3-D9FDA8A15E16}
//

#define STATIC_KSPROPSETID_BdaTableSection \
    0x516b99c5, 0x971c, 0x4aaf, 0xb3, 0xf3, 0xd9, 0xfd, 0xa8, 0xa1, 0x5e, 0x16
DEFINE_GUIDSTRUCT("516B99C5-971C-4aaf-B3F3-D9FDA8A15E16", KSPROPSETID_BdaTableSection);
#define KSPROPSETID_BdaTableSection DEFINE_GUIDNAMED(KSPROPSETID_BdaTableSection)

typedef enum {
    KSPROPERTY_BDA_TABLE_SECTION = 0,
} KSPROPERTY_IDS_BDA_TABLE;

#define DEFINE_KSPROPERTY_ITEM_BDA_TABLE_SECTION(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_TABLE_SECTION,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(BDA_TABLE_SECTION),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)



//------------------------------------------------------------
//
//
//  BDA PID Filter Property Set
//
// {D0A67D65-08DF-4fec-8533-E5B550410B85}
//
#define STATIC_KSPROPSETID_BdaPIDFilter \
    0xd0a67d65, 0x8df, 0x4fec, 0x85, 0x33, 0xe5, 0xb5, 0x50, 0x41, 0xb, 0x85
DEFINE_GUIDSTRUCT("D0A67D65-08DF-4fec-8533-E5B550410B85", KSPROPSETID_BdaPIDFilter);
#define KSPROPSETID_BdaPIDFilter DEFINE_GUIDNAMED(KSPROPSETID_BdaPIDFilter)

typedef enum {
    KSPROPERTY_BDA_PIDFILTER_MAP_PIDS = 0,
    KSPROPERTY_BDA_PIDFILTER_UNMAP_PIDS,
    KSPROPERTY_BDA_PIDFILTER_LIST_PIDS
} KSPROPERTY_BDA_PIDFILTER;

#define DEFINE_KSPROPERTY_ITEM_BDA_PIDFILTER_MAP_PIDS(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_PIDFILTER_MAP_PIDS,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(BDA_PID_MAP),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_PIDFILTER_UNMAP_PIDS(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_PIDFILTER_UNMAP_PIDS,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(BDA_PID_UNMAP),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_PIDFILTER_LIST_PIDS(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_PIDFILTER_LIST_PIDS,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        0,\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)



//------------------------------------------------------------
//
//
//  BDA CA Property Set
//
// {B0693766-5278-4ec6-B9E1-3CE40560EF5A}
//
#define STATIC_KSPROPSETID_BdaCA \
    0xb0693766, 0x5278, 0x4ec6, 0xb9, 0xe1, 0x3c, 0xe4, 0x5, 0x60, 0xef, 0x5a
DEFINE_GUIDSTRUCT("B0693766-5278-4ec6-B9E1-3CE40560EF5A", KSPROPSETID_BdaCA);
#define KSPROPSETID_BdaCA DEFINE_GUIDNAMED(KSPROPSETID_BdaCA)

typedef enum {
    KSPROPERTY_BDA_ECM_MAP_STATUS = 0,
    KSPROPERTY_BDA_CA_MODULE_STATUS,
    KSPROPERTY_BDA_CA_SMART_CARD_STATUS,
    KSPROPERTY_BDA_CA_MODULE_UI,
    KSPROPERTY_BDA_CA_SET_PROGRAM_PIDS,
    KSPROPERTY_BDA_CA_REMOVE_PROGRAM
} KSPROPERTY_BDA_CA;

#define DEFINE_KSPROPERTY_ITEM_BDA_ECM_MAP_STATUS(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_ECM_MAP_STATUS,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(ULONG),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_CA_MODULE_STATUS(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_CA_MODULE_STATUS,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(ULONG),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_CA_SMART_CARD_STATUS(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_CA_SMART_CARD_STATUS,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(ULONG),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_CA_MODULE_UI(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_CA_MODULE_UI,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(BDA_CA_MODULE_UI),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_CA_SET_PROGRAM_PIDS(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_CA_SET_PROGRAM_PIDS,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(BDA_PROGRAM_PID_LIST),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_KSPROPERTY_ITEM_BDA_CA_REMOVE_PROGRAM(GetHandler, SetHandler)\
    DEFINE_KSPROPERTY_ITEM(\
        KSPROPERTY_BDA_CA_REMOVE_PROGRAM,\
        (GetHandler),\
        sizeof(KSP_NODE),\
        sizeof(ULONG),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)



//------------------------------------------------------------
//
//
//  BDA CA Event Set
//
// {488C4CCC-B768-4129-8EB1-B00A071F9068}
//
#define STATIC_KSEVENTSETID_BdaCAEvent \
    0x488c4ccc, 0xb768, 0x4129, 0x8e, 0xb1, 0xb0, 0xa, 0x7, 0x1f, 0x90, 0x68
DEFINE_GUIDSTRUCT("488C4CCC-B768-4129-8EB1-B00A071F9068", KSEVENTSETID_BdaCAEvent);
#define KSEVENTSETID_BdaCAEvent DEFINE_GUIDNAMED(KSEVENTSETID_BdaCAEvent)

typedef enum {
    KSEVENT_BDA_PROGRAM_FLOW_STATUS_CHANGED = 0,
    KSEVENT_BDA_CA_MODULE_STATUS_CHANGED,
    KSEVENT_BDA_CA_SMART_CARD_STATUS_CHANGED,
    KSEVENT_BDA_CA_MODULE_UI_REQUESTED
} KSPROPERTY_BDA_CA_EVENT;

#define DEFINE_KSEVENT_BDA_PROGRAM_FLOW_STATUS_CHANGED(AddHandler, RemoveHandler, SupportHandler)\
    DEFINE_KSEVENT_ITEM(\
        KSEVENT_BDA_PROGRAM_FLOW_STATUS_CHANGED,\
        sizeof( KSEVENTDATA), \
        0, \
        (AddHandler),\
        (RemoveHandler),\
        (SupportHandler)\
        )

#define DEFINE_KSEVENT_BDA_CA_MODULE_STATUS_CHANGED(AddHandler, RemoveHandler, SupportHandler)\
    DEFINE_KSEVENT_ITEM(\
        KSEVENT_BDA_CA_MODULE_STATUS_CHANGED,\
        sizeof( KSEVENTDATA), \
        0, \
        (AddHandler),\
        (RemoveHandler),\
        (SupportHandler)\
        )

#define DEFINE_KSEVENT_BDA_CA_SMART_CARD_STATUS_CHANGED(AddHandler, RemoveHandler, SupportHandler)\
    DEFINE_KSEVENT_ITEM(\
        KSEVENT_BDA_CA_SMART_CARD_STATUS_CHANGED,\
        sizeof( KSEVENTDATA), \
        0, \
        (AddHandler),\
        (RemoveHandler),\
        (SupportHandler)\
        )

#define DEFINE_KSEVENT_BDA_CA_MODULE_UI_REQUESTED(AddHandler, RemoveHandler, SupportHandler)\
    DEFINE_KSEVENT_ITEM(\
        KSEVENT_BDA_CA_MODULE_UI_REQUESTED,\
        sizeof( KSEVENTDATA), \
        0, \
        (AddHandler),\
        (RemoveHandler),\
        (SupportHandler)\
        )


//===========================================================================
//
// BDA Filter Categories
//
//===========================================================================

#define STATIC_KSCATEGORY_BDA_RECEIVER_COMPONENT \
    0xFD0A5AF4, 0xB41D, 0x11d2, 0x9c, 0x95, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0
DEFINE_GUIDSTRUCT("FD0A5AF4-B41D-11d2-9C95-00C04F7971E0", KSCATEGORY_BDA_RECEIVER_COMPONENT);
#define KSCATEGORY_BDA_RECEIVER_COMPONENT DEFINE_GUIDNAMED(KSCATEGORY_BDA_RECEIVER_COMPONENT)


#define STATIC_KSCATEGORY_BDA_NETWORK_TUNER \
    0x71985f48, 0x1ca1, 0x11d3, 0x9c, 0xc8, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0
DEFINE_GUIDSTRUCT("71985F48-1CA1-11d3-9CC8-00C04F7971E0", KSCATEGORY_BDA_NETWORK_TUNER);
#define KSCATEGORY_BDA_NETWORK_TUNER DEFINE_GUIDNAMED(KSCATEGORY_BDA_NETWORK_TUNER)


#define STATIC_KSCATEGORY_BDA_NETWORK_EPG \
    0x71985f49, 0x1ca1, 0x11d3, 0x9c, 0xc8, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0
DEFINE_GUIDSTRUCT("71985F49-1CA1-11d3-9CC8-00C04F7971E0", KSCATEGORY_BDA_NETWORK_EPG);
#define KSCATEGORY_BDA_NETWORK_EPG DEFINE_GUIDNAMED(KSCATEGORY_BDA_NETWORK_EPG)


#define STATIC_KSCATEGORY_BDA_IP_SINK \
    0x71985f4aL, 0x1ca1, 0x11d3, 0x9c, 0xc8, 0x00, 0xc0, 0x4f, 0x79, 0x71, 0xe0
DEFINE_GUIDSTRUCT("71985F4A-1CA1-11d3-9CC8-00C04F7971E0", KSCATEGORY_BDA_IP_SINK);
#define KSCATEGORY_IP_SINK DEFINE_GUIDNAMED(KSCATEGORY_BDA_IP_SINK)


#define STATIC_KSCATEGORY_BDA_NETWORK_PROVIDER \
    0x71985f4b, 0x1ca1, 0x11d3, 0x9c, 0xc8, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0
DEFINE_GUIDSTRUCT("71985F4B-1CA1-11d3-9CC8-00C04F7971E0", KSCATEGORY_BDA_NETWORK_PROVIDER);
#define KSCATEGORY_BDA_NETWORK_PROVIDER DEFINE_GUIDNAMED(KSCATEGORY_BDA_NETWORK_PROVIDER)

// {A2E3074F-6C3D-11d3-B653-00C04F79498E}
#define STATIC_KSCATEGORY_BDA_TRANSPORT_INFORMATION \
        0xa2e3074f, 0x6c3d, 0x11d3, 0xb6, 0x53, 0x0, 0xc0, 0x4f, 0x79, 0x49, 0x8e
DEFINE_GUIDSTRUCT("A2E3074F-6C3D-11d3-B653-00C04F79498E", KSCATEGORY_BDA_TRANSPORT_INFORMATION);
#define KSCATEGORY_BDA_TRANSPORT_INFORMATION DEFINE_GUIDNAMED(KSCATEGORY_BDA_TRANSPORT_INFORMATION)

//===========================================================================
//
// BDA Node Categories
//
//===========================================================================

//
// Typical usage of the node categories to define supported tuner device standards.
//
// Analog (only)
//   KSNODE_BDA_RF_TUNER
//
// ATSC (only):
//   KSNODE_BDA_8VSB_DEMODULATOR node
//
// DVB-T (only):
//   KSNODE_BDA_COFDM_DEMODULATOR
//
// DVB-S (only)
//   KSNODE_BDA_QPSK_DEMODULATOR
//
// DVB-S2 (only)
//   KSNODE_BDA_8PSK_DEMODULATOR
//
// Digital Cable (both DVB-C and US):
//   KSNODE_BDA_QAM_DEMODULATOR node
//
// Hybrid Analog/ATSC:
//   KSNODE_BDA_RF_TUNER & KSNODE_BDA_8VSB_DEMODULATOR nodes
//
// Hybrid Analog/Digital Cable:
//   KSNODE_BDA_RF_TUNER & KSNODE_BDA_QAM_DEMODULATOR nodes
//
// Hybrid Analog and Digital Cable w/ CableCard:
//   KSNODE_BDA_RF_TUNER & KSNODE_BDA_QAM_DEMODULATOR & KSNODE_BDA_OPENCABLE_POD nodes
//
// Hybrid Analog and DVB-T:
//   KSNODE_BDA_RF_TUNER & KSNODE_BDA_COFDM_DEMODULATOR
//

#define STATIC_KSNODE_BDA_RF_TUNER \
    0x71985f4c, 0x1ca1, 0x11d3, 0x9c, 0xc8, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0
DEFINE_GUIDSTRUCT("71985F4C-1CA1-11d3-9CC8-00C04F7971E0", KSNODE_BDA_RF_TUNER);
#define KSNODE_BDA_RF_TUNER DEFINE_GUIDNAMED(KSNODE_BDA_RF_TUNER)

#define STATIC_KSNODE_BDA_ANALOG_DEMODULATOR \
    0x634db199, 0x27dd, 0x46b8, 0xac, 0xfb, 0xec, 0xc9, 0x8e, 0x61, 0xa2, 0xad
DEFINE_GUIDSTRUCT("634DB199-27DD-46b8-ACFB-ECC98E61A2AD",  KSNODE_BDA_ANALOG_DEMODULATOR);
#define  KSNODE_BDA_ANALOG_DEMODULATOR DEFINE_GUIDNAMED( KSNODE_BDA_ANALOG_DEMODULATOR)


#define STATIC_KSNODE_BDA_QAM_DEMODULATOR \
    0x71985f4d, 0x1ca1, 0x11d3, 0x9c, 0xc8, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0
DEFINE_GUIDSTRUCT("71985F4D-1CA1-11d3-9CC8-00C04F7971E0", KSNODE_BDA_QAM_DEMODULATOR);
#define KSNODE_BDA_QAM_DEMODULATOR DEFINE_GUIDNAMED(KSNODE_BDA_QAM_DEMODULATOR)


#define STATIC_KSNODE_BDA_QPSK_DEMODULATOR \
    0x6390c905, 0x27c1, 0x4d67, 0xbd, 0xb7, 0x77, 0xc5, 0xd, 0x7, 0x93, 0x0
DEFINE_GUIDSTRUCT("6390C905-27C1-4d67-BDB7-77C50D079300", KSNODE_BDA_QPSK_DEMODULATOR);
#define KSNODE_BDA_QPSK_DEMODULATOR DEFINE_GUIDNAMED(KSNODE_BDA_QPSK_DEMODULATOR)


#define STATIC_KSNODE_BDA_8VSB_DEMODULATOR \
    0x71985f4f, 0x1ca1, 0x11d3, 0x9c, 0xc8, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0
DEFINE_GUIDSTRUCT("71985F4F-1CA1-11d3-9CC8-00C04F7971E0", KSNODE_BDA_8VSB_DEMODULATOR);
#define KSNODE_BDA_8VSB_DEMODULATOR DEFINE_GUIDNAMED(KSNODE_BDA_8VSB_DEMODULATOR)


#define STATIC_KSNODE_BDA_COFDM_DEMODULATOR \
    0x2dac6e05, 0xedbe, 0x4b9c, 0xb3, 0x87, 0x1b, 0x6f, 0xad, 0x7d, 0x64, 0x95
DEFINE_GUIDSTRUCT("2DAC6E05-EDBE-4b9c-B387-1B6FAD7D6495", KSNODE_BDA_COFDM_DEMODULATOR);
#define KSNODE_BDA_COFDM_DEMODULATOR DEFINE_GUIDNAMED(KSNODE_BDA_COFDM_DEMODULATOR)


#define STATIC_KSNODE_BDA_8PSK_DEMODULATOR \
    0xe957a0e7, 0xdd98, 0x4a3c, 0x81, 0x0b, 0x35, 0x25, 0x15, 0x7a, 0xb6, 0x2e
DEFINE_GUIDSTRUCT("E957A0E7-DD98-4A3C-810B-3525157AB62E", KSNODE_BDA_8PSK_DEMODULATOR);
#define KSNODE_BDA_8PSK_DEMODULATOR DEFINE_GUIDNAMED(KSNODE_BDA_8PSK_DEMODULATOR)


#define STATIC_KSNODE_BDA_OPENCABLE_POD \
    0x345812a0, 0xfb7c, 0x4790, 0xaa, 0x7e, 0xb1, 0xdb, 0x88, 0xac, 0x19, 0xc9
DEFINE_GUIDSTRUCT("345812A0-FB7C-4790-AA7E-B1DB88AC19C9", KSNODE_BDA_OPENCABLE_POD);
#define KSNODE_BDA_OPENCABLE_POD DEFINE_GUIDNAMED(KSNODE_BDA_OPENCABLE_POD)


#define STATIC_KSNODE_BDA_COMMON_CA_POD \
    0xd83ef8fc, 0xf3b8, 0x45ab, 0x8b, 0x71, 0xec, 0xf7, 0xc3, 0x39, 0xde, 0xb4
DEFINE_GUIDSTRUCT("D83EF8FC-F3B8-45ab-8B71-ECF7C339DEB4", KSNODE_BDA_COMMON_CA_POD);
#define KSNODE_BDA_COMMON_CA_POD DEFINE_GUIDNAMED(KSNODE_BDA_COMMON_CA_POD)


#define STATIC_KSNODE_BDA_PID_FILTER \
    0xf5412789, 0xb0a0, 0x44e1, 0xae, 0x4f, 0xee, 0x99, 0x9b, 0x1b, 0x7f, 0xbe
DEFINE_GUIDSTRUCT("F5412789-B0A0-44e1-AE4F-EE999B1B7FBE", KSNODE_BDA_PID_FILTER);
#define KSNODE_BDA_PID_FILTER DEFINE_GUIDNAMED(KSNODE_BDA_PID_FILTER)


#define STATIC_KSNODE_BDA_IP_SINK \
    0x71985f4e, 0x1ca1, 0x11d3, 0x9c, 0xc8, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0
DEFINE_GUIDSTRUCT("71985F4E-1CA1-11d3-9CC8-00C04F7971E0", KSNODE_BDA_IP_SINK);
#define KSNODE_IP_SINK DEFINE_GUIDNAMED(KSNODE_BDA_IP_SINK)

#define STATIC_KSNODE_BDA_VIDEO_ENCODER \
    0xd98429e3, 0x65c9, 0x4ac4, 0x93, 0xaa, 0x76, 0x67, 0x82, 0x83, 0x3b, 0x7a 
DEFINE_GUIDSTRUCT("d98429e3-65c9-4ac4-93aa-766782833b7a", KSNODE_BDA_VIDEO_ENCODER);
#define KSNODE_BDA_VIDEO_ENCODER DEFINE_GUIDNAMED(KSNODE_BDA_VIDEO_ENCODER)

//===========================================================================
//
// IPSink PINNAME GUID
//
//===========================================================================

#define STATIC_PINNAME_IPSINK_INPUT \
    0x3fdffa70L, 0xac9a, 0x11d2, 0x8f, 0x17, 0x00, 0xc0, 0x4f, 0x79, 0x71, 0xe2
DEFINE_GUIDSTRUCT("3fdffa70-ac9a-11d2-8f17-00c04f7971e2", PINNAME_IPSINK_INPUT);
#define PINNAME_IPSINK_INPUT   DEFINE_GUIDNAMED(PINNAME_IPSINK_INPUT)


//===========================================================================
//
// BDA IPSink Categories/Types
//
//===========================================================================


#define STATIC_KSDATAFORMAT_TYPE_BDA_IP\
    0xe25f7b8e, 0xcccc, 0x11d2, 0x8f, 0x25, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe2
DEFINE_GUIDSTRUCT("e25f7b8e-cccc-11d2-8f25-00c04f7971e2", KSDATAFORMAT_TYPE_BDA_IP);
#define KSDATAFORMAT_TYPE_BDA_IP  DEFINE_GUIDNAMED(KSDATAFORMAT_TYPE_BDA_IP)

#define STATIC_KSDATAFORMAT_SUBTYPE_BDA_IP\
    0x5a9a213c, 0xdb08, 0x11d2, 0x8f, 0x32, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe2
DEFINE_GUIDSTRUCT("5a9a213c-db08-11d2-8f32-00c04f7971e2", KSDATAFORMAT_SUBTYPE_BDA_IP);
#define KSDATAFORMAT_SUBTYPE_BDA_IP  DEFINE_GUIDNAMED(KSDATAFORMAT_SUBTYPE_BDA_IP)

#define STATIC_KSDATAFORMAT_SPECIFIER_BDA_IP\
    0x6b891420, 0xdb09, 0x11d2, 0x8f, 0x32, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe2
DEFINE_GUIDSTRUCT("6B891420-DB09-11d2-8F32-00C04F7971E2", KSDATAFORMAT_SPECIFIER_BDA_IP);
#define KSDATAFORMAT_SPECIFIER_BDA_IP  DEFINE_GUIDNAMED(KSDATAFORMAT_SPECIFIER_BDA_IP)



#define STATIC_KSDATAFORMAT_TYPE_BDA_IP_CONTROL\
    0xdadd5799, 0x7d5b, 0x4b63, 0x80, 0xfb, 0xd1, 0x44, 0x2f, 0x26, 0xb6, 0x21
DEFINE_GUIDSTRUCT("DADD5799-7D5B-4b63-80FB-D1442F26B621", KSDATAFORMAT_TYPE_BDA_IP_CONTROL);
#define KSDATAFORMAT_TYPE_BDA_IP_CONTROL  DEFINE_GUIDNAMED(KSDATAFORMAT_TYPE_BDA_IP_CONTROL)

#define STATIC_KSDATAFORMAT_SUBTYPE_BDA_IP_CONTROL\
    0x499856e8, 0xe85b, 0x48ed, 0x9b, 0xea, 0x41, 0xd, 0xd, 0xd4, 0xef, 0x81
DEFINE_GUIDSTRUCT("499856E8-E85B-48ed-9BEA-410D0DD4EF81", KSDATAFORMAT_SUBTYPE_BDA_IP_CONTROL);
#define KSDATAFORMAT_SUBTYPE_BDA_IP_CONTROL  DEFINE_GUIDNAMED(KSDATAFORMAT_SUBTYPE_BDA_IP_CONTROL)


//===========================================================================
//
// MPE PINNAME GUID
//
//===========================================================================

#define STATIC_PINNAME_MPE \
    0xc1b06d73L, 0x1dbb, 0x11d3, 0x8f, 0x46, 0x00, 0xC0, 0x4f, 0x79, 0x71, 0xE2
DEFINE_GUIDSTRUCT("C1B06D73-1DBB-11d3-8F46-00C04F7971E2", PINNAME_MPE);
#define PINNAME_MPE   DEFINE_GUIDNAMED(PINNAME_MPE)


/////////////////////////////////////////////////////////////
//
// BDA MPE Categories/Types
//
#define STATIC_KSDATAFORMAT_TYPE_MPE \
    0x455f176c, 0x4b06, 0x47ce, 0x9a, 0xef, 0x8c, 0xae, 0xf7, 0x3d, 0xf7, 0xb5
DEFINE_GUIDSTRUCT("455F176C-4B06-47ce-9AEF-8CAEF73DF7B5", KSDATAFORMAT_TYPE_MPE);
#define KSDATAFORMAT_TYPE_MPE  DEFINE_GUIDNAMED(KSDATAFORMAT_TYPE_MPE)


//===========================================================================
//
// BDA NETWORK TYPE GUID
//
//===========================================================================

#define STATIC_DIGITAL_CABLE_NETWORK_TYPE \
    0x143827AB, 0xF77B, 0x498d, 0x81, 0xCA, 0x5A, 0x00, 0x7A, 0xEC, 0x28, 0xBF
DEFINE_GUIDSTRUCT("143827AB-F77B-498d-81CA-5A007AEC28BF", DIGITAL_CABLE_NETWORK_TYPE);
#define DIGITAL_CABLE_NETWORK_TYPE   DEFINE_GUIDNAMED(DIGITAL_CABLE_NETWORK_TYPE)

#define STATIC_ANALOG_TV_NETWORK_TYPE \
    0xb820d87e, 0xe0e3, 0x478f, 0x8a, 0x38, 0x4e, 0x13, 0xf7, 0xb3, 0xdf, 0x42
DEFINE_GUIDSTRUCT("B820D87E-E0E3-478f-8A38-4E13F7B3DF42", ANALOG_TV_NETWORK_TYPE);
#define ANALOG_TV_NETWORK_TYPE   DEFINE_GUIDNAMED(ANALOG_TV_NETWORK_TYPE)

#define STATIC_ANALOG_AUXIN_NETWORK_TYPE \
    0x742EF867, 0x9E1, 0x40A3, 0x82, 0xD3, 0x96, 0x69, 0xBA, 0x35, 0x32, 0x5F
DEFINE_GUIDSTRUCT("742EF867-09E1-40A3-82D3-9669BA35325F", ANALOG_AUXIN_NETWORK_TYPE);
#define ANALOG_AUXIN_NETWORK_TYPE   DEFINE_GUIDNAMED(ANALOG_AUXIN_NETWORK_TYPE)

#define STATIC_ANALOG_FM_NETWORK_TYPE \
    0x7728087b, 0x2bb9, 0x4e30, 0x80, 0x78, 0x44, 0x94, 0x76, 0xe5, 0x9d, 0xbb
DEFINE_GUIDSTRUCT("7728087B-2BB9-4E30-8078-449476E59DBB", ANALOG_FM_NETWORK_TYPE);
#define ANALOG_FM_NETWORK_TYPE   DEFINE_GUIDNAMED(ANALOG_FM_NETWORK_TYPE)

#define STATIC_ISDB_TERRESTRIAL_TV_NETWORK_TYPE \
    0x95037f6f, 0x3ac7, 0x4452, 0xb6, 0xc4, 0x45, 0xa9, 0xce, 0x92, 0x92, 0xa2
DEFINE_GUIDSTRUCT("95037F6F-3AC7-4452-B6C4-45A9CE9292A2", ISDB_TERRESTRIAL_TV_NETWORK_TYPE); 
#define ISDB_TERRESTRIAL_TV_NETWORK_TYPE DEFINE_GUIDNAMED(ISDB_TERRESTRIAL_TV_NETWORK_TYPE) 

#define STATIC_ISDB_SATELLITE_TV_NETWORK_TYPE \
    0xb0a4e6a0, 0x6a1a, 0x4b83, 0xbb, 0x5b, 0x90, 0x3e, 0x1d, 0x90, 0xe6, 0xb6 
DEFINE_GUIDSTRUCT("B0A4E6A0-6A1A-4B83-BB5B-903E1D90E6B6", ISDB_SATELLITE_TV_NETWORK_TYPE); 
#define ISDB_SATELLITE_TV_NETWORK_TYPE DEFINE_GUIDNAMED(ISDB_SATELLITE_TV_NETWORK_TYPE)

#define STATIC_ISDB_CABLE_TV_NETWORK_TYPE \
    0xc974ddb5, 0x41fe, 0x4b25, 0x97, 0x41, 0x92, 0xf0, 0x49, 0xf1, 0xd5, 0xd1 
DEFINE_GUIDSTRUCT("C974DDB5-41FE-4B25-9741-92F049F1D5D1", ISDB_CABLE_TV_NETWORK_TYPE); 
#define ISDB_CABLE_TV_NETWORK_TYPE DEFINE_GUIDNAMED(ISDB_CABLE_TV_NETWORK_TYPE)

#define STATIC_DIRECT_TV_SATELLITE_TV_NETWORK_TYPE \
    0x93b66fb5, 0x93d4, 0x4323, 0x92, 0x1c, 0xc1, 0xf5, 0x2d, 0xf6, 0x1d, 0x3f 
DEFINE_GUIDSTRUCT("93B66FB5-93D4-4323-921C-C1F52DF61D3F", DIRECT_TV_SATELLITE_TV_NETWORK_TYPE); 
#define DIRECT_TV_SATELLITE_TV_NETWORK_TYPE DEFINE_GUIDNAMED(DIRECT_TV_SATELLITE_TV_NETWORK_TYPE)

// Same as CLSID_ATSCNetworkProvider in uuids.h
#define STATIC_ATSC_TERRESTRIAL_TV_NETWORK_TYPE \
    0x0dad2fdd, 0x5fd7, 0x11d3, 0x8f, 0x50, 0x00, 0xc0, 0x4f, 0x79, 0x71, 0xe2
DEFINE_GUIDSTRUCT("0DAD2FDD-5FD7-11D3-8F50-00C04F7971E2", ATSC_TERRESTRIAL_TV_NETWORK_TYPE); 
#define ATSC_TERRESTRIAL_TV_NETWORK_TYPE DEFINE_GUIDNAMED(ATSC_TERRESTRIAL_TV_NETWORK_TYPE) 

// Same as CLSID_DVBTNetworkProvider in uuids.h
#define STATIC_DVB_TERRESTRIAL_TV_NETWORK_TYPE \
    0x216c62df, 0x6d7f, 0x4e9a, 0x85, 0x71, 0x05, 0xf1, 0x4e, 0xdb, 0x76, 0x6a
DEFINE_GUIDSTRUCT("216C62DF-6D7F-4E9A-8571-05F14EDB766A", DVB_TERRESTRIAL_TV_NETWORK_TYPE); 
#define DVB_TERRESTRIAL_TV_NETWORK_TYPE DEFINE_GUIDNAMED(DVB_TERRESTRIAL_TV_NETWORK_TYPE) 

// Same as CLSID_DVBSNetworkProvider in uuids.h
#define STATIC_DVB_SATELLITE_TV_NETWORK_TYPE \
    0xfa4b375a, 0x45b4, 0x4d45, 0x84, 0x40, 0x26, 0x39, 0x57, 0xb1, 0x16, 0x23 
DEFINE_GUIDSTRUCT("FA4B375A-45B4-4D45-8440-263957B11623", DVB_SATELLITE_TV_NETWORK_TYPE); 
#define DVB_SATELLITE_TV_NETWORK_TYPE DEFINE_GUIDNAMED(DVB_SATELLITE_TV_NETWORK_TYPE) 

// Same as CLSID_DVBCNetworkProvider in uuids.h
#define STATIC_DVB_CABLE_TV_NETWORK_TYPE \
    0xdc0c0fe7, 0x485, 0x4266, 0xb9, 0x3f, 0x68, 0xfb, 0xf8, 0x0e, 0xd8, 0x34 
DEFINE_GUIDSTRUCT("DC0C0FE7-0485-4266-B93F-68FBF80ED834", DVB_CABLE_TV_NETWORK_TYPE); 
#define DVB_CABLE_TV_NETWORK_TYPE DEFINE_GUIDNAMED(DVB_CABLE_TV_NETWORK_TYPE) 


#if defined(__cplusplus)
}
#endif // defined(__cplusplus)

#endif // !defined(_BDAMEDIA_)


