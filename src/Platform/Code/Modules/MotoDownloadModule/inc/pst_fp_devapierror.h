#pragma once

#include "PST_FP_BootHandler.h"

#ifndef	DEVICE_API_ERROR_H
#define DEVICE_API_ERROR_H

//--------------------------------------------------------------------------------------------------
//                                                                               
//   Header Name: PST_FP_DevApiError.h
//
//   General Description: This contains all the error constants as reported by Flash Device API DLL.
//	 
//   Each error code is a 32-bit value that is layed out as follows:
//
//   1 0 0 0 7 6 5 4 3 2 1 0 3 2 1 0 7 6 5 4 3 2 1 0 7 6 5 4 3 2 1 0                                                             
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+---------------+--------+------+-----------------------+
//  |Sev|C|R|      Cmd      |Facility|  DIR |      Code             |
//  +---+-+-+---------------+--------+------+-----------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Cmd - is the flash command identifier byte
//
//      Facility - is the facility code
//
//      Code - is the facility's status code -12 Bits
//      
//      DIR - Direction/other details for the error - 4 Bits      
//
//--------------------------------------------------------------------------------------------------
//
//                            Motorola Confidential Proprietary
//                       Advanced Technology and Software Operations
//                    (c) Copyright Motorola 2003 - 2004, All Rights Reserved
//   
//
// Revision History:
//                             Modification     Tracking
// Author                          Date          Number     Description of Changes
// -------------------------   ------------    ----------   ----------------------------------------
// Manny Roxas	               04/06/2000      CSGceXXXXX   Initial version.
// Manny Roxas	               05/16/2000	   CSGce58304   Changes per Fagan Review of pst_fp_
//                                                          flashdeviceapi.cpp.
// Manny Roxas	               05/16/2000	   CSGce58394   Fagan Inspection Update: PST_FP_
//                                                          BootHandler.h and PST_FP_BootHander.cpp.
// Manny Roxas                 06/07/2000      CSGse60781   Add a request of RAM Downloader for
//                                                          Flash Bootloader checksum.
// Manny Roxas                 06/08/2000      CSGse61035   Add support to process checksums and
//                                                          versions for all SW code groups.
// Manny Roxas                 07/11/2000      CSGce60032   PST Operation via RS232 transport.
// Manny Roxas                 07/24/2000      CSGce61037   Add SendFlashTestCmd() function.
// Manny Roxas                 09/15/2000      CSGce72792   Fix flashing of naked phones.
//
// Mike Doehler                 6/28/2001      LIBaa06187   Flash/Flex support
// Mike Zarletti			   05/05/2003	   LIBbb63862	Added errors for RQSW support
// Marc Risop                  09/24/2003      LIBdd42886	Added errors for RQSF support
// Marc Risop                  05/27/2004      LIBee05617   Changing BT_PST_HNDL_INITHANDLER_ERR to BT_ADAPTER_PTR_INITHANDLER_ERR
// Dindo Jao                   11/10/2004      LIBee41450   Added error codes and command related error codes.
// Tiago Armani                11/17/2004      LIBee23995   Added CONFIG support for argon+ processors
// Dindo Jao                   12/14/2004      LIBee29396   Alligned the command string array by adding a blank string.
//
// Satya Calloji               12/23/2004      LIBff47956   Added Error Handling changes
//
// Satya Calloji               01/28/2005      LIBff57828   Changed Hex Values to Decimal for GNPO usage
//
// Satya Calloji               05/16/2005      LIBgg11794   Corrected the error strings for commands
// Tiago Armani                05/05/2005      LIBff30411   Added error codes for intel bulverde functions
//
// Tiago Armani                06/15/2005      LIBgg13321   Moved some SU error constants from pst_fp_shared.h to here
//
// Satya Calloji               11/10/2005      LIBgg26534   Sending PROD Command
//
// Satya Calloji               12/02/2005      LIBhh42127   Fix the negative error number issue in Nextest
//
// Norton Santos               12/06/2005      LIBgg24542   Added methods in CFlashErr
//
// Norton Ssntos               02/09/2006      LIBhh78751   Changed CFlashErr attribute m_bLock from private to protected
//
// Satya Calloji               08/29/2005      LIBgg77383   Added ODM flash support
//
// Samir Khial                 03/20/2006      LIBgg77383   Rework ODM Naked/Reflash support 
//
// Mike Zarletti               06/15/2006      LIBgg31593   Added error codes for RQPI command
//
// Samir Khial                 07/03/2006      LIBii76588   Add ETM test command interface support
//
// Samir Khial                 08/01/2006      LIBjj44900   Fix PST init issue
//
// Samir Khial                 08/07/2006      LIBjj52430   Handle Hab error correctly
//
// Fathi Ben Amar              09/18/2006      LIBii99372   Reorganization of include file 
//
// Satya Calloji               09/06/2006      LIBii58143   Handle Sibley Errors
//
// Satya Calloji               09/28/2006      LIBjj80854   Norman Changes
//
// Samir Khial                 12/19/2006      LIBkk61289  Fixed CDMA-D multi-up issue
//
// Stefan Jez                  12/22/2006      LIBkk83083   Added more TI error codes
//
// Tingting Dong				07/17/2007		LIBmm76870  Add RQHSTAT command sending
//
// Karthik Ramachandran		   10/27/2007      LIBnn01226   Added BP_PASS command support for GreatWall
//--------------------------------------------------------------------------------------------------
//                                       INCLUDE FILES
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                         CONSTANTS
//--------------------------------------------------------------------------------------------------


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//
// ERRORS Detected by the Boot Handler class methods.
//
//   Using Facility code of 0 the error code ranges from 0xE0000000 to 0xE000FFFF.
//  
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

// More than one STX character received on ExtractHdrControl() call.
#define BT_MULTIPLE_STX_EXTHDRCNTL_ERR       0xE0000000

// Application buffer is not large enough to store data on ExtractHdrControl() call.
#define BT_BUFFER_TOO_SMALL_EXTHDRCNTL_ERR   0xE0000001

// Timeout occurred while waiting for RAM Downloader response on CheckRAMdownload() call.
#define BT_TMOUT_CHKRAMLOAD_ERR              0xE0000010

// Invalid response by RAM Downloader on CheckRAMdownload() call.
#define BT_INVALID_RESP_CHKRAMLOAD_ERR       0xE0000011

// Null input pointer/s on WritePacket() call.
#define BT_NULL_PTR_WRITEPACKET_ERR          0xE0000020

// Null Interface handle on WritePacket() call.
#define BT_INTERFACE_HNDL_WRITEPACKET_ERR    0xE0000021

// Null FlashAdapter pointer on InitHandler() call.
#define BT_ADAPTER_PTR_INITHANDLER_ERR          0xE0000030

// Device ID is 0 on InitHandler() call.
#define BT_DEVICE_ID_INITHANDLER_ERR         0xE0000031

// ce72792 - MNR
// Interface device handle is NULL on InitHandler() call.
#define BT_INTF_HANDLE_INITHANDLER_ERR       0xE0000032

// Null input pointer/s on ReadPacket() call.
#define BT_NULL_PTR_READPACKET_ERR           0xE0000040

// Null Interface handle on ReadPacket() call.
#define BT_INTERFACE_HNDL_READPACKET_ERR     0xE0000041

// No STX control found on ReadPacket() call.
#define BT_NO_STX_READPACKET_ERR             0xE0000042

// Null input pointer/s on SendRAMbootstrap() call.
#define BT_NULL_PTR_SENDRAMBS_ERR            0xE0000050

// Invalid data length on SendRAMbootstrap() call.
#define BT_INVALID_LEN_SENDRAMBS_ERR         0xE0000051

// Cannot allocate memory on SendRAMbootstrap() call.
#define BT_MEM_ALLOC_SENDRAMBS_ERR           0xE0000052

// Null input pointer/s on SendSUaddr() call.
//???#define BT_NULL_PTR_SENDSUADDR_ERR           0xE0000060

// Null input pointer/s on SendCommandData() call.
#define BT_NULL_PTR_SENDCMDDATA_ERR          0xE0000070

// Cannot allocate memory on SendCommandData() call.
#define BT_MEM_ALLOC_SENDCMDDATA_ERR         0xE0000071

// Timeout occurred on SendCommandData() call.
#define BT_TIMEOUT_SENDCMDDATA_ERR           0xE0000072

// Invalid response on SendCommandData() call.
#define BT_INVALID_RESP_SENDCMDDATA_ERR      0xE0000073

// Null input pointer/s on IntelReadData() call.
#define BT_NULL_PTR_INTELREADDATA_ERR        0xE0000080

// Null Interface handle on IntelReadData() call.
#define BT_INTERFACE_HNDL_INTELREADDATA_ERR  0xE0000081

// Null input pointer/s on IntelWriteData() call.
#define BT_NULL_PTR_INTELWRITEDATA_ERR       0xE0000090

// Null Interface handle on IntelWriteData() call.
#define BT_INTERFACE_HNDL_INTELWRITEDATA_ERR 0xE0000091

// Incorrect baudrate in Param request
#define BT_INCORRECT_BAUD_RATE_ERR           0xE0000092

// Incorrect certificate in Param request
#define BT_INCORRECT_CERT_ERR                0xE0000093

// Incorrect code address in Param request
#define BT_INCORRECT_CODE_ADDR_ERR           0xE0000094

// Address error while flashing RDL
#define BT_INCORRECT_ADDR_ERR                0xE0000095

// Bad block size while flashing RDL
#define BT_INCORRECT_BADBLOCK_ERR            0xE0000096

// First block address does not correspond to code address specified in certificate
#define BT_INCORRECT_INCORR_ADDR_ERR         0xE0000097

// Firmware signature error in case of HS device and hash error in case of Locosto GP device
#define BT_INCORRECT_FIRMWARE_ERR            0xE0000098

// Received code size does not correspond to code size specified in certificate/TOC
#define BT_INCORRECT_CODESIZE_ERR            0xE0000099

// Error during hashing of the firmware
#define BT_INCORRECT_HASH_ERR                0xE000009A

// Error while formatting TI request packet
#define BT_INCORRECT_ODM_MESG_FORMAT         0xE000009B

// Error while parsing TI response packet
#define BT_INCORRECT_MSG_RESP                0xE000009C

// Unable to allocate memory on the heap
#define BT_HEAP_MEMORY_ALLOC_ERR             0xE000009D

// Unable to recognize connected ODM device
#define BT_UNKNOWN_ODM_DEVICE_ERR            0xE000009E

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//
// ERRORS Detected by the Flash Device API class methods.
//
//   Using Facility code of 1 the error code ranges from 0xE0010000 to 0xE001FFFF.
//  
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

// Null input pointer/s on ConvertStringToHex() call.
//???#define API_NULL_PTR_CONVERTSTRING_ERR        0xE0010000

// String too long on ConvertStringToHex() call.
#define API_STRING_LEN_CONVERTSTRING_ERR      0xE0010001

// String in ODD length on ConvertStringToHex() call.
#define API_ODD_STRING_LEN_CONVERTSTRING_ERR  0xE0010002

// Undefined version string on ConvertStringToHex() call.
#define API_VERSION_STRING_CONVERTSTRING_ERR  0xE0010003

// Cannot convert ASCII char to HEX on ConvertStringToHex() call.
#define API_CONVERT_CHAR_CONVERTSTRING_ERR    0xE0010004

// Null status pointer on RAMdownload() call.
//???#define API_NULL_STATUS_PTR_RAMDOWNLOAD_ERR   0xE0010010

// Null data pointer on RAMdownload() call.
#define API_NULL_DATA_PTR_RAMDOWNLOAD_ERR     0xE0010011

// Data length 0 on RAMdownload() call.
#define API_INVALID_LEN_RAMDOWNLOAD_ERR       0xE0010012

// Null input pointer/s on FlashSU() call.
#define API_NULL_PTRS_FLASHSU_ERR             0xE0010020

// Lenght is 0 on FlashSU() call.
#define API_INVALID_LEN_FLASHSU_ERR           0xE0010021

// Null input pointer/s on EraseSU() call.
//???#define API_NULL_PTR_ERASESU_ERR              0xE0010022

// Invalid erase type on EraseSU() call.
#define API_ERASE_TYPE_ERASESU_ERR            0xE0010023

// Invalid erase response on EraseSU() call.
#define API_ERASE_RESP_ERASESU_ERR            0xE0010024

// Timed out on EraseSU() call.
#define API_TIMED_OUT_ERASESU_ERR             0xE0010025

// Null input pointer/s on GetSUchkSum() call.
#define API_NULL_PTRS_GETCHKSUM_ERR           0xE0010030

// Timed out on GetSUchkSum() call.
#define API_TIMED_OUT_GETCHKSUM_ERR           0xE0010031

// Invalid checksum response on GetSUchkSum() call.
#define API_INVALID_RESP_GETCHKSUM_ERR        0xE0010032

// No checksum data response on GetSUchkSum() call.
#define API_NO_DATA_GETCHKSUM_ERR             0xE0010033

// Null input pointer/s on GetSUinfo() call.
#define API_NULL_PTRS_GETSUINFO_ERR           0xE0010040

// Hardware version request timed out on GetSUinfo() call.
#define API_RQHW_TIMED_OUT_GETSUINFO_ERR      0xE0010041

// Invalid hardware version response on GetSUinfo() call.
#define API_RQHW_RESPONSE_GETSUINFO_ERR       0xE0010042

// No hardware version data response on GetSUinfo() call.
#define API_NO_HW_DATA_GETSUINFO_ERR          0xE0010043

// Software version request timed out on GetSUinfo() call.
#define API_RQVN_TIMED_OUT_GETSUINFO_ERR      0xE0010044

// Invalid software version response on GetSUinfo() call.
#define API_RQVN_RESPONSE_GETSUINFO_ERR       0xE0010045

// No software version data response on GetSUinfo() call.
#define API_NO_SW_DATA_GETSUINFO_ERR          0xE0010046

// Null input pointer/s on GetBootFlashFlexInfo() call.
#define API_NULL_PTRS_GETBOOTFLASHFLEXINFO_ERR		0xE0010047

// RQSW boot command timed out on GetBootFlashFlexInfo() call.
#define API_RQSW_TIMED_OUT_GETBOOTFLASHFLEXINFO_ERR	0xE0010048

// Invalid RQSW response on GetBootFlashFlexInfo() call.
#define API_RQSW_RESPONSE_GETBOOTFLASHFLEXINFO_ERR	0xE0010049

// No flash or flex version data in response to RQSW in GetBootFlashFlexInfo() call.
#define API_NO_SW_DATA_GETBOOTFLASHFLEXINFO_ERR		0xE0010050

// RQSW response format error in GetBootFlashFlexInfo() call.
#define API_RQSW_RESPONSE_FORMAT_GETBOOTFLASHFLEXINFO_ERR	0xE0010051 

//Invalid RSSF response from GetNumFlashParts()
#define API_RSSF_RESPONSE_GETNUMFLASHPARTS_ERR  0xE0010052
#define API_RSSF_NO_DATA_IN_RESPONSE_ERR        0xE0010053
#define API_RSSF_NOT_ENOUGH_ADDRESS_INFO_ERR    0xE0010054

// RQSN command timed out
#define API_TMOUT_RQSN_ERR 0xE0010055
#define API_INVALID_RQSN_RESPONSE_ERR 0xE0010056
#define API_NO_RQSN_DATA_ERR 0xE0010057

// FDI Upgrade command-Request FDI Status
#define API_TIMED_OUT_REQ_STAT_ERR 0xE0010058
#define API_INVALID_RESP_REQ_STAT_ERR 0xE0010059
#define API_NO_DATA_REQ_STAT_ERR 0xE001005A

// FDI Upgrade command-Request FDI CS
#define API_TIMED_OUT_REQ_CS_ERR 0xE001005B
#define API_INVALID_RESP_REQ_CS_ERR 0xE001005C
#define API_NO_DATA_REQ_CS_ERR 0xE001005D

// Request range checksum
#define API_TIMED_OUT_RANGE_CKSUM_ERR 0xE001005E
#define API_INVALID_RESP_RANGE_CKSUM_ERR 0xE001005F
#define API_NO_DATA_RANGE_CKSUM_ERR 0xE0010060


//Intel Bulverde errors
#define API_IB_NOT_INITIALIZED_ERR  0xE0010061 // when trying to execute intel command
                                               // without initializing
#define API_IB_INVALID_ANSWER_ERR   0xE0010062 // invalid answer for command
#define API_IB_RETURNED_ERR         0xE0010063 // error returned from SU
#define API_IB_PACKET_OVERFLOW_ERR  0xE0010064 // packet is bigger than supported
#define API_IB_PDATA_NULL_ERR       0xE0010065 // pointer to data is null

//TI errors
#define API_TI_PDATA_NULL_ERR       0xE0010066 // pointer to data is null

// Flash Bootloader checksum string too long on ExtractChecksums() call.
#define API_FLASH_BOOTLOADER_EXTCHKSUM_ERR    0xE0010070

// Header package checksum string too long on ExtractChecksums() call.
#define API_HEADER_PACKAGE_EXTCHKSUM_ERR      0xE0010071 //? not used

// Main code checksum string too long on ExtractChecksums() call.
#define API_MAIN_CODE_EXTCHKSUM_ERR           0xE0010072 //? not used

// FDI checksum string too long on ExtractChecksums() call.
#define API_FDI_CHKSUM_EXTCHKSUM_ERR          0xE0010073 //? not used

// CDMA character ROM checksum string too long on ExtractChecksums() call.
#define API_CDMA_CHAR_ROM_EXTCHKSUM_ERR       0xE0010074 //? not used

// GSM Language pack checksum string too long on ExtractChecksums() call.
#define API_GSM_LANG_PACK_EXTCHKSUM_ERR       0xE0010075 //? not used

//-- CSGce61035 upto
// Invalid SW code group checksum on ExtractChecksums() call.
#define API_CODE_GROUP_EXTCHKSUM_ERR          0xE0010076

// Too many checksums returned by SU on ExtractChecksums() call.
#define API_TOO_MANY_CHKSUM_EXTCHKSUM_ERR     0xE0010077
//-- end CSGce61035

//-- CSGce60781 upto
// Checksum pointer is NULL on GetRAMchecksum() call.
#define API_NULL_PTRS_GETRAMCKSUM_ERR         0xE0010080

// RAM checksum request timed out on GetRAMchecksum() call.
#define API_TIMED_OUT_GETRAMCKSUM_ERR         0xE0010081

// Invalid RAM checksum response on GetRAMchecksum() call.
#define API_INVALID_RESP_GETRAMCKSUM_ERR      0xE0010082

// No RAM checksum data response on GetRAMchecksum() call.
#define API_NO_DATA_GETRAMCKSUM_ERR           0xE0010083
//-- end CSGce60781

// BAUD request timed out on SetBaudRate() call.
#define API_TIMED_OUT_SETBAUDRATE_ERR         0xE0010090

// Invalid BAUD response on SetBaudRate() call.
#define API_INVALID_RESP_SETBAUDRATE_ERR      0xE0010091

// Timeout sending the config cmd
#define API_TMOUT_SENDCONFIG_ERR              0xE0010100

// Invalid response from config cmd
#define API_INVALID_RESP_CONFIGDATA_ERR       0xE0010101

#define API_INVALID_RESP_PROD_CMD_ERR         0xE0010102

#define API_INVALID_ERR_STATUS_PARAM          0xE0010103

#define API_NULL_PTRS_CONFIG_ERR              0xE0010104

// Error codes for Get PDS Info.
#define API_NULL_PTRS_GETBOOTPDSINFO_ERR	  0xE0010105
#define API_RQPI_TIMED_OUT_GETBOOTPDSINFO_ERR 0xE0010106
#define API_RQPI_RESPONSE_GETBOOTPDSINFO_ERR  0xE0010107
#define API_MEM_ALLOCATION_GETBOOTPDSINFO_ERR 0xE0010108
#define API_NO_SW_DATA_GETBOOTPDSINFO_ERR     0xE0010109
#define API_RQPI_RESP_FORMAT_GETBOOTPDSINFO_ERR 0xE001010A

// ENUM response format error in SendEnumCommand() call.
#define API_ENUM_WRONG_RESPONSE_ERR	          0xE001010B

// RQHSTAT command timed out
#define API_RQHSTAT_TMOUT_ERR				  0xE0010110
#define API_RQHSTAT_INVALID_RESPONSE_ERR	  0xE0010111
#define API_RQHSTAT_NO_DATA_ERR				  0xE0010112
#define API_RQHSTAT_NULL_PTRS				  0xE0010113 

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//
// ERRORS returned by the SU.  Since the error is only a byte then it is ORed with
// the rest of the bits.
//
//   Using Facility code of 2 the error code ranges from 0xE0020000 to 0xE002FFFF.
//  
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

#define SU_DETECTED_ERROR                   0xE0020000

#define UNRECOGNIZED_COMMAND                0xE0020085

#define SIBLEY_FILE_TYAX_FLASH_MISMATCH     0xE00200BA
#define TYAX_FILE_SIBLEY_FLASH_MISMATCH     0xE00200B9
#define TYAX_FILE_MISMATCH                  0xE00200B8
#define MICRON_FILE_MISMATCH                0xE00200B7


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//
// ERRORS returned by PST methods.  It is ORed with the rest of the bits.
//
//   Using Facility code of 3, the error code ranges from 0xE0030000 to 0xE003FFFF.
//  
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

enum ECMD_ERR_LIST
{
   ERR_ERASE        =  0x00100000,  //ERASE_CMD          "ERASE"
   ERR_ADDR         =  0x00200000,  //ADDR_CMD           "ADDR"
   ERR_BIN          =  0x00300000,  //BIN_CMD            "BIN"
   ERR_BAUD         =  0x00400000,  //BAUD_CMD           "BAUD"
   ERR_JUMP         =  0x00500000,  //JUMP_CMD           "JUMP"
   ERR_POWER_DOWN   =  0x00600000,  //POWER_DOWN_CMD     "POWER_DOWN"
   ERR_RQ_CKSUM     =  0x00700000,  //RQ_CKSUM_CMD       "RQCS"
   ERR_RQ_HW_VERSION=  0x00800000,  //RQ_HW_VERSION_CMD  "RQHW"
   ERR_RQ_RAM_CKSUM =  0x00900000,  //RQ_RAM_CKSUM_CMD   "RQRC"
   ERR_RANGE_CHECKSUM= 0x01000000,  //RANGE_CHECKSUM_CMD "RQRCS"
   ERR_RQ_SERIAL_NUM = 0x01100000,  //RQ_SERIAL_NUM_CMD  "RQSN"
   ERR_RQ_SW_VERSION = 0x01200000,  //RQ_SW_VERSION_CMD  "RQVN"
   ERR_RQ_FLASH_VRSN = 0x01300000,  //RQ_FLASH_VRSN_CMD  "RQSW"
   ERR_REINSERT_BARKER=0x01400000,  //REINSERT_BARKER    "RBRK"
   ERR_FDI_UPGRADE    =0x01500000,  //FDI_UPGRADE_CMD    "UPGRADE"
   ERR_RESTART        =0x01600000,  //RESTART_COMMAND    "RESTART"
   ERR_SIRFS_UP       =0x01700000,  //SIRFS_UP_COMMAND   "SIRFS_UP"
   ERR_MADDR          =0x01800000,  //MADDR_CMD          "MADDR"
   ERR_MBIN           =0x01900000,  //MBIN_CMD           "MBIN"
   ERR_RQSF           =0x02000000,  //RQSF_CMD           "RQSF"
   ERR_CONFIG         =0x02100000,  //CONFIG_CMD         "CONFIG"
   ERR_IB_RQ_VERSION  =0x02200000,  //INTEL_VERSION_CMD_STR     "INTEL_BLV_VERSION_REQUEST"
   ERR_IB_ST_ADDRS    =0x02300000,  //INTEL_START_ADDRS_CMD_STR "INTEL_BLV_START_ADDRESS"
   ERR_IB_DATA_PACKET =0x02400000,  //INTEL_DATA_CMD_STR        "INTEL_BLV_DATA_PACKET"
   ERR_IB_JUMP        =0x02500000,  //INTEL_JUMP_CMD_STR        "INTEL_BLV_JUMP"
   ERR_IB_PREAMBLE    =0x02600000,  //INTEL_PREAMBLE_STR        "INTEL_BLV_PREAMBLE"
   ERR_PROD           =0x02700000,  // PROD_COMMAND		"PRDCT"
   ERR_TI_SIGNAL      =0x02800000,  //TI_SIGNAL_CMD     "SIGNALING"
   ERR_TI_PARAM       =0x02900000,  //TI_PARAM_CMD      "PARAMETER"
   ERR_TI_WRITE       =0x03000000,  //TI_WRITE_CMD      "WRITE"
   ERR_TI_ABORT       =0x03100000,  //TI_ABORT_CMD      "ABORT"
   ERR_TI_CERT        =0x03200000,  //TI_CERT_CMD       "CERTFICATE"
   ERR_TI_PLATFORM    =0x03300000,  //TI_PLATFORMTYPE_CMD "TI_PLATFORMTYPE"
   ERR_TI_JUMPRUN     =0x03400000,  //TI_RUN_CMD        "TI_RUN"
   ERR_TI_ADDR        =0x03500000,  //TI_ADDR_CMD       "TI_ADDRESS"
   ERR_TI_LEN         =0x03600000,  //TI_LEN_CMD        "TI_LENGTH"
   ERR_TI_ACK         =0x03700000,  //TI_ACK_CMD       "TI_ACK"
   ERR_RQ_PDS_INFO	  =0x03800000,  //RQ_PDS_INFO_CMD   "RQPI"
   ERR_ENUM           =0x03900000,  //ENUM_CMD          "ENUM"
   ERR_RQHSTAT	      =0x04000000,  //RQHSTAT_CMD  "RQHSTAT"
   ERR_BP_PASS	      =0x04100000,  //BP_PASS           "BP_PASS"
   ERR_CMD_TOTAL      =0x0042 // Total meant only for actual commands. Do not add the four suffix zeros to
                              // this value....
};

enum ENONCMD_ERR_LIST
{   // Non command Values meant for GNPO usage only....
   ERR_NCMD_VALIDATE_CKSUM          =0x04000000,
   ERR_NCMD_GET_HANDLE	            =0x04100000,
   ERR_NCMD_GET_DESCRIPTOR          =0x04200000,
   ERR_NCMD_FILE_NOT_FOR_BLANK      =0x04300000,
   ERR_NCMD_FILE_NOT_FOR_REFLASH    =0x04400000,
   ERR_NCMD_MA_MISMATCH             =0x04500000,
   ERR_NCMD_CHIPSET_MISMATCH        =0x04600000,
   ERR_NCMD_INIT                    =0x04700000,
   ERR_NCMD_APP                     =0x04800000,
   ERR_NCMD_RDL                     =0x04900000,
   ERR_NCMD_SUADDR                  =0x05000000,
   ERR_NCMD_FLSU                    =0x05100000,
   ERR_NCMD_GETSUINFO               =0x05200000
};



//--------------------------------------------------------------------------------------------------
//                                          MACROS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                           ENUMS
//--------------------------------------------------------------------------------------------------

// this values comes in the answer from the bulverde SU
enum IntelErrCodes
{
	INTEL_ACK   = 0,    // ACK, the command was successfull
	INTEL_NACK  = 1,    // NACK, command failed
    INTEL_SEQ_ERR = 2   // Sequence error
};


//--------------------------------------------------------------------------------------------------
//                               STRUCTURES AND OTHER TYPEDEFS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                               GLOBAL VARIABLE DECLARATIONS
//--------------------------------------------------------------------------------------------------
static TCHAR* g_arrszCmdString[ERR_CMD_TOTAL] = 
{
    " ",
    ERASE_CMD, //0x01
    ADDR_CMD, //0x02    
    BIN_CMD, //0x03
    BAUD_CMD, //0x04
    JUMP_CMD, //0x05
    POWER_DOWN_CMD, //0x06
    RQ_CKSUM_CMD, //0x07
    RQ_HW_VERSION_CMD, //0x08
    RQ_RAM_CKSUM_CMD, //0x09
    " ", //0x0A
    " ", //0x0B
    " ",//0x0C
    " ",//0x0D
    " ",//0x0E
    " ",//0x0F
    RANGE_CHECKSUM_CMD, //0x10
    RQ_SERIAL_NUM_CMD, //0x11
    RQ_SW_VERSION_CMD, //0x12
    RQ_FLASH_VRSN_CMD, //0x13
    REINSERT_BARKER, //0x14
    FDI_UPGRADE_CMD, //0x15
    RESTART_COMMAND, //0x16
    SIRFS_UP_COMMAND, //0x17
    MADDR_CMD, //0x18
    MBIN_CMD, //0x19
    " ", //0x1A
    " ", //0x1B
    " ",//0x1C
    " ", //0x1D
    " ", //0x1E
    " ", //0x1F
    RQSF_CMD, //0x20
    CONFIG_CMD, //0x21
    INTEL_VERSION_CMD_STR, //0x22
    INTEL_START_ADDRS_CMD_STR, //0x23
    INTEL_DATA_CMD_STR, //0x24
    INTEL_JUMP_CMD_STR, //0x25
    INTEL_PREAMBLE_STR, //0x26
    PROD_CMD, // 0x27
    TI_SIGNAL_CMD, // 0x28
    TI_PARAM_CMD, // 0x29
    " ", //0x2A
    " ", //0x2B
    " ", //0x2C
    " ", //0x2D
    " ", //0x2E
    " ", //0x2F
    TI_WRITE_CMD, // 0x30
    TI_ABORT_CMD, // 0x31
    TI_CERT_CMD, // 0x32
    TI_PLAT_CMD_STR, // 0x33
    TI_RUN_CMD_STR, // 0x34
    TI_ADDR_CMD_STR, // 0x35
    TI_LEN_CMD_STR, // 0x36
    TI_ACK_CMD_STR, // 0x37
    RQ_PDS_INFO_CMD, // 0x38
    ENUM_CMD, // 0x39
	" ", //0x3A
    " ", //0x3B
    " ", //0x3C
    " ", //0x3D
    " ", //0x3E
    " ", //0x3F
	RQHSTAT_CMD // 0x40
};

//--------------------------------------------------------------------------------------------------
//                                   FUNCTION PROTOTYPES
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                         CLASS
//--------------------------------------------------------------------------------------------------

#endif DEVICE_API_ERROR_H