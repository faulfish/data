#pragma once
#include "QualcommRequest.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"

#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif

namespace PLATFORM_TOOL
{
	// ---------------  Sim Lock Store   -------------------------------
	//   Write encrypted sim data to handset
	class RM_CLASS_API CSimLockStoreRequest: public CDiagRequestBase
	{
		public:
			// Success if status is 0
			CSimLockStoreRequest(const GSIDDIAG_SIM_SECURE_STORE_REQ &simReq, DWORD &cmdStatus, DWORD &status);
			CSimLockStoreRequest(const GSIDDIAG_SIM_SECURE_STORE_REQ_576 &simReq, DWORD &cmdStatus, DWORD &status);
			virtual ~CSimLockStoreRequest(void);
			virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
			virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);

		private:
			GSIDDIAG_SIM_SECURE_STORE_REQ  m_SimLockReq;
			GSIDDIAG_SIM_SECURE_STORE_REQ_576 m_SimLockReq_576;
			DWORD &m_CmdStatus;
			DWORD &m_Status;		
	};

	// ------------   SecurityActivate   -------------------------------
	//   Freeze OTP
	class RM_CLASS_API CSecurityActivateRequest: public CDiagRequestBase
	{
		public:
			// Success if status is 0
			CSecurityActivateRequest(const GSDIDIAG_SECURE_DEVICE_ACTIVATE_REQ &secureReq, DWORD &status);
			virtual ~CSecurityActivateRequest(void);
			virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
			virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);

		private:
			GSDIDIAG_SECURE_DEVICE_ACTIVATE_REQ  m_SecureActivateReq;
			DWORD &m_Status;		
	};

	// Query Sim Lock Status
	class RM_CLASS_API CSimLockStatusRequest: public CDiagRequestBase
	{
		public:
			// Success if status is 0
			CSimLockStatusRequest(diagpkt_subsidy_lock_resp &lockResp);
			virtual ~CSimLockStatusRequest(void);
			virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
			virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);

		private:
			diagpkt_subsidy_lock_req  m_Req;
			diagpkt_subsidy_lock_resp  &m_Resp;
	};

	// Unlock Simlock
	class RM_CLASS_API CSimUnLockRequest: public CDiagRequestBase
	{
	public:
		// Unlock Type
		//
		//	GSDIDIAG_PERSO_NW        = 0x00000000,	// Network Personalization
		//	GSDIDIAG_PERSO_NS        = 0x00000001,	// Network Subset Personalization
		//	GSDIDIAG_PERSO_SP        = 0x00000002,	// Service Provider Personalization
		//	GSDIDIAG_PERSO_CP        = 0x00000003,	// Corporate Personalization
		//	GSDIDIAG_PERSO_SIM       = 0x00000004,	// SIM Personalization
		//	GSDIDIAG_PERSO_PROP1	 = 0x00000010	// Antitheft Personalization
		
		CSimUnLockRequest(int nUnLockType, char *sCode, diagpkt_subsidy_unlock_resp &unlockResp);
		virtual ~CSimUnLockRequest(void);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);

	private:
		diagpkt_subsidy_unlock_req  m_Req;
		diagpkt_subsidy_unlock_resp  &m_Resp;
	};

	// Unlock Phone Code and Master Code
	class RM_CLASS_API CSimUnLockPhoneMasterCodeRequest: public CDiagRequestBase
	{
	public:
		CSimUnLockPhoneMasterCodeRequest(char *sCode, gsdidiag_subsidy_unlock_PhoneCode_resp &unlockResp);
		virtual ~CSimUnLockPhoneMasterCodeRequest(void);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);

	private:
		gsdidiag_subsidy_unlock_PhoneCode_req  m_Req8;
		gsdidiag_subsidy_unlock_PhoneCode16_req m_Req16;
		gsdidiag_subsidy_unlock_PhoneCode_resp  &m_Resp;
	};
	
	class RM_CLASS_API CCheckSIMDataRequest  : public CDiagRequestBase
	{
	public:
		CCheckSIMDataRequest(std::string& strSIMLockName,int& nStatus);
		virtual ~CCheckSIMDataRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		int& m_nStatus;
		bool m_bWaitForDelayResponse;
		std::string& m_strSIMLockName;
	};


	/********************CFC SimData Related Request*****************/
	
	/************************************************************************
	// Class:    CQueryLockStatusRequest
	// Purpose:  For querying NWSCP/SSCP lock status
	// Parameter: OUT diagpkt_subsidy_lock_resp& resp: get lock status
	//            OUT bool& bExecuteResult: if false, sim data doesn't exist 
	//			      (bExecuteResult = cmd_code==DIAG_SUBSYS_CMD_VER_2_F && status==0;)
	************************************************************************/
	class RM_CLASS_API CQueryLockStatusRequest : public CDiagRequestBase
	{
	public:
		CQueryLockStatusRequest(diagpkt_subsidy_lock_resp& resp, bool& bExecuteResult);
		virtual ~CQueryLockStatusRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bExecuteResult;
		diagpkt_subsidy_lock_resp& m_resp;
	};
	
	/************************************************************************
	// Class:    CWriteSimDataRequest
	// Purpose:  For writing sim data
	// Parameter: IN BYTE *pSimDataBuf: simdata
	//			  IN int nSimDataBufLen: simdata size
	//			  OUT bool& bExecuteResult: if false, writing simdata fails
	************************************************************************/
	class RM_CLASS_API CWriteSimDataRequest: public CDiagRequestBase
	{
	public:
		CWriteSimDataRequest(BYTE *pSimDataBuf, int nSimDataBufLen, bool& bExecuteResult);
		virtual ~CWriteSimDataRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bExecuteResult;
	};

	/************************************************************************
	// Class:    CUnlockNWSCPRequest
	// Purpose:  unlock NWSCP separately
	// Parameter: IN BYTE nLockType:
	//////////////////////////////////////////////////////////////////////////
	typedef enum
	{
	GSDIDIAG_PERSO_NW        = 0x00000000,	// Network Personalization
	GSDIDIAG_PERSO_NS        = 0x00000001,	// Network Subset Personalization
	GSDIDIAG_PERSO_SP        = 0x00000002,	// Service Provider Personalization
	GSDIDIAG_PERSO_CP        = 0x00000003,	// Corporate Personalization
	GSDIDIAG_PERSO_SIM       = 0x00000004,	// SIM Personalization
	}GSDIDIAG_PERSO;
	//////////////////////////////////////////////////////////////////////////
	//			  IN char* szLock8: lock code - 8 digits
	//            OUT bool& bUnlockOK: if false, unlock fails
	//			  OUT bool& bExecuteResult: if false, command process fails
	************************************************************************/
	class RM_CLASS_API CUnlockNWSCPRequest: public CDiagRequestBase
	{
	public:
		CUnlockNWSCPRequest(BYTE nLockType, char* szLock8, bool& bUnlockOK, bool& bExecuteResult);
		virtual ~CUnlockNWSCPRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bExecuteResult;
		bool& m_bUnlockOK;
	};

	/************************************************************************
	// Class:    CUnlockPhoneCodeRequest
	// Purpose:  unlock SSCP
	// Parameter: IN char* szLock: MasterCode- can be 8 or 16 digits
	//            OUT bool& bUnlockOK: if false, unlock fails
	//			  OUT bool& bExecuteResult: if false, command process fails
	************************************************************************/
	class RM_CLASS_API CUnlockPhoneCodeRequest: public CDiagRequestBase
	{
	public:
		CUnlockPhoneCodeRequest(char* szLock/*MasterCode*/, bool& bUnlockOK, bool& bExecuteResult);
		virtual ~CUnlockPhoneCodeRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bExecuteResult;
		bool& m_bUnlockOK;
	};
}