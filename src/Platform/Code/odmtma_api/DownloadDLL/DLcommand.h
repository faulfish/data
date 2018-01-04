#pragma once

#include "../../Lib/RequestUtility/DiagCommand.h"

#ifndef WRITE_IMAGE_2048
const int PAGE_SIZE	= 0x400; 
#else
const int PAGE_SIZE	= 0x800; 
#endif

const int gc_nDumpImagePageSize = 512;

/* Common combinations of bits. */
#define ACCESSPERMS     0777
#define ALLPERMS        0777
#define DEFFILEMODE     0666


///////////////////////////////////////////////////////////////////////////////////////////
//
//						EFS2 Diag Request Packet
//
///////////////////////////////////////////////////////////////////////////////////////////
#define FS_NAME_MAX 80
#define FS_PATH_MAX 128
#define FS_DIR_ENTRY_SIZE 50
#define FS_MAX_BUFFER_SIZE 512

#define O_ACCMODE          0003
#define O_RDONLY             00
#define O_WRONLY             01
#define O_RDWR               02
#define O_CREAT            0100 /* not fcntl */
#define O_EXCL             0200 /* not fcntl */
#define O_NOCTTY           0400 /* not fcntl */
#define O_TRUNC           01000 /* not fcntl */
#define O_APPEND          02000
#define O_NONBLOCK        04000
#define O_NDELAY        O_NONBLOCK
#define O_SYNC           010000
#define FASYNC           020000 /* fcntl, for BSD compatibility */
#define O_DIRECT         040000 /* direct disk access hint */
#define O_LARGEFILE     0100000
#define O_DIRECTORY     0200000 /* must be a directory */
#define O_NOFOLLOW      0400000 /* don't follow links */
#define O_ITEMFILE     01000000 /* Create special ITEM file. */

//!	File system type length
#define EFS_FSTYPE_LEN 8


typedef enum
{
	Streaming_CMD_HELLO	= 0x01,		/* Initialize connection    */
	Streaming_CMD_HELLORESP	= 0x02,	/* Acknowledge the connection, ready to unlock */
	Streaming_CMD_READ	= 0x03,		/* Read some number of bytes from phone memory */
	Streaming_CMD_READDATA	= 0x04,	/* Response to a simple read */
	Streaming_CMD_SIMWRITE	= 0x05,	/* Write data without erase */
	Streaming_CMD_WRODEOK	= 0x06,	/* Response to simple write */
	Streaming_CMD_STRWRITE	= 0x07, /* Streaming write command */
	Streaming_CMD_BOLCKWRI	= 0x08,	/* Response to stream write */ 
	Streaming_CMD_NOP	= 0x09,		/* No operation */
	Streaming_CMD_NOPRESP	= 0x0A,	/* Response to NOP */
	Streaming_CMD_RESET	= 0x0B,		/* Reset target */
	Streaming_CMD_RESETACK	= 0x0C,	/* Response to reset */
	Streaming_CMD_ERROR	= 0x0D,		/* Target error */
	Streaming_CMD_LOG	= 0x0E,		/* Target informational message */
	Streaming_CMD_UNLOCK	= 0x0F,	/* Unlock target */
	Streaming_CMD_UNLOCKED	= 0x10,	/* Response to unlock */
	Streaming_CMD_POWEROFF	= 0x11,	/* Power off target */
	Streaming_CMD_POWDOWN	= 0x12,	/* Response to power off */
	Streaming_CMD_OPEN	= 0x13,		/* Open for writing image */
	Streaming_CMD_OPENED	= 0x14,	/* Response to open for writing image */
	Streaming_CMD_CLOSE	= 0x15,		/* Close and flush last partial write to flash */
	Streaming_CMD_CLOSED	= 0x16,	/* Response to close and flush last partial write to flash */
	Streaming_CMD_SECMODE	= 0x17,	/* Send security mode to use for programming images */
	Streaming_CMD_SECMODER	= 0x18, /* Response to send security mode*/
	Streaming_CMD_PATTTIONT	= 0x19,	/* Send partition table to use for programming images */
	Streaming_CMD_PATTTIONTR	= 0x1A,	/* Response to send partition table */
	Streaming_CMD_OPENMIMAGE	= 0x1B,	/* Open for writing image ( Multi-image mode only ) */
	Streaming_CMD_OPENEDMIMAGE	= 0x1C, /* Response to open for writing image */
	Streaming_CMD_ERASEFLASH	= 0x1D,	/* Erase entire Flash device */
	Streaming_CMD_EFLASERASED	= 0x1E,	/* Response to erase flash */
	Streaming_CMD_GETECCSTATE	= 0x1F,	/*Read current ECC generation status */
	Streaming_CMD_ECCSTATE	= 0x20,	/* Response to Get ECC state with current stats */
	Streaming_CMD_SETECC	= 0x21,	/* Enable/disable hardware ECC generation */
	Streaming_CMD_SETECCR	= 0x22,	/* Response to set ECC */
	Streaming_CMD_SpeedUp_HELLO	= 0x50,		
	Streaming_CMD_SpeedUp_HELLORESP	= 0x51,	/* Acknowledge the connection, ready to unlock */
	Streaming_CMD_SpeedUp_CheckBadBlock_HELLO	= 0x52,		//check bad block after sending hello.
	Streaming_CMD_SpeedUp_CheckBadBlock_HELLORESP_1st = 0x53,   //when unit get hello command, it sends 1st response 0x53 and begin to check bad block.
	Streaming_CMD_SpeedUp_CheckBadBlock_HELLORESP_2nd = 0x95,  // after checking bad block, it sends 2nd response 0x95.
	Streaming_CMD_WRITEDUMPIMAGE = 0x96, /* Write dump 512 image*/
	Streaming_CMD_CHECKIMAGEINTEGRITY = 0x97 /*Check Image Integrity*/
} Streaming_cmd_code_type;

//typedef enum
//{
//	DIAG_SUBSYS_OEM					= 0,
//	DIAG_SUBSYS_ZREX				= 1,
//	DIAG_SUBSYS_SD					= 2,
//	DIAG_SUBSYS_BT					= 3,
//	DIAG_SUBSYS_WCDMA				= 4,
//	DIAG_SUBSYS_HDR					= 5,
//	DIAG_SUBSYS_DIABLO				= 6,
//	DIAG_SUBSYS_TREX				= 7,
//	DIAG_SUBSYS_GSM					= 8,
//	DIAG_SUBSYS_UMTS				= 9,
//	DIAG_SUBSYS_HWTC				= 10,
//	DIAG_SUBSYS_FTM					= 11,
//	DIAG_SUBSYS_REX					= 12,
//	DIAG_SUBSYS_GPS					= 13,
//	DIAG_SUBSYS_WMS					= 14,
//	DIAG_SUBSYS_CM					= 15,
//	DIAG_SUBSYS_HS					= 16,
//	DIAG_SUBSYS_AUDIO_SETTINGS		= 17,
//	DIAG_SUBSYS_DIAG_SERV			= 18,
//	DIAG_SUBSYS_EFS					= 19,
//	DIAG_SUBSYS_PORT_MAP_SETTINGS	= 20,
//	DIAG_SUBSYS_MEDIAPLAYER			= 21,
//	DIAG_SUBSYS_QCAMERA				= 22,
//	DIAG_SUBSYS_MOBIMON				= 23,
//	DIAG_SUBSYS_GUNIMON				= 24,
//	DIAG_SUBSYS_LSM					= 25,
//	DIAG_SUBSYS_QCAMCORDER			= 26,
//	DIAG_SUBSYS_MUX1X				= 27,
//	DIAG_SUBSYS_DATA1X				= 28,
//	DIAG_SUBSYS_SRCH1X				= 29,
//	DIAG_SUBSYS_CALLP1X				= 30,
//	DIAG_SUBSYS_APPS				= 31,
//	DIAG_SUBSYS_SETTINGS			= 32,
//	DIAG_SUBSYS_GSDI				= 33,
//	DIAG_SUBSYS_TMC					= 34,
//	DIAG_SUBSYS_USB					= 35,
//	DIAG_SUBSYS_PM					= 36,
//	DIAG_SUBSYS_DEBUG				= 37,
//	DIAG_SUBSYS_QTV					= 38,
//	DIAG_SUBSYS_CLKRGM				= 39,
//	DIAG_SUBSYS_DEVICES				= 40,
//	DIAG_SUBSYS_WLAN				= 41,
//	DIAG_SUBSYS_PS_DATA_LOGGING		= 42,
//	DIAG_SUBSYS_MFLO				= 43,
//	DIAG_SUBSYS_DTV					= 44,
//	DIAG_SUBSYS_RRC					= 45,
//	DIAG_SUBSYS_PROF				= 46,
//	DIAG_SUBSYS_TCXOMGR				= 47,
//	DIAG_SUBSYS_NV					= 48,
//	DIAG_SUBSYS_AUTOCONFIG			= 49,
//	DIAG_SUBSYS_PARAMS				= 50,
//	DIAG_SUBSYS_MDDI				= 51,
//	DIAG_SUBSYS_DS_ATCOP			= 52,
//	DIAG_SUBSYS_L4LINUX				= 53,
//	DIAG_SUBSYS_MVS					= 54,
//	DIAG_SUBSYS_CNV					= 55,
//	DIAG_SUBSYS_LAST				= 56
//} DiagPkt_Subsys_cmd_enum_type;
typedef enum
{	EFS2_DIAG_HELLO					= 0,	/* Parameter negotiation packet               */
	EFS2_DIAG_QUERY					= 1,	/* Send information about EFS2 params         */
	EFS2_DIAG_OPEN					= 2,	/* Open a file                                */
	EFS2_DIAG_CLOSE					= 3,	/* Close a file                               */
	EFS2_DIAG_READ					= 4,	/* Read a file                                */
	EFS2_DIAG_WRITE					= 5,	/* Write a file                               */
	EFS2_DIAG_SYMLINK				= 6,	/* Create a symbolic link                     */
	EFS2_DIAG_READLINK				= 7,	/* Read a symbolic link                       */
	EFS2_DIAG_UNLINK				= 8,	/* Remove a symbolic link or file             */
	EFS2_DIAG_MKDIR					= 9,	/* Create a directory                         */
	EFS2_DIAG_RMDIR					= 10,	/* Remove a directory                         */
	EFS2_DIAG_OPENDIR				= 11,	/* Open a directory for reading               */
	EFS2_DIAG_READDIR				= 12,	/* Read a directory                           */
	EFS2_DIAG_CLOSEDIR				= 13,	/* Close an open directory                    */
	EFS2_DIAG_RENAME				= 14,	/* Rename a file or directory                 */
	EFS2_DIAG_STAT					= 15,	/* Obtain information about a named file      */
	EFS2_DIAG_LSTAT					= 16,	/* Obtain information about a symbolic link   */
	EFS2_DIAG_FSTAT					= 17,	/* Obtain information about a file descriptor */
	EFS2_DIAG_CHMOD					= 18,	/* Change file permissions                    */
	EFS2_DIAG_STATFS				= 19,	/* Obtain file system information             */
	EFS2_DIAG_ACCESS				= 20,	/* Check a named file for accessibility       */
	EFS2_DIAG_NAND_DEV_INFO			= 21,	/* Get NAND device info               */
	EFS2_DIAG_FACT_IMAGE_START		= 22,	/* Start data output for Factory Image*/
	EFS2_DIAG_FACT_IMAGE_READ		= 23,	/* Get data for Factory Image         */
	EFS2_DIAG_FACT_IMAGE_END		= 24,	/* End data output for Factory Image  */
	EFS2_DIAG_PREP_FACT_IMAGE		= 25,	/* Prepare file system for image dump */
	EFS2_DIAG_PUT					= 26,	/* Write an EFS item file                     */
	EFS2_DIAG_GET					= 27,	/* Read an EFS item file                      */
	EFS2_DIAG_ERROR					= 28,	/* Semd an EFS Error Packet back through DIAG */
	EFS2_DIAG_EXTENDED_INFO			= 29,	/* Get Extra information.                */
	EFS2_DIAG_CHOWN					= 30,	/* Change ownership                      */
	EFS2_DIAG_BENCHMARK_START_TEST	= 31,	/* Start Benchmark               */
	EFS2_DIAG_BENCHMARK_GET_RESULTS = 32,	/* Get Benchmark Report          */
	EFS2_DIAG_BENCHMARK_INIT        = 33,	/* Init/Reset Benchmark          */
	EFS2_DIAG_SET_RESERVATION       = 34,	/* Set group reservation         */
	EFS2_DIAG_SET_QUOTA             = 35,	/* Set group quota               */
	EFS2_DIAG_GET_GROUP_INFO        = 36	/* Retrieve Q&R values           */
}DIAG_cmd_code_type_EFS;

//6245 MultiImage_type format
typedef enum
{
	Image_type_PRIBL		= 0x01,	/* Primary boot loader */
	Image_type_QCSECBLHD	= 0x02,	/* Qc Secondary boot loader header and config data */
	Image_type_QCSECBL		= 0x03,	/* Qc secondary boot loader */
	Image_type_OEMSECBL		= 0x04,	/* Oem secondary boot loader and need Oem header */
	Image_type_AMSSMODEL	= 0x05,	/* AMSS modem executable and need AMSS modem header */
	Image_type_AMSSAPP		= 0x06,	/* AMSS applications executable and need AMSS applications header */
	Image_type_6250OTPBL	= 0x07, /* MSM6250 OTP boot loader */
	Image_type_FOTAUI		= 0x08,	/* FOTA UI binary */ 
	Image_type_CEFS			= 0x09,	/* Compact EFS2 image */
	Image_type_AMSSAPPBL	= 0x0A,	/* AMSS applications boot loader and need AMSS applications boot loader header */
	Image_type_Flash		= 0x0C, /* Flash.Bin */
} MultiImage_type;


//6240 open_multi_mode_type
typedef enum
{
	OPEN_MULTI_MODE_NONE        = 0x00,    /* Not opened yet                       */
	OPEN_MULTI_MODE_PBL         = 0x01,    /* Primary Boot Loader                  */
	OPEN_MULTI_MODE_QCSBLHDCFG  = 0x02,    /* QC 2ndary Boot Loader Header and     */
	/*      Config Data                     */
	OPEN_MULTI_MODE_QCSBL       = 0x03,    /* QC 2ndary Boot Loader                */
	OPEN_MULTI_MODE_OEMSBL      = 0x04,    /* OEM 2ndary Boot Loader               */
	OPEN_MULTI_MODE_AMSS        = 0x05,    /* AMSS modem executable                */
	OPEN_MULTI_MODE_APPS        = 0x06,    /* APPS executable                      */
	OPEN_MULTI_MODE_OBL         = 0x07,    /* OTP Boot Loader                      */
	OPEN_MULTI_MODE_FOTAUI      = 0x08,    /* FOTA UI binarh                       */
	OPEN_MULTI_MODE_CEFS        = 0x09,    /* Modem CEFS image                     */    
	OPEN_MULTI_MODE_APPSBL      = 0x0A,    /* APPS Boot Loader                     */
	OPEN_MULTI_MODE_APPS_CEFS   = 0x0B,    /* APPS CEFS image                      */    
	OPEN_MULTI_MODE_APPS_WM60   = 0x0C,    /* Flash.bin image for Windows mobile 6 */ 
	OPEN_MULTI_MODE_DSP1        = 0x0D,    /* DSP1 runtime image                   */ 
	OPEN_MULTI_MODE_CUSTOM      = 0x0E,    /* Image for user defined partition     */ 
	OPEN_MULTI_MODE_DBL         = 0x0F,    /* Device Boot Loader                   */
	OPEN_MULTI_MODE_OSBL        = 0x10,    /* Fail Safe Boot Loader                */
	OPEN_MULTI_MODE_FSBL        = 0x11,    /* OS Boot Loader                       */
	OPEN_MULTI_MODE_DSP2        = 0x12,    /* DSP2 runtime image                   */ 
	OPEN_MULTI_MODE_RAW         = 0x13    /* APPS Raw image                       */
} open_multi_mode_type;

//BYTE	lpbyRXdata[0x1000];
#pragma pack(push, 1)
// DMSS_CMD_WRITE  = 0x01,   /* Write a block of data to memory (received)    */
typedef struct 
{
	const static BYTE	nDloadCmd = 0x01;
	BYTE    nsAddress[3];
	BYTE    nsLength[2];
	BYTE    nsData[PAGE_SIZE];
} DmssWriteReq;

// DMSS_CMD_ACK    = 0x02,   /* Acknowledge receiving a packet  (transmitted) */
// DMSS_CMD_NAK    = 0x03,   /* Acknowledge a bad packet        (transmitted) */
typedef union 
{
	// ACK
	BYTE nNopResp;
	// NACK
	BYTE	sNAck[10];
	BYTE dummy[1024];
}DmssACKandNACKResp;

// DMSS_CMD_ERASE  = 0x04,   /* Erase a block of memory         (received)    */
typedef struct
{
	BYTE	nDloadCmd;
	BYTE    nsAddress[3];
	BYTE    nsLength[3];
}DmssEraseReq;

// DMSS_CMD_GO     = 0x05,   /* Begin execution at an address   (received)    */
typedef struct
{
	BYTE	nDloadCmd;
	BYTE    nsSegment[2];
	BYTE    nsOffset[2];
}DmssGOReq;

// DMSS_CMD_NOP    = 0x06,   /* No operation, for debug         (received)    */
typedef struct
{
	BYTE	nDloadCmd;
}DmssNOPReq;

//DMSS_CMD_PREQ   = 0x07,   /* Request implementation info     (received)    */
typedef struct
{
	BYTE	nDloadCmd;
}DmssParaReq;

//DMSS_CMD_PARAMS = 0x08,   /* Provide implementation info     (transmitted) */
typedef struct
{
	BYTE	nDloadCmd;
	BYTE	nVersion;
	BYTE	nMinVersion;
	BYTE	nMaxWriteSize[2];
	BYTE	nMobileModel;
	BYTE	nDrivesize;
	BYTE	nDrivetype;
	BYTE dummy[1024];
}DmssPraResp;

// DMSS_CMD_DUMP   = 0x09,   /* Debug: dump a block of memory   (received)    */
typedef struct
{
	BYTE	nDloadCmd;
	BYTE    nsAddresst[3];
	BYTE    nsLength[2];
}DmssDumpReq;

// DMSS_CMD_RESET  = 0x0A,   /* Reset the phone                 (received)    */
typedef struct
{
	BYTE	nDloadCmd;
}DmssResetReq;

// DMSS_CMD_UNLOCK = 0x0B,   /* Unlock access to secured ops    (received)    */
typedef struct
{
	BYTE	nDloadCmd;
	BYTE    nsSecurityCode[8];
}DmssUnlockReq;

// DMSS_CMD_VERREQ = 0x0C,   /* Request software version info   (received)    */
typedef struct
{
	BYTE	nDloadCmd;
}DmssVERREQ;

// DMSS_CMD_VERRSP = 0x0D,   /* Provide software version info   (transmitted) */
typedef struct
{
	BYTE	nDloadCmd;
	BYTE    nsLength[8];
	BYTE    nsVersionstring[20];
}DmssVERRSP;

// DMSS_CMD_PWROFF = 0x0E,    /* Turn phone power off            (received)    */
typedef struct
{
	const static BYTE nDloadCmd = 0x0E;
}DmssPwroff;

// DMSS_CMD_WRITE32BITADDRESS = 0x0F,	/* Write data to memory using 32-bit address (received)	*/
typedef struct DmssWrite32bitAddressReq
{
	BYTE	nDloadCmd ;
	BYTE    nsAddress[4];
	BYTE    nsLength[2];
	BYTE    nsData[PAGE_SIZE];
} DmssWrite32bitAddressReq;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// Streaming Download ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Streaming_CMD_HELLO	= 0x01,		/* Initialize connection    */
typedef struct 
{
	BYTE	nCmdCode;
	BYTE	nMagicNumber[32];
	BYTE	nVersion;
	BYTE    nCVersion;
	BYTE	nFeaturebit;
}StreamingHelloParmReq;

//Streaming_CMD_HELLORESP	= 0x02,	/* Acknowledge the connection, ready to unlock */
typedef struct 
{
	BYTE	nCmdCode;
	BYTE	nLength[32];
	BYTE	nVersion;
	BYTE    nCVersion;
	BYTE	nMaxBlockSize[4];
	BYTE	nBaseAddressFlash[4];
	BYTE	nFlashIDLength;
	BYTE	nData[576];
	BYTE dummy[1024];
}StreamingHelloParmResp;

typedef struct 
{
	BYTE	nCmdCode;
}StreamingHelloParm_CheckBadBlock2ndResp;

//Streaming_CMD_READ	= 0x03,		/* Read some number of bytes from phone memory */
//Streaming_CMD_READDATA	= 0x04,	/* Response to a simple read */
//Streaming_CMD_SIMWRITE	= 0x05,	/* Write data without erase */
//Streaming_CMD_WRODEOK	= 0x06,	/* Response to simple write */
//Streaming_CMD_STRWRITE	= 0x07, /* Streaming write command */
typedef struct
{
	BYTE	nDloadCmd ;
	BYTE	nAddress[4];
	BYTE	nData[PAGE_SIZE];
}StreamingStrWriteReq;

typedef struct
{
	BYTE	nDloadCmd ;
	BYTE	nAddress[4];
	BYTE	nData[gc_nDumpImagePageSize];
}StreamingStrWriteDumpReq; // write Dump Image

typedef struct
{
	BYTE	nDloadCmd ;
	BYTE	nAddress[4];
	BYTE	nData[PAGE_SIZE];
}StreamingStrWriteDump_NormalReq; // write Dump Image

//Streaming_CMD_BOLCKWRI	= 0x08,	/* Response to stream write */ 
typedef struct
{
	BYTE	nDloadCmd ;
	BYTE	nAddress[4];
	BYTE dummy[1024];
}StreamingStrWriteResp;

//Streaming_CMD_NOP	= 0x09,		/* No operation */
//Streaming_CMD_NOPRESP	= 0x0A,	/* Response to NOP */


//Streaming_CMD_RESET	= 0x0B,		/* Reset target */
typedef struct
{
	BYTE	nDloadCmd ;
}StreamingResetReq;

//Streaming_CMD_RESETACK	= 0x0C,	/* Response to reset */
typedef struct
{
	BYTE	nDloadCmd ;
	BYTE dummy[1024];
}StreamingResetResp;


//QuerySize	= 0x97,	/* Query Size and XOR */
typedef struct
{
	BYTE	nDloadCmd;
}QuerySizeReq;

// response of Querying ImageSize and XOR 
typedef struct
{
	BYTE	nDloadCmd;
	BYTE	nSize[4];
	BYTE    nXOR[4];
	BYTE dummy[1024];
}QuerySizeResp;



//Streaming_CMD_ERROR	= 0x0D,		/* Target error */
typedef struct
{
	BYTE	nDloadCmd ;
	BYTE	nErrorCode;
	BYTE	nTexedata[PAGE_SIZE];
	BYTE dummy[1024];
}StreamingErrorResp;

//Streaming_CMD_LOG	= 0x0E,		/* Target informational message */
typedef struct
{
	BYTE	nDloadCmd ;
	BYTE	nTexedata[PAGE_SIZE];
	BYTE dummy[1024];
}StreamingLogResp;

//Streaming_CMD_UNLOCK	= 0x0F,	/* Unlock target */
//Streaming_CMD_UNLOCKED	= 0x10,	/* Response to unlock */
//Streaming_CMD_POWEROFF	= 0x11,	/* Power off target */
//Streaming_CMD_POWDOWN	= 0x12,	/* Response to power off */
//Streaming_CMD_OPEN	= 0x13,		/* Open for writing image */
//Streaming_CMD_OPENED	= 0x14,	/* Response to open for writing image */
//Streaming_CMD_CLOSE	= 0x15,		/* Close and flush last partial write to flash */

typedef struct
{
	BYTE	nDloadCmd ;
}StreamingCloseReq;

//Streaming_CMD_CLOSED	= 0x16,	/* Response to close and flush last partial write to flash */
typedef struct
{
	BYTE	nDloadCmd ;
	BYTE  dummy[1024];
}StreamingCloseResp;

//Streaming_CMD_SECMODE	= 0x17,	/* Send security mode to use for programming images */
typedef struct
{
	BYTE	nDloadCmd ;
	BYTE	nMode;
}StreamingSecurityReq;

//Streaming_CMD_SECMODER	= 0x18, /* Response to send security mode*/
typedef struct
{
	BYTE	nDloadCmd ;
	BYTE  dummy[1024];
}StreamingSecurityResp;

//Streaming_CMD_PATTTIONT	= 0x19,	/* Send partition table to use for programming images */
const int PARTITION_IMAGE_SIZE	= 512; 
typedef struct
{
	BYTE	nDloadCmd ;
	BYTE	nOverrive;
	BYTE	nPartition[PARTITION_IMAGE_SIZE];
}StreamingPartitionReq;

//Streaming_CMD_PATTTIONTR	= 0x1A,	/* Response to send partition table */
typedef struct
{
	BYTE	nDloadCmd ;
	BYTE	nStatus;
	BYTE  dummy[1024];
}StreamingPartitionResp;

//Streaming_CMD_OPENMIMAGE	= 0x1B,	/* Open for writing image ( Multi-image mode only ) */
typedef struct
{
	BYTE	nDloadCmd ;
	BYTE	nType;
	BYTE	nData[512];
}StreamingOpenmimageReq;

//Streaming_CMD_OPENEDMIMAGE	= 0x1C, /* Response to open for writing image */
typedef struct
{
	BYTE	nDloadCmd ;
	BYTE	nStatus;
	BYTE  dummy[1024];
}StreamingOpenmimageResp;

//Streaming_CMD_ERASEFLASH	= 0x1D,	/* Erase entire Flash device */
//Streaming_CMD_EFLASERASED	= 0x1E,	/* Response to erase flash */
//Streaming_CMD_GETECCSTATE	= 0x1F,	/*Read current ECC generation status */
//Streaming_CMD_ECCSTATE	= 0x20,	/* Response to Get ECC state with current stats */
//Streaming_CMD_SETECC	= 0x21,	/* Enable/disable hardware ECC generation */
//Streaming_CMD_SETECCR	= 0x22	/* Response to set ECC */

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// DIAG ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
	DIAG_CMD_ReadNVitem			= 0x26,		/* Nonvolatile item read request Message	*/
	DIAG_CMD_WriteNVitem		= 0x27,		/* Nonvolatile item read request Message	*/
	DIAG_CMD_ChaneMode			= 0x29,		/* Mode change Request Message */	
//	DIAG_SUBSYS_CMD_F			= 0x4B,		/* SubSystem dispatcher (extended diag cmd) */
//	DIAG_PROTOCOL_LOOPBACK_F	= 0x7B		/* Is Phone Connected request MEssage		*/
} DIAG_cmd_code_type;

#pragma pack(push, 1)
typedef struct
{
	BYTE	nDIAGcmd;
	BYTE	nNVitem[2];
	BYTE	nItemData[128];
	BYTE	nStatus[2];
}DIAGReadNVcmdRequest;


typedef struct
{
	BYTE	nDIAGcmd;
	BYTE	nNVitem[2];
	BYTE	nAddress;
}DIAGReadNVArraycmdRequest;

typedef struct
{
	BYTE	nDIAGcmd;
	BYTE	nNVitem[2];
	BYTE	nItemData[128];
	BYTE	nStatus[2];
}DIAGWriteNVcmdRequest;

typedef struct
{
	BYTE	nDIAGcmd;
	BYTE	nNVitem[2];
	BYTE	nItemData[128];
	BYTE	nStatus[2];
}DIAGNVcmdResponse;
#pragma pack(pop)
typedef struct
{
	BYTE	nDIAGcmd;
	BYTE	nNVitem[2];
	BYTE	nAddress;
	BYTE	nItemData[128];
	BYTE	nStatus[2];
}DIAGWriteNVArraycmdRequest;

typedef struct
{
	BYTE	nDIAGcmd;
	BYTE	nNVitem[2];
	BYTE	nAddress;
	BYTE	nItemData[127];
	BYTE	nStatus[2];
}DIAGNVArraycmdResponse;

typedef struct
{
	BYTE	nDIAGcmd;
	BYTE	nMode[2];
}DIAGChgModecmdRequest;

typedef struct
{
	BYTE	nDIAGcmd;
	BYTE	nMode[2];
}DIAGChgModecmdResponse;

typedef struct 
{
	BYTE	nDIAGcmd;
	BYTE	nZeroToNine[10];
}DIAGIsPhoneConnectedcmdRequest;

typedef struct 
{
	BYTE	nDIAGcmd;
	BYTE	nZeroToNine[10];
}DIAGIsPhoneConnectedcmdResponse;

//#pragma pack(push, 1)		//For Alignment using, You can set up in "Code Generation"
typedef struct  
{
	BYTE	nDIAGcmd;
	BYTE	nSubsysID;
	WORD	nCmdCodeFsAvlSpace;
	BYTE	nPath;
}DIAGEFSFsAvlSpaceCmdRequest;
//#pragma pack(pop)

typedef struct
{
	BYTE	nDIAGcmd;					//!<' Command header
	BYTE	nSubsysID;
	WORD	wCmdCodeFsAvlSpace;
	DWORD	dwErrnum;					//!<' Error code if error, 0 otherwise
	DWORD	dwFsid;						//!<' FileSystem ID
	BYTE	nFstype[EFS_FSTYPE_LEN];	//!<' FileSystem type
	DWORD	dwBlock_size;				//!<' FileSystem block size
	DWORD	dwTotalBlocks;				//!<' FileSystem size in blocks
	DWORD	dwAvailBlocks;				//!<' Blocks available in FileSystem
	DWORD	dwFreeBlocks;				//!<' Blocks free in FileSystem
	DWORD	dwMaxFileSize;				//!<' Maximum size of a file in this FileSystem
	DWORD	dwFiles;					//!<' Current number of files in this FileSystem
	DWORD	dwMaxFiles;					//!<' Maximum number of files in this FileSystem
}DIAGEFSFsAvlSpaceCmdResponse;

typedef struct 
{
	BYTE	nDIAGcmd;
	BYTE	nSubsysID;
	WORD	wCmdCodeOpenFile;
	DWORD	dwFlag;
	DWORD	dwMode;
	BYTE	nPath[MAX_PATH];
}DIAGEFSOpenFileCmdRequest;

typedef struct 
{
	BYTE	nDIAGcmd;
	BYTE	nSubsysID;
	WORD	wCmdCodeOpenFile;
	INT		nFileDescripter;
	INT		nErrNum;
}DIAGEFSOpenFileCmdResponse;

typedef struct 
{
	BYTE	nDIAGcmd;
	BYTE	nSubsysID;
	WORD	wCmdCodeWriteFile;
	DWORD	dwFileDescripter;
	DWORD	dwOffset;
	BYTE	nData[FS_MAX_BUFFER_SIZE];
}DIAGEFSWriteCmdRequest;

typedef struct 
{
	BYTE	nDIAGcmd;
	BYTE	nSubsysID;
	WORD	wCmdCodeWriteFile;
	DWORD	dwFileDescripter;
	DWORD	dwOffset;
	DWORD	dwBytesWritten;
	INT		nErrNum;
}DIAGEFSWriteCmdResponse;

typedef struct 
{
	BYTE	nDIAGcmd;
	BYTE	nSubsysID;
	WORD	wCmdCodeCloseFile;
	DWORD	dwFileDescripter;
}DIAGEFSCloseFileCmdRequest;

typedef struct 
{
	BYTE	nDIAGcmd;
	BYTE	nSubsysID;
	WORD	wCmdCodeCloseFile;
	DWORD	dwErrNum;
}DIAGEFSCloseFileCmdResponse;

typedef struct 
{
	BYTE	nDIAGcmd;
	BYTE	nSubsysID;
	WORD	wCmdMkDir;
	WORD	wCreateMode; 
	BYTE	nPath[MAX_PATH];
}DIAGEFSMkDirCmdRequest;

typedef struct 
{
	BYTE	nDIAGcmd;
	BYTE	nSubsysID;
	WORD	wCmdMkDir;
	INT		nErrNum;
}DIAGEFSMkDirCmdResponse;

#pragma pack(pop)

