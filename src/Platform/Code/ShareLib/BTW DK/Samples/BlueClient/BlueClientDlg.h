// BlueClientDlg.h : header file
//
//  Copyright (c) 2000-2004, Broadcom Corporation, All Rights Reserved.

#if !defined(AFX_BLUECLIENTDLG_H__8F93D002_A752_426B_9FB0_DB259FE05663__INCLUDED_)
#define AFX_BLUECLIENTDLG_H__8F93D002_A752_426B_9FB0_DB259FE05663__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CBtExplorerDlg dialog

#include "ModelessDlg.h"
#include "btwlib.h"

class CBdAddrString : CString
{
public:
    CBdAddrString(BD_ADDR bda)
    {
        Format("%02X:%02X:%02X:%02X:%02X:%02X", bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);
    }
};

#define IMG_FILE_LISTING       0       // Tree image for file listing
#define IMG_DEVICE_DESKTOP     1       // Tree image for Desktop item
#define IMG_DEVICE_LAN         2       // Tree image for LAN item
#define IMG_DEVICE_MODEM       3       // Tree image for Modem item
#define IMG_DEVICE_CELL_PHONE  4       // Tree image for Cell Phone item
#define IMG_DEVICE_PDA         5       // Tree image for PDA item
#define IMG_DEVICE_BT_UNKNOWN  6       // Tree image for unknown BT device item
#define IMG_BT_NEIGHBORHOOD    7       // Tree image for BT Neighborhood
#define IMG_BT_VCARD           8       // Tree image for Vcard
#define IMG_BT_FOLDER          9       // Tree image for Folder
#define IMG_BT_FOLDER_OPEN     10      // Tree image for Open Folder
#define IMG_BT_REMOTE_DOCUMENT 11      // Tree image for a remote document
#define IMG_BT_REMOTE_FOLDER   12      // Tree image for a remote Folder
#define IMG_BT_FOLDER_UP       13      // Tree image for the standard "up one folder" icon
#define IMG_DEVICE_LAPTOP      14      // Tree image for Laptop item

class CDeviceInfo 
{
public:
    CDeviceInfo(BD_ADDR bda, DEV_CLASS dc, BD_NAME bdn);
    CDeviceInfo *pNext;

    BD_ADDR   bd_addr;
    DEV_CLASS dev_class;
    BD_NAME   bd_name;

    HTREEITEM m_item;
    int       m_nServices;
    long      m_hFtp;
	short     m_sSPPPort;
	short     m_sLAPPort;
	short     m_sDUNPort;
	short     m_sFAXPort;
};




/////////////////////////////////////////////////////////////////////////////
// CBlueClientDlg dialog

class CBlueClientDlg : public CDialog, public CBtIf, public COppClient, public CFtpClient
{
// Construction
public:
	CBlueClientDlg(CWnd* pParent = NULL);	// standard constructor
    virtual ~CBlueClientDlg();

enum FTPState {
	FTP_IDLE = 0,
	FTP_BUSY,
	FTP_OPENING,
	FTP_OPEN_EVENT,
	FTP_CLOSE_EVENT,
	FTP_GET_EVENT,
	FTP_PUT_EVENT,
	FTP_DELETE_EVENT,
	FTP_ABORT_EVENT,
	FTP_PROGRESS_EVENT,
	FTP_FOLDER_LIST_EVENT,
	FTP_CHANGE_FOLDER_EVENT,
	FTP_CLOSING,
	FTP_ERROR,
    FTP_CREATE_EVENT
};
typedef enum FTPState tFTPState;

    void NewDevice (BD_ADDR bd_addr, DEV_CLASS dev_class, BD_NAME bd_name, BOOL bConnected = FALSE); 
    void DeleteDevice (BD_ADDR bd_addr); 
	tFTPState GetFTPState();
	void SetFTPState(tFTPState state);
	void ResizeControlsToFit();

    static CDeviceInfo *pFirstDeviceInfo;
	CStatusBar   m_statusBar;
	void SetWaitCursor( bool bEnable );
	bool m_bWaitCursor;
	HANDLE m_hEventStopDiscovery;

// Dialog Data
	//{{AFX_DATA(CBlueClientDlg)
	enum { IDD = IDD_BLUECLIENT_DIALOG };
	CTreeCtrl	m_tree;
	//}}AFX_DATA

    CImageList  m_ImgList;
	HTREEITEM m_hTreeItem;
	CString m_szTreeFileName;
    void GetAvailableServices (BD_ADDR p_bd_addr, int *p_m_nServices, int max_services, CSdpDiscoveryRec * si);
    CDeviceInfo* m_pDeviceSelected;

    enum {MAX_SERVICES = 32};

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlueClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Operations

// Implementation
protected:
	HICON m_hIcon;
	tFTPState m_FTPState;

    static UINT DiscoverServices(LPVOID pParam);
    void FtpRootBrowse(); 
    void DefaultBusinessCardBrowse(); 
    void InboxBrowse(); 
    void FTPInboxBrowse(); 


	// Generated message map functions
	//{{AFX_MSG(CBlueClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDblclkClientTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickClientTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpandingClientTree(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnProperties();
    afx_msg void OnRefresh();
    afx_msg void OnRefreshNeighbors();
	afx_msg void OnRefreshFolderListing();
	afx_msg void OnFTPChangeRemoteDirectory();
	afx_msg void OnFTPDeleteRemoteDirectory();
	afx_msg void OnFTPGetFile();
	afx_msg void OnFTPPutFile();
	afx_msg void OnFTPDeleteFile();
	afx_msg void OnFTPRoot();
	afx_msg void OnFTPAbort();
	afx_msg void OnFTPCreateFolder();
	afx_msg void OnFTPCreateFile();
    afx_msg void OnPullVcard();
    afx_msg void OnExchangeVcard();
    afx_msg void OnPushVcard();
    afx_msg void OnPushVcal();
    afx_msg void OnPushVnote();
    afx_msg void OnPushVmessage();
 	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    HTREEITEM   m_hTreeItemRoot;

private:
    UINT_PTR    m_timer_id;

	int m_hFTP;
    int m_num_services;
	int m_si_num_services;
    CSdpDiscoveryRec  m_si[MAX_SERVICES];
	bool GetFtpDefaultRoot(LPTSTR szRootDir, DWORD dwSize);

    BOOL m_bInquiryComplete;

    CFtpClient::FTP_RETURN_CODE m_ftp_rc;

    // methods to replace virtual methods in base class CBtIf 
    void OnDeviceResponded (BD_ADDR bda, DEV_CLASS devClass, BD_NAME bdName, BOOL bConnected);
    void OnInquiryComplete (BOOL success, short num_responses);
    void OnDiscoveryComplete (UINT16 nRecs, long lResultCode);

    // methods to replace virtual methods in base class CFtpClient 
    void OnOpenResponse(FTP_RESULT_CODE result_code);
    void OnCloseResponse(FTP_RESULT_CODE result_code);
    void OnProgress(FTP_RESULT_CODE result_code, WCHAR * name, long current, long total);
    void OnPutResponse(FTP_RESULT_CODE result_code, WCHAR * name);
    void OnGetResponse(FTP_RESULT_CODE result_code, WCHAR * name);
    void OnCreateResponse(FTP_RESULT_CODE result_code, WCHAR * name);
    void OnDeleteResponse(FTP_RESULT_CODE result_code, WCHAR * name);
    void OnChangeFolderResponse(FTP_RESULT_CODE result_code, tFtpFolder folder_type, WCHAR * folder_name);
    void OnFolderListingResponse(FTP_RESULT_CODE result_code, tFTP_FILE_ENTRY * listing, long entries);
    void OnXmlFolderListingResponse(FTP_RESULT_CODE rc, WCHAR * pfolder_listing, long folder_length );
    void OnAbortResponse(FTP_RESULT_CODE result_code);

    // methods to replace virtual methods in base class COppClient 
    void OnProgress(OPP_RESULT_CODE result_code, BD_ADDR bda, WCHAR * string, long current, long total);
    void OnPushResponse(OPP_RESULT_CODE result_code,  BD_ADDR bda, WCHAR * string);
    void OnPullResponse(OPP_RESULT_CODE result_code , BD_ADDR bda, WCHAR * string);
    void OnExchangeResponse(OPP_RESULT_CODE result_code, BD_ADDR bda, WCHAR * string);
    void OnAbortResponse (OPP_RESULT_CODE result_code);

    BD_ADDR m_BdAddrFilter;
    void GetBDAFilterFromReg();
    BOOL string2BDAddr(BD_ADDR bdAddr, char *lpbdAddrString);
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLUECLIENTDLG_H__8F93D002_A752_426B_9FB0_DB259FE05663__INCLUDED_)
/////////////////////////////////////////////////////////////////////////////
// FTPCreate dialog

class FTPCreate : public CDialog
{
// Construction
public:
	FTPCreate(CWnd* pParent = NULL);   // standard constructor
	bool m_bCreateFile;

// Dialog Data
	//{{AFX_DATA(FTPCreate)
	enum { IDD = IDD_FTPCreate };
	CString	m_Name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FTPCreate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(FTPCreate)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CBtDeviceProps dialog

class CBtDeviceProps : public CModelessDlg
{
// Construction
public:
	CBtDeviceProps(CDeviceInfo *pDevice, UINT num_services, CSdpDiscoveryRec *pSI, CWnd* pParent = NULL);   // standard constructor
    virtual ~CBtDeviceProps();

    static BOOL m_bActive;
    static CBtDeviceProps  *m_pDlg;

// Dialog Data
	//{{AFX_DATA(CBtDeviceProps)
	enum { IDD = IDD_BT_EXP_DEV_PROPS };
	CListBox	m_lbService;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBtDeviceProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBtDeviceProps)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    CDeviceInfo m_devInfo;

    UINT m_num_services;
    CSdpDiscoveryRec *p_si;
    
};
