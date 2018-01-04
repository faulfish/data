#pragma once

#ifdef REPORTCENTER_EXPORTS
#define REPORTCENTER_API extern "C" __declspec(dllexport)
#else
#define REPORTCENTER_API extern "C" __declspec(dllimport)
#endif

/*
Enable or disable log by parameter

Parameter 
bEnable : bool
Enable or disable log

szClass : const char*
Specific the class

nLevel : int
Specific the priority
By default
if nLevel >= 5 , it will hide in release mode and show in debug mode
if nLevel < 5 , it will show in release and debug mode
By user setting
if nLevel >= nUserLevel , it will hide 
*/

namespace IReportCenter
{
	REPORTCENTER_API bool EnableLog(bool bEnable,int nUserLevel = -1);
	REPORTCENTER_API bool Log(const char* szClass,int nErrorCode,int nMinorErrorCode = -1,int nLevel = 5,const char* strMsg = NULL);
	REPORTCENTER_API bool Log_ForSPU(const char* szClass,const char* szErrorCode,const char* szMinorErrorcode = "-1",const char* szLevel = "5",const char* strMsg = NULL);
}
