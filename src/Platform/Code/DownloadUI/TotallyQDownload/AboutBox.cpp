// AboutBox.cpp : implementation file
//

#include "stdafx.h"
#include "DownloadUI.h"
#include "AboutBox.h"


// CAboutBox dialog

IMPLEMENT_DYNAMIC(CAboutBox, CDialog)

CAboutBox::CAboutBox(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutBox::IDD, pParent)
{

}

CAboutBox::~CAboutBox()
{
}

void CAboutBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutBox, CDialog)
END_MESSAGE_MAP()


// CAboutBox message handlers
