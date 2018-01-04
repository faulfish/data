// RequestManager.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "RequestManager.h"
#include "Request\QualcommRequest.h"
#include <cassert>
#include "..\CommonUtil\cloud.h"

namespace PLATFORM_TOOL
{
	CRequestManager::CRequestManager(void)
	{}

	CRequestManager::~CRequestManager(void)
	{
	}
	
	void CRequestManager::AddRef(int nPort)
	{
		COMMAP_Iter Iter;
		if(GetCOMLibrary(nPort,Iter))
		{
			::InterlockedIncrement(&Iter->second->m_lRefCount);
		}
	}

	void CRequestManager::ReleaseRef(int nPort)
	{
		COMMAP_Iter Iter;
		if(GetCOMLibrary(nPort,Iter))
		{
			::InterlockedDecrement(&Iter->second->m_lRefCount);
			if(Iter->second->m_lRefCount == 0)
				m_mapCOM.erase(Iter);
		}
	}

	void CRequestManager::AddLog(int nPort,const TSTRING& strMsg)
	{
		TCHAR szComport[50] = {0};
		::_stprintf(szComport,_T("COM%d_LASTERROR"),nPort);
		CCloud::Put(szComport,static_cast<int>(strMsg.size()*sizeof(TCHAR)),(void*)(strMsg.c_str()));
	}

	

	bool CRequestManager::Open(int nPort)
	{
		bool bRes = false;
		COMMAP_Iter Iter;
		if(GetCOMLibrary(nPort,Iter))
		{			
			SimapleLock lock(Iter->second->csCOM);

			bRes = Iter->second->bIsConnected;
			if(bRes == false)
			{
				TCHAR szComport[50] = {0};
				::_stprintf(szComport,_T("\\\\.\\COM%d"),nPort);
				bRes = RUN_SUCCESSFULLY == Iter->second->ptCOMLibrary->OpenComport(_T(""),szComport,true);
				if(bRes == false)
					AddLog(nPort,_T("CRequestManager::Open::Opencomport Fail"));
				assert(bRes);//Should it need to wait for device ?
				Iter->second->bIsConnected = bRes;
			}
		}
		return bRes;
	}

	bool CRequestManager::Close(int nPort,bool bKeepPotocolType)
	{
		bool bRes = false;
		COMMAP_Iter Iter;
		if(GetCOMLibrary(nPort,Iter))
		{
			SimapleLock lock(Iter->second->csCOM);
			
			bRes = Iter->second->bIsConnected == false;
			if(bRes == false)
			{
				Iter->second->ptCOMLibrary->CloseComport();
				if(bKeepPotocolType == false)
					Iter->second->nPotocolType = NONE;
				bRes = true;
				Iter->second->bIsConnected = false;
			}
		}
		return bRes;
	}

	bool CRequestManager::IsOpen(int nPort)
	{
		bool bRes = false;
		COMMAP_Iter Iter;
		if(GetCOMLibrary(nPort,Iter))
		{
			SimapleLock lock(Iter->second->csCOM);

			bRes = Iter->second->bIsConnected;
		}
		return bRes;
	}

	void CRequestManager::Reset(int nPort)
	{
		COMMAP_Iter Iter;
		if(GetCOMLibrary(nPort,Iter))
		{
			SimapleLock lock(Iter->second->csCOM);
			//Iter->second->nPotocolType = NONE;
			Iter->second->ptCOMLibrary->ResetRequestMode();
		}
	}
	void CRequestManager::ResetMode(int nPort)
	{
		COMMAP_Iter Iter;
		if(GetCOMLibrary(nPort,Iter))
		{
			SimapleLock lock(Iter->second->csCOM);
			Iter->second->nPotocolType = NONE;
		}
	}

	bool CRequestManager::InternalExecuateProbe(SafeCOMLibrayPtr& ptCOMLib,CRequestPtr& ptRequest)
	{
		ptCOMLib->ptCOMLibrary->ResetRequestMode();
		return ptCOMLib->ptCOMLibrary->ExecuateProbe(ptRequest);
	}

	bool CRequestManager::InternalExecuate(SafeCOMLibrayPtr& ptCOMLib,CRequestPtr& ptRequest)
	{
		return ptCOMLib->ptCOMLibrary->ExecuateRequest(ptRequest);
	}

	bool CRequestManager::Execuate(int nPort,CRequestPtr& ptRequest,const std::string& strPlatform)
	{
		bool bRes = false;
		COMMAP_Iter Iter;
		if(GetCOMLibrary(nPort,Iter) && Iter->second->bIsConnected)
		{
			//1. Lock 
			SimapleLock lock(Iter->second->csCOM);

			//2. Makesure in the correctly potocol and the execuate the command
			if(SwitchPotocol(Iter->second,ptRequest,strPlatform))
				bRes = InternalExecuate(Iter->second,ptRequest);
		}
		return bRes;
	}

	bool CRequestManager::GetMode(SafeCOMLibrayPtr& ptCOMLib,bool& bIsDiagMode,bool& bIsDLMode,bool& bIsATMode)
	{
		bool bRes = false;

		bIsDiagMode = false;
		bIsDLMode = false;
		bIsDLMode = false;

		int nMaxRetry = 3;
		while(nMaxRetry-- > 0 && bRes == false)
		{
			CRequestPtr ptProbeRequest = new CAskDLDIAGModeRequest(bIsDiagMode,bIsDLMode,bIsATMode);
			if(InternalExecuateProbe(ptCOMLib,ptProbeRequest))
			{
				if(ptProbeRequest->WaitForWrite())
				{
					bRes = bIsDiagMode || bIsDLMode || bIsATMode;
					if(bIsDiagMode)
						ptCOMLib->nPotocolType = DIAG;
					else if(bIsDLMode)
						ptCOMLib->nPotocolType = DL;
					else if(bIsATMode)
						ptCOMLib->nPotocolType = AT;
					else
					{
						AddLog(ptCOMLib->ptCOMLibrary->GetComPort(),_T("CRequestManager::GetMode::CheckMode Fail"));
						bRes = false;
					}
				}
				else
				{
					AddLog(ptCOMLib->ptCOMLibrary->GetComPort(),_T("CRequestManager::GetMode::Timeout Fail"));
					//Need to retry or change timeout or other issue
				}
			}
			else
			{
				AddLog(ptCOMLib->ptCOMLibrary->GetComPort(),_T("CRequestManager::GetMode::Execuate Fail"));
			}
		};//End of retry

		return bRes;
	}

	PotocolType CRequestManager::GetPotocolType(int nPort)
	{
		PotocolType type = NONE;

		COMMAP_Iter Iter;
		if(GetCOMLibrary(nPort,Iter) && Iter->second->bIsConnected)
		{
			type = Iter->second->nPotocolType;
			if(type == NONE)
			{
				bool bIsDiagMode = false;
				bool bIsDLMode = false;
				bool bIsATMode = false;
				if(GetMode(Iter->second,bIsDiagMode,bIsDLMode,bIsATMode))
				{
					if(bIsDiagMode)
						type = DIAG;
					else if(bIsDLMode)
						type = DL;
					else if(bIsATMode)
						type = AT;
				}
			}
		}
		return type;
	}

	bool CRequestManager::SwitchPotocol(SafeCOMLibrayPtr& ptCOMLib,CRequestPtr& ptRequest,const std::string& strPlatform)
	{
		bool bRes = false;

		if(ptCOMLib->nPotocolType == NONE)
		{//Need to ask which mode is.
			bool bIsDiagMode = false;
			bool bIsDLMode = false;
			bool bIsATMode = false;
			if(GetMode(ptCOMLib,bIsDiagMode,bIsDLMode,bIsATMode))
			{
				if(bIsDiagMode)
					ptCOMLib->nPotocolType = DIAG;
				else if(bIsDLMode)
					ptCOMLib->nPotocolType = DL;
				else if(bIsATMode)
					ptCOMLib->nPotocolType = AT;
			}
			else
			{
				assert(0);//Need to check reason
				//Need to retry or change timeout or other issue
			}
		}

		if(ptRequest->GetPotocolType() != ptCOMLib->nPotocolType)
		{
			switch(ptRequest->GetPotocolType())
			{
			case NONE:
				//Do nothing
				bRes = true;
				break;
			case DIAG:
				assert(0);//Need to implement
				//ptCOMLib->ptCOMLibrary->MakesureToDIAGMode();
				//ptCOMLib->nPotocolType = DIAG;
				bRes = false;
				break;
			case DL:
				if("Qualcomm6245" == strPlatform)
					bRes = SwitchDL_Qualcomm6245(ptCOMLib);
				else if(("Qualcomm6240" == strPlatform) || ("Qualcomm6270" == strPlatform))
					bRes = SwitchDL_Qualcomm6240(ptCOMLib);
				else assert(0);//Need to support the platform
				break;
			case QLIB:
				//ptCOMLib->nPotocolType = QLIB;
				bRes = false;
				assert(0);//Need to implement
				break;
			case AT:
				//ptCOMLib->nPotocolType = AT;
				bRes = false;
				assert(0);//Need to implement
				break;
			default:
				bRes = false;
				assert(0);//Need to implement
				break;
			};
		}
		else bRes = true;

		return bRes;
	}

	bool CRequestManager::SwitchDL_Qualcomm6245(SafeCOMLibrayPtr& ptCOMLib)
	{
		bool bRes = false;
		Sleep(100);

		//Close log first
		CRequestPtr ptRequest1 = new CCloseLog1Request();
		bRes = InternalExecuate(ptCOMLib,ptRequest1);
		assert(bRes);
		if(bRes)
			bRes = ptRequest1->WaitForWrite();
		assert(bRes);

		Sleep(100);
		//20080704 happen fail for [bRes = InternalExecuate(ptCOMLib,ptRequest2);]

		CRequestPtr ptRequest2 = new CCloseLog2Request();
		
		bRes = InternalExecuate(ptCOMLib,ptRequest2);
		assert(bRes);
		if(bRes)
			bRes = ptRequest2->WaitForWrite();
		assert(bRes);

		Sleep(100);

		CRequestPtr ptRequest3 = new CCloseLog3Request();
		
		bRes = InternalExecuate(ptCOMLib,ptRequest3);
		assert(bRes);
		if(bRes)
			bRes = ptRequest3->WaitForWrite();
		assert(bRes);

		Sleep(100);

		//Change 2 DL
		CRequestPtr ptRequestChangeDL = new CDIAG2DL6280Request();
		
		bRes = InternalExecuate(ptCOMLib,ptRequestChangeDL);
		assert(bRes);
		if(bRes)
			bRes = ptRequestChangeDL->WaitForWrite();
		assert(bRes);

		int nRetryTimes = 60;
		if(bRes)
		{
			bool bIsDiagMode = false;
			bool bIsDLMode = false;
			bool bIsATMode = false;
			do
			{
				Sleep(500);
			}
			while(GetMode(ptCOMLib,bIsDiagMode,bIsDLMode,bIsATMode) == false && nRetryTimes--); //It needs amount 5s. I set the timeout as 30s.
		}
		return bRes;
	}
	bool CRequestManager::SwitchDL_Qualcomm6240(SafeCOMLibrayPtr& ptCOMLib)
	{
		bool bRes = false;
		Sleep(100);

		//CRequestPtr ptRequest1 = new CUnknown1Request(); // 0x41,0x30,0x30,0x30,0x30,0x30,0x30,0xDF,0x8A,0x7E
		//bRes = InternalExecuate(ptCOMLib,ptRequest1);
		//if(bRes)
		//	bRes = ptRequest1->WaitForWrite();
	

		//Sleep(100);
		CRequestPtr ptRequest2 = new CUnknown2Request(); //0x29,0x01,0x00,0x31,0x40,0x7E // Mode Change Request message 0x01,0x00 =Offline Digital mode
		bRes = InternalExecuate(ptCOMLib,ptRequest2);
		if(bRes)
			bRes = ptRequest2->WaitForWrite();
		

		Sleep(100);

		CRequestPtr ptRequest3 = new CUnknown3Request(); //0x0C,0x14,0x3A,0x7E
		bRes = InternalExecuate(ptCOMLib,ptRequest3);
		
		if(bRes)
			bRes = ptRequest3->WaitForWrite();
		
		Sleep(100);

		bool bCanChange2DLMode = false;
		for(int i=0; i<10000 && !bCanChange2DLMode; i++)
		{
			CRequestPtr ptRequest = new CDiagAskStateRequest(bCanChange2DLMode);
			if(InternalExecuate(ptCOMLib, ptRequest))
				bRes = ptRequest->WaitForWrite();
		}

		//Change 2 DL_SECURE_BOOT_2
		if(bCanChange2DLMode)
		{
			CRequestPtr ptRequestChangeDL = new CDIAG2DL6280Request(); // 0x3A,0xA1,0x6E,0x7E
			
			bRes = InternalExecuate(ptCOMLib,ptRequestChangeDL);
			//assert(bRes);
			//Reopen comport
			/************************************************************************/
			/* It costs 15s to wait unit HW resetting and comport re-coming out     */
			/* Sleep 10s + close then wait 5s										*/
			/************************************************************************/
			if(bRes)
				Sleep(20000); // wait for the handset HW reset and comport re-coming out
			if(bRes)
			{
				int nComPort = ptCOMLib->ptCOMLibrary->GetComPort();
				if(Close(nComPort))
				{
					bRes = false;
					for(int i=0; i<3 && !bRes; i++)
					{
						Sleep(5000);
						bRes = Open(nComPort);
						AddLog(nComPort, _T("Switchto6240DL: Close+OpenComPort"));
					}	
				}
				else
					bRes = false;
			}
		}
		else
			bRes = false;
		
		if(bRes)
			ptCOMLib->nPotocolType = DL;

		return bRes;
	}

	bool CRequestManager::GetCOMLibrary(int nPort,COMMAP_Iter& Iter)
	{
		bool bRes = false;
		Iter = m_mapCOM.find(nPort);
		if(Iter == m_mapCOM.end())
		{
			SafeCOMLibrayPtr ptLib = new SafeCOMLibray;
			ptLib->ptCOMLibrary = new CSerCOMLibrary;
			m_mapCOM.insert(COMMAP_Pair(nPort,ptLib));

			//Find again
			Iter = m_mapCOM.find(nPort);
			bRes = (Iter != m_mapCOM.end());
		}
		else
		{
			bRes  = true;
		}
		return bRes;
	}
	bool CRequestManager::GetLastError(int nPort, LONG& nErrorCode)
	{
		bool bRes = false;
		COMMAP_Iter Iter = m_mapCOM.find(nPort);
		if (Iter != m_mapCOM.end())
		{
			Iter->second->ptCOMLibrary->GetLastError();
		}
		return bRes;
	}

	void CRequestManager::PopLastLog(int nPort)
	{
		COMMAP_Iter Iter = m_mapCOM.find(nPort);
		if (Iter != m_mapCOM.end())
		{
			Iter->second->ptCOMLibrary->PopLastLog();
		}
	}
}