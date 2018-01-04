/////////////////////////////////////////////////////////////////////////////
//
//  Name        BlueChatDlg.h
//  $Header:
//
//  Function    this file contains the declarations for the CBlueChatDlg
//              class which is the main dialog of the application.
//
//  Date                         Modification
//  -----------------------------------------
//  12/22/2000    P Chaisson     Create
//
//  Copyright (c) 2000-2006, Broadcom Corporation, All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLUECHATDLG_H__2A8095B1_F8CB_4A8E_8B18_DFA53414EFA8__INCLUDED_)
#define AFX_BLUECHATDLG_H__2A8095B1_F8CB_4A8E_8B18_DFA53414EFA8__INCLUDED_

#include "btwlib.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CBlueChatDlg dialog

class CBlueChatDlg : public CDialog, public CRfCommPort
{
// Construction
public:
	CBlueChatDlg(CWnd* pParent = NULL);	// standard constructor
    ~CBlueChatDlg();

// Dialog Data
	//{{AFX_DATA(CBlueChatDlg)
	enum { IDD = IDD_BLUECHAT_DIALOG };
	CStatic	m_CtsRtsStatusLight;
	CButton	m_CtsRtsButton;
	CStatic	m_DsrDtrStatusLight;
	CButton	m_DsrDtrButton;
	CEdit	m_NewLineEdit;
	CButton	m_SendButton;
	CButton	m_ServerCheck;
	CEdit	m_ChatLogEdit;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlueChatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

    
// Implementation
protected:

	void StartSession ();
	void StopSession ();
	void SetStatusLights();
	void AddLogText(const CString& msg);
	BOOL DoChooseServer();
	BOOL DoCreateServiceRecord();

    // Methods that are required in order to receive data and
    // events respectively from the RFCommPort base class.
    //
	void OnDataReceived (void *p_data, UINT16 len);
	void OnEventReceived (UINT32 event_code);
	UINT_PTR m_timer_id;

	HICON m_hIcon;
    CBtIf *m_pBtIf; // instantiating this object initializes access to the stack server !!
    CSdpService *m_pSdpService;
    BD_ADDR m_BdAddr;
    CRfCommIf m_RfCommIf;
   	CBitmap m_bmGreen;
	CBitmap m_bmRed;
	BOOL m_isServer;
    BOOL m_isSessionStarted;
    BOOL m_isDsrDtrToggleOn;
    BOOL m_isCtsRtsToggleOn;
    BOOL m_isConnected;

	// Generated message map functions
	//{{AFX_MSG(CBlueChatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStartButton();
	afx_msg void OnSendButton();
	afx_msg void OnDestroy();
	afx_msg void OnDsrdtrButton();
	afx_msg void OnCtsrtsButton();
	afx_msg void OnStatsButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLUECHATDLG_H__2A8095B1_F8CB_4A8E_8B18_DFA53414EFA8__INCLUDED_)
