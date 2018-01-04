// WaitDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WaitDialog.h"


// CWaitDialog dialog

IMPLEMENT_DYNAMIC(CWaitDialog, CDialog)

CWaitDialog::CWaitDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CWaitDialog::IDD, pParent)
{

}

CWaitDialog::~CWaitDialog()
{
}

void CWaitDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWaitDialog, CDialog)
END_MESSAGE_MAP()


// CWaitDialog message handlers

void CWaitDialog::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class

    CDialog::OnOK();
}
