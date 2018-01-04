// Q_DOWNLOAD.h : main header file for the Q_DOWNLOAD application
//

#if !defined(AFX_Q_DOWNLOAD_H__A48BD2FB_2C7C_4230_BB92_5C79FB417DCD__INCLUDED_)
#define AFX_Q_DOWNLOAD_H__A48BD2FB_2C7C_4230_BB92_5C79FB417DCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <afxMT.h>
#include <atlbase.h>
#include "..\..\..\Modules\QisdaQualcommDownloadModule\Download.h"
//#include "..\..\..\Modules\MotoDownloadModule\MotoDownloadModule.h"
#include "..\..\..\Modules\QisdaNVModule\NVItem.h"
#include "..\..\..\CommonUtil\PIMStringUtility.h"
#include "..\..\..\CommonUtil\NotifyBase.h"
#include "Download_Export.h"
#include <string>
#pragma warning(disable : 4996)

/////////////////////////////////////////////////////////////////////////////
// CQ_DOWNLOADApp:
// See Q_DOWNLOAD.cpp for the implementation of this class
//

class CQ_DOWNLOADApp : public CWinApp
{
public:
	CQ_DOWNLOADApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQ_DOWNLOADApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CQ_DOWNLOADApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//////////////////////////////////////////////////////////////////////////
const int		g_nDefaultTimeout =   10000;
#define			Dummy_IMEI              "000000011234560"
const int		g_nBackupTime = 120000;
const int		g_nRestoreTime =120000;
const int		g_nSW_Version_Size = 4;
const int		g_nHW_Version_Size = 5;
const int		g_nMobile_ID_Size  = 11;
//////////////////////////////////////////////////////////////////////////
const LPCSTR EVENT_UI_MESSAGE		= "TNotify_UI_Message";
const LPCSTR EVENT_UI_PROGRESS		= "TNotify_UI_Progress";
const LPCSTR EVENT_DL_RESULT		= "TNotify_DL_Result";
const LPCSTR EVENT_RESET_UI			= "TNotify_Reset_UI";
//////////////////////////////////////////////////////////////////////////
struct UIProgress
{
	int nSlot;
	int nMAINProgress;
	int nSUBProgress;
};
struct UIResult
{
	int nSlot;
	char szResult[512];
};
//////////////////////////////////////////////////////////////////////////
class CPICSFile : public CStdioFile
{
public:
	CPICSFile(){}
	CPICSFile(FILE* pOpenStream):CStdioFile(pOpenStream){}
	CPICSFile(LPCTSTR lpszFileName, UINT nOpenFlags):CStdioFile(lpszFileName, nOpenFlags){}
	virtual ~CPICSFile(){}
public:
	virtual void WriteALine(LPCTSTR lpsz)
	{
		WriteString(lpsz);
		WriteString("\n");
	}
};
//////////////////////////////////////////////////////////////////////////
class CDLProcess: public QSYNC_LIB::CNotifyBase_ANSI, public QSYNC_LIB::INotify_ANSI
{
	class CSpendTime
	{
	public:
		CSpendTime(const std::string& strTitle, CDLProcess* pMainClass) 
			: m_strTitle(strTitle), m_pMainClass(pMainClass)
		{
			m_dwStartTime = GetTickCount();
		}
		~CSpendTime()
		{
			char cpTemp[100] = {0};
			sprintf(cpTemp," spend: (%.3f)",(GetTickCount()-m_dwStartTime)/1000.0);
			m_strTitle += cpTemp;
			m_pMainClass->AddMessage(m_strTitle.c_str());
		}
		DWORD GetStartTime() {return m_dwStartTime;}
	private:
		std::string m_strTitle;
		DWORD m_dwStartTime;
		CDLProcess* m_pMainClass;
	};
public:
	CDLProcess(long nPort);
	virtual ~CDLProcess();
private:
	long m_nPort;
	CCFCDiagCommandPtr m_pDiag; 

	std::string m_strErrorCode;
	CStringA m_strLog;
	CCriticalSection m_WriteLogCritSection;
	CCriticalSection m_WriteResultLogCritSection;
	CCriticalSection m_WriteBrtCritSection;

	_clsO2Check* m_pITS_O2;
public:
	bool Run6245();
	bool Run6240();
	
	//bool RunMotoProcess();
	static bool TestConnection(int nComPort); /*false=connection closed*/
private:
	bool IsCommandAbleToSent();
	//0.
	//bool IsMotoDLMode(CMotoDownload* pMotoDL, bool& bDLMode);
	
	//1.
	bool ReadIMEI(char* szIMEI100);
	bool ReadBTAddress(char* szBTAddress100);
	bool ITS_QueryIMEI(char *szIMEI_BT, char *szStationtState, char *szErrMsg);
	bool CheckIMEIStationStatus();

	//2.
	bool GetMobileID(std::string& strMobileID, int nTimeout = g_nDefaultTimeout);
	bool BackupBrt(int nTimeout = g_nDefaultTimeout);
	bool RestoreNV(int nTimeout = g_nRestoreTime);
	bool IsHWVersionMatched();
	bool IsHWVersionMatchedInDLMode(CDLFormatCommand* pDl);
	bool PreProcessDownloadThreadFunction(bool bIsDLMode);
	bool ReadFactoryArea(DIAG_NVITEM_PACKET_F_type& resp);
	void ReadHWVersion();
	void RewriteRedownloadCount();

	//3.
	bool BackupNV(int nTimeout = g_nBackupTime);

	//4.
	void Download6245Setting(CDownload6245* pDownload);
	void Download6240Setting(CDownload6240* pDownload);
	bool DL6245(CDownload6245* pDownload);
	bool DL6240(CDownload6240* pDownload);
	bool Is6245RestoreSuccessful();
	bool Is6240RestoreSuccessful();

	//5.
	bool WriteFactoryArea();
	void PrepareFactoryInfoForWriting(DIAG_NVITEM_PACKET_F_type& resp, bool bInNormalMode);
	bool WritePPFFileNameTimeSize();
	bool Restart();
	bool SetOnlineWithoutResetting();
	//6.
	bool WritePICSFile();
	bool CloseConnection();
	void SetResult(bool bResult);
	void WriteLog();
	void WriteResultLog(CSpendTime* pSpendTime);

private:
	void AddMessage(const char* cpMsg);
	void AddErrorAndMessage(const char* cpErrorCode, const char* cpMsg);
	void SetProgress(int nMainProgress, int nSubProgress);
	

	/************************************************************************/
	/*                    Callback related function                         */
	/************************************************************************/
public:// CNotifyBase
	virtual bool Register(QSYNC_LIB::INotify_ANSI* pNotify, const std::string& strEvent);
	virtual bool Register(QSYNC_LIB::EventFunc_ANSI* pEventFunc, const std::string& strEvent);
	virtual bool Unregister(QSYNC_LIB::INotify_ANSI* pNotify, const std::string& strEvent);
	virtual bool Unregister(QSYNC_LIB::EventFunc_ANSI* pEventFunc, const std::string& strEvent);
	virtual void Fire(const std::string& strEvent,long nParam);
	virtual void DefineNotify(const std::string& strNewEvent);
	virtual void Event(const std::string& strEvent,long nParam);
	/************************************************************************/
	/*                Setting related function & Parameter                  */
	/************************************************************************/
public:
	void Set6245DLPath(const tagDLSetting6245& DLPath);
	void Set6240DLPath(const tagDLSetting6240& DLPath);
	bool SetSuperFilePath(const std::string& strFilePath);
	void SetCSV(const char* szCSVPath, const char* szCSVTime, const char* szCSVSize);
	void SetBRT_BACKUP_PATH(const char* szBrtFolder);
	void SetRework(bool bRework);
	void SetCheckFactoryArea(bool bCheck);
	void SetIMEICheck(int nIMEICheck);
	void SetRestartTimeout(int nTimeout);
	void SetIsLoopTest(bool bIsLoop);

	void SetSlotNumber(int nSlot);
	void SetSo(const char* szSO);
	void SetToolVersion(const char* szToolVersion);
	void SetConfigVersion(const char* szConfigVersion);
	void SetSWVersion(const char* szSWVersion);
	void SetHWVersion(const char* szHWVersion);
	void SetComputerName(const char* szComputerName);
	void SetModelName(const char* szModelName);
	void SetOnlyDLCEFS(bool bOnlyDLCEFS);
	void SetDLBootloader(bool bDLBootloader);
	void SetBackupBrt(bool bBackupBrt);
	void SetSaveLog(bool bSaveLog);

	void SetQVCEFS_File_Name(const char* szName);
	void SetQVCEFS_File_Time(const char* szTime);
	void SetQVCEFS_File_Size(const char* szSize);
private:
	int			m_nRewriteMobileIDFlag;
	std::string m_strMobileID;  
	std::string m_strHWVersion; //out setting in normal mode

	/* outside setting */
	// Function Flag
	bool    m_bSaveLog;
	bool	m_bRework;
	bool	m_bCheckFactoryArea;
	bool    m_bOnlyDLCEFS;
	bool	m_bDLBootloader;
	int		m_nIMEICheck;
	int		m_nRestartTimeout;
	bool	m_bBackupBrt;
	bool	m_bLoopTest;

	tagDLSetting6245 m_nDL6245Setting;
	tagDLSetting6240 m_nDL6240Setting;
	std::string		 m_strSuperFilePath;
	std::string		 m_strCSVPath;
	std::string		 m_strCSVTime;
	std::string		 m_strCSVSize;
	std::string		 m_strBRTFolder;

	// Factory Information
	int		m_nSlot;
	std::string m_strSO;
	std::string m_strTool_Version;
	std::string m_strConfig_Version;
	std::string m_strSWVersion;

	std::string m_strComputer_Name; /*example: T01D01*/
	std::string m_strMODEL_NAME;

	//QVCEFS
	std::string m_strQVCEFS_File_Name;
	std::string m_strQVCEFS_File_Time; /*example: 20071023060527*/
	std::string m_strQVCEFS_File_Size; /*example: 29524*/
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Q_DOWNLOAD_H__A48BD2FB_2C7C_4230_BB92_5C79FB417DCD__INCLUDED_)
