// TLEvent.h: interface for the CTLEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TLEVENT_H__BCEC6D1B_E0A4_4EA1_829B_155ED5FFFC95__INCLUDED_)
#define AFX_TLEVENT_H__BCEC6D1B_E0A4_4EA1_829B_155ED5FFFC95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RefObject.h"
#include "Buffer.h"

class CTLEvent : public CRefObject
{
public:
    enum EventType_t
    {
        etDataArrival,
		etWriteCompleted,
        etConnected,
        etDisconnected
    };

    class CTLEventConsumer
    {
    public:
        virtual void OnTLEventDataArrival( CBufferPtr spBuffer ) = 0;
        virtual void OnTLEventConnected() = 0;
        virtual void OnTLEventDisconnected() = 0;
		virtual void OnTLEventWriteCompleted() = 0;
    };

public:
    static CRefObjectPtr<CTLEvent> CreateDataArrival( CBufferPtr spBuffer );
    static CRefObjectPtr<CTLEvent> CreateConnected();
    static CRefObjectPtr<CTLEvent> CreateDisconnected();
	static CRefObjectPtr<CTLEvent> CreateWriteCompleted();

    void FeedToConsumer( CTLEventConsumer* pConsumer );

private:
	CTLEvent( EventType_t eEventType );
public:
	virtual ~CTLEvent();

private:
    EventType_t m_eEventType;

    CBufferPtr m_spBuffer;
};

typedef CRefObjectPtr<CTLEvent> CTLEventPtr;

#endif // !defined(AFX_TLEVENT_H__BCEC6D1B_E0A4_4EA1_829B_155ED5FFFC95__INCLUDED_)
