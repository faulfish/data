// 1FFMakerDlg.h : header file
//

#pragma once

#include <string>
#include <map>
#include "1FFNameMaker.h"
#include "SetLockFileDlg.h"

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

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSWLoadPath();
	afx_msg void OnBnClickedVariantPath();
	afx_msg void OnBnClickedLockFilesPath();
	afx_msg void OnBnClickedOutputFolder();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedSetLockFile();
private:
	CEdit m_Cust_ModelName;

	CEdit m_SWLoadPath;
	CButton m_BtnSWLoadPath;
	CEdit m_VariantPath;
	CButton m_BtnVariantPath;
	CEdit m_LockFilesPath;
	CButton m_BtnLockFilesPath;
	CEdit m_OutputFolder;
	CButton m_BtnOutputFolder;

	CStatic m_StatusText;

	CButton m_BtnMake;
	CButton m_BtnClose;
	//CButton m_BtnZipExe;
	CMulti1FFNameMaker m_NameMaker;
private:
	bool GetFolder(TSTRING& strFolderPath, const TCHAR* szCaption = NULL, HWND hOwner = NULL);
	//bool GetFilePath(const TSTRING& strFilePath);
	bool FindFiles(const TSTRING& strFolderPath);
	bool IsDots(const TCHAR* str);
	void GetINISetting(const CString& strINIFileName,const TSTRING& strSection, map<TSTRING, TSTRING>* pMapSectionContent);
	void GetINISetting(const CString& strINIFileName,const TSTRING& strSection, map<TSTRING, int>* pMapSectionContent);
	bool CopySWLoad(const CString& strTargetFolderPath);
	void UpdateStatusText(const CString& strStatusText);

	bool ClearFolder(const TSTRING& strFolderPath);
	bool CreateOneMotFile(const TSTRING& strFolderPath,const TSTRING& strVariantName, const TSTRING& strLockFileName);
	void SaveINISetting();
	void SavePathMapByEditBox();
	bool IsVariantNameLegal(const CString& strVariantFileName); //檢查檔頭與副檔名

	void InitialLog();
	void WriteLog(const CString& strLog);

public:
	//afx_msg void OnBnClickedBtnzippath();


private:
	CString m_strProjectName_ModelName;
	CString m_strIniFilePath;
	TSTRING m_strtZipExeFullPath;

	map<TSTRING, TSTRING> m_mapPath;

	map<TSTRING, TSTRING> m_mapSWHeader;
	map<TSTRING, TSTRING> m_mapSWExtension;

	map<TSTRING, TSTRING> m_mapVariantExtension;
	map<TSTRING, TSTRING> m_mapLockExtension;

	map<TSTRING, TSTRING> m_mapVariantToLockFile;

	map<TSTRING, int> m_mapCheckNameExtensionNumber;

	CSetLockFileDlg m_SetLockFileDlg;
};
