
typedef int   _BRTdllGetDllVersion(char *cDllVersion);

typedef int   _BRTdllConnect(int comport);

typedef void  _BRTdllDisconnect(void);

typedef void* _BRTdllGetHandle(void);

typedef void  _BRTdllSetHandle(void* hResourceContext);

typedef int   _BRTdllBackupAll(int maxid, char* output_file, char* SPC_code, int log, int *nv_count, int *complete);
 
typedef int   _BRTdllBackupFromList(char* input_list, char* output_file, char* SPC_code, int log, int *nv_count, int *complete);

typedef int   _BRTdllRestore(char* output_file, char* SPC_code, int log, int *nv_count, int *complete);

typedef int   _BRTdllRestoreWithCheck(char* output_file, char* SPC_code, int log, int *nv_count, int *nv_read);

typedef int   _BRTdllGetLastError(char* cErrMsg);

typedef int   _BRTdllSetLogFile(char* LogFileName);


_BRTdllGetDllVersion *pBRTdllGetDllVersion;

_BRTdllConnect *pBRTdllConnect;

_BRTdllDisconnect *pBRTdllDisconnect;

_BRTdllGetHandle *pBRTdllGetHandle;

_BRTdllSetHandle *pBRTdllSetHandle;

_BRTdllBackupAll *pBRTdllBackupAll;
 
_BRTdllBackupFromList *pBRTdllBackupFromList;

_BRTdllRestore *pBRTdllRestore;

_BRTdllRestoreWithCheck *pBRTdllRestoreWithCheck;

_BRTdllGetLastError *pBRTdllGetLastError;

_BRTdllSetLogFile *pBRTdllSetLogFile;


