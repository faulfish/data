// BZAPI.h : main header file for the BZAPI DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CBZAPIApp
// See BZAPI.cpp for the implementation of this class
//

class CBZAPIApp : public CWinApp
{
public:
	CBZAPIApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
