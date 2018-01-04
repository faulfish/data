// ClientDlg.h : header file
//

#pragma once


// CClientDlg dialog
class CClientDlg : public CDialog
{
// Construction
public:
	CClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PUBLISH_CLIENT_DIALOG };

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
	afx_msg void OnBnClickedSetupButton();
	afx_msg void OnCbnSelchangeProject();
	afx_msg void OnCbnSelendokProject();

private:
	void UpdateComboList(UINT nID,const CString& strPrefix,const CString& strPath);
public:
	afx_msg void OnCbnSelchangeStation();
	afx_msg void OnCbnSelchangeVersion();
};
