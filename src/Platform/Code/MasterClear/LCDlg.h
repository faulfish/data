// LCDlg.h : header file
//

#if !defined(AFX_LCDLG_H__7D40F902_60A2_4394_978B_E5E48EAB9346__INCLUDED_)
#define AFX_LCDLG_H__7D40F902_60A2_4394_978B_E5E48EAB9346__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Label.h"
#include "SetPower.h"
#include "extern.h"
#include "Initial.h"
#include "..\Modules\QisdaNVModule\NVItem.h"
/////////////////////////////////////////////////////////////////////////////
// CLCDlg dialog

class CLCDlg : public CDialog
{
// Construction
public:
	CLCDlg(CWnd* pParent = NULL);	// standard constructor

	CSetPower		PowerSupply;
	void writeLog();

	CString SN;
	CString IMEI;
	CString BT;
	CString PPF;
	CString errcode;
	CString errmsg;

	//HMODULE hDll,hDll2,MobileToolKitDll;

	char            temp[512];
	int             comport;
	int             f_MeasureDelay;
	int     FQC;
	int     OQC;
	int     OOB;
	CInitial Initial;
// Dialog Data
	//{{AFX_DATA(CLCDlg)
	enum { IDD = IDD_LC_DIALOG };
	CLabel	m_ppf;
	CLabel	m_imei;
	CLabel	m_bt;
	CLabel	m_flag;
	CLabel	m_sw;
	CLabel	m_hw;
	CLabel	m_sn;
	CLabel	m_mess;
	CListBox	m_list;
	CLabel	m_title;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LCDLG_H__7D40F902_60A2_4394_978B_E5E48EAB9346__INCLUDED_)
