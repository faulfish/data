// CSerCOMLibrary : Serial COM port library. Just for end user download.
// 2005/07/19 Embert Tsai.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SerCOMLibrary.h"
#include "SerialIO.h"
#include "BQPacketLib.h"
#include <string>
#include "../../CommonUtil/LogPolicy.h"

#define CONNECT_TRY_TIMES				6
#define	CONNECT_TRY_FAIL_DELAY_TIME		1000
#define READ_BUFFER_SIZE				8192
#define PHONE_MODE_DETECTION_TIMES		120
#define MODE_DETECTION_RETY_TIMES		40
#define MODE_DETECTION_DELAY			500
#define DELAY_FOR_COM_START				60000
#define DEFAULT_TIME_OUT				3000

CSerCOMLibrary::CSerCOMLibrary()
:m_bIsUsingRequest(false)
,m_bIsGetDLRevocation(false)
,m_bIsSetProbeRequestMode(false)
,m_nPortID(0)
{	
	m_hNotifyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	// Create event for Rx notification.
	::ResetEvent(m_hNotifyEvent);
	m_hReadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	// Create event for read completition.
	::ResetEvent(m_hReadEvent);
	_ASSERTE(m_hNotifyEvent !=NULL);
	_ASSERTE(m_hReadEvent != NULL);
	m_pbBuffer = new BYTE[READ_BUFFER_SIZE];
	ZeroMemory(m_pbBuffer, READ_BUFFER_SIZE);
	m_nReadCount = 0;
}

CSerCOMLibrary::~CSerCOMLibrary()
{	
	CloseHandle(m_hNotifyEvent);
	m_hNotifyEvent = NULL;
	CloseHandle(m_hReadEvent);
	m_hReadEvent = NULL;
	CSerialIO::Close();
	delete[] m_pbBuffer;
	m_pbBuffer = NULL;
	
}

int CSerCOMLibrary::GetComPort()
{
	return m_nPortID;
}

int CSerCOMLibrary::IsDIAGConnected()
{
	int i = 0;
	while ( i < PHONE_MODE_DETECTION_TIMES ){

		switch( GetState()) {
			case CSerialIO::EDL:
				return ERROR_CSERCOMLIBRARY_DL_CHANGE_INTERFACE_FAIL;
			case CSerialIO::EDIAG:
				return RUN_SUCCESSFULLY;
			case CSerialIO::EDS:
				if ( ChangeDS2DIAG() ){
					return RUN_SUCCESSFULLY;
				} else{
					return ERROR_CSERCOMLIBRARY_DS_CHANGE_INTERFACE_FAIL;
			}
		}
		Sleep(700);
		i++;
	}
	return CSerCOMLibrary::ERROR_CSERCOMLIBRARY_COM_SERVER_FAIL;
}

int CSerCOMLibrary::IsDLConnected()
{	
	AddLog(_T("CSerCOMLibrary::IsDLConnected"));

	int i = 0;
	int nRet = RUN_FAILED;
	EState State;
	while ( i < PHONE_MODE_DETECTION_TIMES ) 
	{
		State = GetState();
		switch( State ) 
		{
		case CSerialIO::EDL:
			AddLog(_T("DL detected."));
			nRet = RUN_SUCCESSFULLY;
			break;
		case CSerialIO::EDIAG:
			if ( ChangeDIAG2DL() )
			{
				AddLog(_T("DIAG to DL change completed."));
				nRet = RUN_SUCCESSFULLY;
			}else{
				AddLog(_T("DIAG to DL failed."));
				nRet = ERROR_CSERCOMLIBRARY_DL_CHANGE_INTERFACE_FAIL;
			}
			break;
		case CSerialIO::EDS:
			if ( ChangeDS2DL() )
			{
				AddLog(_T("DS to DL change completed."));
				nRet = RUN_SUCCESSFULLY;
			} 
			else
			{
				AddLog(_T("DS to DL change completed."));
				nRet = ERROR_CSERCOMLIBRARY_DS_CHANGE_INTERFACE_FAIL;
			}
			break;
		default:
			Sleep(1000);
		};//end switch

		i++;
		if(nRet == RUN_SUCCESSFULLY)
			break;
	}//end while
	return nRet;
}

void CSerCOMLibrary::AddLog(LPCTSTR szMsg)
{
#ifdef DETIAL_LOG
	LogPolicy::Log(QSYNC_LIB::GetAnsi(szMsg).c_str(),m_strLogFile.c_str());
#endif//#ifndef DISABLE_LOG
}

int CSerCOMLibrary::OpenComport(LPCTSTR lpszModelName, LPCTSTR lpszComport,bool bRequestFunction)
{

	if ( lpszModelName == NULL || lpszComport == NULL ){
		return ERROR_CSERCOMLIBRARY_INVALID_PARAMETER;
	}
	CString strCOMNum;
	strCOMNum = lpszComport;
	int nIndex =strCOMNum.ReverseFind(_T('M'));
	strCOMNum = strCOMNum.Mid(nIndex+1, strCOMNum.GetLength()-nIndex-1);

	m_nPortID = _ttoi(strCOMNum);
	
	CString strFormat;
	strFormat.Format(_T("Test_COM%02d.txt"),GetComPort());
	m_strLogFile = (LPCTSTR)strFormat;

	// 20050823 Embert Tsai.
	// Clear CRC buffer.
	// Clear all garbage left from last read.
	m_CRCChecker.Erase();
	if ( CSerialIO::Open(lpszComport,EBaud115200,bRequestFunction) != ERROR_SUCCESS ){
		return CSerCOMLibrary::ERROR_CSERCOMLIBRARY_COM_OPEN_FAIL;
	}
	return RUN_SUCCESSFULLY;
}

void CSerCOMLibrary::CloseComport()
{
	CSerialIO::Close();
}
bool CSerCOMLibrary::ResetMode()
{
	bool bRes = true;
	m_eState = EStart;
	bRes = ResetEvent(m_hModeEvent) == TRUE;
	return bRes;
}


INT CSerCOMLibrary::TXRXDiag(CHAR* lpTxData, DWORD dwTxLength, CHAR* lpRxData, DWORD dwRxLength, UINT uiTimeout)
{
	_ASSERTE(lpTxData != NULL);
	_ASSERTE(dwTxLength > 0);
	_ASSERTE(lpRxData != NULL);
	_ASSERTE(dwRxLength > 0 );
	_ASSERTE(uiTimeout > 0 );
	int nRet = 0;
	DWORD dwWritten = 0;
	
	if ( GetState() != CSerialIO::EDIAG )
		return CSerCOMLibrary::ERROR_CSERCOMLIBRARY_NULL_INTERFACE;
	
	return TxRx(lpTxData,dwTxLength,lpRxData,dwRxLength,uiTimeout);

}

int CSerCOMLibrary::TXRXDlGetRevocation(CHAR *lpTxData, DWORD dwTxLen, CHAR *lpRxData, DWORD dwRxLen, UINT uiTimeout)
{
	m_bIsGetDLRevocation = true;
	int nRet = TXRXDl(lpTxData, dwTxLen, lpRxData, dwRxLen, uiTimeout);
	m_bIsGetDLRevocation = false;
	return nRet;
}

INT CSerCOMLibrary::TXRXDl(CHAR *lpTxData, DWORD dwTxLen, CHAR *lpRxData, DWORD dwRxLen, UINT uiTimeout){
	
	int nRet = ERROR_SUCCESS;
	_ASSERTE(lpTxData != NULL);
	_ASSERTE(dwTxLen > 0 );
	_ASSERTE(lpRxData != NULL);
	_ASSERTE(dwTxLen > 0 );
	_ASSERTE(uiTimeout > 0 );

	LPCTSTR szFunc = _T("CSerCOMLibrary::TXRXDl");
	
	for(int i=0; GetState() != CSerialIO::EDL && i<PHONE_MODE_DETECTION_TIMES; i++ )
	{
		Sleep(0);
		TRACE("State: %d\n", GetState());
		Sleep(1000);
	}
	if( GetState()!= CSerialIO::EDL)
	{
		_ASSERTE(false);
		nRet = CSerCOMLibrary::ERROR_CSERCOMLIBRARY_NULL_INTERFACE;
	}
	else
		nRet = TxRx(lpTxData,dwTxLen,lpRxData,dwRxLen,uiTimeout);
	return nRet;

}

bool CSerCOMLibrary::GetReceiverData(CHAR *lpRxData, DWORD& dwRxLen)
{
	bool bRes = false;
	if(lpRxData == NULL)
	{//Timeout ot other error
		SetEvent(m_hReadEvent);
		::InterlockedDecrement((LPLONG)&m_nReadCount);
	}
	else
	{
		if(m_nRxLen <= static_cast<int>(dwRxLen))
		{
			ZeroMemory(lpRxData, dwRxLen);
			dwRxLen = m_nRxLen;
			CopyMemory(lpRxData, m_CRCChecker.GetBuffer(), m_nRxLen);
			SetEvent(m_hReadEvent);
			::InterlockedDecrement((LPLONG)&m_nReadCount);
			bRes = true;
		}	
	}
	return bRes;
}

int CSerCOMLibrary::WriteAndWait(CHAR *pBuffer, DWORD dwTxLen, CHAR *lpRxData, DWORD dwRxLen, UINT uiTimeout)
{
	DWORD dwWritten = 0;
	long lRet = CSerialIO::Write(pBuffer, dwTxLen,&dwWritten);
	
	if ( lRet < 0 )
	{// Write to COM failed.
		lRet = ERROR_CSERCOMLIBRARY_TX_FAIL;
	}
	else
	{
		DWORD dwRet = ::WaitForSingleObject(m_hNotifyEvent, uiTimeout);

		if ( dwRet == WAIT_TIMEOUT)
		{
			lRet = ERROR_CSERCOMLIBRARY_RX_FAIL;
		}
		else if (static_cast<int>(dwRxLen) < m_nRxLen)
		{
			lRet = ERROR_CSERCOMLIBRARY_RX_FAIL;
		}
		else
		{
			GetReceiverData(lpRxData, dwRxLen);
			lRet = (int)dwRxLen;
		}
	}

	return lRet;
}



int CSerCOMLibrary::TxRx(CHAR *lpTxData, DWORD dwTxLen, CHAR *lpRxData, DWORD dwRxLen, UINT uiTimeout, bool bOneTime)
{
	LONG lRet = RUN_FAILED;
	DWORD dwWritten = 0;
	DWORD volatile dwBufPos = m_dwBufEnd-m_dwBufStart;
	CHAR chStart, chEnd;
	void *pBuffer = NULL;
	int nPackedLen = 0;
	chStart = PACKET_NULL_CHAR;
	chEnd = PACKET_NULL_CHAR;
	BOOL bIsReadCompleted = FALSE;
	int nRet = 0;
	DWORD dwRet = 0;
	
	_ASSERTE(lpRxData != NULL);_ASSERTE(lpRxData != NULL);_ASSERTE(dwTxLen > 0);_ASSERTE(dwRxLen > 0);
	
	m_CRCChecker.SetState(m_eState);
	// Boyan Zhou add 2007/4/10
	// for cmd
	BYTE bycmd = lpTxData[0];
	m_CRCChecker.SetCom(bycmd);
	// End
	if(m_eState == EDIAG)
	{
		chEnd = END_CHAR;	   //0x7E
	} 
	else if(m_eState == EDL)
	{
		chStart = START_CHAR;  //0x7E
		chEnd = END_CHAR;      //0x7E
	} 

	nPackedLen = CBQPacketLib::PackBuffer( lpTxData, dwTxLen, chStart, chEnd, &pBuffer);

	_ASSERTE(nPackedLen > 0); _ASSERTE(pBuffer != NULL);

	int nTimes = bOneTime ? 1 : 2;
	if(bOneTime)
		nTimes = 1;
	for(int i=0; i<nTimes && lRet<0; i++)
	{
		lRet = CSerialIO::Write(pBuffer, nPackedLen,&dwWritten);
		CString strFormat;
		strFormat.Format(_T("TXRX: Write %d times"), i);
		AddLog(strFormat);
		

		// Write to COM failed.
		if ( lRet < 0 )
		{
			lRet = ERROR_CSERCOMLIBRARY_TX_FAIL;
		}
		else
		{
			dwRet = WaitForSingleObject(m_hNotifyEvent, uiTimeout);

			if ( dwRet == WAIT_TIMEOUT){
				GetReceiverData(NULL, dwRxLen);
				lRet = ERROR_CSERCOMLIBRARY_RX_FAIL;
				AddLog(_T("Timeout on SercomLibrary::TxRx"));
			}
			else if (static_cast<int>(dwRxLen) < m_nRxLen){
				GetReceiverData(NULL, dwRxLen);
				lRet = ERROR_CSERCOMLIBRARY_RX_FAIL;
				AddLog(_T("Buffer overflow on SercomLibrary::TxRx"));
			}
			else
			{
				GetReceiverData(lpRxData, dwRxLen);
				lRet = (int)dwRxLen;
			}
		}
	}
	delete[] pBuffer;
	pBuffer = NULL;
	

	return lRet;
}

int CSerCOMLibrary::SpeedUpTxRx(CHAR *lpTxData, DWORD dwTxLen, CHAR *lpRxData, DWORD dwRxLen, UINT uiTimeout)
{
	LONG lRet = RUN_FAILED;
	DWORD dwWritten = 0;
	DWORD volatile dwBufPos = m_dwBufEnd-m_dwBufStart;
	CHAR chStart = PACKET_NULL_CHAR;
	CHAR chEnd = PACKET_NULL_CHAR;
	
	BOOL bIsReadCompleted = FALSE;

	m_CRCChecker.SetState(m_eState);
	BYTE bycmd = lpTxData[0];
	m_CRCChecker.SetCom(bycmd);

	if(m_eState == EDIAG)
	{
		chEnd = END_CHAR;	   //0x7E
	} 
	else if(m_eState == EDL)
	{
		chStart = START_CHAR;  //0x7E
		chEnd = END_CHAR;      //0x7E
	} 
	
	void *pBuffer = NULL;
	int nPackedLen = 0;
	nPackedLen = CBQPacketLib::SpeedUpPackBuffer( lpTxData, dwTxLen, chStart, chEnd, &pBuffer);
	
	for(int i=0; i<20 && lRet<0; i++)
	{
		lRet = CSerialIO::Write(pBuffer, nPackedLen,&dwWritten);
		TRACE("TXRXSpeedup: Write %d times", i);
		if ( lRet < 0 )
			lRet = ERROR_CSERCOMLIBRARY_TX_FAIL;
		else
		{
			//	Sleep(0);
			DWORD dwRet = 0;
			dwRet = WaitForSingleObject(m_hNotifyEvent, uiTimeout);
			
			if ( dwRet == WAIT_TIMEOUT)
				lRet = ERROR_CSERCOMLIBRARY_RX_FAIL;
			else if (static_cast<int>(dwRxLen) < m_nRxLen)
				lRet = ERROR_CSERCOMLIBRARY_RX_FAIL;
			else
			{
				GetReceiverData(lpRxData, dwRxLen);
				lRet = (int)dwRxLen;
			}
		}
	}
	delete[] pBuffer;
	pBuffer = NULL;
	return lRet;
}

int CSerCOMLibrary::DLSend(CHAR * lpSendData, DWORD dwSendLen, BOOL IsCheckState){
	
	LPCTSTR szFunc = _T("CSerCOMLibrary::DLSend");	
	_ASSERTE(lpSendData != NULL);
	_ASSERTE(dwSendLen > 0 );
	int nRet = 0;	
	DWORD dwWritten = 0;
	
	//if ( GetState() != CSerialIO::EDL && IsCheckState)
	//	return CSerCOMLibrary::ERROR_CSERCOMLIBRARY_NULL_INTERFACE;

	CHAR chStart, chEnd;
	LONG lRet = 0;
	void *pBuffer = NULL;
	int nPackedLen = 0;
	chStart = PACKET_NULL_CHAR;
	chEnd = PACKET_NULL_CHAR;
	chStart = START_CHAR;
	chEnd = END_CHAR;
	
	nPackedLen = CBQPacketLib::PackBuffer( lpSendData, dwSendLen, chStart, chEnd, &pBuffer);
	//LogInformation(LOG_LEVEL_DEBUG, "Packed buffer source in LONG CSIOChannel::Write(const void* pData, size_t iLen, DWORD* pdwWritten)")	;
	//LogBuffer(LOG_LEVEL_DEBUG, pBuffer, nPackedLen);
	_ASSERTE(nPackedLen > 0);
	_ASSERTE(pBuffer != NULL);

	lRet = CSerialIO::Write(pBuffer, nPackedLen,&dwWritten);
	Sleep(0);
	delete[] pBuffer;
	pBuffer = NULL;
	
	if ( dwWritten != (DWORD)nPackedLen){
		return CSerCOMLibrary::ERROR_CSERCOMLIBRARY_TX_FAIL;
	}
	
	return nRet;

}

void CSerCOMLibrary::OnEvent(EEvent eEvent){

	//AddLog(_T("CSerCOMLibrary::OnEvent(EEvent eEvent)"));

	CHAR	chStart = PACKET_NULL_CHAR;
	CHAR	chEnd	= PACKET_NULL_CHAR;
	int		nRet	= 0;
	DWORD	dwRead	= 0;
	EState	eState	= EStart;
	ResetEvent(m_hModeEvent);

	ZeroMemory(m_pbBuffer, sizeof(m_pbBuffer));
	eState = GetState();

	if ( CSerialIO::EOnRecv == eEvent){
		if( Read(m_pbBuffer, DIAG_MAX_RESPONSE_LEN, &dwRead) != ERROR_SUCCESS ||	//+ Read Failed
			dwRead == 0)										//+ No more received
		{
			AddLog(_T("READ_BUFFER_SIZE"));
			_ASSERTE( dwRead <= READ_BUFFER_SIZE);
			return;
		}
		
		if(m_bIsUsingRequest)
		{
			//AddLog(_T("m_bIsUsingRequest Mode"));

			CBufferPtr ptReceiver = new CBuffer(dwRead, (const char*)m_pbBuffer);
			push(false,ptReceiver);
			if(m_ptRequest->IsCompletePacket(ptReceiver))
			{	
				if(m_ptRequest->WaitForOtherResponse() == false)
					ResetRequestMode();
				m_ptRequest->OnDataReceiver(ptReceiver);			
			}
			else
			{
				AddLog(_T("m_ptRequest->IsCompletePacket(ptReceiver) fail"));
			}
		}
		else if(m_bIsSetProbeRequestMode)
		{
			AddLog(_T("m_bIsSetProbeRequestMode Mode"));

			CBufferPtr ptReceiver = new CBuffer(dwRead, (const char*)m_pbBuffer);			
			push(false,ptReceiver);
			if(m_ptRequest->IsCompletePacket(ptReceiver))
			{				
				m_ptRequest->OnDataReceiver(ptReceiver);				
				m_bIsSetProbeRequestMode = false;
			}
			else
			{
				AddLog(_T("m_ptRequest->IsCompletePacket(ptReceiver) fail"));
			}
		}
		else if(m_bIsSetProbeMode)
		{
			AddLog(_T("m_bIsSetProbeMode Mode"));

			DetectMode(dwRead, (char*)m_pbBuffer);

			m_bIsSetProbeMode = false;
		}
		else if(m_bIsGetDLRevocation)
		{
			AddLog(_T("m_bIsGetDLRevocation Mode"));

			m_nRxLen = m_CRCChecker.Push(m_pbBuffer,3);
			SetEvent(m_hNotifyEvent);
			WaitForSingleObject(m_hReadEvent, INFINITE);
		}
		else
		{
			AddLog(_T("Normal Response Mode"));

			m_CRCChecker.SetState(eState);
			// Set up start character and end character
			if(eState == EDIAG || eState == EDL)
			{
				for ( DWORD j = 0 ; j < dwRead; j ++)
				{
					m_nRxLen = m_CRCChecker.Push(m_pbBuffer[j]);
					if ( m_nRxLen > 0)
					{ // buffer unpack successfully.
						// Set wait event
						::InterlockedIncrement((LPLONG)& m_nReadCount);
						
						AddLog(_T("OnEvent : SetEvent () [can read packet] \n"));
						SetEvent(m_hNotifyEvent);
						WaitForSingleObject(m_hReadEvent, INFINITE);
						AddLog(_T("OnEvent : WaitForSingleObject m_hReadEvent [wait for handling packet] \n"));
						m_nRxLen = 0;
						m_CRCChecker.Erase();
						break;
					}
					else if ( m_nRxLen < 0 )
					{
						// Set wait event
						m_nRxLen = 0;
						m_CRCChecker.Erase();
						return;
					}
				}
			}
			else if ( eState == EDS )
			{
				_ASSERTE(FALSE);
			}
			else
			{
				// should not fall into this field
				_ASSERTE(FALSE);
			}
		}
	}

}

BOOL CSerCOMLibrary::IsOutBufEmpty(){
	return (m_nRxLen >0 );
}

int CSerCOMLibrary::DIAGSend(CHAR * lpSendData, DWORD dwSendLen, BOOL IsCheckState){
	
	LPCTSTR szFunc = _T("CSerCOMLibrary::DLSend");	
	_ASSERTE(lpSendData != NULL);
	_ASSERTE(dwSendLen > 0 );
	int nRet = 0;	
	DWORD dwWritten = 0;
	
	//if ( GetState() != CSerialIO::EDL && IsCheckState)
	//	return CSerCOMLibrary::ERROR_CSERCOMLIBRARY_NULL_INTERFACE;

	CHAR chStart, chEnd;
	LONG lRet = 0;
	void *pBuffer = NULL;
	int nPackedLen = 0;
	chStart = PACKET_NULL_CHAR;
	chEnd = END_CHAR;
	
	nPackedLen = CBQPacketLib::PackBuffer( lpSendData, dwSendLen, chStart, chEnd, &pBuffer);
	//LogInformation(LOG_LEVEL_DEBUG, "Packed buffer source in LONG CSIOChannel::Write(const void* pData, size_t iLen, DWORD* pdwWritten)")	;
	//LogBuffer(LOG_LEVEL_DEBUG, pBuffer, nPackedLen);
	_ASSERTE(nPackedLen > 0);
	_ASSERTE(pBuffer != NULL);

	lRet = CSerialIO::Write(pBuffer, nPackedLen,&dwWritten);
	Sleep(0);
	delete[] pBuffer;
	pBuffer = NULL;
	
	if ( dwWritten != (DWORD)nPackedLen){
		return CSerCOMLibrary::ERROR_CSERCOMLIBRARY_TX_FAIL;
	}
	
	return nRet;

}

void CSerCOMLibrary::ResetReadParameter(){
	
}

void CSerCOMLibrary::ResetRequestMode()
{
	//AddLog(_T("m_bIsUsingRequest = false;"));
	m_bIsUsingRequest = false;
}

bool CSerCOMLibrary::ExecuateProbe(PLATFORM_TOOL::CRequestPtr& ptRequest)
{
	AddLog(_T("CSerCOMLibrary::ExecuateProbe ()"));

	bool bRes = false;

	m_bIsSetProbeRequestMode = true;

	m_ptRequest = ptRequest;

	CBufferPtr ptBufferSend;
	ptRequest->GetSendData(ptBufferSend);
	push(true,ptBufferSend);

	DWORD dwWritten = 0;
	long lRet = Write(const_cast<CHAR *>(ptBufferSend->GetData()),ptBufferSend->GetLength(),&dwWritten);
	bRes = lRet == 0;
	if(bRes == false)
	{
		AddLog(_T("CSerCOMLibrary::ExecuateProbe ==>Write fail"));
		ResetRequestMode();
	}

	return bRes;
}

bool CSerCOMLibrary::ExecuateRequest(PLATFORM_TOOL::CRequestPtr& ptRequest)
{
	AddLog(_T("CSerCOMLibrary::ExecuateRequest ()"));

	bool bRes = false;

	if(m_bIsUsingRequest == false)
	{
		AddLog(_T("m_bIsUsingRequest = true;"));
		m_bIsUsingRequest = true;
		m_ptRequest = ptRequest;

		CBufferPtr ptBufferSend;
		ptRequest->GetSendData(ptBufferSend);
		push(true,ptBufferSend);

		DWORD dwWritten = 0;
		long lRet = Write(const_cast<CHAR *>(ptBufferSend->GetData()),ptBufferSend->GetLength(),&dwWritten);
		bRes = lRet == 0;
		if(bRes == false)
		{
			AddLog(_T("CSerCOMLibrary::ExecuateRequest ==>Write fail"));
			ResetRequestMode();
		}

		/*
		DWORD dwReturn;
		if(WaitForWrite(ptRequest->GetTimeout(),dwReturn))
		{
			int nBufferLen = ptRequest->GetReceiveBufferSize();
			CBufferPtr ptReceiver = new CBuffer(nBufferLen, new char[nBufferLen]);
			DWORD nReceiver = ptReceiver->GetLength();
			if(GetReceiverData(const_cast<CHAR *>(ptReceiver->GetData()),nReceiver))
			{
				ptReceiver->ReleaseBuffer(nReceiver);
				ptRequest->OnDataReceiver(ptReceiver);
				bRes = true;
			}
		}
		*/
	}
	else
	{
		AddLog(_T(" CSerCOMLibrary::ExecuateRequest() fail"));
	}

	return bRes;
}

void CSerCOMLibrary::push(bool bIsSendRequest,CBufferPtr& ptBufferSend)
{
	m_RequestLogQueue.push(CRequestLog(bIsSendRequest,ptBufferSend));
	while(m_RequestLogQueue.size() > LOG_MAXIMUM)
		m_RequestLogQueue.pop();
}

void CSerCOMLibrary::pop()
{
	while(m_RequestLogQueue.empty() == false)
	{
		CRequestLog Log = m_RequestLogQueue.front();
		
		SYSTEMTIME LocalTime;
		SystemTimeToTzSpecificLocalTime(NULL,&Log.m_UTCTime,&LocalTime);

		CString strTemp;
		if(Log.m_bIsSendRequest)
			strTemp.Format(_T("Send %04d\\%02d\\%02d %02d:%02d:%02d\r\n"),LocalTime.wYear,LocalTime.wMonth,LocalTime.wDay,LocalTime.wHour,LocalTime.wMinute,LocalTime.wSecond);
		else
			strTemp.Format(_T("Read %04d\\%02d\\%02d %02d:%02d:%02d\r\n"),LocalTime.wYear,LocalTime.wMonth,LocalTime.wDay,LocalTime.wHour,LocalTime.wMinute,LocalTime.wSecond);
		
		LogBinaryToFile(strTemp,Log.m_ptRequest->GetData(),Log.m_ptRequest->GetLength());		
		m_RequestLogQueue.pop();
	}
}

void CSerCOMLibrary::LogBinaryToFile(const char* szDescriptiion,const char* szTemp,DWORD nBufferSize)
{
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

	strLog = szDescriptiion + strLog;
	LogPolicy::Log(QSYNC_LIB::GetAnsi((LPCTSTR)strLog).c_str(),m_strLogFile.c_str());
}

void CSerCOMLibrary::PopLastLog()
{
	pop();
}