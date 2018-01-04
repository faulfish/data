// RFCalibrationTool.h : main header file for the RFCalibrationTool DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "EnumFiles.h"

void ToolTraceLog(LPCTSTR lpszModule, LPCSTR lpszSrcFile, UINT iSrcLine, EMsgLevel msgLevel, LPCTSTR lpszMsgFormat, ...);
void ToolFactoryLog(INT iProcessCode, LPCTSTR lpszBand, LPCTSTR lpszChannel,INT iErrorCode, LPCTSTR lpszUL, LPCTSTR lpszDL, LPCTSTR lpszMeasured, LPCTSTR lpszUnit, BOOL bPass, LPCTSTR lpszMsgFormat, ...);
void ToolCommandLog(LPCTSTR lpszType, LPCTSTR lpszMessage);

class CCalibrationTest;

class CRFCalibrationToolApp : public CWinApp
{
// Operations
public:
	CWnd* GetRFWnd() {return m_pRFWnd;}
	BOOL RFCalibration(TCHAR *lpszCalItems);
	BOOL RFTest(TCHAR *lpszTestItems);
	BOOL RFCompletePhaseAndCallTest(TCHAR *lpszCalTestItems);
	VOID RFCalTestAbort();

	CCalibrationTest* GetCalibrationTestObj() {return m_pCalTestObj;}

// Attributes
private:
	HINSTANCE m_hResource;
	CCalibrationTest *m_pCalTestObj;
	CWnd *m_pRFWnd;

public:
	CRFCalibrationToolApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
