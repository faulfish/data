// SO.cpp : implementation file
//

#include "stdafx.h"
#include "q_download.h"
#include "SO.h"
#include "PASSWORD.h"
//#include "extern.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSO dialog
CString so;


CSO::CSO(CWnd* pParent /*=NULL*/)
	: CDialog(CSO::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSO)
	//}}AFX_DATA_INIT
}


void CSO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSO)
	DDX_Control(pDX, IDC_SO_NUMBER, m_so);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSO, CDialog)
	//{{AFX_MSG_MAP(CSO)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSO message handlers

BOOL CSO::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	HBITMAP BitMap_OK=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_OK));
	((CButton *)GetDlgItem(IDOK))->SetBitmap(BitMap_OK);
	m_so.SetFocus();
	m_so.SetLimitText(5);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSO::OnOK() 
{
	// TODO: Add extra validation here
	m_so.GetWindowText(so);
	if(so.GetLength()!=5)
	{
		AfxMessageBox("Pls enter five number as so no.!");
		m_so.SetWindowText("");
		m_so.SetFocus();
		return;
	}
	CString inifilename;
	inifilename=".\\SETUP.ini";
	char temp[15];
	CString SONum;
    ::GetPrivateProfileString("SO_NUMBER","OLD","12345",temp,sizeof(temp),inifilename);
    SONum=temp;
	if(so!=SONum)
	{
		CPASSWORD pwd;
		if( pwd.DoModal() == IDCANCEL)
		{
		//	AfxMessageBox("Password is error!");
			m_so.SetWindowText("");
			m_so.SetFocus();
			return;
		} 
		else
		{
			WritePrivateProfileString("SO_NUMBER","OLD",so,inifilename);
		}
		
	}

	CDialog::OnOK();
}

BOOL CSO::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
			return true;
			break;
		case VK_RETURN:
			if(pMsg->hwnd==GetDlgItem(IDC_SO_NUMBER)->m_hWnd)
			{
				OnOK();
				return true;
			}
			break;
		default:
			break;
		}
		default:
			break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
