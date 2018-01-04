#ifndef SHARED_H
#define SHARED_H

#ifndef __cplusplus
#error "This is a C++ header file; it requires C++ to compile."
#endif

//--------------------------------------------------------------------------------------------------
//                                                                               
//   Header Name: PST_FP_Shared.h
//
//   General Description: This header file contains shared information that is used multiple classes
//   of the Flash Programmer. 
//
//--------------------------------------------------------------------------------------------------
//
//                            Motorola Confidential Proprietary
//                       Advanced Technology and Software Operations
//                    (c) Copyright Motorola 2006-2011, All Rights Reserved
//   
//
// Revision History:
//                             Modification     Tracking
// Author                          Date          Number     Description of Changes
// -------------------------   ------------    ----------   ----------------------------------------
// Mark Jones                  04/06/2000      CSGceXXXXX   Header for all variables and structures 
//                                                          that are shared within the Flash Programmer
//
// Mark Jones                  08/11/2000      CSGce68566   Fix on validation of SuperFile
//
// Mark Jones                  10/02/2000      CSGce75000   Flashing Encrypted Files
//
// Marc Risop                   5/8/2001        CSGce86207  Add Language package flashing capability
//
// Mike Doehler                 6/28/2001      LIBaa06187   Flash/Flex support
//
// Mike Doehler                 8/04/2001      LIBbb04911   Added bROMSuperfile flag to indicate superfile type
//
// Jagat Shah                  09/07/2001      LIBbb00391   Added Identifiers for RAINBOW.
//
// Steve Scherer               10/17/2001      LIBbb00390   Added #define BLANK_FLASH_RAINBOW for blank Talon.
//
// 
// Sundar Ramalingam           12/26/2001      LIBbb10409   Added cdma_1x chip and Neptune set definitions
//    
// Anthony Leung                11/19/2001     LIBbb10838   Added Identifiers for LCA NEPTUNE
//
// Sundar Ramalingam            01/03/2002     LIBbb10409   Redefined cdma_1x chip set numbers per Brent Haws 
//
// Steve Scherer                02/28/2002     LIBbb21472   Added iProduct descriptors for OMAP(Paragon) and 
//                                                          added chip sets for OMAP and SiRF(GPS).
//
// Steve Scherer                04/10/2002     LIBbb24163   Added iProduct descriptor for CDMA1X 5100 processor.
//
// Steve Scherer                04/22/2002     LIBbb16228   Added constants for Paragon Flash Interfaces.
//
// Steve Scherer                05/06/2002      LIBbb27863  Added constant NO_RAM_DOWNLOADER for Paragon.
//
// Steve Scherer                05/07/2002      LIBbb27677  Added constants PHONE_SW, AP_FILE_SYSTEM_DATA,
//                                                          and HASH_CHECKSUMS.  (New Data Types in phone code.) 
//
// Steve Scherer                05/31/2002      LIBbb31090  Added product descriptor "Flash MSM5100" to
//                                                          enable flash on Qualcomm 5100 series products.
//
//
// Mike Doehler                 06/01/2002     LIBbb28289   Added RQRCS functionality (ALT_CHECKSUM_MASK, etc.)
//
// Wen Yi                      7/26/2002       LIBbb43511. Added Product ID for Neptune LTS.
//
// Wen Yi                      9/6/2002        LIBbb50246. Added Product ID for Neptune ULS chipset.
// Wen Yi                       7/26/2002      LIBbb43511. Added Productor ID for Neptune LTS.
//
// Marc Risop                   07/29/2002     LIBbb16223   Support multiple flash interfaces for Paragon
//                                                              Moved header structures to shx_file.h
// Steve Scherer                10/22/2002     LIBbb55938   Added String descriptors for Rainbow POG
//
// Steve Scherer                10/22/2002     LIBbb58808   Added sDescription to SUPER_FILE to determine whether
//                                                          the superfile uses blowfish or certificate verification.
//
// Wen Yi			10/30/2002     LIBbb09911   Fix the barker code for code group 0 for CDMA1X
//
// Marc Risop       12/11/2002     LIBbb67553   Add EZX platform type
//
//
// Marc Risop					10/30/2002	   LIBbb58064	Adding string descriptors for EZX
//
// 
// Steve Scherer                12/31/2002     LIBbb71993   Added Secure and Non-Secure descriptors for Neptune.
// Marc Risop                   11/26/02       LIBbb68515   Adding DESCRIPTOR_TYPE_INDEX to describe location of
//
//                                                               descriptor in RSHW.
// 
// Steve Scherer                01/30/2003     LIBbb78127   Added string descriptors for Neptune products.
//
// Marc Risop                   02/19/2003     LIBbb58070   Adding logic support for EZX multi-interface flashing
//
// Marc Risop                   04/08/2003     LIBbb93081   Changing the EZX string descriptors, FLASH_EZX_RAMDOWNLOADER
//                                                          will never be used, and BLANK_EZX_RAMDOWNLOADER is being changed
//                                                          to BLANK_EZX_BOOTLOADER
//
// Marc Risop                  04/14/2003      LIBbb37955  Enhancing messaging for multi-interface flashing.
//
// Marc Risop                   05/14/2003     LIBbb74962  3G Flash - Add logic to Paragon flash to detect various BPs, (POG 3G)
//
// Marc Risop                   07/17/2003     LIBbb16230  Changes needed to make Multi-interface multi-flash work.
//
// Marc Risop                   07/24/2003     libdd12573  Changes to allow flashing of Paragon Pog
//
// Dindo Jao                    10/29/2003     LIBdd40378  Changed DECIMAL to DCML. Conflicts with MS predifined type
//
// Dindo Jao                    02/26/2004     LIBdd63855  Rearranged SUPER_IMAGES structure to be able to initialize using
//                                                         ZeroMemory or memset. 
// Dindo Jao                    07/22/2004     LIBee30697  Error message for Tyax and Micron superfile mismatch.
//
// Dindo Jao                    08/16/2004     LIBee23970  Removed JUMP_OFFSET definition
//
// Dindo Jao                    08/20/2004     LIBee66282  Added Bulverde support.
//
// Dindo Jao                   10/20/2004      LIBff20639   Added registry entries.
//
// Tiago Armani                 12/16/2004     LIBff39875  Added definitions to deal with RQSN data
//
// Dindo Jao                    01/24/2005     LIBff50297   Added secure engineering bulverde support.
//
// Tiago Armani                 05/11/2005     LIBff30411   Modified bulverde USB Flash Device Interface string
//
// Satya Calloji                05/26/2005     LIBff65906   Restart Issue changes
//
// Tiago Armani                 07/25/05       LIBgg35216  Added support to Boot Command Configuration (included
//                                                         Bootcmd enumeration and configuration structure.
//
// Satya Calloji                08/03/2005     LIBgg63882  New options for handling restart as needed by GNPO
// Satya Calloji                11/10/2005     LIBgg26534  Sending PROD Command	
// 
// Tiago Armani                 08/09/2005     LIBgg13321   Added registered message to show pop-up box
// Jagat Shah					10/15/2005	   LIBhh26896	Added interface name for second flash part
// Norton Santos                12/20/2005     LIBhh48672   Moved BYTE SW_CG_VERSION, SU_INFO_STRUC and BOOT_FLASH_FLEX_STRUC
//                                                          from "PST_FP_FlashApiCommon.h" to "PST_FP_Shared.h" 
//
// Satya Calloji                02/20/2006     LIBhh92237  Added BP UID check for MI Superfiles
//
// Dindo Jao                    03/10/2006     LIBhh88482  Virtual memory usage
//
// Satya Calloji                03/20/2006     LIBii20254  Changes for Q Phone - BP Flashing
//
// Satya Calloji                04/18/2006     LIBii44156  Power down after flashing when checksum failure occurs
//
// Dindo Jao                    04/25/2006     LIBii52570  Changed IO.h to IOBase.h 
//
// Samir Khial                  04/24/2006     LIBii41719  Fixed interface handle validation.
//
// Samir Khial                  04/28/2006     LIBii05366  NAND partitioning issue
//
// Mike Zarletti			    06/15/2006	   LIBgg31593  Added method for sending RQPI command
//      
// Satya Calloji                06/01/2006     LIBii85202  Motopro Product String descriptor change 
//
// Norton Santos                06/12/2006     LIBii65606  Supporting consolidated progress bar in Flash DLL
//
// Satya Calloji                07/21/2006     LIBjj52074  RDL Waitime dynamic assignment
//
// Fathi Ben Amar               09/18/2006     LIBii99372  Reorganization of include file 
// Samir Khial                  08/23/2006     LIBjj69053  Support large pages for NAND flashing
// Samir Khial                  08/08/2006     LIBii76569   Add D-program multi-interface support
// Samir Khial                  11/07/2006     LIBkk33695  Read-Only NAND support
// Satya Calloji                09/28/2006     LIBjj80854   Norman Changes
// Stefan Jez                   12/20/2006     LIBkk83083  Changes needed for TI OMAP naked flash
// Samir Khial                  02/08/2007     LIBll31961  Fix ESPOO PROD isssue
//
// Stefan Jez                   02/09/2007     LIBll08737  Removed enum eFlashMethodOperMode
// Kai Zhao						03/19/2007	   LIBii59218  Add inteface string.
// Charles Ndikuryayo            05/29/2007      LIBmm42980 Flash: Configurable RDL reenum timeout value for Norman
// Norton Santos                08/07/2007     LIBff89694  Old PST related code removal
// Tingting Dong				07/17/2007		LIBmm76870  Add RQHSTAT command sending
// Minh Quan Phan Huy		    08/10/2007	   LIBll29195  Added support for Helsinki
// Vidip Acharya				10/22/2007	   LIBmm62633	Add Blank flash support for new TI Chipsets 
// Karthik Ramachandran		    10/27/2007     LIBnn01226   Added support for GreatWall
// Karthik Ramachandran         01/15/2008     LIBoo59544   Support Helsinki unsecure support 
// Xishang Ji                   05/14/2008     LIBpp68150   V325i Read/Write delay
//--------------------------------------------------------------------------------------------------
//                                       INCLUDE FILES
//--------------------------------------------------------------------------------------------------
#pragma once

#include "stdafx.h"
#include "shx_file.h"
#include "PSTCommon.h"
#include "IOBase.h"

//--------------------------------------------------------------------------------------------------
//                                         CONSTANTS
//--------------------------------------------------------------------------------------------------
/* Programming packet definitions */
#define P_DATASIZE_SZ            0x02
#define P_DATA_SZ                0x2000L
#define P_DATA_TI_PACKET_SZ      0x1000L
#define P_CHECKSUM_SZ            0x01
#define PACKET_SIZE              (P_DATASIZE_SZ+P_DATA_SZ+P_CHECKSUM_SZ) 
#define P_BARKER_CODE_SZ         0x08
#define P_SW_DESC_SZ             0x08
#define P_BARKER_BLANK           0xFF
#define BARKER_INDEX             4
#define P_EXTRA_SZ               0x08

#define P_DATASIZE_LOC              0
#define P_DATA_LOC               P_DATASIZE_SZ
#define P_CHECKSUM_LOC           (P_DATASIZE_SZ+P_DATA_SZ)
#define P_DATASIZE_OFFSET           2
#define P_FIELD_OFFSET              8
#define TYPE_FIELD_LEN              1
#define COUNT_FIELD_LEN             2


#define RAM_SECTION_SIZE            0x004000L
#define FLASHBOOT_SECTION_SIZE      0x004000L
#define MAIN_SECTION_SIZE           0x002000L
#define LANG_SECTION_SIZE           0x002000L
#define CHAR_SECTION_SIZE           0x002000L

#define RAM_BOOTSTRAP_SIZE          0x010000L
#define P_RAM_CHECKSUM_LOC          (RAM_BOOTSTRAP_SIZE+P_DATASIZE_SZ)
#define RAM_BOOTSTRAP_BUF_SIZE      (RAM_BOOTSTRAP_SIZE+P_DATASIZE_SZ+P_CHECKSUM_SZ)

/* Datalight NAND packet definitions */
#define P_SMALL_PAGE_RW_SZ          512
#define P_SMALL_SPARE_RW_SZ         16
#define P_SMALL_PAGE_SPARE_RW_SZ    (P_SMALL_PAGE_RW_SZ+P_SMALL_SPARE_RW_SZ)
#define P_SMALL_PAGE_RO_SZ          512
#define P_SMALL_SPARE_RO_SZ         0
#define P_SMALL_PAGE_SPARE_RO_SZ    (P_SMALL_PAGE_RO_SZ+P_SMALL_SPARE_RO_SZ)
#define P_LARGE_PAGE_RW_SZ          2048
#define P_LARGE_SPARE_RW_SZ         64
#define P_LARGE_PAGE_SPARE_RW_SZ    (P_LARGE_PAGE_RW_SZ+P_LARGE_SPARE_RW_SZ)
#define P_LARGE_PAGE_RO_SZ          2048
#define P_LARGE_SPARE_RO_SZ         0
#define P_LARGE_PAGE_SPARE_RO_SZ    (P_LARGE_PAGE_RO_SZ+P_LARGE_SPARE_RO_SZ)
#define P_MAX_PAGE_SPARE_SZ         P_LARGE_PAGE_SPARE_RW_SZ

// USB Flash Device Interface string
const TCHAR FLASH_DEV_INTF_STRING[] = _T("Motorola_Flash");                 // P2k reflash     
const TCHAR FLASH_DEV_CSF_INTF_STRING[] = _T("Secure_Motorola_Flash");      // CSF protected reflash
const TCHAR FLASH_DEV_BOOT_CSF_INTF_STRING[] = _T("Secure_Motorola_Boot");    // CSF protected blank
const TCHAR FLASH_DEV_BOOT_INTF_STRING[] = _T("Motorola_Boot");             // P2k blank
const TCHAR FLASH_A920_OS_INTF_STRING[] = _T("Motorola_Paragon_OS");        // Paragon blank, reflash, bootloader
const TCHAR FLASH_A920_FS_INTF_STRING[] = _T("Motorola_Paragon_Filesystem");// Paragon blank, reflash
const TCHAR FLASH_A920_BP_INTF_STRING[] = _T("Motorola_Paragon_Baseband");  // Paragon blank, reflash
const TCHAR FLASH_DEV_TEST_INTF_STRING[] = _T("Motorola_Test");
const TCHAR FLASH_EZX_AP_INTF_STRING[] =  _T("Motorola_Ezx_AP_Flash");
const TCHAR FLASH_AP_INTF_STRING[] =  _T("Motorola_AP_Flash"); // Bulverde reflash
const TCHAR FLASH_AP_FS_INTF_STRING[] =  _T("Motorola_AP_Filesystem"); // Bulverde reflash second part
const TCHAR FLASH_INTEL_INTF_STRING[] =  _T("FLASHBVY"); // Bulverde naked
const TCHAR FLASH_DEV_CSF_AP_INTF_STRING[] =  _T("Secure_Motorola_AP_Flash"); // CSF AP Flash

//TIROM
const TCHAR FLASH_TIROM_INTF_STRING[] =  _T("pbi");  //TIROM


#define SECURE_MOTOROLA _T("Secure_Motorola") //Used to search for secure flash or boot interfaces


// Superfile Identifier Information
#define SUPERFILE_ID          _T("P2K Superfile")
#define MULTI_INTFACE_TOOL_ID _T("Multi-Interface")
#define PARAGON_DESCRIPTION   _T("Paragon (A920) Super File")
#define EZX_TOOL_DESCRIPTION  _T("EZX Super File")
#define MULTINTF_TOOL_DESCRIPTION  _T("Multi-Interface Super File")
#define BLF_DESCRIPTION       _T("UNIX Generated Superfile")
#define CSF_DESCRIPTION       _T("CSF Protected UNIX Generated Superfile")
#define CSF_IN_DESCRIPTION    _T("CSF")

/* Fixed Data */
#define RAM_BLOCK               0x81
#define FLASHBOOT_BLOCK         0x82
#define MAIN_BLOCK              0x84
#define LANG_BLOCK              0x88
#define CHAR_BLOCK              0x90

//Thread Events
#define FLASH_THREAD_EVENT  _T("Flash Thread Event")
#define FILE_IMAGE_EVENT    _T("Image File Event")

//Flash Descriptors         *** iProduct string descriptors from phone.  They are used to   ***
//                          *** determine if phone is in flash mode and if Blank or Reflash.***
#define BLANK_FLASH_WALLY       _T("Blank P2K Wally")
#define BLANK_FLASH_MSM5105     _T("Blank CDMA1X MSM5105")
#define BLANK_FLASH_PATRIOT     _T("Blank P2K Patriot")
#define BLANK_FLASH_RAINBOW     _T("Blank P2K Rainbow")
#define BLANK_FLASH_LT_NEPTUNE  _T("Blank LCA Neptune")
#define BLANK_FLASH_LTS_NEPTUNE _T("Blank LTS Neptune")
#define BLANK_FLASH_ULS_NEPTUNE _T("Blank ULS Neptune")
#define BLANK_FLASH_OMAP        _T("Blank A920 Paragon")
#define FLASH_WALLY             _T("Flash P2K Wally")
#define FLASH_PATRIOT           _T("Flash P2K Patriot")
#define FLASH_MSM5105_OLD       _T("Motorola V. NE01") 
#define FLASH_MSM5105           _T("Motorola V.120x")
#define FLASH_RAINBOW           _T("Flash P2K Rainbow")
#define FLASH_LT_NEPTUNE        _T("Flash LCA Neptune")
#define FLASH_LTS_NEPTUNE       _T("Flash LTS Neptune")
#define FLASH_ULS_NEPTUNE       _T("Flash ULS Neptune")
#define FLASH_OMAP              _T("Flash A920 Paragon")
#define FLASH_MSM5100           _T("Motorola V.120e")
#define FLASH_MSM5100_ALL       _T("Flash MSM5100")
#define BLANK_POG_2_0           _T("P2K POG")
#define FLASH_A1000				_T("Flash Motorola A1000")
#define BLANK_FLASH_M1000       _T("Blank FOMA M1000")
#define FLASH_M1000             _T("Flash FOMA M1000")


#define S_BLANK_RAINBOW_POG     _T("S Blank Rainbow POG")
#define NS_BLANK_RAINBOW_POG    _T("NS Blank Rainbow POG")
#define S_FLASH_RAINBOW_POG     _T("S Flash Rainbow POG")
#define NS_FLASH_RAINBOW_POG    _T("NS Flash Rainbow POG")
#define FLASH_EZX_BOOTLOADER    _T("Motorola Ezx Bootloader")
#define BLANK_EZX_BOOTLOADER    _T("Blank Ezx")

#define S_BLANK_NEPTUNE_LTS     _T("S Blank Neptune LTS")
#define NS_BLANK_NEPTUNE_LTS    _T("NS Blank Neptune LTS")
#define S_FLASH_NEPTUNE_LTS     _T("S Flash Neptune LTS")
#define NS_FLASH_NEPTUNE_LTS    _T("NS Flash Neptune LTS")

#define S_BLANK_NEPTUNE_LTE     _T("S Blank Neptune LTE")
#define NS_BLANK_NEPTUNE_LTE    _T("NS Blank Neptune LTE")
#define S_FLASH_NEPTUNE_LTE     _T("S Flash Neptune LTE")
#define NS_FLASH_NEPTUNE_LTE    _T("NS Flash Neptune LTE")

#define S_BLANK_NEPTUNE_LTX     _T("S Blank Neptune LTX")
#define NS_BLANK_NEPTUNE_LTX    _T("NS Blank Neptune LTX")
#define S_FLASH_NEPTUNE_LTX     _T("S Flash Neptune LTX")
#define NS_FLASH_NEPTUNE_LTX    _T("NS Flash Neptune LTX")

#define S_BLANK_NEPTUNE_ULS     _T("S Blank Neptune ULS")
#define NS_BLANK_NEPTUNE_ULS    _T("NS Blank Neptune ULS")
#define S_FLASH_NEPTUNE_ULS     _T("S Flash Neptune ULS")
#define NS_FLASH_NEPTUNE_ULS    _T("NS Flash Neptune ULS")

#define NS_FLASH_BULVERDE       _T("NS Flash Bulverde")
#define SE_FLASH_BULVERDE       _T("SE Flash Bulverde")
#define FLASH_ESPOO             _T("Flash ESPOO")
#define S_FLASH_ESPOO			_T("S Flash ESPOO")
#define NS_FLASH_ESPOO			_T("NS Flash ESPOO")
#define BLANK_BULVERDE          _T("BULVERDE")

#define BLANK_LOCOSTO           _T("Blank LOCOSTO")

#define BLANK_OMAP24XX          _T("Blank OMAP24xx")
#define BLANK_OMAP24            _T("Blank OMAP24")
#define BLANK_OMAP3430          _T("Blank OMAP3430")
#define BLANK_NEPTUNE           _T("Blank NEPTUNE")

#define MOTOPRO                 _T("Motopro")
#define RNDIS_PHONE             _T("Motorola USB Remote NDIS Network Device")
#define TI_OMAP					_T("OMAP")


//************Processor IDs*************//

#define UNKNOWN_PROC_ID     -1
#define ARGON_PROC_ID       0x14

//**************************************//

//secure-it header file defines
#define MAX_DATA_BLOCKS         128
#define DCML                     10
#define INVALID_HARDWARE        -1
#define READ_BINARY             _T("rb")
#define READ_WRITE_BIN          _T("rb+")
#define SECURE_IT_VER           _T("Secure-It for UNIX  v1.00")
#define DEFAULT_VER_NUM         1.00
#define DEFAULT_VER             _T("1.00")
#define MAX_VER                 1.10
#define COMMA_NEWLINE           _T(",\n")
#define MAX_PERCENT             100
#define MAX_SUPER_FILES         258

#define FLASH_THREAD_TIMER      500L
#define POSITION_SIZE           512
#define RAM_BUFFER              0xFFFF
#define SET_FILE_ERR            0xFFFFFFFF
#define CHECKSUM_MASK           0xFF
#define CHECKSUM_MAX            256
#define HEX_CHAR_MAX            16
#define BLANK_BARKER_CHKSUM     0x07F8


// Pseudosuperfile defines
#define NAME_SIZE             256
#define VERSION_SIZE          32
#define DATE_SIZE             128
#define DESC_SIZE             256
#define BLOCK_SIZE            64
#define MIN_BOOT_VER_SIZE     32
#define DATA_BLOCK_SIZE       128
#define TOTAL_CODE_GROUPS     256
//#define FILE_PATH_SIZE        256

//CDMA1x interrupt vection size.
#define CDMA1X_INTERRUPT_VECTOR_SIZE 0x20

// Memory Map Addresses
#define RAM_START_ADDR              0x41000000
#define RAM_END_ADDR                0x4100FFFF
#define FLASH_LOADER_START_ADDR     0x40000000
#define FLASH_LOADER_END_ADDR       0x40003FFF
#define JUMP_ADDR                   0x41008010

//Memory Size
#define RAM_SIZE                    0xFFFF
#define FLASH_BOOT_SIZE             0x3FFF


// Extra Information bit masks
#define ROM_BIT_MASK				0x01u
#define HAS_BARKER_BIT_MASK			0x02u
#define HAS_CDMA1X_BARKER_BIT_MASK  0x03u
#define HAS_FDI_TEMPLATE_MASK		0x04u
#define ALT_CHECKSUM_MASK			0x08u
#define HAS_SMALL_PAGE_RW_MASK		0x10u
#define HAS_LARGE_PAGE_RW_MASK		0x20u
#define HAS_SMALL_PAGE_RO_MASK		0x40u
#define HAS_LARGE_PAGE_RO_MASK      0x80u
#define HAS_PAGE_MASK               0xF0u

// Extra Information strings
#define REGULAR_SF_STRING       "Regular File"
#define COMBO_SF_STRING         "Combo File"

#define MULTI_INT_SF_STR        "Multi-interface SF"
#define NOT_MULTI_INT_SF_STR    "Not Multi-interface SF"

#define NO_RAM_DOWNLOADER       0xFF

//Image File Names
#define RAM_DLD_NAME            _T("RAMDLD.smg")
#define FLASHBOOTLOADER_NAME    _T("FBL.smg")
#define CODE_GROUP_NAME         _T("CG")
#define IMAGE_HDR_NAME          _T(".hmg")
#define MULTI_RAM_DLD_NAME      _T("RDL")
#define MULTI_BOOT_NAME         _T("FBL")

//Dialog Names
#define CODE_GROUP_STR          _T("Code Group ")

//Platform type based on MA and Chipset Type Strings *** These strings are used on Flash GUI ***
#define CDMA_WALLY          _T("CDMA Wally")
#define CDMA_MSM5100        _T("CDMA MSM5100")
#define CDMA_MSM5105        _T("CDMA MSM5105")
#define TDMA_PATRIOT        _T("TDMA Patriot")
#define GSM_PATRIOT         _T("GSM Patriot")
#define UMTS_RAINBOW        _T("UMTS Rainbow")
#define UMTS_RAINBOW_POG    _T("UMTS POG")
#define UMTS_OMAP           _T("UMTS OMAP")
#define GSM_LT_NEPTUNE      _T("GSM Neptune LT")
#define GSM_LTS_NEPTUNE     _T("GSM Neptune LTS")
#define GSM_LTE_NEPTUNE     _T("GSM Neptune LTE")
#define GSM_LTX_NEPTUNE     _T("GSM Neptune LTX")
#define GSM_ULS_NEPTUNE     _T("GSM Neptune ULS")
#define GSM_DALHART         _T("GSM Dalhart")
#define BLANK_WALLY         _T("Blank CDMA Wally")
#define BLANK_PATRIOT       _T("Blank Patriot")
#define BLANK_RAINBOW       _T("Blank Rainbow")
#define BLANK_LT_NEPTUNE    _T("Blank Neptune LT")
#define BLANK_LTS_NEPTUNE   _T("Blank Neptune LTS")
#define BLANK_LTE_NEPTUNE   _T("Blank Neptune LTE")
#define BLANK_LTX_NEPTUNE   _T("Blank Neptune LTX")
#define BLANK_ULS_NEPTUNE   _T("Blank Neptune ULS")
#define BLANK_POG           _T("Blank POG")  // Only for POG 2.0 Development IC
#define BLANK_OMAP          _T("Blank OMAP")
#define UNKNOWN_MA          _T("Unknown Platform")

//Security
#define SECURITY_ID     0x0002

#define MAX_BARKER      16

//Friendly Interface Names
#define FRIENDLY_AP_OS  _T("AP-OS")
#define FRIENDLY_AP_FS  _T("AP-FS")
#define FRIENDLY_BP     _T("BP")


#define RDL_WAIT_ENTRY "RDLWaitTimeAfterJump"
//--------------------------------------------------------------------------------------------------
//                                          MACROS
//--------------------------------------------------------------------------------------------------

#define VERIFY_HANDLE(handle)   ((handle!=NULL) && (handle!=INVALID_HANDLE_VALUE))
#define VERIFY_DEVICE(id)       ((id >= MIN_DEVICE_ID) && (id <= MAX_DEVICE_ID))
#define USB_DEVICE(id)          ((id >= USB_MIN_DEVICE_ID) && (id <= USB_MAX_DEVICE_ID))
#define SERIAL_DEVICE(id)       ((id >= COM_MIN_DEVICE_ID) && (id <= COM_MAX_DEVICE_ID))


//--------------------------------------------------------------------------------------------------
//                                           ENUMS
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//                               STRUCTURES AND OTHER TYPEDEFS
//--------------------------------------------------------------------------------------------------

typedef struct
{
    CString         sFileName;
    CString         sDescription;
    CString         sFileType;
    CIO*            pfImageFile;
    DWORD           dwFileSize;
    unsigned short  uChecksum;
    unsigned long   ulStartAddr;
    unsigned long   ulEndAddr;
    unsigned long   ulActualEndAddr; //Actual end address of the image sent to SU
    BYTE            byBarker[ P_BARKER_CODE_SZ ];
    BYTE            byBarkerEx[ P_BARKER_CODE_SZ ];
    unsigned short  byBarkerChksum;
    SW_DESC         SWDesc;
    unsigned short  uExtraInfo;
    BOOL            bIsValid;
    BOOL            bRangeChecksum;
    BOOL            bHasFDI;
    BOOL            bRomSuperfile; //type of superfile indicator
    UINT8           uCodeGroupNum;
    UINT8           uDownLoaderNum;
    UINT8           uUSBinterfaceID;
} SUPER_IMAGES;

typedef struct
{
    BYTE            sFileName[MAX_PATH];
    WORD			wImageFileChecksum;
    BYTE            byBarkerEx[ P_BARKER_CODE_SZ ];
    BOOL            bIsValid;
} SUPER_IMAGES_HDR;

//----------------------------------------//
//  Boot command configuration structure  //
//----------------------------------------//

typedef struct
{
    DWORD Timeout;          // timeout to receive an answer for each boot cmd
    DWORD OverallRetries;   // retries to send/read command if anything fails
    DWORD ReceiveRetries;   // retries to read answer for command before failing
    DWORD SendRetries;      // retries to send command before failing
    DWORD TICmd;            // Associate TI Cmd
} BOOT_CMD_CFG_STRUC;


// New elements as added to INI File can be added to this structure
typedef struct
{
    unsigned int m_nRDLWaitTimeAfterJump;
    unsigned int m_nIOInterval;  //interval between each read/write operation
    
} WAIT_TIME_CFG;

typedef enum eBootCmd
{
    E_ADDR, //0
    E_BIN, //1
    E_BAUD,//2
    E_ERASE,//3
    E_MADDR,//4
    E_MBIN, //5
    E_POWER_DOWN,//6
    E_RQCS,//7
    E_RQHW,//8
    E_RQRC, //9
    E_RQSF,//10
    E_RQSN,//11
    E_RQVN,//12
    E_JUMP,//13
    E_RBRK,//14
    E_RESTART,//15
    E_RUPID,//16
    E_RQRCS, //17
    E_UPGRADE,//18
    E_WUPID,//19
    E_RQSW,//20
    E_RQSI,//21
    E_SIRFS_UP,//22
    E_ENUM,//23
    E_RQFI,//24
    E_RQFV,//25
    E_READ_OTP,//26
    E_READ,//27
    E_WRITE,//28
    E_RQPI,//29
    E_VERSN,//30
    E_PRDCT,//31
    E_CONFIG,//32
    E_IB_RQ_VERSION,//33
    E_IB_ST_ADDRS, //34
    E_IB_DATA_PACKET, //35
    E_IB_JUMP, //36
    E_IB_PREAMBLE, //37
    E_RQRCS_NAND, //38
    E_TI_LEN, //39
    E_TI_ADDR, //40
    E_TI_DATA, //41
    E_TI_RUN, //42
    E_TI_PLAT, //43
    E_TI_ACK, //44 
    E_TI_NACK,//45
    E_RQHSTAT,//46
    E_BP_PASS,//47
    E_TOT_BOOT_CMDS //48
}eBootCmdEnums;

class STR_DESC
{
public:
    CString sDevMan;
    CString sDevProd;
    CString sSerialNum;
    CString sConfig;
    CString sInterface;
    CString sDirectStr;
    STR_DESC operator = (STR_DESC &str_desc)
    {   sDevMan = str_desc.sDevMan;
        sDevProd = str_desc.sDevProd;
        sSerialNum = str_desc.sSerialNum;
        sConfig = str_desc.sConfig;
        sInterface = str_desc.sInterface;
        sDirectStr = str_desc.sDirectStr;
        return *this;
    }
};

typedef enum
{
    RAM_BOOTSTRAP_TYPE,
    RAM_BOOTLOADER_TYPE,
    FLASHBOOTLOADER_TYPE,
    PHONE_CODE_TYPE,
    LANG_PACK_TYPE,
    RESOURCE_PACK_TYPE,
    SUPER_FILE_TYPE,
} FILE_TYPES;


typedef enum
{
    BROWSE_EVENT,
    FLASH_EVENT,
    RESIZE_EVENT,
    CANCEL_EVENT,
    VERIFY_EVENT,
    NO_EVENT
} EVENT_TYPE;

typedef enum
{
    FDI_STATUS_OK,
    FDI_STATUS_UPGRADEABLE,
    FDI_STATUS_NOT_UPGRADEABLE,
    FDI_STATUS_UPGRADED,
    FDI_STATUS_SAVE_COMPLETE,
    FDI_STATUS_SAVE_ERROR,
    FDI_STATUS_ERASE_COMPLETE,
    FDI_STATUS_ERASE_ERROR,
    FDI_STATUS_TEMPLATE_FLASH_COMPLETE,
    FDI_STATUS_TEMPLATE_FLASH_ERROR,
    FDI_STATUS_CHECKSUM_MISMATCH_ERROR,
    FDI_STATUS_CHECKSUM_ERROR,
    FDI_STATUS_DATA_RESTORE_COMPLETE,
    FDI_STATUS_DATA_RESTORE_ERROR,
    FDI_STATUS_UNKNOWN_COMMAND,
    FDI_STATUS_UNKNOWN_ERROR
}FDI_STATUS;

typedef enum
{
    BP_OS_BUNDLE_INDEX,
    AP_FL_BUNDLE_INDEX,
    AP_OS_BUNDLE_INDEX,
    NEXT_BUNDLE_INDEX
} BUNDLE_INDEX;         //defines the order in the bundles

typedef enum
{
    BLF_MODEL,
    CSF_MODEL,
    MD5_MODEL
} SUPERFILE_DATA_TYPE;

//---------------------//
//  SU Info structure  //
//---------------------//


//--------------------------------------------------------------------------------------------------
//                               GLOBAL VARIABLE DECLARATIONS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                   FUNCTION PROTOTYPES
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//                                         CLASS
//--------------------------------------------------------------------------------------------------


#include <afxtempl.h>


//--------------------------------------------------------------------------------------------------
//                                         CONSTANTS
//--------------------------------------------------------------------------------------------------
#define CMN_USB_AP_OS                 0x23
#define CMN_USB_AP_FL                 0x24
#define CMN_USB_BP_OS                 0x25

#define AP_OS_ARR_INDEX                 0
#define AP_FL_ARR_INDEX                 1
#define BP_OS_ARR_INDEX                 2

#define MAXINT                          3
#define MAX_CG_INDEX                   256+2
#define MAX_DEVICE_TOTAL               128 // Maximum as needed by GNPO team
#define INVALID_INTER_NUM              8000

#define RAM_DOWNLDR_CG_NUM      -1
#define BOOTLDR_CG_NUM          -2

#define RAM_DOWNLDR_INDEX MAX_CG_INDEX-1
#define BOOTLDR_INDEX MAX_CG_INDEX-2


//--------------------------------------------------------------------------------------------------
#endif // SHARED_H
