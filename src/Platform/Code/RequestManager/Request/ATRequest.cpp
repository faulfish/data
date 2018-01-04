#include "StdAfx.h"
#include "ATRequest.h"

namespace PLATFORM_TOOL
{	
	CATBase::CATBase(const TSTRING& strName,int nTimeout,bool bHadReturnvalue)
		:CRequestBase(strName,AT,nTimeout,bHadReturnvalue)
		,m_ptTempBufferReceived(new CBuffer)
	{
	}

	CATBase::~CATBase()
	{
	}

	bool CATBase::IsCompletePacket(CBufferPtr& ptBufferReceived1)
	{//check the string "0x0D 0x0A" at the latest
		m_ptTempBufferReceived->Append(ptBufferReceived1);

		bool bRes = false;
		char *szBuffer = const_cast<char *>(m_ptTempBufferReceived->GetData());
		int nTotalLen = m_ptTempBufferReceived->GetLength();		

		CBufferPtr ptBufferSend;
		GetSendData(ptBufferSend);
		if(nTotalLen == ptBufferSend->GetLength() && memcmp(ptBufferSend->GetData(),szBuffer,nTotalLen) == 0)
		{
			m_ptTempBufferReceived->PopFront(nTotalLen);
			bRes=  false;
		}
		else if(nTotalLen >= 4 && szBuffer[nTotalLen - 2] == 0x0D && szBuffer[nTotalLen - 1] == 0x0A)
		{
			bRes=  true;
		}
		
		return bRes;
	}

	bool CATBase::IsOK(CBufferPtr& ptBuffer)
	{
		bool bRes = false;
		if(ptBuffer->GetLength() == 6)
			bRes = memcmp((void *)ptBuffer->GetData(),"\r\nOK\r\n",6) == 0;
		return bRes;
	}

	bool CATBase::IsFAIL(CBufferPtr& ptBuffer)
	{
		bool bRes = false;
		if(ptBuffer->GetLength() == 8)
			bRes = memcmp((void *)ptBuffer->GetData(),"\r\nFAIL\r\n",8) == 0;
		return bRes;
	}

	bool  CATBase::Encode(const char* szCommand,int nCommandLen,CBufferPtr& ptBufferSend)
	{
		bool bRes = false;
	
		char* szNewBuffer = new char[nCommandLen+1];
		memcpy_s(szNewBuffer,nCommandLen+1,szCommand,nCommandLen);		
		szNewBuffer[nCommandLen] = '\r';
		ptBufferSend = new CBuffer(nCommandLen+1,(const char*)szNewBuffer);
		delete [] szNewBuffer;
		bRes = true;

		return bRes;
	}
	
	
	//20090410 Alan
	//Can't put it on the member data to avoid crash issue
	std::map<std::string,bool> m_strKeyPressMap;
	ISPU_CALLBACK *m_pISPU_CALLBACK;

	CATEnableKeypadEventRequest::CATEnableKeypadEventRequest(bool bEnable,bool& bSuccess,int nTimeout,ISPU_CALLBACK* pISPU_CALLBACK)
		:CATBase(_T("CATEnableKeypadEventRequest"),nTimeout)
		,m_bSuccess(bSuccess)
		//,m_pISPU_CALLBACK(pISPU_CALLBACK)
		,m_bOK(false)
		,m_bWaitForOtherResponse(true)
	{
		m_pISPU_CALLBACK = pISPU_CALLBACK;
		m_strKeyPressMap.clear();

		const char szBufEnable[]={"AT+CMER=1,1,0,0,1"};
		const char szBufDisable[]={"AT+CMER=1,1,0,0,0"};

		CBufferPtr ptBufferSend;
		if(bEnable)
			Encode(szBufEnable,static_cast<int>(strlen(szBufEnable)),ptBufferSend);
		else
			Encode(szBufDisable,static_cast<int>(strlen(szBufDisable)),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CATEnableKeypadEventRequest::~CATEnableKeypadEventRequest()
	{
	}

	bool CATEnableKeypadEventRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = false;
		bRes = CATBase::IsCompletePacket(ptBufferReceived);
		if(bRes && m_bOK == false)
		{
			if(m_bOK || IsOK(ptBufferReceived))
			{
				m_bOK = true;
				bRes = true;
			}
			else if(IsFAIL(ptBufferReceived))
			{
				m_bWaitForOtherResponse = false;
				bRes = true;
			}
		}

		if(bRes && m_bOK && !IsOK(ptBufferReceived))
		{
			//+CKEV: 56,0
			char szBuffer[1024] = {0};
			strncpy_s(szBuffer,1024,ptBufferReceived->GetData(),ptBufferReceived->GetLength());
			char* pszIndex = szBuffer + 8;
			bool bPressUp = false;
			for(DWORD i=0;i<(ptBufferReceived->GetLength() - 8);i++)
			{
				if(pszIndex[i] == ',')
				{
					pszIndex[i] = 0;
					bPressUp = pszIndex[i+1] == '0';
					break;
				}
			}
			if(bPressUp)
			{
				std::map<std::string,bool>::size_type nBefore = m_strKeyPressMap.size();
				m_strKeyPressMap.insert(std::pair<std::string,bool>(pszIndex,true));
				std::map<std::string,bool>::size_type nAfter = m_strKeyPressMap.size();
				if(nAfter > nBefore)
				{
					bRes = true;
#ifdef _DEBUG
					bRes = nAfter >= 25;//25
#else
					bRes = nAfter >= 25;//25
#endif
					m_bSuccess = bRes;
					if(m_pISPU_CALLBACK)
						m_pISPU_CALLBACK->Print("+CKEV",pszIndex);	
				}
			}
		}		
		
		return bRes;
	}

	void CATEnableKeypadEventRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		CATBase::OnDataReceiver(ptBufferReceived);
	}
	
	bool CATEnableKeypadEventRequest::WaitForWrite()
	{
		bool bRes = CATBase::WaitForWrite();
		if(bRes)
		{
			const int nElapse = 500;
			int dwMilliseconds = 0;
			while(CheckBreak() == false && m_bSuccess == false && Timeout(dwMilliseconds) == false)
			{
				dwMilliseconds += nElapse;
				Sleep(nElapse);
			};
			m_bWaitForOtherResponse = false;
			bRes = Timeout(dwMilliseconds) == false;
		}
		return bRes;
	}

	bool CATEnableKeypadEventRequest::WaitForOtherResponse()
	{
		return m_bWaitForOtherResponse;
	}

	bool CATEnableKeypadEventRequest::Timeout(int dwMilliseconds)
	{
		bool bRes = false;
		if(GetTimeout() != -1)
			bRes = dwMilliseconds > GetTimeout();
		return bRes;
	}

	bool CATEnableKeypadEventRequest::CheckBreak()
	{
		bool bRes = false;
		if(m_pISPU_CALLBACK)
			bRes = m_pISPU_CALLBACK->CheckBreak();
		return bRes;
	}
}