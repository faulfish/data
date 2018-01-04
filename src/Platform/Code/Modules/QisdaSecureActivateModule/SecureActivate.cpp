#include "StdAfx.h"
#include "SecureActivate.h"
#include "..\..\RequestManager\Request\SimLockStoreRequest.h"
#include "..\..\RequestManager\Request\ODMRequest.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"

CSecureActivate::CSecureActivate(int nCOMPort):CInterfaceBase(nCOMPort)
{
}

CSecureActivate::~CSecureActivate(void)
{
}

BOOL CSecureActivate::SecureActivate(BYTE iSlotId, BYTE iSIM_AppIdLen, BYTE *piSIM_AppId, DWORD *pStatus, int nTimeout)
{
	using namespace PLATFORM_TOOL;

	GSDIDIAG_SECURE_DEVICE_ACTIVATE_REQ cmdReq = {0};

	cmdReq.aps_as_transport = 1;
	cmdReq.subcommand = 86;
	cmdReq.slot_id = iSlotId;
	cmdReq.sim_app_in_len = iSIM_AppIdLen;
	memcpy(cmdReq.sim_app_id, piSIM_AppId, iSIM_AppIdLen);

	CRequestPtr ptRequest = new CSecurityActivateRequest(cmdReq, *pStatus);
	return Execute(ptRequest, nTimeout);
}

BOOL CSecureActivate::IsOTPFrozen(int& nLastError, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bIsOTP = false;

	CRequestPtr ptRequest = new CQueryFreezeOTPRequest(bIsOTP, nLastError);	
	if(Execute(ptRequest, nTimeout))
		return bIsOTP;

	return FALSE;
}

CCFCOTPRelated::CCFCOTPRelated(int nCOMPort):CSecureActivate(nCOMPort), CLastError(nCOMPort)
{}
CCFCOTPRelated::~CCFCOTPRelated(){}
//************************************
// Method:    FreezeOTP
// Returns:   bool
//			  return true only when OTP has been frozen successfully
// Parameter: IN int nTimeout
//************************************
bool CCFCOTPRelated::FreezeOTP(int nTimeout)
{
	bool bRes = false;
	BYTE iSlotId = 1;
	BYTE iSIM_AppIdLen = 16; 
	BYTE iSIM_AppId[16] = {0}; 
	DWORD nStatus = 0;

	bRes = CSecureActivate::SecureActivate(iSlotId, iSIM_AppIdLen, iSIM_AppId, &nStatus, nTimeout) == TRUE;
	if (bRes)
	{
		if(nStatus != 0)
		{
			bRes = false;
			SetCFCLastError(DIAG_ERROR_UNEXPECTED_RESPONSE);
		}
	}
	else
		SetCFCLastError(DIAG_ERROR_TXRX_FAIL);
	return bRes;
}
//************************************
// Method:    IsOTPFrozen
// Returns:   bool
//            return false when OTP isn't frozen or command fail.
//			  return true only when resp.otp_status==2 (command success and otp is frozen)
//			  (add error coad later):
// CQueryFreezeOTPRequest return nErrorCode meaning:
//			  nErrorCode = DIAG_ERROR_WRONG_OTP_PROTECTED_CODE; // resp.otp_status == 0
//			  nErrorCode = DIAG_ERROR_EMPTY_OTP;                // resp.otp_status == 1
//			  nErrorCode = DIAG_ERROR_DAMAGED_OTP;			    // resp.otp_status == 3
// Parameter:IN int nTimeout
//************************************
bool CCFCOTPRelated::IsOTPFrozen(int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	bool bFrozen = false;

	int nErrorCode = 0;
	CRequestPtr ptRequest = new CQueryFreezeOTPRequest(bFrozen, nErrorCode);	
	bRes = Execute(ptRequest, nTimeout);
	if(bRes)
	{
		bRes = bFrozen;
		if(!bRes)
			SetCFCLastError(nErrorCode==0 ? DIAG_ERROR_TXRX_FAIL : nErrorCode);
	}
	else
		SetCFCLastError(DIAG_ERROR_TXRX_FAIL);

	return bRes && bFrozen;
}