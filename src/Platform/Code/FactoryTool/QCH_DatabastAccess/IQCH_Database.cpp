#include "StdAfx.h"
#include "IQCH_Database.h"
#include "PicsCtrl.h"
#include "SSSO2.h"
#include <atlenc.h>

//PicsCtrl API
bool TransferCharToBool(char* szBool)
{
	CStringA strb = szBool;
	strb.MakeLower();
	bool bRes = (strb == "true") ||  (strb == "t");
	return bRes;
}

void TransferBoolToChar(bool b, char* sz)
{
	CStringA strTemp = b ? "true" : "false";
	memcpy(sz, strTemp, strTemp.GetLength()+1);
}

IQCH_DATABASE_EXPORTS_API bool EnablePICSLog(char* szbLogFlag,char* szLogFileName)
{
	bool bRes = false;
	CPicsCtrl picsCtrl;
	if(picsCtrl.Initial_PICSInterfaceDLL() == 0)
		bRes = 0 == picsCtrl.EnableLog(TransferCharToBool(szbLogFlag), szLogFileName);

	return bRes;
}

IQCH_DATABASE_EXPORTS_API bool CheckIMEI(char* szIMEI, char* szPart_Number)
{
	bool bRes = false;
	CPicsCtrl picsCtrl;
	if(picsCtrl.Initial_PICSInterfaceDLL() == 0)
		bRes = 0 == picsCtrl.CheckIMEI(szIMEI, szPart_Number);

	return bRes;
}

IQCH_DATABASE_EXPORTS_API bool CreateIMEIRecord(char* szIMEI)
{
	bool bRes = false;
	CPicsCtrl picsCtrl;
	if(picsCtrl.Initial_PICSInterfaceDLL() == 0)
		bRes = 0 == picsCtrl.CreateIMEIRecord(szIMEI);

	return bRes;
}

IQCH_DATABASE_EXPORTS_API bool QuerySPC(char* szIMEI,char* szSPC)
{
//for test
	int nEncodeDataLen = Base64EncodeGetRequiredLength(576);


	bool bRes = false;
	CPicsCtrl picsCtrl;
	if(picsCtrl.Initial_PICSInterfaceDLL() == 0)
		bRes = 0 == picsCtrl.QuerySPCFromPICS(szIMEI, szSPC);

	return bRes;
}

IQCH_DATABASE_EXPORTS_API bool GetSPC(char* szIMEI,char* szSPC)
{
	bool bRes = false;
	CPicsCtrl picsCtrl;
	if(picsCtrl.Initial_PICSInterfaceDLL() == 0)
		bRes = 0 == picsCtrl.GetSPCCodeFromPICS(szIMEI, szSPC);

	return bRes;
}

IQCH_DATABASE_EXPORTS_API bool QueryFSN(char* szIMEI, char* szFSN)
{
	bool bRes = false;
	CPicsCtrl picsCtrl;
	if(picsCtrl.Initial_PICSInterfaceDLL() == 0)
		bRes = 0 == picsCtrl.QueryFSNCode(szIMEI, (unsigned char*)szFSN);
	return bRes;
}

IQCH_DATABASE_EXPORTS_API bool QueryFSN_Base64(char* szIMEI, char* szFSN_Base64)
{
	bool bRes = false;
	CPicsCtrl picsCtrl;
	BYTE btFSN[33] = {0};
	if(picsCtrl.Initial_PICSInterfaceDLL() == 0)
		bRes = 0 == picsCtrl.QueryFSNCode(szIMEI, btFSN);
	if (bRes)
	{
		int nEncodeDataLen = Base64EncodeGetRequiredLength(33);
		if(nEncodeDataLen <= 1024)
		{
			memset(szFSN_Base64, 0, nEncodeDataLen);
			bRes = Base64Encode(btFSN, 33, szFSN_Base64, &nEncodeDataLen) == TRUE;
		}
		else
			bRes = false;
	}
	return bRes;
}

IQCH_DATABASE_EXPORTS_API bool GetFSN(char* szIMEI, char* szFSN)
{
	bool bRes = false;
	CPicsCtrl picsCtrl;
	memset(szFSN, 0, 33);
	if(picsCtrl.Initial_PICSInterfaceDLL() == 0)
		bRes = 0 == picsCtrl.GetFSNCode(szIMEI, (unsigned char*)szFSN);
	return bRes;
}

IQCH_DATABASE_EXPORTS_API bool GetFSN_Base64(char* szIMEI, char* szFSN_Base64)
{
	bool bRes = false;
	CPicsCtrl picsCtrl;
	BYTE btFSN[33] = {0};
	if(picsCtrl.Initial_PICSInterfaceDLL() == 0)
		bRes = 0 == picsCtrl.GetFSNCode(szIMEI, btFSN);
	if (bRes)
	{
		int nEncodeDataLen = Base64EncodeGetRequiredLength(33);
		if(nEncodeDataLen <= 1024)
		{
			memset(szFSN_Base64, 0, nEncodeDataLen);
			bRes = Base64Encode(btFSN, 33, szFSN_Base64, &nEncodeDataLen) == TRUE;
		}
		else
			bRes = false;
	}
	return bRes;
}

//Output SimData is in base64 format.
IQCH_DATABASE_EXPORTS_API bool GetSIMData(char* sznSIMType, char* szIMEI, char* szFSN, char* szSIMFileName, char* szSIM_Data_Base64, char* sznSIM_Data_Length)
{
	bool bRes = false;
	unsigned char SIM_Data[1153];
	memset(SIM_Data, 0, 1153);
	CPicsCtrl picsCtrl;
	short nSIMDataLen = 1153;
	if(picsCtrl.Initial_PICSInterfaceDLL() == 0)
		bRes = 0 == picsCtrl.GetSIMData(atoi(sznSIMType), szIMEI, (const char*)szFSN, szSIMFileName, SIM_Data, &nSIMDataLen);

	BYTE* SIM_Data_For_Phone = new BYTE[nSIMDataLen];
	if(bRes)
	{//SIM_Data is not binary codes, so decode SIM_Data here.
		size_t nSIMDataEncodeLen = strlen((char *)SIM_Data);
		_strupr_s((char *)SIM_Data, nSIMDataEncodeLen+1);
		
		int i = 0, j = 0;		
		for(i=0; i< nSIMDataLen; i++)
		{
			unsigned char FirstNibble='\0';
			unsigned char SecondNibble='\0';
			if(SIM_Data[j]>='A')
				FirstNibble=(unsigned char)((unsigned char)((unsigned char)SIM_Data[j]-'A')+10);
			else
				FirstNibble=(unsigned char)((unsigned char)SIM_Data[j]-'0');

			if(SIM_Data[j+1]>='A')
				SecondNibble=(unsigned char)((unsigned char)((unsigned char)SIM_Data[j+1]-'A')+10);
			else
				SecondNibble=(unsigned char)((unsigned char)SIM_Data[j+1]-'0');
			SIM_Data_For_Phone[i]=(unsigned char)(FirstNibble<<4)+(unsigned char)SecondNibble;
			j+=2;
		}
	}

	if(bRes)
	{//Encode SIM_Data to Base64 here.
		int nEncodeDataLen = Base64EncodeGetRequiredLength(nSIMDataLen);
		bRes = Base64Encode(SIM_Data_For_Phone, nSIMDataLen, szSIM_Data_Base64, &nEncodeDataLen) == TRUE;
		if(bRes)
		{
			CStringA strSimDataLen;
			strSimDataLen.Format("%d", nEncodeDataLen);
			memcpy(sznSIM_Data_Length, strSimDataLen, strSimDataLen.GetLength()+1);
		}
	}
	return bRes;
}

//IQCH_DATABASE_EXPORTS_API bool GetSIMData(char* sznSIMType, char* szIMEI, char* szFSN, char* szSIMFileName, char* szSIM_Data, char* sznSIM_Data_Length)
//{
//	bool bRes = false;
//	CPicsCtrl picsCtrl;
//	short nSIM_DataLen = 0;
//	if(picsCtrl.Initial_PICSInterfaceDLL() == 0)
//		bRes = 0 == picsCtrl.GetSIMData(atoi(sznSIMType), szIMEI, (const char*)szFSN, szSIMFileName, (unsigned char*)szSIM_Data, &nSIM_DataLen);
//	return bRes;
//}

IQCH_DATABASE_EXPORTS_API bool QueryUnlockUnblockCode(char* szIMEI,
													  char* szNetwork_Unlock_Code,
													  char* szService_Provider_Unlock_Code,
													  char* szCoporate_Unlock_Code,
													  char* szNetwork_Subset_Unlock_Code,
													  char* zsUser_SIM_Unlock_Code,
													  char* szUser_USIM_Unlock_Code,
													  char* szSerial_Code,
													  char* szSPC,
													  char* szPhone_Lock_Code)
{
	bool bRes = false;
	CPicsCtrl picsCtrl;
	short nSIM_DataLen = 0;
	if(picsCtrl.Initial_PICSInterfaceDLL() == 0)
		bRes = 0 == picsCtrl.QueryUnlockUnblockCode(szIMEI, szNetwork_Unlock_Code, szService_Provider_Unlock_Code, szCoporate_Unlock_Code,
		szNetwork_Subset_Unlock_Code, zsUser_SIM_Unlock_Code, szUser_USIM_Unlock_Code, szSerial_Code, szSPC, szPhone_Lock_Code);
	return bRes;
}

IQCH_DATABASE_EXPORTS_API bool QueryUnlockUnblockCode_BT(char* szIMEI,
													  char* szNetwork_Unlock_Code,
													  char* szService_Provider_Unlock_Code,
													  char* szCoporate_Unlock_Code,
													  char* szNetwork_Subset_Unlock_Code,
													  char* zsUser_SIM_Unlock_Code,
													  char* szUser_USIM_Unlock_Code,
													  char* szSerial_Code,
													  char* szSPC,
													  char* szPhone_Lock_Code)
{
	bool bRes = false;
	CPicsCtrl picsCtrl;
	short nSIM_DataLen = 0;
	if(picsCtrl.Initial_PICSInterfaceDLL() == 0)
		bRes = 0 == picsCtrl.QueryUnlockUnblockCode_BT(szIMEI, szNetwork_Unlock_Code, szService_Provider_Unlock_Code, szCoporate_Unlock_Code,
		szNetwork_Subset_Unlock_Code, zsUser_SIM_Unlock_Code, szUser_USIM_Unlock_Code, szSerial_Code, szSPC, szPhone_Lock_Code);
	return bRes;
}

//SSSO2 API:

IQCH_DATABASE_EXPORTS_API bool SSSO2_CheckNAL_PN(char* Part_Number,char* MAL_FROM_SCANNER,char* IMEI_FROM_SCANNER,char *ErrCode, char *ErrMsg)
{
	bool bRes = false;
	CSSSO2 Cssso2;
	if(Cssso2.Initial_SSSO2Dll() == 0)
		bRes = 0 != Cssso2.SSSO2_CheckNAL_PN( Part_Number, MAL_FROM_SCANNER, IMEI_FROM_SCANNER, ErrCode, ErrMsg);
	return bRes;
}

IQCH_DATABASE_EXPORTS_API bool SSSO2_UpdateIMEIStatus(char* szIMEI,char* Software_Version,char* Hardware_Version,char* MAL_FROM_SCANNER,char *ErrCode, char *ErrMsg)
{
	bool bRes = false;
	CSSSO2 Cssso2;
	if(Cssso2.Initial_SSSO2Dll() == 0)
		bRes = 0 != Cssso2.SSSO2_UpdateIMEIStatus( szIMEI, Software_Version, Hardware_Version, MAL_FROM_SCANNER, ErrCode, ErrMsg);
	return bRes;
}

IQCH_DATABASE_EXPORTS_API bool SSSO2_QueryIMEI(char *szIMEI,char* szMSN,char* szBTAddr,char* szSW_ver,char* szHW_ver,char *StationtState, char *ErrCode, char *ErrMsgchar,char *Status_Output)
{
	bool bRes = false;
	CSSSO2 Cssso2;
	if(Cssso2.Initial_SSSO2Dll() == 0)
		bRes = 0 != Cssso2.SSSO2_QueryIMEI( szIMEI, szMSN, szBTAddr, szSW_ver, szHW_ver, StationtState, ErrCode, ErrMsgchar, Status_Output);
	return bRes;
}

IQCH_DATABASE_EXPORTS_API bool SSSO2_GetProcessResult(char* szPicasso,char* szModel,char *ErrCode, char *ErrMsg)
{
	bool bRes = false;
	CSSSO2 Cssso2;
	if(Cssso2.Initial_SSSO2Dll() == 0)
		bRes = 0 != Cssso2.SSSO2_GetProcessResult( szPicasso, szModel, ErrCode, ErrMsg);
	return bRes;
}

//Just for test.
IQCH_DATABASE_EXPORTS_API bool TestSPUParameter(char* sz01, char* sz02, char* sz03, char* sz04, char* sz05, char* sz06, char* sz07, char* sz08, char* sz09, char* sz10, char* sz11, char* sz12)
{
	char* szTest = "Test01";
	memcpy(sz01, szTest, 7);
	szTest = "Test02";
	memcpy(sz02, szTest, 7);
	szTest = "Test03";
	memcpy(sz03, szTest, 7);
	szTest = "Test04";
	memcpy(sz04, szTest, 7);
	szTest = "Test05";
	memcpy(sz05, szTest, 7);
	szTest = "Test06";
	memcpy(sz06, szTest, 7);
	szTest = "Test07";
	memcpy(sz07, szTest, 7);
	szTest = "Test08";
	memcpy(sz08, szTest, 7);
	szTest = "Test09";
	memcpy(sz09, szTest, 7);
	szTest = "Test10";
	memcpy(sz10, szTest, 7);
	szTest = "Test11";
	memcpy(sz11, szTest, 7);
	szTest = "Test12";
	memcpy(sz12, szTest, 7);

	return true;
}
