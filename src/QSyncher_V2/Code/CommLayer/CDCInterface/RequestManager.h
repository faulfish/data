/*****************************************************************************/
/** @file:		RequestManager.h
  This is the header file that contains the request manager class.
  */
/*****************************************************************************/
#ifndef __REQUESTMANAGER_H__
#define __REQUESTMANAGER_H__

#include "Request.h"
#include "ProtocolManager.h"

class CRequestManager : public CRefObject
{
public:
	CRequestManager();
	~CRequestManager();

	BOOL StartUp();
	void ShutDown();
	HRESULT EnqueueRequest(CRequestPtr spRequest,bool bAutoUnlockToken = true);
	BOOL IsWorkingThreadActive() { return TRUE; }
	BOOL IsConnected() { return g_ProtocolManager.IsConnected(); }

private:
	BOOL FeedRequestThreadCreateFunc();
	void ResetWorkingThread();
	void ResetWorkingRequest();
	void ResetPrevRequest();
	HRESULT EnvironmentSetup();
	HRESULT EnvironmentFree();
	void SetWorkingRequestState(enumRequestState eRequestState) { m_spWorkingRequest->SetReqState(eRequestState); }
	void SetCurServiceType(enumServiceType eServiceType) { m_eServiceType = eServiceType; }
	enumServiceType GetCurServiceType() { return m_eServiceType; }
	HRESULT SwitchProtocol(enumProtocolState enumProtocolState);
	HRESULT DoWorkingRequest(CRequestPtr spRequest,bool bAutoUnlockToken);

private:
	CRequestPtr m_spWorkingRequest, m_spPrevRequest;
	enumServiceType m_eServiceType;
	enumProtocolState m_enumProtocolState;
	
	CEvent            m_evReqAvl;
	CEvent            m_evShutDown;
	BOOL              m_bShutDown;

	CRequestToken				m_token;
};

extern CRequestManager g_RequestManager;

#endif // #ifndef __REQUESTMANAGER_H__
