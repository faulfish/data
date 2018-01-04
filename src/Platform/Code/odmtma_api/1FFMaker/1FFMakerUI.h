// 1FFMakerUI.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMy1FFMakerUIApp:
// See 1FFMakerUI.cpp for the implementation of this class
//

class CMy1FFMakerUIApp : public CWinApp
{
public:
	CMy1FFMakerUIApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMy1FFMakerUIApp theApp;