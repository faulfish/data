// ServiceProgramTool.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ServiceProgramTool.h"
#include "SecurityToolDlg.h"
#include "..\STE\MiddleWare\CallbackTypes.h"

static char *MsgBoxTitle = "SecurityTool";

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CServiceProgramToolApp, CWinApp)
END_MESSAGE_MAP()
// CServiceProgramToolApp construction

CServiceProgramToolApp::CServiceProgramToolApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CServiceProgramToolApp object

CServiceProgramToolApp theApp;

BOOL CServiceProgramToolApp::InitInstance()
{
	return CWinApp::InitInstance();
}

int CServiceProgramToolApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}

void CServiceProgramToolApp::ShowServiceProgramTool()
{
	CSecurityToolDlg dlg;
	dlg.DoModal();
}

