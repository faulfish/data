
#include "MobileToolKit.h"
#include "..\..\FactoryTool\SPU\ISPU_CALLBACK.h"

//Comapre internal API
MOBILETOOLKIT_API bool bigger(char* sznTest,char* sznBigger);
MOBILETOOLKIT_API bool smaller(char* sznTest,char* sznSmaller);
MOBILETOOLKIT_API bool ranage(char* sznTest,char* sznSmaller,char* sznBigger);

//Log
MOBILETOOLKIT_API bool EnableLog_ForSPU(char* szEnableLog);
typedef bool (*pFnChangeLogFileName)(char* szCOMPort,char* szNewFileName);
MOBILETOOLKIT_API bool ChangeLogFileName(char* szCOMPort,char* szNewFileName);

//Bluetooth
typedef bool (*pFnWriteBTAddr_ForSPU)(char* szPort,char* inBTAddrBuffer);
MOBILETOOLKIT_API bool WriteBTAddr_ForSPU(char* szPort,char* inBTAddrBuffer);
typedef bool (*pFnGetBTAddr_ForSPU)(char* szPort,char* outBTAddrBuffer,char* szBufferSize);
MOBILETOOLKIT_API bool GetBTAddr_ForSPU(char* szPort,char* outBTAddrBuffer,char* szBufferSize);
typedef bool (*pFnBTBound)(char* szPort, const char* szbBound,const char* szBTAddr, const char* szBTPass,const char* szTimeout);
MOBILETOOLKIT_API bool BTBound(char* szPort, const char* szbBound,const char* szBTAddr, const char* szBTPass,const char* szTimeout);
typedef bool (*pFnWriteHeadsetBTAddress_ForSPU)(char* szPort, char* szBTAddr);
MOBILETOOLKIT_API bool WriteHeadsetBTAddress_ForSPU(char* szPort, char* szBTAddr);
typedef bool (*pFnCheckBTHeadsetState_ForSPU)(char* szPort, char* szBTAddr);
MOBILETOOLKIT_API bool CheckBTHeadsetState_ForSPU(char *szPort, char* szBTAddr);
MOBILETOOLKIT_API bool BTHeadsetChargingPower_ForSPU(char* szPort, char* szIsOn);
MOBILETOOLKIT_API bool BTHeadsetVoltageReading_ForSPU(char* szPort, char* szVoltage, char* szTimeout);
MOBILETOOLKIT_API bool EnableGAP_ForSPU(char* szPort, char* szbEnable);
MOBILETOOLKIT_API bool StartBTPower_ForSPU(char* szPort, char* szDegree);
MOBILETOOLKIT_API bool EnterBTTestMode_ForSPU(char* szPort, char* szbEnable);
MOBILETOOLKIT_API bool BTHFPGateway(char* szPort, char* szbEnable);
MOBILETOOLKIT_API bool BTHFPGatewayConnect(char* szPort, char* szbEnable, char* szBTAddr);
MOBILETOOLKIT_API bool BTHFPGatewayAnswer(char* szPort, char* szbEnable);
MOBILETOOLKIT_API bool BTHFPGatewayAudioConnect(char* szPort, char* szbEnable);
MOBILETOOLKIT_API bool BTHFPGatewaySpeakerGain(char* szPort);
MOBILETOOLKIT_API bool BTHFPGatewayMicrophoneGain(char* szPort);
MOBILETOOLKIT_API bool BTHFPGatewayAudioLoop(char* szPort, char* szbEnable);
MOBILETOOLKIT_API bool BTHFPGatewayRoaming(char* szPort, char* szbEnable);

//Audio
MOBILETOOLKIT_API bool PlayMP3(char* szPort,char* szbEnable);
MOBILETOOLKIT_API bool SpeakerDevice(char* szPort,char* szbEnable);
MOBILETOOLKIT_API bool SpeakerDeviceMultimediaLevel(char* szPort,char* sznLevel);
MOBILETOOLKIT_API bool SpeakerDeviceVoiceLevel(char* szPort,char* sznLevel);
MOBILETOOLKIT_API bool HandsetDevice(char* szPort,char* szbEnable);
MOBILETOOLKIT_API bool HandsetDeviceMultimediaLevel(char* szPort,char* sznLevel);
MOBILETOOLKIT_API bool HandsetDeviceVoiceLevel(char* szPort,char* sznLevel);
MOBILETOOLKIT_API bool MonoHeadsetDevice(char* szPort,char* szbEnable);
MOBILETOOLKIT_API bool MonoHeadsetDeviceMultimediaLevel(char* szPort,char* sznLevel);
MOBILETOOLKIT_API bool MonoHeadsetDeviceVoiceLevel(char* szPort,char* sznLevel);
MOBILETOOLKIT_API bool StereoHeadsetDevice(char* szPort,char* szbEnable);
MOBILETOOLKIT_API bool StereoHeadsetDeviceMultimediaLevel(char* szPort,char* sznLevel);
MOBILETOOLKIT_API bool StereoHeadsetDeviceVoiceLevel(char* szPort,char* sznLevel);
MOBILETOOLKIT_API bool BluetoothDevice(char* szPort,char* szbEnable);
MOBILETOOLKIT_API bool BluetoothDeviceMultimediaLevel(char* szPort,char* sznLevel);
MOBILETOOLKIT_API bool BluetoothDeviceVoiceLevel(char* szPort,char* sznLevel);
MOBILETOOLKIT_API bool AudioPackageLoopback(char* szPort,char* szbEnable);
MOBILETOOLKIT_API bool AlarmTone(char* szPort,char* sznTone);

//FM
MOBILETOOLKIT_API bool FMRegistration_ForSPU(char* szPort);
MOBILETOOLKIT_API bool FMConnect_ForSPU(char* szPort, char* szbConnect);
MOBILETOOLKIT_API bool FMInitialize_ForSPU(char* szPort);
MOBILETOOLKIT_API bool FMSetStation_ForSPU(char* szPort, char* szFMFrequence);
MOBILETOOLKIT_API bool FMMute_ForSPU(char* szPort, char* szbMute);
MOBILETOOLKIT_API bool FMChangeTestModeAndSetLevel(char* szPort, char* sznLevel);
MOBILETOOLKIT_API bool FMReceiveRSSI_ForSPU(char* szPort, char* szRSSI);
MOBILETOOLKIT_API bool FMSpeaker_ForSPU(char* szPort);
MOBILETOOLKIT_API bool FMMonoHeadset_ForSPU(char* szPort);
MOBILETOOLKIT_API bool FMStereoHeadset_ForSPU(char* szPort);

//GSensor
MOBILETOOLKIT_API bool GSensorCalibration(char* szPort,char* sznResult);
MOBILETOOLKIT_API bool IsGSensorWorking_ForSPU(char* szPort);
MOBILETOOLKIT_API bool IsGSensorWorkingV2_ForSPU(char* szPort,char* szbIsGSensorWork, char* szbIsUpWork);
MOBILETOOLKIT_API bool IsGSensorWorkingV3_ForSPU(char* szPort, char* sznX, char* sznY, char* sznZ);
MOBILETOOLKIT_API bool GSensorOffset_ForSPU(char* szPort, char* sznOffsetX, char* sznOffsetY, char* sznOffsetZ);

MOBILETOOLKIT_API bool GetIMEI_ForSPU(char* szPort,char* outIMEI);

MOBILETOOLKIT_API bool WriteIMEI_ForSPU(char* nPort, char* inIMEI);

MOBILETOOLKIT_API bool LockSPC_ForSPU(char* szPort);

MOBILETOOLKIT_API bool IsSPCLocked_ForSPU(char* nPort,char* szStatus);

MOBILETOOLKIT_API bool UnlockSPC_ForSPU(char* szPort, char* szLock, char* szUnlockOK);

MOBILETOOLKIT_API bool ProgramSPC_ForSPU(char* szPort, char* szLock);

MOBILETOOLKIT_API bool ClearActivationData_ForSPU(char* szPort);

MOBILETOOLKIT_API bool MasterClear_ForSPU(char* szPort);

MOBILETOOLKIT_API bool StandbyCurrentTest_ForSPU(char* szPort);
MOBILETOOLKIT_API bool CheckSIMCardExistence_ForSPU(char* szPort, char* szType);

MOBILETOOLKIT_API bool ReadFactoryAreaNew_ForSPU(char* szPort, char* szNVItem, char* szFactoryArea128_Base64, char* szState, char* szTimeOut);//Note: szFactoryArea128 output is in Base64 format, user needs to decode it.

MOBILETOOLKIT_API bool EnableQualcommDebugEvent_ForSPU(char* szPort, char* szbEnable);

//Write SIM data
MOBILETOOLKIT_API bool WriteSimData_ForSPU(char* szPort, char* szSimDataBuf_Base64, char* szSimDataBufLen);//Note: szSimDataBuf input is in Base64 format, this function will decode it.
MOBILETOOLKIT_API bool WriteSimDataV2_ForSPU(char* szPort, char* szSimDataBuf_Base64, char* szSimDataBufLen);//Note: szSimDataBuf input is in Base64 format, this function will decode it.
MOBILETOOLKIT_API bool WriteSimLockFile_ForSPU(char* szPort, char* szSIMLockFileName);

//Backup\Restore NV
MOBILETOOLKIT_API bool BackupNV2BRT_ForSPU(char* szPort, char* szOutputBRTFileName, char* szInputCSVFile);
MOBILETOOLKIT_API bool RestoreBRT2NV_ForSPU(char* szPort, char* szInBRTFileName);
MOBILETOOLKIT_API bool BackupNVPartition_ForSPU(char* szPort);
MOBILETOOLKIT_API bool RestoreNVPartition_ForSPU(char* szPort);
MOBILETOOLKIT_API bool CheckNVPartition_ForSPU(char* szPort, char* szBackupTimes, char* szLastBackupTime);

//NV items
typedef bool (*pFnReadNVItem_ForSPU)(char* szPort, char* szNVItem, char* szitem_data_base64, char* szNV_stat);
MOBILETOOLKIT_API bool ReadNVItem_ForSPU(char* szPort, char* szNVItem, char* szitem_data_base64, char* szNV_stat); //Note: szNVItem input is in Base64 format, this function will decode it.
MOBILETOOLKIT_API bool WriteNVItem_ForSPU(char* szPort, char* sznNVItem, char* szWriteData_Base64, char* sznDataLen,char* szNV_stat);	

//ReadWrite specific NV item
MOBILETOOLKIT_API bool WriteSIMLockFileNameToNV_ForSPU(char* szPort, char* szSIMLockFileName);
MOBILETOOLKIT_API bool ReadSIMLockFileNameFromNV_ForSPU(char* szPort, char* szSIMLockFileName);

//Get version
MOBILETOOLKIT_API bool GetSWHWVersion_ForSPU(char* szPort, char* sSW, char* sHW);
MOBILETOOLKIT_API bool GetLavernockHWVersion_ForSPU(char* szPort, char* sznVersion);
typedef bool (*pFnGetLavernockHWVersionV2_ForSPU)(char* szPort, char* sznVarientVersion, char* sznEVTVersion);
MOBILETOOLKIT_API bool GetLavernockHWVersionV2_ForSPU(char* szPort, char* sznVarientVersion, char* sznEVTVersion);

//Read/Write QFuse
MOBILETOOLKIT_API bool WriteQFuse_ForSPU(char* szPort, char* szChain, char* szLSB, char* szMSB, char* szRegConf2, char* szRegConf3, char* szRegConf4);
MOBILETOOLKIT_API bool ReadQFuseState_ForSPU(char* szPort, char* szReg1, char* szReg2, char* szbSuccess);

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
MOBILETOOLKIT_API bool ChangeMode_ForSPU(char* szPort, char* sznMode);

//FTM Mode

MOBILETOOLKIT_API bool FTMMode_ForSPU(char* szPort);
MOBILETOOLKIT_API bool ChangeFTM_ForSPU(char* szPort); //Write NV item , must be reset
MOBILETOOLKIT_API bool CheckFTM_ForSPU(char* szPort, char* sznFTMValue);//Read NV item

//Online mode
MOBILETOOLKIT_API bool OnLineMode_ForSPU(char* szPort);
MOBILETOOLKIT_API bool SetOnline_ForSPU(char* szPort);
MOBILETOOLKIT_API bool SetOffline_ForSPU(char* szPort);
MOBILETOOLKIT_API bool ChangeOnline_ForSPU(char* szPort);

//PowerOff
MOBILETOOLKIT_API bool PowerOff_ForSPU(char* szPort);

//EM mode
MOBILETOOLKIT_API bool TurnOnOffEM_ForSPU(char* szPort, char* szbOpenEM);

MOBILETOOLKIT_API bool ResetHandset_ForSPU(char* szPort);

//Failurebyte
MOBILETOOLKIT_API bool WriteFailureByte_ForSPU(char* szPort, char* sznFailureByte);

// Search BT device
MOBILETOOLKIT_API bool SearchBTDevice_ForSPU(char* szPort);
// Read Battery voltage
//3100 mV will close
//3500 mV is safe
//4000 mV is for factory
MOBILETOOLKIT_API bool ReadBatteryVoltage_ForSPU(char* szPort, char* sznVoltage);
MOBILETOOLKIT_API bool ReadBatteryTemperature_ForSPU(char* szPort, char* sznTemperature);

// WriteDefaultPhoneCode 1234
MOBILETOOLKIT_API bool WriteDefaultPhoneCode_ForSPU(char* szPort);

MOBILETOOLKIT_API bool FreezeOTP_ForSPU(char* szPort);
//DIAG_ERROR_WRONG_OTP_PROTECTED_CODE
//DIAG_ERROR_EMPTY_OTP
//DIAG_ERROR_DAMAGED_OTP
MOBILETOOLKIT_API bool IsOTPFrozen_ForSPU(char* szPort, char* szbFrozen,char* sznLastError);

MOBILETOOLKIT_API bool ReplaceEFSFile_ForSPU(char* szPort, const std::string& strEFSFilePath, const std::string& strNewLocalFilePath);

MOBILETOOLKIT_API bool Vibrator(char* szPort,char* szEnable);

MOBILETOOLKIT_API bool CheckSIMLock_ForSPU(char* szPort, char* szSIMLockName);

MOBILETOOLKIT_API bool CheckCEFSFileName_ForSPU(char* szPort, char* szCEFSFileNameBuffer, char* sznBufferSize);
typedef bool (*pFnGetMicroSDInfo_ForSPU)(char* szPort, char* sznBlockSize, char* sznTotalBlock);
MOBILETOOLKIT_API bool GetMicroSDInfo_ForSPU(char* szPort, char* sznBlockSize, char* sznTotalBlock);

MOBILETOOLKIT_API bool DLRAWRequest_ForSPU(char* szPort, char* szCmdInput, char* sznCmdLength, char* szbAddCRC);

MOBILETOOLKIT_API bool DiagRAWRequest_ForSPU(char* szPort, char* szCmdInput, char* sznCmdLength, char* szbAddCRC,char* szbHasReturn);

MOBILETOOLKIT_API bool ByPassCellTemperatureCheck_ForSPU(char* szPort);

MOBILETOOLKIT_API bool ReadSPC_ForSPU(char* szPort, char* szSPC, char* szStatus/*unlocked, locked, default unlocked000000*/, char* szTimeOut);

MOBILETOOLKIT_API bool IsConnected_ForSPU(char* szPort);

MOBILETOOLKIT_API bool ReadFSN_ForSPU(char* szPort, char* szFSN_base64, char* sznTimeOut);

MOBILETOOLKIT_API bool WriteFSN_ForSPU(char* szPort, char* szFSN_base64, char* sznTimeOut);

//Factory area
typedef bool (*pFnReadPicassoNumber)(char* szPort,char* szPICASSO);
MOBILETOOLKIT_API bool ReadPicassoNumber(char* szPort,char* szPICASSO);
typedef bool (*pFnWritePicassoNumber)(char* szPort,char* szPICASSO);
MOBILETOOLKIT_API bool WritePicassoNumber(char* szPort,char* szPICASSO);
typedef bool (*pFnReadTrackID)(char* szPort,char* szTrackID);
MOBILETOOLKIT_API bool ReadTrackID(char* szPort,char* szTrackID);
typedef bool (*pFnWriteTrackID)(char* szPort,char* szTrackID);
MOBILETOOLKIT_API bool WriteTrackID(char* szPort,char* szTrackID);
typedef bool (*pFnReadProcessInfo)(char* szPort,char* szProcessIndex,char* szYear,char* szMonth,char* szDay,char* szLine,char* szStation);
MOBILETOOLKIT_API bool ReadProcessInfo(char* szPort,char* szProcessIndex,char* szYear,char* szMonth,char* szDay,char* szLine,char* szStation);
typedef bool (*pFnWriteProcessInfo)(char* szPort,char* szProcessIndex);
MOBILETOOLKIT_API bool WriteProcessInfo(char* szPort,char* szProcessIndex);
typedef bool (*pFnReadMobileID_ForSPU)(char* szPort, char* szLine7,char* szAdapter3,char* szSlot3,char* szDay3,char* szMonth3,char* szYear3,char* szHour3,char* szMinute3,char* szSecond3);
MOBILETOOLKIT_API bool ReadMobileID_ForSPU(char* szPort, char* szLine7,char* szAdapter3,char* szSlot3,char* szDay3,char* szMonth3,char* szYear3,char* szHour3,char* szMinute3,char* szSecond3);
typedef bool (*pFnWriteMobileID_ForSPU)(char* szPort, char* szSlot3);
MOBILETOOLKIT_API bool WriteMobileID_ForSPU(char* szPort, char* szSlot3);
typedef bool (*pFnReadFactoryHWSWMW)(char* szPort,char* szHW5,char* szSW9,char* szMW2);
MOBILETOOLKIT_API bool ReadFactoryHWSWMW(char* szPort,char* szHW5,char* szSW9,char* szMW2);
typedef bool (*pFnWriteFactoryHWSWMW)(char* szPort,char* szHW5,char* szSW9,char* szMW2);
MOBILETOOLKIT_API bool WriteFactoryHWSWMW(char* szPort,char* szHW5,char* szSW9,char* szMW2);
typedef bool (*pFnReadMMIFlag)(char* szPort,char* szTestItem,char* sznResult);
MOBILETOOLKIT_API bool ReadMMIFlag(char* szPort,char* szTestItem,char* sznResult);
typedef bool (*pFnWriteMMIFlag)(char* szPort,char* szTestItem,char* sznResult);
MOBILETOOLKIT_API bool WriteMMIFlag(char* szPort,char* szTestItem,char* sznResult);
/*
384		1						used		EM_BZ_RW_FACTORY_BYTE
385		1						used		EM_BZ_ON_OFF_LCM
386		1						used		EM_BZ_ON_OFF_CLI
387		1						used		EM_BZ_AUDIO_LOOP
388		1						used		EM_BZ_ON_OFF_CAMERA
389		1						used		EM_BZ_TAKE_A_PICTURE
390		1						used		EM_BZ_READ_A_PICTURE
391		1						used		EM_BZ_SET_AUDIO_SPEAKER
392		1						used		EM_BZ_SET_AUDIO_RECEIVER
393		1						used		EM_BZ_SET_AUDIO_FM
394		1						used		EM_BZ_SET_AUDIO_EAR_PIECE
395		1						used		EM_BZ_EN_DI_KEYPAD_VALIDATION
396		1						used		EM_BZ_USB_MASS_STORAGE
397		1						used		EM_BZ_SET_FM_FREQ
398		1						used		EM_BZ_AUDIO_CIRCUIT
399		1						used		EM_BZ_RSSI_OF_FM
400		1						used		EM_BZ_DEEP_SLEEP_MODE
401		1						used		EM_BZ_MEMORY_CARD
402		1						used		EM_BZ_EAR_JACK
403		1						used		EM_BZ_SIGN_OF_LIFE
404		1						used		EM_BZ_BT_PING
405		1						used		EM_BZ_BT_CW_POWER
406		1						used		EM_BZ_CW_POWER_FREQ
407		1						used		EM_BZ_ON_OFF_VIBRATOR
408		1						used		EM_BZ_ON_OFF_BUZZER
409		1						used		EM_BZ_ON_OFF_KEYPAD_LIGHT
410		1						used		EM_BZ_SET_POWER_CHANNEL
411		1						used		EM_BZ_SET_POWER_LEVEL
412		1						used		EM_BZ_EN_DI_TX_POWER
413		1						used		EM_BZ_READ_RX_POWER
414		1						used		EM_BZ_READ_CHARGER_STATUS1
415		1						used		EM_BZ_R_W_FLIP_STATUS
416		1						used		EM_BZ_USB_CONN
417		1						used		EM_BZ_READ_CHARGER_STATUS2
418		1						used		EM_BZ_READ_SIM_CARD_STATUS
419		1						used		EM_BZ_READ_NRG
420		1						used		EM_BZ_OC_WITH_SIM_CARD
421		1						used		EM_BZ_TERMINATE_CALL
422		1						used		EM_BZ_SCAN_RF_CHANNELS
423		1						used		EM_BZ_ANSWER_CALL
424		1						used		EM_BZ_SET_TEH_BAND
425		1						used		EM_BZ_SEND_KEY_PRESS
426		1						used		EM_BZ_READ_BSN
427		1						used		EM_BZ_GSENSOR
428		1						used		EM_BZ_RTC
429		1						used		EM_BZ_MP3
430		1						used		EM_BZ_FM_FREQ
431		1						used		EM_BZ_CONINCELL
*/
typedef bool (*pFnQLIB_CapturePicture_ForSPU)(char* szPort,char* sznSensorID,char* sznWidth,char* sznHeight,char* strTargetFile);
MOBILETOOLKIT_API bool QLIB_CapturePicture_ForSPU(char* szPort,char* sznSensorID,char* sznWidth,char* sznHeight,char* strTargetFile);
typedef bool (*pFnKeypadBacklight)(char* szPort,char* szbEnable);
MOBILETOOLKIT_API bool KeypadBacklight(char* szPort,char* szbEnable);
typedef bool (*pFnKeypadPressTest)(char* szPort,char* szbEnable);
MOBILETOOLKIT_API bool KeypadPressTest(char* szPort,char* szbEnable);
typedef bool (*pFnUSBChargingSwitch)(char* szPort,char* szbEnable);
MOBILETOOLKIT_API bool USBChargingSwitch(char* szPort,char* szbEnable);
typedef bool (*pFnCoinCellChargingSwitch)(char* szPort,char* szbEnable);
MOBILETOOLKIT_API bool CoinCellChargingSwitch(char* szPort,char* szbEnable);
typedef bool (*pFnDisplayTest)(char* szPort,char* szColorR,char* szColorG,char* szColorB,char* szBacklight);
MOBILETOOLKIT_API bool DisplayTest(char* szPort,char* szColorR,char* szColorG,char* szColorB,char* szBacklight);
MOBILETOOLKIT_API bool GetCoincellVoltage(char* szPort,char* sznVoltage);
MOBILETOOLKIT_API bool GetFlipStatus(char* szPort,char* sznOpen);
MOBILETOOLKIT_API bool BatteryTemperatureDetectionSwitch(char* szPort,char* szbEnable);

MOBILETOOLKIT_API bool CaptureScreenshot(char* szPort);
MOBILETOOLKIT_API bool SetRTC(char* szPort);
MOBILETOOLKIT_API bool GetRTC(char* szPort,char* sziMonth, char* sziDay, char* sziYear, char* sziHour, char* sziMinute,char* sziSecond);
MOBILETOOLKIT_API bool CheckRTCWithCurrentTime(char* szPort);

MOBILETOOLKIT_API bool TestForSPU(char* szTemp1,char* szTemp2);

//SPU Thread API
MOBILETOOLKIT_API bool ATEnableKeypadEvent(ISPU_CALLBACK *pISPU_CALLBACK);
