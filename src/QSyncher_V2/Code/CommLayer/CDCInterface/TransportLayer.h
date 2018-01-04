// TransportLayer.h: interface for the CTransportLayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSPORTLAYER_H__CF7514A0_6EB8_41B9_9B69_00692117B856__INCLUDED_)
#define AFX_TRANSPORTLAYER_H__CF7514A0_6EB8_41B9_9B69_00692117B856__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SerialPort.h"
#include "IrdaSocket.h"
#include "TLEvent.h"
#include "TLRequest.h"
#include "TransportConnection.h"

class CTransportLayer :
    public CTLRequest::CTLRequestConsumer
{
public:
    class CTransportLayerCallback
    {
    public:
        virtual void TransportLayerEvent( CTLEventPtr pTLEvent ) = 0;
    };

public:
	CTransportLayer();
	virtual ~CTransportLayer();

    void Startup();
    void Shutdown();

public:
    void Open( LPCTSTR szPort ,int nBaudRate);
    void Close();
    void OptimizeForPhone( LPCTSTR szPhoneType, LPCTSTR szManufacturer, LPCTSTR szIMEI, LPCTSTR szSoftwareVersion );
    void Write( CBufferPtr spBuffer );
    void Read( DWORD dwNoOfExpectedBytes );
    void Advise( CTransportLayerCallback* pCallback );

    void QueueTLRequest( CTLRequestPtr spTLRequest );
    CTLRequestPtr GetNextTLRequest();
    void ClearTLRequestQueue();

// From CTLRequest
public:
    virtual void OnTLRequestOpen( LPCTSTR szPort ,int nBoundRate);
    virtual void OnTLRequestClose();
    virtual void OnTLRequestRead( DWORD dwNoOfExpectedBytes );
    virtual void OnTLRequestWrite( CBufferPtr spBuffer );
    virtual void OnTLRequestOptimizeForPhone( LPCTSTR szPhoneType, LPCTSTR szManufacturer, LPCTSTR szIMEI, LPCTSTR szSoftwareVersion );

private:
    void NotifyConnected();
    void NotifyDisconnected();
    void NotifyDataArrival( CBufferPtr spBuffer );

    static DWORD WINAPI StaticThreadProc( LPVOID lp );;
    void ThreadProc();
    bool WaitProcCOMM();
    bool WaitProcIRDA();
    bool WaitProcDefault();

    CTransportConnection* GetActiveConnection();
    bool IsWriteActive() const;
    void SetWriteActive( bool bActive );

    CTransportConnection::WaitResult_t WaitResult( HANDLE hEventClient ); // Default wait function if no TL connected

    void SetShutdown( bool bShutdown );
    bool GetShutdown() const;

private:
    enum State { tlsDisconnected, tlsConnectedCOMM, tlsCOMMWrite, tlsConnectedIRDA, tlsIRDAWrite };
    static LPCTSTR State2t( State e );
    
    State m_eState;
    CSerialPort m_SerialPort;
    CIrdaSocket m_IrdaSocket;
    CTransportLayerCallback* m_pCallback;

    HANDLE m_hThreadHandle;
    CEvent m_eventShutdown;
    CEvent m_eventRequest;
    CEvent m_eventCancel;
    CEvent m_eventRead;
    CEvent m_eventWrite;
    CEvent m_eventSocket;

    bool m_bShutdown;

    mutable CCriticalSection m_lockData;
    std::list<CTLRequestPtr> m_listTLRequest;
};

#endif // !defined(AFX_TRANSPORTLAYER_H__CF7514A0_6EB8_41B9_9B69_00692117B856__INCLUDED_)
