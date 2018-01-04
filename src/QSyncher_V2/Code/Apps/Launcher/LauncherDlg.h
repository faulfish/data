// LauncherDlg.h : header file
//
#include "../../CommonUtil/CommonUtility.h"
#include "../../Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"

#pragma once

// CLauncherDlg dialog
class CLauncherDlg : public QSYNC_LIB::Win32DoubleBufferDialogWidge
{
// Construction
public:
	CLauncherDlg();	// standard constructor

// Dialog Data
	enum { IDD = IDD_LAUNCHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON											m_hIcon;
	
	std::wstring										m_strBGImagePath;
	int													m_nWidth;
	int													m_nHeight;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	static void OnBnClicked1();
};
