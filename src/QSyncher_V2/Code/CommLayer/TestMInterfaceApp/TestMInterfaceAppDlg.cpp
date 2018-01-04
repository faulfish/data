// TestMInterfaceAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestMInterfaceApp.h"
#include "TestMInterfaceAppDlg.h"
#include "../MobileInterface/Interface_Include/ICommMobile.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTestMInterfaceAppDlg dialog


CTestMInterfaceAppDlg::CTestMInterfaceAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestMInterfaceAppDlg::IDD, pParent)
	, m_strPort(_T(""))
	, m_strDevName(_T(""))
	, m_strModelName(_T(""))
	, m_strIMEI(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestMInterfaceAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INTERFACE_TAB, m_ITab);
	DDX_Text(pDX, IDC_PORT_EDIT, m_strPort);
	DDX_Text(pDX, IDC_MODEL_EDIT, m_strDevName);
	DDX_Control(pDX, IDC_CONNECTION_STATE, m_strConnState);
	DDX_Text(pDX, IDC_EDIT2, m_strModelName);
	DDX_Text(pDX, IDC_EDIT3, m_strIMEI);
}

BEGIN_MESSAGE_MAP(CTestMInterfaceAppDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CONNECT_BUTTON, &CTestMInterfaceAppDlg::OnBnClickedConnectbutton)
	ON_BN_CLICKED(IDC_DISCONNECT_BUTTON, &CTestMInterfaceAppDlg::OnBnClickedDisconnectButton)
END_MESSAGE_MAP()


// CTestMInterfaceAppDlg message handlers

void CTestMInterfaceAppDlg::Event(const TSTRING& strEvent,long nParam)
{
	if(strEvent == _T("Connect"))
	{
		QSYNC_LIB::DeviceDataPt* pspDeviceData = (QSYNC_LIB::DeviceDataPt*)nParam;
		m_strPort = (*pspDeviceData)->ID().c_str();
		m_strDevName = (*pspDeviceData)->GetExt().c_str();
		UpdateData(FALSE);
	}
	else if(strEvent == _T("Disconnect"))
	{
		QSYNC_LIB::DeviceDataPt* pspDeviceData = (QSYNC_LIB::DeviceDataPt*)nParam;
		if(m_strPort == (*pspDeviceData)->ID().c_str() && m_strDevName == (*pspDeviceData)->GetExt().c_str())
		{
			m_strPort.Empty();
			m_strDevName.Empty();
			UpdateData(FALSE);
		}
	}
}

BOOL CTestMInterfaceAppDlg::OnInitDialog()
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

	// TODO: Add extra initialization here
	QSYNC_LIB::GetDeviceFinder()->Register(this);
	QSYNC_LIB::GetDeviceFinder()->StartMonitor();

	int nPageID = 0;
	m_CommPage.Create (IDD_COMM_INTERFACE_PAGE, this);
	m_ITab.AddSSLPage (_T("Comm"), nPageID++, &m_CommPage);
    m_IEFSPage.Create (IDD_IEFS_PAGE, this);
	m_ITab.AddSSLPage (_T("IEFS"), nPageID++, &m_IEFSPage);
	m_IMessagePage.Create (IDD_IMessage_PAGE, this);
	m_ITab.AddSSLPage (_T("IMessage"), nPageID++, &m_IMessagePage);
	m_IPhonebookPage.Create (IDD_IPhonebook_PAGE, this);
	m_ITab.AddSSLPage (_T("IPhonebook"), nPageID++, &m_IPhonebookPage);
	m_ISyncMLPage.Create (IDD_ISyncML_PAGE, this);
	m_ITab.AddSSLPage (_T("ISyncML"), nPageID++, &m_ISyncMLPage);
	m_IIrMCPage.Create (IDD_IIrMC_PAGE, this);
	m_ITab.AddSSLPage (_T("IIrMC"), nPageID++, &m_IIrMCPage);

	CommOpenMobileInterface( theAppName );
	m_CommPage.UpdateInfoList();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestMInterfaceAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestMInterfaceAppDlg::OnPaint()
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
HCURSOR CTestMInterfaceAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestMInterfaceAppDlg::OnBnClickedConnectbutton()
{
  // TODO: Add your control notification handler code here
  UpdateData();
  int nRes;
  TCHAR strModel[MAX_RET_STRING_LENGTH], 
	    strIMEI[MAX_RET_STRING_LENGTH], 
		strDisplayName[MAX_RET_STRING_LENGTH];
  
  nRes=CommConnectMobile( theAppName, eCommSerial, m_strPort, m_strDevName);
  
  if ( nRes == eCommRetSuccess )
  {
	CommGetMobileInfo(theAppName, strModel, strIMEI, strDisplayName);
	
	m_strConnState.SetWindowTextW(_T("Device Connected"));
	m_CommPage.UpdateInfoList();
	CEdit *pIMEIEdit  = (CEdit*)GetDlgItem(IDC_EDIT3);
	CEdit *pModelEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
	pIMEIEdit->SetWindowTextW(strIMEI);
	pModelEdit->SetWindowTextW(strModel);
  }
  else if( nRes == eCommErrDeviceAlreadyConnect )
  {
    AfxMessageBox(_T("A device is already connected."));
  }
  else
  {
    AfxMessageBox(_T("Connect to Device Fail! Please check port and model name."));
  }
}

void CTestMInterfaceAppDlg::OnBnClickedDisconnectButton()
{
  // TODO: Add your control notification handler code here
  UpdateData();
  int nRes;
  nRes = CommDisConnectMobile( theAppName, eCommSerial, m_strPort, m_strDevName);
  
  if ( nRes== eCommRetSuccess )
  {
    m_strConnState.SetWindowTextW(_T("No Connection"));
	m_CommPage.UpdateInfoList();
	CEdit *pIMEIEdit  = (CEdit*)GetDlgItem(IDC_EDIT3);
	CEdit *pModelEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
	pIMEIEdit->SetWindowTextW(_T(""));
	pModelEdit->SetWindowTextW(_T(""));
  }
}
