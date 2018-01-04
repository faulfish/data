// ImplementedDiagCommandTester.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ImplementedDiagCommandTester.h"
#include "ImplementedDiagCommandTesterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImplementedDiagCommandTesterApp

BEGIN_MESSAGE_MAP(CImplementedDiagCommandTesterApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CImplementedDiagCommandTesterApp construction

CImplementedDiagCommandTesterApp::CImplementedDiagCommandTesterApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CImplementedDiagCommandTesterApp object

CImplementedDiagCommandTesterApp theApp;


// CImplementedDiagCommandTesterApp initialization

BOOL CImplementedDiagCommandTesterApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	CImplementedDiagCommandTesterDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
