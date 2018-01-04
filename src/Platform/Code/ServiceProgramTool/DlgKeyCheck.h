#pragma once
#include "afxwin.h"
#include "ServiceProgramTool.h"

// CDlgKeyCheck dialog

class CDlgKeyCheck : public CDialog
{
	DECLARE_DYNAMIC(CDlgKeyCheck)

public:
	CDlgKeyCheck(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgKeyCheck();

// Dialog Data
	enum { IDD = IDD_DLG_WIBUKEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strUserCode;
	CString m_strFirmCode;
	afx_msg void OnBnClickedOk();
};
