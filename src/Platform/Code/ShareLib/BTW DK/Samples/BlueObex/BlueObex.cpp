// BlueObex.cpp : Defines the class behaviors for the application.
//
//  Copyright (c) 2000-2004, Broadcom Corporation, All Rights Reserved.

#include "stdafx.h"
#include "BlueObex.h"
#include "ConnectionMgrDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlueObexApp

BEGIN_MESSAGE_MAP(CBlueObexApp, CWinApp)
	//{{AFX_MSG_MAP(CBlueObexApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlueObexApp construction

CBlueObexApp::CBlueObexApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBlueObexApp object

CBlueObexApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CBlueObexApp initialization

BOOL CBlueObexApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#if ( _MFC_VER < 0x0800 )   // deprecated in MFC 8   
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

//	CBlueObexDlg dlg;
	CConnectionMgrDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
