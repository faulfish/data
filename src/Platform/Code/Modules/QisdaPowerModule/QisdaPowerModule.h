// QisdaPowerModule.h : main header file for the QisdaPowerModule DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#ifdef QISDA_POWER_MODULE_EXPORTS
#define QPM_API __declspec(dllexport)
#else
#define QPM_API __declspec(dllimport)
#endif
// CQisdaPowerModuleApp
// See QisdaPowerModule.cpp for the implementation of this class
//
