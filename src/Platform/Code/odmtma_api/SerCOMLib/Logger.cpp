#include "stdafx.h"
#include <TCHAR.H>
#include <CRTDBG.H>
#include <stdio.h>
#include "Logger.h"

#define LOG_FILE_NAME_LENGTH	100
#define LOG_MESSAGE_LENGTH		4096
#define ERROR_MODE_LENGTH		6
#define HIGH_MASK				0xF0
#define LOW_MASK				0x0F
#define MAX_PACKET_LEN			0x1000
#define MAX_PACKET_LOG_BUF_LEN	((MAX_PACKET_LEN*3)+1)

HANDLE	hLogFile = NULL;
int		nLogLevel = LOG_LEVEL_ERROR;
DWORD	dwAppendOption = 0;	//Append file or create a new file.

////////////////////////////////////////////////////////////////////////////
/*************************************************************************
 * Purpose: Get Registry value and start logging process.
 * Input  : const TCHAR *strRegistry: location of logger setting in registry
 * Return : >0 run succesfully.
 *			< 0  error, please refer to logger.h for complete error message.
 * Exception: None
 * Note	  : 2004/08/9 first version created by Embert
 *			This procedure is not THREAD-SAFE.
 *			2004/10/06 Embert Tsai
 *			Add append option to logger.
 *			20060615 Embert Tsai
 *			Add one log level LOG_LEVEL_NONE for no file generation.
 **************************************************************************/
////////////////////////////////////////////////////////////////////////////
int  StartLogger(LPCTSTR strRegistryKeyPath){

	_ASSERTE( strRegistryKeyPath != NULL );
	_ASSERTE( hLogFile == NULL );    
	
	DWORD 	dwReturnValueType=0;
    DWORD 	dwReturnValueSize=0;
	SYSTEMTIME	SysTime;
	TCHAR	szFileTemp[LOG_FILE_NAME_LENGTH];
	TCHAR szLogFileName[LOG_FILE_NAME_LENGTH];
    HKEY 	hKey = NULL;

	ZeroMemory((PVOID) szLogFileName, LOG_FILE_NAME_LENGTH * sizeof(TCHAR));
	ZeroMemory((PVOID) szFileTemp , LOG_FILE_NAME_LENGTH * sizeof(TCHAR));
	ZeroMemory((PVOID) &SysTime, sizeof(SysTime));
    
    // Open Registry key
	if ( RegOpenKey(HKEY_LOCAL_MACHINE, strRegistryKeyPath, &hKey) != ERROR_SUCCESS ) {
		return CLogger::ERROR_CLOGGER_REGISTRY_ERROR;
	}

	// Read key value
    if (hKey) {
        // Get Log Level
		dwReturnValueSize = sizeof(DWORD);
        RegQueryValueEx(hKey, _T("Level"), NULL, &dwReturnValueType,(LPBYTE)&nLogLevel, &dwReturnValueSize);
		_ASSERTE(nLogLevel <= LOG_LEVEL_DEBUG && nLogLevel >= LOG_LEVEL_NONE );
		if (nLogLevel == LOG_LEVEL_NONE){ //No log file needed.
			RegCloseKey(hKey);
			return RUN_SUCCESSFULLY;
		}
	    // Get Append Option
		dwReturnValueSize = sizeof(DWORD);
        RegQueryValueEx(hKey, _T("AppendOption"), NULL, &dwReturnValueType,(LPBYTE)&dwAppendOption, &dwReturnValueSize);
		// Get Log file name
		dwReturnValueSize = LOG_FILE_NAME_LENGTH * sizeof(TCHAR);
		RegQueryValueEx(hKey, _T("App"), NULL, &dwReturnValueType, (LPBYTE)szFileTemp, &dwReturnValueSize); 
		RegCloseKey(hKey);
    }
	
	// Get local time
	GetLocalTime(&SysTime);
	// Get system time for log file name
	// prepare file name
	// file name format: App040809.log
    wsprintf(szLogFileName, _T("%s%04d%02d%02d.log"), szFileTemp, SysTime.wYear, SysTime.wMonth, SysTime.wDay);

    if ( dwAppendOption ){
		// Append the log to file end.
		hLogFile = (HANDLE)CreateFile((LPCTSTR)szLogFileName,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_ALWAYS,0,NULL);
	} else {
		// Create and replace old file.
		hLogFile = (HANDLE)CreateFile((LPCTSTR)szLogFileName,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_WRITE|FILE_SHARE_READ,NULL,CREATE_ALWAYS,0,NULL);	
	}
	
    if (!hLogFile) 
		return CLogger::ERROR_CLOGGER_FILE_ERROR;
    else {
		// Move to File End
		SetFilePointer(hLogFile, 0, NULL, FILE_END);
        LogInformation(LOG_LEVEL_NORMAL,_T("*******************************************************************************"));
        LogInformation(LOG_LEVEL_NORMAL,_T("*... Start ..."));
        return TRUE;
    }
    
	return RUN_SUCCESSFULLY;
}

////////////////////////////////////////////////////////////////////////////
/*************************************************************************
 * Purpose: Log information into log file.
 * Input  : int nShowLevel : Error Level, please refer to Logger.h
 *			LPCTSTR format ....: just like printf argument list.
 * Return : >0 run succesfully.
 *			<0  error, please refer to logger.h for complete error message.
 * Exception: None
 * Note	  : 2004/08/9 first version created by Embert
 *			This procedure is THREAD-SAFE.
 *			2004/10/06 Embert Tsai
 *			Change wsprintf to _sntprintf to overcome buffer overflow problem.
 *			Change flush buffer behavior to increase logger performance.
 *			2005/01/25 Embert Tsai
 *			Change for larget packet.
 *			2005/08/04
 *			Change for complie warning.
 **************************************************************************/
////////////////////////////////////////////////////////////////////////////
int LogInformation(int nShowLevel, LPCTSTR format,...)
{
    if (nShowLevel > nLogLevel || hLogFile == NULL) 
		return CLogger::OPERATION_NONE ; // Don't need to output

	va_list		marker;
    DWORD		dwOut = 0 ;
    SYSTEMTIME	SysTime;
    TCHAR		*szMsg = NULL;
	TCHAR		*szFormat = NULL;
	TCHAR		szErrorMode[ERROR_MODE_LENGTH];

	// variable initialization
	szMsg = new TCHAR[MAX_PACKET_LOG_BUF_LEN+LOG_MESSAGE_LENGTH];
	szFormat = new TCHAR[MAX_PACKET_LOG_BUF_LEN+LOG_MESSAGE_LENGTH];
	ZeroMemory(&SysTime, sizeof(SysTime));
	ZeroMemory(szMsg, (MAX_PACKET_LOG_BUF_LEN+LOG_MESSAGE_LENGTH) * sizeof(TCHAR));
	ZeroMemory(szFormat, (MAX_PACKET_LOG_BUF_LEN+LOG_MESSAGE_LENGTH) * sizeof(TCHAR));
	ZeroMemory(szErrorMode, ERROR_MODE_LENGTH * sizeof(TCHAR));
	
    if (hLogFile) {

		switch (nShowLevel){
		case LOG_LEVEL_DEBUG:
			_tcscpy(szErrorMode, _T("<DBG>"));
			break;
		case LOG_LEVEL_WARNING:
			_tcscpy(szErrorMode, _T("<WRN>"));
			break;
		case LOG_LEVEL_ERROR:
			_tcscpy(szErrorMode, _T("<!ER>"));
			break;
		case LOG_LEVEL_NORMAL:
			_tcscpy(szErrorMode, _T("<NML>"));
			break;
		default:
			_ASSERTE(FALSE);
		}

        GetLocalTime(&SysTime);
        va_start(marker, format);
        _vstprintf(szFormat,format,marker);
        va_end(marker);
        _sntprintf(szMsg, LOG_MESSAGE_LENGTH * sizeof(TCHAR), _T("%s\t%04d-%02d-%02d %02d:%02d:%02d:%03d\t%s\r\n"),szErrorMode,SysTime.wYear,SysTime.wMonth,SysTime.wDay,SysTime.wHour,SysTime.wMinute,SysTime.wSecond,SysTime.wMilliseconds,szFormat);
        WriteFile(hLogFile,szMsg,_tcslen(szMsg),&dwOut,0L);
		if ( nShowLevel <=  LOG_LEVEL_ERROR ){ // when error happen, force to flush buffer
			FlushFileBuffers(hLogFile);
		}
		// 20050125 Embert Tsai. Change for large packet
		delete[] szMsg;
		delete[] szFormat;
		szMsg = NULL;
		szFormat = NULL;
		
   } 
	return RUN_SUCCESSFULLY;
}

////////////////////////////////////////////////////////////////////////////
/*************************************************************************
 * Purpose: Log system error number information into log file.
 * Input  : DWORD nShowLevel	: Error Level, please refer to Logger.h
 *			void *vpBuf	: buffer need to be logged.
 *			int nLen	: Buffer length.
 * Return : >0 run succesfully.
 *			<0  error, please refer to logger.h for complete error message.
 * Exception: None
 * Note	  : 2004/09/16 first version created by Embert
 *			This procedure is THREAD-SAFE.
 *			2004/09/23 Change source buffer type to const void *.
 *			2004/10/06 Embert Tsai
 *			The following program is not unicode safe. Need to be modified.
 **************************************************************************/
////////////////////////////////////////////////////////////////////////////
 int  LogBuffer(int nShowLevel, const void *pvBuf, int nLen){

	_ASSERTE( pvBuf != NULL );
	_ASSERTE( nLen > 0 );

    if (nShowLevel > nLogLevel || hLogFile == NULL) 
		return CLogger::OPERATION_NONE ; // Don't need to output

	unsigned char *szTarget = NULL;
	szTarget = new unsigned char[nLen*3+1];
	ZeroMemory(szTarget, nLen*3+1);
	if ( nLen > MAX_PACKET_LEN){
		nLen = MAX_PACKET_LEN;
	}
	ConvertBufferToString(pvBuf, nLen, szTarget);
	LogInformation(nShowLevel,_T("The %d bytes buffer content is: %s"), nLen, szTarget);
	delete[] szTarget;
	szTarget = NULL;
	return RUN_SUCCESSFULLY;
}

////////////////////////////////////////////////////////////////////////////
/*************************************************************************
 * Purpose: Convert memory content to hexadecimal readable string.
 * Input  : void *vpBuf	: buffer need to be logged.
 *			int nLen	: Buffer length.
 *			unsigned char *szTarget: Target output buffer.
 *			szTarget format will look like as 'FF 0A 3F'.....
 * Return : >0 run succesfully.
 *			<0  error, please refer to logger.h for complete error message.
 * Exception: None
 * Note	  : 2004/09/16 first version created by Embert
 *			This procedure is THREAD-SAFE.
 *			2004/09/23 Change source buffer type to const void *.
 *			2004/10/06 Embert Tsai
 *			The following function is not unicode safe, need to changed.			
 **************************************************************************/
////////////////////////////////////////////////////////////////////////////
int  ConvertBufferToString(const void *pvBuf, int nLen ,unsigned char *szTarget){

	_ASSERTE( pvBuf != NULL );
	_ASSERTE( nLen > 0 );
	_ASSERTE( szTarget != NULL );

	BYTE *pbSource = (BYTE *) pvBuf;
	BYTE bTemp = 0;

	for ( int i=0 ; i < nLen ; i++ ){
		// Convert the first 4 highest bit first
		bTemp = (*(pbSource+i)) & HIGH_MASK ;
		bTemp >>= 4 ;
		if ( bTemp < 10 ){
			bTemp += 48;	// convert to ASCII number character
		}else {
			bTemp += 55;	// convert to ASCII 'ABCDEF'
		}
		*(szTarget + (i*3) ) = bTemp;
		
		// Convert the other bits
		bTemp = (*(pbSource+i)) & LOW_MASK ;
		if ( bTemp < 10 ){
			bTemp += 48;	// convert to ASCII number character
		}else {
			bTemp += 55;	// convert to ASCII 'ABCDEF'
		}
		*(szTarget + (i*3) + 1 ) = bTemp;
		
		*(szTarget + (i*3) + 2 ) = ' ';

	}
	return RUN_SUCCESSFULLY;
}

////////////////////////////////////////////////////////////////////////////
/*************************************************************************
 * Purpose: Log system error information.
 * Input  : DWORD dwLastError : Error number gotten from GetLastError().
 * Return : >0 run succesfully.
 *			<0  error, please refer to logger.h for complete error message.
 * Exception: None
 * Note	  : 2004/08/9 first version created by Embert
 *			This procedure is THREAD-SAFE.
 **************************************************************************/
////////////////////////////////////////////////////////////////////////////
 int  LogSystemError(DWORD dwLastError){

	// This function is borrowed from MSDN

    if (hLogFile == NULL) 
		return CLogger::OPERATION_NONE ; // Don't need to output

    HMODULE hModule = NULL; // default to system source
    LPSTR MessageBuffer;
    DWORD dwBufferLength;

    DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_IGNORE_INSERTS |
        FORMAT_MESSAGE_FROM_SYSTEM ;

    //
    // Call FormatMessage() to allow for message 
    //  text to be acquired from the system 
    //  or from the supplied module handle.
    //
	if (hLogFile) { 

		if(dwBufferLength = FormatMessageA(
								dwFormatFlags,
								NULL, // module to get message from (NULL == system)
								dwLastError,
								MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
								(LPSTR) &MessageBuffer,
								0,
								NULL
								))
		{
		
			// Fist write error message header for furthur processing.
			LogInformation(LOG_LEVEL_ERROR,_T("The system error number [%d] --> %s"), dwLastError, MessageBuffer);
			LocalFree(MessageBuffer);
		}
	}

	return RUN_SUCCESSFULLY;
}
////////////////////////////////////////////////////////////////////////////
/*************************************************************************
 * Purpose: Stop logging process and release system resource.
 * Input  : NONE
 * Return : >0 run succesfully.
 *			<0  error, please refer to logger.h for complete error message.
 * Exception: None
 * Note	  : 2004/08/9 first version created by Embert
 *			This procedure is NOT THREAD-SAFE.
 **************************************************************************/
////////////////////////////////////////////////////////////////////////////
int  StopLogger(){

   if (hLogFile) { 
	   LogInformation(LOG_LEVEL_NORMAL,_T("*...End...."));
       LogInformation(LOG_LEVEL_NORMAL,_T("*******************************************************************************"));
       CloseHandle(hLogFile); hLogFile=NULL; 
    }    
	
	return RUN_SUCCESSFULLY;
}

int  LogFunctionInformation(int nShowLevel, LPCTSTR szFuncName, LPCTSTR format,...){

    if (nShowLevel > nLogLevel || hLogFile == NULL) 
		return CLogger::OPERATION_NONE ; // Don't need to output

	va_list		marker;
    DWORD		dwOut = 0 ;
    SYSTEMTIME	SysTime;
    TCHAR		szMsg[LOG_MESSAGE_LENGTH]=_T(""),szFormat[LOG_MESSAGE_LENGTH]=_T("");
	TCHAR		szErrorMode[ERROR_MODE_LENGTH] = _T("");

	// variable initialization
	ZeroMemory(&SysTime, sizeof(SysTime));
	ZeroMemory(szMsg, LOG_MESSAGE_LENGTH * sizeof(TCHAR));
	ZeroMemory(szFormat, LOG_MESSAGE_LENGTH * sizeof(TCHAR));
	ZeroMemory(szErrorMode, ERROR_MODE_LENGTH * sizeof(TCHAR));
	
    if (hLogFile) {

		switch (nShowLevel){
		case LOG_LEVEL_DEBUG:
			_tcscpy(szErrorMode, _T("<DBG>"));
			break;
		case LOG_LEVEL_WARNING:
			_tcscpy(szErrorMode, _T("<WRN>"));
			break;
		case LOG_LEVEL_ERROR:
			_tcscpy(szErrorMode, _T("<!ER>"));
			break;
		case LOG_LEVEL_NORMAL:
			_tcscpy(szErrorMode, _T("<NML>"));
			break;
		default:
			_ASSERTE(FALSE);
		}

        GetLocalTime(&SysTime);
        va_start(marker, format);
        _vstprintf(szFormat,format,marker);
        va_end(marker);
        _sntprintf(szMsg, LOG_MESSAGE_LENGTH * sizeof(TCHAR), _T("%s\t%04d-%02d-%02d %02d:%02d:%02d:%03d\t%s\t %s\r\n"),szErrorMode,SysTime.wYear,SysTime.wMonth,SysTime.wDay,SysTime.wHour,SysTime.wMinute,SysTime.wSecond,SysTime.wMilliseconds,szFuncName,szFormat);
        WriteFile(hLogFile,szMsg,_tcslen(szMsg),&dwOut,0L);
		if ( nShowLevel <=  LOG_LEVEL_ERROR ){ // when error happen, force to flush buffer
			FlushFileBuffers(hLogFile);
		}

   } 
	return RUN_SUCCESSFULLY;
}

int LogCOMFuncSystemError(LPCTSTR szCOM, LPCTSTR szFunc,DWORD dwLastError){
	
    if (hLogFile == NULL) 
		return CLogger::OPERATION_NONE ; // Don't need to output

	// This function is borrowed from MSDN

	HMODULE hModule = NULL; // default to system source
    LPSTR MessageBuffer;
    DWORD dwBufferLength;

    DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_IGNORE_INSERTS |
        FORMAT_MESSAGE_FROM_SYSTEM ;

    //
    // Call FormatMessage() to allow for message 
    //  text to be acquired from the system 
    //  or from the supplied module handle.
    //
	if (hLogFile) { 

		if(dwBufferLength = FormatMessageA(
								dwFormatFlags,
								NULL, // module to get message from (NULL == system)
								dwLastError,
								MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
								(LPSTR) &MessageBuffer,
								0,
								NULL
								))
		{
		
			// Fist write error message header for furthur processing.
			LogInformation(LOG_LEVEL_ERROR,_T("%s\t%s\tThe system error number [%d] --> %s"), szCOM, szFunc, dwLastError, MessageBuffer);
			LocalFree(MessageBuffer);
		}
	}

	return RUN_SUCCESSFULLY;

}

int LogCOMFuncInfo(int nShowLevel, LPCTSTR szCOM, LPCTSTR szFunc,  LPCTSTR format,...){

    if (nShowLevel > nLogLevel || hLogFile == NULL) 
		return CLogger::OPERATION_NONE ; // Don't need to output

	va_list		marker;
    DWORD		dwOut = 0 ;
    SYSTEMTIME	SysTime;
    TCHAR		szMsg[LOG_MESSAGE_LENGTH]=_T(""),szFormat[LOG_MESSAGE_LENGTH]=_T("");
	TCHAR		szErrorMode[ERROR_MODE_LENGTH] = _T("");

	// variable initialization
	ZeroMemory(&SysTime, sizeof(SysTime));
	ZeroMemory(szMsg, LOG_MESSAGE_LENGTH * sizeof(TCHAR));
	ZeroMemory(szFormat, LOG_MESSAGE_LENGTH * sizeof(TCHAR));
	ZeroMemory(szErrorMode, ERROR_MODE_LENGTH * sizeof(TCHAR));
	
    if (hLogFile) {

		switch (nShowLevel){
		case LOG_LEVEL_DEBUG:
			_tcscpy(szErrorMode, _T("<DBG>"));
			break;
		case LOG_LEVEL_WARNING:
			_tcscpy(szErrorMode, _T("<WRN>"));
			break;
		case LOG_LEVEL_ERROR:
			_tcscpy(szErrorMode, _T("<!ER>"));
			break;
		case LOG_LEVEL_NORMAL:
			_tcscpy(szErrorMode, _T("<NML>"));
			break;
		default:
			_ASSERTE(FALSE);
		}

        GetLocalTime(&SysTime);
        va_start(marker, format);
        _vstprintf(szFormat,format,marker);
        va_end(marker);
        _sntprintf(szMsg, LOG_MESSAGE_LENGTH * sizeof(TCHAR), _T("%s\t%04d-%02d-%02d %02d:%02d:%02d:%03d\t%s\t%s\t%s\r\n"),szErrorMode,SysTime.wYear,SysTime.wMonth,SysTime.wDay,SysTime.wHour,SysTime.wMinute,SysTime.wSecond,SysTime.wMilliseconds,szCOM,szFunc,szFormat);
        WriteFile(hLogFile,szMsg,_tcslen(szMsg),&dwOut,0L);
		if ( nShowLevel <=  LOG_LEVEL_ERROR ){ // when error happen, force to flush buffer
			FlushFileBuffers(hLogFile);
		}

   } 
	return RUN_SUCCESSFULLY;

}

int LogCOMFuncBuffer(int nShowLevel, LPCTSTR szCOM, LPCTSTR szFunc, const void *pvBuf, int nLen){

    if ( hLogFile == NULL) 
		return CLogger::OPERATION_NONE ; // Don't need to output

	_ASSERTE( pvBuf != NULL );
	_ASSERTE( nLen > 0 );

	LPTSTR szTarget = NULL;

	if ( nLen > MAX_PACKET_LEN){
		nLen = MAX_PACKET_LEN;
	}
	szTarget = new TCHAR[nLen*3+1];
	ZeroMemory(szTarget, nLen*3+1);
	ConvertBufferToTString(pvBuf, nLen, szTarget);
	LogInformation(nShowLevel,_T("%s\t%s\tThe %d bytes buffer content is: %s"), szCOM, szFunc,nLen,szTarget);
	delete[] szTarget;
	szTarget = NULL;
	return RUN_SUCCESSFULLY;

}

////////////////////////////////////////////////////////////////////////////
/*************************************************************************
 * Purpose: Convert memory content to hexadecimal readable unicode compatible string.
 * Input  : void *vpBuf	: buffer need to be logged.
 *			int nLen	: Buffer length.
 *			LPTSTR szTarget: Target output buffer.
 *			szTarget format will look like as 'FF 0A 3F'.....
 * Return : >0 run succesfully.
 *			<0  error, please refer to logger.h for complete error message.
 * Exception: None
 * Note	  : 2005/05/25 Embert Tsai.
 *			This procedure is THREAD-SAFE.
 *			The following function is not unicode safe, need to changed.			
 **************************************************************************/
////////////////////////////////////////////////////////////////////////////
int  ConvertBufferToTString(const void *pvBuf, int nLen ,LPTSTR szTarget){

	_ASSERTE( pvBuf != NULL );
	_ASSERTE( nLen > 0 );
	_ASSERTE( szTarget != NULL );

	BYTE *pbSource = (BYTE *) pvBuf;

	for ( int i=0 ; i < nLen ; i++ ){
		_stprintf(szTarget+i*3, _T("%02X "),*(pbSource+i) );
	}
	return RUN_SUCCESSFULLY;
}
