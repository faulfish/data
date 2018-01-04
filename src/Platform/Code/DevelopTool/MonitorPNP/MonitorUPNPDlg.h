// MonitorUPNPDlg.h : header file
//
#include "..\..\CommonUtil\NotifyBase.h"

#pragma once

// CMonitorUPNPDlg dialog
class CMonitorUPNPDlg : public CDialog , public QSYNC_LIB::INotify
{
// Construction
public:
	CMonitorUPNPDlg(CWnd* pParent = NULL);	// standard constructor
	virtual void Event(const TSTRING& strEvent,long nParam);

// Dialog Data
	enum { IDD = IDD_MONITORUPNP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	CListBox m_listLog;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeLoglist();
};
