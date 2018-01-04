#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <winbase.h>
#include "stdlib.h"
#include <cassert>

#include "LogResult.h"
#include "LogResult_import.h"

HINSTANCE DlDllHandleLogResult;
#define LOAD_WITH_ALTERED_SEARCH_PATH   0x00000008


int LoadLogResultDll(void)
{
	int nStatus = -1;
	DlDllHandleLogResult = LoadLibraryEx("c:\\prod\\bin\\OdmDataLogResults.dll",NULL,LOAD_WITH_ALTERED_SEARCH_PATH);

	//DlDllHandleLogResult = LoadLibraryEx("D:\\Marcio\\ODM\\Implementations_BZ\\files_bz\\OdmDataLogResults.dll",NULL,LOAD_WITH_ALTERED_SEARCH_PATH);
	
	if(DlDllHandleLogResult)
		nStatus = 0;

	return nStatus;
}

int LoadLogResultFunctions(void)
{
	int nStatus = -1;

	pNXT_IMPORT_InitializeDataLogResults = (NXT_IMPORT_InitializeDataLogResults)GetProcAddress((HINSTANCE)DlDllHandleLogResult,"InitializeDataLogResults");
	pNXT_IMPORT_UnInitializeDataLogResults = (NXT_IMPORT_UnInitializeDataLogResults)GetProcAddress((HINSTANCE)DlDllHandleLogResult,"UnInitializeDataLogResults");
	pNXT_IMPORT_StartRecipeTest = (NXT_IMPORT_StartRecipeTest)GetProcAddress((HINSTANCE)DlDllHandleLogResult,"StartRecipeTest");
	pNXT_IMPORT_EvalTestResult = (NXT_IMPORT_EvalTestResult)GetProcAddress((HINSTANCE)DlDllHandleLogResult,"EvalTestResult");
	pNXT_IMPORT_LogTestResult = (NXT_IMPORT_LogTestResult)GetProcAddress((HINSTANCE)DlDllHandleLogResult,"LogTestResult");
	pNXT_IMPORT_StartTimeMeasBlock = (NXT_IMPORT_StartTimeMeasBlock)GetProcAddress((HINSTANCE)DlDllHandleLogResult,"StartTimeMeasBlock");
	pNXT_IMPORT_FinishTimeMeasBlock = (NXT_IMPORT_FinishTimeMeasBlock)GetProcAddress((HINSTANCE)DlDllHandleLogResult,"FinishTimeMeasBlock");


	if(pNXT_IMPORT_InitializeDataLogResults && pNXT_IMPORT_UnInitializeDataLogResults 
		&& pNXT_IMPORT_StartRecipeTest && pNXT_IMPORT_EvalTestResult && pNXT_IMPORT_LogTestResult
		&& pNXT_IMPORT_StartTimeMeasBlock && pNXT_IMPORT_FinishTimeMeasBlock)
			nStatus = 0;
	
	return nStatus;

}

 
int InitLogResult(char * pszParameter, char * pszErrorMsg500)
{
	int nStatus = -1;
	
	nStatus = (pNXT_IMPORT_InitializeDataLogResults)(pszParameter, pszErrorMsg500);

	return nStatus;
}

int LoadLogResult()
{
	int nStatus = -1;
	
	nStatus = LoadLogResultDll();

	if (0 == nStatus)
		nStatus = LoadLogResultFunctions();

	return nStatus;
}


int StartRecipeTest(char * pszStartRecipeParm, char * pszErrorMsg500)
{
	int nStatus=-1;

	nStatus = (pNXT_IMPORT_StartRecipeTest)(pszStartRecipeParm, pszErrorMsg500);
	
	return nStatus;
}
int EvalTestResult( char * pszEvalTestResultParm, char * pszErrorMsg500)
{
	int nStatus=-1;

	//Alan 20090331
	assert(pNXT_IMPORT_EvalTestResult);
	if(pNXT_IMPORT_EvalTestResult)
		nStatus = (pNXT_IMPORT_EvalTestResult)(pszEvalTestResultParm, pszErrorMsg500);
	else
		nStatus = 0;

	return nStatus;
}
int LogTestResult(char * pszLogTestResultParm,int nFileResultType, char * pszErrorMsg500)
{
	int nStatus=-1;
	//Alan 20090331
	assert(pNXT_IMPORT_LogTestResult);
	if(pNXT_IMPORT_LogTestResult)
		nStatus = (pNXT_IMPORT_LogTestResult)(pszLogTestResultParm, nFileResultType, pszErrorMsg500);
	return nStatus;
}
int StartTimeMeasBlock(char * pszBlockName, char * pszErrorMsg500)
{
	int nStatus = -1;
	
	nStatus = (pNXT_IMPORT_StartTimeMeasBlock)(pszBlockName, pszErrorMsg500);

	return nStatus;
}
int FinishTimeMeasBlock(char * pszBlockName, double * pdTimeMeas, char * pszErrorMsg500)
{
	int nStatus = -1;
	
	nStatus = (pNXT_IMPORT_FinishTimeMeasBlock)(pszBlockName, pdTimeMeas, pszErrorMsg500);

	return nStatus;
}

int CloseLogResult(char * pszErrorMsg500)
{
	int nStatus = -1;
			
	if (pNXT_IMPORT_UnInitializeDataLogResults)
		nStatus = (pNXT_IMPORT_UnInitializeDataLogResults)(pszErrorMsg500);

	if (0 == nStatus)
	{
		if(DlDllHandleLogResult)
		{
			FreeLibrary(DlDllHandleLogResult);

				pNXT_IMPORT_InitializeDataLogResults = NULL;
				pNXT_IMPORT_UnInitializeDataLogResults = NULL;
				pNXT_IMPORT_StartRecipeTest = NULL;
				pNXT_IMPORT_EvalTestResult = NULL;
				pNXT_IMPORT_LogTestResult = NULL;
				pNXT_IMPORT_StartTimeMeasBlock = NULL;
				pNXT_IMPORT_FinishTimeMeasBlock = NULL;

		}
	}
	return nStatus;
}
