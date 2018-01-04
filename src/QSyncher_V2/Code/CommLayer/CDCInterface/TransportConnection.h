// TransportConnection.h: interface for the CTransportConnection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSPORTCONNECTION_H__304599E1_CB2C_4FDE_A54A_E66A994F8735__INCLUDED_)
#define AFX_TRANSPORTCONNECTION_H__304599E1_CB2C_4FDE_A54A_E66A994F8735__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Buffer.h"

class CTransportConnection  
{
public:
    enum WaitResult_t
    {
        wrClient,
        wrRead,
        wrWrite,
        wrClose
    };

public:
	CTransportConnection();
	virtual ~CTransportConnection();

    virtual HRESULT Open( LPCTSTR szPortName ,int nBoundRate) = 0;
    virtual void Close() = 0;
    virtual HRESULT OptimizeForPhone( LPCTSTR szPhoneType, LPCTSTR szManufacturer, LPCTSTR szIMEI, LPCTSTR szSoftwareVersion ) = 0;
    virtual HRESULT Write( CBufferPtr spBuffer ) = 0;
    virtual void StartRead( DWORD dwNoOfExpectedBytes ) = 0;
    virtual HRESULT Read( CBufferPtr& spBuffer ) = 0;
    virtual WaitResult_t WaitResult( HANDLE hEventClient ) = 0;

public:
    static LPCTSTR WaitResult2t( WaitResult_t e );
};

#endif // !defined(AFX_TRANSPORTCONNECTION_H__304599E1_CB2C_4FDE_A54A_E66A994F8735__INCLUDED_)
