#pragma warning(disable : 4996)

#ifndef __COMMON_UTIL_H
#define __COMMON_UTIL_H
/*
	This file is modified for VC6.
	by Kevin Tai 2008/11/04
  */

#include "TypeDef.h"
#include "EnumFiles.h"

//#include <atltime.h>
///////////////////////////////////////////////////////////////////////////////////////////
//
//	ASSERT
//
///////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZeroMemory
inline void ZeroMemory(void *p, size_t st)
{
	memset(p, 0, st);
}
#endif //ZeroMmemory

inline int OutputAssertInfo(
							const _TCHAR* pszExpression,						
							const _TCHAR* pszFileName,						
							int nLineNo,
							FILE* pFile = stderr
							)
{
	return _ftprintf(pFile,
		_T("**********************************************************************************")
		_T("Assertion failed!\n")
		_T("	Expression: %s\n")
		_T("		File: %s\n")
		_T("		Line: %d\n")
		_T("**********************************************************************************"),
		pszExpression, 
		pszFileName,
		nLineNo
		);

}

///////////////////////////////////////////////////////////////////////////////////////////
//
//	MACRO
//
///////////////////////////////////////////////////////////////////////////////////////////
#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember)) 


#ifndef ASSERT
#ifdef _DEBUG
#include <assert.h>
#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define ASSERT(exp)				{if(!(exp)) {OutputAssertInfo(_T(#exp), _T(__FILE__), __LINE__); assert(0);\
	assert(0); } }
#define ASSERT_MSG(exp, err)	{if(!(exp)) {OutputAssertInfo(err, _T(__FILE__), __LINE__); assert(0);\
	assert(0); } }
#else //!_DEBUG
#define ASSERT(exp)
#define ASSERT_MSG(exp)
#endif //_DEBUG	
#endif //ASSERT
#ifdef UNICODE
	#define TESTorLOG(module, level, err)	m_piLogger(CStr::ToAnsiString(module).c_str(), __FILE__, __LINE__, level, CStr::ToAnsiString(err)##.c_str()); 
#else
	#define TESTorLOG(module, level, err)	m_piLogger(module, __FILE__, __LINE__, level, err##.c_str()); 
		
#endif //UNICODE

//#define WRITE_LOG(exp, module, level, err)	m_piLogger->LogTraceLine(module, level, err); 

inline void HRTest(HRESULT hr, const String& strErrMsg)
{
//	ASSERT_MSG(!FAILED(hr), strErrMsg.c_str());
	if(FAILED(hr)) 
	{
		OutputAssertInfo(strErrMsg.c_str(), _T(__FILE__), __LINE__); 
		assert(0);
	}
}

#define V_EXIT(s)						if(!(s)) {goto _exit;}
#define V_RET(s)						if(!(s)) {return;}
#define V_CONTINUE(s)					if(!(s)) {continue;}


#define ASSERT_RET(exp)                 { ASSERT(exp); if (!(exp)) { return; } }
#define ASSERT_RET_NULL(exp)            { ASSERT(exp); if (!(exp)) { return NULL; } }
#define ASSERT_EXIT(exp)                { ASSERT(exp); if (!(exp)) { goto _exit; } }
#define ASSERT_RET_FALSE(exp)           { ASSERT(exp); if (!(exp)) { return false; } }
#define ASSERT_CONT(exp)                { ASSERT(exp); if (!(exp)) { continue; } }
#define ASSERT_BREAK(exp)               { ASSERT(exp); if (!(exp)) { break; } }

#define SIZET(T)						T::size_type

#define EMPTY_SZ						""
#define EMPTY_WS						L""
#define EMPTY_TSZ						TEXT("")
#define EMPTY_WSZ						L""

#define VK_0							WORD('0')
#define VK_9							WORD('9')



// size of array
#define SIZEOF(x)						(sizeof(x)/sizeof(x[0]))


template<typename Array>
inline size_t ArraySize(const Array& ar)
{
	size_t stArray= sizeof(ar);
	size_t stElement = sizeof(ar[0]);
	return (sizeof(ar)/sizeof(ar[0]));
}

// size of String						
inline DWORD StrSize(const String& str) {return static_cast<DWORD>(sizeof(String::value_type) * str.size());}

//String Macro
//
// STR:		String
// BSTR:	BSTR*
// SZ:		char*
// WSZ:		wcahr_t*
// WSTR:	WideString

#ifdef UNICODE


inline LPCWSTR TESTTSZ(LPCWSTR wsz) {return (wsz ? wsz : EMPTY_TSZ);}
inline LPCSTR TESTSZ(LPCSTR sz) {return (sz ? sz : EMPTY_SZ);}
inline LPCWSTR TESTWSZ(LPCWSTR wsz) {return (wsz ? wsz : EMPTY_WSZ);}


//STR2
#define STR2TSZ(tstr)				TESTTSZ((tstr).c_str())
#define STR2CSTR(tstr)				String(STR2TSZ(tstr))
#define STR2BSTR(tstr)				TESTWSZ((tstr).c_str())
#define STR2WSZ(tstr)				TESTWSZ((tstr).c_str())

//CSTR2
#define CSTR2STR(cstr)				String(static_cast<LPCWSTR>(cstr))

//BSTR2
#define BSTR2STR(bs)				String(TETSWSZ(cs))
#define BSTR2CSTR(bs)				BSTR2STR(bs)
#define BSTR2SZ(bs)					TESTZS(static_cast<LPCWSTR>(BSTR2CSTR(bs)))
#define BSTR2TSZ(bs)				TESTWSZ(bs)
#define BSTR2WSZ(bs)				TESTWSZ(bs)

//WSZ2
#define WSZ2STR(wsz)				String(TETSWSZ(wsz))

//SZ2
#define SZ2CSTR(sz)					String(TETSSZ(sz))
#define STR2STR(sz)					String(static_cast<LPCWSTR>(SZ2CSTR(sz)))

#endif //UNICODE


//WinDef.h
typedef unsigned long       DWORD;
typedef int INT;
typedef DWORD far           *LPDWORD;
typedef DWORD far           *LPDWORD;
typedef void far            *LPVOID;

//WinNT.h
#ifndef _WINNT_
typedef __nullterminated CONST WCHAR *LPCWSTR, *PCWSTR;
typedef __nullterminated CONST WCHAR *LPCWSTR, *PCWSTR;
typedef __nullterminated WCHAR *NWPSTR, *LPWSTR, *PWSTR;
typedef LPWSTR PTSTR, LPTSTR;

#ifdef STRICT
typedef void *HANDLE;
#define DECLARE_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name
#else
typedef PVOID HANDLE;
#define DECLARE_HANDLE(name) typedef HANDLE name
#endif
typedef HANDLE *PHANDLE;
#endif//_WINNT_


//Macro for UILog

#define LogTrace(level, msg)	TraceLog(_T("RFCalTest"), level, __FILE__, __LINE__, msg);
//#define TestPassLog(iUL, iDL, iMea,msg)	FactoryLog(m_iItemCode, 0, iUL, iDL, iMea, TRUE, msg);
//#define TestFailLog(err, iUL, iDL, iMea, msg)	FactoryLog(m_iItemCode, err, iUL, iDL, iMea, FALSE, msg);
#define TestPassLog(strBand, strChannel, iUL, iDL, iMea, strUnit, msg)	FactoryLog(m_iItemCode, strBand, strChannel, 0, iUL, iDL, iMea, strUnit,TRUE, msg);
#define TestFailLog(strBand, strChannel, err, iUL, iDL, iMea,  strUnit, msg)	FactoryLog(m_iItemCode, strBand, strChannel, err, iUL, iDL, iMea, strUnit, FALSE, msg);

#define CalPassLog(strBand,  msg)	FactoryLog(m_iItemCode, strBand, _T("-"), 0, _T("1"), _T("0"), _T("1"), _T("-"), TRUE, msg);
#define CalFailLog(strBand, err, msg)	FactoryLog(m_iItemCode, strBand, _T("-"), err, _T("1"), _T("0"), _T("0"), _T("-"), FALSE, msg);
//#define CalPassLog(msg)	FactoryLog(m_iItemCode, 0, _T("1"), _T("0"), _T("1"), TRUE, msg);
//#define CalFailLog(err, msg)	FactoryLog(m_iItemCode, err, _T("1"), _T("0"), _T("0"), FALSE, msg);
#define LogPhoneStartCommand(msg)	CommandLog(_T("QMSL_LOG_C_HIGH_LEVEL_START"), msg)
#define LogPhoneStopCommand(msg)	CommandLog(_T("QMSL_LOG_C_HIGH_LEVEL_STOP"), msg)

// Working directory
#define ChangeWorkingDirectoryTo(strWorkingDirectory) \
	CString strOriWorkingDirectory;	\
	_tgetcwd(strOriWorkingDirectory.GetBuffer(_MAX_PATH), _MAX_PATH);	\
	strOriWorkingDirectory.ReleaseBuffer();	\
	_tchdir(strWorkingDirectory);

#define ChangeWorkingDirectoryBack() \
	_tchdir(strOriWorkingDirectory);


///////////////////////////////////////////////////////////////////////////////////////////
//
//	CompareNumber<>
//
///////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
inline int CompareNumber(
						 const T& v1, //Left
						 const T& v2)	//Right	
{
	if(v1 > v2){
		return 1;
	}else if (v1 < v2){
		return -1;
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//	CRandomGenerator
//
///////////////////////////////////////////////////////////////////////////////////////////
class CRandomGenerator{
public:
	CRandomGenerator(){}
	void InitData(DWORD dwRandomNumReq, DWORD dwSeed){
		m_vRandom.resize(dwRandomNumReq);
		srand(dwSeed);
		for(DWORD x = 0; x < dwRandomNumReq; ++x){
			m_vRandom[x] = rand();
		}
	}	
	int GetRand(DWORD dwIndex) {return m_vRandom[dwIndex];}

private:
	vector<int> m_vRandom;
};
typedef CRandomGenerator* PCRandomGenerator;



///////////////////////////////////////////////////////////////////////////////////////////
//
//	MyBinarySearch()
//
///////////////////////////////////////////////////////////////////////////////////////////

template<typename _FI, typename _Ty>
inline _FI MyBinarySearch(
						  _FI			_F,			//first iterator					  
						  _FI			_L,			//last iterator					  
						  const _Ty&	_V			//value to find					  
						  )
{
	_FI I = lower_bound(_F, _L, _V);
	if(_I != _L && !(_V < *_I)) {return _I;}
	return _L;
}

template<typename _FI, typename _Ty, typename _Pr>
inline _FI MyBinarySearch(
						  _FI			_F,			//first iterator					  
						  _FI			_L,			//last iterator					  
						  const _Ty&	_V,			//value to find					  
						  _Pr			_P
						  )
{
	_FI I = lower_bound(_F, _L, _V, _P);
	if(_I != _L && !_P(_V < *_I)) {return _I;}
	return _L;
}

class CompareSizeVtr{
public:
	bool operator()(const SizeVtr& v1, const SizeVtr& v2) const
	{
		int nComp = 0;
		size_t stFeaNum = v1.size();
		for(size_t x = 0; x < stFeaNum; ++x){
			nComp = static_cast<int>(v1[x]) - static_cast<int>(v2[x]) ;
			if(0 != nComp) {break;}
		}
		return nComp < 0;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////
//
//	CStr
//
///////////////////////////////////////////////////////////////////////////////////////////
class CStr{
public:

	static bool LoadFormFile(const String& strFileName, StrVtr& vLine, bool fSkipEmptyLine = false);
	static bool SaveToFile(const String& strFileName, StrVtr& vLine);

	static void ToUpper(String& str);
	static void ToLower(String& str);

	static String ToHexStr(_TCHAR tValue)
	{
		_TCHAR wsBuff[50];
/*
#ifdef UNICODE 
		_stprintf(wsBuff, ArraySize(wsBuff), _T("%04X"), tValue);
#else
		_stprintf(wsBuff, _T("%04X"), tValue);
#endif
*/
		_sntprintf(wsBuff, ArraySize(wsBuff), _T("%04X"), tValue);
		return String(wsBuff);
	}

	static String ToHexStr(size_t tValue)
	{
		_TCHAR wsBuff[50];
/*
#ifdef UNICODE
		_stprintf(wsBuff, ArraySize(wsBuff), _T("%08X"), tValue);
#else
		_stprintf(wsBuff, _T("%08X"), tValue);
#endif
*/
		_sntprintf(wsBuff, ArraySize(wsBuff), _T("%08X"), tValue);
		return String(wsBuff);
	}

	static String SizeToStr(size_t stValue)
	{
		_TCHAR wsBuff[50];
/*
#ifdef UNICODE
		_stprintf(wsBuff, ArraySize(wsBuff), _T("%d"), stValue);
#else
		_stprintf(wsBuff, _T("%d"), stValue);
#endif
*/
		_sntprintf(wsBuff, ArraySize(wsBuff), _T("%d"), stValue);
		return String(wsBuff);
	}

	static String IntToStr(int nValue)
	{
		_TCHAR wsBuff[50];
/*
#ifdef UNICODE
		_stprintf(wsBuff, ArraySize(wsBuff), _T("%d"), nValue);	
#else
		_stprintf(wsBuff, _T("%d"), nValue);
#endif
*/
		_sntprintf(wsBuff, ArraySize(wsBuff), _T("%d"), nValue);	
		return String(wsBuff);
	}
	static String UShortToStr(unsigned short nValue)
	{
		_TCHAR wsBuff[50];
/*
#ifdef UNICODE
		_stprintf(wsBuff, ArraySize(wsBuff), _T("%hu"), nValue);		
#else
		_stprintf(wsBuff, _T("%hu"), nValue);
#endif
*/
		_sntprintf(wsBuff, ArraySize(wsBuff), _T("%hu"), nValue);
		return String(wsBuff);
	}
	static String FloatToStr(double flValue)
	{
		_TCHAR wsBuff[50];
/*
#ifdef UNICODE
		_stprintf(wsBuff, ArraySize(wsBuff), _T("%f"), flValue);		
#else
		_stprintf(wsBuff, _T("%f"), flValue);
#endif
*/
		_sntprintf(wsBuff, ArraySize(wsBuff), _T("%f"), flValue);		
		return String(wsBuff);
	}

	static MYDOUBLE StrToFloat(const String& str)
	{
#ifdef UNICODE
		return static_cast<MYDOUBLE>(_wtof(str.c_str()));
#else
		return static_cast<MYDOUBLE>(atof(str.c_str()));
#endif
	}
	static double StrToDouble(const String& str)
	{
		_TCHAR* stopstring;
#ifdef UNICODE
		return static_cast<double>(wcstod(str.c_str(), &stopstring));
#else
		return static_cast<double>(strtod(str.c_str(), &stopstring));
#endif
	}

	static int StrToInt(const String& str)
	{
#ifdef UNICODE
		return _wtoi(str.c_str());
#else
		return atoi(str.c_str());
#endif
	}

	static size_t StrToSize(const String& str)
	{
		return static_cast<size_t>(StrToInt(str));
	}

	static bool StrToBool(const String& str)
	{
		String strLower = str;
		CStr::ToLower(strLower);
		return strLower == _T("1")
			|| strLower == _T("yes")
			|| strLower == _T("true")
			|| strLower == _T("¬O")
			|| strLower == _T("­n")
			|| strLower == _T("¹ï")
			;
	}

	static wstring ToWideString(const string& strAnsi)
	{
		wchar_t* pws = NULL;
		wstring strWide;
		size_t stSize = strAnsi.size() + 1;
		if(stSize > 1){
			if(pws = new wchar_t[stSize]){
				ZeroMemory(pws, sizeof(wchar_t) * stSize);
#if _MSC_VER >= 1400
				size_t stConverted = 0;
//				mbstowcs_s(&stConverted, pws, stSize, strAnsi.c_str(), stSize-1);
				mbstowcs(pws, strAnsi.c_str(), stSize-1);
#else
				mbstowcs(pws, strAnsi.c_str(), stSize-1);
#endif //_MSC_VER >= 1400
				strWide = pws;
				delete pws; pws = NULL;
			}
		}
		return strWide;
	}

	static string ToAnsiString(const wstring& strWide)
	{
		char* ps = NULL;
		string strAnsi;
		size_t stSize = sizeof(strWide[0]) * (strWide.size() + 1);
		if(stSize > 1){
			if(ps = new char[stSize]){
				ZeroMemory(ps, sizeof(ps[0]) * stSize);
#if _MSC_VER >= 1400
				size_t stConverted = 0;
//				wcstombs_s(&stConverted, ps, stSize, strWide.c_str(), stSize-1);
				wcstombs(ps, strWide.c_str(), stSize-1);
#else
				wcstombs(ps, strWide.c_str(), stSize-1);
#endif //_MSC_VER >= 1400
				strAnsi = ps;
				delete ps; ps = NULL;
			}
		}
		return strAnsi;
	}

	static String& TrimStr(
		String&			str,
		const String&	strTrim = _T(" \t\r\n")
		);

	static int ReplaceString(
		String&			strSource,
		const String&	strFind,
		const String&	strReplace
		);

	static bool ParseStringAll(
		const String&	strSource,
		const String&	strDelimiter,
		StrVtr&			vList,
		bool			fAppendDelimiter = false
		);

	static bool ParseString(
		const String&	strSource,
		const String&	strDelimiter,
		StrVtr&			vList,
		bool			fAppendDelimiter = false
		);

	static size_t GetPatternCount(
		const String&	strToCount,
		const String&	strPattern
		);

	static void ComposeString(
		const StrVtr&	vList,
		const String&	strDelimiter,
		String&			strResult,
		bool			fAppendEnd = false
		);

	static void SplitWordVector(
		const String&	strWord,
		size_t			stTokenLen,
		StrStrVtr&		vSplitWord
		);

	static String LongToStr(long lVal)
	{
		_TCHAR buff[20];
#ifdef UNICODE
		_ltow(lVal, buff,  10);
#else
		ltoa(lVal, buff, 10);
#endif
		return String(buff);
	}

	static String ULongToStr(unsigned long ulVal)
	{
		_TCHAR buff[20];
#ifdef UNICODE
		_ultow(ulVal, buff, 10);
#else
		ultoa(ulVal, buff, 10);
#endif
		return String(buff);
	}
	static String DblToStr(double dblVal)
	{
		_TCHAR buff[20];
//		StringCchPrintf(buff, SIZEOF(buff), TEXT("%f"), dblVal);
/*
#ifdef UNICODE
		swprintf(buff, SIZEOF(buff), TEXT("%f"), dblVal);
#else
		sprintf(buff, TEXT("%f"), dblVal);
#endif
*/
		_sntprintf(buff, SIZEOF(buff), TEXT("%f"), dblVal);
		return String(buff);	
	}

	static String DblToStr(float fltVal)
	{
		return DblToStr(static_cast<DOUBLE>(fltVal));
	}

	static String DwordToStr(DWORD dwVal)
	{
		return IntToStr(static_cast<int>(dwVal));
	}


	//Draw function
	static BOOL GetStrDispSize(
		HDC				hDC,
		const String&	strString,
		SIZE&			size,
		StrSizeT		stPos,
		StrSizeT		stLen
		)
	{
		return GetTextExtentPoint32(hDC, strString.c_str() + stPos, static_cast<INT>(stLen), &size);
	}

	static LONG GetStrDispWidth(
		HDC				hDC,
		const String&	strString,
		StrSizeT		stPos,
		StrSizeT		stLen
		)
	{
		SIZE size;
		if(CStr::GetStrDispSize(hDC, strString, size, stPos, stLen)) {return size.cx;}
		return -1;
	}

	static LONG GetStrDispHeight(
		HDC				hDC,
		const String&	strString,
		StrSizeT		stPos,
		StrSizeT		stLen
		)
	{
		SIZE size;
		if(CStr::GetStrDispSize(hDC, strString, size, stPos, stLen)) {return size.cy;}
		return -1;
	}

	static BOOL GetStrDispSize(
		HDC				hDC,
		const String&	strString,
		SIZE&			size
		)
	{
		return CStr::GetStrDispSize(hDC, strString, size, 0, strString.size());
	}


protected:
	static void DoSplitWordVector(
		const _TCHAR*	pwsWord,
		bool			fTailWord,
		size_t			stTokenLen,
		StrStrVtr&		vSplitWord
		);

private:
	static char* m_pszLocale;
	static locale m_loc;

};


///////////////////////////////////////////////////////////////////////////////////////////
//
//	CFileBase
//
///////////////////////////////////////////////////////////////////////////////////////////
class CFileBase{
public:
	CFileBase() : m_pFile(NULL){}
	virtual ~CFileBase(){}

	virtual bool Open(const String& strFileName) = 0;
	virtual bool Close(void) = 0;

	FILE* GetHandle(void) {return m_pFile;}
	DWORD GetSize(LPDWORD lpdwSizeHigh = NULL){ return ::GetFileSize(m_pFile, lpdwSizeHigh);}
	bool IsOpen(void) {return (NULL != m_pFile);}
	bool IsEndOfFile(void) {return 0 != feof(m_pFile);}
	long GetPointer()
	{
		return ::ftell(m_pFile);
	}
	int SetPointer(long lnDistance, int nOrigin = SEEK_CUR)
	{
		return ::fseek(m_pFile, lnDistance, nOrigin);
	}

	int MoveFileBegin(void)
	{
		return this->SetPointer(0, SEEK_SET);
	}

	int MoveFileEnd(void)
	{
		return this->SetPointer(0, SEEK_END);
	}

	static FILE* MyOpenFile(
		const String&	strFileName,
		const char*	psMode
		)
	{
#if	_MSC_VER >=1400
		FILE* pFile = NULL;
#ifdef UNICODE
		::fopen_s(&pFile, CStr::ToAnsiString(strFileName).c_str(), psMode);
#else
		::fopen_s(&pFile, strFileName.c_str(), psMode);
#endif
		return pFile;
#else
#ifdef UNICODE
		return ::fopen(CStr::ToAnsiString(strFileName).c_str(), psMode);
#else
		return ::fopen(strFileName.c_str(), psMode);
#endif

#endif //_MSC_VER >=1400
	}

protected:
	FILE*		m_pFile;
};
typedef CFileBase* PCFileBase;

///////////////////////////////////////////////////////////////////////////////////////////
//
//	CReadBase
//
///////////////////////////////////////////////////////////////////////////////////////////

class CReadFile : public CFileBase{
public:
	CReadFile(
		bool fTextMode = true	
		);
	~CReadFile();

	virtual bool Open(const String& strFileName);
	virtual bool Close(void);

#ifdef WIN32
	LPVOID GetMapView(void);
#endif

	bool Rewind(void);
	bool Read(void* pvBuff, size_t stSize);
	bool ReadLine(String& strLine);

	bool IsUnicode(void){return m_fUnicode;}

	virtual bool Open(LPCTSTR lpcwsFileName){
		return Open(String(lpcwsFileName));
	}
	BOOL Read(LPVOID lpBuff, DWORD dwSize, LPDWORD lpdwByteRead);
	BOOL ReadLine(LPTSTR lptszBuff, DWORD dwSize);

private:
	bool		m_fTextMode;
	bool		m_fUnicode;
	HANDLE		m_hFileMap;
	LPVOID		m_pViewOfFileMap;


};
typedef CReadFile* PCReadFile;


///////////////////////////////////////////////////////////////////////////////////////////
//
//	CWriteBase
//
///////////////////////////////////////////////////////////////////////////////////////////

class CWriteFile : public CFileBase{
public:
	CWriteFile(bool fAppend = false, bool fTryStdout = false);
	~CWriteFile();

	virtual bool Open(const String& strFileName);
	virtual bool Close(void);

#ifdef WIN32
	LPVOID GetMapView(void);
#endif //WIN32

	void SetAppend(bool fAppend); 
	bool WriteUnicodeHeader(void);
	bool Write(const void* pvBuff, size_t stSize);
	bool Write(const String& str);
	bool WriteLine(const String& strLine);
	bool WriteNewLine(void);


	virtual bool Open(LPCTSTR lpctsFileName){
		return Open(String(lpctsFileName));
	}
	//	BOOL Write(LPVOID lpBuff, DWORD dwSize);
	BOOL WriteLine(LPTSTR lptszBuff);
	BOOL Write(LPTSTR lptszBuff);

private:
	bool		m_fAppend;
	bool        m_fTryStdout;	
};
typedef CWriteFile* PCWriteFile;

class CFileMap{
public:

	CFileMap();

	~CFileMap();

	bool Open(const String&	strFileName);

	long GetSize(void);	

	const void* ReadData(void);

	void Release(void);

private:
	HANDLE m_hFile;
	HANDLE m_hMapping;
	FILE* m_pFile;
	void* m_pvData;

};

//Â²Ác¤¬´«
class ChineseCodeConverter{
public:

	bool InitBig5ToGB2312() {return InitData(_T("b2g.dat"));}
	bool InitGB2312ToBig5() {return InitData(_T("g2b.dat"));}

	bool InitData(
		const String&		strDataFileName
		);
	void Convert(
		const string&		strInput,
		string&				strOutput		
		);
private:
	char				m_arCodeTable[65536 * 2];
};
typedef ChineseCodeConverter* PChineseCodeConverter;


class CHTCharSet{
public:
	static bool IsChinesePunctuation(const _TCHAR wch);
	static bool IsChineseSymbol(const _TCHAR wch);
	static bool IsChineseChar(const _TCHAR wch);

	static bool IsEnglishAlphabet(const _TCHAR wch);
	static bool IsEnglishNumeric(const _TCHAR wch);
	static bool IsEnglishSymbol(const _TCHAR wch);

	static bool IsHalfShapeAlphabet(const _TCHAR wch);
	static bool IsHalfShapeNumeric(const _TCHAR wch);
	static bool IsHalfShapeSymbol(const _TCHAR wch);

	static bool IsSymbol(const _TCHAR wch);
	static bool IsSpace(const _TCHAR wch);

};

////////////////////////////////////////////////////////////////////////////////
//  CExtractPatternParser
//  Parse patterns extracted by following form:
//      {token}<{token}
//      {token}<<{token}
//  This class either returns {token} or '<' or '<<'.
////////////////////////////////////////////////////////////////////////////////

class CExtractPatternParser {
public:
	CExtractPatternParser();

	//  Set extract pattern
	void SetExtractPattern(
		const String&      strExtractPattern
		);

	//  Restart to first token
	void Reset(void);

	//  Get next token
	bool GetNextToken(
		String&            strToken
		);

private:
	String                 m_strExtract;
	StrSizeT                m_stPos;
	StrSizeT                m_stStrLen;
};
typedef CExtractPatternParser*          PCExtractPatternParser;



//////////////////////////////////////////////////////////////////////////////////////////
//
//	Map
//	
//
///////////////////////////////////////////////////////////////////////////////////////////

template <class Key, class Value, class Pred = less<Key> >
class Map{
public:
	typedef map<Key, Value, Pred>	KeyValueMap;
	typedef typename KeyValueMap::size_type SizeType;
	typedef typename KeyValueMap::iterator	KeyValueMapItr;
	typedef typename KeyValueMap::const_iterator	ConstKeyValueMapItr;
	typedef typename iterator_traits<KeyValueMapItr>::difference_type	DifferentType;

	typedef Key					KeyType;
	typedef Value				ValueType;
	typedef const Key			ConstKeyType;
	typedef const Value			ConstValueType;
	typedef KeyType*			PKeyType;
	typedef ValueType*			PValueType;
	typedef const KeyType*		ConstPKeyType;
	typedef const ValueType*	ConstPValueType;

	PValueType Lookup(
		ConstKeyType&	sKey
		) const
	{
		ConstKeyValueMapItr itrFind = m_mMap.find(sKey);
		if(m_mMap.end() == itrFind) {return NULL;}
		return const_cast<PValueType>(&(itrFind->second));
	}

	PValueType Add(
		ConstKeyType&	sKey,
		ConstPValueType	pValue = NULL
		) 
	{
		PValueType pRet = NULL;
		KeyValueMapItr itrFind = m_mMap.find(sKey);
		if(m_mMap.end() == itrFind){
			pair<KeyValueMapItr, bool> prInsert = m_mMap.insert(make_pair(sKey, (pValue ? *pValue : Value())));
			ASSERT(prInsert.second);
			pRet = &(prInsert.first->second);
		} else{
			if(pValue) {itrFind->second = *pValue; }
			pRet = &(itrFind->second);			
		} 
		return pRet;
	}

	bool Remove(
		ConstKeyType&		sKey	
		)
	{
		KeyValueMapItr itrFind = m_mMap.find(sKey);
		if(m_mMap.end() == itrFind){
			return false;
		} else{
			m_mMap.erase(itrFind);	
		}
		return true;
	}

	const Key& TopKey(void) const{
		ASSERT(!Empty());
		return m_mMap.begin()->first;
	}

	Value&	TopValue(void){
		ASSERT(!Empty());
		return m_mMap.begin()->second;	
	}

	const Key& KeyByIndex(SizeType stIndex) const{
		ConstKeyValueMapItr itr = m_mMap.begin();
		advance(itr, stIndex);
		return itr->first;
	}

	Value& ValueByIndex(SizeType stIndex) {
		KeyValueMapItr itr = m_mMap.begin();
		advance(itr, stIndex);
		return itr->second;
	}

	bool Next(
		PKeyType		psKey,
		PValueType*		ppsValue		
		) const 
	{
		if(m_itrCurr == End()){
			Rewind();
			return false;
		}
		if(psKey) {*psKey = m_itrCurr->first;}
		if(ppsValue) {*ppsValue = const_cast<PValueType>(&(m_itrCurr->second));}
		++m_itrCurr;
		++m_stCurrIndex;
		return true;
	}

	SizeType GetCurrIndex(void) const { return m_stCurrIndex; }

	void Rewind(void)const
	{
		m_itrCurr = Begin();		
		m_stCurrIndex = 0;
	}

	KeyValueMapItr Begin()
	{
		return m_mMap.begin();	
	}

	ConstKeyValueMapItr Begin() const
	{
		return m_mMap.begin();	
	}

	KeyValueMapItr End()
	{
		return m_mMap.end();		
	}

	ConstKeyValueMapItr End() const
	{
		return m_mMap.end();		
	}

	void Clear(void)
	{		
		m_mMap.clear();
	}

	bool Empty(void) const
	{
		return m_mMap.empty();
	}

	SizeType Size(void) const
	{
		return m_mMap.size();
	}

private:
	KeyValueMap				m_mMap;
	mutable ConstKeyValueMapItr	m_itrCurr;
	mutable SizeType		m_stCurrIndex;
};



///////////////////////////////////////////////////////////////////////////////////////////
//
//	CConfigSetting
//
///////////////////////////////////////////////////////////////////////////////////////////

typedef Map<String, String> CStrStrMap;
typedef CStrStrMap* PCStrStrMap;
typedef const CStrStrMap* ConstPCStrStrMap;
typedef pair<String, CStrStrMap>    StrStrMapPair;
typedef vector<StrStrMapPair>       StrStrMapPairVtr;
typedef StrStrMapPairVtr::iterator  StrStrMapPairVtrItr;
typedef StrStrMapPairVtr::const_iterator ConstStrStrMapPairVtrItr;

typedef Map<int, int> CIntIntMap;

// CableLoss, GPIB
class CRFEquipmentConfig
{
public:
	CIntIntMap			cGPIBAddressMap;
	CIntIntMap          cGPIBPortMap;
	CStrStrMap			cEquipmentTestSettings;
	StrStrMapPairVtr	vRFCableRXMapPair;
	StrStrMapPairVtr    vRFCableTXMapPair;
	StrStrMapPairVtr    vCalGSMCableLossPair;
	StrStrMapPairVtr    vCalWCDMACableLossPair;
};
typedef CRFEquipmentConfig* PCRFEquipmentConfig;

// Mobile Informatoin
class CMobileInformation
{
public:
	void Clear() {
		m_strMobileID = _T("");
		m_strPicassoNum = _T("");
		m_strSWVer = _T("");
		m_strHWVer = _T("");
	}

public:
	CString m_strMobileID;
	CString m_strPicassoNum;
	CString m_strSWVer;
	CString m_strHWVer;
};

class CConfigSetting
{
public:	
	HWND                m_hMessageHandlingWnd;
	UINT                m_nComPORT;
	BOOL				m_bForcePass;
	StrStrMapPairVtr	vProcessSettingMapPair;
	StrStrMapPairVtr	vDeviceSettingMapPair;
	CStrStrMap			cMainRFSettingMap;

	CRFEquipmentConfig	cRFEquipmentConfig;
	
	CMobileInformation   m_MobileInfo;
};
typedef CConfigSetting* PCConfigSetting;

///////////////////////////////////////////////////////////////////////////////////////////
//
//	CRangePair
//
///////////////////////////////////////////////////////////////////////////////////////////

class CRangePair
{
public:
	void SetRange(double dlFirst, double dlSecond){
		if (dlFirst >= dlSecond)
		{
			m_dlMax = dlFirst;
			m_dlMin = dlSecond;
		} else{
			m_dlMax = dlSecond;
			m_dlMin = dlFirst;
		}
	}
	bool SetRange(const String& strLine)
	{
		StrVtr vToken;
		CStr::ParseString(strLine, _T(","), vToken);
		if (vToken.size() !=2)
		{
			return false;	
		}
		SetRange(CStr::StrToFloat(vToken[0]), CStr::StrToFloat(vToken[1]));
		return true;
	}

	bool InRange(double dlValue){
		if (dlValue > m_dlMax || dlValue < m_dlMin)
		{
			return false;
		}
		return true;
	}
	String RangeStr(void) const;

	inline double GetMax(){return m_dlMax;}
	inline double GetMin(){return m_dlMin;}

private:
	double m_dlMax;
	double m_dlMin;
};
typedef map<int, CRangePair> PCLPwrRangeMap;

typedef void (*FPFactoryLog)(INT iProcessCode,LPCTSTR lpszBand,LPCTSTR lpszChannel, INT iErrorCode, LPCTSTR lpszUL, LPCTSTR lpszDL, LPCTSTR lpszMeasured, LPCTSTR lpszUnit,BOOL bPass, LPCTSTR lpszMsgFormat, ...);
typedef void (*FPTraceLog)(LPCTSTR lpszModule, LPCSTR lpszSrcFile, UINT iSrcLine, EMsgLevel msgLevel, LPCTSTR lpszMsgFormat, ...);
typedef void (*FPCommandLog)(LPCTSTR lpszType, LPCTSTR lpszMessage);


///////////////////////////////////////////////////////////////////////////////////////////
//
//	Critical Section
//
///////////////////////////////////////////////////////////////////////////////////////////
//Disabled for XQT project
/*
class CSimpleCriticalSection
{
public:
	void Lock() { EnterCriticalSection(&m_sec); }
	void Unlock() { LeaveCriticalSection(&m_sec); }
	CSimpleCriticalSection() { InitializeCriticalSection(&m_sec); } 
	~CSimpleCriticalSection() { DeleteCriticalSection(&m_sec); }
private:
	CRITICAL_SECTION m_sec;
};

*/




#endif //__COMMON_UTIL_H



