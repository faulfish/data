#ifndef __MOTOROLA_ODM_API_H
#define __MOTOROLA_ODM_API_H

#define __VBDECLARATION     extern "C" _declspec(dllexport) int __stdcall

#define TPTK_SUCCESS 0
#define TPTK_NOTIMPLEMENTED -1
#define TPTK_FAIL    0x01
#define FAIL_SETTESTMODE_INFLASHMODE -2
#define FAIL_DISCONNECT -3
#define GET_POINTER 0
#define SET_POINTER 1

#define ODM_INITIALIZE_ERROR				0x01
#define ODM_UNINITIALIZE_ERROR				0x02
#define ODM_CLEARERRORMESSAGE_ERROR			0x03
#define ODM_CLEARFILEPATH_ERROR				0x04
#define ODM_CHECKDATASIZE_ERROR				0x05
#define ODM_ADDPORTTOSTRING_ERROR			0x06
#define ODM_APIVERSION_ERROR				0x07
#define ODM_CONNECTCOMMPORT_ERROR			0x08
#define ODM_DISCONNECTCOMMPORT_ERROR		0x09
#define ODM_ISUNITON_ERROR					0x0A
#define ODM_TURNUNITON_ERROR				0x0B
#define ODM_ISUNITOFF_ERROR					0x0C
#define ODM_TURNUNITOFF_ERROR				0x0D
#define ODM_RESTARTUNIT_ERROR				0x0E
#define ODM_READBATTERYLEVEL_ERROR			0x0F
#define ODM_SETTESTMODE_ERROR				0x10
#define ODM_ISPROGRAMMINGCOMPLETE_ERROR 	0x11
#define ODM_MASTERCLEAR_ERROR				0x12
#define ODM_READTRACKID_ERROR				0x13
#define ODM_READBARCODE_ERROR				0x14
#define ODM_READTRANSCEIVERKITNUMBER_ERROR	0x15
#define ODM_READFACTORYINFOBLOCK_ERROR		0x16
#define ODM_SETFACTORYINFOBLOCK_ERROR		0x17
#define ODM_SETINFACTORYFLAG_ERROR			0x18
#define ODM_FACTORYFLEXSWITCH_ERROR			0x19
#define ODM_READUNITINFO_ERROR				0x1A
#define ODM_PROGRAMIMEI_ERROR				0x1B
#define ODM_READIMEI_ERROR					0x1C
#define ODM_VERIFYIMEI_ERROR				0x1D
#define ODM_PROGRAMESN_ERROR				0x1E
#define ODM_READESN_ERROR					0x1F
#define ODM_VERIFYESN_ERROR					0x20
#define ODM_COMPAREFLASH_ERROR				0x21
#define ODM_COMPAREFLEX_ERROR				0x22
#define ODM_PROGRAMPHONEBOOK_ERROR			0x23
#define ODM_READVERSION_ERROR				0x24
#define ODM_PROGRAMBLUETOOTH_ERROR			0x25
#define ODM_READBLUETOOTH_ERROR				0x26
#define ODM_PROGRAMWLAN_ERROR				0x27
#define ODM_READWLAN_ERROR					0x28
#define ODM_SETREALTIMECLOCK_ERROR			0x29
#define ODM_ISUNITLOCKED_ERROR				0x2A
#define ODM_UNLOCKUNIT_ERROR				0x2B
#define ODM_PROGRAMLOCK_ERROR				0x2C
#define ODM_READLOCK_ERROR					0x2D
#define ODM_PROGRAMAKEY_ERROR				0x2E
#define ODM_PROGRAMMOBILEID_ERROR			0x2F
#define ODM_PROGRAMSYSTEMID_ERROR			0x30
#define ODM_PROGRAMSOC_ERROR				0x31
#define ODM_PROGRAMIMSI_ERROR				0x32
#define ODM_GENERICWRITE_ERROR				0x33
#define ODM_GENERICREAD_ERROR				0x34
#define ODM_SETEFEMOFF_ERROR				0x35
#define ODM_SETEFEMON_ERROR					0x36
#define ODM_EXECUTEFLOW_ERROR				0x37
#define ODM_EXECUTEFLOW_SIMPLE_ERROR		0x38
#define ODM_EXECUTEFLOW_GENERALLY_ERROR		0x39
#define ODM_EXECUTEFLOW_COMPLETE_ERROR		0x3A
#define ODM_FLASH_ERROR						0x60
#define ODM_FLEX_ERROR						0x70




// Utility Functions
__VBDECLARATION  ODM_Initialize( char * szInitializeParameters, char * szErrMsg500);
__VBDECLARATION	 ODM_ClearErrorMessage(char * szErrMsg500);
__VBDECLARATION  ODM_ClearFilePath(char * szFilePath );
__VBDECLARATION  ODM_CheckDataSize(long nOdmLength, long &nMotoLen, char * szDataType, char * szErrMsg500); //x
__VBDECLARATION	 ODM_AddPortToString(char * szPortszErrMsg500 , char * szPortName, char *szErrMsg500 );
__VBDECLARATION  ODM_APIVersion( char * szApiVersion1024 , char *szErrMsg500 );
__VBDECLARATION  ODM_UnInitialize( char*  szErrMsg500 );

// Comm port connections
__VBDECLARATION ODM_ConnectCommPort( long &nPort, char * szPortName, char * szErrMsg500 );
__VBDECLARATION ODM_DisConnectCommPort( long &nPort, char * szPortName, char * szErrMsg500 );

// Unit power up, down and restart
__VBDECLARATION ODM_IsUnitOn( long nPort, long& nUnitOnStatus, char * szErrMsg500 );
__VBDECLARATION ODM_TurnUnitOn( long nPort, long nSleepTime, long nAttempts, char * szErrMsg500 );
__VBDECLARATION ODM_IsUnitOff( long nPort, long &nUnitOffStatus, char * szErrMsg500 );
__VBDECLARATION ODM_TurnUnitOff( long nPort, char * szErrMsg500 );
__VBDECLARATION ODM_RestartUnit( long nPort, char * szErrMsg500 );

// Test Mode, Status Check and Master Clear
__VBDECLARATION ODM_SetTestMode( long nPort, char * szErrMsg500 );
__VBDECLARATION ODM_IsProgrammingComplete(long nPort,  long &nCompleteStatus, char * szErrMsg500); //x
__VBDECLARATION ODM_MasterClear( long nPort,  char * szErrMsg500 );

// Unit Processing information
__VBDECLARATION ODM_ReadTrackId( long nPort,  char * szTrackId100, char * szErrMsg500 );
__VBDECLARATION ODM_ReadBarcode( long nPort,  char * szBarcode100, char * szErrMsg500 );
__VBDECLARATION ODM_ReadTransceiverKitNumber(long nPort,  char * szTransceiverKitNumber100, char * szErrMsg500);
__VBDECLARATION ODM_ReadFactoryInfoBlock( long nPort, char * szFactoryInfoBlock1024, long &nLength, char * szErrMsg500);
__VBDECLARATION ODM_SetFactoryInfoBlock( long nPort, char * szFactoryInfoBlock, long nLength, char * szErrMsg500);
__VBDECLARATION ODM_SetInFactoryFlag(long nPort,  long nOnOff, char * szErrMsg500);
__VBDECLARATION ODM_FactoryFlexSwitch(long nPort,  long nToFactory, char * szErrMsg500); //x
__VBDECLARATION ODM_ReadUnitInfo( long nPort,  char * szUnitInfo1024, long &nLength, char * szErrMsg500 ); //x

// Device identification
__VBDECLARATION ODM_ProgramIMEI(long nPort, char * szIMEI, char * szErrMsg500);
__VBDECLARATION ODM_ReadIMEI( long nPort, char * szIMEI100, char * szErrMsg500 );
__VBDECLARATION ODM_VerifyIMEI(long nPort, char * szIMEI, char * szErrMsg500);
__VBDECLARATION ODM_ProgramESN(long nPort, char * szEsn, char * szErrMsg500); //x
__VBDECLARATION ODM_ReadESN( long nPort, char * szEsn100, char * szErrMsg500 );//x
__VBDECLARATION ODM_VerifyESN(long nPort, char * szEsn, char * szErrMsg500); //x

// Flash and Flex
__VBDECLARATION ODM_CompareFlash(long nPort,  char * szFlashFilePath, char * szFlashFileType, char * szErrMsg500);
__VBDECLARATION ODM_CompareFlex(long nPort,  char * szFlexFilePath, char * szFlexFileType, char * szErrMsg500);
__VBDECLARATION ODM_Flash(long nPort,  char * szFlashFilePath, char * szFlashFileType, char * szErrMsg500);
__VBDECLARATION ODM_Flex(long nPort,  char * szFlexFilePath, char * szFlexFileType, char * szErrMsg500);
__VBDECLARATION ODM_ProgramPhoneBook(long nPort,  char * szPhoneBookFilePath,  char * szErrMsg500); //x
__VBDECLARATION ODM_ReadVersion( long nPort, char * szValueType,  char * szReadValue1024, long &nLength, char * szErrMsg500 ); 

// Addresses
__VBDECLARATION ODM_ProgramBlueTooth(long nPort,  char * szAddress, char * szErrMsg500); //x
__VBDECLARATION ODM_ReadBlueTooth(long nPort,  char * szAddress100, long &nLength, char * szErrMsg500);
__VBDECLARATION ODM_ProgramWLAN(long nPort,  char * szAddress, char * szErrMsg500); //x
__VBDECLARATION ODM_ReadWLAN(long nPort,  char * szAddress100, long &nLength, char * szErrMsg500); //x

// Provisioning extras
__VBDECLARATION  ODM_SetRealTimeClock(long nPort,  char * szErrMsg500);

// Locks
__VBDECLARATION ODM_IsUnitLocked(long nPort,  char* szLockType, long &nLockStatus, char * szErrMsg500);
__VBDECLARATION ODM_UnLockUnit(long nPort,  char * szLock, char* szLockType , long &nLockStatus, char * szErrMsg500);
__VBDECLARATION ODM_ProgramLock(long nPort,  char * szLock, char* szLockType , char * szErrMsg500);
__VBDECLARATION ODM_ReadLock(long nPort,  char * szLock100, char* szLockType, char * szErrMsg500); //x
__VBDECLARATION ODM_ProgramAkey(long nPort,  char * szAkey, char* szAkeyType, char * szErrMsg500); //x
__VBDECLARATION ODM_ProgramMobileId(long nPort,  char * szMobileId, char* szIdType, char * szErrMsg500); //x
__VBDECLARATION ODM_ProgramSystemId(long nPort,  char * szSystemId, char* szIdType , char * szErrMsg500); //x
__VBDECLARATION ODM_ProgramSoc(long nPort,  char * szSoc, char* szSocType, char * szErrMsg500); //x
__VBDECLARATION ODM_ProgramIMSI(long nPort,  char * szValue, char* szType , char * szErrMsg500); //x

// ODM WorkAround

__VBDECLARATION ODM_GenericWrite( long nPort, char * szData, char * szTag, char * szErrMsg500 ); //x
__VBDECLARATION ODM_GenericRead( long nPort, long nDSize, char * szData1024, char * szTag,char * szErrMsg500); //x
__VBDECLARATION ODM_ExecuteFlow(long nPort, char *szFileName, char *szErrMsg500);
__VBDECLARATION ODM_ReadBatteryLevel(long nPort,  char * szLevel100,  char * szErrMsg500);


//__VBDECLARATION ODM_SetCallBackFunctionPointer(void * pObject, char * szFuntionName , void * pFn , char * szErrMsg500);
__VBDECLARATION ODM_SetCallBack(char* szFnName, FARPROC pFn, char * szErrMsg500 );

#endif