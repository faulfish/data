#include <string>

#define	DEFAULT_CMD_TIME_OUT	3000	//default time out 

typedef enum
{
 DIAG_ERROR_SUCCESS          = 0,
 DIAG_ERROR_CONNECTION_FAIL        = 2,
 DIAG_ERROR_TXRX_FAIL         = 3,
 DIAG_ERROR_INVALID_FUNCTION_PARAMETER     = 4,
 DIAG_ERROR_READ_NVITEM_INVALID_STATUS     = 5,
 DIAG_ERROR_READ_NVITEM_NOTACTIVE      = 6, 
 DIAG_ERROR_WRITE_NVITEM_INTERNAL_DMSS_USED    = 7,
 DIAG_ERROR_WRITE_NVITEM_UNRECOGNIZED_COMMAND   = 8,
 DIAG_ERROR_WRITE_NVITEM_FULL_NV_MEMORY     = 9,
 DIAG_ERROR_WRITE_NVITEM_COMMAND_FAIL     =10,
 DIAG_ERROR_WRITE_NVITEM_VARIABLE_NOT_ACTIVE    =11, //never written
 DIAG_ERROR_WRITE_NVITEM_BAD_PARAMETER_IN_COMMAND_BLOCK =12,
 DIAG_ERROR_WRITE_NVITEM_READ_ONLY       =13,
 DIAG_ERROR_WRITE_NVITEM_ITEM_NOT_DEFINED    =14, //Item not defined for this target!!
 DIAG_ERROR_WRITE_NVITEM_FREE_MEMORY_EXHAUSTED   =15,
 DIAG_ERROR_WRITE_NVITEM_INTERNAL_USE     =16,
 DIAG_ERROR_WRITE_NVITEM_ERROR       =17,
 DIAG_ERROR_MODE_UNDETECTED        =18,
 DIAG_ERROR_UNEXPECTED_RESPONSE       =19,
 DIAG_ERROR_DLMODE          =20,
 DIAG_ERROR_EMPTY_OTP         =21,
 DIAG_ERROR_DAMAGED_OTP         =22,
 DIAG_ERROR_WRONG_OTP_PROTECTED_CODE      =23,
 DIAG_ERROR_GEN_SIMDATA         =24,
 DIAG_ERROR_CODE_LOCKED         =25
} DIAG_ERROR;

//Unlock SIM lock (common)
typedef bool UnlockSPC(long nComport, char* szLock,int nTimeOut/*=DEFAULT_CMD_TIME_OUT*/);  
typedef bool UnlockPhoneCode(long nComport, char* szLock, int nTimeOut/*=DEFAULT_CMD_TIME_OUT*/);  //Master code (SSCP)
typedef bool UnlockNW(long nComport, char* szLock, int nTimeOut/*=DEFAULT_CMD_TIME_OUT*/);			//Network Personalization
typedef bool UnlockNS(long nComport, char* szLock, int nTimeOut/*=DEFAULT_CMD_TIME_OUT*/);			//Network Subset Personalization
typedef bool UnlockSP(long nComport, char* szLock, int nTimeOut/*=DEFAULT_CMD_TIME_OUT*/);			//Service Provider Personalization
typedef bool UnlockCP(long nComport, char* szLock, int nTimeOu/*t=DEFAULT_CMD_TIME_OUT*/);			//Corporate Personalization
typedef bool UnlockSIM(long nComport, char* szLock,int nTimeOut/*=DEFAULT_CMD_TIME_OUT*/);		//SIM Personalization

typedef bool IsSSCPLocked(long nComPort,long& nStatus/*0=unlocked, 1=locked, 2=Unlocked by default code*/, int nTimeOut/*=DEFAULT_CMD_TIME_OUT*/);
typedef bool IsNWSCPLocked(long nComPort,
						   long & bNWUnlocked, // Network Personalization
						   long & bNSUnlocked, // Network Subset Personalization
						   long & bSPUnlocked, // Service Provider Personalization
						   long & bCPUnlocked, // Corporate Personalization
						   long & bSIMUnlocked,// SIM Personalization
						   int nTimeOut/*=DEFAULT_CMD_TIME_OUT*/);
typedef bool IsSPCLocked(long nComPort, long& nStatus/*0=unlocked, 1=locked, 2=Unlocked by default code*/, int nTimeOut/*=DEFAULT_CMD_TIME_OUT*/);
typedef bool WRITESPC(long nComPort,const char* szLock6,int nTimeout /*= DEFAULT_CMD_TIME_OUT*/);
typedef bool READSPC(long nComPort, long & nStatus/*0=unlocked, 1=locked, 2=Unlocked by default code*/, char* szSPC6, int nTimeOut/*=DEFAULT_CMD_TIME_OUT*/);
typedef bool LockSPC(long nComPort, int nTimeout/*= DEFAULT_CMD_TIME_OUT*/);

typedef bool ReadIMEI(long nComPort, char* szIMEI15, int nTimeout/*=DEFAULT_CMD_TIME_OUT*/);
typedef bool WriteIMEI(long nComPort,const char* szIMEI15, int nTimeout/*=DEFAULT_CMD_TIME_OUT*/);
typedef bool WriteBTAddress(long nComPort, const char* strBTAddr12, int nTimeout/*=DEFAULT_CMD_TIME_OUT*/);
typedef bool ReadBTAddress(long nComPort, char* strBTAddr12, int nTimeout/*=DEFAULT_CMD_TIME_OUT*/);
typedef bool ReadFSN(long nComPort, BYTE* szFSN16,int nTimeout /*= DEFAULT_CMD_TIME_OUT*/);
typedef bool WriteFSN(long nComPort,const BYTE* szFSN16, int nTimeout /*= DEFAULT_CMD_TIME_OUT*/);


//Freeze IMEI,FSN,BT(common)
typedef bool FreezeOTP(long nComPort, int nTimeout/* = DEFAULT_CMD_TIME_OUT*/);
//typedef bool IsOTPFrozen(long nComPort, bool& bFrozen, int nTimeout = DEFAULT_CMD_TIME_OUT*/);


//Write SIM data(common)
typedef bool WriteSimData(long nComport, BYTE *pSimDataBuf, int nSimDataBufLen, int nTimeOut/*=DEFAULT_CMD_TIME_OUT*/);

//Read/Write Factory Area
typedef bool ReadFactoryArea(long nComport, WORD nNVItem, unsigned char* szFactoryArea128, int& nState, int nTimeOut/*=DEFAULT_CMD_TIME_OUT*/);
typedef bool WRITEFACTORYAREA(long nComport, WORD nNVItem, const unsigned char* szFactoryArea128, int nTimeOut/*=DEFAULT_CMD_TIME_OUT*/);

//Backup/Restore
typedef bool BNV(long nComport, int nTimeout/* = 8000*/);
typedef bool RestoreNV(long nComport, int nTimeout /*= 20000*/);
typedef bool BackupBrt(long nComport, const LPCSTR szCsv, const LPCSTR szBrt);
typedef bool RestoreBrt(long nComport, const LPCSTR szCsv, const LPCSTR szBrt);

//Read SW load version from phone for check (common)
typedef bool ReadSWVersion(long nComPort,char* szReadValue1024, long& nLength, int nTimeout /*= DEFAULT_CMD_TIME_OUT*/);

//Switch to FTM/ON-LINE mode(common)
typedef bool SetTestMode(long nComPort, int nTimeout /*= DEFAULT_CMD_TIME_OUT*/);
typedef bool SetOnlineMode(long nComPort, int nTimeout /*= DEFAULT_CMD_TIME_OUT*/);

//Check isDLMode
typedef bool IsDLMode(long nComPort);
typedef bool IsConnected(long nComPort);
typedef bool ResetHandset(long nComPort,bool bWaitForRestart, int nTimeout /*= DEFAULT_CMD_TIME_OUT*/);
typedef bool GetReleaseDateTime(long nComPort, char* szDate12, char* szTime9, int nTimeout);



ReadIMEI         *pReadIMEI;
WriteIMEI        *pWriteIMEI;
WriteBTAddress   *pWriteBTAddress;
ReadBTAddress    *pReadBTAddress;
ReadFSN          *pReadFSN;
WriteFSN         *pWriteFSN;

FreezeOTP        *pFreezeOTP;
//IsOTPFrozen      *pIsOTPFrozen;

WriteSimData     *pWriteSimData;

ReadFactoryArea  *pReadFactoryArea;
WRITEFACTORYAREA *pWriteFactoryArea;

BNV              *pBackupNV; 
BackupBrt        *pBackupBrt;
RestoreBrt       *pRestoreBrt;
RestoreNV        *pRestoreNV;

ReadSWVersion    *pReadSWVersion;

SetTestMode      *pSetTestMode;
SetOnlineMode    *pSetOnlineMode;

UnlockSPC        *pUnlockSPC;  
UnlockPhoneCode  *pUnlockPhoneCode;     //Master code
UnlockNW         *pUnlockNW;           //Network Personalization
UnlockNS         *pUnlockNS;            //Network Subset Personalization
UnlockSP         *pUnlockSP;             //Service Provider Personalization
UnlockCP         *pUnlockCP;            //Corporate Personalization
UnlockSIM        *pUnlockSIM;           //SIM Personalization

IsSSCPLocked     *pIsSSCPLocked;
IsNWSCPLocked    *pIsNWSCPLocked;
IsSPCLocked      *pIsSPCLocked;

WRITESPC         *pWriteSPC;
READSPC          *pReadSPC;
LockSPC          *pLockSPC;


IsConnected      *pIsConnected;
ResetHandset     *pResetHandset;
GetReleaseDateTime  *pGetReleaseDateTime;
//ClearActivationData *pClearActivationData;
//StandbyCurrentTest  *pStandbyCurrentTest;
//MasterClear         *pMasterClear;
//WriteQFuse          *pWriteQFuse;
//BTHeadsetChargingPower   *pBTHeadsetChargingPower;

//WriteHeadsetBTAddress  *pWriteHeadsetBTAddress;
//CheckSIM  *pCheckSIM;
//SetOffline  *pSetOffline;
//ResetHandset  *pResetHandset;
//CheckBTHeadsetState  *pCheckBTHeadsetState;
//pWriteHeadBTAddr  pWriteHeadsetBTAddress;

//MobileToolKit
typedef bool (*pFnReadBatteryVoltage)(int nComportm, int& nVoltage);
typedef bool (*pFnWriteDefaultPhoneCode)(int nComport);
typedef bool (*pFnFTMMode)(int nComport);  //just change NV, don't need restart
typedef bool (*pFnCheckFTM)(int nComport, int& nFTMValue);
typedef bool (*pFnWriteSimLockFile)(int nComport, const std::string & strSIMLockFileName);
typedef bool (*pFnIsOTPFrozen)(int nComport, bool& bFrozen,int& nLastError);
typedef bool (*pFnWriteSIMLockFileNameToNV_VC6)(int nComport,const char* szSIMLockFileName);
typedef bool (*pFnEnableLog)(bool bEnableLog);
typedef bool (*pFnWriteSimDataV2)(long nComport, BYTE *pSimDataBuf, int nSimDataBufLen);


pFnReadBatteryVoltage     pReadBatteryVoltage;
pFnWriteDefaultPhoneCode  pWriteDefaultPhoneCode;
pFnFTMMode                pFTMMode;
pFnCheckFTM               pCheckFTM;
pFnWriteSimLockFile       pWriteSimLockFile;
pFnIsOTPFrozen            pIsOTPFrozen;
pFnWriteSIMLockFileNameToNV_VC6    pWriteSIMLockFileNameToNV_VC6;
pFnEnableLog              pEnableLog;
pFnWriteSimDataV2         pWriteSimDataV2;

//Simencode.dll
typedef struct
{
	char *szNetworkCode;							///< ' Unlock code for Network Lock.
	char *szServiceProviderCode;					///< ' Unlock code for Service Provider Lock.
	char *szCorporateCode;							///< ' Unlock code for Corporate Lock.
	char *szNetworkSubsetCode;						///< ' Unlock code for Network Subset Lock.
	char *szSimCode;								///< ' Unlock code for SIM Lock.
	char *szUsimCode;								///< ' Unblock code for User SIM Lock.
	char *szReserved;								///< ' Reserved code for furture usage.
} T_SIM_UNLOCK_CODE;

typedef int (__stdcall *ISimEnc_AllocSimEncObj)(int, void*&);
typedef int (__stdcall *ISimEnc_SetImei)(const void*, char*, int);
typedef int (__stdcall *ISimEnc_SetFlashSerialNumber)(const void*, char*, int);
typedef int (__stdcall *ISimEnc_SetSimLockFile)(const void*, char*);
typedef int (__stdcall *ISimEnc_SetUnlockCodes)(const void*, T_SIM_UNLOCK_CODE *ptSimUnlockCode);
typedef int (__stdcall *ISimEnc_CheckSimLockData)(const void*, unsigned char*, int);
typedef int (__stdcall *ISimEnc_FreeSimEncObj)(const void *);
typedef int (__stdcall *ISimEnc_GenSimLockData)(const void *hSimEncHandle, unsigned char *pcSimLockData, int *pnSimLockDataLen);

ISimEnc_AllocSimEncObj AllocSimEncObj;
ISimEnc_SetImei SetImei;
ISimEnc_SetFlashSerialNumber SetFlashSerialNumber;
ISimEnc_SetSimLockFile SetSimLockFile;
ISimEnc_SetUnlockCodes SetUnlockCodes;
ISimEnc_CheckSimLockData CheckSimLockData;
ISimEnc_FreeSimEncObj FreeSimEncObj;
ISimEnc_GenSimLockData GenSimLockData;
