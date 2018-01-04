// CDCInterface.h : main header file for the CDCInterface DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCDCInterfaceApp
// See CDCInterface.cpp for the implementation of this class
//

#ifdef _CDCINTERFACEDLL
	#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllexport)
#else
	#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllimport)
#endif

class CDCINTERFACE_CLASS_DLLEXT CCDCInterfaceApp
{
public:
	CCDCInterfaceApp();
	~CCDCInterfaceApp();
};