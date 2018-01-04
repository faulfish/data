// TLRequest.h: interface for the CTLRequest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TLREQUEST_H__8CB207B3_F7AD_4071_884A_1DB3D671EFE1__INCLUDED_)
#define AFX_TLREQUEST_H__8CB207B3_F7AD_4071_884A_1DB3D671EFE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RefObject.h"
#include "Buffer.h"

class CTLRequest : public CRefObject
{
public:
    class CTLRequestConsumer
    {
    public:
        virtual void OnTLRequestOpen( LPCTSTR szPort ,int nBoundRate) = 0;
        virtual void OnTLRequestClose() = 0;
        virtual void OnTLRequestRead( DWORD dwNoOfExpectedBytes ) = 0;
        virtual void OnTLRequestWrite( CBufferPtr spBuffer ) = 0;
        virtual void OnTLRequestOptimizeForPhone( LPCTSTR szPhoneType, LPCTSTR szManufacturer, LPCTSTR szIMEI, LPCTSTR szSoftwareVersion ) = 0;
    };

public:
    void FeedToConsumer( CTLRequestConsumer* pConsumer );

public:
    static CRefObjectPtr<CTLRequest> CreateOpen( LPCTSTR szPort,int nBoundRate );
    static CRefObjectPtr<CTLRequest> CreateClose();
    static CRefObjectPtr<CTLRequest> CreateRead( DWORD dwNoOfExpectedBytes );
    static CRefObjectPtr<CTLRequest> CreateWrite( CBufferPtr spBuffer );
    static CRefObjectPtr<CTLRequest> CreateOptimizeForPhone( LPCTSTR szPhoneType, LPCTSTR szManufacturer, LPCTSTR szIMEI, LPCTSTR szSoftwareVersion );

private:
    enum RequestType_t { rtOpen, rtClose, rtRead, rtWrite, rtOptimizeForPhone };
	CTLRequest( RequestType_t eType );
public:
	virtual ~CTLRequest();

private:
    RequestType_t m_eType;
    CString m_strPort;
	int m_nBoundRate;
    DWORD m_dwNoOfExpectedBytes;
    CBufferPtr m_spBuffer;
    CString m_strPhoneType;
    CString m_strManufacturer;
    CString m_strIMEI;
    CString m_strSoftwareVersion;
};

typedef CRefObjectPtr<CTLRequest> CTLRequestPtr;

#endif // !defined(AFX_TLREQUEST_H__8CB207B3_F7AD_4071_884A_1DB3D671EFE1__INCLUDED_)
