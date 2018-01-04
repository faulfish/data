#ifndef _ICOUND_H_
#define _ICOUND_H_
#pragma once

#include "Win32SHELLUtility.h"
#include <string>



class CCloudLock
{
	friend class CCloud;
protected:
	CCloudLock(HANDLE& hEvent):m_hEvent(hEvent)
	{
		//wait
		WaitForSingleObject(m_hEvent, INFINITE);
		::ResetEvent(m_hEvent);
	}
	~CCloudLock()
	{
		::SetEvent(m_hEvent);
	}
private:
	HANDLE& m_hEvent;
};

class CCloud
{
public:
	static bool Put(const TSTRING& strKey, int nSize, void* pData)
	{
		HANDLE hEvent = CreateEvent(NULL, FALSE, TRUE, _T("CCloudEvent"));
		CCloudLock lock(hEvent);

		bool bRes = false;
		
		HANDLE hFile = ::CreateFile(GetFile(strKey).c_str(),
                   FILE_ALL_ACCESS,
                   FILE_SHARE_READ,
                   NULL,
                   CREATE_ALWAYS,
                   FILE_FLAG_BACKUP_SEMANTICS,
                   NULL);

		if(INVALID_HANDLE_VALUE != hFile)
		{
			DWORD dwWriten = 0;
			if(::WriteFile(hFile,pData,nSize,&dwWriten,NULL))
			{
				bRes = dwWriten == nSize;
			}
			::CloseHandle(hFile);
		}

		return bRes;
	}

	static bool Get(const TSTRING& strKey, int& nSize, void* pData,FILETIME& time)
	{
		HANDLE hEvent = CreateEvent(NULL, FALSE, TRUE, _T("CCloudEvent"));
		CCloudLock lock(hEvent);

		bool bRes = false;

		WIN32_FIND_DATA findData = {0};
		HANDLE hFind = ::FindFirstFile(GetFile(strKey).c_str(),&findData);

		if(INVALID_HANDLE_VALUE != hFind)
		{			
			::FindClose(hFind);
			if(pData == NULL)
			{
				nSize = findData.nFileSizeLow;
				time = findData.ftLastWriteTime;
				bRes = nSize > 0;
			}
			else
			{
				HANDLE hFile = ::CreateFile(GetFile(strKey).c_str(),
					FILE_READ_DATA,
					FILE_SHARE_READ,
					NULL,
					OPEN_EXISTING,
					FILE_FLAG_BACKUP_SEMANTICS,
					NULL);

				DWORD dwRead = 0;
				if(::ReadFile(hFile,pData,nSize,&dwRead,NULL))
					bRes = dwRead == nSize;

				::CloseHandle(hFile);
			}
		}

		return bRes;
	}

protected:
	static TSTRING GetFile(const TSTRING& strFileName)
	{		
		//1. Get folder path
		TCHAR szFolder[MAX_PATH] = {0};
		bool bRes = Win32Utility::GetShellFolder(szFolder,CSIDL_PERSONAL,_T("Cloud"));

		//2. Get file name
		TSTRING strLogFileName = strFileName.empty() ? _T("cloud") : strFileName;
		strLogFileName = szFolder + strLogFileName;

		return strLogFileName;
	}
};

#endif //#ifndef _ICOUND_H_