#pragma once
#include "..\..\lib\requestutility\interfacebase.h"

#ifdef QISDA_WIRELESS_MODULE_EXPORTS
#define QISDA_WIRELESS_API extern "C" __declspec(dllexport)
#define QISDA_WIRELESS_CLASS_API __declspec(dllexport)
#else
#define QISDA_WIRELESS_API extern "C" __declspec(dllimport)
#define QISDA_WIRELESS_CLASS_API __declspec(dllimport)
#endif

/******************************************************************************
						Definitions for EFS operations
*******************************************************************************/

class QISDA_WIRELESS_CLASS_API CWireless : public CInterfaceBase
{
public:
	CWireless(int nCOMPort);

	//BT
	bool EnableGAP(bool bEnable);
	bool StartBTPower(int nDegree);
	bool EnterBTTestMode(bool bEnable);
	bool WriteHeadsetBTAddress(const TSTRING& strBTAddress, int nTimeout = 3000);   // Write BT Address for earphone
	bool BTBound(bool bBound,const TSTRING& strBTAddress, const TSTRING& strBTPass,int nTimeout) ;

	bool BTHFPGateway(bool bActive);
	bool BTHFPGatewayConnect(bool bActive,const TSTRING& strBTAddress);
	bool BTHFPGatewayAnswer(bool bActive);
	bool BTHFPGatewayAudioConnect(bool bActive);
	bool BTHFPGatewaySpeakerGain();
	bool BTHFPGatewayMicrophoneGain();
	bool BTHFPGatewayAudioLoop(bool bEnable);
	bool BTHFPGatewayRoaming(bool bActive);

	//FM
	bool FMRegistration();
	bool FMConnect(bool bConnect);
	bool FMInitialize();
	bool FMSetStation(WORD nFMFrequence);
	bool FMMute(bool bMute);
	bool FMChangeTestModeAndSetLevel(int nLevel);
	bool FMReceiveRSSI(WORD& nRSSI);
	bool FMSpeaker();
	bool FMMonoHeadset();
	bool FMStereoHeadset();

	//Audio
	bool PlayMP3(bool bEnable);//default MP3 in the EFS
	bool SpeakerDevice(bool bEnable);
	bool SpeakerDeviceMultimediaLevel(int nLevel);
	bool SpeakerDeviceVoiceLevel(int nLevel);
	bool HandsetDevice(bool bEnable);
	bool HandsetDeviceMultimediaLevel(int nLevel);
	bool HandsetDeviceVoiceLevel(int nLevel);
    bool MonoHeadsetDevice(bool bEnable);
	bool MonoHeadsetDeviceMultimediaLevel(int nLevel);
	bool MonoHeadsetDeviceVoiceLevel(int nLevel);
	bool BluetoothDevice(bool bEnable);
	bool BluetoothDeviceMultimediaLevel(int nLevel);
	bool BluetoothDeviceVoiceLevel(int nLevel);
	bool StereoHeadsetDevice(bool bEnable);
	bool StereoHeadsetDeviceMultimediaLevel(int nLevel);
	bool StereoHeadsetDeviceVoiceLevel(int nLevel);
	bool AudioPackageLoopback(bool bEnable);
	bool AlarmTone(int nTone);
public:
	virtual ~CWireless(void);
};
