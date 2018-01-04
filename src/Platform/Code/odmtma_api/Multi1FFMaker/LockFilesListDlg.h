#pragma once

#include "resource.h"
#include "../../CommonUtil/PIMStringUtility.h"

using namespace std;

// CLockFilesListDlg dialog

class CLockFilesListDlg : public CDialog
{
	DECLARE_DYNAMIC(CLockFilesListDlg)

public:
	CLockFilesListDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLockFilesListDlg();
	int GetSelectedRow() const;

	void SetListCtrl(const vector<CString>& vecLockFilesList);
// Dialog Data
	enum { IDD = IDD_LockFileListDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

private:
	CListCtrl  m_LockFilesListCtrl;   
	vector<CString> m_vecLockFilesList;
	int m_nSelectedRow;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMDblclkLockfileslist(NMHDR *pNMHDR, LRESULT *pResult);
};
