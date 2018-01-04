#include "stdafx.h"
#include "WirelessRequest.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "..\codec\BQPacketLib.h"
#include "..\codec\QualcommCRCChecker.h"

namespace PLATFORM_TOOL
{		
	CEnableGAPRequest::CEnableGAPRequest(bool bEnableGap,bool& bResult)
		:CDiagRequestBase(_TC("CEnableGAPRequest"),10000)
		,m_bEnableGap(bEnableGap)
		,m_bResult(bResult)
	{
		m_bResult = false;

		CBufferPtr ptBufferSend;

		//Send: 4B 03 01 00 00 A8 36 7E
		//Get:4B 03 01 00 01 21 27 7E

		//Send: 4B 03 01 00 01 21 27 7E
		//Get: 4B 03 01 00 02 BA 15 7E

		BYTE szEnableGapCmd[] = {0x4B, 0x03, 0x01, 0x00, 0x00};
		BYTE szDisableGapCmd[] = {0x4B, 0x03, 0x01, 0x00, 0x01};
		if(m_bEnableGap)
			Encode((const char*)szEnableGapCmd,5,ptBufferSend);
		else			
			Encode((const char*)szDisableGapCmd,5,ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEnableGAPRequest::~CEnableGAPRequest()
	{
	}

	bool CEnableGAPRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);;
	}

	void CEnableGAPRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		if(m_bEnableGap)
		{
			BYTE szResponseCmd[] = {0x4B, 0x03, 0x01, 0x00,0x01};
			if(ptBufferReceived->GetLength() >= 5 && 0 == ::memcmp(ptBufferReceived->GetData(),szResponseCmd,5))
				m_bResult = true;
		}
		else
		{
			BYTE szResponseCmd[] = {0x4B, 0x03, 0x01, 0x00,0x02};
			if(ptBufferReceived->GetLength() >= 5 && 0 == ::memcmp(ptBufferReceived->GetData(),szResponseCmd,5))
				m_bResult = true;
		}
		
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	
	CBTTxPowerRequest::CBTTxPowerRequest(int nDegree,bool& bResult)
		:CDiagRequestBase(_TC("CBTTxPowerRequest"))
		,m_nDegree(nDegree)
		,m_bResult(bResult)
	{
		m_bResult = false;

		CBufferPtr ptBufferSend;

		BYTE szHighPowerCmd[] = {0x4B, 0x03, 0x0E, 0x00, 0xF0};
		BYTE szMiddlePowerCmd[] = {0x4B, 0x03, 0x0E, 0x00, 0xF1};
		BYTE szLowPowerCmd[] = {0x4B, 0x03, 0x0E, 0x00, 0xF2};

		/*
		2. TX power - high
		Send: 4B 03 0E 00 F0 E0 8B 7E
		Get: 4B 03 0E 00 00 6F 7C 7E

		3. TX power - middle
		Send: 4B 03 0E 00 F1 69 9A 7E
		Get: 4B 03 0E 00 00 6F 7C 7E

		4. TX power - low
		Send: 4B 03 0E 00 F2 F2 A8 7E
		Get: 4B 03 0E 00 00 6F 7C 7E
		*/

		if(nDegree == 0xF0)
			Encode((const char*)szHighPowerCmd,5,ptBufferSend);
		else if(nDegree == 0xF1)
			Encode((const char*)szMiddlePowerCmd,5,ptBufferSend);
		else if(nDegree == 0xF2)
			Encode((const char*)szLowPowerCmd,5,ptBufferSend);
		else
			assert(0);//Not define
		SetSendData(ptBufferSend);
	}

	CBTTxPowerRequest::~CBTTxPowerRequest()
	{
	}

	bool CBTTxPowerRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	void CBTTxPowerRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		BYTE szResponseCmd[] = {0x4B, 0x03, 0x0E,0x00, 0x00};
		if(ptBufferReceived->GetLength() >= 5 && 0 == ::memcmp(ptBufferReceived->GetData(),szResponseCmd,5))
			m_bResult = true;
		
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CBTTestModeRequest::CBTTestModeRequest(bool bEnetr,bool& bResult)
		:CDiagRequestBase(_TC("CBTTestModeRequest"))
		,m_bEnetr(bEnetr)
		,m_bResult(bResult)
	{
		m_bResult = false;

		CBufferPtr ptBufferSend;

		BYTE szEnterTestCmd[] = {0x4B, 0x03, 0x0D, 0x00, 0xDD};
		/*
		000159: Bulk or Interrupt Transfer (UP), 20.11.2008 15:58:18.5917989 +0.0
		Pipe Handle: 0x899be7b4 (Endpoint Address: 0x4)
		Send 0x8 bytes to the device:
		4B 03 0D 00 DD 63 9E 7E
		000160: Bulk or Interrupt Transfer (UP), 20.11.2008 15:58:18.5917989 +0.0
		Pipe Handle: 0x899be794 (Endpoint Address: 0x84)
		Get 0x7 bytes from the device:
		4B 03 0D 00 60 75 7E
		*/
		BYTE szLeaveTestCmd[] = {0x4B, 0x03, 0x0D, 0x00, 0xDE};

		if(bEnetr)
			Encode((const char*)szEnterTestCmd,5,ptBufferSend);
		else 
			Encode((const char*)szLeaveTestCmd,5,ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CBTTestModeRequest::~CBTTestModeRequest()
	{
	}

	bool CBTTestModeRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	void CBTTestModeRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		if(ptBufferReceived->GetLength() >= 0)
			m_bResult = true;
		
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	
	CBTBound::CBTBound(bool bBound,const char* szBTAddress,const char* szBTPass,bool& bIsCmdSuccess)
		:CWriteBTHeadsetAddress(szBTAddress,strlen(szBTAddress),bIsCmdSuccess)		
	{
		/*
		Bond device	"SendRawRequest 75 3 3 0 42 169 22 83 00 214 0 0 0 8 0 0 0 48 48 48 48 0 0 0 0 0 0 0 0 0 0 0 0 4 0
		RequestItem ""Bluetooth Req SC"" 42 5445289 214 8 ""0000"" 4 0"
		Debond device	"SendRawRequest 75 3 3 0 43 169 22 83 00 214 0 0 0 8 0 0 0 48 48 48 48 0 0 0 0 0 0 0 0 0 0 0 0 4 0
		RequestItem ""Bluetooth Req SC"" 43 5445289 214 8 ""0000"" 4 0"

		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 03 for Bluetooth command
		BT_REQ_MSG	2	BT request message ¡V BT_REQ_MSG is set to 02 00 for SD command
		BT_CMD_TYPE	1	BT command type ¡V determines the actual action to do
		LOCAL_DEVICE_NAME	51	The local device name
		BD_ADDR_LAP	4	Bluetooth device address ¡V LAP part
		BD_ADDR_UAP	4	Bluetooth device address ¡V UAP part
		BD_ADDR_NAP	4	Bluetooth device address ¡V NAP part
		SECOND_TO_SEARCH	2	Second to search
		MAX_RESPONSE	1	Max number of devices to search
		CLASS_OF_DEVICE	4	Class of device
		DISCOVERABILITY	1	Setting of discoverability mode
		*/
		const char* c = szBTAddress;
		int n = strlen(szBTAddress);
		char sendBuf[ 5 + 12 + 16 + 2] = 
		{
			0x4b
			,0x03
			,0x03
			,0x00
			,42
			,HexToInt(c[n-1]) + HexToInt(c[n-2])* 16
			,HexToInt(c[n-3]) + HexToInt(c[n-4])* 16
			,HexToInt(c[n-5]) + HexToInt(c[n-6])* 16
			,0
			,HexToInt(c[n-7]) + HexToInt(c[n-8])* 16
			,0
			,0
			,0
			,HexToInt(c[n-9]) + HexToInt(c[n-10])* 16
			,HexToInt(c[n-11]) + HexToInt(c[n-12])* 16
			,0
			,0
			,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0	
			,0
			,0
		};

		if(bBound)
			sendBuf[4] = 42;//Bound
		else
			sendBuf[4] = 43;//Debound

		int nPassLen = strlen(szBTPass);
		if(nPassLen <= 16)
		{
			sendBuf[33] = nPassLen;
			for(int nIndex = 0;nIndex < nPassLen;nIndex++)
			{
				sendBuf[17 + nIndex] = szBTPass[nIndex];
			}
		}

		CBufferPtr ptBufferSend;
		Encode(sendBuf, 5 + 12 + 16 + 2, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CBTBound::~CBTBound()
	{
	}
	
	void CBTBound::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 03 for Bluetooth command
		BT_REQ_MSG	2	BT request message ¡V BT_REQ_MSG is set to 02 00 for SD command
		*/		
		const char* data = ptBufferReceived->GetData();
		if(ptBufferReceived->GetLength() >= 4 && data[2] == 0x03 && data[3] == 0x00)
			m_bIsCmdSuccess = true;
		else
			m_bIsCmdSuccess = false;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CBTRequest::CBTRequest(UINT nCommand,const char* szBTAddress,bool& bIsCmdSuccess)
		:CWriteBTHeadsetAddress(szBTAddress,szBTAddress ? strlen(szBTAddress) : 0,bIsCmdSuccess)		
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 03 for Bluetooth command
		BT_REQ_MSG	2	BT request message ¡V BT_REQ_MSG is set to 0B 00 for HFG command
		BT_CMD_TYPE	1	BT command type ¡V determines the actual action to do
		BD_ADDR_LAP	4	Bluetooth device address ¡V LAP part
		BD_ADDR_UAP	4	Bluetooth device address ¡V UAP part
		BD_ADDR_NAP	4	Bluetooth device address ¡V NAP part
		CONNECTION_ID	1	Connection ID
		SPEAKER_GAIN	1	Speaker gain
		MIC_GAIN	1	Microphone gain
		LOOP	1	Loop
		SUPPORTED_FEATURE	2	Supported features

		SendRawRequest 75 3 11 0 180 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 37 0
		RequestItem "Bluetooth Req HFG" 180 0 0 0 0 0 0 0 37
		SendRawRequest 75 3 11 0 181 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		RequestItem "Bluetooth Req HFG" 181 0 0 0 0 0 0 0 0
		*/
		const char* c = szBTAddress;
		int n = szBTAddress ? strlen(szBTAddress) :  0;		
		char sendBuf[ 5 + 12 + 6] = 
		{
			0x4b
			,0x03
			,0x0B
			,0x00
			,nCommand
			,n == 0 ? 0 : HexToInt(c[n-1]) + HexToInt(c[n-2])* 16
			,n == 0 ? 0 : HexToInt(c[n-3]) + HexToInt(c[n-4])* 16
			,n == 0 ? 0 : HexToInt(c[n-5]) + HexToInt(c[n-6])* 16
			,0
			,n == 0 ? 0 : HexToInt(c[n-7]) + HexToInt(c[n-8])* 16
			,0
			,0
			,0
			,n == 0 ? 0 : HexToInt(c[n-9]) + HexToInt(c[n-10])* 16
			,n == 0 ? 0 : HexToInt(c[n-11]) + HexToInt(c[n-12])* 16
			,0
			,0
			,0
			,0
			,0
			,0
		};

		if(nCommand == 180)
		{
			sendBuf[21] = 37;
		}
		else if(nCommand == 181)
		{
			sendBuf[21] = 0;
		}
		else if(nCommand == 190)
		{
			sendBuf[20] = (szBTAddress == NULL) ? 1 : 0;
		}

		CBufferPtr ptBufferSend;
		Encode(sendBuf, 5 + 12 + 6, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CBTRequest::~CBTRequest()
	{
	}
	
	void CBTRequest::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 03 for Bluetooth command
		BT_REQ_MSG	2	BT request message ¡V BT_REQ_MSG is set to 02 00 for SD command
		*/		
		const char* data = ptBufferReceived->GetData();
		if(ptBufferReceived->GetLength() >= 4 && data[0] == 0x4B && data[1] == 0x03 && data[2] == 0x0B)
			m_bIsCmdSuccess = true;
		else
			m_bIsCmdSuccess = false;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	//////////////////////////////////////////////////////////////////////////
	// This class is used for write BT address to mobile
	//////////////////////////////////////////////////////////////////////////
	CWriteBTHeadsetAddress::CWriteBTHeadsetAddress(const char* BTAddress, int length,bool& bIsCmdSuccess) 
		:CDiagRequestBase(_T("CWriteBTHeadsetAddress"))
		,m_pBTAddress(NULL)
		,m_nBTAddressLength(length)
		,m_bIsCmdSuccess(bIsCmdSuccess)
	{
		m_pBTAddress = new char[length];
		memset(m_pBTAddress, 0, sizeof(char)*length);
		memcpy(m_pBTAddress, BTAddress, sizeof(char)*length);

		char* c = m_pBTAddress;
		int n = m_pBTAddress ? strlen(m_pBTAddress) :  0;	
		char sendBuf[17] = 
		{
			0x4b
			,0x03
			,0x0f
			,0x00
			,0xfa
			,n == 0 ? 0 : HexToInt(c[n-1]) + HexToInt(c[n-2])* 16
			,n == 0 ? 0 : HexToInt(c[n-3]) + HexToInt(c[n-4])* 16
			,n == 0 ? 0 : HexToInt(c[n-5]) + HexToInt(c[n-6])* 16
			,0
			,n == 0 ? 0 : HexToInt(c[n-7]) + HexToInt(c[n-8])* 16
			,0
			,0
			,0
			,n == 0 ? 0 : HexToInt(c[n-9]) + HexToInt(c[n-10])* 16
			,n == 0 ? 0 : HexToInt(c[n-11]) + HexToInt(c[n-12])* 16
			,0
			,0
		};

		CBufferPtr ptBufferSend;
		Encode(sendBuf, 17, ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CWriteBTHeadsetAddress::~CWriteBTHeadsetAddress() 
	{
		if(m_pBTAddress != NULL)
			delete [] m_pBTAddress;
		m_pBTAddress = NULL;
	}
	void CWriteBTHeadsetAddress::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{
		const char* data = ptBufferReceived->GetData();
		if(data[4] == 1)
			m_bIsCmdSuccess = true;
		else
			m_bIsCmdSuccess = false;
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CWriteBTHeadsetAddress::CheckCmdResult() 
	{
		return m_bIsCmdSuccess;
	}

	int CWriteBTHeadsetAddress::HexToInt(char c) 
	{
		if(c >= '0' && c <= '9') return c - '0';
		if(c >= 'A' && c <= 'F') return c - 'A' + 10;
		if(c >= 'a' && c <= 'f') return c - 'a' + 10;
		return 0;
	}

	/*
	FM test mode
	*/

	CFMRequest::CFMRequest(WORD nTestCase,WORD nPayload1,WORD nPayload2,WORD& nRSSI,bool& bResult)
		:CDiagRequestBase(_TC("CFMRequest"))
		,m_nTestCase(nTestCase)
		,m_nPayload1(nPayload1)
		,m_nPayload2(nPayload2)
		,m_nRSSI(nRSSI)
		,m_bResult(bResult)
	{		
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			WORD command;
			WORD testCase;
			WORD payload1;
			WORD payload2;
		} diag_FM_req;
#pragma pack(pop)

		//diag_FM_req req = {75,69,1};
		diag_FM_req req = {75,249,1};//For qualcomm's change codbase, 2009/2/17 alan tu
		req.testCase = m_nTestCase;
		req.payload1 = m_nPayload1;
		req.payload2 = m_nPayload2;

		CBufferPtr ptBufferSend;
		Encode((const char*)&req,10,ptBufferSend);
		SetSendData(ptBufferSend);

		m_bResult = false;
	}

	CFMRequest::~CFMRequest()
	{
	}

	bool CFMRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	void CFMRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			WORD command;
			WORD result;
		} diag_FM_resp;
#pragma pack(pop)

		diag_FM_resp resp = {0};
		memcpy(&resp,ptBufferReceived->GetData(),sizeof(diag_FM_resp));
		if(m_nTestCase == 20)
		{
			m_nRSSI = resp.result;
			m_bResult = true;
		}
		else
			m_bResult = resp.result == 1;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}	
}
