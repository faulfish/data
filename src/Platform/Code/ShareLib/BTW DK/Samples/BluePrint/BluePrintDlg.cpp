// BluePrintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BluePrint.h"
#include "BluePrintDlg.h"
#include "ChooseServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CString BdAddrToString(BD_ADDR bd_addr);

/////////////////////////////////////////////////////////////////////////////
// CBluePrintDlg dialog

CBluePrintDlg::CBluePrintDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBluePrintDlg::IDD, pParent),
      m_isConnected(FALSE)
{
	//{{AFX_DATA_INIT(CBluePrintDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CBluePrintDlg::~CBluePrintDlg()
{
}


void CBluePrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBluePrintDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBluePrintDlg, CDialog)
	//{{AFX_MSG_MAP(CBluePrintDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START_BUTTON, OnStartButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBluePrintDlg message handlers

BOOL CBluePrintDlg::OnInitDialog()
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

void CBluePrintDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBluePrintDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CBluePrintDlg::OnStartButton() 
{
CChooseServerDlg dlg;
        
    dlg.DoModal();
	return;
     
}



//////////////////////////////////////
////////// Utility Functions //////////
///////////////////////////////////////

// Return a CString based on a
//  a BD Address (BD_ADDR type).
//
CString BdAddrToString(BD_ADDR bd_addr)
{
    CString s;

    s.Format("%x:%x:%x:%x:%x:%x",
             bd_addr[0],
             bd_addr[1],
             bd_addr[2],
             bd_addr[3],
             bd_addr[4],
             bd_addr[5]);

    return s;
}
