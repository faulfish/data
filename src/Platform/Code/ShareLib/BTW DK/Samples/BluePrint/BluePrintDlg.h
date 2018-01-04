// BluePrintDlg.h : header file
//

#if !defined(AFX_BLUEPRINTDLG_H__0F5FBAB8_1F35_46D9_B787_92E0D8127CAF__INCLUDED_)
#define AFX_BLUEPRINTDLG_H__0F5FBAB8_1F35_46D9_B787_92E0D8127CAF__INCLUDED_

//#include "BtIfDefinitions.h"
#include "BtIfClasses.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CBluePrintDlg dialog

class CBluePrintDlg : public CDialog
{
// Construction
public:
	CBluePrintDlg(CWnd* pParent = NULL);	// standard constructor
    ~CBluePrintDlg();

// Dialog Data
	//{{AFX_DATA(CBluePrintDlg)
	enum { IDD = IDD_BLUEPRINT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBluePrintDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

    void AddLogText(const CString& msg);
	
    BD_ADDR m_BdAddr;
    BOOL m_isConnected;
    char m_serviceName[BT_MAX_SERVICE_NAME_LEN + 1];
    CString     m_Name; // remote server's friendly name

    

	// Generated message map functions
	//{{AFX_MSG(CBluePrintDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStartButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLUEPRINTDLG_H__0F5FBAB8_1F35_46D9_B787_92E0D8127CAF__INCLUDED_)
