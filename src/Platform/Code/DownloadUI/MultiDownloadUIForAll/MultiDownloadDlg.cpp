// MultiDownloadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MultiDownload.h"
#include "MultiDownloadDlg.h"

#include "../../../code/Components/UIFramework/interface/QControlNotify.h"
#include "../../../code/components/UIFramework/layoutmanager/Win32ProgressBarWidge.h"
#include "../../../code/CommonUtil\Win32SHELLUtility.h"
#include "../../../code/CommonUtil\Win32ThreadUtility.h"
#include <fstream>
#include "..\..\Components\MobileFinder\MobileFinderInterface.h"
#include "..\..\..\code\odmtma_api\SerCOMLib\SerCOMLibrary.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
#pragma warning(disable:4996)


// CMultiDownloadDlg dialog

static int nLoopCount = 200;

namespace QSYNC_LIB
{
    CMultiDownloadForAllDlg* g_pThis = NULL;

    UINT DownloadThread(LPVOID pParam)
    {
        ((CDownloadFuncForAll *)pParam)->DownloadProcess();
        return 1;
    }
    CMultiDownloadForAllDlg::CMultiDownloadForAllDlg(CWnd* pParent /*=NULL*/)
        :Win32DoubleBufferQDialogWidge(CMultiDownloadForAllDlg::IDD, pParent),
        m_pLayoutManager(0), m_unAvailCom(0), m_unTotalCom(0), m_unButtonEnNum(0),
        m_sz1FF(_T("")), m_strFolderPath(_T("")), m_pWinBrowse(0), m_pWinStartAll(0),
        m_pWinStart1(0), m_pWinStart2(0), m_pWinStart3(0), m_pWinStart4(0),
        m_pWinStart5(0), m_pWinStart6(0), m_pWinStart7(0), m_pWinStart8(0)
    {
        g_pThis = this;
        m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
        for(int i = 0; i < 8; i++) {
            m_DownloadFunc[i]= NULL;
            m_DownloadProgress[i] = 0;
            m_pWinStarts[i] = NULL;
        }	

        m_pCallBackFunc[0] = CallBack1;
        m_pCallBackFunc[1] = CallBack2;
        m_pCallBackFunc[2] = CallBack3;
        m_pCallBackFunc[3] = CallBack4;
        m_pCallBackFunc[4] = CallBack5;
        m_pCallBackFunc[5] = CallBack6;
        m_pCallBackFunc[6] = CallBack7;
        m_pCallBackFunc[7] = CallBack8;
    }

    void CMultiDownloadForAllDlg::DoDataExchange(CDataExchange* pDX) {
        CDialog::DoDataExchange(pDX);
    }

    BEGIN_MESSAGE_MAP(CMultiDownloadForAllDlg, Win32DoubleBufferQDialogWidge)
        ON_WM_PAINT()
        ON_WM_QUERYDRAGICON()
        //}}AFX_MSG_MAP
        ON_CBN_SELCHANGE(IDC_COMBO9, OnSelchangecom9)
        ON_WM_DESTROY()
        ON_MESSAGE(WM_DL_NOTIFY, OnDLNotify)
    END_MESSAGE_MAP()

    void CMultiDownloadForAllDlg::Set1FFFileToComboBox() {
        m_szFileArray.RemoveAll();
        int nCount = 0;
        for(int i = 0; i < 10; i++) {
            CString szFile, szKey;
            szKey.Format(_T("1FF %d"), i);
            szFile = AfxGetApp()->GetProfileString(_T("MultiDownload"), szKey.GetBuffer());
            if(szFile == _T("")) break;

            int j = 0;
            for(j = 0; j < m_szFileArray.GetCount(); j++)
                if(m_szFileArray.GetAt(j) == szFile)
                    break;

            if(j == m_szFileArray.GetCount()) {
                m_szFileArray.InsertAt(nCount, szFile);
                CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO9);
                pCombo->InsertString(nCount, szFile);
                pCombo->SetItemData(nCount, nCount);
                nCount++;
            }
        }
    }

    void CMultiDownloadForAllDlg::SetIniFilePath() {
        CString strINIFolder;
        TCHAR pszPath[MAX_PATH] = {0};
        if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, pszPath)))  {
            strINIFolder = CString(pszPath) + CString(_T("\\MultiDownload\\"));
            ::CreateDirectory(m_strFolderPath, NULL);
        } else {
            assert(0 && _T("Get Shell Folder Error!!!"));
        }
        m_strIni = strINIFolder + _T("Download.ini");
    }

    BOOL CMultiDownloadForAllDlg::OnInitDialog() {
        CDialog::OnInitDialog();

        SetIcon(m_hIcon, TRUE);			// Set big icon
        SetIcon(m_hIcon, FALSE);		// Set small icon

        // TODO: Add extra initialization here
        SetXMLUI();
        m_pWinStartAll->SetEnable(false);
        GetDeviceFinder()->StartMonitor();
        GetDeviceFinder()->Register(this);
        for(int i = 0; i < 8; i++) {
            GetComboBox(i)->EnableWindow(false);
            EnableStartButton(i,false);
        }
        Set1FFFileToComboBox();
        m_szComArray.RemoveAll();   // remove all comport
        SetIniFilePath();
        return TRUE;  // return TRUE  unless you set the focus to a control
    }

    // If you add a minimize button to your dialog, you will need the code below
    //  to draw the icon.  For MFC applications using the document/view model,
    //  this is automatically done for you by the framework.

    void CMultiDownloadForAllDlg::OnPaint() {
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
    HCURSOR CMultiDownloadForAllDlg::OnQueryDragIcon() {
        return static_cast<HCURSOR>(m_hIcon);
    }

    void CMultiDownloadForAllDlg::SetXMLUI(void) {
        if(m_pLayoutManager == NULL) {
            SetBKImage(_T("BK_Download.png"),BK_Fixed);
            const int nWIDTH_AboutBox = GetBKImage()->GetWidth(); 
            const int nHeight_AboutBox = GetBKImage()->GetHeight();
            SetMinWidth(nWIDTH_AboutBox);
            SetMinHeight(nHeight_AboutBox);
            SetMaxWidth(nWIDTH_AboutBox);
            SetMaxHeight(nHeight_AboutBox);
            m_pLayoutManager = new CLayoutManager(this, this, _T("MultiDownloadLayout.xml"));
        }

        m_pMainLayout = (SplitLayout<HDC>*)m_pLayoutManager->Load(_T("CMultiDownloadDlgID"));

        m_pWinBrowse = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("BrowserButton"));
        m_pWinStartAll = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("StartAll"));
        m_pWinStarts[0] = m_pWinStart1 = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("Start1"));
        m_pWinStarts[1] = m_pWinStart2 = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("Start2"));
        m_pWinStarts[2] = m_pWinStart3 = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("Start3"));
        m_pWinStarts[3] = m_pWinStart4 = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("Start4"));
        m_pWinStarts[4] = m_pWinStart5 = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("Start5"));
        m_pWinStarts[5] = m_pWinStart6 = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("Start6"));
        m_pWinStarts[6] = m_pWinStart7 = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("Start7"));
        m_pWinStarts[7] = m_pWinStart8 = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("Start8"));

        SetLayout(m_pMainLayout);
    }

    void CMultiDownloadForAllDlg::OnEventButtonClick(long nParam)
    {
        if(m_pWinBrowse->GetID() == nParam) //Search 1FF file
            Browse1FFFile();
        else //Start All/Start 1 ~ Start 8
        {
            m_uiDownloadActive = -1;
            if(m_pWinStart1->GetID() == nParam)
                m_uiDownloadMask = DOWNLOAD_BUTTON_1;
            else if(m_pWinStart2->GetID() == nParam)
                m_uiDownloadMask = DOWNLOAD_BUTTON_2;
            else if(m_pWinStart3->GetID() == nParam)
                m_uiDownloadMask = DOWNLOAD_BUTTON_3;
            else if(m_pWinStart4->GetID() == nParam)
                m_uiDownloadMask = DOWNLOAD_BUTTON_4;
            else if(m_pWinStart5->GetID() == nParam)
                m_uiDownloadMask = DOWNLOAD_BUTTON_5;
            else if(m_pWinStart6->GetID() == nParam)
                m_uiDownloadMask = DOWNLOAD_BUTTON_6;
            else if(m_pWinStart7->GetID() == nParam)
                m_uiDownloadMask = DOWNLOAD_BUTTON_7;
            else if(m_pWinStart8->GetID() == nParam)
                m_uiDownloadMask = DOWNLOAD_BUTTON_8;
            else
                m_uiDownloadMask = DOWNLOAD_BUTTON_ALL;

            DownloadFunc(m_uiDownloadMask);
        }
    }

    void CMultiDownloadForAllDlg::Event(const TSTRING& strEvent,long nParam)
    {
        if(strEvent == EVENT_BUTTON_CLICK) {
            OnEventButtonClick(nParam);
        } else if(strEvent == EVENT_SUBMENU) {
            //m_pILocalization->SetRegion(strEvent);
        } else if(strEvent == DEVICE_DIAG_CONNECT) {
            UpdateComportCombo((*(QSYNC_LIB::DeviceDataPt*)nParam)->ID().c_str());
        } else if(strEvent == DEVICE_DIAG_DISCONNECT) {
            UpdateComportCombo((*(QSYNC_LIB::DeviceDataPt*)nParam)->ID().c_str(), false);
        } else {
            //TSTRING strShowMessage = TSTRING(_T("Please add this event")) + strEvent;
            //ASSERT(0 && strShowMessage.c_str());
        }
    }

    void CMultiDownloadForAllDlg::SetButtonEnabled() {
        if(m_unTotalCom < 9) {
            if(!m_pWinStart1->GetEnable() && (m_DownloadFunc[0] == NULL))
                m_pWinStart1->SetEnable(true);
            else if(!m_pWinStart2->GetEnable() && (m_DownloadFunc[1] == NULL))
                m_pWinStart2->SetEnable(true);
            else if(!m_pWinStart3->GetEnable() && (m_DownloadFunc[2] == NULL))
                m_pWinStart3->SetEnable(true);
            else if(!m_pWinStart4->GetEnable() && (m_DownloadFunc[3] == NULL))
                m_pWinStart4->SetEnable(true);
            else if(!m_pWinStart5->GetEnable() && (m_DownloadFunc[4] == NULL))
                m_pWinStart5->SetEnable(true);
            else if(!m_pWinStart6->GetEnable() && (m_DownloadFunc[5] == NULL))
                m_pWinStart6->SetEnable(true);
            else if(!m_pWinStart7->GetEnable() && (m_DownloadFunc[6] == NULL))
                m_pWinStart7->SetEnable(true);
            else if(!m_pWinStart8->GetEnable() && (m_DownloadFunc[7] == NULL))
                m_pWinStart8->SetEnable(true);
            else
                ASSERT(0);
        }
    }
    // New algorithm for adding comport
    bool CMultiDownloadForAllDlg::NewAddComportToCombo(LPCTSTR lpszComport, UINT &unCount, int iCom) {
        
        return false;
    }

    bool CMultiDownloadForAllDlg::AddComportToCombo(LPCTSTR lpszComport, UINT &unCount, int iCom) {
        for(int i = 0; i < static_cast<int>(m_unTotalCom); i++) {
            if(m_szComArray.GetAt(i) == lpszComport) //already exist
                return false;
        }
        m_unTotalCom++;
        m_szComArray.Add(lpszComport);
        UINT nLoop = (m_unTotalCom < 8) ? m_unTotalCom : 8;
        //if(m_unTotalCom < 8 )
        //    nLoop = m_unTotalCom;

        for(UINT i = 0; i < nLoop; i++) {
            CComboBox *pComboBox = GetComboBox(i);
            unCount = pComboBox->GetCount();
            pComboBox->InsertString(unCount,lpszComport);
            pComboBox->SetItemData(unCount, iCom);

            if(unCount == 0) {
                pComboBox->EnableWindow(TRUE);
                pComboBox->SetCurSel(unCount);
            }
            int j = 0;
            unCount++;
            while(unCount < m_unTotalCom) {
                CString szTemp = m_szComArray.GetAt(j);
                int iCom = _wtoi(szTemp.Mid(3).GetBuffer());
                pComboBox->InsertString(j, szTemp);
                pComboBox->SetItemData(j, iCom);
                unCount = pComboBox->GetCount();
                j++;
            }
        }
        SetButtonEnabled();

        if((m_unButtonEnNum == 0) && (m_unTotalCom >= 1))
            m_pWinStartAll->SetEnable(true);
        return true;
    }

    bool CMultiDownloadForAllDlg::RemoveComportFromCombo(LPCTSTR lpszComport) {
        UINT nLoop = (m_unTotalCom < 8) ? m_unTotalCom : 8;

        for(int i = 0; i < static_cast<int>(m_unTotalCom); i++) {
            if(m_szComArray.GetAt(i) == lpszComport) {
                m_szComArray.RemoveAt(i);
                m_unTotalCom--;
                break;
            }
        }

        for(UINT i = 0; i < nLoop; i++) {
            CComboBox *pComboBox = GetComboBox(i);
            int nIndex = pComboBox->FindString(-1, lpszComport);
            int nCurSel = pComboBox->GetCurSel();
            pComboBox->DeleteString(nIndex);

            if(nCurSel == nIndex && m_unTotalCom <8) {
                pComboBox->EnableWindow(false);
                while(pComboBox->GetCount() > 0)
                    pComboBox->DeleteString(0);
                EnableStartButton(i,false);
            } else if(nCurSel == nIndex) {
                int nCount = pComboBox->GetCount();
                nIndex = 0;
                while(nIndex < nCount)
                {
                    int nCom = static_cast<int>(pComboBox->GetItemData(nIndex));
                    int j = 0;
                    for(j = 0 ; j < 8; j++)
                    {
                        CComboBox *pTempComboBox = (CComboBox*)GetDlgItem(IDC_COMBO1 + j);
                        if(pTempComboBox->GetCount() != -1)
                        {
                            int nCom2 = static_cast<int>(pTempComboBox->GetItemData(pTempComboBox->GetCurSel()));
                            if(nCom2 == nCom)
                                break;
                        }
                    }
                    if(j < 8)
                        nIndex++;
                    else
                    {
                        pComboBox->SetCurSel(nIndex);
                        break;
                    }	
                }
            }
        }
        if((m_unButtonEnNum == 0) && (m_unTotalCom > 1))
            m_pWinStartAll->SetEnable(true);
        return true;
    }


    //Update the com port status
    bool CMultiDownloadForAllDlg::UpdateComportCombo(LPCTSTR lpszComport, bool bAdd)  {
        int iCom = _wtoi(&lpszComport[3]);
        UINT unCount = 0;

        //Check if the slot is downloading, if yes, return false
        for(int i = 0; i < 8; i++) {
            if(m_DownloadFunc[i] != NULL && iCom == m_DownloadFunc[i]->GetComPort())  //download port do nothing
                return false;
        }
        return bAdd ? AddComportToCombo(lpszComport, unCount, iCom) : RemoveComportFromCombo(lpszComport);
    }

    void CMultiDownloadForAllDlg::OnDestroy() {
        GetDeviceFinder()->StopMonitor();
        GetDeviceFinder()->Unregister(this);

        for(int i = 0; i < m_szFileArray.GetCount(); i++) {
            CString szKey;
            szKey.Format(_T("1FF %d"), i);
            AfxGetApp()->WriteProfileString(_T("MultiDownload"), szKey.GetBuffer(), m_szFileArray.GetAt(i));
        }

        CDownloadFuncForAll *pDownloadFunc = new CDownloadFuncForAll(0, 0, m_strIni, m_hWnd, NULL, NULL);
        if(pDownloadFunc != NULL) {
            pDownloadFunc->Set1ffFile(m_sz1FF);
            pDownloadFunc->DeleteUnzippedFolder(m_strFolderPath.GetBuffer(0));
            delete pDownloadFunc;
        }
        __super::OnDestroy();

        // TODO: Add your message handler code here
    }

    CString CMultiDownloadForAllDlg::OpenDialogToBrowse1FFFile() {
        TCHAR szPath[MAX_PATH];
        ::GetCurrentDirectory(MAX_PATH, szPath);
        CFileDialog fd(TRUE, NULL, _T(""), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("1FF Files(*.mot)|*.mot|All Files(*.*)|*.*||"));
        if(fd.DoModal() != IDOK) return _T("");
        ::SetCurrentDirectory(szPath);
        return fd.GetPathName(); // GEt full path name
    }


    CString CMultiDownloadForAllDlg::CheckLoad(CString szTemp) {
        CDownloadFuncForAll downloadFunc(0, 0, m_strIni, m_hWnd, NULL, NULL);
        downloadFunc.Set1ffFile(szTemp);
        return downloadFunc.CheckLoad(downloadFunc.GetPlatform(szTemp));
    }

    void CMultiDownloadForAllDlg::OnSelchangecom9() {
        CComboBox *pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO9);
        CString strSelectedFile = m_szFileArray.GetAt(pComboBox->GetCurSel());
        if((strSelectedFile == m_sz1FF)) return;

        CString szTemp1 = CheckLoad(strSelectedFile);

        if(szTemp1 == _T("")) {
            m_szFileArray.RemoveAt(pComboBox->GetCurSel());
            pComboBox->DeleteString(pComboBox->GetCurSel());

            int nIndex = pComboBox->FindString(-1, m_sz1FF);
            if(nIndex != -1)
                pComboBox->SetCurSel(nIndex);

            MessageBox(_T("Can not Unzip the 1FF file, please check the 1FF file is OK!"));
        } else {
            m_sz1FF = strSelectedFile;
            m_strFolderPath = szTemp1;
        }
    }

    void CMultiDownloadForAllDlg::Browse1FFFile() {
        CComboBox *pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO9);
        CString strSelectedFile = OpenDialogToBrowse1FFFile();
        if(strSelectedFile == m_sz1FF) return;

        CString szTemp = CheckLoad(strSelectedFile);

        if(szTemp == _T("")) {
            int nIndex = pComboBox->FindString(-1, m_sz1FF);
            if(nIndex != -1)
                pComboBox->SetCurSel(nIndex);
            MessageBox(_T("Can not Unzip the 1FF file, please check the 1FF file is OK!"));
        } else {
            int nIndex = pComboBox->FindString(-1, strSelectedFile);
            while(nIndex != -1){
                pComboBox->DeleteString(nIndex);
                m_szFileArray.RemoveAt(nIndex);
                nIndex = pComboBox->FindString(-1, strSelectedFile);
            }

            m_szFileArray.InsertAt(0, strSelectedFile);
            pComboBox->InsertString(0, strSelectedFile);
            pComboBox->SetCurSel(0);
            if(pComboBox->GetCount() > 10)
                pComboBox->DeleteString(10);
            m_sz1FF = strSelectedFile;
            m_strFolderPath = szTemp;
        }
    }

    WIN32IconButton* CMultiDownloadForAllDlg::GetStartButton(int i) {
        if(i > 7) return NULL;
        return m_pWinStarts[i];
    }

    Win32ProgressBarWidge* CMultiDownloadForAllDlg::GetProgressBarWidge(int i) {
        if(i > 7) return NULL;
        CString strTmp;
        strTmp.Format(_T("ProgressBar%d"), i+1);
        return (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(strTmp.GetString());
    }

    void CMultiDownloadForAllDlg::PrepareToDownload(int i, UINT nComport) {
        m_DownloadFunc[i] = new CDownloadFuncForAll(nComport, i, m_strIni, m_hWnd, GetStartButton(i), GetProgressBarWidge(i));
        if(m_DownloadFunc[i] != NULL) {
            GetStartButton(i)->SetEnable(false);
            GetComboBox(i)->EnableWindow(false);
            m_DownloadFunc[i]->Set1ffFile(m_sz1FF);
            m_DownloadFunc[i]->SetCallBackFunc(m_pCallBackFunc[i]);
            m_unButtonEnNum++;
            AfxBeginThread(DownloadThread, (LPVOID)m_DownloadFunc[i], THREAD_PRIORITY_NORMAL);
        }
        else
            MessageBox(_T("New memory failed!"));;
    }

    CComboBox* CMultiDownloadForAllDlg::GetComboBox(int i) {
        return (CComboBox*)GetDlgItem(IDC_COMBO1+i);
    }

    int CMultiDownloadForAllDlg::GetSelectedComPort(int i) {
        return GetComboBox(i)->GetCurSel();
    }

    int CMultiDownloadForAllDlg::GetComPortNum(int i) {
        return GetComboBox(i)->GetItemData(GetSelectedComPort(i));
    }

    void CMultiDownloadForAllDlg::CheckInDownloading(int i, bool& bContinueDownload) {
        for(int j = 0; j < 8; j++) {
            if(j == i || m_DownloadFunc[j] == NULL)
                continue;
            if(m_DownloadFunc[j]->GetComPort() == (UINT)GetComPortNum(i)) {
                MessageBox(_T("The comport is used by other thread!"));
                bContinueDownload = false;
                break;
            }
        }
    }
    void CMultiDownloadForAllDlg::DownloadFunc(unsigned int unDownloadButtonMask) {
        if(m_sz1FF == _T("")) {
            MessageBox( _T("Please select the Source code 1FF file first!"), _T("MultiDownlaod"), MB_OK);
            return;
        }

        for(int i = 0; i < 8; i++) {
            if(!((unDownloadButtonMask & (DOWNLOAD_BUTTON_1 << i)) == (DOWNLOAD_BUTTON_1 << i)))
                continue;
            if(!(GetStartButton(i)->GetEnable() && (m_DownloadFunc[i] == NULL)))
                continue;

            if(GetSelectedComPort(i) == -1) {
                MessageBox(_T("Please select the com port first!"));
            } else {
                bool bContinueDownload = true;
                CheckInDownloading(i, bContinueDownload);
                if(bContinueDownload)
                    PrepareToDownload(i, (UINT)GetComPortNum(i));
            }
        }

        if((m_unButtonEnNum > 6) || ((m_unTotalCom - m_unButtonEnNum) < 2))
            m_pWinStartAll->SetEnable(false);
    }

    void CMultiDownloadForAllDlg::UpdateProgressBarPercentage(int nIndex, int nProgress) {
        if(m_DownloadProgress[nIndex]  != nProgress) {
            Win32ProgressBarWidge *pProgressBar = m_DownloadFunc[nIndex]->GetProgressWnd();
            if(pProgressBar != NULL)
                pProgressBar->UpdatePercent(nProgress);
            m_DownloadProgress[nIndex] = nProgress;
        }
    }

    void CMultiDownloadForAllDlg::UpdateDownloadInformation(int nIndex, const char* szMsg, int nProgress)
    {
        CListBox *pList = (CListBox*)GetDlgItem(IDC_LIST1 + nIndex);
        if(pList == NULL) return;
        CString szMessage;
        szMessage.Format(_T("%s, Percentage: %d"), ConvertInformation(szMsg).GetString(), nProgress);
        pList->InsertString(pList->GetCount(), szMessage);
        pList->SetCurSel(pList->GetCount() -1);
    }

    void CMultiDownloadForAllDlg::UpdateButtonState(int nIndex, const char* szMsg) {

        if(ConvertInformation(szMsg) == _T("Download Successful!") || ConvertInformation(szMsg) == _T("Download Failed!")) {
            WIN32IconButton *pButton = (WIN32IconButton*)m_DownloadFunc[nIndex]->GetButtonWnd();
            pButton->SetEnable(true);
            GetComboBox(nIndex)->EnableWindow(true);

            m_unButtonEnNum--;
            m_pWinStartAll->SetEnable(m_unButtonEnNum == 0);

            delete m_DownloadFunc[nIndex];
            m_DownloadFunc[nIndex] = NULL;
        }
    }

    CString CMultiDownloadForAllDlg::ConvertInformation(const char* szMsg) {
        TCHAR szUnicodeInfor[4096] = {0};
        UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szMsg,-1,szUnicodeInfor,4096);
        return szUnicodeInfor;
    }

    bool CMultiDownloadForAllDlg::UpdateUIBehaviorByMessage(int nIndex,const char* szMsg, int nStep, int nProgress)
    {
        UpdateDownloadInformation(nIndex, szMsg, nProgress);
        UpdateProgressBarPercentage(nIndex, nProgress);
        UpdateButtonState(nIndex, szMsg);
        return true;
    }

    bool CMultiDownloadForAllDlg::CallBack1(const char* szMsg, int nStep, int nProgress)
    {
        return g_pThis->UpdateUIBehaviorByMessage(0,szMsg, nStep, nProgress);
    }

    bool CMultiDownloadForAllDlg::CallBack2(const char* szMsg, int nStep, int nProgress)
    {
        return g_pThis->UpdateUIBehaviorByMessage(1,szMsg, nStep, nProgress);
    }

    bool CMultiDownloadForAllDlg::CallBack3(const char* szMsg, int nStep, int nProgress)
    {
        return g_pThis->UpdateUIBehaviorByMessage(2,szMsg, nStep, nProgress);
    }

    bool CMultiDownloadForAllDlg::CallBack4(const char* szMsg, int nStep, int nProgress)
    {
        return g_pThis->UpdateUIBehaviorByMessage(3,szMsg, nStep, nProgress);
    }

    bool CMultiDownloadForAllDlg::CallBack5(const char* szMsg, int nStep, int nProgress)
    {
        return g_pThis->UpdateUIBehaviorByMessage(4,szMsg, nStep, nProgress);
    }

    bool CMultiDownloadForAllDlg::CallBack6(const char* szMsg, int nStep, int nProgress)
    {
        return g_pThis->UpdateUIBehaviorByMessage(5,szMsg, nStep, nProgress);
    }

    bool CMultiDownloadForAllDlg::CallBack7(const char* szMsg, int nStep, int nProgress)
    {
        return g_pThis->UpdateUIBehaviorByMessage(6,szMsg, nStep, nProgress);
    }

    bool CMultiDownloadForAllDlg::CallBack8(const char* szMsg, int nStep, int nProgress)
    {
        return g_pThis->UpdateUIBehaviorByMessage(7,szMsg, nStep, nProgress);
    }

    LRESULT CMultiDownloadForAllDlg::OnDLNotify(WPARAM wParam, LPARAM lParam)
    {
        UINT nPercentage = 0;
        UINT nStep = 0;
        CString szErrorMsg;

        m_DownloadFunc[(UINT)lParam]->GetDLTaskInfo((UINT)wParam, &szErrorMsg, nStep, nPercentage);

        CListBox *pList = (CListBox*)GetDlgItem(IDC_LIST1 + ((UINT)lParam));
        CString szMessage, szLabelInfo;

        szMessage.Format(_T("%s, Percentage: %d"), szErrorMsg.GetString(), nPercentage);

        pList->InsertString(pList->GetCount(), szMessage);
        pList->SetCurSel(pList->GetCount() -1);

        szLabelInfo = szErrorMsg;
        if(szLabelInfo == _T("Download Successful!") || szLabelInfo == _T("Download Failed!")) {
            WIN32IconButton *pButton = (WIN32IconButton*)m_DownloadFunc[(UINT)lParam]->GetButtonWnd();
            pButton->SetEnable(true);
            m_unButtonEnNum--;
            m_pWinStartAll->SetEnable(m_unButtonEnNum == 0);
            delete m_DownloadFunc[(UINT)lParam];
            m_DownloadFunc[(UINT)lParam] = NULL;
        }

        return 0;
    }

    bool CMultiDownloadForAllDlg::EnableStartButton(int nStartButtonIndex,bool bEnable) {
        if(nStartButtonIndex > 7) {
            ASSERT(false);
            return false;
        }
        m_pWinStarts[nStartButtonIndex]->SetEnable(bEnable);
        return true;
    }

    Platform CMultiDownloadForAllDlg::GetPlatform(CString strFileName) {
        if(strFileName.GetLength() == 0) return PLATFORM_UKNOWN;
        Platform nRet = PLATFORM_UKNOWN;
        if(strFileName.Find(_T("WellFleet")) != -1) 
            nRet = PLATFORM_6245;
        else if(strFileName.Find(_T("Lavernock")) != -1)
            nRet = PLATFORM_6245;
        else if(strFileName.Find(_T("GMELB1A")) != -1)
            nRet = PLATFORM_6240;
        else if(strFileName.Find(_T("GMELB2A")) != -1)
            nRet = PLATFORM_6240;
        else if(strFileName.Find(_T("GMELB3A")) != -1)
            nRet = PLATFORM_6240;
        return nRet;
    }
}