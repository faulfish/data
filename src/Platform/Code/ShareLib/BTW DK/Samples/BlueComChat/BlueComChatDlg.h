/////////////////////////////////////////////////////////////////////////////
//
//  Name        BlueComChatDlg.h
//  $Header:
//
//  Function    this file contains the declarations for the CBlueComChatDlg
//              class which is the main dialog of the application.
//
//  Date                         Modification
//  -----------------------------------------
//  22Mar2001   Jim French      Create
//
//  Copyright (c) 2000-2006, Broadcom Corporation, All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLUECOMCHATDLG_H__2A8095B1_F8CB_4A8E_8B18_DFA53414EFA8__INCLUDED_)
#define AFX_BLUECOMCHATDLG_H__2A8095B1_F8CB_4A8E_8B18_DFA53414EFA8__INCLUDED_

#include "btwlib.h"
#include "SerialComm.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CBlueComChatDlg dialog

class CBlueComChatDlg : public CDialog, public CSppClient, public CSppServer
{
// Construction
public:
	CBlueComChatDlg(CWnd* pParent = NULL);	// standard constructor
    ~CBlueComChatDlg();

// Dialog Data
	//{{AFX_DATA(CBlueComChatDlg)
	enum { IDD = IDD_BLUECOMCHAT_DIALOG };
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
	//{{AFX_VIRTUAL(CBlueComChatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

    short m_ComPort;
    BOOL  m_isConnected;
    
// Implementation
protected:
	void StartSession ();
	void StopSession ();
	void AddLogText(const CString& msg);
	BOOL DoChooseServer();
	BOOL DoCreateServiceRecord();

    // Methods that are required in order to receive events
    // from the CSppClient and CSppServer base classes.
    //
    void OnClientStateChange(BD_ADDR bda, DEV_CLASS dev_class, BD_NAME name, short com_port, SPP_STATE_CODE state);
    void OnServerStateChange(BD_ADDR bda, DEV_CLASS dev_class, BD_NAME name, short com_port, SPP_STATE_CODE state);

	HICON m_hIcon;
    CBtIf *m_pBtIf; // instantiating this object initializes access to the stack server !!
    CSdpService *m_pSdpService;
    BD_ADDR m_BdAddr;
    UINT8 m_scn;
	BOOL m_isServer;
    BOOL m_isSessionStarted;
    char m_serviceName[BT_MAX_SERVICE_NAME_LEN + 1];
    CString     m_Name; // remote server's friendly name
    CSerialComm serialComm;
    UINT_PTR    m_timer_id;
    BOOL  m_isConnectedPrevious;

	// Generated message map functions
	//{{AFX_MSG(CBlueComChatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStartButton();
	afx_msg void OnSendButton();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnReadLinkModeButton();
    afx_msg void OnSetSniffModeButton();
    afx_msg void OnCancelSniffModeButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLUECOMCHATDLG_H__2A8095B1_F8CB_4A8E_8B18_DFA53414EFA8__INCLUDED_)
