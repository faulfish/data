// TLRequest.cpp: implementation of the CTLRequest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TLRequest.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTLRequest::CTLRequest( RequestType_t eType )
{
    m_eType = eType;
}

CTLRequest::~CTLRequest()
{
}

CTLRequestPtr CTLRequest::CreateOpen( LPCTSTR szPort ,int nBoundRate)
{
    CTLRequestPtr spTLRequest = new CTLRequest(rtOpen);
    spTLRequest->m_strPort = szPort;
	spTLRequest->m_nBoundRate = nBoundRate;
    return spTLRequest;
}

CTLRequestPtr CTLRequest::CreateClose()
{
    CTLRequestPtr spTLRequest = new CTLRequest(rtClose);
    return spTLRequest;
}

CTLRequestPtr CTLRequest::CreateRead( DWORD dwNoOfExpectedBytes )
{
    CTLRequestPtr spTLRequest = new CTLRequest(rtRead);
    spTLRequest->m_dwNoOfExpectedBytes = dwNoOfExpectedBytes;
    return spTLRequest;
}

CTLRequestPtr CTLRequest::CreateWrite( CBufferPtr spBuffer )
{
    CTLRequestPtr spTLRequest = new CTLRequest(rtWrite);
    spTLRequest->m_spBuffer = spBuffer;
    return spTLRequest;
}

CTLRequestPtr CTLRequest::CreateOptimizeForPhone( LPCTSTR szPhoneType, LPCTSTR szManufacturer, LPCTSTR szIMEI, LPCTSTR szSoftwareVersion )
{
    CTLRequestPtr spTLRequest = new CTLRequest(rtOptimizeForPhone);
    spTLRequest->m_strPhoneType = szPhoneType;
    spTLRequest->m_strManufacturer = szManufacturer;
    spTLRequest->m_strIMEI = szIMEI;
    spTLRequest->m_strSoftwareVersion = szSoftwareVersion;
    return spTLRequest;
}

void CTLRequest::FeedToConsumer( CTLRequestConsumer* pConsumer )
{
    switch( m_eType )
    {
    case rtOpen:
        pConsumer->OnTLRequestOpen( m_strPort ,m_nBoundRate);
        break;
    case rtClose:
        pConsumer->OnTLRequestClose();
        break;
    case rtRead:
        pConsumer->OnTLRequestRead( m_dwNoOfExpectedBytes );
        break;
    case rtWrite:
        pConsumer->OnTLRequestWrite( m_spBuffer );
        break;
    case rtOptimizeForPhone:
        pConsumer->OnTLRequestOptimizeForPhone( m_strPhoneType, m_strManufacturer, m_strIMEI, m_strSoftwareVersion );
        break;
    }
}