#ifndef _REQUESTBASE_H_
#define _REQUESTBASE_H_

#pragma once

#include "RequestManager.h"

class RequestFunctionBase : public QSYNC_LIB::INotify
{
public:	
	//INotify
	virtual void Event(const TSTRING& strEvent,long nParam)
	{
		if(strEvent == _T("Received"))
			m_ReceiveEvent.SetEvent();
	}
protected:
	RequestFunctionBase(int nTimeOut = 3000)
		:m_nTimeOut(nTimeOut)
		,m_hDeviceConnectionStatusEvent(FALSE,TRUE,_T("MobileDeviceConnectionEvent"))
	{}
	virtual ~RequestFunctionBase(){}
	int Execute(CRequestPtr& ptRequest,int nTimeOut)
	{
		ptRequest->Register(this,_T("Received"));	
		m_ReceiveEvent.ResetEvent();
		if (g_RequestManager.EnqueueRequest(ptRequest) == S_OK)
		{
			if(nTimeOut == 0)
				nTimeOut = m_nTimeOut;
			HANDLE hEvents[] = {m_ReceiveEvent.m_hObject,m_hDeviceConnectionStatusEvent.m_hObject};
			DWORD dwRes = ::WaitForMultipleObjects(2,hEvents,FALSE,nTimeOut);
		}
		ptRequest->Unregister(this,_T("Received"));	

		return ptRequest->GetReceiverResult(ptRequest->GetReceiver());
	}
protected:
	CEvent			m_ReceiveEvent;
	int				m_nTimeOut;
	CEvent				m_hDeviceConnectionStatusEvent;
};

#endif //_REQUESTBASE_H_