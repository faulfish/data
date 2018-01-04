// SerialPort.h: Schnittstelle für die Klasse CSerialPort.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALPORT_H__7F612934_ABE7_45C6_8EEE_F63AD49A967C__INCLUDED_)
#define AFX_SERIALPORT_H__7F612934_ABE7_45C6_8EEE_F63AD49A967C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Buffer.h"

//========================================================================
// Serial I/O definitions
// The standard serial I/O definitions can be found in WINBASE.H (i.e.
// CBR_xxxx, PCF_xxx, EV_xxx, CE_xxx, etc).
//========================================================================

#define SEVENDATABITS   7       // No equivalents in WINBASE.H
#define EIGHTDATABITS   8
#define PCF_NOFLOWCTL   0

#define ASCII_NUL       0x00    // Some common control codes
#define ASCII_SOH       0x01
#define ASCII_STX       0x02
#define ASCII_ETX       0x03
#define ASCII_EOT       0x04
#define ASCII_ENQ       0x05
#define ASCII_ACK       0x06
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13
#define ASCII_NAK       0x15
#define ASCII_CAN       0x18

#define SP_INBUFSIZE	64*1024

#define SP_OUTBUFSIZE   4096

#define SP_WRITETIMEOUT 0 //120000 // 2 minutes // 0    // Never

#include "TransportConnection.h"
#include "Buffer.h"
#include "TLRequest.h"

class CSerialPort : public CTransportConnection
{
public:
    CSerialPort();
    virtual ~CSerialPort();

public:
    virtual WaitResult_t WaitResult( HANDLE hEventClient );

    virtual HRESULT Open( LPCTSTR szPortName ,int nBoundRate);
    virtual void Close();
    virtual HRESULT OptimizeForPhone( LPCTSTR szPhoneType, LPCTSTR szManufacturer, LPCTSTR szIMEI, LPCTSTR szSoftwareVersion );
    virtual void StartRead( DWORD dwNoOfExpectedBytes );
    virtual HRESULT Write( CBufferPtr spBuffer );
    virtual HRESULT Read( CBufferPtr& spBuffer );

private:
    void InitCommPort();
    bool SetParityDataStop(int nParity = NOPARITY, int nDataBits = EIGHTDATABITS, int nStopBits = ONESTOPBIT);
    bool SetFlowControl(int nFlowCtrl = PCF_DTRDSR | PCF_RTSCTS, int nXOnLimit = 100, int nXOffLimit = 100,
        char chXOnChar = ASCII_XON, char chXOffChar = ASCII_XOFF);
    bool SetBufferSizes(int nInBufSize = SP_INBUFSIZE, int nOutBufSize = SP_OUTBUFSIZE);
    // The default settings 0,0,0 for SetReadTimeouts means: Read until requested no of bytes received
    bool SetReadTimeouts(int nInterval = 0, int nMultiplier = 0, int nConstant = 0);
    bool SetWriteTimeouts(int nMultiplier = 0, int nConstant = SP_WRITETIMEOUT);

private:
    int SetBaudrate( int nBaudrate );
    int GetSlowerBaudrate( int nBaudrate );
    bool PurgeCommPort(DWORD fdwAction = (PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR) );

private:
    enum InternalState_t
    {
        is_Unchanged = 0,
        is_Closed,
        is_Closing,
        is_ROvl_WNon,
        is_RNfd_WNon,
        is_RNon_WNon,
        is_ROvl_WRdy,
        is_ROvl_WOvl,
        is_RNon_WOvl,
        is_RNon_WRdy,
        is_RNfd_WOvl,
        is_RNfd_WRdy,
        is_RNon_WRdy_StartRead,
        is_RNon_WNon_Wait_StartWait,
        is_RNon_WNon_Wait_GetOverlappedEvent,
        is_RNon_WNon_Wait_StartEvent,

        is_Closed_Open_StartOpen,
        is_Closed_Open_StartEvent,
        is_RNon_WNon_StartRead,
        is_ROvl_WNon_Wait_StartWait,
        is_ROvl_WNon_Wait_GetOverlappedRead,
        is_ROvl_WNon_Wait_GetOverlappedEvent,
        is_ROvl_WNon_Wait_StartEvent,
        is_ROvl_WNon_Write_StartWrite,
        is_RNon_WNon_Write_StartWrite,
        is_ROvl_WOvl_Wait_StartWait,
        is_ROvl_WOvl_Wait_GetOverlappedRead,
        is_ROvl_WOvl_Wait_GetOverlappedWrite,
        is_ROvl_WOvl_Wait_GetOverlappedEvent,
        is_RNon_WOvl_StartRead,
        is_ROvl_WOvl_Wait_StartEvent,
        is_RNon_WOvl_Wait_StartWait,
        is_RNon_WOvl_Wait_GetOverlappedWrite,
        is_RNon_WOvl_Wait_GetOverlappedEvent,
        is_RNon_WOvl_Wait_StartEvent,
        is_RRdy_WNon,
        is_RRdy_WNon_Write_StartWrite,
        is_RRdy_WRdy,
        is_RRdy_WOvl,

        is_Failure,
        is_ALL,
    };

    enum EventValue_t
    {
        ev_Undefined = 0,
        ev_Open,                // Open function called
        ev_Close,               // Close function called
        ev_Wait,                // Wait function called
        ev_StartRead,           // StartRead function called
        ev_Read,                // Read function called
        ev_Write,               // Write function called
        ev_ReturnOk,            // Internal function returned ok
        ev_ReturnError,         // Internal function returned error
        ev_ReturnSyncOk,        // Internal function succeeded synchronously
        ev_ReturnAsyncOk,       // Internal function succeeded asynchronously
        ev_ReturnWrClient,      // Client event signaled
        ev_ReturnWrRead,        // Read overlapped event signaled
        ev_ReturnWrWrite,       // Write overlapped event signaled
        ev_ReturnWrEvent,       // GetCommEvent overlapped event signaled
        ev_ReturnWrClose,       // Closed
        ev_CommEventClosed,
        ev_CommEventOther,
    };

    enum ActionValue_t
    {
        ac_None                         = 0,
        ac_Error                        = 1,
        ac_Call_StartOpen               = 2,
        ac_Call_StartClose              = 3,
        ac_Call_StartWait               = 4,
        ac_Call_StartEvent              = 5,
        ac_Call_StartRead               = 6,
        ac_Call_StartWrite              = 7,
        ac_Call_GetOverlappedRead       = 8,
        ac_Call_GetOverlappedWrite      = 9,
        ac_Call_GetOverlappedEvent      = 10,
        ac_Return_Ok                    = 11,
        ac_Return_ReadBuffer            = 12,
        ac_Return_Error                 = 13,
        ac_Return_WrClient              = 14,
        ac_Return_WrRead                = 15,
        ac_Return_WrWrite               = 16,
        ac_Return_WrClose               = 17,

        // actions that may be specified additionally
        acn_Mask                        = 0xFFFF0000,
        acn_ResetWrWrite                = 1 << 16,
        acn_ResetWrRead                 = 1 << 17,
    };

    LPCTSTR InternalState2t( InternalState_t e );
    LPCTSTR EventValue2t( EventValue_t e );
    LPCTSTR ActionValues2t( DWORD e );

    struct FSMTableEntry
    {
        InternalState_t eInitialState;
        EventValue_t    eEventValue;
        InternalState_t eTargetState;
        DWORD           eActionValue;
    };

    class CEventParams
    {
    public:
        CEventParams() { m_hEventClient = NULL; m_dwNoOfExpectedBytes = 0;}
        virtual ~CEventParams() {}

        HANDLE GetEventClient() const { return m_hEventClient; }
        void SetEventClient( HANDLE hEventClient ) { m_hEventClient = hEventClient; }
        LPCTSTR GetPort() const { return m_strPort; }
        void SetPort( LPCTSTR szPort ,int nBoundRate ) { m_strPort = szPort; m_nBoundRate = nBoundRate;}
		int GetBoundRate() const { return m_nBoundRate; }
        CBufferPtr GetReadBuffer() const { return m_spReadBuffer; }
        void SetReadBuffer( CBufferPtr spReadBuffer ) { m_spReadBuffer = spReadBuffer; }
        CBufferPtr GetWriteBuffer() const { return m_spWriteBuffer; }
        void SetWriteBuffer( CBufferPtr spWriteBuffer ) { m_spWriteBuffer = spWriteBuffer; }
        DWORD GetNoOfExpectedBytes() const { return m_dwNoOfExpectedBytes; }
        void SetNoOfExpectedBytes( DWORD dwNoOfExpectedBytes ) { m_dwNoOfExpectedBytes = dwNoOfExpectedBytes; }
    private:
        HANDLE m_hEventClient;
        CString m_strPort;
        CBufferPtr m_spReadBuffer;
        CBufferPtr m_spWriteBuffer;
        DWORD m_dwNoOfExpectedBytes;
		int m_nBoundRate;
    };

private:
    EventValue_t ExecuteFSM( EventValue_t eInputEventValue, CEventParams& EventParams );
    EventValue_t StartWait( CEventParams& EventParams );
    EventValue_t StartRead( CEventParams& EventParams );
    EventValue_t StartWrite( CEventParams& EventParams );
    EventValue_t StartEvent( CEventParams& EventParams );
    EventValue_t GetOverlappedRead( CEventParams& EventParams );
    EventValue_t GetOverlappedWrite( CEventParams& EventParams );
    EventValue_t GetOverlappedEvent( CEventParams& EventParams );
    EventValue_t StartOpen( CEventParams& EventParams );
    EventValue_t StartClose( CEventParams& EventParams );
    void ResetWrWrite( CEventParams& EventParams );
    void ResetWrRead( CEventParams& EventParams );

private:
    InternalState_t m_eState;
    static FSMTableEntry fsmTable[];

    CString m_strPortName;
    CBufferPtr m_spReadBuffer;
    CBufferPtr m_spWriteBuffer;
    DWORD m_dwEventCommMask;
    bool m_bOverlappedReadActive;
    bool m_bOverlappedWriteActive;
    HANDLE m_hPortId;
    OVERLAPPED	m_overlappedRead;
    OVERLAPPED m_overlappedWrite;
    OVERLAPPED m_overlappedEvent;
    DWORD m_dwBytesToRead; // in one block
    DWORD m_dwNoOfBytesWritten; // of m_spWriteBuffer
    CEvent m_eventRead;
    CEvent m_eventWrite;
    CEvent m_eventEvent;
};

#endif // !defined(AFX_SERIALPORT_H__7F612934_ABE7_45C6_8EEE_F63AD49A967C__INCLUDED_)


