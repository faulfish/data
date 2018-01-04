#if !defined(AFX_CONFIG_H__020D5F3A_92F0_4421_9134_E26F2C4E9873__INCLUDED_)
#define AFX_CONFIG_H__020D5F3A_92F0_4421_9134_E26F2C4E9873__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Config.h : header file
//
#include "Label.h"
#include "SxGroupBox.h"
#include "ColoredCombo.h"

/////////////////////////////////////////////////////////////////////////////
// CConfig dialog

class CConfig : public CDialog
{
// Construction
public:
	CConfig(CWnd* pParent = NULL);   // standard constructor
    CBrush m_brush;
	CFont   font_check;
	void SetCtrlProperty();
	void SetCtrlText();
	CMenu *pSubMenu;
// Dialog Data
	//{{AFX_DATA(CConfig)
	enum { IDD = IDD_DIALOG_CONFIG };
	CEdit	m_PICS_EDIT;
	CLabel	m_PICS;
	CButton	m_PICS_CHECK;
	CSxGroupBox	m_CHECK_STATION;
	CButton	m_ACM;
	CButton	m_FWT;
	CButton	m_LEAKAGE_CHARGER;
	CButton	m_FLEX_OPTION;
	CButton	m_CAL_PRE_3G;
	CButton	m_CAL_PRE_2G;
	CButton	m_BASE_BAND;
	CSxGroupBox	m_DEBUGINFO;
	CButton	m_DEBUGINFO_CHECK;
	CLabel	m_BACKUP;
	CLabel	m_TIMEOUT;
	CLabel	m_MS;
	CLabel	m_CYCLECOUNT;
	CSxGroupBox	m_CONNECT;
	CSxGroupBox	m_CHECK;
	CSxGroupBox	m_BACKUPPATH;
	CEdit	m_TIMEOUT_EDIT;
	CEdit	m_CYCLECOUNT_EDIT;
	CEdit	m_BACKUP_EDIT;
	CButton	m_SW_CHECK;
	CButton	m_PROCESS_CHECK;
	CButton	m_ME_CHECK;
	CButton	m_HW_CHECK;
	CButton	m_SAVE;
	CButton	m_EXIT;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfig)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonExit();
	afx_msg void OnButtonSave();
	afx_msg void OnCheckProcess();
	afx_msg void OnCheckBaseBand();
	afx_msg void OnCheckCalPreTest2g();
	afx_msg void OnCheckCalPreTest3g();
	afx_msg void OnCheckLeakageCharger();
	afx_msg void OnCheckFunctionWireless();
	afx_msg void OnCheckFlexOption();
	afx_msg void OnCheckAcmBoard();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIG_H__020D5F3A_92F0_4421_9134_E26F2C4E9873__INCLUDED_)
