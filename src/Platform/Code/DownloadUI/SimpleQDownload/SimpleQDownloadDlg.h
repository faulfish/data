// SimpleQDownloadDlg.h : header file
//

#ifndef _SIMPLE_QDOWNLOAD_DLG_H_
#define _SIMPLE_QDOWNLOAD_DLG_H_

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <map>
#include "..\..\Components\MobileFinder\MobileFinderInterface.h"

using namespace QSYNC_LIB;
using namespace std;
// CSimpleQDownloadDlg dialog
class CSimpleQDownloadDlg : public CDialog, public INotify
{
// Construction
public:
	CSimpleQDownloadDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SIMPLEQDOWNLOAD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBnClickedBtnDownload();
    afx_msg void OnBnClickedBtnClose();
	DECLARE_MESSAGE_MAP()

public:
    CStatic m_staticPercentage;
    CProgressCtrl m_progressDownload;
    void Event(const TSTRING& strEvent,long nParam);
    static UINT DownloadThread(LPVOID lparam);
    void DownloadFunction();
    static bool CallBackFunction(const char* szMsg, int nStep, int nProgress);

private:
    void GetPort(const TSTRING& strPort, long& nPort);
    bool GetInformationFromHandset(CString& strSW, CString& strHW);
    bool CheckQFuse();
    bool CheckSignedCode(const TSTRING& strAmss, const TSTRING& strOemsbl);
    void SetIniPath();
    void SetButtonEnable(BOOL bEnable);
    void ShowUpgradeVersion();
    bool GetCodeVersion(const TSTRING& strFileName, char* szCheckCode, int nCheckLength, CString& strVer);
    bool GetSoftwareLoadName(const TSTRING& strFileName, char* szCheckCode, int nCheckLength, CString& strName);
    bool ExtractSpecifiedString(const TSTRING& strFileName, char* szCheckCode, int nCheckLength, int nExtractLength, int nIndexToGet, char* nOutput, int nOutputLength);
    void CheckHandsetAndSWVersion();

private:
    long m_nPort;
    CString m_strHandsetSWVersion;
    CString m_strLoadSWVersion;
    CString m_strIniPath;
    TSTRING m_strOemsbl;
    TSTRING m_strAmss;
    bool m_bDownloading;
    map<CString, CString> m_mapLoadInformation;
};


#endif