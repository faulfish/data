
#ifndef __EXPORT_FUNCTION_H__
#define __EXPORT_FUNCTION_H__

#include "../../Modules/QisdaNVModule/NVItem.h"
#define	DEFAULT_CMD_TIME_OUT	3000	//default time out 

// Write Headset BT address into Mobile Trust list
typedef bool (*pFnWriteHeadsetBTAddress)(const char* strBTAddr, int nPort);
// Write BT Address into mobile
typedef bool (*pFnWriteBTAddress)(long nComPort, const char* strBTAddr, int nTimeout);
// Read Mobile BT address
typedef bool (*pFnReadBTAddress)(long nComPort, char* strBTAddr, int nTimeout);
// Read FSN
typedef bool (*pFnReadFSN)(long nComPort, unsigned char* szFSN16,int nTimeout);
// Write FSN
typedef bool (*pFnWriteFSN)(long nComPort,const unsigned char* szFSN16,int nTimeout);
// Write SPC
typedef bool (*pFnWriteSPC)(long nComPort,const char* szLock6,int nTimeout);
//Read SW load version from phone for check (common)
typedef bool (*pFnReadSWVersion)(long nComPort,char* szReadValue1024, long& nLength, int nTimeout);
//Switch to FTM/ON-LINE mode(common)
typedef bool (*pFnSetTestMode)(long nComPort, int nTimeout);

typedef bool (*pFnSetOnlineMode)(long nComPort, int nTimeout);
//Check isDLMode
typedef bool (*pFnIsDLMode)(long nComPort);
//Unlock SIM lock (common)
typedef bool (*pFnUnlockPhoneCode)(long nComport, char* szLock);//Master code
typedef bool (*pFnUnlockNW)(long nComport, char* szLock);		//Network Personalization
typedef bool (*pFnUnlockNS)(long nComport, char* szLock);		//Network Subset Personalization
typedef bool (*pFnUnlockSP)(long nComport, char* szLock);		//Service Provider Personalization
typedef bool (*pFnUnlockCP)(long nComport, char* szLock);		//Corporate Personalization
typedef bool (*pFnUnlockSIM)(long nComport, char* szLock);		//SIM Personalization
typedef bool (*pFnUnlockPROP)(long nComport, char* szLock);		//Antitheft Personalization
//Read/Write Factory Area
typedef bool (*pFnReadFactoryArea)(long nComport, WORD nNVItem, unsigned char* szFactoryArea128, int& nState, int nTimeOut);
typedef bool (*pFnWriteFactoryArea)(long nComport, WORD nNVItem, const unsigned char* szFactoryArea128, int nTimeOut);
//  [7/4/2008 Eddy.Guo]
typedef bool (*pFnReadFactoryAreaNew)(int nComport, WORD nNVItem, unsigned char* szFactoryArea128, int& nState, int nTimeOut);

// clear Activate data
typedef bool (*pFnClearActivationData)(int nPort);
// switch standby current mode
typedef bool (*pFnStandbyCurrentTest)(int nPort);
// Master clear
typedef bool (*pFnMasterClear)(int nPort);

// Open BT charging power / Board level test
typedef bool (*pFnBTHeadsetChargingPower)(int nPort, bool IsOn);
// Check SIM card is working or not
typedef bool (*pFnCheckSIMCardExistence)(int nPort, int& nType);
// BT handset voltage reading
typedef bool (*pFnBTHandsetVoltageReading)(int nPort, long& nVoltage, int nTimeout);
// IsUnitLocked
typedef bool (*pFnIsUnitLocked)(long nPort,long& nStatus, int nLockType);
// 
typedef bool (*pFnSetOnline)(int nComport);
typedef bool (*pFnSetOffline)(int nComport);
typedef bool (*pFnResetHandset)(int nComport);
typedef bool (*pFnNewResetHandset)(long nComPort,bool bWaitForRestart, int nTimeout);

typedef bool (*pFnBTHeadsetVoltageReading)(int nComport, long& nVoltage, int nTimeout);
typedef bool (*pFnChangeOnline)(int nComport);
typedef bool (*pFnEnableQualcommDebugEvent)(int nComport,bool bEnable);


#endif