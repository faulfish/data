// CDAutoRun.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCDAutoRunApp:
// See CDAutoRun.cpp for the implementation of this class
//
namespace QSYNC_LIB
{
	class CImagerAnalysisToolApp : public CWinApp
	{
	public:
		CImagerAnalysisToolApp();

		// Overrides
	public:
		virtual BOOL InitInstance();

		// Implementation

		DECLARE_MESSAGE_MAP()
	};

	extern CImagerAnalysisToolApp theApp;
}