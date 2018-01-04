#pragma once

#ifdef QISDA_EFS_MODULE_EXPORTS
#define QISDA_EFS_API extern "C" __declspec(dllexport)
#define QISDA_EFS_CLASS_API __declspec(dllexport)
#else
#define QISDA_EFS_API extern "C" __declspec(dllimport)
#define QISDA_EFS_CLASS_API __declspec(dllimport)
#endif

typedef bool (*pFnEFS_ReadFile)(const char*szPort,const char *sFileSrc, const char *sFileDst);
QISDA_EFS_API bool EFS_ReadFile(const char*szPort,const char *sFileSrc, const char *sFileDst);
typedef bool (*pFnEFS_ListFolder)(const char*szPort,const char *szFolder, char* szList, const char* sznBufferSize);
QISDA_EFS_API bool EFS_ListFolder(const char*szPort,const char *szFolder, char* szList, const char* sznBufferSize);
typedef bool (*pFnEFS_GetLocFile)(const char*szPort, char *szFileName);
QISDA_EFS_API bool EFS_GetLocFile(const char*szPort, char *szFileName);
typedef bool (*pFnEFS_GetLocFileName)(const char*szPort, char *szFileName);
QISDA_EFS_API bool EFS_GetLocFileName(const char*szPort, char *szFileName);
