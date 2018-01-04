#pragma once

#include "CTabCtrlSSL/TabPageSSL.h"
#include "../MobileInterface/Interface_Include/ICommMobile.h"

// CISyncMLTestPage dialog

class CISyncMLTestPage : public CTabPageSSL
{
	DECLARE_DYNAMIC(CISyncMLTestPage)

public:
	CISyncMLTestPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CISyncMLTestPage();

// Dialog Data
	enum { IDD = IDD_ISyncML_PAGE };

private:
	ISYNCML* m_pISyncML;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
    int m_nISyncML_Function; 

public:
	afx_msg void OnBnClickedNewIsyncml();
public:
	afx_msg void OnBnClickedDeleteInterface();
public:
	afx_msg void OnBnClickedExecFunction();
};
