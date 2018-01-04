// QFuseTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "QFuseTool.h"
#include "QFuseToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CQFuseToolApp

BEGIN_MESSAGE_MAP(CQFuseToolApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CQFuseToolApp construction

CQFuseToolApp::CQFuseToolApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CQFuseToolApp object

CQFuseToolApp theApp;


// CQFuseToolApp initialization

BOOL CQFuseToolApp::InitInstance()
{
	return TRUE;
}