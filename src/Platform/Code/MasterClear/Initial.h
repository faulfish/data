#if !defined(AFX_INITIAL_H__DCFD3F11_EC05_4506_AD80_48A88916AB4B__INCLUDED_)
#define AFX_INITIAL_H__DCFD3F11_EC05_4506_AD80_48A88916AB4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Initial.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInitial dialog

class CInitial : public CDialog
{
// Construction
public:
	CInitial(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInitial)
	enum { IDD = IDD_DIALOG1 };
	CComboBox	m_combosw;
	CComboBox	m_combomode;
	CComboBox	m_combohw;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInitial)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInitial)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INITIAL_H__DCFD3F11_EC05_4506_AD80_48A88916AB4B__INCLUDED_)
