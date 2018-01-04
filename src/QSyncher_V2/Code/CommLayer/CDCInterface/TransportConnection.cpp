// TransportConnection.cpp: implementation of the CTransportConnection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TransportConnection.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTransportConnection::CTransportConnection()
{
}

CTransportConnection::~CTransportConnection()
{
}

LPCTSTR CTransportConnection::WaitResult2t( WaitResult_t e )
{
    switch(e)
    {
    case wrClient   : return _T("wrClient");
    case wrRead     : return _T("wrRead");
    case wrWrite    : return _T("wrWrite");
    case wrClose    : return _T("wrClose");
    default         : return _T("*** undefined ***");
    }
}
