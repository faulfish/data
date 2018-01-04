#include "stdafx.h"
#include "ATPHONEBOOK.h"
#include "ATVCARDRequest.h"

IPHONEBOOK *g_pSingleton = NULL;

IPHONEBOOK* CreateATPHONEBOOK()
{
    if(g_pSingleton == NULL)
		g_pSingleton = new CATPHONEBOOK();
	return g_pSingleton;
}

void DeleteATPHONEBOOK()
{
    if(g_pSingleton)
	{
		delete g_pSingleton;
		g_pSingleton = NULL;
	}    
}

bool CATPHONEBOOK::GetPhonebookRange(ePhonebookType eType, UINT &nMinIdx, UINT &nMaxIdx, UINT &nMaxCount)
{	
	CRequestPtr ptRequest = new CATVCARDRangeRequest(&nMinIdx,&nMaxIdx,&nMaxCount,eType,m_nTimeOut);
	return Execute(ptRequest,m_nTimeOut) > 0;	
}

bool CATPHONEBOOK::ListPhonebookIndex(ePhonebookType eType, vector<UINT> *pvIdxList)
{	
	CRequestPtr ptRequest = new CATVCARDListRequest(eType,pvIdxList,m_nTimeOut);	
	return Execute(ptRequest,m_nTimeOut) > 0;
}

int  CATPHONEBOOK::ReadVCardbyIndex(ePhonebookType eType, UINT nIdx, char *cVCardbuf, int nBufMaxLen, int &nVcardLen)
{	
    if(cVCardbuf == NULL || nBufMaxLen == 0)
		return eInternalError;

	CBufferPtr spbufData = new CBuffer;
	int nErrCode = 0;
	CRequestPtr ptRequest = new CATVCARDReadRequest(nIdx,spbufData,eType,nErrCode,m_nTimeOut);
	int nRes = Execute(ptRequest,m_nTimeOut) > 0 ? eSuccess : eIndexError;
	if(nRes == eSuccess)
	{
		if(nErrCode == 0)
		{
			assert(spbufData->GetLength() <= static_cast<DWORD>(nBufMaxLen));
			if(spbufData->GetLength() <= static_cast<DWORD>(nBufMaxLen))
				memcpy(cVCardbuf,spbufData->GetData(),spbufData->GetLength());
			else nRes = eMemoryNotEnough;
		}
		else nRes = eInvalidData;
	}
	return nRes;
}

int  CATPHONEBOOK::WriteVCardbyIndex(ePhonebookType eType, UINT& nIdx, int& nRetError, char *cVCardbuf, int nBufLen)
{
    if(cVCardbuf == NULL || nBufLen == 0)
		return eInternalError;

	CRequestPtr ptRequest = new CATVCARDWriteRequest(cVCardbuf,nBufLen,nIdx,eType,m_nTimeOut,nIdx,nRetError);
	return Execute(ptRequest,m_nTimeOut) > 0;
}

int  CATPHONEBOOK::RemoveVCardbyIndex(ePhonebookType eType, UINT nIdx)
{
	CRequestPtr ptRequest = new CATVCARDDeleteRequest(nIdx,eType,m_nTimeOut);
	return Execute(ptRequest,m_nTimeOut) > 0;
}
