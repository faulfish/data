typedef int (__stdcall *NXT_IMPORT_InitializeDataLogResults) (char * pszInitParm, char * pszErrorMsg500);
typedef int (__stdcall *NXT_IMPORT_UnInitializeDataLogResults) (char * pszErrorMsg500);
typedef int (__stdcall *NXT_IMPORT_StartRecipeTest) (char * pszStartRecipeParm, char * pszErrorMsg);
typedef int (__stdcall *NXT_IMPORT_EvalTestResult) ( char * pszEvalTestResultParm, char * pszErrorMsg);
typedef int (__stdcall *NXT_IMPORT_LogTestResult) (char * pszLogTestResultParm,int nFileResultType, char * pszErrorMsg500);
typedef int (__stdcall *NXT_IMPORT_StartTimeMeasBlock) (char * pszBlockName, char * pszErrorMsg500);
typedef int (__stdcall *NXT_IMPORT_FinishTimeMeasBlock) (char * pszBlockName, double * pdTimeMeas, char * pszErrorMsg500);


NXT_IMPORT_InitializeDataLogResults pNXT_IMPORT_InitializeDataLogResults= NULL;
NXT_IMPORT_UnInitializeDataLogResults pNXT_IMPORT_UnInitializeDataLogResults= NULL;
NXT_IMPORT_StartRecipeTest pNXT_IMPORT_StartRecipeTest= NULL;
NXT_IMPORT_EvalTestResult pNXT_IMPORT_EvalTestResult= NULL;
NXT_IMPORT_LogTestResult pNXT_IMPORT_LogTestResult= NULL;
NXT_IMPORT_StartTimeMeasBlock pNXT_IMPORT_StartTimeMeasBlock= NULL;
NXT_IMPORT_FinishTimeMeasBlock pNXT_IMPORT_FinishTimeMeasBlock= NULL;

