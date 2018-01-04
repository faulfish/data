// QisdaVideoModule.h : main header file for the QisdaVideoModule DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CQisdaVideoModuleApp
// See QisdaVideoModule.cpp for the implementation of this class
//

class CQisdaVideoModuleApp : public CWinApp
{
public:
	CQisdaVideoModuleApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
