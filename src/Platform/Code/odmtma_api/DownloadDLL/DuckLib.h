/* duck.h */
/* exported functions */
#if !defined(_DUCK_H)
#define _DUCK_H

#include "windows.h"

#define PLATFORM_6250

#ifdef DUCK_EXPORTS
#define DUCK_API __declspec(dllexport)
#else
#define DUCK_API __declspec(dllimport)
#endif

#if defined(DUCK_STATIC)
#undef DUCK_API
#define DUCK_API
//#define DUCK_API __cdecl
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*you must use this function in your aplication before RunDuckLib() function
	bUseQPST = true to use QPST, false to use QPHONEMS*/
 DUCK_API void SetDuckLibMode( unsigned char bUseQPST /*= true*/);

/*you must use this function on your aplication start-up before another DUCK DLL's function, set iEmerg to 1 if you want connect with phone without asking if phone is connected, use it for CEFSEmerg
	returning values:
	0 - if ok
	1 - if fail*/
 DUCK_API int RunDuckLib( const char* strPhysicalPort, int iEmerg /*= 0*/);

/*you must use this function on your aplication start-up before another DUCK DLL's function, set iEmerg to 1 if you want connect with phone without asking if phone is connected, use it for CEFSEmerg
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int RunDuckLibUsingHandler( HANDLE hResourceContext, int iEmerg /*= 0*/ );

/*you must use this function on your aplication exit*/
DUCK_API void ShutdownDuckLib(void);


/*set call back functions*/
DUCK_API void Duck_SetCallBack( void (*CallBackPRG)(int iRng, int iPos ), void (*CallBackMSG)( char* strMsg ) );

/*restart phone
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_RestartPhone( int iWaitForPhone );


/*abort current operation*/
DUCK_API void Duck_AbortOperation(void);

/*abort current operation*/
DUCK_API void Duck_ResetAbortFlag(void);

/*conversion and save results in file
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_NVConvert( char* strPlainIniFile, char* strConversionScript, char* strDuckFile );

/*conversion and download results to phone
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_NVConvertAndDownload( char* strPlainIniFile, char* strConversionScript );

/*conversion, save results in file and download results to phone
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_NVConvertAndSaveAndDownload( char* strPlainIniFile, char* strConversionScript, char* strDuckFile );

/*conversion (ConversionScript is in zip file) and save results in file
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_NVConvertNVZipAndSave( char* strPlainIniFile, char* strNVZipFile, char* strWorkingDirectory, char* strDuckFile );

/*conversion (ConversionScript is in zip file) and download results to phone
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_NVConvertNVZipAndDownload( char* strPlainIniFile, char* strNVZipFile, char* strWorkingDirectory );

/*conversion (ConversionScript is in zip file), save results in file and download results to phone
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_NVConvertNVZipAndSaveAndDownload( char* strPlainIniFile, char* strNVZipFile, char* strWorkingDirectory, char* strDuckFile );

/*download duck file to phone
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_NVDownload( char* strDuckFile );


/*download EFS.zip file to phone
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_EFSDownload( char* strEFSArchiveFile, char* strWorkingDirectory);

/*conversion, download results to phone and download EFS.zip to phone
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_CompleteDownload( char* strPlainIniFile, char* strConversionScript, char* strEFSArchiveFile, char* strWorkingDirectory) ;

/*download duck file and EFS.zip file to phone
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_CompleteDownload_DuckAndEFS( char* strDuckFile, char* strEFSArchiveFile, char* strWorkingDirectory) ;


/*upload results from phone to file
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_NVUpload( char* strPlainIniFile, char* strConversionScript, char* strNVContentIniFile);

/*upload results from phone to file (ConversionScript is in zip file)
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_UploadNVZip( char* strPlainIniFile, char* strNVZipFile, char* strWorkingDirectory, char* strNVContentIniFile );


/*download CEFS file to phone, set iNVList to 1 if you want backup/restore NVItems with MSM6250_device_specific_data.csv file, set iWaitForPhone to 0 if you dont want to wait for phone after CEFS download
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_CEFSDownload(char *strFileName, char* strPartitionFileName, int iNVList /*= 0*/, char * chSPCCode /* "\0" */);

/*download CEFS file to phone, use this function when your phone crashed during CEFSDownload, to use this function you must set iEmerg to 1 in RunDuckLib (not working yet, not supported in QPST)
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_CEFSEmerg(char *strFileName, char* strPartitionFileName);


/*conversion and save results in temporary array
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_CmdConvertPlainIni( char* strPlainIniFile, char* strConversionScript );

/*conversion and save results in temporary array
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_CmdConvertNVZip( char* strPlainIniFile, char* strNVZipFile, char* strWorkingDirectory );

/*download temporary array to mobile
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_NV_Array2Mobile( void );

/*save temporary array to file
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_NV_Array2DuckFile(  char* strDuckFile );

/*write data from file to temporary array
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_DuckFile2NV_Array(  char* strDuckFile ); 

/*return last error number
	returning values:
	error number
	strLastError - error message*/
DUCK_API int Duck_GetLastError(char** strLastError);
/*to reset last error number*/
DUCK_API void Duck_SetLastError(void);

/*return last message*/
DUCK_API char* Duck_GetLastMessage(void);

/*return version*/
DUCK_API char* Duck_GetLibVersion(void);

/*return phone status
	returning values:
	0 - if phone disconnected
	1 - if phone connected*/
DUCK_API int Duck_GetPhoneStatus(void);

/*to reset phone status - EMPTY - NOT USED FROM 1.3.0 version */
DUCK_API void Duck_SetNoPhone(void);

/*return current progress range in current operation*/
DUCK_API int Duck_GetProgressBarRange(void);
/*return current progress position in current operation*/
DUCK_API int Duck_GetProgressBarPosition(void);

/*return free space on phone file system in kbytes*/
DUCK_API unsigned long Duck_EFSGetFreeSpace( void );

/*return version of phone 
	returning values:
	0 - if ok
	1 - if fail
	strSWVersion - software version*/
DUCK_API int Duck_PhoneVersionCheck( char** strSWVersion );


/*restore NV Items with QCN file 
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_RestoreQCN(  char* strQCNFile, int bSkipReset /*= 0*/ , char * chSPCCode /* "\0" */); 

/*restore NV Items with BRT file 
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_RestoreBRT(  char* strBRTFile, char * chSPCCode /* "\0" */ ); 

/*set iEmerg to 1 if you want connect with phone without asking if phone is connected, use it for CEFSEmerg (not working yet, not supported in QPST)
	returning values:
	0 - if ok
	1 - if fail*/
DUCK_API int Duck_Connect(const char* strPhysicalPort, int iEmerg /*= 0*/);

/*disconnect phone*/
DUCK_API void Duck_DisConnect( void );

#ifdef __cplusplus
}	/* extern "C" */
#endif 

#endif	/* defined(_DUCK_H) */
