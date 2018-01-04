// SPU_UI.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SPU_UI.h"
#include "SPU_UIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSPU_UIApp

BEGIN_MESSAGE_MAP(CSPU_UIApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSPU_UIApp construction

CSPU_UIApp::CSPU_UIApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSPU_UIApp object

CSPU_UIApp theApp;


// CSPU_UIApp initialization

BOOL CSPU_UIApp::InitInstance()
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

#ifdef _DEBUG
	CString strScript = _T("default_Debug.script");
#else
	CString strScript = _T("default.script");
#endif

	if (!strstr(theApp.m_lpCmdLine,"stationFactory:"))
	{
		if(_tcslen(theApp.m_lpCmdLine) != 0)
			strScript = theApp.m_lpCmdLine;
	}

	CSPU_UIDlg dlg((LPCTSTR)strScript);
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
