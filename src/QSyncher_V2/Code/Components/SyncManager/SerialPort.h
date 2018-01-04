#pragma once

#include <vector>
#include <string>
#include <loki/smartptr.h>
#include "..\..\Components\PIMDB\commondata.h"
#include "..\..\CommonUtil\Buffer.h"

namespace QSYNC_LIB
{
	using namespace std;
	using namespace Loki;

class CSerialPort
{	
// Construction
public:

	CSerialPort():m_bIsOBEXMode(false),m_hComm(NULL),m_hThread(NULL),m_ptBuffer(new CBuffer){}
	CSerialPort(const TSTRING& portname):m_bIsOBEXMode(false),m_portname(portname),m_hComm(NULL),m_hThread(NULL),m_ptBuffer(new CBuffer){OpenPort(m_portname);}
	virtual ~CSerialPort(){ClosePort();}

public:

	const bool IsOBEXMode() const {return m_bIsOBEXMode;}

	const bool SwitchToOBEX(const bool bSwitchToOBEX)
	{	
		bool bRes = false;
		CBufferPtr ptNewBuffer = new CBuffer;
		if(m_bIsOBEXMode != bSwitchToOBEX)
		{			
			if(bSwitchToOBEX)
			{				
				if(WriteAndRead("AT+CGMI\r",ptNewBuffer))
				{
					bRes = WriteAndRead("AT^SQWE=3\r",ptNewBuffer);
					if(bRes) Sleep(1000);
				}
			}
			else
			{
				BYTE szBuf[3] = {0x2B,0x2B,0x2B};
				WriteAndRead(szBuf,3,ptNewBuffer,1000);
				Sleep(1000);
				m_bIsOBEXMode = false;
				BYTE szEndBuf[4] = {'a','t','z',0x0d};
				while(WriteAndRead(szEndBuf,4,ptNewBuffer,500) == false);
				bRes = WriteAndRead("AT+CGMI\r",ptNewBuffer,1000);
				assert(bRes);//switch to AT common mode fail
			}
		}
		if(bRes) m_bIsOBEXMode = bSwitchToOBEX;
		return bRes;
	}

	const bool WriteAndRead(const string& strBuffer,CBufferPtr& ptNewBuffer, DWORD dwDelay = 0)
	{
		return WriteAndRead((const BYTE*)strBuffer.c_str(),static_cast<long>(strBuffer.size()),ptNewBuffer,dwDelay);
	}

	const bool WriteAndRead(const BYTE* pByteBuffer,const DWORD nSize,CBufferPtr& ptNewBuffer, DWORD dwDelay = 0)
	{
		bool bRes = false;
		BYTE *pbyteBuffer = NULL;
		DWORD nTempSize = 0;
		if(WriteAndRead(pByteBuffer,nSize,&pbyteBuffer,nTempSize,dwDelay))
		{
			ptNewBuffer = new CBuffer(nTempSize,(char*)pbyteBuffer);
			delete [] pbyteBuffer;
			bRes = true;
		}
		return bRes;
	}

	const bool ConfigurePort(DWORD BaudRate, BYTE ByteSize, DWORD fParity, BYTE Parity, BYTE StopBits)
	{
		bool bRes = false;
		if(NULL != m_hComm)
		{
			if((m_bPortReady = GetCommState(m_hComm, &m_dcb))==0)
			{
				CloseHandle(m_hComm);
			}
			else
			{
				m_dcb.BaudRate =BaudRate;
				m_dcb.ByteSize = ByteSize;
				m_dcb.Parity =Parity ;
				m_dcb.StopBits =StopBits;
				m_dcb.fBinary=TRUE;
				m_dcb.fDsrSensitivity=false;
				m_dcb.fParity=fParity;
				m_dcb.fOutX=false;
				m_dcb.fInX=false;
				m_dcb.fNull=false;
				m_dcb.fAbortOnError=TRUE;
				m_dcb.fOutxCtsFlow=FALSE;
				m_dcb.fOutxDsrFlow=false;
				m_dcb.fDtrControl=DTR_CONTROL_DISABLE;
				m_dcb.fDsrSensitivity=false;
				m_dcb.fRtsControl=RTS_CONTROL_DISABLE;
				m_dcb.fOutxCtsFlow=false;
				m_dcb.fOutxCtsFlow=false;

				m_bPortReady = SetCommState(m_hComm, &m_dcb);
				if(m_bPortReady ==0)
					CloseHandle(m_hComm);
				else 
					bRes = true;
			}
		}
		return bRes;
	}

	const bool SetCommunicationTimeouts(DWORD ReadIntervalTimeout, DWORD ReadTotalTimeoutMultiplier, DWORD ReadTotalTimeoutConstant, DWORD WriteTotalTimeoutMultiplier, DWORD WriteTotalTimeoutConstant)
	{
		bool bRes = false;
		if(NULL != m_hComm)
		{
			if((m_bPortReady = GetCommTimeouts (m_hComm, &m_CommTimeouts))!=0)
			{
				m_CommTimeouts.ReadIntervalTimeout =ReadIntervalTimeout;
				m_CommTimeouts.ReadTotalTimeoutConstant =ReadTotalTimeoutConstant;
				m_CommTimeouts.ReadTotalTimeoutMultiplier =ReadTotalTimeoutMultiplier;
				m_CommTimeouts.WriteTotalTimeoutConstant = WriteTotalTimeoutConstant;
				m_CommTimeouts.WriteTotalTimeoutMultiplier =WriteTotalTimeoutMultiplier;
				m_bPortReady = SetCommTimeouts (m_hComm, &m_CommTimeouts);
				if(m_bPortReady == 0)
					CloseHandle(m_hComm);
				else
					bRes = true;
			}
		}
		return bRes;
	}

	const bool OpenPort(const TSTRING& portname)
	{
		if(NULL == m_hComm)
		{
			TSTRING strPort;
			strPort= _TC("//./") +portname;
			m_hComm = CreateFile(strPort.c_str(),
								  GENERIC_READ | GENERIC_WRITE,
								  0,
								  0,
								  OPEN_EXISTING,
								  FILE_FLAG_OVERLAPPED,
								  0);
			assert(m_hComm != NULL);
			BOOL bRes = SetCommMask(m_hComm, EV_RXCHAR);
			assert(bRes);
			memset(&m_opRead,0,sizeof(OVERLAPPED));
			memset(&m_opWrite,0,sizeof(OVERLAPPED));
			m_opRead.hEvent	= ::CreateEvent(NULL,TRUE,FALSE,NULL);	
			m_opWrite.hEvent= ::CreateEvent(NULL,TRUE,FALSE,NULL);

			m_hExitThread = CreateEvent(NULL,TRUE,FALSE,_TC("SerialThreadExitEvent"));
			ResetEvent(m_hExitThread);
			m_ReadToken = CreateEvent(NULL,TRUE,FALSE,_TC("SerialPortRead"));
			ResetEvent(m_ReadToken);
			m_WriteToken = CreateEvent(NULL,TRUE,TRUE,_TC("SerialPortWrite"));

			DWORD dwThreadID = 0;
			m_hThread = CreateThread(NULL,0,ThreadProcA,this,0,&dwThreadID);
		}
		return m_hComm != NULL;
	}

	void ClosePort()
	{
		if(m_hComm)
		{
			::SetCommMask(m_hComm, 0);
			::PurgeComm(m_hComm, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
			SetEvent(m_hExitThread);
			WaitForSingleObject(m_hThread,INFINITE);
			if(m_hThread)
				TerminateProcess(m_hThread,0);
			CloseHandle(m_hExitThread);
			CloseHandle(m_opRead.hEvent);
			CloseHandle(m_opWrite.hEvent);
			CloseHandle(m_hComm);
			m_hComm = NULL;
		}
	}

private:

	static VOID CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode,DWORD dwNumberOfBytesTransfered,LPOVERLAPPED lpOverlapped)
	{
		assert(0);
	}	
	static DWORD WINAPI ThreadProcA(LPVOID lpParameter)
	{
		CSerialPort* pThis = (CSerialPort*)lpParameter;
		HANDLE hEvents[2];
		hEvents[0] = pThis->m_hExitThread;
		hEvents[1] = pThis->m_ReadToken;		

		#define READ_TIMEOUT      500      // milliseconds
		DWORD nReadSize = 0;
		int nTimeOutTimes = 0;
		while(1)
		{	
			DWORD dResWait = WaitForMultipleObjects(2,hEvents,FALSE,INFINITE);
			if(dResWait == WAIT_OBJECT_0)
			{
				SetEvent(pThis->m_WriteToken);
				return 0;
			}
			ResetEvent(pThis->m_ReadToken);
			
			///////////////////////////////////////////////////////////////////////////////////////
			DWORD			dwCommEvent;
			DWORD			dwErr;
			COMSTAT			comStat;
			if(!::WaitCommEvent(pThis->m_hComm, &dwCommEvent, &pThis->m_opRead))
			{
				DWORD dwError = GetLastError();
				if(dwError != ERROR_IO_PENDING)
				{
					ClearCommError(pThis->m_hComm, &dwErr, &comStat);
					assert(0);
					return 0;
				}
				else
				{
					DWORD dwRes = WaitForSingleObject(pThis->m_opRead.hEvent, READ_TIMEOUT);
					if(dwRes != WAIT_OBJECT_0)
					{
						ClearCommError(pThis->m_hComm, &dwErr, &comStat);
						SetEvent(pThis->m_ReadToken);
						continue;
					}
				}
			}
			else
			{
				if((dwCommEvent & EV_RXCHAR) != EV_RXCHAR)
				{
					SetEvent(pThis->m_ReadToken);
					continue;
				}
			}
	 
			ClearCommError(pThis->m_hComm, &dwErr, &comStat);
			if(nTimeOutTimes > 10)
			{
				nTimeOutTimes = 0;
			}
			else
			{
				if(comStat.cbInQue <= 0)
				{
					Sleep(200);
					nTimeOutTimes++;
					SetEvent(pThis->m_ReadToken);
					continue;
				}
				nTimeOutTimes = 0;
				//prepare buffer by comStat.cbInQue
				char* szTemp = new char[comStat.cbInQue];
				CBufferPtr ptTempBuffer = new CBuffer(comStat.cbInQue, szTemp);
				delete [] szTemp;
				ReadFile(pThis->m_hComm, (BYTE*)ptTempBuffer->GetData(), comStat.cbInQue, &nReadSize, &pThis->m_opRead);
				assert(comStat.cbInQue == nReadSize);
				pThis->m_ptBuffer->Append(ptTempBuffer);			
				if(pThis->m_bIsOBEXMode)
				{
					if(!pThis->IsCompleteOBEXPackage(pThis->m_ptBuffer->GetLength(),(BYTE*)pThis->m_ptBuffer->GetData()))
					{
						SetEvent(pThis->m_ReadToken);
						continue;
					}
				}
				else
				{
					if(!IsCompleteATPackage(pThis->m_ptBuffer))
					{
						SetEvent(pThis->m_ReadToken);
						continue;
					}
				}
			}
			SetEvent(pThis->m_WriteToken);
		};

		return 0;
	}

	const bool WriteAndRead(const BYTE* pByteBuffer,const DWORD nSize,BYTE **ppbyteBuffer,DWORD& nRetSize, DWORD dwDelay = 0)
	{		
		nRetSize = 0;
		*ppbyteBuffer = NULL;

		WaitForSingleObject(m_WriteToken,INFINITE);
		ResetEvent(m_WriteToken);

		bool bRes = false;
		if(NULL != m_hComm)
		{
			bRes = (TRUE == WriteFileEx(m_hComm,pByteBuffer,nSize,&m_opWrite,FileIOCompletionRoutine));
			if(bRes == false)
			{
				::WaitForSingleObject(m_opWrite.hEvent,INFINITE);
				DWORD dwBytesTransferred = 0;
				bRes = (TRUE == GetOverlappedResult(m_hComm, &m_opWrite, &dwBytesTransferred, TRUE));
			}
		}

		if(bRes)
		{
			DWORD dwMilliseconds = (dwDelay > 0) ? dwDelay : INFINITE;
			SetEvent(m_ReadToken);
			if(WAIT_OBJECT_0 == WaitForSingleObject(m_WriteToken,dwMilliseconds))
			{
				ResetEvent(m_WriteToken);

				nRetSize = m_ptBuffer->GetLength();
				if(nRetSize > 0)
				{
					*ppbyteBuffer = new BYTE[nRetSize];
					memcpy(*ppbyteBuffer,m_ptBuffer->GetData(),nRetSize);
				}				
				m_ptBuffer = new CBuffer;//reset share buffer
			}
		}

		SetEvent(m_WriteToken);		
		assert((dwDelay == 0 && nRetSize > 0) || (dwDelay > 0));

		return nRetSize > 0;
	}
public:
	static const bool DecodeATPackage(const CBufferPtr& ptBuffer,CBufferPtr& ptReceiverBuffer)
	{
		bool bRes = false;
		if(IsCompleteATPackage(ptBuffer))
		{			
			BYTE* pTemp = (BYTE*)ptBuffer->GetData();
			string strBuffer;
			int nSteps = 0;
			int nTempCount=0;
			BYTE bytePrev = 0;
			for(ULONGLONG i = 0;i<(ptBuffer->GetLength()-6);i++)
			{
				nTempCount++;
				if(pTemp[i] == 0x0a && bytePrev == 0x0d)
				{
					if(nSteps == 1)
					{
						assert((i-nTempCount+1) >= 0 && (i-nTempCount+1) < ptBuffer->GetLength());
						if((i-nTempCount+1) >= 0 && (i-nTempCount+1) < ptBuffer->GetLength())
							ptReceiverBuffer = new CBuffer(nTempCount-2,(const char*)&(pTemp[i-nTempCount+1]));
						bRes = true;
						break;
					}
					nSteps++;
					nTempCount = 0;
				}				
				bytePrev = pTemp[i];
			}
		}
		return bRes;
	}

	static const bool IsCompleteATPackage(const CBufferPtr& ptBuffer)
	{
		bool bRes = false;
		BYTE szATEnd[6] = {0x0d,0x0a,'O','K',0x0d,0x0a};
		if(ptBuffer->GetLength() > 6)
		{
			BYTE* pTemp = (BYTE*)ptBuffer->GetData();
			BYTE* pEnd = pTemp + (ptBuffer->GetLength()-6);
			bRes = memcmp(pEnd,szATEnd,6) == 0;
		}
		return bRes;
	}

	static const bool IsCompleteOBEXPackage( DWORD dwDataSize,BYTE* pData )
	{
		bool bRes = false;
		if( dwDataSize > 3 )
		{
			DWORD dwPackageSize = MAKEWORD(pData[2],pData[1]);
			bRes = dwDataSize == dwPackageSize;
		}
		return bRes;
	}
private:
	HANDLE					m_hComm;
	DCB						m_dcb;
	COMMTIMEOUTS			m_CommTimeouts;
	BOOL					m_bPortReady;	
	OVERLAPPED				m_opRead;
	OVERLAPPED				m_opWrite;
	TSTRING					m_portname;
	bool					m_bIsOBEXMode;
	CBufferPtr				m_ptBuffer;
	HANDLE					m_ReadToken;
	HANDLE					m_WriteToken;
	HANDLE					m_hThread;
	HANDLE					m_hExitThread;
};

};