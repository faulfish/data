///////////////////////////////////////////////////////////
//  Localization.cpp
//  Implementation of the Class Localization
//  Created on:      06-Feb-2007 11:25:55
///////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Localization.h"
#include "..\..\ShareLib\loki-0.1.5\include\loki\Singleton.h"
#include "..\..\CommonUtil\Win32SHELLUtility.h"
#include <io.h>

namespace QSYNC_LIB
{

	TCHAR* szTemp = NULL;

	Localization::Localization(void):m_IConfigure(CreateIConfigure(ModuleName))
	{
		if(m_IConfigure)
			m_IConfigure->Load();

		DefineNotify(TNotify_Language_Change);
		TCHAR szBuffer[32] = {0};
		
#ifdef _DEBUG
		m_strINIFolderPath = _T("..\\Debug\\localization\\");
#else
		m_strINIFolderPath = _T("localization\\");
#endif		
		InitVectorLCID(m_strINIFolderPath.c_str());
		
		m_strDefaultRegion = _T("ENU");
		if(szBuffer)
		{
			TSTRING strLang;

			TSTRING szUserDefaultLang;			
			GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVLANGNAME , szBuffer, 32);
			szUserDefaultLang = szBuffer;
			strLang = m_IConfigure->GetCfgVar(User_Language,szUserDefaultLang);

			SetRegion(m_strDefaultRegion);//for load resource string by default value

			if(-1 != FindLCIDLocation(strLang.c_str()))
			{
				SetRegion(strLang);
				m_CurrentLCID = LOCALE_USER_DEFAULT;
			}
		}

	}	


	Localization::~Localization(void)
	{
		if(m_IConfigure)
		{
			m_IConfigure->Save();
			DeleteIConfigure(ModuleName);
			m_IConfigure = NULL;
		}
	}

	void Localization::InsertData(const TSTRING& strKeyValue,const TSTRING& strRegion)
	{//Key=value
		TSTRING strKey = strRegion + TSTRING(_T(":"));
		TSTRING strValue;
		CString strTemp = strKeyValue.c_str();
		int nIndex = strTemp.Find(_T('='));

		assert(m_mapStrings.find(strKey) == m_mapStrings.end());
		//Please check if ID is repeat.

		if(nIndex >= 0)
		{
			strKey += strTemp.Left(nIndex);
			strValue = strTemp.Mid(nIndex+1);
			m_mapStrings.insert(std::pair<TSTRING,TSTRING>(strKey,strValue));
		}
	}

	void Localization::GetStringData(const TSTRING& strINIFileName,const TSTRING& strRegion)
	{
		const DWORD dwBufferSize = 10240;
		TCHAR ac_Result[dwBufferSize] = {0};  //change size depending on needs	
		DWORD lRetValue = ::GetPrivateProfileSection(_T("STRING"), ac_Result, dwBufferSize,strINIFileName.c_str());
		if(lRetValue == (dwBufferSize-2))
		{
			ASSERT(0);//need to expand buffer size
		}

		TSTRING strSectionData;
		for(DWORD i=0; i<lRetValue; i++)
		{
			if(ac_Result[i] != _T('\0')) 
			{
				strSectionData = strSectionData + ac_Result[i];
			} 
			else 
			{
				InsertData(strSectionData,strRegion);
				strSectionData.clear();
			}
		}
	}

	bool Localization::LoadRegionData(const TSTRING& strRegion)
	{
		bool bRes = false;	
#ifdef _DEBUG
		TSTRING strRegionFile = _T("..\\Debug\\localization\\") + strRegion + _T(".ini");
#else
		TSTRING strRegionFile = _T("localization\\") + strRegion + _T(".ini");
#endif	
		if(::_taccess_s(strRegionFile.c_str(),0) == 0)
		{
			GetStringData(strRegionFile.c_str(),strRegion);
			bRes = true;
		}
		else
		{
			TRACE(_T("Can't be found %s\n"),strRegionFile.c_str());
		}
		return bRes;
	}

	TSTRING Localization::GetString(const TSTRING& strID)
	{
		TSTRING strRes;
		GetString(strID,strRes);
		return strRes;
	}

	bool Localization::IsNormalStringID(const TSTRING& strID)
	{
		return (strID.empty() == false) && (strID.find(_T("IDS_")) == 0);
	}

	void Localization::GetString(const TSTRING& strID, TSTRING& strRes)
	{
		if(IsNormalStringID(strID))
		{
			std::map<TSTRING,TSTRING>::const_iterator cIter;
			cIter = m_mapStrings.find(m_strCurrentRegion + TSTRING(_T(":")) + strID);
			if(cIter != m_mapStrings.end())
				strRes = cIter->second;
			if(strRes.size() == 0 && m_strDefaultRegion!=m_strCurrentRegion)
			{
				cIter = m_mapStrings.find(m_strDefaultRegion + TSTRING(_T(":")) + strID);
				if(cIter != m_mapStrings.end())
					strRes = cIter->second;
				LogUnfoundID(m_strCurrentRegion, strID, _T("[show] ")+strRes);
			}
			if(strRes.size() == 0)
			{
				strRes = strID;
				LogUnfoundID(m_strDefaultRegion, strID, _T("[show] ")+strRes);
			}
		}
		else
		{
			strRes = strID;
		}
	}


	bool Localization::SetRegion(const TSTRING& strRegion)
	{
		bool bRes = false;
		if(m_strCurrentRegion != strRegion)
		{
			if(LoadRegionData(strRegion))
			{
				m_strCurrentRegion = strRegion;
				TSTRING strIniFilePath = m_strINIFolderPath+strRegion+_T(".ini");
				m_CurrentLCID = GetLCID(strIniFilePath.c_str());
				bRes = true;
				Fire(TNotify_Language_Change);
				if(m_IConfigure)
					m_IConfigure->SetCfgVar(User_Language,strRegion);
			}
		}
		return bRes;
	}

	typedef Loki::SingletonHolder<Localization> LocalizationSingleton;

	LOCALIZATION_DLL_DECLSPEC bool CreateILocalization(ILocalization** ppILocalization)
	{
		*ppILocalization = &LocalizationSingleton::Instance();
		return true;
	}

	bool Localization::Register(INotify* pNotify,const TSTRING& strEvent)
	{
		return CNotifyBase::Register(pNotify,strEvent);
	}

	bool Localization::Register(EventFunc* pEventFunc,const TSTRING& strEvent)
	{
		return CNotifyBase::Register(pEventFunc,strEvent);
	}

	bool Localization::Unregister(INotify* pNotify,const TSTRING& strEvent)
	{
		return CNotifyBase::Unregister(pNotify,strEvent);
	}

	bool Localization::Unregister(EventFunc* pEventFunc,const TSTRING& strEvent)
	{
		return CNotifyBase::Unregister(pEventFunc,strEvent);
	}

	void Localization::InitVectorLCID(LPCTSTR lpFolderPath)
	{
		CFileFind finder;

		CString strWildcard(lpFolderPath);
		strWildcard += _T("*.ini");
		BOOL bIsFileFound = finder.FindFile(strWildcard);

		WCHAR szBuffer[32] = {0};
		WCHAR szFileTitle[32] = {0};
		CString strFileTitle;
		if(szBuffer)
		{
			while(bIsFileFound)
			{
				bIsFileFound = finder.FindNextFile();

				LCID lcid=GetLCID((LPCTSTR)finder.GetFilePath());

				GetLocaleInfo(lcid,  LOCALE_SNATIVELANGNAME , szBuffer, 32);
				GetLocaleInfo(lcid,  LOCALE_SABBREVLANGNAME , szFileTitle, 32);
				strFileTitle=szFileTitle;

				/*
				*  Make sure:
				*  1. Menu items won't be empty
				*  2. lcid exists [0 perhaps equals LOCALE_USER_DEFAULT]
				*  3. FileTitle is exactly what lcid creates
			 */
				if(0 != _tcslen(szBuffer) && lcid!=0 && strFileTitle.CompareNoCase(finder.GetFileTitle()) == 0)
				{
					m_vectorLCID.push_back(lcid);
				}		
			}
		}
		finder.Close();

	}


	LCID Localization::GetLCID(LPCTSTR lpINIFilePath)
	{
		TCHAR cpLCID[10];
		TCHAR* cpStop = NULL;

		DWORD lRetValue = ::GetPrivateProfileString(_T("MENU"),_T("LCID"),NULL,cpLCID,10,lpINIFilePath);
		assert(lRetValue >= 0);
		LCID lcid=_tcstol(cpLCID, &cpStop, 16);
		return lcid;
	}


	/*
	*  FindLCIDLocation: 
	*  if the Region can't be found in vector, the return value will be -1.  
	*/
	int Localization::FindLCIDLocation(LPCTSTR lpRegion)
	{
		TCHAR szBuffer[32] = {0};
		int nCount = 0;

		if(szBuffer)
		{
			vector <LCID>::iterator IterLCID;
			for(IterLCID=m_vectorLCID.begin(); IterLCID!=m_vectorLCID.end(); IterLCID++)
			{	

				GetLocaleInfo((*IterLCID),  LOCALE_SABBREVLANGNAME , szBuffer, 32);
				if(0 == _tcscmp(szBuffer,lpRegion))
				{
					return nCount;
					break;
				}
				nCount++;
			}
		}

		return -1;
	}

	int Localization::GetRegionNum(void)
	{
		return static_cast<int>(m_vectorLCID.size());
	}

	void Localization::GetRegion(int nIndex, TSTRING& strRegion)
	{
		WCHAR szBuffer[32] = {0};
		if(szBuffer)
		{
			if(nIndex < static_cast<int>(m_vectorLCID.size()) && nIndex >= 0)
			{
				GetLocaleInfo(m_vectorLCID[nIndex],  LOCALE_SABBREVLANGNAME , szBuffer, 32);
				strRegion = szBuffer;
			}
		}
	}

	void Localization::GetRegionTitle(int nIndex, TSTRING& strRegionTitle)
	{
		WCHAR szBuffer[32] = {0};
		if(szBuffer)
		{
			GetLocaleInfo(m_vectorLCID.at(nIndex),  LOCALE_SNATIVELANGNAME , szBuffer, 32);
			strRegionTitle = szBuffer;
		}

	}

	void Localization::GetCurrentRegion(TSTRING& strRegion)
	{
		strRegion= m_strCurrentRegion;
	}

	void Localization::GetDefaultRegion(TSTRING& strRegion)
	{
		strRegion= m_strDefaultRegion;
	}
	LCID Localization::GetCurrentLCID()
	{
		return m_CurrentLCID;
	}
	bool Localization::GetDate(LCID lcid, int nYear, int nMonth, int nDay, LPCTSTR lpFormat, TSTRING& strDate, DWORD dwflags)	
	{
		bool bRes = true;
		SYSTEMTIME SystemTime;
		SystemTime.wYear = nYear;
		SystemTime.wMonth = nMonth;
		SystemTime.wDay = nDay;

		LPTSTR lpDate;
		int nLength = ::GetDateFormat(lcid, dwflags, &SystemTime, lpFormat, NULL, 0);
		if(nLength != 0)
		{
			lpDate = new TCHAR[nLength];
			::ZeroMemory(lpDate, nLength);
			::GetDateFormat(lcid, dwflags, &SystemTime, lpFormat, lpDate, nLength);
			strDate = lpDate;
			delete [] lpDate;
		}
		else
		{
			bRes = false;
		}
		return bRes;
	}
	bool Localization::GetTime(LCID lcid, int nHour, int nMinute, int nSecond, LPCTSTR lpFormat, TSTRING& strTime, DWORD dwflags)
	{
		bool bRes = true;
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);
		SystemTime.wHour = nHour;
		SystemTime.wMinute = nMinute;
		SystemTime.wSecond = nSecond;

		int nLength = ::GetTimeFormat(lcid, dwflags, &SystemTime, lpFormat, NULL, 0);
		if(nLength != 0)
		{
			LPTSTR lpTime = new TCHAR[nLength];
			::ZeroMemory(lpTime, nLength);
			::GetTimeFormat(lcid, dwflags, &SystemTime, lpFormat, lpTime, nLength);
			strTime = lpTime;
			delete [] lpTime;
		}
		else
		{
			bRes = false;
		}
		return bRes;
	}	
	bool Localization::LogUnfoundID(const TSTRING& strRegion, const TSTRING& strID, const TSTRING& strValue)
	{
		TCHAR szFolder[MAX_PATH] = {0};
		bool bRes = Win32Utility::GetShellFolder(szFolder,CSIDL_PERSONAL,_T("QSyncer"));
		assert(bRes);
		TSTRING strFilePath = szFolder + strRegion + _T(".ini");

		bRes = WritePrivateProfileString(_T("Not Found"), strID.c_str(), strValue.c_str(), strFilePath.c_str()) == TRUE;
		return bRes;
	}

}

