// TransportLayer.cpp: implementation of the CTransportLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TransportLayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTransportLayer::CTransportLayer() : m_eventRead(FALSE,TRUE), m_eventWrite(FALSE,TRUE), m_eventSocket(FALSE,TRUE)
{
    m_eState = tlsDisconnected;
    m_pCallback = NULL;
    m_hThreadHandle = NULL;
    m_bShutdown = false;
}

CTransportLayer::~CTransportLayer()
{
}

void CTransportLayer::Advise( CTransportLayerCallback* pCallback )
{
    m_pCallback = pCallback;
}

void CTransportLayer::Startup()
{
    if( !m_hThreadHandle )
    {
        DWORD dwThreadId = 0;
        m_bShutdown = false;
        m_hThreadHandle = ::CreateThread( NULL, 0, StaticThreadProc, (LPVOID)this, 0, &dwThreadId );
    }
}

void CTransportLayer::Shutdown()
{
    if( m_hThreadHandle )
    {
        m_bShutdown = true;
        m_eventRequest.SetEvent();
        WaitForSingleObject( m_hThreadHandle, INFINITE );
        ::CloseHandle( m_hThreadHandle );
        m_hThreadHandle = NULL;
    }
}

DWORD WINAPI CTransportLayer::StaticThreadProc( LPVOID lp )
{
    CTransportLayer* pThis = (CTransportLayer*)lp;
    pThis->ThreadProc();
    return 0;
}

void CTransportLayer::ThreadProc()
{
    LPCTSTR pcf = _T("CTransportLayer::ThreadProc()");
//    Traceit( TRACEIT_ENTER, TG_CTransportLayer, _T("%s"), pcf );

    while(!GetShutdown())
    {
        HRESULT hr = S_OK;
        CBufferPtr spBuffer;
        CTransportConnection::WaitResult_t eWaitResult = CTransportConnection::wrClient;
        CTransportConnection* pConnection = GetActiveConnection();

        if( pConnection )
            eWaitResult = pConnection->WaitResult( (HANDLE)m_eventRequest );
        else
            eWaitResult = WaitResult( (HANDLE)m_eventRequest );

        switch( eWaitResult )
        {
        case CTransportConnection::wrClient:
            // Just wake up
            break;

        case CTransportConnection::wrRead:
            if( !pConnection )
                break;
            hr = pConnection->Read( spBuffer );
            if( SUCCEEDED(hr) )
                NotifyDataArrival( spBuffer );
            else
                NotifyDisconnected();
            break;

        case CTransportConnection::wrWrite:
            SetWriteActive( false );
			if( m_pCallback )
			    m_pCallback->TransportLayerEvent( CTLEvent::CreateWriteCompleted() );
            break;

        case CTransportConnection::wrClose:
            if( pConnection )
                pConnection->Close();
            m_eState = tlsDisconnected;
            NotifyDisconnected();
            break;
        }

        while( !IsWriteActive() )
        {
            // Check for requests
            CTLRequestPtr spTLRequest = GetNextTLRequest();
            if( spTLRequest )
            {
                spTLRequest->FeedToConsumer(this);
            }
            else
                break;
        }
    }

//    Traceit( TRACEIT_LEAVE, TG_CTransportLayer, _T("%s"), pcf );
}

CTransportConnection::WaitResult_t CTransportLayer::WaitResult( HANDLE hEventClient )
{
	::WaitForSingleObject( hEventClient, INFINITE );
    return CTransportConnection::wrClient;
}

// **************************************************************************
// State mappings
// **************************************************************************

bool CTransportLayer::IsWriteActive() const
{
    switch(m_eState)
    {
    case tlsCOMMWrite:
    case tlsIRDAWrite:
        return true;
    default:
        return false;
    }
}

void CTransportLayer::SetWriteActive( bool bActive )
{
    if( bActive )
    {
        switch(m_eState)
        {
        case tlsConnectedCOMM:
            m_eState = tlsCOMMWrite;
            break;
        case tlsConnectedIRDA:
            m_eState = tlsIRDAWrite;
            break;
        }
    }
    else
    {
        switch( m_eState )
        {
        case tlsCOMMWrite:
            m_eState = tlsConnectedCOMM;
            break;
        case tlsIRDAWrite:
            m_eState = tlsConnectedIRDA;
            break;
        }
    }
}

CTransportConnection* CTransportLayer::GetActiveConnection()
{
    switch( m_eState )
    {
    case tlsConnectedCOMM:
    case tlsCOMMWrite:
        return &m_SerialPort;
    case tlsConnectedIRDA:
    case tlsIRDAWrite:
        return &m_IrdaSocket;
    default:
        return NULL;
    }
}

// **************************************************************************
// Commands
// **************************************************************************

void CTransportLayer::Open( LPCTSTR szPort , int nBaudRate)
{
//    Traceit( TRACEIT_RECVREQ, TG_CTransportLayer, _T("CTransportLayer::Open( szPort=<%s> ) - Add request to queue"), szPort );
    QueueTLRequest( CTLRequest::CreateOpen(szPort,nBaudRate) );
}

void CTransportLayer::Close()
{
//    Traceit( TRACEIT_RECVREQ, TG_CTransportLayer, _T("CTransportLayer::Close() - Add request to queue") );
    QueueTLRequest( CTLRequest::CreateClose() );
}

void CTransportLayer::Read( DWORD dwNoOfExpectedBytes )
{
//    Traceit( TRACEIT_RECVREQ, TG_CTransportLayer, _T("CTransportLayer::Read( dwNoOfExpectedBytes=%ld ) - Add request to queue"), dwNoOfExpectedBytes );
    QueueTLRequest( CTLRequest::CreateRead(dwNoOfExpectedBytes) );
}

void CTransportLayer::Write( CBufferPtr spBuffer )
{
//    Traceit( TRACEIT_RECVREQ, TG_CTransportLayer, _T("CTransportLayer::Write( spBuffer=%s ) - Add request to queue"), spbuffer2hex(spBuffer) );
    QueueTLRequest( CTLRequest::CreateWrite(spBuffer) );
}

void CTransportLayer::OptimizeForPhone( LPCTSTR szPhoneType, LPCTSTR szManufacturer, LPCTSTR szIMEI, LPCTSTR szSoftwareVersion )
{
//    Traceit( TRACEIT_RECVREQ, TG_CTransportLayer, _T("CTransportLayer::OptimizeForPhone( szPhoneType=<%s>, szManufacturer=<%s>, szIMEI=<%s>, szSoftwareVersion=<%s> ) - Add request to queue"),
//        szPhoneType, szManufacturer, szIMEI, szSoftwareVersion );
    QueueTLRequest( CTLRequest::CreateOptimizeForPhone(szPhoneType,szManufacturer,szIMEI,szSoftwareVersion) );
}

void CTransportLayer::QueueTLRequest( CTLRequestPtr spTLRequest )
{
    CSingleLock sl( &m_lockData, TRUE );
    m_listTLRequest.push_back( spTLRequest );
    m_eventRequest.SetEvent();
}

CTLRequestPtr CTransportLayer::GetNextTLRequest()
{
    CSingleLock sl( &m_lockData, TRUE );
    CTLRequestPtr spTLRequest = NULL;
    if( !m_listTLRequest.empty() )
    {
        spTLRequest = m_listTLRequest.front();
        m_listTLRequest.pop_front();
    }
    return spTLRequest;
}

void CTransportLayer::ClearTLRequestQueue()
{
    CSingleLock sl( &m_lockData, TRUE );
    m_listTLRequest.clear();
}

void CTransportLayer::SetShutdown( bool bShutdown )
{
    CSingleLock sl( &m_lockData, TRUE );
    m_bShutdown = bShutdown;
}

bool CTransportLayer::GetShutdown() const
{
    CSingleLock sl( &m_lockData, TRUE );
    return m_bShutdown;
}

void CTransportLayer::OnTLRequestOpen( LPCTSTR szPort ,int nBoundRate)
{
    LPCTSTR pcf = _T("CTransportLayer::OnTLRequestOpen()");
//    Traceit( TRACEIT_ENTER, TG_CTransportLayer, _T("%s: szPort=<%s>"), pcf, szPort );
    HRESULT hr = S_OK;

    if( m_eState == tlsDisconnected )
    {
        if( CString(szPort) == CString(_T("IRDA")) )
        {
            hr = m_IrdaSocket.Open( szPort ,nBoundRate);
            if( SUCCEEDED(hr) )
            {
                m_eState = tlsConnectedIRDA;
            }
        }
        else
        {
            hr = m_SerialPort.Open( szPort ,nBoundRate);
            if( SUCCEEDED(hr) )
            {
                m_eState = tlsConnectedCOMM;
            }
        }
    }
    else
    {
//        Traceit( TRACEIT_INFO, TG_CTransportLayer, _T("%s: Wrong state: %s"), pcf, State2t(m_eState) );
        hr = E_FAIL;
    }

    if( SUCCEEDED(hr) )
        NotifyConnected();
    else
        NotifyDisconnected();

//    Traceit( TRACEIT_LEAVE, TG_CTransportLayer, _T("%s"), pcf );
}


void CTransportLayer::OnTLRequestClose()
{
    LPCTSTR pcf = _T("CTransportLayer::OnTLRequestClose()");
//    Traceit( TRACEIT_ENTER, TG_CTransportLayer, _T("%s"), pcf );

    HRESULT hr = S_OK;
    CTransportConnection* pConnection = GetActiveConnection();

    if( pConnection )
    {
        pConnection->Close();
        NotifyDisconnected();
    }

    m_eState = tlsDisconnected;

//    Traceit( TRACEIT_LEAVE, TG_CTransportLayer, _T("%s"), pcf );
}

void CTransportLayer::OnTLRequestOptimizeForPhone( LPCTSTR szPhoneType, LPCTSTR szManufacturer, LPCTSTR szIMEI, LPCTSTR szSoftwareVersion )
{
    LPCTSTR pcf = _T("CTransportLayer::OnTLRequestOptimizeForPhone()");
//    Traceit( TRACEIT_ENTER, TG_CTransportLayer, _T("%s"), pcf );

    HRESULT hr = S_OK;
    CTransportConnection* pConnection = GetActiveConnection();

    if( pConnection )
    {
        hr = pConnection->OptimizeForPhone( szPhoneType, szManufacturer, szIMEI, szSoftwareVersion );
    }
//    else
//        hr = MPM_E_NOCONNECTION;

//    Traceit( TRACEIT_LEAVE, TG_CTransportLayer, _T("%s"), pcf );
}

void CTransportLayer::OnTLRequestRead( DWORD dwNoOfExpectedBytes )
{
    LPCTSTR pcf = _T("CTransportLayer::OnTLRequestRead()");
//    Traceit( TRACEIT_ENTER, TG_CTransportLayer, _T("%s"), pcf );

    CTransportConnection* pConnection = GetActiveConnection();

    if( pConnection )
    {
        pConnection->StartRead( dwNoOfExpectedBytes );
    }

//    Traceit( TRACEIT_LEAVE, TG_CTransportLayer, _T("%s"), pcf );
}

void CTransportLayer::OnTLRequestWrite( CBufferPtr spBuffer )
{
    LPCTSTR pcf = _T("CTransportLayer::OnTLRequestWrite()");
//    Traceit( TRACEIT_ENTER, TG_CTransportLayer, _T("%s"), pcf );

    HRESULT hr = E_FAIL;
    CTransportConnection* pConnection = GetActiveConnection();

    if( pConnection )
    {
        SetWriteActive( true );
        hr = pConnection->Write( spBuffer );
    }
//    else
//        hr = MPM_E_NOCONNECTION;

//    Traceit( TRACEIT_LEAVE, TG_CTransportLayer, _T("%s"), pcf );
}

void CTransportLayer::NotifyDataArrival( CBufferPtr spBuffer )
{
    if( m_pCallback )
        m_pCallback->TransportLayerEvent( CTLEvent::CreateDataArrival(spBuffer) );
}


void CTransportLayer::NotifyDisconnected()
{
    if( m_pCallback )
        m_pCallback->TransportLayerEvent( CTLEvent::CreateDisconnected() );
}

void CTransportLayer::NotifyConnected()
{
    if( m_pCallback )
        m_pCallback->TransportLayerEvent( CTLEvent::CreateConnected() );
}

LPCTSTR CTransportLayer::State2t( State e )
{
    switch(e)
    {
    case tlsDisconnected        : return _T("");
    case tlsConnectedCOMM       : return _T("");
    case tlsCOMMWrite           : return _T("");
    case tlsConnectedIRDA       : return _T("");
    case tlsIRDAWrite           : return _T("");
    default                     : return _T("*** unknown ***");
    }
}