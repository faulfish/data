// CodecTestToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CodecTestTool.h"
#include "CodecTestToolDlg.h"
#include <atlenc.h>
#include "..\..\CommonUtil\UnicodeUtility.h"

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


// CCodecTestToolDlg dialog




CCodecTestToolDlg::CCodecTestToolDlg(CWnd* pParent /*=NULL*/)
: CDialog(CCodecTestToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCodecTestToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCodecTestToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_PROCRESS1_COMBO, &CCodecTestToolDlg::OnCbnSelchangeProcress1Combo)
	ON_CBN_SELCHANGE(IDC_PROCRESS2_COMBO, &CCodecTestToolDlg::OnCbnSelchangeProcress2Combo)
	ON_BN_CLICKED(IDC_TRANS_BUT, &CCodecTestToolDlg::OnBnClickedTransBut)
END_MESSAGE_MAP()


// CCodecTestToolDlg message handlers

void CCodecTestToolDlg::DefinfeCombo(UINT nID)
{
	CComboBox* pComboProcess = (CComboBox*)GetDlgItem(nID);
	if(pComboProcess && ::IsWindow(pComboProcess->GetSafeHwnd()))
	{
		pComboProcess->AddString(_T("Nothing"));
		pComboProcess->AddString(_T("UTF8"));
		pComboProcess->AddString(_T("QP"));
	}
}

BOOL CCodecTestToolDlg::OnInitDialog()
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
	DefinfeCombo(IDC_PROCRESS1_COMBO);
	DefinfeCombo(IDC_PROCRESS2_COMBO);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCodecTestToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCodecTestToolDlg::OnPaint()
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
HCURSOR CCodecTestToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCodecTestToolDlg::OnCbnSelchangeProcress1Combo()
{
	// TODO: Add your control notification handler code here
}

void CCodecTestToolDlg::OnCbnSelchangeProcress2Combo()
{
	// TODO: Add your control notification handler code here
}

BYTE* QPDecode(BYTE* pOriginalBuffer)
{			
	BYTE* pReturnBuffer = NULL;
	int nRequird = QPDecodeGetRequiredLength(strlen((char*)pOriginalBuffer))+2;
	char* szQPBuffer = new char[nRequird];
	if(szQPBuffer)
	{
		::ZeroMemory(szQPBuffer,nRequird);
		QPDecode(pOriginalBuffer, strlen((char*)pOriginalBuffer), szQPBuffer, &nRequird);  
		if(pOriginalBuffer)
			delete [] pOriginalBuffer;
		pReturnBuffer = (BYTE*)szQPBuffer;
	}
	return pReturnBuffer;
}

BYTE* UTF8Decode(BYTE* pOriginalBuffer)
{			
	BYTE* pReturnBuffer = NULL;
	int nWideLen = ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pOriginalBuffer, -1, NULL, 0);
	wchar_t* lpwszUTF8Buffer = new wchar_t[nWideLen];
	if(lpwszUTF8Buffer)
	{
		::ZeroMemory(lpwszUTF8Buffer,nWideLen*sizeof(wchar_t));
		::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pOriginalBuffer, -1, lpwszUTF8Buffer, nWideLen);
		if(pOriginalBuffer)
			delete [] pOriginalBuffer;
		pReturnBuffer = (BYTE*)lpwszUTF8Buffer;
	}
	return pReturnBuffer;
}

void CCodecTestToolDlg::OnBnClickedTransBut()
{
	using namespace QSYNC_LIB ;	

	CString strSrc;
	CEdit* pEditSrc = (CEdit*)GetDlgItem(IDC_SRC);
	if(pEditSrc && ::IsWindow(pEditSrc->GetSafeHwnd()))
		pEditSrc->GetWindowText(strSrc);

	if(strSrc.IsEmpty() == false)
	{
		BYTE* pByteBuffer = NULL;

		//Always to ansi string frst
		int buffersize = (strSrc.GetLength()+1) * sizeof(TCHAR);
		char* pAnsiString = new char[buffersize];
#ifdef _UNICODE
		UnicodeToAnsiBuffer((LPCTSTR)strSrc.GetBuffer(strSrc.GetLength()), pAnsiString, buffersize);
#else
		::strcpy(pAnsiString,(LPCTSTR)strSrc.GetBuffer(strSrc.GetLength()));
#endif
		if(pByteBuffer)
			delete [] pByteBuffer;
		pByteBuffer = (BYTE*)pAnsiString;

		for(int i=0;i<2;i++)
		{
			CComboBox* pComboProcess = (CComboBox*)GetDlgItem(IDC_PROCRESS1_COMBO+i);
			if(pComboProcess && ::IsWindow(pComboProcess->GetSafeHwnd()))
			{
				int nSel = pComboProcess->GetCurSel();
				if(CB_ERR != nSel)
				{
					CString strSel;
					pComboProcess->GetLBText(nSel,strSel);
					if(strSel == _T("QP"))
						pByteBuffer = QPDecode(pByteBuffer);
					else if(strSel == _T("UTF8"))
						pByteBuffer = UTF8Decode(pByteBuffer);
				}
			}
		}

		CEdit* pEditTar = (CEdit*)GetDlgItem(IDC_TAR);
		if(pEditTar && ::IsWindow(pEditTar->GetSafeHwnd()))
			pEditTar->SetWindowText((wchar_t*)pByteBuffer);

		if(pByteBuffer)
		{
			delete [] pByteBuffer;
			pByteBuffer = NULL;
		}
	}
}
