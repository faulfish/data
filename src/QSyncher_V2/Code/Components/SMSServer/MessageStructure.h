/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   MessageStructure.h  $
 *  $Revision:   1.14  $
 *  $Date:   Nov 09 2007 12:59:08  $
 *
 ***************************************************************************
 *
 * File Description
 * ----------------
 *
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Components/SMSServer/MessageStructure.h.-arc  $
 * 
 *    Rev 1.14   Nov 09 2007 12:59:08   Eddy Guo
 * Add Header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file MessageStructure.h
    @brief {brief description of MessageStructure.h}

    {detail description of MessageStructure.h}
*/
#ifndef __SMSSERVER_MESSAGESTRUCTURE_H__
#define __SMSSERVER_MESSAGESTRUCTURE_H__
 
#include <vector>
using namespace std;
 
#ifdef SMSSERVER_EXPORTS
#define SMSSERVER_API __declspec(dllexport)
#else
#define SMSSERVER_API __declspec(dllimport)
#endif
 
#pragma once
//#include "..\..\CommLayer\MobileInterface\Interface_Include\isms.h"
// DCS
#define DCS_7BIT    0x00                ///< data coding scheme 7 bit
#define DCS_8BIT    0x01                ///< data coding scheme 8 bit (ASCII Code)
#define DCS_UCS2    0x02                ///< data coding scheme UCS2 (Unicode)
 
typedef enum enumStorageTag             ///< Storage Type, It can be SM or ME
{
    STORAGE_SIM,						///< message storage SIM
    STORAGE_HANDSET,					///< message storage handset
} enumStorage;
 
// standard at command folder definition
typedef enum enumMessageStat {          ///< Standard Message Stat
    RECEIVED_UNREAD =   0,              ///< Inbox Unread
    RECEIVED_READ =     1,              ///< Inbox Read
    STORED_UNSENT =     2,              ///< Draft
    STORED_SEND =       3,              ///< Sent or Outbox
    ALL =               4,              ///< List all messages
    UNKNOWN_STAT
} MESSAGE_STAT;
 
typedef enum enumMsgLocTag				///< message folder for TI phones
{
    INBOX,								///< message folder inbox
    DRAFT,								///< message folder draft
    OUTBOX,								///< message folder outbox
    SIM_INBOX,							///< message folder SIM inbox
    SIM_DRAFT,							///< message folder SIM draft
    UNSENT,								///< message folder unsent
    PRI_INBOX,							///< message folder private inbox
    PRI_OUTBOX,							///< message folder private outbox
    PRI_DRAFT,							///< message folder private draft
    PRI_SENT,							///< message folder private sent
    UNKNOWN_FOLDER,                     ///< message folder unknown (may use at initialization)
    ROOT_FOLDER,                        ///< message folder root
    MOBILE_FOLDER,                      ///< message folder mobile
    SIM_CARD_FOLDER                     ///< message folder sim card
} enumMsgLoc;
 
inline enumMsgLoc operator++(enumMsgLoc& rs, int) {
    return rs = (enumMsgLoc)(rs + 1);
}
 
inline enumMsgLoc operator++(enumMsgLoc& rs) {
    return rs = (enumMsgLoc)(rs + 1);
}
 
inline enumMsgLoc mapIntToMsgLoc(UINT loc) {
    return (enumMsgLoc)loc;
}
 
// ----------------------------------------------------------------------------
// TimeStamp is used for time stamp
// ----------------------------------------------------------------------------
class _TimeStamp {
public:
    _TimeStamp(): nYear(0), nMonth(0), nDay(0), nHour(0), nMinute(0),
        nSecond(0), nTimeZone(0)
    {
        TRACE("Time Stamp Constructor!!\n");
    }
    virtual ~_TimeStamp() {
        TRACE("Time Stamp Destructor!!\n");
    }
 
public:
    UINT         nYear;                  ///< year
    UINT         nMonth;                 ///< month
    UINT         nDay;                   ///< day
    UINT         nHour;                  ///< hour
    UINT         nMinute;                ///< minute
    UINT         nSecond;                ///< second
    UINT         nTimeZone;              ///< time zone
};
 
// ----------------------------------------------------------------------------
// PDU Content information
// ----------------------------------------------------------------------------
class _PDU {
public:
    _PDU(): nSMSCLength(0), nSMstraSCAddressType(0), straSCAddress(""),
        nFirstOctectOfMessage(0), nMessageReference(0), nSenderAddressLength(0),
        nSenderAddressType(0), straSenderAddress(""), nProtocolIdentifier(0),
        nDataCodingScheme(0), nValidityPeriod(0), nUserDataLength(0), szUserData(NULL),
        nUserDataHeaderLength(0), nReferenceNumber(0), nTotalMessageCount(0),
        nCurrentnMessageID(0)
    {
        TRACE("PDU Constructor!!\n");
    }
    virtual ~_PDU() {
        TRACE("PDU Destructor!!\n");
    }
public:
    UINT        nSMSCLength;            // Service Center Address長度
    BYTE        nSMstraSCAddressType;   // Service Center Address type, "+"
    CStringA    straSCAddress;          // Service Center Address
    BYTE        nFirstOctectOfMessage;  // 第一個byte
    BYTE        nMessageReference;      // Message Reference
    UINT        nSenderAddressLength;   // Sender Address長度
    BYTE        nSenderAddressType;     // Sender Address type, "+"
    CStringA    straSenderAddress;      // Sender Address
    BYTE        nProtocolIdentifier;    // Protocol ID
    BYTE        nDataCodingScheme;      // Data Coding Scheme
    _TimeStamp  TimeStamp;              // Time Stamp
    BYTE        nValidityPeriod;        // Validity Date
    UINT        nUserDataLength;        // User Data Length, 指編碼後的長度, 非真正的長度
    char*       szUserData;             // User Data
    UINT        nUserDataHeaderLength;  // User Data Header Length
    UINT        nReferenceNumber;       // Reference Number, 多page使用
    UINT        nTotalMessageCount;     // Total Message Count, 多page使用
    UINT        nCurrentnMessageID;     // Current Message ID, 多page使用
};
 
 
//-----------------------------------------------------------------------------
// Standard At Command 的 Short Message
//-----------------------------------------------------------------------------
class SM {
public:
    SM(): straUnDecodedPDU(""), nUnDecodedPDULength(0), Location(UNKNOWN_FOLDER),
        nMessageID(0), straAlpha(""), nLength(0)
    {
        TRACE("SM Constructor!!\n");
    }
    virtual ~SM() {
        TRACE("SM Destructor!!\n");
    }
public:
    CStringA    straUnDecodedPDU;       // 原始的SMS資訊也就是沒解開的PDU
    UINT        nUnDecodedPDULength;    // 原始的SMS資訊也就是沒解開的PDU的長度
    enumMsgLoc  Location;               // 簡訊所在的位置
    UINT        nMessageID;             // 簡訊的ID
    CStringA    straAlpha;              // 送件人, 通常為空白
    UINT        nLength;                // 一共編了多少字元, 不是pdu長度, 而是解碼後的長度
    _PDU        PDU;                    // PDU
};
 
 
//-----------------------------------------------------------------------------
// BenQ At Command 的 Short Message
// 命名為BQSM, 和原來的SM不同...
//-----------------------------------------------------------------------------
class ContentHdr {
public:
    ContentHdr(): Byte1(0), TON(0), AddrLen(0), DCS(0), ref(0), pid(0), idx(0)
    {
        max = 0;
        memset(Addr, 0, sizeof(char)*20);
        TRACE("ContentHdr Constructor!!\n");
    }
    virtual ~ContentHdr() {
        TRACE("ContentHdr Destructor!!\n");
    }
public:
    BYTE    Byte1;
    BYTE    TON;
    BYTE    AddrLen;
    char    Addr[20];
    BYTE    DCS;
    BYTE    ref;
    BYTE    pid;
    BYTE    max;
    BYTE    idx;
};
 
class MessageHdr {
public:
    MessageHdr(): uiMsgID(0), MsgType(0), Loc(0), bIsRead(0), bISCompleted(0), iNumPacket(0) {
        TRACE("MessageHdr Constructor!!\n");
    }
    virtual ~MessageHdr() {
        TRACE("MessageHdr Destructor!!\n");
    }
public:
    UINT        uiMsgID;
    BYTE        MsgType;
    _TimeStamp  SentTime;
    UINT        Loc;
    ContentHdr  ContentHdr;
    BYTE        bIsRead;
    BYTE        bISCompleted;
    BYTE        bIsDRM;
    UINT        iNumPacket;
};
//-----------------------------------------------------------------------------
// 原本的BenQ Message
//-----------------------------------------------------------------------------
class BQSM {
public:
    BQSM() : bIs16BitConcatenated(0), bIsEMS(0), iTotalMsgSize(0), iTotalMsgObjSize(0),
        MsgData(NULL), MsgObjData(NULL)
    {
        memset(&MsgHdr, 0, sizeof(MessageHdr));
        memset(MsgLen, 0, sizeof(UINT) * 9);
        memset(MsgObjLen, 0, sizeof(BYTE) * 9);
        memset(MsgObjCount, 0, sizeof(BYTE) * 9);
        TRACE("BQSM Constructor!!\n");
    }
    virtual ~BQSM() {
        TRACE("BQSM Destructor!!\n");
    }
public:
    MessageHdr  MsgHdr;
    BYTE        bIs16BitConcatenated;
    BYTE        bIsEMS;
    UINT        iTotalMsgSize;
    UINT        iTotalMsgObjSize;
    UINT        MsgLen[9];
    BYTE        MsgObjLen[9];
    BYTE        MsgObjCount[9];
    BYTE        *MsgData;
    BYTE        *MsgObjData;
};
 
typedef enum enumMessageType
{
    STD_MESSAGE,            // Standard Message Type
    BQ_MESSAGE,             // BenQ Message Type
    UNKNOWN_MESSAGE         // Unknown Message Type
} MsgType;
 
class CMessageObject
{
public:
    CMessageObject():m_MessageObject(NULL)
        , m_nMessageLength(0)
        , m_MessageType(STD_MESSAGE)
        , m_nMessageID(0)
        , m_bIsComplete(false)
        , m_bIsRead(false)
        , m_Folder(UNKNOWN_FOLDER)
        , m_MessageStat(UNKNOWN_STAT)
    {
        TRACE("Message Object Constructor...\n");
    }
public:
    virtual ~CMessageObject()
    {
        TRACE("Message Object Destructor...\n");
        delete m_MessageObject;
    }
public:
    void*           m_MessageObject;    // Message Object
    long            m_nMessageLength;   // Message Length
    MsgType         m_MessageType;      // Message Type
    UINT            m_nMessageID;       // Message ID
    bool            m_bIsComplete;      // 是否為完整的Message
    bool            m_bIsRead;          // 是否為讀取過的Message
    enumMsgLocTag   m_Folder;           // 在哪個folder
    MESSAGE_STAT    m_MessageStat;      // Message的狀態, 用於standard message
};
 
class CBQSM : public CMessageObject
{
public:
    CBQSM(void) {
        TRACE("BQSM Constructor...\n");
    }
 
    virtual ~CBQSM(void) {
        if(m_BQShortMessage.MsgObjData != NULL) {
            delete m_BQShortMessage.MsgObjData;
            m_BQShortMessage.MsgObjData = NULL;
        }
        if(m_BQShortMessage.MsgData != NULL) {
            delete m_BQShortMessage.MsgData;
            m_BQShortMessage.MsgData = NULL;
        }
        TRACE("BQSM Destructor...\n");
    }
public:
    BQSM m_BQShortMessage;
};
 
 
class CSM : public CMessageObject
{
public:
    CSM(void) {
        TRACE("SM Constructor...\n");
    }
 
    virtual ~CSM(void) {
        if(m_ShortMessage.PDU.szUserData != NULL) {
            delete[] m_ShortMessage.PDU.szUserData;
            m_ShortMessage.PDU.szUserData = NULL;
        }
        TRACE("SM Destructor...\n");
    }
public:
    SM m_ShortMessage;
};
 
 
class CDecodedMessage
{
 
public:
    CDecodedMessage(void):
    m_strSenderNumber(_T("")), m_nSenderNumberLength(0), m_strMessage(_T(""))
    , m_bIsConactenated(false), m_bIsComplete(false), m_MessageLocation(DRAFT)
    , m_nReferenceNumber(0), m_nCurrentNumber(0), m_nTotalMessageCount(0), m_nMessageLength(0)
    {
        memset(m_pMessageIDs, -1, sizeof(UINT) * 256);
        TRACE("Decoded Message Constructor...\n");
    }
 
    virtual ~CDecodedMessage(void) {
        TRACE("Decoded Message Destructor...\n");
    }
 
public:
    CString     m_strSenderNumber;      // 送件者
    UINT        m_nSenderNumberLength;  // 送件者長度
    _TimeStamp  m_TimeStamp;            // 時間
    CString     m_strMessage;           // 解碼後的message(Unicode)
    bool        m_bIsConactenated;      // 是否已結合對個page的message, 如果是的話就是一個完整的messaeg
    bool        m_bIsComplete;          // 解開完之後是不是一個完整的message(缺page就是不完整)
    UINT        m_pMessageIDs[256];     // Message id, 所有合成後的id
    enumMsgLoc  m_MessageLocation;      // Message所在的location
    UINT        m_nReferenceNumber;     // 指定一個唯一的數字分辨多page時用
    UINT        m_nCurrentNumber;       // 現在是第幾個page
    UINT        m_nTotalMessageCount;   // 一共是多個page
    UINT        m_nMessageLength;       // Message的長度, 可以用CString.GetLength()取得, 注意Unicode
};
 
/**
 * @brief  Class Short Message
 * @author Eddy Guo
 * This class is the final result of short message.
 * It contains message content, date time, phone number, ID and stat/Location
 * Notice that the encode and decode are done in CDCInterface.
 */
 
 
// This class is used for all type of messages.
// All necessary of Message View will store into here.
class CShortMessage {
public:
    CShortMessage() : m_nMessageLocation(0), m_bIsRead(false) {}
    virtual ~CShortMessage() {}
public:
    vector<UINT>    m_vecMessageID;         ///< Message ID
    bool            m_bIsRead;              ///< This message is read or not
    CString         m_strDateTime;          ///< Message Date time, decoded
    CString         m_strPhoneNumber;       ///< In inbox, this means sender. In outbox/sent, this means receiver
    UINT            m_nMessageLocation;     ///< Message Stat/Location
    CString         m_strMessage;           ///< Message Content, deocded, merged
    enumStorageTag  m_storage;              ///< Messages are int ME or SIM card
};
 
typedef struct FolderTag {
    UINT        nFolderID;
    CString     cstrFolderName;
    UINT        nTotalSubFolder;
    bool        bEditable;
    bool        bCanCreateSubfolder;
} MsgFolder;
 
typedef struct MemUsedStruct {
	DWORD m_dwUsedMem1;
    DWORD m_dwUsedMem2;
    DWORD m_dwUsedMem3;
    DWORD m_dwTotalMem1;
    DWORD m_dwTotalMem2;
    DWORD m_dwTotalMem3;
} MemUsed;
 
class CMessageFolderInfo                    ///< Folder Information
{
public:
    UINT    nID;                            ///< Folder ID
    CString strDisplayName;                 ///< Folder Name
    bool    bCanCreateSubfolder;            ///< This Folder Can Be Create Sub folder or not
    bool    bHasSubFolder;                  ///< This Folder Contains Sub folder or not
    bool    bIsProtected;                   ///< Modified to Protected or not
    bool    bEditable;                      ///< This Folder Can Be Edited or not
    vector<CMessageFolderInfo> childFolder; ///< Record the child folders
};
 
#endif // __SMSSERVER_MESSAGESTRUCTURE_H__
