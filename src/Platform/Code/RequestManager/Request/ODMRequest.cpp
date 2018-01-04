#include "stdafx.h"
#include "ODMRequest.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"

namespace PLATFORM_TOOL
{
	//CMotoRequestBase

	CMotoRequestBase::CMotoRequestBase(int nVersion)
		:CDiagRequestBase(_TC("CMotoRequestBase"))
	{
		char szBuf[4]={0x4B,0x43,0x1,0x0};
		if(nVersion == 0)//For Athens only
			;
		else if(nVersion == 1)
			szBuf[1] = 0xF7;

		CBufferPtr ptBufferSend;
		Encode(szBuf,4,ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CMotoRequestBase::~CMotoRequestBase()
	{
	}

	//CWriteExceptionToEFSRequestBase

	CWriteExceptionToEFSRequest::CWriteExceptionToEFSRequest(int nVersion)
		:CDiagRequestBase(_TC("CWriteExceptionToEFSRequest"))
	{
		char szBuf[4]={0x4B,0x42,0x0C,0x00};
		if(nVersion == 0)//For Athens only
			;
		else if(nVersion == 1)
			szBuf[1] = 0xF6;

		CBufferPtr ptBufferSend;
		Encode(szBuf,4,ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CWriteExceptionToEFSRequest::~CWriteExceptionToEFSRequest()
	{
	}
		
	CWriteSIMDataRequest::CWriteSIMDataRequest(BYTE *pSimDataBuf, int nSimDataBufLen,int& nResult)
		:CDiagRequestBase(_TC("CWriteSIMDataRequest"))
		,m_nResult(nResult)
	{
		GSIDDIAG_SIM_SECURE_STORE_REQ Req = {0};
		Req.command_code = DIAG_SUBSYS_CMD_VER_2_F;
		Req.subsys_id = DIAG_SUBSYS_GSDI;
		Req.subcommand = 87;
		Req.aps_as_transport =  1;
		Req.subsys_cmd_code = 72; //GSDIDIAG_SIM_SECURE_STORE_CMD
		Req.slot_id = 0x01;
		Req.sim_app_in_len = 16;
		Req.simsecure_data_len = nSimDataBufLen;
		memcpy(Req.simsecure_data, pSimDataBuf, nSimDataBufLen);

		CBufferPtr ptBufferSend;
		Encode((const char*)&Req,sizeof(GSIDDIAG_SIM_SECURE_STORE_REQ),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CWriteSIMDataRequest::~CWriteSIMDataRequest()
	{
	}

	void CWriteSIMDataRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		GSIDDIAG_SIM_SECURE_STORE_RESP Resp = {0};
		memcpy(&Resp,ptBufferReceived->GetData(),sizeof(Resp));
		m_nResult = Resp.status;
		
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	
	CFailureByteRequest::CFailureByteRequest(WORD nFunction,BYTE& byteFailure,bool& bSuccess,int nType)
		:CDiagRequestBase(_TC("CWriteFailureByteRequest"))
		,m_nFunction(nFunction)
		,m_byteFailure(byteFailure)
		,m_bSuccess(bSuccess)
        ,m_nType(nType)
	{

        FailureByte_REQ Req = {0x4B, m_nType == 0 ? 0x43 : 0xF7};
		Req.subsys_cmd_code = 0x04;
		Req.FailureByte = m_byteFailure;


		CBufferPtr ptBufferSend;
		Encode((const char*)&Req,sizeof(FailureByte_REQ),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CFailureByteRequest::~CFailureByteRequest()
	{
	}

	void CFailureByteRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		FailureByte_RESP Resp = {0};
		memcpy(&Resp,ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bSuccess = Resp.Success == 1;
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CWriteDefaultPhoneCodeRequest::CWriteDefaultPhoneCodeRequest(bool& bSuccess)
		:CDiagRequestBase(_TC("CWriteDefaultPhoneCodeRequest"))
		,m_bSuccess(bSuccess)
	{
		m_bSuccess = false;
		//default phonecode 1234
		//SendRawRequest 128 33 72 00 0 0 0 0 94 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 4 0 0 0 49 50 51 52

		BYTE szRAWCommand[] = {128, 33, 72, 0, 0, 0, 0, 0, 94, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 49, 50, 51, 52};

		CBufferPtr ptBufferSend;
		Encode((const char*)szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CWriteDefaultPhoneCodeRequest::~CWriteDefaultPhoneCodeRequest()
	{
	}

	void CWriteDefaultPhoneCodeRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		BYTE szSuccess[] = {0x80 ,0x21 ,0x48 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00 ,0x01 ,0x00,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x5E ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 };
		//BYTE szFail[] = {0x80 ,0x21 ,0x48 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00 ,0x01 ,0x00,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x5E ,0x00 ,0x00 ,0x00 ,0x14 ,0x00 ,0x00 ,0x00 };
		m_bSuccess = memcmp(ptBufferReceived->GetData(),szSuccess,sizeof(szSuccess)) == 0;
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CFreezeOTPRequest::CFreezeOTPRequest(bool& bSuccess)
		:CDiagRequestBase(_TC("CFreezeOTPRequest"))
		,m_bSuccess(bSuccess)
	{
		m_bSuccess = false;

		GSDIDIAG_SECURE_DEVICE_ACTIVATE_REQ req = {0};
		req.command_code = DIAG_SUBSYS_CMD_VER_2_F;
		req.subsys_id = DIAG_SUBSYS_GSDI;
		req.subsys_cmd_code = 72;
		req.aps_as_transport = 1;
		req.subcommand = 86;
		req.slot_id = 1;
		req.sim_app_in_len = 16;

		CBufferPtr ptBufferSend;
		Encode((const char*)&req,sizeof(GSDIDIAG_SECURE_DEVICE_ACTIVATE_REQ),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CFreezeOTPRequest::~CFreezeOTPRequest()
	{
	}

	void CFreezeOTPRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{		
		GSDIDIAG_SECURE_DEVICE_ACTIVATE_RESP resp = {0};
		memcpy(&resp,ptBufferReceived->GetData(), sizeof(GSDIDIAG_SECURE_DEVICE_ACTIVATE_RESP));
		m_bSuccess = resp.status == 0;
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CQueryFreezeOTPRequest::CQueryFreezeOTPRequest(bool& bSuccess,int &nLastError)
		:CDiagRequestBase(_TC("CFreezeOTPRequest"))
		,m_bSuccess(bSuccess)
		,m_nLastError(nLastError)
	{
		m_bSuccess = false;
		m_nLastError = 0;

		OTP_STATUS_CHK_REQ req = {0};

		req.command_code = 139;
		req.subsys_id = 250;
		req.subsys_cmd_code = 2;
		req.subsys_cmd_id = 0;
		req.req_data_length = 2;
		req.rsp_pkt_size = 12;
		req.protect_code = 3125;

		CBufferPtr ptBufferSend;
		Encode((const char*)&req,sizeof(OTP_STATUS_CHK_REQ),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CQueryFreezeOTPRequest::~CQueryFreezeOTPRequest()
	{
	}

	void CQueryFreezeOTPRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{		
		OTP_STATUS_CHK_RESP resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(), sizeof(OTP_STATUS_CHK_RESP));
		if(resp.otp_status == 2)
			m_bSuccess = true;
		else if(resp.otp_status == 0)
			m_nLastError = DIAG_ERROR_WRONG_OTP_PROTECTED_CODE;
		else if(resp.otp_status == 1)
			m_nLastError = DIAG_ERROR_EMPTY_OTP;
		else if(resp.otp_status == 3)
			m_nLastError = DIAG_ERROR_DAMAGED_OTP;
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	
	//------------------------------------------------------------------------------//

	CQueryHDVersionRequest::CQueryHDVersionRequest(int& nPhoneCodeVersion, int& nBootloaderVersion, bool& bExecuteResult)
		:m_nPhoneCodeVersion(nPhoneCodeVersion), m_nBootloaderVersion(nBootloaderVersion), m_bExecuteResult(bExecuteResult)
	{
		OTP_STATUS_CHK_REQ req = {0};
		req.command_code = 139;
		req.subsys_id = 250;
		req.subsys_cmd_code = 2;
		req.subsys_cmd_id = 0;
		req.req_data_length = 2;
		req.rsp_pkt_size = 12;
		req.protect_code = 3125;
		CBufferPtr ptBufferSend;
		Encode((char*)&req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CQueryHDVersionRequest::~CQueryHDVersionRequest(){}
	void CQueryHDVersionRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		OTP_STATUS_CHK_RESP resp = {0};
		memcpy(&resp, ptBufferReceived->GetData(), sizeof(resp));
		if(resp.otp_status != 3)
		{
			m_nPhoneCodeVersion = resp.phonecode_ver_baseline;
			m_nBootloaderVersion = resp.boot_ver_baseline;
			m_bExecuteResult = true;
		}
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool CQueryHDVersionRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	//------------------------------------------------------------------------------//

	CReleaseDateRequest::CReleaseDateRequest(char* szDate12, char* szTime9, bool& bExecuteResult)
	 :m_szDate12(szDate12), m_szTime9(szTime9), m_bExecuteResult(bExecuteResult)
	{
		m_bExecuteResult = false;

		Version_Number_Req req ={0};
		req.cmd_code = DIAG_VERNO_F;
		CBufferPtr ptBufferSend;
		Encode((char*)&req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}
	CReleaseDateRequest::~CReleaseDateRequest(){}
	void CReleaseDateRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		Version_Number_Resp resp ={0};
		if(resp.cmd_code == DIAG_VERNO_F)
		{
			memcpy(m_szDate12, (const char*)resp.Rel_Date, 11);
			memcpy(m_szTime9, (const char*)resp.Rel_Time, 8);
			m_bExecuteResult = true;
		}
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
	bool CReleaseDateRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	CGSensorRequest::CGSensorRequest(bool& bSuccess,int& nOffsetX,int& nOffsetY,int& nOffsetZ)
		:CDiagRequestBase(_TC("CGSensorRequest"))
		,m_bSuccess(bSuccess)
		,m_nX(nOffsetX)
		,m_nY(nOffsetY)
		,m_nZ(nOffsetZ)
		,m_bIsGSensorWork(bSuccess)//Unused
		,m_bIsUpWork(bSuccess)//Unused
		,m_nVersion(4)
	{
		char szBuf[4]={0x4B,0xF5,0x05,0x00};
		CBufferPtr ptBufferSend;
		Encode(szBuf,4,ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CGSensorRequest::CGSensorRequest(int nFunction,int& nTemp, bool& bSuccess)
	:CDiagRequestBase(_TC("CGSensorRequest"))
	,m_bSuccess(bSuccess)
	,m_nX(nTemp)
	,m_nY(nTemp)
	,m_nZ(nTemp)
	,m_bIsGSensorWork(bSuccess)//Unused
	,m_bIsUpWork(bSuccess)//Unused
	,m_nVersion(3)
	{
		char szBuf[4]={0x4B,0xF5,nFunction,0x00};
		CBufferPtr ptBufferSend;
		Encode(szBuf,4,ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CGSensorRequest::CGSensorRequest(int& nX,int& nY,int& nZ,  bool& bSuccess)
		:CDiagRequestBase(_TC("CGSensorRequest"))
		,m_bSuccess(bSuccess)
		,m_nX(nX)
		,m_nY(nY)
		,m_nZ(nZ)
		,m_bIsGSensorWork(bSuccess)//Unused
		,m_bIsUpWork(bSuccess)//Unused
		,m_nVersion(2)
	{
		m_nX = 0;
		m_nY = 0;
		m_nZ = 0;
		m_bSuccess = false;

		char szBuf[4]={0x4B,0xF5,0x02,0x00};
		CBufferPtr ptBufferSend;
		Encode(szBuf,4,ptBufferSend);
		SetSendData(ptBufferSend);
	}

	//------------------------------------------------------------------------------//	
	CGSensorRequest::CGSensorRequest(bool& bIsGSensorWork,bool& bIsUpWork,int& nTemp)
		:CDiagRequestBase(_TC("CGSensorRequest"))
		,m_bIsGSensorWork(bIsGSensorWork)
		,m_bIsUpWork(bIsUpWork)
		,m_bSuccess(bIsUpWork)//Unused
		,m_nX(nTemp)//Unused
		,m_nY(nTemp)//Unused
		,m_nZ(nTemp)//Unused
		,m_nVersion(1)
	{
		m_bIsGSensorWork = false;
		m_bIsUpWork = false;

		char szBuf[4]={0x4B,0xF5,0x01,0x00};
		CBufferPtr ptBufferSend;
		Encode(szBuf,4,ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CGSensorRequest::~CGSensorRequest()
	{
	}

	void CGSensorRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		if(m_nVersion == 1)
		{
#pragma pack(push, 1)
			typedef struct  
			{
				BYTE command_code;
				BYTE subsys_id;
				BYTE subsys_id1;//0x01
				BYTE subsys_id2;//0x00
				BYTE gsensor_id;  
				BYTE uP_id;
			} diag_get_gsensor_id_rsp_type;
#pragma pack(pop)

			diag_get_gsensor_id_rsp_type resp = {0};
			memcpy(&resp,ptBufferReceived->GetData(),ptBufferReceived->GetLength());
			m_bIsGSensorWork = resp.gsensor_id == 0x02;
			m_bIsUpWork = resp.uP_id == 0x01;
		}
		else if(m_nVersion == 2)
		{
#pragma pack(push, 1)
			typedef struct  
			{
				BYTE command_code;
				BYTE subsys_id;
				BYTE subsys_id1;//0x02
				BYTE subsys_id2;//0x00
				int nX; 
				int nY;
				int nZ;
				BYTE byteSuccess;
			} diag_get_gsensor_id_rsp_type;
#pragma pack(pop)

			diag_get_gsensor_id_rsp_type resp = {0};
			memcpy(&resp,ptBufferReceived->GetData(),sizeof(diag_get_gsensor_id_rsp_type));
			
			m_nX = resp.nX;
			m_nY = resp.nY;
			m_nZ = resp.nZ;
			m_bSuccess = resp.byteSuccess == 1;
		}
		else if(m_nVersion == 3)
		{			
#pragma pack(push, 1)
			typedef struct  
			{
				BYTE command_code;
				BYTE subsys_id;
				BYTE subsys_id1;//0x02
				BYTE subsys_id2;//0x00
				BYTE byteSuccess;
			} diag_get_gsensor_id_rsp_type;
#pragma pack(pop)
			diag_get_gsensor_id_rsp_type resp = {0};
			if(ptBufferReceived->GetLength() >= 5)
				memcpy(&resp,ptBufferReceived->GetData(),sizeof(diag_get_gsensor_id_rsp_type));

			if(resp.command_code == 0x4b && resp.subsys_id == 0xF5)
			{
				m_nX = resp.byteSuccess;
				switch(resp.subsys_id1)
				{
				case 3:
					/*
					DIAG_CAL_GSENSOR_OK	0
					DIAG_CAL_GSENSOR_SKIP	1
					DIAG_CAL_GSENSOR_FAIL	2
					*/
					m_bSuccess = (m_nX == 0 || m_nX == 1);
					break;
				case 6:
					/*
					DIAG_ST_GSENSOR_OK	0
					DIAG_ST_GSENSOR_ST0_FAIL	1
					DIAG_ST_GSENSOR_ST1_FAIL	2
					DIAG_ST_GSENSOR_VECTOR_ST_FAIL	3
					*/
					m_bSuccess = (m_nX == 0);
					break;
				case 7:
					/*
					DIAG_GSENSOR_TEST_ALL_PASS	0
					DIAG_GSENSOR_TEST_ST0_FAIL	1
					DIAG_GSENSOR_TEST_ST1_FAIL	2
					DIAG_GSENSOR_TEST_VECTOR_ST_FAIL	3
					DIAG_GSENSOR_TEST_CAL_FAIL	4
					*/
					m_bSuccess = (m_nX == 0);
					break;
				};
			}
		}
		else if(m_nVersion == 4)
		{			
#pragma pack(push, 1)
			typedef struct  
			{
				BYTE command_code;
				BYTE subsys_id;
				BYTE subsys_id1;//0x05
				BYTE subsys_id2;//0x00
				short nOffsetX;
				short nOffsetY;
				short nOffsetZ;
				BYTE byteSuccess;
			} diag_get_gsensor_id_rsp_type;
#pragma pack(pop)
			diag_get_gsensor_id_rsp_type resp = {0};
			if(ptBufferReceived->GetLength() >= 5)
			{
				memcpy(&resp,ptBufferReceived->GetData(),sizeof(diag_get_gsensor_id_rsp_type));
				m_nX = resp.nOffsetX;
				m_nY = resp.nOffsetY;
				m_nZ = resp.nOffsetZ;
				m_bSuccess = resp.byteSuccess == 1;
			}
		}
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CDiagRAWRequest::CDiagRAWRequest(const char* szCmdInput, int nCmdLength, bool bAddCRC,bool bHasReturn)
		:CDiagRequestBase(5000, bHasReturn)
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

	CDiagRAWRequest::~CDiagRAWRequest(){}

	
	CByPassCellTemperatureCheckRequest::CByPassCellTemperatureCheckRequest()
		:CDiagRequestBase(5000, false)
	{
		BYTE szRAWCommand[] = {75, 36, 8, 0, 0, 0};

		CBufferPtr ptBufferSend;
		Encode((const char*)szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CByPassCellTemperatureCheckRequest::~CByPassCellTemperatureCheckRequest(){}

	
	CAskLavernockVarientRequest::CAskLavernockVarientRequest(int& nVersion)
		:CDiagRequestBase(_T("CAskLavernockVarientRequest"))
		,m_nVarientVersion(nVersion)
		,m_nEVTVersion(nVersion)
		,m_bNewVersion(false)
	{
		BYTE szRAWCommand[] = {75, 9, 12, 0};

		CBufferPtr ptBufferSend;
		Encode((const char*)szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CAskLavernockVarientRequest::CAskLavernockVarientRequest(int& nVarientVersion,int& nEVTVersion)
		:CDiagRequestBase(_T("CAskLavernockVarientRequest2"))
		,m_nVarientVersion(nVarientVersion)
		,m_nEVTVersion(nEVTVersion)
		,m_bNewVersion(true)
	{
		BYTE szRAWCommand[] = {75, 9, 12, 0};

		CBufferPtr ptBufferSend;
		Encode((const char*)szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}	
	
	CAskLavernockVarientRequest::~CAskLavernockVarientRequest(){}

	void CAskLavernockVarientRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{		
		if(m_bNewVersion)
		{
#pragma pack(push, 1)
			typedef struct  
			{
				BYTE command_code;
				BYTE subsys_id;
				short mobile_id; // Mode id - set to 12 for get Lavernock HW ID/Variant Version cmd.
				BYTE VarientVersion; // GLAVS1A: 1  GLAVS2A: 2  GLAVS3A: 3  GLAVS4A: 4
				BYTE HWVersion; //Pilotrun Version
				BYTE HWMinorVersion;//Pilotrun minor Version
				BYTE DUMMY; //Name : 1: Pilotrun 2: PCR
			} diag_get_lavernock_rsp_type;
#pragma pack(pop)
			diag_get_lavernock_rsp_type resp = {0};
			memcpy(&resp,ptBufferReceived->GetData(),ptBufferReceived->GetLength());

			m_nVarientVersion = resp.VarientVersion;
			m_nEVTVersion = resp.HWVersion;
		}
		else
		{
#pragma pack(push, 1)
			typedef struct  
			{
				BYTE command_code;
				BYTE subsys_id;
				short mobile_id;
				BYTE HWVersion;
                BYTE DUMMY;
			} diag_get_lavernock_rsp_type;
#pragma pack(pop)
            diag_get_lavernock_rsp_type resp = {0};
			memcpy(&resp,ptBufferReceived->GetData(),ptBufferReceived->GetLength());

			m_nVarientVersion = resp.HWVersion;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CChargingSwitchRequest::CChargingSwitchRequest(short Mode_ID,bool bEnable,bool& bSuccess)
		:CDiagRequestBase(_T("CChargingSwitchRequest"))
		,m_bSuccess(bSuccess)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 36 for PM
		Mode ID	2	Mode ID ¡V set to 6 for USB charging switch.
		Mode ID	2	Mode ID ¡V set to 4 for coin cell charging switch.
		Enable/Disable	1	Enable/Disable ¡V Enable: 1  Disable: 0
		*/
		BYTE szRAWCommand[] = {75, 36, 6, 0,1};
		szRAWCommand[2] = Mode_ID;
		if(bEnable)
			szRAWCommand[4] = 1;
		else
			szRAWCommand[4] = 0;

		CBufferPtr ptBufferSend;
		Encode((const char*)szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CChargingSwitchRequest::~CChargingSwitchRequest()
	{
	}

	void CChargingSwitchRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 36 for PM
		Mode ID	2	Mode ID ¡V set to 6 for USB charging switch.
		Result	2	Result ¡V USB charging status has been updated: 1
		USB charging status hasn¡¦t been updated: 0 or 1
		*/
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short Mode_ID;
			short Result;
		} diag_USBChargingSwitch_rsp_type;
#pragma pack(pop)
		diag_USBChargingSwitch_rsp_type resp = {0};
		memcpy(&resp,ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bSuccess = (resp.Result == 0 || resp.Result == 1);
		assert(m_bSuccess);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CKeypadRequest::CKeypadRequest(short Mode_ID,bool bEnable,bool& bSuccess)
		:CDiagRequestBase(_T("CKeypadRequest"))
		,m_bSuccess(bSuccess)
		,m_nMode_ID(Mode_ID)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 16 for handset
		Command code	2	4 : keypad backlight
		Command type	1	0 : turn off keypad backlight
		1 : turn on keypad backlight

		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 16 for handset
		Command code	2	5 : key press
		Command type	1	0 : register
		1 : deregister
		*/
		BYTE szRAWCommand[] = {75, 16, 4, 0,1};
		szRAWCommand[2] = m_nMode_ID;		
		if(m_nMode_ID == 4)
		{//4 : keypad backlight
			if(bEnable)
				szRAWCommand[4] = 1;
			else
				szRAWCommand[4] = 0;
		}
		else if(m_nMode_ID == 5)
		{//5 : key press
			if(bEnable)
				szRAWCommand[4] = 0;
			else
				szRAWCommand[4] = 1;
		}
		else assert(0);//undefine

		CBufferPtr ptBufferSend;
		Encode((const char*)szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CKeypadRequest::~CKeypadRequest()
	{
	}

	void CKeypadRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 36 for PM
		Mode ID	2	Mode ID ¡V set to 6 for USB charging switch.
		Result	2	Result ¡V USB charging status has been updated: 1
		USB charging status hasn¡¦t been updated: 0
		*/
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short Mode_ID;
		} diag_Keypad_rsp_type;
#pragma pack(pop)
		diag_Keypad_rsp_type resp = {0};
		memcpy(&resp,ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bSuccess = resp.Mode_ID == m_nMode_ID;
		assert(m_bSuccess);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CDisplayRequest::CDisplayRequest(short Color565,BYTE byteBacklight,bool& bSuccess)
		:CDiagRequestBase(_T("CDisplayRequest"))
		,m_bSuccess(bSuccess)
		,m_Color565(Color565)
		,m_byteBacklight(byteBacklight)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 11 for FTM
		Mode ID	2	Mode ID ¡V set to 0x8000 for customer extension
		EXT ID	2	EXD ID ¡V set to 2 for LCD test
		Data len	2	Data length ¡V set to 3 (color_byte1 color_byte2 backlight_level)
		Rsp len	2	Response length ¡V set to 0
		Color	2	Color bytes (RGB565)
		BL level	1	Backlight level, 0~255

		EX: (must be in FTM mode)
		Black:	sendrawrequest				75 11 0 128 2 0 3 0 0 0 0 0 255
		Red:	sendrawrequest				75 11 0 128 2 0 3 0 0 0 0 248 255
		Green:	sendrawrequest			75 11 0 128 2 0 3 0 0 0 224 7 255
		Blue:	sendrawrequest				75 11 0 128 2 0 3 0 0 0 31 0 255
		White:	sendrawrequest				75 11 0 128 2 0 3 0 0 0 255 255 255
		Backlight off: sendrawrequest		75 11 0 128 2 0 3 0 0 0 0 0 0
		*/		
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_Id;
			short Mode_ID;
			short Ext_ID;
			short Data_Len;
			short Rsp_Len;
			short Color;
			BYTE Backlight_Level;
		} diag_Display_res_type;
#pragma pack(pop)
		diag_Display_res_type req = {75,11,0x8000,2,3,0,0,255};		
		req.Color = m_Color565;
		req.Backlight_Level = m_byteBacklight;

		CBufferPtr ptBufferSend;
		Encode((const char*)&req,sizeof(diag_Display_res_type),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CDisplayRequest::~CDisplayRequest()
	{
	}

	void CDisplayRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 11 for FTM
		Mode ID	2	Mode ID ¡V set to 0x8000 for customer extension
		EXT ID	2	EXD ID ¡V set to 2 for LCD test
		Data len	2	Data length ¡V set to 3 (color_byte1 color_byte2 backlight_level)
		Rsp len	2	Response length ¡V set to 0
		Color	2	Color bytes (RGB565)
		BL level	1	Backlight level, 0~255
		*/
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_Id;
			short Mode_ID;
			short Ext_ID;
			short Data_Len;
			short Rsp_Len;
			short Color;
			BYTE Backlight_Level;
		} diag_Display_rsp_type;
#pragma pack(pop)
		diag_Display_rsp_type resp = {0};
		memcpy(&resp,ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bSuccess = (resp.command_code == 0x4B) && (resp.Color == m_Color565);
		assert(m_bSuccess);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	
	CVibratorRequest::CVibratorRequest(bool bEnable,bool& bSuccess)
		:CDiagRequestBase(_T("CVibratorRequest"))
		,m_bSuccess(bSuccess)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 36 for PM
		Mode ID	2	Mode ID ¡V set to 3 for vibrator switch.
		Enable/Disable	1	Enable/Disable ¡V Enable: 1  Disable: 0
		*/
		BYTE szRAWCommand[] = {75, 36, 3, 0,1};
		if(bEnable)
			szRAWCommand[4] = 1;
		else
			szRAWCommand[4] = 0;

		CBufferPtr ptBufferSend;
		Encode((const char*)szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}
	
	void CVibratorRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 36 for PM
		Mode ID	2	Mode ID ¡V set to 3 for vibrator switch.
		Result	2	Result ¡V Success: 0
		*/
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short Mode_ID;
			short RESULT;
		} diag_vibrator_rsp_type;
#pragma pack(pop)
		diag_vibrator_rsp_type resp = {0};
		memcpy(&resp,ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bSuccess = (resp.RESULT == 0 && resp.command_code == 0x4B);
		assert(m_bSuccess);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CReadCoincellVoltageRequest::CReadCoincellVoltageRequest(int& nVoltage,bool& bSuccess)
		:CDiagRequestBase(_T("CReadCoincellVoltageRequest"))
		,m_bSuccess(bSuccess)
		,m_nVoltage(nVoltage)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 36 for PM
		Mode ID	2	Mode ID ¡V set to 11 for read coin cell voltage.
		*/
		BYTE szRAWCommand[] = {75, 36, 11, 0};

		CBufferPtr ptBufferSend;
		Encode((const char*)szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}
	
	void CReadCoincellVoltageRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID ¡V SUBSYS_ID is set to 36 for PM
		Mode ID	2	Mode ID ¡V set to 11 for read coin cell voltage.
		Result	2	Coin Cell Voltage in mV.
		*/
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short Mode_ID;
			short RESULT;
		} diag_coincell_rsp_type;
#pragma pack(pop)
		diag_coincell_rsp_type resp = {0};
		memcpy(&resp,ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_nVoltage = resp.RESULT;
		m_bSuccess = (resp.RESULT >= 0 && resp.command_code == 0x4B);
		assert(m_bSuccess);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CFlipStatusRequest::CFlipStatusRequest(bool& bOpenFlip,bool& bSuccess)
		:CDiagRequestBase(_T("CFlipStatusRequest"))
		,m_bSuccess(bSuccess)
		,m_bOpenFlip(bOpenFlip)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 138 for this message
		*/
		BYTE szRAWCommand[] = {138};

		CBufferPtr ptBufferSend;
		Encode((const char*)szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}
	
	void CFlipStatusRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 138 for this message
		Result	2	0: Undefined  1: Open  2: Close.
		*/
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			short RESULT;
		} diag_rsp_type;
#pragma pack(pop)
		diag_rsp_type resp = {0};
		memcpy(&resp,ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bOpenFlip = resp.RESULT == 1;
		m_bSuccess = (resp.RESULT != 0 && resp.command_code == 138);
		assert(m_bSuccess);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CCaptureScreenshotRequest::CCaptureScreenshotRequest()
		:CDiagRequestBase(_T("CCaptureScreenshotRequest"))
		,m_bFirst(true)
	{
		BYTE szRAWCommand[] = {
			0x80, 0x10, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x51, 0x50, 0x53,
			0x54, 0x44, 0x69, 0x73, 0x70, 0x43, 0x61, 0x70, 0x74, 0x75, 0x72, 0x65, 0x2E, 0x62, 0x6D, 0x70,
			0x00
		};

		CBufferPtr ptBufferSend;
		Encode((const char*)szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	bool CCaptureScreenshotRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		if(bRes && m_bFirst)
			m_bFirst = false;
		bRes = (m_bFirst == false) && bRes;
		return bRes;
	}

	void CCaptureScreenshotRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CBatteryTemperatureDetectionSwitchRequest::CBatteryTemperatureDetectionSwitchRequest(bool bEnable,bool& bSuccess)
		:CDiagRequestBase(_T("CBatteryTemperatureDetectionSwitchRequest"))
		,m_bSuccess(bSuccess)
	{
		BYTE szRAWCommand[] = {
			0x4B, 0x24, 0x08, 0x00, bEnable ? 0x01 : 0x00
		};

		CBufferPtr ptBufferSend;
		Encode((const char*)szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	bool CBatteryTemperatureDetectionSwitchRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		bool bRes = CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		return bRes;
	}

	void CBatteryTemperatureDetectionSwitchRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		/*
		Field 	Length (bytes)	Description
		CMD_CODE	1	Message ID ¡V CMD_CODE is set to 138 for this message
		Result	2	0: Undefined  1: Open  2: Close.
		*/
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsystem_id;
			short MODEID;
			short RESULT;
		} diag_rsp_type;
#pragma pack(pop)
		diag_rsp_type resp = {0};
		memcpy(&resp,ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bSuccess = (resp.RESULT == 0 && resp.subsystem_id == 0x24);
		assert(m_bSuccess);
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
}
