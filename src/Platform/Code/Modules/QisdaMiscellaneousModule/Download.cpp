#include "StdAfx.h"
#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include "..\..\CommonUtil\unicodeutility.h"
#include "Download.h"
#include "..\..\RequestManager\Request\DownloadRequest.h"

CDownload::CDownload(int nCOMPort):CInterfaceBase(nCOMPort)
{
}

CDownload::~CDownload(void)
{
}

bool CDownload::GetDLBootLoaderRevocation(int& nStatus,int& nVersion)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	CRequestPtr ptRequest = new CDLBootLoaderRevacationRequest(nStatus,nVersion);	
	bRes = Execute(ptRequest);

	return bRes;
}

bool CDownload::GetDLPhoneCodeRevocation(int& nStatus,int& nVersion)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	CRequestPtr ptRequest = new CDLPhoneCodeRevacationRequest(nStatus,nVersion);	
	bRes = Execute(ptRequest);

	return bRes;
}