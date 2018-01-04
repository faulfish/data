#pragma once

#include "IRequestManager.h"

#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif

namespace PLATFORM_TOOL
{
	RM_API bool GetIRequestManager(CIRequestManagerPtr& ptIRequestManager,int nCOMPOrt,const std::string& strPlatform);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Sample code
#include "..\RequestManager\Request\ODMRequest.h"

//0. Take care to use in the namespace PLATFORM_TOOL
using namespace PLATFORM_TOOL;

//1. Get IRequestManager

CIRequestManagerPtr m_ptIRequestManager;
GetIRequestManager(m_ptIRequestManager,nComPort);

//2. Open by IRequestManager

if(m_ptIRequestManager->IsOpen() == false)
	m_bConnected = m_ptIRequestManager->Open();
else
	m_bConnected = true;

//3. Execuate by IRequestManager

bool CQualcommDiag::GetMotoXPRS(char* szReceiverBuffer,int nBufferLength,int nTimeOut)
{
	bool bRes = false;
	if(m_bConnected)
	{
		int nMotoXPRSDATASize = sizeof(MotoXPRSDATA);
		if(nBufferLength >= nMotoXPRSDATASize )
		{
			using namespace PLATFORM_TOOL;
			CRequestPtr ptRequest = new CMotoRequestBase;

			if(m_ptIRequestManager->Execuate(ptRequest))
			{
				CBufferPtr	ptBufferReceived;
				ptRequest->GetReceiveData(ptBufferReceived);
				if(ptBufferReceived->GetLength())
					bRes = true;
			}
		}
	}
	else
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	return bRes;
}

*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////