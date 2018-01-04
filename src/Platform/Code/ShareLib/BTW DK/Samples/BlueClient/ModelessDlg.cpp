// ModelessDlg.cpp : implementation file
//
//  Copyright (c) 2000-2004, Broadcom Corporation, All Rights Reserved.

#include "stdafx.h"
//#include "BlueClient.h"
#include "ModelessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModelessDlg dialog


CModelessDlg::CModelessDlg(UINT id, CWnd* pParent /*=NULL*/)
	: CDialog(id, pParent)
//	: CDialog(CModelessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModelessDlg)
		// NOTE: the ClassWizard will add member initialization here
	m_id = id;
	//}}AFX_DATA_INIT
}


void CModelessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModelessDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModelessDlg, CDialog)
	//{{AFX_MSG_MAP(CModelessDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDOK, OnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModelessDlg message handlers
BOOL CModelessDlg::Create()
{
    CDialog::Create (m_id);
    ShowWindow (SW_SHOW);
    return (TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CModelessDlg message handlers

void CModelessDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	delete this;
}

void CModelessDlg::OnCancel() 
{
    DestroyWindow();	
}

void CModelessDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

void CModelessDlg::OnOK() 
{
    DestroyWindow();	
}
