#if !defined(AFX_CONFIG_H__08C531AF_7941_455C_A5A3_561DC3D05D17__INCLUDED_)
#define AFX_CONFIG_H__08C531AF_7941_455C_A5A3_561DC3D05D17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Config.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfig dialog
class CConfig : public CDialog
{
// Construction
public:
	CConfig(CWnd* pParent = NULL);   // standard constructor
	INT_PTR DoModal(const char* szINIPath)
	{
		m_strINIPath = szINIPath;
		INT_PTR nRet = CDialog::DoModal();
		return nRet;
	}
// Dialog Data
	//{{AFX_DATA(CConfig)
	enum { IDD = IDD_CONFIG };
	CButton	m_DL_QVCEFS_Flag;
	CEdit	m_Restart_Timeout;
	CEdit	m_Waite_PowerOn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	int m_nOnly_DL_Qvcefs_Flag;
	int m_nPowerOnTimeout;
	int m_nRestartTimeout;
	std::string m_strINIPath;
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfig)
	afx_msg void OnSave();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIG_H__08C531AF_7941_455C_A5A3_561DC3D05D17__INCLUDED_)
