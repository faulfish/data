#pragma once
#include "..\..\lib\requestutility\interfacebase.h"


#ifdef QISDA_AUDIO_MODULE_EXPORTS
#define QISDA_AUDIO_API extern "C" __declspec(dllexport)
#define QISDA_AUDIO_CLASS_API __declspec(dllexport)
#else
#define QISDA_AUDIO_API extern "C" __declspec(dllimport)
#define QISDA_AUDIO_CLASS_API __declspec(dllimport)
#endif

typedef enum 
{
	FTM_AUDIO_PATH_HANDSET			= 0,	//!<'  Handset
	FTM_AUDIO_PATH_HANDSFREE		= 1,	//!<'  Handsfree kit
	FTM_AUDIO_PATH_MONO_HEADSET		= 2,	//!<'  Mono headset
	FTM_AUDIO_PATH_STEREO_HEADSET	= 3,	//!<'  Stereo headset
	FTM_AUDIO_PATH_ANALOG_HANDSFREE	= 4,	//!<'  Analog handsfree kit
	FTM_AUDIO_PATH_STEREO_DAC		= 5,	//!<'  Stereo DAC
	FTM_AUDIO_PATH_SPEAKER_PHONE	= 6,	//!<'  Speaker phone
	FTM_AUDIO_PATH_TTY_HANDSFREE	= 7,	//!<'  TTY Handsfree kit
	FTM_AUDIO_PATH_TTY_HEADSET		= 8,	//!<'  TTY headset
	FTM_AUDIO_PATH_TTY_VCO			= 9,	//!<'  TTY VCO
	FTM_AUDIO_PATH_TTY_HCO			= 10,	//!<'  TTY HCO
	FTM_AUDIO_PATH_BT_INTERCOM		= 11,	//!<'  Bluetooth intercom
	FTM_AUDIO_PATH_BT_HEADSET		= 12,	//!<'  Bluetooth headset
	FTM_AUDIO_PATH_BT_AUDIO			= 13,	//!<'  Bluetooth local audio
	FTM_AUDIO_PATH_USB_AUDIO		= 14,	//!<'  USB audio
	FTM_AUDIO_PATH_FM_MONO_HS		= 15,	//!<'  FM Mono headset
	FTM_AUDIO_PATH_FM_STEREO_HS		= 16,	//!<'  FM Stereo headset
	FTM_AUDIO_PATH_FTM_HEADSET		= 17,   //!<'  SND_DEVICE_FTM_HEADSET
	FTM_AUDIO_PATH_FTM_SPEAKER		= 18,   //!<'  SND_DEVICE_FTM_SPEAKER
	FTM_AUDIO_PATH_FTM_CARKIT		= 19,   //!<'  SND_DEVICE_FTM_CARKIT
} FTM_Audio_Device_Id_Enum;

typedef enum
{		
	FTM_AUDIO_SND_METHOD_VOICE = 0,			//!<'  Use the device's voice generator
	FTM_AUDIO_SND_METHOD_KEY_BEEP = 1,		//!<'  Use the device's keybeep generator              
	FTM_AUDIO_SND_METHOD_MESSAGE = 2,		//!<'  Use the path's ringer, or voice generator       
	FTM_AUDIO_SND_METHOD_RING = 3			//!<'  Use the device's ring generator                 
} FTM_Audio_Sound_Methods_Enum;

#define DIAG_FTM_STATUS_SUCCESS 0

class QISDA_AUDIO_CLASS_API CAudio :
	public CInterfaceBase
{
public:
	CAudio(int nCOMPort);
	virtual ~CAudio(void);

public:
	virtual bool EnableDiagEvent(bool bEnable);

public:
	bool Audio_SetPath(const WORD iAudioDevice, long &iErrorCode);
	bool Audio_SetVolume(const BYTE iVolume, long &iErrorCode);
	bool Audio_DSPLoopBack(const bool bLoopbackOn, long &iErrorCode);
	bool Audio_PCMLoopBack(const bool bLoopbackOn, long &iErrorCode);
	bool Audio_TonesPlay(const WORD iHiFreq , const WORD iLoFreq , const WORD iMethod, long &iErrorCode);
	bool Audio_TonesStop(long &iErrorCode);
	bool Audio_NSControl(const bool bNoiseSuppressorOn, long &iErrorCode);
	bool Audio_PCMCaptureStart(const WORD iNumBuffers, const WORD iDelays, long &iErrorCode);
	bool Audio_PCMCaptureStop(long &iErrorCode);
	bool Audio_GetPCMCaptureData(const DWORD iOffset, const WORD iDataSize, TCHAR* pBuffer, long &iErrorCode);
	bool Audio_SetCodectxgain_Adjust(const short iCodecTxGainAdjust,long &iErrorCode); 

//	bool AudioTest(short iTestPath);
};
