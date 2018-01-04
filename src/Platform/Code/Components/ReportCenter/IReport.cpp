#include "StdAfx.h"
#include "IReport.h"
#include "..\..\CommonUtil\Win32SHELLUtility.h"

class IConfigure
{
public:
	virtual bool Load(const std::wstring& strStoreType = L"IniFile") =0;
	virtual bool Save(const std::wstring& strStoreType = L"IniFile") =0;
	virtual int GetCfgVar(const std::wstring& strName, int nDefault) = 0;
	virtual bool SetCfgVar(const std::wstring& strName, int nVar) = 0;
	virtual TSTRING GetCfgVar(const std::wstring& strName,const std::wstring& strDefault) = 0;
	virtual bool SetCfgVar(const std::wstring& strName, const std::wstring& strVar) = 0;
};

typedef IConfigure* (*pFnCreateIConfigure)(LPCWSTR szConfigureTitle, LPCWSTR szFolderName);
typedef bool (*pFnDeleteIConfigure)(LPCWSTR szConfigureTitle);
HMODULE g_hConfigureDll = NULL;
pFnCreateIConfigure g_CreateIConfigure = NULL;
pFnDeleteIConfigure g_DeleteIConfigure = NULL;

void Init()
{	
	if(g_hConfigureDll == NULL)
	{
#ifdef _DEBUG
		g_hConfigureDll =::LoadLibrary(_T("ConfigureTool_StaticUD.dll"));
#else
		g_hConfigureDll =::LoadLibrary(_T("ConfigureTool_StaticU.dll"));
#endif    
	}

	if(g_hConfigureDll)
	{
		if(g_CreateIConfigure == NULL)
		{
			g_CreateIConfigure = (pFnCreateIConfigure)GetProcAddress(g_hConfigureDll, "CreateIConfigure");
			g_DeleteIConfigure = (pFnDeleteIConfigure)GetProcAddress(g_hConfigureDll, "DeleteIConfigure");
		}
	}
}

void SetINIValue(const std::wstring& strName,int nLevel)
{
	Init();
	if(g_CreateIConfigure)
	{
		IConfigure* pIConfigure = g_CreateIConfigure(L"ReportUtility",L"ReportUtility");
		if(pIConfigure)
		{
			pIConfigure->Load();
			pIConfigure->SetCfgVar(strName,nLevel);
			pIConfigure->Save();
			g_DeleteIConfigure(L"ReportUtility");
		}
	}
}

int GetINIValue(const std::wstring& strName,int nDefault)
{
	int nDefaultUserMaximumLeve = nDefault;

	Init();
	if(g_CreateIConfigure)
	{
		IConfigure* pIConfigure = g_CreateIConfigure(L"ReportUtility",L"ReportUtility");
		if(pIConfigure)
		{
			pIConfigure->Load();
			nDefaultUserMaximumLeve = pIConfigure->GetCfgVar(strName,nDefaultUserMaximumLeve);
			//pIConfigure->Save();
			g_DeleteIConfigure(L"ReportUtility");
		}
	}

	return nDefaultUserMaximumLeve;
}

int GetMaximumLevel()
{
	int nValue = GetINIValue(L"UserMaximumLeve",-1);
#ifdef _DEBUG//Debug(0-9)
	int nDefaultMaximumLavel = nValue == -1 ? 9 : nValue;
#else//Release (0 - 4)
	int nDefaultMaximumLavel = nValue == -1 ? 4 : nValue;
#endif
	return nDefaultMaximumLavel;
}

bool AllowLog(const char* szClass,int nLevel)
{
	bool bRes= 1 == GetINIValue(L"EnableAllLog",1);
	//Check Level
	if(bRes)
		bRes = (GetMaximumLevel() == -1) || (GetMaximumLevel() != -1 && nLevel <= GetMaximumLevel());
	return bRes;
}

bool GetFile(const char* szFileName,char* szFolder)
{		
	//1. Get folder path
	bool bRes = Win32Utility::GetShellFolder(szFolder,CSIDL_PERSONAL,"Log");

	//2. Get file name
	if(bRes)
		bRes = TRUE == ::PathAppend(szFolder,0 == ::strlen(szFileName) ? "Default.xml" : szFileName);

	return bRes;
}

REPORTCENTER_API bool EnableLog(bool bEnable,int nUserLevel)
{
	SetINIValue(L"EnableAllLog",bEnable ? 1 : 0);
	if(nUserLevel != -1)
		SetINIValue(L"UserMaximumLeve",nUserLevel);
	return true;
}

REPORTCENTER_API bool Log(const char* szClass,int nErrorCode,int nMinorErrorCode,int nLevel,const char* strMsg)
{
	bool bRes = false;

	if(AllowLog(szClass,nLevel))
	{
		char szFilePath[MAX_PATH] = {0};
		if(GetFile(szClass + CStringA(".xml"),szFilePath))
		{
			CFile logFile;
			CFileException ex;
			if (logFile.Open(szFilePath, CFile::modeWrite |CFile::shareExclusive | CFile::modeCreate | CFile::modeNoTruncate , &ex))
			{
				const CStringA strEndMessage("\r\n\t\t</class>\r\n\t</Log>\r\n</XML>");
				DWORD nEndLength = strEndMessage.GetLength();
				ULONGLONG dwLength = logFile.SeekToEnd();
				if(dwLength == 0)
				{
					CStringA strStartMessage;
					strStartMessage.Format("<XML>\r\n\t<Log>\r\n\t\t<class name=\"%s\">",szClass);
					logFile.Write(strStartMessage,strStartMessage.GetLength());
					logFile.Write(strEndMessage,nEndLength);
					dwLength = logFile.SeekToEnd();
				}
				dwLength = logFile.Seek(dwLength - nEndLength + 1,CFile::begin);
				
				CStringA strTime = CTime::GetCurrentTime().Format(" Date=\"%Y%m%d\" Time=\"%H%M%S");				
				CStringA strMessage;
				if(strMsg)
				{
					strMessage.Format("\r\n\t\t\t\t<Log ErrorCode=\"%06d\" MinorErrorCode=\"%06d\" %s.%06d\" >\r\n\t\t\t\t\t%s\r\n\t\t\t\t</Log>",nErrorCode,nMinorErrorCode,strTime,clock(),strMsg);
				}
				else
				{
					strMessage.Format("\r\n\t\t\t\t<Log ErrorCode=\"%06d\" MinorErrorCode=\"%06d\" %s.%06d\" />",nErrorCode,nMinorErrorCode,strTime,clock());
				}
				logFile.Write(strMessage,strMessage.GetLength());
				logFile.Write(strEndMessage,nEndLength);

				logFile.Close();
			}
			bRes = true;
		}
	}
	return bRes;
}

REPORTCENTER_API bool Log_ForSPU(const char* szClass,const char* szErrorCode,const char* szMinorErrorcode,const char* szLevel,const char* strMsg)
{
	return Log(szClass,::atoi(szErrorCode),::atoi(szMinorErrorcode),::atoi(szLevel),strMsg);
}
