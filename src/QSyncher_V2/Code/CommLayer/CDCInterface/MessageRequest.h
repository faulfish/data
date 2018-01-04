/*****************************************************************************/
/** @file: MessageRequest.h
  This is the header file that contains all the message related classes.
  $Author:   Alan Tu  $
  $Version:  1.0 $
  $Date:   Mar 21 2008 16:23:08  $
 */
/*****************************************************************************/

#ifndef __MESSAGEREQUEST_H__
#define __MESSAGEREQUEST_H__

#pragma once
#include "PotocolRequestBase.h"
#include "Global_Def.h"
#include "ATFSRequest.h"
#include "../../Components/SMSServer/MessageStructure.h"
#include "../../Components/Configuration/QConfigure/QMobileConfigure.h"
#include "../../CommLayer/MobileInterface/Interface_Include/ICommMobile.h"

//-----------------------------------------------------------------------------
// Input: Codec Type (m_spAtCodec)
//        Request Type (CMessageBaseRequest)
//        HANDLE (hDataArrivalEv)
// This tamplate function is used for Execute Request.
// This template function is refactoring of all ExecuteRequest of each Request.
// Please see the following to know what codes are refactroing.
//-----------------------------------------------------------------------------
template<class CodecType, class RequestType>
HRESULT ExecuteRequestT(CodecType spAtCodec, RequestType* request, HANDLE& hDataArrivalEvent) {
    g_ProtocolManager.ExecuteProtocolRequest(spAtCodec->GetPackageBuffer(), request);
    HRESULT hr = WaitForSingleObject(hDataArrivalEvent, 30000);
    if(hr == WAIT_OBJECT_0) {
        if(!request->GetResult())
            hr = E_FAIL;
        else
            hr = S_OK;
    }
    return hr;
}

/*! @class		CMessageBaseRequest
 *  @brief		the base class of message request with AT command
 *  @author		Eddy Guo
 *  @date		2007/01/30
 */

using namespace QSYNC_LIB;
class CMessageBaseRequest: public CATFSBaseRequest {
public:
    // when create a request, this request have to know what command it used.
    // to get the command set by reading config xml file.
    CMessageBaseRequest(DWORD dwTimeout = INFINITE): CATFSBaseRequest(dwTimeout) { }
    virtual ~CMessageBaseRequest() {}
    virtual const bool DoDataArrival(CBufferPtr &spBuffer) { return true; }
    virtual enumProtocolConnection Open() { return CONNECTION_SUCCEEDED; }
    virtual enumProtocolConnection Close() { return CONNECTION_SUCCEEDED; }
    virtual BOOL GetResult() { return m_bResult; }
    virtual void OnWriteCompleted() {}
    enumAtCmdSet GetCurAtCmdSet() { return m_eCurAtCmdSet; }
    void SetCurAtCmdSet(enumAtCmdSet set) { m_eCurAtCmdSet = set; }
protected:
    CQMobileConfigure* m_pConfigure;    ///< Get the mobile config
    CAtCodecPtr m_spATCodec;            ///< at codec, please refer CAtCodecPtr
    BOOL m_bResult;                     ///< the command is success or not
    enumAtCmdSet m_eCurAtCmdSet;        ///< indicate the command is what kinds of AT command set
};


/*! @class		CRequestGetSCA
 *  @brief		get the service center address
 *  @author		Eddy Guo
 *  @date		2007/01/30
 */
class CRequestGetSCA: public CMessageBaseRequest {
public:
    CRequestGetSCA();
    ~CRequestGetSCA() {}
    HRESULT ExecuteRequest();
    void OnDataArrival(CBufferPtr spBuffer);
    CString GetSCA() { return m_strSCA; }
private:
    HANDLE m_hDataArrivalEvent;
    CString m_strSCA;   ///< SCA string
};
typedef CRefObjectPtr<CRequestGetSCA> CRequestGetSCAPtr;


/*! @class		CRequestSetPDUMode
 *  @brief		set the mobile to PDU mode
 *  @author		Eddy Guo
 *  @date		2007/01/30
 */
class CRequestSetPDUMode: public CMessageBaseRequest {
public:
    CRequestSetPDUMode();
    CRequestSetPDUMode(UINT nMode);
    ~CRequestSetPDUMode() {}
    HRESULT ExecuteRequest();
    void OnDataArrival(CBufferPtr spBuffer);
private:
    HANDLE m_hDataArrivalEvent;
    UINT m_nMode; ///< If the value is 0, this is PDU mode. If it is 1, this is TEXT mode.
};
typedef CRefObjectPtr<CRequestSetPDUMode> CRequestSetPDUModePtr;


/*! @class		CRequestSetMessageStorage
 *  @brief		set the message storage
 *  @author		Eddy Guo
 *  @date		2007/01/30
 */
class CRequestSetMessageStorage: public CMessageBaseRequest {
public:
    CRequestSetMessageStorage(enumStorage eMsgStorage, MemUsed& used);
    ~CRequestSetMessageStorage() {}
    HRESULT ExecuteRequest();
    void OnDataArrival(CBufferPtr spBuffer);
private:
    HANDLE m_hDataArrivalEvent;
    enumStorage m_eMsgStorage;  //< indicate what storage it is
    MemUsed* m_used;
};
typedef CRefObjectPtr<CRequestSetMessageStorage> CRequestSetMessageStoragePtr;


/*! @class		CRequestQueryMessageSpace
 *  @brief		query the message storage status
 *  @author		Eddy Guo
 *  @date		2007/01/30
 */
class CRequestQueryMessageSpace: public CMessageBaseRequest {
public:
    CRequestQueryMessageSpace(enumStorage eMsgStorage);
    ~CRequestQueryMessageSpace() {}
    HRESULT ExecuteRequest();
    void OnDataArrival(CBufferPtr spBuffer);
    DWORD GetUsed() { return m_dwUsed; }
    DWORD GetTotal() { return m_dwTotal; }
private:
    HANDLE m_hDataArrivalEvent;
    enumStorage m_eMsgStorage;
    DWORD m_dwUsed;     ///< used count
    DWORD m_dwTotal;    ///< total count
};
typedef CRefObjectPtr<CRequestQueryMessageSpace> CRequestQueryMessageSpacePtr;


/*! @class		CRequestListMessageTi
 *  @brief		list the message index in a specified folder
 *  @author		Eddy Guo
 *  @modified    Eddy Guo, Alan Tu
 *  @date		2007/01/30
 *  @last update 2007/08/10
 */
class CRequestListMessageTi: public CMessageBaseRequest {
public:
    CRequestListMessageTi(enumMsgLoc eMsgLoc, CString* pcstrMsgIDList, enumAtCmdSet set): m_pstrMsgList(pcstrMsgIDList)
    {
        m_spATCodec = CAtCodec::CreateReqListMessagesTi(eMsgLoc);
        m_ptBufferSend = m_spATCodec->GetPackageBuffer();
        m_eCurAtCmdSet = set;
        
    }
    virtual ~CRequestListMessageTi() {}
    virtual const bool DoDataArrival(CBufferPtr &spBuffer);
    bool IsOK(const CBufferPtr &ptBuffer) {
        bool bRes = false;
        BYTE szATOK[14] =  { 0x0d, 0x0a, 'O', 'K', 0x0d, 0x0a, 0x0d, 0x0a, 0x0d, 0x0a, 0x0d, 0x0a, 0x0d, 0x0a};
        if(ptBuffer->GetLength() >= 14) {
            BYTE *pTemp = (BYTE*)ptBuffer->GetData();
            BYTE *pEnd = pTemp + (ptBuffer->GetLength() - 14);
            bRes = memcmp(pEnd, szATOK, 14) == 0;
        }
        return bRes;
    }
private:
    CString *m_pstrMsgList; ///< ID list in the location
};
typedef CRefObjectPtr<CRequestListMessageTi> CRequestListMessageTiPtr;


/*! @class		CRequestListMessageStd
 *  @brief		list all the messages
 *  @author		Eddy Guo
 *  @date		2007/01/30
 */
class CRequestListMessageStd: public CMessageBaseRequest {
public:
    CRequestListMessageStd(int nStat, CString* pcstrMsgIDList, std::vector<SM>* pVecSM,
        enumAtCmdSet set, std::vector<CShortMessage>* pVecShortMessage):
      m_pVecSM(pVecSM), m_pstrMsgList(pcstrMsgIDList), m_pVecShortMessage(pVecShortMessage)
    {
        m_spATCodec = CAtCodec::CreateReqListMessagesStd(nStat);
        m_ptBufferSend = m_spATCodec->GetPackageBuffer();
        m_eCurAtCmdSet = set;
        m_pstrMsgList = pcstrMsgIDList;
    }
    virtual ~CRequestListMessageStd() {}
    virtual const bool DoDataArrival(CBufferPtr &spBuffer);
private:
    CString *m_pstrMsgList;     ///< ID in the stat
    std::vector<SM>* m_pVecSM;  ///< message vector, decoded, merged
    std::vector<CShortMessage>* m_pVecShortMessage;
};
typedef CRefObjectPtr<CRequestListMessageStd> CRequestListMessageStdPtr;

/*! @class		CRequestListMessageNew
 *  @brief		List Messages ID
 *  @author		Eddy Guo
 *  @date		2007/11/1
 */
class CRequestListMessageNew: public CMessageBaseRequest {
public:
    CRequestListMessageNew(UINT nFolderID, CString* pcstrMsgIDList,
        enumAtCmdSet set, vector<CShortMessage>* pVecShortMessage):
      m_nFolderID(nFolderID), m_pstrMsgIDList(pcstrMsgIDList), m_pVecShortMessage(pVecShortMessage)
    {
        m_spATCodec = CAtCodec::CreateReqListMessagesNew(nFolderID);
        m_ptBufferSend = m_spATCodec->GetPackageBuffer();
        m_eCurAtCmdSet = set;
    }
    virtual ~CRequestListMessageNew() {}
    virtual const bool DoDataArrival(CBufferPtr &spBuffer);
private:
    UINT m_nFolderID;
    CString *m_pstrMsgIDList;     ///< ID in the stat
    //std::vector<SM>* m_pVecSM;  ///< message vector, decoded, merged
    std::vector<CShortMessage>* m_pVecShortMessage;
};
typedef CRefObjectPtr<CRequestListMessageNew> CRequestListMessageNewPtr;


/*! @class		CRequestSendMessage
 *  @brief		send a message
 *  @author		Eddy Guo
 *  @date		2007/01/30
 */
class CRequestSendMessage: public CMessageBaseRequest {
public:
    CRequestSendMessage(BYTE *pMsgData, DWORD dwMsgDataSize,int& nErrorCode);
    ~CRequestSendMessage() {}
    HRESULT ExecuteRequest();
    void OnDataArrival(CBufferPtr spBuffer);
    DWORD GetMsgID() { return m_dwMsgID; }
private:
    HANDLE m_hDataArrivalEvent;
    BOOL m_bIsFirstSegmentWritten;
    BYTE *m_pMsgData;       ///< Message data
    DWORD m_dwMsgDataSize;  ///< Message data size
    DWORD m_dwMsgID;        ///< Return Message ID
	int&			m_nErrorCode;
};
typedef CRefObjectPtr<CRequestSendMessage> CRequestSendMessagePtr;


/*! @class		CRequestSendMessageInStorage
 *  @brief		send a message that stored in the handset
 *  @author		Eddy Guo
 *  @date		2007/01/30
 */
class CRequestSendMessageInStorage: public CMessageBaseRequest {
public:
    CRequestSendMessageInStorage(DWORD dwMsgID, CString strAddress);
    ~CRequestSendMessageInStorage() {}
    HRESULT ExecuteRequest();
    void OnDataArrival(CBufferPtr spBuffer);
private:
    //BOOL QueryCurAtCommandSetFromCfgAndSet();
private:
    HANDLE m_hDataArrivalEvent;
    DWORD m_dwMsgID;        ///< Send Message is memory
    CString m_strAddress;   ///< Address
};
typedef CRefObjectPtr<CRequestSendMessageInStorage> CRequestSendMessageInStoragePtr;


/*! @class		CRequestWriteMessage
 *  @brief		write a message into a specified folder in the handset
 *  @author		Eddy Guo
 *  @date		2007/01/30
 */
class CRequestWriteMessage: public CMessageBaseRequest {
public:
    CRequestWriteMessage(enumMsgLoc eMsgLoc, BYTE *pMsgData, DWORD dwMsgDataSize, UINT* puiMsgID);
    ~CRequestWriteMessage() {}
    HRESULT ExecuteRequest();
    void OnDataArrival(CBufferPtr spBuffer);
    DWORD GetMsgID() { return m_dwMsgID; }
    // If the command is new AT_CMD_NEW_BENQ, these member data have to be input
    // The return value are tow part of the message id that written in ME and the time of the message.
    void SetReqWriteMessageParam(enumStorageTag eMsgStorage, UINT nFolderID, UINT nTag, BYTE* pMsgData, UINT uiMsgDataSize, UINT* puiMsgID, char* szTime) {
        m_eMsgStorage = eMsgStorage;
        m_nFolderID = nFolderID;
        m_nTag = nTag;
        m_pMsgData = pMsgData;
        m_msgDataSize = uiMsgDataSize;
        m_nRetID = puiMsgID;
        m_szTime = szTime;
    }
private:
    // BOOL QueryCurAtCommandSetFromCfgAndSet();
private:
    HANDLE m_hDataArrivalEvent;
    BOOL m_bIsFirstSegmentWritten;
    enumMsgLoc m_eMsgLoc;
    BYTE *m_pMsgData;
    DWORD m_dwMsgDataSize;
    DWORD m_dwMsgID;

    enumStorage m_eMsgStorage;  ///< Message Storage
    UINT m_nFolderID;           ///< Folder ID
    UINT m_nTag;                ///< Tag
    UINT* m_nRetID;             ///< Return ID
    int m_msgDataSize;          ///< Recipient length
    char* m_szTime;               ///< This is output for timestamp
};
typedef CRefObjectPtr<CRequestWriteMessage> CRequestWriteMessagePtr;


/*! @class		CRequestReadMessage
 *  @brief		read a message
 *  @author		Eddy Guo
 *  @date		2007/01/30
 */
class CRequestReadMessage: public CMessageBaseRequest {
public:
    CRequestReadMessage(DWORD dwMsgID, CShortMessage* pCSM = NULL);
    ~CRequestReadMessage() {}
    HRESULT ExecuteRequest();
    void OnDataArrival(CBufferPtr spBuffer);
    DWORD GetMsgDataSize() { return m_dwMsgDataSize; }
    CBufferPtr GetMsgDataBuffer() { return m_spMsgDataBuffer; }
private:
    HANDLE m_hDataArrivalEvent;
    DWORD m_dwMsgID;
    DWORD m_dwMsgDataSize;
    CBufferPtr m_spMsgDataBuffer;
    CShortMessage* m_pCSM;
};
typedef CRefObjectPtr<CRequestReadMessage> CRequestReadMessagePtr;


/*! @class		CRequestDeleteMessage
 *  @brief		delete a message
 *  @author		Eddy Guo
 *  @date		2007/01/30
 */
class CRequestDeleteMessage: public CMessageBaseRequest {
public:
    CRequestDeleteMessage(DWORD dwMsgID);
    ~CRequestDeleteMessage() {}
    HRESULT ExecuteRequest();
    void OnDataArrival(CBufferPtr spBuffer);

private:
    HANDLE m_hDataArrivalEvent;
    DWORD m_dwMsgID;
};
typedef CRefObjectPtr<CRequestDeleteMessage> CRequestDeleteMessagePtr;

/*! @class		CRequestListFolder
 *  @brief		delete a message
 *  @author		Eddy Guo
 *  @date		2007/01/30
 */
class CRequestListFolder: public CMessageBaseRequest {
public:
    CRequestListFolder(unsigned int nFolderID, std::vector<CMessageFolderInfo>* vecFolder);
    ~CRequestListFolder() {}
    HRESULT ExecuteRequest();
    void OnDataArrival(CBufferPtr spBuffer);
private:
    HANDLE m_hDataArrivalEvent;
    unsigned int m_nFolderID;
    std::vector<CMessageFolderInfo>* m_pVecFolder;
};
typedef CRefObjectPtr<CRequestListFolder> CRequestListFolderPtr;

/*! @class		CRequestUpdateMessageState
*  @brief		update message state
*  @author		Eddy Guo
*  @date		2008/01/15
*/
class CRequestUpdateMessageState: public CMessageBaseRequest {
public:
    CRequestUpdateMessageState(unsigned int nIndex, unsigned int nState);
    ~CRequestUpdateMessageState() {}
    HRESULT ExecuteRequest();
    void OnDataArrival(CBufferPtr spBuffer);
private:
    HANDLE m_hDataArrivalEvent;
    unsigned int m_nIndex;
    unsigned int m_nState;

};
typedef CRefObjectPtr<CRequestUpdateMessageState> CRequestUpdateMessageStatePtr;


#endif // #ifndef __MESSAGEREQUEST_H__
