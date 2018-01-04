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


//typedef bool (*pfn_Download)(long, bool(*CallBack)(const char*, int , int));
//typedef bool (*pfn_SetDLIniPath)(const TSTRING&);

#pragma once
namespace QSYNC_LIB
{

    class CDownloadUIDlgForMelos :  public Win32DoubleBufferQDialogWidge, public INotify, public INotify_ANSI
    {
        // Construction
    public:
        CDownloadUIDlgForMelos(CWnd* pParent = NULL);	// standard constructor
        virtual ~CDownloadUIDlgForMelos();
        enum { IDD = IDD_DOWNLOADUI_DIALOG };
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
        // Implementation
    protected:
        HICON m_hIcon;
        DECLARE_MESSAGE_MAP()
        // Generated message map functions
        virtual BOOL OnInitDialog();
        afx_msg void OnPaint();
        afx_msg HCURSOR OnQueryDragIcon();
        afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
        afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
        afx_msg void OnBackGround();
        afx_msg void OnImage();
        afx_msg void OnOption();
        afx_msg void OnAboutBox();
        //afx_msg void OnTimer(UINT nIDEvent);
    public:
        virtual void Event(const TSTRING& strEvent,long nParam);
        virtual void Event(const std::string& strEvent,long nParam);
        static bool CallBack(const char* szMsg, int nStep, int nProgress);
        bool UpdateUIProgressBar(const char* szMsg,int nStep, int nProgress);

    private:
        WIN32IconButton* m_pWinUpdateBtn;
        WIN32IconButton* m_pWinCloseBtn;
        WIN32IconButton* m_pWinLanguageBtn;
        WIN32PictureViewerWidge* m_pStep[8];
        WIN32PictureViewerWidge* m_pImage;
        WIN32LabelWidge* m_pWinStatusLabel;
        SplitLayout<HDC>* m_pMainLayout;
        CLayoutManager* m_pLayoutManager;
        ILocalization* m_pILocalization;
        IConfigure* m_pIConfigure;
        QCommonControlBase m_QCommonControlBase_Language;
        LanguageSubmenu m_LanguageSubmenu;

        bool m_bTimerEnable;
        int m_nPreStep;
        int m_nPreProgress;
        CString m_strPreMessage;
        bool m_bDownloadStart;
        int m_nComNum;
        int m_nSelectCom;
        int m_nStep;
        CSerCOMLibrary m_cSerComlibrary;
        CString m_strIni;
        CString m_strBackupNVPartition;
        CString m_strRestoreNVPartition;
        CString m_strBackupBRT;
        CString m_strRestoreBRT;
        CString m_strDownloadDU;

        IMAGE m_imgHEX;
        IMAGE m_imgPBL;
        IMAGE m_imgPartition;

        IMAGE m_imgDBL;
        IMAGE m_imgFSBL;
        IMAGE m_imgOSBL;
        IMAGE m_imgAMSS;
        IMAGE m_imgCEFS;
        //CFile m_logFile;
        bool m_bDifferentPartition;
        bool m_bAutoBackupRestoreBRT;
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
        bool SetAllImageParameters();
        int CheckBatteryStage();
        void DownloadStage();
        void FinalReboot();
        bool ExtractSpecifiedString(const TSTRING& strFileName, char* szCheckCode, int nCheckLength, int nExtractLength, int nIndexToGet, char* nOutput, int nOutputLength);
        void UpdateStepInformation(WIN32PictureViewerWidge** pStep, int nType);
        void SetDownloadSettingParameters();
        bool OpenAllImage();
        bool OpenImage(IMAGE & imgLoad, CString strLoadName);
        void GetStepString(CStringA& strMsg);
        void OnEventButtonClick(long nParam);
        void OnEventConnected(long nParam);
        void OnEventDisconnect(long nParam);
        void BackupBRTForMelos();
        void RestoreBRTForMelos();
        bool GetMyDocumentFolder(CStringA& strFolder);
        void ForceOpenAllImage();
        bool GetFileData(CString strLoadFileName, IMAGE & imgLoad);
    };
}
