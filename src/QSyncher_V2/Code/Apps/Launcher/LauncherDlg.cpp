// LauncherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Launcher.h"
#include "LauncherDlg.h"
#include "../../Components\UIFramework\LayoutManager\GridLayout.h"
#include "../../Components\UIFramework\LayoutManager\Win32Widge.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace QSYNC_LIB;

// CLauncherDlg dialog
CLauncherDlg::CLauncherDlg()
:QSYNC_LIB::Win32DoubleBufferDialogWidge(CLauncherDlg::IDD,NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLauncherDlg, CDialog)
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CLauncherDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CLauncherDlg message handlers

BOOL CLauncherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	using namespace QSYNC_LIB;

	std::wstring		strImagePrefix;
#ifdef _DEBUG
	strImagePrefix = L"..\\Debug\\";
#else
	strImagePrefix = L"";
#endif
	
	//SetMinWidth(400);

	GridLayout<HDC>* pLayout = new GridLayout<HDC>;
	//pLayout->SetMinWidth(300);
	//pLayout->SetMaxHeight(600);
	//pLayout->SetMaxWidth(500);
	
	//Drawer widge demo
	m_strBGImagePath = strImagePrefix + _T("Pictures\\Launcher\\Launch_BG.bmp");
	//pLayout->AddWidget(new WIN32DrawerWidge(this,m_strBGImagePath),0,0,3,3);

	//Attach window widge demo
	//Limitation size of widge demo
	WIN32WindowWidge* pWidge = new WIN32WindowWidge(GetDlgItem(IDC_BUTTON1));
	//pWidge->SetMinHeight(50);
	//pWidge->SetMaxHeight(200);
	pLayout->AddWidget(pWidge,0,1,3,1);
	
	std::wstring strDisplay = strImagePrefix + L"Pictures\\Launcher\\Language_Close.bmp";
	std::wstring strHighlightDisplay = strImagePrefix + L"Pictures\\Launcher\\Language_Close_Highlight.bmp";	

	//Button widge demo
	pLayout->AddWidget(new WIN32ButtonWidge(this,strDisplay,strHighlightDisplay,OnBnClicked1),1,2,2,2);

	//Drawer widge demo
	pLayout->AddWidget(new WIN32DrawerWidge(this,strDisplay),2,2);

	//Attach window widge demo
	pLayout->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_EDIT1)),1,0);

	//Nest layout demo
	GridLayout<HDC>* pSecondLayout = new GridLayout<HDC>;
	pSecondLayout->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_BUTTON2)),1,1);
	pLayout->AddWidget(pSecondLayout,2,0);

	SetLayout(pLayout);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLauncherDlg::OnBnClicked1()
{
	AfxMessageBox(_T("void CLauncherDlg::OnBnClicked1()"));
}

void CLauncherDlg::OnBnClickedButton1()
{
}

