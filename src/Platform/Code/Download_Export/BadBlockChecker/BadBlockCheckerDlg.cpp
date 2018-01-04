// BadBlockCheckerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BadBlockChecker.h"
#include "BadBlockCheckerDlg.h"
#include "..\..\Modules\QisdaQualcommDownloadModule\Download.h"

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


// CBadBlockCheckerDlg dialog




CBadBlockCheckerDlg::CBadBlockCheckerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBadBlockCheckerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBadBlockCheckerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_HexFile, m_HexFile);
	DDX_Control(pDX, IDC_EDIT_COM, m_ComPort);
}

BEGIN_MESSAGE_MAP(CBadBlockCheckerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_FILE, &CBadBlockCheckerDlg::OnBnClickedBtnFile)
	ON_BN_CLICKED(IDC_BTN_DL, &CBadBlockCheckerDlg::OnBnClickedBtnDl)
	ON_BN_CLICKED(IDC_BTN_DL6240, &CBadBlockCheckerDlg::OnBnClickedBtnDl6240)
END_MESSAGE_MAP()


// CBadBlockCheckerDlg message handlers

BOOL CBadBlockCheckerDlg::OnInitDialog()
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
	GetDlgItem(IDC_EDIT_CheckTimeout)->SetWindowText(_T("60"));
	for (int i=0;i < 10;i++)
	{
		TCHAR cTemp[10] = {0};
		_itot_s(i, cTemp, 10, 10);
		((CComboBox*)GetDlgItem(IDC_COMBO_OPTION))->AddString(cTemp);
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_OPTION))->SetCurSel(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBadBlockCheckerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBadBlockCheckerDlg::OnPaint()
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
HCURSOR CBadBlockCheckerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CBadBlockCheckerDlg::OnBnClickedBtnFile()
{
	CFileDialog FileDlg(TRUE);
	
	if(IDOK == FileDlg.DoModal())
	{
		m_HexFile.SetWindowText(FileDlg.GetPathName());
	}
}

int CBadBlockCheckerDlg::GetComPort()
{
	CString cstrComPort;
	GetDlgItem(IDC_EDIT_COM)->GetWindowText(cstrComPort);
	int nComPort = _ttoi(cstrComPort.GetBuffer());
	cstrComPort.ReleaseBuffer();
	return nComPort;
}


int CBadBlockCheckerDlg::GetCheckBadBlockTimeout()
{
	CString cstrTimeout;
	GetDlgItem(IDC_EDIT_CheckTimeout)->GetWindowText(cstrTimeout);
	int nTimeout = _ttoi(cstrTimeout.GetBuffer());
	cstrTimeout.ReleaseBuffer();
	if(nTimeout == 0)
		nTimeout = 60;
	return nTimeout;
}

int CBadBlockCheckerDlg::GetOption()
{
	int nOption = ((CComboBox*)GetDlgItem(IDC_COMBO_OPTION))->GetCurSel();
	return nOption;
}

void CBadBlockCheckerDlg::OnBnClickedBtnDl()
{
	CDownload6245 DL(GetComPort());
	CString cstrHexFile;
	GetDlgItem(IDC_EDIT_HexFile)->GetWindowText(cstrHexFile);
	if(DL.DMSSDownload((TSTRING)cstrHexFile))
	{
		if(DL.StreamingHello(false, 50, 500))
		{
			TSTRING strBadBlock;
			if(DL.CheckBadBlock(strBadBlock, GetOption(), GetCheckBadBlockTimeout()*1000))
			{	
				GetDlgItem(IDC_EDIT_BadBlock)->SetWindowText(strBadBlock.c_str());
			}
			else
			{
				GetDlgItem(IDC_EDIT_BadBlock)->SetWindowText(_T("Failed in checking BadBlock command"));
			}
		}
		else
			GetDlgItem(IDC_EDIT_BadBlock)->SetWindowText(_T("Failed in streaming DL(Send Hello)"));
	}
	else
		GetDlgItem(IDC_EDIT_BadBlock)->SetWindowText(_T("Failed in DMSS DL"));

}

void CBadBlockCheckerDlg::OnBnClickedBtnDl6240()
{
	if(GetComPort() !=0 )
	{
		CDownload6240 DL(GetComPort());
		CString cstrHexFile;
		GetDlgItem(IDC_EDIT_HexFile)->GetWindowText(cstrHexFile);
		if(::_taccess(cstrHexFile, 0) == 0)
		{
			if(DL.DMSSDownload((TSTRING)cstrHexFile))
			{
				Sleep(35000);
				if(DL.StreamingHello(false, 50, 500))
				{
					if(DL.StreamingSecurity("", false))
					{
						if(DL.StreamingReset(false))
						{
							GetDlgItem(IDC_EDIT_BadBlock)->SetWindowText(_T("Streaming Reset succeed"));
						}
						else
							GetDlgItem(IDC_EDIT_BadBlock)->SetWindowText(_T("Failed in streaming DL(Reset)"));
					}
					else
						GetDlgItem(IDC_EDIT_BadBlock)->SetWindowText(_T("Failed in streaming DL(Security)"));
				}
				else
					GetDlgItem(IDC_EDIT_BadBlock)->SetWindowText(_T("Failed in streaming DL(Send Hello)"));
			}
			else
				GetDlgItem(IDC_EDIT_BadBlock)->SetWindowText(_T("Failed in DMSS DL"));
		}
		else
			GetDlgItem(IDC_EDIT_BadBlock)->SetWindowText(_T("Hex file doesn't exist"));
	}
	else
		GetDlgItem(IDC_EDIT_BadBlock)->SetWindowText(_T("Please input the correct COMPort"));
	
}
