#include "stdafx.h"
#include "PhoneOperation.h"
#include "SecureActivate.h"
#include "SimLockStore.h"
#include "nvitem.h"
#include "Tool.h"
#include <string>
#include "..\STE\MiddleWare\CallbackTypes.h"
#include "QisdaMiscellaneousModule.h"

static char *MsgBoxTitle = "SecurityTool";

namespace PHONEOP{

using namespace std;

#define DIAG_NV_ITEM_DATA_SIZE		128	

// nvitems id
#define NV_SEC_CODE_I				85
#define NV_FTM_MODE_I				453
#define NV_UE_IMEI_I				550
#define NV_DEVICE_SERIAL_NO_I		2824

#define CI_IMEI_DIGIT_SIZE			15
#define CI_FSN_DIGIT_SIZE			16

int ChangePhoneMode(long nComPort, long nMode);
BOOL GetNvUeImei(BYTE *pucNvData, int iNvDataSize, char *sImei, int &iImeiSize);
BOOL PutNvUeImei(char *sImei, int iImeiSize, BYTE **ppucNvData, int &iNvDataSize);

BOOL PutNvUeImei(char *sImei, int iImeiSize, BYTE **ppucNvData, int &iNvDataSize)
{
	// 1: first byte is reserved for data size; 2: BCD data multiple 2 is digit size; 1: dummy digit ('A').
	ASSERT((sImei != NULL) && (iImeiSize == (NV_UE_IMEI_SIZE-1)*2-1));
	ASSERT((ppucNvData != NULL) && (*ppucNvData == NULL));

	// do nothing if parameters are not correct.
	if ((sImei == NULL) || (iImeiSize != (NV_UE_IMEI_SIZE-1)*2-1)) return FALSE;
	if ((ppucNvData == NULL) || (*ppucNvData != NULL)) return FALSE;

	// prepare the NV data.
	nv_item_type tNvItems;
	tNvItems.ue_imei.ue_imei[0] = NV_UE_IMEI_SIZE-1;			// following data size.
	CTool::DigitToHexBcdB(sImei, iImeiSize, (BYTE*)&tNvItems.ue_imei.ue_imei[1], NV_UE_IMEI_SIZE-1, 'a');	

	// set the output parameters.
	iNvDataSize = sizeof(tNvItems.ue_imei);
	*ppucNvData = new BYTE[iNvDataSize];

	memcpy(*ppucNvData, &tNvItems, sizeof(tNvItems.ue_imei));

	return TRUE;
}

BOOL GetNvUeImei(BYTE *pucNvData, int iNvDataSize, char *sImei, int &iImeiSize)
{
	ASSERT((pucNvData != NULL) && (iNvDataSize >= sizeof(nv_ue_imei_type)));
	ASSERT((sImei != NULL) && (iImeiSize >= (NV_UE_IMEI_SIZE-1)*2-1));

	// do nothing if parameters are not correct.
	if ((pucNvData == NULL) || (iNvDataSize < sizeof(nv_ue_imei_type))) return FALSE;
	if ((sImei == NULL) || (iImeiSize < (NV_UE_IMEI_SIZE-1)*2-1)) return FALSE;

	// parse the NV data.
	char sDigitBuf[(NV_UE_IMEI_SIZE-1)*2];
	nv_item_type* ptNvItems = (nv_item_type*)pucNvData;
	CTool::HexBcdToDigit(&ptNvItems->ue_imei.ue_imei[1], NV_UE_IMEI_SIZE-1, sDigitBuf, (NV_UE_IMEI_SIZE-1)*2);
	memcpy(sImei, &sDigitBuf[1], (NV_UE_IMEI_SIZE-1)*2-1);

	// check the NV data content.
	if (ptNvItems->ue_imei.ue_imei[0] != NV_UE_IMEI_SIZE-1) return FALSE;

	// set the output parameters.
	iImeiSize = (NV_UE_IMEI_SIZE-1)*2-1;

	return TRUE;
}

bool FreezeOTP(long nComPort)
{
	CSecureActivate freezeOTP(nComPort);

	BYTE appID[17] = {0};
	DWORD status;
	
	if (freezeOTP.SecureActivate(1, 16, appID, &status))
	{
		if (status == 0)
			return true;
		else 
			return false;
	}

	return false;
}

bool GetIMEI(long nComPort, BYTE *imei, const size_t size)
{
	ASSERT(imei != NULL);

	CNVItem nvitem(nComPort);
	DIAG_NVITEM_PACKET_F_type resp;
	
	if (!(nvitem.ReadNVItem(NV_UE_IMEI_I, resp)) || resp.nv_stat != NV_DONE_S)
		return false;

	char sDigitIMEI[260] = {0};
	int szImei = 260;
	GetNvUeImei((BYTE*)resp.item_data, 128, sDigitIMEI, szImei);

	memset(imei, 0, size);

	if (size < CI_IMEI_DIGIT_SIZE)
		memcpy(imei, sDigitIMEI, size);
	else
		memcpy(imei, sDigitIMEI, CI_IMEI_DIGIT_SIZE);

	return true;
}

bool SetIMEI(long nComPort, const BYTE *imei)
{
	BYTE* pNvData = NULL;
	int iNvDataSize = 0;

	if (!PutNvUeImei((char*)imei, strlen((char*)imei), &pNvData, iNvDataSize))
		return false;

	CNVItem nvitem(nComPort);
	DIAG_NVITEM_PACKET_F_type resp;

	if (nvitem.WriteNVItem(NV_UE_IMEI_I, resp, pNvData, iNvDataSize))
	{
		if (resp.nv_stat == NV_DONE_S)
			return true;
		else
			return false;
	}

	return false;
}

bool GetFSN(long nComPort, BYTE *fsn, const size_t size, const BYTE *SPC/*=NULL*/)
{	
	ASSERT(fsn != NULL);

	CNVItem nvitem(nComPort);

	// Query SPC
	char SPCCode[7] = {0};
	if (SPC != NULL)
		memcpy(SPCCode, SPC, 6);

	bool bUnlockOK;

	if (nvitem.UnlockSPC((char*)SPC, bUnlockOK) && bUnlockOK)
		;
	else 
	{
		string strOutput;
		OutputPrompt(MsgBoxTitle, "Query SPC failed", "OK", strOutput);
		return false;
	}

	Sleep(100);

	// Read FSN	
	DIAG_NVITEM_PACKET_F_type resp;

	if (!nvitem.ReadNVItem(NV_DEVICE_SERIAL_NO_I, resp)  || resp.nv_stat != NV_DONE_S)
		return false;

	memset(fsn, 0, size);
	if (size < CI_FSN_DIGIT_SIZE)
		memcpy(fsn, resp.item_data, size);
	else 
		memcpy(fsn, resp.item_data, CI_FSN_DIGIT_SIZE);

	return true;
}

bool SetFSN(long nComPort, const BYTE *fsn, const BYTE *SPC/*=NULL*/)
{
	ASSERT(fsn != NULL);

	// Query SPC
	char SPCCode[7] = {'0', '0', '0', '0', '0', '0'};
	if (SPC != NULL)
		memcpy(SPCCode, SPC, 6);

	if (!QuerySPC(nComPort, (BYTE*)SPCCode))
		return false;

	// Set FSN
	CNVItem nvitem(nComPort);
	DIAG_NVITEM_PACKET_F_type resp;

	if (nvitem.WriteNVItem(NV_DEVICE_SERIAL_NO_I, resp, fsn, CI_FSN_DIGIT_SIZE))
	{
		if (resp.nv_stat == NV_DONE_S)
			return true;
		else
			return false;
	}

	return false;
}

bool GetBTAddress(long nComPort, BYTE *addr, const size_t size)
{
	ASSERT(addr != NULL);

	CNVItem nvitem(nComPort);
	DIAG_NVITEM_PACKET_F_type resp;

	if (!nvitem.ReadNVItem(NV_BD_ADDR_I, resp))
		return false;

	// status = 5, no active
	if (resp.nv_stat != 0)	
		return false;

	char BTAddr[20] = {0};
	CTool::HexBcdToDigit((uint8*)(resp.item_data), 6, (char*)BTAddr, 6*2+1);
	_strrev(BTAddr);

	memset(addr, 0, sizeof(BYTE)*size);
	if (size < 12)
		memcpy((void*)addr, (void*)BTAddr, size);
	else
		memcpy((void*)addr, (void*)BTAddr, 12);

	return true;
}

int GenSimFile(int iSimTypeID, char *szImei, char *szFsn, char *szMasterCode, char *szSubsidyCode,
				char *szLockPath, BYTE **simEncryptedData, int *simEncryptedDataSize)
{
	ASSERT (szImei != NULL && szFsn != NULL && szMasterCode != NULL && 
		szSubsidyCode != NULL && szLockPath != NULL && simEncryptedData != NULL &&
		simEncryptedDataSize != NULL);

	void *hSimEncHandle = NULL;
	int nResult;

	nResult = ISimEnc_AllocSimEncObj(iSimTypeID, hSimEncHandle);
	if (nResult != SIM_RET_SUCCESS)
		return nResult;

	nResult = ISimEnc_SetImei(hSimEncHandle, szImei, strlen((char*)szImei));
	if (nResult != SIM_RET_SUCCESS)
		return SIM_RET_NO_IMEI;

	nResult = ISimEnc_SetFlashSerialNumber(hSimEncHandle, szFsn, strlen(szFsn));
	if (nResult != SIM_RET_SUCCESS)
		return SIM_RET_NO_FSN;

	nResult = ISimEnc_SetSimLockFile(hSimEncHandle, szLockPath);
	if (nResult != SIM_RET_SUCCESS)
	{
		string strOutput;
		OutputPrompt(MsgBoxTitle, "Load the SIM Lock file failed", "OK", strOutput);
		return nResult;
	}

	T_SIM_UNLOCK_CODE tSimUnlockCode;
	tSimUnlockCode.szNetworkCode			= szSubsidyCode;
	tSimUnlockCode.szServiceProviderCode	= szSubsidyCode;
	tSimUnlockCode.szCorporateCode			= szSubsidyCode;
	tSimUnlockCode.szNetworkSubsetCode		= szSubsidyCode;
	tSimUnlockCode.szSimCode				= szSubsidyCode;
	tSimUnlockCode.szUsimCode				= szMasterCode;
	tSimUnlockCode.szReserved				= NULL;

	nResult = ISimEnc_SetUnlockCodes(hSimEncHandle, &tSimUnlockCode);
	if (nResult != SIM_RET_SUCCESS)
		return nResult;

	int nSimLockData = 1024;
	BYTE pcSimLockData[1024] = {0};
	nResult = ISimEnc_GenSimLockData(hSimEncHandle, pcSimLockData, &nSimLockData);
	if (nResult != SIM_RET_SUCCESS)
		return nResult;

	*simEncryptedDataSize = nSimLockData;
	if (*simEncryptedData != NULL)
		delete []simEncryptedData[0];
	
	*simEncryptedData = new BYTE[*simEncryptedDataSize];
	if (*simEncryptedData == NULL)
		return 1;

	memcpy((void*)*simEncryptedData, (void*)pcSimLockData, sizeof(BYTE)*nSimLockData);

	nResult = ISimEnc_FreeSimEncObj(hSimEncHandle);
	if (nResult != SIM_RET_SUCCESS)
		return nResult;
	
	return 0;
}

bool WriteSimData(long nComPort, BYTE *simData, const size_t simDataSize, DWORD *cmdStatus, DWORD *status)
{
	ASSERT(simData != NULL);

	BYTE ucSimAppLen = 16;
	BYTE ucSimApp[16] = {0};	

	CSimLockStore simStore(nComPort);

	if (simStore.StoreSimData(1, ucSimAppLen, ucSimApp, simDataSize, simData, cmdStatus, status))
		return true;

	return false;
}

bool WriteSimLockFileName(long nComPort, char *pFileName)
{
	CNVItem nvitem(nComPort);
	return nvitem.WriteSIMLockFileNameToNV(pFileName);
}

bool CheckSimDataIsProgrammed(long nComPort)
{
	// Note: when successfully querying lock status, it means that the sim data
	//       is already programmed into the mobile phone.
	CSimLockStatus lockStatus(nComPort);

	return lockStatus.QueryLockStatus();
}

bool QuerySPC(long nComPort, const BYTE *SPC)
{
	ASSERT(SPC != NULL);

	CNVItem nvitem(nComPort);
	bool bUnlockOK;

	if (nvitem.UnlockSPC((char*)SPC, bUnlockOK) && bUnlockOK)
		return true;
	else 
		return false;
}

bool SetSPC(long nComPort, const BYTE *SPC)
{
	ASSERT(SPC != NULL);

	// Set SPC
	CNVItem nvitem(nComPort);
	DIAG_NVITEM_PACKET_F_type resp;

	return nvitem.WriteNVItem(NV_SEC_CODE_I, resp, SPC, 6);
}

bool QueryLockStatus(long nComPort, bool *nwStatus, bool *spStatus, bool *nsStatus, 
					 bool *cpStatus, bool *simStatus, bool *masterStatus)
{
	CSimLockStatus lockStatus(nComPort);

	return lockStatus.QueryLockStatus(nwStatus, spStatus, nsStatus, cpStatus, simStatus, masterStatus);
}

bool UnLockSimData(long nComPort, int unlockType, char *szCode, DWORD *status)
{
	CSimUnLockStatus unlockSim(nComPort);
	return unlockSim.UnlockStatus(unlockType, szCode, status);
}

bool UnLockPhoneMasterCode(long nComPort, char *szCode, DWORD *status)
{
	CSimUnLockStatus unlockSim(nComPort);
	return unlockSim.UnlockMasterCode(szCode, status);
}

void RestartPhone(long nComPort)
{
	CControlModeSet phone(nComPort);
	phone.Restart();
}

bool EnableQualcommEventLog(long nComPort, bool bEnable)
{
	CNVItem nvitem(nComPort);
	return nvitem.EnableDiagEvent(bEnable);
}

bool ChangeToOnlineMode(long nComPort)
{
	CControlModeSet modeSet(nComPort);
	return modeSet.DiagControlMode(MODE_ONLINE_F);
}

bool ChangeToFTMMode(long nComPort)
{
	CControlModeSet modeSet(nComPort);
	return modeSet.DiagControlMode(MODE_FTM_F);
}

MOBILEMODEL GetCurrentMobileModel(long nComPort)
{
	CQisdaMiscellaneousModule Miscellaneous(nComPort);
	CStringA strSW, strHW;
	bool bSuccess = Miscellaneous.GetSWHWVersion(strSW.GetBuffer(100), strHW.GetBuffer(100));
	strSW.ReleaseBuffer();
	strHW.ReleaseBuffer();

	if (!bSuccess)
		return MOBILEMODEL_LAST;
	
	if (strHW.CompareNoCase(_T("Athens")) == 0)
		return MOBILEMODEL_WELLFLEET;
	else if (strHW.CompareNoCase(_T("Lavernock")) == 0)
		return MOBILEMODEL_LAVERNOCK;
	else if (strHW.CompareNoCase(_T("Melos")) == 0)
		return MOBILEMODEL_MELOS;
	else
		return MOBILEMODEL_LAST;
}

}	// namespace PHONEOP