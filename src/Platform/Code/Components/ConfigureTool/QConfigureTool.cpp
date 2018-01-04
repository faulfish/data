#include "StdAfx.h"
#include "QConfigureTool.h"
#include "..\..\CommonUtil\Win32SHELLUtility.h"
#include <cassert>

namespace QSYNC_LIB
{
	class CProtectMutexEvent
	{
	public:
		CProtectMutexEvent(HANDLE hEvent,DWORD dwMilliseconds = INFINITE):m_hMutexEvent(hEvent)
		{
			WaitForSingleObject(m_hMutexEvent,dwMilliseconds);
		}
		~CProtectMutexEvent()
		{
			ReleaseMutex(m_hMutexEvent);
		}
	private:
		HANDLE m_hMutexEvent;
	};

	CONFIGURETOOL_DLL_DECLSPEC IConfigure* CreateIConfigure(LPCTSTR szConfigureTitle, LPCTSTR szFolderName)
	{
		CQConfigureTool* pICfgTool = NULL;
		map<TSTRING, CQConfigureTool*>::iterator iter = g_pCfgToolMap.find(szConfigureTitle);
		if(iter != g_pCfgToolMap.end())
		{
			pICfgTool = iter->second;
		}
		else
		{
			if(!szFolderName)
				szFolderName = _T("");
			pICfgTool = new CQConfigureTool(szConfigureTitle, szFolderName);
			g_pCfgToolMap[szConfigureTitle] = pICfgTool;
		}
		return pICfgTool;
	}
	CONFIGURETOOL_DLL_DECLSPEC bool DeleteIConfigure(LPCTSTR szConfigureTitle)
	{
		bool bRes = false;
		map<TSTRING, CQConfigureTool*>::iterator iter = g_pCfgToolMap.find(szConfigureTitle);
		if(iter != g_pCfgToolMap.end())
		{
			if(iter->second)
				delete iter->second;
			g_pCfgToolMap.erase(iter);
			bRes = true;
		}
		return bRes;
	}

	CQConfigureTool::CQConfigureTool(LPCTSTR szConfigureTitle, LPCTSTR szFolderName):m_strCfgTitle(szConfigureTitle),m_strCfgFolder(szFolderName)
	{
		m_hEvent = CreateEvent( 
			NULL,         // default security attributes
			TRUE,         // manual-reset event
			TRUE,         // initial state is signaled
			EVENT_CONFIGURETOOL  // object name
			); 
	}


	CQConfigureTool::~CQConfigureTool()
	{
		CloseHandle(m_hEvent);
	}

	int CQConfigureTool::GetCfgVar(const TSTRING& strName,int nDefault)
	{
		CProtectMutexEvent auto_protect(m_hEvent);

		int nRet = nDefault;
		std::map<TSTRING,int>::const_iterator iter = m_mapInteger.find(strName);
		if(iter != m_mapInteger.end())
		{
			nRet = iter->second;
		}
		return nRet;
	}

	bool  CQConfigureTool::SetCfgVar(const TSTRING& strName, int nVar)
	{
		CProtectMutexEvent auto_protect(m_hEvent);

		bool bRes = false;
		std::map<TSTRING,int>::iterator iter = m_mapInteger.find(strName);
		if(iter == m_mapInteger.end())
		{
			m_mapInteger.insert(std::pair<TSTRING,int>(strName,0));
			iter = m_mapInteger.find(strName);			
			if(iter != m_mapInteger.end())
			{
				iter->second = nVar;
				bRes = true;
			}
		}
		else
		{
			iter->second = nVar;
			bRes = true;
		}
		return bRes;
	}

	TSTRING CQConfigureTool::GetCfgVar(const TSTRING& strNameconst, const TSTRING& strDefault)
	{
		CProtectMutexEvent auto_protect(m_hEvent);

		TSTRING strRet = strDefault;
		std::map<TSTRING,TSTRING>::iterator iter = m_mapString.find(strNameconst);
		if(iter != m_mapString.end())
		{
			strRet = iter->second;
		}
		return strRet;
	}

	bool CQConfigureTool::SetCfgVar(const TSTRING& strName, const TSTRING& strVar)
	{
		CProtectMutexEvent auto_protect(m_hEvent);

		bool bRes = false;
		std::map<TSTRING,TSTRING>::iterator iter = m_mapString.find(strName);
		if(iter == m_mapString.end())
		{
			m_mapString.insert(std::pair<TSTRING,TSTRING>(strName,_TC("")));
			iter = m_mapString.find(strName);			
		}
		if(iter != m_mapString.end())
		{
			iter->second = strVar;
			bRes = true;
		}
		return bRes;
	}

	bool CQConfigureTool::Load(const TSTRING& strStorage = STORAGE_INIFILE)
	{
		CProtectMutexEvent auto_protect(m_hEvent);
		bool bRes = true;	
		if(strStorage == STORAGE_INIFILE)
		{
			CAccessIniFile AccessIni(this);
			bRes = AccessIni.Load();
		}
		return bRes;
	}


	bool CQConfigureTool::Save(const TSTRING& strStorage = STORAGE_INIFILE)
	{
		CProtectMutexEvent auto_protect(m_hEvent);
		bool bRes = true;	

		if(strStorage == STORAGE_INIFILE)
		{
			CAccessIniFile AccessIni(this);
			bRes = AccessIni.Save();
		}
		return bRes;
	}

	CAccessIniFile::CAccessIniFile(CQConfigureTool* pCfgTool) : m_pCfgTool(pCfgTool)
	{
		if(m_pCfgTool)
		{
			if(TSTRING::npos != m_pCfgTool->m_strCfgFolder.find(_T("\\")))
			{
				basic_string<TCHAR>::size_type nIndex = m_pCfgTool->m_strCfgFolder.find(_T("\\"));
				m_pCfgTool->m_strCfgFolder = m_pCfgTool->m_strCfgFolder.erase(nIndex, m_pCfgTool->m_strCfgFolder.length()- nIndex);
			}
			LPTSTR lpCfgFoler = (LPTSTR)m_pCfgTool->m_strCfgFolder.c_str();
			TCHAR szFolder[MAX_PATH] = {0};

			//Prepare user path
			bool bRes = Win32Utility::GetShellFolder(szFolder,CSIDL_PERSONAL,lpCfgFoler);
			assert(bRes);
			m_strFilePath =szFolder + m_pCfgTool->m_strCfgTitle + _TC(".ini");

			//Prepare system path
			bRes = Win32Utility::GetShellFolder(szFolder,CSIDL_COMMON_DOCUMENTS,lpCfgFoler);
			m_strSystemFilePath =szFolder + m_pCfgTool->m_strCfgTitle + _TC(".ini");
		}
	}

	CAccessIniFile::~CAccessIniFile()
	{}

	bool CAccessIniFile::Save()
	{
		bool bRes = true;
		if(!m_pCfgTool->m_mapString.empty())
			bRes = CombineDataFromMap(INIFILE_KEYWORD_STRING);
		if(bRes && !m_pCfgTool->m_mapInteger.empty())
			bRes = CombineDataFromMap(INIFILE_KEYWORD_INT);
		
		return bRes;
	}

	bool CAccessIniFile::CombineDataFromMap(LPCTSTR szTYPE)
	{
		bool bRes = false;
		TSTRING strData;
		TCHAR *szBuffer = NULL;
		if(szTYPE == INIFILE_KEYWORD_STRING)
		{
			cIterStrMap iterEnd= m_pCfgTool->m_mapString.end();
			iterEnd--;
			for(cIterStrMap iter= m_pCfgTool->m_mapString.begin(); iter != m_pCfgTool->m_mapString.end(); iter++)
			{
				strData += iter->first + _TC("=") + iter->second;
				if(iter != iterEnd)
					strData += _TC("\r\n\0");
			}
		}
		else if(szTYPE == INIFILE_KEYWORD_INT)
		{	
			cIterIntMap iterEnd= m_pCfgTool->m_mapInteger.end();
			iterEnd--;
			for(cIterIntMap iter= m_pCfgTool->m_mapInteger.begin(); iter != m_pCfgTool->m_mapInteger.end(); iter++)
			{
				TSTRING strValue;
				iToTStr(iter->second, strValue);
				strData += iter->first + _TC("=") + strValue;
				if(iter != iterEnd)
					strData += _TC("\r\n\0");
			}
		}
		szBuffer = new TCHAR[strData.size() + 2];
		::ZeroMemory(szBuffer,(strData.size() + 2)*sizeof(TCHAR));
		memcpy(szBuffer,strData.c_str(),(strData.size())*sizeof(TCHAR));
		assert((strData.size() + 2) <= 65535);
		if(szBuffer)
		{
			bRes = WritePrivateProfileSection(szTYPE, szBuffer, m_strFilePath.c_str()) == TRUE;
			delete [] szBuffer;
		}
		return bRes;
	}

	bool CAccessIniFile::Load()
	{
		TSTRING strPath;
		bool bRes = false;
		if(_taccess_s(m_strFilePath.c_str(),0) == 0 && m_pCfgTool)
		{	
			strPath = m_strFilePath;			
			bRes = true;
		}
		else if(_taccess_s(m_strSystemFilePath.c_str(),0) == 0 && m_pCfgTool)
		{
			strPath = m_strSystemFilePath;			
			bRes = true;
		}

		if(bRes)
		{
			m_pCfgTool->m_mapString.clear();
			InsertDataIntoMap(INIFILE_KEYWORD_STRING,strPath);
			m_pCfgTool->m_mapInteger.clear();
			InsertDataIntoMap(INIFILE_KEYWORD_INT,strPath);
		}

		return bRes;
	}

	bool CAccessIniFile::InsertDataIntoMap(LPCTSTR szTYPE,const TSTRING& strFilePath)
	{
		bool bRes = false;
		const DWORD dwBufferSize = 10240;
		TCHAR ac_Result[dwBufferSize] = {0};  //change size depending on needs	
		DWORD lRetValue = ::GetPrivateProfileSection(szTYPE, ac_Result, dwBufferSize, strFilePath.c_str());
		if(lRetValue == (dwBufferSize-2)){assert(0);}//need to expand buffer size

		TSTRING strSectionData;
		for(DWORD i=0; i<lRetValue; i++)
		{
			if(ac_Result[i] != _T('\0')) 
			{
				strSectionData = strSectionData + ac_Result[i];
			} 
			else 
			{
				TSTRING strKey, strValue;
				if(Split<TSTRING>(_TC("="), strSectionData, strKey, strValue))
				{
					if(szTYPE == INIFILE_KEYWORD_STRING)
					{
						m_pCfgTool->m_mapString.insert(std::pair<TSTRING,TSTRING>(strKey,strValue));
					}
					else if(szTYPE == INIFILE_KEYWORD_INT)
					{	
						int nValue= _tstoi(strValue.c_str());
						m_pCfgTool->m_mapInteger.insert(std::pair<TSTRING,int>(strKey,nValue));
					}
					bRes = true;
				}
				strSectionData.clear();
			}
		}
		return bRes;
	}


}//namespace QSYNC_LIB