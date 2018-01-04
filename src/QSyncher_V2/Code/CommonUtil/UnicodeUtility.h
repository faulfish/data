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
	   �@�ΡGŪ�J�@��CAchieve��A�N�i�H���ɮ׸̭��Ҧ����r����X�ӡA�r��榡�ݬ�CStringA

	2. CStringA  UnicodetoASCII(CString);
	   �@�ΡG�ഫCString to CStringA

	3. CString ASCIItoUnicode(CStringA);
	   �@�ΡG�ഫCStringA to CString

	4. void WriteStringAtoFile(CArchieve&, CStringA);
	   �@�ΡG���@��CStringA�A�N�i�H��o�Ӧr��g�J���w���ɮ׸̭��A���ɮפ������e�ݥ��M�šA�̭��u��ګ��w���r��C

	1. 	CString ASCIItoUnicode(char*);
        �@�ΡG��J�@��char�}�C�����СA�i�ഫCString

	2.	void WriteAppendStringAtoFile(CArchieve&, CStringA);
     	�@�ΡG���@��CStringA�A�N�i�H��o�Ӧr��[�J���w���ɮ׸̭��A�Ӫ��[�b�쥻�ɮפ��e�����U�@�桨�F�Y�ɮ׭쥻�O�Ū��A�h���ݴ������[�C
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