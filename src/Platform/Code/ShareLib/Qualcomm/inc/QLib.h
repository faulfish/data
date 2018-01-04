/******************************************************************************/
/**
Program: QLib

\file 
  Source\QLib.h
\code 

	- Defines the "C" interface for all Factory Library functions.

	- Table of contents (text search can be done for the following sections)
		
			- General Information
			- Related Documents
			- Version History
			- Callback function definitions

			- Connection/Disconnection Functions
			- Text Logging
			- GSM Diag
			- Streaming Download Diag
			- Diagnostic commands
			- Phone async Logging
			- Phone sync (command) Logging
			- Keypad Diag
			- Handset Diag
			- MediaFLO Diag

			- Commands for multiple FTM modes
			- Dual RX Chain FTM RF Commands 
			- FTM CDMA RF Calibration API V2 
			- GSM FTM
			- GSM Polar Tx Cal FTM
			- PMIC FTM
			- cdma2000 FTM
			- cdma2000 FTM non-signaling
			- EVDO Non-signaling FTM 
			- Bluetooth FTM
			- Audio FTM
			- Camera FTM
			- FTM Log

			- WCDMA BER FTM
			- HSDPA BER FTM

			- GSM BER FTM
			- EGPRS BER FTM
			- GSDI Diag
			- AGPS FTM
			- Common RF FTM
			- MediaFLO FTM
			- MediaFLO NS FTM
			- WLAN FTM
			- QFUSE FTM

			- Software Download
			- EFS Functions

	------------------------------------------------------


	----------------------
	General Information
	----------------------

	- All data types used in this interface are ANSI C compatible.

	- All returns that indicate "true if succeed, false if fail" are defined
		as:  1 = true = Success , 0 = false = failure

		Functions that are not entirely upper case do not correspond directly to 
		an FTM or diagnostic command. 

		For example, QLIB_ConnectServer() is not mapped directly to a diagnostic 
		command.

			
	- Variable Names
		Variable names are prefixed in lower case to indicate the data type.
		The following are defined:

			p = Pointer
			i = Integer type, either char, short, or long
			d = Double or floating point
			e = Enumeration (possibly stored as a short, but representing 
			                 a fixed list of possibilities)
			b = unsigned char, 0=false, 1=true

	----------------------
	Related Documents 
	----------------------

	- The function name convention used in this document is as follows:

		QLIB_ indicates that this function corresponds to library function
		in the QLib project.

		From there, all diagnostic and FTM functions are named exactly as
		the command identifiers are defined in the QLib_Defines.h.

		For a list of all documents referred to by QMSL, please see following seciton in the QDART Help file:
			QDART -> QMSL -> Getting Started -> References.

\endcode


	-----------------------
	Version History
	-----------------------

 \b QUALCOMM  \b PROPRIETARY 

This document contains propriety information, and except with written
permission of Qualcomm INC, such information shall not be 
published, or disclosed to others, or used for any purpose, and the   
document shall not be duplicated in whole or in part.  

\version 
Version information is stored in the file "QMSL_Release_Notes.txt"

\note   
Compiler:  Microsoft Visual C++ v6.0 SP4  
*******************************************************************************/

#if !defined(_QLIB_H)
#define _QLIB_H

#include "windows.h"

/**
	Establish whether the library is being compiled into a DLL (exporting),
	or being included from a client (importing)

	When the DLL is built, then QLIB_EXPORTS should be defined
*/
#ifdef QLIB_EXPORTS
#define QLIB_API __declspec(dllexport)
#else
#define QLIB_API __declspec(dllimport)
#endif

#if defined(QLIB_STATIC)
#undef QLIB_API
#define QLIB_API
//#define QLIB_API __cdecl
#endif


#ifdef __cplusplus
extern "C" {
#endif 


/******************************************************************************
						Callback function definitions
*******************************************************************************/
#if !defined(DIAG_FS_MAX_FILENAME_LEN)
#define DIAG_FS_MAX_FILENAME_LEN      80 /* Specified by EFS2              */
#define DIAG_FS_MAX_PATHNAME_LEN     128 /* Specified by EFS2              */
#endif
#define QLIB_EFS_MAX_FILENAME_LEN DIAG_FS_MAX_PATHNAME_LEN


//! Call back for an EFS directory element 
typedef unsigned char( *EfsDirCB ) 
( 
	char name[QLIB_EFS_MAX_FILENAME_LEN],  //defined in DiagEfsPkt.h
	unsigned char isFile,

	//attributes follow below
	unsigned short iAttributeMask,
	unsigned char iBufferingOption,
	unsigned char iCleanupOption,
	unsigned long iCreateDate,
	unsigned long iFileSize,
	HANDLE hContextID			
);

/**
	Call back for EFS functions
*/
typedef unsigned char (*EfsFileTransferCB)
        (
         char srcname[512],
         char dstname[512],
         int oper,
         int suboper,
         unsigned long bytestransferred,
         unsigned long filesize,
		 HANDLE hContextID
);

/**
	Call back for general SW Download event( replaces the other non-EFS call backs)

	Optional for NV backup/restore and normal download.  Must be used for boot loader and
	multimage download
*/
typedef unsigned char( *generalSWDownloadCB ) 
(
	unsigned char* pEventStructure	// type union generalSwDownloadEvent_union, as defined in SoftwareDownload.h
									// Not declared explicitly here because general users do not need to 
									// include the SWDownload.h file.
);

/**
	Call back for receipt of async messages

	Optional for NV backup/restore and normal download.  Must be used for boot loader and
	multimage download

	\param iMessageSize = number of bytes in the message
	\param pMessageBuffer = pointer to a buffer of memory with the message.  This must be copied by the
	                        user because after the call back is called, the buffer can be changed at
							any time
	\param hContextID = the same context ID used for function calls on a certain COM port.


*/
typedef void( *asyncMessageCB ) 
(
	unsigned short iMessageSize,
	unsigned char* iMessageBuffer,
	HANDLE hContextID
);


/**
	Call back for user-defined send (PC to mobile) function.  
	To be used with QLIB_ConnectServer_UserDefinedTransport()


	\param hQMSL_ContextID = the QMSL that is associated with the connection. 
	\param hUserContextID = the context ID that was passed to QLIB_ConnectServer_UserDefinedTransport()
	\param iRequestSize = Number of bytes to be sent in the request packet.
	\param piRequestBytes = Pointer to a buffer of request packet contents.
	\param piActualWriteSize = to be updated by the user function, the actual # of bytes that were written

	\return 0 if successful (ERROR_SUCCESS) otherwise, an error code as defined by WINERR.H error codes,
	          usually the value of ::GetLastError

     http://msdn.microsoft.com/library/default.asp?url=/library/en-us/debug/base/system_error_codes__0-499_.asp
*/
typedef unsigned long( *userDefinedSend ) 
(
	HANDLE hQMSL_ContextID,
	HANDLE hUserContextID,
	unsigned long iRequestSize,
	unsigned char* piRequestBytes,
	unsigned long* piActualWriteSize
);


/**
	Call back for user-defined receive (from mobile to PC) function.  
	To be used with QLIB_ConnectServer_UserDefinedTransport()

	\param hQMSL_ContextID = the QMSL that is associated with the connection. 
	\param hUserContextID = the context ID that was passed to QLIB_ConnectServer_UserDefinedTransport()
	\param iRequestSize = Number of bytes to be sent in the request packet.
	\param piRequestBytes = Pointer to a buffer of request packet contents.
	\param piResponseSize = Pointer to number of bytes received in the response packet
							NOTE: when calling this function, the value must be equal to the
							maximum size of the receive buffer.  When the function returns, the
							value will be the actual number of bytes filled into the receive buffer.
	\param iResponseBytes = Pointer to a buffer to store the response packet contents.

	\return 0 if successful (ERROR_SUCCESS) otherwise, an error code as defined by WINERR.H error codes,
	          usually the value of ::GetLastError

     http://msdn.microsoft.com/library/default.asp?url=/library/en-us/debug/base/system_error_codes__0-499_.asp

*/
typedef unsigned long( *userDefinedReceive ) 
(
	HANDLE hQMSL_ContextID,
	HANDLE hUserContextID,
	unsigned long* piResponseSize,
	unsigned char* piResponseBytes
);

/**
	Call back for user-defined function to flush Tx and Rx buffers.

	\param hQMSL_ContextID = the QMSL that is associated with the connection. 
	\param hUserContextID = the context ID that was passed to QLIB_ConnectServer_UserDefinedTransport()
	\return 0 if successful (ERROR_SUCCESS) otherwise, an error code as defined by WINERR.H error codes,
	          usually the value of ::GetLastError

     http://msdn.microsoft.com/library/default.asp?url=/library/en-us/debug/base/system_error_codes__0-499_.asp

*/
typedef unsigned long( *userDefinedFlushTxRx ) 
(
	HANDLE hQMSL_ContextID,
	HANDLE hUserContextID

);

/******************************************************************************
						Connection/Disconnection Functions
*******************************************************************************/

	/******************************************************************************/
	/**

	Connect the server to a certain COM port number

	\param iComPort = COM port number to use for communication with phone.
						This does not open the COM port at the operating system
						level until a call is made to the embedded target.


						For auto detection, the COM port passed in can be
						QLIB_COM_AUTO_DETECT, and the first attached phone will 
						be used automatically.

	\return HANDLE to use for all subsequent QLIB calls that require a handle.  
	        If NULL, then no valid phone was found.

	*******************************************************************************/
	QLIB_API HANDLE QLIB_ConnectServer( unsigned int iComPort );

	/******************************************************************************/
	/**

	Connect the server to a certain COM port number

	\param iComPort = COM port number to use for communication with phone.
						This does not open the COM port at the operating system
						level until a call is made to the embedded target.

						For auto detection, the COM port passed in can be
						QLIB_COM_AUTO_DETECT, and the first attached phone will 
						be used automatically.

	\param iWait_ms = # of milliseconds to wait before an error is considered.

	\return HANDLE to use for all subsequent QLIB calls that require a handle.  
	        If NULL, then no valid phone was found.

	*******************************************************************************/
	QLIB_API HANDLE QLIB_ConnectServerWithWait( unsigned int iComPort, unsigned long iWait_ms );

	/******************************************************************************/
	/**

	Connect to a specified logical port number.  Before this function is called, 
	a logical<-->physical mapping must be created using the function: QLIB_AddPortAliasEntry()


	\param iLogicalPort = Logical number to create a connection for.

	\return void 

	*******************************************************************************/
	QLIB_API HANDLE QLIB_ConnectServer_LogicalPort( unsigned int iLogicalPort );

	/******************************************************************************/
	/**

	Set the state of a flag which determines whether the DATA 2 DIAG switch 
	should be attempted when there is a IsPhoneConnected() failure.

	DATA 2 DIAG is a feature of the "AT" data mode, which allows the "diagnostic"
	and "data" channels to the MSM to be multiplexed on the same serial path. 
	This is used only for RS-232 communication, not USB.

	The default setting of the library will be to not have DATA 2 DIAG mode enabled.
	This will make it faster to detect whether or not a phone is connected to a certain
	COM port.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param bSwitchData2Daig = true to attempt AT command to change from data mode to diag mode.
	
	\return True if the library is not in QPST mode.

	\warning This function does not apply when the library is in QPST mode.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SetData2DiagSearch( HANDLE hResourceContext, unsigned char bSwitchData2Daig );

	/******************************************************************************/
	/**

	Setup a connection using a user-defined transport layer.  The user defined transport
	consists of three functions--one to send data, another to receive data, and the last
	to flush the Rx buffer.  


	These functions	will be called by QMSL when QMSL needs to access the physical layer.

	The user also has the option of having QMSL to handle the HDLC formatting (including CRC)
	seperately for the send and receive packets.

 	Before calling this function, the communication channel should be already established
	so that QMSL can make calls to the user functions immediately.

	Until DisconnectServer() is called, a background thread will be running to continually
	read the COM port and check for received data.

	This function will automatically set the library into 'QPHONEMS' mode by calling 
	QLIB_SetLibraryMode() with a parameter of FALSE.  The user-defined mode is a sub
	mode of the the internal engine, QPHONEMS.
 
	\param hUserHandle = User defined handle, to be passed when the send or receive functions are called.
						This value can be any number except for -1 (0xFFFF).  When QLIB_GetComPortNumber()
						is called, this value will be returned.

	\param pUserDefinedSend = User send function, to be called when QMSL will send data to the mobile

	\param pUserDefinedReceive = User defined receive function, to be called when QMSL will receive data 
	                             from the mobile

	\param pUserDefinedFlushRx = user defined function to flush the rx buffer

	\param bQMSL_HandlesTxHDLC = TRUE if QMSL is to handle the HDLC tasks when sending a packet,
	                             including escape sequences, CRC, and trailing flag.

	\param bQMSL_HandlesRxHDLC = TRUE if QMSL is to handle the HDLC tasks when receiving a packet, 
								 including escape sequences, CRC, and trailing flag.

	\return void 

	*******************************************************************************/
	QLIB_API HANDLE QLIB_ConnectServer_UserDefinedTransport( 
			HANDLE hUserHandle,
			userDefinedSend pUserDefinedSend,
			userDefinedReceive pUserDefinedReceive,
			userDefinedFlushTxRx pUserDefinedFlushRx,
			unsigned char bQMSL_HandlesTxHDLC,
			unsigned char bQMSL_HandlesRxHDLC
			);

	/******************************************************************************/
	/**

	Pause the receive thread for a specific device context.  This can only be used
	when the library is in the QPHONEMS mode, and the sub-mode of user
	defined transport layer mode.

	This function can be used to prevent QMSL from accessing the COM port during the
	time that the operation is paused.

	Call QLIB_ResumeDataReceive() to resume operation

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if failure.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_PauseDataReceive( HANDLE hResourceContext );

	/******************************************************************************/
	/**

	Resumes receive operations that were paused by calling QLIB_PauseDataReceive().

	This can only be used when the library is in the QPHONEMS mode, and the sub-mode, 
	of user defined transport layer mode.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if failure.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_ResumeDataReceive( HANDLE hResourceContext );

	/******************************************************************************/
	/**

	Sets the maximum size of how much data will be written to a serial device in one
	call to the Windows serial device call, ::WriteFile().

	In order words, if a packet is larger than this value, then it will be broken
	into smaller sections, with the size of "iSendDataWriteSizeLimit" 
	
	This function is needed for some USB->RS232 converters, which allow only 1k of
	data to be written at a time.  Most USB and PC hardware can allow larger
	transfer sizes.

	This function is only useful for very large packets, such as EFS Write transfers.

	This can only be used when the library is in the QPHONEMS mode. It is ignored
	in the QPST mode and the "user defined transport layer" mode.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSendDataWriteSizeLimit = maximum size of data written to the serial device per call to 
	                                 the Windows serial device driver.  Default size is 1000 at
									 the time the library starts

	\return true if successful, false if failure.

	\warning WARNING: If this number is set higher that 1500, then some phones will have diagnostic
	         buffer overflows.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SetSendDataSerialWriteSizeLimit( 
			HANDLE hResourceContext, 
			unsigned long iSendDataWriteSizeLimit );

	/******************************************************************************/
	/**

	Disconnect the server and close the COM port associated with the server.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return void 

	*******************************************************************************/
	QLIB_API void QLIB_DisconnectServer( HANDLE hResourceContext );

	/******************************************************************************/
	/**

	Disconnects all servers and closes the COM ports associated with those server.

	Normally, this function does not need to be called, because a call to 
	QLIB_DisconnectServer() can be done for each reasource that is opened. 

	This function is to be used for cases when multiple connections are made to different
	COM ports during the lifetime of the application, or in the case of emergency/exception
	shut down.

	\return void 

	\warning All handles will be invalid after this function is called.

	*******************************************************************************/
	QLIB_API void QLIB_DisconnectAllServers( void );

	/******************************************************************************/
	/**

	Uses the port list in the system registry to determine which COM ports are available
	on the PC and returns the information via iNumPorts and pPortList.  This function
	is only tested on Windows XP.

	\param iNumPorts = input/output, The input value is the maximum number of entries that
					   can be added to the pPortList array.  The output value is updated to 
					   reflect how many valid ports were found on the system.  

	\param pPortList = output, byte array.  Each byte indicates a COM port number that is 
	                   available on the system.

	\return true if operation was able to complete successfully and at least one
	        valid port is found.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GetAllPortList( unsigned short* iNumPorts, unsigned short* pPortList  );

	/******************************************************************************/
	/**

	Uses GetAllPortList() to determine which ports have phones available, then uses
	attempts a connection on each of the ports returned.

	\param iNumPorts = input/output, The input value is the maximum number of entries that
					   can be added to the pPortList array.  The output value is updated to 
					   reflect how many valid ports were found on the system.  

	\param pPortList = output, unsigned char array.  Each unsigned char indicates a COM port number that is 
	                   available on the system.

	\param iNumIgnorePorts = # of ports in the "ignore port list."  Zero to check all ports
	\param pIgnorePortList = a list of port numbers that are to be ignored.  This can speed up
	                         the auto detection of ports.

	\return true if operation was able to complete successfully and at least one
	        valid port is found.

	\warning 1) this function will take about 1 second for each port that is on the system 

	         2) All connections will be closed before searching begins.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GetAvailablePhonesPortList( 
		unsigned short* iNumPorts, unsigned short* pPortList,
		unsigned short iNumIgnorePorts, unsigned short* pIgnorePortList );

	/******************************************************************************/
	/**

	Returns the COM port number associated with a specific resource context.  This
	can be used to determine the COM port number for a phone that was connected
	to automatically.
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piPhysicalPort = output, physical port number, such as a 6 for COM6

	\return true if COM port could be determined successfully

	*******************************************************************************/	
	QLIB_API unsigned char QLIB_GetComPortNumber(HANDLE hResourceContext , unsigned short* piPhysicalPort );


	/******************************************************************************/
	/**

	Clears the port alias list.  
	
	The port alias list is a method for mapping a logical port number to a physical
	port number.  This allows users to respond to a situation in which USB ports
	are mapped to unique numbers, depending on which devices have been installed in 
	the past.

	An example of a port alias list:

	    Logical    Physical
	 	 Port       Port
		-------    --------
		   1         COM7
		   2         COM19
		   3         COM6
		   4         COM30

	The port mapping process can be facilitated by the GetAvailablePhonesPortList() 
	function, which returns a list of which ports have a phone installed.  A "calibration"
	procedure could be setup, which tells the user to plug one phone into a port, then
	records which port the phone is in, then instructs them to plug the phone into the
	next USB port, and so on.

	During this procedure, the user software would create a logical list of ports 
	(like 1,2,3,4) and remember which physical COM port number is associated with 
	each logical port.  It is the user's (user of QLIB) responsibility to store this
	association in a persistant way (such as the registry or INI file), then when 
	their software starts up the software would first call ClearPortAliasList(),
	then call AddPortAliasEntry() for each logical/physical port association.

	NOTE: It is not necessary to ClearPortAliasList(), if not alias have been created
	      in the current run-time session.  In other words, the alias list is empty
		  by default, and is only filled in by users of QLIB.

	After that, they could call the QLIB_ConnectServer_LogicalPort() function, to
	connect to a logical port number that has been configured.

	\return true if port alias list is successfully cleared.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_ClearPortAliasList( void );

	/******************************************************************************/
	/**

	Add an entry to the port alias list.  This function should be called once for
	each logical/physical port association.
	
	See ClearPortAliasList() for more information about port aliasing

	\param iLogicalPort = logical port number, user defined number
	\param iPhysicalPort = physical port number, such as a 6 for COM6

	\return true if port alias is successfully updated.

	*******************************************************************************/	
	QLIB_API unsigned char QLIB_AddPortAliasEntry(unsigned short iLogicalPort, unsigned short iPhysicalPort );

	/******************************************************************************/
	/**

	Configures call back functions.

	If one of the pointers is NULL, then the call back will be disabled for
	that call back type.
	
		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param pEfsDirCallback   = Callback for the EfsDirectory() operation
		\param pGeneralSwDownloadCB = Callback for download events
		\param pAsyncMessageCB = Callback for filtering async messages

	\return None

	*******************************************************************************/
	QLIB_API void QLIB_ConfigureCallBacks
				(
				HANDLE hResourceContext,
				EfsDirCB pEfsDirCallback,
				generalSWDownloadCB pGeneralSwDownloadCB,
				asyncMessageCB pAsyncMessageCB
				);

	/******************************************************************************/
	/**

	Configures EFS2 call back functions.

	If one of the pointers is NULL, then the call back will be disabled for
	that call back type.
	
		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param pEfsCallback = Callback for general EFS2 operations
		\param EfsDirCB = Callback for EFS Directory operations

	\return None

	*******************************************************************************/
	QLIB_API void QLIB_ConfigureEfs2CallBacks
				(
				HANDLE hResourceContext,
				EfsFileTransferCB pEfsCallback,
				EfsDirCB pEfsDirCB
				);


	/******************************************************************************/
	/**

	Configure Library Text Message call back function.

	The library text message call back is called each time a text message is printed
	to the log file.  The call back structure will contain the message level (e.g. LOG_IO
	or LOG_FN), and the NULL-terminated string containing the message text.
	
		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param pAsyncMessageCB = Callback for library generated text messages. The "unsigned char*" pointer
		                         will be of the structure type: QMSL_TextLog_struct, defined in QLib_Defines.h
								 Specify a NULL value to disable this callback.

	\return None

	*******************************************************************************/
	QLIB_API void QLIB_ConfigureLibraryTextLogCallBack
				(
				HANDLE hResourceContext,
				asyncMessageCB pAsyncMessageCB
				);

	/******************************************************************************/
	/**

	Configures a specific timeout value.
	
		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param eTimeOutId = Identifier index of timeout to change, index defined by 
		                    QMSL_TimeOutType_Enum in QLibDefines.h 
		\param iNewValue_ms = number of millseconds for timeout value

	\return None

	*******************************************************************************/
	QLIB_API unsigned char QLIB_ConfigureTimeOut
				(
				HANDLE hResourceContext,
				unsigned long eTimeOutId,
				unsigned long iNewValue_ms
				);

	/******************************************************************************/
	/**

	Get a specific timeout value.
	
		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param eTimeOutId = Identifier index of timeout to change, index defined by 
		                    QMSL_TimeOutType_Enum 

		\return timeout value in milliseconds

	\return None

	*******************************************************************************/
	QLIB_API unsigned long QLIB_GetTimeOut( HANDLE hResourceContext, unsigned long eTimeOutId );

	/******************************************************************************/
	/**

	CDMA ICD, 3.4.123 Diagnostic Protocol Loopback Request/Response

	Pings the phone using the diagnostic command DIAG_PROTOCOL_LOOPBACK_F.

	Additionally verifies the connection using the dignostic version command DIAG_VERNO_F.

	This command has a timeout value that can be checked or modified using the identifier
	QMSL_Timeout_IsPhoneConnected when calling QLIB_ConfigureTimeOut() or QLIB_GetTimeOut()

	The default timeout is 200ms, so that as little time as possible will be spent to wait
	for a phone request in the event that a phone is not present.  Other diagnostic functions
	use a longer timeout, so this function is the only one that should be called until a 
	phone is verified to be on a certain COM port.


	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\return Returns true if the phone is connected, false if it is not connected

	*******************************************************************************/
	QLIB_API unsigned char QLIB_IsPhoneConnected(HANDLE hResourceContext );

	/******************************************************************************/
	/**

	CDMA ICD, 2.1.2 Asynchronous data protocol

	Sends a request/response packet.  This allows the user to send any diagnostic
	or FTM command that is available in the protocol documents.

	The request and response packets will be formatted properly for HDLC transmission.

	This function can also be used to send packets to access new FTM functionality
	that is added before the PC library is updated to support that new function
	directly.

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iRequestSize = Number of bytes to be sent in the request packet.
		\param piRequestBytes = Pointer to a buffer of request packet contents.
		\param piResponseSize = Pointer to number of bytes received in the response packet
		                        NOTE: when calling this function, the value must be equal to the
								maximum size of the receive buffer.  When the function returns, the
								value will be the actual number of bytes filled into the receive buffer.
		\param iResponseBytes = Pointer to a buffer to store the response packet contents.
		\param iTimeout = Number of milliseconds to wait for a timeout.  

	\return Returns true if the first byte of the response packet matches
	        the first byte of the request packet, false if it the phone is
			not connected or the first bytes of request and response packets
			do not match.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SendSync
				(
				HANDLE hResourceContext, 
				short iRequestSize,
				unsigned char* piRequestBytes,
				short* piResponseSize,
				unsigned char* piResponseBytes,
				unsigned long iTimeout		
				);

	/******************************************************************************/
	/**

	Sends a asynchronous packet.  Unlike SendSync, this command does not wait for a
	response packet from the phone.

	This command is only usable when the phone is in QPHONEMS mode, not QPST mode

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iRequestSize = Number of bytes to be sent in the request packet.
		\param piRequestBytes = Pointer to a buffer of request packet contents.
		\param iTimeout = Number of milliseconds to wait for a timeout.
		\param bStartFlag = 0 - SendASync will send data as it is, 1 - SendASync will add Start and Stop bytes

	\return Returns true if the send operation completes successfully.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SendASync
				(
				HANDLE hResourceContext, 
				unsigned short iRequestSize,
				unsigned char* piRequestBytes,
				unsigned long iTimeout,
				unsigned char bStartFlag
				);

	/******************************************************************************/
	/**
	Sets mode for Send
	This command is only usable when the phone is in QPHONEMS mode, not QPST mode

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param bStartFlag = 0 - Send will send data as it is, 1 - Send will add Start and Stop bytes

	\return Returns last status of StartFlag

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SetStartFlag
				(
				HANDLE hResourceContext, 
				unsigned char bStartFlag
				);

	/******************************************************************************/
	/**

	Sends RAW data.  This allows the user to send any AT 
	command that is available in the protocol documents.

	The message will not be formatt in any way. The user should format message correclty.
	(i.e. AT commant finished with "\r" and close string with "\0")

	This command is only usable when the phone is in QPHONEMS mode, not QPST mode.

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iRequestSize = Number of bytes to be sent in the request packet.
		\param piRequestBytes = Pointer to a buffer of request packet contents.
		\param piResponseSize = Pointer to number of bytes received in the response packet
		                        NOTE: when calling this function, the value must be equal to the
								maximum size of the receive buffer.  When the function returns, the
								value will be the actual number of bytes filled into the receive buffer.
		\param iResponseBytes = Pointer to a buffer to store the response packet contents.
		\param iTimeout = Number of milliseconds to wait for a timeout.  

	\return Returns true response message received, false if it the phone is
			not connected or the time delay has run out.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SendRAW
				(
				HANDLE hResourceContext, 
				short iRequestSize,
				unsigned char* piRequestBytes,
				short* piResponseSize,
				unsigned char* piResponseBytes,
				unsigned long iTimeout		
				);

	/******************************************************************************/
	/**
	Sets packet mode for sending Diagnostic, FTM or RAW data.

	This function sets both phone and Qlib to send data either in Diagnostic or RAW mode.

	This command is only usable when the phone is in QPHONEMS mode, not QPST mode

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iPacketMode =	QLIB_PacketMode_BothDiag = 0 - both Qlib and phone in Diagnostic/FTM command mode
									(sending Diagnostic/FTM commands via Diagnostic COM port to phones Diagnostic port),
									used to send diagnostic commands via serial cable or diagnostic COM port on USB cable
								QLIB_PacketMode_BothAT = 1 - both Qlib and phone in Data/AT command mode
									(sending AT commands via Diagnostic COM port to phones Diagnostic port),
									used to send AT comands via serial cable
								QLIB_PacketMode_LibAT = 2 - Qlib in Data/At command mode and phone as it is
									(sending AT commands via Diagnostic COM port to phones modem port)
									used to send AT commands via modem COM port on USB cable
								QLIB_PacketMode_OBEX = 3 - QLib in OBEX protocol mode

	\return Returns true if mode switched, false if phone was not connectedor failed switching modes

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SetPacketMode
				(
				HANDLE hResourceContext, 
				unsigned char iPacketMode
				);

	/******************************************************************************/
	/**
	Streaming Download ICD, 3.2.1 19 Hello packet

	This function sends the "hello" packet to the phone, and returns the response
	message, in its entirety, by copying the response packet to a user defined
	buffer.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param iVersionNumber = Host shall set this field to indicate the maximum version 
	                        of this protocol that the host supports. The value for this 
							field is 0x03.

	\param iCompatibleVersion = Host shall set this field to indicate the lowest version 
	                            of the protocol that it supports. The value for this field 
								is 0x02.

	\param iFeatureBits = Host shall set these bits to indicate the negotiated set 
	                      of features requested to be used.

	\param pResponseBuffer = The entire response packet, as defined by section 3.2.2 of
	                         the streaming download ICD.

	\return true if response packet was successfully returned, false if fail for any reason.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DOWNLOAD_Hello
		( 
			HANDLE hResourceContext, 
			unsigned char iVersionNumber,
			unsigned char iCompatibleVersion,
			unsigned char iFeatureBits,
			unsigned char* pResponseBuffer
		);


	/******************************************************************************/
	/**

	Set the state of DTR.  
	This implementation is only defined with the non-QPST server version of the library

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param bSetDTR_High = true to set DTR High, false to set DTR Low
	\return Returns true if the call succeeded, false if it does not

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SetDTR_State( HANDLE hResourceContext, unsigned char bSetDTR_High);

	/******************************************************************************/
	/**

	Set the state of RTS.  
	This implementation is only defined with the non-QPST server version of the library

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param bSetRTS_High = true to set RTS High, false to set RTS Low
	\return Returns true if the call succeeded, false if it does not

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SetRTS_State( HANDLE hResourceContext, unsigned char bSetRTS_High);

	/******************************************************************************/
	/**

	Clears the Rx buffer.  

	This implementation is only defined with the non-QPST server version of the library

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return Returns true if the call succeeded, false if it does not

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FlushRxBuffer( HANDLE hResourceContext  );


	/******************************************************************************/
	/**

	Returns a NULL terminated string with the version information in the format:
		"QLIB VXX.yy.xx, <MODE>" where: XX is the major version #, yy is the sub-
		minor version, xx is the minor version number and <MODE> is either QPST or QPHONEMS, 
		depending upon the library mode

		Examples:
			"QLIB V04.0.11,QPHONEMS"
			"QLIB V04.0.11,QPST"

	\param psVersion = Buffer to store the version number, 25 bytes should be reserved
	                  for this buffer.
	\return void

	*******************************************************************************/
	QLIB_API void QLIB_GetLibraryVersion( char* psVersion );

	/******************************************************************************/
	/**

	Set the library mode--QPST or QPHONEMS.  The default state of the library is to
	use QPHONEMS.  In order to use QPST, this function must be called with 
	bUseQPST set to TRUE.

	\param bUseQPST = true to use QPST, false to use QPHONEMS
	\return void

	*******************************************************************************/
	QLIB_API void QLIB_SetLibraryMode( unsigned char bUseQPST );

	/******************************************************************************/
	/**

	Returns flags indicating the capabilities of the library

	\param pbSupportsDiag         = true if diag/ftm commands are supported
	\param pbSupportsEFS          = true if EFS commands are supported
	\param pbSupportsSwDownload   = true if software download commands are supported
	\param pbUsingQPST            = true if QPST is being used, false if QPHONEMS is used

	\return void

	*******************************************************************************/
	QLIB_API void QLIB_GetLibraryCapabilities( 
		unsigned char* pbSupportsDiag, 
		unsigned char* pbSupportsEFS, 
		unsigned char* pbSupportsSwDownload, 
		unsigned char* pbUsingQPST );

	/******************************************************************************/
	/**

	The function checks whether the phone is in FTM mode.

	Depending on the command code ( FTM_COMMAND_59 or FTM_COMMAND75 ), 
	this function uses different methods to check for FTM mode.  

	Method 1:
	If the command code is set to FTM_COMMAND_59, this function reads the NV_FTM_MODE item to check
	whether the phone is in FTM mode.
	
	Note that MSM6000/6025/6050 and older MSM targets use FTM_COMMAND_59 command code for FTM mode transcation.  

	Method 2:
	If the command code is set to FTM_COMMAND_75, this function issues a CM state info request 
	to query the phone state.  The command is CMLOG_STATE_INFO_F (refer 80-V1294-7, 3.2).  

	General comment:
	The command code should be set by QLIB_FTM_SET_COMMAND_CODE function before this function is called.
	By default, the command code is FTM_COMMAND_75.

	\param pbIsFTMMode = output, true(1) if the phone is in FTM mode, 
					             false(0) if the phone is not in FTM mode 

	\return true if the function is successful

	*******************************************************************************/
	QLIB_API unsigned char QLIB_IsFTM_Mode(HANDLE hResourceContext, unsigned char *pbIsFTMMode);


	/******************************************************************************/
	/**

	The function changes phone to FTM or ONLINE mode through NV item NV_FTM_MODE method.

	This function:

	1. Changes phone to offline mode (QLIB_DIAG_CONTROL_F(MODE_OFFLINE_F))
	2. Delays by the timeout value QMSL_Timeout_Delay_OFFLINE, which can be set wiht
	   QLIB_ConfigureTimeout()
	2. Writes bFTMmode value to NV item NV_FTM_MODE (453)
	
	If (bReset == 1), the function returns immediately after reset command is issued.

	\param bFTMMode = (1 = FTM mode), (0 = ONLINE mode)  
	\param bReset = (1 = Reset after NV write), (0 = No reset after NV write)

	\return true if the function is successful

	*******************************************************************************/

	QLIB_API unsigned char QLIB_ChangeFTM_BootMode(HANDLE hResourceContext, unsigned char bFTMmode, unsigned char bReset);

	/******************************************************************************/
	/**

	The function changes the mode to ONLINE or FTM mode using run time method

	The function calls IsFTM_Mode to determines the phone is in FTM mode or not.

	If current mode is not equal to bFTMmode, it will switch mode.

	General comment:
	This function can't be used in MSM6000/6025/6050 and older MSM targets.  As they
	use FTM_COMMAND_59 command code for FTM and DO NOT support runtime swtiching to FTM
		
	The command code should be set to FTM_COMMAND_75 by QLIB_FTM_SET_COMMAND_CODE function 
	before this function is called.

	By default, the command code is FTM_COMMAND_75.

	\param bFTMMode = (1 = FTM mode), (0 = ONLINE mode) 

	\return true if the function is successful

	*******************************************************************************/

	QLIB_API unsigned char QLIB_ChangeFTM_ModeRuntime(HANDLE hResourceContext, unsigned char bFTMmode);

/******************************************************************************
					Text Logging
*******************************************************************************/

	/******************************************************************************/
	/**

	Function to start logging send/receive data information to a text file.  After calling this
	function, the log file will remain open until the ClosePort() or DisconnectServer()
	operations are called.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param sLogfile = path to log file that will be created
	
	\return True if file is opened successfully.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_StartLogging( HANDLE hResourceContext, char* sLogFile );

	/******************************************************************************/
	/**

	Function to Stop text logging

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		
	\return True if file is opened successfully.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_StopLogging( HANDLE hResourceContext );

	/******************************************************************************/
	/**

	Function to start logging send/receive data information to a DLF file.  After calling this
	function, the log file will remain open until the ClosePort() or DisconnectServer()
	operations are called.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param sLogfile = path to log file that will be created.  Extension .DLF should be specified
	\param bAlwaysUsePC_Time = TRUE to always use PC time in the log stamps for the DLF entries. 

	                           If FALSE then time will be determined from the mobile's time stamp
							   after an async messasge (log, event, debug message) has been received.
							   Currently this behavior is disabled and DLF entries are always made
							   with the the PC time.
	
	\return True if file is opened successfully.

	\warning Funciton is not implemented

	*******************************************************************************/
	QLIB_API unsigned char QLIB_StartDLF_Logging( 
			HANDLE hResourceContext, 
			char* sLogFile,
			unsigned char bAlwaysUsePC_Time );

	/******************************************************************************/
	/**

	Function to Stop DLF logging

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		
	\return True if file is opened successfully.

	\warning Function is not implement

	*******************************************************************************/
	QLIB_API unsigned char QLIB_StopDLF_Logging( HANDLE hResourceContext );

	/******************************************************************************/
	/**

	Function to set log level for text logging (not the same as log records from the phone).

	Log levels are treated as flags and can be added:
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param uiLogFlags = Mask for text log
	\code
	LOG_NOTHING		0x0000	= log nothing

	LOG_IO          0x0001	= data IO (data bytes)
	LOG_FN          0x0002	= function calls with parameters
	LOG_RET         0x0004	= function return data
	LOG_INF         0x0008	= general information (nice to know)
	LOG_ERR         0x0010	= critial error information

	LOG_C_HIGH_LEVEL_START  0x0200 = High level C function start, indicates the begining of a high level C function, which
	                                 calls other low level C functions internal to the library

	LOG_C_HIGH_LEVEL_STOP   0x4000 = High level C function stop

	LOG_IO_AHDLC    0x0020	= HDLC IO tracing (data bytes)
	LOG_FN_AHDLC    0x0040	= HDLC layer function calls
	LOG_RET_AHDLC   0x0080	= HDLC function return data
	LOG_INF_AHDLC   0x0100	= HDLC general information

	LOG_IO_DEV      0x0400	= device IO tracing (data bytes)
	LOG_FN_DEV      0x0800	= device layer function calls
	LOG_RET_DEV     0x1000	= device function return data
	LOG_INF_DEV     0x2000	= device general information

	LOG_DEFAULT     ( LOG_IO | LOG_ERR | LOG_ERR_AHDLC | LOG_ERR_DEV )

	LOG_ALL			0xFFFF	// everything
	\endcode

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param uiLogFlags = log flags as listed above
	
	\return True if file is opened successfully.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SetLogFlags( HANDLE hResourceContext, unsigned int uiLogFlags );

	/******************************************************************************/
	/**

	Function to actvate/deactivate an extended text message category.

	This is used to enable verbose text messages, which can be used for troubleshooting.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param eMessageCategory = enumeration value of the message category to be enabled.  
	                           Uses the QMSL_Extended_Text_Message_Category_Enum, in QLib_Defines.h.

						
	\code
       QMSL_ExtTextMsgCat_MediaFLO_Parse         = 0,  //!< ' MediaFLO Layer one log parsing
       QMSL_ExtTextMsgCat_cdma2000_RDA_Parse     = 1   //!< ' cdma2000 RDA log parsing for SER/FER
	\endcode
	\param bEnableCategory = TRUE to enable the message category, FALSE to disable it
	
	\return True if file is opened successfully.


	*******************************************************************************/
	QLIB_API unsigned char QLIB_ExtendedTextMessage_SetCategoryEnable( 
								HANDLE hResourceContext, 
								unsigned short eMessageCategory, 
								unsigned char bEnableCategory
								);

	/******************************************************************************/
	/**

	Function to get the size of the extended text message category list.  This can be used before
	calling QLIB_GetExtetendedTextMessageCategoryListItem(), in order to determine the
	list size, in case the memory must be allocated in the client.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piListSize = Output, stores the number of extended text message categories
	
	\return True if file is opened successfully.


	*******************************************************************************/
	QLIB_API unsigned char QLIB_ExtendedTextMessage_GetCategoryListSize( 
								HANDLE hResourceContext, 
								unsigned short* piListSize
								);

	/******************************************************************************/
	/**

	Function retreive one element in the extended text message category list.  
	The element values are a string with the name of the item and an enable/disable status

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param eMessageCategory = Input, enumeration value of the message category to be enabled.  
	                           Uses the QMSL_Extended_Text_Message_Category_Enum, in QLib_Defines.h.
	\param sCategoryName = Output, stores the number of extended text message category indexed
	                       by eMessageCategory.  Must be QMSL_EXTENDED_TEXT_MESSAGE_CATEGORY_NAME_SIZE
						   bytes reserved, including NULL termination.

						   If sCategoryName is NULL, then no action is taken and only the 
						   bEnabled value will be updated.
	\param pbEnabled = Output, TRUE if the category is enabled, FALSE if it is not.
	
	\return True if file is opened successfully.


	*******************************************************************************/
	QLIB_API unsigned char QLIB_ExtendedTextMessage_GetCategoryListItem( 
								HANDLE hResourceContext, 
								unsigned short eMessageCategory,
								char* sCategoryName,
								unsigned char* pbEnabled
								);



/******************************************************************************
					GSM Diag
*******************************************************************************/

	/******************************************************************************/
	/**
	GSM Diagnostic IC, 3.2.2/3 Status Request/Response
	
	  Get the GSM Call status

		\param aiIMEI[9] = First byte is length (0 to 15), Next 8 bytes are packed BCD
				#define GSM_DIAG_IMEI_SIZE  9

		\param aiIMSI[9] = First byte is length (0 to 15), Next 8 bytes are packed BCD
				#define GSM_DIAG_IMSI_SIZE  9

		\param aiLAI[5] = Location area ID. Format:  3bytes, Public Land Mobile Network Identity, 2bytes Location Area Code 
				#define GSM_DIAG_LAI_SIZE   5

		\param piCellID = Cell identity
		\param piCM_CallState = refer to enumeration cm_call_state_enum:
			\code
               CM_CALL_STATE_NONE	= -1,		-  FOR INTERNAL USE OF CM ONLY!
               CM_CALL_STATE_IDLE	= 0,		-  Call is in idle state - i.e. no call
               CM_CALL_STATE_ORIG	= 1,		-  Call is in origination state 
               CM_CALL_STATE_INCOM	= 2,		-  Call is in alerting state 
               CM_CALL_STATE_CONV,	= 3,		-  Call is in conversation state
               CM_CALL_STATE_CC_IN_PROGRESS = 4	-  Call is originating but waiting for call control to complete 
			\endcode
		\param piCM_OperationalMode = refer to enumeration sys_oprt_mode_enum:
			\code
               SYS_OPRT_MODE_NONE	= -1,				-  FOR INTERNAL USE OF CM ONLY!
               SYS_OPRT_MODE_PWROFF	= 0,				-  Phone is powering off
               SYS_OPRT_MODE_FTM		= 1,			-  Phone is in factory test mode
               SYS_OPRT_MODE_OFFLINE	= 2,			-  Phone is offline
               SYS_OPRT_MODE_OFFLINE_AMPS = 3,			-  Phone is offline analog
               SYS_OPRT_MODE_OFFLINE_CDMA = 4,			-  Phone is offline cdma
               SYS_OPRT_MODE_ONLINE = 5,				-  Phone is online
               SYS_OPRT_MODE_LPM = 6,					-  Phone is in LPM - Low Power Mode
               SYS_OPRT_MODE_RESET = 7,					-  Phone is resetting - i.e. power-cycling
               SYS_OPRT_MODE_NET_TEST_GW = 8,			-  Phone is conducting network test for GSM/WCDMA. 
               SYS_OPRT_MODE_OFFLINE_IF_NOT_FTM = 9,	-  offline request during powerup.
               SYS_OPRT_MODE_PSEUDO_ONLINE = 10,		-  Phone is pseudo online, tx disabled

			\endcode
		\param piCM_SystemMode = refer to enumeration for sys_sys_mode_enum:
			\code
               SYS_SYS_MODE_NONE	= -1,		//!< ' FOR INTERNAL USE ONLY!
               SYS_SYS_MODE_NO_SRV	= 0,		//!< ' No service = NV_MODE_INACTIVE
               SYS_SYS_MODE_AMPS	= 1,		//!< ' Analog Mobile Phone System (AMPS) mode
               SYS_SYS_MODE_CDMA	= 2,		//!< ' Code Division Multiple Access (CDMA) mode
               SYS_SYS_MODE_GSM		= 3,		//!< ' Global System for Mobile communications (GSM) mode
               SYS_SYS_MODE_HDR		= 4,		//!< ' High Data Rate (HDR) mode
               SYS_SYS_MODE_WCDMA	= 5,		//!< ' Wideband CDMA (WCDMA) mode
               SYS_SYS_MODE_GPS		= 6,		//!< ' Global Positioning System (GPS) mode
               SYS_SYS_MODE_GW		= 7,		//!< ' GSM and WCDMA mode
               SYS_SYS_MODE_MAX					//!< ' FOR INTERNAL USE ONLY!
			\endcode

	\return true if response packet was successfully returned, false if fail for any reason.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_GSM_STATUS_F( 
					HANDLE hResourceContext,
					unsigned char iIMEI[9], unsigned char aiIMSI[9], 
					unsigned char aiLAI[5],  unsigned short* piCellID, unsigned char* piCM_CallState, 
					unsigned char* piCM_OperationalMode, unsigned char* piCM_SystemMode );


	/******************************************************************************/
	/**
	GSM Diagnostic IC, 3.2.7/8 Call origination request/response
	
	  Originate a call from the mobile

		\param iNumDigits = number of digits in the IMSI
		\param aiIMSI = array of IMSI digits to dial
							#define GSM_DIAG_CALL_ORG_IMSI_SIZE 32 = # of digits
						IMSI characters are in the ASCII format, for example 0x35 for '5'

	\return true if response packet was successfully returned, false if fail for any reason.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_GSM_CALL_ORIG_F( HANDLE hResourceContext, unsigned char iNumDigits, unsigned char aiIMSI[32] );


	/******************************************************************************/
	/**
	GSM Diagnostic IC, 3.2.9/10 Call end request/response
	
	  End the currently active call 

	\return true if response packet was successfully returned, false if fail for any reason.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_GSM_CALL_END_F( HANDLE hResourceContext );

/******************************************************************************
					Streaming Download Diag
*******************************************************************************/

	/******************************************************************************/
	/**
	Streaming Download ICD, 3.2.1 19 Hello packet, A specialized version of this 
	command, to be used as a method to PING a phone in download mode.

	Call DOWNLOAD_Hello with default parameters.  

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if response packet was successfully returned, false if fail for any reason.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DOWNLOAD_Ping( HANDLE hResourceContext );



/******************************************************************************
						Diagnostic commands
*******************************************************************************/


	/******************************************************************************/
	/**
	DMSS Diagnostic ICD, 3.4.38 Mode Change Request/Response
	WCDMA Diagnostic ICD, 3.5.23 Mode Change Request/Response

	Changes the current system state.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param eMode = Defined by mode_enum_type in QLib_Defines.h
	\code 
		MODE_OFFLINE_A_F = 0,	//!<' Go to offline analog
		MODE_OFFLINE_D_F = 1,	//!<' Go to offline digital 
		MODE_RESET_F = 2,		//!<' Reset. Only exit from offline 
		MODE_FTM_F = 3,			//!<' FTM mode
		MODE_ONLINE_F = 4,		//!<' Go to Online 
		MODE_MAX_F = 5			//!<' Last (and invalid) mode enum value
	
	\endcode

	\return Returns true if the phone mode change executed correctly
	
	\warning,  If the new mode is MODE_FTM_F, a delay will be issued after the command
	is successfully sent to the mobile.  The duration of the delay is based on the
	value of delay type "QMSL_Timeout_Delay_ONLINE_FTM" which can be controled by 
	QLIB_ConfigureTimeOut() and read by QLIB_GetTimeOut()

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_CONTROL_F(HANDLE hResourceContext, int eMode );


	/******************************************************************************/
	/**
	
	DMSS ICD, 3.4.35 Nonvolatile Item Read Request/Response
	WCDMA Diagnostic ICD, 3.5.23 Mode Change Request/Response

	Read an NV Item


	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  item_id   = Enumeration ID of NV Item
	\param  pItemData = Reference to storage area for data
	\param	iLength   = size of data, in unsigned chars
	\param  iStatus   = Reference to storage area for status
	\code
     NV_DONE_S = 0,      // Request completed okay 
     NV_BUSY_S = 1,      // Request is queued 
     NV_BADCMD_S = 2,    // Unrecognizable command field 
     NV_FULL_S = 3,      // The NVM is full 
     NV_FAIL_S = 4,      // Command failed, reason other than NVM was full 
     NV_NOTACTIVE_S = 5, // Variable was not active 
     NV_BADPARM_S = 6,   // Bad parameter in command block 
     NV_READONLY_S = 7,  // Parameter is write-protected and thus read only 
     NV_BADTG_S = 8,     // Item not valid for Target 
     NV_NOMEM_S = 9,     // free memory exhausted 
     NV_NOTALLOC_S = 10  // address is not a valid allocation 

	\endcode

	\return true if successful, false if fail.

	\warning None.

	*******************************************************************************/
	QLIB_API unsigned char  QLIB_DIAG_NV_READ_F
		(HANDLE hResourceContext, unsigned short iItemID, unsigned char* pItemData, int iLength, unsigned short* iStatus);

	/******************************************************************************/
	/**
	
	DMSS ICD, 3.4.35 Nonvolatile Item Read Request/Response
	WCDMA Diagnostic ICD, 3.5.23 Mode Change Request/Response

	Read an NV Item--this version handles NV array items


	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  item_id   = Enumeration ID of NV Item
	\param	iArrayIndex =  Index within NV array
	\param  pItemData = Reference to storage area for data
	\param	iLength   = size of data, in unsigned chars
	\param  iStatus   = Reference to storage area for status
	\code
     NV_DONE_S = 0,      // Request completed okay 
     NV_BUSY_S = 1,      // Request is queued 
     NV_BADCMD_S = 2,    // Unrecognizable command field 
     NV_FULL_S = 3,      // The NVM is full 
     NV_FAIL_S = 4,      // Command failed, reason other than NVM was full 
     NV_NOTACTIVE_S = 5, // Variable was not active 
     NV_BADPARM_S = 6,   // Bad parameter in command block 
     NV_READONLY_S = 7,  // Parameter is write-protected and thus read only 
     NV_BADTG_S = 8,     // Item not valid for Target 
     NV_NOMEM_S = 9,     // free memory exhausted 
     NV_NOTALLOC_S = 10  // address is not a valid allocation 

	\endcode

	\return true if successful, false if fail.

	\warning None.

	*******************************************************************************/
	QLIB_API unsigned char  QLIB_DIAG_NV_READ_ARRAY_F
		(HANDLE hResourceContext, unsigned short iItemID, unsigned char iArrayIndex, unsigned char* pItemData, int iLength, unsigned short* iStatus);

	/******************************************************************************/
	/**

	DMSS ICD, 3.4.36 Nonvolatile Item Write Request/Response	
	WCDMA Diagnostic ICD, 3.5.22 Nonvolatile Item Write Request/Response

	Write an NV Item


	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iItemID  = ID of NV Item
	\param  pItemData = Reference to storage area for data
	\param	iLength = size of data, in unsigned chars
	\param  iStatus = Reference to storage area for status
	\code
     NV_DONE_S = 0,      // Request completed okay 
     NV_BUSY_S = 1,      // Request is queued 
     NV_BADCMD_S = 2,    // Unrecognizable command field 
     NV_FULL_S = 3,      // The NVM is full 
     NV_FAIL_S = 4,      // Command failed, reason other than NVM was full 
     NV_NOTACTIVE_S = 5, // Variable was not active 
     NV_BADPARM_S = 6,   // Bad parameter in command block 
     NV_READONLY_S = 7,  // Parameter is write-protected and thus read only 
     NV_BADTG_S = 8,     // Item not valid for Target 
     NV_NOMEM_S = 9,     // free memory exhausted 
     NV_NOTALLOC_S = 10  // address is not a valid allocation 

	\endcode

	\return true if successful, false if fail.

	\warning None.

	*******************************************************************************/
	QLIB_API unsigned char  QLIB_DIAG_NV_WRITE_F
		(HANDLE hResourceContext, unsigned short iItemID, unsigned char* pItemData, int iLength, unsigned short* iStatus);

	/******************************************************************************/
	/**

	DMSS ICD, 3.4.36 Nonvolatile Item Write Request/Response	
	WCDMA Diagnostic ICD, 3.5.22 Nonvolatile Item Write Request/Response

	Write an NV Item--this version handles NV array items

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iItemID  = ID of NV Item
	\param	iArrayIndex =  Index within NV array
	\param  pItemData = Reference to storage area for data
	\param	iLength = size of data, in unsigned chars
	\param  iStatus = Reference to storage area for status
	\code
     NV_DONE_S = 0,      // Request completed okay 
     NV_BUSY_S = 1,      // Request is queued 
     NV_BADCMD_S = 2,    // Unrecognizable command field 
     NV_FULL_S = 3,      // The NVM is full 
     NV_FAIL_S = 4,      // Command failed, reason other than NVM was full 
     NV_NOTACTIVE_S = 5, // Variable was not active 
     NV_BADPARM_S = 6,   // Bad parameter in command block 
     NV_READONLY_S = 7,  // Parameter is write-protected and thus read only 
     NV_BADTG_S = 8,     // Item not valid for Target 
     NV_NOMEM_S = 9,     // free memory exhausted 
     NV_NOTALLOC_S = 10  // address is not a valid allocation 

	\endcode

	\return true if successful, false if fail.

	\warning None.

	*******************************************************************************/
	QLIB_API unsigned char  QLIB_DIAG_NV_WRITE_ARRAY_F
		(HANDLE hResourceContext, unsigned short iItemID, unsigned char iArrayIndex, 
		 unsigned char* pItemData, int iLength, unsigned short* iStatus);

	/******************************************************************************/
	/**

	Activates a batch mode for performing NV Write operations.  In this
	mode each call to QLIB_DIAG_NV_WRITE_F and QLIB_DIAG_NV_WRITE_ARRAY_F will be 
	queued in the PC's RAM and will not be written immediately to the phone.  This is 
	done in order to take advantage of the multiple-NV write operation, which will
	be invoked when the DIAG_NV_WRITE_FlushBatchQueue() command is called.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param bStartBatchMode = true to activate NV Write batch mode, false to stop it.

	\return true if successful, false if fail.

	\warning After this function is completed, the batch mode will be disabled, and must be
	         restarted with a call to DIAG_NV_WRITE_SetBatchMode.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_NV_WRITE_SetBatchMode( HANDLE hResourceContext, unsigned char bStartBatchMode );

	/******************************************************************************/
	/**

	Allows the user to specify a list of NV items that can be read-only.  When
	the batch queue is flushed, if these NV items fail because the field is
	read-only, then the command will not fail.

	An example of a field that can be read-only is #571, NV_RF_CAL_DATE_I
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param aiNvIdList = Array of NV Item ID numbers.

	\param iListSize = number of items in the NV Item ID list.

	\return true if successful, false if fail.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_NV_WRITE_BatchQueue_SetRO_List( 
							HANDLE hResourceContext, 
							unsigned short* aiNvIdList, 
							unsigned short iListSize);

	/******************************************************************************/
	/**

	Flushes all NV write requests that have been queued since the last time that 
	QLIB_DIAG_NV_WRITE_SetBatchMode().  Uses the multiple NV Write operation to optimize
	the NV write time.
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piLastErrorNvId = the ID of the last NV item that had an error.  If no error,
	                      then it will be a value of NV_WRITE_BATCH_ITEM_NO_ERROR, -1

	\param piLastTotalStatus = return value of the entire write operation, refer to MNVW_Status_Enum
	\param piLastItemStatus = last NV_WRITE status, refer to nv_stat_enum_type

	\return true if successful, false if fail.

	\warning Will reset any open batch session.  If items has been queued up, they 
	         must be written using the QLIB_DIAG_NV_WRITE_FlushBatchQueue().

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_NV_WRITE_FlushBatchQueue( 
		HANDLE hResourceContext, unsigned short* piLastErrorNvId, 
		unsigned short* piLastTotalStatus, unsigned char* piLastItemStatus  );

	/******************************************************************************/
	/**
	DMSS ICD, 3.4.29 Handset Emulation Keypress Request/Response
	WCDMA Diagnostic ICD, 3.5.18 Handset Emulation Keypress Request/Response

	Emulate a key press


	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iKey = ID of Key.  Lookup in QLib_Defines.h
	\param  iHold = Key release indicator
	\code
		0 = Selected keypress will be emulated, 
		    then a key release will be emulated

		1 = Selected keypress will be emulated, 
		    but no key release will be emulated; 
		    this is used to simulate the effect of 
		    holding down a key, such as CLEAR 
		    or END; you should send a second message

	\endcode

	\return true if successful, false if fail.

	\warning None.

	*******************************************************************************/
	QLIB_API unsigned char  QLIB_DIAG_HS_KEY_F(HANDLE hResourceContext, int iKey, unsigned char iHold);


	/******************************************************************************/
	/**
	DMSS ICD, 3.4.1 Version Number Request/Response
	WCDMA Diagnostic ICD, 3.5.15 Diag version request/response

	Return software version information

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param comp_date[ 11] = Compile date Jun 11 1991   
		\param comp_time[ 8 ] = Compile time hh:mm:ss      
		\param rel_date [ 11] = Release date               
		\param rel_time [ 8 ] = Release time               
		\param ver_dir  [ 8 ] = Version directory          
		\param scm =  Station Class Mark         
		\param mob_cai_rev = CAI rev                    
		\param mob_model = Mobile Model               
		\param mob_firm_rev = Firmware Rev               
		\param slot_cycle_index = Slot Cycle Index           
		\param voc_maj = Vocoder major version                
		\param voc_min = Vocoder minor version                

	\return true if successful, false if fail.

	\warning None.

	*******************************************************************************/
	QLIB_API unsigned char  QLIB_DIAG_VERNO_F(
										HANDLE hResourceContext, 
										char comp_date[ 11 ],
										char comp_time[ 8 ],
										char rel_date [ 11 ],
										char rel_time [ 8 ],
										char ver_dir  [ 8 ],
										unsigned char* scm,
										unsigned char* mob_cai_rev,
										unsigned char* mob_model,
										unsigned short* mob_firm_rev,
										unsigned char* slot_cycle_index,
										unsigned char* voc_maj,
										unsigned char* voc_min
											);

	/******************************************************************************/
	/**
	DMSS ICD, 3.4.3 Memory Peek Request/Response  (8-bit version)
	WCDMA Diagnostic ICD, 3.5.1 Memory peek request/response

	When the DM sends a Memory Peek Request Message to the DMSS, the DMSS responds with a
	Memory Peek Response Message that contains the contents of a block of memory starting at the
	given ADDRESS and consisting of LENGTH items.
	
	The request may ask for up to 16 BYTES.
	
	The ADDRESS is specified as a 32-bit flat address.

	This packet is protected by the DMSS Security Plan. The Security Passunsigned short Request Message
	must be sent to the DMSS successfully before this message will be accepted.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iAddress = Address of peek region start; given as a flat address
	\param	iNumItems = number of items to read, must be <= 16 bytes (16 8-bit items)
	\param  pArray = array of items in which data will be stored.  Size must be at least
	                     "<iNumItems> * data size" bytes long 
						 For 8-bit items, this is <iNumItems> * 1 (bytes)

	\return true if successful, false if fail.

	\warning None

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_MEMORY_PEEK_F( 
		HANDLE hResourceContext, unsigned long iAddress, unsigned short iNumItems, unsigned char* pItemArray );


	/******************************************************************************/
	/**
	DMSS ICD, 3.4.3 Memory Peek Request/Response (16-bit version)
	WCDMA Diagnostic ICD, 3.5.1 Memory peek request/response

	When the DM sends a Memory Peek Request Message to the DMSS, the DMSS responds with a
	Memory Peek Response Message that contains the contents of a block of memory starting at the
	given ADDRESS and consisting of LENGTH items.
	
	The request may ask for up to 8 16-bit values.
	
	The ADDRESS is specified as a 32-bit flat address.

	This packet is protected by the DMSS Security Plan. The Security Passunsigned short Request Message
	must be sent to the DMSS successfully before this message will be accepted.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iAddress = Address of peek region start; given as a flat address
	\param	iNumItems = number of items to read, must be <= 16 bytes (8 16-bit items)
	\param  pArray = array of items in which data will be stored.  Size must be at least
	                     "<iNumItems> * data size" bytes long 
						 For 16-bit items, this is <iNumItems> * 2 (bytes)

	\return true if successful, false if fail.

	\warning !!! The ICD reports 16-bytes available for transfer, but the source code limits the size to 4 bytes.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_MEMORY_PEEK_WORD_F( 
		HANDLE hResourceContext, unsigned long iAddress, unsigned short iNumItems, unsigned short* pItemArray );

	/******************************************************************************/
	/**
	DMSS ICD, 3.4.3 Memory Peek Request/Response (32-bit version)
	WCDMA Diagnostic ICD, 3.5.1 Memory peek request/response

	When the DM sends a Memory Peek Request Message to the DMSS, the DMSS responds with a
	Memory Peek Response Message that contains the contents of a block of memory starting at the
	given ADDRESS and consisting of LENGTH items.
	
	The request may ask for up to 4 16-bit values.
	
	The ADDRESS is specified as a 32-bit flat address.

	This packet is protected by the DMSS Security Plan. The Security Passunsigned short Request Message
	must be sent to the DMSS successfully before this message will be accepted.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iAddress = Address of peek region start; given as a flat address
	\param	iNumItems = number of items to read, must be <= 16 bytes (4 32-bit items)
	\param  pArray = array of items in which data will be stored.  Size must be at least
	                     "<iNumItems> * data size" bytes long 
						 For 32-bit items, this is <iNumItems> * 4 (bytes)

	\return true if successful, false if fail.

	\warning !!! The ICD reports 16-bytes available for transfer, but the source code limits the size to 8 bytes.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_MEMORY_PEEK_DWORD_F( 
		HANDLE hResourceContext, unsigned long iAddress, unsigned short iNumItems, unsigned long* pItemArray );

	/******************************************************************************/
	/**
	DMSS ICD, 3.4.4 Memory Poke Request/Response  (16-bit version)
	WCDMA Diagnostic ICD, 3.5.2 Memory poke request/response

	When the DM sends a Memory Poke Request Message to the DMSS, the DMSS writes the
	data provided into its memory and responds with a Memory Poke Response Message. The
	poke request may request up to 16 BYTES. The response to a correctly formatted Memory Poke Request
	is a copy of the request.

	This packet is protected by the DMSS Security Plan. The Security Passunsigned short Request Message
	must be successfully sent to the DMSS before this message will be accepted.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iAddress = Address of peek region start; given as a flat address
	\param	iNumItems = number of items to read, must be <= 16 bytes (16 8-bit items)
	\param  pArray = array of items in which data will be stored.  Size must be at least
	                     "<iNumItems> * data size" bytes long 
						 For 8-bit items, this is <iNumItems> * 1 (bytes)
	                     The size of this array must be at least <iNumBytes> long

	\return true if successful, false if fail.

	\warning !!! The ICD reports 16-bytes available for transfer, but the source code limits the size to 4 bytes.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_MEMORY_POKE_F( 
		HANDLE hResourceContext, unsigned long iAddress, unsigned short iNumItems, unsigned char* pItemArray );

	/******************************************************************************/
	/**
	DMSS ICD, 3.4.4 Memory Poke Request/Response  (16-bit version)
	WCDMA Diagnostic ICD, 3.5.2 Memory poke request/response

	When the DM sends a Memory Poke Request Message to the DMSS, the DMSS writes the
	data provided into its memory and responds with a Memory Poke Response Message. The
	poke request may request up to 8 16-bit numbers. The response to a correctly formatted 
	Memory Poke Request is a copy of the request.

	This packet is protected by the DMSS Security Plan. The Security Passunsigned short Request Message
	must be successfully sent to the DMSS before this message will be accepted.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iAddress = Address of peek region start; given as a flat address
	\param	iNumItems = number of items to read, must be <= 16 bytes (8 16-bit items)
	\param  pArray = array of items in which data will be stored.  Size must be at least
	                     "<iNumItems> * data size" bytes long 
						 For 16-bit items, this is <iNumItems> * 2 (bytes)
	                     The size of this array must be at least <iNumBytes> long

	\warning !!! The ICD reports 16-bytes available for transfer, but the source code limits the size to 4 bytes.

	\warning None.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_MEMORY_POKE_WORD_F( 
		HANDLE hResourceContext, unsigned long iAddress, unsigned short iNumItems, unsigned short* pItemArray );

	/******************************************************************************/
	/**
	DMSS ICD, 3.4.4 Memory Poke Request/Response (32-bit version)
	WCDMA Diagnostic ICD, 3.5.2 Memory poke request/response

	When the DM sends a Memory Poke Request Message to the DMSS, the DMSS writes the
	data provided into its memory and responds with a Memory Poke Response Message. The
	poke request may request up to 4 32-bit numbers. The response to a correctly formatted 
	Memory Poke Request is a copy of the request.

	This packet is protected by the DMSS Security Plan. The Security Passunsigned short Request Message
	must be successfully sent to the DMSS before this message will be accepted.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iAddress = Address of peek region start; given as a flat address
	\param	iNumItems = number of items to read, must be <= 16 bytes (4 32-bit items)
	\param  pArray = array of items in which data will be stored.  Size must be at least
	                     "<iNumItems> * data size" bytes long 
						 For 32-bit items, this is <iNumItems> * 4 (bytes)
	                     The size of this array must be at least <iNumBytes> long

	\warning !!! The ICD reports 16-bytes available for transfer, but the source code limits the size to 8 bytes.

	\warning None.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_MEMORY_POKE_DWORD_F( 
		HANDLE hResourceContext, unsigned long iAddress, unsigned short iNumItems, unsigned long* pItemArray );

	/******************************************************************************/
	/**
	DMSS ICD, 3.4.63 Service Programming Code Request/Response
	WCDMA Diagnostic ICD, 3.5.29.3 Service Programming Code Request Message

	Sends the service programming code (SPC) to the phone.

	The SPC is a 6 digit number, which can enable certain operations, according to
	the DMSS Security Plan. 

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iSPC = array of 6 bytes, which hold the service programming code
					6 bytes of security code, in ASCII format (digits 0..9 only)
	\param	piSPC_Result = Result of the SPC
		\code
			1 = code was correct and Service Programming (SP) is unlocked
			0 = code was incorrect and SP is still locked
		\endcode

	\return true if successful, false if fail.

	\warning if the code was incorrect, the phone will time out for 10 sec
		before responding to any more requests through the serial interface.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_SPC_F( 
		HANDLE hResourceContext, unsigned char iSPC[6], unsigned char* piSPC_Result );

	/******************************************************************************/
	/**
	DMSS ICD, 3.4.124.2 Extended Build ID Response Message

	This packet is an extension to the Version Number Request/Response packet (CMD_CODE 0)

	\param piMSM_HW_Version = MSM version.  This is an extension of the MSM_VER 
	                         field from the Version Number Request/Response packet (DIAG_VER_NO_F)
	\param piMobModel = Manufacturer's mobile model number.  This is an extension of 
	                   MOB_MODEL field from the Version Number Request/Response packet 

	\param sMobSwRev = Mobile software revision string. This string is a NULL-terminated 
	                   ASCII string. If string is nonexistent, a NULL char indicates an 
					   empty string. This field is an extension of the VER_DIR field 
					   from the Version Number Request/Response packet

	                   NOTE: Enough memory must be available at this pointer location
					   to store the entire string.  Recommend a buffer of 512 bytes.

	\param sModelStr = Mobile model string.  This string is an ASCII NULL-terminated string. 
	                   If string is nonexistent, a NULL char indicates an empty string.

                       NOTE: Enough memory must be available at this pointer location
					   to store the entire string.  Recommend a buffer of 512 bytes.

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_EXT_BUILD_ID_F
		( HANDLE hResourceContext, unsigned long* piMSM_HW_Version, unsigned long* piMobModel, 
			char* sMobSwRev, char* sModelStr );

	/******************************************************************************/
	/**
	DMSS ICD, 3.4.9 Status Request/response Message

	The Status Request Message asks for current DMSS status information. 
	This information is returned in the Status Response Message.

	\param ESN = Electronic serial number. See IS-95-A 2.3.2 or J-STD-008 2.3.2.
	\param RF_mode
		\code
			Current mode of the RF hardware
			0 = Analog
			1 = CDMA cellular
			2 = CDMA PCS
			3 = Sleep mode
			4 = GPS
			5 = HDR
		\endcode
	\param MIN1_Analog = Analog MIN1 for the current NAM (valid only for analog
                         capability phones)
	\param MIN1_CDMA = CDMA MIN1 for the current NAM.
                       For IS-95-A compliance, these are set to the same value.
                       See IS-95-A 2.3.1 or J-STD-008 6.3.1. (MIN1 is equivalent
                       to IMSI_S1.)
	\param MIN2_Analog = Analog MIN2 for the current NAM (valid only for analog
                         capability phones)
	\param MIN2_CDMA = CDMA MIN2 for the current NAM
                         For IS-95-A compliance, these are set to the same value.
                         See IS-95-A 2.3.1, or J-STD-008 6.3.1. (MIN2 is equivalent
                         to IMSI_S2.)
	\param CDMA_RX_State = Current CDMA RX process state
		\code
           - 0 = Entering CDMA state
           - 1 = Sync channel state
           - 2 = Paging channel state
           - 3 = Traffic channel initialization state
           - 4 = Traffic channel state
           - 5 = Exit state
		\endcode
	\param CDMA_Good_Frames = Valid only for Sync and Paging Channel States
		\code
           1 = Good frames received since the last status request
           0 = Five or more bad frames and no good frames
               received since the last status request
           -1 = Unknown - no good or bad frames received for two
                or more status requests
		\endcode
	\param Analog_Bad_Frames = Count of bad frames (Analog only); saturates at 65535
                               This item is only valid for analog capability phones.
	\param Analog_Word_Syncs = Number of word sync sequences received in analog control
                               or voice channel modes; saturates at 65535
                               See IS-95-A 3.7.1.
                               This item is only valid for analog capability phones.
	\param Entry_Reason = Reason call processing entered
                          Use the field RF_MODE to determine which interpretation
                          of the following values to use.
                          CDMA mode (Cellular and PCS)
		\code
           0 = Normal CDMA operation
           1 = Continue previous operation
           2 = Enter offline CDMA mode
           3 = Originate call in CDMA mode

           Analog Mode
           0 = Normal analog operation
           1 = Digital-to-analog handoff
           2 = Originate call in Analog mode
           3 = Page response
           4 = Voice channel assignment
           5 = Continue previous operation
           6 = Enter offline Analog mode
		\endcode
	\param Current_Chan = Current center frequency channel for Analog or CDMA
	\param CDMA_Code_Chan = CDMA code channel (CDMA only).
                            This is invalid after any Handoff Direction Message on the
                            Traffic Channel. See IS-95-A 7.1.3.1 or J-STD-008 3.1.3
	\param Pilot_Base = Pilot PN of current cell (CDMA only)
                        See IS-95-A 7.1.3.2 or J-STD-008 3.1.3.2.
	\param SID = Current system ID
                 See IS-95-A 7.7.1.3 or J-STD-008 3.7.1.3.
	\param NID = Current network ID
                 See IS-95-A 7.7.1.3 or J-STD-008 3.7.1.3.
	\param LocaID = Current location area ID (Analog only)
                    This item is only valid for analog capability phones. See
                    IS-95-A 2.3.4.2.
	\param RSSI = Current RSSI (RF signal strength) level for analog
                  To convert to dBm/30 KHz, use -110 + (RSSI * 62/255). This
                  value is a rather rough estimate. This item is only valid for
                  analog capability phones.
	\param Power = Current mobile output power level index for analog
                  This item is only valid for analog capability phones. See
                  IS-95-A Table 2.1.2-1.

	\return true if successful, false if fail.

	\warning NOT IMPLEMENTED

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_STATUS_REQUEST_F( HANDLE hResourceContext, 
			unsigned long* ESN,
			unsigned short *RF_mode,
			unsigned long *MIN1_Analog,
			unsigned long *MIN1_CDMA,
			unsigned short *MIN2_Analog,
			unsigned short *MIN2_CDMA,
			unsigned short *CDMA_RX_State,
			unsigned char *CDMA_Good_Frames,
			unsigned short *Analog_Bad_Frames,
			unsigned short *Analog_Word_Syncs,
			unsigned short *Entry_Reason,
			unsigned short *Current_Chan,
			unsigned char *CDMA_Code_Chan,
			unsigned short *Pilot_Base,
			unsigned short *SID,
			unsigned short *NID,
			unsigned short *LocaID,
			unsigned short *RSSI,
			unsigned char *Power
			);

	/******************************************************************************/
	/**
	WCDMA ICD, 3.4.9 Status request message (cmd_code 14 of the WCDMA subsystem)

	Returns the phone status--offline, online, other

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piTMC_State = Return value of TMC state:
		\code
        0  = DIAG_STATUS_OFFLINE =  Offline
        1  = DIAG_STATUS_ONLINE  =  Online
        2  = DIAG_STATUS_LPM     =  Low power mode
        99 = DIAG_STATUS_UNKNOWN =  Unknown state
		\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_STATUS_F( HANDLE hResourceContext, unsigned char* piTMC_State );

	/******************************************************************************/
	/**
	WDMSS ICD, 3.5.24 Error Record Retrieval Request/Response
	CDMA ICD, 3.4.39 Error Record Retrieval Request/Response

	When the DMSS software encounters an error condition, it stores an error record in its
	nonvolatile memory. An error is defined uniquely by its source file and line number. Another
	invocation of an error that has already been recorded does not allocate another error record, it
	increments the count for the existing error. There are dozens of potential errors in the DMSS
	code, however, there is room in NV for 20 different error records, each of which includes the
	number of times the error has occurred, the source file and line of the location of the software
	reporting the error, and whether the error was fatal. Fatal errors cause the DMSS to reset,
	nonfatal errors do not.
	The error number (0 to 19) does not indicate anything about the errors other than the order in
	which they were recorded. It is just a 20-position array, and as a new error needs to be
	recorded, a new error record is used to record it.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param pErrorRead = Pointer to a structure (QLIB_DIAG_Err_Read) that will hold the error 
	                    read results.

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_ERR_READ_F( HANDLE hResourceContext, unsigned char *pErrorRead );

	/******************************************************************************/
	/**
	WDMSS ICD, 3.5.25 Error Record Clear Request/Response
	CDMA ICD, 3.4.40.1 Error Record Clear Request Message

	The accumulated error statistics in the DMSS can be cleared by sending an Error Record Clear
	Request Message. A single error record (identified by index, 0 to 19) or all error records
	(identified by index -1) may be cleared with a single Error Record Clear Request Message. After
	clearing the specified error record, the DMSS shall send an Error Record Clear Response
	Message to the DM.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piErrorRec = Index of error record to be cleared.  If ERROR_REC is -1 (0xFF), the DMSS 
	                   shall clear all error records. If iErrorRec is between 0 and 19 inclusive, 
					   the DMSS shall clear the error record specified by iErrorRec.

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_ERR_CLEAR_F( HANDLE hResourceContext, unsigned char iErrorRec );

	/******************************************************************************/
	/**
	Sirius custom diag command

	Sends the trigger for the security freeze step to be executed on the mobile

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_SECURITY_FREEZE_F( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	CDMA ICD, 3.4.56 Switch to Downloader Request/Response
	WCDMA ICD, 3.5.29 Switch to Downloader Request/Response

	Sending a Switch to Downloader Request Message will cause a jump to the resident
	downloader. In response, the phone sends a Switch to Downloader Response Message before
	it jumps to the resident downloader. The DMSS must be in an offline state in order to accept
	this command. 

    \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if fail.

	\warning After this command is sent, the phone will respond only to download protocol commands

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_DLOAD_F( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	Start CNV backup

	If the CNV backup has been triggered by a DIAG command, the response to the DIAG 
	command will tell the PC if the backup was successful or not (If it's SWUOTA 
	triggered F3 messages are all that can give status of CNV backup). 
	There are 2 responses to the DIAG command, first reports on the status if the 
	command has been successfully queued in CNV, and the second response reports on 
	the status of the backup. 

	NV_CNV_DIAG_BACKUP: Command sent from the DM to the DMSS to backup the NV data to CNV.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if fail.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_NV_CNV_DIAG_BACKUP_F( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	CDMA ICD, 3.4.44 DIP Switch Retrieval Request/Response

	The DM requests the software DIP switch settings by sending a DIP Switch Retrieval Request
	Message to the DMSS. The DMSS responds by sending a DIP Switch Retrieval Response
	Message to the DM that contains the current setting of the software switches. Each bit is the
	value of a single switch. The DIP switches are not true DIP switches but software-only, and
	they reset upon power cycle.

	DIP switches are highly hardware dependent and their meanings are volatile; any permanent
	configuration options will be given NV items or DIAG packets.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piSwitchValues = 16-bit bit mask of switch values

	\return true if successful, false if fail.

	\warning None.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_GET_DIPSW_F( HANDLE hResourceContext, unsigned short* piSwitchValues );
	
	/******************************************************************************/
	/**
	CDMA ICD, 3.4.45 DIP Switch Set Request/Response

	The DM changes the value of one or more of the software DIP switches by sending a DIP
	Switch Set Request Message. The DMSS responds by setting the value of the switch word to
	the value given in the DIP Switch Set Request Message, and sending a DIP Switch Set
	Response Message in return. Note that all the switches are set at once, so it is necessary to read
	the current switch settings and return the current values of all the switches that are not being
	changed.

	DIP switches are highly hardware dependent and their meanings are volatile; any permanent
	configuration switches options will be given NV items or DIAG packets.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSwitchValues = 16-bit bit mask of switch values

	\return true if successful, false if fail.

	\warning None.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_SET_DIPSW_F( HANDLE hResourceContext, unsigned short iSwitchValues );


	/******************************************************************************/
	/**
	Set/Get Dip switch for HDR subsystem (80-V1294-2 2.2.3 DIP Switch request/response)

	When the DM sends a DIP switch request message, the DMSS responds by sending a DIP switch
	response message to the DM that contains the current setting of the switches. Each bit is the value
	of a single switch. The DIP switches are reset upon power cycle.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iAction	=	Action; possible values are:
						0 = Reset DIP switch
						1 = Set DIP switch
						2 = Clear all DIP switches
						3 = Read DIP switch mask

	\param  iDipSwitchBit = DIP switch bit to set or reset (only valid if iAction = 0,1)
						   Refer to 80-V1294-2 table 2.2 for a list of implemented DIP switches.
					
	\param piDipSwitchMask = A pointer to DIP switch mask returned from DIP SWITCH response message
	
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/

	QLIB_API unsigned char QLIB_HDR_DIAG_DIP_SWITCH( HANDLE hResourceContext, unsigned char iAction, unsigned char iDipSwitchBit, unsigned long * piDipSwitchMask );


	/******************************************************************************/
	/**
	Read ESN, special case of DIAG_NV_READ_F

	Returns the value of the NV_ESN_I NV field, item #0.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piESN = Pointer to the location where the ESN will be stored

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_READ_ESN_F( HANDLE hResourceContext, unsigned long* piESN );

	/******************************************************************************/
	/**
	Get certain fields from the IS-2000 status.
	CDMA ICD 3.4.111

	The IS2000 Status Request Message asks for current DMSS status information. This information
	is returned in the IS2000 Status Response Message. 

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piBandClass = output, Band class number
	\param piCurrChan = output, current channel number
	\param piCDMA_RxState = output, reciever state
		\code
         0 = CDMA_STATE (initialization state)
         1 = SC_STATE (sync channel state)
         2 = PC_STATE (paging channel state)
         3 = TCI_STATE (traffic channel initialization state)
         4 = TC_STATE (traffic channel state)
         5 = EXIT_STATE (exit state)
		\endcode
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_IS2000_STATUS_F( 
							HANDLE hResourceContext, 
							unsigned char* piBandClass,
							unsigned short* piCurrChan,
							unsigned short* piCDMA_RxState );

	/******************************************************************************/
	/**
	Send a "GPS Configure test paraemters" message, usuall used for a "GPS Cold start," 
	which is used in the GPS PERL scripts for GPS testing.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iDeleteAlm = 1; # 1 is to delete, 0 =no action
	\param iDeleteEph = 1; # 1 is to delete, 0 =no action
	\param iDeletePos = 1; # 1 is to delete, 0 =no action
	\param iDeleteTime = 1; # 1 is to delete, 0 =no action
	\param iDeleteIono= 1; # 1 is to delete, 0 =no action
	\param iTimeUnc = 0; #Inject Time uncertainity. Only applicable if delete_time value is 0
	\param iPosUnc = 0;  #Inject Position Uncertainity. Only applicable if delete pos is 0
	\param iTimeOffset = 0; #Add a time offset. Only applicable if delete_time value is 0
	\param iPosOffset = 0;  #Add a position offset. Only applicable if delete_pos value is 0
	

	GPS PERL script being emulated is pd_coldstart.pl, the section:

		# Build a diagnostic request for GPS_SET_TEST_MODE_PARAMETERS 
		$req_testparm = new PD_COLDSTART;
		$req_testparm->{"cmd"}          = 75; #Diag Cmd 
		$req_testparm->{"id"}           = 13; #Diag ID
		$req_testparm->{"sub_cmd"}      = 18; #Diag Subsystem ID
		$req_testparm->{"delete_alm"}  = 1; # 1 is to delete, 0 =no action
		$req_testparm->{"delete_eph"}  = 1; # 1 is to delete, 0 =no action
		$req_testparm->{"delete_pos"}  = 1; # 1 is to delete, 0 =no action
		$req_testparm->{"delete_time"}  = 1; # 1 is to delete, 0 =no action
		$req_testparm->{"delete_iono"}  = 1; # 1 is to delete, 0 =no action
		$req_testparm->{"time_unc"}  = 0; #Inject Time uncertainity. Only applicable if delete_time value is 0
		$req_testparm->{"pos_unc"}  = 0;  #Inject Position Uncertainity. Only applicable if delete pos is 0
		$req_testparm->{"time_offset"}  = 0; #Add a time offset. Only applicable if delete_time value is 0
		$req_testparm->{"pos_offset"}  = 0;  #Add a position offset. Only applicable if delete_pos value is 0
		$req_testparm->{"client_id"}  = 0; #Not used


	\return true if successful, false if fail.

	\warning None.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GPS_SET_TEST_MODE_PARAMS(	
				HANDLE hResourceContext, unsigned char   iDeleteAlm,  unsigned char   iDeleteEph, 
				unsigned char   iDeletePos,  unsigned char   iDeleteTime,
				unsigned char   iDeleteIono, unsigned long iTimeUnc, 
				unsigned long iPosUnc, unsigned long iTimeOffset, unsigned long iPosOffset
							);


/******************************************************************************
						Phone async Logging
*******************************************************************************/

	/******************************************************************************/
	/**

		Set phone logging on or off

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param bEnableLogging = true to enable logging, false to disable

		\return true if call succeeds, false if it fails

		\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_SetPhoneLoggingState( HANDLE hResourceContext, unsigned char bEnableLogging );


	/******************************************************************************/
	/**

	Waits a specified amount of time for a phone log message and returns the contents to the user.
	NOTE: phone log is different from "text" logs

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param piLogSize = Pointer to number of bytes received in the log message
		\param piLogBytes = Pointer to a buffer to store the log message.
		\param iTimeout = millisecond time out 

	\return Returns true if a valid log message was found within the specified time-out.\

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_GetNextPhoneLog
				(
				HANDLE hResourceContext, 
				unsigned short* piLogSize,
				unsigned char* piLogBytes,
				unsigned long iTimeOut 
				);

	/******************************************************************************/
	/**
	  Searches the async queue for a specific exnteded debug message (text message)
	  and returns whether the message is found + the parameters of the message.

	  Extended messages are response message #121, documented in the CDMA and WCDMA 
	  ICD's.  They operate like a printf statement with text format string and 4
	  32-bit integer arguments.

	  Example:
	    string = "CEFS extraction completed with status %d"
		arg1 = 1 or 0
		arg2 = 0 always
		arg3 = 0 always
		arg4 = 0 always

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param sSearch string = NULL terminated 8-bit ASCII string to search for

		\param pbFoundMessage = output, true if the text message was found.
		\param piArg1 = output, value of the ARGS_1 field
		\param piArg2 = output, value of the ARGS_2 field
		\param piArg3 = output, value of the ARGS_3 field

	\return true if all commands are successful

	\warning The async queue be cleared as messages are searched for.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_FindDebugMessage
				(
				HANDLE hResourceContext, 
				char* sSearchString, 
				unsigned char* pbFoundMessage,
				unsigned long* piArg1,
				unsigned long* piArg2,
				unsigned long* piArg3
				);

	/******************************************************************************/
	/**
		Clear log message queue

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		
	\return true if success, false if failed.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_ClearPhoneLogQueue( HANDLE hResourceContext );

	/******************************************************************************/
	/**
		Returns the current size of the Phone Log queue

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param piLogQueueSize = Number of entries currently in the log queue

	\return Returns true if a valid log message was in the queue, false if not.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_GetPhoneLogQueueSize( HANDLE hResourceContext, unsigned long* iLogQueueSize );

	/******************************************************************************/
	/**

		Sets the phone's log mask, based on an extended log code.  An extended
		log code is one which is a sub-group of an equipment ID.  This function will overwrite
		any existing log code mask that has been setup.

		For example, for "MMGPS report measurement (LOG_CODE 0x7005)"

		The equipment ID is 7 and the item ID is 

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iEquipmentID = equipment id, as defined by the ICD documents, e.g. UMTS is code 0x0007
		\param iLogCode = log code as defined by the ICD documents, e.g. MMGPS report measurement is 0x7005
		\param iLastItem = last possible item that can be logged for the given equipment ID

		\return true if call succeeds, false if it fails

		\warning Only a small set of log messages are enabled to be handled by QMSL.  These list
		         Is defined here:
		\code
		0x1027,		// cdma2000 Paging channel SER indicator
		0x1036,		// IS801 Forward Link
		0x1037,		// IS801 RL
		0x103A,		// Factory Test Mode Forward CRC Summary
		0x108a,		// EVDO Finger Info
		0x1068,		// EVDO Air Link Summary
		0x1084,		// EVDO Forward Statistics Summary
		0x10C9,		// Rate Determination Algorithm Frame Information
		0x10D9,		// Rate Determination Algorithm Frame Information 2
		0x506C,		// GSM L1 burst metrics
		0x117C,		// FTM2
		0x119c,		// cdma2k TNG General Status
		0x119D,		// CDMA2K Srch TNG Demod Info
		0x11F5,		// LOG_MFLO_RSSI_VALUE_DYNAMIC_PARAMS_C              
		0x120C,		// LOG_MFLO_FTAP_PACKETRECORD_HEADER_PARAMS_C
		0x120D,		// LOG_MFLO_FTAP_PACKETRECORD_PARAMS_C
		0x120E,		// LOG_MFLO_FTAP_WOIS_PARAMS_C
		0x120F,		// LOG_MFLO_FTAP_LOIS_PARAMS_C
		0x1210,		// LOG_MFLO_FTAP_WID_LID_PARAMS_C
		0x121F,		// LOG_MFLO_FDM_RECORDS_C
		0x1253,		// LOG_GPS_DEMOD_SOFT_DECISIONS_C
		0x7005,		// AAGPS report measurement
		0x7001,		// AAGPS Report Measurement 3
		0x7002,		// AAGPS report clock
		0x7003,		// AAGPS report position 
		0x7008,		// AAGPS report RF status
		0x7009,		// AAGPS report frequency calibration
		0x700C,		// AAGPS Report Measurement 2
		0x700D,		// AAGPS report prescribed dwell results
		0x701E,		// AAGPS report position type 2
		0x701F,		// AAGPS report measurement type 3
		0x7009,		// LOG_UMTS_MMGPS_REP_FREQ_CALIBRATION_LOG_PACKET_C
		0x700D,		// LOG_UMTS_MMGPS_REP_DWELL_LOG_PACKET_C
		0x4000,		// WCDMA InterRAT GSM Measurement
		0x4015,		// WCDMA AGC Version 3
		0x4105,		// LOG_WCDMA_AGC_C
		0x4165,		// WCDMA AGC2
		0x4206,		// HS Decode Packet V1
		0x5064,		// interRAT WCDMA to GSM Idle mode measurement
		0x5082		// GSM L1 monitor bursts
		\endcode
				

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_SetExtendedLogCode( 
				HANDLE hResourceContext, unsigned char iEquipmentID, unsigned short iLogCode, unsigned short iLastItem );


	/******************************************************************************/
	/**

		Sets the phone's log mask, based on an extended log code.  An extended
		log code is one which is a sub-group of an equipment ID.  This function will 
		append the requested log code to the phone's exisiting log code mask.

		For example, for "MMGPS report measurement (LOG_CODE 0x7005)"

		The equipment ID is 7 and the item ID is 

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iEquipmentID = equipment id, as defined by the ICD documents, e.g. UMTS is code 0x0007
		\param iLogCode = log code as defined by the ICD documents, e.g. MMGPS report measurement is 0x7005

		\return true if call succeeds, false if it fails

		\warning Only a small set of log codes are supported by QMSL.  If the log code
		         is not part of the supported list then a failure will occur.  The
				 list is documented in the comments for QLIB_DIAG_SetExtendedLogCode()

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_AddExtendedLogCode( 
				HANDLE hResourceContext, unsigned char iEquipmentID, unsigned short iLogCode );

	/******************************************************************************/
	/**

		Sets the phone's log mask, based on multiple user-provided log code.  An extended
		log code is one which is a sub-group of an equipment ID.  This function will 
		append the requested log code to the phone's exisiting log code mask.

		For example, for "MMGPS report measurement (LOG_CODE 0x7005)"

		The equipment ID is 7 and the item ID is 0x7005

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iEquipmentID = equipment id, as defined by the ICD documents, e.g. UMTS is code 0x0007
		\param aiLogCodeList = log code as defined by the ICD documents, e.g. MMGPS report measurement is 0x7005
		\param iLogCodeListSize = number of items in the aiLogCodeList

		\return true if call succeeds, false if it fails

		\warning Only a small set of log codes are supported by QMSL.  If the log code
		         is not part of the supported list then a failure will occur.  The
				 list is documented in the comments for QLIB_DIAG_SetExtendedLogCode()

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_AddExtendedLogCodes( 
				HANDLE hResourceContext, 
				unsigned char iEquipmentID, 
				unsigned short* aiLogCodeList, 
				unsigned short iLogCodeListSize
				);

	/******************************************************************************/
	/**

		Clear all log codes for a specific equipment ID.

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iEquipmentID = equipment id, as defined by the ICD documents, e.g. UMTS is code 0x0007

		\return true if call succeeds, false if it fails

		\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_ClearAllLogCodes( 
				HANDLE hResourceContext, 
				unsigned char iEquipmentID
				);

	/******************************************************************************/
	/**

		Looks through the existing async queue for a specific log code.  If not found
		then waits (based on the timeout) for a specified log code to be captured.

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iEquipmentID = equipment id, as defined by the ICD documents, e.g. UMTS is code 0x0007
		\param iLogCode = log code as defined by the ICD documents, e.g. MMGPS report measurement is 0x7005
		\param iTimeOut_ms = millisecond timeout 
		\param pLogData = pointer to a buffer that the log data will be copied into.  All data
		                  received (except CRC) is copied into this buffer.
						  Size of each the storage buffer should be DIAG_MAX_PACKET_SIZE
		\param piLogSize = number of bytes stored in the log structure

		\return true if at least one log code was captured, false if no log was found

		\warning Only a small set of log codes are supported by QMSL.  If the log code
		         is not part of the supported list then a failure will occur.  The
				 list is documented in the comments for QLIB_DIAG_SetExtendedLogCode()

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_GetSingleLog( 
				HANDLE hResourceContext, 
				unsigned char iEquipmentID,
				unsigned short iLogCode,
				unsigned long iTimeOut_ms,
				void* pLogData,
				unsigned short* aiLogDataSize
				);

	/******************************************************************************/
	/**

		Looks through the existing async queue for a specific log code.  If not found
		then waits (based on the timeout) for a specified log code to be captured.

		Will collect logs until iNumLogs has been captured or the timeout occurs

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iEquipmentID = equipment id, as defined by the ICD documents, e.g. UMTS is code 0x0007
		\param iLogCode = log code as defined by the ICD documents, e.g. MMGPS report measurement is 0x7005
		\param piNumLogs = as an input, it is the number of logs to look for.  as an output
		                   it is the number of logs that were actually found.

		\param iTimeOut_ms = millisecond timeout 
		\param apLogData = array of pointers to buffers that the log data will be copied into.  
		                   All data received (except CRC) is copied into these buffer.
						   Size of array must be at least iNumLogs.

		\param aiLogDataSizes = output, the size of each log will be stored here

		\return true if at least one log code was captured, false if no log codes were found

		\warning Only a small set of log codes are supported by QMSL.  If the log code
		         is not part of the supported list then a failure will occur.  The
				 list is documented in the comments for QLIB_DIAG_SetExtendedLogCode()

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_GetMultipleLogs( 
				HANDLE hResourceContext, 
				unsigned char iEquipmentID,
				unsigned short iLogCode,
				unsigned short* piNumLogs,
				unsigned long iTimeOut_ms,
				void* apLogData[],
				unsigned short aiLogDataSizes[]
				);

	/******************************************************************************/
	/**
		CDMA ICD, 3.4.94 Event Report Control Request/Response
		WCDMA ICD, 3.5.35 Event Report Control Request/Response

		When the DM sends an Event Report Control Request Message, the DMSS responds with an
		Event Report Control Response Message. This command is used to configure the service, as
		well as for the service to send information. See Section 3.4.95 for information on the Event
		Report Message.

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param bOperationSwitch = Turn scheduling on or off: 0 = Off, 1 = On

		\return true if call succeeds, false if it fails

		\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_EventReportControl( HANDLE hResourceContext, unsigned char bOperationSwitch );

	/******************************************************************************/
	/**
		Sets or clears an individual bit in the Event Mask by calling QLIB_DIAG_SetEventMaskBits()

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iMaskID = bit position in the event mask, which will be turned on or off
		\param bMaskState = 0 = Off-Disabled, 1 = On-Enabled

		\return true if call succeeds, false if it fails

		\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_SetEventMaskBit( 
					HANDLE hResourceContext, 
					unsigned short iMaskID,
					unsigned char bMaskState);

	/******************************************************************************/
	/**
		Sets or clears the entire Event Mask.

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param bMaskState = 0 = Off-Disable all events, 1 = On-Enable all events

		\return true if call succeeds, false if it fails

		\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_SetEntireEventMaskState( 
					HANDLE hResourceContext, 
					unsigned char bMaskState);

	/******************************************************************************/
	/**

	Returns the next event on the event queue. 

	See QLIB_DIAG_GetEventQueueSize() for more information about the Event queue.

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param pEvent = Pointer to a structure QMSL_Event_Element_Struct.

	\return Returns true if an event was in the queue, false if not.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_GetNextEvent
				(
				HANDLE hResourceContext,
				void* pEvent
				);

	/******************************************************************************/
	/**

	Waits a specified amount of time for a phone event and returns the contents to the user.

	See QLIB_DIAG_GetEventQueueSize() for more information about the Event queue.

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iEventID = ID of event to look for.  For a wild card event ID, use QMSL_EVENT_ID_WILDCARD (0xFFFF)
		                  For a full list of events, see the AMSS software header file, 
						  services\diag\event_defs.h
		\param pEvent = Pointer to a structure QMSL_Event_Element_Struct.
		\param iTimeout_ms = millisecond time out 

	\return Returns true if a valid log message was found within the specified time-out.\

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_WaitForEvent
				(
				HANDLE hResourceContext,
				unsigned short iEventID,
				void* pEvent,
				unsigned long iTimeOut_ms
				);

	/******************************************************************************/
	/**
		Clear Event queue.  
		
		See QLIB_DIAG_GetEventQueueSize() for more information about the Event queue.

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		
	\return true if success, false if failed.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_ClearEventQueue( HANDLE hResourceContext );

	/******************************************************************************/
	/**
		Returns the current size of the Event queue.  The an event queue element
		is added for each event received from the mobile phone.  One "event response" packet
		may contain multiple events packets.  QMSL will parse the "event response" packet
		and extract each event, then place each event into the queue.

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param piEventQueueSize = Number of entries currently in the Event queue

	\return Returns true if a valid log message was in the queue, false if not.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_GetEventQueueSize( HANDLE hResourceContext, unsigned long* piEventQueueSize );

	/******************************************************************************/
	/**
		Sets or clears multiple bits in the Event Mask by calling these functions:

		1) CDMA ICD, 3.4.129 Event Get Mask Request/Response
		2) CDMA ICD, 3.4.130 Event Set Mask Request/Response

		Get Mask:
		
		The Event Get Mask Request Message is sent by the DM to the DMSS to get the current event
		mask.
		Event Mask is a byte array where each bit denotes an event ID’s configuration. A bit value of 1
		specifies that the item is enabled. A bit value of 0 specifies that the item is disabled. The Event
		mask representation is similar to the Log mask representation described in Section 3.4.115.
		EVENT_LAST_ID is the last event ID supported. The Event mask array is of size
		(EVENT_LAST_ID / 8 + 1).

		NOTE The first byte at index zero (i.e., left-most) in the event mask array is the least significant byte;
		last byte (i.e., right-most) is the most significant byte. Within a byte, the right-most bit is the
		least significant bit and the left-most bit is the most significant bit.

		Set Mask:
		The Event Set Mask Request Message is sent by the DM to the DMSS to set the event mask.
		NUM_BITS specifies the mask length of the user. If the number of bits specified in the set mask
		command is smaller than the total number of bits supported, then only the specified bits are set
		starting from the beginning; the remaining bits are not changed.

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param aiMaskID_List = array of mask ID's, which are bit position in the event mask. 
		\param iMaskID_ListSize = number of mask ID's in aiMaskID_List
		\param bMaskState = 0 = Off-Disabled, 1 = On-Enabled the corresponding bit in the event mask

		\return true if call succeeds, false if it fails

		\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_SetEventMaskBits( 
					HANDLE hResourceContext, 
					unsigned short* aiMaskID_List,
					unsigned char iMaskID_ListSize,
					unsigned char bMaskState);

	/******************************************************************************/
	/**
		3.5.45.2 Logging Configuration Response Message, CMD_CODE=115

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iOperation = OPERATION parameter of the 3.5.45.1 Logging Configuration 
		                    Request Message

		\param piOperationData = OPERATION_DATA parameter of the 3.5.45.1 Logging 
		                         Configuration Request Message

		                         If the operation data is returned from the message, this buffer
								 will be overwritten

		\param piOperationDataSize = number of bytes to be sent in the piOperationData buffer

		                         If the operation data is returned from the message, this number
								 will be updated with the size of the operation data buffer that was
								 returned.

		\param iStatus = STATUS returned in the 3.5.45.2 Logging Configuration 
		                 Response Message

	\return Returns true if a valid log message was found within the specified time-out.

		\warning Only a small set of log codes are supported by QMSL.  If the log code
		         is not part of the supported list then a failure will occur.  The
				 list is documented in the comments for QLIB_DIAG_SetExtendedLogCode()
	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_SetExtendedLogMask( HANDLE hResourceContext, unsigned long iOperation, unsigned char* piOperationData,  unsigned short* piOperationDataSize);

	/******************************************************************************/
	/**
		WCDMA ICD, 3.5.17 Message Request/Response, CMD_CODE=31

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iMessageLevel= Minimum message severity level
		\code
           0000 = all messages (MSG_LVL_LOW)
           0001 = medium and above (MSG_LVL_MED)
           0002 = high and above (MSG_LVL_HIGH)
           0003 = error and above (MSG_LVL_ERROR)
           0004 = fatal error only (MSG_LVL_FATAL)
           00FF = no messages (MSG_LVL_NONE)
		\endcode


	\return Returns true if a valid log message was found within the specified time-out.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_SetMessageLevel( HANDLE hResourceContext, unsigned short iMessageLevel );

	/******************************************************************************/
	/**
		CDMA ICD, 3.4.125 Extended Message Configuration Request/Response

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

		\param iSSID   
			See section 3.4.125.2 of the CDMA ICD for description of SSID's, 
			basically they are unique identifiers assigned to an internal client 
			of the debug message service.

			For example, sub system 6000 is a hex value of 0x1770

			Sub systems ID's can be identified as the value on the left side of the message ID 
			listed in QXDM.  The right side of the message id is the bit number, which is the
			exponent of 2, used to determine the RT_MASK value.
			
			For example 0003/04 is a subsystem value of 3 a RT_MASK of 16, which is (2^4)
			Another example 6002/01 is a subsystem value of 0x1772 and a RT mask of 1, which is (2^1)


		\param iRT_MASK = Bit mask to determine what message levels are enabled
			\code
                Bit 1 (0x01) = Low   
                Bit 2 (0x02_ = Medium
                Bit 3 (0x04) = High
                Bit 4 (0x08) = Error
                Bit 5 (0x10) = Fatal
                ...
                There can be numerous more sub categories, depending on the SSID
                
                0xFFFFFFFF = all messaages for the SSID
			\endcode
			
	

	\return Returns true if command succeeded, false if failed.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_SetMessageFilter( HANDLE hResourceContext, unsigned short iSSID, unsigned long iRT_MASK );

	/******************************************************************************/
	/**

	  Fills in the ExtendedMessage structure with data from an extended log message
	  buffer.  This function is necessary because the extended messages are not always
	  the same size, depending on how many arguments are passed, and how long the
	  file name and format string fields are.

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param pBuffer = byte buffer to be parsed.  The referenced buffer must contain a valid extended message.
		\param pExtendedMessage = pointer to the ExtendedMessage structure that will be filled in based
		                          upon the byte buffer.  This buffer is cast to a unsigned char* so that
								  the ExtendedMessage structure does not need to be defined for all
								  users of the library.
	
	\return Returns true if a valid extended log message was found in the pBuffer
	        data buffer, and then parsed with no invalid values.


	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_ParseExtendedMessage
				(
				HANDLE hResourceContext, 
				unsigned char* pBuffer,
				unsigned char* pExtendedMessage
				);

	/******************************************************************************/
	/**

	  Parses all messages the current Async queue and places a summary report on 
	  the console and/or the QLIB text log file.

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iLogType = Log type bit mask, defined in QLib_Defines.h, for example, LOG_IO
		\param bSendToConsole = true to show data on the console, false to not show on console
		\param bSendToQlibLogFile = true to send report to QLIB text log file, false to not send
		\param bShowBinaryData = true to show the binary data of the async packet, false to not show
		                          upon the byte buffer.  
	\return 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DumpAsyncMessageToTextLog
				(
				HANDLE hResourceContext, 
				unsigned int iLogType,
				unsigned char bSendToConsole,
				unsigned char bSendToQlibLogFile,
				unsigned char bShowBinaryData
				);

	/******************************************************************************/
	/**

	  Starts a millisecond-resolution timer and returns a handle to the timer so the
	  user can query the timer value later.

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return output, the unique ID that will be used to identify the timer later

	*******************************************************************************/
	QLIB_API unsigned long QLIB_StartTimer( HANDLE hResourceContext );

	/******************************************************************************/
	/**

	  Gets the elapsed time of a timer
	
		\param iTimerHandle = input, the unique ID that was returned from StartTimer()

	\return the elapsed time

	*******************************************************************************/
	QLIB_API unsigned long QLIB_GetTimerElapsed( unsigned long iTimerHandle );

	/******************************************************************************/
	/**

	  Prints  a millisecond-resolution timer to the text log.
	
		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iTimerHandle = input, the unique ID that was returned from StartTimer()
		\param sLogText = input, Text printed log file, preceding the the timer value.

	\return the elapsed time

	*******************************************************************************/
	QLIB_API unsigned long QLIB_PrintTimer( HANDLE hResourceContext, unsigned long iTimerHandle, char* sLogText );

	/******************************************************************************/
	/**

	  Stops and deletes the millisecond-resolution timer associated with the user
	  provided timer handle
	
		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iTimerHandle = input, the unique ID that was returned from StartTimer()

	\return the elapsed time

	*******************************************************************************/
	QLIB_API unsigned long QLIB_StopTimer( HANDLE hResourceContext, unsigned long iTimerHandle );

	/******************************************************************************/
	/**

	Adds a user defined message to the text log
	
		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iLogType = Log type bit mask, defined in QLib_Defines.h, for example, LOG_IO
		\param sUserMessage = message to send to the log
		\param bUpdateConsole = true to update the console, false for no update

	\return unsigned, integer value of the hex string

	*******************************************************************************/
	QLIB_API unsigned char QLIB_AddUserMessageToTextLog
		( 
		HANDLE hResourceContext, 
		unsigned int iLogType, 
		char* sUserMessage, 
		unsigned char bUpdateConsole  
		);

	/******************************************************************************/
	/**

	Adds the text version of user binary async message to the text log
	
		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iLogType = Log type bit mask, defined in QLib_Defines.h, for example, LOG_IO
		\param pMessage = pointer to the binary data containing the message
		\param bUpdateConsole = true to update the console, false for no update

		\return unsigned, integer value of the hex string

	*******************************************************************************/
	QLIB_API unsigned char QLIB_AddUserAsyncLogMessageToTextLog( 
		HANDLE hResourceContext, unsigned int iLogType, unsigned char*pMessage, 
		unsigned char bSendToQlibLogFile, unsigned char bUpdateConsole   );

	/******************************************************************************/
	/**

	Parse a user defined file and add all of the async and sync response messages to the
	command/async queues.  This will put the library into a "playback" mode.

	When commands are called in the future, they will not actually be sent to the phone,
	instead the responses will be retrieved from the queues that were populated from
	the log files.
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param sLogFile = File name of the the QLIB log file to parse
	\param bLogIsQpstMode = true to indicate the log was taken in QPST mode.  
	                        In this mode, messages do not have CRC's.  If the file is 
							not in QPST mode, then CRC's are checked and processed.

	\return true if file is found and response data is added to the playback queue

	\warning the command and async queues will be cleared before messages are added.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_StartLogFilePlaybackMode( 
		HANDLE hResourceContext, const char* sLogFile, unsigned char bLogIsQpstMode  );

	/******************************************************************************/
	/**

	Indicates that log file playback should be stopped.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	
	\return always true

	\warning the command and async queues will be cleared.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_StopLogFilePlaybackMode( HANDLE hResourceContext   );


/******************************************************************************
						Phone sync (command) Logging
*******************************************************************************/

	/******************************************************************************/
	/**
		Clear command response queue.  This is a queue of all messages that
		are not asynchronous log/debug/event types

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		
	\return true if success, false if failed.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_ClearCommandResponseQueue( HANDLE hResourceContext );

	/******************************************************************************/
	/**
		Returns the current size of the Command Responsequeue

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param piCmdRspQueueSize = Number of entries currently in the log queue

	\return Returns true if a valid message was in the Command Response queue, false if not.

	\warning .
	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_GetCommandResponseQueueSize( 
		HANDLE hResourceContext, unsigned long* piCmdRspQueueSize );

	/******************************************************************************/
	/**

	  Waits a specified amount of time for a command response message and returns the 
	  contents to the user directly.

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param piSize = Pointer to number of bytes received in the command response message
		\param piBytes = Pointer to a buffer to store the command response message.
		\param iTimeout = millisecond time out 

	\return Returns true if a valid log message was found within the specified time-out.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_GetNextCommandResponse
				(
				HANDLE hResourceContext, 
				unsigned short* piSize,
				unsigned char* piBytes,
				unsigned long iTimeOut
				);

/******************************************************************************
						Keypad Diag
*******************************************************************************/

	/******************************************************************************/
	/**
	Diagnostic Keypad function

		This function starts the capturing of key presses by configuring the 
		phone's extended message parameters to look for the "Applicaitons Default-high"
		set of messages.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_KeyPress_StartCapture( HANDLE hResourceContext  );

	/******************************************************************************/
	/**
	Diagnostic Keypad function

		This function stops the capturing of key presses by configuring the 
		phone's extended message parameters to ignore the "Applicaitons Default-high"
		and "Legacy/Error" sets of messages.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_KeyPress_StopCapture( HANDLE hResourceContext  );

	/******************************************************************************/
	/**
	Diagnostic Keypad function

		This function clears the "key press" queue, which is a list of log messages
		that have been captured, indicating key press events

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_KeyPress_ClearQueue( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	Diagnostic Keypad function

		This function clears the "key press" queue, which is a list of log messages
		that have been captured, indicating key press events

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piKeypressQueueSize = Number of entries currently in the Keypress Queue

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_KeyPress_GetQueueSize ( HANDLE hResourceContext, unsigned long* piKeypressQueueSize);


	/******************************************************************************/
	/**
	Diagnostic Keypad function

		This function returns the next key press event in the KeyPress queue.
		
		The KeyPress queue us determined by capturing log messages from the
		phone and locating certain key press event messages.

		When this function is called, the first key press event message to be received 
		is returned and then that message is removed from the queue.


	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piKeypressID = Key press identifier, according to the KeyPressID enumeration
	\param piKeyPressEvent = Key press event type, according to the KeyPressEvent enumeration

	\return true if successful, false if fail or if KeyPress queue is empty.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_KeyPress_GetNextKeyEvent
		( HANDLE hResourceContext, unsigned short* piKeyPressID, unsigned char* piKeyPressEvent );

	/******************************************************************************/
	/**
	Diagnostic Keypad function

		This function clears the keypress map, which correlates 16-bit hardware keypress
		ID's to 8-bit software ID's.

		After this function is called, keypress map entries must be specified using
		the DIAG_KeyPress_AddMapEntry() operation, to add the desired mapping entries.

		Before this function is called, a default (hard-coded) set of mapping entries 
		will be used in the library.
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if fail or if KeyPress queue is empty.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_KeyPress_ClearMap( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	Diagnostic Keypad function

		This function clears the keypress map, which correlates 16-bit hardware keypress
		ID's to 8-bit software ID's.

		After this function is called, keypress map entries must be specified using
		the DIAG_KeyPress_AddMapEntry() operation, to add the desired mapping entries.

		Before this function is called, a default (hard-coded) set of mapping entries 
		will be used in the library.
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iHW_ID  = Hardware ID of the keypress map entry

	\param iKeypressID = Key press software identifier, according to the KeyPressID enumeration
	                     Normally, these would map into the KeyPressID enumeration, but
						 for flexibility this data type is an unsigned 8-bit number,
						 so that values beyond the enermation list can be used.

	\return true if successful, false if fail or if KeyPress queue is empty.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_KeyPress_AddMapEntry
		( HANDLE hResourceContext, unsigned short iHW_ID, unsigned char iKeyPressSW_ID );

	/******************************************************************************/
	/**
	Diagnostic Keypad function

		This function sets up the strings used to detect a keypress up/down in 
		F3 messages

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param sKeyUpString = the text string that will be sent when a key is released
	                      (allowed up).  Only the porition of the string that is
						  constant for every key should be shown.  

                          For example "AEEKey_RemovePressed: " 

						  Following  this string, the F3 messages should have the hex 
						  value of the hardware ID for the key press.  For example,
						  "AEEKey_RemovePressed: 0x0327" would indicate key with the
						  HW id of 0x0327.
					


	\param sKeyDownString = the text string that will be sent when a key is pressed.  
	                      Only the porition of the string that is constant for every 
						  key should be shown.  

                          For example "AEEKey_AddPressed: " 

						  Following  this string, the F3 messages should have the hex 
						  value of the hardware ID for the key press.  For example,
						  "AEEKey_AddPressed: 0x0327" would indicate key with the
						  HW id of 0x0327.
		
	\param iSSID  = the sub-system ID of the Message that needs to be activated in order
	                to receive key press messages.  Usually this is a value of 0 for 
					the "legacy" category.  See the function DIAG_SetMessageFilter() for
					more information.

	\param iRT_MASK = the real time mask of the subsystem message.  The typical value
	                  is 8, which corresponds to the "Error" catgory of message.  These
					  messsages are not necessarily errors--it is  just a message category.

					  See the function DIAG_SetMessageFilter() for more information.

	\return true if successful, false if failiure.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_KeyPress_ConfigureKeyDetect
		 ( HANDLE hResourceContext, char* sKeyUpString, char* sKeyDownString, unsigned short iSSID, unsigned long iRT_MASK  );

/******************************************************************************
					Handset Diag
*******************************************************************************/

	/******************************************************************************/
	/**
	Handset Diagnostic ICD, 3.1 HS_DISPLAY_GET_PARAMS_CMD Request/Response
	
	  This command is used to request device-specific information (display).

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iDisplayID = input, the display # to get information for, 0 = primary, 1 = secondary

		\param piDisplayHeight = output, # of veritcal pixels  (description differs from ICD document rev B)
		\param piDisplayWidth = output, # of horizontal pixels (description differs from ICD document rev B)
		\param piPixelDepth = output, bits per pixel
		\param piPanelOrientation = output, 0 - Portrait, 1 - Landscape


	\return true if response packet was successfully returned, false if fail for any reason.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DIAG_HS_DISPLAY_GET_PARAMS_CMD( 
					HANDLE hResourceContext, 
					unsigned char iDisplayID, unsigned short* piDisplayHeight, unsigned short* piDisplayWidth, 
					unsigned char* piPixelDepth, unsigned char* piPanelOrientation
					);

	/******************************************************************************/
	/**
	Handset Diagnostic ICD, 3.3 HS_DISPLAY_CAPTURE_DELAYED_CMD 2 Request/Response
	
		This command requests the target to capture the display buffer region specified by
		[(STARTING_ROW, STARTING_COLUMN),(HEIGHT,WIDTH)] coordinates to a file. The
		file is written in Windows® BMP 24-bit format to the EFS. Specifying [(0,0),(0xFFFF,0xFFFF)] would
		7 capture the whole display.

		This version uses the Diagnostic subsystem 2, so diag is not blocked while the
		command executes.  On the QLIB side, the function will block, and not return
		until the embedded side has completed.

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iDisplayID = input, the display # to get information for, 0 = primary, 1 = secondary

		\param iStartingRow = input, Upper left-hand horizontal coordinate

		\param iStartingCol = input, Upper left-hand vertical coordinate

		\param iHeight = input, Number of rows from the STARTING_ROW; according to BMP format, 
		                        this must be a multiple of 4
		\param iWidth = input, Number of columns from the STARTING_COLUMN; according to BMP format, 
		                       this must be a multiple of 4

		\param sFileName = input, NULL-terminated name of file to be written to in the EFS;
		                   after the capture is completed successfully, this file will
		                   be in 24-bit Windows BMP format

	\return true if response packet was successfully returned, false if fail for any reason.

	\warning Not Currently Implemented

	*******************************************************************************/
	QLIB_API unsigned char QLIB_HS_DISPLAY_CAPTURE_DELAYED_CMD
					( 
					HANDLE hResourceContext, 
					unsigned char iDisplayID, unsigned short iStartingRow, unsigned short iStartingCol, 
					unsigned short iHeight, unsigned short iWidth, char* sFileName
					);


/******************************************************************************
						Commands for multiple FTM modes
*******************************************************************************/

	/******************************************************************************/
	/**

	Factory Test Mode WCDMA Commands 3.2.1.9, Set mode

	Factory Test Mode GSM Commands 3.5, Set mode 

	Factory Test Mode 1x RF Commands 3.2.11 (CL93-V5419-1)Set mode 

	Factory Test Mode RF Commands 3.3.1.10 (CL93-V4168-1) Set mode

	This function sets the RF operating mode of the mobile. It initializes all of the necessary 
			hardware to place the mobile in the requested RF mode.

	  

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  _eNewMode  = specifies the mode that the phone is to enter. 
	          FTM_RF_Mode_Enum is the definitive list of available modes.

	          The following mode is available:
	\code
		PHONE_MODE_FM        = 1      (FM)
		PHONE_MODE_GPS       = 3      (GPS)
		PHONE_MODE_GPS_SINAD = 4      (GPS SINAD)
		PHONE_MODE_CDMA_800  = 5      (CDMA 800)
		PHONE_MODE_CDMA_1900 = 6      (CDMA 1900)
		PHONE_MODE_CDMA_1800 = 8      (CDMA 1800)
		PHONE_MODE_J_CDMA    = 14     (JCDMA)
		PHONE_MODE_CDMA_450  = 17     (CDMA 450)
		PHONE_MODE_IMT       = 19     (CDMA IMT)

		PHONE_MODE_WCDMA_IMT   =9      (WCDMA IMT, Band I)
		PHONE_MODE_GSM_900     =10     (GSM 900)
		PHONE_MODE_GSM_1800    =11     (GSM 1800)
		PHONE_MODE_GSM_1900    =12,    (GSM 1900)
		PHONE_MODE_WCDMA_1900A =15,    (WCDMA 1900 A, Band II Add)
		PHONE_MODE_WCDMA_1900B =16,    (WCDMA 1900 B, Band II Gen)
		PHONE_MODE_GSM_850     =18,    (GSM 850)
		PHONE_MODE_WCDMA_800   =22,    (WCDMA 800, Band V Gen)
		PHONE_MODE_WCDMA_800A  =23,    (WCDMA 800, Band V Add)
		PHONE_MODE_WCDMA_1800  =25,    (WCDMA 1800, Band III)
		PHONE_MODE_WCDMA_BC4   =28,    (WCDMA BC4-used for both Band IV Gen and Band IV Add)
		PHONE_MODE_WCDMA_BC8   =29,    (WCDMA BC8, Band VIII)
		PHONE_MODE_MF_700      =30,    (MediaFLO)
		PHONE_MODE_WCDMA_BC9   =31,    (WCDMA BC9 (1750MHz & 1845MHz), Band IX)
		PHONE_MODE_CDMA_BC15   =32     (CDMA Band Class 15)

		PHONE_MODE_MAX         =255    (Last possible value, not a valid mode)

	\endcode 

	\return true if successful, false if fail.

	\warning Must be called before any other FTM RF commands.
	\warning,  If the new mode is a GSM RF mode, a delay will be issued after the command
	is successfully sent to the mobile.  The duration of the delay is based on the
	value of delay type "QMSL_Timeout_Delay_GSM_RF_Mode" which can be controled by 
	QLIB_ConfigureTimeOut() and read by QLIB_GetTimeOut()


	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_MODE( HANDLE hResourceContext, short eNewMode );

	/******************************************************************************/
	/**

	This function returns the command mode (59, 75 or -1 (invalid));

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iCommandMode     = Detected command mode.  The value can be 59 (used in MSM6000/6025/6050),
														75, or -1 (invalid)

	\return true if successful, false if fail.

	*******************************************************************************/


	QLIB_API unsigned char QLIB_FTM_DETECT_COMMAND_CODE( HANDLE hResourceContext, short* iCommandMode );

	/******************************************************************************/
	/**

	This function sets the command code (59 or 75);

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param eCommandMode	    = Command code (59 or 75)
		
	Note: The library uses command code 75 by default. MSM6000/6025/6050 uses commmand code 59.
	For MSM6000/6025/6050, this function should be called prior to other FTM functions

	\return true if successful, false otherwise

	*******************************************************************************/

	QLIB_API unsigned char QLIB_FTM_SET_COMMAND_CODE( HANDLE hResourceContext, short eCommandCode );

	/******************************************************************************/
	/**

	This function gets the command code (59 or 75) used by the handle

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param eCommandMode	    = Command code (59 or 75)
		
	\return true if successful, false otherwise

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GET_COMMAND_CODE( HANDLE hResourceContext, short* eCommandCode );

	/******************************************************************************/
	/**

	This function sets the system mode ID;

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param eModeId = Mode ID
	
	The Mode IDs are from the FTM_Mode_Id_Enum in QLib_Defines.h
	\code
	FTM_MODE_ID_CDMA_1X     = 0,		//!<' RF CDMA 1X mode - RX0
	FTM_MODE_ID_WCDMA       = 1,		//!<' RF WCDMA mode
	FTM_MODE_ID_GSM         = 2,		//!<' RF GSM Mode
	FTM_MODE_ID_CDMA_1X_RX1 = 3,		//!<' RF CDMA 1X mode - RX1
	FTM_MODE_ID_BLUETOOTH   = 4,		//!<' Bluetooth
	FTM_MODE_ID_CDMA_1X_CALL= 7,		//!<' CALL CDMA 1X mode 
	FTM_MODE_ID_LOGGING     = 9,		//!<' FTM Logging
	FTM_MODE_ID_AGPS        = 10,		//!<' Async GPS
	FTM_MODE_ID_PMIC        = 11,		//!<' PMIC FTM Command
	FTM_MODE_GSM_BER        = 13,		//!<' GSM BER
	FTM_MODE_ID_AUDIO       = 14,		//!<' Audio FTM
	FTM_MODE_ID_CAMERA      = 15,		//!<' Camera
	FTM_MODE_WCDMA_BER      = 16,		//!<' WCDMA BER
	FTM_MODE_ID_GSM_EXTENDED_C = 17,	//!<' GSM Extended commands
	FTM_MODE_CDMA_API_V2    = 18,		//!<' CDMA RF Cal API v2
	FTM_MODE_ID_MF_C        = 19,		//!<' MediaFLO
	FTM_MODE_RF_COMMON      = 20,		//!<' RF Common
	FTM_MODE_WCDMA_RX1      = 21,		//!<' WCDMA Diversity Rx (RX1)
	FTM_MODE_ID_PRODUCTION  = 0x8000,	//!<' Production FTM
	FTM_MODE_ID_LTM         = 0x8001	//!<' LTM
	\endcode	
	Note: This function sets

	\return true if successful, false otherwise

	*******************************************************************************/

	QLIB_API unsigned char QLIB_FTM_SET_MODE_ID( HANDLE hResourceContext, short eModeId );

	/******************************************************************************/
	/**
	Factory Test Mode WCDMA Commands 3.2.1.1 Set PDM

	Factory Test Mode 1x RF Commands 3.2.2 (CL93-V5419-1)Set mode 

	Factory Test Mode RF Commands 3.3.1.1 (CL93-V4168-1) Set mode

	This command sets the value of the PDM, based on PDM_ID.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iPDMtype  = represents the PDM to be set.
		\code
            WCDMA mode values
              2 - Tx AGC Adjust PDM
              4 - Trk Lo Adjust PDM

            GSM mode values
              0 - Trk Lo Adjust PDM

            CDMA mode values
              2 - Tx AGC Adjust PDM
		\endcode

	\param	iPDMvalue = 
		\code
            For WCDMA mode Tx AGC Adjust PDM:
              PDM values from 0 to 511
                 0   - lowest AGC voltage
                 511 - highest AGC voltage

            For GSM mode values:
               PDM values from 0 to 4096

            For CDMA mode values:
               PDM values from 0 to 511
               0   - highest AGC voltage
               511 - lowest AGC voltage

            For FM mode values:
               PDM values from 0 to 511
               0   - lowest AGC voltage
              511 - highest AGC voltage
		\endcode

	\return true if successful, false if fail.

	*******************************************************************************/
	
	QLIB_API unsigned char QLIB_FTM_SET_PDM
		( HANDLE hResourceContext, unsigned short iPDMtype, unsigned short iPDMvalue );

	/******************************************************************************/
	/**
	This function is the same as QLIB_FTM_SET_PDM(), except the iPDMvalue is signed
	for this version of the function.  This is applicable to the GSM VCTCXO PDM control.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iPDMtype  = represents the PDM to be set.
		\code
            WCDMA mode values
              2 - Tx AGC Adjust PDM
              4 - Trk Lo Adjust PDM

            GSM mode values
              0 - Trk Lo Adjust PDM

            CDMA mode values
              2 - Tx AGC Adjust PDM
		\endcode

	\param	iPDMvalue = 
		\code
            For WCDMA mode Tx AGC Adjust PDM:
              PDM values from 0 to 511
                 0   - lowest AGC voltage
                 511 - highest AGC voltage

            For GSM mode values:
               PDM values from -4096 to 4096

            For CDMA mode values:
               PDM values from 0 to 511
               0   - highest AGC voltage
               511 - lowest AGC voltage

            For FM mode values:
               PDM values from 0 to 511
               0   - lowest AGC voltage
              511 - highest AGC voltage
		\endcode

	\return true if successful, false if fail.

	*******************************************************************************/
	
	QLIB_API unsigned char QLIB_FTM_SET_PDM_signed
		( HANDLE hResourceContext, unsigned short iPDMtype, short iPDMvalue );

	/******************************************************************************/
	/**
	Factory Test Mode WCDMA Commands 3.2.1.12 Set TX on

	Factory Test Mode GSM Commands 3.3 Set Tx ON

	Factory Test Mode 1x RF Commands 3.2.14 (CL93-V5419-1)Set Tx on 

	Factory Test Mode RF Commands 3.3.1.13 (CL93-V4168-1) Set Tx on
	
		CDMA:  This command turns on the phone.s transmit chain (including the PA) and causes a CDMA
		reverse-link waveform to be generated on the current channel. All necessary encoder hardware
		initialization for transmitting is handled by this function.

		WCDMA: This command turns on the phone’s transmit chain (including the PA) and causes a WCDMA
		uplink waveform or CW to be generated on the current channel. All necessary encoder hardware 
		initialization for transmitting is handled by this function.

	
		GSM:This command turns on the phone’s transmit chain (including the PA) and causes a GSM uplink
		burst to be generated on the current channel. The data that is transmitted and other parameters of
		transmission are set up by the Set Transmit Burst API (see Section 3.2.1.10). 
		
		Hence, the Set Transmit Burst API must be called at least once after a mode change (to one 
		of the GSM modes) and prior to the execution of the Set TX ON API.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\warning GSM: Set Transmit Burst must be setup before calling this function.


	\return true if successful, false if fail.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_TX_ON( HANDLE hResourceContext );


	/******************************************************************************/
	/**
	Factory Test Mode WCDMA Commands 3.2.1.11 Set TX off

	Factory Test Mode GSM Commands 3.4 Set Tx OFF

	Factory Test Mode 1x RF Commands 3.2.13 (CL93-V5419-1)Set Tx off 

	Factory Test Mode RF Commands 3.3.1.12 (CL93-V4168-1) Set Tx off

	This command turns off the phone’s transmit chain.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if fail.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_TX_OFF( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	Factory Test Mode WCDMA Commands 3.2.1.8 Set LNA range

	Factory Test Mode GSM Commands 3.7 Set LNA range

	Factory Test Mode 1x RF Commands 3.2.10 (CL93-V5419-1)Set LNA range

	Factory Test Mode RF Commands 3.3.1.9 (CL93-V4168-1) Set LNA range

	This command sets the LNA range state machine to the specified state.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iLNArange = 

				CDMA: the binary state of the LNA range state machine:
							0 - LNA_R0 - highest gain,
							1 - LNA_R1,
							2 - LNA_R2,
							3 - LNA_R3 
							4 - LNA_R4 - lowest gain 

				The number of possible gain states varies in different RF chipsets
				Please refer to relevant NV documents for the number of possible gain states. 

				WCDMA: the binary state of the LNA range state machine:
							0 - LNA_R0 - highest gain,
							1 - LNA_R1,
							2 - LNA_R2,
							3 - LNA_R3 - lowest gain

				GSM: iLNArange = desired LNA range (0-3), 0 is highest gain, 3 is lowest gain

	\return true if successful, false if fail.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_LNA_RANGE( HANDLE hResourceContext, unsigned char iLNArange);

	/******************************************************************************/
	/**
	Factory Test Mode WCDMA Commands 3.2.1.6 Set RF channel

	Factory Test Mode GSM Commands 3.6 Set channel

	Factory Test Mode 1x RF Commands 3.2.7 (CL93-V5419-1)Set channel

	Factory Test Mode RF Commands 3.3.1.6 (CL93-V4168-1) Set channel

	 CDMA/WCDMA: This command tunes the RF synthesizers to the frequency defined by the channel parameter,
			which is based on the band class from the Set Mode command. Set Mode must be called first.

			GSM: This command tunes the RF synthesizers to the frequency defined by the ARFCN parameter,
			which is based on the band class from the Set Mode command. Set mode must be called first.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iChannel = 
	
					CDMA:	desired channel that is used
							PHONE_MODE_CDMA_800 :	1-799, 991-1023
							PHONE_MODE_CDMA_1900:	0 - 1199
							PHONE_MODE_CDMA_1800:	0 - 599
							PHONE_MODE_J_CDMA:		1-799, 801-1039, 1041-1199, 1201-1600
					
					WCDMA: desired channel that is used

					GSM: desired ARFCN that is used:
							128 to 251 for GSM 850 MHz,
							0 to 124 and 975 to 1023 for GSM 900 MHz,
							512 to 885 for GSM 1800 MHz,
							512 to 810 for GSM 1900 MHz

	\return true if successful, false if fail.

	\warning Set Mode must be called before this.  This function must be called before
			 any Tx or Rx specific functions.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_CHAN( HANDLE hResourceContext, unsigned short iChannel);


	/******************************************************************************/
	/**
	Factory Test Mode WCDMA Commands 3.2.1.2 Get ADC

	Factory Test Mode 1x RF Commands 3.2.3 (CL93-V5419-1) Get ADC

	Factory Test Mode RF Commands 3.3.1.2 (CL93-V4168-1)  Get ADC

	This command returns the value of the specified analog-to-digital converter (ADC) channels.

	\param  iADC_type  = the ADC channel to be read
	\param  iADC_value = ADC value of the desired channel

	\return true if successful, false if fail.
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GET_ADC_VAL
		( HANDLE hResourceContext, unsigned short iADC_type, unsigned short* piADC_value );

	/******************************************************************************/
	/**
	Factory Test Mode WCDMA Commands 3.2.1.3 Get RX AGC

	Factory Test Mode 1x RF Commands 3.2.4 (CL93-V5419-1) Get Rx AGC

	Factory Test Mode RF Commands 3.3.1.3 (CL93-V4168-1)  Get Rx AGC

	This command returns the 10-bit twos complement output of the RX AGC loop.

	NOTE:  This number is normally represented as a signed number.  In this
	       case, the sign is on the 10th bit, so if a signed number is 
		   desired, the user of this function will have to perform the
		   conversion to a 16-bit signed value.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iRX_AGC = For CDMA/WCDMA mode, AGC return value is from -512 to 511

	\return true if successful, false if fail.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GET_RX_AGC( HANDLE hResourceContext, unsigned short* piRX_AGC );

	/******************************************************************************/
	/**
	Factory Test Mode WCDMA Commands 3.2.1.4 Get synthesizer lock state

	Factory Test Mode 1x RF Commands 3.2.5 (CL93-V5419-1) Get synthesizer lock state

	Factory Test Mode RF Commands 3.3.1.4 (CL93-V4168-1)  Get synthesizer lock state

	This command returns the combined (ANDed) lock state of the RF synthesizers.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  bLockState = 0 = if one or more is out of lock
                         1 = if the synthesizers are all in lock

	\return true if successful, false if fail.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GET_SYNTH_STATE( HANDLE hResourceContext, unsigned char* pbLockState);

	/******************************************************************************/
	/**
	Factory Test Mode WCDMA Commands 3.2.1.5 Set uplink waveform to CW

	Factory Test Mode 1x RF Commands 3.2.6 (CL93-V5419-1) Set  CDMA wave CW

	Factory Test Mode RF Commands 3.3.1.5 (CL93-V4168-1)  Set  CDMA wave CW


		This command turns off the spreading on the uplink in WCDMA mode. The TX chain and all 
		clocks are active, but there is no modulating signal applied to the carrier; therefore a CW is 
		generated.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  bSelectCW = 0 - OFF/LO/FALSE deselect CW
						1 - ON/HI/TRUE select CW

	\return true if successful, false if fail.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA_CW_WAVEFORM( HANDLE hResourceContext, unsigned char bSelectCW);


	/******************************************************************************/
	/**
	Factory Test Mode WCDMA Commands 3.2.1.7 Set LNA offset

	Factory Test Mode 1x RF Commands 3.2.9 (CL93-V5419-1) Set LNA offset

	Factory Test Mode RF Commands 3.3.1.8 (CL93-V4168-1)  Set LNA offset

	This command sets the LNA offset for a given index value.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iLNAindex  = LNA offset index ID (0 is highest gain)

	The number of possible LNA offset varies with RF chipsets
			Please refer to applicable RF NV documents for the number of possible gain state

	\param iLNAoffset = LNA offset value in 1/12 dB resolution for the index ID

	\return true if successful, false if fail.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_LNA_OFFSET
		( HANDLE hResourceContext, unsigned short iLNAindex, unsigned short iLNAoffset);


	/******************************************************************************/
	/**
	Factory Test Mode WCDMA Commands 3.2.1.10 Set PA range

	Factory Test Mode 1x RF Commands 3.2.12 (CL93-V5419-1) Set PA range

	Factory Test Mode RF Commands 3.3.1.11 (CL93-V4168-1)  Set PA range

	This command sets the PA range state machine to the specified state.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iPArange = the binary state of the PA range state:
	\code
		0 - R0 = 0, R1 = 0,
		1 - R0 = 1, R1 = 0,
		2 - R0 = 0, R1 = 1,
		3 - R0 = 1, R1 = 1

	\endcode
	\return true if successful, false if fail.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_PA_RANGE( HANDLE hResourceContext, unsigned short iPArange );



	/******************************************************************************/
	/**
	Factory Test Mode WCDMA Commands 3.2.1.13 Get WCDMA IM2

	Factory Test Mode 1x RF Commands 3.2.15 (CL93-V5419-1) Get CDMA IM2

	Factory Test Mode RF Commands (cdma2000) 3.3.1.14 (CL93-V4168-1)  Get CDMA IM2

	FTM GSM RF Commands 3.8(CL93-V5370-1)  Get CDMA IM2

			This command returns the optimum IM2 value for the current mode and channel of the mobile. 
			RX signal inputs are required to be set before issuing this command or the results will not be 
			accurate.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iI_Value = The I value
	\param  iQ_Value = The Q value
	\param  iTransConductance_Value = The transconductance value.

	\return true if successful, false if fail.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GET_CDMA_IM2
		( 
			HANDLE hResourceContext, unsigned char* piI_Value, unsigned char* piQ_Value, 
			unsigned char* piTransConductance_Value 
		);

	/******************************************************************************/
	/**
	Factory Test Mode WCDMA Commands 3.2.1.14 Set DVGA offset
	
	Factory Test Mode 1x RF Commands 3.2.16 (CL93-V5419-1) Set DVGA offset

	Factory Test Mode RF Commands 3.3.1.15 (CL93-V4168-1)  Set DVGA offset

	This function sets the DVGA offset register for WCDMA mode.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iDVGAoffset = offset value, in 1/120ths of dB

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_DVGA_OFFSET( HANDLE hResourceContext, unsigned short iDVGAoffset );

	/******************************************************************************/
	/**
	Factory Test Mode WCDMA Commands 3.2.1.15 TX calibration sweep

	Factory Test Mode 1x RF Commands 3.2.18 (CL93-V5419-1) Tx Sweep Cal

	Factory Test Mode RF Commands 3.3.1.17 (CL93-V4168-1)  Tx Sweep Cal

	This command sets up one or more complete TX sweeps from high power to low power with 
	a predefined TX adjust PDM step size. Figure 2 illustrates one TX sweep. 
	The start power is determined by the last TX adjust PDM value set by the user before the 
	sweep. To prevent users from setting the TX power too high, the start power is also limited by 
	TX_SWEEP_CAL_MAX. 

	NOTE By default, TX_SWEEP_CAL_MAX is 450, while TX_SWEEP_CAL_MIN is 100. The
	TX_SWEEP_CAL_ STEP is set to 10.

	The stop power is governed by TX_SWEEP_CAL_MIN. The current implementation has two full 
	sweeps to ensure a spectrum analyzer will catch at least one full sweep with the video trigger 
	option. It could be reduced to one sweep for an automated test.

	This command sets up one or more complete TX sweeps from high power to low power with 
			a predefined TX adjust PDM step size.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  bOnOff = 0 - no effect, 1 - to trigger TX calibration sweep

	\return true if successful, false if fail.
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_TX_SWEEP_CAL( HANDLE hResourceContext, unsigned char bOnOff );

	/******************************************************************************/
	/**
	Factory Test Mode WCDMA Commands 3.2.1.16 Get DVGA offset

	Factory Test Mode 1x RF Commands 3.2.18 (CL93-V5419-1) Get DVGA offset

	Factory Test Mode RF Commands 3.3.1.17 (CL93-V4168-1)  Get DVGA offset

	
	For MSM6000/6025/6050 targets, iExpectedRX_AGC is ignored.  

	For MSM6100 (DMSS version: ??), iExpectedRX_AGC is ignored.  
	For	MSM6300 (DMSS version: ??), iExpectedRX_AGC is ignored.  
	For	MSM6500 (DMSS version: ??), iExpectedRX_AGC is ignored.  

	For MSM6550 (DMSS version: ??), iExpectedRX_AGC is used.
	For MSM6500 (DMSS version: ??), iExpectedRX_AGC is used.

	For MSM6200 targets, this command assumes the user is inputting a mobile RX signal at the 
	level equivalent to the NV item WCDMA_LNA_RANGE_FALL_I (all external losses, including 
	fixture must be accounted for). The return value is in 1/12 dB resolution. 

	For MSM6250 phase II targets, this command takes DVGA gain offset as an input instead of
	reading RFNV WCDMA_LNA_RANGE_FALL_I value. The input value needs to be converted from
	dBm to an RX AGC value.

	This command calibrates and returns the DVGA offset for the current channel of operation.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iExpectedRX_AGC = power level in RX AGC units, at the input to the phone
	\param  iDVGAoffset = return value of the measured DVGA offset

	\return true if successful, false if fail.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GET_DVGA_OFFSET( HANDLE hResourceContext, short iExpectedRX_AGC, short* piDVGAoffset );

	/******************************************************************************/
	/**
	Factory Test Mode WCDMA Commands 3.2.1.17 Get LNA offset

	Factory Test Mode 1x RF Commands 3.2.20 (CL93-V5419-1) Get LNA offset

	Factory Test Mode RF Commands 3.3.1.19 (CL93-V4168-1)  Get LNA offset
	

		This command calculates and returns the LNA offset value for the input ID parameter. The
		function assumes the user has set up the RX signal into the mobile at an appropriate level. This
		function controls the AGC state for the purpose of calculating the LNA offset of interest. This
		function was designed to be called with the following sequence in mind:
		
		1. Set the power level into the mobile RX appropriate for id=1.
		2. Call the function Get LNA Offset with id=1.
		3. Set the power level into the mobile RX appropriate for id=2.
		4. Call the function Get LNA Offset with id=2.
		5. Set the power level into the mobile RX appropriate for id=3.
		6. Call the function Get LNA Offset with id=3.
		
		NOTE Since the DVGA dynamic range is limited, all indexes feed on the previous index being
		calculated (with the exception of id=1). If the above sequence is not followed, the returned
		values may be meaningless.

	This command calculates and returns the LNA offset value for the input ID parameter.

	For all MSM6000/6025/6050 targets, iExpectedRX_AGC is ignored.  

	For MSM6100 (DMSS version: ??), iExpectedRX_AGC is ignored.  
	For	MSM6300 (DMSS version: ??), iExpectedRX_AGC is ignored.  
	For	MSM6500 (DMSS version: ??), iExpectedRX_AGC is ignored.  

	For MSM6550 (DMSS version: ??), iExpectedRX_AGC is used.
	For MSM6500 (DMSS version: ??), iExpectedRX_AGC is used.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iLNAindex = LNA offset index, 1-3, zero is the highest gain state, but is not a 
					   valid option for this function.
	\param  iExpectedRX_AGC = power level in RX AGC units, at the input to the phone
	\param iLNAoffsetValue = measured value of the LNA offset, in 1/12dB resolution

	\return true if successful, false if fail.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GET_LNA_OFFSET
		( HANDLE hResourceContext, unsigned short iLNAindex , short iExpectedRX_AGC, short* piLNAoffsetValue );

	/******************************************************************************/
	/**
	\code
	Factory Test Mode WCDMA Commands 3.2.2 Get HDET from Tx Sweep Cal
	Factory Test Mode RF Commands 3.3.1.20 (CL93-V4168-1) Get HDET from Tx Sweep Cal
	\endcode

		While the TX is ON, data is collected after the complete execution of the TX Sweep CAL subcommand. 
		Each entry is bound to a tx_agc_pdm value taken during TX Sweep CAL. If the user wishes to read 
		HDET values unrelated to TX Sweep CAL, the FTM_GET_ADC_VAL API should be used.

	This command returns a value from the HDET TX Sweep Cal array.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iIndex = HDET array index, the 0 index corresponds to the first step 
					 of the TX Sweep.

	\param  iHDETvalue = value of the HDET at the iIndex position

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GET_HDET_FROM_TX_SWEEP_CAL
				( HANDLE hResourceContext, unsigned short iIndex, unsigned short* piHDETvalue );

	/******************************************************************************/
	/**
	Factory Test Mode WCDMA Commands 4.19 Get HDET from Tx Sweep Cal

	Factory Test Mode 1x RF Commands 3.2.26 (CL93-V5419-1) Get ALL HDET from Tx Sweep Cal 	

		While the TX is ON, data is collected after the complete execution of the TX Sweep CAL subcommand. 
		Each entry is bound to a tx_agc_pdm value taken during TX Sweep CAL. If the user wishes to read 
		HDET values unrelated to TX Sweep CAL, the FTM_GET_ADC_VAL API should be used.

		This command returns a value from the HDET TX Sweep Cal array.

  \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param  piHDETvalues = Array of 32 bytes, to store the average HDET for the first 32 
	                       steps of the Tx Sweep

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GET_ALL_HDET_FROM_TX_SWEEP_CAL
		( HANDLE hResourceContext, unsigned char * piHDETvalues );

	/******************************************************************************/
	/**
	Factory Test Mode 1x RF Commands 3.2.28 (CL93-V5419-1) Set RF Power Mode 	

		This command overrides the automatic IntelliCeiver Power mode decision. 
		This setting will be lost when changing the channel with the FTM_SET_CHAN API,
		or when the phone is returned to Online mode.

	\param  iMode = Power mode to force the IntelliCeiver to:
					0 = High Power mode
					1 = Mid Power mode
					2 = Low Power mode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_RF_POWER_MODE( HANDLE hResourceContext, unsigned char iMode );


	/******************************************************************************/
	/**
	Factory Test Mode WCDMA Commands 4.20 Set HDET Tracking

		This command enables/disables the HDET tracking. When the HDET tracking is enabled, the
		software will periodically read the HDET value and adjust the uplink power to meet a desired
		level based on a given HDET value.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iOnOff = 
		\code
           0 = disable HDET tracking
           1 = enable HDET tracking
		\endcode

	\param  iHDETvalue = desired HDET value for setpoint

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_HDET_TRACKING
		( HANDLE hResourceContext, unsigned short iOnOff, unsigned short iHDETvalue );
	

	/******************************************************************************/
	/**
	Factory Test Mode WCDMA Commands, 4.21 Configure Tx Calibration Sweep

	This command sets the time interval of each step in the Tx calibration sweep 
	and the number of HDET readings averaged per step.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iTimeInterval = 1/32.768 kHz = 0.0305 ms per count.  
							The minimum value is 66, which is about 2 ms. Any number less
							than 66 will be forced to 66.

                            The time interval is how long the step is (in time).  We found that a 2ms 
							step time is ideal and anything below that is not repeatable.  We also 
							tested the CMU-200's and the FSP measurement capabilities for 2ms and 
							found them to be acceptable.  For these reasons, we suggest a value of 
							66 (2ms) to be used for the time interval.

					

	\param  iNumofHDETReadingsPerStep= Number of HDET reads for averaging (from 1 to 255)
                                       During the top 32 power steps, the HDET will be measured.  
									   During the measurement of the HDET, each value is averaged 
									   with other HDET readings at the same power level.   This 
									   setting controls how many HDET averages are taken during 
									   each power step. 

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CONFIGURE_TX_SWEEP_CAL
		( HANDLE hResourceContext, unsigned short iTimeInterval, unsigned short iNumofHDETReadingsPerStep);

	/******************************************************************************/
	/**
	Factory Test Mode WCDMA Commands, 4.24 Set Tx sweep step size

	This command sets the PDM step size used for the Tx calibration sweep. For each step of the
	sweep, the PDM will be reduced by the specified step size. If this command is not used, then the
	step size will default to 10.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iStepSize = PDM step size to use for the Tx calibration sweep

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_TX_SWEEP_STEP_SIZE
		( HANDLE hResourceContext, unsigned char iStepSize );


	/******************************************************************************/
	/**
	Factory Test Mode 1x RF Commands 3.2.23 (CL93-V5419-1)Get CtoN

	 This command returns the carrier-to-noise ratio as calculated at the Rx front end of the Rx chain.
	 The value returned is 10 * (C/N) dB. This value was chosen to provide the result with one
	 decimal place of precision.

	 see also: Factory Test Mode Get C-to-N, CL93-V6487-1

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iCtoN = Returns the C/N ratio value with one decimal place = 10 * (C/N) dB
				
	\return true if successful, false if fail.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GET_CTON( HANDLE hResourceContext, long* iCtoN);

	/******************************************************************************/
	/**
	Factory Test Mode WCDMA Commands, 4.27 Do DC calibration

	This command performs DC calibration. The Set mode and Set RF Channel commands should
	be called first. The results of the calibration will be written to NV directly.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if fail.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_DO_DC_CAL(	HANDLE hResourceContext );

	/******************************************************************************/
	/**
	Factory Test Mode 1x RF Commands 3.2.25 (CL93-V5419-1)Get CDMA IM2 with suppression

	This command returns the suppression achieved as well as the I, Q, and transconductance (gM)
	optimum IM2 values for the current mode and channel of the mobile. It is intended to augment the original Get CDMA IM2.
 
	Rx signal inputs are required to be set before issuing this command or results will not be accurate.

	The suppression level is given in units of dB.
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iI_Value = I value
	\param	iQ_Value = Q value
	\param	iTransConductance_Value = Transconductance Value
	\param	iSuppression = Suppression
				
	\return true if successful, false if fail.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GET_CDMA_IM2_WITH_SUPPRESSSION(	HANDLE hResourceContext,
																	unsigned char* iI_Value, 
																	unsigned char* iQ_Value, 
																	unsigned char* iTransConductance_Value,
																	unsigned short* iSuppression);

	
	
	/******************************************************************************/
	/**
	Factory Test Mode FM Commands, 3.3.1.16(CL93-V4168-1) Set frequency sense gain

	This command sets the frequency sense gain register to set the DFM modulation index..

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iModulationIndex = 
											Range of data is from 0 to 63 (6 LSBits of input byte):
											0 = lowest
											63 = highest.

	\return true if successful, false if fail.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_FREQ_SENSE_GAIN(	HANDLE hResourceContext,
							unsigned char iModulationIndex );

	/******************************************************************************/
	/**
	Factory Test Mode FM Commands, 3.3.1.21(CL93-V4168-1) Get FM IQ

	This command returns the A and B compensation values for the FM RF Mode. This command
	assumes that the user is adding a Rx CW on channel center at -75 dBm. This function will lock
	the trk-lo tune of the radio off one channel, calculate the A and B values, retune the radio back to
	the original channel, and return the A and B values as a packed double word.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iFmIQValue = 
											Most significant 16 bits = B values
											Least significant 16 bits = A values.

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GET_FM_IQ(	HANDLE hResourceContext,
												unsigned long* iFmIQValue );

/******************************************************************************
						Dual RX Chain FTM RF Commands 
*******************************************************************************/
	/******************************************************************************/
	/**
	Dual Rx Chain FTM Command 3.3.10 (CL93-V5864-1) Set secondary chain

	This command sets the secondary chain in one of three modes, DISABLED, RECEIVE DIVERSITY and OFF FREQUENCY SEARCH
 	
	\param  mode
		\code
           0 = Disabled - puts the secondary chain ZIFRIC and ZIFLNA in disabled mode via SBI
           1 = Receive Diversity - enables the secondary chain ZIFRIC and ZIFLN via SBI and switches the RX_VCO_SEL MUX
                                   to the primary chain PLL; this means that both the primary
                                   and secondary chain are set to the same band class and
                                   channel.

           2 = Off frequency search (OFS) = allows you to set the secondary chain to a band class and channel that is
               independent of the primary chain settings
		\endcode
	\return true if successful, false if fail.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_SECONDARY_CHAIN( HANDLE hResourceContext, unsigned short iMode );

	/******************************************************************************/
	/**
	Dual Rx Chain FTM Command 3.3.12 (CL93-V5864-1) Set second chain test call mode

	This function forces the AMSS to use the second chain for all CDMA 1x RX processing in online
	mode. This test mode is intended for production testing of the second receive chain functionality.
	To use the second chain test mode, first place the phone in FTM mode. Then issue the Second
	Chain Test Call Mode command, with the mode parameter set to Enable. Issue a runtime switch
	back to online mode. The phone will now only respond to forward link signals on the second
	antenna. All transmit functions continue to use the primary antenna port. The phone will return to
	normal operation after a reset, or a Second Chain Test Call Mode command with the mode set to
	Disable.

	See also: CL93-V6408-1, App Note : FTM, Set Up MSM for Call Process on Second Rx Chain
	
	\param  Byte mode  = enables and disables the second chain test call mode:
	\code
       0 = Primary chain only
       1 = Secondary chain only
       2 = Primary and secondary chain
	\endcode
		
	\return true if successful, false if fail.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SECOND_CHAIN_TEST_CALL( HANDLE hResourceContext, unsigned char iMode );



/******************************************************************************
						FTM CDMA RF Calibration API V2 
*******************************************************************************/

	/******************************************************************************/
	/**
	FTM CDMA RF Cal API v2 3.1 (80-V2376-1) Commit RF Calibration to NV

	The Commit RF Cal command is used to store the current state of the RF calibration into NV.
	The Version 2 RF calibration APIs maintain states internally, temporarily recording any receiver
	calibration updates. When calibration is complete, all values can be written once to NV with a
	single command, eliminating the need to externally compute Rx calibration NV and resend the
	values to the mobile. In platforms that support Rx diversity, Rx calibration for both Rx chains are
	written to NV with a single execution of this command.
	The Commit API is optional and need not be used. Rx calibration may still be written to NV with
	individual NV write requests, as per the old API. NV remains unchanged until the command is
	issued, so the Version 2 calibration APIs may be used safely for testing, without concern of
	corrupting existing calibration data. All uncommitted changes will be lost when the mobile
	reboots or executes a runtime switch into Online mode.
	
 	
	\param  iBandClass,
				Band to be written to NV
				0 = band class 0
				1 = band class 1
				3 = band class 3
				4 = band class 4
				5 = band class 5
				6 = band class 6
			Values 2 and 7 through 20 are reserved for future CDMA use

	\return true if successful, false if fail.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA_API2_COMMIT_RF_TO_NV( HANDLE hResourceContext, unsigned short iBandClass );

	
	/******************************************************************************/
	/**
	FTM CDMA RF Cal API v2 3.2 (80-V2376-1) Calibrate DVGA

	This command calculates and returns the DVGA offset for the current channel of operation. This
	is a frequency-dependent API, and the DVGA offset returned will be automatically associated
	with the correct bin in the channel list.
	The input parameters are the receiver path the user wants to calibrate (path 1 is only supported on
	platforms that implement Rx diversity) and the input power at the antenna. This input power is in
	AGC units. To convert input power in dBm to AGC units in a 102.4-dB dynamic range platform
	with a minimum RSSI of -115 dBm, use the equation:
		-512 + 10*(InputPower(dBM) + 115)
	
	The input power should be selected such that the baseband input to the MSM™ is sufficiently
	strong but will not saturate the baseband ADC. This is a target-dependent value and should be
	selected based on the expected RF gain for a given gain lineup with margin for part-to-part
	variation.

	IntelliCeiver platforms may return more than one DVGA offset calibration value. The number of
	calibration values that are returned will be indicated in the Calibration Count field of the response
	packet
 	
	\param	iPath,	0 - Primary Rx path
					1 - diversity Rx path
	\param	iInputPower	Input power at the antenna, in AGC units (1/10th dB)
	\param  iCount[in/out]		[in] Size of iDVGAarray Buffer
								[out] Calibration Count (N) 
	\param  iDVGAArray DVGA value array { DVGA_0, ... DVGA_N }

	\return true if successful, false if fail.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA_API2_CALIBRATE_DVGA( HANDLE hResourceContext,
															  unsigned char iPath, 
															  short iInputPower, 
															  unsigned char* iCount, 
															  unsigned short* iDVGAarray );


	/******************************************************************************/
	/**
	FTM CDMA RF Cal API v2 3.3 (80-V2376-1) Calibrate LNA

	This command calculates and returns the LNA offset for a particular gain state for the current
	channel of operation. This is a frequency-dependent API, and the LNA offset returned will be
	automatically associated with the correct bin in the channel list.
	The input parameters are the receiver path the user wants to calibrate (path 1 is only supported on
	platforms that implement Rx diversity), the LNA gain step to calibrate, and the input power at the
	antenna. This input power is in AGC units. To convert input power in dBm to AGC units, use the
	equation described in Section 3.2.
	
	 IntelliCeiver platforms may return more than one LNA offset calibration value. The number of
	calibration values that are returned will be indicated in the Calibration Count field of the response
	packet.

 	
	\param	iPath,	0 - Primary Rx path
					1 - diversity Rx path
	\param  iLNAstep LNA step
					.. 0 = first LNA gain step (G0-G1)
					.. 1 = second LNA gain step (G1-G2)
					.. 2 = third LNA gain step (G2-G3)
					.. 3 = fourth LNA gain step (G3-G4)
	 \param	iInputPower	Input power at the antenna, in AGC units (1/10th dB)
	\param  iCount[in/out]		[in] Size of iLNAarray Buffer
								[out] Calibration Count (N) 
	\param  iLNAarray LNA value array { LNA<iLNAstep>0, ... LNA<iLNAstep>N }

	\return true if successful, false if fail.

	\warning 
	*******************************************************************************/

	QLIB_API unsigned char QLIB_FTM_CDMA_API2_CALIBRATE_LNA(	HANDLE hResourceContext,
																unsigned char	iPath, 
																short  iInputPower, 
																unsigned char	iLNAstep, 
																unsigned char*	iCount, 
																unsigned short* iLNAarray );

	/******************************************************************************/
	/**
	FTM CDMA RF Cal API v2 3.4 (80-V2376-1) Calibrate IM2

	This command returns the optimum IM2 value for the current band. This is a
	frequency-independent calibration item. The input parameter is the receiver path the user
	wants to calibrate (path 1 is only supported on platforms that implement Rx diversity).
	IntelliCeiver platforms may return more than one IM2 value. The number of calibration values
	that are returned will be indicated in the Calibration Count field of the response packet.
	The Suppression field returns a measure of the IM2 performance improvement. Suppression will
	vary from part-to-part and is described in the IM2 calibration with suppression section of [Q4].

 	
	\param	iPath,	0 - Primary Rx path
					1 - diversity Rx path

	\param  iSuppression Characteristic IM2 performance vs. uncalibrated (dB)
	\param  iCount[in/out]		[in] Size of aiIM2result Buffer
								[out] Calibration Count (N) 
	\param  aiIM2result			IM2 result array { I 0, Q 0, Transconductance 0, ...  I N, Q N, Transconductance N}

	\return true if successful, false if fail.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA_API2_CALIBRATE_IM2(	HANDLE hResourceContext, 
																unsigned char iPath, 
																unsigned short* iSuppression, 
																unsigned char* iCount, 
																unsigned char* iIM2result );

	/******************************************************************************/
	/**
	FTM CDMA RF Cal API v2 3.5 (80-V2376-1) Calibrate  IntelliCeiver

	This command calibrates the internal circuitry of IntelliCeiver chips. This is a
	frequency-independent calibration item. The parameters are a receive path, for which only
	path 0 is supported, and the power level of the input signal, in AGC units, as described in
	
	Section 3.2.
	The calibration input waveform must be an unmodulated carrier 1-MHz positive offset from
	the center of the channel where the calibration is performed. The calibration channel is not
	significant, though it should be selected such that the input waveform will not be affected by any
	rolloff in band selection filters. It is recommended that this calibration be performed on the same
	channel on which the IM2 calibration is performed, but it is not necessary.
	The response packet contains a Success field, indicating if the calibration was successful. A zero
	returned in this field indicates a problem was detected during calibration. Check the levels and
	setup of the test equipment and repeat the calibration. The remaining data is an opaque BLOB
	containing the IntelliCeiver calibration. The Response Length field will be used to indicate the
	number of valid data bytes in the packet. The calibration BLOB data are the bytes following the
	packet header and success field, to the end of the packet indicated by the Response Length.

 	
	\param	iPath,	0 - Primary Rx path
					1 - diversity Rx path

	\param  iInputPower	Input power at the antenna, in AGC units (1/10th dB)
	\param  iSizeOfBLOB [in/out]		[in] Size of iCalibtrationBLOB input buffer
										[out] Actual size of calibration BLOB returned from embedded side
	\param  iCalibrationBLOB			Opaque calibration value that must be written to the
										IntelliCeiver calibration NV item exactly as returned

	\return true if successful, false if fail.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA_API2_CALIBRATE_INTELLICEIVER(	HANDLE hResourceContext,
																		unsigned char iPath, 
																		short iInputPower, 
																		unsigned char* iSizeOfBLOB,
																		unsigned char* iCalibrationBLOC );




/******************************************************************************
						GSM FTM
*******************************************************************************/

	/******************************************************************************/
	/**
	Factory Test Mode GSM Commands 3.9 Set PA profile

			1) Downloads the user-defined PA (power amplifier) ramp-up and ramp-down profile to the
			   embedded platform and stores it in the PA LUT (look-up table) for use in subsequent
			   transmits.

			2) Updates the four PA ramp offset tables for GSM 850 and GSM 900 RF operating mode and
			   two PA ramp offset tables for GSM 1800 and GSM 1900 RF operating mode, such that they
			   all have the same shape as the input PA ramp profile. GSM 850 and GSM 900 share the same
			   PA ramp offset tables, as do GSM 1800 and GSM 1900. The first offset table is shared
			   between both RF operating modes. The algorithm used to compute the PA ramp offset tables
			   from the input PA ramp profile is explained below.


			The input data will be concatonated into the pa_lut_type structure,
			as summarized below:
		\code
            #define PA_LUT_MAX_SIZE      64	// Number of entries in the PA LUT
			
            struct pa_lut_cfg_type
            {
               unsigned char ramp_step_size;
               unsigned char pa_lut_size;
            } ;
			  
            struct pa_lut_type
            {
               unsigned short ramp_up[PA_LUT_MAX_SIZE];
               unsigned short ramp_down[PA_LUT_MAX_SIZE];
            };
		\endcode
  
			See GSM FTM document for more detailed information.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  pRampUp   = pointer to memory buffer of 64 ramp up values
	\param  pRampDown = pointer to memory buffer of 64 ramp down values
	\param  iRampSize = Number of elements in the ramp size.  This is a maximum of 64. Normally this value is <TBD>
	\param  iPaLookupTableSize = Number of elements in the PA look up table.  Normally this value is <TBD>.

	\return true if successful, false if fail.

	\warning This operation has not been tested yet in the factory library!

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_PA_PROFILE
		( HANDLE hResourceContext, unsigned short* pRampUp, unsigned short* pRampDown, 
									  unsigned char iRampSize, unsigned char iPaLookupTableSize );

	/******************************************************************************/
	/**
	Factory Test Mode GSM Commands 3.11 Set transmit continuous

	This function configures transmit parameters to perform a continuous transmit operation.

		NOTE: A continuous transmit operation is one in which the transmitter, modulator, and related RF
		circuitry are left on and not allowed to burst. The actual transmit begins when the subsequent
		Set Tx ON function is issued, then the succeeding call to Set Tx OFF function will turn off the
		transmitter, modulator, and related circuitry.


		The data will be packed into this structure, which is referenced in the document
  
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotNum    = slot number
	\param iDataSource = Modulation data source:
		\code
            0 - FTM_GSM_TX_DATA_SOURCE_PSDRND 
            1 - FTM_GSM_TX_DATA_SOURCE_TONE,
            2 - FTM_GSM_TX_DATA_SOURCE_BUFFER
            3 - FTM_GSM_TX_DATA_SOURCE_TWOTONE
		\endcode
	\param iTSCindex			= TSC Index number
	\param iNumBursts			= # of bursts, if not infinate
	\param bIsInfiniteDuration	= 
		\code
            1 - Infinate
            0 - Count bursts
		\endcode

	\return true if successful, false if fail.

	\warning When the Tx hardware is turned on, the receiver must be turned off; 
	         therefore, this API cannot be used simultaneously with the Set Rx Burst 
			 or the Set Rx Continuous API.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_TRANSMIT_CONT
		( HANDLE hResourceContext, unsigned char iSlotNum, short iDataSource, unsigned char iTSCindex,
										unsigned long iNumBursts, unsigned char bIsInfiniteDuration );


	/******************************************************************************/
	/**
	Factory Test Mode GSM Commands 3.12 Set transmit burst

	This function configures parameters associated with a Tx burst.

			NOTE The bursts will start when the Set Tx ON API is issued and will last for the number of TDMA
			frames specified as one of the tx_burst parameters. If this parameter lists infinite burst
			transmission, the phone will keep issuing transmit bursts until the Set Tx OFF API is issued.


	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotNum    = slot number
	\param iDataSource = Modulation data source:
	\code
			0 - TX_DATA_SOURCE_PSDRND 
			1 - TX_DATA_SOURCE_TONE,
			2 - TX_DATA_SOURCE_BUFFER

	\endcode
	\param iTSCindex			= TSC Index number
	\param iNumBursts			= # of bursts, if not infinate
	\param bIsInfiniteDuration	= 1 - Infinate, 0 - Count bursts

	\return true if successful, false if fail.

	\warning When the Tx hardware is turned on, the receiver must be turned off; 
	         therefore, this API cannot be used simultaneously with the Set Rx Burst 
			 or the Set Rx Continuous API.
	*/
	QLIB_API unsigned char QLIB_FTM_SET_TRANSMIT_BURST
		( HANDLE hResourceContext, unsigned char iSlotNum, short iDataSource, unsigned char iTSCindex,
										unsigned long iNumBursts, unsigned char bIsInfiniteDuration );

	/******************************************************************************/
	/**
	Factory Test Mode GSM Commands 3.13 Set Rx burst

			This function configures parameters to either start (RX_FUNCTION_GET_RSSI or
			RX_FUNCTION_GET_DC_OFFSETS) or stop sending Rx burst commands to the MDSP
			(RX_FUNCTION_STOP_CONTINOUS_RX), as specified by the enum_rx_function_type parameter.

			The document refers to the rx_burst_type structure, which is defined here:
	\code
             struct rx_burst_type
             {
                 unsigned char slot_num;	// 1unsigned char, unsigned 8-bit = slot number
                 short rx_funct;			// 2unsigned chars, signed 16-bit, rx function ID
             };
	\endcode

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotNum = Slot number to receive
	\param iRxFunction =  Type of function for DSP to perform
	\code
			0  - RX_FUNCTION_GET_RSSI
			1  - RX_FUNCTION_GET_DC_OFFSETS
			2  - RX_FUNCTION_STOP_CONTINOUS_RX

	\endcode
	\return true if successful, false if fail.

	\warning Cannot be used while FTM_SET_TRANSMIT_BURST is active

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_RX_BURST( HANDLE hResourceContext, unsigned char iSlotNum, short iRxFunction);

	/******************************************************************************/
	/**
	Factory Test Mode GSM Commands 3.14 Set Rx continuous

			This function configures the RF hardware such that the receiver is ON continuously and is not
			bursting. The enum_rx_function_type parameter specifies whether the receiver is to be turned
			ON (RX_FUNCTION_RF_RX_HW_ON) or OFF (RX_FUNCTION_RF_RX_HW_ON). This mode is used for
			performing hardware test and measurements.

			The document refers to the rx_burst_type structure, which is defined here:
				struct rx_burst_type
				{
					unsigned char slot_num;	// 1unsigned char, unsigned 8-bit = slot number
					short rx_funct;			// 2unsigned chars, signed 16-bit, rx function ID
				};

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotNum	 = Slot number to receive
	\param iRxFunction = Type of function for DSP to perform
		\code
           0  - RX_FUNCTION_GET_RSSI
           1  - RX_FUNCTION_GET_DC_OFFSETS
           2  - RX_FUNCTION_STOP_CONTINOUS_RX
		\endcode

	\return true if successful, false if fail.

	\warning Cannot be used while QLIB_FTM_SET_TRANSMIT_BURST is active
	\warning When RX Continuous is used the RSSI will not update.  This command mode is used for troubleshooting purposes only.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_RX_CONTINUOUS( HANDLE hResourceContext, unsigned char iSlotNum, short iRxFunction );

	/******************************************************************************/
	/**
	Factory Test Mode GSM Commands 3.15 Set Tx power level

			This function reads PA ramp profiles from NV or if NV is not loaded, it reads the default PA
			ramp profile from software, and then sets the corresponding Tx power level.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iPowerLevelID = Power level number to set,
	\code
			0 to 14 = 5 dBm to 33 dBm in GSM 850 and GSM 900 mode,
			0 to 15 = 0 dBm to 30 dBm in GSM 1800 and GSM1900 mode

	\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_TX_POWER_LEVEL( HANDLE hResourceContext, unsigned char iPowerLevelID );

	/******************************************************************************/
	/**
	Factory Test Mode GSM Commands 3.16 Get RSSI

			This function returns a 32-bit unsigned raw receiver signal strength indicator (RSSI) value. This
			number is averaged over multiple bursts--currently over 10 bursts.

		A couple of points about this function, from questions asked:
			1. 10 bursts are just our default implementation on embedded side.  The PC
			side (QMSL) doesn't do any averaging.  If customer wants to change/define
			the number of averaging, they need to 1) change the embedded side FTM
			behavior and 2) change the PC side (by extending QMSL).  To extend the PC
			side, they can use SendSync function.  Keep in mind that QMSL is just a PC
			wrapper to all FTM functions.  In this case, it is just a wrapper of
			functions documented in CL93-V5370-1 (GSM FTM).

			2. Since QMSL is just a PC side wrapper on FTM function, you can look up the
			embedded side implementation to see how RSSI value is defined/derived.  You
			should be able to trace through the FTM dispatch table on embedded side.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iRssiValue = Return value of the RSSI value

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GET_RSSI( HANDLE hResourceContext, unsigned long* piRssiValue );

	/******************************************************************************/
	/**
	Factory Test Mode GSM Commands 3.17 Set PA start delta

		This function offsets the PA ramp start time by the specified amount. A positive value for
		3 startDelta causes the ramp to start later than it does, and a negative value causes the PA ramp to
		4 start earlier. The unit assumed for startDelta is quarter symbols.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iPA_StartDelta = Value (1/4 symbol) to adjust PA ramp start time

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_PA_START_DELTA( HANDLE hResourceContext, unsigned short iPA_StartDelta );

	/******************************************************************************/
	/**
	Factory Test Mode GSM Commands 3.18 Set PA stop delta

		This function offsets the PA ramp stop time by the specified amount. A positive value for
		9 stopDelta causes the ramp to stop later than it does, and a negative value causes the PA ramp to
		10 stop earlier. The unit assumed for stopDelta is quarter symbols.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iPA_StopDelta = Value to adjust PA ramp stop time (1/4 symbols)

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_PA_STOP_DELTA( HANDLE hResourceContext, unsigned short iPA_StopDelta );


	/******************************************************************************/
	/**
	Factory Test Mode GSM Commands 3.2.19 Set PA DAC input

			The Set PA DAC input API uses the PA ramp offset table and the specified dacInput value to
			generate a PA ramp profile. 

			This profile is such that it has the shape specified in the offset table,
			and the flat portion of the profile has a power corresponding to the dacInput specified. This
			generated PA profile is then copied to the PA LUT for use in the subsequent transmits (burst or
			continuous).

			In GSM 850 and GSM 900 RF operating mode, there are four PA ramp offset tables and in GSM
			1800 and GSM 1900 RF operating mode, there are two PA ramp offset tables to choose from.
			GSM 850 and GSM900 shared the same PA ramp offset tables, as does GSM 1800 and GSM
			1900. One of these offset tables will used to generate a PA ramp profile based on the dacInput
			value and RF operating mode.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iDACinput = DAC value that is used for the maximum level of the burst.

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_PA_DAC_INPUT( HANDLE hResourceContext, unsigned short iDACinput );

	/******************************************************************************/
	/**
	Factory Test Mode GSM Commands 3.20 Set Tx Cal Sweep

			This function takes 3 parameters, a dac_start value, a dac_stop value and a dac_step value. It then
			turns the transmit burst on and varies the power from dac_start to dac_stop incrementing the
			power in steps of dac_steps. It uses the SET PA DAC input API to set the power for each burst. It
			has been introduced to make characterization of the GSM power amplifier easier and faster. Note
			the dac_step has to be positive.
			
			

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iDAC_Start	= DAC value for the first burst step
	\param	iDAC_Stop	= DAC value for the last burst step
	\param	iDAC_Step	= Number of DAC values to increment between each burst (positive number)

	\return true if successful, false if fail.

	\warning The SET_TX_OFF command must be called before executing this command. Otherwise, results
			 can be unpredictable. 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_TX_CAL_SWEEP
		( HANDLE hResourceContext, unsigned short iDAC_Start, unsigned short iDAC_Stop, unsigned short iDAC_Step);


	/******************************************************************************/
	/**
	Factory Test Mode GSM Commands 3.21 Set Tx frame matrix

	This function configures the Tx slots. It needs to be called seven times to configure all seven 3 slots.
		
	\param  iSlotNum	= Slot number to be configured, 0 to 7
	\param	iTurnSlotOn	= Turn the slot on or off, 0=OFF, 1=ON
	\param	iPwrLevel	= Power level for this slot; should be in dB*100
	\param  iModScheme = Modulation Code Scheme; 0=MCS1, 1=MCS2, to 8=MCS9

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_TX_FRAME_MATRIX ( HANDLE hResourceContext,  
		unsigned char iSlotNum, unsigned char iTurnSlotOn, unsigned short iPwrLevel, unsigned char iModScheme);

	/******************************************************************************/
	/**
	Factory Test Mode GSM Commands 3.22 Rx gain range cal

	This command combines a number of steps required to perform Rx gain range calibration across frequencies.

	The traditional method of performing gain range calibration involved the following steps:
	\code
       1. Set Mode.
       2. Set Channel 1.
       3. Turn RX on.
       4. Set gain range 0.
       5. Get RSSI.
       6. Repeat steps 4 and 5 through all gain ranges.
       7. Set Channel 2.
       8. Repeat steps 4 through 6, etc.
	\endcode

	This API now accepts a bit mask to specify which gain ranges are being calibrated and returns the
	RSSI for all those gain ranges in a single command, i.e., it combines steps 5 and 6 above into a
	single command. In addition, it also turns the receiver on/off based on input from the user.
	18 The modified method of performing the gain range calibration is now:
	\code
       1. Set Mode.
       2. Set Channel 1.
       3. Call Rx gain range cal command with:
          a. gain_range_bitmask = 0xF (for 4 gain ranges) or 0x1F (for 5 gain ranges)
          b. Number of Burst to Average set to some number greater than 1
          c. Toggle RX on/off set to TRUE (this will cause the phone to turn on the receiver)
       4. Read the response of Rx gain range cal with the result in the RSSI Result field.
       5. Set Channel 2.
       6. Call Rx gain range cal command as in step 3, but set Toggle RX on/off field to FALSE.
       7. Read the response of the command.
       8. Continue for the remaining channels.
       9. Set the last channel number.
       10. Call Rx gain range cal 1 command as in step 3, i.e., set Toggle RX on/off field to TRUE.
           This should cause the phone to turn off the receiver after making measurements for the last
           channel number.
       11. Read the results.
	\endcode

	\param  iRxRangeBitMask  = input, When a bit position is set, the gain range corresponding
						to that bit position is calibrated, e.g., 0b00001111 will
						give RSSI measurements for gain range 0,1,2,3
		Refer to FTM_GSM_RX_GainRange_Enum for definitions
		\code
          FTM_GSM_RX_GAIN_RANGE_0	= 0x01,	//!<' Gain range 0
          FTM_GSM_RX_GAIN_RANGE_1	= 0x02,	//!<' Gain range 1
          FTM_GSM_RX_GAIN_RANGE_2	= 0x03,	//!<' Gain range 2
          FTM_GSM_RX_GAIN_RANGE_3	= 0x04,	//!<' Gain range 3
          FTM_GSM_RX_GAIN_RANGE_4	= 0x05	//!<' Gain range 4
		\endcode
		
	\param iNumAverages = Each RSSI measurement is obtained by averaging RSSI
	                      over a number of bursts; this parameter allows the
	                      number of bursts to be configured.

	\param bToggleRx = input, Possible values are:
		\code
          TRUE = If RX was on, turn it off, If RX was off, turn it on
          FALSE = Do not alter the state of the receiver
		\endcode

	\param aiRSSI_Result = output, Array of the results:
                             uint32 rssi[8]

			For example, if gain range bitmask = 0b00011101,
			rssi[0] = RSSI for gain range 0, rssi[1] = 0, rssi[2] = RSSI
			for gain range 2, rssi[3] = RSSI for gain range 3, rssi[4]
			= RSSI for gain range 4, etc.
			Note: Current targets have either 4 or 5 gain ranges.
			Setting bit positions that have no corresponding gain
			range will result in erroneous results.

			The size of the array is defined by FTM_GSM_RX_GAIN_RANGE_CAL_NUM_RETURN_VALUES in QLib_Defines.h

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_RX_GAIN_RANGE_CAL( 
			HANDLE hResourceContext,  
			unsigned char iRxRangeBitMask, 
			unsigned char iNumAverages,
			unsigned char bToggleRx,
			unsigned long aiRSSI_Result[8]
			);

/******************************************************************************
						GSM Polar Tx Cal FTM
*******************************************************************************/

	/******************************************************************************/
	/**
	Factory Test Mode GSM Polar Command 2.2 Do Autocal
	
	The FTM_DO_AUTOCAL command is used to measure the amplitude and phase nonlinearity of the GSM power amplifier. 
	The function returns two tables. One of them is a measure of the DAC input to the power 
	amplifier versus the output power of the power amplifier. 
	The other is a table of the DAC input to the power amplifier versus the output-phase 
	distortion caused by the PA. These tables are then interpolated, as required, 
	and loaded into the MSM. The transmit data is then predistorted to compensate for the measured nonlinearity.
	The inputs to the Autocal function are:

	1. An external power measurement point.
	2. The dynamic range of the measurement.
	3. A flag to specify if the measured data is to be stored in NV.
	4. The RSB-correction parameters.

	This function will start the FTM2 logging, which will capture the GSM AUTOCAL log packet results.

	Call QLIB_FTM_DO_GSM_AUTOCAL_GetResults() to get the results returned from the phone.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iPower	= The transmitter output power at PA-DAC value; to allow for added resolution, 
	the power is read in units of (dBm/100); hence, the user inputs a number that is dBm value measured x 100.

	\param	iPA_DAC_Value	= The DAC value in the external reference measurement that yielded the power in the next parameter.

	\param	iMax_Power	= The maximum power to which the Autocal curves need to be measured and stored; 
		to allow for added resolution, the power is read in units of (dBm/100); 
		hence, the user inputs a number that is dBm value measured x 100.
	\param  iMin_Power  = The minimum power to which the Autocal curves need to be measured and stored; 
		to allow for added resolution, the power is read in units of (dBm/100); 
		hence, the user inputs a number that is dBm value measured x 100.
	
	\param  iWriteNVFlag = Flag that specifies whether to write the curves and associated parameters to NV, and 
			also specifies to the frequency with which to associate the measured curve.
			 0 = Do not write NV
			 1 = Write curves and parameters to NV associated with frequency F1 of current band	
			 2 = Write curves and parameters to NV associated with frequency F1 of current band
	
	\param  iRSB_Cor_A_R0 = A measure of the receiver phase imbalance in gain range 0; 
		it is used by the system to correct the curves for this imbalance. 
		Currently, RSB parameters are not being measured. The default value to use is 0
	
	\param  iRSB_Cor_A_R1	= A measure of the receiver phase imbalance in gain range 1; 
		it is used by the system to correct the curves for this imbalance. 
		Currently, RSB parameters are not being measured. The default value to use is 0.
	
	\param  iRSB_Cor_B_R0 = A measure of the receiver phase and amplitude imbalance in gain range 0; 
		it is used by the system to correct the curves for this imbalance. 
		Currently, RSB parameters are not being measured. The default value to use is 10000.
	
	\param  iRSB_Cor_B_R1 = A measure of the receiver phase and amplitude imbalance in gain range 1; 
		it is used by the system to correct the curves for this imbalance. 
		Currently, RSB parameters are not being measured. The default value to use is 10000.

	\return true if successful, false if fail.

	\warning  

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_DO_GSM_AUTOCAL( 
								HANDLE hResourceContext, 
								short iPower, 
								unsigned short iPA_DAC_Value, 
								long iMax_Power, 
								long iMin_Power,
								unsigned char iWriteNVFlag, 
								unsigned short iRSB_Cor_A_R0, 
								unsigned short iRSB_Cor_A_R1,	
								unsigned short iRSB_Cor_B_R0, 
								unsigned short iRSB_Cor_B_R1);

	/******************************************************************************/
	/**
	Gets the logged results from QLIB_FTM_DO_GSM_AUTOCAL.  
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param piNumPoints = number of measured points.  The maximum size will be 500, as set
	                    by QMSL, so all arrays below should be at least 500 elements.
	                    
	
	\param aiDAC_Values = Array of DAC values used for the sweep. 
	                       Number of elements filled in is the same as iNumPoints 

	\param afAMAM_Values = Array of AMAM values. Number of elements filled in is the same as iNumPoints .
	\param afAMPM_Values = Array of AMpm values. Number of elements filled in is the same as iNumPoints .

	\return true if successful, false if fail.

	\warning  QLIB_FTM_DO_GSM_AUTOCAL must be called first otherwise the FTM2 logs will not be properly configured.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_DO_GSM_AUTOCAL_GetResults( 
								HANDLE hResourceContext, 
								unsigned short* piNumPoints,
								unsigned short aiDAC_Values[],
								float afAMAM_Values[],
								float afAMPM_Values[]
								);

	/******************************************************************************/
	/**
	Factory Test Mode GSM Polar Command 2.3 Set path delay
	
	This API is used to introduce delays into the phase or envelope paths in the MSM to ensure that 
	the waveform does not become distorted. If the delay is a positive number the envelope path is delayed. 
	When the delay is negative, the phase path is delayed. The delay is in units of 1/416 us ~ 2.403 ns. 
	This API returns the delay in a packet format that can then be stored in NV memory.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	
	\param  piDelay	= input/output, Positive delay -> envelope path delayed 
										Negative delay -> phase path delayed
	
	\return true if successful, false if fail.

	\warning  

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_PATH_DELAY( HANDLE hResourceContext, short *piDelay);

	/******************************************************************************/
	/**
	Factory Test Mode GSM Polar Command 2.3 Set path delay

	This function is the same as QLIB_FTM_SET_PATH_DELAY(), except the input
	and output delay values are separated into 2 variables instead of one.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	
	\param  iDelay_input	= input, Positive delay -> envelope path delayed 
										Negative delay -> phase path delayed

  	\param  piDelay_output	= output, Measured value returned from FTM operation
	                                    Positive delay -> envelope path delayed 
										Negative delay -> phase path delayed

	\return true if successful, false if fail.

	\warning  

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_PATH_DELAY_V2( 
				HANDLE hResourceContext, 
				short iDelay_input,
				short *piDelay_output );

	/******************************************************************************/
	/**
	Factory Test Mode GSM Polar Command 2.4 Enable Polar Ref Cal
	
	This API is used by the SetPADAC API in Polar targets (MSM6275). When issued with the enable parameter set to 
	true, it allows the user to populate the entire AMAM table with the same DAC value. This has the same effect 
	as writing directly to the PA DAC. In order to return to regular transmit operation, and use the 
	measured AMAM curves, this API must be called with the enable parameter set to false.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	
	\param  iEnable	=  1--enable , 0--disable
	
	\return true if successful, false if fail.

	\warning  

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_ENABLE_POLAR_REF_CAL( HANDLE hResourceContext,unsigned char iEnable);

	/******************************************************************************/
	/**
	Factory Test Mode GSM Polar Command 2.5 Set Two-tone Freq Offset
	
	This API is used in conjunction with the Set OPLL bandwidth API and the tx.? API in GSM to calibrate the 
	Tx bandwidth. The tx.? API can be used to transmit a two-tone waveform that has one tone at center frequency 
	and another tone offset from the center. The offset in frequency of the second tone from the first is set 
	by this API. The difference in amplitude between the two tones is dependent on the frequency response of the OPLL. 
	This can be measured and the Set OPLL Bandwidth API can be programmed to achieve the required response.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	
	\param  iFreqOffsetSecTone	= Frequency offset of second tone specified in kHz, e.g., 5 MHz, will be specified as 5000
	
	\return true if successful, false if fail.

	\warning  

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_TWOTONE_FREQ_OFFSET( HANDLE hResourceContext, unsigned short iFreqOffsetSecTone);

	/******************************************************************************/
	/**
	Factory Test Mode GSM Polar Command 2.6 Do Calpath RSB
	
	This API is used to measure the phase and amplitude imbalance, i.e., RSB or residual side band, in the calibration 
	path receiver. This API accepts two input parameters and returns two output parameters. The first parameter is a 
	subcommand ID, the second is a flag that should be set when the RSB measurement is completed, and the last two 
	parameters contain the results of the RSB measurement. The RSB measurement procedure involves multiple steps. 
	Each step is specified by the RSB subcommand ID.

	The procedure used to measure the imbalances is as follows:

	1.	Turn off input to the receiver and call the API with subcommand ID 0. Clear the RSB done flag. 
			This step ensures that the receiver DC is calibrated out (in gain range 0) 
			before the imbalances are measured.

	2.	Input a strong tone (>0 dBm), offset from the carrier by about 30 to 40 kHz, to the receiver. 
			Call the API with subcommand ID 4 with the RSB done flag cleared. This measures the RSB and returns 
			the correction values A and B. These parameters should be written to NV for use during Autocal measurements.
	
	3.	Turn off input to the receiver and call the API with subcommand ID 1 with the RSB done flag cleared. 
			This step ensures that the receiver DC is calibrated out (in gain range 1) before the imbalances are measured.
	
	4.	Repeat Step 2 but set the RSB done flag. This indicates that the measurement is completed and the 
			internal variables can be reset. If this flag is not set, the phone will be in an invalid state and 
			other APIs might not function.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	
	\param  iRSB_Subcmd_ID	=	0 = Do dc calibration in gain range 0
														1 = Do dc calibration in gain range 1
														2,3 = Unused
														4 = Measure RSB
	\param iDone_RSG = Done measuring RSB (Used to reset internal state)
	\param piRSB_A = Measured RSB correction value - A
	\param piRSB_B = Measured RSB correction value - B

	\return true if successful, false if fail.

	\warning  

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_DO_CALPATH_RSB(HANDLE hResourceContext,unsigned char iRSB_Subcmd_ID, unsigned char iDone_RSG, unsigned short* piRSB_A, unsigned short* piRSB_B);

	/******************************************************************************/
	/**
	Factory Test Mode GSM Polar Command 2.7 Set OPLL bandwidth
	
	This API is used to adjust the OPLL bandwidth. It is used in conjunction with the tx.? API and the Set 
	two-tone frequency offset API to adjust the 3-dB bandwidth of the OPLL. It accepts a parameter that takes values 
	between 1 and 9. The bandwidth increases with increasing values of this parameter.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	
	\param  iBandwidth_Num	= Number used to adjust OPLL bandwidth options are 1 to 9

	\return true if successful, false if fail.

	\warning  

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_OPLL_BW(HANDLE hResourceContext,unsigned char iBandwidth_Num);

	/******************************************************************************/
	/**
	Factory Test Mode GSM Polar Command 2.8 Baseband bandwidth Cal
	
	This API allows for calibration of the RF-baseband bandwidth. The bandwidth of the RF baseband can be set by 
	issuing this API with a parameter between 0 and 15. The number that results in the best transmit spectrum is 
	then written to NV for use by software.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	
	\param  iBandwidth_Num = Number used to increase the RF-baseband bandwidth
	
	\return true if successful, false if fail.

	\warning  

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_BASEBAND_BW_CAL(HANDLE hResourceContext,unsigned char iBandwidth_Num);


	/******************************************************************************/
	/**
	Factory Test Mode GSM Polar Command 2.9 Polar Tx Sweep Cal
	
	This API allows for AMAM and AMPM calibration of the PA. A sequence of DAC and Duration are the inputs.
	The transmitter transmits the sequence of given DAC values for the durations specified.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	
	\param  numDac   : size of  dacList

	\param  dacList  : Array of DACs to be transmitted

	\param  dacDuration : Array of dacDurations.
	
	\return true if successful, false if fail.

	\warning  none

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GSM_POLAR_TX_SWEEP_CAL(HANDLE hResourceContext,unsigned long numDac, unsigned long * dacList, unsigned long * dacDuration );

	/******************************************************************************/
	/**
	Factory Test Mode GSM Polar Command 2.10 TX_RSB_DC_SWEEP
	
	This API turns on the transmitter and sweeps through the specified RSB and DC correction
	parameters. Each set of correction parameters are programmed for the specified duration (in units
	of 1 symbol period or 3.69 us). The resulting waveform can be used in conjunction with external
	measurements to find the correction values that result in the lowest Tx RSB and Tx DC values.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param iListSize = Number of steps that will be in the sweep.  For each step, there is an element
	                   in the other input arrays (Alpha, Beta, IDC, QDC, Segement length)

	\param aiAlpha = input, Array of Alpha value set during first segment (one of the two Tx RSB correction parameters)
	\param aiBeta = input, Array of Beta value set during first segment (one of the two Tx RSB correction parameters)
	\param aiI_DC_Correction = input, Array of DC corrections on I channel 
	\param aiQ_DC_Correction = input, Array of DC corrections on Q channel 
	\param aiSegmentLength = input, Array of segment length values, units are 3.69 us (microseconds)
	\param piStatus = output, 0 - Success, 1 - Failure
	
	\return true if successful, false if fail.

	\warning  

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_TX_RSB_DC_SWEEP(
				HANDLE hResourceContext,
				unsigned char iListSize,
				short* aiAlpha,
				short* aiBeta,
				char* aiI_DC_Correction,
				char* aiQ_DC_Correction,
				unsigned short* aiSegmentLength,
				unsigned short* piStatus
				);
	
	/******************************************************************************/
	/**
	Factory Test Mode GSM Polar Command 2.11 GSM_DETECTOR_CAL
	
	This API is used to calibrate the transmit RF chipset's output power to expected levels. 

	This is required to ensure that process variations do not cause variation in output power across different phones. 
	
	This must be done prior to FTM_GSM_POLAR_TX_SWEEP_CAL, which is used to calibrate the PA's non-linearity. 


	In order to calibrate the RF chip's output power, the RF gain setting is varied and an internal power 
	detector is read. This command then searches through different gain settings until a required power detector 
	reading is obtained. 
	
	The inputs to this command are the required power detector setting (in units of unsigned 8-bits) 
	and a tolerance value. The command then settles to a power detector setting in the range - 
	Required detector settings - tolerance <= final reading <= Required detector settings + tolerance

	Once this calibration is completed, the resulting value is programmed to hardware.  

	In addition, the command computes the gain settings required for cold temperature and 
	hot temperature conditions.  These values are optionally stored in NV for future use during 
	temperature compensation. The command also accepts a flag specifying whether the resulting 
	gain settings (at cold, room and hot temperature) should be stored to NV. 
	
	When the flag is true, the results are stored to NV. 
	The NV item names used are:  
	\code
       NV_<band>_PA_TEMP_COMP_INDEX_10_I. 
          - Each NV item has an array of numbers where the results are stored. The order in which 
            the results are stored is illustrated below:

               - NV_<band>_PA_TEMP_COMP_INDEX_10_I.value[0] = cold temp setting 
			                  -                             = max( room temp setting - 20, 0)

               - NV_<band>_PA_TEMP_COMP_INDEX_10_I.value[1] = room temp setting.
               - NV_<band>_PA_TEMP_COMP_INDEX_10_I.value[2] =  high temp setting.
                                                            = min( room temp setting + 60, 255)
	\endcode
	\param iDetVal	= Input, This is the target power detector code to be achieved
	\param iTol		= Input, This is the tolerance allowed when searching for the target value.
	\param iStoreRes	= Input, 
		\code
            0-	Do not write the results to NV.
            1-	Write the results to NV.
		\endcode

	\param piVgaOut	= Output, Gain setting that causes the target power detector setting. This 
	              Output, is the "room temp setting" value that is written to NV.
	\param piDetOut	= Output, Final power detector setting that was achieved. This should be within +/- 
	              tolerance of required target, unless the search could not converge within 
				  the required limit.
	\param piStatus	=  Output,
	    \code
            0 - Success
            0xFFFF - Failure. The required target could not be achieved within the given tolerance.
		\endcode
	
	\return true if successful, false if fail.

	\warning  

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GSM_DO_TX_DETECTOR_CAL( 
										HANDLE hResourceContext,
										unsigned char iDetVal, 
										unsigned char iTol, 
										unsigned char iStoreRes, 
										unsigned char* piVgaOut, 
										unsigned char* piDetOut, 
										unsigned short* piStatus  
										);

	/******************************************************************************/
	/**
	Factory Test Mode GSM Polar Command 2.12 TX_4xFMOD_SWEEP
	
	This API is used to measure the RF setting that yields the lowest spur at the 4xfMod frequency. 
	The 4xfMod frequency is offset from the modulating frequency by 4 times the modulating frequency. 

	The modulating frequency used in this command is 13/192 MHz or 67.7kHz. Therefore this spur 
	would be at 67.7+4*67.7 = 5*67.7 kHz or at 67.7-4*67.7 = -3*67.7kHz. 
	
	Each possible set of RF settings that can affect this spur is identified by a unique index. 

	This command turns on the transmitter and steps through all possible indices. 

	The first step is considered a trigger and the RF setting for this step should be ignored. 
	In order to trigger the measurement instrument on the first step, the user must first call the 
	FTM_SET_TWOTONE_FREQ_OFFSET api to set the frequency offset to the 4xfMod frequency. 
	This will generate a tone at the 4xfmod frequency for the first step. 

	Following this, the remainder of the steps will each be programmed at one of the possible 
	RF indices. Thus steps 2 - N of the sweep will cycle through RF settings 1 to (N-1). 
	The command accepts the following 2 inputs:

		1.	The duration of each step in units of symbols (i.e 3.69us). The duration must not 
		    be set very low, or else the measuring instrument will not be able to distinguish 
			between steps. This number may be chosen based on the measurement resolution.

		2.	The trigger level (in units of DAC level). This turns on the transmitter to the 
		    specified DAC level. In the first step this transmit power will be at the frequency 
			offset specified by FTM_SET_TWOTONE_FREQ_OFFSET. Following the first step, this 
			transmit power will move to 67.7 kHz.

	The measurement box (spectrum analyzer) must be setup to measure the spur at the specified 
	4xfmod frequencies to capture this sweep. The user (or program that calls this API) may then 
	gather results of this sweep and write the best index to NV item name - NV_<band>_PA_TEMP_COMP_INDEX_11_I. 

	Note - This command will not write to NV. The user is responsible for measuring the sweep, 
	       finding the best index and writing that index to the above mentioned NV item.

	\param iSegLength		= Duration of each step in units of symbols per step
	\param iDACLevel		= DAC level of trigger step.

	\return true if successful, false if fail.

	\warning  

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GSM_TX_4X_FMOD_SWEEP( 
									HANDLE hResourceContext, 
									unsigned short iSegLength, 
									unsigned short iDACLevel  
									);


/******************************************************************************
						PMIC FTM
*******************************************************************************/


	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 2.1  RTC Set or Get Time Command

	Sets or gets the phone's real time clock.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param	iReadWrite = 0 - Get Time, 1 - Set Time
	\param	iTimeMode = 0 - 12 hour format (AM), 1 - 12 hour format (PM), 2 - 24 hour format
	\param	iMonth = Month to set/get (1...12), 1 - January...12 - December	
	\param	iDay = Day of Month.  Valid range is [0,31]
	\param	iYear = Year to set/get.  Valid range is [1980,2116]
 	\param	iHour = Hour field of RTC time to set/get.  Valid range depends on time_mode field.  
						For time_mode = 0 or 1, valid range is [1,12].  For time_mode = 2, valid range is [0,23].
	\param	iMinute = Minute field of RTC time to set/get.  Valid range is [0-59]
	\param  iSecond =Second field of RTC time to set/get.  Valid range is [0-59]

	\return true if successful, false if fail.

	\warning If a "Get" operation is selected then the "by reference" values will be updated.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_RTC_SET_GET_TIME
		( 
			HANDLE hResourceContext, unsigned char iReadWrite, unsigned char* iTimeMode, unsigned char* iMonth, 
			unsigned char* iDay, unsigned short* iYear, unsigned char* iHour, unsigned char* iMinute,
			unsigned char* iSecond
		);

	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 2.2  RTC Set or Get Alarm Time Command

	Sets or gets the phone's real time clock alarm.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param	iReadWrite = 0 - Get Alarm Time, 1 - Set Alarm Time
	\param  iAlarmId = 0 - RTC_ALARM_1, 1 - RTC_ALARM_2, 2 - RTC_ALARM_3, 
	        Note: RTC_ALARM_2 and RTC_ALARM_3 are not currently supported.

	\param	iTimeMode = 0 - 12 hour format (AM), 1 - 12 hour format (PM), 2 - 24 hour format
	\param	iMonth = Month to set/get (1...12), 1 - January...12 - December	
	\param	iDay = Day of Month.  Valid range is [0,31]
	\param	iYear = Year to set/get.  Valid range is [1980,2116]
 	\param	iHour = Hour field of RTC time to set/get.  Valid range depends on time_mode field.  
					For time_mode = 0 or 1, valid range is [1,12].  For time_mode = 2, valid range is [0,23].
	\param	iMinute = Minute field of RTC time to set/get.  Valid range is [0-59]
	\param  iSecond =Second field of RTC time to set/get.  Valid range is [0-59]

	\return true if successful, false if fail.

	\warning If a "Get" operation is selected then the "by reference" values will be updated.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_RTC_SET_GET_ALARM_TIME( 
							HANDLE hResourceContext, unsigned char iReadWrite, unsigned char* iAlarmId, 
							unsigned char* iTimeMode, unsigned char* iMonth, 
							unsigned char* iDay, unsigned short* iYear, unsigned char* iHour, unsigned char* iMinute,
							unsigned char* iSecond);


	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 2.3  RTC Stop Command

	Stop the real time clock 

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_RTC_STOP( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 2.4  RTC Get Alarm Status Command

	Get the alarm status

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iAlarmStatus = Status bits.  Alarm 1 trigger status is returned in bit 0 of alarm_status.  
	                       Alarms 2 and 3 are not currently supported, so alarm_status[7:1] 
						   will always be 'b0000000.

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_RTC_GET_ALARM_STATUS( HANDLE hResourceContext, unsigned char* iAlarmStatus );

	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 2.5  RTC Disable Alarm Command

	Disable a RTC alarm

	\param iAlarmId = Alarm to disable, 0 - RTC_ALARM_1, 1 - RTC_ALARM_2, 2 - RTC_ALARM_3
		              Note: RTC_ALARM_2 and RTC_ALARM_3 are not currently supported.  

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_RTC_DISABLE_ALARM( HANDLE hResourceContext, unsigned char iAlarmId );


	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 2.6  RTC Set/Get Time Adjust Command

	Get or set the RTC time adjustment factor

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iReadWrite = 0 - Get Time Adjust, 1 - Set Time Adjust
	\param iTimeAdjust = 32 kHz clock adjustment to set in RTC hardware, or get the current value 
	                          from hardware.  Valid range is [0,127].  Actual adjustment is 
							  (time_adjust-64)*3.05ppm

	\return true if successful, false if fail.

	\warning if iReadWrite = 0, then the by-reference value in iTimeAdjust will be updated

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_RTC_SET_GET_TIME_ADJUST
		( HANDLE hResourceContext, unsigned char iReadWrite, unsigned char* iTimeAdjust );

	
	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 3.1  Set Charger Switch Command

	Configure the charger switch

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param   iChargerSwitchType = 
	\code
               0 - CHG_SWITCH_WALL_CHGR
               1 - CHG_SWITCH_BATTERY_TRANSISTOR
               2 - CHG_SWITCH_WALL_PULSE_CHGR
               3 - CHG_SWITCH_VCP
               4 - CHG_SWITCH_BATT_ALARM
               5 - CHG_SWITCH_COIN_CELL
               6 - CHG_SWITCH_USB_CHGR
               7 - CHG_SWITCH_APP_CODE

	\endcode
               Note.  In the case of chg_switch_type = 7, this represents the application level code 
			   that handles the charger state machine.  

			   Disabling this switch resets the state machine and initializes the charger to a default state.  

	\param   iChargerSwitchCommand = 0 - Disable Switch, 1 - Enable Switch  
		
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_CHG_SET_SWITCH_STATE
		( HANDLE hResourceContext, unsigned char iChargerSwitchType, unsigned char iChargerSwitchCommand );


	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 3.2.1  Wall Pulse Charger Config

	Configure wall pulse charging

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iChargerType = 0 - Q_LOW_SIDE, 1 - Q_HIGH_SIDE
	\param iTimeOn = Time on during pulse cycle
		\code
			0 - TON_62p5_MS
			1 - TON_125_MS
			2 - TON_250_MS
			3 - TON_500_MS

		\endcode
	\param iTimeOff = Time off during pulse cycle
		\code
			0 - TOFF_62p5_MS
			1 - TOFF_125_MS
			2 - TOFF_250_MS
			3 - TOFF_500_MS

		\endcode
	\param iTimeDone = Represents a t_done time as a multiple of the configured t_off time.
		\code
				0 - TOFF_X_16
				1 - TOFF_X_32
				2 - TOFF_X_64
				3 - TOFF_X_128

		\endcode
		
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_CONFIG_WALL_PULSE_CHGR ( 
					HANDLE hResourceContext, unsigned char iChargerType, 
					unsigned char iTimeOn, unsigned char iTimeOff, unsigned char iTimeDone  
					);


	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 3.2.2  VBATT detect voltage Config

	Voltage threshold to enable wall pulse charging

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iVbattDetectVoltage = Voltage level in mV
		
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_CONFIG_WALL_PULSE_CHGR_VBAT_DET 
		( HANDLE hResourceContext, unsigned char iVbattDetectVoltage );


	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 3.2.3  Wall Charger Transistor Limits Config

	Configure the voltage and current limits of the wall charger transistor

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iChargerMaxCurrent = Maximum current throught the wall charger.  
	                                 The current is represented by a voltage across a sense resistor, typically 0.1 ohm.
		\code			

        0 - (010mV)/R
        1 - (020mV)/R
        2 - (030mV)/R
        3 - (040mV)/R
        4 - (050mV)/R
        5 - (060mV)/R
        6 - (070mV)/R
        7 - (080mV)/R
        8 - (090mV)/R
        9 - (100mV)/R
        10 - (110mV)/R
        11 - (120mV)/R
        12 - (130mV)/R
        13 - (140mV)/R
        14 - (150mV)/R
        15 - (160mV)/R
        16 - (170mV)/R
        17 - (180mV)/R
        18 - (190mV)/R
        19 - (200mV)/R
        20 - (210mV)/R
        21 - (220mV)/R
        22 - (230mV)/R
        23 - (240mV)/R
        24 - (250mV)/R
        25 - (260mV)/R
        26 - (270mV)/R
        27 - (280mV)/R
        28 - (290mV)/R
        29 - (300mV)/R

		\endcode

	\param iChargerMaxVoltage = Maximum wall transistor voltage.
		\code
			0 - VMAXSEL_4000mV
			1 - VMAXSEL_4025mV
			2 - VMAXSEL_4050mV
			3 - VMAXSEL_4075mV
			4 - VMAXSEL_4100mV
			5 - VMAXSEL_4125mV
			6 - VMAXSEL_4150mV
			7 - VMAXSEL_4175mV
			8 - VMAXSEL_4200mV
			9 - VMAXSEL_4225mV
			10 - VMAXSEL_4250mV
			11 - VMAXSEL_4275mV
			12 - VMAXSEL_4300mV
			13 - VMAXSEL_4325mV
			14 - VMAXSEL_4350mV
			15 - VMAXSEL_4375mV

		\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_CONFIG_WALL_CHGR_TRANSISTOR_LIMITS 
		( HANDLE hResourceContext, unsigned char iChargerMaxCurrent, unsigned char iChargerMaxVoltage );

	
	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 3.2.4  Trickle Charger Config

	Configure the trickle charger limits

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iChargerMaxCurrent = Maximum trickle charger current.
		\code			
			0 - IMAX_00mA
			1 - IMAX_20mA
			2 - IMAX_30mA
			3 - IMAX_40mA
			4 - IMAX_50mA
			5 - IMAX_60mA
			6 - IMAX_70mA
			7 - IMAX_80mA

		\endcode


	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_CONFIG_TRICKLE_CHARGER
		( HANDLE hResourceContext, unsigned char iChargerMaxCurrent );


	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 3.2.5  Coin Cell Charger Config

	Set the coin cell charger configuratin

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iVoltageSetting = Coin Cell voltage setting
		\code			
			0 - VSET_3p0V
			1 - VSET_3p1V
			2 - VSET_3p2V.

		\endcode
	\param iResistorSetting = Coin Cell resistor setting
		\code
			0 - RSET_2100_Ohms
			1 - RSET_1700_Ohms
			2 - RSET_1200_Ohms
			3 - RSET_800_Ohms

		\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_CONFIG_COIN_CELL_CHGR
		( HANDLE hResourceContext, unsigned char iVoltageSetting, unsigned char iResistorSetting );


	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 3.2.6  Battery Alarm Config

	Voltage threshold to enable wall pulse charging

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iUpperLimit  = Upper threshold voltage for battery alarm (mV)
	\param iLowerLimit  = Lower threshold voltage for battery alarm (mV)
	\param iHysteresisSetting  = Hysteresis setting.
		\code
			0 - HYST_122us
			1 - HYST_244us
			2 - HYST_488us
			3 - HYST_976us

		\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_CONFIG_BATT_ALARM( HANDLE hResourceContext, unsigned short iUpperLimit, 
														unsigned short iLowerLimit, unsigned char iHysteresisSetting );


	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 3.2.7  USB Charger Transistor Limit Config

	Configure the USB charger transistor

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iMaxCurrent  = Maximum current through USB pass transistor.  
	                           Current is given as a voltage across a sense resistor, typically 0.1 ohm.
		\code
			0 - IMAX_010mV_div_R
			1 - IMAX_020mV_div_R
			2 - IMAX_030mV_div_R
			3 - IMAX_040mV_div_R
			4 - IMAX_050mV_div_R
			5 - IMAX_060mV_div_R
			6 - IMAX_070mV_div_R
			7 - IMAX_080mV_div_R
			8 - IMAX_090mV_div_R
			9 - IMAX_100mV_div_R
			10 - IMAX_110mV_div_R
			11 - IMAX_120mV_div_R
			12 - IMAX_130mV_div_R
			13 - IMAX_140mV_div_R
			14 - IMAX_150mV_div_R
			15 - IMAX_160mV_div_R
			16 - IMAX_170mV_div_R
			17 - IMAX_180mV_div_R
			18 - IMAX_190mV_div_R
			19 - IMAX_200mV_div_R

		\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_CONFIG_USB_TRANSISTOR_LIMITS
		( HANDLE hResourceContext, unsigned char iMaxCurrent );


	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 3.2.8  Charger Power Control Limit Config

	Configure the charger power control limit

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iChargerType = Select Charger type to configure.
		\code
			0 - WALL_CHARGER
			1 - USB_CHARGER

		\endcode
	
	\param iMaxPower = Maximum power delivered by charger.
		\code
			0 - PMAXSEL_DISABLE
			1 - PMAXSEL_400mW
			2 - PMAXSEL_500mW
			3 - PMAXSEL_600mW
			4 - PMAXSEL_750mW
			5 - PMAXSEL_1000mW
			6 - PMAXSEL_1500mW
			7 - PMAXSEL_2000mW

		\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_CONFIG_PWR_CTRL_LIMIT( 
							HANDLE hResourceContext, 
							unsigned char iChargerType, 
							unsigned char iMaxPower );


	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 4.1  Configure Analog Mux

	Configure the analog multiplexer

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iMuxSelect = Select AMUX channel to route to HKADC block. 
	                          Built-in prescaler value shown in parenthesis.
		\code
			0 - ADC_MUXSEL0 (2/3)
			1 - ADC_MUXSEL1 (1/2)
			2 - ADC_MUXSEL2 (1/8)
			3 - ADC_MUXSEL3 (none)
			4 - ADC_MUXSEL4 (1/2)
			5 - ADC_MUXSEL5 (none)
			6 - ADC_MUXSEL6 (none)
			7 - ADC_MUXSEL7 (none)
			8 - ADC_MUXSEL8 (none)
			9 - ADC_MUXSEL9 (none)
			10 - ADC_MUXSEL10 (2/5)
			11 - ADC_MUXSEL11 (none)
			12 - ADC_MUXSEL12 (none)
			13 - ADC_MUXSEL13 (none)
			14 - ADC_MUXSEL14 (none)
			15 - ADC_MUXSEL15 (disables AMUX)

		\endcode
	
	\param iMuxScale = Offset and gain settings to be applied to the input voltage.  Offset and gain applied after prescaler stage.  
							(Offset [Volts],Gain) shown in parenthesis
		\code
			0 - ADC_MUXSCALE0        (0, 1)
			1 - ADC_MUXSCALE1        (0, 1.25)
			2 - ADC_MUXSCALE2        (0, 1.667)
			3 - ADC_MUXSCALE3        (0, 2.5)
			4 - ADC_MUXSCALE4        (0, 5)
			5 - ADC_MUXSCALE5        (0.5, 1.25)
			6 - ADC_MUXSCALE6        (0.5, 1.667)
			7 - ADC_MUXSCALE7        (0.5, 2.5)
			8 - ADC_MUXSCALE8        (0.5, 5)
			9 - ADC_MUXSCALE9        (1, 1.667)
			10 - ADC_MUXSCALE10      (1, 2.5)
			11 - ADC_MUXSCALE11      (1, 5)
			12 - ADC_MUXSCALE12      (1.5, 2.5)
			13 - ADC_MUXSCALE13      (1.5, 5)
			14 - ADC_MUXSCALE14      (2, 5)
			15 - ADC_MUXSCALE15      (0, 1) [bypass mode]

		\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_AMUX_CONFIG
		( HANDLE hResourceContext, unsigned char iMuxSelect, unsigned char iMuxScale );

	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 6.1  Enable/Disable VREG Command

	Enable/disable a voltage regulator

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iVRegCmd   = 0 - Disable selected regulators, 1 - Enable selected regulators

	\param iVRegSelect = Voltage regulators to perform the vreg_cmd on.  This can be a logical 
	                          OR of any of the following voltage regulator masks.
		\code
			0x00000004 - VREG_MSME
			0x00000008 - VREG_MSMC1
			0x00000010 - VREG_MSMC2
			0x00000020 - VREG_MSMS
			0x00000040 - VREG_MEM
			0x00000080 - VREG_GP
			0x00000100 - VREG_CAM
			0x00000200 - VREG_TCXO
			0x00000400 - VREG_PA
			0x00000800 - VREG_RFTX
			0x00001000 - VREG_RFRX1
			0x00002000 - VREG_RFRX2
			0x00004000 - VREG_SYNT
			0x00008000 - VREG_WLAN
			0x00010000 - VREG_USB
			0x00020000 - VREG_BOOST
			0x00040000 - VREG_MMC
			0x00080000 - VREG_RUIM

		\endcode
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_VREG_CONTROL
		( HANDLE hResourceContext, unsigned char iVRegCmd, unsigned long iVRegSelect );


	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 6.2  Set Voltage Level Command

	Set the voltage level of a regulator

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iVRegSelect = Voltage regulator to set a desired output level.
		\code
			0 - VREG_MSMA
			1 - VREG_MSMP
			2 - VREG_MSME
			3 - VREG_MSMC1
			4 - VREG_MSMC2
			5 - VREG_MSMS
			6 - VREG_MEM
			7 - VREG_GP
			8 - VREG_CAM
			9 - VREG_TCXO
			10 - VREG_PA
			11 - VREG_RFTX
			12 - VREG_RFRX1
			13 - VREG_RFRX2
			14 - VREG_SYNT
			15 - VREG_WLAN
			16 - VREG_USB
			17 - VREG_BOOST
			18 - VREG_MMC
			19 - VREG_RUIM

		\endcode
	
	\param iVoltageLevel = Voltage level in millivolts to configure the selected regulator.  
		\code
				Valid input range depends on the selected regulator ID.  

				For VREG_PA,VREG_MSME,VREG_MSMC1,VREG_MSMC2:
					Range = [750, 1525] in 25 mV steps
								   [1550,3050] in 50 mV steps
				For VREG_BOOST and VREG_USB
					Range = [3000, 6100] in 100 mV steps
				For all other regulators
					Range = [1500, 3100] in 50 mV steps.
				NOTE: Command will support any mV input level between the lower and upper limits, but this will get rounded down 
						by PMIC driver API to the closest voltage the regulator can support (step size dependent).  
						
					Example:  User selection of VREG_PA = 2940 mV will get converted to 2900 mV.

		\endcode
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_VREG_SET_LEVEL
		( HANDLE hResourceContext, unsigned char iVRegSelect, unsigned short iVoltageLevel  );

	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 6.X  Get Voltage Level Command

	Get the voltage level of a regulator

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iVRegSelect = Voltage regulator to be queried.
		\code
			0 - VREG_MSMA
			1 - VREG_MSMP
			2 - VREG_MSME
			3 - VREG_MSMC1
			4 - VREG_MSMC2
			5 - VREG_MSMS
			6 - VREG_MEM
			7 - VREG_GP
			8 - VREG_CAM
			9 - VREG_TCXO
			10 - VREG_PA
			11 - VREG_RFTX
			12 - VREG_RFRX1
			13 - VREG_RFRX2
			14 - VREG_SYNT
			15 - VREG_WLAN
			16 - VREG_USB
			17 - VREG_BOOST
			18 - VREG_MMC
			19 - VREG_RUIM

		\endcode
	
	\param iVoltageLevel = Voltage level in millivolts returned from the selected regulator.  
		\code
				Valid input range depends on the selected regulator ID.  

				For VREG_PA,VREG_MSME,VREG_MSMC1,VREG_MSMC2:
					Range = [750, 1525] in 25 mV steps
								   [1550,3050] in 50 mV steps
				For VREG_BOOST and VREG_USB
					Range = [3000, 6100] in 100 mV steps
				For all other regulators
					Range = [1500, 3100] in 50 mV steps.
				NOTE: Command will support any mV input level between the lower and upper limits, but this will get rounded down 
						by PMIC driver API to the closest voltage the regulator can support (step size dependent).  
						
					Example:  User selection of VREG_PA = 2940 mV will get converted to 2900 mV.

		\endcode
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_VREG_GET_LEVEL
		( HANDLE hResourceContext, unsigned char iVRegSelect, unsigned short* iVoltageLevel  );

	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 6.3  Config SMPS Mode

	Configure SMPS regulator mode.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iVRegSmps = Voltage regulator to configure
		\code
			0 - SMPS_PA
			1 - SMPS_MSME
			2 - SMPS_MSMC1
			3 - SMPS_MSMC2
			4 - SMPS_BOOST

		\endcode
	
	\param iSmpsMode = 
		\code
			0 - SMPS_MODE_TCXO_EN
			1 - SMPS_MODE_PWM
			2 - SMPS_MODE_PBM
			3 - SMPS_MODE_AUTOMATIC

		\endcode
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_VREG_CONFIG_SMPS_MODE
		( HANDLE hResourceContext, unsigned char iVRegSmps, unsigned char iSmpsMode  );


	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 6.4  Config LDO Powerdown Switch

	Configure LDO power down functionality

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iVRegCmd = 
		\code
			0 - Disable LDO Powerdown functionality
			1 - Enable LDO Powerdown functionality

		\endcode
	
	\param iVRegSelect = 
		\code
			0 - PDOWN_MSMP
			1 - PDOWN_MSMA
			2 - PDOWN_WLAN
			3 - PDOWN_RFRX2
			4 - PDOWN_RFRX1
			5 - PDOWN_RFTX
			6 - PDOWN_SYNT
			7 - PDOWN_TCXO
			8 - PDOWN_USB
			9 - PDOWN_MMC
			10 - PDOWN_RUIM
			11 - PDOWN_MSMS
			12 - PDOWN_MEM
			13 - PDOWN_CAM
			14 - PDOWN_GP 

		\endcode
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_VREG_CONFIG_LDO_PD_SW
		( HANDLE hResourceContext, unsigned char iVRegCmd, unsigned char iVRegSelect );


	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 6.5  Config Low Power Mode

	Configure LDO low power functionality

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iVRegCmd = 
		\code
			0 - Disable Low power functionality
			1 - Enable Low power functionality

		\endcode
	
	\param iVRegSelect = 
		\code
			0 - LP_MSMA
			1 - LP_MSMP 

		\endcode
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_VREG_CONFIG_LP_MODE_CONTROL
		( HANDLE hResourceContext, unsigned char iVRegCmd, unsigned char iVRegSelect);

	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 6.6  Config SMPS Clock

	Configure SMPS clock

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSmpsClockCmd = 
		\code
			0 - Select source for SMPS clock
			1 - Select TCXO Divider for SMPS clock

		\endcode
	
	\param iSmpsClockConfig = 
		\code
			For vreg_smps_clk_cmd = 0
			0 - SMPS_CLK_SEL_TCXO
			1 - SMPS_CLK_SEL_RC

			For vreg_smps_clk_cmd = 1
			0 - SMPS_TCXO_DIV_8
			1 - SMPS_TCXO_DIV_10
			2 - SMPS_TCXO_DIV_12
			3 - SMPS_TCXO_DIV_14
			4 - SMPS_TCXO_DIV_16
			5 - SMPS_TCXO_DIV_18

		\endcode
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_VREG_SMPS_CLK_CONFIG
		( HANDLE hResourceContext, unsigned char iSmpsClockCmd, unsigned char iSmpsClockConfig );

	
	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 7.1  Get Real Time Status Command

	Get interrupt status

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iInterruptStatusId= 
		\code
			0 - VALID_CHG_RT_ST       
			1 - INVALID_CHG_RT_ST
			2 - VALID_USB_CHG_RT_ST      
			3 - INVALID_USB_CHG_RT_ST    
			4 - CHG_I_LIMITING_RT_ST     
			5 - CHG_P_LIMITING_RT_ST     
			6 - PULSE_CHG_DONE_RT_ST    
			7 - VCP_IRQ_RT_ST            
			8 - T_STAT_CHANGED_RT_ST    
			9 - BAT_STAT_CHANGED_RT_ST 
			10 - VBAT_DET_RT_ST           
			11-  BAT_FET_ON_RT_ST         
			12 - KPD_PWR_KEY_ON_RT_ST  
			13 - KPD_PWR_KEY_OFF_RT_ST   
			14 - RTC_ALRM_RT_ST           
			15 - OSC_HALT_RT_ST           
			16 - PWR_RST_RT_ST            
			17 - SMPL_RT_ST               
			18 - CABLE_IN_RT_ST           
			19 - OVER_TEMP_RT_ST          
			20 - WDOG_TOUT_RT_ST        
			21 - KPD_PWRON_EVENT_RT_ST    
			22 - USB_BUS_VALID_RT_ST      
			23 - USB_SN_VALID_RT_ST   
			24 - USB_D_PLUS_HI_RT_ST  
			25 - USB_ID_GND_RT_ST 
			26 - USB_D_MINUS_HI_RT_ST 
			27 - USB_ID_FLOATING_RT_ST
			28 - USB_BOFF_AON_RT_ST   
			29 - USB_CARKIT_INT_RT_ST 
			38 - MPP01_CHGED_RT_ST
			39 - MPP02_CHGED_RT_ST
			40 - MPP03_CHGED_RT_ST
			41 - MPP04_CHGED_RT_ST
			42 - MPP05_CHGED_RT_ST
			43 - MPP06_CHGED_RT_ST
			44 - MPP07_CHGED_RT_ST
			45 - MPP08_CHGED_RT_ST
			46 - MPP09_CHGED_RT_ST   
			47 - MPP10_CHGED_RT_ST
			48 - MPP11_CHGED_RT_ST
			49 - MPP12_CHGED_RT_ST
			50 - MPP13_CHGED_RT_ST
			51 - MPP14_CHGED_RT_ST
			52 - MPP15_CHGED_RT_ST
			53 - MPP16_CHGED_RT_ST

		\endcode

	\param piInterruptCmd = Status returned.
		\code
			0 - Interrupt event real time status not triggered
			1 - Interrupt event real time status triggered

		\endcode
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_INT_GET_RT_STATUS
		( HANDLE hResourceContext, unsigned char iInterruptStatusId, unsigned char* piInterruptStatus );

	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands 7.2  Clear IRQ Command

	Clear the interrupt status for a specified handler

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iInterruptHandler= 
		\code
			0 - VALID_CHG_RT_ST       
			1 - INVALID_CHG_RT_ST
			2 - VALID_USB_CHG_RT_ST      
			3 - INVALID_USB_CHG_RT_ST    
			4 - CHG_I_LIMITING_RT_ST     
			5 - CHG_P_LIMITING_RT_ST     
			6 - PULSE_CHG_DONE_RT_ST    
			7 - VCP_IRQ_RT_ST            
			8 - T_STAT_CHANGED_RT_ST    
			9 - BAT_STAT_CHANGED_RT_ST 
			10 - VBAT_DET_RT_ST           
			11-  BAT_FET_ON_RT_ST         
			12 - KPD_PWR_KEY_ON_RT_ST  
			13 - KPD_PWR_KEY_OFF_RT_ST   
			14 - RTC_ALRM_RT_ST           
			15 - OSC_HALT_RT_ST           
			16 - PWR_RST_RT_ST            
			17 - SMPL_RT_ST               
			18 - CABLE_IN_RT_ST           
			19 - OVER_TEMP_RT_ST          
			20 - WDOG_TOUT_RT_ST        
			21 - KPD_PWRON_EVENT_RT_ST    
			22 - USB_BUS_VALID_RT_ST      
			23 - USB_SN_VALID_RT_ST   
			24 - USB_D_PLUS_HI_RT_ST  
			25 - USB_ID_GND_RT_ST 
			26 - USB_D_MINUS_HI_RT_ST 
			27 - USB_ID_FLOATING_RT_ST
			28 - USB_BOFF_AON_RT_ST   
			29 - USB_CARKIT_INT_RT_ST 
			38 - MPP01_CHGED_RT_ST
			39 - MPP02_CHGED_RT_ST
			40 - MPP03_CHGED_RT_ST
			41 - MPP04_CHGED_RT_ST
			42 - MPP05_CHGED_RT_ST
			43 - MPP06_CHGED_RT_ST
			44 - MPP07_CHGED_RT_ST
			45 - MPP08_CHGED_RT_ST
			46 - MPP09_CHGED_RT_ST   
			47 - MPP10_CHGED_RT_ST
			48 - MPP11_CHGED_RT_ST
			49 - MPP12_CHGED_RT_ST
			50 - MPP13_CHGED_RT_ST
			51 - MPP14_CHGED_RT_ST
			52 - MPP15_CHGED_RT_ST
			53 - MPP16_CHGED_RT_ST

		\endcode
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_INT_IRQ_CLR( HANDLE hResourceContext, unsigned char iInterruptHandler );



	/******************************************************************************/
	/**
		Factory Test Mode PMIC Commands 8.1  Set PMIC Pin Drive Level

		The PMIC has three high strength drive pins that are connected to the LCD 
		backlight (LCD_DRV pin), the keypad LEDs (KPD_DRV pin), and the vibrator 
		(VIB_DRV pin).  Each pin has a programmable drive level as described below.

		FTM_SET_PMIC_DRIVE_LEVEL allows selection of one of three high drive pins 
		and selection of the drive level.  A drive level selection of 0 will turn 
		off the pin drive.

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iHighDrivePinID =  Selects pin for drive level programming, values for this field are:
		\code
              0 = LCD_DRV pin
              1 = KPD_DRV pin
              2 = VIB_DRV pin
		\endcode

		\param iDriveLevel = Programs drive level for selected pin:
		\code
              a) LCD_DRV pin current = drive_level * 10mA.  Valid range is 0-15
              b) KPD_DRV pin current = drive_level * 10mA.  Valid range is 0-15
              c) VIB_DRV pin voltage = drive_level  * 100mV.  Valid range is 0 (off) or 12-31.
		\endcode
	
		\return true if successful, false if fail.

		\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_SET_PMIC_DRIVE_LEVEL
		( HANDLE hResourceContext, unsigned char iHighDrivePinID, unsigned char iDriveLevel );

	/******************************************************************************/
	/**
		Factory Test Mode PMIC Commands 8.2  Configure PMIC Speaker Path
	
		This command will configure the portion of the audio path that goes through 
		the PMIC.  The command will support commands to enable/disable the speaker, 
		to set the speaker gain, and to set the speaker delay.  

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iSpeakerCMD = 
			\code
                0 - SPKR_DISABLE
                1 - SPKR_ENABLE
                2 - SPKR_MUTE_OFF
                3 - SPKR_MUTE_ON
                4 - SPKR_OFF
                5 - SPKR_ON
			\endcode
		\param iSpeakerGain = 
			\code
                0 - SPKR_GAIN_MINUS_16DB
                1 - SPKR_GAIN_MINUS_12DB
                2 - SPKR_GAIN_MINUS_08DB
                3 - SPKR_GAIN_MINUS_04DB
                4 - SPKR_GAIN_00DB
                5 - SPKR_GAIN_PLUS_04DB
                6 - SPKR_GAIN_PLUS_08DB
                7 - SPKR_GAIN_PLUS_12DB
			\endcode

		\param iSpeakerDelay
			\code
                0 - SPKR_DELAY_10MS
                1 - SPKR_DELAY_100MS
			\endcode
	
		\return true if successful, false if fail.

		\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_SPKR_CONFIG ( 
						HANDLE hResourceContext, unsigned char iSpeakerCMD, 
						unsigned char iSpeakerGain, unsigned char iSpeakerDelay 
						);

	/******************************************************************************/
	/**
		Factory Test Mode PMIC Commands 9.1  Set digital in command

		This command will configure the selected Multi Purpose pin (MPP) to be a digital input pin.
		
		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iMPP =	Which MPP we want to configure. Values for this field are:

						\code
						0 - PM_MPP_1	
						1 - PM_MPP_2 
						2 - PM_MPP_3
						= PM_NUM_MPP_PM6620
						= PM_NUM_MPP_PM6640
						= PM_NUM_MPP_PANORAMIX
						...
						...
						...
						PM_MPP_INVALID

						\endcode
		
		\param iLevel = The logic level reference that we want to use with this MPP. Values for this field are:

						\code
						0 - PM_MPP__DLOGIC__LVL_MSME 
						1 - PM_MPP__DLOGIC__LVL_MSMP
						2 - PM_MPP__DLOGIC__LVL_RUIM
						3 - PM_MPP__DLOGIC__LVL_MMC
						4 - PM_MPP__DLOGIC__LVL_VDD
						\endcode

		\param piStatus = Return status
						\code
						0 = COMMAND_SUCESS
						>0 = Error code
						\endcode
	
		\return true if successful, false if fail.

		\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_MPP_CONFIG_DIGITAL_IN(
						HANDLE hResourceContext,
						unsigned char iMpp, 
						unsigned char iLevel, 
						unsigned short * piStatus);

	/******************************************************************************/
	/**
		Factory Test Mode PMIC Commands 9.2  Set digital out command

		This command will configure the selected Multi Purpose pin (MPP) to be a digital output pin.
		
		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iMPP =	Which MPP we want to configure. Values for this field are:

						\code
						0 - PM_MPP_1	
						1 - PM_MPP_2 
						2 - PM_MPP_3
						= PM_NUM_MPP_PM6620
						= PM_NUM_MPP_PM6640
						= PM_NUM_MPP_PANORAMIX
						...
						...
						...
						PM_MPP_INVALID

						\endcode
		
		\param iLevel = The logic level reference that we want to configure. Values for this field are:

						\code
						0 - PM_MPP__DLOGIC__LVL_MSME 
						1 - PM_MPP__DLOGIC__LVL_MSMP
						2 - PM_MPP__DLOGIC__LVL_RUIM
						3 - PM_MPP__DLOGIC__LVL_MMC
						4 - PM_MPP__DLOGIC__LVL_VDD
						\endcode
		\param iCtrl = The logic level reference that we want to use with this MPP. Values for this field are:
						
						\code
						0 - PM_MPP__DLOGIC_OUT__CTRL_LOW,    MPP OUTPUT = LOGIC LOW 
						1 - PM_MPP__DLOGIC_OUT__CTRL_HIGH,    MPP OUTPUT = LOGIC HIGH
						2 - PM PM_MPP__DLOGIC_OUT__CTRL_MPP,    MPP OUTPUT = CORRESPONDING MPP INPUT (see the note regarding MPP pairs within the description section)
						3 - PM_MPP__DLOGIC_OUT__CTRL_NOT_MPP,   MPP OUTPUT  = CORRESPONDING INVERTED MPP INPUT (see the note regarding MPP pairs within the description section)
						\endcode

		\param piStatus = Return status
						\code
						0 = COMMAND_SUCESS
						>0 = Error code
						\endcode
	
		\return true if successful, false if fail.

		\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_MPP_CONFIG_DIGITAL_OUT(
						HANDLE hResourceContext,
						unsigned char iMpp, 
						unsigned char iLevel, 
						unsigned char iCtrl, 
						unsigned short * piStatus);

	/******************************************************************************/
	/**
		Factory Test Mode PMIC Commands 9.3  Set digital inout command

		This command will configure the selected Multi Purpose pin (MPP) to be a digital bidirectional pin.
		
		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iMPP =	Which MPP we want to configure. Values for this field are:

						\code
						0 - PM_MPP_1	
						1 - PM_MPP_2 
						2 - PM_MPP_3
						= PM_NUM_MPP_PM6620
						= PM_NUM_MPP_PM6640
						= PM_NUM_MPP_PANORAMIX
						...
						...
						...
						PM_MPP_INVALID

						\endcode
		
		\param iLevel = The logic level reference that we want to configure. Values for this field are:

						\code
						0 - PM_MPP__DLOGIC__LVL_MSME 
						1 - PM_MPP__DLOGIC__LVL_MSMP
						2 - PM_MPP__DLOGIC__LVL_RUIM
						3 - PM_MPP__DLOGIC__LVL_MMC
						4 - PM_MPP__DLOGIC__LVL_VDD
						\endcode
		\param iPup = The pull-up resistor setting of the selected MPP. Values fro this field are:
						
						\code
						0 - PM_MPP__DLOGIC_INOUT__PUP_1K          PULL UP = 1  K Ohms
						1 - PM_MPP__DLOGIC_INOUT__PUP_3K          PULL UP = 3  K Ohms
						2 - PM_MPP__DLOGIC_INOUT__PUP_10K        PULL UP = 10  K Ohms
						3 - PM_MPP__DLOGIC_INOUT__PUP_30K        PULL UP = 30  K Ohms

						\endcode

		\param piStatus = Return status
						\code
						0 = COMMAND_SUCESS
						>0 = Error code
						\endcode
	
		\return true if successful, false if fail.

		\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_MPP_CONFIG_DIGITAL_INOUT(
						HANDLE hResourceContext,
						unsigned char iMpp, 
						unsigned char iLevel, 
						unsigned char iPup, 
						unsigned short * piStatus);

	/******************************************************************************/
	/**
		Factory Test Mode PMIC Commands 9.4  Set analog in command

		This command will configure the selected Multi Purpose pin (MPP) to be an analog input pin.
		
		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iMPP =	Which MPP we want to configure. Values for this field are:

						\code
						0 - PM_MPP_1	
						1 - PM_MPP_2 
						2 - PM_MPP_3
						= PM_NUM_MPP_PM6620
						= PM_NUM_MPP_PM6640
						= PM_NUM_MPP_PANORAMIX
						...
						...
						...
						PM_MPP_INVALID

						\endcode
		
		\param iChannel = Which analog mux or analog bus will the selected MPP be routed to. Values for this field are:

						\code
						0 - PM_MPP__AIN__CH_AMUX5
						1 - PM_MPP__AIN__CH_AMUX6
						2 - PM_MPP__AIN__CH_AMUX7
						3 - PM_MPP__AIN__CH_AMUX8
						4 - PM_MPP__AIN__CH_AMUX9
						5 - PM_MPP__AIN__CH_AMUX1
						6 - PM_MPP__AIN__CH_AMUX2
						7 - PM_MPP__AIN__CH_AMUX3

						\endcode
		
		\param piStatus = Return status
						\code
						0 = COMMAND_SUCESS
						>0 = Error code
						\endcode
	
		\return true if successful, false if fail.

		\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_MPP_CONFIG_ANALOG_IN(
						HANDLE hResourceContext,
						unsigned char iMpp, 
						unsigned char iChannel, 
						unsigned short * piStatus);
	/******************************************************************************/
	/**
		Factory Test Mode PMIC Commands 9.5  Set analog out command

		This command will configure the selected Multi Purpose pin (MPP) to be an analog output pin
		
		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iMPP =	Which MPP we want to configure. Values for this field are:

						\code
						0 - PM_MPP_1	
						1 - PM_MPP_2 
						2 - PM_MPP_3
						= PM_NUM_MPP_PM6620
						= PM_NUM_MPP_PM6640
						= PM_NUM_MPP_PANORAMIX
						...
						...
						...
						PM_MPP_INVALID

						\endcode
		
		\param iMpp_switch = This option allows the user to enable/disable the MPP output. Values for this field are:

						\code
						0 - PM_MPP__AOUT__SWITCH_OFF
						1 - PM_MPP__AOUT__SWITCH_ON
						2 - PM_MPP__AOUT__SWITCH_ON_IF_MPP_HIGH
						3 - PM_MPP__AOUT__SWITCH_ON_IF_MPP_LOW

						\endcode

		\param piStatus = Return status
						\code
						0 = COMMAND_SUCESS
						>0 = Error code
						\endcode
	
		\return true if successful, false if fail.

		\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_MPP_CONFIG_ANALOG_OUT(
						HANDLE hResourceContext,
						unsigned char iMpp, 
						unsigned char iMpp_switch, 
						unsigned short * piStatus);
	/******************************************************************************/
	/**
		Factory Test Mode PMIC Commands 9.6  Set current sink command

		This command will configure the selected Multi Purpose pin (MPP) to be a current sink. 
		
		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iMPP =	Which MPP we want to configure. Values for this field are:

						\code
						0 - PM_MPP_1	
						1 - PM_MPP_2 
						2 - PM_MPP_3
						= PM_NUM_MPP_PM6620
						= PM_NUM_MPP_PM6640
						= PM_NUM_MPP_PANORAMIX
						...
						...
						...
						PM_MPP_INVALID

						\endcode
		
		\param iLevel = How much current we want to allow the MPP to sink.  Values for this field are:

						\code
						0 - PM_MPP__I_SINK__LEVEL_5mA
						1 - PM_MPP__I_SINK__LEVEL_10mA
						2 - PM_MPP__I_SINK__LEVEL_15mA
						3 - PM_MPP__I_SINK__LEVEL_20mA
						4 - PM_MPP__I_SINK__LEVEL_25mA
						5 - PM_MPP__I_SINK__LEVEL_30mA
						6 - PM_MPP__I_SINK__LEVEL_35mA
						7 - PM_MPP__I_SINK__LEVEL_40mA

						\endcode
		
		\param iMpp_switch = This option allows the user to enable/disable the MPP output. Values for this field are:

						\code
						0 - PM_MPP__AOUT__SWITCH_OFF
						1 - PM_MPP__AOUT__SWITCH_ON
						2 - PM_MPP__AOUT__SWITCH_ON_IF_MPP_HIGH
						3 - PM_MPP__AOUT__SWITCH_ON_IF_MPP_LOW

						\endcode

		\param piStatus = Return status
						\code
						0 = COMMAND_SUCESS
						>0 = Error code
						\endcode
	
		\return true if successful, false if fail.

		\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_MPP_CONFIG_I_SINK(
						HANDLE hResourceContext,
						unsigned char iMpp, 
						unsigned char iLevel, 
						unsigned char iMpp_switch, 
						unsigned short * piStatus);

	/******************************************************************************/
	/**
		Factory Test Mode PMIC Commands 1.2  Get Voltage Regulator Enumration List

		This command gets supported voltage Regulator enumeration list 
		
		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param piNumVreg = Num of supported voltage regulator 

		\param piBufferSize =  Size of pBuffer.  The length of enumeration list string is returned with this paramter
							   A value of -1 is to indicate buffer overflow condition

		\param pBuffer = A buffer to store enumeration list string (null character terminated)

		\param piStatus = Return status
						\code
						0 = COMMAND_SUCESS
						>0 = Error code
						\endcode
	
		\return true if successful, false if fail.

		\warning If the user buffer size is less than enumeration list string length, the function will return false
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_VREG_GET_ENUM_LIST(
						HANDLE hResourceContext,
						unsigned short* piNumVreg, 
						short * piBufferSize, 
						unsigned char * pBuffer, 
						unsigned short *piStatus);

	/******************************************************************************/
	/**
		Factory Test Mode PMIC Commands 1.2  Get Voltage Regulator Enumration List

		This command calls FTM_PMIC_VREG_GET_ENUM_LIST to get supported voltage Regulator enumeration list 
		
		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param piNumVreg = Num of supported voltage regulator 

		\param aEnumIdList =  An array to store the list of enumeration id

		\param aEnumStrList = An array to store the list of enumeration string

		\param piStatus = Return status
						\code
						0 = COMMAND_SUCESS
						>0 = Error code
						\endcode
	
		\return true if successful, false if fail.

		\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_VREG_GET_ENUM_LIST_IN_ARRAY_FORM(
						HANDLE hResourceContext,
						unsigned short* piNumVreg, 
						unsigned short aEnumIdList[20], 
						unsigned char aEnumStrList[20][8], 
						unsigned short *piStatus);


	/******************************************************************************/
	/**
	Factory Test Mode PMIC Commands X.X Get PMIC Model
	
	This command calls FTM_PMIC_GET_MODEL to get PMIC Model ID
		
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piModel	= PMIC model number:
						\code
						0 = PMIC_IS_PM6610,
						1 = PMIC_IS_PM6620,
						2 = PMIC_IS_PM6640,
						3 = PMIC_IS_PM6650,
						4 = PMIC_IS_PM7500,
						5 = PMIC_IS_PANORAMIX,
						6 = PMIC_IS_INVALID
						\endcode

	\param piStatus = Return status
						\code
						0 = COMMAND_SUCESS
						>0 = Error code
						\endcode
	
	\return true if successful, false if fail.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_PMIC_GET_MODEL(
								HANDLE hResourceContext, 
								unsigned short* piModel, 
								unsigned short * piStatus);
/******************************************************************************
					cdma2000 FTM
*******************************************************************************/
	

	/******************************************************************************/
	/**
	CDMA2000 Set the NV_FTM_MODE_I nv item value to either 0 or 1 


	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         
	\param iMode = The mode can be either DMSS(0), or FTM(1). The default value is 0. 
				   The mode change will not take effect until the next phone reset.

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA2000_SET_MODE(	
								HANDLE hResourceContext,
								unsigned char iMode);

							
/******************************************************************************
					cdma2000 FTM non-signaling
*******************************************************************************/
	/******************************************************************************/
	/**
	Enables logging for the CDMA2000 non-signaling commands.  If this is not done, then 
	the CDMA2000 NS commands will act only as simple request/response types.  
	
	After this function is called, the cdma2000 non-signaling commands will wait 
	for assocated events to occur, before returning.

	The log used to determine searcher state is: 0x119C, FTM_CDMA2000_SRC_TNG_STATUS

	The logs used to determine FER and SER are:
	  0x10D9 - FTM_CDMA2000_RDA2_FRAME_INFO, for MSM6100 and later + SC1X and later
	  0x10C9 - FTM_CDMA2000_RDA_FRAME_INFO, for MSM6000, MSM6025, MSM6050


	The logs used for AGC are FTM logs:
		LOG_FTM2_LOG_1X_AGC = 0x4
		LOG_FTM2_LOG_1X_AGC_C1	= 0x6

  	After this function is called, then the library will begin collecting 
	statistics (SER, AGC)


	The function QLIB_FTM_CDMA2000_NS_Stop() should be called when the the CDMA2000 NS 
	testing is completed.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         

	\param eDR_Type = Dymamic range type, defined by FTM_CDMA2000_NS_DR_Types_Enum in QLib_Defines.h
     \code
      0 = FTM_CDMA2000_NS_DR_85_3  = 85.3dB  dynamic range, min_rssi = -106.0dBm
      1 = FTM_CDMA2000_NS_DR_102_4 = 102.4dB dynamic range, min_rssi = -115.0dBm
     \endcode
	\param dTurnAroundConstant = cdma turn around constant, -73dBm for BC0, -76 for BC1.

	\param bEnableDiversityAGCLog = Enabling AGC Logging for diversity antenna
									This should be set to 0 for non recieve diversity target
									0 = Diversity AGC logging off
									1 = Diversity AGC logging on

	\return true if successful, false if fail.

	\warning Will clear status and enable all CDMA2000 NS statistics, via 
	         FTM_CDMA2000_NS_SetStatisticsState()

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA2000_NS_Start( 
							HANDLE hResourceContext, 
							unsigned char eDR_Type, 
							double dTurnAroundConstant,
							unsigned char bEnableDiversityAGCLog);

	/******************************************************************************/
	/**
	Disables logging for the CDMA2000 non-signaling commands.  This should be done
	after FTM_CDMA2000_NS_StartLogging() is called, and after all CDMA2000 NS testing is
	completed.
		
	\return true if successful, false if fail.

	\warning The log mask for the 1X equipment ID will be cleared

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA2000_NS_Stop( HANDLE hResourceContext );


	/******************************************************************************/
	/**
	Turn on or off logging for a certain type of 1xEV-DO non-signaling log
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         
	\param eLogType = log type to control
		\code
			FTM_CDMA2000_NS_Log_FingerInfo,				//!<' 1xEV-DO Finger, Ver 2 (LOG_CODE 0x108A), C/I for both C0 and C1
			FTM_CDMA2000_NS_Log_AGC_C0,					//!<' FTM2 log, sub log 5, for Primary AGC
			FTM_CDMA2000_NS_Log_AGC_C1,					//!<' FTM2 log, sub log 7, for Diversity AGC
			FTM_CDMA2000_NS_Log_PER,					//!<' 1xEV-DO Air Link Summary (LOG_CODE 0x1068), used for PER
		\endcode
	\param bEnableStatistics = true to enable statistics of the specified log type
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA2000_NS_SetStatisticsState( 
							HANDLE hResourceContext, 
							unsigned char eLogType, 
							unsigned char bEnableStatistics);

	/******************************************************************************/
	/**
	CDMA2000 Non-signaling, 
	
		Reset the entire internal status structure, including all averages

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param bIgnore1stRDA = true to ignore the first RDA log, which may have bad data if the
	                       signal generator settings were just changed.
	\return true if successful, false if failure

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA2000_NS_ClearStatus( 
							HANDLE hResourceContext,
							unsigned char bIgnore1stRDA);

	/******************************************************************************/
	/**
	CDMA2000 Non-signaling, 
	
		Reset the SER/FER internal status structure.  This will not affect
		the other status items, such as AGC

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\return true if successful, false if failure

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA2000_NS_ClearFER_Status( 
							HANDLE hResourceContext);

	/******************************************************************************/
	/**
	CDMA2000 Non-signaling, 
			Returns a copy of the internal state. 

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         
	\param pCDMA2000_NS_State = reference to a FTM_CDMA2000_NS_Status_Struct structure that will be 
	                        filled in with the state information.  
							Structure type is FTM_CDMA2000_NS_Status_Struct, defined in QLib_Defines.h

							

	\return true if all operations succeeded and the correct event messages were received

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA2000_NS_GetStatus( 
							HANDLE hResourceContext, 
							void* pCDMA2000_NS_State );


	/******************************************************************************/
	/**
	CDMA2000 Non-signaling, 
			Sets the radio configuratoin and FCH rate set to be used for calculating SER.
			This function can also be used for setting up SER measurements in signaling
			mode. 

			In signaling mode, the function QLIB_DIAG_CDMA2000_SetSER_State() should be
			used to control the capture of frame data and calculation of SER results.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         
	\param eCurrentCDMA2000_RC = Radio configuration to be used. Of type FTM_CDMA2000_NS_FWD_RC_Enum
	                             defined in QLib_Defines.h

	\param eCurrentCDMA2000_DataRate = FCH Data rate being used.  Of type  FTM_CDMA2000_NS_REV_FCH_Rates_Enum
										defined in QLib_Defines.h
	

	\return true if set successfully

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA2000_NS_SetSER_RCandRS( 
								HANDLE hResourceContext, 
								unsigned long eCurrentCDMA2000_RC,
								unsigned long eCurrentCDMA2000_DataRate);


	/******************************************************************************/
	/**
	CDMA2000 Non-signaling 
		Returns a subset of the cdma2000 NS status--the current Fundamental channel
		SER and FER with the option to set a minimum number.  
			
		Will wait until a certain number of symbols have been counted or a time out expires.


	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         

	\param iMinimumNumSymbols = input, minimum number of symbols to capture.  Set to 0 to return
	                            the current statistics with no wait
	\param piTotalSymbols = output, total number of symbols that have been recorded
	\param piTotalErrorSymbols = output, total symbol errors recorded
	\param piTotalFrames = output, total frames recorded
	\param piTotalFrameErrors = output, total number of frame errors
	\param pdSER_Calculated = output, current SER, since the last time that statistics were cleared.
	\param pdFER_Calculated = output, current FER, since the last time that statistics were cleared.
	\param iTimeOut_ms = input, number of milliseconds to wait for the number of symbols to exceed 
	                     iMinimumNumSymbols.  This time-out applies only when iMinimumNumSymbols > 0.

	\return true if all operations succeeded 

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA2000_NS_Get_FCH_SER_FER( 
							HANDLE hResourceContext, 
							unsigned long iMinimumNumSymbols,
							unsigned long iMinimumNumFrames,
							unsigned long* piTotalSymbols, 
							unsigned long* piTotalErrorSymbols, 
							unsigned long* piTotalFrames, 
							unsigned long* piTotalFrameErrors, 
							double* pdSER_Calculated,
							double* pdFER_Calculated,
							unsigned long iTimeOut_ms
							);


		/******************************************************************************/
	/**
	CDMA2000  
		Returns a subset of the cdma2000 status--the Paging Channel Message Error Rate with the
		option to set a minimum number.  
			
		Will wait until a certain number of symbols have been counted or a time out expires.


	\param iMinimumNumSymbols = input, minimum number of symbols to capture.  
	\param piTotalSymbols = output, total number of symbols that have been recorded for paging channel
	\param piTotalErrorSymbols = output, total symbol errors recorded for paging channel
	\param pdPaging_MER_Calculated = output, iTotalErrorSymbols / iTotalSymbols

	\param iTimeOut_ms = input, number of milliseconds to wait for the number of symbols to exceed 
	                     iMinimumNumSymbols.  This time-out applies only when iMinimumNumSymbols > 0.

	\return true if all operations succeeded 

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_CDMA2000_Get_Paging_MER(	
								HANDLE hResourceContext, 
								unsigned long iMinimumNumSymbols,
								unsigned long* piTotalSymbols,
								unsigned long* piTotalErrorSymbols, 
								double* pdPaging_MER_Calculated,
								unsigned long iTimeOut_ms );


	/******************************************************************************/
	/**
	CDMA2000 Non-signaling command 3.2.1 Acquire pilot

	This function acquires pilot

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         
	\param iBand_Class = Indicates the band class. Possible values are in FTM_CDMA2000_BandClasstype

		\code
          FTM_CDMA2000_BAND_BC0	= 0,
          FTM_CDMA2000_BAND_BC1	= 1,
          FTM_CDMA2000_BAND_BC2	= 2,
          FTM_CDMA2000_BAND_BC3	= 3,
          FTM_CDMA2000_BAND_BC4	= 4,
          FTM_CDMA2000_BAND_BC5	= 5,
          FTM_CDMA2000_BAND_BC6	= 6,
          FTM_CDMA2000_BAND_BC7	= 7,
          FTM_CDMA2000_BAND_BC8	= 8,
          FTM_CDMA2000_BAND_BC9	= 9,
          FTM_CDMA2000_BAND_BC10	= 10,
          FTM_CDMA2000_BAND_BC11	= 11,
          FTM_CDMA2000_BAND_BC12	= 12,
          FTM_CDMA2000_BAND_BC13	= 13,
          FTM_CDMA2000_BAND_BC14	= 14,
          FTM_CDMA2000_BAND_BC15	= 15,
          FTM_CDMA2000_BAND_BC16	= 16,
          FTM_CDMA2000_BAND_BC17	= 17
		\endcode
	\param iCDMA_Freq  = Indicates the RF channel number.
	\param iACQ_MODE   = Indicates the acquisition mode. Possible values are
		\code
            FTM_CDMA2000_NS_FTM_ACQ_MODE_FULL(0),
            FTM_CDMA2000_NS_FTM_ACQ_MODE_MINI(1),
            FTM_CDMA2000_NS_FTM_ACQ_MODE_MICRO(2).
		 \endcode

	\return true if successful, false if fail.

	\warning If this call fails, then QLIB_FTM_CDMA2000_DEMOD_SYNC() should not be called.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA2000_PILOT_ACQ(	
								HANDLE hResourceContext, 
								unsigned long iBand_Class, 
								unsigned short iCDMA_Freq, 
								unsigned long iAcq_Mode);

	/******************************************************************************/
	/**
	CDMA2000 Non-signaling command 3.2.2 Acquire sync channel

			This function acquires sync channel

	\return true if successful, false if fail.

	\warning QLIB_FTM_CDMA2000_PILOT_ACQ() must be called first, with a passing result.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA2000_DEMOD_SYNC(HANDLE hResourceContext);

	/******************************************************************************/
	/**
	CDMA2000 Non-signaling command 3.2.3 Assign forward fundamental channel

	This function assigns forward fundamental channel

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         
	\param iRadio_cfg	= Indicates the Radio Configuration of the F-FCH.
						Possible values are:
		\code
          FTM_CDMA2000_NS_RC_1(1),
          FTM_CDMA2000_NS_RC_2(2),
          FTM_CDMA2000_NS_RC_3(3),
          FTM_CDMA2000_NS_RC_4(4), 
          FTM_CDMA2000_NS_RC_5(5).
		\endcode
	\param iWalsh		= Indicates the Walsh code to be used for F-FCH.
	\param iQOF_Mask_id = Indicates the QOF index. Possible values are 0, 1, 2,and 3.
							For FTM_FWD_RC1 and FTM_FWD_RC2, this field should be set to 0.
	\param iBypass_LC	= If set to TRUE, the long code mask is set to 0.
	\param iFrame_Offset = Indicates the frame offset for F-FCH.
	\param iSubchan_Gain  = Indicates the Power Control Subchannel Gain.
	\param iFFPC_enable = If set to TRUE, Fast Forward Power Control is enabled.
	\param iFPC_Mode	= Indicates the Forward Power Control Mode.
						  FPC_MODE is not currently used at this time.
	\param iInit_Setpt  = Indicates the initial setpoint for F-FCH.
	\param iOLC_Params_Target_FER = Indicates the target frame error rate for F-FCH.
	\param iOLC_Params_Min_Setpt = Indicates the minimum setpoint for F-FCH.
	\param iOLC_Params_Max_Setpt = Indicates the maximum setpoint for F-FCH.
	
	\return true if successful, false if fail.

	\warning QLIB_FTM_CDMA2000_DEMOD_SYNC() must be called first, with a passing result.

	*******************************************************************************/	
	QLIB_API unsigned char QLIB_FTM_CDMA2000_DEMOD_FCH(	
								HANDLE hResourceContext, 
								unsigned long iRadio_Cfg,  
								unsigned char iWalsh, 
								unsigned char iQOF_Mask_ID, 
								unsigned char iBypass_LC, 
								unsigned char iFrame_Offset, 
								unsigned char iSubChan_Gain, 
								unsigned char iFFPC_Enable, 
								unsigned long iFPC_Mode, 
								unsigned char iInit_Setpt, 
								unsigned char iOLC_Params_Target_FER, 
								unsigned char iOLC_Params_Min_Setpt, 
								unsigned char iOLC_Params_Max_Setpt);
							
	/******************************************************************************/
	/**
	CDMA2000 Non-signaling command 3.2.4 Assign forward supplemental channel

	This function assigns forward supplemental channel
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         
	\param iRadio_cfg	= Indicates the Radio Configuration of the F-SCH.
							Possible values are:
		\code
          FTM_CDMA2000_NS_RC_3(3),
          FTM_CDMA2000_NS_RC_4(4), 
          FTM_CDMA2000_NS_RC_5(5).
		\endcode
	\param iWALSH		= Indicates the Walsh code to be used for F-SCH.
	\param iQOF_Mask_Id = Indicates the QOF index. Possible values are 0, 1, 2, and 3.
	\param iSch_Rate	= Indicates the data rate for F-SCH
	
	 Possible values for the rate are 0 to 4 which correspond to
	   \code
        FTM_RATE_1X (9.6 kbps)
        FTM_RATE_2X (19.2 kbps)
        FTM_RATE_4X (38.4 kbps)
        FTM_RATE_8X(76.8 kbps)
        FTM_RATE_16X (153.6 kbps)
	    \endcode

							
      For turbo-encoded SCH, the possible values are:
	    \code
        FTM_RATE_1X (9.6 kbps)
        FTM_RATE_2X (19.2 kbps)
        FTM_RATE_4X (38.4 kbps)
        FTM_RATE_8X(76.8 kbps)
        FTM_RATE_16X (153.6 kbps)
	    \endcode
	\param iCoding_type = Indicates the coding type for F-SCH. Possible values are 
		\code
         0 - FTM_SCH_CODING_CONVOLUTIONAL
         1 - FTM_SCH_CODING_TURBO
		 \endcode
	\param iFFPC_Enable = Indicates whether Forward Power Control is to be enabled.
	\param iFPC_Mode	= Indicates the Forward Power Control Mode.
						  iFPC_MODE is not used at this time.
	\param iInit_Setpt	= Indicates the initial setpoint for F-SCH.
	\param iOLC_Params_Target_FER = Indicates the target frame-error rate for F-SCH.
	\param iOLC_Params_Min_Setpt = Indicates the minimum setpoint for F-SCH.
	\param iOLC_PARAMS_Max_Setpt = Indicates the maximum setpoint for F-SCH.
   
	\return true if successful, false if fail.

	\warning QLIB_FTM_CDMA2000_DEMOD_SYNC() must be called first, with a passing result.

	*******************************************************************************/	
	
	QLIB_API unsigned char QLIB_FTM_CDMA2000_DEMOD_FSCH(
								HANDLE hResourceContext, 
								unsigned long iRadio_Cfg, 
								unsigned char iWalsh, 
								unsigned char iQPF_Mask_ID, 
								unsigned long iSch_Rate, 
								unsigned long iCoding_Type, 
								unsigned char iFFPC_Enable,
								unsigned long iFPC_Mode, 
								unsigned char iInit_Setpt, 
								unsigned char iOLC_Params_Target_FER, 
								unsigned char iOLC_Params_Min_Setpt, 
								unsigned char iOLC_Params_Max_Setpt);


	/******************************************************************************/
	/**
	CDMA2000 Non-signaling command 3.2.5 Assign reverse fundamental channel

	This function assigns reverse fundamental channel
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         
	\param iRadio_cfg = Indicates the Radio Configuration for R-FCH.
						Possible values are:
		\code
         1 = FTM_REV_RC1
		 2 = FTM_REV_RC2
		 3 = FTM_REV_RC3
		 4 = FTM_REV_RC4
		\endcode
	\param iFrame_offset = Indicates the frame offset for R-FCH.
	\param iFCH_Rate	= Indicates the data rate for R-FCH. Possible values are 
		\code
         0 = FTM_CDMA2000_NS_REV_FCH_FULL_RATE
         1 = FTM_CDMA2000_NS_REV_FCH_HALF_RATE
         2 = FTM_CDMA2000_NS_REV_FCH_QTR_RATE
         3 = FTM_CDMA2000_NS_REV_FCH_8TH_RATE
		\endcode
	\param iEnable_RPC	= If set to TRUE, Reverse Power Control is enabled.
	\param iNum_Preambles = Indicates the number of preambles that will be sent on R-FCH. One preamble = 20 ms.
	\param iTX_Pattern	=	Specifies the Tx pattern that is to be transmitted on R-FCH. 
							This pattern will be repeated every 4 bytes.

	\return true if successful, false if fail.

	\warning QLIB_FTM_CDMA2000_DEMOD_SYNC() must be called first, with a passing result.

	*******************************************************************************/	

	
	QLIB_API unsigned char QLIB_FTM_CDMA2000_MOD_FCH(	
								HANDLE hResourceContext, 
								unsigned long iRadio_Cfg, 
								unsigned char iFrame_Offset, 
								unsigned long iFCH_Rate, 
								unsigned char iEnable_RPC, 
								unsigned short iNum_Preambles,
								unsigned long iTX_Pattern);	
	
	/******************************************************************************/
	/**
	CDMA2000 Non-signaling command 3.2.6 Assign reverse supplemental channel

	This function assigns reverse supplemental channel
	
	\param iRadio_cfg = Indicates the Radio Configuration for R-SCH.
						Possible value is FTM_REV_RC3(3).
	\param iSch_Rate  = Indicates the R-SCH data rate. Possible values are from 0 to 7 which correspond to
	                    The enumeration, FTM_SCH_DataRates_Enum
       \code
       FTM_REV_SCH_1_5_RATE (0 = 1.5 kbps),
       FTM_REV_SCH_2_7_RATE (1 = 2.7 kbps),
       FTM_REV_SCH_4_8_RATE (2 = 4.8 kbps),
       FTM_REV_SCH_9_6_RATE (3 = 9.6 kbps),
       FTM_REV_SCH_19_2_RATE(4 = 19.2 kbps), 
       FTM_REV_SCH_38_4_RATE (5 = 38.4 kbps), 
       FTM_REV_SCH_76_8_RATE (6 = 76.8kbps), and 
       FTM_REV_SCH_153_6_RATE (7 = 153.6kbps) respectively.
	   \endcode

		Note that for turbo-encoded frames, the minimum supported rate is FTM_REV_SCH_19_2_RATE.


	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         
	\param iCoding_type = Indicates the coding type for R-SCH. Possible values are: 
		\code
         0 - FTM_SCH_CODING_CONVOLUTIONAL
         1 - FTM_SCH_CODING_TURBO
		 \endcode

	\param iTX_Pattern = Specifies the Tx pattern for R-SC

	\return true if successful, false if fail.

	\warning QLIB_FTM_CDMA2000_DEMOD_SYNC() must be called first, with a passing result.

	*******************************************************************************/	
	
	QLIB_API unsigned char QLIB_FTM_CDMA2000_MOD_SCH(	
								HANDLE hResourceContext, 
								unsigned long iRadio_Cfg, 
								unsigned long iSCH_Rate, 
								unsigned long iCoding_Type, 
								unsigned long iTX_Pattern);	
	
	/******************************************************************************/
	/**
	CDMA2000 Non-signaling command 3.2.7 Enable loopback on the fundamental channel

	This command enables the loopback mode where all data received on the forward link is copied
	to the reverse link. The forward and reverse fundamental channels must already be set up before
	loopback mode is requested. The rate of the reverse link matches the forward link. In the case
	where an erasure is received on the forward link, a full rate frame of all zeros is sent on the
	reverse link.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         
	\return true if successful, false if fail.

	\warning FTM_CDMA2000_DEMOD_FCH and FTM_CDMA2000_MOD_FCH must be called first

	*******************************************************************************/

	QLIB_API unsigned char QLIB_FTM_CDMA2000_FCH_LOOPBACK( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	CDMA2000 Non-signaling command 3.2.8 Enable loopback on the supplemental channel

	This command enables the SCH loopback mode. The forward and reverse supplemental channels
	should already be set up to the same rate.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         
	\return true if successful, false if fail.

	\warning QLIB_FTM_CDMA2000_DEMOD_SYNC() and QLIB_FTM_CDMA2000_MOD_SCH must be 
	         called first, with a passing result.
	         

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA2000_SCH_LOOPBACK( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	CDMA2000 Non-signaling command 3.2.9 Deassign all traffic channels

	This command releases all traffic channels and leaves the phone ready to acquire a pilot.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA2000_CMD_RELEASE( HANDLE hResourceContext );


	/******************************************************************************/
	/**
	CDMA2000 Non-signaling command 

	This command calls pilot acquistion, synchronization and forward fundamental assignment in order.
	Specifically, these functions are called:
		QLIB_FTM_CDMA2000_PILOT_ACQ()
		QLIB_FTM_CDMA2000_DEMOD_SYNC()
		QLIB_FTM_CDMA2000_DEMOD_FCH()

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         
	\param iBand_Class, Band Class number (1=BC1)
	\param iCDMA_Freq,  CDMA channel number
	\param iRadio_Cfg,  Indicates the Radio Configuration of the F-FCH.
						Possible values are:
		\code
         1 = FTM_FWD_RC_1
		 2 = FTM_FWD_RC_2
		 3 = FTM_FWD_RC_3
         4 = FTM_FWD_RC_4
		 5 = FTM_FWD_RC_5
		\endcode
	\param iWalsh		Wash code to be used for F-FCH
	\param iFFPC_Enable	If set to TRUE, Fast Forward Power Control is enabled.

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA2000_GOTO_FWD_TRAFFIC_STATE( 
									HANDLE hResourceContext,  
									unsigned long iBand_Class,
									unsigned short iCDMA_Freq,
									unsigned long iRadio_Cfg,  
									unsigned char iWalsh, 
									unsigned char iFFPC_Enable );

	/******************************************************************************/
	/**

	CDMA2000 Non-signaling command 3.2.10 Assign forward dedicated control channel

	This command assigns forward dedicated control channel

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         
	\param iRadio_Cfg = Indicates the Radio Configuration of the F-FCH.
											Possible values are FTM_FWD_RC_3(3), FTM_FWD_RC_4(4), and FTM_FWD_RC_5(5).
	\param iWalsh     = Indicates the Walsh code to be used for F-FCH.
	\param iQof_Mask_Id = Indicates the QOF index. Possible values are 0, 1, 2, and 3. 
												For FTM_FWD_RC1 and FTM_FWD_RC2, this field should be set to 0.
	\param iBypass_Lc = If set to TRUE, the long code mask is set to 0.
	\param iSo_Pri = Indicates the primary service option.
	\param iSo_Sec = Indicates the secondary service option
	\param iFrame_Offset = Indicates the frame offset for F-DCCH.
	\param iIlpc_Enable = If set to TRUE, the inter loop power control is enable.
	\param iOlpc_Enable = If set to TRUE, the open loop power control is enable.
	\param iInit_Setpt = Indicates the Initial setpoint for F-DDCH.
	\param iFpc_Subchan_Gain = Forward Power control sub-chan gain
	\param iOlc_Params_Target_Fer = Indicates the target frame-error rate for F-DCCH.
	\param iOlc_Params_Min_Setpt = Indicates the minimum setpoint for F-DCCH.
	\param iOlc_Params_Max_Setpt = Indicates the maximum setpoint for F-DCCH.

	\return true if successful, false if fail.

	\warning QLIB_FTM_CDMA2000_DEMOD_SYNC() must be called first, with a passing result.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA2000_DEMOD_DCCH	(	HANDLE hResourceContext,
	  unsigned long iRadio_Cfg, 
	  unsigned char iWalsh, 
	  unsigned char iQof_Mask_Id, 
	  unsigned char iBypass_Lc, 
	  unsigned short iSo_Pri, 
	  unsigned short iSo_Sec, 
	  unsigned char iIlpc_Enable, 
	  unsigned char iOlpc_Enable, 
	  unsigned char iFpc_Subchan_Gain,
	  unsigned char iOlc_Params_Target_Fer, 
	  unsigned char iOlc_Params_Min_Setpt, 
	  unsigned char iOlc_Params_Max_Setpt);

	/******************************************************************************/
	/**
	CDMA2000 Non-signaling command 3.2.11 Assign reverse dedicated control channel

	This command assigns reverse dedicated control channel

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         
	\param iRadio_Cfg = Indicates the Radio Configuration for R-DCCH.
											Possible values are FTM_REV_RC3(3) and FTM_REV_RC4(4).
	\param iFrame_Offset - Indicates the frame offset for R-DCCH.
	\param iSch_On = If set to TRUE, the SCH is enabled.
	\param iSch_Rc = Indicates the Radio Configuration of the F-SCH.
									Possible values are FTM_FWD_RC_3(3), FTM_FWD_RC_4(4).
	\param iSch_Turbo = Indicates the coding type for SCH. Possible values
										are FTM_CODING_CONVOLUTIONAL(0) and FTM_CODING_TURBO(1).
	\param iSch_Ltu_Size = Indicates the logical transition unit size
	\param iSch_Rate = Indicates the data rate for SCH. 
	                    Possible values for	
	   \code
        FTM_RATE_1X (9.6 kbps)
        FTM_RATE_2X (19.2 kbps)
        FTM_RATE_4X (38.4 kbps)
        FTM_RATE_8X(76.8 kbps)
        FTM_RATE_16X (153.6 kbps)
		FTM_RATE_32X (5 = 307.2 kbps)
	    \endcode

			For turbo-encoded SCH, the possible values are FTM_RATE_2X, FTM_RATE_4X, FTM_RATE_8X,
			FTM_RATE_16X and FTM_RATE_32X.
	\param iNum_Preambles = Indicates the number of preambles that will be sent on R-DCCH. One preamble = 20 ms.
	\param iTx_Pattern = Specifies the Tx pattern that is to be transmitted on R-DCCH. This pattern will be 
											repeated every 4 bytes.
	
	\return true if successful, false if fail.

	\warning QLIB_FTM_CDMA2000_DEMOD_SYNC() must be called first, with a passing result.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA2000_MOD_DCCH(	HANDLE hResourceContext,
														unsigned long iRadio_Cfg, 
														unsigned char iFrame_Offset, 
														unsigned char iSch_On, 
														unsigned long iSch_Rc, 
														unsigned char iSch_Turbo,
														unsigned short iSch_Ltu_Size, 
														unsigned long iSch_Rate, 
														unsigned short iNum_Preambles, 
														unsigned long iTx_Pattern);
	/******************************************************************************/
	/**
	CDMA2000 Non-signaling command 3.2.12 Enable loopback on the dedicated control channel

	This command enables loopback on the dedicated control channel

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         
	\return true if successful, false if fail.

	\warning QLIB_FTM_CDMA2000_DEMOD_SYNC(), QLIB_FTM_CDMA2000_MOD_DCCH(),  and
	         QLIB_FTM_CDMA2000_DEMOD_DCCH() must be called first, each with a passing result.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA2000_DCCH_LOOPBACK(HANDLE hResourceContext);

	/******************************************************************************/
	/**
	CDMA2000 Non-signaling command x.x.x Handover

	This function will command the mobile to a new band class and channel number.

	The FTM code for this function was added in December 2006, long afer the 
	original cdma2000 non-signaling commands were in FTM.  If the command fails,
	it is quite possible that embedded software does not support it.

	In order to verify the RF channel has been changed, this function uses 
	QLIB_DIAG_IS2000_STATUS_F() to check the mobile status for the band class
	and channel number.  If the mobile does not report the new numbers, then a 
	this function will also fail.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         
	\param iBand_Class = Indicates the band class. Possible values are in FTM_CDMA2000_BandClasstype
						See QLIB_FTM_CDMA2000_PILOT_ACQ() for the full list.
	\param iCDMA_Freq  = Indicates the RF channel number.

	\return true if successful, false if fail.

	\warning This must be called only after the QLIB_FTM_CDMA2000_DEMOD_FCH() is called successfully

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CDMA2000_FTM_FWD_HHO_SC(	
								HANDLE hResourceContext, 
								unsigned long iBand_Class, 
								unsigned short iCDMA_Freq
								);


/******************************************************************************
						EVDO Non-signaling FTM 
*******************************************************************************/

	/******************************************************************************/
	/**
	Enables logging for the EVDO non-signaling commands.  If this is not done, then 
	the EVDO NS commands will act only as simple request/response types.  
	
	After this function is called, the commands will for assocated events
	to occur, before they returning to the library client.

	In addition, after this function is called, then the library will begin collecting 
	statistics (PER, C/I, AGC)

	The log used to determine searcher state is: 0x108A, FTM_EVDO_NS_LOG_CODE_FINGER_INFO

	The logs used to determine PER is: 0x1068, FTM_EVDO_NS_LOG_CODE_AIR_LINK_SUMMARY

	The logs used for AGC are FTM logs:
	\code
      LOG_FTM2_LOG_HDR_AGC		= 0x5
      LOG_FTM2_LOG_HDR_AGC_C1 = 0x7
	\endcode

	When this function is called, only log 0x108A, FTM_EVDO_NS_LOG_CODE_FINGER_INFO will
	be enabled.  At the same time, log 0x1068, FTM_EVDO_NS_LOG_CODE_AIR_LINK_SUMMARY and the
	FTM AGC logs will be >>DISABLED<<
	
	The user must call QLIB_FTM_EVDO_NS_EnableMeasurementLogs() in order to activate 
	0x1068, FTM_EVDO_NS_LOG_CODE_AIR_LINK_SUMMARY and the FTM AGC logs and measure
	AGC and PER.  This is because the MSM6800 design is such that there will be 
	problems if the FTM AGC log is enabled when the pilot acquisition is perfomed. This
	is by-design, so the PC side must be managed around this.

	The function FTM_EVDO_NS_Stop should be called when the the EVDO NS testing
	is completed.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         

	\param iDR_Type = Dymamic range type, defined by FTM_EVDO_NS_DR_Types_Enum in QLib_Defines.h
		\code
          0 = FTM_EVDO_NS_DR_85_3  = 85.3dB  dynamic range, min_rssi = -106.0dBm
          1 = FTM_EVDO_NS_DR_102_4 = 102.4dB dynamic range, min_rssi = -115.0dBm
		\endcode
	\param dTurnAroundConstant = cdma turn around constant, -73dBm for BC0, -76 for BC1.
	\return true if successful, false if fail.

	\warning Will clear status and enable all EVDO NS statistics, via 
	         FTM_EVDO_NS_SetStatisticsState().  Will also clear the EVDO NS status
			 using QLIB_FTM_EVDO_NS_ClearStatus()

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EVDO_NS_Start( HANDLE hResourceContext,  unsigned char iDR_Type, double dTurnAroundConstant  );


	/******************************************************************************/
	/**
	Enables measurement logs for the EVDO non-signaling commands.  If this is not done, 
	then it will not be possible to report PER and AGC from the QMSL functions.

 	This function should be called after a successful transfer to traffice state when
	QLIB_FTM_EVDO_DEMOD_CC_MAC_FTC() is called.
 
	After this function is called, then the library will begin collecting 
	statistics (PER, C/I, AGC)
	
	The logs used to determine PER is: 0x1068, FTM_EVDO_NS_LOG_CODE_AIR_LINK_SUMMARY

	The logs used for AGC are FTM logs:
	\code
       LOG_FTM2_LOG_HDR_AGC    = 0x5
       LOG_FTM2_LOG_HDR_AGC_C1 = 0x7
	\endcode


	The function FTM_EVDO_NS_Stop should be called when the the EVDO NS testing
	is completed.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         

	\param iLogMask = Dymamic range type, defined by FTM_EVDO_NS_DR_Types_Enum in QLib_Defines.h
		The logs can be enabled with a bit mask as defined by FTM_EVDO_NS_MeasurementLogs_Enum.  
		
		The mask values are as follows:
		\code
		  FTM_EVDO_NS_LOG_AIR_LINK_SUMMARY = 0x01,  //!<'  Air link summary
		  FTM_EVDO_NS_LOG_AGC_C0           = 0x02,  //!<'  FTM AGC Log for primary Rx (C0)
		  FTM_EVDO_NS_LOG_AGC_C1           = 0x04,  //!<'  FTM AGC Log for secondary Rx (C1)

		  FTM_EVDO_NS_LOG_Default          = ALL
		\endcode

		The default mask is FTM_EVDO_NS_LOG_Default, which includes all logs
	\return true if successful, false if fail.

	\warning Will clear status and enable all EVDO NS statistics, via 
	         FTM_EVDO_NS_SetStatisticsState().  Will also clear the EVDO NS status
			 using QLIB_FTM_EVDO_NS_ClearStatus()

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EVDO_NS_EnableMeasurementLogs( HANDLE hResourceContext,  unsigned char iLogMask );

	/******************************************************************************/
	/**
	Disables logging for the EVDO non-signaling commands.  This should be done
	after FTM_EVDO_NS_StartLogging() is called, and after all EVDO NS testing is
	completed.
		
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         

	\return true if successful, false if fail.

	\warning The log mask for the 1X equipment ID will be cleared

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EVDO_NS_Stop( HANDLE hResourceContext );


	/******************************************************************************/
	/**
	Turn on or off logging for a certain type of 1xEV-DO non-signaling log
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         
	\param eLogType = log type to control
		\code
			FTM_EVDO_NS_Log_FingerInfo,				//!<' 1xEV-DO Finger, Ver 2 (LOG_CODE 0x108A), C/I for both C0 and C1
			FTM_EVDO_NS_Log_AGC_C0,					//!<' FTM2 log, sub log 5, for Primary AGC
			FTM_EVDO_NS_Log_AGC_C1,					//!<' FTM2 log, sub log 7, for Diversity AGC
			FTM_EVDO_NS_Log_PER,					//!<' 1xEV-DO Air Link Summary (LOG_CODE 0x1068), used for PER
		\endcode
	\param bEnableStatistics = true to enable statistics of the specified log type
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EVDO_NS_SetStatisticsState( HANDLE hResourceContext, unsigned char eLogType, unsigned char bEnableStatistics);


	/******************************************************************************/
	/**
	1XEV-DO Non-signaling, 
	
		Reset the internal status structure, including all averages

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if failure

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EVDO_NS_ClearStatus( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	1XEV-DO Non-signaling, 
	
		Reset the internal PER status structure, will not affect other statistics,
		such as AGC averages

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if failure

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EVDO_NS_ClearPER_Status( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	1XEV-DO Non-signaling, 
			Returns a copy of the internal state. 

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         

	\param pEVDO_NS_State = reference to a FTM_EVDO_NS_Status_Struct structure that will be 
	                        filled in with the state information.  
							Structure type is FTM_EVDO_NS_Status_Struct, defined in QLib_Defines.h

							

	\return true if all operations succeeded and the correct event messages were received

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EVDO_NS_GetStatus( HANDLE hResourceContext, void* pEVDO_NS_State );

	/******************************************************************************/
	/**
	1XEV-DO Non-signaling, 
			Returns a subset of the EVDO NS status--the current average PER and
			the number of 16-

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         

	\param iMinimumNumPackets = input, minimum number of packets to capture.  Set to 0 to return
	                            the current statistics
	\param piTotalPackets = output, total number of packets that have been recorded
	\param pdPER_Calculated = output, current PER, since the last time that statistics were cleared.
	\param iTimeOut_ms = input, number of milliseconds to wait for the number of packets to exceed 
	                     iMinimumNumPackets.  This time-out applies only when iMinimumNumPackets > 0.

	\return true if all operations succeeded 

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EVDO_NS_GetPER( 
							HANDLE hResourceContext, 
							unsigned long iMinimumNumPackets,
							unsigned long* piTotalReceivedPackets, 
							double* pdPER_Calculated,
							unsigned long iTimeOut_ms
							);

	/******************************************************************************/
	/**
	1XEV-DO Non-signaling, command 3.2.1 Acquire 1xEV-DO forward pilot channel

	This command acquires 1xEV-DO forward pilot channel

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param iBand_Class = Indicates the band class. Possible values are:
		\code
          FTM_EVDO_BAND_CLASS_CELLULAR	= 0,
          FTM_EVDO_BAND_CLASS_PCS			= 1,
          FTM_EVDO_BAND_BC0	= 0,
          FTM_EVDO_BAND_BC1	= 1,
          FTM_EVDO_BAND_BC2	= 2,
          FTM_EVDO_BAND_BC3	= 3,
          FTM_EVDO_BAND_BC4	= 4,
          FTM_EVDO_BAND_BC5	= 5,
          FTM_EVDO_BAND_BC6	= 6,
          FTM_EVDO_BAND_BC7	= 7,
          FTM_EVDO_BAND_BC8	= 8,
          FTM_EVDO_BAND_BC9	= 9,
          FTM_EVDO_BAND_BC10	= 10,
          FTM_EVDO_BAND_BC11	= 11,
          FTM_EVDO_BAND_BC12	= 12,
          FTM_EVDO_BAND_BC13	= 13,
          FTM_EVDO_BAND_BC14	= 14,
          FTM_EVDO_BAND_BC15	= 15,
          FTM_EVDO_BAND_BC16	= 16,
          FTM_EVDO_BAND_BC17	= 17
		\endcode

	\param iCdma_Freq = Indicates the RF channel number.
	\param iAcq_Mode = Indicates the acquisition mode; possible value is FTM_ACQ_MODE_FULL(0)
	\param iDiversity_Mode = Indicates whether diversity is enabled (1) or	disabled (0).
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EVDO_PILOT_ACQ(	
								HANDLE hResourceContext,
								unsigned long iBand_Class, 
								unsigned short iCDMA_Freq, 
								unsigned long iACQ_Mode, 
								unsigned long iDiversity_Mode);

	/******************************************************************************/
	/**
	1XEV-DO Non-signaling, command 3.2.2 Acquire 1xEV-DO system time

	This command acquires 1xEV-DO system time

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\warning 

	*******************************************************************************/
	
	QLIB_API unsigned char QLIB_FTM_EVDO_SYS_TIME_ACQ( 
								HANDLE hResourceContext);
	
	/******************************************************************************/
	/**
	1XEV-DO Non-signaling, command 3.2.3 Demodulate 1xEV-DO forward CC/MAC/FTC

	This command demodulates 1xEV-DO forward CC/MAC/FTC

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iArq_Enable = Enables/disables the early termination feature when decoding the forward channel.
	\param iDrc_Lockperiod = Time interval in slots between transmission of two consecutive DRCLock bit 
													transmissions on the Forward MAC Channel.
	\param iDrc_Locklength = Number of times a DRCLock bit is repeated.
	\param iMac_Index	 = Medium access control index assigned to the AT: 5 to 63.
	\param iRab_Length = Number of slots over which reverse activity bit is transmitted.
	\param iRab_Offset = Slots in which a new reverse activity bit can be transmitted.

	\warning 
	*******************************************************************************/
	
	QLIB_API unsigned char QLIB_FTM_EVDO_DEMOD_CC_MAC_FTC(	
								HANDLE hResourceContext,
								unsigned char* iARQ_Enable, 
								unsigned char* iDRC_Lockperiod, 
								unsigned char* iDRC_Locklength,
								unsigned char* iMAC_Index, 
								unsigned char* iRAB_Index, 
								unsigned char* iRAB_Offset);

	/******************************************************************************/
	/**
	1XEV-DO Non-signaling, command 3.2.4 Modulate reverse access channel
	
	This subcommand modulates the reverse access channel.
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iACLC_Mask_I_Hi = Upper 32 bits of access channel long code mask for I quadrature.
	\param iACLC_Mask_I_Lo = Lower 32 bits of access channel long code mask for I quadrature.
	\param iACLC_Mask_Q_Hi = Upper 32 bits of access channel long code mask for Q quadrature.
	\param iACLC_Mask_Q_Lo = Lower 32 bits of access channel long code mask for Q quadrature.
	\param iAC_Txpattern = Data pattern to transmit.
	\param iAccess_Cycle_Duration = Duration of an access channel cycle.
	\param iOpen_Loop_Adjust = Nominal power used in open-loop power estimate.
	\param iProbe_Initial_Adjust = Correction power factor used in open-loop power estimate.
	\param iProbe_Num_Step = Maximum number of access probes in single-access sequence.
	\param iPower_Step = Probe power increase step.
	\param iPreamble_Length = Length of access probe preamble portion.

	\warning

	*******************************************************************************/

	QLIB_API unsigned char QLIB_FTM_EVDO_MOD_ACC(	
								HANDLE hResourceContext,
								unsigned long iACLC_Mask_I_Hi, 
								unsigned long iACLC_Mask_I_Lo, 
								unsigned long iACLC_Mask_Q_Hi, 
								unsigned long iACLC_Mask_Q_Lo,
								unsigned long iAC_Txpattern, 
								unsigned char iAccess_Cycle_Duration, 
								unsigned char iOpen_Loop_Adjust,
								unsigned char iProbe_Initial_Adjust, 
								unsigned char iProbe_Num_Step, 
								unsigned char iPower_Step, 
								unsigned char iPreamble_Length);

	/******************************************************************************/
	/**
	1XEV-DO Non-signaling, command 3.2.5 Modulate reverse traffic channel
	
	This subcommand modulates the reverse traffic  channel.
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iACK_Gain	=	Ratio of the power level of the ACK channel (when transmitted) to the power level 
											of the reverse pilot channel.
	\param iDataoffset_Nom = Nominal offset of reverse data channel power to the reverse pilot channel power.
	\param iDataoffset_9K6 = Ratio of reverse data channel power at 9.6 kbps to 
													 the nominal reverse data channel power at 9.6 kbps.
	\param iDataoffset_19K2 = Ratio of reverse data channel power at 19.2 kbps to
														the nominal reverse data channel power at 19.2 kbps.
	\param iDataoffset_38K4 = Ratio of reverse data channel power at 38.4 kbps to
														the nominal reverse data channel power at 38.4 kbps.
	\param iDataoffset_76K8 = Ratio of reverse data channel power at 76.8 kbps to
														the nominal reverse data channel power at 76.8 kbps.
	\param iDataoffset_153K6 = Ratio of reverse data channel power at 153.6 kbps to
														 the nominal reverse data channel power at 153.6 kbps.
	\param iDRC_Value = DRC value.
	\param iDRC_Cover = DRC cover.
	\param iDRC_Gain = Ratio of the power level of the DRC channel (when
											transmitted) to the power level of the reverse traffic pilot channel.
	\param iDRC_Length = DRC length.
	\param iDRC_Gating = DRC gating enable/disable.
	\param iFrame_Offset = Frame offset.
	\param iPRC_Enable = RPC enable/disable.
	\param iRPC_Step = Reverse power control step.
	\param iRRI = Reverse rate indicator.
	\param iRTCLC_Mask_I_Hi = Upper 32 bits of reverse traffic channel long code mask for I quadrature.
	\param iRTCLC_Mask_I_Lo = Lower 32 bits of reverse traffic channel long code mask for I quadrature.
	\param iRTCLC_Mask_Q_Hi = Upper 32 bits of reverse traffic channel long code mask for Q quadrature.
	\param iRTCLC_Mask_Q_Lo = Lower 32 bits of reverse traffic channel long code mask for Q quadrature.
	\param iRTC_Txpattern = Data pattern to transmit.
	\param iRTC_Txduty_Cycle = Frame duty cycle of RTCTxPattern. 8 bits. Duty cycle starts at MSB 
															and wraps around after LSB. 1 bit
	\param iHDR_ACK_Mode = ack, nak, or do not ack every slot in the non-signaling
			Mode:
		\code
           0 = FTM_EVDO_IS856_COMPLIANT: Normal IS-856 operation. Early decode enabled; do not ack.
           1 = FTM_EVDO_IS856_NO_ACK: IS856 but with no early termination. Early decode disabled; do not ack.
           2 = FTM_EVDO_IS890_MODE_0: First slot decode mode. Ack each slot (test app mode).
           3 = FTM_EVDO_IS890_MODE_1: Full packet decode mode; Nak each slot (test app mode).
           4 = FTM_EVDO_IS890A_MODE_ACK_BPSK: Always ack with bipolar keying (BPSK) modulation.
           5 = FTM_EVDO_IS890A_MODE_NAK_BPSK: Always nak with bipolar keying (BPSK) modulation.
           6 = FTM_EVDO_IS890A_MODE_ACK_OOK: Always ack with on-off keying (OOK) modulation.
           7 = FTM_EVDO_IS890A_MODE_NAK_OOK: Always nak with on-off keying (OOK) modulation.
		\endcode
		Default value is 2, FTM_EVDO_IS890_MODE_0.
	\warning
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EVDO_MOD_REVERSE_TRA(	
								HANDLE hResourceContext,
								unsigned char iACK_Gain, 
								unsigned char iDataoffset_Nom, 
								unsigned char iDataoffset_9K6, 
								unsigned char iDataoffset_19K2,
								unsigned char iDataoffset_38K4, 
								unsigned char iDataoffset_76K8, 
								unsigned char iDataoffset_153K6, 
								unsigned char iDRC_Value,
								unsigned char iDRC_Cover,
								unsigned char iDRC_Gain, 
								unsigned char iDRC_Length, 
								unsigned char iDRC_Gating, 
								unsigned char iFrame_Offset,
								unsigned char iPRC_Enable, 
								unsigned char iRPC_Step, 
								unsigned char iRRI, 
								unsigned long iRTCLC_Mask_I_Hi, 
								unsigned long iRTCLC_Mask_I_Lo,
								unsigned long iRTCLC_Mask_Q_Hi, 
								unsigned long iRTCLC_Mask_Q_Lo, 
								unsigned long iRTC_Txpattern, 
								unsigned char iRTC_Txduty_Cycle,
								unsigned char iHDR_ACK_Mode);

	/******************************************************************************/
	/**
	1XEV-DO Non-signaling, command 3.2.6 This subcommand deassigns all 1xEV-DO channels
	
	This subcommand deassigns all 1xEV-DO channels

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EVDO_CMD_RELEASE(	
								HANDLE hResourceContext);
	


	/******************************************************************************/
	/**
	EVDO Non-signaling command 3.2.7 Demodulate Forward Link with No Reverse Link Rev 0.


	This single replaces the this sequence of commands:
		QLIB_FTM_EVDO_DEMOD_CC_MAC_FTC()
		QLIB_FTM_EVDO_MOD_ACC()
		QLIB_FTM_EVDO_MOD_REVERSE_TRA()

	However, versions of AMSS created before 2006 may not have this command available in the embedded
	source code.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iDRC_VALUE = Data rate at which AN transmits data to AT; valid values are from 1 to 14.
	\param iDRC_COVER = DRC cover value associated with the sector from which the forward traffic data at a particular DRC
						value is requested; valid values are from 1 to 7.
	\param iARQ_ENABLE = Enables/disables the early termination feature when decoding the forward channel
	\param iDRC_LOCKPERIOD = Time interval in slots between transmission of two consecutive DRCLock bit transmissions on the
							forward MAC channel; valid values are 0 and 1:
							0 = DRC lock period of 8 slots
							1 = DRC lock period of 16 slots
	\param iDRC_LOCKLENGTH = Number of times a DRCLock bit is repeated; valid values are from 0 to 3:
							0 = Repeated 4 times for Rev 0
							1 = Repeated 8 times for Rev 0
							2 = Repeated 16 times for Rev 0
							3 = Repeated 32 times for Rev 0
		
	\param iMAC_INDEX = Medium access control index assigned to the AT; valid values are 5 to 63 for Rev 0

	\param iRAB_LENGTH = Number of slots over which reverse activity bit is transmitted; valid values are 0 to 3:
							0 = Corresponds to 8 slots
							1 = Corresponds to 16 slots
							2 = Corresponds to 32 slots
							3 = Corresponds to 64 slots
						This field is applicable only for Rev 0.

	\param iRAB_OFFSET = Slots in which a new reverse activity bit can be transmitted; the value in slots of RAB Offset is the
						 value of the field x RAB_LENGTH/8.
						
						Valid values are from 0 to 7.
						
						This field is applicable only for Rev 0.
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EVDO_DEMOD_FWD_WITH_NO_REV(
										HANDLE hResourceContext,
										unsigned char iDRC_Value, 
										unsigned char iDRC_Cover, 
										unsigned char iARQ_Enable, 
										unsigned char iDRC_Lockperiod,
										unsigned char iDRC_Locklength, 
										unsigned char iMAC_Index, 
										unsigned char iRAB_Length, 
										unsigned char iRAB_Offset);

	/******************************************************************************/
	/**
	EVDO Non-signaling command 3.2.8 Set Idle mode

	This command puts the searcher in Idle mode
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EVDO_SET_IDLE( HANDLE hResourceContext );
	
	/******************************************************************************/
	/**
	EVDO Non-signaling command 3.2.9 Configure Rev A MAC for Forward CC/MAC/FTC
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSHORT_PACKET_ENABLE = Sets the Short Packet mode bit of the MDSP HDR mode register:
								0 = Do not try to decode short packet
								1 = Try all hypotheses, long and short packets, starting with the canonical (long) packet
	\param iMULTI_USER_PACKET_ENABLE = Enables or disables the use of multi-user packet; valid values are 0 and 1:
								0 = Disables multi-user packet
								1 = Enables multi-user packet

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EVDO_REV_A_CONF_MAC_FOR_FWD_CC_MAC_FTC(
													HANDLE hResourceContext, 
													unsigned char iShort_Packet_Enable, 
													unsigned char  iMulti_User_Packet_Enable);
	
	/******************************************************************************
	/**
	EVDO Rev A Non-signaling command 3.2.10 Modulate Reverse Access Channel Rev A

	This subcommand modulates the reverse access channel in IS-896 Rev A mode

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iACLC_MASK_I_HI = Upper 32 bits of access channel long code mask for I quadrature
	\param iACLC_MASK_I_LO = Lower 32 bits of access channel long code mask for I quadrature
	\param iACLC_MASK_Q_HI = Upper 32 bits of access channel long code mask for Q quadrature
	\param iACLC_MASK_Q_LO = Lower 32 bits of access channel long code mask for Q quadrature
	\param iAC_TXPATTERN = Data pattern to transmit
	\param iACCESS_CYCLE_DURATION = Duration of an access channel cycle in units of slots; 
		   valid values are in range of 1 to 255
	\param iOPEN_LOOP_ADJUST = Negative of the nominal power used in open-loop power estimate, 
							   expressed as an unsigned value in units of 1 dB
							   Value used by AT is -1 times the value of this field
	\param iPROBE_INITIAL_ADJUST =	Correction power factor used in open-loop power estimate for initial transmission 
									on access channel
									Expressed as 2s complement value in units of 1 dB.
									Valid values are from -16 dB to +15 dB
	\param iPROBE_NUM_STEP = Maximum number of access probes in singleaccess sequence; valid values are 1 to 15
	\param iPOWER_STEP = Probe power increase step; valid values are 0 to 15 in units of 0.5 dB
	\param iPREAMBLE_LENGTH =	Length of access probe preamble portion in frames of 16 slots; valid values are from 1 to 7 frames
								Applicable only if enhanced access channel feature is not enabled
	\param iENHANCED_ACCESS_ENABLE = Whether enhanced access channel is enabled; valid values are 0 and 1:
								0 = Enhanced access channel feature is disabled
								1 = Enhanced access channel feature is enabled
	\param iPREAMBLE_LENGTH_SLOTS = Preamble length in slots; valid values are 0 and 1:
								0 = Corresponds to 4 slots
								1 = Corresponds to 16 slots
								Applicable only if enhanced access channel feature is enabled
	\param iACCESS_OFFSET = Indicates the slot relative to the start of access cycle duration when the access terminal may start
							the access probe; valid values are 0 to 3 in steps of 4 slots
							Defaults to 0 if enhanced acess channel feature is not enabled
	\param iSECTOR_ACCESS_RATE_MAX = Indicates the maximum data rate at which access channel capsule can be transmitted tor the
									particular sector; valid values are from 0 to 2:
									0 = Corresponds to 9.6 kbps
									1 = Corresponds to 19.2 kbps
									2 = Corresponds to 38.4 kbps
									Defaults to 0 if enhanced access channel feature is not enabled
	\param iTERMINAL_ACCESS_RATE_MAX = Indicates the maximum data rate at which AT is allowed to transmit an access probe; 
										valid values are from 0 to 2:
									0 = Corresponds to 9.6 kbps
									1 = Corresponds to 19.2 kbps
									2 = Corresponds to 38.4 kbps
									Defaults to 0 if enhanced access channel feature is not enabled

	***********************************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EVDO_REV_A_MOD_ACC(	HANDLE hResourceContext, 
														unsigned long iACLC_MASK_I_HI, 
														unsigned long iACLC_MASK_I_LO, 
														unsigned long iACLC_MASK_Q_HI, 
														unsigned long iACLC_MASK_Q_LO, 
														unsigned long iAC_Txpattern,
														unsigned char iACCESS_CYCLE_DURATION, 
														unsigned char iOpen_Loop_Adjust, 
														unsigned char iProbe_Initial_Adjust, 
														unsigned char iProbe_Num_Step, 
														unsigned char iPower_Step, 
														unsigned char iPreamble_Length,
														unsigned char iEnhanced_Access_Enable, 
														unsigned char iPreamble_Length_Slots, 
														unsigned char iAccess_Offset, 
														unsigned char iSector_Access_Rate_Max,
														unsigned char iTerminal_Access_Rate_Max);
	
	/******************************************************************************
	/**

	EVDO Rev A Non-signaling command 3.2.11 Modulate Reverse Traffic Channel Rev A

	This subcommand modulates the reverse traffic channel in IS-896 Rev A.
	
	\param iDRC_VALUE = Data rate at which AN transmits data to AT; valid values are 1 to 14
	\param iDRC_COVER_VALUE = DRC cover value associated with the sector from which the forward traffic data at 
								particular DRC value is requested; valid values are from 1 to 7
	\param iDRC_LENGTH = Number of slots the AT shall use to transmit a single DRC value; valid values are from 0 to 3:
						0 = DRC length is 1 slot
						1 = DRC length is 2 slots
						2 = DRC length is 4 slots
						3 = DRC length is 8 slots
	\param iDRC_GATING = DRC gating; valid values are 0 or 1:
						0 = Continuous transmission of DRC value in each slot
						1 = Discontinuous transmission of DRC channel
	\param iDRC_CH_GAIN =	Power of DRC channel relative to pilot channel expressed as 2s complement value in units of 0.5 dB;
							valid values are from -9 dB to 6 dB
	\param iACK_CH_GAIN = Power of the ACK channel relative to the pilot channel expressed as 2s complement value in units 
						  of 0.5 dB; valid values are from -3 dB to 6 dB
	\param iDSC_VALUE = Cell cover associated with the forward link data source; valid values are from 1 to 7
	\param iDSC_LENGTH = Length of single DSC value transmission in units of 8 slots; valid values are from 1 to 32
	\param iDSC_GAIN = Power of the DSC channel relative to the pilot channel in units of -0.5 dB; 
						valid values are from 0 to -15.5 dB
	\param iRPC_STEP = Power control step size to be used by AT when controlling the power of reverse link; valid values are 0 and 1:
						0 = Corresponds to 0.5 dB step
						1 = Corresponds to 1 dB step
	\param iFRAME_OFFSET = Offset from CDMA system time for frame start in units of slots; valid values are from 0 to 15 slots
	\param iRPC_ENABLE = RPC enable/disable
	\param iRTC_TXPATTERN = Data pattern to transmit
	\param iLCM_I_CHAN_HI = Long code mask for I channel high 32 bits
	\param iLCM_I_CHAN_LOW = Long code mask for I channel low 32 bits
	\param iLCM_Q_CHAN_HI = Long code mask for q channel high 32 bits
	\param iLCM_Q_CHAN_LOW = Long code mask for q channel low 32 bits
	\param iRRI_PAYLOAD = Physical layer packet size index; valid values are from 1 to 12
	\param iHI_CAP_LO_LATENCY = The transmit mode of operation; valid values are 0 and 1:
								0 = High capacity
								1 = Low capacity
	\param iGAIN_TRANSITION_POINT = Set to one less than the number of subframes for which the receiver shall use 
									pretransition T2P values and the number of subpackets after 
									which the receiver shall use posttransition values based on the above-set transmission 
									mode and RRI; valid values are from 0 to 3
	\param iT2P_PRE_TRANSITION = Set to the data channel power relative to the pilot channel power based on above-set RRI and
								 transmission mode for the subframes prior to T2P transition; 
								 valid values are from 0 to 128 (inclusive) in units of 0.25 dB
	\param iT2P_POST_TRANSITION = Set to the data channel power relative to the pilot channel power based on above-set RRI and
								transmission mode for the subframes following the T2P_transition_point; 
								valid values are from 0 to 128 (inclusive) in units of 0.25 dB
	\param iARQ_MODE = Reverse link ARQ mode for sending Acks to and Nacks for reverse link packets; valid values are 0 and 1:
								0 = ARQ mode bi-polar
								1 = ARQ mode ON-OFF keying
	\param iAUX_PILOT_MIN_PAYLOAD =	Minimum reverse traffic channel payload for which the acess channel is required to 
									transmit the auxiliary pilot channel; valid values are from 0 to 11
	\param iAUX_PILOT_CHAN_GAIN = Set to auxiliary pilot channel gain relative to the traffic data channel gain; 
									a 4-bit value in units of -1 dB in the range of 0 to -15 dB
	\param iRRICH_GAIN_PRE_TRANSITION = Set to RRI channel gain for the subframes transmitted before the T2P transition 
										based on above-set RRI and transmission mode; specified as
										2s complement 4-bit number in units of 1 dB
	\param iRRICH_GAIN_POST_TRANSITION = Set to RRI channel gain for the subframes transmitted following the T2P transition 
										based on above-set RRI and transmission mode; 
										specified as 2s complement 4-bit number in units of 1 dB
	\param iHDR_ACK_Mode = ack, nak, or do not ack every slot in the non-signaling
			Mode:
		\code
           0 = FTM_EVDO_IS856_COMPLIANT: Normal IS-856 operation. Early decode enabled; do not ack.
           1 = FTM_EVDO_IS856_NO_ACK: IS856 but with no early termination. Early decode disabled; do not ack.
           2 = FTM_EVDO_IS890_MODE_0: First slot decode mode. Ack each slot (test app mode).
           3 = FTM_EVDO_IS890_MODE_1: Full packet decode mode; Nak each slot (test app mode).
           4 = FTM_EVDO_IS890A_MODE_ACK_BPSK: Always ack with bipolar keying (BPSK) modulation.
           5 = FTM_EVDO_IS890A_MODE_NAK_BPSK: Always nak with bipolar keying (BPSK) modulation.
           6 = FTM_EVDO_IS890A_MODE_ACK_OOK: Always ack with on-off keying (OOK) modulation.
           7 = FTM_EVDO_IS890A_MODE_NAK_OOK: Always nak with on-off keying (OOK) modulation.
		\endcode
		Default value is 2, FTM_EVDO_IS890_MODE_0.

	*********************************************************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EVDO_REV_A_MOD_TRA(	HANDLE hResourceContext, 
														unsigned char iDRC_Value, 
														unsigned char iDRC_Cover_Value, 
														unsigned char iDRC_Length, 
														unsigned char iDRC_Gating, 
														unsigned char iDRC_Ch_Gain,
														unsigned char iACK_Ch_Gain, 
														unsigned char iDSC_Value, 
														unsigned char iDSC_Length, 
														unsigned char iDSC_Gain, 
														unsigned char iRPC_Step, 
														unsigned char iFrame_Offset,
														unsigned char iRPC_Enable, 
														unsigned long iRTC_Txpattern, 
														unsigned long iLCM_I_CHAN_HI, 
														unsigned long iLCM_I_CHAN_LOW, 
														unsigned long iLCM_Q_CHAN_HI,
														unsigned long iLCM_Q_CHAN_LOW, 
														unsigned char iRRI_Payload, 
														unsigned char iHi_Cap_Lo_Latency, 
														unsigned char iGain_Transition_Point, 
														unsigned char iT2P_Pre_Transition,
														unsigned char iT2P_Post_Transition, 
														unsigned char iARQ_Mode, 
														unsigned char iAux_Pilot_Min_Payload, 
														unsigned char iAux_Pilot_Chan_Gain,
														unsigned char iRRICH_Gain_Pre_Transition, 
														unsigned char iRRICH_Gain_Post_Transition,
														unsigned char iHDR_ACK_Mode);
	
	/*********************************************************************************************************************
	/**
	
	EVDO Rev A Non-signaling command 3.2.12 Demodulate Forward Link with No Reverse Link Rev A
	
	\param iDRC_VALUE = Data rate at which AN transmits data to AT; valid values are from 1 to 14
	\param iDRC_COVER = DRC cover value associated with the sector from which the forward traffic data at a particular DRC
						value is requested; valid values are from 1 to 7
	\param iDSC_VALUE = Cell cover associated with the forward link data source; valid values are from 1 to 7
	\param iARQ_ENABLE = Enables/disables the early termination feature when decoding the forward channel
	\param iDRC_LOCKPERIOD =	Time interval in slots between transmission of two consecutive DRCLock bit transmissions on the
								forward MAC channel; valid values are 0 and 1:
								0 = DRC lock period of 8 slots
								1 = DRC lock period of 16 slots
	\param iDRC_LOCKLENGTH = Number of times a DRCLock bit is repeated; valid values are from 0 to 3:
							0 = Repeated 8 times for Rev A
							1 = Repeated 16 times for Rev A
							2 = Repeated 32 times for Rev A
							3 = Repeated 64 times for Rev A
	\param iMAC_INDEX = Medium access control index assigned to the AT; valid values are 5 to 63 and 72 to 127 for Rev A
	\param iRAB_LENGTH = Number of slots over which reverse activity bit is transmitted; valid values are from 0 to 3:
							0 = Corresponds to 8 slots
							1 = Corresponds to 16 slots
							2 = Corresponds to 32 slots
							3 = Corresponds to 64 slots
						This field is applicable only for Rev0. For RevA mode, this field is always set to 0.
	\param iRAB_OFFSET = Slots in which a new reverse activity bit can be transmitted. 
						The value in slots of RAB Offset is the value of the field x RAB_LENGTH/8; 
						valid values are from 0 to 7.
						This field is applicable only for Rev 0.
						For Rev A mode, this field is always set to 0.
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EVDO_REV_A_DEMOD_FWD_WITH_NO_REV(	HANDLE hResourceContext, 
																		unsigned char iDRC_Value, 
																		unsigned char iDRC_Cover, 
																		unsigned char iDSC_Value, 
																		unsigned char iARQ_Enable, 
																		unsigned char iDRC_Lockperiod,
																		unsigned char iDRC_Locklength, 
																		unsigned char iMAC_Index, 
																		unsigned char iRAB_Length, 
																		unsigned char iRAB_Offset);

/******************************************************************************
						Bluetooth FTM
*******************************************************************************/

	/******************************************************************************/
	/**
	Factory Test Mode Bluetooth Commands 1.2.1.1 User-specified HCI command

	Send a host controller interface (HCI) command to a device

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param pBuffer = pointer to a series of bytes that will be sent as an HCI command.
	\param iLength = number of bytes to send, from pBuffer
	
	\return true if successful, false if fail.

	\warning QLIB_FTM_SET_MODE( PHONE_MODE_BLUETOOTH ) must be called first

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_BT_HCI_USER_CMD
		( HANDLE hResourceContext, unsigned char* pBuffer, unsigned char iLength);

	/******************************************************************************/
	/**
	Factory Test Mode Bluetooth, custom command to activate Bluetooth mode

	Sends: 1) HCI Enable DUT, 2)HCI Write Scan Enable, 3) HCI Set Event Filter Conn 2

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param =pBuffer = pointer to a series of bytes that will be sent as an HCI command.
	\param iLength = number of bytes to send, from pBuffer
	
	\return true if successful, false if fail.

	\warning QLIB_FTM_SET_MODE( PHONE_MODE_BLUETOOTH ) must be called first

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_BT_Enable_Bluetooth( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	Factory Test Mode Bluetooth, custom command turn on BT transmitter to a specific channel number

	The underlying HCI command is a vendor-specific command called BT_HCI_TX_CONTINUOUS_FREQ. 
	This does not actually produce a CW signal, rather a a DH5 packet is transmitted every 6 slots.
	The result is a bursted signal.  It is apparently not possible to transmit just a CW signal
	using the available HCI commands.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iChannel= channel number for Tx. (Tx frequency = 2402 + chan_num, in MHz)
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_BT_SetCW(  HANDLE hResourceContext, unsigned char iChannel );

	/******************************************************************************/
	/**
	Factory Test Mode Bluetooth, custom command turn on BT transmitter with a CW,
	to a specific channel number

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iAddr = 6 digit Bluetooth address.  This will temporarily load the BT address
	               into embedded RAM.  When modes are reset or the phone is restarted,
				   the address will be gone.

                   The address will be transformed as follows:
				   Input array: { 0x12, 0x90, 0x78, 0x56 0x34, 0x12 }

				   Resulting BT address: 123456789012
				

	\return true if successful, false if fail.

	\warning If the NV_BD_ADDR_I NV item has a value then the NV address will always
	         be used and the value specified in this command will be ignored.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_BT_SetAddr(  HANDLE hResourceContext, unsigned char iAddr[6] );

	/******************************************************************************/
	/**

	This command issues a Bluetooth "Read_Local_Version_Information" HCI command to 
	the Bluetooth module and then capture the resulting Bluetooth event log, which 
	contains the Local_Version of the Bluetooth module.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param pBluetootHCI_Version = reference to the structure that will be filled in with the local version information.
	                              use the type Bluetooth_LocalVersion_struct, defined in QLIB_Defines.h

	\return true if successful, false if fail.

	\warning Will reset the log code and logging state 
	

	*******************************************************************************/
	QLIB_API unsigned char QLIB_BT_GetModuleVersion( HANDLE hResourceContext, unsigned char* pBluetootHCI_Version  );


/******************************************************************************
						Audio FTM
*******************************************************************************/
	/******************************************************************************/
	/**
	Factory Test Mode Audio, 2.4 Audio Set Path command

		The audio set path command activates a supported audio device in the phone. The audio device
		refers to the physical connection of a microphone/speaker or another audio input/output hardware
		to one of the vocoder’s PCM paths. Typical audio devices are handset, headset, and Bluetooth
		headset.

		NOTE Since some of the audio devices are “featurized,” not all audio devices are available on all
		platforms. If an audio path selected is not supported in the phone, the status field in the response
		packet is set to 2.
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iAudioDevice - set the receive/transmit pair of audio devices

	\code
		0 = Handset
		1 = Handsfree kit
		2 = Mono headset
		3 = Stereo headset
		4 = Analog handsfree kit
		5 = Stereo DAC
		6 = Speaker phone
		7 = TTY Handsfree kit
		8 = TTY headset
		9 = TTY VCO
		10 = TTY HCO
		11 = Bluetooth intercom
		12 = Bluetooth headset
		13 = Bluetooth local audio
		14 = USB audio
		15 = SND_DEVICE_FM_RADIO_HEADSET
		16 = SND_DEVICE_FTM_HANDSET
		17 = SND_DEVICE_FTM_HEADSET
		18 = SND_DEVICE_FTM_SPEAKER
		19 = SND_DEVICE_FTM_CARKIT

	\endcode
		* The microphone and speaker used by each audio device varies between platforms. Refer
		to file msmaud.h for the mapping between audio devices and microphone/speaker pairs.


	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_AUDIO_SET_PATH( HANDLE hResourceContext, unsigned short iAudioDevice );

	/******************************************************************************/
	/**
	Factory Test Mode Audio, 2.5 Audio Set Volume command
		The audio set volume command sets the RXvolume on the current audio path to the specified
		level (reference Figure 2-1). The volume steps are defined in arrays corresponding to the sound
		device and method in sndcal.c. Volume steps are from 0...4 (0 being mute and 4 being the
		loudest).	

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iVolume - Volume steps from 0...4

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_AUDIO_SET_VOLUME( HANDLE hResourceContext, unsigned char iVolume );


	/******************************************************************************/
	/**
	Factory Test Mode Audio, 2.6 Audio DSP Loopback command

		The DSP loopback command can be used to perform an audio loopback in the DSP. After this
		command is sent, audio samples from the microphone specified in the current audio path are
		routed to the corresponding speaker.

	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param bLoopbackOn - unsigned charean to switch DSP loopback on or off. 0 = Off, 1 = On

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_AUDIO_DSP_LOOPBACK( HANDLE hResourceContext, unsigned char bLoopbackOn );

	/******************************************************************************/
	/**
	Factory Test Mode Audio, 2.7 Audio PCM Loopback command
		The PCM loopback command can be used to perform a PCM loopback in the codec. Once this
		command is sent, audio samples from the microphone specified in the current audio path are
		looped back in the codec to the corresponding speaker.

	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param bLoopbackOn  - unsigned charean to switch PCM loopback on or off. 0 = Off, 1 = On

	\code
	\endcode


	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_AUDIO_PCM_LOOPBACK( HANDLE hResourceContext, unsigned char bLoopbackOn );

	/******************************************************************************/
	/**
	Factory Test Mode Audio, 2.8 Audio Tones Play command

		The tones play command can be used to play out tones in specified frequencies, using the speaker
		in the current audio path. The user can specify up to two frequencies, the higher one first.

		Only one frequency is desired, the iLoFreq parameter is set to 0
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iHiFreq - Higher of the two frequencies in Hz. The maximum frequency that can be specified is 3500 Hz
	\param iLoFreq - Lower of the two frequencies in Hz. The minimum, frequency that can be specified is 50 Hz
	\param iMethod - method to play the sound. FTM_Audio_Sound_Methods_Enum can be used for the
	                 basic values, but the specific audio features must be known to access methods beyond #3
		\code
		
          SND_METHOD_VOICE = 0,   // Use the device's voice generator
  
          // Application-Specific Methods
  
          SND_METHOD_KEY_BEEP = 1,// Use the device's keybeep generator              
          SND_METHOD_MESSAGE = 2, // Use the path's ringer, or voice generator       
          SND_METHOD_RING = 3,    // Use the device's ring generator                 
          #ifdef FEATURE_CLICK
             SND_METHOD_CLICK,       // Use the device's click generator                
          #endif
          #if defined(FEATURE_AUDIO_FORMAT) || defined(FEATURE_MIDI_OUT)
            SND_METHOD_MIDI,        // Use the device's Midi generator                 
          #endif
            SND_METHOD_AUX,         // Use the device's auxiliary generator if avail. 
            SND_METHOD_MAX          // Always last in the list                         

		\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_AUDIO_TONES_PLAY
		( HANDLE hResourceContext, unsigned short iHiFreq, unsigned short iLoFreq, unsigned short iMethod );

	/******************************************************************************/
	/**
	Factory Test Mode Audio, 2.9 Audio Tones Stop command
		The tones stop command stops the current tone that is being played.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_AUDIO_TONES_STOP( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	Factory Test Mode Audio, 2.10 Audio Noise Suppressor command
		The noise suppressor command can be used to turn the noise suppressor on or off.

	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iNoiseSuppressorOn - 0 = Off, 1 = On (the default state)

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_AUDIO_NS_CONTROL( HANDLE hResourceContext, unsigned char iNoiseSuppressorOn );


	/******************************************************************************/
	/**
	Factory Test Mode Audio, 
		
		  Captures PCM data at 8kHz, with 16-bit samples.

	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param iNumBuffers - Number of buffers to be captured.  Can be from 1 to 100.

	                     Each buffer is 160 samples, 1 sample = 16 bits.  
						 100 buffers = 100 * 160 words = 100 * 160 * 2 byte = 32,000 bytes

	\param iDelay_ms - Number of milliseconds to wait between sending the command and receving
	                   a response packet.  This time increased as more buffers are captured.  
					   The maximum value needed is 2000 for to seconds.

	\return true if successful, false if fail.

	\warning QLIB_FTM_AUDIO_PCM_STOP_CAPTURE must be called after the PCM data is transferred to the PC

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_AUDIO_PCM_CAPTURE
		( HANDLE hResourceContext, unsigned short iNumBuffers, unsigned short iDelay_ms);

	/******************************************************************************/
	/**
	Factory Test Mode Audio, 
		Releases the memory buffer held for the captured PCM data.  
		Must be called after QLIB_FTM_AUDIO_PCM_CAPTURE.
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_AUDIO_PCM_STOP_CAPTURE( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	Factory Test Mode Audio, 2.11 Get Audio PCM Capture Data
		This command is used to return the data from a PCM capture.  Data is 
		separated into section of 512 unsigned chars.  This command returns one section of the
		data last captured using the FTM_AUDIO_PCM_CAPTURE command

	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iDataOffset - Offset in the PCM Capture buffer from which to return.
	\param iDataSize   - Number of bytes to transfer

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_AUDIO_GET_PCM_CAPTURE_DATA
		( HANDLE hResourceContext, unsigned long iDataOffset, unsigned short iDataSize, unsigned char* pBuffer);

	/******************************************************************************/
	/**
	Factory Test Mode Audio

		This function calls the FTM_AUDIO_PCM_CAPTURE and FTM_AUDIO_GET_PCM_CAPTURE_DATA
		functions such that the PCM data will be captured and all of the data filled 
		into the pBuffer storage area specified by the user.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iNumBuffers - Number of buffers to be captured.  Can be from 1 to 100.

	                     Each buffer is 160 samples, 1 sample = 16 bits.  
						 100 buffers = 100 * 160 words = 100 * 160 * 2 byte = 32,000 bytes

	\param iDelay_ms - delay in milliseconds between the Send Request and the receive Response operations
	\param pBuffer = storage space for PCM data.  For buffer size, see comments for bCreateWAVformat

	\param iDataTransferSize - number of bytes to transfer for each time the 
	                     FTM_AUDIO_GET_PCM_CAPTURE_DATA function is called.
						 This number should be a factor of the iDataSize number.
						 Only the value 512 will be tested for this parameter.

	\param bCreateWAVformat = Flag to indicate if the buffer should be stored in a WAV format.
	                          If set to 0, no action occurs.  If set to 1, the first 44 bytes of
							  the buffer will be filled in with a WAV header and the PCM data
							  will be stored after that.

	                          Note!  If this option is set to 1, then the user must specify
							  a buffer that is 44 bytes larger than the PCM data to be captured.

							  The WAV format is referenced from this web site:
							     http://ccrma.stanford.edu/courses/422/projects/WaveFormat/ 
	\code							  
       The WAV header is created as follows:
       // WAVE Header
       const dword c_iWAV_HeaderSize = 44;
       const byte c_iWAV_HeaderBuffer[ c_iWAV_HeaderSize ] = 
       {
       0x52, 0x49, 0x46, 0x46, 0x3E, 0xA4, 0x00, 0x00, // 1-8
       0x57, 0x41, 0x56, 0x45, 0x66, 0x6D, 0x74, 0x20, // 9-16
       0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, // 17-24
       0x40, 0x1F, 0x00, 0x00, 0x80, 0x3E, 0x00, 0x00, // 25-32
       0x02, 0x00, 0x10, 0x00, 0x64, 0x61, 0x74, 0x61, // 33-40
       0x80, 0x3E, 0x00, 0x00                          // 41-44
       };
	\endcode							  
							  

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_AUDIO_CapturePCMandTransferData
		(	HANDLE hResourceContext, 
			unsigned short iNumBuffers, 
			unsigned short iDelay_ms,
			unsigned char* pBuffer, 
			unsigned short iDataTransferSize, 
			unsigned char bCreateWAVformat
		);


	/******************************************************************************/
	/**
	Factory Test Mode Audio, 
		Set the adjustment factor for CodecTxGain.  
		The change will take effect immediately in the codec.

		The value still needs to be written to the NV item NV_CODECTXGAIN_ADJUST_I
		once the proper level is determined.

	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iCodecTxGainAdjust = adjustment value from -600 to +600, in 1/100dB steps

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_AUDIO_SET_CODECTXGAIN_ADJUST( HANDLE hResourceContext, short iCodecTxGainAdjust );



/******************************************************************************
						Camera FTM
*******************************************************************************/

	/******************************************************************************/
	/**
	Factory Test Mode Camera, 2.3.1 Camera Start
		This command starts the camera services. The power to the sensor is turned on and set to
		the power-up state for operation. The sensor and the device driver are initialized. Setting picture
		dimensions and taking pictures cannot happen until this command is executed.
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode


	\code
	This section of sample code taken from the QlibDemo.cpp program.  It demonstrates how to use the 
	Camera API.  The assumption is that the QLIB communications with the phone have already been setup
	and the variable g_hResourceContext is a valid HANDLE.


	// Setup variables
	char _sPictureFileName[] = "C:\\FactoryTest.jpg";
	unsigned char	_bAutoPictureSize = true;	// Auto determine picture width
	unsigned short	_iPictureWidth = 0;			// not used because auto-
	unsigned short	_iPictureHeight = 0; 
	unsigned short	_iDelay_ms = 2000;			// 2 seconds
	unsigned long	_iQuality = 99;				// % quality (0-99)
	unsigned short	_iFormat = CAMERA_JPEG;		// JPEG format
	unsigned long	_iPicDataSize = 0;			// Will get updated when picture is taken
	unsigned short	_iDataTransferSize = 512;	// Number of bytes transferred at a time
	void*	_piPicData = NULL;
	unsigned short	_iCameraStatus = CAMERA_SUCCESS;

	printf("\nTaking picture...");
	bOk = QLIB_FTM_CAMERA_PerformTakePictureSequence
			( 
			g_hResourceContext, 
			_bAutoPictureSize, 
			_iPictureWidth, 
			_iPictureHeight, 
			_iDelay_ms, 
			_iQuality, 
			_iFormat, 
			&_iPicDataSize, 
			&_iCameraStatus 
			);
	printf("\nQLIB_FTM_CAMERA_PerformTakePictureSequence = %s", bOk ? "PASS" : "FAIL");

	// Now transfer the picture into memory
	if (	( bOk ) 
		&&	(_iPicDataSize != 0 )
		)
	{

		// Allocate memory 
		_piPicData = malloc( _iPicDataSize );

		if ( _piPicData != NULL )
		{
			printf("\nRetrieving picture (%d bytes)...", _iPicDataSize );

			bOk = QLIB_FTM_CAMERA_GetEntirePicture
					( 
						g_hResourceContext, 
						_iPicDataSize, 
						_iDataTransferSize, 
						(unsigned char*) _piPicData, 
						&_iCameraStatus 
					);

			printf("\nQLIB_FTM_CAMERA_GetEntirePicture = %s", bOk ? "PASS" : "FAIL");

			if (bOk)
			{
				// Select binary mode for the IO library
				_fmode = _O_BINARY;

				// Setup file pointer
				FILE* _hGraphicFile = NULL;

				// Now store this data into a file
				if (_hGraphicFile = fopen( _sPictureFileName,"w"))
				{
					// Write the data to a file
					fwrite( _piPicData, _iPicDataSize, 1, _hGraphicFile);
				}	// fopen()

				// Close the file
				if (_hGraphicFile != NULL )
					fclose( _hGraphicFile );

			}	// if (bOk)

		}	// if ( _piPicData != NULL )

		// Close the Camera Service
		bOk = QLIB_FTM_CAMERA_STOP( g_hResourceContext,  &_iCameraStatus );

		// Free th memory
		free( _piPicData);

	}	// if ( bOk ... )


	\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CAMERA_START( HANDLE hResourceContext, unsigned short* piCameraStatus );

	/******************************************************************************/
	/**
	Factory Test Mode Camera, 2.3.2 Camera Stop
		This command terminates the camera services. The power sensor is turned off. This command
		also de-allocates the memory heap allocated for picture storage.

	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CAMERA_STOP( HANDLE hResourceContext, unsigned short* piCameraStatus );

	/******************************************************************************/
	/**
	Factory Test Mode Camera, 2.3.3 Camera Set Parm
		This command sets the operational parameters of the camera sensor. For a list of possible
		parameter identifiers, refer to the Camera Services Interface Specification and Operational
		Description (80-V5310-1).

	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param	iParmId = Camera parameter identifier.  Refer to 80-V5310-1 for definitions of parameter identifiers.
	\code
        0 = CAMERA_PARM_STATE (Read Only)
        1 = CAMERA_PARM_ACTIVE_CMD (Read Only)
        2 = CAMERA_PARM_ZOOM
        3 = CAMERA_PARM_ENCODE_ROTATION
        4 = CAMERA_PARM_SENSOR_POSITION
        5 = CAMERA_PARM_CONTRAST
        6 = CAMERA_PARM_BRIGHTNESS
        7 = CAMERA_PARM_SHARPNESS
        8 = CAMERA_PARM_EXPOSURE
        9 = CAMERA_PARM_WB
        10 = CAMERA_PARM_EFFECT
        11 = CAMERA_PARM_AUDIO_FMT
        12 = CAMERA_PARM_FPS
        13 = CAMERA_PARM_FLASH
        14 = CAMERA_PARM_RED_EYE_REDUCTION
        15 = CAMERA_PARM_NIGHTSHOT_MODE
        16 = CAMERA_PARM_REFLECT
        17 = CAMERA_PARM_PREVIEW_MODE
        18 = CAMERA_PARM_ANTIBANDING
        19 = CAMERA_PARM_THUMBNAIL_WIDTH
        20 = CAMERA_PARM_THUMBNAIL_HEIGHT
        21 = CAMERA_PARM_THUMBNAIL_QUALITY
	\endcode
	\param	iParmValue = This is the camera parameter value. 
	                     Refer 80-V5310-1 for specific definitions for each parameter identifiers.

	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CAMERA_SET_PARM
		( 
			HANDLE hResourceContext, 
			unsigned short iParmId, 
			unsigned long iParmValue, 
			unsigned short* piCameraStatus 
		);

	/******************************************************************************/
	/**
	Factory Test Mode Camera, 2.3.4 Camera Set Dimensions
		This command sets the picture dimensions. The picture width is the horizontal width of the
		picture in number of pixels and the picture height is the vertical height of the picture in number of
		lines. The Camera Start command must be executed before using this command. Refer to Chapter
		3 for recommended picture resolutions.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iPictureWidth - This is the horizontal width of the picture in number of pixels.
	\param iPictureHeight- This is the Vertical height of the picture in number of pixels.
	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode
	\param iLCD_Width = # of pixel on the X axis of the LCD

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CAMERA_SET_DIMENSIONS
		( 
			HANDLE hResourceContext, 
			unsigned short iPictureWidth, 
			unsigned short iPictureHeight, 
			unsigned short iLCD_Width,
			unsigned short* piCameraStatus 
		);

	/******************************************************************************/
	/**
	Factory Test Mode Camera, 2.3.5 Camera Start Preview
		This command enters the preview state of camera services. This command must be executed each
		time the Camera Start or the Camera Set Dimensions command is called.
		NOTE A two second delay is recommended between Camera Start Preview and Camera Stop Preview
		commands in order for white balance and exposure to take effect. There is no software feedback
		mechanism indicating these parameters are ready; two seconds is a reasonable time for the human
		eye, which is the only method to verify this.

	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CAMERA_START_PREVIEW( HANDLE hResourceContext, unsigned short* piCameraStatus );

	/******************************************************************************/
	/**
	Factory Test Mode Camera, 2.3.6 Camera Stop Preview
		This command exits the preview state of camera services. This command must be executed after
		Camera Start Preview command and before a picture is taken.
		NOTE A two second delay is recommended between Camera Start Preview and Camera Stop Preview
		commands in order for white balance and exposure to take effect. There is no software feedback
		mechanism indicating these parameters are ready; two seconds is a reasonable time for the human
		eye, which is the only method to verify this.

	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CAMERA_STOP_PREVIEW( HANDLE hResourceContext, unsigned short* piCameraStatus );

	/******************************************************************************/
	/**
	Factory Test Mode Camera, 2.3.7 Camera Take Picture and Encode

		This command takes the picture, encodes the picture, and saves it in the phone’s allocated
		memory heap. The format of the picture can be either raw or JPEG-encoded data stored in the
		allocated memory buffer. Memory heap is allocated when this command is executed. Refer to
		Appendix A for details on how the allocated memory buffer or memory length is calculated.
		The compression ratio or quality factor is selected, and it impacts the output file size. A quality
		factor from 1 to 99 can be selected, with 1 as the lowest quality and 99 as the highest quality. In
		addition, the compression ratio is only used for JPEG-encoded format (not with the raw format).

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param	iQuality - This is the compression ratio or quality factor (valid range of [1:99], 
	                   with 1 as the lowest quality and 99 as the highest quality).
	\param iFormat   - Format of picture data:
	\code
               0 = CAMERA_RAW
               1 = CAMERA_JPEG
	\endcode

	\param piPicDataSize = This is the size of the entire picture in bytes.  

	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CAMERA_TAKE_PICTURE_AND_ENCODE
		( 
			HANDLE hResourceContext, unsigned long iQuality, unsigned short iFormat, 
			unsigned long* piPicDataSize, unsigned short* piCameraStatus 
		);

	/******************************************************************************/
	/**
	Factory Test Mode Camera, 2.3.8 Camera Get Picture
		This command retrieves raw or JPEG-encoded picture data from the allocated memory space
		used in the Camera Take Picture and Encode command, based on a data offset and size. Given the
		data offset and size, this command will need to be executed a number of times until all picture
		data is retrieved.
		The recommended size to retrieve at one time is 512 bytes. The picture data format is based on
		the format parameter configured in the Camera Take Picture and Encode command.
		NOTE When retrieving the picture data, the picture will not be displayed on the phone’s LCD.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iDataOffset - The DATA_OFFSET is the offset in which to start retrieving picture data from memory.
	\param iDataSize - The size of picture data in bytes to retrieve from memory during this transaction.
	\param piPicData - The actual picture data is returned into this buffer, the number of bytes is DATA_SIZE.
  
	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CAMERA_GET_PICTURE
		( 
			HANDLE hResourceContext, unsigned long iDataOffset, unsigned short iDataSize, 
			unsigned char* piPicData, unsigned short* piCameraStatus 
		);

	/******************************************************************************/
	/**
	Factory Test Mode Camera, 2.3.9 Camera Get Parm
		This command retrieves the operational parameters of the camera sensor. For a list of possible
		parameter identifiers, refer to Camera Services Interface Specification and Operational
		Description (80-V5310-1).

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iParmID - Camera parameter identifier. Refer to 80-V5310-1 for definitions of parameter identifiers.		
	\code
        0 = CAMERA_PARM_STATE (Read Only)
        1 = CAMERA_PARM_ACTIVE_CMD (Read Only)
        2 = CAMERA_PARM_ZOOM
        3 = CAMERA_PARM_ENCODE_ROTATION
        4 = CAMERA_PARM_SENSOR_POSITION
        5 = CAMERA_PARM_CONTRAST
        6 = CAMERA_PARM_BRIGHTNESS
        7 = CAMERA_PARM_SHARPNESS
        8 = CAMERA_PARM_EXPOSURE
        9 = CAMERA_PARM_WB
        10 = CAMERA_PARM_EFFECT
        11 = CAMERA_PARM_AUDIO_FMT
        12 = CAMERA_PARM_FPS
        13 = CAMERA_PARM_FLASH
        14 = CAMERA_PARM_RED_EYE_REDUCTION
        15 = CAMERA_PARM_NIGHTSHOT_MODE
        16 = CAMERA_PARM_REFLECT
        17 = CAMERA_PARM_PREVIEW_MODE
        18 = CAMERA_PARM_ANTIBANDING
        19 = CAMERA_PARM_THUMBNAIL_WIDTH
        20 = CAMERA_PARM_THUMBNAIL_HEIGHT
        21 = CAMERA_PARM_THUMBNAIL_QUALITY
	\endcode

	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piMinimumValue - Returns the minimum value of the operational parameter.
	\param piMaximumValue - Returns the maximum value of the operational parameter.
	\param piStepValue    - Returns the step value of the operational parameter.
	\param piDefaultValue - Returns the default value of the operational parameter.
	\param piCurrentValue - Returns the current value of the operational parameter.
	
	\param  piCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CAMERA_GET_PARM
		(	
			HANDLE hResourceContext, unsigned short iParmId, unsigned long* piMinimumValue, 
			unsigned long* piMaximumValue, unsigned long* piStepValue, unsigned long* piDefaultValue, 
			unsigned long* piCurrentValue, unsigned short* piCameraStatus 
		);

	/******************************************************************************/
	/**
	Factory Test Mode Camera, 2.3.10 Camera Get Info
		This command retreives camera information such as the sensor width, height, rotation and
		orientation. Refer to Chapter 3 for possible sensor resolutions.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piSensorWidth  - Returns the sensor width.
	\param piSensorHeight - Returns the sensor height.
	\param piFPS          - Returns the frames per second.
	\param piSensorRotationSensing  - Returns whether the sensor can sense when it is rotated (0 = not rotated, 1=Rotated)
	\param piDefaultRotation        - Returns the default rotation value when the sensor is not rotated.
	\param piDefaultOrientation     - Returns orientation (landscape or portrait):
	\code
        0 = CAMERA_ORIENTATION_LANDSCAPE
        1 = CAMERA_ORIENTATION_PORTRAIT
	\endcode

	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CAMERA_GET_INFO
			(	HANDLE hResourceContext, 
				unsigned short* piSensorWidth, unsigned short* piSensorHeight, 
				unsigned short* piFPS, unsigned char* piSensorRotationSensing, 
				unsigned short* piDefaultRotation,  unsigned short* piDefaultOrientation,
				unsigned short* piCameraStatus 
			);

	/******************************************************************************/
	/**
	Factory Test Mode Camera, Select camera sensor
		This command selects which camera sensor will be used for subsequent calls
		to the FTM Camera API.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSensorID  - Sensor number to select.  The sensor number is the same ID number used
	                    in the embedded source code, specifically to the call to 
						camera_select_sensor().  If you are unsure of the sensor ID, then ask
						the embedded developers.

	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CAMERA_SET_SENSOR_ID
			(	HANDLE hResourceContext, 
				unsigned char iSensorID,
				unsigned short* piCameraStatus 
			);

	/******************************************************************************/
	/**
	Factory Test Mode Camera, FTM_CAMERA_PerformTakePictureSequencePicture

		This command will sequence several steps in the FTM Camera API to take a picture.  
		After this command is issued, the FTM_CAMERA_GetEntirePicture() can be called to return
		the picture data to the PC memory.

		The steps taken are documented in Section A.1 in the FTM Camera API document:
			1) FTM_START_CAMERA
			2) FTM_CAMERA_SET_PARM(  CAMERA_PARM_PREVIEW_MODE, CAMERA_PREVIEW_MODE_SNAPSHOT )
			3) FTM_CAMERA_SET_DIMENSIONS( x, y)
			4) FTM_CAMERA_START_PREVIEW()
			5) Delay delay_ms
			6) FTM_CAMERA_STOP_PREVIEW()
			7) image_size = FTM_CAMERA_TAKE_PICTURE_AND_ENCODE( quality, format )
			8) Return image_size
			

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param bAudioPictureWidth - 1=Determine picture dimentions automatically, by using the largest size possible
	                            for the installed sensor.  If this value == 1, then the iPictureWidth and iPictureHeight
								values are ignored.

	                            0=Use the dimensions specified in iPictureWidth and iPictureHeight
	\param iPictureWidth - This is the horizontal width of the picture in number of pixels.
	\param iPictureHeight- This is the Vertical height of the picture in number of pixels.
	\param iDelay_ms - delay in milliseconds between the Send Request and the Receive Response operations
	\param	iQuality - This is the compression ratio or quality factor (valid range of [1:99], 
	                   with 1 as the lowest quality and 99 as the highest quality).
	\param iFormat   - Format of picture data:
	\code
               0 = CAMERA_RAW
               1 = CAMERA_JPEG
	\endcode

	\param piPicDataSize = This is the size of the entire picture in bytes.  

	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode

	\return true if successful, false if fail.

	\warning FTM_STOP_CAMERA() must be called after this function is called.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CAMERA_PerformTakePictureSequence
		( 
		HANDLE hResourceContext, 
		unsigned char bAutoPictureSize, unsigned short iPictureWidth, unsigned short iPictureHeight, 
		unsigned short iDelay_ms, unsigned long iQuality, unsigned short iFormat, 
		unsigned long* piPicDataSize, unsigned short* piCameraStatus 
		);

	/******************************************************************************/
	/**
	Factory Test Mode Camera, FTM_CAMERA_GetEntirePicture

		This function calls the FTM_CAMERA_GET_PICTURE funciton as many times as
		necessary to return the entire picture from the phone to the PC.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iDataSize - The size of picture data in bytes to retrieve from memory during per transaction.
	\param piPicData - The actual picture data is returned into this buffer, the number of bytes is DATA_SIZE.
	
	\param  iCameraStatus - Camera services status, returns code from calling camera services API:
	\code
       0 = CAMERA_SUCCESS
       1 = CAMERA_INVALID_STATE
       2 = CAMERA_INVALID_PARM
       3 = CAMERA_INVALID_FORMAT
       4 = CAMERA_NO_SENSOR
       5 = CAMERA_NO_MEMORY
       6 = CAMERA_NOT_SUPPORTED
       7 = CAMERA_FAILED
       8 = CAMERA_INVALID_STAND_ALONE_FORMAT
       9 = CAMERA_MALLOC_FAILED_STAND_ALONE
	\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_CAMERA_GetEntirePicture
		( 
			HANDLE hResourceContext, unsigned long iPicDataSize, unsigned short iDataTransferSize, 
			unsigned char* piPicData, unsigned short* piCameraStatus 
		);


/******************************************************************************
						FTM Log
*******************************************************************************/
	/******************************************************************************/
	/**
	FTM Log, 3.1 FTM enable log

		FTM log packets are enabled with the FTM_LOG_ENABLE request. The parameters to this
		request are the ID of the FTM log to enable, the periodicity of log packet generation, and the
		number of readings to be included in each log packet. To reduce log bandwidth, several readings
		can be buffered into a single log packet and sent as a single burst.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iLogCode - FTM Log packet number
	\param iInterval - Time (in ms) between subsequent log packets
	\param iNumBuffers - Number of buffers per log packet

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_LOG_ENABLE( 
		HANDLE hResourceContext, unsigned short iLogCode, unsigned long iInterval, unsigned long iNumBuffers  );


	/******************************************************************************/
	/**
	FTM Log, 3.2 FTM disable log
		An FTM log is disabled with the FTM_LOG_DISABLE command.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iNumBuffers - Number of buffers per log packet

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_LOG_DISABLE( HANDLE hResourceContext, unsigned short iLogCode );

	/******************************************************************************/
	/**
	High level function to activate logging for a specific FTM2 log.  This function
	also turns on QMSL logging and sets up the phone's log filter to allow
	the FTM2 log to be generated.

	The operations called are:
		QLIB_FTM_LOG_ENABLE
		QLIB_DIAG_SetExtendedLogCode
		QLIB_DIAG_SetPhoneLoggingState
		QLIB_DIAG_ClearPhoneLogQueue

	\param iFTM_LogCode = FTM Log code, such as LOG_FTM2_LOG_WCDMA_BER or LOG_FTM2_LOG_PRINTF

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_LOG_StartFTM_Log( HANDLE hResourceContext, unsigned short iFTM_LogCode);

	/******************************************************************************/
	/**
	High level function to disable logging for a specific FTM2 log.  This function
	also turns off QMSL logging.

	The operations called are:
		FTM_LOG_DISABLE
		DIAG_SetPhoneLoggingState
		DIAG_ClearPhoneLogQueue

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iFTM_LogCode = FTM Log code, such as LOG_FTM2_LOG_WCDMA_BER or LOG_FTM2_LOG_PRINTF

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_LOG_StopFTM_Log( HANDLE hResourceContext, unsigned short iFTM_LogCode);

	/******************************************************************************/
	/**
	High level function to return the most recently received log report, or if no report
	is in the async queue, then the function will wait up-to a specified amount of time 
	for a report to be received.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iFTM_LogCode = FTM Log code, such as LOG_FTM2_LOG_WCDMA_BER or LOG_FTM2_LOG_PRINTF
	\param pFTM2LogMessage = pointer to a structure of FTM2LogMessage type (defined in QLib_Defines.h).
	       If a lot message is found, the FTM2-specific section will be copied into this
		   structure.
	\param piLogSize = number of bytes copied into the pFTM2LogMessage structure

	\param iTimeOut_ms = millisecond time-out. If not log code is detected by the time this 
	                     expires, then the return value will be false.
	

	\return true if a log was found, false if one was not found.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_LOG_WaitForNextLog( 
					HANDLE hResourceContext, 
					unsigned short iFTM_LogCode, 
					void* pFTM2LogMessage, 
					unsigned short* piLogSize,
					unsigned long iTimeOut_ms );



/******************************************************************************
						WCDMA BER FTM
*******************************************************************************/

	/******************************************************************************/
	/**
	WCDMA BER, Start a WCDMA BER session. After this function is called, the acquisition
	function will need to be called, FTM_WCDMA_BER_Acquire.

		This function calls these operations and checks for errors:
			QLIB_FTM_WCDMA_BER_ClearStatus

		These low-level FTM operations are called and the associated events are verifed:
			START_WCDMA_MODE_REQ

		If the first START_WCDMA_MODE_REQ call fails, because a physical channel is
		already setup, then the function will call STOP_WCDMA_MODE_REQ and try to 
		re-start the WCDMA non-signaling session.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iTimeOut_ms = millisecond timeout for all WCDMA BER operations.  This should
						 be long enough that all operations will return the desired
						 events before the timeout is complete.  

						 If the command response events occur before the timeout is 
						 complete then the program flow will continue immediately, so there
						 is no penalty for having a long time-out, unless an error occurs.

						 FTM_NONSIG_DEFAULT_TIMEOUT to use the system's default timeout.

						 Will only be updated when a session is being intilalized.

	\return true if network session is setup successful, false if there are any failures

	\warning The active log mask will be changed to log only log FTM2 log packets.
	         When the function is completed, the state of ASYNC logging will be enabled.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WCDMA_BER_StartSession(	HANDLE hResourceContext, unsigned long iTimeOut_ms );

	/******************************************************************************/
	/**
	WCDMA BER, acquire a downlink pilot.  Before this function is called, 
	FTM_WCDMA_BER_StartSession must be called.  When the function is completed, the 
	phone will be in the IDLE state.


	These low-level FTM operations are called and the associated events are verifed:
		ACQUIRE_REQ

		START_WCDMA_IDLE_REQ - will be called if any RMC channels have been setup sinc
		the last time that ACQUIRE_REQ has been called.
		
	This function will automatically retry the downlink acquisition up to 5 times, for the 
	very first acquisition after the QLIB_FTM_WCDMA_BER_StartSession() is called. 
	This is necessary because there are some layer 1 reasons why the first acquisition 
	may not be successful immediately. In testing of 1000 attempts, it was to not 
	explicitly issue an WCDMA_BER_IDLE command because this command is not necessary.			

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iChannel = WCDMA RF channel

	\return true if network session is setup successful, false if there are any failures

	\warning Default values are used for the following:
		\code
          iAcqType = FTM_WCDMA_BER_AcqTypeFreqOnly
          iAcqMode = FTM_WCDMA_BER_AcqModeFull
          iScrCode = 0
          iPN_Pos  = 0
		\endcode

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WCDMA_BER_AcquirePilot( HANDLE hResourceContext, unsigned short iChannel );

	/******************************************************************************/
	/**
	WCDMA BER, go to IDLE mode.

		This function calls these operations and checks for errors:
			FTM_WCDMA_START_IDLE_MODE_REQ

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if idle mode has been entered successfully, false if there are any failures

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WCDMA_BER_Idle( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	WCDMA BER, Setup a RMC DCH channel.  This must be done after the 
	FTM_WCDMA_BER_AcquirePilot function has completed successfully.

		This low-level FTM operation is called and the associated event is verifed:
			RMC_DCH_SETUP_REQ

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iRMC_Type (refer to FTM_WCDMA_BER_RMC_Type_Enum)
		\code
			FTM_WCDMA_BER_RMC_Type12_2kpbs				= 0,	//<!' RMC 12.2 kbps channel
			FTM_WCDMA_BER_RMC_Type64kbps				= 1,	//<!' RMC 64 kbps channel
			FTM_WCDMA_BER_RMC_Type384kpbs				= 2,	//<!' RMC 384 kbps channe (Not currently supported)
			FTM_WCDMA_BER_RMC_Type12_2kpbs_Symmetric	= 3,	//<!' RMC 12.2 kbps channel, block size in the DL and UL is the same
			FTM_WCDMA_BER_RMC_Type64kbps_Symmetric		= 4,	//<!' RMC 64 kbps channel, block size in the DL and UL is the same
			FTM_WCDMA_BER_RMC_Type384kbps_Symmetric		= 5		//<!' RMC 384 kbps channel, block size in the DL and UL is the same
			Note: For first 3 data rate enumeration, UL CRC bits are looped back from DL
			Note: For last  3 data rate enumeration, UL CRC is valid and computed based on Rx data bits
		\endcode
	\param iUL_MaxTxPwr = Maximum transmit power in dBm.
		
	\param iUL_DPCH_ScrCode = Scrambling code to use for the uplink.
	\param iUL_PCA (refer FTM_WCDMA_BER_PCA_Size_Enum)
		\code
          FTM_WCDMA_BER_PCA_Type_Alg1 = 0 = Power control algorithm 1
          FTM_WCDMA_BER_PCA_Type_Alg2 = 1 = Power control algorithm 2
		\endcode
	\param iUL_TPC_StepSize 
		\code
         FTM_WCDMA_BER_PCA_Size_1dB = 0 = Stepsize 1 dB
         FTM_WCDMA_BER_PCA_Size_2dB = 1 = Stepsize 2 dB
		\endcode
	\param iDL_PriCPICH_Scr_Code = Primary CPICH downlink scrambling code.
	\param iDL_ChanCode = Downlink DPCH channelization code.

	\return true if all operations succeeded and the correct event messages were received

	\warning 

	*******************************************************************************/	
	QLIB_API unsigned char QLIB_FTM_WCDMA_BER_SetupRMC_DCH( 
			HANDLE hResourceContext, 
			unsigned char iRMC_Type, unsigned char iUL_MaxTxPwr, unsigned long iUL_DPCH_ScrCode, unsigned char iUL_PCA,
			unsigned char iUL_TPC_StepSize, unsigned short iDL_PriCPICH_Scr_Code, unsigned short iDL_ChanCode );

	/******************************************************************************/
	/**
	WCDMA BER, handover to a new RMC DCH channel.  This must be done after the 
	QLIB_FTM_WCDMA_BER_SetupRMC_DCH function has completed successfully.

		This low-level FTM operation is called and the associated event is verifed:
			RMC_DCH_SETUP_REQ_V2

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iRMC_Type (refer to FTM_WCDMA_BER_RMC_Type_Enum)
		\code
			FTM_WCDMA_BER_RMC_Type12_2kpbs				= 0,	//<!' RMC 12.2 kbps channel
			FTM_WCDMA_BER_RMC_Type64kbps				= 1,	//<!' RMC 64 kbps channel
			FTM_WCDMA_BER_RMC_Type384kpbs				= 2,	//<!' RMC 384 kbps channe (Not currently supported)
			FTM_WCDMA_BER_RMC_Type12_2kpbs_Symmetric	= 3,	//<!' RMC 12.2 kbps channel, block size in the DL and UL is the same
			FTM_WCDMA_BER_RMC_Type64kbps_Symmetric		= 4,	//<!' RMC 64 kbps channel, block size in the DL and UL is the same
			FTM_WCDMA_BER_RMC_Type384kbps_Symmetric		= 5		//<!' RMC 384 kbps channel, block size in the DL and UL is the same
			Note: For first 3 data rate enumeration, UL CRC bits are looped back from DL
			Note: For last  3 data rate enumeration, UL CRC is valid and computed based on Rx data bits
		\endcode
	\param iUL_MaxTxPwr = Maximum transmit power in dBm.
		
	\param iUL_DPCH_ScrCode = Scrambling code to use for the uplink.
	\param iUL_PCA (refer FTM_WCDMA_BER_PCA_Size_Enum)
		\code
          FTM_WCDMA_BER_PCA_Type_Alg1 = 0 = Power control algorithm 1
          FTM_WCDMA_BER_PCA_Type_Alg2 = 1 = Power control algorithm 2
		\endcode
	\param iUL_TPC_StepSize 
		\code
         FTM_WCDMA_BER_PCA_Size_1dB = 0 = Stepsize 1 dB
         FTM_WCDMA_BER_PCA_Size_2dB = 1 = Stepsize 2 dB
		\endcode
	\param iDL_PriCPICH_Scr_Code = Primary CPICH downlink scrambling code.
	\param iDL_ChanCode = Downlink DPCH channelization code.
	\param iBetaC = Gain factor for DPCCH
	\param iBetaD = Gain factor for DPDCH
	\param iChannel = WCDMA RF Channel (downlink)

	\return true if all operations succeeded and the correct event messages were received

	\warning 

	*******************************************************************************/	
	QLIB_API unsigned char QLIB_FTM_WCDMA_BER_Handover( 
			HANDLE hResourceContext, 
			unsigned char iRMC_Type, unsigned char iUL_MaxTxPwr, unsigned long iUL_DPCH_ScrCode, unsigned char iUL_PCA,
			unsigned char iUL_TPC_StepSize, unsigned short iDL_PriCPICH_Scr_Code, unsigned short iDL_ChanCode,
			unsigned char iBetaC, unsigned char iBetaD, unsigned short iChannel);

	/******************************************************************************/
	/**
	WCDMA BER, handover to a new RMC DCH channel.  This must be done after the 
	FTM_WCDMA_BER_SetupRMC_DCH function has completed successfully.

	This low-level FTM operation is called and the associated events are verifed:
	of the embedded function RMC_DCH_SETUP_REQ_V2.

	For the timer values (Txxx) refer to the 3GPP standard 25.331, section 8
	For the counter values (Nxxx) refer to section "13.3 - UE constants and parameters"

		This low-level FTM operation is called and the associated event is verifed:
			RMC_DCH_SETUP_REQ_V2B

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iRMC_Type (refer to FTM_WCDMA_BER_RMC_Type_Enum)
		\code
			FTM_WCDMA_BER_RMC_Type12_2kpbs				= 0,	//<!' RMC 12.2 kbps channel
			FTM_WCDMA_BER_RMC_Type64kbps				= 1,	//<!' RMC 64 kbps channel
			FTM_WCDMA_BER_RMC_Type384kpbs				= 2,	//<!' RMC 384 kbps channe (Not currently supported)
			FTM_WCDMA_BER_RMC_Type12_2kpbs_Symmetric	= 3,	//<!' RMC 12.2 kbps channel, block size in the DL and UL is the same
			FTM_WCDMA_BER_RMC_Type64kbps_Symmetric		= 4,	//<!' RMC 64 kbps channel, block size in the DL and UL is the same
			FTM_WCDMA_BER_RMC_Type384kbps_Symmetric		= 5		//<!' RMC 384 kbps channel, block size in the DL and UL is the same
			Note: For first 3 data rate enumeration, UL CRC bits are looped back from DL
			Note: For last  3 data rate enumeration, UL CRC is valid and computed based on Rx data bits
		\endcode
	\param iUL_MaxTxPwr = Maximum transmit power in dBm.
		
	\param iUL_DPCH_ScrCode = Scrambling code to use for the uplink.
	\param iUL_PCA (refer FTM_WCDMA_BER_PCA_Size_Enum)
		\code
          FTM_WCDMA_BER_PCA_Type_Alg1 = 0 = Power control algorithm 1
          FTM_WCDMA_BER_PCA_Type_Alg2 = 1 = Power control algorithm 2
		\endcode
	\param iUL_TPC_StepSize 
		\code
         FTM_WCDMA_BER_PCA_Size_1dB = 0 = Stepsize 1 dB
         FTM_WCDMA_BER_PCA_Size_2dB = 1 = Stepsize 2 dB
		\endcode
	\param iDL_PriCPICH_Scr_Code = Primary CPICH downlink scrambling code.
	\param iDL_ChanCode = Downlink DPCH channelization code.
	\param iBetaC = Gain factor for DPCCH. Default is 8.
	\param iBetaD = Gain factor for DPDCH. Default is 15.
    \param iDPCCH_PowerOffset = DPCCH Power Offset, default is -60
    \param n_312 = Maximum number of "in sync" received from L1.  Default is 0 (s1).
		\code
           FTM_WCDMA_BER_N312_s1    = 0
           FTM_WCDMA_BER_N312_s50   = 1
           FTM_WCDMA_BER_N312_s100  = 2
           FTM_WCDMA_BER_N312_s200  = 3
           FTM_WCDMA_BER_N312_s400  = 4
           FTM_WCDMA_BER_N312_s600  = 5
           FTM_WCDMA_BER_N312_s800  = 6
           FTM_WCDMA_BER_N312_s1000 = 7
		\endcode
    \param n_313 = Maximum number of successive "out of sync" received from L1. Default is 4 (s20).
		\code
           FTM_WCDMA_BER_N313_s1   = 0,
           FTM_WCDMA_BER_N313_s2   = 1,
           FTM_WCDMA_BER_N313_s4   = 2,
           FTM_WCDMA_BER_N313_s10  = 3,
           FTM_WCDMA_BER_N313_s20  = 4,
           FTM_WCDMA_BER_N313_s50  = 5,
           FTM_WCDMA_BER_N313_s100 = 6,
           FTM_WCDMA_BER_N313_s200 = 7
		\endcode
    \param n_315 = Maximum number of successive "in sync" received from L1 during T313 is activated. Default is 0 (s1).
		\code
           FTM_WCDMA_BER_N315_s1   = 0,
           FTM_WCDMA_BER_N315_s50  = 1,
           FTM_WCDMA_BER_N315_s100 = 2,
           FTM_WCDMA_BER_N315_s200 = 3,
           FTM_WCDMA_BER_N315_s400 = 4,
           FTM_WCDMA_BER_N315_s600 = 5,
           FTM_WCDMA_BER_N315_s800 = 6,
           FTM_WCDMA_BER_N315_s1000 = 7
		\endcode
    \param t_312 = T312: Timer in seconds for physical channel establishment procedure.  Refer to 25.331 Section 8 for 
	                     explicit definition.  Default is 1.
    \param t_313 = T313: Timer in seconds for radio link failure detection.  Refer to 25.331 Section 8 for explicit definition.
	               Default is 3.
	\param iChannel = WCDMA RF Channel (downlink). 

	\return true if all operations succeeded and the correct event messages were received

	\warning The active log mask will be changed to log only log FTM2 log packets.
	         When the function is completed, the state of ASYNC logging will be enabled.

	*******************************************************************************/	
	QLIB_API unsigned char QLIB_FTM_WCDMA_BER_Handover_V2B( 
			HANDLE hResourceContext,
			unsigned char iRMC_Type, unsigned char iUL_MaxTxPwr, unsigned long iUL_DPCH_ScrCode, unsigned char iUL_PCA,
			unsigned char iUL_TPC_StepSize, unsigned short iDL_PriCPICH_Scr_Code, unsigned short iDL_ChanCode,
			unsigned char iBetaC, unsigned char iBetaD, 
			short iDPCCH_PowerOffset,
			unsigned char n_312,
			unsigned char n_313,
			unsigned char n_315,
			unsigned char t_312,
			unsigned char t_313,	
			unsigned short iChannel );

	/******************************************************************************/
	/**
	WCDMA BER, Setup a default RMC DCH channel.  This function calls 
	QLIB_FTM_WCDMA_BER_SetupRMC_DCH() with the following parameters:


	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	iRMC_Type =  RMC 12.2 kbps channel
	iRMC_Type =  24dBm.
	iUL_DPCH_ScrCode = 0 = Scrambling code to use for the uplink.
	iUL_PCA =  Power Control Algorithm 1
	iUL_TPC_StepSize  = Stepsize 1 dB
	iDL_PriCPICH_Scr_Code = 0 = Primary CPICH downlink scrambling code.
	iDL_ChanCode = 3 = Downlink DPCH channelization code.

	\return true if all operations succeeded and the correct event messages were received

	\warning 

	*******************************************************************************/	
	QLIB_API unsigned char QLIB_FTM_WCDMA_BER_SetupDefaultRMC_DCH( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	WCDMA BER, 3.8 FTM WCDMA TFCS Configuration request

	The tfcs_config table must be 4 unique numbers, all between 0 and 3.  
	This defines how the TFCI values should be mapped to each Transport Format 
	Combination.  

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param aiTFCS_Config= 
	
		To solve the Agilent issue you would send the command using tfcs_select = 
		FTM_WCDMA_BER_UL_TFCS_CONFIG, and tfcs_config = {0,2,1,3}.  
	
		If you don't send the command the default behavior is tfcs_config = {0,1,2,3}.  
		Subtle difference, enough to break the Agilent.  Right now we don't really 
		need to send a DL_TFCS_CONFIG since other the equipment are flexible or adhere 
		to the standard RMC config.

	\param eTFCS_Select = specifies configuration of UL or DL, defined by
		\code
           FTM_WCDMA_BER_UL_TFCS_CONFIG = 0,
           FTM_WCDMA_BER_DL_TFCS_CONFIG = 1
		\endcode

	\param iStatus 
		\code
          0 = Success (COMMAND_SUCCESS)
          1 = Failure (COMMAND_FAILED)
		\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WCDMA_BER_RMC_DCH_TFCS_CONFIG_REQ( 
		HANDLE hResourceContext,
		unsigned char eTFCS_Select,
		unsigned char aiTFCS_Config[ 4] );

	/******************************************************************************/
	/**
	WCDMA BER, Clear the internal status of the WCDMA BER structure

		Resets the contents of all fields in the internal WCDMA_BER_State state structure.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if all operations succeeded and the correct event messages were received

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WCDMA_BER_ClearStatus( HANDLE hResourceContext );


	/******************************************************************************/
	/**
	WCDMA BER, Returns a copy of the internal WCDMA BER state. On the internal copy
	         it clears the "updated" fields for each message type so that the
			 next time this funciton is called. Only items that been received
			 since the first function call are marked as "updated."
	         

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param pWCDMA_BER_State = reference to a WCDMA_BER_State structure that will be 
	                        filled in with the state information

	\return true if all operations succeeded and the correct event messages were received

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WCDMA_BER_GetStatus( HANDLE hResourceContext, unsigned char* pWCDMA_BER_State );

	/******************************************************************************/
	/**
	WCDMA BER, Returns a simplified status of whether a specified event has occured.

	The function operations upon the bHasUpdated array held in the WCDMA BER status
	structure (data type WCDMA_BER_State, as defined in QLib_Defines.h).  

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param eEventID = The input parameter is an index into this array, and the function
	                   returns a true if the event has occured and false if it has not.

		Data type is log_FTM2_WCDMA_BER_log_id_enum, defined in QLib_Defines.h

		Values are:
		\code
         LOG_FTM2_WCDMA_START_MODE_CNF          = 0 = FTM WCDMA BER start WCDMA mode confirmation
         LOG_FTM2_WCDMA_STOP_MODE_CNF           = 1 = FTM WCDMA BER stop WCDMA mode confirmation
         LOG_FTM2_WCDMA_START_IDLE_MODE_REQ     = 2 = FTM WCDMA BER start idle mode confirmation
         LOG_FTM2_WCDMA_ACQUIRE_CNF             = 3 = FTM WCDMA BER acquire confirmation
         LOG_FTM2_WCDMA_CPHY_SETUP_CNF	        = 4 = FTM WCDMA BER CPHY setup confirmation
         LOG_FTM2_WCDMA_PHYCHAN_ESTABLISHED_IND = 5 = FTM WCDMA BER physical channel established confirmation
         LOG_FTM2_WCDMA_CPHY_ERROR_IND          = 6 = FTM WCDMA BER pyhsical channel error
         LOG_FTM2_WCDMA_RL_FAILURE_IND          = 7 = FTM WCDMA BER Reverselink error
		\endcode
	         

	\param pWCDMA_BER_State = reference to a WCDMA_BER_State structure that will be 
	                        filled in with the state information

	\return true the event type has occurred, false if it has not.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WCDMA_BER_GetEventStatus( HANDLE hResourceContext, unsigned char eEventID );

	/******************************************************************************/
	/**
	WCDMA BER, close the WCDMA Non-signaling session.

		This low-level FTM operation is called and the associated event is verifed:
 			STOP_WCDMA_MODE_REQ

	                 
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\return true if all operations succeeded and the correct event messages were received

	\warning This function will disable async logging and clear the async queue.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WCDMA_BER_StopSession( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	
	Start logging of WCDMA AGC packets.

	This command calls the FTM_LOG_ENABLE() function with the proper settings.
	Subsequently the function WaitForNextWCDMA_AGC_Log() can be called to wait
	for the next WCDMA log message and have the values returned.

	Note QLIB_FTM_WCDMA_BER_StartSession() automatically calls this function when
	it begins.

	\return true if successful, false if fail.

	\warning
		
        1. Before this function is called, the phone must be set in a WCDMA FTM mode, by
           calling either QLIB_FTM_SET_MODE() for basic RF testing, or by calling 
				QLIB_FTM_WCDMA_BER_StartSession() for WCDMA BER testing.

		2. This function will set the async messasge logging status to ON, to
           turn off async messages, QLIB_DIAG_SetPhoneLoggingState() should be called.

	 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WCDMA_AGC_Start_Log( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	
	Wait a specified number of milliseconds for the next WCMA AGC log message.

	If the message is found, then the contents of the message are copied into 

	This command calls the FTM_LOG_ENABLE() function with the proper settings.
	Subsequently the function WaitForNextWCDMA_AGC_Log() can be called to wait
	for the next WCDMA log message and have the values returned.

	\param pWCDMA_AGC_Log = pointer to the are to store the WCDMA AGC log information.
	                        This is of type "FTM2LogMessage_WCDMA_AGC" defined in
							QLIB_Defines.h

	\param iTimeout_ms = # of milliseconds to wait for the AGC.  Default is 300ms

	\return true if a valid WCDMA log was found within the specified timeout, false if not.

	\warning Before this function is called, StartWCDMA_AGC_Log() must be called to setup
	         capturing of WCDMA AGC messages.
	 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WCDMA_AGC_WaitForNextLog( 
					HANDLE hResourceContext,  void* pWCDMA_AGC_Log, unsigned long iTimeout_ms );

	/******************************************************************************/
	/**
	
	Stop logging of WCDMA AGC packets.  This is important to do before changing 
	RF modes or starting/stopping WCDMA BER

	This command calls the FTM_LOG_DISABLE() function with the proper settings.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if fail.

	\warning This function will set the async messasge logging status to OFF.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WCDMA_AGC_Stop_Log( HANDLE hResourceContext  );


	/******************************************************************************/
	/**
	UMTS non-signaling, ensure mobile phone is in a "stopped" state for all 
	UMTS non-signlaing modes (WCDMA/GSM).
	
	The procedure used is:
		1) Call STOP_WCDMA_MODE_REQ
		2) If the status code is successful, then this indicates the mobile was in
		   the WCDMA mode, so the function will wait for the corresponding event.

		3) Call STOP_GSM_MODE_REQ
		4) If the status code is successful, then this indicates the mobile was in
		   the GSM mode, so the function will wait for the corresponding event.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if the commands were executed without a device communication failure.

	\warning The mobile phone must be in FTM mode before this function is called.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_UMTS_BER_ForceStopped(	HANDLE hResourceContext );

/*******************************************************************************************
	HSDPA BER FTM
/*******************************************************************************************/

	/******************************************************************************/
	/**
	HSDPA BER, Start a HSDPA BER session. Before this operation is called, a valid
	WCDMA BER session must be started by calling QLIB_FTM_WCDMA_BER_StartSession().

		This function calls these operations and checks for errors:
			FTM_HSDPA_BER_ClearStatus

		This low-level FTM operation is called and the associated event is verifed:
			START_HSDPA_REQ

		If the first START_HSDPA_REQ call fails, because a mode is already started then the 
		function will stop HSDPA using STOP_HSDPA_REQ and try to re-start using START_HSDPA_REQ
		again.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if network session is setup successful, false if there are any failures

	\warning The active log mask will be changed to log only log FTM2 log packets.
	         When the function is completed, the state of ASYNC logging will be enabled.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_HSDPA_BER_StartSession( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	High level command for HSDPA BLER, calls RECONFIG_HSDPA_REQ and
	handles all related embedded events.

	While an HSDPA link is active, it is possible to dynamically reconfigure any or all of the high
	speed channels. The user should send updated channel configurations using the appropriate
	configuration commands in Sections 2.4 through 2.7, and then use this command to indicate
	which channels should be reconfigured by Layer 1. Note the reconfiguration happens using an
	activation time of now.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param iReconfigBitmask - This bitmask represents which channels should be reconfigured by L1. It is possible
	                          to reconfigure one or many channels by setting the appropriate bit in this 8 bit field. It
	                          is assumed that the actual updated channel configurations have already been sent
	                          down using the configuration commands. At least one of the channels must be
	                          specified for reconfiguration using this bitmask. The user should not set any of the
	                          upper 4 bits.
		\code
          Bit 0 = Reconfigure HS-DSCH channel
          Bit 1 = Reconfigure HS-PDSCH
          Bit 2 = Reconfigure HS-SCCH
          Bit 3 = Reconfigure HS-DPCCH
          Bits 4-7 = Reserved. Set to 0.

          Example: If the user wants to reconfigure both HS-DSCH and HS-DPCCH, the
                   reconfig_bitmask should be set to 0x09.
		\endcode


	\return true if successful, false if fail.

	\warning Not implemented 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_HSDPA_BLER_Reconfigure_HS( HANDLE hResourceContext, unsigned char iReconfigBitmask );

	/******************************************************************************/
	/**
	High level command for HSDPA BLER, calls CONFIG_HS_PDSCH_REQ and verifies the
	return status.

	This command configures the HS-PDSCH. All configurations are buffered by the FTM software.
	This command is used when the user sends the start HSDPA command (if HSDPA is inactive) or
	reconfigure command (if HSDPA is active).

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param iHS_PDSCH_RadioLink_PCS Primary scrambling code for the HS radio link.


	\return true if successful, false if fail.

	\warning Not implemented

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_HSDPA_BLER_Configure_PDSCH( HANDLE hResourceContext, unsigned short iHS_PDSCH_RadioLink_PCS );

	/******************************************************************************/
	/**
	High level command for HSDPA BLER, calls CONFIGURE_HS_SCCH_REQ  and verifies the
	return status.

	This command configures the HS-SCCH. The user can indicate to L1 the HS-SCCH
	channelisation code set. Up to four codes can be monitored at once by the UE.
	All configurations are buffered by the FTM software. This command is used when the user sends
	the start HSDPA command (if HSDPA is inactive) or reconfigure command (if HSDPA is
	active).

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param iNumCodes - Number of channelisation codes in the HS-SCCH code set. Range is from 1-4

	\param aiCodes[4] - Array of 4 unsigned chars, which represent the code numbers in the code set. Only num_codes
                      unsigned chars need to be valid. For example, if num_codes is 3, the 4 unsigned chars in the codes field
                      can be set to [ 12, 13, 14, X], where the first 3 unsigned chars dictates the code set, and X is
                      don’t care.

	\return true if successful, false if fail.

	\warning Not implemented 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_HSDPA_BLER_Configure_HS_SCCH( HANDLE hResourceContext, unsigned char iNumCodes, unsigned char aiCodes[4] );

	/******************************************************************************/
	/**
	High level command for HSDPA BLER, calls CONFIGURE_HS_DSCH_REQ and
	handles all related embedded events.

	This command configures the HS-DSCH. The user can indicate the H-RNTI identifier, the
	number of HARQ processes, and explicitly declare the Virtual IR buffer memory partition for
	each process. Up to eight HARQ processes can be configured. Note the total amount of Virtual IR
	buffer space that can be defined is limited by the UE category. If the user tries to configure the
	HS-DSCH for more soft memory than allowable by the UE, the UE will reject the configuration,
	and will result in an error in the HSDPA log events that results from the start or reconfigure
	HSDPA operation.

	All configurations are buffered by the FTM software. This command is used when the user sends
	the start HSDPA command (if HSDPA is inactive) or reconfigure command (if HSDPA is
	active).
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	
	\param iH_RNTI - 16 bit UE ID for use during HSDPA session.

	\param iNum_HARQ_Processes - Number of HARQ processes to define. Range is from 1-8.

	\param bMemPartitionValid - unsigned char, set to indicate the mem_partition_info field contains valid data.

	\param aiMemPartitionInfo[8] Array of 8 unsigned chars that represent the number of soft bits to define for each HARQ
                                 process. This array is only valid if mem_partition_valid field is set to TRUE. If
                                 so, only num_harq_processes elements of this array need to be valid. The rest
                                 can be don’t care values. Each valid element should take on one of the
                                 following values:
		\code
           0 = HMS800 (indicates 800 soft bits for the HARQ process)
           1 = HMS1600
           2 = HMS2400
           3 = HMS3200
           4 = HMS4000
           5 = HMS4800
           6 = HMS5600
           7 = HMS6400
           8 = HMS7200
           9 = HMS8000
           10 = HMS8800
           11 = HMS9600
           12 = HMS10400
           13 = HMS11200
           14 = HMS12000
           15 = HMS12800
           16 = HMS13600
           17 = HMS14400
           18 = HMS15200
           19 = HMS16000
           20 = HMS17600
           21 = HMS19200
           22 = HMS20800
           23 = HMS22400
           24 = HMS24000
           25 = HMS25600
           26 = HMS27200
           27 = HMS28800
           28 = HMS30400
           29 = HMS32000
           30 = HMS36000
           31 = HMS40000
           32 = HMS44000
           33 = HMS48000
           34 = HMS52000
           35 = HMS56000
           36 = HMS60000
           37 = HMS64000
           38 = HMS68000
           39 = HMS72000
           40 = HMS76000
           41 = HMS80000
           42 = HMS88000
           43 = HMS96000
           44 = HMS104000
           45 = HMS112000
           46 = HMS120000
           47 = HMS128000
           48 = HMS136000
           49 = HMS144000
           50 = HMS152000
           51 = HMS160000
           52 = HMS176000
           53 = HMS192000
           54 = HMS208000
           55 = HMS224000
           56 = HMS240000
           57 = HMS256000
           58 = HMS272000
           59 = HMS288000
           60 = HMS304000

		   Example: For Fixed Reference Channel HSET5 QPSK, the mem_partition_info
		   is set up as [ 11, 11, 11, X, X, X, X, X] where 11 represents 9600 soft bits for
		   HARQ processes 0-2, and X is don’t care value.
		\endcode

	\return true if successful, false if fail.

	\warning Not implemented

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_HSDPA_BLER_Configure_HS_DSCH( 
			HANDLE hResourceContext, 
			unsigned short iH_RNTI,
			unsigned char iNum_HARQ_Processes, 
			unsigned char bMemPartitionValid,  
			unsigned char aiMemPartitionInfo[8]
			);

	/******************************************************************************/
	/**
	High level command for HSDPA BLER, calls QLIB_FTM_HSDPA_BLER_Configure_HS_DSCH
	with a specific HSET configuration.

	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	
	\param iH_RNTI - 16 bit UE ID for use during HSDPA session.

	\param eHSET = parameter of type ftm_hsdpa_ber_hset_type (defined in QLib_Defines.h)
		\code
          HSDPA_HSET_1_QPSK = 1,		//!< ' HSET 1, QPSK
          HSDPA_HSET_1_16QAM,			//!< ' HSET1, 16QAM
          HSDPA_HSET_2_QPSK,			//!< ' HSET2, QPSK
          HSDPA_HSET_2_16QAM,			//!< ' HSET2, 16QAM
          HSDPA_HSET_3_QPSK,			//!< ' HSET3, QPSK
          HSDPA_HSET_3_16QAM,			//!< ' HSET3, 16QAM
          HSDPA_HSET_4_QPSK,			//!< ' HSET4, QPST
          HSDPA_HSET_5_QPSK,			//!< ' HSET5, QPST
          HSDPA_HSET_6_QPSK = 0xFF		//!< ' HSET6, QPST
		\endcode

	\return true if successful, false if fail.

	\warning Not implemented

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_HSDPA_BLER_Configure_HS_DSCH_HSET( HANDLE hResourceContext, unsigned short iH_RNTI, unsigned char eHSET );

	/******************************************************************************/
	/**
	High level command for HSDPA BLER, calls CONFIGURE_HS_DPCCH_REQ and
	handles all related embedded events.

	This command configures the HS-DPCCH. All configurations are buffered by the FTM software.
	This command is used when the user sends the start HSDPA command (if HSDPA inactive) or
	reconfigure command (if HSDPA active).

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param iCPICH_PowerOffset- Power offset of HS-PDSCH relative to serving CPICH. Values can be -12
                              to 26, indicating -6 dB to 13 dB, in 0.5 dB steps

	\param iCQI_FeedbackCycle - Period for CQI reports. Range is from 0-160 with units in milliseconds. 0
                              indicates CQI reporting is to be turned off. Valid values between 0 and
                              160 are defined in the 3GPP specifications

	\param iCQI_RepetitionFactor - Parameter that controls if CQI reports are to be repeated. Valid range is 1-4

	\param iDeltaCQI- Parameter that controls power adjustment for CQI reports. Valid range is 0-8.

	\param iDeltaACK - Parameters that controls power adjustments for ACK indications. Valid range is 0-8.

	\param iDeltaNACK - Parameter that controls power adjustments for NACK indications. Valid range is 0-8
	
	\param iACK_NACK_RepetitionFactor - Parameter that controls repetition of ACK/NACK indications. Valid range is 1-4.

	\return true if successful, false if fail.

	\warning Not implemented 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_HSDPA_BLER_Configure_HS_DPCCH( 
				HANDLE hResourceContext, 
				unsigned char iCPICH_PowerOffset, 
				unsigned char iCQI_FeedbackCycle,
				unsigned char iCQI_RepetitionFactor,
				unsigned char iDeltaCQI,
				unsigned char iDeltaACK,
				unsigned char iDeltaNACK,
				unsigned char iACK_NACK_RepetitionFactor
				);


	/******************************************************************************/
	/**
	WCDMA BER, Clear the internal status of the WCDMA BER structure

		Resets the contents of all fields in the internal WCDMA_BER_State state structure.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if all operations succeeded and the correct event messages were received

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_HSDPA_BER_ClearStatus( HANDLE hResourceContext );


	/******************************************************************************/
	/**
	HSDPA BER, Returns a copy of the internal WCDMA BER state. On the internal copy
	         it clears the "updated" fields for each message type so that the
			 next time this funciton is called. Only items that been received
			 since the first function call are marked as "updated."

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()	         

	\param pWCDMA_BER_State = reference to a WCDMA_BER_State structure that will be 
	                        filled in with the state information.  Structure type is
							HSDPA_BER_State.

	\return true if all operations succeeded and the correct event messages were received

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_HSDPA_BER_GetStatus( HANDLE hResourceContext, unsigned char* pHSDPA_BER_State );

	/******************************************************************************/
	/**
	HSDPA BER, close the HSDPA Non-signaling session by calling this function and
	checking for the corresponding event:
		STOP_HSDPA_REQ

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	                 
	\return true if all operations succeeded and the correct event messages were received

	\warning This function will disable async logging and clear the async queue.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_HSDPA_BER_StopSession( HANDLE hResourceContext );

/******************************************************************************
						GSM BER FTM
*******************************************************************************/

	/******************************************************************************/
	/**
	GSM BER, Start a GSM BER session. After this function is called, the BCCH acquistion 
	will need to be setup using FTM_GSM_BER_AssignBCCH.

		This function calls these operations and checks for errors:
			FTM_GSM_BER_ClearStatus

		This low-level FTM operation is called and the associated event is verifed:
			START_GSM_MODE_REQ

		\param iTimeOut_ms = millisecond timeout for all GSM BER operations.  This should
							 be long enough that all operations will return the desired
							 events before the timeout is complete.  

							 If the command response events occur before the timeout is 
							 complete then the program flow will continue immediately, so there
							 is no penalty for having a long time-out, unless an error occurs.

							 FTM_NONSIG_DEFAULT_TIMEOUT to use the system's default timeout.

							 Will only be updated when a session is being intilalized.

	\return true if network session is seutp successful, false if there are any failures

	\warning The active log mask will be changed to log only log FTM2 log packets.
	         When the function is completed, the state of ASYNC logging will be enabled.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GSM_BER_StartSession( 
			HANDLE hResourceContext, unsigned long iTimeOut_ms );

	/******************************************************************************/
	/**
	GSM BER, Assign a BCC.  Before this function is called, FTM_GSM_BER_StartSession
	must be called.  When the function is completed, the phone will be in the IDLE
	state.

	If using a CMU-200, the "connect" command should not be sent until after this function
	is executed.  Before this calling this function though, the BCCH channel should be 
	established so that the phone can successfully acquire.


		These low-level FTM operations are called and the associated events are verifed:
			SELECT_SPECIFIC_BCCH_REQ
			START_IDLE_MODE_REQ
			
		\param iChannel = RF channel. Valid range depends on band.
		\param iBand 
			\code
			  0 = PGSM 900 = FTM_GSM_BER_PGSM_900
			  1 = EGSM 900 = FTM_GSM_BER_EGSM_900 
			  2 = PCS 1900 = FTM_GSM_BER_PCS_1900
			  3 = DCS 1800 = FTM_GSM_BER_DCS_1800
			  4 = CELL 850 = FTM_GSM_BER_CELL_850
			\endcode

	\return true if network session is seutp successful, false if there are any failures

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GSM_BER_AssignBCCH(  
		HANDLE hResourceContext, unsigned short iChannel, unsigned char iBand);

	/******************************************************************************/
	/**
	GSM BER, Assign a GSM traffic channel and configure a loopback.  Before using this
	function, FTM_GSM_BER_AssignBCCH() should be completed successfully and if a CMU-200
	is being used, the "connect" command should be sent it.

		These low-level FTM operations are called and the associated events are verifed:
			CHANNEL_ASSIGN_REQ
			CONFIG_LOOPBACK_TYPE_REQ
			

		\param iLoopbackType (uses FTM_GSM_BER_LoopbackType_Enum)
		\code
          0 = Disable Loopback = FTM_GSM_BER_Disable_Loopback
          1 = Loopback Type A  = FTM_GSM_BER_Loopback_Type_A
          2 = Loopback Type B  = FTM_GSM_BER_Loopback_Type_B
          3 = Loopback Type C  = FTM_GSM_BER_Loopback_Type_C
		\endcode
		\param iSlot = Slot - TCH slot:
		\code
          0 = Slot 0
          1 = Slot 1
          2 = Slot 2
          3 = Slot 3
          4 = Slot 4
          5 = Slot 5
          6 = Slot 6
          7 = Slot 7
		\endcode
		NOTE: NOTE When using a BCCH+TCH simultaneous configuration (required), not all slots 
		will be available for the TCH.

		\param iMode
		\code
          0 = SPEECH - FULL RATE = FTM_GSM_BER_SPEECH_FULL_RATE
          1 = SPEECH - HALF RATE = FTM_GSM_BER_SPEECH_HALF_RATE
          2 = SPEECH - ENHANCED FULL RATE = FTM_GSM_BER_SPEECH_ENHANCED_FULL_RATE
          3 = AMR - FULL RATE = FTM_GSM_BER_AMR_FULL_RATE
          4 = AMR - HALF RATE = FTM_GSM_BER_AMR_HALF_RATE
          5 = DATA - 14.4 KBPS FULL RATE = FTM_GSM_BER_DATA_14_4_KBPS_FULL_RATE
          6 = DATA - 9.6 KBPS FULL RATE FTM_GSM_BER_DATA_9_6_KBPS_FULL_RATE
		\endcode
			NOTE: Options 1-6 may not be supported by the FTM software.

	\param iStatus 
		\code
          0 = Success (DIAG_FTM_STATUS_SUCCESS)
          1 = Failure (DIAG_FTM_STATUS_FAIL)
		\endcode

	\return true if network session is seutp successful, false if there are any failures

	\warning The active log mask will be changed to log only log FTM2 log packets.
	         When the function is completed, the state of ASYNC logging will be enabled.

	*******************************************************************************/	
	QLIB_API unsigned char QLIB_FTM_GSM_BER_AssignTraffic( 
			HANDLE hResourceContext, unsigned short iLoopbackType, 
			unsigned char iSlot, unsigned char iMode );

	/******************************************************************************/
	/**
	GSM BER, 2.4 FTM configuration loopback type request
		This command calls CONFIG_LOOPBACK_TYPE_REQ and checks the return status.

		This used to set the UE to one of the supported loopback modes. 
		Currently the supported modes are Loopback A (TCH loop with signalling of 
		erased frames), Loopback B (TCH loop without signalling of erased frames), 
		and Loopback C (TCH burst by burst). The user should be able to change 
		loopback modes directly (example: Loopback C to Loopback A) at any time on 
		the TCH.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iLoopbackType (uses FTM_GSM_BER_LoopbackType_Enum)
		\code
          0 = Disable Loopback = FTM_GSM_BER_Disable_Loopback
          1 = Loopback Type A  = FTM_GSM_BER_Loopback_Type_A
          2 = Loopback Type B  = FTM_GSM_BER_Loopback_Type_B
          3 = Loopback Type C  = FTM_GSM_BER_Loopback_Type_C
		\endcode
	
	\return true if successful, false if fail.

	\warning The status return value in the FTM response packet will not be evaluated.  This is
	         because some early versions of embedded software were not initializing this status
			 response value.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GSM_ConfigLoopbackType( 
			HANDLE hResourceContext, unsigned short iLoopbackType );

	/******************************************************************************/
	/**
	GSM BER, perform a handover operation.  Before using this function, FTM_GSM_BER_StartCall() 
	should be completed successfully and if a CMU-200 is being used, the "connect" 
	command should be sent it.

	This command can e used in place of FTM_GSM_BER_AssignBCCH() and FTM_GSM_BER_AssignTraffic() 
	within the same session, in order to begin a traffic connection for the 1st time in a session.

	It can also be used to perform a handover to a new band, once the first channel of GSM non-signaling 
	traffic has been setup.

		These low-level FTM operations are called and the associated events are verifed:
			CHANNEL_ RECONFIGURE_REQ

	Note: even though the loopback type is a parameter, it is not used right now for optimization
	      reasons.  If a loopback is assigned before the handover it will be kept active after the
		  handover is completed.  QLIB_FTM_GSM_ConfigLoopbackType() can be used to control the
		  loopback separately from this handover command.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iChannel = RF channel. Valid range depends on band.
	\param iBand 
		\code
          0 = PGSM 900 = FTM_GSM_BER_PGSM_900
          1 = EGSM 900 = FTM_GSM_BER_EGSM_900 
          2 = PCS 1900 = FTM_GSM_BER_PCS_1900
          3 = DCS 1800 = FTM_GSM_BER_DCS_1800
          4 = CELL 850 = FTM_GSM_BER_CELL_850
		\endcode
	\param iTSC = Training Sequence number (0-7).  The same number is used for both UL and DL.
	\param iSlot = Slot - TCH slot:
		\code
          0 = Slot 0
          1 = Slot 1
          2 = Slot 2
          3 = Slot 3
          4 = Slot 4
          5 = Slot 5
          6 = Slot 6
          7 = Slot 7
		\endcode
		NOTE: NOTE When using a BCCH+TCH simultaneous configuration (required), not all slots 
		will be available for the TCH.

	\param iMode
		\code
          0 = SPEECH - FULL RATE = FTM_GSM_BER_SPEECH_FULL_RATE
          1 = SPEECH - HALF RATE = FTM_GSM_BER_SPEECH_HALF_RATE
          2 = SPEECH - ENHANCED FULL RATE = FTM_GSM_BER_SPEECH_ENHANCED_FULL_RATE
          3 = AMR - FULL RATE = FTM_GSM_BER_AMR_FULL_RATE
          4 = AMR - HALF RATE = FTM_GSM_BER_AMR_HALF_RATE
          5 = DATA - 14.4 KBPS FULL RATE = FTM_GSM_BER_DATA_14_4_KBPS_FULL_RATE
          6 = DATA - 9.6 KBPS FULL RATE FTM_GSM_BER_DATA_9_6_KBPS_FULL_RATE
		\endcode
		NOTE: Options 1-6 may not be supported by the FTM software.

	\param iPowerLevel = the new PCL to set the Tx level to
		\param iLoopbackType (uses FTM_GSM_BER_LoopbackType_Enum)
			\code
			  0 = Disable Loopback = FTM_GSM_BER_Disable_Loopback
			  1 = Loopback Type A  = FTM_GSM_BER_Loopback_Type_A
			  2 = Loopback Type B  = FTM_GSM_BER_Loopback_Type_B
			  3 = Loopback Type C  = FTM_GSM_BER_Loopback_Type_C
			\endcode

	\param iStatus 
		\code
          0 = Success (DIAG_FTM_STATUS_SUCCESS)
          1 = Failure (DIAG_FTM_STATUS_FAIL)
		\endcode

	\return true if all operations succeeded and the correct event messages were received

	\warning The active log mask will be changed to log only log FTM2 log packets.
	         When the function is completed, the state of ASYNC logging will be enabled.

	*******************************************************************************/	
	QLIB_API unsigned char QLIB_FTM_GSM_BER_Handover( 
			HANDLE hResourceContext,
			unsigned short iChannel, unsigned char iBand, unsigned char iTSC, unsigned char iSlot, 
			unsigned char iMode, unsigned char iPowerLevel, 
			unsigned char  iLoopbackType
			);



	/******************************************************************************/
	/**
	GSM BER, Clear the internal status of the GSM BER structure

		Resets the contents of all fields in the internal GSM_BER_State state structure.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GSM_BER_ClearStatus( HANDLE hResourceContext );


	/******************************************************************************/
	/**
	GSM BER, Returns a copy of the internal GSM BER state. On the internal copy
	         it clears the "updated" fields for each message type so that the
			 next time this funciton is called. Only items that been received
			 since the first function call are marked as "updated."
	         
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param pGSM_BER_State = reference to a GSM_BER_State structure that will be  filled in with 
	                        the state information

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GSM_BER_GetStatus( HANDLE hResourceContext, unsigned char* pGSM_BER_State );

	/******************************************************************************/
	/**
	GSM BER, Returns a simplified status of whether a specified event has occured.

	The function operations upon the bHasUpdated array held in the GSM BER status
	structure (data type GSM_BER_State, as defined in QLib_Defines.h).  
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param eEventID = The input parameter is an index into this array, and the function
	                   returns a true if the event has occured and false if it has not.

		Data type is log_FTM2_GSM_BER_log_id_enum, defined in QLib_Defines.h

		Values are:
		\code
          LOG_FTM2_START_GSM_MODE_CNF       = 0 = Start GSM confirm
          LOG_FTM2_SELECT_SPECIFIC_BCCH_CNF = 1 = FTM GSM BER select specific BCCH confirmation
          LOG_FTM2_START_IDLE_MODE_CNF      = 2 = FTM GSM BER start idle mode confirmation
          LOG_FTM2_CHANNEL_ASSIGN_CNF       = 3 = FTM GSM BER channel assign confirmation
          LOG_FTM2_CHANNEL_RELEASE_CNF      = 4 = FTM GSM BER channel release confirmation
          LOG_FTM2_STOP_GSM_MODE_CNF        = 5 = FTM GSM BER stop GSM mode confirmation
          LOG_FTM2_PH_DATA_IND              = 6 = FTM GSM BER Ph data indication
          LOG_FTM2_FTM_LOG_GSM_BER_DED_RPT  = 7 = FTM GSM BER dedicated measurement report
		\endcode
	         

	\param pGSM_BER_State = reference to a GSM_BER_State structure that will be 
	                        filled in with the state information

	\return true the event type has occurred, false if it has not.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GSM_BER_GetEventStatus( HANDLE hResourceContext, unsigned char eEventID );

	/******************************************************************************/
	/**
	GSM BER, Returns the current Layer 1, RX Level.  
	         
	This function will wait until the next FTM_LOG_GSM_BER_DED_RPT message arrives,
	then return the status of that message.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piDED_RPtRxLev_Full = RxLev_Full as reported by Layer 1
	\param piDED_RPtRxQual_Full = RxQual_Full as reported by Layer 1


	More information about RX_QUAL:

	In brief, the mobile measures the Rx qual as defined in 3GPP 45.08.

	There are two rx qual thresholds calculated namely rx qual full and rx qual sub on the downlink 
	radio bursts. These two measure the radio's downlink quality over a single reporting period and 
	report the measurements to the network ( measurement reporting period is104 frames for tch channel) 
	so that the network has some information as to 'how the mobile see's the downlink radio quality'.

	Rx qual full is where the mobile makes quality threshold measurements on all downlink burst, ie tch, 
	facch and sacch blocks utilizing burst information from the equalizer output for all 104 bursts 
	on the downlink.

	Rx Qual Sub is a sub set of these 104 bursts. This is where the mobile only uses for quality 
	measurements the SACCH blocks and the Silent Identifier (SID) frames. This is the scenario where 
	there is no speech or signaling frames on the downlink to measure the downlink quality. The mobile 
	will always have information in these frames as the network will always send SACCH frames and if no 
	signaling or speech frames to transmit then it sends SID frames at specific radio blocks instead of 
	signaling or speech frames, these frames are identified as SID frames. This Rx Qual sub is therefore 
	an accurate measure of the quality of the downlink on a subset of the 104 burst.

	Therefore in a measurement report from the mobile to the network, the rx qual full might be 7 
	(ie poor) and the rx sub might be 0 (ie good) when no signaling/speech information is on the downlink.

	PRBS would imply signal being generated all the time and hence the rx qual full would be 0, ie a 
	full set of downlink bursts to process.

	ECHO would imply some silence in the downlink and hence the rx qual full would be 7 as 
	there is poor data to measure quality on 

	\return true if all operations succeeded and the correct event messages were received

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GSM_BER_GetRxMetrics( 
		HANDLE hResourceContext, unsigned char* piDED_RPtRxLev_Full, unsigned char* piDED_RPtRxQual_Full );

	/******************************************************************************/
	/**
	GSM BER, Returns the status of the GSM traffic channel
	         
	Use this function after GSM TCH has been assigned.  
	
	Will return true if the last PH_DATA_IND message for the SACCH type had a valid CRC_PASS
	AND if the SACCH accumulator is > 0.

	The SACCH accumulator begins at a value of 40 when QLIB_FTM_GSM_BER_ClearStatus() is
	called.  It is then decremented by 1 if an invalid SACCH CRC is reported and incremented
	by 2 for each valid SACCH CRC.  The accumulator will saturate at 0 on the low end
	and 40 on the high end.

	The user can change the maximum value (normally 40) by calling QLIB_FTM_GSM_BER_SetSACCH_AccumulatorMax()

	This specification for this logic comes from the 3GPP document 45.008, in the 
	"radio link failure" section.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return pass/fail according to the rules above.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GSM_BER_IsTrafficValid(	HANDLE hResourceContext );

	/******************************************************************************/
	/**
	GSM BER, Sets the maximum value for the SACCH accumulator.
	         
	See the description for QLIB_FTM_GSM_BER_IsTrafficValid() for how the accumulator
	works
	
	Default value in the system is 40.  This function does not need to be called unless
	the user desires a value other than 40.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSACCH_AccumulatorMax = new SACCH accumulator maximum value to be used for QLIB_FTM_GSM_BER_IsTrafficValid()

	\return true if successful, false if failure

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GSM_BER_SetSACCH_AccumulatorMax( HANDLE hResourceContext, short iSACCH_AccumulatorMax );

	/******************************************************************************/
	/**
	GSM BER, Release the GSM channel and optionally stop a GSM Non-signaling session.

		This low-level FTM operation is called and the associated event are verifed:
			CHANNEL_RELEASE_REQ

	\return true if all operations succeeded and the correct event messages were received

	\warning If further GSM BER sessions will occur before the phone is reset, then
	         the traffic loopback should be disabled using QLIB_FTM_GSM_ConfigLoopbackType()

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GSM_BER_ReleaseTCH( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	GSM BER, close the GSM Non-signaling session.
	
		This low-level FTM operation is called and the associated event are verifed:
			STOP_GSM_MODE_REQ

		Note: this command does not wait for the asynchronous stop responds because
		      some versions of software do not have this enabled and the response
			  is low priority for this commands, compared to other commands such
			  as traffice assign and handover.

	                 
	\return true if all operations succeeded and the correct event messages were received

	\warning 1. This function will disable async logging and clear the async queue.
	         2. If further GSM BER sessions will occur before the phone is reset, then
	            the traffic loopback should be disabled using QLIB_FTM_GSM_ConfigLoopbackType()


	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_GSM_BER_StopSession( HANDLE hResourceContext );

/******************************************************************************
						High level EGPRS BER FTM
*******************************************************************************/

	/******************************************************************************/
	/**
	The only difference	between this function and FTM_GSM_BER_AssignBCCH is that 
	this function goes into EGRPS IDLE mode instead of GSM Idle mode.  
	
    Before this function is called, FTM_GSM_BER_StartSession
	must be called.  When the function is completed, the phone will be in the IDLE
	state.

	This function must be called before FTM_EGPRS_BER_StartIdleMode

		These low-level FTM operations are called and the associated events are verifed:
			SELECT_SPECIFIC_BCCH_REQ
			FTM_EGPRS_BER_START_GPRS_IDLE_MODE_REQ

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iChannel = RF channel. Valid range depends on band.
		\param iBand 
			\code
			  0 = PGSM 900 = FTM_GSM_BER_PGSM_900
			  1 = EGSM 900 = FTM_GSM_BER_EGSM_900 
			  2 = PCS 1900 = FTM_GSM_BER_PCS_1900
			  3 = DCS 1800 = FTM_GSM_BER_DCS_1800
			  4 = CELL 850 = FTM_GSM_BER_CELL_850
			\endcode

	\return true if network session is setup successful, false if there are any failures

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EGPRS_BER_AssignBCCH( 
		HANDLE hResourceContext, unsigned short iChannel, unsigned char iBand );

	/******************************************************************************/
	/**
	EGPRS BER, Start a EGPRS BER session by putting the phone into EGPRS Idle mode. 
	Before this function is called, a valid	GSM BER session must be active.

		These low-level FTM operations are called and the associated events are verifed:
			FTM_EGPRS_BER_START_GPRS_IDLE_MODE_REQ

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if network session is setup successful, false if there are any failures

	\warning The active log mask will be changed to log only log FTM2 log packets.
	         When the function is completed, the state of ASYNC logging will be enabled.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EGPRS_BER_StartIdleMode( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	High level EGPRS BER, calls FTM_EGPRS_BER_CONFIGURE_DL_TBF, and checks the status of
	the command response.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iDL_TSC - Training sequence code for use on DL PDCH
	\param iDL_TFI - Expected temporary flow ID for use on DL PDCH. During the
	                SRB loop the UE actually ignores the received TFI, so this can
	                be fixed to 0.
	\param iChannel - Channel number for both downlink and uplink TBFs. Valid
	                  range depends on band.
	\param iBand - Band information for both downlink and uplink TBFs
		\code
           0 = PGSM 900
           1 = EGSM 900
           2 = PCS 1900
           3 = DCS 1800
           4 = CELL 850
		\endcode


	\param bFirstSlotActive - First active slot flag. unsigned charean to indicate if the timeslot configuration is valid. 
	                          This should always be TRUE, since the feature currently supports	only one DL slot.

	\param iFirstSlotNum - First active timeslot number - Valid range is 0-7. This indicates the timeslot of the DL PDCH,
	                             and will only be read if first active slot flag is TRUE.

	\param bSecondSlotActive - Second active slot flag - unsigned charean to indicate if the timeslot configuration is valid. This
	                           should always be FALSE, since the feature does not currently
	                           support a 2 DL configuration.
	\param bSecondSlotNum - Second active timeslot number - Valid range is 0-7. This indicates the timeslot of the DL PDCH,
	                        and will only be read if second active slot flag is TRUE.

	\return true if successful, false if fail.

	\warning Not implemented 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EGPRS_BER_Configure_DL_TBF( 
				HANDLE hResourceContext, 
				unsigned char iDL_TSC, 
				unsigned char iDL_TFI,
				unsigned short iChannel,
				unsigned char iBand,
				unsigned char bFirstSlotActive, 
				unsigned char iFirstSlotNum,
				unsigned char bSecondSlotActive,
				unsigned char iSecondSlotNum
				);


	/******************************************************************************/
	/**
	High level EGPRS BER, calls FTM_EGPRS_BER_CONFIGURE_UL_TBF, and checks the status of
	the command response.

	This command sends the configuration for the UL TBF to the UE. The embedded software will
	store this configuration for use when the SRB loop is established.

  	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iUL_TSC - Training sequence code for use on UL PDCH

	\param iUSF - Uplink state flag assigned to the UE. During the SRB loop the
	              UE actually ignores the received USF, so this can be set to 0.

	\param iGamma - GAMMA_TN value.  It is a basically sets the attenuation level relative to max power
	                for a band, in unts of 2dB.

				   Valid range is 0-31. This maps to the GAMMA_TN value in
	               3GPP TS 44.060. This sets the uplink power level based on the
                   equation given in 3GPP TS 45.008, section 10.2. The embedded software
                   uses an gamma parameter of 0.

                   The GAMMA_TN field is the binary representation of the parameter *CH for MS output 
				   power control in units of 2 dB, see 3GPP TS 45.008. The GAMMA_TN field is coded 
				   according to the following table:
		\code
         bit
         5 4 3 2 1
         0 0 0 0 0	*CH = 0 dB
         0 0 0 0 1	*CH = 2 dB
         : : : :
         1 1 1 1 0	*CH = 60 dB
         1 1 1 1 1	*CH = 62 dB
		\endcode

	\param bFirstSlotActive - First active slot flag. unsigned charean to indicate if the timeslot configuration is valid. 
	                          This should always be TRUE, since the feature currently supports	only one DL slot.

	\param iFirstSlotNum - First active timeslot number - Valid range is 0-7. This indicates the timeslot of the DL PDCH,
	                             and will only be read if first active slot flag is TRUE.

	\param bSecondSlotActive - Second active slot flag - unsigned charean to indicate if the timeslot configuration is valid. 
	                           This should always be FALSE, since the feature does not currently
	                           support a 2 DL configuration.
	\param bSecondSlotNum - Second active timeslot number - Valid range is 0-7. This indicates the timeslot of the DL PDCH,
	                        and will only be read if second active slot flag is TRUE.

	\return true if successful, false if fail.

	\warning Not implemented 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EGPRS_BER_Configure_UL_TBF(
				HANDLE hResourceContext, 
				unsigned char iUL_TSC, 
				unsigned char iUSF,
				unsigned char iGamma,
				unsigned char bFirstSlotActive, 
				unsigned char iFirstSlotNum,
				unsigned char bSecondSlotActive,
				unsigned char iSecondSlotNum
				);


	/******************************************************************************/
	/**
	High level EGPRS BER, calls FTM_EGPRS_BER_ESTABLISH_SRB_LOOPBACK, and handles all
	related events.

	After the DL and UL TBFs have been configured, the user should issue this command to establish
	the TBFs and close the SRB loop.

	Also calls QLIB_FTM_EGPRS_BER_ClearStatus() to clear all EGPRS status.

  	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if fail.

	\warning Not implemented 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EGPRS_BER_Establish_SRB_LOOPBACK( HANDLE hResourceContext );


	/******************************************************************************/
	/**
	High level EGPRS BER, calls FTM_EGPRS_BER_RELEASE_ALL_TBF, and handles all
	related events.

	This command directs the UE to release the UL and DL TBFs. This should only be issued when
	the UE is connected to the PDCH.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if fail.

	\warning Not implemented 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EGPRS_BER_Release_All_TBF( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	EGPRS BER, Clear the internal status of the EGPRS BER structure

		Resets the contents of all fields in the internal EGPRS_BER_State state structure.

  	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if all operations succeeded and the correct event messages were received

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EGPRS_BER_ClearStatus( HANDLE hResourceContext );


	/******************************************************************************/
	/**
	EGPRS BER, Returns a copy of the internal EGPRS BER state. On the internal copy
	         it clears the "updated" fields for each message type so that the
			 next time this funciton is called. Only items that been received
			 since the first function call are marked as "updated."
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param pEGPRS_BER_State = reference to a EGPRS_BER_State structure that will be 
	                        filled in with the state information.  
							The structure type pointed to is EGPRS_BER_State

	\return true if all operations succeeded and the correct event messages were received

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EGPRS_BER_GetStatus(HANDLE hResourceContext, unsigned char* pEGPRS_BER_State );

	/******************************************************************************/
	/**
	EGPRS BER, Returns the current Layer 1, RX Level.  
	         
	This function will wait until the next FTM_LOG_EGPRS_BER_DED_RPT message arrives,
	then return the status of that message.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iRxLev_average = RxLev_average as reported by Layer 1. Range should be 0-63.

	\return true if all operations succeeded and the correct event messages were received

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_EGPRS_BER_GetRxMetrics( HANDLE hResourceContext, unsigned char* piRxLev_average );

/******************************************************************************
					GSDI Diag
*******************************************************************************/

	/******************************************************************************/
	/**
	GDSI ICD, 4.16  DELAYED RESPONSE: GSDIDIAG_ROOT_KEY_WRITE_CMD

	This function is used to write the root key to the FLASH.

	This function handles the delayed response message and the return values are 
	taken from delayed asynchronous message, not the first response message.

	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotID = Input, SLOT_ID - Slot number for the SIM for which this information 
	                 needs retrieved. Values applicable to dual slot targets are:
		\code
            0x01 - SLOT 1
            0x02 - SLOT 2
		\endcode
		For single slot targets, slot number should be None.

	\param iSIM_AppIdLen = Input, Length of the Application ID from which to retrieve information; 
	                      set to Length = 0 when not used. Be sure to set this to 0 when not
                          used. Optional and applicable only to USIM. For a USIM, this implies 
						  that the USIM ADF is to be selected by default as necessary.
						  Note: When using GSM cards and RUIM cards, set this to 0.

	\param piSIM_AppId = Input, Optional and applicable only to USIM.
					   USIM application identifier (APP ID); should only be used when the
					   Card is a UICC with application directory files (ADFs)
					   The APP IDs are retrieved from the USIM file EF-DIR.
					   Set bytes [1 to 16] to 0x00 for GSM cards and RUIM cards.

					   If this is not used, then set value to NULL, otherwise, must be 16 bytes long.
					

  	\param iRootKeyLen = Input, # of bytes used by the root key, 
	\param piRootKeyData = Input, storage area of the root key, must be a valid pointer to 
	                       "iRootKeyLen" number of bytes.

	\param piCommandStatus = Output, Command status is set to GSDI_SUCCESS if APS_AS_TRANSPORT is 
	                        set to 1.Otherwise it is set to the status returned by GSDI

							This value is not valid if the function returns a false.

	
	\param piStatus = Output, status code returned by the delayed response GSDI. 
                     These are the codes returned in the CDM_STATUS fields of Events.

                     This value is not valid if the function returns a false.
	\code 
         0	- COMMAND COMPLETED SUCCESSFULLY
         1	- GENERAL ERROR:  No other information specified
         2	- ACCESS DENIED:  Security Procedures performed thus far don't satisfy restrictions
         3	- NOT FOUND:  SIM Elementary File or Directory File was not found.
         4	- INCOMPATIBLE PIN STATUS:  Trying to perform a PIN Action for a PIN that is not in the correct state.  
		      For example, trying to Disable an already Disabled PIN.
         5	- INCORRECT CODE:  The Code provided is incorrect for the PIN ID used.
         6	- CODE BLOCKED:  The Number of retries has been exceeded and the PIN is now blocked.
         7	- INCREASE IMPOSSIBLE:  The INCREASE Command has failed due to a Bad Value
         8	- INCORRECT PARAMETERS:  The SIM detected an incorrect parameter for the action performed.
         9	- NOT SUPPORTED:  Not allowed by GSDI due to Service Table Values
         10	- NOT INIT:  Cache has not been INIT
         11	- SUCCESS BUT ILLEGAL SIM:  The SIM has been marked as an illegal SIM for the network, 
		      but the actions are succeeding on the SIM.
         12	- RESERVED
         13	- RESERVED
         14	- SIM TECHNICAL PROBLEMS:  The SIM malfunctioned.
         15	- NO EF SELECTED:  Attempted to read a file without fist selecting it.
         16	- EF INCONSISTENT:  Trying to Read a Record when the file is a Binary or similar type error.
         17	- RESERVED
         18	- UNKNOWN INST CLASS:  Trying to send a UICC Command to a GSM/RUIM Card or vice versa.
         19	- RESERVED
         20	- RESERVED
         21	- INCORRECT LENGTH:  A Bad length was provided in the command.
         22	- DRIVERS TIMED OUT:  The Drivers communicating with the Card Timed Out.
         23	- CODE PERM BLOCKED:  The Unblock Code has been blocked.  Need Administrative assistance to unblock it.
         24	- RESERVED
         25	- RESERVED
         26	- RESERVED
	\endcode
	

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GSDIDIAG_ROOT_KEY_WRITE_CMD
		( 
			HANDLE hResourceContext, 
			unsigned char iSlotId, 
			unsigned char iSIM_AppIdLen, 
			unsigned char * piSIM_AppId, 
			unsigned long iRootKeyLen, 
			unsigned char * piRootKeyData, 
			unsigned long* piCommandStatus, 
			unsigned long* piStatus
		);

	/******************************************************************************/
	/**
	GDSI ICD, 4.17  DELAYED RESPONSE: GSDIDIAG_ROOT_KEY_READ_CMD 

	This function is used to read the root key from the FLASH.

	This function handles the delayed response message and the return values are 
	taken from delayed asynchronous message, not the first response message.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotID = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param iSIM_AppIdLen = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param piSIM_AppId = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>

  	\param piRootKeyLen = Output, # of bytes used by the root key, 
	\param piRootKeyData = Output, storage area of the root key, must be a valid pointer to 
	                       at least 260 bytes.

	\param piCommandStatus = Output, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param piStatus = Output, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GSDIDIAG_ROOT_KEY_READ_CMD
		( 
			HANDLE hResourceContext, 
			unsigned char iSlotId, unsigned char iSIM_AppIdLen, 
			unsigned char* piSIM_AppId, 
			unsigned long* piRootKeyLen, 
			unsigned char* piRootKeyData, 
			unsigned long* piCommandStatus, 
			unsigned long* piStatus
		);

	/******************************************************************************/
	/**
	GDSI ICD, 1.2.17  DELAYED RESPONSE: GSDIDIAG_SECURE_DEVICE_INFO_CMD

	This function is used to save IMEI, Serial Number to the Flash and perform 
	the activation procedure.

	This function handles the delayed response message and the return values are 
	taken from delayed asynchronous message, not the first response message.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotID = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param iSIM_AppIdLen = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param piSIM_AppId = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>

	\param piCommandStatus = Output, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param piStatus = Output, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GSDIDIAG_SECURE_DEVICE_INFO_CMD
		( 
			HANDLE hResourceContext, 
			unsigned char iSlotId, 
			unsigned char iSIM_AppIdLen, 
			unsigned char* piSIM_AppId, 
			unsigned long* piCommandStatus, 
			unsigned long* piStatus
		);

	/******************************************************************************/
	/**
	GDSI ICD, 4.15  DELAYED RESPONSE: GSDIDIAG_SIM_SECURE_STORE_CMD

	This function is used to store encrypted SIM lock data.

	This function handles the delayed response message and the return values are 
	taken from delayed asynchronous message, not the first response message.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotID = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param iSIM_AppIdLen = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param piSIM_AppId = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param iSIM_SecureDataLen = input, size of sim secure buffer.  At most 1360 bytes
	\param piSIM_SecureData = input, buffer of SIM secure data, must be a minimum of 
	                          "iSIM_SecureDataLen" bytes, at most 1360 bytes

	\param piCommandStatus = Output, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param piStatus = Output, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GSDIDIAG_SIM_SECURE_STORE_CMD
		( 
			HANDLE hResourceContext, 
			unsigned char iSlotId, 
			unsigned char iSIM_AppIdLen, 
			unsigned char* piSIM_AppId, 
			unsigned long iSIM_SecureDataLen, 
			unsigned char* piSIM_SecureData, 
			unsigned long* piCommandStatus, 
			unsigned long* piStatus
		);

	/******************************************************************************/
	/**
	GDSI ICD, 4.18.2 GSDIDIAG_SECURE_READ_CMD response

	This function is used to read the encrypted SIM lock data.

	This function handles the delayed response message and the return values are 
	taken from delayed asynchronous message, not the first response message.
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotID = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param iSIM_AppIdLen = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param piSIM_AppId = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>

	\param piSIM_SecureDataLen = output, return value of the number of bytes in the SIM secure data
	\param piSIM_SecureData = output, buffer to store the contents of the SIM secure data, at most 1360 bytes.

	\param piCommandStatus = Output, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param piStatus = Output, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\return true if successful, false if fail.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_GSDIDIAG_SIM_SECURE_READ_CMD
		(
			HANDLE hResourceContext, 
			unsigned char iSlotId, 
			unsigned char iSIM_AppIdLen, 
			unsigned char * piSIM_AppId, 
			unsigned long*piSIM_SecureDataLen, 
			unsigned char * piSIM_SecureData, 
			unsigned long* piCommandStatus, 
			unsigned long* piStatus
		);

	/******************************************************************************/
	/**
	GDSI ICD, 4.25  DELAYED RESPONSE: GSDIDIAG_ACTIVATE_ONCHIP_SIM_CMD

	ICD description:
	This function is used to activate Virtual SIM.

	This function handles the delayed response message and the return values are 
	taken from delayed asynchronous message, not the first response message.

	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotID = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param iSIM_AppIdLen = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param piSIM_AppId = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>

	\param piCommandStatus = Output, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param piStatus = Output, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\return true if successful, false if fail.

	\warning The SPC code must be sent to the phone first, using QLIB_DIAG_SPC_F()

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GSDIDIAG_ACTIVATE_ONCHIP_SIM_CMD
		( 
			HANDLE hResourceContext, 
			unsigned char iSlotId, 
			unsigned char iSIM_AppIdLen, 
			unsigned char* piSIM_AppId, 
			unsigned long* piCommandStatus, 
			unsigned long* piStatus
		);

	/******************************************************************************/
	/**
	MMGDSI ICD, TBD Section,  DELAYED RESPONSE: MMGSDIDIAG_ACTIVATE_ONCHIP_SIM_CMD

	ICD description:
	This function is used to activate Virtual SIM.  There is a difference between the 
	"GSDIDIAG" and "MMGSDIDIAG" commands.  The MMGSDI command also allows specifying
	the IMSI, Access Class, Admin data, LOCI, FPLMN, SST, and Phase.

	This function sends a fixed configuration on the data.  This is done because
	the ICD definition was not completed at the time of the function writing and
	it is a standard configuration, which can be used to make a call on most call processing
	equipment

	Then send the Activate ONCHIP SIM Data.
	\code
      send_data 0x80 0x21 0x48 0x00 0x01 0x00 0x00 0x00 0x83 0x00 0x00 0x00 0x01 0x10 0x00 0x00 0x00 0x00 0x00 
                0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0xbc 0xda 0xcd 0xab 0xcd 0xab 0xba 0xdc  
				0x30 0x00 0x00 0x00 0x00 0x2D 0x00 0x02 0x09 0x08 0x09 0x10 0x10 0x10 0x32 0x54 0x76 0x98 0x07 
				0x02 0xFB 0xF8 0x0A 0x04 0x81 0x00 0x01 0x02 0x03 0x0B 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0x00 
				0x00 0xFF 0x01 0x08 0x03 0xFF 0xFF 0xFF 0x06 0x04 0xFF 0xFF 0xFF 0xFF 0x09 0x01 0x03
      About the data:

      0x30 0x00 0x00 0x00           : Total length of the Activate ONCHIP SIM Payload data
      0x2D 0x00                     : Total Length of the TLV Data
      0x02 0x09 0x08 0x09 0x10 0x10 0x10 0x32 0x54 0x76 0x98               :  IMSI:  0x001 01 0123456789
      0x07 0x02 0xFB 0xF8           :  Access Class:  
      0x0A 0x04 0x81 0x00 01 02     :  Administrative Data: 
      0x03 0x0B 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0x0x00 0x0x00 0xFF 0x01 :  LOCI
      0x08 0x03 0xFF 0xFF 0xFF      :  FPLMN
      0x06 0x04 0xFF 0xFF 0xFF 0xFF :  SST 
      0x09 0x01 0x03                :  Phase

	\endcode

	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\return true if successful, false if fail.

	\warning The SPC code must be sent to the phone first, using QLIB_DIAG_SPC_F()

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MMGSDIDIAG_ACTIVATE_ONCHIP_SIM_CMD_FixedConfig
		( 
			HANDLE hResourceContext
		);

	/*
		GSDI commands with Event response.

		This group of commands has response packets which are in the form of "event" 
		responses, rather than "delayed" responses.  The difference has to do with the 
		type of packet that contains the response information.

		For this group, the process of getting response data is different from
		the delayed response types.  In addition to calling the GSDI request command,
		the function QLIB_GSDIDIAG_GetEventResponse() must be called to get the request
		response information.

		An example of this can be seen in QLibDemo.cpp, in the function Test_Security(),
		which calls QLIB_GSDIDIAG_GET_FEATURE_INDICATION_CMD()

		If the request command is not directly available in QMSL, it is possible to send
		the request directly using QLIB_GSDIDIAG_SendRequestWithEventResponse().  Then
		any command can be requested and the response can be captured with 
		QLIB_GSDIDIAG_GetEventResponse().
	*/
	/******************************************************************************/
	/**

	Function to send any user defined "event response GSDI function" 
	This funtion will enable the event capturing and prepare the proper header
	for the request packet:
		CMD_CODE
		SUBSYS_CODE
		SUBSYS_CMD_CODE
		SIM_SLOT_NUMBER

	The user must supply the remaining request parameters in the data buffer, pUserRequestData

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSUBSYS_CMD_CODE = GSDI command number, to be used for the field, SUBSYS_CMD_CODE, refer to
	                          enumeration, GSDI_DIAG_CMD_Enum.
	\param iSlotID = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param pUserRequestData = all data after the SIM_SLOT_NUMBER field, NULL if there are no parameters
	\param iUserRequestDataSize = number of bytes to be sent in pUserRequestData, 0 if there are no parameters
	\param piTransactionID = Output, returned in order to identify the related event
	\param piCommandStatus = Output, result of CMD_STATUS field, if not GSDI_SUCCESS then the function returns a false.	

	\return true if a valid event was found, matching the transaction ID and the event 
			which were specified in iTransactionID and iEventID.


	\warning Before this function, one of the specific GSDI requests should be made 
			(e.g. GSDIDIAG_VERIFY_PIN_CMD), or the generic event-based GSDI request
			should be made QLIB_GSDIDIAG_SendRequestWithEventResponse()
	


	*******************************************************************************/
	QLIB_API unsigned char QLIB_GSDIDIAG_SendUserDefineEventResponseCommand(
			HANDLE hResourceContext, 
			unsigned short iSUBSYS_CMD_CODE,
			unsigned char iSlotId, 
			void* pUserRequestData,
			unsigned short iUserRequestDataSize,
			unsigned short* piTransactionID,
			unsigned long* piCommandStatus
			);

	/******************************************************************************/
	/**

	Function used to get the event response for all GSDI functions which result
	in event responses.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iTransactionID = Input, the transaction ID returned by the request
	\param iEventID = Input, the event ID to wait for, e.g. EVENT_GSDI_GET_FEATURE_IND. For 
	                  a wildcard event type, use QMSL_EVENT_ID_WILDCARD and any event will be
					  returned
	\param pEvent = a pointer to an event structure, QMSL_Event_Element_Struct.

					They "payload" field of this structure maps to a GSDIDIAG_EventHeader structure.


	\return true if a valid event was found, matching the transaction ID and the event 
			which were specified in iTransactionID and iEventID.


	\warning Before this function, one of the specific GSDI requests should be made 
			(e.g. GSDIDIAG_VERIFY_PIN_CMD), or the generic event-based GSDI request
			should be made QLIB_GSDIDIAG_SendRequestWithEventResponse()
	


	*******************************************************************************/
	QLIB_API unsigned char QLIB_GSDIDIAG_GetEventResponse(
			HANDLE hResourceContext, 
			unsigned short iTransactionID,
			unsigned short iEventID,
			void* pEvent
			);

	/******************************************************************************/
	/**
	GDSI ICD, 2.6 GSDIDIAG_VERIFY_PIN_CMD 

	ICD description:
		This command is used to verify the requested PIN. Currently used to verify PIN1/CHV1 or
		PIN2/CHV2. When the proper ID is supplied, it can be used to verify other PINs.
		This command will not need to Read/Write to EFS. Upon completion of this command, the result
		in EVENT_GSDI_SIM_VERIFY_PIN will send a status as well as the number of retries
		remaining.

	Related script command:  SIMVerifyPin
	
	  The QMSL function will currently >>not<< capture and process the event that results from this
	  function call.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotID = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param iSIM_AppIdLen = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param piSIM_AppId = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param iSIM_PIN_ID = input, 
	  \code
       - 0x00 = PIN1 (SIM/RUIM) PIN1 Application 1 (USIM)
       - 0x01 = PIN2 (SIM/RUIM) PIN1 Application 2 (USIM)
       - 0x02 = PIN1 Application 3 (USIM)
       - 0x03 = PIN1 Application 4 (USIM)
       - 0x04 = PIN1 Application 5 (USIM)
       - 0x05 = PIN1 Application 6 (USIM)
       - 0x06 = PIN1 Application 7 (USIM)
       - 0x07 = PIN1 Application 8 (USIM)
       - 0x0A = ADM1
       - 0x0B = ADM2
       - 0x0C = ADM3
       - 0x0D = ADM4
       - 0x0E = ADM5
       - 0x11 = UNIVERSAL PIN
       - 0x81 = PIN2 Application 1 (USIM)
       - 0x82 = PIN2 Application 2 (USIM)
       - 0x83 = PIN2 Application 3 (USIM)
       - 0x84 = PIN2 Application 4 (USIM)
       - 0x85 = PIN2 Application 5 (USIM)
       - 0x86 = PIN2 Application 6 (USIM)
       - 0x87 = PIN2 Application 7 (USIM)
       - 0x88 = PIN2 Application 8 (USIM)
       - 0x8A = ADM6
       - 0x8B = ADM7
       - 0x8C = ADM8
       - 0x8D = ADM9
       - 0x8E = ADM10-
	 \endcode
	 \param sSIM_PIN = input, NULL terminated string containing pin
	\param piTransactionID = Output, returned in order to identify the related event
	\param piCommandStatus = Output, result of CMD_STATUS field, if not GSDI_SUCCESS then the function returns a false.	

	\return true if successful, false if fail.

	\warning Will activate event messages and clear the current event queue

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GSDIDIAG_VERIFY_PIN_CMD
		( 
			HANDLE hResourceContext, 
			unsigned char iSlotId, 
			unsigned char iSIM_AppIdLen, 
			unsigned char* piSIM_AppId, 
			unsigned char iSIM_PIN_ID, 
			char* sSIM_PIN,
			unsigned short* piTransactionID,
			unsigned long* piCommandStatus
		);

	/******************************************************************************/
	/**
	GDSI ICD, 2.3 GSDIDIAG_SIM_READ_CMD

	ICD description:
		This is used to retrieve information from a Cyclic/Binary/Record Elementary Files from SIM.
		The data retrieved will be in a raw and unparsed format. It is up to the user to parse the data per
		the following specification. All fields are mandatory unless they specifically indicate that they
		are optional.
		The command is processed and put onto the GSDI Task Command Queue. If the queuing of the
		command is successful, a response with a valid Transaction ID is provided. Check the Command
		Status and verify that it is non-zero before accepting the Transaction ID.
		At the completion of the Command Processing in the GSDI Task, an event is generated to notify
		the client that the contents of the request can now be accessed from the filename provided in the
		request.
		Disclaimer: The client must maintain the EFS filenames and ensure that the same filenames are
		not used without first retrieving the data.

	Related script command:  READ_BINARY
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotID = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param iSIM_AppIdLen = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param piSIM_AppId = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param iSIM_ElementaryFile = input, see enumeration defintion, GSDI_DIAG_SIM_ElementaryFile_Enum
	\param iSIM_SimpleReq = input, Represents if write parameters are user specified values, or
							the phone calculates appropriate values; also represents if the
							user has to deal with chunking:
	  \code
        0 = GSDIDIAG_SIM_SIMPLE_REQ_ONE_STEP, User specified values for SIM_INDEX_MODE, SIM_
            OFFSET, SIM_REC_NUM, SIM_DATA_LEN: user has to
            deal with chunking: for record based files, user has to
            specify SIM_REC_NUM
        1 = GSDIDIAG_SIM_SIMPLE_REQ_ALL_STEPS, Phone uses appropriate values; also returns the
            complete data in a file; user is transparent to chunking
	  \endcode
	\param iSIM_IndexMode = input, See GSDI_DIAG_IndexModes_Enum
	\param iSIM_Offset = Input, Range = 0 to 256, Set to 0 when an offset is not to be specified
	\param iSIM_RecNum = Input, Range = 0 to 500.  Set to 0 when a Record Number is not required; 
							this will be the case when the file is either Cyclical or Binary/Transparent
	\param iSIM_DataLen = Input, Number of bytes to write
	\param sEFS_Filename = Input, Range = Variable; must be Null Terminated. File on EFS that the read data is to be stored in

	\param piTransactionID = Output, returned in order to identify the related event
	\param piCommandStatus = Output, result of CMD_STATUS field, if not GSDI_SUCCESS then the function returns a false.	

	\return true if successful, false if fail.

	\warning Will activate event messages and clear the current event queue

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GSDIDIAG_SIM_READ_CMD
		( 
			HANDLE hResourceContext, 
			unsigned char iSlotId, 
			unsigned char iSIM_AppIdLen, 
			unsigned char* piSIM_AppId, 
			unsigned long iSIM_Elementary_File,
			unsigned char iSIM_SimpleReq, 
			unsigned char iSIM_IndexMode,
			unsigned short iSIM_Offset, 
			unsigned short iSIM_RecNum, 
			unsigned short iSIM_DataLen,
			char* sEFS_Filename,
			unsigned short* piTransactionID,
			unsigned long* piCommandStatus
		);

	/******************************************************************************/
	/**

	GDSI ICD, 2.3.4 GSDIDIAG_SIM_WRITE_CMD

	ICD description:
	This command is used to store information on a Binary/Record Elementary Files in the
	SIM/USIM/RUIM. It is important that all relevant CHV/PIN(s) are considered and that all PIN
	Verification Procedures are performed prior to attempting a write. Otherwise, the write request
	will fail.
	The Command will result in a write to the EFS filename provided. The interface will then
	retrieve this information and write it to the SIM. EVENT_GSDI_SIM_WRITE is generated as a
	result of completion of this command.

	Related script command:  WRITE_BINARY

	  The QMSL function will currently >>not<< capture and process the event that results from this
	  function call.
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotID = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param iSIM_AppIdLen = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param piSIM_AppId = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param iSIM_ElementaryFile = input, see enumeration defintion, GSDI_DIAG_SIM_ElementaryFile_Enum
	\param iSIM_SimpleReq = input, Represents if write parameters are user specified values, or
							the phone calculates appropriate values; also represents if the
							user has to deal with chunking:
	  \code
        0 = User specified values for SIM_INDEX_MODE, SIM_
            OFFSET, SIM_REC_NUM, SIM_DATA_LEN: user has to
            deal with chunking: for record based files, user has to
            specify SIM_REC_NUM
        1 = Phone uses appropriate values; also returns the
            complete data in a file; user is transparent to chunking
	  \endcode
	\param iSIM_IndexMode = input, See GSDI_DIAG_IndexModes_Enum
	\param iSIM_Offset = Input, Range = 0 to 256, Set to 0 when an offset is not to be specified
	\param iSIM_RecNum = Input, Range = 0 to 500.  Set to 0 when a Record Number is not required; 
							this will be the case when the file is either Cyclical or Binary/Transparent
	\param iSIM_DataLen = Input, Number of bytes to write
	\param sEFS_Filename = Input, Range = Variable; must be Null Terminated. File on EFS that the data is taken from.
	\param piTransactionID = Ouptut, returned in order to identify the related event
	\param piCommandStatus = Output, result of CMD_STATUS field, if not GSDI_SUCCESS then the function returns a false.	

	\return true if successful, false if fail.

	\warning Will activate event messages and clear the current event queue

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GSDIDIAG_SIM_WRITE_CMD
		( 
			HANDLE hResourceContext, 
			unsigned char iSlotId, 
			unsigned char iSIM_AppIdLen, 
			unsigned char* piSIM_AppId, 
			unsigned long iSIM_Elementary_File,
			unsigned char iSIM_SimpleReq, 
			unsigned char iSIM_IndexMode,
			unsigned short iSIM_Offset, 
			unsigned short iSIM_RecNum, 
			unsigned short iSIM_DataLen,
			char* sEFS_Filename,
			unsigned short* piTransactionID,
			unsigned long* piCommandStatus
		);

	/******************************************************************************/
	/**

	GDSI ICD, 2.4 GSDIDIAG_GET_FILE_ATTRIBUTES_CMD

	ICD description:
		This command, when called, will get an immediate response based on whether the command was
		successfully queued. The Command contents are as follows:
	
	Related script command:  GET_FILE_ATTR

 	  The QMSL function will currently >>not<< capture and process the event that results from this
	  function call.
 
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotID = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param iSIM_AppIdLen = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param piSIM_AppId = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param iSIM_ElementaryFile = input, see enumeration defintion, GSDI_DIAG_SIM_ElementaryFile_Enum

	\param sEFS_Filename = Input, Range = Variable; must be Null Terminated. File on EFS that the read data is to be stored in

	\param piTransactionID = Ouptut, returned in order to identify the related event
	\param piCommandStatus = Output, result of CMD_STATUS field, if not GSDI_SUCCESS then the function returns a false.	

	\return true if successful, false if fail.

	\warning Will activate event messages and clear the current event queue

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GSDIDIAG_GET_FILE_ATTRIBUTES_CMD
		( 
			HANDLE hResourceContext, 
			unsigned char iSlotId, 
			unsigned char iSIM_AppIdLen, 
			unsigned char* piSIM_AppId, 
			unsigned long iSIM_Elementary_File,
			char* sEFS_Filename, 
			unsigned short* piTransactionID,
			unsigned long* piCommandStatus
		);

	/******************************************************************************/
	/**

	GDSI ICD, 3.9 GSDIDIAG_ACTIVATE_FEATURE_INDICATOR_CMD 

	ICD description:
		This command is used to activate the personalization feature indicator in SIM Lock feature. All
		fields are mandatory unless they specifically indicate that they are optional.
		The command is processed and put onto the GSDI Task Command Queue. So long as the
		queuing of the command is successful, a response with a valid Transaction ID is provided. Check
		the Command Status and verify it is successful before accepting the Transaction ID.
		At the completion of the Command Processing in the GSDI Task, an event is generated to notify
		the client.

	Related script command:  SIMActivateFeature
	
 	  The QMSL function will currently >>not<< capture and process the event that results from this
	  function call.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotID = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param iSIM_PersoFeature = input, see description of GSDI_DIAG_PersoFeature_Enum
	\param sSIM_PersoKey = input, Personalization Key Value; Input is in string format, NULL terminated

  	\param piTransactionID = Ouptut, returned in order to identify the related event
	\param piCommandStatus = Output, result of CMD_STATUS field, if not GSDI_SUCCESS then the function returns a false.	

	\return true if successful, false if fail.

	\warning Will activate event messages and clear the current event queue

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GSDIDIAG_ACTIVATE_FEATURE_INDICATOR_CMD
		( 
			HANDLE hResourceContext, 
			unsigned char iSlotId, 
			unsigned char iSIM_PersoFeature,	
			char* sSIM_PersoKey,
			unsigned short* piTransactionID,
			unsigned long* piCommandStatus
		);

	/******************************************************************************/
	/**

	GDSI ICD, 3.10 GSDIDIAG_DEACTIVATE_FEATURE_INDICATOR_CMD 

	ICD description:
		This request message is used to deactivate the personalization feature indicator in SIM Lock
		feature. All fields are mandatory unless they specifically indicate that they are optional.
		The command is processed and put onto the GSDI Task Command Queue. So long as the
		queuing of the command is successful, a response with a valid Transaction ID is provided. Check
		the Command Status and verify it is successful before accepting the Transaction ID.
		At the completion of the Command Processing in the GSDI Task, an event is generated to notify
		the client.

	Related script command:  SIMDeactivateFeature

 	  The QMSL function will currently >>not<< capture and process the event that results from this
	  function call.
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotID = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param iSIM_PersoFeature = input, see description of GSDI_DIAG_PersoFeature_Enum
	\param sSIM_PersoKey = input, Personalization Key Value; Input is in string format, NULL terminated

	\param piTransactionID = Ouptut, returned in order to identify the related event
	\param piCommandStatus = Output, result of CMD_STATUS field, if not GSDI_SUCCESS then the function returns a false.	

	\return true if successful, false if fail.

	\warning Will activate event messages and clear the current event queue

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GSDIDIAG_DEACTIVATE_FEATURE_INDICATOR_CMD
		( 
			HANDLE hResourceContext, 
			unsigned char iSlotId, 
			unsigned char iSIM_PersoFeature,
			char* sSIM_PersoKey,
			unsigned short* piTransactionID,
			unsigned long* piCommandStatus
		);

	/******************************************************************************/
	/**

	GDSI ICD, 3.11 GSDIDIAG_GET_FEATURE_INDICATION_CMD

	ICD Description:
		This request message is used to get the various personalization feature indication with respect to
		SIM Lock feature. All fields are mandatory unless they specifically indicate that they are
		optional.
		The command is processed and put onto the GSDI Task Command Queue. So long as the
		queuing of the command is successful, a response with a valid Transaction ID is provided. Check
		the Command Status and verify it is successful before accepting the Transaction ID.
		At the completion of the Command Processing in the GSDI Task, an event is generated to notify
		the client.
		
	Related script command:  SIMGetFeatureIndicators

 	  The QMSL function will currently >>not<< capture and process the event that results from this
	  function call.
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotID = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>

	\param piTransactionID = Ouptut, returned in order to identify the related event
	\param piCommandStatus = Output, result of CMD_STATUS field, if not GSDI_SUCCESS then the function returns a false.	

	\return true if successful, false if fail.

	\warning Will activate event messages and clear the current event queue

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GSDIDIAG_GET_FEATURE_INDICATION_CMD
		( 
			HANDLE hResourceContext, 
			unsigned char iSlotId,
			unsigned short* piTransactionID,
			unsigned long* piCommandStatus
		);

	/******************************************************************************/
	/**

	GDSI ICD, 3.12 GSDIDIAG_SET_FEATURE_INDICATOR_DATA_CMD

	ICD Description:
		This request message is used to set the data for the corresponding personalization feature
		indicator that the mobile is required to compare to the SIM with. All fields are mandatory unless
		they specifically indicate that they are optional.
		The command is processed and put onto the GSDI Task Command Queue. So long as the
		queuing of the command is successfully, a response with a valid Transaction ID is provided.
		Check the Command Status and verify it is successful before accepting the Transaction ID.
		At the completion of the Command Processing in the GSDI Task, an event is generated to notify
		the client.	

	Related script command:  SIMSetFeatureData

  	  The QMSL function will currently >>not<< capture and process the event that results from this
	  function call.
 
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotID = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param iSIM_PersoFeature = input, see description of GSDI_DIAG_PersoFeature_Enum
	\param piSIM_PersoData = input, byte buffer
	\code
      Personalization Data Value. Input is in HEX
      representation + string format
      Formats of the data are:
      - Network personalization (multiple of 3 bytes)
	    As defined in IMSI, e.g.:
         - 44097(2 digits MNC) . >= 44F079
         - 440973 (3 digits MNC) >= 443079

      Network Subset personalization (multiple of 4 bytes) 
      As defined in Network personalization + 6 and 7 bits in
      IMSI, e.g.;
          - 4409712 (2 digits MNC) >= 44F07921
          - 44097312 (3 digits MNC) >= 44307921	   

      Service Provider personalization (multiple of 5 bytes) 
      MCC + MNC + bcd format of service provider code, e.g.:
        - MCC + MNC=44097, SP=6 >= 44F07960
        - MCC + MNC=440973, SP=12 >= 44307921

      Corporate personalization (multiple of 6 bytes) 
      Network personalization + Service provider
      personalization representation + Corporate code in
      BCD format, e.g.:
        - MCC + MNC = 44097, SP = 13, CP = 17, >= 44F0793171
        - SIM/USIM personalization

      When multiple data entries are made for each
      personalization level data, each entry should follow the
      format indicated above and there should be no space in
      between entries

	\endcode
	\param iSIM_PersoDataSize = size of piSIM_PersonData, in bytes
	\param piTransactionID = Ouptut, returned in order to identify the related event
	\param piCommandStatus = Output, result of CMD_STATUS field, if not GSDI_SUCCESS then the function returns a false.	

	\return true if successful, false if fail.

	\warning Will activate event messages and clear the current event queue

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GSDIDIAG_SET_FEATURE_INDICATOR_DATA_CMD
		( 
			HANDLE hResourceContext, 
			unsigned char iSlotId, 
			unsigned char iSIM_PersoFeature,	
			unsigned char* piSIM_PersoData, 
			unsigned short iSIM_PersoDataSize,
			unsigned short* piTransactionID,
			unsigned long* piCommandStatus
		);

	/******************************************************************************/
	/**

	GDSI ICD, 3.13 GSDIDIAG_GET_FEATURE_INDICATOR_KEY_CMD

	ICD Description:
		This request message is used to get the various personalization feature control key value with
		respect to SIM Lock feature. All fields are mandatory unless they specifically indicate that they
		are optional.
		The command is processed and put onto the GSDI Task Command Queue. So long as the
		queuing of the command is successful, a response with a valid Transaction ID is provided. Check
		the Command Status and verify it is successful before accepting the Transaction ID.
		At the completion of the Command Processing in the GSDI Task, an event is generated to notify
		the client.
	
	Related script command:  SIMGetFeatureKey

 	  The QMSL function will currently >>not<< capture and process the event that results from this
	  function call.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotID = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param iSIM_PersoFeature = input, see description of GSDI_DIAG_PersoFeature_Enum
	\param sEFS_Filename = File at which the return IMEI + key data will be stored at
	\param piTransactionID = Ouptut, returned in order to identify the related event
	\param piCommandStatus = Output, result of CMD_STATUS field, if not GSDI_SUCCESS then the function returns a false.	\return true if successful, false if fail.

	\return true if successful, false if fail.

	\warning Will activate event messages and clear the current event queue

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GSDIDIAG_GET_FEATURE_INDICATOR_KEY_CMD
		( 
			HANDLE hResourceContext, 
			unsigned char iSlotId, 
			unsigned char iSIM_PersoFeature,	
			char* sEFS_Filename, 
			unsigned short* piTransactionID,
			unsigned long* piCommandStatus
		);

	/******************************************************************************/
	/**
	GDSI ICD, 3.14 GSDIDIAG_UNBLOCK_FEATURE_INDICATOR_CMD

	ICD Description:
		This request message is used to unblock a blocked personalization feature indicator in SIM Lock
		feature. All fields are mandatory unless specifically indicate that they are an optional field.
		The command is processed and put onto the GSDI Task Command Queue. So long as the
		queuing of the command is successful, a response with a valid Transaction ID is provided. Check
		the Command Status and verify it is successful before accepting the Transaction ID.
		At the completion of the Command Processing in the GSDI Task, an event is generated to notify
		the client.
  
	Related script command: SIMUnblockFeature
 
 	  The QMSL function will currently >>not<< capture and process the event that results from this
	  function call.
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotID = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param iSIM_PersoFeature = input, see description of GSDI_DIAG_PersoFeature_Enum
	\param sSIM_PersoKey = input, Personalization Key Value; Input is in string format, NULL terminated

	\param piTransactionID = Ouptut, returned in order to identify the related event
	\param piCommandStatus = Output, result of CMD_STATUS field, if not GSDI_SUCCESS then the function returns a false.	

	\return true if successful, false if fail.

	\warning Will activate event messages and clear the current event queue 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GSDIDIAG_UNBLOCK_FEATURE_INDICATOR_CMD
		( 
			HANDLE hResourceContext, 
			unsigned char iSlotId, 
			unsigned char iSIM_PersoFeature,	
			char* sSIM_PersoKey,
			unsigned short* piTransactionID,
			unsigned long* piCommandStatus
		);

	/******************************************************************************/
	/**
	GDSI ICD, 3.15 GSDIDIAG_GET_PERM_FEATURE_INDICATION_CMD
	
	ICD Description:
		This request message is used to get the various permanent personalization feature indication with
		respect to SIM Lock feature. All fields are mandatory unless they specifically indicate that they
		are optional.
		The command is processed and put onto the GSDI Task Command Queue. So long as the
		queuing of the command is successful, a response with a valid Transaction ID is provided. Check
		the Command Status and verify it is successful before accepting the Transaction ID.
		At the completion of the Command Processing in the GSDI Task, an event is generated to notify
		the client.

	Related script command:  SIMGetPermanentFeatureIndicators

  	  The QMSL function will currently >>not<< capture and process the event that results from this
	  function call.
 
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotID = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>

	\param piTransactionID = Ouptut, returned in order to identify the related event
	\param piCommandStatus = Output, result of CMD_STATUS field, if not GSDI_SUCCESS then the function returns a false.	

	\return true if successful, false if fail.

	\warning Will activate event messages and clear the current event queue

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GSDIDIAG_GET_PERM_FEATURE_INDICATION_CMD
		( 
			HANDLE hResourceContext, 
			unsigned char iSlotId, 
			unsigned short* piTransactionID,
			unsigned long* piCommandStatus
		);
	
	/******************************************************************************/
	/**
	GDSI ICD, 3.16 GSDIDIAG_PERM_DISABLE_FEATURE_INDICATOR_CMD 

	ICD Description:
		This request message is used to permanently disable a personalization feature indicator in SIM
		Lock feature. All fields are mandatory unless they specifically indicate that they are optional.
		The command is processed and put onto the GSDI Task Command Queue. So long as the
		queuing of the command is successful, a response with a valid Transaction ID is provided. Check
		the Command Status and verify it is successful before accepting the Transaction ID.
		At the completion of the Command Processing in the GSDI Task, an event is generated to notify
		the client.

	Related script command:  SIMDisableFeaturePermanently

  	  The QMSL function will currently >>not<< capture and process the event that results from this
	  function call.
 
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iSlotID = input, <See description of GSDIDIAG_ROOT_KEY_WRITE_CMD>
	\param iSIM_PersoFeature = input, see description of GSDI_DIAG_PersoFeature_Enum
	\param sSIM_PersoKey = input, Personalization Key Value; Input is in string format, NULL terminated

	\param piTransactionID = Ouptut, returned in order to identify the related event
	\param piCommandStatus = Output, result of CMD_STATUS field, if not GSDI_SUCCESS then the function returns a false.	

	\return true if successful, false if fail.

	\warning Will activate event messages and clear the current event queue

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GSDIDIAG_PERM_DISABLE_FEATURE_INDICATOR_CMD
		( 
			HANDLE hResourceContext, 
			unsigned char iSlotId, 
			unsigned char iSIM_PersoFeature,	
			char* sSIM_PersoKey,
			unsigned short* piTransactionID,
			unsigned long* piCommandStatus
		);


/******************************************************************************
						AGPS FTM
*******************************************************************************/

	/******************************************************************************/
	/**
	Factory Test Mode AGPS, 3.2.1 Subcommand 0 - set test mode

	Enables/disables AGPS Test Mode

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iOnOff = 0 - OFF disable test mode, 1 - ON enable test mode
	
	\return true if successful, false if fail.

	\warning Not implemented yet 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_AGPS_SET_TEST_MODE( HANDLE hResourceContext, unsigned char iOnOff );

	/******************************************************************************/
	/**
	Factory Test Mode AGPS, 3.2.2 Subcommand 1 - self-test

	Starts a self-test of AGPS-based MSM; tests include memory test, signal
	        processing test, and boot code checksum test

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iPassFail = returns 1 byte with 3 LSB indicators:
	                      0x01 = Memory result
						  0x02 = STP result
						  0x03 = Bootcode Checksum

						  these 3 results are 0 if self test passes, 1 if self test fails

						  This field should be and with 0x07 and if the result is zero,
						  then all 3 modes are pass.

	\return true if sent successfully, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_AGPS_SELF_TEST( HANDLE hResourceContext, unsigned char* iPassFail  );

	/******************************************************************************/
	/**
	Factory Test Mode AGPS, 3.2.3 Subcommand 2 - set idle mode

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	Places the Location Services Manager (LSM) task in idle mode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_AGPS_IDLE_MODE( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	Factory Test Mode AGPS, 3.2.4 Subcommand 3 - standalone test mode

	Verifies the GSP operation in a standalone mode of AGPS-based MSM

		In this mode, an AGPS-based MSM performs position fixes at the rate of 1 sec continuously. The
		results of each fix are sent out as MMGPS LOG packets. The results could include Satellite 
		Vehicle (SV) statistics (like SNR, Doppler, code phase, uncertainties, etc.), calculated positions,
		and/or statistics of the RF path (oscillator frequency bias, AGC calibration, DC offset
		adjustments, etc.).
		This test is repetitive and is performed every second. This can be stopped by sending either an
		Idle Mode command or by sending a request to start a different test.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_AGPS_STANDALONE_TEST_MODE( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	Factory Test Mode AGPS, 3.2.5 Subcommand 4 - SV tracking

	Processes a single SV continuously and produces results on a regular basis

		The results, which include SV SNR, Doppler, code phase, etc., are sent out as MMGPS 20
		LOG packets.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param = iSatelliteVehicleId = SV identifier, 0-32
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_AGPS_SV_TRACKING( HANDLE hResourceContext, unsigned char iSatelliteVehicleId );

	/******************************************************************************/
	/**
	Factory Test Mode AGPS, 3.2.6 Subcommand 5 - IQ capture

	Captures a snapshot of the baseband IQ data

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iCenterFreq_Hz = center frequency (Hz)
	\param iSampleFreq_Hz = (1 MHz or 2MHz)
	  \code
         0 - I/Q Capture Sampling frequency 1MHz
		 1 - I/Q Capture Sampling frequency 2KHz 
	  \endcode
	\param iStartDelay = start delay (ms)
	\param iAutoCenter = auto center frequency, 0-No auto center, 1-Auto Center
	\param piMaxRecordSize = output, maximum number of samples take.  Each sample is (1) I sample and (2) Q sample.
	                         Each I sample is a 16-bit signed number, as is each Q sample.
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_AGPS_IQ_CAPTURE
		( 
			HANDLE hResourceContext, unsigned long iCenterFreq_Hz, unsigned long iSampleFreq_Hz, 
			unsigned short iStartDelay, unsigned char iAutoCenter, unsigned short* piMaxRecordSize 
		);

	/******************************************************************************/
	/**
	Factory Test Mode AGPS, 3.2.7 Subcommand 6 - get IQ data

	Returns the baseband IQ data

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param pIQ_Data = Buffer of 256 16-bit numbers, representing 128 pairs of I and Q samples
	\param piRecord = The record number that is being requested.  A group of 256 samples is a record.
					  Each sample is either an I or a Q sample of 16-bits.  The order is I0, then Q0,
					  then I1 then Q1, to I127 and Q127.
					  
	                  The response packet returns the actual record number that is returned, and
					  this value is updated with the value stored in the response packet.
	
	\return true if successful, false if fail.

	\warning

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_AGPS_GET_IQ_DATA
		( HANDLE hResourceContext, short* piIQ_Data, unsigned short* piRecord );

	/******************************************************************************/
	/**
	Factory Test Mode AGPS, 3.2.8 Subcommand 122 - get C/N 

	Returns the AGPS C/N in units 1/10dB.  Uses the LSM to perform an IQ capture
	and spectral analysis

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piCTON = Output, C/N value in dB multiplied by 10
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_AGPS_GET_CTON
		( HANDLE hResourceContext, long* piCTON );

/******************************************************************************
						Common RF FTM
*******************************************************************************/

	/******************************************************************************/
	/**
	FTM RF Common, 80-VA888-1 section 2.1, FTM_TX_RX_FREQ_CAL_SWEEP

	This command instructs the mobile to perform the CDMA TX RX Frequency measurement
	described by 80-V5532-2.  It will setup the appropriate mobile logging so that the
	resulting log message will be captured.  To read the contents of the log message
	use the function Get_FTM_TX_RX_FREQ_CAL_SWEEP_Results()

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param pFTM_Tx_Rx_Freq_Cal_Sweep_Request_Response = structure to hold input and output values,
	       of type "FTM_Tx_Rx_Freq_Cal_Sweep_Request_Response," as defined in QLIB_Defines.h

	\return true if successful, false if fail.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_TX_RX_FREQ_CAL_SWEEP( 
		HANDLE hResourceContext, void* pFTM_Tx_Rx_Freq_Cal_Sweep_Request_Response);


	/******************************************************************************/
	/**
	FTM RF Common, 80-VA888-1 section 2.2, FTM_LOAD_RF_NV

	This operation is used to load RF NV from NV and into the RF Driver immediately. 
	This allows the test automation to skip the step of putting the mobile into ONLINE only 
	for the purpose of getting RF NV loaded properly.

	This command is designed to be used in test stations where RF Calibration is performed and 
	non-signaling (specification/performance) testing is to be done immediately afterwards, without resetting the phone.

	If this step is not done, then the RF NV is not loaded from NV to the RF Driver, and the phone will not
	use the NV item changes that have occurred since the last time the phone was powered up.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	
	\param pFTM_Error_Code =RFNV_READ_SUCCESS =0
							RFNV_READ_FAILED=1
							RFNV_WRITE_SUCCESS=2
							RFNV_WRITE_FAILED=3
							RFNV_INACTIVE_ITEM=4

	\return true if successful, false if fail.

	\warning The transmitter should be turned off before this command is called.
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_LOAD_RF_NV( 
		HANDLE hResourceContext, unsigned short* pFTM_Error_Code);

	/******************************************************************************/
	/**
	FTM RF Common, 80-VA888-1, FTM_DO_INTERNAL_DEVICE_CAL

	This operation is used to perform internal calibrations, which are normally done
	the first time a phone is powered.   For example, on the MSM6280 with RTR6275,
	this function will calibrate NV_RFR_IQ_LINE_RESISTOR_I, NV_RFR_BB_FILTER_I, and 
	NV_RTR_BB_FILTER_I.


	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	
	\return true if successful, false if fail.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_DO_INTERNAL_DEVICE_CAL(	HANDLE hResourceContext );


/******************************************************************************
						Software Download
*******************************************************************************/

	/******************************************************************************/
	/**
	Upload a HEX file from the PC to the phone.  
	
	This is also commonly referred to as "software download," but in a technical sense 
	the information is being transferred from the PC to the phone, so the function name 
	is based on the "upload" operation

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param sFileName = the local path and filename of the hex file to upload.
	\param sSPC = Service Provider Code, normally "000000"
	\param bClearErrorLog = true to clear phone's NV items which start the most recent errors.
	\param bOverrideModelCheck = true to allow a mismtach between the HEX file and the phone's model identifier
	\param bSkipReset = true to skip the reset of the phone after the HEX file is downloaded
	\param bUseCustomArmProg = true to always use "ARMPRG.HEX"  
	
							   false, the ARM PROG file will be determined by querying the phone 
							   model and selecting the correct APRGxxxx.HEX file.

								These files are always in the QPST Server executable folder, usually
								C:\Program Files\QPST\bin

	\param bUploadArmProg = true to send ARMPROG, false to not send ARMPROG
	\param bDoAutoRestoreBackup = true to backup the phone's NV data to the QCN file indicated
									by sAutoRestoreBackupName.
	\param sAutoRestoreBackupName = Name of the QCN file to backup the phone's data into.
  
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_UploadHexFile
	( 
		HANDLE hResourceContext,
		char* sFileName,
		char* sSPC,
		unsigned char  bClearErrorLog,
		unsigned char  bOverrideModelCheck,
		unsigned char  bSkipReset,
		unsigned char  bUseCustomArmProg,
		unsigned char  bUploadArmProg,
		unsigned char  bDoAutoRestoreBackup,
		char* sAutoRestoreBackupName
	);

	/******************************************************************************/
	/**
	Upload a QCN (NV memory contents) file from the PC to the phone.  
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param sFileName = Path and file name of QCN file
	\param sSPC = Service provider code, usually "000000"
	\param bAutoRestore = true always, when sending a QCN file to the phone
	\param bSkipReset = true to not reset after QCN file uploaded, false to force reset after QCN file is uploaded
	\param bAllowEsnMismatch = true to allow the ESN to be mismatched between the QCN file and the phone
	\param bIgnorePhoneModel = true to allow the phone model number to be mismatched between the QCN file and the phone
  
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_UploadQcnFile
	(
		HANDLE hResourceContext, 
		char* sFileName,
		char* sSPC,
		unsigned char bAutoRestore,
		unsigned char bSkipReset,
		unsigned char bAllowEsnMismatch,
		unsigned char bIgnorePhoneModel
	);

	/******************************************************************************/
	/**
	Download a QCN (NV memory contents) file from the phone to the PC.  
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param sFileName = Path and file name of QCN file
	\param sSPC = Service provider code, usually "000000"
  
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DownloadQcnFile
	(
		HANDLE hResourceContext, 
		char* sFileName,
		char* sSPC
	);

	/******************************************************************************/
	/**
	Download boot loader image taken from hex file from PC to MP and next should restart
	MP (but this funtionality is not done good in MP). 
	QLIB_UploadBootLoaderOnlyHexFile
	 
	
	*******************************************************************************/
	QLIB_API unsigned char QLIB_UploadBootLoaderOnlyHexFile( 
							 HANDLE hResourceContext, 
							 char* sFileName, char* sSPC,  
							 unsigned char bUseCustomArmProg, 
							 unsigned char bUploadArmProg
							);

	/******************************************************************************/
	/**
	Upload a Boot file from the PC to the phone.  
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param sFileName = the local path and filename of the hex boot file to upload.
  
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_UploadBootFileBeforeHexFile
	( 
		HANDLE hResourceContext,
		char* sFileName
	);

	/******************************************************************************/
	/**
	Upload a CEFS file from the PC to the phone.  
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param sFileName = the local path and filename of the CEFS file to upload.
  
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_UploadCEFS_File
	( 
		HANDLE hResourceContext,
		char* sFileName,
		char* sPartitionFileName
	);

	/******************************************************************************/
	/**
	Upload a CEFS file from the PC to the phone, specifying the ARMPROG file, for
	use when the phone is already in download mode.
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param sFileName = the local path and filename of the CEFS file to upload.
  	\param iDloadArmprg = if bUseCustomArmprg = false, then ID of the ARMPRG to use
	                       refer to the enumeration, dloadArmprgType

	\code
	From QPST code, SharedInc/ArmprgType.h:

	typedef enum		// Default armprg to use when mobile alread in download mode.
	{
	  apStandard	= 0,	// MSM3000, 3100, 3300, 5000, 5010, 5100, 5105
	  apMSM6000     = 1,	// MSM6000
	  apMSM6050     = 2,    // MSM6050
	  apMSM6200_1	= 3,	// MSM6200 Cheetah
	  apMSM6100     = 4,    // MSM6100 Cougar with NOR flash
	  apMSM6100NAND	= 5,	// MSM6100 Cougar with NAND flash.
	  apMSM6250     = 6,	// MSM6250 (Saber) NOR flash
	  apMSM6250NAND	= 7,	// MSM6250 (Saber) NAND flash
	  apMSM6500     = 8,	// MSM6500 (Jaguar) NOR flash
	  apMSM6500NAND	= 9,	// MSM6500 (Jaguar) NAND flash
	  apMSM6100NORFFA=10,	// MSM6100 FFA with NOR flash
	  apMSM6550     = 11,	// MSM6550 (Eagle) NOR flash
	  apMSM6550NAND = 12,	// MSM6550 NAND (3/9/05)
	  apMSM6550PB   = 13,	// MSM6550 progressive boot
	  apEM6700      = 14,	// MSM6700 emulator (Quasar) 
	  apMSM6275NOR  = 15,	// MSM6275 (Raven) NOR
	  apMSM6275NAND = 16,	// MSM6275 (Raven) NAND
	  apMSM6800NOR  = 17,	// MSM6800 (DoRA) NOR
	  apMSM6800NAND = 18,	// MSM6800 (DoRA) NAND
	  apMSM6250Sec  = 19,	// MSM6250 secure (one-time programmable)
	  apMSM7500NAND = 20,   // MSM7500 (Phoenix)
	  apMSMSC1x     = 21,	// SC1x a.k.a. QSC, NOR progressive
	  amMSM6280NAND = 22,	// MSM6280 NAND multi-image

	  apMarkerForDialog // Don't use!
	} dloadArmprgType ;
	\endcode

	\return true if successful, false if fail.

	\warning This function is grouped with the EFS functions, not the download functions,
	         so it is important to check for errors in the EFS area, not Software download.

			 This might be a little confusing because the other CEFS functions are grouped
			 under software download.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_UploadCEFS_File2
	( 
		HANDLE hResourceContext,
		char* sFileName,
		char* sPartitionFileName,
		long iDloadArmprg
	);

	/******************************************************************************/
	/**
	Upload a multi-image software image from the PC to the phone.  This function
	performs an "OBL" download, which is equivalent to the QPST Software Download application.

	For the following string inputs DON'T PASS NULL's, use "".  To control which files
	will be sent, use the paraemter, iDownloadBitMask.
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param sPartitionFileName = partition path and file name, usually "partition.mbn"
	\param sOTBPL_FileName = OTBPL path and file name, usually "obl.mbn"
	\param sPBL_FileName = Primary Boot Loader, usually "pbl.mbn"
	\param sQCSBL_FileName = secure boot loader, usually "qcsblhd_cfgdata.mbn"
	\param sQCSBL_HeaderName = secure boot loader header, usually "qcsbl.mbn"
	\param sOEMSBL_FileName = OEM secure boot loader, usually "oemsbl.mbn"
	\param sOEMSBL_HeaderName = OEM secure boot loader header, usually "oemsblhd.mbn"
	\param sModemFileName = AMSS File, usually "amss.mbn"
	\param sModemHeaderName = AMSS Header, usually "amsshd.mbn"
	\param sAppsFileName = Application file
	\param sAppsHeaderName = Application header file
	\param char bOverridePrtnTable = TRUE to overwrite the stored partition table in the phone
	\param char bUseTrustedMode = TRUE to use trusted mode and the AMSS has a built-in PBL. 
	                              If FALSE, then PBL must be provided.

	\param sSPC = Service Programming Code, usually "000000"
	\param bClearErrorLog = TRUE to clear the QPST error log
	\param sStorageFile = file name of temporary QCN file
	\param bOverrideModelCheck = TRUE to override a model number check
	\param bSkipReset = TRUEto skip the reset operation after the phone is programmed
	\param bUseCustomArmprg =  FALSE to let QPST server determine which ARMPRG to bet used.
	                          If TRUE, then the NPRG or ARMPRG will be searched for in the
							  same folder as the modem file name.
	\param iDloadArmprg = if bUseCustomArmprg = false, then ID of the ARMPRG to use
	                       refer to the enumeration, dloadArmprgType
	\param iDownloadBitMask = bitmask of files to download, based on SWD_miFileTypeBitmask_enum
  
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_UploadMultiImage
	( 
		HANDLE hResourceContext,
		char* sPartitionFileName,
		char* sOTBPL_FileName,
		char* sPBL_FileName,
		char* sQCSBL_FileName,
		char* sQCSBL_HeaderName,
		char* sOEMSBL_FileName,
		char* sOEMSBL_HeaderName,
		char* sModemFileName,
		char* sModemHeaderName,
		char* sAppsFileName,
		char* sAppsHeaderName,
		char* sAppsBlFileName,
		char* sAppsBlHeaderFileName,
		unsigned char bOverridePrtnTable,
		unsigned char bUseTrustedMode,
		char* sSPC,
		unsigned char bClearErrorLog,
		char* sStorageFile,
		unsigned char bOverrideModelCheck,
		unsigned char bSkipReset,
		unsigned char bUseCustomArmprg,
		long iDloadArmprg,
		long iDownloadBitMask
	);

	/******************************************************************************/
	/**
	Upload a multi-image software image from the PC to the phone.  This function
	is equivalent to the "Multi-image" tab in the QPST Software Download application.

	For the following string inputs DON'T PASS NULL's, use "".  To control which files
	will be sent, use the paraemter, iDownloadBitMask.
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param sPartitionFileName = partition path and file name, usually "partition.mbn"
	\param sPBL_FileName = Primary Boot Loader, usually "pbl.mbn"
	\param sQCSBL_FileName = secure boot loader, usually "qcsblhd_cfgdata.mbn"
	\param sQCSBL_HeaderName = secure boot loader header, usually "qcsbl.mbn"
	\param sOEMSBL_FileName = OEM secure boot loader, usually "oemsbl.mbn"
	\param sOEMSBL_HeaderName = OEM secure boot loader header, usually "oemsblhd.mbn"
	\param sModemFileName = AMSS File, usually "amss.mbn"
	\param sModemHeaderName = AMSS Header, usually "amsshd.mbn"
	\param sAppsFileName = Application file
	\param sAppsHeaderName = Application header file
	\param sAppsBlFileName = Application boot loader file
	\param sAppsBlHeaderFileName = Application boot loader header file
	\param char bOverridePrtnTable = TRUE to overwrite the stored partition table in the phone
	\param char bUseTrustedMode = TRUE to use trusted mode and the AMSS has a built-in PBL. 
	                              If FALSE, then PBL must be provided.

	\param sSPC = Service Programming Code, usually "000000"
	\param bClearErrorLog = TRUE to clear the QPST error log
	\param sStorageFile = file name of temporary QCN file
	\param bOverrideModelCheck = TRUE to override a model number check
	\param bSkipReset = TRUEto skip the reset operation after the phone is programmed
	\param bUseCustomArmprg =  FALSE to let QPST server determine which ARMPRG to bet used.
	                          If TRUE, then the NPRG or ARMPRG will be searched for in the
							  same folder as the modem file name.
	\param iDloadArmprg = if bUseCustomArmprg = false, then ID of the ARMPRG to use
	                       refer to the enumeration, dloadArmprgType
	\param iDownloadBitMask = bitmask of files to download, based on SWD_miFileTypeBitmask_enum
  
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_UploadMultiImage2
	( 
		HANDLE hResourceContext,
		char* sPartitionFileName,
		char* sPBL_FileName,
		char* sQCSBL_FileName,
		char* sQCSBL_HeaderName,
		char* sOEMSBL_FileName,
		char* sOEMSBL_HeaderName,
		char* sModemFileName,
		char* sModemHeaderName,
		char* sAppsFileName,
		char* sAppsHeaderName,
		char* sAppsBlFileName,
		char* sAppsBlHeaderFileName,
		unsigned char bOverridePrtnTable,
		unsigned char bUseTrustedMode,
		char* sSPC,
		unsigned char bClearErrorLog,
		char* sStorageFile,
		unsigned char bOverrideModelCheck,
		unsigned char bSkipReset,
		unsigned char bUseCustomArmprg,
		long iDloadArmprg,
		long iDownloadBitMask
	);

	/******************************************************************************/
	/**
	Aborts the most recent software download operation (SW Download, QCN file transfer).  

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	
	\return none

	\warning Any current operations will be terminated.  This can cause problems with the
	         phone state.  If a FLASH programming is interrupted, the phone may become
			 unusable and require JTAG.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_AbortSW_DownloadOperation( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	Return the phone status, as reported by QPST Server.  

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piStatus = value of the current status, as defined by the SWD_enPhoneMode enumeration 
	                  in SoftwareDownload.h

	\return true if successful, false if fail.

	\warning---this function is only valid when the library is in QPST mode

	*******************************************************************************/
	QLIB_API unsigned char QLIB_GetQpstPhoneStatus( HANDLE hResourceContext, unsigned long* piStatus );

	/******************************************************************************/
	/**

	Get the error status and description of the last software download operation.

		\param bErrorOccurred    = true if an error occurred during the last operation.  False if no error
								   occurred during the last operation.

		\param sErrorMessage     = Filled filled in with a text description of the most recent error.
								   Empty if no error has occurred.  

		\param iMaxStringLength  = Maximum number of characters available in the sErrorMessage buffer

		\return true if successful, false if failure

		\warning  The error information will be cleared after this is called.

	*******************************************************************************/
	QLIB_API void QLIB_GetLastSoftwareDownloadErrorInfo
		( HANDLE hResourceContext, unsigned char* bErrorOccurred, char *sErrorMessage, int iMaxStringLength );

/******************************************************************************
						MediaFLO FTM
*******************************************************************************/

	/******************************************************************************/
	/**
	Factory Test Mode MediaFLO, 3.10 Subcommand 400 - Get RSSI Cal Point

			This function returns a 16-bit signed RSSI cal point for the requested gain state. 

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iGainState = the gain state (0 = highest gain, 3 = lowest gain)
	\param iInputPower = the receiver input power, 1 dB resolution
	\param piCalPoint = the RSSI cal point for the requested gain state, 1/256 dB
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MF_GET_RSSI_CAL_POINT(HANDLE hResourceContext, unsigned short iGainState,
										  short iInputPower, short *piCalPoint);

	/******************************************************************************/
	/**
	Factory Test Mode MediaFLO, 3.9 Subcommand 401 - Get Rx RSSI

			This function returns a 16-bit signed receiver signal strength indicator (RSSI) value. 

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piRssiValue = RSSI value in 1/100 dB
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MF_GET_RX_RSSI( HANDLE hResourceContext, short *piRssiValue );

	/******************************************************************************/
	/**
	Factory Test Mode MediaFLO, 3.7 Subcommand 402 - Get AGC State

			This function returns the current gain state and 15-bit signed DVGA accumulator value.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piGainState = current gain state (0 = highest gain state, 3 = lowest gain state)
	\param piDvga = current DVGA accumulator value, 15-bit signed value
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MF_GET_AGC_STATE(HANDLE hResourceContext, unsigned short *piGainState, short *piDvga);

	/******************************************************************************/
	/**
	Factory Test Mode MediaFLO, 3.12 Subcommand 403 - Write Cal Data

			This function stores the current state of the RF Calibration to NV.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iBand = the MediaFLO band to write to NV
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MF_WRITE_CAL_DATA(HANDLE hResourceContext, unsigned short iBand);
	
	/******************************************************************************/
	/**
	Factory Test Mode MediaFLO, 3.2 Subcommand 406 - LNA Select

			This function enables the desired LNA in the RBR device.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iWhichLna = which LNA to enable (1=LNA1, 2=LNA2)
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MF_LNA_SELECT( HANDLE hResourceContext, unsigned short iWhichLna );
	
	/******************************************************************************/
	/**
	Factory Test Mode MediaFLO, 3.3 Subcommand 407 - RF Switch Config

			This function sets the state of the LNA RF Switch Control pins SW_CTL1 and SW_CTL2. 

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iState = the state of the LNA RF Switch Control pins
						0 - SW_CTL1 Low, SW_CTL2 Low
						1 - SW_CTL1 Low, SW_CTL2 High
						2 - SW_CTL1 High, SW_CTL2 Low
						3 - SW_CTL1 High, SW_CTL2 High
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MF_RF_SWITCH_CONFIG( HANDLE hResourceContext, unsigned short iState );

	/******************************************************************************/
	/**
	Factory Test Mode MediaFLO, 3.6 Subcommand 408 - Synth Lock

			This function returns the lock state of the synthesizer.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param pbLockState = the lock state of the synthesizer (0=unlocked, 1=locked)
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MF_SYNTH_LOCK( HANDLE hResourceContext, unsigned char* pbLockState );

	/******************************************************************************/
	/**
	Factory Test Mode MediaFLO, 3.8 Subcommand 411 - Set Gain State

			This function sets the gain state of the currently selected LNA.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iGainState = the desired gain state (0=highest gain state, 3=lowest gain state)
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MF_SET_GAIN_STATE(HANDLE hResourceContext, unsigned short iGainState);

	/******************************************************************************/
	/**
	Factory Test Mode MediaFLO, 3.5 Subcommand 413 - Tune PLL

			This function tunes the PLL to the requested FLO Air Interface Specification channel.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iChannel = the desired channel (0 - 8191)
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MF_TUNE_PLL( HANDLE hResourceContext, unsigned short iChannel );

	/******************************************************************************/
	/**
	Factory Test Mode MediaFLO, 3.11 Subcommand 415 - Get IM2

			This function gets the MediaFlo IM2 calibration values.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piSuppression = characteristic IM2 performance improvement vs. uncalibrated in dB
	\param piI_Value = I channel IM2 calibration value
	\param piQ_Value = Q channel IM2 calibration value
	\param piTransConductance_Value = Transconductor IM2 calibration value
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MF_GET_IM2(	HANDLE hResourceContext,
												short* piSuppression,
												unsigned char* piI_Value, 
												unsigned char* piQ_Value, 
												unsigned char* piTransConductance_Value);

/******************************************************************************
						MediaFLO NS FTM
*******************************************************************************/

	/******************************************************************************/
	/**
	Factory Test Mode MediaFLO Non-Signalling, 3.3.1 Subcommand 0 - Enable FLO Demod

			This function enables FLO demodulation during FTM mode. 
			The MediaFLO hardware must have been previously	initialized by the 
			FTM MFLO Set Mode RF. 

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MFLO_NS_SET_NS_MODE( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	Factory Test Mode MediaFLO Non-Signalling, 3.3.2 Subcommand 1 - Acquire FLO System

			This function forces FLO to perform a FLO system acquisition. An attempt 
			will be made to acquire both the WIDE and LOCAL systems.   Acquisition may 
			require several seconds. The request message contains an additional field 
			that instructs FTM to wait (in msec) before returning the system status in 
			the response message.  If the wait time is 0 the response will be sent 
			immediately.  In this case the FLO system may not be acquired when the 
			response is sent.  The FTM_MF_GET_OIS_STATE command can be used to determine 
			if a system has been acquired.  

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iacqWaitTime = time to wait for acquisition.
	\param bLocalAcquired = 1: Local System is valid (acquired)
							0: Local System is invalid (not acquired)
	\param bWideAcquired = 1: Wide System is valid (acquired)
						   0: Wide System is invalid (not acquired) 
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MFLO_NS_ACQUIRE_FLO_SYSTEM( HANDLE hResourceContext, 
															  unsigned short iAcqWaitTime,
															  unsigned char *pbLocalAcquired,
															  unsigned char *pbWideAcquired );

	/******************************************************************************/
	/**

	Factory Test Mode MediaFLO Non-Signalling, 3.3.3 Subcommand 2 - Activate Flow

			This function activates the Flow specified by the caller.  
			Activation may require several seconds. The request message contains 
			an additional field that instructs FTM to wait (in msec) before returning 
			the activation response message.  If the wait time is 0 the response will 
			be sent immediately and the ACTIVATION_STATUS in the response will be set 
			to ACTIVATING_FLOW.  In addition the stream information returned in the 
			response will be invalid (set to 0).  The FTM_MFLO_NS_GET_STREAM_INFO command 
			can be used to determine if the Flow was successfully activated. If a wait 
			time is specified the ACTIVATION_STATUS in the response will be set to either 
			ACTIVATION_FAILED or ACTIVATION_SUCCESS depending upon the stream state at the 
			wait time expiration. 
			Non-signaling mode in FTM supports a maximum of 20 simultaneously active Flows.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iAcqWaitTime = time to wait for acquisition.
	\param iFlowId = specfies the flow to activate.
	\param eBinding = specifies the binding type. This is both an input and output param
		Defined by mftypes_binding_type:
		\code
		MFLO_OVERHEAD   = 0x00,
		RT_VIDEO_CODEC  = 0x01,
		RT_AUDIO_CODEC  = 0x02,
		NRT_MEDIA       = 0x03,
		DATACAST        = 0x04,
		TAP             = 0x05,
		FIXED_TAP       = 0x06,
		\endcode
	\param eStatus = specifies activate flow status.
	Defined by mftypes_activate_status_type:
		\code
		FAILED				= 0x00,
		ACTIVATING			= 0x01,
		SUCCESS				= 0x02
		\endcode
	\param iMlcID = specifies the MLC ID.
	\param iStreamID = specifies the Stream ID.
	\param bStreamUsesBothLayers = true(1) if enhanced layer fata is present.
	\param bByteInterleaveEnabled = true(1) if byte interleave is enabled.
	\param eSystem = specifies the system.
		Defined by mftypes_system_type:
		\code
		WIDE			= 0x00,		//! Wide System 
		LOCAL			= 0x01,		//! Local System 
		\endcode
		
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MFLO_NS_ACTIVATE_FLOW( HANDLE hResourceContext,
														 unsigned short iAcqWaitTime,
														 unsigned long iFlowId,	
														 unsigned char *piBindingType,
														 unsigned char *piStatus,
														 unsigned char *piMlcId,
														 unsigned char *piSteamId,
														 unsigned char *pbStreamUsesBothLayers,
														 unsigned char *pbByteInterleaveEnabled,
														 unsigned char *piSystem );

	/******************************************************************************/
	/**

	Factory Test Mode MediaFLO Non-Signalling, 3.3.4 Subcommand 3 - Deactivate Flow

			This function deactivates the Flow specified by the caller.  If the Flow 
			Id in the request message is not active FLOW_NOT_ACTIVATED is returned 
			in the response, otherwise FLOW_DEACTIVATING is returned.  
			The FTM_MFLO_NS_GET_STREAM_INFO command can be used to determine if the Flow 
			was successfully deactivated.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iFlowId = specifies the Flow ID.
	\param eStatus = specifies deactivate flow status.
	Defined by mftypes_flow_status_type:
		\code
		FLOW_NOT_ACTIVATED  	= 0x00,
		FLOW_DEACTIVATING		= 0x02
		\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MFLO_NS_DEACTIVATE_FLOW( HANDLE hResourceContext,
															 unsigned long iFlowId, 
															 unsigned char *piStatus );

	/******************************************************************************/
	/**

	Factory Test Mode MediaFLO Non-Signalling, 1.3.5 Subcommand 4 - Get Stream Info

			This function command retrieves the stream information for the specified 
			Flow ID.  If the Flow Id specified in the request message is not active 
			FLOW_NOT_ACTIVATED is returned in the response message, otherwise 
			FLOW_ACTIVATED is returned.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iFlowId = specifies the Flow ID.
	\param eStatus = specifies deactivate flow status.
	Defined by mftypes_flow_status_type:
		\code
		FLOW_NOT_ACTIVATED  	= 0x00,
		FLOW_ACTIVATED			= 0x01
		\endcode
	\param iMlcID = specifies the MLC ID.
	\param iStreamID = specifies the Stream ID.
	\param bStreamUsesBothLayers = true(1) if enhanced layer fata is present.
	\param bByteInterleaveEnabled = true(1) if byte interleave is enabled.
	\param peBindingType = specifies the binding type.
		Defined by mftypes_binding_type:
		\code
		MFLO_OVERHEAD   = 0x00,
		RT_VIDEO_CODEC  = 0x01,
		RT_AUDIO_CODEC  = 0x02,
		NRT_MEDIA       = 0x03,
		DATACAST        = 0x04,
		TAP             = 0x05,
		FIXED_TAP       = 0x06,
		\endcode
	\param eSystem = specifies the system.
		Defined by mftypes_system_type:
		\code
		WIDE			= 0x00,		//! Wide System 
		LOCAL			= 0x01,		//! Local System 
		\endcode
			
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_STREAM_INFO( HANDLE hResourceContext,
														   unsigned long iFlowId,
														   unsigned char *piStatus,
														   unsigned char *piMlcID,
														   unsigned char *piStreamID,
														   unsigned char *pbStreamUsesBothLayers,
														   unsigned char *pbByteInterleaveEnabled,
														   unsigned char *piBindingType,
														   unsigned char *piSystem );

	/******************************************************************************/
	/**

	Factory Test Mode MediaFLO Non-Signalling, 3.3.6 Subcommand 5 - Get MLC Dynamic Parameters.
	
		This command retrieves the dynamic parameters for the specified MLC ID. The information 
		returned can be used to calculate PER statistics. If the MLC is not active 0 is returned 
		in the first byte of the response message, otherwise 1 is returned.
		

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param iMlcID = specifies the MLC ID. This is both an input and output param.
	\param eSystem = specifies the system
		Defined by mftypes_system_type:
		\code
		WIDE			= 0x00,		//! Wide System 
		LOCAL			= 0x01,		//! Local System 
		\endcode
	\param piFFTAddress = MLC Base FFT Address
	\param peTransmitMode = Transmit Mode 
		Defined by mftypes_trans_mode_type:
		\code
		REG_QPSK_1_3         = 0,
		MREG_QPSK_1_2        = 1,
		REG_16_QAM_1_3       = 2,
		REG_16_QAM_1_2       = 3,
		REG_16_QAM_2_3       = 4,
		REG_16_QPSK_1_5      = 5,
		LAY_QPSK_1_3_ER_4    = 6,
		LAY_QPSK_1_2_ER_4    = 7,
		LAY_QPSK_2_3_ER_4    = 8,
		LAY_QPSK_1_3_ER_6_25 = 9,
		LAY_QPSK_1_2_ER_6_25 = 10,
		LAY_QPSK_2_3_ER_6_25 = 11 
		\endcode
	\param peRSOuterCode = RS Outer Code
		Defined by mftypes_reed_solomon_code_type:
		\code
		RS_16_16_0		= 0x0,
		RS_16_14_2		= 0x1,
		RS_16_12_4		= 0x2,
		RS_16_8_8		= 0x3 
		\endcode
	\param piCodeBlocks = Code Blocks in current Superframe
	\param pbStream1Bound = true(1) if Stream 1 is bound
	\param pbStream2Bound = true(1) if Stream 2 is bound
	\param iStream0Length = Stream 0 length
	\param iStream1Length = Stream 1 length
	\param iStream2Length = Stream 2 length
	\param pBasePlpStats = structure of type mftypes_plp_stats_type to hold the Base PLP statistics 
									as defined in QLIB_Defines.h
	\param pEnhPlpStats = structure of type mftypes_plp_stats_type to hold the Enhanced PLP statistics 
									as defined in QLIB_Defines.h
	\param pBaseCbStats = structure of type mftypes_cb_stats_type to hold the Base CB statistics 
									as defined in QLIB_Defines.h
	\param pEnhCbStats = structure of type mftypes_cb_stats_type to hold the Enhanced CB statistics 
									as defined in QLIB_Defines.h
	
	\return true if successful, false if fail.

	\warning Prior to calling this function, sufficient space should be allocated in
			 pMlcPlpStats to store structure of type mftypes_mlc_plp_stats_type

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_MLC_DYN_PARAMS( HANDLE hResourceContext,
															  unsigned char iMlcID,
															  unsigned char iSystem,
															  unsigned char *piStatus,
															  unsigned long *piFFTAddress,
															  unsigned char *piTransmitMode,
															  unsigned char *piRSOuterCode,
															  unsigned char *piCodeBlocks,
															  unsigned char *pbStream1Bound,
															  unsigned char *pbStream2Bound,
															  unsigned short *piStream0Length,
															  unsigned short *piStream1Length,
															  unsigned short *piStream2Length,
															  void		   *pBasePlpStats,
															  void		   *pEnhPlpStats,
															  void		   *pBaseCbStats,
															  void		   *pEnhCbStats );

	/******************************************************************************/
	/**

	Factory Test Mode MediaFLO Non-Signalling, 3.3.7 Subcommand 7 - Reset MLC PLP Stats.
	
		This command clears (resets to 0) the accumulated PLP/CB statistics for the specified MLC ID. 
		

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param iMlcID = specifies the MLC ID
	\param eSystem = specifies the system
		Defined by mftypes_system_type:
		\code
		WIDE			= 0x00,		//! Wide System 
		LOCAL			= 0x01,		//! Local System 
		\endcode
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MFLO_NS_RESET_MLC_PLP_STATS( HANDLE hResourceContext,
															   unsigned char iMlcID,
															   unsigned char iSystem );

	/******************************************************************************/
	/**

	Factory Test Mode MediaFLO Non-Signalling, 3.3.8 Subcommand 8 - Reset ALL PLP Stats.
	
		This command clears (resets to 0) the accumulated PLP/CB statistics for all of the active MLCs. 		

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
		
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MFLO_NS_RESET_ALL_PLP_STATS( HANDLE hResourceContext );

	/******************************************************************************/
	/**

	Factory Test Mode MediaFLO Non-Signalling, 3.3.9 Subcommand 9 - Get the number of Active Flows.
	
		This command returns the number of currently active Flows. FTM Non-signaling mode 
		supports a maximum of 20 simultaneously active Flows. 		

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
		
	\param piFlowCount = the number of active flows
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_ACTIVE_FLOW_COUNT( HANDLE hResourceContext, unsigned char *piFlowCount );
	
	/******************************************************************************/
	/**

	Factory Test Mode MediaFLO Non-Signalling, 3.3.9 Subcommand 9 - Get all active flow Ids.
	
		This command returns a list of all of the currently active flows.   
		The total number of active flows is also returned.  The list of active flow IDs 
		returned is formatted as a 20 entry by uint32 (4 byte) array (80 total bytes).  
		Each array entry corresponds to an active Flow ID.  If there are less than 20 active 
		Flows (FTM Non-signaling mode supports a maximum of 20 simultaneously active flows) 
		the array will be zero padded. Note that 0 is a valid Flow ID. 		

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
		
	\param piFlowCount = This is both and input and output param.  As an input param
							  it indicates the number of elements in pActiveFlowInfoArray. 
							  As an output param it indicates the actual number of active 
							  flows in the system.
	\param pArrayFlowIds = array of all active flow ids. The list of active Flow IDs returned is 
						formatted as a 20 entry by uint32 (4 byte) array (64 total bytes).  Each 
						array entry corresponds to an active Flow ID.  If there are less than 16 
						active Flows (20 is maximum concurrent activated Flows in FTM non-signaling 
						mode) the array will be zero padded. Note that 0 is a valid Flow ID.
	
	\return true if successful, false if fail.

	\warning Prior to calling this function, sufficient space should be allocated in
			 pArrayFlowIds to store 20x4Byte array (80 total bytes).

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_ALL_ACTIVE_FLOW_IDS( HANDLE hResourceContext, unsigned char *piFlowCount, unsigned long *pArrayFlowIds );
	
	/******************************************************************************/
	/**
	Factory Test Mode MediaFLO Non-Signalling, 3.3.10 Subcommand 10 - Get control channel records.
	
		This command returns a list of control channel records for the specified system.  
		A maximum of eight records are returned per response message.  The record retrieval starts 
		at the record number (greater than 0) specified in the request message.  The total number 
		of records for the specified system is returned.  The next record number in the record 
		database is also returned.  If the system contains more than eight records subsequent 
		requests must be performed to retrieve all of the records. The total_records and next_record 
		fields allow all of the records for a system to be retrieved via a series of 
		FTM_MFLO_NS _GET_CONTROL_CHANNEL_RECORDS messages.  For example, if the starting_record field 
		in the request was set to 5 and the total_records field contains 15 in the response the 
		next_record field in the response field will contain 13 (a maximum 8 records can be retrieved 
		in one request).  Since eight records are retrieved in each response the number of requests 
		required to retrieve all of the records for a system is:
			( TOTAL_RECORD )   mod 8
		The list of Control Channel records returned is formatted as an 8 entry (8 records) array of 
		8 byte records (64 total bytes).  Each array entry corresponds to a Control Channel record.  
		If there are less than 8 records the array will be zero padded. 
		The Flow ID for a particular service is computed as follows using the Control Channel record:
			Flow ID = ( Service ID * 16 ) + Flow Descriptor

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param eSystem = the system
		Defined by mftypes_system_type:
		\code
		WIDE			= 0x00,		//! Wide System 
		LOCAL			= 0x01,		//! Local System 
		\endcode
	\param iStartRecord = the starting record for the next retrieval. 
	\param eStatus = Record retrieval status
		0  = starting record count is invalid or no records are available for the specified system
		1 = records were retrieved
		Defined by mftypes_record_retrieval_status_type:
		\code
		NO_RECORDS_AVAILABLE	= 0x00,
		RECORDS_RETRIEVED	= 0x01 
		\endcode
	\param iTotalRecords = The total number of records that are available for the specified system.
						The iTotalRecords and iNextRecord allow all of the records for a system to 
						be retrieved via a series of calls to QLIB_MFLO_NS_GET_CONTROL_CHANNEL_RECORDS. 
						Since eight records are retrieved in each response the number of requests required 
						to retrieve all of the records for a system is: ( TOTAL_RECORD ) mod 8
	\param iNextRecord = the record number for the next available record for the system.  
						For example, if the iStartRecord param i was set to 5 and the iTotalRecord 
						param contains 15, then the iNextRecord will be 13 (maximum 8 records can 
						be retrieved in one request).  
	\param pControlChannelRecordArray = Array[8] of mftypes_mf_control_channel_record structures 
									(mftypes_mf_control_channel_record is defined in QLib_Defines.h)

	\return true if successful, false if fail.

	\warning Prior to calling this function, sufficient space should be allocated in
			 pControlChannelRecordArray to store eight structures of type mftypes_mf_control_channel_record

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_CONTROL_CHANNEL_RECORDS( HANDLE hResourceContext, 
																	     unsigned char  eSystem,
																		 unsigned short iStartRecord,
																		 unsigned char *piStatus,
																		 unsigned short *piTotalRecords,
																		 unsigned short *piNextRecord,
																		 void			 *pControlChannelRecordArray );

	/******************************************************************************/
	/**
	Factory Test Mode MediaFLO Non-Signalling, 3.3.11 Subcommand 11 - Get AIS channel.
	
	This command returns the current AIS channel.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param piChannel = Currently programmed AIS Channel.
	AIS channel to Khz conversion: Freq (Khz) = 470,000 + AIS_CHAN*50
.
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_AIS_CHANNEL( HANDLE hResourceContext, 
														   unsigned short *piChannel );

	/******************************************************************************/
	/**
	Factory Test Mode MediaFLO Non-Signalling, 3.3.12 Subcommand 12 - Deactivate all flows.
	
	This command causes all flows to be deactivated.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	
	\return true if successful, false if fail.

	\warning

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MFLO_NS_DEACTIVATE_ALL_FLOWS( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	Factory Test Mode MediaFLO Non-Signalling, 3.3.13 Subcommand 13 - Tune Freq and BW.
	
	This command tunes the RF to the specified frequency and bandwidth.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param iFreqKhz = Frequency in KHz.
	\param iBandwidthMHz = Bandwidth in MHz.
.
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MFLO_NS_TUNE_FREQ_AND_BW( HANDLE hResourceContext, 
															  unsigned long iFreqKhz,
															  unsigned char iBandwidthMHz );

	/******************************************************************************/
	/**
	Factory Test Mode MediaFLO Non-Signalling, 1.3.14 Subcommand 431 - Get RSSI.
	
	This command retrieves the RSSI. This value is not valid when the FLO is not actively decoding.  

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param bRssiValid = specifies validity of the RSSI measurement
	\param iRssi = Signed RSSI value in 1/100th dB resolution.

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_RSSI( HANDLE hResourceContext, 
												      unsigned char *pbRssiValid,
													  short *piRssi );

	/******************************************************************************/
	/**
	Factory Test Mode MediaFLO Non-Signalling, 1.3.15 Subcommand 15 - Set FLO Sleep.
	
	This command Enables/disables FLO sleep.  

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param eSleepSetting = indicates sleep setting. 
		Defined by mftypes_sleep_settings_type:
		\code
		SLEEP_AND_SNOOZE_DISABLED		= 0x00,
		SLEEP_ENABLED_SNOOZE_DISABLED	= 0x01,
		SLEEP_DISABLED_SNOOZE_ENABLED	= 0x02,
		SLEEP_ENABLED_SNOOZE_ENABLED	= 0x03
		\endcode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MFLO_NS_SET_FLO_SLEEP( HANDLE hResourceContext, 
													       unsigned char iSleepSetting );

	/******************************************************************************/
	/**
	Factory Test Mode MediaFLO Non-Signalling, 1.3.16 Subcommand 433 - Get OIS State.
	
	This command returns the state of the WIde and Local OIS.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param piWideOisStatus = Wide OIS Status.
		Defined by mftypes_state_type:
		\code
		INVALID_STATE		= 0x00,
		ACQUIRING_STATE		= 0x01,
		IDLE_STATE			= 0x02,
		ACTIVE_STATE		= 0x03,
		UNAVAILABLE_STATE	= 0x04
		\endcode
	\param piLocalOisStatus = Local OIS Status.
		Defined by mftypes_state_type:
		\code
		INVALID_STATE		= 0x00,
		ACQUIRING_STATE		= 0x01,
		IDLE_STATE			= 0x02,
		ACTIVE_STATE		= 0x03,
		UNAVAILABLE_STATE	= 0x04
		\endcode
.
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_OIS_STATE( HANDLE hResourceContext, 
														 unsigned char *piWideOisStatus,
														 unsigned char *piLocalOisStatus );

	/******************************************************************************/
	/**
	MFLO get active flow Info

	This command gets information regarding all active flows in the system.  

	Calls QLIB_FTM_MFLO_NS_GET_ALL_ACTIVE_FLOW_IDS then calls QLIB_FTM_MFLO_NS_GET_STREAM_INFO
	for each active flow.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param piActiveFlowCount = This is both and input and output param.  As an input param
							  it indicates the number of elements in pActiveFlowInfoArray. 
							  As an output param it indicates the actual number of active 
							  flows in the system.
	\param pActiveFlowInfoArray = Array of mftypes_active_flow_info structures 
									(mftypes_active_flow_info is defined in QLib_Defines.h)

	\return true if successful, false if fail.

	\warning Prior to calling this function, sufficient space should be allocated in
			 pActiveFlowInfoArray to	store information about all active flows in
			 the system.  The required amount of space can be determined by first calling
			 QLIB_MFLO_GetBoundFlowCount.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MFLO_NS_GET_ACTIVE_FLOW_INFO_LIST( HANDLE hResourceContext, 
																	   unsigned char* piActiveFlowCount, 
																	   void* pActiveFlowInfoArray );
	
	/******************************************************************************/
	/**
	This function measures the Physical layer packet error rate.  This type of packet 
	error rate measurement does not require a known nor finite sequence of FTAP packets 
	from the FLO Tx Waveform.  The measurement is made by querying the accumulated PLP 
	statistics for all requested Flow IDs.  Accumulated statistics continue to be 
	queried until the minimum number of PLPs have been recorded.  Accumulated PLP 
	statistics are queried through the FTM_MFLO_NS_GET_MLC_DYN_PARAMS function. 
	
	After calling FTM_MFLO_NS_RESET_ALL_PLP_STATS to reset the accumulated statistics for all
	MLCs, this function may be called repeatedly until the desired level of confidence 
	is achieved. To measure the packet error rate again, call FTM_MFLO_NS_RESET_ALL_PLP_STATS again 
	before calling this function.

	Usage examples:

		FTM_MFLO_NS_RESET_ALL_PLP_STATS	
		FTM_MFLO_NS_GetPER_Phy		--------------	Measure PER for first test
									

		FTM_MFLO_NS_RESET_ALL_PLP_STATS	---------
		FTM_MFLO_NS_GetPER_Phy					 |
		FTM_MFLO_NS_GetPER_Phy					 |-----  Measure PER for second test 
		FTM_MFLO_NS_GetPER_Phy					 |		Note: Example shows measurement ran longer  
		FTM_MFLO_NS_GetPER_Phy			---------		      to achieve desired level of confidence

	This function reports the packet error rate statistics for all requested Flow IDs, 
	the current system state, and an error code.  

	\param iNumberOfFlowIDs = the number of FlowIDs in aiFlowIDs
	\param aiFlowIDs = array of Flow IDs for which to get accumulated PLP statistics
	\param aiMlcIDs = array of MLC IDs that correspond to the Flow IDs in aiFlowIDs,
	                  this is required because MFLO_GET_MLC_PLP_STAT_DYN_PARAMS requires 
					  the MLC ID not Flow ID
	\param aeSystem = array of mftypes_system_type with elements that correspond to the 
	                  Flow IDs in aiFlowIDs, the System for each Flow ID is required by
					  QLIB_MFLO_GET_MLC_PLP_STAT_DYN_PARAMS
	\param iNoStatsTimeout = time in seconds to wait for statistics to update, max of 5 
							 seconds recommended
	\param ePerToMeas = which packet error rate to measure, statistics for both Base and 
	                    Enhancement layers will be reported but the determination of
						whether or not iMinPLPs have been measured depends on what this
						variable has been set to (mftypes_per_to_measure)
	\param iMinPLPs = the minimum number of PLPs to measure for all Flow IDs
	\param eRS = which statistics to report, Pre or Post Reed Solomon (mftypes_reed_solomon)
	\param paOverallStats = an array of type mftypes_per_statistics with length 
							iNumberOfFlowIDs to store the packet error rate statistics for 
							all requested Flow IDs
	\param peWideAreaSystemState = the current wide area system state if one of the 
								   elements in aeSystem is WIDE (mftypes_state_type)
	\param peLocalAreaSystemState = the current local area system state if one of the 
	                                elements in aeSystem is LOCAL (mftypes_state_type)
	\param peErrorCode = an error code used whenever possible to provide more information 
	                     about why the function failed, see the warning below for one
						 exception (mftypes_error_code)

	\return true if successful, false if fail.

	\warning This function does not dynamically allocate memory for the pOverallStats 
			 array.  Sufficient memory for pOverallStats must be allocated by the 
			 calling program.

			 The results reported in paOverallStats are only valid if...
				FTM_MFLO_NS_GetPER_Phy return value is true		AND
				Requested system state(s) == ACTIVE_STATE	AND
			    peErrorCode == NOT_AVAILABLE.

			 If at some point during the measurement, the requested system state is not 
			 ACTIVE_STATE,  the function will return but not fail.  The current system 
			 state will be reported through peWideAreaSystemState and peLocalAreaSystemState.

			 If the statistics for one or more of the MLC IDs in aiMlcIDs have not updated
			 after the number of seconds specified by iNoStatsTimeout have elapsed, the 
			 function will return but not fail. peErrorCode will be set to PLP_STATS_NOT_UPDATING.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_MFLO_NS_GetPER_Phy( HANDLE hResourceContext, unsigned char iNumberOfFlowIDs, 
														unsigned long* aiFlowIDs, unsigned char* aiMlcIDs, 
														unsigned char* aeSystem, unsigned short iNoStatsTimeout, 
														unsigned char ePerToMeas, unsigned long iMinPLPs, 
														unsigned char eRS, void* paOverallStats, 
														unsigned char* peWideAreaSystemState, 
														unsigned char* peLocalAreaSystemState, 
														unsigned char* peErrorCode );
	
/******************************************************************************
						WLAN FTM

  NOTE: Please check the module manfucturer's documentation for specific information
        about the commands or parameters.  The documentation for the module-specific
		commands is based sources and Qualcomm can give very little support about
		individual commands and they can also not add new commands unless the 
		module vendor writes the corresponding firmware and provides the command
		specification.

*******************************************************************************/
	/******************************************************************************/
	/**

	WLAN FTM, Set the WLAN module type

	Set the WLAN module type, to be used for all QMSL WLAN FTM functions except for
	QLIB_FTM_WLAN_USER_CMD().

	If this funciton is not called, then the default type is FTM_WLAN_ModuleType_Philips.

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param eModuletype = 
			Defined by FTM_WLAN_ModuleTypeEnum:
			\code
			FTM_WLAN_ModuleType_Philips	= 0x00,	//!< Philips WLAN module
			FTM_WLAN_ModuleType_Atheros	= 0x01,	//!< Atheros WLAN module
			\endcode
		\param iRequestSize = Number of bytes to be sent in the request packet.
		\param piRequestBytes = Pointer to a buffer of request packet contents.
		\param piResponseSize = Pointer to number of bytes received in the response packet
								NOTE: when calling this function, the value must be equal to the
								maximum size of the receive buffer.  When the function returns, the
								value will be the actual number of bytes filled into the receive buffer.
		\param iResponseBytes = Pointer to a buffer to store the response packet contents.

	\return Returns true if the command was dispatched successfully through the FTM.

	\warning This function only sets QMSL's mode.  The phone must also have a certain
	         NV item written to indicate whether the phone things the module is
			 Atheros or Philips.  The NV item number is 4202. A value of 0 indicates
			 Philips and a value of 1 indicates Atheros.


			These files come from the wlan folder in the bulid and must be put into the 
			phone's EFS folder /brew

				AMSS folder: wlan\oem\phg_80211g\libs

			Finally, in the phone's EFS, you must delete file /brew/mod/netlist

	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_SetModuleType
				(
				HANDLE hResourceContext, 
				unsigned long eModuleType
				);


	/******************************************************************************/
	/**

	WLAN FTM ICD, 2.1 FTM_WLAN_DISPATCH

	Sends a request/response packet to a WLAN module.  The command packets are defined
	by the individual module vendors (e.g. Philips/Atheros).

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iRequestSize = Number of bytes to be sent in the request packet.
		\param piRequestBytes = Pointer to a buffer of request packet contents.
		\param piResponseSize = Pointer to number of bytes received in the response packet
								NOTE: when calling this function, the value must be equal to the
								maximum size of the receive buffer.  When the function returns, the
								value will be the actual number of bytes filled into the receive buffer.
		\param iResponseBytes = Pointer to a buffer to store the response packet contents.

	\return Returns true if the command was dispatched successfully through the FTM.
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_USER_CMD
				(
				HANDLE hResourceContext, 
				unsigned short iRequestSize,
				unsigned char* piRequestBytes,
				unsigned short* piResponseSize,
				unsigned char* piResponseBytes
				);


	/******************************************************************************/
	/**

	Philips WLAN command, START

		This message is defined in [8] but not defined or implemented in the BGW200 
		implementation.

		To avoid any confusion the message is defined and implemented for BGW211. 
		The target always returns PCTI_SUCCESS. The same message is used as CONNECT_DUT.

		Request and confirm have no parameters. In case of SPI and UART the status is always
		PCTI_SUCCESS.

	\param piStatusCode = return status code, defined by FTM_WLAN_Philips_enPctiStatus
	\return true if successful, false if failure
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Philips_START
				(	
					HANDLE hResourceContext,	
					unsigned char* piStatusCode
				);


	/******************************************************************************/
	/**

	Philips WLAN command, STOP

		On reception of the STOP message the target cancels the currently performed test. The confirm
	message is sent when this has been finished. If no test is running the target immediately returns
	with success.

	The STOP request message has no parameters. The confirm message has no other parameters
	than the mandatory status code.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iStatusCode = return status code, defined by FTM_WLAN_Philips_enPctiStatus
	\return true if successful, false if failure
	\warning FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Philips_STOP
				(	
					HANDLE hResourceContext,	
					unsigned char* piStatusCode
				);

	/******************************************************************************/
	/**

	Philips WLAN command, TX_CONTINUOUS

		On TX_CONTINUOUS message the target configures the system to continuously 
		transmit a single sine wave. The offset frequency of this message is configured 
		with the FreqOffs parameter. The confirm message is sent directly after the 
		target has initiated the sine wave.
		
		The transmission stops on the STOP message. In case another test is already 
		performing (e.g.TX_BURST), the target won’t do anything and confirm with 
		the PCTI_FAILURE.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piStatusCode = return status code, defined by FTM_WLAN_Philips_enPctiStatus
	\param iChannelNo  = specifies the channel used for Tx. (1-14)
	\param iAntenna    = Antenna to be used for tx (1, 2). 
	\param iRates      = Rate of transmission (IGNORED)
	\param iMode       = Data mode (IGNORED)
	\param iTxGain     = 0x00-0x0F are used for BGW200. Since this setting cannot be properly
                         mapped to BGW211, PCTI returns PCTI_FAILURE for all values except
                         for 0x10. With 0x10 the target takes the current values that can be
                         configured with TX_POWER_CONTROL (see 3.2.4)

	\param iSignalType = Offset frequency relative to the carrier, defined by FTM_WLAN_Philips_enSignalType 
		\code
           FTM_WLAN_Philips_SINGLETONE_100KHZ    = 1,
           FTM_WLAN_Philips_SINGLETONE_3MHZ      = 2,
           FTM_WLAN_Philips_SINGLETONE_5d5MHZ    = 3,
           FTM_WLAN_Philips_DUALTONE_100_300KHZ  = 4,
           FTM_WLAN_Philips_DUALTONE_3_3d1MHZ    = 5,
           FTM_WLAN_Philips_DUALTONE_5d5_5d6MHZ  = 6,
           FTM_WLAN_Philips_QPSK_0_1_SEQUENCE    = 7,
           FTM_WLAN_Philips_QPSK_PN_SEQUENCE     = 8

		\endcode
	\return true if successful, false if failure
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Philips_TX_CONTINUOUS
				(
					HANDLE hResourceContext,
					unsigned char* piStatusCode,
					unsigned char iChannelNo,
					unsigned char iAntenna,
					unsigned short iRates,
					unsigned char iMode,
					unsigned char iTxGain,
					unsigned short iSignalType
				);

	/******************************************************************************/
	/**

	Philips WLAN command, TX_BURST

	   On TX_BURST the target initiates the transmission of valid WLAN broadcast frames.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piStatusCode = return status code, defined by FTM_WLAN_Philips_enPctiStatus
	\param 	iChannelNo       = This field specifies the channel used for Tx.
	\param 	iAntenna	     = Antenna to be used for tx
	\param 	iRates           = Rate of transmission, defined by FTM_WLAN_Philips_enDataRate
		\code
          // 11b rates backwards compatible with washington
          FTM_WLAN_Philips_RATE_1_MBPS	= 0x0,
          FTM_WLAN_Philips_RATE_2_MBPS	= 0x1,
          FTM_WLAN_Philips_RATE_5_MBPS	= 0x2,
          FTM_WLAN_Philips_RATE_11_MBPS	= 0x3,
          // plcp rate codes used for OFDM rates
          FTM_WLAN_Philips_RATE_6_MBPS	= 11,
          FTM_WLAN_Philips_RATE_9_MBPS	= 15,
          FTM_WLAN_Philips_RATE_12_MBPS	= 10,
          FTM_WLAN_Philips_RATE_18_MBPS	= 14,
          FTM_WLAN_Philips_RATE_24_MBPS	= 9,
          FTM_WLAN_Philips_RATE_36_MBPS	= 13,
          FTM_WLAN_Philips_RATE_48_MBPS	= 8,
          FTM_WLAN_Philips_RATE_54_MBPS	= 12

		\endcode
	\param 	bPreamble        = 0 = short, 
	                           1 = long preamble Only valid for 11b rates, in case of 1Mbps only long preamble is
                               allowed. For short preamble, PCTI returns PCTI_STATUS_FAILURE

	\param 	iPacketLength    = Tx packet length to be used.
								Number of bytes payload (frame body). The maximum length supported for the
								frame body is 1500 bytes. The length of the frame on the air is
								therefore Packet Length + 28 bytes (MAC header + FCS)
	\param 	iPacketInterval  = Tx packet interframe time, units of microseconds
	\param 	iNumberOfPackets = # of Tx packets to be used, 0 = infinite 

	\return true if successful, false if failure
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Philips_TX_BURST
				(
					HANDLE hResourceContext,
					unsigned char* piStatusCode,
					unsigned char iChannelNo,
					unsigned char iAntenna,
					unsigned char iRates,
					unsigned char bPreamble,
					unsigned short iPacketLength,
					unsigned short iPacketInterval,
					unsigned long iNumberOfPackets
				);

	/******************************************************************************/
	/**

	Philips WLAN command, TX_POWER_CONTROL

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piStatusCode = return status code, defined by FTM_WLAN_Philips_enPctiStatus
	\param iTxBB = Global linear baseband scaling factor, the default value is 16384.
					 The scaling factor to get a target baseband backoff can be calculated
					 using the Scaling Equation. The maximum allowed value of 32767 leads to a backoff 
					 of 3dB. Useful backoff values are between 3dB and 30dB.

	\param iTxLF  = LF attenuator setting in the RF IC (0 - 15dB)
	\param iTxRF  = RF attenuator setting in the RF IC (0 - 15dB)

	  Scaling Equation: S = 10 ^[  (B - 3dB)/20 ] * 32768

	\return true if successful, false if failure
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Philips_TX_POWER_CONTROL
				(
					HANDLE hResourceContext,
					unsigned char* piStatusCode,
					unsigned short iTxBB,
					unsigned char iTxLF,
					unsigned char iTxRF
				);

	/******************************************************************************/
	/**

	Philips WLAN command, RX_CONTINUOUS

		On RX_CONTINUOUS the target goes into receive mode. While it is in the receive mode it
		updates the internal RX statistics. All statistics are reset internally before the RX_CONTINUOUS
		mode is started.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piStatusCode = return status code, defined by FTM_WLAN_Philips_enPctiStatus
	\param iChannelNo = This field specifies the channel used for Tx.
	\param iAntenna = Antenna to be used for tx

	\return true if successful, false if failure
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Philips_RX_CONTINUOUS
				(
					HANDLE hResourceContext,
					unsigned char* piStatusCode,
					unsigned char iChannelNo,
					unsigned char iAntenna
				);

	/******************************************************************************/
	/**

	Philips WLAN command, GET_MAC_ADDRESS

		The target reads the burned-in MAC address from the EEPROM and returns the address with the
		confirm message. This should never fail. In case the MAC_ADDRESS is not configured it returns
		0x424242424242.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piStatusCode = return status code, defined by FTM_WLAN_Philips_enPctiStatus
	\param aiMAC_Address = Buffer to store MAC address, size is defined by FTM_WLAN_MAC_ADRESS_SIZE

	\return true if successful, false if failure
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Philips_GET_MAC_ADDRESS
				(
					HANDLE hResourceContext,
					unsigned char* piStatusCode,
					unsigned char aiMAC_Address[6]
				);

	/******************************************************************************/
	/**

	Philips WLAN command, SET_MAC_ADDRESS

		If there is no MAC_ADDRESS in the EEPROM, SET_MAC_ADDRESS appends a new
		information element containing the given address to the customer section of the EEPROM.
		If there is already a MAC_ADDRESS in the EEPROM, SET_MAC_ADDRESS replaces the old
		one with the given new one.

		If there is no valid VERSION field in the EEPROM, SET_MAC_ADDRESS appends a new
		VERSION information element containing the main version number associated with the firmware.
		This specification is valid for PCTI v1.36 or higher.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piStatusCode = return status code, defined by FTM_WLAN_Philips_enPctiStatus
	\param aiMAC_Address = Buffer containing MAC address to be programmed.  
	                       Size is defined by FTM_WLAN_MAC_ADRESS_SIZE

	\return true if successful, false if failure
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Philips_SET_MAC_ADDRESS
				(
					HANDLE hResourceContext,
					unsigned char* piStatusCode,
					unsigned char aiMAC_Address[6]
				);

	/******************************************************************************/
	/**

	Philips WLAN command, SET_MODULE

		The SET_MODULE message is used to indicate to the target what kind of module it is working
		on. This may affect some internal settings. Currently there is only BGW211 defined for SA5253
		based modules.

		On the SET_MODULE message the target returns PCTI_SUCCESS in the case that the module
		type is BGW211. For all other values it returns PCTI_NOT_SUPPORTED.

		The SET_MODULE request message carries a single byte indicating the module type. 
		PCTI currently only supports the BGW211 module. The confirm message only contains the
		status.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piStatusCode = return status code, defined by FTM_WLAN_Philips_enPctiStatus
	\param iModule = Module id, defined by FTM_WLAN_Philips_enModuleType
		\code
          // Washington modules (not supported) 
          FTM_WLAN_Philips_M1		= 0x00,
          FTM_WLAN_Philips_M2		= 0x01,
          FTM_WLAN_Philips_M3		= 0x02,
          FTM_WLAN_Philips_M4		= 0x03,
          FTM_WLAN_Philips_MWAmax	= 0x0F, 

          // Georgia modules 
          FTM_WLAN_Philips_M_BGW211	= 0x10

		\endcode

	\return true if successful, false if failure
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Philips_SET_MODULE
				(
					HANDLE hResourceContext,
					unsigned char* piStatusCode,
					unsigned char iModule
				);

	/******************************************************************************/
	/**

	Philips WLAN command, GET_HWSW_VERSIONS

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piStatusCode = return status code, defined by FTM_WLAN_Philips_enPctiStatus
	\param piSwVer       = Major version in upper byte, minor version in lower byte
	\param piHwBBChipVer = Baseband IC version
	\param piHwRFChipVer = RF Chip version
	\param iEEPROMVerFirmware = Version expected and written by the firmware
	\param iEEPROMVerManufacture = Version found in the manufacturer section of the EEPROM
	\param iEEPROMVerCustomer = Version found in the customer section of the EEPROM

	\return true if successful, false if failure
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Philips_GET_HWSW_VERSIONS
				(
					HANDLE hResourceContext,
					unsigned char* piStatusCode,
					unsigned short* piSwVer,
					unsigned short* piHwBBChipVer,
					unsigned short* piHwRFChipVer,
					unsigned short*	piEEPROMVerFirmware,
					unsigned short*	piEEPROMVerManufacture,
					unsigned short* piEEPROMVerCustomer
				);

	/******************************************************************************/
	/**

	Philips WLAN command, TX_POWER_SET_CALIBRATED

		On TX_POWER_SET_CALIBRATED the target uses the calibration data as stored in the internal
		EEPROM to set the TX output power at the antenna to the value requested in the message.

		In the case that no valid calibration data is stored in the EEPROM the target returns
		PCTI_FAILURE.

		The request message carries a single signed byte for the targeted output power in dBm. The full
		range is valid. If the specified power is larger than the calibrated power, the latter will be used. If
		the specified power is lower than the minimum possible power, which is at least 20 dB below the
		calibrated power, the minimum power will be used.


	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piStatusCode = return status code, defined by FTM_WLAN_Philips_enPctiStatus
	\param iPower_dBm = Tx power in dBm
	\param iAddress = Address returned from confirm message body

	\return true if successful, false if failure
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Philips_TX_POWER_SET_CALIBRATED
				(
					HANDLE hResourceContext,
					unsigned char* piStatusCode,
					unsigned char iPower_dBm,
					unsigned long* piAddress
				);

	/******************************************************************************/
	/**

	Philips WLAN command, READ_STATISTICS (partial)

		Parses enough of the READ_STATISTICS result to return the RSSI
		levels.  Use the QLIB_FTM_WLAN_Philips_READ_STATISTICS_Partial() command 
		to return the entire READ_STATISTICS structure.

		On READ_STATISTICS the target collects the internal counter in a structure 
		and returns it with the confirm message and status PCTI_SUCCESS.

		Only a few of the available parameters are returned through the C interface.  
		 
		If the full packet response is desired, then the user command should be called.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piStatusCode = return status code, defined by FTM_WLAN_Philips_enPctiStatus
	\param piNumRxFrames     - CRCOKA1
	\param piNumRxBytesOk    - CumulatedNumRx - BytesCRCOKA1
	\param piRSSI_OK         - BBRSSICumulated - AllRxCRCOKA1
	\param piRSSI_NOK        - BBRSSICumulated - AllRxCRCErrorA1
	\param piNumRxFrames_ant2     - CRCOKA2
	\param piNumRxBytesOk_ant2    - CumulatedNumRx - BytesCRCOKA2
	\param piRSSI_OK_ant2         - BBRSSICumulated - AllRxCRCOKA2
	\param piRSSI_NOK_ant2        - BBRSSICumulated - AllRxCRCErrorA2



	\return true if successful, false if failure
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Philips_READ_STATISTICS_Partial
				(
					HANDLE hResourceContext,
					unsigned char* piStatusCode,
					unsigned long* piNumRxFrames_ant1, 
					unsigned long* piNumRxBytesOk_ant1,
					unsigned long* piRSSI_OK_ant1,
					unsigned long* piRSSI_NOK_ant1,
					unsigned long* piNumRxFrames_ant2, 
					unsigned long* piNumRxBytesOk_ant2,
					unsigned long* piRSSI_OK_ant2,
					unsigned long* piRSSI_NOK_ant2
				);

	/******************************************************************************/
	/**

	Philips WLAN command, READ_STATISTICS (full)

		Reads the entire READ_STATISTICS result and copies it into a user defined
		buffer.  The parsing must be done by the user.

		To get RSSI only fields, use QLIB_FTM_WLAN_Philips_READ_STATISTICS_Partial()

		On READ_STATISTICS the target collects the internal counter in a structure 
		and returns it with the confirm message and status PCTI_SUCCESS.

		Only a few of the available parameters are returned through the C interface.  
		This is done to focus on the Antenna 1 receiver level measurements.  
		
		If the full packet response is desired, then the user command should be called.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piStatusCode = return status code, defined by FTM_WLAN_Philips_enPctiStatus
	\param pReadStatisticsUserBuffer - pointer to 128 bytes, into which the READ_STATISTICS 
		   structure will be copied.  Size of buffer is defined by 
		   FTM_WLAN_Philips_PCTI_READ_STATISTICS_RESPONSE_SIZE


	\return true if successful, false if failure
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Philips_READ_STATISTICS_Full
				(
					HANDLE hResourceContext,
					unsigned char* piStatusCode,
					void* pReadStatisticsUserBuffer
				);


	/******************************************************************************/
	/**

	Philips WLAN command, RESET_STATISTICS

		Reset the statistics reutrued by READ_STATISTICS

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piStatusCode = return status code, defined by FTM_WLAN_Philips_enPctiStatus

	\return true if successful, false if failure
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Philips_RESET_STATISTICS( 
		HANDLE hResourceContext,
		unsigned char* piStatusCode
		);

	/******************************************************************************/
	/**

	Philips WLAN command, SLEEP_MODE

		Used to initiate Sleep Mode Setting for Power Save. The parameters specify
		modes like Sleep between beacons, sleep between DTIM beacons or wake after a 
		specified time. Power save during IBSS is also specifiable.


	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piStatusCode = return status code, defined by FTM_WLAN_Philips_enPctiStatus
	\param char iMode, defined by FTM_WLAN_Philips_enSleepMode
	  \code

        FTM_WLAN_Philips_PCTI_SLEEP_DOWN = 0x0,
		0 = Go to sleep and awake with the next message from the host. Similar to
			SLEEP. Wake-up will not work over UART. The confirm is sent from the
			target after and of sleep. That means, in case of UART, never, in case of SPI
			before the command that woke up the target is processed.

        FTM_WLAN_Philips_PCTI_TIMED_SLEEP = 0x1
		1 = Go to sleep and wake up after a defined time. The request is confirmed after
			wake-up or directly in case of an error.
	  \endcode
	\param iSleepTime_ms = sleep time in ms, when iMode = 1
	\param iReserved = reserved


	\return true if successful, false if failure
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Philips_SLEEP_MODE
	(
		HANDLE hResourceContext,
		unsigned char* piStatusCode,
		unsigned char iMode,
		unsigned long iSleepTime_ms,
		unsigned char iReserved
	);


	/******************************************************************************/
	/**

	Philips WLAN command, LOOPBACK_MODE

		The LOOPBACK_MODE message enables a loopback behind the modem. Frames received
		from the air are completely decoded including error correction and sent back to the air with the
		same data rate after a programmable amount of time.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piStatusCode = return status code, defined by FTM_WLAN_Philips_enPctiStatus
	\param 	iChannel = Channel to operate on
	\param  iIfs     = Time in ìs between end of the received frame and start of the transmission,
	                   Units are ms

	\return true if successful, false if failure
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Philips_LOOPBACK_MODE
	(
		HANDLE hResourceContext,
		unsigned char* piStatusCode,
		unsigned char iChannel,
		unsigned long iIfs
	);

	/******************************************************************************/
	/**

	Philips WLAN command, DEBUG_LEVEL

		The DEBUG_LEVEL messages configures the amount of debug messages sent over the UART
		interface. This may help customers to provide detailed information in case of problems. The
		message carries a bitmask where different kinds of debug messages can be separately
		configured. The debug output is enabled if the associated bit is set to 1. Per default (at startup),
		debug outputs are disabled.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piStatusCode = return status code, defined by FTM_WLAN_Philips_enPctiStatus
	\param 	iBitmask = Channel to operate on
	  \code
		 Bit 0 PCTI command handler verbose, show only PCTI command related prints
		 Bit 1 Global verbose, show all prints
		 Bitmask
		 Bit 2-15 Reserved (unused)
	  \endcode

	\return true if successful, false if failure
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Philips_DEBUG_LEVEL
	(
		HANDLE hResourceContext,
		unsigned char* piStatusCode,
		unsigned char iBitmask
	);

	/******************************************************************************/
	/**

	Philips WLAN command, CHECK_EXT_32KHZ_CLOCK

		Returns the status of the 32kHz clock

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piStatusCode = return status code, defined by FTM_WLAN_Philips_enPctiStatus
	\param 	iStatus = 
		\code
		   0 = External clock is not available / running
		   1 = External clock is available / running
		\endcode

	\param  piClockStatus, output,
		\code
		   0 = External clock is not available / running
		   1 = External clock is available / running
		\endcode


	\return true if successful, false if failure
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Philips_CHECK_EXT_32KHZ_CLOCK
	(
		HANDLE hResourceContext,
		unsigned char* piStatusCode,
		unsigned char* piClockStatus
	);

	/******************************************************************************/
	/**

	Philips WLAN command to get the status value of the last QLIB_FTM_WLAN_Philips()
	function that was called. This is needed for QRCT because QRCT will not update
	the output status value in the case of a failure.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piStatusCode = returns the status code of the last Philips WLAN function, 
	                      defined by FTM_WLAN_Philips_enPctiStatus

	\return true if successful, false if failure
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Philips_GetLastStatus
	(
		HANDLE hResourceContext,
		unsigned char* piLastStatusCode
	);

	/*
	 Notes on testing WLAN commands (May 22, 2006)
		QLIB_FTM_WLAN_Philips_SLEEP_MODE
		-	mode=SLEEP_DOWN fails, the phone does not send a response packet.  
		    This is probably because the module has gone to sleep and is not communicating with the ARM

		-	mode=SLEEP_TIMED, fails with Philips status code = 1 


		QLIB_FTM_WLAN_Philips_LOOPBACK_MODE
		- Cannot be called after other Tx functions, like QLIB_FTM_WLAN_Philips_TX_CONTINUOUS

		QLIB_FTM_WLAN_Philips_RX_CONTINUOUS
		-	Cannot be called after other Tx functions, like QLIB_FTM_WLAN_Philips_TX_CONTINUOUS 
		    and  QLIB_FTM_WLAN_Philips_LOOPBACK_MODE


		QLIB_FTM_WLAN_Philips_TX_BURST
		-	Problem with rate=0 (1_MBPS), reported status is value 1 instead of 0

		QLIB_FTM_WLAN_Philips_READ_STATISTICS_Partial
		-	Not sure if this function is working because it has not been tested with WLAN equipment

		QLIB_FTM_WLAN_Philips_SET_MODULE
		- Philips command does not return a status value, so the value reported by QMSL is forced to 
		  FTM_WLAN_Philips_PCTI_FAILURE

	*/

	/*
		These Philips commands are not enabled with a specific C interface, but it is 
		possible to call QLIB_FTM_WLAN_USER_CMD().

		DISCONNECT_DUT
		CONNECT_DUT
		GET_TEMPERATURE
		SET_CALIBRATIONINFO
		GET_CALIBRATIONINFO 
		EEPROM_READ
		EEPROM_WRITE 
		SET_TX_DATA
		MEMORY_READ
		MEMORY_WRITE
		SB_REGISTER_WRITE
		RF_REGISTER_READ 
		RF_REGISTER_WRITE
		COEX_LINE_READ
		COEX_LINE_WRITE
	*/

	/******************************************************************************/
	/**

	Atheros WLAN command to enable/disable continuous transmit

	Note: Enable/disable continuous tx test cmd works only when target is awake.

	\param iMode =	0 = Disabling continuous Tx, 
					1 = Enabling continuous unmodulated Tx
					2 = Enabling continuous modulated Tx

		   iFreq = Transmit Frequency in MHz
		           For example, 2412 for channel 1 in 802.11g 

		   iDataRate =  0 - 1 Mbps
						1 - 2 Mbps
						2 - 5.5 Mbps
						3 - 11 Mbps
						4 - 6 Mbps
						5 - 9 Mbps
						6 - 12 Mbps
						7 - 18 Mbps
						8 - 24 Mbps
						9 - 36 Mbps	
						10 - 48 Mbps
						11 - 54 Mbps
		  
		  iTxPwr = Tx Power in dBm 
					5 - 11 dBm for unmodulated Tx
					5 - 14 dBm for modulated Tx
		   
	      iAntenna 1 = antenna one
		           2 = antenna two
	
	\return true if successful, false if failure
	
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_CONT_TRANSMIT
	(	
		HANDLE hResourceContext,
		unsigned long iMode, 
		unsigned long iFreq, 
		unsigned long iDataRate, 
		unsigned long iTxPwr, 
		unsigned long iAntenna
	);


	/******************************************************************************/
	/**

	Atheros WLAN command to enable continuous receive

	\param iAction =	0 = promiscuous mode (accept all incoming frames)
					1 = filter mode (accept only frames with dest address equal specified mac address
				
		   iFreq = Receive Frequency in MHz
		           For example, 2412 for channel 1 in 802.11g 

	      iAntenna 1 = antenna one
		           2 = antenna two
	
	\return true if successful, false if failure
	
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_CONT_RECEIVE
	( 
			HANDLE hResourceContext,
			unsigned long iAction, 
			unsigned long iFreq, 
			unsigned long iAntenna
	);

	/******************************************************************************/
	/**

	Atheros WLAN command to get receive report

	\param iTotalPkt =	Total packet received

		   iRSSI = RSSI

	\return true if successful, false if failure
	
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_RECEIVE_REPORT
	( 
			HANDLE hResourceContext,
			unsigned long* iTotalPkt, 
			unsigned long* iRSSI
	);
	/******************************************************************************/
	/**

	Atheros WLAN command to set MAC address

	\param iMAC_Address  =	MAC address

	\return true if successful, false if failure
	
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_SET_MAC_ADDRESS
	( 
			HANDLE hResourceContext,
			unsigned char aiMAC_Address[6] 
	);

	/******************************************************************************/
	/**

	Atheros WLAN command to force SLEEP or AWAKE

	\param iMode  =	1 = Wake up target
					2 = Force the target to sleep

	\return true if successful, false if failure
	
	\warning QLIB_FTM_WLAN_SetModuleType() must be called first to set the WLAN module type
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_SLEEP_WAKE
	(
		HANDLE hResourceContext,
		unsigned char iMode	
	);

/******************************************************************************
						QFUSE FTM
*******************************************************************************/

	/******************************************************************************/
	/**

	QFUSE Programming 2.1 FTM_FUSE_READ

	\param	iChainSelect =	0 = Configuraiton chain
				
			iWriteStatus =	0 = Success
							1 = Read operation Failure
							2 = Write operation Failure
							3 = Operation on selected chain not supported

			aConfigRegister = an array of 5 elements and size of each element is 4 bytes.

	\return true if successful, false if failure
	
	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_QFUSE_READ
	(
		HANDLE hResourceContext,
		unsigned short iChainSelect,
		unsigned short* iReadStatus,
		unsigned long   aConfigRegister[5]
	);


	/******************************************************************************/
	/**

	QFUSE Programming 2.2 FTM_FUSE_WRITE

	\param	iChainSelect =	0 = Configuraiton chain
				
			iWriteStatus =	0 = Success
							1 = Read operation Failure
							2 = Write operation Failure
							3 = Operation on selected chain not supported

			aConfigRegister = an array of 5 elements and size of each element is 4 bytes.

	\return true if successful, false if failure
	
	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_FTM_QFUSE_WRITE
	(
		HANDLE hResourceContext,
		unsigned short	iChainSelect,
		unsigned short* iWriteStatus,
		unsigned long   aConfigRegister[5]
	);
/******************************************************************************
						EFS Functions
*******************************************************************************/


	/******************************************************************************/
	/**
	Initiates an EFS directory on a folder and calls a call-back function for each 
	entry in the folder (both files and folders)

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param sFolderPath = must be in the format "folder1/folder2" without a leading "/"
	\return none

	\warning If callback function is not supplied, data will not be returned to the calling program.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_EfsDirectory( HANDLE hResourceContext, char* sFolderPath );



	/******************************************************************************/
	/**

	Write file from computer to phone

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param sLocalFile	= absolute path of file on PC
		\param sEFS_File		= absolute path of file to write on phone,
		                          must be in the format "folder/file.ext" without a leading "/"

	\return true if successful, false if failure

	*******************************************************************************/
	QLIB_API unsigned char QLIB_EfsWrite( HANDLE hResourceContext, char *sLocalFile, char *sEFS_File );

	/******************************************************************************/
	/**

	Write file from computer to phone using a RAM buffer isntead of a file.

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param sLocalMem	    = buffer pointer to file contenet keeping in memory on PC
		\param iSize		    = buffer size
		\param sEFS_File		= absolute path of file to write on phone,
		                          must be in the format "folder/file.ext" without a leading "/"

	\return true if successful, false if failure
	*******************************************************************************/
	QLIB_API unsigned char QLIB_EfsWriteMem( HANDLE hResourceContext, char *sLocalMem, unsigned long iSize, char *sEFS_File );

	/******************************************************************************/
	/**

	Read file from phone

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param sEFS_File        = absolute path of file on phone,
		                          must be in the format "folder/file.ext" without a leading "/"
		\param sLocalFile       = absolute path of file to create on PC

	Comments:  creates attr file is the same file path as data but with .att appended

	\return true if successful, false if failure

	*******************************************************************************/
	QLIB_API unsigned char QLIB_EfsRead( HANDLE hResourceContext, char *sEFS_File, char *sLocalFile );

	/******************************************************************************/
	/**

	Set the size of the payload data to be used when requesting an EFS Read or Write.
	The higher this number, the fewer request/response packets will be needed to complete
	an EFS operation.

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iEFS_Read_Size = size of the payload of data used for the EFS read request packet.
		                        The size should be less than DIAG_MAX_PACKET_SIZE minus the packet
								response header for an EFS read. Default value is 524.

		\param iEFS_Write_Size = size of the payload of data used for the EFS write request packet.
		                        The size should be less than DIAG_MAX_PACKET_SIZE minus the packet
								request header for an EFS write.  Default is 536

	Comments:  creates attr file is the same file path as data but with .att appended

	\return true if successful, false if failure

	\warning The packet size has dependancies on the transport layer (QPST + Diagnostic handler in embedded software)
	         If the size is set to a point that is <DIAG_MAX_PACKET_SIZE, but the request is failing, then the problem
			 is most likely in one of these physical layer handlers and not in QMSL.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_SetEfsReadAndWritePacketSize( 
							HANDLE hResourceContext, 
							unsigned short iEFS_Read_Size, 
							unsigned short iEFS_Write_Size );

	/******************************************************************************/
	/**

	Delete file from phone

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param sFileName  - absolute path of file on phone,
							must be in the format "folder/file.ext" without a leading "/"

	Comments:   Delete takes the phone offline, so you need to reset afterwards

	\return true if successful, false if failure
	 

	\warning    This function is mapped into the diagnostic class, so that they will 
	            be available when QPST is not used.  Given this, they will not return
				error information via the EFS interface--rather the error information
				will be returned via the diagnostic interface.


	*******************************************************************************/
	QLIB_API unsigned char QLIB_EfsDelete( HANDLE hResourceContext, char *sFileName );


	/******************************************************************************/
	/**

	Sets the permission bits on a file on the phone

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param sFileName =  Absolute path of file on phone
		\param oAttr  = Attributes to set the file to

	Return Value:
		true if successful, false if failure

	\return true if successful, false if failure

	\warning  This function is not currently implemented.  If called, an error will be returned.

	*******************************************************************************/
	//unsigned char QLIB_EfsSetPermissions( char *sFileName, EfsDirItem* oAttr ) ;
	// bn: Oct 8, 2004, Disabled for Oct 15 release because more information is needed about this operation


	/******************************************************************************/
	/**

	Create a folder in the EFS file system

	Parameter(s):

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param sEFS_Folder = absolute path of dir to create on phone.
						 So to make the dir "temp" off of the dir 'brew'
						 which is at root do "brew/temp" as the path
						 must be in the format "folder/folder2" without a leading "/"

					

	\return true if successful, false if failure

	*******************************************************************************/
	QLIB_API unsigned char QLIB_EfsMkDir( HANDLE hResourceContext, char *sEFS_Folder );


	/******************************************************************************/
	/**

	Remove directory from phone

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param sEFS_Folder      - absolute path of dir to create on phone.
						 So to remove the dir "temp" off of the dir 'brew'
						 which is at root, do "brew/temp" as the path
						 must be in the format "folder1/folder2" without a leading "/"

	\return true if successful, false if failure

	\warning    This function is mapped into the diagnostic class, so that they will 
	            be available when QPST is not used.  Given this, they will not return
				error information via the EFS interface--rather the error information
				will be returned via the diagnostic interface.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_EfsRmDir( HANDLE hResourceContext, char *sEFS_Folder );

	/******************************************************************************/
	/**

	Remove a directory tree from the phone's EFS

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param sEFS_Folder      - absolute path of dir to create on phone.
						 So to remove the dir "temp" off of the dir 'brew'
						 which is at root, do "brew/temp" as the path
						 must be in the format "folder1/folder2" without a leading "/"

	\return true if successful, false if failure

	\warning    This function is mapped into the diagnostic class, so that they will 
	            be available when QPST is not used.  Given this, they will not return
				error information via the EFS interface--rather the error information
				will be returned via the diagnostic interface.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_EfsRmTree( HANDLE hResourceContext, char *sEFS_Folder );

	/******************************************************************************/
	/**
	EFS2, 3.16 EFS2_DIAG_RENAME request/response

	This is used to rename a file or directory. Note that duplicate requests to rename a directory will 
	lead to an error code being returned the second time if the rename was successful the first time. If 
	the duplicate request is due to a missing response packet, the tool needs to check the error code to 
	make sure that the error is reasonable (ENOENT will be the error returned in this case).

	NOTE If a request is received to remove an already removed directory, nothing is done 
	but an error is	not reported.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piErrorNo = Error code if error; otherwise, 0
	\param sOldFilePath = Old path name of file, for example "FactoryTest/Pictures/1.jpg" 
						This is case sensitive.
						must be in the format "folder/file.ext" without a leading "/"

	\param sNewFilePath = New path name of file, for example "FactoryTest/Pictures/1.jpg" 
						This is case sensitive.
						must be in the format "folder/file.ext" without a leading "/"
		
	\return True if successful, false if the command fails

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_EFS2_DIAG_RENAME( 
		HANDLE hResourceContext, char* sOldFilePath, char* sNewFilePath, unsigned long* piErrorNo ); 

	/******************************************************************************/
	/**
	EFS2, 3.17 EFS2_DIAG_STAT request/response

	This is used to retrieve information about a named file or directory.

	\param sFilePath = File, for example "FactoryTest/Pictures/1.jpg" 
						This is case sensitive.
						must be in the format "folder/file.ext" without a leading "/"

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piErrorNo = Error code if error; otherwise, 0
	\param piMode = File mode
	\param piSize = File Size
	\param piNlink = Number of links
	\param piAtime = Time of last access
	\param piMtime = Time of last modify
	\param piCtime = Time of last change
	\return True if successful, false if the command fails

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_EFS2_DIAG_STAT( 
		HANDLE hResourceContext, char* sFilePath, unsigned long* piErrorNo, unsigned long* piMode, unsigned long* piSize, 
		unsigned long* piNlink, unsigned long* piAtime, unsigned long* piMtime, unsigned long* piCtime ); 

	/******************************************************************************/
	/**
	EFS2, 3.20 EFS2_DIAG_CHMOD request/response

	This is used to change the access permissions of a file or directory.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piMode = New Mode
	\param sFilePath = File, for example "FactoryTest/Pictures/1.jpg" 
						This is case sensitive.
						must be in the format "folder/file.ext" without a leading "/"
	\param piErrorNo = Error code if error; otherwise, 0

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_EFS2_DIAG_CHMOD( 
		HANDLE hResourceContext, unsigned short iMode, char* sFilePath, unsigned long* piErrorNo); 

	/******************************************************************************/
	/**
	EFS2, 3.21 EFS2_DIAG_STATFS request/response

	This is used to retrieve information about a file system. Note that the request packet must specify 
	a path to a file or directory. Information is returned about the file system containing the named
	file or directory.


	\param iMode = New Mode
	\param sFilePath = File, for example "FactoryTest/Pictures/1.jpg" 
						This is case sensitive.
						must be in the format "folder/file.ext" without a leading "/"

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piErrorNo = Error code if error; otherwise, 0
	\param piFS_ID = File system ID
	\param piFS_Type = File system type
	\param piBlockSize = File system block size
	\param piTotalBlocks = File system size in bytes
	\param piAvailBlocks = Blocks available in file system 
	\param piFreeBlocks = Blocks free in file system
	\param piMaxFileSize = Maximum size of a file in this file system
	\param piNfiles = Current number of files in this file system
	\param piMaxNfiles = Maximum number of files in this file system

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_EFS2_DIAG_STATFS( 
		HANDLE hResourceContext, char* sFilePath, unsigned long* piErrorNo, unsigned long* piFS_ID, 
		unsigned long* piFS_Type, unsigned long* piBlockSize, 
		unsigned long* piTotalBlocks, unsigned long* piAvailBlocks, unsigned long* piFreeBlocks, unsigned long* piMaxFileSize,
		unsigned long* piNfiles, unsigned long* piMaxNfiles); 

	/******************************************************************************/
	/**
	EFS2, 3.22 EFS2_DIAG_ACCESS request/response

	Check a named file for accessibility. This is used to check access 
	permissions for a file or directory.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param iPermissionBits = Permissions bits
	\param sFilePath = File, for example "FactoryTest/Pictures/1.jpg" 
						This is case sensitive.
						must be in the format "folder/file.ext" without a leading "/"

	\param piErrorNo = Error code if error; otherwise, 0

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_EFS2_DIAG_ACCESS( HANDLE hResourceContext, unsigned short iPermissionBits, char* sFilePath, unsigned long* piErrorNo); 

	/******************************************************************************/
	/**
	EFS2, 3.23 EFS2_DIAG_DEV_INFO request/response

	This command is used to request the specifications of the flash device being used in the phone.

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param piErrorNo = Error code if error; otherwise, 0
	\param piTotalNumOfBlocks = Total number of blocks in the device
	\param piNumPagesPerBlock = Number of pages in a block
	\param piPageSize = Size of page data region in bytes
	\param piTotalPageSize = Size of total page_size
	\param piMakerId = Device maker ID
	\param piDeviceId = Device ID
	\param piDeviceType = 0 indicates NOR device, 1 indicates NAND
	\param sDeviceName = Beginning of the string containing name of the device (null terminated string)

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_EFS2_DIAG_DEV_INFO( 
		HANDLE hResourceContext, unsigned long* piErrorNo, unsigned long* piTotalNumOfBlocks, 
		unsigned long* piNumPagesPerBlock, unsigned long* piPageSize, unsigned long* piTotalPageSize, 
		unsigned long* piMakerId, unsigned long* piDeviceId, unsigned char* piDeviceType, char* psDeviceName ); 

	/******************************************************************************/
	/**
	EFS2,   EFS2_DIAG_READLINK request/response

	this is used to retrieve path name pointed out by a link.


	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param sPathName =	path name of link file, for example "VoiceDB/Lang/English/engdat1.dtw" 
						this is case sensitive.
						must be in the format "folder/file.ext" without a leading "/"
	
	\param sPointedFilePath = received path pointed out by a link
	\return true if successful, false if the command fails

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_EfsReadLink( HANDLE hResourceContext, char *sEFS_Path, char *sEFS_Pointed_Path);

	/******************************************************************************/
	/**
	EFS2,   EFS2_DIAG_CREATE_LINK request/response

	this is used to create link

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param sPathName =	path name of link file, for example "VoiceDB/Lang/English/engdat1.dtw" 
						this is case sensitive.
						must be in the format "folder/file.ext" without a leading "/"
	
	\param sPointedFilePath = path pointed by a link
	\return true if successful, false if the command fails

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_EfsCreateLink( HANDLE hResourceContext, char *sEFS_Path, char *sEFS_Pointed_Path);

	/******************************************************************************/
	/**
	EFS2,   EFS2_DIAG_EXTENDED_INFO request/response

	this is used to request EFS information.  The user must supply a query type, and
	the function will perform the query and return either an integer or a string,
	depending upon the query type.


	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

	\param iQueryType = type of query to perform. Values are defined by EFS2_EXT_Kind_Enum in QLib_Defines.h

	\param piReturnDataType = output, data type (int or string) that is being returned.  
	                          Values are defined by EFS2_EXT_Kind_Enum in QLib_Defines.h

	\param piReturnInt = output, holds integer return values

	\param sReturnString = output, holds string return values
	
	\return true if successful, false if the command fails

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_EFS2_DIAG_EXTENDED_INFO( 
							HANDLE hResourceContext, 
							unsigned long iQueryType,
							unsigned long* piReturnDataType, 
							unsigned long* piReturnInt,
							char sReturnString[32]
							);

	/******************************************************************************/
	/**

	Download a CEFS file from the phone to the PC
	
	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
	\param sLocalFile = absolute path of file to create on PC
  
	\return true if successful, false if fail.

	\warning 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_DownloadCEFS_File( HANDLE hResourceContext, char* sLocalFile );


	/******************************************************************************/
	/**

	Function:  DownloadPRL

	Description:  Copy PRL from phone to PC


	Parameter(s):
		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iNAM_Index = NAM index for the PRL
		\param sPRL_Path = path of PRL file
		
		\return true if successful, false if failure

		\warning  

	*******************************************************************************/
	QLIB_API unsigned char QLIB_DownloadPRL( HANDLE hResourceContext, short iNAM_Index, char* sPRL_Path );

	/******************************************************************************/
	/**

	Function:  UploadPRL

	Description:  Copy PRL from PC to phone


	Parameter(s):
		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param iNAM_Index = NAM index for the PRL
		\param sPRL_Path = path of PRL file
		
		\return true if successful, false if failure

		\warning  

	*******************************************************************************/
	QLIB_API unsigned char QLIB_UploadPRL( HANDLE hResourceContext, short iNAM_Index, char* sPRL_Path );

	/******************************************************************************/
	/**
	Aborts the most recent EFS download operation.  Will only affect these functions:
		QLIB_EfsWrite()
		QLIB_EfsRead()

	\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer().

	
	
	\return none

	\warning Any current operations will be terminated.  This can cause problems with the
	         phone state.  
	*******************************************************************************/
	QLIB_API unsigned char QLIB_AbortEFS_Operation( HANDLE hResourceContext );

	/******************************************************************************/
	/**


	Get the error status and description of the last EFS operation.  The only operations
	that use this mechanism are:
		QLIB_DownloadPRL()
		QLIB_UploadPRL()
		QLIB_DownloadCEFS_File()
		QLIB_EfsRmTree()

		\param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
		\param bErrorOccurred    = true if an error occurred during the last operation.  False if no error
								   occurred during the last operation.

		\param sErrorMessage     = Filled filled in with a text description of the most recent error.
								   Empty if no error has occurred.  

		\param iMaxStringLength  = Maximum number of characters available in the sErrorMessage buffer

		\return true if successful, false if failure

		\warning  The error information will be cleared after this is called.

	*******************************************************************************/
	QLIB_API void QLIB_GetLastEfsErrorInfo
		( HANDLE hResourceContext, unsigned char* bErrorOccurred, char *sErrorMessage, int iMaxStringLength );


/******************************************************************************
						MediaFLO DIAG
*******************************************************************************/

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.1 MFLO reset PLP statistics
	This command resets the PLP statistics for either one MLC or all MLCs.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param bResetAll = indicates which PLP statistics to reset
					   true resets PLP statistics for all MLCs, 
					   false resets PLP statistics for the requested MLC
	\param iMlcId = the MLC ID for which to reset the PLP statistics,
					this parameter only applies if bResetAll is set to false
	\param eSystem = the system (mftypes_system_type),
					 this parameter only applies if bResetAll is set to false
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_RST_PLP_STATS(	HANDLE hResourceContext,
													unsigned char bResetAll,
													unsigned char iMlcId,
													unsigned char eSystem );

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.2 MFLO get FLO state

	This command gets the current system state for the specified system.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param eSystem = the system (mftypes_system_type),
	\param peSystemState = the current system state (mftypes_state_type)
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_GET_FLO_STATE(	HANDLE hResourceContext,
													unsigned char eSystem,
													unsigned char* peSystemState );

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.3 MFLO start FLO

	This command sends the request to acquire the system.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_START_FLO(	HANDLE hResourceContext );

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.4 MFLO get FLO version

	This command gets the FLO HW and SW version information.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param piHWmajor = the HW Major Number
	\param piHWminor = the HW Minor Number
	\param piHWpatch = the HW Patch Number
	\param piSWmajor = the SW Major Number
	\param piSWminor = the SW Minor Number
	\param piSWpatch = the SW Patch Number
	\param piSWbuild = the SW Build Number
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_GET_FLO_VERSION_INFO(	HANDLE hResourceContext, 
															unsigned char* piHWmajor,
															unsigned char* piHWminor, 
															unsigned char* piHWpatch,
															unsigned char* piSWmajor, 
															unsigned char* piSWminor, 
															unsigned char* piSWpatch, 
															unsigned char* piSWbuild );

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.5 MFLO activate flow

	This command activates the specified flow.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param iFlowID = the Flow ID
	\param eBinding = the binding value (mftypes_binding_type)
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_ACTIVATE_FLOW( HANDLE hResourceContext, 
													unsigned long iFlowID, 
													unsigned char eBinding );

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.6 MFLO deactivate

	This command deactivates the specified flow.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param iFlowID = the Flow ID
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_DEACTIVATE_FLOW( HANDLE hResourceContext, unsigned long iFlowID );

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.7 MFLO get active flow

	This command gets the number of active flows in the system.  
	Use QLIB_MFLO_GET_BOUND_FLOW_LIST to get more information about the active flows.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param piBoundFlowCount = the number of active flows
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_GET_BOUND_FLOW_COUNT( HANDLE hResourceContext, unsigned char* piBoundFlowCount );

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.7 MFLO get active flow

	This command gets information regarding all active flows in the system.  

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param piBoundFlowCount = This is both and input and output param.  As an input param
							  it indicates the number of elements in pActiveFlowInfoArray. 
							  As an output param it indicates the actual number of active 
							  flows in the system.
	\param pActiveFlowInfoArray = Array of mftypes_active_flow_info structures 
									(mftypes_active_flow_info is defined in QLib_Defines.h)

	\return true if successful, false if fail.

	\warning Prior to calling this function, sufficient space should be allocated in
			 pActiveFlowInfoArray to	store information about all active flows in
			 the system.  The required amount of space can be determined by first calling
			 QLIB_MFLO_GetBoundFlowCount.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_GET_BOUND_FLOW_LIST( HANDLE hResourceContext, 
														  unsigned char* piBoundFlowCount, 
														  void* pActiveFlowInfoArray);

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.8 MFLO get MLC info

	This command gets information about the specified MLC ID.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param iMlcID = the MLC ID
	\param eSystem = the system (mftypes_system_type)
	\param pActiveMlcInfo = information regarding the specified MLC (mftypes_active_mlc_info)
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_GET_MLC_INFO(	HANDLE hResourceContext, 
													unsigned char iMlcID, 
													unsigned char eSystem, 
													void* pActiveMlcInfo );

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.9 MFLO get RSSI value

	This command gets the current calculated RSSI (Received Signal Strength Indicator) value.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param pdRSSI = the RSSI value in dBm
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_GET_RSSI_VALUE( HANDLE hResourceContext, double* pdRSSI );

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.10 MFLO get MLC PLP

	This command gets accumulated PLP stats and accumulated CB stats before and after RS.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param iMlcID = the MLC ID
	\param eSystem = the system (mftypes_system_type)
	\param pMlcPlpStats = structure of type mftypes_mlc_plp_stats_type to hold the PLP and CB statistics 
									as defined in QLIB_Defines.h
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_GET_MLC_PLP_STAT_DYN_PARAMS(	HANDLE hResourceContext, 
																	unsigned char iMlcID, 
																	unsigned char eSystem, 
																	void* pMlcPlpStats );

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.11 MFLO set channel

	This command sets the MediaFLO RF channel.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param iChannel = the channel
	
	\return true if successful, false if fail.

	\warning Once the channel is changed, the MFLO_START_FLO command needs to be sent in order
			 to acquire the system on the new channel.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_SET_RF_CHNL( HANDLE hResourceContext, unsigned char iChannel );

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.12 MFLO get channel

	This command gets the MediaFLO RF channel.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param piChannel = the channel
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_GET_RF_CHNL( HANDLE hResourceContext, unsigned char *piChannel );

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.13 MFLO reset MLC PLP

	This command resets the PLP and CB statistics associated with the specified MLC ID.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param iMlcId = the MLC ID for which to reset the PLP and CB statistics
	\param eSystem = the system (mftypes_system_type),
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_RST_MLC_PLP_STATS(	HANDLE hResourceContext,
													    unsigned char iMlcId,
													    unsigned char eSystem );

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.14 MFLO MLC dynamic

	This command gets the MLC Dynamic information associated with the specified MLC ID.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param iMlcID = the MLC ID
	\param eSystem = the system (mftypes_system_type)
	\param pMLC_Dyn_Params = structure to store the info associated with the MLC ID 
							 (mftypes_mlc_dyn_params_type)

	\return true if successful, false if fail.

	\warning The response to this command is only valid if the specified MLC ID is 
			 currently active.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_GET_MLC_DYN_PARAMS(	HANDLE hResourceContext, 
															unsigned char iMlcID, 
															unsigned char eSystem, 
															void* pMLC_Dyn_Params);

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.15 MFLO get OIS PLPs

	This command gets the OIS PLP statistics associated with the specified OIS type.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param eOIStype = the OIS type, wide area or local area (mftypes_system_type)
	\param piLastNumGoodPLPs = number of good PLPs from last decoding
	\param piLastNumErasures = number of erasures from last decoding
	\param piTotalNumGoodPLPs = cummulative number of good PLPs including from last decoding
	\param piTotalNumErasures = cummulative number of erasures including from last decoding

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_GET_OIS_PLP_STAT(	HANDLE hResourceContext, 
														unsigned char eOIStype, 
														unsigned long* piLastNumGoodPLPs, 
														unsigned long* piLastNumErasures,
														unsigned long* piTotalNumGoodPLPs, 
														unsigned long* piTotalNumErasures );

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.16 MFLO reset OIS PLPs statistics

	This command resets the OIS PLP statistics associated with the specified OIS type.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param eOIStype = the OIS type, wide area or local area (mftypes_system_type)

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_RST_OIS_PLP_STAT( HANDLE hResourceContext, unsigned char eOIStype );

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.17 MFLO GET FLO SUBSTATE

	This command gets the protocol task's substate.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param eSystem = the system (mftypes_system_type)
	\param peSubstate = the protocol task's substate (mftypes_substate)

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_GET_FLO_SUBSTATE(	HANDLE hResourceContext, 
														unsigned char eSystem, 
														unsigned char* peSubstate );

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.18 MFLO GET ACTIVE MLC LIST

	This command gets the number of active MLCs on both of the systems, WOIS and LOIS.
	Use QLIB_MFLO_GET_ACTIVE_MLC_LIST to get more information about the active MLCs.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param piActiveMlcCount = the number of active MLCs
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_GET_ACTIVE_MLC_COUNT( HANDLE hResourceContext, unsigned char* piActiveMlcCount );

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.18 MFLO GET ACTIVE MLC LIST

	This command gets information regarding all active MLCs on both of the systems.  

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param piActiveMlcCount = This is both and input and output param.  As an input param
							  it indicates the number of elements in pActiveMlcInfoArray. 
							  As an output param it indicates the actual number of active 
							  MLCs on both systems.
	\param pActiveMlcInfoArray = Array of mftypes_active_mlc_info structures 
								 (mftypes_active_mlc_info is defined in QLib_Defines.h)

	\return true if successful, false if fail.

	\warning Prior to calling this function, sufficient space should be allocated in
			 pActiveMlcInfoArray to	store information about all active MLCs on both
			 systems.  The required amount of space can be determined by first calling
			 QLIB_MFLO_GetActiveMlcCount.

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_GET_ACTIVE_MLC_LIST( HANDLE hResourceContext, 
														  unsigned char* piActiveMlcCount, 
														  void* pActiveMlcInfoArray);

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.19 MFLOG GET FLO PLP DYN

	This command gets the PLP statistics for a specific Flow ID.  

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param iFlowID = the Flow ID
	\param piTotalNumGoodPlps = the total number of good PLPs
	\param piTotalNumErasures = the total number of erasures
	\param piSFNumGoodPlps = the number of good PLPs in the last superframe
	\param piSFNumErasures = the number of erasures in the last superframe

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_GET_FLO_PLP_DYN_PARAMS( HANDLE hResourceContext, 
															 unsigned long iFlowID,
														     unsigned short* piTotalNumGoodPlps, 
														     unsigned short* piTotalNumErasures,
															 unsigned short* piSFNumGoodPlps, 
														     unsigned short* piSFErasures);

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.20 MFLO ENABLE FTAP PLP DATA

	This command enables logging of FTAP PLP parameters.  This is the first diag 
	command that needs to be issued when logging FTAP PLP packet parameters.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param bEnable = true enables logging, false disables logging

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_ENABLE_FTAP_PLP_DATA( HANDLE hResourceContext, unsigned char bEnable );

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.21 MFLO ENABLE FTAP OIS RECEPTION MODE

	This command places the FDS in a test mode called "FTAP OIS Reception Mode".
	In this mode, the device decodes WOIS Only, LOIS Only or both WOIS and LOIS
	every superframe.  Once in this mode, the FDS starts logging WOIS/LOIS log
	codes (0x120E and 0x120F) once every superframe.  This command can also be used
	to disable "FTAP OIS Reception Mode".

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param eOIS_ReceptionMode = the FTAP OIS reception mode (mftypes_ois_reception_mode_type)

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_ENABLE_FTAP_OIS( HANDLE hResourceContext, unsigned char eOIS_ReceptionMode );

	/******************************************************************************/
	/**
	MediaFLO Diagnostic ICD, 2.3.22 MFLO ENABLE FTAP WICLIC RECEPTION MODE

	This command places the FDS in a test mode called "FTAP WIC/LIC Reception Mode".
	In this mode, the device decodes WIC/LIC every superframe.  Once in this mode, 
	the FDS starts logging the WIC/LIC log code (0x1210) once every superframe.  
	This command can also be used to disable "FTAP WIC/LIC Reception Mode".

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param bEnable = true places FDS in FTAP WIC/LIC Reception Mode,
					 false disables WIC/LIC decode

	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_ENABLE_WIC_LIC( HANDLE hResourceContext, unsigned char bEnable );

	/******************************************************************************/
	/**
	This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
	API's to perform a specialized task.

	This command is used to enable the logging of LOG_MFLO_FDM_RECORDS_C log packets.
	The LOG_MFLO_FDM_RECORDS_C log packets contain the FDM (Flow Description Message) 
	information for a given WIDE/LOCAL system - a mapping table between Flow IDs and 
	MLC IDs.
	
	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param eMessageType = identifies which part of the control channel data is being 
					      requested (mftypes_cc_msg_type)

	\return true if the LOG_MFLO_FDM_RECORDS_C log packets have been generated
	        successfully, otherwise false.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_GET_CONTROL_CHANNEL_DATA(HANDLE hResourceContext, unsigned char eMessageType );

	/******************************************************************************/
	/**
	This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
	API's to perform a specialized task.

	This function gets information required to deactivate all active Flow IDs through 
	QLIB_MFLO_GET_BOUND_FLOW_LIST.  Then it sends the QLIB_MFLO_DEACTIVATE_FLOW
	command for each active Flow ID.  After deactivating all Flow IDs, this function 
	will call QLIB_MFLO_GET_BOUND_FLOW_COUNT in one second intervals until the command
	reports	that the number of active Flow IDs is zero or the number of seconds specified 
	by the iTimeOut input parameter have elapsed.
	
    This function will fail if any of the functions called fail or if all active
	Flow IDs have not been de-activated after the specified number of seconds to wait.

 	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param iTimeOut - the number of seconds to wait for QLIB_MFLO_GET_BOUND_FLOW_COUNT 
					  to report that the number of active Flow IDs is zero. 

	\return true if successful, false if fail.

	\warning none

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_DeactivateAllFlows( HANDLE hResourceContext, unsigned short iTimeOut );

	/******************************************************************************/
	/**
	This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
	API's to perform a specialized task.

	This command gets the current system state for the specified system(s) by calling
	QLIB_MFLO_GET_FLO_STATE for each system requested.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param eSystem = the system (mftypes_system_type)
	\param peWideAreaSystemState = the current wide area system state (mftypes_state_type)
	\param peLocalAreaSystemState = the current local area system state (mftypes_state_type)
	
	\return true if successful, false if fail.

	\warning 

	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_GetSystemState( HANDLE hResourceContext, unsigned char eSystem, 
													 unsigned char* peWideAreaSystemState,
													 unsigned char* peLocalAreaSystemState );

	/******************************************************************************/
	/**
	This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
	API's to perform a specialized task.

	This function measures the Physical layer packet error rate.  This type of packet 
	error rate measurement does not require a known nor finite sequence of FTAP packets 
	from the FLO Tx Waveform.  The measurement is made by querying the accumulated PLP 
	statistics for all requested Flow IDs.  Accumulated statistics continue to be queried 
	until the minimum number of PLPs have been recorded.	Accumulated PLP statistics 
	are queried through the QLIB_MFLO_GET_MLC_PLP_STAT_DYN_PARAMS function. 

	After calling QLIB_MFLO_RST_PLP_STATS to reset the accumulated statistics for all
	MLCs, this function may be called repeatedly until the desired level of confidence 
	is achieved.
	
	The time period that is used when checking for status is controlled by the
	timing index QMSL_Timeout_Delay_MediaFLO_StatusCheck, to be used with the functions 
	QLIB_ConfigureTimeOut() and QLIB_GetTimeOut().

	Usage examples:

		QLIB_MFLO_RST_PLP_STATS	
		QLIB_MFLO_GetPER_Phy		--------------	Measure PER for first test
									

		QLIB_MFLO_RST_PLP_STATS		---------
		QLIB_MFLO_GetPER_Phy				|
		QLIB_MFLO_GetPER_Phy				|-----  Measure PER for second test 
		QLIB_MFLO_GetPER_Phy				|		Note: Example shows measurement ran longer  
		QLIB_MFLO_GetPER_Phy		---------		      to achieve desired level of confidence

	This function reports the packet error rate statistics for all requested Flow IDs, 
	the current system state, and an error code.  

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param iNumberOfFlowIDs = the number of FlowIDs in aiFlowIDs
	\param aiFlowIDs = array of Flow IDs for which to get accumulated PLP statistics
	\param aiMlcIDs = array of MLC IDs that correspond to the Flow IDs in aiFlowIDs,
	                  this is required because QLIB_MFLO_GET_MLC_PLP_STAT_DYN_PARAMS 
					  requires the MLC ID not Flow ID
	\param aeSystem = array of mftypes_system_type with elements that correspond to the 
	                  Flow IDs in aiFlowIDs, the System for each Flow ID is required by
					  QLIB_MFLO_GET_MLC_PLP_STAT_DYN_PARAMS
	\param iNoStatsTimeout = time in seconds to wait for statistics to update, max of 5 
							 seconds recommended
	\param ePerToMeas = which packet error rate to measure, statistics for both Base and 
	                    Enhancement layers will be reported but the determination of
						whether or not iMinPLPs have been measured depends on what this
						variable has been set to (mftypes_per_to_measure)
	\param iMinPLPs = the minimum number of PLPs to measure for all Flow IDs
	\param eRS = which statistics to report, Pre or Post Reed Solomon (mftypes_reed_solomon)
	\param paOverallStats = an array of type mftypes_per_statistics with length 
							iNumberOfFlowIDs to store the packet error rate statistics for 
							all requested Flow IDs
	\param peWideAreaSystemState = the current wide area system state if one of the 
								   elements in aeSystem is WIDE (mftypes_state_type)
	\param peLocalAreaSystemState = the current local area system state if one of the 
	                                elements in aeSystem is LOCAL (mftypes_state_type)
	\param peErrorCode = an error code used whenever possible to provide more information 
	                     about why the function failed, see the warning below for one
						 exception (mftypes_error_code)

	\return true if successful, false if fail.

	\warning This function does not dynamically allocate memory for the pOverallStats 
			 array.  Sufficient memory for pOverallStats must be allocated by the 
			 calling program.

			 The results reported in paOverallStats are only valid if...
				QLIB_MFLO_GetPER_Phy return value is 1		AND
				Requested system state(s) == ACTIVE_STATE	AND
			    peErrorCode == NOT_AVAILABLE.

			 If at some point during the measurement, the requested system state is not 
			 ACTIVE_STATE,  the function will return but not fail.  The current system 
			 state will be reported through peWideAreaSystemState and peLocalAreaSystemState.

			 If the statistics for one or more of the MLC IDs in aiMlcIDs have not updated
			 after the number of seconds specified by iNoStatsTimeout have elapsed, the 
			 function will return but not fail. peErrorCode will be set to PLP_STATS_NOT_UPDATING.
	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_GetPER_Phy( HANDLE hResourceContext, unsigned char iNumberOfFlowIDs, 
		                                         unsigned long* aiFlowIDs, unsigned char* aiMlcIDs, 
												 unsigned char* aeSystem, unsigned short iNoStatsTimeout, 
												 unsigned char ePerToMeas, unsigned long iMinPLPs, 
												 unsigned char eRS, void* paOverallStats, 
												 unsigned char* peWideAreaSystemState, 
												 unsigned char* peLocalAreaSystemState, 
												 unsigned char* peErrorCode );

	/******************************************************************************/
	/**
	This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
	API's to perform a specialized task.

	This function performs initialization steps required before making ANY 
	Physical layer + MAC layer packet error rate measurements.  This type of packet 
	error rate measurement requires a known and finite sequence of FTAP packets from 
	the FLO Tx Waveform.  This function initializes an internal data structure with
	information about the finite sequence of FTAP packets in the FLO Tx Waveform and
	dynamically allocates memory for internal data structures used to make the packet 
	error rate measurement.
	
	This function will fail if pTAP_MsgContent is NULL or if there are any memory 
	allocation errors.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param pTAP_MsgContent = pointer to mftypes_tap_msg_content structure filled with 
					         information about the finite sequence of FTAP packets in 
							 the FLO Tx Waveform.  
	\param peErrorCode = an error code used whenever possible to provide more 
						 information about why the function failed (mftypes_error_code)

	\return true if successful, false if fail.

	\warning This function must be called once at the start of testing, before calling 
			 any of the following functions:

				QLIB_MFLO_Start_PhyMAC
				QLIB_MFLO_GetPER_PhyMAC
				QLIB_MFLO_Stop_PhyMAC

			 There is no need to call this function more than once during one session 
			 if the contents of the mftypes_tap_msg_content structure (pTAP_MsgContent) 
			 do not need to change.

			 To prevent memory leaks, memory dynamically allocated by this function 
			 should be freed by calling QLIB_MFLO_FinalCleanup_PhyMAC at the end of 
			 testing.

			 See QLIB_MFLO_GetPER_PhyMAC for usage examples.
 	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_InitTAPMsgContent_PhyMAC( HANDLE hResourceContext, 
															   void* pTAP_MsgContent, unsigned char* peErrorCode );

	/******************************************************************************/
	/**
	This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
	API's to perform a specialized task.

	This function performs initialization steps required before making a new 
	Physical layer + MAC layer packet error rate measurement.  This type of packet 
	error rate measurement requires a known and finite sequence of FTAP packets from 
	the FLO Tx Waveform.  This function resets the overall packet error rate statistics, 
	updates data structures used in the packet error rate measurement, clears the log 
	queue, registers the following log codes:

			LOG_MFLO_FTAP_PACKETRECORD_HEADER_PARAMS_C	0x120C
			LOG_MFLO_FTAP_PACKETRECORD_PARAMS_C			0x120D
	
	and enables FTAP PLP Data Logging Mode by through QLIB_MFLO_ENABLE_FTAP_PLP_DATA.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param iNumberOfFlowIDs = the number of FlowIDs in aiFlowIDs, must match the actual
	                          number of active FlowIDs or this funtion will fail
	\param aiFlowIDs = array of Flow IDs for which to measure the packet error rate
	\param paOverallStats = an array of type mftypes_per_statistics with length 
							iNumberOfFlowIDs to store the packet error rate 
							statistics for all requested Flow IDs
	\param peErrorCode = an error code used whenever possible to provide more 
						 information about why the function failed (mftypes_error_code)

	\return true if successful, false if fail.

	\warning This function does not dynamically allocate memory for the pOverallStats 
			 array.  Sufficient memory for pOverallStats must be allocated by the 
			 calling program.
			 
			 QLIB_MFLO_InitTAPMsgContent_PhyMAC must be called once before the first 
			 time this function is called.
			 
			 This function must be called once before calling QLIB_MFLO_GetPER_PhyMAC.
			 The function QLIB_MFLO_Stop_PhyMAC should be called at the end of the packet 
			 error rate measurement.  To make a new packet error rate measurement, 
			 this function must be called again before calling QLIB_MFLO_GetPER_PhyMAC.  
			 See QLIB_MFLO_GetPER_PhyMAC for usage examples.
	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_Start_PhyMAC( HANDLE hResourceContext, 
		                                           unsigned char iNumberOfFlowIDs, unsigned long* aiFlowIDs, 
												   void* paOverallStats, unsigned char* peErrorCode );

	/******************************************************************************/
	/**
	This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
	API's to perform a specialized task.

	This function measures the Physical layer + MAC layer packet error rate.  This 
	type of packet error rate measurement requires a known and finite sequence of 
	FTAP packets from the FLO Tx Waveform.  
	
	The measurement is made by getting erasure and FTAP Sequence Number / 
	FTAP Test Signature information from log packets with the following log codes:

		LOG_MFLO_FTAP_PACKETRECORD_HEADER_PARAMS_C	0x120C
		LOG_MFLO_FTAP_PACKETRECORD_PARAMS_C			0x120D

    FTAP Sequence Number / FTAP Test Signature errors are determined by comparing 
	the values reported in the log packets to the expected values that were set
	by QLIB_MFLO_InitTAPMsgContent_PhyMAC.  Missing or mismatching pairs in each 
	superframe are counted as errors.
	
	Total errors include erasures and FTAP Sequence Number / FTAP Test Signature errors.

	This function continues parsing log packets and keeping track of overall packet 
	error rate statistics until the minimum number of PLPs have been measured for all 
	requested Flow IDs.

	After calling QLIB_MFLO_Start_PhyMAC, this function may be called repeatedly until 
	desired level of confidence is achieved. Once the current measurement is complete, 
	call QLIB_MFLO_Stop_PhyMAC.  To measure the packet error rate again, call 
	QLIB_MFLO_Start_PhyMAC again before calling this function.

	Usage example:

		QLIB_MFLO_InitTAPMsgContent_PhyMAC	-------- Called once at start of session

		QLIB_MFLO_Start_PhyMAC				---------
		QLIB_MFLO_GetPER_PhyMAC						|-----	Measure PER for first test
		QLIB_MFLO_Stop_PhyMAC				---------

		QLIB_MFLO_Start_PhyMAC				---------
		QLIB_MFLO_GetPER_PhyMAC						|
		QLIB_MFLO_GetPER_PhyMAC						|-----  Measure PER for second test 
		QLIB_MFLO_GetPER_PhyMAC						|		Note: Example shows measurement 
		QLIB_MFLO_GetPER_PhyMAC						|		      ran longer to achieve 
		QLIB_MFLO_Stop_PhyMAC				---------             desired confidence
				.
				.
				.		
		QLIB_MFLO_FinalCleanup_PhyMAC		-------- Called once at end of session

	This function reports the packet error rate statistics for all requested Flow IDs, 
	the current system state, and an error code.  

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()
	\param iNoLogPktsTimeout = time in seconds to wait for log packets
  	\param ePerToMeas = which packet error rate to measure, statistics for both Base 
						and Enhancement layers will be reported but the determination of
						whether or not iMinPLPs have been measured depends on what this
						variable has been set to (mftypes_per_to_measure)
	\param iMinPLPs = the minimum number of PLPs to measure for all Flow IDs
	\param eSystem = the system, required for getting the system state (mftypes_system_type)
	\param eErasureIndicator = which erasure indicator to use when counting erasures 
	                           (mftypes_reed_solomon)
	\param paOverallStats = an array of type mftypes_per_statistics to store the packet 
							error rate statistics for all requested Flow IDs
	\param peWideAreaSystemState = the current wide area system state if eSystem was set to
	                               WIDE or WIDE_AND_LOCAL (mftypes_state_type)
	\param peLocalAreaSystemState = the current local area system state if eSystem was set to
	                                LOCAL or WIDE_AND_LOCAL (mftypes_state_type)
	\param peErrorCode = an error code used whenever possible to provide more information 
	                     about why the function failed, see the warning below for two
						 exceptions (mftypes_error_code)

	\return true if successful, false if fail.

	\warning This function does not dynamically allocate memory for the pOverallStats 
			 array.  Sufficient memory for pOverallStats must be allocated by the 
			 calling program.

			 The results reported in paOverallStats are only valid if...
				QLIB_MFLO_GetPER_PhyMAC return value is true		AND
				Requested system state(s) == ACTIVE_STATE			AND
			    peErrorCode == NOT_AVAILABLE.

			 If at some point during the measurement, the system state is not ACTIVE_STATE,
	         the function will return but not fail.  The current system state will be reported 
			 through peWideAreaSystemState and peLocalAreaSystemState.  At this point 
			 QLIB_MFLO_Stop_PhyMAC should be called.  

			 If no log packets are received after the number of seconds specified by 
			 iNoLogPktsTimeout have elapsed, the function will return but not fail and
			 peErrorCode will be set to NO_LOG_PACKETS. At this point QLIB_MFLO_Stop_PhyMAC 
			 should be called. 

			The time period that is used when checking for status is controlled by the
			timing index QMSL_Timeout_Delay_MediaFLO_StatusCheck, to be used with the functions 
			QLIB_ConfigureTimeOut() and QLIB_GetTimeOut().


			 This function depends on receiving all packets for all requested Flow IDs in 
			 an expected order.  If any expected log packets are not received, the function
			 will return but not fail and peErrorCode will be set to MISSING_LOG_PACKETS. 
			 At this point QLIB_MFLO_Stop_PhyMAC should be called. 
	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_GetPER_PhyMAC( HANDLE hResourceContext, unsigned short iNoLogPktsTimeout, 
													unsigned char ePerToMeas, unsigned long iMinPLPs,
													unsigned char eSystem, unsigned char eErasureIndicator, 
													void* paOverallStats, unsigned char* peWideAreaSystemState, 
													unsigned char* peLocalAreaSystemState, unsigned char* peErrorCode );

	/******************************************************************************/
	/**
	This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
	API's to perform a specialized task.

	This function stops the current Physical layer + MAC layer packet error rate
	measurement.  It disables FTAP PLP Data Logging Mode, clears all log codes for 
	LOG_EQUIP_ID_1X, and clears the log queue.

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()

	\return true if successful, false if fail.

	\warning This function should be called at the end of a packet error rate 
			 measurement after all calls to QLIB_MFLO_GetPER_PhyMAC for the current 
			 measurement are complete. See QLIB_MFLO_GetPER_PhyMAC for usage examples.
 	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_Stop_PhyMAC( HANDLE hResourceContext );

	/******************************************************************************/
	/**
	This is a MediaFLO "helper function" which calls one of the MediaFLO Diagnostic ICD
	API's to perform a specialized task.

	This function performs final cleanup for Physical layer + MAC layer packet error
	rate measurements.  This function only needs to be called once at the end of a 
	test session after all packet error rate measurements to be made with the current
	TAP Message content that was initialized by QLIB_MFLO_InitTAPMsgContent_PhyMAC 
	are complete.  This function disables FTAP PLP Data Logging Mode clears all log 
	codes for LOG_EQUIP_ID_1X, and clears the log queue.  It also frees up any memory 
	that was dynamically allocated by QLIB_MFLO_InitTAPMsgContent_PhyMAC and 
	QLIB_MFLO_Start_PhyMAC.  

	\param hResourceContext = Resource context that was returned from the call to ConnectServer()

	\return true if successful, false if fail.

	\warning To prevent memory leaks, this function must be called once at the end 
			 of testing when no more packet error rate measurements are to be made 
			 with the current TAP Message Content.  See QLIB_MFLO_GetPER_PhyMAC for 
			 a usage example.
 	*******************************************************************************/
	QLIB_API unsigned char QLIB_MFLO_FinalCleanup_PhyMAC( HANDLE hResourceContext );

#ifdef __cplusplus
}	// extern "C"
#endif 

#endif	// defined(_QLIB_H)
