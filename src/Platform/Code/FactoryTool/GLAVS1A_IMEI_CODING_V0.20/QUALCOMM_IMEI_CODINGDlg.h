// QUALCOMM_IMEI_CODINGDlg.h : header file
//

#if !defined(AFX_QUALCOMM_IMEI_CODINGDLG_H__2635CA94_C71C_44C7_A130_B600B6AB6CE5__INCLUDED_)
#define AFX_QUALCOMM_IMEI_CODINGDLG_H__2635CA94_C71C_44C7_A130_B600B6AB6CE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Label.h"
#include "SxGroupBox.h"
#include "ColoredCombo.h"
#include "SSSO2.h"
#include "PicsCtrl.h"
#include "Message.h"
#include "ColoredStatusBarCtrl.h"
#include "ColoredStatusBarCtrlCtrl.h"
#include <string>

#include "..\spu\ISPU_CALLBACK.h"


#include "excel.h"  //for use EXCEL
#include "comdef.h"  //for use VARIANT

/////////////////////////////////////////////////////////////////////////////
// CQUALCOMM_IMEI_CODINGDlg dialog

class CQUALCOMM_IMEI_CODINGDlg : public CDialog,public ISPU_CALLBACK
{
// Construction
public:
	bool WaitInput(const TSTRING& strName,TCHAR* szBuffer,int nBufferSize);
	bool Print(const TSTRING& strName,const TSTRING& strOutput);
	bool CheckBreak();
public:
	bool Initial_IMEIStationDLL();
	bool GetEXCEL();
	void ShowPicassoTrans(void);
	void SetTestRecord(CString ErrorCode);
	void SetStatusBar();
	unsigned char WritePICSFile(void);
	void SetLog(void);
	CWinThread *ThreadHandle;
	double TimeMeter(int nIndex, bool bStart);
	void EndReset(unsigned char ShowMessageDlg_Flag);
	void EnableDisableButton(unsigned char Enable_Disable_Flag);
	void ResetEditContent(void);
	void ShowMessageDlg(void);
	void Initial(void);
	void SetReady(void);
	unsigned char CheckPhoneLockStatus(void);
	unsigned char ConnectServer(void);
	unsigned char TestSrc();//for test
	virtual ~CQUALCOMM_IMEI_CODINGDlg();
	CQUALCOMM_IMEI_CODINGDlg(CWnd* pParent = NULL);	// standard constructor
    CBrush m_brush;
	CString GetFileTime(CString FileName);
	CString GetFileSize(CString FileName);
	CString GetComputerName(void);
	CFont   font_LOCK;
	CSSSO2 SSSO2_Ctrl;
	CPicsCtrl Pics_Interface_Ctrl;
//  CStatusBarCtrl               *m_StatusBar;//for Method 2
//  CColoredStatusBarCtrlCtrl      m_StatusBar;//for Method 2,new
//  CStatusBar                   m_StatusBar;//for Method 3
	CColoredStatusBarCtrl        m_StatusBar;//for Method 3,new
	CFont   StatusFont;
	void Set_Focus(void);
	void GetLockIndicator(void);
	void Initial_Parameter(void);
	CString Set_Title(void);
	void Set_Ctrl_Text(void);
	void Set_Ctrl_Property(void);


	_Application exlapp;   //组件服务器的各个classes
	_Workbook wbk;
	Workbooks wbks;
	_Worksheet wht;
	Worksheets whts;
	Range range,usedRange;
	LPDISPATCH lpDisp;

	HMODULE hDll,hDll2,hDll3;
// Dialog Data
	//{{AFX_DATA(CQUALCOMM_IMEI_CODINGDlg)
	enum { IDD = IDD_QUALCOMM_IMEI_CODING_DIALOG };
	CEdit	m_SO_EDIT;
	CLabel	m_SO;
	CLabel	m_TRANS2;
	CLabel	m_TRANS1;
	CLabel	m_PICASSO;
	CEdit	m_PICASSO_EDIT;
	CLabel	m_TIME_STAMP_DUCK2;
	CLabel	m_TIME_STAMP_DUCK1;
	CLabel	m_KB_DUCK;
	CLabel	m_FILE_SIZE_DUCK2;
	CLabel	m_FILE_SIZE_DUCK1;
	CLabel	m_DUCK2;
	CLabel	m_DUCK1;
	CButton	m_CONFIG;
	CButton	m_CHANGE;
	CSxGroupBox	m_IMEI_MAL;
	CLabel	m_IMEI_PART;
	CLabel	m_MAL_PART;
	CEdit	m_MAL_PART_EDIT;
	CEdit	m_IMEI_PART_EDIT;
	CSxGroupBox	m_LOCK;
	CButton	m_SIM;
	CButton	m_USER_SIM;
	CButton	m_SERVICE_PROVIDER;
	CButton	m_NETWORK_SUBSET;
	CButton	m_NETWORK;
	CButton	m_CORPORATE;
	CLabel	m_MAL;
	CEdit	m_MAL_EDIT;
	CLabel	m_MOBILEID2;
	CLabel	m_MOBILEID1;
	CLabel	m_KB_CSV;
	CButton	m_START;
	CButton m_LOGBUTTON;
	CButton	m_EXIT;
	CListBox	m_LIST;
	CProgressCtrl	m_PROGRESS;
	CLabel	m_BACKUP_TYPE2;
	CLabel	m_BACKUP_TYPE1;
	CLabel	m_CSV2;
	CLabel	m_CSV1;
	CLabel	m_QZCS2;
	CLabel	m_QZCS1;
	CLabel	m_QVCEFS2;
	CLabel	m_QVCEFS1;
	CLabel	m_QSWINFO2;
	CSxGroupBox	m_QSWINFO_SIMDATA;
	CLabel	m_QSWINFO1;
	CLabel	m_QSNV2;
	CLabel	m_QSNV1;
	CLabel	m_PROGRESS_STATIC;
	CSxGroupBox	m_PPF;
	CLabel	m_PN2;
	CLabel	m_PN1;
	CLabel	m_PERCENT;
	CLabel	m_PC2;
	CLabel	m_PC1;
	CLabel	m_MODE2;
	CLabel	m_MODE1;
	CLabel	m_MECHANICAL2;
	CLabel	m_MECHANICAL1;
	CLabel	m_KB_SIMDATA;
	CLabel	m_KB_QZCS;
	CLabel	m_KB_QVCEFS;
	CLabel	m_KB_QSWINFO;
	CLabel	m_KB_QSNV;
	CSxGroupBox	m_COM;
	CLabel	m_SIMDATA1;
	CLabel	m_SIMDATA2;
	CSxGroupBox	m_SW_HW_ME;
	CLabel	m_SW1;
	CLabel	m_SW2;
	CSxGroupBox	m_TARGET_MODE_PN;
	CLabel	m_TARGET2;
	CLabel	m_TARGET1;
	CLabel	m_TIME_STAMP_SIMDATA2;
	CLabel	m_TIME_STAMP_SIMDATA1;
	CLabel	m_TIME_STAMP_QZCS2;
	CLabel	m_TIME_STAMP_QZCS1;
	CLabel	m_TIME_STAMP_QVCEFS2;
	CLabel	m_TIME_STAMP_QVCEFS1;
	CLabel	m_TIME_STAMP_QSWINFO2;
	CLabel	m_TIME_STAMP_QSWINFO1;
	CLabel	m_TIME_STAMP_QSNV2;
	CLabel	m_TIME_STAMP_QSNV1;
	CLabel	m_TIME_STAMP_CSV2;
	CLabel	m_TIME_STAMP_CSV1;
	CSxGroupBox	m_TIME_ELAPSE;
	CLabel	m_TIME1;
	CLabel	m_TIME2;
	CLabel	m_TIME3;
	CLabel	m_TITLE;
	CEdit	m_IMEI_EDIT;
	CLabel	m_IMEI;
	CLabel	m_HW2;
	CLabel	m_HW1;
	CLabel	m_FILE_SIZE_SIMDATA2;
	CLabel	m_FILE_SIZE_SIMDATA1;
	CLabel	m_FILE_SIZE_QZCS2;
	CLabel	m_FILE_SIZE_QZCS1;
	CLabel	m_FILE_SIZE_QVCEFS2;
	CLabel	m_FILE_SIZE_QVCEFS1;
	CLabel	m_FILE_SIZE_QSWINFO2;
	CLabel	m_FILE_SIZE_QSWINFO1;
	CLabel	m_FILE_SIZE_QSNV2;
	CLabel	m_FILE_SIZE_QSNV1;
	CLabel	m_FILE_SIZE_CSV2;
	CLabel	m_FILE_SIZE_CSV1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQUALCOMM_IMEI_CODINGDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CQUALCOMM_IMEI_CODINGDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonChange();
	afx_msg void OnButtonExit2();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonConfig();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnTerminateThread(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnResetTestCount(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

////for script
public:
	SPU_RESULT ExecuteSPU(const TSTRING& szScript);
private:
	///// WRITE IMEI
	bool CheckMAL();
	unsigned char Check_MAL_FirstPart(CString MAL_String);
	unsigned char CheckMALLength(CString MAL_String);
	bool CheckPICASSO();
	bool CheckIMEIScanner();
	unsigned char CheckIMEIValidity(CString IMEI_String);
	unsigned char CheckIMEILength(CString IMEI_String);
	unsigned char Check_IMEI_FirstPart(CString IMEI_String);
	bool IMEIUpdateMobileID(const char *szFactoryArea);
	bool IMEICheckIMEIIsValid(const char* szIMEI);
	unsigned char CheckIMEIZeroDummyEmpty(char IMEI_Array[16]);
	unsigned char CheckIMEIDifference(CString IMEI_String);
	bool IMEICheckPPFFILE(const char *szFactoryArea);

	bool QueryIMEI_CHECK_LEN();
	bool SetAfxProgressStatus(const char *pStatus);
	bool BackUpBRTPre();
	bool BackUpBRTLast(CString szBRTPath);
	bool DealFsnFromPICS(const char *szDevSerialNo);
	bool WriteDebugLog();
	bool SavePICSFile();
	bool DealSIMDataFromPICS();
	bool CheckMEVersion();
	bool CheckSWVersion();
	bool CheckHWVersion();
	bool CheckProcess();
	bool QueryIMEICheck(const char * Status_Output);
public:
	bool DealIMEICodingEnd();
	void InsertStringIntoList(CString szStrng);
public:
	int mFlag;
	double Time_Elapse;
	CString m_szIMEIType;

	bool m_bTranciver;
	bool m_bBRTBackup1;
	bool m_bBRTBackup2;
	CString m_szBRTBackup1Path;
	CString m_szStartTime;

public:
	afx_msg void OnBnClickedButtonLog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUALCOMM_IMEI_CODINGDLG_H__2635CA94_C71C_44C7_A130_B600B6AB6CE5__INCLUDED_)
