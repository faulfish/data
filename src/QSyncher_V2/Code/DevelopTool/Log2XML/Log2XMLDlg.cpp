// Log2XMLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Log2XML.h"
#include "Log2XMLDlg.h"

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


// CLog2XMLDlg dialog




CLog2XMLDlg::CLog2XMLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLog2XMLDlg::IDD, pParent)
	, m_strOutput(_T(""))
	, m_strLog(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLog2XMLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITXML, m_strOutput);
	DDX_Text(pDX, IDC_EDITLOG, m_strLog);
}

BEGIN_MESSAGE_MAP(CLog2XMLDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTONTRANSFER, &CLog2XMLDlg::OnBnClickedButtontransfer)
	ON_BN_CLICKED(IDC_CLEAR_BUT, &CLog2XMLDlg::OnBnClickedClear)
END_MESSAGE_MAP()


// CLog2XMLDlg message handlers

BOOL CLog2XMLDlg::OnInitDialog()
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

void CLog2XMLDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLog2XMLDlg::OnPaint()
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
HCURSOR CLog2XMLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CLog2XMLDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}

CString GetSubstringAfterBy(const CString& strOrg, const CString& strFind)
{
	CString strResult;
	int nFindIndex = strOrg.Find(strFind);
	if(nFindIndex >=0 )
		strResult = strOrg.Mid(nFindIndex+strFind.GetLength());
	ASSERT(!strResult.IsEmpty());
	return strResult;
}

#include "..\..\CommonUtil\unicodeutility.h"
#include "atlenc.h"
CString GetHEX(const CString& strHEX)
{
	int nLen = 0;
	for(;AtlGetHexValue(strHEX[nLen]) != -1;nLen++);
	CString strValue = strHEX.Left(nLen);
	return strValue;
}

#include "..\..\Components\WBXML\wbxmlSyncml.h"
#include "..\..\CommLayer\ProtocolCodec\ObexCodec.h"
CString AddWbxmlToEdit(BYTE* pByte,DWORD dwSize)
{	
	CString strTemp;
	CBufferPtr spBuffer = new CBuffer(dwSize,(const char*)pByte);
	CObexCodecPtr spObex = new CObexCodec();	
	spObex->DecodePackage(spBuffer,CObexCodec::oUnknown);
	spBuffer = spObex->GetBody();
	ASSERT(spBuffer);
	if(spBuffer)
	{
		DWORD dwLen = 0;		
		WbxmlToXml((BYTE*)spBuffer->GetData(),spBuffer->GetLength(),strTemp,dwLen);
	}
	return strTemp;
}

void CLog2XMLDlg::OnBnClickedClear()
{	
	CEdit* pEditSrc = (CEdit*)GetDlgItem(IDC_EDITLOG);
	if(pEditSrc && ::IsWindow(pEditSrc->GetSafeHwnd()))
		pEditSrc->SetWindowText(_T(""));

	UpdateData(TRUE);
}

#include <vector>
void CLog2XMLDlg::OnBnClickedButtontransfer()
{
	UpdateData(TRUE);

	m_strOutput = GetSubstringAfterBy(m_strLog,_T("(Endpoint Address: 0x"));
	CString strType = GetHEX(m_strOutput);
	if(strType == _T("82"))
		m_strOutput = GetSubstringAfterBy(m_strOutput,_T("Get 0x"));
	else
		m_strOutput = GetSubstringAfterBy(m_strOutput,_T("Send 0x"));

	CString strTotal = GetHEX(m_strOutput);
	DWORD dwTotal = QSYNC_LIB::HEX2Ineger(strTotal);
	std::vector<BYTE> vectBytes;
	while(vectBytes.size() < dwTotal && m_strOutput.GetLength() > 0)
	{
		m_strOutput = GetSubstringAfterBy(m_strOutput,_T("\r\n"));
		if(m_strOutput.GetLength() > 0)
		{
			if(m_strOutput[0] == _T(' '))
			{
				for(int nIndex = 0;nIndex<16;nIndex++)
				{
					CString strHEX = m_strOutput.Mid(nIndex*3+1,2);
					BYTE byteTotal = static_cast<BYTE>(QSYNC_LIB::HEX2Ineger(strHEX));
					vectBytes.push_back(byteTotal);
					if(vectBytes.size() == 3)
					{
						dwTotal = MAKEWORD(vectBytes[2],vectBytes[1]);
					}
					if(vectBytes.size() >= dwTotal)
						break;
				}
			}
		}
		else
		{
			AfxMessageBox(_T("Please check source is complete package of OBEX"));
		}
	}
	BYTE* byteArray = new BYTE[vectBytes.size()];
	for(size_t nIndex=0;nIndex<vectBytes.size();nIndex++)
		byteArray[nIndex] = vectBytes[nIndex];
	m_strOutput = AddWbxmlToEdit(byteArray,static_cast<DWORD>(vectBytes.size()));
	delete [] byteArray;

	UpdateData(FALSE);
}

