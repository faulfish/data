// QCH_DatabastAccess.h : main header file for the QCH_DatabastAccess DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CQCH_DatabastAccessApp
// See QCH_DatabastAccess.cpp for the implementation of this class
//

class CQCH_DatabastAccessApp : public CWinApp
{
public:
	CQCH_DatabastAccessApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
