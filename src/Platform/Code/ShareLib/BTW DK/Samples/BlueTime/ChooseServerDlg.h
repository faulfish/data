/////////////////////////////////////////////////////////////////////////////
//
//  Name        ChooseServerDlg.h
//  $Header:
//
//  Function    this file contains the declarations for the CChooseServerDlg
//              class which is the dialog for choosing a Time Server.
//
//  Date                         Modification
//  -----------------------------------------
//  12/22/2000    P Chaisson     Create
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
    CButton     m_DiscoverButton;
    CButton     m_OkButton;
    CListCtrl   m_ServerList;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CChooseServerDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    virtual void OnStackStatusChange(STACK_STATUS new_status);

    // Attributes
public:
    CWinThread* m_pDiscoveryWorkerThread;
    HANDLE m_hEventStopDiscoveryThread;
    HANDLE m_hEventDiscoveryThreadKilled;
    BOOLEAN m_isDialogClosing;

    UINT16 m_Psm;
    BD_ADDR m_BdAddr;
    GUID* m_pServiceGuid;

// Operations
public:
    void DoAddDevice(BD_ADDR p_bda, BD_NAME bd_name);

private:
    // methods to replace virtual methods in base class CBtIf 
    void OnDeviceResponded (BD_ADDR bda, DEV_CLASS devClass, BD_NAME bdName, BOOL bConnected);
    void OnDiscoveryComplete ();
    BD_ADDR m_BdAddrFilter;
    void GetBDAFilterFromReg();
    BOOL string2BDAddr(BD_ADDR bdAddr, char *lpbdAddrString);
    


// Implementation
protected:

//    static void OnDeviceFound(void *app_info, BD_ADDR p_bda, DEV_CLASS dev_class, BD_NAME bd_name, BOOL bConnected);
    static UINT DiscoverServices(LPVOID pParam);
//    static void OnDiscoveryComplete (void *app_info);

//    CBtIf m_BtIf;

    // Generated message map functions
    //{{AFX_MSG(CChooseServerDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnClickServerList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDestroy();
    afx_msg void OnDiscoverButton();
    afx_msg void OnBondingButton();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHOOSESERVERDLG_H__9A90A1C8_6C4A_4F84_9864_C89884BB8073__INCLUDED_)
