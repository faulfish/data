/*****************************************************************************/
/** @file:		MessageRequest.cpp
This is the file that contains all the functions related to the message
classes.
$Author:   Alan Tu  $
$Version:		1.0 $
$Date:   Mar 21 2008 16:23:08  $
 */

/*****************************************************************************/

#include "stdafx.h"
#include "MessageRequest.h"
#include "ProtocolManager.h"

//////////////////////////////////////////////////////////////////////////
// get service center address											//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
construction
@param		None
@return		None
@author		Eddy Guo
@date       2007/01/30
 */
CRequestGetSCA::CRequestGetSCA() {
    m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_bResult = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
execute the request
@param		None
@return		HRESULT - S_OK if successful; E_FAIL if failed
@author		Eddy Guo
@date       2007/01/30
 */
HRESULT CRequestGetSCA::ExecuteRequest() {
    HRESULT hr = S_OK;
    ::ResetEvent(m_hDataArrivalEvent);
    m_spATCodec = CAtCodec::CreateReqGetServiceCenterAddress();
    return ::ExecuteRequestT(m_spATCodec, this, m_hDataArrivalEvent);
}

/*---------------------------------------------------------------------------*/
/*!
the callback function when data arrived from the handset
@param		CBufferPtr spBuffer - the smart pointer points to the buffer 
that contains the data from the handset
@return		None
@author		Eddy Guo
@date       2007/01/30
 */
void CRequestGetSCA::OnDataArrival(CBufferPtr spBuffer) {
    CAtCodecPtr spAtCodec = new CAtCodec();
    CAtCodec::Opcode_t enuActiveOpcode = CAtCodec::otReqAt;
    if(m_spATCodec) {
        enuActiveOpcode = m_spATCodec->GetActiveOpcode();
    }
    m_ptBufferReceiver->Append(spBuffer);
    while(spAtCodec->IsCompleteResult(enuActiveOpcode, m_ptBufferReceiver)) {
        if(spAtCodec->GetOpcode() == CAtCodec::otResult) {
            m_bResult = TRUE;
            spAtCodec->DecodeResult();
        }
        m_ptBufferReceiver->PopFront(spAtCodec->GetPackageSize());
    }
    if(m_bResult) {
        m_strSCA = spAtCodec->GetSCA();
        SetEvent(m_hDataArrivalEvent);
    } else{}
}

//////////////////////////////////////////////////////////////////////////
// set PDU mode															//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
construction
@param		None
@return		None
@author		Eddy Guo
@date       2007/01/30
 */
CRequestSetPDUMode::CRequestSetPDUMode(): m_nMode(0) {
    m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_bResult = FALSE;
}
/*!
construction
@param		nMode, indicate what mode it is
@return		None
@author		Eddy Guo
@date       2007/01/30
 */
CRequestSetPDUMode::CRequestSetPDUMode(UINT nMode): m_nMode(nMode) {
    CRequestSetPDUMode();
}

/*---------------------------------------------------------------------------*/
/*!
execute the request
@param		None
@return		HRESULT - S_OK if successful; E_FAIL if failed
@author		Eddy Guo
@date       2007/01/30
 */
HRESULT CRequestSetPDUMode::ExecuteRequest() {
    HRESULT hr = S_OK;
    ::ResetEvent(m_hDataArrivalEvent);
    m_spATCodec = CAtCodec::CreateReqSetPDUMode(m_nMode);
    return ::ExecuteRequestT(m_spATCodec, this, m_hDataArrivalEvent);
}

/*---------------------------------------------------------------------------*/
/*!
the callback function when data arrived from the handset
@param		CBufferPtr spBuffer - the smart pointer points to the buffer 
that contains the data from the handset
@return		None
@author		Eddy Guo
@date       2007/01/30
 */
void CRequestSetPDUMode::OnDataArrival(CBufferPtr spBuffer) {
    CAtCodecPtr spAtCodec = new CAtCodec();
    CAtCodec::Opcode_t enuActiveOpcode = CAtCodec::otReqAt;
    if(m_spATCodec) {
        enuActiveOpcode = m_spATCodec->GetActiveOpcode();
    }
    m_ptBufferReceiver->Append(spBuffer);
    while(spAtCodec->IsCompleteResult(enuActiveOpcode, m_ptBufferReceiver)) {
        if(spAtCodec->GetOpcode() == CAtCodec::otResult) {
            m_bResult = TRUE;
            spAtCodec->DecodeResult();
        }
        m_ptBufferReceiver->PopFront(spAtCodec->GetPackageSize());
    }
    if(m_bResult) {
        SetEvent(m_hDataArrivalEvent);
    } else{}
}

//////////////////////////////////////////////////////////////////////////
// set message storage													//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
construction
@param		enumStorage eMsgStorage - the message storage to be set
@return		None
@author		Eddy Guo
@date       2007/01/30
 */
CRequestSetMessageStorage::CRequestSetMessageStorage(enumStorage eMsgStorage, MemUsed& used): m_eMsgStorage(eMsgStorage), m_used(&used) {
    m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_bResult = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
execute the request
@param		None
@return		HRESULT - S_OK if successful; E_FAIL if failed
@author		Eddy Guo
@date       2007/01/30
 */
HRESULT CRequestSetMessageStorage::ExecuteRequest() {
    HRESULT hr = S_OK;
    ::ResetEvent(m_hDataArrivalEvent);
    m_spATCodec = CAtCodec::CreateReqPreferredMessageStorage(m_eMsgStorage);
    return ::ExecuteRequestT(m_spATCodec, this, m_hDataArrivalEvent);
}

/*---------------------------------------------------------------------------*/
/*!
the callback function when data arrived from the handset
@param		CBufferPtr spBuffer - the smart pointer points to the buffer 
that contains the data from the handset
@return		None
@author		Eddy Guo
@date       2007/01/30
 */
void CRequestSetMessageStorage::OnDataArrival(CBufferPtr spBuffer) {
    CAtCodecPtr spAtCodec = new CAtCodec();
    CAtCodec::Opcode_t enuActiveOpcode = CAtCodec::otReqAt;
    if(m_spATCodec) {
        enuActiveOpcode = m_spATCodec->GetActiveOpcode();
    }
    m_ptBufferReceiver->Append(spBuffer);
    while(spAtCodec->IsCompleteResult(enuActiveOpcode, m_ptBufferReceiver)) {
        if(spAtCodec->GetOpcode() == CAtCodec::otResult) {
            m_bResult = TRUE;
            spAtCodec->DecodeResult();
        }
        m_ptBufferReceiver->PopFront(spAtCodec->GetPackageSize());
    }
    if(m_bResult) {
        spAtCodec->GetSetMessageStorageResult(*m_used);
        SetEvent(m_hDataArrivalEvent);
    } else{}
}

//////////////////////////////////////////////////////////////////////////
// query message space													//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
construction
@param		enumStorage eMsgStorage - the message storage to be query
@return		None
@author		Eddy Guo
@date       2007/01/30
 */
CRequestQueryMessageSpace::CRequestQueryMessageSpace(enumStorage eMsgStorage): m_eMsgStorage(eMsgStorage) {
    m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_bResult = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
execute the request
@param		None
@return		HRESULT - S_OK if successful; E_FAIL if failed
@author		Eddy Guo
@date       2007/01/30
 */
HRESULT CRequestQueryMessageSpace::ExecuteRequest() {
    HRESULT hr = S_OK;
    ::ResetEvent(m_hDataArrivalEvent);
    m_spATCodec = CAtCodec::CreateReqQueryMessageSpace(m_eMsgStorage);
    return ::ExecuteRequestT(m_spATCodec, this, m_hDataArrivalEvent);
}

/*---------------------------------------------------------------------------*/
/*!
the callback function when data arrived from the handset
@param		CBufferPtr spBuffer - the smart pointer points to the buffer 
that contains the data from the handset
@return		None
@author		Eddy Guo
@date       2007/01/30
 */
void CRequestQueryMessageSpace::OnDataArrival(CBufferPtr spBuffer) {
    CAtCodecPtr spAtCodec = new CAtCodec();
    CAtCodec::Opcode_t enuActiveOpcode = CAtCodec::otReqAt;
    if(m_spATCodec) {
        enuActiveOpcode = m_spATCodec->GetActiveOpcode();
    }
    m_ptBufferReceiver->Append(spBuffer);
    while(spAtCodec->IsCompleteResult(enuActiveOpcode, m_ptBufferReceiver)) {
        if(spAtCodec->GetOpcode() == CAtCodec::otResult) {
            m_bResult = TRUE;
            spAtCodec->DecodeResult();
        }
        m_ptBufferReceiver->PopFront(spAtCodec->GetPackageSize());
    }
    if(m_bResult) {
        m_dwUsed = spAtCodec->GetUsedTi();
        m_dwTotal = spAtCodec->GetTotalTi();
        SetEvent(m_hDataArrivalEvent);
    } else{}
}

//////////////////////////////////////////////////////////////////////////
// List Message															//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
// Do Data Arrival will return the decoded string
const bool CRequestListMessageTi::DoDataArrival(CBufferPtr &spBuffer) {
    CAtCodecPtr spAtCodec = new CAtCodec();
    CAtCodec::Opcode_t enuActiveOpcode = CAtCodec::otReqAt;
    if(m_spATCodec) {
        enuActiveOpcode = m_spATCodec->GetActiveOpcode();
    }
    while(spAtCodec->IsCompleteResult(enuActiveOpcode, m_ptBufferReceiver)) {
        if(spAtCodec->GetOpcode() == CAtCodec::otResult) {
            m_bResult = TRUE;
            spAtCodec->DecodeResult();
        }
        m_ptBufferReceiver->PopFront(spAtCodec->GetPackageSize());
    }
    if(m_bResult) {
        *m_pstrMsgList = spAtCodec->GetMessageIDList();
    }
    return m_bResult ? true : false;
}

// This function is just list all the message id, no meter it return all message content.
// Notice that: This command will return the status of this message.
// See AT+CMGL=<stat>
const bool CRequestListMessageStd::DoDataArrival(CBufferPtr &spBuffer) {
    CAtCodecPtr spAtCodec = new CAtCodec();
    CAtCodec::Opcode_t enuActiveOpcode = CAtCodec::otReqAt;
    if(m_spATCodec) {
        enuActiveOpcode = m_spATCodec->GetActiveOpcode();
    }
    while(spAtCodec->IsCompleteResult(enuActiveOpcode, m_ptBufferReceiver)) {
        if(spAtCodec->GetOpcode() == CAtCodec::otResult) {
            m_bResult = TRUE;
            spAtCodec->DecodeResult();
        }
        m_ptBufferReceiver->PopFront(spAtCodec->GetPackageSize());
    }
    if(m_bResult) {
        // 這裡的m_pstrMsgList是SMS Server傳進來要讀出去的資料
        *m_pstrMsgList = spAtCodec->GetMessageIDList();
        // 這裡的m_pVecSM是SMS Server傳進來要讀出去的, 和上面相同
        // 雖然是member data但是是指向SMS Server的資料
        spAtCodec->GetMessageList(*this->m_pVecSM);
        spAtCodec->GetShortMessage(*this->m_pVecShortMessage);
    }
    return m_bResult ? true : false;
}

//
//  List Message ID NEW
//

const bool CRequestListMessageNew::DoDataArrival(CBufferPtr &spBuffer) {
    CAtCodecPtr spAtCodec = new CAtCodec();
    CAtCodec::Opcode_t enuActiveOpcode = CAtCodec::otReqAt;
    if(m_spATCodec) {
        enuActiveOpcode = m_spATCodec->GetActiveOpcode();
    }
    while(spAtCodec->IsCompleteResult(enuActiveOpcode, m_ptBufferReceiver)) {
        if(spAtCodec->GetOpcode() == CAtCodec::otResult) {
            m_bResult = TRUE;
            spAtCodec->DecodeResult();
        }
        m_ptBufferReceiver->PopFront(spAtCodec->GetPackageSize());
    }
    if(m_bResult) {
        *m_pstrMsgIDList = spAtCodec->GetMessageIDList();
        spAtCodec->GetShortMessage(*m_pVecShortMessage);
    }
    return m_bResult ? true : false;
}


//////////////////////////////////////////////////////////////////////////
// send message															//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
construction
@param		BYTE* pMsgData - the pointer points to the data buffer
@param		DWORD dwMsgDataSize - size of the buffer
@return		None
@author		Eddy Guo
@date       2007/01/30
 */
CRequestSendMessage::CRequestSendMessage(BYTE *pMsgData, DWORD dwMsgDataSize,int& nErrorCode)
:m_pMsgData(pMsgData)
,m_dwMsgDataSize(dwMsgDataSize) 
,m_nErrorCode(nErrorCode)
{
    m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_bResult = FALSE;
    m_bIsFirstSegmentWritten = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
execute the request
@param		None
@return		HRESULT - S_OK if successful; E_FAIL if failed
@author		Eddy Guo
@date       2007/01/30
 */
HRESULT CRequestSendMessage::ExecuteRequest() {
    HRESULT hr = S_OK;
    ::ResetEvent(m_hDataArrivalEvent);
    m_spATCodec = CAtCodec::CreateReqDirectSendMessage(m_pMsgData, m_dwMsgDataSize);

    g_ProtocolManager.ExecuteProtocolRequest(m_spATCodec->GetPackageBuffer(), this);
    hr = WaitForSingleObject(m_hDataArrivalEvent, 3000);
    if(hr == WAIT_OBJECT_0) {
        if(!GetResult()) {
            hr = E_FAIL;
        } else {
            ::ResetEvent(m_hDataArrivalEvent);
            m_bResult = FALSE;
            m_spATCodec = CAtCodec::CreateReqMessagePDUTransferDirectSendMessage(m_pMsgData, m_dwMsgDataSize);
            hr = ::ExecuteRequestT(m_spATCodec, this, m_hDataArrivalEvent);
        }
    }
    return hr;
}

/*---------------------------------------------------------------------------*/
/*!
the callback function when data arrived from the handset
@param		CBufferPtr spBuffer - the smart pointer points to the buffer 
that contains the data from the handset
@return		None
@author		Eddy Guo
@date       2007/01/30
 */
void CRequestSendMessage::OnDataArrival(CBufferPtr spBuffer) {
    CAtCodecPtr spAtCodec = new CAtCodec();
    CAtCodec::Opcode_t enuActiveOpcode = CAtCodec::otReqAt;
    if(m_spATCodec) {
        enuActiveOpcode = m_spATCodec->GetActiveOpcode();
    }
    m_ptBufferReceiver->Append(spBuffer);
    while(spAtCodec->IsCompleteResult(enuActiveOpcode, m_ptBufferReceiver)) {
        if((spAtCodec->GetOpcode() == CAtCodec::otPDUPrompt) || (spAtCodec->GetOpcode() == CAtCodec::otResult)) {
            m_bResult = TRUE;
            spAtCodec->DecodeResult();
        } else  if(spAtCodec->GetOpcode() == CAtCodec::otCMSError) {
            m_bResult = FALSE;
            break;
        }
        m_ptBufferReceiver->PopFront(spAtCodec->GetPackageSize());
    }
    if(m_bResult) {
        if(m_bIsFirstSegmentWritten) {
            m_dwMsgID = spAtCodec->GetMsgID();
        }
        SetEvent(m_hDataArrivalEvent);
        m_bIsFirstSegmentWritten = TRUE;
        m_eReqState = STATE_COMPLETED_SUCCEEDED;
    } else {
		m_nErrorCode = 1;
        m_eReqState = STATE_COMPLETED_FAILED;
    }
}

//////////////////////////////////////////////////////////////////////////
// send message in storage												//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
construction
@param		DWORD dwMsgID - the ID of the message to be sent
@param		CString strAddress - destination addresss
@return		None
@author		Eddy Guo
@date       2007/01/30
 */
CRequestSendMessageInStorage::CRequestSendMessageInStorage(DWORD dwMsgID, CString strAddress): m_dwMsgID(dwMsgID), m_strAddress(strAddress) {
    m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_bResult = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
execute the request
@param		None
@return		HRESULT - S_OK if successful; E_FAIL if failed
@author		Eddy Guo
@date       2007/01/30
 */
HRESULT CRequestSendMessageInStorage::ExecuteRequest() {
    HRESULT hr = S_OK;
    ::ResetEvent(m_hDataArrivalEvent);
    if(GetCurAtCmdSet() == AT_CMD_STANDARD) {
        m_spATCodec = CAtCodec::CreateReqSendStoredMessageTi(m_dwMsgID, m_strAddress);
    } else if(GetCurAtCmdSet() == AT_CMD_BENQ) {
        m_spATCodec = CAtCodec::CreateReqSendStoredMessageStd(m_dwMsgID, m_strAddress);
    } else {
        ASSERT(FALSE);
    }
    return ::ExecuteRequestT(m_spATCodec, this, m_hDataArrivalEvent);
}

/*---------------------------------------------------------------------------*/
/*!
the callback function when data arrived from the handset
@param		CBufferPtr spBuffer - the smart pointer points to the buffer 
that contains the data from the handset
@return		None
@author		Eddy Guo
@date       2007/01/30
 */
void CRequestSendMessageInStorage::OnDataArrival(CBufferPtr spBuffer) {
    CAtCodecPtr spAtCodec = new CAtCodec();
    CAtCodec::Opcode_t enuActiveOpcode = CAtCodec::otReqAt;
    if(m_spATCodec) {
        enuActiveOpcode = m_spATCodec->GetActiveOpcode();
    }
    m_ptBufferReceiver->Append(spBuffer);
    while(spAtCodec->IsCompleteResult(enuActiveOpcode, m_ptBufferReceiver)) {
        if(spAtCodec->GetOpcode() == CAtCodec::otResult) {
            m_bResult = TRUE;
            spAtCodec->DecodeResult();
        }
        m_ptBufferReceiver->PopFront(spAtCodec->GetPackageSize());
    }
    if(m_bResult) {
        SetEvent(m_hDataArrivalEvent);
    } else{}
}

//////////////////////////////////////////////////////////////////////////
// write message														//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
construction
@param		enumMsgLoc eMsgLoc - the message folder to be written into
@param		BYTE* pMsgData - the pointer points to the data buffer
@param		DWORD dwMsgDataSize - size of the buffer
@return		None
@author		Eddy Guo
@date       2007/01/30
 */
CRequestWriteMessage::CRequestWriteMessage(enumMsgLoc eMsgLoc, BYTE *pMsgData, DWORD dwMsgDataSize, UINT* puiMsgID):
m_eMsgLoc(eMsgLoc), m_pMsgData(pMsgData), m_dwMsgDataSize(dwMsgDataSize), m_nRetID(puiMsgID) {
    m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_bResult = FALSE;
    m_bIsFirstSegmentWritten = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
execute the request
@param		None
@return		HRESULT - S_OK if successful; E_FAIL if failed
@author		Eddy Guo
@date       2007/01/30
 */
HRESULT CRequestWriteMessage::ExecuteRequest() {
    HRESULT hr = S_OK;
    ::ResetEvent(m_hDataArrivalEvent);
    if(GetCurAtCmdSet() == AT_CMD_STANDARD) {
        m_spATCodec = CAtCodec::CreateReqWriteMessageToMemoryStd((int)m_eMsgLoc, m_pMsgData, m_dwMsgDataSize);
    } else if(GetCurAtCmdSet() == AT_CMD_BENQ) {
        m_spATCodec = CAtCodec::CreateReqWriteMessageToMemoryTi(m_eMsgLoc, m_dwMsgDataSize);
    } else if(GetCurAtCmdSet() == AT_CMD_NEW_BENQ) {
        m_spATCodec = CAtCodec::CreateReqWriteMessagesNew(m_eMsgStorage, m_nFolderID, m_nTag, m_dwMsgDataSize);
    }
    g_ProtocolManager.ExecuteProtocolRequest(m_spATCodec->GetPackageBuffer(), this);
    hr = WaitForSingleObject(m_hDataArrivalEvent, 3000);
    if(hr == WAIT_OBJECT_0) {
        if(!GetResult()) {
            hr = E_FAIL;
        } else {
            ::ResetEvent(m_hDataArrivalEvent);
            m_bResult = FALSE;
            if(GetCurAtCmdSet() == AT_CMD_STANDARD) {
                m_spATCodec = CAtCodec::CreateReqMessagePDUTransferWriteMessageToMemoryStd(m_pMsgData, m_dwMsgDataSize);
            } else if(GetCurAtCmdSet() == AT_CMD_BENQ) {
                m_spATCodec = CAtCodec::CreateReqMessagePDUTransferWriteMessageToMemoryTi(m_pMsgData, m_dwMsgDataSize);
            } else if(GetCurAtCmdSet() == AT_CMD_NEW_BENQ) {
                m_spATCodec = CAtCodec::CreateReqMessagePDUTransferWriteMessageToMemoryNew(m_pMsgData, m_dwMsgDataSize);
            }
            hr = ::ExecuteRequestT(m_spATCodec, this, m_hDataArrivalEvent);
        }
    }
    return hr;
}

/*---------------------------------------------------------------------------*/
/*!
the callback function when data arrived from the handset
@param		CBufferPtr spBuffer - the smart pointer points to the buffer 
that contains the data from the handset
@return		None
@author		Eddy Guo
@date       2007/01/30
 */
void CRequestWriteMessage::OnDataArrival(CBufferPtr spBuffer) {
    CAtCodecPtr spAtCodec = new CAtCodec();
    CAtCodec::Opcode_t enuActiveOpcode = CAtCodec::otReqAt;
    if(m_spATCodec) {
        enuActiveOpcode = m_spATCodec->GetActiveOpcode();
    }
    m_ptBufferReceiver->Append(spBuffer);
    while(spAtCodec->IsCompleteResult(enuActiveOpcode, m_ptBufferReceiver)) {
        if((spAtCodec->GetOpcode() == CAtCodec::otResult) || (spAtCodec->GetOpcode() == CAtCodec::otPDUPrompt)) {
            m_bResult = TRUE;
            spAtCodec->DecodeResult();
        }
        m_ptBufferReceiver->PopFront(spAtCodec->GetPackageSize());
    }
    if(m_bResult) {
        if(m_bIsFirstSegmentWritten) {
            m_dwMsgID = spAtCodec->GetMsgID();
            *m_nRetID = (UINT)m_dwMsgID;
            if(GetCurAtCmdSet() == AT_CMD_NEW_BENQ) {
                CString strTime = spAtCodec->GetTime();
                sprintf_s(m_szTime, 20, "%S", (LPSTR)(LPCTSTR)strTime);
            }
        }
        SetEvent(m_hDataArrivalEvent);
        m_bIsFirstSegmentWritten = TRUE;
    } else{}
}


//////////////////////////////////////////////////////////////////////////
// read message															//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
construction
@param		DWORD dwMsgID - the ID of the message
@return		None
@author		Eddy Guo
@date       2007/01/30
 */
CRequestReadMessage::CRequestReadMessage(DWORD dwMsgID, CShortMessage* pCSM): 
m_dwMsgID(dwMsgID), m_pCSM(pCSM)
{
    m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_bResult = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
execute the request
@param		None
@return		HRESULT - S_OK if successful; E_FAIL if failed
@author		Eddy Guo
@date       2007/01/30
 */
HRESULT CRequestReadMessage::ExecuteRequest() {
    HRESULT hr = S_OK;
    ::ResetEvent(m_hDataArrivalEvent);
    if(GetCurAtCmdSet() == AT_CMD_STANDARD) {
        m_spATCodec = CAtCodec::CreateReqReadMessageStd(m_dwMsgID);
    } else if(GetCurAtCmdSet() == AT_CMD_BENQ) {
        m_spATCodec = CAtCodec::CreateReqReadMessageTi(m_dwMsgID);
    } else if(GetCurAtCmdSet() == AT_CMD_NEW_BENQ) {
        m_spATCodec = CAtCodec::CreateReqReadMessageNew(m_dwMsgID);
    } else {
        ASSERT(FALSE);
    }
    return ::ExecuteRequestT(m_spATCodec, this, m_hDataArrivalEvent);
}

/*---------------------------------------------------------------------------*/
/*!
the callback function when data arrived from the handset
@param		CBufferPtr spBuffer - the smart pointer points to the buffer 
that contains the data from the handset
@return		None
@author		Eddy Guo
@date       2007/01/30
 */
void CRequestReadMessage::OnDataArrival(CBufferPtr spBuffer) {
    CAtCodecPtr spAtCodec = new CAtCodec();
    CAtCodec::Opcode_t enuActiveOpcode = CAtCodec::otReqAt;
    if(m_spATCodec) {
        enuActiveOpcode = m_spATCodec->GetActiveOpcode();
    }
    m_ptBufferReceiver->Append(spBuffer);
    while(spAtCodec->IsCompleteResult(enuActiveOpcode, m_ptBufferReceiver)) {
        if(spAtCodec->GetOpcode() == CAtCodec::otResult) {
            m_bResult = TRUE;
            spAtCodec->SetReadMessage(*m_pCSM);
            spAtCodec->DecodeResult();
            break;
        }
        m_ptBufferReceiver->PopFront(spAtCodec->GetPackageSize());
    }
    if(m_bResult) {
        m_dwMsgDataSize = spAtCodec->GetMsgSize();
        m_spMsgDataBuffer = spAtCodec->GetMsgBuffer();
        if(GetCurAtCmdSet() == AT_CMD_NEW_BENQ && m_pCSM != NULL)
            spAtCodec->GetReadMessage(*m_pCSM);
        SetEvent(m_hDataArrivalEvent);
    } else{}
}

//////////////////////////////////////////////////////////////////////////
// delete message														//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
construction
@param		DWORD dwMsgID - the ID of the message
@return		None
@author		Eddy Guo
@date       2007/01/30
 */
CRequestDeleteMessage::CRequestDeleteMessage(DWORD dwMsgID): m_dwMsgID(dwMsgID) {
    m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_bResult = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
execute the request
@param		None
@return		HRESULT - S_OK if successful; E_FAIL if failed
@author		Eddy Guo
@date       2007/01/30
 */
HRESULT CRequestDeleteMessage::ExecuteRequest() {
    HRESULT hr = S_OK;
    ::ResetEvent(m_hDataArrivalEvent);

    if(GetCurAtCmdSet() == AT_CMD_STANDARD) {
        m_spATCodec = CAtCodec::CreateReqDeleteMessageStd(m_dwMsgID);
    } else if(GetCurAtCmdSet() == AT_CMD_BENQ) {
        m_spATCodec = CAtCodec::CreateReqDeleteMessageTi(m_dwMsgID);
    } else if(GetCurAtCmdSet() == AT_CMD_NEW_BENQ) {
        // Here use the same as XTC delete message command
        // Or using CAtCodec::CreateReqDeleteMessageNew will be fine
        m_spATCodec = CAtCodec::CreateReqDeleteMessageTi(m_dwMsgID);
    }

    return ::ExecuteRequestT(m_spATCodec, this, m_hDataArrivalEvent);
}

/*---------------------------------------------------------------------------*/
/*!
the callback function when data arrived from the handset
@param		CBufferPtr spBuffer - the smart pointer points to the buffer 
that contains the data from the handset
@return		None
@author		Eddy Guo
@date       2007/01/30
 */
void CRequestDeleteMessage::OnDataArrival(CBufferPtr spBuffer) {
    CAtCodecPtr spAtCodec = new CAtCodec();
    CAtCodec::Opcode_t enuActiveOpcode = CAtCodec::otReqAt;
    if(m_spATCodec) {
        enuActiveOpcode = m_spATCodec->GetActiveOpcode();
    }
    m_ptBufferReceiver->Append(spBuffer);
    while(spAtCodec->IsCompleteResult(enuActiveOpcode, m_ptBufferReceiver)) {
        if(spAtCodec->GetOpcode() == CAtCodec::otResult) {
            m_bResult = TRUE;
            spAtCodec->DecodeResult();
        }
        m_ptBufferReceiver->PopFront(spAtCodec->GetPackageSize());
    }
    if(m_bResult) {
        SetEvent(m_hDataArrivalEvent);
    } else{}
}


//////////////////////////////////////////////////////////////////////////
// List Folder                                                          //
//////////////////////////////////////////////////////////////////////////

CRequestListFolder::CRequestListFolder(unsigned int nFolderID, std::vector<CMessageFolderInfo>* vecFolder):
m_nFolderID(nFolderID),
m_pVecFolder(vecFolder)
{
    m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_bResult = FALSE;
}

HRESULT CRequestListFolder::ExecuteRequest() {
    HRESULT hr = S_OK;
    ::ResetEvent(m_hDataArrivalEvent);

    if(GetCurAtCmdSet() == AT_CMD_STANDARD) {
        ASSERT(FALSE);
        return S_FALSE;
    } else if(GetCurAtCmdSet() == AT_CMD_BENQ) {
        ASSERT(FALSE);
        return S_FALSE;
    } else if(GetCurAtCmdSet() == AT_CMD_NEW_BENQ) {
        m_spATCodec = CAtCodec::CreateReqListFolder(m_nFolderID);
        //ASSERT(FALSE);
    }
    return ::ExecuteRequestT(m_spATCodec, this, m_hDataArrivalEvent);
}

void CRequestListFolder::OnDataArrival(CBufferPtr spBuffer) {
    CAtCodecPtr spAtCodec = new CAtCodec();
    CAtCodec::Opcode_t enuActiveOpcode = CAtCodec::otReqAt;
    if(m_spATCodec) {
        enuActiveOpcode = m_spATCodec->GetActiveOpcode();
    }
    m_ptBufferReceiver->Append(spBuffer);
    while(spAtCodec->IsCompleteResult(enuActiveOpcode, m_ptBufferReceiver)) {
        if(spAtCodec->GetOpcode() == CAtCodec::otResult) {
            m_bResult = TRUE;
            spAtCodec->DecodeResult();
        }
        m_ptBufferReceiver->PopFront(spAtCodec->GetPackageSize());
    }
    if(m_bResult) {
        spAtCodec->GetFolderList(*m_pVecFolder);
        SetEvent(m_hDataArrivalEvent);
    } else{}
}


CRequestUpdateMessageState::CRequestUpdateMessageState(unsigned int nIndex, unsigned int nState):
m_nIndex(nIndex),
m_nState(nState)
{
    m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_bResult = FALSE;
}

HRESULT CRequestUpdateMessageState::ExecuteRequest() {
    HRESULT hr = S_OK;
    ::ResetEvent(m_hDataArrivalEvent);

    //if(GetCurAtCmdSet() == AT_CMD_STANDARD) {
    //    ASSERT(FALSE);
    //    return S_FALSE;
    //} else if(GetCurAtCmdSet() == AT_CMD_BENQ) {
    //    ASSERT(FALSE);
    //    return S_FALSE;
    //} else if(GetCurAtCmdSet() == AT_CMD_NEW_BENQ) {
        m_spATCodec = CAtCodec::CreateReqUpdateMessageState(m_nIndex, m_nState);
        //ASSERT(FALSE);
    //}
    return ::ExecuteRequestT(m_spATCodec, this, m_hDataArrivalEvent);
}

void CRequestUpdateMessageState::OnDataArrival(CBufferPtr spBuffer) {
    CAtCodecPtr spAtCodec = new CAtCodec();
    CAtCodec::Opcode_t enuActiveOpcode = CAtCodec::otReqUpdateMessageState;
    if(m_spATCodec) {
        enuActiveOpcode = m_spATCodec->GetActiveOpcode();
    }
    m_ptBufferReceiver->Append(spBuffer);
    while(spAtCodec->IsCompleteResult(enuActiveOpcode, m_ptBufferReceiver)) {
        if(spAtCodec->GetOpcode() == CAtCodec::otResult) {
            m_bResult = TRUE;
            spAtCodec->DecodeResult();
        }
        m_ptBufferReceiver->PopFront(spAtCodec->GetPackageSize());
    }
    //if(m_bResult) {
        SetEvent(m_hDataArrivalEvent);
    //}
}