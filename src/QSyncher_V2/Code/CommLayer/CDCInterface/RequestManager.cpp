/*****************************************************************************/
/** @file:		RequestManager.cpp
This is the file that contains the functions related to the RequestManager
class.
*/
/*****************************************************************************/
#include "stdafx.h"
#include "RequestManager.h"
#include "GeneralRequest.h"

CRequestManager g_RequestManager;

CRequestManager::CRequestManager():m_evReqAvl(FALSE,TRUE,NULL,NULL),
m_evShutDown(FALSE,TRUE,NULL,NULL),
m_bShutDown(FALSE),
m_eServiceType(SERVICE_AT_CMD),
m_enumProtocolState(STATE_AT_CMD),
m_spPrevRequest(NULL),
m_spWorkingRequest(NULL)
{
}

CRequestManager::~CRequestManager()
{
	ShutDown();
}

void CRequestManager::ResetWorkingRequest()
{
	if (m_spWorkingRequest != NULL)
	{
		m_spPrevRequest = m_spWorkingRequest;
		m_spWorkingRequest = NULL;
	}
}

void CRequestManager::ResetPrevRequest()
{
	if (m_spPrevRequest != NULL)
		m_spPrevRequest = NULL;
}

BOOL CRequestManager::StartUp()
{
	return TRUE;
}

void CRequestManager::ShutDown()
{
	ResetWorkingRequest();
	ResetPrevRequest();
}

HRESULT CRequestManager::EnqueueRequest(CRequestPtr spRequest,bool bAutoUnlockToken)
{
	return DoWorkingRequest(spRequest,bAutoUnlockToken);
}

HRESULT CRequestManager::DoWorkingRequest(CRequestPtr spRequest,bool bAutoUnlockToken)
{	
	HRESULT hr = S_OK;	

	assert(m_token.IsLock() == false);//for template check
	if(m_token.IsLock() == false)
	{
		ResetWorkingRequest();
		m_spWorkingRequest = spRequest;

		if(!IsConnected() && (m_spWorkingRequest->GetServiceType() != SERVICE_CONNECTION))
		{
			m_spWorkingRequest->Abort();
		}
		else
		{
			m_token.Lock();

			hr = EnvironmentSetup();
			SetWorkingRequestState(STATE_RUNNING);

			if(bAutoUnlockToken == false)
				m_spWorkingRequest->SetIRequestToken(&m_token);

			hr = m_spWorkingRequest->ExecuteRequest();

			if(hr == S_OK)
				SetWorkingRequestState(STATE_COMPLETED_SUCCEEDED);
			else
				SetWorkingRequestState(STATE_COMPLETED_FAILED);

			if(bAutoUnlockToken)
				m_token.Unlock();
		}
	}
	return hr;
}

HRESULT CRequestManager::SwitchProtocol(enumProtocolState enumProtocolState)
{
	HRESULT hr = E_FAIL;
	if(m_enumProtocolState != enumProtocolState)
	{
		switch(enumProtocolState)
		{
		case STATE_AT_CMD:
			{				
				CRequestPtr spRequest = new CRequestSwitchOBEXToAT();
				hr = spRequest->ExecuteRequest();
			}
			break;
		case STATE_OBEX:
			{			
				CRequestPtr spRequest;
				/*
				CRequestPtr spRequest = new CRequestSwitchDriver();
				hr = spRequest->ExecuteRequest();

				spRequest = new CRequestResetPort();
				spRequest->ExecuteRequest();

				spRequest = new CRequestSetPort(_T("com1"),57600);
				spRequest->ExecuteRequest();
				*/

				spRequest = new CRequestSwitchATToOBEX();
				hr = spRequest->ExecuteRequest();
			}
			break;
		default:
			break;
		};
	}
	if(SUCCEEDED(hr))
		m_enumProtocolState = enumProtocolState;
	return hr;
}

HRESULT CRequestManager::EnvironmentSetup()
{
	HRESULT hr = S_OK;	
	if(m_spPrevRequest != NULL && m_eServiceType != m_spWorkingRequest->GetServiceType())
	{
		if(m_spPrevRequest->Close() == CONNECTION_SUCCEEDED)
		{				
			if(m_spWorkingRequest->GetProtocolType() != m_enumProtocolState)
				SwitchProtocol(m_spWorkingRequest->GetProtocolType());
			if(m_spWorkingRequest->Open() == CONNECTION_SUCCEEDED)
				SetCurServiceType(m_spWorkingRequest->GetServiceType());
		}
	}
	return hr;
}