#ifndef FLASH_COMMON_H
#define FLASH_COMMON_H

//--------------------------------------------------------------------------------------------------
//                                                                               
//   Header Name: FlashCommon.h
//
//   General Description: This header file contains shared information that is used 
//   of the SDK Flash Programmer. 
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
// Fathi Ben Amar               18/09/2006      LIBii99372   Header for all variables and structures 
// Stefan Jez                   09/02/2007      LIBll08737   Add new error msg ERR_API_DEPRECATED
// Norton Santos                08/07/2007      LIBff89694   Old PST related code removal
// Tingting Dong				17/07/2007		LIBmm76870   Add RQHSTAT command sending
// Xiang Ye                     11/08/2007      LIBnn61500   Add new error msg ERR_MESSAGEPUMPTHREAD_START
// Karthik Ramachandran			11/12/2007      LIBnn01226   Support for GreatWall BP PASS through mode flashing
// Zhiyong Yang                 11/21/2007      LIBoo08651   SAT-ODM: Add Blank mode support for ODM phones
// 
// Miguel Moretti               09/10/2007	    LIBnn17795   Added new blank error code
//--------------------------------------------------------------------------------------------------
//                                       INCLUDE FILES
//--------------------------------------------------------------------------------------------------
#pragma once
#include "pstcommon.h"
//--------------------------------------------------------------------------------------------------
//                                         CONSTANTS
//--------------------------------------------------------------------------------------------------

#define PST_DETECTED_ERROR                    0xE0030000

#define PST_DETECTED_MASK                     0xF00F0000
#define SU_DETECTED_MASK                      0xF00F0000
#define ERROR_DIR_MASK                        0x0000F000
#define ERROR_CODE_MASK                       0x00000FFF
#define ERROR_NUM_MASK                        0xF00F0FFF


#define ERROR_CMD_MASK                        0x0FF00000
#define ERROR_CMDDIR_MASK                     0x0FF0F000
#define ERROR_CMD_POS                         20 // number of bits from the right

#define FLERR_DEF_CG_NUM -256
#define FLDET_ERR_GENERR 256
#define FLERR_DEF_CMD_VALUE 0

#define INVALID_CG_NUM                -256
#define INVALID_DEV_ID                0xFFFF

#define DESCRPTR_TYPE_INDEX 0
#define CHIPSET_INDEX       1
#define MA_TYPE_INDEX       2
#define DATA_TYPE_INDEX     3
#define VERSION_INDEX       6

//defines for UID (RQSN processing)
#define UID_LENGTH 16
#define PHONE_UID_LENGTH 14
#define ULS_UID_LENGTH 6
#define ZAS_UID_LENGTH 7
#define PHONE_HABSIG_INDEX 0x0F
#define PHONE_HABSIG_INDEX_ZAS 0x07
#define ARGON_STR_ID "argon"
#define SCMA11_STR_ID "scm-a11"
#define ULS_STR_ID "uls"

// This message is handled by RSD Lite, that shows a pop-up box with the message 
// and logs its text.
const UINT WM_FLASHDLL_SHOW_MSGBOX  = ::RegisterWindowMessage(_T("WM_FLASHDLL_SHOW_MSGBOX"));


//--------------------------------------------------------------------------------------------------
//                                          MACROS
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//                                           ENUMS
//--------------------------------------------------------------------------------------------------
typedef enum eActionsAftFlash 
{
	E_POWERDN_SYNCHRONOUS=0, // Power down all phones synchronously
	E_RESTART_SYNCHRONOUS=1, // Restart all phones synchronously
    E_POWERDN_ASYNCHRONOUS=2, // Power down the individual phone which has 
                              //completed flashing and not wait for other phones
    E_RESTART_ASYNCHRONOUS=3, // Restart the individual phone which has completed 
                             //flashing and not wait for other phones
    E_SYNC_NOACTION_NEEDED=4, // For the GUI to keep threads synchronized for 
                             //multiple devices.
	E_ASYNC_NOACTION_NEEDED=5, //No synchronization for multiple devices
    E_RESTART_SYNC_NOWAITFORREENUM=6, // Synchronized restart commands only,
    //but will not wait for device re-enum. The method will return without 
    // waiting for re-enum.
    E_RESTART_ASYNC_NOWAITFORREENUM=7, // Asynchronous restart command,
    //but will not wait for device re-enum. The method will return without 
    // waiting for re-enum.
    E_PDN_AFTANY_ERROR_RESTART_ASYNC =8, // Power down after any error /restart asynchronous- needed for GNPO
    E_PDN_AFTANY_ERROR_PDN_ASYNC =9, // Power down after any error /pdn asynchronous- needed for GNPO
    E_RESTART_AFTANY_ERROR_RESTART_NOWAIT_SYNC=10, // Restart after any error / restart nowait synchronous - needed for GNPO    
    E_RESTART_AFTANY_ERROR_RESTART_NOWAIT_ASYNC=11, // Restart after any error / restart nowait asynchronous- needed for GNPO
    E_ACT_TOTAL//=12
} eActAftFlash;

typedef enum eExtraActionsDuringFlash 
{
    E_NO_EXTRA_ACTION   = 0,  // No extra action during flash
    E_EFUSE_BLOW        = 1,  // Blow e-fuses
    E_STOP_AFTER_RDL    = 2   // Stop after sending RAM downloader
} eExtActDuringFlash;

enum EMsgDir
{
    EDIR_XMIT_ERR=0x00001000,
    EDIR_RCV_ERR=0x00002000,
    EDIR_XMIT_TIMEOUT=0x00003000,
    EDIR_RCV_TIMEOUT=0x00004000,
    EDIR_RCV_BAD_DATA=0x00005000,
    EDIR_RCV_NO_DATA=0x00006000,
    EDIR_GEN_APP_ERR=0x00007000,
    EDIR_INVALID_ERR=0x00009000 
}; 

enum EFlashStatus
{
    FLASH_STATUS_OK = 0,
    FLASH_STATUS_FAILED=1,
    FLASH_STATUS_WRONG_PARAM_NUMBER=2,
    FLASH_STATUS_ADAPTER_LOAD_ERROR=3,
    FLASH_STATUS_INTERNAL_ERROR=4,
    ERR_DBG_LOG_CREATE=5,
    ERR_DBG_LOG_CREATE2=6,
    ERR_LOG_CREATE=7,
    ERR_LOG_CREATE2=8,
    ERR_IMAGEFILE_WRITE=9,
    ERR_IMAGEFILE_READ=10,
    ERR_REG_CREATE=11,
    ERR_REG_CREATE2=12,
    ERR_REG_SET_VALUE=13,
    ERR_REG_GET_VALUE=14,
    ERR_FLASH_FILE_PRCS=15,
    ERR_IMG_OPEN=16,
    ERR_IMG_VALID=17,
    ERR_FILEIO_NULL=18,
    ERR_FLASHDEV_NULL=19,
    ERR_FLASHTHREAD_CREATE=20,
    ERR_FLASHDLG_CREATE=21,
    ERR_GET_DIR=22,
    ERR_VER_QUERY=23,
    ERR_GET_VER=24,
    ERR_GLOB_ALLOC=25,
    ERR_VER_SIZE=26,
    ERR_FLASHDEV_INIT=27,
    ERR_RAM_BOOTSTRAP=28,
    ERR_RAM_BOOTLOADER=29,
    ERR_FLASH_SU=30,
    ERR_ERASE_SU=31,
    ERR_SU_INFO=32,
    ERR_SU_CHKSUM=33,
    ERR_PWR_DOWN=34,
    ERR_REBOOT=35,
    ERR_REENUM_SET_EVENT=36,
    ERR_FILE_PTR_SET=37,
    ERR_REFRESH_CONNECTION=38,
    ERR_INTF_HANDLE=39,
    ERR_PROD_STR_DESC=40,
    ERR_INI_FILE=41,
    ERR_STR_DESC_NULL=42,
    ERR_BARKER_CODE=43,
    ERR_RAMDLD_CHKSUM=44,
    ERR_RAMDLD_CHKSUM_RET=45,
    ERR_VER_BL_CHKSUM=46,
    ERR_BL_CHKSUM=47,
    ERR_VER_SIRF_CHKSUM=48,
    ERR_VER_CGRP_CHKSUM=49,
    ERR_NO_SU_CHKSUM=50,
    ERR_INVALID_SUPER_FILE=51,
    ERR_INVALID_NUM_FILES=52,
    ERR_CORRUPT_SUPER_FILE=53,
    ERR_REPROG_BARKER=54,
    ERR_LANG_PKG_MISMATCH=55,
    ERR_SF_NAKED_MISMATCH=56,
    ERR_SF_REFLASH_MISMATCH=57,
    ERR_SF_TYPE_MISMATCH=58,
    ERR_MI_SF_INCOMPATIBLE=59,
    ERR_SIRF_FLASH=60,
    ERR_SEC_VER_CHECK=61,
    ERR_RDL_REENUMERATION=62,
    ERR_MBM_REENUMERATION=63,
    ERR_SF_MD5_SECURITY_MISSING=64,
    ERR_RQSN_BOOT_CMD=65,
    ERR_RQSF_BOOT_CMD=66,
    ERR_INV_NUM_FLASHPART=67,
    ERR_INQ_TIMED_OUT=68,
    ERR_OUTQ_TIMED_OUT=69,
    ERR_SENDING_MADDR=70,
    ERR_SENDING_MBIN=71,
    ERR_REMOUTQCOUNT=72,
    ERR_REMINQCOUNT=73,
    ERR_REMFROMOUTQ=74,
    ERR_REMFROMINQ=75,
    ERR_ADDOUTQCOUNT=76,
    ERR_ADDINQCOUNT=77,
    ERR_ADDTOOUTQ=78,
    ERR_ADDTOINQ=79,
    ERR_FLASHPART_ALLOC=80,
    ERR_UNKNOWNSFEXT=81,
    ERR_IO_ALLOC=82,
    ERR_FLASHFILE_ALLOC=83,
    ERR_CALC_CHKSM_NOTMATCH=84,
    ERR_PHONE_PWR=85,
    ERR_UNKNOWN_MA_TYPE=86,
    ERR_INVALID_SW_DESC=87,
    ERR_FILE_TYAXMISMATCH=88,
    ERR_FILE_MICRONMISMATCH=89,
    ERR_CHIPSET_TYPE_MISMATCH=90,
    ERR_MA_TYPE_MISTMATCH=91,
    ERR_DATA_TYPE_CG_MISMATCH=92,
    ERR_DATA_TYPE_CG_CONTINUE=93,
    ERR_SUPERFILE_RDL_MISSING=94,
    ERR_SECURITY_TYPE_MISMATCH=95,
    ERR_NO_SU_CONN=96,
    ERR_BAUD_RATE=97,
    ERR_BAUD_SU=98,
    ERR_BAUD_PC=99,
    ERR_MAX_BARKER=100,
    ERR_RDL_JMP=101,
    ERR_INTF_NOT_FOUND=102,
    ERR_FDI_STAT_CMD=103,
    ERR_FDI_RESTORE_CMD=104,
    ERR_FDI_CHECKSUM_MISMATCH=105,
    ERR_FDI_CHECKSUM_REQ=106,
    ERR_FLASHING_FDI_DATA=107,
    ERR_FDI_ERASE=108,
    ERR_FDI_SAVE=109,
    ERR_FDI_NOT_UPG=110,
    ERR_FDI_UPGRADE_CMD=111,
    ERR_LOAD_MANIFEST=112,
    ERR_LOAD_AP_RDL=113,
    ERR_SUPERFILE_UNSECURE=114,
    ERR_INTF_THREAD=115,
    ERR_SUPERFILE_UNSIGNED=116,
    ERR_UID_MISTMATCH=117,
    ERR_UID_CREATE=118,
    ERR_JMP_SEND=119,
    ERR_FLASH_FAIL=120,
    ERR_SU_REBOOT=121,
    ERR_SU_POWERDOWN=122,
    ERR_VER_CHKSM=123,
    ERR_VER_SU_SF=124,
    ERR_SUPERFILE_NOTEXISTING=125,    
    ERR_TRIPLETS_FLASH_STOP=126,    
    ERR_INVALID_BOOTL_BARKER=127,
    ERR_SEND_CONFIG_CMD=128,
    ERR_BLOB_CG_NOT_FOUND = 129,
    ERR_SEND_INTEL_PREAMBLE = 130,
    ERR_SEND_INTEL_VERSION = 131,
    ERR_SEND_INTEL_START_ADDRESS = 132,
    ERR_SEND_INTEL_DATA = 133,
    ERR_SEND_INTEL_JUMP = 134,
    ERR_WRONG_METHOD_CALLED_IN_BATCH=135,
    ERR_WRONG_OPERN_AFTR_FLASH=136,
    ERR_ADDING_DEVICE=137,
    ERR_REENUM_DEVICE=138,
    ERR_REMOVING_DEVICE=139,
    ERR_MINTF_TIMEDOUT=140,
    ERR_SETTING_BOOTCMD_CONFIG=141,
    ERR_NULL_IMG_FILE_PTR=142,
	ERR_RQSW_BOOT_CMD=143,
    ERR_PROD_CMD=144,
    ERR_BLANK_NA = 145,
    ERR_ALLOC_MEMORY=146,
    ERR_PASSED_PARAM_NULL=147,
    ERR_IMAGEFILE_SEEK=148,
    ERR_IMAGEFILE_CREATE_PACKET=149,
    ERR_SEND_TI_DATA=150,
    ERR_RCV_TI_DATA=151,
    ERR_GET_CONFIG_CMD=152,
    ERR_ENUM_SER_DEVICE=153,
    ERR_START_SER_DEVICE=154,
    ERR_STOP_SER_DEVICE=155,
    ERR_WRONG_PARAM_VALUE=156,
    ERR_DEVICE_NOT_SUPPORTED=157,
    ERR_INVALID_SERIAL_PARAMS=158,
    ERR_RDL_CHECK=159,
    ERR_BAUD_MBM=160,
    ERR_BAUD_CFG=161,
    ERR_REG_NOTIFICATION=162,
    ERR_FLASHADAPTER_NULL=163,
    ERR_TYAX_INTO_SIBLEY=164,
    ERR_SIBLEY_INTO_TYAX=165,
    ERR_INVALID_FLASH_PARAMS=166,
    ERR_LOADING_XML_FILE=167,
    ERR_UPGRADING_BL=168,
    ERR_NO_BL_INFO_IN_XML=169,
    ERR_RAMDL_FLASH=170,
    ERR_CREATING_XML_CLASS=171,
    ERR_XML_CLASS_NULL=172,
    ERR_CREATING_BL_UPGRADE_CLASS=173,
    ERR_CREATING_RDL_UPGRADE_CLASS=174,
    ERR_RAMDL_EXTRACTION=175,
	ERR_CREATING_XML_FILE_DETAILS_CLASS=176,
	ERR_CREATING_XML_FILE_HDLR_CLASS=177,
    ERR_EXTRA_INFO_BAD_VALUE=178,
	ERR_SEND_ENUM_CMD=179,
    ERR_FLASHTHREAD_START=180,
    ERR_FLASHTHREAD_INSTANCE=181,
    ERR_STR_ARRAY_SIZE=182,
    ERR_UID_INFO=183,
    ERR_BOOT_INFO=184,
    ERR_PDS_INFO=185,
    ERR_API_DEPRECATED=186,
	ERR_RQHSTAT_SEND=187,
    ERR_SEND_TI_CMD=188,
    ERR_MESSAGEPUMPTHREAD_START=189,
    ERR_BP_PASS_MODE=190,

    /* this items are the last error items. if new error codes are added, it should be above these items*/
    ERR_INDEX_OUT_OF_BOUND = 191,
    FLASHERRENUM=192
};

enum EProgressBarType
{
    SUB_TASK_PROGRESS_BAR=0,            // Flash DLL will start and stop the progress bar multiple times for each flashing operation sub-task
    TOTAL_CONSOLIDATED_PROGRESS_BAR=1,  // Progress bar will be started at the beginning of flashing and continue till the end 
    INVALID_PROGRESS_BAR=2              // Used to check if the value is valid (values greater or equal than INVALID_PROGRESS_BAR are invalid) 
};

//Image File Index
enum{
    RAM_DOWNLOADER_INDEX = 0,
    FLASHBOOTLOADER_INDEX,
    CODE_GROUP_0,
    CODE_GROUP_1,
    CODE_GROUP_2,
    CODE_GROUP_3,
    CODE_GROUP_4,
    CODE_GROUP_5,
    //add additional code groups here
    //note: it is only necessary to add additional code groups here if the super file 
    //image information is to be accessed.  A code group will be flashed even if
    //it is not represented in this enumeration.
    MAX_CODE_GROUP = 256
};

typedef enum
{
    HABSIG_NO_HAB = 0x00,
	HABSIG_NOT_SIGNED = 0x01,
    HABSIG_SIGNED_PRODUCTION = 0x02,
	HABSIG_SIGNED_BOUND = 0x03,
	HABSIG_DISABLED = 0x08
}HAB_SIGNATURE_TYPE;
//--------------------------------------------------------------------------------------------------
//                               STRUCTURES AND OTHER TYPEDEFS
//--------------------------------------------------------------------------------------------------

// Higher level structures additions for end-users of the API
typedef WORD DEVICE_ID;

//-- MNR - CGSce61035
typedef BYTE SW_CG_VERSION[8];
//  This contains the hardware versions
//  and software versions of the SU

typedef struct {
	CString strFlashVersion;
	CString strFlexVersion;
    // Same as DeviceID Datatype only 
    DEVICEID DeviceID;
} BOOT_FLASH_FLEX_STRUC;

typedef struct BOOT_PDS_INFO_STRUC {
	BYTE * pbyESN;
	DWORD dwESNLen;
	BYTE * pbyFactoryTrackingInfo;
	DWORD dwFactoryTrackingInfoLen;
	BYTE * pbyIMEI;
	DWORD dwIMEILen;

	BOOT_PDS_INFO_STRUC()
	{
		pbyESN = NULL;
		dwESNLen = 0;
		pbyFactoryTrackingInfo = NULL;
		dwFactoryTrackingInfoLen = 0;
		pbyIMEI = NULL;
		dwIMEILen = 0;
	}

	~BOOT_PDS_INFO_STRUC()
	{
		delete [] pbyESN;
		delete [] pbyFactoryTrackingInfo;
		delete [] pbyIMEI;
	}
} BOOT_PDS_INFO_STRUC;




typedef struct {
    BYTE  byHWdescriptors[8];
    BYTE  byRunningBootVer[8];
    BYTE  byFlashBootVer[8];
    BYTE  byPhoneSWpackage[8];
    SW_CG_VERSION aSWcodeGroupVer[MAX_CODE_GROUP]; // SW code group 0 to 255
} SU_INFO_STRUC;

//--------------------------------------------------------------------------------------------------
//                               GLOBAL VARIABLE DECLARATIONS
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//                                   FUNCTION PROTOTYPES
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//                                         CLASS
//--------------------------------------------------------------------------------------------------
class CFlashErr
{
public:
    // Constructor and Destructor
    CFlashErr()
    {
        ResetErrorDetails();
    };

    // Set Codegroup Number
    inline void SetCGNum(short sCGNum)
    {
        if(!m_bLock)
            m_sCGNum=sCGNum;
    };

    // Set Flash Status Error Number
    inline void SetFlashStatusError(EFlashStatus eErrNum)
    {
        if(!m_bLock)
            m_eErrNum=eErrNum;
    };

    // Set Error Message
    inline void SetErrorMessage(LPCTSTR szErrMsg)
    {
        if(!m_bLock)
            ::lstrcpy(m_szErrMsg, szErrMsg);
    };

    // Set Error
    inline void SetError(DWORD dwErrNum)
    {
       if(!m_bLock)
            m_dwErrNum=dwErrNum;
    };

    // Set Error Direction
    inline void SetDirection(EMsgDir eDirection)
    {
        if(!m_bLock)
            m_eDirection=eDirection;
    };

    // Set Command Value
    inline void SetCmdValue(int nCmdValue)
    {
        if(!m_bLock)
            m_nCmdValue=nCmdValue;
    };

    // Get Error Message
    inline CString GetErrorMessage()
    {
        return m_szErrMsg;
    };

    // Get Flash Status Error Number
    inline EFlashStatus GetFlashStatusError()
    {
        return m_eErrNum;
    };

    // Lock object
    inline void Lock()
    {
       m_bLock=TRUE;
    };

    // Unlock object
    inline void Unlock()
    {
        m_bLock=FALSE;
    };

    void SetErrorDetails()
    {
        if(!m_bLock)
        {
            // Check if m_dwErrNum is valid
            if(m_dwErrNum != FLDET_ERR_GENERR)
            {
                // Command Value
                if(m_nCmdValue == FLERR_DEF_CMD_VALUE)
                    m_nCmdValue = m_dwErrNum & ERROR_CMD_MASK;
       
                // Error Direction
                m_eDirection = (EMsgDir)(m_dwErrNum & ERROR_DIR_MASK);
   
                // Error Number
                m_dwErrNum= (m_dwErrNum & ERROR_NUM_MASK);
            }
            else
            {
                // Error Number
                m_dwErrNum = m_eErrNum;
            }
        }
    }

    CString GetErrorDetailsMessage()
    {
        CString strErrMsg,
                strCodeGroup,
                strDirection;

        // Codegroup Number
        if (m_sCGNum != FLERR_DEF_CG_NUM)
            strCodeGroup.Format("%d", m_sCGNum);
        else
            strCodeGroup.Format("No Codegroup");

        // Error Direction
        if ((m_eDirection != EDIR_GEN_APP_ERR) &&
            (m_eDirection != EDIR_INVALID_ERR))
            strDirection.Format("%x", (unsigned long)m_eDirection);
        else
            strDirection.Format("No Direction");

        // Detailed Error Message
        strErrMsg.Format("Flash failure: %s (Error Code: %x),\nDetailed Error Details: Direction of the Error=%s, Command Value=%x, Code Group Number=%s",
                         m_szErrMsg, m_dwErrNum, strDirection, m_nCmdValue, strCodeGroup);
    
        return strErrMsg;
    };

    void ResetErrorDetails()
    {
        m_eErrNum=FLASH_STATUS_OK;
        m_dwErrNum=FLDET_ERR_GENERR;
        m_nCmdValue=FLERR_DEF_CMD_VALUE;
        m_sCGNum=FLERR_DEF_CG_NUM;
        m_eDirection=EDIR_GEN_APP_ERR;
        memset(m_szErrMsg,'\0',MAX_PATH);

        m_bLock=FALSE;
    };

    CFlashErr& operator=( const CFlashErr &error )
    {
        ResetErrorDetails();

        m_eErrNum = error.m_eErrNum;
        m_dwErrNum = error.m_dwErrNum;
        m_nCmdValue = error.m_nCmdValue;
        m_sCGNum = error.m_sCGNum;
        m_eDirection = error.m_eDirection;
        memcpy(m_szErrMsg, error.m_szErrMsg, MAX_PATH);
    
        return *this;
    };

public:
    EFlashStatus m_eErrNum;     // EFlashStatus error
    char m_szErrMsg[MAX_PATH];  // Error Message
    DWORD m_dwErrNum;           // Error Number - No error is zero. 
    //Values other than detailed error numbers will be General Application error-256 - &H100
    unsigned int m_nCmdValue;   // Command Value - Invalid/default value will be 0.
    EMsgDir m_eDirection;       // Enum value containing XMIT or RECV 
                                // or General Application Error.- Invalid/default value will be 256.
    short m_sCGNum;             // Code Group Number filled in wherever possible.
                                // 256 will be Bootloader and 257 will be RamDownloader.
protected:
	BOOL m_bLock;               // If TRUE, no errors can be set anymore
};



class CFlashFileInfo
{
//Made public b'cos of need of quick access...
public:
	CString m_strFileName;
	CString m_strFileDate;
	CString m_strFileSize;
};


class CFlashSuperFileInfo
{
	//Made public b'cos of need of quick access...
public:
	CString m_strFileName;
	CString m_strRAMDLVer;
	CString m_strBootVer;
	CString m_strPlatFormType;
	CString m_strHABSigType;
// Flash Files creation time
    double m_dFlashFileCreationTime;
};

typedef CArray <unsigned short, unsigned short> unsShortArray;
typedef CArray <short,short> ShortArray;

class IndexList
{
public:
    ShortArray m_IndexArray;
};

class DeviceIDList
{
public:
    unsShortArray m_DevIndexes;
};

class CGIntItem
{
public:
    CGIntItem()
    {
        m_bCGDetailsFilled=false;
        m_dCGSize=0;
    // The value in Seconds
        m_dFlashRate=0;
        m_dFlashTime=0;
        m_sIntNum=0;
    };

    virtual ~CGIntItem(){};

    // The value in MB
	double m_dCGSize;
    // The value in Seconds for each Code Group.
	double m_dFlashTime;
    // Flash rate - MB/Sec rate value calculated from CGSize and FlashTime
    double m_dFlashRate;
    // Interface number - Will be set to one of the 3 HASHDEF Values
    unsigned short m_sIntNum;
    bool m_bCGDetailsFilled; // If CG details are filled, it means that the CG exists in the file.
};


class InterfaceItem 
{
public:
    InterfaceItem()
    {
       m_dEraseTime=0;
       m_bIntDetailsFilled=false;
    };
    
    virtual ~InterfaceItem(){};

    // Time taken for erasing the interface
    double m_dEraseTime;
    bool m_bIntDetailsFilled; // Interface details filled or not. 
};


class CGItem : public CGIntItem
{
public:    
    CGItem()
    {
        m_sCGNum=0;
    };

    // Code Group number
    short m_sCGNum;
};

class IntItem : public InterfaceItem
{
public:    
    IntItem()
    {
        m_sIntNum=0;
    };

    // Interface number
    unsigned short m_sIntNum;
};

class CDeviceBaseDetails
{
public:
    CDeviceBaseDetails()
    {
        m_bRestartAckObtained=false;
        m_dTimeforFlashMode=0;
        m_dTimeforReEnumeration=0;
        m_dTotalFlashTime=0;
    };

    virtual ~CDeviceBaseDetails(){};

    // Device Restart Ack obtained on Restart
    bool m_bRestartAckObtained;

    // Total time for the device to flash(From Start to End and not actual flash time alone)
    double m_dTotalFlashTime;
    // Total time taken by device to switch to flash mode
    double m_dTimeforFlashMode;
    // The value in Seconds - For Reenumeration
	double m_dTimeforReEnumeration;
};


typedef CArray <IntItem*, IntItem*&> IntfArray;
typedef CArray <CGItem*,CGItem*&> CGArray;


class CDeviceDetails : public CDeviceBaseDetails 
{
public:
    IntfArray m_IntfArray;
    CGArray m_CGArray;
    DEVICE_ID m_sDeviceID;
    
    CDeviceDetails()
    {
        m_sDeviceID=0; 
    };

    virtual ~CDeviceDetails()
    {
        RemoveAllElements(true);
        RemoveAllElements(false);
    };

    bool RemoveAllElements(bool bFromCG)
    {
        int nSize=0;
        CGItem *pCGItem=NULL;
        InterfaceItem *pIntItem=NULL;

        unsigned short sLoopCnt=0;


        // Cleanup the array, if elements exist already...
        if(bFromCG)
        {
            nSize=m_CGArray.GetSize();
            if(nSize > 0)
            {
                for(sLoopCnt=0; sLoopCnt < nSize;sLoopCnt++)
                {
                    pCGItem=NULL;
                    pCGItem=m_CGArray.GetAt(sLoopCnt);
                    delete pCGItem;
                }
            }

            m_CGArray.RemoveAll();
        }
        else
        { 
            // Cleanup the array, if elements exist already...
            nSize=m_IntfArray.GetSize();
            if(nSize > 0)
            {
                for(sLoopCnt=0; sLoopCnt < nSize;sLoopCnt++)
                {
                    pIntItem=NULL;
                    pIntItem=m_IntfArray.GetAt(sLoopCnt);
                    delete pIntItem;
                }
            }
            m_IntfArray.RemoveAll();
        }
        return true;
    };
};

//--------------------------------------------------------------------------------------------------
#endif // FLASH_COMMON_H
