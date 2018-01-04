#if !defined(AFX_INITIAL_H__C4E39D82_1D41_4798_B1E2_38310D0EBBED__INCLUDED_)
#define AFX_INITIAL_H__C4E39D82_1D41_4798_B1E2_38310D0EBBED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// INITIAL.h : header file
//
#include "Label.h"
#include "SxGroupBox.h"
#include "ColoredCombo.h"

/////////////////////////////////////////////////////////////////////////////
// CINITIAL dialog

class CINITIAL : public CDialog
{
// Construction
public:
	void Set_Ctrl_Property(void);
	CINITIAL(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CINITIAL)
	enum { IDD = IDD_DIALOG_INITIAL };
	CLabel	m_SO;
	CColoredEdit	m_SO_EDIT;
	CColoredCombo	m_DUCK;
	CLabel	m_QZCS_STATIC;
	CLabel	m_QVCEFS_STATIC;
	CLabel	m_QSNV_STATIC;
	CSxGroupBox	m_PPF_FILE;
	CLabel	m_DUCK_STATIC;
	CColoredCombo	m_QZCS;
	CColoredCombo	m_QVCEFS;
	CColoredCombo	m_QSWINFO;
	CColoredCombo	m_QSNV;
	CColoredCombo	m_CSV;
	CSxGroupBox	m_SECURITY_MECHANISM_FLAG;
	CButton	m_SECURITY_MECHANISM_FLAG_CHECK;
	CSxGroupBox	m_MAL_CHECK2;
	CButton	m_MAL_CHECK;
	CColoredCombo	m_TARGET_TYPE;
	CLabel	m_TARGET_TYPE_STATIC;
	CSxGroupBox	m_SW_HW_ME_PN;
	CLabel	m_SW_STATIC;
	CSxGroupBox	m_QSWINFO_SIMDATA_CSV;
	CLabel	m_QSWINFO_STATIC;
	CLabel	m_PN_STATIC;
	CLabel	m_MODEL_NAME_STATIC;
	CSxGroupBox	m_MODEL_MODE;
	CLabel	m_MODE_NAME_STATIC;
	CLabel	m_ME_STATIC;
	CLabel	m_HW_STATIC;
	CLabel	m_CSV_STATIC;
	CSxGroupBox	m_BRT_QCN_CHECK;
	CColoredCombo	m_SW;
	CColoredCombo	m_PN;
	CColoredCombo	m_MODEL_NAME;
	CColoredCombo	m_MODE_NAME;
	CColoredCombo	m_ME;
	CColoredCombo	m_HW;
	CButton	m_QCN_CHECK;
	CButton	m_BRT_CHECK;
	//}}AFX_DATA
	CBrush m_brush;
	CFont   font_BRT_QCN;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CINITIAL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CINITIAL)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckBrt();
	afx_msg void OnCheckQcn();
	afx_msg void OnDropdownComboPn();
	afx_msg void OnButtonExit();
	afx_msg void OnButtonOk();
	afx_msg void OnSelchangeComboTargetType();
	afx_msg void OnCheckSecurityMechanismFlag();
	afx_msg void OnSelchangeComboSw();
	afx_msg void OnDropdownComboQsnv();
	afx_msg void OnDropdownComboQswinfo();
	afx_msg void OnDropdownComboQvcefs();
	afx_msg void OnDropdownComboQzcs();
	afx_msg void OnDropdownComboCsv();
	afx_msg void OnDropdownComboDuck();
	afx_msg void OnSelchangeComboModelName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INITIAL_H__C4E39D82_1D41_4798_B1E2_38310D0EBBED__INCLUDED_)
