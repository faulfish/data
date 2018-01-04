/**************************************************************************************************
 Copyright (c) 2007 by Motorola, Inc., All Rights Reserved

 Description: declaration of the API functions

 Modification History:

 YYYY/MM/DD		Name				CR/PR		Description
 +--------------+-------------------+-----------+----------------------------
 2007/02/20	 Luc Lalague			LIBkk45196	Creation
 2007/04/17	 Luc Lalague            Libll87090	Completion of phase 2; error logging and processing
 2007/07/17  Renan Battaglin        LIBmm73274  Replaced "_LIB" with "_FLEX_LIB"
 2007/08/08  ZhiMin Leng            Libmm77021  Support the LATEST FLEX format 
****************************************************************************************************/

#ifndef _FLEX_API_H_
#define _FLEX_API_H_

#ifndef _FLEX_LIB // defined in the static library case

	#ifdef FLEXDLLEXPORT
		#define FLEX_API  __declspec(dllexport)
	#else
		#define FLEX_API  __declspec(dllimport)
	#endif  // #ifdef FLEXDLLEXPORT

#else

	#define FLEX_API

#endif  // #ifndef _FLEX_LIB

#include "stdafx.h"
#include <string>
#include <stdlib.h>
#include "pstcommon.h"
#include "LIMITS.H"

// IMPORTANT NOTE: whenever adding errors to EFLEXSTATUS, need to have exact match in the corresponding error message array FLEXSTATUS_MSGS[]
// return values of methods used to keep track of the status of the flexing operation
enum EFLEXSTATUS
{
    EFLEXSTATUS_MIN = 0,
	
    EFLEXSTATUS_OK	= EFLEXSTATUS_MIN, // 0
	EFLEXSTATUS_CANNNOT_OPEN_FLEXFILE, // 1
	EFLEXSTATUS_ERROR_READING_FLEXFILE, // 2
	EFLEXSTATUS_FILE_EMPTY, // 3
	EFLEXSTATUS_FLEXHANDLER_NOT_INITIALIZED, // 4
	EFLEXSTATUS_FLEXFILEPATH_EMPTY, // 5
	EFLEXSTATUS_FILE_SUFFIX_NOT_RECOGNIZED, // 6
	EFLEXSTATUS_CANNOT_FIND_FLEX_TYPE, // 7
	EFLEXSTATUS_PST_FAILED_INITIALIZING_TEST_COMMAND_INTERFACE, // 8
	EFLEXSTATUS_CANNOT_SUSPEND_DEVICE, // 9
	EFLEXSTATUS_PST_TCI_GENERIC_FAILURE, // 10
	EFLEXSTATUS_PST_GET_INTERFACE_HANDLE_FAILED, // 11
	EFLEXSTATUS_PST_SEND_TEST_COMMAND_FAILED, // 12
	EFLEXSTATUS_FAILED_CONVERTING_LINE_TO_HEX_VALUES, // 13
	EFLEXSTATUS_STELEM_SIZE_NOT_VALID, // 14
	EFLEXSTATUS_CANNOT_RESTART_DEVICE, // 15
	EFLEXSTATUS_CANNOT_UNSUSPEND_DEVICE, // 16
	EFLEXSTATUS_PST_SEND_RAW_TEST_COMMAND_FAILED, // 17
	EFLEXSTATUS_CANNOT_GET_TEST_COMMAND, // 18
	EFLEXSTATUS_UNEXPECTED_NULL_POINTER_TO_FLEX_OBJECT, // 19
	EFLEXSTATUS_ARRAY_INDEX_OUT_OF_RANGE, // 20
	EFLEXSTATUS_UNEXPECTED_NULL_POINTER, // 21
	EFLEXSTATUS_WINDOW_HANDLE_CANNOT_BE_NULL, // 22
	EFLEXSTATUS_CANNOT_INSTANTIATE_FLEXLOGGER, //23
	EFLEXSTATUS_CANNOT_CREATE_LOGFILE, // 24

    EFLEXSTATUS_MAX = EFLEXSTATUS_CANNOT_CREATE_LOGFILE// update whenever new status added
};

// IMPORTANT NOTE: whenever adding errors to EFLEXSTATUS, need to have exact match in the corresponding error message of this array
char* const FLEXSTATUS_MSGS[] = 
{
	"OK",														// 0
	"Error opening flex file",									// 1
	"Error reading flex file",									// 2
	"Flex file is empty",										// 3
	"Failed to initialize flex library",						// 4
	"Specified flex file path is empty",						// 5
	"Specified flex file name extension not supported",			// 6
	"Unknown flex file format",									// 7
	"Failed to initialize PST Test Command Interface",			// 8
	"Failed to suspend device",									// 9
	"Generic PST TCI Error",									// 10
	"Failed to retrieve interface handle from PST",				// 11
	"Failed to send test command",								// 12
	"Failed to convert flex line to hex values",				// 13
	"Invalid STELEM Test Command",								// 14
	"Failed to restart device",									// 15
	"Failed to unsuspend device",								// 16
	"Failed to send raw test command",							// 17
	"Failed to retrieve test command type",						// 18
	"Failed to instantiate flex object",						// 19
	"Array out of bounds",										// 20
	"Unexpected NULL pointer",									// 21
	"FlexRegister(): window handle parameter cannot be null",	// 22
	"FlexRegister(): cannot instantiate flex logger",			// 23
	"FlexDevice(): cannot create logfile"						// 24
};


class FLEX_API CFlexError
{
public:
	CFlexError();

	// initializes / resets all member variables
	void ResetErrorDetails();

	// returns detailed string message
	std::string GetErrorDetailsMessage();

	// returns summary string message
	std::string GetErrorMessage();

	// returns the EFLEXSTATUS error code
	EFLEXSTATUS GetFlexStatusError();

	// if not locked, sets the device ID associated with the error
	void SetDeviceID(DEVICEID deviceID);

	// if not locked, sets the EFLEXSTATUS error code for the error
	void SetFlexStatusError(EFLEXSTATUS eErrNum);

	// if not locked, sets the brief error string
	void SetErrorMessage(const std::string& strErrMsg);

	// if not locked, sets the name of the test command associated with the error
	void SetCommandName(const std::string& strCommandName);
	
	// Lock object
	void Lock();

	// Unlock object
	void Unlock();

	CFlexError& operator=( const CFlexError& in_error );

	// static function to set an CFlexError object
    static void SetError(CFlexError *in_pFlexError, DEVICEID in_deviceID, EFLEXSTATUS in_eErrNum, std::string in_strCommandName = "");

	static std::string getErrorMessageForFlexLogfile(CFlexError* in_pFlexError, EFLEXSTATUS in_status);

protected:
	DEVICEID m_deviceID;			// ID of device for which error occurred
	EFLEXSTATUS m_eErrNum;	 		// EFLEXSTATUS error
	std::string m_strErrMsg;  		// Error Message
	std::string m_strCommandName;	// Test command for which error occurred
	BOOL m_bLock;					// If TRUE, no errors can be set anymore
};

// to indicate the what action to take once the flexing of a device is completed
enum EFLEX_ACTION_AFTER_FLEX
{
	NO_ACTION = 0,
	RESTART,
	UNSUSPEND
};

// To be used with the API function FlexDevice as input type
struct StructFlexInput
{
	DEVICEID				m_deviceId;			// the device Id of the device to be flexed
	std::string				m_sFlexFilePath;	// the path of the .hs file to be used to flex the device
	EFLEX_ACTION_AFTER_FLEX m_eActionAfterFlex;
	CFlexError*				m_pFlexError;

	StructFlexInput()
	{
		m_deviceId			= USHRT_MAX;
		m_sFlexFilePath		= "";
		m_eActionAfterFlex	= NO_ACTION;
		m_pFlexError		= NULL;
	}
};


extern "C" FLEX_API EFLEXSTATUS FlexRegister( HWND* inClientHandle = NULL ); 	 // register the window handle of the client application to receive PST messages
																		 // for error logging, error notification and progress status;
																		 // no other PST messages will be sent;
																		 // if NULL, no message is sent.

extern "C" FLEX_API EFLEXSTATUS FlexDevice( const StructFlexInput& inClientInput ); // function used by the client to launch the flexing of a device
  

#endif //  #ifndef _FLEX_API_H_