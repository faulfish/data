#pragma once

#include "CTabCtrlSSL/TabPageSSL.h"
#include "../MobileInterface/Interface_Include/ICommMobile.h"
#include "afxwin.h"

// CIMessageTestPage dialog

class CIMessageTestPage : public CTabPageSSL
{
	DECLARE_DYNAMIC(CIMessageTestPage)

public:
	CIMessageTestPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIMessageTestPage();

// Dialog Data
	enum { IDD = IDD_IMessage_PAGE };

private:
    ISMS* m_pISMS;

public:
	int m_nISMS_Function;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedNewInterface();
public:
	afx_msg void OnBnClickedDeleteInterface();
public:
	afx_msg void OnBnClickedExecFunction();
public:
	CString m_Param1;
public:
	CString m_param2;
public:
	CString m_param3;
public:
	CString m_param4;
public:
	CListBox m_result_list;
public:
	CEdit m_result_edit;
};
