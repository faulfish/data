// Calendar_V2View.cpp : implementation of the CCalendar_V2View class
//

#include "stdafx.h"
#include "Calendar_V2View.h"
#include "..\..\CommLayer\MobileFinder\MobileFinderInterface.h"
#include "../../Components/UIFramework/LayoutManager/Win32Widge.h"
#include "ListPage.h"

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

IMPLEMENT_DYNCREATE(CCalendar_V2View, Win32DoubleBufferFormViewWidge)

BEGIN_MESSAGE_MAP(CCalendar_V2View, Win32DoubleBufferFormViewWidge)
	//{{AFX_MSG_MAP(CCalendar_V2View)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

extern class NotifyReceiver g_NotifyReceiver;

CCalendar_V2View::CCalendar_V2View(): Win32DoubleBufferFormViewWidge(CCalendar_V2View::IDD), 
m_bSwitchView(true),m_bShow(false),m_bConnected(false),m_pPage(NULL), m_pBasicLayout(NULL)
{
	TRACE_FILE_LINE_INFO();

	g_NotifyReceiver.m_pINotify = this;

	m_bInit = FALSE;
	m_nRightCx = -1;

	m_pPage = new CListPage;
	ASSERT(m_pPage != NULL);

	m_pPage->m_QDataCtrl.Register(this, CALENDAR_V2VIEW_SWITCHVIEW_ON);
	m_pPage->m_QDataCtrl.Register(this, CALENDAR_V2VIEW_SWITCHVIEW_OFF);
}

CCalendar_V2View::~CCalendar_V2View()
{
	TRACE_FILE_LINE_INFO();

	m_pPage->m_QDataCtrl.Unregister(this, CALENDAR_V2VIEW_SWITCHVIEW_ON);
	m_pPage->m_QDataCtrl.Unregister(this, CALENDAR_V2VIEW_SWITCHVIEW_OFF);
	if(m_pPage != NULL)
	{
		delete m_pPage;
		m_pPage = NULL;
	}

}

void CCalendar_V2View::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CCalendar_V2View::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~WS_BORDER;

	return CFormView::PreCreateWindow(cs);
}

void CCalendar_V2View::OnInitialUpdate()
{
	TRACE_FILE_LINE_INFO();

	CFormView::OnInitialUpdate();

	m_pBasicLayout = new QSYNC_LIB::BasicLayout<HDC>;

	CRect rect;
	GetClientRect(&rect);

	if(m_pPage->Create(IDD_List_Page, this))
	{
		m_pPage->ShowWindow(SW_SHOW);
		m_pBasicLayout->AddWidget(new WIN32WindowWidge(m_pPage));
	}
	SetLayout(m_pBasicLayout);
}

void CCalendar_V2View::AssertValid() const
{
	CFormView::AssertValid();
}

void CCalendar_V2View::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

void CCalendar_V2View::Event(const TSTRING& strEvent,long nParam)
{	
	if(strEvent ==  DEVICE_CONNECT)
	{		
		if(m_bShow && m_bConnected == false)
		{
			TCHAR strApp[MAX_RET_STRING_LENGTH] = {0};
			TCHAR pMobileModel[MAX_RET_STRING_LENGTH] = {0};
			TCHAR pIMEI[MAX_RET_STRING_LENGTH] = {0};
			TCHAR pDisplayName[MAX_RET_STRING_LENGTH] = {0};

			CommGetMobileInfo(strApp, pMobileModel, pIMEI, pDisplayName);

			((CListPage*)m_pPage)->m_bIsConnectSuccess = true;
			((CListPage*)m_pPage)->m_QDataCtrl.m_szMobileModel = pMobileModel;
			((CListPage*)m_pPage)->m_QDataCtrl.m_szIMEI = pIMEI;
			((CListPage*)m_pPage)->SetConnect(true);

			m_bConnected = true;
		}
	}
	else if(strEvent ==  DEVICE_DISCONNECT)
	{
		m_bConnected = false;

		((CListPage*)m_pPage)->m_bIsConnectSuccess = false;
		((CListPage*)m_pPage)->m_QDataCtrl.m_szMobileModel.clear();
		((CListPage*)m_pPage)->m_QDataCtrl.m_szIMEI.clear();
		((CListPage*)m_pPage)->SetConnect(false);
	}
	else if(strEvent ==  TNotify_Before_Switch)
	{
		using namespace QSYNC_LIB;	

		SwitchViewPara* view_switch = (SwitchViewPara*)nParam;
		view_switch->m_bCanSwitch = m_bSwitchView;
		nParam = (long)view_switch;
		if(view_switch->m_bCanSwitch)
			m_bShow = false;
	}
	else if(strEvent ==  TNotify_After_Switch)
	{
		m_bShow = true;
		TSTRING strMobileModel, strIMEI;
		SwitchViewPara* view_switch = (SwitchViewPara*)nParam;
		if(view_switch){
			bool bIsConnect = view_switch->m_pIMainframe->IsConnected(strMobileModel, strIMEI);
			if(bIsConnect)
			{
				((CListPage*)m_pPage)->m_bIsConnectSuccess = true;
				((CListPage*)m_pPage)->m_QDataCtrl.m_szMobileModel = strMobileModel;
				((CListPage*)m_pPage)->m_QDataCtrl.m_szIMEI = strIMEI;
				if(((CListPage*)m_pPage)->m_bConnected == false)
					((CListPage*)m_pPage)->SetConnect(true);
			}
		}
	}
	else if(strEvent == CALENDAR_V2VIEW_SWITCHVIEW_ON)
	{
		m_bSwitchView = true;
	}
	else if(strEvent == CALENDAR_V2VIEW_SWITCHVIEW_OFF)
	{
		m_bSwitchView = false;
	}
}

void CCalendar_V2View::OnDestroy()
{
	TRACE_FILE_LINE_INFO();

	if(m_pPage)
		m_pPage->DestroyWindow();

	CFormView::OnDestroy();
}

#pragma warning(pop)