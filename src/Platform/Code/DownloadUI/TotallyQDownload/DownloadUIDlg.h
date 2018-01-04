// DownloadUIDlg.h : header file
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
#include "LanguageSubmenu.h"
#include "..\..\..\code\odmtma_api\SerCOMLib\SerCOMLibrary.h"
#include "..\..\lib\requestutility\interfacebase.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "../../Modules/QisdaQualcommDownloadModule/Download.h"
#include "DownloadInterface.h"
#include "Download6240Logic.h"
#include "Download6245Logic.h"

//typedef bool (*pfn_Download)(long, bool(*CallBack)(const char*, int , int));
//typedef bool (*pfn_SetDLIniPath)(const TSTRING&);

#pragma once

namespace QSYNC_LIB
{
    class CTotallyQDownload :  public Win32DoubleBufferQDialogWidge, public INotify//, public INotify_ANSI
    {
        // Construction
    public:
        CTotallyQDownload(CWnd* pParent = NULL);	// standard constructor
        virtual ~CTotallyQDownload();
        enum { IDD = IDD_DOWNLOADUI_DIALOG };
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

    public:
        virtual void Event(const TSTRING& strEvent,long nParam);
        //virtual void Event(const std::string& strEvent,long nParam);
        static bool CallBack(const char* szMsg, int nStep, int nProgress);
        bool UpdateUIProgressBar(const char* szMsg,int nStep, int nProgress);

    private:
        BOOL m_bLogOpened;
        CFile m_logFile;
        CLayoutManager* m_pLayoutManager;
        CSerCOMLibrary m_cSerComlibrary;
        CString m_strBackupBRT;
        CString m_strBackupNVPartition;
        CString m_strDownloadDU;
        CString m_strHWVersion;
        CString m_strIni;
        CString m_strPreMessage;
        CString m_strRestoreBRT;
        CString m_strRestoreNVPartition;
        CString m_strSWVersion;
        CString m_strVariantVersion;
        DownloadInterface* m_dif;
        IConfigure* m_pIConfigure;
        ILocalization* m_pILocalization;
        LanguageSubmenu m_LanguageSubmenu;
        QCommonControlBase m_QCommonControlBase_Language;
        SplitLayout<HDC>* m_pMainLayout;
        WIN32IconButton* m_pWinCloseBtn;
        WIN32IconButton* m_pWinLanguageBtn;
        WIN32IconButton* m_pWinUpdateBtn;
        WIN32LabelWidge* m_pWinStatusLabel;
        WIN32PictureViewerWidge* m_pImage;
        WIN32PictureViewerWidge* m_pStep[8];
        bool m_bDownloadStart;
        bool m_bEnableLog;
        bool m_bConnected;
        int m_nComNum;
        int m_nPreProgress;
        int m_nPreStep;
        int m_nSelectCom;
        int m_nStep;

    public:
        void DownloadFunc();
        bool SetIniPath();

    private:
        void SetXMLUI(void);
        void SetBackground(TSTRING strBkImage);
        void WinClose();
        void Initialize();
        bool GetInfoFromME();
        bool GetDLPathFromIni(const TSTRING& strIniPath, const TSTRING& strName, TSTRING& strPath);
        void InsertStringList(CString szMessage, CString szLabelInfo = _T(""));
        bool CheckSignedCode();
        bool CheckQFuseState1();
        bool CheckQFuseState2();
        bool CheckQFuseState3();
        int GetCodeSecurityVersion(const TSTRING& strFileName, char* szCheckCode, int nCheckLength);
        bool GetCodeVersion(const TSTRING& strFileName, char* szCheckCode, int nCheckLength, CString& strVer);
        bool GetCodeSigned(const TSTRING& strFileName, char* szCheckCode, int nCheckLength);
        void MB2WC(string mb, wstring wc);
        bool GetVariantString(CString& strVariantString);
        void ShowVariantVersion();
        void ShowSoftwareLoadVersion();
        void ShowSignedUnsignedCode();
        int ReadBatteryVoltage();
        int CheckBatteryStage();
        void DownloadStage();
        void FinalReboot();
        bool ExtractSpecifiedString(const TSTRING& strFileName, char* szCheckCode, int nCheckLength, int nExtractLength, int nIndexToGet, char* nOutput, int nOutputLength);
        void UpdateStepInformation(WIN32PictureViewerWidge** pStep, int nType);

        void OnEventButtonClick(long nParam);
        void OnEventConnected(long nParam);
        void OnEventDisconnect(long nParam);
        static void CALLBACK TimerProcFunc(HWND hwnd, UINT iMsg, UINT_PTR TimerID, DWORD Time);
    protected:
        HICON m_hIcon;
        DECLARE_MESSAGE_MAP()
        // Generated message map functions
        afx_msg HCURSOR OnQueryDragIcon();
        afx_msg void OnAboutBox();
        afx_msg void OnBackGround();
        afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
        afx_msg void OnImage();
        afx_msg void OnOption();
        afx_msg void OnPaint();
        afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
        virtual BOOL OnInitDialog();
        //afx_msg void OnTimer(UINT nIDEvent);
    };
};
