// SPU_UI.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSPU_UIApp:
// See SPU_UI.cpp for the implementation of this class
//

class CSPU_UIApp : public CWinApp
{
public:
	CSPU_UIApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSPU_UIApp theApp;