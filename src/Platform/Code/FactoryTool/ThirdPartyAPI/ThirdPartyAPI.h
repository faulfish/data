// ThirdPartyAPI.h : main header file for the ThirdPartyAPI DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CThirdPartyAPIApp
// See ThirdPartyAPI.cpp for the implementation of this class
//

class CThirdPartyAPIApp : public CWinApp
{
public:
	CThirdPartyAPIApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
