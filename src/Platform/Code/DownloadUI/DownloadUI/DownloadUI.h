// DownloadUI.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "LimitSingleInstance.h"


// CDownloadUIApp:
// See DownloadUI.cpp for the implementation of this class
//
namespace QSYNC_LIB
{
	class CDownloadUIApp : public CWinApp
	{
	public:
		CDownloadUIApp();

	// Overrides
		public:
		virtual BOOL InitInstance();
		void CreateQSyncerLimitSingleInstance();

	// Implementation
	private:
		CLimitSingleInstance* m_pSingleInstanceObj;//GUID of QSyncer

		DECLARE_MESSAGE_MAP()
	};

	extern CDownloadUIApp theApp;
}