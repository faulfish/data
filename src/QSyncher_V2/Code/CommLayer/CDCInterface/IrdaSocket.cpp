// IrdaSocket.cpp: implementation of the CIrdaSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IrdaSocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIrdaSocket::CIrdaSocket() : m_eventSocket(FALSE,TRUE)
{
    m_socket = INVALID_SOCKET;
    m_dwNoOfBytesWritten = 0;
    m_bWriteNotifyPending = false;
}

CIrdaSocket::~CIrdaSocket()
{
}

HRESULT CIrdaSocket::Open( LPCTSTR szPortName ,int nBoundRate)
{
    LPCTSTR pcf = _T("CIrdaSocket::Open()");
//	Traceit( TRACEIT_ENTER, TG_CIrdaSocket, _T("%s: PortName:%s"), pcf, szPortName );

    HRESULT hr = S_OK;
    WSADATA WSAData;
    int nResult = 0;

    if( m_socket != INVALID_SOCKET )
        hr = E_FAIL;
    
    if( SUCCEEDED(hr) )
    {
        // Initialize
        nResult = ::WSAStartup( MAKEWORD(1,1), &WSAData);

        if( nResult != 0 )
            hr = E_FAIL;
    }

    if( SUCCEEDED(hr) )
    {
        // Create socket
        m_socket = ::socket(AF_IRDA, SOCK_STREAM, 0);
        if (m_socket == INVALID_SOCKET)
        {
            WSACleanup();
            hr = E_FAIL;
        }
    }

    DEVICELIST DeviceList;
    if( SUCCEEDED(hr) )
    {
        // Discover an IRDA device
        int nDeviceListSize = sizeof(DeviceList);
        memset( &DeviceList, 0, sizeof(DeviceList) );

        // Sock is not in connected state
        nResult = getsockopt( m_socket, SOL_IRLMP, IRLMP_ENUMDEVICES, (char *) &DeviceList, &nDeviceListSize );
        if( nResult != 0 || DeviceList.numDevice == 0 )
        {
            ::closesocket(m_socket);
            m_socket = INVALID_SOCKET;
            WSACleanup();
            hr = E_FAIL;
        }
    }

    if( SUCCEEDED(hr) )
    {
        // Connect socket
        SOCKADDR_IRDA address = {AF_IRDA,0,0,0,0,0};

        memcpy( &address.irdaDeviceID[0], &DeviceList.Device[0].irdaDeviceID[0], 4);

        memcpy( &address.irdaServiceName[0], "IrDA:IrCOMM", 12 );

    //  nResult = ::connect( m_socket, (struct sockaddr *)&address, sizeof(SOCKADDR_IRDA) );

        nResult = IrCOMMConnect( &address );

        if( nResult == 0 )
        {
            unsigned long l = 0;
            nResult = ::ioctlsocket( m_socket, FIONBIO, &l ); // Set to non-blocking mode
        }
        else
        {
            ::closesocket(m_socket);
            m_socket = INVALID_SOCKET;
            ::WSACleanup();
            hr = E_FAIL;
        }
    }

    if( SUCCEEDED(hr) )
    {
        nResult = ::WSAEventSelect( m_socket, (HANDLE) m_eventSocket, FD_READ | FD_WRITE | FD_CONNECT | FD_CLOSE );
    }

//	Traceit( TRACEIT_LEAVE, TG_CIrdaSocket, _T("%s: hr=0x%08lx"), pcf, hr );
    return hr;
}

int CIrdaSocket::IrCOMMConnect(PSOCKADDR_IRDA pPeerIRAddr)
{
    BYTE          IASQueryBuff[sizeof(IAS_QUERY) - 3 + 32];
    int           IASQueryLen = sizeof(IASQueryBuff);
    PIAS_QUERY    pIASQuery   = (PIAS_QUERY) &IASQueryBuff;

    BOOL          found = FALSE;
    UCHAR         *pPI, *pPL, *pPV;
    int           enable9WireMode  = 1;

    memcpy(&pIASQuery->irdaDeviceID[0], &pPeerIRAddr->irdaDeviceID[0], 4);

    memcpy(&pIASQuery->irdaClassName[0], "IrDA:IrCOMM", 12);
    memcpy(&pIASQuery->irdaAttribName[0], "Parameters", 11);

    if( ::getsockopt( m_socket, SOL_IRLMP, IRLMP_IAS_QUERY,  (char*)pIASQuery, &IASQueryLen) == SOCKET_ERROR)
	{
		int nError = WSAGetLastError();
		return nError;
	}

    if (pIASQuery->irdaAttribType != IAS_ATTRIB_OCTETSEQ)
	{
        return WSAECONNREFUSED;
	}

    if (pIASQuery->irdaAttribute.irdaAttribOctetSeq.Len < 3)
	{
        return WSAECONNREFUSED;
	}

    pPI = pIASQuery->irdaAttribute.irdaAttribOctetSeq.OctetSeq;
    pPL = pPI + 1;
    pPV = pPI + 2;

    while (TRUE)
    {
        if ((*pPI == 0) && ((*pPV & 0x04) != 0))
        {
            found = TRUE;
            break;
        }

        if (pPL + *pPL >= pIASQuery->irdaAttribute.irdaAttribOctetSeq.OctetSeq + 
                          pIASQuery->irdaAttribute.irdaAttribOctetSeq.Len)
            break;

         pPI = pPL + *pPL;
         pPL = pPI + 1;
         pPV = pPI + 2;
    }

    if( !found )
	{
        return WSAECONNREFUSED;
	}

    if( ::setsockopt( m_socket, SOL_IRLMP, IRLMP_9WIRE_MODE, (const char*)&enable9WireMode, sizeof(int)) == SOCKET_ERROR)
	{
		int nError = WSAGetLastError();
		return nError;
	}

    if( ::connect( m_socket, (struct sockaddr*)pPeerIRAddr, sizeof(SOCKADDR_IRDA)) == SOCKET_ERROR)
	{
		int nError = ::WSAGetLastError();
		return nError;
	}
	
    return 0;
}

void CIrdaSocket::Close()
{
    LPCTSTR pcf = _T("CIrdaSocket::Close()");
//	Traceit( TRACEIT_ENTER, TG_CIrdaSocket, _T("%s"), pcf );

    if( m_socket != INVALID_SOCKET )
    {
        ::WSAEventSelect( m_socket, (HANDLE) m_eventSocket, 0 );
        ::closesocket( m_socket );
        ::WSACleanup();
    }
    m_socket = INVALID_SOCKET;

//	Traceit( TRACEIT_LEAVE, TG_CIrdaSocket, _T("%s"), pcf );
}

HRESULT CIrdaSocket::OptimizeForPhone( LPCTSTR szPhoneType, LPCTSTR szManufacturer, LPCTSTR szIMEI, LPCTSTR szSoftwareVersion )
{
    LPCTSTR pcf = _T("CIrdaSocket::OptimizeForPhone()");
//	Traceit( TRACEIT_ENTER, TG_CIrdaSocket, _T("%s: Nothing to be done"), pcf );
//	Traceit( TRACEIT_LEAVE, TG_CIrdaSocket, _T("%s: hr=0x%08lx"), pcf, S_OK );
    return S_OK;
}

CTransportConnection::WaitResult_t CIrdaSocket::WaitResult( HANDLE hEventClient )
{
    LPCTSTR pcf = _T("CIrdaSocket::WaitResult()");
//	Traceit( TRACEIT_ENTER, TG_CIrdaSocket, _T("%s"), pcf );

    WaitResult_t eWaitResult = wrClient;
    HANDLE hWaitForHandles[2];

    hWaitForHandles[0] = hEventClient;
    hWaitForHandles[1] = (HANDLE) m_eventSocket;

    bool bClient = false;
    bool bRead = false;
    bool bWrite = false;
    bool bClose = false;

    if( m_bWriteNotifyPending )
    {
        bWrite = true;
        m_bWriteNotifyPending = false;
    }

    while( !bClient && !bRead && ! bWrite && !bClose )
    {
        DWORD dwWaitObject = WaitForMultipleObjects( 2, hWaitForHandles, FALSE, INFINITE );

        switch( dwWaitObject )
        {
        case WAIT_OBJECT_0:
            bClient = true;
            break;
        case WAIT_OBJECT_0 + 1:
            {
                WSANETWORKEVENTS wsaNetworkEvents;
                int nResult = WSAEnumNetworkEvents( m_socket, (HANDLE)m_eventSocket, &wsaNetworkEvents );
                if( nResult != 0 )
                {
////        	        Traceit( TRACEIT_ERROR, TG_CIrdaSocket, _T("%s: WSAEnumNetworkEvents returned %d"), pcf, nResult );
                    break;
                }

                if( wsaNetworkEvents.lNetworkEvents & FD_WRITE )
                {
                    HRESULT hr = S_OK;
                    bool bReady = false;

                    hr = WriteRest( bReady );
                    if( SUCCEEDED(hr) && bReady )
                        bWrite = true;
                }

                if( wsaNetworkEvents.lNetworkEvents & FD_READ )
                {
                    bRead = true;
                }

                if( wsaNetworkEvents.lNetworkEvents & FD_CLOSE )
                {
                    bClose = true;
                }
            }
            break;
        }
    }

    if( bClose )
        eWaitResult = wrClose;
    else if( bClient )
        eWaitResult = wrClient;
    else if( bRead )
    {
        eWaitResult = wrRead;
        if(bWrite)
            m_bWriteNotifyPending = true;
    }
    else if( bWrite )
        eWaitResult = wrWrite;

//	Traceit( TRACEIT_LEAVE, TG_CIrdaSocket, _T("%s: eWaitResult=%s"), pcf, WaitResult2t(eWaitResult) );
    return eWaitResult;
}

HRESULT CIrdaSocket::WriteRest( bool& bReady )
{
    LPCTSTR pcf = _T("CIrdaSocket::WriteRest()");
//    Traceit( TRACEIT_ENTER, TG_CIrdaSocket, _T("%s"), pcf );

    HRESULT hr = S_OK;

    bReady = false;

    if( !m_spWriteBuffer || m_dwNoOfBytesWritten == m_spWriteBuffer->GetLength() )
    {
        bReady = true;
    }
    else
    {
        int nResult = ::send( m_socket, m_spWriteBuffer->GetData() + m_dwNoOfBytesWritten,
                        m_spWriteBuffer->GetLength() - m_dwNoOfBytesWritten, 0 );

        if( nResult == SOCKET_ERROR )
            hr = E_FAIL;
        else
        {
            m_dwNoOfBytesWritten += nResult;
            if( m_dwNoOfBytesWritten == m_spWriteBuffer->GetLength() )
                bReady = true;
            else
                bReady = false;
        }
    }

//	Traceit( TRACEIT_LEAVE, TG_CIrdaSocket, _T("%s: hr=0x%08lx, bReady=%s"), pcf, hr, bool2t(bReady) );
    return hr;
}

HRESULT CIrdaSocket::Write( CBufferPtr spBuffer )
{
    LPCTSTR pcf = _T("CIrdaSocket::Write()");
//	Traceit( TRACEIT_ENTER, TG_CIrdaSocket, _T("%s: spBuffer=%s"), pcf, spbuffer2hex(spBuffer) );

    HRESULT hr = S_OK;

    if( m_socket == INVALID_SOCKET )
        hr = E_FAIL;

    if( SUCCEEDED(hr) )
    {
        m_spWriteBuffer = spBuffer;
        m_dwNoOfBytesWritten = 0;

        if( m_spWriteBuffer && m_spWriteBuffer->GetLength() > 0 )
        {
            int nResult = ::send( m_socket, m_spWriteBuffer->GetData() + m_dwNoOfBytesWritten,
                            m_spWriteBuffer->GetLength() - m_dwNoOfBytesWritten, 0 );

            if( nResult == SOCKET_ERROR )
            {
                nResult = ::WSAGetLastError();
                if( nResult != WSAEWOULDBLOCK )
                    hr = E_FAIL;
            }
            else
            {
                m_dwNoOfBytesWritten = nResult;
            }
        }
    }

    if( SUCCEEDED(hr) )
    {
        if( !m_spWriteBuffer || m_spWriteBuffer->GetLength() == m_dwNoOfBytesWritten )
        {
            m_bWriteNotifyPending = true;
        }
    }

//	Traceit( TRACEIT_LEAVE, TG_CIrdaSocket, _T("%s"), pcf );
    return S_OK;
}

/*
bool CIrdaSocket::GetNetworkEvents( bool& bRead, bool&bWrite, bool& bClose )
{
    LPCSTR pcf = "CIrdaSocket::GetNetworkEvents()";
	Traceit( TRACEIT_ENTER, TG_CIrdaSocket, "%s", pcf );

    WSANETWORKEVENTS wsaNetworkEvents;
    bool bSuccess = false;
    int nResult = WSAEnumNetworkEvents( m_socket, m_hEventSocket, &wsaNetworkEvents );

    bRead = false;
    bWrite = false;
    bClose = false;

    if( nResult == 0 )
    {
        if( wsaNetworkEvents.lNetworkEvents & FD_READ )
            bRead = true;

        if( wsaNetworkEvents.lNetworkEvents & FD_WRITE )
            bWrite = true;

        if( wsaNetworkEvents.lNetworkEvents & FD_CLOSE )
            bClose = true;

        bSuccess = true;
    }
    else
    {
        bSuccess = false;
    }

    if( m_lNetworkevent & FD_READ )
        bRead = true;

    if( m_lNetworkevent & FD_WRITE )
        bWrite = true;

    if( m_lNetworkevent & FD_CLOSE )
        bClose = true;

    m_lNetworkevent = 0;

	Traceit( TRACEIT_LEAVE, TG_CIrdaSocket, "%s: bSuccess=%s, bRead=%s, bWrite=%s, bClose=%s",
        pcf, bool2a(bSuccess), bool2a(bRead), bool2a(bWrite), bool2a(bClose) );
    return bSuccess;
}
*/

HRESULT CIrdaSocket::Read( CBufferPtr& spBuffer )
{
    LPCTSTR pcf = _T("CIrdaSocket::Read()");
//	Traceit( TRACEIT_ENTER, TG_CIrdaSocket, _T("%s"), pcf );

    HRESULT hr = S_OK;

    spBuffer = new CBuffer;

    int nResult = ::recv( m_socket, spBuffer->GetBufferSetLength(1000), 1000, 0 );
////    Traceit( TRACEIT_INFO, TG_CIrdaSocket, _T("CIrdaSocket::Read(): recv returned %d"), nResult );

    if( nResult == 0 )
    {
        spBuffer = NULL;
        hr = E_FAIL;
    }
    else if( nResult == SOCKET_ERROR )
    {
        nResult = ::WSAGetLastError();
        if( nResult != WSAEWOULDBLOCK )
        {
            spBuffer = NULL;
            hr = E_FAIL;
        }
    }
    else
    {
        spBuffer->ReleaseBuffer(nResult);
        hr = S_OK;
    }

//	Traceit( TRACEIT_ENTER, TG_CIrdaSocket, _T("%s: spBuffer=%s"), pcf, spbuffer2hex(spBuffer) );
    return hr;
}