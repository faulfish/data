// QUALCOMM_IMEI_CODING.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "QUALCOMM_IMEI_CODING.h"
#include "QUALCOMM_IMEI_CODINGDlg.h"
#include "Global_Variable_Declare.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQUALCOMM_IMEI_CODINGApp

BEGIN_MESSAGE_MAP(CQUALCOMM_IMEI_CODINGApp, CWinApp)
	//{{AFX_MSG_MAP(CQUALCOMM_IMEI_CODINGApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQUALCOMM_IMEI_CODINGApp construction

CQUALCOMM_IMEI_CODINGApp::CQUALCOMM_IMEI_CODINGApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CQUALCOMM_IMEI_CODINGApp object

CQUALCOMM_IMEI_CODINGApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CQUALCOMM_IMEI_CODINGApp initialization

BOOL CQUALCOMM_IMEI_CODINGApp::InitInstance()
{
	AfxEnableControlContainer();
    if(!AfxOleInit())//³õÊ¼»¯COM¿â
	{
      AfxMessageBox("OLE Initial Fail");
	  return FALSE;
	}
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CQUALCOMM_IMEI_CODINGDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
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


BOOL CQUALCOMM_IMEI_CODINGApp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (pMsg->message)
	{
		case WM_LBUTTONDBLCLK:
			::SendMessage(Global_WND,WM_RESETTESTCOUNT,(WPARAM)pMsg->pt.x,(LPARAM)pMsg->pt.y);
		default:
			break;
	}
	return CWinApp::PreTranslateMessage(pMsg);
}
