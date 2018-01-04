// QisdaSimLockModule.h : main header file for the QisdaSimLockModule DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CQisdaSimLockModuleApp
// See QisdaSimLockModule.cpp for the implementation of this class
//

class CQisdaSimLockModuleApp : public CWinApp
{
public:
	CQisdaSimLockModuleApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
