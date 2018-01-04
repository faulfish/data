#ifndef MESSAGES_H
#define MESSAGES_H

#ifndef __cplusplus
#error "This is a C++ header file; it requires C++ to compile."
#endif

//--------------------------------------------------------------------------------------------------
//
//   Header Name: PST_FP_Messages.h
//
//   General Description: This Header file contains Status String that are displayed within the
//   Flash GUI.
//
//--------------------------------------------------------------------------------------------------
//
//                            Motorola Confidential Proprietary
//                       Advanced Technology and Software Operations
//                    (c) Copyright Motorola 2005-2010, All Rights Reserved
//
//
// Revision History:
//                             Modification     Tracking
// Author                          Date          Number     Description of Changes
// -------------------------   ------------    ----------   ----------------------------------------
// Mark Jones                  04/06/2000       CSGceXXXXX   Header for all string used within the Flash
//                                                          Programmer.
//
// Mark Jones                  08/01/2000       CSGce67517   Added Error Log file to release build
//
// Mark Jones                  08/11/2000       CSGce68566   Fix on validation of SuperFile
//
// Mark Jones                  10/02/2000       CSGce75000   Flashing Encrypted Files
//
// Mark Jones                  10/05/2000       CSGce75602   Update user when verifying checksums
//
// Marc Risop                   5/8/2001        CSGce86207  Add Language package flashing capability
//
// Mike Doehler                 6/28/2001       LIBaa06187   Flash/Flex support
//
// Mike Doehler                 8/4/2001        LIBbb04911   Added superfile type incomatibility messages
//
// Jagat Shah                   09/07/2001      CSGce87929   Added Message PHONE_PWR_ERROR
//
// Steve Scherer                05/06/2002      LIBbb27863  Added error message if phone needs RAM downloader
//                                                          but the superfile doesn't have one.
//
// Marc Risop                   07/29/2002      LIBbb16223  Multi Interface support for Paragon
//                                                          added FUNCTION_USAGE_ERR to indicate incorrect
//                                                          parameters were passed.
//
// Marc Risop                   08/14/2002      LIBbb37946  Adding MI_SF_INCOMPATIBLE preprocessor definition
//
// Scott DeBates				06/24/2002		LIBbb40555  Update Flash questions
//
// Steve Scherer                10/22/2002      LIBbb58808  Added error cases for POG HAB and incompatible SHX
//                                                          files, certificate vs blowfish.
//
// Marc Risop                   11/13/2002      LIBbb66949  Sirf flashing should be handled as seperate process
//
// Marc Risop                   07/24/2003      libdd12573  Changes to allow flashing of Paragon Pog
//
// Dennis Kim					09/26/2003		LIBdd05867	Add support for HAB.
//
// Dindo Jao                    10/16/2003      LIBdd40378  Added error messages and enum for compound flash
//
// Steve Scherer                02/25/2004      LIBaa01575  Removed unused FLASH_BOOT_VER and CODEGRP_VER messages.
//
// Steve Scherer                03/23/2004      LIBdd87981  Added an error message if RQSF fails.
//
// Dindo Jao                    07/22/2004      LIBee30697  Error message for Tyax and Micron superfile mismatch.
//
// Dindo Jao                    11/10/2004      LIBee41450  Rearrange errors, removed unused errors and append ERR_ to
//                                                          beginning of each error.
//
// Dindo Jao                    03/21/2005      LIBff75519  New message status when waiting for barbedos phone.
// Satya Calloji                03/07/2005      LIBff56268  Added super file non-existnce error.
//
// Satya Calloji                03/24/2005      LIBff80832  Lock-Triplets Crash issue
// Tiago Armani                 04/12/2005      LIBff68631  Added error code for invalid barker in bootloader image
//                                                          and for error sending the CONFIG cmd.
// Satya Calloji                06/01/2005      LIBff65906  Restart issue error codes and messages 
//
// Satya Calloji                06/09/2005      LIBff61071  Multi Interface hangup issue
//
// Tiago Armani                 05/04/2005      LIBff30411  Added errors for bulverde BLOB download
//
// Tiago Armani                 08/02/2005      LIBff35216  Added error for boot command configuration setting
//
// Norton Santos                11/10/2005      LIBff67978  Added error message if RQSW fails
//
// Satya Calloji			    11/10/2005	    LIBgg26534	Sending PROD Command
//
// Norton Santos                12/19/2005      LIBgg24542  Added error message if index is out of bound
//
// Samir Khial                  02/15/2006      LIBgg77383  Rework ODM Naked/Reflash support  
//
// Norton Santos                04/10/2006      LIBhh93915  Added ERR_GET_CONFIG_CMD
//
// Samir Khial                  05/17/2006      LIBii41706   Add Serial Flash API support
//
// Samir Khial                  06/07/2006      LIBii47509  Added support for Kenai dll
//
// Samir Khial                  07/07/2006      LIBii76588   Add ETM test command interface support
//
// Guilherme Cincoetti          07/07/2006      LIBii35340  Added ERR_FLASHADAPTER_NULL error.
//
// Fathi Ben Amar               09/18/2006      LIBii99372  Reorganization of include file 
//
//
// Satya Calloji                09/07/2006      LIBii58143  Added Sibley errors
//
// Satya Calloji                09/15/2006      LIBjj92070  Added missing commas
//
// Vidip Acharya                09/28/2006      LIBii98862  BL/RDL Security changes
//
// Samir Khial                  10/19/2006      LIBjj80854  Norman Changes
// Samir Khial                  12/19/2006      LIBkk61289  Fixed CDMA-D multi-up issue
//
// Samir Khial                  12/05/2006      LIBkk72564  Additional Norman changes
//
// Stefan Jez                   09/02/2007      LIBll08737  Add new error msg for ERR_API_DEPRECATED 
// Norton Santos                08/07/2007      LIBff89694  Old PST related code removal
// Tingting Dong				07/17/2007		LIBmm76870  Add RQHSTAT command sending
// Xiang Ye                     11/08/2007      LIBnn61500  Add new error msg for ERR_MESSAGEPUMPTHREAD_START
//
// Karthik Ramachandran			11/12/2007      LIBnn01226   Support for GreatWall BP PASS through mode flashing
//
//--------------------------------------------------------------------------------------------------
//                                       INCLUDE FILES
//--------------------------------------------------------------------------------------------------
#include "flashcommon.h"

//--------------------------------------------------------------------------------------------------
//                                         CONSTANTS
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//                                          MACROS
//--------------------------------------------------------------------------------------------------

//Status Messages.
#define CREATE_IMAGE        _T("Creating Image File.")
#define ERASING_FLASH       _T("Erasing flash memory.")
#define ERASING_LANGPCK     _T("Erasing Language Package only.")
#define ERASE_COMP          _T("Erase Complete.")
#define FLASH_RAMBOOTSTRAP  _T("Loading RAM Downloader for Bootstrap.")
#define FLASH_RAMBOOTLOAD   _T("Loading RAM Downloader for Bootloader.")
#define FLASH_FLASHBOOT     _T("Flashing Flash Bootloader.")
#define FLASHBOOT_FLASHED   _T("Flash Bootloader Flashed.")
#define FLASH_PHONECODE     _T("Flashing phone code")
#define FLASH_LANGPCK       _T("Flashing Language Pack")
#define FLASH_RESPCK        _T("Flashing Resource Pack")
#define FLASH_SIRF          _T("Flashing Sirf")
#define FLASH_DONE          _T("FLASH COMPLETED!!!.")
#define FLASH_CANCEL        _T("flash process canceled")
#define ENTER_FILE          _T("Please enter in a file")
#define FLASH_CODE_GROUP    _T("Flashing Code Group ")
#define CODE_GROUP_STR      _T("Code Group ")
#define FLASHED_STR         _T(" flashed.")
#define VERIFY_SUCCESS      _T("Verify Successful")
#define VERIFY_FAIL         _T("Verify FAILED!!!")
#define POWERDOWN_SU        _T("Phone is powering DOWN!!!")
#define REBOOT_SU           _T("Phone is being rebooted")
#define SIRF_FLASH_PASS     _T("Sirf Flashing Passed.")
#define FLASH_IMAGE_FILES     _T("Flashing image files...")
#define FLASH_IMAGE_FILES_DONE     _T("Completed flashing image files.")
#define VERIFY_CHECKSUM     _T("Verifying Checksums...")
#define VER_CGRP_CHKSUM_OK  _T("Code Group %i checksums match.")
#define VER_FB_CHKSUM_OK    _T("Flash Bootloader checksums match.")
#define FLASH_WAIT_PHONE_BUSY    _T("Phone is processing data, please wait.")
#define DEVICE_WAIT_FOR_OTHERS _T("Device is waiting for other devices to complete flashing.")
#define USE_BAUDRATE        _T("Using baud rate %i for flashing device.")
#define WAIT_FOR_DEVICE_ENUM  _T("Waiting for device to enumerate.")
#define SEND_MANIFEST       _T("Sending Manifest")
#define REFRESH_DEV_CONN    _T("Refreshing Device Connections...")
#define DEVICE_WAIT_POWER_DOWN     _T("Device is waiting for power down")
#define DEVICE_WAIT_RESTART _T("Device is waiting for restart")
#define IMCOMPATIBLE_FILE   _T("Incompatible file!")
#define ERROR_RESPONSE_PHONE    _T("Error retrieving response from phone")
#define START_SEND_BLOB     _T("Started: Sending the BLOB to the phone")
#define FINISH_SEND_BLOB    _T("Finished: Sending the BLOB to the phone")
#define WAIT_REENUMERATION  _T("Waiting for re-enumeration")
#define FLASH_CG2           _T("Flashing Code Group 2")
#define VERIFY_CG2          _T("Verifying Code Group 2 Checksum")
#define VER_CG2_CHKSUM_OK   _T("Code Group 2 Checksums match")
#define PRE_UPGRADE_STAT_CHK    _T("Performing Pre Upgrade Status Check")
#define PLEASE_WAIT         _T("Please wait (This takes up to 20 seconds)")
#define SWITCH_BP_PASS_MODE _T("Switching device to BP Pass through mode")



//--------------------------------------------------------------------------------------------------
//                                           ENUMS
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//                               STRUCTURES AND OTHER TYPEDEFS
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//                               GLOBAL VARIABLE DECLARATIONS
//--------------------------------------------------------------------------------------------------
static TCHAR* g_arrszErrMsg[FLASHERRENUM] =
{
    _T("Flash operation was successful."),//FLASH_STATUS_OK
    _T("Flash operation failed."),//FLASH_STATUS_FAILED
    _T("An invalid number of parameter was provided."),//FLASH_STATUS_WRONG_PARAM_NUMBER
    _T("Unable to load the adapater properly."),//FLASH_STATUS_ADAPTER_LOAD_ERROR
    _T("An internal error occurred in flash."),//FLASH_STATUS_INTERNAL_ERROR
    _T("Failed creating debug log %s."),//ERR_DBG_LOG_CREATE
    _T("Failed creating debug log %s.  Please delete unused debug log files."),//ERR_DBG_LOG_CREATE2
    _T("Failed creating error log %s."),//ERR_LOG_CREATE
    _T("Failed creating error log file %s.  Please delete unused error log files."),//ERR_LOG_CREATE2
    _T("An error occurred on creating or writing to an image file."),//ERR_IMAGEFILE_WRITE
    _T("An error occurred while reading image file."),//ERR_IMAGEFILE_READ
    _T("Error creating flash registry key."),//ERR_REG_CREATE
    _T("Flash registry key was not created."),//ERR_REG_CREATE2
    _T("Unable to set registry value."),//ERR_REG_SET_VALUE
    _T("Unable to retrieve registry value."),//ERR_REG_GET_VALUE
    _T("Error processing flash file."),//ERR_FLASH_FILE_PRCS
    _T("An error occurred while opening software image file."),//ERR_IMG_OPEN
    _T("Error validating image files."),//ERR_IMG_VALID
    _T("Flash file IO pointer is NULL."),//ERR_FILEIO_NULL
    _T("Flash device API pointer is NULL."),//ERR_FLASHDEV_NULL
    _T("Unable to create flash thread class."),//ERR_FLASHTHREAD_CREATE
    _T("Unable to create flash dialog class."),//ERR_FLASHDLG_CREATE
    _T("Unable to get current directory."),//ERR_GET_DIR
    _T("Unable to query flash dll version."),//ERR_VER_QUERY
    _T("Error retrieving File Version Info for the Flash DLL."),//ERR_GET_VER
    _T("Error globally allocating memory for Flash DLL version number."),//ERR_GLOB_ALLOC
    _T("Error getting file version info size."),//ERR_VER_SIZE
    _T("Error initializing flash device API."),//ERR_FLASHDEV_INIT
    _T("Error sending RAM download for bootstrap."),//ERR_RAM_BOOTSTRAP
    _T("Error sending RAM download for bootloader."),//ERR_RAM_BOOTLOADER
    _T("Error flashing subscriber unit."),//ERR_FLASH_SU
    _T("Error erasing subscriber unit."),//ERR_ERASE_SU
    _T("Error getting subscriber unit information."),//ERR_SU_INFO
    _T("Error geting subscriber unit checksum."),//ERR_SU_CHKSUM
    _T("Error powering down subscriber unit."),//ERR_PWR_DOWN
    _T("Error rebooting subscriber unit."),//ERR_REBOOT
    _T("Error setting reenumeration event."),//ERR_REENUM_SET_EVENT
    _T("Error setting file pointer."),//ERR_FILE_PTR_SET
    _T("Error refreshing device list."),//ERR_REFRESH_CONNECTION
    _T("Unable to retrieve interface handle."),//ERR_INTF_HANDLE
    _T("Unable to get string descriptors."),//ERR_PROD_STR_DESC
    _T("Unable to retrieve initialization values from INI file."),//ERR_INI_FILE
    _T("String descriptor is NULL."),//ERR_STR_DESC_NULL
    _T("Error sending barker code."),//ERR_BARKER_CODE
    _T("Checksum error in the RAM Downloader for Flash Bootloader. File: 0x%X, Phone: 0x%X"),//ERR_RAMDLD_CHKSUM
    _T("Unable to retrieve RAM Downloader for Flash Bootloader checksum."),//ERR_RAMDLD_CHKSUM_RET
    _T("Error verifying Bootloader checksums.  File: 0x%X, Phone: 0x%X"),//ERR_VER_BL_CHKSUM
    _T("Bootloader checksum do not match.  File: 0x%X, Phone: 0x%X"),//ERR_BL_CHKSUM
    _T("Error verifying Sirf checksums. File: 0x%X, Phone: 0x%X"),//ERR_VER_SIRF_CHKSUM
    _T("Error verifying Code Group %i checksums. File: 0x%X, Phone: 0x%X"),//ERR_VER_CGRP_CHKSUM
    _T("Error Code Group %i contains no Subscriber Unit checksum."),//ERR_NO_SU_CHKSUM
    _T("The file selected is an invalid superfile.\nPlease enter in a valid superfile."),//ERR_INVALID_SUPER_FILE
    _T("The number of image files from the Superfile exceeds the limit."),//ERR_INVALID_NUM_FILES
    _T("Possible Corrupt SuperFile.  File count does not match with the number of valid files."),//ERR_CORRUPT_SUPER_FILE
    _T("Unable to re-program Code Group 0 barker. An error occurred sending RBRK command."),//ERR_REPROG_BARKER
    _T("Language package version 0x%X does not match Subscriber Unit version 0x%X."),//ERR_LANG_PKG_MISMATCH
    _T("The superfile selected is for reflash.  Please use a rom superfile."),//ERR_SF_NAKED_MISMATCH
    _T("The superfile selected is NOT for reflash.  Please use a reflash superfile."),//ERR_SF_REFLASH_MISMATCH
    _T("Incompatible superfile for Subscriber Unit state."),//ERR_SF_TYPE_MISMATCH
    _T("Phone is not compatible with multi-interface super-file."),//ERR_MI_SF_INCOMPATIBLE
    _T("Unable to flash Sirf"),//ERR_SIRF_FLASH
    _T("Phone failed initial security verification checks. HAB error 0x%X."),//ERR_SEC_VER_CHECK
    _T("Phone did not re-enumerate after RAM-downloader was sent."),//ERR_RDL_REENUMERATION
    _T("Phone did not re-enumerate after MBM was sent."),//ERR_MBM_REENUMERATION
    _T("Phone is secure, super-file does not contain Security Hash."),//ERR_SF_MD5_SECURITY_MISSING
    _T("Error sending RQSN boot command."),//ERR_RQSN_BOOT_CMD
    _T("Error sending RQSF boot command."),//ERR_RQSF_BOOT_CMD
    _T("Invalid number of flash part to format."),//ERR_INV_NUM_FLASHPART
    _T("In Queue thread timed out."),//ERR_INQ_TIMED_OUT
    _T("Out Queue thread timed out."),//ERR_OUTQ_TIMED_OUT
    _T("Failed sending MADDR command."),//ERR_SENDING_MADDR
    _T("Failed sending MBIN command."),//ERR_SENDING_MBIN
    _T("Failed to remove OUT queue item count."),//ERR_REMOUTQCOUNT
    _T("Failed to remove IN queue item count."),//ERR_REMINQCOUNT
    _T("Unable to remove item from OUT queue."),//ERR_REMFROMOUTQ
    _T("Unable to remove item form IN queue."),//ERR_REMFROMINQ
    _T("Failed to set OUT queue item count."),//ERR_ADDOUTQCOUNT
    _T("Failed to set IN queue item count."),//ERR_ADDINQCOUNT
    _T("Unable to add item to OUT queue."),//ERR_ADDTOOUTQ
    _T("Unable to add item to IN queue."),//ERR_ADDTOINQ
    _T("Unable to allocate flash part structure."),//ERR_FLASHPART_ALLOC
    _T("Unknown superfile extension."),//ERR_UNKNOWNSFEXT
    _T("Unable to allocate IO class."),//ERR_IO_ALLOC
    _T("Unable to allocate Flash File class."),//ERR_FLASHFILE_ALLOC
    _T("Calculated superfile checksum does not match checksum in last S7 record."),//ERR_CALC_CHKSM_NOTMATCH
    _T("Phone is not fully powered up. Power-up the phone and restart the Flash program."),//ERR_PHONE_PWR
    _T("Unknown MA type."),//ERR_UNKNOWN_MA_TYPE
    _T("Invalid Software Descriptors."),//ERR_INVALID_SW_DESC
    _T("Attempting to flash a file built for Micron into a phone with a Tyax flash part."),//ERR_FILE_TYAXMISMATCH
    _T("Attempting to flash a file built for Tyax into a phone with a Micron flash part."),//ERR_FILE_MICRONMISMATCH
    _T("Error verifying Subscriber Unit Info: Chipset type does not match."),//ERR_CHIPSET_TYPE_MISMATCH
    _T("Error verifying Subscriber Unit Info: MA type does not match"),//ERR_MA_TYPE_MISTMATCH
    _T("Error verifying Subscriber Unit Info: Code Group %d Data Type does not match."),//ERR_DATA_TYPE_CG_MISMATCH
    _T("Error verifying Subscriber Unit Info: Code Group %d Data Type does not match.\r\nDo you still want to flash the phone?"),//ERR_DATA_TYPE_CG_CONTINUE
    _T("No RAM downloader in superfile, phone needs one to flash."),//ERR_SUPERFILE_RDL_MISSING
    _T("Error verifying Subscriber Unit Info: Security type does not match."),//ERR_SECURITY_TYPE_MISMATCH
    _T("No Subscriber Unit connected.  Please check the connection or connect a subscriber unit."),//ERR_NO_SU_CONN
    _T("Error is setting the PST baud rate"),//ERR_BAUD_RATE
    _T("Error in setting the BAUD rate on the phone side"),//ERR_BAUD_SU
    _T("Error in setting the BAUD rate on the PC side"),//ERR_BAUD_PC
    _T("Exceeded the number of barker code limit"),//ERR_MAX_BARKER
    _T("Error jumping to RAM Downloader"),//ERR_RDL_JMP
    _T("The \"%s\" Interface could not be found."),//ERR_INTF_NOT_FOUND
    _T("Sending FDI status request command produced an error."),//ERR_FDI_STAT_CMD
    _T("Error sending FDI RESTORE command."),//ERR_FDI_RESTORE_CMD
    _T("FDI checksum mismatch! Checksum=%s"),//ERR_FDI_CHECKSUM_MISMATCH
    _T("Sending FDI checksum request command produced an error."),//ERR_FDI_CHECKSUM_REQ
    _T("Error flashing new settings!"),//ERR_FLASHING_FDI_DATA
    _T("Error sending FDI ERASE command."),//ERR_FDI_ERASE
    _T("Error sending FDI SAVE command."),//ERR_FDI_SAVE
    _T("FDI not upgradeable."),//ERR_FDI_NOT_UPG
    _T("FDI upgrade failed."),//ERR_FDI_UPGRADE
    _T("Error loading manifest."),//ERR_LOAD_MANIFEST
    _T("Loading AP RAM-downloader failed"),//ERR_LOAD_AP_RDL
    _T("The \"%s\" interface was found, but the super-file is not digitally signed, and the phone requires it to be."),//ERR_SUPERFILE_UNSECURE
    _T("Unable to start thread for %s interface."),//ERR_INTF_THREAD
    _T("Flashing an unsigned superfile to a productions phone is not allowed."),//ERR_SUPERFILE_UNSIGNED
    _T("Bound File UID1(%s) does not match phone UID2(%s)."),//ERR_UID_MISTMATCH
    _T("Unable to create UID file."),//ERR_UID_CREATE
    _T("Error sending JUMP command"),//ERR_JMP_SEND
    _T("Flash failed."),//ERR_FLASH_FAIL
    _T("Unable to reboot subscriber unit."),//ERR_SU_REBOOT
    _T("Unable to power down subscriber unit."),//ERR_SU_POWERDOWN
    _T("Verifying all checksums failed."),//ERR_VER_CHKSM
    _T("Verifying subscriber unit info with superfile info failed."),//ERR_VER_SU_SF
    _T("The superfile specified does not exist"),//ERR_SUPERFILE_NOTEXISTING
    _T("The Triplets phone failed to flash due to an error in the communication between the phone and the tool."),//ERR_TRIPLETS_FLASH_STOP
    _T("Invalid barker in bootLoader image file."),//ERR_INVALID_BOOTL_BARKER    
    _T("Error sending CONFIG command."),//ERR_SEND_CONFIG_CMD
    _T("No valid BLOB codegroup was found."),//ERR_BLOB_CG_NOT_FOUND
    _T("Error sending the Intel bulverde preamble command."),//ERR_SEND_INTEL_PREAMBLE
    _T("Error sending the Intel bulverde version request command."),//ERR_SEND_INTEL_VERSION
    _T("Error sending the Intel bulverde start address command."),//ERR_SEND_INTEL_START_ADDRESS
    _T("Error sending the Intel bulverde data package command."),//ERR_SEND_INTEL_DATA
    _T("Error sending the Intel bulverde jump to BLOB command."),//ERR_SEND_INTEL_JUMP
    _T("In a batch of phones, the same flash method as called earlier needs to be called"), //ERR_WRONG_METHOD_CALLED_IN_BATCH
    _T("In a batch of phones, the same action after flash as called earlier needs to be set"), //ERR_WRONG_OPERN_CALLED_IN_BATCH
    _T("Unable to add the device for processing,please check the Device ID"), //ERR_ADDING_DEVICE
    _T("Device did not re-enumerate after flash operation.Please manually reconnect the device"), //ERR_REENUM_DEVICE
    _T("Device removal operation failed at the end"), //ERR_REMOVING_DEVICE
    _T("Multi Interface threads timedout and did not complete the flashing operation"), //ERR_MINTF_TIMEDOUT
    _T("Could not set the configuration for the boot commands from INI file. Using default FlashDevAPI configuration."), //ERR_SETTING_BOOTCMD_CONFIG
	_T("Image file pointer is NULL."), //ERR_NULL_IMG_FILE_PTR
	_T("Error sending RQSW boot command."),//ERR_RQSW_BOOT_CMD
	_T("Error while sending the PROD Command."), //ERR_PROD_CMD
    _T("Index out of bound."), //ERR_INDEX_OUT_OF_BOUND
    _T("Unable to allocate memory."), //ERR_ALLOC_MEMORY
    _T("Passed parameter is NULL."), //ERR_PASSED_PARAM_NULL
    _T("Error occured while positioning image file pointer."), //ERR_IMAGEFILE_SEEK
    _T("Error occured while creating image file packet."), //ERR_IMAGEFILE_CREATE_PACKET    
	_T("Error sending TI ROM data packet request."), //ERR_SEND_TI_DATA
	_T("Error receiving TI ROM data packet response."), //ERR_RCV_TI_DATA
        _T("Error getting CONFIG command."), //ERR_GET_CONFIG_CMD
	_T("Device did not enumerate before flash operation."), //ERR_ENUM_SER_DEVICE
	_T("Unable to start device before flashing."), //ERR_START_SER_DEVICE
	_T("Unable to stop device after flashing."), //ERR_STOP_SER_DEVICE
    _T("Invalid parameter value was provided."), //ERR_WRONG_PARAM_VALUE
    _T("Device type is not supported."), //ERR_DEVICE_NOT_SUPPORTED
    _T("Failed to validate Serial Params."), //ERR_INVALID_SERIAL_PARAMS
    _T("Error checking the RAM Downloader."), //ERR_RDL_CHECK
    _T("Error setting baud rate for MBM flashing."), //ERR_BAUD_MBM
    _T("Generic exception caught.baudrate info is not completed."), //ERR_BAUD_CFG 
    _T("Unable to register flash for notifications."), //ERR_REG_NOTIFICATION
    _T("Flash Adapter API pointer is NULL."), //ERR_FLASHADAPTER_NULL
    _T("Cannot flash TYAX flashfile into SIBLEY parts."), //ERR_TYAX_INTO_SIBLEY
    _T("Cannot flash SIBLEY flashfile into TYAX parts."), //ERR_SIBLEY_INTO_TYAX
    _T("Error invalid number of flash parameters."), //ERR_INVALID_FLASH_PARAMS
	_T("Error loading XML file"), //ERR_LOADING_XML_FILE
    _T("Error upgrading bootloader."), //ERR_UPGRADING_BL
    _T("Error bootloader info not found in XML file."), //ERR_NO_BL_INFO_IN_XML
    _T("Error ram downloader flashing using XML file."), //ERR_RAMDL_FLASH
    _T("Error creating XML helper class."), //ERR_CREATING_XML_CLASS
    _T("Error Pointer to XML helper class is NULL."), //ERR_XML_CLASS_NULL
    _T("Error creating BL upgrade class."), //ERR_CREATING_BL_UPGRADE_CLASS
    _T("Error creating RDL upgrade class."), //ERR_CREATING_RDL_UPGRADE_CLASS
    _T("Error extracting RDL."), //ERR_RAMDL_EXTRACTION
	_T("Error creating XML File Details class."), //ERR_CREATING_XML_FILE_DETAILS_CLASS=176,
	_T("Error creating XML File Handler class."), //ERR_CREATING_XML_FILE_HDLR_CLASS=177,
	_T("Bad value for code group extra info."), //ERR_EXTRA_INFO_BAD_VALUE
    _T("Error sending ENUM command."),//ERR_SEND_ENUM_CMD
    _T("Unable to start flash thread."), //ERR_FLASHTHREAD_START
    _T("Unable to set flash thread instance."), //ERR_FLASHTHREAD_INSTANCE
    _T("Invalid PST string array size."), //ERR_STR_ARRAY_SIZE
    _T("Unable to get device UID value."), //ERR_UID_INFO
    _T("Unable to get boot flash flex version."), //ERR_BOOT_INFO
    _T("Unable to get boot PDS info."), //ERR_PDS_INFO
    _T("API has been deprecated."), //ERR_API_DEPRECATED
    _T("Error sending RQHSTAT command."), //ERR_RQHSTAT_SEND
    _T("Error sending TI ROM command."),//ERR_SEND_TI_CMD=188,
    _T("Unable to start message pump thread.") //ERR_MESSAGEPUMPTHREAD_START
    _T("Error switching phone to BP Pass through mode") // ERR_BP_PASS_MODE
    // insert new error message here.
};

//Memory integrity check
static TCHAR* g_arrszRQHSTATMemMap[15] =
{
	_T(""), //0x00
	_T("L1 Cache: "), //0x01
	_T("L2 Cache: "), //0x02
	_T("Internal Ram: "), //0x03
	_T(""), //0x04
	_T(""), //0x05
	_T("CS0 External Ram: "), //0x06
	_T("CS1 External Ram: "), //0x07
	_T(""), //0x08
	_T(""), //0x09
	_T("CS0 Flash: "), //0x0A
	_T("CS1 Flash: "), //0x0B
	_T("CS2 Flash: "), //0x0C
	_T("CS3 Flash: "), //0x0D
	_T("CS4 Flash: ") //0x0E
};

//--------------------------------------------------------------------------------------------------
//                                   FUNCTION PROTOTYPES
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//                                         CLASS
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
#endif // MESSAGES_H
