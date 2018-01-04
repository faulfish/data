#if !defined(AFX_CONNSTATSDLG_H__A07AF32C_55B5_4DD4_A64B_8AE2C49B54E6__INCLUDED_)
#define AFX_CONNSTATSDLG_H__A07AF32C_55B5_4DD4_A64B_8AE2C49B54E6__INCLUDED_
//  Copyright (c) 2000-2004, Broadcom Corporation, All Rights Reserved.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConnStatsDlg.h : header file
//
#include "ModelessDlg.h"

#include "btwlib.h"

#include "BlueAudioDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CConnStatsDlg dialog

class CConnStatsDlg : public CModelessDlg
{
// Construction
public:
	CConnStatsDlg(CBlueAudioDlg *p_dlg, CWnd* pParent = NULL);   // standard constructor
    virtual ~CConnStatsDlg();

//static BOOL m_bActive;
static CConnStatsDlg  *m_pDlg;

// Dialog Data
	//{{AFX_DATA(CConnStatsDlg)
	enum { IDD = IDD_CONN_STATS_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConnStatsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConnStatsDlg)
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	UINT_PTR m_timer_id;
	BOOL	 m_previous_connection_status;

	tBT_CONN_STATS m_conn_stats;
	void DisplayCurrentStats();
	CBlueAudioDlg *m_p_dlg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNSTATSDLG_H__A07AF32C_55B5_4DD4_A64B_8AE2C49B54E6__INCLUDED_)
