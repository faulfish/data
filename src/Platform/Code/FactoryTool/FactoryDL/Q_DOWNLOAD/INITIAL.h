#if !defined(AFX_INITIAL_H__F026F83A_EA14_4919_B444_8C3E1BC94CB1__INCLUDED_)
#define AFX_INITIAL_H__F026F83A_EA14_4919_B444_8C3E1BC94CB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// INITIAL.h : header file
//
#include <string>

//////////////////////////////////////////////////////////////////////////
typedef struct _SetupInfo
{
	int nPowerOnDelay;
	int nDEBUG_ONOFF_FLAG;
	int nPower_Supply_Control_Flag;
	
	//////////////////////////////////////////////////////////////////////////

	CStringA strSETUP_INI_FILE_PATH;
	CStringA strSW_Path;
	CStringA strCurrentDirectory;

	CStringA strSO;
	CStringA strMODEL_NAME;
	CStringA strSW_VERSION;
	CStringA strHW_VERSION;
	CStringA strMode_Name;
	bool	 bREWORK_FLAG;
	bool	 bCheckFactoryArea;
	CStringA strTool_Version;
	CStringA strConfig_Version;
	int nIMEI_CHECK;
	int nWriteQFuseFlag;
	int nOnly_DL_Qvcefs_Flag;
	int nDL_Bootloader_Flag;
	int nPowerOnTimeout;
	int nRestartTimeout;
	int nCreateLog;
	int nSaveLog;

	CStringA strPARTITION_FILE_NAME;
	CStringA strHEX_FILE_NAME;
	CStringA strAMSSH_FILE_NAME;
	CStringA strAMSS_FILE_NAME;
	CStringA strQVCEFS_FILE_NAME;
	CStringA strQVCEFS_FILE_TIME;
	CStringA strQVCEFS_FILE_SIZE;

	CStringA strOEMSBLH_FILE_NAME;
	CStringA strOEMSBL_FILE_NAME;
	CStringA strQCSBLH_FILE_NAME;
	CStringA strQCSBL_FILE_NAME;

	CStringA strFSBL_FILE_NAME;
	CStringA strDBL_FILE_NAME;
	CStringA strOSBL_FILE_NAME;
	CStringA strQCN_FILE_NAME;

	CStringA strBRT_BACKUP_PATH;
	bool	 bBackupBrt;
	CStringA strCSV_PATH;
	CStringA strCSV_TIME;
	CStringA strCSV_SIZE;
}SetupInfoStruct;
/////////////////////////////////////////////////////////////////////////////
// CINITIAL dialog

class CINITIAL : public CDialog
{
// Construction
public:
	void FileFind(CString szFile, int iCombo);
	CINITIAL(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CINITIAL)
	enum { IDD = IDD_INITIAL };
	CComboBox	m_COMBO_QVCEFS;
	CComboBox	m_COMBO_SW;
	CComboBox	m_COMBO_QCSBLH;
	CComboBox	m_COMBO_QCSBL;
	CComboBox	m_COMBO_PARTITION;
	CComboBox	m_COMBO_OEMSBLH;
	CComboBox	m_COMBO_OEMSBL;
	CComboBox	m_COMBO_MODE;
	CComboBox	m_COMBO_MODEL;
	CComboBox	m_COMBO_HW;
	CComboBox	m_COMBO_HEX;
	CComboBox	m_COMBO_AMSSH;
	CComboBox	m_COMBO_AMSS;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CINITIAL)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
public:
	INT_PTR DoModal(SetupInfoStruct* SetupInfo)
	{
		m_SetupInfo = SetupInfo;
		INT_PTR nRet = CDialog::DoModal();
		return nRet;
	}
	//}}AFX_VIRTUAL

// Implementation
private:
	SetupInfoStruct* m_SetupInfo;
protected:

	// Generated message map functions
	//{{AFX_MSG(CINITIAL)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboMode();
	afx_msg void OnSelchangeComboModel();
	afx_msg void OnSelchangeComboSw();
	virtual void OnOK();
	afx_msg void OnDropdownComboQvcefsFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INITIAL_H__F026F83A_EA14_4919_B444_8C3E1BC94CB1__INCLUDED_)
