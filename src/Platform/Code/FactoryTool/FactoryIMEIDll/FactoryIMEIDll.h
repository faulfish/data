// FactoryIMEIDll.h : main header file for the FactoryIMEIDll DLL
//

long IMEI_Status=0; 
char IMEI_SPC_Code[7]="\0";

char IMEI_BT_Network_Unlock_Code[17]="\0";
char IMEI_BT_Service_Provider_Unlock_Code[17]="\0";
char IMEI_BT_Coporate_Unlock_Code[17]="\0";
char IMEI_BT_Network_Subset_Unlock_Code[17]="\0";
char IMEI_BT_User_SIM_Unlock_Code[17]="\0";
char IMEI_BT_User_USIM_Unlock_Code[17]="\0";
char IMEI_BT_Serial_Code[17]="\0";
char IMEI_BT_Phone_Lock_Code[17]="\0";

typedef struct
{
	char *szNetworkCode;							///< ' Unlock code for Network Lock.
	char *szServiceProviderCode;					///< ' Unlock code for Service Provider Lock.
	char *szCorporateCode;							///< ' Unlock code for Corporate Lock.
	char *szNetworkSubsetCode;						///< ' Unlock code for Network Subset Lock.
	char *szSimCode;								///< ' Unlock code for SIM Lock.
	char *szUsimCode;								///< ' Unblock code for User SIM Lock.
	char *szReserved;								///< ' Reserved code for furture usage.
} T_SIM_UNLOCK_CODE;

typedef int (__stdcall *ISimEnc_AllocSimEncObj)(int, void*&);
typedef int (__stdcall *ISimEnc_SetImei)(const void*, char*, int);
typedef int (__stdcall *ISimEnc_SetFlashSerialNumber)(const void*, char*, int);
typedef int (__stdcall *ISimEnc_SetSimLockFile)(const void*, char*);
typedef int (__stdcall *ISimEnc_SetUnlockCodes)(const void*, T_SIM_UNLOCK_CODE *ptSimUnlockCode);
typedef int (__stdcall *ISimEnc_CheckSimLockData)(const void*,char*, int);
typedef int (__stdcall *ISimEnc_FreeSimEncObj)(const void *);
typedef int (__stdcall *ISimEnc_GenSimLockData)(const void *hSimEncHandle, unsigned char *pcSimLockData, int *pnSimLockDataLen);

ISimEnc_AllocSimEncObj AllocSimEncObj;
ISimEnc_SetImei SetImei;
ISimEnc_SetFlashSerialNumber SetFlashSerialNumber;
ISimEnc_SetSimLockFile SetSimLockFile;
ISimEnc_SetUnlockCodes SetUnlockCodes;
ISimEnc_CheckSimLockData CheckSimLockData;
ISimEnc_FreeSimEncObj FreeSimEncObj;
ISimEnc_GenSimLockData GenSimLockData;

#ifdef FACTORYIMEIDLL_EXPORTS
#define FACTORYIMEIDLL_API extern "C" __declspec(dllexport)
#else
#define FACTORYIMEIDLL_API extern "C" __declspec(dllimport)
#endif
#include <string>

FACTORYIMEIDLL_API bool GetBytesFromFactoryArea(const char* szFactory, const char* szIndex, const char* szBytes, char* szData);
FACTORYIMEIDLL_API bool SetPicassoTranceiverGSN(char* szFactory, const char* szPicassoNo, const char* szTransNO, char* szGSNString);
FACTORYIMEIDLL_API bool SetPicassoTranceiver(char* szFactory, const char* szPicassoNo, const char* szTransNO);
FACTORYIMEIDLL_API bool SetKLFByte(char* szFactory,char *szComputeName);
FACTORYIMEIDLL_API bool CompareFactoryArea(char* szFactory, const char* szPicassoNo, const char* szTransNO);


FACTORYIMEIDLL_API bool ComCtryObjInitial(const char* szCurDirect);

///////////////PICS
FACTORYIMEIDLL_API bool Create_IMEI_PICS(char *szInIMEI,char * szRes);
FACTORYIMEIDLL_API bool QuerySpc_PICS(char *szInIMEI,char *szRes,char *szOutSpc);
FACTORYIMEIDLL_API bool GetFSNCode_PICS(char *szInIMEI,char * szFSN,char *szRes);
FACTORYIMEIDLL_API bool GetSPCCode_PICS(char *szInIMEI,char * szSPC,char *szRes);
FACTORYIMEIDLL_API bool QueryFSNCode_PICS(char *szInIMEI,char * szFSN,char *szRes);
FACTORYIMEIDLL_API bool GetSIMData_PICS(char* szSimType,char *szInIMEI,char * szFSN,char *szSimDataFile ,char *szSimDataFromPICS,char *szSimDataSize,char *szRes);
FACTORYIMEIDLL_API bool QueryUnlockUnblockCode_PICS(char *IMEI_Array,
												   char *Network_Unlock_Code_Array,
												   char *Service_Provider_Unlock_Code_Array,
												   char *Coporate_Unlock_Code_Array,
												   char *Network_Subset_Unlock_Code_Array,
												   char *User_SIM_Unlock_Code_Array,
												   char *User_USIM_Unlock_Code_Array,
												   char *Serial_Code_Array,
												   char *SPC_Array,
												   char *Phone_Lock_Code_Array);

///////////////SSS02
FACTORYIMEIDLL_API bool Check_MAL_PN_SSSO2(char *szInIMEI,char *szPort,char *szInMal,char *szErrCode, char *szErrMsg);
FACTORYIMEIDLL_API bool GetProcessResult_SSS02(char *Picasso, char* Model, char* szErrCode, char* szErrMsg);
FACTORYIMEIDLL_API bool QueryIMEI_SSSO2(char *szStationtState,
										char *pIMEI,
										char *szMSN,
										char *szBTAddr,
										char *szSWver,
										char *szHWver,
										char *szErrCode, 
										char *szErrMsg,
										char *szStatusOutput);
FACTORYIMEIDLL_API bool UpdateIMEIStatus_SSSO2(char *pIMEI,char *szSWver,char *szHWver,char *szMal,char *szErrCode, char *szErrMsg);

FACTORYIMEIDLL_API bool CheckPicassoSMT_SSS02(char *Picasso, char* szProcess);
FACTORYIMEIDLL_API bool CheckPicassoPICS_SSS02(char* szModel, char* Picasso, char* szIMEI, char* szMobileID, char* szProcess, char* szError);
///////////////SimEncorder


FACTORYIMEIDLL_API bool CheckSimData_SimEncorder(char* szSimType,
												 char *szIMEIFromPhone,
												 char *szFSN,
												 char*szIMEIFromScanner,
												 char *szSimDateFileName,
												 char *szSimDataForPhone_Base64,
												 char *szLogString);