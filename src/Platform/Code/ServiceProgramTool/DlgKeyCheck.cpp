// DlgKeyCheck.cpp : implementation file
//

#include "stdafx.h"
#include "DlgKeyCheck.h"


// CDlgKeyCheck dialog

IMPLEMENT_DYNAMIC(CDlgKeyCheck, CDialog)

CDlgKeyCheck::CDlgKeyCheck(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgKeyCheck::IDD, pParent)
	, m_strUserCode(_T(""))
	, m_strFirmCode(_T(""))
{

}

CDlgKeyCheck::~CDlgKeyCheck()
{
}

void CDlgKeyCheck::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_strUserCode);
	DDX_Text(pDX, IDC_EDIT1, m_strFirmCode);
}


BEGIN_MESSAGE_MAP(CDlgKeyCheck, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgKeyCheck::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgKeyCheck message handlers

void CDlgKeyCheck::OnBnClickedOk()
{
	UpdateData();
	CDialog::OnOK();
}
