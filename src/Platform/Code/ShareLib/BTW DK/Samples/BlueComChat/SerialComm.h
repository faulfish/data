// SerialComm.h: interface for the CSerialComm class.
//
//////////////////////////////////////////////////////////////////////
//  Copyright (c) 2000-2004, Broadcom Corporation, All Rights Reserved.

#if !defined(AFX_SERIALCOMM_H__519C327F_A637_4DD1_99E9_BFB13540B7AA__INCLUDED_)
#define AFX_SERIALCOMM_H__519C327F_A637_4DD1_99E9_BFB13540B7AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSerialComm 
{
public:
	UINT8	    m_comm_port;	        // Comm port used
	UINT32	    m_comm_speed;			// Baud rate of Comm port
    BOOL        m_use_flow_ctrl;        // Use flow control

    DCB         m_dcb;

    HANDLE      m_hCommDevice;
    BOOL        m_bConnected;
    OVERLAPPED  m_osRead, m_osWrite;

//    CSerialComm *p_peer_sc;

    char        name[30];

// Construction
	CSerialComm();
	~CSerialComm();
    BOOL OpenCommPort(UINT8 com_port);
	BOOL CloseCommPort();
	BOOL SetupCommPort();
    int ReadData( LPSTR lpszBlock, int nMaxLength );
	BOOL WriteData( LPSTR lpByte , DWORD dwBytesToWrite);
    BOOL IsConnected () { return m_bConnected;  }
    HANDLE GetHandle()  { return m_hCommDevice; }
};


#endif // !defined(AFX_SERIALCOMM_H__519C327F_A637_4DD1_99E9_BFB13540B7AA__INCLUDED_)
