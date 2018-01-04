#pragma once

#include "CTabCtrlSSL/TabPageSSL.h"

// CIIrMCTestPage dialog

class CIIrMCTestPage : public CTabPageSSL
{
	DECLARE_DYNAMIC(CIIrMCTestPage)

public:
	CIIrMCTestPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIIrMCTestPage();

// Dialog Data
	enum { IDD = IDD_IIrMC_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedNewInterface();
};
