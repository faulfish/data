// QFuseToolDlg.h : header file
//

#pragma once

#include <map>
#include "../QLib_Export/QLib_export.h"
#include "Resource.h"
#include <math.h>
using namespace std;

// CQFuseToolDlg dialog
class CQFuseToolDlg : public CDialog
{
// Construction
public:
	CQFuseToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_QFUSETOOL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBnClickedGetPort();
    afx_msg void OnBnClickedMsm6ReadQfuse();
    afx_msg void OnBnClickedMsm6WriteQfuse();
    afx_msg void OnBnClickedMsm7ReadQfuse();
    afx_msg void OnBnClickedMsm7WriteQfuse();
    afx_msg void OnCbnSelchangeCombo1();
private:
    void DeleteAllComportList();
    void AddPortToCombo(int nPorts,unsigned short* Ports);
    unsigned long ConvertStringToHex(CString strHex);  //ゅ计锣计, 计, 肚unsigned long
    void CheckWritedParameters(); // 浪琩糶㎝弄琌妓, 糶把计edit
private:
    CString m_strSelectedPort;
    map<CString, int> m_mapPortList;
public:
    afx_msg void OnBnClickedChangeFtm();
    afx_msg void OnBnClickedChangeOnline();
};
