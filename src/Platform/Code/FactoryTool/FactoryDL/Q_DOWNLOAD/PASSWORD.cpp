// PASSWORD.cpp : implementation file
//

#include "stdafx.h"
#include "q_download.h"
#include "PASSWORD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPASSWORD dialog

CPASSWORD::CPASSWORD(CWnd* pParent /*=NULL*/)
	: CDialog(CPASSWORD::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPASSWORD)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPASSWORD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPASSWORD)
	DDX_Control(pDX, PASSWORD, m_password);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPASSWORD, CDialog)
	//{{AFX_MSG_MAP(CPASSWORD)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPASSWORD message handlers

void CPASSWORD::OnOK() 
{
	// TODO: Add extra validation here
	m_password.GetWindowText(m_strSOPW);

	if(m_strSOPW!="1829")
	{
		AfxMessageBox("Password error!!");
		CDialog::OnCancel();
	}
	else
	{	
		CDialog::OnOK();
	}

}

BOOL CPASSWORD::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	HBITMAP BitMap_OK=LoadBitmap(AfxGetInstanceHandle( ),MAKEINTRESOURCE(IDB_BITMAP_OK));
	((CButton *)GetDlgItem(IDOK))->SetBitmap(BitMap_OK);
    
	m_password.SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPASSWORD::PreTranslateMessage(MSG* pMsg) 
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
			OnOK();
			return true;
			break;
		default:
			break;
		}
		default:
			break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
