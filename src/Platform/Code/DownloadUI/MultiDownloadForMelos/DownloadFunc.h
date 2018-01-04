#include "../../../code\Components/UIFramework/LayoutManager/WIN32IconButtonWidgeVer2.h"
#include "../../../code\Components/UIFramework/LayoutManager/WIN32ProgressBarWidge.h"
#include "..\..\..\code\odmtma_api\SerCOMLib\SerCOMLibrary.h"
#include "..\..\lib\requestutility\interfacebase.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "../../Modules/QisdaQualcommDownloadModule/Download.h"
#include "../../CommonUtil/NotifyBase.h"

#pragma once
#define WM_DL_NOTIFY WM_USER + 05
#define DOWNLOAD_INFO_SIZE 10

typedef struct DOWN_LOAD_INFO_TAG
{
	ULONG ulPercentage;		// 0 ~100
	ULONG uStep;
	WCHAR cMessage[400];

}DownloadInfo, *pDownloadInfo;

typedef bool (*CallBackInfo)(const char* szMsg, int nStep, int nProgress);

namespace QSYNC_LIB
{
    typedef struct _DownloadFuncParameter {
        IMAGE* pImgHEX;
        IMAGE* pImgPBL;
        IMAGE* pImgPartition;
        IMAGE* pImgDBL;
        IMAGE* pImgFSBL;
        IMAGE* pImgOSBL;
        IMAGE* pImgAMSS;
        IMAGE* pImgCEFS;
    } DownloadFuncParameter;

    class CDownloadFuncForMelos : public INotify_ANSI
	{
	public:
		CDownloadFuncForMelos(UINT unComport, UINT unButtonIndex, CString iniPath,HWND hUIWnd, WIN32IconButton *pButton, Win32ProgressBarWidge* pProgress);
        CDownloadFuncForMelos(UINT unComport, UINT unButtonIndex, CString iniPath,HWND hUIWnd, WIN32IconButton *pButton, Win32ProgressBarWidge* pProgress, DownloadFuncParameter* pImageParameter);
		virtual ~CDownloadFuncForMelos(void);

	private:
		UINT m_unComport;       ///< comport number
		UINT m_unButtonIndex;   ///< which button is pressed
		CString m_strIni;       ///< ini file location / path
		CString m_str1ff;       ///< 1ff file location / path
		UINT m_DownloadInfoIndex;   ///< 
		DownloadInfo m_DownloadInfo[DOWNLOAD_INFO_SIZE];
		WIN32IconButton* m_pButtonWnd;  ///< button HWND
		Win32ProgressBarWidge* m_pProgressWnd;  ///< progress HWND
		HWND m_hUIWnd;
		CallBackInfo m_pCallBackFunc;

        int m_nPercentage;
        DownloadFuncParameter* m_pImageParameter;
        static bool m_bSigned;
        static int m_nAmssVersion;
        static int m_nOemsblVersion;
        static CStringArray	m_strCheckLoadInfoArray;
        CString	m_strFolderPath;

	public:
		void DownloadProcess();
		bool GetDLTaskInfo(UINT nIndex, TCHAR* szMsg, UINT& nStep, UINT& nProgress);
		static bool CallBackFunc(const char* szMsg, int nStep, int nProgress);
		WIN32IconButton* ReturnButtonWnd();
		Win32ProgressBarWidge* ReturnProgressWnd();
		UINT ReturnComPort();
		void Set1ffFile(const TCHAR* str1ff);
		void SetCallBackFunc(CallBackInfo pCallBac);
		CString CheckLoad();
		bool DeleteUnzippedFolder(const TCHAR* strFolderPath);

	private:
		void SetDLTaskInfo(const TCHAR* szMsg, int nStep = -1, int nProgress = 0);
		bool GetInfoFromME();
		bool CheckQFuseState1();
		bool CheckQFuseState2();
		bool CheckQFuseState3();
		bool CheckSignedCode(const TSTRING& strAmss, const TSTRING& strOemsbl) ;
		bool GetCodeVersion(const TSTRING& strFileName, char* szCheckCode, int nCheckLength, CString& strVer);
		bool GetVariantString(CString& strVariantString);
		int GetCodeSecurityVersion(const TSTRING& strFileName, char* szCheckCode, int nCheckLength);
		TCHAR* GetDLPathFromIni(const TCHAR* strIniPath, const TCHAR* strName);
		bool SetDLPathToIni(const TCHAR* strIniPath, const TCHAR* strName, TCHAR* strPath);
        BOOL OpenLogFile(CFile& file);
        void WriteLog(CFile& file, CStringA& info, int nLoop);
        int CheckBatteryStage();
        int ReadBatteryVoltage();
        bool CheckIsInDownload(bool& bDownloadOK);
        void ResetHandsetAfterDownload(bool bDownloadOK, CStringA &strWrite, CFile& f, int i);
        void BackupBRTAfterDownload();
        void Event(const string& strEvent,long nParam);

	};
}