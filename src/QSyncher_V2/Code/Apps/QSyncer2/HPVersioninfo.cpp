//////////////////////////////////////////////////////////////////////
// 
// HPVersionInfo.cpp
// 
// Copyright (c) 2003, Henri Hein
// 
// HPVERsionInfo.cpp: implementation of the CVersionInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HPVERsionInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVersionInfo::CVersionInfo(VS_FIXEDFILEINFO* pver)
{
	SetFromFFI(pver);
}

CVersionInfo::CVersionInfo(const CVersionInfo& ver)
{
	SetFromVerInfo(ver);
}

CVersionInfo::CVersionInfo(LPCTSTR strVer)
{
	Reset();
	SetFromString(strVer);
}

CVersionInfo::CVersionInfo(HMODULE hmod)
{
	SetFromInstance(hmod);
}

CVersionInfo::CVersionInfo()
{
	Reset();
}

CVersionInfo::~CVersionInfo()
{

}

void CVersionInfo::Reset()
{
	m_verMajor = 0;
	m_verMinor = 0;
	m_verAux = 0;
	m_verMaintenance = 0;
	m_strProductName.assign(_T(""));
	m_strCompany.assign(_T(""));
	m_strLegal.assign(_T(""));
}

void CVersionInfo::SetFromVerInfo(const CVersionInfo& ver)
{
	m_verMajor = ver.m_verMajor;
	m_verMinor = ver.m_verMinor;
	m_verAux = ver.m_verAux;
	m_verMaintenance = ver.m_verMaintenance;
	m_strProductName = ver.m_strProductName;
	m_strCompany = ver.m_strCompany;
	m_strLegal = ver.m_strLegal;
}

void CVersionInfo::SetFromFFI(VS_FIXEDFILEINFO* pver)
{
	m_verMajor = HIWORD(pver->dwFileVersionMS);
	m_verMinor = LOWORD(pver->dwFileVersionMS);
	m_verAux = HIWORD(pver->dwFileVersionLS);
	m_verMaintenance = LOWORD(pver->dwFileVersionLS);
}

void CVersionInfo::SetFromString(LPCTSTR strVer)
{
	const char chDelim = '.';
	LPCTSTR pch = strVer;
	HPVER *ppVer[] = { &m_verMajor, 
						&m_verMinor, 
						&m_verAux, 
						&m_verMaintenance, 
						NULL };
	HPVER **ppVerNext = ppVer;

	while (pch && *pch)
	{
#ifdef _UNICODE
		**ppVerNext = (HPVER)_wtoi(pch);
		pch = wcschr(pch, chDelim);
#else
		**ppVerNext = atoi(pch);
		pch = strchr(pch, chDelim);
#endif
		if (pch) 
			pch++;
		ppVerNext++;
		if (!*ppVerNext)
			break;
	}
}

void CVersionInfo::SetFromInstance(HMODULE hmod)
{
	TCHAR szFile[MAX_PATH];

	if (NULL == hmod)
	{
		Reset();
		return;
	}
	GetModuleFileName(hmod, szFile, sizeof(szFile));
	GetVersionOfFile(szFile);
}

void CVersionInfo::SetFromFilename(LPCTSTR tszFile)
{
	GetVersionOfFile(tszFile);
}

void CVersionInfo::SetMajor(HPVER verMajor)
{
	m_verMajor = verMajor;
}

void CVersionInfo::SetMinor(HPVER verMinor)
{
	m_verMinor = verMinor;
}

void CVersionInfo::SetAux(HPVER verAux)
{
	m_verAux = verAux;
}

void CVersionInfo::SetMaintenance(HPVER verMaintenance)
{
	m_verMaintenance = verMaintenance;
}


void CVersionInfo::AsResource(LPTSTR str) const
{
	wsprintf(str, _T("%d,%d,%d,%d"), m_verMajor, 
									 m_verMinor, 
									 m_verAux, 
									 m_verMaintenance);
}

LPTSTR CVersionInfo::AsString(LPTSTR tszStrOut) const
{
	wsprintf(tszStrOut, _T("%d.%d.%d.%d"), m_verMajor, 
											m_verMinor, 
											m_verAux, 
											m_verMaintenance);
	return tszStrOut;
}

LPTSTR CVersionInfo::AsStringShort(LPTSTR tszStrOut) const
{
	wsprintf(tszStrOut, _T("%d.%d"), m_verMajor, m_verMinor);
	return tszStrOut;
}

LPCTSTR CVersionInfo::AsString()
{
	return AsString(m_tszStrOut);
}

LPCTSTR CVersionInfo::AsString(TSTRING& str) const
{
	TCHAR tszVer[CBVERSTR];

	str = AsString(tszVer);
	return str.data();
}

LPCTSTR CVersionInfo::AsStringShort(TSTRING& str) const
{
	TCHAR tszVer[CBVERSTR];

	str = AsStringShort(tszVer);
	return str.data();
}

//Extended info
LPCTSTR CVersionInfo::GetCompanyName() const
{
	return m_strCompany.data();
}

LPCTSTR CVersionInfo::GetExtProductName(TSTRING* pstr) const
{
	TSTRING str;

	pstr->assign(m_strProductName);
	pstr->append(_T(" "));
	pstr->append(AsStringShort(str));
	return pstr->data();
}

LPCTSTR CVersionInfo::GetProductName() const
{
	return m_strProductName.data();
}

LPCTSTR CVersionInfo::GetLegalInfo() const
{
	return m_strLegal.data();
}


//
// Compare methods and Operators
//

//Compare: 1 is >, -1 is <, 0 is ==
UINT CVersionInfo::Compare(const CVersionInfo& ver) const
{
	HPVER rgVerThis[] = { m_verMajor, 
							m_verMinor, 
							m_verAux, 
							m_verMaintenance };
	HPVER rgVer[] = { ver.m_verMajor, 
							ver.m_verMinor, 
							ver.m_verAux, 
							ver.m_verMaintenance };
	int ixRg = 0;

	ATLASSERT(sizeof(rgVerThis) == sizeof(rgVer));
	while (ixRg < (sizeof(rgVerThis)/sizeof(HPVER)))
	{
		if (rgVerThis[ixRg] > rgVer[ixRg])
			return 1;
		if (rgVerThis[ixRg] < rgVer[ixRg])
			return -1;
		ixRg++;
	}
	return 0;
}

// operators methods
CVersionInfo& CVersionInfo::operator=(const CVersionInfo& ver)
{
	SetFromVerInfo(ver);
	return *this;
}

BOOL CVersionInfo::operator==(const CVersionInfo& ver) const
{
	return (0 == Compare(ver));
}

BOOL CVersionInfo::operator<(const CVersionInfo& ver) const
{
	return (-1 == Compare(ver));
}

BOOL CVersionInfo::operator>(const CVersionInfo& ver) const
{
	return (1 == Compare(ver));
}


void GetVersionString(LPTSTR lpVI, 
					  LPTSTR tszStr, 
					  WORD* lng, 
					  TSTRING* pstr)
{
    DWORD dwBufSize; 
	TCHAR tszVerStrName[CBVERSTR];
	LPVOID lpt;

	wsprintf(tszVerStrName, _T("\\StringFileInfo\\%04x%04x\\%s"), 
			lng[0], lng[1], tszStr);
	if (VerQueryValue(lpVI, tszVerStrName, &lpt, (UINT*)&dwBufSize))
		pstr->assign((LPTSTR)lpt);
}

BOOL CVersionInfo::GetVersionOfFile(LPCTSTR szFile) 
{
    DWORD dwBufSize; 
    VS_FIXEDFILEINFO* lpFFI; 
    LPTSTR lpVI = GetVersionInfo(szFile); 
    BOOL bRet = FALSE;
	WORD* langInfo;
	UINT cbLang;

	if (!lpVI)
		return FALSE;
    if (bRet = VerQueryValue(lpVI, _T("\\"), 
								(LPVOID *) &lpFFI, 
								(UINT *) &dwBufSize))
	{
		SetFromFFI(lpFFI);
	}
	VerQueryValue(lpVI, 
              _T("\\VarFileInfo\\Translation"),
              (LPVOID*)&langInfo,
              &cbLang);
	GetVersionString(lpVI, 
						_T("ProductName"), 
						langInfo, 
						&m_strProductName);
	GetVersionString(lpVI, 
						_T("CompanyName"), 
						langInfo, 
						&m_strCompany);
	GetVersionString(lpVI, 
						_T("LegalCopyright"), 
						langInfo, 
						&m_strLegal);
	
    GlobalFree((HGLOBAL)lpVI); 

    return bRet; 
} 

LPTSTR CVersionInfo::GetVersionInfo(LPCTSTR szFile) 
{
    DWORD dwLen, dwUseless; 
    LPTSTR lpVI; 

    dwLen = GetFileVersionInfoSize((LPTSTR)szFile, &dwUseless); 
    if (dwLen==0) 
        return 0; 

    lpVI = (LPTSTR) GlobalAlloc(GPTR, dwLen); 
	if (lpVI)
	{
	    GetFileVersionInfo((LPTSTR)szFile, NULL, dwLen, lpVI); 
		return lpVI;
	}	
    return NULL; 
}

