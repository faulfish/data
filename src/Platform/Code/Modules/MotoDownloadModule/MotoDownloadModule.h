// MotoDownloadModule.h : main header file for the MotoDownloadModule DLL
//
#ifndef __MOTO_DL_MODULE__H__
#define __MOTO_DL_MODULE__H__
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <io.h>
#include <windows.h>
#include <OLEAUTO.H>
#include <map>

#include "SetupAPI.h"
#pragma comment(lib, "SetupAPI.lib")

#include "RSDInterface.h"
#include "RSDUSBNotify.h"
#include ".\inc\pstcommon.h"

#include "..\..\CommonUtil\RefObject.h"
#include "..\..\CommonUtil\NotifyBase.h"
#include "..\..\Components\MobileFinder\MobileFinderInterface.h"
#include "../../CommonUtil/ErrorCode.h"

//////////////////////////////////////////////////////////////////////////

#ifdef MOTO_DOWNLOAD_MODULE_EXPORTS
#define MOTO_DOWNLOAD_API extern "C" __declspec(dllexport)
#define MOTO_DOWNLOAD_CLASS_API __declspec(dllexport)
#else
#define MOTO_DOWNLOAD_API extern "C" __declspec(dllimport)
#define MOTO_DOWNLOAD_CLASS_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////////
// CMotoDownloadModuleApp
// See MotoDownloadModule.cpp for the implementation of this class
//

class CMotoDownloadModuleApp : public CWinApp
{
public:
	CMotoDownloadModuleApp();

	//CRSDInterface* GetRSDInterface() const { return m_pRSDInterface; }
// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
protected:
	CRSDInterface* m_pRSDInterface;
};

//////////////////////////////////////////////////////////////////////////
const LPCSTR EVENT_MOTO_DL_MESSAGE		= "TNotify_MotoDL_Message";
const LPCSTR EVENT_MOTO_DL_PROGRESS		= "TNotify_MotoDL_Progress";
//////////////////////////////////////////////////////////////////////////
const LPCSTR g_strMoto_VID				= "22b8";
const LPCSTR g_strNormal_Melos_PID		= "2d50";
const LPCSTR g_strDL_Melos_PID			= "2d53";
//////////////////////////////////////////////////////////////////////////

class MOTO_DOWNLOAD_CLASS_API CMotoDownload:public QSYNC_LIB::CNotifyBase_ANSI, public CRefObject
{
public:
	CMotoDownload();
	virtual ~CMotoDownload();

	bool Initialize();
	void UnInitialize();
	bool IsInitiated();
public:
	bool RunDLThread(long nPort/*USB Prefix*/, char* szFlashFilePath);
		static UINT AfxRunDL(LPVOID pParam);
private:
	bool Download(long nPort/*USB Prefix*/, char* szFlashFilePath, CRSDUSBNotify* pRSDUSBNotify);

public:
	bool GetErrorInfo();
	void AddLog(int nPort, char* szMessage);
	
	static bool GetPrefix(long nPort,std::string& strPathPrefix);
	static bool MotoPortToCOMPort(long nPort/*USB Prefix*/, int& nComport, const std::string& strVID = g_strMoto_VID,const std::string& strPID = g_strNormal_Melos_PID);
	static bool TransferPrefixToComPort(const std::string& strPathPrefix,const std::string& strVID,const std::string& strPID, long& nComport);	
		static bool PathPrefixToParentId(const std::string& strPathPrefix,const std::string& strVID,const std::string& strPID, std::string& strParentIdPrefix);
		static bool GetComPortFromParentId(std::string strParentIdPrefix,const std::string& strVID,const std::string& strPID,std::string& strCOMPort); /*MI_01*/
	static void GetUSBInterfaceList(const GUID*  pDeviceClsName, CStringArray &interfaceList);
	//////////////////////////////////////////////////////////////////////////
	// Unfinished
	bool GetPrefixFromMap(long nPort,std::string& strPathPrefix);
	//////////////////////////////////////////////////////////////////////////
	bool IsConnected(long nPort/*USB Prefix*/, CStringA& cstrSymbolicLink, bool& bDLMode);
	bool IsDLMode(const std::string& strPathPrefix, CStringA& cstrSymbolicLink);
	bool IsNormalMode(const std::string& strPathPrefix, CStringA& cstrSymbolicLink);
	static bool FindSymLink(GUID guid, const std::string& strPathPrefix, CStringA& cstrSymbolicLink);

public:
	//BOOL CreateGhostWnd();
	//BOOL RegisterWindow();
	//static LRESULT CALLBACK stWinMsgHandler(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	//LRESULT CALLBACK WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//static CMotoDownload* GetObjectFromWindow(HWND hWnd);
public:
	void AddMsg(const char* szMsg);
	void SetProgress(int nProgress);
	void SetLastError(long nPort, int nError);

	/************************************************************************/
	/*                    Callback related function                         */
	/************************************************************************/
public:// CNotifyBase_ANSI
	virtual bool Register(QSYNC_LIB::INotify_ANSI* pNotify, const std::string& strEvent);
	virtual bool Register(QSYNC_LIB::EventFunc_ANSI* pEventFunc, const std::string& strEvent);
	virtual bool Unregister(QSYNC_LIB::INotify_ANSI* pNotify, const std::string& strEvent);
	virtual bool Unregister(QSYNC_LIB::EventFunc_ANSI* pEventFunc, const std::string& strEvent);
	virtual void Fire(const std::string& strEvent,long nParam);
	virtual void DefineNotify(const std::string& strNewEvent);
	
//////////////////////////////////////////////////////////////////////////
private:
	CCriticalSection m_LogCritSection;
	CCriticalSection m_PortMapCritSection;
	CCriticalSection m_LastErrorMapCritSection;
	CCriticalSection m_DLParamCritSection;
private:
	struct DLParams
	{
		long nPort;
		char* szFlashFilePath;
		HANDLE hExecuteEvent;
		bool bDLResult;
		CRSDUSBNotify* pRSDNotify;
	};

	CRSDInterface m_RSDInterface;
	CStringA m_strClassName;
	CStringA m_strTitleName;
	HWND m_hWnd;
	bool m_bInitiated;
	int m_nErrorInfo;
	std::map<long/*MotoPort*/, std::string/*Moto Prefix*/> m_ErrorMap;
	std::map<long/*MotoPort*/, std::string/*Moto Prefix*/> m_PortMap;
};

typedef CRefObjectPtr<CMotoDownload> CMotoDownloadPtr;


#endif //__MOTO_DL_MODULE__H__