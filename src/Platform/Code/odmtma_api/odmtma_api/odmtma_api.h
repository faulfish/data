// odmtma_api.h : main header file for the odmtma_api DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Codmtma_apiApp
// See odmtma_api.cpp for the implementation of this class
//

class Codmtma_apiApp : public CWinApp
{
public:
	Codmtma_apiApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
