// 22View.cpp : implementation of the CFileManager_VersionView2 class
//


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "stdafx.h"
#include "FileManager_VersionView2.h"
#include <map>
#include "../../Components/UIFramework/interface/IQTreeUI.h"
#include "../../Components/UIFramework/interface/QCommonControlBase.h"
#include "../../Components/UIFramework/porting/QProgressServer.h"
#include "..\..\CommonUtil\QSyncerFrameworks.h"
#include "..\..\CommLayer\MobileFinder\MobileFinderInterface.h"
#include "..\..\CommonUtil\Win32SHELLUtility.h"
#include "../../Components/UIFramework/interface/ShowMessage.h"


#pragma warning(push)
#pragma warning(disable:4312)
#pragma warning(disable:4311)

using namespace QSYNC_LIB;

IMPLEMENT_DYNCREATE(CFileManager_VersionView2, Win32DoubleBufferFormViewWidge)

BEGIN_MESSAGE_MAP(CFileManager_VersionView2, Win32DoubleBufferFormViewWidge)
    ON_WM_CREATE()
END_MESSAGE_MAP()

// CFileManager_VersionView2 construction/destruction
extern class NotifyReceiver g_NotifyReceiver;
CFileManager_VersionView2* g_pThis = NULL;

CFileManager_VersionView2::CFileManager_VersionView2()
: Win32DoubleBufferFormViewWidge(CFileManager_VersionView2::IDD)
,m_pILocalization(NULL)
,m_pMETreeLogical(NULL)
,m_pMEListLogical(NULL)
,m_pPCTreeLogical(NULL)
,m_pPCListLogical(NULL)
,m_pCurrentFileNode(NULL)
,m_bShow(false)
,m_bConnected(false)
,m_bCanExit(true)
,m_bIsDragging(false)
,m_EnableExistNotify(this)
,m_pCurrentNode(NULL)
,m_pMEEventHandler(NULL)
,m_pPCEventHandler(NULL)
,m_pIPIMConfig(NULL)
,m_pLayoutManager(NULL)
,m_CurrentWnd(None)
,m_pInputFolderDlg(NULL)
,m_pRenameFileDlg(NULL)
{
	g_pThis = this;
    g_NotifyReceiver.m_pINotify = this;
    m_pMEEventHandler = new CFileManagerEventMEHandler(this);
    m_pPCEventHandler = new CFileManagerEventPCHandler(this);
}

CFileManager_VersionView2::~CFileManager_VersionView2()
{
    m_pMETreeLogical->Unregister(m_pMEEventHandler, EVENT_TREE_INSERT_NODE);
    m_pMETreeLogical->Unregister(m_pMEEventHandler, EVENT_TREE_DELETE_NODE);
    m_pMETreeLogical->Unregister(m_pMEEventHandler, EVENT_TREE_UPDATE_NODE);
    m_pMETreeLogical->Unregister(m_pMEEventHandler, EVENT_TREE_SELECT_NODE);
    m_pMETreeLogical->Unregister(m_pMEEventHandler, EVENT_TREE_RBUTTONDOWN_NODE);
    m_pMETreeLogical->Unregister(m_pMEEventHandler, EVENT_TREE_UI_EDIT_BEGIN);
    m_pMETreeLogical->Unregister(m_pMEEventHandler, EVENT_TREE_UI_EDIT_END);
    m_pMETreeLogical->Unregister(m_pMEEventHandler, EVENT_TREE_UI_DROP);
	m_pMETreeLogical->Unregister(m_pMEEventHandler, EVENT_TREE_UI_KILLFOCUS);
	m_pMETreeLogical->Unregister(m_pMEEventHandler, EVENT_TREE_UI_SETFOCUS);
	m_pMETreeLogical->Unregister(m_pMEEventHandler, EVENT_LIST_UI_KILLFOCUS);
	m_pMETreeLogical->Unregister(m_pMEEventHandler, EVENT_LIST_UI_SETFOCUS);

    m_pMEListLogical->Unregister(m_pMEEventHandler, EVENT_LIST_UI_LBTNDOWN);
    m_pMEListLogical->Unregister(m_pMEEventHandler, EVENT_LIST_UI_RBTNDOWN);
    m_pMEListLogical->Unregister(m_pMEEventHandler, EVENT_LIST_UI_LDBCLICK);
    m_pMEListLogical->Unregister(m_pMEEventHandler, EVENT_LIST_UI_RDBCLICK);
    m_pMEListLogical->Unregister(m_pMEEventHandler, EVENT_LIST_UI_LCLICK);
    m_pMEListLogical->Unregister(m_pMEEventHandler, EVENT_LIST_UI_RCLICK);
    m_pMEListLogical->Unregister(m_pMEEventHandler, EVENT_LIST_UI_DROP);
    m_pMEListLogical->Unregister(m_pMEEventHandler, EVENT_LIST_UI_DRAG_ENTER);
    m_pMEListLogical->Unregister(m_pMEEventHandler, EVENT_LIST_UI_DRAG_OVER);
    m_pMEListLogical->Unregister(m_pMEEventHandler, EVENT_LIST_UI_DRAG_LEAVE);
    m_pMEListLogical->Unregister(m_pMEEventHandler, EVENT_LIST_UI_HEADER_BUTTON_HIT);
    m_pMEListLogical->Unregister(m_pMEEventHandler, EVENT_LIST_UI_DRAG_BEGIN);	
    m_pMEListLogical->Unregister(m_pMEEventHandler, EVENT_LIST_UI_EDIT_BEGIN);	
    m_pMEListLogical->Unregister(m_pMEEventHandler, EVENT_LIST_UI_EDIT_END);
	m_pMEListLogical->Unregister(m_pMEEventHandler, EVENT_LIST_UI_DELETE_ROW);

    m_PCListCtrl.Unregister(m_pPCEventHandler, EVENT_ME_LIST_SUBMENU_DOWNLOAD_NODE);

    if(m_pMETreeLogical)
        delete m_pMETreeLogical;
    if(m_pMEListLogical)
        delete m_pMEListLogical;
    if(m_pPCTreeLogical)
        delete m_pPCTreeLogical;
    if(m_pPCListLogical)
        delete m_pPCListLogical;
    if(m_pMEEventHandler)
        delete m_pMEEventHandler;
    if(m_pPCEventHandler)
        delete m_pPCEventHandler;

	if(m_pLayoutManager)
	{
		delete m_pLayoutManager;
		m_pLayoutManager = NULL;
	}
	
	if(m_pInputFolderDlg)
	{
		delete m_pInputFolderDlg;
		m_pInputFolderDlg = NULL;
	}
	if(m_pRenameFileDlg)
	{
		delete m_pRenameFileDlg;
		m_pRenameFileDlg = NULL;
	}
}

void CFileManager_VersionView2::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ME_TREE, m_METreeCtrl);
    DDX_Control(pDX, IDC_ME_LIST, m_MEListCtrl);
    DDX_Control(pDX, IDC_PC_TREE, m_PCTreeCtrl);
    DDX_Control(pDX, IDC_PC_LIST, m_PCListCtrl);
	
}

BOOL CFileManager_VersionView2::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs
    return CFormView::PreCreateWindow(cs);
}

void CFileManager_VersionView2::SetXMLUI()
{
	try{
	std::wstring strBkImage = L"MainFrame\\Bk_GeneralView.png";
	SetBKImage(strBkImage,BK_Fixed);
	// Load
	m_pLayoutManager = new CLayoutManager(this, this);
	Widge<HDC>* pWidget = m_pLayoutManager->Load(_T("FileManagerID"));
	m_pDeleteButton = (WIN32ButtonWidge*)m_pLayoutManager->GetWidgetByID(_T("DeleteButton"));
	m_pRefreshButton = (WIN32ButtonWidge*)m_pLayoutManager->GetWidgetByID(_T("RefreshButton"));
	bool bSetLayout = SetLayout(pWidget);
	}
	catch(...)
	{
		AfxMessageBox(_T("error"));
	}	
}

void CFileManager_VersionView2::FuncDelete()
{
	if(ShowMessage(GetSafeHwnd(),_T("IDS_FileMgr_DELETE_PROMPT"), _T("IDS_FILEMANAGER_FUNCTION"), QMB_YESNO) == QIDNO)
		return;
	bool bDeleteSuccess = false;
	FileManager_ListWnd CurrentWnd = GetCurrentWnd();
	switch(CurrentWnd){
		case MEList:
			bDeleteSuccess = DelMEListSelectedFile();
			break;
		case METree:
			{
				CQTreeNode* pNode = m_METreeCtrl.GetCurrentNode();
				bDeleteSuccess = DelMETreeFolder(pNode);
			}
			break;
		default:
			break;
	};// switch end

		if(bDeleteSuccess == false)
			ShowMessage(GetSafeHwnd(),_T("IDS_FileMgr_Delete_Fail"),_T("IDS_FILEMANAGER_FUNCTION"));
}

void CFileManager_VersionView2::FuncRefresh()
{
	if(m_pMETreeLogical)
		m_pMETreeLogical->RefreshClear();
	if(m_pMEListLogical)
		m_pMEListLogical->RefreshClear();

	m_METreeDataSrc.GetRoot();
	m_pDeleteButton->SetEnable(false);
}

void CFileManager_VersionView2::OnInitialUpdate()
{
    Win32DoubleBufferFormViewWidge::OnInitialUpdate();
    // Setting register for ME
    bool bCont = false;
    m_METreeCtrl.SetDropable(true);

    if(m_pMETreeLogical != NULL && m_pMEEventHandler != NULL) {
        m_pMETreeLogical->Register(m_pMEEventHandler, EVENT_TREE_INSERT_NODE);
        m_pMETreeLogical->Register(m_pMEEventHandler, EVENT_TREE_DELETE_NODE);
        m_pMETreeLogical->Register(m_pMEEventHandler, EVENT_TREE_UPDATE_NODE);
        m_pMETreeLogical->Register(m_pMEEventHandler, EVENT_TREE_SELECT_NODE);
        m_pMETreeLogical->Register(m_pMEEventHandler, EVENT_TREE_RBUTTONDOWN_NODE);
        m_pMETreeLogical->Register(m_pMEEventHandler, EVENT_TREE_UI_EDIT_BEGIN);
        m_pMETreeLogical->Register(m_pMEEventHandler, EVENT_TREE_UI_EDIT_END);
        m_pMETreeLogical->Register(m_pMEEventHandler, EVENT_TREE_UI_DROP);
		m_pMETreeLogical->Register(m_pMEEventHandler, EVENT_TREE_UI_KILLFOCUS);
		m_pMETreeLogical->Register(m_pMEEventHandler, EVENT_TREE_UI_SETFOCUS);
	}
    m_pMEListLogical = new CQCommListCtrl(&m_MEListDataSrc, &m_MEListCtrl );
    if(m_pMEListLogical != NULL && m_pMEEventHandler != NULL) {
        m_pMEListLogical->Register(m_pMEEventHandler, EVENT_LIST_UI_LBTNDOWN);
        m_pMEListLogical->Register(m_pMEEventHandler, EVENT_LIST_UI_RBTNDOWN);
        m_pMEListLogical->Register(m_pMEEventHandler, EVENT_LIST_UI_LDBCLICK);
        m_pMEListLogical->Register(m_pMEEventHandler, EVENT_LIST_UI_RDBCLICK);
        m_pMEListLogical->Register(m_pMEEventHandler, EVENT_LIST_UI_LCLICK);
        m_pMEListLogical->Register(m_pMEEventHandler, EVENT_LIST_UI_RCLICK);
        m_pMEListLogical->Register(m_pMEEventHandler, EVENT_LIST_UI_DROP);
        m_pMEListLogical->Register(m_pMEEventHandler, EVENT_LIST_UI_DRAG_ENTER);
        m_pMEListLogical->Register(m_pMEEventHandler, EVENT_LIST_UI_DRAG_OVER);
        m_pMEListLogical->Register(m_pMEEventHandler, EVENT_LIST_UI_DRAG_LEAVE);
        m_pMEListLogical->Register(m_pMEEventHandler, EVENT_LIST_UI_HEADER_BUTTON_HIT);
        m_pMEListLogical->Register(m_pMEEventHandler, EVENT_LIST_UI_DRAG_BEGIN);	
        m_pMEListLogical->Register(m_pMEEventHandler, EVENT_LIST_UI_EDIT_BEGIN);	
        m_pMEListLogical->Register(m_pMEEventHandler, EVENT_LIST_UI_EDIT_END);
		m_pMEListLogical->Register(m_pMEEventHandler, EVENT_LIST_UI_ITEMCHANGED);
		m_pMEListLogical->Register(m_pMEEventHandler, EVENT_LIST_UI_KILLFOCUS);
		m_pMEListLogical->Register(m_pMEEventHandler, EVENT_LIST_UI_SETFOCUS);
		m_pMEListLogical->Register(m_pMEEventHandler, EVENT_DRAG_END);
		m_pMEListLogical->Register(m_pMEEventHandler, EVENT_LIST_UI_DELETE_ROW);
    }
	m_METreeDataSrc.SetParentHWND(GetSafeHwnd());
    bool bListFlag = InitFileMgrList();
	SetXMLUI();


    //m_PCListCtrl.SubclassDlgItem(IDC_PC_LIST, GetDlgItem(IDD_FILEMANAGER_VERSION2_FORM));
    m_PCListCtrl.InitilizeCtrl();
    m_PCTreeCtrl.m_pShellListCtrl = &m_PCListCtrl;
    //m_PCTreeCtrl.SubclassDlgItem(IDC_PC_TREE, GetDlgItem(IDD_FILEMANAGER_VERSION2_FORM));
    m_PCTreeCtrl.InitializeCtrl();
    m_PCListCtrl.m_pShellTreeCtrl = &m_PCTreeCtrl;
    m_PCListCtrl.Register(m_pMEEventHandler, EVENT_ME_LIST_SUBMENU_DOWNLOAD_NODE);
    ASSERT(bListFlag);
}
FORCEINLINE bool CFileManager_VersionView2::InitFileMgrList(void)
{
    bool bRet = false;
    bRet = m_MEListDataSrc.InsertColumnKeyAndName(strFILE_NAME_LISTKEY, _T("IDS_FileMgr_List_FileName"),250);
    bRet = m_MEListDataSrc.InsertColumnKeyAndName(strFILE_ATTR_LISTKEY, _T("IDS_FileMgr_List_FileAttr"),50);
    bRet = m_MEListDataSrc.InsertColumnKeyAndName(strFILE_SIZE_LISTKEY, _T("IDS_FileMgr_List_FileSize"),50);
    bRet = m_MEListDataSrc.InsertColumnKeyAndName(strFILE_MDATE_LISTKEY, _T("IDS_FileMgr_List_FileMDate"),130);
    return bRet;
}

// CFileManager_VersionView2 diagnostics

#ifdef _DEBUG
void CFileManager_VersionView2::AssertValid() const
{
    CFormView::AssertValid();
}

void CFileManager_VersionView2::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}
#endif //_DEBUG

bool CFileManager_VersionView2::DownloadFilesIntoPC(const CString& strPCFolder, vector<CString> &vct_strMEFilePath)
{
    m_Progress.InitDownload(m_METreeDataSrc.GetIEFS()->CanCancel() == TRUE,&m_METreeDataSrc,strPCFolder,vct_strMEFilePath);
    m_Progress.Register(&m_EnableExistNotify,EVENT_PROGRESS_TASK_FINISH);
    CQProgressServer::Exec(&m_Progress);

    return true;
}

void CFileManager_VersionView2::UploadFilesToME(CQTreeNode *pNode,CDragDropFireParam *pParam)
{
	if(IsDragging()==false && IsSourceAndTargetFolderTheSame(pParam,pNode) == false)
	{
		std::map<CString, CString> mapFileType;

		TSTRING strSoundsFileType;
		TSTRING strVideosFileType;
		TSTRING strPicturesFileType;

		if(!m_pIPIMConfig)
			m_pIPIMConfig = (IPIMCONFIG*) CommCreateInterface(_T("QSycner"), _T("FileManager"), eIPIMConfigure);

		if(m_pIPIMConfig)
		{
			m_pIPIMConfig->GetProperty(_T("FileManagerConfig"), _T("FileType:Sounds"), strSoundsFileType);
			m_pIPIMConfig->GetProperty(_T("FileManagerConfig"), _T("FileType:Videos"), strVideosFileType);
			m_pIPIMConfig->GetProperty(_T("FileManagerConfig"), _T("FileType:Pictures"), strPicturesFileType);
		}

		mapFileType[_T("Sounds")] = strSoundsFileType.c_str();
		mapFileType[_T("Videos")] = strVideosFileType.c_str();
		mapFileType[_T("Pictures")] = strPicturesFileType.c_str();

		CString strFolderName = pNode->GetName().c_str();
		CQTreeNode* pTmp = pNode;
		CString strCheckFolder;
		while(pTmp->GetParent() != NULL) {
			CString strName = pTmp->GetParent()->GetName().c_str();
			if(strName == _T("/")) {
				strCheckFolder = pTmp->GetName().c_str();
				break;
			}
			pTmp = pTmp->GetParent();
		}
		CString strChecking = mapFileType[strCheckFolder];
		strChecking.MakeLower();
		if(pNode != NULL)
		{
			vector<CString> vct_strFilePath;
			INT_PTR strSize = pParam->GetFileCount();
			bool bAllFileIsValidated = true;
			for(int i = 0; i < strSize; i++)
			{
				int nFind = 0;
				CString strFileName = pParam->GetByIndex(i).c_str();
				if(!strChecking.IsEmpty()) {
					int nDotPos = strFileName.ReverseFind('.');
					CString strExtension = strFileName.Mid(nDotPos+1);
					nFind = strChecking.Find(strExtension.MakeLower());
				}
				if(!strFileName.IsEmpty() && nFind != -1)
					vct_strFilePath.push_back(strFileName);
				else
				{
					bAllFileIsValidated = false;
				}
			}//for end

			if(!bAllFileIsValidated)
			{
				strChecking.Replace(_T("|"),_T(", "));
				CString strMsgText = m_pILocalization->GetString(_T("IDS_FileMgr_MsgBox_FileType_Limitation")).c_str() + strChecking;
				ShowMessage(GetSafeHwnd(),(TSTRING)strMsgText,m_pILocalization->GetString(_T("IDS_FILEMANAGER_FUNCTION")));
			}
			else
			{
				EnableExit(false);
				m_Progress.InitUpload(m_METreeDataSrc.GetIEFS()->CanCancel() == TRUE,&m_METreeDataSrc,pNode,vct_strFilePath);
				m_Progress.Register(&m_EnableExistNotify,EVENT_PROGRESS_TASK_FINISH);
				CQProgressServer::Exec(&m_Progress);
			}
		}
	}
}

bool CFileManager_VersionView2::IsSourceAndTargetFolderTheSame(CDragDropFireParam* pSourceParam,CQTreeNode* pTargetNode)
{
	CString strTargetFolder = pTargetNode->GetFullPath().c_str();
	strTargetFolder.MakeLower();
	strTargetFolder.Replace(_T("/"),_T("\\"));

	CString strSourceFileFullPath = pSourceParam->GetByIndex(0).c_str();
	CString strSourceFileName = PathFindFileName(strSourceFileFullPath);
	CString strSourceFolder = strSourceFileFullPath.Left(strSourceFileFullPath.GetLength() - strSourceFileName.GetLength() -1);
	strSourceFolder.MakeLower();
	strSourceFolder.Replace(_T("/"),_T("\\"));
	return strSourceFolder == strTargetFolder;
}


FORCEINLINE void CFileManager_VersionView2::OnEventTreeInsertNode(long nParam)
{
    if(nParam != NULL)
    {
        CQTreeNode* pNode = (CQTreeNode*)(nParam);
        m_METreeDataSrc.CreateNode(pNode);
    }
}
FORCEINLINE void CFileManager_VersionView2::OnEventTreeSelectNode(long nParam)
{
    if(nParam != NULL)
    {
        CCommonFileNode* pNode = (CCommonFileNode*)(nParam);
        DisplayNodeInfoToList(pNode);

		//只有當Focus未改變時才由OnEventTreeSelectNode決定DeleteButton的行為, 否則一律由EVENT_TREE_UI_SETFOCUS決定
		if(GetFocus() && GetFocus()->GetSafeHwnd() == GetCtrlHwnd(METree))
		{
			m_pDeleteButton->SetEnable(IsFolderDeleteable(pNode));
		}
    }
}
FORCEINLINE void CFileManager_VersionView2::OnEventTreeDeleteNode(long nParam)
{
    if(nParam != NULL)
    {
        CQTreeNode* pNode = (CQTreeNode*)(nParam);
        m_METreeDataSrc.DeleteNode(pNode);
    }
}
FORCEINLINE void CFileManager_VersionView2::OnEventTreeUpdateNode(long nParam)
{
    if(nParam != NULL)
    {
        CQTreeNode* pNode = (CQTreeNode*)(nParam);
        m_METreeDataSrc.UpdateNode(pNode);
    }
}
FORCEINLINE void CFileManager_VersionView2::OnEventTreeRButtonDown(long nParam)
{
    FireUIParam<CQTreeNode> *pFireParam = (FireUIParam<CQTreeNode>*)(nParam);
    bool bRet =  ProcSubMenu(pFireParam);
}
template <class T> bool CFileManager_VersionView2::ProcSubMenu(FireUIParam<T>* pParam)
{
    using namespace QSYNC_LIB;
    bool bRet = false;
    if(pParam != NULL)
    {		
		CCommonFileNode *pFileNode = static_cast<CCommonFileNode*>(pParam->m_pData);
		m_METreeSubMenu.SetFolderNode(pFileNode);     
		m_QCommonControlBase_METree.SetSubMenu(&m_METreeSubMenu);
        for(int i =0;i < m_METreeSubMenu.GetMenuItemCount();i++)
        {
            TSTRING strDisplay,strNotifyID;
            m_METreeSubMenu.GetMenuItem(i,strDisplay,strNotifyID);
            m_QCommonControlBase_METree.Register(m_pMEEventHandler,strNotifyID);
        }

        m_METreeCtrl.ClientToScreen( &(pParam->m_Point));
        QPoint ptPoint;
        ptPoint.m_nPosX = pParam->m_Point.x;
        ptPoint.m_nPosY = pParam->m_Point.y;	
        m_QCommonControlBase_METree.Event(EVENT_SUBMENU,(long)&ptPoint);	
        bRet = true;
    }
    return bRet;
}
FORCEINLINE void CFileManager_VersionView2::OnEventListRButtonDown(long nParam)
{
    vector<FireListDataParameter*>*  pvctFireData = (vector<FireListDataParameter*>*)nParam;

    CPoint point;
    if(!pvctFireData->empty())
    {
        vector<FireListDataParameter*>::iterator iter = pvctFireData->begin();
        point = (*iter)->point;

        CRect rect;
        m_MEListCtrl.GetWindowRect(&rect);

        point.x += rect.left;
        point.y += rect.top;
        QPoint ptPoint;
        ptPoint.m_nPosX = point.x;
        ptPoint.m_nPosY = point.y;

        m_QCommonControlBase_MEList.SetSubMenu(&m_MEListSubMenu);
		m_MEListSubMenu.SetFileSelectedCount(pvctFireData->size());
        for(int i =0;i < m_MEListSubMenu.GetMenuItemCount();i++)
        {
            TSTRING strDisplay,strNotifyID;
            m_MEListSubMenu.GetMenuItem(i,strDisplay,strNotifyID);
            m_QCommonControlBase_MEList.Register(m_pMEEventHandler,strNotifyID);
        }
        if(m_MEListSubMenu.GetMenuItemCount() > 0)
            m_QCommonControlBase_MEList.Event(EVENT_SUBMENU,(long)&ptPoint);
    }
}
FORCEINLINE void CFileManager_VersionView2::OnEventListHeaderButtonHit(long nParam)
{
    ColumnData* pColumnData = (ColumnData*)nParam;
    if(pColumnData)
    {
        CString szSortColumnKey = pColumnData->m_strKey.c_str();
        m_MEListDataSrc.SortByColumn(szSortColumnKey);
    }
}
FORCEINLINE void CFileManager_VersionView2::OnEventListUIDrop(long nParam)
{
    CDragDropFireParam *pParam = (CDragDropFireParam*)nParam;
    if(pParam) {
        CQTreeNode* pNode = m_METreeCtrl.GetCurrentNode();
		UploadFilesToME(pNode,pParam);
    }
}
FORCEINLINE void CFileManager_VersionView2::OnEventTreeUIDrop(long nParam)
{
    CDragDropFireParam *pParam = (CDragDropFireParam*)nParam;
    if(pParam) {
        CQTreeNode* pNode = (CQTreeNode*)pParam->GetPara();
        UploadFilesToME(pNode,pParam);
    }
}
FORCEINLINE void CFileManager_VersionView2::OnEventAfterViewSwitch(long nParam)
{
    m_bShow = true;
    TSTRING strMobileModel, strIMEI;
    SwitchViewPara* view_switch = (SwitchViewPara*)nParam;
    if(view_switch) {
        bool bIsConnect = view_switch->m_pIMainframe->IsConnected(strMobileModel, strIMEI);
        if(bIsConnect)
            OnConnect();
		m_pRefreshButton->SetEnable(m_bConnected);
    }
}
FORCEINLINE void CFileManager_VersionView2::OnEventBeforeViewSwitch(long &nParam)
{
    SwitchViewPara* view_switch = (SwitchViewPara*)nParam;
    view_switch->m_bCanSwitch = m_bCanExit;
    nParam = (long)view_switch;
    if(m_bCanExit)
        m_bShow = false;
}
FORCEINLINE void CFileManager_VersionView2::OnEventClose(long nParam)
{
	//For force switch to At command mode only
	bool bIsOnline = false;
	CommGetConnectivity(bIsOnline);

    CloseViewPara* view_close = (CloseViewPara*)nParam;
    view_close->m_bCanClose = m_bCanExit;
    nParam = (long)view_close;
}
FORCEINLINE void CFileManager_VersionView2::OnEventDeviceConnect()
{
    if(m_bShow && m_bConnected == false)
        OnConnect();
    m_METreeCtrl.EnableWindow(m_bConnected);
    m_MEListCtrl.EnableWindow(m_bConnected);
	m_pRefreshButton->SetEnable(m_bConnected);
}
FORCEINLINE void CFileManager_VersionView2::OnEventDeviceDisconnect()
{
	if(m_pInputFolderDlg)
		m_pInputFolderDlg->Disconnect(_T("IDS_FILEMANAGER_FUNCTION"));
	if(m_pRenameFileDlg)
		m_pRenameFileDlg->Disconnect(_T("IDS_FILEMANAGER_FUNCTION"));

	m_pRefreshButton->SetEnable(false);
    m_bConnected = false;
    m_MEListDataSrc.Clear();
    m_METreeCtrl.EnableWindow(m_bConnected);
    m_MEListCtrl.EnableWindow(m_bConnected);
    m_MEListCtrl.DeleteAllItems();
	m_pMETreeLogical->RefreshClear();
    m_METreeDataSrc.DeleteFileManagerInterface();

	CommDeleteInterface(m_pIPIMConfig);	
	m_pIPIMConfig = NULL;
}

void CFileManager_VersionView2::Event(const TSTRING& strEvent,long nParam)
{
    if(strEvent ==  TNotify_After_Switch) {
        OnEventAfterViewSwitch(nParam);
    } else if(strEvent ==  TNotify_Before_Switch) {
        OnEventBeforeViewSwitch(nParam);
    } else if(strEvent ==  TNotify_Close) {
        OnEventClose(nParam);
    } else if(strEvent ==  DEVICE_CONNECT) {
        OnEventDeviceConnect();
    } else if(strEvent ==  DEVICE_DISCONNECT) {
        OnEventDeviceDisconnect();
	} else if(strEvent == EVENT_BUTTON_CLICK){
		if(nParam == m_pDeleteButton->GetID()) //DeleteButton
			FuncDelete();
		else if(nParam == m_pRefreshButton->GetID()) //RefreshButton
			FuncRefresh();
	}
	 else {  }
}

CMEFileNode* CFileManager_VersionView2::ConvertListIndexToTreeNode(int nListIndex)
{
    CMEFileNode* pVar1 = NULL;
    CQListData* pFileListRowData = NULL;
    if(m_MEListDataSrc.GetDataByIndex(nListIndex,&pFileListRowData) && pFileListRowData)
        pVar1 = (CMEFileNode*)pFileListRowData->GetLPARAM();
    return pVar1;
}

// CFileManager_VersionView2 message handlers

void CFileManager_VersionView2::OnConnect()
{
    if(m_bConnected == false)
    {
		FuncRefresh();
        m_bCanExit = false;        
        m_bConnected = true;
        m_bCanExit = true;	
    }
}


int CFileManager_VersionView2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;

    CreateILocalization(&m_pILocalization);
    m_pILocalization->Register(m_pMEEventHandler, TNotify_Language_Change);
    m_pILocalization->Register(m_pPCEventHandler, TNotify_Language_Change);
    m_pMETreeLogical = new CQCommonTreeCtrl(&m_METreeCtrl, &m_METreeDataSrc);
    return 0;
}

bool CFileManager_VersionView2::DisplayNodeInfoToList(CQTreeNode* pNode, bool bForceRefresh)
{
    bool bRet = false;
    if(pNode && (bForceRefresh || m_pCurrentNode != pNode))
    {
        m_pCurrentNode = pNode;

		if(m_pMEListLogical)
			m_pMEListLogical->RefreshClear();
        //m_MEListDataSrc.Clear();
        int nSize = pNode->GetChildSize();
        if(nSize > 0)
        {
            for(int i = 0; i < nSize; i++)
            {
                CMEFileNode* pFileNode = static_cast<CMEFileNode*>(pNode->GetChild(i));
                if(pFileNode)
                {
                    bool bIsFolder = pFileNode->IsFolder();
                    if(!bIsFolder)
                        m_MEListDataSrc.DisplayFileNodeInfo(pFileNode);
                }
            }
        }
        bRet = true;
    }
    return bRet;
}

bool CFileManager_VersionView2::NewMEFolder(CQTreeNode* pNode)
{
    CString strFolder;
    bool bRet = false;
    if(pNode)
    {
        strFolder = ((CMEFileNode*)pNode)->GetFullPath().c_str();
        if(!strFolder.IsEmpty())
        {
			if(m_pInputFolderDlg)
			{
				delete m_pInputFolderDlg;
				m_pInputFolderDlg = NULL;
			}
			if(m_pInputFolderDlg == NULL)
				m_pInputFolderDlg = new CInputFolderDlg(pNode);

            INT_PTR nRet = m_pInputFolderDlg->DoModal();
            if(nRet == IDOK)
            {
                CString m_strFolder = m_pInputFolderDlg->GetInputString();
                strFolder = strFolder + _T("/") + m_strFolder;

                if( m_METreeDataSrc.NewFolder(strFolder)) //Insert New Folder to GUI
                {
                    CMEFileNode* pFNode = new CMEFileNode(static_cast<CMEFileNode*>(pNode));
                    pFNode->SetName(m_strFolder);
                    pFNode->SetDisplayName(pFNode->GetName().c_str());
                    pFNode->m_bExpanded = true;
                    pFNode->SetNodeAsFolder();
                    m_METreeDataSrc.CreateNode((CQTreeNode*)pFNode);
                    bRet = true;
                }
				else
				{
					bRet = false;
					ShowMessage(GetSafeHwnd(),_T("IDS_InsertFail"), _T("IDS_FILEMANAGER_FUNCTION"));
				}
            }
        }
    }
    return bRet;
}

bool CFileManager_VersionView2::DelMETreeFolder(CQTreeNode* pNode)
{
    bool bRet = false;
    if(pNode)
		bRet = m_METreeDataSrc.RecursiveDelNode(pNode, true);
    return bRet;
}

bool CFileManager_VersionView2::DelMEListSelectedFile()
{
	bool bDeleteSuccess = false;
	vector<FireListDataParameter> vectSelected;
	m_pMEListLogical->GetSelectedItem(vectSelected);

	if(vectSelected.empty())
		bDeleteSuccess = true;
	else
	{
		CWaitCursor wait;
		CQTreeNode* pCurrFolder = m_METreeCtrl.GetCurrentNode();
		for(vector<FireListDataParameter>::iterator Iter = vectSelected.begin();Iter != vectSelected.end();Iter++)
		{			
			CBasicData *pData = Iter->pListData->GetValueByKey(strFILE_FULL_LISTKEY);
			bDeleteSuccess = m_METreeDataSrc.DelFileByFilePath(pData->GetTextString().c_str());							 
		}
		if(bDeleteSuccess)
			DisplayNodeInfoToList(m_pCurrentNode,true);
	}
	return bDeleteSuccess;
}

CString CFileManager_VersionView2::Getfolder() 
{
    TCHAR szFolder[MAX_PATH] = {0};
    bool bRes = Win32Utility::GetShellFolder(szFolder,CSIDL_PERSONAL,_T("QSyncer"));

    TSTRING strFolderPath;
    Win32Utility::GetFolder(strFolderPath, m_pILocalization->GetString(_T("IDS_FileMgr_SELECT_DOWNLOAD_FOLDER")).c_str(), m_hWnd, NULL, szFolder);
    return strFolderPath.c_str();
}

bool CFileManager_VersionView2::IsFolderDeleteable(CCommonFileNode* pNode) const
{
	bool bDeleteable;
	if(pNode->IsRoot())
		bDeleteable = false;
	else
		bDeleteable = (pNode->GetFileAttr() & FILE_ATTRIBUTE_READONLY) == 0 ;
	return bDeleteable;
}

void CFileManager_VersionView2::OnRefreshUILocalization() {
    ColumnData cd;
    m_MEListDataSrc.GetColumnByIndex(0, &cd);
    m_MEListDataSrc.UpdateColumnByKey(cd.m_strKey.c_str(), m_pILocalization->GetString(_T("IDS_FileMgr_List_FileName")).c_str());
    m_MEListCtrl.SetColumnWidth(0, cd.m_nWidth);
    m_MEListDataSrc.GetColumnByIndex(1, &cd);
    m_MEListDataSrc.UpdateColumnByKey(cd.m_strKey.c_str(),m_pILocalization->GetString(_T("IDS_FileMgr_List_FileAttr")).c_str());
    m_MEListCtrl.SetColumnWidth(1, cd.m_nWidth);
    m_MEListDataSrc.GetColumnByIndex(2, &cd);
    m_MEListDataSrc.UpdateColumnByKey(cd.m_strKey.c_str(), m_pILocalization->GetString(_T("IDS_FileMgr_List_FileSize")).c_str());
    m_MEListCtrl.SetColumnWidth(2, cd.m_nWidth);
    m_MEListDataSrc.GetColumnByIndex(3, &cd);
    m_MEListDataSrc.UpdateColumnByKey(cd.m_strKey.c_str(), m_pILocalization->GetString(_T("IDS_FileMgr_List_FileMDate")).c_str());
    m_MEListCtrl.SetColumnWidth(3, cd.m_nWidth);

    LVCOLUMN lvCol0 = {0};
    LVCOLUMN lvCol1 = {0};
    LVCOLUMN lvCol2 = {0};
    LVCOLUMN lvCol3 = {0};
    lvCol0.mask = lvCol1.mask = lvCol2.mask = lvCol3.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_IMAGE;
    lvCol0.cchTextMax = lvCol1.cchTextMax = lvCol2.cchTextMax = lvCol3.cchTextMax = MAX_PATH;
    m_PCListCtrl.GetColumn(0, &lvCol0);
    m_PCListCtrl.GetColumn(1, &lvCol1);
    m_PCListCtrl.GetColumn(2, &lvCol2);
    m_PCListCtrl.GetColumn(3, &lvCol3);
    CString strCol0(m_pILocalization->GetString(_T("IDS_FileMgr_List_FileName")).c_str());
    CString strCol1(m_pILocalization->GetString(_T("IDS_FileMgr_List_FileAttr")).c_str());
    CString strCol2(m_pILocalization->GetString(_T("IDS_FileMgr_List_FileSize")).c_str());
    CString strCol3(m_pILocalization->GetString(_T("IDS_FileMgr_List_FileMDate")).c_str());
    lvCol0.pszText = (LPTSTR)(LPCTSTR)strCol0;
    lvCol1.pszText = (LPTSTR)(LPCTSTR)strCol1;
    lvCol2.pszText = (LPTSTR)(LPCTSTR)strCol2;
    lvCol3.pszText = (LPTSTR)(LPCTSTR)strCol3;
    m_PCListCtrl.SetColumn(0, &lvCol0);
    m_PCListCtrl.SetColumn(1, &lvCol1);
    m_PCListCtrl.SetColumn(2, &lvCol2);
    m_PCListCtrl.SetColumn(3, &lvCol3);
}
bool CFileManager_VersionView2::RenameListFile()
{
	bool bRes = false;
	vector<FireListDataParameter> vectSelected;
	m_pMEListLogical->GetSelectedItem(vectSelected);

	if(vectSelected.size() == 1)
	{
		//CRenameDlg	RenameFileDlg(m_METreeCtrl.GetCurrentNode());
		vector<FireListDataParameter>::iterator Iter = vectSelected.begin();

		CMEFileNode* pTreeNode = (CMEFileNode*)Iter->pListData->GetLPARAM();
		CBasicData *pData = Iter->pListData->GetValueByKey(strFILE_NAME_LISTKEY);
		CString strTest = pData->GetTextString().c_str();
		
		if(m_pRenameFileDlg)
		{
			delete m_pRenameFileDlg;
			m_pRenameFileDlg = NULL;
		}
		if(m_pRenameFileDlg == NULL)
			m_pRenameFileDlg = new CRenameDlg(m_METreeCtrl.GetCurrentNode(),strTest);

		//RenameFileDlg.SetInitialString(strTest);
		INT_PTR nRet = m_pRenameFileDlg->DoModal();
		if(nRet == IDOK)
		{
			CString m_strNewFileName = m_pRenameFileDlg->GetInputString();
			if(pTreeNode && pTreeNode->GetParent() && OKRename(pTreeNode->GetParent()->GetFullPath().c_str(),pTreeNode,m_strNewFileName))
			{
				bRes = true;
				//refresh ListCtrl
				CBasicData NameData((LPCTSTR)m_strNewFileName);
				NameData.m_Alignment = CBasicData::Alignment_Left;
				Iter->pListData->SetData(strFILE_NAME_LISTKEY,NameData);

				CBasicData FullpathData(pTreeNode->GetFullPath());
				FullpathData.m_Alignment = CBasicData::Alignment_Left;
				Iter->pListData->SetData(strFILE_FULL_LISTKEY, FullpathData);

				m_MEListCtrl.Invalidate();
			}
		}
		else if(nRet == IDCANCEL)
			bRes = true;
	}
	return bRes;
}

bool CFileManager_VersionView2::OKRename(const CString& strFolder,CMEFileNode *pNode,const CString& strNewFileName)
{
	//Rename file here :
	CWaitCursor wait;
	return m_METreeDataSrc.RenameFile(strFolder,pNode,strNewFileName);
}

void CFileManager_VersionView2::CFileManagerEventMEHandler::Event(const TSTRING& strEvent, long nParam) {
    if(strEvent == EVENT_TREE_INSERT_NODE)
    {
        m_pView->OnEventTreeInsertNode(nParam);
    }
    else if(strEvent == EVENT_TREE_SELECT_NODE)
    {
		m_pView->SetCurrentWnd(METree);
        m_pView->OnEventTreeSelectNode(nParam);
    }
    else if(strEvent == EVENT_TREE_DELETE_NODE)
    {
		CQTreeNode* pNode = m_pView->m_METreeCtrl.GetCurrentNode();
		CCommonFileNode* pCommonFileNode = static_cast<CCommonFileNode*>(pNode);
		if(pCommonFileNode 
			&& m_pView->IsFolderDeleteable(pCommonFileNode) 
			&& 	ShowMessage(m_pView->GetSafeHwnd(),_T("IDS_FileMgr_DELETE_PROMPT"), _T("IDS_FILEMANAGER_FUNCTION"), QMB_YESNO) == QIDYES
			)
		{
			bool bDeleteSucceed = false;
			bDeleteSucceed = m_pView->DelMETreeFolder(pCommonFileNode);
			if (bDeleteSucceed == false)
				ShowMessage(m_pView->GetSafeHwnd(),_T("IDS_FileMgr_Delete_Fail"),_T("IDS_FILEMANAGER_FUNCTION"));
		}
    }
    else if(strEvent == EVENT_TREE_UPDATE_NODE)
    {
        m_pView->OnEventTreeUpdateNode(nParam);
    }
    //  SubMenu -- R Button down ---------------------------------------------------------------------------------
    else if(strEvent == EVENT_TREE_RBUTTONDOWN_NODE)
    {
        m_pView->OnEventTreeRButtonDown(nParam);
    }
    else if(strEvent == EVENT_LIST_UI_RBTNDOWN)
    {
        m_pView->OnEventListRButtonDown(nParam);
    }
    else if(strEvent == EVENT_LIST_UI_HEADER_BUTTON_HIT)
    {
        m_pView->OnEventListHeaderButtonHit(nParam);
    }
    // List Drop ---------------------------------------------------------------------------------
    else if(strEvent == EVENT_LIST_UI_DROP)
    {
        m_pView->OnEventListUIDrop(nParam);

		m_pView->SetDragging(false);
    }
    // Tree Drop ---------------------------------------------------------------------------------
    else if(strEvent == EVENT_TREE_UI_DROP)
    {
        m_pView->OnEventTreeUIDrop(nParam);
		m_pView->SetDragging(false);
    }
    //---------------------------------------------------------------------------------------
    // Sub Menu event process begin 
    else if(strEvent == EVENT_FILE_SUBMENU_INSERT_NODE)
    {
        FireUIParam<CQTreeNode> *pFireParam = (FireUIParam<CQTreeNode>*)(nParam);
        CQTreeNode* pNode = (CQTreeNode*)m_pView->m_METreeSubMenu.GetFolderNode();
        bool bRet =  m_pView->NewMEFolder(pNode);
    }
    else if(strEvent == EVENT_FILE_SUBMENU_DELETE_NODE)
    {
		CQTreeNode* pTreeNode = (CQTreeNode*)m_pView->m_METreeSubMenu.GetFolderNode();
		assert(pTreeNode);
		if(pTreeNode &&
			ShowMessage(m_pView->GetSafeHwnd(),_T("IDS_FileMgr_DELETE_PROMPT"), _T("IDS_FILEMANAGER_FUNCTION"), QMB_YESNO) == QIDYES)
		{
			bool bDeleteSucceed = false;
			bDeleteSucceed = m_pView->DelMETreeFolder(pTreeNode);
			if (bDeleteSucceed == false)
				ShowMessage(m_pView->GetSafeHwnd(),_T("IDS_FileMgr_Delete_Fail"),_T("IDS_FILEMANAGER_FUNCTION"));
		}
    }
    else if(strEvent == EVENT_ME_LIST_SUBMENU_DOWNLOAD_NODE)
    {
        CString strPath = m_pView->m_PCTreeCtrl.GetCurrentFolderPath();
        vector<FireListDataParameter> vectSelected;
        m_pView->m_pMEListLogical->GetSelectedItem(vectSelected);
        if(!vectSelected.empty())		
        {
            //1. Choose folder
            //CString strFolder = m_pView->Getfolder();
            CString strFolder = strPath;
            //2. Prepare files
            if(!strFolder.IsEmpty())
            {
                vector<CString> vct_strFilePath;
                for(vector<FireListDataParameter>::iterator Iter = vectSelected.begin();Iter != vectSelected.end();Iter++)
                {
                    CBasicData *pData = Iter->pListData->GetValueByKey(strFILE_FULL_LISTKEY);
                    vct_strFilePath.push_back(pData->GetTextString().c_str());
                }//for end

                m_pView->DownloadFilesIntoPC(strFolder,vct_strFilePath);
            }
        }
    }
    else if(strEvent == EVENT_ME_LIST_SUBMENU_DELETE_NODE)
    {
		assert(m_pView);
		if(ShowMessage(m_pView->GetSafeHwnd(),_T("IDS_FileMgr_DELETE_PROMPT"), _T("IDS_FILEMANAGER_FUNCTION"), QMB_YESNO) == QIDYES)
			if(m_pView->DelMEListSelectedFile() == false)
				ShowMessage(m_pView->GetSafeHwnd(),_T("IDS_FileMgr_Delete_Fail"),_T("IDS_FILEMANAGER_FUNCTION"));
	}
	else if(strEvent == EVENT_ME_LIST_SUBMENU_RENAME_NODE)
	{
		bool bRes = m_pView->RenameListFile();
		if(bRes == false)
			ShowMessage(m_pView->GetSafeHwnd(),_T("IDS_FileMgr_LIST_RENAME_FAIL"), _T("IDS_FILEMANAGER_FUNCTION"));
	}
	else if(strEvent == EVENT_LIST_UI_DRAG_OVER)
	{
		//CDragDropFireParam *pFireParam = (CDragDropFireParam*)(nParam);
		//if(m_pView->IsDragging())
		//	pFireParam->SetEffect(DROPEFFECT_NONE);
	}
    else if(strEvent == EVENT_LIST_UI_DRAG_BEGIN)
    {
		m_pView->SetDragging(true);
        CDragDropFireParam *pFireParam = (CDragDropFireParam*)(nParam);
        pFireParam->SetEffect(DROPEFFECT_COPY);

        if(m_pView->m_pMEListLogical)
        {
            vector<FireListDataParameter> vectSelected;
            m_pView->m_pMEListLogical->GetSelectedItem(vectSelected);
            for(vector<FireListDataParameter>::iterator Iter = vectSelected.begin();Iter != vectSelected.end();Iter++)
            {
                CBasicData *pData = Iter->pListData->GetValueByKey(strFILE_FULL_LISTKEY);				
                pFireParam->Insert(pData->GetTextString());
            }
        }
    }
    //---------------------------------------------------------------------------------------
    // Sub Menu event process end	
    else if(strEvent ==  EVENT_LIST_UI_EDIT_BEGIN)
    {
        if(m_pView->m_pCurrentNode)
        {
            CEditParam* peditParam = (CEditParam*)nParam;
            CMEFileNode* pNode = m_pView->ConvertListIndexToTreeNode(peditParam->GetItemIndex());
            if(pNode)
            {
                peditParam->SetEditable(m_pView->m_METreeDataSrc.CanRenameFile(m_pView->m_pCurrentNode->GetFullPath().c_str(),pNode));
            }
        }
    }
    else if(strEvent ==  EVENT_LIST_UI_EDIT_END)
    {
        if(m_pView->m_pCurrentNode)
        {
            CEditParam* peditParam = (CEditParam*)nParam;
            CMEFileNode* pNode = m_pView->ConvertListIndexToTreeNode(peditParam->GetItemIndex());
            if(pNode)
            {
                peditParam->SetFinishEdit(m_pView->m_METreeDataSrc.RenameFile(m_pView->m_pCurrentNode->GetFullPath().c_str(),pNode,peditParam->GetEditString().c_str()));
                if(peditParam->GetFinishEdit())
                    m_pView->DisplayNodeInfoToList(m_pView->m_pCurrentNode,true);
            }
        }
    }
    else if(strEvent ==  EVENT_TREE_UI_EDIT_BEGIN)
    {
        if(m_pView->m_pCurrentNode)
        {
            CTreeEditParam* peditParam = (CTreeEditParam*)nParam;
            CMEFileNode* pNode = static_cast<CMEFileNode*>(peditParam->GetNode());
            if(pNode)
            {
                peditParam->SetEditable(m_pView->m_METreeDataSrc.CanRenameFolder(m_pView->m_pCurrentNode->GetFullPath().c_str(),pNode));
            }
        }
    }
    else if(strEvent ==  EVENT_TREE_UI_EDIT_END)
    {
        if(m_pView->m_pCurrentNode)
        {
            CTreeEditParam* peditParam = (CTreeEditParam*)nParam;
            CMEFileNode* pNode = static_cast<CMEFileNode*>(peditParam->GetNode());
            if(pNode)
            {
                peditParam->SetFinishEdit(m_pView->m_METreeDataSrc.RenameFolder(pNode,peditParam->GetEditString().c_str()));
            }
        }
    }
    else if(strEvent == TNotify_Language_Change) 
	{
        m_pView->OnRefreshUILocalization();
    }
    else if(strEvent == EVENT_LIST_UI_ITEMCHANGED)
	{
		//只有當Focus未改變時才由EVENT_LIST_UI_ITEMCHANGED決定DeleteButton的行為, 否則一律由EVENT_LIST_UI_SETFOCUS決定
		if(GetFocus() && GetFocus()->GetSafeHwnd() == m_pView->GetCtrlHwnd(MEList))
		{
			if( m_pView->m_pMEListLogical)	
			{
				vector<FireListDataParameter> vectSelected;
				m_pView->m_pMEListLogical->GetSelectedItem(vectSelected);
				m_pView->m_pDeleteButton->SetEnable(vectSelected.empty() == false);
			}
		}
    }
	else if(strEvent == EVENT_DRAG_END)
	{
		m_pView->SetDragging(false);
	}
	else if(strEvent == EVENT_TREE_UI_KILLFOCUS)
	{
		if (m_pView->m_pDeleteButton)
			m_pView->m_pDeleteButton->SetEnable(false);
	}
	else if(strEvent == EVENT_TREE_UI_SETFOCUS)
	{
		m_pView->SetCurrentWnd(METree);
		CQTreeNode* pNode = m_pView->m_METreeCtrl.GetCurrentNode();
		CCommonFileNode* pCommonFileNode = static_cast<CCommonFileNode*>(pNode);
		if(pCommonFileNode)
		{
			m_pView->m_pDeleteButton->SetEnable(m_pView->IsFolderDeleteable(pCommonFileNode));
		}
	}
	else if(strEvent == EVENT_LIST_UI_KILLFOCUS)
	{
		if (m_pView->m_pDeleteButton)
			m_pView->m_pDeleteButton->SetEnable(false);
	}
	else if(strEvent == EVENT_LIST_UI_SETFOCUS)
	{
		m_pView->SetCurrentWnd(MEList);
		vector<FireListDataParameter> vectSelected;
		m_pView->m_pMEListLogical->GetSelectedItem(vectSelected);
		m_pView->m_pDeleteButton->SetEnable(vectSelected.empty() == false);
	}
	else if(strEvent == EVENT_LIST_UI_DELETE_ROW)
	{
		assert(m_pView);
		if(ShowMessage(m_pView->GetSafeHwnd(),_T("IDS_FileMgr_DELETE_PROMPT"), _T("IDS_FILEMANAGER_FUNCTION"), QMB_YESNO) == QIDYES)
			if(m_pView->DelMEListSelectedFile() == false)
				ShowMessage(m_pView->GetSafeHwnd(),_T("IDS_FileMgr_Delete_Fail"),_T("IDS_FILEMANAGER_FUNCTION"));
	}
}

void CFileManager_VersionView2::CFileManagerEventPCHandler::Event(const TSTRING& strEvent, long nParam) {
}


#pragma warning(pop)