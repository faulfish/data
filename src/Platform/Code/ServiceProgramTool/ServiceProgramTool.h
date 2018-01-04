// ServiceProgramTool.h : main header file for the ServiceProgramTool DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// CServiceProgramToolApp
// See ServiceProgramTool.cpp for the implementation of this class
//
typedef enum
{
	MOBILEMODEL_WELLFLEET,
	MOBILEMODEL_LAVERNOCK,
	MOBILEMODEL_MELOS,
	MOBILEMODEL_LAST
}MOBILEMODEL;

static CONST TCHAR *MOBILEMARKETNAME[] = {
	_T("VE538"),			// Wellfleet
	_T("W7"),				// Lavernock
	_T("Melos"),				// Melos
	_T("Error")
};

class CServiceProgramToolApp: public CWinApp
{
public:
	HINSTANCE m_hResource;
	CServiceProgramToolApp();

	void ShowServiceProgramTool();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	DECLARE_MESSAGE_MAP()
};
