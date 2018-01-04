// GoodJobToolDlg.h : header file
//

#pragma once

#include "..\..\Diag_Export\QualcommDiag.h"


// CGoodJobToolDlg dialog
class CGoodJobToolDlg : public CDialog
{
// Construction
public:
	CGoodJobToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GOODJOBTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	static bool CallBackFn(const char* szInfo, int nStep, int nProgress);

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
	afx_msg void OnBnClickedSettestmode();
	afx_msg void OnBnClickedRestore();
	afx_msg void OnBnClickedBackup();
	afx_msg void OnBnClickedBtnRestart();
	afx_msg void OnBnClickedBtnDownload();
	afx_msg void OnBnClickedIsdlmode();
	afx_msg void OnBnClickedBtnBackupbrt();
	afx_msg void OnBnClickedBtnIsQfused();
};
