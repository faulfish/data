// QisdaEFsModule.h : main header file for the QisdaEFsModule DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CQisdaEFsModuleApp
// See QisdaEFsModule.cpp for the implementation of this class
//

class CQisdaEFsModuleApp : public CWinApp
{
public:
	CQisdaEFsModuleApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
