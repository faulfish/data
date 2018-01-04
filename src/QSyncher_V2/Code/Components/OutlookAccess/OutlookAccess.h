// OutlookAccess.h : main header file for the OutlookAccess DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// COutlookAccessApp
// See OutlookAccess.cpp for the implementation of this class
//

class COutlookAccessApp : public CWinApp
{
public:
	COutlookAccessApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
