// ReportCenter.h : main header file for the ReportCenter DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CReportCenterApp
// See ReportCenter.cpp for the implementation of this class
//

class CReportCenterApp : public CWinApp
{
public:
	CReportCenterApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
