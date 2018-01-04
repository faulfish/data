// LogPolicy.h: interface for the LogPolicy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPOLICY_H__65C94F08_0D69_4AFB_BCA0_8A758B060DB3__INCLUDED_)
#define AFX_LOGPOLICY_H__65C94F08_0D69_4AFB_BCA0_8A758B060DB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Win32SHELLUtility.h"

#ifdef _UNICODE
#define TRACE_FILE_LINE_INFO() do{\
	/*CString str;*/\
/*str.Format(_T("file=%s,line=%u\r\n"),LogPolicy::ansitowide(__FILE__,CP_ACP).c_str(),__LINE__);*/\
/*LogPolicy::Log(str);*/\
	   }while(0)
#else
#define TRACE_FILE_LINE_INFO() do{\
	CString str;\
str.Format(_T("file=%s,line=%u\r\n"),__FILE__,__LINE__);\
LogPolicy::Log(str);\
	   }while(0)
#endif

class LogPolicy  
{
public:	
	static void DebugLog(const char* szMessage,const CString& strFileName = _T(""))
	{
#ifdef _DEBUG
#ifdef _UNICODE
		Log(szMessage,strFileName);
#else
		Log(szMessage,strFileName);
#endif
#endif
	}
	static void DebugLog(const CString& strMessage,const CString& strFileName = _T(""))
	{
#ifdef _DEBUG
		Log(strMessage,strFileName);
#endif
	}
	static void Log(const char* szMessage,const CString& strFileName = _T(""))
	{
		implementLog(true,szMessage,_T(""),strFileName);
	}
	static void Log(const CString& strMessage,const CString& strFileName = _T(""))
	{
		implementLog(false,"",strMessage,strFileName);
	}	
	static wstring ansitowide(const string& strAnsi, UINT CodePage)
	{
		wstring strDes;

		int nBufferSize = (static_cast<int>(strAnsi.size())+1)*3;
		LPWSTR wcpDes = new wchar_t[nBufferSize];
		if(wcpDes)
		{
			::ZeroMemory(wcpDes, nBufferSize);
			::MultiByteToWideChar(CodePage, 0, strAnsi.c_str(), static_cast<int>(strAnsi.size()), wcpDes, nBufferSize);
			strDes = wcpDes;
			delete [] wcpDes;
		}

		return strDes;
	}
protected:
	static CString GetFile(const CString& strFileName)
	{		
		//1. Get folder path
		TCHAR szFolder[MAX_PATH] = {0};
		bool bRes = Win32Utility::GetShellFolder(szFolder,CSIDL_PERSONAL,_T("Log"));

		//2. Get file name
		CString strLogFileName = strFileName.IsEmpty() ? _T("eventlog.txt") : strFileName;
		strLogFileName = szFolder + strLogFileName;

		return strLogFileName;
	}
	static void WriteMessage(CFile& logFile,const CString& strMessage)
	{
		logFile.Write(strMessage,strMessage.GetLength()*sizeof(TCHAR));
	}
	static void WriteMessage(CFile& logFile,const char* szMessage)
	{
		logFile.Write(szMessage,static_cast<int>(strlen(szMessage)));
	}
	static void implementLog(bool bAnsi,const char* szaMessage,const CString& strMessage,const CString& strFileName)
	{
		static CCriticalSection	m_Critsection;
		CSingleLock singleLock(&m_Critsection,TRUE);

		//Write data to file
		const TCHAR szCRLF[] = _T("\r\n");
		const char szaCRLF[] = "\r\n";

		CFile logFile;
		CFileException ex;
		if (!logFile.Open(GetFile(strFileName), CFile::modeWrite |CFile::shareExclusive | CFile::modeCreate | CFile::modeNoTruncate , &ex))
		{
#ifdef _DEBUG
			TCHAR szError[1024] = {0};
			ex.GetErrorMessage(szError, 1024);
			//AfxMessageBox(szError);
			//ASSERT(0);
#endif
		}
		else
		{
			//Write to File
			ULONGLONG dwLength = logFile.SeekToEnd();
			if(dwLength == 0)
			{
#ifdef _UNICODE
				if(bAnsi == false)
				{
					BYTE byteUnicode[] = {0xFF,0xFE};
					logFile.Write(byteUnicode,2);
				}
#endif
			}
			
			if(bAnsi == false)
			{
				WriteMessage(logFile,GetCurrentTime() + szCRLF);
				WriteMessage(logFile,GetModuleName() + szCRLF);
				WriteMessage(logFile,strMessage);
				WriteMessage(logFile,szCRLF);
				WriteMessage(logFile,szCRLF);
			}
			else
			{
				WriteMessage(logFile,szaMessage);
				WriteMessage(logFile,szaCRLF);
				WriteMessage(logFile,szaCRLF);
			}

			logFile.Close();
		}
	}
	static const CString GetCurrentTime()
	{
		return CTime::GetCurrentTime().Format(_T("%A, %B %d, %Y %H:%M:%S"));
	}
	static const CString GetModuleName()
	{
		CString strModuleName;
		::GetModuleFileName(AfxGetInstanceHandle(), strModuleName.GetBuffer(MAX_PATH), MAX_PATH);
		strModuleName.ReleaseBuffer();
		return strModuleName;
	}
};

#endif // !defined(AFX_LOGPOLICY_H__65C94F08_0D69_4AFB_BCA0_8A758B060DB3__INCLUDED_)
