#ifndef _QUALCOMM_DIAG_
#define _QUALCOMM_DIAG_

#pragma once

#include "../odmtma_api/SerCOMLib/SerCOMLibrary.h"
#include "../Lib/RequestUtility/DiagCommand.h"
#include <string>
#include "../../Code/CommonUtil/LogPolicy.h"
#include "..\Lib\EncodeSIMData\SimEncorderApi.h"
#include <stdlib.h>
//#include <time.h>


#define	DEFAULT_QUERY_SIMDATA_RELATED	60000

class CQualcommDiag
{
public:
	CQualcommDiag(LPCTSTR lpszModelName, int  nComPort);
	virtual ~CQualcommDiag();
private:
	CSerCOMLibrary					m_objsercom;
	bool							m_bConnected;
public:
	
	// Get MotoXPRS by spec
	bool GetMotoXPRS(char* szReceiverBuffer,int nBufferLength,int nTimeOut);
	
	// Connection and Mode Detection
	bool IsComPortAlive(long nPort);
	bool IsConnected();
	bool IsTestMode(int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool IsEMU(int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool IsDLMode();
	bool IsUnitOn();
	bool IsUnitOff();

	// Change status
	bool Reset(int nTimeOut=DEFAULT_CMD_TIME_OUT, bool bWaitForRestart = true);
	bool Restart(int nTimeOut=DEFAULT_CMD_TIME_OUT, bool bWaitForRestart = true);
	bool SetTestMode(int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool CloseConnection(int nTimeOut=DEFAULT_CMD_TIME_OUT);

	//lock status related
	bool IsSPCLocked(long& nStatus, int nTimeOut=DEFAULT_QUERY_SIMDATA_RELATED);
	bool ReadSPC(char* szLock,long& nStatus,int nTimeOut=DEFAULT_CMD_TIME_OUT); // same as IsSPCLocked
	bool UnLockSPC(char* szLock, bool& bUnlockOK, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool ProgramSPC(char* szLock, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool UnLockUnit(int nLockType, char* szLock, bool& bUnlockOK, int nTimeOut=DEFAULT_QUERY_SIMDATA_RELATED); //unlock NWSCP 
	bool UnLockPhoneCode(char* szLock, bool& bUnlockOK, int nTimeOut=DEFAULT_QUERY_SIMDATA_RELATED); //unlock master code(SSCP)
	bool IsUnitLocked(long& nStatus, int nLockType/*0 =NWSCP, 1=SSCP*/, int nTimeOut=DEFAULT_QUERY_SIMDATA_RELATED); //support NWSCP, SSCP
	bool IsNWSCPLocked(bool& bNWUnlocked, // Network Personalization
						bool& bNSUnlocked, // Network Subset Personalization
						bool& bSPUnlocked, // Service Provider Personalization
						bool& bCPUnlocked, // Corporate Personalization
						bool& bSIMUnlocked,// SIM Personalization
						int nTimeOut=DEFAULT_QUERY_SIMDATA_RELATED);
	bool QuerySimDataStatus(bool& bExist, int nTimeOut=DEFAULT_QUERY_SIMDATA_RELATED);
	bool WriteLockFileName(const std::string& strLocFileName, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool ReadLockFileName(std::string& strLocFileName, int nTimeOut=DEFAULT_CMD_TIME_OUT);

	// sim lock related
	bool ReadIMEI(char* szIMEI100, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool WriteIMEI(char* szIMEI, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool ReadBTAddr(char* szAddress100, long& nLength, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool WriteBTAddr(char* szAddress, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool GenFSN(BYTE* szFSN16);
	bool ReadFSN(BYTE* szFSN16, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool WriteFSN(BYTE* szFSN16, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool FreezeOTP(int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool IsOTPFrozen(int nTimeOut=DEFAULT_CMD_TIME_OUT);
	/*bool GenSimData(const BYTE *szImei, const BYTE *szFsn, const BYTE *szMasterCode, const BYTE *szSubsidyCode, 
					const BYTE *szLockPath, BYTE *simEncryptedDataBuf, const int simEncryptedDataBufLen);*/
	bool WriteSimData(BYTE *pSimDataBuf, int nSimDataBufLen, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	
	// Version related
	bool IsQfused(bool& bQfused, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool WriteQfused(WORD chain_select, int config_register[5],  int& nQfusedStatus, int nTimeOut = DEFAULT_CMD_TIME_OUT);
	bool ReadQfuse(unsigned int& nReg1, unsigned int& nReg2, int nTimeOut =DEFAULT_CMD_TIME_OUT );
	bool ReadFlexVersion(char* szREadValue1024, long& nLength, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool ReadSWVersion(char* szReadValue1024, long& nLength, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool GetHDVersion(int& nPhoneCodeVersion, int& nBootloaderVersion, int nTimeOut=DEFAULT_CMD_TIME_OUT); // same as isOTPFrozen command
	bool GetReleaseDate(char* szDate12, char* szTime9, int nTimeOut=DEFAULT_CMD_TIME_OUT);

	// Factory Value Query
	bool ReadTransceiverKitNumber(char* szTKN100, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool WriteTransceiverKitNumber(char* szTKN100, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool ReadTrackID(char* szTrackID, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool WriteTrackID(char* szTrackID, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool ReadFactoryInforBlock(char* szBlock, long& nLength, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool WriteFactoryInforBlock(char* szBlock, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	
	// MMI setting
	bool TurnOnOffEM(bool bOpen, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool SetRTC(int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool MasterClear(int nTimeOut=DEFAULT_CMD_TIME_OUT);
		
	// NV Setting
	bool Backup(int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool Restore(int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool GetBackupNVStatus(int& nBackupTimes, CTime& LastBackupTime, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	
	// Protocol related
	bool GetCMState(DWORD& nStatus, int nTimeOut= DEFAULT_CMD_TIME_OUT);

	/*bool CheckRF4225(int nTimeOut = DEFAULT_CMD_TIME_OUT);
	bool CheckRF4226(int nTimeOut = DEFAULT_CMD_TIME_OUT);
	bool CheckRF4794(int nTimeOut = DEFAULT_CMD_TIME_OUT);*/

public:
	bool Cmd_DIAG_CONTROL_F(int nMode, int nTimeOut = DEFAULT_CMD_TIME_OUT);
	bool ReadNVItem(const WORD& NVItem, DIAG_NVITEM_PACKET_F_type& ReadResp, int nTimeOut=DEFAULT_CMD_TIME_OUT);
	bool WriteNVItem(DIAG_NVITEM_PACKET_F_type& WriteReq, int nTimeOut =DEFAULT_CMD_TIME_OUT);

	void GetCSerCOMLibrary(CSerCOMLibrary** objsercom){*objsercom = &m_objsercom;}
	int HexBcdToDigit(BYTE *pucHexBcdBuf, int iHexBcdBufSize, char *sDigitBuf, int iDigitBufSize);
	int DigitToHexBcd(char *sDigitBuf, int iDigitBufSize,BYTE *pucHexBcdBuf, int iHexBcdBufSize);

	int  GetLastError();
	void SetLastError(int nError);

    void ReOpenComport(long nPort);

private:	
	bool Write1NVValue(WORD item, BYTE item_value, int nTimeOut =DEFAULT_CMD_TIME_OUT); // only for 1 byte
	
public:	
	void AddLog(const TSTRING& strMessage);

private:
	int m_nLastError;
	TSTRING m_strLogFile;
};

#endif //_QUALCOMM_DIAG_