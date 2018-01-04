// DownloadUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DownloadUI.h"
#include "DownloadUIDlg.h"
#include "AboutBox.h"
#include "../../../code/Components/UIFramework/interface/QControlNotify.h"
#include "../../../code/components/UIFramework/layoutmanager/Win32ProgressBarWidge.h"
#include "../../../code/CommonUtil\Win32SHELLUtility.h"
#include "../../../code/CommonUtil\Win32ThreadUtility.h"
#include <fstream>
#include "..\..\Components\MobileFinder\MobileFinderInterface.h"
#include "OptionDlg.h"
#include "..\..\..\code\odmtma_api\SerCOMLib\SerCOMLibrary.h"
//#include "..\..\..\Code\odmtma_api\QDownload_Export\Download_Export.h"
#include "..\..\..\Code\Modules\QisdaMiscellaneousModule\QisdaMiscellaneousModule.h"
#include "..\..\..\Code\Modules\QisdaNVModule\NVItem.h"
#include "../../FactoryTool/MobileToolKit/MobileToolKit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
#pragma warning(disable:4996)

// CDownloadUIDlg dialog

#define VERSION _T("1.0.0.8 for Melos")

namespace QSYNC_LIB {

    char* g_strStartLog = "----------QDownload Start Log----------\r\n";
    char* g_strEndLog   = "-------------End Start Log-------------";

    CDownloadUIDlgForMelos* g_pThis = NULL;
    CDownloadUIDlgForMelos::CDownloadUIDlgForMelos(CWnd* pParent /*=NULL*/) :Win32DoubleBufferQDialogWidge(CDownloadUIDlgForMelos::IDD, pParent)
        , m_nStep(0), m_bDifferentPartition(false), m_bAutoBackupRestoreBRT(false)
    {
        m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
        CreateILocalization(&m_pILocalization);
        g_pThis = this;
        m_pLayoutManager = NULL;
        m_pMainLayout = NULL;
        m_pWinUpdateBtn= NULL;
        m_pWinCloseBtn = NULL;
        m_pWinLanguageBtn = NULL;
        m_pWinStatusLabel = NULL;
        m_bTimerEnable = false;

        m_imgHEX.pData = NULL; m_imgHEX.nSize = 0; 
        m_imgPBL.pData = NULL; m_imgPBL.nSize = 0;
        m_imgPartition.pData = NULL; m_imgPartition.nSize = 0;
        m_imgDBL.pData = NULL; m_imgDBL.nSize = 0;
        m_imgFSBL.pData = NULL; m_imgFSBL.nSize = 0;
        m_imgOSBL.pData = NULL; m_imgOSBL.nSize = 0;
        m_imgAMSS.pData = NULL; m_imgAMSS.nSize = 0;
        m_imgCEFS.pData = NULL; m_imgCEFS.nSize = 0;
        SetDownloadSettingParameters();
        OpenAllImage();
        //TCHAR szMyDocumentPath[512] = {0};
        //SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_DEFAULT, szMyDocumentPath);
        //CString strDownloadLogPath = CString(szMyDocumentPath) + CString("\\Log\\QDownloadLog.txt");
        //BOOL bOpened = m_logFile.Open(strDownloadLogPath, CFile::modeWrite | CFile::modeCreate);
        //m_logFile.Write(g_strStartLog, strlen(g_strStartLog));
    }

    CDownloadUIDlgForMelos::~CDownloadUIDlgForMelos() {
        if(m_pLayoutManager)
            delete m_pLayoutManager;
        if(m_imgHEX.pData)
            delete m_imgHEX.pData;
        if(m_imgPBL.pData)
            delete m_imgPBL.pData;
        if(m_imgPartition.pData)
            delete m_imgPartition.pData;
        if(m_imgDBL.pData)
            delete m_imgDBL.pData;
        if(m_imgFSBL.pData)
            delete m_imgFSBL.pData;
        if(m_imgOSBL.pData)
            delete m_imgOSBL.pData;
        if(m_imgAMSS.pData)
            delete m_imgAMSS.pData;
        if(m_imgCEFS.pData)
            delete m_imgCEFS.pData;
        //m_logFile.Write(g_strEndLog, strlen(g_strEndLog));
        //m_logFile.Close();
    }

    void CDownloadUIDlgForMelos::DoDataExchange(CDataExchange* pDX) {
        CDialog::DoDataExchange(pDX);
    }

    BEGIN_MESSAGE_MAP(CDownloadUIDlgForMelos, Win32DoubleBufferQDialogWidge)
        ON_WM_PAINT()
        ON_WM_QUERYDRAGICON()
        ON_WM_SIZING()
        ON_WM_CONTEXTMENU()
        ON_COMMAND(IDM_POPMENU_BACKGROUND, OnBackGround)
        ON_COMMAND(IDM_POPMENU_IMAGE, OnImage)
        ON_COMMAND(IDM_POPMENU_OPTION, OnOption)
        ON_COMMAND(IDM_POPMENU_ABOUTBOX, OnAboutBox)
        //ON_WM_TIMER(WM_TIMER, OnTimer)
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    BOOL CDownloadUIDlgForMelos::OnInitDialog() {
        CDialog::OnInitDialog();
        this->SetXMLUI();
        CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST1);
        CString szVersion = CString(_T("QDownload Tool ")) + VERSION;
        pListBox->InsertString(pListBox->GetCount(), szVersion.GetBuffer(0));
        pListBox->InsertString(pListBox->GetCount(), _T("This version is for Melos..."));
        SetWindowText(szVersion);
        SetIcon(m_hIcon, TRUE);			// Set big icon
        SetIcon(m_hIcon, FALSE);		// Set small icon
        this->Initialize();
        //ShowVariantVersion();
        //ShowSoftwareLoadVersion();
        //ShowSignedUnsignedCode();
        m_pWinUpdateBtn->SetEnable(false);
        m_pWinCloseBtn->SetEnable(true);
        return TRUE;  // return TRUE  unless you set the focus to a control
    }

    // If you add a minimize button to your dialog, you will need the code below
    //  to draw the icon.  For MFC applications using the document/view model,
    //  this is automatically done for you by the framework.

    void CDownloadUIDlgForMelos::OnPaint() {
        if (IsIconic()) {
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
        } else {
            CDialog::OnPaint();
        }
    }

    // The system calls this function to obtain the cursor to display while the user drags
    //  the minimized window.
    HCURSOR CDownloadUIDlgForMelos::OnQueryDragIcon() {
        return static_cast<HCURSOR>(m_hIcon);
    }

    void CDownloadUIDlgForMelos::SetXMLUI(void) {
        if(m_pLayoutManager == NULL) {
            SetBackground(_T(""));
            m_pLayoutManager = new CLayoutManager(this, this, _T("DownloadLayout.xml"));
        }

        m_pMainLayout = (SplitLayout<HDC>*)m_pLayoutManager->Load(_T("CDownloadDialogDlgID"));
        m_pWinUpdateBtn = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("UpdateButton"));
        m_pWinCloseBtn = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("CloseButton"));
        //m_pWinLanguageBtn = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("LanguageButton"));
        m_pWinStatusLabel = (WIN32LabelWidge *)m_pLayoutManager->GetWidgetByID(_T("InfoLabel"));
        m_pStep[0] = (WIN32PictureViewerWidge*)m_pLayoutManager->GetWidgetByID(_T("Step1"));
        m_pStep[1] = (WIN32PictureViewerWidge*)m_pLayoutManager->GetWidgetByID(_T("Step2"));
        m_pStep[2] = (WIN32PictureViewerWidge*)m_pLayoutManager->GetWidgetByID(_T("Step3"));
        m_pStep[3] = (WIN32PictureViewerWidge*)m_pLayoutManager->GetWidgetByID(_T("Step4"));
        m_pStep[4] = (WIN32PictureViewerWidge*)m_pLayoutManager->GetWidgetByID(_T("Step5"));
        m_pStep[5] = (WIN32PictureViewerWidge*)m_pLayoutManager->GetWidgetByID(_T("Step6"));
        m_pStep[6] = (WIN32PictureViewerWidge*)m_pLayoutManager->GetWidgetByID(_T("Step7"));
        m_pStep[7] = (WIN32PictureViewerWidge*)m_pLayoutManager->GetWidgetByID(_T("Step8"));
        m_pImage = (WIN32PictureViewerWidge*)m_pLayoutManager->GetWidgetByID(_T("CoachImage"));
        SetLayout(m_pMainLayout);
    }

    void CDownloadUIDlgForMelos::SetBackground(TSTRING strBkImage) {
        SetBKImage(_T("BK_Download.png"),BK_Fixed);
        const int nWIDTH_AboutBox = GetBKImage()->GetWidth(); 
        const int nHeight_AboutBox = GetBKImage()->GetHeight();
        SetMinWidth(nWIDTH_AboutBox);
        SetMinHeight(nHeight_AboutBox);
        SetMaxWidth(nWIDTH_AboutBox);
        SetMaxHeight(nHeight_AboutBox);
    }

    void CDownloadUIDlgForMelos::WinClose() {
        GetDeviceFinder()->StopMonitor();
        PostMessage(WM_CLOSE);
        //FreeLibrary(m_hDll);
    }

    UINT DownloadThread(LPVOID pParam) {
        CDownloadUIDlgForMelos* pDlg = (CDownloadUIDlgForMelos*)pParam;
        pDlg->DownloadFunc();
        return 1;
    }

    void CDownloadUIDlgForMelos::GetStepString(CStringA& strMsg) {
        if(strMsg.Find("DMSSOpenAndWriteHexFile success") != -1) {
            m_nStep = 1; // the 1st light
        } else if(strMsg.Find("StreamingPartition success") != -1) {
            m_nStep = 2; // the 2nd light
        } else if(strMsg.Find("********** [Image] DBL **********") != -1) {
            m_nStep = 3;
        } else if(strMsg.Find("********** [Image] FSBL **********") != -1) {
            m_nStep = 4;
        } else if(strMsg.Find("********** [Image] OSBL **********") != -1) {
            m_nStep = 5;
        } else if(strMsg.Find("********** [Image] (AMSSHD)/AMSS **********") != -1) {
            m_nStep = 6;
        } else if(strMsg.Find("********** [Image] CEFS **********") != -1) {
            m_nStep = 7;
        } else if(strMsg.Find("ReOpenAfterReset success") != -1) {
            m_nStep = 8;
        }
    }

    // This event is used for download progress, please see the CShareDownload6240
    void CDownloadUIDlgForMelos::Event(const std::string& strEvent,long nParam) {
        if(strEvent == EVENT_DL_PROGRESS) {
            int nProcentage = ((DL_PROGRESS*)nParam)->nProgress;
            CStringA strPercent;
            DL_PROGRESS* dlMsg = (DL_PROGRESS*)nParam;
            strPercent.Format("%s %d%%",((DL_PROGRESS*)nParam)->szMsg, nProcentage);
            //m_logFile.Write(strPercent.GetString(), strPercent.GetLength());
            //m_logFile.Write("\r\n", 2);
            //m_logFile.Flush();
            DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, strPercent.GetString(), -1, NULL, 0);
            wchar_t *pwText = new wchar_t[dwNum];
            MultiByteToWideChar(CP_ACP, 0, strPercent.GetString(), -1, pwText, dwNum);
            InsertStringList(pwText, _T("Downloading..."));
            delete[] pwText;
            GetStepString(strPercent);
            if(strPercent.Find("StreamingOpenMultiImage success") != -1) return;
            UpdateUIProgressBar("", 0, nProcentage);
        } else if(strEvent == EVENT_DL_PARTITION_DIF) {
            m_bDifferentPartition = true;
        }
    }

    // This event is used for Mobile finder or any wstring event
    void CDownloadUIDlgForMelos::Event(const TSTRING& strEvent, long nParam) {
        if(strEvent == EVENT_BUTTON_CLICK) {
            OnEventButtonClick(nParam);
        } else if(strEvent == EVENT_SUBMENU) {
            //m_pILocalization->SetRegion(strEvent);
        } else if(strEvent == DEVICE_DIAG_CONNECT) {
            OnEventConnected(nParam);
        } else if(strEvent == DEVICE_DIAG_DISCONNECT) {
            OnEventDisconnect(nParam);
        } else {
            //ASSERT(0);
        }
    }

    void CDownloadUIDlgForMelos::OnEventButtonClick(long nParam) {
        if(m_pWinCloseBtn->GetID() == nParam) {
            WinClose();
        } else if(m_pWinUpdateBtn->GetID() == nParam) {
            if(m_bDownloadStart) {
                MessageBox(_T("Download Thread already Open, Please Wait!"));
            } else {
                m_pWinUpdateBtn->SetEnable(false);
                m_pWinCloseBtn->SetEnable(false);
                for(int i = 0; i < 8; ++i)
                    UpdateStepInformation(m_pStep + i, 1);
                CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO1);
                m_nSelectCom = (int)pCombo->GetItemData(pCombo->GetCurSel());
                AfxBeginThread(DownloadThread, (LPVOID) this, THREAD_PRIORITY_NORMAL);
            }
        }
    }
    void CDownloadUIDlgForMelos::OnEventConnected(long nParam) {
        DeviceDataPt* pspDeviceData = (QSYNC_LIB::DeviceDataPt*)nParam;
        DeviceDataPt spDeviceData = *pspDeviceData; 
        CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO1);
        pCombo->InsertString(m_nComNum, spDeviceData->ID().c_str());
        CString szTemp = spDeviceData->ID().c_str();
        szTemp.Delete(0,3);
        int nData = 0;
        for(int i = 0; i < szTemp.GetLength(); i++)
            nData = nData*10 + (szTemp.GetAt(i)- _T('0'));
        pCombo->SetItemData(m_nComNum, nData);
        if(m_bDownloadStart) {
            //SetComport(nData);
            m_nSelectCom = nData;
        }
        m_nComNum++;
        if(m_nComNum == 1)
            pCombo->SetCurSel(0);
        m_pWinUpdateBtn->SetEnable((!m_bDownloadStart) && (m_nComNum > 0));
        m_pWinStatusLabel->SetDisplayString(_T("Connected..."));
        m_nSelectCom = (int)pCombo->GetItemData(pCombo->GetCurSel());
        //GetInfoFromME();
    }

    void CDownloadUIDlgForMelos::OnEventDisconnect(long nParam) {
        DeviceDataPt* pspDeviceData = (QSYNC_LIB::DeviceDataPt*)nParam;
        DeviceDataPt spDeviceData = *pspDeviceData; 

        CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO1);
        int nIndex = pCombo->FindString(-1, spDeviceData->ID().c_str());
        if(nIndex >= 0) {
            int nNext = nIndex;
            if(nIndex == pCombo->GetCurSel()) {
                if(nIndex > 0)
                    nNext--;						
            }
            pCombo->DeleteString(nIndex);
            pCombo->SetCurSel(nNext);
            m_nComNum--;
        }
        m_pWinUpdateBtn->SetEnable((!m_bDownloadStart) && (m_nComNum > 0));
    }

    void CDownloadUIDlgForMelos::OnSizing(UINT fwSide, LPRECT pRect) {
        //Win32DoubleBufferFormCWndWidge<CQDialog>::OnSizing(fwSide, pRect);
        GeometryRect rcClient = GetGeometry();
        CQDialog::SetWindowRegion(pRect, rcClient);
        // TODO: Add your message handler code here
    }

    int CDownloadUIDlgForMelos::CheckBatteryStage() {
        int nRet = IDYES;
        if(ReadBatteryVoltage() < 3500) {
            int nRet = AfxMessageBox(_T("The bettery is not enough to download.\nAre you sure to download?"), MB_YESNO, MB_ICONINFORMATION);
            if(nRet == IDNO) {
                InsertStringList(_T("User cancelled download procedure."), _T(""));
                m_pWinUpdateBtn->SetEnable(true);
                m_pWinCloseBtn->SetEnable(true);
                m_bDownloadStart = false;
            }
        }
        return nRet;
    }

    bool CDownloadUIDlgForMelos::SetAllImageParameters() {
        int nRet = _taccess(m_strIni.GetString(), 0);
        if(nRet == -1) return false;

    }

    void CDownloadUIDlgForMelos::DownloadStage() {
        m_bDownloadStart = true;
        InsertStringList(_T("Start to download"), _T("Start Download"));
        CString szPath;
        ::GetModuleFileName(NULL, szPath.GetBuffer(MAX_PATH), MAX_PATH);
        szPath.ReleaseBuffer();
        szPath = szPath.Left(szPath.ReverseFind(_T('\\'))+1);
        char cstrPath[MAX_PATH] ={0};
        UINT iUTF8CharCount = WideCharToMultiByte(CP_ACP,FALSE,szPath.GetBuffer(0),szPath.GetLength(), cstrPath,MAX_PATH,NULL,NULL);	
        CStringA szIni ;
        szIni.Format("%sDownload.ini", cstrPath);
        m_strIni = szIni;

        /// call download here, but check each part exists or not
        CShareDownload6240 download(m_nSelectCom);
        if(m_imgAMSS.pData != NULL)
            download.SetAMSS(m_imgAMSS.pData, m_imgAMSS.nSize);
        if(m_imgCEFS.pData != NULL)
            download.SetCEFS(m_imgCEFS.pData, m_imgCEFS.nSize);
        if(m_imgDBL.pData != NULL)
            download.SetDBL(m_imgDBL.pData, m_imgDBL.nSize);
        if(m_imgFSBL.pData != NULL)
            download.SetFSBL(m_imgFSBL.pData, m_imgFSBL.nSize);
        if(m_imgHEX.pData != NULL)
            download.SetHex(m_imgHEX.pData, m_imgHEX.nSize);
        if(m_imgOSBL.pData != NULL)
            download.SetOSBL(m_imgOSBL.pData, m_imgOSBL.nSize);
        if(m_imgPartition.pData != NULL)
            download.SetPartition(m_imgPartition.pData, m_imgPartition.nSize);
        if(m_imgPBL.pData != NULL)
            download.SetPBL(m_imgPBL.pData, m_imgPBL.nSize);

        download.Register(this, EVENT_DL_PROGRESS);
        download.Register(this, EVENT_DL_PARTITION_DIF);
        download.SetResetMode(false);
        bool bContinueDownload = download.Download(true);
        if(m_bDifferentPartition == true) {
            UINT nRet = AfxMessageBox(_T("The partition is different from the handset.\nWould you like to change the partition!!\nPress YES to change."), MB_YESNO);
            if(nRet == IDNO) {
                OnLineMode(m_nSelectCom);
                PowerOff(m_nSelectCom);
                //AfxMessageBox(_T("Please reset the handset..."));
                return;
            }
        }
        if(m_bDifferentPartition == true && bContinueDownload == false) { // partition is different
            download.SetIsOveridePartition(true);
            ForceOpenAllImage();
            if(m_imgAMSS.pData != NULL)
                download.SetAMSS(m_imgAMSS.pData, m_imgAMSS.nSize);
            if(m_imgCEFS.pData != NULL)
                download.SetCEFS(m_imgCEFS.pData, m_imgCEFS.nSize);
            if(m_imgDBL.pData != NULL)
                download.SetDBL(m_imgDBL.pData, m_imgDBL.nSize);
            if(m_imgFSBL.pData != NULL)
                download.SetFSBL(m_imgFSBL.pData, m_imgFSBL.nSize);
            if(m_imgHEX.pData != NULL)
                download.SetHex(m_imgHEX.pData, m_imgHEX.nSize);
            if(m_imgOSBL.pData != NULL)
                download.SetOSBL(m_imgOSBL.pData, m_imgOSBL.nSize);
            if(m_imgPartition.pData != NULL)
                download.SetPartition(m_imgPartition.pData, m_imgPartition.nSize);
            if(m_imgPBL.pData != NULL)
                download.SetPBL(m_imgPBL.pData, m_imgPBL.nSize);
            download.SetResetMode(true);
            bContinueDownload = download.StreamingDLFromPartition(true);
        }
        if(bContinueDownload == false) {
            if(m_nStep == 0) m_nStep = 1;
            UpdateStepInformation(m_pStep + m_nStep - 1, 2);
        }
        download.Unregister(this, EVENT_DL_PROGRESS);
        download.Unregister(this, EVENT_DL_PARTITION_DIF);
        m_bDownloadStart = false;
    }

    void CDownloadUIDlgForMelos::FinalReboot() {
        TCHAR szBuf[16] = {0};
        DWORD dwResult = ::GetPrivateProfileString(_T("Setting"), _T("Reboot"), _T("0"), szBuf, 16, m_strIni.GetString());
        if(*szBuf == '0') {
            //OnLineMode(m_nSelectCom);
            PowerOff(m_nSelectCom);
        } else {
            //while(true) {
            //    //ChangeOnline(m_nSelectCom);
            //    int nFTMValue = 0;
            //    CheckFTM(m_nSelectCom, nFTMValue);
            //    if(nFTMValue == 0)
            //        break;
            //}
            //PowerOff(m_nSelectCom);
            m_bDownloadStart = false;
        }
    }

    void CDownloadUIDlgForMelos::DownloadFunc() {
        bool bDownloadOK = true;
        // Check battery
        bool bInDLMode = IsInDownloadMode(m_nSelectCom);
        if(!bInDLMode)
            if(CheckBatteryStage() == IDNO) return;

        CTime t = CTime::GetCurrentTime();
        CString szTime;
        szTime.Format(_T("Download Start at: %02d:%02d:%02d"), t.GetHour(), t.GetMinute(), t.GetSecond());
        InsertStringList(szTime, _T(""));

        if(bDownloadOK && !bInDLMode) {
            InsertStringList(_T("Get ME info, Please wait......"), _T("Checking ..."));
            if(!GetInfoFromME()) {
                InsertStringList(_T("Can not Get Info From ME! Please try again!"));
                bDownloadOK = false;
            }
        }
        if(m_bAutoBackupRestoreBRT && !bInDLMode) {
            BackupBRTForMelos();
            bool bBackup = BackupNVPartition(m_nSelectCom);
        }
        if(bDownloadOK) {
            //DownloadStage();
        }
        if(m_bAutoBackupRestoreBRT && !bInDLMode) {
            RestoreBRTForMelos();
            bool bRestore = RestoreNVPartition(m_nSelectCom);
        }
        RestoreNVPartition(m_nSelectCom);
        ChangeOnline(m_nSelectCom);
        ChangeOnline(m_nSelectCom);
        OnLineMode(m_nSelectCom);
        PowerOff(m_nSelectCom);
        t = CTime::GetCurrentTime();
        szTime.Format(_T("Download End at: %02d:%02d:%02d"),t.GetHour(),t.GetMinute(),t.GetSecond());
        InsertStringList(szTime, _T("Download End"));
        m_pWinUpdateBtn->SetEnable(true);
        m_pWinCloseBtn->SetEnable(true);

        //FinalReboot();
    }

    bool CDownloadUIDlgForMelos::GetMyDocumentFolder(CStringA& strFolder) {
        char pszPath[MAX_PATH] = {0};
        if(SUCCEEDED(SHGetSpecialFolderPathA(NULL, pszPath, CSIDL_PERSONAL, FALSE))) {
            strFolder = pszPath;
            return true;
        }
        return false;
    }
    void CDownloadUIDlgForMelos::BackupBRTForMelos() {
        InsertStringList(_T("Auto Backup / Restore BRT...Backup!!!"), _T(""));
        char szPath[512] = {0};
        GetCurrentDirectoryA(512, szPath);;
        CStringA strCSVFilePath = CStringA(szPath) + CStringA("\\BackupNV-Items.csv");
        CStringA strBackupItems = CStringA(szPath) + CStringA("\\BackupBRT.brt");
        BackupNV2BRT(m_nSelectCom, strBackupItems.GetString(), strCSVFilePath.GetString());
    }
    void CDownloadUIDlgForMelos::RestoreBRTForMelos() {
        InsertStringList(_T("Auto Backup / Restore BRT...Restore!!!"), _T(""));
        char szPath[512] = {0};
        GetCurrentDirectoryA(512, szPath);;
        CStringA strBackupItems = CStringA(szPath) + CStringA("\\BackupBRT.brt");
        RestoreBRT2NV(m_nSelectCom, strBackupItems.GetString());
        DeleteFileA(strBackupItems.GetString());
    }

    void CDownloadUIDlgForMelos::Initialize() {
        assert(m_pWinStatusLabel != NULL);
        assert(m_pWinUpdateBtn != NULL);
        m_pWinStatusLabel->SetDisplayString(_T("Connecting..."));
        m_nPreStep = 0;
        m_nPreProgress = 0;
        m_strPreMessage = L"";
        m_bDownloadStart = false;
        m_nComNum = 0;
        m_nSelectCom = -1;
        GetDeviceFinder()->StartMonitor();
        GetDeviceFinder()->Register(this);
    }

    bool CDownloadUIDlgForMelos::GetInfoFromME() {
        CQisdaMiscellaneousModule *pMsModule = new CQisdaMiscellaneousModule(m_nSelectCom);
        char szSW[100] ={0};
        char szHW[100] ={0};
        if(pMsModule->GetSWHWVersion(szSW, szHW)) {
            TCHAR szUnicodeInfor[4096] = {0};
            UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szHW,-1,szUnicodeInfor,4096);	
            CString szString;
            szString.Format( _T("Your Phone HW: %s"), szUnicodeInfor);
            InsertStringList(szString, _T("Checking ..."));
            iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szSW,-1,szUnicodeInfor,4096);	
            szString.Format(_T("Your Phone SW: SVN %s"), szUnicodeInfor);
            InsertStringList(szString, _T("Checking ..."));
        } else {
            InsertStringList(_T("Can not Get SWHW Version from ME"), _T("Checking ..."));
        }
        delete pMsModule;
        DIAG_NVITEM_PACKET_F_type NVResponse;
        CNVItem *pNVModule = new CNVItem(m_nSelectCom);
        if(pNVModule->ReadNVItem(3497, NVResponse)) {
            TCHAR szUnicodeInfor[4096] = {0};
            UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)NVResponse.item_data,-1,szUnicodeInfor,4096);	
            CString szString;
            szString.Format(_T("Your Phone Variant: %s"), szUnicodeInfor);
            szString = szString.Mid(0, szString.ReverseFind('.'));
            szString.Replace('_', ' ');
            InsertStringList(szString, _T("Checking ..."));
        } else {
            InsertStringList( _T("Can not get Variant version from ME"), _T("Checking Fail"));
        }
        delete pNVModule;
        return true;
    }

    bool CDownloadUIDlgForMelos::CallBack(const char* szMsg, int nStep, int nProgress) {
        g_pThis->UpdateUIProgressBar(szMsg, nStep, nProgress);
        return true;
    }

    void CDownloadUIDlgForMelos::UpdateStepInformation(WIN32PictureViewerWidge** pStep, int nType) {
        if(pStep == NULL || *pStep == NULL)
            return;
        switch(nType) {
            case 0:
                (*pStep)->SetImagePath(_T("Btn_Success.png"));
            break;
            case 1:
                (*pStep)->SetImagePath(_T("Btn_Empty.PNG"));
            break;
            case 2:
                (*pStep)->SetImagePath(_T("Btn_Fail.PNG"));
            break;
            default:
                ASSERT(false);
            break;

        }
        (*pStep)->Update();
    }

    bool CDownloadUIDlgForMelos::UpdateUIProgressBar(const char* szMsg, int nStep, int nProgress) {
        if(nProgress == 100 && m_nStep != 0) {
            UpdateStepInformation(m_pStep + m_nStep - 1, 0);
            if(m_nStep == 8) {
                m_pWinUpdateBtn->SetEnable(true);
                m_pWinCloseBtn->SetEnable(true);
            }
        }

        static Win32ProgressBarWidge *pProgressBar = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar"));
        if(pProgressBar != NULL)
            pProgressBar->UpdatePercent(nProgress);
        return true;
    }
    void CDownloadUIDlgForMelos::OnContextMenu(CWnd* pWnd, CPoint point) {
        CMenu PopMenu;
        PopMenu.CreatePopupMenu();
//#ifdef POPMENU		
        PopMenu.AppendMenu(MF_STRING, IDM_POPMENU_BACKGROUND, _T("&Back Ground"));
        //		PopMenu.AppendMenu(MF_STRING, IDM_POPMENU_IMAGE, _T("&Image"));
        PopMenu.AppendMenu(MF_STRING, IDM_POPMENU_OPTION, _T("&Option"));
//#endif
        //CMenu PopMenu;
        //PopMenu.CreatePopupMenu();
        PopMenu.AppendMenu(MF_STRING, IDM_POPMENU_ABOUTBOX, _T("&About QDownload"));
        //PopMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
    }

    void CDownloadUIDlgForMelos::OnBackGround() {
        TCHAR szPath[MAX_PATH];
        ::GetCurrentDirectory(MAX_PATH, szPath);
        CFileDialog fd(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("JPG File(*.jpg)|*.jpg|PNG File(*.png)|*.png|Bitmap File(*.bmp)|*.bmp||"));
        if(fd.DoModal() == IDOK) {
            SetCurrentDirectory(szPath);
            //Record this BK image
            SetBKImage_AbsolutePath(fd.GetPathName().GetBuffer(),BK_Fixed);
            this->Update();
        }
    }

    void CDownloadUIDlgForMelos::OnImage() {
        TCHAR szPath[MAX_PATH];
        ::GetCurrentDirectory(MAX_PATH, szPath);
        CFileDialog fd(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("JPG File(*.jpg)|*.jpg|PNG File(*.png)|*.png|Bitmap File(*.bmp)|*.bmp||"));
        if(fd.DoModal() == IDOK) {
            SetCurrentDirectory(szPath);
            //Record this BK imag;
            m_pImage->SetImagePath(fd.GetPathName().GetBuffer());
            m_pImage->Update(true);
        }
    }

    void CDownloadUIDlgForMelos::OnOption() {
        //CString szPath;
        //::GetModuleFileName(NULL, szPath.GetBuffer(MAX_PATH), MAX_PATH);
        //szPath.ReleaseBuffer();
        //szPath = szPath.Left(szPath.ReverseFind(_T('\\'))+1);
        //COptionDlg dlg(szPath);
        //if(dlg.DoModal() == IDOK) {
        //    //CString szPath;
        //    //::GetModuleFileName(NULL, szPath.GetBuffer(MAX_PATH), MAX_PATH);
        //    //szPath.ReleaseBuffer();
        //    //szPath = szPath.Left(szPath.ReverseFind('\\')+1);
        //    //szPath = szPath+L"Download.ini"; 
        //}
    }

    void CDownloadUIDlgForMelos::OnAboutBox() {
        CAboutBox AboutBox;
        AboutBox.DoModal();
    }

    bool CDownloadUIDlgForMelos::GetDLPathFromIni(const TSTRING& strIniPath, const TSTRING& strName, TSTRING& strPath) {
        bool bRes = false;
        TCHAR szPath[500] ={0};
        DWORD lRetValue = ::GetPrivateProfileString(_T("Path"),strName.c_str(),NULL,szPath, 500, strIniPath.c_str());
        strPath = szPath;
        if(::_taccess_s(strPath.c_str(),0) == 0)
            bRes = true;
        else
            strPath = _T("");
        return bRes;
    }

    void CDownloadUIDlgForMelos::InsertStringList(CString szMessage, CString szLabelInfo) {
        CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST1);
        pList->InsertString(pList->GetCount(), szMessage);
        m_pWinStatusLabel->SetDisplayString(szLabelInfo.GetBuffer(0));
        pList->SetCurSel(pList->GetCount()-1);
    }

    bool CDownloadUIDlgForMelos::SetIniPath() {
        TCHAR szBuf[MAX_PATH] = {0};
        ::GetModuleFileName(NULL, szBuf, MAX_PATH);
        CString szPath = szBuf;
        szPath = szPath.Left(szPath.ReverseFind(_T('\\'))+1);
        char cstrPath[MAX_PATH] ={0};
        UINT iUTF8CharCount = WideCharToMultiByte(CP_ACP,FALSE,szPath.GetBuffer(0),szPath.GetLength(), cstrPath,MAX_PATH,NULL,NULL);	
        CStringA szIni ;
        szIni.Format("%sDownload.ini", cstrPath);
        int nRet = _access(szIni.GetString(), 0);
        m_strIni = szIni;
        return !(ENOENT == nRet);
    }

    bool CDownloadUIDlgForMelos::CheckSignedCode() {
        InsertStringList(_T("Check Signed Code..."), _T(""));
        bool bRes = false;
        TSTRING strAmss, strOemsbl;
        GetDLPathFromIni(m_strIni.GetString(), _T("amss"), strAmss);
        GetDLPathFromIni(m_strIni.GetString(), _T("oemsbl"), strOemsbl);
        if(strOemsbl.length() == 0) {
            CString strTmp = strAmss.c_str();
            strTmp.Replace(_T("amss"), _T("oemsbl"));
            strOemsbl = strTmp.GetString();
        }
        const char* szAttestation = "QUALCOMM Attestation CA";
        bool bAmss = false, bOemsbl = false;
        CFile famss;
        if(strAmss.length() != 0) {
            famss.Open(strAmss.c_str(), CFile::modeRead | CFile::typeBinary);
            BYTE* szAmssBuf = new BYTE[4096];
            famss.Read(szAmssBuf, 4096);
            for(int i = 0; i <  4096 - 23; ++i) {
                int n = memcmp(&(szAmssBuf[i]), szAttestation, 23);
                if(n == 0) {
                    bAmss = true;
                    break;
                }
            }
            delete[] szAmssBuf;
            if(bAmss)
                InsertStringList(_T("Check Signed Code--amss..."), _T("Signed"));
            else
                InsertStringList(_T("Check Signed Code--amss..."), _T("Unsigned"));
        }
        CFile foemsbl;
        if(strOemsbl.length() != 0) {
            foemsbl.Open(strOemsbl.c_str(), CFile::modeRead | CFile::typeBinary);
            BYTE* szOemsblBuf = new BYTE[(size_t)foemsbl.GetLength()];
            foemsbl.Read(szOemsblBuf, (UINT)foemsbl.GetLength());
            for(int i = 0; i <  foemsbl.GetLength() - 23; ++i) {
                int n = memcmp(&(szOemsblBuf[i]), szAttestation, 23);
                if(n == 0) {
                    bOemsbl = true;
                    break;
                }
            }
            delete[] szOemsblBuf;
            if(bOemsbl)
                InsertStringList(_T("Check Signed Code--oemsbl..."), _T("Signed"));
            else
                InsertStringList(_T("Check Signed Code--oemsbl..."), _T("Unsigned"));
        }
        return bAmss && bOemsbl;
    }

    bool CDownloadUIDlgForMelos::ExtractSpecifiedString(const TSTRING& strFileName, char* szCheckCode, int nCheckLength, int nExtractLength, int nIndexToGet, char* nOutput, int nOutputLength) {
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

    int CDownloadUIDlgForMelos::GetCodeSecurityVersion(const TSTRING& strFileName, char* szCheckCode, int nCheckLength) {
        char szOutput[10] = {0};
        ExtractSpecifiedString(strFileName, szCheckCode, nCheckLength, 100, 5, szOutput, 10);
        int nRet = atoi((const char*)szOutput);
        return nRet;
    }

    bool CDownloadUIDlgForMelos::GetCodeVersion(const TSTRING& strFileName, char* szCheckCode, int nCheckLength, CString& strVer) {
        char szOutput[10] = {0};
        ExtractSpecifiedString(strFileName, szCheckCode, nCheckLength, 100, 2, szOutput, 10);
        strVer.Format(_T("%S"), szOutput);
        return true;
    }

    bool CDownloadUIDlgForMelos::GetCodeSigned(const TSTRING& strFileName, char* szCheckCode, int nCheckLength) {
        char szOutput[10] = {0};
        ExtractSpecifiedString(strFileName, szCheckCode, nCheckLength, 100, 4, szOutput, 10);
        return *szOutput == 'P' ? true : false;
    }

    // Check QFuse if mobile in FTM
    bool CDownloadUIDlgForMelos::CheckQFuseState1() {
        unsigned long reg1 = 0, reg2 = 0;
        bool bSuccess = false;
        ReadQFuseState(m_nSelectCom, reg1, reg2, bSuccess);
        //m_pFnReadQFuse(m_nSelectCom, reg1, reg2, bSuccess);
        if(bSuccess == false) return false;
        return !(reg1 == 6 && reg2 == 0);
    }
    // Check QFuse after change FTM
    bool CDownloadUIDlgForMelos::CheckQFuseState2() {
        return false;
    }
    // Check QFuse after write NV 453 (FTM)
    bool CDownloadUIDlgForMelos::CheckQFuseState3() {
        ChangeFTM(m_nSelectCom);
        SetOffline(m_nSelectCom);
        ResetHandset(m_nSelectCom);
        Sleep(25000);
        bool bRet = CheckQFuseState1();
        return bRet;
    }

    bool CDownloadUIDlgForMelos::GetVariantString(CString& strVariantString) {
        TSTRING strVar;
        GetDLPathFromIni(m_strIni.GetString(), _T("cefs"), strVar);
        if(strVar.length() == 0) return true;
        strVariantString = strVar.c_str();
        int nFound = strVariantString.ReverseFind('\\');
        strVariantString = strVariantString.Mid(nFound+1, strVariantString.GetLength() - nFound);
        strVariantString.Replace(_T(".qvcefs"), _T(""));
        strVariantString.Replace(_T("_"), _T(" "));
        return true;
    }

    void CDownloadUIDlgForMelos::ShowVariantVersion() {
        CString strVariantString;
        GetVariantString(strVariantString);
        if(strVariantString.GetLength() != 0) {
            strVariantString = CString(_T("Variant: ")) + strVariantString;
            InsertStringList(strVariantString, _T(""));
        }
    }

    void CDownloadUIDlgForMelos::ShowSoftwareLoadVersion() {
        CString strCodeVersion;
        TSTRING strAmss, strOemsbl;
        GetDLPathFromIni(m_strIni.GetString(), _T("amss"), strAmss);
        GetDLPathFromIni(m_strIni.GetString(), _T("oemsbl"), strOemsbl);
        if(strOemsbl.length() == 0) {
            CString strTmp = strAmss.c_str();
            strTmp.Replace(_T("amss"), _T("oemsbl"));
            strOemsbl = strTmp.GetString();
        }
        GetCodeVersion(strOemsbl, "O'zbplt", 7, strCodeVersion);
        if(strCodeVersion.GetLength() != 0) {
            strCodeVersion = CString(_T("Software version: ")) + strCodeVersion;
            InsertStringList(strCodeVersion, _T(""));
        }
    }

    void CDownloadUIDlgForMelos::ShowSignedUnsignedCode() {
        TSTRING strAmss, strOemsbl;
        GetDLPathFromIni(m_strIni.GetString(), _T("amss"), strAmss);
        GetDLPathFromIni(m_strIni.GetString(), _T("oemsbl"), strOemsbl);
        if(strOemsbl.length() == 0) {
            CString strTmp = strAmss.c_str();
            strTmp.Replace(_T("amss"), _T("oemsbl"));
            strOemsbl = strTmp.GetString();
        }
        bool bSigned = GetCodeSigned(strOemsbl, "O'zbplt", 7);
        CString strCodeVersion = CString(_T("The code is ")) + (bSigned ? CString(_T("signed")) : CString(_T("unsigned")));
        InsertStringList(strCodeVersion, _T(""));
    }

    int CDownloadUIDlgForMelos::ReadBatteryVoltage() {
        InsertStringList(_T("Checking Battery voltage..."), _T(""));
        int nVoltage = 0;
        ::ReadBatteryVoltage(m_nSelectCom, nVoltage);
        //m_pFnReadBatteryVoltage(m_nSelectCom, nVoltage);
        TCHAR* szShowString = (nVoltage > 3500) ? _T("Battery voltage is enough to download...") : _T("Battery voltage is not enough to download...");
        InsertStringList(szShowString, _T(""));
        return nVoltage;
    }

    void CDownloadUIDlgForMelos::SetDownloadSettingParameters() {
        if(SetIniPath()) {
            TCHAR szBuf[20] ={0};
            DWORD dwResult = ::GetPrivateProfileString(_T("Setting"), _T("BackupNVPartition"), _T("0"), szBuf, 16, m_strIni.GetString());
            m_strBackupNVPartition = szBuf;
            dwResult = ::GetPrivateProfileString(_T("Setting"), _T("RestoreNVPartition"), _T("0"), szBuf, 16, m_strIni.GetString());
            m_strRestoreNVPartition = szBuf;
            dwResult = ::GetPrivateProfileString(_T("Setting"), _T("BackupBRT"), _T("0"), szBuf, 16, m_strIni.GetString());
            m_strBackupBRT = szBuf;
            dwResult = ::GetPrivateProfileString(_T("Setting"), _T("RestoreBRT"), _T("0"), szBuf, 16, m_strIni.GetString());
            m_strRestoreBRT = szBuf;
            dwResult = ::GetPrivateProfileString(_T("Setting"), _T("DownloadDU"), _T("0"), szBuf, 16, m_strIni.GetString());
            m_strDownloadDU = szBuf;
            dwResult = ::GetPrivateProfileString(_T("Setting"), _T("AutoBackupRestoreBRT"), _T("0"), szBuf, 16, m_strIni.GetString());
            m_bAutoBackupRestoreBRT = (szBuf[0] == '0' ? false :  true);
        }
    }

    bool CDownloadUIDlgForMelos::OpenImage(IMAGE & imgLoad, CString strLoadName) {
        TCHAR szCurrentFolder[MAX_PATH] = {0};
        ::GetCurrentDirectory(MAX_PATH, szCurrentFolder);
        m_strIni = CString(szCurrentFolder) + _T("\\Download.ini");
        ASSERT(_taccess(m_strIni.GetString(), 0) == 0);

        TCHAR szLoadPath[MAX_PATH] = {0};
        GetPrivateProfileString(_T("path"), strLoadName.GetString(), _T(""), szLoadPath, MAX_PATH, m_strIni.GetString());
        CString strLoadFileName = CString(szCurrentFolder) + CString(_T("\\")) + CString(szLoadPath);
        bool bLoadExist = (_taccess(strLoadFileName.GetString(), 0) == 0);
        if(!bLoadExist) return false;
        GetFileData(strLoadFileName, imgLoad);
        return bLoadExist;
    }

    bool CDownloadUIDlgForMelos::OpenAllImage() {
        bool bAMSSExist = OpenImage(m_imgAMSS, _T("amss"));
        bool bCEFSExist = OpenImage(m_imgCEFS, _T("cefs"));
        bool bDBLExist = OpenImage(m_imgDBL, _T("dbl"));
        bool bFSBLExist = OpenImage(m_imgFSBL, _T("fsbl"));
        bool bOSBLExist = OpenImage(m_imgOSBL, _T("osbl"));
        bool bPartitionExist = OpenImage(m_imgPartition, _T("partition"));
        bool bHEXExist = OpenImage(m_imgHEX, _T("hex"));
        return bAMSSExist & bCEFSExist & bDBLExist & bFSBLExist & bOSBLExist & bPartitionExist & bHEXExist;
    }

    void CDownloadUIDlgForMelos::ForceOpenAllImage() {
        TCHAR szCurrentFolder[MAX_PATH] = {0};
        ::GetCurrentDirectory(MAX_PATH, szCurrentFolder);
        CString strAMSSLoadFileName = CString(szCurrentFolder) + CString(_T("\\")) + CString(_T("amss.mbn"));
        CString strDBLLoadFileName = CString(szCurrentFolder) + CString(_T("\\")) + CString(_T("dbl.mbn"));
        CString strFSBLLoadFileName = CString(szCurrentFolder) + CString(_T("\\")) + CString(_T("fsbl.mbn"));
        CString strMIBIBLoadFileName = CString(szCurrentFolder) + CString(_T("\\")) + CString(_T("mibib.mbn"));
        CString strOSBLLoadFileName = CString(szCurrentFolder) + CString(_T("\\")) + CString(_T("osbl.mbn"));
        //CString strPARTITIONLoadFileName = CString(szCurrentFolder) + CString(_T("\\")) + CString(_T("partition.mbn"));
        //CString strHEXLoadFileName = CString(szCurrentFolder) + CString(_T("\\")) + CString(_T("NPRG6270.hex"));
        GetFileData(strAMSSLoadFileName, m_imgAMSS);
        GetFileData(strDBLLoadFileName, m_imgDBL);
        GetFileData(strFSBLLoadFileName, m_imgFSBL);
        //GetFileData(strMIBIBLoadFileName, m_imgMIBIB);
        GetFileData(strOSBLLoadFileName, m_imgOSBL);
    }

    bool CDownloadUIDlgForMelos::GetFileData(CString strLoadFileName, IMAGE & imgLoad)
    {
        CFile fLoad;
        BOOL bOpened = fLoad.Open(strLoadFileName.GetString(), CFile::modeRead);
        if(imgLoad.pData == NULL && bOpened) {
            imgLoad.pData = new BYTE[fLoad.GetLength()+2];
            memset(imgLoad.pData, 0, fLoad.GetLength()+2);
            imgLoad.nSize = fLoad.GetLength();
            fLoad.Read(imgLoad.pData, fLoad.GetLength());
        };
        return bOpened;
    }

}



#pragma warning(pop)