#include "stdafx.h"
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

CString GetCurrentActivePort();