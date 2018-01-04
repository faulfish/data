///////////////////////////////////////////////////////////
//  CQConfigureTool.h
//  Implementation of the Class CQConfigureTool
//  Created on:      16-Jul-2007 17:18:31
//  Original author: mei
///////////////////////////////////////////////////////////

#if !defined(EA_E7F660A4_2418_4f8a_BA52_638634E2CCE1__INCLUDED_)
#define EA_E7F660A4_2418_4f8a_BA52_638634E2CCE1__INCLUDED_
#pragma once

#include "IConfigure.h"
#include <map>
#include <tchar.h>
#include <io.h>
#include <assert.h>



using namespace std;

namespace QSYNC_LIB{	

	class CQConfigureTool;
	static map<TSTRING, CQConfigureTool*> g_pCfgToolMap;

	class CQConfigureTool : public IConfigure
	{
		friend class CAccessIniFile;
	public:
		CQConfigureTool(LPCTSTR szConfigureTitle, LPCTSTR szFolderName);
		virtual ~CQConfigureTool();
		virtual int GetCfgVar(const TSTRING& strName, int nDefault);
		virtual bool SetCfgVar(const TSTRING& strName, int nVar);
		virtual TSTRING GetCfgVar(const TSTRING& strName,const TSTRING& strDefault);
		virtual bool SetCfgVar(const TSTRING& strName, const TSTRING& strVar);
		virtual bool Load(const TSTRING& strStorage);
		virtual bool Save(const TSTRING& strStorage);

	private:
		HANDLE m_hEvent; 
		std::map<TSTRING,int> m_mapInteger;
		std::map<TSTRING,TSTRING> m_mapString;
		TSTRING m_strCfgTitle;
		TSTRING m_strCfgFolder;
	};
	typedef std::map<TSTRING,int>::const_iterator cIterIntMap;
	typedef std::map<TSTRING,TSTRING>::const_iterator cIterStrMap;

	const LPCTSTR INIFILE_KEYWORD_STRING = _TC("String");
	const LPCTSTR INIFILE_KEYWORD_INT = _TC("int");
	class CAccessIniFile
	{
	public:
		CAccessIniFile(CQConfigureTool* pCfgTool);
		virtual ~CAccessIniFile();
		bool Load();
		bool Save();
	private:
		CQConfigureTool* m_pCfgTool;
		TSTRING m_strFilePath;
		TSTRING m_strSystemFilePath;
	private:
		bool CombineDataFromMap(LPCTSTR szTYPE);
		bool InsertDataIntoMap(LPCTSTR szTYPE,const TSTRING& strFilePath);
	};

	template<typename Str>// typename Str is only for TSTRING and CString
	bool Split(const Str& strSplitter, const Str& strOrg, Str& strFirst, Str& strSecond )
	{
		bool bRes = false;
		TSTRING szOrg = strOrg;
		TSTRING szSplitter = strSplitter;
		basic_string <char>::size_type nLocation = szOrg.find(strSplitter, 0);
		if( nLocation != TSTRING::npos )
		{
			strFirst = szOrg.substr(0, nLocation).c_str();
			strSecond = szOrg.substr(nLocation+szSplitter.size(), szOrg.size()-nLocation-szSplitter.size()).c_str();
			bRes = true;
		}
		else
		{
			strFirst = strOrg;
		}
		return bRes;
	}
	template<typename Str>// typename Str is only for TSTRING and CString
	bool iToTStr(int nInt, Str& strDes)
	{
		bool bRes = false;
		TCHAR pDes[50];
		if(pDes)
		{
			_itot_s(nInt, pDes, 50, 10);
			strDes = pDes;
			bRes = true;
		}
		return bRes;
	}

}//namespace QSYNC_LIB
#endif // !defined(EA_E7F660A4_2418_4f8a_BA52_638634E2CCE1__INCLUDED_)
