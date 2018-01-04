#pragma once
#include "Request\Requestbase.h"
#include "../odmtma_api/SerCOMLib/SerCOMLibrary.h"
#include <map>
#include "..\CommonUtil\SimpleCriticalSection.h"
#include "..\CommonUtil\RefObject.h"

namespace PLATFORM_TOOL
{
	class CRequestManager
	{
		class SimapleLock
		{
		public:
			SimapleLock(CSimpleCriticalSection& csCOM):m_csCOM(csCOM){csCOM.Lock();}
			~SimapleLock(){m_csCOM.Unlock();}
		private:
			CSimpleCriticalSection& m_csCOM;
		};
		class SafeCOMLibray  : public CRefObject
		{
			friend class CRequestManager;
		protected:
			SafeCOMLibray():bIsConnected(false),nPotocolType(NONE),m_lRefCount(0){}
			virtual ~SafeCOMLibray()
			{
				/*
				It will close COM port by itself lastly
				*/
				if(bIsConnected)
					ptCOMLibrary->Close();
			}
			CSerCOMLibraryPtr ptCOMLibrary;
			CSimpleCriticalSection csCOM;
			bool	 bIsConnected;
			PotocolType nPotocolType;		
			long m_lRefCount;
		};
	public:
		CRequestManager(void);
		virtual ~CRequestManager(void);
		void AddRef(int nPort);
		void ReleaseRef(int nPort);
	public:		
		bool Open(int nPort);
		bool Close(int nPort,bool bKeepPotocolType = false);
		bool IsOpen(int nPort);
		bool Execuate(int nPort,CRequestPtr& prRequest,const TSTRING& strPlatform);
		void Reset(int nPort);
		void ResetMode(int nPort);
		PotocolType GetPotocolType(int nPort);
		bool GetLastError(int nPort, LONG& nErrorCode);
		void PopLastLog(int nPort);
	private:		
		typedef CRefObjectPtr<SafeCOMLibray> SafeCOMLibrayPtr;
		std::map<int,SafeCOMLibrayPtr>     m_mapCOM;
		typedef std::map<int,SafeCOMLibrayPtr>::iterator COMMAP_Iter;
		typedef std::map<int,SafeCOMLibrayPtr>::const_iterator COMMAP_CIter;
		typedef std::pair <int, SafeCOMLibrayPtr> COMMAP_Pair;
	protected:
		bool GetCOMLibrary(int nPort,COMMAP_Iter& Iter);
		bool SwitchDL_Qualcomm6240(SafeCOMLibrayPtr& ptCOMLib);
		bool SwitchDL_Qualcomm6245(SafeCOMLibrayPtr& ptCOMLib);
		bool IsPNPCOMPortAvailable(int nPort,int nDelayTime);
	private:
		//Don't change this function,InternalExecuate(), to protected or public
		//It's not safe for any other class
		bool InternalExecuateProbe(SafeCOMLibrayPtr& ptCOMLib,CRequestPtr& ptRequest);
		bool InternalExecuate(SafeCOMLibrayPtr& ptCOMLib,CRequestPtr& ptRequest);
		bool SwitchPotocol(SafeCOMLibrayPtr& ptCOMLib,CRequestPtr& ptRequest,const std::string& strPlatform);
		bool GetMode(SafeCOMLibrayPtr& ptCOMLib,bool& bIsDiagMode,bool& bIsDLMode,bool& bIsATMode);
		void AddLog(int nPort,const TSTRING& strMsg);
	};
}