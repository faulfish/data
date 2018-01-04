// ServiceProgramTool.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ServiceProgramTool.h"
#include "SecurityToolDlg.h"
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CServiceProgramToolApp, CWinApp)
END_MESSAGE_MAP()
// CServiceProgramToolApp construction

const LPCTSTR DLGAPPNAME = _T("OneTimeProgramming");
const int LOGFILESIZE = 150000;

CServiceProgramToolApp::CServiceProgramToolApp(LPCTSTR lpszAppName)
:CWinApp(lpszAppName)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CServiceProgramToolApp object

CServiceProgramToolApp theApp(DLGAPPNAME);
CLogProxy theLog(_T("OTPTool.log"));

CLogProxy* GetLogInst()
{
	return &theLog;
}

BOOL CServiceProgramToolApp::InitInstance()
{
	return CWinApp::InitInstance();;
}

int CServiceProgramToolApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}

void CServiceProgramToolApp::ShowServiceProgramTool()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());  

	m_hResource = AfxGetResourceHandle();
	AfxSetResourceHandle(((CServiceProgramToolApp*)AfxGetApp())->m_hResource);

	CSecurityToolDlg dlg;
	dlg.DoModal();
}

CLogProxy::CLogProxy(LPCTSTR pLogFileName)
{
	m_strLogFileName = pLogFileName;
}

CLogProxy::~CLogProxy()
{
	if (NULL != m_pLogFile)
		fclose(m_pLogFile);
}

void CLogProxy::InitLogInst()
{
	if (m_pLogFile != NULL)
		return;

	TCHAR szPath[MAX_PATH];
	LPTSTR pszExtension;

	int ret = ::GetModuleFileName(AfxGetApp()->m_hInstance, szPath, MAX_PATH);
	if (ret == 0 || ret == MAX_PATH){
		ASSERT(FALSE);
		_tcscpy(szPath, _T("c:\\"));
	}
	::PathRemoveFileSpec(szPath);
	::PathAddBackslash(szPath);
	
	CString strLogPath = CString(szPath) + m_strLogFileName;

	m_pLogFile = _tfopen(strLogPath, _T("a"));

	if (m_pLogFile != NULL){
		fseek(m_pLogFile, 0L, SEEK_END);
		if (ftell(m_pLogFile) > LOGFILESIZE){
			fclose(m_pLogFile);
			time_t now = time(NULL);
			struct tm *today = localtime(&now);
			CString curTime;
			_tcsftime(curTime.GetBuffer(1024), 1024, _T("%m%d%H%M"),today);
			curTime.ReleaseBuffer();
			CString replaceFilePath = CString(strLogPath) + _T("_old_") + curTime;
			_trename(strLogPath, replaceFilePath);

			m_pLogFile = _tfopen(strLogPath, _T("a"));
		}
	}
	ASSERT (m_pLogFile != NULL);
}

void CLogProxy::LogMessage(const TCHAR *ID, const TCHAR *msg)
{	
	if (NULL == msg)
		return; 

	InitLogInst();

	LogID(ID);
	_ftprintf(m_pLogFile, _T("%s\n\n"), msg);
}

void CLogProxy::LogID(const TCHAR *ID)
{
	if ((NULL != ID) && (_T('\0') != ID[0]))
		_ftprintf(m_pLogFile, _T("[%s] "), ID);

	time_t now = time(NULL);

	_ftprintf(m_pLogFile, _T("%s"), _tctime(&now));
}