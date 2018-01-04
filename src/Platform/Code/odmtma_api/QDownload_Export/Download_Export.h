// Download_Export.h : main header file for the Download_Export DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "../DownloadDLL/QDOperator.h"
#include "../../Diag_Export/QualcommDiag.h"
#include "../QFlash/QZip.h"
#include "../QFlash/QVersionParser.h"
#include "../1FFMaker/C1FFMaker.h"

#ifdef DL_EXPORT_EXPORTS
#define DL_API extern "C" __declspec(dllexport)
#define DL_CLASS_API __declspec(dllexport)
#else
#define DL_API extern "C" __declspec(dllimport)
#define DL_CLASS_API __declspec(dllimport)
#endif

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif

// CDownload_ExportApp
// See Download_Export.cpp for the implementation of this class
//

class CDownload_ExportApp : public CWinApp
{
public:
	CDownload_ExportApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

static TSTRING g_strIniPath = _T(".\\Download.ini");
CCriticalSection g_CritUnZip;

struct DLSetting
{
	char szHex[256];
	char szPartition[256];
	char szQcsbl[256];
	char szQcsblhd[256];
	char szOemsbl[256]; 
	char szOemsblhd[256];
	char szAmss[256]; 
	char szAmsshd[256]; 
	char szCefs[256];
};

DL_API bool Extract1FFToINI(const char* sz1FFPath, char* szIniPath, int nszIniPathBufferLength , const char* szExtractFolder = NULL);
DL_API bool Extract1FF(const char* sz1FFPath, char* szIniPath, int nszIniPathBufferLength , const char* szExtractFolder = NULL);
DL_API bool SetIniBy1ffFor6245(const string& strIni, const string& strFolderPath, const string& str1FF);
DL_API bool SetIniBy1ffFor6240(const string& strIni, const string& strFolderPath, const string& str1FF);
//DL_API bool SetIniBy1ffFor6270(const string& strIni, const string& strFolderPath, const string& str1FF);
typedef struct _DownloadParameter {
    bool bDownloadOemsbl;
    bool bBackupNV;
    bool bRestoreNV;
    bool bBackupBRT;
    bool bRestoreBRT;
    bool bDownloadDU;
    TSTRING strModelName;
} DownloadParameter;


//DL_API bool Download(long nComPort, bool(*CallBack)(const char*, int , int),const LPCSTR strIniPath, bool bDownloadOemsbl = true);
DL_API bool Download(long nComPort, bool(*CallBack)(const char*, int , int),const LPCSTR strIniPath, DownloadParameter *pdp);
DL_API bool DownloadByPath(long nComPort, bool(*CallBack)(const char*, int , int), const DLSetting& tagSetting);
DL_API bool BackupBrt(long nComPort, const LPCSTR szCsv, const LPCSTR szBrt);
DL_API bool RestoreBrt(long nComPort, const LPCSTR szBrt,bool bFilterRealonly);
DL_API bool BackupNV(long nComPort,  int nTimeOut/* =DEFAULT_CMD_TIME_OUT */);
DL_API bool RestoreNV(long nComPort, int nTimeOut/* =DEFAULT_CMD_TIME_OUT */);
DL_API bool DownloadDU(long nComPort, bool(*CallBack)(const char*, int , int), const LPCSTR szDuPath);
DL_API bool Restart(long nComPort, int nTimeOut/* =DEFAULT_CMD_TIME_OUT */);
DL_API bool IsQfused(long nComPort, bool& bQfused, int nTimeOut/* =DEFAULT_CMD_TIME_OUT */);
DL_API bool ReadQfuse(long nComPort, unsigned int& nReg1, unsigned int& nReg2, int nTimeOut/* =DEFAULT_CMD_TIME_OUT */); /*nReg1=6, nReg2 =0: Not Qfused*/
DL_API bool WriteQfused(long nComPort,WORD chain_select, int config_register[5],  int& nQfusedStatus/*0=success, 1=Read Operation failed, 2=Write Operation Failed, 3=Operation on selected chain not supported*/ , int nTimeOut);
DL_API bool Signed_CheckVersion(long nComPort, const LPCTSTR szAMSS, const LPCTSTR szOEMSBL, bool& bDownloadOEMSBL, int nTimeOut/* =DEFAULT_CMD_TIME_OUT */);
DL_API bool UnSigned_CheckVersion(const LPCTSTR szAMSS, const LPCTSTR szOEMSBL);


DL_API bool ReadFactoryArea(long nComport, WORD nNVItem, unsigned char* szFactoryArea128, int& nState, int nTimeOut/* =DEFAULT_CMD_TIME_OUT */);
DL_API bool WriteFactoryArea(long nComport, WORD nNVItem, const unsigned char* szFactoryArea128, int nTimeOut/* =DEFAULT_CMD_TIME_OUT */);
DL_API bool IsDLMode(long nComPort);
DL_API bool IsConnected(long nComPort);

//  [7/31/2008 Eddy.Guo]
DL_API bool CheckSWHWVersion(long nComPort, int nPhoneCodeVersion, int nBootloadVersion, int nTimeout);
DL_API void SetComport(long nComPort);

class CDL_Export
{
	
};