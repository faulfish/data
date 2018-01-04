// SerialPort.cpp: Implementierung der Klasse CSerialPort.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SerialPort.h"

//------------------------------------------------------------------------------
// CSerialPort::CSerialPort
// 
//------------------------------------------------------------------------------
CSerialPort::CSerialPort() : m_eventRead(FALSE,TRUE), m_eventWrite(FALSE,TRUE), m_eventEvent(FALSE,TRUE)
{
    m_hPortId = INVALID_HANDLE_VALUE;

    // Create the overlapped event structures.  No security, explicit
    // reset request, initial event reset, no name.
    memset( &m_overlappedRead, 0, sizeof(OVERLAPPED) );
    memset( &m_overlappedWrite, 0, sizeof(OVERLAPPED) );
    memset( &m_overlappedEvent, 0, sizeof(OVERLAPPED) );

    m_overlappedRead.hEvent = (HANDLE) m_eventRead;
    m_overlappedWrite.hEvent = (HANDLE) m_eventWrite;
    m_overlappedEvent.hEvent = (HANDLE) m_eventEvent;

    m_dwEventCommMask = 0;

    m_eState = is_Closed;

    m_dwBytesToRead = 1000; // The no of bytes to be read as a block (if possible)
    m_dwNoOfBytesWritten = 0;
}

CSerialPort::~CSerialPort()
{
    if( m_hPortId != INVALID_HANDLE_VALUE )
        ::CloseHandle( m_hPortId );

}

CTransportConnection::WaitResult_t CSerialPort::WaitResult( HANDLE hEventClient )
{
    LPCTSTR pcf = _T("CSerialPort::WaitResult()");
//    Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("%s"), pcf );

    WaitResult_t eWaitResult = wrClient;
    CEventParams EventParams;
    EventParams.SetEventClient( hEventClient );

    EventValue_t eReturnEventValue = ExecuteFSM( ev_Wait, EventParams );

    switch( eReturnEventValue )
    {
    case ev_ReturnWrClient:
        eWaitResult = wrClient;
        break;
    case ev_ReturnWrRead:
        eWaitResult = wrRead;
        break;
    case ev_ReturnWrWrite:
        eWaitResult = wrWrite;
        break;
    default:
    case ev_ReturnWrClose:
        eWaitResult = wrClose;
        break;
    }

//    Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: eWaitResult=%s"), pcf, WaitResult2t(eWaitResult) );
    return eWaitResult;
}

HRESULT CSerialPort::Open( LPCTSTR szPort ,int nBoundRate)
{
    LPCTSTR pcf = _T("CSerialPort::Open()");
//    Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("%s: szPort=<%s>"), pcf, szPort );

    CEventParams EventParams;
    EventParams.SetPort( szPort ,nBoundRate);

    EventValue_t eReturnValue = ExecuteFSM( ev_Open, EventParams );
    HRESULT hr = ( eReturnValue == ev_ReturnOk ) ? S_OK : E_FAIL;

//    Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: hr=0x%08lx"), pcf, hr );
    return hr;
}

void CSerialPort::Close()
{
    LPCTSTR pcf = _T("CSerialPort::Close()");
//    Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("%s"), pcf );

    CEventParams EventParams;

    EventValue_t eReturnValue = ExecuteFSM( ev_Close, EventParams );

//    Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s"), pcf );
}

void CSerialPort::StartRead( DWORD dwNoOfExpectedBytes )
{
    LPCTSTR pcf = _T("CSerialPort::StartRead()");
//    Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("%s, dwNoOfExpectedBytes=%ld"), pcf, dwNoOfExpectedBytes );

    CEventParams EventParams;
    EventParams.SetNoOfExpectedBytes( dwNoOfExpectedBytes );

    EventValue_t eReturnValue = ExecuteFSM( ev_StartRead, EventParams );
    HRESULT hr = ( eReturnValue == ev_ReturnOk ) ? S_OK : E_FAIL;

//    Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s"), pcf );
}

HRESULT CSerialPort::Read( CBufferPtr& spBuffer )
{
    LPCTSTR pcf = _T("CSerialPort::Read()");
//    Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("%s"), pcf );

    CEventParams EventParams;

    EventValue_t eReturnValue = ExecuteFSM( ev_Read, EventParams );
    HRESULT hr = ( eReturnValue == ev_ReturnOk ) ? S_OK : E_FAIL;

    spBuffer = EventParams.GetReadBuffer();

//    Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: hr=0x%08lx, spBuffer=%s"), pcf, hr, spbuffer2hex(spBuffer) );
    return hr;
}

HRESULT CSerialPort::Write( CBufferPtr spBuffer )
{
    LPCTSTR pcf = _T("CSerialPort::Write()");
//    Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("%s: spBuffer=%s"), pcf, spbuffer2hex(spBuffer) );

    CEventParams EventParams;
    EventParams.SetWriteBuffer( spBuffer );

    EventValue_t eReturnValue = ExecuteFSM( ev_Write, EventParams );
    HRESULT hr = ( eReturnValue == ev_ReturnOk ) ? S_OK : E_FAIL;

//    Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: hr=0x%08lx"), pcf, hr );
    return hr;
}

HRESULT CSerialPort::OptimizeForPhone( LPCTSTR szPhoneType, LPCTSTR szManufacturer, LPCTSTR szIMEI, LPCTSTR szSoftwareVersion )
{
    // Currently only the SK65 is supported, which is accessible at 921600 baud

#ifdef FOR_PHONE_SIM // The phone simulator only supports fixed baudrate
#else
//    SetBaudrate( 921600 );
    SetBaudrate( 115200 );
#endif

    return S_OK;
}

// The FSMTable of CSerialPort controls the interaction between external functions and port control functions.
//
// External functions:
//   Open, Close, Read, Write, WaitResult
//
// Functions working on the port:
//   StartOpen, StartClose, StartRead, StartWrite, StartWait, GetOverlappedRead, GetOverlappedWrite
//
// The central processing point between these function groups is the function ExecuteFSM().
// Each external function calls ExecuteFSM() instead of calling port control functions directly.
// ExecuteFSM() uses the FSMTable to decide what to do. 


CSerialPort::FSMTableEntry CSerialPort::fsmTable[] = {

// Port is closed
{ is_Closed,                            ev_Open,                is_Closed_Open_StartOpen,               ac_Call_StartOpen },

// StartOpen() is running: opening the port
{ is_Closed_Open_StartOpen,             ev_ReturnOk,            is_Closed_Open_StartEvent,              ac_Call_StartEvent },
{ is_Closed_Open_StartOpen,             ev_ReturnError,         is_Closed,                              ac_Return_Error },

// StartEvent() is running: overlapped start of WaitCommEvent()
{ is_Closed_Open_StartEvent,            ev_ReturnSyncOk,        is_Unchanged,                           ac_Call_StartEvent },
{ is_Closed_Open_StartEvent,            ev_ReturnAsyncOk,       is_RNon_WNon,                           ac_Return_Ok },
{ is_Closed_Open_StartEvent,            ev_ReturnError,         is_Failure,                             ac_Return_Error },

// Something in my read buffer (m_spReadBuffer)
{ is_RNfd_WNon,                         ev_Read,                is_RNon_WNon,                           ac_Return_ReadBuffer },

// Nothing active
{ is_RNon_WNon,                         ev_Wait,                is_RNon_WNon_Wait_StartWait,            ac_Call_StartWait },
{ is_RNon_WNon,                         ev_StartRead,           is_RNon_WNon_StartRead,                 ac_Call_StartRead },
{ is_RNon_WNon,                         ev_Write,               is_RNon_WNon_Write_StartWrite,          ac_Call_StartWrite },

{ is_RNon_WNon_Wait_StartWait,          ev_ReturnWrClient,      is_RNon_WNon,                           ac_Return_WrClient },
{ is_RNon_WNon_Wait_StartWait,          ev_ReturnWrEvent,       is_RNon_WNon_Wait_GetOverlappedEvent,   ac_Call_GetOverlappedEvent },
{ is_RNon_WNon_Wait_StartWait,          ev_ReturnWrWrite,       is_Unchanged,                           acn_ResetWrWrite + ac_Call_StartWait },
{ is_RNon_WNon_Wait_StartWait,          ev_ReturnWrRead,        is_Unchanged,                           acn_ResetWrRead + ac_Call_StartWait },

{ is_RNon_WNon_Wait_GetOverlappedEvent, ev_ReturnOk,            is_RNon_WNon_Wait_StartEvent,           ac_Call_StartEvent },
{ is_RNon_WNon_Wait_GetOverlappedEvent, ev_ReturnError,         is_Failure,                             ac_Return_WrClose },

{ is_RNon_WNon_Wait_StartEvent,         ev_ReturnSyncOk,        is_Unchanged,                           ac_Call_StartEvent },
{ is_RNon_WNon_Wait_StartEvent,         ev_ReturnAsyncOk,       is_RNon_WNon_Wait_StartWait,            ac_Call_StartWait },
{ is_RNon_WNon_Wait_StartEvent,         ev_ReturnError,         is_Failure,                             ac_Return_WrClose },

{ is_RNon_WNon_StartRead,               ev_ReturnSyncOk,        is_RRdy_WNon,                           ac_Return_Ok },
{ is_RNon_WNon_StartRead,               ev_ReturnAsyncOk,       is_ROvl_WNon,                           ac_Return_Ok },
{ is_RNon_WNon_StartRead,               ev_ReturnError,         is_Failure,                             ac_Return_Error },

{ is_RNon_WNon_Write_StartWrite,        ev_ReturnSyncOk,        is_RNon_WRdy,                           ac_Return_Ok },
{ is_RNon_WNon_Write_StartWrite,        ev_ReturnAsyncOk,       is_RNon_WOvl,                           ac_Return_Ok },
{ is_RNon_WNon_Write_StartWrite,        ev_ReturnError,         is_Failure,                             ac_Return_Error },

// 
{ is_RRdy_WNon,                         ev_Wait,                is_RNfd_WNon,                           ac_Return_WrRead },
{ is_RRdy_WNon,                         ev_Write,               is_RRdy_WNon_Write_StartWrite,          ac_Call_StartWrite },

{ is_RRdy_WNon_Write_StartWrite,        ev_ReturnSyncOk,        is_RRdy_WRdy,                           ac_Return_Ok },
{ is_RRdy_WNon_Write_StartWrite,        ev_ReturnAsyncOk,       is_RRdy_WOvl,                           ac_Return_Ok },
{ is_RRdy_WNon_Write_StartWrite,        ev_ReturnError,         is_Failure,                             ac_Return_Error },

// Only overlapped read active
{ is_ROvl_WNon,                         ev_Wait,                is_ROvl_WNon_Wait_StartWait,            ac_Call_StartWait },
{ is_ROvl_WNon,                         ev_Write,               is_ROvl_WNon_Write_StartWrite,          ac_Call_StartWrite },

{ is_ROvl_WNon_Wait_StartWait,          ev_ReturnWrClient,      is_ROvl_WNon,                           ac_Return_WrClient },
{ is_ROvl_WNon_Wait_StartWait,          ev_ReturnWrRead,        is_ROvl_WNon_Wait_GetOverlappedRead,    ac_Call_GetOverlappedRead },
{ is_ROvl_WNon_Wait_StartWait,          ev_ReturnWrWrite,       is_Unchanged,                           acn_ResetWrWrite + ac_Call_StartWait },
{ is_ROvl_WNon_Wait_StartWait,          ev_ReturnWrEvent,       is_ROvl_WNon_Wait_GetOverlappedEvent,   ac_Call_GetOverlappedEvent },

{ is_ROvl_WNon_Wait_GetOverlappedRead,  ev_ReturnOk,            is_RNfd_WNon,                           ac_Return_WrRead },
{ is_ROvl_WNon_Wait_GetOverlappedRead,  ev_ReturnError,         is_Failure,                             ac_Return_WrClose },

{ is_ROvl_WNon_Wait_GetOverlappedEvent, ev_ReturnError,         is_Failure,                             ac_Return_WrClose },
{ is_ROvl_WNon_Wait_GetOverlappedEvent, ev_ReturnOk,            is_ROvl_WNon_Wait_StartEvent,           ac_Call_StartEvent },

{ is_ROvl_WNon_Wait_StartEvent,         ev_ReturnSyncOk,        is_Unchanged,                           ac_Call_StartEvent },
{ is_ROvl_WNon_Wait_StartEvent,         ev_ReturnAsyncOk,       is_ROvl_WNon_Wait_StartWait,            ac_Call_StartWait },
{ is_ROvl_WNon_Wait_StartEvent,         ev_ReturnError,         is_Failure,                             ac_Return_WrClose },

{ is_ROvl_WNon_Write_StartWrite,        ev_ReturnSyncOk,        is_ROvl_WRdy,                           ac_Return_Ok },
{ is_ROvl_WNon_Write_StartWrite,        ev_ReturnAsyncOk,       is_ROvl_WOvl,                           ac_Return_Ok },
{ is_ROvl_WNon_Write_StartWrite,        ev_ReturnError,         is_Failure,                             ac_Return_Error },

// Overlapped read is active, a write was completed. This has to be notified at next wait-call
{ is_ROvl_WRdy,                         ev_Wait,                is_ROvl_WNon,                           ac_Return_WrWrite },

// Nothing active, a write was completed. This has to be notified at next wait-call
{ is_RNon_WRdy,                         ev_Wait,                is_RNon_WNon,                           ac_Return_WrWrite },
{ is_RNon_WRdy,                         ev_StartRead,           is_RNon_WRdy_StartRead,                 ac_Call_StartRead },

{ is_RNon_WRdy_StartRead,               ev_ReturnSyncOk,        is_RRdy_WRdy,                           ac_Return_Ok },
{ is_RNon_WRdy_StartRead,               ev_ReturnAsyncOk,       is_ROvl_WRdy,                           ac_Return_Ok },
{ is_RNon_WRdy_StartRead,               ev_ReturnError,         is_Failure,                             ac_Return_Error },

// Data was received and a write was completed
{ is_RRdy_WRdy,                         ev_Wait,                is_RNfd_WRdy,                           ac_Return_WrRead },

// A read was notified, a write was completed
{ is_RNfd_WRdy,                         ev_Read,                is_RNon_WRdy,                           ac_Return_ReadBuffer },

// Overlapped read and overlapped write active.
{ is_ROvl_WOvl,                         ev_Wait,                is_ROvl_WOvl_Wait_StartWait,            ac_Call_StartWait },

{ is_ROvl_WOvl_Wait_StartWait,          ev_ReturnWrClient,      is_ROvl_WOvl,                           ac_Return_WrClient },
{ is_ROvl_WOvl_Wait_StartWait,          ev_ReturnWrRead,        is_ROvl_WOvl_Wait_GetOverlappedRead,    ac_Call_GetOverlappedRead },
{ is_ROvl_WOvl_Wait_StartWait,          ev_ReturnWrWrite,       is_ROvl_WOvl_Wait_GetOverlappedWrite,   ac_Call_GetOverlappedWrite },
{ is_ROvl_WOvl_Wait_StartWait,          ev_ReturnWrEvent,       is_ROvl_WOvl_Wait_GetOverlappedEvent,   ac_Call_GetOverlappedEvent },

{ is_ROvl_WOvl_Wait_GetOverlappedRead,  ev_ReturnOk,            is_RNfd_WOvl,                           ac_Return_WrRead },
{ is_ROvl_WOvl_Wait_GetOverlappedRead,  ev_ReturnError,         is_Failure,                             ac_Return_WrClose },

{ is_ROvl_WOvl_Wait_GetOverlappedWrite, ev_ReturnOk,            is_ROvl_WNon,                           ac_Return_WrWrite },
{ is_ROvl_WOvl_Wait_GetOverlappedWrite, ev_ReturnError,         is_Failure,                             ac_Return_WrClose },

{ is_ROvl_WOvl_Wait_GetOverlappedEvent, ev_ReturnError,         is_Failure,                             ac_Return_WrClose },
{ is_ROvl_WOvl_Wait_GetOverlappedEvent, ev_ReturnOk,            is_ROvl_WOvl_Wait_StartEvent,           ac_Call_StartEvent },

{ is_ROvl_WOvl_Wait_StartEvent,         ev_ReturnSyncOk,        is_Unchanged,                           ac_Call_StartEvent },
{ is_ROvl_WOvl_Wait_StartEvent,         ev_ReturnAsyncOk,       is_ROvl_WOvl_Wait_StartWait,            ac_Call_StartWait },
{ is_ROvl_WOvl_Wait_StartEvent,         ev_ReturnError,         is_Failure,                             ac_Return_WrClose },

// Overlapped write active and there is something in my read buffer
{ is_RNfd_WOvl,                         ev_Read,                is_RNon_WOvl,                           ac_Return_ReadBuffer },

// Only overlapped write active
{ is_RNon_WOvl,                         ev_Wait,                is_RNon_WOvl_Wait_StartWait,            ac_Call_StartWait },
{ is_RNon_WOvl,                         ev_StartRead,           is_RNon_WOvl_StartRead,                 ac_Call_StartRead },

{ is_RNon_WOvl_Wait_StartWait,          ev_ReturnWrClient,      is_RNon_WOvl,                           ac_Return_WrClient },
{ is_RNon_WOvl_Wait_StartWait,          ev_ReturnWrWrite,       is_RNon_WOvl_Wait_GetOverlappedWrite,   ac_Call_GetOverlappedWrite },
{ is_RNon_WOvl_Wait_StartWait,          ev_ReturnWrEvent,       is_RNon_WOvl_Wait_GetOverlappedEvent,   ac_Call_GetOverlappedEvent },
{ is_RNon_WOvl_Wait_StartWait,          ev_ReturnWrRead,        is_Unchanged,                           acn_ResetWrRead + ac_Call_StartWait },

{ is_RNon_WOvl_Wait_GetOverlappedWrite, ev_ReturnOk,            is_RNon_WNon,                           ac_Return_WrWrite },
{ is_RNon_WOvl_Wait_GetOverlappedWrite, ev_ReturnError,         is_Failure,                             ac_Return_WrClose },

{ is_RNon_WOvl_Wait_GetOverlappedEvent, ev_ReturnError,         is_Failure,                             ac_Return_WrClose },
{ is_RNon_WOvl_Wait_GetOverlappedEvent, ev_ReturnOk,            is_RNon_WOvl_Wait_StartEvent,           ac_Call_StartEvent },

{ is_RNon_WOvl_Wait_StartEvent,         ev_ReturnSyncOk,        is_Unchanged,                           ac_Call_StartEvent },
{ is_RNon_WOvl_Wait_StartEvent,         ev_ReturnAsyncOk,       is_RNon_WOvl_Wait_StartWait,            ac_Call_StartWait },
{ is_RNon_WOvl_Wait_StartEvent,         ev_ReturnError,         is_Failure,                             ac_Return_WrClose },

{ is_RNon_WOvl_StartRead,               ev_ReturnSyncOk,        is_RRdy_WOvl,                           ac_Return_Ok },
{ is_RNon_WOvl_StartRead,               ev_ReturnAsyncOk,       is_ROvl_WOvl,                           ac_Return_Ok },
{ is_RNon_WOvl_StartRead,               ev_ReturnError,         is_Failure,                             ac_Return_Error },

// 
{ is_RRdy_WOvl,                         ev_Wait,                is_RNfd_WOvl,                           ac_Return_WrRead },

// Something failed - to continue Close must be called
{ is_Failure,                           ev_Wait,                is_Unchanged,                           ac_Return_WrClose },

// Every state can handle Close()
{ is_ALL,                               ev_Close,               is_Closing,                             ac_Call_StartClose },

// StartClose() is running
{ is_Closing,                           ev_ReturnOk,            is_Closed,                              ac_Return_WrClose },

// End-Of-Table marker
{ is_Unchanged,                         ev_Undefined,           is_Unchanged,                           ac_Error } };

CSerialPort::EventValue_t CSerialPort::ExecuteFSM( EventValue_t eInputEventValue, CEventParams& EventParams )
{
    EventValue_t eReturnValue = ev_Undefined;
    EventValue_t eEventValue = eInputEventValue;
    FSMTableEntry* pTableEntry = NULL;

    for( eReturnValue = ev_Undefined; eReturnValue == ev_Undefined; )
    {
        // Find the appropriate FSMTableEntry
        for( pTableEntry = &fsmTable[0]; pTableEntry->eInitialState != is_Unchanged; pTableEntry++ )
        {
            if( pTableEntry->eInitialState == m_eState && pTableEntry->eEventValue == eEventValue )
                break;
            if( pTableEntry->eInitialState == is_ALL && pTableEntry->eEventValue == eEventValue )
                break;
        }
        if( pTableEntry->eInitialState == is_Unchanged )
        {
            eReturnValue = ev_ReturnError;
            break;
        }

        // Switch the FSM state
        if( pTableEntry->eTargetState != is_Unchanged )
        {
			//TRACE(_T("%s(%s), \n"),InternalState2t(m_eState),EventValue2t(eInputEventValue));
            m_eState = pTableEntry->eTargetState;
        }

        if( pTableEntry->eActionValue & acn_ResetWrWrite )
            ResetWrWrite( EventParams );

        if( pTableEntry->eActionValue & acn_ResetWrRead )
            ResetWrRead( EventParams );

        // Execute the action
        switch( pTableEntry->eActionValue & ~acn_Mask )
        {
        case ac_Return_WrClose:
            eReturnValue = ev_ReturnWrClose;
            break;
        case ac_Return_ReadBuffer:
            eReturnValue = ev_ReturnOk;
            EventParams.SetReadBuffer( m_spReadBuffer );
            break;
        case ac_Return_Ok:
            eReturnValue = ev_ReturnOk;
            break;
        case ac_Return_Error:
            eReturnValue = ev_ReturnError;
            break;
        case ac_Return_WrClient:
            eReturnValue = ev_ReturnWrClient;
            break;
        case ac_Return_WrRead:
            eReturnValue = ev_ReturnWrRead;
            break;
        case ac_Return_WrWrite:
            eReturnValue = ev_ReturnWrWrite;
            break;
        case ac_Call_StartOpen:
            eEventValue = StartOpen( EventParams );
            break;
        case ac_Call_StartClose:
            eEventValue = StartClose( EventParams );
            break;
        case ac_Call_StartRead:
            eEventValue = StartRead( EventParams );
            break;
        case ac_Call_StartWrite:
            eEventValue = StartWrite( EventParams );
            break;
        case ac_Call_StartEvent:
            eEventValue = StartEvent( EventParams );
            break;
        case ac_Call_StartWait:
            eEventValue = StartWait( EventParams );
            break;
        case ac_Call_GetOverlappedRead:
            eEventValue = GetOverlappedRead( EventParams );
            break;
        case ac_Call_GetOverlappedWrite:
            eEventValue = GetOverlappedWrite( EventParams );
            break;
        case ac_Call_GetOverlappedEvent:
            eEventValue = GetOverlappedEvent( EventParams );
            break;
        default:
//            Traceit( TRACEIT_ERROR, TG_CSerialPort, _T("%s: Action not found: action=%s"),
//                pcf, ActionValues2t(pTableEntry->eActionValue) );
            eReturnValue = ev_ReturnError;
            break;
        }
    };

//    Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: eReturnValue=%s"), pcf, EventValue2t(eReturnValue) );
    return eReturnValue;
}

CSerialPort::EventValue_t CSerialPort::StartWait( CEventParams& EventParams )
{
    LPCTSTR pcf = _T("CSerialPort::StartWait()");
//    Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("%s"), pcf );

    EventValue_t eEventValue = ev_Undefined;

    HANDLE hWaitForHandles[4];
    hWaitForHandles[0] = EventParams.GetEventClient();
    hWaitForHandles[1] = (HANDLE) m_eventRead;
    hWaitForHandles[2] = (HANDLE) m_eventWrite;
    hWaitForHandles[3] = (HANDLE) m_eventEvent;

    DWORD dwResult = WaitForMultipleObjects( 4, hWaitForHandles, FALSE, INFINITE );

    switch( dwResult )
    {
    case WAIT_OBJECT_0:
        eEventValue = ev_ReturnWrClient;
        break;

    case WAIT_OBJECT_0 + 1:
        eEventValue = ev_ReturnWrRead;
        break;

    case WAIT_OBJECT_0 + 2:
        eEventValue = ev_ReturnWrWrite;
        break;

    case WAIT_OBJECT_0 + 3:
        eEventValue = ev_ReturnWrEvent;
        break;
    }

//    Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: eEventValue=%s"), pcf, EventValue2t(eEventValue) );
    return eEventValue;
}

CSerialPort::EventValue_t CSerialPort::StartRead( CEventParams& EventParams )
{
    LPCTSTR pcf = _T("CSerialPort::StartRead()");
//    Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("%s"), pcf );

    EventValue_t eEventValue = ev_Undefined;
    DWORD dwBytesTransfered = 0;

    m_dwBytesToRead = EventParams.GetNoOfExpectedBytes();
    if( m_dwBytesToRead == 0 )
        m_dwBytesToRead = 1000;

    m_spReadBuffer = new CBuffer;
    BOOL bResult = ReadFile( m_hPortId, m_spReadBuffer->GetBufferSetLength(m_dwBytesToRead), m_dwBytesToRead,
        &dwBytesTransfered, &m_overlappedRead );
    if( bResult )
    {
        // Sync read
        m_spReadBuffer->ReleaseBuffer(dwBytesTransfered);
        EventParams.SetReadBuffer( m_spReadBuffer );
        eEventValue = ev_ReturnSyncOk;
    }
    else
    {
        DWORD dwLastError = ::GetLastError();

        if( dwLastError == ERROR_IO_PENDING )
        {
            eEventValue = ev_ReturnAsyncOk;
        }
        else
        {
//            Traceit( TRACEIT_INFO, TG_CSerialPort, _T("%s: ReadFile returned FALSE, GetLastError()=%s"),
//                pcf, LastError2t(dwLastError) );
            eEventValue = ev_ReturnError;
        }
    }

//    Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: eEventValue=%s"), pcf, EventValue2t(eEventValue) );
    return eEventValue;
}

CSerialPort::EventValue_t CSerialPort::StartWrite( CEventParams& EventParams )
{
    LPCTSTR pcf = _T("CSerialPort::StartWrite()");
//    Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("%s"), pcf );

    EventValue_t eEventValue = ev_Undefined;
    DWORD dwBytesTransfered = 0;

    m_spWriteBuffer = EventParams.GetWriteBuffer();
    BOOL bResult = WriteFile( m_hPortId, m_spWriteBuffer->GetData(), m_spWriteBuffer->GetLength(),
        &dwBytesTransfered, &m_overlappedWrite );
    if( bResult )
    {
        // Sync write
        eEventValue = ev_ReturnSyncOk;
    }
    else
    {
        DWORD dwLastError = ::GetLastError();

        if( dwLastError == ERROR_IO_PENDING )
        {
            eEventValue = ev_ReturnAsyncOk;
        }
        else
        {
//            Traceit( TRACEIT_INFO, TG_CSerialPort, _T("%s: WriteFile returned FALSE, GetLastError()=%s"),
//                pcf, LastError2t(dwLastError) );
            eEventValue = ev_ReturnError;
        }
    }

//    Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: eEventValue=%s"), pcf, EventValue2t(eEventValue) );
    return eEventValue;
}

CSerialPort::EventValue_t CSerialPort::StartEvent( CEventParams& EventParams )
{
    LPCTSTR pcf = _T("CSerialPort::StartEvent()");
//    Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("%s"), pcf );

    EventValue_t eEventValue = ev_Undefined;

    BOOL bResult = WaitCommEvent( m_hPortId, &m_dwEventCommMask, &m_overlappedEvent );

    if( bResult )
    {
//        Traceit( TRACEIT_INFO, TG_CSerialPort, _T("%s: WaitCommEvent completed synchronously: commmask=0x%08lx, DSR=%s, ERR=%s"),
//            pcf, m_dwEventCommMask, bool2t((m_dwEventCommMask & EV_DSR)?true:false), bool2t((m_dwEventCommMask & EV_ERR)?true:false) );

        eEventValue = ev_ReturnSyncOk;
    }
    else
    {
	    COMSTAT tagComStat;
        DWORD dwCommErrors = 0;
        DWORD dwLastError = ::GetLastError();

        if( dwLastError == ERROR_IO_PENDING )
        {
//            Traceit( TRACEIT_INFO, TG_CSerialPort, _T("%s: GetLastError=ERROR_IO_PENDING"), pcf );

            eEventValue = ev_ReturnAsyncOk;
        }
        else
        {
//            Traceit( TRACEIT_INFO, TG_CSerialPort, _T("%s: GetLastError=%s"), pcf, LastError2t(dwLastError) );
            ::ClearCommError(m_hPortId, &dwCommErrors, &tagComStat);

            eEventValue = ev_ReturnError;
        }
    }

//    Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: eEventValue=%s"), pcf, EventValue2t(eEventValue) );
    return eEventValue;
}

CSerialPort::EventValue_t CSerialPort::GetOverlappedRead( CEventParams& EventParams )
{
    LPCTSTR pcf = _T("CSerialPort::GetOverlappedRead()");
//    Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("%s"), pcf );

    EventValue_t eEventValue = ev_Undefined;

    DWORD dwBytesTransfered = 0;
    BOOL bResult = GetOverlappedResult( m_hPortId, &m_overlappedRead, &dwBytesTransfered, TRUE );
    m_eventRead.ResetEvent();

    if( bResult )
    {
//        Traceit( TRACEIT_INFO, TG_CSerialPort, _T("%s: GetOverlappedResult returned TRUE, %ld bytes read"), pcf, dwBytesTransfered );
        m_spReadBuffer->ReleaseBuffer(dwBytesTransfered);
        eEventValue = ev_ReturnOk;
    }
    else
    {
        DWORD dwLastError = ::GetLastError();
	    COMSTAT tagComStat;
        DWORD dwCommErrors = 0;

//        Traceit( TRACEIT_INFO, TG_CSerialPort, _T("%s: GetOverlappedResult returned FALSE, GetLastError=%s"),
//            pcf, LastError2t(dwLastError) );
        ::ClearCommError(m_hPortId, &dwCommErrors, &tagComStat);
        eEventValue = ev_ReturnError;
    }

//    Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: eEventValue=%s"), pcf, EventValue2t(eEventValue) );
    return eEventValue;
}

CSerialPort::EventValue_t CSerialPort::GetOverlappedWrite( CEventParams& EventParams )
{
    LPCTSTR pcf = _T("CSerialPort::GetOverlappedWrite()");
//    Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("%s"), pcf );

    EventValue_t eEventValue = ev_Undefined;

    DWORD dwBytesTransfered = 0;
    BOOL bResult = GetOverlappedResult( m_hPortId, &m_overlappedWrite, &dwBytesTransfered, TRUE );
    m_eventWrite.ResetEvent();

    if( bResult )
    {
//        Traceit( TRACEIT_INFO, TG_CSerialPort, _T("%s: GetOverlappedResult returned TRUE, %ld bytes written"), pcf, dwBytesTransfered );
        eEventValue = ev_ReturnOk;
    }
    else
    {
        DWORD dwLastError = ::GetLastError();
	    COMSTAT tagComStat;
        DWORD dwCommErrors = 0;

//        Traceit( TRACEIT_INFO, TG_CSerialPort, _T("%s: GetOverlappedResult returned FALSE, GetLastError=%s"),
//            pcf, LastError2t(dwLastError) );
        ::ClearCommError(m_hPortId, &dwCommErrors, &tagComStat);
        eEventValue = ev_ReturnError;
    }

//    Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: eEventValue=%s"), pcf, EventValue2t(eEventValue) );
    return eEventValue;
}

CSerialPort::EventValue_t CSerialPort::GetOverlappedEvent( CEventParams& EventParams )
{
    LPCTSTR pcf = _T("CSerialPort::GetOverlappedEvent()");
//    Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("%s"), pcf );

    EventValue_t eEventValue = ev_Undefined;

    DWORD dwBytesTransfered = 0;
    BOOL bResult = GetOverlappedResult( m_hPortId, &m_overlappedEvent, &dwBytesTransfered, TRUE );
    m_eventEvent.ResetEvent();

    if( bResult )
    {
//        Traceit( TRACEIT_INFO, TG_CSerialPort, _T("%s: WaitCommEvent completed: commmask=0x%08lx, DSR=%s, ERR=%s"),
//            pcf, m_dwEventCommMask, bool2t((m_dwEventCommMask & EV_DSR)?true:false), bool2t((m_dwEventCommMask & EV_ERR)?true:false) );

        if( m_dwEventCommMask & EV_DSR ) // DSR toggled
        {
            DWORD dwCommMask = 0;
            BOOL bResult = GetCommModemStatus( m_hPortId, &dwCommMask );

            if( !bResult )
            {
                DWORD dwLastError = ::GetLastError();
//                Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: GetCommModemStatus returned FALSE, dwLastError=%s"),
//                    pcf, LastError2t(dwLastError) );
                eEventValue = ev_ReturnError;
            }
            else
            {
                if( (dwCommMask & MS_DSR_ON) == 0 )
                {
//                    Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: GetCommModemStatus returned MS_DSR_ON=0: dwCommMask=0x%08lx"),
//                        pcf, dwCommMask );
                    eEventValue = ev_ReturnError;
                }
                else
                {
//                    Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: GetCommModemStatus returned MS_DSR_ON=1: dwCommMask=0x%08lx"),
//                        pcf, dwCommMask );
                    eEventValue = ev_ReturnOk;
                }
            }
        }
        else if (m_dwEventCommMask & EV_ERR)
        {
            
            DWORD lastError = 0;
            COMSTAT comStat;
            BOOL bSuccess = ClearCommError(m_hPortId, &lastError, &comStat);
            if(bSuccess)
            {
//                Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: ClearCommError succeeded error=%s, ComStatus=%s"),
//                    pcf, CCError2t(lastError), COMStatus2t(comStat) );    
            }
            else
            {
                DWORD dwLastError = ::GetLastError();
//                Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: ClearCommError  failed lastError=%s"),
//                    pcf, LastError2t(dwLastError) );    
            }                        
            
            eEventValue = ev_ReturnError;            
        }
        else
             eEventValue = ev_ReturnOk;
    }
    else
    {
        DWORD dwLastError = ::GetLastError();
	    COMSTAT tagComStat;
        DWORD dwCommErrors = 0;

//        Traceit( TRACEIT_INFO, TG_CSerialPort, _T("%s: GetOverlappedResult returned FALSE, GetLastError=%s"),
//            pcf, LastError2t(dwLastError) );
        ::ClearCommError(m_hPortId, &dwCommErrors, &tagComStat);

        eEventValue = ev_ReturnError;
    }

//    Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: eEventValue=%s"), pcf, EventValue2t(eEventValue) );
    return eEventValue;
}

CSerialPort::EventValue_t CSerialPort::StartOpen( CEventParams& EventParams )
{
    LPCTSTR pcf = _T("CSerialPort::StartOpen()");
//  Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("%s: PortName:%s"), pcf, EventParams.GetPort() );

    EventValue_t eEventValue = ev_Undefined;

    if( m_hPortId != INVALID_HANDLE_VALUE )
    {
//        Traceit( TRACEIT_INFO, TG_CSerialPort, _T("%s: Already open"), pcf );
        eEventValue = ev_ReturnError;
    }
    else
    {

        m_strPortName.Format( _T("\\\\.\\%s"), EventParams.GetPort() );

	    // Open the port.  The device must exist and must be opened for
	    // exclusive access (no sharing).  No security is used, overlapped
	    // I/O is enabled.
	    //m_hPortId = CreateFile(m_strPortName, GENERIC_READ | GENERIC_WRITE,
		//				    0, NULL, OPEN_EXISTING,
		//				    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH | FILE_FLAG_OVERLAPPED, NULL);

		m_hPortId = CreateFile(m_strPortName,GENERIC_READ | GENERIC_WRITE,
							0, 0, OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED,0);
			
        if(m_hPortId == INVALID_HANDLE_VALUE)
        {
            DWORD dwLastError = ::GetLastError();
            eEventValue = ev_ReturnError;
        }
        else
        {
            SetCommMask( m_hPortId, EV_ERR | EV_DSR );

            memset( &m_overlappedRead, 0, sizeof(OVERLAPPED) );
            memset( &m_overlappedWrite, 0, sizeof(OVERLAPPED) );
            memset( &m_overlappedEvent, 0, sizeof(OVERLAPPED) );

            m_eventRead.ResetEvent();
            m_eventWrite.ResetEvent();
            m_eventEvent.ResetEvent();

            m_overlappedRead.hEvent = (HANDLE) m_eventRead;
            m_overlappedWrite.hEvent = (HANDLE) m_eventWrite;
            m_overlappedEvent.hEvent = (HANDLE) m_eventEvent;

			SetBaudrate(EventParams.GetBoundRate());
            SetParityDataStop();
			
			//SetFlowControl();
            //SetBufferSizes(4096,4096);
                                // nInterval, nMultiplier, nConstant
	        SetReadTimeouts(       1,         0,           0 );
	        SetWriteTimeouts();

	        PurgeCommPort();

            DWORD dwCommMask = 0;
            BOOL bResult = GetCommModemStatus( m_hPortId, &dwCommMask );

            if( !bResult )
            {
                DWORD dwLastError = ::GetLastError();
//                Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: GetCommModemStatus returned FALSE, dwLastError=%s"),
//                    pcf, LastError2t(dwLastError) );
                ::CloseHandle( m_hPortId );
                m_hPortId = INVALID_HANDLE_VALUE;
                eEventValue = ev_ReturnError;
            }
            else
            {
                //if( (dwCommMask & MS_DSR_ON) == 0 )
				if(false)
                {
//                    Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: GetCommModemStatus returned MS_DSR_ON=0: dwCommMask=0x%08lx"),
//                        pcf, dwCommMask );
                    ::CloseHandle( m_hPortId );
                    m_hPortId = INVALID_HANDLE_VALUE;
                    eEventValue = ev_ReturnError;
                }
                else
                {
                    eEventValue = ev_ReturnOk;
                }
            }
        }
    }
    	
//    Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: eEventValue=%s"), pcf, EventValue2t(eEventValue) );
    return eEventValue;
}

CSerialPort::EventValue_t CSerialPort::StartClose( CEventParams& EventParams )
{
    LPCTSTR pcf = _T("CSerialPort::Close()");
//  Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("%s"), pcf );
    HRESULT hr = S_OK;

    if(m_hPortId == INVALID_HANDLE_VALUE)
    {
//        Traceit( TRACEIT_INFO, TG_CSerialPort, _T("%s: Already closed"), pcf );
    }
    else
    {
        // Purge any outstanding reads/writes
        ::CancelIo( m_hPortId );
        PurgeCommPort(PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);

		::CloseHandle(m_hPortId);
        m_hPortId = INVALID_HANDLE_VALUE;
    }

//  Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: eEventValue=%s"), pcf, EventValue2t(ev_ReturnOk) );
    return ev_ReturnOk;
}

void CSerialPort::ResetWrWrite( CEventParams& EventParams )
{
    m_eventWrite.ResetEvent();
}

void CSerialPort::ResetWrRead( CEventParams& EventParams )
{
    m_eventRead.ResetEvent();
}

//-----------------------------------------------------------------
// Function			: CSerialPort::SetBaudrate
// Description		: 
// Returnvalue		: int baud rate afterwards (0 means no change)
// Argument         : int nBaudrate
//-----------------------------------------------------------------
int CSerialPort::SetBaudrate( int nBaudrate )
{
    LPCTSTR pcf = _T("CSerialPort::SetBaudrate()");
//	Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("%s: nBaudrate=%d"), pcf, nBaudrate );
	
    DCB dcb;
    DWORD dwLastError = ERROR_SUCCESS;

    // Always retrieve the current state before modifying it
    dcb.DCBlength = sizeof(DCB);
    BOOL bRetVal = ::GetCommState( m_hPortId, &dcb );

    if(bRetVal)
    {
        bRetVal = FALSE;

        for( ; nBaudrate != 0; nBaudrate = GetSlowerBaudrate(nBaudrate) )
        {
            dcb.BaudRate = nBaudrate;
            bRetVal = ::SetCommState(m_hPortId, &dcb);
            if( bRetVal )
                break;
            else
            {
                DWORD dwLastError = ::GetLastError();
//            	Traceit( TRACEIT_INFO, TG_CSerialPort, _T("%s: ::SetCommState( nBaudrate=%d ) failed: GetLastError() = %s"), pcf, nBaudrate, LastError2t(dwLastError) );
            }
        }

        if(bRetVal)
        {
            // nBaudRate contains the new baud rate
        }
        else
        {
            nBaudrate = 0; // means unchanged
        }
    }

//    Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("%s: ret=%d"), pcf, nBaudrate );
	return nBaudrate;
}

int CSerialPort::GetSlowerBaudrate( int nBaudrate )
{
    int pnBaudrates[] = { 921600, 460800, 230400, 115200, 57600, 38400, 19200, 9600 };

    for( int i = 0; i < sizeof(pnBaudrates)/sizeof(pnBaudrates[0]); i++ )
    {
        if( nBaudrate > pnBaudrates[i] )
            return pnBaudrates[i];
    }

    return 0;
}

//-----------------------------------------------------------------
// Function			: CSerialPort::SetParityDataStop
// Description		: 
// Returnvalue		: BOOL 
// Argument         : int nParity
// Argument         : int nDataBits
// Argument         : int nStopBits
//-----------------------------------------------------------------

bool CSerialPort::SetParityDataStop(int nParity, int nDataBits,int nStopBits)
{
//	Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("CSerialPort::SetParityDataStop(nParity=%d nDataBits=%d nStopBits=%d)"), nParity, nDataBits, nStopBits );
    bool bSuccess = true;

    DCB dcb;
    BOOL bRetVal;

    dcb.DCBlength = sizeof(DCB);
    bRetVal = ::GetCommState(m_hPortId, &dcb);

    if(bRetVal)
    {
        dcb.Parity   = nParity;
        dcb.ByteSize = nDataBits;
        dcb.StopBits = nStopBits;
        dcb.fParity  = TRUE;        // Parity errors are reported

        // Enable the options
        bRetVal = ::SetCommState(m_hPortId, &dcb);
    }

    bSuccess = bRetVal ? true : false;

//	Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("CSerialPort::SetParityDataStop() ret=%s"), bool2t(bSuccess) );
    return bSuccess;
}

// CSerialPort::SetFlowControl
// 
// Setting flow control can get a little involved.  This function takes
// care of all the necessary details based upon the parameters passed.
// 
// int nFlowCtrl
//    [in]	
// 
// int nXOnLimit
//    [in]	
// 
// int nXOffLimit
//    [in]	
// 
// char chXOnChar
//    [in]	
// 
// char chXOffChar
//    [in]	
// 
// return
// 	BOOL 
// 

bool CSerialPort::SetFlowControl(int nFlowCtrl, int nXOnLimit,int nXOffLimit, char chXOnChar, char chXOffChar)
{
//	Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("CSerialPort::SetFlowControl( nFlowCtrl=%d, nXOnLimit=%d, nXOffLimit=%d, chXOnChar=%d, chXOffChar=%d )"),
//        nFlowCtrl, nXOnLimit, nXOffLimit, (int)chXOnChar, (int)chXOffChar );
    bool bSuccess = true;
	
    DCB  dcb;
    BYTE byOnOff;
    BOOL bRetVal;

    dcb.DCBlength = sizeof(DCB);
    bRetVal = ::GetCommState(m_hPortId, &dcb);

    if(bRetVal)
    {
        // Set hardware flow control options
        byOnOff = (BYTE)((nFlowCtrl & PCF_DTRDSR) != 0);
        dcb.fOutxDsrFlow = byOnOff;
        if(byOnOff)
            dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
        else
            dcb.fDtrControl = DTR_CONTROL_ENABLE;

        byOnOff = (BYTE)((nFlowCtrl & PCF_RTSCTS) != 0);
        dcb.fOutxCtsFlow = byOnOff;
        if(byOnOff)
            dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
        else
            dcb.fRtsControl = RTS_CONTROL_ENABLE;

        // Set software flow control options
        byOnOff = (BYTE)((nFlowCtrl & PCF_XONXOFF) != 0);
        dcb.fInX = dcb.fOutX = byOnOff;

        dcb.XonChar = chXOnChar;
        dcb.XoffChar = chXOffChar;
        dcb.XonLim = nXOnLimit;
        dcb.XoffLim = nXOffLimit;

        // Miscellaneous
        dcb.fBinary = TRUE;     // Must always be TRUE for NT
        dcb.fNull = FALSE;      // Keep received null bytes

        // Enable the options
        bRetVal = ::SetCommState(m_hPortId, &dcb);

        bSuccess = bRetVal ? true : false;
    }

//    Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("CSerialPort::SetFlowControl() ret=%s"), bool2t(bSuccess) );
	return bSuccess;
}

// CSerialPort::SetBufferSizes
// 
//	
// 
// int nInBufSize
//    [in]	
// 
// int nOutBufSize
//    [in]	
// 
// return
// 	BOOL 
// 

bool CSerialPort::SetBufferSizes(int nInBufSize, int nOutBufSize)
{
//    Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("CSerialPort::SetBufferSizes( nInBufSize=%d, nOutBufSize=%d)"), nInBufSize, nOutBufSize );

    BOOL bRetVal = ::SetupComm(m_hPortId, nInBufSize, nOutBufSize);

    bool bSuccess = bRetVal ? true : false;
//	Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("CSerialPort::SetBufferSizes() ret=%s"), bool2t(bSuccess) );
	return bSuccess;
}

// CSerialPort::SetReadTimeouts
// 
//	
// 
// int nInterval
//    [in]	
// 
// int nMultiplier
//    [in]	
// 
// int nConstant
//    [in]	
// 
// return
// 	BOOL 
// 
bool CSerialPort::SetReadTimeouts(int nInterval, int nMultiplier, int nConstant)
{
//    Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("CSerialPort::SetReadTimeouts( nInterval=%d, nMultiplier=%d, nConstant=%d )"),
//        nInterval, nMultiplier, nConstant );
	
	COMMTIMEOUTS  tagCommTimeouts;
    BOOL bRetVal;

    // Set the read timeout values
    bRetVal = ::GetCommTimeouts(m_hPortId, &tagCommTimeouts);
    if(bRetVal)
    {
        tagCommTimeouts.ReadIntervalTimeout = nInterval;
        tagCommTimeouts.ReadTotalTimeoutMultiplier = nMultiplier;
        tagCommTimeouts.ReadTotalTimeoutConstant = nConstant;
        bRetVal = ::SetCommTimeouts(m_hPortId, &tagCommTimeouts);
    }

    bool bSuccess = bRetVal ? true : false;

//	Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("CSerialPort::SetReadTimeouts() ret=%s"), bool2t(bSuccess) );
	return bSuccess;
}

// CSerialPort::SetWriteTimeouts
// 
//	
// 
// int nMultiplier
//    [in]	
// 
// int nConstant
//    [in]	
// 
// return
// 	BOOL 
// 
bool CSerialPort::SetWriteTimeouts(int nMultiplier, int nConstant)
{
//    Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("CSerialPort::SetWriteTimeouts( nMultiplier=%d, nConstant=%d )"), nMultiplier, nConstant );
	
	COMMTIMEOUTS  tagCommTimeouts;
    BOOL bRetVal;

    // Set the write timeout values
    bRetVal = ::GetCommTimeouts(m_hPortId, &tagCommTimeouts);
    if(bRetVal)
    {
        tagCommTimeouts.WriteTotalTimeoutMultiplier = nMultiplier;
        tagCommTimeouts.WriteTotalTimeoutConstant = nConstant;
        bRetVal = ::SetCommTimeouts(m_hPortId, &tagCommTimeouts);
    }

    bool bSuccess = bRetVal ? true : false;
//	Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("CSerialPort::SetWriteTimeouts() ret=%s"), bool2t(bSuccess) );
	return bSuccess;
}

// CSerialPort::PurgeCommPort
// 
//	The PurgeComm function discards all characters 
//	from the output or input buffer of a specified 
//	communications resource. It can also terminate 
//	pending read or write operations on the resource.
// 
// DWORD fdwAction
//    [in]	
// 
// return
// 	BOOL 
// 
bool CSerialPort::PurgeCommPort(DWORD fdwAction)
{
//    Traceit( TRACEIT_ENTER, TG_CSerialPort, _T("CSerialPort::PurgeCommPort( fdwAction=0x%08lx )"), fdwAction );
	
    BOOL bRetVal = ::PurgeComm(m_hPortId, fdwAction);

#ifdef _DEBUG
    if(!bRetVal)
    {
        DWORD dwLastError = 0;
        dwLastError = ::GetLastError();
    }
#endif

    bool bSuccess = bRetVal ? true : false;
//    Traceit( TRACEIT_LEAVE, TG_CSerialPort, _T("CSerialPort::PurgeCommPort() ret=%s"), bool2t(bSuccess) );
	return bSuccess;
}

LPCTSTR CSerialPort::InternalState2t( InternalState_t e )
{
    switch(e)
    {
    case is_Unchanged                           : return _T("is_Unchanged");
    case is_Closed                              : return _T("is_Closed");
    case is_Closing                             : return _T("is_Closing");
    case is_ROvl_WNon                           : return _T("is_ROvl_WNon");
    case is_RNfd_WNon                           : return _T("is_RNfd_WNon");
    case is_RNon_WNon                           : return _T("is_RNon_WNon");
    case is_ROvl_WRdy                           : return _T("is_ROvl_WRdy");
    case is_ROvl_WOvl                           : return _T("is_ROvl_WOvl");
    case is_RNon_WOvl                           : return _T("is_RNon_WOvl");
    case is_RNon_WRdy                           : return _T("is_RNon_WRdy");
    case is_RNfd_WOvl                           : return _T("is_RNfd_WOvl");
    case is_RNfd_WRdy                           : return _T("is_RNfd_WRdy");
    case is_RNon_WRdy_StartRead                 : return _T("is_RNon_WRdy_StartRead");
    case is_RNon_WNon_Wait_StartWait            : return _T("is_RNon_WNon_Wait_StartWait");
    case is_RNon_WNon_Wait_GetOverlappedEvent   : return _T("is_RNon_WNon_Wait_GetOverlappedEvent");
    case is_RNon_WNon_Wait_StartEvent           : return _T("is_RNon_WNon_Wait_StartEvent");
    case is_Closed_Open_StartOpen               : return _T("is_Closed_Open_StartOpen");
    case is_Closed_Open_StartEvent              : return _T("is_Closed_Open_StartEvent");
    case is_RNon_WNon_StartRead                 : return _T("is_RNon_WNon_StartRead");
    case is_ROvl_WNon_Wait_StartWait            : return _T("is_ROvl_WNon_Wait_StartWait");
    case is_ROvl_WNon_Wait_GetOverlappedRead    : return _T("is_ROvl_WNon_Wait_GetOverlappedRead");
    case is_ROvl_WNon_Wait_GetOverlappedEvent   : return _T("is_ROvl_WNon_Wait_GetOverlappedEvent");
    case is_ROvl_WNon_Wait_StartEvent           : return _T("is_ROvl_WNon_Wait_StartEvent");
    case is_ROvl_WNon_Write_StartWrite          : return _T("is_ROvl_WNon_Write_StartWrite");
    case is_RNon_WNon_Write_StartWrite          : return _T("is_RNon_WNon_Write_StartWrite");
    case is_ROvl_WOvl_Wait_StartWait            : return _T("is_ROvl_WOvl_Wait_StartWait");
    case is_ROvl_WOvl_Wait_GetOverlappedRead    : return _T("is_ROvl_WOvl_Wait_GetOverlappedRead");
    case is_ROvl_WOvl_Wait_GetOverlappedWrite   : return _T("is_ROvl_WOvl_Wait_GetOverlappedWrite");
    case is_ROvl_WOvl_Wait_GetOverlappedEvent   : return _T("is_ROvl_WOvl_Wait_GetOverlappedEvent");
    case is_RNon_WOvl_StartRead                 : return _T("is_RNon_WOvl_StartRead");
    case is_ROvl_WOvl_Wait_StartEvent           : return _T("is_ROvl_WOvl_Wait_StartEvent");
    case is_RNon_WOvl_Wait_StartWait            : return _T("is_RNon_WOvl_Wait_StartWait");
    case is_RNon_WOvl_Wait_GetOverlappedWrite   : return _T("is_RNon_WOvl_Wait_GetOverlappedWrite");
    case is_RNon_WOvl_Wait_GetOverlappedEvent   : return _T("is_RNon_WOvl_Wait_GetOverlappedEvent");
    case is_RNon_WOvl_Wait_StartEvent           : return _T("is_RNon_WOvl_Wait_StartEvent");
    case is_RRdy_WNon                           : return _T("is_RRdy_WNon");
    case is_RRdy_WNon_Write_StartWrite          : return _T("is_RRdy_WNon_Write_StartWrite");
    case is_RRdy_WRdy                           : return _T("is_RRdy_WRdy");
    case is_RRdy_WOvl                           : return _T("is_RRdy_WOvl");

    case is_Failure                             : return _T("is_Failure");
    case is_ALL                                 : return _T("isALL");
    default                                     : return _T("*** unknown ***");
    }
}
 
LPCTSTR CSerialPort::EventValue2t( EventValue_t e )
{
    switch(e)
    {
    case ev_Undefined                           : return _T("ev_Undefined");
    case ev_Open                                : return _T("ev_Open");
    case ev_Close                               : return _T("ev_Close");
    case ev_Wait                                : return _T("ev_Wait");
    case ev_StartRead                           : return _T("ev_StartRead");
    case ev_Read                                : return _T("ev_Read");
    case ev_Write                               : return _T("ev_Write");
    case ev_ReturnOk                            : return _T("ev_ReturnOk");
    case ev_ReturnError                         : return _T("ev_ReturnError");
    case ev_ReturnSyncOk                        : return _T("ev_ReturnSyncOk");
    case ev_ReturnAsyncOk                       : return _T("ev_ReturnAsyncOk");
    case ev_ReturnWrClient                      : return _T("ev_ReturnWrClient");
    case ev_ReturnWrRead                        : return _T("ev_ReturnWrRead");
    case ev_ReturnWrWrite                       : return _T("ev_ReturnWrWrite");
    case ev_ReturnWrEvent                       : return _T("ev_ReturnWrEvent");
    case ev_ReturnWrClose                       : return _T("ev_ReturnWrClose");
    default                                     : return _T("*** unknown ***");
    }
}

LPCTSTR CSerialPort::ActionValues2t( DWORD e )
{
    static CString str;

    str = _T("");

    if( e & acn_ResetWrWrite )
        str += _T("acn_ResetWrWrite + ");

    if( e & acn_ResetWrRead )
        str += _T("acn_ResetWrRead + ");

    switch(e & ~acn_Mask)
    {
    case ac_None                                : str += _T("ac_None"); break;
    case ac_Error                               : str += _T("ac_Error"); break;
    case ac_Call_StartOpen                      : str += _T("ac_Call_StartOpen"); break;
    case ac_Call_StartClose                     : str += _T("ac_Call_StartClose"); break;
    case ac_Call_StartWait                      : str += _T("ac_Call_StartWait"); break;
    case ac_Call_StartEvent                     : str += _T("ac_Call_StartEvent"); break;
    case ac_Call_StartRead                      : str += _T("ac_Call_StartRead"); break;
    case ac_Call_StartWrite                     : str += _T("ac_Call_StartWrite"); break;
    case ac_Call_GetOverlappedRead              : str += _T("ac_Call_GetOverlappedRead"); break;
    case ac_Call_GetOverlappedWrite             : str += _T("ac_Call_GetOverlappedWrite"); break;
    case ac_Call_GetOverlappedEvent             : str += _T("ac_Call_GetOverlappedEvent"); break;
    case ac_Return_Ok                           : str += _T("ac_Return_Ok"); break;
    case ac_Return_ReadBuffer                   : str += _T("ac_Return_ReadBuffer"); break;
    case ac_Return_Error                        : str += _T("ac_Return_Error"); break;
    case ac_Return_WrClient                     : str += _T("ac_Return_WrClient"); break;
    case ac_Return_WrRead                       : str += _T("ac_Return_WrRead"); break;
    case ac_Return_WrWrite                      : str += _T("ac_Return_WrWrite"); break;
    case ac_Return_WrClose                      : str += _T("ac_Return_WrClose"); break;
    default                                     : str += _T("*** unknown ***"); break;
    }

    return str;
}