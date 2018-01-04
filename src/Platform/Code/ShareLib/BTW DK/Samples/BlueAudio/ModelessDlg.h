#if !defined(AFX_MODELESSDLG_H__F8E8FFA2_E130_4B40_A25C_C417C925298C__INCLUDED_)
#define AFX_MODELESSDLG_H__F8E8FFA2_E130_4B40_A25C_C417C925298C__INCLUDED_
//  Copyright (c) 2000-2004, Broadcom Corporation, All Rights Reserved.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelessDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModelessDlg dialog

class CModelessDlg : public CDialog
{
// Construction
public:
	CModelessDlg(UINT id,CWnd* pParent = NULL);   // standard constructor
    BOOL Create();

// Dialog Data
	//{{AFX_DATA(CModelessDlg)
//	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelessDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModelessDlg)
	afx_msg void OnClose();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    int m_id;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELESSDLG_H__F8E8FFA2_E130_4B40_A25C_C417C925298C__INCLUDED_)
