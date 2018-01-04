// 1FFMakerDlg.h : header file
//

#pragma once

#include <string>
#include "C1FFMaker.h"

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif

// CMy1FFMakerDlg dialog
class CMy1FFMakerDlg : public CDialog
{
// Construction
public:
	CMy1FFMakerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MY1FFMAKERUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


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
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedClose();

private:
	CEdit m_Cust_ModelPath;
	CEdit m_FolderPath;
	CEdit m_ZipPath;
	CButton m_BtnSelectFolder;
	CButton m_BtnMake;
	CButton m_BtnClose;
	CButton m_BtnZipExe;
	C1FFNameMaker m_NameMaker;
private:
	bool GetFolder(TSTRING& strFolderPath, const TCHAR* szCaption = NULL, HWND hOwner = NULL);
	bool GetFilePath(const TSTRING& strFilePath);
	bool FindFiles(const TSTRING& strFolderPath);
	bool IsDots(const TCHAR* str);
	
public:
	afx_msg void OnBnClickedBtnzippath();
};
