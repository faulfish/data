// BadBlockCheckerDlg.h : header file
//

#pragma once


// CBadBlockCheckerDlg dialog
class CBadBlockCheckerDlg : public CDialog
{
// Construction
public:
	CBadBlockCheckerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_BADBLOCKCHECKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CEdit m_HexFile;
	CEdit m_ComPort;
public:
	int GetComPort();
	int GetCheckBadBlockTimeout();
	int GetOption();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnFile();
	afx_msg void OnBnClickedBtnDl();
	afx_msg void OnBnClickedBtnDl6240();
};
