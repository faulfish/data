#pragma once

#ifdef IQCH_DATABASE_EXPORTS
#define IQCH_DATABASE_EXPORTS_API extern "C" __declspec(dllexport)
#else
#define IQCH_DATABASE_EXPORTS_API extern "C" __declspec(dllimport)
#endif

//PicsCtrl API:

typedef bool (*pFnEnablePICSLog)(char* szbLogFlag, char* szLogFileName);
IQCH_DATABASE_EXPORTS_API bool EnablePICSLog(char* szbLogFlag, char* szLogFileName);

/****************************************************************************************************
1. Check the IMEI format before starting any functions.
2. Check the IMEI validation:
- check if it exists in specific IMEI range (TACFAC and SSS booking range) for the part number.
- check the SP (Spare, the 15th digit of IMEI) is valid.
*****************************************************************************************************/
typedef bool (*pFnCheckIMEI)(char* szIMEI, char* szPart_Number);
IQCH_DATABASE_EXPORTS_API bool CheckIMEI(char* szIMEI, char* szPart_Number);

/*************************************************************
1. Create an IMEI record.
2. Factory Tool will use CheckImei() to check if the IMEI is valid or not, so WriteImei() needn't to check again.
*************************************************************/
typedef bool (*pFnCreateIMEIRecord)(char* szIMEI);
IQCH_DATABASE_EXPORTS_API bool CreateIMEIRecord(char* szIMEI);


//Use IMEI to query SPC Code from PICS
typedef bool (*pFnQuerySPC)(char* szIMEI, char* szSPC);
IQCH_DATABASE_EXPORTS_API bool QuerySPC(char* szIMEI, char* szSPC);

/*************************************************************
1. CI use IMEI to query SPC Code from PICS. Then CI could unlock SPC protection or write a new SPC code into phone.
PICS should always generate a new SPC, store into PICS and return to CI.
2. The SPC Code (6 digits: 0~9) should be generated randomly excluding 000000 and 999999 every time; it's not unique.
*************************************************************/
typedef bool (*pFnGetSPC)(char* szIMEI, char* szSPC);
IQCH_DATABASE_EXPORTS_API bool GetSPC(char* szIMEI, char* szSPC);


//CI use IMEI to query FSN Code from PICS.
typedef bool (*pfnQueryFSN)(char* szIMEI, char* szFSN);
IQCH_DATABASE_EXPORTS_API bool QueryFSN(char* szIMEI, char* szFSN);
typedef bool (*pfnQueryFSN_Base64)(char* szIMEI, char* szFSN_Base64);
IQCH_DATABASE_EXPORTS_API bool QueryFSN_Base64(char* szIMEI, char* szFSN_Base64);

/***********************************************************************************
1. CI use IMEI to query FSN Code from PICS and then write it into phone.
PICS should always generate a new FSN, store into PICS and return to CI.
2. The FSN Code (16 bytes raw data) should be generated randomly and must be unique for all projects. It can't be overwritten FSN in the flash chip.
3. The output FSN is encoded to base64 format.
***********************************************************************************/
typedef bool (*pFnGetFSN)(char* szIMEI, char* szFSN);
IQCH_DATABASE_EXPORTS_API bool GetFSN(char* szIMEI, char* szFSN);
typedef bool (*pFnGetFSN_Base64)(char* szIMEI, char* szFSN_Base64);
IQCH_DATABASE_EXPORTS_API bool GetFSN_Base64(char* szIMEI, char* szFSN_Base64);

//It 's for base64 format.
typedef bool (*pFnGetSIMData)(char* sznSIMType, char* szIMEI, char* szFSN, char* szSIMFileName, char* szSIM_Data_Base64, char* sznSIM_Data_Length);
IQCH_DATABASE_EXPORTS_API bool GetSIMData(char* sznSIMType, char* szIMEI, char* szFSN, char* szSIMFileName, char* szSIM_Data_Base64, char* sznSIM_Data_Length);

//typedef bool (*pFnGetSIMData)(char* sznSIMType, char* szIMEI, char* szFSN, char* szSIMFileName, char* szSIM_Data, char* sznSIM_Data_Length);
//IQCH_DATABASE_EXPORTS_API bool GetSIMData(char* sznSIMType, char* szIMEI, char* szFSN, char* szSIMFileName, char* szSIM_Data, char* sznSIM_Data_Length);

typedef bool (*pFnQueryUnlockUnblockCode)(char* IMEI,
										  char* szNetwork_Unlock_Code,
										  char* szService_Provider_Unlock_Code,
										  char* szCoporate_Unlock_Code,
										  char* szNetwork_Subset_Unlock_Code,
										  char* zsUser_SIM_Unlock_Code,
										  char* szUser_USIM_Unlock_Code,
										  char* szSerial_Code,
										  char* szSPC,
										  char* szPhone_Lock_Code);
IQCH_DATABASE_EXPORTS_API bool QueryUnlockUnblockCode(char* szIMEI,
							char* szNetwork_Unlock_Code,
							char* szService_Provider_Unlock_Code,
							char* szCoporate_Unlock_Code,
							char* szNetwork_Subset_Unlock_Code,
							char* zsUser_SIM_Unlock_Code,
							char* szUser_USIM_Unlock_Code,
							char* szSerial_Code,
							char* szSPC,
							char* szPhone_Lock_Code);

typedef bool (*pFnQueryUnlockUnblockCode_BT)(char* szIMEI,
											 char* szNetwork_Unlock_Code,
											 char* szService_Provider_Unlock_Code,
											 char* szCoporate_Unlock_Code,
											 char* szNetwork_Subset_Unlock_Code,
											 char* zsUser_SIM_Unlock_Code,
											 char* szUser_USIM_Unlock_Code,
											 char* szSerial_Code,
											 char* szSPC,
											 char* szPhone_Lock_Code);
IQCH_DATABASE_EXPORTS_API bool QueryUnlockUnblockCode_BT(char* szIMEI,
													  char* szNetwork_Unlock_Code,
													  char* szService_Provider_Unlock_Code,
													  char* szCoporate_Unlock_Code,
													  char* szNetwork_Subset_Unlock_Code,
													  char* zsUser_SIM_Unlock_Code,
													  char* szUser_USIM_Unlock_Code,
													  char* szSerial_Code,
													  char* szSPC,
													  char* szPhone_Lock_Code);

//SSSO2 API :
typedef bool (*pFnSSSO2_CheckNAL_PN)(char* Part_Number,char* MAL_FROM_SCANNER,char* IMEI_FROM_SCANNER,char *ErrCode, char *ErrMsg);
IQCH_DATABASE_EXPORTS_API bool SSSO2_CheckNAL_PN(char* Part_Number,char* MAL_FROM_SCANNER,char* IMEI_FROM_SCANNER,char *ErrCode, char *ErrMsg);
typedef bool (*pFnSSSO2_UpdateIMEIStatus)(char* szIMEI,char* Software_Version,char* Hardware_Version,char* MAL_FROM_SCANNER,char *ErrCode, char *ErrMsg);
IQCH_DATABASE_EXPORTS_API bool SSSO2_UpdateIMEIStatus(char* szIMEI,char* Software_Version,char* Hardware_Version,char* MAL_FROM_SCANNER,char *ErrCode, char *ErrMsg);
typedef bool (*pFnSSSO2_QueryIMEI)(char *szIMEI,char* szMSN,char* szBTAddr,char* szSW_ver,char* szHW_ver,char *StationtState, char *ErrCode, char *ErrMsgchar,char *Status_Output);
IQCH_DATABASE_EXPORTS_API bool SSSO2_QueryIMEI(char *szIMEI,char* szMSN,char* szBTAddr,char* szSW_ver,char* szHW_ver,char *StationtState, char *ErrCode, char *ErrMsgchar,char *Status_Output);
typedef bool (*pFnSSSO2_GetProcessResult)(char* szPicasso,char* szModel,char *ErrCode, char *ErrMsg);
IQCH_DATABASE_EXPORTS_API bool SSSO2_GetProcessResult(char* szPicasso,char* szModel,char *ErrCode, char *ErrMsg);

//Just for test.
IQCH_DATABASE_EXPORTS_API bool TestSPUParameter(char* sz01, char* sz02, char* sz03, char* sz04, char* sz05, char* sz06, char* sz07, char* sz08, char* sz09, char* sz10, char* sz11, char* sz12);