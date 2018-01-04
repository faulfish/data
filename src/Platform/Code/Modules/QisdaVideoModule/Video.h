#pragma once
#include "..\..\lib\requestutility\interfacebase.h"

#ifdef QISDA_VIDEO_MODULE_EXPORTS
#define QISDA_VIDEO_API extern "C" __declspec(dllexport)
#define QISDA_VIDEO_CLASS_API __declspec(dllexport)
#else
#define QISDA_VIDEO_API extern "C" __declspec(dllimport)
#define QISDA_VIDEO_CLASS_API __declspec(dllimport)
#endif


typedef enum 
{
	CAMERA_SUCCESS,
	CAMERA_INVALID_STATE,
	CAMERA_INVALID_PARM,
	CAMERA_INVALID_FORMAT,
	CAMERA_NO_SENSOR,
	CAMERA_NO_MEMORY,
	CAMERA_NOT_SUPPORTED,
	CAMERA_FAILED,
	CAMERA_INVALID_STAND_ALONE_FORMAT,
	CAMERA_MALLOC_FAILED_STAND_ALONE,
} FTM_Camera_Status_Enum;

class QISDA_VIDEO_CLASS_API CVideo :
	public CInterfaceBase
{
public:
	CVideo(int nCOMPort);
public:
	virtual ~CVideo(void);

public:
	virtual bool EnableDiagEvent(bool bEnable);

public:
	bool Camera_PowerOn(WORD& iCameraStatus);
	bool Camera_PowerOff(WORD& iCameraStatus);
	bool Camera_SetParam(const WORD iParmId,const DWORD iParmValue,WORD& iCameraStatus);
	bool Camera_SetDimension(const WORD iPictureWidth, const WORD iPictureHeight, const WORD iLCD_Width,WORD& iCameraStatus);
	bool Camera_PreviewStart(WORD& iCameraStatus);
	bool Camera_PreviewStop(WORD& iCameraStatus);

	bool Camera_TakePicture(const DWORD iQuality, const WORD iFormat, DWORD& iPicDataSize, WORD& iCameraStatus);
	bool Camera_GetPciture(const DWORD iDataOffset, WORD& iDataSize, TCHAR* piPicData, WORD& iCameraStatus);
	bool Camera_GetParam(const WORD iParmId, DWORD& iMinimumValue, DWORD& iMaximumValue, DWORD& iStepValue,DWORD& iDefaultValue, DWORD& iCurrentValue, WORD& iCameraStatus);
	bool Camera_GetInfo(WORD& iSensorWidth, WORD& iSensorHeight, WORD& iFPS, BYTE& iSensorRotationSensing, \
				WORD& iDefaultRotation,  WORD& iDefaultOrientation,WORD& iCameraStatus);
	bool Camera_SetSensorId(const BYTE iSensorID, WORD& iCameraStatus);
};
