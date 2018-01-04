// SPU_UIDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "../../FactoryTool/spu/SPUExport.h"
#include <Gdiplus.h>
#include "files_bz/handler_bz.h"
#include <vector>
#include <afxmt.h>

// CInputDlg dialog

class CInputDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputDlg)

public:
	CInputDlg(const CString& strPrompt,CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputDlg();
	virtual BOOL OnInitDialog();

	// Dialog Data
	enum { IDD = IDD_INPUT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_szInput;
	CString m_strPrompt;
public:
	afx_msg void OnBnClickedOk();
};


class UI_CallbackClass : public ISPU_CALLBACK
{
	friend class CSPU_UIDlg;
public:
	UI_CallbackClass(CSPU_UIDlg* pCSPU_UIDlg)
		:m_pCSPU_UIDlg(pCSPU_UIDlg)
		,m_bBreak(false)
	{}
	virtual ~UI_CallbackClass(void)	
	{}

	//interface of ISPU_CALLBACK
	virtual bool CheckBreak();
	virtual bool WaitInput(const TSTRING& strName,TCHAR* szBuffer,int nBufferSize);
	virtual bool Print(const TSTRING& strName,const TSTRING& strOutput);	

protected:
	void	SetBreak();
	void	ResetBreak();

private:
	CSPU_UIDlg* m_pCSPU_UIDlg;
	bool	m_bBreak;
};

// CSPU_UIDlg dialog
class CSPU_UIDlg : public CDialog
{
	friend class UI_CallbackClass;
// Construction
public:
	CSPU_UIDlg(LPCTSTR cScript);

// Dialog Data
	enum { IDD = IDD_SPU_UI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
//	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnClose( );
	afx_msg BOOL OnEraseBkgnd( CDC * );
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CString m_szScript;
	CListBox m_lbNote;
	UINT	m_unUIType;
	clock_t m_start;
	int m_nTotalTime;
	CString m_strDescription;
	CString m_strTitle;
	CString m_strUIType;
	CString m_strCOMPort;
	CString m_strImagePath;
	Gdiplus::Image* m_pImage;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	CCriticalSection	m_LockObj;
	CCriticalSection	m_WaitingObj;
	std::vector<std::string> m_AppendImages;
public:
	UI_CallbackClass* m_pCallBack;
	HANDLE	m_hSPULoader;
	CWinThread *m_pExecuteThread ;

public:
	int	m_nTotal;
	int	m_nPass;
	int	m_nFail;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonClear();
public:
	void Parser(const TSTRING& strTemp);
public:
	void UpdateExecuteResult(SPU_RESULT nResult);
protected:
	void LoadScriptFile(const CString& szFilePath);	
	void ShowMessage(const CString& strMessage);
	void SetValue(UINT nID,int nValue);
	void SetValue(UINT nID,double nValue);
	void SetValue(UINT nID,const CString& strValue);
	CString GetPort(const CString& strPort);
	void SetCOMPort(const CString& strCOMPort);
	void SetDisplayImage(const CString& strDisplayImage);
	void AppendDisplayImage(const CString& strDisplayImage);
	bool IsQisdaTaipei();
public:
	afx_msg void OnBnClickedLogButton();
	afx_msg void OnCbnSelchangeMotname();
	afx_msg void OnCbnSelchangeCombomotmodel();
	afx_msg void OnBnClickedButton1();
};
