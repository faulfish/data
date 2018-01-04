#include "StdAfx.h"
#include "Video.h"
#include "..\..\RequestManager\Request\VideoRequest.h"


CVideo::CVideo(int nCOMPort):CInterfaceBase(nCOMPort)
{
}

CVideo::~CVideo(void)
{
}

bool CVideo::EnableDiagEvent(bool bEnable)
{
	return CInterfaceBase::EnableDiagEvent(bEnable);
}

/*******************************************************************************/
/*  Camera Power On Command
	This command starts the camera services. The power to the sensor is turned on and set to
	the power-up state for operation. The sensor and the device driver are initialized. Setting picture
	dimensions and taking pictures cannot happen until this command is executed.
	
	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode

	\return true if successful, false if fail.
	*/
/*******************************************************************************/

bool CVideo::Camera_PowerOn(WORD& iCameraStatus)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CCameraPowerOnRequest();	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		TCHAR *pBuffer = (TCHAR*)ptBufferReceived->GetData();

		iCameraStatus = pBuffer[10] + (pBuffer[11] << 8) ;

		if(iCameraStatus != CAMERA_SUCCESS)
			bRes = false;
	}

	return bRes;
	
}

/******************************************************************************/
/*	Camera Power Off Command
	This command terminates the camera services. The power sensor is turned off. This command
	also de-allocates the memory heap allocated for picture storage.

	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode

	\return true if successful, false if fail.

	\warning 

*******************************************************************************/

bool CVideo::Camera_PowerOff(WORD& iCameraStatus)
{	
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CCameraPowerOffRequest();	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		TCHAR *pBuffer = (TCHAR*)ptBufferReceived->GetData();

		iCameraStatus = pBuffer[10] + (pBuffer[11] << 8) ;

		if(iCameraStatus != CAMERA_SUCCESS)
			bRes = false;
	}

	return bRes;
}

/******************************************************************************/
/*	Camera Set Param Command
	This command sets the operational parameters of the camera sensor. For a list of possible
	parameter identifiers, refer to the Camera Services Interface Specification and Operational
	Description (80-V5310-1).
	
	\param	iParmId = Camera parameter identifier. Refer to 80-V5310-1 for definitions of parameter identifiers.
	\code
        0 = CAMERA_PARM_STATE (Read Only)
        1 = CAMERA_PARM_ACTIVE_CMD (Read Only)
        2 = CAMERA_PARM_ZOOM
        3 = CAMERA_PARM_ENCODE_ROTATION
        4 = CAMERA_PARM_SENSOR_POSITION
        5 = CAMERA_PARM_CONTRAST
        6 = CAMERA_PARM_BRIGHTNESS
        7 = CAMERA_PARM_SHARPNESS
        8 = CAMERA_PARM_EXPOSURE
        9 = CAMERA_PARM_WB
        10 = CAMERA_PARM_EFFECT
        11 = CAMERA_PARM_AUDIO_FMT
        12 = CAMERA_PARM_FPS
        13 = CAMERA_PARM_FLASH
        14 = CAMERA_PARM_RED_EYE_REDUCTION
        15 = CAMERA_PARM_NIGHTSHOT_MODE
        16 = CAMERA_PARM_REFLECT
        17 = CAMERA_PARM_PREVIEW_MODE
        18 = CAMERA_PARM_ANTIBANDING
        19 = CAMERA_PARM_THUMBNAIL_WIDTH
        20 = CAMERA_PARM_THUMBNAIL_HEIGHT
        21 = CAMERA_PARM_THUMBNAIL_QUALITY
	\endcode
	\param	iParmValue = This is the camera parameter value. 
	                     Refer 80-V5310-1 for specific definitions for each parameter identifiers.

	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode

	\return true if successful, false if fail.

	\warning 

*******************************************************************************/

bool CVideo::Camera_SetParam(const WORD iParmId,const DWORD iParmValue,WORD& iCameraStatus)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CCameraSetParamRequest(iParmId,iParmValue);	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		TCHAR *pBuffer = (TCHAR*)ptBufferReceived->GetData();

		iCameraStatus = pBuffer[10] + (pBuffer[11] << 8) ;

		if(iCameraStatus != CAMERA_SUCCESS)
			bRes = false;
	}

	return bRes;
}

/******************************************************************************/
/*	Camera Set Dimensions Command
	This command sets the picture dimensions. The picture width is the horizontal width of the
	picture in number of pixels and the picture height is the vertical height of the picture in number of
	lines. The Camera Start command must be executed before using this command. Refer to Chapter
	3 for recommended picture resolutions.

	\param iPictureWidth - This is the horizontal width of the picture in number of pixels.
	\param iPictureHeight- This is the Vertical height of the picture in number of pixels.
	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode
	\param iLCD_Width = # of pixel on the X axis of the LCD

	\return true if successful, false if fail.

	\warning 

*******************************************************************************/

bool CVideo::Camera_SetDimension(const WORD iPictureWidth, const WORD iPictureHeight, const WORD iLCD_Width, WORD& iCameraStatus)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CCameraSetDimensionRequest(iPictureWidth,iPictureHeight,iLCD_Width);	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		TCHAR *pBuffer = (TCHAR*)ptBufferReceived->GetData();

		iCameraStatus = pBuffer[10] + (pBuffer[11] << 8) ;

		if(iCameraStatus != CAMERA_SUCCESS)
			bRes = false;
	}

	return bRes;
}

/******************************************************************************/
/*	Camera Start Preview Command
	This command enters the preview state of camera services. This command must be executed each
	time the Camera Start or the Camera Set Dimensions command is called.
	NOTE A two second delay is recommended between Camera Start Preview and Camera Stop Preview
	commands in order for white balance and exposure to take effect. There is no software feedback
	mechanism indicating these parameters are ready; two seconds is a reasonable time for the human
	eye, which is the only method to verify this.
	
	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode

	\return true if successful, false if fail.

	\warning 

*******************************************************************************/

bool CVideo::Camera_PreviewStart(WORD& iCameraStatus)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CCameraPreviewStartRequest();	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		TCHAR *pBuffer = (TCHAR*)ptBufferReceived->GetData();

		iCameraStatus = pBuffer[10] + (pBuffer[11] << 8) ;

		if(iCameraStatus != CAMERA_SUCCESS)
			bRes = false;
	}

	return bRes;
}

/******************************************************************************/
/*	Camera Stop Preview Command
	This command exits the preview state of camera services. This command must be executed after
	Camera Start Preview command and before a picture is taken.
	NOTE A two second delay is recommended between Camera Start Preview and Camera Stop Preview
	commands in order for white balance and exposure to take effect. There is no software feedback
	mechanism indicating these parameters are ready; two seconds is a reasonable time for the human
	eye, which is the only method to verify this.

	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode

	\return true if successful, false if fail.

	\warning 

*******************************************************************************/

bool CVideo::Camera_PreviewStop(WORD& iCameraStatus)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CCameraPreviewStopRequest();	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		TCHAR *pBuffer = (TCHAR*)ptBufferReceived->GetData();

		iCameraStatus = pBuffer[10] + (pBuffer[11] << 8) ;

		if(iCameraStatus != CAMERA_SUCCESS)
			bRes = false;
	}

	return bRes;
}

/******************************************************************************/
/*	Camera Take Picture and Encode Command
	This command takes the picture, encodes the picture, and saves it in the phone’s allocated
	memory heap. The format of the picture can be either raw or JPEG-encoded data stored in the
	allocated memory buffer. Memory heap is allocated when this command is executed. Refer to
	Appendix A for details on how the allocated memory buffer or memory length is calculated.
	The compression ratio or quality factor is selected, and it impacts the output file size. A quality
	factor from 1 to 99 can be selected, with 1 as the lowest quality and 99 as the highest quality. In
	addition, the compression ratio is only used for JPEG-encoded format (not with the raw format).

	\param	iQuality - This is the compression ratio or quality factor (valid range of [1:99], 
	                   with 1 as the lowest quality and 99 as the highest quality).
	\param iFormat   - Format of picture data:
	\code
               0 = CAMERA_RAW
               1 = CAMERA_JPEG
	\endcode

	\param piPicDataSize = This is the size of the entire picture in bytes.  

	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode

	\return true if successful, false if fail.

	\warning 

*******************************************************************************/

bool CVideo::Camera_TakePicture(const DWORD iQuality, const WORD iFormat,DWORD& iPicDataSize, WORD& iCameraStatus)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CCameraTakePictureRequest(iQuality,iFormat);	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		TCHAR *pBuffer = (TCHAR*)ptBufferReceived->GetData();

		iCameraStatus = pBuffer[10] + (pBuffer[11] << 8);
		iPicDataSize = pBuffer[12] + (pBuffer[13] << 8) + (pBuffer[14] << 16) + (pBuffer[15] << 24) ;

		if(iCameraStatus != CAMERA_SUCCESS)
			bRes = false;
	}

	return bRes;
}

/******************************************************************************/
/*	Camera Get Picture Command
	This command retrieves raw or JPEG-encoded picture data from the allocated memory space
	used in the Camera Take Picture and Encode command, based on a data offset and size. Given the
	data offset and size, this command will need to be executed a number of times until all picture
	data is retrieved.
	The recommended size to retrieve at one time is 512 bytes. The picture data format is based on
	the format parameter configured in the Camera Take Picture and Encode command.
	NOTE When retrieving the picture data, the picture will not be displayed on the phone’s LCD.

	\param iDataOffset - The DATA_OFFSET is the offset in which to start retrieving picture data from memory.
	\param iDataSize - The size of picture data in bytes to retrieve from memory during this transaction.
	\param piPicData - The actual picture data is returned into this buffer, the number of bytes is DATA_SIZE.
  
	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode

	\return true if successful, false if fail.

	\warning 

*******************************************************************************/

bool CVideo::Camera_GetPciture(const DWORD iDataOffset, WORD& iDataSize, TCHAR* piPicData, WORD& iCameraStatus)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CCameraGetPictureRequest(iDataOffset,iDataSize);	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		TCHAR *pBuffer = (TCHAR*)ptBufferReceived->GetData();

		iCameraStatus = pBuffer[10] + (pBuffer[11] << 8);
		DWORD iOffset = pBuffer[12] + (pBuffer[13] << 8) + (pBuffer[14] << 16) + (pBuffer[15] << 24);
		iDataSize = pBuffer[16] + (pBuffer[17] << 8);

		if(iCameraStatus != CAMERA_SUCCESS || (iOffset != iDataOffset))
			bRes = false;
		memcpy(piPicData, pBuffer+18, iDataSize);
	}

	return bRes;
}

/******************************************************************************/
/*	Camera Get Param Command
	This command retrieves the operational parameters of the camera sensor. For a list of possible
	parameter identifiers, refer to Camera Services Interface Specification and Operational
	Description (80-V5310-1).

	\param iParmID - Camera parameter identifier. Refer to 80-V5310-1 for definitions of parameter identifiers.		
	\code
        0 = CAMERA_PARM_STATE (Read Only)
        1 = CAMERA_PARM_ACTIVE_CMD (Read Only)
        2 = CAMERA_PARM_ZOOM
        3 = CAMERA_PARM_ENCODE_ROTATION
        4 = CAMERA_PARM_SENSOR_POSITION
        5 = CAMERA_PARM_CONTRAST
        6 = CAMERA_PARM_BRIGHTNESS
        7 = CAMERA_PARM_SHARPNESS
        8 = CAMERA_PARM_EXPOSURE
        9 = CAMERA_PARM_WB
        10 = CAMERA_PARM_EFFECT
        11 = CAMERA_PARM_AUDIO_FMT
        12 = CAMERA_PARM_FPS
        13 = CAMERA_PARM_FLASH
        14 = CAMERA_PARM_RED_EYE_REDUCTION
        15 = CAMERA_PARM_NIGHTSHOT_MODE
        16 = CAMERA_PARM_REFLECT
        17 = CAMERA_PARM_PREVIEW_MODE
        18 = CAMERA_PARM_ANTIBANDING
        19 = CAMERA_PARM_THUMBNAIL_WIDTH
        20 = CAMERA_PARM_THUMBNAIL_HEIGHT
        21 = CAMERA_PARM_THUMBNAIL_QUALITY
	\endcode

	\param piMinimumValue - Returns the minimum value of the operational parameter.
	\param piMaximumValue - Returns the maximum value of the operational parameter.
	\param piStepValue    - Returns the step value of the operational parameter.
	\param piDefaultValue - Returns the default value of the operational parameter.
	\param piCurrentValue - Returns the current value of the operational parameter.
	
	\param iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode

	\return true if successful, false if fail.

	\warning 

*******************************************************************************/

bool CVideo::Camera_GetParam(const WORD iParmId, DWORD& iMinimumValue,DWORD& iMaximumValue, DWORD& iStepValue, \
		DWORD& iDefaultValue, DWORD& iCurrentValue, WORD& iCameraStatus)
{	
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CCameraGetParamRequest(iParmId);	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		TCHAR *pBuffer = (TCHAR*)ptBufferReceived->GetData();

		iCameraStatus = pBuffer[10] + (pBuffer[11] << 8) ;
		WORD iParmId2 = pBuffer[12] + (pBuffer[13] << 8) ;
		iMinimumValue = pBuffer[14] + (pBuffer[15] << 8) + (pBuffer[16] << 16) + (pBuffer[17] << 24);
		iMaximumValue = pBuffer[18] + (pBuffer[19] << 8) + (pBuffer[20] << 16) + (pBuffer[21] << 24);
		iStepValue = pBuffer[22] + (pBuffer[23] << 8) + (pBuffer[24] << 16) + (pBuffer[25] << 24);
		iDefaultValue = pBuffer[26] + (pBuffer[27] << 8) + (pBuffer[28] << 16) + (pBuffer[29] << 24);
		iCurrentValue = pBuffer[30] + (pBuffer[31] << 8) + (pBuffer[32] << 16) + (pBuffer[33] << 24);

		if((iCameraStatus != CAMERA_SUCCESS) || (iParmId != iParmId2))
			bRes = false;
	}

	return bRes;
}

/******************************************************************************/
/* Camera Get Information Command
	This command retreives camera information such as the sensor width, height, rotation and
	orientation. Refer to Chapter 3 for possible sensor resolutions.

	\param piSensorWidth  - Returns the sensor width.
	\param piSensorHeight - Returns the sensor height.
	\param piFPS          - Returns the frames per second.
	\param piSensorRotationSensing  - Returns whether the sensor can sense when it is rotated (0 = not rotated, 1=Rotated)
	\param piDefaultRotation        - Returns the default rotation value when the sensor is not rotated.
	\param piDefaultOrientation     - Returns orientation (landscape or portrait):
	\code
        0 = CAMERA_ORIENTATION_LANDSCAPE
        1 = CAMERA_ORIENTATION_PORTRAIT
	\endcode

	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode

	\return true if successful, false if fail.

	\warning 

*******************************************************************************/

bool CVideo::Camera_GetInfo(WORD& iSensorWidth, WORD& iSensorHeight, WORD& iFPS, BYTE& iSensorRotationSensing, WORD& iDefaultRotation, \
				WORD& iDefaultOrientation, WORD& iCameraStatus)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CCameraGetInfoRequest();
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		TCHAR *pBuffer = (TCHAR*)ptBufferReceived->GetData();
		
		iCameraStatus = pBuffer[10] + (pBuffer[11] << 8);
		iSensorWidth = pBuffer[12] + (pBuffer[13] << 8);
		iSensorHeight = pBuffer[14] + (pBuffer[15] << 8);
		iFPS = pBuffer[16] + (pBuffer[17] << 8);
		iSensorRotationSensing = pBuffer[18];
		iDefaultRotation = pBuffer[19] + (pBuffer[20] << 8);
		iDefaultOrientation = pBuffer[21] + (pBuffer[22] << 8) ;
		
		if(iCameraStatus != CAMERA_SUCCESS)
			bRes = false;
		
	}

	return bRes;
}

/******************************************************************************/
/*	Select camera sensor
	This command selects which camera sensor will be used for subsequent calls
	to the FTM Camera API.

	\param iSensorID  - Sensor number to select.  The sensor number is the same ID number used
	                    in the embedded source code, specifically to the call to 
						camera_select_sensor().  If you are unsure of the sensor ID, then ask
						the embedded developers.

	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode

	\return true if successful, false if fail.

	\warning 

*******************************************************************************/

bool CVideo::Camera_SetSensorId(const BYTE iSensorID, WORD& iCameraStatus)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CCameraSetSensorIdRequest(iSensorID);	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		TCHAR *pBuffer = (TCHAR*)ptBufferReceived->GetData();

		iCameraStatus = pBuffer[10] + (pBuffer[11] << 8) ;

		if(iCameraStatus != CAMERA_SUCCESS)
			bRes = false;
	}

	return bRes;
}