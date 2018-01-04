// DownloadUI.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DownloadUI.h"
#include "DownloadUIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace QSYNC_LIB
{

	// CDownloadUIApp

	BEGIN_MESSAGE_MAP(CDownloadUIApp, CWinApp)
		ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
	END_MESSAGE_MAP()


	// CDownloadUIApp construction

	CDownloadUIApp::CDownloadUIApp():m_pSingleInstanceObj(NULL)
	{
		CreateQSyncerLimitSingleInstance();
	}

	void CDownloadUIApp::CreateQSyncerLimitSingleInstance()
	{
		if (m_pSingleInstanceObj == NULL)
			m_pSingleInstanceObj = new CLimitSingleInstance(TEXT("Global\\{{9F0DC9FD-154D-4348-B088-11F9382E61A3}}"));//GUID of QDownload
	}

	// The one and only CDownloadUIApp object

	CDownloadUIApp theApp;


	// CDownloadUIApp initialization

	BOOL CDownloadUIApp::InitInstance()
	{		
		//If another QDownload is running, return false.
		if(m_pSingleInstanceObj && m_pSingleInstanceObj->IsAnotherInstanceRunning())
		{
			return FALSE; 
		}

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

		AfxEnableControlContainer();

		// Standard initialization
		// If you are not using these features and wish to reduce the size
		// of your final executable, you should remove from the following
		// the specific initialization routines you do not need
		// Change the registry key under which our settings are stored
		// TODO: You should modify this string to be something appropriate
		// such as the name of your company or organization
//		SetRegistryKey(_T("Local AppWizard-Generated Applications"));

		CDownloadUIDlg dlg;
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
}