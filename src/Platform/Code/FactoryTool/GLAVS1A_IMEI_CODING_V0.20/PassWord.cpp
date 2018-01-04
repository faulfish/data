// PassWord.cpp : implementation file
//

#include "stdafx.h"
#include "QUALCOMM_IMEI_CODING.h"
#include "PassWord.h"
#include "Global_Variable_Declare.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPassWord dialog


CPassWord::CPassWord(CWnd* pParent /*=NULL*/)
	: CDialog(CPassWord::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPassWord)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPassWord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPassWord)
	DDX_Control(pDX, IDC_STATIC_PASSWORD, m_PASSWORD);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_PASSWORD_EDIT);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPassWord, CDialog)
	//{{AFX_MSG_MAP(CPassWord)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPassWord message handlers

HBRUSH CPassWord::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(nCtlColor ==CTLCOLOR_DLG)
         return m_brush;

	if((pWnd->GetDlgCtrlID()==IDC_EDIT_PASSWORD))   
	{   
		pDC->SetTextColor(RGB(125,0,255));   
	} 
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CPassWord::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_brush.CreateSolidBrush(RGB(200,200,100));

	m_PASSWORD.SetBkColor(RGB(200, 200, 100));
	m_PASSWORD.SetTextColor(RGB(0, 0, 255));
    m_PASSWORD.SetFontBold(TRUE);

	((CButton *)GetDlgItem(IDOK))->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_OK)));
    ((CButton *)GetDlgItem(IDCANCEL))->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_EXIT)));
	CFont   font1;   
    font1.CreatePointFont(15*8,   "Arial");   
    m_PASSWORD_EDIT.SetFont(&font1,1);
	return TRUE;  // return TRUE unless you set the focus to a control	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPassWord::OnOK() 
{
	// TODO: Add extra validation here
	CString PassWord;
	m_PASSWORD_EDIT.GetWindowText(PassWord);
	if((CheckDiff&&(PassWord!="1829"))||(SetConfig&&(PassWord!="1829")))
	{
		AfxMessageBox("Password error!!");
		CDialog::OnCancel();
	}
	else
	{	
		CDialog::OnOK();
	}
}
