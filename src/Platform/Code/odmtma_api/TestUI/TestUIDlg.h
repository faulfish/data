// TestUIDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include <afxmt.h>

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif

// CTestUIDlg dialog
class CTestUIDlg : public CDialog
{
// Construction
public:
	CTestUIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
public:
	CListCtrl m_ListLog;
	static int CallBackFn(const char* szInfo, int nStatus);
	static bool CallBackFn(const char* szInfo, int nStep, int nProgress);
	int LogFn(const char* szInfo, int nStatus);

protected:
	void TestDwonload();

	bool ChangeToFTM(int nPort);
	bool ChangeToDL(int nPort);
	void TestDL(int nPort,int nRepeatCount);
	void AddString(const CString& strMsg,bool bLog = true);
	void DisplayString(const CString& strMsg);
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedChangedl();
	afx_msg void OnBnClickedTestNoNetwork();
	CListBox m_strListLog;
	CString m_strCOMPort;
	CString m_strLogFile;
	CString m_strRepeat;
	CString m_strDisplay;

	CEvent m_ThreadStopEvent;
	afx_msg void OnBnClickedSafestop();
	CStatic m_DisplayCtrl;
	CButton m_ChangeDLCtrl;
	afx_msg void OnBnClickedDlreset();
	afx_msg void OnBnClickedChangeftm();
	afx_msg void OnBnClickedBackupnv();
	CEdit m_BTAddrCtrl;
	CEdit m_IMEICtrl;
	afx_msg void OnBnClickedWritebtaddr();
	afx_msg void OnBnClickedWriteimei();
};
