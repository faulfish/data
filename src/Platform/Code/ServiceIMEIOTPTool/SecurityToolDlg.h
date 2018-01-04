// SecurityToolDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "ServiceProgramTool.h"
#include <string>
#include "Shlwapi.h"

using namespace std;

#define TB_DEF_COMBO_WIDTH 90
#define STEP_COUNT 5

typedef bool (__cdecl *pOutputPrompt)(char*, char*, char*, string&);

// CSecurityToolDlg dialog
class CSecurityToolDlg : public CDialog
{
	DECLARE_DYNAMIC(CSecurityToolDlg)
// Construction
public:	
	CSecurityToolDlg(CWnd* pParent = NULL);	// standard constructor

	void SetCode(long firmCode, long userCode);

// Dialog Data
	enum { IDD = IDD_SECURITYTOOL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:	
	// Main Operations
	BOOL OSP_ChangeToFTM();
	BOOL OSP_RestoreBRT();	 
	BOOL OSP_QFuse();		 
	BOOL OSP_IMEI();		 
	BOOL OSP_OTP();
	BOOL OSP_ChangeToOnline();

	// IMEI related 
	BOOL GetIMEI();
	BOOL SetIMEI();
	BOOL GetFSN();
	BOOL SetFSN();
	BOOL GetBTAddr();
	BOOL SetBTAddr();

	//
	CString GetBRTFilePath();

	UINT m_nComPort;
	BOOL m_Check;	// to check if WibuKey is inserted or not
	long m_FirmCode;
	long m_UserCode;
	HANDLE m_hThread;

	// WibuKey Protection
	BOOL KeyCheck();
	BOOL IsDongleCodeCorrect();

	// Others
	long GetComPort();	// find COM port number phone connected to
	void RandomGenFSN();
	BOOL CheckIMEIValid(LPCTSTR pIMEI);
	BOOL CheckBTAddrValid(LPCTSTR pBT);

	// UI
	void EnableButton(BOOL bEnable);
	void EnableAllItems(BOOL bEnable);
	void EnableItem(UINT nStepItem, BOOL bEnable);	

	static UINT OneStopProgramming(LPVOID param);

private:
	BOOL m_bStepCheck[STEP_COUNT];
	Bitmap	*m_bmpCheck;
	UINT m_nTimer;
	UINT m_nTimeCounter;

// Implementation
protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	// Generated message map functions

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComPort;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnWorkThreadFinish(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnWriteImeiFsn();	
	CString m_strIMEI;
	CString m_strFSN;
	CString m_strBTAddr;	
	afx_msg void OnClose();
	afx_msg void OnPaint();
};
