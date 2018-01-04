// Log2XMLDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CLog2XMLDlg dialog
class CLog2XMLDlg : public CDialog
{
// Construction
public:
	CLog2XMLDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LOG2XML_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CString m_strOutput;
	CString m_strLog;
	afx_msg void OnBnClickedButtontransfer();
	afx_msg void OnBnClickedClear();
};
