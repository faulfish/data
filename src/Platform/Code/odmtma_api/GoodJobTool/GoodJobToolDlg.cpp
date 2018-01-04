// GoodJobToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GoodJobTool.h"
#include "GoodJobToolDlg.h"
#include <atltime.h>

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


// CGoodJobToolDlg dialog




CGoodJobToolDlg::CGoodJobToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGoodJobToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGoodJobToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGoodJobToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SetTestMode, &CGoodJobToolDlg::OnBnClickedSettestmode)
	ON_BN_CLICKED(IDC_Restore, &CGoodJobToolDlg::OnBnClickedRestore)
	ON_BN_CLICKED(IDC_BACKUP, &CGoodJobToolDlg::OnBnClickedBackup)
	ON_BN_CLICKED(IDC_BTN_RESTART, &CGoodJobToolDlg::OnBnClickedBtnRestart)
	ON_BN_CLICKED(IDC_BTN_Download, &CGoodJobToolDlg::OnBnClickedBtnDownload)
	ON_BN_CLICKED(IDC_IsDLMode, &CGoodJobToolDlg::OnBnClickedIsdlmode)
	ON_BN_CLICKED(IDC_BTN_BackupBrt, &CGoodJobToolDlg::OnBnClickedBtnBackupbrt)
	ON_BN_CLICKED(IDC_BTN_READQFUSE, &CGoodJobToolDlg::OnBnClickedBtnIsQfused)
END_MESSAGE_MAP()


// CGoodJobToolDlg message handlers

BOOL CGoodJobToolDlg::OnInitDialog()
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGoodJobToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGoodJobToolDlg::OnPaint()
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
HCURSOR CGoodJobToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGoodJobToolDlg::OnBnClickedSettestmode()
{
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T(""));
	CString cstrComPort;
	GetDlgItem(IDC_EditComPort)->GetWindowText(cstrComPort);
	int nComPort = _ttoi(cstrComPort.GetBuffer());
	cstrComPort.ReleaseBuffer();
	CQualcommDiag QualcommDiag(_T(""), nComPort);
	if(QualcommDiag.IsConnected())
	{
		if(!QualcommDiag.SetTestMode())
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("fail"));
		else
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("success"));
	}
	else
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("connection fail"));
}

void CGoodJobToolDlg::OnBnClickedRestore()
{
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T(""));
	CString cstrComPort;
	GetDlgItem(IDC_EditComPort)->GetWindowText(cstrComPort);
	int nComPort = _ttoi(cstrComPort.GetBuffer());
	cstrComPort.ReleaseBuffer();
	CQualcommDiag QualcommDiag(_T(""), nComPort);
	if(QualcommDiag.IsConnected())
	{
		bool bRestore = QualcommDiag.Restore(12000);
		if(!bRestore)
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("fail"));
		else
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("success"));
	}
	else
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("connection fail"));
	

}

void CGoodJobToolDlg::OnBnClickedBackup()
{
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T(""));
	CString cstrComPort;
	GetDlgItem(IDC_EditComPort)->GetWindowText(cstrComPort);
	int nComPort = _ttoi(cstrComPort.GetBuffer());
	cstrComPort.ReleaseBuffer();
	CQualcommDiag QualcommDiag(_T(""), nComPort);
	if(QualcommDiag.IsConnected())
	{
		bool bRestore = QualcommDiag.Backup(8000);
		if(!bRestore)
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("fail"));
		else
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("success"));
	}
	else
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("connection fail"));
	
}

void CGoodJobToolDlg::OnBnClickedBtnRestart()
{
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T(""));
	CString cstrComPort;
	GetDlgItem(IDC_EditComPort)->GetWindowText(cstrComPort);
	int nComPort = _ttoi(cstrComPort.GetBuffer());
	cstrComPort.ReleaseBuffer();
	CQualcommDiag QualcommDiag(_T(""), nComPort);
	if(QualcommDiag.IsConnected())
	{
		if(!QualcommDiag.Restart())
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("fail"));
		else
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("success"));
	}
	else
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("connection fail"));

}
typedef bool (*pfn_Download)(long, bool(*CallBack)(const char*, int , int),const LPCSTR);
typedef bool (*pfn_SetDLIniPath)(const TSTRING&);
void CGoodJobToolDlg::OnBnClickedBtnDownload()
{
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T(""));
	CString cstrComPort;
	GetDlgItem(IDC_EditComPort)->GetWindowText(cstrComPort);
	int nComPort = _ttoi(cstrComPort.GetBuffer());
	cstrComPort.ReleaseBuffer();

#ifdef _DEBUG
	TSTRING strDllPath = _T("..\\debug\\Download_Export_StaticMD.dll");
#else
	TSTRING strDllPath = _T("Download_Export_StaticM.dll");
#endif
	if(_taccess_s(strDllPath.c_str(),0) == 0)
	{
		HMODULE hSysDll = ::LoadLibrary(strDllPath.c_str());
		if(hSysDll)
		{
			pfn_Download Download= (pfn_Download)GetProcAddress(hSysDll, "Download");
			pfn_SetDLIniPath SetDLIniPath = (pfn_SetDLIniPath)GetProcAddress(hSysDll, "SetDLIniPath");
			if(SetDLIniPath)
			{
#ifdef _DEBUG
				SetDLIniPath(_T("..\\debug\\Download.ini"));
#else
				SetDLIniPath(_T("Download.ini"));
#endif
			}
			if(Download)
			{
				CTime tStart=CTime::GetCurrentTime();
#ifdef _DEBUG
				bool bDownload = Download(nComPort, CallBackFn, _T("..\\debug\\Download.ini"));
#else
				bool bDownload = Download(nComPort, CallBackFn, _T("Download.ini"));
#endif
				CTime tEnd=CTime::GetCurrentTime();
				CTimeSpan tSpan = tEnd - tStart;
				CString szStatus = bDownload? _T("[success] ") : _T("[fail] ");
				CString szTime =szStatus + tSpan.Format("Total time: %H:%M:%S");
				
				GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(szTime.GetBuffer());
				szTime.ReleaseBuffer();				
			}
		}
	}
	else
	{
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("Fail in accessing dll"));
	}
}

bool CGoodJobToolDlg::CallBackFn(const char* szInfo, int nStep, int nProgress)
{
	return true;
}
void CGoodJobToolDlg::OnBnClickedIsdlmode()
{
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T(""));
	CString cstrComPort;
	GetDlgItem(IDC_EditComPort)->GetWindowText(cstrComPort);
	int nComPort = _ttoi(cstrComPort.GetBuffer());
	cstrComPort.ReleaseBuffer();
	CQualcommDiag QualcommDiag(_T(""), nComPort);
	if(QualcommDiag.IsConnected())
	{
		if(!QualcommDiag.IsDLMode())
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("fail"));
		else
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("success"));
	}
	else
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("connection fail"));
}


typedef bool (*pfn_BackupBrt)(long nComPort, const LPCSTR szCsv, const LPCSTR szBrt);
void CGoodJobToolDlg::OnBnClickedBtnBackupbrt()
{
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T(""));
	CString cstrComPort;
	GetDlgItem(IDC_EditComPort)->GetWindowText(cstrComPort);
	int nComPort = _ttoi(cstrComPort.GetBuffer());
	cstrComPort.ReleaseBuffer();

	TSTRING strDllPath = _T(".\\Download_Export_StaticM.dll");
#ifdef _DEBUG
	strDllPath = _T("C:\\QSyncher_V2\\PlatformTool\\Code\\odmtma_api\\debug\\Download_Export_StaticMD.dll");
#endif
	TSTRING strCsvPath = _T(".\\BackupNV-Items.csv");
	if(_taccess_s(strDllPath.c_str(),0) == 0)
	{
		HMODULE hSysDll = ::LoadLibrary(strDllPath.c_str());
		if(hSysDll)
		{
			pfn_BackupBrt BackupBrt= (pfn_BackupBrt)GetProcAddress(hSysDll, "BackupBrt");
			
			if(BackupBrt)
			{
				if(!BackupBrt((long)nComPort, strCsvPath.c_str(), _T("temp.brt")))
					GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("fail"));
				else
					GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("success"));
			}
		}
	}
	else
	{
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("Fail in accessing dll"));
	}
}

typedef bool (*pfn_ReadQfuse)(long nComPort, unsigned int& nReg1, unsigned int& nReg2, int nTimeOut/* =DEFAULT_CMD_TIME_OUT */);
void CGoodJobToolDlg::OnBnClickedBtnIsQfused()
{
	CString cstrComPort;
	GetDlgItem(IDC_EditComPort)->GetWindowText(cstrComPort);
	int nComPort = _ttoi(cstrComPort.GetBuffer());
	cstrComPort.ReleaseBuffer();

	TSTRING strDllPath = _T(".\\Download_Export_StaticM.dll");
#ifdef _DEBUG
	strDllPath = _T("C:\\QSyncher_V2\\PlatformTool\\Code\\odmtma_api\\debug\\Download_Export_StaticMD.dll");
#endif

	if(_taccess_s(strDllPath.c_str(),0) == 0)
	{
		HMODULE hSysDll = ::LoadLibrary(strDllPath.c_str());
		if(hSysDll)
		{
			pfn_ReadQfuse ReadQfuse= (pfn_ReadQfuse)GetProcAddress(hSysDll, "ReadQfuse");

			if(ReadQfuse)
			{
				unsigned int nReg1=0, nReg2=0;
				if(!ReadQfuse((long)nComPort, nReg1, nReg2, 8000))
					GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("fail"));
				else
				{
					CString strReg1, strReg2;
					TCHAR szReg1[100] = {0};
					TCHAR szReg2[100] = {0};
					strReg1 = _itot(nReg1, szReg1, 10);
					strReg2 = _itot(nReg2, szReg2, 10);

					if(nReg1 == 6 && nReg2 == 0)
					{
						CString strResult = _T("No, Reg1 = ");
						strResult += strReg1;
						strResult += _T(", Reg2 = ");
						strResult += strReg2;
						GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(strResult);	
					}
					else
					{
						CString strResult = _T("Yes, Reg1 = ");
						strResult += strReg1;
						strResult += _T(", Reg2 = ");
						strResult += strReg2;
						GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(strResult);	
					}
				}
			}
		}
	}
	else
	{
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("Fail in accessing dll"));
	}
}