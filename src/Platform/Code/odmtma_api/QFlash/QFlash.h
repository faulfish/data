// QFlash.h : main header file for the QFlash DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CQFlashApp
// See QFlash.cpp for the implementation of this class
//

class CQFlashApp : public CWinApp
{
public:
	CQFlashApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
