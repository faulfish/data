/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   SMSServer.cpp  $
 *  $Revision:   1.40  $
 *  $Date:   Mar 24 2008 10:33:52  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Components/SMSServer/SMSServer.cpp.-arc  $
   
      Rev 1.40   Mar 24 2008 10:33:52   Alan Tu
   check error
   
      Rev 1.39   Mar 21 2008 16:23:48   Alan Tu
   update for return value
   
      Rev 1.38   Feb 19 2008 16:18:04   Eddy Guo
   update
   
      Rev 1.37   Feb 19 2008 08:40:46   Eddy Guo
   update for sending message at offline mode
   
      Rev 1.36   Jan 23 2008 17:18:36   Eddy Guo
   update for standard at command
   
      Rev 1.35   Jan 16 2008 19:21:30   Eddy Guo
   remove warning
   
      Rev 1.34   Jan 16 2008 10:53:44   Eddy Guo
   update progress bar and add update message state
   
      Rev 1.33   Jan 14 2008 14:24:12   Eddy Guo
   update
   
      Rev 1.32   Dec 17 2007 15:05:56   Eddy Guo
   update
   
      Rev 1.31   Dec 17 2007 14:09:04   Eddy Guo
   update
   
      Rev 1.30   Nov 23 2007 08:31:32   Eddy Guo
   update
   
      Rev 1.29   Nov 22 2007 10:02:54   Eddy Guo
   update
   
      Rev 1.28   Nov 20 2007 17:13:52   Eddy Guo
   update
   
      Rev 1.27   Nov 09 2007 12:59:08   Eddy Guo
   Add Header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file SMSServer.cpp
    @brief {brief description of SMSServer.cpp}

    {detail description of SMSServer.cpp}
*/
// SMSServer.cpp : Defines the entry poUINT for the DLL application.
//
#include "stdafx.h"
#include "SMSServer.h"
#include <atlenc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
 
#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

CWinApp theApp;

using namespace std;

HINSTANCE InterfaceInst;

UINT _tmain(UINT argc, TCHAR* argv[], TCHAR* envp[]) {
    UINT nRetCode = 0;
    if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)) {
        _tprintf(_T("Fatal Error: MFC initialization failed\n"));
        nRetCode = 1;
    } else {
        TRACE("SMS Server DLL Main...");
    }
    return nRetCode;
}


CSMSServer::CSMSServer(): m_lMaxTimeOut(INFINITE), m_nMaxPageCount(5), m_bIsSupportPrivateFolder(false), m_pISMS(NULL) {
    DefineNotify();
}

CSMSServer::~CSMSServer(void) {
    TRACE("SMS Server Destructor...\n");
}

void CSMSServer::DefineNotify() {
    CNotifyBase::DefineNotify(EVENT_SMSSERVER_READ_MESSAGE    );
    CNotifyBase::DefineNotify(EVENT_SMSSERVER_DELETE_MESSAGE  );
    CNotifyBase::DefineNotify(EVENT_SMSSERVER_SAVE_MESSAGE    );
    CNotifyBase::DefineNotify(EVENT_SMSSERVER_LIST_ALL_MESSAGE);
    CNotifyBase::DefineNotify(EVENT_SMSSERVER_QUERY_SCA       );
    CNotifyBase::DefineNotify(EVENT_SMSSERVER_SEND_MESSAGE    );
    CNotifyBase::DefineNotify(EVENT_SMSSERVER_BACKUP_MESSAGE  );
    CNotifyBase::DefineNotify(EVENT_SMSSERVER_RESTORE_MESSAGE );
    CNotifyBase::DefineNotify(EVENT_SMSSERVER_QUERY_FOLDER    );

    CNotifyBase::DefineNotify(SMSSERVER_LIST_FOLDER_OK    );
    CNotifyBase::DefineNotify(SMSSERVER_LIST_ONE_FOLDER   );
    CNotifyBase::DefineNotify(SMSSERVER_READ_ONE_MESSAGE  );
    CNotifyBase::DefineNotify(SMSSERVER_DELETE_ONE_MESSAGE);
    CNotifyBase::DefineNotify(SMSSERVER_WRITE_ONE_MESSAGE );
    CNotifyBase::DefineNotify(SMSSERVER_SAVE_ONE_MESSAGE  );

    CNotifyBase::DefineNotify(SMSSERVER_BEGIN_PROGRESS_BAR);
    CNotifyBase::DefineNotify(SMSSERVER_END_PROGRESS_BAR  );

    CNotifyBase::DefineNotify(SMSSERVER_BACKUP_ONE_MESSAGE );
    CNotifyBase::DefineNotify(SMSSERVER_RESTORE_ONE_MESSAGE);
    CNotifyBase::DefineNotify(SMSSERVER_GET_MESSAGE_BY_LOC);

    CNotifyBase::DefineNotify(SMSSERVER_UPDATE_TREE_CONTROL);
    CNotifyBase::DefineNotify(SMSSERVER_UPDATE_LIST_CONTROL);

    CNotifyBase::DefineNotify(SMSSERVER_SEND_ONE_PAGE_MSG);
}
// if the at command set is standard, the get message by location
// is different from at new benq at command set.
// notice that: before get message by location, we need to know 
// what command set now is used.
bool CSMSServer::GetMessagesByLocation(int loc) {
    if((m_ATCommandSet  = GetATCommandSet()) == AT_CMD_UNKNOWN) return false;
    if(m_vecShortMessage.size() == 0) return false;
    vector<CShortMessage> vecCSM;
    if(m_ATCommandSet == AT_CMD_NEW_BENQ) {
        for(unsigned int i = 0; i < m_vecShortMessage.size(); ++i)
            if(m_vecShortMessage.at(i).m_nMessageLocation == loc)
                vecCSM.push_back(m_vecShortMessage.at(i));
    } else if(m_ATCommandSet == AT_CMD_STANDARD) {
        if(loc == 0 || loc == 1) {
            for(unsigned int i = 0; i < m_vecShortMessage.size(); ++i) {
                if(m_vecShortMessage.at(i).m_nMessageLocation == 1 || m_vecShortMessage.at(i).m_nMessageLocation == 0) {
                    vecCSM.push_back(m_vecShortMessage.at(i));
                }
            }
        } else {
            for(unsigned int i = 0; i < m_vecShortMessage.size(); ++i)
                if(m_vecShortMessage.at(i).m_nMessageLocation == loc)
                    vecCSM.push_back(m_vecShortMessage.at(i));
        }
    }
    Fire(SMSSERVER_GET_MESSAGE_BY_LOC, reinterpret_cast<long>(&vecCSM));
    return true;
}

//-----------------------------------------------------------------------------
// input: enumMsgLoc loc
// output: UINT
// usage: Indicate Location, return the number of this location
//-----------------------------------------------------------------------------
UINT CSMSServer::GetMessagesCountByLocation(enumMsgLoc loc) {
    if (DecodedMessages.IsEmpty()) return 0;
    CDecodedMessage* c;
    UINT totalCount = 0; // 在這個location裡有多少筆簡訊
    POSITION pos = DecodedMessages.GetHeadPosition();
    while (pos) {
        c = DecodedMessages.GetNext(pos);
        if (c->m_MessageLocation == loc) totalCount++;
    }
    return totalCount;
}

//-----------------------------------------------------------------------------
// input: UINT IDs[], UINT count, UINT fromFolder, UINT toFolder, long timeout
// output: bool
// usage: 指定搬移的message id, 一次可傳入多個id, 代參數count指定有多少id,
//         fromFolder指定從哪個folder搬到toFolder, timeout指定了等待時間,
//         超過也算失敗, 且不會再搬移未搬完的id直接回傳失敗
//-----------------------------------------------------------------------------
//bool CSMSServer::MoveFolder(UINT nIDs[], UINT count, UINT fromFolder, UINT toFolder, long timeout) {
//    // TODO:
//    // 將原來的message刪除之後再重新寫入, 注意所有的message是不是都讀出來了
//    return false;
//}



void CSMSServer::BackupWriteToFile(enumStorage storageSetting, CStdioFile& file) {
    CString strSMSBackup, strIDList;
    vector<SM> vecSM;   // 轉換一次, 使vecSM直接存到vecShortMessage裡
    vector<CShortMessage> vecShortMessage;
    MemUsed used;
    if(m_pISMS->SetMessageStorage(storageSetting, used) != S_OK)
        ASSERT(false);
    m_pISMS->ListMessage((enumMsgLoc)4, &strIDList, &vecSM, &vecShortMessage);
    if(storageSetting == STORAGE_HANDSET)
        strSMSBackup.Format(_T("ME SMS Backup: %d\n"), vecSM.size());
    else
        strSMSBackup.Format(_T("SM SMS Backup: %d\n"), vecSM.size());
    file.WriteString(strSMSBackup);
    for(UINT i = 0; i < vecSM.size(); ++i) {
        CString strWrite;
        strWrite.Format(_T("%d,%d,%S\n"), vecSM.at(i).nLength, vecSM.at(i).Location, (LPSTR)(LPCSTR)vecSM.at(i).straUnDecodedPDU);
        file.WriteString(strWrite);
        delete vecSM.at(i).PDU.szUserData;
        vecSM.at(i).PDU.szUserData = NULL;
        Fire(SMSSERVER_BACKUP_ONE_MESSAGE);
    }
}

//-----------------------------------------------------------------------------
// Method:    Backup
// FullName:  CSMSServer::Backup
// Access:    virtual public
// Returns:   bool
// Qualifier:
// Parameter: LPCTSTR pFilename indicate the name of file
//-----------------------------------------------------------------------------
bool CSMSServer::Backup(LPCTSTR pFilename) {
    bool bRes = false;
    CStdioFile file;
    if(file.Open(pFilename, CFile::modeCreate|CFile::modeWrite)) {
        if(m_pISMS == NULL) CreateISMS();
        BackupWriteToFile(STORAGE_HANDSET, file);
        BackupWriteToFile(STORAGE_SIM, file);
        file.Flush();
        file.Close();
        bRes = true;
    }
    MemUsed used;
    m_pISMS->SetMessageStorage(STORAGE_HANDSET, used);
    return bRes;
}

//-----------------------------------------------------------------------------
// Method:    GetNumberOfPDU
// FullName:  CSMSServer::GetNumberOfPDU
// Access:    public
// Returns:   int
// Qualifier:
// Parameter: LPCTSTR pFilename indicate the file to open
//-----------------------------------------------------------------------------
int CSMSServer::GetNumberOfPDU(LPCTSTR pFilename) {
    int nMENumOfPDU = 0;
    int nSMNumOfPDU = 0;
    CStdioFile file;
    CString strLine;
    if(file.Open(pFilename, CFile::modeRead)) {
        while(file.ReadString(strLine)) {
            if(strLine.Find(_T("ME SMS Backup:")) > -1)
                _stscanf((LPTSTR)(LPCTSTR)strLine, _T("ME SMS Backup: %d"), &nMENumOfPDU);
            else if(strLine.Find(_T("SM SMS Backup:")) > -1)
                _stscanf((LPTSTR)(LPCTSTR)strLine, _T("SM SMS Backup: %d"), &nSMNumOfPDU);
        }
    }
    file.Close();
    return nMENumOfPDU+nSMNumOfPDU;
}

//-----------------------------------------------------------------------------
// Method:    Restore
// FullName:  CSMSServer::Restore
// Access:    virtual public
// Returns:   bool
// Qualifier:
// Parameter: LPCTSTR pFilename
// Usage: indicate the file name to restore into Mobile.
//        there may be error when restore the same reference number.
//        change the reference number here maybe the good solution.
//-----------------------------------------------------------------------------
bool CSMSServer::Restore(LPCTSTR pFilename) {
    CStdioFile file;
    if(file.Open(pFilename, CFile::modeRead)) {
        CString strLine;
        int nNumOfPDU = 0;
        file.ReadString(strLine);
        if(strLine.Find(_T("ME SMS Backup:")) < 0) {
            ASSERT(false);
            return false;
        }
        _stscanf((LPTSTR)(LPCTSTR)strLine, _T("ME SMS Backup: %d"), &nNumOfPDU);
        if(m_pISMS == NULL) CreateISMS();
        MemUsed used;
        m_pISMS->SetMessageStorage(STORAGE_HANDSET, used);
        for(unsigned int i = 0; i < used.m_dwTotalMem1; ++i)
            m_pISMS->DeleteMessage(i);
        m_pISMS->SetMessageStorage(STORAGE_SIM, used);
        for(unsigned int i = 0; i < used.m_dwTotalMem1; ++i)
            m_pISMS->DeleteMessage(i);
        m_pISMS->SetMessageStorage(STORAGE_HANDSET, used);
        while(file.ReadString(strLine)) {
            if(strLine.Find(_T("SM SMS Backup:")) > -1) {
                _stscanf((LPTSTR)(LPCTSTR)strLine, _T("SM SMS Backup: %d"), &nNumOfPDU);
                m_pISMS->SetMessageStorage(STORAGE_SIM, used);
                file.ReadString(strLine);
            }
            UINT nSize = _tstoi(_tcstok((LPTSTR)(LPCTSTR)strLine, _T(",")));
            UINT nLoc  = _tstoi(_tcstok(NULL, _T(",")));
            TCHAR* szPDU = _tcstok(NULL, _T(","));
            CStringA straPDU;
            straPDU.Format("%S", szPDU);
            UINT retID;
            WriteMessage((enumMsgLoc)nLoc, (BYTE*)(LPSTR)(LPCSTR)straPDU, straPDU.GetLength(), &retID);
            Fire(SMSSERVER_RESTORE_ONE_MESSAGE);
        }
    }
    return true;
}


//-----------------------------------------------------------------------------
// input: UINT nCurrentFolderID, VecUINT& vecFolderID
// output: bool
// usage: 指定現在這一層所在的Folder ID, 詢問這一層裡還有哪些Folder
//         記錄在vector裡頭, 取出時注意, 因為要重建folder tree, 未定...
//-----------------------------------------------------------------------------
//bool CSMSServer::QueryFolder(bool bIsRoot, UINT nCurrentFolderID, VecUINT& vecFolderID) {
//    return false;
//}


//-----------------------------------------------------------------------------
// input: UINT nCurrentFolderID, VecUINT& vecFolderID
// output: bool
// usage: 指定現在這一層所在的Folder ID, 詢問這一層裡還有哪些Folder
//         記錄在vector裡頭, 取出時注意, 因為要重建folder tree, 未定...
//-----------------------------------------------------------------------------
//bool CSMSServer::QueryAllFolders() {
//    return false;
//}


//-----------------------------------------------------------------------------
// input: UINT nCurrentFolderID, VecUINT& vecMessageID
// output: bool
// usage: 指定現在這一層所在的Folder ID, 詢問這一層裡還有哪些Message ID
//         注意這裡只列出Message的ID, 並不列出訊息內容, 要用Read Message
//         來讀取, 未定...
//-----------------------------------------------------------------------------
//bool CSMSServer::QueryMessageByFolderID(UINT nCurrentFolderID, VecUINT& vecMessageID) {
//    return false;
//}
size_t CSMSServer::ListMessageByLocation(UINT nStart, UINT nEnd, size_t nTotalMessage, enumAtCmdSet cmdSet) {
    for(unsigned int i = nStart; i < nEnd; ++i) {
        vector<CShortMessage> vecTmp;
        if(cmdSet == AT_CMD_STANDARD)
            ListMessage(i, vecTmp); // list 0, 1, 2, 3 standard
        else if(cmdSet == AT_CMD_NEW_BENQ)
            ListMessage(m_vecFolder[i].nID, vecTmp);
        for(unsigned int j = 0; j < vecTmp.size(); ++j) {
            if(cmdSet == AT_CMD_STANDARD)
                vecTmp[j].m_nMessageLocation = i;
            else if(cmdSet == AT_CMD_NEW_BENQ)
                vecTmp[j].m_nMessageLocation = m_vecFolder[i].nID;
            m_vecShortMessage.push_back(vecTmp[j]);
        }
        nTotalMessage += vecTmp.size();
    }
    return nTotalMessage;
}

bool CSMSServer::DeleteFolderByID(UINT nFolderID) {
    return true;
}

//-----------------------------------------------------------------------------
// In this ReloadAllMessageFromMobile
// Each mobile may use different command to list message
// For standard, list all will read out all the messages in mobile
// For XTC BenQ, list will return the message id. You have to read each message by this id list
// For New BenQ, list will return the message id. You have to read each message by this id list
// Notice that each kind of command will return different value to indicate the message status
// Combining these information to show on VIEW is important to do so
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Method:    ReloadAllMessageFromMobile
// FullName:  CSMSServer::ReloadAllMessageFromMobile
// Access:    public 
// Returns:   bool
// Qualifier:
//-----------------------------------------------------------------------------
bool CSMSServer::ReloadAllMessageFromMobile() {
    bool bRes = false;
    if(m_pISMS == NULL) CreateISMS();
    CShortMessage csm;
    m_vecFolder.clear();
    ListFolder(0, m_vecFolder);
    m_vecShortMessage.clear();
    size_t nTotalMessage = 0;
    if(GetATCommandSet() == AT_CMD_STANDARD) {
        nTotalMessage = ListMessageByLocation(0, 4, nTotalMessage, AT_CMD_STANDARD);
    } else if(GetATCommandSet() == AT_CMD_NEW_BENQ) {
        nTotalMessage = ListMessageByLocation(0, (UINT)m_vecFolder.size(), nTotalMessage, AT_CMD_NEW_BENQ);
    }
    HRESULT hr = S_OK;
    for(unsigned int i = 0; i < nTotalMessage; ++i) {
        hr = m_pISMS->ReadMessage(m_vecShortMessage.at(i).m_vecMessageID.at(0), m_vecShortMessage.at(i));
    }
    Fire(EVENT_SMSSERVER_READ_MESSAGE, (long)&m_vecShortMessage);
    Fire(SMSSERVER_UPDATE_TREE_CONTROL, (long)&m_vecShortMessage);
    return bRes;
}

//UINT CSMSServer::ReadMessagesFromFolder(UINT nFolderID) {
//    return 0;
//}
//
//UINT CSMSServer::ListFolders(UINT nCurrentFolderID, VecUINT* vecFolderList) {
//    return 0;
//}

void CSMSServer::Event(const std::wstring &strEvent, long nParam) {
}


//-------------------------------------------------------------------------
// There interfaces is supplying for Message View. Message View should
// uses only List, Read, Send, Write and Delete to do message operation.
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
// For STD, ListMessage will return all message contents including message
// id, stat, alpha, length, pdu.
// For BenQ for TI, ListMessage will return all the message id only.
// For New Command, ListMessage will return all the message id and its
// status. This vector is saving decoded messages.
// List Message will transform all types(STD, BenQ, New BenQ) into
// CShortMessage.
//-------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Method:    ListMessage
// FullName:  CSMSServer::ListMessage
// Access:    public 
// Returns:   bool
// Qualifier:
// Parameter: int nLocation
// Parameter: vector<CShortMessage> & vecShortMessage
//-----------------------------------------------------------------------------
bool CSMSServer::ListMessage(int nLocation, vector<CShortMessage>& vecShortMessage) {
    if(m_pISMS == NULL) CreateISMS();
    bool bRes = true;
    CString strIDList;
    vector<SM> vecSM;
    vector<CShortMessage> vecCSM;
    MemUsed UsedMem;
    m_pISMS->SetMessageStorage(STORAGE_HANDSET, UsedMem);
    m_pISMS->ListMessage((enumMsgLoc)nLocation, &strIDList, &vecSM, &vecShortMessage);
    //m_pISMS->SetMessageStorage(STORAGE_SIM, SMUsed);
    //m_pISMS->ListMessage((enumMsgLoc)nLocation, &strIDList, &vecSMSM, &vecCSMSM);
    //for(size_t i = 0; i < vecCSMME.size(); ++i) {
    //    vecCSMME.at(i).m_storage = STORAGE_HANDSET;
    //    vecShortMessage.push_back(vecCSMME.at(i));
    //}
    //for(size_t i = 0; i < vecCSMSM.size(); ++i) {
    //    vecCSMSM.at(i).m_storage = STORAGE_SIM;
    //    vecShortMessage.push_back(vecCSMSM.at(i));
    //}
    for(unsigned int i = 0; i < vecSM.size(); ++i) {
        delete vecSM.at(i).PDU.szUserData;
        vecSM.at(i).PDU.szUserData = NULL;
    }
    //for(unsigned int i = 0; i < vecSMSM.size(); ++i) {
    //    delete vecSMSM.at(i).PDU.szUserData;
    //    vecSMSM.at(i).PDU.szUserData = NULL;
    //}
    return bRes;
}

// Read message by id. This function is reading out a message by the message id.
// It will return a decoded short message.
//-----------------------------------------------------------------------------
// Method:    ReadMessage
// FullName:  CSMSServer::ReadMessage
// Access:    public 
// Returns:   bool
// Qualifier:
// Parameter: UINT nMessageID
// Parameter: CShortMessage & message
//-----------------------------------------------------------------------------
bool CSMSServer::ReadMessage(UINT nMessageID, CShortMessage& message) {
    if(m_pISMS == NULL) CreateISMS();

    BYTE pMsgData[1024] = {0};
    UINT nMsgDataSize = 0;
    // ISMS裡頭的read message必須改變, 讀完就是已解碼的message,
    // 否測SMS Server無法得知要用什麼方式去解開, decode要在
    // 讀出來時就完成. 所以傳入時要用CShortMessage去接
    if(m_pISMS->ReadMessage(nMessageID, pMsgData, &nMsgDataSize) == S_OK) {
        CShortMessage csm;
        csm.m_vecMessageID.push_back(nMessageID);
        csm.m_strMessage.Format(_T("%S"), pMsgData);
        m_vecShortMessage.push_back(csm);
    } else {
        ASSERT(false);
    }
    Fire(SMSSERVER_UPDATE_TREE_CONTROL, (long)&m_vecShortMessage);
    return true;
}

// Send a message. Input short message and a vector of Recipients.
// Notice: After sending a message, write this message into mobile (outbox/sent).
//-----------------------------------------------------------------------------
// Method:    SendMessage
// FullName:  CSMSServer::SendMessage
// Access:    public 
// Returns:   bool
// Qualifier:
// Parameter: CString cstrMessage
// Parameter: vector<CString> & vecRecipient
//-----------------------------------------------------------------------------
bool CSMSServer::SendMessage(CString cstrMessage, vector<CString>& vecRecipient) {
    // call write message
    // call send message
    if(m_pISMS == NULL) CreateISMS();
    return false;
}

//-----------------------------------------------------------------------------
// Method:    SendMessage
// FullName:  CSMSServer::SendMessage
// Access:    public 
// Returns:   bool
// Qualifier:
// Parameter: CString strContactList
// Parameter: CString strContent
//-----------------------------------------------------------------------------
bool CSMSServer::SendMessage(CString strContactList, CString strContent) {
    bool bIsOnline = false;
    CommGetConnectivity(bIsOnline);
    if(!bIsOnline) {
        WriteMessage(strContactList, strContent, 3);
        return false;
    }
    if(m_pISMS == NULL) CreateISMS();
    bool bRet = false;
    if(GetATCommandSet() == AT_CMD_NEW_BENQ)
        bRet = SendMessageByUsingNewMechanism(strContactList, strContent);
    else if(GetATCommandSet() == AT_CMD_STANDARD)
        bRet = SendMessageByUsingStandardMethod(strContactList, strContent);
    return bRet;
}


// Send a message by indicating ID.
// Because the message may be a Concatenated message, this function will read from UI message
// to send multi page messages.
// Notice: After sending a message, write this message into mobile (outbox/sent).
bool CSMSServer::SendMessage(UINT nMessageID, CString cstrAddress) {
    if(m_pISMS == NULL) CreateISMS();
    bool bRes = false;
    if(m_pISMS->SendMessageInStorage(nMessageID, cstrAddress) == S_OK)
        bRes = true;
    else
        ASSERT(false);
    return false;
}

//-----------------------------------------------------------------------------
// Method:    WriteMessage
// FullName:  CSMSServer::WriteMessage
// Access:    public 
// Returns:   bool
// Qualifier:
// Parameter: enumMsgLoc eMsgLoc
// Parameter: BYTE * pMsgData
// Parameter: UINT uiMsgDataSize
// Parameter: UINT * puiMsgID
//-----------------------------------------------------------------------------
bool CSMSServer::WriteMessage(enumMsgLoc eMsgLoc, BYTE* pMsgData, UINT uiMsgDataSize, UINT* puiMsgID) {
    if(m_pISMS == NULL) CreateISMS();
    bool bRes = false;
    if(m_pISMS->WriteMessage(eMsgLoc, pMsgData, uiMsgDataSize, puiMsgID) == S_OK) {
        // save this message into vector
        bRes = true;
    } else {
        ASSERT(false);
    }
    return bRes;
}

//-----------------------------------------------------------------------------
// Method:    WriteMessage
// FullName:  CSMSServer::WriteMessage
// Access:    public
// Returns:   bool
// Qualifier:
// Parameter: CString strContactList
// Parameter: CString strContent
// Parameter: int nLoc
// Usage: calling this function will re-call
// WriteMessage(enumMsgLoc eMsgLoc, BYTE* pMsgData, UINT uiMsgDataSize, UINT* puiMsgID)
//-----------------------------------------------------------------------------
bool CSMSServer::WriteMessage(CString strContactList, CString strContent, int nLoc) {
    bool bRetVal = false;
    if(GetATCommandSet() == AT_CMD_NEW_BENQ)
        bRetVal = WriteMessageByUsingNewMechanism(strContactList, strContent, nLoc);
    else if(GetATCommandSet() == AT_CMD_STANDARD)
        bRetVal = WriteMessageByUsingStandardMethod(strContactList, strContent, nLoc);


    return bRetVal;
}

bool CSMSServer::WriteMessage(enumStorageTag eMsgStorage, UINT nFolderID, UINT nTag, BYTE* pMsgData, UINT uiMsgDataSize, UINT* nRetID, char* szTime) {
    if(m_pISMS == NULL) CreateISMS();
    return (m_pISMS->WriteMessage(eMsgStorage, nFolderID, nTag, pMsgData, uiMsgDataSize, nRetID, szTime) == S_OK) ? true : false;
}

bool CSMSServer::WriteDropMessage(CString strSender, CString strMsg, CString strState, CString strLoc, CString strStorage) {
    CString strSenderTmp = strSender.Mid(strSender.Find(':') + 2);
    CStringA strSenderA;
    strSenderA.Format("%S", strSenderTmp);
    CString strMsgTmp = strMsg.Mid(strMsg.Find(':') + 2);
    CStringA strMsgA;
    strMsgA.Format("%S", strMsgTmp);
    int nOutLength = QPEncodeGetRequiredLength(strMsgA.GetLength());
    char* szQPEncodeString = new char[nOutLength+1];
    memset(szQPEncodeString, 0, nOutLength+1);
    QPEncode((BYTE*)(LPSTR)(LPCSTR)strMsgA, strMsgA.GetLength(), szQPEncodeString, &nOutLength);
    CStringA strMsgWrite = strSenderA + CStringA("\r\n") + CStringA(szQPEncodeString) + CStringA("\r\n");
    CString strStateTmp = strState.Mid(strState.Find(':') + 2);
    CString strLocTmp = strLoc.Mid(strLoc.Find(':') + 2);
    CString strStorageTmp = strStorage.Mid(strStorage.Find(':') + 2);
    UINT nRetID = 0;
    char szTime[50];
    bool bRes = WriteMessage(
        strStorageTmp == "ME" ? STORAGE_HANDSET : STORAGE_SIM,
        _ttoi((LPCTSTR)strLocTmp),
        strStateTmp == "read" ? 1 : 0,
        (BYTE*)(LPSTR)(LPCSTR)strMsgWrite,
        strMsgWrite.GetLength(),
        &nRetID,
        szTime
        );
    delete szQPEncodeString;
    if(true) {
        CShortMessage csm;
        csm.m_bIsRead = true;
        csm.m_nMessageLocation = _ttoi((LPCTSTR)strLocTmp);
        csm.m_strDateTime.Format(_T("%S"), szTime);
        DWORD dwSize = MultiByteToWideChar(CP_UTF8, 0, strMsgA, -1, NULL, 0);
        wchar_t* szMsg = new wchar_t[dwSize+2];
        memset(szMsg, 0, sizeof(wchar_t) * (dwSize+2));
        MultiByteToWideChar(CP_UTF8, 0, strMsgA, -1, szMsg, dwSize+2);;
        csm.m_strMessage = szMsg;
        csm.m_strPhoneNumber = strSenderTmp;
        m_vecShortMessage.push_back(csm);
        Fire(EVENT_SMSSERVER_SAVE_MESSAGE, (long)&csm);
        Fire(SMSSERVER_UPDATE_TREE_CONTROL, (long)&m_vecShortMessage);
        delete szMsg;
    }
    Fire(SMSSERVER_RESTORE_ONE_MESSAGE);
    return bRes;
}


void CSMSServer::GetServiceCenterAddress(CString& strSCA)
{
    if(m_pISMS == NULL) CreateISMS();
    m_pISMS->GetServiceCenterAddress(&strSCA);
}

// Delete a message. Delete a message by indicating message id.
// UI will call DeleteMessage(CString strIDList);
// Delete will parse all index to delete. This is used for multi page SMS
bool CSMSServer::DeleteMessage(UINT nMessageID) {
    if(m_pISMS == NULL) CreateISMS();
    bool bRes = false;
    if(m_pISMS->DeleteMessage(nMessageID) == S_OK) {
        for(vector<CShortMessage>::iterator i1 = m_vecShortMessage.begin(); i1 != m_vecShortMessage.end(); ++i1) {
            for(vector<UINT>::iterator i2 = i1->m_vecMessageID.begin(); i2 != i1->m_vecMessageID.end(); ++i2) {
                if(*i2 == nMessageID) {
                    m_vecShortMessage.erase(i1);
                    return true;
                }
            }
        }
    }
    Fire(SMSSERVER_DELETE_ONE_MESSAGE);
    Fire(SMSSERVER_UPDATE_TREE_CONTROL, (long)&m_vecShortMessage);
    return bRes;
}

//-----------------------------------------------------------------------------
// Method:    DeleteMessage
// FullName:  CSMSServer::DeleteMessage
// Access:    public
// Returns:   bool
// Qualifier:
// Parameter: CString strIDList
//-----------------------------------------------------------------------------
bool CSMSServer::DeleteMessage(CString strIDList) {
    if(m_pISMS == NULL) CreateISMS();
    bool bRes = false;
    vector<UINT> vecID;
    TCHAR* szIDList = (LPTSTR)(LPCTSTR)strIDList;
    TCHAR* token = _tcstok(szIDList, _T(","));
    if(token != NULL)
        vecID.push_back((UINT)_ttoi64(token));

    while(token) {
        vecID.push_back((UINT)_ttoi64(token));
        token = _tcstok(NULL, _T(","));
    }
    if(vecID.size() == 0)
        return false;
    for(UINT i = 0; i < vecID.size(); ++i)
        DeleteMessage(vecID.at(i));
    return bRes;
}

//-----------------------------------------------------------------------------
// Method:    ListFolder
// FullName:  CSMSServer::ListFolder
// Access:    public
// Returns:   bool
// Qualifier:
// Parameter: UINT nFolderID
// Parameter: vector<CMessageFolderInfo> & vecFolder
//-----------------------------------------------------------------------------
bool CSMSServer::ListFolder(UINT nFolderID, vector<CMessageFolderInfo>& vecFolder) {
    // for Standard
    bool bRes = false;
    if(m_pISMS == NULL) CreateISMS();
    if(m_pISMS && m_pISMS->GetFolderEntries(FALSE, nFolderID, &vecFolder) == S_OK) {
        Fire(SMSSERVER_LIST_FOLDER_OK, (long)&vecFolder);
        bRes = true;
    }
    return bRes;
}

//-----------------------------------------------------------------------------
// Method:    QueryMessageSpace
// FullName:  CSMSServer::QueryMessageSpace
// Access:    public
// Returns:   bool
// Qualifier:
// Parameter: enumStorage eStorage
// Parameter: UINT & nRemnant
//-----------------------------------------------------------------------------
bool CSMSServer::QueryMessageSpace(enumStorage eStorage, UINT& nRemnant) {
    bool bRes = false;
    if(m_pISMS == NULL) CreateISMS();
    MemUsed usedMem;
    m_pISMS->SetMessageStorage(eStorage, usedMem);
    nRemnant = usedMem.m_dwTotalMem1 - usedMem.m_dwUsedMem1;
    m_pISMS->SetMessageStorage((STORAGE_HANDSET), usedMem);
    return true;
}


bool CSMSServer::UpdateMessageState(UINT nIndex, UINT nState) {
    bool bRes = false;
    if(m_pISMS == NULL) CreateISMS();
    HRESULT hRes = m_pISMS->UpdateMessageState(nIndex, nState);
    // after update the message, update the message already read into vector
    // add notify to update the message
    for(size_t i = 0; i < m_vecShortMessage.size(); ++i) {
        if(m_vecShortMessage.at(i).m_vecMessageID.at(0) == nIndex) {
            m_vecShortMessage.at(i).m_bIsRead = true;
            break;
        }
    }
    Fire(EVENT_SMSSERVER_READ_MESSAGE, (long)(&m_vecShortMessage));
    Fire(SMSSERVER_UPDATE_TREE_CONTROL, (long)&m_vecShortMessage);
    return true;
}

enumAtCmdSet CSMSServer::GetATCommandSet() {
    CQMobileConfigure* pConfigure = GetMobileConFig();
    CQSetting qSetting;
    TCHAR szMobileModel[512] = {0};
    TCHAR szIMEI[512] = {0};
    TCHAR szDisplayName[512] = {0};
    CommGetMobileInfo(_T("Message Request"), szMobileModel, szIMEI, szDisplayName);
    pConfigure->GetMobileSettings(szMobileModel, qSetting);
    TSTRING szDCS = qSetting.Get(_T("Data_Coding_Scheme"));
    if(szDCS == _T("STANARD_shcheme"))
        m_ATCommandSet = AT_CMD_STANDARD;
    else if(szDCS == _T("BENQ_shcheme"))
        m_ATCommandSet = AT_CMD_BENQ;
    else if(szDCS == _T("NEW_BENQ_shcheme"))
        m_ATCommandSet = AT_CMD_NEW_BENQ;
    return m_ATCommandSet;
}

// private function for send and write message
bool CSMSServer::SendMessageByUsingNewMechanism(CString strContactList, CString strContent) {
    CMessageUtil util;
    CString strSCA;
    GetServiceCenterAddress(strSCA);
    vector<CString> vecStrContactList;
    vector<CStringA> vecStrSplit7BitContent;
    vector<CString> vecStrSplitUCS2Content;
    vector<CStringA> vecUserData;
    vector<CStringA> vecWritePDU;
    util.SplitStringIntoVector(strContactList, vecStrContactList, _T(",;"));
    //GetPDU(strContactList, strContent, vecStrContactList, vecStrSplit7BitContent, vecStrSplitUCS2Content, vecUserData, vecWritePDU);
    vector<UINT> vecReturnID;
    UINT nReturnID = 0;
    HRESULT hr = S_OK;
    bool bRetVal = true;
    for(UINT i = 0; i < vecStrContactList.size(); ++i) {
        vector<CString> vecStrEachContact;
        util.GetPDU(vecStrContactList.at(i), strContent, strSCA, vecStrEachContact, vecStrSplit7BitContent, vecStrSplitUCS2Content, vecUserData, vecWritePDU);
        for(UINT j = 0; j < vecWritePDU.size(); ++j) {
            hr = m_pISMS->SendMessage((BYTE*)(LPSTR)(LPCSTR)vecWritePDU.at(j), vecWritePDU.at(j).GetLength(), &nReturnID);
            if(hr == E_FAIL) {
                bRetVal = false;
                break;
            }
            Sleep(200); ///< if send multi page, this thread should sleep. Or mobile will power cut.
        }
        if(hr == E_FAIL) break;
        vecReturnID.push_back(nReturnID);
    }
    CString writeContacts((LPTSTR)(LPCTSTR)strContactList);
    if(bRetVal) WriteMessage(writeContacts, strContent, 4); // Send SUCCEEDED, write into SENT
    else        WriteMessage(writeContacts, strContent, 3); // Send failed, write into OUTBOX
    return bRetVal;
}
bool CSMSServer::SendMessageByUsingStandardMethod(CString strContactList, CString strContent) {
    CString writeContacts((LPTSTR)(LPCTSTR)strContactList);
    CMessageUtil util;
    CString strSCA;
    GetServiceCenterAddress(strSCA);
    vector<CString> vecStrContactList;
    vector<CStringA> vecStrSplit7BitContent;
    vector<CString> vecStrSplitUCS2Content;
    vector<CStringA> vecUserData;
    vector<CStringA> vecWritePDU;
    util.SplitStringIntoVector(strContactList, vecStrContactList, _T(",;"));
    //GetPDU(strContactList, strContent, vecStrContactList, vecStrSplit7BitContent, vecStrSplitUCS2Content, vecUserData, vecWritePDU);
    vector<UINT> vecReturnID;
    UINT nReturnID = 0;
    HRESULT hrSendMessage;
    bool bWriteSuccess = false;
    for(UINT i = 0; i < vecStrContactList.size(); ++i) {
        vector<CString> vecStrEachContact;
        util.GetPDU(vecStrContactList.at(i), strContent, strSCA, vecStrEachContact, vecStrSplit7BitContent, vecStrSplitUCS2Content, vecUserData, vecWritePDU);
		for(UINT j = 0; j < vecWritePDU.size(); ++j) {
			hrSendMessage = m_pISMS->SendMessage((BYTE*)(LPSTR)(LPCSTR)vecWritePDU.at(j), vecWritePDU.at(j).GetLength(), &nReturnID);
			if((j+1) < vecWritePDU.size())
				Sleep(200); ///< if send multi page, this thread should sleep. Or mobile will power cut.
		}
		for(UINT j = 0; j < vecWritePDU.size(); ++j) {
			if(hrSendMessage == S_OK)
				bWriteSuccess = WriteMessage((enumMsgLoc)3, (BYTE*)(LPSTR)(LPCSTR)vecWritePDU.at(j), vecWritePDU.at(j).GetLength(), &nReturnID);
			else
				bWriteSuccess = WriteMessage((enumMsgLoc)2, (BYTE*)(LPSTR)(LPCSTR)vecWritePDU.at(j), vecWritePDU.at(j).GetLength(), &nReturnID);
			vecReturnID.push_back(nReturnID);
			if((j+1) < vecWritePDU.size())
				Sleep(200); ///< if write multi page, this thread should sleep. Or mobile will power cut.
		}
        if(bWriteSuccess) {
            CShortMessage csm;
            csm.m_bIsRead = true;
            if(hrSendMessage == S_OK)
                csm.m_nMessageLocation = 3;
            else
                csm.m_nMessageLocation = 2;
            csm.m_strDateTime.Format(_T("%S"), "----/--/-- --:--:--");
            csm.m_strMessage = strContent;
            csm.m_vecMessageID = vecReturnID;
            if(vecStrContactList.size() != 0)
                csm.m_strPhoneNumber = vecStrContactList.at(0);
            m_vecShortMessage.push_back(csm);
            Fire(EVENT_SMSSERVER_SAVE_MESSAGE, (long)&csm);
            Fire(SMSSERVER_UPDATE_TREE_CONTROL, (long)&m_vecShortMessage);
        }
    }
    return hrSendMessage == S_OK;
}
bool CSMSServer::WriteMessageByUsingNewMechanism(CString strContactList, CString strContent, int nLoc) {
    DWORD dwNum = WideCharToMultiByte(CP_UTF8, NULL, (LPTSTR)(LPCTSTR)strContent, -1, NULL, 0, NULL, FALSE);
    char* szUTF8 = new char[dwNum];
    memset(szUTF8, 0, dwNum);
    WideCharToMultiByte (CP_UTF8, NULL, (LPTSTR)(LPCTSTR)strContent, -1, szUTF8, dwNum, NULL, FALSE);
    if(m_pISMS == NULL) CreateISMS();
    int nOutLength = QPEncodeGetRequiredLength(dwNum);
    char* szQPEncodeString = new char[nOutLength+100];
    memset(szQPEncodeString, 0, nOutLength+100);
    QPEncode((BYTE*)szUTF8, dwNum, szQPEncodeString, &nOutLength);
    CStringA strToME(szQPEncodeString);
    CMessageUtil util;
    vector<CString> vecContact;
    util.SplitStringIntoVector(strContactList, vecContact, _T(";, "));
    CStringA strFirst;
    if(vecContact.size() != 0)
        strFirst.Format("%S", (LPTSTR)(LPCTSTR)vecContact.at(0));
    CStringA strWrite = strFirst + CStringA("\r\n") + CStringA(szQPEncodeString) + CStringA("\r\n");
    delete [] szQPEncodeString;
    delete [] szUTF8;
    UINT nRetID = 0;        ///< for output message id
    char szTime[20] = {0};  ///< for output timestamp
    WriteMessage(STORAGE_HANDSET, nLoc, 1, (BYTE*)(LPSTR)(LPCSTR)strWrite, strWrite.GetLength(), &nRetID, szTime);

    CShortMessage csm;
    csm.m_bIsRead = true;
    csm.m_nMessageLocation = nLoc;
    csm.m_strDateTime.Format(_T("%S"), szTime);
    csm.m_strMessage = strContent;
    csm.m_vecMessageID.push_back(nRetID);
    if(vecContact.size() != 0)
        csm.m_strPhoneNumber = vecContact.at(0);
    m_vecShortMessage.push_back(csm);
    Fire(EVENT_SMSSERVER_SAVE_MESSAGE, (long)&csm);
    Fire(SMSSERVER_UPDATE_TREE_CONTROL, (long)&m_vecShortMessage);
    return true;
}
bool CSMSServer::WriteMessageByUsingStandardMethod(CString strContactList, CString strContent, int nLoc) {
    CMessageUtil util;
    CString strSCA;
    GetServiceCenterAddress(strSCA);
    vector<CString> vecStrContactList;
    vector<CStringA> vecStrSplit7BitContent;
    vector<CString> vecStrSplitUCS2Content;
    vector<CStringA> vecUserData;
    vector<CStringA> vecWritePDU;
    util.GetPDU(strContactList, strContent, strSCA, vecStrContactList, vecStrSplit7BitContent, vecStrSplitUCS2Content, vecUserData, vecWritePDU);

    vector<UINT> vecReturnID;
    UINT nReturnID;
    for(UINT i = 0; i < vecWritePDU.size(); ++i) {
        WriteMessage((enumMsgLoc)nLoc, (BYTE*)(LPSTR)(LPCSTR)vecWritePDU.at(i), vecWritePDU.at(i).GetLength(), &nReturnID);
        vecReturnID.push_back(nReturnID);
    }
    CShortMessage csm;
    csm.m_bIsRead = true;
    csm.m_nMessageLocation = nLoc;
    csm.m_strDateTime = _T("----/--/-- --:--:--");
    csm.m_strMessage = strContent;
    if(vecStrContactList.size() != 0) csm.m_strPhoneNumber = vecStrContactList.at(0);
    csm.m_vecMessageID = vecReturnID;
    m_vecShortMessage.push_back(csm);
    Fire(EVENT_SMSSERVER_SAVE_MESSAGE, (long)&csm);
    Fire(SMSSERVER_UPDATE_TREE_CONTROL, (long)&m_vecShortMessage);
    return true;
}

#pragma warning(pop)