// BRTDllCtrl.cpp: implementation of the CBRTDllCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QUALCOMM_IMEI_CODING.h"
#include "BRTDllCtrl.h"
#include "BRT_Dll.h"
#include "BRTdll.h"
#include "Global_Variable_Declare.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBRTDllCtrl::CBRTDllCtrl()
{

}

CBRTDllCtrl::~CBRTDllCtrl()
{
	if(DllHandle)
		FreeLibrary(DllHandle);
}

void CBRTDllCtrl::BRTPassHandle(HANDLE Phone_Handle)
{
	//BRTdllSetHandle(Phone_Handle);
	pBRTdllSetHandle(Phone_Handle);
	return;
	/* passes handle to connected device; connection not needed */
}
///////////////////////////////////////

int CBRTDllCtrl::BRTBackUpAllNVItem(int MaxID, char *BRTFileName,char *SPC_Array, int Log_Flag, int *NVItem_Count, int *Completed)
{
	//return BRTdllBackupAll(MaxID, BRTFileName,SPC_Array, Log_Flag, NVItem_Count, Completed);//return BRTdllBackupAll(MaxID, BRTFileName,SPC_Array, Log_Flag, NVItem_Count, Completed);
	return pBRTdllBackupAll(MaxID, BRTFileName,SPC_Array, Log_Flag, NVItem_Count, Completed);
	/* backup all NV-items with Ids within range between 0 and maxid */ 
}

int CBRTDllCtrl::BRTBackUpNVItemAccordingToCSV(char *CSVFileName, char *BRTFileName,char *SPC_Array,int Log_Flag,int *NVItem_Count,int *Completed)
{
	//return BRTdllBackupFromList(CSVFileName, BRTFileName,SPC_Array,Log_Flag,NVItem_Count,Completed);
	return pBRTdllBackupFromList(CSVFileName, BRTFileName,SPC_Array,Log_Flag,NVItem_Count,Completed);
	/* backup NV-items specified in input_list file (CSV file) */ 
}

int CBRTDllCtrl::BRTRestore(char *BRTFileName,char *SPC_Array,int Log_Flag, int *NVItem_Count,int *Completed)
{
	//return BRTdllRestore(BRTFileName,SPC_Array,Log_Flag, NVItem_Count,Completed);
	return pBRTdllRestore(BRTFileName,SPC_Array,Log_Flag, NVItem_Count,Completed);
	/* restore all NV-items stored in output_file */
} 

int CBRTDllCtrl::BRTRestoreWithCheck(char *BRTFileName, char *SPC_Array,int Log_Flag,int *NVItem_Count,int *NVItem_Read)
{
	//return BRTdllRestoreWithCheck(BRTFileName, SPC_Array,Log_Flag,NVItem_Count,NVItem_Read);
	return pBRTdllRestoreWithCheck(BRTFileName, SPC_Array,Log_Flag,NVItem_Count,NVItem_Read);
	/* restore all NV-items stored in output_file & check result of restore by reading them from mobile */
}

int CBRTDllCtrl::BRTGetLastError(char *ErrorMessage)
{
	//return BRTdllGetLastError(ErrorMessage);
	return pBRTdllGetLastError(ErrorMessage);
	/* retrives last error message */
}

int CBRTDllCtrl::BRTSetLogFile(char *LogFileName)
{
	//return BRTdllSetLogFile(LogFileName);
	return pBRTdllSetLogFile(LogFileName);
	/* sets full path to log file */
}


unsigned char CBRTDllCtrl::Initial_BRT_Dll()
{
	if(DllHandle)
		FreeLibrary(DllHandle);
	DllHandle=::LoadLibraryEx("BRTdll.dll", NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	
	if(DllHandle==NULL)
		return 0;

	ASSERT(pBRTdllGetDllVersion=(_BRTdllGetDllVersion *)GetProcAddress((HMODULE)DllHandle, "BRTdllGetDllVersion"));

	ASSERT(pBRTdllConnect=(_BRTdllConnect *)GetProcAddress((HMODULE)DllHandle, "BRTdllConnect"));

	ASSERT(pBRTdllDisconnect=(_BRTdllDisconnect *)GetProcAddress((HMODULE)DllHandle, "BRTdllDisconnect"));

	ASSERT(pBRTdllGetHandle=(_BRTdllGetHandle *)GetProcAddress((HMODULE)DllHandle, "BRTdllGetHandle"));

	ASSERT(pBRTdllSetHandle=(_BRTdllSetHandle *)GetProcAddress((HMODULE)DllHandle, "BRTdllSetHandle"));

	ASSERT(pBRTdllBackupAll=(_BRTdllBackupAll *)GetProcAddress((HMODULE)DllHandle, "BRTdllBackupAll"));
 
	ASSERT(pBRTdllBackupFromList=(_BRTdllBackupFromList *)GetProcAddress((HMODULE)DllHandle, "BRTdllBackupFromList"));

	ASSERT(pBRTdllRestore=(_BRTdllRestore *)GetProcAddress((HMODULE)DllHandle, "BRTdllRestore"));

	ASSERT(pBRTdllRestoreWithCheck=(_BRTdllRestoreWithCheck *)GetProcAddress((HMODULE)DllHandle, "BRTdllRestoreWithCheck"));

	ASSERT(pBRTdllGetLastError=(_BRTdllGetLastError *)GetProcAddress((HMODULE)DllHandle, "BRTdllGetLastError"));

	ASSERT(pBRTdllSetLogFile=(_BRTdllSetLogFile *)GetProcAddress((HMODULE)DllHandle, "BRTdllSetLogFile"));
	return true;
}
