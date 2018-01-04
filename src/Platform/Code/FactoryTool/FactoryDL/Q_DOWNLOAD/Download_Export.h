#ifndef __DL_SETTING__H__
#define __DL_SETTING__H__


//typedef struct DLSetting
//{
//	char szHex[256];
//    char szPartition[256];	
//    char szQcsbl[256];	
//    char szQcsblhd[256];	
//    char szOemsbl[256]; 	
//    char szOemsblhd[256];	
//    char szAmss[256]; 	
//    char szAmsshd[256]; 	
//    char szCefs[256];
//	
//}tagDLSetting;



struct tagDLSetting6245
{
	char szHex[256];
	char szPartition[256];
	char szQcsbl[256];
	char szQcsblhd[256];
	char szOemsbl[256]; 
	char szOemsblhd[256];
	char szAmss[256]; 
	char szAmsshd[256]; 
	char szCefs[256];
};
struct tagDLSetting6240
{
	char szHex[256];
	char szPartition[256];
	char szDBL[256];   // like QCSBLHD
	char szFSBL[256]; // like QCSBL
	char szOSBL[256]; // like OEMSBL
	char szAmss[256]; 
	char szCefs[256];
};

////*************Download_Export.dll********************//
//typedef bool _IsDLMode(long nComPort);
//typedef bool _DownloadByPath(long nComPort, bool(*CallBack)(const char*, int , int), const DLSetting &tagSetting);
//typedef bool _ReadFactoryArea(long nComport, WORD nNVItem, unsigned char *szFactoryArea128,int& nState, int nTimeOut);
//typedef bool _WriteFactoryArea(long nComport, WORD nNVItem, const unsigned char *szFactoryArea128, int nTimeOut);
//typedef bool _BackupBrt(long nComport, const LPCSTR szCsv, const LPCSTR szBrt);
//typedef bool _RestoreBrt(long nComPort, const LPCSTR szBrt,bool bFilterRealonly);
//typedef bool _WriteQfused(long nComPort,WORD chain_select, int config_register[5],  int& nQfusedStatus/*0=success, 1=Read Operation failed, 2=Write Operation Failed, 3=Operation on selected chain not supported*/ , int nTimeOut);
//
//typedef bool _BackupNV(long nComport, int nTimeout);
//typedef bool _RestoreNV(long nComport, int nTimeout);
//typedef bool _Signed_CheckVersion(long nComPort, const LPCTSTR szAMSS, const LPCTSTR szOEMSBL, bool& bDownloadOEMSBL, int nTimeOut/* =DEFAULT_CMD_TIME_OUT */);
//typedef bool _UnSigned_CheckVersion(long nComPort, const LPCTSTR szAMSS, const LPCTSTR szOEMSBL, int nTimeOut);
//typedef bool _IsQfused(long nComPort, bool& bQfused, int nTimeOut);
//typedef bool _DownloadDU(long nComPort, bool(*CallBack)(const char*, int , int), const LPCSTR szDuPath);
//typedef bool _Restart(int nPort, int nTimeOut);
//
////*************IMEIStation_StaticM.dll********************//
//typedef bool _UnlockSPC(long nComport, char* szLock,int nTimeOut); 
//typedef bool _IsSPCLocked(long nComPort, long& nStatus/*0=unlocked, 1=locked, 2=Unlocked by default code*/, int nTimeOut);
//typedef bool _ReadIMEI(long nComPort, char* szIMEI15, int nTimeout);
//typedef bool _ReadBTAddress(long nComPort, char* strBTAddr12, int nTimeout);
//
////*************MobileToolKit_StaticM.dll********************//
//typedef bool _GetLavernockHWVersion(long nComport,int& nVersion);
//typedef bool _EnableLog(bool bEnableLog);
//
////*************QisdaQualcommDownloadModule.dll********************//
//
//typedef bool _Download6245(long nComPort, bool(*CallBack)(const char*, int , int), const DLSetting6245 &tagSetting);
//typedef bool _Download6240(long nComPort, bool(*CallBack)(const char*, int , int), const DLSetting6240 &tagSetting);
//
//_BackupBrt              *pBackupBrt[8];
//_RestoreBrt             *pRestoreBrt[8];
//_DownloadByPath         *pDownloadByPath[8];
//_WriteFactoryArea       *pWriteFactoryArea[8];
//_ReadFactoryArea        *pReadFactoryArea[8];
//_WriteQfused             *pWriteQfused[8];
//_IsDLMode               *pIsDLMode[8];
//_BackupNV               *pBackupNV[8];
//_RestoreNV              *pRestoreNV[8];
//_Signed_CheckVersion    *pSigned_CheckVersion[8];
//_UnSigned_CheckVersion  *pUnSigned_CheckVersion[8];
//_IsQfused               *pIsQfused[8];
//_DownloadDU             *pDownloadDU[8];
//_Restart                *pRestart[8];
//
//_UnlockSPC              *pUnlockSPC;
//_IsSPCLocked            *pIsSPCLocked;
//_ReadIMEI               *pReadIMEI[8];        
//_ReadBTAddress          *pReadBTAddress[8];       
//
//_Download6245           *pDownload6245[8]; 
//_Download6240           *pDownload6240[8];
//
//_GetLavernockHWVersion   *pGetLavernockHWVersion[8];
//_EnableLog              *pEnableLog[8];


#endif //__DL_SETTING__H__