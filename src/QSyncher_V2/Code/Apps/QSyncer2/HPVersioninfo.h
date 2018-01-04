//////////////////////////////////////////////////////////////////////
// 
// HPVersionInfo.h
// 
// Copyright (c) 2003, Henri Hein
// 
// HPVERsionInfo.h: interface for the CVersionInfo class.
//
// Wrapper for a VS_FIXEDFILEINFO structure.  Can parse a 
// string (such as "1.0.2"), or format the version info 
// into a string.  Default is using '.' as the separator, 
// but the class also supports ',' as separator to be able
// to read/write to/from resource files.  Performs inter-object
// compares, which weighs the version numbers -- for instance,
// "1.0.99" is less than both "2.0.1" and "1.1.0".  
// If instantiated through an hinstance or hmodule, also stores
// extra version information, such as product and company name.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HPVERSIONINFO_H__66D1395F_A084_4608_84C6_4851BEEE382C__INCLUDED_)
#define AFX_HPVERSIONINFO_H__66D1395F_A084_4608_84C6_4851BEEE382C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef unsigned short HPVER;

#define CBVERSTR 64

#include <string>

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif

class CVersionInfo  
{
public:
	CVersionInfo(const CVersionInfo& ver);
	CVersionInfo(VS_FIXEDFILEINFO*);
	CVersionInfo(LPCTSTR strVer);	//Parse strVer as version
	CVersionInfo(HMODULE);			//Read from module
	CVersionInfo();
	virtual ~CVersionInfo();

	//Set methods
	void SetFromString(LPCTSTR);
	void SetFromFFI(VS_FIXEDFILEINFO*);
	void SetFromVerInfo(const CVersionInfo&);
	void SetMajor(HPVER verMajor);
	void SetMinor(HPVER verMinor);
	void SetAux(HPVER verAux);
	void SetMaintenance(HPVER verMaintenance);
	void SetFromInstance(HMODULE);
	void SetFromFilename(LPCTSTR);

	//Out methods
	void AsResource(LPTSTR) const;	//Use ',' as sep
	LPCTSTR AsString(TSTRING& str) const;
	// The "Short" methods only returns major and minor nums.
	LPCTSTR AsStringShort(TSTRING& str) const;
	LPTSTR AsStringShort(LPTSTR) const;
	LPTSTR AsString(LPTSTR) const;
	//Use internal buffer to format string
	LPCTSTR AsString();

	//For extended info
	LPCTSTR GetExtProductName(TSTRING*) const;
	LPCTSTR GetProductName() const;
	LPCTSTR GetCompanyName() const;
	LPCTSTR GetLegalInfo() const;

	//Compare method(s)
	UINT Compare(const CVersionInfo& ver) const;

	// operators methods
	CVersionInfo& operator=(const CVersionInfo& ver);
	BOOL operator==(const CVersionInfo& ver) const;
	BOOL operator<(const CVersionInfo& ver) const;
	BOOL operator>(const CVersionInfo& ver) const;

	//Static helpers
	static LPTSTR GetVersionInfo(LPCTSTR szFile);

protected:
	BOOL GetVersionOfFile(LPCTSTR szFile);
	void Reset();

private:
	TCHAR m_tszStrOut[128];

	HPVER m_verMaintenance;
	HPVER m_verAux;
	HPVER m_verMinor;
	HPVER m_verMajor;

	TSTRING m_strProductName;
	TSTRING m_strCompany;
	TSTRING m_strLegal;
};

#endif // !defined(AFX_HPVERSIONINFO_H__66D1395F_A084_4608_84C6_4851BEEE382C__INCLUDED_)
