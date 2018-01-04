#if !defined(AFX_ESCO_H__FA7BCF36_60D6_455F_88FA_14A4A500DBEA__INCLUDED_)
#define AFX_ESCO_H__FA7BCF36_60D6_455F_88FA_14A4A500DBEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ESco.h : header file
//
#include "btwlib.h"

/////////////////////////////////////////////////////////////////////////////
// CEScoDlg dialog

class CEScoDlg : public CDialog
{
// Construction
public:
	CEScoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEScoDlg)
	enum { IDD = IDD_ESCOSETTING };
    	//}}AFX_DATA
    tBTM_ESCO_PARAMS   *m_pEScoParams;
    BOOL  m_bChangeEsco;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEScoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEScoDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ESCO_H__FA7BCF36_60D6_455F_88FA_14A4A500DBEA__INCLUDED_)
