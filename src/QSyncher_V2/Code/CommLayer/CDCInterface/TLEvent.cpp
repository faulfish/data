// TLEvent.cpp: implementation of the CTLEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TLEvent.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTLEvent::CTLEvent( EventType_t eEventType )
{
    m_eEventType = eEventType;
}

CTLEvent::~CTLEvent()
{
}

CRefObjectPtr<CTLEvent> CTLEvent::CreateDataArrival( CBufferPtr spBuffer )
{
    CRefObjectPtr<CTLEvent> spTLEvent = new CTLEvent( etDataArrival );
    spTLEvent->m_spBuffer = spBuffer;
    return spTLEvent;
}

CRefObjectPtr<CTLEvent> CTLEvent::CreateConnected()
{
    CRefObjectPtr<CTLEvent> spTLEvent = new CTLEvent( etConnected );
    return spTLEvent;
}

CRefObjectPtr<CTLEvent> CTLEvent::CreateDisconnected()
{
    CRefObjectPtr<CTLEvent> spTLEvent = new CTLEvent( etDisconnected );
    return spTLEvent;
}

CRefObjectPtr<CTLEvent> CTLEvent::CreateWriteCompleted()
{
    CRefObjectPtr<CTLEvent> spTLEvent = new CTLEvent( etWriteCompleted );
    return spTLEvent;
}

void CTLEvent::FeedToConsumer( CTLEventConsumer* pConsumer )
{
    switch( m_eEventType )
    {
    case etDataArrival:
        pConsumer->OnTLEventDataArrival( m_spBuffer );
        break;

    case etConnected:
        pConsumer->OnTLEventConnected();
        break;

    case etDisconnected:
        pConsumer->OnTLEventDisconnected();
        break;
	case etWriteCompleted:
        pConsumer->OnTLEventWriteCompleted();
        break;
    }
}