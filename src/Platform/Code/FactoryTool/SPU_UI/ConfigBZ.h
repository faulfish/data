#pragma once
#include "afxwin.h"

#ifdef _WIN32_WCE
#error "CDHtmlDialog is not supported for Windows CE."
#endif 

// CConfigBZ dialog

class CConfigBZ : public CDialog
{
	DECLARE_DYNCREATE(CConfigBZ)

public:
	CConfigBZ(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigBZ();

	void LoadConfigFile();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CButton m_LogData;
	CButton m_Check_Status;
	CButton m_ProfileLoader;
	CEdit m_site;
	CEdit m_FactoryId;
	CEdit m_BenchId;
	afx_msg void OnBnClickedCheckLogData();
	CButton m_PrintLabel;
	afx_msg void OnEnChangeEditPassword();
	CEdit m_Password;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOk1();
};
