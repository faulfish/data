/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
*
*                           QSyncher V2 Project
*
*	             Copyright (c) 2005 BENQ Corpration
*
***************************************************************************
*
*  $Workfile:   Message_Version2View.cpp  $
*  $Revision:   1.83  $
*  $Date:   Mar 27 2008 16:34:20  $
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
* $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Message_Version2/Message_Version2View.cpp.-arc  $
   
      Rev 1.83   Mar 27 2008 16:34:20   Alan Tu
   update
   
      Rev 1.82   Mar 27 2008 15:26:56   Alan Tu
   Auto close when disconnect
   
      Rev 1.81   Mar 27 2008 15:07:10   Alan Tu
   Clear text when disconnect
   
      Rev 1.80   Mar 24 2008 14:23:52   Alan Tu
   safe time for check events
   
      Rev 1.79   Mar 24 2008 14:20:20   Alan Tu
   check self item when drag
   
      Rev 1.78   Mar 17 2008 22:35:24   Alan Tu
   update
   
      Rev 1.77   Mar 17 2008 21:15:38   Alan Tu
   remove setUI
   
      Rev 1.76   Mar 03 2008 20:51:12   Jame Tsai
   change button enable policy
   
      Rev 1.75   Feb 27 2008 20:03:58   Jame Tsai
   Add UIXML and use it
   
      Rev 1.74   Feb 26 2008 14:22:38   Alan Tu
   remove garbage
   
      Rev 1.73   Feb 26 2008 13:59:10   Alan Tu
   update for message
   
      Rev 1.72   Feb 26 2008 10:54:56   Alan Tu
   update for disconnect issue
*
*
***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file Message_Version2View.cpp
@brief {brief description of Message_Version2View.cpp}

{detail description of Message_Version2View.cpp}
*/
// Message_Version2View.cpp : implementation of the CMessage_Version2View class
//
#include "stdafx.h"
#include "Message_Version2View.h"
#include "../../Components/UIFramework/interface/ShowMessage.h"
#include "../../Components/UIFramework/CreateRgnFromFile.h"
#include "../Phonebook_version2/PhonebookExport.h"
#include "..\..\CommonUtil\Win32ThreadUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
extern class NotifyReceiver g_NotifyReceiver;
#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
// CMessage_Version2View
IMPLEMENT_DYNCREATE(CMessage_Version2View, CFormView)
BEGIN_MESSAGE_MAP(CMessage_Version2View, Win32DoubleBufferFormViewWidge)
    ON_NOTIFY(HDN_ENDTRACK, 0, &CMessage_Version2View::OnHdnEndtrackList)
    ON_NOTIFY(HDN_DIVIDERDBLCLICK, 0, &CMessage_Version2View::OnHdnDividerDBLClickList)
    ON_MESSAGE(WM_DELETE_SUCCESS  , &CMessage_Version2View::OnThreadDeleteMessage)
    ON_MESSAGE(WM_DELETE_FAILED   , &CMessage_Version2View::OnThreadDeleteMessage)
    ON_MESSAGE(WM_FORWARD_SUCCESS , &CMessage_Version2View::OnThreadForwardMessage)
    ON_MESSAGE(WM_FORWARD_FAILED  , &CMessage_Version2View::OnThreadForwardMessage)
    ON_MESSAGE(WM_SEND_SUCCESS    , &CMessage_Version2View::OnThreadSendMessage)
    ON_MESSAGE(WM_SEND_FAILED     , &CMessage_Version2View::OnThreadSendMessage)
    ON_MESSAGE(WM_SAVE_SUCCESS    , &CMessage_Version2View::OnThreadSaveMessage)
    ON_MESSAGE(WM_SAVE_FAILED     , &CMessage_Version2View::OnThreadSaveMessage)
    ON_MESSAGE(WM_BACKUP_SUCCESS  , &CMessage_Version2View::OnThreadBakupMessage)
    ON_MESSAGE(WM_BACKUP_FAILED   , &CMessage_Version2View::OnThreadBakupMessage)
    ON_MESSAGE(WM_RESTORE_SUCCESS , &CMessage_Version2View::OnThreadRestoreMessage)
    ON_MESSAGE(WM_RESTORE_FAILED  , &CMessage_Version2View::OnThreadRestoreMessage)
    ON_MESSAGE(WM_REFRESH_SUCCESS , &CMessage_Version2View::OnThreadRefreshMessage)
    ON_MESSAGE(WM_REFRESH_FAILED  , &CMessage_Version2View::OnThreadRefreshMessage)
END_MESSAGE_MAP()

CMessage_Version2View* g_pThis = NULL;

CMessage_Version2View::CMessage_Version2View()
: Win32DoubleBufferFormViewWidge(CMessage_Version2View::IDD),
m_bIsConnected(false),
m_bIsSynced(false),
m_pProgressThread(NULL)
,m_bIsShow(false)
,m_btnNew(NULL)    
,m_btnForward(NULL)
,m_btnReply(NULL)
,m_btnSend(NULL)   
,m_btnDelete(NULL) 
,m_btnRefresh(NULL)
,m_bIsDragging(false)
,m_bIsSelfDragging(false)
{
	g_pThis = this;
    g_NotifyReceiver.m_pINotify = this;
    m_pCfgTool = CreateIConfigure(_T("MessageViewVersion2"));
    m_pCfgTool->Load(STORAGE_INIFILE);
    srand((unsigned)time(NULL));
}

CMessage_Version2View::~CMessage_Version2View() {
	if(m_pListCtrlLogic)
		delete m_pListCtrlLogic;
	if(m_pTreeCtrlLogic)
		delete m_pTreeCtrlLogic;
	if(m_pServerAdapter)
		delete m_pServerAdapter;
    m_pCfgTool->Save(STORAGE_INIFILE);
    DeleteIConfigure(_T("MessageViewVersion2"));
}

void CMessage_Version2View::OnInitialUpdate() {
    CFormView::OnInitialUpdate();
    ResizeParentToFit();
    Init();
    InsertColumn();
    InsertTreeNode();
    RefreshUILocalization();
    m_SubmenuBase.SetSubMenu(&m_MessageSubMenu);
    m_cntEdit.SetBackColor(255, 255, 255);
    for(int i = 0; i < m_MessageSubMenu.GetMenuItemCount(); i++) {
        TSTRING strDisplay, strNotifyID;
        m_MessageSubMenu.GetMenuItem(i,strDisplay, strNotifyID);
        m_SubmenuBase.Register(this, strNotifyID);
    }
	SetXMLUI();
    UpdateButtonStatus(false, false, false, false, false,/* false, false,*/ false);    
}

void CMessage_Version2View::DoDataExchange(CDataExchange* pDX) {
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CONTENT_EDIT, m_cntEdit);
    DDX_Control(pDX, IDC_MESSAGE_FOLDER_TREE, m_folderTreeCtrl);
    DDX_Control(pDX, IDC_MESSAGE_LIST, m_msgListCtrl);
}

FORCEINLINE void CMessage_Version2View::Init() {
    CreateILocalization(&m_pLocalization);
    ASSERT(&m_SMSServer);
    ASSERT(&m_MsgDataSrc);
    ASSERT(&m_MsgTreeSrc);
    ASSERT(m_pLocalization);

    m_pLocalization->Register(this, TNotify_Language_Change);

    m_pListCtrlLogic = new CQCommListCtrl(&m_MsgDataSrc, &m_msgListCtrl);
    m_pListCtrlLogic->Register(this, EVENT_LIST_UI_LBTNDOWN);
    m_pListCtrlLogic->Register(this, EVENT_LIST_UI_RBTNDOWN);
    m_pListCtrlLogic->Register(this, EVENT_LIST_UI_LDBCLICK);
    m_pListCtrlLogic->Register(this, EVENT_LIST_UI_ITEMCHANGED);
    m_pListCtrlLogic->Register(this, EVENT_LIST_UI_DRAG_BEGIN);
    m_pListCtrlLogic->Register(this, EVENT_LIST_UI_DROP);
	m_pListCtrlLogic->Register(this, EVENT_DRAG_END);
	m_pListCtrlLogic->Register(this, EVENT_LIST_UI_DRAG_OVER);	
	m_pListCtrlLogic->Register(this, EVENT_LIST_UI_DRAG_ENTER);	
	m_pListCtrlLogic->Register(this, EVENT_LIST_UI_DRAG_LEAVE);	

    m_pListCtrlLogic->Register(this, EVENT_LIST_UI_ONSIZE);
    m_pListCtrlLogic->Register(this, EVENT_LIST_UI_ONSIZING);

    m_pListCtrlLogic->Register(&m_MsgDataSrc, EVENT_LIST_UI_HEADER_BUTTON_HIT);

    m_pTreeCtrlLogic = new CQCommonTreeCtrl(&m_folderTreeCtrl, &m_MsgTreeSrc);
    m_pTreeCtrlLogic->Register(this, EVENT_TREE_SELECT_NODE);

    m_pServerAdapter = new CMessageServerAdapter(&m_SMSServer, &m_MsgDataSrc, &m_MsgTreeSrc, &m_folderTreeCtrl, &m_msgListCtrl);
    m_pServerAdapter->Register(this, SMSSERVER_LIST_ONE_FOLDER   );
    m_pServerAdapter->Register(this, SMSSERVER_LIST_FOLDER_OK    );
    m_pServerAdapter->Register(this, SMSSERVER_READ_ONE_MESSAGE  );
    m_pServerAdapter->Register(this, SMSSERVER_DELETE_ONE_MESSAGE);
    m_pServerAdapter->Register(this, SMSSERVER_WRITE_ONE_MESSAGE );
    m_pServerAdapter->Register(this, SMSSERVER_SAVE_ONE_MESSAGE  );

    m_SMSServer.Register(this, SMSSERVER_BEGIN_PROGRESS_BAR);
    m_SMSServer.Register(this, SMSSERVER_END_PROGRESS_BAR  );
    m_SMSServer.Register(this, SMSSERVER_BACKUP_ONE_MESSAGE);
    m_SMSServer.Register(this, SMSSERVER_RESTORE_ONE_MESSAGE);
    m_SMSServer.Register(this, SMSSERVER_DELETE_ONE_MESSAGE);
    m_SMSServer.Register(this, SMSSERVER_SEND_ONE_PAGE_MSG);
}
void CMessage_Version2View::SetXMLUI()
{
	std::wstring strBkImage = L"MainFrame\\Bk_GeneralView.png";
	SetBKImage(strBkImage,BK_Fixed);

	CLayoutManager	SMSLayoutManager(this, this);
	Widge<HDC>* pWidget = SMSLayoutManager.Load(_T("Message_ViewID"));
	//m_pButtonOK = (WIN32ButtonWidge *)pLayoutManager->GetWidgetByID(_T(""));
	m_btnNew = (WIN32ButtonWidge *)SMSLayoutManager.GetWidgetByID(_T("Win32Button_0"));    
	m_btnForward = (WIN32ButtonWidge *)SMSLayoutManager.GetWidgetByID(_T("Win32Button_1"));
	m_btnReply = (WIN32ButtonWidge *)SMSLayoutManager.GetWidgetByID(_T("Win32Button_2"));  
	m_btnSend = (WIN32ButtonWidge *)SMSLayoutManager.GetWidgetByID(_T("Win32Button_3"));   
	m_btnDelete = (WIN32ButtonWidge *)SMSLayoutManager.GetWidgetByID(_T("Win32Button_4")); 
	m_btnRefresh = (WIN32ButtonWidge *)SMSLayoutManager.GetWidgetByID(_T("Win32Button_5"));
	SetLayout(pWidget);
}

// reload all windows control text
void CMessage_Version2View::RefreshUILocalization() {
    //--List設定
    m_MsgDataSrc.UpdateColumnByKey(_T("State"), GetString(_T("IDS_Message_List_State")));
    m_MsgDataSrc.UpdateColumnByKey(_T("Time"), GetString(_T("IDS_Message_List_DateTime")));
    m_MsgDataSrc.UpdateColumnByKey(_T("Sender"), GetString(_T("IDS_Message_List_Sender")));
    // Notice, this is set the column width
    m_msgListCtrl.SetColumnWidth(0, m_pCfgTool->GetCfgVar(_T("State"), 100));
    m_msgListCtrl.SetColumnWidth(1, m_pCfgTool->GetCfgVar(_T("Sender"), 140));
    m_msgListCtrl.SetColumnWidth(2, m_pCfgTool->GetCfgVar(_T("Time"), 140));

    //--Tree設定
    CTreeCtrl* pTreeCtrl = &m_folderTreeCtrl;
    HTREEITEM root = pTreeCtrl->GetRootItem();
    pTreeCtrl->SetItemText(root, GetString(_T("IDS_MESSAGE_GROUP")));

    m_pServerAdapter->OnEventUpdateTreeControl((long)&(m_SMSServer.m_vecShortMessage));
    pTreeCtrl->Expand(root, TVE_EXPAND);
}

// Insert columns to list control UI
FORCEINLINE void CMessage_Version2View::InsertColumn()  {
    m_msgListCtrl.CListCtrl::SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

    //m_MsgDataSrc.InsertColumnKeyAndName(_T("Index"), _T(""), m_pCfgTool->GetCfgVar(_T("Index"), 100));
    m_MsgDataSrc.InsertColumnKeyAndName(_T("State"),_T(""), m_pCfgTool->GetCfgVar(_T("State"), 100));
    m_MsgDataSrc.InsertColumnKeyAndName(_T("Sender"),_T(""),m_pCfgTool->GetCfgVar(_T("Sender"), 140));
    m_MsgDataSrc.InsertColumnKeyAndName(_T("Time"),_T(""),m_pCfgTool->GetCfgVar(_T("Time"), 140));
    //m_MsgDataSrc.InsertColumnKeyAndName(_T("Location"),_T(""),m_pCfgTool->GetCfgVar(_T("Index"), 140));
}

// Insert tree node, notice that: folders must be asked
FORCEINLINE void CMessage_Version2View::InsertTreeNode() {

}

#ifdef _DEBUG
void CMessage_Version2View::AssertValid() const
{
    CFormView::AssertValid();
}

void CMessage_Version2View::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}
#endif //_DEBUG

// New Button Clicked
void CMessage_Version2View::OnMsgBtnNewClick() {
    //m_MsgBox.m_pPostMessageWnd = this;
    if(m_mapPB.empty()) GetPhonebook();
    CString strContactList, strContentText;
    bool bSaveToSim = false;
    INT_PTR nRetCode = m_MsgBox.DoModal(strContactList, strContentText, bSaveToSim, &m_mapPB);
    ProcessMessageEditorReturn(nRetCode, strContactList, strContentText);
}
void CMessage_Version2View::New()
{
	g_pThis->OnMsgBtnNewClick();
}
// Forward Button Clicked
void CMessage_Version2View::OnMsgBtnForwardClick() {
    if(m_mapPB.empty()) GetPhonebook();
    vector<FireListDataParameter> retVec;
    m_pListCtrlLogic->GetSelectedItem(retVec);
    if(retVec.size() == 0) return;
    FireListDataParameter &f = retVec.at(0);
    CString strContactList;
    CString strContentText = f.pListData->GetValueByKey(_T("Message"))->m_strText.c_str();
    bool bSaveToSim = false;
    INT_PTR nRetCode = m_MsgBox.DoModal(strContactList, strContentText, bSaveToSim, &m_mapPB);
    ProcessMessageEditorReturn(nRetCode, strContactList, strContentText);
}
void CMessage_Version2View::For()
{
	g_pThis->OnMsgBtnForwardClick();
}
// Reply Button Clicked
void CMessage_Version2View::OnMsgBtnReplyClick() {
    if(m_mapPB.empty()) GetPhonebook();
    vector<FireListDataParameter> retVec;
    this->m_pListCtrlLogic->GetSelectedItem(retVec);
    if(retVec.size() == 0) return;
    FireListDataParameter &f = retVec.at(0);
    CString strContactList = f.pListData->GetValueByKey(_T("Sender"))->m_strText.c_str();
    CString strContentText;
    bool bSaveToSim = false;
    INT_PTR nRetCode = m_MsgBox.DoModal(strContactList, strContentText, bSaveToSim, &m_mapPB);
    ProcessMessageEditorReturn(nRetCode, strContactList, strContentText);
}

void CMessage_Version2View::Rep()
{
	g_pThis->OnMsgBtnReplyClick();
}

// Send Button Clicked
void CMessage_Version2View::OnMsgBtnSendClick() {
    if(m_mapPB.empty()) GetPhonebook();
    vector<FireListDataParameter> retVec;
    this->m_pListCtrlLogic->GetSelectedItem(retVec);
    if(retVec.size() == 0) return;
    FireListDataParameter &f = retVec.at(0);
    CString strContactList = f.pListData->GetValueByKey(_T("Sender"))->m_strText.c_str();
    if(strContactList == _T("!"))
        strContactList.Empty();
    CString strContentText = f.pListData->GetValueByKey(_T("Message"))->m_strText.c_str();
    bool bSaveToSim = false;
    INT_PTR nRetCode = m_MsgBox.DoModal(strContactList, strContentText, bSaveToSim, &m_mapPB);
    ProcessMessageEditorReturn(nRetCode, strContactList, strContentText);
}
void CMessage_Version2View::Sen()
{
	g_pThis->OnMsgBtnSendClick();
}

void CMessage_Version2View::ProcessMessageEditorReturn(INT_PTR nRetCode, CString strReveivers, CString strContent, bool bSavetoSim) {
    switch(nRetCode) {
        case IDC_MSG_DLG_BTN_SEND:
            OnDlgSendMessage(strReveivers, strContent);
            break;
        case IDC_MSG_DLG_BTN_SAVE:
            OnDlgSaveMessage(strReveivers, strContent);
            break;
        case IDC_MSG_DLG_BTN_CANCEL:
            break;
        case 2:
            break;
        default:
            ASSERT(false);
            break;
    }
    m_SMSServer.m_pISMS = NULL;
    m_mapPB.clear();
}
// This function is processing that the editor dialog return send message.
// If send is completed, the messagebox is showing on thread.
// See SendMessageThread
void CMessage_Version2View::OnDlgSendMessage(CString strContactList, CString strContentText) {
    // 傳送出去, 之後還要再寫入sent或是outbox
    //TCHAR szContactList[1024] = {0};
    //_tcscpy(szContactList, (LPCTSTR)strContactList);
    SendThreadParam* pThParam = new SendThreadParam;
    pThParam->pServer = &m_SMSServer;
    pThParam->strContact = strContactList;
    pThParam->strContent = strContentText;
    pThParam->pThis = this;
    BeginProgressBar(100);
    AfxBeginThread(SendMessageThread, pThParam);
}

// This function is processing that the editor dialog return save message.
// If save is completed, the messagebox is showing on thread
// See SaveMessageThread
void CMessage_Version2View::OnDlgSaveMessage(CString strContactList, CString strContentText) {
    // 直接寫到draft, 如果strContactList是多個人的話, 只一存第一個人
    // 如果strContentText超過一個page, 則分開寫入手機
    WriteThreadParam* pThParam = new WriteThreadParam;  ///< delete in thread
    pThParam->pServer = &m_SMSServer;
    pThParam->strContact = strContactList;
    pThParam->strContent = strContentText;
    pThParam->pThis = this;
    AfxBeginThread(SaveMessageThread, pThParam);
}

// This function is processing that delete function.
// If delete is completed, the messagebox is showing on thread
// Before deleting messages, the comfirming message box is showing.
// After deleteing, the messagebox is showing in thread
// See the DeleteMessageThread
void CMessage_Version2View::OnMsgBtnDeleteClick() {
    // 記著現在所指到的哪一筆, 刪除前先詢問使用者
    vector<FireListDataParameter>* retVec = new vector<FireListDataParameter>;
    m_pListCtrlLogic->GetSelectedItem(*retVec);
    if(retVec->size() == 0) return;
    MESSAGERETURN mr = ShowMessage(this->GetSafeHwnd(), _T("IDS_Message_Delete_Prompt"), _T("IDS_MESSAGE_FUNCTION"), QMB_YESNO);
    if(mr == QIDYES) {
        DeleteThreadParam* pThParam = new DeleteThreadParam;    ///< delete in thread
        pThParam->pServer = &m_SMSServer;
        pThParam->pDataSrc = &m_MsgDataSrc;
        pThParam->pVecFDP = retVec;
        pThParam->pThis = this;
        BeginProgressBar((long)retVec->size());
        AfxBeginThread(DeleteMessageThread, pThParam);
        ClearEditText();
    } else {
        delete retVec;
    }
}

void CMessage_Version2View::Del() {
	g_pThis->OnMsgBtnDeleteClick();
}


void CMessage_Version2View::OnMsgBtnRefreshClick() {
    std::vector<UINT> vecFolderID;
    m_msgListCtrl.DeleteAllItems();
    m_MsgDataSrc.DeleteAllDataSource();
    RefreshThreadParam* rtp = new RefreshThreadParam;
    rtp->pServer = &m_SMSServer;
    rtp->pThis = this;
    BeginProgressBar(100);
	m_ProgressFinishEvent.ResetEvent();
    AfxBeginThread(RefreshMessageThread, rtp);

	//Due to the message will not occupate the window message so disable it.
	//QSYNC_LIB::MessageLoop(&m_ProgressFinishEvent.m_hObject,1);
	//ShowMessage(GetSafeHwnd(), SGetString(m_strMessage), _T("IDS_MESSAGE_FUNCTION")));
}

void CMessage_Version2View::Ref() {
	g_pThis->OnMsgBtnRefreshClick();
}

FORCEINLINE void CMessage_Version2View::OnEventListUILButtonDown(FireListDataParameter* pFdp) {
    if(pFdp->pListData == NULL) return;
    CString strColon(_T(": "));
    m_cntEdit.SetWindowText(pFdp->pListData->GetValueByKey(_T("Message"))->m_strText.c_str());
    m_MsgDataSrc.m_nCurrentMessageID = (UINT)_ttoi64(pFdp->pListData->GetValueByKey(_T("Index"))->m_strText.c_str());
}

FORCEINLINE void CMessage_Version2View::OnEventListUILButtonDoubleClick(FireListDataParameter* pFdp) {
    if(m_mapPB.empty()) GetPhonebook();
    m_MsgBox.m_pPostMessageWnd = this;
    CString strContactList = pFdp->pListData->GetValueByKey(_T("Sender"))->m_strText.c_str();
    CString strMessageID = pFdp->pListData->GetValueByKey(_T("Index"))->m_strText.c_str();
    CString strMessageState = pFdp->pListData->GetValueByKey(_T("HideState"))->m_strText.c_str();
    if(strMessageState == _T("unread")) {   ///< set the message from unread to read
        UINT nIndex = (UINT)_ttoi64(strMessageID);
        m_SMSServer.UpdateMessageState(nIndex, 1);
    }
    if(strContactList == _T("!"))
        strContactList.Empty();
    CString strContentText = pFdp->pListData->GetValueByKey(_T("Message"))->m_strText.c_str();
    bool bSaveToSim = false;
    INT_PTR nRetCode = m_MsgBox.DoModal(strContactList, strContentText, bSaveToSim, &m_mapPB);
    ProcessMessageEditorReturn(nRetCode, strContactList, strContentText);
}

FORCEINLINE void CMessage_Version2View::OnEventTreeUILSelectChange(CQTreeNode* pNode) {
    CMessageFolderNode* pMessageFolderNode = reinterpret_cast<CMessageFolderNode*>(pNode);
    if(pNode == NULL || pMessageFolderNode->m_nFolderID == INFINITE) return;
    while(m_MsgDataSrc.GetRowCount()) m_MsgDataSrc.DeleteDataByIndex(0);
    enumMsgLoc nCurrentFolder = (enumMsgLoc)pMessageFolderNode->m_nFolderID;
    m_pServerAdapter->SetCurrentFolderID(pMessageFolderNode->m_nFolderID);
    if(nCurrentFolder == SIM_DRAFT || nCurrentFolder == OUTBOX)
        m_MsgDataSrc.UpdateColumnByKey(_T("Sender"), GetString(_T("IDS_Message_Button_Receiver")));
    else
        m_MsgDataSrc.UpdateColumnByKey(_T("Sender"), GetString(_T("IDS_Message_List_Sender")));
    m_msgListCtrl.SetColumnWidth(1, m_pCfgTool->GetCfgVar(_T("Sender"), 140));
    m_SMSServer.GetMessagesByLocation(nCurrentFolder);
    ClearEditText();
}

FORCEINLINE void CMessage_Version2View::OnEventListUIRButtonDown(vector<FireListDataParameter*>* vecFPD) {
    for(vector<FireListDataParameter*>::iterator iter = vecFPD->begin(); iter != vecFPD->end(); iter++) {
        CPoint& point = (*iter)->point;
        UINT& nFlags = (*iter)->nFlags;
        if((*iter)->isFocused) {
            RECT rect;
            m_msgListCtrl.GetWindowRect(&rect);
            point.x += rect.left;
            point.y += rect.top;
            QPoint ptPoint;
            ptPoint.m_nPosX = point.x + 2;
            ptPoint.m_nPosY = point.y + 2;
            m_SubmenuBase.Event(EVENT_SUBMENU, reinterpret_cast<long>(&ptPoint));
            CWnd::OnRButtonDown(nFlags, point);
            break;
        }
    }
}

FORCEINLINE void CMessage_Version2View::UpdateButtonStatus(bool btnNew, bool btnForward, bool btnReply, bool btnSend, bool btnDelete,/* bool btnBackup, bool btnRestore,*/ bool btnRefresh) {
    if(m_btnNew)
		m_btnNew->SetEnable(btnNew);
	if(m_btnForward)
		m_btnForward->SetEnable(btnForward);
	if(m_btnReply)
		m_btnReply->SetEnable(btnReply);
	if(m_btnSend)
		m_btnSend->SetEnable(btnSend);
	if(m_btnDelete)
		m_btnDelete->SetEnable(btnDelete);
	if(m_btnRefresh)
		m_btnRefresh->SetEnable(btnRefresh);
}


FORCEINLINE void CMessage_Version2View::OnEventAfterSwitchView(long nParam)
{
    TSTRING strMobileModel, strIMEI;
    SwitchViewPara* view_switch = (SwitchViewPara*)nParam;
    if(view_switch) {
        if(view_switch->m_pIMainframe->IsConnected(strMobileModel, strIMEI)) {
            UpdateButtonStatus(true, false, false, false, false, /*true, true,*/ true);
            m_bIsConnected = true;
        }
    }
}

FORCEINLINE void CMessage_Version2View::IncreaseProgressBar() {
    // 對progress bar進度加1個單位
    //m_GetMessageEvent.SetEvent();
    if(m_nTotalMessageCount != m_nCurrentProgress) {
        if(m_nTotalMessageCount != 0)
            m_ProgressBar.m_nPersent = m_nCurrentProgress * 100 / m_nTotalMessageCount;
        m_nCurrentProgress++;
        TRACE("Progress percent: %d\n", m_ProgressBar.m_nPersent);
    } else {
        m_ProgressBar.m_nPersent = 100;
        m_ProgressBar.m_bFinish = true;
    }
}


//===================================================================
// Method:    ProgressBarThread
// FullName:  QSYNC_LIB::CMessage_Version2View::ProgressBarThread
// Access:    public static 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID
//===================================================================
UINT CMessage_Version2View::ProgressBarThread(LPVOID lpParam) {
    CMessage_Version2View* pThis = (CMessage_Version2View*)lpParam;
    if(pThis->m_nTotalMessageCount != 0)
        CQProgressServer::Exec(&(pThis->m_ProgressBar));
    AfxEndThread(0);
    return TRUE;
}

FORCEINLINE void CMessage_Version2View::BeginProgressBar(long nParam) {
    m_ProgressBar.m_nPersent = 0;
    m_ProgressBar.m_bFinish = false;
    m_nTotalMessageCount = nParam;
    m_nCurrentProgress = 1;
    m_ProgressBar.SetDiaplayText(_T("IDS_Message_Progressing"));
	m_ProgressBar.m_RunningEvent.SetEvent();
    m_pProgressThread = AfxBeginThread(ProgressBarThread, this);
}

FORCEINLINE void CMessage_Version2View::EndProgressBar(TSTRING strMessage) {	
    m_ProgressBar.m_nPersent = 100;
    m_ProgressBar.m_bFinish = true;
	m_ProgressBar.m_RunningEvent.ResetEvent();
    if(m_pProgressThread)
        m_pProgressThread->ExitInstance();
    m_pProgressThread = NULL;
	m_strMessage = strMessage;
	m_ProgressFinishEvent.SetEvent();
}

FORCEINLINE void CMessage_Version2View::OnHdnEndtrackList(NMHDR *pNMHDR, LRESULT *pResult) {
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    ColumnData cd;
    m_MsgDataSrc.GetColumnByIndex(phdr->iItem, &cd);
    m_pCfgTool->SetCfgVar(cd.m_strKey, phdr->pitem->cxy);
    m_pCfgTool->Save(STORAGE_INIFILE);
    *pResult = 0;
}

FORCEINLINE void CMessage_Version2View::OnHdnDividerDBLClickList(NMHDR *pNMHDR, LRESULT *pResult) {
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    int nWidth = m_msgListCtrl.GetColumnWidth(phdr->iItem);
    ColumnData cd;
    m_MsgDataSrc.GetColumnByIndex(phdr->iItem, &cd);
    m_pCfgTool->SetCfgVar(cd.m_strKey, nWidth);
    m_pCfgTool->Save(STORAGE_INIFILE);
    *pResult = 0;
}

FORCEINLINE void CMessage_Version2View::ClearEditText() {
    m_cntEdit.SetWindowText(_T(""));
}

FORCEINLINE void CMessage_Version2View::OnListUIBeginDrag(long nParam) {
    CDragDropFireParam *pFireParam = (CDragDropFireParam*)(nParam);
    pFireParam->SetEffect(DROPEFFECT_COPY);
    vector<FireListDataParameter> vecParam;
    m_pListCtrlLogic->GetSelectedItem(vecParam);
    TCHAR szMy_DocumentPath[MAX_PATH] = {0};
    SHGetFolderPath(GetSafeHwnd(), CSIDL_COMMON_TEMPLATES, NULL, NULL, szMy_DocumentPath);
    for(UINT i = 0; i < vecParam.size(); ++i) {
        CString strPath(szMy_DocumentPath);
        CString strRandom;
        strPath += _T("\\[");
        strRandom.Format(_T("%.5d"), rand());
        strPath += strRandom;
        strPath += _T("][");
        strPath += vecParam.at(i).pListData->GetValueByKey(_T("Sender"))->GetTextString().c_str();
        strPath += _T("].qmsg");
        CFile file(strPath, CFile::modeCreate | CFile::modeWrite);
        CString strWrite(_T("Sender: "));
        strWrite += vecParam.at(i).pListData->GetValueByKey(_T("Sender"))->GetTextString().c_str();
        strWrite += _T("\r\nMessage: ");
        strWrite += vecParam.at(i).pListData->GetValueByKey(_T("Message"))->GetTextString().c_str();
        strWrite += _T("\r\nState: ");
        strWrite += vecParam.at(i).pListData->GetValueByKey(_T("HideState"))->GetTextString().c_str();
        strWrite += _T("\r\nLocation: ");
        strWrite += vecParam.at(i).pListData->GetValueByKey(_T("Location"))->GetTextString().c_str();
        strWrite += _T("\r\nStorage: ");
        strWrite += vecParam.at(i).pListData->GetValueByKey(_T("HideStorage"))->GetTextString().c_str();
        DWORD dwNum = WideCharToMultiByte(CP_UTF8, NULL, (LPTSTR)(LPCTSTR)strWrite, -1, NULL, 0, NULL, FALSE);
        char* szWriteOut = new char[dwNum];
        WideCharToMultiByte (CP_UTF8, NULL, (LPTSTR)(LPCTSTR)strWrite, -1, szWriteOut, dwNum, NULL, FALSE);
        file.Write((LPVOID)szWriteOut, dwNum);
        file.Flush();
        file.Close();
        delete [] szWriteOut;
        pFireParam->Insert((LPTSTR)(LPCTSTR)strPath);
    }
}

FORCEINLINE void CMessage_Version2View::OnEventUIItemChanged(long nParam) 
{
	vector<FireListDataParameter>* vecParam = reinterpret_cast<vector<FireListDataParameter>*>(nParam);
	ClearEditText();
	if(vecParam->size() == 0) 
	{
		UpdateButtonStatus(true, false, false, false, false,/* true, true, */true);
	} 
	else if (vecParam->size() == 1)
	{
		switch(m_pServerAdapter->GetCurrentFolderID())
		{
		case 1: //Inbox
			UpdateButtonStatus(true, true, true, false, true, /*true, true, */true);
			break;
		case 2: //Draft
			UpdateButtonStatus(true, false, false, true, true, /*true, true, */true);
			break;
		case 3: //Outbox
			UpdateButtonStatus(true, false, false, true, true, /*true, true, */true);
			break;
		case 4: //Sent
			UpdateButtonStatus(true, true, false, true, true, /*true, true, */true);
			break;
		default:
			break;
		}
		OnEventListUILButtonDown(&vecParam->at(0));
	}
	else //multi selected
	{
		UpdateButtonStatus(true, false, false, false, true, /*true, true, */true);
		OnEventListUILButtonDown(&vecParam->at(0));
	}


}

FORCEINLINE void CMessage_Version2View::OnEventListUIDrop(long nParam) {
    CDragDropFireParam* pdfp = (CDragDropFireParam*)nParam;
    UINT nFileCount = pdfp->GetFileCount();
    CDragDropFireParam* pTmp = new CDragDropFireParam();
    for(UINT i = 0; i < nFileCount; ++i)
        pTmp->Insert(pdfp->GetByIndex(i));

    ListUIDropThreadParam* ltp = new ListUIDropThreadParam;
    ltp->pDropFileParam = pTmp;
    ltp->pServer = &m_SMSServer;
    ltp->pThis = this;
    BeginProgressBar(nFileCount);
    AfxBeginThread(ListUIDropThread, ltp);
}

FORCEINLINE void CMessage_Version2View::OnEventOnSize(long nParam) {
    if(nParam == 0) return;
    FireOnSizeParam* fsp = (FireOnSizeParam*)nParam;
}
FORCEINLINE void CMessage_Version2View::OnEventOnSizing(long nParam) {
    if(nParam == 0) return;
    FireOnSizingParam* fsp = (FireOnSizingParam*)nParam;
}

//===================================================================
// Following:
// Thread for sending, saving, deleting, refreshing message
//===================================================================

//===================================================================
// Method:    ForwardMessageThread
// FullName:  QSYNC_LIB::CMessage_Version2View::ForwardMessageThread
// Access:    public static 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID lpParam
//===================================================================
UINT CMessage_Version2View::ForwardMessageThread(LPVOID lpParam) {
    AfxEndThread(0);
    return TRUE;
}

//===================================================================
// Method:    SendMessageThread
// FullName:  QSYNC_LIB::CMessage_Version2View::SendMessageThread
// Access:    public static 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID lpParam
//===================================================================
UINT CMessage_Version2View::SendMessageThread(LPVOID lpParam) {
    SendThreadParam& thParam = *(SendThreadParam*)lpParam;
    bool bRetVal = thParam.pServer->SendMessage(thParam.strContact, thParam.strContent);
    if(bRetVal)
        ::SendMessage(thParam.pThis->GetSafeHwnd(), WM_SEND_SUCCESS, WM_SEND_SUCCESS, NULL);
    else
        ::SendMessage(thParam.pThis->GetSafeHwnd(), WM_SEND_FAILED, WM_SEND_FAILED, NULL);
    delete &thParam;
    AfxEndThread(0);
    return bRetVal;
}

//===================================================================
// Method:    SaveMessageThread
// FullName:  QSYNC_LIB::CMessage_Version2View::SaveMessageThread
// Access:    public static 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID lpParam
//===================================================================
UINT CMessage_Version2View::SaveMessageThread(LPVOID lpParam) {
    WriteThreadParam& thParam = *(WriteThreadParam*)lpParam;
    bool bRetVal = thParam.pServer->WriteMessage(thParam.strContact, thParam.strContent);
    if(bRetVal)
        ::SendMessage(thParam.pThis->GetSafeHwnd(), WM_SAVE_SUCCESS, WM_SAVE_SUCCESS, NULL);
    else
        ::SendMessage(thParam.pThis->GetSafeHwnd(), WM_SAVE_FAILED, WM_SAVE_FAILED, NULL);
    delete &thParam;
    AfxEndThread(0);
    return TRUE;
}

// This function is the same as previous, but this function is for new AT command using
// It will contains different parameters from previous'.
// Notice that if the mobile is different, this thread function should be replace previous.
//===================================================================
// Method:    SaveMessageThreadNew
// FullName:  QSYNC_LIB::CMessage_Version2View::SaveMessageThreadNew
// Access:    public static 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID lpParam
//===================================================================
UINT CMessage_Version2View::SaveMessageThreadNew(LPVOID lpParam) {
    WriteThreadParam& thParam = *(WriteThreadParam*)lpParam;
    bool bRetVal = thParam.pServer->WriteMessage(thParam.strContact, thParam.strContent);
    if(bRetVal)
        ::SendMessage(thParam.pThis->GetSafeHwnd(), WM_SAVE_SUCCESS, WM_SAVE_SUCCESS, NULL);
    else
        ::SendMessage(thParam.pThis->GetSafeHwnd(), WM_SAVE_FAILED, WM_SAVE_FAILED, NULL);
    delete &thParam;
    AfxEndThread(0);
    return TRUE;
}

//===================================================================
// Method:    DeleteMessageThread
// FullName:  QSYNC_LIB::CMessage_Version2View::DeleteMessageThread
// Access:    public static 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID lpParam
//===================================================================
UINT CMessage_Version2View::DeleteMessageThread(LPVOID lpParam) {
    DeleteThreadParam& thParam = *(DeleteThreadParam*)lpParam;
    for(size_t i = 0; i < thParam.pVecFDP->size(); ++i) {
        FireListDataParameter& f = thParam.pVecFDP->at(i);
        CString strIndex = f.pListData->GetValueByKey(_T("Index"))->m_strText.c_str();
        thParam.pServer->DeleteMessage(strIndex);
        thParam.pDataSrc->DeleteDataByIndex(f.nRow - (int)i);
    }
    ::SendMessage(thParam.pThis->GetSafeHwnd(), WM_DELETE_SUCCESS, WM_DELETE_SUCCESS, NULL);
    delete thParam.pVecFDP;
    thParam.pVecFDP = NULL;
    delete &thParam;
    AfxEndThread(0);
    return TRUE;
}

//===================================================================
// Method:    RefreshMessageThread
// FullName:  QSYNC_LIB::CMessage_Version2View::RefreshMessageThread
// Access:    public static 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID lpParam
//===================================================================
UINT CMessage_Version2View::RefreshMessageThread(LPVOID lpParam) {
    RefreshThreadParam* rtp = (RefreshThreadParam*)lpParam;
    bool bRes = rtp->pServer->ReloadAllMessageFromMobile();
    //ASSERT(bRes);
    ::PostMessage(rtp->pThis->GetSafeHwnd(), WM_REFRESH_SUCCESS, WM_REFRESH_SUCCESS, NULL);
    delete rtp;
    AfxEndThread(0);
    return TRUE;
}

//===================================================================
// Method:    BackupMessageThread
// FullName:  QSYNC_LIB::CMessage_Version2View::BackupMessageThread
// Access:    public static 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID lpParam
//===================================================================
UINT CMessage_Version2View::BackupMessageThread(LPVOID lpParam) {
    BackupRestoreThreadParam* brtp = (BackupRestoreThreadParam*)lpParam;
    bool bRetVal = brtp->pServer->Backup(brtp->strFileName);
    if(bRetVal)
        ::SendMessage(brtp->pThis->GetSafeHwnd(), WM_BACKUP_SUCCESS, WM_BACKUP_SUCCESS, NULL);
    else
        ::SendMessage(brtp->pThis->GetSafeHwnd(), WM_BACKUP_FAILED, WM_BACKUP_FAILED, NULL);
    delete brtp;
    AfxEndThread(0);
    return TRUE;
}

//===================================================================
// Method:    RestoreMessageThread
// FullName:  QSYNC_LIB::CMessage_Version2View::RestoreMessageThread
// Access:    public static 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID lpParam
//===================================================================
UINT CMessage_Version2View::RestoreMessageThread(LPVOID lpParam) {
    BackupRestoreThreadParam* brtp = (BackupRestoreThreadParam*)lpParam;
    bool bRetVal = brtp->pServer->Restore(brtp->strFileName);
    if(bRetVal)
        ::SendMessage(brtp->pThis->GetSafeHwnd(), WM_RESTORE_SUCCESS, WM_RESTORE_SUCCESS, NULL);
    else
        ::SendMessage(brtp->pThis->GetSafeHwnd(), WM_RESTORE_FAILED, WM_RESTORE_FAILED, NULL);
    delete brtp;
    AfxEndThread(0);
    return TRUE;
}

UINT CMessage_Version2View::ListUIDropThread(LPVOID lpParam) {
    ListUIDropThreadParam* ltp = (ListUIDropThreadParam*)lpParam;
    TCHAR szMy_DocumentPath[MAX_PATH] = {0};
    SHGetFolderPath(ltp->pThis->GetSafeHwnd(), CSIDL_COMMON_TEMPLATES, NULL, NULL, szMy_DocumentPath);
    int nCount = ltp->pDropFileParam->GetFileCount();
    for(int i = 0; i < nCount; ++i) {
        if(ltp->pDropFileParam->GetByIndex(i).find(szMy_DocumentPath) != wstring::npos)
            return FALSE;;
    }
    // TODO: open these file and write to mobile.
    // TODO: it doesn't need to check that there is the same message or not
    // TODO: read file and parse message, phone, location, tag, state, storage then write into mobile
    for(int i = 0; i < nCount; ++i) {
        //IncreaseProgressBar();
        if(ltp->pDropFileParam->GetByIndex(i).find(_T(".qmsg")) == wstring::npos)
            continue;
        CStdioFile file(ltp->pDropFileParam->GetByIndex(i).c_str(), CFile::modeRead);
        CString strSender, strMsg, strState, strLoc, strStorage;
        if(file.m_pStream) {
            file.ReadString(strSender);
            file.ReadString(strMsg);
            file.ReadString(strState);
            file.ReadString(strLoc);
            file.ReadString(strStorage);
            ltp->pServer->WriteDropMessage(strSender, strMsg, strState, strLoc, strStorage);
        }
        file.Close();
    }
    ltp->pThis->EndProgressBar(_T("IDS_Message_Write_Successfully"));
    delete ltp->pDropFileParam;
    delete ltp;
    AfxEndThread(0);
    return TRUE;
}

void CMessage_Version2View::GetPhonebook() {

    using namespace QSYNC_LIB;
    HINSTANCE hDLL = NULL;        
    hDLL = ::LoadLibrary(_T("Phonebook_version2.dll"));
    if(hDLL) {
        LPGetPhonebook lpfnDllFunc = (LPGetPhonebook)::GetProcAddress(hDLL,"GetPhonebook");
        if(lpfnDllFunc) {
            m_mapPB.clear();
            lpfnDllFunc(m_mapPB);
        }
    }
    ::FreeLibrary(hDLL);
}

void CMessage_Version2View::Event(const TSTRING& strEvent, long nParam) {
    if(strEvent == EVENT_LIST_UI_LBTNDOWN) {
        // UI在list control下按下左鍵
        OnEventListUILButtonDown((reinterpret_cast<vector<FireListDataParameter*>*>(nParam))->at(0));
    } else if(strEvent == EVENT_LIST_UI_LDBCLICK) {
        // UI在list control下按下double click左鍵
        OnEventListUILButtonDoubleClick((reinterpret_cast<vector<FireListDataParameter*>*>(nParam))->at(0));
    } else if(strEvent == EVENT_LIST_UI_ITEMCHANGED) {
        //fdp& fdpChangeItem = (reinterpret_cast<vector<fdp>*>(nParam))->at(0);
        OnEventUIItemChanged(nParam);
    } else if(strEvent == EVENT_LIST_UI_RBTNDOWN) {
        // UI在list control下按下右鍵
        OnEventListUIRButtonDown(reinterpret_cast<vector<FireListDataParameter*>*>(nParam));
    } else if(strEvent == EVENT_TREE_SELECT_NODE) {
        // UI在tree control下選到了不同的tree node
        OnEventTreeUILSelectChange(reinterpret_cast<CQTreeNode*>(nParam));
    } else if(strEvent == EVENT_LIST_UI_DRAG_BEGIN) {
		m_bIsDragging  = true;
        OnListUIBeginDrag(nParam);
    } else if(strEvent == EVENT_LIST_UI_DROP) {
		if(!m_bIsSelfDragging)
			OnEventListUIDrop(nParam);
	} else if(strEvent == EVENT_LIST_UI_DRAG_ENTER) {
		if(!m_bIsDragging)
			m_bIsSelfDragging = false;
	} else if(strEvent == EVENT_LIST_UI_DRAG_OVER) {
    } else if(strEvent == EVENT_DRAG_END) {
		if(m_bIsDragging)
			m_bIsSelfDragging = true;
		m_bIsDragging = false;
    } else if(strEvent == SMSSERVER_LIST_ONE_FOLDER   ) {
        TRACE("SMSSERVER_LIST_ONE_FOLDER\n");
    } else if(strEvent == SMSSERVER_READ_ONE_MESSAGE  ) {
        RedrawWindow();
        IncreaseProgressBar();
    } else if(strEvent == SMSSERVER_LIST_FOLDER_OK  ) {
        BeginProgressBar(nParam);
    } else if(strEvent == SMSSERVER_DELETE_ONE_MESSAGE) {
        ClearEditText();
    } else if(strEvent == SMSSERVER_WRITE_ONE_MESSAGE ) {
    } else if(strEvent == SMSSERVER_SAVE_ONE_MESSAGE  ) {
    } else if(strEvent == _T("IDS_MessageSubMenu_New")  ) {
        OnMsgBtnNewClick();
    } else if(strEvent == _T("IDS_MessageSubMenu_Forward")  ) {
        OnMsgBtnForwardClick();
    } else if(strEvent == _T("IDS_MessageSubMenu_Reply")  ) {
        OnMsgBtnReplyClick();
    } else if(strEvent == _T("IDS_MessageSubMenu_Send")  ) {
        OnMsgBtnSendClick();
    } else if(strEvent == _T("IDS_MessageSubMenu_Delete")  ) {
        OnMsgBtnDeleteClick();
    } else if(strEvent == TNotify_Language_Change) {
        RefreshUILocalization();
    } else if(strEvent == _T("TNotify_Connect")) {
		if(m_bIsShow)
		{
			m_pServerAdapter->GetATCommandSet();    // let server adapter get the setting of mobile
			UpdateButtonStatus(true, false, false, false, false, /*true, true,*/ true);
			m_bIsConnected = true;
			m_SMSServer.CreateISMS();
			if(m_bIsConnected) {
				OnMsgBtnRefreshClick(); // doing first refresh, user won't need press this button
				m_bIsSynced = true;
			}
			EnableWindow(TRUE);
		}
    } else if(strEvent ==  _T("TNotify_Disconnect")) {
		m_MsgBox.Disconnect(_T("IDS_MESSAGE_FUNCTION"));
        UpdateButtonStatus(false, false, false, false, false,/* false, false,*/ false);
        m_bIsConnected = false;
        m_bIsSynced = false;
        m_SMSServer.m_pISMS = NULL;
        m_msgListCtrl.DeleteAllItems();
        m_folderTreeCtrl.DeleteAllItems();
		m_cntEdit.SetWindowText(_T(""));
    } else if(strEvent == _T("TNotify_Before_Switch")) 
	{
		m_bIsShow = false;
    } else if(strEvent ==  _T("TNotify_After_Switch")) 
	{
		m_bIsShow = true;
        OnEventAfterSwitchView(nParam);
        if(m_bIsConnected && !m_bIsSynced) {
            OnMsgBtnRefreshClick();
            m_bIsSynced = true;
        }
    } else if(strEvent == SMSSERVER_BEGIN_PROGRESS_BAR) {
        BeginProgressBar(nParam);
    } else if(strEvent == SMSSERVER_END_PROGRESS_BAR  ) {
        std::wstring* pStr = (std::wstring*)nParam;
        EndProgressBar(*pStr);
    } else if(strEvent == SMSSERVER_BACKUP_ONE_MESSAGE ) {
        //IncreaseProgressBar();
    } else if(strEvent == SMSSERVER_RESTORE_ONE_MESSAGE) {
        //IncreaseProgressBar();
    } else if(strEvent == SMSSERVER_SEND_ONE_PAGE_MSG) {
        //IncreaseProgressBar();
    } else if(strEvent == EVENT_LIST_UI_ONSIZE) {
        OnEventOnSize(nParam);
    } else if(strEvent == EVENT_LIST_UI_ONSIZING) {
        OnEventOnSizing(nParam);
    }
	else if(strEvent == EVENT_BUTTON_CLICK)
	{
		if(m_btnNew->GetID() == nParam)
			OnMsgBtnNewClick();
		else if(m_btnForward->GetID() == nParam)
			OnMsgBtnForwardClick();
		else if(m_btnReply->GetID() == nParam)
			OnMsgBtnReplyClick();
		else if(m_btnSend->GetID() == nParam)
			OnMsgBtnSendClick();
		else if(m_btnDelete->GetID() == nParam)
			OnMsgBtnDeleteClick();
		else if(m_btnRefresh->GetID() == nParam)
			OnMsgBtnRefreshClick();
	}
}

// Handel the message box if the thread send back a message
LRESULT CMessage_Version2View::OnThreadBakupMessage(WPARAM wParam, LPARAM lParam) {
    switch(wParam) {
    case WM_BACKUP_SUCCESS:
        ShowMessage(this->GetSafeHwnd(), _T("IDS_Message_Backup_Successfully"), _T("IDS_MESSAGE_FUNCTION") );
        break;
    case WM_BACKUP_FAILED:
        ShowMessage(this->GetSafeHwnd(), _T("IDS_Message_Backup_Failed"), _T("IDS_MESSAGE_FUNCTION"));
        break;
    }
    return 0;
}
LRESULT CMessage_Version2View::OnThreadDeleteMessage(WPARAM wParam, LPARAM lParam) {
    EndProgressBar(_T("IDS_Message_Delete_Successfully"));
    return 0;
}
LRESULT CMessage_Version2View::OnThreadForwardMessage(WPARAM wParam, LPARAM lParam) {
    return 0;
}
LRESULT CMessage_Version2View::OnThreadRefreshMessage(WPARAM wParam, LPARAM lParam) {
    EndProgressBar(_T("IDS_Message_Refresh_Successfully"));	
    return 0;
}
LRESULT CMessage_Version2View::OnThreadRestoreMessage(WPARAM wParam, LPARAM lParam) {
    switch(wParam) {
    case WM_RESTORE_SUCCESS:
        ShowMessage(this->GetSafeHwnd(), _T("IDS_Message_Restore_Successfully"), _T("IDS_MESSAGE_FUNCTION"));
        break;
    case WM_RESTORE_FAILED:
        ShowMessage(this->GetSafeHwnd(), _T("IDS_Message_Restore_Failed"), _T("IDS_MESSAGE_FUNCTION"));
        break;
    }
    return 0;
}
LRESULT CMessage_Version2View::OnThreadSaveMessage(WPARAM wParam, LPARAM lParam) {
    switch(wParam) {
        case WM_SAVE_SUCCESS:
            ShowMessage(this->GetSafeHwnd(), _T("IDS_Message_Write_Successfully"), _T("IDS_MESSAGE_FUNCTION"));
            break;
        case WM_SAVE_FAILED:
            ShowMessage(this->GetSafeHwnd(), _T("IDS_Message_Write_Failed"), _T("IDS_MESSAGE_FUNCTION"));
            break;
    }
    return 0;
}
LRESULT CMessage_Version2View::OnThreadSendMessage(WPARAM wParam, LPARAM lParam) {
    switch(wParam) {
        case WM_SEND_SUCCESS:
            EndProgressBar(_T("IDS_Message_Send_Successfully"));
            break;
        case WM_SEND_FAILED:
            EndProgressBar(_T("IDS_Message_Send_Failed"));
            break;
    }
    return 0;
}

#pragma warning(pop)