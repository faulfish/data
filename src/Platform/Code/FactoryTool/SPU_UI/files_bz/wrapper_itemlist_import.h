typedef int (__stdcall *NXT_IMPORT_InitItemList)(void);
typedef int (__stdcall *NXT_IMPORT_CloseItemList)(void);
typedef int (__stdcall *NXT_IMPORT_LoadAndReturnItemListFile)(char * pFileName, char ** pszFileContentsAddr, char * pErrorMessage500 );
typedef int (__stdcall *NXT_IMPORT_LoadFileAndConvertToItemList)(char * pFileName, char ** pszFileContentsAddr, char * pErrorMessage500 );
typedef int (__stdcall *NXT_IMPORT_ReturnItemListSetValues)(char * pszItemList, char * pszItemListSetName, char * pszTagListToSearch, char * pszTagListCriteria, char * pszTagListCriteriaValues, char ** pszSearchResult, bool bEnableMultipleList, char * pErrorMessage500);
typedef int (__stdcall *NXT_IMPORT_ReturnItemValue)(char * pszItemList,char * pszItemSearch,char ** pszSearchResult,char * pErrorMessage500);
typedef int (__stdcall *NXT_IMPORT_ReturnItemListWrapperVersion)(char * pszWrapperVersion500, char * pszErrorMessage500);
typedef int (__stdcall *NXT_IMPORT_SetCallBackFunctionPointer)(char* pszFnName, FARPROC pFn, char * pszErrMsg500 );

NXT_IMPORT_InitItemList pNXT_IMPORT_InitItemList= NULL;
NXT_IMPORT_CloseItemList pNXT_IMPORT_CloseItemList = NULL;
NXT_IMPORT_LoadAndReturnItemListFile pNXT_IMPORT_LoadAndReturnItemListFile=NULL;
NXT_IMPORT_LoadFileAndConvertToItemList pNXT_IMPORT_LoadFileAndConvertToItemList=NULL;
NXT_IMPORT_ReturnItemListSetValues pNXT_IMPORT_ReturnItemListSetValues=NULL;
NXT_IMPORT_ReturnItemValue pNXT_IMPORT_ReturnItemValue=NULL;
NXT_IMPORT_ReturnItemListWrapperVersion pNXT_IMPORT_ReturnItemListWrapperVersion=NULL;
NXT_IMPORT_SetCallBackFunctionPointer pNXT_IMPORT_SetCallBackFunctionPointer = NULL;