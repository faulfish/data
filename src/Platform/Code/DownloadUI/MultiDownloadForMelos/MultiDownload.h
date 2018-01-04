// MultiDownload.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMultiDownloadApp:
// See MultiDownload.cpp for the implementation of this class
//
namespace QSYNC_LIB
{

	class CMultiDownloadApp : public CWinApp
	{
	public:
		CMultiDownloadApp();

	// Overrides
		public:
		virtual BOOL InitInstance();

	// Implementation

		DECLARE_MESSAGE_MAP()
	};

	extern CMultiDownloadApp theApp;
}