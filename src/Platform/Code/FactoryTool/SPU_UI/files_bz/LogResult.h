int LoadLogResultDll(void);
int LoadLogResultFunctions(void);
int InitLogResult(char * pszParameter, char * pszErrorMsg500);
int StartRecipeTest(char * pszStartRecipeParm, char * pszErrorMsg500);
int EvalTestResult( char * pszEvalTestResultParm, char * pszErrorMsg500);
int LogTestResult(char * pszLogTestResultParm,int nFileResultType, char * pszErrorMsg500);
int StartTimeMeasBlock(char * pszBlockName, char * pszErrorMsg500);
int FinishTimeMeasBlock(char * pszBlockName, double * pdTimeMeas, char * pszErrorMsg500);
int CloseLogResult(char * pszErrorMsg500);
int LoadLogResult();
