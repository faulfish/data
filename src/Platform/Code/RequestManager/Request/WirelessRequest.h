#include "QualcommRequest.h"

#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif

namespace PLATFORM_TOOL
{
	class RM_CLASS_API CWriteBTHeadsetAddress : public CDiagRequestBase 
	{
    public:
        CWriteBTHeadsetAddress(const char* BTAddress, int length,bool& bIsCmdSuccess);
        virtual ~CWriteBTHeadsetAddress();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        int HexToInt(char c);
        bool CheckCmdResult();
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived) {
            return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
        }
	protected:
        bool& m_bIsCmdSuccess;
    private:
        char* m_pBTAddress;
        int m_nBTAddressLength;
    };
	
    class RM_CLASS_API CBTBound : public CWriteBTHeadsetAddress 
	{
    public:
        CBTBound(bool bBound,const char* szBTAddress,const char* szBTPass,bool& bIsCmdSuccess);
        virtual ~CBTBound();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived) 
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
    };

    class RM_CLASS_API CBTRequest : public CWriteBTHeadsetAddress 
	{
    public:
        CBTRequest(UINT nCommand,const char* szBTAddress,bool& bIsCmdSuccess);
        virtual ~CBTRequest();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived) 
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
    };

	/*
	1. GAP command
	Commands used to test basic Bluetooth functionality.
	Request packet
	Field 	Length (bytes)	Description
	CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
	SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 03 for Bluetooth command
	BT_REQ_MSG	2	BT request message ¡V BT_REQ_MSG is set to 01 00 for GAP command
	BT_CMD_TYPE	1	BT command type ¡V determines the actual action to do
	Response packet
	No response packet
	*/
	class RM_CLASS_API CEnableGAPRequest  : public CDiagRequestBase
	{
	public:
		CEnableGAPRequest(bool bEnableGap,bool& bResult);
		virtual ~CEnableGAPRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bResult;
		bool m_bEnableGap;
	};
	
	/*
	13. TXPOWER command
	Commands used to test TX power.
	Request packet
	Field 	Length (bytes)	Description
	CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
	SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 03 for Bluetooth command
	BT_REQ_MSG	2	BT request message ¡V BT_REQ_MSG is set to 0E 00 for TXPOWER command
	BT_CMD_TYPE	1	BT command type ¡V determines the actual action to do
	Response packet
	Field 	Length (bytes)	Description
	CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
	SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 03 for Bluetooth command
	BT_REQ_MSG	2	BT request message ¡V BT_REQ_MSG is set to 0E 00 for TXPOWER command
	err	1	Error code
	*/
	class RM_CLASS_API CBTTxPowerRequest  : public CDiagRequestBase
	{
	public:
		CBTTxPowerRequest(int nDegree,bool& bResult);
		virtual ~CBTTxPowerRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		int m_nDegree;
		bool& m_bResult;
	};

	/*
	Bluetooth test mode
	*/
	class RM_CLASS_API CBTTestModeRequest  : public CDiagRequestBase
	{
	public:
		CBTTestModeRequest(bool bEnetr,bool& bResult);
		virtual ~CBTTestModeRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bResult;
		bool m_bEnetr;
	};

	
	/*
	FM test mode
	*/
	class RM_CLASS_API CFMRequest  : public CDiagRequestBase
	{
	public:
		CFMRequest(WORD nTestCase,WORD nPayload1,WORD nPayload2,WORD& nRSSI,bool& bResult);
		virtual ~CFMRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bResult;
		WORD m_nTestCase;
		WORD m_nPayload1;
		WORD m_nPayload2;
		WORD& m_nRSSI;
	};
}
