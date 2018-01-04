#include "StdAfx.h"
#include "Audio.h"
#include "..\..\RequestManager\Request\AudioRequest.h"


CAudio::CAudio(int nCOMPort):CInterfaceBase(nCOMPort)
{
}

CAudio::~CAudio(void)
{
}

bool CAudio::EnableDiagEvent(bool bEnable)
{
	return CInterfaceBase::EnableDiagEvent(bEnable);
}

/******************************************************************************/
/*  Audio Set Path command
	The audio set path command activates a supported audio device in the phone. The audio device
	refers to the physical connection of a microphone/speaker or another audio input/output hardware
	to one of the vocoder’s PCM paths. Typical audio devices are handset, headset, and Bluetooth
	headset.

	\param  iAudioDevice - set the receive/transmit pair of audio devices

	\code
		0 = Handset
		1 = Handsfree kit
		2 = Mono headset
		3 = Stereo headset
		4 = Analog handsfree kit
		5 = Stereo DAC
		6 = Speaker phone
		7 = TTY Handsfree kit
		8 = TTY headset
		9 = TTY VCO
		10 = TTY HCO
		11 = Bluetooth intercom
		12 = Bluetooth headset
		13 = Bluetooth local audio
		14 = USB audio
		15 = SND_DEVICE_FM_RADIO_HEADSET
		16 = SND_DEVICE_FTM_HANDSET
		17 = SND_DEVICE_FTM_HEADSET
		18 = SND_DEVICE_FTM_SPEAKER
		19 = SND_DEVICE_FTM_CARKIT
	\endcode

	\return true if successful, false if fail.

	\warning 
*/
/******************************************************************************/
bool CAudio::Audio_SetPath(const WORD iAudioDevice, long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CAudioSetPathRequest(iAudioDevice);	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		TCHAR *pBuffer = (TCHAR*)ptBufferReceived->GetData();

		iErrorCode = pBuffer[10] + (pBuffer[11] << 8) ;

		if(iErrorCode != DIAG_FTM_STATUS_SUCCESS)
			bRes = false;
	}

	return bRes;
}

/******************************************************************************/
/*	Audio Set Volume command
	The audio set volume command sets the RXvolume on the current audio path to the specified
		level (reference Figure 2-1). The volume steps are defined in arrays corresponding to the sound
		device and method in sndcal.c. Volume steps are from 0? (0 being mute and 4 being the
		loudest).	

	\param iVolume - Volume steps from 0?

	\return true if successful, false if fail.

	\warning 
*/
/******************************************************************************/
bool CAudio::Audio_SetVolume(const BYTE iVolume, long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CAudioSetVolumeRequest(iVolume);	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		TCHAR *pBuffer = (TCHAR*)ptBufferReceived->GetData();

		iErrorCode = pBuffer[10] + (pBuffer[11] << 8) ;

		if(iErrorCode != DIAG_FTM_STATUS_SUCCESS)
			bRes = false;
	}

	return bRes;
}

/******************************************************************************/
/**
	Audio DSP Loopback command

	The DSP loopback command can be used to perform an audio loopback in the DSP. After this
		command is sent, audio samples from the microphone specified in the current audio path are
		routed to the corresponding speaker.

	\param bLoopbackOn - unsigned charean to switch DSP loopback on or off. 0 = Off, 1 = On

	\return true if successful, false if fail.

	\warning 
*/
/******************************************************************************/
bool CAudio::Audio_DSPLoopBack(const bool bLoopbackOn, long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CAudioDSPLoopbackRequest(bLoopbackOn);	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		TCHAR *pBuffer = (TCHAR*)ptBufferReceived->GetData();

		iErrorCode = pBuffer[10] + (pBuffer[11] << 8) ;

		if(iErrorCode != DIAG_FTM_STATUS_SUCCESS)
			bRes = false;
	}

	return bRes;
}

/******************************************************************************/
/**
Audio PCM Loopback command
	The PCM loopback command can be used to perform a PCM loopback in the codec. Once this
		command is sent, audio samples from the microphone specified in the current audio path are
		looped back in the codec to the corresponding speaker.

	\param bLoopbackOn  - unsigned charean to switch PCM loopback on or off. 0 = Off, 1 = On

	\return true if successful, false if fail.

	\warning 
*/
/******************************************************************************/
bool CAudio::Audio_PCMLoopBack(const bool bLoopbackOn, long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CAudioPCMLoopbackRequest(bLoopbackOn);	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		TCHAR *pBuffer = (TCHAR*)ptBufferReceived->GetData();

		iErrorCode = pBuffer[10] + (pBuffer[11] << 8) ;

		if(iErrorCode != DIAG_FTM_STATUS_SUCCESS)
			bRes = false;
	}

	return bRes;
}

/******************************************************************************/
/******************************************************************************/
bool CAudio::Audio_TonesPlay(const WORD iHiFreq , const WORD iLoFreq , const WORD iMethod, long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CAudioTonesPlayRequest(iHiFreq, iLoFreq, iMethod);	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		TCHAR *pBuffer = (TCHAR*)ptBufferReceived->GetData();

		iErrorCode = pBuffer[10] + (pBuffer[11] << 8) ;

		if(iErrorCode != DIAG_FTM_STATUS_SUCCESS)
			bRes = false;
	}

	return bRes;
}

/******************************************************************************/
/******************************************************************************/
bool CAudio::Audio_TonesStop(long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CAudioTonesStopRequest();	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		TCHAR *pBuffer = (TCHAR*)ptBufferReceived->GetData();

		iErrorCode = pBuffer[10] + (pBuffer[11] << 8) ;

		if(iErrorCode != DIAG_FTM_STATUS_SUCCESS)
			bRes = false;
	}

	return bRes;
}

/******************************************************************************/
/*	Audio Noise Suppressor command
	The noise suppressor command can be used to turn the noise suppressor on or off.

	\param bNoiseSuppressorOn - 0 = Off, 1 = On (the default state)

	\return true if successful, false if fail.
*/
/******************************************************************************/

bool CAudio::Audio_NSControl(const bool bNoiseSuppressorOn, long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CAudioNSControlRequest(bNoiseSuppressorOn);	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		TCHAR *pBuffer = (TCHAR*)ptBufferReceived->GetData();

		iErrorCode = pBuffer[10] + (pBuffer[11] << 8) ;

		if(iErrorCode != DIAG_FTM_STATUS_SUCCESS)
			bRes = false;
	}

	return bRes;
}

/******************************************************************************/
/*	Audio PCM Capture
	The noise suppressor command can be used to turn the noise suppressor on or off.

	\param iNumBuffers - Number of buffers to be captured.  Can be from 1 to 100.
	                     Each buffer is 160 samples, 1 sample = 16 bits.  
						 100 buffers = 100 * 160 words = 100 * 160 * 2 byte = 32,000 bytes

	\return true if successful, false if fail.

	\warning Audio_PCMStopCapture must be called after the PCM data is transferred to the PC
*/
/******************************************************************************/
bool CAudio::Audio_PCMCaptureStart(const WORD iNumBuffers, const WORD iDelays, long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CAudioPCMCatureRequest(iNumBuffers);	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		TCHAR *pBuffer = (TCHAR*)ptBufferReceived->GetData();

		iErrorCode = pBuffer[10] + (pBuffer[11] << 8) ;

		if(iErrorCode != DIAG_FTM_STATUS_SUCCESS)
			bRes = false;
	}

	return bRes;
}

/******************************************************************************/
/*
	Turns off the PCM Capture.  Must be called after Audio_PCMCaptureStart.
	
	\return true if successful, false if fail.

	\warning 
*/
/******************************************************************************/
bool CAudio::Audio_PCMCaptureStop(long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CAudioPCMCatureStopRequest();	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		TCHAR *pBuffer = (TCHAR*)ptBufferReceived->GetData();

		iErrorCode = pBuffer[10] + (pBuffer[11] << 8) ;

		if(iErrorCode != DIAG_FTM_STATUS_SUCCESS)
			bRes = false;
	}

	return bRes;
}
/******************************************************************************/
/*
	Get the PCM Capture Data.
	
	\return true if successful, false if fail.

	\warning 
*/
/******************************************************************************/

bool CAudio::Audio_GetPCMCaptureData(const DWORD iOffset, const WORD iDataSize, TCHAR* pBuffer, long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CAudioGetPCMCatureDataRequest(iOffset,iDataSize );	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		char *pBufferTemp = (char*)ptBufferReceived->GetData();

		iErrorCode = pBufferTemp[10] + (pBufferTemp[11] << 8) ;
		unsigned long iOffset_rsp = pBufferTemp[12] + (pBufferTemp[13] << 8) + (pBufferTemp[14] << 16) + (pBufferTemp[15] << 24);
		unsigned long iDataSize_rsp = pBufferTemp[16] + (pBufferTemp[17] << 8) ;

		if((iErrorCode != DIAG_FTM_STATUS_SUCCESS) || (iOffset_rsp != iOffset))
			bRes = false;

		memcpy(pBuffer, pBufferTemp+18, iDataSize_rsp);
	}

	return bRes;
}

bool CAudio::Audio_SetCodectxgain_Adjust(short iCodecTxGainAdjust,long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CAudioSetCodecTxGainRequest(iCodecTxGainAdjust);	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		char *pBufferTemp = (char*)ptBufferReceived->GetData();

		iErrorCode = pBufferTemp[10] + (pBufferTemp[11] << 8) ;

		if((iErrorCode != DIAG_FTM_STATUS_SUCCESS))
			bRes = false;

	}

	return bRes;
}

/*bool CAudio::AudioTest(short iTestPath)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	//iTestPath == 1 Main Audio Path
	//iTestPath == 2 Lound Audio Path
	CRequestPtr ptRequest = new CKeyPressRequest(HS_STAR_K, 0);	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		char *pBufferTemp = (char*)ptBufferReceived->GetData();

//		iErrorCode = pBufferTemp[10] + (pBufferTemp[11] << 8) ;

//		if((iErrorCode != DIAG_FTM_STATUS_SUCCESS))
//			bRes = false;

	}
//	delete ptRequest;

//	ptRequest = new CKeyPressRequest(HS_POUND_K, 0);

	return bRes;

}*/