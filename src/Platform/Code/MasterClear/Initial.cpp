// Initial.cpp : implementation file
//

#include "stdafx.h"
#include "LC.h"
#include "Initial.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInitial dialog
CString SW="",HW="",mode="";

CInitial::CInitial(CWnd* pParent /*=NULL*/)
	: CDialog(CInitial::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInitial)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CInitial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInitial)
	DDX_Control(pDX, IDC_COMBO_SW, m_combosw);
	DDX_Control(pDX, IDC_COMBO_MODE, m_combomode);
	DDX_Control(pDX, IDC_COMBO_HW, m_combohw);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInitial, CDialog)
	//{{AFX_MSG_MAP(CInitial)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInitial message handlers

BOOL CInitial::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	char ModelTemp[100];
	char ModelList[50];

	int Count=0;
	Count=GetPrivateProfileInt("SOFTWARE","SW_COUNT",1,".\\Setup.ini");
	for(int n=1;n<=Count;n++)
	{
		strcpy(ModelTemp,"SW_");
		sprintf(ModelTemp+3,"%d",n);
		GetPrivateProfileString("SOFTWARE",ModelTemp,"1",ModelList,50,".\\Setup.ini");
		m_combosw.AddString(ModelList);
	}
	m_combosw.SetCurSel(0);

	Count=GetPrivateProfileInt("HARDWARE","HW_COUNT",1,".\\Setup.ini");
	for(int n=1;n<=Count;n++)
	{
		strcpy(ModelTemp,"HW_");
		sprintf(ModelTemp+3,"%d",n);
		GetPrivateProfileString("HARDWARE",ModelTemp,"1",ModelList,50,".\\Setup.ini");
		m_combohw.AddString(ModelList);
	}
	m_combohw.SetCurSel(0);
    m_combomode.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInitial::OnOK() 
{
	// TODO: Add extra validation here
	m_combosw.GetLBText(m_combosw.GetCurSel(),SW);
	m_combohw.GetLBText(m_combohw.GetCurSel(),HW);
	m_combomode.GetLBText(m_combomode.GetCurSel(),mode);
	CDialog::OnOK();
}
