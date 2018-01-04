#if !defined(AFX_SO_H__320B4C9B_C721_4A10_84D1_D68DEF58770C__INCLUDED_)
#define AFX_SO_H__320B4C9B_C721_4A10_84D1_D68DEF58770C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SO.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSO dialog

class CSO : public CDialog
{
// Construction
public:
	CSO(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSO)
	enum { IDD = IDD_SO };
	CEdit	m_so;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSO)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSO)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SO_H__320B4C9B_C721_4A10_84D1_D68DEF58770C__INCLUDED_)
