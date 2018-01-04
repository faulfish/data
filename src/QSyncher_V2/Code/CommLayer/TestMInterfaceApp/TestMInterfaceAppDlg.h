// TestMInterfaceAppDlg.h : header file
//

#ifndef _BENQ_MOBILEPHONE_INTERFACE_TEST_DLG_H_
#define _BENQ_MOBILEPHONE_INTERFACE_TEST_DLG_H_

#pragma once
#include "afxcmn.h"
#include "CTabCtrlSSL/TabCtrlSSL.h"
#include "CommInterfaceTestPage.h"
#include "IEFSTestPage.h"
#include "IMessageTestPage.h"
#include "IPhonebookTestPage.h"
#include "ISyncMLTestPage.h"
#include "IIrMCTestPage.h"
#include "afxwin.h"
#include "../MobileFinder/MobileFinderInterface.h"

// CTestMInterfaceAppDlg dialog
class CTestMInterfaceAppDlg : public CDialog , public QSYNC_LIB::INotify
{
	virtual void Event(const TSTRING& strEvent,long nParam);
// Construction
public:
	CTestMInterfaceAppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTMINTERFACEAPP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;
	CCommInterfaceTestPage  m_CommPage;
	CIEFSTestPage			m_IEFSPage;
	CIMessageTestPage		m_IMessagePage;
	CIPhonebookTestPage		m_IPhonebookPage;
	CISyncMLTestPage		m_ISyncMLPage;
	CIIrMCTestPage			m_IIrMCPage;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedConnectbutton();
public:
	afx_msg void OnBnClickedDisconnectButton();

public:
	CTabCtrlSSL m_ITab;
public:
	CString m_strPort;
public:
	CString m_strDevName;
public:
	CStatic m_strConnState;
public:
	CString m_strModelName;
public:
	CString m_strIMEI;
};

#endif