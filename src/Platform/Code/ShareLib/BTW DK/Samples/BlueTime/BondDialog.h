#if !defined(AFX_BONDDIALOG_H__DBF3396B_8212_4DA7_84A4_B4168863F953__INCLUDED_)
#define AFX_BONDDIALOG_H__DBF3396B_8212_4DA7_84A4_B4168863F953__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BondDialog.h : header file
//
#include "BtIfDefinitions.h"
#include "BtIfClasses.h"


/////////////////////////////////////////////////////////////////////////////
// CBondDialog dialog

class CBondDialog : public CDialog
{
// Construction
public:
	CBondDialog(CWnd* pParent = NULL);   // standard constructor
    BD_ADDR m_BdAddr;
    CString m_Name;
	BOOL BondCb(CBtIf::eBOND_CB_EVENT event, UINT32 event_data);

// Dialog Data
	//{{AFX_DATA(CBondDialog)
	enum { IDD = IDD_BOND_DIALOG };
	CButton	m_unbond_button;
	CButton	m_bond_button;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBondDialog)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

    CBtIf m_BtIf;
	CString m_pin_code;



	// Generated message map functions
	//{{AFX_MSG(CBondDialog)
	afx_msg void OnBondExButton();
	afx_msg void OnUnbondButton();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSend();
public:
	afx_msg void OnBnClickedCancelSend();
public:
	afx_msg void OnBnClickedBond();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BONDDIALOG_H__DBF3396B_8212_4DA7_84A4_B4168863F953__INCLUDED_)
