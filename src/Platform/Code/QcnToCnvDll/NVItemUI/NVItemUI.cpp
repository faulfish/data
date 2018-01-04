// NVItemUI.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "NVItemUI.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "NVItemUIDoc.h"
#include "NVItemUIView.h"
#include "NVItemTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNVItemUIApp

BEGIN_MESSAGE_MAP(CNVItemUIApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CNVItemUIApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	ON_UPDATE_COMMAND_UI(ID_OPEN_NVFILE, &CNVItemUIApp::OnUpdateOpenNvfile)
	ON_COMMAND(ID_VIEW_NVITEMVIEW, &CNVItemUIApp::OnViewNvitemview)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NVITEMVIEW, &CNVItemUIApp::OnUpdateViewNvitemview)
END_MESSAGE_MAP()


// CNVItemUIApp construction

CNVItemUIApp::CNVItemUIApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bIsLoadFileOpen = TRUE;
	m_bIsNVItemViewOpen = FALSE;
}

CNVItemUIApp::~CNVItemUIApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CNVItemUIApp object

CNVItemUIApp theApp;


// CNVItemUIApp initialization

BOOL CNVItemUIApp::InitInstance()
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

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	
CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_LoadFileTYPE,
		RUNTIME_CLASS(CNVItemUIDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CNVItemTreeView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd


	// Parse command line for standard shell commands, DDE, file open
	
	//CCommandLineInfo cmdInfo;
	//ParseCommandLine(cmdInfo);


	//// Dispatch commands specified on the command line.  Will return FALSE if
	//// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	//if (!ProcessShellCommand(cmdInfo))
	//	return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CNVItemUIApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CNVItemUIApp message handlers

void CNVItemUIApp::OnUpdateOpenNvfile(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bIsLoadFileOpen);
}

bool CNVItemUIApp::OpenTemplate(const CString szWndTitle)
{
	POSITION		pos;
	CDocTemplate*	pTemplate;
	BOOL			bRet = FALSE;
	CString			strWndTitle;

	pos = GetFirstDocTemplatePosition();
	while( (pos != NULL) && !bRet )
	{
		pTemplate = GetNextDocTemplate(pos);
		if(pTemplate->GetDocString(strWndTitle, CDocTemplate::windowTitle))
		{
			if(strWndTitle == szWndTitle)
			{
				ASSERT(pTemplate != NULL);
				ASSERT_KINDOF(CDocTemplate, pTemplate);
				if( pTemplate->OpenDocumentFile(NULL))
					bRet = TRUE;
			}
		}
	}

	return bRet;
}


void CNVItemUIApp::OnViewNvitemview()
{
	// TODO: Add your command handler code here
	CString title = _T("Load File");
	if(OpenTemplate(title))
	{
		m_bIsLoadFileOpen = FALSE;
		m_bIsNVItemViewOpen = TRUE;
	}
}

void CNVItemUIApp::OnUpdateViewNvitemview(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
		pCmdUI->Enable(!m_bIsNVItemViewOpen);
}
