// BackupRestoreBRTDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BackupRestoreBRT.h"
#include "BackupRestoreBRTDlg.h"
#include "../FactoryTool/MobileToolKit/MobileToolKit.h"
#include "..\STE\MiddleWare\CallbackTypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBackupRestoreBRTDlg dialog




CBackupRestoreBRTDlg::CBackupRestoreBRTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBackupRestoreBRTDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBackupRestoreBRTDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT1, m_editFileName);
    DDX_Control(pDX, IDC_EDIT2, m_editCSVFileName);
    DDX_Control(pDX, IDC_EDIT3, m_editPort);
    DDX_Control(pDX, IDC_STATIC_RUNNING, m_staticRunning);
}

BEGIN_MESSAGE_MAP(CBackupRestoreBRTDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_RESTORE_BRT, &CBackupRestoreBRTDlg::OnBnClickedRestoreBrt)
    ON_BN_CLICKED(IDCANCEL, &CBackupRestoreBRTDlg::OnBnClickedCancel)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CBackupRestoreBRTDlg message handlers

BOOL CBackupRestoreBRTDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    m_editFileName.SetBackColor(0xFFFFFF);
    m_editFileName.SetTextColor(0x000000);
    m_editCSVFileName.SetBackColor(0xFFFFFF);
    m_editCSVFileName.SetTextColor(0x000000);
    m_editPort.SetBackColor(0xFFFFFF);
    m_editPort.SetTextColor(0x000000);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBackupRestoreBRTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBackupRestoreBRTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CBackupRestoreBRTDlg::GetPort() {
    unsigned short nPorts = 20;
    unsigned short Ports[20] = {0};
    if(QL_GetDiagPort(&nPorts, Ports) && nPorts > 0) {
        for(int i = 0; i < nPorts; ++i) {
            m_vecPortList.push_back(Ports[i]);
        }
    }
    ASSERT(m_vecPortList.size() != 0);
}

UINT CBackupRestoreBRTDlg::Restorethread(LPVOID lparam) {
    CBackupRestoreBRTDlg* pThis = (CBackupRestoreBRTDlg*)lparam;
    pThis->m_bRestored = RestoreBRT2NV(pThis->m_vecPortList.at(0), (pThis->m_strBackupFileName).GetString());
    return 0;
}

//void CBackupRestoreBRTDlg::RestoreOriginalBRT()
//{
//    GetPort();
//    if(m_vecPortList.size() == 0) {
//        AfxMessageBox(_T("Please connect the handset..."));
//        return;
//    }
//    CFileDialog fd(TRUE, _T("brt"), _T("*.brt"), 4|2, _T("*.brt||"));
//    INT_PTR nRet = fd.DoModal();
//    if(nRet == 1) {
//        m_strBackupFileName = fd.GetPathName();
//        m_strRestorePath = m_strBackupFileName.Mid(0, m_strBackupFileName.ReverseFind('\\'));
//        m_editFileName.SetWindowText(m_strBackupFileName.Mid(m_strBackupFileName.ReverseFind('\\')+1));
//    } else if(nRet == 2) {
//        return;
//    }
//    //TCHAR szCurrentDir[512] = {0};
//    //GetCurrentDirectory(512, szCurrentDir);
//    //m_strBackupFileName = CString(szCurrentDir) + CString(_T("\\WellFleet_ATHB1A _PCR1_QV002.brt"));
//    CString strPort;
//    strPort.Format(_T("COM%d"), m_vecPortList.at(0));
//    m_editPort.SetWindowText(strPort);
//    SetTimer(0, 500, NULL);
//    CWinThread* pThreadRestoreBRT = AfxBeginThread(Restorethread, (LPVOID)this);
//    ThreadWait(pThreadRestoreBRT);
//}
//
//void CBackupRestoreBRTDlg::RestoreBRTWithRFValue()
//{
//    CButton* radioBtnMelb1a = (CButton*)GetDlgItem(IDC_RADIO1);
//    CButton* radioBtnMelb2a = (CButton*)GetDlgItem(IDC_RADIO2);
//    if(radioBtnMelb1a->GetState() != 0) {
//        m_strBackupFileName = m_strRestorePath + CString(_T("\\MELB1A_Online.brt"));
//    } else if(radioBtnMelb2a->GetState() != 0) {
//        m_strBackupFileName = m_strRestorePath + CString(_T("\\MELB2A_Online.brt"));
//    }
//    CFile fTest;
//    BOOL bOpened = fTest.Open(m_strBackupFileName.GetString(), CFile::modeRead);
//    if(bOpened == FALSE) {
//        AfxMessageBox(_T("There is no RF Brt file.\n Please put the file along with original brt."));
//        return;
//    }
//    CWinThread* pThreadRestoreRFBrt = AfxBeginThread(Restorethread, (LPVOID)this);
//    ThreadWait(pThreadRestoreRFBrt);
//}

void CBackupRestoreBRTDlg::OnBnClickedRestoreBrt()
{
    //CButton* radioBtnMelb1a = (CButton*)GetDlgItem(IDC_RADIO1);
    //CButton* radioBtnMelb2a = (CButton*)GetDlgItem(IDC_RADIO2);
    //if(radioBtnMelb1a->GetState() == 0 && radioBtnMelb2a->GetState() == 0) {
    //    AfxMessageBox(_T("Please select a restoring version to download...\nMELB1A or MELB2A..."));
    //    return;
    //}
    GetPort();
    if(m_vecPortList.size() == 0) {
        AfxMessageBox(_T("Please connect the handset..."));
        return;
    }
    CFileDialog fd(TRUE, _T("brt"), _T("*.brt"), 4|2, _T("*.brt||"));
    INT_PTR nRet = fd.DoModal();
    if(nRet == 1) {
        m_strBackupFileName = fd.GetPathName();
        m_strRestorePath = m_strBackupFileName.Mid(0, m_strBackupFileName.ReverseFind('\\'));
        m_editFileName.SetWindowText(m_strBackupFileName.Mid(m_strBackupFileName.ReverseFind('\\')+1));
    } else if(nRet == 2) {
        return;
    }
    //TCHAR szCurrentDir[512] = {0};
    //GetCurrentDirectory(512, szCurrentDir);
    //m_strBackupFileName = CString(szCurrentDir) + CString(_T("\\WellFleet_ATHB1A _PCR1_QV002.brt"));
    CString strPort;
    strPort.Format(_T("COM%d"), m_vecPortList.at(0));
    m_editPort.SetWindowText(strPort);
    SetTimer(0, 500, NULL);
    CWinThread* pThreadRestoreBRT = AfxBeginThread(Restorethread, (LPVOID)this);
    ThreadWait(pThreadRestoreBRT);


    //if(radioBtnMelb1a->GetState() != 0) {
    //    m_strBackupFileName = m_strRestorePath + CString(_T("\\MELB1A_Online.brt"));
    //} else if(radioBtnMelb2a->GetState() != 0) {
    //    m_strBackupFileName = m_strRestorePath + CString(_T("\\MELB2A_Online.brt"));
    //}
    //CFile fTest;
    //BOOL bOpened = fTest.Open(m_strBackupFileName.GetString(), CFile::modeRead);
    //fTest.Close();
    //if(bOpened == FALSE) {
    //    AfxMessageBox(_T("There is no RF Brt file.\n Please put the file along with original brt."));
    //    return;
    //}

    //CWinThread* pThreadRestoreRFBrt = AfxBeginThread(Restorethread, (LPVOID)this);
    //ThreadWait(pThreadRestoreRFBrt);
    KillTimer(0);
    std::tstring strOutput;
    ASSERT(m_bRestored);
    if(m_bRestored) {
        m_staticRunning.SetWindowText(_T("Restoer complete..."));
        OutputPrompt(_T("Retore BRT Dialog"), _T("Restoer complete..."), _T("OK"), strOutput);
    } else {
        m_staticRunning.SetWindowText(_T("Restoer failed..."));
        OutputPrompt(_T("Retore BRT Dialog"), _T("Restoer failed..."), _T("OK"), strOutput);
    }
    ChangeFtmAndPowerOff();
}

void CBackupRestoreBRTDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    OnCancel();
}

void CBackupRestoreBRTDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    CDialog::OnTimer(nIDEvent);
    static int i = 0;
    if(i == 0) {
        m_staticRunning.SetWindowText(_T("Restoring."));
    } else if(i == 1) {
        m_staticRunning.SetWindowText(_T("Restoring.."));
    } else if(i == 2) {
        m_staticRunning.SetWindowText(_T("Restoring..."));
    }
    i = ++i % 3;
}

void CBackupRestoreBRTDlg::ChangeFtmAndPowerOff()
{
    // Change handset to FTM, Write NV method then reset handset
    ChangeFTM(m_vecPortList.at(0));
    PowerOff(m_vecPortList.at(0));
}

void CBackupRestoreBRTDlg::ChangeOnlineAndPowerOff()
{
    // Change handset to Online, Write NV method then reset handset
    ChangeOnline(m_vecPortList.at(0));
    PowerOff(m_vecPortList.at(0));
}
