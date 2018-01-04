#pragma once

#include "resource.h"
#include <map>
#include "../../CommonUtil/PIMStringUtility.h"
#include "LockFilesListDlg.h"

using namespace std;
// CSetLockFileDlg dialog

class CSetLockFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetLockFileDlg)

public:
	CSetLockFileDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetLockFileDlg();
	INT_PTR DoModal(const CString& strVariantPath, const CString& strLockFilePath, const map<TSTRING, TSTRING>& mapVariantExtension, const map<TSTRING, TSTRING>& mapLockExtension);
	const map<TSTRING, TSTRING>& GetVariantToLockFileMap() const {return m_mapVariantToLockFile;}
	const vector<CString>& GetVariantFiles(const TSTRING& strVariantPath, const map<TSTRING, TSTRING>& mapVariantExtension);

// Dialog Data
	enum { IDD = IDD_SetLockFileDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	bool GetAllVariantFiles();
	bool GetAllLockFiles();
	//bool SaveVariantToLockIniFile();

private:
	CString m_strVariantPath;
	map<TSTRING, TSTRING> m_mapVariantExtension;
	vector<CString> m_vecVariantFileName; //沒有副檔名

	CString m_strLockFilePath;
	map<TSTRING, TSTRING> m_mapLockExtension;
	vector<CString> m_vecLockFileName; //沒有副檔名

private:
	CButton m_BtnOK;
	CListCtrl  m_VariantToLockListCtrl;   
	map<TSTRING, TSTRING> m_mapVariantToLockFile;
	//CString m_strVariantToLockIniFilePath;

	CLockFilesListDlg m_LockFilesListDlg;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMDblclkVarianttolocklist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
};
