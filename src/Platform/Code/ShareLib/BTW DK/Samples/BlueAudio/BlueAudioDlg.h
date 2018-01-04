/////////////////////////////////////////////////////////////////////////////
//
//  Name        BlueAudioDlg.h
//  $Header:
//
//  Function    this file contains the declarations for the CBlueAudioDlg
//              class which is the main dialog of the application.
//
//  Date                         Modification
//  -----------------------------------------
//  12/22/2000    P Chaisson     Create
//
//  Copyright (c) 2000-2006, Broadcom Corporation, All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BlueAudioDLG_H__2A8095B1_F8CB_4A8E_8B18_DFA53414EFA8__INCLUDED_)
#define AFX_BlueAudioDLG_H__2A8095B1_F8CB_4A8E_8B18_DFA53414EFA8__INCLUDED_

#include "BtIfDefinitions.h"
#include "BtIfClasses.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CBlueAudioDlg dialog

class CBlueAudioDlg : public CDialog, public CRfCommPort
{
// Construction
public:
	CBlueAudioDlg(CWnd* pParent = NULL);	// standard constructor
    ~CBlueAudioDlg();

// Dialog Data
	//{{AFX_DATA(CBlueAudioDlg)
	enum { IDD = IDD_BLUEAUDIO_DIALOG };
	CStatic	m_CtsRtsStatusLight;
	CButton	m_CtsRtsButton;
	CStatic	m_DsrDtrStatusLight;
	CButton	m_DsrDtrButton;
	CButton	m_ServerCheck;
	CEdit	m_ChatLogEdit;
    CButton	m_Sco;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlueAudioDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

    
// Implementation
protected:

	void StartSession ();
	void StopSession ();
	void AddLogText(const CString& msg);
	BOOL DoChooseServer();
	BOOL DoCreateServiceRecord();
    BOOL DoESco(BOOL bChangeEsco = FALSE);
    	

    // Methods that are required in order to receive data and
    // events respectively from the RFCommPort base class.
    //
	void OnDataReceived (void *p_data, UINT16 len);
	void OnEventReceived (UINT32 event_code);
    void OnAudioConnected(UINT16 audioHandle);
    void OnAudioDisconnect(UINT16 audioHandle);
	UINT_PTR    m_timer_id;

	HICON m_hIcon;
    CBtIf *m_pBtIf; // instantiating this object initializes access to the stack server !!
    CSdpService *m_pSdpService;
    BD_ADDR m_BdAddr;
    CRfCommIf m_RfCommIf;
	BOOL m_isServer;
    BOOL m_isSessionStarted;
    BOOL m_isConnected;
    UINT16 m_audio_handle;
    int     m_scoType;
    tBTM_ESCO_EVT_DATA m_EScoEventData;
    tBTM_ESCO_PARAMS   m_EScoParams;
    tBTM_CHG_ESCO_PARAMS m_chgEscoParam;
   

	// Generated message map functions
	//{{AFX_MSG(CBlueAudioDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStartButton();
	afx_msg void OnDestroy();
	afx_msg void OnStatsButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg LRESULT OnConnect2Audio(WPARAM wParam, LPARAM lParam);
   	afx_msg void OnSco();
	afx_msg void OnEsco();
	afx_msg void OnReadesco();
	afx_msg void OnChangeesco();	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BlueAudioDLG_H__2A8095B1_F8CB_4A8E_8B18_DFA53414EFA8__INCLUDED_)
