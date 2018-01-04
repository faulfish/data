#ifndef __CALLBACK_TYPES_H__
#define __CALLBACK_TYPES_H__

#include "tstl.h"
using namespace std;


#ifdef MIDDLEWARE_EXPORTS
#define MIDDLEWARE_API extern "C" __declspec(dllexport)
#else
#define MIDDLEWARE_API extern "C" __declspec(dllimport)
#endif

typedef char*(__stdcall *pMessageSender)(char*, char*);

static pMessageSender g_pMessageSender = NULL;

pMessageSender* GetCallbackFunctionPointer();

#define PassFailQuit    "PassFailQuit"
#define OK              "OK"
#define YesNo           "YesNo"
#define TextPrompt      "TextPrompt"

MIDDLEWARE_API bool OutputData(char* pDescription, char* pData, string& outputString);
MIDDLEWARE_API bool OutputDetailResult(char* pDescription, char* pBand, char* pChannel, char* pLowLimit, char* pHighLimit, char* pResult, char* pUnits, char* pPassFailFlag);
MIDDLEWARE_API bool OutputHeader(char* pHeader, string& outputString);
MIDDLEWARE_API bool OutputMessage(char* pMessage, string& outputString);
MIDDLEWARE_API bool OutputPrompt(char* pTitle, char* pPrompt, char* PromptType, string& outputString);
MIDDLEWARE_API bool OutputResult(char* pDescription, char* pLowLimit, char* pHighLimit, char* pResult, char* pPassFailFlag, string& outputString);
MIDDLEWARE_API bool OutputStatus(char* pStatusCode, string& outputString);
MIDDLEWARE_API bool CheckAbortStatus(tstring& outputString);

MIDDLEWARE_API  char* RFOutputCallBackFunc(char* pType, char* pData);
MIDDLEWARE_API bool ManageResource(char* pAction, char* pResource, tstring& outputString);

#endif