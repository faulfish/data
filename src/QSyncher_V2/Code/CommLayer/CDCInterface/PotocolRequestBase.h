#pragma once

#include "Request.h"
#include "ProtocolManager.h"

class CPotocolRequestBase: public CProtocolManager::CProtocolManagerCallback, public CRequest
{
protected:
	CPotocolRequestBase(DWORD dwTimeout = 3000):m_ptBufferSend(new CBuffer),m_dwTimeOut(dwTimeout),m_HandleReceiveEvent(FALSE,TRUE)
		,m_hDeviceConnectionStatusEvent(FALSE,TRUE,_T("MobileDeviceConnectionEvent"))
	{
		DefineNotify(_T("Writen"));
		DefineNotify(_T("Received"));
	}
	virtual ~CPotocolRequestBase(void)
	{
	}
public:
	enumProtocolConnection Open()
	{
		TRACE_FILE_LINE_INFO();
		if(!IsOpen())
			DoSetOpen(DoOpen());

		if (IsOpen())
			return CONNECTION_SUCCEEDED;
		else
			return CONNECTION_FAILED;
	}
	enumProtocolConnection Close()
	{
		TRACE_FILE_LINE_INFO();
		bool bRes = IsOpen();
		if(bRes)
		{
			if(CanClose())
			{
				bRes = DoClose();
				DoSetOpen(!bRes);
			}
			else
				return CONNECTION_REJECTED;
		}

		if (!IsOpen())
			return CONNECTION_SUCCEEDED;
		else
			return CONNECTION_FAILED;
	}
	HRESULT ExecuteRequest()
	{
		TRACE_FILE_LINE_INFO();
		HRESULT hr = E_FAIL;
		if(IsOpen())
			hr = DoRequest() ? S_OK : E_FAIL;
		return hr;
	}
	virtual void OnDataArrival(CBufferPtr spBuffer)
	{		
		TRACE_FILE_LINE_INFO();
		::WaitForSingleObject(m_HandleReceiveEvent,INFINITE);

		bool bEcho = false;
		if(GetProtocolType() == STATE_AT_CMD)
		{
			//1. remove system information
			RemoveSysInfo(spBuffer);

			//2. remove echo to replace of ATZ=1\r
			if(m_ptBufferSend->GetLength() > 0 && spBuffer->GetLength() >= m_ptBufferSend->GetLength())
				bEcho = memcmp(m_ptBufferSend->GetData(),spBuffer->GetData(),m_ptBufferSend->GetLength()) == 0;
			if(bEcho)
			{
				spBuffer->PopFront(m_ptBufferSend->GetLength());
				m_ptBufferSend = new CBuffer;
			}
		}
		if(spBuffer->GetLength() > 0)
		{
			GetReceiver()->Append(spBuffer);
			if(DoIsFinishPackage(GetReceiver()))
				EnableReceiveEvent(true);
		}
	}
	virtual void OnWriteCompleted(){Fire(_T("Writen"));}
protected:
	bool WaitReceiver()
	{
		TRACE_FILE_LINE_INFO();
		EnableReceiveEvent(false);
		
		HANDLE hEvents[] = {m_ReceiveEvent.m_hObject,m_hDeviceConnectionStatusEvent.m_hObject};
		DWORD dwRes = ::WaitForMultipleObjects(2,hEvents,FALSE,m_dwTimeOut);
        //DWORD dwRes = ::WaitForSingleObject(m_ReceiveEvent,m_dwTimeOut);

		ASSERT(WAIT_TIMEOUT != dwRes);//No response
		return WAIT_TIMEOUT != dwRes;
	}
	void EnableReceiveEvent(bool bSet)
	{
		CSingleLock singleLock(&m_Critsection,TRUE);

		if(bSet)
		{
			TRACE_FILE_LINE_INFO();
			m_HandleReceiveEvent.ResetEvent();
			m_ReceiveEvent.SetEvent();
		}
		else
		{
			TRACE_FILE_LINE_INFO();
			m_HandleReceiveEvent.SetEvent();
			m_ReceiveEvent.ResetEvent();
		}
	}
	const bool IsOpen(){return DoIsOpen();}
	const bool CanClose(){return DoCanClose();}
	const DWORD GetTimeout() const {return m_dwTimeOut;}
	void SetTimeout(DWORD dwTimeOut) {m_dwTimeOut = dwTimeOut;}

    //-----------------------------------------------------------------------
    // Modified by Eddu Guo.
    // Modify this method into virtual, because there are many type of "OK"
    // If a request has different return status, you should modify this.
    // There is an example => See class CRequestListMessageTi. 
    //-----------------------------------------------------------------------
	virtual bool IsOK(const CBufferPtr& ptBuffer)
	{		
		TRACE_FILE_LINE_INFO();
		bool bRes = false;
		BYTE szATOK[6] = {0x0d,0x0a,'O','K',0x0d,0x0a};
		if(ptBuffer->GetLength() >= 6)
		{
			BYTE* pTemp = (BYTE*)ptBuffer->GetData();
			BYTE* pEnd = pTemp + (ptBuffer->GetLength()-6);
			bRes = memcmp(pEnd,szATOK,6) == 0;
		}
		return bRes;
	}
    //-----------------------------------------------------------------------
    // Modified by Eddu Guo.
    // Modify this method into virtual, because there are many type of "FAIL"
    //-----------------------------------------------------------------------
	virtual bool IsFAIL(const CBufferPtr& ptBuffer)
	{
		TRACE_FILE_LINE_INFO();
		bool bRes = false;
		BYTE szATFAIL[9] = {0x0d,0x0a,'E','R','R','O','R',0x0d,0x0a};
		if(ptBuffer->GetLength() >= 9)
		{
			BYTE* pTemp = (BYTE*)ptBuffer->GetData();
			BYTE* pEnd = pTemp + (ptBuffer->GetLength()-9);
			bRes = memcmp(pEnd,szATFAIL,9) == 0;
		}
		return bRes;
	}
	virtual bool IsCMEError(const CBufferPtr& ptBuffer)
	{
		bool bRes = false;
		/*
		000083: Bulk or Interrupt Transfer (UP), 19.12.2007 11:42:45.3312843 +0.0 Pipe Handle: 0x8648884c Get 0x1e bytes from the device:
		0D 0A 2B 43 4D 45 20 45 52 52 4F 52 3A 20 6D 65
		6D 6F 72 79 20 66 61 69 6C 75 72 65 0D 0A
		..+CME ERROR: me
		mory failure..
		*/
		BYTE szATCMEERROR[12] = {0x0d,0x0a,'+','C','M','E',' ','E','R','R','O','R'};
		if(ptBuffer->GetLength() >= 12)
		{
			BYTE* pTemp = (BYTE*)ptBuffer->GetData();
			bRes = memcmp(pTemp,szATCMEERROR,12) == 0;
		}
		return bRes;
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
	const bool IsCompleteATPackage(const CBufferPtr& ptBuffer)
	{
		TRACE_FILE_LINE_INFO();
		return IsOK(ptBuffer) || IsFAIL(ptBuffer) || IsCMEError(ptBuffer);
	}
protected:
	virtual const bool	DoClose()			= 0;
	virtual const bool	DoOpen()			= 0;	
	virtual const bool DoRequest(bool bResursive = false)
	{
		TRACE_FILE_LINE_INFO();
		GetReceiver()  = new CBuffer;
		bool bRes = g_ProtocolManager.ExecuteProtocolRequest(m_ptBufferSend, this) == S_OK;
		if(bRes)
		{
			WaitReceiver();
			while(DoDataArrival(GetReceiver()) == false);			
			Fire(_T("Received"));
			bRes = GetReceiver()->GetLength() > 0;
		}		
		return bRes;
	}
	virtual void		DoSetOpen(const bool bIsOpen)	= 0;
	virtual const bool	DoIsOpen()						= 0;
	virtual const bool	DoCanClose()					= 0;
	virtual const bool	DoDataArrival(CBufferPtr& spBuffer) = 0;
	virtual const bool  DoIsFinishPackage(CBufferPtr& spBuffer) = 0;
protected:	
	CBufferPtr							m_ptBufferSend;
private:
	CEvent								m_HandleReceiveEvent;
	CEvent								m_ReceiveEvent;
	DWORD								m_dwTimeOut;
	CCriticalSection				m_Critsection;
	CEvent								m_hDeviceConnectionStatusEvent;
};
