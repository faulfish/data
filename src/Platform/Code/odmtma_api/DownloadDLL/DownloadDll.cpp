#include "stdafx.h"
#include "QDOperator.h"
#include "ComportLister.h"
#include "DownloadDll.h"

extern "C" __declspec (dllexport) HANDLE _stdcall StartLog()
{
	return 0;
}

extern "C" __declspec (dllexport) HANDLE _stdcall EndLog()
{
	return 0;
}

extern "C" __declspec (dllexport) HANDLE _stdcall SetComPort(char* szComPort)
{
	CQDOperator* pQDoperator  = new CQDOperator;
	pQDoperator->SetCOMPort(szComPort);
	return (HANDLE)pQDoperator;

}

extern "C" __declspec (dllexport) int _stdcall SetHexFile(HANDLE hResource, char* szHexFile)
{
	ASSERT(hResource != NULL);
	CQDOperator* pQDoperator = (CQDOperator*)hResource;
	return pQDoperator->SetHex(szHexFile);
}

extern "C" __declspec (dllexport) int _stdcall SetMbnFile(HANDLE hResource, char* szPart, char* szOtp, char* szPbl, char* szQcsblHd, char* szQcsbl, char* szOemsblHd, char* szOemsbl, char* szAmssHd, char* szAmss, char* szCefs)
{
	ASSERT(hResource != NULL);
	CQDOperator* pQDoperator = (CQDOperator*)hResource;
	pQDoperator->SetPartition(szPart);
	pQDoperator->SetPbl(szPbl);
	pQDoperator->SetQcsblhd(szQcsblHd);
	pQDoperator->SetQcsbl(szQcsbl);
	pQDoperator->SetOemsblhd(szOemsblHd);
	pQDoperator->SetOemsbl(szOemsbl);
	pQDoperator->SetAMSShd(szAmssHd);
	pQDoperator->SetAMSS(szAmss);
	pQDoperator->SetCEFS(szCefs);
	return 0;
}

extern "C" __declspec (dllexport) int _stdcall SetSkipCheck(HANDLE hResource)
{
	ASSERT(hResource != NULL);
	CQDOperator* pQDoperator  = (CQDOperator*)hResource;
	pQDoperator->SetSkipChecking();
	return 0;
}

extern "C" __declspec (dllexport) int _stdcall StartDownload(HANDLE hResource)
{
	ASSERT(hResource != NULL);
	CQDOperator* pQDoperator = (CQDOperator*)hResource;
	return pQDoperator->DownloadFunc();
	
}


extern "C" __declspec (dllexport) int _stdcall SetCallBack(HANDLE hResource, int(*CallBack)(const char*, int))
{
	ASSERT(hResource != NULL);
	CQDOperator* pQDoperator = (CQDOperator*)hResource;
	pQDoperator->SetCallBackFunc(CallBack);
	return 0;
}

extern "C" __declspec (dllexport) int _stdcall SetCsvBrtFile(HANDLE hResource, char* szCsv, char* szBrt)
{
	ASSERT(hResource != NULL);
	CQDOperator* pQDoperator = (CQDOperator*)hResource;
	pQDoperator->SetCsv(szCsv);
	pQDoperator->SetBrt(szBrt);
	return 0;
}

extern "C" __declspec (dllexport) int _stdcall StartBackupBrt(HANDLE hResource)
{
	ASSERT(hResource != NULL);
	CQDOperator* pQDoperator = (CQDOperator*)hResource;
	return pQDoperator->BackupFunc();
}

extern "C" __declspec (dllexport) int _stdcall StartRestoreBrt(HANDLE hResource,bool bFilterRealonly)
{
	ASSERT(hResource != NULL);
	CQDOperator* pQDoperator = (CQDOperator*)hResource;
	return pQDoperator->RestoreFunc(bFilterRealonly);
}

extern "C" __declspec (dllexport) void _stdcall GetComportList(const char* pszTemp)
{
	CComportLister listner;
	CopyMemory((void*)pszTemp, listner.GetComportList(), strlen(listner.GetComportList())+1);
}


/*******************************************************************************/

extern "C" __declspec (dllexport) BOOL _stdcall DuckToNVArray(HANDLE hResource, char* szDuckFile)
{
	ASSERT(hResource != NULL);
	CQDOperator* pQDoperator = (CQDOperator*)hResource;
	return pQDoperator->DuckFile2NVArray(szDuckFile);
}

extern "C" __declspec (dllexport) BOOL _stdcall NVArrayToMobile(HANDLE hResource)
{
	ASSERT(hResource != NULL);
	CQDOperator* pQDoperator = (CQDOperator*)hResource;
	return pQDoperator->NVArray2Mobile();
}

extern "C" __declspec (dllexport) int _stdcall Close(HANDLE hResource)
{
	ASSERT(hResource != NULL);
	CQDOperator* pQDoperator = (CQDOperator*)hResource;
	delete pQDoperator;
	pQDoperator = NULL;
	return 0;
}