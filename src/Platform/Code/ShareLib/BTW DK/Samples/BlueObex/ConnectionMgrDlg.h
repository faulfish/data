// ConnectionMgrDlg.h : header file
//
//  Copyright (c) 2000-2005, Broadcom Corporation, All Rights Reserved.

#if !defined(AFX_CONNECTIONMGRDLG_H__0298060A_A0D5_4FD6_A1F2_E80FFF91CEB1__INCLUDED_)
#define AFX_CONNECTIONMGRDLG_H__0298060A_A0D5_4FD6_A1F2_E80FFF91CEB1__INCLUDED_
#include <AFXTEMPL.H>

#include "BtIfDefinitions.h"
#include "btwlib.h"
#include "checkheaders.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHeadersListEntry
{
public:

	CHeadersListEntry ();
	CHeadersListEntry (WCHAR * p_file_name); 
	~CHeadersListEntry();

	CString file;
	UINT8	obex_req_resp;		// request (OBEX_REQ_*) or response code (tOBEX_RESPONSE_CODE)
	UINT16	mtu;				// Maximum transmission unit - client open req or server open confirm
	BD_ADDR	bd_addr;			// only for client open req
	UINT8	scn;				// only for client open req
	BOOL	final;				// for client put req and get req; for server get confirm
	BOOL	backup;				// for client set path req
	BOOL	create;				// for client set path req
	CObexHeaders *p_object;		// object version of tOBEX_HEADERS

    // This class will not support the compiler-supplied copy constructor or assignment operator,
    // so these are declared private to prevent inadvertent use by the application.
    CHeadersListEntry(const CHeadersListEntry & x);
    CHeadersListEntry& operator= (const CHeadersListEntry & x); 
};


/////////////////////////////////////////////////////////////////////////////
// CConnectionMgrDlg dialog

class CConnectionMgrDlg : public CDialog, public CObexClient, public CObexServer
{
// Construction
public:
	CConnectionMgrDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConnectionMgrDlg)
	enum { IDD = IDD_CONNECTION_DIALOG };
	CEdit	m_SendReceiveLog;
	CButton	m_ServerCheck;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConnectionMgrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation

    void AddLogText(const CString& msg);

protected:

	HICON m_hIcon;
	void StartSession ();
	void StopSession ();
	BOOL DoChooseServer();
	BOOL DoCreateServiceRecord();
	ObexCheckHeaders checker;

    // Methods that are required in order to receive events
    // from the CObexClient base class.
	virtual void OnOpen(CObexHeaders *p_confirm, UINT16 tx_mtu, tOBEX_ERRORS code, tOBEX_RESPONSE_CODE response);
	virtual void OnClose(CObexHeaders *p_confirm, tOBEX_ERRORS code, tOBEX_RESPONSE_CODE response);
	virtual void OnAbort (CObexHeaders *p_confirm, tOBEX_ERRORS code, tOBEX_RESPONSE_CODE response);
	virtual void OnPut (CObexHeaders *p_confirm, tOBEX_ERRORS  code, tOBEX_RESPONSE_CODE response);
	virtual void OnGet (CObexHeaders *p_confirm, tOBEX_ERRORS  code, BOOL final, tOBEX_RESPONSE_CODE response);
	virtual void OnSetPath (CObexHeaders *p_confirm, tOBEX_ERRORS code, tOBEX_RESPONSE_CODE response);

    // Methods that are required in order to receive events
    // from the CObexServer base class.
	virtual void OnOpenInd(CObexHeaders *p_request);
	virtual void OnSetPathInd (CObexHeaders * p_request, BOOL backup, BOOL create);
	virtual void OnPutInd (CObexHeaders * p_request, BOOL final);
	virtual void OnPutCreateInd(CObexHeaders * p_request);
	virtual void OnPutDeleteInd (CObexHeaders * p_request);
	virtual void OnGetInd (CObexHeaders * p_request, BOOL final);
	virtual void OnAbortInd (CObexHeaders * p_request);
	virtual void OnCloseInd(CObexHeaders *p_request);

	CBtIf *m_pBtIf; // instantiating this object initializes access to the stack server !!
    CSdpService *m_pSdpService;
    BD_ADDR m_BdAddr;
    CRfCommIf m_RfCommIf;
    UINT8	m_scn;
	UINT8	*p_m_target;
	UINT16	m_mtu;

	BOOL m_isServer;
    BOOL m_isSessionStarted;
    BOOL m_isConnected;
    char m_serviceName[BT_MAX_SERVICE_NAME_LEN + 1];
    CString     m_Name; // remote server's friendly name

	// Generated message map functions
	//{{AFX_MSG(CConnectionMgrDlg)
	afx_msg void OnDblclkSendReceiveList();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnStartButton();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    UINT_PTR    m_timer_id;

private:
	CArray <CHeadersListEntry, CHeadersListEntry&> m_client_list;
	CArray <CHeadersListEntry, CHeadersListEntry&> m_server_list;

	int client_list_index;
	int client_list_max;

    int server_list_index;
	int server_list_max;

	void CConnectionMgrDlg::SendNextClientObject();		
    void CConnectionMgrDlg::SendNextServerObject(UINT8 request_code, BOOL create_req = FALSE, BOOL delete_req = FALSE);		

	void CConnectionMgrDlg::init_objects();		// setup some objects for testing

	CObexHeaders client_open_headers;
	CObexHeaders client_close_headers;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNECTIONMGRDLG_H__0298060A_A0D5_4FD6_A1F2_E80FFF91CEB1__INCLUDED_)
