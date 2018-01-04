// QLib_Export.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "QLib_export.h"
#include "QLib_export_class.h"
#include <stdio.h>

#ifdef _MANAGED
#pragma managed(push, off)
#endif

//BOOL APIENTRY DllMain( HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
//{
//    return TRUE;
//}

/*Sample code*/
Q_API bool Q_STL_UploadFile(const std::string& strUploadPath, const std::string& strMobilePath,int nComPort)
{
	return Q_UploadFile(strUploadPath.c_str(),strMobilePath.c_str(),nComPort);
}

Q_API bool Q_UploadFile(const char strUploadPath[STR_BUFFER_MAX],const char strMobilePath[STR_BUFFER_MAX]/*const std::string& strUploadPath, const std::string& strMobilePath*/,int nComPort)
{
	QLIB_EXPORT QLib(nComPort);
	bool bRes = QLib.UploadFile(strUploadPath,strMobilePath);
	if(bRes)
		QLib.PowerOff();
	return bRes;
}

Q_API bool Q_STL_DownloadFile(const std::string& strTargetPath, const std::string& strMobilePath,int nComPort)
{
	return Q_DownloadFile(strTargetPath.c_str(),strMobilePath.c_str(),nComPort);
}

Q_API bool Q_DownloadFile(const char strTargetPath[STR_BUFFER_MAX],const char strMobilePath[STR_BUFFER_MAX],int nComPort)
{
	QLIB_EXPORT QLib(nComPort);
	bool bRes = QLib.DownloadFile(strTargetPath,strMobilePath);
	return bRes;
}

Q_API bool Q_DeleteFile(const char strTargetPath[STR_BUFFER_MAX],int nComPort)
{
	QLIB_EXPORT QLib(nComPort);
	bool bRes = QLib.DeleteFile(strTargetPath);
	return bRes;
}

Q_API int Q_IsTestMode(long nComPort)
{
	return QL_IsTestMode(nComPort);
}

int QL_IsTestMode(long nComPort)
{
	QLIB_EXPORT QLib(nComPort);
	return QLib.IsTestMode();
}

bool QL_ReadIMEI(long nComPort, char* szIMEI100)
{
	QLIB_EXPORT QLib(nComPort);
	bool bRes = QLib.ReadIMEI(szIMEI100);
	return bRes;
}

bool QL_Reset(long nComPort)
{
	QLIB_EXPORT QLib(nComPort);
	bool bRes = QLib.Reset();
	Sleep(10000);
	return bRes;
}

int QL_SetTestMode(long nComPort)
{
	int nRet = TRUE;
	QLIB_EXPORT QLib(nComPort);
	if(QLib.IsTestMode() != TRUE)
	{	
		nRet = QLib.SetTestMode();
	}
	return nRet;
}

bool QL_SetOnlineMode(long nComPort)
{
	QLIB_EXPORT QLib(nComPort);
	return QLib.SetOnlineMode();
}

bool QL_GetDiagPort(unsigned short* iNumPorts, unsigned short* pPortList)
{
	return QLIB_EXPORT::GetAllPortList(iNumPorts,pPortList);
}

Q_API bool QL_STL_CapturePicture(int nSensorID,int nWidth,int nHeight,const std::string& strTargetFile,long nComPort)
{
	QLIB_EXPORT QLib(nComPort);
	return QLib.CaptureFTM(nSensorID,nWidth,nHeight,strTargetFile);
}

Q_API bool QL_STL_CapturePicture_ForSPU(char* szPort,char* sznSensorID,char* sznWidth,char* sznHeight,char* strTargetFile)
{
	return QL_STL_CapturePicture(::atoi(sznSensorID),::atoi(sznWidth),::atoi(sznHeight),strTargetFile,::atoi(szPort));
}

Q_API bool QL_GetRTCTime_ForSPU(char* szPort,char* sziMonth, char* sziDay, char* sziYear, char* sziHour, char* sziMinute,char* sziSecond)
{
	QLIB_EXPORT QLib(::atoi(szPort));
	unsigned char iMonth = -1;
	unsigned char iDay = -1;
	unsigned short iYear = -1;
	unsigned char iHour = -1;
	unsigned char iMinute = -1;
	unsigned char iSecond = -1;
	bool bRes = QLib.GetRTCTime(&iMonth, &iDay, &iYear, &iHour, &iMinute, &iSecond);
	if(bRes)
	{
	}
	return bRes;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

