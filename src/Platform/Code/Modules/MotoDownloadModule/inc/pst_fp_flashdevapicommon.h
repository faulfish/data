#ifndef DEVICE_API_COMMON_H
#define DEVICE_API_COMMON_H

//--------------------------------------------------------------------------------------------------
//                                                                               
//   Header Name: PST_FP_FlashApiCommon.h
//
//   General Description: This contains enums, data structures and constants that are common and
//                        used by both the application and Flash Device API.
//
//--------------------------------------------------------------------------------------------------
//
//                            Motorola Confidential Proprietary
//                       Advanced Technology and Software Operations
//                    (c) Copyright Motorola 2005 - 2010, All Rights Reserved
//   
//
// Revision History:
//                             Modification     Tracking
// Author                          Date          Number     Description of Changes
// -------------------------   ------------    ----------   ----------------------------------------
// Manny Roxas                 04/06/2000      CSGceXXXXX   Initial version.
// Manny Roxas                 06/08/2000      CSGse61035   Add support to process checksums and
//                                                          versions for all SW code groups.
// Manny Roxas                 07/11/2000      CSGce60032   PST Operation via RS232 transport.
//
// Marc Risop                   5/8/2001        CSGce86207  Add Language package flashing capability
//
// Jagat Shah                  09/04/2001      LIBbb00391   Added MAtype and Chipset type identifiers
//                                                          For RAINBOW.
// Anthony Leung               11/19/2001      LIBbb10838   Add Chipset identifiers for LCA NEPTUNE
//
// Sundar Ramalingam           01/03/2002      LIBbb10409   Declare CDMA1xtype in enumeration function
//
// Steve Scherer               03/01/2002      LIBbb21472   Deleted Chip_set and MA_type definitions.
//                                                          These are defined in pst_fp_shared.h.
// Scott DeBates			   06/18/2002	   LIBbb38685	Added a new define for number of retries
// Mike Zarletti			   05/05/2003	   LIBbb63862	Added struct for use with RQSW command
//
// Dindo Jao                   10/14/2003      LIBdd40378   Removed MAX_CODE_GROUP definition since it is already in
//                                                          PST_FP_Shared.h. Included PST_FP_Shared.h header file. 
// Satya Calloji               03/30/2005      LIBff71882   For RQVN Issue
//
// Tiago Armani                06/23/2005      LIBgg13330   Defined JUMP_CMD_RETRY as 1
//
// Norton Santos               12/20/2005      LIBhh48672   Moved BYTE SW_CG_VERSION, SU_INFO_STRUC and BOOT_FLASH_FLEX_STRUC
//                                                          from "PST_FP_FlashApiCommon.h" to "PST_FP_Shared.h" 
//
// Satya Calloji               12/30/2005      LIBgg64966   Changing the RQVN logic to improve yield as per GNPO logic
//
// Samir Khial                 02/15/2006      LIBgg77383   Rework ODM Naked/Reflash support  
//
// Fathi Ben Amar              09/18/2006      LIBii99372   Reorganization of include file 
// Norton Santos               08/07/2007      LIBff89694   Old PST related code removal
//
//--------------------------------------------------------------------------------------------------
//                                       INCLUDE FILES
//--------------------------------------------------------------------------------------------------
#include "flashcommon.h"
//--------------------------------------------------------------------------------------------------
//                                         CONSTANTS
//--------------------------------------------------------------------------------------------------

//-------------------//
//  Flash File type  //
//-------------------//
#define FLASH_BOOTLOADER  1
#define PHONE_CODE_HEADER 2
#define PHONE_CODE_IMAGE  3
#define CHAR_ROM_FILE     4
#define LANGUAGE_PACK     5
#define MEMORY_BIT_MAP    6

// MNR - CSGce60032
#define SERIAL_COM_DELAY   100

#define CMD_RETRY	    3	//Number of times to retry sending data
#define JUMP_CMD_RETRY  1   //Number of times to retry sending jump cmd
#define SUINFO_RETRY    1   //Number of times for overall retry for GetSubscriberUnitInfo function

#define UID_LENGTH 16
//--------------------------------------------------------------------------------------------------
//                                          MACROS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                           ENUMS
//--------------------------------------------------------------------------------------------------

//------------------------//
// Erase Type enumeration //
//------------------------//
enum EraseTypeEnum
{
    GeneralErase,
    UPIDerase,
    LanguagePackageErase
};

//------------------------//
//   MA Type enumeration  //
//------------------------//
enum MAtypeEnum
{
    CDMAtype,
    GSMtype,
    TDMAtype,
	UMTStype,
	CDMA1Xtype
};

//--------------------------------------------------------------------------------------------------
//                               STRUCTURES AND OTHER TYPEDEFS
//--------------------------------------------------------------------------------------------------

//-----------//
// Checksums //
//-----------//

//-- MNR - CSGce61035
typedef struct{
    BOOL bCodeGroup;
    WORD wCGchksum;
} SW_CG_CKSUM_STRUC;

typedef struct
{
    BOOL bBoot;
    WORD wBoot;
    SW_CG_CKSUM_STRUC astructCDcksum[MAX_CODE_GROUP];
} CHECKSUMS_STRUC;


//-- end of CSGce61035

//--------------------------------------------------------------------------------------------------
//                               GLOBAL VARIABLE DECLARATIONS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                   FUNCTION PROTOTYPES
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                         CLASS
//--------------------------------------------------------------------------------------------------

#endif // DEVICE_API_COMMON_H
