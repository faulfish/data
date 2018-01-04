#if !defined(AFX_PASSWORD_H__17236C52_7ABB_441A_9FF8_C91FF37AB804__INCLUDED_)
#define AFX_PASSWORD_H__17236C52_7ABB_441A_9FF8_C91FF37AB804__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PassWord.h : header file
//
#include "Label.h"

/////////////////////////////////////////////////////////////////////////////
// CPassWord dialog

class CPassWord : public CDialog
{
// Construction
public:
	CPassWord(CWnd* pParent = NULL);   // standard constructor
	CBrush m_brush;

// Dialog Data
	//{{AFX_DATA(CPassWord)
	enum { IDD = IDD_DIALOG_PASSWORD };
	CLabel	m_PASSWORD;
	CEdit	m_PASSWORD_EDIT;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPassWord)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPassWord)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSWORD_H__17236C52_7ABB_441A_9FF8_C91FF37AB804__INCLUDED_)
