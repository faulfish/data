#include "StdAfx.h"
#include "QLib_export_class.h"
#include "..\..\Code\ShareLib\Qualcomm\inc\QLib.h"
#include "..\..\Code\ShareLib\Qualcomm\inc\QLib_Defines.h"
#include "..\..\Code\ShareLib\Qualcomm\inc\QLib_SoftwareDownloadDefines.h"
#include <string>
#include <assert.h>


#pragma warning(push)
#pragma warning(disable:4800)

QLIB_EXPORT::QLIB_EXPORT(int nComPort):m_hResourceContext(NULL),m_nComPort(nComPort),m_bIsConnected(false)
{
	//Setup the 
	::QLIB_SetLibraryMode(QLIB_LIB_MODE_QPHONEMS);

	//Connect the mobile
	m_hResourceContext = ::QLIB_ConnectServer(nComPort);

	//Test if connected
	if(m_hResourceContext)
		m_bIsConnected = ::QLIB_IsPhoneConnected(m_hResourceContext);
}

QLIB_EXPORT::~QLIB_EXPORT()
{
	if(m_bIsConnected && m_hResourceContext)
	{
		//Disconnect to the mobile by the handle
		::QLIB_DisconnectServer(m_hResourceContext);
		m_hResourceContext = NULL;
	}
}

bool QLIB_EXPORT::GetAllPortList(unsigned short* iNumPorts, unsigned short* pPortList)
{
	//Get All ports withou check
	//bool bRes = ::QLIB_GetAllPortList(iNumPorts,pPortList);	
	//0 meas alway check all ports
	return ::QLIB_GetAvailablePhonesPortList(iNumPorts,pPortList,0,NULL);
}

bool QLIB_EXPORT::GetMobileInfo(ULONG iMSMHwVersion,ULONG iMobileModel,char szMobSwRev[STR_BUFFER_MAX],char szModel[STR_BUFFER_MAX])
{
	bool bRes = false;
	if(m_bIsConnected && m_hResourceContext)
	{
		//Get mobile information
		bRes = ::QLIB_DIAG_EXT_BUILD_ID_F(m_hResourceContext, &iMSMHwVersion, &iMobileModel, szMobSwRev, szModel) ? true : false;
	}
	return bRes;
}

bool QLIB_EXPORT::CheckFolderExist(const std::string& strMobilePath,bool bForceCreate)
{
	bool bRes = false;
	if(m_bIsConnected && m_hResourceContext)
	{
		//Check the folder exist
		char szPath[STR_BUFFER_MAX] = {0};
		strcpy_s(szPath,strMobilePath.c_str());
		bRes = QLIB_EfsDirectory(m_hResourceContext, szPath) ? true : false;
		if(bRes == false && bForceCreate)
		{
			bRes = ::QLIB_EfsMkDir(m_hResourceContext, szPath) ? true : false;
			bRes = ::QLIB_EfsDirectory(m_hResourceContext, szPath) ? true : false;
		}
	}
	return bRes;
}

bool QLIB_EXPORT::UploadFile(const std::string& strUploadPath, const std::string& strMobilePath,bool bNotify)
{
	bool bRes = false;
	if(m_bIsConnected && m_hResourceContext)
	{
		if(bNotify == true)
		{
			//Setup the callback function
			::QLIB_ConfigureEfs2CallBacks(m_hResourceContext, EFSCallBackFunc, NULL);
		}

		char szSrcPath[STR_BUFFER_MAX] = {0};
		strcpy_s(szSrcPath,strUploadPath.c_str());
		char szDestPath[STR_BUFFER_MAX] = {0};
		strcpy_s(szDestPath,strMobilePath.c_str());
		//Write the file to mobile
		bRes = ::QLIB_EfsWrite(m_hResourceContext, szSrcPath, szDestPath)  ? true : false;
		/*
		QLIB_API unsigned char QLIB_EfsWrite  (  HANDLE  hResourceContext,  char *  sLocalFile,  char *  sEFS_File  )  
		Parameter :
		sLocalFile  = absolute path of file on PC  
		sEFS_File  = absolute path of file to write on phone, must be in the format "folder/file.ext" without a leading "/" 
		*/
	}
	return bRes;
}

bool QLIB_EXPORT::DownloadFile(const std::string& strTargetPath,const std::string& strMobilePath)
{
	bool bRes = false;
	if(m_bIsConnected && m_hResourceContext)
	{
		bRes = ::QLIB_EfsRead(m_hResourceContext,const_cast<char*>(strMobilePath.c_str()),const_cast<char*>(strTargetPath.c_str()));
	}
	return bRes;
}

bool QLIB_EXPORT::DeleteFile(const std::string& strTargetPath)
{
	bool bRes = false;
	if(m_bIsConnected && m_hResourceContext)
	{
		bRes = ::QLIB_EfsDelete(m_hResourceContext,const_cast<char*>(strTargetPath.c_str()));
	}
	return bRes;
}

bool QLIB_EXPORT::PowerOff()
{
	bool bRes = false;
	if(m_bIsConnected && m_hResourceContext)
	{
		//Power off the mobile
		bRes = ::QLIB_DIAG_CONTROL_F(m_hResourceContext, MODE_POWER_OFF_F) ? true : false;
	}
	return bRes;
}

unsigned char QLIB_EXPORT::EFSCallBackFunc(char srcname[STR_BUFFER_MAX], char dstname[STR_BUFFER_MAX], int oper, int suboper,unsigned long bytestransferred,unsigned long filesize,HANDLE hContextID)
{
	assert(0);//Need to implement
	
	double ByteSize = filesize;
	long double Percent = (bytestransferred/ByteSize)*100.0;

	return 0;
}

bool QLIB_EXPORT::CheckFreeSpace(ULONGLONG ulSumFileSize) 
{
	bool bRes= false;
	if(m_bIsConnected && m_hResourceContext)
	{
		ULONG piErrorNo = 0; 
		ULONG piFS_ID = 0; 
		ULONG piFS_Type = 0; 
		ULONG piBlockSize = 0; 
		ULONG piTotalBlocks = 0; 
		ULONG piAvailBlocks = 0; 
		ULONG piFreeBlocks = 0; 
		ULONG piMaxFileSize = 0;
		ULONG piNfiles = 0; 
		ULONG piMaxNfiles = 0;

		bRes = ::QLIB_EFS2_DIAG_STATFS(m_hResourceContext, "//", &piErrorNo, &piFS_ID, &piFS_Type, &piBlockSize, &piTotalBlocks, &piAvailBlocks, &piFreeBlocks, &piMaxFileSize, &piNfiles, &piMaxNfiles) ? true : false;

		ULONG ulFreeSpace = piAvailBlocks*piBlockSize;	
		bRes = (ulSumFileSize*2 + 200*1024) < ulFreeSpace;//?? how calculate about mobile has enough free space
	}
	return bRes;
}

int QLIB_EXPORT::IsTestMode()
{
	int bRes = FALSE;
	unsigned char cIsTestMode = 0;
	if(m_bIsConnected && m_hResourceContext)
	{
		//bRes = ::QLIB_DIAG_NV_READ_F(m_hResourceContext, 453, szBuffer, 128, nStatus) ? true : false;
		if(::QLIB_IsFTM_Mode(m_hResourceContext, &cIsTestMode))
			bRes = cIsTestMode ? TRUE: FALSE;
		else
			bRes = RUN_FAILED;
	}
	return bRes;
}

int QLIB_EXPORT::SetTestMode()
{
	int nRes = FALSE;
	if(m_bIsConnected && m_hResourceContext)
		nRes = ToFTMMode() ? TRUE : FALSE;
	else
		nRes = FAIL_CONNECTION;
	return nRes;
}

bool QLIB_EXPORT::SetOnlineMode()//Without make sure to Online mode
{
	return ToOnlineMode();
}

int QLIB_EXPORT::HexBcdToDigit(BYTE *pucHexBcdBuf, int iHexBcdBufSize, char *sDigitBuf, int iDigitBufSize)
{
	int nRet = 0;
	if (pucHexBcdBuf && (iHexBcdBufSize > 0) && sDigitBuf && (iDigitBufSize>0) && iHexBcdBufSize*2 <= iDigitBufSize)
	{
		BYTE ucLowByte, ucHighByte;
		int i = 0;
		for (i = 0; i < iHexBcdBufSize; i++)
		{
			ucLowByte = *(pucHexBcdBuf+i)&0x0F;
			*(sDigitBuf+(2*i)) = (ucLowByte>=10) ? ((ucLowByte-10)+'a') : (ucLowByte+'0');

			ucHighByte = (*(pucHexBcdBuf+i)&0xF0)>>4;
			*(sDigitBuf+(2*i+1)) = (ucHighByte>=10) ? ((ucHighByte-10)+'a') : (ucHighByte+'0');
		} 

		if (iHexBcdBufSize%2)
		{
			ucLowByte = *(pucHexBcdBuf+i)&0x0F;
			*(sDigitBuf+(2*i)) = (ucLowByte>=10) ? ((ucLowByte-10)+'a') : (ucLowByte+'0');
		}
		nRet = (iHexBcdBufSize%2) ? (2*i+1) : (2*i);
	}
	return nRet;
}

bool QLIB_EXPORT::ReadIMEI(char* szIMEI100)
{
	bool bRes = false;

	unsigned short iStatus = 0;
	unsigned char szHexIMEI[15] = {0};
	char sDigitIMEI[31] = {0};
	if(m_bIsConnected && m_hResourceContext)
	{
		if(::QLIB_DIAG_NV_READ_F(m_hResourceContext, 550, szHexIMEI, 15, &iStatus))
		{
			if(iStatus==0 && HexBcdToDigit(szHexIMEI, 15, sDigitIMEI, 31))
			{
				for(int i=0; i<15; i++)
					szIMEI100[i] = sDigitIMEI[i+3];
				bRes = true;
			}
			else if(iStatus == 5)
			{
				sprintf(szIMEI100, "000000011234560");
				bRes = true;
			}
		}
		Sleep(1000);
		
	}
	return bRes;
}

bool QLIB_EXPORT::Reset()
{
	bool bRes = false;
	if(m_bIsConnected && m_hResourceContext)
	{
		if(::QLIB_DIAG_CONTROL_F(m_hResourceContext, MODE_OFFLINE_D_F))
		{
			if(	::QLIB_DIAG_CONTROL_F(m_hResourceContext, MODE_RESET_F))
			{
				bRes = true;
			}
		}
			
	}
	return bRes;
}

bool QLIB_EXPORT::ToOnlineMode()
{
	return ToMode(0);
}

bool QLIB_EXPORT::ToFTMMode()
{
	return ToMode(1);
}

bool QLIB_EXPORT::ToMode(unsigned char bFTMmode)//Must make sure had connected
{	
	unsigned char bIsFTMMode = false;
	assert(m_bIsConnected && m_hResourceContext);
	if(m_bIsConnected && m_hResourceContext)
	{
		bool bOk = ::QLIB_IsFTM_Mode(m_hResourceContext,&bIsFTMMode);
		if(bOk && bIsFTMMode != bFTMmode)
		{
			bOk = ::QLIB_ChangeFTM_BootMode(m_hResourceContext,bFTMmode,1);
			if(bOk)
			{
				m_bIsConnected = false;
				m_hResourceContext = NULL;
				WaitUntilReset();
			}
			else assert(0);

			if(bOk)
				bOk = ::QLIB_IsFTM_Mode(m_hResourceContext,&bIsFTMMode);
			else assert(0);
		}
	}
	assert(bIsFTMMode == bFTMmode);
	return bIsFTMMode;
}

void QLIB_EXPORT::WaitUntilReset()
{
	while(!(m_hResourceContext && m_bIsConnected))
	{
		::Sleep(1000);
		//Connect the mobile
		m_hResourceContext = ::QLIB_ConnectServer(m_nComPort);
		//Test if connected
		if(m_hResourceContext)
		{
			m_bIsConnected = ::QLIB_IsPhoneConnected(m_hResourceContext);
			if(m_bIsConnected == false)
			{
				::QLIB_DisconnectServer(m_hResourceContext);
				m_hResourceContext = NULL;
			}
		}
		//For first reboot only
		if(m_bIsConnected && m_hResourceContext)
			::Sleep(5000);
	};
}

bool QLIB_EXPORT::CaptureFTM(int nSensorID,int nWidth,int nHeight,const std::string& strTargetFile)
{
	bool bOk = false;
	
	if(m_bIsConnected && m_hResourceContext)
	{
		if(ToFTMMode())
		{	
			unsigned short _iCameraStatus;

			bOk = ::QLIB_FTM_CAMERA_SET_SENSOR_ID(m_hResourceContext,nSensorID,&_iCameraStatus);

			bOk = ::QLIB_FTM_CAMERA_STOP(m_hResourceContext,&_iCameraStatus);

			bOk = ::QLIB_FTM_CAMERA_START(m_hResourceContext,&_iCameraStatus);
			assert(bOk);

			
			if(bOk)
			{
				bOk = ::QLIB_FTM_CAMERA_SET_PARM(m_hResourceContext, 17, NULL, &_iCameraStatus);
				assert(bOk);

				bOk = TakePicture(nWidth,nHeight,strTargetFile);
			}

			::QLIB_FTM_CAMERA_STOP(m_hResourceContext,&_iCameraStatus);			
		}
	}

	return bOk;
}

bool QLIB_EXPORT::TakePicture(int nWidth,int nHeight,const std::string& strTargetFile)
{
	bool bOk = false;
	unsigned short _iCameraStatus;
	unsigned char	_bAutoPictureSize = false;	// Auto determine picture width
	unsigned short	_iPictureWidth = nWidth;			// not used because auto-
	unsigned short	_iPictureHeight = nHeight; 
	unsigned short	_iDelay_ms = 2000;			// 2 seconds
	unsigned long	_iQuality = 90;				// % quality (0-99)
	unsigned short	_iFormat = CAMERA_JPEG;		// JPEG format
	unsigned long	_iPicDataSize = 0;			// Will get updated when picture is taken
	unsigned short	_iDataTransferSize = 512;	// Number of bytes transferred at a time
	void*	_piPicData = NULL;

	/*
	bOk = ::QLIB_FTM_CAMERA_PerformTakePictureSequence
		( 
		m_hResourceContext, 
		_bAutoPictureSize, 
		_iPictureWidth, 
		_iPictureHeight, 
		_iDelay_ms, 
		_iQuality, 
		_iFormat, 
		&_iPicDataSize, 
		&_iCameraStatus 
		);
	*/

	bOk = ::QLIB_FTM_CAMERA_SET_DIMENSIONS(m_hResourceContext,nWidth,nHeight,0,&_iCameraStatus);
	assert(bOk);

	bOk = ::QLIB_FTM_CAMERA_START_PREVIEW(m_hResourceContext, &_iCameraStatus);
	assert(bOk);

	/******************************************************************************************************************************************
	The reason to sleep here is the following two:
	1. For camera stability, it needs to wait for 1~2 seconds, else the photo will be very green.
	2. In Lavernock, if no sleep, the following QLIB_FTM_CAMERA_TAKE_PICTURE_AND_ENCODE function will fail and unit will reset.
	*******************************************************************************************************************************************/
	Sleep(1000);

	bOk = ::QLIB_FTM_CAMERA_TAKE_PICTURE_AND_ENCODE(m_hResourceContext,_iQuality,CAMERA_JPEG,&_iPicDataSize,&_iCameraStatus);
	assert(bOk);

	// Now transfer the picture into memory
	if (	bOk  &&	_iPicDataSize != 0 )
	{
		// Allocate memory 
		_piPicData = malloc( _iPicDataSize );

		if ( _piPicData != NULL )
		{
			bOk = ::QLIB_FTM_CAMERA_GetEntirePicture
				( 
				m_hResourceContext, 
				_iPicDataSize, 
				_iDataTransferSize, 
				(unsigned char*) _piPicData, 
				&_iCameraStatus 
				);

			if (bOk)
			{
				// Setup file pointer
				FILE* _hGraphicFile = NULL;

				// Now store this data into a file
				if (_hGraphicFile = ::fopen( strTargetFile.c_str(),"wb"))
				{
					// Write the data to a file
					fwrite( _piPicData, _iPicDataSize, 1, _hGraphicFile);
				}	// fopen()

				// Close the file
				if (_hGraphicFile != NULL )
					fclose( _hGraphicFile );
			}

		}	// if ( _piPicData != NULL )

		// Free th memory
		::free( _piPicData);
	}

	return bOk;
}

bool QLIB_EXPORT::GetCamerInfo()
{
	bool bOk = false;
	unsigned short _iCameraStatus;
	unsigned short iSensorWidth;
	unsigned short iSensorHeight;
	unsigned short iFPS;
	unsigned char  iSensorRotationSensing;
	unsigned short iDefaultRotation;
	unsigned short iDefaultOrientation;
	bOk = ::QLIB_FTM_CAMERA_GET_INFO(m_hResourceContext
		,&iSensorWidth
		,&iSensorHeight
		,&iFPS
		,&iSensorRotationSensing
		,&iDefaultRotation
		,&iDefaultOrientation
		,&_iCameraStatus);
	return bOk;
}

bool QLIB_EXPORT::GetRTCTime(unsigned char* iMonth,unsigned char* iDay, unsigned short* iYear, unsigned char* iHour, unsigned char* iMinute,unsigned char* iSecond)
{
	unsigned char nTimeMode = 2;//Means 24 Hour
	unsigned char szTemp1[50] = {0};
	unsigned char szTemp2[50] = {0};
	unsigned short szTemp3[50] = {0};
	unsigned char szTemp4[50] = {0};
	unsigned char szTemp5[50] = {0};
	unsigned char szTemp6[50] = {0};
	bool bOk = ::QLIB_FTM_PMIC_RTC_SET_GET_TIME(m_hResourceContext,0,&nTimeMode,szTemp1, szTemp2, szTemp3, szTemp4, szTemp5,szTemp6);
	return bOk;
}

#pragma warning(pop)