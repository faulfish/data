#pragma once

#include "PotocolRequestBase.h"
#include <vector>
#include <list>
#include <string.h>

using namespace std;

#ifndef TSTRING
	#ifdef _UNICODE
		#define TSTRING wstring
		#define _TC(x) L ## x
		#define _tfopen_s   _wfopen_s
	#else
		#define TSTRING string
		#define _TC(x) "x"
	#endif
#endif

class SyncMLPolicy
{
public:
	static enumServiceType GetServiceType() { return SERVICE_OBEX_SYNCML; }
	static enumProtocolState GetProtocolType() { return STATE_OBEX; }
	static CBufferPtr CreateConnection(){return CObexCodec::CreateReqOBEXConnectSyncML();}
};

class FSPolicy
{
public:
	static enumServiceType GetServiceType() { return SERVICE_OBEX_FS; }
	static enumProtocolState GetProtocolType() { return STATE_OBEX; }
	static CBufferPtr CreateConnection(){return CObexCodec::CreateReqOBEXConnectFileSystem();}
};

template <class OBEXTypePolicy>
class COBEXRequestBase : public CPotocolRequestBase
{
public:
	virtual const enumServiceType GetServiceType() const { return OBEXTypePolicy::GetServiceType(); }
	virtual const enumProtocolState GetProtocolType() const { return OBEXTypePolicy::GetProtocolType(); }
	virtual const bool				DoCanClose()					{return true;}
	virtual const bool				DoIsOpen()						{return m_bIsOpen();}
	virtual void					DoSetOpen(const bool bIsOpen)	{m_bIsOpen() = bIsOpen;}	

protected:
	COBEXRequestBase(DWORD dwTimeout = 3000):CPotocolRequestBase(dwTimeout),m_bOpenOrCloseing(false){}
	virtual ~COBEXRequestBase(){}
public:
	virtual const bool DoOpen()
	{
		if(!m_bIsOpen())
		{
			m_bOpenOrCloseing = true;
			CBufferPtr ptTempBuffer = new CBuffer;
			ptTempBuffer = m_ptBufferSend;
			m_ptBufferSend = OBEXTypePolicy::CreateConnection(); 
			DoRequest();

			int nRet = -1;
			CBufferPtr spBuffer = GetReceiver();
			if(spBuffer->GetLength() > 0)
			{
				BYTE* pBuffer = (BYTE*)spBuffer->GetData();		
				nRet = pBuffer[0];
			}

			assert(nRet == 0xA0);

			m_ptBufferSend = ptTempBuffer;
		}
		return IsOpen();
	}
	virtual const bool DoClose()
	{
		if(m_bIsOpen())
		{
			m_bOpenOrCloseing = true;
			CBufferPtr ptTempBuffer = new CBuffer;
			ptTempBuffer = m_ptBufferSend;
			m_ptBufferSend = CObexCodec::CreateReqOBEXDisconnect(m_dwConnID());
			DoRequest();
			m_ptBufferSend = ptTempBuffer;
		}
		return !IsOpen();
	}
	virtual const bool  DoIsFinishPackage(CBufferPtr& spBuffer)
	{
		CObexCodecPtr ptObex = new CObexCodec;
		return ptObex->IsCompletePackage(m_ptBufferReceiver);
	}
	virtual const bool DoDataArrival(CBufferPtr& spBuffer)
	{
		bool bRes = m_bOpenOrCloseing;
		if(m_bOpenOrCloseing)
		{
			if(m_bIsOpen() == false)
			{
				if(spBuffer->GetLength() > 0)
				{
					BYTE* pBuffer = (BYTE*)spBuffer->GetData();
					DoSetOpen((pBuffer[0] == 0xA0));
					if(IsOpen())
					{
						CObexCodecPtr ptObex = new CObexCodec();
						ptObex->DecodePackage(spBuffer,CObexCodec::oreqConnect);
						m_dwConnID() = ptObex->GetConnId();
						m_dwMaxObexPackageSize() = ptObex->GetMaxObexPackageSize();
					}
				}
			}
			else
			{
				if(spBuffer->GetLength() > 0)
				{
					BYTE* pBuffer = (BYTE*)spBuffer->GetData();
					DoSetOpen(!(pBuffer[0] == 0xA0));
				}
			}
			m_bOpenOrCloseing = false;
		}
		else bRes = true;
		return bRes;
	}
	virtual const bool DoRequest(bool bResursive = false)
	{	
		m_ptBufferReceiver  = new CBuffer;
		bool bRes = g_ProtocolManager.ExecuteProtocolRequest(m_ptBufferSend, this) == S_OK;
		if(bRes)
		{
			WaitReceiver();
			if(m_bOpenOrCloseing)
				COBEXRequestBase<OBEXTypePolicy>::DoDataArrival(GetReceiver());
			else
			{
				while(DoDataArrival(GetReceiver()) == false);
				Fire(_T("Received"));
			}
			bRes = GetReceiver()->GetLength() > 0;
		}		
		return bRes;
	}
	virtual int GetReceiverResult(const CBufferPtr& ptReceiver)
	{
		int nRet = -1;
		if(ptReceiver->GetLength() > 0)
		{
			BYTE* pBuffer = (BYTE*)ptReceiver->GetData();
			nRet = pBuffer[0];
		}
		return nRet;
	}
protected:
	static bool& m_bIsOpen()
	{
		static bool bIsOpen = false;
		return bIsOpen;
	}
	static DWORD& m_dwConnID()
	{
		static DWORD dwConnID = -1;
		return dwConnID;
	}
	static DWORD& m_dwMaxObexPackageSize()
	{
		static DWORD dwMaxObexPackageSize = 0;
		return dwMaxObexPackageSize;
	}
private:
	bool m_bOpenOrCloseing;
};

typedef COBEXRequestBase<SyncMLPolicy>	COBEXSyncMLBase;
typedef COBEXRequestBase<FSPolicy>		COBEXFSBase;
