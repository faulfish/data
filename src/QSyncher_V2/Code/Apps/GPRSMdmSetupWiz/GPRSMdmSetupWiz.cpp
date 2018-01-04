// GPRSMdmSetupWiz.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GPRSMdmSetupWiz.h"
#include "GPRSMdmSetupPropertySheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGPRSMdmSetupWizApp

BEGIN_MESSAGE_MAP(CGPRSMdmSetupWizApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGPRSMdmSetupWizApp construction

CGPRSMdmSetupWizApp::CGPRSMdmSetupWizApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
    m_OperatorAPNs.clear();
    m_OperatorAPNs.clear();
    CString szDirPath;
    GetDir(szDirPath);
    CString strAPNFileName = CString(szDirPath) + CString(_T("\\ModemSetupSetting\\")) + CString(_T("Operator APNs.csv"));
    CStdioFile file(strAPNFileName, CFile::modeRead);
    CString strLine;
    file.ReadString(strLine);   ///< filter the first line
    while(file.ReadString(strLine)) {
        if(strLine == _T("")) break;
        int nPos = 0;
        CString strCountry = strLine.Tokenize(_T(","), nPos);
        CString strOperator = strLine.Tokenize(_T(","), nPos);
        CString strAPN = strLine.Tokenize(_T(","), nPos);
        m_CountryOperators[(LPCTSTR)strCountry].push_back((LPCTSTR)strOperator);
        m_OperatorAPNs[(LPCTSTR)strOperator].push_back((LPCTSTR)strAPN);
    }
}


// The one and only CGPRSMdmSetupWizApp object

CGPRSMdmSetupWizApp theApp;


// CGPRSMdmSetupWizApp initialization

BOOL CGPRSMdmSetupWizApp::InitInstance()
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

	CGPRSMdmSetupPropertySheet propSheet;
	propSheet.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
