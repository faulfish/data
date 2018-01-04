#ifndef _QLIB_EXPORT_CLASS_H_
#define _QLIB_EXPORT_CLASS_H_

#pragma once

#include "QLib_export.h"
#include <string>

//C++ version
class Q_CLASS_API QLIB_EXPORT
{
public:
	QLIB_EXPORT(int nComPort);
	~QLIB_EXPORT();
	bool GetMobileInfo(ULONG iMSMHwVersion,ULONG iMobileModel,char szMobSwRev[STR_BUFFER_MAX],char szModel[STR_BUFFER_MAX]);
	bool CheckFolderExist(const std::string& strMobilePath,bool bForceCreate = false);
	bool UploadFile(const std::string& strUploadPath, const std::string& strMobilePath,bool bNotify = false);
	bool DownloadFile(const std::string& strTargetPath, const std::string& strMobilePath);
	bool DeleteFile(const std::string& strTargetPath);
	bool PowerOff();
	bool CheckFreeSpace(ULONGLONG ulSumFileSize);
	int IsTestMode();
	int SetTestMode();//Without make sure to FTM mode
	bool SetOnlineMode();//Without make sure to Online mode
	bool ReadIMEI(char* szIMEI100);
	bool Reset();
	bool CaptureFTM(int nSensorID,int nWidth,int nHeight,const std::string& strTargetFile);

	static bool GetAllPortList(unsigned short* iNumPorts, unsigned short* pPortList);
	bool GetRTCTime(unsigned char* iMonth,unsigned char* iDay, unsigned short* iYear, unsigned char* iHour, unsigned char* iMinute,	unsigned char* iSecond);

protected:
	static unsigned char EFSCallBackFunc(char srcname[STR_BUFFER_MAX], char dstname[STR_BUFFER_MAX], int oper, int suboper,unsigned long bytestransferred,unsigned long filesize,HANDLE hContextID);
	void WaitUntilReset();
	bool ToFTMMode();
	bool ToOnlineMode();
	bool TakePicture(int nWidth,int nHeight,const std::string& strTargetFile);
	bool GetCamerInfo();
	bool ToMode(unsigned char bFTMmode);//Must make sure had connected
private:
	int HexBcdToDigit(BYTE *pucHexBcdBuf, int iHexBcdBufSize, char *sDigitBuf, int iDigitBufSize);
private:
	HANDLE						m_hResourceContext;
	int									m_nComPort;
	unsigned char				m_bIsConnected;
};

#endif// _QLIB_EXPORT_CLASS_H_