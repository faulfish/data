// IIrMCTestPage.cpp : implementation file
//

#include "stdafx.h"
#include "TestMInterfaceApp.h"
#include "IIrMCTestPage.h"


// CIIrMCTestPage dialog

IMPLEMENT_DYNAMIC(CIIrMCTestPage, CDialog)

CIIrMCTestPage::CIIrMCTestPage(CWnd* pParent /*=NULL*/)
	: CTabPageSSL(CIIrMCTestPage::IDD, pParent)
{

}

CIIrMCTestPage::~CIIrMCTestPage()
{
}

void CIIrMCTestPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIIrMCTestPage, CDialog)
	ON_BN_CLICKED(IDC_NEW_INTERFACE, &CIIrMCTestPage::OnBnClickedNewInterface)
END_MESSAGE_MAP()


// CIIrMCTestPage message handlers

void CIIrMCTestPage::OnBnClickedNewInterface()
{
	// TODO: Add your control notification handler code here
}
