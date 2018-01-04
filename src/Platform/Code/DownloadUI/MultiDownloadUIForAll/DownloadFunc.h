#include "../../../code\Components/UIFramework/LayoutManager/WIN32IconButtonWidgeVer2.h"
#include "../../../code\Components/UIFramework/LayoutManager/WIN32ProgressBarWidge.h"
#include "..\..\..\code\odmtma_api\SerCOMLib\SerCOMLibrary.h"
#include "..\..\lib\requestutility\interfacebase.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "../../Modules/QisdaQualcommDownloadModule/Download.h"
#include "SoftwareLoad.h"

#pragma once

#define WM_DL_NOTIFY WM_USER + 05
#define DOWNLOAD_INFO_SIZE 10

typedef struct DOWN_LOAD_INFO_TAG {
	ULONG	ulPercentage;		// 0 ~100
	ULONG	uStep;
	WCHAR	cMessage[400];

} DownloadInfo, *pDownloadInfo;

typedef bool (*CallBackInfo)(const char* szMsg, int nStep, int nProgress);

namespace QSYNC_LIB {
    class CDownloadFuncForAll : public INotify_ANSI {
	public:
		CDownloadFuncForAll(UINT unComport, UINT unButtonIndex, CString iniPath,HWND hUIWnd, WIN32IconButton *pButton, Win32ProgressBarWidge* pProgress);
		~CDownloadFuncForAll();

	private:
		UINT m_unComport;
		UINT m_unButtonIndex;
		CString m_strIni;
		CString m_str1ff;
		UINT m_DownloadInfoIndex;
		DownloadInfo m_DownloadInfo[DOWNLOAD_INFO_SIZE];
		WIN32IconButton* m_pButtonWnd;
		Win32ProgressBarWidge* m_pProgressWnd;
		HWND m_hUIWnd;
		CallBackInfo m_pCallBackFunc;
        CString	m_strFolderPath;
        static bool m_bSigned;
        static int m_nAmssVersion;
        static int m_nOemsblVersion;
        static CStringArray	m_strCheckLoadInfoArray;
        Platform m_nPlatform;
        bool m_bPartitionDifferent;

	public:
		void DownloadProcess();
        bool CheckHandsetAndSoftwareVersion();
        bool CheckQFuseStage();
		bool GetDLTaskInfo(UINT nIndex, CString* szMsg, UINT& nStep, UINT& nProgress);
		static bool CallBackFunc(const char* szMsg, int nStep, int nProgress);
        void Event(const std::string& strEvent,long nParam);
		WIN32IconButton* GetButtonWnd();
		Win32ProgressBarWidge* GetProgressWnd();
		UINT GetComPort();
		void Set1ffFile(const TCHAR* str1ff);
		void SetCallBackFunc(CallBackInfo pCallBac);
		CString CheckLoad(Platform nPlatform);
        bool DeleteUnzippedFolder(const TCHAR* strFolderPath);
        Platform GetPlatform(CString strFileName);

	private:
		void SetDLTaskInfo(const TCHAR* szMsg, int nStep = -1, int nProgress = 0);
		bool GetInfoFromME(CStringA& strSW, CStringA& strHW);
        void GetHWSWVersionFromMobile(CStringA &strSW, CStringA &strHW);
        void GetVariantVersion();
		bool CheckQFuseState1();
		bool CheckQFuseState2();
		bool CheckQFuseState3();
		bool GetVariantString(CString& strVariantString);
		TCHAR* GetDLPathFromIni(const TCHAR* strIniPath, const TCHAR* strName);
		bool SetDLPathToIni(const TCHAR* strIniPath, const TCHAR* strName, TCHAR* strPath);
        int CheckBatteryStage();
        int ReadBatteryVoltage();
        bool NewDownloadStage();
        bool WarnInDownloadMode(bool bInDownloadMode);
        bool OpenImageFile(SoftwareLoad* load);
        void CreateTempFolder();
        void SetDownload6240Information(CShareDownload6240* download);
        void SetDownload6245Information(CShareDownload6245* download);
        bool Download(CDLFormatCommand* download);
	};
}