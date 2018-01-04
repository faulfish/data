#include "stdafx.h"

#include "PIMStringUtility.h"
#include <string>
#include <list>
#include <vector>
#include <atlenc.h>
#include <cstringt.h>


namespace QSYNC_LIB 
{
	using namespace std;

bool StrToTime(const CString& strTime, COleDateTime& oleTime)  //strTime = yyyymmddThhmmssZ	
{
	bool bRet = false;
	int nYear, nMonth, nDay, nHour, nMinute, nSecond =0;
	if(16 == strTime.GetLength() || 15 == strTime.GetLength())
	{
		nYear = _ttoi(strTime.Mid(0,4));
		nMonth = _ttoi(strTime.Mid(4,2));
		nDay = _ttoi(strTime.Mid(6,2));
		nHour = _ttoi(strTime.Mid(9,2));
		nMinute = _ttoi(strTime.Mid(11,2));
		nSecond = _ttoi(strTime.Mid(13,2));
		if(0 == oleTime.SetDateTime(nYear, nMonth, nDay, nHour, nMinute, nSecond))
			bRet = true;
	}
	else if(0 == strTime.GetLength() )
	{	
		oleTime = COleDateTime::GetCurrentTime();
		bRet = true;
	}
	return bRet;
}


bool ConvertTime(const COleDateTime& DTOrg, COleDateTime& DTDes, bool bUTCtoLocal)
{
	bool bRes = true;
	//[Note]: Windows NT/2000以上可以直接用"SystemTimeToTzSpecificLocalTime()" API.
	///CTime tUTCTimeTemp(in_oledtUTCTime.GetYear(), in_oledtUTCTime.GetMonth(), in_oledtUTCTime.GetDay(), in_oledtUTCTime.GetHour(), in_oledtUTCTime.GetMinute(), in_oledtUTCTime.GetSecond(), -1);

	SYSTEMTIME stOrg, stDes;
	stOrg.wYear = DTOrg.GetYear();
	stOrg.wMonth = DTOrg.GetMonth();
	stOrg.wDay = DTOrg.GetDay();
	stOrg.wHour = DTOrg.GetHour();
	stOrg.wMinute = DTOrg.GetMinute();
	stOrg.wSecond = DTOrg.GetSecond();
	stOrg.wMilliseconds = 0;

	if(bUTCtoLocal)
		SystemTimeToTzSpecificLocalTime(NULL, &stOrg, &stDes);
	else // local to UTC
	{
		//TzSpecificLocalTimeToSystemTime(NULL, &stOrg, &stDes);
		FILETIME ft, ft_utc;

		if (!(SystemTimeToFileTime(&stOrg, &ft) && 
			LocalFileTimeToFileTime(&ft, &ft_utc) &&
			FileTimeToSystemTime(&ft_utc,&stDes)))
		{
			bRes = false;
		}
	}
	if(bRes)
	{
		int nYear, nMonth, nDay, nHour, nMin, nSec = 0;
		nYear	 = stDes.wYear;
		nMonth	 = stDes.wMonth;
		nDay	 = stDes.wDay;
		nHour	 = stDes.wHour;
		nMin	 = stDes.wMinute;
		nSec	 = stDes.wSecond;

		DTDes.SetDateTime(nYear, nMonth, nDay, nHour, nMin, nSec);
		bRes = (DTDes.GetStatus()!=COleDateTime::null)&&(DTDes.GetStatus()!=COleDateTime::invalid);
	}
	return bRes;
}
int MinusTime(const CString& strFirst, const CString& strSecond)
{
	int nInterval = 0;
	if(strSecond != _T("") && strFirst != _T(""))
	{
		COleDateTime oleFirst, oleSecond;
		StrToTime(strFirst, oleFirst);
		StrToTime(strSecond, oleSecond);
		COleDateTimeSpan spanTime = oleFirst - oleSecond;
		double dAlarmInterval = spanTime.GetTotalMinutes();
		if(dAlarmInterval==1)
			nInterval = 1;
		else if(dAlarmInterval==5)
			nInterval = 2;
		else if(dAlarmInterval==10)
			nInterval = 3;
		else if(dAlarmInterval==15)
			nInterval = 4;
		else if(dAlarmInterval==30)
			nInterval = 5;
		else if(dAlarmInterval==60)
			nInterval = 6;
		//else if(dAlarmInterval < 1440 && dAlarmInterval >60)
		//	nInterval = 6;
		else if(dAlarmInterval==1440)
			nInterval = 7;
		else // Default
			nInterval = 8;
	}
	return nInterval; 
}
} //namespace QSYNC_LIB 