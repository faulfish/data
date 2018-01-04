#ifndef __TYPE_H__
#define __TYPE_H__

#include <tchar.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include <fstream>
#include <complex>

// include standard template library
#include <vector>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <deque>
#include <algorithm>
#include <string>
using namespace std;


#define N_MAX_BUFFER_LEN				256

#if defined(_WIN32) || defined(WIN32)
#define STDCALLCONV STDMETHODCALLTYPE
#else	//!WIN32
#define STDCALLCONV
#endif //defined(_WIN32) || defined(WIN32)

typedef double DOUBLE;

#ifdef _MSC_VER

	#include <windows.h>
//	#define STRSAFE_NO_DEPRECATE
//	#include <strsafe.h>
	#include <commctrl.h>

#else
	typedef unsigned short				wchar_t;
	typedef basic_string<wchar_t>		wstring;
	#ifdef __cplusplus
	#define EXTERN_C extern "C"
	#else	
	#define EXTERN_C extern 
	#endif //__cplusplus

	#define UNALIGNED

// Primitive type definition
	#define far
	#define near

//Copied from windef.h line 148-174
#ifndef CONST
	#define CONST               const
#endif //CONST

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef BOOL near           *PBOOL;
typedef BOOL far            *LPBOOL;
typedef BYTE near           *PBYTE;
typedef BYTE far            *LPBYTE;
typedef int near            *PINT;
typedef int far             *LPINT;
typedef WORD near           *PWORD;
typedef WORD far            *LPWORD;
typedef long far            *LPLONG;
typedef DWORD near          *PDWORD;
typedef DWORD far           *LPDWORD;
typedef void far            *LPVOID;
typedef CONST void far      *LPCVOID;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;

//Copied from winnt.h line 270-395

//
// Basics
//

#ifndef VOID
	#define VOID void
	typedef char CHAR;
	typedef short SHORT;
	typedef long LONG;
#if !defined(MIDL_PASS)
typedef int INT;
#endif //!defined(MIDL_PASS)
#endif //VOID

//
// UNICODE (Wide Character) types
//

#ifndef _MAC
	typedef wchar_t WCHAR;    // wc,   16-bit UNICODE character
#else
// some Macintosh compilers don't define wchar_t in a convenient location, or define it as a char
	typedef unsigned short WCHAR;    // wc,   16-bit UNICODE character
#endif //_MAC

typedef WCHAR *PWCHAR, *LPWCH, *PWCH;
typedef CONST WCHAR *LPCWCH, *PCWCH;
typedef __nullterminated WCHAR *NWPSTR, *LPWSTR, *PWSTR;
typedef __nullterminated PWSTR *PZPWSTR;
typedef __nullterminated CONST PWSTR *PCZPWSTR;
typedef __nullterminated WCHAR UNALIGNED *LPUWSTR, *PUWSTR;
typedef __nullterminated CONST WCHAR *LPCWSTR, *PCWSTR;
typedef __nullterminated PCWSTR *PZPCWSTR;
typedef __nullterminated CONST WCHAR UNALIGNED *LPCUWSTR, *PCUWSTR;


#if _WIN32_WINNT >= 0x0600 || (defined(__cplusplus) && defined(WINDOWS_ENABLE_CPLUSPLUS))

typedef CONST WCHAR *LPCWCHAR, *PCWCHAR;
typedef CONST WCHAR UNALIGNED *LPCUWCHAR, *PCUWCHAR;

//
//  UCS (Universal Character Set) types
//

typedef unsigned long UCSCHAR;

//
//  Even pre-Unicode agreement, UCS values are always in the
//  range U+00000000 to U+7FFFFFFF, so we'll pick an obvious
//  value.

#define UCSCHAR_INVALID_CHARACTER (0xffffffff)

#define MIN_UCSCHAR (0)

//
//  We'll assume here that the ISO-10646 / Unicode agreement
//  not to assign code points after U+0010FFFF holds so that
//  we do not have to have separate "UCSCHAR" and "UNICODECHAR"
//  types.
//

#define MAX_UCSCHAR (0x0010FFFF)

typedef UCSCHAR *PUCSCHAR;
typedef const UCSCHAR *PCUCSCHAR;

typedef UCSCHAR *PUCSSTR;
typedef UCSCHAR UNALIGNED *PUUCSSTR;

typedef const UCSCHAR *PCUCSSTR;
typedef const UCSCHAR UNALIGNED *PCUUCSSTR;

typedef UCSCHAR UNALIGNED *PUUCSCHAR;
typedef const UCSCHAR UNALIGNED *PCUUCSCHAR;

#endif // _WIN32_WINNT >= 0x0600


//
// ANSI (Multi-byte Character) types
//
typedef CHAR *PCHAR, *LPCH, *PCH;
typedef CONST CHAR *LPCCH, *PCCH;

typedef __nullterminated CHAR *NPSTR, *LPSTR, *PSTR;
typedef __nullterminated PSTR *PZPSTR;
typedef __nullterminated CONST PSTR *PCZPSTR;
typedef __nullterminated CONST CHAR *LPCSTR, *PCSTR;
typedef __nullterminated PCSTR *PZPCSTR;

//
// Neutral ANSI/UNICODE types and macros
//
#ifdef  UNICODE                     // r_winnt

#ifndef _TCHAR_DEFINED
typedef WCHAR TCHAR, *PTCHAR;
typedef WCHAR TBYTE , *PTBYTE ;
#define _TCHAR_DEFINED
#endif /* !_TCHAR_DEFINED */

typedef LPWCH LPTCH, PTCH;
typedef LPWSTR PTSTR, LPTSTR;
typedef LPCWSTR PCTSTR, LPCTSTR;
typedef LPUWSTR PUTSTR, LPUTSTR;
typedef LPCUWSTR PCUTSTR, LPCUTSTR;
typedef LPWSTR LP;
#define __TEXT(quote) L##quote      // r_winnt

#else   /* UNICODE */               // r_winnt

#ifndef _TCHAR_DEFINED
typedef char TCHAR, *PTCHAR;
typedef unsigned char TBYTE , *PTBYTE ;
#define _TCHAR_DEFINED
#endif /* !_TCHAR_DEFINED */

typedef LPCH LPTCH, PTCH;
typedef LPSTR PTSTR, LPTSTR, PUTSTR, LPUTSTR;
typedef LPCSTR PCTSTR, LPCTSTR, PCUTSTR, LPCUTSTR;
#define __TEXT(quote) quote         // r_winnt

#endif /* UNICODE */                // r_winnt
#define TEXT(quote) __TEXT(quote)   // r_winnt

#endif //__MSC_VER

typedef size_t SizeT;

#ifdef UNICODE
typedef wstring String;
typedef wofstream Tofstream;
typedef wifstream Tifstream;
#else
typedef string							String;
typedef ofstream						Tofstream;
typedef ifstream						Tifstream;
#endif //UNICODE

typedef String::iterator StrItr;
typedef String::const_iterator ConstStrItr;
typedef String::size_type StrSizeT;
typedef String*							PString;
typedef const String*					PCString;

typedef list<String> StrList;
typedef StrList::iterator StrListItr;
typedef StrList::const_iterator ConstStrListItr;
typedef StrList*						PStrList;

typedef vector<size_t> SizeVtr;
typedef SizeVtr::iterator SizeVtrItr;

typedef vector<String> StrVtr;
typedef StrVtr::iterator StrVtrItr;
typedef StrVtr::const_iterator ConstStrVtrItr;
typedef StrVtr*							PStrVtr;

typedef set<String> StrSet;
typedef StrSet::iterator StrSetItr;
typedef StrSet::const_iterator ConstStrSet;
typedef StrSet*							PStrSet;

typedef vector<StrVtr> StrStrVtr;
typedef StrStrVtr::iterator StrStrVtrItr;
typedef StrStrVtr::const_iterator ConstStrStrVtrItr;

typedef vector<StrStrVtr> StrStrStrVtr;
typedef StrStrStrVtr::iterator StrStrStrVtrItr;
typedef StrStrStrVtr::const_iterator ConstStrStrStrVtr;

typedef vector<INT> IntVtr;
typedef IntVtr::iterator IntVtrItr;

typedef vector<DOUBLE> DoubleVtr;
typedef DoubleVtr::iterator DoubleVtrItr;

typedef map<String, String> StrStrMap;
typedef StrStrMap::iterator StrStrMapItr;
typedef StrStrMap::const_iterator ConstStrStrMapItr;

typedef map<String, DWORD> StrCountMap;
typedef StrCountMap::iterator StrCountMapItr;
typedef StrCountMap::const_iterator ConstCountStrMapItr;
typedef StrCountMap*					PStrCountMap;


typedef float MYDOUBLE;
typedef unsigned char BYTE;

inline bool NPOS(StrSizeT s) {return (String::npos == s);}

#endif//__TYPE_H__