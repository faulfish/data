#include "stdafx.h"
#include "DownloadRequest.h"
#include "..\codec\BQPacketLib.h"
#include "..\codec\QualcommCRCChecker.h"


namespace PLATFORM_TOOL
{
	bool ByteToChar(BYTE *pucHexBcdBuf, int iHexBcdBufSize, CStringA& strBuffer)
	{
		bool bRes = false;
		if (pucHexBcdBuf && (iHexBcdBufSize > 0))
		{
			for(int i=0; i< iHexBcdBufSize; i++)
			{
				CStringA strTemp;
				strTemp.Format("%02X ",pucHexBcdBuf[i]);
				strBuffer+=strTemp;
				if(i!=0 && (i+1)%18 ==0 )
					strBuffer += "\r\n";
			}
			bRes = true;
		}
		return bRes;
	}

	CDownloadRequestBase::CDownloadRequestBase(bool bHadReturnvalue)
		:CRequestBase(DL, 5000, bHadReturnvalue), 
		m_nErrorCode(DL_ERROR_SUCCESS), m_bRunSuccess(false),
		m_ptTempBufferReceived(new CBuffer)
	{
	}

	CDownloadRequestBase::CDownloadRequestBase(const TSTRING& strName,PotocolType nPotocolType,int	nTimeout ,bool bHadReturnvalue)
		:CRequestBase(strName, nPotocolType, nTimeout, bHadReturnvalue), 
		m_nErrorCode(DL_ERROR_SUCCESS), m_bRunSuccess(false),
		m_ptTempBufferReceived(new CBuffer)
	{
	}

	CDownloadRequestBase::~CDownloadRequestBase(){}

	bool CDownloadRequestBase::Encode(const char* szCommand,int nCommandLen,CBufferPtr& ptBufferSend, bool bSpeedup)
	{
		bool bRes = false;

		char chStart = START_CHAR;
		char chEnd = END_CHAR;	   //0x7E;
		void *pBuffer = NULL;
		int nPackedLen = 0;
		
		if(bSpeedup)
			nPackedLen = CBQPacketLib::SpeedUpPackBuffer( szCommand, nCommandLen, chStart, chEnd, &pBuffer);
		else
			nPackedLen =  CBQPacketLib::PackBuffer( szCommand, nCommandLen, chStart, chEnd, &pBuffer);
		
		ptBufferSend = new CBuffer(nPackedLen,(const char*)pBuffer);		
		delete[] pBuffer;

		bRes = nPackedLen > 0;
		assert(bRes);
		return bRes;
	}

	bool CDownloadRequestBase::Tx(const char* szCommand,int nCommandLen, bool bSpeedUp)
	{
		bool bRes = false;
		CBufferPtr ptBufferSend = new CBuffer;
		if(Encode(szCommand,nCommandLen,ptBufferSend,bSpeedUp))
		{		
			SetSendData(ptBufferSend);
			bRes = true;
		}
		else
			m_nErrorCode = DL_ERROR_TX_ERROR;
		return bRes;
	}


	bool CDownloadRequestBase::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = false;
		m_ptTempBufferReceived->Append(ptBufferReceived);

		char *szBuffer = const_cast<char *>(m_ptTempBufferReceived->GetData());		
		QualcommCRCChecker CRCChecker(true);
		for ( DWORD j = 0 ; j < m_ptTempBufferReceived->GetLength(); j ++)
		{
			int nRxLen = CRCChecker.Push(szBuffer[j]);
			if ( nRxLen > 0)
			{
				bRes = true;
				ptBufferReceived = new CBuffer(nRxLen, (const char*)CRCChecker.GetBuffer());
				m_ptTempBufferReceived->SetEmpty();
				break;
			}
		}
		return bRes;
	}
	bool CDownloadRequestBase::GetReceiveData(CBufferPtr& ptBufferReceived)
	{
		CRequestBase::GetReceiveData(ptBufferReceived);
		return GetProcessResult();
	}
	
	int CDownloadRequestBase::GetLastError()
	{
		return m_nErrorCode;
	}
	void CDownloadRequestBase::SetLastError(int nError)
	{
		m_nErrorCode = nError;
	}
	bool CDownloadRequestBase::GetProcessResult()
	{
		return m_bRunSuccess;
	}
	void CDownloadRequestBase::SetProcessResult(bool bResult)
	{
		m_bRunSuccess = bResult;
	}
//*******************************************************************//		

	//CDLRAWRequest Request
	CDLRAWRequest::CDLRAWRequest(const char* szCmdInput, int nCmdLength, bool bAddCRC)
		:CDownloadRequestBase(false)
	{
		if (bAddCRC)
		{
			CBufferPtr ptBufferSend = new CBuffer;
			Encode(szCmdInput, nCmdLength,ptBufferSend);
			SetSendData(ptBufferSend);
		}
		else
		{
			CBufferPtr ptBufferSend = new CBuffer(nCmdLength, szCmdInput);
			SetSendData(ptBufferSend);
		}
	}
	CDLRAWRequest::~CDLRAWRequest()
	{
	}

	//*******************************************************************//		

	//Send dl raw request with returned value
	CDLRawWithReturnRequest::CDLRawWithReturnRequest(const char* szCmdInput, int nCmdLength, bool bAddCRC)
	{
		if (bAddCRC)
		{
			CBufferPtr ptBufferSend = new CBuffer;
			Encode(szCmdInput, nCmdLength,ptBufferSend);
			SetSendData(ptBufferSend);
		}
		else
		{
			CBufferPtr ptBufferSend = new CBuffer(nCmdLength, szCmdInput);
			SetSendData(ptBufferSend);
		}
	}
	CDLRawWithReturnRequest::~CDLRawWithReturnRequest()
	{
	}
	void CDLRawWithReturnRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		SetProcessResult(true);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}

	//*******************************************************************//	
	CDLBootLoaderRevacationRequest::CDLBootLoaderRevacationRequest(int& nStatus,int& nVersion):m_nStatus(nStatus),m_nVersion(nVersion)
	{
		char szBuf[1] = {0x14};
		CBufferPtr ptBufferSend = new CBuffer;
		Encode((char*)szBuf,sizeof(szBuf),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CDLBootLoaderRevacationRequest::~CDLBootLoaderRevacationRequest()
	{
	}

	bool CDLBootLoaderRevacationRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return true;
	}

	void CDLBootLoaderRevacationRequest::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{
		/*
		Field	Field Length (bits)	Description
		Command (0x15)	8	Command identifier code ¡V The host shall set this field to 0x15.
		Status	8	The status of revocation baseline read
		0: Version read succeed
		1: OTP is damaged
		2: OTP is empty
		Boot loader secure version	8 	An uint8 variable represent the boot loader¡¦s secure version
		*/
		
		
		struct ResRevaction
		{
			unsigned char nDiagCmd;
			unsigned char nStatus;
			unsigned char nVersion;
		} Result;

		bool bRes = false;
		char *szBuffer = const_cast<char *>(ptBufferReceived->GetData());		
		QualcommCRCChecker CRCChecker(false);
		for ( DWORD j = 0 ; j < ptBufferReceived->GetLength(); j ++)
		{
			int nRxLen = CRCChecker.Push(szBuffer[j]);
			if ( nRxLen > 0)
			{
				::memcpy(&Result,szBuffer,nRxLen);		
				bRes = true;
				break;
			}
		}
		
		m_nStatus = Result.nStatus;
		m_nVersion = Result.nVersion;
		SetProcessResult(true);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}

	//*******************************************************************//	
	CDLPhoneCodeRevacationRequest::CDLPhoneCodeRevacationRequest(int& nStatus,int& nVersion):m_nStatus(nStatus),m_nVersion(nVersion)
	{
		char szBuf[1] = {0x16};
		CBufferPtr ptBufferSend = new CBuffer;
		Encode((char*)szBuf,sizeof(szBuf),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CDLPhoneCodeRevacationRequest::~CDLPhoneCodeRevacationRequest()
	{
	}

	bool CDLPhoneCodeRevacationRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return true;
	}
	void CDLPhoneCodeRevacationRequest::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{
		/*
		Field	Field Length (bits)	Description
		Command (0x17)	8	Command identifier code ¡V The host shall set this field to 0x15.
		Status	8	The status of revocation baseline read
		0: Version read succeed
		1: OTP is damaged
		2: OTP is empty
		Boot loader secure version	8 	An uint8 variable represent the boot loader¡¦s secure version
		*/
		
		struct ResRevaction
		{
			unsigned char nDiagCmd;
			unsigned char nStatus;
			unsigned char nVersion;
		} Result;

		bool bRes = false;
		char *szBuffer = const_cast<char *>(ptBufferReceived->GetData());		
		QualcommCRCChecker CRCChecker(false);
		for ( DWORD j = 0 ; j < ptBufferReceived->GetLength(); j ++)
		{
			int nRxLen = CRCChecker.Push(szBuffer[j]);
			if ( nRxLen > 0)
			{
				::memcpy(&Result,szBuffer,nRxLen);		
				bRes = true;
				break;
			}
		}
		
		m_nStatus = Result.nStatus;
		m_nVersion = Result.nVersion;
		SetProcessResult(true);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//	

	CDLCheckBadBlockRequest::CDLCheckBadBlockRequest(int nOption)
	{
		BYTE nCheckBadBlock[2] = {0};
		nCheckBadBlock[0] = 0x99;
		nCheckBadBlock[1] = nOption;
		CDownloadRequestBase::Tx((const char*) &nCheckBadBlock, sizeof(nCheckBadBlock));
	}
	CDLCheckBadBlockRequest::~CDLCheckBadBlockRequest(){}
	void CDLCheckBadBlockRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		SetProcessResult(true);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//	
	CDLRepeatBadBlockTestRequest::CDLRepeatBadBlockTestRequest(BYTE* szCmdInput, int nCmdLength, CallBackInfo AddMsg)
		:m_AddMsg(AddMsg)
	{
		if(szCmdInput)
		{
			CDownloadRequestBase::Tx((const char*)szCmdInput, nCmdLength, false);
			
		}
		
	}
	CDLRepeatBadBlockTestRequest::~CDLRepeatBadBlockTestRequest()
	{
	}
	void CDLRepeatBadBlockTestRequest::AddMsg(const TSTRING& strMsg)
	{
		if(m_AddMsg)
		{	
			m_AddMsg(strMsg);
		}
	}
	
	bool CDLRepeatBadBlockTestRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = false;
		if(CDownloadRequestBase::IsCompletePacket(ptBufferReceived))
		{
			BYTE* lpData = (BYTE*)(ptBufferReceived->GetData());
			if(lpData[0] == 0x02) // End
			{
				bRes = true;
				TSTRING strMsg = _T("End: ");
				int nTimes = 0; 
				memcpy(&nTimes, lpData+1, 4);
				TCHAR szTimes[100] = {0};
				_itot_s(nTimes, szTimes, 10);
				strMsg += szTimes;
				AddMsg(strMsg+_T(" times"));
			}
			else if(lpData[0] == 0x00) // Success
			{
				TSTRING strMsg = _T("Success: ");
				int nTimes = 0; 
				memcpy(&nTimes, lpData+1, 4);
				TCHAR szTimes[100] = {0};
				_itot_s(nTimes, szTimes, 10);
				strMsg += szTimes;
				
				AddMsg(strMsg+_T(" times"));
			}
			else if(lpData[0] == 0x01) // Fail
			{
				TSTRING strMsg = _T("Fail:");
				int nTimes = 0; 
				memcpy(&nTimes, lpData+1, 4);
				TCHAR szTimes[100] = {0};
				_itot_s(nTimes, szTimes, 10);
				strMsg += szTimes;
				AddMsg(strMsg+_T(" times"));
			}
			//else if(lpData[0] == 0x03) // dump data
			//{
			//	
			//}
			else
			{
				
				AddMsg(_T("Unknown Response(without 7E and CRC):"));
				CStringA strData;
				if(ByteToChar((BYTE*)ptBufferReceived->GetData(), ptBufferReceived->GetLength(), strData))
				{	
#ifdef _UNICODE
					std::string strTemp = strData;
					m_AddMsg(QSYNC_LIB::GetTSTRING(strTemp));
#else
					AddMsg((std::string)strData);
#endif
					
				}
				
			}
		}
		return bRes;
	}
	void CDLRepeatBadBlockTestRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		SetProcessResult(true);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//	
	CDLQueryHWVariant::CDLQueryHWVariant(int& nHWID, int& nVariantID):m_nHWID(nHWID), m_nVariantID(nVariantID)
	{
		BYTE nCmd = 0x18;
		CBufferPtr ptBufferSend = new CBuffer;
		CDownloadRequestBase::Tx((const char*)&nCmd, sizeof(nCmd), false);
	}
	CDLQueryHWVariant::~CDLQueryHWVariant(){}
	bool CDLQueryHWVariant::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = false;
		m_ptTempBufferReceived->Append(ptBufferReceived);

		char *szBuffer = const_cast<char *>(m_ptTempBufferReceived->GetData());		
		QualcommCRCChecker CRCChecker(false);
		for ( DWORD j = 0 ; j < m_ptTempBufferReceived->GetLength(); j ++)
		{
			int nRxLen = CRCChecker.Push(szBuffer[j]);
			if ( nRxLen > 0)
			{
				bRes = true;
				ptBufferReceived = new CBuffer(nRxLen, (const char*)CRCChecker.GetBuffer());
				m_ptTempBufferReceived->SetEmpty();
				break;
			}
		}
		return bRes;
	}
	void CDLQueryHWVariant::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		DLQueryHWVariant_RESP resp = {0};
		if(ptBufferReceived->GetLength() > sizeof(resp))
			memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
		else
			memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		if(resp.command_code == 0x19)
		{
			m_nHWID = resp.hw_id;
			m_nVariantID = resp.varinat_id;
		}
		SetProcessResult(resp.command_code == 0x19);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//	
	CDMSSNop::CDMSSNop(): CDownloadRequestBase(_T("CDMSSNop"), DL)
	{
		BYTE nDloadNopCmd = DMSS_CMD_NOP;
		CDownloadRequestBase::Tx((const char*) &nDloadNopCmd, sizeof(nDloadNopCmd));
	}
	CDMSSNop::~CDMSSNop(){}
	void CDMSSNop::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		DmssACKandNACKResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nNopResp == DMSS_CMD_ACK);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//	
	CDMSSParameter::CDMSSParameter(): CDownloadRequestBase(_T("CDMSSParameter"), DL)
	{
		DmssParaReq req = {0};
		req.nDloadCmd = DMSS_CMD_PREQ;
		CDownloadRequestBase::Tx((const char*) &req, sizeof(req));
	}
	CDMSSParameter::~CDMSSParameter(){}
	void CDMSSParameter::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		DmssPraResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == DMSS_CMD_PARAMS);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//	
	CDMSSWrite32Bit::CDMSSWrite32Bit(BYTE* lpWData, WORD dwWLength, DWORD dwAddress): CDownloadRequestBase(_T("CDMSSWrite32Bit"), DL)
	{
		DmssWrite32bitAddressReq req = {0};
		req.nDloadCmd = DMSS_CMD_WRITE32BITADDRESS;
		DWORD dwtemp=dwAddress;
		for (int i=0;i<4;i++)
		{
			req.nsAddress[3-i] = dwtemp%256;
			dwtemp=dwtemp/256;
		}
		WORD wtemp=dwWLength;
		for (int i=0;i<2;i++)
		{
			req.nsLength[1-i]=wtemp%256;
			wtemp=wtemp/256;
		}
		memcpy(req.nsData,lpWData,dwWLength);
		CDownloadRequestBase::Tx((const char*) &req, dwWLength+7);
	}
	CDMSSWrite32Bit::~CDMSSWrite32Bit(){}
	void CDMSSWrite32Bit::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		DmssACKandNACKResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nNopResp == DMSS_CMD_ACK);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	CDMSSGo::CDMSSGo(WORD nwSegment,WORD nwoffset): CDownloadRequestBase(_T("CDMSSNop"), DL)
	{
		DmssGOReq req = {0};
		req.nDloadCmd = DMSS_CMD_GO;
		WORD wtemp = nwSegment;
		for (int i=0;i<2;i++)
		{
			req.nsSegment[1-i]=wtemp%256;
			wtemp=wtemp/256;
		}
		wtemp=nwoffset;
		for (int i=0;i<2;i++)
		{
			req.nsOffset[1-i]=wtemp%256;
			wtemp=wtemp/256;
		}
		CDownloadRequestBase::Tx((const char*) &req, sizeof(req));
	}
	CDMSSGo::~CDMSSGo(){}
	void CDMSSGo::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		DmssACKandNACKResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nNopResp == DMSS_CMD_ACK);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	CStreamingDLHello::CStreamingDLHello(bool bSpeedUp) : CDownloadRequestBase(_T("CStreamingDLHello"), DL), m_bSpeedUp(bSpeedUp)
	{
		StreamingHelloParmReq req = {0};
		req.nCmdCode = m_bSpeedUp ? Streaming_CMD_SpeedUp_HELLO : Streaming_CMD_HELLO;
		memcpy(req.nMagicNumber,"QCOM fast download protocol host",32);
		req.nVersion = 0x03;
		req.nCVersion = 0x03;
		req.nFeaturebit = 0x09;
		CDownloadRequestBase::Tx((const char*) &req, sizeof(req));
	}
	CStreamingDLHello::~CStreamingDLHello(){}
	bool CStreamingDLHello::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingHelloParmResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}	
			if(resp.nCmdCode == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLHello::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingHelloParmResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		if(m_bSpeedUp)
			SetProcessResult(resp.nCmdCode == Streaming_CMD_SpeedUp_HELLORESP);
		else
			SetProcessResult(resp.nCmdCode == Streaming_CMD_HELLORESP);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	CStreamingDLHello_CheckBadBlock::CStreamingDLHello_CheckBadBlock(bool& bIsHexSupported) 
		: CDownloadRequestBase(_T("CStreamingDLHello_CheckBadBlock"), DL)
		, m_bFirstResponse(true)
		, m_hSecondResEvent(::CreateEvent(NULL,TRUE,FALSE,NULL))
		, m_bWaitForOtherResponse(true)
		, m_bIsHexSupported(bIsHexSupported)
	{
		::ResetEvent(m_hSecondResEvent);
		//memset(&m_FirstResponse, 0, sizeof(StreamingHelloParmResp));

		m_bIsHexSupported = true;
		StreamingHelloParmReq req = {0};
		req.nCmdCode = Streaming_CMD_SpeedUp_CheckBadBlock_HELLO;
		memcpy(req.nMagicNumber, "QCOM fast download protocol host", 32);
		req.nVersion = 0x03;
		req.nCVersion = 0x03;
		req.nFeaturebit = 0x09;
		CDownloadRequestBase::Tx((const char*) &req, sizeof(req));
	}
	CStreamingDLHello_CheckBadBlock::~CStreamingDLHello_CheckBadBlock(){}
	bool CStreamingDLHello_CheckBadBlock::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			if (m_bFirstResponse)
			{
				StreamingHelloParmResp resp = {0};
				if(ptBufferReceived && ptBufferReceived->GetData())
				{
					if(ptBufferReceived->GetLength() > sizeof(StreamingHelloParmResp))
						memcpy(&resp, ptBufferReceived->GetData(), sizeof(StreamingHelloParmResp));
					else
						memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
				}	
				if(resp.nCmdCode == Streaming_CMD_LOG)
					bRes = false;
			}
			else
			{
				m_bWaitForOtherResponse = false;
				bRes = true;
			}
		}
		return bRes;
	}

	bool CStreamingDLHello_CheckBadBlock::WaitForWrite()
	{
		bool bRes = CDownloadRequestBase::WaitForWrite();
		if (bRes)
		{
			if(m_bWaitForOtherResponse)
			{
				if(m_hSecondResEvent)
					bRes = WAIT_OBJECT_0 == ::WaitForSingleObject(m_hSecondResEvent, 60000);
			}
		}
		return bRes;
	}

	void CStreamingDLHello_CheckBadBlock::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		if (m_bFirstResponse)
		{
			StreamingHelloParmResp resp = {0};
			if(ptBufferReceived->GetLength() > sizeof(resp))
				memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
			else
				memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());

			if (resp.nCmdCode == Streaming_CMD_SpeedUp_CheckBadBlock_HELLORESP_1st)
				m_bFirstResponse = false;
			else
			{
				m_bWaitForOtherResponse = false;
				if(resp.nCmdCode == Streaming_CMD_ERROR)
					m_bIsHexSupported = false;
			}
			CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
			SetProcessResult(resp.nCmdCode == Streaming_CMD_SpeedUp_CheckBadBlock_HELLORESP_1st);
		}
		else
		{
			StreamingHelloParm_CheckBadBlock2ndResp resp = {0};
			if(ptBufferReceived->GetLength() > sizeof(resp))
				memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
			else
				memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());

			SetProcessResult(resp.nCmdCode == Streaming_CMD_SpeedUp_CheckBadBlock_HELLORESP_2nd);
			if (resp.nCmdCode == Streaming_CMD_SpeedUp_CheckBadBlock_HELLORESP_2nd)
			{
				::SetEvent(m_hSecondResEvent);
			}
		}
	}
	//*******************************************************************//
	CStreamingDLSecurity::CStreamingDLSecurity(BYTE nMode, bool bSpeedUp): CDownloadRequestBase(_T("CStreamingDLSecurity"), DL)
	{
		StreamingSecurityReq req = {0};
		req.nDloadCmd = Streaming_CMD_SECMODE;
		req.nMode = nMode;
		CDownloadRequestBase::Tx((const char*) &req, sizeof(req), bSpeedUp);
	}
	CStreamingDLSecurity::~CStreamingDLSecurity(){}
	bool CStreamingDLSecurity::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingSecurityResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}	
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLSecurity::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		StreamingSecurityResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_SECMODER);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//

	CStreamingDLPartition::CStreamingDLPartition(BYTE* lpbyData, WORD wnlength, bool bOverride, BYTE& nStatus, bool bSpeedUp) 
		: CDownloadRequestBase(_T("CStreamingDLPartition"), DL), m_nRespStatus(nStatus)
	{
		StreamingPartitionReq req = {0};
		req.nDloadCmd = Streaming_CMD_PATTTIONT;
		req.nOverrive = bOverride ? 0x1 : 0x0;
		memcpy(req.nPartition,lpbyData,wnlength);
		CDownloadRequestBase::Tx((const char*) &req, wnlength+2, bSpeedUp);
	}
	CStreamingDLPartition::~CStreamingDLPartition(){}
	bool CStreamingDLPartition::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingPartitionResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLPartition::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingPartitionResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_PATTTIONTR);
		m_nRespStatus = resp.nStatus;/*0 = Partition table accepted*/
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	CStreamingDLOpenMultiImage::CStreamingDLOpenMultiImage(BYTE nType, BYTE* lpData, WORD wnLength, bool bSpeedUp): CDownloadRequestBase(_T("CStreamingDLOpenMultiImage"), DL)
	{
		StreamingOpenmimageReq req = {0};
		req.nDloadCmd = Streaming_CMD_OPENMIMAGE;
		req.nType = nType;
		memcpy(req.nData,lpData,wnLength);
		CDownloadRequestBase::Tx((const char*) &req, wnLength+2, bSpeedUp);
	}
	CStreamingDLOpenMultiImage::~CStreamingDLOpenMultiImage(){}
	bool CStreamingDLOpenMultiImage::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingOpenmimageResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLOpenMultiImage::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingOpenmimageResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_OPENEDMIMAGE);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	CStreamingDLStrWrite::CStreamingDLStrWrite(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp) : m_dwAddress(dwAddress)
	{
		StreamingStrWriteReq req = {0};
		req.nDloadCmd = Streaming_CMD_STRWRITE;
		memcpy(req.nAddress,&dwAddress,sizeof(dwAddress));
		m_dwAddress = dwAddress;
		memcpy(req.nData,lpData,wnLength);
		CDownloadRequestBase::Tx((const char*) &req, wnLength+5, bSpeedUp);
	}
	CStreamingDLStrWrite::~CStreamingDLStrWrite(){}
	bool CStreamingDLStrWrite::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingStrWriteResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLStrWrite::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingStrWriteResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_BOLCKWRI && memcmp(resp.nAddress,&m_dwAddress,sizeof(m_dwAddress)) == 0);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}

	//*******************************************************************//
	CStreamingDLStrWriteWithXOR::CStreamingDLStrWriteWithXOR(BYTE* lpData, WORD wnLength/*max=1024*/, DWORD dwAddress,  Struct_XOR& nXOR) 
		:CDownloadRequestBase(_T("CStreamingDLStrWriteWithXOR"), DL), m_dwAddress(dwAddress), m_nXOR(nXOR)
	{
		StreamingStrWriteReq req = {0};
		req.nDloadCmd = Streaming_CMD_STRWRITE;
		memcpy(req.nAddress,&dwAddress,sizeof(dwAddress));
		m_dwAddress = dwAddress;
		memcpy(req.nData,lpData,wnLength);
		
		CBufferPtr ptBufferSend = new CBuffer;
		if(Encode((const char*)&req,wnLength+5,ptBufferSend))
		{		
			SetSendData(ptBufferSend);
		}
	}
	CStreamingDLStrWriteWithXOR::~CStreamingDLStrWriteWithXOR(){}
	bool CStreamingDLStrWriteWithXOR::Encode(const char* szCommand,int nCommandLen,CBufferPtr& ptBufferSend)
	{
		bool bRes = false;

		char chStart = START_CHAR;
		char chEnd = END_CHAR;	   //0x7E;
		void *pBuffer = NULL;
		int nPackedLen = 0;

		nPackedLen = CBQPacketLib::SpeedUpPackBufferWithXOR( szCommand, nCommandLen, chStart, chEnd, &pBuffer, (BYTE*)m_nXOR.nXOR);		

		ptBufferSend = new CBuffer(nPackedLen,(const char*)pBuffer);		
		delete[] pBuffer;

		bRes = nPackedLen > 0;
		assert(bRes);
		return bRes;
	}
	bool CStreamingDLStrWriteWithXOR::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingStrWriteResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLStrWriteWithXOR::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingStrWriteResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_BOLCKWRI && memcmp(resp.nAddress,&m_dwAddress,sizeof(m_dwAddress)) == 0);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}

	//*******************************************************************//
	CWriteDumpImage512::CWriteDumpImage512(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp) : m_dwAddress(dwAddress)
	{
		StreamingStrWriteDumpReq req = {0};
		req.nDloadCmd = Streaming_CMD_WRITEDUMPIMAGE;
		memcpy(req.nAddress,&dwAddress,sizeof(dwAddress));
		m_dwAddress = dwAddress;
		memcpy(req.nData,lpData,wnLength);
		CDownloadRequestBase::Tx((const char*) &req, sizeof(req), bSpeedUp);
	}
	CWriteDumpImage512::~CWriteDumpImage512(){}
	void CWriteDumpImage512::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingStrWriteResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_BOLCKWRI && memcmp(resp.nAddress,&m_dwAddress,sizeof(m_dwAddress)) == 0);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	CWriteDumpImage::CWriteDumpImage(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp) : m_dwAddress(dwAddress)
	{
		StreamingStrWriteDump_NormalReq req = {0};
		req.nDloadCmd = Streaming_CMD_WRITEDUMPIMAGE;
		memcpy(req.nAddress,&dwAddress,sizeof(dwAddress));
		m_dwAddress = dwAddress;
		memcpy(req.nData,lpData,wnLength);
		CDownloadRequestBase::Tx((const char*) &req, sizeof(req), bSpeedUp);
	}
	CWriteDumpImage::~CWriteDumpImage(){}
	void CWriteDumpImage::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingStrWriteResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_BOLCKWRI && memcmp(resp.nAddress,&m_dwAddress,sizeof(m_dwAddress)) == 0);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//

	CStreamingDLStrWrite512WithXOR::CStreamingDLStrWrite512WithXOR(BYTE* lpData, WORD wnLength/*max=512*/, DWORD dwAddress,  Struct_XOR& nXOR) 
		: m_dwAddress(dwAddress), m_nXOR(nXOR)
	{
		StreamingStrWriteDumpReq req = {0};
		req.nDloadCmd = Streaming_CMD_STRWRITE;
		memcpy(req.nAddress,&dwAddress,sizeof(dwAddress));
		m_dwAddress = dwAddress;
		memcpy(req.nData,lpData,wnLength);

		CBufferPtr ptBufferSend = new CBuffer;
		if(Encode((const char*)&req,sizeof(req),ptBufferSend))
		{		
			SetSendData(ptBufferSend);
		}
	}
	CStreamingDLStrWrite512WithXOR::~CStreamingDLStrWrite512WithXOR(){}
	bool CStreamingDLStrWrite512WithXOR::Encode(const char* szCommand,int nCommandLen,CBufferPtr& ptBufferSend)
	{
		bool bRes = false;

		char chStart = START_CHAR;
		char chEnd = END_CHAR;	   //0x7E;
		void *pBuffer = NULL;
		int nPackedLen = 0;

		nPackedLen = CBQPacketLib::SpeedUpPackBufferWithXOR( szCommand, nCommandLen, chStart, chEnd, &pBuffer, (BYTE*)m_nXOR.nXOR);		

		ptBufferSend = new CBuffer(nPackedLen,(const char*)pBuffer);		
		delete[] pBuffer;

		bRes = nPackedLen > 0;
		assert(bRes);
		return bRes;
	}
	bool CStreamingDLStrWrite512WithXOR::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingStrWriteResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLStrWrite512WithXOR::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingStrWriteResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_BOLCKWRI && memcmp(resp.nAddress,&m_dwAddress,sizeof(m_dwAddress)) == 0);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	CStreamingDLCloseImage::CStreamingDLCloseImage(bool bSpeedUp): CDownloadRequestBase(_T("CStreamingDLCloseImage"), DL)
	{
		StreamingCloseReq req = {0};
		req.nDloadCmd = Streaming_CMD_CLOSE;
		CDownloadRequestBase::Tx((const char*) &req, sizeof(req), bSpeedUp);
	}
	CStreamingDLCloseImage::~CStreamingDLCloseImage(){}
	bool CStreamingDLCloseImage::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingCloseResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLCloseImage::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingCloseResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_CLOSED);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
	//*******************************************************************//
	CStreamingDLReset::CStreamingDLReset(bool bSpeedUp): CDownloadRequestBase(_T("CStreamingDLReset"), DL)
	{
		StreamingResetReq req = {0};
		req.nDloadCmd = Streaming_CMD_RESET;
		CDownloadRequestBase::Tx((const char*) &req, sizeof(req), bSpeedUp);
	}
	CStreamingDLReset::~CStreamingDLReset(){}
	bool CStreamingDLReset::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			StreamingResetResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CStreamingDLReset::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		StreamingResetResp resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
		SetProcessResult(resp.nDloadCmd == Streaming_CMD_RESETACK);
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}

	//*******************************************************************//
	CQueryImageSizeRequest::CQueryImageSizeRequest(QuerySizeResp& resp, bool& bSupported)
		: m_resp(resp), m_bSupported(bSupported)
	{
		m_bSupported = false;

		QuerySizeReq req = {0};
		req.nDloadCmd = Streaming_CMD_CHECKIMAGEINTEGRITY;
		CDownloadRequestBase::Tx((const char*) &req, sizeof(req), true);
	}
	CQueryImageSizeRequest::~CQueryImageSizeRequest(){}
	bool CQueryImageSizeRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDownloadRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes)
		{ 
			QuerySizeResp resp = {0};
			if(ptBufferReceived && ptBufferReceived->GetData())
			{
				if(ptBufferReceived->GetLength() > sizeof(resp))
					memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
				else
					memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			}
			if(resp.nDloadCmd == Streaming_CMD_LOG)
			{
				bRes = false;
			}
		}
		return bRes;
	}
	void CQueryImageSizeRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		memcpy(&m_resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
		SetProcessResult(m_resp.nDloadCmd == 0x97 || m_resp.nDloadCmd == Streaming_CMD_ERROR);
		m_bSupported = m_resp.nDloadCmd == 0x97;
		CDownloadRequestBase::OnDataReceiver(ptBufferReceived);
	}
}
