// QCN2CNVUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QCN2CNVUI.h"
#include "QCN2CNVUIDlg.h"

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


CQCN2CNVUIDlg::CQCN2CNVUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQCN2CNVUIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQCN2CNVUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_QCNPATH, m_QCNCtrl);
	DDX_Control(pDX, IDC_CNVPATH, m_CNVCtrl);
	DDX_Control(pDX, IDC_GetDU, m_DUCtrl);
	DDX_Control(pDX, IDC_DUPATH, m_DUEditCtrl);
	DDX_Control(pDX, IDC_SRCCNVPATH, m_SRCCNVCtrl);
	DDX_Control(pDX, IDC_NEW_QCN_PATH, m_NEWQCNCtrl);
}

BEGIN_MESSAGE_MAP(CQCN2CNVUIDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_GetQCN, &CQCN2CNVUIDlg::OnBnClickedGetqcn)
	ON_BN_CLICKED(IDC_GetDU, &CQCN2CNVUIDlg::OnBnClickedGetdu)
	ON_BN_CLICKED(IDC_CONVERT_BUTTON, &CQCN2CNVUIDlg::OnBnClickedConvertButton)
	ON_BN_CLICKED(IDC_GETCNV, &CQCN2CNVUIDlg::OnBnClickedGetcnv)
	ON_BN_CLICKED(IDC_CNV2QCN__BUTTON, &CQCN2CNVUIDlg::OnBnClickedCnv2qcn)
	ON_BN_CLICKED(IDC_CONVERT_TO_BRT_BUTTON, &CQCN2CNVUIDlg::OnBnClickedConvertToBrtButton)
END_MESSAGE_MAP()


// CQCN2CNVUIDlg message handlers

BOOL CQCN2CNVUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

void CQCN2CNVUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CQCN2CNVUIDlg::OnPaint()
{
	CDialog::OnPaint();
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CQCN2CNVUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#include "..\QcnToCnvDLL\QcnToCnvDll.h"
#include "Shlwapi.h"

void CQCN2CNVUIDlg::OnBnClickedGetqcn()
{
	TCHAR szFilters[]=  _T("QCN Files (*.qcn)||");
   CFileDialog fileDlg (TRUE, _T("QCN"), _T("*.qcn"),OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);

   CString strQCNPath;
	if(IDOK == fileDlg.DoModal())
	{		
		strQCNPath = fileDlg.GetPathName();
		
		m_QCNCtrl.SetWindowText(strQCNPath);		
	}
}

void CQCN2CNVUIDlg::OnBnClickedGetdu()
{
	TCHAR szFilters[]=  _T("DU Files (*.du)||");
   CFileDialog fileDlg (TRUE, _T("DU"), _T("*.du"),OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);

   CString strDUPath;
	if(IDOK == fileDlg.DoModal())
	{		
		strDUPath = fileDlg.GetPathName();
		
		m_DUEditCtrl.SetWindowText(strDUPath);		
	}
}

void CQCN2CNVUIDlg::OnBnClickedConvertButton()
{
	CString strQCNPath;
	m_QCNCtrl.GetWindowText(strQCNPath);

	CString strDUPath;
	m_DUEditCtrl.GetWindowText(strDUPath);
	
	CString strCNVPath = strQCNPath;
	PathRemoveExtension(strCNVPath.GetBuffer(strCNVPath.GetLength()));
	strCNVPath.ReleaseBuffer();
	strCNVPath += _T(".CNV");

	QcnAndDUToCnvFunction(strDUPath,strQCNPath,strCNVPath);
	m_CNVCtrl.SetWindowText(strCNVPath);		
}

void CQCN2CNVUIDlg::OnBnClickedGetcnv()
{
	TCHAR szFilters[]=  _T("CNV Files (*.cnv)||");
   CFileDialog fileDlg (TRUE, _T("CNV"), _T("*.cnv"),OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);

   CString strCNVPath;
	if(IDOK == fileDlg.DoModal())
	{		
		strCNVPath = fileDlg.GetPathName();
		
		m_SRCCNVCtrl.SetWindowText(strCNVPath);		
	}
}

void CQCN2CNVUIDlg::OnBnClickedCnv2qcn()
{
	CString strSRCCNVPath;
	m_SRCCNVCtrl.GetWindowText(strSRCCNVPath);
	
	CString strNewQCNPath = strSRCCNVPath;
	PathRemoveExtension(strNewQCNPath.GetBuffer(strNewQCNPath.GetLength()));
	strNewQCNPath.ReleaseBuffer();
	strNewQCNPath += _T(".QCN");

	CnvToQcnFunction(strSRCCNVPath,strNewQCNPath);
	m_NEWQCNCtrl.SetWindowText(strNewQCNPath);
}

void CQCN2CNVUIDlg::OnBnClickedConvertToBrtButton()
{
	CString strQCNPath;
	m_QCNCtrl.GetWindowText(strQCNPath);

	CString strDUPath;
	m_DUEditCtrl.GetWindowText(strDUPath);
	
	CString strBRTPath = strQCNPath;
	PathRemoveExtension(strBRTPath.GetBuffer(strBRTPath.GetLength()));
	strBRTPath.ReleaseBuffer();
	strBRTPath += _T(".BRT");

	QcnAndDUToBRTFunction(strDUPath,strQCNPath,strBRTPath);
	m_CNVCtrl.SetWindowText(strBRTPath);	
}
