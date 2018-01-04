// OutlookTesterDlg.h : header file
//
#include "../../CommonUtil/notifybase.h"
#include "../../Components/OutlookAccess/IOutlookCtrl.h"



#pragma once
// COutlookTesterDlg dialog
class COutlookTesterDlg : public CDialog, public QSYNC_LIB::INotify
{
public:
	COutlookTesterDlg(CWnd* pParent = NULL);	// standard constructor
	enum { IDD = IDD_OUTLOOKTESTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CButton m_BtnNew;
	CButton m_BtnModify;
	CButton m_BtnDelete;
	CButton m_BtnNewCal;
	CButton m_BtnModifyCal;
	CButton m_BtnDeleteCal;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	
	virtual void Event(const TSTRING& strEvent,long nParam);
private:
	bool m_bFinishContactFetch;
	bool m_bFinishCalFetch;
	QSYNC_LIB::IOutlookCtrl* m_pContactCtrl;
	QSYNC_LIB::IOutlookCtrl* m_pCalendarCtrl;
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
};
