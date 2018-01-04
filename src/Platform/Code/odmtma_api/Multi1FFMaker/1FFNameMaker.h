#pragma once

#include <string>
#include <vector>
#include <map>
#include "../../CommonUtil/PIMStringUtility.h"
#include <io.h>

using namespace std;

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif

class CMulti1FFNameMaker
{
public:
	CMulti1FFNameMaker(void);
	~CMulti1FFNameMaker(void);

	// Encode to .mot
	bool GetMotName(TSTRING& strMotName);
	//bool GetMotNameEx(TSTRING& strMotName);
	bool GetMotName_3(TSTRING& strMotName);

	void AddFileName(const TSTRING& strName) { m_vstrFiles.push_back(strName); }
	void ClearFileNames(){ m_vstrFiles.clear(); }
	void SetMarketName(const TSTRING& strMarketName){ m_strMarketName = strMarketName; }
	//bool MakeStringByRule(const CString& strApp, const CString& strValue, CString& strResult);
	void SetNamingRuleINIFilePath(const CString& strINIFilePath) {m_strNamingRuleINIFilePath = strINIFilePath;}

private:
	bool MakeStringByRule_3(const CString& strApp, const CString& strValue, CString& strResult);
	bool FindFileNameByHeader(const CString& strHeader, CString& strFileName);
	bool FindFileNameByExtension(const CString& strHeader, CString& strFileName);

private:

	vector<TSTRING> m_vstrFiles;  // for encode
	TSTRING m_strMarketName;
	CString m_strNamingRuleINIFilePath;

	//map<CString, vector<CString>> m_MapSWNamingRule;
	//map<CString, vector<CString>> m_MapVariantNamingRule;
	//map<CString, vector<CString>> m_MapLockNamingRule;
	map<CString, CString> m_MapOutputNamingRule;
};

