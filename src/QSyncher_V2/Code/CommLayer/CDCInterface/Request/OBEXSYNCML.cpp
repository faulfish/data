#include "StdAfx.h"
#include "OBEXSYNCML.h"
#include "RequestManager.h"
#include "GeneralRequest.h"

COBEXSYNCML *g_pSingleton_COBEXSYNCML = NULL;

CDCINTERFACE_CLASS_DLLEXT ISYNCML* CreateOBEXISYNCML()
{
	if(g_pSingleton_COBEXSYNCML == NULL)
		g_pSingleton_COBEXSYNCML = new COBEXSYNCML();
	return g_pSingleton_COBEXSYNCML;
}

CDCINTERFACE_CLASS_DLLEXT void DeleteOBEXISYNCML()
{
	if(g_pSingleton_COBEXSYNCML)
	{
		delete g_pSingleton_COBEXSYNCML;
		g_pSingleton_COBEXSYNCML = NULL;
	}
}

COBEXSYNCML::COBEXSYNCML(void):m_nTimeOut(5000)
{
}

COBEXSYNCML::~COBEXSYNCML(void)
{
}

void COBEXSYNCML::Event(const TSTRING& strEvent,long nParam)
{
	if(strEvent == _T("Received"))
		m_ReceiveEvent.SetEvent();
}

void STDMETHODCALLTYPE COBEXSYNCML::SetOperationTimeOut(int nTimeOut)
{
	m_nTimeOut = nTimeOut;
}

CBufferPtr GetOBEXData(const CBufferPtr& ptBuffer)
{	
	CObexCodecPtr ptObex = new CObexCodec;
	ptObex->DecodePackage(ptBuffer,CObexCodec::oreqGet);
	return ptObex->GetBody();
};

int STDMETHODCALLTYPE COBEXSYNCML::Get(BYTE **ppFileData, DWORD& nSize)
{
	int nRet = eSYNCMLSRetFail;
	
	CBufferPtr ptBuffer = new CBuffer;
	CRequestPtr ptRequest = new COBEXSYNCML_Get("application/vnd.syncml+wbxml");
	nRet = Execute(ptRequest);
	while(nRet == eSYNCMLRetSuccessContinue)
	{
		ptBuffer->Append(GetOBEXData(ptRequest->GetReceiver()));
		nRet = Execute(ptRequest);
	}
	if(nRet == eSYNCMLRetSuccess)
	{
		ptBuffer->Append(GetOBEXData(ptRequest->GetReceiver()));
		nSize = ptBuffer->GetLength();
		*ppFileData = new BYTE[nSize];		
		memcpy(*ppFileData,ptBuffer->GetData(),nSize);
	}
	return nRet;
}

int STDMETHODCALLTYPE COBEXSYNCML::Put(BYTE *pFileData, DWORD nSize)
{
	int nRet = eSYNCMLSRetFail;
	CBufferPtr ptFileBuffer = new CBuffer(nSize,(const char*)pFileData);
	CRequestPtr ptRequest = new COBEXSYNCML_Put(ptFileBuffer);
	nRet = Execute(ptRequest);
	return nRet;
}

int  STDMETHODCALLTYPE COBEXSYNCML::Open()
{
	int nRet = eSYNCMLRetSuccess;
	return nRet;
}

int  STDMETHODCALLTYPE COBEXSYNCML::Close()
{
	int nRet = eSYNCMLRetSuccess;
	
	CString strIMEI;
	CRequestPtr spATRequest = new CRequestGetPhoneModel(&strIMEI);
	Execute(spATRequest);

	return nRet;
}

int COBEXSYNCML::Execute(CRequestPtr& ptRequest)
{
	int nRet = eSYNCMLSRetFail;

	ptRequest->Register(this,_T("Received"));
	if (g_RequestManager.EnqueueRequest(ptRequest) == S_OK)
	{
		::WaitForSingleObject(m_ReceiveEvent,m_nTimeOut);
	}
	ptRequest->Unregister(this,_T("Received"));
	if(ptRequest->GetReceiver()->GetLength() > 0)
	{
		BYTE* pBuffer = (BYTE*)ptRequest->GetReceiver()->GetData();
		if(pBuffer[0] == 0xA0)
			nRet = eSYNCMLRetSuccess;
		else if(pBuffer[0] == 0x90)
			nRet = eSYNCMLRetSuccessContinue;
	}
	return nRet;
}
