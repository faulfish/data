// Q_DOWNLOADDlg.h : header file
//

#if !defined(AFX_Q_DOWNLOADDLG_H__788CCB34_C02B_4BEF_B0E4_ADBD1BAEA588__INCLUDED_)
#define AFX_Q_DOWNLOADDLG_H__788CCB34_C02B_4BEF_B0E4_ADBD1BAEA588__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CQ_DOWNLOADDlg dialog
#include "Label.h"
#include <afxMT.h>
//#include "PowerSupply.h"
#include <atlbase.h>
#include "Download_Export.h"
#include "..\..\..\CommonUtil\NotifyBase.h"
#include "..\..\..\Components\MobileFinder\MobileFinderInterface.h"
#include "Q_Download.h"
#include "INITIAL.h"
#include <vector>

#pragma warning(disable : 4996)

//////////////////////////////////////////////////////////////////////////
const int g_nSlotTotalCount = 8;
//////////////////////////////////////////////////////////////////////////

class CQ_DOWNLOADDlg : public CDialog, public QSYNC_LIB::INotify_ANSI, public QSYNC_LIB::INotify
{
// Construction
public:
	CQ_DOWNLOADDlg(CWnd* pParent = NULL);	// standard constructor
	
// Dialog Data
	//{{AFX_DATA(CQ_DOWNLOADDlg)
	enum { IDD = IDD_Q_DOWNLOAD_DIALOG };
	CLabel	m_QVCEFS_FILE;
	CButton	m_config;
	CListBox	m_LISTBOX;
	CLabel	m_TITLE;
	CLabel	m_PCNAME;
	CLabel	m_OEMSBL;
	CLabel	m_TOTAL;
	CLabel	m_AMSSH;
	CLabel	m_SW;
	CLabel	m_SO;
	CLabel	m_QCSBLH;
	CLabel	m_QCSBL;
	CLabel	m_PASS;
	CLabel	m_PARTITION;
	CLabel	m_OEMSBLH;
	CLabel	m_HW;
	CLabel	m_HEX;
	CLabel	m_FAIL;
	CLabel	m_AMSS;
	CLabel	m_QVCEFS_TIME;
	CLabel	m_QVCEFS_SIZE;
	CLabel        m_Static[21];
	CLabel        m_LabelPROGRESS[8];
	CLabel        m_LabelCOM[8];
	CProgressCtrl m_SUBProgressCtrl[8];
	CProgressCtrl m_MAINProgressCtrl[8];
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQ_DOWNLOADDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

protected:
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CQ_DOWNLOADDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//afx_msg LRESULT OnTerminateThread(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT ThreadSafe_OnUPDATEPROGRESS(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT ThreadSafe_OnQUERYIMEISTATUS(WPARAM wParam, LPARAM lParam);
	afx_msg void OnButtonStart();
	afx_msg void OnButtonExit();
	afx_msg void OnButtonReset();
	afx_msg void OnTest();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnConfig();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	SetupInfoStruct m_SetupInfo;
private:
	//bool ThreadSafe_Signed_CheckVersion(int ArrayNo,CQ_DOWNLOADDlg* pDownload);
private:
	CCriticalSection m_AddMsgCritSection;
	CCriticalSection m_ModifyUnitStatusCritSection;
	CCriticalSection m_DeleteUnitCritSection;
	CCriticalSection m_SetCountCritSection;

	//setting
	int m_nTotalNo;
	int m_nPassNo;
	int m_nFailNo;
	void AddTotalCount(bool bPASS, const char* szErrorCode);
	void SetCount(int nTotal, int nPassNo, int nFailNo);
	//int m_tCycle;
	int m_nCycTimes;
	//int m_nTestTime;
	//    CPowerSupply PowerSupply_Handle;
	
	
	bool m_bSkipComputerName;
	CStringA m_strCOMPUTER_NAME;

	tagDLSetting6245 m_DL6245Setting;
	tagDLSetting6240 m_DL6240Setting;
	bool m_bStop;
	bool m_bStartON; // true= disable button, false=can click button
	struct HandsetInfo
	{
		int nSlot;
		int nPort;
		int nCOMPort;
		bool bRunning;
	};
	
	int m_TempCycle[g_nSlotTotalCount];
	int m_TempSlot[g_nSlotTotalCount]; 
	HandsetInfo m_vRunningUnit[g_nSlotTotalCount];
	bool ModifyUnitStatus(int nSlot, int nPort, bool bRunning);
	bool ISUnitProcessing(int nSlot, int nPort, bool& bRunning);
	bool GetSlotByPort(int nPort, int& nSlot);
	bool GetPortBySlot(int nSlot, int& nPort);
	bool IsAnyUnitProcessing();
	bool SetStopRunningFlag(int nSlot, int nPort);
	
public:
	bool ThreadSafe_DownloadDefaultQCN(int ArrayNo,CQ_DOWNLOADDlg* pDownload);
	void FStrCpy(CString filename, char * filepath);
	void GetFileSizeTime(const char *FileName,char *Date, char *Size);
	void SetTestRecord(unsigned char Pass_Fail_Flag,CString ErrorCode);
public:
	void RunProcess(int nSlot, long nPort);
	static UINT AfxRun6245(LPVOID pParam);
	static UINT AfxRun6240(LPVOID pParam);
//	static UINT AfxRunMotoProcess(LPVOID pParam);
	void AddMessage(const char* szMsg);
	void SetProgress(int nSlot, int nMAINProgress, int nSUBProgress);
	
	virtual void Event(const std::string& strEvent,long nParam);

	bool UI_Setting();
	void UI_SetStart(BOOL bON/*click=ON*/);
	void ResetUIStatus(int nSlot);
	void Query_SettingParameters(CString SetupFilePath);
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Q_DOWNLOADDLG_H__788CCB34_C02B_4BEF_B0E4_ADBD1BAEA588__INCLUDED_)
