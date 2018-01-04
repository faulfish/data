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
#include "..\..\..\Code\Modules\QisdaMiscellaneousModule\QisdaMiscellaneousModule.h"
#include "..\..\..\Code\Modules\QisdaNVModule\NVItem.h"
#include "../../FactoryTool/MobileToolKit/MobileToolKit.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(push)
#pragma warning(disable: 4311)
#pragma warning(disable: 4312)
#pragma warning(disable: 4996)

// CDownloadUIDlg dialog

#define VERSION _T("Totally QDownload for All Projects")

namespace QSYNC_LIB {

    char* g_strStartLog = "----------QDownload Start Log----------\r\n";
    char* g_strEndLog   = "-------------End Start Log-------------";

    static CTotallyQDownload* g_pThis = NULL;
    CTotallyQDownload::CTotallyQDownload(CWnd* pParent /*=NULL*/) :Win32DoubleBufferQDialogWidge(CTotallyQDownload::IDD, pParent)
        , m_nStep(0), m_bEnableLog(false), m_dif(NULL), m_pLayoutManager(NULL), m_pMainLayout(NULL), m_bConnected(false)
        , m_pWinUpdateBtn(NULL), m_pWinCloseBtn(NULL), m_pWinLanguageBtn(NULL), m_pWinStatusLabel(NULL)
    {
        m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
        CreateILocalization(&m_pILocalization);
        g_pThis = this;
        TCHAR szMyDocumentPath[512] = {0};
        SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_DEFAULT, szMyDocumentPath);
        CString strLogPath = CString(szMyDocumentPath) + CString("\\Log");
        BOOL bDirectoryCreated = CreateDirectory(strLogPath, NULL);
        CString strDownloadLogPath = CString(szMyDocumentPath) + CString("\\Log\\QDownloadLog.txt");
        m_bLogOpened = m_logFile.Open(strDownloadLogPath, CFile::modeWrite | CFile::modeCreate);
        if(m_bLogOpened)
            m_logFile.Write(g_strStartLog, (UINT)strlen(g_strStartLog));
    }

    CTotallyQDownload::~CTotallyQDownload() {
        if(m_pLayoutManager)
            delete m_pLayoutManager;
        if(m_bLogOpened) {
            m_logFile.Write(g_strEndLog, (UINT)strlen(g_strEndLog));
            m_logFile.Close();
        }
    }

    void CTotallyQDownload::DoDataExchange(CDataExchange* pDX) {
        CDialog::DoDataExchange(pDX);
    }

    BEGIN_MESSAGE_MAP(CTotallyQDownload, Win32DoubleBufferQDialogWidge)
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

    BOOL CTotallyQDownload::OnInitDialog() {
        CDialog::OnInitDialog();
        SetXMLUI();
        CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST1);
        CString szVersion = CString(_T("QDownload Tool ")) + VERSION;
        pListBox->InsertString(pListBox->GetCount(), szVersion.GetBuffer(0));
        pListBox->InsertString(pListBox->GetCount(), _T("This version is generally for all handsets"));
        SetWindowText(szVersion);
        SetIcon(m_hIcon, TRUE);			// Set big icon
        SetIcon(m_hIcon, FALSE);		// Set small icon
        Initialize();
        m_pWinUpdateBtn->SetEnable(false);
        m_pWinCloseBtn->SetEnable(true);
        SetTimer(1, 500, &QSYNC_LIB::CTotallyQDownload::TimerProcFunc);
        return TRUE;  // return TRUE  unless you set the focus to a control
    }

    // If you add a minimize button to your dialog, you will need the code below
    //  to draw the icon.  For MFC applications using the document/view model,
    //  this is automatically done for you by the framework.

    void CTotallyQDownload::OnPaint() {
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
    HCURSOR CTotallyQDownload::OnQueryDragIcon() {
        return static_cast<HCURSOR>(m_hIcon);
    }

    void CTotallyQDownload::SetXMLUI(void) {
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

    void CTotallyQDownload::SetBackground(TSTRING strBkImage) {
        SetBKImage(_T("BK_Download.png"),BK_Fixed);
        const int nWIDTH_AboutBox = GetBKImage()->GetWidth(); 
        const int nHeight_AboutBox = GetBKImage()->GetHeight();
        SetMinWidth(nWIDTH_AboutBox);
        SetMinHeight(nHeight_AboutBox);
        SetMaxWidth(nWIDTH_AboutBox);
        SetMaxHeight(nHeight_AboutBox);
    }

    void CTotallyQDownload::WinClose() {
        GetDeviceFinder()->StopMonitor();
        KillTimer(1);
        PostMessage(WM_CLOSE);
        //FreeLibrary(m_hDll);
    }

    UINT DownloadThread(LPVOID pParam) {
        CTotallyQDownload* pDlg = (CTotallyQDownload*)pParam;
        pDlg->DownloadFunc();
        return 1;
    }

    // This event is used for Mobile finder or any wstring event
    void CTotallyQDownload::Event(const TSTRING& strEvent, long nParam) {
        if(strEvent == EVENT_BUTTON_CLICK) {
            OnEventButtonClick(nParam);
        } else if(strEvent == EVENT_SUBMENU) {
            //m_pILocalization->SetRegion(strEvent);
        } else if(strEvent == DEVICE_DIAG_CONNECT) {
            OnEventConnected(nParam);
        } else if(strEvent == DEVICE_DIAG_DISCONNECT) {
            OnEventDisconnect(nParam);
        } else if(strEvent == EVENT_TOTALLY_CALLBACK) {
            TCHAR* szMessage = (TCHAR*)nParam;
            InsertStringList(szMessage, _T("Downloading..."));
        } else if(strEvent == EVENT_TOTALLY_PROGRESS) {
            UpdateUIProgressBar("", 0, nParam);
        } else if(strEvent == EVENT_TOTALLY_STEP_LIGHT) {
            m_nStep = nParam;
        } else if(strEvent == EVENT_TOTALLY_GET_VARIANT_STR) {
            CString strMessage = CString(_T("Software Variant: ")) + (TCHAR*)nParam;
            InsertStringList(strMessage.GetString(), _T(""));
        }
    }

    void CTotallyQDownload::OnEventButtonClick(long nParam) {
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
    void CTotallyQDownload::OnEventConnected(long nParam) {
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
            m_nSelectCom = nData;
        }
        m_nComNum++;
        if(m_nComNum == 1)
            pCombo->SetCurSel(0);
        m_pWinUpdateBtn->SetEnable((!m_bDownloadStart) && (m_nComNum > 0));
        m_pWinStatusLabel->SetDisplayString(_T("Connected..."));
        m_bConnected = true;
    }

    void CTotallyQDownload::OnEventDisconnect(long nParam) {
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
        m_pWinStatusLabel->SetDisplayString(_T("Disconnected..."));
        m_bConnected = false;
    }

    void CTotallyQDownload::OnSizing(UINT fwSide, LPRECT pRect) {
        //Win32DoubleBufferFormCWndWidge<CQDialog>::OnSizing(fwSide, pRect);
        GeometryRect rcClient = GetGeometry();
        CQDialog::SetWindowRegion(pRect, rcClient);
        // TODO: Add your message handler code here
    }

    int CTotallyQDownload::CheckBatteryStage() {
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

    void CTotallyQDownload::DownloadStage() {
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
        if(m_strHWVersion == _T("Lavernock") && m_dif == NULL)
            m_dif = new CDownload6245Logic(m_nSelectCom);
        else if(m_strHWVersion == _T("Melos") && m_dif == NULL)
            m_dif = new CDownload6240Logic(m_nSelectCom);
        else if(m_strHWVersion == _T("")) {
            Platform pNum = DownloadInterface::GetPlatform();
            if(m_dif == NULL) {
                switch (pNum)
                {
                case NPRG6260:
                    m_dif = new CDownload6245Logic(m_nSelectCom);
                    break;
                case NPRG6240:
                case NPRG6270:
                    m_dif = new CDownload6240Logic(m_nSelectCom);
                    break;
                case UNKNOWN:
                    TRACE("Unknown Paltform...\n");
                    ASSERT(0);
                }
            }
        }
        if(m_dif == NULL) return;
        m_dif->SetImageParameters();
        m_dif->Register(this, EVENT_TOTALLY_CALLBACK);
        m_dif->Register(this, EVENT_TOTALLY_PROGRESS);
        m_dif->Register(this, EVENT_TOTALLY_STEP_LIGHT);
        m_dif->Register(this, EVENT_TOTALLY_GET_VARIANT_STR);
        m_dif->PreDownload();
        bool bContinueDownload = m_dif->StartDownload();
        m_dif->PostDownload();
        m_dif->Unregister(this, EVENT_TOTALLY_CALLBACK);
        m_dif->Unregister(this, EVENT_TOTALLY_PROGRESS);
        m_dif->Unregister(this, EVENT_TOTALLY_STEP_LIGHT);
        m_dif->Unregister(this, EVENT_TOTALLY_GET_VARIANT_STR);

        if(bContinueDownload == false) {
            UpdateStepInformation(m_pStep + m_nStep, 2);
        }

        FinalReboot();
        delete m_dif;
        m_dif = NULL;
    }

    void CTotallyQDownload::FinalReboot() {
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
            m_bDownloadStart = false;
        }
    }

    void CTotallyQDownload::DownloadFunc() {
        bool bDownloadOK = true;
        m_bDownloadStart = true;
        bool bInDownloadMode = IsInDownloadMode(m_nSelectCom);
        // Check battery
        if(!bInDownloadMode) {
            if(CheckBatteryStage() == IDNO) return;
        }
        
        if(bDownloadOK && !bInDownloadMode) {
            InsertStringList(_T("Get ME info, Please wait......"), _T("Checking ..."));
            if(!GetInfoFromME()) {
                InsertStringList(_T("Can not Get Info From ME! Please try again!"));
                bDownloadOK = false;
            }
        }
        // show the start time
        CTime t = CTime::GetCurrentTime();
        CString szTime;
        szTime.Format(_T("Download Start at: %02d:%02d:%02d"), t.GetHour(), t.GetMinute(), t.GetSecond());
        InsertStringList(szTime, _T(""));

        if(bDownloadOK) {
            DownloadStage();
        }
        
        // show the end time
        t = CTime::GetCurrentTime();
        szTime.Format(_T("Download End at: %02d:%02d:%02d"),t.GetHour(),t.GetMinute(),t.GetSecond());
        InsertStringList(szTime, _T("Download End"));
        m_bDownloadStart = false;

        //m_pWinUpdateBtn->SetEnable(true);
        //m_pWinCloseBtn->SetEnable(true);
    }

    void CTotallyQDownload::Initialize() {
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

    bool CTotallyQDownload::GetInfoFromME() {
        CQisdaMiscellaneousModule *pMsModule = new CQisdaMiscellaneousModule(m_nSelectCom);
        char szSW[100] ={0};
        char szHW[100] ={0};
        if(pMsModule->GetSWHWVersion(szSW, szHW)) {
            TCHAR szUnicodeInfor[4096] = {0};
            CString szString;

            UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szHW,-1,szUnicodeInfor,4096);	
            m_strHWVersion = szUnicodeInfor;
            szString.Format( _T("Your Phone HW: %s"), szUnicodeInfor);
            InsertStringList(szString, _T("Checking ..."));

            iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szSW,-1,szUnicodeInfor,4096);	
            m_strSWVersion = szUnicodeInfor;
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
            m_strVariantVersion = szUnicodeInfor;
            CString szString;
            szString.Format(_T("Your Phone Variant: %s"), szUnicodeInfor);
            InsertStringList(szString, _T("Checking ..."));
        } else {
            InsertStringList( _T("Can not get Variant version from ME"), _T("Checking Fail"));
        }
        delete pNVModule;
        return true;
    }

    bool CTotallyQDownload::CallBack(const char* szMsg, int nStep, int nProgress) {
        if(g_pThis == NULL) return false;
        g_pThis->UpdateUIProgressBar(szMsg, nStep, nProgress);
        return true;
    }

    void CTotallyQDownload::UpdateStepInformation(WIN32PictureViewerWidge** pStep, int nType) {
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

    bool CTotallyQDownload::UpdateUIProgressBar(const char* szMsg, int nStep, int nProgress) {
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
    void CTotallyQDownload::OnContextMenu(CWnd* pWnd, CPoint point) {
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

    void CTotallyQDownload::OnBackGround() {
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

    void CTotallyQDownload::OnImage() {
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

    void CTotallyQDownload::OnOption() {
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

    void CTotallyQDownload::OnAboutBox() {
        CAboutBox AboutBox;
        AboutBox.DoModal();
    }

    bool CTotallyQDownload::GetDLPathFromIni(const TSTRING& strIniPath, const TSTRING& strName, TSTRING& strPath) {
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

    void CTotallyQDownload::InsertStringList(CString szMessage, CString szLabelInfo) {
        CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST1);
        pList->InsertString(pList->GetCount(), szMessage);
        m_pWinStatusLabel->SetDisplayString(szLabelInfo.GetBuffer(0));
        pList->SetCurSel(pList->GetCount()-1);
    }

    bool CTotallyQDownload::SetIniPath() {
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

    bool CTotallyQDownload::CheckSignedCode() {
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

    bool CTotallyQDownload::ExtractSpecifiedString(const TSTRING& strFileName, char* szCheckCode, int nCheckLength, int nExtractLength, int nIndexToGet, char* nOutput, int nOutputLength) {
        CFile file;
        BOOL bOpen = file.Open(strFileName.c_str(), CFile::modeRead | CFile::typeBinary);
        if(!bOpen) return false;
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

    int CTotallyQDownload::GetCodeSecurityVersion(const TSTRING& strFileName, char* szCheckCode, int nCheckLength) {
        char szOutput[10] = {0};
        ExtractSpecifiedString(strFileName, szCheckCode, nCheckLength, 100, 5, szOutput, 10);
        int nRet = atoi((const char*)szOutput);
        return nRet;
    }

    bool CTotallyQDownload::GetCodeVersion(const TSTRING& strFileName, char* szCheckCode, int nCheckLength, CString& strVer) {
        char szOutput[10] = {0};
        ExtractSpecifiedString(strFileName, szCheckCode, nCheckLength, 100, 2, szOutput, 10);
        strVer.Format(_T("%S"), szOutput);
        return true;
    }

    bool CTotallyQDownload::GetCodeSigned(const TSTRING& strFileName, char* szCheckCode, int nCheckLength) {
        char szOutput[10] = {0};
        ExtractSpecifiedString(strFileName, szCheckCode, nCheckLength, 100, 4, szOutput, 10);
        return *szOutput == 'P' ? true : false;
    }

    // Check QFuse if mobile in FTM
    bool CTotallyQDownload::CheckQFuseState1() {
        unsigned long reg1 = 0, reg2 = 0;
        bool bSuccess = false;
        ReadQFuseState(m_nSelectCom, reg1, reg2, bSuccess);
        //m_pFnReadQFuse(m_nSelectCom, reg1, reg2, bSuccess);
        if(bSuccess == false) return false;
        return !(reg1 == 6 && reg2 == 0);
    }
    // Check QFuse after change FTM
    bool CTotallyQDownload::CheckQFuseState2() {
        return false;
    }
    // Check QFuse after write NV 453 (FTM)
    bool CTotallyQDownload::CheckQFuseState3() {
        ChangeFTM(m_nSelectCom);
        SetOffline(m_nSelectCom);
        ResetHandset(m_nSelectCom);
        Sleep(25000);
        bool bRet = CheckQFuseState1();
        return bRet;
    }

    bool CTotallyQDownload::GetVariantString(CString& strVariantString) {
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

    void CTotallyQDownload::ShowVariantVersion() {
        CString strVariantString;
        GetVariantString(strVariantString);
        if(strVariantString.GetLength() != 0) {
            strVariantString = CString(_T("Variant: ")) + strVariantString;
            InsertStringList(strVariantString, _T(""));
        }
    }

    void CTotallyQDownload::ShowSoftwareLoadVersion() {
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

    void CTotallyQDownload::ShowSignedUnsignedCode() {
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

    int CTotallyQDownload::ReadBatteryVoltage() {
        InsertStringList(_T("Checking Battery voltage..."), _T(""));
        int nVoltage = 0;
        ::ReadBatteryVoltage(m_nSelectCom, nVoltage);
        //m_pFnReadBatteryVoltage(m_nSelectCom, nVoltage);
        TCHAR* szShowString = (nVoltage > 3500) ? _T("Battery voltage is enough to download...") : _T("Battery voltage is not enough to download...");
        InsertStringList(szShowString, _T(""));
        return nVoltage;
    }

    void CALLBACK CTotallyQDownload::TimerProcFunc(HWND hwnd, UINT iMsg, UINT_PTR TimerID, DWORD Time) {
        static int i = 0;
        if(g_pThis == NULL) return;
        g_pThis->m_pWinUpdateBtn->SetEnable(g_pThis->m_bConnected && !g_pThis->m_bDownloadStart);
        g_pThis->m_pWinCloseBtn->SetEnable(!g_pThis->m_bDownloadStart);
        switch(i++) {
            case 0:
                g_pThis->m_pWinStatusLabel->SetTextColor(255, 255, 255, 0);
                break;
            case 1:
                g_pThis->m_pWinStatusLabel->SetTextColor(255, 0, 255, 255);
                break;
        }
        g_pThis->m_pWinStatusLabel->Update(true);
        i = i % 2;
    }
}

#pragma warning(pop)