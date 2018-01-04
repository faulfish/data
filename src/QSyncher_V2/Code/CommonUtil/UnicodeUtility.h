#ifndef _UNICODE_UTILITR_H_
#define _UNICODE_UTILITR_H_
#pragma once
#include <string>
#include <list>
#include <vector>
#include <atlenc.h>
#include <cstringt.h>
#include <stdlib.h>
#include <time.h>

namespace QSYNC_LIB 
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

	using namespace std;
	/*
	(in)	pStr	null terminal string.
	(out)	wchar_t	Can't be null. Buffer of return string.
	(in)	int		bufferSize by byte
	*/
	void AnsiToUnicodeBuffer(const char *pStr,wchar_t *pDestBuffer,int bufferSize);

	/*
	(in)	pStr	null terminal string.
	(out)	wchar_t	Can't be null. Buffer of return string.
	(in)	int		bufferSize by byte
	*/
	void UnicodeToAnsiBuffer(const wchar_t *pStr,char *pDestBuffer,int bufferSize);

	/*
	(in)	szHEX	null terminal string like 04AB, 4AB , 4A , 4 ,...etc.
	(out)	DWORD	value of HEX
	*/
	DWORD HEX2Ineger(LPCTSTR szHEX);

	/*
	1. CStringA  ReadFileStringA(CArchieve&);
	   作用：讀入一個CAchieve後，就可以把檔案裡面所有的字串取出來，字串格式需為CStringA

	2. CStringA  UnicodetoASCII(CString);
	   作用：轉換CString to CStringA

	3. CString ASCIItoUnicode(CStringA);
	   作用：轉換CStringA to CString

	4. void WriteStringAtoFile(CArchieve&, CStringA);
	   作用：給一個CStringA，就可以把這個字串寫入指定的檔案裡面，但檔案內的內容需先清空，裡面只放我指定的字串。

	1. 	CString ASCIItoUnicode(char*);
        作用：丟入一個char陣列的指標，可轉換CString

	2.	void WriteAppendStringAtoFile(CArchieve&, CStringA);
     	作用：給一個CStringA，就可以把這個字串加入指定的檔案裡面，而附加在原本檔案內容的”下一行”；若檔案原本是空的，則不需換行後附加。
    */
	CStringA	ReadFileStringA(CArchive& arFile);
	CStringA	UnicodetoASCII(const CString& strTemp);
	CString		ASCIItoUnicode(const CStringA& strAnsi);
	void		WriteStringAtoFile(CArchive& arFile, const CStringA& strAnsi);
	CString		ASCIItoUnicode(const char* szAnsi);
	void		WriteAppendStringAtoFile(CArchive& arFile, const CStringA& strAnsi);

	//if CodePage == 0 (default), CodePage will be CP_ACP. (ASCII)
	CString CharToWideChar(const CStringA& strSrc, UINT CodePage);
	CStringA WideCharToChar(const CString& strSrc, UINT CodePage);
	string widetoansi(const wstring& strSrc, UINT CodePage = CP_ACP);
	wstring ansitowide(const string& strAnsi, UINT CodePage = CP_ACP);

	TSTRING GetTSTRING(const std::string& strAnsi, UINT CodePage = CP_ACP);
	TSTRING GetTSTRING(const std::wstring& strUnicode, UINT CodePage = CP_ACP);
	std::string GetAnsi(const TSTRING& tstring, UINT CodePage = CP_ACP);
	std::wstring GetUnicode(const TSTRING& tstring, UINT CodePage = CP_ACP);

	bool QPEncoder( const TSTRING& strOrg, TSTRING& strDes, UINT CodePage);
	bool QPDecoder( const TSTRING& strOrg, TSTRING& strDes, UINT CodePage );
	bool Base64Encoder( const TSTRING& strOrg, TSTRING& strDes, UINT CodePage );
	bool Base64Decoder( const TSTRING& strOrg, TSTRING& strDes, UINT CodePage );

	CString UTF8ToUCS2(const std::string& strUTF8);
}

#endif //_UNICODE_UTILITR_H_