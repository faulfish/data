// odmtpa_qisda_Melos.h : main header file for the odmtpa_qisda_Melos DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Codmtpa_qisda_MelosApp
// See odmtpa_qisda_Melos.cpp for the implementation of this class
//

class Codmtpa_qisda_MelosApp : public CWinApp
{
public:
	Codmtpa_qisda_MelosApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
