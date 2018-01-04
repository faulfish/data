// PrintingDlg.h

#pragma once

//#include "BtPrintUtils.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPrintingDlg dialog

class CPrintingDlg : public CDialog
{
public:
	CString				m_sFileToPrint;
	BOOL                m_bIsPrintActive;
	CPrintClient::ePRINT_PROFILE      m_PrintProfile;
    CPrintClient        *m_pPrintClient;
	BD_ADDR				m_bdaddr;
    
	CPrintingDlg(CPrintClient::ePRINT_PROFILE ProfileToUse, BD_ADDR bd_addr, LPCTSTR pszFileToPrint, CWnd* pParent = NULL);
    ~CPrintingDlg();

	//{{AFX_DATA(CPrintingDlg)
	enum { IDD = IDD_PRINTING };
	CButton				m_cCancelBtn;
	CStatic				m_cCancelStatic;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPrintingDlg)
	protected:
	virtual void		DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:

	void				StartPrinting();
	void				MonitorPrinting();

	// Generated message map functions
	//{{AFX_MSG(CPrintingDlg)
	virtual BOOL		OnInitDialog();
	afx_msg void		OnTimer(UINT_PTR nIDEvent);
	virtual void		OnOK();
	virtual void		OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
