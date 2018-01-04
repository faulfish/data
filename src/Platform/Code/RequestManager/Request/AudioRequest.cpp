#include "stdafx.h"
#include "AudioRequest.h"

namespace PLATFORM_TOOL
{
	CAudioSetPathRequest::CAudioSetPathRequest(WORD iAudioDevice)
	{
		TCHAR szBuf[20];
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_FTM;
		szBuf[2] = (BYTE)(FTM_MODE_ID_AUDIO & 0xFF);
		szBuf[3] = (BYTE)((FTM_MODE_ID_AUDIO >> 8) & 0xFF);
		szBuf[4] = (BYTE)(_FTM_AUDIO_SET_PATH & 0xFF);
		szBuf[5] = (BYTE)((_FTM_AUDIO_SET_PATH >> 8) & 0xFF);
		szBuf[6] = 0x02;
		szBuf[7] = 0x00;
		szBuf[8] = 0x0C;
		szBuf[9] = 0x00;
		szBuf[10] = (BYTE)(iAudioDevice & 0xFF);
		szBuf[11] = (BYTE)((iAudioDevice >> 8 )& 0xFF);

		CBufferPtr ptBufferSend;
		Encode(szBuf, 12, ptBufferSend);
		SetSendData(ptBufferSend);

	}

	CAudioSetPathRequest::~CAudioSetPathRequest()
	{
	}

	CAudioSetVolumeRequest::CAudioSetVolumeRequest(const BYTE iVolume)
	{
		TCHAR szBuf[20];
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_FTM;
		szBuf[2] = (BYTE)(FTM_MODE_ID_AUDIO & 0xFF);
		szBuf[3] = (BYTE)((FTM_MODE_ID_AUDIO >> 8) & 0xFF);
		szBuf[4] = (BYTE)(_FTM_AUDIO_SET_VOLUME & 0xFF);
		szBuf[5] = (BYTE)((_FTM_AUDIO_SET_VOLUME >> 8) & 0xFF);
		szBuf[6] = 0x01;
		szBuf[7] = 0x00;
		szBuf[8] = 0x0C;
		szBuf[9] = 0x00;
		szBuf[10] = (BYTE) iVolume;

		CBufferPtr ptBufferSend;
		Encode(szBuf, 11, ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CAudioSetVolumeRequest::~CAudioSetVolumeRequest()
	{
	}

	CAudioDSPLoopbackRequest::CAudioDSPLoopbackRequest(bool bLoopbackON)
	{
		TCHAR szBuf[20];
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_FTM;
		szBuf[2] = (BYTE)(FTM_MODE_ID_AUDIO & 0xFF);
		szBuf[3] = (BYTE)((FTM_MODE_ID_AUDIO >> 8) & 0xFF);
		szBuf[4] = (BYTE)(_FTM_AUDIO_DSP_LOOPBACK & 0xFF);
		szBuf[5] = (BYTE)((_FTM_AUDIO_DSP_LOOPBACK >> 8) & 0xFF);
		szBuf[6] = 0x01;
		szBuf[7] = 0x00;
		szBuf[8] = 0x0C;
		szBuf[9] = 0x00;
		szBuf[10] = (BYTE)bLoopbackON;

		CBufferPtr ptBufferSend;
		Encode(szBuf, 11, ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CAudioDSPLoopbackRequest::~CAudioDSPLoopbackRequest()
	{
	}

	CAudioPCMLoopbackRequest::CAudioPCMLoopbackRequest(bool bLoopbackON)
	{
		TCHAR szBuf[20];
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_FTM;
		szBuf[2] = (BYTE)(FTM_MODE_ID_AUDIO & 0xFF);
		szBuf[3] = (BYTE)((FTM_MODE_ID_AUDIO >> 8) & 0xFF);
		szBuf[4] = (BYTE)(_FTM_AUDIO_PCM_LOOPBACK & 0xFF);
		szBuf[5] = (BYTE)((_FTM_AUDIO_PCM_LOOPBACK >> 8) & 0xFF);
		szBuf[6] = 0x01;
		szBuf[7] = 0x00;
		szBuf[8] = 0x0C;
		szBuf[9] = 0x00;
		szBuf[10] = (BYTE)bLoopbackON;

		CBufferPtr ptBufferSend;
		Encode(szBuf, 11, ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CAudioPCMLoopbackRequest::~CAudioPCMLoopbackRequest()
	{
	}

	CAudioTonesPlayRequest::CAudioTonesPlayRequest(const WORD iHiFreq, const WORD iLoFreq, const WORD iMethod)
	{
		TCHAR szBuf[20];
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_FTM;
		szBuf[2] = (BYTE)(FTM_MODE_ID_AUDIO & 0xFF);
		szBuf[3] = (BYTE)((FTM_MODE_ID_AUDIO >> 8) & 0xFF);
		szBuf[4] = (BYTE)(_FTM_AUDIO_TONES_PLAY & 0xFF);
		szBuf[5] = (BYTE)((_FTM_AUDIO_TONES_PLAY >> 8) & 0xFF);
		szBuf[6] = 0x04;
		szBuf[7] = 0x00;
		szBuf[8] = 0x0C;
		szBuf[9] = 0x00;
		szBuf[10] = (BYTE)(iHiFreq & 0xFF);
		szBuf[11] = (BYTE)((iHiFreq >> 8 )& 0xFF);
		szBuf[12] = (BYTE)(iLoFreq & 0xFF);
		szBuf[13] = (BYTE)((iLoFreq >> 8 )& 0xFF);
		szBuf[14] = (BYTE)(iMethod & 0xFF);
		szBuf[15] = (BYTE)((iMethod >> 8 )& 0xFF);

		CBufferPtr ptBufferSend;
		Encode(szBuf, 16, ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CAudioTonesPlayRequest::~CAudioTonesPlayRequest()
	{
	}

	CAudioTonesStopRequest::CAudioTonesStopRequest(void)
	{
		TCHAR szBuf[20];
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_FTM;
		szBuf[2] = (BYTE)(FTM_MODE_ID_AUDIO & 0xFF);
		szBuf[3] = (BYTE)((FTM_MODE_ID_AUDIO >> 8) & 0xFF);
		szBuf[4] = (BYTE)(_FTM_AUDIO_TONES_STOP & 0xFF);
		szBuf[5] = (BYTE)((_FTM_AUDIO_TONES_STOP >> 8) & 0xFF);
		szBuf[6] = 0x00;
		szBuf[7] = 0x00;
		szBuf[8] = 0x0C;
		szBuf[9] = 0x00;

		CBufferPtr ptBufferSend;
		Encode(szBuf, 10, ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CAudioTonesStopRequest::~CAudioTonesStopRequest()
	{
	}

	CAudioNSControlRequest::CAudioNSControlRequest(const bool bEnable)
	{
		TCHAR szBuf[20];
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_FTM;
		szBuf[2] = (BYTE)(FTM_MODE_ID_AUDIO & 0xFF);
		szBuf[3] = (BYTE)((FTM_MODE_ID_AUDIO >> 8) & 0xFF);
		szBuf[4] = (BYTE)(_FTM_AUDIO_NS_CONTROL & 0xFF);
		szBuf[5] = (BYTE)((_FTM_AUDIO_NS_CONTROL >> 8) & 0xFF);
		szBuf[6] = 0x01;
		szBuf[7] = 0x00;
		szBuf[8] = 0x0C;
		szBuf[9] = 0x00;
		szBuf[10] = (BYTE)bEnable;

		CBufferPtr ptBufferSend;
		Encode(szBuf, 11, ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CAudioNSControlRequest::~CAudioNSControlRequest()
	{
	}

	CAudioPCMCatureRequest::CAudioPCMCatureRequest(const WORD iNumBuffers)
	{
		TCHAR szBuf[20];
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_FTM;
		szBuf[2] = (BYTE)(FTM_MODE_ID_AUDIO & 0xFF);
		szBuf[3] = (BYTE)((FTM_MODE_ID_AUDIO >> 8) & 0xFF);
		szBuf[4] = (BYTE)(_FTM_AUDIO_PCM_CAPTURE & 0xFF);
		szBuf[5] = (BYTE)((_FTM_AUDIO_PCM_CAPTURE >> 8) & 0xFF);
		szBuf[6] = 0x02;
		szBuf[7] = 0x00;
		szBuf[8] = 0x0C;
		szBuf[9] = 0x00;
		szBuf[10] = (BYTE)(iNumBuffers & 0xFF);
		szBuf[11] = (BYTE)((iNumBuffers >> 8) & 0xFF);

		CBufferPtr ptBufferSend;
		Encode(szBuf, 12, ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CAudioPCMCatureRequest::~CAudioPCMCatureRequest()
	{
	}

	CAudioPCMCatureStopRequest::CAudioPCMCatureStopRequest(void)
	{
		TCHAR szBuf[20];
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_FTM;
		szBuf[2] = (BYTE)(FTM_MODE_ID_AUDIO & 0xFF);
		szBuf[3] = (BYTE)((FTM_MODE_ID_AUDIO >> 8) & 0xFF);
		szBuf[4] = (BYTE)(_FTM_AUDIO_PCM_CAPTURE_STOP & 0xFF);
		szBuf[5] = (BYTE)((_FTM_AUDIO_PCM_CAPTURE_STOP >> 8) & 0xFF);
		szBuf[6] = 0x00;
		szBuf[7] = 0x00;
		szBuf[8] = 0x0C;
		szBuf[9] = 0x00;

		CBufferPtr ptBufferSend;
		Encode(szBuf, 10, ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CAudioPCMCatureStopRequest::~CAudioPCMCatureStopRequest()
	{
	}

	CAudioGetPCMCatureDataRequest::CAudioGetPCMCatureDataRequest(const DWORD iDataOffset, const WORD iDataSize)
	{
		TCHAR szBuf[20];
		WORD size = 10+4+2+iDataSize;

		szBuf[0] =(BYTE) DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_FTM;
		szBuf[2] = (BYTE)(FTM_MODE_ID_AUDIO & 0xFF);
		szBuf[3] = (BYTE)((FTM_MODE_ID_AUDIO >> 8) & 0xFF);
		szBuf[4] = (BYTE)(_FTM_AUDIO_GET_PCM_CAPTURE_DATA & 0xFF);
		szBuf[5] = (BYTE)((_FTM_AUDIO_GET_PCM_CAPTURE_DATA >> 8) & 0xFF);
		szBuf[6] = 0x02;
		szBuf[7] = 0x00;
		szBuf[8] = (BYTE)(size & 0xFF);
		szBuf[9] = (BYTE)((size >> 8) & 0xFF);
		szBuf[10] = (BYTE)(iDataOffset & 0xFF);
		szBuf[11] = (BYTE)((iDataOffset>>8) & 0xFF);
		szBuf[12] = (BYTE)((iDataOffset>>16) & 0xFF);
		szBuf[13] = (BYTE)((iDataOffset>>24) & 0xFF);
		szBuf[14] = (BYTE)(iDataSize & 0xFF);
		szBuf[15] = (BYTE)((iDataSize >> 8) & 0xFF);


		CBufferPtr ptBufferSend;
		Encode(szBuf, 16, ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CAudioGetPCMCatureDataRequest::~CAudioGetPCMCatureDataRequest()
	{
	}

	CAudioSetCodecTxGainRequest::CAudioSetCodecTxGainRequest(const WORD iCodecTxGainAdjust)
	{
		TCHAR szBuf[20];
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_FTM;
		szBuf[2] = (BYTE)(FTM_MODE_ID_AUDIO & 0xFF);
		szBuf[3] = (BYTE)((FTM_MODE_ID_AUDIO >> 8) & 0xFF);
		szBuf[4] = (BYTE)(_FTM_AUDIO_SET_CODECTXGAIN_ADJ & 0xFF);
		szBuf[5] = (BYTE)((_FTM_AUDIO_SET_CODECTXGAIN_ADJ >> 8) & 0xFF);
		szBuf[6] = 0x02;
		szBuf[7] = 0x00;
		szBuf[8] = 0x0C;
		szBuf[9] = 0x00;
		szBuf[10] = (BYTE)(iCodecTxGainAdjust & 0xFF);
		szBuf[11] = (BYTE)((iCodecTxGainAdjust >> 8) & 0xFF);

		CBufferPtr ptBufferSend;
		Encode(szBuf, 12, ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CAudioSetCodecTxGainRequest::~CAudioSetCodecTxGainRequest()
	{
	}

	CAudioLoopbackRequest::CAudioLoopbackRequest(int nTestCase,int nPayload1,bool& bSuccess)
		:CDiagRequestBase(_T("CAudioLoopbackRequest"))
		,m_bSuccess(bSuccess)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 248 for FM-RDS
		Command	2	Only 1 command, always set 2
		Test case	2	See below
		Payload 1	2	Command parameter1

		Case	Function	Payload
		30	MP3 Playback on	No payload
		31	MP3 Playback Stop	No payload
		32	Turning on Speaker Device	No payload
		33	Turning off Speaker Device	No payload
		34	Setting Speaker Multimedia Level	Payload 1 ¡Ð 1 ~ 10 volume
		35	Setting Speaker Voice Level	Payload 1 ¡Ð 1 ~ 10 volume

		36	Turning on Handset Device	No payload
		37	Turning off Handset Device	No payload
		38	Setting Handset Multimedia Level	Payload 1 ¡Ð 1 ~ 10 volume
		39	Setting Handset Voice Level	Payload 1 ¡Ð 1 ~ 10 volume
		40	Turning on Stereo Headset Device	No payload
		41	Turning off Stereo Headset Device	No payload
		42	Setting Stereo Headset Multimedia Level	Payload 1 ¡Ð 1 ~ 10 volume
		43	Setting Stereo Headset Voice Level	Payload 1 ¡Ð 1 ~ 10 volume
		44	Audio Package Loopback ON	No payload
		45	Package Loopback OFF	No payload
		46	Alarm Tone	Payload 1 ¡Ð always be 1 currently
		*/
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE SUB_SYS_ID;
			short COMMAND;
			short TESTCASE;
			BYTE Payload;
		} diag_request_type;
#pragma pack(pop)
		diag_request_type req = {75,248,2,nTestCase,nPayload1};

		CBufferPtr ptBufferSend;
		Encode((const char*)&req,sizeof(diag_request_type),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CAudioLoopbackRequest::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 248 for FM-RDS
		Command	2	Only 1 command, always set 2
		Result	2	0: success (in RSSI command, this field means RSSI)
		*/
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE SUB_SYS_ID;
			short COMMAND;
			short RESULT;
		} diag_rsp_type;
#pragma pack(pop)
		diag_rsp_type resp = {0};
		memcpy(&resp,ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bSuccess = resp.command_code = 0x4B && resp.RESULT == 0;
		assert(m_bSuccess);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
}
