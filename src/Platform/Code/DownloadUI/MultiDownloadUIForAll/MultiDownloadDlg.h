// MultiDownloadDlg.h : header file
//
#include <afxole.h>

#include "..\..\..\code\Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"
#include "..\..\..\code\Components\UIFramework\LayoutManager\SplitLayout.h"
#include "..\..\..\code\Components\UIFramework\LayoutManager\BoxLayout.h"
#include "../../../code\Components/UIFramework/LayoutManager/WIN32IconButtonWidgeVer2.h"
#include "../../../code\Components/UIFramework/interface/QCommonControlBase.h"
#include "..\..\..\code\Components\ConfigureTool\IConfigure.h"
#include "../../../code\Components\UIFramework\Porting\QUIDropTraget.h"
#include "../../../code\Components\UIFramework\Porting\UIOleDataSource.h"
#include "..\..\..\code\Components\UIFramework\LayoutManager\LayoutManager.h"
#include "DownloadFunc.h"
#include <map>
#include <vector>


#define DOWNLOAD_BUTTON_1 0x1
#define DOWNLOAD_BUTTON_2 0x2
#define DOWNLOAD_BUTTON_3 0x4
#define DOWNLOAD_BUTTON_4 0x8
#define DOWNLOAD_BUTTON_5 0x10
#define DOWNLOAD_BUTTON_6 0x20
#define DOWNLOAD_BUTTON_7 0x40
#define DOWNLOAD_BUTTON_8 0x80
#define DOWNLOAD_BUTTON_ALL 0x1|0x2|0x4|0x8|0x10|0x20|0x40|0x80
#pragma once

namespace QSYNC_LIB
{
    using namespace std;

    // CMultiDownloadDlg dialog
    class CMultiDownloadForAllDlg : public Win32DoubleBufferQDialogWidge, public INotify
    {
        // Construction
    public:
        CMultiDownloadForAllDlg(CWnd* pParent = NULL);	// standard constructor
        ~CMultiDownloadForAllDlg() {
            if(m_pLayoutManager)
                delete m_pLayoutManager;
            for(int i = 0; i < 8; ++i) {
                if(m_DownloadFunc[i] == NULL) continue;
                delete m_DownloadFunc[i];
                m_DownloadFunc[i] = NULL;
            }
        }

        // Dialog Data
        enum { IDD = IDD_MULTIDOWNLOAD_DIALOG };

    protected:
        virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


        // Implementation
    protected:
        HICON m_hIcon;

        // Generated message map functions
        virtual BOOL OnInitDialog();
        afx_msg void OnPaint();
        afx_msg HCURSOR OnQueryDragIcon();
        DECLARE_MESSAGE_MAP()
        afx_msg void OnSelchangecom9();
        afx_msg void OnDestroy();
        LRESULT OnDLNotify(WPARAM wParam, LPARAM lParam);
    public:
        virtual void Event(const TSTRING& strEvent,long nParam);
        UINT		m_uiDownloadActive;

    private:
        SplitLayout<HDC>* m_pMainLayout;
        CLayoutManager* m_pLayoutManager;

        WIN32IconButton* m_pWinBrowse;
        WIN32IconButton* m_pWinStartAll;
        WIN32IconButton* m_pWinStart1;
        WIN32IconButton* m_pWinStart2;
        WIN32IconButton* m_pWinStart3;
        WIN32IconButton* m_pWinStart4;
        WIN32IconButton* m_pWinStart5;
        WIN32IconButton* m_pWinStart6;
        WIN32IconButton* m_pWinStart7;
        WIN32IconButton* m_pWinStart8;
        WIN32IconButton* m_pWinStarts[8];
        CallBackInfo m_pCallBackFunc[8];
        CDownloadFuncForAll	*m_DownloadFunc[8];
        int m_DownloadProgress[8];

        UINT m_unAvailCom;
        UINT m_unTotalCom;
        UINT m_unButtonEnNum;
        UINT m_uiDownloadMask;

        CStringArray m_szFileArray;
        CStringArray m_szComArray;
        CString m_strIni;
        CString m_strFolderPath;
        CString m_sz1FF;
    public:
        bool UpdateUIBehaviorByMessage(int nIndex,const char* szMsg, int nStep, int nProgress);
        static bool CallBack1(const char* szMsg, int nStep, int nProgress);
        static bool CallBack2(const char* szMsg, int nStep, int nProgress);
        static bool CallBack3(const char* szMsg, int nStep, int nProgress);
        static bool CallBack4(const char* szMsg, int nStep, int nProgress);
        static bool CallBack5(const char* szMsg, int nStep, int nProgress);
        static bool CallBack6(const char* szMsg, int nStep, int nProgress);
        static bool CallBack7(const char* szMsg, int nStep, int nProgress);
        static bool CallBack8(const char* szMsg, int nStep, int nProgress);
    private:
        void SetXMLUI(void);
        bool UpdateComportCombo(LPCTSTR lpszComport, bool bAdd = true);
        void Browse1FFFile(void);
        void DownloadFunc(unsigned int unDownloadButtonMask);
        bool EnableStartButton(int nStartButtonIndex,bool bEnable);
        void OnEventButtonClick(long nParam);
        bool AddComportToCombo(LPCTSTR lpszComport, UINT &unCount, int iCom);
        bool RemoveComportFromCombo(LPCTSTR lpszComport);
        void Set1FFFileToComboBox();
        void SetIniFilePath();
        void SetButtonEnabled();
        void UpdateProgressBarPercentage(int nIndex, int nProgress);
        void UpdateDownloadInformation(int nIndex, const char* szMsg, int nProgress);
        void UpdateButtonState(int nIndex, const char* szMsg);
        CString ConvertInformation(const char* szMsg);
        Platform GetPlatform(CString strFileName);
        CString OpenDialogToBrowse1FFFile();
        CString CheckLoad(CString szTemp);
        WIN32IconButton* GetStartButton(int i);
        Win32ProgressBarWidge* GetProgressBarWidge(int i);
        CComboBox* GetComboBox(int i);
        int GetSelectedComPort(int i);
        int GetComPortNum(int i);
        void PrepareToDownload(int i, UINT nComport);
        void CheckInDownloading(int i, bool& bContinueDownload);
        bool NewAddComportToCombo(LPCTSTR lpszComport, UINT &unCount, int iCom);
    };
}