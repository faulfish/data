// CodecTestToolDlg.h : header file
//

#pragma once


// CCodecTestToolDlg dialog
class CCodecTestToolDlg : public CDialog
{
// Construction
public:
	CCodecTestToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CODECTESTTOOL_DIALOG };

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

	void DefinfeCombo(UINT nID);
public:
	afx_msg void OnCbnSelchangeProcress1Combo();
	afx_msg void OnCbnSelchangeProcress2Combo();
	afx_msg void OnBnClickedTransBut();
};
