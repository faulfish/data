// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "NVItemUI.h"

#include "MainFrm.h"
#include "Resource.h"
#include "NVItemUIDoc.h"
#include "../../QLib_Export/QLib_export.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_CBN_SELCHANGE(ID_KOMBO_COM,OnComboChange)
	ON_CBN_DROPDOWN(ID_KOMBO_COM,OnCbnDropdownCombo)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	int index = 0;
	RECT rect;
	while(m_wndToolBar.GetItemID(index)!=ID_KOMBO_COM)
		index++;
	m_wndToolBar.SetButtonInfo(index, ID_KOMBO_COM, TBBS_SEPARATOR, 70);
	m_wndToolBar.GetItemRect(index, &rect);

	rect.top +=2;
	rect.bottom = rect.top+80;
	rect.left +=85;
	rect.right +=85;

	if(!m_ComboCom.Create(WS_CHILD|WS_VISIBLE| CBS_AUTOHSCROLL| CBS_DROPDOWNLIST | CBS_HASSTRINGS , rect, 
		&m_wndToolBar, ID_KOMBO_COM))
	{  
		TRACE0("Failed to create combo-box\n");
		return -1;
	}
	m_ComboCom.ShowWindow(SW_SHOW);
	m_wndToolBar.GetToolBarCtrl().HideButton(index,FALSE);
	RECT rect2;
	index = 0;
	while(m_wndToolBar.GetItemID(index)!=ID_EDIT_COM)
		index++;
	m_wndToolBar.SetButtonInfo(index, ID_EDIT_COM, TBBS_SEPARATOR, 105);
	m_wndToolBar.GetItemRect(index, &rect2);
	rect2.top+=5;
	m_comPort.Create(WS_CHILD |WS_DISABLED| WS_VISIBLE , rect2, &m_wndToolBar,ID_EDIT_COM);
	m_comPort.Clear();
	m_comPort.SetWindowText("MobileComPort:");
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnCbnDropdownCombo(void)
{
	m_ComboCom.ResetContent();
	unsigned short nPorts = 20;
	unsigned short Ports[20] = {0};
	if(QL_GetDiagPort(&nPorts, Ports) && nPorts > 0) {
		for(int i = 0 ; i < nPorts; ++i) {
			CString strPort;
			strPort.Format(_T("COM%d"), Ports[i]);
			m_ComboCom.AddString((LPCTSTR)strPort);
		}
		if((m_ComboCom.GetCurSel() == -1) && (m_ComboCom.GetCount() > 0))
			m_ComboCom.SetCurSel(0);
	}
}

void CMainFrame::OnComboChange(void)
{
	m_ComboCom.GetLBText(m_ComboCom.GetCurSel(),COM);
}


CString CMainFrame::getComPort(void)
{
	if(m_ComboCom.GetCurSel() != -1)
		m_ComboCom.GetLBText(m_ComboCom.GetCurSel(), COM);
	else 
		COM = _T("");

	return COM;
}

