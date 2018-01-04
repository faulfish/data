#include "../../../code\Components/UIFramework/LayoutManager/WIN32IconButtonWidgeVer2.h"
#include "../../../code\Components/UIFramework/LayoutManager/WIN32ProgressBarWidge.h"
#include "..\..\..\code\odmtma_api\SerCOMLib\SerCOMLibrary.h"
#include "..\..\lib\requestutility\interfacebase.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"


#pragma once

#define WM_DL_NOTIFY WM_USER + 05

#define DOWNLOAD_INFO_SIZE 10


typedef struct DOWN_LOAD_INFO_TAG
{
	ULONG	ulPercentage;		// 0 ~100
	ULONG	uStep;
	WCHAR	cMessage[400];

}DownloadInfo, *pDownloadInfo;

typedef bool (*CallBackInfo)(const char* szMsg, int nStep, int nProgress);

namespace QSYNC_LIB
{

	class CDownloadFunc
	{
	public:
		CDownloadFunc(UINT unComport, UINT unButtonIndex, CString iniPath,HWND hUIWnd, WIN32IconButton *pButton, Win32ProgressBarWidge* pProgress);
	public:
		~CDownloadFunc(void);

	private:
		UINT	m_unComport;
		UINT	m_unButtonIndex;
		CString m_strIni;
		CString m_str1ff;

		UINT			m_DownloadInfoIndex;
		DownloadInfo	m_DownloadInfo[DOWNLOAD_INFO_SIZE];

		WIN32IconButton*	m_pButtonWnd;
		Win32ProgressBarWidge* m_pProgressWnd;

		HWND	m_hUIWnd;
		CallBackInfo m_pCallBackFunc;


	public:
		void	DownloadProcess();
		bool	GetDLTaskInfo(UINT nIndex, TCHAR* szMsg, UINT& nStep, UINT& nProgress);

		static bool CallBackFunc(const char* szMsg, int nStep, int nProgress);

		//void SetButtonWnd(WIN32IconButton* pButton);
		//void SetPorgressWnd(Win32ProgressBarWidge* pProgress);
		//void SetUIWnd(HWND hWnd);
		//void SetComport(UINT unComport);
		//void SetButtonIndex(UINT unButtonIndex);
		//void SetIniFile(CString IniPath);
		WIN32IconButton* ReturnButtonWnd();
		Win32ProgressBarWidge* ReturnProgressWnd();
		UINT	ReturnComPort();
		void	Set1ffFile(const TCHAR* str1ff);
		void	SetCallBackFunc(CallBackInfo pCallBac);
//		void	SetLoadInfo(const bool bSigned, const int nAmssVersion, const int nOemsblVersion);
//		void	SetLoadCheckFlag(bool bCheck)
		CString		CheckLoad();
		bool		DeleteUnzippedFolder(const TCHAR* strFolderPath);
	private:
		void	SetDLTaskInfo(const TCHAR* szMsg, int nStep = -1, int nProgress = 0);
		bool	GetInfoFromME(CStringA& strSW, CStringA& strHW);
		bool	CheckQFuseState1();
		bool	CheckQFuseState2();
		bool	CheckQFuseState3();
//		bool	UnZip1fftoIni();
		bool	CheckSignedCode(const TSTRING& strAmss, const TSTRING& strOemsbl) ;
		bool	GetCodeVersion(const TSTRING& strFileName, char* szCheckCode, int nCheckLength, CString& strVer);
		bool	GetVariantString(CString& strVariantString);
		int		GetCodeSecurityVersion(const TSTRING& strFileName, char* szCheckCode, int nCheckLength);
		TCHAR*	GetDLPathFromIni(const TCHAR* strIniPath, const TCHAR* strName);
		bool	SetDLPathToIni(const TCHAR* strIniPath, const TCHAR* strName, TCHAR* strPath);
        void    WriteLog(CFile& file, CStringA& info, int nLoop);
        int CheckBatteryStage();
        int ReadBatteryVoltage();

		static bool		m_bSigned;
		static int		m_nAmssVersion;
		static int		m_nOemsblVersion;
//		static bool		m_bCheckLoad;
		static CStringArray	m_strCheckLoadInfoArray;
		CString	m_strFolderPath;


	};
}