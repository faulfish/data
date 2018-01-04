#pragma once

#include "Requestbase.h"
#include <vector>
#include <string>

#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif

const int g_nDEFAULT_QUERY_SIMDATA_RELATED = 60000;
const int g_nDEFAULT_CMD_TIME_OUT		   = 20000;

using namespace std;
namespace PLATFORM_TOOL
{
	class CDiagRequestBase  : public CRequestBase
	{
	public:		
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	protected:
		CDiagRequestBase(int nTimeout = 5000,bool bHadReturnvalue = true);
		CDiagRequestBase(const TSTRING& strName,int nTimeout = 5000,bool bHadReturnvalue = true);
		virtual ~CDiagRequestBase();
	protected:
		bool Encode(const char* szCommand,int nCommandLen,CBufferPtr& ptBufferSend);
	private:		
		CBufferPtr m_ptTempBufferReceived;
	};

	class RM_CLASS_API CEventRequest  : public CDiagRequestBase
	{
	public:
		CEventRequest(bool bEnable);
		virtual ~CEventRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	};

	class RM_CLASS_API CAskDLDIAGModeRequest  : public CRequestBase
	{
	public:
		CAskDLDIAGModeRequest(bool& bIsDiagMode,bool& bIsDLMode,bool& bIsATMode);
		virtual ~CAskDLDIAGModeRequest();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bIsDiagMode;
		bool& m_bIsDLMode;
		bool& m_bIsATMode;
		CBufferPtr ptBufferReceived;
	};

	class RM_CLASS_API CDS2DIAGRequest  : public CRequestBase
	{
	public:
		CDS2DIAGRequest();
		virtual ~CDS2DIAGRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	};

	class RM_CLASS_API CDIAG2DL6280Request  : public CRequestBase
	{
	public:
		CDIAG2DL6280Request();
		virtual ~CDIAG2DL6280Request();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	};

	///////////////////////////////////////////////////////	
	class RM_CLASS_API CCloseLog1Request  : public CRequestBase
	{
	public:
		CCloseLog1Request();
		virtual ~CCloseLog1Request();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	};
	class RM_CLASS_API CCloseLog2Request  : public CRequestBase
	{
	public:
		CCloseLog2Request();
		virtual ~CCloseLog2Request();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	};
	class RM_CLASS_API CCloseLog3Request  : public CRequestBase
	{
	public:
		CCloseLog3Request();
		virtual ~CCloseLog3Request();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	};
	//////////////////////////////////////////////////////////////////////////
	class RM_CLASS_API CUnknown1Request  : public CRequestBase
	{
	public:
		CUnknown1Request();
		virtual ~CUnknown1Request();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	};
	class RM_CLASS_API CUnknown2Request  : public CRequestBase
	{
	public:
		CUnknown2Request();
		virtual ~CUnknown2Request();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	};
	class RM_CLASS_API CUnknown3Request  : public CRequestBase
	{
	public:
		CUnknown3Request();
		virtual ~CUnknown3Request();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	};
	///////////////////////////////////////////////////////
	class RM_CLASS_API CDiagAskStateRequest  : public CDiagRequestBase
	{
	public:
		CDiagAskStateRequest(bool& bCanBeChanged2DL);
		virtual ~CDiagAskStateRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bCanBeChanged2DL;
	};
	///////////////////////////////////////////////////////
	class RM_CLASS_API CDIAGGetSWVersionRequest  : public CDiagRequestBase
	{
	public:
		CDIAGGetSWVersionRequest();
		virtual ~CDIAGGetSWVersionRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	};

}