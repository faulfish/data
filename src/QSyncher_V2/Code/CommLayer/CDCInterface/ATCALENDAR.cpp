#include "stdafx.h"
#include "ATCALENDAR.h"
#include "ATVCALENDARRequest.h"

ICALENDAR *g_pSingleton = NULL;

ICALENDAR* CreateATCALENDAR()
{
    if(g_pSingleton == NULL)
		g_pSingleton = new CATCALENDAR();
	return g_pSingleton;
}

void DeleteATCALENDAR()
{
    if(g_pSingleton)
	{
		delete g_pSingleton;
		g_pSingleton = NULL;
	}    
}

bool CATCALENDAR::SetOperationTimeOut(int nTimeOut)
{
	return false;
}

bool CATCALENDAR::GetRange(UINT &nMinIdx, UINT &nMaxIdx, UINT &nMaxCount)
{	
	CRequestPtr ptRequest = new CVCALENDARRangeRequest(&nMinIdx,&nMaxIdx,&nMaxCount,m_nTimeOut);
	return Execute(ptRequest,m_nTimeOut) > 0;	
}

bool CATCALENDAR::ListIndex( vector<UINT> *pvIdxList)
{	
	CRequestPtr ptRequest = new CVCALENDARListRequest(pvIdxList,m_nTimeOut);	
	return Execute(ptRequest,m_nTimeOut) > 0;
}

bool  CATCALENDAR::ReadByIndex(UINT nIdx, char *cVCardbuf, int nBufMaxLen, int &nVcardLen)
{	
	bool bRes = false;
    if(cVCardbuf == NULL || nBufMaxLen == 0)
		bRes = false;

	CBufferPtr spbufData = new CBuffer;
	CRequestPtr ptRequest = new CVCALENDARReadRequest(nIdx,spbufData,m_nTimeOut);
	bRes = Execute(ptRequest,m_nTimeOut) > 0;
	if(bRes)
	{
		assert(spbufData->GetLength() <= static_cast<DWORD>(nBufMaxLen));
		if(spbufData->GetLength() <= static_cast<DWORD>(nBufMaxLen))
			memcpy(cVCardbuf,spbufData->GetData(),spbufData->GetLength());
		else bRes = false;
	}
	return bRes;
}

bool  CATCALENDAR::WriteByIndex(UINT& nIdx, char *cVCardbuf, int nBufLen)
{
	bool bRes = false;
    if(cVCardbuf == NULL || nBufLen == 0)
		bRes = false;

	int nRetError  = -1;
	CRequestPtr ptRequest = new CVCALENDARWriteRequest(cVCardbuf,nBufLen,nIdx,m_nTimeOut,nIdx,nRetError);
	return Execute(ptRequest,m_nTimeOut) > 0;
}

bool  CATCALENDAR::RemoveByIndex(UINT nIdx)
{
	bool bRes = false;
	CRequestPtr ptRequest = new CVCALENDARDeleteRequest(nIdx,m_nTimeOut,bRes);
	return Execute(ptRequest,m_nTimeOut) > 0 && bRes;
}
