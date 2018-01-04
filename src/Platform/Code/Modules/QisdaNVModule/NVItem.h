#ifndef _NVITEM_H_
#define _NVITEM_H_

#pragma once
#include "..\..\Lib\RequestUtility\InterfaceBase.h"
#include "..\..\Lib\msm\nv_items.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "..\..\CommonUtil\RefObject.h"
#include "..\..\CommonUtil\ErrorCode.h"
#include "..\..\odmtma_api\DownloadDLL\NV_ItemHolder.h"

#ifdef QISDA_NV_MODULE_EXPORTS
#define CNVItem_API extern "C" __declspec(dllexport)
#define CNVItem_CLASS_API __declspec(dllexport)
#else
#define CNVItem_API extern "C" __declspec(dllimport)
#define CNVItem_CLASS_API __declspec(dllimport)
#endif

CNVItem_API int CAD(int nPort, int nTimeout);
CNVItem_API int SCT(int nPort, int nTimeout);
CNVItem_API int MC(int nPort, int nTimeout);
CNVItem_API int WQF(int nPort, unsigned int chain, unsigned long reg[5], int nTimeout);
CNVItem_API int BTCP(int nPort, int IsOn, int nTimeout);
CNVItem_API int CSIMCE(int nPort, int& nType, int nTimeout);
CNVItem_API int RFA(int nPort, unsigned char* szFactoryBuf, int& nState, int nTimeout);
CNVItem_API int BNV(int nPort, int nTimeout);
CNVItem_API int BTHVR(int nPort, long& nVoltage, int nTimeout);

// Ref: DIAG_CONTROL_F_MODE enumeration
//	MODE_OFFLINE_A_F = 0,	//!<' Go to offline analog
//	MODE_OFFLINE_D_F = 1,	//!<' Go to offline digital 
//	MODE_RESET_F = 2,		//!<' Reset. Only exit from offline 
//	MODE_FTM_F = 3,			//!<' FTM mode
//	MODE_ONLINE_F = 4,		//!<' Go to Online 
//	MODE_LPM_F = 5,			//!<' Low Power Mode (if supported)
//	MODE_POWER_OFF_F = 6,	//!<' Power off (if supported)
//	MODE_MAX_F = 7			//!<' Last (and invalid) mode enum value
//
// Note:
//   Use MODE_OFFLINE_D_F to change to FTM mode, and set NVItem 453 to 1
//   Use MODE_ONLINE_F to change to Online mode, and set NVItem 453 to 0
class CNVItem_CLASS_API CControlModeSet: public CInterfaceBase
{
public:
	CControlModeSet(int nCOMPort);
	virtual ~CControlModeSet(void);

	bool Restart(unsigned int nSleepTime=20000);
	bool DiagControlMode(const unsigned int mode);
};

class CNVItem_CLASS_API CNVItem :	public CInterfaceBase, public CLog
{
public:
	CNVItem(int nCOMPort);
	virtual ~CNVItem(void);

	TSTRING GetBTAddr();
    TSTRING GetIMEI();
	virtual bool WriteIMEI(const string& strIMEI);

	// -----   SPC-related func.   -----
	virtual bool LockSPC();
	virtual bool IsSPCLocked(long& nStatus);
	virtual bool UnlockSPC(const char* szLock, bool& bUnlockOK);
	virtual bool ProgramSPC(const char* szLock);

	// -----   PRL   -----//preferred roaming list
	bool ReadPRL(BYTE* pPRLData, UINT& nPRLDataSize, BYTE &nStatus);
	bool WritePRL(BYTE* pPRLData, UINT& nPRLDataSize, BYTE &nStatus);

	//Backup\Restore NV
	bool BackupNVPartition(int nTimeout = 3000);
	bool RestoreNVPartition(int nTimeout = 3000);
	bool CheckNVPartition(int& nBackupTimes, CTime& LastBackupTime, int nTimeout = 3000);
	bool BackupBRT(const std::string& strOutputBRTFileName,const std::string& strInputCSVFile);
	bool LineToNVItem(const CString& strLine, std::vector<CNV_ItemHolder>& vNVItem);

	//Factory area
	bool ReadMobileID(char* szLine7,char* szAdapter3,char* szSlot3,char* szDay3,char* szMonth3,char* szYear3,char* szHour3,char* szMinute3,char* szSecond3);
	bool WriteMobileID(char* szSlot3);
	bool ReadPicassoNumber(char* szPICASSO, int& nBufferSize);
	bool WritePicassoNumber(char* szPICASSO);
	bool ReadTrackID(char* szTrackID, int& nBufferSize,int nVer);
	bool WriteTrackID(char* szTrackID,int nVer);
	bool ReadProcessInfo(char* szProcessIndex,char* szYear,char* szMonth,char* szDay,char* szLine,char* szStation);
	bool WriteProcessInfo(char* szProcessIndex);
	bool ReadFactoryHWSWMW(char* szHW5,char* szSW9,char* szMW2);
	bool WriteFactoryHWSWMW(char* szHW5,char* szSW9,char* szMW2);
	bool ReadMMIFlag(char* szTestItem,char* sznResult);
	bool WriteMMIFlag(char* szTestItem,char* sznResult);

	virtual bool ReadNVItem(const WORD& NVItem, DIAG_NVITEM_PACKET_F_type& ReadResp);//	Read NV Item from the phone
    virtual bool WriteNVItem(const WORD& NVItem, DIAG_NVITEM_PACKET_F_type& ReadResp, const BYTE *WriteData, const size_t szWriteData);	    
    bool ClearActivationData(int nTimeout = 3000);
    bool StandbyCurrentTest(int nTimeout = 3000);
    virtual bool MasterClear(int nTimeout = 3000);
    bool WriteQFuse(UINT chain, uint32 LSB, uint32 MSB, uint32 RegConf2, uint32 RegConf3, uint32 RegConf4, int nTimeout = 3000);
    bool BTHeadsetChargingPower(bool IsOn, int nTimeout = 3000);
    bool SearchBTDevice(int nTimeout = 3000);
    bool QuerySearchBTResult(int nTimeout = 3000);
    bool QueryBTPairCommand(const TSTRING& strBTAddress,int nTimeout = 3000);
    bool CheckSIMCardExistence(int &nType, int nTimeout = 3000);
    bool ReadFactoryAreaNew(unsigned char* szFactoryBuf, int& nState, int nTimeout = 3000);    
    bool BTHandsetVoltageReading(long& nVoltage, int nTimeout = 3000);
	bool WriteSIMLockFileNameToNV(const std::string& strSIMLockFileName, int nTimeout = 3000);
	bool ReadSIMLockFileNameFromNV(std::string& strSIMLockFileName, int nTimeout = 3000);

    bool OnLineMode();
    bool FTMMode();

    bool ReadQFuseState(unsigned long& reg1, unsigned long& reg2, bool& bSuccess);
    bool SetOnline();
    bool SetOffline();
    bool ResetHandset();
    bool SetFTM();
    bool PowerOff();
    bool ChangeOnline();
    bool ChangeFTM();

    bool CheckFTM(int& nFTMValue);
	virtual bool EnableDiagEvent(bool bEnable);

	bool ReadBatteryTemperature(int& nTemperature);
    bool ReadBatteryVoltage(int& nVoltage);

	bool SendRawRequest(const BYTE* pReqData, const UINT nReqDataSize, BYTE* pRespData, UINT& nRespDataSize); 
};



class CNVItem_CLASS_API CCFCDiagCommand : protected CNVItem, public CRefObject, public CLastError
{
public:
	CCFCDiagCommand(int nComPort);
	virtual ~CCFCDiagCommand();

	// Connection and Mode Detection
	//bool IsComPortAlive(long nPort);
	bool IsConnected(); // wait for Alan's function
	bool IsConnectinOpen();
	bool IsTestMode(int nTimeOut = g_nDEFAULT_CMD_TIME_OUT);
	bool IsEMU(int nTimeOut = g_nDEFAULT_CMD_TIME_OUT);
	bool IsDLMode();
	bool IsUnitOn();
	bool IsUnitOff();
	
	// Change status
	bool Reset(int nTimeOut=g_nDEFAULT_CMD_TIME_OUT, bool bWaitForRestart = true/*true=sleep 20s*/);
	bool Restart(int nTimeOut=g_nDEFAULT_CMD_TIME_OUT, bool bWaitForRestart = true);
	bool ReopenComPort(int nWaitingTime = 10000, bool bUsePNP = false, int nRetryTimes = 20, int nDelayTime = 5000);

	//If you want to use this function, set the preprocessor UseMobileFinder on NVModule and set its dependency on MobileFinder.
#ifdef UseMobileFinder
	bool Restart_UsePNP(int nRestartTimeOut , int nNVTimeOut = g_nDEFAULT_CMD_TIME_OUT);
#endif

	bool SetTestMode(int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);
	bool CloseConnection(int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);

	//SPC related
	bool IsSPCLocked(long& nStatus, int nTimeOut=g_nDEFAULT_QUERY_SIMDATA_RELATED);
	bool ReadSPC(char* szLock6,long& nStatus/*0=unlocked, 1=locked, 2=default unlocked000000*/,int nTimeOut=g_nDEFAULT_CMD_TIME_OUT); // same as IsSPCLocked
	bool UnLockSPC(char* szLock6, bool& bUnlockOK, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT); //please check IsSPCLocked before use it
	bool ProgramSPC(char* szLock6, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT); // write spc but won't lock spc
	bool LockSPC(int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);

	// sim lock related
	bool ReadIMEI(char* szIMEI100, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);
	bool WriteIMEI(char* szIMEI, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);
	bool ReadBTAddr(char* szAddress100, long& nLength, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);
	bool WriteBTAddr(char* szAddress, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);
	bool GenFSN(BYTE* szFSN16);
	bool ReadFSN(BYTE* szFSN16, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);
	bool WriteFSN(BYTE* szFSN16, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);

	/*QFuse related command must be executed in FTM*/
	bool IsQfused(bool& bQfused, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);
	bool WriteQfused(WORD chain_select, int config_register[5],  int& nQfusedStatus/*0=success, 1=Read Operation failed, 2=Write Operation Failed, 3=Operation on selected chain not supported*/, int nTimeOut = g_nDEFAULT_CMD_TIME_OUT);
	bool ReadQfuse(unsigned int& nReg1, unsigned int& nReg2, int nTimeOut =g_nDEFAULT_CMD_TIME_OUT );
	
	// Version related
	bool ReadFlexVersion(char* szReadValue1024, long& nLength, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);
	bool ReadSWVersion(char* szReadValue1024, long& nLength, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);
	bool ReadINIFileName(char* szReadName1024, long& nLength, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);

	// Factory Value Query
	bool ReadTransceiverKitNumber(char* szTKN100, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);
	bool WriteTransceiverKitNumber(char* szTKN100, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);
	bool ReadTrackID(char* szTrackID, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);
	bool WriteTrackID(char* szTrackID, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);
	bool ReadFactoryInforBlock(char* szBlock, long& nLength, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);
	bool WriteFactoryInforBlock(char* szBlock, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);
	bool GetLavernockVersion(int& nVarientVersion,int& nEVTVersion, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);

	// MMI setting
	bool TurnOnOffEM(bool bOpen, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);
	bool SetRTC(int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);
	bool GetRTC(bool& bExecuteResult,BYTE& nMonth, BYTE& nDay, WORD& nYear, BYTE& nHour, BYTE& nMin, BYTE& nSec,int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);
	bool MasterClear(int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);

	// NV Setting
	bool Backup(int nTimeOut=60000){return CNVItem::BackupNVPartition(nTimeOut);}  // backup to nv partition
	bool Restore(int nTimeOut=60000){return CNVItem::RestoreNVPartition(nTimeOut);} // restore from nv partition
	bool GetBackupNVStatus(int& nBackupTimes, CTime& LastBackupTime, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT)
	{
		return CNVItem::CheckNVPartition(nBackupTimes, LastBackupTime, nTimeOut);
	}
	bool GetRestoreNVStatus(bool& bNVRestoreSuccess, bool& bCNVRestoreSuccess, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);
	bool GetMelosRestoreNVStatus(bool& bNVRestoreSuccess, bool& bCNVRestoreSuccess, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);

	//For FactoryTemporaily usage
	bool BackupBRT(const std::string& strOutputBRTFileName,const std::string& strInputCSVFile){ return CNVItem::BackupBRT(strOutputBRTFileName, strInputCSVFile); }

public:
	bool ReadNVItem(const WORD& NVItem, DIAG_NVITEM_PACKET_F_type& ReadResp, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);
	bool WriteNVItem(DIAG_NVITEM_PACKET_F_type& WriteReq, int nTimeOut =g_nDEFAULT_CMD_TIME_OUT);
	bool Write1NVValue(WORD item, BYTE item_value, int nTimeOut =g_nDEFAULT_CMD_TIME_OUT); // only for 1 byte
	bool Cmd_DIAG_CONTROL_F(int nMode, int nTimeOut = g_nDEFAULT_CMD_TIME_OUT);
};

typedef CRefObjectPtr<CCFCDiagCommand> CCFCDiagCommandPtr;

#endif //_NVITEM_H_