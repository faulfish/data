// QConfigure.h : main header file for the QConfigure DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CQConfigureApp
// See QConfigure.cpp for the implementation of this class
//



class CQConfigureApp : public CWinApp
{
public:
	CQConfigureApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

};
