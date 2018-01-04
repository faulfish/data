// QisdaNVModule.h : main header file for the QisdaNVModule DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#ifdef QISDA_NV_MODULE_EXPORTS
#define QNVM_API __declspec(dllexport)
#else
#define QNVM_API __declspec(dllimport)
#endif
// CQisdaNVModuleApp
// See QisdaNVModule.cpp for the implementation of this class
//

