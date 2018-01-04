#include "QualcommRequest.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "stdio.h"

#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif

typedef enum 
{
	_FTM_AUDIO_SET_PATH				= 0,	//!<'  Sets up the audio path
	_FTM_AUDIO_SET_VOLUME			= 1,	//!<'  Sets up the volume in the current audio path
	_FTM_AUDIO_DSP_LOOPBACK			= 2,	//!<'  Sets up audio loopback in the DSP
	_FTM_AUDIO_PCM_LOOPBACK			= 3,	//!<'  Sets up audio loopback in the codec
	_FTM_AUDIO_TONES_PLAY			= 4,	//!<'  Plays tones at specified frequency
	_FTM_AUDIO_TONES_STOP			= 5,	//!<'  Stops the tones that are currently playing
	_FTM_AUDIO_NS_CONTROL			= 6,	//!<'  Toggles the noise suppressor ON/OFF
	_FTM_AUDIO_PCM_CAPTURE			= 7,	//!<'  Start a PCM capture
	_FTM_AUDIO_GET_PCM_CAPTURE_DATA	= 8,	//!<'  Get data from a PCM capture
	_FTM_AUDIO_PCM_CAPTURE_STOP		= 9,	//!<'  Clear the PCM capture buffers
	_FTM_AUDIO_SET_CODECTXGAIN_ADJ	= 10	//!<'  Set the CodecTxGainAdjust

} FTM_Audio_SubCmd_Id_Enum;


typedef enum 
{
	FTM_MODE_ID_CDMA_1X		= 0,		//!<' RF CDMA 1X mode - RX0
	FTM_MODE_ID_WCDMA		= 1,		//!<' RF WCDMA mode
	FTM_MODE_ID_GSM			= 2,		//!<' RF GSM Mode
	FTM_MODE_ID_CDMA_1X_RX1	= 3,		//!<' RF CDMA 1X mode - RX1
	FTM_MODE_ID_BLUETOOTH	= 4,		//!<' Bluetooth
	FTM_MODE_ID_CDMA_1X_CALL= 7,		//!<' CALL CDMA 1X mode 
	FTM_MODE_ID_HDR_C		= 8,		//!<' HDC non signaling
	FTM_MODE_ID_LOGGING		= 9,		//!<' FTM Logging
	FTM_MODE_ID_AGPS		= 10,		//!<' Async GPS
	FTM_MODE_ID_PMIC        = 11,		//!<' PMIC FTM Command
	FTM_MODE_GSM_BER		= 13,		//!<' GSM BER
	FTM_MODE_ID_AUDIO		= 14,		//!<' Audio FTM
	FTM_MODE_ID_CAMERA		= 15,		//!<' Camera
	FTM_MODE_WCDMA_BER		= 16,		//!<' WCDMA BER
	FTM_MODE_ID_GSM_EXTENDED_C = 17,	//!<' GSM Extended commands
	FTM_MODE_CDMA_API_V2	= 18,		//!<' CDMA RF Cal API v2
	FTM_MODE_ID_MF_C		= 19,		//!<' MediaFLO
	FTM_MODE_RF_COMMON		= 20,		//!<' RF Common
	FTM_MODE_WCDMA_RX1		= 21,		//!<' WCDMA diversity RX (RX1)
	FTM_MODE_ID_PRODUCTION	= 0x8000,	//!<' Production FTM
	FTM_MODE_ID_LTM			= 0x8001	//!<' LTM
} FTM_Mode_Id_Enum;


namespace PLATFORM_TOOL
{
	class RM_CLASS_API CAudioSetPathRequest: public CDiagRequestBase
	{
	public:
		CAudioSetPathRequest(const WORD iAudioDevice);
		virtual ~CAudioSetPathRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CAudioSetVolumeRequest: public CDiagRequestBase
	{
	public:
		CAudioSetVolumeRequest(const BYTE iVolume);
		virtual ~CAudioSetVolumeRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CAudioDSPLoopbackRequest: public CDiagRequestBase
	{
	public:
		CAudioDSPLoopbackRequest(const bool bLoopbackON);
		virtual ~CAudioDSPLoopbackRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CAudioPCMLoopbackRequest: public CDiagRequestBase
	{
	public:
		CAudioPCMLoopbackRequest(const bool bLoopbackON);
		virtual ~CAudioPCMLoopbackRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CAudioTonesPlayRequest: public CDiagRequestBase
	{
	public:
		CAudioTonesPlayRequest(const WORD iHiFreq, const WORD iLoFreq, const WORD iMethod);
		virtual ~CAudioTonesPlayRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CAudioTonesStopRequest: public CDiagRequestBase
	{
	public:
		CAudioTonesStopRequest(void);
		virtual ~CAudioTonesStopRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CAudioNSControlRequest: public CDiagRequestBase
	{
	public:
		CAudioNSControlRequest(const bool bEnable);
		virtual ~CAudioNSControlRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CAudioPCMCatureRequest: public CDiagRequestBase
	{
	public:
		CAudioPCMCatureRequest(const WORD iNumBuffers);
		virtual ~CAudioPCMCatureRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CAudioPCMCatureStopRequest: public CDiagRequestBase
	{
	public:
		CAudioPCMCatureStopRequest();
		virtual ~CAudioPCMCatureStopRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CAudioGetPCMCatureDataRequest: public CDiagRequestBase
	{
	public:
		CAudioGetPCMCatureDataRequest(const DWORD iDataOffset, const WORD iDataSize);
		virtual ~CAudioGetPCMCatureDataRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CAudioSetCodecTxGainRequest: public CDiagRequestBase
	{
	public:
		CAudioSetCodecTxGainRequest(const WORD iCodecTxGainAdjust);
		virtual ~CAudioSetCodecTxGainRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};
	
	class RM_CLASS_API CAudioLoopbackRequest: public CDiagRequestBase
	{
	public:
		CAudioLoopbackRequest(int nTestCase,int nPayload1,bool& bSuccess);
		virtual ~CAudioLoopbackRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
	};
}
