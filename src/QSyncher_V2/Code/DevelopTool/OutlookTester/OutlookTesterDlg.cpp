// OutlookTesterDlg.cpp : implementation file
//
#pragma once
#include "stdafx.h"
#include "OutlookTester.h"
#include "OutlookTesterDlg.h"
//#include "../../Code/Components/OutlookAccess/.h"
#include "../../Components/OutlookAccess/VCardCtrl.h"
#include "../../Components/PIMDB/SyncManager.h"
#include "../../Components/PIMDB/PIMRecord.h"
#include "../../Dll/Calendar_Version2/VCalCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// COutlookTesterDlg dialog




COutlookTesterDlg::COutlookTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COutlookTesterDlg::IDD, pParent), m_bFinishCalFetch(false), m_bFinishContactFetch(false),m_pContactCtrl(NULL),m_pCalendarCtrl(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COutlookTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_BtnNew);
	DDX_Control(pDX, IDC_BUTTON2, m_BtnModify);
	DDX_Control(pDX, IDC_BUTTON3, m_BtnDelete);
	DDX_Control(pDX, IDC_BUTTON5, m_BtnNewCal);
	DDX_Control(pDX, IDC_BUTTON6, m_BtnModifyCal);
	DDX_Control(pDX, IDC_BUTTON7, m_BtnDeleteCal);
}

BEGIN_MESSAGE_MAP(COutlookTesterDlg, CDialog)
	//ON_WM_SYSCOMMAND()
	//ON_WM_PAINT()
	//ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &COutlookTesterDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &COutlookTesterDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &COutlookTesterDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &COutlookTesterDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &COutlookTesterDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON4, &COutlookTesterDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &COutlookTesterDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &COutlookTesterDlg::OnBnClickedButton7)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// COutlookTesterDlg message handlers

BOOL COutlookTesterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COutlookTesterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COutlookTesterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COutlookTesterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void COutlookTesterDlg::OnBnClickedOk() // start to fetch contact
{
	using namespace QSYNC_LIB;
	if(m_pContactCtrl == NULL)
		CreateOutlookAccess(KEYWORD_CONTACT, &m_pContactCtrl);
	//time_t timestart;
	//time_t timeend;
	//time(&timestart);
	m_pContactCtrl->Register(this,EVENT_CONTACT_FETCHING_FINISH);
	m_bFinishCalFetch = false;
	m_pContactCtrl->StartToFetch();
	
	//time(&timeend);
	//TRACE("total time: %ld\n", timeend-timestart);
}

void COutlookTesterDlg::Event(const TSTRING& strEvent,long nParam)
{
	AfxMessageBox(strEvent.c_str());

	if(0 == _tcscmp(strEvent.c_str(), QSYNC_LIB::EVENT_CONTACT_FETCHING_FINISH))
	{
		m_bFinishContactFetch = true;
		m_BtnNew.EnableWindow(TRUE);
		m_BtnModify.EnableWindow(TRUE);
		m_BtnDelete.EnableWindow(TRUE);
	}
	else if(0 == _tcscmp(strEvent.c_str(), QSYNC_LIB::EVENT_CALENDAR_FETCHING_FINISH))
	{
		m_bFinishCalFetch = true;
		m_BtnNewCal.EnableWindow(TRUE);
		m_BtnModifyCal.EnableWindow(TRUE);
		m_BtnDeleteCal.EnableWindow(TRUE);
	}
	else if(0 == _tcscmp(strEvent.c_str(), QSYNC_LIB::EVENT_ACCESS_ESTABLISH_FAIL))
	{
		
	;	
	}
	else if(0 == _tcscmp(strEvent.c_str(), QSYNC_LIB::EVENT_CONTACT_FETCHING_FAIL))
	{
	
	}
	else if(0 == _tcscmp(strEvent.c_str(), QSYNC_LIB::EVENT_CALENDAR_FETCHING_FAIL))
	{
		
	}
}
void COutlookTesterDlg::OnBnClickedButton1() // new contact
{
	using namespace QSYNC_LIB;

	//if(m_bFinishContactFetch)
	{
		//new
		IPIMPt ptPIMRecordFactory =new PIMFactory<CPIMRecord>;
		IPIMObjPt PIMObjPt = ptPIMRecordFactory->CreatePIMObject();
		
		if(PIMObjPt)
		{
			CVCardCtrl vCtrl(PIMObjPt);
			bool bSetEmptyPIMObj = vCtrl.SetEmptyIPIMObjPt();
			assert(bSetEmptyPIMObj);
			if(bSetEmptyPIMObj)
			{
				bool bSet = vCtrl.SetCmnStrData( "VCARD:N:0" , _T("TE45ST"));
				//vCtrl.SetCmnWStrData( "VCARD:N:0", L"TEST");
				bSet = vCtrl.SetCmnStrData( "VCARD:TEL", _T("12345678569"));
				TSTRING strRaw;
				PIMObjPt->encode(strRaw);
				bool bInsert = m_pContactCtrl->Insert(PIMObjPt);
				assert(bInsert);
			}
		}
	}
}

void COutlookTesterDlg::OnBnClickedButton2()  // modify contact
{
	using namespace QSYNC_LIB;
	IPIMObjPt PIMObjPt;
	bool bGet = m_pContactCtrl->GetByIndex(0, PIMObjPt);
	assert(bGet);
	if(PIMObjPt && bGet)
	{
		CVCardCtrl vCtrl(PIMObjPt);
		vCtrl.SetCmnStrData("VCARD:TEL", _T("meimei"));
		vCtrl.SetCmnStrData( "VCARD:N:0" , _T("DF"));
		bool bModify = m_pContactCtrl->Update(PIMObjPt);
		assert(bModify);
	}
}

void COutlookTesterDlg::OnBnClickedButton3() // delete Contact
{
	using namespace QSYNC_LIB;
	IPIMObjPt PIMObjPt;
	bool bGet = m_pContactCtrl->GetByIndex(0, PIMObjPt);
	if(bGet)
	{
		bool bDelete = m_pContactCtrl->Delete(PIMObjPt);
		assert(bDelete);
	}
}

void COutlookTesterDlg::OnBnClickedButton4()  // start to fetch calendar
{
	using namespace QSYNC_LIB;
	if(m_pCalendarCtrl == NULL)
		CreateOutlookAccess(KEYWORD_CALENDAR, &m_pCalendarCtrl);
	m_pCalendarCtrl->Register(this,EVENT_CALENDAR_FETCHING_FINISH);
	m_bFinishCalFetch = false;
	m_pCalendarCtrl->StartToFetch();
}

void COutlookTesterDlg::OnBnClickedButton5() // new appointment
{
	using namespace QSYNC_LIB;

	if(m_bFinishCalFetch)
	{
		//new
		IPIMPt ptPIMRecordFactory =new PIMFactory<CPIMRecord>;
		IPIMObjPt PIMObjPt = ptPIMRecordFactory->CreatePIMObject();
		
		if(PIMObjPt)
		{
			CVCalCtrl VCalCtrl(PIMObjPt);
			VCalCtrl.SetCmnStrData(KEYWORD_SUBJECT, _T("hahahahahahaha"));
			VCalCtrl.SetCmnStrData(KEYWORD_DTSTART, _T("20070920T130000Z"));
			VCalCtrl.SetCmnStrData(KEYWORD_RECURRENCE,_T("W2 MO WE FR #10"));
			bool bInsert = m_pCalendarCtrl->Insert(PIMObjPt);
			assert(bInsert);
		}
	}
}



void COutlookTesterDlg::OnBnClickedButton6()  // Modify appointment
{
	using namespace QSYNC_LIB;
	IPIMObjPt PIMObjPt;
	bool bGet =  m_pCalendarCtrl->GetByIndex(0, PIMObjPt);
	assert(bGet);
	if(PIMObjPt && bGet)
	{
		CVCardCtrl vCtrl(PIMObjPt);
		CVCalCtrl VCalCtrl(PIMObjPt);
		VCalCtrl.SetCmnStrData(KEYWORD_SUBJECT, _T("heyejyejeyejeyjeyjeyjeyjeyjey"));
		VCalCtrl.SetCmnStrData(KEYWORD_DTSTART, _T("20070920T130000Z"));
		VCalCtrl.SetCmnStrData(KEYWORD_RECURRENCE,_T("W2 MO WE FR #10"));
		bool bUpdate = m_pCalendarCtrl->Update(PIMObjPt);
		assert(bUpdate);
	}
}

void COutlookTesterDlg::OnBnClickedButton7()  // Delete Appointment
{	using namespace QSYNC_LIB;
	IPIMObjPt PIMObjPt;
	bool bGet = m_pCalendarCtrl->GetByIndex(0, PIMObjPt);
	assert(bGet);
	if(PIMObjPt && bGet)
	{
		bool bDelete = m_pCalendarCtrl->Delete(PIMObjPt);
		assert(bDelete);
	}
}

void COutlookTesterDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	__super::OnClose();
}

void COutlookTesterDlg::OnDestroy()
{
	__super::OnDestroy();

	// TODO: Add your message handler code here
	if(m_pContactCtrl)
		DeleteOutlookAccess(m_pContactCtrl);
	if(m_pCalendarCtrl)
		DeleteOutlookAccess(m_pCalendarCtrl);
}
