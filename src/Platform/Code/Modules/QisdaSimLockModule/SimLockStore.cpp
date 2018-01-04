#include "StdAfx.h"
#include "SimLockStore.h"
#include "..\..\RequestManager\Request\SimLockStoreRequest.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"

CSimLockStore::CSimLockStore(int nCOMPort):CInterfaceBase(nCOMPort)
{
}


CSimLockStore::~CSimLockStore(void)
{
}

BOOL CSimLockStore::StoreSimData(BYTE iSlotId, BYTE iSIM_AppIdLen, BYTE* piSIM_AppId, DWORD iSIM_SecureDataLen, 
								 BYTE* piSIM_SecureData, DWORD *pCmdStatus, DWORD *pStatus, int nTimeout)
{
	if (iSIM_SecureDataLen <= 512)	// Project H21, Athens, AthensBT
		return StoreSimData_512(iSlotId, iSIM_AppIdLen, piSIM_AppId, iSIM_SecureDataLen, piSIM_SecureData, pCmdStatus, pStatus, nTimeout);
	else if (iSIM_SecureDataLen <= 576)	// Project Lavernock
		return StoreSimData_576(iSlotId, iSIM_AppIdLen, piSIM_AppId, iSIM_SecureDataLen, piSIM_SecureData, pCmdStatus, pStatus, nTimeout);
	else
		return FALSE;
}

BOOL CSimLockStore::StoreSimData_512(BYTE iSlotId, BYTE iSIM_AppIdLen, BYTE* piSIM_AppId, DWORD iSIM_SecureDataLen, 
									 BYTE* piSIM_SecureData, DWORD *pCmdStatus, DWORD *pStatus, int nTimeout)
{
	bool bRes = false;

	using namespace PLATFORM_TOOL;
	GSIDDIAG_SIM_SECURE_STORE_REQ cmdReq;

	memset(&cmdReq, 0, sizeof(GSIDDIAG_SIM_SECURE_STORE_REQ));
	cmdReq.aps_as_transport = 1;
	cmdReq.subcommand = 87;
	cmdReq.slot_id = iSlotId;
	cmdReq.sim_app_in_len = iSIM_AppIdLen;
	memcpy(cmdReq.sim_app_id, piSIM_AppId, iSIM_AppIdLen);
	cmdReq.simsecure_data_len = iSIM_SecureDataLen;
	memcpy(cmdReq.simsecure_data, piSIM_SecureData, iSIM_SecureDataLen);

	CRequestPtr ptRequest = new CSimLockStoreRequest(cmdReq, *pCmdStatus, *pStatus);
	ptRequest->SetTimeout(nTimeout);
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

BOOL CSimLockStore::StoreSimData_576(BYTE iSlotId, BYTE iSIM_AppIdLen, BYTE* piSIM_AppId, DWORD iSIM_SecureDataLen, 
									 BYTE* piSIM_SecureData, DWORD *pCmdStatus, DWORD *pStatus, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	GSIDDIAG_SIM_SECURE_STORE_REQ_576 cmdReq;

	memset(&cmdReq, 0, sizeof(GSIDDIAG_SIM_SECURE_STORE_REQ_576));
	cmdReq.aps_as_transport = 1;
	cmdReq.subcommand = 87;
	cmdReq.slot_id = iSlotId;
	cmdReq.sim_app_in_len = iSIM_AppIdLen;
	memcpy(cmdReq.sim_app_id, piSIM_AppId, iSIM_AppIdLen);
	cmdReq.simsecure_data_len = iSIM_SecureDataLen;
	memcpy(cmdReq.simsecure_data, piSIM_SecureData, iSIM_SecureDataLen);

	CRequestPtr ptRequest = new CSimLockStoreRequest(cmdReq, *pCmdStatus, *pStatus);
	ptRequest->SetTimeout(nTimeout);
	bool bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

CSIM_API int WriteSimLockData(UINT nComPort, DWORD iSIM_SecureDataLen, BYTE* piSIM_SecureData, DWORD* pCmdStatus, DWORD* pStatus, int nTimeout)
{
	BYTE ucSimAppLen = 16;
	BYTE ucSimApp[16] = {0};	

	CSimLockStore simStore(nComPort);

	if (simStore.StoreSimData(1, ucSimAppLen, ucSimApp, iSIM_SecureDataLen, piSIM_SecureData, pCmdStatus, pStatus, nTimeout))
		return 0;

	return 1;
}

// --------------------------------------------------------

CSimLockStatus::CSimLockStatus(int nCOMPort):CInterfaceBase(nCOMPort)
{
}


CSimLockStatus::~CSimLockStatus(void)
{
}

bool CSimLockStatus::QueryLockStatus(bool *nwStatus, bool *spStatus, bool *nsStatus, bool *cpStatus, bool *simStatus, bool *masterStatus)
{
	using namespace PLATFORM_TOOL;

	diagpkt_subsidy_lock_resp resp;
	CRequestPtr ptRequest = new CSimLockStatusRequest(resp);

	if (!Execute(ptRequest))
		return false;

	if (resp.status != 0)	// query fail
		return false;

	if (nwStatus != NULL)		*nwStatus = resp.network_personalization_status != 0;
	if (spStatus != NULL)		*spStatus = resp.service_provider_personalization_status != 0;
	if (nsStatus != NULL)		*nsStatus = resp.network_subsuet_personalization_status != 0;
	if (cpStatus != NULL)		*cpStatus = resp.corporation_personalization_status != 0;
	if (simStatus != NULL)		*simStatus = resp.sim_personalization_status != 0;
	if (masterStatus != NULL)	*masterStatus = resp.antitheft_personalization_status != 0;

	return true;
}

bool CSimLockStatus::CheckSIMLock(std::string& strSIMLockName, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	int nStatus;
	CRequestPtr ptRequest = new CCheckSIMDataRequest(strSIMLockName,nStatus);	
	ptRequest->SetTimeout(nTimeout);
	if(Execute(ptRequest))
	{
		bRes = nStatus == 0;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

// --------------------------------------------------------

CSimUnLockStatus::CSimUnLockStatus(int nCOMPort):CInterfaceBase(nCOMPort)
{
}


CSimUnLockStatus::~CSimUnLockStatus(void)
{
}

bool CSimUnLockStatus::UnlockStatus(int unlockType, char *sCode, DWORD *pStatus, int nTimeout)
{
	if (sCode == NULL)
	{
		if (pStatus != NULL)
			*pStatus = 0;
		return true;
	}

	using namespace PLATFORM_TOOL;

	diagpkt_subsidy_unlock_resp resp = {0};
	CRequestPtr ptRequest = new CSimUnLockRequest(unlockType, sCode, resp);

	if (!Execute(ptRequest, nTimeout))
		return false;

	if (pStatus != NULL)
		*pStatus = resp.status;

	return true;
}

bool CSimUnLockStatus::UnlockMasterCode(char *sCode, DWORD *pStatus, int nTimeout)
{
	bool bRes = false;

	if (sCode == NULL || (strlen(sCode) != 8 && strlen(sCode) != 16))
	{
		if (pStatus != NULL)
			*pStatus = 0;
	}
	else
	{
		using namespace PLATFORM_TOOL;

		gsdidiag_subsidy_unlock_PhoneCode_resp resp = {0};
		CRequestPtr ptRequest = new CSimUnLockPhoneMasterCodeRequest(sCode, resp);

		if (Execute(ptRequest, nTimeout))
		{
			if (pStatus != NULL && resp.unknown == 0)
				*pStatus = resp.status;

			bRes=  true;
		}

		if(bRes == false)
			PopLastLog();
	}

	return bRes;
}

bool _stdcall CheckSimDataIsProgrammed(long nComPort)
{
	// Note: when successfully querying lock status, it means that the sim data
	//       is already programmed into the mobile phone.
	CSimLockStatus lockStatus(nComPort);

	return lockStatus.QueryLockStatus();
}


CCFCSimLockRelated::CCFCSimLockRelated(int nComPort):CSimLockStatus(nComPort), CLastError(nComPort)
{}
CCFCSimLockRelated::~CCFCSimLockRelated(){}

bool CCFCSimLockRelated::UnlockNWSCP(BYTE nType, char *szCode, bool& bUnlockOK, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(szCode)
	{
		bool bExecuteResult = false;
		CRequestPtr ptRequest = new CUnlockNWSCPRequest(nType, szCode, bUnlockOK, bExecuteResult);
		if(Execute(ptRequest, nTimeout))
		{
			bRes = bExecuteResult;
			if(bExecuteResult)
				SetCFCLastError(DIAG_ERROR_UNEXPECTED_RESPONSE);
		}
		else
			SetCFCLastError(DIAG_ERROR_TXRX_FAIL);

		if(bRes == false)
			PopLastLog();

	}
	return bRes;
}

bool CCFCSimLockRelated::UnlockNW(char* szLock8, bool& bUnlockOK, int nTimeout)
{
	bool bRes = false;
	bUnlockOK = false; // default unlock fail
	if(UnlockNWSCP(GSDIDIAG_PERSO_NW, szLock8, bUnlockOK, nTimeout))
	{
		bRes = true;
	}
	return bRes;
}
bool CCFCSimLockRelated::UnlockNS(char* szLock8, bool& bUnlockOK, int nTimeout)
{
	bool bRes = false;
	bUnlockOK = false; // default unlock fail
	if(UnlockNWSCP(GSDIDIAG_PERSO_NS, szLock8, bUnlockOK, nTimeout))
	{
		bRes = true;
	}
	return bRes;
}
bool CCFCSimLockRelated::UnlockSP(char* szLock8, bool& bUnlockOK, int nTimeout)
{
	bool bRes = false;
	bUnlockOK = false; // default unlock fail
	if(UnlockNWSCP(GSDIDIAG_PERSO_SP, szLock8, bUnlockOK, nTimeout))
	{
		bRes = true;
	}
	return bRes;
}
bool CCFCSimLockRelated::UnlockCP(char* szLock8, bool& bUnlockOK, int nTimeout)
{
	bool bRes = false;
	bUnlockOK = false; // default unlock fail
	if(UnlockNWSCP(GSDIDIAG_PERSO_CP, szLock8, bUnlockOK, nTimeout))
	{
		bRes = true;
	}
	return bRes;
}
bool CCFCSimLockRelated::UnlockSIM(char* szLock8, bool& bUnlockOK, int nTimeout)
{
	bool bRes = false;
	bUnlockOK = false; // default unlock fail
	if(UnlockNWSCP(GSDIDIAG_PERSO_SIM, szLock8, bUnlockOK, nTimeout))
	{
		bRes = true;
	}
	return bRes;
}

bool CCFCSimLockRelated::UnlockSSCP(char* szLock8, bool& bUnlockOK, int nTimeout) //unlock phone code by using master code
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(szLock8)
	{
		bool bExecuteResult = false;
		CRequestPtr ptRequest = new CUnlockPhoneCodeRequest(szLock8, bUnlockOK, bExecuteResult);
		if(Execute(ptRequest, nTimeout))
		{
			bRes = bExecuteResult;
			if(bExecuteResult==false)
				SetCFCLastError(DIAG_ERROR_UNEXPECTED_RESPONSE);
		}
		else
			SetCFCLastError(DIAG_ERROR_TXRX_FAIL);

		if(bRes == false)
			PopLastLog();

	}
	return bRes;
}
bool CCFCSimLockRelated::IsSSCPLocked(long& nStatus/*0=unlocked, 1=locked*/, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	bool bExecuteResult = false;
	diagpkt_subsidy_lock_resp resp={0};
	CRequestPtr ptRequest = new CQueryLockStatusRequest(resp, bExecuteResult);
	if(Execute(ptRequest, nTimeout))
	{
		if(bExecuteResult)
		{
			nStatus = resp.antitheft_personalization_status == 0 ? 0 : 1;
			bRes = true;
		}
		else
			SetCFCLastError(DIAG_ERROR_UNEXPECTED_RESPONSE);
	}
	else
		SetCFCLastError(DIAG_ERROR_TXRX_FAIL);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCSimLockRelated::IsNWSCPLocked(long& nStatus/*0=unlocked, 1=locked*/, int nTimeout)
{
	bool bRes = false;
	nStatus = 0; // default:unlocked
	bool bNWUnblocked = false; 
	bool bNSUnlocked = false;
	bool bSPUnlocked = false;
	bool bCPUnlocked = false;
	bool bSIMUnlocked = false;
	if(IsNWSCPLocked(bNWUnblocked, bNSUnlocked, bSPUnlocked, bCPUnlocked, bSIMUnlocked, nTimeout))
	{
		bRes = true;
		if(!bNWUnblocked || !bNSUnlocked || !bSPUnlocked || !bCPUnlocked || !bSIMUnlocked)
			nStatus = 1;  // NWSCP is locked if any code is locked
	}
	return bRes;
}
bool CCFCSimLockRelated::IsNWSCPLocked(bool& bNWUnlocked, // Network Personalization
									   bool& bNSUnlocked, // Network Subset Personalization
									   bool& bSPUnlocked, // Service Provider Personalization
									   bool& bCPUnlocked, // Corporate Personalization
									   bool& bSIMUnlocked,// SIM Personalization
									   int nTimeOut)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	bool bExecuteResult = false;
	diagpkt_subsidy_lock_resp resp={0};
	CRequestPtr ptRequest = new CQueryLockStatusRequest(resp, bExecuteResult);
	if(CInterfaceBase::Execute(ptRequest, nTimeOut))
	{
		if(bExecuteResult)
		{
			bNWUnlocked = resp.network_personalization_status == 0;
			bNSUnlocked = resp.network_subsuet_personalization_status == 0;
			bSPUnlocked = resp.service_provider_personalization_status ==0;
			bCPUnlocked = resp.corporation_personalization_status == 0;
			bSIMUnlocked = resp.sim_personalization_status ==0;
			bRes = true;
		}
		else
			SetCFCLastError(DIAG_ERROR_UNEXPECTED_RESPONSE);
	}
	else
		SetCFCLastError(DIAG_ERROR_TXRX_FAIL);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCSimLockRelated::QuerySimDataStatus(bool& bExist, int nTimeOut)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	diagpkt_subsidy_lock_resp resp={0};
	CRequestPtr ptRequest = new CQueryLockStatusRequest(resp, bExist);
	if(CInterfaceBase::Execute(ptRequest, nTimeOut))
	{
		bRes = true;
	}
	else
		SetCFCLastError(DIAG_ERROR_TXRX_FAIL);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCSimLockRelated::WriteSimData(BYTE *pSimDataBuf, int nSimDataBufLen, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	bool bExecuteResult = false;
	CRequestPtr ptRequest = new CWriteSimDataRequest(pSimDataBuf, nSimDataBufLen, bExecuteResult);
	if(Execute(ptRequest, nTimeout))
	{
		bRes = bExecuteResult;
		if(bExecuteResult == false)
			SetCFCLastError(DIAG_ERROR_SIMDATA_UNEXPECTED_RESPONSE);
	}
	else	
		SetCFCLastError(DIAG_ERROR_TXRX_FAIL);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CCFCSimLockRelated::GetSimDataVersion(std::string& strVersion, int nTimeout)
{
	return CSimLockStatus::CheckSIMLock(strVersion, nTimeout);
}

/************************************************************************/
/*  For SPU                                                                                   */
/************************************************************************/
void TransferBoolToChar(bool b, char* sz)
{
	CStringA strTemp = b ? "true" : "false";
	memcpy(sz, strTemp, strTemp.GetLength()+1);
}

CSIM_API bool IsSSCPLocked_ForSPU(char* szPort, char* szStatus/*return unlocked or locked*/, char* szTimeOut)
{
	CCFCSimLockRelated CSimLock(atoi(szPort));
	long nStatus = -1;
	bool bRes = CSimLock.IsSSCPLocked(nStatus, atoi(szTimeOut));
	if (bRes)
	{
		CStringA strTemp = nStatus ==  0 ? "unlocked" : "locked";
		memcpy(szStatus, strTemp, strTemp.GetLength()+1);
	}
	else
		memcpy(szStatus, "error", 6);

	return bRes;
}

CSIM_API bool IsNWSCPLocked_ForSPU(
								   char* szPort,
								   char* szNWStatus/*return unlocked or locked*/, 
								   char* szNSStatus/*return unlocked or locked*/, 
								   char* szSPStatus/*return unlocked or locked*/, 
								   char* szCPStatus/*return unlocked or locked*/, 
								   char* szSIMStatus/*return unlocked or locked*/, 
								   char* szTimeOut)
{
	CCFCSimLockRelated CSimLock(atoi(szPort));
	bool bNWUnlocked(false), bNSUnlocked(false), bSPUnlocked(false), bCPUnlocked(false), bSIMUnlocked(false);

	bool bRes = CSimLock.IsNWSCPLocked(bNWUnlocked, bNSUnlocked, bSPUnlocked, bCPUnlocked, bSIMUnlocked, atoi(szTimeOut));
	if (bRes)
	{
		CStringA strTemp = bNWUnlocked ? "unlocked" : "locked";
		memcpy(szNWStatus, strTemp, strTemp.GetLength()+1);
		strTemp = bNSUnlocked ? "unlocked" : "locked";
		memcpy(szNSStatus, strTemp, strTemp.GetLength()+1);
		strTemp = bSPUnlocked ? "unlocked" : "locked";
		memcpy(szSPStatus, strTemp, strTemp.GetLength()+1);
		strTemp = bCPUnlocked ? "unlocked" : "locked";
		memcpy(szCPStatus, strTemp, strTemp.GetLength()+1);
		strTemp = bSIMUnlocked ? "unlocked" : "locked";
		memcpy(szSIMStatus, strTemp, strTemp.GetLength()+1);
	}
	else
	{
		memcpy(szNWStatus, "error", 6);
		memcpy(szNSStatus, "error", 6);
		memcpy(szSPStatus, "error", 6);
		memcpy(szCPStatus, "error", 6);
		memcpy(szSIMStatus, "error", 6);
	}

	return bRes;
}

CSIM_API bool UnlockNW_ForSPU(char* szPort, char* szLockCode, char* szUnlockOK, char* szTimeOut)
{
	CCFCSimLockRelated CSimLock(atoi(szPort));
	bool bUnlockSuccess = false;
	bool bRes = CSimLock.UnlockNW(szLockCode, bUnlockSuccess, atoi(szTimeOut));
	if (bRes)
		TransferBoolToChar(bUnlockSuccess, szUnlockOK);
	else
		memcpy(szUnlockOK, "error", 6);

	return bRes;
}

CSIM_API bool UnlockNS_ForSPU(char* szPort, char* szLockCode, char* szUnlockOK, char* szTimeOut)
{
	CCFCSimLockRelated CSimLock(atoi(szPort));
	bool bUnlockSuccess = false;
	bool bRes = CSimLock.UnlockNS(szLockCode, bUnlockSuccess, atoi(szTimeOut));
	if (bRes)
		TransferBoolToChar(bUnlockSuccess, szUnlockOK);
	else
		memcpy(szUnlockOK, "error", 6);

	return bRes;
}

CSIM_API bool UnlockSP_ForSPU(char* szPort, char* szLockCode, char* szUnlockOK, char* szTimeOut)
{
	CCFCSimLockRelated CSimLock(atoi(szPort));
	bool bUnlockSuccess = false;
	bool bRes = CSimLock.UnlockSP(szLockCode, bUnlockSuccess, atoi(szTimeOut));
	if (bRes)
		TransferBoolToChar(bUnlockSuccess, szUnlockOK);
	else
		memcpy(szUnlockOK, "error", 6);

	return bRes;
}

CSIM_API bool UnlockCP_ForSPU(char* szPort, char* szLockCode, char* szUnlockOK, char* szTimeOut)
{
	CCFCSimLockRelated CSimLock(atoi(szPort));
	bool bUnlockSuccess = false;
	bool bRes = CSimLock.UnlockCP(szLockCode, bUnlockSuccess, atoi(szTimeOut));
	if (bRes)
		TransferBoolToChar(bUnlockSuccess, szUnlockOK);
	else
		memcpy(szUnlockOK, "error", 6);

	return bRes;
}

CSIM_API bool UnlockSIM_ForSPU(char* szPort, char* szLockCode, char* szUnlockOK, char* szTimeOut)
{
	CCFCSimLockRelated CSimLock(atoi(szPort));
	bool bUnlockSuccess = false;
	bool bRes = CSimLock.UnlockSIM(szLockCode, bUnlockSuccess, atoi(szTimeOut));
	if (bRes)
		TransferBoolToChar(bUnlockSuccess, szUnlockOK);
	else
		memcpy(szUnlockOK, "error", 6);

	return bRes;
}
