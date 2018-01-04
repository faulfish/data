/*****************************************************************************/
/** @file:		Message.cpp
  This is the file that contains the functions related to the Message class.
  This file includes functions that should be implemented in ISMS for Short
  Message.
  $Author:   Alan Tu  $
  $Version:     1.0 $
  $Date:   Mar 21 2008 16:23:08  $
  */
/*****************************************************************************/
#include "stdafx.h"
#include "Message.h"
#include "MessageUtil.h"

CMessage *g_pSingleton = NULL;

CDCINTERFACE_CLASS_DLLEXT ISMS* CreateISMS() {
	if(g_pSingleton == NULL) g_pSingleton = new CMessage();
	return g_pSingleton;
}

CDCINTERFACE_CLASS_DLLEXT void DeleteISMS() {
	if(g_pSingleton) {
		delete g_pSingleton;
		g_pSingleton = NULL;
	}
}

/*---------------------------------------------------------------------------*/
/*!
  construction

  @param		None
  @return		None
  @author		Eddy Guo
  @date			2007/02/08
  */
CMessage::CMessage():m_nTimeOut(60000), m_pConfigure(NULL), m_eCurAtCmdSet(AT_CMD_UNKNOWN) {
    if(QueryAtCommandSet() == AT_CMD_UNKNOWN) {
        ASSERT(false);
    }
}

/*---------------------------------------------------------------------------*/
/*!
  destruction

  @param		None
  @return		None
  @author		Eddy Guo
  @date			2007/02/08
  */
CMessage::~CMessage() {
}

/*---------------------------------------------------------------------------*/
/*!
  Query AT Command set
  indicate what this command is
  @param		none
  @return		enumAtCmdSet
  @author		Eddy Guo
  @date			2007/09/13
  */
enumAtCmdSet CMessage::QueryAtCommandSet() {
    if(m_eCurAtCmdSet == AT_CMD_UNKNOWN) {
        if(m_pConfigure == NULL)
            m_pConfigure = GetMobileConFig();
        CQSetting qSetting;
        TCHAR szMobileModel[512] = {0};
        TCHAR szIMEI[512] = {0};
        TCHAR szDisplayName[512] = {0};
        CommGetMobileInfo(_T("Message Request"), szMobileModel, szIMEI, szDisplayName);
        m_pConfigure->GetMobileSettings(szMobileModel, qSetting);
        TSTRING szDCS = qSetting.Get(_T("Data_Coding_Scheme"));
        if(szDCS == _T("STANARD_shcheme"))
            m_eCurAtCmdSet = AT_CMD_STANDARD;
        else if(szDCS == _T("BENQ_shcheme"))
            m_eCurAtCmdSet = AT_CMD_BENQ;
        else if(szDCS == _T("NEW_BENQ_shcheme"))
            m_eCurAtCmdSet = AT_CMD_NEW_BENQ;
    } else {
        ASSERT(false);
    }
    return m_eCurAtCmdSet;
}


/*---------------------------------------------------------------------------*/
/*!
  get service center address

  @param		CString* pcstrSCA - a referenced pointer to delever the service
				center address back
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Eddy Guo
  @date			2007/02/08
  */
HRESULT CMessage::GetServiceCenterAddress(CString* pcstrSCA)
{
    HRESULT hr = S_OK;

    CRequestGetSCAPtr spRequest = new CRequestGetSCA();

    spRequest->SetCurAtCmdSet(this->m_eCurAtCmdSet);

    hr = EnqueueRequest(hr, spRequest);

    if(hr == S_OK)
        *pcstrSCA = spRequest->GetSCA();

    return hr;
}



/*---------------------------------------------------------------------------*/
/*!
  set the mobile to the PDU mode

  @param		None
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Eddy Guo
  @date			2007/02/08
  */
HRESULT CMessage::SetPDUMode()
{
	HRESULT hr = S_OK;

    CRequestSetPDUModePtr spRequest = new CRequestSetPDUMode();

    spRequest->SetCurAtCmdSet(this->m_eCurAtCmdSet);

    hr = EnqueueRequest(hr, spRequest);

	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
  set the message storage

  @param		enumStorage eMsgStorage - the specified storage to be set
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Eddy Guo
  @date			2007/02/08
  */
HRESULT CMessage::SetMessageStorage(enumStorage eMsgStorage, MemUsed& used)
{
	HRESULT hr = S_OK;

    CRequestSetMessageStoragePtr spRequest = new CRequestSetMessageStorage(eMsgStorage, used);

    spRequest->SetCurAtCmdSet(this->m_eCurAtCmdSet);

    hr = EnqueueRequest(hr, spRequest);

	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
  query the message storage status

  @param		enumStorage eMsgStorage - the specified storage to be set
  @param		UINT* puiUsed - a referenced pointer to delever the number of
				existing messages
  @param		UINT* puiTotal - a referenced pointer to delever the number of
				total message space
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Eddy Guo
  @date			2007/02/08
  */
HRESULT CMessage::QueryMessageSpace(enumStorage eMsgStorage, UINT* puiUsed, UINT* puiTotal)
{
	HRESULT hr = S_OK;

    CRequestQueryMessageSpacePtr spRequest = new CRequestQueryMessageSpace(eMsgStorage);

    spRequest->SetCurAtCmdSet(this->m_eCurAtCmdSet);

    hr = EnqueueRequest(hr, spRequest);

    if (hr == S_OK) {
        *puiUsed = spRequest->GetUsed();
        *puiTotal = spRequest->GetTotal();
    }
	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
  list the message index in a specified folder

  Here, list message function have to know what type of command it should
  write into mobile and what codec (XTC, STD, NEW BENQ) it should use to
  deocde the output result. Originally CRequestListMessageTi has its own
  codec. May reference the operation of CRequestListMessageTi.

  @param		enumMsgLoc eMsgLoc - the specified folder
  @param		CString* pcstrMsgIDList - a referenced pointer to delever the
				string of message idcices
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Eddy Guo
  @date			2007/02/08
  */
HRESULT CMessage::ListMessage(enumMsgLoc eMsgLoc, CString* pcstrMsgIDList, std::vector<SM>* pVecSM, std::vector<CShortMessage>* pVecShortMessage)
{
	HRESULT hr = S_OK;

	try
    {
        CRequestPtr ptRequest;

        if(m_eCurAtCmdSet == AT_CMD_BENQ) {
            ptRequest = new CRequestListMessageTi(eMsgLoc, pcstrMsgIDList, AT_CMD_BENQ);
        } else if(m_eCurAtCmdSet == AT_CMD_STANDARD) {
            ptRequest = new CRequestListMessageStd((int)eMsgLoc, pcstrMsgIDList, pVecSM, AT_CMD_STANDARD, pVecShortMessage);
        } else if(m_eCurAtCmdSet == AT_CMD_NEW_BENQ) {
            ptRequest = new CRequestListMessageNew((UINT)eMsgLoc, pcstrMsgIDList, AT_CMD_NEW_BENQ, pVecShortMessage);
        }
		hr = Execute(ptRequest,m_nTimeOut * 3000) > 0 ? S_OK : E_FAIL;
        if(m_eCurAtCmdSet == AT_CMD_BENQ && pVecShortMessage->size() == 0 && pcstrMsgIDList->GetLength() != 0) {
            ReadEachMessageByID(pcstrMsgIDList, pVecShortMessage);
        }
	}
	catch(...)
	{
		hr = E_FAIL;
	}

	return hr;
}

void CMessage::ReadEachMessageByID(CString* pcstrMsgIDList, std::vector<CShortMessage>* pVecShortMessage)
{
    CMessageUtil util;
    std::vector<UINT> vecID;
    util.DecodeIDList(*pcstrMsgIDList, vecID);
    for(UINT i = 0; i < vecID.size(); ++i) {
        BYTE pData[1024];
        UINT nDataSize = 1024;
        this->ReadMessage(vecID.at(i), pData, &nDataSize);
        BQSM bqsm;
        BOOL bIsEMS = FALSE;
        UINT puiMsgDataSize = 0;
        BYTE pMsgData[1024] = {0};
        char cSizeTmp[4] = {0};
        BYTE* m_MsgData = NULL;;
        BYTE* m_MsgObjData = NULL;
        UINT iTotalSize = 0, iSizeTmp = 0, iSizeTag = 0, iSizeObjTag = 0;
        UINT nTotalSize = util.ParseBQSMHeader((char*)pData, nDataSize, &bqsm, vecID.at(i));
        UINT iMsgObjDataSize = util.GetBQSMContent(&bqsm, iTotalSize, iSizeTmp, iSizeTag, iSizeObjTag, cSizeTmp, m_MsgData, m_MsgObjData);
        
        char szDst[1024] = {0};
        //util.ChangeEndian(szDst, (char*)m_MsgData, iSizeTag);
        TCHAR* a = (TCHAR*)szDst;
        TCHAR* b = (TCHAR*)m_MsgData;
        bqsm.iTotalMsgSize = iTotalSize;
        bqsm.iTotalMsgObjSize = iMsgObjDataSize;
        bqsm.MsgData = m_MsgData;
        bqsm.MsgObjData = m_MsgObjData;
        CBQSM* pInput = new CBQSM();
        pInput->m_MessageType = BQ_MESSAGE;
        pInput->m_BQShortMessage = bqsm;
        CDecodedMessage decodedMessage;
        util.DecodedBQMessage(pInput, decodedMessage, nDataSize);
        TRACE("");
    }
}


/*---------------------------------------------------------------------------*/
/*!
  send a message

  @param		BYTE* pMsgData - pointer that points to the buffer of message
  @param		UINT uiMsgDataSize - size of the message
  @param		UINT* puiMsgID - a referenced pointer to delever the message ID
				which has been sent successfully
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Eddy Guo
  @date			2007/02/08
  */
HRESULT CMessage::SendMessage(BYTE* pMsgData, UINT uiMsgDataSize, UINT* puiMsgID)
{
	HRESULT hr = S_OK;

	int nErrorCode = 0;
    CRequestSendMessagePtr spRequest = new CRequestSendMessage(pMsgData, uiMsgDataSize,nErrorCode);

    EnqueueRequest(hr, spRequest);

	hr = nErrorCode == 0 ? S_OK : S_FALSE;

	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
  send a message that stored in the handset

  @param		UINT uiMsgID - the ID of the message
  @param		CString cstrAddress - the destination address
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Eddy Guo
  @date			2007/02/08
  */
HRESULT CMessage::SendMessageInStorage(UINT uiMsgID, CString cstrAddress)
{
	HRESULT hr = S_OK;

    CRequestSendMessageInStoragePtr spRequest = new CRequestSendMessageInStorage(uiMsgID, cstrAddress);

    spRequest->SetCurAtCmdSet(this->m_eCurAtCmdSet);

    hr = EnqueueRequest(hr, spRequest);

	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
  write a message into a specified folder in the handset

  @param		enumMsgLoc eMsgLoc - the specified folder
  @param		BYTE* pMsgData - pointer that points to the buffer of message
  @param		UINT uiMsgDataSize - size of the message
  @param		UINT* puiMsgID - a referenced pointer to delever the message ID
				which has been written successfully
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Eddy Guo
  @date			2007/02/08
  */
HRESULT CMessage::WriteMessage(enumMsgLoc eMsgLoc, BYTE* pMsgData, UINT uiMsgDataSize, UINT* puiMsgID)
{
	HRESULT hr = S_OK;

    CRequestWriteMessagePtr spRequest = new CRequestWriteMessage(eMsgLoc, pMsgData, uiMsgDataSize, puiMsgID);

    // from now, using standard at command
    enumAtCmdSet eCurrent = m_eCurAtCmdSet;
    m_eCurAtCmdSet = AT_CMD_STANDARD;

    if(m_eCurAtCmdSet == AT_CMD_STANDARD)

        spRequest->SetCurAtCmdSet(this->m_eCurAtCmdSet);

    hr = EnqueueRequest(hr, spRequest);

    if (hr == S_OK) *puiMsgID = spRequest->GetMsgID();

    m_eCurAtCmdSet = eCurrent;  ///< restore current command set

	return hr;
}

HRESULT CMessage::WriteMessage(enumStorageTag eMsgStorage, UINT nFolderID, UINT nTag, BYTE* pMsgData, UINT uiMsgDataSize, UINT* puiMsgID, char* szTime) {
    HRESULT hr = S_OK;

    CRequestWriteMessagePtr spRequest = new CRequestWriteMessage(DRAFT, pMsgData, uiMsgDataSize, puiMsgID);

    spRequest->SetReqWriteMessageParam(eMsgStorage, nFolderID, nTag, pMsgData, uiMsgDataSize, puiMsgID, szTime);
    // from now, using standard at command
    enumAtCmdSet eCurrent = m_eCurAtCmdSet;

    m_eCurAtCmdSet = AT_CMD_NEW_BENQ;

    spRequest->SetCurAtCmdSet(m_eCurAtCmdSet);

    hr = EnqueueRequest(hr, spRequest);

    if (hr == S_OK) *puiMsgID = spRequest->GetMsgID();

    m_eCurAtCmdSet = eCurrent;  ///< restore current command set

    return hr;
    return S_OK;
}


/*---------------------------------------------------------------------------*/
/*!
  read a message

  @param		UINT uiMsgID - the message ID
  @param		BYTE* pMsgData - a reference pointer that points to the buffer 
				of message
  @param		UINT* uiMsgDataSize - a reference pointer that points to the 
				size of the message
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Eddy Guo
  @date			2007/02/08
  */
HRESULT CMessage::ReadMessage(UINT uiMsgID, BYTE* pMsgData, UINT* puiMsgDataSize)
{
	HRESULT hr = S_OK;

    CRequestReadMessagePtr spRequest = new CRequestReadMessage(uiMsgID);

    spRequest->SetCurAtCmdSet(this->m_eCurAtCmdSet);

    hr = EnqueueRequest(hr, spRequest);

    if (hr == S_OK)
    {
        CBufferPtr spBuf = spRequest->GetMsgDataBuffer();

        *puiMsgDataSize = spRequest->GetMsgDataSize();

        memcpy(pMsgData, spBuf->GetData(), spBuf->GetLength());
    }
	return hr;
}

HRESULT CMessage::ReadMessage(UINT uiMsgID, CShortMessage& csm)
{
    if(m_eCurAtCmdSet == AT_CMD_STANDARD) return S_OK;

    HRESULT hr = S_OK;

    CRequestReadMessagePtr spRequest = new CRequestReadMessage(uiMsgID, &csm);

    spRequest->SetCurAtCmdSet(AT_CMD_NEW_BENQ);

    hr = EnqueueRequest(hr, spRequest);

    if (hr == S_OK)
    {
        //CBufferPtr spBuf = spRequest->GetMsgDataBuffer();

        //*puiMsgDataSize = spRequest->GetMsgDataSize();

        //memcpy(pMsgData, spBuf->GetData(), spBuf->GetLength());
    }
	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
  delete a message

  @param		UINT uiMsgID - the message ID
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Eddy Guo
  @date			2007/02/08
  */
HRESULT CMessage::DeleteMessage(UINT uiMsgID)
{
	HRESULT hr = S_OK;

    CRequestDeleteMessagePtr spRequest = new CRequestDeleteMessage(uiMsgID);

    spRequest->SetCurAtCmdSet(this->m_eCurAtCmdSet);

    hr = EnqueueRequest(hr, spRequest);

	return hr;
}

// get sub folders by folder ID
HRESULT CMessage::GetFolderEntries(BOOL bGetRootFolder, UINT nParentFolderID, vector<CMessageFolderInfo> *vEntryList)
{
    if(m_eCurAtCmdSet == AT_CMD_STANDARD) {
        CMessageFolderInfo mfiSMS;
        mfiSMS.bCanCreateSubfolder = false;
        mfiSMS.bEditable = false;
        mfiSMS.bHasSubFolder = true;
        mfiSMS.bIsProtected = true;
        mfiSMS.nID = 0;
        mfiSMS.strDisplayName = _T("SMS");

        CMessageFolderInfo mfiINBOX;
        mfiINBOX.bCanCreateSubfolder = false;
        mfiINBOX.bEditable = false;
        mfiINBOX.bHasSubFolder = false;
        mfiINBOX.bIsProtected = true;
        mfiINBOX.nID = 0;
        mfiINBOX.strDisplayName = _T("INBOX");

        CMessageFolderInfo mfiDRAFT;
        mfiDRAFT.bCanCreateSubfolder = false;
        mfiDRAFT.bEditable = false;
        mfiDRAFT.bHasSubFolder = false;
        mfiDRAFT.bIsProtected = true;
        mfiDRAFT.nID = 2;
        mfiDRAFT.strDisplayName = _T("DRAFT & OUTBOX");

        //CMessageFolderInfo mfiOUTBOX;
        //mfiOUTBOX.bCanCreateSubfolder = false;
        //mfiOUTBOX.bEditable = false;
        //mfiOUTBOX.bHasSubFolder = false;
        //mfiOUTBOX.bIsProtected = true;
        //mfiOUTBOX.nID = 2;
        //mfiOUTBOX.strDisplayName = _T("OUTBOX");

        CMessageFolderInfo mfiSENT;
        mfiSENT.bCanCreateSubfolder = false;
        mfiSENT.bEditable = false;
        mfiSENT.bHasSubFolder = false;
        mfiSENT.bIsProtected = true;
        mfiSENT.nID = 3;
        mfiSENT.strDisplayName = _T("SENT");

        vEntryList->push_back(mfiINBOX);
        vEntryList->push_back(mfiDRAFT);
        //vEntryList->push_back(mfiOUTBOX);
        vEntryList->push_back(mfiSENT);

        return S_OK;
    }

    HRESULT hr = S_OK;

    CRequestListFolderPtr spRequest = new CRequestListFolder(nParentFolderID, vEntryList);

    spRequest->SetCurAtCmdSet(AT_CMD_NEW_BENQ);

    hr = EnqueueRequest(hr, spRequest);

	return hr;
}
   
// create or rename message sub folders by folder ID
HRESULT CMessage::WriteFolderEntries(BOOL bCreate, BOOL bCreateRootFolder, UINT nParentFolderID, UINT nRenameFolderID, CString strDiaplayName, UINT &nNewID)
{
   HRESULT hr = S_OK;
   return hr;
}
   
// delete sub folders by folder ID
HRESULT CMessage::DeleteFolderEntry( UINT nFolderID )
{
   HRESULT hr = S_OK; 
   return hr;
}

HRESULT CMessage::UpdateMessageState(UINT uiMsgID, UINT nState) {

    HRESULT hr = S_OK;

    CRequestUpdateMessageStatePtr spRequest = new CRequestUpdateMessageState(uiMsgID, nState);

    spRequest->SetCurAtCmdSet(AT_CMD_NEW_BENQ);

    hr = EnqueueRequest(hr, spRequest);

    return hr;
    
}


int CMessage::Execute(CRequestPtr& ptRequest,int nTimeOut)
{
	ptRequest->Register(this,_T("Received"));	
	m_ReceiveEvent.ResetEvent();
	if (g_RequestManager.EnqueueRequest(ptRequest) == S_OK)
    {
		if(nTimeOut == 0)
			nTimeOut = m_nTimeOut;
		::WaitForSingleObject(m_ReceiveEvent,nTimeOut);
	}
	ptRequest->Unregister(this,_T("Received"));	
	return ptRequest->GetReceiverResult(ptRequest->GetReceiver());
}

//-------------------------------------------------------------------------
// This template function is extract the code from all above functions.
// This template function is used for all interface of class Message.
// This template function have to input a smart point type to verify
// what smart point it pointed. Please see the above function definition.
// There are many examples to show how to use this
// CMessage::EnqueueRequest()
//-------------------------------------------------------------------------
template<class SpType>
HRESULT CMessage::EnqueueRequest(HRESULT hr, SpType& spRequest)
{
    try {
        hr = g_RequestManager.EnqueueRequest(spRequest.GetRawPointer());
        if (hr == S_OK) {
            while (true) {
                if (spRequest->IsRequstCompleted()) {
                    if(spRequest->IsRequstCompletedFailed())
                        hr = E_FAIL;
                    break;
                }

                if (!g_RequestManager.IsConnected()) {
                    hr = E_FAIL;
                    break;
                }
            }
        }
        else
            hr = E_FAIL;
    } catch(...) {
        hr = E_FAIL;
    }
    return hr;
}