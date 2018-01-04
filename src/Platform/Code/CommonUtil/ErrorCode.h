#pragma once

#include "LogPolicy.h"
#include "Cloud.h"
#include <Tchar.h>

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif


static LPCTSTR g_szCFCErrorCodeTitle = _T("CFC_Error_COM");

class CErrorCode
{
public:
	CErrorCode(){}
	virtual ~CErrorCode(){}

	static const TSTRING& GetErrorString(const TSTRING& strModule, int nErrorCode)
	{
		static TSTRING strErrorMsg;
		TSTRING strIniPath = _T(".\\ErrorCode.ini");

		if(::_taccess_s(strIniPath.c_str(),0) == 0)
		{
			TCHAR cpErrorCode[10] = {0};
			_itot_s(nErrorCode, cpErrorCode, 10);

			TCHAR cpErrorMsg[200] ={0};
			DWORD lRetValue = ::GetPrivateProfileString(strModule.c_str(),cpErrorCode,NULL,cpErrorMsg, 200, strIniPath.c_str());
			strErrorMsg = cpErrorMsg;
		}
		return strErrorMsg;
	}
};

class CLastError
{
public:
	CLastError(int nComPort): m_nComPort(nComPort){}
	virtual ~CLastError(){}
public:
	void SetCFCLastError(int nLastError)
	{
		TSTRING strKey = g_szCFCErrorCodeTitle;
		TCHAR szComPort[10] = {0};
		_itot_s(m_nComPort, szComPort, 10);
		strKey += szComPort;

		CStringA cstrError;
		cstrError.Format("%d", nLastError);
		CCloud::Put(strKey, cstrError.GetLength(), cstrError.GetBuffer());
		cstrError.ReleaseBuffer();
	}

	int GetCFCLastError()
	{
		TSTRING strKey = g_szCFCErrorCodeTitle;
		TCHAR szComPort[10] = {0};
		_itot_s(m_nComPort, szComPort, 10);
		strKey += szComPort;

		TCHAR szError[10] = {0};
		int nSize = 0;
		FILETIME time;
		CCloud::Get(strKey, nSize, szError, time);
		return _ttoi(szError);
	}

private:
	int m_nComPort;
};



/************************************************************************
Class: CLog
Purpose: For everyone adding their Log easily
Instruction:
1. inherit public CLog
2. add your own Log function which should insert DISABLE LOG Flag
3. (If your don't want to log in COMxx.txt, please set your own FileName inside your function)
************************************************************************/
class CLog
{
public:
	CLog(int nCOMPort):m_nComPort(nCOMPort)
	{
		m_cstrFileName.Format(_T("COM%02d.txt"),nCOMPort);
	}
	virtual ~CLog(){}
public:
	void AddDLErrorLog(const TSTRING& strLog)
	{
#ifndef DISABLE_DL_LOG
		LogPolicy::FunctionLog(strLog.c_str(), _T("DL_")+m_cstrFileName);
#endif
	}
	void AddNVItemLog(const TSTRING& strLog)
	{
#ifndef DISABLE_NVITEM_LOG
		LogPolicy::FunctionLog(strLog.c_str(), _T("NVItem_")+m_cstrFileName);
#endif
	}

	void AddDLLog(bool bSuccess, const CString& strMessage,const CString& strFileName)
	{
#ifdef DownLoad_LOG
		CString strTmp = strMessage;
		strTmp += bSuccess ? _T(" Success") : _T(" Fail");

		CString strTitle;
		strTitle.Format(_T("_COM%d.txt"), m_nComPort);
		LogPolicy::Log(strTmp, strFileName + strTitle);
#endif
	}

	void AddDLLog(const CString& strMessage,const CString& strFileName)
	{
#ifdef DownLoad_LOG
		CString strTitle;
		strTitle.Format(_T("_COM%d.txt"), m_nComPort);
		LogPolicy::Log(strMessage, strFileName + strTitle);
#endif
	}
private:
	int m_nComPort;
	CString m_cstrFileName;
};