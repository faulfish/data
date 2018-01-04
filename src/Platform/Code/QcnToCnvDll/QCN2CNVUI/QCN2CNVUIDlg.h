// QCN2CNVUIDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CQCN2CNVUIDlg dialog
class CQCN2CNVUIDlg : public CDialog
{
// Construction
public:
	CQCN2CNVUIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_QCN2CNVUI_DIALOG };

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
	afx_msg void OnBnClickedGetqcn();
	CEdit m_QCNCtrl;
	CEdit m_CNVCtrl;
	afx_msg void OnBnClickedGetdu();
	afx_msg void OnBnClickedConvertButton();
	CButton m_DUCtrl;
	CEdit m_DUEditCtrl;
	afx_msg void OnBnClickedGetcnv();
	CEdit m_SRCCNVCtrl;
	afx_msg void OnBnClickedCnv2qcn();
	CEdit m_NEWQCNCtrl;
	afx_msg void OnBnClickedConvertToBrtButton();
};
