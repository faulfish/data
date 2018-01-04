// DLTesterDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include <string>
#include <vector>

#include "..\..\Modules\QisdaQualcommDownloadModule\Download.h"
#include "..\..\Modules\QisdaQualcommDownloadModule\QisdaQualcommDownloadModule.h"
#include "..\..\CommonUtil\PIMStringUtility.h"
#include "..\..\CommonUtil\UnicodeUtility.h"
#include "..\..\CommonUtil\Buffer.h"
using namespace std;

// CDLTesterDlg dialog
class CDLTesterDlg : public CDialog
{
// Construction
public:
	CDLTesterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DLTESTER_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
public:
	CEdit		m_EditIni;
	CListBox	m_ListBoxReult;
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnDumpImage();
	afx_msg void OnBnClickedBtnGetIniPath();
	afx_msg void OnBnClickedBtnDlDumpimage();
	afx_msg void OnBnClickedBtnSaveimage();
	afx_msg void OnBnClickedBtnClearboard();
private:
	std::string m_strIniPath;
	int			m_nPlatform;
	CBufferPtr m_ptBuffer;
	//DumpImage & DLDumpImage
	std::string m_strHexPath;
	//DumpImage
	std::string m_strRawRequest;
	int			m_nRawRequestTimeOut;
	//DL DumpImage
	std::string m_strPartition;
	int			m_nTimeOut;
	std::string m_strCefs;
	DWORD		m_dwStartAddress;

public:
	bool GetIniInfo(const std::string& strTag);
	bool GetComPort(int& nComPort);
	void AddMessage(LPCTSTR lpMsg);
	bool SetValueFromIni(const std::string& strTag, const std::string& strKey, std::string& strValue);
	bool SetValueFromIni(const std::string& strTag, const std::string& strKey, int& nValue);
	bool SetValueFromIni(const std::string& strTag, const std::string& strKey, DWORD& nValue);
	
	static UINT DLDumpImageThread(LPVOID pParam);
	static UINT DumpImageThread(LPVOID pParam);
};
