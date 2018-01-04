// odmtpa_qisda_Lavernock.h : main header file for the odmtpa_qisda_Lavernock DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Codmtpa_qisda_LavernockApp
// See odmtpa_qisda_Lavernock.cpp for the implementation of this class
//

class Codmtpa_qisda_LavernockApp : public CWinApp
{
public:
	Codmtpa_qisda_LavernockApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
