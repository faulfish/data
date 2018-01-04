#include "Loader.h"
#include <vector>
#include <string>

#ifdef LOADER_EXPORTS
#define LOADER_API extern "C" __declspec(dllexport)
#define LOADER_CLASS_API __declspec(dllexport)
#else
#define LOADER_API extern "C" __declspec(dllimport)
#define LOADER_CLASS_API __declspec(dllimport)
#endif

typedef bool (*pFnGetSPULoader)(ISPU_CALLBACK* pCallBack);
LOADER_API HANDLE GetSPULoader(ISPU_CALLBACK* pCallBack);
typedef bool (*pFnLoadScript)(HANDLE hResource, const TSTRING& szScript);
LOADER_API bool LoadScript(HANDLE hResource, const TSTRING& szScript);
typedef bool (*pFnExecuteCommand)(HANDLE hResource);
LOADER_API SPU_RESULT ExecuteCommand(HANDLE hResource);
typedef bool (*pFnGetDescription)(HANDLE hResource,TCHAR* szTemp,int nBufferSize);
LOADER_API SPU_RESULT GetDescription(HANDLE hResource,TCHAR* szTemp,int nBufferSize);
typedef bool (*pFnGetHeader)(HANDLE hResource,TCHAR* szTemp,int nBufferSize);
LOADER_API SPU_RESULT GetHeader(HANDLE hResource,TCHAR* szTemp,int nBufferSize);
typedef bool (*pFnGetUIType)(const TSTRING& szScript);
LOADER_API UINT GetUIType(const TSTRING& szScript);
typedef bool (*pFnReleaseHandle)(HANDLE hResource);
LOADER_API void ReleaseHandle(HANDLE hResource);
