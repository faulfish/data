// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Publish_Client.h"
#include "ClientDlg.h"
#include "Shlwapi.h"
#include <cassert>

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


// CClientDlg dialog




CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SETUP_BUTTON, &CClientDlg::OnBnClickedSetupButton)
	ON_CBN_SELCHANGE(IDC_PROJECT, &CClientDlg::OnCbnSelchangeProject)
	ON_CBN_SELENDOK(IDC_PROJECT, &CClientDlg::OnCbnSelendokProject)
	ON_CBN_SELCHANGE(IDC_STATION, &CClientDlg::OnCbnSelchangeStation)
	ON_CBN_SELCHANGE(IDC_VERSION, &CClientDlg::OnCbnSelchangeVersion)
END_MESSAGE_MAP()


// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
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
	TCHAR szPath[MAX_PATH] = {0};
	::GetModuleFileName(NULL,szPath,MAX_PATH);
	::PathRemoveFileSpec(szPath);
	::PathAppend(szPath,_T("*"));

	UpdateComboList(IDC_PROJECT,_T("Project_"),szPath);

	CComboBox* pComboCtrl = (CComboBox*)GetDlgItem(IDC_VERSION);
	pComboCtrl->EnableWindow(false);
	pComboCtrl = (CComboBox*)GetDlgItem(IDC_STATION);
	pComboCtrl->EnableWindow(false);

	GetDlgItem(IDC_INSTALLSTATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_INSTALLPATH)->ShowWindow(SW_HIDE);	

	TCHAR szComputer[1024] = {0};
	DWORD dwSize = 1024;
	GetComputerName(szComputer,&dwSize);
	SetWindowText(szComputer);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientDlg::OnPaint()
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CClientDlg::OnBnClickedSetupButton()
{
	CString strProject;
	CComboBox* pComboCtrl = (CComboBox*)GetDlgItem(IDC_PROJECT);
	pComboCtrl->GetLBText(pComboCtrl->GetCurSel(),strProject);

	CString strVersion;
	pComboCtrl = (CComboBox*)GetDlgItem(IDC_VERSION);
	pComboCtrl->GetLBText(pComboCtrl->GetCurSel(),strVersion);

	CString strStation;
	pComboCtrl = (CComboBox*)GetDlgItem(IDC_STATION);
	pComboCtrl->GetLBText(pComboCtrl->GetCurSel(),strStation);

	TCHAR szPath[MAX_PATH] = {0};
	::GetModuleFileName(NULL,szPath,MAX_PATH);
	::PathRemoveFileSpec(szPath);
	::PathAppend(szPath,strProject);
	::PathAppend(szPath,strVersion);
	::PathAppend(szPath,strStation);
	::PathAppend(szPath,_T("setup.exe"));

	if(::_taccess(szPath,0) == -1)
	{
		CString strMessage(_T("Can't not find the setup file\r\n"));
		AfxMessageBox(strMessage + szPath);
	}
	else
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		ZeroMemory( &pi, sizeof(pi) );

		// Start the child process. 
		if( !CreateProcess( NULL,   // No module name (use command line). 
			szPath, // Command line. 
			NULL,             // Process handle not inheritable. 
			NULL,             // Thread handle not inheritable. 
			FALSE,            // Set handle inheritance to FALSE. 
			0,                // No creation flags. 
			NULL,             // Use parent's environment block. 
			NULL,             // Use parent's starting directory. 
			&si,              // Pointer to STARTUPINFO structure.
			&pi )             // Pointer to PROCESS_INFORMATION structure.
			) 
		{
			printf( "CreateProcess failed (%d).\n", GetLastError() );
			return;
		}

		// Wait until child process exits.
		WaitForSingleObject( pi.hProcess, INFINITE );

		// Close process and thread handles. 
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
	}
}

void CClientDlg::OnCbnSelendokProject()
{
}

void CClientDlg::UpdateComboList(UINT nID,const CString& strPrefix,const CString& strPath)
{
	WIN32_FIND_DATA FindFileData = {0};
	HANDLE hFind = ::FindFirstFile(strPath, &FindFileData);

	CComboBox* pComboCtrl = (CComboBox*)GetDlgItem(nID);
	while(pComboCtrl->GetCount())
		pComboCtrl->DeleteString(0);
	pComboCtrl->EnableWindow(true);
	pComboCtrl->UpdateWindow();

	do
	{
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(::_tcsncicmp(FindFileData.cFileName,strPrefix,strPrefix.GetLength()) == 0)
			{					
				pComboCtrl->AddString(FindFileData.cFileName);
			}
		}
	}
	while(::FindNextFile(hFind,&FindFileData));
	FindClose(hFind);

	GetDlgItem(IDC_SETUP_BUTTON)->EnableWindow(FALSE);
}

void CClientDlg::OnCbnSelchangeStation()
{
	CString strStation;
	CComboBox* pComboCtrl = (CComboBox*)GetDlgItem(IDC_STATION);
	pComboCtrl->GetLBText(pComboCtrl->GetCurSel(),strStation);

	if(strStation.IsEmpty() == FALSE)
		GetDlgItem(IDC_SETUP_BUTTON)->EnableWindow(TRUE);
}

void CClientDlg::OnCbnSelchangeVersion()
{
	CString strProject;
	CComboBox* pComboCtrl = (CComboBox*)GetDlgItem(IDC_PROJECT);
	pComboCtrl->GetLBText(pComboCtrl->GetCurSel(),strProject);

	CString strVersion;
	pComboCtrl = (CComboBox*)GetDlgItem(IDC_VERSION);
	pComboCtrl->GetLBText(pComboCtrl->GetCurSel(),strVersion);

	TCHAR szPath[MAX_PATH] = {0};
	::GetModuleFileName(NULL,szPath,MAX_PATH);
	::PathRemoveFileSpec(szPath);
	::PathAppend(szPath,strProject);
	::PathAppend(szPath,strVersion);
	::PathAppend(szPath,_T("*"));

	UpdateComboList(IDC_STATION,_T("Station_"),szPath);
}

void CClientDlg::OnCbnSelchangeProject()
{
	CString strProject;
	CComboBox* pComboCtrl = (CComboBox*)GetDlgItem(IDC_PROJECT);
	pComboCtrl->GetLBText(pComboCtrl->GetCurSel(),strProject);

	pComboCtrl = (CComboBox*)GetDlgItem(IDC_STATION);
	while(pComboCtrl->GetCount())
		pComboCtrl->DeleteString(0);
	pComboCtrl->EnableWindow(false);
	pComboCtrl->UpdateWindow();

	TCHAR szPath[MAX_PATH] = {0};
	::GetModuleFileName(NULL,szPath,MAX_PATH);
	::PathRemoveFileSpec(szPath);
	::PathAppend(szPath,strProject);
	::PathAppend(szPath,_T("*"));

	UpdateComboList(IDC_VERSION,_T("Version_"),szPath);
}