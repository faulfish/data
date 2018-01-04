#ifndef _CQVERSIONPARSER_HPP__
#define _CQVERSIONPARSER_HPP__
#pragma once

#include <string>
#include "../../CommonUtil/PIMStringUtility.h"
#include "afxmt.h"

namespace ODMTMA_LIB
{

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif

class CQVersionParser
{
public:
	CQVersionParser();
	~CQVersionParser();
	bool IsCodeSigned(const TSTRING& strAmss, const TSTRING& strOemsbl, const TSTRING& strQcsbl);
	bool IsCodeSigned(const TSTRING& strAmss, const TSTRING& strOemsbl);
	int GetPhoneCodeVersion(const TSTRING& strAmss);
	int GetBootloaderVersion(const TSTRING& strOemsbl);

private:
	int GetVersion(const TSTRING& strFilePath, const TSTRING& strSearchWord);
private:
	CCriticalSection m_CritAMSS;
	CCriticalSection m_CritOemsbl;
	CCriticalSection m_CritQcsbl;
};


} // namespace ODMTMA_LIB

#endif //_CQVERSIONPARSER_HPP__