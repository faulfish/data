int InitItemList(void);
int CloseItemList(void);
int ReturnItemListSetValues(char * pszItemList, char * pszItemListSetName, char * pszTagListToSearch, char * pszTagListCriteria, char * pszTagListCriteriaValues, char ** pszSearchResult, bool bEnableMultipleList, char * pErrorMessage500);
int ReturnItemValue(char * pszItemList,char * pszItemSearch,char ** pszSearchResult,char * pErrorMessage500);
int LoadFileAndConvertToItemList(char * pFileName, char ** pszFileContentsAddr, char * pErrorMessage500 );
int LoadAndReturnItemListFile(char * pFileName, char ** pszFileContentsAddr, char * pErrorMessage500 );
int SetCallBackFunctionPointer(char* pszFnName, FARPROC pFn,char * pszErrMsg500);

