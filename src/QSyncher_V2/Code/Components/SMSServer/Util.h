/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   Util.h  $
 *  $Revision:   1.10  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Components/SMSServer/Util.h.-arc  $
 * 
 *    Rev 1.10   Nov 09 2007 12:59:08   Eddy Guo
 * Add Header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file Util.h
    @brief {brief description of Util.h}

    {detail description of Util.h}
*/
#ifndef __SMSSERVER_UTIL_H__
#define __SMSSERVER_UTIL_H__
 
#ifdef SMSSERVER_EXPORTS
#define SMSSERVER_API __declspec(dllexport)
#else
#define SMSSERVER_API __declspec(dllimport)
#endif
 
#pragma once
#include "MessageStructure.h"
#include <vector>
 
using namespace std;
 
#define CHAR2BCD(x,y)			( ((x - '0') ) + ((y - '0') << 4))
#define CHAR2HEXBCD(x,y)		( ((x >= 'A') ? (x - 'A' + 10) : (x - '0')) + (((y >= 'A') ? (y - 'A' + 10) : (y - '0')) << 4) )
#define HEXCHAR2BYTE(x,y)		( (((x >= 'A') ? (x - 'A' + 10) : (x - '0')) << 4 ) + ((y >= 'A') ? (y - 'A' + 10) : (y - '0')) )
 
#define MAX_TIMEOUT
#define MAX_CON_7BIT_LEN    153 // �̪�7bit�i�H��153�Ӧr
#define MAX_CON_8BIT_LEN    134 // �̪�8bit�i�H��134�Ӧr
#define MAX_CON_UCS2_LEN    67  // �̪�UCS2�i�H��67�Ӧr
 
#define INBOX_FOLDER		0x2001
#define DRAFT_FOLDER		0x2002
#define SENT_FOLDER			0x2003
#define UNSENT_FOLDER		0x2004
#define PRI_INBOX_FOLDER	0x2005
#define PRI_DRAFT_FOLDER	0x2006
#define PRI_SENT_FOLDER		0x2007
#define PRI_UNSENT_FOLDER   0x2008
 
#define USER_DATA_HEADER_LEN	1
#define CON_16BIT_LEN			6
#define CON_8BIT_LEN			5
#define EXT_OBJ_HEADER_LEN		7
#define COMPRESSION_HEADER_LEN	3
// The one and only application object
#define COMPOSER_AT_WAIT_RECV_TIMEOUT		2000
#define COMPOSER_WAIT_RECV_TIMEOUT			3000
#define MAX_RECV_SIZE						SERIAL_QUEUE_SIZE
#define MAX_TPDU_SIZE						164
 
// IEI for EMS
#define EMS_TEXT_FORMAT				0x0A
#define EMS_PRE_SOUND				0x0B
#define EMS_USER_SOUND				0x0C
#define EMS_PRE_ANIMATION			0x0D
#define EMS_LARGE_ANIMATION			0x0E
#define EMS_SMALL_ANIMATION			0x0F
#define EMS_LARGE_PIC				0x10
#define EMS_SMALL_PIC				0x11
#define EMS_VAR_PIC					0x12
#define EMS_USER_PROMPT_IND			0x13
#define EMS_EXT_OBJ					0x14
#define EMS_REUSED_EXT_OBJ			0x15
#define EMS_COMPRESSION_CTRL		0x16
#define EMS_OBJ_DISTRIBUTION_IND	0x17
#define EMS_STD_WVG_OBJ				0x18
#define EMS_CHAR_SIZE_WVG_OBJ		0x19
#define EMS_EXT_OBJ_DATA_REQ_CMD	0x1A
 
// DCS
#define DCS_7BIT					0x00
#define DCS_8BIT					0x01
#define DCS_UCS2					0x02
 
// IEI defination
#define IEI_CON_8BIT				0x00
#define IEI_CON_16BIT				0x08
#define IEI_TEXT_FORMAT				0x0A
#define IEI_PRE_SOUND				0x0B
#define IEI_USER_SOUND				0x0C
#define IEI_PRE_ANIMATION			0x0D
#define IEI_LARGE_ANIMATION			0x0E
#define IEI_SMALL_ANIMATION			0x0F
#define IEI_LARGE_PIC				0x10
#define IEI_SMALL_PIC				0x11
#define IEI_VAR_PIC					0x12
#define IEI_EXT_OBJ					0x14
#define IEI_COMPRESSION_CTRL		0x16
 
// extended object type
#define EXT_OBJ_PRE_SOUND			0x00
#define EXT_OBJ_IMELODY				0x01
#define EXT_OBJ_BW_BITMAP			0x02
#define EXT_OBJ_2BIT_BITMAP			0x03
#define EXT_OBJ_6BIT_BITMAP			0x04
#define EXT_OBJ_PRE_ANIMATION		0x05
#define EXT_OBJ_BW_ANIMATION		0x06
#define EXT_OBJ_2BIT_ANIMATION		0x07
#define EXT_OBJ_6BIT_ANIMATION		0x08
#define EXT_OBJ_VCARD				0x09
#define EXT_OBJ_VCALENDAR			0x0A
 
 
// IEI for EMS
#define EMS_TEXT_FORMAT             0x0A
#define EMS_PRE_SOUND               0x0B
#define EMS_USER_SOUND              0x0C
#define EMS_PRE_ANIMATION           0x0D
#define EMS_LARGE_ANIMATION         0x0E
#define EMS_SMALL_ANIMATION         0x0F
#define EMS_LARGE_PIC               0x10
#define EMS_SMALL_PIC               0x11
#define EMS_VAR_PIC                 0x12
#define EMS_USER_PROMPT_IND         0x13
#define EMS_EXT_OBJ                 0x14
#define EMS_REUSED_EXT_OBJ          0x15
#define EMS_COMPRESSION_CTRL        0x16
#define EMS_OBJ_DISTRIBUTION_IND    0x17
#define EMS_STD_WVG_OBJ             0x18
#define EMS_CHAR_SIZE_WVG_OBJ       0x19
#define EMS_EXT_OBJ_DATA_REQ_CMD    0x1A
 
namespace SMSUtil {
 
class SMSSERVER_API CUtil
{
public:
    CUtil::CUtil(void) {
    }
 
    virtual CUtil::~CUtil(void) {
    }
public:
    // 16�i�쪺��Ʀr�ର16�i�쪺�Ʀr
    UINT GSMString2Bytes(const char* pSrc, BYTE* pDst, UINT nSrcLength);
    // 16�i�쪺�Ʀr�ର16�i�쪺��Ʀr
    UINT GSMBytes2String(const BYTE* pSrc, char* pDst, UINT nSrcLength);
 
    bool HEXToDigit(BYTE *pHexBuf, UINT uiHexBufLen, BYTE *pDigitBuf);
    UINT DigitToHEX(BYTE *pHexBuf, CStringA cstrDigit, char chDummyDigit);
    bool BCDToDigit(BYTE *pBcdBuf, UINT uiBcdBufLen, BYTE *pDigitBuf);
    UINT DigitToBCD(BYTE *pBcdBuf, CStringA cstrDigit, char chDummyDigit = '0');
    bool HEXBCDToDigit(BYTE *pHexBcdBuf, UINT uiHexBcdBufLen, BYTE *pDigitBuf);
    UINT DigitToHEXBCD(BYTE *pHexBcdBuf, CStringA cstrDigit, char chDummyDigit = '0');
 
    // encode 7bit�r��
    UINT Encode7Bit(char *inBuf, UINT inBufLen, char *outBuf);
 
    // decode 7bit�r��
    UINT Decode7Bit(char *inBuf, UINT inBufLen, char *outBuf, UINT outBufLen, BYTE nFillBit);
 
    // encode 7bit�@�ǯS��X
    bool Encode7BitCode(BYTE *cBuf, BYTE *ExtByte);
 
    // decode 7bit�@�ǯS��X
    bool Decode7BitCode(BYTE *cBuf, bool bIsExtension);
 
    // encode UCS2 (UnitCode)
    UINT EncodeUCS2(char *inBuf, UINT inBufLen, char *outBuf);
 
    // decode UCS2 (UnitCode)
    UINT DecodeUCS2(char *inBuf, UINT inBufLen, char *outBuf);
 
    // �P�_�O���OUCS2�s�X
    bool IsUCS2(CStringA InputString);
 
    // �P�_�O���OEMS
    BOOL IsEMS(char *inBuf, UINT iSize);
 
    // ���omessage���Y��object
    BYTE GetObjCount(char *inBuf, UINT iSize);
 
    // ���s�ˬdPacket Number, �]���bMessage���Y���h�Ӧa��X�{
    UINT ReCheckPacketNum(BQSM *pMsg);
 
    // decode 8bit�r��
    UINT Decode8BitFcn(char *inBuf, UINT inBufLen, char *outBuf);
 
    // �q16�i���Ʀr�ন10�i��Ʀr
    UINT hex2dec(char c);
 
    // ��pool���Y�������}�b�Upage��message�զX�_��, ��bDEcodedMessage
    bool ConactenateMessages(UINT IDs[], UINT IDCount,
        CList<CMessageObject*>* ShortMessages,
        CList<CDecodedMessage*>* DecodedMessages);
 
//-----------------------------------------------------------------------------
// �U���OStandard������
//-----------------------------------------------------------------------------
    // ����pdu���U�ӳ�, ���ѧ���N�Ǧ^�@��_PDU
    bool PDUParser(char* in, UINT length, _PDU& out);
 
    // ����PDU�����Y
    bool ParsrPDUHeader(char* CMGLRet, UINT length, SM* sm, bool isstraAlpha = false);
 
    // �ǤJ�r��, �r�����, ����H�q��, �ǥX�r��, �ǥX�r�����
    bool PDUComposer(char* msg, UINT msgLength, CStringA sender, char* out, UINT& outLength);
 
    // �إ�PDU�����Y
    BYTE ComposePDUHeader(char *outBuf, bool bStatusReq, bool bDataHeader,
        bool bReplyPath, BYTE AddressLen, bool bIsInter, char *cAddress,
        bool bIsUCS2, BYTE DataLen);
//-----------------------------------------------------------------------------
 
 
//-----------------------------------------------------------------------------
// �U���OBQ Message������
    // ����BQ message, ���ѧ���Ǧ^�@��BQSM
    UINT BQSMParser(char* in, UINT msgLength, BQSM& out);
 
    // �ǤJ�r��, �r�����, ����H�q��, �ǥX�r��, �ǥX�r�����
    bool BQSMComposer(char* msg, UINT msgLength, CStringA strReceiver,
        char* out, UINT& outLength, BYTE nLocation);
 
    // ����BenQ Message�����Y, ���ѧ���Ǧ^�@��BQSM, �]��BQ message�O���θѶ}��
    // ���Y��message�N�O�s�X�᪺, �u�n�ѽX�N�n
    UINT ParseBQSMHeader(char *inBuf, UINT inDataBufLen, BQSM *Msg, UINT uiMessageID);
//-----------------------------------------------------------------------------
 
 
    // �Ѷ}�Ҧ��bShoerMessages���Y���T��, ��KDecodedMessages�ϥ�, �C��private
    bool DecodeMessages(CList<CMessageObject*>* Messages, CList<CDecodedMessage*>* DecodedMessages);
 
    // ��ConactenateMessages, DecodeMessages��
    CDecodedMessage* GetDecodedMessageByCurrentNumber(CList<CDecodedMessage*>* DecodedMessages,
        UINT nReferenceNumber, UINT currentNumber, UINT nTotalMessageCount);
 
    // �ǤJ�r���, �̷өҦ�delimter�Ӥ��Φr��, �^�Ǥ@��vector���Y�񪺬OCString
    bool GetToken(TCHAR* str, UINT strLength, TCHAR* delim, UINT delLength, vector<CString> &outTokenVec);
 
    // �N��Ӭ۾F��byte�洫, �F��change endian���ĪG
    bool ChangeEndian(char* szDst, char* szSrc, UINT nSrcLength);
 
    // ���o�ثe���ɶ�, �Ǧ^�~��ɮɤ���
    bool GetCurrentDateTime(CStringA &strYear, CStringA &strMonth, CStringA &strDay,
        CStringA &strHour, CStringA &strMinute, CStringA &strSecond);
 
    void ParseObjectFromBQSM(BQSM* pBqsm, /*UINT& iTotalSize,*/
        UINT& iSizeTmp, UINT& iSizeTag, UINT& iSizeObjTag, UINT& x, UINT& iOffset,
        char* cSizeTmp, UINT& iMsgObjDataSize);
 
    void ExtractObjectFromBQSM(BQSM* pBqsm, UINT& iTotalSize, UINT& iMsgDataSize,
        UINT& iSizeTmp, UINT& iSizeTag, UINT& iSizeObjTag, UINT& iOffset,
        char* cSizeTmp, UINT& iMsgObjDataSize);
 
    void GetMessageObject16Bit(BQSM* pBqsm, UINT& iSizeTag, UINT& iSizeObjTag,
        UINT& iOffset, BYTE* m_MsgData, BYTE* m_MsgObjData, UINT& l);
 
    void GetMessageObject7Bit(BQSM* pBqsm, UINT iSizeTag, UINT iSizeObjTag,
        UINT iOffset, BYTE* m_MsgData, BYTE* m_MsgObjData, UINT& l);
 
private:
    void DecodedBQMessage(CBQSM* pMessage, CDecodedMessage& decodedMessage, int nOriginalLength);
    void DecodedSTDMessage(CSM* pMessage, CDecodedMessage& decodedMessage);
};
 
}
 
#endif // __SMSSERVER_UTIL_H__
