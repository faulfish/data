// SimpleQDownloadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SimpleQDownload.h"
#include "SimpleQDownloadDlg.h"

#include "..\..\..\Code\odmtma_api\QDownload_Export\Download_Export.h"
#include "..\..\..\Code\Modules\QisdaMiscellaneousModule\QisdaMiscellaneousModule.h"
#include "..\..\..\Code\Modules\QisdaNVModule\NVItem.h"
#include "..\..\..\code\FactoryTool\MobileToolKit\MobileToolKit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSimpleQDownloadDlg dialog

CSimpleQDownloadDlg* g_pThis = NULL;


CSimpleQDownloadDlg::CSimpleQDownloadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimpleQDownloadDlg::IDD, pParent), m_nPort(0), m_bDownloading(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    g_pThis = this;
}

void CSimpleQDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_PERCENTAGE, m_staticPercentage);
    DDX_Control(pDX, IDC_PROGRESS_DOWNLOAD, m_progressDownload);
}

BEGIN_MESSAGE_MAP(CSimpleQDownloadDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(ID_BTN_DOWNLOAD, &CSimpleQDownloadDlg::OnBnClickedBtnDownload)
    ON_BN_CLICKED(ID_BTN_CLOSE, &CSimpleQDownloadDlg::OnBnClickedBtnClose)
END_MESSAGE_MAP()


// CSimpleQDownloadDlg message handlers

BOOL CSimpleQDownloadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, TRUE);		// Set small icon

	// TODO: Add extra initialization here
    SetIniPath();
    ShowUpgradeVersion();
    m_staticPercentage.SetWindowText(_T("0%"));
    CStatic* status = (CStatic*)GetDlgItem(IDC_STATIC_STATUS);
    CButton* btnDownload = (CButton*)GetDlgItem(ID_BTN_DOWNLOAD);
    btnDownload->EnableWindow(FALSE);
    CButton* btnClose = (CButton*)GetDlgItem(ID_BTN_CLOSE);
    btnClose->EnableWindow(FALSE);
    status->SetWindowText(_T("Disconnect..."));
    GetDeviceFinder()->Register(this);
    GetDeviceFinder()->StartMonitor();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSimpleQDownloadDlg::OnPaint()
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
HCURSOR CSimpleQDownloadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSimpleQDownloadDlg::ShowUpgradeVersion() {
    CString strCodeVersion, strLoadName;
    GetCodeVersion(m_strAmss, "O'zapft", 7, strCodeVersion);
    m_strLoadSWVersion = strCodeVersion;
    GetSoftwareLoadName(m_strAmss, "O'zapft", 7, strLoadName);
    CStatic* upgradeVersion = (CStatic*)GetDlgItem(IDC_STATIC_UPGRADE_VERSION);
    CString strShowString;
    strShowString.Format(_T("Software Version: %s"), strCodeVersion.GetString());
    upgradeVersion->SetWindowText(strShowString);
    CString tmp;
    tmp.Format(_T("%s SW Upgrade Tool"), strLoadName.GetString());
    this->SetWindowText(tmp);
}

void CSimpleQDownloadDlg::SetButtonEnable(BOOL bEnable) {
    CButton* btnDownload = (CButton*)GetDlgItem(ID_BTN_DOWNLOAD);
    btnDownload->EnableWindow(bEnable);
    CButton* btnClose = (CButton*)GetDlgItem(ID_BTN_CLOSE);
    btnClose->EnableWindow(bEnable);
}


void CSimpleQDownloadDlg::CheckHandsetAndSWVersion() {
    CString strSW, strHW;
    GetInformationFromHandset(strSW, strHW);
    CStatic* upgradeVersion = (CStatic*)GetDlgItem(IDC_STATIC_UPGRADE_VERSION);
    CString strTmp;
    strTmp.Format(_T("Upgrade form %s to %s"), strSW.GetString(), m_strLoadSWVersion.GetString());
    upgradeVersion->SetWindowText(strTmp);
    if(strSW.GetString()[0] != m_strLoadSWVersion.GetString()[0] && strSW.GetLength() != 0) {
        AfxMessageBox(_T("The software version does not match the handset.\nPlease download the correct version."));
        SetButtonEnable(FALSE);
    }
}

void CSimpleQDownloadDlg::Event(const TSTRING& strEvent,long nParam) {
    if(strEvent == DEVICE_DIAG_CONNECT) {
        CStatic* status = (CStatic*)GetDlgItem(IDC_STATIC_STATUS);
        TSTRING strStatus = TSTRING(_T("Connected ")) + (*(DeviceDataPt*)nParam)->ID();
        status->SetWindowText(strStatus.c_str());
        GetPort((*(DeviceDataPt*)nParam)->ID().substr(3, (*(DeviceDataPt*)nParam)->ID().length()), m_nPort);
        if(m_bDownloading) {
            SetButtonEnable(FALSE);
        } else {
            SetButtonEnable(TRUE);
        }
        CheckHandsetAndSWVersion();
    } else if(strEvent == DEVICE_DIAG_DISCONNECT) {
        CStatic* status = (CStatic*)GetDlgItem(IDC_STATIC_STATUS);
        status->SetWindowText(_T("Disconnect..."));
        SetButtonEnable(FALSE);
        m_nPort = 0;
    }
}

void CSimpleQDownloadDlg::GetPort(const TSTRING& strPort, long& nPort) {
    nPort = _ttoi(strPort.c_str());
}

void CSimpleQDownloadDlg::OnBnClickedBtnDownload()
{
    SetButtonEnable(FALSE);
    m_bDownloading = true;
    AfxBeginThread(DownloadThread, this);
}

void CSimpleQDownloadDlg::OnBnClickedBtnClose()
{
    OnCancel();
}

UINT CSimpleQDownloadDlg::DownloadThread(LPVOID lparam) {
    CSimpleQDownloadDlg* pThis = (CSimpleQDownloadDlg*)lparam;
    pThis->DownloadFunction();
    return 0;
}

void CSimpleQDownloadDlg::SetIniPath() {
    TCHAR szCurrentPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, szCurrentPath);
    m_strIniPath = CString(szCurrentPath) + CString(_T("\\Download.ini"));
    TSTRING strOemsbl, strAmss;
    TCHAR szPath[500] ={0};
    DWORD lRetValue = ::GetPrivateProfileString(_T("Path"),_T("oemsbl"),NULL,szPath, 500, m_strIniPath.GetString());
    m_strOemsbl = szPath;
    lRetValue = ::GetPrivateProfileString(_T("Path"),_T("amss"),NULL,szPath, 500, m_strIniPath.GetString());
    m_strAmss = szPath;

    if(m_strOemsbl.length() == 0) {
        CString strTmp = m_strAmss.c_str();
        strTmp.Replace(_T("amss"), _T("oemsbl"));
        m_strOemsbl = strTmp.GetString();
    }
}

void CSimpleQDownloadDlg::DownloadFunction() {
    bool bDownloadOK = false;
    CStringA strIniPath;
#ifdef _UNICODE
    DWORD dwLength = WideCharToMultiByte(CP_ACP, 0, m_strIniPath.GetString(), m_strIniPath.GetLength(), 0, 0, 0, 0);
    char* szBuf = new char[dwLength*2];
    memset(szBuf, 0, dwLength*2);
    WideCharToMultiByte(CP_ACP, 0, m_strIniPath.GetString(), m_strIniPath.GetLength(), szBuf, dwLength*2, 0, 0);
    strIniPath = szBuf;
#else
    strIniPath = m_strIniPath;
#endif
    DownloadParameter dp = {0};
    dp.bBackupBRT = true;
    dp.bBackupNV = false;
    dp.bDownloadOemsbl = false;
    dp.bRestoreBRT = false;
    dp.bRestoreNV = true;
    dp.bDownloadDU = true;
    // Do not do any verification
    bDownloadOK = Download(m_nPort, &CSimpleQDownloadDlg::CallBackFunction, strIniPath.GetString(), &dp);
    ChangeOnline(m_nPort);
    PowerOff(m_nPort);
    SetButtonEnable(TRUE);
    m_bDownloading = false;
}

bool CSimpleQDownloadDlg::CallBackFunction(const char* szMsg, int nStep, int nProgress) {
    g_pThis->m_progressDownload.SetPos(nProgress);
    CString strInformation;
    CString strPercantage;
    strPercantage.Format(_T("%d%%"), nProgress);
    
#ifdef _UNICODE
    strInformation.Format(_T("%S"), szMsg);
#else
    strInformation = szMsg;
#endif
    CStatic* staticInformation = (CStatic*)g_pThis->GetDlgItem(IDC_STATIC_STATUS);
    staticInformation->SetWindowText(strInformation);
    CStatic* staticPercentage = (CStatic*)g_pThis->GetDlgItem(IDC_STATIC_PERCENTAGE);
    staticPercentage->SetWindowText(strPercantage);
    return true;
}

bool CSimpleQDownloadDlg::CheckQFuse() {
    int nFTMValue = 0;
    if(!CheckFTM(m_nPort, nFTMValue)) {
        ASSERT(FALSE);
    }
    unsigned long reg1 = 0, reg2 = 0;
    bool bOK = false;
    if(nFTMValue == 0) {
        ChangeFTM(m_nPort);
        Sleep(1000);
        PowerOff(m_nPort);
        Sleep(25000);
    }
    ReadQFuseState(m_nPort, reg1, reg2, bOK);
    return !(reg1 == 6 && reg2 == 0);
}

bool CSimpleQDownloadDlg::CheckSignedCode(const TSTRING& strAmss, const TSTRING& strOemsbl) {

    CFile famss(strAmss.c_str(), CFile::modeRead | CFile::typeBinary| CFile::shareDenyNone );
    CFile foemsbl(strOemsbl.c_str(), CFile::modeRead | CFile::typeBinary| CFile::shareDenyNone );

    BYTE* szAmssBuf = new BYTE[4096];
    BYTE* szOemsblBuf = new BYTE[foemsbl.GetLength()];

    const char* szAttestation = "QUALCOMM Attestation CA";
    famss.Read(szAmssBuf, 4096);
    foemsbl.Read(szOemsblBuf, foemsbl.GetLength());

    bool bAmss = false, bOemsbl = false;
    for(int i = 0; i <  4096 - 23; ++i) {
        int n = memcmp(&(szAmssBuf[i]), szAttestation, 23);
        if(n == 0) {
            bAmss = true;
            break;
        }
    }

    for(int i = 0; i <  foemsbl.GetLength() - 23; ++i) {
        int n = memcmp(&(szOemsblBuf[i]), szAttestation, 23);
        if(n == 0) {
            bOemsbl = true;
            break;
        }
    }

    delete[] szAmssBuf;
    delete[] szOemsblBuf;
    famss.Close();
    foemsbl.Close();
    return bAmss && bOemsbl;
}

bool CSimpleQDownloadDlg::GetInformationFromHandset(CString& strSW, CString& strHW) {
    CQisdaMiscellaneousModule *pMsModule = new CQisdaMiscellaneousModule(m_nPort);

    char szSW[100] ={0};
    char szHW[100] ={0};

    if(pMsModule->GetSWHWVersion(szSW, szHW))
    {
        TCHAR szUnicodeInfor[4096] = {0};
        UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szHW,-1,szUnicodeInfor,4096);	
        CString szString;
        szString.Format( _T("Your Phone HW: %s"), szUnicodeInfor);
        strHW = szUnicodeInfor;
        //SetDLTaskInfo(szString);
        iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szSW,-1,szUnicodeInfor,4096);	
        szString.Format(_T("Your Phone SW: SVN %s"), szUnicodeInfor);
        strSW = szUnicodeInfor;
        //SetDLTaskInfo(szString);

    }
    else
    {
        //SetDLTaskInfo(_T("Can not Get SWHW Version from ME"));
    }

    delete pMsModule;

    DIAG_NVITEM_PACKET_F_type NVResponse;
    CNVItem *pNVModule = new CNVItem(m_nPort);
    if(pNVModule->ReadNVItem(3497, NVResponse))
    {
        TCHAR szUnicodeInfor[4096] = {0};
        UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)NVResponse.item_data,-1,szUnicodeInfor,4096);	
        CString szString;
        szString.Format(_T("Your Phone Variant: %s"), szUnicodeInfor);
        //SetDLTaskInfo(szString);

    }
    else
    {
        //SetDLTaskInfo(_T("Can not get Variant version from ME"));
    }

    delete pNVModule;
    return true;
}

bool CSimpleQDownloadDlg::GetCodeVersion(const TSTRING& strFileName, char* szCheckCode, int nCheckLength, CString& strVer) {
    char szOutput[10] = {0};
    ExtractSpecifiedString(strFileName, szCheckCode, nCheckLength, 100, 2, szOutput, 10);
    strVer.Format(_T("%S"), szOutput);
    return true;
}

bool CSimpleQDownloadDlg::GetSoftwareLoadName(const TSTRING& strFileName, char* szCheckCode, int nCheckLength, CString& strName) {
    char szOutput[1024] = {0};
    ExtractSpecifiedString(strFileName, szCheckCode, nCheckLength, 100, 1, szOutput, 10);
    strName.Format(_T("%S"), szOutput);
    return true;
}

bool CSimpleQDownloadDlg::ExtractSpecifiedString(const TSTRING& strFileName, char* szCheckCode, int nCheckLength, int nExtractLength, int nIndexToGet, char* nOutput, int nOutputLength) {
    CFile file;
    BOOL bOpen = file.Open(strFileName.c_str(), CFile::modeRead | CFile::typeBinary);
    if(!bOpen) return -1;
    char* szBuf = new char[(size_t)file.GetLength()];
    file.Read(szBuf, (UINT)file.GetLength());
    int nCheckPoint = 0;
    for(long i = 0; i < file.GetLength() - nCheckLength; ++i) {
        if(memcmp(szBuf+i, szCheckCode, nCheckLength) == 0) {
            nCheckPoint = i;
            break;
        }
    }
    char* szGet = new char[nExtractLength];
    memset(szGet, 0, nExtractLength);
    memcpy(szGet, szBuf+nCheckPoint, nExtractLength);
    delete [] szBuf;
    int nCount = 0, nIndex = 0;
    char* szExtract = new char[nExtractLength];
    memset(szExtract, 0, nExtractLength);
    for(int i = 0; i < nExtractLength; ++i) {
        if(*(szGet+i) == ';') ++nCount;
        if(nCount == nIndexToGet) {
            szExtract[nIndex++] = szGet[i];
        } else if(nCount == nIndexToGet+1) {
            break;
        }
    }
    if(nIndex > 0)
        memcpy(nOutput, szExtract+1, nIndex-1);
    delete [] szGet;
    delete [] szExtract;
    return true;
}