/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   MessageServerAdapter.cpp  $
 *  $Revision:   1.29  $
 *  $Date:   Feb 19 2008 08:35:30  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Message_Version2/res/MessageServerAdapter.cpp.-arc  $
   
      Rev 1.29   Feb 19 2008 08:35:30   Eddy Guo
   update to fix '!' at contact field (sender)
   
      Rev 1.28   Feb 15 2008 09:43:24   Eddy Guo
   fix switch view bug
   
      Rev 1.27   Jan 23 2008 17:17:52   Eddy Guo
   update for standard at command
   
      Rev 1.26   Jan 16 2008 19:23:18   Eddy Guo
   remove warning
   
      Rev 1.25   Jan 16 2008 10:55:16   Eddy Guo
   update progress bar
   
      Rev 1.24   Nov 22 2007 10:04:12   Eddy Guo
   update
   
      Rev 1.23   Nov 20 2007 17:16:36   Eddy Guo
   upadte
   
      Rev 1.22   Nov 19 2007 15:38:44   Jame Tsai
   replace image paths
   
      Rev 1.21   Nov 09 2007 13:02:42   Eddy Guo
   add header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file MessageServerAdapter.cpp
    @brief {brief description of MessageServerAdapter.cpp}

    {detail description of MessageServerAdapter.cpp}
*/
#include "StdAfx.h"
#include "MessageServerAdapter.h"
#include "../../CommLayer/MobileInterface/Interface_Include/ICommMobile.h"


#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

CMessageServerAdapter::CMessageServerAdapter() :
m_pLocalization(NULL),
m_pQListDataSource(NULL),
m_pQTreeDataSource(NULL),
m_pSMSServer(NULL),
m_pTreeCtrl(NULL),
m_pListCtrl(NULL),
m_nCurrentFolderID(0),
m_ATCommandSet(AT_CMD_UNKNOWN)
{
    GetATCommandSet();
}
 
CMessageServerAdapter::CMessageServerAdapter(ISMSServer* pSMSServer, IQListDataSource* pListDataSrc, IQTreeDataSource* pTreeDataSrc, CQPortingTreeUI* pTreeCtrl, CQListCtrl* pListCtrl):
m_pLocalization(NULL),
m_pSMSServer(pSMSServer),
m_pQListDataSource(pListDataSrc),
m_pQTreeDataSource(pTreeDataSrc),
m_pTreeCtrl(pTreeCtrl),
m_pListCtrl(pListCtrl),
m_nCurrentFolderID(0),
m_ATCommandSet(AT_CMD_UNKNOWN)
{
    DefineNotify();
    ASSERT(m_pSMSServer);
    ASSERT(m_pQListDataSource);
    ASSERT(m_pQTreeDataSource);
    if(m_pSMSServer == NULL || m_pQListDataSource == NULL || m_pQTreeDataSource == NULL) {
        AfxMessageBox(_T("Error in Message Server Adapter!!!"));
        ASSERT(false);
    }
    //AddInstance(pSMSServer, pListDataSrc, pTreeDataSrc);
    CreateILocalization(&m_pLocalization);
 
    m_pSMSServer->Register(this, EVENT_SMSSERVER_READ_MESSAGE    );
    m_pSMSServer->Register(this, EVENT_SMSSERVER_DELETE_MESSAGE  );
    m_pSMSServer->Register(this, EVENT_SMSSERVER_SAVE_MESSAGE    );
    m_pSMSServer->Register(this, EVENT_SMSSERVER_LIST_ALL_MESSAGE);
    m_pSMSServer->Register(this, EVENT_SMSSERVER_QUERY_SCA       );
    m_pSMSServer->Register(this, EVENT_SMSSERVER_SEND_MESSAGE    );
    m_pSMSServer->Register(this, EVENT_SMSSERVER_BACKUP_MESSAGE  );
    m_pSMSServer->Register(this, EVENT_SMSSERVER_RESTORE_MESSAGE );
    m_pSMSServer->Register(this, EVENT_SMSSERVER_QUERY_FOLDER    );
 
    m_pSMSServer->Register(this, SMSSERVER_LIST_FOLDER_OK);
    m_pSMSServer->Register(this, SMSSERVER_LIST_ONE_FOLDER  );
    m_pSMSServer->Register(this, SMSSERVER_READ_ONE_MESSAGE  );
    m_pSMSServer->Register(this, SMSSERVER_DELETE_ONE_MESSAGE);
    m_pSMSServer->Register(this, SMSSERVER_WRITE_ONE_MESSAGE );
    m_pSMSServer->Register(this, SMSSERVER_SAVE_ONE_MESSAGE  );
    m_pSMSServer->Register(this, SMSSERVER_GET_MESSAGE_BY_LOC);
    m_pSMSServer->Register(this, SMSSERVER_UPDATE_TREE_CONTROL);
    m_pSMSServer->Register(this, SMSSERVER_UPDATE_LIST_CONTROL);
 
    //m_pQListDataSource->Register(this, EVENT_LIST_DATA_DELETE_NODE  );
    //m_pQListDataSource->Register(this, EVENT_LIST_DATA_INSERT_NODE  );
    //m_pQListDataSource->Register(this, EVENT_LIST_DATA_UPDATE_NODE  );
    //m_pQListDataSource->Register(this, EVENT_LIST_COLUMN_DELETE_NODE);
    //m_pQListDataSource->Register(this, EVENT_LIST_COLUMN_INSERT_NODE);
    //m_pQListDataSource->Register(this, EVENT_LIST_COLUMN_UPDATE_NODE);
    //m_pQListDataSource->Register(this, EVENT_LIST_COLUMN_SORTOK     );
 
    //m_pQTreeDataSource->Register(this, EVENT_TREE_DATA_DELETE_NODE);
    //m_pQTreeDataSource->Register(this, EVENT_TREE_DATA_INSERT_NODE);
    //m_pQTreeDataSource->Register(this, EVENT_TREE_DATA_UPDATE_NODE);
    GetATCommandSet();
}
 
CMessageServerAdapter::~CMessageServerAdapter()
{
 
    m_pSMSServer->Unregister(this, EVENT_SMSSERVER_READ_MESSAGE    );
    m_pSMSServer->Unregister(this, EVENT_SMSSERVER_DELETE_MESSAGE  );
    m_pSMSServer->Unregister(this, EVENT_SMSSERVER_SAVE_MESSAGE    );
    m_pSMSServer->Unregister(this, EVENT_SMSSERVER_LIST_ALL_MESSAGE);
    m_pSMSServer->Unregister(this, EVENT_SMSSERVER_QUERY_SCA       );
    m_pSMSServer->Unregister(this, EVENT_SMSSERVER_SEND_MESSAGE    );
    m_pSMSServer->Unregister(this, EVENT_SMSSERVER_BACKUP_MESSAGE  );
    m_pSMSServer->Unregister(this, EVENT_SMSSERVER_RESTORE_MESSAGE );
    m_pSMSServer->Unregister(this, EVENT_SMSSERVER_QUERY_FOLDER    );
 
    m_pSMSServer->Unregister(this, SMSSERVER_LIST_FOLDER_OK);
    m_pSMSServer->Unregister(this, SMSSERVER_LIST_ONE_FOLDER  );
    m_pSMSServer->Unregister(this, SMSSERVER_READ_ONE_MESSAGE  );
    m_pSMSServer->Unregister(this, SMSSERVER_DELETE_ONE_MESSAGE);
    m_pSMSServer->Unregister(this, SMSSERVER_WRITE_ONE_MESSAGE );
    m_pSMSServer->Unregister(this, SMSSERVER_SAVE_ONE_MESSAGE  );
    m_pSMSServer->Unregister(this, SMSSERVER_GET_MESSAGE_BY_LOC);
    m_pSMSServer->Unregister(this, SMSSERVER_UPDATE_TREE_CONTROL);
    m_pSMSServer->Unregister(this, SMSSERVER_UPDATE_LIST_CONTROL);
 
    //m_pQListDataSource->Unregister(this, EVENT_LIST_DATA_DELETE_NODE  );
    //m_pQListDataSource->Unregister(this, EVENT_LIST_DATA_INSERT_NODE  );
    //m_pQListDataSource->Unregister(this, EVENT_LIST_DATA_UPDATE_NODE  );
    //m_pQListDataSource->Unregister(this, EVENT_LIST_COLUMN_DELETE_NODE);
    //m_pQListDataSource->Unregister(this, EVENT_LIST_COLUMN_INSERT_NODE);
    //m_pQListDataSource->Unregister(this, EVENT_LIST_COLUMN_UPDATE_NODE);
    //m_pQListDataSource->Unregister(this, EVENT_LIST_COLUMN_SORTOK     );
 
    //m_pQTreeDataSource->Unregister(this, EVENT_TREE_DATA_DELETE_NODE);
    //m_pQTreeDataSource->Unregister(this, EVENT_TREE_DATA_INSERT_NODE);
    //m_pQTreeDataSource->Unregister(this, EVENT_TREE_DATA_UPDATE_NODE);
    //TRACE("Message Server Adapter Destructor...\n");
}
 
void CMessageServerAdapter::DefineNotify() {
    CINotifyBase::DefineNotify(EVENT_SMSSERVER_READ_MESSAGE    );
    CINotifyBase::DefineNotify(EVENT_SMSSERVER_DELETE_MESSAGE  );
    CINotifyBase::DefineNotify(EVENT_SMSSERVER_SAVE_MESSAGE    );
    CINotifyBase::DefineNotify(EVENT_SMSSERVER_LIST_ALL_MESSAGE);
    CINotifyBase::DefineNotify(EVENT_SMSSERVER_QUERY_SCA       );
    CINotifyBase::DefineNotify(EVENT_SMSSERVER_SEND_MESSAGE    );
    CINotifyBase::DefineNotify(EVENT_SMSSERVER_BACKUP_MESSAGE  );
    CINotifyBase::DefineNotify(EVENT_SMSSERVER_RESTORE_MESSAGE );
    CINotifyBase::DefineNotify(EVENT_SMSSERVER_QUERY_FOLDER    );
 
    CINotifyBase::DefineNotify(SMSSERVER_LIST_FOLDER_OK    );
    CINotifyBase::DefineNotify(SMSSERVER_LIST_ONE_FOLDER   );
    CINotifyBase::DefineNotify(SMSSERVER_READ_ONE_MESSAGE  );
    CINotifyBase::DefineNotify(SMSSERVER_DELETE_ONE_MESSAGE);
    CINotifyBase::DefineNotify(SMSSERVER_WRITE_ONE_MESSAGE );
    CINotifyBase::DefineNotify(SMSSERVER_SAVE_ONE_MESSAGE  );
 
}
 
void CMessageServerAdapter::AddInstance(ISMSServer* pISMSServer, IQListDataSource* pIListDataSrc, IQTreeDataSource* pITreeDataSrc) {
    this->m_pSMSServer = pISMSServer;
    this->m_pQListDataSource = pIListDataSrc;
    this->m_pQTreeDataSource = pITreeDataSrc;
}
 
void CMessageServerAdapter::ConvertCShortMessageToListData(CShortMessage* pCSM, CQListData& listData) {
    CString strIDList;
    for(UINT i = 0; i < pCSM->m_vecMessageID.size(); ++i) {
        wchar_t cID[20] = {0};
        _i64tow_s(pCSM->m_vecMessageID.at(i), cID, 20, 10);
        strIDList += cID;
        strIDList += _T(",");
    }
    CBasicData ID;
    ID.m_strText = strIDList.TrimRight(_T(","));;
    listData.SetData(_T("Index"), ID);
 
    CBasicData state;
    if(pCSM->m_bIsRead == false) {
        state.m_strImagePath = pCSM->m_storage == STORAGE_SIM ? L"Message\\List\\List_SIMUnread.PNG" : L"Message\\List\\List_Unread.PNG";
    } else {
        state.m_strImagePath = pCSM->m_storage == STORAGE_SIM ? L"Message\\List\\List_SIMRead.PNG" : L"Message\\List\\List_Read.PNG";
    }
    if(pCSM->m_nMessageLocation > 1) {
        pCSM->m_bIsRead = true;
        state.m_strImagePath = L"Message\\List\\List_Read.PNG";
    }

    listData.SetData(_T("State"), state);
 
    CBasicData time;
    time.m_strText = pCSM->m_strDateTime;
    listData.SetData(_T("Time"), time);
 
    CBasicData sender;
    if(pCSM->m_strPhoneNumber != _T("!"))
        sender.m_strText += pCSM->m_strPhoneNumber;
    else
        sender.m_strText = _T("");
    listData.SetData(_T("Sender"), sender);
 
    CBasicData message;
    message.m_strText = pCSM->m_strMessage;
    listData.SetData(_T("Message"), message);
 
    CBasicData location;
    CString msgLocation;
    if(m_ATCommandSet == AT_CMD_STANDARD && (pCSM->m_nMessageLocation == 0 || pCSM->m_nMessageLocation == 1)) {
        msgLocation.Format(_T("%d"), 0);
    } else if(m_ATCommandSet == AT_CMD_NEW_BENQ) {
        msgLocation.Format(_T("%d"), pCSM->m_nMessageLocation);
    }
    location.m_strText = msgLocation;
    listData.SetData(_T("Location"), location);

    CBasicData hideState;
    CString strHideState = pCSM->m_bIsRead? _T("read") : _T("unread");
    hideState.m_strText = strHideState;
    listData.SetData(_T("HideState"), hideState);

    CBasicData hideStorage;
    CString strHideStorage = (pCSM->m_storage == STORAGE_SIM) ? _T("SM") : _T("ME");
    hideStorage.m_strText = strHideStorage;
    listData.SetData(_T("HideStorage"), hideStorage);

}
 
void CMessageServerAdapter::OnEventSMSServerSaveMessage(long nParam) {
    CShortMessage* param = (CShortMessage*)nParam;
    if(param->m_nMessageLocation == this->m_nCurrentFolderID) {
        CQListData* cld = new CQListData();
        ConvertCShortMessageToListData(param, *cld);
        m_pQListDataSource->InsertDataByIndex(cld);
    }
}
 
void CMessageServerAdapter::OnEventSMSServerReadMessage(long nParam)
{
    m_pListCtrl->DeleteAllItems();
    HTREEITEM item = m_pTreeCtrl->GetSelectedItem();
    if(item == NULL) return;
    vector<CShortMessage>* pVecCSM = reinterpret_cast<vector<CShortMessage>*>(nParam);
    if(m_ATCommandSet == AT_CMD_NEW_BENQ) {
        for(UINT i = 0; i < pVecCSM->size(); ++i) {
            if(m_nCurrentFolderID != pVecCSM->at(i).m_nMessageLocation) continue;
            CQListData* cld = new CQListData();
            ConvertCShortMessageToListData(&(pVecCSM->at(i)), *cld);
            m_pQListDataSource->InsertDataByIndex(cld);
        }
    } else if(m_ATCommandSet == AT_CMD_STANDARD) {
        if(m_nCurrentFolderID == 0 || m_nCurrentFolderID == 1) {
            for(UINT i = 0; i < pVecCSM->size(); ++i) {
                if(pVecCSM->at(i).m_nMessageLocation == 0 || pVecCSM->at(i).m_nMessageLocation == 1) {
                    CQListData* cld = new CQListData();
                    ConvertCShortMessageToListData(&(pVecCSM->at(i)), *cld);
                    m_pQListDataSource->InsertDataByIndex(cld);
                }
            }
        } else {
            for(UINT i = 0; i < pVecCSM->size(); ++i) {
                if(m_nCurrentFolderID != pVecCSM->at(i).m_nMessageLocation) continue;
                CQListData* cld = new CQListData();
                ConvertCShortMessageToListData(&(pVecCSM->at(i)), *cld);
                m_pQListDataSource->InsertDataByIndex(cld);
            }
        }
    }
}
 
void CMessageServerAdapter::UpdateTreeNodeString(LPCTSTR strIndexName, LPCTSTR strLocalizationID, CString& strCount) {
    CQTreeNode* node = (CQTreeNode*)m_pQTreeDataSource->GetRoot()->FindChildByName(strIndexName);
    if(node) {
        node->SetDisplayName(GetString(strLocalizationID) + strCount);
        m_pQTreeDataSource->UpdateNode(node);
    }
}
 
void CMessageServerAdapter::UpdateMessageCountOnTreeCtrl(int nUnread, int nInbox, int nDraft, int nOutbox, int nSent)
{
    CQTreeNode* rootNode = m_pQTreeDataSource->GetRoot();
    CString strCount;
    if(rootNode) {
        strCount.Format(_T(" (%d/%d)"), nUnread, nInbox);
        UpdateTreeNodeString(_T("INBOX"), _T("IDS_INBOX"), strCount);
        strCount.Format(_T(" (%d)"), nDraft);
        if(m_ATCommandSet == AT_CMD_NEW_BENQ) { // New BenQ will separate the draft and outbox folder
            UpdateTreeNodeString(_T("DRAFT"), _T("IDS_DRAFT"), strCount);
            strCount.Format(_T(" (%d)"), nOutbox);
            UpdateTreeNodeString(_T("OUTBOX"), _T("IDS_OUTBOX"), strCount);
        } else if(m_ATCommandSet == AT_CMD_STANDARD) {  // standard command won't
            CString strDisplay  = m_pLocalization->GetString(_T("IDS_DRAFT")).c_str() + CString(_T(" & ")) + 
                CString(m_pLocalization->GetString(_T("IDS_OUTBOX")).c_str());
            UpdateTreeNodeString(_T("DRAFT & OUTBOX"), strDisplay, strCount);
        }
        strCount.Format(_T(" (%d)"), nSent);
        UpdateTreeNodeString(_T("SENT"), _T("IDS_SENT"), strCount);
        m_pQTreeDataSource->ExpandNode(rootNode);
    }
}
 
void CMessageServerAdapter::OnEventUpdateTreeControl(long nParam)
{
    if(m_ATCommandSet == AT_CMD_NEW_BENQ) {
        vector<CShortMessage>* vecSM = (vector<CShortMessage>*)nParam;
        int nUnread = 0, nInbox = 0, nDraft = 0, nOutbox = 0, nSent = 0;
        for(UINT i = 0; i < vecSM->size(); ++i) {
            switch(vecSM->at(i).m_nMessageLocation) {
            case 1:
                nInbox++;
                if(vecSM->at(i).m_bIsRead == false) nUnread++;
                break;
            case 2:
                nDraft++;
                break;
            case 3:
                nOutbox++;
                break;
            case 4:
                nSent++;
                break;
            default:
                ASSERT(false);
                break;
            }
        }
        UpdateMessageCountOnTreeCtrl(nUnread, nInbox, nDraft, nOutbox, nSent);
    } else if(m_ATCommandSet == AT_CMD_STANDARD) {
        vector<CShortMessage>* vecSM = (vector<CShortMessage>*)nParam;
        int nUnread = 0, nRead = 0, nUnsend = 0, nSent = 0;
        for(UINT i = 0; i < vecSM->size(); ++i) {
            switch(vecSM->at(i).m_nMessageLocation) {
            case 0:
                nUnread++;
                break;
            case 1:
                nRead++;
                break;
            case 2:
                nUnsend++;
                break;
            case 3:
                nSent++;
                break;
            default:
                ASSERT(false);
                break;
            }
        }
        UpdateMessageCountOnTreeCtrl(nUnread, nUnread+nRead, nUnsend, nSent, nSent);
    }
}
 
void CMessageServerAdapter::OnEventListFolderOK(long nParam)
{
    vector<CMessageFolderInfo>* vecFolder = (vector<CMessageFolderInfo>*)nParam;
    m_pQTreeDataSource->DeleteNode(m_pQTreeDataSource->GetRoot());
    CMessageFolderNode* rootNode = new CMessageFolderNode(NULL);
    rootNode->m_strSelectedImagePath = rootNode->m_strImagePath = L"Message\\Tree\\Tree_SMS.png";
    rootNode->SetData(_T("IDS_MESSAGE_GROUP"), GetString(_T("IDS_MESSAGE_GROUP")), 0);
    m_pQTreeDataSource->CreateNode(rootNode);
    for(size_t i = 0; i < vecFolder->size(); ++i) {
        CMessageFolderNode* childNode = new CMessageFolderNode(rootNode);
        CString strDisplayName;
        if(vecFolder->at(i).strDisplayName == _T("INBOX") || vecFolder->at(i).strDisplayName == _T("OUTBOX") ||
            vecFolder->at(i).strDisplayName == _T("DRAFT") || vecFolder->at(i).strDisplayName == _T("SENT")) {
            strDisplayName = m_pLocalization->GetString((LPCTSTR)(_T("IDS_") + vecFolder->at(i).strDisplayName)).c_str();
            if(vecFolder->at(i).strDisplayName == _T("INBOX")) {
                childNode->m_strSelectedImagePath = childNode->m_strImagePath = L"Message\\Tree\\Tree_Inbox.png";
            } else if(vecFolder->at(i).strDisplayName == _T("OUTBOX")) {
                childNode->m_strSelectedImagePath = childNode->m_strImagePath = L"Message\\Tree\\Tree_Outbox.png";
            } else if(vecFolder->at(i).strDisplayName == _T("DRAFT")) {
                childNode->m_strSelectedImagePath = childNode->m_strImagePath = L"Message\\Tree\\Tree_Draft.png";
            } else if(vecFolder->at(i).strDisplayName == _T("SENT")) {
                childNode->m_strSelectedImagePath = childNode->m_strImagePath = L"Message\\Tree\\Tree_Sent.png";
            }
        } else {
            if(vecFolder->at(i).strDisplayName == _T("DRAFT & OUTBOX"))
                strDisplayName = m_pLocalization->GetString(_T("IDS_DRAFT")).c_str() + CString(_T(" & ")) + 
                    CString(m_pLocalization->GetString(_T("IDS_OUTBOX")).c_str());
            childNode->m_strSelectedImagePath = childNode->m_strImagePath = L"Message\\Tree\\Tree_Draft.png";
        }
        childNode->SetData(vecFolder->at(i).strDisplayName, strDisplayName, vecFolder->at(i).nID);
        m_pQTreeDataSource->CreateNode(childNode);
    }
    m_pQTreeDataSource->UpdateNode(rootNode);
}
 
void CMessageServerAdapter::Event(const TSTRING& strEvent,long nParam) {
    if(strEvent == EVENT_SMSSERVER_READ_MESSAGE) {
        // Server讀入一筆資料, 也要在Data Source insert一筆
        // 資料可能要做轉換才能insert到Data Source
        OnEventSMSServerReadMessage(nParam);
    } else if(strEvent == EVENT_SMSSERVER_DELETE_MESSAGE  ) {
        // Server刪除一筆資料, 也要在Data Source delete一筆
        m_pQListDataSource->DeleteDataByIndex(nParam);
    } else if(strEvent == EVENT_SMSSERVER_SAVE_MESSAGE    ) {
        // 資料可能要做轉換才能insert到Data Source
        // Server存入一筆資料, 也要在Data Source insert一筆
        OnEventSMSServerSaveMessage(nParam);
    } else if(strEvent == EVENT_SMSSERVER_LIST_ALL_MESSAGE) {
        // 讀取全部的message, 所以要一筆一筆寫入data source
        // 必須再建立一個vec來存放所有的message, 由server傳來的就必須存在裡頭
        // Server存入一筆資料, 也要在Data Source insert一筆
    } else if(strEvent == EVENT_SMSSERVER_QUERY_SCA       ) {
        // server讀出SCA, 傳給別人, 目前先不會用到
    } else if(strEvent == EVENT_SMSSERVER_SEND_MESSAGE    ) {
        // server傳送出一筆資料, 傳完之後, 要insert到Data Source裡頭
        // 注意回傳的ID
    } else if(strEvent == EVENT_SMSSERVER_BACKUP_MESSAGE  ) {
        //
    } else if(strEvent == EVENT_SMSSERVER_RESTORE_MESSAGE ) {
        //
    } else if(strEvent == EVENT_SMSSERVER_QUERY_FOLDER    ) {
        //
    } else if(strEvent == EVENT_LIST_DATA_DELETE_NODE  ) {
        //
        // m_pSMSServer->DeleteMessageByID(nParam);
    } else if(strEvent == EVENT_LIST_DATA_INSERT_NODE  ) {
        //
    } else if(strEvent == EVENT_LIST_DATA_UPDATE_NODE  ) {
        //
    } else if(strEvent == EVENT_LIST_COLUMN_DELETE_NODE) {
        //
    } else if(strEvent == EVENT_LIST_COLUMN_INSERT_NODE) {
        //
    } else if(strEvent == EVENT_LIST_COLUMN_UPDATE_NODE) {
        //
    } else if(strEvent == EVENT_LIST_COLUMN_SORTOK     ) {
        //
    } else if(strEvent == SMSSERVER_LIST_ONE_FOLDER  ) {
    } else if(strEvent == SMSSERVER_LIST_FOLDER_OK   ) {
        OnEventListFolderOK(nParam);
        //Fire(SMSSERVER_LIST_FOLDER_OK, nParam);
    } else if(strEvent == SMSSERVER_READ_ONE_MESSAGE  ) {
        Fire(SMSSERVER_READ_ONE_MESSAGE);
    } else if(strEvent == SMSSERVER_DELETE_ONE_MESSAGE) {
    } else if(strEvent == SMSSERVER_WRITE_ONE_MESSAGE ) {
    } else if(strEvent == SMSSERVER_SAVE_ONE_MESSAGE  ) {
    } else if(strEvent == SMSSERVER_GET_MESSAGE_BY_LOC  ) {
        OnEventSMSServerReadMessage(nParam);
    } else if(strEvent == SMSSERVER_UPDATE_TREE_CONTROL) {
        OnEventUpdateTreeControl(nParam);
    } else if(strEvent == SMSSERVER_UPDATE_LIST_CONTROL) {
    } else {
        ASSERT(false);
    }
}

enumAtCmdSet CMessageServerAdapter::GetATCommandSet() {
    CQMobileConfigure* pConfigure = NULL;
    //TRACE("Message Server Adapter Constructor...\n");
    if(m_ATCommandSet == AT_CMD_UNKNOWN) {
        if(pConfigure == NULL)
            pConfigure = GetMobileConFig();
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
    }
    return m_ATCommandSet;
}


#pragma warning(pop)