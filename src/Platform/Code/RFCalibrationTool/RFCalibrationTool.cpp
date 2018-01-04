// RFCalibrationTool.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "RFCalibrationTool.h"
#include "CalibrationTest.h"
#include <direct.h>
#include <time.h>
#include "RFCalibrationToolWnd.h"
#include "..\STE\MiddleWare\CallbackTypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

void ToolTraceLog(LPCTSTR lpszModule, LPCSTR lpszSrcFile, UINT iSrcLine, EMsgLevel msgLevel, LPCTSTR lpszMsgFormat, ...)
{
	CCalibrationTest *pCalTestObj = ((CRFCalibrationToolApp*)AfxGetApp())->GetCalibrationTestObj();

	if (pCalTestObj == NULL)
		return;		

	va_list args;
	va_start(args, lpszMsgFormat);
	CString strLogMsg;
	_vstprintf(strLogMsg.GetBuffer(1000), lpszMsgFormat, args);
	strLogMsg.ReleaseBuffer();
	va_end(args);	

	pCalTestObj->TraceLog(lpszModule, lpszSrcFile, iSrcLine, msgLevel, strLogMsg);

	return;
}

void ToolFactoryLog(INT iProcessCode, LPCTSTR lpszBand, LPCTSTR lpszChannel, INT iErrorCode, LPCTSTR lpszUL, LPCTSTR lpszDL, LPCTSTR lpszMeasured, LPCTSTR lpszUnit, BOOL bPass, LPCTSTR lpszMsgFormat, ...)
{
	CCalibrationTest *pCalTestObj = ((CRFCalibrationToolApp*)AfxGetApp())->GetCalibrationTestObj();

	if (pCalTestObj == NULL)
		return;	

	va_list args;
	va_start(args, lpszMsgFormat);
	CString strLogMsg;
	_vstprintf(strLogMsg.GetBuffer(1000), lpszMsgFormat, args);
	strLogMsg.ReleaseBuffer();
	va_end(args);	

	pCalTestObj->FactoryLog(iProcessCode, lpszBand, lpszChannel, iErrorCode, lpszUL, lpszDL, lpszMeasured, lpszUnit, bPass, strLogMsg);

	return;
}

void ToolCommandLog(LPCTSTR lpszType, LPCTSTR lpszMessage)
{
	CCalibrationTest *pCalTestObj = ((CRFCalibrationToolApp*)AfxGetApp())->GetCalibrationTestObj();

	if (pCalTestObj == NULL)
		return;	

	pCalTestObj->CommandLog(lpszType, lpszMessage);

	return;
}

// CRFCalibrationToolApp

BEGIN_MESSAGE_MAP(CRFCalibrationToolApp, CWinApp)
END_MESSAGE_MAP()


// CRFCalibrationToolApp construction

CRFCalibrationToolApp::CRFCalibrationToolApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance	
	m_pRFWnd = NULL;
	m_pCalTestObj = NULL;
}


// The one and only CRFCalibrationToolApp object

CRFCalibrationToolApp theApp;

// CRFCalibrationToolApp initialization

BOOL CRFCalibrationToolApp::InitInstance()
{
	CWinApp::InitInstance();

	m_pRFWnd = new CRFCalibrationToolWnd;
	m_pMainWnd = m_pRFWnd;
	m_pRFWnd->ShowWindow(SW_HIDE);
//	((CRFCalibrationToolWnd*)m_pRFWnd)->CreateTimer();	

	return TRUE;
}

int CRFCalibrationToolApp::ExitInstance()
{
	//if (m_pRFWnd != NULL) {
	//	delete m_pRFWnd;
	//	m_pRFWnd = NULL;
	//}

	//if (m_pCalTestObj != NULL) {
	//	delete m_pCalTestObj;
	//	m_pCalTestObj = NULL;
	//}

	return CWinApp::ExitInstance();
}

BOOL CRFCalibrationToolApp::RFCalibration(TCHAR *lpszCalItems)
{		
	((CRFCalibrationToolWnd*)m_pMainWnd)->CreateTimer();

	DWORD dwStart = ::GetTickCount();

	m_pCalTestObj = new CCalibrationTest;
	BOOL bRes = m_pCalTestObj->RFCalibration(lpszCalItems);	

	((CRFCalibrationToolWnd*)m_pMainWnd)->StopTimer();

	DWORD dwEnd = ::GetTickCount();
	DWORD dwTime = (dwEnd - dwStart)/1000;
	CString strTime;
	std::string strOutput;
	strTime.Format(_T("Test Times: %d Mins, %d Secs"), dwTime/60, dwTime%60);
	OutputHeader((LPTSTR)(LPCTSTR)strTime, strOutput);

	delete m_pCalTestObj;
	m_pCalTestObj = NULL;

	return bRes;
}

BOOL CRFCalibrationToolApp::RFTest(TCHAR *lpszTestItems)
{
	((CRFCalibrationToolWnd*)m_pMainWnd)->CreateTimer();

	DWORD dwStart = ::GetTickCount();

	m_pCalTestObj = new CCalibrationTest;
	BOOL bRes = m_pCalTestObj->RFTest(lpszTestItems);

	((CRFCalibrationToolWnd*)m_pMainWnd)->StopTimer();

	DWORD dwEnd = ::GetTickCount();
	DWORD dwTime = (dwEnd - dwStart)/1000;
	CString strTime;
	std::string strOutput;
	strTime.Format(_T("Test Times: %d Mins, %d Secs"), dwTime/60, dwTime%60);	
	OutputHeader((LPTSTR)(LPCTSTR)strTime, (std::string)(""));

	delete m_pCalTestObj;
	m_pCalTestObj = NULL;

	return bRes;
}

BOOL CRFCalibrationToolApp::RFCompletePhaseAndCallTest(TCHAR *lpszCalTestItems)
{
	((CRFCalibrationToolWnd*)m_pMainWnd)->CreateTimer();

	DWORD dwStart = ::GetTickCount();

	m_pCalTestObj = new CCalibrationTest;
	BOOL bRes = m_pCalTestObj->RFCalAndTest(lpszCalTestItems);

	((CRFCalibrationToolWnd*)m_pMainWnd)->StopTimer();

	DWORD dwEnd = ::GetTickCount();
	DWORD dwTime = (dwEnd - dwStart)/1000;
	CString strTime;
	std::string strOutput;
	strTime.Format(_T("Test Times: %d Mins, %d Secs"), dwTime/60, dwTime%60);
	OutputHeader((LPTSTR)(LPCTSTR)strTime, strOutput);

	delete m_pCalTestObj;
	m_pCalTestObj = NULL;
	
	return bRes;
}

VOID CRFCalibrationToolApp::RFCalTestAbort()
{
	if (m_pCalTestObj == NULL)
		return;
	return m_pCalTestObj->RFAbort();
}