// DLTesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DLTester.h"
#include "DLTesterDlg.h"


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


// CDLTesterDlg dialog




CDLTesterDlg::CDLTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDLTesterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_ptBuffer = new CBuffer;
}

void CDLTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INI, m_EditIni);
	DDX_Control(pDX, IDC_LIST_Result, m_ListBoxReult);
}

BEGIN_MESSAGE_MAP(CDLTesterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_DUMP_IMAGE, &CDLTesterDlg::OnBnClickedBtnDumpImage)
	ON_BN_CLICKED(IDC_BTN_GET_INI, &CDLTesterDlg::OnBnClickedBtnGetIniPath)
	ON_BN_CLICKED(IDC_BTN_DL_DUMPIMAGE, &CDLTesterDlg::OnBnClickedBtnDlDumpimage)
	ON_BN_CLICKED(IDC_BTN_SaveImage, &CDLTesterDlg::OnBnClickedBtnSaveimage)
	ON_BN_CLICKED(IDC_BTN_ClearBoard, &CDLTesterDlg::OnBnClickedBtnClearboard)
END_MESSAGE_MAP()


// CDLTesterDlg message handlers

BOOL CDLTesterDlg::OnInitDialog()
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
	char szDirectory[400] = {0};
	::GetCurrentDirectory(400, szDirectory);
	m_strIniPath = std::string(szDirectory) + "\\setup.ini";
	GetDlgItem(IDC_EDIT_INI)->SetWindowText(m_strIniPath.c_str());

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLTesterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDLTesterDlg::OnPaint()
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
HCURSOR CDLTesterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CDLTesterDlg::OnBnClickedBtnGetIniPath()
{
	CFileDialog FileDlg(TRUE);
	char szDirectory[400] = {0};
	::GetCurrentDirectory(400, szDirectory);
	if(IDOK == FileDlg.DoModal())
	{
		::SetCurrentDirectory(szDirectory);
		m_EditIni.SetWindowText(FileDlg.GetPathName());
		m_strIniPath = FileDlg.GetPathName();
	}
}


bool CDLTesterDlg::SetValueFromIni(const std::string& strTag, const std::string& strKey, std::string& strValue)
{
	bool bRes = true;
	char cpValue[300] ={0};
	DWORD lRetValue = ::GetPrivateProfileString(strTag.c_str(),strKey.c_str(),NULL,cpValue, 300, m_strIniPath.c_str());
	strValue = cpValue;
	std::string strMsg;
	strMsg = strKey+ ": " + strValue;
	AddMessage(strMsg.c_str());
	return bRes;
}
bool CDLTesterDlg::SetValueFromIni(const std::string& strTag, const std::string& strKey, int& nValue)
{
	bool bRes = true;
	char cpValue[300] ={0};
	DWORD lRetValue = ::GetPrivateProfileString(strTag.c_str(),strKey.c_str(),NULL,cpValue, 300, m_strIniPath.c_str());
	nValue = _ttoi(cpValue);
	std::string strMsg;
	strMsg = strKey+ ": " + cpValue;
	AddMessage(strMsg.c_str());
	return bRes;
}
bool CDLTesterDlg::SetValueFromIni(const std::string& strTag, const std::string& strKey, DWORD& nValue)
{
	bool bRes = true;
	char cpValue[300] ={0};
	DWORD lRetValue = ::GetPrivateProfileString(strTag.c_str(),strKey.c_str(),NULL,cpValue, 300, m_strIniPath.c_str());
	nValue = _ttol(cpValue);
	std::string strMsg;
	strMsg = strKey+ ": " + cpValue;
	AddMessage(strMsg.c_str());
	return bRes;
}

bool CDLTesterDlg::GetIniInfo(const std::string& strTag)
{
	bool bRes = false;
	if(::_taccess_s(m_strIniPath.c_str(),0) == 0)
	{
		if(strTag == "DumpImage")
		{
			SetValueFromIni(strTag, "Platform", m_nPlatform);
			SetValueFromIni(strTag, "Hex", m_strHexPath);
			SetValueFromIni(strTag, "SendRawRequest", m_strRawRequest);
			SetValueFromIni(strTag, "SendRawRequestTimeOut", m_nRawRequestTimeOut);
			if(m_nRawRequestTimeOut == 0)
				m_nRawRequestTimeOut = 20000;
			bRes = true;
		}
		else if(strTag == "DownloadDumpImage")
		{
			SetValueFromIni(strTag, "Platform", m_nPlatform);
			SetValueFromIni(strTag, "Hex", m_strHexPath);
			//SetValueFromIni(strTag, "Partition", m_strPartition);
			SetValueFromIni(strTag, "Cefs", m_strCefs);
			//SetValueFromIni(strTag, "TimeOut", m_nTimeOut);
			SetValueFromIni(strTag, "StartAddress", m_dwStartAddress);
			bRes = true;
		}
	}
	else
	{
		AddMessage(_T("Ini File doesn't exist!"));
	}
	return bRes;
}

bool CDLTesterDlg::GetComPort(int& nComPort)
{
	bool bRes = false;
	CString cstrComPort;
	GetDlgItem(IDC_EDIT_COM)->GetWindowText(cstrComPort);
	if(!cstrComPort.IsEmpty())
	{
		nComPort = _ttoi(cstrComPort.GetBuffer());
		cstrComPort.ReleaseBuffer();
		bRes = true;
	}
	return bRes;
}
CDLTesterDlg* g_pThis = NULL;

void CDLTesterDlg::OnBnClickedBtnDlDumpimage()
{
	if(GetIniInfo("DownloadDumpImage"))
	{
		if(::_taccess_s(m_strHexPath.c_str(),0) == 0 &&
			::_taccess_s(m_strCefs.c_str(),0) == 0)
		{
			g_pThis = this;
			AfxBeginThread(DLDumpImageThread,g_pThis,THREAD_PRIORITY_NORMAL,0,0,NULL);
		}
		else
		{
			AddMessage("Information from IniFile is not enough.");
		}
	}
}

UINT CDLTesterDlg::DLDumpImageThread(LPVOID pParam)
{
	CDLTesterDlg* pThis = (CDLTesterDlg*)pParam;
	(CButton *)pThis->GetDlgItem(IDC_BTN_DUMP_IMAGE)->EnableWindow(FALSE);
	(CButton *)pThis->GetDlgItem(IDC_BTN_DL_DUMPIMAGE)->EnableWindow(FALSE);
	(CButton *)pThis->GetDlgItem(IDC_BTN_SaveImage)->EnableWindow(FALSE);
	int nComPort = 0;
	if(pThis->GetComPort(nComPort))
	{
		if(pThis->m_nPlatform == 6245)
		{
			CDownloadDumpImage dl(nComPort, "Qualcomm6245");
			dl.SetHex(pThis->m_strHexPath);
			dl.SetStartAddress(pThis->m_dwStartAddress);
			dl.SetBody(pThis->m_strCefs);
			bool bDownload = dl.Download(0);
			pThis->AddMessage(bDownload ? "Download succeed" : "Download fail");
		}
		else if(pThis->m_nPlatform == 6240)
		{
			CDownloadDumpImage dl(nComPort, "Qualcomm6240");
			dl.SetHex(pThis->m_strHexPath);
			dl.SetStartAddress(pThis->m_dwStartAddress);
			dl.SetBody(pThis->m_strCefs);
			bool bDownload = dl.Download6240(0);
			pThis->AddMessage(bDownload ? "Download succeed" : "Download fail");
		}
		else
		{
			pThis->AddMessage("Please define Platform(6245 or 6240) in Setup.ini");
		}
	}

	(CButton *)pThis->GetDlgItem(IDC_BTN_DUMP_IMAGE)->EnableWindow(TRUE);
	(CButton *)pThis->GetDlgItem(IDC_BTN_DL_DUMPIMAGE)->EnableWindow(TRUE);
	(CButton *)pThis->GetDlgItem(IDC_BTN_SaveImage)->EnableWindow(TRUE);
	return 0;
}

void CDLTesterDlg::OnBnClickedBtnDumpImage()
{
	if(GetIniInfo("DumpImage") )
	{
		if(::_taccess_s(m_strHexPath.c_str(),0) == 0 && !m_strRawRequest.empty())
		{
			g_pThis = this;
			AfxBeginThread(DumpImageThread,g_pThis,THREAD_PRIORITY_NORMAL,0,0,NULL);
		}
		else
		{
			AddMessage("Information from IniFile is not enough.");
		}
	}
}

UINT CDLTesterDlg::DumpImageThread(LPVOID pParam)
{
	CDLTesterDlg* pThis = (CDLTesterDlg*)pParam;
	(CButton *)pThis->GetDlgItem(IDC_BTN_DUMP_IMAGE)->EnableWindow(FALSE);
	(CButton *)pThis->GetDlgItem(IDC_BTN_DL_DUMPIMAGE)->EnableWindow(FALSE);
	(CButton *)pThis->GetDlgItem(IDC_BTN_SaveImage)->EnableWindow(FALSE);
	int nComPort = 0;
	if(pThis->GetComPort(nComPort))
	{
		pThis->m_ptBuffer->SetEmpty();
		//////////////////////////////////////////////////////////////////////////
		vector<CString> vecInput;
		QSYNC_LIB::Tokenize<CString>(_T(" "), pThis->m_strRawRequest.c_str(), vecInput, false);
		char* cmd = new char[vecInput.size()];
		for (vector<CString>::size_type nIndex = 0; nIndex < vecInput.size(); ++nIndex)
		{
			if(vecInput.at(nIndex).Left(2).MakeLower() == _T("0x"))
				vecInput.at(nIndex) = vecInput.at(nIndex).Mid(2);
			cmd[nIndex] = static_cast<char>(QSYNC_LIB::HEX2Ineger((vecInput.at(nIndex).GetString())));
		}
		//////////////////////////////////////////////////////////////////////////
		bool bDump = false;
		if(pThis->m_nPlatform == 6245)
		{
			CDownload6245 dl(nComPort);

			if(dl.DMSSDownload(pThis->m_strHexPath) && dl.StreamingHello(false, 50, 700))
			{
				bool bStopAccepting = false;
				do 
				{
					bDump = dl.DumpImage(bStopAccepting, cmd, vecInput.size(), pThis->m_nRawRequestTimeOut, pThis->m_ptBuffer);
				} while(!bStopAccepting && bDump);
			}
		}
		else if(pThis->m_nPlatform == 6240)
		{
			CDownload6240 dl(nComPort);

			if(dl.DMSSDownload(pThis->m_strHexPath))
			{
				Sleep(35000);
				if(dl.StreamingHello(false, 50, 700))
				{
					bool bStopAccepting = false;
					do 
					{
						bDump = dl.DumpImage(bStopAccepting, cmd, vecInput.size(), pThis->m_nRawRequestTimeOut, pThis->m_ptBuffer);
					} while(!bStopAccepting && bDump);
				}
				else
					pThis->AddMessage("Fail in Hello");
			}
			else
				pThis->AddMessage("Fail in DMSS Download");
		}
		else
		{
			pThis->AddMessage("Please define Platform(6245 or 6240) in Setup.ini");
		}

		pThis->AddMessage(bDump? "Dump Succeed" : "Dump Fail");
		if(bDump)
			pThis->OnBnClickedBtnSaveimage();
	}

	(CButton *)pThis->GetDlgItem(IDC_BTN_DUMP_IMAGE)->EnableWindow(TRUE);
	(CButton *)pThis->GetDlgItem(IDC_BTN_DL_DUMPIMAGE)->EnableWindow(TRUE);
	(CButton *)pThis->GetDlgItem(IDC_BTN_SaveImage)->EnableWindow(TRUE);
	return 0;
}

void CDLTesterDlg::OnBnClickedBtnSaveimage()
{
	CFile file;
	CFileException e;
	if(file.Open("Image.bin", CFile::modeWrite | CFile::modeCreate, &e))
	{
		file.Write(m_ptBuffer->GetData(), m_ptBuffer->GetLength());
		m_ptBuffer->SetEmpty();
		file.Close();
		AddMessage("SaveImage succeed.");
	}
	else
		AddMessage("SaveImage fail");
	
}

void CDLTesterDlg::OnBnClickedBtnClearboard()
{
	m_ListBoxReult.ResetContent();	
}
void CDLTesterDlg::AddMessage(LPCTSTR lpMsg)
{
	CTime t = CTime::GetCurrentTime();
	CString strMsg = t.Format("[%Y/%m/%d, %H:%M:%S] ");
	strMsg += lpMsg;
	m_ListBoxReult.AddString(strMsg);
}