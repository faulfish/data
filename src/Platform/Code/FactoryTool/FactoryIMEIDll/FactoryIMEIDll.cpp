// FactoryIMEIDll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <atlenc.h>
#include "FactoryIMEIDll.h"
#include "PicsCtrl.h"
#include "SSSO2.h"

#define SPU_BUFFER_SIZE 2048

CSSSO2 SSSO2_Ctrl;
CPicsCtrl Pics_Interface_Ctrl;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only application object

CWinApp theApp;

using namespace std;

#pragma warning(push)
#pragma warning(disable:4996)

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}

FACTORYIMEIDLL_API bool GetBytesFromFactoryArea(const char* szFactory, const char* szIndex, const char* szBytes, char* szData)
{
	int nIndex = atoi(szIndex);
	int nBytes = atoi(szBytes);

	BYTE  szFactoryArea128[256] = {0};
	int nDataLen = Base64DecodeGetRequiredLength((int)strlen(szFactory));
	memset(szFactoryArea128, 0x00, nDataLen);

	bool bRes = Base64Decode(szFactory, (int)strlen(szFactory), szFactoryArea128, &nDataLen) == TRUE;
	if(bRes)
	{
		if(nIndex == 45)
		{
			BYTE szDataTemp[128] = {0};
			memcpy(szDataTemp, &szFactoryArea128[nIndex], nBytes);
			nDataLen = Base64EncodeGetRequiredLength(nBytes);
			Base64Encode(szDataTemp, nBytes, szData, &nDataLen);
		}
		else
		{
			memcpy(szData, &szFactoryArea128[nIndex], nBytes);
			szData[nBytes] = '\0';
		}	
	}
	return bRes;
}

FACTORYIMEIDLL_API bool SetPicassoTranceiverGSN(char* szFactory, const char* szPicassoNo, const char* szTransNO, char* szGSNString)
{

	BYTE  szFactoryArea128[256] = {0};
	int nDataLen = Base64DecodeGetRequiredLength((int)strlen(szFactory));
	memset(szFactoryArea128, 0x00, nDataLen);

	bool bRes = Base64Decode(szFactory, (int)strlen(szFactory), szFactoryArea128, &nDataLen) == TRUE;
	if(bRes)
	{
		BYTE szDataTemp[128] = {0};
		for(int i=0;i<10;i++)//94 to 103 is for picasso no
		{
			szFactoryArea128[94+i]=szPicassoNo[i];
		}

		for(int i=0;i<10;i++)//104 to 113 is for tranceiver no
		{
			szFactoryArea128[104+i]=szTransNO[i];
		}

		for(int i=0;i<10;i++)//114 to 123 is for MSN
		{
			szFactoryArea128[114+i]=szGSNString[i];
		}

		memcpy(szDataTemp, &szFactoryArea128[0], 128);
		nDataLen = Base64EncodeGetRequiredLength(128);
		bRes = Base64Encode(szDataTemp, 128, szFactory, &nDataLen) == TRUE;
	}
	return bRes;
}
FACTORYIMEIDLL_API bool SetPicassoTranceiver(char* szFactory, const char* szPicassoNo, const char* szTransNO)
{

	BYTE  szFactoryArea128[256] = {0};
	int nDataLen = Base64DecodeGetRequiredLength((int)strlen(szFactory));
	memset(szFactoryArea128, 0x00, nDataLen);

	bool bRes = Base64Decode(szFactory, (int)strlen(szFactory), szFactoryArea128, &nDataLen) == TRUE;
	if(bRes)
	{
		BYTE szDataTemp[128] = {0};
		for(int i=0;i<10;i++)//94 to 103 is for picasso no
		{
			szFactoryArea128[94+i]=szPicassoNo[i];
		}

		for(int i=0;i<10;i++)//104 to 113 is for tranceiver no
		{
			szFactoryArea128[104+i]=szTransNO[i];
		}

		memcpy(szDataTemp, &szFactoryArea128[0], 128);
		nDataLen = Base64EncodeGetRequiredLength(128);
		bRes = Base64Encode(szDataTemp, 128, szFactory, &nDataLen) == TRUE;
	}
	return bRes;
}

FACTORYIMEIDLL_API bool SetKLFByte(char* szFactory,char *szComputeName)
{
	CString Computer_Name;
	Computer_Name.Format("%s",szComputeName);
	BYTE  szFactoryArea128[256] = {0};
	int nDataLen = Base64DecodeGetRequiredLength((int)strlen(szFactory));
	memset(szFactoryArea128, 0x00, nDataLen);

	bool bRes = Base64Decode(szFactory, (int)strlen(szFactory), szFactoryArea128, &nDataLen) == TRUE;
	if(bRes)
	{
		BYTE szDataTemp[128] = {0};

		unsigned char KLF_Byte=szFactoryArea128[3];
		KLF_Byte|=0x80;
		szFactoryArea128[3]=KLF_Byte;

		//sunlight add for test count   
		szFactoryArea128[59]+=0x01;
		//sunlight add for process info
		CTime Time=CTime::GetCurrentTime();
		szFactoryArea128[29]=Time.GetDay();
		szFactoryArea128[30]=Time.GetMonth();
		szFactoryArea128[31]=Time.GetYear()-2000;
		szFactoryArea128[32]=((unsigned char)(atoi(Computer_Name.Mid(4,2)))<<4)+(unsigned char)(atoi(Computer_Name.Mid(1,2)));
		
		memcpy(szDataTemp, &szFactoryArea128[0], 128);
		nDataLen = Base64EncodeGetRequiredLength(128);
		bRes = Base64Encode(szDataTemp, 128, szFactory, &nDataLen) == TRUE;
	}
	return bRes;
}
FACTORYIMEIDLL_API bool CompareFactoryArea(char* szFactory, const char* szPicassoNo, const char* szTransNO)
{

	BYTE  szFactoryArea128[256] = {0};
	int nDataLen = Base64DecodeGetRequiredLength((int)strlen(szFactory));
	memset(szFactoryArea128, 0x00, nDataLen);
	char Picasso_Read[11]={0};
	char XCVR_Read[11]={0};

	bool bRes = Base64Decode(szFactory, (int)strlen(szFactory), szFactoryArea128, &nDataLen) == TRUE;
	if(bRes)
	{
		for(int i=0;i<10;i++)
		{
			Picasso_Read[i]=szFactoryArea128[94+i];
			XCVR_Read[i]=szFactoryArea128[104+i];
		}
		if((strncmp((char*)Picasso_Read,szPicassoNo,10)!=0)||(strncmp((char*)XCVR_Read,szTransNO,10)!=0))
		{
			return false;
		}
	}
	return bRes;
}
FACTORYIMEIDLL_API bool ComCtryObjInitial(const char* szCurDirect)
{
	if((!SSSO2_Ctrl.Initial_SSSO2Dll())||(!Pics_Interface_Ctrl.Initial_PICSInterfaceDLL()))
		return FALSE;
	else 
		return TRUE;
}

//////////PICS///////////
FACTORYIMEIDLL_API bool Create_IMEI_PICS(char *szInIMEI,char * szRes)
{
	short temp = Pics_Interface_Ctrl.CreateIMEIRecord(szInIMEI);
	_itoa_s(temp,szRes,SPU_BUFFER_SIZE,10);
	return temp==0?TRUE:FALSE;
}

FACTORYIMEIDLL_API bool QuerySpc_PICS(char *szInIMEI,char *szRes,char *szOutSpc)
{
	short temp = Pics_Interface_Ctrl.QuerySPCFromPICS(szInIMEI,szOutSpc);
	_itoa_s(temp,szRes,SPU_BUFFER_SIZE,10);
	return temp==0?TRUE:FALSE;
}
FACTORYIMEIDLL_API bool GetFSNCode_PICS(char *szInIMEI,char * szFSN,char *szRes)
{
	short temp = Pics_Interface_Ctrl.GetFSNCode(szInIMEI,szFSN);
	_itoa_s(temp,szRes,SPU_BUFFER_SIZE,10);
	return temp==0?TRUE:FALSE;
}
FACTORYIMEIDLL_API bool GetSPCCode_PICS(char *szInIMEI,char * szSPC,char *szRes)
{
	short temp = Pics_Interface_Ctrl.GetSPCCodeFromPICS(szInIMEI,szSPC);
	_itoa_s(temp,szRes,SPU_BUFFER_SIZE,10);
	return temp==0?TRUE:FALSE;
}
FACTORYIMEIDLL_API bool QueryFSNCode_PICS(char *szInIMEI,char * szFSN,char *szRes)
{
	short temp = Pics_Interface_Ctrl.QueryFSNCode(szInIMEI,szFSN);
	_itoa_s(temp,szRes,SPU_BUFFER_SIZE,10);
	return temp==0?TRUE:FALSE;
}
FACTORYIMEIDLL_API bool GetSIMData_PICS(char* szSimType,char *szInIMEI,char * szFSN,char *szSimDataFile ,char *szSimDataFromPICS,char *szSimDataSize,char *szRes)
{						
	short nSimType = atoi(szSimType);
	short nSimDataSize = atoi(szSimDataSize); 
	char szSimDataFromPICS_Temp[4096] = {0};
	short temp = Pics_Interface_Ctrl.GetSIMData(nSimType,szInIMEI,szFSN,szSimDataFile,szSimDataFromPICS_Temp,&nSimDataSize);
	ASSERT(SPU_BUFFER_SIZE > nSimDataSize*2);
	if(SPU_BUFFER_SIZE > nSimDataSize*2)
	{
		memcpy(szSimDataFromPICS,szSimDataFromPICS_Temp,nSimDataSize*2);
		_itoa_s(nSimDataSize,szSimDataSize,SPU_BUFFER_SIZE,10);
		_itoa_s(temp,szRes,SPU_BUFFER_SIZE,10);
	}
	return temp==0?TRUE:FALSE;
}
FACTORYIMEIDLL_API bool QueryUnlockUnblockCode_PICS(char *IMEI_Array,
													char *Network_Unlock_Code_Array,
													char *Service_Provider_Unlock_Code_Array,
													char *Coporate_Unlock_Code_Array,
													char *Network_Subset_Unlock_Code_Array,
													char *User_SIM_Unlock_Code_Array,
													char *User_USIM_Unlock_Code_Array,
													char *Serial_Code_Array,
													char *SPC_Array,
													char *Phone_Lock_Code_Array)
{
	short temp = Pics_Interface_Ctrl.QueryUnlockUnblockCode_BT(IMEI_Array,
															   Network_Unlock_Code_Array,
															   Service_Provider_Unlock_Code_Array,
															   Coporate_Unlock_Code_Array,
															   Network_Subset_Unlock_Code_Array,
															   User_SIM_Unlock_Code_Array,
															   User_USIM_Unlock_Code_Array,
															   Serial_Code_Array,
															   SPC_Array,
															   Phone_Lock_Code_Array);
	return temp==0?TRUE:FALSE;
}

//////////SSS02///////////
FACTORYIMEIDLL_API bool Check_MAL_PN_SSSO2(char *szInIMEI,char *szPort,char *szInMal,char *szErrCode, char *szErrMsg)
{
	return SSSO2_Ctrl.SSSO2_CheckNAL_PN(szInIMEI,szPort,szInMal,szErrCode,szErrMsg);
}
FACTORYIMEIDLL_API bool GetProcessResult_SSS02(char *Picasso, char* Model, char* szErrCode, char* szErrMsg)
{
	return SSSO2_Ctrl.SSSO2_GetProcessResult(Picasso, Model, szErrCode, szErrMsg);
}
FACTORYIMEIDLL_API bool CheckPicassoSMT_SSS02(char *Picasso, char* szProcess)
{
	return SSSO2_Ctrl.SSS02_CheckPicassoInSMT(Picasso, szProcess);
}

FACTORYIMEIDLL_API bool CheckPicassoPICS_SSS02(char* szModel, char* Picasso, char* szIMEI, char* szMobileID, char* szProcess, char* szError)
{
	return SSSO2_Ctrl.SSS02_CheckPiacssoInPICS(szModel, Picasso, szIMEI, szMobileID, szProcess, szError);
}

FACTORYIMEIDLL_API bool QueryIMEI_SSSO2(char *szStationtState,
										char *pIMEI,
										char *szMSN,
										char *szBTAddr,
										char *szSWver,
										char *szHWver,
										char *szErrCode, 
										char *szErrMsg,
										char *szStatusOutput)
{
	return SSSO2_Ctrl.SSSO2_QueryIMEI(szStationtState,
										pIMEI,
										szMSN,
										szBTAddr,
										szSWver,
										szHWver,
										szErrCode,
										szErrMsg
										,szStatusOutput);
}

FACTORYIMEIDLL_API bool UpdateIMEIStatus_SSSO2(char *pIMEI,char *szSWver,char *szHWver,char *szMal,char *szErrCode, char *szErrMsg)
{
	return SSSO2_Ctrl.SSSO2_UpdateIMEIStatus(pIMEI,szSWver,szHWver,szMal,szErrCode,szErrMsg);
}


/////////////////SimEncorder

FACTORYIMEIDLL_API bool CheckSimData_SimEncorder(char* szSimType,
												 char *szIMEIFromPhone,
												 char *szFSN,
												 char*szIMEIFromScanner,
												 char *szSimDateFileName,
												 char *szSimDataForPhone_Base64,
												 char *szLogString)
{
	
	char temp_log[1024];
	CString LogStr;
	HMODULE hDll2 = ::LoadLibrary(_T("SimEncorder.dll"));
	ASSERT(hDll2);
	if(hDll2 == NULL) return false;
	AllocSimEncObj       = (ISimEnc_AllocSimEncObj )::GetProcAddress(hDll2, "_ISimEnc_AllocSimEncObj@8");
	SetImei              = (ISimEnc_SetImei )::GetProcAddress(hDll2, "_ISimEnc_SetImei@12");
	SetFlashSerialNumber = (ISimEnc_SetFlashSerialNumber )::GetProcAddress(hDll2, "_ISimEnc_SetFlashSerialNumber@12");
	SetSimLockFile       = (ISimEnc_SetSimLockFile )::GetProcAddress(hDll2, "_ISimEnc_SetSimLockFile@8");
	SetUnlockCodes       = (ISimEnc_SetUnlockCodes )::GetProcAddress(hDll2, "_ISimEnc_SetUnlockCodes@8");
	CheckSimLockData     = (ISimEnc_CheckSimLockData )::GetProcAddress(hDll2, "_ISimEnc_CheckSimLockData@12");
	FreeSimEncObj        = (ISimEnc_FreeSimEncObj )::GetProcAddress(hDll2, "_ISimEnc_FreeSimEncObj@4");
	
	int nSimType = atoi(szSimType);
	void * hSimEncHandle = NULL;
	int result=NULL;
	result=AllocSimEncObj(nSimType,hSimEncHandle);
	if(result!=0)
	{
		sprintf(temp_log,"AllocSimEncObj fail, return %d",result);
		LogStr+=temp_log;
		LogStr+="\n";
		::memcpy(szLogString,LogStr.GetBuffer(),LogStr.GetLength());
		szLogString[LogStr.GetLength()] ='\0';
		return false;
	}

	result=SetImei(hSimEncHandle,szIMEIFromPhone, (int)strlen(szIMEIFromPhone));
	if(result!=0)
	{
		sprintf(temp_log,"SetImei fail, return %d",result);
		LogStr+=temp_log;
		LogStr+="\n";
		::memcpy(szLogString,LogStr.GetBuffer(),LogStr.GetLength());
		szLogString[LogStr.GetLength()] ='\0';
		return false;
	}

	result=SetFlashSerialNumber(hSimEncHandle,szFSN, (int)strlen(szFSN));
	if(result!=0)
	{
		sprintf(temp_log,"SetFlashSerialNumber fail, return %d",result);
		LogStr+=temp_log;
		LogStr+="\n";
		::memcpy(szLogString,LogStr.GetBuffer(),LogStr.GetLength());
		szLogString[LogStr.GetLength()] ='\0';
		return false;
	}

	sprintf(temp_log,"\\\\10.94.5.50\\simfile\\%s",szSimDateFileName);
	result=SetSimLockFile(hSimEncHandle,temp_log);
	if(result!=0)
	{
		sprintf(temp_log,"SetSimLockFile fail, return %d",result);
		LogStr+=temp_log;
		LogStr+="\n";
		::memcpy(szLogString,LogStr.GetBuffer(),LogStr.GetLength());
		szLogString[LogStr.GetLength()] ='\0';
		return false;
	}
	T_SIM_UNLOCK_CODE tSimUnlockCode;
	sprintf(temp_log,"Query unlock unblock code from PICS by IMEI(%s)......",szIMEIFromScanner);
	LogStr+=temp_log;
	LogStr+="\n";
	IMEI_Status=Pics_Interface_Ctrl.QueryUnlockUnblockCode_BT(szIMEIFromScanner,
		IMEI_BT_Network_Unlock_Code,
		IMEI_BT_Service_Provider_Unlock_Code,
		IMEI_BT_Coporate_Unlock_Code,
		IMEI_BT_Network_Subset_Unlock_Code,
		IMEI_BT_User_USIM_Unlock_Code,
		IMEI_BT_User_SIM_Unlock_Code,
		IMEI_BT_Serial_Code,
		IMEI_SPC_Code,
		IMEI_BT_Phone_Lock_Code);
	if(IMEI_Status)
	{
		sprintf(temp_log,"Query unlock unlock code from PICS fail......");
		LogStr+=temp_log;
		LogStr+="\n";
		::memcpy(szLogString,LogStr.GetBuffer(),LogStr.GetLength());
		szLogString[LogStr.GetLength()] ='\0';
		return false;
	}
	else
	{
		sprintf(temp_log,"Query unlock unblock code from PICS success...NW->%s,SS->%s",IMEI_BT_Network_Unlock_Code,IMEI_BT_User_USIM_Unlock_Code);
	}

	tSimUnlockCode.szNetworkCode			= IMEI_BT_Network_Unlock_Code;
	tSimUnlockCode.szServiceProviderCode	= IMEI_BT_Service_Provider_Unlock_Code;
	tSimUnlockCode.szCorporateCode			= IMEI_BT_Coporate_Unlock_Code;
	tSimUnlockCode.szNetworkSubsetCode		= IMEI_BT_Network_Subset_Unlock_Code;
	tSimUnlockCode.szSimCode				= IMEI_BT_User_SIM_Unlock_Code;
	tSimUnlockCode.szUsimCode				= IMEI_BT_User_USIM_Unlock_Code;
	tSimUnlockCode.szReserved				= NULL;

	result=SetUnlockCodes(hSimEncHandle,&tSimUnlockCode);
	if(result!=0)
	{
		sprintf(temp_log,"SetUnlockCodes fail, return %d",result);
		LogStr+=temp_log;
		LogStr+="\n";
		::memcpy(szLogString,LogStr.GetBuffer(),LogStr.GetLength());
		szLogString[LogStr.GetLength()] ='\0';
		return false;
	}
	int nDecLen = Base64DecodeGetRequiredLength( (int)strlen(szSimDataForPhone_Base64));
	BYTE* SimDataForPhone = new BYTE[nDecLen];
	memset(SimDataForPhone, 0, nDecLen);
	Base64Decode(szSimDataForPhone_Base64, (int)strlen(szSimDataForPhone_Base64), SimDataForPhone, &nDecLen);
	result=CheckSimLockData(hSimEncHandle,(char*)SimDataForPhone,nDecLen);
	delete [] SimDataForPhone;

	if(result!=0)
	{
		sprintf(temp_log,"CheckSimLockData fail, return %d",result);
		LogStr+=temp_log;
		LogStr+="\n";
		::memcpy(szLogString,LogStr.GetBuffer(),LogStr.GetLength());
		szLogString[LogStr.GetLength()] ='\0';
		return false;
	}

	result=FreeSimEncObj(hSimEncHandle);
	if(result!=0)
	{
		sprintf(temp_log,"FreeSimEncObj fail, return %d",result);
		LogStr+=temp_log;
		LogStr+="\n";
		::memcpy(szLogString,LogStr.GetBuffer(),LogStr.GetLength());
		szLogString[LogStr.GetLength()] ='\0';
		return false;
	}
	::memcpy(szLogString,LogStr.GetBuffer(),LogStr.GetLength());
	szLogString[LogStr.GetLength()] ='\0';
	return true;
}

#pragma warning(pop)