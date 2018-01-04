// ChildFrm.cpp : implementation of the CChildFrame class
//
#include "stdafx.h"
#include "NVItemUI.h"
#include "NVItemTreeView.h"
#include "ChildFrm.h"
#include "NVItemDataView.h"
#include "NVItemListView.h"
#include "NVItemUIDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
END_MESSAGE_MAP()


// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{
}
void CChildFrame::ActivateFrame(int nCmdShow)   
{   
	//   TODO:   Modify   this   function   to   change   how   the   frame   is   activated.   
	nCmdShow   =   SW_SHOWMAXIMIZED;   
	CMDIChildWnd::ActivateFrame(nCmdShow);   
}  

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpCreateStruct, CCreateContext* pContext)
{
	CRect rect;
	GetClientRect(&rect);
	m_wndSplitter1.CreateStatic(this,1,2);

	m_wndSplitter1.CreateView(0,0,RUNTIME_CLASS(CNVItemTreeView),CSize(rect.Width()/3,rect.Height()),pContext);//父窗口的创建参数

	m_wndSplitter2.CreateStatic(&m_wndSplitter1,2,1,WS_CHILD | WS_VISIBLE | WS_BORDER,m_wndSplitter1.IdFromRowCol(0,1));
	m_wndSplitter2.CreateView(0,0,RUNTIME_CLASS(CNVItemDataView),CSize(rect.Width()-rect.Width()/3,rect.Height()-rect.Height()/3),pContext);
	m_wndSplitter2.CreateView(1,0,RUNTIME_CLASS(CNVItemListView),CSize(rect.Width()-rect.Width()/3,rect.Height()/3),pContext);
	
	//CMainFrame * pMain = (CMainFrame *)this->GetMDIFrame();

	//return pMain->addPortToToolBar();
	return TRUE;
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame message handlers



void CChildFrame::OnComboChange(void)
{
	//CNVItemUIDoc *doc = (CNVItemUIDoc*) this->GetActiveDocument();
	//int com = 4;
	////CString COM;
	////m_ComboCom.GetLBText(m_ComboCom.GetCurSel(),COM);
	////COM.Delete(0,4);
	////sscanf_s(COM,"%d",&com);
	//doc->SetCom();
}

