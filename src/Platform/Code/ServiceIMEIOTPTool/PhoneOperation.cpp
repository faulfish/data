#include "stdafx.h"
#include "PhoneOperation.h"
#include "SecureActivate.h"
#include "nvitem.h"
#include "MobileToolKit.h"
#include "Tool.h"
#include "CallbackTypes.h"
#include <string>

namespace PHONEOP{

using namespace std;

static char *MsgBoxTitle = "IMEIOTP";

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

BOOL FreezeOTP(const UINT nComPort)
{
	CSecureActivate freezeOTP(nComPort);

	BYTE appID[17] = {0};
	DWORD status;
	
	if (freezeOTP.SecureActivate(1, 16, appID, &status)){
		if (status == 0)
			return TRUE;
		else 
			return FALSE;
	}

	return FALSE;
}

BOOL GetIMEI(const UINT nComPort, BYTE *imei, const size_t size)
{
	ASSERT(imei != NULL);

	CNVItem nvitem(nComPort);
	DIAG_NVITEM_PACKET_F_type resp;
	
	if (!(nvitem.ReadNVItem(NV_UE_IMEI_I, resp)) || resp.nv_stat != NV_DONE_S)
		return FALSE;

	char sDigitIMEI[260] = {0};
	int szImei = 260;
	GetNvUeImei((BYTE*)resp.item_data, 128, sDigitIMEI, szImei);

	memset(imei, 0, size);

	if (size < CI_IMEI_DIGIT_SIZE)
		memcpy(imei, sDigitIMEI, size);
	else
		memcpy(imei, sDigitIMEI, CI_IMEI_DIGIT_SIZE);

	return TRUE;
}

BOOL SetIMEI(const UINT nComPort, const BYTE *imei)
{
	BYTE* pNvData = NULL;
	int iNvDataSize = 0;

	if (!PutNvUeImei((char*)imei, strlen((char*)imei), &pNvData, iNvDataSize))
		return FALSE;

	CNVItem nvitem(nComPort);
	DIAG_NVITEM_PACKET_F_type resp;

	if (nvitem.WriteNVItem(NV_UE_IMEI_I, resp, pNvData, iNvDataSize) && (resp.nv_stat == NV_DONE_S))
		return TRUE;

	return FALSE;
}

BOOL GetFSN(const UINT nComPort, BYTE *fsn, const size_t size, const BYTE *SPC/*=NULL*/)
{	
	ASSERT(fsn != NULL);

	CNVItem nvitem(nComPort);

	// Query SPC
	char SPCCode[7] = {0};
	if (SPC != NULL)
		memcpy(SPCCode, SPC, 6);

	bool bUnlockOK;

	if (!nvitem.UnlockSPC((char*)SPC, bUnlockOK) && bUnlockOK){
		string strOutput;
		OutputPrompt(MsgBoxTitle, "Query SPC failed", "OK", strOutput);
		return FALSE;
	}

	// Read FSN	
	DIAG_NVITEM_PACKET_F_type resp;

	if (!nvitem.ReadNVItem(NV_DEVICE_SERIAL_NO_I, resp)  || resp.nv_stat != NV_DONE_S)
		return false;

	memset(fsn, 0, size);
	if (size < CI_FSN_DIGIT_SIZE)
		memcpy(fsn, resp.item_data, size);
	else 
		memcpy(fsn, resp.item_data, CI_FSN_DIGIT_SIZE);

	return TRUE;
}

BOOL SetFSN(const UINT nComPort, const BYTE *fsn, const BYTE *SPC/*=NULL*/)
{
	ASSERT(fsn != NULL);

	// Query SPC
	char SPCCode[7] = {'0', '0', '0', '0', '0', '0'};
	if (SPC != NULL)
		memcpy(SPCCode, SPC, 6);

	if (!QuerySPC(nComPort, (BYTE*)SPCCode))
		return FALSE;

	// Set FSN
	CNVItem nvitem(nComPort);
	DIAG_NVITEM_PACKET_F_type resp;

	if (nvitem.WriteNVItem(NV_DEVICE_SERIAL_NO_I, resp, fsn, CI_FSN_DIGIT_SIZE) && (resp.nv_stat == NV_DONE_S))
		return TRUE;

	return FALSE;
}

BOOL SetBTAddress(const UINT nComPort, BYTE *addr)
{
	ASSERT(addr != NULL);

	CNVItem nvitem(nComPort);
	DIAG_NVITEM_PACKET_F_type resp;
	BYTE strAddr[10] = {0};
	char byteHex[10] = {'0', 'x'};
	for (int i = 5; i >=0; i--){		
		byteHex[2] = addr[2*i];
		byteHex[3] = addr[2*i+1];

		sscanf(byteHex, "%x", &strAddr[5-i]);
	}

	if (nvitem.WriteNVItem(NV_BD_ADDR_I, resp, strAddr, 6) && (resp.nv_stat == 0))
		return TRUE;
	return FALSE;
}

BOOL GetBTAddress(const UINT nComPort, BYTE *addr, const size_t size)
{
	ASSERT(addr != NULL);

	CNVItem nvitem(nComPort);
	DIAG_NVITEM_PACKET_F_type resp;

	if (!nvitem.ReadNVItem(NV_BD_ADDR_I, resp))
		return FALSE;

	// status = 5, no active
	if (resp.nv_stat != 0)	
		return FALSE;

	char BTAddr[20] = {0};
	CTool::HexBcdToDigit((uint8*)(resp.item_data), 6, (char*)BTAddr, 6*2+1);
	_strrev(BTAddr);

	memset(addr, 0, sizeof(BYTE)*size);
	if (size < 12)
		memcpy((void*)addr, (void*)BTAddr, size);
	else
		memcpy((void*)addr, (void*)BTAddr, 12);

	return TRUE;
}

BOOL QuerySPC(const UINT nComPort, const BYTE *SPC)
{
	ASSERT(SPC != NULL);

	CNVItem nvitem(nComPort);
	bool bUnlockOK;

	if (nvitem.UnlockSPC((char*)SPC, bUnlockOK) && bUnlockOK)
		return TRUE;

	return FALSE;
}

BOOL SetSPC(const UINT nComPort, const BYTE *SPC)
{
	ASSERT(SPC != NULL);

	// Set SPC
	CNVItem nvitem(nComPort);
	DIAG_NVITEM_PACKET_F_type resp;

	return nvitem.WriteNVItem(NV_SEC_CODE_I, resp, SPC, 6);
}

BOOL RestartPhone(const UINT nComPort, const UINT nSleepTime)
{
	CControlModeSet phone(nComPort);
	return (BOOL)phone.Restart(nSleepTime);
}

BOOL ChangeToOnlineMode(const UINT nComPort)
{
	CControlModeSet modeSet(nComPort);
	if (!modeSet.DiagControlMode(4))
		return FALSE;

	CNVItem nvitem(nComPort);
	DIAG_NVITEM_PACKET_F_type resp;
	BYTE isFTM[2] = {0x0, 0x0};

	if (!nvitem.WriteNVItem(NV_FTM_MODE_I, resp, isFTM, 1))
		return FALSE;

	return TRUE;
}

BOOL ChangeToFTMMode(const UINT nComPort)
{
	CControlModeSet modeSet(nComPort);
	if (!modeSet.DiagControlMode(1))
		return FALSE;

	CNVItem nvitem(nComPort);
	DIAG_NVITEM_PACKET_F_type resp;
	BYTE isFTM[2] = {0x1, 0x0};

	if (!nvitem.WriteNVItem(NV_FTM_MODE_I, resp, isFTM, 1))
		return FALSE;
	
	return TRUE;
}

BOOL BackNV(const UINT nComPort)
{
	CNVItem nvitem(nComPort);
	return (BOOL)nvitem.BackupNVPartition(10000);
}

BOOL RestoreBRT(const UINT nComPort, LPCTSTR pBRTFilePath)
{
    char szSW[512] = {0}, szHW[512] = {0};
    BOOL bRestored = FALSE;
    ::GetSWHWVersion(nComPort, szSW, szHW);
    if(memcmp("Athens", szHW, 6) == 0) {
        bRestored = (BOOL)RestoreBRT2NV(nComPort, pBRTFilePath);
        bRestored = TRUE;
    } else if(memcmp("Lavernock", szHW, 9) == 0) {
        bRestored = TRUE;
    }
	ASSERT (pBRTFilePath != NULL);
	return bRestored;
}

BOOL CheckHWVersion(long nPort, unsigned long reg[5]) {
    char szSW[512] = {0}, szHW[512] = {0};
    ::GetSWHWVersion(nPort, szSW, szHW);
    if(memcmp("Athens", szHW, 6) == 0) {
        reg[0] = 0x301007;
        reg[1] = 0x40000E00;
    } else if(memcmp("Lavernock", szHW, 9) == 0) {
        reg[0] = 0x301016;
        reg[1] = 0x40000600;
    }
    return TRUE;
}

BOOL WriteQFuse(long nPort, long nChaig, unsigned long reg[5]) {
    return (BOOL)::WriteQFuse(nPort, nChaig, reg);;
}

BOOL CheckQFuse(long nPort, unsigned long reg[5]) {
    unsigned long reg1 = 0, reg2 = 0;
    bool bSuccess = false;
    ::ReadQFuseState(nPort, reg1, reg2, bSuccess);
    if(reg1 != reg[0] || reg2 != reg[1]) {
        return FALSE;
    }
    return TRUE;
}


}	// namespace PHONEOP