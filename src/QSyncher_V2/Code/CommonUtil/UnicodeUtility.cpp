#include "stdafx.h"
#include "unicodeutility.h"
#include <string>
#include <list>
#include <vector>
#include <atlenc.h>
#include <cstringt.h>
#include "PIMStringUtility.h"



namespace QSYNC_LIB 
{

	using namespace std;

	HRESULT __fastcall AnsiToUnicode_internal(LPCSTR pszA, LPOLESTR* ppszW) 
	{
		ULONG cCharacters;
		DWORD dwError;

		// If input is null then just return the same.
		if (NULL == pszA)
		{
			*ppszW = NULL;
			return NOERROR;
		}

		// Determine number of wide characters to be allocated for the
		// Unicode string.
		cCharacters =  static_cast<ULONG>(::strlen(pszA)+1);

		// Use of the OLE allocator is required if the resultant Unicode
		// string will be passed to another COM component and if that
		// component will free it. Otherwise you can use your own allocator.
		//*ppszW = (LPOLESTR) CoTaskMemAlloc(cCharacters*2);
		*ppszW = (LPOLESTR) new char[cCharacters*2];

		if (NULL == *ppszW)
			return E_OUTOFMEMORY;

		// Covert to Unicode.
		if (0 == MultiByteToWideChar(CP_ACP, 0, pszA, cCharacters,
			*ppszW, cCharacters))
		{
			dwError = GetLastError();
			//CoTaskMemFree(*ppszW);
			delete *ppszW;
			*ppszW = NULL;
			return HRESULT_FROM_WIN32(dwError);
		}

		return NOERROR;

	} 

	void AnsiToUnicodeBuffer(const char *pStr,wchar_t *pDestBuffer,int bufferSize)
	{
		bufferSize/=sizeof(wchar_t);
		bufferSize = bufferSize+1;

		LPOLESTR ppszW=NULL;

		AnsiToUnicode_internal(pStr,&ppszW);

		if( bufferSize > 0 )
			::wcsncpy_s(pDestBuffer,bufferSize,ppszW,::strlen(pStr));

		delete ppszW;
	}

	HRESULT __fastcall UnicodeToAnsi_internal(LPCOLESTR pszW, LPSTR* ppszA) 
	{ 
		HRESULT hr = NOERROR;    

		// If input is null then just return the same.
		if (pszW == NULL)
		{
			*ppszA = NULL;
		}
		else
		{
			ULONG cbAnsi, cCharacters;
			DWORD dwError;

			cCharacters = static_cast<ULONG>(wcslen(pszW))+1;
			// Determine number of bytes to be allocated for ANSI string. An
			// ANSI string can have at most 2 bytes per character (for Double
			// Byte Character Strings.)
			cbAnsi = cCharacters*2;

			// Use of the OLE allocator is not required because the resultant
			// ANSI  string will never be passed to another COM component. You
			// can use your own allocator.
			//*ppszA = (LPSTR) CoTaskMemAlloc(cbAnsi);
			*ppszA = new char[cbAnsi];
			if (NULL == *ppszA)
			{
				delete *ppszA;
				hr = E_OUTOFMEMORY;
			}
			else if (0 == WideCharToMultiByte(CP_ACP, 0, pszW, cCharacters, *ppszA,cbAnsi, NULL, NULL))
			{
				dwError = GetLastError();
				//CoTaskMemFree(*ppszA);
				delete *ppszA;
				*ppszA = NULL;
				hr = HRESULT_FROM_WIN32(dwError);
			}
		}

		return hr;

	} 

	void UnicodeToAnsiBuffer(const wchar_t *pStr,char *pDestBuffer,int bufferSize)
	{
		LPCOLESTR pszW=pStr;
		LPSTR ppszA;

		UnicodeToAnsi_internal(pszW,&ppszA);

		if( bufferSize > 0 ) 
		{
			bufferSize--;
			::strncpy_s(pDestBuffer,bufferSize,ppszA,strlen(ppszA));
			pDestBuffer[bufferSize]=0;
		}

		delete [] ppszA;
	}

	DWORD HEX2Ineger(LPCTSTR szHEX)
	{
		LPSTR ppszA = NULL;

#ifdef _UNICODE
		std::wstring strNewHex;
		UnicodeToAnsi_internal(szHEX,&ppszA);
#else
		std::string strNewHex;
		ppszA = szHEX;
#endif
		int nLen = 0;
		for(;AtlGetHexValue(ppszA[nLen]) != -1;nLen++)
			strNewHex += szHEX[nLen];
		if(strNewHex.size()%2 != 0)
			strNewHex = _T('0') + strNewHex;

		BYTE byteBuffer[50] = {0};
		int nBufferLen = 50;
#ifdef _UNICODE
		char szBuffer[50] = {0};
		UnicodeToAnsiBuffer(strNewHex.c_str(),szBuffer,50);
		::AtlHexDecode(szBuffer,static_cast<int>(strNewHex.size()),byteBuffer,&nBufferLen);
#else
		::AtlHexDecode(strNewHex.c_str(),strNewHex.size(),byteBuffer,&nBufferLen);
#endif
		DWORD dwValue = 0;
		DWORD dwDegree = 1;
		for(int i=nBufferLen;i>0;i--)
		{
			dwValue += byteBuffer[i-1]*(dwDegree);
			dwDegree *= 256;
		}

#ifdef _UNICODE
		delete [] ppszA;
#endif
		return dwValue;
	}

	CStringA ReadFileStringA(CArchive& arFile)
	{
		CStringA strTemp;
		char ch;
		while(arFile.Read(&ch,1) == 1)
			strTemp += ch;
		return strTemp;
	}

	CStringA UnicodetoASCII(const CString& strTemp)
	{
		CStringA strAnsi;
#ifdef _UNICODE
		int nBufferSize = (strTemp.GetLength()+1)*sizeof(TCHAR);
		UnicodeToAnsiBuffer(strTemp,strAnsi.GetBuffer(nBufferSize),nBufferSize);
		strAnsi.ReleaseBuffer(nBufferSize);
#else
		strTemp = strTemp;
#endif
		return strAnsi;
	}

	CString	ASCIItoUnicode(const CStringA& strAnsi)
	{
		CString strTemp;
#ifdef _UNICODE
		int nBufferSize = (strAnsi.GetLength()+1)*sizeof(TCHAR);
		AnsiToUnicodeBuffer(strAnsi,strTemp.GetBuffer(nBufferSize),nBufferSize);
		strTemp.ReleaseBuffer(nBufferSize);
#else
		strTemp = strAnsi;
#endif
		return strTemp;
	}

	void WriteStringAtoFile(CArchive& arFile, const CStringA& strAnsi)
	{
		arFile.Write(strAnsi,strAnsi.GetLength());
	}

	CString	ASCIItoUnicode(const char* szAnsi)
	{
		CStringA strAnsi = szAnsi;
		return ASCIItoUnicode(strAnsi);
	}

	void WriteAppendStringAtoFile(CArchive& arFile, const CStringA& strAnsi)
	{
		arFile.Write(strAnsi,strAnsi.GetLength());
	}

	CString UTF8ToUCS2(const std::string& strUTF8)
	{
		CString strResult;

		const CStringA straUTF8 = strUTF8.c_str();
		strResult = CharToWideChar(straUTF8,CP_UTF8);
		return strResult;
	}

	CStringA WideCharToChar(const CString& strSrc, UINT CodePage)
	{
		CStringA strDes;
#ifdef _UNICODE
		strDes = widetoansi((LPCTSTR)strSrc,CodePage).c_str();
#else
		strDes = strSrc;
#endif
		return strDes;
	}

	string widetoansi(const wstring& strSrc, UINT CodePage)
	{
		string strDes;

		int nBufferSize = (static_cast<int>(strSrc.size())+1)*3;
		LPSTR cpDes = new char[nBufferSize];
		if(cpDes)
		{
			::ZeroMemory(cpDes, nBufferSize);
			::WideCharToMultiByte(CodePage, 0, strSrc.c_str(), static_cast<int>(strSrc.size()), cpDes, nBufferSize, NULL, NULL);
			strDes = cpDes;
			delete [] cpDes;
		}

		return strDes;
	}

	wstring ansitowide(const string& strAnsi, UINT CodePage)
	{
		wstring strDes;

		int nBufferSize = (static_cast<int>(strAnsi.size())+1)*3;
		LPWSTR wcpDes = new wchar_t[nBufferSize];
		if(wcpDes)
		{
			::ZeroMemory(wcpDes, nBufferSize);
			::MultiByteToWideChar(CodePage, 0, strAnsi.c_str(), static_cast<int>(strAnsi.size()), wcpDes, nBufferSize);
			strDes = wcpDes;
			delete [] wcpDes;
		}

		return strDes;
	}

	TSTRING GetTSTRING(const std::wstring& strUnicode, UINT CodePage)
	{
		TSTRING tstrRes;
#ifdef _UNICODE
		tstrRes = strUnicode;
#else
		tstrRes = widetoansi(strUnicode, CodePage);
#endif
		return tstrRes;
	}
	TSTRING GetTSTRING(const std::string& strAnsi, UINT CodePage)
	{
		TSTRING tstrRes;
#ifdef _UNICODE
		tstrRes = ansitowide(strAnsi, CodePage);
#else
		tstrRes = strAnsi;
#endif
		return tstrRes;
	}
	std::string GetAnsi(const TSTRING& tstring, UINT CodePage)
	{
		std::string strRes;
#ifdef _UNICODE
		strRes = widetoansi(tstring, CodePage);
#else
		strRes = tstring;
#endif
		return strRes;
	}
	std::wstring GetUnicode(const TSTRING& tstring, UINT CodePage)
	{
		std::wstring strRes;
#ifdef _UNICODE
		strRes = tstring;
#else
		strRes = ansitowide(tstring, CodePage);
#endif
		return strRes;
	}

	CString CharToWideChar(const CStringA& strSrc, UINT CodePage)
	{
		if(CodePage == 0)
			CodePage = CP_ACP;
		CString strDes;
#ifdef _UNICODE
		int nBufferSize = (strSrc.GetLength()+1)*3;
		LPWSTR wcpDes = new wchar_t[nBufferSize];
		if(wcpDes){
			::ZeroMemory(wcpDes, nBufferSize);
			MultiByteToWideChar(CodePage, 0, (LPCSTR)strSrc, strSrc.GetLength(), wcpDes, nBufferSize);
			strDes = wcpDes;
			delete [] wcpDes;
		}
#else
		strDes = strSrc;
#endif
		return strDes;
	}

	bool QPEncoder(const TSTRING& strOrg, TSTRING& strDes, UINT CodePage) 
	{	
		bool bSuccess = true;
		CStringA strDesA; 
		std::string strOrgA = widetoansi(strOrg,CodePage);

		int nQDecLen = QPEncodeGetRequiredLength( (int)strOrgA.size()+2 );
		bSuccess = QPEncode((BYTE*)strOrgA.c_str(), (int)strOrgA.size(), (LPSTR)strDesA.GetBuffer(nQDecLen), &nQDecLen) == TRUE;
		strDesA.ReleaseBuffer(nQDecLen);
	
		//replace ";" to "=3B"
		strDesA.Replace(";", "=3B");
		strDes = (TSTRING)CharToWideChar(strDesA, CodePage);
		
		TSTRING strReplace = strDes;
		vector<TSTRING> vstr;
		Tokenize<TSTRING>(_T("=\r\n"), strReplace, vstr, true);
		if((*(vstr.rbegin())).empty())
			vstr.pop_back();
		strDes = StrConnect<TSTRING>(_T("=\r\n"),vstr, true);

		return bSuccess;
	}
	bool QPDecoder(const TSTRING& strOrg, TSTRING& strDes, UINT CodePage) 
	{
		bool bSuccess = true;
		CStringA strDesA;
		std::string strOrgA = widetoansi(strOrg, CodePage);

		int nQDecLen = QPDecodeGetRequiredLength( (int)strOrgA.size()+2 );
		bSuccess = QPDecode((BYTE*)strOrgA.c_str(), (int)strOrgA.size(), (LPSTR)strDesA.GetBuffer(nQDecLen), &nQDecLen) == TRUE;
		strDesA.ReleaseBuffer(nQDecLen);

		strDes = (TSTRING)CharToWideChar(strDesA, CodePage);
		return bSuccess;
	}
	bool Base64Encoder(const TSTRING& strOrg, TSTRING& strDes, UINT CodePage)
	{
		bool bSuccess = true;
		CStringA strOrgA, strDesA; 
		strOrgA = WideCharToChar(strOrg.c_str(), CodePage);

		int nBDecLen = Base64EncodeGetRequiredLength( strOrgA.GetLength()+2 );
		bSuccess = Base64Encode((BYTE*)(LPCSTR)strOrgA, strOrgA.GetLength(), (LPSTR)strDesA.GetBuffer(nBDecLen), &nBDecLen) == TRUE;
		strDesA.ReleaseBuffer(nBDecLen);

		strDes = (TSTRING)CharToWideChar(strDesA, CodePage);
		return bSuccess;
	}
	bool Base64Decoder(const TSTRING& strOrg, TSTRING& strDes, UINT CodePage)
	{
		bool bSuccess = true;
		CStringA strDesA; 
		std::string strOrgA = widetoansi(strOrg);

		int nBDecLen = Base64DecodeGetRequiredLength( (int)strOrgA.size()+2 );
		bSuccess = Base64Decode(strOrgA.c_str(), (int)strOrgA.size(), (BYTE*)strDesA.GetBuffer(nBDecLen), &nBDecLen) == TRUE;
		strDesA.ReleaseBuffer(nBDecLen);

		strDes = (TSTRING)CharToWideChar(strDesA, CodePage);
		return bSuccess;
	}

} //namespace QSYNC_LIB 