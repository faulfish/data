/*****************************************************************************/
/** @file:		Request.h
This is the header file that contains the base request class.
*/
/*****************************************************************************/
#ifndef __REQUEST_H__
#define __REQUEST_H__

#include "../../CommonUtil/RefObject.h"
#include "../../CommLayer/ProtocolCodec/Global_Def.h"
#include "../../CommonUtil/NotifyBase.h"
#include "../../CommonUtil/Buffer.h"
#include "../../CommonUtil/LogPolicy.h"

class CRequestManager;

interface IRequestToken
{
	virtual void	Unlock() = 0;
};

class CRequestToken :  public IRequestToken
{
	friend class CRequestManager;
protected:
	CRequestToken():m_bAvailable(true){}
	~CRequestToken(){}
	void Lock()
	{
		TRACE_FILE_LINE_INFO();
		if(m_bAvailable)
			m_bAvailable = false;
	}
	bool IsLock(){return m_bAvailable == false;}
public:
	virtual void		Unlock()
	{		
		TRACE_FILE_LINE_INFO();
		if(m_bAvailable == false)
			m_bAvailable = true;
	}
private:
	bool		m_bAvailable;
};

class CRequest : public QSYNC_LIB::CTNotifyBase<CRefObject>
{
public:
	CRequest():m_ptBufferReceiver(new CBuffer), m_eReqState(STATE_WAITING),m_pIRequestToken(NULL) {;}
	virtual ~CRequest() {}

	virtual HRESULT ExecuteRequest() = 0;
	virtual enumProtocolConnection Open() = 0;
	virtual enumProtocolConnection Close() = 0;
	virtual void Abort(){;};

	virtual const enumServiceType GetServiceType() const = 0;
	virtual const enumProtocolState GetProtocolType() const = 0;

	void SetReqState(enumRequestState eReqState) { m_eReqState = eReqState; }
	bool IsRequstCompleted() { return ((m_eReqState == STATE_COMPLETED_SUCCEEDED) || (m_eReqState == STATE_COMPLETED_FAILED)) ? TRUE : FALSE; };
	bool IsRequstCompletedFailed() { return (m_eReqState == STATE_COMPLETED_FAILED) ? TRUE : FALSE; };
	CBufferPtr& GetReceiver(){return m_ptBufferReceiver;}
	virtual int GetReceiverResult(const CBufferPtr& ptReceiver){/*not implement*/ return -1;}

	virtual void SetIRequestToken(IRequestToken *pIRequestToken){m_pIRequestToken = pIRequestToken;}
	virtual void FreeToken()
	{
		TRACE_FILE_LINE_INFO();
		if(m_pIRequestToken)
			m_pIRequestToken->Unlock();
	}

protected:	
	CBufferPtr		m_ptBufferReceiver;
    enumRequestState	m_eReqState;
private:
	IRequestToken*			m_pIRequestToken;
};

typedef CRefObjectPtr<CRequest> CRequestPtr;

#endif // #ifndef __REQUEST_H__
