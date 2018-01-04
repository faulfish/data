// QisdaMiscellaneousModule.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "QisdaMiscellaneousModule.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinApp theApp;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    int nRetCode = 0;

    // initialize MFC and print and error on failure
    if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
    {
        // TODO: change error code to suit your needs
        _tprintf(_T("Fatal Error: MFC initialization failed\n"));
        nRetCode = 1;
    }
    else
    {
        // TODO: code your application's behavior here.
    }

    return nRetCode;
}


CQisdaMiscellaneousModule::CQisdaMiscellaneousModule(int nCOMPort):CInterfaceBase(nCOMPort)
{
}

CQisdaMiscellaneousModule::~CQisdaMiscellaneousModule(void)
{
}

bool CQisdaMiscellaneousModule::GetSWHWVersion(LPCSTR sSW, LPCSTR sHW)
{

	using namespace PLATFORM_TOOL;

	bool bRes = false;

	// To determine the length of the request packet
	CRequestPtr ptRequest = new CDIAGGetSWVersionRequest();	
	bRes = Execute(ptRequest);

    if(!bRes) {
        PopLastLog();
        return false;
    }

    CBufferPtr ptBufferReceived;
    ptRequest->GetReceiveData(ptBufferReceived);
    TCHAR *pBuffer = (TCHAR *)ptBufferReceived->GetData();
    int nReceiceSize = ptBufferReceived->GetLength();

    if(nReceiceSize <= 0) {
        return false;
    }

    bool bFirstSymbol = false;
    bool bSecondSymbol = false;
    CString szHW = _T(""), szSW = _T("");
    for(int i = 0; i < nReceiceSize; i++) {
        if(pBuffer[i] == _T(';') && !bFirstSymbol)
            bFirstSymbol = true;
        else if(pBuffer[i] ==_T(';') && bFirstSymbol && !bSecondSymbol)
            bSecondSymbol = true;
        else if(pBuffer[i] == _T(';') && bFirstSymbol && bSecondSymbol)
            break;
        else if(bFirstSymbol && !bSecondSymbol)
            szHW += pBuffer[i];
        else if(bFirstSymbol && bSecondSymbol)
            szSW += pBuffer[i];
        else
            ;

    }
    _tcscpy((TCHAR*)sSW, szSW.GetBuffer(0));
    _tcscpy((TCHAR*)sHW, szHW.GetBuffer(0));

	return bRes;

}

bool CQisdaMiscellaneousModule::GetHDVersion(int& nPhoneCodeVersion, int& nBootloaderVersion, int nTimeOut)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	bool bExecuteResult = false;
	CRequestPtr ptRequest = new CQueryHDVersionRequest(nPhoneCodeVersion, nBootloaderVersion, bExecuteResult);
	if(Execute(ptRequest, nTimeOut) && bExecuteResult)
	{
		bRes = true;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CQisdaMiscellaneousModule::GetReleaseDate(char* szDate12, char* szTime9, int nTimeOut)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	bool bExecuteResult = false;
	CRequestPtr ptRequest = new CReleaseDateRequest(szDate12, szTime9, bExecuteResult);
	if(Execute(ptRequest, nTimeOut) && bExecuteResult)
	{
		bRes = true;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}