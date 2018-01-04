// NVItemUI.h : main header file for the NVItemUI application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CNVItemUIApp:
// See NVItemUI.cpp for the implementation of this class
//

class CNVItemUIApp : public CWinApp
{
public:
	CNVItemUIApp();
	~CNVItemUIApp();

protected:
	bool  OpenTemplate(const CString szWndTitle);
public:
	bool        m_bIsLoadFileOpen;
	bool		m_bIsNVItemViewOpen;
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnOpenNvfile();
	afx_msg void OnUpdateOpenNvfile(CCmdUI *pCmdUI);
	afx_msg void OnViewNvitemview();
	afx_msg void OnUpdateViewNvitemview(CCmdUI *pCmdUI);
};

extern CNVItemUIApp theApp;