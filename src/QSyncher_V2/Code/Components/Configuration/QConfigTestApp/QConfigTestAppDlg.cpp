// QConfigTestAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QConfigTestApp.h"
#include "QConfigTestAppDlg.h"
#include "..\QConfigure\QMobileConfigure.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CQConfigTestAppDlg dialog

CQConfigTestAppDlg::CQConfigTestAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQConfigTestAppDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQConfigTestAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CQConfigTestAppDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CQConfigTestAppDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CQConfigTestAppDlg message handlers

BOOL CQConfigTestAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CQConfigTestAppDlg::OnPaint()
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
HCURSOR CQConfigTestAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQConfigTestAppDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
    CFile qConfigFile;
	BOOL  bResult;
	const int MAX_CONFIG_FSIZE = 8*1024; //Max 8K 

	bResult = qConfigFile.Open(_T("QConfigure.xml"),CFile::modeRead);

    if ( bResult )
	{
	   CQMobileConfigure* myConfigParser = GetMobileConFig();
	   char cBuffer[MAX_CONFIG_FSIZE];
	   
	   int nSize;
	   nSize = qConfigFile.Read( cBuffer, MAX_CONFIG_FSIZE);
	   
	   if (nSize > 0)
	   {
			CString strConfigXML( cBuffer, nSize );
			myConfigParser->LoadQConfigXML( strConfigXML );
            
			CQMobileSetting MobSetting;
			myConfigParser->GetMobileSettings( _T("MERB1A"), MobSetting );

			CQSetting AppSetting;
			myConfigParser->GetAppSettings( _T("AppDefault"), AppSetting );

			CQSetting InterfaceSetting;
			myConfigParser->GetDeviceInterfaceSettings(MobSetting.QMobDeviceSetting.strQSettingValue,
				                                       MobSetting.QMobInterfaceSetting.strQSettingValue, 
													   InterfaceSetting);
	   }
	}
	
	//OnOK();
}
