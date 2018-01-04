/////////////////////////////////////////////////////////////////////////////
//
//  Name        ChooseServerDlg.h
//  $Header:
//
//  Function    this file contains the declarations for the CChooseServerDlg
//              class which is the dialog for choosing a Chat Server.
//
//  Date                         Modification
//  -----------------------------------------
//  5Jun2001    Jim French  Create
//
//  Copyright (c) 2000-2006, Broadcom Corporation, All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHOOSESERVERDLG_H__9A90A1C8_6C4A_4F84_9864_C89884BB8073__INCLUDED_)
#define AFX_CHOOSESERVERDLG_H__9A90A1C8_6C4A_4F84_9864_C89884BB8073__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "btwlib.h"


/////////////////////////////////////
//
// Handy class for holding Device
// information held in the item list.
//
class CBdInfo
{
public:

    CBdInfo() : m_Scn(0) { m_Name = ""; m_serviceName[0] = 0;  memset (m_BdAddr, 0, BD_ADDR_LEN); }
    CBdInfo(BD_ADDR bd_addr, BD_NAME bd_name) : m_Scn(0), m_Name(bd_name) { m_serviceName[0] = 0; memcpy (m_BdAddr, bd_addr, BD_ADDR_LEN); }
    ~CBdInfo() { ; }
    BOOL isBdAddrEqual(BD_ADDR bd_addr) { return (0 == memcmp(m_BdAddr, bd_addr, sizeof(BD_ADDR))); }
    CString DeviceAsString()
    {
        CString s;

        // Default the string to the BD Address only.
        //
        s.Format("[%x:%x:%x:%x:%x:%x]", m_BdAddr[0], m_BdAddr[1], m_BdAddr[2], m_BdAddr[3], m_BdAddr[4], m_BdAddr[5]);

        // If friendly name provided then prepend it.
        //
        if (m_Name.GetLength())
            s = m_Name + " " + s;

        return s;
    }
    CString LabelAsString()
    {
        CString label(DeviceAsString());

        // add the service name
        label += " ";
        label += m_serviceName;

        return label;
    }

    BD_ADDR     m_BdAddr;
    CString     m_Name;     // device 'friendly' name
    UINT8       m_Scn;
    char        m_serviceName[BT_MAX_SERVICE_NAME_LEN + 1];

};


/////////////////////////////////////////////////////////////////////////////
// CChooseServerDlg dialog

class CChooseServerDlg : public CDialog, public CBtIf
{
// Construction
public:
	CChooseServerDlg(CWnd* pParent = NULL);   // standard constructor
    ~CChooseServerDlg();

// Dialog Data
	//{{AFX_DATA(CChooseServerDlg)
	enum { IDD = IDD_CHOOSE_SERVER_DIALOG };
	CButton	m_DiscoverButton;
	CButton	m_OkButton;
	CListCtrl	m_ServerList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChooseServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
public:
   	CWinThread* m_pDiscoveryWorkerThread;
	HANDLE m_hEventStopDiscoveryThread;
	HANDLE m_hEventDiscoveryThreadKilled;
    BOOLEAN m_isDialogClosing;
    BOOLEAN m_wasDialogInterrupted;

    UINT8 m_Scn;
    BD_ADDR m_BdAddr;
    GUID* m_pServiceGuid;
    char        m_serviceName[BT_MAX_SERVICE_NAME_LEN + 1];
    CString     m_Name; // remote server's friendly name

// Operations
public:
	void DoAddDevice(BD_ADDR p_bda, BD_NAME bd_name);


private:
    // methods to replace virtual methods in base class CBtIf 
    void OnDeviceResponded (BD_ADDR bda, DEV_CLASS devClass, BD_NAME bdName, BOOL bConnected);
    void OnDiscoveryComplete ();
    void OnInquiryComplete (BOOL success, short num_responses);

    int m_num_services_per_device;
#define MAX_SERVICES_PER_DEVICE    20
    CSdpDiscoveryRec    m_si[MAX_SERVICES_PER_DEVICE];

    int m_num_devices;
#define MAX_SERVER_DEVICES 40
    CBdInfo  *           m_pDevInfo[MAX_SERVER_DEVICES];    

    // Implementation
protected:

    static UINT DiscoverServices(LPVOID pParam);

	// Generated message map functions
	//{{AFX_MSG(CChooseServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickServerList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnDiscoverButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
        UINT_PTR m_timer_id;
        BOOL     m_start_discovery;
		BOOL	 m_discovery_in_progress;
        BD_ADDR m_BdAddrFilter;
        void GetBDAFilterFromReg();
        BOOL string2BDAddr(BD_ADDR bdAddr, char *lpbdAddrString);
    
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHOOSESERVERDLG_H__9A90A1C8_6C4A_4F84_9864_C89884BB8073__INCLUDED_)
