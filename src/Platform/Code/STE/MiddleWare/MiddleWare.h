// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MIDDLEWARE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MIDDLEWARE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifndef __MIDDLE_WARE_H__
#define __MIDDLE_WARE_H__

#include <string>
#include "tstl.h"
#include "..\..\Modules\QisdaAudioModule\Audio.h"
#include "..\..\Modules\QisdaEFsModule\EFs.h"
#include "..\..\Modules\QisdaVideoModule\Video.h"
#include "..\..\odmtma_api\SerCOMLib\SerCOMLibrary.h"

using namespace std;

#ifdef MIDDLEWARE_EXPORTS
#define MIDDLEWARE_API extern "C" __declspec(dllexport)
#else
#define MIDDLEWARE_API extern "C"  __declspec(dllimport)
#endif

typedef char*(__stdcall *pMessageSender)(char*, char*);


//  [5/22/2008 Eddy.Guo]
// Test function
MIDDLEWARE_API bool GetIMEI(TCHAR* szIMEI);
MIDDLEWARE_API bool GetMotoXPRSLogs(TCHAR* szLogPath);
//MIDDLEWARE_API int ReadBootLoaderVersion();


//  [5/22/2008 Eddy.Guo]
// Command Module API.
MIDDLEWARE_API bool ReportCapabilities(TCHAR* configFile, TCHAR* outputString);
MIDDLEWARE_API bool OpenDialog(TCHAR* dialogName);
MIDDLEWARE_API bool PhoneCommand(TCHAR* parameters, TCHAR* outputString);
MIDDLEWARE_API bool ReportDialogs(TCHAR* outputString);
MIDDLEWARE_API bool SetCallBack(pMessageSender pCallBack);
MIDDLEWARE_API bool SetModel(TCHAR* model, TCHAR* protocol);
MIDDLEWARE_API bool Shutdown();


//  [5/22/2008 Eddy.Guo]
// Callback Types, implemented in CallbackTypes.cpp
//MIDDLEWARE_API bool CheckAbortStatus(tstring& outputString);
//MIDDLEWARE_API bool ManageResource(char* pAction, char* pResource, tstring& outputString);
//MIDDLEWARE_API bool OutputData(char* pDescription, char* pData, tstring& outputString);
//MIDDLEWARE_API bool OutputDetailResult(char* pDescription, char* pBand, char* pLowLimit, char* pHighLimit, char* pResult, char* pUnits, char* pPassFailFlag, tstring& outputString);
//MIDDLEWARE_API bool OutputHeader(char* pHeader, tstring& outputString);
//MIDDLEWARE_API bool OutputMessage(char* pMessage, tstring& outputString);
//MIDDLEWARE_API bool OutputPrompt(char* pTitle, char* pPrompt, char* PromptType, tstring& outputString);
//MIDDLEWARE_API bool OutputResult(char* pDescription, char* pLowLimit, char* pHighLimit, char* pResult, char* pPassFailFlag, tstring& outputString);
//MIDDLEWARE_API bool OutputStatus(char* pStatusCode, tstring& outputString);


//  [5/22/2008 Eddy.Guo]
// Command Module Dialogs
// Dialog "FlashFlex"
// Dialog "Keypad"


//  [5/22/2008 Eddy.Guo]
// Phone Commands
MIDDLEWARE_API bool Alert(bool alertOn, TCHAR* outputString);
MIDDLEWARE_API bool AntennaPosition(TCHAR* outputString);
MIDDLEWARE_API bool AudioLoopback(TCHAR* loopbackState, TCHAR* outputString);
MIDDLEWARE_API bool AudioMidi(TCHAR playState, TCHAR* outputString);
MIDDLEWARE_API bool AudioTest(TCHAR* audioState, TCHAR* outputString);
MIDDLEWARE_API bool CallDropLogger(TCHAR* readType, TCHAR* outputString);
MIDDLEWARE_API bool CameraFlashLED(TCHAR ledOn, TCHAR* outputString);
MIDDLEWARE_API bool CameraImage(TCHAR* cameraState, TCHAR* outputString);
MIDDLEWARE_API bool CameraViewfinder(TCHAR* viewFinderOn, TCHAR* outputString);
MIDDLEWARE_API bool CliDisplay(TCHAR* onOrOff, TCHAR* outputString);
MIDDLEWARE_API bool CliDisplayBacklight(TCHAR* backLightOn, TCHAR* outputString);
MIDDLEWARE_API bool Clock(TCHAR* outputString);
MIDDLEWARE_API bool Dtmf(TCHAR* tone, TCHAR* volume, TCHAR* audioState, TCHAR* outputString);
//MIDDLEWARE_API bool EnableTSB();
MIDDLEWARE_API bool Flip(TCHAR* outputString);
MIDDLEWARE_API bool FunLight(TCHAR* ledColor, TCHAR* lefOn, TCHAR* outputString);
MIDDLEWARE_API bool HeadSet(TCHAR* outputString);
MIDDLEWARE_API bool HeadsetLoopback(TCHAR* loopbackState, TCHAR* outputString);
MIDDLEWARE_API bool InternalCameraImage(TCHAR* cameraState, TCHAR* outputString);
MIDDLEWARE_API bool InternalCameraViewfinder(TCHAR* viewFinderOn, TCHAR* outputString);
MIDDLEWARE_API bool INVMMasterClear(TCHAR* outputString);
MIDDLEWARE_API bool KeypadBacklight(TCHAR* backLightOn, TCHAR* outputString);
MIDDLEWARE_API bool LedStatusLight(TCHAR* ledColor, TCHAR* outputString);
MIDDLEWARE_API bool LightSensor(TCHAR* outputString);
MIDDLEWARE_API bool MainDisplay(TCHAR* onOrOff, TCHAR* outputString);
MIDDLEWARE_API bool MainDisplayBacklight(TCHAR* backLightOn, TCHAR* outputString);
MIDDLEWARE_API bool MemoryCardDetect(TCHAR* suspend, TCHAR* outputString);
MIDDLEWARE_API bool MemoryCardRead(TCHAR* restart, TCHAR* outputString);
//MIDDLEWARE_API bool MFTVibrator();
MIDDLEWARE_API bool OpProfLogger(TCHAR* purge, TCHAR* outputString);
MIDDLEWARE_API bool PanicAccumulatorLogger(TCHAR* purge, TCHAR* outputString);
MIDDLEWARE_API bool PanicCodeReader(TCHAR* outputString);
MIDDLEWARE_API bool Ping(TCHAR* outputString);
MIDDLEWARE_API bool PowerOff(TCHAR* outputString);
MIDDLEWARE_API bool PressKey(TCHAR* keyCode, TCHAR* outputString);
MIDDLEWARE_API bool ReadBand(TCHAR* outputString);
MIDDLEWARE_API bool ReadBootLoaderVersion(TCHAR* outputString);
MIDDLEWARE_API bool ReadChargingStatus(TCHAR* chargeType, TCHAR* outputString);
MIDDLEWARE_API bool ReadESN(TCHAR* outputString);
MIDDLEWARE_API bool ReadFactoryBoardAssembly(TCHAR* outputString);
MIDDLEWARE_API bool ReadFactoryByte(TCHAR* outputString);
MIDDLEWARE_API bool ReadFactoryLastStationDateTime(TCHAR* outputString);
MIDDLEWARE_API bool ReadFactoryRadioAssembly(TCHAR* outputString);
MIDDLEWARE_API bool ReadFactoryTrackId(TCHAR* outputString);
MIDDLEWARE_API bool ReadFlexVersion(TCHAR* outputString);
MIDDLEWARE_API bool ReadGatewayIP(TCHAR* outputString);
//MIDDLEWARE_API bool ReadIMEI();
MIDDLEWARE_API bool ReadKeypress(TCHAR* outputString);
MIDDLEWARE_API bool ReadLifetimeTimer(TCHAR* outputString);
MIDDLEWARE_API bool ReadLockCode(TCHAR* outputString);
MIDDLEWARE_API bool ReadMEID(TCHAR* outputString);
MIDDLEWARE_API bool ReadMin(TCHAR* outputString);
MIDDLEWARE_API bool ReadModel(TCHAR* outputString);
MIDDLEWARE_API bool ReadMotoXPRSLogs(TCHAR* outputString);
MIDDLEWARE_API bool ReadPrlVersion(TCHAR* outputString);
MIDDLEWARE_API bool ReadProductionState(TCHAR* outputString);
MIDDLEWARE_API bool ReadSecurityCode(TCHAR* outputString);
MIDDLEWARE_API bool ReadSWVersion(TCHAR* outputString);
MIDDLEWARE_API bool ReadWapIPAddress(TCHAR* outputString);
MIDDLEWARE_API bool Restart(TCHAR* outputString);
MIDDLEWARE_API bool RFProbeConnected(TCHAR* outputString);
MIDDLEWARE_API bool VerifyBluetooth(TCHAR* outputString);
MIDDLEWARE_API bool Vibrator(TCHAR* vibratoeOn, TCHAR* outputString);
MIDDLEWARE_API bool WriteFactoryByte(TCHAR* factoryByte, TCHAR* outputString);
MIDDLEWARE_API bool WriteLifetimeTimer(TCHAR* livetime, TCHAR* outputString);
MIDDLEWARE_API bool WriteLockCode(TCHAR* lockCode, TCHAR* outputString);
MIDDLEWARE_API bool WriteSecurityCode(TCHAR* securityCode, TCHAR* outputString);


//  [5/22/2008 Eddy.Guo]
// Test & Calibration Sequences
MIDDLEWARE_API bool BatteryPhase(TCHAR* outputString);
MIDDLEWARE_API bool BlutoothPhase(TCHAR* outputString);
MIDDLEWARE_API int Calibration(TCHAR* band, TCHAR* outputString, int nStartEnd);
MIDDLEWARE_API int CallTest(TCHAR* band, TCHAR* outputString, bool bNotOutput);
MIDDLEWARE_API bool ChargerPhase(TCHAR* outputString);
MIDDLEWARE_API bool CompletePhase(TCHAR* band, TCHAR* outputString);
MIDDLEWARE_API bool CompletePhaseAndCallTest(TCHAR* band, TCHAR* outputString);
MIDDLEWARE_API bool EdgeCallTest(TCHAR* band, TCHAR* outputString);
MIDDLEWARE_API bool EVDOCallTest(TCHAR* band, TCHAR* outputString);
MIDDLEWARE_API bool Fre1Phase(TCHAR* band, TCHAR* outputString);
MIDDLEWARE_API bool KeypadTest(TCHAR* outputString);
MIDDLEWARE_API bool MediaFloTest(TCHAR* outputString);
MIDDLEWARE_API bool RXPhase(TCHAR* band, TCHAR* outputString);
MIDDLEWARE_API bool TXPhase(TCHAR* band, TCHAR* outputString);
//MIDDLEWARE_API bool VerifyPhasingTightLimits();
//MIDDLEWARE_API bool VerifyPhasingWideLimits();


//  [5/22/2008 Eddy.Guo]
// Others
MIDDLEWARE_API bool EFS_ReadFile(const TCHAR *sFileSrc, const TCHAR *sFileDst);
MIDDLEWARE_API bool EFS_WriteFile(const TCHAR *sFileSrc, const TCHAR *sFileDst);
MIDDLEWARE_API bool EFS_DeleteFile(const TCHAR *sFile);
MIDDLEWARE_API bool EFS_Rename(const TCHAR *sOldName, const TCHAR* sNewName);
MIDDLEWARE_API bool EFS_NewFolder(const TCHAR *sFolder);
MIDDLEWARE_API bool EFS_DeleteFolder(const TCHAR *sFolder);
MIDDLEWARE_API bool EFS_QueryFDStatus();
MIDDLEWARE_API bool EFS_QueryFSStatus(const TCHAR *sFile);
MIDDLEWARE_API bool EFS_QueryFileStatus(const TCHAR *sFile);
MIDDLEWARE_API bool EFS_CreateLink(const TCHAR *sFile, const TCHAR* sLinkFile);
MIDDLEWARE_API bool EFS_ReadLink(const TCHAR *sLinkFile, TCHAR* sFile);
MIDDLEWARE_API bool EFS_Chomd(const TCHAR *sFile, const DWORD iMode);
MIDDLEWARE_API bool EFS_CheckAccessAttribute(const TCHAR *sFile, const WORD iPermissionBits);
MIDDLEWARE_API bool EFS_ListFolder(const TCHAR *sFolder, std::vector<Efs2DirItem> *pDirList, DWORD &iSize,BYTE entryType);
MIDDLEWARE_API bool Audio_SetPath(const WORD iAudioDevice);
MIDDLEWARE_API bool Audio_SetVolume(const BYTE iVolume);
MIDDLEWARE_API bool Audio_DSPLoopBack(const bool bOn);
MIDDLEWARE_API bool Audio_PCMLoopBack(const bool bOn);
MIDDLEWARE_API bool Audio_NSControl(const bool bOn);
MIDDLEWARE_API bool Audio_TonesPlay(const WORD iHiFreq, const WORD iLoFreq, const WORD iMethod);
MIDDLEWARE_API bool Audio_TonesStop();
MIDDLEWARE_API bool Audio_SetAdjustCode(const short iCodecTxGainAdjust);
MIDDLEWARE_API bool Audio_PCMCaptureStart(const WORD iNumBuffers, const WORD iDelays);
MIDDLEWARE_API bool Audio_PCMCaptureStop();
MIDDLEWARE_API bool Audio_PCMCaptureGetData(const DWORD iOffset, const WORD iDataSize, char* pBuffer);
MIDDLEWARE_API bool Video_GetInfo(WORD& iSensorWidth, WORD& iSensorHeight, WORD& iFPS, BYTE& iSensorRotationSensing, WORD& iDefaultRotation, \
				WORD& iDefaultOrientation, WORD& iCameraStatus);
MIDDLEWARE_API bool Video_GetParam(const WORD iParmId, DWORD& iMininumValue, DWORD& iMaximumValue, DWORD& iStepValue,DWORD& iDefaultValue, DWORD& iCurrentValue, WORD& iCameraStatus);
MIDDLEWARE_API bool Video_GetPicture(const DWORD iDataOffset, WORD& iDataSize, TCHAR* piPicData, WORD& iCameraStatus);
MIDDLEWARE_API bool Video_PowerOff(WORD& iCameraStatus);
MIDDLEWARE_API bool Video_PowerOn(WORD& iCameraStatus);
MIDDLEWARE_API bool Video_PreviewStart(WORD& iCameraStatus);
MIDDLEWARE_API bool Video_PreviewStop(WORD& iCameraStatus);
MIDDLEWARE_API bool Video_SetDimension(const WORD iPictureWidth, const WORD iPictureHeight, const WORD iLCD_Width, WORD& iCameraStatus);
MIDDLEWARE_API bool Video_SetParam(const WORD iParmId,const DWORD iParmValue,WORD& iCameraStatus);
MIDDLEWARE_API bool Video_SetSensorId(const BYTE iSensorID, WORD& iCameraStatus);
MIDDLEWARE_API bool Video_TakePicture(const DWORD iQuality, const WORD iFormat,DWORD& iPicDataSize, WORD& iCameraStatus);

//  [5/31/2008 Eddy.Guo]
MIDDLEWARE_API bool OpenBackupRestoreDialog(HWND hWnd);
// 2008/08/14
MIDDLEWARE_API bool OpenMultiDownload(HWND hWnd);
//  [10/20/2008 Eddy.Guo]
MIDDLEWARE_API bool OpenRestoreBRTDialog(HWND hWnd);
//  [4/17/2009 Eddy.Guo]
MIDDLEWARE_API bool AddFailure(HWND hWnd);
MIDDLEWARE_API bool GSensorCalibration(HWND hWnd);

#endif