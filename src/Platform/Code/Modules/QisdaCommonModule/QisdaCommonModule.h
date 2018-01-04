// QisdaCommonModule.h : main header file for the QisdaCommonModule DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#ifdef QISDA_COMMON_MODULE_EXPORTS
#define QCM_API __declspec(dllexport)
#else
#define QCM_API __declspec(dllimport)
#endif

// CQisdaCommonModuleApp
// See QisdaCommonModule.cpp for the implementation of this class
//

