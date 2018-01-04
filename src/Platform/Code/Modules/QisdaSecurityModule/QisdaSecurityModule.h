// QisdaSecurityModule.h : main header file for the QisdaSecurityModule DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#ifdef QISDA_SECURITY_MODULE_EXPORTS
#define QSM_API __declspec(dllexport)
#else
#define QSM_API __declspec(dllimport)
#endif
// CQisdaSecurityModuleApp
// See QisdaSecurityModule.cpp for the implementation of this class
//


