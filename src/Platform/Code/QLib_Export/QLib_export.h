#ifndef _QLIB_EXPORT_
#define _QLIB_EXPORT_

#pragma once
#include <string>

#ifdef QLIB_EXPORT_EXPORTS
#define Q_API extern "C" __declspec(dllexport)
#define Q_CLASS_API __declspec(dllexport)
#else
#define Q_API extern "C" __declspec(dllimport)
#define Q_CLASS_API __declspec(dllimport)
#endif


#define STR_BUFFER_MAX 512

#define RUN_SUCCESSFULLY	1
#define RUN_FAILED			(-1)
#define FAIL_CONNECTION		(-2)

//pure c version
Q_API bool Q_UploadFile(const char strUploadPath[STR_BUFFER_MAX],const char strMobilePath[STR_BUFFER_MAX],int nComPort);
Q_API bool Q_DownloadFile(const char strTargetPath[STR_BUFFER_MAX],const char strMobilePath[STR_BUFFER_MAX],int nComPort);
Q_API bool Q_DeleteFile(const char strTargetPath[STR_BUFFER_MAX],int nComPort);
Q_API int Q_IsTestMode(long nComPort);
int QL_IsTestMode(long nComPort);
int QL_SetTestMode(long nComPort);
bool QL_SetOnlineMode(long nComPort);
bool QL_ReadIMEI(long nComPort, char* szIMEI100);
//Sample sode for Q_GetMotoXPRS
/*
MotoXPRSDATA data;
memset(&data,0,sizeof(MotoXPRSDATA));
Q_GetMotoXPRS(87,data,5000);
*/
bool QL_Reset(long nComPort);
bool QL_GetDiagPort(unsigned short* iNumPorts, unsigned short* pPortList);
/*
unsigned short nPorts = 10;
unsigned short Ports[10] = {0};
QL_GetDiagPort(&nPorts,Ports);
*/

//STL version
Q_API bool Q_STL_UploadFile(const std::string& strUploadPath, const std::string& strMobilePath,int nComPort);
Q_API bool Q_STL_DownloadFile(const std::string& strTargetPath, const std::string& strMobilePath,int nComPort);
Q_API bool QL_STL_CapturePicture(int nSensorID,int nWidth,int nHeight,const std::string& strTargetFile,long nComPort);
Q_API bool QL_STL_CapturePicture_ForSPU(char* szPort,char* sznSensorID,char* sznWidth,char* sznHeight,char* strTargetFile);
Q_API bool QL_GetRTCTime_ForSPU(char* szPort,char* sziMonth, char* sziDay, char* sziYear, char* sziHour, char* sziMinute,char* sziSecond);
/*
QL_STL_CapturePicture(0,240,320,"c:\\Sensor0.jpg",Ports[0]);
QL_STL_CapturePicture(1,240,320,"c:\\Sensor1.jpg",Ports[0]);
*/
#endif //_QLIB_EXPORT_