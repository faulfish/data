// SerialIO.cpp: implementation of the CSerialIO class.
// 2004/10/11
// Change std::string to character string
//////////////////////////////////////////////////////////////////////
#define STRICT

#include "stdafx.h"
#include <crtdbg.h>
#include <tchar.h>
#include <stdio.h>
#include "SerialIO.h"
#include "..\..\CommonUtil\LogPolicy.h"

#define READ_BUFFER_LEN	100
#define RF_ANALOG_MODE	0
#define CMDIAG_MODE_OFFLINE_A_F  0	/* Go to offline analog */
#define CMDIAG_MODE_OFFLINE_D_F	 1   /* Go to offline digital */
#define DIAG_OFFLINE_DIGITAL 2
#define DIAG_OFFLINE_ANALOG  6
#define MODE_DETECTION_RETY_TIMES 20
#define MODE_DETECTION_DELAY	1000
#define SERIAL_QUEUE_SIZE		4096


CSerialIO::CSerialIO() : m_lLastError(ERROR_SUCCESS),
						 m_hFile(NULL),
						 m_hWriteEvent(NULL),
						 m_hWaitEvent(NULL),
						 m_hModeEvent(NULL),
						 m_hStopEvent(NULL),
						 m_fStopping(FALSE),
						 m_bIsCOMReopen(FALSE),
						 m_bIsReOpen(FALSE),
						 m_hThread(NULL),
						 m_hReadEvent(NULL),
						 m_eState(EStop),
						 m_bDestroy(FALSE),
						 m_dwBufStart(0),
						 m_dwBufEnd(0),
						 m_dwCOMNum(0),
						 m_dwCurWriteCount(0),
						 m_nCurrentSpeedTry(0),
						 m_bIsPreventOuterRead(FALSE),
						 m_bIsPreventOuterWrite(FALSE),
						 m_ePreferBaudRate(EBaud115200),
						 m_dwBufSize(RECV_BUFFER_SIZE),
						 m_bIsSetProbeMode(false),
						 m_bIsRequestFunction(false)
{
	m_pbRecvBuf = new BYTE [m_dwBufSize];
	ZeroMemory(m_pbRecvBuf, m_dwBufSize);
	ZeroMemory(m_szDevice, sizeof(TCHAR) * COM_PORT_NAME_LENGTH);
	ZeroMemory(m_szCOM, sizeof(TCHAR) * COM_PORT_NAME_LENGTH);
	m_TestSpeed[0]= EBaud115200;
	m_TestSpeed[1]= EBaud230400;
	m_TestSpeed[2]= EBaud460800;
}

CSerialIO::~CSerialIO()
{
	m_bDestroy = TRUE;
	StopListener(INFINITE);

	InternalClose();
	_ASSERTE(m_pbRecvBuf != NULL);
	delete[] m_pbRecvBuf;
	m_pbRecvBuf = NULL;
}

LONG CSerialIO::Open(LPCTSTR lpszDevice, EBaudrate eBaudrate,bool bRequestFunction)
{
	LONG lRet = ERROR_SUCCESS;
	m_bIsRequestFunction = bRequestFunction;

	_ASSERTE(lpszDevice != NULL);
	if(m_hFile)
		m_lLastError = ERROR_ALREADY_INITIALIZED;
	else
	{
		ZeroMemory(m_szDevice, sizeof(TCHAR) * COM_PORT_NAME_LENGTH);
		ZeroMemory(m_szCOM, sizeof(TCHAR) * COM_PORT_NAME_LENGTH);

		m_dwCOMNum = GetCOMNum(lpszDevice);
		_stprintf(m_szCOM, _T("COM%d"),m_dwCOMNum);
		if ( m_dwCOMNum > 9 )// COM port number larger than 9, add prefix string.
			_stprintf(m_szDevice, _T("\\\\.\\COM%d"),m_dwCOMNum);
		else	
			_stprintf(m_szDevice, _T("COM%d"),m_dwCOMNum);
	
		m_eBaudrate = eBaudrate;
		if(IsCOMPortAlive(m_dwCOMNum) == TRUE)
		{
			lRet = InternalOpen();
			if(lRet == ERROR_SUCCESS)
				StartListener();
		}
		else
		{
			lRet = RUN_FAILED;
		}
		if(lRet != ERROR_SUCCESS)
			m_lLastError = lRet;
	}
	return lRet;
}

LONG CSerialIO::Close(void)
{
	LONG lRet = ERROR_SUCCESS;
	
	StopListener();
	if(m_hFile != 0)
	{
		if ( InternalClose() != ERROR_SUCCESS )
		{
			AddLog(_T("internal close failed."));
			lRet = RUN_FAILED;
		} 
		else 
		{
			lRet = RUN_SUCCESSFULLY;
		}
	}
	m_lLastError = ERROR_SUCCESS;

	return lRet;
}

// 20050223 Embert Tsai
// Add this method for Ulysses download
int CSerialIO::ReOpen(){
	TRACE("CSerialIO::ReOpen()");
	int nRet = RUN_FAILED;

	m_bIsReOpen = TRUE;
	StopListener();
	Sleep(500);	
	m_ePreferBaudRate = m_eBaudrate;
	m_nCurrentSpeedTry = GetSpeedIndex(m_ePreferBaudRate);
	int i=1;
	for (i = 1 ; i< 10 ; i+=i){
		if ( InternalOpen() != ERROR_SUCCESS )
			Sleep(i * 1000);	// 2004/09/21 Embert Tsai. Give mobile and driver more time to change to download mode.
		else 
			break;
	}
	m_bIsReOpen = FALSE;
	if ( i <= 10 )
	{	
		nRet = RUN_SUCCESSFULLY;
	}

	return nRet;
}

LONG CSerialIO::StartListener (void)
{
	if (m_hThread == 0)
	{
		_ASSERTE(!m_fStopping);
		DWORD dwThreadId = 0;
		m_hThread = ::CreateThread(0,0,ThreadProc,LPVOID(this),0,&dwThreadId);
		if (m_hThread == 0)
		{
			m_lLastError = ::GetLastError();
		}
		// add code to wait for create create complete
	} 
	else 
	{
		m_lLastError = ERROR_SUCCESS;
	}
	return m_lLastError;
}

LONG CSerialIO::StopListener (DWORD dwTimeout)
{
	if (m_hThread)
	{
		SetEvent(m_hStopEvent);
		Sleep(0);
		SetEvent(m_hReadEvent);
		::WaitForSingleObject(m_hThread,dwTimeout);
		m_fStopping = FALSE;
		::CloseHandle(m_hThread);
		m_hThread = 0;

		ResetMode();
	} 

	m_lLastError = ERROR_SUCCESS;
	return m_lLastError;
}

BOOL CSerialIO::SendProbe(void)
{
	AddLog(_T("SendProbe\n"));

	m_bIsSetProbeMode = true;

	BOOL bRes = FALSE;
	UCHAR szBuffer[10];
	DWORD dwWrited = 0;
	szBuffer[0] = 0x61;
	szBuffer[1] = 0x74;
	szBuffer[2] = 0x0d;
	szBuffer[3] = 0x1c;
	szBuffer[4] = 0xd5;
	szBuffer[5] = 0x7e;
	// 20040921 Embert Tsai. The following command is download ack command.
	szBuffer[6] = 0x06;
	szBuffer[7] = 0x4E;
	szBuffer[8] = 0x95;
	szBuffer[9] = 0x7E;

	if( InternalWrite(szBuffer, 10, &dwWrited) != ERROR_SUCCESS || dwWrited != 10 )
	{	
		AddLog(_T("SendProbe: Write detection packet to com port failed.\n"));
	}
	else
	{
		bRes = TRUE;
	}
	return bRes;
}

DWORD WINAPI CSerialIO::ThreadProc (LPVOID lpArg)
{
	CSerialIO* pThis = reinterpret_cast<CSerialIO*>(lpArg);
	return pThis->ThreadProc();
}

DWORD CSerialIO::ThreadProc(void)
{
	char szBuffer[4096] = {0};
	OVERLAPPED ovInternal = {0};
	DWORD dwRead = 0;
	BOOL bContinueRecv = FALSE;

	ResetMode();
	if(m_bDestroy != TRUE)
	{
		OnEvent(EOnStart);
	}
	do
	{
		if(m_eState == EStart)  
		{
			if( (bContinueRecv == FALSE) && m_bIsCOMReopen)
			{ //close and restart continously
				InternalClose();
				::Sleep(100);	// Added by Embert to let driver have more time to start up

				if(InternalOpen() != ERROR_SUCCESS)
				{
					::Sleep(500);
					continue;
				};
			}
			ovInternal.hEvent = m_hReadEvent;

			if(m_bIsRequestFunction == false)
			{
				Sleep(100); // Let driver have enough time to response.
				SendProbe();
			}
		}

		if (FALSE == ::ReadFile(m_hFile, szBuffer, 4096, &dwRead, &ovInternal))
		{
			long lLastError = ::GetLastError();
			if (lLastError != ERROR_IO_PENDING)
			{
				TRACE("COMM port disconnected\n");
				bContinueRecv = NotErrorIOPending(lLastError, bContinueRecv);// COMM port disconnected.
			}
			else
			{
				bContinueRecv = ErrorIOPending(ovInternal, dwRead, szBuffer, bContinueRecv);
			}
		}
		else
		{
			bContinueRecv = ErrorIOPending(ovInternal, dwRead, szBuffer, bContinueRecv);
		}
	}
	while (WaitForSingleObject(m_hStopEvent, 0) == WAIT_TIMEOUT);
	AddLog(_T("ThreadProc: InternalClose"));
	InternalClose();

	if ( !m_bIsReOpen)
		m_eState = EStop;
	if(m_bDestroy != TRUE && !m_bIsReOpen)
	{
		TRACE("ThreadProc: OnStop Notify\n");
		OnEvent(EOnStop);
	}
	// Bye bye
	return 0;
}
BOOL CSerialIO::ErrorIOPending( OVERLAPPED &ovInternal, DWORD dwRead, char * szBuffer, BOOL bContinueRecv )
{
	Sleep(0);
	HANDLE hEvents[2] = {ovInternal.hEvent, m_hStopEvent};
	//switch (::WaitForSingleObject(ovInternal.hEvent,DEFAULT_CMD_TIME_OUT))
	switch(::WaitForMultipleObjects(2, hEvents, FALSE, INFINITE))
	{
	case WAIT_OBJECT_0 + 0:
		if (!::GetOverlappedResult(m_hFile,&ovInternal,&dwRead,FALSE))
		{
			m_lLastError = ::GetLastError();
		}
		else
		{
			if(dwRead != 0)
			{
				AddLog(_T("Get data"));
				LogBinary(szBuffer,dwRead);

				{
					AutoProtect protect(m_csRead);
					AppendBuffer(szBuffer,dwRead);
				}
				if(m_bDestroy != TRUE)
				{
					if ( !m_bIsPreventOuterRead )
					{
						OnEvent(EOnRecv);
					}
				}
			}
		}
		break;

	case WAIT_OBJECT_0 + 1:
		::CancelIo(m_hFile);
		m_lLastError = ERROR_TIMEOUT;
		SetEvent(m_hStopEvent);
		break;

	default:
		_ASSERTE(FALSE);
		m_lLastError = ::GetLastError();
		SetEvent(m_hStopEvent);
		break;
	};
	return bContinueRecv;
}
BOOL CSerialIO::DetectMode( DWORD dwRead, char * szBuffer)
{
	bool bRes = false;
	if( dwRead >= sizeof(DETECT_DS_RSP)-1 &&
		memcmp(szBuffer, DETECT_DS_RSP, sizeof(DETECT_DS_RSP)-1) == 0)
	{
		m_eState = EDS;
		if(m_bDestroy != TRUE)
		{
			OnEvent(EOnConnectDS);
		}
		bRes = true;
	}
	else if( (dwRead >= sizeof(DETECT_DIAG_RSP_5105)-1 && memcmp(szBuffer, DETECT_DIAG_RSP_5105, sizeof(DETECT_DIAG_RSP_5105)-1) == 0) ||
		(dwRead >= sizeof(DETECT_DIAG_RSP_6250)-1 && memcmp(szBuffer, DETECT_DIAG_RSP_6250, sizeof(DETECT_DIAG_RSP_6250)-1) == 0) )
	{
		m_eState = EDIAG;
		if(m_bDestroy != TRUE)
		{
			OnEvent(EOnConnectDIAG);
			TRACE("Mode:Diag\n");
		}
		bRes = true;
	}
	else if ((dwRead >= sizeof(DETECT_DL)-1 && memcmp(szBuffer, DETECT_DL, sizeof(DETECT_DL)-1) == 0) ||
			 (dwRead >= sizeof(DETECT_DL2)-1 && memcmp(szBuffer, DETECT_DL2, sizeof(DETECT_DL2)-1) == 0))
	{
		m_eState = EDL;
		if(m_bDestroy != TRUE)
		{
			OnEvent(EOnConnectDL);
			TRACE("Mode:DL\n");			
		}
		bRes = true;
	}
	else
	{
		AddLog(_T("Mode:can't determined\n"));
	}	
	if(bRes)
		SetEvent(m_hModeEvent);

	return bRes;
}
BOOL CSerialIO::NotErrorIOPending(long lLastError, BOOL bContinueRecv )
{
	if(m_eState != EStart && m_bDestroy != TRUE)
	{
		ResetMode();
		OnEvent(EOnStart);
	}
	m_lLastError = lLastError;
	bContinueRecv = FALSE;
	Sleep(500);	
	return bContinueRecv;
}
// 20050215 Embert Tsai
// This function is used to detect com port status by searching registry key.
BOOL CSerialIO::IsCOMPortAlive(long nComPort)
{
	if(nComPort == -1)
		nComPort = m_dwCOMNum;
	BOOL bRes = FALSE;

	TCHAR szValueName[128] = _T("");
	TCHAR szValueData[128] = _T("");
	DWORD retCode = 0;
	int i = 0;
	HKEY   hkEnum=NULL;
	ULONG ulValueNameLength = 128, ulValueDataLength = 128;
	int nCurrentPortNum = 0;

	retCode = RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("HARDWARE\\DEVICEMAP\\SERIALCOMM"),0,KEY_QUERY_VALUE,&hkEnum);

	for (i = 0; ; i++)
	{
		ulValueNameLength = 128;
		ulValueDataLength = 128;
		retCode = RegEnumValue(hkEnum, i, szValueName, &ulValueNameLength, NULL, NULL, (unsigned char *)szValueData, &ulValueDataLength);
		if(retCode != ERROR_SUCCESS)
		{
			break;
		}

		if ( GetCOMNum(szValueData) == (int)nComPort )
		{
			// found the COM port
			bRes = TRUE;
		}
	}

	return bRes;
}

LONG CSerialIO::InternalOpen()
{
	AutoProtect protect(m_csWrite);

	bool bRes = false;
	AddLog(_T("CSerialIO::InternalOpen"));
	m_lLastError = ERROR_SUCCESS;

	m_eBaudrate = m_TestSpeed[m_nCurrentSpeedTry++];

	if ( m_nCurrentSpeedTry == SPEED_ARRAY_SIZE )
		m_nCurrentSpeedTry = 0;

	if (m_hFile)
	{
		m_lLastError = ERROR_ALREADY_INITIALIZED;
		AddLog(_T("COM port has been opened."));
		return m_lLastError;
	}

	int nRetry = 0;
	do
	{
		AddLog(m_szDevice);
		m_hFile = ::CreateFile(m_szDevice,
			GENERIC_READ|GENERIC_WRITE|MAXIMUM_ALLOWED,
			0,
			0,
			OPEN_EXISTING,
			FILE_FLAG_OVERLAPPED,
			0);
		char szhFile[100] = {0};
		sprintf(szhFile,"%x", m_hFile);
		//AddLog(szhFile);

		if(m_hFile == INVALID_HANDLE_VALUE)
		{
			m_lLastError = ::GetLastError();
			AddLog(_T("Fails in opening COM port."));

			if(++nRetry > 13)
			{
				AddLog(_T("++nRetry > 13"));
				break;
			}

			Sleep(nRetry*1000);
		}
		else
		{
			m_lLastError = InternalResetCOM();
			if ( ERROR_SUCCESS != m_lLastError)
			{
				AddLog(_T("InternalResetCOM fail."));
			}
			else
				bRes = true;
			break;
		}
	}
	while(m_hFile == INVALID_HANDLE_VALUE);

	if(bRes)
	{
		// create overlapped event
		m_hWriteEvent = ::CreateEvent(0, FALSE, FALSE, 0);
		if (m_hWriteEvent == 0)
		{
			m_lLastError = ::GetLastError();
			AddLog(_T("::CreateEvent Write event failed."));
		}

		m_hReadEvent = ::CreateEvent(0, FALSE, FALSE, 0);
		if (m_hReadEvent == 0)
		{
			m_lLastError = ::GetLastError();
			AddLog(_T("Create read event failed."));
		}

		if(!m_hStopEvent)
			m_hStopEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);	
		if (m_hStopEvent == 0)
		{
			m_lLastError = ::GetLastError();
			AddLog(_T("Create m_hStopEvent event failed."));
		}
		else
			ResetEvent(m_hStopEvent); //unsignal

		if(!m_hModeEvent)
			m_hModeEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		if (m_hModeEvent == 0)
		{
			m_lLastError = ::GetLastError();
			AddLog(_T("Create m_hModeEvent event failed."));
		}
		else
			ResetMode();		

		if(m_lLastError = ERROR_SUCCESS)
		{
			m_lLastError = ::GetLastError();
			m_hFile = NULL;
		}
	}
	else
		AddLog(_T("InternalOpen() fail"));

	return m_lLastError;
}

LONG CSerialIO::InternalClose(void)
{
	AutoProtect protect(m_csWrite);

	m_lLastError = ERROR_SUCCESS;
	if (m_hFile == 0)
	{
		return m_lLastError;
	}

	if ( !::CloseHandle(m_hFile) ){
		m_lLastError = GetLastError();
	}

	m_hFile = 0;

	if (m_hWriteEvent)
	{
		if ( ! ::CloseHandle(m_hWriteEvent) ){
			m_lLastError = GetLastError();
			return m_lLastError;
		}
		m_hWriteEvent = 0;
	}

	if (m_hReadEvent)
	{
		if ( !::CloseHandle(m_hReadEvent) ){
			m_lLastError = GetLastError();
			return m_lLastError;
		}
		m_hReadEvent = 0;
	}

	if (m_hStopEvent)
	{
		if ( !::CloseHandle(m_hStopEvent) ){
			m_lLastError = GetLastError();
			return m_lLastError;
		}
		m_hStopEvent = 0;
	}

	if(m_hModeEvent)
	{
		if ( !::CloseHandle(m_hModeEvent) )
		{
			m_lLastError = GetLastError();
			return m_lLastError;
		}
		m_hModeEvent = 0;
	}

	return m_lLastError;
}


LONG CSerialIO::Write(const void* pData, size_t iLen, DWORD* pdwWritten)
{
	//AddLog(_T("CSerialIO::Write"));

	_ASSERTE( pData != NULL );
	_ASSERTE( iLen > 0 );
	if ( m_bIsPreventOuterWrite)	//Prevent outer class to write packet
	{		
		AddLog(_T("CSerialIO::Write ==> Prevent outer class to write packet"));
		return -1;
	}

	::InterlockedIncrement(&m_dwCurWriteCount);

	long lRet = 0;
	lRet = InternalWrite(pData, iLen, pdwWritten);

	::InterlockedDecrement(&m_dwCurWriteCount);
	return lRet;
}

LONG CSerialIO::Read(void* pData, size_t iLen, DWORD* pdwRead)
{
	_ASSERTE(pData != NULL);
	_ASSERTE(iLen > 0);
	m_lLastError = ERROR_SUCCESS;

	DWORD dwRead;
	if (pdwRead == 0)
	{
		pdwRead = &dwRead;
	}
	*pdwRead = 0;

	if (m_hFile == 0)
	{
		m_lLastError = ERROR_INVALID_HANDLE;
		return m_lLastError;
	}

#ifdef _DEBUG
	//memset(pData,0xDC,iLen);
	FillMemory(pData, iLen, 0xDC);
#endif
	{
		AutoProtect protect(m_csRead);
		*pdwRead = ReadBuffer(pData,iLen);
	}

	return m_lLastError;
}

BOOL CSerialIO::ChangeToDSMode(void)
{
	switch ( m_eState ){
	case EDL:
		return FALSE;
	case EDIAG:
		return ChangeDIAG2DS();
	case EDS:
		return TRUE;
	}
	_ASSERTE(FALSE);
	return FALSE;
}

bool CSerialIO::MakesureToDIAGMode(void)
{	
	bool bRes = false;
	for(int nTimes =0; nTimes< MODE_DETECTION_RETY_TIMES ; nTimes++)
	{
		if(m_eState == EDIAG)
		{
			bRes = true;
			break;
		}
		else
		{
			if(m_eState == EDS)
				bRes = ChangeToDIAGMode() == TRUE;
			else
			{
				//MakesureCOMPortReadyToUse();					
				WaitForSingleObject(m_hModeEvent, 6000);
				if(m_eState != EDIAG)
				{
					SendProbe();
				}
			}
		}
	}
	return bRes;
}
bool CSerialIO::IsDLorDIAGMode()
{
	bool bRes = false;
	for(int nTimes =0; nTimes< MODE_DETECTION_RETY_TIMES ; nTimes++)
	{
		if(m_eState == EDIAG || m_eState == EDL)
		{
			AddLog(_T("CSerialIO::IsDLorDIAGMode - is EDiag or EDL"));
			bRes = true;
			break;
		}
		else
		{
			AddLog(_T("CSerialIO::IsDLorDIAGMode - Not EDiag || EDL"));
			if(m_eState == EDS)
				bRes = ChangeToDIAGMode() == TRUE;
			else
			{
				//MakesureCOMPortReadyToUse();					
				WaitForSingleObject(m_hModeEvent, 10000);
				if(m_eState == EDIAG || m_eState == EDL)
				{
					bRes = true;
					break;					
				}
				else
				{
					SendProbe();
				}
			}
		}
	}
	return bRes;
}


BOOL CSerialIO::ChangeToDIAGMode(void)
{
	switch ( m_eState ){
	case EDL:
		return FALSE;
	case EDIAG:
		return TRUE;
	case EDS:
		return ChangeDS2DIAG();
	default:
		_ASSERTE(FALSE);
		return FALSE;
	}
	_ASSERTE(FALSE);
	return FALSE;
}

BOOL CSerialIO::ChangeToDLMode(void){
	TRACE("CSerialIO::ChangeToDLMode\n");
	switch ( m_eState ) {
	case EDL:
		return TRUE;
	case EDIAG:
		return ChangeDIAG2DL();
	case EDS:
		return ChangeDS2DL();
	default:
		_ASSERTE(FALSE);
		return FALSE;
	}
}


CSerialIO::EBaudrate CSerialIO::GetBaudrate()
{
	if ( m_eState != EDIAG && m_eState != EDL  && m_eState != EDS ) 
		return EBaudUnknown;

	m_lLastError = ERROR_SUCCESS;
	if (m_hFile == 0)
	{
		m_lLastError = ERROR_INVALID_HANDLE;
		return EBaudUnknown;
	}
	DCB dcb;
	dcb.DCBlength = sizeof (DCB);
	if (!::GetCommState(m_hFile,&dcb))
	{
		m_lLastError = ::GetLastError();
		return EBaudUnknown;
	}
	return EBaudrate(dcb.BaudRate);
}



// 20040913 Change DS mode to DIAG mode
BOOL CSerialIO::ChangeDS2DIAG(void){
	
	if(!InternalWrite("at$qcdmg\r", strlen("at$qcdmg\r")))
	{
		StopListener();
		StartListener();
		int i = 0;
		while( (i++) < 100 )
		{
			switch(m_eState)
			{
			case EStop:
			case EStart:
				break;
			case EDS:
				return FALSE;
			case EDIAG:
				return TRUE;
			case EDL:
				return FALSE;
			}
			Sleep(200);
		}
		return FALSE;
	}

	return FALSE;

}

// Change DIAG mode to DS mode
BOOL CSerialIO::ChangeDIAG2DS(void){

	if(!InternalWrite("\x1f\xff\x00\x5e\xf6\x7e", sizeof("\x1f\xff\x00\x5e\xf6\x7e")-1) &&
	   !InternalWrite("\x73\x00\x00\x00\x00\x00\x00\x00\xda\x81\x7e", sizeof("\x73\x00\x00\x00\x00\x00\x00\x00\xda\x81\x7e")-1) &&
	   !InternalWrite("\x44\x58\xf4\x7e", sizeof("\x44\x58\xf4\x7e")-1) )
	{
		StopListener();
		StartListener();
		int i = 0;
		while((i++)<100)
		{
			switch(m_eState)
			{
			case EStop:
			case EStart:
				break;
			case EDS:
				return TRUE;
			case EDIAG:
				return FALSE;
			case EDL:
				return FALSE;
			}
			Sleep(200);
		}
		return FALSE;
	}
	return FALSE;

}

////////////////////////////////////////////////////////////////////////
// 20040916 Embert Tsai
// This is not allowed in current situation. (5150 and 6250)
///////////////////////////////////////////////////////////////////////
BOOL CSerialIO::ChangeDL2DIAG(void){
	return FALSE;
}

////////////////////////////////////////////////////////////////////////
// 20040916 Embert Tsai
// 1. First change mobile to offline.
// 2. Send DIAG cmd to change to DL mode.
///////////////////////////////////////////////////////////////////////
BOOL CSerialIO::ChangeDIAG2DL(void){

	_ASSERTE(m_eState == EDIAG);
	BOOL bRes = FALSE;
	CSerialIO::EPlatform EPlatform;
	//EPlatform = MakeSureGetPlatformID();
	EPlatform = CSerialIO::E_PLAT_6280;
	switch(EPlatform)
	{
		case (CSerialIO::E_PLAT_6280):
			//bRes = InternalChange2DL6280();
			bRes = DIAG2DL();
			if(!bRes && GetState()==EDIAG)
			{
				bRes = DIAG2DL();
			}
			break;
		case(CSerialIO::E_PLAT_DEFAULT):
			bRes = InternalChange2DLDefault();
			break;
		default:
			_ASSERTE(FALSE);
	}
	return bRes;
}

CSerialIO::EPlatform CSerialIO::MakeSureGetPlatformID()
{
	CSerialIO::EPlatform EPlatform= E_PLAT_NOT_FOUND;
	for(int i=0; i<MODE_DETECTION_RETY_TIMES && EPlatform == E_PLAT_NOT_FOUND; i++)
		EPlatform = GetPlatformID();
	return EPlatform;
}

BOOL CSerialIO::ChangeDS2DL(void){
	BOOL bRet = FALSE;
	if ( ! ChangeDS2DIAG() ){
		return FALSE;
	}
	if ( ! ChangeDIAG2DL() ) {
		return FALSE;
	}
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////
/*************************************************************************
 * Purpose: Change phone to offline mode.
 * Input  : NONE
 * Return : TRUE -->run succesfully.
 *			FALSE  -->error.
 * Exception: This function is called for Domingo model. Although it does not 
			  Work in Ulysses mode. But it does not have other side effect.
 * Note	  : 2004/09/21 first version created by Embert
 ***************************************************************************/
////////////////////////////////////////////////////////////////////////////
BOOL CSerialIO::ChangeToOffline(){

	_ASSERTE(m_eState == EDIAG);
	DWORD dwWriteByte	=	0;
	DWORD dwReadByte	=	0;
	DWORD wEntryReason	=	0;
	int nTestCount = 20;
	
	// First detect phone status
	BYTE pbCMDStatus[4] = {0x0C,0x14,0x3A,0x7E};
	if ( (InternalWrite(pbCMDStatus,4, &dwReadByte) != ERROR_SUCCESS) || (dwReadByte != 4) ){
		return FALSE;
	}
	
	BYTE pbCMDRsp[READ_BUFFER_LEN];
	ZeroMemory(pbCMDRsp,READ_BUFFER_LEN);
	//while ( m_strRecv.length() == 0 ) ;	// Waiting for Reading buffer completion
	//Sleep(100);
    // 20050118 Embert Tsai
	if ( WaitandReadBuffer(pbCMDRsp, READ_BUFFER_LEN, 51, &dwReadByte) != ERROR_SUCCESS ){
		return FALSE;
	}
	if ( pbCMDRsp[0] != 0x0C ){
		return TRUE;	// It is a very tricky. 
	}
	
	WORD wRFMode = 0;
	BYTE pbCMDOffline[6];
	ZeroMemory(pbCMDOffline, 6);
	// Get RF mode
	int nReadCount = 0;
	int nIndex = 0;
	int nRFIndex = 0;
	int nEntryIndex = 0;
	while (TRUE){
		if ( pbCMDRsp[nIndex] != 0x7D){
			nIndex++;
			nReadCount++;
		}else{
			nIndex++;
		}
		if ( 8 == nReadCount ){		//Original RF mode value position.
			nRFIndex = nIndex;
			break;
		}
	}

	CopyMemory( &wRFMode, pbCMDRsp + nRFIndex , 2); 
	if ( wRFMode == RF_ANALOG_MODE ){ /* Go to offline analog */
		// The following code should be fixed to meet analog mode.
		pbCMDOffline[0]=0x29;
		pbCMDOffline[1]=0x01;
		pbCMDOffline[2]=0x00;
		pbCMDOffline[3]=0x31;
		pbCMDOffline[4]=0x40;
		pbCMDOffline[5]=0x7E;
	}else{ /* Go to offline digital */
		pbCMDOffline[0]=0x29;
		pbCMDOffline[1]=0x01;
		pbCMDOffline[2]=0x00;
		pbCMDOffline[3]=0x31;
		pbCMDOffline[4]=0x40;
		pbCMDOffline[5]=0x7E;
	}

	// Set mobile phone offline
	dwWriteByte = 0;

	if ( InternalWrite(pbCMDOffline, 6 , &dwWriteByte) != ERROR_SUCCESS || (dwWriteByte != 6 )) {
		return FALSE;
	}
	
	dwReadByte = 0;
	ZeroMemory(pbCMDRsp,READ_BUFFER_LEN);
	//while ( m_strRecv.length() == 0 );
	//Sleep(100);
    // 20050118 Embert Tsai
	if ( WaitandReadBuffer(pbCMDRsp, READ_BUFFER_LEN, 6, &dwReadByte) != ERROR_SUCCESS ){
		return FALSE;
	}

	if ( memcmp(pbCMDOffline, pbCMDRsp, 6 ) != 0 ){
		return FALSE;
	}
	
	while ( nTestCount > 0 ) {
		Sleep(200);
		nTestCount --;
		// dectec phone mode
		if ( (InternalWrite(pbCMDStatus,4, &dwReadByte) != ERROR_SUCCESS) || (dwReadByte != 4) ){
			return FALSE;
		}
	
		BYTE pbCMDRsp[READ_BUFFER_LEN];
		ZeroMemory(pbCMDRsp,READ_BUFFER_LEN);
                // 20041110 Embert Tsai
		if ( WaitandReadBuffer(pbCMDRsp, READ_BUFFER_LEN, 51, &dwReadByte) != ERROR_SUCCESS ){
			return FALSE;
		}
		if ( pbCMDRsp[0] != 0x0C ){
			return FALSE;	
		}
		// 20050720 Embert Tsai.
		// Skip 0x7D in front of RF mode data.
		nReadCount = 0;
		nIndex = 0;
		nRFIndex = 0;
		nEntryIndex = 0;
		while (TRUE){
			if ( pbCMDRsp[nIndex] != 0x7D){
				nIndex++;
				nReadCount++;
			}else{
				nIndex++;
			}
			if ( 8 == nReadCount ){		//Original RF mode value position.
				nRFIndex = nIndex;
			}
			if ( 32 == nReadCount){		//Original Entry Reason value position.
				nEntryIndex = nIndex;
				break;
			}
		}
		CopyMemory( &wRFMode, pbCMDRsp+nRFIndex , 2);
		CopyMemory( &wEntryReason, pbCMDRsp+nEntryIndex, 2);		
		if( ( wRFMode == CMDIAG_MODE_OFFLINE_A_F && wEntryReason == DIAG_OFFLINE_ANALOG) ||
				( wRFMode == CMDIAG_MODE_OFFLINE_D_F && wEntryReason == DIAG_OFFLINE_DIGITAL))
		{
			return TRUE;
		}

	}
	return FALSE;
}


// 20050113 Embert Tsai
// Add this method for Topaz download
int CSerialIO::ChangeBaudRate(EBaudrate eBaudRate)
{
	StopListener();
	Sleep(2000);
	m_ePreferBaudRate = eBaudRate;
	m_nCurrentSpeedTry = GetSpeedIndex(m_ePreferBaudRate);
	StartListener();
	return RUN_SUCCESSFULLY;
}

void CSerialIO::LogBinary(const char* szTemp,DWORD nBufferSize)
{
#ifndef DISABLE_LOG

	const int nLenMax = 32;
	char szCharTemp[nLenMax+1] = {0};
	int nLineCount = 0;

	CStringA strLog;

	for(DWORD i=0;i<nBufferSize;i++)
	{		
		if(i > 0 && i % nLenMax == 0)
		{
			strLog += "   ";
			for(int j=0;j<nLenMax;j++)
			{
				CStringA strTemp;
				strTemp.Format("%c",szCharTemp[j]);
				strLog += strTemp;
			}
			strLog += "\n";
			nLineCount = 0;
		}
		BYTE nTest = szTemp[i];
		CStringA strTemp;
		strTemp.Format("%02X ",nTest);
		strLog += strTemp;
		szCharTemp[i%nLenMax] = (nTest <= 0x1F || nTest >= 0xFF) ? '.' : szTemp[i];
		nLineCount++;
	}

	for(int nSpaceCount = 0;nSpaceCount < (nLenMax - nLineCount);nSpaceCount++)	
		strLog += "   ";

	strLog += "   ";
	for(int j=0;j<nLineCount;j++)
	{		
		CStringA strTemp;
		strTemp.Format("%c",szCharTemp[j]);
		strLog += strTemp;
	}
	strLog += "\n\n";

	AddLog(strLog);

#endif//#ifndef DISABLE_LOG
}

long CSerialIO::InternalWrite(const void* pData, size_t iLen, DWORD* pdwWritten /* = 0 */)
{
	AutoProtect protect(m_csWrite);

	//AddLog(_T("CSerialIO::InternalWrite"));

	m_lLastError = ERROR_SUCCESS;
	// Use our own variable for read count
	DWORD dwWritten;
	if (pdwWritten == 0)
	{
		pdwWritten = &dwWritten;
	}
	// Reset the number of bytes written
	*pdwWritten = 0;
	// Check if the device is open
	if (m_hFile == 0 || m_hFile == INVALID_HANDLE_VALUE)
	{
		m_lLastError = ERROR_INVALID_HANDLE;
		AddLog(_T("COM port handle is null"));
		return m_lLastError;
	}

	OVERLAPPED ovInternal = {0};
	ovInternal.hEvent = m_hWriteEvent;

	if (!::WriteFile(m_hFile,pData,iLen,pdwWritten,&ovInternal))
	{
		long lLastError = ::GetLastError();
		if (lLastError != ERROR_IO_PENDING)
		{
			m_lLastError = lLastError;
		}
		else
		{
			switch (::WaitForSingleObject(ovInternal.hEvent,INFINITE))
			{
			case WAIT_OBJECT_0:
				// The overlapped operation has completed
				if (!::GetOverlappedResult(m_hFile,&ovInternal,pdwWritten,FALSE))
				{
					m_lLastError = ::GetLastError();
					AddLog(_T("InteralWrite: Write fail\n"));
				}
				else
				{					
					AddLog(_T("Write data success"));
					LogBinary((const char*)pData,*pdwWritten);
				}
				break;

			case WAIT_TIMEOUT:
				::CancelIo(m_hFile);
				m_lLastError = ERROR_TIMEOUT;
				AddLog(_T("InteralWrite: Write time out\n"));
				break;
			default:
				m_lLastError = ::GetLastError();
				break;
			}
		}
	}
	else
		// It should means the unit is disconnected.
		m_lLastError = ERROR_NOT_READY;

	return m_lLastError;
}

////////////////////////////////////////////////////////////////////////////
/*************************************************************************
* Purpose: Change mode from DIAG to DL for all MSM6000 and MSM6250 related platform.
* Input  : NONE
* Return : TRUE	-->run succsesfully.
*			FALSE	-->mode change failed.
* Exception: NONE
* Note	  : 2007/11/13 first version created by Embert Tsai.
***************************************************************************/
////////////////////////////////////////////////////////////////////////////
int CSerialIO::InternalChange2DLDefault(){
	// 20050218 Embert Tsai.
	// Prevent outer process to read and write com port when entering download.
	m_bIsPreventOuterWrite = TRUE;
	m_bIsPreventOuterRead = TRUE;
	Sleep(500);
	StopLogEventMessagePacket();
	EMobileMode nMobileMode = GetMobileMode();
	if ( ! ChangeToOffline() ){
		m_bIsPreventOuterWrite = FALSE;
		m_bIsPreventOuterRead = FALSE;
		return FALSE;
	}
	StopListener();
	Sleep(1000);	// 2004/09/21 Embert Tsai. Give mobile and driver more time to change to download mode.
	int i = 1;
	m_ePreferBaudRate = m_eBaudrate;
	m_nCurrentSpeedTry = GetSpeedIndex(m_ePreferBaudRate);
	for ( i = 1 ; i< 10 ; i+=i){
		if ( InternalOpen() != ERROR_SUCCESS ){
			Sleep(i * 1000);	// 2004/09/21 Embert Tsai. Give mobile and driver more time to change to download mode.
		} else {
			break;
		}
	}
	if ( i > 10 )
		return FALSE;

	BYTE pbCMDDL[4] = {0x3A,0xA1,0x6E,0x7E};
	::Sleep(1000);
	if(!InternalWrite(pbCMDDL,4))
	{
		if ( nMobileMode == MM_ULYSSES){
			InternalClose();
			Sleep(5000);	// 2004/09/21 Embert Tsai. Give mobile and driver more time to change to download mode.
		}
		m_bIsPreventOuterWrite = FALSE;
		m_bIsPreventOuterRead = FALSE;
		StartListener();
		int i = 0;
		while( (i++) < MODE_DETECTION_RETY_TIMES )
		{
			switch(m_eState)
			{
			case EStop:
			case EStart:
				break;
			case EDS:
				return FALSE;
			case EDIAG:
				return FALSE;
			case EDL:
				return TRUE;
			}
			Sleep(MODE_DETECTION_DELAY);
		}
		return FALSE;
	}

	m_bIsPreventOuterWrite = TRUE;
	m_bIsPreventOuterRead = TRUE;

	return FALSE;

}

////////////////////////////////////////////////////////////////////////////
/*************************************************************************
* Purpose: Change mode from DIAG to DL for all 6280 related platform.
* Input  : NONE
* Return : TRUE	-->run successfully.
*			FALSE	-->mode change failed.
* Exception: NONE
* Note	  : 2007/11/13 first version created by Embert
***************************************************************************/
////////////////////////////////////////////////////////////////////////////
int CSerialIO::InternalChange2DL6280(){

	BOOL bRet = FALSE;
	// Prevent outer process to read and write com port when entering download.
	m_bIsPreventOuterWrite = TRUE;
	m_bIsPreventOuterRead = TRUE;
	StopLogEventMessagePacket();

	BYTE pbCMDDL[4] = {0x3A,0xA1,0x6E,0x7E};	// mode change command for all Qualcomm platform.
	if(!InternalWrite(pbCMDDL,4))
	{
		m_bIsPreventOuterWrite = FALSE;
		m_bIsPreventOuterRead = FALSE;
		m_bIsCOMReopen = FALSE;
		
		
		//InternalResetCOM();	// Reset COM port status.
		StopListener();
			
		for (int i = 1 ; i< 10 ; i+=i)
		{
			if ( InternalOpen() != ERROR_SUCCESS )
			{	
				Sleep(i * 1000);	// 2004/09/21 Embert Tsai. Give mobile and driver more time to change to download mode.
				TRACE("InternalChange2DL6280: InternalOpen failed###########################\n");
			}
			else
			{
				TRACE("InternalChange2DL6280: InternalOpen OK!!###########################\n");
				break;
			}
		}
		TRACE("InternalChange2DL6280: StartListener###########################\n");
		
		StartListener();
		int i = 0;
		
		ResetMode();

		while( (i++) < MODE_DETECTION_RETY_TIMES ) // busy waiting
		{	TRACE("CSerialIO::InternalChange2DL6280(): busying waiting change mode\n");
			switch(m_eState)
			{
			case EStop:
			case EStart:
				Sleep(0);
				break;
			case EDS:
				break;
			case EDIAG:
				break;
			case EDL:
				bRet = TRUE;
			}
			if(bRet == FALSE)
			{
				Sleep(MODE_DETECTION_DELAY);
				Sleep(0);
			}
			else
				break;
		}
	}
	if(bRet == FALSE)
	{
		m_bIsPreventOuterWrite = TRUE;
		m_bIsPreventOuterRead = TRUE;
	}
	
	return bRet;
}
int CSerialIO::DIAG2DL()
{
	BOOL bRet = FALSE;
	AddLog(_T("CSerialIO::DIAG2DL()\n"));
	_ASSERTE(m_eState == EDIAG);
	// Prevent outer process to read and write com port when entering download.
	m_bIsPreventOuterWrite = TRUE;
	m_bIsPreventOuterRead = TRUE;
	StopLogEventMessagePacket();

	BYTE pbCMDDL[4] = {0x3A,0xA1,0x6E,0x7E};	// mode change command for all Qualcomm platform.
	AddLog(_T("CSerialIO::InternalChange2DL6280(): Write Diag to DL mode command \n"));
	if(!InternalWrite(pbCMDDL,4))
	{
		m_bIsPreventOuterWrite = FALSE;
		m_bIsPreventOuterRead = FALSE;
		m_bIsCOMReopen = FALSE;

		ResetMode();
		
		AddLog(_T("CSerialIO::DIAG2DL() ResetMode()"));

		int nCountWaitMax = 6;
		SendProbe();
		while(WAIT_TIMEOUT == ::WaitForSingleObject(m_hModeEvent, 20000))
		{
			nCountWaitMax--;
			if(nCountWaitMax < 0)
			{
				AddLog(_T("Timeout"));
				break;
			}
			SendProbe();
			AddLog(_T("WAIT_TIMEOUT == ::WaitForSingleObject(m_hModeEvent, 20000)"));
		};
	}	
	if(GetState() == EDL)
	{
		AddLog(_T("GetState() == EDL"));
		bRet = TRUE;
	}

	if(bRet == FALSE)
	{
		AddLog(_T("Change to DL fail."));
		m_bIsPreventOuterWrite = TRUE;
		m_bIsPreventOuterRead = TRUE;
	}

	return bRet;
}

////////////////////////////////////////////////////////////////////////////
/*************************************************************************
* Purpose: Rest all serial COM port setting and purge it. The parameters comes from QPST log.
* Input  : NONE
* Return : ERROR_SUCCESS -->run successfully.
*			OTHERS  -->error.
* Exception: NONE
* Note	  : 2007/11/13 first version created by Embert
***************************************************************************/
////////////////////////////////////////////////////////////////////////////
int CSerialIO::InternalResetCOM(){
	// Set COM port event mask

	if ( !::SetCommMask(m_hFile, EV_CTS | EV_DSR | EV_RLSD | EV_BREAK) ){
		m_lLastError = ::GetLastError();
		return m_lLastError;	
	}

	// Set Input/Ouput Queue Size
	if (!::SetupComm(m_hFile, SERIAL_QUEUE_SIZE, SERIAL_QUEUE_SIZE)){
		m_lLastError = ::GetLastError();
		return m_lLastError;	
	}

	// Purge COM port buffer
	if(!::PurgeComm(m_hFile, PURGE_TXCLEAR |
		PURGE_RXCLEAR |
		PURGE_TXABORT |
		PURGE_RXABORT) )
	{
		m_lLastError = ::GetLastError();
		return m_lLastError;	
	}

	// setup the DCB structure for the device
	DCB dcb;
	if(!::GetCommState(m_hFile, &dcb))
	{
		m_lLastError = ::GetLastError();
		return m_lLastError;	
	}
	dcb.DCBlength = sizeof (DCB);
	dcb.BaudRate = m_eBaudrate;
	dcb.fBinary = TRUE;
	dcb.fParity = FALSE;
	// Enable RTS & DTR control signal to support power for data cable (PhoneJack)
	dcb.fRtsControl = RTS_CONTROL_ENABLE;
	dcb.fDtrControl = DTR_CONTROL_ENABLE;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fDsrSensitivity = FALSE;
	dcb.fTXContinueOnXoff = FALSE;
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;
	dcb.fErrorChar = FALSE;
	dcb.fNull = FALSE;
	dcb.fAbortOnError = TRUE;  //  check with ClearCommError ()
	dcb.XonLim = 0;  // don't care
	dcb.XoffLim = 0;  //  don't care
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.XonChar = 0;  //  don't care
	dcb.XoffChar = 0;  //  don't care
	dcb.ErrorChar = 0;  //  don't care
	dcb.EofChar = 0x00;  //  20071113 Embert Tsai. Change for 6280.
	dcb.EvtChar = 0x7E;  //  should signal EV_RXFLAG

	if(!::SetCommState(m_hFile, &dcb))
	{
		m_lLastError = ::GetLastError();
		return m_lLastError;	
	}

	// The following default value are changed according to QPST log.
	COMMTIMEOUTS cto;
	cto.ReadIntervalTimeout = DEF_READ_INTERVAL_TIMEOUT;
	cto.ReadTotalTimeoutMultiplier = DEF_READ_TOTAL_TIMEOUT_MULTIPLIER;
	cto.ReadTotalTimeoutConstant = DEF_READ_TOTAL_TIMEOUT_CONSTANT;
	cto.WriteTotalTimeoutMultiplier = DEF_WRITE_TOTAL_TIMEOUT_MULTIPLIER;
	cto.WriteTotalTimeoutConstant = DEF_WRITE_TOTAL_CONSTANT;

	if (!::SetCommTimeouts(m_hFile,&cto))
	{
		m_lLastError = ::GetLastError();
		return m_lLastError;	
	}

	if(!::PurgeComm(m_hFile, PURGE_TXCLEAR |
		PURGE_RXCLEAR |
		PURGE_TXABORT |
		PURGE_RXABORT) )
	{
		m_lLastError = ::GetLastError();
		return m_lLastError;	
	}

	return ERROR_SUCCESS;
}

// 20050218 Embert Tsai 
// Stop log, event, and message packet.
void CSerialIO::StopLogEventMessagePacket(){
	// Wait for all write process complete.
	DWORD dwLoopCount = 0;
	while ( m_dwCurWriteCount != 0){
		Sleep(200);
	}
	Sleep(200);	

	BYTE pbCMDRsp[READ_BUFFER_LEN];
	DWORD dwReadByte = 0; 

	for ( int i = 0 ; i < 1 ; i ++){
		InternalWrite("\x1f\xff\x00\x5e\xf6\x7e", sizeof("\x1f\xff\x00\x5e\xf6\x7e")-1);
		WaitandReadBuffer(pbCMDRsp, READ_BUFFER_LEN, 4);
		// CMD_CODE 96 Event Configuration Request
		InternalWrite( "\x60\x00\x12\x6A\x7E", sizeof("\x60\x00\x12\x6A\x7E")-1);
		WaitandReadBuffer(pbCMDRsp, READ_BUFFER_LEN, 4);
		// CMD_CODE 115 Logging Configuration Request
		InternalWrite("\x73\x00\x00\x00\x00\x00\x00\x00\xda\x81\x7e", sizeof("\x73\x00\x00\x00\x00\x00\x00\x00\xda\x81\x7e")-1);
		WaitandReadBuffer(pbCMDRsp, READ_BUFFER_LEN, 4);
	}
	// Wait for all packet completed.
	// Start reading all content from read buffer
	while ( dwLoopCount < 5){
		if ( m_dwBufStart != m_dwBufEnd){		
			CSerialIO::Read(pbCMDRsp, READ_BUFFER_LEN);
			dwLoopCount = 0;
		} else {
			dwLoopCount++;
		}
		Sleep(100);
	}
	
}

long CSerialIO::WaitandReadBuffer(void* pData, size_t iLen, DWORD dwExpectedLen, DWORD* pdwRead){
	
	_ASSERTE( pData != NULL);
	_ASSERTE( iLen > 0 );
	// 20060105 Embert Tsai.
	// To fix infinite loop bug, add time out count for while loop.
	DWORD nCount = 0;
	DWORD nReadCountLimit = 20;
	DWORD dwRead = 0;
	DWORD dwRet = 0;

	if (pdwRead == 0)
	{
		pdwRead = &dwRead;
	}
	*pdwRead = 0;

	_ASSERTE(m_hWaitEvent == NULL);
	// Wait for read buffer complete
	
	m_hWaitEvent = ::CreateEvent(0, FALSE, FALSE, 0);
	dwRet = WaitForSingleObject(m_hWaitEvent, 2000);

	if ( WAIT_TIMEOUT == dwRet){
		// Read time out
		::CloseHandle(m_hWaitEvent);
		m_hWaitEvent = NULL;
		return -1;
	}
/*
	while ( ((m_dwBufStart-m_dwBufEnd) < dwExpectedLen) && ((nCount) < nReadCountLimit) ) {
		Sleep(100);
		nCount++;
	}
	Sleep(100);
*/
	
	::CloseHandle(m_hWaitEvent);
	m_hWaitEvent = NULL;

	return CSerialIO::Read(pData, iLen, pdwRead);

}

// 20041110 Embert Tsai
// Change read buffer operation to reduce memory operation.
int CSerialIO::ReadBuffer(void* pvDest, int nLen){
	int nRet;
	DWORD dwBufLen = m_dwBufEnd - m_dwBufStart;
	nRet = nLen <= (int) dwBufLen ? nLen : (int)dwBufLen;
	CopyMemory(pvDest, m_pbRecvBuf+m_dwBufStart, nRet);
	m_dwBufStart += nRet;

	return nRet;
}

// 20041110 Embert Tsai
// Change read buffer operation to reduce memory operation.
int CSerialIO::AppendBuffer(const void* pvSrc, int nLen){

	if ( ( nLen + ( m_dwBufEnd - m_dwBufStart ) ) > m_dwBufSize  ){ // buffer size is tool small to contain the buffer
		BYTE *pbOld = m_pbRecvBuf;
		m_dwBufSize += ((nLen/RECV_BUFFER_SIZE)+1) * RECV_BUFFER_SIZE;
		m_pbRecvBuf = new BYTE[m_dwBufSize];
		ZeroMemory(m_pbRecvBuf, m_dwBufSize);
		if ( m_dwBufStart != m_dwBufEnd)	// Check buffer is empty.
			CopyMemory(m_pbRecvBuf, pbOld + m_dwBufStart, m_dwBufEnd-m_dwBufStart);

		m_dwBufEnd -= m_dwBufStart;
		m_dwBufStart = 0;
		delete[] pbOld;
		pbOld = NULL;
	} else if ( ( m_dwBufEnd + nLen )  >=  m_dwBufSize) {
		MoveMemory(m_pbRecvBuf, m_pbRecvBuf + m_dwBufStart, m_dwBufEnd - m_dwBufStart);
		m_dwBufEnd -= m_dwBufStart;
		m_dwBufStart = 0;
	}
	CopyMemory(m_pbRecvBuf+m_dwBufEnd,pvSrc, nLen );
	m_dwBufEnd += nLen;
	if ( m_hWaitEvent != NULL ){
		::SetEvent(m_hWaitEvent);
	}
	return nLen;

}

// 20050215 Embert Tsai.
// Translate COM string to COM port number
int	CSerialIO::GetCOMNum(LPCTSTR szDevice)
{
	TCHAR szCOMNum[COM_PORT_NAME_LENGTH] = {0};
	TCHAR szDeviceName[COM_PORT_NAME_LENGTH] = {0};
	TCHAR *szFind = NULL;
	int nCurrentPortNum = 0;

	_tcscpy(szDeviceName, szDevice);
	_tcsupr(szDeviceName);
	szFind = _tcschr(szDeviceName, 'M');
	_ASSERTE(szFind != NULL);
	_tcscpy(szCOMNum, szFind+1);
	nCurrentPortNum = _ttoi(szCOMNum);
	return nCurrentPortNum;
}

// 20050215 Embert Tsai
// Get current mobile mode. But currently, topaz is not workable.
EMobileMode CSerialIO::GetMobileMode(){

	UCHAR szBuffer[4];
	DWORD dwWrited = 0;
	DWORD dwReadByte = 0;

	szBuffer[0] = 0x00;
	szBuffer[1] = 0x78;
	szBuffer[2] = 0xF0;
	szBuffer[3] = 0x7E;
	InternalWrite(szBuffer, 4, &dwWrited);
	Sleep(1);

	BYTE pbCMDRsp[READ_BUFFER_LEN]={0};
	//ZeroMemory(pbCMDRsp,READ_BUFFER_LEN);
	// 20050218 Embert Tsai, for complete packet
	if ( WaitandReadBuffer(pbCMDRsp, READ_BUFFER_LEN, 58, &dwReadByte) != ERROR_SUCCESS ){
		return MM_UNKNOW;
	}

	if ( pbCMDRsp[0] != 0x00 ){
		return MM_UNKNOW;
	}
	char szVer[9] = {0};
	ZeroMemory(szVer,9);
	CopyMemory(szVer,pbCMDRsp+39,8);
	_strlwr_s(szVer,9);

	if( (strstr(szVer,"ulysses") != NULL) || (strstr(szVer,"ulyc2a") != NULL) || (strstr(szVer,"ulyc2b") != NULL) || (strstr(szVer,"ulyb1") != NULL))
	{
		return MM_ULYSSES;
	}
	else if(strstr(szVer,"tpz") != NULL)
	{
		return MM_TOPAZ;
	}
	else if(strstr(szVer, "msm5105") != NULL)
	{
		return MM_DOMINGO;
	}
	else
	{
		return  MM_UNKNOW;
	}

	return MM_UNKNOW;
}

int CSerialIO::GetSpeedIndex(EBaudrate eBaudrate){
	for ( int i = 0 ; i < SPEED_ARRAY_SIZE; i ++){
		if ( eBaudrate == m_TestSpeed[i])
			return i;
	}
	return -1;
}

LPCTSTR CSerialIO::GetComport(void){
	_ASSERTE(m_szCOM[0] == _T('C'));
	return m_szCOM;
}

////////////////////////////////////////////////////////////////////////////
/*************************************************************************
 * Purpose: Get the platform ID 
 * Input  : NONE
 * Return : Please refer to CSerialIO::EPlatform enumeration value for details.
  * Exception: NONE
 * Note	  : 2007/11/13 first version created by Embert Tsai.
 ***************************************************************************/
////////////////////////////////////////////////////////////////////////////
CSerialIO::EPlatform CSerialIO::GetPlatformID(){
	UCHAR szBuffer[4];
	DWORD dwWrited = 0;
	DWORD dwReadByte = 0;

	m_bIsPreventOuterWrite = TRUE;
	m_bIsPreventOuterRead = TRUE;

	szBuffer[0] = 0x7C;
	szBuffer[1] = 0x93;
	szBuffer[2] = 0x49;
	szBuffer[3] = 0x7E;
	InternalWrite(szBuffer, 4, &dwWrited);
	
	BYTE pbCMDRsp[READ_BUFFER_LEN] = {0};
	// 20050218 Embert Tsai, for complete packet
	if ( WaitandReadBuffer(pbCMDRsp, READ_BUFFER_LEN, 39, &dwReadByte) != ERROR_SUCCESS ){
		m_bIsPreventOuterWrite = FALSE;
		m_bIsPreventOuterRead = FALSE;
		return CSerialIO::E_PLAT_NOT_FOUND;
	}
	
	if ( pbCMDRsp[0] != 0x7C ){
		m_bIsPreventOuterWrite = FALSE;
		m_bIsPreventOuterRead = FALSE;
		return CSerialIO::E_PLAT_NOT_FOUND;
	}
	char szPlatformID[6] = {0};
	CopyMemory(szPlatformID,pbCMDRsp+12,5);
	_strupr_s(szPlatformID,6);
	
	if( (strstr(szPlatformID,"H10") != NULL) || (strstr(szPlatformID,"M6280") != NULL) || (strstr(szPlatformID,"ATHB1") != NULL) || (strstr(szPlatformID,"CORB1") != NULL))
	{
		m_bIsPreventOuterWrite = FALSE;
		m_bIsPreventOuterRead = FALSE;
		return CSerialIO::E_PLAT_6280;
	}else{
		m_bIsPreventOuterWrite = FALSE;
		m_bIsPreventOuterRead = FALSE;
		return CSerialIO::E_PLAT_DEFAULT;
	}
	
	m_bIsPreventOuterWrite = FALSE;
	m_bIsPreventOuterRead = FALSE;
	return E_PLAT_NOT_FOUND;

}

bool CSerialIO::MakesureCOMPortReadyToUse(int nCOMPort)
{
	bool bRes = false;
	if(nCOMPort == -1)
		nCOMPort = m_dwCOMNum;

	if(IsCOMPortAlive(nCOMPort) == FALSE)
	{
		DWORD  dwFilter = REG_NOTIFY_CHANGE_NAME |
			REG_NOTIFY_CHANGE_ATTRIBUTES |
			REG_NOTIFY_CHANGE_LAST_SET |
			REG_NOTIFY_CHANGE_SECURITY; 

		HKEY hKey;

		LONG lErrorCode = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_NOTIFY, &hKey);
		if (lErrorCode == ERROR_SUCCESS)
		{
			HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			if(hEvent)
			{
				// Watch the registry key for a change of value.
				lErrorCode = RegNotifyChangeKeyValue(hKey, TRUE, dwFilter, hEvent, TRUE);
				if(lErrorCode == ERROR_SUCCESS)
				{
					for(int i=0; i<2 && !bRes; i++)
					{
						if(WaitForSingleObject(hEvent, 600000) == WAIT_OBJECT_0)
						{
							bRes = IsCOMPortAlive(nCOMPort) == TRUE;
						}
					}
					
				}
				CloseHandle(hEvent);
			}
			m_lLastError = RegCloseKey(hKey);
		}
	}
	else
		bRes = true;

	
	
	return bRes;
}