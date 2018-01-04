//-----------------------------------------------------------------------------
// This file is implementation of AtCodec about message interfaces.
// If AtCodec is add a new interface, please implement member function in this
// file.
// !!!DO NOT ADD MEMBER FUNCTIONS IN ATCODEC.CPP file!!!
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "AtCodec.h"
#include "MessageUtil.h"

static CMessageUtil g_util;

// Message
//////////////////////////////////////////////////////////////////////////
// set PDU mode															//
//////////////////////////////////////////////////////////////////////////
CAtCodecPtr CAtCodec::CreateReqSetPDUMode(int nMode)
{
    // AT+CMGF=0 is setting the mobile into PDU mode
    // AT+CMGF=1 is setting the mobile into TEXT mode
    char sz[30] = {0};
    sprintf(sz, "AT+CMGF=%d\r", nMode);
    return CreateAtCodec(otRequest, (DWORD)strlen(sz), sz);
}

//////////////////////////////////////////////////////////////////////////
// list message															//
//////////////////////////////////////////////////////////////////////////
CAtCodecPtr CAtCodec::CreateReqListMessagesTi(enumMsgLoc eLocation)
{
    char sz[30] = {0};
    sprintf(sz, "AT$BQMGL=%d\r", eLocation);
    return CreateAtCodec(otReqListMessagesTi, (DWORD)strlen(sz), sz);
}

void CAtCodec::DecodeListMessagesTi(char* pBuffer, DWORD dwBufferLength)
{
    //pBuffer = result without trailing <CR>, <LF> and without OK <CR>, <LF> (pure data)
    USES_CONVERSION;

    int nMessageCounter = 0;
    DWORD dwBufferLeft = dwBufferLength;
    char *pBufferTmp = new char [dwBufferLength + 1];
    CString strRes, strParam, strTmp;
    BOOL bIsOKFound = FALSE, bIsInitial = TRUE;

    ZeroMemory(pBufferTmp, dwBufferLength + 1);
    memcpy(pBufferTmp, pBuffer, dwBufferLength);

    strRes.Format(_T("%s"), A2T(pBufferTmp));

    delete [] pBufferTmp;

    while ((!bIsOKFound) && (strRes.GetLength() != 0))
    {
        if (strRes.Find(_T("$BQMGL:")) != -1)
        {
            strRes = strRes.Right(strRes.GetLength() - strRes.Find(_T("\r\n")));

            CutBeginningCrlf(strRes);
        }
        else
        {
            // find OK
            strParam = strRes.Left(strRes.Find(_T("\r\n")));

            if (strParam.Compare(_T("OK")) == 0)
            {	// OK found
                bIsOKFound = TRUE;

                m_strMessageIDList += "\r";

                return;
            }
            else
            {
                // Message ID
                if (!bIsInitial)
                    m_strMessageIDList += ",";
                else
                    bIsInitial = FALSE;

                m_strMessageIDList += strParam;

                strRes = strRes.Right(strRes.GetLength() - strRes.Find(_T("\r\n")));

                CutBeginningCrlf(strRes);
            }
        }
    }
}

CAtCodecPtr CAtCodec::CreateReqListMessagesStd(int nStat)
{
    char sz[30] = {0};

    sprintf(sz, "AT+CMGL=%d\r", nStat);

    return CreateAtCodec(otReqListMessagesStd, (DWORD)strlen(sz), sz);
}

// decode message std, this will return a vector of message and a list of message ids
void CAtCodec::DecodeListMessagesStd(char* pBuffer, DWORD dwBufferLength)
{
    if(pBuffer == NULL || dwBufferLength == 0) return;
    CStringA tokenString = pBuffer;
    while(tokenString.GetLength() != 0) {
        tokenString.TrimLeft("\r\n");
        tokenString.TrimLeft("+CMGL: ");
        char* delim = ",\r\n", * id = "", * loc = "", * straAlpha = "", * len = "", * pdu = "", *nextToken;
        // index
        SM sm;
        if ((id = strtok_s((LPSTR)(LPCSTR)tokenString, delim, &nextToken)) != NULL) {
            CString tmp;
            tmp.Format(_T("%S"), id);
            m_strMessageIDList = m_strMessageIDList + tmp + CString(_T(","));
            sm.nMessageID = atoi(id);
        } else {
            ASSERT(false);
        }
        // location
        if ((loc = strtok_s(NULL, delim, &nextToken)) != NULL) {
            sm.Location = mapIntToMsgLoc(atoi(loc));
        } else {
            ASSERT(false);
        }
        // straAlpha, 如果有必要讀取時, 要傳入參數指定要不要讀取
        bool isIncludingAlpha = false;
        if (isIncludingAlpha) {
            if ((straAlpha = strtok_s(NULL, delim, &nextToken))) {
                sm.straAlpha = straAlpha;
            }
        } else {
            straAlpha = "";
            //sm.straAlpha = "";
        }
        // length
        if ((len = strtok_s(NULL, delim, &nextToken)) != NULL) {
            sm.nLength = atoi(len);
        } else {
            ASSERT(false);
        }
        // Undecoded pdu
        if ((pdu = strtok_s(NULL, delim, &nextToken)) != NULL) {
            sm.straUnDecodedPDU = pdu;
        } else {
            ASSERT(false);
        }
        m_vecSM.push_back(sm);
        tokenString = nextToken;    // 記錄剩下的字串(多筆時)
    }
    g_util.DecodeSTDMessage(m_vecSM, m_vecShortMessage);
    m_strMessageIDList.TrimRight(_T(","));
}

//////////////////////////////////////////////////////////////////////////
// set message storage													//
//////////////////////////////////////////////////////////////////////////
CAtCodecPtr CAtCodec::CreateReqPreferredMessageStorage(enumStorage eMsgStorage)
{
    const char* sz;

    if (eMsgStorage == STORAGE_SIM)
        sz = "AT+CPMS=\"SM\",\"SM\",\"SM\"\r";
    else if (eMsgStorage == STORAGE_HANDSET)
        sz = "AT+CPMS=\"ME\",\"ME\",\"ME\"\r";

    return CreateAtCodec(otReqPreferredMessageStorage, (DWORD)strlen(sz), sz);
}

void CAtCodec::DecodePreferredStorage(char* pBuffer)
{
    //pBuffer = result without trailing <CR>, <LF> and without OK <CR>, <LF> (pure data)

    // Write Command "AT+CPMS=<mem1>,<mem2>,<mem3>"
    // Response: +CPMS: <used1>,<total1>,<used2>,<total2>,<used3>,<total3> 
    // OK/ERROR/+CMS ERROR

    sscanf(pBuffer, "+CPMS: %d,%d,%d,%d,%d,%d", &m_dwUsedMem1, &m_dwTotalMem1, &m_dwUsedMem2, 
        &m_dwTotalMem2, &m_dwUsedMem3, &m_dwTotalMem3) ;
}

//////////////////////////////////////////////////////////////////////////
// write message														//
//////////////////////////////////////////////////////////////////////////
CAtCodecPtr CAtCodec::CreateReqWriteMessageToMemoryTi(enumMsgLoc eMsgLoc, int iDataLen)
{
    char sz[30] = {0};
    const char* fmt = "AT$BQMGW=%d,%d\r";             

    sprintf(sz, fmt, eMsgLoc, iDataLen);

    return CreateAtCodec(otReqWriteMessageToMemoryTi, (DWORD)strlen(sz), sz);
}

CAtCodecPtr CAtCodec::CreateReqMessagePDUTransferWriteMessageToMemoryTi(BYTE* pMsgData, int nDataLen)
{
    return CreateAtCodec(otReqPDUDataWriteTi, (DWORD)nDataLen, (char*)pMsgData);
}

void CAtCodec::DecodeWriteMessageToMemoryTi(char* pBuffer)
{
    //pBuffer = result without trailing <CR>, <LF> and without OK <CR>, <LF> (pure data)

    // Write Command "AT+CMGW=<length><CR>PDU<ctrl-Z/ESC>"
    // Response: +CMGW: <index> / CMS+ ERROR

    sscanf(pBuffer, "$BQMGW:%d", &m_dwMessageIndex);    
}

CAtCodecPtr CAtCodec::CreateReqWriteMessageToMemoryStd(int nLoc, BYTE* pMsgData, int nDataLen)
{
    char szSCASize[3], sz[30];
    int nSCASize;
    const char* fmt = "AT+CMGW=%d,%d\r";

    ZeroMemory(szSCASize, 3);
    ZeroMemory(sz, 30);

    memcpy(szSCASize, pMsgData, 2);

    nSCASize = atoi(szSCASize);

    sprintf(sz, fmt, nDataLen / 2 - nSCASize/* SCA */ - 1/* SCA length */, nLoc);

    return CreateAtCodec(otReqWriteMessageToMemoryStd, (DWORD)strlen(sz), sz);
}

CAtCodecPtr CAtCodec::CreateReqMessagePDUTransferWriteMessageToMemoryStd(BYTE* pMsgData, int nDataLen)
{
    BYTE* pReqBuf = pReqBuf = new BYTE [nDataLen + 1];
    memcpy(pReqBuf, pMsgData, nDataLen + 1); 
    pReqBuf[nDataLen] = 0x1A;   
    CAtCodecPtr spAtCodec = CreateAtCodec(otReqPDUDataWriteStd, (DWORD)nDataLen+1, (char*)pReqBuf);
    delete pReqBuf;
    return spAtCodec;
}

void CAtCodec::DecodeWriteMessageToMemoryStd(char* pBuffer)
{
    //pBuffer = result without trailing <CR>, <LF> and without OK <CR>, <LF> (pure data)

    // Write Command "AT+CMGW=<length><CR>PDU<ctrl-Z/ESC>"
    // Response: +CMGW: <index> / CMS+ ERROR

    sscanf(pBuffer, "+CMGW:%d", &m_dwMessageIndex);    
}

//////////////////////////////////////////////////////////////////////////
// send message															//
//////////////////////////////////////////////////////////////////////////
CAtCodecPtr CAtCodec::CreateReqDirectSendMessage(BYTE* pMsgData, int nDataLen)
{
    char szSCASize[3], sz[30] = {0};
    int nSCASize;
    const char* fmt = "AT+CMGS=%d\r";

    ZeroMemory(szSCASize, sizeof(szSCASize));
    memcpy(szSCASize, pMsgData, 2);

    nSCASize = atoi(szSCASize);

    sprintf(sz, fmt, nDataLen / 2 - nSCASize/* SCA */ - 1/* SCA length */);

    return CreateAtCodec(otReqDirectSendMessage, (DWORD)strlen(sz), sz);
}

CAtCodecPtr CAtCodec::CreateReqMessagePDUTransferDirectSendMessage(BYTE* pMsgData, int nDataLen)
{    
    BYTE* pReqBuf = 0;                

    pReqBuf = new BYTE [nDataLen + 1];

    memcpy(pReqBuf, pMsgData, nDataLen + 1); 
    pReqBuf[nDataLen] = 0x1A;             

    CAtCodecPtr spAtCodec = CreateAtCodec(otReqPDUDataDirectSend, (DWORD)nDataLen + 1, (char*)pReqBuf);
    delete [] pReqBuf;

    return spAtCodec;
}

void CAtCodec::DecodeDirectSendMessage(char* pBuffer)          
{
    // pBuffer = result without trailing <CR>, <LF> and without OK <CR>, <LF> (pure data)

    // Write Command "AT+CMGS=<mr><CR>PDU<ctrl-Z/ESC>"
    // Response: +CMGS: <mr> / CMS+ ERROR

    sscanf(pBuffer, "+CMGS:%d", &m_dwMessageIndex);
}

//////////////////////////////////////////////////////////////////////////
// send message in storage												//
//////////////////////////////////////////////////////////////////////////
CAtCodecPtr CAtCodec::CreateReqSendStoredMessageTi(DWORD dwIndex, LPCTSTR pstrAddress)
{
    USES_CONVERSION;

    char sz[30] = {0};

    if (pstrAddress != _T(""))
        sprintf(sz, "AT$BQMSS=%d,\"%s\"\r", dwIndex, T2CA(pstrAddress));
    else
        sprintf(sz, "AT$BQMSS=%d\r", dwIndex);

    return CreateAtCodec(otReqSendStoredMessageTi, (DWORD)strlen(sz), sz);
}

void CAtCodec::DecodeSendStoredMessageTi(char* pBuffer)
{
    // pBuffer = result without trailing <CR>, <LF> and without OK <CR>, <LF> (pure data)

    // Write Command "AT+CMSS=<mr><CR>PDU<ctrl-Z/ESC>"
    // Response: +CMGS: <mr> / CMS+ ERROR

    sscanf(pBuffer, "$BQMSS:%d", &m_dwMessageReference);
}

CAtCodecPtr CAtCodec::CreateReqSendStoredMessageStd(DWORD dwIndex, LPCTSTR pstrAddress)
{
    USES_CONVERSION;

    char sz[30] = {0};

    if (pstrAddress != _T(""))
        sprintf(sz, "AT+CMSS=%d,\"%s\"\r", dwIndex, T2CA(pstrAddress));
    else
        sprintf(sz, "AT+CMSS=%d\r", dwIndex);

    return CreateAtCodec(otReqSendStoredMessageStd, (DWORD)strlen(sz), sz);
}

void CAtCodec::DecodeSendStoredMessageStd(char* pBuffer)
{
    // pBuffer = result without trailing <CR>, <LF> and without OK <CR>, <LF> (pure data)

    // Write Command "AT+CMSS=<mr><CR>PDU<ctrl-Z/ESC>"
    // Response: +CMGS: <mr> / CMS+ ERROR

    sscanf(pBuffer, "+CMSS:%d", &m_dwMessageReference);
}

//////////////////////////////////////////////////////////////////////////
// delete message														//
//////////////////////////////////////////////////////////////////////////
CAtCodecPtr CAtCodec::CreateReqDeleteMessageTi(DWORD dwIndex)
{
    char sz[30];
    const char* fmt = "AT$BQMGD=%u\r";

    sprintf(sz, fmt, dwIndex);

    return CreateAtCodec(otRequest, (DWORD)strlen(sz), sz);
}

CAtCodecPtr CAtCodec::CreateReqDeleteMessageStd(DWORD dwIndex)
{
    char sz[30];
    const char* fmt = "AT+CMGD=%u\r";

    sprintf(sz, fmt, dwIndex);

    return CreateAtCodec(otRequest, (DWORD)strlen(sz), sz);
}

//////////////////////////////////////////////////////////////////////////
// read message															//
//////////////////////////////////////////////////////////////////////////
CAtCodecPtr CAtCodec::CreateReqReadMessageTi(DWORD dwIndex)
{
    char sz[30];
    const char* fmt = "AT$BQMGR=%u\r";

    sprintf(sz, fmt, dwIndex);

    return CreateAtCodec(otReqReadMessageTi, (DWORD)strlen(sz), sz);
}


void CAtCodec::DecodeReadMessageTi(char* pBuffer, DWORD dwBufferLength)
{
    static const char* szResultCode = "$BQMGR:";
    static const char* szCrLf="\r\n";

    char* pPtr = pBuffer;              
    HRESULT hRes = S_OK;
    int nPduLen=0;

    //extract parameter list
    char* pParamData=0;    
    DWORD dwParamLength=0;
    DWORD dwIndex = 0;      
    char szGrid1[10] = "xxxxxx";
    char szGrid2[10] = "xx";        

    DWORD dwStartPos = 0;
    DWORD dwEndPos = 0;

    dwIndex = 0;
    hRes = NextToken(szResultCode, szGrid1, szCrLf, szGrid2, pPtr, dwBufferLength, &dwStartPos, &dwEndPos);                    

    pPtr += dwStartPos;
    sscanf(pPtr, "%d,%d", &dwIndex, &nPduLen);
    pPtr += (dwEndPos-dwStartPos+2);

    m_dwMsgSize = nPduLen;
    m_spMsgBuffer = new CBuffer((DWORD)nPduLen, pPtr);
}

CAtCodecPtr CAtCodec::CreateReqReadMessageStd(DWORD dwIndex)
{
    char sz[30];
    const char* fmt = "AT+CMGR=%u\r";

    sprintf(sz, fmt, dwIndex);

    return CreateAtCodec(otReqReadMessageStd, (DWORD)strlen(sz), sz);
}


void CAtCodec::DecodeReadMessageStd(char* pBuffer, DWORD dwBufferLength)
{
    static const char* szResultCode = "+CMGR:";
    static const char* szCrLf="\r\n";

    char* pPtr = pBuffer;
    DWORD dwRestLength=dwBufferLength;               
    HRESULT hRes = S_OK;

    //extract parameter list
    char* pParamData=0;    
    DWORD dwParamLength=0;
    {        
        char szGrid1[10] = "xxxxxx";
        char szGrid2[10] = "xx";        

        DWORD dwStartPos = 0;
        DWORD dwEndPos = 0;
        hRes = NextToken(szResultCode, szGrid1, szCrLf, szGrid2, pPtr, dwRestLength, &dwStartPos, &dwEndPos);                    

        int nAlpha=0;
        pPtr += dwStartPos;
        int nPduLen=0;
        sscanf(pPtr, "%d,,%d", &m_dwStatus, &nPduLen);
        pPtr += (dwEndPos-dwStartPos);
        dwRestLength -= dwEndPos;
    }    

    //extract pdu data
    char* pRawData=0;    
    if(SUCCEEDED(hRes))
    {        
        char szGrid1[10] = "xx";
        char szGrid2[10] = "xx";        

        DWORD dwStartPos = 0;
        DWORD dwEndPos = 0;
        hRes = NextToken(szCrLf, szGrid1, szCrLf, szGrid2, pPtr, dwRestLength, &dwStartPos, &dwEndPos); 
        dwRestLength -= dwEndPos;        

        if(SUCCEEDED(hRes))
        {
            pRawData = pPtr;
            pRawData += dwStartPos;
            m_dwMsgSize = dwEndPos - dwStartPos;
            m_spMsgBuffer = new CBuffer((DWORD)m_dwMsgSize, pRawData);
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// get service center address											//
//////////////////////////////////////////////////////////////////////////
CAtCodecPtr CAtCodec::CreateReqGetServiceCenterAddress()
{
    const char* sz = "AT+CSCA?\r";

    return CreateAtCodec(otReqGetServiceCenterAddress, (DWORD)strlen(sz), sz);
}


void CAtCodec::DecodeServiceCenterAddress(char* pBuffer, int& nResult)
{
    //pBuffer = result without trailing <CR>, <LF> and without OK <CR>, <LF> (pure data)

    // Read Command "AT+CSCA?\r";
    // Response +CSCA: <sca>,<tosca>

    unsigned char szPath[1024];
    DWORD dwSCA;

    nResult = sscanf( pBuffer, "+CSCA: \"%[^\"]\",%d", szPath, &dwSCA );
    if (nResult == 2)
    {
        m_strSCA = szPath; 
        m_strToSCA.Format(_T("%d"), dwSCA);				
    }
}

void CAtCodec::DecodeGetServiceCenterAddress(char* pBuffer)
{
    //pBuffer = result without trailing <CR>, <LF> and without OK <CR>, <LF> (pure data)

    // Read Command "AT+CSCA?\r";
    // Response +CSCA: <sca>,<tosca>

    m_strSCA	= GetToken(pBuffer, '\"', '\"');
    m_strToSCA	= GetToken(pBuffer, ',', '\x0D');
}

//////////////////////////////////////////////////////////////////////////
// query message space													//
//////////////////////////////////////////////////////////////////////////
CAtCodecPtr CAtCodec::CreateReqQueryMessageSpace(enumStorage eMsgStorage)
{
    const char* sz;

    if (eMsgStorage == STORAGE_SIM)
        sz = "AT$MGQS=\"SM\"\r";
    else if (eMsgStorage == STORAGE_HANDSET)
        sz = "AT$MGQS=\"ME\"\r";

    return CreateAtCodec(otReqQueryMessageSpace, (DWORD)strlen(sz), sz);
}

void CAtCodec::DecodeQueryMessageSpace(char* pBuffer)
{
    sscanf(pBuffer, "$MGQS:%d,%d", &m_dwUsedTi, &m_dwTotalTi) ;       
}

//////////////////////////////////////////////////////////////////////////
// check password														//
//////////////////////////////////////////////////////////////////////////
CAtCodecPtr CAtCodec::CreateReqCheckPassword(LPCTSTR Password)
{
    USES_CONVERSION;

    char sz[30];

    sprintf(sz, "AT$CHKPL=1,\"%s\"\r", T2CA(Password));

    return CreateAtCodec(otRequest, (DWORD)strlen(sz), sz);
}

bool CAtCodec::ParsePDUPrompt(bool bSuccess)
{
    //			Traceit( TRACEIT_INFO, TG_CAtCodec, _T("%s: PDUPrompt"), pcf);
    bSuccess = true;
    return bSuccess;
}


CAtCodecPtr CAtCodec::CreateReqListFolder(UINT nFolderID)
{
    USES_CONVERSION;

    char sz[30] = {0};;

    sprintf(sz, "AT$BQMGFL=%d\r", nFolderID);

    return CreateAtCodec(otReqQueryFolderNew, (DWORD)strlen(sz), sz);
}

// Decode the return value of AT$BQMGFL.
// Author: Eddy Guo
// Date: 2007.10.24
// Last update: 2007.10.25

void CAtCodec::DecodeListFolder(char* pBuffer)
{
    if(pBuffer == NULL) return;
    char* delim = "\r\n:";
    char* nextToken;
    char* szNumOfFolder = strtok_s(pBuffer, "\r\n", &nextToken);
    CStringA tokenString = nextToken;

    while(tokenString.GetLength() != 0) {
        CMessageFolderInfo folderInfo;
        char* id, *name, *editable, *writable;
        if((id = strtok_s((LPSTR)(LPCSTR)tokenString, delim, &nextToken)) != NULL) {
            folderInfo.nID = atoi(id);
        }
        if((name = strtok_s(NULL, delim, &nextToken)) != NULL) {
            folderInfo.strDisplayName.Format(_T("%S"), name);
        }
        if((editable = strtok_s(NULL, delim, &nextToken)) != NULL) {
            folderInfo.bEditable = atoi(editable) > 0 ? true : false;
        }
        if((writable = strtok_s(NULL, delim, &nextToken)) != NULL) {
            folderInfo.bHasSubFolder = atoi(writable) > 0 ? true : false;
        }
        m_vecFolder.push_back(folderInfo);
        tokenString = nextToken;
    }
}

CAtCodecPtr CAtCodec::CreateReqReadMessageNew(DWORD dwIndex)
{
    USES_CONVERSION;

    char sz[30] = {0};;

    sprintf(sz, "AT$BQMGR=%ld\r", dwIndex);

    return CreateAtCodec(otReqReadMsgNew, (DWORD)strlen(sz), sz);
}

void CAtCodec::DecodeReadMessageNew(char* pBuffer, DWORD dwBufferLength) {
    if(pBuffer == NULL) return;
    m_dwMsgSize = dwBufferLength;
    m_spMsgBuffer = new CBuffer((DWORD)m_dwMsgSize, pBuffer);
    
    char* delim = "\r\n:";
    char* nextToken;
    char* phone, *time;
    int year = 0, month = 0, day = 0, hour = 0, minute = 0;
    CStringA strBuffer = pBuffer;
    phone = strtok_s((LPSTR)(LPCSTR)strBuffer, "\r\n", &nextToken);    
    if((time = strtok_s(NULL, delim, &nextToken)) != NULL) {
        sscanf(time, "%d-%d-%d-%d-%d", &year, &month, &day, &hour, &minute);
    }
    CStringA strTmp = nextToken;
    strTmp.TrimLeft("\r\n");
    char szDecoded[1024] = {0};
    int nLength = 1024;
    QPDecode((BYTE*)(LPSTR)(LPCSTR)strTmp, strTmp.GetLength(), szDecoded, &nLength);
    DWORD dwNum = MultiByteToWideChar(CP_UTF8, NULL, szDecoded, -1, NULL, 0);
    wchar_t *psText = new wchar_t[dwNum];
    MultiByteToWideChar(CP_UTF8, 0, szDecoded, -1, psText, dwNum);
    m_shortMessage.m_strMessage = psText;
    delete [] psText;
    m_shortMessage.m_strDateTime.Format(_T("%.4d/%.2d/%.2d %.2d:%.2d"), year, month, day, hour, minute);
    m_shortMessage.m_strPhoneNumber = phone;
}

CAtCodecPtr CAtCodec::CreateReqListMessagesNew(DWORD nFolderID) {
    USES_CONVERSION;

    char sz[30] = {0};;

    sprintf(sz, "AT$BQMGL=%ld\r", nFolderID);

    return CreateAtCodec(otReqListMsgIDNew, (DWORD)strlen(sz), sz);
}

void CAtCodec::DecodeListMessagesNew(char* pBuffer, DWORD dwBufferLength) {
    if(pBuffer == NULL) return;
    m_dwMsgSize = dwBufferLength;
    m_spMsgBuffer = new CBuffer((DWORD)m_dwMsgSize, pBuffer);
    
    CStringA strBuffer = pBuffer;
    char* delim = "\r\n:", * nextToken;
    char* total = strtok_s((LPSTR)(LPCSTR)strBuffer, "\r\n", &nextToken);
    int nCount = atoi(total);
    for(int i = 0; i < nCount; ++i) {
        CShortMessage csm;
        char* nID = strtok_s(NULL, delim, &nextToken);
        csm.m_vecMessageID.push_back((UINT)_atoi64(nID));
        char* isRead = strtok_s(NULL, delim, &nextToken);
        csm.m_bIsRead = (*isRead != '0')? true : false;
        char* storage = strtok_s(NULL, delim, &nextToken);
        csm.m_storage = (strcmp(storage, "ME") == 0)? STORAGE_HANDSET : STORAGE_SIM;
        this->m_vecShortMessage.push_back(csm);
    }
}
// The same as XTC delete message
CAtCodecPtr CAtCodec::CreateReqDeleteMessagesNew(DWORD nMsgID) {

    return CreateReqDeleteMessageTi(nMsgID);
}

// Create new Write command
// indicate storage (SIM or ME), Folder id, Tag, and Recipient
// parameter 1: storage
// parameter 2: folder id
// parameter 3: tag, indicate the message is read or not
// parameter 4: message length for input first
// If this command is complete, mobile will return OK. Then input the message with UTF-8 + QP-Encode
CAtCodecPtr CAtCodec::CreateReqWriteMessagesNew(enumStorage eMsgStorage, UINT FolderID, UINT nTag, UINT nMessageLength) {
    USES_CONVERSION;

    char sz[100] = {0};
    // this command is AT$BQCMGW=<Storage>,<FolderID>,<Read/Unread Tag>,<Length>
    // Not implemented.
    sprintf(sz, "AT$BQMGW=%s,%ld,%ld,%ld\r", (eMsgStorage == STORAGE_SIM ? "\"SM\"" : "\"ME\""), FolderID, nTag, nMessageLength);

    return CreateAtCodec(otReqWriteMessageToMemoryNew, (DWORD)strlen(sz), sz);

}

// Data write into memory.
// See the definition of AT$BQCMGW=<Storage>,<FolderID>,<Read/Unread Tag>,<Length>
// The previos command is OK then write message into mobile.
CAtCodecPtr CAtCodec::CreateReqMessagePDUTransferWriteMessageToMemoryNew(BYTE* pMsgData, int nDataLen) {

    return CreateAtCodec(otReqPDUDataWriteNew, (DWORD)nDataLen, (char*)pMsgData);
}

void CAtCodec::DecodeWriteMessageNewResult(char* pBuffer) {
    char* delim = ":";
    char* nextToken = NULL;
    char* szID = strtok_s(pBuffer, delim, &nextToken);
    int year = 0, month = 0, day = 0, hour = 0, minute = 0;
    char* szTime = strtok_s(NULL, delim, &nextToken);
    sscanf(szTime, "%d-%d-%d-%d-%d", &year, &month, &day, &hour, &minute);
    m_dwMessageIndex = atol(szID);
    m_strTime.Format(_T("%.4d/%.2d/%.2d %.2d:%.2d"), year, month, day, hour, minute);
}

CRefObjectPtr<CAtCodec> CAtCodec::CreateReqUpdateMessageState(DWORD dwIndex, UINT nState) {
    USES_CONVERSION;
    char sz[100] = {0};
    // this command is AT$BQMGU=<Index>,<State>
    // Not implemented.
    sprintf(sz, "AT$BQMGU=%ld,%ld\r", dwIndex, nState);

    return CreateAtCodec(otReqUpdateMessageState, (DWORD)strlen(sz), sz);
}

void CAtCodec::DecodeUpdateMessageState(char* pBuffer) {

}