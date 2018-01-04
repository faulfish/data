#ifndef _CalFunctionTime_H_
#define _CalFunctionTime_H_

class CalFunctionTime
{
public:
	CalFunctionTime(const CString& strFuncName = _T("")):m_strFuncName(strFuncName),m_tcStart(GetTickCount())
	{
		TRACE(_T("FUNCTION(%s) CALCLATE START\n"),m_strFuncName);
	}
	~CalFunctionTime()
	{		
		DWORD tcEnd = GetTickCount();
		TRACE(_T("FUNCTION(%s) CALCLATE END----%d clocks\n"),m_strFuncName,tcEnd - m_tcStart);
	}
private:
	DWORD m_tcStart;
	CString m_strFuncName;
};

#endif //_CalFunctionTime_H_