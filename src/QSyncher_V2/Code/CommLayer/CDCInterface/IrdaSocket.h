// IrdaSocket.h: interface for the CIrdaSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IRDASOCKET_H__011BBB63_15A4_4673_9745_BCE68BDA52A0__INCLUDED_)
#define AFX_IRDASOCKET_H__011BBB63_15A4_4673_9745_BCE68BDA52A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Buffer.h"
#include "TransportConnection.h"

class CIrdaSocket : public CTransportConnection
{
public:
	CIrdaSocket();
	virtual ~CIrdaSocket();

public:
    virtual HRESULT Open( LPCTSTR szPortName ,int nBoundRate = -1);
    virtual void Close();
    virtual HRESULT OptimizeForPhone( LPCTSTR szPhoneType, LPCTSTR szManufacturer, LPCTSTR szIMEI, LPCTSTR szSoftwareVersion );
    virtual HRESULT Write( CBufferPtr spBuffer );
    virtual void StartRead( DWORD dwNoOfExpectedBytes ) {}
    virtual HRESULT Read( CBufferPtr& spBuffer );
    virtual WaitResult_t WaitResult( HANDLE hEventClient );

private:
    int IrCOMMConnect(PSOCKADDR_IRDA pPeerIRAddr);
    HRESULT WriteRest( bool& bReady );

private:
    SOCKET m_socket;
    CEvent m_eventSocket;

    DWORD m_dwNoOfBytesWritten;
//    long m_lNetworkevent; // Remember simulated network events

    CBufferPtr m_spWriteBuffer;
    bool m_bWriteNotifyPending;
};

#endif // !defined(AFX_IRDASOCKET_H__011BBB63_15A4_4673_9745_BCE68BDA52A0__INCLUDED_)
