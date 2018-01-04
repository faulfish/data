#if !defined(LOGGER_H__3D46E606_32E3_47C4_9053_68ECEEF82D74__INCLUDED_)
#define LOGGER_H__3D46E606_32E3_47C4_9053_68ECEEF82D74__INCLUDED_

//#include "stdafx.h"
#include <windows.h>
//20040914 Embert Tsai
// In order to prevent EXPORT duplicate definition

#define RUN_SUCCESSFULLY	1
#define RUN_FAILED			(-1)
#define ERROR_MASK			0xF0000000

#define LOG_LEVEL_DEBUG		5
#define LOG_LEVEL_WARNING	3
#define	LOG_LEVEL_ERROR		1
#define LOG_LEVEL_NORMAL	0
#define LOG_LEVEL_NONE		(-1)

#define WEIRD_BYTE			0xA3
#define WEIRD_WORD			0xA3A3
#define WEIRD_DWORD			0xA3A3A3A3

// class for error code definition.
class CLogger{
	public:
		enum ErrorType{
			ERROR_CLOGGER_REGISTRY_ERROR	=	ERROR_MASK | 0x00000001,
			ERROR_CLOGGER_FILE_ERROR		=	ERROR_MASK | 0x00000002
		}; 
		enum OperationType{
			OPERATION_NONE		=	0
		};
};


int  StartLogger(LPCTSTR RegistryKey);
int  StopLogger();
int  LogSystemError(DWORD dwLastError);
int  LogInformation(int dwShowLevel, LPCTSTR format,...);
int  LogFunctionInformation(int dwShowLevel, LPCTSTR szFuncName, LPCTSTR format,...);
int  LogBuffer(int wShowLevel, const void *pvBuf, int nLen);
int  ConvertBufferToString(const void *pvBuf, int nLen, unsigned char *szTarget);
int  ConvertBufferToTString(const void *pvbf, int nLen, LPTSTR szTarget);

// 2004/10/30 Embert Tsai
// The following function is just for COM port specific and debug ready
// can be load into EXCEL for detail trace.
int LogCOMFuncSystemError(LPCTSTR szCOM, LPCTSTR szFunc,DWORD dwLastError);
int LogCOMFuncInfo(int dwShowLevel, LPCTSTR szCOM, LPCTSTR szFunc,  LPCTSTR format,...);
int LogCOMFuncBuffer(int wShowLevel, LPCTSTR szCOM, LPCTSTR szFunc, const void *pvBuf, int nLen);

#endif