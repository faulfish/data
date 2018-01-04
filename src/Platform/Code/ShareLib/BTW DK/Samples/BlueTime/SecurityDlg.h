#if !defined(AFX_SECURITYDLG_H__F068ABFE_0701_47E9_97F4_FB15604DBC1B__INCLUDED_)
#define AFX_SECURITYDLG_H__F068ABFE_0701_47E9_97F4_FB15604DBC1B__INCLUDED_
//  Copyright (c) 2000-2004, Broadcom Corporation, All Rights Reserved.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SecurityDlg.h : header file
//
#include <AFXTEMPL.H>
#include "BtIfDefinitions.h"
#include "BtIfClasses.h"


/////////////////////////////////////////////////////////////////////////////
// CSecurityDlg dialog

class CSecurityDlg : public CDialog
{

// Construction
public:
	CSecurityDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSecurityDlg)
	enum { IDD = IDD_SECURITY_DIALOG };
	CListBox	m_security_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSecurityDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
class CSecurityOption
{
public:
    CSecurityOption()
    {
        m_display_name = "";
        m_flag = 0;
        m_selected = FALSE;
    }

    CSecurityOption(CString display_name, UINT8 flag, BOOL selected = FALSE)
    {
        m_display_name = display_name;
        m_flag = flag;
        m_selected = selected;
    }

    ~CSecurityOption() {}
    CString     m_display_name;
    UINT8       m_flag;
    BOOL        m_selected;
};

public:
    UINT8   m_security_level_in;
    UINT8   m_security_level_out;
	BOOL	m_isServer;

protected:

	// Generated message map functions
	//{{AFX_MSG(CSecurityDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeSecurityList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    CArray <CSecurityOption, CSecurityOption&> m_internal_list;
private:
    void PresentNewList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECURITYDLG_H__F068ABFE_0701_47E9_97F4_FB15604DBC1B__INCLUDED_)
