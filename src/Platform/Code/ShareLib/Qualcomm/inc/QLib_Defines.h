/******************************************************************************/
/**
Program: QLib

\file 
  Source\Q_Defines.h

\brief Definition for data types, structures, and used for the QLib.

 \b QUALCOMM  \b PROPRIETARY 

This document contains propriety information, and except with written
permission of Qualcomm INC, such information shall not be 
published, or disclosed to others, or used for any purpose, and the   
document shall not be duplicated in whole or in part.  

\code
List of sections contained in this document:
	QMSL - Connection/disconnection
	Diagnostic - General Asynchronous message Logging
	Diagnostic - Specific log messages
	Diagnostic - phone version info
	Diagnostic - reading embedded error information
	Diagnostic - General command defines
	Diagnostic - EFS Subsystem
	Diagnostic - NV Subsystem
	Diagnostic - GSDI Subsystem
	Diagnostic - Streaming Download subsystem
	Diagnostic - GPS Subsystem
	Diagnostic - Call Manager subsytem
	Diagnostic - GSM subsytem
	Diagnostic - Handset subsytem
	Diagnostic - MediaFLO subsystem
	FTM - Command ID's
	FTM - System Modes
	FTM - RF Modes
	FTM - GSM RF
	FTM - cdma2000 non-signaling log parser definitions
	FTM - cdma2000 non-signaling
	FTM - EVDO non-signaling log parser definitions
	FTM - EVDO non-signaling
	FTM - Bluetooth
	FTM - AGPS
	FTM - PMIC
	FTM - Customer Extensions
	FTM - Audio
	FTM - Camera
	FTM - Log
	FTM - WCDMA BER
	FTM - HSDPA BLER
	FTM - GSM BER
	FTM - EGPRS BLER
	FTM - Common RF
	FTM - MediaFLO
	FTM - WLAN
	QMSL - Text logging flags
	QMSL - Time Out defintions
\endcode 

\version 
\code 


\endcode  

\note   
Compiler:  Microsoft Visual C++ v6.0 SP4  
*******************************************************************************/

#if !defined(_QLIB_DEFINES_H)
#define _QLIB_DEFINES_H

#pragma pack(1)        /* DM <--> DMSS packet definitions are byte packed */

/*===========================================================================
                       Definition of primitive data types
===========================================================================*/
#if !defined( QMSL_NO_REDEFINITION_OF_UNSIGNED_INTEGER_TYPES )

	typedef unsigned char byte;			//!<' Definition of unsigned 8-bit type
	typedef unsigned short word ;		//!<' Definition of unsigned 16-bit type
	typedef unsigned long dword ;		//!<' Definition of unsigned 32-bit type
#endif	// #if !defined( QMSL_NO_REDEFINITION_OF_UNSIGNED_INTEGER_TYPES )

/******************************************************************************
                       Basic Definitions
*******************************************************************************/


#if !defined( TRUE )
	#define TRUE   1   //!<' Boolean true value.
#endif

#if !defined( FALSE )
	#define FALSE  0   //!<' Boolean false value.
#endif

#if !defined( ON )
	#define  ON   1    //!<' On value. 
#endif

#if !defined( OFF )
	#define  OFF  0    //!<' Off value.
#endif

//! Size of the ITEM DATA field for an NV request
#define DIAG_NV_ITEM_DATA_SIZE 128	

//! Maximum size of diagnostic packet
#define DIAG_MAX_PACKET_SIZE 4096


//! Definition of the COM port value that will be used to "auto detect" the COM port
#define QLIB_COM_AUTO_DETECT ((word)0xFFFF)

//! Definition of SUCCESS for the FTM STATUS field
#define DIAG_FTM_STATUS_SUCCESS 0

//! Definition of FAIL for the FTM STATUS field
#define DIAG_FTM_STATUS_FAIL 1

/******************************************************************************
						QMSL - Connection/disconnection
*******************************************************************************/
/**
	Packet mode definitions for QLIB_SetPacketMode

*/
typedef enum 
{
	QLIB_PacketMode_BothDiag	= 0,	//!<' Both library and phone in Diagnostic mode
	QLIB_PacketMode_BothAT		= 1,	//!<' Both library and phone in AT mode
	QLIB_PacketMode_LibAT		= 2		//!<' Library in AT mode, phone is not controlled
} QLIB_PacketMode_Enum;

#define QLIB_LIB_MODE_QPHONEMS false	//!<' Internal engine, QPHONEMS, also used for user defined transport
#define QLIB_LIB_MODE_QPST     true		//!<' Use QPST for packet transport

/******************************************************************************
						Diagnostic - General Request/Response
*******************************************************************************/

/*--------------------------------------------------------------------------

  Command Codes between the Diagnostic Monitor and the mobile. Packets
  travelling in each direction are defined here, while the packet templates
  for requests and responses are distinct.  Note that the same packet id
  value can be used for both a request and a response.  These values
  are used to index a dispatch table in diag.c, so 

  DON'T CHANGE THE NUMBERS ( REPLACE UNUSED IDS WITH FILLERS ). NEW IDs
  MUST BE ASSIGNED AT THE END.

  This list is originally taken from the file DiagDefines.h, then reduced
  to only include items necessary for this library.

  If more items are desired or if new diag commands are added then this list 
  should be resync'd.
  

  Note: The prefix _ is used here because the function names are exactly
        the enumeration ID, as used in the embedded code
		
----------------------------------------------------------------------------*/

#define _DIAG_VERNO_F			0	//!<' Version Number Request/Response            
#define _DIAG_ESN_F				1	//!<' Mobile Station ESN Request/Response        
#define _DIAG_MEMORY_PEEK_BYTE_F 2	//!<' Memory peek request/response (8-bit)
#define _DIAG_MEMORY_PEEK_WORD_F 3	//!<' Memory peek request/response (16-bit)
#define _DIAG_MEMORY_PEEK_DWORD_F 4	//!<' Memory peek request/response (32-bit)
#define _DIAG_MEMORY_POKE_BYTE_F 5	//!<' Memory poke request/response (8-bit)
#define _DIAG_MEMORY_POKE_WORD_F 6	//!<' Memory poke request/response (16-bit)
#define _DIAG_MEMORY_POKE_DWORD_F 7	//!<' Memory poke request/response (32-bit)
#define _DIAG_STATUS_REQUEST_F	12	//!<'Status Request/Response
#define _DIAG_STATUS_F			14	//!<' Phone status
#define _DIAG_LOG_F				16  //!<' Log packet Request/Response 

#define _DIAG_BAD_CMD_F			19	//!<' Invalid Command Response
#define _DIAG_BAD_PARM_F		20	//!<' Invalid parmaeter Response
#define _DIAG_BAD_LEN_F			21	//!<' Invalid packet length Response
#define _DIAG_BAD_MODE_F		24	//!<' Packet not allowed in this mode 

/* 22-23 Reserved */

/* Packet not allowed in this mode 
   ( online vs offline )                      */
#define DIAG_BAD_MODE_F     24

#define _DIAG_MSG_F				31	//!<' Request for msg report
#define _DIAG_HS_KEY_F			32	//!<' Handset Emulation -- keypress              
#define _DIAG_NV_READ_F			38	//!<' Read NV item                               
#define _DIAG_NV_WRITE_F		39	//!<' Write NV item                             
#define _DIAG_CONTROL_F			41	//!<' Mode change request                       
#define _DIAG_ERR_READ_F		42	//!<' Read error list
#define _DIAG_ERR_CLEAR_F		43	//!<' Clear error list
#define _DIAG_GET_DIPSW_F		47 	//!<' Retreive dipswitch
#define _DIAG_SET_DIPSW_F		48	//!<' Set dipswitch
#define _DIAG_VOC_PCM_LB_F		49  //!<' Start/Stop Vocoder PCM loopback            
#define _DIAG_VOC_PKT_LB_F		50  //!<' Start/Stop Vocoder PKT loopback            
#define _DIAG_DLOAD_F			58	//!<' Switch to download mode
#define _DIAG_SPC_F				65  //!<' Send the Service Prog. Code to allow SP    
#define _DIAG_SERIAL_MODE_CHANGE 68 //!<' Switch mode from diagnostic to data
#define _DIAG_EXT_LOGMASK_F		93	//!<' Extended logmask for > 32 bits.
#define _DIAG_EVENT_REPORT_F    96  //!<' Static Event reporting. 
#define _DIAG_SUBSYS_CMD_F		75  //!<' Subssytem dispatcher (extended diag cmd) 
#define _DIAG_NV_WRITE_ONLINE_F	76	//!<' Write to NV location without going Offline                            
#define _DIAG_IS2000_STATUS_F   112	//!<' IS-2000 Status
#define _DIAG_LOG_CONFIG_F		115	//!<' Logging configuration packet
#define _DIAG_EXT_MSG_F			121 //!<' Extended msg report
#define _DIAG_PROTOCOL_LOOPBACK_F  123 //!<' Diagnostics protocol loopback.
#define _DIAG_EXT_BUILD_ID_F    124	//!<' Extended build ID 
#define _DIAG_EXT_MSG_CONFIG_F  125 //!<' Request for extended msg report
#define _DIAG_SECURITY_FREEZE_F 0xff	//!<' Request for Sirius security freeze (not defined yet)
#define _DIAG_MAX_F				126	//!<' Number of packets defined.
#define _DIAG_SUBSYS_CMD_VER_2_F	128	
#define _DIAG_EVENT_MASK_GET_F		129	//!<' Get event mask
#define _DIAG_EVENT_MASK_SET_F		130	//!<' Set event mask

/**
	Diagnostic Subsystems, from Services\diagcmd.h

*/
typedef enum 
{
	_DIAG_SUBSYS_OEM		= 0,
	_DIAG_SUBSYS_ZREX		= 1,
	_DIAG_SUBSYS_SD			= 2,
	_DIAG_SUBSYS_BT			= 3,
	_DIAG_SUBSYS_WCDMA		= 4,
	_DIAG_SUBSYS_HDR		= 5,
	_DIAG_SUBSYS_DIABLO		= 6,
	_DIAG_SUBSYS_TREX		= 7,
	_DIAG_SUBSYS_GSM		= 8,
	_DIAG_SUBSYS_UMTS		= 9,
	_DIAG_SUBSYS_HWTC		= 10,
	_DIAG_SUBSYS_FTM		= 11,
	_DIAG_SUBSYS_REX		= 12,
	_DIAG_SUBSYS_GPS		= 13,
	_DIAG_SUBSYS_WMS		= 14,
	_DIAG_SUBSYS_CM			= 15,
	_DIAG_SUBSYS_HS			= 16,
	_DIAG_SUBSYS_AUDIO_SETTINGS   = 17,
	_DIAG_SUBSYS_DIAG_SERV	= 18,
	_DIAG_SUBSYS_EFS		= 19,
	_DIAG_SUBSYS_PORT_MAP_SETTINGS  = 20,
	_DIAG_SUBSYS_MEDIAPLAYER= 21,
	_DIAG_SUBSYS_QCAMERA	= 22,
	_DIAG_SUBSYS_MOBIMON	= 23,
	_DIAG_SUBSYS_GUNIMON	= 24,
	_DIAG_SUBSYS_LSM		= 25,
	_DIAG_SUBSYS_QCAMCORDER	= 26,
	_DIAG_SUBSYS_MUX1X		= 27,
	_DIAG_SUBSYS_DATA1X		= 28,
	_DIAG_SUBSYS_SRCH1X		= 29,
	_DIAG_SUBSYS_CALLP1X	= 30,
	_DIAG_SUBSYS_APPS		= 31,
	_DIAG_SUBSYS_SETTINGS	= 32,
	_DIAG_SUBSYS_GSDI		= 33,
	_DIAG_SUBSYS_TMC		= 34,
	_DIAG_SUBSYS_USB		= 35,
	_DIAG_SUBSYS_PM			= 36,
	_DIAG_SUBSYS_DEBUG		= 37,
	_DIAG_SUBSYS_QTV		= 38,
	_DIAG_SUBSYS_CLKRGM		= 39,
	_DIAG_SUBSYS_DEVICES	= 40,
	_DIAG_SUBSYS_WLAN		= 41,
	_DIAG_SUBSYS_PS_DATA_LOGGING=42,
	_DIAG_SUBSYS_MFLO		= 43,
	_DIAG_SUBSYS_DTV		= 44,
	_DIAG_SUBSYS_RRC		= 45,
	_DIAG_SUBSYS_PROF		= 46,
	_DIAG_SUBSYS_TCXOMGR	= 47,
	_DIAG_SUBSYS_NV			= 48,
	_DIAG_SUBSYS_AUTOCONFIG	= 49,
	_DIAG_SUBSYS_PARAMS		= 50,
	_DIAG_SUBSYS_MDDI		= 51,
	_DIAG_SUBSYS_DS_ATCOP	= 52,
	_DIAG_SUBSYS_L4LINUX	= 53,
	_DIAG_SUBSYS_MVS		= 54,
	_DIAG_SUBSYS_CNV		= 55,
	_DIAG_SUBSYS_LAST
} DiagPkt_Subsys_cmd_enum_type;

/******************************************************************************
						Diagnostic - General Asynchronous message Logging
*******************************************************************************/

//! Structure to define storage area for a response messages in a queue
typedef struct
{
	word iMessageLength;

	// This object can be accessed as a single large buffer of bytes...
	byte iData[DIAG_MAX_PACKET_SIZE];
		
} ResponseMessage;

/*===========================================================================*/
/**
	enum log_equip_id_enum_type

	\brief Log Equipment IDs. The number is represented by 4 bits.
*/
/*===========================================================================*/
typedef enum 
{
  LOG_EQUIP_ID_OEM   = 0,	//<!' 3rd party OEM (licensee) use
  LOG_EQUIP_ID_1X    = 1,	//<!' Traditional 1X line of products 
  LOG_EQUIP_ID_RSVD2 = 2,
  LOG_EQUIP_ID_RSVD3 = 3,
  LOG_EQUIP_ID_WCDMA = 4,	//<!' WCDMA
  LOG_EQUIP_ID_GSM   = 5,	//<!' GSM
  LOG_EQUIP_ID_MSP   = 6,
  LOG_EQUIP_ID_UMTS  = 7,	//<!' UMGS
  LOG_EQUIP_ID_TDMA  = 8,	//<!' TDMA

  LOG_EQUIP_ID_LAST_DEFAULT = LOG_EQUIP_ID_TDMA,
  LOG_EQUIP_ID_MAX = 16

} log_equip_id_enum_type;

/*===========================================================================*/
/**
	enum log_operation_id_enum_type

	\brief List of log configuration operations
*/
typedef enum 
{
	LOG_CONFIG_OP_DisableLogging,	//!<' 0 = Disable logging service
	LOG_CONFIG_OP_RetrieveIDs,		//!<' 1 = Retrieve ID ranges
	LOG_CONFIG_OP_Reserved,			//!<' 2 = (Reserved)
	LOG_CONFIG_OP_SetLogMask,		//!<' 3 = Set log mask
	LOG_CONFIG_OP_GetLogMask		//!<' 4 = Get log mask

} log_operation_id_enum_type;

#define LOG_CODE_FTM_2			0x117C	//<!' FTM Version 2 log file
#define LOG_CODE_LAST_ITEM_1X	0x02E8	//<!' Last item number for 1X, log code 1.

//! Number of bytes used for time stamp in diagnostic async messages
#define QMSL_DIAG_TIME_STAMP_SIZE 8

/*===========================================================================*/
/**
	structure for an ExtendedMessage, as described in CDMA ICD 3.4.121 Extended Message

*/
/*===========================================================================*/
typedef struct
{
	byte CMD_CODE;		//!< ' Message ID. The DMSS sets CMD_CODE to 121 for this message.
	byte TS_TYPE;		//!< ' Time stamp type; an enumeration indicating the format of the TIME field
	byte NUM_ARGS;		//!< ' The number of 32-bit arguments listed in the ARGS field.
	byte DROP_CNT;		//!< ' Total number of messages dropped between this message and the previous one
	byte TIME[QMSL_DIAG_TIME_STAMP_SIZE];		//!< ' Time the message was originally generated (not transmitted)
	word LINE_NUMBER;	//!< ' Line number identifying location of the message in the file denoted by FILE_NAME
	word SS_ID;			//!< ' Subsystem identifier; see Section 3.4.125.2 for description
	dword SS_MASK;		//!< ' Subsystem mask; see Section 3.4.125.2 for description
	dword ARG_1;		//!< ' Array of 32-bit signed arguments, corresponding to the printf-style FMT_STRING
	dword ARG_2;		//!< ' 
	dword ARG_3;		//!< ' 
	char FMT_STRING[100];		//!< ' NULL-terminated ASCII string containing a printf()-style format, 
	                            //!< '  string, with formatting specifiers
	char FILE_NAME[100];		//!< ' NULL-terminated ASCII file name string identifying the location of message


} ExtendedMessage;


/*===========================================================================*/
/**
	Structure of a log message, as described in WCDMA ICD section 4.1 - Log record structure
*/
/*===========================================================================*/

typedef struct 
{
	byte CMD_CODE;
	byte MORE;
	word length;
	word length2;		// Not sure why length is being put in twice...could be a receieve data error
	word log_item;
	byte iTimeStamp[QMSL_DIAG_TIME_STAMP_SIZE];
	byte iLogMsgData[DIAG_MAX_PACKET_SIZE - 6] ;

} LogMessage;

/*===========================================================================*/
/**
	Structure of a streaming message, as described in WCDMA ICD 3.5.17.2 - Message Response Message
*/
/*===========================================================================*/

typedef struct 
{
	byte CMD_CODE;		// (31) for a streaming message
	word QUANTITY;
	dword DROP_CNT;
	dword TOTAL_MSGS;
	byte MESSAGE_LEVEL;
	char FILE_NAME[13];
	word LINE_NUMBER;
	byte FMT_STRING[40];
	dword CODE1;
	dword CODE2;
	dword CODE3;
	byte TIME[QMSL_DIAG_TIME_STAMP_SIZE];
} MessageResponse;

/**
	Subsystem dispatcher header, request or response.
*/
typedef struct                
{
  byte	cmdCode ;             //!<' = DIAG_SUBSYS_CMD_F
  byte	subsysId ;            //!<' = diagpkt_subsys_cmd_enum_type
  word	subsysCmdCode ;      //!<' Subsystem-specific command code.
} diagpkt_subsys_header_type;


/**
	FTM Subsystem dispatcher header, request with extended information
*/
typedef struct                
{
	diagpkt_subsys_header_type Diag_Header;

	word cmd_id;			//!<' ftm_hdr.cmd_id;
	word data_len;			//!<' ftm_hdr.data_len
	word rsp_pkt_size;		//!<' ftm_hdr.rsp_pkt_size

} ftm_extended_request_header_type;

/**
	FTM Subsystem dispatcher header, response with extended information + status
*/
typedef struct                
{
	ftm_extended_request_header_type ftm_extended_header;
	word status;

} ftm_extended_response_header_type;

/*===========================================================================*/
/**
	Structure of a streaming event, as described in WCDMA ICD 3.4.95 Event Report
*/
/*===========================================================================*/

typedef struct 
{
	byte CMD_CODE;		//!<' (96) for a streaming event
	word iLength;		//!<' # of bytes in the EVENTS field
	byte aiEvents[ DIAG_MAX_PACKET_SIZE - 3];	//!<' EVENTS field, list of event items
} Event_struct;

/*===========================================================================*/
/**
	Structure of a the EVENT item defintion, as described in WCDMA ICD 3.4.95 Event Report
*/
/*===========================================================================*/

typedef struct 
{
	word EVENT_ID;		//!<' # of event ID
	word iLength;		//!<' # of bytes in the EVENTS field
	byte iTime[QMSL_DIAG_TIME_STAMP_SIZE];		//!<' time event was generated, this is possibly a 2-byte field
	byte iPayloadEn;	//!<' Is the payload enabled?
	byte aiPayload[DIAG_MAX_PACKET_SIZE];	//!<' payload, generally not used, size is variable

} EventItem_struct;

/**	
	Bit field maping of the EVENT_ID field
*/
typedef struct
{


	
	//! Unique event ID
	word event_id : 12;

	word reserved : 1;

	/**
		Flag to specify whether PAYLOAD _DATA is present in event
		\code
		0 = No payload data
		1 = One byte of payload data, no PAYLOAD_LENGTH field
		2 = Two bytes of payload data, no PAYLOAD_LENGTH field
		3 = One byte PAYLOAD_LENGTH field specifying length of payload data to follow it	
		\endcode
	*/
	word payload_length_flag : 2;

	/**
		\code
		0 = Full system time stamp is 8 bytes
		1 = Truncated time stamp is 2 bytes
		\endcode
	*/
	word time_length : 1;

} EventIdBitField;


/*===========================================================================*/
/**
	enum time_length_enum_type

	\brief Enumeration for the time stamp in CDMA DMSS ICD 80-V1294-1 3.4.95
*/
typedef enum 
{
	FULL_SYS_TIME = 0,
	TRUNCATED_SYS_TIME,
	MAX_SYS_TIME = 0xff
}time_length_enum_type;

/*===========================================================================*/
/**
	enum time_stamp_length

	\brief Enumeration for the time stamp length (bytes) in CDMA DMSS ICD 80-V1294-1 3.4.95
*/
typedef enum
{
	TRUNCATED_TIMESTAMP = 2,
	FULL_SYSTEM_TIMESTAMP = 8
}time_stamp_length;

/*===========================================================================*/
/**
	enum payload_length_data_flag

	\brief Enumeration for the payload_length_data_flag in CDMA DMSS ICD 80-V1294-1 3.4.95
*/
typedef enum
{
	NO_PAYLOAD_DATA = 0,
	ONE_BYTE_PAYLOAD_DATA,
	TWO_BYTE_PAYLOAD_DATA,
	VAR_LENGTH_PAYLOAD_DATA,
	MAX_PAYLOAD_DATA = 0xff
}payload_length_data_flag;



/*===========================================================================*/
/**
	struct QSML_Event_Element_Struct

	\brief Structure to define how QMSL will store events in the event queue
*/
#define QMSL_EVENT_PAYLOAD_DATA_MAX 256
typedef struct
{
	unsigned char time_length;	//!< # of bytes used to store type, 8 = full system time (FULL_SYSTEM_TIMESTAMP), 2=Truncated time (TRUNCATED_TIMESTAMP)
	unsigned char time[QMSL_DIAG_TIME_STAMP_SIZE];		//!< 8 or 2 bytes of system time, format depends upon time_length field
	unsigned short event_id;	//!< 12-bit unique ID of event
	unsigned char payload_len;	//!< # of bytes stored in payload_len
	unsigned char payload_data[ QMSL_EVENT_PAYLOAD_DATA_MAX ];	//!< payload data, payload_len field determines # of valid bytes
} QMSL_Event_Element_Struct;


//! Wildcard event ID
#define QMSL_EVENT_ID_WILDCARD 0xFFFF

/******************************************************************************
						Diagnostic - Specific log messages
*******************************************************************************/


/*===========================================================================*/
/**
	Structure for interpretting log code 0x7005, MMGPS report measurement

		This log message has an array of these items.  The overall structure
		is defined by MMGPS_ReportMeasurement.
*/
/*===========================================================================*/
typedef struct 
{
	byte	SV_ID;
	byte	HEALTH;
	byte	ELEVATION;
	byte	AZIMUTH;
	word	SNR;
	word	CNO;
	word	LATENCY_MS;
	byte	PRE_INT;
	word	POST_INT;
	dword	MILLISECOND;
	dword	SUB_MILLISECOND;
	word	SV_TIME_UNC;
	dword	SV_SPEED;
	word	SV_SPEED_UNC;
	word	MEAS_STATUS;
	byte	CHANNEL_STATE;
	byte	GOOD_OBSERVATIONS;
	byte	TOTAL_OBSERVATIONS;
	word	RESERVED;
} MMGPS_ReportMeasurement_MeasurementRow;


#define DIAG_MMGPS_NUM_MEASUREMENT_REPORTS 13
/*===========================================================================*/
/**
	Structure for interpretting log code 0x7005, MMGPS report measurement

*/
/*===========================================================================*/
typedef struct 
{
	dword	FCOUNT;
	MMGPS_ReportMeasurement_MeasurementRow aMeasurements[ DIAG_MMGPS_NUM_MEASUREMENT_REPORTS ];
} MMGPS_ReportMeasurement;


// Added structure for 0x701f measurement reports

/*===========================================================================*/
/**
	Structure for interpretting log code 0x701f, MMGPS report measurement

		This log message has an array of these items.  The overall structure
		is defined by MMGPS_ReportMeasurement.
*/
/*===========================================================================*/
typedef struct 
{
	byte	SV_ID;
	byte	HEALTH;
	byte	ELEVATION;
	byte	AZIMUTH;
	word	SNR;
	word	CNO;
	word	LATENCY_MS;
	byte	PRE_INT;
	word	POST_INT;
	dword	MILLISECOND;
	dword	SUB_MILLISECOND;
	word	SV_TIME_UNC;
	dword	SV_SPEED;
	word	SV_SPEED_UNC;
	word	MEAS_STATUS;
	byte	CHANNEL_STATE;
	byte	GOOD_OBSERVATIONS;
	byte	TOTAL_OBSERVATIONS;
	dword	RESERVED;		// The size of this field is the difference between version 1 and 3
} MMGPS_ReportMeasurement_MeasurementRow_V3;

#define DIAG_MMGPS_NUM_MEASUREMENT_REPORTS_V3 32 

// Added structure for 0x701f measurement reports

/*===========================================================================*/
/**
	Structure for interpretting log code 0x701f, MMGPS report measurement

*/
/*===========================================================================*/
typedef struct 
{
	dword	FCOUNT;
	word LENGTH;
	byte VERSION;
	byte RESERVE1[8];
	byte NUMBER_SVS;	// 1 to 32

	MMGPS_ReportMeasurement_MeasurementRow aMeasurements[ DIAG_MMGPS_NUM_MEASUREMENT_REPORTS_V3 ];
} MMGPS_ReportMeasurement_V3;

/**
	Structure of the Bluetooth local version, as defined in "BLUETOOTH SPECIFICATION Version 1.1"
*/
typedef struct
{
	byte  BT_EventHeader[6];	//!<'  Header information associated with the Bluetooth event
	byte  BT_Status;			//!<'  Should == BT_BE_SUCCESS == 0x00
	byte  HCI_Version;			//!<'  Should == (BT_HCI_VERSION_1_1 == 0x01) ||(BT_HCI_VERSION_1_2 = 0x02)
	word HCI_Revision;			//!<'  Should == 0
	byte  LMP_Version;			//!<'  For 1.1: (BT_LMP_VERSION_1_0 == 0x01), for 2.0: BT_LMP_VERSION_1_2
	word Manufacturer_Name;		//!<'  For QCOM: 29 (0x1D)
	word LMP_Subversion;		//!<'  BT_LMP_SUB_VER_NUM_0 == 0
} Bluetooth_LocalVersion_struct;


//! Maximum size of a QMSL Text Log
#define QLIB_MAX_TEXT_LOG_SIZE 1000

/**
	Definition of the LOG code #id that will be used to indicate that a certain log is actually
	a QMSL Text log, not a normal phone log.

	The idea is that text messages from the library will be sent the the QMSL client using the
	asynchronous log mechanism.  Log messages will be generated of command code #16, which is
	defined in the parimary ICD's for CDMA and WCDMA.

*/
#define QLIB_TEXT_LOG_CODE 0xFFFF

/**
	Structure of the QMSL text messages, which are sent through the async logging interface
*/
typedef struct
{
	/**
		Log level is a 16-bit field that indicates what level this log code is.

		The levels are defined in the pre-processor constants list that includes 
		LOG_IO and LOG_FN.

			#define LOG_IO			0x0001	// data IO (data bytes)
			#define LOG_FN			0x0002	// function calls with parameters
		
	*/
	word iLogLevel;									
	word iReserved;									//!<'  Reserved for future use
	long iTimer_ms;									//!<'  # of milliseconds since the log was started
	byte  sLogText[ QLIB_MAX_TEXT_LOG_SIZE ];		//!<'  The text string 

} QMSL_TextLog_struct;

/******************************************************************************
						Diagnostic - phone version info
*******************************************************************************/

/*============================================================================

PACKET   diag_verno_rsp_type

ID       DIAG_VERNO_F

PURPOSE  Sent by the DMSS, contains version and date information

============================================================================*/
#define QLIB_VERNO_DATE_STRLEN 11				//!<' Length of date field (bytes)
#define QLIB_VERNO_TIME_STRLEN  8				//!<' Length of time field (bytes)
#define QLIB_VERNO_DIR_STRLEN   8				//!<' Length of version directory (bytes)


/*===========================================================================*/
/**
	\brief Version information structure.

	To be used with the QLIB_DIAG_VERNO_F command.
*/
/*===========================================================================*/
struct QLIB_PhoneResp_Version
{
  byte cmd_code;                              //!<' Command code               
  char comp_date[ QLIB_VERNO_DATE_STRLEN ];   //!<' Compile date Jun 11 1991   
  char comp_time[ QLIB_VERNO_TIME_STRLEN ];   //!<' Compile time hh:mm:ss      
  char rel_date [ QLIB_VERNO_DATE_STRLEN ];   //!<' Release date               
  char rel_time [ QLIB_VERNO_TIME_STRLEN ];   //!<' Release time               
  char ver_dir  [ QLIB_VERNO_DIR_STRLEN ];    //!<' Version directory          
  byte scm;                                   //!<' Station Class Mark         
  byte mob_cai_rev;                           //!<' CAI rev                    
  byte mob_model;                             //!<' Mobile Model               
  word mob_firm_rev;                          //!<' Firmware Rev               
  byte slot_cycle_index;                      //!<' Slot Cycle Index           
  byte voc_maj;                               //!<' Vocoder major version
  byte voc_min;								  //!<' Vocoder minor version
};


/******************************************************************************
		Diagnostic - reading embedded error information
*******************************************************************************/

/*===========================================================================*/
/**
	\brief Error record information structure.

	To be used with the QLIB_DIAG_ERR_READ_F command.
*/
/*===========================================================================*/
typedef struct 
{
	/**
		Error address.  Index (0-19) of this error record; sequential		
	*/
	byte iAddress;			
									
	/**
		Error count
		Number of times this error has been recorded. If zero, this error
		record is empty; this saturates at 255		
	*/
	byte iCount;			
									
	/**
		Root name of file.
		The root name of the file where the error was detected, such as
		mobile for mobile.c. Longfile names are truncated to the first
		eight characters		
	*/
	byte sFileName[ 8 ];	
									
	/**
		Line number within file.
		The line number within FILE_NAME where the error was detected		
	*/
	word iLineNum;		

	/**
		Error fatal indicator
		If nonzero, the error was fatal. If zero, the error was nonfatal		
	*/
	byte bFatal;			

} QLIB_DIAG_Err_Read_Element;

#define QLIB_NUM_ERR_READ_ELEMENTS 20

/*===========================================================================*/
/**
	\brief Error record information structure.

	To be used with the QLIB_DIAG_ERR_READ_F command.
*/
/*===========================================================================*/
typedef struct 
{
	/**
		Error count.  The number of ERR_RECORDS that have valid error data.
	*/
	word iErrorCount;		

	/**
		Errors ignored count
		Number of errors ignored since last DMSS reset. This happens when another 
		error record cannot be allocated, or the COUNT for a given error is saturated.
		ERR_RECORDS = 20 of the following five records
	*/
	word iErrorIgnored;

	//! Array of error read elements
	QLIB_DIAG_Err_Read_Element aErrorRecords[ QLIB_NUM_ERR_READ_ELEMENTS ];
} QLIB_DIAG_Err_Read;


/******************************************************************************
						Diagnostic - General command defines
*******************************************************************************/

/*===========================================================================*/
/**
PACKET   diag_control_req_type

ID       DIAG_CONTROL_F

PURPOSE  Sent by DM to direct the DMSS to go offline or reset
         These are defined in services/cm/cmdiag.h

RESPONSE DMSS changes mode or resets
*/
/*===========================================================================*/
typedef enum  
{
    MODE_OFFLINE_A_F = 0,	//!<' Go to offline analog
    MODE_OFFLINE_D_F = 1,	//!<' Go to offline digital 
    MODE_RESET_F = 2,		//!<' Reset. Only exit from offline 
    MODE_FTM_F = 3,			//!<' FTM mode
    MODE_ONLINE_F = 4,		//!<' Go to Online 
	MODE_LPM_F = 5,			//!<' Low Power Mode (if supported)
	MODE_POWER_OFF_F = 6,	//!<' Power off (if supported)
    MODE_MAX_F = 7			//!<' Last (and invalid) mode enum value

} mode_enum_type;


/*===========================================================================*/
/**
	enum nv_stat_enum_type

	\brief States defined as possible return values for an NV function.

	This definition was taken from NV.H and should be resync'd if the list in that 
	file changes.
/*
/*===========================================================================*/
typedef enum 
{
    NV_DONE_S,          //!<' Request completed okay 
    NV_BUSY_S,          //!<' Request is queued
    NV_BADCMD_S,        //!<' Unrecognizable command field
    NV_FULL_S,          //!<' The NVM is full
    NV_FAIL_S,          //!<' Command failed, reason other than NVM was full 
    NV_NOTACTIVE_S,     //!<' Variable was not active
    NV_BADPARM_S,       //!<' Bad parameter in command block
    NV_READONLY_S,      //!<' Parameter is write-protected and thus read only
    NV_BADTG_S,         //!<' Item not valid for Target 
    NV_NOMEM_S,         //!<' free memory exhausted
    NV_NOTALLOC_S,      //!<' address is not a valid allocation
    NV_STAT_ENUM_PAD = 0x7FFF     //!<' Pad to 16 bits on ARM
} nv_stat_enum_type;


/*===========================================================================*/
/**
Enums for the return values of the command DIAG_STATUS_F


*/
/*===========================================================================*/
typedef enum  
{
    DIAG_STATUS_OFFLINE = 0,	//!<' OFFLINE Status
    DIAG_STATUS_ONLINE = 1	,	//!<' ONLINE Status
    DIAG_STATUS_LPM = 2	,		//!<' Low Power Status
    DIAG_STATUS_UNKNOWN = 99	//!<' Last (and invalid) mode enum value
} diag_status_enum_type;


/*===========================================================================
                       NV Item enumerations

  The NV ID's are not published in QMSL, in order to avoid synchronization
  issues with new builds of software.  The enumerations should be taken
  from the SERVCIES\NV\NV.H file in the embedded software, which will always 
  have the correct and up-to-date enumeration ID's for a specific target.
===========================================================================*/


/*===========================================================================*/
/**
	enum KeyPressID

	\brief Enumeration of key press identifiers

	These definitions came from PHONEI.H.  If more key press ID's are added then
	the list should be resync'd
*/
/*===========================================================================*/
typedef enum  
{						
  HS_NONE_K = 0,        //!<'  No more keycodes available              
  HS_ON_HOOK_K,         //!<'  phone has gone on-hook                  
  HS_OFF_HOOK_K,        //!<'  phone has gone off-hook                 
  HS_RING_VOL_0_K,      //!<'  ringer volume 0 (Keep 0-2 in order)     
  HS_RING_VOL_1_K,      //!<'  ringer volume 1 ( and sequential!!)     
  HS_RING_VOL_2_K,      //!<'  ringer volume 2                         
  HS_EAR_UP_K,          //!<'  earpiece up                             
  HS_EAR_UP_END_K,      //!<'  earpiece up + end pressed               
  HS_EAR_DOWN_K,        //!<'  earpiece down                           
  HS_PF1_K,             //!<'  softkey #1 (left-most)                  
  HS_PF2_K,             //!<'  softkey #2 (right-most)                 
  HS_MSG_K,             //!<'  message waiting                         
  HS_POUND_K = 0x23,    //!<'  '#' key, ASCII '#'                      
  HS_STAR_K = 0x2A,     //!<'  '*' key, ASCII '*'                      
  HS_0_K = 0x30,        //!<'  '0' key, ASCII '0'                      
  HS_1_K = 0x31,        //!<'  '1' key, ASCII '1'                      
  HS_2_K = 0x32,        //!<'  '2' key, ASCII '2'                      
  HS_3_K = 0x33,        //!<'  '3' key, ASCII '3'                      
  HS_4_K = 0x34,        //!<'  '4' key, ASCII '4'                      
  HS_5_K = 0x35,        //!<'  '5' key, ASCII '5'                      
  HS_6_K = 0x36,        //!<'  '6' key, ASCII '6'                      
  HS_7_K = 0x37,        //!<'  '7' key, ASCII '7'                      
  HS_8_K = 0x38,        //!<'  '8' key, ASCII '8'                      
  HS_9_K = 0x39,        //!<'  '9' key, ASCII '9'                      
  HS_SEND_K = 0x50,     //!<'  Send key                                
  HS_END_K = 0x51,      //!<'  End key or Power key (Based on Target)  
  HS_CLR_K = 0x52,      //!<'  Clear key                               
  HS_STO_K = 0x53,      //!<'  Store key                               
  HS_UP_K = 0x54,       //!<'  Up-arrow key was pressed                
  HS_DOWN_K = 0x55,     //!<'  Down-arrow key was pressed              
  HS_MUTE_K = 0x56,     //!<'  Mute Key                                
  HS_RCL_K = 0x57,      //!<'  Recall key                              
  HS_SD1_K = 0x58,      //!<'  speed dial #1                           
  HS_SD2_K = 0x59,      //!<'  speed dial #2                           
  HS_SD3_K = 0x5A,      //!<'  speed dial #3                           
  HS_MENU_K = 0x5B,     //!<'  Menu key                                
  HS_ALPHA_K = 0x5C,    //!<'  Alpha key                               
  HS_DEC_POUND_K,       //!<'  Decoded # DTMF                          
  HS_DEC_STAR_K,        //!<'  Decoded * DTMF                          
  HS_DEC_0_K,           //!<'  Decoded 0 DTMF                          
  HS_DEC_1_K,           //!<'  Decoded 1 DTMF                          
  HS_DEC_2_K,           //!<'  Decoded 2 DTMF                          
  HS_DEC_3_K,           //!<'  Decoded 3 DTMF                          
  HS_DEC_4_K,           //!<'  Decoded 4 DTMF                          
  HS_DEC_5_K,           //!<'  Decoded 5 DTMF                          
  HS_DEC_6_K,           //!<'  Decoded 6 DTMF                          
  HS_DEC_7_K,           //!<'  Decoded 7 DTMF                          
  HS_DEC_8_K,           //!<'  Decoded 8 DTMF                          
  HS_DEC_9_K,           //!<'  Decoded 9 DTMF                          
  HS_DEC_A_K,           //!<'  Decoded A DTMF                          
  HS_DEC_B_K,           //!<'  Decoded B DTMF                          
  HS_DEC_C_K,           //!<'  Decoded C DTMF                          
  HS_DEC_D_K,           //!<'  Decoded D DTMF                          
  HS_PWR_ON_K,          //!<'  Power key is switched 'on'              
  HS_PWR_OFF_K,         //!<'  Power key is switched 'off'             
  HS_PWR_K,             //!<'  Power key                               
  HS_INFO_K,            //!<'  Info key                                
  HS_FREE_K,            //!<'  Phone was placed in hands-free cradle   
  HS_PHONE_K,           //!<'  Phone was lifted from hands-free cradle 
  HS_IGN_OFF_K,         //!<'  Ignition was turned off                 
  HS_IGN_ON_K,          //!<'  Ignition was turned on                  
  HS_TMR_ON_K,          //!<'  Power-up timer 'on' pseudo-key          
  HS_TMR_OFF_K,         //!<'  Power-up timer 'off' pseudo-key         
  HS_BAD_BATT_K,        //!<'  The car battery is failing              
  HS_EXT_PWR_ON_K,      //!<'  External power was turned on            
  HS_EXT_PWR_OFF_K,     //!<'  External power was turned off           
  HS_REDIAL_K,          //!<'  Redial key                              
  HS_RSSI_K,            //!<'  RSSI key                                
  HS_HFK_CORD_K,        //!<'  Coil cord carkit                        
  HS_HFK_NOCORD_K,      //!<'  Non-coil cord carkit                    
  HS_HEADSET_K,         //!<'  Headset connected                       
  HS_HEADSET_OFF_K,     //!<'  Headset disconnected                    
  HS_DEV_UNKNOWN_K,     //!<'  Unknown device ID                       
  HS_EXT_DEV_K,         //!<'  ONES detector finds external device     
  HS_CHARGING_ON_K,     //!<'  Key to note battery charging active     
  HS_CHARGING_OFF_K,    //!<'  Key to note battery charging stopped    
  HS_LEFT_K,			//!<'  Left soft Key
  HS_RIGHT_K,			//!<'  Right soft Key
  HS_APPLICATION_K,		//!<'  Application key
  HS_RIGHT_SELECT_K,	//!<'  Right hand select key
  HS_LEFT_SELECT_K,		//!<'  Left hand select key
  HS_BROWSER_K,			//!<'  Browser key
  HS_LO_RES_CAMERA_K,	//!<'  Low resolution Camera key
  HS_HI_RES_CAMERA_K,	//!<'  High resolution Camera key
  HS_PTT_K,				//!<'  Push to talk
  HS_RELEASE_K = 0xff   //!<'  Key to note that all keys are up        
} KeyPressID;

/*===========================================================================*/
/**
	enum KeyPressEvent

	\brief Enumeration of key press identifiers

	These definitions came from PHONEI.H.  If more key press ID's are added then
	the list should be resync'd
*/
/*===========================================================================*/
typedef enum  
{						
	DIAG_KeyPress_Down		= 1,
	DIAG_KeyPress_Up		= 2,
	DIAG_KeyPress_Unknown	= 3
} KeyPressEventType;


//! Structure for defining a key press event
typedef struct
{
	KeyPressID eKeyID;
	KeyPressEventType eKeyEvent;
} KeyPressEvent;



/******************************************************************************
						Diagnostic - EFS Subsystem
*******************************************************************************/


/**
	EFS sub system commands
*/
typedef enum
{
	_EFS2_DIAG_HELLO	= 0,		//!<' Parameter negotiation packet
	_EFS2_DIAG_QUERY	= 1,		//!<' Send information about EFS2 params
	_EFS2_DIAG_OPEN		= 2,		//!<' Open a file
	_EFS2_DIAG_CLOSE	= 3,		//!<' Close a file
	_EFS2_DIAG_READ		= 4,		//!<' Read a file
	_EFS2_DIAG_WRITE	= 5,		//!<' Write a file
	_EFS2_DIAG_SYMLINK	= 6,		//!<' Create a symbolic link
	_EFS2_DIAG_READLINK	= 7,		//!<' Read a symbolic link
	_EFS2_DIAG_UNLINK	= 8,		//!<' Remove a symbolic link or file
	_EFS2_DIAG_MKDIR	= 9,		//!<' Create a directory
	_EFS2_DIAG_RMDIR	= 10,		//!<' Remove a directory
	_EFS2_DIAG_OPENDIR	= 11,		//!<' Open a directory for reading               */
	_EFS2_DIAG_READDIR	= 12,		//!<' Read a directory                           */
	_EFS2_DIAG_CLOSEDIR	= 13,		//!<' Close an open directory                    */
	_EFS2_DIAG_RENAME	= 14,		//!<' Rename a file or directory                 */
	_EFS2_DIAG_STAT		= 15,		//!<' Obtain information about a named file      */
	_EFS2_DIAG_LSTAT	= 16,		//!<' Obtain information about a symbolic link   */
	_EFS2_DIAG_FSTAT	= 17,		//!<' Obtain information about a file descriptor */
	_EFS2_DIAG_CHMOD	= 18,		//!<' Change file permissions                    */
	_EFS2_DIAG_STATFS	= 19,		//!<' Obtain file system information             */
	_EFS2_DIAG_ACCESS	= 20,		//!<' Check a named file for accessibility       */
	_EFS2_DIAG_NAND_DEV_INFO	= 21,	//!<' Get NAND device info */
	_EFS2_DIAG_FACT_IMAGE_START	= 22,	//!<' Start EFS2 data output for Factory Image */
	_EFS2_DIAG_FACT_IMAGE_READ	= 23,	//!<' Get EFS2 data for Factory Image */
	_EFS2_DIAG_FACT_IMAGE_END	= 24,	//!<' End EFS2 data output for Factory Image */
	_EFS2_DIAG_PREP_FACT_IMAGE	= 25,	//!<' Prepare file system for image dump */
	_EFS2_DIAG_PUT		= 26,			//!<' 
	_EFS2_DIAG_GET		= 27,			//!<' 
	_EFS2_DIAG_ERROR	= 28,			//!<' 
	_EFS2_DIAG_EXTENDED_INFO	= 29	//!<' Get Extra information. */

} EFS2_CMD_Enum;

/**
	Data types of extended EFS information--string or integer
*/
typedef enum
{
	EFS2_EXT_Kind_Int		= 0,	//!<' Extended Information will be returned as an integer
	EFS2_EXT_Kind_String	= 1		//!<' ExtendedInformation will be returned as a string

} EFS2_EXT_Kind_Enum;

/**
	Currently defined queries. Querying information that is not available
	will return an efs2_diag_error_rsp_type packet with the error code set
	to FS_DIAG_UNAVAILABLE_INFO. All queries expect 'path' to be set to an
	EFS2 path that determines which filesystem the query applies to. The
	path must be null terminated.

*/


/**
	Types of extended EFS information queries
*/
typedef enum
{
	/**
	Return the flash device name. 'data' is ignored Results in a string.
	*/
	EFS2_DIAG_EXT_QUERY_DEVICE_NAME			= 0,

	/**
		Return a field of the device ID. 'data' indicates which ID
		field to return. '0' is always the Maker ID, and '1' begins the
		device-specific ID. The client should always request values
		until the FS_DIAG_UNAVAILABLE_INFO is returned, indicating there
		are no more ID values.
	*/
	EFS2_DIAG_EXT_QUERY_DEVICE_ID			= 1,

	/**
		Return the number of blocks in this EFS2 partition. These are
		FLASH blocks (called sectors for NOR). 'data' is ignored. 
	*/
	EFS2_DIAG_EXT_QUERY_FILESYSTEM_BLOCKS	= 2,


	/**
		Return the total number of blocks in the flash device containing
		this filesystem. 'data' is ignored.
	*/
	EFS2_DIAG_EXT_QUERY_TOTAL_BLOCKS		= 3,

	/**
		Return the number of pages per block for this device. This
		query is only valid for devices that have a fixed block size.
		For devices with varying block sizes, the TBD request must be
		used. 'data' is ignored. 		
	*/
	EFS2_DIAG_EXT_QUERY_PAGES_PER_BLOCK		= 4,

	/**
		Return the number of bytes in a single page for this device.
		'data' is ignored'. 
	*/
	EFS2_DIAG_EXT_QUERY_PAGE_SIZE			= 5,

	/**
		Return the bus width of the flash device used in this
		filesystem. 'data' is ignored. 
	*/
	EFS2_DIAG_EXT_QUERY_FLASH_BUS_WIDTH		= 6,


	/**
		Return string description of flash type ("NAND", "NOR", etc). 
	*/
	EFS2_DIAG_EXT_QUERY_FLASH_TYPE			= 7

} EFS2_EXT_QueryType_Enum;


/******************************************************************************
						Diagnostic - NV Subsystem
*******************************************************************************/

/**
	Multiple NV write Subsystem commands
*/
typedef enum
{
	_DIAG_NV_MNVW_Write = 0
} MNVW_CMD_Enum;

// Maximum payload size for multiple NV write.  Leave 100 bytes margin so that HDLC characters 
// can be expand with escape sequences.  This still might not be enough if one NV field has 
// numerous 0x7e or 0x7d values
#define DIAG_MNVW_MAX_PAYLOAD_SIZE 1520  - 100

/**
	Status types for Multiple NV Write
*/
typedef enum
{
	DIAG_MNVW_Status_OK,				//!<' 0 - Request NV write items completed OK
	DIAG_MNVW_Status_InvalidTotal,		//!<' 1 - Request total item number invalid
	DIAG_MNVW_Status_InvalidLength,		//!<' 2 - Request total item length limit exceeded
	DIAG_MNVW_Status_MismatchLength,	//!<' 3 - Request item lengths mismatched with total item length
	DIAG_MNVW_Status_Error				//!<' 4 - Request NV write items failed with ERROR
} MNVW_Status_Enum;

/**
	Structure for MNV Diag Command response header
*/
typedef struct
{
	byte  iCMD_Code;
	byte  iSubSysId;
	word iSubSysCmdCode;
	word iNvItemTotal; 
	word iNvItemTotalLength; 
	word iNvStatusTotal; 
	byte iNvItemDataTotal;

} MVNW_Response_Struct;

/**
	Structure for individual NV items in the MNV Diag Command response 
*/
typedef struct
{
	byte  iNvItemLength;
	word iNvItem;
	byte  iNvItemStatus;
	byte  iNvItemData; 
} MVNW_Response_NvItem_Struct;

/**
	CNV Subsystem commands
*/
typedef enum
{
	_DIAG_NV_CNV_DIAG_BACKUP_F = 0
} CNV_CMD_Enum;

/**
	Structure for Diag  Command version 2 responses
*/
typedef struct
{
	byte  iCMD_Code;
	byte  iSubSysId;
	word iSubSysCmdCode;
	dword  iCommandStatus; 
	word iDelayedResponseId;
	word iDelayedResponseCount;

} DIAG_CMD_Ver2_Resp;

/**
	For the "command status" result on Diag Version 2 commands.
*/
#define DIAG_V2_SUCCESS 0 

/**
	For multiple NV Write,
*/
#define NV_WRITE_BATCH_ITEM_NO_ERROR -1

//	---------------------------------------------------------
//	defines for log codes
//
//	---------------------------------------------------------

//! AAGPS Measurement log
#define _DIAG_LOG_AAGPS_MEASUREMENT 0x7005

// Added following for 0x701f measurement reports
// AAGPS Measurement log version 3
#define _DIAG_LOG_AAGPS_MEASUREMENT_V3 0x701F



/******************************************************************************
						Diagnostic - GSDI Subsystem
*******************************************************************************/

//! GSDI Generic command, for the SUBSYS_CMD_CODE field
#define GSDIDIAG_GENERIC_COMMAND 0X00000000

/**
	GSDIDIAG Commands
*/
typedef enum 
{
    //GSDIDIAG_MIN_CMD =-1
    _GSDIDIAG_SIM_READ_CMD                       = 0x00,
    _GSDIDIAG_SIM_WRITE_CMD                      = 0x01,
    _GSDIDIAG_SIM_INCREASE_CMD                   = 0x02,
    _GSDIDIAG_GET_FILE_ATTRIBUTES_CMD            = 0x04,
    _GSDIDIAG_GET_PIN_STATUS_CMD                 = 0x05,
    _GSDIDIAG_VERIFY_PIN_CMD                     = 0x06,
    _GSDIDIAG_CHANGE_PIN_CMD                     = 0x07,
    _GSDIDIAG_UNBLOCK_PIN_CMD                    = 0x08,
    _GSDIDIAG_DISABLE_PIN_CMD                    = 0x09,
    _GSDIDIAG_ENABLE_PIN_CMD                     = 0x0A,
    _GSDIDIAG_GET_SIM_CAPABILITES_CMD            = 0x0C,
    _GSDIDIAG_ILLEGAL_SIM_CMD                    = 0x0F,
    _GSDIDIAG_SIM_SEARCH_CMD                     = 0x10,
    _GSDIDIAG_SELECT_CMD                         = 0x12,
    _GSDIDIAG_GET_IMAGE_CMD                      = 0x14,
    _GSDIDIAG_SIM_REFRESH_CMD                    = 0x15,
    _GSDIDIAG_GET_AVAILABLE_APPS_CMD             = 0x17,
    _GSDIDIAG_ACTIVATE_FEATURE_INDICATOR_CMD     = 0x40,
    _GSDIDIAG_DEACTIVATE_FEATURE_INDICATOR_CMD   = 0x41,
    _GSDIDIAG_GET_FEATURE_INDICATION_CMD         = 0x42,
    _GSDIDIAG_SET_FEATURE_INDICATOR_DATA_CMD     = 0x43,
    _GSDIDIAG_GET_FEATURE_INDICATOR_KEY_CMD      = 0x44,
    _GSDIDIAG_UNBLOCK_FEATURE_INDICATOR_CMD      = 0x45,
    _GSDIDIAG_GET_PERM_FEATURE_INDICATION_CMD    = 0x46,
    _GSDIDIAG_PERM_DISABLE_FEATURE_INDICATOR_CMD = 0x47,
    _GSDIDIAG_GENERIC_CMD                        = 0x48,
    _GSDIDIAG_GET_ATR_CMD                        = 0x49,
    _GSDIDIAG_SIM_OPEN_CHANNEL_CMD               = 0x50,
    _GSDIDIAG_SIM_CLOSE_CHANNEL_CMD              = 0x51,
    _GSDIDIAG_SIM_SEND_CHANNEL_DATA_CMD          = 0x52,
    _GSDIDIAG_ATCSIM_RAW_APDU_CMD                = 0x53,
    _GSDIDIAG_SIM_USIM_RUIM_SEARCH_CMD           = 0x54,
    _GSDIDIAG_GET_ECC_CMD                        = 0x55,
    _GSDIDIAG_SECURE_DEVICE_INFO_CMD             = 0x56,
    _GSDIDIAG_SIM_SECURE_STORE_CMD               = 0x57,
    _GSDIDIAG_ROOT_KEY_WRITE_CMD                 = 0x58,
    _GSDIDIAG_ROOT_KEY_READ_CMD                  = 0x59,
    _GSDIDIAG_SIM_SECURE_READ_CMD                = 0x5A,
    _GSDIDIAG_VERIFY_PHONE_CODE_CMD              = 0x5B,
    _GSDIDIAG_CHANGE_PHONE_CODE_CMD              = 0x5C,
    _GSDIDIAG_UNBLOCK_PHONE_CODE_CMD             = 0x5D,
    _GSDIDIAG_SET_PHONE_CODE_CMD                 = 0x5E,
    _GSDIDIAG_GET_LIB_VERSION_CMD                = 0x5F,
	_GSDIDIAG_ACTIVATE_ONCHIP_SIM_CMD            = 0x61,
    _GSDIDIAG_SEND_APDU_CMD                      = 0x99,
    _GSDIDIAG_MAX_CMD                            = 0xFF

} GSDI_DIAG_CMD_Enum;

/**
	Values for the APS_AS_TRANSPORT field
*/
typedef enum
{
	GSDI_DIAG_APS_As_Transport_CmdResponse,	//!< ' Response is in Command Response Header (Pass or Fail)
	GSDI_DIAG_APS_As_Transport_DelayedResponse	//!< ' Response is in the Response Payload.
} GSDI_DIAG_APS_AsTransport_Enum;


/**
	Header for the GSDI Response 
\code
	Field				Length
	------				------
	CMD_CODE 				1
	SUBSYS_ID				1
	SUBSYS_CMD_CODE			2
	COMMAND_STATUS			4
	DELAYED_RESPONSE_ ID	2
	DELAYED_RESPONSE_COUNT	2
	SUBCOMMAND				4
	APS_AS_TRANSPORT		4
	STATUS					4  <--- This one seems to be missing

	typedef PACKED struct gsdidiag_generic_cmd_rsp_type
	{
		typedef PACKED struct  gsdidiag_rsp_header_type
		{
			uint8  command_code;
			uint8  subsys_id;
			uint16 subsys_cmd_code;
			uint32 status;
			uint16 delayed_rsp_id;
			uint16 rsp_count;
			uint32 aps_as_transport;
		} rsp_header;

		typedef PACKED struct
		{
		   uint32 subcommand;
		   uint32 gsdi_status;
		}gsdidiag_subcmd_rsp_hdr_type;

		    
		PACKED union gsdidiag_rsp_payload_type
		{
		   gsdidiag_rsp_data_payload_type         data;
		   gsdidiag_rsp_channel_data_payload_type channel_data;
		   gsdidiag_rsp_atcsim_payload_type       atcsim_data;
		   gsdidiag_rsp_sim_read_payload_type     sim_read_data;
		   gsdidiag_rsp_sim_get_ecc_type          ecc_data;
		   uint8 num_retries;
		   byte rec_num_accessed;
		   gsdidiag_root_key_rsp_type             root_key_write;
		   gsdidiag_root_key_rsp_type             root_key_read;
		   gsdidiag_sim_secure_read_rsp_type      sim_secure_read;
		   gsdidiag_get_lib_version_rsp_type      lib_version;
		}rsp_payload;

	} response;
\endcode
*/
typedef struct
{
	byte  iCMD_Code;
	byte  iSubSysId;
	word iSubSysCmdCode;
	dword  iCommandStatus; 
	word iDelayedResponseId;
	word iDelayedResponseCount;
	dword  iApsAsTransport;
	dword  iSubCommand;
	dword  iStatus;


} GSDI_DIAG_DelayedResponseHeader;

/*
	GSDI Request Header

	typedef PACKED struct
	{
		typedef PACKED struct gsdidiag_sim_get_file_attributes_cmd_req_type
		{
			typedef PACKED struct gsdidiag_cmd_header_type
			{
				uint8 command_code;
				uint8 subsys_id;
				uint16 subsys_cmd_code;
			} header;

			uint8  sim_slot_number;
			uint8  sim_app_id_len;
			uint8  sim_app_id[GSDI_MAX_APP_ID_LEN];
			uint32 sim_elementary_file;
			char   efs_filename[1];
		} cmd_header;

		typedef PACKED struct gsdidiag_req_payload_type
		{
			typedef PACKED struct gsdidiag_subcmd_req_hdr_type
			{
			   uint32 subcommand;
			   uint32 gsdi_status;
			} sub_cmd_hdr;

			PACKED union
			{
				gsdidiag_two_pin_cmd_type      change_pin;
				gsdidiag_channel_req_cmd_type  channel_req;
				gsdidiag_sim_read_req_cmd_type sim_read_req;
				gsdidiag_sim_usim_ruim_search  sim_usim_ruim_search_req;
				gsdidiag_sim_secure_store_cmd_type sim_secure_store_req;
				gsdidiag_root_key_write_req_cmd_type root_key_write_req;
				gsdidiag_verify_phone_code_cmd_type  verify_phone_code_req;
				gsdidiag_change_phone_code_cmd_type  change_phone_code_req;
				gsdidiag_unblock_phone_code_cmd_type unblock_phone_code_req;
				gsdidiag_set_phone_code_cmd_type     set_phone_code_req;
			}request;
		}req_payload;



	}gsdidiag_generic_cmd_req_type;

*/


/**
	GSDI Response structure for commands that are not "delayed response"
*/
typedef struct
{
	byte  iCMD_Code;
	byte  iSubSysId;
	word iSubSysCmdCode;
	dword  iCommandStatus; 
	word iTransactionID;
	word iSIM_SlotNumber;

} GSDI_DIAG_ResponseHeader;


/**
	Enum of status values for GSDI commands
*/
typedef enum
{
	 GSDI_DIAG_STAT_Success				= 0,  //!< ' COMMAND COMPLETED SUCCESSFULLY
	 GSDI_DIAG_STAT_GeneralError		= 1,  //!< ' GENERAL ERROR:  No other information specified
	 GSDI_DIAG_STAT_AccessDenied		= 2,  //!< ' ACCESS DENIED:  Security Procedures performed thus far don't satisfy restrictions
	 GSDI_DIAG_STAT_NotFound			= 3,  //!< ' NOT FOUND:  SIM Elementary File or Directory File was not found.
	 GSDI_DIAG_STAT_IncompatPinStauts	= 4,  //!< ' INCOMPATIBLE PIN STATUS:  Trying to perform a PIN Action for a PIN that is not in the correct state.  For example, trying to Disable an already Disabled PIN.
	 GSDI_DIAG_STAT_IncorrectCode		= 5,  //!< ' INCORRECT CODE:  The Code provided is incorrect for the PIN ID used.
	 GSDI_DIAG_STAT_CodeBlocked			= 6,  //!< ' CODE BLOCKED:  The Number of retries has been exceeded and the PIN is now blocked.
	 GSDI_DIAG_STAT_IncreaseImpossible	= 7,  //!< ' INCREASE IMPOSSIBLE:  The INCREASE Command has failed due to a Bad Value
	 GSDI_DIAG_STAT_IncorrectParameters = 8,  //!< ' INCORRECT PARAMETERS:  The SIM detected an incorrect parameter for the action performed.
	 GSDI_DIAG_STAT_NotSupported		= 9,  //!< ' NOT SUPPORTED:  Not allowed by GSDI due to Service Table Values
	 GSDI_DIAG_STAT_NotInitialized		= 10, //!< ' NOT INIT:  Cache has not been INIT
	 GSDI_DIAG_STAT_IllegalSim			= 11, //!< ' SUCCESS BUT ILLEGAL SIM:  The SIM has been marked as an illegal SIM for the network, but the actions are succeeding on the SIM.
	 GSDI_DIAG_STAT_SimTechnicalProblems = 14,//!< ' SIM TECHNICAL PROBLEMS:  The SIM malfunctioned.
	 GSDI_DIAG_STAT_NoEfSelected		= 15, //!< ' NO EF SELECTED:  Attempted to read a file without first selecting it.
	 GSDI_DIAG_STAT_EfInconsistent		= 16, //!< ' EF INCONSISTENT:  Trying to Read a Record when the file is a Binary or similar type error.
	 GSDI_DIAG_STAT_UnknownInstClass	= 17, //!< ' UNKNOWN INST CLASS:  Trying to send a UICC Command to a GSM/RUIM Card or vice versa.
	 GSDI_DIAG_STAT_IncorrectLength		= 21, //!< ' INCORRECT LENGTH:  A Bad length was provided in the command.
	 GSDI_DIAG_STAT_DriversTimedOut		= 22, //!< ' DRIVERS TIMED OUT:  The Drivers communicating with the Card Timed Out.
	 GSDI_DIAG_STAT_CodePermBlocked		= 23  //!< ' CODE PERM BLOCKED:  The Unblock Code has been blocked.  Need Administrative assistance to unblock it.
} GSDI_DIAG_StatusEnum;

/**
	For the "command status" result on GSDI commands.
*/
#define GSDI_SUCCESS DIAG_V2_SUCCESS

/**
	Enum of dealyed response counts GSDI commands
*/
typedef enum
{
	GSDI_DIAG_ResponseCount_First				= 0,	//!< ' 0 - First Response (Immediate)
	GSDI_DIAG_ResponseCount_Delayed				= 1		//!< ' 1 - Second Response  (Delayed)

} GSDI_DIAG_ResponseCountEnum;

/**
	Enum for GSDI SIM Index mode 

	Copied from embedded source code file gsdidiag.h
*/
typedef enum
{
	GSDI_DIAG_IndexNone		= 0x00000000,
	GSDI_DIAG_IndexAbsolute = 0x00000001,
	GSDI_DIAG_IndexCurrent	= 0x00000002,
	GSDI_DIAG_IndexNext		= 0x00000003,
	GSDI_DIAG_IndexPrevious = 0x00000004
} GSDI_DIAG_SIM_IndexMode;


/**
    Enumerated type:  gsdidiag_elementary_file_enum_type
      Define Filenames Available through GSDI - DIAG Interface
      CDMA RUIM Related Files Range:  0x00000001 - 0x000000FF
      GSM  SIM  Related Files Range:  0x00001000 - 0x0000FF00
      UMTS USIM Related Files Range:  0x00010000 - 0x00FF0000
      TELECOM   Related Files Range:  0x01000000 - 0xFF000000

  Copied from embedded source code file gsdidiag.h
*/

typedef enum
{
    //GSDIDIAG_MIN_CMD =-1

    GSDIDIAG_MF_ICCID                             = 0x00000001,
    GSDIDIAG_MF_ELP                               = 0x00000002,
    GSDIDIAG_MF_DIR                               = 0x00000003,
    GSDIDIAG_MF_ARR                               = 0x00000004,
    GSDIDIAG_MF_PL                                = 0x00000005,

    GSDIDIAG_CDMA_CC                              = 0x00000100,
    GSDIDIAG_CDMA_IMSI_M                          = 0x00000101,
    GSDIDIAG_CDMA_IMSI_T                          = 0x00000102,
    GSDIDIAG_CDMA_ANALOG_HOME_SID                 = 0x00000103,
    GSDIDIAG_CDMA_ANALOG_OP_PARAMS                = 0x00000104,
    GSDIDIAG_CDMA_ANALOG_LOCN_AND_REGN_IND        = 0x00000105,
    GSDIDIAG_CDMA_HOME_SID_NID                    = 0x00000106,
    GSDIDIAG_CDMA_ZONE_BASED_REGN_IND             = 0x00000107,
    GSDIDIAG_CDMA_SYS_REGN_IND                    = 0x00000108,
    GSDIDIAG_CDMA_DIST_BASED_REGN_IND             = 0x00000109,
    GSDIDIAG_CDMA_ACCOLC                          = 0x0000010A,
    GSDIDIAG_CDMA_CALL_TERM_MOD_PREF              = 0x0000010B,
    GSDIDIAG_CDMA_SCI                             = 0x0000010C,
    GSDIDIAG_CDMA_ANALOG_CHAN_PREF                = 0x0000010D,
    GSDIDIAG_CDMA_PRL                             = 0x0000010E,
    GSDIDIAG_CDMA_RUIM_ID                         = 0x0000010F,
    GSDIDIAG_CDMA_SVC_TABLE                       = 0x00000110,
    GSDIDIAG_CDMA_SPC                             = 0x00000111,
    GSDIDIAG_CDMA_OTAPA_SPC_ENABLE                = 0x00000112,
    GSDIDIAG_CDMA_NAM_LOCK                        = 0x00000113,
    GSDIDIAG_CDMA_OTASP_OTAPA_FEATURES            = 0x00000114,
    GSDIDIAG_CDMA_SERVICE_PREF                    = 0x00000115,
    GSDIDIAG_CDMA_ESN_ME                          = 0x00000116,
    GSDIDIAG_CDMA_RUIM_PHASE                      = 0x00000117,
    GSDIDIAG_CDMA_PREF_LANG                       = 0x00000118,
    GSDIDIAG_CDMA_UNASSIGNED_1                    = 0x00000119,
    GSDIDIAG_CDMA_SMS                             = 0x0000011A,
    GSDIDIAG_CDMA_SMS_PARAMS                      = 0x0000011B,
    GSDIDIAG_CDMA_SMS_STATUS                      = 0x0000011C,
    GSDIDIAG_CDMA_SUP_SVCS_FEATURE_CODE_TABLE     = 0x0000011D,
    GSDIDIAG_CDMA_UNASSIGNED_2                    = 0x0000011E,
    GSDIDIAG_CDMA_HOME_SVC_PVDR_NAME              = 0x0000011F,
    GSDIDIAG_CDMA_UIM_ID_USAGE_IND                = 0x00000120,
    GSDIDIAG_CDMA_ADM_DATA                        = 0x00000121,
    GSDIDIAG_CDMA_MSISDN                          = 0x00000122,
    GSDIDIAG_CDMA_MAXIMUM_PRL                     = 0x00000123,
    GSDIDIAG_CDMA_SPC_STATUS                      = 0x00000124,

    GSDIDIAG_GSM_LP                               = 0x00000200,
    GSDIDIAG_GSM_IMSI                             = 0x00000201,
    GSDIDIAG_GSM_KC                               = 0x00000202,
    GSDIDIAG_GSM_PLMN                             = 0x00000203,
    GSDIDIAG_GSM_HPLMN                            = 0x00000204,
    GSDIDIAG_GSM_ACM_MAX                          = 0x00000205,
    GSDIDIAG_GSM_SST                              = 0x00000206,
    GSDIDIAG_GSM_ACM                              = 0x00000207,
    GSDIDIAG_GSM_GID1                             = 0x00000208,
    GSDIDIAG_GSM_GDI2                             = 0x00000209,
    GSDIDIAG_GSM_SPN                              = 0x0000020A,
    GSDIDIAG_GSM_PUCT                             = 0x0000020B,
    GSDIDIAG_GSM_CBMI                             = 0x0000020C,
    GSDIDIAG_GSM_BCCH                             = 0x0000020D,
    GSDIDIAG_GSM_ACC                              = 0x0000020E,
    GSDIDIAG_GSM_FPLMN                            = 0x0000020F,
    GSDIDIAG_GSM_LOCI                             = 0x00000210,
    GSDIDIAG_GSM_AD                               = 0x00000211,
    GSDIDIAG_GSM_PHASE                            = 0x00000222,
    GSDIDIAG_GSM_VGCS                             = 0x00000223,
    GSDIDIAG_GSM_VGCSS                            = 0x00000224,
    GSDIDIAG_GSM_VBS                              = 0x00000225,
    GSDIDIAG_GSM_VBSS                             = 0x00000226,
    GSDIDIAG_GSM_EMLPP                            = 0x00000227,
    GSDIDIAG_GSM_AAEM                             = 0x00000228,
    GSDIDIAG_GSM_CBMID                            = 0x00000229,
    GSDIDIAG_GSM_ECC                              = 0x0000022A,
    GSDIDIAG_GSM_CBMIR                            = 0x0000022B,
    GSDIDIAG_GSM_DCK                              = 0x0000022C,
    GSDIDIAG_GSM_CNL                              = 0x0000022D,
    GSDIDIAG_GSM_NIA                              = 0x0000022E,
    GSDIDIAG_GSM_KCGPRS                           = 0x0000022F,
    GSDIDIAG_GSM_LOCIGPRS                         = 0x00000230,
    GSDIDIAG_GSM_SUME                             = 0x00000231,
    GSDIDIAG_GSM_PLMNWAT                          = 0x00000232,
    GSDIDIAG_GSM_OPLMNWACT                        = 0x00000233,
    GSDIDIAG_GSM_CPBCCH                           = 0x00000234,
    GSDIDIAG_GSM_INVSCAN                          = 0x00000235,
    GSDIDIAG_GSM_RPLMNACT                         = 0x00000236,
    GSDIDIAG_GSM_SAI                              = 0x00000237,
    GSDIDIAG_GSM_SLL                              = 0x00000238,
    GSDIDIAG_GSM_MEXE_ST                          = 0x00000239,
    GSDIDIAG_GSM_ORPK                             = 0x0000023A,
    GSDIDIAG_GSM_ARPK                             = 0x0000023B,
    GSDIDIAG_GSM_TPRPK                            = 0x0000023C,
    GSDIDIAG_GSM_IMG                              = 0x0000023D,
    GSDIDIAG_GSM_IMG1INST1                        = 0x0000023E,
    GSDIDIAG_GSM_IMG1INST2                        = 0x0000023F,
    GSDIDIAG_GSM_IMG1INST3                        = 0x00000240,
    GSDIDIAG_GSM_IMG2INST1                        = 0x00000241,
    GSDIDIAG_GSM_IMG2INST2                        = 0x00000242,
    GSDIDIAG_GSM_IMG2INST3                        = 0x00000243,
    GSDIDIAG_GSM_IMG3INST1                        = 0x00000244,
    GSDIDIAG_GSM_IMG3INST2                        = 0x00000245,
    GSDIDIAG_GSM_IMG3INST3                        = 0x00000246,
    GSDIDIAG_GSM_HPLMNWACT                        = 0x00000247,

    GSDIDIAG_TELECOM_ADN                          = 0x00000300,
    GSDIDIAG_TELECOM_FDN                          = 0x00000301,
    GSDIDIAG_TELECOM_SMS                          = 0x00000302,
    GSDIDIAG_TELECOM_CCP                          = 0x00000303,
    GSDIDIAG_TELECOM_ECCP                         = 0x00000304,
    GSDIDIAG_TELECOM_MSISDN                       = 0x00000305,
    GSDIDIAG_TELECOM_SMSP                         = 0x00000306,
    GSDIDIAG_TELECOM_SMSS                         = 0x00000307,
    GSDIDIAG_TELECOM_LND                          = 0x00000308,
    GSDIDIAG_TELECOM_SDN                          = 0x00000309,
    GSDIDIAG_TELECOM_EXT1                         = 0x0000030A,
    GSDIDIAG_TELECOM_EXT2                         = 0x0000030B,
    GSDIDIAG_TELECOM_EXT3                         = 0x0000030C,
    GSDIDIAG_TELECOM_BDN                          = 0x0000030D,
    GSDIDIAG_TELECOM_EXT4                         = 0x0000030E,
    GSDIDIAG_TELECOM_SMSR                         = 0x0000030F,
    GSDIDIAG_TELECOM_CMI                          = 0x00000310,
    GSDIDIAG_TELECOM_SUME                         = 0x00000311,
    GSDIDIAG_TELECOM_ARR                          = 0x00000312,
    GSDIDIAG_TELECOM_PBR                          = 0x00000313,
    GSDIDIAG_TELECOM_CCP1                         = 0x00000314,
    GSDIDIAG_TELECOM_UID                          = 0x00000315,
    GSDIDIAG_TELECOM_PSC                          = 0x00000316,
    GSDIDIAG_TELECOM_CC                           = 0x00000317,
    GSDIDIAG_TELECOM_PUID                         = 0x00000318,
    GSDIDIAG_TELECOM_ADN1                         = 0x00000319,
    GSDIDIAG_TELECOM_GRP                          = 0x0000031A,
    GSDIDIAG_TELECOM_GRP1                         = 0x0000031B,
    GSDIDIAG_TELECOM_GAS                          = 0x0000031C,
    GSDIDIAG_TELECOM_SNE                          = 0x0000031D,
    GSDIDIAG_TELECOM_SNE1                         = 0x0000031E,
    GSDIDIAG_TELECOM_EMAIL                        = 0x0000031F,
    GSDIDIAG_TELECOM_EMAIL1                       = 0x00000320,
    GSDIDIAG_TELECOM_IAP                          = 0x00000321,
    GSDIDIAG_TELECOM_IAP1                         = 0x00000322,
    GSDIDIAG_TELECOM_PBC                          = 0x00000323,

    GSDIDIAG_TELECOM_GRAPHICS_IMG                 = 0x00000400,

    GSDIDIAG_USIM_LI                              = 0x00000500,
    GSDIDIAG_USIM_IMSI                            = 0x00000501,
    GSDIDIAG_USIM_KEYS                            = 0x00000502,
    GSDIDIAG_USIM_KEYSPS                          = 0x00000503,
    GSDIDIAG_USIM_PLMNWACT                        = 0x00000504,
    GSDIDIAG_USIM_UPLMNSEL                        = 0x00000505,
    GSDIDIAG_USIM_HPLMN                           = 0x00000506,
    GSDIDIAG_USIM_ACM_MAX                         = 0x00000507,
    GSDIDIAG_USIM_UST                             = 0x00000508,
    GSDIDIAG_USIM_ACM                             = 0x00000509,
    GSDIDIAG_USIM_GID1                            = 0x0000050A,
    GSDIDIAG_USIM_GID2                            = 0x0000050B,
    GSDIDIAG_USIM_SPN                             = 0x0000050C,
    GSDIDIAG_USIM_PUCT                            = 0x0000050D,
    GSDIDIAG_USIM_CMBI                            = 0x0000050E,
    GSDIDIAG_USIM_ACC                             = 0x0000050F,
    GSDIDIAG_USIM_FPLMN                           = 0x00000510,
    GSDIDIAG_USIM_LOCI                            = 0x00000511,
    GSDIDIAG_USIM_AD                              = 0x00000512,
    GSDIDIAG_USIM_CBMID                           = 0x00000513,
    GSDIDIAG_USIM_ECC                             = 0x00000514,
    GSDIDIAG_USIM_CBMIR                           = 0x00000515,
    GSDIDIAG_USIM_PSLOCI                          = 0x00000516,
    GSDIDIAG_USIM_FDN                             = 0x00000517,
    GSDIDIAG_USIM_SMS                             = 0x00000518,
    GSDIDIAG_USIM_MSISDN                          = 0x00000519,
    GSDIDIAG_USIM_SMSP                            = 0x0000051A,
    GSDIDIAG_USIM_SMSS                            = 0x0000051B,
    GSDIDIAG_USIM_SDN                             = 0x0000051C,
    GSDIDIAG_USIM_EXT2                            = 0x0000051D,
    GSDIDIAG_USIM_EXT3                            = 0x0000051E,
    GSDIDIAG_USIM_SMSR                            = 0x0000051F,
    GSDIDIAG_USIM_ICI                             = 0x00000520,
    GSDIDIAG_USIM_OCI                             = 0x00000521,
    GSDIDIAG_USIM_ICT                             = 0x00000522,
    GSDIDIAG_USIM_OCT                             = 0x00000523,
    GSDIDIAG_USIM_EXT5                            = 0x00000524,
    GSDIDIAG_USIM_CCP2                            = 0x00000525,
    GSDIDIAG_USIM_EMLPP                           = 0x00000526,
    GSDIDIAG_USIM_AAEM                            = 0x00000527,
    GSDIDIAG_USIM_GMSI                            = 0x00000528,
    GSDIDIAG_USIM_HIDDENKEY                       = 0x00000529,
    GSDIDIAG_USIM_BDN                             = 0x0000052A,
    GSDIDIAG_USIM_EXT4                            = 0x0000052B,
    GSDIDIAG_USIM_CMI                             = 0x0000052C,
    GSDIDIAG_USIM_EST                             = 0x0000052D,
    GSDIDIAG_USIM_ACL                             = 0x0000052E,
    GSDIDIAG_USIM_DCK                             = 0x0000052F,
    GSDIDIAG_USIM_CNL                             = 0x00000530,
    GSDIDIAG_USIM_START_HFN                       = 0x00000531,
    GSDIDIAG_USIM_THRESHOLD                       = 0x00000532,
    GSDIDIAG_USIM_OPLMNWACT                       = 0x00000533,
    GSDIDIAG_USIM_OPLMNSEL                        = 0x00000534,
    GSDIDIAG_USIM_HPLMNWACT                       = 0x00000535,
    GSDIDIAG_USIM_ARR                             = 0x00000536,
    GSDIDIAG_USIM_RPLMNACT                        = 0x00000537,
    GSDIDIAG_USIM_NETPAR                          = 0x00000538,
    GSDIDIAG_USIM_ADN                             = 0x00000539,
    GSDIDIAG_USIM_ADN1                            = 0x0000053A,
    GSDIDIAG_USIM_GAS                             = 0x0000053B,
    GSDIDIAG_USIM_GAS1                            = 0x0000053C,
    GSDIDIAG_USIM_GRP1                            = 0x0000053D,
    GSDIDIAG_USIM_SNE                             = 0x0000053E,
    GSDIDIAG_USIM_SNE1                            = 0x0000053F,
    GSDIDIAG_USIM_EMAIL                           = 0x00000540,
    GSDIDIAG_USIM_EMAIL1                          = 0x00000541,
    GSDIDIAG_USIM_IAP                             = 0x00000542,
    GSDIDIAG_USIM_IAP1                            = 0x00000543,

    GSDIDIAG_USIM_PHONEBOOK_PBR                   = 0x00000600,
    GSDIDIAG_USIM_PHONEBOOK_CCP1                  = 0x00000601,
    GSDIDIAG_USIM_PHONEBOOK_UID                   = 0x00000602,
    GSDIDIAG_USIM_PHONEBOOK_PSC                   = 0x00000603,
    GSDIDIAG_USIM_PHONEBOOK_CC                    = 0x00000604,
    GSDIDIAG_USIM_PHONEBOOK_PUID                  = 0x00000605,
    GSDIDIAG_USIM_PHONEBOOK_PBC                   = 0x00000606,

    GSDIDIAG_USIM_GSMACCESS_KC                    = 0x00000700,
    GSDIDIAG_USIM_GSMACCESS_KCGPRS                = 0x00000701,
    GSDIDIAG_USIM_GSMACCESS_CPBCCH                = 0x00000702,
    GSDIDIAG_USIM_GSMACCESS_INVSCAN               = 0x00000703,

    GSDIDIAG_USIM_MEXE_MEXE_ST                    = 0x00000800,
    GSDIDIAG_USIM_MEXE_ORPK                       = 0x00000801,
    GSDIDIAG_USIM_MEXE_ARPK                       = 0x00000802,
    GSDIDIAG_USIM_MEXE_TPRPK                      = 0x00000803
  //reserved
} GSDI_DIAG_SIM_ElementaryFile_Enum;


/** 
Enumerated type for GSDI SIM Simple Req 

    Defines Index Modes Available for Commands.
        0 = User specified values for SIM_INDEX_MODE, SIM_
            OFFSET, SIM_REC_NUM, SIM_DATA_LEN: user has to
            deal with chunking: for record based files, user has to
            specify SIM_REC_NUM
        1 = Phone uses appropriate values; also returns the
            complete data in a file; user is transparent to chunking

	Copied from embedded source code file gsdidiag.h 

*/

typedef enum
{
    GSDIDIAG_SIM_SIMPLE_REQ_ONE_STEP = 0x00,	// Mobile software performs only one step of preparing data
    GSDIDIAG_SIM_SIMPLE_REQ_ALL_STEPS = 0x01	// Mobile software merges all data into a complete file

}GSDI_DIAG_SimpleReq_Enum;

/** 
Enumerated type for GSDI indexing modes 

    Defines Index Modes Available for Commands.
    GSDIDIAG_NO_INDEX        :  Applicable to Binary/Transparent Files
    GSDIDIAG_INDEX_ABSOLUTE  :  Applicable to Linear Fixed / Cyclic Files
    GSDIDIAG_INDEX_NEXT      :  Applicable to Cyclic Files
    GSDIDIAG_INDEX_PREVIOUS  :  Applicable to Cyclic Files

	Copied from embedded source code file gsdidiag.h 

*/

typedef enum
{
    GSDIDIAG_NO_INDEX       = 0x00,
    GSDIDIAG_INDEX_ABSOLUTE = 0x01,
    GSDIDIAG_INDEX_CURRENT  = 0x02,
    GSDIDIAG_INDEX_NEXT     = 0x03,
    GSDIDIAG_INDEX_PREVIOUS = 0x04,

}GSDI_DIAG_IndexModes_Enum;


/**
    Enumerated personality types to be used with GSDI functions.

	Copied from embedded source code file gsdidiag.h 
*/
typedef enum
{
    GSDIDIAG_PERSO_NW      = 0x00,
    GSDIDIAG_PERSO_NS      = 0x01,
    GSDIDIAG_PERSO_SP      = 0x02,
    GSDIDIAG_PERSO_CP      = 0x03,
    GSDIDIAG_PERSO_SIM     = 0x04
} GSDI_DIAG_PersoFeature_Enum;


/**
    Enumerated personality types to be used with GSDI functions.

	Copied from embedded source code file gsdidiag.h 
*/
typedef enum
{
	EVENT_GSDI_GET_FILE_ATTRIBUTES	= 999,
	EVENT_GSDI_SIM_READ				= 1000,
	EVENT_GSDI_SIM_WRITE			= 1001,
	EVENT_GSDI_GET_PIN_STATUS		= 1002,
	EVENT_GSDI_VERIFY_PIN			= 1003,
	EVENT_GSDI_UNBLOCK_PIN			= 1004,
	EVENT_GSDI_DISABLE_PIN			= 1005,
	EVENT_GSDI_ENABLE_PIN			= 1006,
	EVENT_GSDI_SIM_INCREASE			= 1007,
	EVENT_GSDI_EXECUTE_APDU_REQ		= 1008,
	EVENT_GSDI_ACTIVATE_FEATURE_IND	= 1037,
	EVENT_GSDI_DEACTIVATE_FEATURE_IND	= 1038,
	EVENT_GSDI_GET_FEATURE_IND			= 1039,
	EVENT_GSDI_SET_FEATURE_DATA			= 1040,
	EVENT_GSDI_UNBLOCK_FEATURE_IND		= 1041,
	EVENT_GSDI_GET_CONTROL_KEY			= 1042,
	EVENT_GSDI_OTA_DEPERSO				= 1043,
	EVENT_GSDI_GET_PERM_FEATURE_IND		= 1044,
	EVENT_GSDI_PERM_DISBALE_FEATURE_IND	= 1045

} GSDI_DIAG_EventId;


/**
	Structure for parsing GSDI event responses
*/
typedef struct 
{
	unsigned long CMD_STATUS;		//!<' GSDI_SUCCESS = success, otherwise failure codes in chapter 5 of GSDI ICD
	unsigned short TRANSACTION_ID;	//!<' Transaction ID returned from request response.
	unsigned char data[1];			//!<' First data element of data the other data elments 
} GSDIDIAG_EventHeader;

/******************************************************************************
						Diagnostic - Streaming Download subsystem
*******************************************************************************/
/**
	 definitions of legal streaming download protocol commands
*/
typedef enum
{
	HELLO_CMD =          0x1,				//!< ' Hello command                 - sent */
	HELLO_RSP_CMD =      0x2,				//!< ' Hello command response        - recv */
	READ_CMD =           0x3,				//!< ' Read command                  - sent */
	READ_RSP_CMD =       0x4,				//!< ' Read command response         - recv */
	S_WRITE_CMD =        0x5,				//!< ' Simple write command          - sent */
	S_WRITE_RSP_CMD =    0x6,				//!< ' Simple write command response - recv */
	STRM_WRITE_CMD =     0x7,				//!< ' Stream write command          - sent */
	STRM_WRITE_RSP_CMD = 0x8,				//!< ' Stream write command response - recv */
	NOP_CMD =            0x9,				//!< ' NOP command                   - sent */
	NOP_RSP_CMD =        0xa,				//!< ' NOP command response          - recv */
	RESET_CMD =          0xb,				//!< ' Reset command                 - sent */
	RESET_RSP_CMD =      0xc,				//!< ' Reset command response        - recv */
	ERROR_CMD =          0xd,				//!< ' Error response                - recv */
	LOG_CMD =            0xe,				//!< ' Log response                  - recv */
	UNLOCK_CMD =         0xf,				//!< ' Unlock command                - sent */
	UNLOCK_RSP_CMD =    0x10,				//!< ' Unlock command response       - recv */
	POWER_OFF_CMD =     0x11,				//!< ' Power off command             - sent */
	POWER_OFF_RSP_CMD = 0x12,				//!< ' Power off command response    - recv */
	DWNLDR_OPEN_CMD =   0x13,				//!< ' Open downloader               - sent */
	DWNLDR_OPEN_RSP_CMD = 0x14,				//!< ' Open downloader response      - recv */
	DWNLDR_CLOSE_CMD =  0x15,				//!< ' Close downloader              - sent */
	DWNLDR_CLOSE_RSP_CMD = 0x16,			//!< ' Close downloader response     - recv */
	DWNLDR_SECURITY_MODE_CMD = 0x17,		//!< ' Multi-image security mode          - sent
	DWNLDR_SECURITY_MODE_RSP_CMD = 0x18,	//!< ' Multi-image security mode response - recv
	DWNLDR_PRTN_TBL_CMD = 0x19,				//!< ' Multi-image partition command      - sent
	DWNLDR_PRTN_TBL_RSP_CMD = 0x1A,			//!< ' Multi-image partition response     - recv
	DWNLDR_MULTI_IMG_OPEN_CMD = 0x1B,		//!< ' Multi-image open command           - sent
	DWNLDR_MULTI_IMG_OPEN_RSP_CMD = 0x1C,	//!< ' Multi-image open response          - recv
	DWNLDR_FLASH_ERASE_CMD = 0x1D,			//!< ' Erase flash (multi- to single- d/l) cmd
	DWNLDR_FLASH_ERASE_RSP_CMD = 0x1E		//!< ' Erase flash response

} Download_Cmd_Enum;

/******************************************************************************
						Diagnostic - GPS Subsystem
*******************************************************************************/


/*
	GPS sub system commands, from Services\LSM\diag_pdapi.h
*/
typedef enum
{
	_GPS_START_CMD				= 0x0000,
	_GPS_END_CMD				= 0x0001,
	_GPS_STATE_CMD				= 0x0002,
	_GPS_SET_NV_PARAM_CMD		= 0x0003,
	_GPS_GET_LAST_KNOWN_POS_CMD	= 0x0004,

	_GPS_GET_PDSM_PARAMS_CMD	= 0x0006,
	_GPS_PDSM_TCP_SOCKET_CMD	= 0x0007,
	_GPS_DIAG_START_TASK		= 0x0008,
	_GPS_DIAG_END_TASK			= 0x000C,
	_GPS_DIAG_TEST_API_CMD		= 0x000D,
	_GPS_NOTIFY_VERIFY_SEND_USER_ACTION		=	0x0010,
	_GPS_API_CMD				= 0x0011,
	_GPS_SET_TEST_MODE_PARAMS	= 0x0012,
	_GPS_USE_THIS_POS			= 0x0013

} GPS_Cmd_Enum;



/******************************************************************************
						Diagnostic - Call Manager subsytem
*******************************************************************************/


//! Call states, from services/cm/cm.h
typedef enum  
{

  CM_CALL_STATE_NONE	= -1,		//!< 'FOR INTERNAL USE OF CM ONLY!
  CM_CALL_STATE_IDLE	= 0,		//!< ' Call is in idle state - i.e. no call
  CM_CALL_STATE_ORIG	= 1,		//!< ' Call is in origination state 
  CM_CALL_STATE_INCOM	= 2,		//!< ' Call is in alerting state 
  CM_CALL_STATE_CONV	= 3,		//!< ' Call is in conversation state
  CM_CALL_STATE_CC_IN_PROGRESS = 4,	//!< ' Call is originating but waiting for call control to complete 
  CM_CALL_STATE_MAX					//!< ' FOR INTERNAL USE OF CM ONLY!

} cm_call_state_enum;


//! System operating modes, from services/cm/sys.h
typedef enum
{
  SYS_OPRT_MODE_NONE	= -1,		//!< ' FOR INTERNAL USE OF CM ONLY!
  SYS_OPRT_MODE_PWROFF	= 0,		//!< ' Phone is powering off
  SYS_OPRT_MODE_FTM		= 1,		//!< ' Phone is in factory test mode
  SYS_OPRT_MODE_OFFLINE	= 2,		//!< ' Phone is offline
  SYS_OPRT_MODE_OFFLINE_AMPS = 3,	//!< ' Phone is offline analog
  SYS_OPRT_MODE_OFFLINE_CDMA = 4,	//!< ' Phone is offline cdma
  SYS_OPRT_MODE_ONLINE = 5,			//!< ' Phone is online
  SYS_OPRT_MODE_LPM = 6,			//!< ' Phone is in LPM - Low Power Mode
  SYS_OPRT_MODE_RESET = 7,			//!< ' Phone is resetting - i.e. power-cycling
  SYS_OPRT_MODE_NET_TEST_GW = 8,	//!< ' Phone is conducting network test for GSM/WCDMA. 
  SYS_OPRT_MODE_OFFLINE_IF_NOT_FTM = 9,	//!< ' offline request during powerup.
  SYS_OPRT_MODE_PSEUDO_ONLINE = 10,	//!< ' Phone is pseudo online, tx disabled
  SYS_OPRT_MODE_MAX					//!< ' FOR INTERNAL USE OF CM ONLY!

} sys_oprt_mode_enum;

//! System modes, from services/cm/sys.h
typedef enum
{
  SYS_SYS_MODE_NONE		= -1,		//!< ' FOR INTERNAL USE ONLY!
  SYS_SYS_MODE_NO_SRV	= 0,		//!< ' No service = NV_MODE_INACTIVE
  SYS_SYS_MODE_AMPS		= 1,		//!< ' Analog Mobile Phone System (AMPS) mode
  SYS_SYS_MODE_CDMA		= 2,		//!< ' Code Division Multiple Access (CDMA) mode
  SYS_SYS_MODE_GSM		= 3,		//!< ' Global System for Mobile communications (GSM) mode
  SYS_SYS_MODE_HDR		= 4,		//!< ' High Data Rate (HDR) mode
  SYS_SYS_MODE_WCDMA	= 5,		//!< ' Wideband CDMA (WCDMA) mode
  SYS_SYS_MODE_GPS		= 6,		//!< ' Global Positioning System (GPS) mode
  SYS_SYS_MODE_GW		= 7,		//!< ' GSM and WCDMA mode
  SYS_SYS_MODE_MAX					//!< ' FOR INTERNAL USE ONLY!

} sys_sys_mode_enum;


/******************************************************************************
						Diagnostic - HDR subsytem
*******************************************************************************/
//! QCT HDR Subsystem command IDs (80-V1294-2) Table 2.1
typedef enum
{
	_HDR_RST_ACCESS_CNT					= 0,
	_HDR_RST_FWD_STATS					= 1,
	_HDR_RST_RTC_RATE_CNT				= 2,
	_HDR_RST_CONN_ATTEMPT_CNT			= 3,
	_HDR_RST_PAGE_MSG_CNT				= 4,
	_HDR_RST_SESSION_ATTEMPT_CNT		= 5,
	_HDR_RST_RLP_STATS					= 6,
	_HDR_AT_CONFIG						= 7,	
	_HDR_STATE							= 8,
	_HDR_DIP_SWITCH						= 9,
	_HDR_MCS_FLOW_COMMAND				= 10,
	_HDR_MCS_DEBUG_COMMAND				= 11,
	_HDR_MCS_CLR_STATS_COMMAND			= 12,
	_HDR_RST_CONNECTED					= 13
} hdr_subsys_cmd_id_enum;

/******************************************************************************
						Diagnostic - GSM subsytem
*******************************************************************************/

// Constants for GSM Diag
#define GSM_DIAG_IMEI_SIZE  9
#define GSM_DIAG_IMSI_SIZE  9
#define GSM_DIAG_LAI_SIZE   5
#define GSM_DIAG_CALL_ORG_IMSI_SIZE 32

/**
	GSM Diagnostic commands, from Services\diag\gsmdiag.h

*/
typedef enum 
{
	_GSM_DIAG_VERNO_F           = 0,
	_GSM_STATUS_F               = 1,
	_GSM_TMC_STATE_RETRIEVAL_F  = 2,
	_GSM_DIAG_CALL_ORIG_F       = 3,
	_GSM_DIAG_CALL_END_F        = 4,
	_GSM_GPRS_LOG_PACKET_REQ_F  = 5
} DiagGsmCmd_Enum;

/******************************************************************************
						Diagnostic - MediaFLO subsytem
*******************************************************************************/

/**
	MediaFLO Diagnostic commands, from mflog.h

*/
typedef enum 
{   
	_MFLO_RST_PLP_STATS					= 0,	//!< ' Reset Total/Bad PLP counters                */
	_MFLO_GET_FLO_STATE					= 1,    //!< ' Get FLO State                               */
    _MFLO_START_FLO						= 2,    //!< ' Start FLO                                   */
    _MFLO_GET_FLO_VERSION_INFO			= 6,	//!< ' Get the FLO H/W & S/W Versions              */
    _MFLO_ACTIVATE_FLOW					= 9,    //!< ' Start decoding FLOW                         */
    _MFLO_DEACTIVATE_FLOW				= 10,   //!< ' Stop decoding FLOW                          */ 
    _MFLO_GET_BOUND_FLOW_LIST			= 11,   //!< ' Get Bound FLOW List                         */
    _MFLO_GET_MLC_INFO					= 12,   //!< ' Get MLC Info                                */
    _MFLO_GET_RSSI_VALUE				= 13,   //!< ' Get RSSI Value                              */
    _MFLO_GET_MLC_PLP_STAT_DYN_PARAMS	= 14,   //!< ' Get MLC PLPs/CBs stats                      */
    _MFLO_SET_RF_CHNL					= 15,   //!< ' Set RF Channel                              */
    _MFLO_GET_RF_CHNL					= 16,   //!< ' Get RF Channel                              */
    _MFLO_RST_MLC_PLP_STATS				= 17,   //!< ' Reset MLC Total/Bad PLP counters            */
    _MFLO_GET_MLC_DYN_PARAMS			= 18,   //!< ' Get MLC Dynamic Parameters                  */
    _MFLO_GET_OIS_PLP_STAT				= 21,   //!< ' Get PLP stats for OIS channel               */
    _MFLO_RST_OIS_PLP_STAT				= 22,   //!< ' Reset PLP stats for OIS channel             */
    _MFLO_GET_FLO_SUBSTATE				= 39,   //!< ' Get FLO SUB State                           */
    _MFLO_GET_ACTIVE_MLC_LIST			= 40,   //!< ' Get MLCs Info for the all active ones       */
    _MFLO_GET_FLO_PLP_DYN_PARAMS		= 43,   //!< ' Get PLP Params based on FLOW Id             */
    _MFLO_ENABLE_FTAP_PLP_DATA			= 44,   //!< ' Enable PacketRecord logging mode            */
    _MFLO_ENABLE_FTAP_OIS				= 45,   //!< ' Enable OIS logging mode                     */
	_MFLO_ENABLE_WIC_LIC				= 46,   //!< ' Enable FTAP WIC/LIC Logging                 */ 
    _MFLO_FTAP_ACTIVATE_FLOW			= 48,   //!< ' Activate FTAP FLOW						   */
    _MFLO_GET_CONTROL_CHANNEL_DATA		= 54    //!< ' Enable Control Channel Data logging		   */
} mflo_diag_subsys_cmd_enum_type;

/**
	MediaFLO Log Codes, from mflog.h

*/
#define LOG_MFLO_RSSI_VALUE_DYNAMIC_PARAMS_C		0x11F5
#define LOG_MFLO_FTAP_PACKETRECORD_HEADER_PARAMS_C	0x120C
#define LOG_MFLO_FTAP_PACKETRECORD_PARAMS_C			0x120D
#define LOG_MFLO_FTAP_WOIS_PARAMS_C                 0x120E   
#define LOG_MFLO_FTAP_LOIS_PARAMS_C					0x120F               
#define LOG_MFLO_FTAP_WID_LID_PARAMS_C				0x1210 
#define LOG_MFLO_FDM_RECORDS_C						0x121F
  
#define MFLO_MAX_LOG_PACKET_SIZE					1542
#define MFLO_MAX_FTAP_PACKET_RECORDS				111
#define MFLO_FIRST_LAYERED_TX_MODE					6
#define MFLO_MAX_MLC_FLOW_MAPPING_RECORDS			80

//! MediaFLO Binding type 
typedef enum
{
   MFLO_OVERHEAD   = 0x00,
   RT_VIDEO_CODEC  = 0x01,
   RT_AUDIO_CODEC  = 0x02,
   NRT_MEDIA       = 0x03,
   DATACAST        = 0x04,
   TAP             = 0x05,
   FIXED_TAP       = 0x06,
   NOT_BOUND	   = 0xFF
} mftypes_binding_type;

//! MediaFLO System type 
typedef enum
{
   WOIS				= 0x00,		//! Wide System 
   WIDE				= 0x00,		//! Wide System 
   LOIS				= 0x01,		//! Local System
   LOCAL			= 0x01,		//! Local System 
   WIDE_AND_LOCAL	= 0x02
} mftypes_system_type;

//! MediaFLO State type 
typedef enum
{
   INVALID_STATE		= 0x00,
   ACQUIRING_STATE		= 0x01,
   IDLE_STATE			= 0x02,
   ACTIVE_STATE			= 0x03,
   UNAVAILABLE_STATE	= 0x04,
   VARIABLE_NOT_SET		= 0xFF	//! Used when a function to get the state fails
} mftypes_state_type;

//! MediaFLO Transmit mode type 
typedef enum
{
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
} mftypes_trans_mode_type;

//! MediaFLO Activate Status type 
typedef enum
{
   ACTIVATE_FAILED		= 0x00,
   ACTIVATING			= 0x01,
   ACTIVATE_SUCCESS		= 0x02
   
} mftypes_activate_status_type;

//! MediaFLO Flow Status type 
typedef enum
{
   FLOW_NOT_ACTIVATED  	= 0x00,
   FLOW_ACTIVATED		= 0x01,
   FLOW_DEACTIVATING	= 0x02
   
} mftypes_flow_status_type;

//! MediaFLO MLC Status type 
typedef enum
{
   MLC_NOT_ACTIVE  	= 0x00,
   MLC_ACTIVE		= 0x01
   
} mftypes_mlc_status_type;

//! MediaFLO Record Retrieval Status type 
typedef enum
{
   NO_RECORDS_AVAILABLE	= 0x00,
   RECORDS_RETRIEVED	= 0x01
   
} mftypes_record_retrieval_status_type;

//! MediaFLO Sleep Setting type 
typedef enum
{
   SLEEP_AND_SNOOZE_DISABLED		= 0x00,
   SLEEP_ENABLED_SNOOZE_DISABLED	= 0x01,
   SLEEP_DISABLED_SNOOZE_ENABLED	= 0x02,
   SLEEP_ENABLED_SNOOZE_ENABLED		= 0x03
   
} mftypes_sleep_settings_type;

//! MediaFLO Structure to store MLC PLP statistics
typedef struct 
{
	dword BasePlpsPreRsGood;		
	dword BasePlpsPreRsErasures;
	dword BasePlpsPostRsGood;
	dword BasePlpsPostRsErasures;
	dword EnhPlpsPreRsGood;
	dword EnhPlpsPreRsErasures;
	dword EnhPlpsPostRsGood; 
	dword EnhPlpsPostRsErasures;
	dword BaseCbsPreRsGood; 
	dword BaseCbsPreRsErasures;
	dword BaseCbsPostRsGood; 
	dword BaseCbsPostRsErasures;
	dword EnhCbsPreRsGood; 
	dword EnhCbsPreRsErasures;
	dword EnhCbsPostRsGood; 
	dword EnhCbsPostRsErasures;
}mftypes_mlc_plp_stats_type;

//! MediaFLO Structure to store NS PLP statistics
typedef struct 
{
	dword PlpsPreRsGood;		
	dword PlpsPreRsErasures;
	dword PlpsPostRsGood;
	dword PlpsPostRsErasures;
}mftypes_plp_stats_type;

//! MediaFLO Structure to store NS CB statistics
typedef struct 
{
	dword CbsPostRsGood; 
	dword CbsPostRsErasures;
}mftypes_cb_stats_type;

//! MediaFLO Protocol task's substate type
typedef enum 
{
	SUBSTATE_IDLE		= 0x0,
	SUBSTATE_GET_OIS	= 0x1,
	SUBSTATES_GET_CC	= 0x2
}mftypes_substate;

//! MediaFLO OIS reception mode type
typedef enum 
{
	DISABLE_WOIS_AND_LOIS	= 0x0,
	ENABLE_WOIS_ONLY		= 0x1,
	ENABLE_LOIS_ONLY		= 0x2,
	ENABLE_WOIS_AND_LOIS	= 0x3
}mftypes_ois_reception_mode_type;

//! MediaFLO Reed-Solomon code type
typedef enum 
{
	RS_16_16_0		= 0x0,
	RS_16_14_2		= 0x1,
	RS_16_12_4		= 0x2,
	RS_16_8_8		= 0x3
}mftypes_reed_solomon_code_type;

//! MediaFLO Structure to store MLC dynamic param info
typedef struct
{
	byte MlcID;
	dword BaseFFTaddress;
	word SymbolOffset;
	byte StartSlotOffset;
	byte MinSlot;
	byte MaxSlot;
	word NumSlots;
	byte NumCodeBlocks;
	mftypes_trans_mode_type TxMode;
	byte OuterCode;
	byte Stream1Bound;
	byte Stream2Bound;
	byte FFTframeCount;
}mftypes_mlc_dyn_params_type;

//! MediaFLO Structure to store info about active flows
typedef struct
{
	dword FlowID;
	byte MlcID;
	byte StreamID;
	byte StreamUsesBothLayers;
	byte ByteInterleaveEnabled;
	mftypes_binding_type BindingType;
	mftypes_system_type System;
}mftypes_active_flow_info;

//! MediaFLO Structure to store info about active MLCs
typedef struct
{
	dword Base_FFT_Address;
	mftypes_trans_mode_type TransmitMode;
	byte OuterCode;
	byte NumCbsInCurrentSF;
	mftypes_mlc_plp_stats_type MlcPlpStats;
	byte Stream1Bound;
	byte Stream2Bound;
}mftypes_active_mlc_info;

//! Structure to associate Flow ID, MLC ID, Tx Mode,  
//! Base FTAP PLP per Superframe, Enhancement FTAP PLP per Superframe
typedef struct
{
	dword FlowID;
	byte MLC_ID;
	byte TxMode;
	word PLPperSuperframeBase;
	word PLPperSuperframeEnh;
}mftypes_mlc_schedule;

//! MediaFLO Structure to associate FTAP Sequence Numbers with their corresponding FTAP Test Signature
typedef struct
{
	dword FTAP_SequenceNumber;
	byte FTAP_TestSignature;
}mftypes_seq_vs_sig;

//! MediaFLO Structure to store the TAP Message Content for the FLO Tx Waveform
typedef struct
{
	byte NumberOfSuperframes;				// Number of superframes in the FLO Tx Waveform
	byte NumberOfFlowIDs;					// Number of FlowIDs available to be decoded in the FLO Tx Waveform
	mftypes_mlc_schedule* pMLC_Schedule;	// Structure which associates Flow ID, MLC ID, Tx Mode and PLP per superframe for the FLO Tx Waveform
	mftypes_seq_vs_sig* pSeq_vs_Sig;		// Structure which associates FTAP Sequence Numbers in the FLO Tx Waveform with their corresponding FTAP Test Signature
}mftypes_tap_msg_content;

//! MediaFLO Structure to store data for one Flow ID for one superframe while parsing log packets
typedef struct
{
	dword FlowID;							// Flow ID corresponding to the statistics in this data structure
	word PLPperSuperframeBase;				// Number of Base PLPs in each superframe
	word PLPperSuperframeEnh;				// Number of Enhancement PLPs in each superframe
	mftypes_seq_vs_sig* pLoggedSeqVsSigBase;// Pointer to array of mftypes_seq_vs_sig structures to store Base FTAP Seq Num / FTAP Test Sig pairs 
	mftypes_seq_vs_sig* pLoggedSeqVsSigEnh;	// Pointer to array of mftypes_seq_vs_sig structures to store Enhancement FTAP Seq Num / FTAP Test Sig pairs 
	word BaseLength;						// Number of Base FTAP Seq Num / FTAP Test Sig pairs recorded 
	word EnhLength;							// Number of Enhancement FTAP Seq Num /FTAP Test Sig pairs recorded
	byte* pMatches;							// Pointer to byte array with length PLPperSuperframeBase * Number of Superframes
	word NumBasePLPs;						// Number of Base PLPs recorded
	word NumEnhPLPs;						// Number of Enhancement PLPs recorded
	word NumBaseErasures;					// Number of Base erasures recorded
	word NumEnhErasures;					// Number of Enhancement erasures recorded
	word NumBaseSeqSigErrors;				// Number of Base FTAP Seq Num / FTAP Test Sig errors recorded
	word NumEnhSeqSigErrors;				// Number of Enhancement FTAP Seq Num / FTAP Test Sig errors recorded
}mftypes_current_sf_stats; 

//! MediaFLO Error code type
typedef enum
{
	NOT_AVAILABLE								= 0,
	TAP_MSG_CONTENT_NOT_INITIALIZED				= 1,
	NO_LOG_PACKETS								= 2,
	MEMORY_ALLOCATION_ERROR						= 3,
	INVALID_NUMBER_OF_FLOW_IDS					= 4,
	RCVD_LOGPACKET_FOR_INVALID_FLOWID			= 5,
	PLP_STATS_ALL_ZERO							= 6,
	INVALID_REED_SOLOMON_TYPE					= 7,
	INVALID_NUMBER_OF_PLP_RECORDS				= 8,
	MISSING_LOG_PACKETS							= 9,
	PLP_STATS_NOT_UPDATING						= 10
}mftypes_error_code;

//! MediaFLO Type of PER to measure
typedef enum
{
	BASE_ONLY					= 0,
	ENHANCEMENT_ONLY			= 1,
	BASE_AND_ENHANCEMENT		= 2
}mftypes_per_to_measure;

//! MediaFLO Reed Solomon type
typedef enum
{
	PRE_REED_SOLOMON			= 0,
	POST_REED_SOLOMON			= 1,
	PRE_OR_POST_REED_SOLOMON	= 2
}mftypes_reed_solomon;

//! MediaFLO PER measurement result
typedef enum
{
	FAILED				= 0,
	PASSED				= 1,
	KEEP_TESTING		= 2
}mftypes_per_result;

//! MediaFLO control channel message type 
typedef enum
{
   MFCC_FDM				= 0x00,
   MFCC_RDM				= 0x01,
   MFCC_NDM				= 0x02,
   MFCC_FILLER			= 0x03,
   MFCC_FMS				= 0xEF
} mftypes_cc_msg_type;

//! MediaFLO Structure for holding the overall PER statistics for the current measurement
typedef struct
{
	dword iFlowID;					//! Flow ID corresponding to the statistics in this data structure
	double dPER_Base;				//! Base Layer Packet Error Rate for all superframes
	double dPER_Enh;				//! Enhancement Layer Packet Error Rate for all superframes
	dword iTotalPLPBase; 			//! Total number of Base Layer PLPs recorded
	dword iTotalPLPEnh; 			//! Total number of Enhancement Layer PLPs recorded
	dword iTotalErrorsBase; 		//! Total number of Base Layer Errors recorded (includes erasures and mismatched FTAP_SequenceNumber / FTAP_TestSignature pairs)
	dword iTotalErrorsEnh; 			//! Total number of Enhancement Layer Errors recorded (includes erasures and mismatched FTAP_SequenceNumber / FTAP_TestSignature pairs)
	dword iTotalErasuresBase;		//! Total number of Base Layer Erasures recorded
	dword iTotalErasuresEnh;		//! Total number of Enhancement Layer Erasures recorded
	dword iTotalSeqSigErrorsBase;	//! Total number of Base Layer FTAP Sequence Number / FTAP Test Signature Errors recorded
	dword iTotalSeqSigErrorsEnh;	//! Total number of Enhancement Layer FTAP Sequence Number / FTAP Test Signature Errors recorded
}mftypes_per_statistics;

//! MediaFLO Structure for holding the overall WID LID statistics for the current measurement
typedef struct
{
	dword iTotalWIDLIDMeasured; 	//! Total number WID and LIDs measured
	dword iTotalErrorsWID; 			//! Total number of WID Errors.
	dword iTotalErrorsLID; 			//! Total number of LID Errors.
	double dWIDErrorRate;           //! WID Error Rate
	double dLIDErrorRate;           //! LID Error Rate
}mftypes_widlid_statistics;

//! MediaFLO Structure used to parse the LOG_MFLO_FTAP_PACKETRECORD_HEADER_PARAMS_C log packets
typedef struct
{
	dword sysTime;
	dword flow_id;
	byte mlc_id;
	byte strm_id;
	byte system;
	byte trans_mode;
	byte rs_code;
	word rssi_int_part;
	byte rssi_fract_part;
	byte rssi_value_positive;
	dword num_PLP_records;
}mftypes_ftap_packet_record_header_params;

//! MediaFLO Structure used to parse individual PLP records. A LOG_MFLO_FTAP_PACKETRECORD_PARAMS_C
//! log packet contains a variable number of individual PLP records.
typedef struct
{
	dword flow_id;
	byte data_record_or_parity;
	byte base_or_enhancement;
	byte preRS_ErasureIndicator;
	byte postRS_ErasureIndicator;
	dword FTAP_SequenceNumber;
	byte FTAP_TestSignature;
}mftypes_ftap_plp_record;

//! MediaFLO Structure used to parse the first four entries in LOG_MFLO_FTAP_PACKETRECORD_PARAMS_C log packets
typedef struct
{
	dword sysTime;
	word log_seq_num;
	byte last_packet;
	word actual_num_plp_records;
}mftypes_ftap_logpkt_header_type;

//! MediaFLO Structure used to parse the LOG_MFLO_FTAP_PACKETRECORD_PARAMS_C log packets
typedef struct
{
	mftypes_ftap_logpkt_header_type logpkt_header;
	mftypes_ftap_plp_record plp_records[MFLO_MAX_FTAP_PACKET_RECORDS];
}mftypes_ftap_packet_record_params;

//! MediaFLO Structure used to parse the first five entries in LOG_MFLO_FDM_RECORDS_C log packets
typedef struct
{
	dword superframe_number;
	word current_seq_num;
	word total_seq_num;
	word num_records_in_this_log;
	word reserved;
}mftypes_fdm_record_header_type;

//! MediaFLO Structure used to parse individual records that map MLC ID to Flow ID in LOG_MFLO_FDM_RECORDS_C
//! log packets. A LOG_MFLO_FDM_RECORDS_C log packet contains a variable number of mapping records.
typedef struct
{
	dword service_id;
	byte flow_desc;
	byte mlc_id;
	byte system;
	byte stream_id;
	byte trans_mode;
	byte outer_code;
	word reserved1;
	dword reserved2;
}mftypes_mfcc_mlc_flow_mapping_info_type;

//! MediaFLO Structure used to parse LOG_MFLO_FDM_RECORDS_C log packets
typedef struct
{
	mftypes_fdm_record_header_type logpkt_header;
	mftypes_mfcc_mlc_flow_mapping_info_type mflog_fdm_mapping[MFLO_MAX_MLC_FLOW_MAPPING_RECORDS];
}mftypes_fdm_records_type;

//! MediaFLO Structure used to store a Control Channel Record
typedef struct
{
	word service_id;
	byte flow_desc;
	byte mlc_id;
	byte system;
	byte stream_id;
	byte trans_mode;
	byte outer_code;	
}mftypes_mf_control_channel_record;

/******************************************************************************
						Diagnostic - Handset subsytem
*******************************************************************************/

/**
	Handset Diagnostic commands, from Services\hs\gsmdiag.h

*/
typedef enum 
{
	_HS_DISPLAY_GET_PARAMS_CMD		= 0,	//!< ' Get display information (height, width, bpp)	
	_HS_DISPLAY_CAPTURE_DELAYED_CMD	= 2		//!< ' Capture screen contents
} DiagHsCmd_Enum;




/******************************************************************************
						FTM - Command ID's
*******************************************************************************/

/*===========================================================================*/
/**
	\brief Enumeration to choose between the command formats.

	Currently, the two formats are using CMD_CODE = 59 and CMD_CODE = 75
	59 is used for MSM5xxx targets and 75 was introduced for MSM6xxx targets
*/
/*===========================================================================*/
typedef enum
{
	FTM_COMMAND_75 = 75,	//!<' Command code 75 is used for MSM6100 and later
	FTM_COMMAND_59 = 59,	//!<' Command code 59 is used for 5500 and 60xx
	FTM_COMMAND_TYPE_INVALID = -1	//!<' Invalid indicator
}  FTM_Command_Type;

/**
	FTM Commands enumeration.

	These are renamed with an _ prefix in order to avoid conflict with 
	the function names.

*/
typedef enum 
{
	_FTM_SET_PDM				= 0,	//!< Sets a PDM to a value
	_FTM_SET_TX_ON				= 2,	//!< Sets the transmit circuitry ON
	_FTM_SET_TX_OFF				= 3,	//!< Sets the transmit circuitry OFF
	_FTM_SET_MODE				= 7,	//!< Sets the phones operating mode
	_FTM_SET_CHAN				= 8,	//!< Sets the phones current channel
	_FTM_GET_SYNTH_STATE		= 16,	//!< Gets the current synthesizer lock state
	_FTM_FM_TX_ST				= 37,	//!< Sends signaling tone
	_FTM_CDMA_CW_WAVEFORM		= 50,	//!< Turns on/off I/Q spreading of CDMA reverse	link
	_FTM_GET_RX_AGC				= 53,	//!< Gets AGC value of Rx AGC loop
	_FTM_SET_PA_RANGE			= 54,	//!< Sets PA range
	_FTM_SET_LNA_RANGE			= 58,	//!< Sets LNA range
	_FTM_GET_ADC_VAL			= 59,	//!< Gets ADC value
	_FTM_SET_LNA_OFFSET			= 81,	//!< Sets LNA offset
	_FTM_GET_CDMA_IM2			= 114,	//!< Gets IM2 IQ values
	_FTM_SET_DVGA_OFFSET		= 111,	//!< Sets DVGA offset
	_FTM_SET_FREQUENCY_SENSE_GAIN = 115,//!< Sets Frequency Sense Gain
	_FTM_TX_SWEEP_CAL			= 116,	//!< FTM Tx Sweep Cal
	_FTM_GET_DVGA_OFFSET		= 117,	//!< Gets DVGA Offset
	_FTM_GET_LNA_OFFSET			= 118,	//!< Gets LNA Offset
	_FTM_GET_HDET_FROM_TX_SWEEP_CAL = 119, //!< FTM Get HDET from Tx Cal
	_FTM_GET_FM_IQ				= 120,	//!< Gets FM IQ
	_FTM_SET_SECONDARY_CHAIN	= 121,	//!< Set secondary Rx mode
	_FTM_GET_CTON				= 122,	//!< Get Rx Carrier / Noise ratio
	_FTM_SECOND_CHAIN_TEST_CALL	= 123,	//!< Set the call test mode for the second Rx
	_FTM_SET_HDET_TRACKING		= 124,	//!< Track Tx power to a specified HDET value
	_FTM_CONFIGURE_TX_SWEEP_CAL = 125,	//!< Configure the Tx Sweep command parameters
	_FTM_GET_CDMA_IM2_WITH_SUPPRESSION = 126, //!<Get CDMA IM2 with suppression
	_FTM_GET_ALL_HDET_FROM_TX_SWEEP_CAL=127,	//!< Return all of the HDET values measured in the last TX SWEEP
	_FTM_SET_RF_POWER_MODE		= 128,	//!<' Set RF mode to High,Mid or Low power mode
	_FTM_SET_TX_SWEEP_STEP_SIZE = 129,	//!<' Configure CDMA/WCDMA Tx AGC PDM Step size for Tx sweep
	_FTM_TX_RX_FREQ_CAL_SWEEP	= 130,	//!<' CDMA Tx Rx Frequency measurement
	_FTM_LOAD_RF_NV				= 131,  //!<' load RF NV from NV 
	_FTM_DO_INTERNAL_DEVICE_CAL	= 132,	//!<' Perform internal device cal
	_FTM_DO_DC_CAL				= 133,	//!<' Initialize the DC accumulators.
	_FTM_SET_PA_PROFILE			= 212,
	_FTM_CONF_MODULATE_DATA		= 215,
	_FTM_SET_TRANSMIT_CONT		= 216, 
	_FTM_SET_TRANSMIT_BURST		= 217, 
	_FTM_SET_RX_BURST			= 225, 
	_FTM_SET_TX_POWER_LEVEL		= 227, 
	_FTM_GET_RSSI				= 230,
	_FTM_SET_PA_START_DELTA		= 231,
	_FTM_SET_PA_STOP_DELTA		= 232,
	_FTM_SET_PA_DAC_INPUT		= 233,
	_FTM_SET_RX_CONTINUOUS		= 234,	//!< Set Rx continuous
	_FTM_SET_TX_CAL_SWEEP		= 235,	//!< Tx cal sweep

	// GSM Polar Calibration
	_FTM_DO_GSM_AUTOCAL			= 236,	//!< Do autocal
	_FTM_SET_PATH_DELAY			= 237,	//!< Set the path delay for AMAM/AMPM
	_FTM_SET_AMAM_TABLE_GAIN	= 238,	//!< Set the gain for the AMAM table
	_FTM_SET_AMAM_TABLE_DC		= 239,	//!< Set the DC offset for the AMAM table
	_FTM_SET_TX_FRAME_MATRIX	= 240,	//!< Set the TX frame matrix
	_FTM_ENABLE_POLAR_REF_CAL	= 241,	//!< Enable polar path reference calibration
	_FTM_SET_TWOTONE_FREQ_OFFSET= 242,	//!< Sets the frequency offset for the two tone transmit
	_FTM_DO_CALPATH_RSB			= 243,	//!< Measure calpath RSB
	_FTM_DO_RTR6250_RSB			= 244,	//!< Load VI firmware for RSB
	_FTM_SET_OPLL_BW			= 245,	//!< set Chargepump current registers
	_FTM_RX_GAIN_RANGE_CAL		= 246,	//!< GSM Rx Gain Range Calibration (multiple gain states in one command)

	_FTM_BASEBAND_BW_CAL		= 250,	//!< Baseband Bandwidth calibration

	// EGPRS non-signaling Command ID's
	_FTM_EGPRS_BER_START_GPRS_IDLE_MODE_REQ = 256,	//!< EGPRS - Idle mode request
	_FTM_EGPRS_BER_ESTABLISH_SRB_LOOPBACK	= 257,	//!< EGPRS - establish SRB loopback
	_FTM_EGPRS_BER_CONFIGURE_DL_TBF			= 258,	//!< EGPRS - configure DL temporary block flow
	_FTM_EGPRS_BER_CONFIGURE_UL_TBF			= 259,	//!< EGPRS - configure UL temporary block flow
	_FTM_EGPRS_BER_RELEASE_ALL_TBF			= 260,	//!< EGPRS - release all TBF
	_FTM_EGPRS_BER_RELEASE_UL_TBF			= 261,	//!< EGPRS - release all UL TBF

	// WCDMA HSDPA command ID's
    _FTM_HSDPA_BLER_CMD_START_HS			= 256,	//!< HSDPA - 
    _FTM_HSDPA_BLER_CMD_STOP_HS				= 257,	//!< HSDPA - 
    _FTM_HSDPA_BLER_CMD_RECONFIG_HS			= 258,	//!< HSDPA - 
    _FTM_HSDPA_BLER_CMD_CONFIG_HS_PDSCH		= 259,	//!< HSDPA - 
    _FTM_HSDPA_BLER_CMD_CONFIG_HS_SCCH		= 260,	//!< HSDPA - 
    _FTM_HSDPA_BLER_CMD_CONFIG_HS_DSCH		= 261,	//!< HSDPA - 
    _FTM_HSDPA_BLER_CMD_CONFIG_HS_DPCCH		= 262,	//!< HSDPA - 



	/* FTM GSM Extended Command IDs */
	_FTM_GSM_POLAR_TX_SWEEP_CAL = 300,		//!< FTM GSM TX Sweep calibration
	_FTM_TX_RSB_DC_SWEEP = 301,				//!< GSM Tx RSB DC Sweep calibration
	_FTM_GSM_TX_DETECTOR_CAL = 302,			//!< GSM Tx Detector Cal
	_FTM_GSM_TX_4X_FMOD_SWEEP = 303,		//!< GSM Sweep Tx 4xFMod calibration

	_FTM_SET_WCDMA_SECONDARY_CHAIN = 121	//!< Enable/disable secondary Rx


} FTM_Command_ID_Type;

/******************************************************************************
						FTM - System Modes
*******************************************************************************/

/**
	\brief Enumeration of FTM Mode ID's, to be used int the "mode_id" field of the
	       FTM header
*/
typedef enum 
{
	FTM_MODE_ID_CDMA_1X		= 0,		//!<' RF CDMA 1X mode - RX0
	FTM_MODE_ID_WCDMA		= 1,		//!<' RF WCDMA mode
	FTM_MODE_ID_GSM			= 2,		//!<' RF GSM Mode
	FTM_MODE_ID_CDMA_1X_RX1	= 3,		//!<' RF CDMA 1X mode - RX1
	FTM_MODE_ID_BLUETOOTH	= 4,		//!<' Bluetooth
	FTM_MODE_ID_CDMA_1X_CALL= 7,		//!<' CALL CDMA 1X mode 
	FTM_MODE_ID_HDR_C		= 8,		//!<' HDC non signaling
	FTM_MODE_ID_LOGGING		= 9,		//!<' FTM Logging
	FTM_MODE_ID_AGPS		= 10,		//!<' Async GPS
	FTM_MODE_ID_PMIC        = 11,		//!<' PMIC FTM Command
	FTM_MODE_GSM_BER		= 13,		//!<' GSM BER
	FTM_MODE_ID_AUDIO		= 14,		//!<' Audio FTM
	FTM_MODE_ID_CAMERA		= 15,		//!<' Camera
	FTM_MODE_WCDMA_BER		= 16,		//!<' WCDMA BER
	FTM_MODE_ID_GSM_EXTENDED_C = 17,	//!<' GSM Extended commands
	FTM_MODE_CDMA_API_V2	= 18,		//!<' CDMA RF Cal API v2
	FTM_MODE_ID_MF_C		= 19,		//!<' MediaFLO
	FTM_MODE_RF_COMMON		= 20,		//!<' RF Common
	FTM_MODE_WCDMA_RX1		= 21,		//!<' WCDMA diversity RX (RX1)
	FTM_MODE_WLAN			= 22,		//!<' WLAN FTM
	FTM_MODE_QFUSE			= 24,		//!<' QFUSE FTM
	FTM_MODE_ID_MF_NS		= 26,		//!<' MediaFLO NS FTM
	FTM_MODE_ID_PRODUCTION	= 0x8000,	//!<' Production FTM
	FTM_MODE_ID_LTM			= 0x8001	//!<' LTM
} FTM_Mode_Id_Enum;


/******************************************************************************
						FTM - RF Modes
*******************************************************************************/

/**
	\brief Enumeration of RF Mode ID's, from services\ftm\ftm.h
*/
typedef enum 
{

	PHONE_MODE_FM=1,				//!<' FM
	PHONE_MODE_SLEEP=2,				//!<' Sleep Mode
	PHONE_MODE_GPS=3,				//!<' GPS
	PHONE_MODE_GPS_SINAD=4,			//!<' GPS SINAD
	PHONE_MODE_CDMA_800=5,			//!<' CDMA 800
	PHONE_MODE_CDMA_1900=6,			//!<' CDMA 1900
	PHONE_MODE_CDMA_1800=8,			//!<' CDMA 1800
	PHONE_MODE_J_CDMA=14,			//!<' JCDMA
	PHONE_MODE_CDMA_450=17,			//!<' CDMA 450
	PHONE_MODE_CDMA_IMT=19,			//!<' CDMA IMT
	PHONE_MODE_CDMA_1900_EXT=26,	//!<' Secndary CDMA 1900MHz Band, Band Class 14
	PHONE_MODE_CDMA_450_EXT=27,		//!<' CDMA BC 11 (450 Extension)
	PHONE_MODE_CDMA_800_SEC=99,		//!<' Secondary CDMA 800MHz Band, Band Class 10
	
	
	PHONE_MODE_WCDMA_IMT=9,			//!<' WCDMA IMT, Band I
	PHONE_MODE_GSM_900=10,			//!<' GSM 900
	PHONE_MODE_GSM_1800=11,			//!<' GSM 1800
	PHONE_MODE_GSM_1900=12,			//!<' GSM 1900
	PHONE_MODE_BLUETOOTH=13,		//!<' Bluetooth
	PHONE_MODE_WCDMA_1900A=15,		//!<' WCDMA 1900 A, Band II Add
	PHONE_MODE_WCDMA_1900B=16,		//!<' WCDMA 1900 B, Band II Gen
	PHONE_MODE_GSM_850=18,			//!<' GSM 850
	PHONE_MODE_WCDMA_800=22,		//!<' WCDMA 800, Band V Gen
	PHONE_MODE_WCDMA_800A=23,		//!<' WCDMA 800, Band V Add
	PHONE_MODE_WCDMA_1800=25,		//!<' WCDMA 1800, Band III
	PHONE_MODE_WCDMA_BC4=28,		//!<' WCDMA BC4-used for both Band IV Gen and Band IV Add
	PHONE_MODE_WCDMA_BC8=29,		//!<' WCDMA BC8, Band VIII
	
	PHONE_MODE_MF_700=30,			//!<' MediaFLO
	PHONE_MODE_WCDMA_BC9=31,		//!<' WCDMA BC9 (1750MHz & 1845MHz), Band IX
	PHONE_MODE_CDMA_BC15=32,		//!<' CDMA Band Class 15 
	/*
		QMSL Developers: please modify:
			- QLib.h  ->  QLIB_FTM_SET_MODE() 
			- Diag_FTM.cpp  ->  Diag_FTM::FTM_SET_MODE()
			- QLIBFTMPhone.cpp when this list is changed.
	*/
	PHONE_MODE_MAX=255				//!<' Maximum possible mode ID

} FTM_RF_Mode_Enum;

/*
	The following are obsolete RF modes, or unused for Factory:

	PHONE_MODE_FM=1,				//!<' AMPS
	PHONE_MODE_CDMA=0,				//!<' Obsolete 
	PHONE_MODE_GPS_SINAD=4,			//!<' GPS SINAD (obsolete)
	PHONE_MODE_CDMA_800=5,			//!<' cdma2000 BC0
	PHONE_MODE_CDMA_1900=6,			//!<' cdma2000 BC1
	PHONE_MODE_HDR=7,				//!<' HDR Call Test
	PHONE_MODE_CDMA_1800=8,			//!<' cdma2000 1800MHz
	PHONE_MODE_J_CDMA=14,			//!<' cdma2000 JCDMA
	PHONE_MODE_CDMA_450=17,			//!<' cdma2000 BC4, 450MHz
	PHONE_MODE_CDMA_IMT=19,			//!<' cdma2000 BC6, IMT band
*/


/******************************************************************************
						FTM - GSM RF
*******************************************************************************/

/**
	\brief Enumeration of GSM Tx data sources for QLIB_FTM_SET_TRANSMIT_CONT
*/
typedef enum 
{
	FTM_GSM_TX_DATA_SOURCE_PSDRND,		//!<' Pseudorandom
	FTM_GSM_TX_DATA_SOURCE_TONE,		//!<' Single tone 
	FTM_GSM_TX_DATA_SOURCE_BUFFER,		//!<' Buffer
	FTM_GSM_TX_DATA_SOURCE_TWOTONE		//!<' 2 tone
} FTM_GSM_TX_DataSources_Enum;

/**
	Maximum number of rows to be returned from the GSM Tx Polar Internal calibration log
*/
#define FTM_GSM_TX_POLAR_INTERNAL_AUTOCAL_MAX_DATA_ROWS 500


/**
	\brief Enumeration of LNA states to be used for the iBitMask parameter of QLIB_FTM_RX_GAIN_RANGE_CAL()
*/
typedef enum 
{
	FTM_GSM_RX_GAIN_RANGE_0	= 0x01,	//!<' Gain range 0
	FTM_GSM_RX_GAIN_RANGE_1	= 0x02,	//!<' Gain range 1
	FTM_GSM_RX_GAIN_RANGE_2	= 0x03,	//!<' Gain range 2
	FTM_GSM_RX_GAIN_RANGE_3	= 0x04,	//!<' Gain range 3
	FTM_GSM_RX_GAIN_RANGE_4	= 0x05	//!<' Gain range 4

} FTM_GSM_RX_GainRange_Enum;

/**
	Number of return values for the aiRSSI_Result array used in QLIB_FTM_RX_GAIN_RANGE_CAL()
*/
#define FTM_GSM_RX_GAIN_RANGE_CAL_NUM_RETURN_VALUES 8



/******************************************************************************
						FTM - cdma2000 non-signaling log parser definitions
*******************************************************************************/

/**
	Sub packet of RDA2 -- RDA_CHANNELS_INFO_TYPE / INDIVIDUAL_CHANNELS
*/
typedef union
{
	struct
	{
		byte RPICH	: 1;
		byte FCH	: 1;
		byte DCCH	: 1;
		byte SCH0	: 1;
		byte SCCH	: 3;
		byte reserved	: 1;
	} individual_channels;
	byte mask;
} RDA_Channels_Info_Type;

/**
	Sub packet of RDA2 -- CHAN_RC_TYPE
*/
typedef struct
{
	word FCH_rc;
	word DCCH_rc;
	word SCH_rc;
	word reserved0;
	word reserved1;
}Chan_RC;

/**
	Sub packet of RDA2 -- FCH_RDA_SUB_RECORD_TYPE
*/
typedef struct
{
	dword reserved0;
	word  ser;
	byte  decision;		//!< Data rate, according to RDA_CDMA_TRAFFIC_DATA_RATE_TYPE
	byte  reserved1;
}FCH_RDA_Sub_Record_Type;

/**
	Sub packet of RDA2 -- DCCH_RDA_SUB_RECORD_TYPE
*/
typedef struct
{
	dword reserved0;
	dword reserved1;
	word  reserved2;
	word  reserved3;
	word  ser;
	byte decision;		//!< Data rate, according to RDA_CDMA_TRAFFIC_DATA_RATE_TYPE
	byte reserved4[3];
}DCCH_RDA_Sub_Record_Type;

/**
	Enumeration of "decision" types FCH_RDA_SUB_RECORD_TYPE
	available for CDMA system
*/
typedef enum
{
	RDA_DataRate_Erasure,
	RDA_DataRate_OneEighth,
	RDA_DataRate_OneQuarter,
	RDA_DataRate_OneHalf,
	RDA_DataRate_Full,
	RDA_DataRate_Max
} RDA_CDMA_TRAFFIC_DATA_RATE_TYPE;

/**
	Sub-FIELD OF RDA2 -- SCH_RDA_SUB_RECORD_2_TYPE
*/
typedef union
{
	struct
	{
		word	ltu_crc;			//!< ' LTU crc value
		word	ser;				//!< ' SER for the frame
		byte	status;				//!< ' frame CRC 
		byte	reserved[3];
	} dev_sch_conv;
	struct
	{
		word	min_llr;			//!< ' Log Likelihood Ratio 
		byte	num_iterations;		//!< ' Indicates the number of iterations run by the TD
		byte	crc_pass;			//!< ' 1=Good frame
		byte	reserved[4];
	} dev_sch_turbo;
} sch_code_type;
/* --This definition was removed December 14, 2005
typedef union
{
	struct
	{
		word	ltu_crc;
		byte	status;
		byte	ser;
	} dev_sch_conv;
	struct
	{
		word	min_llr;
		byte	num_iterations;
		byte	crc_pass;
	} dev_sch_turbo;
} sch_code_type;
*/

/**
	Sub packet of RDA2 -- SCH_RDA_SUB_RECORD_2_TYPE
*/
typedef	struct
{
	dword	reserved0;
	sch_code_type sch_code;
	word    reserved2;
	word    reserved3;
	word    rate;			
	byte    decision;		//!< Data rate, according to RDA_CDMA_TRAFFIC_DATA_RATE_TYPE
	byte    codeType;		//!< (0) FTM_SCH_CODING_CONVOLUTIONAL,	(1) FTM_SCH_CODING_TURBO 
} SCH_RDA_Sub_Record_Type;


/**
	Sub-FIELD OF RDA2 -- SCH_RDA_SUB_RECORD_2_TYPE
*/
typedef union
{
	struct
	{
		word	ltu_crc;
		word	ser;
		byte	status;
		byte	reserved[3];
	} dev_sch_conv;
	struct
	{
		word	min_llr;
		byte	num_iterations;
		byte	crc_pass;
		byte	reserved[4];
	} dev_sch_turbo;
} sch_code_2_type;

/**
	Sub packet of RDA2 -- SCH_RDA_SUB_RECORD_2_TYPE

	This structure is the only difference between the
	RDA and RDA2 log packet types
*/
typedef struct
{
	dword	 reserved0;
	sch_code_2_type sch_code;
	word	 reserved2;
	word	 reserved3;
	word	 rate;		//!< Max Data rate of SCH 
	byte	 decision;	//!< Data rate, according to RDA_CDMA_TRAFFIC_DATA_RATE_TYPE	
	byte	 codeType;	//!< (0) FTM_SCH_CODING_CONVOLUTIONAL,	(1) FTM_SCH_CODING_TURBO 
}SCH_RDA_Sub_Record_2_Type;


/**
	Union for the RC field of RDA_SUB_RECORD_HEADER_TYPE
*/
typedef union
{
	struct
	{
		word 	fch_rc	: 3;
		word 	dcch_rc	: 3;
		word 	sch_rc :	3;
		word 	frame_offset : 7;
	} chan_rc;
	word mask;

} Chan_RC_Type;


/**
	Sub packet of RDA2 -- RDA_SUB_RECORD_HEADER_TYPE
*/
typedef struct
{
	Chan_RC_Type			rc;
	RDA_Channels_Info_Type	assigned_channels;

	byte     num_sub_records;
	byte	 reference_buf[1];
}RDA_Sub_Record_Header_Type;



/**
	Searcher log header (logs 0x119A - 0x11A3)
*/
typedef struct
{
	byte iVersion;
	byte iNumSubPackets;
	word iReserved;
	byte iSubPacketsData[1];

} SRCH_TNG_GeneralizedSearcherHeader;

/**
	Searcher sub packet data header (logs 0x119A - 0x11A3)
*/
typedef struct
{
	byte iSubPacketID;
	byte iSubPacketVersion;
	word iSubPacketSize;
	byte iSubPacketData[1];

} SRCH_TNG_GeneralizedSearcherSubPacketHeader;

/**
	Searcher status sub packet data (sub packet type #1)
*/
typedef struct
{
	byte iSearcherState;
	byte iReserved;
	word iForwardRC;

} SRCH_TNG_SearcherStatus;

/**
	RF sub packet data (sub packet type #2)
*/
typedef struct
{
	byte iRX0BandClass;
	byte iReserved;	//!< empty value 
	word iRX0CDMAChannel;
	byte iRX1BandClass;
	word iRX1CDMAChannel;
	byte iRX0AGC;
	byte iRX1AGC;
	word iTxPower;
	byte iTxGainAdj;
	byte iTxPowerLimit;

} SRCH_TNG_RFStatus;

/**
	Idle Demod Stats sub packet data (sub packet type #18)
*/
typedef struct
{
	byte iChannel;    // 0-PCH, 1-BCCH, 2-FCCCH
	byte iReserved;		
	word iFrameSEC;
	dword iFrameEnergy;

} SRCH_TNG_IdleDemodStats;


//! Definition of searcher TNG General Status
#define FTM_CDMA2000_SRC_TNG_STATUS		0x119C

//! Rate Determination Algorithm Frame Information (LOG_CODE 0x10D9)
#define FTM_CDMA2000_RDA2_FRAME_INFO		0x10D9

//! Rate Determination Algorithm Frame Information (LOG_CODE 0x10D9)
#define FTM_CDMA2000_RDA_FRAME_INFO		0x10C9

//! Definition of searcher TNG Demodulation Info (LOG_CODE 0x119D)
#define FTM_CDMA2000_SRC_TNG_DEMOD_INFO	0x119D 

//! C2K paging channel symbol rate
#define CDMA2000_PAGING_CHANNEL_SYMBOL_RATE 19200


//! Stucture of SER_Sub_Data record used in Paging MER Message
typedef struct
{
	byte chain;
    word ser;
    byte bad_message; // This message would have been counted in the MER statistics as a failure
}SER_Sub_Data;

//! Max number of accumulated SER_Sub_Data Record
#define MAX_NUM_ACCUM_SER 20

//! Structure of Paging MER Message
typedef struct
{
     word   count; /* number of SER record in the ser array, could use uint8 here, but for alignment, use uint16*/
     byte	pch_rate; 
     SER_Sub_Data records[MAX_NUM_ACCUM_SER];
}RXC_SER_Log;

//! Stucture of Paging MER Statistics
typedef struct
{
	long totalPagingSER;
	long totalNumSymbol;
}CDMA2000_Paging_MER_Stat;


/**
	Structure of FTM2 1X Primary AGC, FTM Log code = LOG_FTM2_LOG_1X_AGC = 0x4
*/
typedef struct 
{
	word ftm_log_id;
	short rx_agc;
	short tx_gain_ctl;
	short tx_gain_adj;
	short tx_open_loop;
	word tx_agc_adj;
	byte pa_ratchet;
	byte lna_state;
	byte pa_state;
	byte hdet_raw;
	word therm_raw;
	byte therm_scaled;
	byte temp_index;
	byte temp_remainder;
	byte intelliceiver_state;
} FTM_LOG_1X_AGC_Struct;

/**
	Structure of FTM 1 1X Secondary AGC, FTM Log Code = LOG_FTM2_LOG_1X_AGC_C1	= 0x6
*/
typedef struct 
{
	word ftm_log_id;
	short rx_agc;
	short tx_gain_ctl;
	short tx_gain_adj;
	short tx_open_loop;
	word tx_agc_adj;
	byte pa_ratchet;
	byte lna_state;
	byte pa_state;
	byte hdet_raw;
	word therm_raw;
	byte therm_scaled;
	byte temp_index;
	byte temp_remainder;
	short rx_agc_c1;
	byte lna_state_c1;
	byte intelliceiver_state;
} FTM_LOG_1X_AGC_C1_Struct;


/**
	Number of symbols per frame for FCH
*/
typedef enum
{
	FTM_CDMA2000_NS_NUM_SYMBOLS_RC1_FRAME = 384,
	FTM_CDMA2000_NS_NUM_SYMBOLS_RC2_FRAME = 384,
	FTM_CDMA2000_NS_NUM_SYMBOLS_RC3_FRAME = 768,
	FTM_CDMA2000_NS_NUM_SYMBOLS_RC4_FRAME = 384,
	FTM_CDMA2000_NS_NUM_SYMBOLS_RC5_FRAME = 768

}FTM_CDMA2000_NS_SymbolsPerFrame_Enum;

/**
	Number of symbols per frame for F-DCCH
*/
typedef enum
{
	FTM_CDMA2000_NS_DCCH_NUM_SYMBOLS_RC1_FRAME = 0,
	FTM_CDMA2000_NS_DCCH_NUM_SYMBOLS_RC2_FRAME = 0,
	FTM_CDMA2000_NS_DCCH_NUM_SYMBOLS_RC3_FRAME = 768,
	FTM_CDMA2000_NS_DCCH_NUM_SYMBOLS_RC4_FRAME = 384,
	FTM_CDMA2000_NS_DCCH_NUM_SYMBOLS_RC5_FRAME = 768

}FTM_CDMA2000_NS_DCCH_SymbolsPerFrame_Enum;

/**
	Number of symbols per frame for F-SCH 
*/
typedef enum
{
	FTM_CDMA2000_NS_SCH_NUM_SYMBOLS_RC1_FRAME = 0,
	FTM_CDMA2000_NS_SCH_NUM_SYMBOLS_RC2_FRAME = 0,
	FTM_CDMA2000_NS_SCH_NUM_SYMBOLS_RC3_FRAME = 768,
	FTM_CDMA2000_NS_SCH_NUM_SYMBOLS_RC4_FRAME = 384,
	FTM_CDMA2000_NS_SCH_NUM_SYMBOLS_RC5_FRAME = 768

}FTM_CDMA2000_NS_SCH_SymbolsPerFrame_Enum;

/******************************************************************************
						FTM - cdma2000 non-signaling
*******************************************************************************/


/**
	\brief Enumeration of CDMA2000 non-signaling FTM Sub-command ID's
*/
typedef enum 
{
	_FTM_CDMA2000_PILOT_ACQ		= 100,	//!<'	Acquire Pilot
	_FTM_CDMA2000_DEMOD_SYNC	= 101,	//!<'	Acquire Sync channel
	_FTM_CDMA2000_DEMOD_FCH		= 102,	//!<'	Assign forward fundamental channel and setup parameters
	_FTM_CDMA2000_DEMOD_FSCH	= 103,	//!<'	Assign forward supplemental channel
	_FTM_CDMA2000_MOD_FCH		= 104,	//!<'   Assign reverse fundamental channel
	_FTM_CDMA2000_MOD_SCH		= 105,	//!<'   Assign reverse supplemental channel
	_FTM_CDMA2000_FCH_LOOPBACK	= 106,	//!<'   Enable loopback on the fundamental channel
	_FTM_CDMA2000_SCH_LOOPBACK	= 107,  //!<'   Enable loopback on the supplemental channel
	_FTM_CDMA2000_CMD_RELEASE	= 108,	//!<'   Deassign all traffic channels
	_FTM_CDMA2000_SET_MODE		= 109,	//!<'   Set Mode
	_FTM_CDMA2000_DEMOD_DCCH	= 110,	//!<' Assign fotward dedicated control channel 
	_FTM_CDMA2000_MOD_DCCH		= 111,  //!<' Assign reverse dedicated control channel 
	_FTM_CDMA2000_DCCH_LOOPBACK = 112,	//!<' Enable loopback on the dedicated control channel
	_FTM_EVDO_PILOT_ACQ			= 113,	//!<'Acquire HDR forware pilot channel
	_FTM_EVDO_DEMOD_SYNC		= 114,	//!<'Acquire HDR system time
	_FTM_EVDO_DEMOD_CC_MAC_FTC	= 115,	//!<'Demodulate HDR CC/MAC/FTC
	_FTM_EVDO_MOD_ACC			= 116,	//!<'Modulate reverse access channel
	_FTM_EVDO_MOD_TRA			= 117,	//!<'Modulate reverse traffic channel
	_FTM_EVDO_CMD_RELEASE		= 118,	//!<'Deassign HDR channels
	_FTM_CDMA2000_RF_CMDS		= 119,	//!<' RF Subcommands (RF Calibration)
	_FTM_CDMA2000_DEMOD_FCH_DCCH = 120, 
	//!<'Assign forward fundamental channel and dedicated control channel simultaneously (Release A)
	_FTM_CDMA2000_MOD_FCH_DCCH = 121,
	//!<'Assign reverse fundamental channel and dedicated control channel simultaneously (Release A)
	_FTM_EVDO_DEMOD_FWD_WITH_NO_REV = 122, //!<'Demodulate Forward Link with No Reverse Link Rev 0
	_FTM_EVDO_SET_IDLE				= 123, //'<'Put the searcher in Idle mode
	_FTM_EVDO_REV_A_CONF_MAC_FOR_FWD_CC_MAC_FTC = 124, //'<'configure the Rev A packet mode
	_FTM_EVDO_REV_A_MOD_ACC			= 125, //'<'Modulate the reverse access channel in IS-896 Rev A mode
	_FTM_EVDO_REV_A_MOD_TRA			= 126, //'<'Modulate the reverse traffic channel in IS-896 Rev A
	_FTM_EVDO_REV_A_DEMOD_FWD_WITH_NO_REV  = 127, //'<'Demodulate Forward Link with No Reverse Link Rev A
	_FTM_FWD_HHO_SC				= 128,	//'<'Handover for cdma2000

} FTM_CDMA2000_Cmd_Id_Enum;



/**
	\brief Enumeration of Calibration API V2 (80-V2376-1) FTM Sub-command ID's
*/
typedef enum
{
	_FTM_CDMA_CAL_V2_COMMIT_NV			= 0,			//!<'Commit RF calibration to NV
	_FTM_CDMA_CAL_V2_CAL_DVGA			= 1,			//!<'Calibrate DVGA
	_FTM_CDMA_CAL_V2_CAL_LNA			= 2,			//!<'Calibrate LNA
	_FTM_CDMA_CAL_V2_CAL_IM2			= 3,			//!<'Calibrate IM2
	_FTM_CDMA_CAL_V2_INTELLICEIVER		= 4				//!<'Calibrate Intelliceiver

}	FTM_CDMA_CAL_V2_Cmd_Id_Enum;

/**
	cdma2000 Band Class identifiers for QLIB_FTM_CDMA2000_PILOT_ACQ()

	The enumeration values are the same as the band class number.  If an 
	enumeration is not available for a new band class number then the 
	an integer with the band class number can be used.

*/
typedef enum 
{
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


} FTM_CDMA2000_BandClasstype;

/**
	1x dynamic range types

	This is a list of dynamic range configurations used for 1x non-signaling,
  to convert results from AGC units to dB and dBm
*/
typedef enum
{
	FTM_CDMA2000_NS_DR_85_3,		//!<' 85.3dB dynamic range, min_rssi = -106.0dBm
    FTM_CDMA2000_NS_DR_102_4		//!<' 102.4dB dynamic range, min_rssi = -115.0dBm
} FTM_CDMA2000_NS_DR_Types_Enum;


/**
	1x acquisition modes
*/
typedef enum
{
	FTM_CDMA2000_NS_FTM_ACQ_MODE_FULL	= 0,	//!<' Full 
	FTM_CDMA2000_NS_FTM_ACQ_MODE_MINI	= 1,	//!<' Mini, not supported, not to be used
	FTM_CDMA2000_NS_FTM_ACQ_MODE_MICRO	= 2		//!<' Micro, not supported, not to be used
} FTM_CDMA2000_NS_PilotAcqMode_Enum;

/**
	1x REV FCH rates
*/
typedef enum
{
	FTM_CDMA2000_NS_REV_FCH_FULL_RATE	= 0,	//!<' Full rate
	FTM_CDMA2000_NS_REV_FCH_HALF_RATE	= 1,	//!<' half rate
	FTM_CDMA2000_NS_REV_FCH_QTR_RATE	= 2,	//!<' quarter rate
	FTM_CDMA2000_NS_REV_FCH_8TH_RATE	= 3		//!<' eighth rate
} FTM_CDMA2000_NS_REV_FCH_Rates_Enum;


/**
	1x FCH rate SETS
*/
typedef enum
{
	FTM_CDMA2000_NS_RC_1	= 1,	//!<' RC1
	FTM_CDMA2000_NS_RC_2	= 2,	//!<' RC2
	FTM_CDMA2000_NS_RC_3	= 3,	//!<' RC3
	FTM_CDMA2000_NS_RC_4	= 4,	//!<' RC4
	FTM_CDMA2000_NS_RC_5	= 5		//!<' RC5
} FTM_CDMA2000_NS_FWD_RC_Enum;



/**
	CDMA2000 SCH Coding types
*/
typedef enum
{
	FTM_SCH_CODING_CONVOLUTIONAL,
	FTM_SCH_CODING_TURBO
} FTM_SCH_CODING_Enum;


/**
	CDMA2000 SCH reverse link data rates
*/
typedef enum
{
	FTM_REV_SCH_1_5_RATE,	//!<' (0 = 1.5 kbps),
	FTM_REV_SCH_2_7_RATE,	//!<'  (1 = 2.7 kbps),
	FTM_REV_SCH_4_8_RATE,	//!<'  (2 = 4.8 kbps),
	FTM_REV_SCH_9_6_RATE,	//!<'  (3 = 9.6 kbps),
	FTM_REV_SCH_19_2_RATE,	//!<' (4 = 19.2 kbps), 
	FTM_REV_SCH_38_4_RATE,	//!<'  (5 = 38.4 kbps), 
	FTM_REV_SCH_76_8_RATE,	//!<'  (6 = 76.8kbps), and 
	FTM_REV_SCH_153_6_RATE	//!<'  (7 = 153.6kbps) respectively.

	// Note that for turbo-encoded frames, the minimum supported rate is FTM_REV_SCH_19_2_RATE.

} FTM_SCH_DataRates_Enum;


/**
	CDMA2000 SCH foward link data rates
*/
typedef enum
{
	FTM_RATE_1X,	//!<' (9.6 kbps)
	FTM_RATE_2X,	//!<' (19.2 kbps)
	FTM_RATE_4X,	//!<' (38.4 kbps)
	FTM_RATE_8X,	//!<' (76.8 kbps)
	FTM_RATE_16X	//!<' (153.6 kbps)
} ftm_spread_rate_type;


/**
	CDMA2000 Non-signaling return status
*/
typedef enum
{
	_FTM_CDMA2000_CMD_SUCCESS		= 0,				//!<'Indicates Success
	_FTM_CDMA2000_INVALID_CMD		= 1,				//!<'Indicates an invalid cmd
	_FTM_CDMA2000_INVALID_STATE		= 2,				//!<'Indicates that FTM cannot process the command in current state
	_FTM_CDMA2000_NO_SUCCESS		= 3					//!<'Indicates failure
} FTM_CDMA2000_CMD_STATUS;

/**
	CDMA2000 Non-signaling Log types

	This is a list of the types of log events that will be monitored during CDMA2000 non-signaling
*/
typedef enum
{

	FTM_CDMA2000_NS_Log_Searcher,				//!<' CDMA2000 Searcher (LOG_CODE 0x119C)
	FTM_CDMA2000_NS_Log_AGC_C0,					//!<' FTM2 log, sub log 5, for Primary AGC
	FTM_CDMA2000_NS_Log_AGC_C1,					//!<' FTM2 log, sub log 7, for Diversity AGC
	FTM_CDMA2000_NS_Log_RDA2,					//!<' CDMA2000 (LOG_CODE 0x10D9), used for SER
	FTM_CDMA2000_NS_Log_RDA,					//!<' CDMA2000 (LOG_CODE 0x10C9), used for SER

	FTM_CDMA2000_NS_Log_MAX						//!<' Maximum # of logs that are being scanned for

}FTM_CDMA2000_NS_Log_Types_Enum;

/**
	CDMA2000 Non-signaling Event types

	This is a list of the types of event types that will be monitored during CDMA2000 non-signaling.

	Generally an event is based on reveiving one asynchronous log message + checking the state 
	of one log sub fields.

*/
typedef enum
{
	FTM_CDMA2000_NS_Event_PilotAcq,				//!<' Searcher State = 0x3 (Acquisition of the pilot channel)
	FTM_CDMA2000_NS_Event_RecSync,				//!<' Searcher State = 0x4 (Reception of the sync channel)
	FTM_CDMA2000_NS_Event_Traffic,				//!<' Searcher State = 0x8 (Operation on the traffic channel)
	
	FTM_CDMA2000_NS_Event_RDA_Update,			//!<' Rate determination algorithm report #1
	FTM_CDMA2000_NS_Event_RDA2_Update,			//!<' Rate determination algorithm report #2
	FTM_CDMA2000_NS_Event_PrimaryAGC,			//!<' Primary AGC
	FTM_CDMA2000_NS_Event_SecondaryAGC,			//!<' Secondary AGC
	
	FTM_CDMA2000_NS_Event_MAX					//!<' Maximum # of events

} FTM_CDMA2000_NS_Event_Types_Enum;


/**
	Searcher states, as used in "Searcher Status subpacket" in Srch TNG General Status Log (0x119C)
*/
typedef enum
{
	FTM_CDMA2000_Searcher_RawInit			= 0x00,	//!<'  Raw initialization state
	FTM_CDMA2000_Searcher_DeepSleep			= 0x01,	//!<'  Deep sleep in start state
	FTM_CDMA2000_Searcher_InitStateCDMA		= 0x02,	//!<'  Initial state for CDMA operation
	FTM_CDMA2000_Searcher_PilotAcq			= 0x03,	//!<'  Acquisition of the pilot channel
	FTM_CDMA2000_Searcher_RecSync			= 0x04,	//!<'  Reception of the sync channel
	FTM_CDMA2000_Searcher_SyncToPage		= 0x05,	//!<'  Transition from sync to paging channel
	FTM_CDMA2000_Searcher_Paging			= 0x06,	//!<'  Operation on the paging channel
	FTM_CDMA2000_Searcher_SlottedSleep		= 0x07,	//!<'  Slotted mode sleep state
	FTM_CDMA2000_Searcher_Traffic			= 0x08,	//!<'  Operation on the traffic channel
	FTM_CDMA2000_Searcher_RetToSync			= 0x09,	//!<'  Return from paging or traffic to sync channel (unslew)
	FTM_CDMA2000_Searcher_PCG				= 0x0a,	//!<'  Operation in PCG state
	FTM_CDMA2000_Searcher_PowerUp			= 0x0b	//!<'  Powerup state 
													
} FTM_CDMA2000_SearcherStates_Enum;



/**
	Statistics for 1X AGC, structure definition is shared between Primary and secondary antennas,
	but separate instances will be created for both
	
*/
typedef struct 
{
	dword iTotalRecords;	//!< Total number of AGC records that have been received

	//unsigned double ftm_log_id;
	double dRxAGC;				//!< AGC units
	double dRxAGC_linear;		//!< Linear power (mW)
	double dRxAGC_dBm;			//!< Log power (dBm)

	double dTxGainCtl;			//!< AGC units
	double dTxGainCtl_linear;	//!< Linear power (mW)
	double dTxGainCtl_dBm;		//!< Log power (dBm)

	double dTxGainAdj;			//!< AGC units
	double dTxGainAdj_dB;		//!< Log power (dB)

	double dTxOpenLoop;			//!< AGC units
	double dTxOpenLoop_linear;	//!< Linear power (mW)
	double dTxOpenLoop_dBm;		//!< Log power (dBm)

	double dTxAgcAdj;
	double dPA_Ratchet;
	double dLNA_State;
	double dPA_State;
	double dHDET_Raw;
	double dThermRaw;
	double dThermScaled;
	double dTempIndex;
	double dTempRemainder;
	double dIntelliceiverState;
} FTM_LOG_1X_AGC_Statistics;


/**
	SER statistics structure to be used for cdma2000 non-signaling.
*/
typedef struct
{
	long iSumFrames;		//!< Number of frames counted
	long iSumSymbols;		//!< Number of symbols counted

	long iSumFramesErrors;	//!< Number of frames errors counted
	long iSumSymbolErrors;	//!< Number of symbols errors counted

	double dFER;			//!< Calculated FER
	double dSER;			//!< Calculated SER
} FTM_CDMA2000_SER_Statistics;

/**
	Structure to be used for cdma2000 non-signaling status.

	Contains searcher status, AGC statistics, and SER/FER statistics.
*/
typedef struct
{
	//! Array to keep track of whether a certain event has occured, indexed to FTM_CDMA2000_NS_Event_Types_Enum
	byte bHasUpdated[ FTM_CDMA2000_NS_Event_MAX ];

	/**	
		Searcher information
	*/
	FTM_CDMA2000_SearcherStates_Enum eSearcherState;

	//FTM_LOG_EVDO_CtoI_Statistics_Struct oEVDO_CtoI_Stats;

	/**	
		Data for primary AGC
	*/
	FTM_LOG_1X_AGC_Struct oCDMA2000_AGC_C0;					//!< Most recent Primary AGC information
	FTM_LOG_1X_AGC_Statistics oCDMA2000_AGC_C0_LatestStats;	//!< Most recent Statistics for Primary AGC, transformed from oCDMA2000_AGC_C0
	FTM_LOG_1X_AGC_Statistics oCDMA2000_AGC_C0_SumStats;	//!< Summed Statistics for Primary AGC
	FTM_LOG_1X_AGC_Statistics oCDMA2000_AGC_C0_AvgStats;	//!< Averageed statistics for Secondary AGC

	/** 
		Data for secondary AGC
	*/
	FTM_LOG_1X_AGC_C1_Struct oCDMA2000_AGC_C1;			//!< Most recent Secondary AGC information
	FTM_LOG_1X_AGC_Statistics oCDMA2000_AGC_C1_LatestStats;	//!< Most recent Statistics for Secondary AGC, transformed from oCDMA2000_AGC_C1
	FTM_LOG_1X_AGC_Statistics oCDMA2000_AGC_C1_SumStats;	//!< Summed for Secondary AGC
	FTM_LOG_1X_AGC_Statistics oCDMA2000_AGC_C1_AvgStats;	//!< Averageed statistics for Secondary AGC

	/**
		FCH SER statistics
	*/
	FTM_CDMA2000_SER_Statistics oCDMA2000_FCH_SER_Stats;	

	/**
		FCH SER statistics per data rate
	
		Indexed by the enumeration: RDA_CDMA_TRAFFIC_DATA_RATE_TYPE;

	*/
	FTM_CDMA2000_SER_Statistics oCDMA2000_FCH_SER_Stats_perRate[ RDA_DataRate_Max ];	

	/**
		DCCH SER statistics
	*/
	FTM_CDMA2000_SER_Statistics oCDMA2000_DCCH_SER_Stats;	

	/**
		SCH SER statistics
	*/
	FTM_CDMA2000_SER_Statistics oCDMA2000_SCH_SER_Stats;	

}FTM_CDMA2000_NS_Status_Struct;


/******************************************************************************
						FTM - EVDO non-signaling log parser definitions
*******************************************************************************/

//! Definition of log code for 1xEV-DO Finger Info
#define FTM_EVDO_NS_LOG_CODE_FINGER_INFO		0x108A

//! Definition of log code for 1xEV-DO Air Link Summary
#define FTM_EVDO_NS_LOG_CODE_AIR_LINK_SUMMARY	0x1068

//! Definition of log code for 1xEVDO Foward link statistics summary
#define EVDO_FORWARD_LINK_STATISTICS_SUMMARY	0x1084


/**
	Structure of FTM2 EVDO Primary AGC, FTM Log code = LOG_FTM2_LOG_HDR_AGC		= 0x5
*/
typedef struct 
{
	word ftm_log_id;
	short rx_agc;
	short tx_gain_ctl;
	short tx_gain_adj;
	short tx_open_loop;
	word tx_agc_adj;
	byte pa_ratchet;
	byte lna_state;
	byte pa_state;
	byte hdet_raw;
	word therm_raw;
	byte therm_scaled;
	byte temp_index;
	byte temp_remainder;
	byte intelliceiver_state;
} FTM_LOG_EVDO_AGC_C0_Struct;


/**
	Structure of FTM2 EVDO Secondary AGC, FTM Log Code = LOG_FTM2_LOG_HDR_AGC_C1 = 0x7
*/
typedef struct 
{
	word ftm_log_id;
	short rx_agc;
	short tx_gain_ctl;
	short tx_gain_adj;
	short tx_open_loop;
	word tx_agc_adj;
	byte pa_ratchet;
	byte lna_state;
	byte pa_state;
	byte hdet_raw;
	word therm_raw;
	byte therm_scaled;
	byte temp_index;
	byte temp_remainder;
	short rx_agc_c1;
	byte lna_state_c1;
	byte intelliceiver_state;
} FTM_LOG_EVDO_AGC_C1_Struct;

/**
	Structure of the per-finger data used in log code 0x108A
*/
typedef struct 
{
	word PilotPN;			//!<' 
	dword  RTCOffset;		//!<' 
	word C2I;				//!<' 
	byte locked_antenna_diversity_fingindex;	//!<' 
	byte RPCCellIdx_ASPIdx;						//!<' 
	word Antenna0C2I;				//!<' C/I equivalent of the finger for antenna 0; only valid when
									//!<' Diversity is enabled
	word Antenna1C2I;				//!<' C/I equivalent of the finger for antenna 1; only valid when
									//!<' Diversity is enabled
} LOG_1XEVDO_Finger_Ver2_FingerInfo_Struct;

#define LOG_1XEVDO_MAX_FINGERS 12
/**
	Structure of the header information for log code 0x108A,
	at the end is the data for up to 12 fingers
*/
typedef struct
{
	byte srch_state;				//!<' State of the HDRSRCH state machine; refer to for nomenclature
	dword MSTR;						//!<' Mobile station (AT) time reference offset relative to the RTC 
									//!<' timebase in units of 1/8 chip

	word MSTRError;					//!<' MSTR offset relative to earliest arriving in-lock and
									//!<' enabled active set pilot in units of 1/8 chip

	word MSTRPilotPN;				//!<' Pilot PN of the finger, which the MSTR is tracking;
									//!<' typically, the earliest arriving finger

	byte NumFingers;		//!<' Number of fingers (12 max)
	LOG_1XEVDO_Finger_Ver2_FingerInfo_Struct fingerData[ LOG_1XEVDO_MAX_FINGERS ];	//!<' Finger data

} LOG_1XEVDO_Finger_Ver2_Info_Struct;


/** Structure of the header information for log code 0x1084
	1xEV-DO Forward Statistics Summery 
*/
typedef struct
{
	dword	cc38400_good;
	dword	cc38400_bad;
	dword	cc76800_good;
	dword	cc76800_bad;
	dword	tc38400_good;
	dword	tc38400_bad;
	dword	tc76800_good;
	dword	tc76800_bad;
	dword	tc153600_good;
	dword	tc153600_bad;
	dword	tc307200short_good;
	dword	tc307200short_bad;
	dword	tc307200long_good;
	dword	tc307200long_bad;
	dword	tc614400short_good;
	dword	tc614400short_bad;
	dword	tc614400long_good;
	dword	tc614400long_bad;
	dword	tc921600_good;
	dword	tc921600_bad;
	dword	tc1228800short_good;
	dword	tc1228800short_bad;
	dword	tc1228800long_good;
	dword	tc1228800long_bad;
	dword	tc1843200_good;
	dword	tc1843200_bad;
	dword	tc2457600_good;
	dword	tc2457600_bad;
	dword	tcrate38400[16];
	dword	tcrate76800[8];
	dword	tcrate153600[4];
	dword	tcrate307200short[2];
	dword	tcrate307200long[4];
	dword	tcrate614400short;
	dword	tcrate614400long[2];
	dword	tcrate921600[2];
	dword	tcrate1228800short;
	dword	tcrate1228800long[2];
	dword	tcrate1843200;
	dword	tcrate2457600;
	dword	ccrate38400[16];
	dword	ccrate76800[8];
	dword	seq_num;
}LOG_1XEVDO_Forward_Statistics_Summary_Struct;


/******************************************************************************
						FTM - EVDO non-signaling
*******************************************************************************/

/**
	EVDO Return Status
*/
typedef enum
{
	_FTM_EVDO_CMD_SUCCESS			= 0,		//!<'Indicate Success
	_FTM_EVDO_CMD_INVALID_STATE		= 1,		//!<'Indicates that FTM cannot process the cmd in current state
	_FTM_EVDO_CMD_NO_SUCCESS		= 2			//!<'Indicates faiure
} FTM_EVDO_CMD_STATUS;

/**
	EVDO Band Class identifiers for QLIB_FTM_EVDO_PILOT_ACQ

	The enumeration values are the same as the band class number.  If an 
	enumeration is not available for a new band class number then the 
	an integer with the band class number can be used.

*/
typedef enum 
{
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

} FTM_EVDO_BandClasstype;

/**
	EVDO Band Class ACK type for reverse traffic channel identifiers for QLIB_FTM_EVDO_PILOT_ACQ
*/
typedef enum  
{
	FTM_EVDO_IS856_COMPLIANT = 0,       //!<' 0 Normal IS-856 operation: early decode enabled; no ACK override
	FTM_EVDO_IS856_NO_ACK,              //!<' 1 IS-856 but with no early termination; early decode disabled 
	FTM_EVDO_IS890_MODE_0,              //!<' 2 First  slot decode mode; ACK each slot (test app mode) 
	FTM_EVDO_IS890_MODE_1,              //!<' 3 Full packet decode mode; NAK each slot (test app mode)
	FTM_EVDO_IS890A_MODE_ACK_BPSK,      //!<' 4 Always ACK with bipolar keying (BPSK) modulation 
	FTM_EVDO_IS890A_MODE_NAK_BPSK,      //!<' 5 Always NAK with bipolar keying (BPSK) modulation 
	FTM_EVDO_IS890A_MODE_ACK_OOK,       //!<' 6 Always ACK with on-off keying (OOK) modulation 
	FTM_EVDO_IS890A_MODE_NAK_OOK        //!<' 7 Always NAK with on-off keying (OOK) modulation
} FTM_EVDO_MAC_ACK_Mode_Enum;

/**
	EVDO dynamic range types

	This is a list of dynamic range configurations used for 1xEV-DO non-signaling,
  to convert results from AGC units to dB and dBm
*/
typedef enum
{
    FTM_EVDO_NS_DR_85_3,				//!<' 85.3dB dynamic range, min_rssi = -106.0dBm
    FTM_EVDO_NS_DR_102_4				//!<' 102.4dB dynamic range, min_rssi = -115.0dBm
} FTM_EVDO_NS_DR_Types_Enum;

/**
	EVDO Non-signaling Log types

	This is a list of the types of log events that will be monitored during EVDO non-signaling
*/
typedef enum
{
	FTM_EVDO_NS_Log_FingerInfo,				//!<' 1xEV-DO Finger, Ver 2 (LOG_CODE 0x108A)
	FTM_EVDO_NS_Log_AGC_C0,					//!<' FTM2 log, sub log 5, for Primary AGC
	FTM_EVDO_NS_Log_AGC_C1,					//!<' FTM2 log, sub log 7, for Diversity AGC
	FTM_EVDO_NS_Log_PER,					//!<' 1xEV-DO Air Link Summary (LOG_CODE 0x1068), used for PER
	FTM_EVDO_NS_Log_FWD_Stat_Summary,		//!<' 1xEV-DO Forward Statistis Summary (LOG_CODE 0x1084)

	FTM_EVDO_NS_Log_MAX						//!<' Maximum 3 of logs that are being scanned for
		
} FTM_EVDO_NS_Log_Types_Enum;


/**
	EVDO Non-signaling Event types

	This is a list of the types of event types that will be monitored during EVDO non-signaling.

	Generally an event is based on reveiving one asynchronous log message + checking the state 
	of one log sub fields.

*/
typedef enum
{
	FTM_EVDO_NS_Event_Synchronization,		//!<' FingerInfo log, Searcher == "Synchronization" after Aquire Pilot
	FTM_EVDO_NS_Event_Idle,					//!<' FingerInfo log, Searcher == "Idle" after Aquire system time
	FTM_EVDO_NS_Event_Traffic,				//!<' FingerInfo log, Searcher == "Traffic" after Modulate Reverse Traffic
	FTM_EVDO_NS_Event_Deassign,				//!<' FingerInfo log, Searcher == "Inactive" after Deassign 
											//!<' (not working on all targets)

	FTM_EVDO_NS_Event_PrimaryAGC,			//!<' FTM2 log, sub log 5, for Primary AGC
	FTM_EVDO_NS_Event_SecondaryAGC,			//!<' FTM2 log, sub log 7, for Diversity AGC
	FTM_EVDO_NS_Event_PER_Update,			//!<' 1xEV-DO Air Link Summary (LOG_CODE 0x1068), used for PER
	FTM_EVDO_NS_Event_C2I_Update,			//!<' FingerInfo log, C2I updates from individual finger reports
	FTM_EVDO_NS_Event_FWD_Stat_Summary_Update,	//!<' 1xEV-DO Forward Statistis Summary (LOG_CODE 0x1084)

	FTM_EVDO_NS_Event_MAX					//!<' Maximum # of events

} FTM_EVDO_NS_Event_Types_Enum;


/**
	Searcher states, as used in the "SrchState" field of 1xEV-DO Finger log (0x108A)
*/
typedef enum
{
	FTM_EVDO_NS_Searcher_Inactive			= 0x00,	//!<'  Start (inactive)
	FTM_EVDO_NS_Searcher_Acquisition		= 0x01,	//!<'  Acquisition 0x01
	FTM_EVDO_NS_Searcher_Synchronization	= 0x02,	//!<'  Synchronization 0x02
	FTM_EVDO_NS_Searcher_Idle				= 0x03,	//!<'  Idle 0x03
	FTM_EVDO_NS_Searcher_IdleSuspended		= 0x3a,	//!<'  Idle suspended 0x3a
	FTM_EVDO_NS_Searcher_IdleOffFreq		= 0x3c,	//!<'  Idle off-frequency searching (OFS) 0x3c
	FTM_EVDO_NS_Searcher_Sleep				= 0x3e,	//!<'  Sleep 0x3e
	FTM_EVDO_NS_Searcher_Reqacusition		= 0x3f,	//!<'  Reacquisition 0x3f
	FTM_EVDO_NS_Searcher_Traffic			= 0x04,	//!<'  Traffic 0x04
	FTM_EVDO_NS_Searcher_TrafficSuspended	= 0x4a,	//!<'  Traffic suspended 0x4a
	FTM_EVDO_NS_Searcher_Traffic_OFS		= 0x4c,	//!<'  Traffic OFS 0x4c
	FTM_EVDO_NS_Searcher_Invalid			= 0xFF	//!<'  Invalid (defined at PC Library level, not embedded)
} FTM_EVDO_NS_SearcherStates_Enum;


/**
	Structure to contain statistics about the EVDO AGC History for either Primary or diversity AGC.

	One instance of this structure will be created for both "sums" and "averages" 
	for both antenna 0 and antenna 1.  This means that a total of 4 will be available:
		1) C0 Sums
		2) C0 Averages
		3) C1 Sums
		4) C1 Averages

	For the following fields, when averages are calculated, the fields are converted to dB:
		double dTxGainAdj;				//!< tx_gain_adj;

	For the following fields, when averages are calculated, the fields are converted to dBm:
		double dRxAGC;					//!< rx_agc field for C0 and rx_agc_c1 for C1 logs
		double dTxGainCtl;				//!< tx_gain_ctl field
		double dTxOpenLoop;				//!< tx_open_loop;
*/
typedef struct 
{
	dword iTotalRecords;	//!< Total number of AGC records that have been received

	double dRxAGC;					//!< rx_agc field for C0 and rx_agc_c1 for C1 logs -- AGC units
	double dRxAGC_linear;			//!< Linear power (mW)
	double dRxAGC_dBm;				//!< Log power (dBm)

	double dTxGainCtl;				//!< tx_gain_ctl field -- AGC units
	double dTxGainCtl_linear;		//!< Linear power (mW)
	double dTxGainCtl_dBm;			//!< Log power (dBm)

	double dTxGainAdj;				//!< tx_gain_adj -- AGC units
	double dTxGainAdj_dB;			//!< Log power (dB)

	double dTxOpenLoop;				//!< tx_open_loop -- AGC units
	double dTxOpenLoop_linear;		//!< Linear power (mW)
	double dTxOpenLoop_dBm;			//!< Log power (dBm)

	double dTxAgcAdj;				//!< tx_agc_adj -- AGC units

	double dPA_Ratchet;				//!< pa_ratchet;
	double dLNA_State;				//!< lna_state for C0 and lna_state_c1 for C1
	double dPA_State;				//!< pa_state
	double dHDET_Raw;				//!< hdet_raw;
	double dThermRaw;				//!< therm_raw;
	double dThermScaled;			//!< therm_scaled;
	double dTempIndex;				//!< temp_index
	double dTempRemainder;			//!< temp_remainder
	double dIntelliceiverState;		//!< intelliceiver_state, only for C0

} FTM_EVDO_NS_AGC_Statistics;

/**
	Structure to contain statistics about the C/I measurements
*/
typedef struct 
{
	dword iTotalReports;			//!< Total # of C/I reports, each report hold 16 packet results

	long iSumC2I;					//!< Sum of all combined CtoI reports 
	double dC2I_dB;					//!< Calculated Combined C2I, C_I0 (dB) = 10 x Log10 (C2I/512)

	long iSumC2I_C0;				//!< Sum of all CtoI reports for primary antenna
	double dC2I_C0_dB;				//!< Calculated C2I for primary antenna, C_I0 (dB) = 10 x Log10 (C2I/512)

	dword iSumC2I_C1;				//!< Sum of all CtoI reports for diversity antenna
	double dC2I_C1_dB;				//!< Calculated C2I for diversity antenna, C_I0 (dB) = 10 x Log10 (C2I/512)

} FTM_LOG_EVDO_CtoI_Statistics_Struct;


/**
	Structure of the log message 0x1068, 1xEV-DO Air Link Summary
*/
typedef struct 
{

	byte CDRCBuffer[8];			//!<' DRC indices predicted by the predictor for the 16 slots of
								//!<' the previous frames. Each DRC index is 4 bits. Two DRC
								//!<' indices are combined into 1 byte.

	word BestASPSINRBuffer[16];	//!<' SINR for the BEST ASP for previous frame’s last 16 odd
								//!<' half slots. This represents for both the antennas.
	byte Flags;					//!<' PktRcvdFlag, PAState, BestASPPredicted

	word ASPFilteredSINR[6];	//!<' Filtered SINR for the 6 ASPs [1,6]

	word PilotPNASP[6];			//!<' Pilot PN for the 6 ASPs [1,6]

	short RPC[6];				//!<' Filtered RPC values read from the DSP for six possible
								//!<' ASPs [1,6]. RPC[..] values are IIR filtered by the DSP
								//!<' over past 64 samples.

	short PERInst;				//!<' Instantaneous Packet Error Rate. This value is calculated
								//!<' once every 16 packets are received.
								//!<' PERInst = Number of bad packets / 16; 16384 = 100%

	word PERSequence;			//!<' Packet Error Rate sequence. This represents the number
								//!<' of times the PERInstantaneous value is calculated.

	word HSTR;					//!<' Current half slot reference count (DSP register)

	byte RPCCellIndex[6];		//!<' RPC Cell map for each ASP index

} LOG_1XEVDO_Air_Link_Summary_Struct;

/**
	Structure to contain statistics about the EVDO PER History,
	from (event == FTM_EVDO_NS_Event_PER_Update)
*/
typedef struct 
{
	dword iTotalReports;		//!< Total # of PER reports, each report hold 16 packet results

	dword iTotalPackets;		//!< Total # of packets that are included

	double dSumOfPER;			//!< Total of all instantaneous (kept in "bad packets/16" units)


	/*
		Calculated "PER short" average based on all the PER reports and adjusted for 16 packets per report

	*/
	double dPER_Calculated;				


} FTM_EVDO_NS_PER_Statistics_Struct;

/**
	Searcher states, as used in the "SrchState" field of 1xEV-DO Finger log (0x108A)
*/
typedef struct
{
	//! Array to keep track of whether a certain event has occured, indexed to FTM_EVDO_NS_Event_Types_Enum
	byte bHasUpdated[ FTM_EVDO_NS_Event_MAX ];

	/**	
		Finger information
	*/
	FTM_EVDO_NS_SearcherStates_Enum eSearcherState;

	FTM_LOG_EVDO_CtoI_Statistics_Struct oEVDO_LatestCtoI_Stats;	//!<' Most recent CtoI report
	FTM_LOG_EVDO_CtoI_Statistics_Struct oEVDO_CtoI_Stats;		//!<' Average CtoI 

	/**	
		Data for primary AGC
	*/
	FTM_LOG_EVDO_AGC_C0_Struct oEVDO_AGC_C0;				//!<' Most recent Primary AGC information
	FTM_EVDO_NS_AGC_Statistics oEVDO_AGC_C0_LatestStats;	//!<' Most recent primary AGC report, converted to dBm
	FTM_EVDO_NS_AGC_Statistics oEVDO_AGC_C0_SumStats;		//!<' Sums Primary AGC
	FTM_EVDO_NS_AGC_Statistics oEVDO_AGC_C0_AvgStats;		//!<' Averages for Primary AGC


	/** 
		Data for seC0ndary AGC
	*/
	FTM_LOG_EVDO_AGC_C1_Struct oEVDO_AGC_C1;				//!<' Most recent secondary AGC information
	FTM_EVDO_NS_AGC_Statistics oEVDO_AGC_C1_LatestStats;	//!<' Most recent secondary AGC report, converted to dBm
	FTM_EVDO_NS_AGC_Statistics oEVDO_AGC_C1_SumStats;		//!<' Sums for secondary AGC
	FTM_EVDO_NS_AGC_Statistics oEVDO_AGC_C1_AvgStats;		//!<' Averages for secondary AGC

	/**
		Data for PER (event == FTM_EVDO_NS_Event_PER_Update)
	*/
	word iLastInstantaneousPER;						//!<' Most recent instantaneous PER recorded, 
													//!<' PERInst = Number of bad packets / 16, 16384 = 100%

	FTM_EVDO_NS_PER_Statistics_Struct oPER_Stats;	//!<' PER Statistics

	/**
		Last received Air Link Summary log message
	*/
	LOG_1XEVDO_Air_Link_Summary_Struct oAirLinkSummary;
	
	/**
		Last received EVDO Forward Statistics Summary
	*/
	LOG_1XEVDO_Forward_Statistics_Summary_Struct oFWDStatSummary;

} FTM_EVDO_NS_Status_Struct;


//! Define for the bWaitForStatus flag for the EVDO NS functions -- indicate wait for status
#define FTM_EVDO_NS_WaitForStatus TRUE

//! Define for the bWaitForStatus flag for the EVDO NS functions -- indicate no wait for status
#define FTM_EVDO_NS_NoWaitForStatus FALSE


/**
	Bit mask enumeration of measurement logs available for 1XEVD0 non-signaling
*/
typedef enum
{
	FTM_EVDO_NS_LOG_AIR_LINK_SUMMARY = 0x01,  //!<'  Air link summary
	FTM_EVDO_NS_LOG_AGC_C0           = 0x02,  //!<'  FTM AGC Log for primary Rx (C0)
	FTM_EVDO_NS_LOG_AGC_C1           = 0x04,  //!<'  FTM AGC Log for secondary Rx (C1)
	FTM_EVDO_NS_LOG_Default          = 0x07   //!<'  Default is to include all logs

} FTM_EVDO_NS_MeasurementLogs_Enum;



/******************************************************************************
						FTM - Bluetooth
*******************************************************************************/

/**
	\brief Enumeration of Bluetooth FTM Command ID's
*/
typedef enum 
{
	_FTM_BT_HCI_USER_CMD		= 0		//!<' Bluetooth HCI User Command.  Use _ to avoid name conflict with function
} FTM_Bluetooth_Cmd_Id_Enum;


/******************************************************************************
						FTM - AGPS
*******************************************************************************/

/**
	\brief Enumeration of AGS FTM Command ID's
			Use _ to differentiate from the function names
*/
typedef enum 
{
	_FTM_AGPS_SET_TEST_MODE			= 0,	//!<' Set AGPS Test Mode
	_FTM_AGPS_SELF_TEST				= 1,	//!<' AGPS Self Test
	_FTM_AGPS_IDLE_MODE				= 2,	//!<' Set AGPS Idle Mode
	_FTM_AGPS_STANDALONE_TEST_MODE	= 3,	//!<' Set AGPS Standalone Test Mode
	_FTM_AGPS_SV_TRACKING			= 4,	//!<' Set AGPS single SV tracking 
	_FTM_AGPS_IQ_CAPTURE			= 5,	//!<' Set AGPS IQ Capture
	_FTM_AGPS_GET_IQ_DATA			= 6,		//!<' Set AGPS Get IQ data
	_FTM_AGPS_GET_CTON				= 122	//!<' Get AGPS C/N

} FTM_AGPS_Cmd_Id_Enum;

/******************************************************************************
						FTM - PMIC
*******************************************************************************/
/**
	\brief Enumeration of PMIC sub Command ID's
*/
typedef enum 
{
	_FTM_PMIC_TEST_RTC		= 0,	//!<' PMIC Real Time Clock
	_FTM_PMIC_TEST_CHG		= 1,	//!<' PMIC Charger control
    _FTM_PMIC_TEST_USB		= 2,	//!<' PMIC USB
    _FTM_PMIC_TEST_AMUX		= 3,	//!<' PMIC Analog Multiplexer
    _FTM_PMIC_TEST_VREG		= 4,	//!<' PMIC Voltage Regulator
    _FTM_PMIC_TEST_INT		= 5,	//!<' PMIC Interrupts
    _FTM_PMIC_TEST_UI		= 6	,	//!<' PMIC User Interface Commands
	_FTM_PMIC_TEST_MPP		= 12,	//!<' PMIC Test Multi-purpose Pin (MPP)
	_FTM_PMIC_TEST_GEN		= 13	//!<' PMIC General testing
} FTM_PMIC_Cmd_Id_Enum;

/**
	\brief Enumeration of PMIC FTM Sub-command ID's
*/
typedef enum 
{
	// FTM_PMIC_TEST_RTC group
	_FTM_PMIC_RTC_SET_GET_TIME			= 0,	//!<' PMIC RTC Set/Get time
	_FTM_PMIC_RTC_SET_GET_ALARM_TIME	= 1,	//!<' PMIC RTC Set/Get alarm
	_FTM_PMIC_RTC_STOP					= 2,	//!<' PMIC RTC Stop real time clock
	_FTM_PMIC_RTC_GET_ALARM_STATUS		= 3,	//!<' PMIC RTC Get alarm status
	_FTM_PMIC_RTC_DISABLE_ALARM			= 4,	//!<' PMIC RTC Disable Alarm
	_FTM_PMIC_RTC_SET_GET_TIME_ADJUST	= 5,	//!<' PMIC RTC Get/set time adjustment

	// Charger Commands
	_FTM_PMIC_CHG_SET_SWITCH_STATE		= 0,	//!<' PMIC Charger, Set switch state
    _FTM_PMIC_CHG_SET_CONFIG			= 1,	//!<' PMIC Charger, configure

	// Analog Mux
	_FTM_PMIC_AMUX_CONFIG				= 0,		//!<' PMIC Analog Mux, Configure

	// Voltage Reglator
    _FTM_PMIC_VREG_CONTROL				= 0,	//!<' PMIC Vreg, Control LDO
    _FTM_PMIC_VREG_SET_LEVEL			= 1,	//!<' PMIC Vreg, Set voltage level
    _FTM_PMIC_VREG_CONFIG_SMPS_MODE		= 2,	//!<' PMIC Vreg, configure SMPS mode
    _FTM_PMIC_VREG_CONFIG_LDO_PD_SW		= 3,	//!<' PMIC Vreg, Configure LDO Power Down
    _FTM_PMIC_VREG_LP_MODE_CONTROL		= 4,	//!<' PMIC Vreg, Configure Low Power mode
    _FTM_PMIC_VREG_SMPS_CLK_CONFIG		= 5,	//!<' PMIC Vreg, Configure SMPS clock
	_FTM_PMIC_VREG_GET_LEVEL			= 6,	//!<' PMIC Vreg, Get voltage level
	_FTM_PMIC_VREG_GET_ENUM_LIST		= 7,	//!<' PMIC Vreg, Get a list of vreg enumeration

	// Interrupts
    _FTM_PMIC_INT_GET_RT_STATUS			= 0,	//!<' PMIC Interrupt, Get interrupt status
    _FTM_PMIC_INT_CLR_IRQ				= 1,	//!<' PMIC Interrupt, clear interrupts

	// User Interface
    _FTM_PMIC_UI_SET_DRIVE_LEVEL		= 0,	//!<' PMIC User Interface, set drive level
    _FTM_PMIC_UI_CONFIG_SPKR			= 1,	//!<' PMIC User Interface, configure speaker

	// MPP
	_FTM_PMIC_MPP_CONFIG_DIGITAL_IN		= 0,	//!<' PMIC MPP, Config digital input pin
	_FTM_PMIC_MPP_CONFIG_DIGITAL_OUT	= 1,	//!<' PMIC MPP, Config digital output pin
	_FTM_PMIC_MPP_CONFIG_DIGITAL_INOUT	= 2,	//!<' PMIC MPP, Config digital bidirectional pin
	_FTM_PMIC_MPP_CONFIG_ANALOG_IN		= 3,	//!<' PMIC MPP, Config analog input pin
	_FTM_PMIC_MPP_CONFIG_ANALOG_OUT		= 4,	//!<' PMIC MPP, Config analog output pin
	_FTM_PMIC_MPP_CONFIG_I_SINK			= 5,	//!<' PMIC MPP, Config a pin to be current sink

	// General
	_FTM_PMIC_GET_MODEL					= 0		//!<' PMIC General, Get Model


} FTM_PMIC_SubCmd_Id_Enum;


typedef enum 
{
	// FTM_PMIC_TEST_RTC group
	FTM_PMIC_SET_VALUE = 0,	//!<' PMIC Set value
	FTM_PMIC_GET_VALUE = 1	//!<' PMIC Get value

} FTM_PMIC_SET_GET_Enum;

/* This type specifies the charger switch types available to control */

typedef enum
{

    FTM_PMIC_CHG_SWITCH_WALL_CHGR            = 0,
    FTM_PMIC_CHG_SWITCH_BATTERY_TRANSISTOR   = 1,
    FTM_PMIC_CHG_SWITCH_WALL_PULSE_CHGR      = 2,
    FTM_PMIC_CHG_SWITCH_VCP                  = 3,
    FTM_PMIC_CHG_SWITCH_BATT_ALARM           = 4,
    FTM_PMIC_CHG_SWITCH_COIN_CELL            = 5,
    FTM_PMIC_CHG_SWITCH_USB_CHGR             = 6,
    FTM_PMIC_CHG_SWITCH_CHG_APP              = 7,      //!<' Charger Application task
    FTM_PMIC_CHG_SWITCH_NUM_SWITCHES        

} ftm_pmic_chg_switch_type;


/** This type specifies the charger type available for configuration */
typedef enum
{
    
    FTM_PMIC_CHG_CONFIG_WALL_PULSE_CHGR            = 0,
    FTM_PMIC_CHG_CONFIG_WALL_PULSE_CHGR_VBAT_DET   = 1,
    FTM_PMIC_CHG_CONFIG_WALL_CHGR_TRANISTOR_LIMITS = 2,
    FTM_PMIC_CHG_CONFIG_TRICKLE_CHARGER            = 3,
    FTM_PMIC_CHG_CONFIG_COIN_CELL_CHGR             = 4,
    FTM_PMIC_CHG_CONFIG_BATT_ALARM                 = 5,
    FTM_PMIC_CHG_CONFIG_USB_TRANSISTOR_LIMITS      = 6,
    FTM_PMIC_CHG_CONFIG_PWR_CTRL_LIMIT             = 7

} ftm_pmic_chg_set_config_type;

/** This type specifies the charger types available for power control limiting configuration */
typedef enum
{

    FTM_PMIC_CHG_PWR_CTRL_LIMIT_WALL = 0,
    FTM_PMIC_CHG_PWR_CTRL_LIMIT_USB  = 1

} ftm_pmic_chg_pwr_ctrl_limit_select_type;

/** This type specifies the time format for the RTC commands */
typedef enum
{

    FTM_PMIC_RTC_MODE_12_HOUR_AM  = 0,
    FTM_PMIC_RTC_MODE_12_HOUR_PM  = 1,
    FTM_PMIC_RTC_MODE_24_HOUR     = 2

} ftm_pmic_rtc_mode_type;



/** This type specifies the VERG SMPS Clock commands */
typedef enum
{

    FTM_PM_VREG_SMPS_CLK_SEL_CMD       = 0,
    FTM_PM_VREG_SMPS_CLK_TCXO_DIV_CMD  = 1,
    FTM_PM_VREG_SMPS_CLK_INVALID_CMD

} ftm_pmic_vreg_smps_config_clk_cmd;



/** This type specifies the pin types for the FTM_SET_PMIC_DRIVE_LEVEL command */
typedef enum
{

    FTM_PMIC_UI_DRV_SEL_LCD = 0,
    FTM_PMIC_UI_DRV_SEL_KPD = 1,
    FTM_PMIC_UI_DRV_SEL_VIB = 2

} ftm_pmic_ui_high_drive_pin_id_type;


/* This type specifies the interface for the set time rtc cmd */

typedef struct
{
 
    byte      ftm_rtc_month;
    byte      ftm_rtc_day;
    word      ftm_rtc_year;
    byte      ftm_rtc_hour;
    byte      ftm_rtc_min;
    byte      ftm_rtc_sec;

} ftm_pmic_rtc_set_time_cmd_type;

/**
	\brief Enumeration of PMIC RTC Alarm ID's
*/
typedef enum 
{
	// FTM_PMIC_TEST_RTC group
	RTC_ALARM_1 = 1,
	RTC_ALARM_2 = 2,
	RTC_ALARM_3 = 3

} FTM_PMIC_RTC_Alarm_Id_Enum;



/******************************************************************************
						FTM - Audio
*******************************************************************************/
/**
	\brief Enumeration of Audio FTM Sub-command ID's
*/
typedef enum 
{
	_FTM_AUDIO_SET_PATH				= 0,	//!<'  Sets up the audio path
	_FTM_AUDIO_SET_VOLUME			= 1,	//!<'  Sets up the volume in the current audio path
	_FTM_AUDIO_DSP_LOOPBACK			= 2,	//!<'  Sets up audio loopback in the DSP
	_FTM_AUDIO_PCM_LOOPBACK			= 3,	//!<'  Sets up audio loopback in the codec
	_FTM_AUDIO_TONES_PLAY			= 4,	//!<'  Plays tones at specified frequency
	_FTM_AUDIO_TONES_STOP			= 5,	//!<'  Stops the tones that are currently playing
	_FTM_AUDIO_NS_CONTROL			= 6,	//!<'  Toggles the noise suppressor ON/OFF
	_FTM_AUDIO_PCM_CAPTURE			= 7,	//!<'  Start a PCM capture
	_FTM_AUDIO_GET_PCM_CAPTURE_DATA	= 8,	//!<'  Get data from a PCM capture
	_FTM_AUDIO_PCM_CAPTURE_STOP		= 9,	//!<'  Clear the PCM capture buffers
	_FTM_AUDIO_SET_CODECTXGAIN_ADJ	= 10	//!<'  Set the CodecTxGainAdjust

} FTM_Audio_SubCmd_Id_Enum;


/**
	\brief Enumeration of Audio Devices for use with the FTM_AUDIO_SET_PATH command
*/
typedef enum 
{
	FTM_AUDIO_PATH_HANDSET			= 0,	//!<'  Handset
	FTM_AUDIO_PATH_HANDSFREE		= 1,	//!<'  Handsfree kit
	FTM_AUDIO_PATH_MONO_HEADSET		= 2,	//!<'  Mono headset
	FTM_AUDIO_PATH_STEREO_HEADSET	= 3,	//!<'  Stereo headset
	FTM_AUDIO_PATH_ANALOG_HANDSFREE	= 4,	//!<'  Analog handsfree kit
	FTM_AUDIO_PATH_STEREO_DAC		= 5,	//!<'  Stereo DAC
	FTM_AUDIO_PATH_SPEAKER_PHONE	= 6,	//!<'  Speaker phone
	FTM_AUDIO_PATH_TTY_HANDSFREE	= 7,	//!<'  TTY Handsfree kit
	FTM_AUDIO_PATH_TTY_HEADSET		= 8,	//!<'  TTY headset
	FTM_AUDIO_PATH_TTY_VCO			= 9,	//!<'  TTY VCO
	FTM_AUDIO_PATH_TTY_HCO			= 10,	//!<'  TTY HCO
	FTM_AUDIO_PATH_BT_INTERCOM		= 11,	//!<'  Bluetooth intercom
	FTM_AUDIO_PATH_BT_HEADSET		= 12,	//!<'  Bluetooth headset
	FTM_AUDIO_PATH_BT_AUDIO			= 13,	//!<'  Bluetooth local audio
	FTM_AUDIO_PATH_USB_AUDIO		= 14,	//!<'  USB audio
	FTM_AUDIO_PATH_FM_MONO_HS		= 15,	//!<'  FM Mono headset
	FTM_AUDIO_PATH_FM_STEREO_HS		= 16	//!<'  FM Stereo headset
} FTM_Audio_Device_Id_Enum;


typedef enum
{		
	FTM_AUDIO_SND_METHOD_VOICE = 0,			//!<'  Use the device's voice generator
	FTM_AUDIO_SND_METHOD_KEY_BEEP = 1,		//!<'  Use the device's keybeep generator              
	FTM_AUDIO_SND_METHOD_MESSAGE = 2,		//!<'  Use the path's ringer, or voice generator       
	FTM_AUDIO_SND_METHOD_RING = 3			//!<'  Use the device's ring generator                 
} FTM_Audio_Sound_Methods_Enum;

/******************************************************************************
						FTM - Camera
*******************************************************************************/

/**
	\brief Enumeration of Camera FTM Sub-command ID's
*/
typedef enum 
{
	_FTM_CAMERA_START				= 0,		//!<' Starts camera services
	_FTM_CAMERA_STOP				= 1,		//!<' Stops camera services
	_FTM_CAMERA_SET_PARM			= 2,		//!<' Sets operational parameters for camera sensor 
	_FTM_CAMERA_SET_DIMENSIONS		= 3,		//!<' Sets camera dimensions
	_FTM_CAMERA_START_PREVIEW		= 4,		//!<' Enters preview state 
	_FTM_CAMERA_STOP_PREVIEW		= 5,		//!<' Exits preview state
	_FTM_CAMERA_TAKE_PICTURE_AND_ENCODE	= 6,	//!<' Takes a picture and encode it
	_FTM_CAMERA_GET_PICTURE			= 7,		//!<' Gets raw or encoded data from phone
	_FTM_CAMERA_GET_PARM			= 8,		//!<' Gets operational parameters for camera sensor 
	_FTM_CAMERA_GET_INFO			= 9,		//!<' Reads the camera information
	_FTM_CAMERA_SET_SENSOR_ID		= 10		//!<' Select the camera sensor
} FTM_Camera_SubCmd_Id_Enum;


/**
	\brief Enumeration of Camera Status, which is returned from calling camera services API.
	See also: (80-V5310-1) Camera Services Interface Specification and Operational Description
*/
typedef enum 
{
	CAMERA_SUCCESS,
	CAMERA_INVALID_STATE,
	CAMERA_INVALID_PARM,
	CAMERA_INVALID_FORMAT,
	CAMERA_NO_SENSOR,
	CAMERA_NO_MEMORY,
	CAMERA_NOT_SUPPORTED,
	CAMERA_FAILED,
	CAMERA_INVALID_STAND_ALONE_FORMAT,
	CAMERA_MALLOC_FAILED_STAND_ALONE,
} FTM_Camera_Status_Enum;

/**
	\brief Enumeration of Camera Status, for use with the FTM_CAMERA_SET_PARM command.
	See also: (80-V5310-1) Camera Services Interface Specification and Operational Description, Table 2-4.
*/
typedef enum 
{
	CAMERA_PARM_STATE,
	CAMERA_PARM_ACTIVE_CMD,
	CAMERA_PARM_ZOOM,
	CAMERA_PARM_ENCODE_ROTATION,
	CAMERA_PARM_SENSOR_POSITION,
	CAMERA_PARM_CONTRAST,
	CAMERA_PARM_BRIGHTNESS,
	CAMERA_PARM_SHARPNESS,
	CAMERA_PARM_EXPOSURE,
	CAMERA_PARM_WB,
	CAMERA_PARM_EFFECT,
	CAMERA_PARM_AUDIO_FMT,
	CAMERA_PARM_FPS,
	CAMERA_PARM_FLASH,
	CAMERA_PARM_RED_EYE_REDUCTION,
	CAMERA_PARM_NIGHTSHOT_MODE,
	CAMERA_PARM_REFLECT,
	CAMERA_PARM_PREVIEW_MODE,
	CAMERA_PARM_ANTIBANDING,
	CAMERA_PARM_THUMBNAIL_WIDTH,
	CAMERA_PARM_THUMBNAIL_HEIGHT,
	CAMERA_PARM_THUMBNAIL_QUALITY
} FTM_Camera_Param_Enum;


/**
	\brief Enumeration of Camera Preview settings for use with the FTM_CAMERA_SET_PARM command.
	See also: (80-V5310-1) Camera Services Interface Specification and Operational Description, Table 2-4.
*/
typedef enum 
{
	CAMERA_PREVIEW_MODE_SNAPSHOT,
	CAMERA_PREVIEW_MODE_MOVIE
} FTM_Camera_Param_Preview_Enum;

/**
	\brief Enumeration of Camera orientations
*/
typedef enum 
{
	CAMERA_ORIENTATION_LANDSCAPE,
	CAMERA_ORIENTATION_PORTRAIT
} FTM_Camera_Orientation_Enum;


/**
	\brief Enumeration of Camera orientations
*/
typedef enum 
{
	CAMERA_RAW,
	CAMERA_JPEG
} FTM_Camera_Format_Enum;

/******************************************************************************
						FTM - Log
*******************************************************************************/


/**
	\brief Enumeration of FTM Logging commands.
*/
typedef enum 
{
	_FTM_LOG_ENABLE		= 0,
	_FTM_LOG_DISABLE	= 1
} FTM_Logging_SubCmd_Id_Enum;


/*===========================================================================*/
/**
	enum log_FTM2_id_enum

	\brief Log ID's for the FTM2 log message
*/
/*===========================================================================*/
typedef enum 
{
  LOG_FTM2_LOG_PRINTF		= 0x00,		//<!' FTM printf
  LOG_FTM2_LOG_BT			= 0x01,		//<!' FTM Bluetooth
  LOG_FTM2_LOG_DACC_ACCUM	= 0x02,		//<!' DACC accumulator 
  LOG_FTM2_LOG_DACC_ACCUM_C0= 0x03,		//<!' DACC accumulator, antenna 2
  LOG_FTM2_LOG_1X_AGC		= 0x4,		//<!' 1X Primary AGC
  LOG_FTM2_LOG_HDR_AGC		= 0x5,		//<!' EVDO Primary AGC
  LOG_FTM2_LOG_1X_AGC_C1	= 0x6,		//<!' 1X Secondary AGC
  LOG_FTM2_LOG_HDR_AGC_C1	= 0x7,		//<!' EVDO Secondary AGC
  LOG_FTM2_LOG_IM2_DFT		= 0x8,
  LOG_FTM2_LOG_FFT			= 0x9,
  LOG_FTM2_GSM_AUTOCAL		= 0xA,		//<!' FTM GSM Auto Calibration
  LOG_FTM2_GSM_THERM_VBATT	= 0xB,		//<!' FTM GSM Therm & Batt
  LOG_FTM2_INTELLICEIVER	= 0xB,		//<!' FTM CDMA Intelliceiver
  LOG_FTM2_LOG_TX_RX_SWEEP	= 0x0B,		//<!' results from FTM_LOG_TX_RX_SWEEP
  LOG_FTM2_LOG_HDET_TRACKING   = 0x10,


  LOG_FTM2_LOG_WCDMA_AGC	= 0x1004,	//<!' FTM WCDMA AGC
  LOG_FTM2_LOG_WCDMA_BER	= 0x1006,	//<!' FTM WCDMA BER
  LOG_FTM2_LOG_GSM_BER		= 0x1005	//<!' FTM GSM BER


} log_FTM2_id_enum;

//! Size of the header information on an FTM2 log packet, including the standard log header+FTM2 log id fields
#define LOG_FTM2_HEADER_SIZE 14

/*===========================================================================*/
/**
	Structure of the general FTM2LogMessage, 80-V9147-1_A - FTM Logging API
*/
/*===========================================================================*/

typedef struct 
{
	// Portion unique to FTM logs
	word iFTM_LogID;	//!<' Log ID
	byte  Data[DIAG_MAX_PACKET_SIZE];		//!<' Pointer to the data

} FTM2LogMessage;

/*===========================================================================*/
/**
	Structure of the WCDMA FTM2LogMessage, 80-V9147-1_A - FTM Logging API
*/
/*===========================================================================*/

typedef struct 
{	
	// Portion unique to FTM logs
	word iFTM_LogID;	//!<' Log ID
	short iRX_AGC;		
	short iTX_AGC;		
	short iTx_AGC_ADJ;	
	byte  iLNA_STATE;
	byte  iPA_STATE;
	byte  iHDET;
	word iThemistor;
	byte  iScaledThermistor;
	byte  iTempCompIndex;
	byte  iTempCompRemainder;

} FTM2LogMessage_WCDMA_AGC;




/**
	Indicator that a default timeout should be used
*/
#define FTM_NONSIG_DEFAULT_TIMEOUT  0



/******************************************************************************
						FTM - WCDMA BER
*******************************************************************************/

/**
	\brief Enumeration of FTM WCDMA BER commands.
*/
typedef enum 
{
	_FTM_WCDMA_START_MODE_REQ		= 0,
	_FTM_WCDMA_STOP_MODE_REQ		= 1,
	_FTM_WCDMA_START_IDLE_MODE_REQ	= 2,
	_FTM_WCDMA_ACQUIRE_REQ			= 3,
	_FTM_WCDMA_RMC_DCH_SETUP_REQ	= 4,
	_FTM_WCDMA_RMC_DCH_SETUP_REQ_V2	= 5,
	_FTM_WCDMA_BER_RMC_DCH_TFCS_CONFIG_REQ	 = 6
} FTM_WCDMA_BER__SubCmd_Id_Enum;

/**
	Structure of response packet for certian non-sigaling commands
		CMD_CODE	- Message ID. 		DM sets CMD_CODE to 75
		SUB_SYS_ID	- FTM ID is 11.		Mode ID Mode ID for FTM WCDMA
		Reserved	- 0
		rsp_id		- 0x1002 = FTM_WCDMA_BER
		rsp_cid		- Command ID that generated. 
		rsp_scid	- Subcommand ID that generated
		Status		- 0 = DIAG_FTM_STATUS_SUCCESS. 1 = DIAG_FTM_STATUS_SUCCESS
*/
typedef struct
{
	byte  iCMD_Code;
	byte  iSubSysId;
	byte  iModeID;
	byte  iReserved; 
	word iRspId;
	word iRspCid;
	word iRspSCid;
	byte  iStatus;

} FTM_BER_Response_Struct;



/*===========================================================================*/
/**
	enum log_FTM2_WCDMA_BER_log_id_enum, 80-V9698-1_A

	\brief Log ID's for the WCDMA BER FTM2 log message
*/
/*===========================================================================*/
typedef enum 
{
  LOG_FTM2_WCDMA_START_MODE_CNF			= 0,		//<!' FTM WCDMA BER start WCDMA mode confirmation
  LOG_FTM2_WCDMA_STOP_MODE_CNF			= 1,		//<!' FTM WCDMA BER stop WCDMA mode confirmation
  LOG_FTM2_WCDMA_START_IDLE_MODE_REQ	= 2,		//<!' FTM WCDMA BER start idle mode confirmation
  LOG_FTM2_WCDMA_ACQUIRE_CNF			= 3,		//<!' FTM WCDMA BER acquire confirmation
  LOG_FTM2_WCDMA_CPHY_SETUP_CNF			= 4,		//<!' FTM WCDMA BER CPHY setup confirmation
  LOG_FTM2_WCDMA_PHYCHAN_ESTABLISHED_IND= 5,		//<!' FTM WCDMA BER physical channel established confirmation
  LOG_FTM2_WCDMA_CPHY_ERROR_IND			= 6,		//<!' FTM WCDMA BER pyhsical channel error
  LOG_FTM2_WCDMA_RL_FAILURE_IND			= 7,		//<!' FTM WCDMA BER Reverselink error
  LOG_FTM2_WCDMA_BER_Max
} log_FTM2_WCDMA_BER_log_id_enum;


/*===========================================================================*/
/**
	enum for WCDMA non-signaling Acquisition type
*/
/*===========================================================================*/
typedef enum
{
	FTM_WCDMA_BER_AcqTypeFreqOnly		= 0,	//<!' Frequency valid only
	FTM_WCDMA_BER_AcqTypeFreqAndScrCode	= 1,	//<!' Frequency and scrambling code valid only
	FTM_WCDMA_BER_AcqTypeFreqScrCodePN	= 2		//<!' Frequency, scrambling code, and PN position valid
} FTM_WCDMA_BER_AcqType_Enum;


/*===========================================================================*/
/**
	enum for WCDMA non-signaling Acquisition mode
*/
/*===========================================================================*/
typedef enum
{
	FTM_WCDMA_BER_AcqModeMicro	= 0,	//<!' Micro acquisition (not currently supported)
	FTM_WCDMA_BER_AcqModeFull	= 1		//<!' 1 = Full acquisition freq ARFCN to attempt acquisition. Required for acq_type = 0, 1, or 2.

} FTM_WCDMA_BER_AcqMode_Enum;

/*===========================================================================*/
/**
	enum for WCDMA non-signaling RMC type
*/
/*===========================================================================*/
typedef enum
{
	FTM_WCDMA_BER_RMC_Type12_2kpbs				= 0,	//<!' RMC 12.2 kbps channel, 
	FTM_WCDMA_BER_RMC_Type64kbps				= 1,	//<!' RMC 64 kbps channel, 
	FTM_WCDMA_BER_RMC_Type384kpbs				= 2,	//<!' RMC 384 kbps channel, (Not currently supported)
	FTM_WCDMA_BER_RMC_Type12_2kpbs_Symmetric	= 3,	//<!' RMC 12.2 kbps channel, block size in the DL and UL is the same
	FTM_WCDMA_BER_RMC_Type64kbps_Symmetric		= 4,	//<!' RMC 64 kbps channel, block size in the DL and UL is the same
    FTM_WCDMA_BER_RMC_Type384kbps_Symmetric		= 5		//<!' RMC 384 kbps channel, block size in the DL and UL is the same
														
	// Note: For first 3 data rate enumeration, UL CRC bits are looped back from DL
	// Note: For last  3 data rate enumeration, UL CRC is valid and computed based on Rx data bits
} FTM_WCDMA_BER_RMC_Type_Enum;

/*===========================================================================*/
/**
	enum for WCDMA non-signaling Power Algorithm types
*/
/*===========================================================================*/
typedef enum
{
	FTM_WCDMA_BER_PCA_Type_Alg1	= 0,	//<!' Power control algorithm 1
	FTM_WCDMA_BER_PCA_Type_Alg2	= 1		//<!' Power control algorithm 2
} FTM_WCDMA_BER_PCA_Type_Enum;

/*===========================================================================*/
/**
	enum for WCDMA non-signaling Power Algorithm types
*/
/*===========================================================================*/
typedef enum
{
	FTM_WCDMA_BER_PCA_Size_1dB	= 0,	//<!' 1dB for power control algorithm
	FTM_WCDMA_BER_PCA_Size_2dB	= 1		//<!' 2dB for power control algorithm

} FTM_WCDMA_BER_PCA_Size_Enum;


/*===========================================================================*/
/**
	enum for WCDMA non-signaling Power Algorithm types
*/
/*===========================================================================*/
typedef enum
{ 
    FTM_WCDMA_BER_UL_TFCS_CONFIG = 0,
    FTM_WCDMA_BER_DL_TFCS_CONFIG = 1

} FTM_WCDMA_BER_TFCS_ConfigType;

/**
	Structure to store the state of WCDMA BER.  This structure is used with the functions:
	  QLIB_FTM_WCDMA_BER_ClearStatus()
	  QLIB_FTM_WCDMA_BER_GetStatus()
*/
typedef struct
{
	/**
		Status array, indexed by log_FTM2_WCDMA_BER_log_id_enum

		Will be set to true if a log message was received for that log type
	*/
	byte bHasUpdated[ LOG_FTM2_WCDMA_BER_Max ];

	//
	// Related to ACQUIRE_CNF
	//
		/**
			ACQUIRE_CNF.Status = 0 = Acquisition failed, frequency and scr_code data not valid, 1 = Acquisition successful
		*/
		byte iACQUIRE_CNF_status;

		//! ACQUIRE_CNF.Frequency = Frequency where CPICH was acquired
		dword iACQUIRE_CNF_frequency;	

		//! ACQUIRE_CNF.Scr_Code = Primary CPICH scrambling code identified during acquisition
		dword iACQUIRE_CNF_scr_code;	

	//
	// Related to CPHY_SETUP_CNF
	//
		/**
			CPHY_SETUP_CNF.status
				0 = CPHY_SETUP request failed
				1 = CPHY_SETUP request successful.
		*/
		byte iCPHY_SETUP_status;

	//
	// Related to PHYCHAN_ESTABLISHED_IND
	//
		/**
			PHYCHAN_ESTABLISHED_IND.status
				0 = Physical channel could not be established.
				1 = Physical channel was successfully established.
		*/
		byte iPHYCHAN_ESTABLISHED_IND_status;

	//
	// WCDMA AGC message
	//
		//! Updated flag for WCDMA AGC
		byte bWCDMA_AGChasUpdated;

		//! Storage for WCDMA AGC
		FTM2LogMessage_WCDMA_AGC oWCDMA_AGC;

}  WCDMA_BER_State;


/*===========================================================================*/
/**
	enum for WCDMA non-signaling N312 parameters
*/
/*===========================================================================*/
typedef enum
{
    FTM_WCDMA_BER_N312_s1    = 0,
    FTM_WCDMA_BER_N312_s50   = 1,
    FTM_WCDMA_BER_N312_s100  = 2,
    FTM_WCDMA_BER_N312_s200  = 3,
    FTM_WCDMA_BER_N312_s400  = 4,
    FTM_WCDMA_BER_N312_s600  = 5,
    FTM_WCDMA_BER_N312_s800  = 6,
    FTM_WCDMA_BER_N312_s1000 = 7,
  
} ftm_wcdma_ber_n312_type;


/*===========================================================================*/
/**
	enum for WCDMA non-signaling N313 parameters
*/
/*===========================================================================*/
typedef enum
{
    
    FTM_WCDMA_BER_N313_s1   = 0,
    FTM_WCDMA_BER_N313_s2   = 1,
    FTM_WCDMA_BER_N313_s4   = 2,
    FTM_WCDMA_BER_N313_s10  = 3,
    FTM_WCDMA_BER_N313_s20  = 4,
    FTM_WCDMA_BER_N313_s50  = 5,
    FTM_WCDMA_BER_N313_s100 = 6,
    FTM_WCDMA_BER_N313_s200 = 7

} ftm_wcdma_ber_n313_type;

/*===========================================================================*/
/**
	enum for WCDMA non-signaling N315 parameters
*/
/*===========================================================================*/
typedef enum
{
    
    FTM_WCDMA_BER_N315_s1   = 0,
    FTM_WCDMA_BER_N315_s50  = 1,
    FTM_WCDMA_BER_N315_s100 = 2,
    FTM_WCDMA_BER_N315_s200 = 3,
    FTM_WCDMA_BER_N315_s400 = 4,
    FTM_WCDMA_BER_N315_s600 = 5,
    FTM_WCDMA_BER_N315_s800 = 6,
    FTM_WCDMA_BER_N315_s1000 = 7

} ftm_wcdma_ber_n315_type;


/******************************************************************************
						FTM - HSDPA BLER
*******************************************************************************/

/** This enumeration specifies the pre-configured HSET list, to be used with QLIB_FTM_HSDPA_BLER_Configure_HS_DSCH */
typedef enum 
{
	HSDPA_HSET_1_QPSK = 1,		//!< ' HSET 1, QPSK
	HSDPA_HSET_1_16QAM,			//!< ' HSET1, 16QAM
	HSDPA_HSET_2_QPSK,			//!< ' HSET2, QPSK
	HSDPA_HSET_2_16QAM,			//!< ' HSET2, 16QAM
	HSDPA_HSET_3_QPSK,			//!< ' HSET3, QPSK
	HSDPA_HSET_3_16QAM,			//!< ' HSET3, 16QAM
	HSDPA_HSET_4_QPSK,			//!< ' HSET4, QPST
	HSDPA_HSET_5_QPSK,			//!< ' HSET5, QPST
	HSDPA_HSET_6_QPSK = 0xFF	//!< ' HSET6, QPST
} ftm_hsdpa_ber_hset_type;

/** This type specifies the possible asynchronous event packet types */
typedef enum
{

    FTM_LOG_HSDPA_HS_CHANNEL_START_IND	= 0x40,	//!< ' HSDPA Start completion indicator
    FTM_LOG_HSDPA_HS_CHANNEL_STOP_IND	= 0x41,	//!< ' HSDPA Stop completion indicator
    FTM_LOG_HSDPA_HS_CHANNEL_RECFG_IND	= 0x42,	//!< ' HSDPA Reconfigure completion indicator
	

	// Identifiers to help with the size of the event list
	FTM_LOG_HSDPA_EVENT_Max,										//!< ' Maximum ID + 1
	FTM_LOG_HSDPA_EVENT_First = FTM_LOG_HSDPA_HS_CHANNEL_START_IND,	//!< ' First valid ID
	FTM_LOG_HSDPA_EVENT_Last = FTM_LOG_HSDPA_EVENT_Max - 1,			//!< ' Last valid ID
	FTM_LOG_HSDPA_EVENT_Count = FTM_LOG_HSDPA_EVENT_Max -
								FTM_LOG_HSDPA_EVENT_First			//!< ' Size of valid list

} log_FTM2_HSDPA_BER_log_id_enum;


/**
	Enumeration of HSDPA channel states
*/
typedef enum
{
    FTM_HSDPA_BLER_HS_CHAN_STATE_INACTIVE,
    FTM_HSDPA_BLER_HS_CHAN_STATE_ACTIVE

} ftm_hsdpa_bler_hs_chan_state_type;

/**
	Enumeration of HSDPA force-stop error states
*/
typedef enum
{
    FTM_HSDPA_BLER_HS_FORCE_STOP__NO_ERROR = 0,
    FTM_HSDPA_BLER_HS_FORCE_STOP__ERROR_OP_IN_PROGRESS,
    FTM_HSDPA_BLER_HS_FORCE_STOP__ERROR_HS_INACTIVE

} ftm_hsdpa_bler_hs_force_stop_error_type;



/**
	Structure to store the state of HSDPA BER.  This structure is used with the functions:
	  QLIB_FTM_HSDPA_BER_ClearStatus()
	  QLIB_FTM_HSDPA_BER_GetStatus()
*/
typedef struct
{
	/**
		Status array, indexed by log_FTM2_WCDMA_BER_log_id_enum

		Will be set to true if a log message was received for that log type
	*/
	byte bHasUpdated[ FTM_LOG_HSDPA_EVENT_Count ];

	//
	// Related to FTM_LOG_HSDPA_HS_CHANNEL_START_IND
	//
		/**
			CHANNEL_START_IND.status = 0 = Start operation failed, 1 = Start operation success
		*/
		byte iCHANNEL_START_IND_status;

	//
	// Related to FTM_LOG_HSDPA_HS_CHANNEL_STOP_IND
	//
		/**
			CHANNEL_STOP_IND.status = 0 = Stop operation failed, 1 = Stop operation success
		*/
		byte iCHANNEL_STOP_IND_status;

	//
	// Related to FTM_LOG_HSDPA_HS_CHANNEL_RECFG_IND
	//
		/**
			CHANNEL_RECFG_IND.ReconfigureStatus = 0 = Reconfigure operation failed, 1 = Reconfigure operation success
		*/
		byte iCHANNEL_RECFG_IND_ReconfigureStatus;


}  HSDPA_BER_State;


/******************************************************************************
						FTM - GSM BER
*******************************************************************************/

/**
	\brief Enumeration of FTM GSM BER commands.
*/
typedef enum 
{
	_FTM_GSM_START_MODE_REQ				= 0,
	_FTM_GSM_SELECT_SPECIFIC_BCCH_REQ	= 1,
	_FTM_GSM_START_IDLE_MODE_REQ		= 2,
	_FTM_GSM_CONFIG_LOOPBACK_TYPE_REQ	= 3,
	_FTM_GSM_CHANNEL_ASSIGN_REQ			= 4,
	_FTM_GSM_CHANNEL_RELEASE_REQ		= 5,
	_FTM_GSM_STOP_GSM_MODE_REQ			= 6,
	_FTM_GSM_BER_CHANNEL_ASSIGN_V2_REQ	= 7
} FTM_GSM_BER_SubCmd_Id_Enum;


/**
	\brief Enumerations for GSM Band, for GSM BER tests
*/
typedef enum
{
	FTM_GSM_BER_PGSM_900,
	FTM_GSM_BER_EGSM_900,
	FTM_GSM_BER_PCS_1900,
	FTM_GSM_BER_DCS_1800,
	FTM_GSM_BER_CELL_850
} FTM_GSM_BER_Band_Enum;


/**
	\brief Enumerations for GSM loopback types
*/
typedef enum
{
	FTM_GSM_BER_Disable_Loopback,
	FTM_GSM_BER_Loopback_Type_A,
	FTM_GSM_BER_Loopback_Type_B,
	FTM_GSM_BER_Loopback_Type_C
} FTM_GSM_BER_LoopbackType_Enum;

/**
	\brief Enumerations for channel modes
*/
typedef enum
{
	FTM_GSM_BER_SPEECH_FULL_RATE,
	FTM_GSM_BER_SPEECH_HALF_RATE,
	FTM_GSM_BER_SPEECH_ENHANCED_FULL_RATE,
	FTM_GSM_BER_AMR_FULL_RATE,
	FTM_GSM_BER_AMR_HALF_RATE,
	FTM_GSM_BER_DATA_14_4_KBPS_FULL_RATE,
    FTM_GSM_BER_DATA_9_6_KBPS_FULL_RATE
} FTM_GSM_BER_Channel_Modes_Enum;


/*===========================================================================*/
/**
	enum log_FTM2_GSM_BER_log_id_enum, 80-V3951-1

	\brief Log ID's for the GSM BER FTM2 log message
*/
/*===========================================================================*/
typedef enum 
{
  LOG_FTM2_START_GSM_MODE_CNF			= 0,	//<!' Start GSM confirm
  LOG_FTM2_SELECT_SPECIFIC_BCCH_CNF		= 1,	//<!' FTM GSM BER select specific BCCH confirmation
  LOG_FTM2_START_IDLE_MODE_CNF			= 2,	//<!' FTM GSM BER start idle mode confirmation
  LOG_FTM2_CHANNEL_ASSIGN_CNF			= 3,	//<!' FTM GSM BER channel assign confirmation
  LOG_FTM2_CHANNEL_RELEASE_CNF			= 4,	//<!' FTM GSM BER channel release confirmation
  LOG_FTM2_STOP_GSM_MODE_CNF			= 5,	//<!' FTM GSM BER stop GSM mode confirmation
  LOG_FTM2_PH_DATA_IND					= 6,	//<!' FTM GSM BER Ph data indication
  LOG_FTM2_FTM_LOG_GSM_BER_DED_RPT		= 7,	//<!' FTM GSM BER dedicated measurement report

  LOG_FTM2_FTM_LOG_GSM_MAX					//<!' Size of list, not a valid log ID
} log_FTM2_GSM_BER_log_id_enum;


/*===========================================================================*/
/**
	enum log_FTM2_GSM_BER_PH_Channel_Type_enum, 80-V3951-1

	\brief Channel types for the PH_DATA_IND message
*/
/*===========================================================================*/
typedef enum 
{
	LOG_FTM2_GSM_PH_IND_DCCH	= 0,	//<!' DCCH
	LOG_FTM2_GSM_PH_IND_BCCH	= 1,	//<!' BCCH
	LOG_FTM2_GSM_PH_IND_RACH	= 2,	//<!' RACH
	LOG_FTM2_GSM_PH_IND_CCCH	= 3,	//<!' CCCH
	LOG_FTM2_GSM_PH_IND_SACCH	= 4,	//<!' SACCH
	LOG_FTM2_GSM_PH_IND_SDCCH	= 5,	//<!' SDCCH
	LOG_FTM2_GSM_PH_IND_FACCH_F	= 6,	//<!' FACCH_F
	LOG_FTM2_GSM_PH_IND_FACCH_H	= 7,	//<!' FACCH_H

	LOG_FTM2_FTM_PH_IND_Chan_Max	//<!' Size of list, not a valid channel ID
} log_FTM2_GSM_BER_PH_Channel_Type_enum;

/**
	Structure to store the state of GSM BER.  This structure is used with the functions:
	  QLIB_FTM_GSM_BER_ClearStatus()
	  QLIB_FTM_GSM_BER_GetStatus()
*/
typedef struct
{
	/**
		Status array, indexed by log_FTM2_GSM_BER_log_id_enum

		Will be set to true if a log message was received for that log type
	*/
	byte bHasUpdated[ LOG_FTM2_FTM_LOG_GSM_MAX ];

	//
	// Related to SELECT_SPECIFIC_BCCH_CNF
	//
		/** 
			This confirmation indicates that the select specific BCCH request has completed, and reports
			whether the FCCH/SCH channels were detected, as well as the SCH data.

			0 = FCCH/SCH not found. Acquisition failed. 1 = FCCH/SCH acquired successfully.
		*/
		byte bSCCH_Found;

		//! Data decoded from SCH channel. Data is only valid if ‘bSCCH_Found’ is 1.
		dword iSchData;	

	//
	// Related to CHANNEL_ASSIGN_CNF
	//
		//! 0 = Success. >0 = Error occurred.
		byte iCHANNEL_ASSIGN_CNF_status;

	//
	// Related to PH_DATA_IND
	//
		/**
			The last channel type message:

			0 = DCCH
			1 = BCCH
			2 = RACH
			3 = CCCH
			4 = SACCH
			5 = SDCCH
			6 = FACCH_F
			7 = FACCH_H

			Not all of these channel_types are used during the BER test.
		*/
		byte iPH_ChannelType;

		/**
			The last status:

			0 = Data block for the associated channel_type has not passed the
			crc_check
			1 = Data block for the associated channel_type has passed the
			crc_check
		*/
		byte iPH_CRC_Pass;

		/**
			Array of channels, indicating whether a certain channel status has been received
			since the last time that status was cleared.
		*/
		byte abHasPH_DATA_ReceivedChannelStatus[ LOG_FTM2_FTM_PH_IND_Chan_Max ] ;
		
		/**
			Array of channels, indicating channel status.  Only valid if the cooresponding 
			abHasRecievedChannelStatus[] array index is set to true, to indicate that a status
			message for this channel has been received since the last time that status was cleared.
		*/
		byte abPH_DATA_LastChannelStatus[ LOG_FTM2_FTM_PH_IND_Chan_Max ] ;


	//
	// Related to GSM_BER_DED_RPT
	//
		//! RxLev_Full as reported by Layer 1
		byte iDED_RPtRxLev_Full;

		//! RxQual_Full as reported by Layer 1
		byte iDED_RPtRxQual_Full;


	//
	// Related to call status, SACCH accumulator which indicates
	//
		//! Starting level for the SACCH counter, will be set to value of 40, but user can override
		short iGSM_BER_SACCH_Accumulator_Max;

		//! Current value of the SACCH counter, decremented 1 for a failed SACCH, incremented 1 for a good SACCH
		//! saturated at 0 on low end and iGSM_BER_SACCH_Count_max on the high end
		short iGSM_BER_SACCH_Accumulator_Value;

		//! Total number of updates that have been made to iGSM_BER_SACCH_Accumulator_Value.
		//! if the value is zero, then no updates have been made so status cannot be determined
		short iGSM_BER_SACCH_Accumulator_TotalUpdates;

}  GSM_BER_State;

/**
	Enumeration to control whether a non-signaling session should be initialized
*/
typedef enum
{
	FTM_GSM_BER_SLOT_0  = 0,		//!<' Slot 0
	FTM_GSM_BER_SLOT_1  = 1,		//!<' Slot 1
	FTM_GSM_BER_SLOT_2  = 2,		//!<' Slot 2
	FTM_GSM_BER_SLOT_3  = 3,		//!<' Slot 3
	FTM_GSM_BER_SLOT_4  = 4,		//!<' Slot 4
	FTM_GSM_BER_SLOT_5  = 5,		//!<' Slot 5
	FTM_GSM_BER_SLOT_6  = 6,		//!<' Slot 6
	FTM_GSM_BER_SLOT_7  = 7			//!<' Slot 7

} FTM_GSM_BER_Slot_Enum;

/******************************************************************************
						FTM - EGPRS BLER
*******************************************************************************/

/**
	Enumeration of EGPRS BER log events
*/
typedef enum
{

    FTM_LOG_EGPRS_BER_TBF_IND           = 0x40,
    FTM_LOG_EGPRS_BER_MAC_PH_DATA_IND,
    FTM_LOG_EGPRS_BER_SERVING_MEAS_IND,

	// Identifiers to help with the size of the event list
	FTM_LOG_EGPRS_EVENT_Max,											//!< ' Maximum ID + 1
	FTM_LOG_EGPRS_EVENT_First	= FTM_LOG_EGPRS_BER_TBF_IND,			//!< ' First valid ID
	FTM_LOG_EGPRS_EVENT_Last	= FTM_LOG_EGPRS_EVENT_Max - 1,			//!< ' Last valid ID
	FTM_LOG_EGPRS_EVENT_Count	= FTM_LOG_EGPRS_EVENT_Max -
									FTM_LOG_EGPRS_EVENT_First			//!< ' Size of valid list

} log_FTM2_EGPRS_BER_log_id_enum;


/**
	This enumeration specifies the types of TBFs that we generate events for 
*/

typedef enum
{
    FTM_LOG_EGPRS_BER_TBF_DL,	//!<' 0 = DL TBF
    FTM_LOG_EGPRS_BER_TBF_UL	//!<' 1 = UL TBF

} ftm_log_egprs_tbf_enum;

/**
	This enumeration specifies the sub-event types for the FTM_LOG_EGPRS_BER_TBF_IND EVENT
*/
typedef enum
{
    
    FTM_LOG_EGPRS_TBF_EVENT_ESTABLISHED,	//!<' TBF Established. TBF type indicated in 
											//!<' tbf_type field has been successfully established.

    FTM_LOG_EGPRS_TBF_EVENT_RELEASED,		//!<' TBF Released. TBF type indicated in tbf_type field has been
											//!<' successfully released.

    FTM_LOG_EGPRS_TBF_EVENT_ERROR			//!<' TBF Error. An error has occurred related to the TBF indicated in
											//!<' the tbf_type field. This may require a reconfiguration of the TBF
											//!<' before proceeding.

} ftm_log_egprs_tbf_event_type;


/**
	Structure to store the state of EGPRS BER.  This structure is used with the functions:
	  QLIB_FTM_EGPRS_BER_ClearStatus()
	  QLIB_FTM_EGPRS_BER_GetStatus()
*/
typedef struct
{
	/*

	Events represented:
		FTM_LOG_EGPRS_BER_TBF_IND
		FTM_LOG_EGPRS_BER_MAC_PH_DATA_IND,
		FTM_LOG_EGPRS_BER_SERVING_MEAS_IND,
	*/


	/**
		Status array, indexed by log_FTM2_EGPRS_BER_log_id_enum

		Will be set to true if a log message was received for that log type
	*/
	byte bHasUpdated[ FTM_LOG_EGPRS_EVENT_Count ];

	//
	// Related to FTM_LOG_EGPRS_BER_TBF_IND
	//
		/**
			Type of event the event is for:
			\code
               0 = DL TBF
               1 = UL TBF
			\endcode

			Type can be cast as: ftm_log_egprs_tbf_type

		*/
			byte bTBF_IND_type;

		/** 
			This indication informs the user of an event related to either the DL or UL TBF.

			\code
              0 = TBF Established. TBF type indicated in tbf_type field has been
              successfully established.

              1 = TBF Released. TBF type indicated in tbf_type field has been
              successfully released.

              2 = TBF Error. An error has occurred related to the TBF indicated in
              the tbf_type field. This may require a reconfiguration of the TBF
              before proceeding.
			\endcode

			Type can be cast as: ftm_log_egprs_tbf_event_type
	*/
		byte eTBF_IND_UL_event;

	//! DL status, same as the UL values
		byte eTBF_IND_DL_event;


	//
	// Related to FTM_LOG_EGPRS_BER_SERVING_MEAS_IND
	//
		//! RxLev_average as reported by Layer 1. Range should be 0-63.
		byte iSERVING_MEAS_IND_RxLev_average;

}  EGPRS_BER_State;


/******************************************************************************
						FTM - Common RF
*******************************************************************************/

/**
	This enumeration specifies the RX Action list for the FTM_TX_RX_FREQ_CAL_SWEEP function
*/
typedef enum
{
	FTM_TX_RX_FREQ_RX_ACTION_LNA0	= 0,	//!<' - calibrate LNA offset 0
	FTM_TX_RX_FREQ_RX_ACTION_LNA1	= 1,	//!<' - calibrate LNA offset 1
	FTM_TX_RX_FREQ_RX_ACTION_LNA2	= 2,	//!<' - calibrate LNA offset 2
	FTM_TX_RX_FREQ_RX_ACTION_LNA3	= 3,	//!<' - calibrate LNA offset 3
	FTM_TX_RX_FREQ_RX_ACTION_LNA4	= 4,	//!<' - calibrate LNA offset 4
	FTM_TX_RX_FREQ_RX_ACTION_DVGA	= 100,	//!<' - calibrate DVGA offset
	FTM_TX_RX_FREQ_RX_ACTION_NOTHING = 255	//!<' - do nothing

} ftm_tx_rx_freq_cal_sweep_rx_action_enum;

/**
	This enumeration specifies RX chain list for the FTM_TX_RX_FREQ_CAL_SWEEP function
*/
typedef enum
{
	FTM_TX_RX_FREQ_RX_CHAIN_0		= 0,	//!<' - Primary RX
	FTM_TX_RX_FREQ_RX_CHAIN_1		= 1		//!<' - Secondary RX

} ftm_tx_rx_freq_cal_sweep_rx_chain_enum;


/**
	List specifying the rx power mode to use for each step of a given frequency. 
	The values are ignored for chipsets that do not support rx power modes. 
*/
typedef enum
{
	FTM_TX_RX_FREQ_RX_PM_High		= 0,	//!<' - Rx high power mode
	FTM_TX_RX_FREQ_RX_PM_Medium		= 1,	//!<' - Rx medium power mode
	FTM_TX_RX_FREQ_RX_PM_Low		= 2		//!<' - Rx low power mode

} ftm_tx_rx_freq_cal_sweep_rx_power_mode_enum;



/**
	Maximum number of frequencies 
*/
#define FTM_TX_RX_FREQ_MAX_FREQUENCIES 16

/**
	Maximum number of Segments per frequency
*/
#define FTM_TX_RX_FREQ_MAX_SEGMENTS 20

/**
	Structure that contains a super set of the command request parameters and the 
	resulting log.  Divided into an inputs section and outputs section.

	To be used with the function QLIB_FTM_TX_RX_FREQ_CAL_SWEEP()
*/

typedef struct
{
/**
Inputs, must be filled in before the QLIB_FTM_TX_RX_FREQ_CAL_SWEEP() function is called, but
        will also be overwritten by the response data.
	
*/
	/**
		Specifies which rx chain is active for each segment
        \code
        FTM_TX_RX_FREQ_RX_CHAIN_0 = 0, //!<' - Primary RX
        FTM_TX_RX_FREQ_RX_CHAIN_1 = 1  //!<' - Secondary RX
		\endcode
	*/
	word iRxChain;

	/**
		The number of frequencies on which the calibration procedure will be performed. Maximum is 16.
	*/
	byte iNumFreqs;

	/**
		The number of steps per frequency. A step corresponds to a power level at which tx 
		and rx measurements may be performed. This number must include one step for switching 
		frequency. Maximum value is 6.
	*/
	byte iNumSteps;

	
	/**
		The length of one power step in units of sleep counter cycles. Minimum value is 655 (20ms).
	*/
	word iStepLength;
	
	/**
		List of uplink channel numbers on which calibration is to be performed. Must have num_freqs elements.
	*/
	word aiChannelList[ FTM_TX_RX_FREQ_MAX_FREQUENCIES ];
	
	/**
		List giving the tx pdm to set for each step. Must have num_steps elements.
	*/
	word aiTxPDM_List[ FTM_TX_RX_FREQ_MAX_SEGMENTS ];
	
	/**
		The expected agc value used as the input to the LNA or DVGA offset calibration routine 
		for the primary RX path for each step. If rx_action_list is 255 for a particular step then 
		the expected agc value is not used.
	*/
	short aiExpectedRxAGC_ValList[ FTM_TX_RX_FREQ_MAX_SEGMENTS ];
	
	/**
		List giving the desired PA range for each step at a given frequency. Must have 
		num_steps elements. A value of 255 indicates no tx activity will take place for that step.
	*/
	byte aiPA_RangeList[ FTM_TX_RX_FREQ_MAX_SEGMENTS ];
	
	/**
		Flag indicating whether to take an HDET measurement for each step. Zero 
		indicates an HDET measurement will not be taken, a non-zero value indicates an 
		HDET measurement will be taken.
	*/
	byte aiReadHDET_List[ FTM_TX_RX_FREQ_MAX_SEGMENTS ];

	/**
		List specifying the rx action to take for each step of a given frequency. 
		
		Valid values are defined by the enumeration ftm_tx_rx_freq_cal_sweep_rx_action_enum:
				\code
                FTM_TX_RX_FREQ_RX_ACTION_LNA0	= 0,	//!<' - calibrate LNA offset 0
                FTM_TX_RX_FREQ_RX_ACTION_LNA1	= 1,	//!<' - calibrate LNA offset 1
                FTM_TX_RX_FREQ_RX_ACTION_LNA2	= 2,	//!<' - calibrate LNA offset 2
                FTM_TX_RX_FREQ_RX_ACTION_LNA3	= 3,	//!<' - calibrate LNA offset 3
                FTM_TX_RX_FREQ_RX_ACTION_LNA4	= 4,	//!<' - calibrate LNA offset 4
                FTM_TX_RX_FREQ_RX_ACTION_DVGA	= 100,	//!<' - calibrate DVGA offset
                FTM_TX_RX_FREQ_RX_ACTION_NOTHING = 255	//!<' - do nothing
				\endcode
	*/
	byte aiRxActionList[ FTM_TX_RX_FREQ_MAX_SEGMENTS ];



	/**
		List specifying the rx power mode to use for each step of a given frequency. 
		The values are ignored for chipsets that do not support rx power modes. 
		Valid values are:
        \code
        FTM_TX_RX_FREQ_RX_PM_High		= 0,	//!<' - Rx high power mode
        FTM_TX_RX_FREQ_RX_PM_Medium		= 1,	//!<' - Rx medium power mode
        FTM_TX_RX_FREQ_RX_PM_Low		= 2		//!<' - Rx low power mode
		\endcode
	*/
	byte aiRxPowerMode[ FTM_TX_RX_FREQ_MAX_SEGMENTS ];


/**
Output, will be filled in by QLIB_FTM_TX_RX_FREQ_CAL_SWEEP()
	
*/
	/**
		 The result of the requested rx calibration on the primary receive path (if any) for 
		 a given step and frequency. The results are ordered as each step for the first frequency, 
		 followed by each step for the second frequency, etc.
	*/
	word aiRxResults[FTM_TX_RX_FREQ_MAX_FREQUENCIES][ FTM_TX_RX_FREQ_MAX_SEGMENTS ];

	/**
		The HDET value for each step and each frequency. If read_hdet_list is not set to one 
		for a particular step than hdet_results should be ignored for the corresponding steps. 
		The results are ordered as each step for the first frequency, followed by each step for 
		the second frequency, etc.	
	*/
	word aiHDET_Results[FTM_TX_RX_FREQ_MAX_FREQUENCIES][ FTM_TX_RX_FREQ_MAX_SEGMENTS ];

	/**
		This value will be set to a 1 if the response log received from the mobile includes
		the extended fields for rx_chain_list and rx_lpm_list. 

		If this value is zero, then the values of aiRxChainList and aiRxPowerMode should
		not be used
	*/
	unsigned char bIsExtendedFormat;

} FTM_Tx_Rx_Freq_Cal_Sweep_Request_Response;
 
/******************************************************************************
						FTM - MediaFLO
*******************************************************************************/
/**
	\brief Enumeration of MediaFLO Command IDs
*/
typedef enum 
{
	_FTM_MF_GET_RSSI_CAL_POINT				= 400,	//!< MediaFLO - get Rssi Cal Point
	_FTM_MF_GET_RX_RSSI						= 401,	//!< MediaFLO - get Rssi value
	_FTM_MF_GET_AGC_STATE					= 402,	//!< MediaFLO - get AGC state
	_FTM_MF_WRITE_CAL_DATA					= 403,	//!< MediaFLO - write cal data to NV
	_FTM_MF_LNA_SELECT						= 406,	//!< MediaFLO - select LNA
	_FTM_MF_RF_SWITCH_CONFIG				= 407,	//!< MediaFLO - switch control pins
	_FTM_MF_SYNTH_LOCK						= 408,	//!< MediaFLO - synthesizer lock state
	_FTM_MF_SET_GAIN_STATE					= 411,	//!< MediaFLO - set gain state
	_FTM_MF_TUNE_PLL						= 413,	//!< MediaFLO - tune PLL 
	_FTM_MF_GET_IM2							= 415,	//!< MediaFLO - perform IM2 cal
} FTM_MF_Cmd_Id_Enum;

typedef enum 
{
	_FTM_MFLO_NS_SET_NS_MODE					= 0,   //!< MediaFLO NS - enable FLO demod
	_FTM_MFLO_NS_ACQUIRE_FLO_SYSTEM				= 1,   //!< MediaFLO NS - acquire FLO system
	_FTM_MFLO_NS_ACTIVATE_FLOW					= 2,   //!< MediaFLO NS - activate FLO
	_FTM_MFLO_NS_DEACTIVATE_FLOW				= 3,   //!< MediaFLO NS - deactivate FLO
	_FTM_MFLO_NS_GET_STREAM_INFO				= 4,   //!< MediaFLO NS - get stream info
	_FTM_MFLO_NS_GET_MLC_DYN_PARAMS				= 5,   //!< MediaFLO NS - get MLC dynamic params
	_FTM_MFLO_NS_RESET_MLC_PLP_STATS			= 7,   //!< MediaFLO NS - reset MLC PLP stats
	_FTM_MFLO_NS_RESET_ALL_PLP_STATS			= 8,   //!< MediaFLO NS - reset all PLP stats
	_FTM_MFLO_NS_GET_ALL_ACTIVE_FLOW_IDS		= 9,   //!< MediaFLO NS - get all active flow ids
	_FTM_MFLO_NS_GET_CONTROL_CHANNEL_RECORDS	= 10,  //!< MediaFLO NS - get control channel records
	_FTM_MFLO_NS_GET_AIS_CHANNEL				= 11,  //!< MediaFLO NS - get AIS channel
	_FTM_MFLO_NS_DEACTIVATE_ALL_FLOWS			= 12,  //!< MediaFLO NS - deactivate all flows
	_FTM_MFLO_NS_TUNE_FREQ_AND_BW				= 13,  //!< MediaFLO NS - tune freq and bw
	_FTM_MFLO_NS_GET_RSSI						= 14,  //!< MediaFLO NS - get RSSI
	_FTM_MFLO_NS_SET_FLO_SLEEP					= 15,  //!< MediaFLO NS - set FLO sleep
	_FTM_MFLO_NS_GET_OIS_STATE					= 16   //!< MediaFLO NS - get OIS State
} FTM_MF_NS_Cmd_Id_Enum;

typedef struct
{
  dword  num_good_pre_rs;
  dword  num_erasure_pre_rs;
  dword  num_good_post_rs;
  dword  num_erasure_post_rs;
} ftm_mflo_stats_type;

//! Structure to define storage area for a mediaFLO NS FTM request/response messages
typedef struct
{
  dword flow_id;
  byte   binding;
  byte  mlc_id;           
  byte  strm_id;
  byte  strm_uses_both_layers;
  byte  byte_interleave_enabled;
  byte  system;
  dword base_fft_addr;
  byte  trans_mode;
  byte  outer_code;
  byte  num_cbs_in_curr_sf;
  union 
  {
    ftm_mflo_stats_type plp_stats[2];  /* 8 uint32s */
    dword gen_1_w[8];
  } ftm_mflo_var_data_1;
  union 
  {
    ftm_mflo_stats_type cb_stats[2];   /* 8 uint32s */
    dword gen_2_w[8];
  } ftm_mflo_var_data_2;
  byte  strm1_bound;
  byte  strm2_bound;
  word strm_lens[2];
} ftm_mflo_type;


//! Structure to parse WIC and LIC packets
typedef struct
{
	int reserved;
	char WID;
	char LID;
}wid_lid_packet;

/******************************************************************************
						FTM - WLAN
*******************************************************************************/

////////////////////
// First are the Philips definitions
////////////////////

/** Maximum message size */
#define FTM_WLAN_Philips_MAX_MSG_SIZE 1600


/** Philips "source" field value */
#define FTM_WLAN_Philips_SOURCE 0xFF


/** The FTM_WLAN_Philips_PCTI_READ_STATISTICS  response size, in byte */
#define FTM_WLAN_Philips_PCTI_READ_STATISTICS_RESPONSE_SIZE 128

/**
	These are the module types which will be passed to the firmware
	for each specific module selection from the host.

	Used for SET_MODULE.
*/
typedef enum 
{
	/* Washington modules (not supported) */
	FTM_WLAN_Philips_M1		= 0x00,
	FTM_WLAN_Philips_M2		= 0x01,
	FTM_WLAN_Philips_M3		= 0x02,
	FTM_WLAN_Philips_M4		= 0x03,
	FTM_WLAN_Philips_MWAmax	= 0x0F, /* end of Washington range */
	/* Georgia modules */
	FTM_WLAN_Philips_M_BGW211	= 0x10
} FTM_WLAN_Philips_enModuleType;

/* *
	The data rate which will be passed from the host
	This gets transferred to the rates required in the firmware

	Used for TX_BURST
*/
typedef enum 
{
	/* 11b rates backwards compatible with washington */
	FTM_WLAN_Philips_RATE_1_MBPS	= 0x0,
	FTM_WLAN_Philips_RATE_2_MBPS	= 0x1,
	FTM_WLAN_Philips_RATE_5_MBPS	= 0x2,
	FTM_WLAN_Philips_RATE_11_MBPS	= 0x3,
	/* plcp rate codes used for OFDM rates */
	FTM_WLAN_Philips_RATE_6_MBPS	= 11,
	FTM_WLAN_Philips_RATE_9_MBPS	= 15,
	FTM_WLAN_Philips_RATE_12_MBPS	= 10,
	FTM_WLAN_Philips_RATE_18_MBPS	= 14,
	FTM_WLAN_Philips_RATE_24_MBPS	= 9,
	FTM_WLAN_Philips_RATE_36_MBPS	= 13,
	FTM_WLAN_Philips_RATE_48_MBPS	= 8,
	FTM_WLAN_Philips_RATE_54_MBPS	= 12
} FTM_WLAN_Philips_enDataRate;

/**
	Sleep Test Option defines, used for SLEEP_MODE
*/
typedef enum 
{
	FTM_WLAN_Philips_PCTI_SLEEP_DOWN = 0x0,
	FTM_WLAN_Philips_PCTI_TIMED_SLEEP = 0x1
} FTM_WLAN_Philips_enSleepMode;

/**
	These are the results which will be passed from the firmware
	for each specific command from the host
*/
typedef enum 
{
	FTM_WLAN_Philips_PCTI_SUCCESS			= 0x00,
	FTM_WLAN_Philips_PCTI_FAILURE			= 0x01,
	FTM_WLAN_Philips_PCTI_NOT_SUPPORTED		= 0x02,
	/* Georgia (PCTI) specific */
	FTM_WLAN_Philips_PCTI_INVALID_PARAMETER	= 0x03,
	FTM_WLAN_Philips_PCTI_MISSING_PARAMETER	= 0x04,
	FTM_WLAN_Philips_PCTI_TEST_RUNNING		= 0x05
} FTM_WLAN_Philips_enPctiStatus;


/**
	This corresponds to the data specific part of TX_CONTINUOUS test.
*/
typedef enum 
{
	FTM_WLAN_Philips_SINGLETONE_100KHZ		= 1,
	FTM_WLAN_Philips_SINGLETONE_3MHZ		= 2,
	FTM_WLAN_Philips_SINGLETONE_5d5MHZ		= 3,
	FTM_WLAN_Philips_DUALTONE_100_300KHZ	= 4,
	FTM_WLAN_Philips_DUALTONE_3_3d1MHZ		= 5,
	FTM_WLAN_Philips_DUALTONE_5d5_5d6MHZ	= 6,
	FTM_WLAN_Philips_QPSK_0_1_SEQUENCE		= 7,
	FTM_WLAN_Philips_QPSK_PN_SEQUENCE		= 8
} FTM_WLAN_Philips_enSignalType;

/**
	Specific request type used for passing Diagnostic request to the host
*/
typedef enum 
{
	FTM_WLAN_Philips_PCTI_START					= 0x01, //!< 'same as FTM_WLAN_Philips_PCTI_CONNECT_DUT 
	FTM_WLAN_Philips_PCTI_SLEEP					= 0x02, //!< 'not supported by PCTI
	FTM_WLAN_Philips_PCTI_TX_CONTINUOUS			= 0x03,
	FTM_WLAN_Philips_PCTI_TX_BURST				= 0x04,
	FTM_WLAN_Philips_PCTI_TX_POWER_CONTROL		= 0x05,
	FTM_WLAN_Philips_PCTI_RX_CONTINUOUS			= 0x06,
	FTM_WLAN_Philips_PCTI_REG_READ				= 0x08, //!< 'not supported by PCTI
	FTM_WLAN_Philips_PCTI_REG_WRITE				= 0x09, //!< 'not supported by PCTI
	FTM_WLAN_Philips_PCTI_EEPROM_READ			= 0x0A,
	FTM_WLAN_Philips_PCTI_EEPROM_WRITE			= 0x0B,
	FTM_WLAN_Philips_PCTI_SDIO_LOGIC			= 0x0C, //!< 'not supported by PCTI
	FTM_WLAN_Philips_PCTI_GET_MAC_ADDRESS		= 0x0D,
	FTM_WLAN_Philips_PCTI_STOP					= 0x12,
	FTM_WLAN_Philips_PCTI_SET_TX_DATA			= 0x13,
	FTM_WLAN_Philips_PCTI_THREE_WIRE_READ		= 0x14, //!< 'not supported by PCTI
	FTM_WLAN_Philips_PCTI_THREE_WIRE_WRITE		= 0x15, //!< 'not supported by PCTI
	FTM_WLAN_Philips_PCTI_SET_MODULE			= 0x16,
	FTM_WLAN_Philips_PCTI_DISCONNECT_DUT		= 0x17,
	/* Georgia (PCTI) specific */
	FTM_WLAN_Philips_PCTI_GET_HWSW_VERSIONS		= 0x20,
	FTM_WLAN_Philips_PCTI_TX_POWER_SET_CALIBRATED	= 0x21,
	FTM_WLAN_Philips_PCTI_MEMORY_READ			= 0x22,
	FTM_WLAN_Philips_PCTI_MEMORY_WRITE			= 0x23,
	FTM_WLAN_Philips_PCTI_SB_REGISTER_READ		= 0x24,
	FTM_WLAN_Philips_PCTI_SB_REGISTER_WRITE		= 0x25,
	FTM_WLAN_Philips_PCTI_RF_REGISTER_READ		= 0x26,
	FTM_WLAN_Philips_PCTI_RF_REGISTER_WRITE		= 0x27,
	FTM_WLAN_Philips_PCTI_COEX_LINE_READ		= 0x28,
	FTM_WLAN_Philips_PCTI_COEX_LINE_WRITE		= 0x29,
	FTM_WLAN_Philips_PCTI_CHECK_EXT_32KHZ_CLOCK = 0x2A,
	FTM_WLAN_Philips_PCTI_READ_STATISTICS		= 0x2B,
	FTM_WLAN_Philips_PCTI_RESET_STATISTICS		= 0x2C,
	FTM_WLAN_Philips_PCTI_SET_MAC_ADDRESS		= 0x2D,
	FTM_WLAN_Philips_PCTI_SLEEP_MODE			= 0x2E,
	FTM_WLAN_Philips_PCTI_GET_TEMPERATURE		= 0x2F,
	FTM_WLAN_Philips_PCTI_GET_CALIBRATIONINFO	= 0x30,
	FTM_WLAN_Philips_PCTI_SET_CALIBRATIONINFO	= 0x31,
	FTM_WLAN_Philips_PCTI_LOOPBACK_MODE			= 0x32,
	FTM_WLAN_Philips_PCTI_DEBUG_LEVEL			= 0x33
} FTM_WLAN_Philips__enPctiType;

/**
	Redefinition for Washington Compatibility
*/
#define FTM_WLAN_Philips_PCTI_CONNECT_DUT FTM_WLAN_Philips_PCTI_START


////////////////////
// Second are the Atheros definitions
////////////////////
/** Atheros: Continuous Transmit */
#define FTM_WLAN_Atheros_CMD_CONT_TRANSMIT	0x0

/** Atheros: Continuous Receive */
#define FTM_WLAN_Atheros_CMD_CONT_RECEIVE	0x1

/** Atheros: Force wake/sleep */ 
#define FTM_WLAN_Atheros_CMD_WAKE_SLEEP		0x2

/** Atheros: Continuous Receive Command Action*/

/** Accept all incoming frames */
#define FTM_WLAN_Atheros_CMD_CONT_RECEIVE_ACT_PROMISCUOUS_MODE	0x0
/** Accept frames accept only frames with dest address equal specified mac address */
#define FTM_WLAN_Atheros_CMD_CONT_RECEIVE_ACT_FILTER_MODE		0x1
/** Disable Rx and get last Rx report */
#define FTM_WLAN_Atheros_CMD_CONT_RECEIVE_ACT_OFF_MODE			0x2
/** Set MAC Address */
#define FTM_WLAN_Atheros_CMD_CONT_RECEIVE_ACT_SET_MAC_ADDRESS	0x3

typedef enum 
{
	/* 11b rates backwards compatible with washington */
	FTM_WLAN_Atheros_RATE_1_MBPS	= 0,
	FTM_WLAN_Atheros_RATE_2_MBPS	= 1,
	FTM_WLAN_Atheros_RATE_5_MBPS	= 2,
	FTM_WLAN_Atheros_RATE_11_MBPS	= 3,
	/* plcp rate codes used for OFDM rates */
	FTM_WLAN_Atheros_RATE_6_MBPS	= 4,
	FTM_WLAN_Atheros_RATE_9_MBPS	= 5,
	FTM_WLAN_Atheros_RATE_12_MBPS	= 6,
	FTM_WLAN_Atheros_RATE_18_MBPS	= 7,
	FTM_WLAN_Atheros_RATE_24_MBPS	= 8,
	FTM_WLAN_Atheros_RATE_36_MBPS	= 9,
	FTM_WLAN_Atheros_RATE_48_MBPS	= 10,
	FTM_WLAN_Atheros_RATE_54_MBPS	= 11,
} FTM_WLAN_Atheros_enDataRate;

typedef enum
{
	FTM_WLAN_Atheros_CMD_SYNC = 0,
	FTM_WLAN_Atheros_CMD_ASYNC = 1
} FTM_WLAN_Atheros_cmdType;


/////////////////////////////
///   Below this are Qualcomm specific definitions
/////////////////////////////



/**
	The list of commands in the FTM_WLAN mode_id
*/
typedef enum 
{
	_FTM_WLAN_USER_CMD		 = 0		//!<' Send a user command buffer to any WLAN module
} FTM_WLAN_CmdEnum;


/**
	Defines the types of modules supported, to be used for the eModuleType parameter
	for FTM_WLAN operations in QLIB.H, and for the FTM_WLAN_USER_CMD.

*/
typedef enum 
{
	FTM_WLAN_ModuleType_Philips	= 0x00,	//!< Philips WLAN module
	FTM_WLAN_ModuleType_Atheros	= 0x01,	//!< Atheros WLAN module

} FTM_WLAN_ModuleTypeEnum;

//! Size of WLAN MAC Address
#define FTM_WLAN_MAC_ADRESS_SIZE 6

/******************************************************************************
						FTM - QFUSE
*******************************************************************************/
typedef enum
{
	FTM_QFUSE_Read	= 0x00,		//!< QFUSE read request
	FTM_QFUSE_Write	= 0x01,		//!< QFUSE write request
} FTM_QFUSE_CmdEnum;

#define FTM_QFUSE_Configuration_Chain 0

/******************************************************************************
						QMSL - Text logging flags
*******************************************************************************/

//	---------------------------------------------------------
//	defines for phone logging settings
//
//	---------------------------------------------------------

#define LOG_NOTHING		0x0000	//!<' log nothing

#define LOG_C_HIGH_LEVEL_START	0x0200	//!<' High level C function start, indicates the begining of a high level C function, which
										//!<' calls other low level C functions internal to the library

#define LOG_C_HIGH_LEVEL_STOP	0x4000	//!<' High level C function stop

#define LOG_IO			0x0001	//!<' data IO (data bytes)
#define LOG_FN			0x0002	//!<' function calls with parameters
#define LOG_RET			0x0004	//!<' function return data

#define LOG_INF			0x0008	//!<' general information (nice to know)--do not use this one, as
								//!<' this space needs to be reserved for async messages

#define LOG_ASYNC		0x0008	//!<' asynchronous messages

#define LOG_ERR			0x0010	//!<' critial error information

#define LOG_IO_AHDLC	0x0020	//!<' HDLC IO tracing (data bytes)
#define LOG_FN_AHDLC	0x0040	//!<' HDLC layer function calls
#define LOG_RET_AHDLC	0x0080	//!<' HDLC function return data
#define LOG_INF_AHDLC	0x0100	//!<' HDLC general information	
#define LOG_ERR_AHDLC	LOG_INF_AHDLC	//!<' HDLC Error info merged with LOG_INF_AHDLC, to free up the log bit


#define LOG_IO_DEV		0x0400	//!<' device IO tracing (data bytes)
#define LOG_FN_DEV		0x0800	//!<' device layer function calls
#define LOG_RET_DEV		0x1000	//!<' device function return data
#define LOG_INF_DEV		0x2000	//!<' device general information
#define LOG_ERR_DEV		LOG_INF_DEV		//!<' device error information, merged with LOG_INF_DEV to free up the log bit

#define LOG_DEFAULT		(LOG_C_HIGH_LEVEL_START|LOG_C_HIGH_LEVEL_STOP|LOG_FN|LOG_IO|LOG_RET|LOG_ERR|LOG_ASYNC) //!<'  default settings

#define LOG_ALL			0xFFFF	//!<' everything

/**
	Enumeration of extended text logging categories, to be used with
	QLIB_ExtendedTextMessage_SetCategoryEnable(), 

	QMSL Developer note:  when this list is updated, please also update these functions:
	
*/
typedef enum
{
	QMSL_ExtTextMsgCat_MediaFLO_Parse         = 0,  //!< ' MediaFLO Layer one log parsing
	QMSL_ExtTextMsgCat_cdma2000_RDA_Parse     = 1   //!< ' cdma2000 RDA log parsing for SER/FER
} QMSL_Extended_Text_Message_Category_Enum;


//! Maximum number of characters in the category name string, to be used with QLIB_ExtendedTextMessage_GetCategoryListItem()
#define QMSL_EXTENDED_TEXT_MESSAGE_CATEGORY_NAME_SIZE 30

/******************************************************************************
						QMSL - Time Out defintions
*******************************************************************************/

/**
	Enumeration of time-out values that can be set / retrieved using
	QLIB_ConfigureTimeOut() and QLIB_GetTimeOut()

*/
typedef enum
{
	
	// Timeouts for Diag_FTM area
	QMSL_Timeout_General,			//<! 'General communications time out, used for SendSync()  (4,000ms default)
	QMSL_Timeout_IsPhoneConnected,	//<! 'Timeout when connecting IsPhoneConnected() command (80ms default)
	QMSL_Timeout_Connect,			//<! 'Timeout for connecting to a phone the first time (200ms default)
	
	QMSL_Timeout_Nonsignaling,		//<! 'Non-signaling timeout (1,000ms default)

	QMSL_Timeout_GSDI,				//<! 'Timeout to be used for GSDI commands (6,000ms default)
	QMSL_Timeout_CNV,				//<! 'Timeout to be used for preparation of CNV (default 10,000ms)

	QMSL_Timeout_CDMA_FreqSweep,	//<! 'Timeout to be used for CDMA Tx Rx Frequency Sweep (default 10,000ms)

	QMSL_Timeout_WriteData,			//<! 'Timeout to be used for writing data to a communications device, 
									//<! ' does not include read back of response (default 900ms)
									//<! ' this cannot be set in QPST mode.  

	QMSL_Timeout_ReadData,			//<! 'Timeout to be used for reading data from a communications device, 
									//<! ' this timeout is only for the low level read operation and does not 
									//<! ' consider the call context, such as whether a synchronous command is
									//<! ' currently being executed.  Default is 500ms

	QMSL_Timeout_GSDI_Event,		//<! 'Timeout to be used when waiting for a GSDI Diag event
									//<! ' Default is 3000ms

	/*
		Below this are configurable delays for the Diag_FTM area
	*/

	/**
		Delay when switching from ONLINE-FTM mode.  This is needed
		for some targets because they do not process FTM commands 
		immediately after going into FTM mode.  A recommended value
		is 200ms, but the default value is 0ms for backwards compatability.

		This is used in the function QLIB_DIAG_CONTROL_F()
	*/
	QMSL_Timeout_Delay_ONLINE_FTM,

	/**
		Delay when switching to ONLINE mode.  This is needed
		because it often takes some time for the AMSS to change modes.
		Default is 0ms

		This is used in the function QLIB_DIAG_CONTROL_F()
	*/
	QMSL_Timeout_Delay_ONLINE,

	/**
		Delay when switching to OFFLINE mode.  This is needed
		because it often takes some time for the AMSS to change modes, for example
		a power down registration must be done for some systems.
		Default is 3000ms

		This is used in the function QLIB_ChangeFTM_BootMode() and 
		in QLIB_DIAG_CONTROL_F().
	*/
	QMSL_Timeout_Delay_OFFLINE,

	/**
		This delay will be used when the mobile enters a GSM RF mode
	*/
	QMSL_Timeout_Delay_GSM_RF_Mode,

	
	/**
		This delay will be used by QLIB_MFLO_GetPER_Phy() and QLIB_MFLO_GetPER_PhyMAC(),
		to set the delay between checking FLO status.  Default value is 100ms	

		QLIB_MFLO_GetPER_Phy() uses this value to set the timeout when waiting for
		a status log.

		QLIB_MFLO_GetPER_PhyMAC() uses the value as the duration between the calls
		to poll the phone for status.

	 /**
		This delay will be used by QLIB_MFLO_GetPER_Phy() and QLIB_MFLO_GetPER_PhyMAC(),
		to set the delay between checking FLO status.  Default value is 100ms.  

		QLIB_MFLO_GetPER_Phy() uses this value as the duration between calls to get the
		stats when the stats are not updating.

		QLIB_MFLO_GetPER_PhyMAC() uses the value as the duration between the calls
		to get the next log packet from the queue when the queue is empty.
	*/
	QMSL_Timeout_Delay_MediaFLO_StatusCheck,



	// Note to QMSL developers, new delays and timeouts should be added here, before the
	// the SW Download timouts


	// Timeout for software download
	/**
		Timeout for the duration of the next softare download action.
		For example, a download that should take 7 minutes, the timeout can be set to 8 minutes.
		Default will be 10 minutes ( 10 minutes * 60seconds/minute * 1000milliseconds/secon = 600000ms
	*/
	QMSL_Timeout_SoftwareDownloadActivity,	


	QMSL_Timeout_ListSizeMax		//<! 'Not a timeout, just used to determine timeout list size
} QMSL_TimeOutType_Enum;

#pragma pack()	// Restore default packeting

#endif // !defined(_QLIB_DEFINES)
