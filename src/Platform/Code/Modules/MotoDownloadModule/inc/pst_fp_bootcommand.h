#ifndef BOOT_COMMAND_H
#define BOOT_COMMAND_H

//--------------------------------------------------------------------------------------------------
//                                                                               
//   Header Name: PST_FP_BootCommand.h
//
//   General Description: This contains all variable definitions and constants for
//                        sending Boot command to SU.
//   
//
//--------------------------------------------------------------------------------------------------
//
//                            Motorola Confidential Proprietary
//                       Advanced Technology and Software Operations
//                    (c) Copyright Motorola 2003, All Rights Reserved
//   
//
// Revision History:
//                             Modification     Tracking
// Author                          Date          Number     Description of Changes
// -------------------------   ------------    ----------   ----------------------------------------
// Manny Roxas                 04/06/2000      CSGceXXXXX   Initial version.
// Manny Roxas                 05/11/2000      CSGce57852   Fagan Inspection updates: pst_fp_
//                                                          flashdevapi and include files.
// Manny Roxas                 05/16/2000      CSGce58304   Changes per Fagan Review of pst_fp_
//                                                          flashdeviceapi.cpp.
// Manny Roxas                 05/16/2000      CSGce58394   Fagan Inspection Update: PST_FP_
//                                                          BootHandler.h and PST_FP_BootHander.cpp.
// Manny Roxas                 06/07/2000      CSGse60781   Add a request of RAM Downloader for
//                                                          Flash Bootloader checksum.
// Manny Roxas                 06/09/2000      CSGse61381   Change RAMLOAD_WAIT_TIME to 500 millisec.
// Manny Roxas                 06/16/2000      CSGse62291   Change SUinfo init values from 0 to 0xFF.
// Manny Roxas                 07/11/2000      CSGce60032   PST Operation via RS232 transport.
// Manny Roxas                 07/24/2000      CSGce61037   Add SendFlashTestCmd() function.
// Manny Roxas                 09/15/2000      CSGce72792   Fix flashing of naked phones.
// Mark Jones                  10/05/2000      CSGce75600   Increase the delay after sending RAM 
//                                                          Downloader.
// Marc Risop                   5/8/2001        CSGce86207  Add Language package flashing capability
//
// Mike Doehler                 6/28/2001      LIBaa06187   Flash/Flex support
//
// Mike Doehler                06/01/2002      LIBbb28289   Adding new checksum command (checksum over a range)
//
// Marc Risop                  08/14/2002      LIBbb37946   Adding Sirfs_up command
// Mike Zarletti			   05/05/2003	   LIBbb63862   Add definitions for RQSW command
//
// Dennis Kim					09/26/2003		LIBdd05867	Add support for HAB.
// Marc Risop                  09/24/2003      LIBdd42886   Adding RQSF, MADDR, MBIN and RSSF commands.
//
// Dindo Jao                   10/10/2003      LIBdd40378   Revoved definition UINT16. Also defined in WSD_StdTypes.h header file.
//                                                          Include WSD_StdTypes.h header file.
//
// Zhongwei Qiu                09/22/2004      LIBff19413   Support for simulator with a device id greater than 5000
//
// Tiago Armani                 11/17/2004      LIBee23995  Added CONFIG support for argon+ processors
//
// Dindo Jao                   01/24/2005      LIBff50297   Added wait ack response.
//
// Satya Calloji               03/31/2005      LIBff71882   Reduced the RAM DL wait Timeout for RQVN issue
// Tiago Armani                05/05/2005      LIBff30411   Added intel bulverde command strings
// Tiago Armani                07/25/2005      LIBgg35216   Some defines were reallocated to here
//
// Satya Calloji               08/29/2005      LIBgg77383   Added ODM flash support
//
// Satya Calloji			   11/10/2005	   LIBgg26534   Sending PROD Command	
//
// Samir Khial                 03/20/2006      LIBgg77383   Rework ODM Naked/Reflash support  
// Mike Zarletti			   06/15/2006	   LIBgg31593	Added defines for RQPI command
// Satya Calloji               07/21/2006      LIBjj52074   RDL Waitime dynamic assignment
// Samir Khial                 08/08/2006      LIBii76569   Add D-program multi-interface support
// Satya Calloji               09/29/2006      LIBjj80854   Norman changes
//
// Stefan Jez                  12/18/2006      LIBkk83083   Changes needed for TI OMAP naked flash
// Norton Santos               08/07/2007      LIBff89694   Old PST related code removal
// Tingting Dong			   07/17/2007		LIBmm76870  Add RQHSTAT command sending
// Karthik Ramachandran		   10/27/2007      LIBnn01226   Added BP_PASS command
//--------------------------------------------------------------------------------------------------
//                                       INCLUDE FILES
//--------------------------------------------------------------------------------------------------
#include "PST_FP_Shared.h"

//--------------------------------------------------------------------------------------------------
//                                         CONSTANTS
//--------------------------------------------------------------------------------------------------

//----------------------------//
//  Intel command strings     //
//  (used only for error      //
//    reporting)              //
//----------------------------//

#define INTEL_VERSION_CMD_STR       "INTEL_BLV_VERSION_REQUEST"     // string to be displayed in error messages
#define INTEL_START_ADDRS_CMD_STR   "INTEL_BLV_START_ADDRESS"       // string to be displayed in error messages
#define INTEL_DATA_CMD_STR          "INTEL_BLV_DATA_PACKET"         // string to be displayed in error messages
#define INTEL_JUMP_CMD_STR          "INTEL_BLV_JUMP"                // string to be displayed in error messages
#define INTEL_PREAMBLE_STR          "INTEL_BLV_PREAMBLE"            // string to be displayed in error messages

//----------------------------//
//  TI commands               //
//----------------------------//
#define TI_LEN_CMD          0xAAFF5502
#define TI_ADDR_CMD         0x55AAFF08
#define TI_DATA_CMD         0xFFAA5501
#define TI_RUN_CMD          0xFF55AA20
#define TI_PLAT_CMD         0x0000AAF0
#define TI_ACK_CMD          0xFF555580 
#define TI_NACK_CMD         0xFF0000FF 

//----------------------------//
//  TI command strings        //
//  (used only for error      //
//    reporting)              //
//----------------------------//
#define TI_LEN_CMD_STR              "TI_LENGTH"         // string to be displayed in error messages
#define TI_ADDR_CMD_STR             "TI_ADDRESS"        // string to be displayed in error messages
#define TI_DATA_CMD_STR             "TI_DATA"           // string to be displayed in error messages
#define TI_RUN_CMD_STR              "TI_RUN"            // string to be displayed in error messages
#define TI_PLAT_CMD_STR             "TI_PLAT"           // string to be displayed in error messages
#define TI_ACK_CMD_STR              "TI_ACK"            // string to be displayed in error messages
#define TI_NACK_CMD_STR             "TI_NACK"           // string to be displayed in error messages

//----------------------------//
//  ROM Bootstrap constants   //
//----------------------------//

// 64K byte max
#define RAM_BOOTSTRAP_MAX_SIZE  0xFFFF

const BYTE PREAMBLE_VALUE[4] = {0x55, 0x55, 0x55, 0xAA};

//----------------------------//
//  BOOT Command Send to SU   //
//----------------------------//

#define ERASE_CMD          "ERASE"
#define ADDR_CMD           "ADDR"
#define BIN_CMD            "BIN"
#define BAUD_CMD           "BAUD"
#define JUMP_CMD           "JUMP"
#define POWER_DOWN_CMD     "POWER_DOWN"
#define RQ_CKSUM_CMD       "RQCS"
#define RQ_HW_VERSION_CMD  "RQHW"
#define RQ_RAM_CKSUM_CMD   "RQRC"
#define RANGE_CHECKSUM_CMD "RQRCS"
#define RQ_SERIAL_NUM_CMD  "RQSN"
#define CONFIG_CMD         "CONFIG"
#define RQ_SW_VERSION_CMD  "RQVN"
#define RQ_FLASH_VRSN_CMD  "RQSW"
#define REINSERT_BARKER    "RBRK"
#define FDI_UPGRADE_CMD    "UPGRADE"
#define RESTART_COMMAND    "RESTART"
#define SIRFS_UP_COMMAND   "SIRFS_UP"
#define REQUEST_FDI_STATUS "REQ_STAT"
#define SAVE_FDI_DATA      "SAVE"
#define ERASE_FDI_AREA     "ERASE_FDI"
#define REQUEST_FDI_CS     "REQ_FDI_CS"
#define RESTORE_FDI        "RESTORE"
#define MADDR_CMD          "MADDR"
#define MBIN_CMD           "MBIN"
#define RQSF_CMD           "RQSF"
#define PROD_CMD		   "PRDCT"
#define RQ_PDS_INFO_CMD	   "RQPI"
#define ENUM_CMD           "ENUM"
#define RQHSTAT_CMD        "RQHSTAT"
#define BP_PASS			   "BP_PASS"
//----------------------------//
//  TI ROM Commands Send to SU   //
//----------------------------//
#define TI_SIGNAL_CMD     "SIGNALING"
#define TI_PARAM_CMD      "PARAMETER"
#define TI_WRITE_CMD      "WRITE"
#define TI_ABORT_CMD      "ABORT"
#define TI_CERT_CMD       "CERTFICATE"

//---------------------//
//  Response from SU   //
//---------------------//

#define ACK_RESP            "ACK"
#define ACK_RESP_LEN        3
#define ERR_RESP            "ERR"
#define ERR_RESP_LEN        3
#define ERASE_RESP          "ERASE"
#define ERASE_RESP_LEN      5
#define CHECKSUM_RESP       "RSCS"
#define CHECKSUM_RESP_LEN   4
#define HW_VERSION_RESP     "RSHW"
#define HW_VERSION_RESP_LEN 4
#define SW_VERSION_RESP     "RSVN"
#define SW_VERSION_RESP_LEN 4
#define FLASH_VRSN_RESP     "RSSW"
#define FLASH_VRSN_RESP_LEN 4
#define RAM_CKSUM_RESP      "RSRC"
#define RAM_CKSUM_RESP_LEN  4
#define SERIAL_NUM_RESP     "RSSN"
#define SERIAL_NUM_RESP_LEN 4
#define RQHSTAT_RESP        "RSHSTAT"
#define RQHSTAT_RESP_LEN	7
#define RQHSTAT_MODE_LEN    4
#define RQHSTAT_DATALEN_LEN 2
#define RQHSTAT_RESULT_LEN  1
#define RQHSTAT_RECORD_LEN	3
#define RQHSTAT_RECLEN_LEN	1
#define RQHSTAT_DEVTYPE_LEN 1
#define RQHSTAT_MAX_RECORDS	30
#define RQHSTAT_REC_LOC		5
// CSGce60032 - MNR
#define BAUD_RESP           "BAUD"
#define BAUD_RESP_LEN       4
#define RQSN_RESP			"RSSN"
#define RQSN_RESP_LEN		4
#define RQPI_RESP			"RSPI"
#define RQPI_RESP_LEN		4


//responses to UPGRADE/REQ_STAT
#define UPGRADE_CMD_RS      "RSUPGRADE"
#define FDI_NOT_UPGRADEABLE "NOT_UPGRADEABLE"
#define FDI_UPGRADEABLE     "UPGRADEABLE"
#define FDI_UPGRADED        "UPGRADED"

//response to UPGRADE/REQ_FDI_CS
#define FDI_CHECKSUM_RS     "RSUPGRADECS"

//response to request for a checksum over a specified range
#define RANGE_CHECKSUM_RS   "RSRCS"

//response to RQSF
#define RSSF_RESP           "RSSF"
#define RSSF_RESP_LEN       4
#define RSSF_NUM_ADDRS      1  //it's really a nibble
#define NUM_ADDRS_ASCII_LEN 1  //one ascii byte

//response to CONFIG
#define CONFIG_RESP         "CONFIG"
#define CONFIG_RESP_LEN     6
#define WAIT_RESP           "WAIT"
#define WAIT_RESP_LEN		4

// response to ENUM
#define ENUM_RESP           "ENUM"
#define ENUM_RESP_DELIM     ","
#define ENUM_RESP_LEN       4
#define ENUM_STATE_LEN      8

#define RS_BYTE             1
#define W_BYTE              1 //length of one byte
#define ASCII_COMMA         0x2C

//-----------------------//
//  Control Characters   //
//-----------------------//

#define STX_CTL   0x02
#define ETX_CTL   0x03
#define RS_CTL    0x1E
#define PAD_CTL   0x07

#define STX_ETX_LEN  2

//-------------------------//
//  Address and checksum   //
//-------------------------//

// ASCII Address and checksum
#define ADDRESS_ASCII_LEN 8
#define CHKSUM_ASCII_LEN  2
#define ADDRESS_LEN (ADDRESS_ASCII_LEN + CHKSUM_ASCII_LEN)
// CSGce60781 - MNR
// 2 8-byte ASCII stard and end address
// and 1 byte of COMMA character.
#define RQCS_ASCII_ADDRESS_LEN  17

#define CHECKSUM_MASK    0xFF

//-------------------//
//  Retries values   //
//-------------------//

#define CMD_NO_RETRY                        0

#define CMD_DEFAULT_SMALL_OVERALL_RETRIES   1
#define INTEL_CMD_DEFAULT_OVERALL_RETRIES   3       // maximum number of retries to send a command

#define CMD_DEFAULT_OVERALL_RETRIES         3
#define CMD_DEFAULT_RECEIVE_RETRIES         3
#define CMD_DEFAULT_SEND_RETRIES            CMD_NO_RETRY

#define CMD_DEFAULT_BIG_RECEIVE_RETRIES     5


//-------------------//
//  Timeout values   //
//-------------------//

#define ONE_SECOND            1000 //in ms
#define INTEL_DEFAULT_TIMEOUT 6    // timeout in seconds (for ReadPacket())
#define TWENTY_SECOND_TIMEOUT 20
#define TWO_MINUTE_TIMEOUT    120
#define FIVE_SECOND_TIMEOUT   5
#define BOOTHANDLER_READ_WRITE_MAX_TIMEOUT (WORD)60	//framework waits 60 seconds before returning an error

// general erase can be much longer than a mere 60 seconds
// LCA experiences 2+ minutes, 8MB could take nearly that
// long if a significant amount of flash had to be erased
// and newer 16MB flash parts would take much longer.
// Spec for Whitecliff 16MB says 180.9 S typical for all
// 16MB.  Max timing for that part is 1,037.7 S.  Since
// the parts have generally behaved such that erase takes
// the spec typical or better time to erase, 10% above that
// should be reasonable (180.9*1.10 = 198.99 ~= 200).  Denali 
// stacked 32MB parts will take double the 200, or 400sec.

// increased time out from 400 to 600 to support Bulverde
#define ERASE_READ_WRITE_MAX_TIMEOUT  (WORD)600; /* 600 second wait max */

#define LONG_WAIT_TIMEOUT 3600 // one hour timeout



//---------------------//
//  Packet constants   //
//---------------------//

// 4K bytes maximum packet the USB drive can handle per transfer
#define MAX_PACKET_SIZE  4096

#define MAX_READ_BUFFER_SIZE 1024

//----------------//
// VERSION TOKENS //
//----------------//

#define RUNNING_BOOT_VER_TOKEN    0
#define FLASHBOOTLOADER_VER_TOKEN 1
#define SW_PACKAGE_VER_TOKEN      2
#define HEADER_PACKAGE_VER_TOKEN  3
#define MAIN_CODE_VER_TOKEN       4
#define FDI_VER_TOKEN             5

#define MAX_VERSION_TOKEN         6

//-----------------//
// CHECKSUM TOKENS //
//-----------------//

#define FLASHBOOTLOADER_CHKSUM_TOKEN  0
#define HEADER_PACKAGE_CHKSUM_TOKEN   1
#define MAIN_CODE_CHKSUM_TOKEN        2
#define FDI_CHKSUM_TOKEN              3

#define MAX_COMMON_CHKSUM_TOKEN   4
#define MAX_CHKSUM_STRING_LEN     4

//-------------------//
// ERASE DATA VALUES //
//-------------------//

#define GENERAL_ERASE 0x80
#define LANG_PACKAGE_ERASE 0x02
//??? Not sure about this one yet
#define UPID_ERASE    0x00

//----------------//
//  Miscellaneous //
//----------------//

#define HW_VERSION_LEN  8
#define SW_VERSION_LEN  8

// MNR-ce62291
#define SU_INFO_INIT_VALUE  0xFF

//--------------------------------------------------------------------------------------------------
//                                          MACROS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                           ENUMS
//--------------------------------------------------------------------------------------------------

//FDI command response type enum
typedef enum
{
    UnknownCommand  = 0x0C,
    FDI_Upgradeable = 0x01,
    FDI_NotUpgradeable = 0x02,
    FDI_Upgraded = 0x03,
    UnrecognisedCommand = 0x04
}RESPONSE_TYPE;



//--------------------------------//
// HEX enumeration for conversion //
//--------------------------------//
enum HexByteEnum
{
    OneHexByte = 1,
    FourHexBytes = 4,
    EightHexBytes = 8,
	SixteenHexBytes = 16
};

//--------------------------------------------------------------------------------------------------
//                               STRUCTURES AND OTHER TYPEDEFS
//--------------------------------------------------------------------------------------------------

//  ROM Bootstrap
typedef struct {
    BYTE byPreamble[4];
    BYTE byDestAddr[4];
    BYTE byDataLen[2];
    WORD wBaudRate;
} RAM_BS_NO_DATA_STRUC;

// Extraction of Header controls
typedef struct {
    BYTE  *pbySrcBuffer;  
    DWORD dwNumOfBytesRead;
    BYTE  *pbyDestBuffer;   
    DWORD *pdwDestIndex;  
    DWORD dwMaxBufLength;  
    BOOL  *pbReadDone;      
} EXT_HDR_CTL_STRUC;

typedef struct EnumRespVal{
    int m_nProcID;
    BYTE  abyPhoneState[ENUM_STATE_LEN]; //null terminated string
    DWORD dwErrSts;

    EnumRespVal()
    {
        memset(abyPhoneState,'\0',sizeof(abyPhoneState));
        m_nProcID = UNKNOWN_PROC_ID;
        dwErrSts = 0;
    };

} ENUM_RESPVALUE;

//--------------------------------------------------------------------------------------------------
//                               GLOBAL VARIABLE DECLARATIONS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                   FUNCTION PROTOTYPES
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                         CLASS
//--------------------------------------------------------------------------------------------------


#endif BOOT_COMMAND_H