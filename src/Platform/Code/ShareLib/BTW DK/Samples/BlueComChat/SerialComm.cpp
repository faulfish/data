// SerialComm.cpp: implementation of the CSerialComm class.
//
//////////////////////////////////////////////////////////////////////
//  Copyright (c) 2000-2004, Broadcom Corporation, All Rights Reserved.

#include "stdafx.h"
#include "BtIfDefinitions.h"
#include "SerialComm.h"
#include "BtIfDefinitions.h"

// Platform types
typedef enum 
{ 
	PT_NONE = 0, 
	PT_WIN95 = 1, 
	PT_WIN98 = 2,
	PT_WINME = 3,
	PT_WINNT = 4, 
	PT_WIN2000 = 5,
	PT_WINXP = 6
} PLATFORM_TYPE;

PLATFORM_TYPE GetPlatformType()
{
    OSVERSIONINFO VI;
    PLATFORM_TYPE eType = PT_NONE;
    
    memset(&VI, 0, sizeof(VI));
    VI.dwOSVersionInfoSize = sizeof(VI);
    
    
    if (GetVersionEx(&VI))
    {
        switch (VI.dwPlatformId)
        {
        case VER_PLATFORM_WIN32_NT:
            if (VI.dwMajorVersion == 5 && VI.dwMinorVersion == 0)
                eType = PT_WIN2000;
            else if ((VI.dwMajorVersion > 5) || (VI.dwMajorVersion == 5 && VI.dwMinorVersion >= 1))
                eType = PT_WINXP;
			else if (VI.dwMajorVersion == 4 && VI.dwMinorVersion == 0)
				eType = PT_WINNT;
            break;
            
        case VER_PLATFORM_WIN32_WINDOWS:
            if (VI.dwMajorVersion == 4 && VI.dwMinorVersion == 10)
                eType = PT_WIN98;
            else if (VI.dwMajorVersion == 4 && VI.dwMinorVersion == 90)
                eType = PT_WINME;
            else
                eType = PT_WIN95;
            
            break;
        default:
            break;    // should not come here
        }
    }
    
    return (eType);
} 


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//DWORD WINAPI CommWatchProc (LPVOID lParam);
#define COMM_PORT_BUFSIZE   4096
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CSerialComm

CSerialComm::CSerialComm()
{
    m_comm_port = 1;
    m_comm_speed = 19200;
    m_use_flow_ctrl = TRUE;
    
    m_hCommDevice = NULL;
    m_bConnected = FALSE;
    m_osRead.Offset = 0;
    m_osRead.OffsetHigh = 0;
    m_osRead.hEvent = NULL;
    m_osWrite.Offset = 0;
    m_osWrite.OffsetHigh = 0;
    m_osWrite.hEvent = NULL;
}

CSerialComm::~CSerialComm()
{
    if (m_bConnected)
        CloseCommPort();
    
    m_bConnected = FALSE;
}


BOOL CSerialComm::OpenCommPort(UINT8 comm_port)
{
    char        buff[20];

    m_comm_port = comm_port;

    PLATFORM_TYPE eType = GetPlatformType();
	if (eType >= PT_WINNT)
//    if ((eType == PT_WIN2000) || (eType == PT_WINNT))
    {
	    sprintf (buff, "\\\\?\\COM%d", m_comm_port);
	}
    else
    {
	    sprintf (buff, "COM%d", m_comm_port);
	}

    // create overlapped event
    m_osRead.hEvent = CreateEvent( NULL,    // no security
        TRUE,    // explicit reset req
        FALSE,   // initial event reset
        NULL ) ; // no name
    
    if (m_osRead.hEvent == NULL)
    {
        return FALSE;
    }
    
    m_osWrite.hEvent = CreateEvent( NULL,    // no security
        TRUE,    // explicit reset req
        FALSE,   // initial event reset
        NULL ) ; // no name
    
    if (NULL == m_osWrite.hEvent)
    {
        CloseHandle( m_osRead.hEvent ) ;
        m_osRead.hEvent = NULL;
        return ( FALSE ) ;
    }
    
    
    // open COMM device
    if ((m_hCommDevice = CreateFile (buff, GENERIC_READ | GENERIC_WRITE,
        0,                    // exclusive access
        NULL,                 // no security attrs
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL |
        FILE_FLAG_OVERLAPPED, // overlapped I/O
        NULL ) ) == INVALID_HANDLE_VALUE)
    {
        CloseHandle( m_osRead.hEvent ) ;
        m_osRead.hEvent = NULL;
        CloseHandle( m_osWrite.hEvent ) ;
        m_osWrite.hEvent = NULL;
        return ( FALSE ) ;
    }
    
    if (!SetupCommPort( ))
        return (FALSE);
    
    m_bConnected = TRUE ;
    
    return (TRUE);
    
} // end of OpenCommPort()

BOOL CSerialComm::SetupCommPort( )
{
    if (!SetupComm (m_hCommDevice, COMM_PORT_BUFSIZE, COMM_PORT_BUFSIZE))
    {
        return FALSE;
    }
    
    m_dcb.DCBlength = sizeof (DCB);
    
    GetCommState (m_hCommDevice, &m_dcb);
    
    m_dcb.BaudRate = m_comm_speed;
    m_dcb.ByteSize = 8;
    m_dcb.StopBits = ONESTOPBIT;
    m_dcb.Parity   = NOPARITY;
    m_dcb.fBinary  = TRUE;
    
    m_dcb.fOutxDsrFlow  = FALSE;
    m_dcb.fDtrControl   = DTR_CONTROL_ENABLE;
    
    m_dcb.fRtsControl   = (m_use_flow_ctrl) ? RTS_CONTROL_HANDSHAKE : RTS_CONTROL_ENABLE;
    m_dcb.fOutxCtsFlow  = m_use_flow_ctrl;
    
    m_dcb.fOutX         = FALSE;
    m_dcb.fInX          = FALSE;
    m_dcb.fAbortOnError = TRUE;
    
    m_dcb.XonLim    = COMM_PORT_BUFSIZE * 2 / 3;
    m_dcb.XoffLim   = COMM_PORT_BUFSIZE / 3;
    
    if (!SetCommState (m_hCommDevice, &m_dcb))
    {
        return FALSE;
    }
    
    if (!SetCommMask (m_hCommDevice, (DWORD)(EV_RXCHAR | EV_CTS | EV_DSR | EV_RING | EV_RLSD)))
    {
        return FALSE;
    }
    
    COMMTIMEOUTS cto;
    memset (&cto, 0, sizeof (COMMTIMEOUTS));
    cto.ReadIntervalTimeout = MAXDWORD;
    
    if (!SetCommTimeouts (m_hCommDevice, &cto))
    {
        return FALSE;
    }
    
    return (TRUE);
    
} // end of SetupCommPort()



BOOL CSerialComm::CloseCommPort( )
{
    if (!m_bConnected)
        return (TRUE);
    
    // set connected flag to FALSE
    
    m_bConnected = FALSE ;
    
    // disable event notification and wait for thread
    // to halt
    
    SetCommMask( m_hCommDevice, 0 ) ;
    
    // drop DTR
    
    EscapeCommFunction( m_hCommDevice, CLRDTR ) ;
    
    // purge any outstanding reads/writes and close device handle
    
    PurgeComm( m_hCommDevice, PURGE_TXABORT | PURGE_RXABORT |
        PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
    CloseHandle( m_hCommDevice ) ;
    m_hCommDevice = NULL;
    
    CloseHandle( m_osRead.hEvent ) ;
    m_osRead.hEvent = NULL;
    
    CloseHandle( m_osWrite.hEvent ) ;
    m_osWrite.hEvent = NULL;
    
    return ( TRUE ) ;
    
} // end of CloseCommPort()


int CSerialComm::ReadData( LPSTR lpszBlock, int nMaxLength )
{
    BOOL       fReadStat ;
    COMSTAT    ComStat ;
    DWORD      dwErrorFlags;
    DWORD      dwLength;
    DWORD      dwError;
    //   char       szError[ 10 ] ;
    
    // only try to read number of bytes in queue
    ClearCommError( m_hCommDevice, &dwErrorFlags, &ComStat ) ;
    dwLength = min( (DWORD) nMaxLength, ComStat.cbInQue ) ;
    
    if (dwLength > 0)
    {
        fReadStat = ReadFile( m_hCommDevice, lpszBlock,
            dwLength, &dwLength, &m_osRead ) ;
        if (!fReadStat)
        {
            if (GetLastError() == ERROR_IO_PENDING)
            {
                TRACE("\n\rIO Pending");
                // We have to wait for read to complete.
                // This function will timeout according to the
                // CommTimeOuts.ReadTotalTimeoutConstant variable
                // Every time it times out, check for port errors
                while(!GetOverlappedResult( m_hCommDevice,
                    &m_osRead, &dwLength, TRUE ))
                {
                    dwError = GetLastError();
                    if(dwError == ERROR_IO_INCOMPLETE)
                        // normal result if not finished
                        continue;
                    else
                    {
                        // an error occurred, try to recover
                        ClearCommError( m_hCommDevice, &dwErrorFlags, &ComStat ) ;
                        break;
                    }
                    
                }
                
            }
            else
            {
                // some other error occurred
                dwLength = 0 ;
                ClearCommError( m_hCommDevice, &dwErrorFlags, &ComStat ) ;
            }
        }
    }
    
    return ( dwLength ) ;
    
} // end of ReadData()

BOOL CSerialComm::WriteData( LPSTR lpByte , DWORD dwBytesToWrite)
{
    
    BOOL        fWriteStat ;
    DWORD       dwBytesWritten ;
    DWORD       dwErrorFlags;
    DWORD   	dwError;
    DWORD       dwBytesSent=0;
    COMSTAT     ComStat;
    char        szError[ 128 ] ;
    
    
    fWriteStat = WriteFile( m_hCommDevice, lpByte, dwBytesToWrite,
        &dwBytesWritten, &m_osWrite ) ;
    
    // Note that normally the code will not execute the following
    // because the driver caches write operations. Small I/O requests
    // (up to several thousand bytes) will normally be accepted
    // immediately and WriteFile will return true even though an
    // overlapped operation was specified
    
    if (!fWriteStat)
    {
        if(GetLastError() == ERROR_IO_PENDING)
        {
            // We should wait for the completion of the write operation
            // so we know if it worked or not
            
            // This is only one way to do this. It might be beneficial to
            // place the write operation in a separate thread
            // so that blocking on completion will not negatively
            // affect the responsiveness of the UI
            
            // If the write takes too long to complete, this
            // function will timeout according to the
            // CommTimeOuts.WriteTotalTimeoutMultiplier variable.
            // This code logs the timeout but does not retry
            // the write.
            
            while(!GetOverlappedResult( m_hCommDevice,
                &m_osWrite, &dwBytesWritten, TRUE ))
            {
                dwError = GetLastError();
                if(dwError == ERROR_IO_INCOMPLETE)
                {
                    // normal result if not finished
                    dwBytesSent += dwBytesWritten;
                    continue;
                }
                else
                {
                    // an error occurred, try to recover
                    ClearCommError( m_hCommDevice, &dwErrorFlags, &ComStat ) ;
                    break;
                }
            }
            
            dwBytesSent += dwBytesWritten;
            
            if( dwBytesSent != dwBytesToWrite )
                wsprintf(szError,"\nProbable Write Timeout: Total of %ld bytes sent", dwBytesSent);
            else
                wsprintf(szError,"\n%ld bytes written", dwBytesSent);
            
#ifdef _DEBUG
            OutputDebugString(szError);
#endif _DEBUG
            
        }
        else
        {
            // some other error occurred
            ClearCommError( m_hCommDevice, &dwErrorFlags, &ComStat ) ;
            return ( FALSE );
        }
    }
    return ( TRUE ) ;
    
} // end of WriteData()



