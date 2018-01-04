
#include "stdafx.h"
#include "CallbackTypes.h"

pMessageSender* GetCallbackFunctionPointer() {
    return &g_pMessageSender;
}

MIDDLEWARE_API char* RFOutputCallBackFunc(char* pType, char* pData)
{	
	char* pResult = NULL;
	if(g_pMessageSender != NULL)
		pResult = (*g_pMessageSender)(pType, pData);
	return pResult;
}

MIDDLEWARE_API bool CheckAbortStatus(string& outputString) {
    if(g_pMessageSender != NULL)
        outputString = (*g_pMessageSender)("CheckAbortStatus","");
    return true;
}

MIDDLEWARE_API bool ManageResource(char* pAction, char* pResource, string& outputString) {
    CStringA strParam =
        CStringA(pAction) + "," +
        CStringA(pResource);
    if(g_pMessageSender != NULL)
        outputString = (*g_pMessageSender)("ManageResources",(LPSTR)(LPCSTR)strParam);
    return true;
}
MIDDLEWARE_API bool OutputData(char* pDescription, char* pData, string& outputString) {
    CStringA strParam = 
        CStringA(pDescription) + "," +
        CStringA(pData);
    if(g_pMessageSender != NULL)
        outputString = (*g_pMessageSender)("OutputData",(LPSTR)(LPCSTR)strParam);
    return true;
}

MIDDLEWARE_API bool OutputDetailResult(char* pDescription, char* pBand, char* pChannel, char* pLowLimit, char* pHighLimit, char* pResult, char* pUnits, char* pPassFailFlag) {
    CStringA strParam =
        CStringA(pDescription) + "," +
        CStringA(pBand) + "," +
		CStringA(pChannel) + "," +
        CStringA(pLowLimit) + "," +
        CStringA(pHighLimit) + "," +
        CStringA(pResult) + "," +
        CStringA(pUnits) + "," +
        CStringA(pPassFailFlag);
    if(g_pMessageSender != NULL)
         (*g_pMessageSender)("OutputDetailedResult",(LPSTR)(LPCSTR)strParam);
    return true;
}

MIDDLEWARE_API bool OutputHeader(char* pHeader, string& outputString) {
    CStringA strParam = pHeader;
    if(g_pMessageSender != NULL)
        outputString = (*g_pMessageSender)("OutputHeader",(LPSTR)(LPCSTR)strParam);
    return true;
}

MIDDLEWARE_API bool OutputMessage(char* pMessage, string& outputString) {
    CStringA strParam = pMessage;
    if(g_pMessageSender != NULL)
        outputString = (*g_pMessageSender)("OutputMessage",(LPSTR)(LPCSTR)strParam);
    return true;
}

MIDDLEWARE_API bool OutputPrompt(char* pTitle, char* pPrompt, char* PromptType, string& outputString) {
    CStringA strParam =
        CStringA(pTitle) + "|" +
        CStringA(pPrompt) + "|" +
        CStringA(PromptType);
    if(g_pMessageSender != NULL)
        outputString = (*g_pMessageSender)("OutputPrompt",(LPSTR)(LPCSTR)strParam);
    return true;
}

MIDDLEWARE_API bool OutputResult(char* pDescription, char* pLowLimit, char* pHighLimit, char* pResult, char* pPassFailFlag, string& outputString) {
    CStringA strParam =
        CStringA(pDescription) + "," +
        CStringA(pLowLimit) + "," +
        CStringA(pHighLimit) + "," +
        CStringA(pResult) + "," +
        CStringA(pPassFailFlag);
    if(g_pMessageSender != NULL)
        outputString = (*g_pMessageSender)("OutputResult",(LPSTR)(LPCSTR)strParam);
    return true;
}

MIDDLEWARE_API bool OutputStatus(char* pStatusCode, string& outputString) {
    CStringA strParam = pStatusCode;
    if(g_pMessageSender != NULL)
        outputString = (*g_pMessageSender)("OutputStatus",(LPSTR)(LPCSTR)strParam);
    return true;
}