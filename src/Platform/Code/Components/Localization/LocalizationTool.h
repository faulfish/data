// LocalizationTool.h : main header file for the LocalizationTool DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CLocalizationToolApp
// See LocalizationTool.cpp for the implementation of this class
//

class CLocalizationToolApp : public CWinApp
{
public:
	CLocalizationToolApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

