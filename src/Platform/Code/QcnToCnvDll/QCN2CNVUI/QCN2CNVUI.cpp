// QCN2CNVUI.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "QCN2CNVUI.h"
#include "QCN2CNVUIDlg.h"
#include "..\QcnToCnvDLL\QcnToCnvDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CQCN2CNVUIApp

BEGIN_MESSAGE_MAP(CQCN2CNVUIApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CQCN2CNVUIApp construction

CQCN2CNVUIApp::CQCN2CNVUIApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CQCN2CNVUIApp object

CQCN2CNVUIApp theApp;


// CQCN2CNVUIApp initialization

BOOL CQCN2CNVUIApp::InitInstance()
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

	//CCommandLineInfo cmdInfo;
	//ParseCommandLine(cmdInfo);

	CWinApp* pApp = AfxGetApp();
	CString strCmdLine = pApp->m_lpCmdLine;
	CString resToken;

	CString strQCNPath,strCNVPath;
	CString strDUPath;
	int nStart = 0;
	resToken = strCmdLine.Tokenize(_T(" "),nStart);
	strQCNPath = resToken;	
	if(nStart >= 0){
		resToken = strCmdLine.Tokenize(_T(" "),nStart);
		strDUPath = resToken;
		resToken = strCmdLine.Tokenize(_T(" "),nStart);
		strCNVPath = resToken;
	}

	if(strQCNPath.IsEmpty() == false && strCNVPath.IsEmpty() == false)
	{
		QcnAndDUToCnvFunction(strDUPath,strQCNPath,strCNVPath);
	}
	else
	{
		CQCN2CNVUIDlg dlg;
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
