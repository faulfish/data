#include "stdafx.h"
#include "VideoRequest.h"

namespace PLATFORM_TOOL
{
	CVideoRequest::CVideoRequest()
	{
	}

	CVideoRequest::~CVideoRequest()
	{
	}
//*******************************************************************//	
	CCameraPowerOnRequest::CCameraPowerOnRequest()
	{
		char szBuf[20];
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_FTM;
		szBuf[3] = (BYTE)(FTM_MODE_ID_CAMERA&0xFF);
		szBuf[4] = (BYTE)((FTM_MODE_ID_CAMERA>>8)&0xFF);
		szBuf[5] = (BYTE)(_FTM_CAMERA_START&0xFF);
		szBuf[6] = (BYTE)((_FTM_CAMERA_START>>8)&0xFF);
		szBuf[6] = 0x0A;
		szBuf[7] = 0x00;
		szBuf[8] = 0x0C;
		szBuf[9] = 0x00;

		CBufferPtr ptBufferSend;
		Encode(szBuf,10,ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CCameraPowerOnRequest::~CCameraPowerOnRequest()
	{
	}

	CCameraPowerOffRequest::CCameraPowerOffRequest()
	{
		char szBuf[20];
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_FTM;
		szBuf[2] = (BYTE)(FTM_MODE_ID_CAMERA&0xFF);
		szBuf[3] = (BYTE)((FTM_MODE_ID_CAMERA>>8)&0xFF);
		szBuf[4] = (BYTE)(_FTM_CAMERA_STOP&0xFF);
		szBuf[5] = (BYTE)((_FTM_CAMERA_STOP>>8)&0xFF);
		szBuf[6] = 0x0A;
		szBuf[7] = 0x00;
		szBuf[8] = 0x0C;
		szBuf[9] = 0x00;
	
		CBufferPtr ptBufferSend;
		Encode(szBuf,10,ptBufferSend);
		SetSendData(ptBufferSend);

	}

	CCameraPowerOffRequest::~CCameraPowerOffRequest()
	{
	}

	CCameraSetParamRequest::CCameraSetParamRequest(const WORD iParmId,const DWORD iParmValue)
	{
		char szBuf[20];
		szBuf[0] = DIAG_SUBSYS_CMD_F;
		szBuf[1] = DIAG_SUBSYS_FTM;
		szBuf[2] = FTM_MODE_ID_CAMERA;
		szBuf[3] = (char)(FTM_MODE_ID_CAMERA>>8)&0xFF;
		szBuf[4] = _FTM_CAMERA_SET_PARM;
		szBuf[5] = (char)(_FTM_CAMERA_SET_PARM>>8)&0xFF;
		szBuf[6] = 0x10;
		szBuf[7] = 0x00;
		szBuf[8] = 0x0C;
		szBuf[9] = 0x00;
		szBuf[10]= (char)iParmId & 0xFF;
		szBuf[11]= (char)(iParmId >>8 ) & 0xFF;
		szBuf[12]= (char)iParmValue & 0xFF;
		szBuf[13]= (char)(iParmValue >> 8 ) & 0xFF;	
		szBuf[14]= (char)(iParmValue >> 16 ) & 0xFF;	
		szBuf[15]= (char)(iParmValue >> 24 ) & 0xFF;	

		CBufferPtr ptBufferSend;
		Encode(szBuf, 16, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CCameraSetParamRequest::~CCameraSetParamRequest()
	{
	}
	
	CCameraSetDimensionRequest::CCameraSetDimensionRequest(const WORD iPictureWidth,const WORD iPictureHeight, const WORD iLCD_Width)
	{
		char szBuf[20];
		szBuf[0] = DIAG_SUBSYS_CMD_F;
		szBuf[1] = DIAG_SUBSYS_FTM;
		szBuf[2] = FTM_MODE_ID_CAMERA;
		szBuf[3] = (char)(FTM_MODE_ID_CAMERA>>8)&0xFF;
		szBuf[4] = _FTM_CAMERA_SET_DIMENSIONS;
		szBuf[5] = (char)(_FTM_CAMERA_SET_DIMENSIONS>>8)&0xFF;
		szBuf[6] = 0x0E;
		szBuf[7] = 0x00;
		szBuf[8] = 0x0C;
		szBuf[9] = 0x00;
		szBuf[10]= (char)iPictureWidth & 0xFF;
		szBuf[11]= (char)(iPictureWidth >>8 ) & 0xFF;
		szBuf[12]= (char)iPictureHeight & 0xFF;
		szBuf[13]= (char)(iPictureHeight >> 8) & 0xFF;
		szBuf[14]= (char)iLCD_Width & 0xFF;
		szBuf[15]= (char)(iLCD_Width >> 8) & 0xFF;

		CBufferPtr ptBufferSend;
		Encode(szBuf, 16, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CCameraSetDimensionRequest::~CCameraSetDimensionRequest()
	{
	}

	CCameraPreviewStartRequest::CCameraPreviewStartRequest()
	{
		char szBuf[20];
		szBuf[0] = DIAG_SUBSYS_CMD_F;
		szBuf[1] = DIAG_SUBSYS_FTM;
		szBuf[2] = FTM_MODE_ID_CAMERA;
		szBuf[3] = (char)(FTM_MODE_ID_CAMERA>>8)&0xFF;
		szBuf[4] = _FTM_CAMERA_START_PREVIEW;
		szBuf[5] = (char)(_FTM_CAMERA_START_PREVIEW >> 8)&0xFF;
		szBuf[6] = 0x00;
		szBuf[7] = 0x00;
		szBuf[8] = 0x0C;
		szBuf[9] = 0x00;


		CBufferPtr ptBufferSend;
		Encode(szBuf, 10, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CCameraPreviewStartRequest::~CCameraPreviewStartRequest()
	{
	}

	CCameraPreviewStopRequest::CCameraPreviewStopRequest()
	{
		char szBuf[20];
		szBuf[0] = DIAG_SUBSYS_CMD_F;
		szBuf[1] = DIAG_SUBSYS_FTM;
		szBuf[2] = FTM_MODE_ID_CAMERA;
		szBuf[3] = (char)(FTM_MODE_ID_CAMERA>>8)&0xFF;
		szBuf[4] = _FTM_CAMERA_STOP_PREVIEW;
		szBuf[5] = (char)(_FTM_CAMERA_STOP_PREVIEW >> 8)&0xFF;
		szBuf[6] = 0x00;
		szBuf[7] = 0x00;
		szBuf[8] = 0x0C;
		szBuf[9] = 0x00;


		CBufferPtr ptBufferSend;
		Encode(szBuf, 10, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CCameraPreviewStopRequest::~CCameraPreviewStopRequest()
	{
	}
	
	CCameraTakePictureRequest::CCameraTakePictureRequest(const DWORD iQuality, const WORD iFormat)
	{
		char szBuf[20];
		szBuf[0] = DIAG_SUBSYS_CMD_F;
		szBuf[1] = DIAG_SUBSYS_FTM;
		szBuf[2] = FTM_MODE_ID_CAMERA;
		szBuf[3] = (char)(FTM_MODE_ID_CAMERA>>8)&0xFF;
		szBuf[4] = _FTM_CAMERA_TAKE_PICTURE_AND_ENCODE;
		szBuf[5] = (char)(_FTM_CAMERA_TAKE_PICTURE_AND_ENCODE >> 8)&0xFF;
		szBuf[6] = 0x0A;
		szBuf[7] = 0x00;
		szBuf[8] = 0x10;
		szBuf[9] = 0x00;
		szBuf[10] = (char)iQuality & 0xFF;
		szBuf[11] = (char)(iQuality >>8) & 0xFF;
		szBuf[12] = (char)(iQuality >>16) & 0xFF;
		szBuf[13] = (char)(iQuality >>24) & 0xFF;
		szBuf[14] = iFormat & 0xFF;
		szBuf[15] = (char)(iFormat >>8) & 0xFF;
				
		CBufferPtr ptBufferSend;
		Encode(szBuf, 16, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CCameraTakePictureRequest::~CCameraTakePictureRequest()
	{
	}

	CCameraGetPictureRequest::CCameraGetPictureRequest(const DWORD iDataOffset,const WORD iDataSize)
	{
		char szBuf[20];
		szBuf[0] = DIAG_SUBSYS_CMD_F;
		szBuf[1] = DIAG_SUBSYS_FTM;
		szBuf[2] = FTM_MODE_ID_CAMERA;
		szBuf[3] = (char)(FTM_MODE_ID_CAMERA>>8)&0xFF;
		szBuf[4] = _FTM_CAMERA_GET_PICTURE;
		szBuf[5] = (char)(_FTM_CAMERA_GET_PICTURE >> 8)&0xFF;
		szBuf[6] = 0x10;
		szBuf[7] = 0x00;
		szBuf[8] = (0x12 + iDataSize) & 0xFF;
		szBuf[9] = (char)((0x12 + iDataSize) >>8) & 0xFF;
		szBuf[10] = (char)iDataOffset & 0xFF;
		szBuf[11] = (char)(iDataOffset >>8) & 0xFF;
		szBuf[12] = (char)(iDataOffset >>16) & 0xFF;
		szBuf[13] = (char)(iDataOffset >>24) & 0xFF;
		szBuf[14] = iDataSize & 0xFF;
		szBuf[15] = (char)(iDataSize >>8) & 0xFF;


		CBufferPtr ptBufferSend;
		Encode(szBuf, 16, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CCameraGetPictureRequest::~CCameraGetPictureRequest()
	{
	}

	CCameraGetParamRequest::CCameraGetParamRequest(const WORD iParmId)
	{
		char szBuf[20];
		szBuf[0] = DIAG_SUBSYS_CMD_F;
		szBuf[1] = DIAG_SUBSYS_FTM;
		szBuf[2] = FTM_MODE_ID_CAMERA;
		szBuf[3] = (char)(FTM_MODE_ID_CAMERA>>8)&0xFF;
		szBuf[4] = _FTM_CAMERA_GET_PARM;
		szBuf[5] = (char)(_FTM_CAMERA_GET_PARM >> 8)&0xFF;
		szBuf[6] = 0x0C;
		szBuf[7] = 0x00;
		szBuf[8] = 0x0C;
		szBuf[9] = 0x00;
		szBuf[10] = (char)iParmId & 0xFF;
		szBuf[11] = (char)(iParmId >>8) & 0xFF;

		CBufferPtr ptBufferSend;
		Encode(szBuf, 12, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CCameraGetParamRequest::~CCameraGetParamRequest()
	{
	}
	
	CCameraGetInfoRequest::CCameraGetInfoRequest()
	{
		char szBuf[20];
		szBuf[0] = DIAG_SUBSYS_CMD_F;
		szBuf[1] = DIAG_SUBSYS_FTM;
		szBuf[2] = FTM_MODE_ID_CAMERA;
		szBuf[3] = (char)(FTM_MODE_ID_CAMERA>>8)&0xFF;
		szBuf[4] = _FTM_CAMERA_GET_INFO;
		szBuf[5] = (char)(_FTM_CAMERA_GET_INFO >> 8)&0xFF;
		szBuf[6] = 0x0A;
		szBuf[7] = 0x00;
		szBuf[8] = 0x15;
		szBuf[9] = 0x00;

		CBufferPtr ptBufferSend;
		Encode(szBuf, 10, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CCameraGetInfoRequest::~CCameraGetInfoRequest()
	{
	}

	CCameraSetSensorIdRequest::CCameraSetSensorIdRequest(const BYTE iSensorID)
	{
		char szBuf[20];
		szBuf[0] = DIAG_SUBSYS_CMD_F;
		szBuf[1] = DIAG_SUBSYS_FTM;
		szBuf[2] = FTM_MODE_ID_CAMERA;
		szBuf[3] = (char)(FTM_MODE_ID_CAMERA>>8)&0xFF;
		szBuf[4] = _FTM_CAMERA_SET_SENSOR_ID;
		szBuf[5] = (char)(_FTM_CAMERA_SET_SENSOR_ID >> 8)&0xFF;
		szBuf[6] = 0x0B;
		szBuf[7] = 0x00;
		szBuf[8] = 0x0C;
		szBuf[9] = 0x00;
		szBuf[10] = iSensorID;

		CBufferPtr ptBufferSend;
		Encode(szBuf, 11, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CCameraSetSensorIdRequest::~CCameraSetSensorIdRequest()
	{
	}


}
