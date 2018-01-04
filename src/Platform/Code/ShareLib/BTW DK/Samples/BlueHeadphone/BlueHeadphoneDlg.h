// BlueHeadphoneDlg.h : header file
//

#if !defined(AFX_BLUEHEADPHONEDLG_H__B16EE75F_513F_4693_B1B4_446C1F8833B7__INCLUDED_)
#define AFX_BLUEHEADPHONEDLG_H__B16EE75F_513F_4693_B1B4_446C1F8833B7__INCLUDED_
#include "btwlib.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CBlueHeadphoneDlg dialog

class CBlueHeadphoneDlg : public CDialog, public CHeadphoneClient
{
// Construction
public:
    long    m_handle;
    void AddLogText(const CString& msg);
    CEdit	m_LogEdit;

	CBlueHeadphoneDlg(CWnd* pParent = NULL);	// standard constructor
    ~CBlueHeadphoneDlg();

// Dialog Data
	//{{AFX_DATA(CBlueHeadphoneDlg)
	enum { IDD = IDD_BLUEHEADPHONE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlueHeadphoneDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

   

protected:
	HICON m_hIcon;

    
	BOOL DoChooseServer();
	BOOL DoCreateServiceRecord();
   
    char    m_serviceName[BT_MAX_SERVICE_NAME_LEN + 1];
    UINT8   m_scn;
    BOOL    m_isConnected;
    BD_ADDR m_BdAddr;
    CString m_Name; // remote server's friendly name
    

	// Generated message map functions
	//{{AFX_MSG(CBlueHeadphoneDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnStartButton();
	afx_msg void OnDisconnect();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLUEHEADPHONEDLG_H__B16EE75F_513F_4693_B1B4_446C1F8833B7__INCLUDED_)
