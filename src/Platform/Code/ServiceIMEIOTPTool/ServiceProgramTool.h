// ServiceProgramTool.h : main header file for the ServiceProgramTool DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// CServiceProgramToolApp
// See ServiceProgramTool.cpp for the implementation of this class
//

class CLogProxy;
CLogProxy* GetLogInst();

#define LOG (GetLogInst()->LogMessage)

class CServiceProgramToolApp: public CWinApp
{
public:
	HINSTANCE m_hResource;
	CServiceProgramToolApp(LPCTSTR lpszAppName = NULL);

	void ShowServiceProgramTool();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	DECLARE_MESSAGE_MAP()
};

class CLogProxy
{
public:
	void LogMessage(const TCHAR* pID, const TCHAR* pMessage);

private:
	FILE *m_pLogFile;
	CString m_strLogFileName;

	void InitLogInst();
	void LogID(const TCHAR* pID);	

public:	
	CLogProxy(LPCTSTR pLogFileName);
	~CLogProxy();
};
