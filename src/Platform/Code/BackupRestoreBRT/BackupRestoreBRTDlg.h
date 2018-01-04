// BackupRestoreBRTDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "ReadOnlyEdit.h"
#include <vector>
#include <string>
#include "../QLib_Export/QLib_export.h"
#include "afxwin.h"


using namespace std;

typedef void (*pFnCallBack)(CString strMessage, int nPercentage);

// CBackupRestoreBRTDlg dialog
class CBackupRestoreBRTDlg : public CDialog
{
// Construction
public:
	CBackupRestoreBRTDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_BACKUPRESTOREBRT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    static UINT Restorethread(LPVOID lparam);
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBnClickedRestoreBrt();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

public:
    void GetPort();
    void ChangeFtmAndPowerOff();
    void ChangeOnlineAndPowerOff();

private:
    //void RestoreBRTWithRFValue();
    //void RestoreOriginalBRT();
    void ThreadWait( CWinThread* pThread )
    {
        DWORD result;
        while(true) {
            MSG msg;
            while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                if(msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST) 
                    continue;
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
            result = ::MsgWaitForMultipleObjects(1, &(pThread->m_hThread), FALSE, INFINITE, QS_ALLINPUT);
            if(result == (WAIT_OBJECT_0 + 1))
                continue;
            else
                break;
        }
    }

public:
    CReadOnlyEdit m_editFileName;
    CReadOnlyEdit m_editCSVFileName;
    CReadOnlyEdit m_editPort;
    vector<int> m_vecPortList;
    CStatic m_staticRunning;
    CString m_strBackupFileName;
    CString m_strRestorePath;
    bool m_bRestored;
};
