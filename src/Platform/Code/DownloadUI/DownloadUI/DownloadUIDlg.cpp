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
#include "..\..\..\Code\odmtma_api\QDownload_Export\Download_Export.h"
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

#define VERSION _T("1.0.0.7")

namespace QSYNC_LIB {

    CDownloadUIDlg* g_pThis = NULL;
    CDownloadUIDlg::CDownloadUIDlg(CWnd* pParent /*=NULL*/) :Win32DoubleBufferQDialogWidge(CDownloadUIDlg::IDD, pParent) {
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
        }
    }

    void CDownloadUIDlg::DoDataExchange(CDataExchange* pDX) {
        CDialog::DoDataExchange(pDX);
    }

    BEGIN_MESSAGE_MAP(CDownloadUIDlg, Win32DoubleBufferQDialogWidge)
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

    BOOL CDownloadUIDlg::OnInitDialog() {
        CDialog::OnInitDialog();
        this->SetXMLUI();
        CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST1);
        CString szVersion = CString(_T("QDownload Tool ")) + VERSION;
        pListBox->InsertString(pListBox->GetCount(),szVersion.GetBuffer(0));
        SetWindowText(szVersion);
        SetIcon(m_hIcon, TRUE);			// Set big icon
        SetIcon(m_hIcon, FALSE);		// Set small icon
        this->Initialize();
        ShowVariantVersion();
        ShowSoftwareLoadVersion();
        ShowSignedUnsignedCode();
        m_pWinUpdateBtn->SetEnable(false);
        m_pWinCloseBtn->SetEnable(true);
        return TRUE;  // return TRUE  unless you set the focus to a control
    }

    // If you add a minimize button to your dialog, you will need the code below
    //  to draw the icon.  For MFC applications using the document/view model,
    //  this is automatically done for you by the framework.

    void CDownloadUIDlg::OnPaint() {
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
    HCURSOR CDownloadUIDlg::OnQueryDragIcon() {
        return static_cast<HCURSOR>(m_hIcon);
    }

    void CDownloadUIDlg::SetXMLUI(void) {
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

    void CDownloadUIDlg::SetBackground(TSTRING strBkImage) {
        SetBKImage(_T("BK_Download.png"),BK_Fixed);
        const int nWIDTH_AboutBox = GetBKImage()->GetWidth(); 
        const int nHeight_AboutBox = GetBKImage()->GetHeight();
        SetMinWidth(nWIDTH_AboutBox);
        SetMinHeight(nHeight_AboutBox);
        SetMaxWidth(nWIDTH_AboutBox);
        SetMaxHeight(nHeight_AboutBox);
    }

    void CDownloadUIDlg::WinClose() {
        GetDeviceFinder()->StopMonitor();
        PostMessage(WM_CLOSE);
        //FreeLibrary(m_hDll);
    }

    UINT DownloadThread(LPVOID pParam) {
        CDownloadUIDlg* pDlg = (CDownloadUIDlg*)pParam;
        pDlg->DownloadFunc();
        return 1;
    }

    void CDownloadUIDlg::Event(const TSTRING& strEvent,long nParam) {
        if(strEvent == EVENT_BUTTON_CLICK) {
            if(m_pWinCloseBtn->GetID() == nParam) {
                WinClose();
            } else if(m_pWinUpdateBtn->GetID() == nParam) {
                if(m_bDownloadStart) {
                    MessageBox(_T("Download Thread already Open, Please Wait!"));
                } else {
                    m_bDownloadStart = true;
                    m_pWinUpdateBtn->SetEnable(false);
                    m_pWinCloseBtn->SetEnable(false);
                    CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO1);
                    m_nSelectCom = (int)pCombo->GetItemData(pCombo->GetCurSel());

                    AfxBeginThread(DownloadThread, (LPVOID) this, THREAD_PRIORITY_NORMAL);
                }
            }
        } else if(strEvent == EVENT_SUBMENU) {
            m_pILocalization->SetRegion(strEvent);
        } else if(strEvent == DEVICE_DIAG_CONNECT) {
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
            m_nComNum++;
            if(m_nComNum == 1)
                pCombo->SetCurSel(0);
            m_pWinUpdateBtn->SetEnable((!m_bDownloadStart) && (m_nComNum > 0));
            m_pWinStatusLabel->SetDisplayString(_T("Connected..."));
        } else if(strEvent == DEVICE_DIAG_DISCONNECT) {
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
        } else {
            //ASSERT(0);
        }
    }

    void CDownloadUIDlg::OnSizing(UINT fwSide, LPRECT pRect) {
        //Win32DoubleBufferFormCWndWidge<CQDialog>::OnSizing(fwSide, pRect);
        GeometryRect rcClient = GetGeometry();
        CQDialog::SetWindowRegion(pRect, rcClient);
        // TODO: Add your message handler code here
    }

    void CDownloadUIDlg::Language() {
        m_QCommonControlBase_Language.SetSubMenu(&m_LanguageSubmenu);
        for(int i =0;i < m_LanguageSubmenu.GetMenuItemCount();i++) {
            TSTRING strDisplay,strNotifyID;
            m_LanguageSubmenu.GetMenuItem(i,strDisplay,strNotifyID);
            m_QCommonControlBase_Language.Register(this,strNotifyID);
        }
        GeometryRect rcLangBtn = m_pWinLanguageBtn->GetGeometry();
        RECT rcMainFrame = {0};
        GetClientRect(&rcMainFrame);
        ClientToScreen(&rcMainFrame);
        QPoint ptPoint;
        ptPoint.m_nPosX = rcMainFrame.left + rcLangBtn.m_nX;
        ptPoint.m_nPosY = rcMainFrame.top + rcLangBtn.m_nY + rcLangBtn.m_nHeight;
        m_QCommonControlBase_Language.Event(EVENT_SUBMENU,(long)&ptPoint);
    }

    int CDownloadUIDlg::WarnDownloadMode(bool& bFused, bool& bSignedCode) {
        CString message;
        message = bSignedCode ? _T("The handset is in DL or Emerency DL mode.\nThe software load is SIGNED.\nAre you sure continuing to download?") : _T("The handset is in DL or Emerency DL mode.\nThe software load is UNSIGNED.\nAre you sure continuing to download?");
        int nRet = AfxMessageBox(message, MB_YESNO, MB_ICONINFORMATION);
        if(nRet == IDYES) {
            bFused = false;         ///< for sure the code will be download
            bSignedCode = false;    ///< for sure the code will be download
        } else if(nRet == IDNO) {
            InsertStringList(_T("User cancel download procedure."), _T(""));
            m_pWinUpdateBtn->SetEnable(true);
            m_pWinCloseBtn->SetEnable(true);
            m_bDownloadStart = false;
        }
        return nRet;
    }

    bool CDownloadUIDlg::CheckQFuseStage(bool bFused) {
        int nFTMValue = 0;
        CheckFTM(m_nSelectCom, nFTMValue);
        //m_pFnCheckFTM(m_nSelectCom, nFTMValue);
        if(!bFused && nFTMValue == 1)
            bFused = CheckQFuseState1();    ///< if the value is true, the mobile is fused
        //if(!bFused && nFTMValue == 0)
        //    bFused = CheckQFuseState2();
        if(!bFused && nFTMValue == 0)
            bFused = CheckQFuseState3();
        return bFused;
    }

    void CDownloadUIDlg::CheckRevocationStage(int &nAmssVersion, int &nOemsblVersion, bool &CheckVersion) {
        TSTRING strAmss, strOemsbl;
        GetDLPathFromIni(m_strIni.GetString(), _T("amss"), strAmss);
        GetDLPathFromIni(m_strIni.GetString(), _T("oemsbl"), strOemsbl);
        if(strAmss.length() != 0)
            nAmssVersion = GetCodeSecurityVersion(strAmss, "O'zapft", 7);
        if(strOemsbl.length() != 0)
            nOemsblVersion = GetCodeSecurityVersion(strOemsbl, "O'zbplt", 7);
        CheckVersion = CheckSWHWVersion(m_nSelectCom, nAmssVersion, nOemsblVersion, 3000);
    }

    int CDownloadUIDlg::CheckBatteryStage() {
        int nRet = IDYES;
        if(ReadBatteryVoltage() < 3500) {
            int nRet = AfxMessageBox(_T("The bettery is not enough to download.\nAre you sure to download?"), MB_YESNO, MB_ICONINFORMATION);
            if(nRet == IDNO) {
                InsertStringList(_T("User cancel download procedure."), _T(""));
                m_pWinUpdateBtn->SetEnable(true);
                m_pWinCloseBtn->SetEnable(true);
                m_bDownloadStart = false;
            }
        }
        return nRet;
    }

    void CDownloadUIDlg::DownloadStage(bool bCheckVersion) {
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
        DownloadParameter dp = {0};
        dp.bBackupBRT = m_strBackupBRT == _T("1");
        dp.bBackupNV = m_strBackupNVPartition == _T("1");
        dp.bRestoreBRT = m_strRestoreBRT == _T("1");
        dp.bRestoreNV = m_strRestoreNVPartition == _T("1");
        dp.bDownloadDU = m_strDownloadDU == _T("1");
        //bool bDownload = Download(m_nSelectCom, CallBack, szIni.GetBuffer(0), bCheckVersion);
        bool bDownload = Download(m_nSelectCom, CallBack, szIni.GetBuffer(0), &dp);
        TCHAR* szShowString = bDownload ? _T("Download Success") : _T("Download Failed");
        InsertStringList(szShowString, szShowString);
    }

    void CDownloadUIDlg::FinalReboot() {
        TCHAR szBuf[16] = {0};
        DWORD dwResult = ::GetPrivateProfileString(_T("Setting"), _T("Reboot"), _T("0"), szBuf, 16, m_strIni.GetString());
        if(*szBuf == '0') {
            OnLineMode(m_nSelectCom);
            PowerOff(m_nSelectCom);
            //m_pFnOnlineMode(m_nSelectCom);
            //m_pFnPowerOff(m_nSelectCom);
        } else {
            ChangeOnline(m_nSelectCom);
            PowerOff(m_nSelectCom);
            //m_pFnChangeOnline(m_nSelectCom);
            //m_pFnPowerOff(m_nSelectCom);
        }
        //m_pFnResetHandset(m_nSelectCom);
    }

    void CDownloadUIDlg::DownloadFunc() {
        bool bDownloadOK = true;
        bool bSignedCode = CheckSignedCode();   ///< if the value is true, the code is signed
        if(CheckBatteryStage() == IDNO) return;

        bool bDLMode = IsDLMode(m_nSelectCom);
        bool bFused = false, bCheckVersion = false;
        int nAmssVersion = 0, nOemsblVersion = 0;
        if(bDLMode) {
            // If the handset is in Emergency DL, we won't know the mobile is QFused or not.
            // Ask the user to download or not
            if(WarnDownloadMode(bFused, bSignedCode) == IDNO) return;
        } else {
            // Read FTM NV item first, then check QFuse step 1 can be ignore
            bFused = CheckQFuseStage(bFused);
            CheckRevocationStage(nAmssVersion, nOemsblVersion, bCheckVersion);
        }
        if(bFused) {
            // Download only signed code
            bDownloadOK = bSignedCode;
            if(!bDownloadOK) {
                InsertStringList(_T("The handset is signed..."), _T(""));
                InsertStringList(_T("The software is not signed..."), _T(""));
            }
        } else {
            // Download signed or unsigned code
            bDownloadOK = true;
        }
        CTime t = CTime::GetCurrentTime();
        CString szTime;
        szTime.Format(_T("Download Start at: %02d:%02d:%02d"), t.GetHour(), t.GetMinute(), t.GetSecond());
        InsertStringList(szTime, _T(""));

        if(bDownloadOK && !bDLMode) {
            InsertStringList(_T("Get ME info, Please wait......"), _T("Checking ..."));
            if(!GetInfoFromME()) {
                InsertStringList(_T("Can not Get Info From ME! Please try again!"));
                bDownloadOK = false;
            }
        }

        if(bDownloadOK) {
            DownloadStage(bCheckVersion);
        }
        t = CTime::GetCurrentTime();
        szTime.Format(_T("Download End at: %02d:%02d:%02d"),t.GetHour(),t.GetMinute(),t.GetSecond());
        InsertStringList(szTime, _T("Download End"));
        FinalReboot();
        char *szInfo = (!bDownloadOK) ? "Download Fail" : "Download Success";
        LogFn(szInfo, 10, 0);
    }

    void CDownloadUIDlg::Initialize() {
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

    bool CDownloadUIDlg::GetInfoFromME() {
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
            InsertStringList(szString, _T("Checking ..."));
        } else {
            InsertStringList( _T("Can not get Variant version from ME"), _T("Checking Fail"));
        }
        delete pNVModule;
        return true;
    }

    bool CDownloadUIDlg::CallBack(const char* szMsg, int nStep, int nProgress) {
        g_pThis->LogFn(szMsg, nStep, nProgress);
        return true;
    }

    void CDownloadUIDlg::UpdateStepInformation(WIN32PictureViewerWidge** pStep) {
        (*pStep)->SetImagePath(_T("Btn_Success.png"));
        (*pStep)->Update();
    }

    bool CDownloadUIDlg::LogFn(const char* szMsg, int nStep, int nProgress) {
        TCHAR szUnicodeInfor[4096] = {0};
        CString szLabelInfo, szMessage, szPath;
        UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szMsg,-1,szUnicodeInfor,4096);	
        szMessage.Format(_T("%s, Percentage: %d"), szUnicodeInfor, nProgress);
        szLabelInfo = szUnicodeInfor;
        if(szLabelInfo == _T("Download Success") || szLabelInfo == _T("Download Fail") ||
            szLabelInfo == _T("Download Failed!") || szLabelInfo == _T("Download End") ||
            szLabelInfo == _T("Do not download..."))
        {
            m_bDownloadStart = false;
            m_pWinUpdateBtn->SetEnable(m_nComNum > 0);
            m_pWinCloseBtn->SetEnable(true);
        }

        static int nPreviousProgress = -1;
        if(nPreviousProgress == nProgress && nProgress != 100)
            return true;
        else
            nPreviousProgress = nProgress;
        static CListBox *pList = (CListBox*)GetDlgItem(IDC_LIST1);

        szPath = _T("Btn_Success.png");

        switch(nStep) {
        case 1:
            szLabelInfo = _T("Open Com Port");
            break;
        case 2:
            szLabelInfo = _T("Backup");
            UpdateStepInformation(m_pStep);
            break;
        case 3:
            szLabelInfo = _T("Change To DL Mode");
            UpdateStepInformation(m_pStep+1);
            break;
        case 4:
            szLabelInfo = _T("Hex File Download");
            UpdateStepInformation(m_pStep+2);
            break;
        case 5:
            szLabelInfo = _T("Streaming Download");
            UpdateStepInformation(m_pStep+3);
            break;
        case 6:
            szLabelInfo = _T("Re-open Com Port");
            UpdateStepInformation(m_pStep+4);
            break;
        case 7:
            szLabelInfo = _T("Restore");
            UpdateStepInformation(m_pStep+5);
            break;
        case 8:
            szLabelInfo = _T("Download DU");
            UpdateStepInformation(m_pStep+6);
            break;
        default:
            szLabelInfo = _T("Download End");
            UpdateStepInformation(m_pStep+7);
            break;
        }

        //if(m_strPreMessage != szUnicodeInfor)
        InsertStringList(szMessage, szLabelInfo);
        //else
        //	m_strPreMessage = szUnicodeInfor;

        static Win32ProgressBarWidge *pProgressBar = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar"));
        if(pProgressBar != NULL)
            pProgressBar->UpdatePercent(nProgress);
        return true;
    }
    void CDownloadUIDlg::OnContextMenu(CWnd* pWnd, CPoint point) {
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
        PopMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
    }

    void CDownloadUIDlg::OnBackGround() {
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

    void CDownloadUIDlg::OnImage() {
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

    void CDownloadUIDlg::OnOption() {
        CString szPath;
        ::GetModuleFileName(NULL, szPath.GetBuffer(MAX_PATH), MAX_PATH);
        szPath.ReleaseBuffer();
        szPath = szPath.Left(szPath.ReverseFind(_T('\\'))+1);
        COptionDlg dlg(szPath);
        if(dlg.DoModal() == IDOK) {
            //CString szPath;
            //::GetModuleFileName(NULL, szPath.GetBuffer(MAX_PATH), MAX_PATH);
            //szPath.ReleaseBuffer();
            //szPath = szPath.Left(szPath.ReverseFind('\\')+1);
            //szPath = szPath+L"Download.ini"; 
        }
    }

    void CDownloadUIDlg::OnAboutBox() {
        CAboutBox AboutBox;
        AboutBox.DoModal();
    }

    bool CDownloadUIDlg::GetDLPathFromIni(const TSTRING& strIniPath, const TSTRING& strName, TSTRING& strPath) {
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

    void CDownloadUIDlg::InsertStringList(CString szMessage, CString szLabelInfo) {
        CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST1);
        pList->InsertString(pList->GetCount(), szMessage);
        m_pWinStatusLabel->SetDisplayString(szLabelInfo.GetBuffer(0));
        pList->SetCurSel(pList->GetCount()-1);
    }

    bool CDownloadUIDlg::SetIniPath() {
        CString szPath;
        ::GetModuleFileName(NULL, szPath.GetBuffer(MAX_PATH), MAX_PATH);
        szPath.ReleaseBuffer();
        szPath = szPath.Left(szPath.ReverseFind(_T('\\'))+1);
        char cstrPath[MAX_PATH] ={0};
        UINT iUTF8CharCount = WideCharToMultiByte(CP_ACP,FALSE,szPath.GetBuffer(0),szPath.GetLength(), cstrPath,MAX_PATH,NULL,NULL);	
        CStringA szIni ;
        szIni.Format("%sDownload.ini", cstrPath);
        int nRet = _access(szIni.GetString(), 0);
        m_strIni = szIni;
        return !(ENOENT == nRet);
    }

    bool CDownloadUIDlg::CheckSignedCode() {
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

    bool CDownloadUIDlg::ExtractSpecifiedString(const TSTRING& strFileName, char* szCheckCode, int nCheckLength, int nExtractLength, int nIndexToGet, char* nOutput, int nOutputLength) {
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

    int CDownloadUIDlg::GetCodeSecurityVersion(const TSTRING& strFileName, char* szCheckCode, int nCheckLength) {
        char szOutput[10] = {0};
        ExtractSpecifiedString(strFileName, szCheckCode, nCheckLength, 100, 5, szOutput, 10);
        int nRet = atoi((const char*)szOutput);
        return nRet;
    }

    bool CDownloadUIDlg::GetCodeVersion(const TSTRING& strFileName, char* szCheckCode, int nCheckLength, CString& strVer) {
        char szOutput[10] = {0};
        ExtractSpecifiedString(strFileName, szCheckCode, nCheckLength, 100, 2, szOutput, 10);
        strVer.Format(_T("%S"), szOutput);
        return true;
    }

    bool CDownloadUIDlg::GetCodeSigned(const TSTRING& strFileName, char* szCheckCode, int nCheckLength) {
        char szOutput[10] = {0};
        ExtractSpecifiedString(strFileName, szCheckCode, nCheckLength, 100, 4, szOutput, 10);
        return *szOutput == 'P' ? true : false;
    }

    // Check QFuse if mobile in FTM
    bool CDownloadUIDlg::CheckQFuseState1() {
        unsigned long reg1 = 0, reg2 = 0;
        bool bSuccess = false;
        ReadQFuseState(m_nSelectCom, reg1, reg2, bSuccess);
        //m_pFnReadQFuse(m_nSelectCom, reg1, reg2, bSuccess);
        if(bSuccess == false) return false;
        return !(reg1 == 6 && reg2 == 0);
    }
    // Check QFuse after change FTM
    bool CDownloadUIDlg::CheckQFuseState2() {
        //HMODULE hDll = LoadLibrary(_T("MobileToolKit_StaticM.dll"));
        //if(hDll == NULL) {
        //    TRACE("Load dll error...\n");
        //    return false;
        //}
        //pFnOnlineMode Online = (pFnOnlineMode)GetProcAddress(hDll, "OnLineMode");
        //pFnFTMMode FTM = (pFnFTMMode)GetProcAddress(hDll, "FTMMode");
        //if(Online != NULL && FTM != NULL) {
        //    Online(m_nSelectCom);
        //    FTM(m_nSelectCom);
        //}
        //bool bRet = CheckQFuseState1();
        //Online(m_nSelectCom);
        //FreeLibrary(hDll);
        return false;
    }
    // Check QFuse after write NV 453 (FTM)
    bool CDownloadUIDlg::CheckQFuseState3() {
        ChangeFTM(m_nSelectCom);
        SetOffline(m_nSelectCom);
        ResetHandset(m_nSelectCom);
        //m_pFnChangeFTM(m_nSelectCom);
        //m_pFnSetOffline(m_nSelectCom);
        //m_pFnResetHandset(m_nSelectCom);
        Sleep(25000);
        bool bRet = CheckQFuseState1();
        return bRet;
    }

    bool CDownloadUIDlg::GetVariantString(CString& strVariantString) {
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

    void CDownloadUIDlg::ShowVariantVersion() {
        CString strVariantString;
        GetVariantString(strVariantString);
        if(strVariantString.GetLength() != 0) {
            strVariantString = CString(_T("Variant: ")) + strVariantString;
            InsertStringList(strVariantString, _T(""));
        }
    }

    void CDownloadUIDlg::ShowSoftwareLoadVersion() {
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

    void CDownloadUIDlg::ShowSignedUnsignedCode() {
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

    int CDownloadUIDlg::ReadBatteryVoltage() {
        InsertStringList(_T("Checking Battery voltage..."), _T(""));
        int nVoltage = 0;
        ::ReadBatteryVoltage(m_nSelectCom, nVoltage);
        //m_pFnReadBatteryVoltage(m_nSelectCom, nVoltage);
        TCHAR* szShowString = (nVoltage > 3500) ? _T("Battery voltage is enough to download...") : _T("Battery voltage is not enough to download...");
        InsertStringList(szShowString, _T(""));
        return nVoltage;
    }
}

#pragma warning(pop)