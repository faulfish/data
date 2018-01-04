#if !defined(FLASH_API_INCLUDED)
#define FLASH_API_INCLUDED

#ifndef __cplusplus
#error "This is a C++ header file; it requires C++ to compile."
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
 
//--------------------------------------------------------------------------------------------------
//                                                                               
//   Header Name: Flash_API.h
//
//   General Description: This header file will be used by clients of the flash component.  
//                          Applications wishing to use the flash dialog class defined for the 
//                          "user tool" concept must also include pst_fp_flash.h.
//
//--------------------------------------------------------------------------------------------------
//
//                            Motorola Confidential Proprietary
//                       Advanced Technology and Software Operations
//                    (c) Copyright Motorola 2000 - 2004, All Rights Reserved
//   
//
// Revision History:
//                             Modification     Tracking
// Author                          Date          Number     Description of Changes
// -------------------------   ------------    ----------   ----------------------------------------
// Marc Risop                   05/24/2004     LIBee05617   This header file will be used by clients of the
//                                                          flash component.  Applications wishing to use the
//                                                          flash dialog class defined for the "user tool" concept
//                                                          must also include pst_fp_flash.h.
//
// Dindo Jao                    11/10/2004     LIBee41450   Move EFlashStatus enum.
//
// Satya Calloji                12/03/2004     LIBff29751   Added Common Flash APIs.
//
// Satya Calloji                12/22/2004     LIBff47956   Added Error Handling changes
// Tiago Armani                 12/16/2004     LIBff39875   Added include for PST_FP_Shared.h
//
// Tiago Armani                 12/16/2004     LIBff39875   Added include for PST_FP_Shared.h
//
// Tiago Armani                 12/16/2004     LIBff39875   Added include for PST_FP_Shared.h
//
//
// Satya Calloji                04/04/2005     LIBff65906   Restart issue changes.
//
// Satya Calloji                06/20/2005     LIBgg33477   Added Image files delete method
//
// Norton Santos                11/10/2005     LIBff67978   Added APIs: GetBootFlashFlexInfo, GetSUInfo and GetUID
//
// Tiago Armani                 08/18/2005     LIBgg13321   New GetErrorMessageString exported function
//
// Norton Santos                12/06/2005     LIBhh48672   Removed include to "PST_FP_FlashDevApiCommon.h"
//
// Dindo Jao                    12/06/2005     LIBhh38363   Made the file common for both static and dynamic library.
//
// Norton Santos                12/06/2005     LIBgg24542   Changed GetErrorMessageString() method
//
// Samir Khial                  05/17/2006     LIBii41706   Add Serial Flash API support
//
// Samir Khial                  06/07/2006     LIBii47509   Added support for Kenai dll
//
// Norton Santos                06/12/2006     LIBii65606   Supporting consolidated progress bar in Flash DLL
//
// Mike Zarletti                06/15/2006     LIBgg31593   Added method for sending RQPI command
//
// Fathi Ben Amar               09/18/2006     LIBii99372   Reorganization of include file 
//
// Norton Santos			    03/29/2007     LIBll36670   Removed UTCResize() and UTCOpen() APIs and FlashDlg 
//                                                          related code
//
// Norton Santos                08/07/2007     LIBff89694   Old PST related code removal
//
// Xiang Ye                     11/08/2007     LIBnn61500   Flash: Fix crash while unloading dll in autotest  
//
//--------------------------------------------------------------------------------------------------
//                                       INCLUDE FILES
//--------------------------------------------------------------------------------------------------
#include "FlashAdapterApi.h"
#include "flashcommon.h"


//--------------------------------------------------------------------------------------------------
//                                         CONSTANTS
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//                                          MACROS
//--------------------------------------------------------------------------------------------------

#ifndef _LIB

	#ifdef FLASHDLLEXPORT
		#define UT_FLASH_API extern "C" __declspec(dllexport)
	#else
		#define UT_FLASH_API extern "C" __declspec(dllimport)
	#endif

#else

	#define UT_FLASH_API

#endif


#define FlashFailure(Status) Status != FLASH_STATUS_OK
#define FlashSuccess(Status) Status == FLASH_STATUS_OK




//--------------------------------------------------------------------------------------------------
//                                           ENUMS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                               STRUCTURES AND OTHER TYPEDEFS
//--------------------------------------------------------------------------------------------------
class CSerialParams  
{
public:
	CSerialParams()
    {
        m_eBaudRate = Baud_None;
        m_bODMPhone = TRUE;
        m_eActions = E_RESTART_SYNCHRONOUS;
        m_eActDuringFlash = E_NO_EXTRA_ACTION;
        m_nParamNum = 0;
    };

    CString m_strCOMPort;       // Will contain COM1, COM2 etc.
    BaudRateEnum m_eBaudRate;   // Baud rate used for flashing purposes
    BOOL m_bODMPhone;           // Is device ODM phone or not.
    eActAftFlash m_eActions;    // Actions after flash to be performed
    eExtActDuringFlash m_eActDuringFlash;   // Extra actions during flash (Reserved for future use)
    CFlashErr m_FlErrObj;       // Error Object
    UINT m_nParamNum;           // Number of other parameters in Arglist    
    va_list m_Arglist;            // (required) LPCTSTR which contains the super-file path and filename; 
                                // (required) WORD a unique ID for the serial device within the range [1000..1031]
};

class CFlashParams  
{
public:
	CFlashParams()
    {
        m_eActions = E_RESTART_SYNCHRONOUS;
        m_eActDuringFlash = E_NO_EXTRA_ACTION;
        m_nParamNum = 0;
		m_bUpgradeBootLoader = FALSE;
    };

	BOOL m_bUpgradeBootLoader;				// Is boot loader upgrade required
    eActAftFlash m_eActions;				// Actions after flash to be performed
    eExtActDuringFlash m_eActDuringFlash;   // Extra actions during flash (Reserved for future use)
    CFlashErr m_FlErrObj;					// Error Object
    UINT m_nParamNum;						// Number of other parameters in Arglist    
    va_list m_Arglist;						// (required) LPCTSTR which contains the super-file path and filename; 
};

class CFlashHdlr;



//--------------------------------------------------------------------------------------------------
//                               GLOBAL VARIABLE DECLARATIONS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                   FUNCTION PROTOTYPES
//--------------------------------------------------------------------------------------------------
UT_FLASH_API EFlashStatus UTCInit(HWND hwndClient, CFlashAdapterApi* pFlashAdapterApi);
UT_FLASH_API EFlashStatus UTCInitAndRegister(HWND hwndClient, CFlashAdapterApi* pFlashAdapterApi);
UT_FLASH_API EFlashStatus UTCClose();
UT_FLASH_API EFlashStatus FlashDeviceWOptions(UINT nParamNum,eActAftFlash eActions,
                                              CFlashErr &ErrObj, va_list Arglist,eExtActDuringFlash eActDuringFlash=E_NO_EXTRA_ACTION);
UT_FLASH_API EFlashStatus FlashSerialDevice(CSerialParams &SerialParam);
UT_FLASH_API EFlashStatus GetSuperfileInformation(CString strFile, CPSTStringArray& rarrValues, WORD& rwVerGroup0);
UT_FLASH_API EFlashStatus GetDeviceInformation(DEVICEID DeviceID, CPSTStringArray& rarrValues, WORD& rwVerGroup0);
UT_FLASH_API EFlashStatus GetBootFlashFlexInfo(DEVICEID DeviceID, BOOT_FLASH_FLEX_STRUC *pPhoneInfo, DWORD *pdwErrSts);
UT_FLASH_API EFlashStatus GetSUInfo(DEVICEID DeviceID, SU_INFO_STRUC *pSUInfo, DWORD *pdwErrSts, BOOL bBadRom = FALSE);
UT_FLASH_API EFlashStatus GetUID(DEVICEID DeviceID, BYTE *byPhoneUID, DWORD *pdwErrSts);
UT_FLASH_API EFlashStatus GetBootPDSInfo(DEVICEID DeviceID, BOOT_PDS_INFO_STRUC *pPhonePSDInfo, DWORD *pdwErrSts, bool bSwitchToFlashMode = false);

UT_FLASH_API BOOL GetErrorMessageString(CString& strMessage, EFlashStatus eFlashStatus, ...);
UT_FLASH_API BOOL GetErrorMessageStringV(CString& strMessage, EFlashStatus eFlashStatus, va_list argList);

UT_FLASH_API BOOL SetProgressBarType(EProgressBarType eProgressBarType);

UT_FLASH_API EFlashStatus FlashDeviceWXML(CFlashParams& FlashParams);

//Additions for LIBff29751 Changes
// Higher-level APIs
// Flash file info
UT_FLASH_API bool InitArraySize(unsigned short sMaxDeviceIndex,unsigned short sMaxIndexOfCG);

//Obtain the flash file information like size, date etc.
UT_FLASH_API bool GetFlashFileInfo(CFlashFileInfo &FileInfo);
//Obtain the flash super file related information like DL ver etc.
UT_FLASH_API bool GetSuperFileInfo(CFlashSuperFileInfo &FlashInfo);
UT_FLASH_API bool ResetStatsForDevice(IN DEVICEID sDeviceID); // Resets all statistics
UT_FLASH_API bool GetDeviceCGIntfDetails(CDeviceDetails &DevDet); // Obtain all device,Code Group and interface details...
UT_FLASH_API bool RemoveDevCGIntfElements(CDeviceDetails &DevDet); // Remove all device, CG and interface details from the arrays....
// Get the total time taken for flashing a particular device
UT_FLASH_API double GetTotalFlashTime(IN DEVICEID sDeviceID);
// For device and CG stats Info...
UT_FLASH_API DeviceIDList& GetDeviceListForCG();
// Get the total size of all code groups for a device.
UT_FLASH_API unsigned long GetCGTotalSize(IN DEVICEID sDeviceID);

// Lower-level APIs
// If the user wants to retrieve all the items at one time... 
// The user can loop through the entire collection to obtain all the needed details...
UT_FLASH_API IndexList &GetCGList(IN DEVICEID sDeviceID);
// Obtain the total flash size for a given device, code group number...
UT_FLASH_API double GetFlashSizeForCG(IN DEVICEID sDeviceID,IN short sCGNum,OUT unsigned short *sIntNum);
// Obtain the total flash rate for a given device, code group number...
UT_FLASH_API double GetFlashTimeForCG(IN DEVICEID sDeviceID,IN short sCGNum,OUT unsigned short *sIntNum);
// Obtain the total flash rate for a given device, code group number...
UT_FLASH_API double GetFlashRateForCG(IN DEVICEID sDeviceID,IN short sCGNum,OUT unsigned short *sIntNum);
// Flash process stats and erase information...
UT_FLASH_API DeviceIDList& GetDeviceListForProcStats();
// Obtain the time to ener flash mode ...
UT_FLASH_API double GetTimeToFlashMode(IN DEVICEID sDeviceID);
// Obtain the time to re-enumerate...
UT_FLASH_API double GetTimeToReEnum(IN DEVICEID sDeviceID);
// Obtain the time to erase...
UT_FLASH_API  double GetEraseTime(IN DEVICEID sDeviceID,IN short sIntNum);
// Obtain the restart status...
UT_FLASH_API bool GetRestartStatus(IN DEVICEID sDeviceID);
UT_FLASH_API bool DeleteAllImageFiles();


#endif // !defined(FLASH_API_INCLUDED)    