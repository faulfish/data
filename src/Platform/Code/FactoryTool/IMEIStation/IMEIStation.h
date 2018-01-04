// IMEIStation.h : main header file for the IMEIStation DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "../../Diag_Export/QualcommDiag.h"
#include "../../odmtma_api/DownloadDLL/QDOperator.h"

#ifdef IMEISTATION_EXPORTS
#define IMEISTATION_API extern "C" __declspec(dllexport)
#else
#define IMEISTATION_API extern "C" __declspec(dllimport)
#endif


// CIMEIStationApp
// See IMEIStation.cpp for the implementation of this class
//

class CIMEIStationApp : public CWinApp
{
public:
	CIMEIStationApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

#ifndef DEFAULT_CMD_TIME_OUT
#define	DEFAULT_CMD_TIME_OUT	3000	//default time out 
#endif

//Unlock SIM lock (common)
IMEISTATION_API bool UnlockSPC(long nComport, char* szLock,int nTimeOut=DEFAULT_CMD_TIME_OUT);  
IMEISTATION_API bool UnlockPhoneCode(long nComport, char* szLock, int nTimeOut=DEFAULT_CMD_TIME_OUT);  //Master code (SSCP)
IMEISTATION_API bool UnlockNW(long nComport, char* szLock, int nTimeOut=DEFAULT_CMD_TIME_OUT);			//Network Personalization
IMEISTATION_API bool UnlockNS(long nComport, char* szLock, int nTimeOut=DEFAULT_CMD_TIME_OUT);			//Network Subset Personalization
IMEISTATION_API bool UnlockSP(long nComport, char* szLock, int nTimeOut=DEFAULT_CMD_TIME_OUT);			//Service Provider Personalization
IMEISTATION_API bool UnlockCP(long nComport, char* szLock, int nTimeOut=DEFAULT_CMD_TIME_OUT);			//Corporate Personalization
IMEISTATION_API bool UnlockSIM(long nComport, char* szLock,int nTimeOut=DEFAULT_CMD_TIME_OUT);		//SIM Personalization

IMEISTATION_API bool IsSSCPLocked(long nComPort,long& nStatus/*0=unlocked, 1=locked, 2=Unlocked by default code*/, int nTimeOut=DEFAULT_CMD_TIME_OUT);
IMEISTATION_API bool IsNWSCPLocked(long nComPort,
								   long& nNWStatus, // Network Personalization /*0=unlocked, 1=locked*/
								   long& nNSStatus, // Network Subset Personalization /*0=unlocked, 1=locked*/
								   long& nSPStatus, // Service Provider Personalization /*0=unlocked, 1=locked*/
								   long& nCPnStatus, // Corporate Personalization /*0=unlocked, 1=locked*/
								   long& nSIMStatus,// SIM Personalization  /*0=unlocked, 1=locked*/
								   int nTimeOut=DEFAULT_CMD_TIME_OUT);
IMEISTATION_API bool ReadSPC(long nComPort, long& nStatus/*0=unlocked, 1=locked, 2=Unlocked by default code*/, char* szSPC6, int nTimeOut=DEFAULT_CMD_TIME_OUT);
IMEISTATION_API bool IsSPCLocked(long nComPort, long& nStatus/*0=unlocked, 1=locked, 2=Unlocked by default code*/, int nTimeOut=DEFAULT_CMD_TIME_OUT);
IMEISTATION_API bool WriteSPC(long nComPort,const char* szLock6,int nTimeout = DEFAULT_CMD_TIME_OUT);
IMEISTATION_API bool LockSPC(long nComPort, int nTimeout = DEFAULT_CMD_TIME_OUT);

//Read/write IMEI/BT/FSN/SPC(common)
IMEISTATION_API bool ReadIMEI(long nComPort, char* szIMEI15, int nTimeout=DEFAULT_CMD_TIME_OUT);
IMEISTATION_API bool WriteIMEI(long nComPort,const char* szIMEI15, int nTimeout=DEFAULT_CMD_TIME_OUT);
IMEISTATION_API bool WriteBTAddress(long nComPort, const char* strBTAddr12, int nTimeout=DEFAULT_CMD_TIME_OUT);
IMEISTATION_API bool ReadBTAddress(long nComPort, char* strBTAddr12, int nTimeout=DEFAULT_CMD_TIME_OUT);
IMEISTATION_API bool ReadFSN(long nComPort, BYTE* szFSN16,int nTimeout = DEFAULT_CMD_TIME_OUT);
IMEISTATION_API bool WriteFSN(long nComPort,const BYTE* szFSN16, int nTimeout = DEFAULT_CMD_TIME_OUT);


//Freeze IMEI,FSN,BT(common)
IMEISTATION_API bool FreezeOTP(long nComPort, int nTimeout = DEFAULT_CMD_TIME_OUT);
IMEISTATION_API bool IsOTPFrozen(long nComPort, bool& bFrozen, int nTimeout = DEFAULT_CMD_TIME_OUT);

//Write SIM data(common)
IMEISTATION_API bool WriteSimData(long nComport, BYTE *pSimDataBuf, int nSimDataBufLen, int nTimeOut=DEFAULT_CMD_TIME_OUT);

//Read/Write Factory Area
IMEISTATION_API bool ReadFactoryArea(long nComport, WORD nNVItem, unsigned char* szFactoryArea128, int& nState, int nTimeOut=DEFAULT_CMD_TIME_OUT);
IMEISTATION_API bool WriteFactoryArea(long nComport, WORD nNVItem, const unsigned char* szFactoryArea128, int nTimeOut=DEFAULT_CMD_TIME_OUT);

//Backup/Restore
IMEISTATION_API bool BackupNV(long nComport, int nTimeout = 8000);
IMEISTATION_API bool RestoreNV(long nComport, int nTimeout = 20000);
IMEISTATION_API bool BackupBrt(long nComport, const LPCSTR szCsv, const LPCSTR szBrt);
typedef bool (*pFnBackupBrtV2)(long nComport, const LPCSTR szCsv, const LPCSTR szBrt,LPSTR szErrorListBuffer,int nBufferSize);
IMEISTATION_API bool BackupBrtV2(long nComport, const LPCSTR szCsv, const LPCSTR szBrt,LPSTR szErrorListBuffer = NULL,int nBufferSize = 0);
IMEISTATION_API bool RestoreBrt(long nComport, const LPCSTR szBrt);

//Read SW load version from phone for check (common)
IMEISTATION_API bool ReadSWVersion(long nComPort,char* szReadValue1024, long& nLength, int nTimeout = DEFAULT_CMD_TIME_OUT);
IMEISTATION_API bool GetReleaseDateTime(long nComPort, char* szDate12, char* szTime9, int nTimeout = DEFAULT_CMD_TIME_OUT);


//Switch to FTM/ON-LINE mode(common)
IMEISTATION_API bool SetTestMode(long nComPort, int nTimeout = DEFAULT_CMD_TIME_OUT);
IMEISTATION_API bool SetOnlineMode(long nComPort, int nTimeout = DEFAULT_CMD_TIME_OUT);

typedef bool (*pFnResetHandset)(long nComPort,bool bWaitForRestart, int nTimeout);
IMEISTATION_API bool ResetHandset(long nComPort,bool bWaitForRestart, int nTimeout = DEFAULT_CMD_TIME_OUT);

//Check Mode
IMEISTATION_API bool IsDLMode(long nComPort);
IMEISTATION_API bool IsConnected(long nComPort);


//Read/Write NV
IMEISTATION_API bool ReadNVItem(long nComport,const WORD& NVItem, BYTE  item_data[128],WORD&  nv_stat);//	Read NV Item from the phone
IMEISTATION_API bool WriteNVItem(long nComport,const WORD& NVItem, const BYTE *WriteData, const size_t szWriteData);