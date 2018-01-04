// PIMDB.h : main header file for the PIMDB DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPIMDBApp
// See PIMDB.cpp for the implementation of this class
//

class CPIMDBApp : public CWinApp
{
public:
	CPIMDBApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
