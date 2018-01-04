#include "QualcommRequest.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"

#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif

typedef enum 
{
	_FTM_CAMERA_START				= 0,		//!<' Starts camera services
	_FTM_CAMERA_STOP				= 1,		//!<' Stops camera services
	_FTM_CAMERA_SET_PARM			= 2,		//!<' Sets operational parameters for camera sensor 
	_FTM_CAMERA_SET_DIMENSIONS		= 3,		//!<' Sets camera dimensions
	_FTM_CAMERA_START_PREVIEW		= 4,		//!<' Enters preview state 
	_FTM_CAMERA_STOP_PREVIEW		= 5,		//!<' Exits preview state
	_FTM_CAMERA_TAKE_PICTURE_AND_ENCODE	= 6,	//!<' Takes a picture and encode it
	_FTM_CAMERA_GET_PICTURE			= 7,		//!<' Gets raw or encoded data from phone
	_FTM_CAMERA_GET_PARM			= 8,		//!<' Gets operational parameters for camera sensor 
	_FTM_CAMERA_GET_INFO			= 9,		//!<' Reads the camera information
	_FTM_CAMERA_SET_SENSOR_ID		= 10		//!<' Select the camera sensor
} FTM_Camera_SubCmd_Id_Enum;

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



typedef enum 
{
	CAMERA_PARM_STATE,
	CAMERA_PARM_ACTIVE_CMD,
	CAMERA_PARM_ZOOM,
	CAMERA_PARM_ENCODE_ROTATION,
	CAMERA_PARM_SENSOR_POSITION,
	CAMERA_PARM_CONTRAST,
	CAMERA_PARM_BRIGHTNESS,
	CAMERA_PARM_SHARPNESS,
	CAMERA_PARM_EXPOSURE,
	CAMERA_PARM_WB,
	CAMERA_PARM_EFFECT,
	CAMERA_PARM_AUDIO_FMT,
	CAMERA_PARM_FPS,
	CAMERA_PARM_FLASH,
	CAMERA_PARM_RED_EYE_REDUCTION,
	CAMERA_PARM_NIGHTSHOT_MODE,
	CAMERA_PARM_REFLECT,
	CAMERA_PARM_PREVIEW_MODE,
	CAMERA_PARM_ANTIBANDING,
	CAMERA_PARM_THUMBNAIL_WIDTH,
	CAMERA_PARM_THUMBNAIL_HEIGHT,
	CAMERA_PARM_THUMBNAIL_QUALITY
} FTM_Camera_Param_Enum;

typedef enum 
{
	CAMERA_PREVIEW_MODE_SNAPSHOT,
	CAMERA_PREVIEW_MODE_MOVIE
} FTM_Camera_Param_Preview_Enum;

typedef enum 
{
	CAMERA_ORIENTATION_LANDSCAPE,
	CAMERA_ORIENTATION_PORTRAIT
} FTM_Camera_Orientation_Enum;

typedef enum 
{
	CAMERA_RAW,
	CAMERA_JPEG
} FTM_Camera_Format_Enum;

namespace PLATFORM_TOOL
{
	class RM_CLASS_API CVideoRequest: public CDiagRequestBase
	{
	public:
		CVideoRequest();
		virtual ~CVideoRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CCameraPowerOnRequest: public CDiagRequestBase
	{
	public:
		CCameraPowerOnRequest();
		virtual ~CCameraPowerOnRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CCameraPowerOffRequest: public CDiagRequestBase
	{
	public:
		CCameraPowerOffRequest();
		virtual ~CCameraPowerOffRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CCameraSetParamRequest: public CDiagRequestBase
	{
	public:
		CCameraSetParamRequest(const WORD iParmId,const DWORD iParmValue);
		virtual ~CCameraSetParamRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CCameraSetDimensionRequest: public CDiagRequestBase
	{
	public:
		CCameraSetDimensionRequest(const WORD iPictureWidth, const WORD iPictureHeight, const WORD iLCD_Width);
		virtual ~CCameraSetDimensionRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CCameraPreviewStartRequest: public CDiagRequestBase
	{
	public:
		CCameraPreviewStartRequest();
		virtual ~CCameraPreviewStartRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CCameraPreviewStopRequest: public CDiagRequestBase
	{
	public:
		CCameraPreviewStopRequest();
		virtual ~CCameraPreviewStopRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CCameraTakePictureRequest: public CDiagRequestBase
	{
	public:
		CCameraTakePictureRequest(const DWORD iQuality, const WORD iFormat);
		virtual ~CCameraTakePictureRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CCameraGetPictureRequest: public CDiagRequestBase
	{
	public:
		CCameraGetPictureRequest(const DWORD iDataOffset,const WORD iDataSize);
		virtual ~CCameraGetPictureRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};
	
	class RM_CLASS_API CCameraGetParamRequest: public CDiagRequestBase
	{
	public:
		CCameraGetParamRequest(const WORD iParmId);
		virtual ~CCameraGetParamRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CCameraGetInfoRequest: public CDiagRequestBase
	{
	public:
		CCameraGetInfoRequest();
		virtual ~CCameraGetInfoRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};
	
	class RM_CLASS_API CCameraSetSensorIdRequest: public CDiagRequestBase
	{
	public:
		CCameraSetSensorIdRequest(const BYTE iSensorID);
		virtual ~CCameraSetSensorIdRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};
		

	


}
