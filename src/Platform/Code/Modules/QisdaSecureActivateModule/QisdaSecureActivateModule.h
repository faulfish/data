// QisdaSecureActivateModule.h : main header file for the QisdaSecureActivateModule DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CQisdaSecureActivateModuleApp
// See QisdaSecureActivateModule.cpp for the implementation of this class
//

class CQisdaSecureActivateModuleApp : public CWinApp
{
public:
	CQisdaSecureActivateModuleApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
