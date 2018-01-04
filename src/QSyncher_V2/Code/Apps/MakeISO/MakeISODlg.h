// MakeISODlg.h : header file
//
#include "..\..\Components\ConfigureTool\IConfigure.h"

#pragma once


// CMakeISODlg dialog
class CMakeISODlg : public CDialog
{
// Construction
public:
	CMakeISODlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MAKEISO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON														m_hIcon;
	QSYNC_LIB::IConfigure*							m_IConfigure;
	QSYNC_LIB::CCfgStr								m_strSource;
	QSYNC_LIB::CCfgStr								m_strTarget;
	QSYNC_LIB::CCfgStr								m_strCodeName;
	QSYNC_LIB::CCfgStr								m_strProjectName;
	QSYNC_LIB::CCfgStr								m_strProjectVersion;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSamefolderCheck();
private:
	bool IsCheck(UINT ID);
	void SetCheck(UINT ID,bool bCheck);
	void SetString(UINT ID,const CString& strBuffer);
	CString GetString(UINT ID);
	void MakeSample();
	CString GetNewFormat();
	CString GetTempISOFileName();
	void DeleteTempISOFirst();
	bool IsISOFileExist();
	void WinExec(LPCTSTR lpApplicationName,LPTSTR lpCommandLine);
	void SystemCommand(LPCTSTR lpApplicationName,LPTSTR lpCommandLine);
public:
	afx_msg void OnBnClickedFolderButton(UINT ID);
	afx_msg void OnBnClickedExecButton();
	afx_msg void OnBnClickedRenameButton();
	afx_msg void OnEnChangeSourefolderEdit();
	afx_msg void OnDestroy();
	afx_msg void OnEnChangeTargetfolderEdit();
	afx_msg void OnEnChangeCodenameEdit();
	afx_msg void OnEnChangeProjectnameEdit();
	afx_msg void OnEnChangeProjectversionEdit();
};
