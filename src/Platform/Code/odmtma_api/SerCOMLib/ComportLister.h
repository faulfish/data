// ComportLister.h: interface for the CComportLister class.
//
//////////////////////////////////////////////////////////////////////

#ifndef COMPORTLISTER_H
#define COMPORTLISTER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class CComportLister  
{
public:
	CComportLister();
	virtual ~CComportLister();

	const char* GetComportList();
private:
	std::string m_strComportList;
};

#endif // COMPORTLISTER_H
