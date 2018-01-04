// MobileFinder.h : main header file for the MobileFinder DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMobileFinderApp
// See MobileFinder.cpp for the implementation of this class
//

class CMobileFinderApp : public CWinApp
{
public:
	CMobileFinderApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance(); // return app exit code

	DECLARE_MESSAGE_MAP()
};
