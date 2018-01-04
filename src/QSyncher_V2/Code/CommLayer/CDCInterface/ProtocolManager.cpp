/*****************************************************************************/
/** @file:		ProtocolManager.cpp
This is the file that contains the functions related to the ProtocolManager
class.
*/
/*****************************************************************************/
#include "stdafx.h"
#include "ProtocolManager.h"
#include "..\..\CommonUtil\LogPolicy.h"

CProtocolManager g_ProtocolManager;

CDCINTERFACE_CLASS_DLLEXT void CDCExist()
{
	g_ProtocolManager.Shutdown();
}

/*---------------------------------------------------------------------------*/
/*!
construction

@param		None
@return		None
*/
CProtocolManager::CProtocolManager()
:m_bIsConnected(FALSE)
,m_dwNoOfExpectedBytes(1600)
,m_ptrCallBack(NULL)
,m_IConfigure(QSYNC_LIB::CreateIConfigure(_T("TraceLog")))
,m_nEnableLog(m_IConfigure,_T("EnableTraceLog"),0)
{
	m_TransportLayer.Advise(this);
	m_TransportLayer.Startup();

	if(m_IConfigure)
		m_IConfigure->Load(QSYNC_LIB::STORAGE_INIFILE);
}

/*---------------------------------------------------------------------------*/
/*!
destruction

@param		None
@return		None
*/
CProtocolManager::~CProtocolManager()
{
	if(m_IConfigure)
		QSYNC_LIB::DeleteIConfigure(_T("TraceLog"));
}

void CProtocolManager::Shutdown()
{
	m_TransportLayer.Shutdown();
}

CTransportLayer *CProtocolManager::GetTransportLayerPtr()
{
	return &m_TransportLayer;
}

/*---------------------------------------------------------------------------*/
/*!
set the com port

@param		CString cstrPort - the com port
@return		HRESULT - S_OK if successful; E_FAIL if failed
*/
HRESULT CProtocolManager::SetPort(CString cstrPort,int nBaudRate)
{
	HRESULT hr = S_OK;

	m_hConnectedHandle.ResetEvent();

	m_TransportLayer.Open(cstrPort,nBaudRate); 

	hr = ::WaitForSingleObject(m_hConnectedHandle, INFINITE);

	if (hr == WAIT_OBJECT_0)
		m_TransportLayer.Read(0);
	else ASSERT(0); //Fail to set port, but why ?

	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
close the com port

@param		None
@return		HRESULT - S_OK if successful; E_FAIL if failed
*/
HRESULT CProtocolManager::ResetPort()
{
	HRESULT hr = S_OK;	

	m_TransportLayer.Close();

	hr = ::WaitForSingleObject(m_hDisconnectedHandle, 5000);

	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
trigger the transport layer event

@param		CTLEventPtr spTLEvent - the smart pointer points to the TLEvent
@return		None
*/
void CProtocolManager::TransportLayerEvent(CTLEventPtr spTLEvent)
{
	spTLEvent->FeedToConsumer(this);
}

/*---------------------------------------------------------------------------*/
/*!
the callback function when the connection is established

@param		None
@return		None
*/
void CProtocolManager::OnTLEventConnected()
{
	m_bIsConnected = TRUE;
	m_hConnectedHandle.SetEvent();
	m_hDisconnectedHandle.ResetEvent();
}    

/*---------------------------------------------------------------------------*/
/*!
the callback function when the connection is terminated

@param		None
@return		None
*/
void CProtocolManager::OnTLEventDisconnected()
{
	m_bIsConnected = FALSE;
	m_hDisconnectedHandle.SetEvent();
}

/*---------------------------------------------------------------------------*/
/*!
the callback function when write completed

@param		None
@return		None
*/
void CProtocolManager::OnTLEventWriteCompleted()
{
	if(m_ptrCallBack)
		m_ptrCallBack->OnWriteCompleted();
}

void CProtocolManager::TraceBinary(CBufferPtr spBuffer)
{
	if(m_nEnableLog == 1)
	{
		CSingleLock singleLock(&m_CritSection,TRUE);

		const int nLenMax = 32;
		char szCharTemp[nLenMax+1] = {0};
		int nLineCount = 0;

		CStringA strLog;

		const char* szTemp = spBuffer->GetData();
		for(DWORD i=0;i<spBuffer->GetLength();i++)
		{		
			if(i > 0 && i % nLenMax == 0)
			{
				strLog += "   ";
				for(int j=0;j<nLenMax;j++)
				{
					CStringA strTemp;
					strTemp.Format("%c",szCharTemp[j]);
					strLog += strTemp;
				}
				strLog += "\n";
				nLineCount = 0;
			}
			BYTE nTest = szTemp[i];
			CStringA strTemp;
			strTemp.Format("%02X ",nTest);
			strLog += strTemp;
			szCharTemp[i%nLenMax] = (nTest <= 0x1F || nTest >= 0xFF) ? '.' : szTemp[i];
			nLineCount++;
		}

		for(int nSpaceCount = 0;nSpaceCount < (nLenMax - nLineCount);nSpaceCount++)	
			strLog += "   ";

		strLog += "   ";
		for(int j=0;j<nLineCount;j++)
		{		
			CStringA strTemp;
			strTemp.Format("%c",szCharTemp[j]);
			strLog += strTemp;
		}
		strLog += "\n\n";

		TRACE(_T("%s"),strLog);
		LogPolicy::DebugLog(LPCSTR(strLog),_T("CDCData.txt"));
	}
}

bool RemoveSysInfo(CBufferPtr& ptBuffer)
{
	bool bRes = true;
	const char* pData = ptBuffer->GetData(); 
	const char* pLastData = pData + (ptBuffer->GetLength() - 8);
	if((ptBuffer->GetLength() >= 3) && (memcmp(pData,"+++",3) == 0))
		ptBuffer->PopFront(3);
	else if((ptBuffer->GetLength() >= 8) && (memcmp(pData,"\r\nRING\r\n",8) == 0))
		ptBuffer->PopFront(8);
	else if((ptBuffer->GetLength() >= 8) && (memcmp(pLastData,"\r\nRING\r\n",8) == 0))
		ptBuffer->ReleaseBuffer(ptBuffer->GetLength() - 8);
	else if((ptBuffer->GetLength() >= 8) && (memcmp(pData,"\r\nBUSY\r\n",8) == 0))
		ptBuffer->PopFront(8);
	else if((ptBuffer->GetLength() >= 13) && (memcmp(pData,"\r\nNO ANSWER\r\n",13) == 0))
		ptBuffer->PopFront(13);
	else if((ptBuffer->GetLength() >= 14) && (memcmp(pData,"\r\nNO CARRIER\r\n",14) == 0))
		ptBuffer->PopFront(14);
	else if((ptBuffer->GetLength() >= 15) && (memcmp(pData,"\r\nNO DIALTONE\r\n",15) == 0))
		ptBuffer->PopFront(15);
	else
		bRes = false;
	return bRes;
}

/*---------------------------------------------------------------------------*/
/*!
the callback function when data arrived from the handset

@param		CBufferPtr spBuffer - the smart pointer points to the buffer 
that contains the data from the handset
@return		None
*/
void CProtocolManager::OnTLEventDataArrival(CBufferPtr spBuffer)
{	
	//TRACE(_T("Raw data from CProtocolManager::OnTLEventDataArrival : %d\n"),spBuffer->GetLength());
	TraceBinary(spBuffer);
	
	RemoveSysInfo(spBuffer);

	if(m_ptrCallBack && spBuffer->GetLength() > 0)
		m_ptrCallBack->OnDataArrival(spBuffer);

	m_TransportLayer.Read(m_dwNoOfExpectedBytes);
}

/*---------------------------------------------------------------------------*/
/*!
excute the protocol package

@param		CBufferPtr spBuffer - the smart pointer points to the buffer 
that contains the data to be deliver to the handset
@param		CProtocolManagerCallback* ptrCallBack - the pointer that 
indicate wheere the callback function is
@return		HRESULT - S_OK if successful; E_FAIL if failed
*/
HRESULT CProtocolManager::ExecuteProtocolRequest(CBufferPtr spBuffer, CProtocolManagerCallback* ptrCallBack)
{
	HRESULT hr = S_FALSE;

	ASSERT(IsConnected());
	if(IsConnected())
	{
		//Trace log
		//TRACE(_T("Raw data from CProtocolManager::ExecuteProtocolRequest : %d\n"),spBuffer->GetLength());
		TraceBinary(spBuffer);

		//Write to m_TransportLayer (like comport)
		m_ptrCallBack = ptrCallBack;
		m_TransportLayer.Write(spBuffer);
		hr = S_OK;
	}

	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
unregister callback function

@param		CProtocolManagerCallback* ptrCallBack - the pointer that 
indicate where the callback function is
@return		none
*/
void CProtocolManager::UnRegisterCallBack(CProtocolManagerCallback* ptrCallBack)
{
	if(m_ptrCallBack == ptrCallBack)
		m_ptrCallBack = NULL;
}
