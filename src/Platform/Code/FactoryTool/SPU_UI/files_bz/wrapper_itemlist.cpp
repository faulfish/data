#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <winbase.h>
#include "stdlib.h"


#include "wrapper_itemlist.h"
#include "wrapper_itemlist_import.h"

HINSTANCE DlDllHandleItemList;
#define LOAD_WITH_ALTERED_SEARCH_PATH   0x00000008


int LoadItemListWrapperDll(void)
{
	int nStatus = -1;
	DlDllHandleItemList = LoadLibraryEx("C:\\prod\\bin\\wrapperitemlist.dll",NULL,LOAD_WITH_ALTERED_SEARCH_PATH);
	
	if(DlDllHandleItemList)
		nStatus = 0;

	return nStatus;
}

int LoadItemListWrapperFunctions(void)
{
	int nStatus = -1;
	pNXT_IMPORT_InitItemList = (NXT_IMPORT_InitItemList)GetProcAddress((HINSTANCE)DlDllHandleItemList,"InitItemList");
	pNXT_IMPORT_CloseItemList = (NXT_IMPORT_CloseItemList)GetProcAddress((HINSTANCE)DlDllHandleItemList,"CloseItemList");
	pNXT_IMPORT_LoadAndReturnItemListFile =(NXT_IMPORT_LoadAndReturnItemListFile)GetProcAddress((HINSTANCE)DlDllHandleItemList,"LoadAndReturnItemListFile");
	pNXT_IMPORT_LoadFileAndConvertToItemList =(NXT_IMPORT_LoadFileAndConvertToItemList)GetProcAddress((HINSTANCE)DlDllHandleItemList,"LoadFileAndConvertToItemList");
	pNXT_IMPORT_ReturnItemListSetValues =(NXT_IMPORT_ReturnItemListSetValues)GetProcAddress((HINSTANCE)DlDllHandleItemList,"ReturnItemListSetValues");
	pNXT_IMPORT_ReturnItemValue =(NXT_IMPORT_ReturnItemValue)GetProcAddress((HINSTANCE)DlDllHandleItemList,"ReturnItemValue");
	pNXT_IMPORT_ReturnItemListWrapperVersion =(NXT_IMPORT_ReturnItemListWrapperVersion)GetProcAddress((HINSTANCE)DlDllHandleItemList,"ReturnItemListWrapperVersion");
	pNXT_IMPORT_SetCallBackFunctionPointer =(NXT_IMPORT_SetCallBackFunctionPointer)GetProcAddress((HINSTANCE)DlDllHandleItemList,"SetCallBackFunctionPointer");

	
	if(pNXT_IMPORT_InitItemList && pNXT_IMPORT_CloseItemList && 
		pNXT_IMPORT_LoadFileAndConvertToItemList && pNXT_IMPORT_ReturnItemListSetValues &&
		pNXT_IMPORT_ReturnItemValue && pNXT_IMPORT_ReturnItemListWrapperVersion && pNXT_IMPORT_SetCallBackFunctionPointer)
		nStatus = 0;
	
	return nStatus;

}
 
int InitItemList()
{
	int nStatus = -1;
	
	nStatus = LoadItemListWrapperDll();

	if (0 == nStatus)
		nStatus = LoadItemListWrapperFunctions();

	if (0 == nStatus)		
		nStatus = (pNXT_IMPORT_InitItemList)();
	

	return nStatus;
}

int ReturnItemListSetValues(char * pszItemList, char * pszItemListSetName, char * pszTagListToSearch, char * pszTagListCriteria, char * pszTagListCriteriaValues, char ** pszSearchResult, bool bEnableMultipleList, char * pErrorMessage500)
{
    int	nStatus = -1;

	CString strTagValue = _T("");

	nStatus = (pNXT_IMPORT_ReturnItemListSetValues)( pszItemList,  pszItemListSetName,  pszTagListToSearch,  pszTagListCriteria,  pszTagListCriteriaValues,  pszSearchResult,  bEnableMultipleList, pErrorMessage500);
														
	return nStatus;

}

int ReturnItemValue(char * pszItemList,char * pszItemSearch,char ** pszSearchResult,char * pErrorMessage500)
{
    int	nStatus = -1;

	CString strTagValue = _T("");

	nStatus = (pNXT_IMPORT_ReturnItemValue)( pszItemList, pszItemSearch, pszSearchResult, pErrorMessage500);
														
	return nStatus;

}

int LoadFileAndConvertToItemList(char * pFileName, char ** pszFileContentsAddr, char * pErrorMessage500 )
{
    int	nStatus = -1;

	CString strTagValue = _T("");

	nStatus = (pNXT_IMPORT_LoadFileAndConvertToItemList)(pFileName,  pszFileContentsAddr, pErrorMessage500 );
														
	return nStatus;

}

int LoadAndReturnItemListFile(char * pFileName, char ** pszFileContentsAddr, char * pErrorMessage500 )
{
    int	nStatus = -1;

	CString strTagValue = _T("");

	nStatus = (pNXT_IMPORT_LoadAndReturnItemListFile)( pFileName,  pszFileContentsAddr, pErrorMessage500 );
														
	return nStatus;

}

int SetCallBackFunctionPointer(char* pszFnName, FARPROC pFn,char * pszErrMsg500)
{
	int nStatus = -1;

	if(pNXT_IMPORT_SetCallBackFunctionPointer)
		nStatus = (pNXT_IMPORT_SetCallBackFunctionPointer)(pszFnName,pFn,pszErrMsg500);

	return nStatus;
	

}


int CloseItemList(void)
{
	int nStatus = -1;
	
	if (pNXT_IMPORT_CloseItemList)
		nStatus = (pNXT_IMPORT_CloseItemList)();

	if (0 == nStatus)
	{
		if(DlDllHandleItemList)
		{
			FreeLibrary(DlDllHandleItemList);

			pNXT_IMPORT_InitItemList= NULL;
			pNXT_IMPORT_CloseItemList = NULL;
			pNXT_IMPORT_LoadAndReturnItemListFile=NULL;
			pNXT_IMPORT_LoadFileAndConvertToItemList=NULL;
			pNXT_IMPORT_ReturnItemListSetValues=NULL;
			pNXT_IMPORT_ReturnItemValue=NULL;
			pNXT_IMPORT_ReturnItemListWrapperVersion=NULL;
			pNXT_IMPORT_SetCallBackFunctionPointer = NULL;
		

		}
	}
	
	return nStatus;

}