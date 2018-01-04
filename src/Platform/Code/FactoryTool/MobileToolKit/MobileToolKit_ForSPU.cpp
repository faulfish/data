#include "stdafx.h"
#include "MobileToolKit_ForSPU.h"
#include <assert.h>
#include <atlenc.h>
#include "../../Modules\ODMCoachModule\ODM.h"
#include "../../Modules\QisdaEFsModule\EFS.h"
#include "../../Modules\QisdaMiscellaneousModule\Wireless.h"

extern CCriticalSection	m_LockObj;

bool TransferCharToBool(const char* szBool)
{
	CStringA strb = szBool;
	strb.MakeLower();
	bool bRes = (strb == "true") ||  (strb == "t");
	return bRes;
}

void TransferBoolToChar(bool b, char* sz)
{
	CStringA strTemp = b ? "true" : "false";
	memcpy(sz, strTemp, strTemp.GetLength()+1);
}

MOBILETOOLKIT_API bool bigger(char* sznTest,char* sznBigger)
{
	return ::atoi(sznTest) > ::atoi(sznBigger);
}

MOBILETOOLKIT_API bool smaller(char* sznTest,char* sznSmaller)
{
	return ::atoi(sznTest) < ::atoi(sznSmaller);
}

MOBILETOOLKIT_API bool ranage(char* sznTest,char* sznSmaller,char* sznBigger)
{
	return ::atoi(sznTest) >= ::atoi(sznSmaller) && ::atoi(sznTest) <= ::atoi(sznBigger);
}

MOBILETOOLKIT_API bool EnableLog_ForSPU(char* szEnableLog)
{
	return EnableLog(TransferCharToBool(szEnableLog));
}

MOBILETOOLKIT_API bool ChangeLogFileName(char* szCOMPort,char* szNewFileName)
{
	TCHAR szFolderOld[MAX_PATH] = {0};
	Win32Utility::GetShellFolder(szFolderOld,CSIDL_PERSONAL,_T("Log"));
	TCHAR szFolderNew[MAX_PATH] = {0};
	Win32Utility::GetShellFolder(szFolderNew,CSIDL_PERSONAL,_T("Log"));

	CString strFileName;
	strFileName.Format(_T("Test_COM%02d.txt"),::atoi(szCOMPort));

	PathAppend(szFolderOld,strFileName);
	PathAppend(szFolderNew,szNewFileName);
	if(_access(szFolderOld,0)  != -1)
	{
		if(_access(szFolderNew,0)  == -1)
			::rename(szFolderOld,szFolderNew);
		else
		{
			CFile logFile;
			CFileException ex;
			if (logFile.Open(szFolderNew, CFile::modeWrite |CFile::shareExclusive | CFile::modeCreate | CFile::modeNoTruncate , &ex))
			{
				ULONGLONG dwLength = logFile.SeekToEnd();

				CFile oldlogFile;
				CFileException oldex;
				if (oldlogFile.Open(szFolderOld, CFile::modeRead |CFile::shareExclusive, &oldex))
				{
					char szBuffer[1024] = {0};
					UINT nBytesRead = oldlogFile.Read(szBuffer,1024);

					while(nBytesRead > 0)
					{
						logFile.Write(szBuffer,nBytesRead);
						nBytesRead = oldlogFile.Read(szBuffer,1024);
					};

					oldlogFile.Close();
					::remove(szFolderOld);
				}

				logFile.Close();
			}
		}
	}
	else
	{//Nothing
	}
	return true;
}

MOBILETOOLKIT_API bool BTHFPGateway(char* szPort, char* szbEnable)
{
	CWireless wireless(::atoi(szPort));
	return wireless.BTHFPGateway(TransferCharToBool(szbEnable));
}

MOBILETOOLKIT_API bool BTHFPGatewayConnect(char* szPort, char* szbEnable, char* szBTAddr)
{
	CWireless wireless(::atoi(szPort));
	return wireless.BTHFPGatewayConnect(TransferCharToBool(szbEnable),szBTAddr);
}

MOBILETOOLKIT_API bool BTHFPGatewayAnswer(char* szPort, char* szbEnable)
{
	CWireless wireless(::atoi(szPort));
	return wireless.BTHFPGatewayAnswer(TransferCharToBool(szbEnable));
}

MOBILETOOLKIT_API bool BTHFPGatewayAudioConnect(char* szPort, char* szbEnable)
{
	CWireless wireless(::atoi(szPort));
	return wireless.BTHFPGatewayAudioConnect(TransferCharToBool(szbEnable));
}

MOBILETOOLKIT_API bool BTHFPGatewaySpeakerGain(char* szPort)
{
	CWireless wireless(::atoi(szPort));
	return wireless.BTHFPGatewaySpeakerGain();
}

MOBILETOOLKIT_API bool BTHFPGatewayMicrophoneGain(char* szPort)
{
	CWireless wireless(::atoi(szPort));
	return wireless.BTHFPGatewayMicrophoneGain();
}

MOBILETOOLKIT_API bool BTHFPGatewayAudioLoop(char* szPort, char* szbEnable)
{
	CWireless wireless(::atoi(szPort));
	return wireless.BTHFPGatewayAudioLoop(TransferCharToBool(szbEnable));
}

MOBILETOOLKIT_API bool BTHFPGatewayRoaming(char* szPort, char* szbEnable)
{
	CWireless wireless(::atoi(szPort));
	return wireless.BTHFPGatewayRoaming(TransferCharToBool(szbEnable));
}

MOBILETOOLKIT_API bool WriteBTAddr_ForSPU(char* szPort,char* inBTAddrBuffer)
{
	CCFCDiagCommand nv(atoi(szPort));
	bool bRes = nv.WriteBTAddr(inBTAddrBuffer);
	return bRes;
}

MOBILETOOLKIT_API bool CheckRTCWithCurrentTime(char* szPort)
{
	bool bRes = false;

	bool bExecuteResult = false;
	BYTE nMonth = -1;
	BYTE nDay = -1;
	WORD nYear = -1;
	BYTE nHour = -1;
	BYTE nMin = -1;
	BYTE nSec = -1;
	CCFCDiagCommand nv(atoi(szPort));	
	if(nv.GetRTC(bExecuteResult,nMonth,nDay,nYear,nHour,nMin,nSec) && bExecuteResult)
	{
		COleDateTime CurrentTime(COleDateTime::GetCurrentTime());
		COleDateTime time;
		if(QSYNC_LIB::ConvertTime(CurrentTime, time, false))
		{
			BYTE nMonth = time.GetMonth();
			BYTE nDay = time.GetDay();
			WORD nYear = time.GetYear();
			BYTE nHour = time.GetHour();
			BYTE nMin = time.GetMinute();
			BYTE nSec = time.GetSecond();
		}

		bRes = (nMonth == time.GetMonth()) && (nDay == time.GetDay()) && (nYear == time.GetYear());
		if(bRes)
			bRes = (nHour == time.GetHour()) && (nMin == time.GetMinute());
		if(bRes)
		{
			BYTE nSec2 = time.GetSecond();
			bRes = (nSec == time.GetSecond()) || (nSec == (time.GetSecond()+1)) || (nSec == (time.GetSecond() -1));
		}
	}

	return bRes;
}

MOBILETOOLKIT_API bool SetRTC(char* szPort)
{
	CCFCDiagCommand nv(atoi(szPort));
	bool bRes = nv.SetRTC();
	return bRes;
}

MOBILETOOLKIT_API bool GetRTC(char* szPort,char* sziMonth, char* sziDay, char* sziYear, char* sziHour, char* sziMinute,char* sziSecond)
{
	bool bExecuteResult = false;
	BYTE nMonth = -1;
	BYTE nDay = -1;
	WORD nYear = -1;
	BYTE nHour = -1;
	BYTE nMin = -1;
	BYTE nSec = -1;
	CCFCDiagCommand nv(atoi(szPort));
	bool bRes = nv.GetRTC(bExecuteResult,nMonth,nDay,nYear,nHour,nMin,nSec);
	if(bRes)
	{
		sprintf_s(sziMonth,1024,"%02d",nMonth);
		sprintf_s(sziDay,1024,"%02d",nDay);
		sprintf_s(sziYear,1024,"%02d",nYear);
		sprintf_s(sziHour,1024,"%02d",nHour);
		sprintf_s(sziMinute,1024,"%02d",nMin);
		sprintf_s(sziSecond,1024,"%02d",nSec);
	}
	return bRes;
}

MOBILETOOLKIT_API bool GetBTAddr_ForSPU(char* szPort, char* outBTAddrBuffer, char* szBufferSize)
{
	bool bRes = false;
	int nBufferSize = atoi(szBufferSize);
	if (nBufferSize <= 1024)
	{
		memset(outBTAddrBuffer, 0, nBufferSize);
		bRes = GetBTAddr(atoi(szPort), outBTAddrBuffer, nBufferSize);
	}
	else
		assert(0);
	return bRes;
}

MOBILETOOLKIT_API bool BTBound(char* szPort, const char* szbBound,const char* szBTAddr, const char* szBTPass,const char* szTimeout)
{
	CWireless wireless(::atoi(szPort));
	return wireless.BTBound(TransferCharToBool(szbBound),szBTAddr,szBTPass,::atoi(szTimeout));
}

MOBILETOOLKIT_API bool WriteHeadsetBTAddress_ForSPU(char* szPort, char* szBTAddr)
{
	return WriteHeadsetBTAddress(szBTAddr, atoi(szPort));
}

MOBILETOOLKIT_API bool CheckBTHeadsetState_ForSPU(char *szPort, char* szBTAddr)
{
	return CheckBTHeadsetState(szBTAddr, atoi(szPort));
}

MOBILETOOLKIT_API bool BTHeadsetChargingPower_ForSPU(char* szPort, char* szIsOn)
{
	return BTHeadsetChargingPower(atoi(szPort), TransferCharToBool(szIsOn));
}

MOBILETOOLKIT_API bool EnableGAP_ForSPU(char* szPort, char* szbEnable)
{
	return EnableGAP(atoi(szPort), TransferCharToBool(szbEnable));
}

MOBILETOOLKIT_API bool StartBTPower_ForSPU(char* szPort, char* szDegree)
{
	return StartBTPower(atoi(szPort), atoi(szDegree));
}

MOBILETOOLKIT_API bool EnterBTTestMode_ForSPU(char* szPort, char* szbEnable)
{
	return EnterBTTestMode(atoi(szPort), TransferCharToBool(szbEnable));
}

MOBILETOOLKIT_API bool SearchBTDevice_ForSPU(char* szPort)
{
	return SearchBTDevice(atoi(szPort));
}

MOBILETOOLKIT_API bool GetIMEI_ForSPU(char* szPort, char* outIMEI)
{
	memset(outIMEI, 0, 128);
	return GetIMEI(atoi(szPort), outIMEI);
}

MOBILETOOLKIT_API bool WriteIMEI_ForSPU(char* nPort, char* inIMEI)
{
	return WriteIMEI(atoi(nPort), inIMEI);
}

MOBILETOOLKIT_API bool LockSPC_ForSPU(char* szPort)
{
	return LockSPC(atoi(szPort));
}

MOBILETOOLKIT_API bool IsSPCLocked_ForSPU(char* szPort, char* szStatus)
{
	long nStatus = 0;
	bool bRes = IsSPCLocked(atoi(szPort), nStatus);
	if(bRes)
	{
		CStringA strStatus = (nStatus == 0 ?  "SPC not locked" : "SPC Locked");
		memcpy(szStatus, strStatus, strStatus.GetLength()+1);
	}
	return bRes;
}

MOBILETOOLKIT_API bool UnlockSPC_ForSPU(char* szPort, char* szLock, char* szUnlockOK)
{
	bool bUnlockOK = false;
	bool bRes = UnlockSPC(atoi(szPort), szLock, bUnlockOK);
	if (bRes)
	{
		CStringA strStatus = (bUnlockOK ? "Unlock SPC succeed." : "Unlock SPC failed.");
		memcpy(szUnlockOK, strStatus, strStatus.GetLength()+1);
	}
	return bRes;
}

MOBILETOOLKIT_API bool ProgramSPC_ForSPU(char* szPort, char* szLock)
{
	return ProgramSPC(atoi(szPort), szLock);
}

MOBILETOOLKIT_API bool ClearActivationData_ForSPU(char* szPort)
{
	return ClearActivationData(atoi(szPort));
}

MOBILETOOLKIT_API bool StandbyCurrentTest_ForSPU(char* szPort)
{
	return StandbyCurrentTest(atoi(szPort));
}

MOBILETOOLKIT_API bool MasterClear_ForSPU(char* szPort)
{
	return MasterClear(atoi(szPort));
}

MOBILETOOLKIT_API bool CheckSIMCardExistence_ForSPU(char* szPort, char* szType)
{
	int nType = 0;
	bool bRes = CheckSIMCardExistence(atoi(szPort), nType);
	if (bRes)
	{
		CStringA strType;
		strType.Format("%d", nType);
		memcpy(szType, strType, strType.GetLength()+1);
	}
	return bRes;
}

//Note: szFactoryArea128 output is in Base64 format, user needs to decode it.
MOBILETOOLKIT_API bool ReadFactoryAreaNew_ForSPU(char* szPort, char* szNVItem, char* szFactoryArea128_Base64, char* szState, char* szTimeOut)
{
	int nState = 0;
	WORD nNVItem = (WORD)atoi(szNVItem);
	BYTE  szFactoryArea128[128] = {0};
	bool bRes = ReadFactoryAreaNew(atoi(szPort), nNVItem, (unsigned char*)szFactoryArea128, nState, atoi(szTimeOut));
	if (bRes)
	{
		CStringA strState;
		strState.Format("%d", nState);
		memcpy(szState, strState, strState.GetLength()+1);

		int nDataLen = Base64EncodeGetRequiredLength(128);
		if(nDataLen > 1024)
			bRes = false;
		else
		{
			memset(szFactoryArea128_Base64, 0, nDataLen);
			bRes = Base64Encode(szFactoryArea128, 128, szFactoryArea128_Base64, &nDataLen) == TRUE;
		}
	}
	return bRes;
}

MOBILETOOLKIT_API bool BTHeadsetVoltageReading_ForSPU(char* szPort, char* szVoltage, char* szTimeout)
{
	long nVoltage = 0;
	bool bRes = BTHeadsetVoltageReading(atoi(szPort), nVoltage, atoi(szTimeout));
	if (bRes)
	{
		CStringA strVoltage;
		strVoltage.Format("%d", nVoltage);
		memcpy(szVoltage, strVoltage, strVoltage.GetLength()+1);
	}
	return bRes;
}

MOBILETOOLKIT_API bool EnableQualcommDebugEvent_ForSPU(char* szPort, char* szbEnable)
{
	return EnableQualcommDebugEvent(atoi(szPort), TransferCharToBool(szbEnable));
}

//Note: szSimDataBuf input is in Base64 format, this function will decode it.
MOBILETOOLKIT_API bool WriteSimData_ForSPU(char* szPort, char* szSimDataBuf_Base64, char* szSimDataBufLen)
{
	bool bRes = false;
	int nOgnDataBufLen = atoi(szSimDataBufLen);
	int nDecLen = Base64DecodeGetRequiredLength(nOgnDataBufLen);
	if (nDecLen <= 1024)
	{
		BYTE* SimDataBuf = new BYTE[nDecLen];
		memset(SimDataBuf, 0, nDecLen);

		bRes = Base64Decode(szSimDataBuf_Base64, nOgnDataBufLen, SimDataBuf, &nDecLen) == TRUE;
		if (bRes)
			bRes = WriteSimData(atoi(szPort), SimDataBuf, nDecLen);

		delete [] SimDataBuf;
	}
	return bRes;
}

//Note: szSimDataBuf input is in Base64 format, this function will decode it.
MOBILETOOLKIT_API bool WriteSimDataV2_ForSPU(char* szPort, char* szSimDataBuf_Base64, char* szSimDataBufLen)
{
	bool bRes = false;
	//int nOgnDataBufLen = atoi(szSimDataBufLen);
	size_t nOgnDataBufLen = strlen(szSimDataBuf_Base64);
	int nDecLen = Base64DecodeGetRequiredLength((int)nOgnDataBufLen);
	BYTE* SimDataBuf = new BYTE[nDecLen];
	memset(SimDataBuf, 0, nDecLen);
	bRes = Base64Decode(szSimDataBuf_Base64, (int)nOgnDataBufLen, SimDataBuf, &nDecLen) && WriteSimDataV2(atoi(szPort), SimDataBuf, nDecLen);
	delete [] SimDataBuf;
	return bRes;
}

MOBILETOOLKIT_API bool WriteSimLockFile_ForSPU(char* szPort, char* szSIMLockFileName)
{
	std::string strSIMLockFileName = szSIMLockFileName;
	return WriteSimLockFile(atoi(szPort), strSIMLockFileName);
}

MOBILETOOLKIT_API bool BackupNV2BRT_ForSPU(char* szPort, char* szOutputBRTFileName, char* szInputCSVFile)
{
	return BackupNV2BRT(atoi(szPort), szOutputBRTFileName, szInputCSVFile);
}

MOBILETOOLKIT_API bool RestoreBRT2NV_ForSPU(char* szPort, char* szInBRTFileName)
{
	return RestoreBRT2NV(atoi(szPort), szInBRTFileName);
}

MOBILETOOLKIT_API bool BackupNVPartition_ForSPU(char* szPort)
{
	return BackupNVPartition(atoi(szPort));
}

MOBILETOOLKIT_API bool RestoreNVPartition_ForSPU(char* szPort)
{
	return RestoreNVPartition(atoi(szPort));
}

MOBILETOOLKIT_API bool CheckNVPartition_ForSPU(char* szPort, char* szBackupTimes, char* szLastBackupTime)
{
	bool bRes = false;
	int nBackupTimes  =0;
	CTime LastBackupTime;
	bRes = CheckNVPartition(atoi(szPort), nBackupTimes, LastBackupTime);
	if (bRes)
	{
		CStringA strBackupTimes;
		strBackupTimes.Format("%d", nBackupTimes);
		memcpy(szBackupTimes, strBackupTimes, strBackupTimes.GetLength()+1);

		CStringA strLastBackupTime;
		strLastBackupTime.Format("%d %d/%d %d:%d:%d", LastBackupTime.GetYear(), LastBackupTime.GetMonth(), LastBackupTime.GetDay(), LastBackupTime.GetHour(), LastBackupTime.GetMinute(), LastBackupTime.GetSecond());
		memcpy(szLastBackupTime, strLastBackupTime, strLastBackupTime.GetLength()+1);
	}
	return bRes;
}

//Note: szNVItem input is in Base64 format, this function will decode it.
MOBILETOOLKIT_API bool ReadNVItem_ForSPU(char* szPort, char* szNVItem, char* szitem_data_base64, char* szNV_stat)
{
	WORD nv_stat = 0;
	BYTE  item_data[128] = {0};
	bool bRes = ReadNVItem(atoi(szPort), atoi(szNVItem), item_data, nv_stat);
	if (bRes)
	{
		int nDataLen = Base64EncodeGetRequiredLength(128);
		if(nDataLen > 1024)
			bRes = false;
		else
		{
			memset(szitem_data_base64, 0, nDataLen);
			bRes = Base64Encode(item_data, 128, szitem_data_base64, &nDataLen) == TRUE;
		}
	}

	if(bRes)
	{
		CStringA strNVState;
		strNVState.Format("%d", nv_stat);
		memcpy(szNV_stat, strNVState, strNVState.GetLength()+1);
	}
	return bRes;
}

MOBILETOOLKIT_API bool WriteNVItem_ForSPU(char* szPort, char* sznNVItem, char* szWriteData_Base64, char* sznDataLen,char* szNV_stat)
{
	bool bRes = false;
	int nDecLen = Base64DecodeGetRequiredLength((int)strlen(szWriteData_Base64));
	BYTE* DataBuf = new BYTE[nDecLen];
	memset(DataBuf, 0, nDecLen);
	WORD nv_stat = 0;
	bRes = Base64Decode(szWriteData_Base64, (int)strlen(szWriteData_Base64), DataBuf, &nDecLen) && WriteNVItem(atoi(szPort), atoi(sznNVItem), DataBuf, nDecLen, nv_stat);

	if (bRes)
	{
		CStringA strNVState;
		strNVState.Format("%d", nv_stat);
		memcpy(szNV_stat, strNVState, strNVState.GetLength()+1);
	}
	return bRes;
}

//Audio
MOBILETOOLKIT_API bool PlayMP3(char* szPort,char* szbEnable)
{
	CWireless wireless(::atoi(szPort));
	return wireless.PlayMP3(TransferCharToBool(szbEnable));
}
MOBILETOOLKIT_API bool SpeakerDevice(char* szPort,char* szbEnable)
{
	CWireless wireless(::atoi(szPort));
	return wireless.SpeakerDevice(TransferCharToBool(szbEnable));
}
MOBILETOOLKIT_API bool SpeakerDeviceMultimediaLevel(char* szPort,char* sznLevel)
{
	CWireless wireless(::atoi(szPort));
	return wireless.SpeakerDeviceMultimediaLevel(::atoi(sznLevel));
}
MOBILETOOLKIT_API bool SpeakerDeviceVoiceLevel(char* szPort,char* sznLevel)
{
	CWireless wireless(::atoi(szPort));
	return wireless.SpeakerDeviceVoiceLevel(::atoi(sznLevel));
}
MOBILETOOLKIT_API bool HandsetDevice(char* szPort,char* szbEnable)
{
	CWireless wireless(::atoi(szPort));
	return wireless.HandsetDevice(TransferCharToBool(szbEnable));
}
MOBILETOOLKIT_API bool HandsetDeviceMultimediaLevel(char* szPort,char* sznLevel)
{
	CWireless wireless(::atoi(szPort));
	return wireless.HandsetDeviceMultimediaLevel(::atoi(sznLevel));
}
MOBILETOOLKIT_API bool HandsetDeviceVoiceLevel(char* szPort,char* sznLevel)
{
	CWireless wireless(::atoi(szPort));
	return wireless.HandsetDeviceVoiceLevel(::atoi(sznLevel));
}
MOBILETOOLKIT_API bool MonoHeadsetDevice(char* szPort,char* szbEnable)
{
	CWireless wireless(::atoi(szPort));
	return wireless.MonoHeadsetDevice(TransferCharToBool(szbEnable));
}
MOBILETOOLKIT_API bool MonoHeadsetDeviceMultimediaLevel(char* szPort,char* sznLevel)
{
	CWireless wireless(::atoi(szPort));
	return wireless.MonoHeadsetDeviceMultimediaLevel(::atoi(sznLevel));
}
MOBILETOOLKIT_API bool MonoHeadsetDeviceVoiceLevel(char* szPort,char* sznLevel)
{
	CWireless wireless(::atoi(szPort));
	return wireless.MonoHeadsetDeviceVoiceLevel(::atoi(sznLevel));
}
MOBILETOOLKIT_API bool StereoHeadsetDevice(char* szPort,char* szbEnable)
{
	CWireless wireless(::atoi(szPort));
	return wireless.StereoHeadsetDevice(TransferCharToBool(szbEnable));
}
MOBILETOOLKIT_API bool StereoHeadsetDeviceMultimediaLevel(char* szPort,char* sznLevel)
{
	CWireless wireless(::atoi(szPort));
	return wireless.StereoHeadsetDeviceMultimediaLevel(::atoi(sznLevel));
}
MOBILETOOLKIT_API bool StereoHeadsetDeviceVoiceLevel(char* szPort,char* sznLevel)
{
	CWireless wireless(::atoi(szPort));
	return wireless.StereoHeadsetDeviceVoiceLevel(::atoi(sznLevel));
}
MOBILETOOLKIT_API bool BluetoothDevice(char* szPort,char* szbEnable)
{
	CWireless wireless(::atoi(szPort));
	return wireless.BluetoothDevice(TransferCharToBool(szbEnable));
}
MOBILETOOLKIT_API bool BluetoothDeviceMultimediaLevel(char* szPort,char* sznLevel)
{
	CWireless wireless(::atoi(szPort));
	return wireless.BluetoothDeviceMultimediaLevel(::atoi(sznLevel));
}
MOBILETOOLKIT_API bool BluetoothDeviceVoiceLevel(char* szPort,char* sznLevel)
{
	CWireless wireless(::atoi(szPort));
	return wireless.BluetoothDeviceVoiceLevel(::atoi(sznLevel));
}
MOBILETOOLKIT_API bool AudioPackageLoopback(char* szPort,char* szbEnable)
{
	CWireless wireless(::atoi(szPort));
	return wireless.AudioPackageLoopback(TransferCharToBool(szbEnable));
}
MOBILETOOLKIT_API bool AlarmTone(char* szPort,char* sznTone)
{
	CWireless wireless(::atoi(szPort));
	return wireless.AlarmTone(::atoi(sznTone));
}

MOBILETOOLKIT_API bool FMRegistration_ForSPU(char* szPort)
{
	return FMRegistration(atoi(szPort));
}

MOBILETOOLKIT_API bool FMConnect_ForSPU(char* szPort, char* szbConnect)
{
	return FMConnect(atoi(szPort), TransferCharToBool(szbConnect));
}

MOBILETOOLKIT_API bool FMInitialize_ForSPU(char* szPort)
{
	return FMInitialize(atoi(szPort));
}

MOBILETOOLKIT_API bool FMSetStation_ForSPU(char* szPort, char* szFMFrequence)
{
	return FMSetStation(atoi(szPort), atoi(szFMFrequence));
}

MOBILETOOLKIT_API bool FMMute_ForSPU(char* szPort, char* szbMute)
{
	return FMMute(atoi(szPort), TransferCharToBool(szbMute));
}

MOBILETOOLKIT_API bool FMChangeTestModeAndSetLevel(char* szPort, char* sznLevel)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CWireless wireless(::atoi(szPort));
	return wireless.FMChangeTestModeAndSetLevel(::atoi(sznLevel));
}

MOBILETOOLKIT_API bool FMReceiveRSSI_ForSPU(char* szPort, char* szRSSI)
{
	WORD nRSSI = 0;
	bool bRes = FMReceiveRSSI(atoi(szPort), nRSSI);
	if(bRes)
	{
		CStringA strRSSI;
		strRSSI.Format("%d", nRSSI);
		memcpy(szRSSI, strRSSI, strRSSI.GetLength()+1);
	}
	return bRes;
}

MOBILETOOLKIT_API bool FMSpeaker_ForSPU(char* szPort)
{
	return FMSpeaker(atoi(szPort));
}

MOBILETOOLKIT_API bool FMMonoHeadset_ForSPU(char* szPort)
{
	return FMMonoHeadset(atoi(szPort));
}

MOBILETOOLKIT_API bool FMStereoHeadset_ForSPU(char* szPort)
{
	return FMStereoHeadset(atoi(szPort));
}

MOBILETOOLKIT_API bool WriteSIMLockFileNameToNV_ForSPU(char* szPort, char* szSIMLockFileName)
{
	return WriteSIMLockFileNameToNV(atoi(szPort), szSIMLockFileName);
}

MOBILETOOLKIT_API bool ReadSIMLockFileNameFromNV_ForSPU(char* szPort, char* szSIMLockFileName)
{
	std::string strSIMLockFileName;
	bool bRes = ReadSIMLockFileNameFromNV(atoi(szPort), strSIMLockFileName);
	if (bRes)
		memcpy(szSIMLockFileName, strSIMLockFileName.c_str(), strSIMLockFileName.length()+1);
	return bRes;
}

MOBILETOOLKIT_API bool GetSWHWVersion_ForSPU(char* szPort, char* sSW, char* sHW)
{
	return GetSWHWVersion(atoi(szPort), sSW, sHW);
}

MOBILETOOLKIT_API bool GetLavernockHWVersion_ForSPU(char* szPort, char* sznVersion)
{
	int nVersion = 0;
	bool bRes = GetLavernockHWVersion(atoi(szPort), nVersion);
	if (bRes)
	{
		CStringA strTemp;
		strTemp.Format("%d", nVersion);
		memcpy(sznVersion, strTemp, strTemp.GetLength()+1);
	}
	return bRes;
}

MOBILETOOLKIT_API bool GetLavernockHWVersionV2_ForSPU(char* szPort, char* sznVarientVersion, char* sznEVTVersion)
{
	int nVarientVersion = 0;
	int nEVTVersion = 0;	

	CODM odm(atoi(szPort));
	bool bRes =  odm.GetLavernockHWVersionV2(nVarientVersion,nEVTVersion);
	if (bRes)
	{
		CStringA strTemp;
		strTemp.Format("%d", nVarientVersion);
		memcpy(sznVarientVersion, strTemp, strTemp.GetLength()+1);
		
		strTemp.Format("%d", nEVTVersion);
		memcpy(sznEVTVersion, strTemp, strTemp.GetLength()+1);
	}
	return bRes;
}

MOBILETOOLKIT_API bool WriteQFuse_ForSPU(char* szPort, char* szChain, char* szLSB, char* szMSB, char* szRegConf2, char* szRegConf3, char* szRegConf4)
{
	unsigned long int Config_Register[5] = {atoi(szLSB), atoi(szMSB), atoi(szRegConf2), atoi(szRegConf3), atoi(szRegConf4)};
	return WriteQFuse(atoi(szPort), atoi(szChain), Config_Register);
}

MOBILETOOLKIT_API bool ReadQFuseState_ForSPU(char* szPort, char* szReg1, char* szReg2, char* szbSuccess)
{
	unsigned long reg1(0), reg2(0);
	bool bSuccess = false;
	bool bRes = ReadQFuseState(atoi(szPort), reg1, reg2, bSuccess);
	if(bRes)
	{
		CStringA strTemp;
		strTemp.Format("%d", reg1);
		memcpy(szReg1, strTemp, strTemp.GetLength()+1);
		strTemp.Format("%d", reg2);
		memcpy(szReg2, strTemp, strTemp.GetLength()+1);
		
		TransferBoolToChar(bSuccess, szbSuccess);
	}
	return bRes;
}

MOBILETOOLKIT_API bool ChangeMode_ForSPU(char* szPort, char* sznMode)
{
	return ChangeMode(atoi(szPort), atoi(sznMode));
}

MOBILETOOLKIT_API bool FTMMode_ForSPU(char* szPort)
{
	return FTMMode(atoi(szPort));
}

MOBILETOOLKIT_API bool ChangeFTM_ForSPU(char* szPort) //Write NV item , must be reset
{
	return ChangeFTM(atoi(szPort));
}

MOBILETOOLKIT_API bool CheckFTM_ForSPU(char* szPort, char* sznFTMValue)
{
	int nFTMValue  = -2;
	bool bRes = CheckFTM(atoi(szPort), nFTMValue);
	if (bRes)
	{
		CStringA strTemp;
		strTemp.Format("%d", nFTMValue);
		memcpy(sznFTMValue, strTemp, strTemp.GetLength()+1);
	}
	return bRes;
}

MOBILETOOLKIT_API bool OnLineMode_ForSPU(char* szPort)
{
	return OnLineMode(atoi(szPort));
}

MOBILETOOLKIT_API bool SetOnline_ForSPU(char* szPort)
{
	return SetOnline(atoi(szPort));
}

MOBILETOOLKIT_API bool SetOffline_ForSPU(char* szPort)
{
	return SetOffline(atoi(szPort));
}

MOBILETOOLKIT_API bool ChangeOnline_ForSPU(char* szPort)
{
	return ChangeOnline(atoi(szPort));
}

MOBILETOOLKIT_API bool PowerOff_ForSPU(char* szPort)
{
	return PowerOff(atoi(szPort));
}

//EM mode
MOBILETOOLKIT_API bool TurnOnOffEM_ForSPU(char* szPort, char* szbOpenEM)
{
	return TurnOnOffEM(atoi(szPort), TransferCharToBool(szbOpenEM));
}

MOBILETOOLKIT_API bool ResetHandset_ForSPU(char* szPort)
{
	return ResetHandset(atoi(szPort));
}

MOBILETOOLKIT_API bool WriteFailureByte_ForSPU(char* szPort, char* sznFailureByte)
{
	return WriteFailureByte(atoi(szPort), atoi(sznFailureByte));
}

MOBILETOOLKIT_API bool ReadBatteryVoltage_ForSPU(char* szPort, char* sznVoltage)
{
	int nVoltage  = 0;
	bool bRes = ReadBatteryVoltage(atoi(szPort), nVoltage);
	if (bRes)
	{
		CStringA strTemp;
		strTemp.Format("%d", nVoltage);
		memcpy(sznVoltage, strTemp, strTemp.GetLength()+1);
	}
	return bRes;
}
MOBILETOOLKIT_API bool ReadBatteryTemperature_ForSPU(char* szPort, char* sznTemperature)
{
	int nTemperature  = 0;
	bool bRes = ReadBatteryTemperature(atoi(szPort), nTemperature);
	if (bRes)
	{
		CStringA strTemp;
		strTemp.Format("%d", nTemperature);
		memcpy(sznTemperature, strTemp, strTemp.GetLength()+1);
	}
	return bRes;
}

MOBILETOOLKIT_API bool WriteDefaultPhoneCode_ForSPU(char* szPort)
{
	return WriteDefaultPhoneCode(atoi(szPort));
}

MOBILETOOLKIT_API bool FreezeOTP_ForSPU(char* szPort)
{
	return FreezeOTP(atoi(szPort));
}

MOBILETOOLKIT_API bool IsOTPFrozen_ForSPU(char* szPort, char* szbFrozen,char* sznLastError)
{
	int nLastError = 0;
	bool bIsFrozen = false;
	bool bRes = IsOTPFrozen(atoi(szPort), bIsFrozen, nLastError);
	TransferBoolToChar(bIsFrozen, szbFrozen);

	CStringA strTemp;
	strTemp.Format("%d", nLastError);
	memcpy(sznLastError, strTemp, strTemp.GetLength()+1);
	return bRes;
}

MOBILETOOLKIT_API bool ReplaceEFSFile_ForSPU(char* szPort, const std::string& strEFSFilePath, const std::string& strNewLocalFilePath)
{
	return ReplaceEFSFile(atoi(szPort), strEFSFilePath, strNewLocalFilePath);
}

MOBILETOOLKIT_API bool Vibrator(char* szPort,char* szEnable)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	bool bRes = false;
	CODM odm(::atoi(szPort));
	odm.Vibrator(TransferCharToBool(szEnable),bRes);
	return bRes;
}

#pragma warning(push)
#pragma warning(disable:4996)
MOBILETOOLKIT_API bool GSensorCalibration(char* szPort,char* sznResult)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	bool bRes = false;
	CODM odm(::atoi(szPort));
	int nRet = -1;
	odm.GSensorCalibration(bRes,nRet);
	sprintf(sznResult,"%0d",nRet);
	return bRes;
}
#pragma warning(pop)

MOBILETOOLKIT_API bool IsGSensorWorking_ForSPU(char* szPort)
{
	return IsGSensorWorking(atoi(szPort));
}

MOBILETOOLKIT_API bool IsGSensorWorkingV2_ForSPU(char* szPort,char* szbIsGSensorWork, char* szbIsUpWork)
{
	bool bIsGSensorWork = false;
	bool bIsUpWork = false;
	bool bRes = IsGSensorWorkingV2(atoi(szPort), bIsGSensorWork, bIsUpWork);
	TransferBoolToChar(bIsGSensorWork, szbIsGSensorWork);
	TransferBoolToChar(bIsUpWork, szbIsUpWork);
	return bRes;
}

MOBILETOOLKIT_API bool IsGSensorWorkingV3_ForSPU(char* szPort, char* sznX, char* sznY, char* sznZ)
{
	int nX = 0;
	int nY = 0;
	int nZ = 0;
	bool bRes = IsGSensorWorkingV3(atoi(szPort), nX, nY, nZ);
	CStringA strTemp;
	strTemp.Format("%d", nX);
	memcpy(sznX, strTemp, strTemp.GetLength()+1);
	strTemp.Format("%d", nY);
	memcpy(sznY, strTemp, strTemp.GetLength()+1);
	strTemp.Format("%d", nZ);
	memcpy(sznZ, strTemp, strTemp.GetLength()+1);
	return bRes;
}

MOBILETOOLKIT_API bool GSensorOffset_ForSPU(char* szPort, char* sznOffsetX, char* sznOffsetY, char* sznOffsetZ)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	bool bRes = false;
	CODM odm(::atoi(szPort));
	int nOffsetX = -1;
	int nOffsetY = -1;
	int nOffsetZ = -1;
	odm.GSensorOffset(bRes,nOffsetX,nOffsetY,nOffsetZ);
	sprintf_s(sznOffsetX,1024,"%0d",nOffsetX);
	sprintf_s(sznOffsetY,1024,"%0d",nOffsetY);
	sprintf_s(sznOffsetZ,1024,"%0d",nOffsetZ);
	return bRes;
}

MOBILETOOLKIT_API bool CheckSIMLock_ForSPU(char* szPort, char* szSIMLockName)
{
	std::string strSIMLockName;
	bool bRes = CheckSIMLock(atoi(szPort), strSIMLockName);
	if(bRes && strSIMLockName.length() <= 1024)
		memcpy(szSIMLockName, strSIMLockName.c_str(), strSIMLockName.length()+1);
	else
		bRes = false;

	return bRes;
}

MOBILETOOLKIT_API bool CheckCEFSFileName_ForSPU(char* szPort, char* szCEFSFileNameBuffer, char* sznBufferSize)
{
	return CheckCEFSFileName(atoi(szPort), szCEFSFileNameBuffer, atoi(sznBufferSize));
}

MOBILETOOLKIT_API bool GetMicroSDInfo_ForSPU(char* szPort, char* sznBlockSize, char* sznTotalBlock)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	int nFilesystemType;
	int nBlockSize;
	int nTotalBlockCount;
	int nFreeBlockCount;
	int nAvailableBlockCount;
	int nTotalFileNumber;
	int nFreeFileNumber;
	int nFileSystemID;
	int nMaximumFilename;
	BYTE bStatus;

	CEFs efs(::_ttoi(szPort));	
	bool bRes = efs.GetMicroSDInfo(nFilesystemType,nBlockSize,nTotalBlockCount,nFreeBlockCount
		,nAvailableBlockCount,nTotalFileNumber,nFreeFileNumber,nFileSystemID	,nMaximumFilename,bStatus);
	if(bRes)
	{
#pragma warning(push)
#pragma warning(disable:4996)
		::sprintf(sznBlockSize,"%0d",nBlockSize);
		::sprintf(sznTotalBlock,"%0d",nTotalBlockCount);
#pragma warning(pop)
	}
	return bRes;
}

MOBILETOOLKIT_API bool DLRAWRequest_ForSPU(char* szPort, char* szCmdInput, char* sznCmdLength, char* szbAddCRC)
{
	return DLRAWRequest(atoi(szPort), szCmdInput, atoi(sznCmdLength), TransferCharToBool(szbAddCRC));
}

MOBILETOOLKIT_API bool DiagRAWRequest_ForSPU(char* szPort, char* szCmdInput, char* sznCmdLength, char* szbAddCRC,char* szbHasReturn)
{
	return DiagRAWRequest(atoi(szPort), szCmdInput, atoi(sznCmdLength), TransferCharToBool(szbAddCRC), TransferCharToBool(szbHasReturn));
}

MOBILETOOLKIT_API bool ByPassCellTemperatureCheck_ForSPU(char* szPort)
{
	return ByPassCellTemperatureCheck(atoi(szPort));
}

MOBILETOOLKIT_API bool ReadSPC_ForSPU(char* szPort, char* szSPC, char* szStatus/*unlocked, locked, default unlocked000000*/, char* szTimeOut)
{
	CCFCDiagCommand CDiagCommand(atoi(szPort));
	long nStatus = 0;
	bool bRes = CDiagCommand.ReadSPC(szSPC, nStatus, atoi(szTimeOut));
	if (bRes)
	{
		CStringA strTemp;
		if (nStatus == 0)
			strTemp = "unlocked";
		else if (nStatus == 1)
			strTemp = "locked";
		else if (nStatus == 2)
			strTemp = "default unlocked";
		else
			strTemp = "unknown status";
		memcpy(szStatus, strTemp, strTemp.GetLength()+1);
	}
	return bRes;
}

MOBILETOOLKIT_API bool IsConnected_ForSPU(char* szPort)
{
	CCFCDiagCommand CDiagCommand(atoi(szPort));
	return CDiagCommand.IsConnected();
}

MOBILETOOLKIT_API bool ReadFSN_ForSPU(char* szPort, char* szFSN_base64, char* sznTimeOut)
{
	CCFCDiagCommand CDiagCommand(atoi(szPort));
	BYTE  szFSN16[16] = {0};
	bool bRes =CDiagCommand.ReadFSN(szFSN16, atoi(sznTimeOut));
	if (bRes)
	{
		int nDataLen = Base64EncodeGetRequiredLength(16);
		if(nDataLen > 1024)
			bRes = false;
		else
		{
			memset(szFSN_base64, 0, nDataLen);
			bRes = Base64Encode(szFSN16, 16, szFSN_base64, &nDataLen) == TRUE;
		}
	}

	return bRes;
}

MOBILETOOLKIT_API bool WriteFSN_ForSPU(char* szPort, char* szFSN_base64, char* sznTimeOut)
{
	bool bRes = false;
	CCFCDiagCommand CDiagCommand(atoi(szPort));
	size_t nOgnDataBufLen = strlen(szFSN_base64);
	int nDecLen = Base64DecodeGetRequiredLength((int)nOgnDataBufLen);
	if (nDecLen <= 1024)
	{
		BYTE* FSNBuf = new BYTE[nDecLen];
		memset(FSNBuf, 0, nDecLen);

		bRes = Base64Decode(szFSN_base64, (int)nOgnDataBufLen, FSNBuf, &nDecLen) == TRUE;
		if (bRes)
			bRes = CDiagCommand.WriteFSN(FSNBuf, 5000);

		delete [] FSNBuf;
	}
	return bRes;
}

MOBILETOOLKIT_API bool ReadFactoryHWSWMW(char* szPort,char* szHW5,char* szSW9,char* szMW2)
{
	CNVItem nvItem(::_ttoi(szPort));
	return nvItem.ReadFactoryHWSWMW(szHW5,szSW9,szMW2);
}

MOBILETOOLKIT_API bool WriteFactoryHWSWMW(char* szPort,char* szHW5,char* szSW9,char* szMW2)
{
	CNVItem nvItem(::_ttoi(szPort));
	return nvItem.WriteFactoryHWSWMW(szHW5,szSW9,szMW2);
}

MOBILETOOLKIT_API bool ReadMobileID_ForSPU(char* szPort, char* szLine7,char* szAdapter3,char* szSlot3,char* szDay3,char* szMonth3,char* szYear3,char* szHour3,char* szMinute3,char* szSecond3)
{
	return ReadMobileID(::atoi(szPort), szLine7,szAdapter3,szSlot3, szDay3, szMonth3, szYear3, szHour3, szMinute3,szSecond3);
}

MOBILETOOLKIT_API bool WriteMobileID_ForSPU(char* szPort, char* szSlot3)
{
	return WriteMobileID(::atoi(szPort), szSlot3);
}

MOBILETOOLKIT_API bool ReadProcessInfo(char* szPort,char* szProcessIndex,char* szYear,char* szMonth,char* szDay,char* szLine,char* szStation)
{
	CNVItem nvItem(::_ttoi(szPort));
	return nvItem.ReadProcessInfo(szProcessIndex,szYear,szMonth,szDay,szLine,szStation);
}

MOBILETOOLKIT_API bool WriteProcessInfo(char* szPort,char* szProcessIndex)
{
	CNVItem nvItem(::_ttoi(szPort));
	return nvItem.WriteProcessInfo(szProcessIndex);
}

MOBILETOOLKIT_API bool ReadPicassoNumber(char* szPort,char* szPICASSO)
{
	CNVItem nvItem(::_ttoi(szPort));
	int nBufferSize  = 1024;
	return nvItem.ReadPicassoNumber(szPICASSO,nBufferSize);
}

MOBILETOOLKIT_API bool WritePicassoNumber(char* szPort,char* szPICASSO)
{
	CNVItem nvItem(::_ttoi(szPort));
	return nvItem.WritePicassoNumber(szPICASSO);
}

MOBILETOOLKIT_API bool ReadTrackID(char* szPort,char* szTrackID)
{
	CNVItem nvItem(::_ttoi(szPort));
	int nBufferSize  = 1024;
	return nvItem.ReadTrackID(szTrackID,nBufferSize,20);
}

MOBILETOOLKIT_API bool WriteTrackID(char* szPort,char* szTrackID)
{
	CNVItem nvItem(::_ttoi(szPort));
	return nvItem.WriteTrackID(szTrackID,20);
}

//The sample is GetMobileID("T01D02",0)
CStringA GenerateMobileID(const char* strComputer_Name,int nSlot)
{
	// 45-55: MobileID = line number(3 bytes)+adapter+DUT number+ datetime(6 bytes)
	CStringA cstrMobileID;
	//1. line number = COMPUTER_NAME will be like T01D02, we remove the character and add "00" to create a new computer name and the new name will be like 010200--->line number
	CStringA strComputerName = strComputer_Name;
	strComputerName = strComputerName.Mid(1,2)+strComputerName.Mid(4,2)+"00";
	//2. adapter = "00"
	//3. DUT number = slot
	CStringA strSlot;
	strSlot.Format("%2u", nSlot);
	//4. date time
	CStringA strDataTime;
	CTime Current_Time = CTime::GetCurrentTime();
	strDataTime = Current_Time.Format("%d%m%y%H%M%S");
	cstrMobileID = strComputerName+ "00" + strSlot + strDataTime;
	//assert(cstrMobileID.GetLength() == g_nMobile_ID_Size*2);
	//if(cstrMobileID.GetLength() != g_nMobile_ID_Size*2)
	//	AfxMessageBox("MOBILE Length Wrong!!");
	BYTE nMobileID[100] = {0};
	QSYNC_LIB::CharToByte(cstrMobileID, cstrMobileID.GetLength(), nMobileID);
	//memcpy(resp.item_data+45, nMobileID, g_nMobile_ID_Size);

	return cstrMobileID;
}

MOBILETOOLKIT_API bool ReadMMIFlag(char* szPort,char* szTestItem,char* sznResult)
{
	CNVItem nvItem(::_ttoi(szPort));
	return nvItem.ReadMMIFlag(szTestItem,sznResult);
}

MOBILETOOLKIT_API bool WriteMMIFlag(char* szPort,char* szTestItem,char* sznResult)
{
	CNVItem nvItem(::_ttoi(szPort));
	return nvItem.WriteMMIFlag(szTestItem,sznResult);
}

#pragma warning(push)
#pragma warning(disable:4996)
MOBILETOOLKIT_API bool TestForSPU(char* szTemp1,char* szTemp2)
{
	strcpy(szTemp2,szTemp1);
	return true;
}
#pragma warning(pop)

MOBILETOOLKIT_API bool USBChargingSwitch(char* szPort,char* szbEnable)
{
	CODM odm(atoi(szPort));
	bool bRes =  odm.USBChargingSwitch(TransferCharToBool(szbEnable));
	return bRes;
}

MOBILETOOLKIT_API bool DisplayTest(char* szPort,char* szColorR,char* szColorG,char* szColorB,char* szBacklight)
{
	#define RGB565(R,G,B)         ((unsigned short)( (((WORD)B&0xF8)>>3) | (((WORD)G&0xFC)<<3) | (((WORD)R&0xF8)<<8)))
	WORD byteColorR = ::atoi(szColorR);
	WORD byteColorG = ::atoi(szColorG);
	WORD byteColorB = ::atoi(szColorB);
	short nColor565 = RGB565(byteColorR,byteColorG,byteColorB);
	BYTE byteBackLight = ::atoi(szBacklight);
	CODM odm(atoi(szPort));
	bool bRes =  odm.DisplayTest(nColor565,byteBackLight);
	return bRes;
}

MOBILETOOLKIT_API bool CoinCellChargingSwitch(char* szPort,char* szbEnable)
{
	CODM odm(atoi(szPort));
	bool bRes =  odm.CoinCellChargingSwitch(TransferCharToBool(szbEnable));
	return bRes;
}

MOBILETOOLKIT_API bool QLIB_CapturePicture_ForSPU(char* szPort,char* sznSensorID,char* sznWidth,char* sznHeight,char* strTargetFile)
{
	CODM odm(atoi(szPort));
	bool bRes =  odm.QLIB_CapturePicture_ForSPU(sznSensorID,sznWidth,sznHeight,strTargetFile);
	return bRes;
}

MOBILETOOLKIT_API bool KeypadBacklight(char* szPort,char* szbEnable)
{
	CODM odm(atoi(szPort));
	bool bRes =  odm.KeypadBacklight(TransferCharToBool(szbEnable));
	return bRes;
}

MOBILETOOLKIT_API bool KeypadPressTest(char* szPort,char* szbEnable)
{
	CODM odm(atoi(szPort));
	bool bRes =  odm.KeypadPressTest(TransferCharToBool(szbEnable));
	return bRes;
}

MOBILETOOLKIT_API bool GetCoincellVoltage(char* szPort,char* sznVoltage)
{
	CODM odm(atoi(szPort));
	int nVoltage = 0;
	bool bRes =  odm.GetCoincellVoltage(nVoltage,bRes);
	if(bRes)
	{
		::sprintf_s(sznVoltage,1024,"%04d",nVoltage);
	}
	return bRes;
}

MOBILETOOLKIT_API bool GetFlipStatus(char* szPort,char* sznOpen)
{
	CODM odm(atoi(szPort));
	bool bFlipOpen = false;
	bool bRes =  odm.GetFlipStatus(bFlipOpen);
	if(bRes)
	{
		TransferBoolToChar(bFlipOpen,sznOpen);
	}
	return bRes;
}

MOBILETOOLKIT_API bool CaptureScreenshot(char* szPort)
{
	CODM odm(atoi(szPort));	
	bool bRes =  odm.CaptureScreenshot();
	return bRes;
}

MOBILETOOLKIT_API bool BatteryTemperatureDetectionSwitch(char* szPort,char* szbEnable)
{
	CODM odm(atoi(szPort));	
	bool bRes =  odm.BatteryTemperatureDetectionSwitch(TransferCharToBool(szbEnable));
	return bRes;
}

MOBILETOOLKIT_API bool ATEnableKeypadEvent(ISPU_CALLBACK *pISPU_CALLBACK)
{
	bool bRes = false;
	if(pISPU_CALLBACK)
	{
		char szPort[50] = {0};
		pISPU_CALLBACK->WaitInput("Modemport",szPort,50);
		CODM odm(atoi(szPort));	
		bRes =  odm.ATEnableKeypadEvent(pISPU_CALLBACK,30000);
	}
	return bRes;
}