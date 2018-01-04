#include "stdafx.h"
#include "NVRequest.h"
#include "..\codec\BQPacketLib.h"
namespace PLATFORM_TOOL
{
	//////////////////////////////////////////////////////////////////////////
	// This class is used for read IMEI information
	//////////////////////////////////////////////////////////////////////////
	CNVRequest::CNVRequest(const WORD& NVItem, const WORD& nDIAGcmd, DIAG_NVITEM_PACKET_F_type& NVResp, const BYTE *WriteData, const size_t szWriteData)
		:CDiagRequestBase(_T("CNVRequest"))
		,m_NVItem(NVItem)
		,m_ReadResp(NVResp)
	{
		memset(&m_ReadResp, 0, sizeof(m_ReadResp));

		m_ReadResp.nDIAGcmd = nDIAGcmd;
		m_ReadResp.item = m_NVItem;

		// Write NVItem?
		if (DIAG_NV_WRITE_F == nDIAGcmd)
		{
			if (szWriteData > DIAG_NV_ITEM_SIZE)
				memcpy((char*)(NVResp.item_data), WriteData, DIAG_NV_ITEM_SIZE);
			else
				memcpy((char*)(NVResp.item_data), WriteData, szWriteData);
		}

		CBufferPtr ptBufferSend;
		Encode((char*)&m_ReadResp, sizeof(m_ReadResp), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CNVRequest::~CNVRequest(void)
	{
	}

	void CNVRequest::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{
		//Sample to get NV_UE_IMEI_I
		/*
		Pipe Handle: 0x89a8908c (Endpoint Address: 0x4)
		Send 0x88 bytes to the device:
		26 26 02 00 00 00 00 00 00 00 00 00 00 00 00 00   
		00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   
		00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   
		00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   
		00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   
		00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   
		00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   
		00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   
		00 00 00 00 00 3E 2B 7E

		000624: Bulk or Interrupt Transfer (UP), 12.05.2008 15:58:49.5924257 +0.0156010
		Pipe Handle: 0x89a8906c (Endpoint Address: 0x84)
		Get 0x88 bytes from the device:
		26 26 02 08 3A 85 92 05 00 42 14 07 00 00 00 00  
		00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   
		00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   
		00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   
		00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   
		00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   
		00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   
		00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   
		00 00 00 00 00 C4 84 7E
		*/		
		::memcpy(&m_ReadResp,ptBufferReceived->GetData(),ptBufferReceived->GetLength());		
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CNVRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDiagRequestBase::IsCompletePacket(ptBufferReceived);		
		if (bRes)
		{
			BYTE *szBuffer = (BYTE *)(ptBufferReceived->GetData());
			/*
			3.4.64 Bad SPC Mode Response
			This message is special. The DM should not be generating request packets of
			sends a request to read or write an item that is considered part of service programming,
			correct SPC has not yet been entered to unlock service programming, this command
			in response. This message contains the first 16 bytes of the offending message.
			Field Length (bytes) Description
			CMD_CODE (66 / 0x42) 1 Message ID
			The DMSS sets CMD_CODE to 66 for
			DATA 16 First 16 bytes of disallowed request
			*/
			if (szBuffer[0] == m_ReadResp.nDIAGcmd)
				bRes = true;
			else if(szBuffer[0] == 0x42)
				bRes = true;//3.4.64 Bad SPC Mode Response
			else if(szBuffer[0] == 0x15)
				bRes = false;//Unknow need to check
			else if(szBuffer[0] == 0x13)
				bRes = false;//Unknow need to check
			else
			{
				assert(0);
				//Need to check szBuffer[0] on 80-V1294-YC
			}
		}
		return bRes;
	}

	// [6/19/2008 Eddy.Guo]
	//////////////////////////////////////////////////////////////////////////
	// This class is used for clear Activation data
	//////////////////////////////////////////////////////////////////////////
	CClearActivationDataRequest::CClearActivationDataRequest()
		:CDiagRequestBase(_T("CClearActivationDataRequest"))
		, m_bIsCmdSuccess(false) {
		CBufferPtr ptBufferSend;
		this->Encode(ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CClearActivationDataRequest::~CClearActivationDataRequest(void) {

	}
	void CClearActivationDataRequest::OnDataReceiver(CBufferPtr& ptBufferReceived) {		
		const char* data = ptBufferReceived->GetData();
		if(data[4] == 1)
			m_bIsCmdSuccess = true;
		else
			m_bIsCmdSuccess = false;
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CClearActivationDataRequest::CheckCmdResult() {
		return m_bIsCmdSuccess;
	}

	bool CClearActivationDataRequest::Encode(CBufferPtr& ptBufferSend) {
		// this send command is clear activation data
		unsigned char pSendBuf[] = {0x4b, 0x43, 0x03, 0x00, 0x06, 0xe9, 0x7E};
		ptBufferSend = new CBuffer(7, (const char*)pSendBuf);
		return true;
	}

	//  [6/24/2008 Eddy.Guo]
	//////////////////////////////////////////////////////////////////////////
	// This class is used for Standby Current Test
	//////////////////////////////////////////////////////////////////////////
	CStandbyCurrentTest::CStandbyCurrentTest()
		: CDiagRequestBase(_T("StandbyCurrentTest"),5000,false)
	{
		CBufferPtr ptBufferSend;
		
		char sendBuf[10] = 
		{
			0x4b, 0x0b, 0x00, 0x80, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00
		};
		Encode(sendBuf,10,ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CStandbyCurrentTest::~CStandbyCurrentTest() { }
	bool CStandbyCurrentTest::IsCompletePacket(CBufferPtr& ptBufferReceived) 
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
	void CStandbyCurrentTest::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	//  [6/26/2008 Eddy.Guo]
	//////////////////////////////////////////////////////////////////////////
	// This class is used for Master Clear(clean all data on handset)
	//////////////////////////////////////////////////////////////////////////
	CMasterClear::CMasterClear() 
		:CDiagRequestBase(_T("CMasterClear"))
	{
		CBufferPtr ptBufferSend;		
		char szSendBuf[0x85] = {0};
		szSendBuf[0] = 0x27; szSendBuf[1] = 0xc5; szSendBuf[2] = 0x1f; szSendBuf[3] = 0x01;		
		Encode(szSendBuf,0x85,ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CMasterClear::~CMasterClear() { }

	//  [6/26/2008 Eddy.Guo]
	//////////////////////////////////////////////////////////////////////////
	// Just only for Write
	//////////////////////////////////////////////////////////////////////////
	CWriteNVData::CWriteNVData(const WORD& NVItem, const WORD& nDIAGcmd, DIAG_NVITEM_PACKET_F_type& WritePacket) 
		:CDiagRequestBase(_T("CWriteNVData"))
	{
		CBufferPtr ptBufferSend;
		char szSendCmd[0x85] = {0};
		szSendCmd[0] = WritePacket.nDIAGcmd;  //< 39 -- 0x27 means write NV
		szSendCmd[1] = (WritePacket.item & 0xff00) >> 8;
		szSendCmd[2] = (WritePacket.item & 0xff);
		for(int i = 0; i < 128; ++i) 
		{
			szSendCmd[3+i] = WritePacket.item_data[i];
		}
		Encode(szSendCmd,0x85, ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CWriteNVData::~CWriteNVData() {}
	bool CWriteNVData::GetResponse(DIAG_NVITEM_PACKET_F_type& ResponsePacket) {
		return false;
	}

	//  [6/27/2008 Eddy.Guo]
	//////////////////////////////////////////////////////////////////////////
	// This class is used for Write QFuse
	//////////////////////////////////////////////////////////////////////////
	CWriteQFuse::CWriteQFuse(UINT chain, uint32 LSB, uint32 MSB, uint32 RegConf2, uint32 RegConf3, uint32 RegConf4,bool& bIsFTM) 
		:CDiagRequestBase(_T("WriteQFuse"))
		,m_bIsFTM(bIsFTM)
	{
		m_parameters.chain = chain;
		m_parameters.LSB = LSB;
		m_parameters.MSB = MSB;
		m_parameters.RegConf2 = RegConf2;
		m_parameters.RegConf3 = RegConf3;
		m_parameters.RegConf4 = RegConf4;

		char szBufSend[0x20] = {0x4b, 0x0b, 0x18, 0x00, 0x01, 0x00, 0x16, 0x00, 0x0c, 0x00};
		szBufSend[10] = m_parameters.chain & 0x00ff;
		szBufSend[11] = (m_parameters.chain & 0xff00) >> 8;

		szBufSend[12] = (char)(m_parameters.LSB & 0x000000ff);
		szBufSend[13] = (char)((m_parameters.LSB & 0x0000ff00) >> 8);
		szBufSend[14] = (char)((m_parameters.LSB & 0x00ff0000) >> 16);
		szBufSend[15] = (char)((m_parameters.LSB & 0xff000000) >> 24);

		szBufSend[16] = (char)(m_parameters.MSB & 0x000000ff);
		szBufSend[17] = (char)((m_parameters.MSB & 0x0000ff00) >> 8);
		szBufSend[18] = (char)((m_parameters.MSB & 0x00ff0000) >> 16);
		szBufSend[19] = (char)((m_parameters.MSB & 0xff000000) >> 24);

		CBufferPtr ptBufferSend;
		Encode(szBufSend,0x20,ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CWriteQFuse::~CWriteQFuse() {}
	void CWriteQFuse::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{		
		const char* szBuffer = ptBufferReceived->GetData();
		if(szBuffer[0] == 0x4B)
		{
			m_bIsFTM = true;
		}
		else
		{
			m_bIsFTM = false;
		}
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool CWriteQFuse::IsCompletePacket(CBufferPtr& ptBufferReceived) 
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	//  [6/27/2008 Eddy.Guo]
	//////////////////////////////////////////////////////////////////////////
	// This class is used for opening charging to headset
	//////////////////////////////////////////////////////////////////////////
	CBTHeadsetCharging::CBTHeadsetCharging(bool IsOn)
		:CDiagRequestBase(_T("CBTHeadsetCharging"))
		, m_bIsOn(IsOn) 
	{
		CBufferPtr ptBufferSend;
		char bufSend[] = {
			0x4b
			,0x24
			,0xec
			,0x03
			,m_bIsOn ? 0x01 : 0x00
		};
		Encode(bufSend,5,ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CBTHeadsetCharging::~CBTHeadsetCharging() {

	}
	void CBTHeadsetCharging::OnDataReceiver(CBufferPtr& ptBufferReceived) {
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool CBTHeadsetCharging::IsCompletePacket(CBufferPtr& ptBufferReceived) 
	{
		CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		unsigned char r[] = {0x4b, 0x24, 0xec, 0x03, 0x00, 0x00};
		const char* retVal = ptBufferReceived->GetData();
		bool bRes = false;
		if(memcmp(r, retVal, 6) == 0)
			bRes = true;
		return bRes;
	}

	//  [6/28/2008 Eddy.Guo]
	//////////////////////////////////////////////////////////////////////////
	// For searching BT device
	//////////////////////////////////////////////////////////////////////////
	CSearchBTDevice::CSearchBTDevice()
		:CDiagRequestBase(_T("CSearchBTDevice"))
		, m_status(false) {
		CBufferPtr ptBufferSend;
		Encode(ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CSearchBTDevice::~CSearchBTDevice() { }
	void CSearchBTDevice::OnDataReceiver(CBufferPtr& ptBufferReceived) {
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool CSearchBTDevice::Encode(CBufferPtr& ptBufferSend) {
		return true;
	}
	bool CSearchBTDevice::IsCompletePacket(CBufferPtr& ptBufferReceived) {
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
	bool CSearchBTDevice::GetStatus() {
		return m_status;
	}

	// For Query BT Headset status
	//////////////////////////////////////////////////////////////////////////
	CQueryCommand::CQueryCommand(const char* BTAddress, int length,bool& bIsPaired) 
		:CDiagRequestBase(_T("CQueryBTPairCommand"))
	    ,m_pBTAddress(NULL)
		,m_nBTAddressLength(length)
		,m_bIsPaired(bIsPaired)
	{		
		m_bIsPaired = false;
		m_pBTAddress = new char[length];
		memset(m_pBTAddress, 0, sizeof(char)*length);
		memcpy(m_pBTAddress, BTAddress, sizeof(char)*length);

		char* c = m_pBTAddress;
		int n = m_nBTAddressLength;

		CBufferPtr ptBufferSend;

		unsigned char bufSend[17] = {
			0x4b
			,0x03
			,0x10
			,0x00
			,0xfb
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
		};
		Encode((const char *)bufSend,17,ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CQueryCommand::~CQueryCommand() 
	{ 
		if(m_pBTAddress)
			delete [] m_pBTAddress;
	}

	int CQueryCommand::HexToInt(char c) 
	{
		if(c >= '0' && c <= '9') return c - '0';
		if(c >= 'A' && c <= 'F') return c - 'A' + 10;
		if(c >= 'a' && c <= 'f') return c - 'a' + 10;
		return 0;
	}

	void CQueryCommand::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{		
		const char* retVal = ptBufferReceived->GetData();
        char szCmpString[] = {0x4b, 0x03, 0x10,0x00,0x01};
		m_bIsPaired = 0 == ::memcmp(szCmpString, retVal, 5);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CQueryCommand::IsCompletePacket(CBufferPtr& ptBufferReceived) 
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	//  [7/4/2008 Eddy.Guo]
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	CReadFactoryArea::CReadFactoryArea(unsigned char* szBuf, int* nState)
		:CDiagRequestBase(_T("CReadFactoryArea"))
		, m_szFactoryArea(szBuf)
		, m_pnState(nState) 
	{
		CBufferPtr ptBufferSend;
		char bufSend[0x85] = {0};
		bufSend[0] = 0x26; bufSend[1] = 0xc1; bufSend[2] = 0x09;
		Encode(bufSend,0x85,ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CReadFactoryArea::~CReadFactoryArea() {

	}
	void CReadFactoryArea::OnDataReceiver(CBufferPtr& ptBufferReceived) {		
		const char* retVal = ptBufferReceived->GetData();
		int length = ptBufferReceived->GetLength();
		memcpy(m_szFactoryArea, &(retVal[3]), 128);
		*m_pnState = retVal[131] + retVal[132] * 16;
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool CReadFactoryArea::IsCompletePacket(CBufferPtr& ptBufferReceived) {
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
	int CReadFactoryArea::GetState() {
		return *m_pnState;
	}

	//  [7/7/2008 Eddy.Guo]
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	CCheckSIM::CCheckSIM(int* nState) 
		: CDiagRequestBase(_T("CheckSIM"))
		,m_pnState(nState) 
	{
		CBufferPtr ptBufferSend;
		char bufSend[] = {0x4b, 0x21, 0x17, 0x00};
		Encode(bufSend,4,ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CCheckSIM::~CCheckSIM() {}
	void CCheckSIM::OnDataReceiver(CBufferPtr& ptBufferReceived) {		
		const char* retVal = ptBufferReceived->GetData();
		*m_pnState = retVal[13];
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool CCheckSIM::IsCompletePacket(CBufferPtr& ptBufferReceived) {
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
	int CCheckSIM::GetState() {
		return *m_pnState;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	CBackupRestoreNV::CBackupRestoreNV(int nFunction,bool& bStatus)
		:CDiagRequestBase("Backup Restore NV Partition")
		,m_nFunction(nFunction)
		,m_bStatus(bStatus)
		,m_nBackupTimes(nFunction)
		,m_nY(nFunction)
		,m_nM(nFunction)
		,m_nD(nFunction)
		,m_nH(nFunction)
		,m_nMin(nFunction)
		,m_nS(nFunction)
	{
		diagpkt_subsys_hdr_type_v2 req = {0};
		
		req.command_code = DIAG_SUBSYS_CMD_VER_2_F;
		req.subsys_id = DIAG_SUBSYS_CNV;
		req.subsys_cmd_code = m_nFunction;

		CBufferPtr ptBufferSend;
		Encode((const char*)&req,sizeof(diagpkt_subsys_hdr_type_v2),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CBackupRestoreNV::CBackupRestoreNV(int nFunction,bool& bStatus,int& nBackupTimes, int& nY, int& nM, int& nD, int& nH, int& nMin, int& nS)
		:CDiagRequestBase("Check NV Partition")
		,m_nFunction(nFunction)
		,m_bStatus(bStatus)
		,m_nBackupTimes(nBackupTimes)
		,m_nY(nY)
		,m_nM(nM)
		,m_nD(nD)
		,m_nH(nH)
		,m_nMin(nMin)
		,m_nS(nS)
	{				
		CBufferPtr ptBufferSend;
		if(m_nFunction == 3)
		{
			GET_NV_PARTITION_STATUS_REQ req = {0};
			req.subsys_hdr.command_code = 0x4B;
			req.subsys_hdr.subsys_id = 0x37;
			req.subsys_hdr.subsys_cmd_code = m_nFunction;
			Encode((const char*)&req,sizeof(GET_NV_PARTITION_STATUS_REQ),ptBufferSend);
		}
		else
		{			
			diagpkt_subsys_hdr_type_v2 req = {0};
			req.command_code = DIAG_SUBSYS_CMD_VER_2_F;
			req.subsys_id = DIAG_SUBSYS_CNV;
			req.subsys_cmd_code = m_nFunction;
			Encode((const char*)&req,sizeof(diagpkt_subsys_hdr_type_v2),ptBufferSend);
		}

		SetSendData(ptBufferSend);
	}

	CBackupRestoreNV::~CBackupRestoreNV() { }

	void CBackupRestoreNV::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{		
		const char* retVal = ptBufferReceived->GetData();

		if(m_nFunction == 3)
		{
			GET_NV_PARTITION_STATUS_RESP resp = {0};
			::memcpy(&resp,retVal,sizeof(GET_NV_PARTITION_STATUS_RESP));
			m_nBackupTimes = resp.cnv_backup_checked.BackupCount;
			m_nY = resp.cnv_backup_checked.BackupTime.year;
			m_nM = resp.cnv_backup_checked.BackupTime.month;
			m_nD = resp.cnv_backup_checked.BackupTime.day;
			m_nH = resp.cnv_backup_checked.BackupTime.hour;
			m_nMin = resp.cnv_backup_checked.BackupTime.minute;
			m_nS = resp.cnv_backup_checked.BackupTime.second;
			m_bStatus = true;
		}
		else
		{
			diagpkt_subsys_hdr_type_v2 resp = {0};
			::memcpy(&resp,retVal,ptBufferReceived->GetLength());
			m_bStatus = resp.status ==0;
		}
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CBackupRestoreNV::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	CBTHandsetVoltageReading::CBTHandsetVoltageReading(long* nVoltage) 
		:CDiagRequestBase(_T("CBTHandsetVoltageReading"))
		, m_pnVoltage(nVoltage) 
	{
		CBufferPtr ptBufferSend;
		char bufSend[] = {0x4b, 0x24, 0x05, 0x00};
		Encode(bufSend,4,ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CBTHandsetVoltageReading::~CBTHandsetVoltageReading() { }
	void CBTHandsetVoltageReading::OnDataReceiver(CBufferPtr& ptBufferReceived) {		
		const unsigned char* retVal = (const unsigned char*)ptBufferReceived->GetData();
		*m_pnVoltage = retVal[4] + retVal[5] * 16 * 16;
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool CBTHandsetVoltageReading::IsCompletePacket(CBufferPtr& ptBufferReceived) {
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	CSPCRequest::CSPCRequest(const char *szSPC, bool &status)
		:CDiagRequestBase(_T("CSPCRequest"))
		,m_Status(status)
	{
		m_Req.cmd_code = DIAG_SPC_F;
		memcpy((char*)(m_Req.digits), szSPC, sizeof(char)*6);

		CBufferPtr ptBufferSend;
		Encode((char*)&m_Req,sizeof(m_Req),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CSPCRequest::~CSPCRequest(void)
	{
	}

	void CSPCRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{				
		diagpkt_sec_code_resp_type resp = {0};
		::memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));

		// true, if spc code is correct
		// false, if spc is incorrectly given
		m_Status = resp.sec_code_ok;	
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CSPCRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		if (bRes)
		{
			BYTE *szBuffer = (BYTE *)(ptBufferReceived->GetData());
			if (szBuffer[0] == DIAG_SPC_F)
				return true;
		}
		return false;
	}


	//////////////////////////////////////////////////////////////////////////
	// Set offline mode (Mobile mode control)
	//////////////////////////////////////////////////////////////////////////
	COfflineMode::COfflineMode() 
		:CDiagRequestBase(_T("COfflineMode"))
	{
		CBufferPtr ptBufferSend;
		char bufSend[] = {0x29, 0x01, 0x00};
		Encode(bufSend,3,ptBufferSend);
		SetSendData(ptBufferSend);
	}
	COfflineMode::~COfflineMode(void) {}
	void COfflineMode::OnDataReceiver(CBufferPtr& ptBufferReceived) {
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool COfflineMode::IsCompletePacket(CBufferPtr& ptBufferReceived) {
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	CResetHandset::CResetHandset() 
		:CDiagRequestBase(_T("CResetHandset"))
	{
		CBufferPtr ptBufferSend;
		char bufSend[] = {0x29, 0x02, 0x00};
		Encode(bufSend,3,ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CResetHandset::~CResetHandset() {}
	void CResetHandset::OnDataReceiver(CBufferPtr& ptBufferReceived) {
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool CResetHandset::IsCompletePacket(CBufferPtr& ptBufferReceived) {
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// Set FTM (Mobile mode control)
	//////////////////////////////////////////////////////////////////////////
	CFTMMode::CFTMMode() 
		:CDiagRequestBase(_T("CFTMMode"))
	{
		CBufferPtr ptBufferSend;
		char bufSend[] = {0x29, 0x03, 0x00};
		Encode(bufSend,3,ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CFTMMode::~CFTMMode(void) {}
	void CFTMMode::OnDataReceiver(CBufferPtr& ptBufferReceived) {
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool CFTMMode::IsCompletePacket(CBufferPtr& ptBufferReceived) {
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	//////////////////////////////////////////////////////////////////////////
	// Set Online mode (Mobile mode control)
	//////////////////////////////////////////////////////////////////////////
	COnlineMode::COnlineMode() 
		:CDiagRequestBase(_T("COnlineMode"))
	{
		CBufferPtr ptBufferSend;
		char bufSend[] = {0x29, 0x04, 0x00};
		Encode(bufSend,3,ptBufferSend);
		SetSendData(ptBufferSend);
	}
	COnlineMode::~COnlineMode(void) {}
	void COnlineMode::OnDataReceiver(CBufferPtr& ptBufferReceived) {
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool COnlineMode::IsCompletePacket(CBufferPtr& ptBufferReceived) {
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	CControlMode::CControlMode(const unsigned int mode)
		:CDiagRequestBase(_T("CControlMode"))
	{
		DIAG_CONTROL_F_PACKET_type req = {0};

		req.nDIAGcmd = DIAG_CONTROL_F;
		req.nMode = mode;

		CBufferPtr ptBufferSend;
		Encode((char*)&req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CControlMode::~CControlMode(void)
	{
	}
	void CControlMode::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		DIAG_CONTROL_F_PACKET_type resp = {0};		
		::memcpy(&resp, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CControlMode::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	CReadQFuseState::CReadQFuseState(unsigned long* reg1, unsigned long* reg2, bool* bSuccess)
		:CDiagRequestBase(_T("ReadQFuseState"))
		,m_pReg1(reg1)
		,m_pReg2(reg2)
        ,m_bSuccess(bSuccess)
	{
		CBufferPtr ptBufferSend;
		Encode(ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CReadQFuseState::~CReadQFuseState(void) {}
	void CReadQFuseState::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{		
		const unsigned char* retVal = (unsigned char*)ptBufferReceived->GetData();
		int length = ptBufferReceived->GetLength();
		if(retVal[0] == 0x18) {
			*m_bSuccess = false;
			*m_pReg1 = 6;
			*m_pReg2 = 0;
		} else if(retVal[0] == 0x4b) {
			*m_bSuccess = true;
			*m_pReg1 = (long)retVal[12] + (long)retVal[13] * 256 + (long)retVal[14] * 256 * 256 + (long)retVal[15] * 256 * 256 * 256;
			*m_pReg2 = (long)retVal[16] + (long)retVal[17] * 256 + (long)retVal[18] * 256 * 256 + (long)retVal[19] * 256 * 256 * 256;
		}
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CReadQFuseState::Encode(CBufferPtr& ptBufferSend) 
	{
		unsigned char bufSend[] = {0x4B, 0x0B, 0x18, 0x00, 0x00, 0x00, 0x02, 
			0x00, 0x20, 0x00, 0x00, 0x00, 0xDB, 0xC4, 0x7E};
		ptBufferSend = new CBuffer(0xf, (const char*)bufSend);
		return true;
	}

	bool CReadQFuseState::IsCompletePacket(CBufferPtr& ptBufferReceived) 
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	//////////////////////////////////////////////////////////////////////////
	// Write NV item (boot mode online)
	//////////////////////////////////////////////////////////////////////////
	CChangeOnline::CChangeOnline()
		:CDiagRequestBase(_T("CChangeOnline"))
	{
		CBufferPtr ptBufferSend;
		Encode(ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CChangeOnline::~CChangeOnline() {}
	void CChangeOnline::OnDataReceiver(CBufferPtr& ptBufferReceived) {
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool CChangeOnline::Encode(CBufferPtr& ptBufferSend) {
		char szSendCmd[0x88] = {0};
		szSendCmd[0] = 0x27;  //< 39 -- 0x27 means write NV
		szSendCmd[1] = 0xc5;
		szSendCmd[2] = 0x01;
		// because online code is 0x00, we don't write any code into item_data
		// FTM code is 0x01
		WORD wCRC = CCRCMachine::CRC16LCalculation((CHAR*)szSendCmd, 133*8);
		szSendCmd[133] = wCRC & 0xff;
		szSendCmd[134] = (wCRC & 0xff00) >> 8;
		szSendCmd[135] = 0x7e;
		ptBufferSend = new CBuffer(136, (const char*)szSendCmd);
		return true;
	}
	bool CChangeOnline::IsCompletePacket(CBufferPtr& ptBufferReceived) {
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	//////////////////////////////////////////////////////////////////////////
	// Write NV item (boot mode FTM)
	//////////////////////////////////////////////////////////////////////////
	CChangeFTM::CChangeFTM()
		:CDiagRequestBase(_T("CChangeFTM"))
	{
		CBufferPtr ptBufferSend;
		Encode(ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CChangeFTM::~CChangeFTM() {}
	void CChangeFTM::OnDataReceiver(CBufferPtr& ptBufferReceived) {
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool CChangeFTM::Encode(CBufferPtr& ptBufferSend) {
		char szSendCmd[0x88] = {0};
		szSendCmd[0] = 0x27;  //< 39 -- 0x27 means write NV
		szSendCmd[1] = 0xc5;
		szSendCmd[2] = 0x01;
		szSendCmd[3] = 0x01;
		WORD wCRC = CCRCMachine::CRC16LCalculation((CHAR*)szSendCmd, 133*8);
		szSendCmd[133] = wCRC & 0xff;
		szSendCmd[134] = (wCRC & 0xff00) >> 8;
		szSendCmd[135] = 0x7e;
		ptBufferSend = new CBuffer(136, (const char*)szSendCmd);
		return true;
	}
	bool CChangeFTM::IsCompletePacket(CBufferPtr& ptBufferReceived) {
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}


    //  [7/31/2008 Eddy.Guo]
    //////////////////////////////////////////////////////////////////////////
    //
    //////////////////////////////////////////////////////////////////////////
    CPowerOff::CPowerOff()
		:CDiagRequestBase(_T("CPowerOff"))
	{
        CBufferPtr ptBufferSend;
        Encode(ptBufferSend);
        SetSendData(ptBufferSend);
    }
    CPowerOff::~CPowerOff() {}
    void CPowerOff::OnDataReceiver(CBufferPtr& ptBufferReceived){
        CDiagRequestBase::OnDataReceiver(ptBufferReceived);
    }
    bool CPowerOff::Encode(CBufferPtr& ptBufferSend) {
        unsigned char szSendCmd[] = {0x29, 0x06, 0x00, 0x39, 0x0d, 0x7e};
        ptBufferSend = new CBuffer(6, (const char*)szSendCmd);
        return true;
    }
    bool CPowerOff::IsCompletePacket(CBufferPtr& ptBufferReceived) {
        return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
    }

	//  [7/31/2008 Eddy.Guo]
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	CGetBootLoaderRevocation::CGetBootLoaderRevocation(int* pnVersion) 
		:CDiagRequestBase(_T("CGetBootLoaderRevocation"))
		, m_pnVersion(pnVersion)
	{
		CBufferPtr ptBufferSend;
		Encode(ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CGetBootLoaderRevocation::~CGetBootLoaderRevocation() {}
	void CGetBootLoaderRevocation::OnDataReceiver(CBufferPtr& ptBufferReceived) {		
		const char* retVal = ptBufferReceived->GetData();
		int length = ptBufferReceived->GetLength();
		*m_pnVersion = retVal[2];
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool CGetBootLoaderRevocation::Encode(CBufferPtr& ptBufferSend) {
		unsigned char szSendCmd[5] = {0x7e, 0x14, 0xdd, 0xa6, 0x7e};
		ptBufferSend = new CBuffer(5, (const char*)szSendCmd);
		return true;
	}
	bool CGetBootLoaderRevocation::IsCompletePacket(CBufferPtr& ptBufferReceived) {
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	//  [7/31/2008 Eddy.Guo]
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	CGetPhonecodeRevocation::CGetPhonecodeRevocation(int* pnVersion) 
		:CDiagRequestBase(_T("CGetPhonecodeRevocation"))
		, m_pnVersion(pnVersion)
	{
		CBufferPtr ptBufferSend;
		Encode(ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CGetPhonecodeRevocation::~CGetPhonecodeRevocation() {}
	void CGetPhonecodeRevocation::OnDataReceiver(CBufferPtr& ptBufferReceived) {		
		const char* retVal = ptBufferReceived->GetData();
		int length = ptBufferReceived->GetLength();
		*m_pnVersion = retVal[2];
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool CGetPhonecodeRevocation::Encode(CBufferPtr& ptBufferSend) {
		unsigned char szSendCmd[5] = {0x7e, 0x16, 0xcf, 0x85, 0x7e};
		ptBufferSend = new CBuffer(5, (const char*)szSendCmd);
		return true;
	}
	bool CGetPhonecodeRevocation::IsCompletePacket(CBufferPtr& ptBufferReceived) {
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	//  [7/31/2008 Eddy.Guo]
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	CReadFTMNV::CReadFTMNV(int* nOutFTMValue)
		:CDiagRequestBase(_T("CReadFTMNV"))
		, m_pnFTMValue(nOutFTMValue)
	{
		CBufferPtr ptBufferSend;
		Encode(ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CReadFTMNV::~CReadFTMNV() {}
	void CReadFTMNV::OnDataReceiver(CBufferPtr& ptBufferReceived) {
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
		char szCheckValue[] = {0x26, 0xc5, 0x01};
		const char* retVal = ptBufferReceived->GetData();
		int length = ptBufferReceived->GetLength();
		if(memcmp(szCheckValue, retVal, 3) == 0)
			*m_pnFTMValue = retVal[3];
		else
			*m_pnFTMValue = -1;
	}
	bool CReadFTMNV::Encode(CBufferPtr& ptBufferSend) {
		unsigned char szSendCmd[0x88] = {0x26, 0xc5, 0x01};
		szSendCmd[0x88 - 1] = 0x7e;
		szSendCmd[0x88 - 2] = 0x1a;
		szSendCmd[0x88 - 3] = 0x0b;
		ptBufferSend = new CBuffer(0x88, (const char*)szSendCmd);
		return true;
	}
	bool CReadFTMNV::IsCompletePacket(CBufferPtr& ptBufferReceived) {
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

    //  [8/13/2008 Eddy.Guo]
    //////////////////////////////////////////////////////////////////////////
    //
    //////////////////////////////////////////////////////////////////////////
    CAutoCommand::CAutoCommand() 
		:CDiagRequestBase(_T("CAutoCommand"))
	{
        CBufferPtr ptBufferSend;
        Encode(ptBufferSend);
        SetSendData(ptBufferSend);
    }
    CAutoCommand::~CAutoCommand() {}
    void CAutoCommand::OnDataReceiver(CBufferPtr& ptBufferReceived) {
        CDiagRequestBase::OnDataReceiver(ptBufferReceived);
    }
    bool CAutoCommand::Encode(CBufferPtr& ptBufferSend) {
        unsigned char szSendCmd[0x14] = {0x4b, 0x03, 0x0f, 0x00, 0xfc};
        szSendCmd[0x14 - 1] = 0x7e;
        szSendCmd[0x14 - 2] = 0x11;
        szSendCmd[0x14 - 3] = 0x3a;
        ptBufferSend = new CBuffer(0x14, (const char*)szSendCmd);
        return true;
    }
    bool CAutoCommand::IsCompletePacket(CBufferPtr& ptBufferReceived) {
        return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
    }

    //  [8/13/2008 Eddy.Guo]
    //////////////////////////////////////////////////////////////////////////
    //
    //////////////////////////////////////////////////////////////////////////
    CQueryAutoCommandResult::CQueryAutoCommandResult(bool* bRet)
		:CDiagRequestBase(_T("CQueryAutoCommandResult")) , m_pbResult(bRet) 
	{
        CBufferPtr ptBufferSend;
        Encode(ptBufferSend);
        SetSendData(ptBufferSend);
    }
	CQueryAutoCommandResult::~CQueryAutoCommandResult() {}
    void CQueryAutoCommandResult::OnDataReceiver(CBufferPtr& ptBufferReceived) {        
        const char* retVal = ptBufferReceived->GetData();
        char szCmpBuf[] = {0x4b, 0x03, 0x10};
        if(*(retVal+4) == 0 && memcmp(szCmpBuf, retVal, 3) == 0)
            *m_pbResult = true;
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
    }
    bool CQueryAutoCommandResult::Encode(CBufferPtr& ptBufferSend) {
        unsigned char szSendCmd[0x08] = {0x4b, 0x03, 0x10, 0x00, 0xfb};
        szSendCmd[0x08 - 1] = 0x7e;
        szSendCmd[0x08 - 2] = 0xa0;
        szSendCmd[0x08 - 3] = 0xbd;
        ptBufferSend = new CBuffer(0x14, (const char*)szSendCmd);
        return true;
    }
    bool CQueryAutoCommandResult::IsCompletePacket(CBufferPtr& ptBufferReceived) {
        return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
    }

	
    CReadBatteryTemperature::CReadBatteryTemperature(int& nTemperature) 
		:CDiagRequestBase(_T("CReadBatteryTemperature"))
		, m_nTemperature(nTemperature) 
	{
		const char szSendCmd[6] = {0x4b, 0x24, 0x07, 0x00, 0x00, 0x00};
		CBufferPtr ptBufferSend;        
        Encode(szSendCmd,6,ptBufferSend);
        SetSendData(ptBufferSend);
    }
    CReadBatteryTemperature::~CReadBatteryTemperature() 
	{
	}
    void CReadBatteryTemperature::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{ 
		/*
		typedef PACKED struct 
		{
		diagpkt_subsys_header_type  header;
		uint16 return_value;
		} PM_APP_DIAG_READ_RSP;
		*/
        char szCmpBuf[] = {0x4b, 0x24, 0x07};
        const unsigned char* retVal = (const unsigned char*)ptBufferReceived->GetData();
        if(memcmp(szCmpBuf, retVal, 3) == 0) {
            m_nTemperature = retVal[5] * 256 + retVal[4];
            m_nTemperature -= 273;
        } else {
            m_nTemperature = 0;
        }
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
    }
    bool CReadBatteryTemperature::IsCompletePacket(CBufferPtr& ptBufferReceived) 
	{
        return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
    }

    //////////////////////////////////////////////////////////////////////////
    //
    //////////////////////////////////////////////////////////////////////////
    CReadBatteryVoltage::CReadBatteryVoltage(int* nVoltage) 
		:CDiagRequestBase(_T("CReadBatteryVoltage"))
		, m_pnVoltage(nVoltage) 
	{
        CBufferPtr ptBufferSend;
        Encode(ptBufferSend);
        SetSendData(ptBufferSend);
    }
    CReadBatteryVoltage::~CReadBatteryVoltage() 
	{
	}
    void CReadBatteryVoltage::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{        
        char szCmpBuf[] = {0x4b, 0x24, 0x02};
        const unsigned char* retVal = (const unsigned char*)ptBufferReceived->GetData();
        if(memcmp(szCmpBuf, retVal, 3) == 0) {
            *m_pnVoltage = retVal[5] * 256 + retVal[4];
            //*m_pnVoltage = (*(retVal+5) * 16 * 16) + (*(retVal+4)) + 256;
        } else {
            *m_pnVoltage = 0;
        }
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
    }
    bool CReadBatteryVoltage::Encode(CBufferPtr& ptBufferSend) 
	{
        unsigned char szSendCmd[0x09] = {0x4b, 0x24, 0x02, 0x00, 0x00, 0x00, 0x8c, 0xac, 0x7e};
        ptBufferSend = new CBuffer(0x09, (const char*)szSendCmd);
        return true;
    }
    bool CReadBatteryVoltage::IsCompletePacket(CBufferPtr& ptBufferReceived) 
	{
        return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
    }

	// -----------------------------------------------------------------
	CSendSync::CSendSync(const BYTE* pReqData, const UINT nReqDataSize, BYTE* pRespData, UINT& nRespDataSize)
		:CDiagRequestBase(_T("CSendSync"))
		, m_pRespData(pRespData)
		, m_nRespDataSize(nRespDataSize)
	{
		memset((void*)pRespData, 0, sizeof(BYTE)*nRespDataSize);

		CBufferPtr ptBufferSend;
		Encode((char*)pReqData, sizeof(BYTE)*nReqDataSize, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CSendSync::~CSendSync(void)
	{
	}

	void CSendSync::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{	
		if (m_nRespDataSize < ptBufferReceived->GetLength())
			memcpy(m_pRespData, ptBufferReceived->GetData(), m_nRespDataSize);
		else
			memcpy(m_pRespData, ptBufferReceived->GetData(), ptBufferReceived->GetLength());
		m_nRespDataSize = ptBufferReceived->GetLength();		
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CSendSync::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	// -----------------------------------------------------------------
	CPRLRequest::CPRLRequest(BYTE* pPRLData, UINT& nPRLDataSize, BYTE& Status, BOOL bRead)
		:CDiagRequestBase(_T("CPRLRequest"))
		,m_pPRLData(pPRLData), m_nPRLDataSize(nPRLDataSize), m_Status(Status), m_bRead(bRead)
	{
		CBufferPtr ptBufferSend;

		if (bRead)
		{
			DIAG_PR_LIST_RD_REQ_TYPE rd_req = {0};
			rd_req.cmd_code = 73;
			Encode((char*)&rd_req, sizeof(DIAG_PR_LIST_RD_REQ_TYPE), ptBufferSend);
		}
		else
		{
			DIAG_PR_LIST_WR_REQ_TYPE wr_req = {0};
			wr_req.cmd_code = 72;			

			if (nPRLDataSize < DIAG_PR_LIST_BLOCK_SIZE)
			{
				memcpy((void*)wr_req.pr_list_data, (void*)pPRLData, nPRLDataSize);
				wr_req.num_bits = nPRLDataSize << 3;
			}
			else
			{
				memcpy((void*)wr_req.pr_list_data, (void*)pPRLData, DIAG_PR_LIST_BLOCK_SIZE);
				wr_req.num_bits = DIAG_PR_LIST_BLOCK_SIZE << 3;
			}

			Encode((char*)&wr_req, sizeof(DIAG_PR_LIST_WR_REQ_TYPE), ptBufferSend);
		}		
		
		SetSendData(ptBufferSend);
	}

	CPRLRequest::~CPRLRequest(void)
	{
	}

	void CPRLRequest::OnDataReceiver(CBufferPtr& ptBufferReceived) 
	{	
		if (m_bRead)
		{
			DIAG_PR_LIST_RD_RSP_TYPE rd_resp;
			memcpy((void*)&rd_resp, (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

			m_Status = rd_resp.rl_status;
			UINT szData = rd_resp.num_bits >> 3;
			if (szData < m_nPRLDataSize)
				memcpy((void*)m_pPRLData, (void*)&rd_resp.pr_list_data, szData);
			else
				memcpy((void*)m_pPRLData, (void*)&rd_resp.pr_list_data, m_nPRLDataSize);

			m_nPRLDataSize = szData;
		}
		else
		{
			DIAG_PR_LIST_WR_RSP_TYPE wr_resp;
			memcpy((void*)&wr_resp, (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

			m_Status = wr_resp.rl_status;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CPRLRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
	// -----------------------------------------------------------------
	CIsEMURequest::CIsEMURequest(bool& bIsEMU)
		: CDiagRequestBase(_T("CIsEMURequest"))
		, m_bIsEMU(bIsEMU)
	{
		diagpkt_IsEMU req = {0};
		req.command_code = DIAG_SUBSYS_CMD_F;
		req.subsys_id = DIAG_SUBSYS_PM;
		req.subsys_cmd_code = 5;

		CBufferPtr ptBufferSend;
		Encode((const char*)&req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CIsEMURequest::~CIsEMURequest(){}
	bool CIsEMURequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
	void CIsEMURequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		diagpkt_IsEMU resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bIsEMU = resp.IsEMU == 1;
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	// -----------------------------------------------------------------
	CWriteNVItemRequest::CWriteNVItemRequest(DIAG_NVITEM_PACKET_F_type& req,  WORD& nNVStatus)
	: CDiagRequestBase(_T("CWriteNVItemRequest"))
	, m_nNVStatus(nNVStatus)
	{
		req.nDIAGcmd = DIAG_NV_WRITE_F;
		CBufferPtr ptBufferSend;
		Encode((char*)&req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CWriteNVItemRequest::~CWriteNVItemRequest(){}
	bool CWriteNVItemRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
	void CWriteNVItemRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		DIAG_NVITEM_PACKET_F_type resp={0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_nNVStatus = resp.nv_stat;
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	// -----------------------------------------------------------------
	CCloseConnectionRequest::CCloseConnectionRequest(bool& bExeucteResult)
		: CDiagRequestBase(_T("CCloseConnectionRequest"))
		, m_bExecuteResult(bExeucteResult)
	{
		m_bExecuteResult = false;

		diagpkt_subsys_hdr_type req = {0};
		req.cmd_code = DIAG_SUBSYS_CMD_F;
		req.subsys_id = DIAG_SUBSYS_USB;
		req.subsys_cmd_code = 1000;
		CBufferPtr ptBufferSend;
		Encode((char*)&req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CCloseConnectionRequest::~CCloseConnectionRequest(){}
	void CCloseConnectionRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		diagpkt_subsys_hdr_type resp = {0};
		if(ptBufferReceived->GetLength() > sizeof(resp))
			memcpy(&resp, ptBufferReceived->GetData(),sizeof(resp));
		else
			memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		//if(req == resp)
		if(resp.cmd_code==DIAG_SUBSYS_CMD_F && resp.subsys_id==DIAG_SUBSYS_USB && resp.subsys_cmd_code==1000)
			m_bExecuteResult = true;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool CCloseConnectionRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	// -----------------------------------------------------------------

	CSetPMICTimeRequest::CSetPMICTimeRequest(bool& bExecuteResult,BYTE nMode, BYTE nMonth, BYTE nDay, WORD nYear, BYTE nHour, BYTE nMin, BYTE nSec)
		: CDiagRequestBase(_T("CSetPMICTimeRequest"))
		, m_bExecuteResult(bExecuteResult)
	{
		m_bExecuteResult = false;

		ftm_pmic_pkt_type req = {0};
		req.cmd_code = DIAG_SUBSYS_CMD_F;
		req.subsys_id = DIAG_SUBSYS_FTM;
		req.subsys_cmd_code = 11;
		req.Mode = nMode;
		req.Month = nMonth;
		req.Day = nDay;
		req.YEAR = nYear;
		req.Hour = nHour;
		req.Min = nMin;
		req.Sec = nSec;
		CBufferPtr ptBufferSend;
		Encode((char*)&req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CSetPMICTimeRequest::~CSetPMICTimeRequest(){}
	void CSetPMICTimeRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		ftm_pmic_pkt_type resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		if(resp.unknown[1] == 0x10)
			m_bExecuteResult = true; 

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool CSetPMICTimeRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
	// -----------------------------------------------------------------

	CGetPMICTimeRequest::CGetPMICTimeRequest(bool& bExecuteResult, BYTE nMode/*2=24hrs*/, BYTE& nMonth, BYTE& nDay, WORD& nYear, BYTE& nHour, BYTE& nMin, BYTE& nSec)
		: CDiagRequestBase(_T("CGetPMICTimeRequest"))
		, m_bExecuteResult(bExecuteResult)
		,m_nMonth(nMonth)
		,m_nDay(nDay)
		,m_nYear(nYear)
		,m_nHour(nHour)
		,m_nMin(nMin)
		,m_nSec(nSec)
	{
		m_bExecuteResult = false;

		char szCmpBuf[] = {0x4B, 0x0B , 0x0B , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x01 , 0x78 , 0x4C , 0x4C , 0xC8
 , 0xEE , 0x28 , 0x00 , 0x4C };
		CBufferPtr ptBufferSend;
		Encode((char*)szCmpBuf, sizeof(szCmpBuf), ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CGetPMICTimeRequest::~CGetPMICTimeRequest(){}
	void CGetPMICTimeRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		ftm_pmic_pkt_type resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		if(resp.unknown[7] == 0x01)
		{
			m_bExecuteResult = true; 
			m_nMonth = resp.Month;
			m_nDay = resp.Day;
			m_nYear = resp.YEAR;
			m_nHour = resp.Hour;
			m_nMin = resp.Min;
			m_nSec = resp.Sec;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool CGetPMICTimeRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	// -----------------------------------------------------------------

	CReadQfuseRequest::CReadQfuseRequest(unsigned int& nReg1, unsigned int& nReg2, bool& bExecuteResult)
		: CDiagRequestBase(_T("CReadQfuseRequest"))
		, m_nReg1(nReg1), m_nReg2(nReg2), m_bExecuteResult(bExecuteResult)
	{
		m_bExecuteResult = false;

		QFUSE_Read_REQ req={0};
		req.command_code = DIAG_SUBSYS_CMD_F;
		req.subsys_id = DIAG_SUBSYS_FTM;
		req.subsys_cmd_code = 0x18;
		req.FTM_REQ_DATA_LEN = 0x02;
		req.FTM_RSP_PKT_SIZE = 0x20;
		CBufferPtr ptBufferSend;
		Encode((char*)&req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CReadQfuseRequest::~CReadQfuseRequest(){}
	void CReadQfuseRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		QFUSE_Read_RESP resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),sizeof(resp));
		if(resp.command_code == DIAG_SUBSYS_CMD_F && resp.subsys_id == DIAG_SUBSYS_FTM && resp.status == 0)
		{
			m_nReg1 = resp.nConfig_Register[0];
			m_nReg2 = resp.nConfig_Register[1];
			m_bExecuteResult = true;
		}
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool CReadQfuseRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}
	// -----------------------------------------------------------------
	CWriteQfuseRequest::CWriteQfuseRequest(WORD chain_select, int config_register[5],  int& nQfusedStatus, bool& bExecuteResult)
		: CDiagRequestBase(_T("CWriteQfuseRequest"))
		, m_bExecuteResult(bExecuteResult), m_nQfusedStatus(nQfusedStatus)
	{
		m_bExecuteResult = false;

		QFUSE_Write_REQ req = {0};
		req.command_code = DIAG_SUBSYS_CMD_F;
		req.subsys_id = DIAG_SUBSYS_FTM;
		req.FTM_CDM_ID = 1;
		req.subsys_cmd_code = 0x18;
		req.FTM_REQ_DATA_LEN = 12;
		req.FTM_RSP_PKT_SIZE = 22;
		req.chain_select = chain_select;
		for(int i=0; i<5; i++)
			req.config_register[i] = config_register[i];
		CBufferPtr ptBufferSend;
		Encode((char*)&req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CWriteQfuseRequest::~CWriteQfuseRequest(){}
	void CWriteQfuseRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		QFUSE_Write_RESP resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),sizeof(resp));
		if(resp.command_code == DIAG_SUBSYS_CMD_F && resp.subsys_id == DIAG_SUBSYS_FTM)
		{
			m_nQfusedStatus = resp.status; /*0=success, 1=Read Operation failed, 2=Write Operation Failed, 3=Operation on selected chain not supported*/
			m_bExecuteResult = true;
		}
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool CWriteQfuseRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	// -----------------------------------------------------------------
	CReadSWVersionRequest::CReadSWVersionRequest(char* szReadValue1024, long& nLength)
		: CDiagRequestBase(_T("CReadSWVersionRequest"))
		, m_szReadValue1024(szReadValue1024), m_nLength(nLength)
	{
		BYTE DIAGcmd = DIAG_EXT_BUILD_ID_F;
		CBufferPtr ptBufferSend;
		Encode((char*)&DIAGcmd, sizeof(DIAGcmd), ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CReadSWVersionRequest::~CReadSWVersionRequest(){}
	void CReadSWVersionRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		toolsdiag_ext_build_id_rsp_type resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		strcpy(m_szReadValue1024, resp.ver_strings);
		m_nLength = strlen(m_szReadValue1024);
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool CReadSWVersionRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	// -----------------------------------------------------------------
	CUnlockSPCRequest::CUnlockSPCRequest(const char *szSPC, bool &bUnlockOK, bool& bExecuteResult)
		: CDiagRequestBase(_T("CUnlockSPCRequest"))
		, m_bUnlockOK(bUnlockOK), m_bExecuteResult(bExecuteResult)
	{
		m_bExecuteResult = false;
		diagpkt_sec_code_req_type req = {0};
		req.cmd_code = DIAG_SPC_F;
		memcpy(req.digits, szSPC, 6);

		CBufferPtr ptBufferSend;
		Encode((char*)&req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CUnlockSPCRequest::~CUnlockSPCRequest(){}
	void CUnlockSPCRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		diagpkt_sec_code_resp_type resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(),sizeof(resp));
		if(resp.cmd_code == DIAG_SPC_F)
		{
			m_bExecuteResult = true;
			m_bUnlockOK = resp.sec_code_ok;
		}
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool CUnlockSPCRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

}