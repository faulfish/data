#ifndef __REQUEST_BASE_H__
#define __REQUEST_BASE_H__
#pragma once
#include "..\..\CommonUtil\RefObject.h"
#include "..\..\CommonUtil\Buffer.h"
#include <assert.h>
#include <string>

#pragma warning(push)
#pragma warning(disable:4275)

namespace PLATFORM_TOOL
{
#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif

	enum PotocolType
	{
		NONE,
		DIAG,		//DIAG mode
		DL,			//Download mode
		QLIB,		//Q_LIB
		AT			//AT command
	};

	class CRequestBase  : public CRefObject
	{
	public:
		CRequestBase(PotocolType nPotocolType,int	nTimeout = 5000,bool bHadReturnvalue = true)
			:m_nTimeout(nTimeout)
		,m_nPotocolType(nPotocolType)
		,m_bHadReturnvalue(bHadReturnvalue)
		,m_hFinishEvent(::CreateEvent(NULL,TRUE,FALSE,NULL))
		,m_ptBufferReceived(new CBuffer)
		{
			::ResetEvent(m_hFinishEvent);
		}
		CRequestBase(const TSTRING& strName,PotocolType nPotocolType,int	nTimeout = 5000,bool bHadReturnvalue = true)
			:m_nTimeout(nTimeout)
		,m_nPotocolType(nPotocolType)
		,m_bHadReturnvalue(bHadReturnvalue)
		,m_hFinishEvent(::CreateEvent(NULL,TRUE,FALSE,NULL))
		,m_strName(strName)
		,m_ptBufferReceived(new CBuffer)
		{
			::ResetEvent(m_hFinishEvent);
		}
		virtual ~CRequestBase(void)
		{
			if(m_hFinishEvent)
			{
				::CloseHandle(m_hFinishEvent);
				m_hFinishEvent = NULL;
			}
		}
		const TSTRING& GetName() const {return m_strName;}
		virtual PotocolType GetPotocolType() const 
		{
			return m_nPotocolType;
		}
		virtual void GetSendData(CBufferPtr& ptBufferSend) 
		{
			ptBufferSend = m_ptBufferSend;
		}
		virtual bool GetReceiveData(CBufferPtr& ptBufferReceived) 
		{
			ptBufferReceived = m_ptBufferReceived;
			return true;
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived) 
		{
			m_ptBufferReceived = ptBufferReceived;
			if(m_hFinishEvent)
				::SetEvent(m_hFinishEvent);
		}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			assert(0);
			return false;
		}
		virtual int GetTimeout() const
		{
			return m_nTimeout;
		}
        virtual void SetTimeout(int ntimeout) {
            m_nTimeout = ntimeout;
        }
		virtual bool IsHadReturnValue() const
		{
			return m_bHadReturnvalue;
		}
		virtual bool WaitForWrite()
		{
			bool bRes = false;
			if(IsHadReturnValue())
			{
				if(m_hFinishEvent)
					bRes = WAIT_OBJECT_0 == ::WaitForSingleObject(m_hFinishEvent,m_nTimeout);
			}
			else
			{
				bRes = true;
			}
			return bRes;
		}
		virtual bool WaitForOtherResponse() {return false;}
	protected:
		void SetSendData(CBufferPtr& ptBufferSend)
		{
			m_ptBufferSend = ptBufferSend;
		}
	private:
		int					m_nTimeout;
		bool					m_bHadReturnvalue;
		PotocolType	m_nPotocolType;
		CBufferPtr		m_ptBufferSend;
		CBufferPtr		m_ptBufferReceived;
		HANDLE		m_hFinishEvent;
		TSTRING		m_strName;
	};

	typedef CRefObjectPtr<CRequestBase> CRequestPtr;
}

#endif