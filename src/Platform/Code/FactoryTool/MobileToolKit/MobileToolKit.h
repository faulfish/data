// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MOBILETOOLKIT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MOBILETOOLKIT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MOBILETOOLKIT_EXPORTS
#define MOBILETOOLKIT_API extern "C" __declspec(dllexport)
#else
#define MOBILETOOLKIT_API extern "C" __declspec(dllimport)
#endif
#include <string>

/*
typedef enum
{
	DIAG_ERROR_SUCCESS										= 0,
	DIAG_ERROR_CONNECTION_FAIL								= 2,
	DIAG_ERROR_TXRX_FAIL									= 3,
	DIAG_ERROR_INVALID_FUNCTION_PARAMETER					= 4,
	DIAG_ERROR_READ_NVITEM_INVALID_STATUS					= 5,
	DIAG_ERROR_READ_NVITEM_NOTACTIVE						= 6,	
	DIAG_ERROR_WRITE_NVITEM_INTERNAL_DMSS_USED				= 7,
	DIAG_ERROR_WRITE_NVITEM_UNRECOGNIZED_COMMAND			= 8,
	DIAG_ERROR_WRITE_NVITEM_FULL_NV_MEMORY					= 9,
	DIAG_ERROR_WRITE_NVITEM_COMMAND_FAIL					=10,
	DIAG_ERROR_WRITE_NVITEM_VARIABLE_NOT_ACTIVE				=11, //never written
	DIAG_ERROR_WRITE_NVITEM_BAD_PARAMETER_IN_COMMAND_BLOCK	=12,
	DIAG_ERROR_WRITE_NVITEM_READ_ONLY		 				=13,
	DIAG_ERROR_WRITE_NVITEM_ITEM_NOT_DEFINED				=14, //Item not defined for this target!!
	DIAG_ERROR_WRITE_NVITEM_FREE_MEMORY_EXHAUSTED			=15,
	DIAG_ERROR_WRITE_NVITEM_INTERNAL_USE					=16,
	DIAG_ERROR_WRITE_NVITEM_ERROR							=17,
	DIAG_ERROR_MODE_UNDETECTED								=18,
	DIAG_ERROR_UNEXPECTED_RESPONSE							=19,
	DIAG_ERROR_DLMODE										=20,
	DIAG_ERROR_EMPTY_OTP									=21,
	DIAG_ERROR_DAMAGED_OTP									=22,
	DIAG_ERROR_WRONG_OTP_PROTECTED_CODE						=23,
	DIAG_ERROR_GEN_SIMDATA									=24,
	DIAG_ERROR_CODE_LOCKED									=25
} DIAG_ERROR;
*/
#define	DEFAULT_CMD_TIME_OUT	3000	//default time out 

typedef bool (*pFnEnableLog)(bool bEnableLog);
MOBILETOOLKIT_API bool EnableLog(bool bEnableLog);

typedef bool (*pFnGetBTAddr)(int nPort,char* outBTAddrBuffer,int nBufferSize);
MOBILETOOLKIT_API bool GetBTAddr(int nPort,char* outBTAddrBuffer,int nBufferSize);

//Read/write IMEI/BT/FSN/SPC(common)
typedef bool (*pFnGetIMEI)(int nPort,char* outIMEI);
MOBILETOOLKIT_API bool GetIMEI(int nPort,char* outIMEI);

typedef bool (*pFnWriteIMEI)(int nPort,const char* inIMEI);
MOBILETOOLKIT_API bool WriteIMEI(int nPort,const char* inIMEI);

typedef bool (*pFnLockSPC)(int nPort);
MOBILETOOLKIT_API bool LockSPC(int nPort);

typedef bool (*pFnIsSPCLocked)(int nPort,long& nStatus);
MOBILETOOLKIT_API bool IsSPCLocked(int nPort,long& nStatus);

typedef bool (*pFnUnlockSPC)(int nPort,const char* szLock, bool& bUnlockOK);
MOBILETOOLKIT_API bool UnlockSPC(int nPort,const char* szLock, bool& bUnlockOK);

typedef bool (*pFnProgramSPC)(int nPort,const char* szLock);
MOBILETOOLKIT_API bool ProgramSPC(int nPort,const char* szLock);

/*
Return true means the command had send to handset.
Return false means timeout to get the response from handset

The return value can't promise the BT pair is ok or not.
*/
typedef bool (*pFnWriteHeadsetBTAddress)(const char* strBTAddr, int nPort);
MOBILETOOLKIT_API bool WriteHeadsetBTAddress(const char* strBTAddr, int nPort);

/*
說明是否配對成功過
*/
typedef bool (*pFnCheckBTHeadsetState)(const char *szBTAddr,int nComport);
MOBILETOOLKIT_API bool CheckBTHeadsetState(const char *szBTAddr,int nComport);

typedef bool (*pFnClearActivationData)(int nPort);
MOBILETOOLKIT_API bool ClearActivationData(int nPort);

typedef bool (*pFnMasterClear)(int nPort);
MOBILETOOLKIT_API bool MasterClear(int nPort);

MOBILETOOLKIT_API bool BTHeadsetChargingPower(int nPort, bool IsOn);
MOBILETOOLKIT_API bool BTHeadsetVoltageReading(int nComport, long& nVoltage, int nTimeout);

MOBILETOOLKIT_API bool StandbyCurrentTest(int nPort);
MOBILETOOLKIT_API bool CheckSIMCardExistence(int nPort, int& nType);

MOBILETOOLKIT_API bool ReadFactoryAreaNew(int nComport, WORD nNVItem, unsigned char* szFactoryArea128, int& nState, int nTimeOut);

typedef bool (*pFnWriteMobileID)(int nComport,char* szSlot3);
MOBILETOOLKIT_API bool WriteMobileID(int nComport, char* szSlot3);
typedef bool (*pFnReadMobileID)(int nComport, char* szLine7,char* szAdapter3,char* szSlot3,char* szDay3,char* szMonth3,char* szYear3,char* szHour3,char* szMinute3,char* szSecond3);
MOBILETOOLKIT_API bool ReadMobileID(int nComport, char* szLine7,char* szAdapter3,char* szSlot3,char* szDay3,char* szMonth3,char* szYear3,char* szHour3,char* szMinute3,char* szSecond3);

typedef bool (*pFnReadHWVersion_FromFactoryArea)(int nComport, char* szHWVersion);
MOBILETOOLKIT_API bool ReadHWVersion_FromFactoryArea(int nComport, char* szHWVersion);
typedef bool (*pFnWriteHWVersion_ToFactoryArea)(int nComport, char* szHWVersion);
MOBILETOOLKIT_API bool WriteHWVersion_ToFactoryArea(int nComport, char* szHWVersion);

typedef bool (*pFnEnableQualcommDebugEvent)(int nComport,bool bEnable);
MOBILETOOLKIT_API bool EnableQualcommDebugEvent(int nComport,bool bEnable);

//Write SIM data
typedef bool (*pFnWriteSimData)(long nComport, BYTE *pSimDataBuf, int nSimDataBufLen);
MOBILETOOLKIT_API bool WriteSimData(long nComport, BYTE *pSimDataBuf, int nSimDataBufLen);// only support for SimLen=512 (ex:Athens)
typedef bool (*pFnWriteSimDataV2)(long nComport, BYTE *pSimDataBuf, int nSimDataBufLen);
MOBILETOOLKIT_API bool WriteSimDataV2(long nComport, BYTE *pSimDataBuf, int nSimDataBufLen);// support for SimLen=512, 576... (Athens, Lavernock)
typedef bool (*pFnWriteSimLockFile)(long nComport, const std::string& strSIMLockFileName);
MOBILETOOLKIT_API bool WriteSimLockFile(long nComport, const std::string& strSIMLockFileName);

//Backup\Restore NV
typedef bool (*pFnBackupNV2BRT)(long nComport,const std::string& strOutputBRTFileName,const std::string& strInputCSVFile);
MOBILETOOLKIT_API bool BackupNV2BRT_VC6(long nComport,const char* strOutputBRTFileName,const char* strInputCSVFile);
MOBILETOOLKIT_API bool BackupNV2BRT(long nComport,const std::string& strOutputBRTFileName,const std::string& strInputCSVFile);
typedef bool (*pFnRestoreBRT2NV)(long nComport,const std::string& strInBRTFileName);
MOBILETOOLKIT_API bool RestoreBRT2NV(long nComport,const std::string& strInBRTFileName);
typedef bool (*pFnBackupNVPartition)(long nComport);
MOBILETOOLKIT_API bool BackupNVPartition(long nComport);
typedef bool (*pFnRestoreNVPartition)(long nComport);
MOBILETOOLKIT_API bool RestoreNVPartition(long nComport);
typedef bool (*pFnCheckNVPartition)(long nComport, int& nBackupTimes, CTime& LastBackupTime);
MOBILETOOLKIT_API bool CheckNVPartition(long nComport, int& nBackupTimes, CTime& LastBackupTime);

//Wireless
typedef bool (*pFnEnableGAP)(long nComport,bool bEnable);
MOBILETOOLKIT_API bool EnableGAP(long nComport,bool bEnable);
typedef bool (*pFnStartBTPower)(long nComport,int nDegree);
MOBILETOOLKIT_API bool StartBTPower(long nComport,int nDegree);
typedef bool (*pFnEnterBTTestMode)(int nComport,bool bEnable);
MOBILETOOLKIT_API bool EnterBTTestMode(long nComport,bool bEnable);
typedef bool (*pFnFMRegistration)(int nComport);
MOBILETOOLKIT_API bool FMRegistration(int nComport);
typedef bool (*pFnFMConnect)(int nComport,bool bConnect);
MOBILETOOLKIT_API bool FMConnect(int nComport, bool bConnect);
typedef bool (*pFnFMInitialize)(int nComport);
MOBILETOOLKIT_API bool FMInitialize(int nComport);
typedef bool (*pFnFMSetStation)(int nComport, WORD nFMFrequence);
MOBILETOOLKIT_API bool FMSetStation(int nComport, WORD nFMFrequence);
typedef bool (*pFnFMMute)(int nComport,bool bMute);
MOBILETOOLKIT_API bool FMMute(int nComport,bool bMute);
typedef bool (*pFnFMReceiveRSSI)(int nComport,WORD& nRSSI);
MOBILETOOLKIT_API bool FMReceiveRSSI(int nComport,WORD& nRSSI);
typedef bool (*pFnFMSpeaker)(int nComport);
MOBILETOOLKIT_API bool FMSpeaker(int nComport);
typedef bool (*pFnFMMonoHeadset)(int nComport);
MOBILETOOLKIT_API bool FMMonoHeadset(int nComport);
typedef bool (*pFnFMStereoHeadset)(int nComport);
MOBILETOOLKIT_API bool FMStereoHeadset(int nComport);

//NV items
typedef bool (*pFnReadNVItem)(long nComport,const WORD& NVItem, BYTE  item_data[128],WORD&  nv_stat);
MOBILETOOLKIT_API bool ReadNVItem(long nComport, const WORD& NVItem, BYTE  item_data[128], WORD&  nv_stat);//	Read NV Item from the phone
typedef bool (*pFnWriteNVItem)(long nComport,const WORD& NVItem, const BYTE *WriteData, const size_t szWriteData,WORD&  nv_stat);
MOBILETOOLKIT_API bool WriteNVItem(long nComport,const WORD& NVItem, const BYTE *WriteData, const size_t szWriteData,WORD&  nv_stat);	

//ReadWrite specific NV item
typedef bool (*pFnWriteSIMLockFileNameToNV)(int nComport,const std::string& strSIMLockFileName);
MOBILETOOLKIT_API bool WriteSIMLockFileNameToNV(int nComport,const std::string& strSIMLockFileName);
typedef bool (*pFnReadSIMLockFileNameFromNV)(int nComport,std::string& strSIMLockFileName);
MOBILETOOLKIT_API bool ReadSIMLockFileNameFromNV(int nComport,std::string& strSIMLockFileName);
typedef bool (*pFnWriteSIMLockFileNameToNV_VC6)(int nComport,const char* szSIMLockFileName);
MOBILETOOLKIT_API bool WriteSIMLockFileNameToNV_VC6(int nComport,const char* strSIMLockFileName);

//Get version
typedef bool (*pFnGetSWHWVersion)(long nComport,LPCSTR sSW, LPCSTR sHW);
MOBILETOOLKIT_API bool GetSWHWVersion(long nComport,LPCSTR sSW, LPCSTR sHW);
typedef bool (*pFnGetLavernockHWVersion)(long nComport,int& nVersion);
MOBILETOOLKIT_API bool GetLavernockHWVersion(long nComport,int& nVersion);

//Read/Write QFuse
typedef bool (*pFnWriteQFuse)(int nPort, unsigned int chain, unsigned long int Config_Register[5]);
MOBILETOOLKIT_API bool WriteQFuse(int nPort, unsigned int chain, unsigned long int Config_Register[5]);
typedef bool (*pFnReadQFuseState)(int nComport, unsigned long& reg1, unsigned long& reg2, bool& bSuccess);
MOBILETOOLKIT_API bool ReadQFuseState(int nComport, unsigned long& reg1, unsigned long& reg2, bool& bSuccess);

//Mode

/*
// Ref: DIAG_CONTROL_F_MODE enumeration
//	MODE_OFFLINE_A_F = 0,	//!<' Go to offline analog
//	MODE_OFFLINE_D_F = 1,	//!<' Go to offline digital 
//	MODE_RESET_F = 2,		//!<' Reset. Only exit from offline 
//	MODE_FTM_F = 3,			//!<' FTM mode
//	MODE_ONLINE_F = 4,		//!<' Go to Online 
//	MODE_LPM_F = 5,			//!<' Low Power Mode (if supported)
//	MODE_POWER_OFF_F = 6,	//!<' Power off (if supported)
//	MODE_MAX_F = 7			//!<' Last (and invalid) mode enum value
*/
typedef bool (*pFnChangeMode)(int nComport,const unsigned int mode);
MOBILETOOLKIT_API bool ChangeMode(int nComport,const unsigned int mode);

//FTM Mode

typedef bool (*pFnFTMMode)(int nComport);
MOBILETOOLKIT_API bool FTMMode(int nComport);
typedef bool (*pFnChangeFTM)(int nComport);
MOBILETOOLKIT_API bool ChangeFTM(int nComport); //Write NV item , must be reset
typedef bool (*pFnCheckFTM)(int nComport, int& nFTMValue);
MOBILETOOLKIT_API bool CheckFTM(int nComport, int& nFTMValue);//Read NV item

//Online mode
MOBILETOOLKIT_API bool OnLineMode(int nComport);
MOBILETOOLKIT_API bool SetOnline(int nComport);
MOBILETOOLKIT_API bool SetOffline(int nComport);
MOBILETOOLKIT_API bool ChangeOnline(int nComport);

//PowerOff
MOBILETOOLKIT_API bool PowerOff(int nComport);

//EM mode
typedef bool (*pFnTurnOnOffEM)(int nComport,bool bOpenEM);
MOBILETOOLKIT_API bool TurnOnOffEM(long nComport,bool bOpenEM);

typedef bool (*pFnResetHandset)(int nComport);
MOBILETOOLKIT_API bool ResetHandset(int nComport);

//Failurebyte
typedef bool (*pFnWriteFailureByte)(int nComport,BYTE nFailureByte);
MOBILETOOLKIT_API bool WriteFailureByte(int nComport,BYTE nFailureByte);

// Search BT device
MOBILETOOLKIT_API bool SearchBTDevice(int nComport);
// Read Battery voltage
//3100 mV will close
//3500 mV is safe
//4000 mV is for factory
typedef bool (*pFnReadBatteryVoltage)(int nComport, int& nVoltage);
MOBILETOOLKIT_API bool ReadBatteryVoltage(int nComportm, int& nVoltage);
typedef bool (*pFnReadBatteryTemperature)(int nComport, int& nTemperature);
MOBILETOOLKIT_API bool ReadBatteryTemperature(int nComportm, int& nTemperature);

// WriteDefaultPhoneCode 1234
typedef bool (*pFnWriteDefaultPhoneCode)(int nComport);
MOBILETOOLKIT_API bool WriteDefaultPhoneCode(int nComport);

typedef bool (*pFnFreezeOTP)(int nComport);
MOBILETOOLKIT_API bool FreezeOTP(long nComPort);
//DIAG_ERROR_WRONG_OTP_PROTECTED_CODE
//DIAG_ERROR_EMPTY_OTP
//DIAG_ERROR_DAMAGED_OTP
typedef bool (*pFnIsOTPFrozen)(int nComport, bool& bFrozen,int& nLastError);
MOBILETOOLKIT_API bool IsOTPFrozen(long nComPort, bool& bFrozen,int& nLastError);


typedef bool (*pFnReplaceEFSFile)(int nComport, const std::string& strEFSFilePath, const std::string& strNewLocalFilePath);
MOBILETOOLKIT_API bool ReplaceEFSFile(int nComPort, const std::string& strEFSFilePath, const std::string& strNewLocalFilePath);

typedef bool (*pFnIsGSensorWorking)(int nComport);
MOBILETOOLKIT_API bool IsGSensorWorking(int nComPort);
typedef bool (*pFnIsGSensorWorkingV2)(int nComport,bool& bIsGSensorWork,bool& bIsUpWork);
MOBILETOOLKIT_API bool IsGSensorWorkingV2(int nComPort,bool& bIsGSensorWork,bool& bIsUpWork);
typedef bool (*pFnIsGSensorWorkingV3)(int nComport,int& nX,int& nY,int& nZ);
MOBILETOOLKIT_API bool IsGSensorWorkingV3(int nComPort,int& nX,int& nY,int& nZ);

typedef bool (*pFnCheckSIMLock)(int nComport,std::string& strSIMLockName);
MOBILETOOLKIT_API bool CheckSIMLock(int nComPort,std::string& strSIMLockName);
typedef bool (*pFnCheckSIMLock_VC6)(int nComport,char* szSIMLockNameBuffer,int nBufferSize);
MOBILETOOLKIT_API bool CheckSIMLock_VC6(int nComPort,char* szSIMLockNameBuffer,int nBufferSize);

typedef bool (*pFnCheckCEFSFileName)(int nComport,char* szCEFSFileNameBuffer,int nBufferSize);
MOBILETOOLKIT_API bool CheckCEFSFileName(int nComPort,char* szCEFSFileNameBuffer,int nBufferSize);

typedef bool (*pFnDLRAWRequest)(int nComport, const char* szCmdInput, int nCmdLength, bool bAddCRC);
MOBILETOOLKIT_API bool DLRAWRequest(int nComport, const char* szCmdInput, int nCmdLength, bool bAddCRC);

typedef bool (*pFnDiagRAWRequest)(int nComport, const char* szCmdInput, int nCmdLength, bool bAddCRC,bool bHasReturn );
MOBILETOOLKIT_API bool DiagRAWRequest(int nComport, const char* szCmdInput, int nCmdLength, bool bAddCRC,bool bHasReturn);

typedef bool (*pFnByPassCellTemperatureCheck)(int nComport);
MOBILETOOLKIT_API bool ByPassCellTemperatureCheck(int nComport);

typedef bool (*pIsInDownloadMode)(int nComport);
MOBILETOOLKIT_API bool IsInDownloadMode(int nComport);