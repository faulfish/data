#ifndef __DownloadDll_H__
#define __DownloadDll_H__

#define QDOWNLOADAPI extern "C" __declspec (dllexport)

//#ifdef __cplusplus
//extern "C" {
//#endif

//////////////////////////////////////////////////////////
//////////////////Common Interface////////////////////////
//////////////////////////////////////////////////////////

/*	Open ComPort and return Mobile Handle				*/
/*	szComPort should be formatted as "COM" + "Number"	*/
/*	Such as "COM11".."COM32"...etc.						*/
QDOWNLOADAPI HANDLE _stdcall StartLog();
QDOWNLOADAPI HANDLE _stdcall EndLog();

QDOWNLOADAPI HANDLE _stdcall SetComPort(char* szComPort);

/*  Set CallBackFunc									*/
/*	pFunc(char*, int)									*/
/*	char* send process information						*/
/*	int send process status( 0 = TRUE, -1 = FALSE)		*/
QDOWNLOADAPI int _stdcall SetCallBack(HANDLE hResource, int(*CallBack)(const char*, int));

/* GetComPortList function								*/
QDOWNLOADAPI void _stdcall GetComportList(const char* pszTemp);

//////////////////////////////////////////////////////////
//////////////////For Download////////////////////////////
//////////////////////////////////////////////////////////

/*	Set HEX file for download using						*/
/*	hResource was handle return from SetComPort()		*/
QDOWNLOADAPI int _stdcall SetHexFile(HANDLE hResource, char* szHexFile);

/*	Set download MBN files								*/
/*	hResource was handle return from SetComPort()		*/
QDOWNLOADAPI int _stdcall SetMbnFile(HANDLE hResource, char* szPart, char* szOtp, char* szPbl, 
									 char* szQcsblHd, char* szQcsbl, char* szOemsblHd, 
									 char* szOemsbl, char* szAmssHd, char* szAmss, char* szCefs);

/*	Skip checking "Reset status" after Download finish	*/	
/*	hResource was handle return from SetComPort()		*/
/*	Special for some mobiles, needn't have to do it		*/
QDOWNLOADAPI int _stdcall SetSkipCheck(HANDLE hResource);

/*	Start Download process								*/
/*	hResource was handle return from SetComPort()		*/
QDOWNLOADAPI int _stdcall StartDownload(HANDLE hResource);


//////////////////////////////////////////////////////////
///////////////////For Backup Restore NV//////////////////
//////////////////////////////////////////////////////////

/*  Set CSV and BRT file for NV Backup or Restore using	*/
/*	hResource was handle return from SetComPort()		*/
QDOWNLOADAPI int _stdcall SetCsvBrtFile(HANDLE hResource, char* szCsv, char* szBrt);

/*	Start Backup NV process								*/
/*	hResource was handle return from SetComPort()		*/
QDOWNLOADAPI int _stdcall StartBackupBrt(HANDLE hResource);

/*	Start Restore NV process							*/
/*	hResource was handle return from SetComPort()		*/
QDOWNLOADAPI int _stdcall StartRestoreBrt(HANDLE hResource,bool bFilterRealonly);



//////////////////////////////////////////////////////////
///////////////////Customization//////////////////////////
//////////////////////////////////////////////////////////

QDOWNLOADAPI BOOL _stdcall DuckToNVArray(HANDLE hResource, char* szDuckFile);
QDOWNLOADAPI BOOL _stdcall NVArrayToMobile(HANDLE hResource); 

//////////////////////////////////////////////////////////
///////////////////Common Interface///////////////////////
//////////////////////////////////////////////////////////

/*	Close handle and release resource					*/
/*	hResource was handle return from SetComPort()		*/
QDOWNLOADAPI int _stdcall Close(HANDLE hResource);




//#ifdef __cplusplus
//}	// extern "C"
//#endif 

#endif //__DownloadDll_H__