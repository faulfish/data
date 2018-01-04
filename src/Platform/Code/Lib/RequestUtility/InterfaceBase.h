#ifndef __INTERFACE_BASE_H__
#define __INTERFACE_BASE_H__

#pragma once
#include "..\..\RequestManager\RequestManagerExport.h"
#include "..\..\RequestManager\Request\QualcommRequest.h"
#include "..\..\CommonUtil\UnicodeUtility.h"
#include "..\..\CommonUtil\LogPolicy.h"

#include <map>

#pragma warning(push)
#pragma warning(disable:4273)

class CInterfaceBase
{
protected:
	class TemporaryCloseComport
	{
	public:
		TemporaryCloseComport(CInterfaceBase* pThis,bool bKeepPotocolType = false):m_ptIRequestManager(pThis->m_ptIRequestManager)
		{
			if(m_ptIRequestManager)
				m_ptIRequestManager->Close(bKeepPotocolType);
		}
		~TemporaryCloseComport()
		{		
			if(m_ptIRequestManager)
				m_ptIRequestManager->Open();
		}
		int GetComport() const 
		{
			int nComPort = -1;
			
			if(m_ptIRequestManager)
				nComPort = m_ptIRequestManager->GetComport();

			return nComPort;
		}
	private:
		PLATFORM_TOOL::CIRequestManagerPtr& m_ptIRequestManager;
	};

	class CReOpenComport
	{
		DWORD _MessageLoop (  HANDLE* lphObjects, int     cObjects	,DWORD dwMilliseconds = INFINITE)
		{ 
			DWORD result; 

			// The message loop lasts until we get a WM_QUIT message,
			// upon which we shall return from the function.
			while (TRUE)
			{
				// block-local variable 		
				MSG msg ; 

				// Read all of the messages in this next loop, 
				// removing each message as we read it.
				while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
				{ 
					// If it is a quit message, exit.
					if (msg.message == WM_QUIT)  
						return 1; 
					// Otherwise, dispatch the message.
					::DispatchMessage(&msg); 
				} // End of PeekMessage while loop.

				// Wait for any message sent or posted to this queue 
				// or for one of the passed handles be set to signaled.
				result = ::MsgWaitForMultipleObjects(cObjects, lphObjects, FALSE, dwMilliseconds, QS_ALLINPUT); 

				// The result tells us the type of event we have.
				if (result == (WAIT_OBJECT_0 + cObjects))
				{
					// New messages have arrived. 
					// Continue to the top of the always while loop to 
					// dispatch them and resume waiting.
					continue;
				} 
				else 
				{ 
					// One of the handles became signaled. 
					break;
				} // End of else clause.
			} // End of the always while loop. 

			return result;
		} // End of function.


	public:
		CReOpenComport(CInterfaceBase* pThis,bool bKeepPotocolType = false)
			:m_ptIRequestManager(pThis->m_ptIRequestManager), m_bKeepProtocolType(bKeepPotocolType)
		{
			TCHAR szComport[50] = {0};
			::_stprintf_s(szComport,50,_T("COM%d"), GetComport());
			HANDLE hPortEvent = ::CreateEvent(NULL,TRUE,FALSE,szComport);
			ResetEvent(hPortEvent);
		}
		~CReOpenComport(){}
		void SetKeepProtocolType(bool bKeepProtocolType)
		{ 
			m_bKeepProtocolType = bKeepProtocolType;
		}
		void Sleep_ByMessageLoop(unsigned long nMinisecond)
		{
			HANDLE hSleepEvent = ::CreateEvent(NULL,TRUE,FALSE, _T("sleep"));
			ResetEvent(hSleepEvent);
			_MessageLoop(&hSleepEvent,1, nMinisecond);
		}
		void AddRestartTimeLog(const CString& strMsg)
		{
#ifdef Test_RestartUnit
			CString strFileName = _T("RestartTime.txt");
		
			TCHAR szComport[50] = {0};
			::_stprintf_s(szComport,_T("COM%d"), GetComport());

			CString strLog = szComport;
			strLog += ": ";
			strLog += strMsg;
			LogPolicy::Log(strLog, strFileName);
#endif
		}
		bool ReOepnComport(int nWaitingTime = 10000, bool bUsePNP = false, int nRetryTimes = 20, int nDelayTime = 5000)
		{
			/*
			In this function, we use the function Sleep_ByMessageLoop() to sleep everywhere.
			If we use sleep(), we will lose some events, ex: comport in.
			*/
			bool bRes = false;
			
			DWORD nReopenComPortStartTime = GetTickCount();
			DWORD nPortOutTime = -1;
			DWORD nPortInTime = -1;
			
			TCHAR szComport[50] = {0};
			::_stprintf_s(szComport,_T("COM%d"), GetComport());
			HANDLE hPortEvent = ::CreateEvent(NULL,TRUE,TRUE,szComport);
			int nIndex = 0;
			bool bPNPCOMPort = false;
			bool bPortOutMSGGet = true;
			if(m_ptIRequestManager)
			{
				if(m_ptIRequestManager->Close(m_bKeepProtocolType))
				{
					if (bUsePNP)
					{

						//Wait for port out
						TCHAR szComport[50] = {0};
						::_stprintf_s(szComport,_T("FirstCOM%d"), GetComport());
						HANDLE hFirstPortEvent = ::CreateEvent(NULL,TRUE,TRUE,szComport);
						bool bFirst = WAIT_OBJECT_0 == ::WaitForSingleObject(hFirstPortEvent,0);
						::ResetEvent(hFirstPortEvent);
						if(bFirst == false)
						{
							while(WAIT_OBJECT_0 == _MessageLoop(&hPortEvent,1, 0))
							{
								if (GetTickCount() - nReopenComPortStartTime < static_cast<DWORD>(nWaitingTime))
									Sleep_ByMessageLoop(100);
								else
								{
									bPortOutMSGGet = false;
									break;
								}
							}
							nPortOutTime = GetTickCount() - nReopenComPortStartTime;
						}

						else
						{
							AddRestartTimeLog(_T("First time to reopen comport, we won't get the port-out event."));
						}
						//Wait for comport in.
						if (bPortOutMSGGet)
							bPNPCOMPort = IsPNPCOMPortAvailable(GetComport(), nWaitingTime);
						if (bPNPCOMPort)
							nPortInTime = GetTickCount() - nReopenComPortStartTime;

						//Whether PNPComport success or not, we try to open comport.
						for(nIndex = 0; nIndex < nRetryTimes && !bRes; nIndex++)
						{
							Sleep_ByMessageLoop(nDelayTime);
							bRes = m_ptIRequestManager->Open();
						}
					}
					else
					{
						if(!bUsePNP)
							Sleep(nWaitingTime);
						
						for(nIndex=0; nIndex< nRetryTimes && !bRes; nIndex++)
						{
							Sleep(nDelayTime);
							bRes = m_ptIRequestManager->Open();
						}
					}
				}
				else
					assert(0);
			}
			DWORD nReopenComPortTime = GetTickCount() - nReopenComPortStartTime;

			//Save log to tone Reopen time, and hello retry time.
			
			
			CString strMessage;
			if(bUsePNP)
				strMessage.Format(_T("ReopenPortTime,%d,Port out time,%d,Port in time,%d,OpenRetryTimes,%d,PNP %s,Open port %s "),
					nReopenComPortTime,
					nPortOutTime,
					nPortInTime,
					nIndex,
					bPNPCOMPort ? "Success" : "Fail",
					bRes ? "Success" : "Fail");
			else
				strMessage.Format(_T("[Not UsePNP]ReopenPortTime,%d, Sleep %d, OpenRetryTimes,%d, Open port %s "),
				nReopenComPortTime,
				nWaitingTime,
				nIndex,
				bRes ? "Success" : "Fail");
			AddRestartTimeLog(strMessage);
			

			return bRes;
		}

		bool IsPNPCOMPortAvailable(int nPort,int nWaitingTime)
		{
			//If MobileFinder is not enable, assume the comport is available
			bool bAvailable = false;
			TCHAR szComport[50] = {0};
			_stprintf_s(szComport,_T("COM%d"),nPort);

			HANDLE hPortEvent = ::CreateEvent(NULL,TRUE,FALSE,szComport);
			bAvailable = WAIT_OBJECT_0 == _MessageLoop(&hPortEvent,1, nWaitingTime);

			return bAvailable;
		}
		int GetComport() const 
		{
			int nComPort = -1;

			if(m_ptIRequestManager)
				nComPort = m_ptIRequestManager->GetComport();

			return nComPort;
		}
	private:
		bool m_bKeepProtocolType;
		PLATFORM_TOOL::CIRequestManagerPtr& m_ptIRequestManager;
		//HINSTANCE m_hMobileFinderDLL;
	};
protected:
	CInterfaceBase(int nCOMPort, const std::string& strPlatform = "Qualcomm6245")
		:m_nCOMPort(nCOMPort)
		,m_strPlatform(strPlatform)
		,m_ptReceiveBuffer(new CBuffer)
		,m_ptSendBuffer(new CBuffer)
	{		
		CString strFormat;
		strFormat.Format(_T("Test_COM%02d.txt"),m_nCOMPort);
		m_strLogFile = (LPCTSTR)strFormat;		

		using namespace PLATFORM_TOOL;
		if(GetIRequestManager(m_ptIRequestManager,m_nCOMPort,m_strPlatform))
			m_ptIRequestManager->Open();
		else
			AddLog(_T("m_ptIRequestManager->Open() fail"));
	}
	virtual ~CInterfaceBase()
	{
		//2008/5/6 Alan tu
		//Don't close on destructor, due to other intsance
		//It will close byitself
		//If you should close it for some specially case, please call Close() and don't forget to reopen it.

		//if(IsConnected())
		//	m_ptIRequestManager->Close();

		// Jame's command time test
		/*for (std::map<TSTRING, RequestAnalysis>::iterator iter = m_mapRequestAnalysis.begin(); iter != m_mapRequestAnalysis.end(); ++iter)
		{
			CString strMsg;
			strMsg.Format(_T("%s Total test counts : %d times, Total time: %d clocks, average time: %f clocks"), iter->first.c_str(), iter->second.m_nRequestCount, iter->second.m_nRequestTime, ((double)iter->second.m_nRequestTime) / iter->second.m_nRequestCount);
			AddLog((TSTRING)strMsg);
		}*/
	}
	void AddLog(const TSTRING& strMessage)
	{
#ifndef DISABLE_LOG
		LogPolicy::Log(strMessage.c_str(),m_strLogFile.c_str());
#endif //DISABLE_LOG
	}

	bool IsConnected() const
	{
		bool bConnected = false;
		if(m_ptIRequestManager)
			bConnected = m_ptIRequestManager->IsOpen();
		return bConnected;
	}
	virtual bool EnableDiagEvent(bool bEnable)
	{
		using namespace PLATFORM_TOOL;

		bool bRes = false;	
		CRequestPtr ptRequest = new CEventRequest(bEnable);
		bRes = Execute(ptRequest);
		return bRes;
	}

	bool Execute(PLATFORM_TOOL::CRequestPtr& ptRequest,int nTimeout)
	{
		//DWORD nRequestStartTime = GetTickCount();
		ptRequest->SetTimeout(nTimeout);
		bool bRes = Execute(ptRequest);
		//DWORD nRequestEndTime = GetTickCount();

		//if(ptRequest->GetName().empty() == false)
		//{
		//	++(m_mapRequestAnalysis[ptRequest->GetName()].m_nRequestCount);
		//	m_mapRequestAnalysis[ptRequest->GetName()].m_nRequestTime += (nRequestEndTime - nRequestStartTime);
		//}
		return bRes;
	}

	PLATFORM_TOOL::PotocolType GetPotocolType() 
	{
		PLATFORM_TOOL::PotocolType type = PLATFORM_TOOL::NONE;
		if(IsConnected())
			type = m_ptIRequestManager->GetPotocolType();
		return  type;
	}

	bool Execute(PLATFORM_TOOL::CRequestPtr& ptRequest)
	{
		//if (ptRequest->GetName().empty() == false)
		//	AddLog(ptRequest->GetName());

		ptRequest->GetSendData(m_ptSendBuffer);  // for download log usage, please don't delete it.
		

		bool bRes = false;
		if(IsConnected())
			bRes = m_ptIRequestManager->Execuate(ptRequest,m_strPlatform);
		assert(bRes);
		if(bRes)
			bRes = ptRequest->WaitForWrite();

		//assert(bRes);
		if(bRes == false)
		{
			m_ptIRequestManager->Reset();

			TCHAR szLog[255] = {0};
			::_stprintf_s(szLog, 255, _T("InterfaceBase:: Execute %s Timeout"), ptRequest->GetName().c_str());

			AddLog(szLog);
			m_ptReceiveBuffer->SetEmpty(); // for download log usage, please don't delete it.
		}
		else
			ptRequest->GetReceiveData(m_ptReceiveBuffer); // for download log usage, please don't delete it.
	
		return bRes;
	}
	void ResetMode() // Reset Mode
	{
		m_ptIRequestManager->ResetMode();
	}

	//Utility
public:
	int HexBcdToDigit(BYTE *pucHexBcdBuf, int iHexBcdBufSize, char *sDigitBuf, int iDigitBufSize)
	{
		int nRet = 0;
		if (pucHexBcdBuf && (iHexBcdBufSize > 0) && sDigitBuf && (iDigitBufSize>0) && iHexBcdBufSize*2 <= iDigitBufSize)
		{
			BYTE ucLowByte, ucHighByte;
			int i = 0;
			for (i = 0; i < iHexBcdBufSize; i++)
			{
				ucLowByte = *(pucHexBcdBuf+i)&0x0F;
				*(sDigitBuf+(2*i)) = (ucLowByte>=10) ? ((ucLowByte-10)+'a') : (ucLowByte+'0');

				ucHighByte = (*(pucHexBcdBuf+i)&0xF0)>>4;
				*(sDigitBuf+(2*i+1)) = (ucHighByte>=10) ? ((ucHighByte-10)+'a') : (ucHighByte+'0');
			} 

			if (iHexBcdBufSize%2)
			{
				ucLowByte = *(pucHexBcdBuf+i)&0x0F;
				*(sDigitBuf+(2*i)) = (ucLowByte>=10) ? ((ucLowByte-10)+'a') : (ucLowByte+'0');
			}
			nRet = (iHexBcdBufSize%2) ? (2*i+1) : (2*i);
		}
		return nRet;
	}

	int DigitToHexBcd(char *sDigitBuf, int iDigitBufSize, BYTE *pucHexBcdBuf, int iHexBcdBufSize)
	{
		int nRet = 0;
		if(sDigitBuf && pucHexBcdBuf && iHexBcdBufSize >= (iDigitBufSize-1)/2)
		{
			for(int i=0; i< (iDigitBufSize-1)/2; i++)
			{
				TCHAR szTemp[3] = {0};
				szTemp[0] = sDigitBuf[i*2];
				szTemp[1] = sDigitBuf[i*2+1];
				pucHexBcdBuf[(iDigitBufSize-1)/2 -i -1] = (BYTE)QSYNC_LIB::HEX2Ineger(szTemp);
			}
		}
		return nRet;
	}
	unsigned int IntEndianTransfer(unsigned int usValue)
	{
		return ::ntohl(usValue);
	}
	unsigned short ShortEndianTransfer(unsigned short usValue)
	{
		return ::ntohs(usValue);
	}

public:
	void PopLastLog()
	{
		if(m_ptIRequestManager)
			m_ptIRequestManager->PopLastLog();
	}
	bool GetSendData(CBufferPtr& ptSendData)
	{
		bool bRes = false;
		if(m_ptSendBuffer)
		{
			ptSendData = m_ptSendBuffer;
			bRes = true;
		}
		return bRes;
	}
	bool GetReceiveData(CBufferPtr& ptReceiveData)
	{
		bool bRes = false;
		if(m_ptReceiveBuffer)
		{
			ptReceiveData = m_ptReceiveBuffer;
			bRes = true;
		}
		return bRes;
	}

private:
	PLATFORM_TOOL::CIRequestManagerPtr m_ptIRequestManager;
	int																m_nCOMPort;
	TSTRING															m_strLogFile;
	std::string														m_strPlatform;

	struct  RequestAnalysis
	{
		RequestAnalysis():m_nRequestCount(0), m_nRequestTime(0){}
		DWORD m_nRequestCount ;
		DWORD m_nRequestTime ;
	};

	private:
		std::map<TSTRING, RequestAnalysis> m_mapRequestAnalysis;
	private:
		CBufferPtr m_ptSendBuffer;
		CBufferPtr m_ptReceiveBuffer;
};



#endif //__INTERFACE_BASE_H__



