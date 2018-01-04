/*===============================================================================================
  Module Name        : PSTLogger.h
  General Description: Macro definition of the PST core logger
===================================================================================================
                                Motorola Confidential Proprietary
                           Advanced Technology and Software Operations
                         (c) Copyright Motorola 2004-2006, All Rights Reserved

Revision History:             

                                Modification       Tracking		            Description
Author                              Date            Number                 of Changes
-----------------             ----------------   -------------     ---------------------------       
1.  Dan Murphy					  09/16/2003		LIBbb41574		Initial Creation
2.  Dindo Jao                     10/14/2004        LIBff20639      Added new log level used for error.
3.  Dindo Jao                     02/14/2006        LIBhh93979      Change the level 8 logging error to avoid confusion   
4.  Samir Khial                   02/15/2006        LIBgg77374      Rework ODM Naked/Reflash support
5.  Samir Khial                   11/21/2006        LIBkk61289      Fixed CDMA-D multi-up issue

=================================================================================================*/

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*=================================================================================================
                                          INCLUDE FILES
=================================================================================================*/
/*=================================================================================================
                                          Structure
=================================================================================================*/

typedef struct _PST_CORE_MESSAGING
{
	WORD	   wDeviceID;
	DWORD	   dwMessageLevel;
	LPCSTR     strMessage;
      
} PST_CORE_MESSAGING, *PPST_CORE_MESSAGING;

/*=================================================================================================
                                          Defines
=================================================================================================*/

#define LEVEL_8_RW_ERROR	 0x10000000	//Read/Write Error. An exclusive error only used in reading and writing
#define LEVEL_8_SER_ERROR    0x20000000	//Serial Critical Error.

#define LEVEL_6			 0x00100000	//Flash logging.

#define LEVEL_5_TEST_CMD 0x00010000	//Test command logging

#define LEVEL_4_WRITE_USB		0x00001000	//USB Write data logging
#define LEVEL_4_READ_USB		0x00002000	//USB Read data logging 
#define LEVEL_4_WRITE_SERIAL	0x00004000	//Serial Write data logging
#define LEVEL_4_READ_SERIAL		0x00008000	//Serial Write data logging

#define LEVEL_3			 0x00000100 //Device handle creation and switching

#define LEVEL_2_SERIAL	 0x00000010 //Serial Messages
#define LEVEL_2_USB		 0x00000020 //USB Arrival/Removal Messages

#define LEVEL_1			 0x00000001 //Log general initialization events 


/*=================================================================================================
                                          Macros
=================================================================================================*/

#define LogPSTMessage(deviceID, eLogLevel, sLogMessage)	\
	CPSTMessageLogger::PSTMessage(deviceID, eLogLevel, sLogMessage);
		




