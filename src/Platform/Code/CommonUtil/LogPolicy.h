// LogPolicy.h: interface for the LogPolicy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPOLICY_H__65C94F08_0D69_4AFB_BCA0_8A758B060DB3__INCLUDED_)
#define AFX_LOGPOLICY_H__65C94F08_0D69_4AFB_BCA0_8A758B060DB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Win32SHELLUtility.h"
#include "afxmt.h"
#include "cloud.h"

#ifdef _DEBUG
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
#else
#define TRACE_FILE_LINE_INFO() 
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
	static void FunctionLog(const TCHAR* szMessage, const CString& strFileName)
	{
		static CCriticalSection	m_Critsection;
		CSingleLock singleLock(&m_Critsection,TRUE);

		const TCHAR szCRLF[] = _T("\r\n");
		const char szaCRLF[] = "\r\n";

		bool bOpened = false;
		CFile logFile;
		CFileException ex;
		if (logFile.Open(GetFile(strFileName), CFile::modeWrite |CFile::shareExclusive | CFile::modeCreate | CFile::modeNoTruncate , &ex))
		{
			bOpened = true;
			//Write to File
			ULONGLONG dwLength = logFile.SeekToEnd();
			if(bOpened)
			{	
				CStringA strTime = UnicodetoASCII(GetCurrentTime()) + " : ";
				WriteMessage(logFile,(LPCSTR)strTime);
				WriteMessage(logFile,szMessage);
				WriteMessage(logFile,szaCRLF);
				WriteMessage(logFile,szaCRLF);	
			}
			logFile.Close();
		}		
		

	}
	static void Log(const char* szMessage,const CString& strFileName = _T(""))
	{
		implementLog(true,szMessage,_T(""),strFileName);
	}
	static void Log(const CString& strMessage,const CString& strFileName = _T(""), bool bShowTime = true)
	{
		implementLog(false,"",strMessage,strFileName, bShowTime);
	}	
	static std::wstring ansitowide(const std::string& strAnsi, UINT CodePage)
	{
		using namespace std;
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
	static const CString GetCurrentTime()
	{
		return CTime::GetCurrentTime().Format(_T("[%Y/%m/%d %H:%M:%S]"));
	}
	static const CString GetModuleName()
	{
		CString strModuleName;
		//::GetModuleFileName(AfxGetInstanceHandle(), strModuleName.GetBuffer(MAX_PATH), MAX_PATH);
		//strModuleName.ReleaseBuffer();
		return strModuleName;
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
	static void implementLog(bool bAnsi,const char* szaMessage,const CString& strMessage,const CString& strFileName, bool bShowTime = true)
	{
		bool bEnableLog = true;
		int nSize = 0;
		FILETIME time;
		if(CCloud::Get(_T("LogPolicy"),nSize,NULL,time) && nSize > 0)
		{
			char* szTemp = new char[nSize];
			CCloud::Get(_T("LogPolicy"),nSize,szTemp,time);
			bEnableLog = (strcmp(szTemp,"disableLOG") == 0) == false;
			delete [] szTemp;
		}

		if(bEnableLog)
		{
//#ifndef DISABLE_LOG
			static CCriticalSection	m_Critsection;
			CSingleLock singleLock(&m_Critsection,TRUE);

			//Write data to file
			const TCHAR szCRLF[] = _T("\r\n");
			const char szaCRLF[] = "\r\n";

			bool bOpened = false;;
			CFile logFile;
			CFileException ex;
			if (!logFile.Open(GetFile(strFileName), CFile::modeWrite |CFile::shareExclusive | CFile::modeCreate | CFile::modeNoTruncate , &ex))
			{
#ifdef _DEBUG
				//TCHAR szError[1024] = {0};
				//ex.GetErrorMessage(szError, 1024);
				//AfxMessageBox(szError);
				//ASSERT(0);
#endif
			}
			else
			{
				bOpened = true;
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

				if(bOpened)
				{
					if(bAnsi == false)
					{
						if (bShowTime)
						{
							WriteMessage(logFile,GetCurrentTime() + szCRLF);
							WriteMessage(logFile,GetModuleName() + szCRLF);
						}
						WriteMessage(logFile,strMessage);
						WriteMessage(logFile,szCRLF);
						if (bShowTime)
							WriteMessage(logFile,szCRLF);
					}
					else
					{
						if (bShowTime)
						{
							CStringA strTime = UnicodetoASCII(GetCurrentTime()) + szaCRLF;
							WriteMessage(logFile,(LPCSTR)strTime);
							CStringA strModule = UnicodetoASCII(GetModuleName()) + szaCRLF;
							WriteMessage(logFile,(LPCSTR)strModule);
						}
						WriteMessage(logFile,szaMessage);
						WriteMessage(logFile,szaCRLF);
						if (bShowTime)
							WriteMessage(logFile,szaCRLF);
					}
				}

				logFile.Close();
			}		
//#endif //#ifndef DISABLE_LOG
		}
	}

	static HRESULT __fastcall UnicodeToAnsi_internal(LPCOLESTR pszW, LPSTR* ppszA) 
	{ 
		HRESULT hr = NOERROR;    

		// If input is null then just return the same.
		if (pszW == NULL)
		{
			*ppszA = NULL;
		}
		else
		{
			ULONG cbAnsi, cCharacters;
			DWORD dwError;

			cCharacters = static_cast<ULONG>(wcslen(pszW))+1;
			// Determine number of bytes to be allocated for ANSI string. An
			// ANSI string can have at most 2 bytes per character (for Double
			// Byte Character Strings.)
			cbAnsi = cCharacters*2;

			// Use of the OLE allocator is not required because the resultant
			// ANSI  string will never be passed to another COM component. You
			// can use your own allocator.
			//*ppszA = (LPSTR) CoTaskMemAlloc(cbAnsi);
			*ppszA = new char[cbAnsi];
			if (NULL == *ppszA)
			{
				delete *ppszA;
				hr = E_OUTOFMEMORY;
			}
			else if (0 == WideCharToMultiByte(CP_ACP, 0, pszW, cCharacters, *ppszA,cbAnsi, NULL, NULL))
			{
				dwError = GetLastError();
				//CoTaskMemFree(*ppszA);
				delete *ppszA;
				*ppszA = NULL;
				hr = HRESULT_FROM_WIN32(dwError);
			}
		}

		return hr;

	} 

	static void UnicodeToAnsiBuffer(const wchar_t *pStr,char *pDestBuffer,int bufferSize)
	{
		LPCOLESTR pszW=pStr;
		LPSTR ppszA;

		UnicodeToAnsi_internal(pszW,&ppszA);

		if( bufferSize > 0 ) 
		{
			bufferSize--;
			::strncpy_s(pDestBuffer,bufferSize,ppszA,strlen(ppszA));
			pDestBuffer[bufferSize]=0;
		}

		delete [] ppszA;
	}

	static CStringA UnicodetoASCII(const CString& strTemp)
	{
		CStringA strAnsi;
#ifdef _UNICODE
		int nBufferSize = (strTemp.GetLength()+1)*sizeof(TCHAR);
		UnicodeToAnsiBuffer(strTemp,strAnsi.GetBuffer(nBufferSize),nBufferSize);
		strAnsi.ReleaseBuffer(nBufferSize);
#else
		strAnsi = strTemp;
#endif
		return strAnsi;
	}
};

#endif // !defined(AFX_LOGPOLICY_H__65C94F08_0D69_4AFB_BCA0_8A758B060DB3__INCLUDED_)
