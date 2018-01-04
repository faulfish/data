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

#define VERSION _T("1.0.0.9 for Lavernock")

namespace QSYNC_LIB {

    char* g_strStartLog = "----------QDownload Start Log----------\r\n";
    char* g_strEndLog   = "-------------End Start Log-------------";

    CDownloadUIDlgForLavernock* g_pThis = NULL;
    CDownloadUIDlgForLavernock::CDownloadUIDlgForLavernock(CWnd* pParent /*=NULL*/) :Win32DoubleBufferQDialogWidge(CDownloadUIDlgForLavernock::IDD, pParent)
        , m_nStep(0), m_bEnableLog(false), m_bIsInDLMode(false)
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
        m_imgPartition.pData = NULL; m_imgPartition.nSize = 0;
        m_imgQCSBL.pData = NULL; m_imgQCSBL.nSize = 0;

        m_imgQCSBLHD.pData = NULL; m_imgQCSBLHD.nSize = 0;
        m_imgOEMSBL.pData = NULL; m_imgOEMSBL.nSize = 0;
        m_imgOEMSBLHD.pData = NULL; m_imgOEMSBLHD.nSize = 0;
        m_imgAMSS.pData = NULL; m_imgAMSS.nSize = 0;
        m_imgAMSSHD.pData = NULL; m_imgAMSSHD.nSize = 0;
        m_imgCEFS.pData = NULL; m_imgCEFS.nSize = 0;
        m_imgDU.pData = NULL; m_imgDU.nSize = 0;
        SetDownloadSettingParameters();
        OpenAllImage();
        TCHAR szMyDocumentPath[512] = {0};
        SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_DEFAULT, szMyDocumentPath);
        CString strLogPath = CString(szMyDocumentPath) + CString("\\Log");
        bool bDirectoryCreated = CreateDirectory(strLogPath, NULL);
        CString strDownloadLogPath = CString(szMyDocumentPath) + CString("\\Log\\QDownloadLog.txt");
        m_bLogOpened = m_logFile.Open(strDownloadLogPath, CFile::modeWrite | CFile::modeCreate);
        if(m_bLogOpened)
            m_logFile.Write(g_strStartLog, strlen(g_strStartLog));
    }

    CDownloadUIDlgForLavernock::~CDownloadUIDlgForLavernock() {
        if(m_pLayoutManager)
            delete m_pLayoutManager;
        if(m_imgHEX.pData != NULL)
            delete m_imgHEX.pData;
        if(m_imgPartition.pData != NULL)
            delete m_imgPartition.pData;
        if(m_imgQCSBL.pData != NULL)
            delete m_imgQCSBL.pData;
        if(m_imgQCSBLHD.pData != NULL) 
            delete m_imgQCSBLHD.pData;
        if(m_imgOEMSBL.pData != NULL)
            delete m_imgOEMSBL.pData;
        if(m_imgOEMSBLHD.pData != NULL)
            delete m_imgOEMSBLHD.pData;
        if(m_imgAMSS.pData != NULL)
            delete m_imgAMSS.pData;
        if(m_imgAMSSHD.pData != NULL)
            delete m_imgAMSSHD.pData;
        if(m_imgCEFS.pData != NULL)
            delete m_imgCEFS.pData;
        if(m_bLogOpened) {
            m_logFile.Write(g_strEndLog, strlen(g_strEndLog));
            m_logFile.Close();
        }
    }

    void CDownloadUIDlgForLavernock::DoDataExchange(CDataExchange* pDX) {
        CDialog::DoDataExchange(pDX);
    }

    BEGIN_MESSAGE_MAP(CDownloadUIDlgForLavernock, Win32DoubleBufferQDialogWidge)
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

    BOOL CDownloadUIDlgForLavernock::OnInitDialog() {
        CDialog::OnInitDialog();
        this->SetXMLUI();
        CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST1);
        CString szVersion = CString(_T("QDownload Tool ")) + VERSION;
        pListBox->InsertString(pListBox->GetCount(), szVersion.GetBuffer(0));
        pListBox->InsertString(pListBox->GetCount(), _T("This version is for Lavernock..."));
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

    void CDownloadUIDlgForLavernock::OnPaint() {
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
    HCURSOR CDownloadUIDlgForLavernock::OnQueryDragIcon() {
        return static_cast<HCURSOR>(m_hIcon);
    }

    void CDownloadUIDlgForLavernock::SetXMLUI(void) {
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

    void CDownloadUIDlgForLavernock::SetBackground(TSTRING strBkImage) {
        SetBKImage(_T("BK_Download.png"),BK_Fixed);
        const int nWIDTH_AboutBox = GetBKImage()->GetWidth(); 
        const int nHeight_AboutBox = GetBKImage()->GetHeight();
        SetMinWidth(nWIDTH_AboutBox);
        SetMinHeight(nHeight_AboutBox);
        SetMaxWidth(nWIDTH_AboutBox);
        SetMaxHeight(nHeight_AboutBox);
    }

    void CDownloadUIDlgForLavernock::WinClose() {
        GetDeviceFinder()->StopMonitor();
        PostMessage(WM_CLOSE);
        //FreeLibrary(m_hDll);
    }

    UINT DownloadThread(LPVOID pParam) {
        CDownloadUIDlgForLavernock* pDlg = (CDownloadUIDlgForLavernock*)pParam;
        pDlg->DownloadFunc();
        return 1;
    }

    void CDownloadUIDlgForLavernock::GetStepString(CStringA& strMsg) {
        if(strMsg.Find("DMSSOpenAndWriteHexFile success") != -1) {
            m_nStep = 1; // the 1st light
        } else if(strMsg.Find("StreamingPartition success") != -1) {
            m_nStep = 2; // the 2nd light
        } else if(strMsg.Find("********** [Image] QCSBL **********") != -1) {
            m_nStep = 3;
        } else if(strMsg.Find("********** [Image] OEMSBLHD/OEMSBL **********") != -1) {
            m_nStep = 4;
        } else if(strMsg.Find("********** [Image] (AMSSHD)/AMSS **********") != -1) {
            m_nStep = 5;
        } else if(strMsg.Find("********** [Image] CEFS **********") != -1) {
            m_nStep = 6;
        } else if(strMsg.Find("********** [Image] DU **********") != -1) {
            m_nStep = 7;
        }  else if(strMsg.Find("ReOpenAfterReset success") != -1 || strMsg.Find("StreamingReset success") != -1) {
            m_nStep = 8;
        }
        //} else if(strMsg.Find("StreamingReset success") != -1) {
        //    m_nStep = 8;
        //}
    }

    // This event is used for download progress, please see the CShareDownload6240
    void CDownloadUIDlgForLavernock::Event(const std::string& strEvent,long nParam) {
        if(strEvent == EVENT_DL_PROGRESS) {
            int nProcentage = ((DL_PROGRESS*)nParam)->nProgress;
            CStringA strPercent;
            DL_PROGRESS* dlMsg = (DL_PROGRESS*)nParam;
            strPercent.Format("%s %d%%",((DL_PROGRESS*)nParam)->szMsg, nProcentage);
            if(m_bEnableLog && m_bLogOpened) {
                m_logFile.Write(strPercent.GetString(), strPercent.GetLength());
                m_logFile.Write("\r\n", 2);
                m_logFile.Flush();
            }
            DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, strPercent.GetString(), -1, NULL, 0);
            wchar_t *pwText = new wchar_t[dwNum];
            MultiByteToWideChar(CP_ACP, 0, strPercent.GetString(), -1, pwText, dwNum);
            InsertStringList(pwText, _T("Downloading..."));
            delete[] pwText;
            GetStepString(strPercent);
            if(strPercent.Find("StreamingOpenMultiImage success") != -1) return;
            UpdateUIProgressBar("", 0, nProcentage);
        }
    }

    // This event is used for Mobile finder or any wstring event
    void CDownloadUIDlgForLavernock::Event(const TSTRING& strEvent, long nParam) {
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

    void CDownloadUIDlgForLavernock::OnEventButtonClick(long nParam) {
        if(m_pWinCloseBtn->GetID() == nParam) {
            WinClose();
        } else if(m_pWinUpdateBtn->GetID() == nParam) {
            if(m_bDownloadStart) {
                MessageBox(_T("Download Thread already Open, Please Wait!"));
            } else {
                m_pWinUpdateBtn->SetEnable(false);
                m_pWinCloseBtn->SetEnable(false);
                // clear all lights
                for(int i = 0; i < 8; ++i)
                    UpdateStepInformation(m_pStep + i, 1);
                CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO1);
                m_nSelectCom = (int)pCombo->GetItemData(pCombo->GetCurSel());
                AfxBeginThread(DownloadThread, (LPVOID) this, THREAD_PRIORITY_NORMAL);
            }
        }
    }
    void CDownloadUIDlgForLavernock::OnEventConnected(long nParam) {
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
    }

    void CDownloadUIDlgForLavernock::OnEventDisconnect(long nParam) {
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
        m_pWinStatusLabel->SetDisplayString(_T("Disconnedted..."));
        m_pWinUpdateBtn->SetEnable((!m_bDownloadStart) && (m_nComNum > 0));
    }

    void CDownloadUIDlgForLavernock::OnSizing(UINT fwSide, LPRECT pRect) {
        //Win32DoubleBufferFormCWndWidge<CQDialog>::OnSizing(fwSide, pRect);
        GeometryRect rcClient = GetGeometry();
        CQDialog::SetWindowRegion(pRect, rcClient);
        // TODO: Add your message handler code here
    }

    int CDownloadUIDlgForLavernock::CheckBatteryStage() {
        int nRet = IDYES;
        if(ReadBatteryVoltage() < 3600) {
            nRet = AfxMessageBox(_T("The battery is not enough to download.\nAre you sure to download?"), MB_YESNO, MB_ICONINFORMATION);
            if(nRet == IDNO) {
                InsertStringList(_T("User cancelled download procedure."), _T(""));
                m_pWinUpdateBtn->SetEnable(true);
                m_pWinCloseBtn->SetEnable(true);
                m_bDownloadStart = false;
            }
        }
        return nRet;
    }

    bool CDownloadUIDlgForLavernock::SetAllImageParameters() {
        int nRet = _taccess(m_strIni.GetString(), 0);
        if(nRet == -1) return false;

    }

    void CDownloadUIDlgForLavernock::DownloadStage() {
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
        CShareDownload6245 download(m_nSelectCom);
        if(m_imgHEX.pData != NULL)
            download.SetHex(m_imgHEX.pData, m_imgHEX.nSize);
        if(m_imgPartition.pData != NULL)
            download.SetPartition(m_imgPartition.pData, m_imgPartition.nSize);
        if(m_imgQCSBL.pData != NULL)
            download.SetQCSBL(m_imgQCSBL.pData, m_imgQCSBL.nSize);
        if(m_imgQCSBLHD.pData != NULL) 
            download.SetQCSBLHD(m_imgQCSBLHD.pData, m_imgQCSBLHD.nSize);
        if(m_imgOEMSBL.pData != NULL)
            download.SetOEMSBL(m_imgOEMSBL.pData, m_imgOEMSBL.nSize);
        if(m_imgOEMSBLHD.pData != NULL)
            download.SetOEMSBLHD(m_imgOEMSBLHD.pData, m_imgOEMSBLHD.nSize);
        if(m_imgAMSS.pData != NULL)
            download.SetAMSS(m_imgAMSS.pData, m_imgAMSS.nSize);;
        if(m_imgAMSSHD.pData != NULL)
            download.SetAMSSHD(m_imgAMSSHD.pData, m_imgAMSSHD.nSize);
        if(m_imgCEFS.pData != NULL)
            download.SetCEFS(m_imgCEFS.pData, m_imgCEFS.nSize);

        download.Register(this, EVENT_DL_PROGRESS);
        bool bContinueDownload = download.Download(true);
        if(bContinueDownload == false) {
            UpdateStepInformation(m_pStep + m_nStep, 2);
        }
        download.Unregister(this, EVENT_DL_PROGRESS);
        FinalReboot();
    }

    void CDownloadUIDlgForLavernock::FinalReboot() {
        TCHAR szBuf[16] = {0};
        DWORD dwResult = ::GetPrivateProfileString(_T("Setting"), _T("Reboot"), _T("0"), szBuf, 16, m_strIni.GetString());
        if(*szBuf == '0') {
            //OnLineMode(m_nSelectCom);
            PowerOff(m_nSelectCom);
        } else {
            while(true) {
                OnLineMode(m_nSelectCom);
                OnLineMode(m_nSelectCom);
                ChangeOnline(m_nSelectCom);
                ChangeOnline(m_nSelectCom);
                int nFTMValue = 0;
                CheckFTM(m_nSelectCom, nFTMValue);
                if(nFTMValue == 0)
                    break;
            }
            PowerOff(m_nSelectCom);
        }
    }

    bool CDownloadUIDlgForLavernock::CheckVariantVersion() {
        int nVer = 0;
        GetLavernockHWVersion(m_nSelectCom, nVer);
        CString strHandsetVariantVersion, strQDownloadVariantVersion;
        strHandsetVariantVersion.Format(_T("GLAVS%dA"), nVer);
        CFileFind ff;
        BOOL bWorking = ff.FindFile(_T("*.qvcefs"));
        while (bWorking)
        {
            bWorking = ff.FindNextFile();
            strQDownloadVariantVersion = ff.GetFileName();
            if(strQDownloadVariantVersion.Find(strHandsetVariantVersion.GetString()) != -1)
                break;
        }
        strQDownloadVariantVersion = strQDownloadVariantVersion.Mid(0, 7);
        InsertStringList(strHandsetVariantVersion);
        return strHandsetVariantVersion.CompareNoCase(strQDownloadVariantVersion) == 0;
    }

    void CDownloadUIDlgForLavernock::DownloadFunc() {
        bool bDownloadOK = true;
        // Check battery
        if(!m_bIsInDLMode)
            if(CheckBatteryStage() == IDNO) return;

        CTime t = CTime::GetCurrentTime();
        CString szTime;
        szTime.Format(_T("Download Start at: %02d:%02d:%02d"), t.GetHour(), t.GetMinute(), t.GetSecond());
        InsertStringList(szTime, _T(""));

        if(bDownloadOK) {
            InsertStringList(_T("Get ME info, Please wait......"), _T("Checking ..."));
            if(!GetInfoFromME()) {
                InsertStringList(_T("Can not Get Info From ME! Please try again!"));
                bDownloadOK = false;
            } else {
                if(CheckVariantVersion() == false) {
                    int nRet = AfxMessageBox(_T("The variant versions are different\nbetween the handset and the software.\nAre you sure to continue?"), MB_YESNO|MB_ICONQUESTION);
                    if(nRet == IDNO) {
                        InsertStringList(_T("User cancel!!"));
                        return;
                    }
                }
                int nVer = 0;
                GetLavernockHWVersion(m_nSelectCom, nVer);
                CString strVariantVersion;
                strVariantVersion.Format(_T("Your Phone: GLAVS%dA"), nVer);
                InsertStringList(strVariantVersion);
            }
        }

        if(bDownloadOK) {
            DownloadStage();
            //Sleep(30000);
            //InsertStringList(_T("Restore BRT..."));
            //char szBuf[512] = {0};
            //GetCurrentDirectoryA(512, szBuf);
            //CStringA szBrtPath = CStringA(szBuf) + CStringA("\\Lavernock_GLAVS1A_EVT2_QV003.brt");
            //if(RestoreBRT2NV(m_nSelectCom, szBrtPath.GetString())) {
            //    InsertStringList(_T("Restore BRT Complete..."));
            //} else {
            //    InsertStringList(_T("Restore BRT Failed..."));
            //}
            m_bDownloadStart = false;
            for(int i = 0; i < m_nStep; ++i)
                UpdateStepInformation(m_pStep + i, 0);
        }
        //int nBackuptimes = 0;
        CTime time = CTime::GetCurrentTime();
        //bool bRestored = false;
        //if(CheckNVPartition(m_nSelectCom, nBackuptimes, time)) {
        //    InsertStringList(_T("Restore NV partition..."));
        //    bRestored = RestoreNVPartition(m_nSelectCom);
        //} else {
        //    InsertStringList(_T("The NV partition is incorrect..."));
        //}
        //ASSERT(bRestored);
        //OnLineMode(m_nSelectCom);
        //OnLineMode(m_nSelectCom);
        //ChangeOnline(m_nSelectCom);
        //ChangeOnline(m_nSelectCom);
        //PowerOff(m_nSelectCom);
        t = CTime::GetCurrentTime();
        szTime.Format(_T("Download End at: %02d:%02d:%02d"),t.GetHour(),t.GetMinute(),t.GetSecond());
        InsertStringList(szTime, _T("Download End"));
        m_pWinUpdateBtn->SetEnable(true);
        m_pWinCloseBtn->SetEnable(true);

        //FinalReboot();
    }

    void CDownloadUIDlgForLavernock::FindBRTFile(CStringA& strOutput) {
        CFileFind finder;
        BOOL bWorking = finder.FindFile(_T("*.brt"));
        if(bWorking)
            bWorking = finder.FindNextFile();
        if(bWorking) {
            DWORD dwMinSize = WideCharToMultiByte(CP_OEMCP,NULL,finder.GetFileName().GetString(),-1,NULL,0,NULL,FALSE);
            char* szBuf = new char[dwMinSize+2];
            memset(szBuf, 0, dwMinSize+2);
            WideCharToMultiByte(CP_OEMCP,NULL,finder.GetFileName().GetString(),-1,szBuf,dwMinSize,NULL,FALSE);
            strOutput = szBuf;
            delete [] szBuf;
        }
    }

    void CDownloadUIDlgForLavernock::Initialize() {
        assert(m_pWinStatusLabel != NULL);
        assert(m_pWinUpdateBtn != NULL);
        m_pWinStatusLabel->SetDisplayString(_T("Connecting..."));
        m_nPreStep = 0;
        m_nPreProgress = 0;
        m_strPreMessage = L"";
        m_bDownloadStart = false;
        m_nComNum = 0;
        m_nSelectCom = -1;
        GetDeviceFinder()->Register(this);
        GetDeviceFinder()->StartMonitor();
    }

    bool CDownloadUIDlgForLavernock::GetInfoFromME() {
        //CQisdaMiscellaneousModule *pMsModule = new CQisdaMiscellaneousModule(m_nSelectCom);
        char szSW[100] ={0};
        char szHW[100] ={0};
        int  nHWVer = 0;
        GetLavernockHWVersion(m_nSelectCom, nHWVer);
        if(GetSWHWVersion(m_nSelectCom, szSW, szHW)) {
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
        //delete pMsModule;
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

    bool CDownloadUIDlgForLavernock::CallBack(const char* szMsg, int nStep, int nProgress) {
        g_pThis->UpdateUIProgressBar(szMsg, nStep, nProgress);
        return true;
    }

    void CDownloadUIDlgForLavernock::UpdateStepInformation(WIN32PictureViewerWidge** pStep, int nType) {
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

    bool CDownloadUIDlgForLavernock::UpdateUIProgressBar(const char* szMsg, int nStep, int nProgress) {
        if(nProgress == 100 && m_nStep != 0) {
            if(m_nStep != 8) {
                for(int i = 0; i < m_nStep; ++i)
                    UpdateStepInformation(m_pStep + i, 0);
            }
            //if(m_nStep == 8) {
            //    m_pWinUpdateBtn->SetEnable(true);
            //    m_pWinCloseBtn->SetEnable(true);
            //}
        }

        static Win32ProgressBarWidge *pProgressBar = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar"));
        if(pProgressBar != NULL)
            pProgressBar->UpdatePercent(nProgress);
        return true;
    }
    void CDownloadUIDlgForLavernock::OnContextMenu(CWnd* pWnd, CPoint point) {
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

    void CDownloadUIDlgForLavernock::OnBackGround() {
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

    void CDownloadUIDlgForLavernock::OnImage() {
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

    void CDownloadUIDlgForLavernock::OnOption() {
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

    void CDownloadUIDlgForLavernock::OnAboutBox() {
        CAboutBox AboutBox;
        AboutBox.DoModal();
    }

    bool CDownloadUIDlgForLavernock::GetDLPathFromIni(const TSTRING& strIniPath, const TSTRING& strName, TSTRING& strPath) {
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

    void CDownloadUIDlgForLavernock::InsertStringList(CString szMessage, CString szLabelInfo) {
        CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST1);
        pList->InsertString(pList->GetCount(), szMessage);
        m_pWinStatusLabel->SetDisplayString(szLabelInfo.GetBuffer(0));
        pList->SetCurSel(pList->GetCount()-1);
    }

    bool CDownloadUIDlgForLavernock::SetIniPath() {
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

    bool CDownloadUIDlgForLavernock::CheckSignedCode() {
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

    bool CDownloadUIDlgForLavernock::ExtractSpecifiedString(const TSTRING& strFileName, char* szCheckCode, int nCheckLength, int nExtractLength, int nIndexToGet, char* nOutput, int nOutputLength) {
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

    int CDownloadUIDlgForLavernock::GetCodeSecurityVersion(const TSTRING& strFileName, char* szCheckCode, int nCheckLength) {
        char szOutput[10] = {0};
        ExtractSpecifiedString(strFileName, szCheckCode, nCheckLength, 100, 5, szOutput, 10);
        int nRet = atoi((const char*)szOutput);
        return nRet;
    }

    bool CDownloadUIDlgForLavernock::GetCodeVersion(const TSTRING& strFileName, char* szCheckCode, int nCheckLength, CString& strVer) {
        char szOutput[10] = {0};
        ExtractSpecifiedString(strFileName, szCheckCode, nCheckLength, 100, 2, szOutput, 10);
        strVer.Format(_T("%S"), szOutput);
        return true;
    }

    bool CDownloadUIDlgForLavernock::GetCodeSigned(const TSTRING& strFileName, char* szCheckCode, int nCheckLength) {
        char szOutput[10] = {0};
        ExtractSpecifiedString(strFileName, szCheckCode, nCheckLength, 100, 4, szOutput, 10);
        return *szOutput == 'P' ? true : false;
    }

    // Check QFuse if mobile in FTM
    bool CDownloadUIDlgForLavernock::CheckQFuseState1() {
        unsigned long reg1 = 0, reg2 = 0;
        bool bSuccess = false;
        ReadQFuseState(m_nSelectCom, reg1, reg2, bSuccess);
        //m_pFnReadQFuse(m_nSelectCom, reg1, reg2, bSuccess);
        if(bSuccess == false) return false;
        return !(reg1 == 6 && reg2 == 0);
    }
    // Check QFuse after change FTM
    bool CDownloadUIDlgForLavernock::CheckQFuseState2() {
        return false;
    }
    // Check QFuse after write NV 453 (FTM)
    bool CDownloadUIDlgForLavernock::CheckQFuseState3() {
        ChangeFTM(m_nSelectCom);
        SetOffline(m_nSelectCom);
        ResetHandset(m_nSelectCom);
        Sleep(25000);
        bool bRet = CheckQFuseState1();
        return bRet;
    }

    bool CDownloadUIDlgForLavernock::GetVariantString(CString& strVariantString) {
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

    void CDownloadUIDlgForLavernock::ShowVariantVersion() {
        CString strVariantString;
        GetVariantString(strVariantString);
        if(strVariantString.GetLength() != 0) {
            strVariantString = CString(_T("Variant: ")) + strVariantString;
            InsertStringList(strVariantString, _T(""));
        }
    }

    void CDownloadUIDlgForLavernock::ShowSoftwareLoadVersion() {
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

    void CDownloadUIDlgForLavernock::ShowSignedUnsignedCode() {
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

    int CDownloadUIDlgForLavernock::ReadBatteryVoltage() {
        InsertStringList(_T("Checking Battery voltage..."), _T(""));
        int nVoltage = 0;
        ::ReadBatteryVoltage(m_nSelectCom, nVoltage);
        //m_pFnReadBatteryVoltage(m_nSelectCom, nVoltage);
        TCHAR* szShowString = (nVoltage > 3600) ? _T("Battery voltage is enough to download...") : _T("Battery voltage is not enough to download...");
        InsertStringList(szShowString, _T(""));
        return nVoltage;
    }

    void CDownloadUIDlgForLavernock::SetDownloadSettingParameters() {
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
            dwResult = ::GetPrivateProfileString(_T("Setting"), _T("EnableLog"), _T("0"), szBuf, 16, m_strIni.GetString());
            m_bEnableLog = (szBuf[0] == '1' ? true : false);
        }
    }

    bool CDownloadUIDlgForLavernock::OpenImage(IMAGE & imgLoad, CString strLoadName) {
        TCHAR szCurrentFolder[MAX_PATH] = {0};
        ::GetCurrentDirectory(MAX_PATH, szCurrentFolder);
        m_strIni = CString(szCurrentFolder) + _T("\\Download.ini");
        ASSERT(_taccess(m_strIni.GetString(), 0) == 0);

        TCHAR szLoadPath[MAX_PATH] = {0};
        GetPrivateProfileString(_T("path"), strLoadName.GetString(), _T(""), szLoadPath, MAX_PATH, m_strIni.GetString());
        CString strLoadFileName = CString(szCurrentFolder) + CString(_T("\\")) + CString(szLoadPath);
        bool bLoadExist = (_taccess(strLoadFileName.GetString(), 0) == 0);
        if(!bLoadExist) return false;
        CFile fLoad;
        BOOL bOpened = fLoad.Open(strLoadFileName.GetString(), CFile::modeRead);
        if(imgLoad.pData == NULL && bOpened != FALSE) {
            imgLoad.pData = new BYTE[fLoad.GetLength()+2];
            memset(imgLoad.pData, 0, fLoad.GetLength()+2);
            imgLoad.nSize = fLoad.GetLength();
            fLoad.Read(imgLoad.pData, fLoad.GetLength());
        }
        return bLoadExist & bOpened;
    }

    bool CDownloadUIDlgForLavernock::OpenAllImage() {
        bool bAMSSExist = OpenImage(m_imgAMSS, _T("amss"));
        bool bAMSSEHDxist = OpenImage(m_imgAMSSHD, _T("amsshd"));
        bool bCEFSExist = OpenImage(m_imgCEFS, _T("cefs"));
        bool bHEXExist = OpenImage(m_imgHEX, _T("hex"));
        bool bQCSBLExist = OpenImage(m_imgQCSBL, _T("qcsbl"));
        bool bQCSBLHDExist = OpenImage(m_imgQCSBLHD, _T("qcsblhd"));
        bool bOEMSBLExist = OpenImage(m_imgOEMSBL, _T("oemsbl"));
        bool bOEMSBLHDExist = OpenImage(m_imgOEMSBLHD, _T("oemsblhd"));
        bool bPartitionExist = OpenImage(m_imgPartition, _T("partition"));
        return bAMSSExist & bAMSSEHDxist & bCEFSExist & bHEXExist &
            bQCSBLExist & bQCSBLHDExist & bOEMSBLExist & bOEMSBLHDExist &
            bPartitionExist;
    }
    int CDownloadUIDlgForLavernock::WriteSpecialDUforCNVersion(int nComPort) {
        typedef int (__cdecl *pfRunDuckLib)(const unsigned int);
        typedef int (__cdecl *pfShutdownDuckLib)();
        typedef int (__cdecl *pfDuck_NVDownload)(const TCHAR*);
        InsertStringList(_T("Restore DU..."));
        HMODULE hDll = ::LoadLibrary(_T("Duck_StaticM.dll"));
        ASSERT(hDll);
        pfRunDuckLib RunDuckLib           = (pfRunDuckLib)     GetProcAddress(hDll, "RunDuckLib");
        ASSERT(RunDuckLib);
        pfShutdownDuckLib ShutdownDuckLib = (pfShutdownDuckLib)GetProcAddress(hDll, "ShutdownDuckLib");
        ASSERT(ShutdownDuckLib);
        pfDuck_NVDownload Duck_NVDownload = (pfDuck_NVDownload)GetProcAddress(hDll, "Duck_NVDownload");
        ASSERT(Duck_NVDownload);

        if(RunDuckLib(nComPort) != 0){
            return -1;
        }

        if(Duck_NVDownload((LPCTSTR)"NV_List.du") != 0){
            return -1;
        }
        InsertStringList(_T("Restore DU Complete..."));
        ShutdownDuckLib();
        FreeLibrary(hDll);
        m_pWinUpdateBtn->SetEnable(true);
        m_pWinCloseBtn->SetEnable(true);
        return 0;
    }
}

#pragma warning(pop)