// BackupRestoreBRT.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CBackupRestoreBRTApp:
// See BackupRestoreBRT.cpp for the implementation of this class
//

class CBackupRestoreBRTApp : public CWinApp
{
public:
	CBackupRestoreBRTApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CBackupRestoreBRTApp theApp;