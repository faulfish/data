// BlueTimeDlg.h : header file
//
//  Copyright (c) 2000-2004, Broadcom Corporation, All Rights Reserved.

#if !defined(AFX_BLUETIMEDLG_H__3EFB6FDA_A230_484A_97CD_D63A26577C64__INCLUDED_)
#define AFX_BLUETIMEDLG_H__3EFB6FDA_A230_484A_97CD_D63A26577C64__INCLUDED_
//#include "TimeConn.h" // Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "btwlib.h"

/////////////////////////////////////////////////////////////////////////////
// CBlueTimeDlg dialog

class CBlueTimeDlg : public CDialog, public CL2CapConn
{
// Construction
public:
    CBlueTimeDlg(CWnd* pParent = NULL); // standard constructor
    ~CBlueTimeDlg();

// Dialog Data
    //{{AFX_DATA(CBlueTimeDlg)
    enum { IDD = IDD_BLUETIME_DIALOG };
    CButton m_SecurityButton;
    CRichEditCtrl   m_remote_time;
    CRichEditCtrl   m_local_time;
    CButton m_ServerCheck;
    CString m_connected_device;
    CString m_connected_text;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CBlueTimeDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
// Attributes
public:
    // control for clock thread
    CWinThread* m_pClockThread;
    HANDLE m_hEventStopClockThread;
    HANDLE m_hEventClockThreadKilled;
    BOOL m_isDialogClosing;

    BD_ADDR m_BdAddr;
    BD_ADDR m_connected_BdAddr;
    CL2CapIf *m_pL2CapIf;
    BOOL m_isServiceRecordCreated;
    BOOL m_isSessionStarted;
    BOOL m_isServer;

// Operations
public:
    static UINT RunningClock(LPVOID pParam);
    
// Implementation
protected:
    void StartSession ();
    void StopSession ();
    BOOL DoChooseServer();
    void DoInitComServer();
    BOOL DoCreateServiceRecord();
    BOOL SecurityOK();

    // methods to replace the virtual methods in CL2CapConn
    void OnIncomingConnection ();
    void OnConnected();
    void OnDataReceived (void *p_data, UINT16 length);
    void OnCongestionStatus (BOOL is_congested);
    void OnRemoteDisconnected (UINT16 reason);

    HICON m_hIcon;
//    CBtIf m_BtIf; // instantiatiing this object initiates access to the stack server!!!!
    CSdpService *m_pSdpService;
    BOOL      m_IsConnected;
    UINT8     m_security_level;

    // Generated message map functions
    //{{AFX_MSG(CBlueTimeDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnStartButton();
    afx_msg void OnDestroy();
    afx_msg void OnSecurityButton();
    afx_msg void OnStatsButton();
    afx_msg LRESULT OnIncomingMessage(WPARAM wParam, LPARAM lParam);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLUETIMEDLG_H__3EFB6FDA_A230_484A_97CD_D63A26577C64__INCLUDED_)
