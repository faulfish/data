// IPhonebookTestPage.cpp : implementation file
//

#include "stdafx.h"
#include "TestMInterfaceApp.h"
#include "IPhonebookTestPage.h"


// CIPhonebookTestPage dialog

IMPLEMENT_DYNAMIC(CIPhonebookTestPage, CDialog)

CIPhonebookTestPage::CIPhonebookTestPage(CWnd* pParent /*=NULL*/)
	: CTabPageSSL(CIPhonebookTestPage::IDD, pParent)
{

}

CIPhonebookTestPage::~CIPhonebookTestPage()
{
}

void CIPhonebookTestPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIPhonebookTestPage, CDialog)
END_MESSAGE_MAP()


// CIPhonebookTestPage message handlers
