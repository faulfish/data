/*****************************************************************************/
/** @file:		ProtocolManager.h
  This is the header file that contains the protocol manager class.

  $Author:   Alan Tu  $
  $Version:		1.0 $
  $Date:   Dec 27 2007 09:46:52  $
  */
/*****************************************************************************/
#ifndef __PROTOCOLMANAGER_H__
#define __PROTOCOLMANAGER_H__

#ifndef _CDCINTERFACEDLL
	#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllimport)
	CDCINTERFACE_CLASS_DLLEXT void CDCExist();
#else
	#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllexport)

#include "TransportLayer.h"
#include "AtCodec.h"
#include "ObexCodec.h"
#include "..\..\Components\ConfigureTool\IConfigure.h"

/*! @class		CProtocolManager
 *  @brief		manage the protocol package
 *  @author		Eddy Guo
 *  @date		2007/01/30
 */
class CProtocolManager :
	public CTransportLayer::CTransportLayerCallback,
    public CTLEvent::CTLEventConsumer
{
public:
	CProtocolManager();
	~CProtocolManager();

	class CProtocolManagerCallback
	{
	public:
		virtual void OnDataArrival(CBufferPtr spBuffer) = 0;
		virtual void OnWriteCompleted() = 0;
	};
	
	virtual void OnTLEventConnected();
	virtual void OnTLEventDisconnected();
	virtual void OnTLEventWriteCompleted();
	virtual void OnTLEventDataArrival(CBufferPtr spBuffer);	

	void TransportLayerEvent(CTLEventPtr spTLEvent);

	CTransportLayer* GetTransportLayerPtr();

	HRESULT SetPort(CString cstrPort,int nBaudRate);
	HRESULT ResetPort();

	HRESULT ExecuteProtocolRequest(CBufferPtr spBuffer, CProtocolManagerCallback* ptrCallBack);
	
	void    UnRegisterCallBack(CProtocolManagerCallback* ptrCallBack);

	BOOL IsConnected(){return m_bIsConnected;};
	void Shutdown();
private:
	void TraceBinary(CBufferPtr spBuffer);
private:
	CCriticalSection	m_CritSection;
	CEvent m_hConnectedHandle;
	CEvent m_hDisconnectedHandle;
	CTransportLayer m_TransportLayer;
	DWORD m_dwNoOfExpectedBytes;
    CProtocolManagerCallback* m_ptrCallBack;

	BOOL   m_bIsConnected;
	QSYNC_LIB::IConfigure*		m_IConfigure;
	QSYNC_LIB::CCfgInt			m_nEnableLog;
};

extern CProtocolManager g_ProtocolManager;

#endif
#endif // #ifndef __PROTOCOLMANAGER_H__
