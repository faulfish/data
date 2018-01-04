#pragma once
#include "../msm/diagcmd.h"

typedef enum
{
	DL_ERROR_SUCCESS			= 0,
	DL_ERROR_UNZIP				= 52,
	DL_ERROR_MISSFILES			= 53,
	DL_ERROR_FilePathEmpty  	= 54,
	DL_ERROR_BACKUP				= 55,
	DL_ERROR_ChangeDLMode		= 56,
	DL_ERROR_DMSS_NOP			= 57,
	DL_ERROR_DMSS_PARAMETER 	= 58,
	DL_ERROR_DMSS_Hex			= 59,
	DL_ERROR_DMSS_GO			= 60,
	DL_ERROR_Streaming_Hello	= 61,
	DL_ERROR_Streaming_Security	= 62,
	DL_ERROR_Streaming_Partition= 63,
	DL_ERROR_Streaming_Image	= 64,
	DL_ERROR_ReOpenComport		= 65,
	DL_ERROR_Restore			= 66,
	DL_ERROR_NVArray2Mobile		= 67,
	DL_ERROR_DuckFile2NVArray	= 68,
	DL_ERROR_InvaildFunctionParameter = 69,
	DL_ERROR_CHECKVERSION		= 71,
	DL_ERROR_SETFILENAME		= 72,
	DL_ERROR_UNDEFINE_COMMAND	= 73,
	DL_ERROR_DMSS_WRITE32BITADDRESS = 74,
	DL_ERROR_TX_ERROR			= 75,
	DL_ERROR_DIFFERENT_PARTITION= 76,
	DL_ERROR_NVRESTORE_FAIL		= 77,
	DL_ERROR_NVRESTORE_CNV_FAIL = 78,
	DL_ERROR_GETLOCFILE_FAIL	= 79, 
	DL_ERROR_Streaming_Hello_CheckBadBlock = 80
} DL_ERROR;

typedef enum
{
	DMSS_CMD_WRITE  = 0x01,   /* Write a block of data to memory (received)    */
	DMSS_CMD_ACK    = 0x02,   /* Acknowledge receiving a packet  (transmitted) */
	DMSS_CMD_NAK    = 0x03,   /* Acknowledge a bad packet        (transmitted) */
	DMSS_CMD_ERASE  = 0x04,   /* Erase a block of memory         (received)    */
	DMSS_CMD_GO     = 0x05,   /* Begin execution at an address   (received)    */
	DMSS_CMD_NOP    = 0x06,   /* No operation, for debug         (received)    */
	DMSS_CMD_PREQ   = 0x07,   /* Request implementation info     (received)    */
	DMSS_CMD_PARAMS = 0x08,   /* Provide implementation info     (transmitted) */
	DMSS_CMD_DUMP   = 0x09,   /* Debug: dump a block of memory   (received)    */
	DMSS_CMD_RESET  = 0x0A,   /* Reset the phone                 (received)    */
	DMSS_CMD_UNLOCK = 0x0B,   /* Unlock access to secured ops    (received)    */
	DMSS_CMD_VERREQ = 0x0C,   /* Request software version info   (received)    */
	DMSS_CMD_VERRSP = 0x0D,   /* Provide software version info   (transmitted) */
	DMSS_CMD_PWROFF = 0x0E,    /* Turn phone power off            (received)    */
	DMSS_CMD_WRITE32BITADDRESS = 0x0F,	/* Write data to memory using 32-bit address (received)	*/
} DMSS_cmd_code_type;


/*===========================================================================
PACKET   GET_DL_REVOCATION request response

ID       

PURPOSE  to check if revocation of boot loader and phone code

Table 1-1 Boot Loader Revocation Baseline request packet
Field	Field Length (bits)	Description
Command 0x14	8	Command identifier code ¡V The host shall set this field to 0x14.

Table 1-2 Boot Loader Revocation Baseline response packet
Field	Field Length (bits)	Description
Command (0x15)	8	Command identifier code ¡V The host shall set this field to 0x15.
Status	8	The status of revocation baseline read
0: Version read succeed
1: OTP is damaged
2: OTP is empty
Boot loader secure version	8 	An uint8 variable represent the boot loader¡¦s secure version

Table 2-1 Phone Code Revocation Baseline request packet
Field	Field Length (bits)	Description
Command (0x16)	8	Command identifier code ¡V The host shall set this field to 0x16.

Table 2-2 Phone Code Revocation Baseline response packet
Field	Field Length (bits)	Description
Command (0x17)	8	Command identifier code ¡V The host shall set this field to 0x17.
Status	8	The status of revocation baseline read
0: Version read succeed
1: OTP is damaged
2: OTP is empty
Phone code secure version	8 	An uint8 variable represent the phone code¡¦s secure version

===========================================================================*/
#pragma pack(push, 1)
typedef struct  
{
	BYTE command_code;
}GET_DL_REVOCATION;

typedef struct  
{
	BYTE command_code;
	BYTE status;
	BYTE version;
}GET_DL_REVOCATION_RESP;

#pragma pack(pop)

/*===========================================================================
PACKET   DMSS_Reset request response

ID       

PURPOSE  Reset in DMSS

===========================================================================*/

#pragma pack(push, 1)
typedef struct  
{
	BYTE command_code;
}DMSS_CMD_RESET_REQ;

typedef struct  
{
	BYTE command_code;
	BYTE dummy[1024];
}DMSS_CMD_RESET_RESP;

#pragma pack(pop)

// for double XOR 
// which is used in StreamWriteCommand
typedef struct
{
	BYTE	nXOR[4] ;
}Struct_XOR;

/*===========================================================================
PACKET   DLQueryHWVariant_RESP response

ID       

PURPOSE  For Lavernock querying HW ID and VariantID
===========================================================================*/
#pragma pack(push, 1)
typedef struct  
{
	BYTE command_code;
	BYTE varinat_id;
	BYTE hw_id;
}DLQueryHWVariant_RESP;

#pragma pack(pop)
