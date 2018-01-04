#pragma once

#include "Requestbase.h"
#include <vector>
#include <string>
#include "..\..\FactoryTool\SPU\ISPU_CALLBACK.h"
#include <map>

#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif

using namespace std;
namespace PLATFORM_TOOL
{
	class CATBase  : public CRequestBase
	{
	public:		
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	protected:
		CATBase(const TSTRING& strName,int nTimeout = 5000,bool bHadReturnvalue = true);
		virtual ~CATBase();
	protected:
		bool IsOK(CBufferPtr& ptBuffer);
		bool IsFAIL(CBufferPtr& ptBuffer);
		bool Encode(const char* szCommand,int nCommandLen,CBufferPtr& ptBufferSend);
	private:		
		CBufferPtr m_ptTempBufferReceived;
	};

	class RM_CLASS_API CATEnableKeypadEventRequest  : public CATBase
	{
	public:		
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool WaitForWrite();
		virtual bool WaitForOtherResponse();
	protected:
		bool CheckBreak();
		bool Timeout(int dwMilliseconds);
	public:
		CATEnableKeypadEventRequest(bool bEnable,bool& bSuccess,int nTimeout,ISPU_CALLBACK *pISPU_CALLBACK);
		virtual ~CATEnableKeypadEventRequest();
	private:		
		bool& m_bSuccess;
		bool m_bOK;
		//ISPU_CALLBACK *m_pISPU_CALLBACK;
		//std::map<std::string,bool> m_strKeyPressMap;
		bool m_bWaitForOtherResponse;
	};

}