/***************************************************************************
 *
 *                               QSyncer 2.0 Project
 *
 *	             Copyright (c) 2007 Qisda Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   PhonebookTreeDlg.cpp  $
 *  $Revision:   1.8  $
 *  $Date:   Nov 13 2007 10:05:48  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Phonebook_version2/PhonebookTreeDlg.cpp.-arc  $
   
      Rev 1.8   Nov 13 2007 10:05:48   Alan Tu
   update UI
 *
 *
 ***************************************************************************/

#include "stdafx.h"
#include "PhonebookTreeDlg.h"
#include "..\..\CommLayer\MobileFinder\MobileFinderInterface.h"
#include "../../Components/UIFramework/interface/CommonUIParameter.h"

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

// CPhonebookTreeDlg dialog

IMPLEMENT_DYNAMIC(CPhonebookTreeDlg, Win32DoubleBufferDialogWidge)

CPhonebookTreeDlg::CPhonebookTreeDlg(CWnd* pParent /*=NULL*/)
	: Win32DoubleBufferDialogWidge(CPhonebookTreeDlg::IDD, pParent),m_TreeLogic(&m_TreeCtrl,&m_TreeDatasource)
{

}

CPhonebookTreeDlg::~CPhonebookTreeDlg()
{
}

void CPhonebookTreeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PHONEBOOK_TREE, m_TreeCtrl);
}


BEGIN_MESSAGE_MAP(CPhonebookTreeDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CPhonebookTreeDlg message handlers

void CPhonebookTreeDlg::OnClose()
{
	CDialog::OnClose();
}
/*
void CPhonebookTreeDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(::IsWindow(m_TreeCtrl.GetSafeHwnd()))
	{
		CRect rcArea;
		GetClientRect(&rcArea);
		m_TreeCtrl.MoveWindow(&rcArea);
	}
}*/

BOOL  CPhonebookTreeDlg::OnInitDialog()
{	
	CRect rcArea;
	GetClientRect(&rcArea);
	BOOL bRes = m_TreeCtrl.Create(WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES, rcArea, this, IDC_PHONEBOOK_TREE);
	assert(bRes);

	m_TreeLogic.Register(this,EVENT_TREE_UI_DRAG_BEGIN);
	m_TreeLogic.Register(this,EVENT_TREE_SELECT_NODE);
	m_TreeLogic.Register(this, EVENT_TREE_RBUTTONDOWN_NODE);

	m_TreeDatasource.CreateNode(NULL);
	return CDialog::OnInitDialog();
}

void CPhonebookTreeDlg::OnDestroy()
{
	m_TreeCtrl.DestroyWindow();
	CDialog::OnDestroy();	
}

void CPhonebookTreeDlg::Event(const TSTRING& strEvent,long nParam)
{
	if(EVENT_TREE_UI_DRAG_BEGIN == strEvent)
	{
		CDragDropFireParam* pDragFireParam = (CDragDropFireParam*)nParam;
		if(pDragFireParam)
		{
			CQTreeNode* pNode = m_TreeLogic.GetCurrentNode();
			if(pNode)
			{
				pDragFireParam->Insert(pNode->GetName().c_str());
				pDragFireParam->SetEffect(DROPEFFECT_COPY);
			}
		}
	}
	else if(strEvent == EVENT_TREE_SELECT_NODE)
	{
		if(nParam != NULL)
		{
			CQTreeNode* pNode = (CQTreeNode*)(nParam);
		}
	}
	else if(strEvent == EVENT_TREE_RBUTTONDOWN_NODE)
	{
		using namespace QSYNC_LIB;
		FireUIParam<CQTreeNode> *pParam = (FireUIParam<CQTreeNode>*)(nParam);
		if(pParam != NULL)
		{
			m_QCommonControlBase_Tree.SetSubMenu(&m_submenu);
			for(int i =0;i < m_submenu.GetMenuItemCount();i++)
			{
				TSTRING strDisplay,strNotifyID;
				m_submenu.GetMenuItem(i,strDisplay,strNotifyID);
				m_QCommonControlBase_Tree.Register(this,strNotifyID);
			}

			ClientToScreen( &(pParam->m_Point));
			QPoint ptPoint;
			ptPoint.m_nPosX = pParam->m_Point.x;
			ptPoint.m_nPosY = pParam->m_Point.y;	
			m_QCommonControlBase_Tree.Event(EVENT_SUBMENU,(long)&ptPoint);			
		}
	}
	else if(strEvent == EVENT_PHONEBOOK_SUBMENU_INSERT)
	{
		assert(0);
	}
	else if(strEvent == EVENT_PHONEBOOK_SUBMENU_DELETE)
	{
		CQTreeNode* pNode = m_TreeCtrl.GetCurrentNode();
		m_TreeDatasource.DeleteNode(pNode);
	}
	else if(strEvent == EVENT_PHONEBOOK_SUBMENU_EDIT)
	{
		assert(0);
	}
}

BOOL CPhonebookTreeDlg::ShowWindow(int nShow)
{
	if(nShow == SW_HIDE)
	{//Disconnected
		m_TreeDatasource.Disconnect();
	}
	else if(nShow == SW_SHOW)
	{
		m_TreeDatasource.Connect();
	}
		
	return CDialog::ShowWindow(nShow);
}

BOOL CPhonebookTreeDlg::Create(UINT nIDTemplate, CWnd* pParentWnd)
{
	return CDialog::Create(nIDTemplate,pParentWnd);
}

#pragma warning(pop)