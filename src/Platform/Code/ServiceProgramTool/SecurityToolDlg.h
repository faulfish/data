// SecurityToolDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "ServiceProgramTool.h"
#include <string>

using namespace std;

#define TB_DEF_COMBO_WIDTH 90

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

private:
	bool m_Check;
	int m_iComPort;
	long GetComPort();
	long m_FirmCode;
	long m_UserCode;
	MOBILEMODEL m_MobileModel;

	bool KeyCheck();
	bool IsDongleCodeCorrect();

	bool ReadImeiFsn(const int portNum);

	bool UnLockSimData();
	bool WriteSimData();

	void UpdateTitleBarText();
	void GetMobileModel();

// Implementation
protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	// Generated message map functions

	DECLARE_MESSAGE_MAP()
public:	
	CComboBox m_ComPort;
	afx_msg void OnBnClickedBtnOpenLockfile();
	afx_msg void OnBnClickedBtnWiteSimlock();
	afx_msg void OnBnClickedBtnWiteSpc();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_sOldNWSCP;
	CString m_sOldSSCP;
	CString m_sNewNWSCP;
	CString m_sNewSSCP;
	CString m_strIMEI;
	CString m_strFSN;
	CString m_strLockFilePath;
	CString m_strOldSPC;
	CString m_strNewSPC;
};
