// RSDInterface.cpp: implementation of the CRSDInterface class.
//
//////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------------------------------

                            Motorola Confidential Proprietary
                       Advanced Technology and Software Operations
                    (c) Copyright Motorola 2001-2008, All Rights Reserved
   

 Revision History:
                       Modification     Tracking
 Author                   Date          Number     Description of Changes
-------------------   ------------    ----------   ----------------------------------------
 Zhiyong Yang          10/13/2008     LIBqq78510   Initial Creation

--------------------------------------------------------------------------------------------------*/


#include "stdafx.h"
#include "RSDInterface.h"
#include "RSDUSBNotify.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#ifdef _DEBUG
    #define PST_DLL_NAME "pst.dll"
    #define FLASH_DLL_NAME "pst_fp_flash_d.dll"
#else
    #define PST_DLL_NAME "pst.dll"
    #define FLASH_DLL_NAME "pst_fp_flash.dll"
#endif




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRSDInterface::CRSDInterface()
{
    ResetMembers();
    
    //// create USBNotify object
    //m_pUSBNotify = new CRSDUSBNotify(this);
}


CRSDInterface::~CRSDInterface()
{
    ////UnInitialize();
    //
    //if (m_pUSBNotify) delete m_pUSBNotify;
}

//---------------------------------------------------------------------------/
// FUNCTION: Initialize
//
// DESCRIPTION:
//   Do all initialization work
//
// PARAMETERS:
//   None
//
// RETURN: 
//   BOOL, TRUE means no error
//---------------------------------------------------------------------------/
BOOL CRSDInterface::Initialize()
{
    // check pointers
    //if (!m_pUSBNotify) return FALSE;
    
    // load RSD dlls
    if ( !LoadDlls() ) return FALSE;
    
    // call RSD Initialization APIs
    if ( !InitRSD() ) return FALSE;
    
    // start USB detection
	return TRUE;
//    return m_pUSBNotify->RegisterNotification();

}

//---------------------------------------------------------------------------/
// FUNCTION: UnInitialize
//
// DESCRIPTION:
//   Do all clean-up work
//
// PARAMETERS:
//   None
//
// RETURN: 
//   BOOL, TRUE means no error
//---------------------------------------------------------------------------/
BOOL CRSDInterface::UnInitialize()
{
    UnInitRSD();

    if (m_hFlashDll)   FreeLibrary(m_hFlashDll);
    if (m_hPSTDll)     FreeLibrary(m_hPSTDll);
    
    ResetMembers();
    
    return TRUE;
}

//

//---------------------------------------------------------------------------/
// FUNCTION: ResetMembers
//
// DESCRIPTION:
//   Reset the class members to default values
//
// PARAMETERS:
//   None
//
// RETURN: 
//   None
//---------------------------------------------------------------------------/
void CRSDInterface::ResetMembers()
{
    // dll handles
    m_hPSTDll   = NULL;
    m_hFlashDll = NULL;
    
    // function pointers
    PSTInitializeAndRegisterEx  = NULL;
    PSTUninitialize             = NULL;
    PSTNotifyOnDeviceChange     = NULL;
    PSTEnableBlankFlashTIROM    = NULL;
	PSTRegisterForNotifications = NULL;
	PSTUnregisterForNotifications = NULL;

    
    UTCInit                     = NULL;
    UTCClose                    = NULL;
    FlashDeviceWOptions         = NULL;
}

//---------------------------------------------------------------------------/
// FUNCTION: LoadDlls
//
// DESCRIPTION:
//   Load RSD dlls
//
// PARAMETERS:
//   None
//
// RETURN: 
//   BOOL, TRUE means no error
//---------------------------------------------------------------------------/
BOOL CRSDInterface::LoadDlls()
{
    // load PST dll & get the function pointers
    m_hPSTDll = LoadLibrary(PST_DLL_NAME);
    if (m_hPSTDll)
    {
        PSTInitializeAndRegisterEx  = (FPPSTInitializeAndRegisterEx)  GetProcAddress(m_hPSTDll, "PSTInitializeAndRegisterEx");
		PSTUninitialize             = (FPPSTUninitialize)             GetProcAddress(m_hPSTDll, "PSTUninitialize");
        PSTProcessDevice            = (FPPSTProcessDevice)            GetProcAddress(m_hPSTDll, "PSTProcessDevice");
        PSTIgnoreDevice             = (FPPSTIgnoreDevice)             GetProcAddress(m_hPSTDll, "PSTIgnoreDevice");
        PSTNotifyOnDeviceChange     = (FPPSTNotifyOnDeviceChange)     GetProcAddress(m_hPSTDll, "PSTNotifyOnDeviceChange");
        PSTEnableBlankFlashTIROM    = (FPPSTEnableBlankFlashTIROM)    GetProcAddress(m_hPSTDll, "PSTEnableBlankFlashTIROM");
		//PSTRegisterForNotifications = (FPPSTRegisterForNotifications) GetProcAddress(m_hPSTDll, "PSTRegisterForNotifications");
		//PSTUnregisterForNotifications =	(FPPSTUnregisterForNotifications) GetProcAddress(m_hPSTDll, "PSTUnregisterForNotifications");

        // check function pointers
        if ( ! (PSTInitializeAndRegisterEx && PSTUninitialize && PSTProcessDevice
                && PSTIgnoreDevice && PSTNotifyOnDeviceChange && PSTEnableBlankFlashTIROM
				/*&& PSTRegisterForNotifications && PSTUnregisterForNotifications*/) )
            return FALSE;
    }
    else
    {
        return FALSE;
    }
    
    // load Flash dll & get the function pointers
    m_hFlashDll = LoadLibrary(FLASH_DLL_NAME);
    if (m_hFlashDll)
    {
        UTCInit             = (FPUTCInit)               GetProcAddress(m_hFlashDll, "UTCInit");
        UTCClose            = (FPUTCClose)              GetProcAddress(m_hFlashDll, "UTCClose");
        FlashDeviceWOptions = (FPFlashDeviceWOptions)   GetProcAddress(m_hFlashDll, "FlashDeviceWOptions");

        // check function pointers
        if ( ! (UTCInit && UTCClose && FlashDeviceWOptions) )
            return FALSE;
    }
    else
    {
        return FALSE;
    }
    
    return TRUE;
}

//---------------------------------------------------------------------------/
// FUNCTION: InitRSD
//
// DESCRIPTION:
//   After RSD dlls are loaded, do initialization work
//
// PARAMETERS:
//   None
//
// RETURN: 
//   BOOL, TRUE means no error
//---------------------------------------------------------------------------/
BOOL CRSDInterface::InitRSD()
{
    // init PST dll in a non-detect mode, in this mode, 
    // PST will not do any [device detection / device id allocation] work.
    PSTInitializeStruct MystructPSTInitializeInput;
    MystructPSTInitializeInput.eWorkMode        = MODE_USB;
    MystructPSTInitializeInput.eSwitch          = MOT_AUTOSWITCH_CONFIG8;
    MystructPSTInitializeInput.eDetectionOption = DETECT_NONE;          // no detection
    MystructPSTInitializeInput.eWorkMode        = MODE_DETECTION_OFF;   // no detection
    MystructPSTInitializeInput.eDeviceIdOption  = MODE_DEVICEID_OPERATION_PASSIVE;  // no device id allocation
    
    EPSTStatus eStatus = PSTInitializeAndRegisterEx(MystructPSTInitializeInput); 
    if (eStatus != PST_OK)
    {
        return FALSE;
    }
    
    
    // init Flash dll 
    EFlashStatus FlashStatus = UTCInit(HWND_BROADCAST, NULL);
    if (FlashStatus != FLASH_STATUS_OK)
    {
        PSTUninitialize();
        return FALSE;
    }
    
    return TRUE;
}

//---------------------------------------------------------------------------/
// FUNCTION: UnInitRSD
//
// DESCRIPTION:
//   UnInitialize RSD dlls
//
// PARAMETERS:
//   None
//
// RETURN: 
//   BOOL, TRUE means no error
//---------------------------------------------------------------------------/
BOOL CRSDInterface::UnInitRSD()
{
	
	EFlashStatus FlashStatus = FLASH_STATUS_FAILED;
	EPSTStatus eStatus = PST_NOT_SUPPORTED;
	if(m_hFlashDll)
		FlashStatus = UTCClose();
	if(m_hPSTDll)
		eStatus = PSTUninitialize(); 

    return ( (FlashStatus == FLASH_STATUS_OK) && (eStatus == PST_OK) );
}

//---------------------------------------------------------------------------/
//// FUNCTION: RegisterDevice
////
//// DESCRIPTION:
////   Register a device which RSD interested
////
//// PARAMETERS:
////   sSymbolicLink: the USB SymbolicLink
////   nDeviceId:     the DeviceId allocated by TPA
////
//// RETURN: 
////   None
////---------------------------------------------------------------------------/
//void CRSDInterface::RegisterDevice(const CString& sSymbolicLink, UINT nDeviceId)
//{
//    ASSERT(m_pUSBNotify);
//    if (m_pUSBNotify) m_pUSBNotify->RegisterDevice(sSymbolicLink, nDeviceId);
//}
//
////---------------------------------------------------------------------------/
//// FUNCTION: UnRegisterDevice
////
//// DESCRIPTION:
////   UnRegisterDevice a device which RSD not interested
////
//// PARAMETERS:
////   sSymbolicLink: the USB SymbolicLink
////   nDeviceId:     the DeviceId allocated by TPA
////
//// RETURN: 
////   None
////---------------------------------------------------------------------------/
//void CRSDInterface::UnRegisterDevice(const CString& sSymbolicLink, UINT nDeviceId)
//{
//    ASSERT(m_pUSBNotify);
//    if (m_pUSBNotify) m_pUSBNotify->UnRegisterDevice(sSymbolicLink, nDeviceId);
//}


////---------------------------------------------------------------------------/
//// FUNCTION: GetPathPrefix
////
//// DESCRIPTION:
////   Extract the "PathPrefix"(USB port topology) from the SymbolicLink
////
//// PARAMETERS:
////   sSymbolicLink: the USB SymbolicLink
////
//// RETURN: 
////   CString, the "PathPrefix"
////
//// IMPORTANT NOTES:
////   Cause we only care about Moto Flash devices here, we ignored Composite devices
////   in this function, for Composite devices, it will just return ""
////---------------------------------------------------------------------------/
//CString CRSDInterface::GetPathPrefix(const CString& sSymbolicLink)
//{
//    CString sPathPrefix;
//    CString localBuffer = sSymbolicLink;
//    int intFirstIndex;
//    int intSecondIndex;
//
//
//    //************************************************************************************** //
//    //WIN2K and WINXP only
//    //----------------------------------------------------------------------------------------
//    
//    //Examples of interface names returned by the USB driver for a phone in Config. 8 connected to Port 1:
//    
//    //Interface 8
//    // \\?\USB#Vid_22b8&Pid_0001&Mi_08#6& 2ce3fdc6 &2&8#{4e1e1e15-52d7-11d4-852d-00105aa6f80b}
//    // 0          10       20        30        40        50        60        70        80  
//    // 012345678901234567890123456789012345678901234567890123456789012345678901234567890123 
//    //Interface 5
//    // \\?\usb#vid_22b8&pid_0001&mi_05#6&2ce3fdc6&2&5#{4e1e1e15-52d7-11d4-852d-00105aa6f80b}
//    //Interface 6
//    // \\?\usb#vid_22b8&pid_0001&mi_06#6&2ce3fdc6&2&6#{4e1e1e15-52d7-11d4-852d-00105aa6f80b}
//    // Non Composite device
//    // \\##?#USB#Vid_22b8&Pid_2801#5&92832f8&0&1#{4e1e1e15-52d7-11d4-852d-00105aa6f80b}
//    //Different lenghts of substring
//    // \\?\USB#Vid_22b8&Pid_1001&MI_08#6&38c78cb&0&0008#{4e1e1e15-52d7-11d4-852d-00105aa6f80b}
//    // \\?\USB#Vid_22b8&Pid_4901&Mi_08#6&28e14028&0&8#{4e1e1e15-52d7-11d4-852d-00105aa6f80b}
//    // \\?\usb#root_hub#4&1cfa10b2&0#{f18a0e88-c30c-11d0-8815-00a0c906bed8}
//    // we need to find a substring which is the same for all interfaces in the same config, in order to associate them with the same SU
//    
//    //Extract this substring meeting the above criteria: ie 6&2ce3fdc6
//    //Find where to start, then where to end, then use those indexes to extract the substring
//    localBuffer.MakeLower();  //make all lower case to ignore case differences in finds	
//    
//    if(localBuffer.Find("root_hub") != -1)
//        intFirstIndex = localBuffer.Find("#",10)+1;
//    else
//        intFirstIndex = localBuffer.Find("#", 20) + 1; //Search after the PID in the string
//
//    // ignore the composite devices
//    if(localBuffer.Find("mi") != -1)
//    {
//        int iTempIndex= localBuffer.Find("&", intFirstIndex + 3); 
//        /*Look for third ampersand in the string after the # sign*/
//        intSecondIndex = localBuffer.Find("&", iTempIndex + 2); 
//        //m_bComposite = true;
//        m_bFound = FALSE;
//        CString strKey = ENUMUSBKEY;
//        CString strID = localBuffer.Mid(intFirstIndex , intSecondIndex - intFirstIndex); //extract the substring
//        QueryKey(strKey,strID);
//        return GetPathPrefix( m_strHubName);
//
//    }
//
//    
//    
//    //String must contain instance number also since parentIDs on some composite deivces are the same 
//    intSecondIndex = localBuffer.Find("#", intFirstIndex + 3);
//
//    //extract the substring
//    sPathPrefix = localBuffer.Mid(intFirstIndex, intSecondIndex - intFirstIndex); 
//    
//    return sPathPrefix;
//}
//
//CString CRSDInterface::QueryKey(CString& strKey,const CString& strDevice) 
//{ 
//    CSingleLock lockQueryRegistry(&g_csQueryRegistry, TRUE); // lock first to prevent threads conflicting
//    TCHAR    achKey[MAX_PATH]= TEXT("");   // buffer for subkey name
//    DWORD    cbName=0;                   // size of name string 
//    TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
//    DWORD    cchClassName = MAX_PATH;  // size of class string 
//    DWORD    cSubKeys=0;               // number of subkeys 
//    DWORD    cbMaxSubKey=0;              // longest subkey size 
//    DWORD    cchMaxClass=0;              // longest class string 
//    DWORD    cValues=0;              // number of values for key 
//    DWORD    cchMaxValue=0;          // longest value name 
//    DWORD    cbMaxValueData=0;       // longest value data 
//    DWORD    cbSecurityDescriptor=0; // size of security descriptor 
//    FILETIME ftLastWriteTime;      // last write time 
//    
//    DWORD i, retCode; 
//    
//    TCHAR  achValue[MAX_VALUE_NAME]= TEXT(""); 
//    DWORD cchValue = MAX_VALUE_NAME; 
//    HKEY hKey;
//    
//    long error = 0;
//    if(m_bFound)
//        return m_strHubName;
//    error = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
//        strKey,
//        0, KEY_READ, &hKey );
//    
//    if (ERROR_SUCCESS != error)
//    {
//        //error = GetLastError();
//        //LogPSTMessage(NO_DEVICE_ID, PST_LOG_LEVEL_ERROR, PST_LOG_TYPE_GENERAL, "CPSTSessionMgr::QueryKey - Open (%s) failed, Error = (%d)",strKey, error);
//        return "";
//    }
//    // Get the class name and the value count. 
//    retCode = RegQueryInfoKey(
//        hKey,                    // key handle 
//        achClass,                // buffer for class name 
//        &cchClassName,           // size of class string 
//        NULL,                    // reserved 
//        &cSubKeys,               // number of subkeys 
//        &cbMaxSubKey,            // longest subkey size 
//        &cchMaxClass,            // longest class string 
//        &cValues,                // number of values for this key 
//        &cchMaxValue,            // longest value name 
//        &cbMaxValueData,         // longest value data 
//        &cbSecurityDescriptor,   // security descriptor 
//        &ftLastWriteTime);       // last write time 
//    
//    // Enumerate the subkeys, until RegEnumKeyEx fails.
//    
//    if (cSubKeys)
//    {
//        CString strSubKey;
//        for (i=0; i<cSubKeys; i++) 
//        { 
//            cbName = MAX_PATH;
//            retCode = RegEnumKeyEx(hKey, i,
//                achKey, 
//                &cbName, 
//                NULL, 
//                NULL, 
//                NULL, 
//                &ftLastWriteTime); 
//            if (retCode == ERROR_SUCCESS) 
//            {
//                if(!m_bFound)
//                {
//                    strSubKey= strKey+"\\"+achKey;
//                    QueryKey(strSubKey,strDevice);
//                }
//                else
//                {
//                    break;
//                }
//            }
//        }
//    }
//    
//    
//    
//    // Enumerate the key values. 
//    
//    if (cValues) 
//    {
//        
//        for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++) 
//        { 
//            cchValue = MAX_VALUE_NAME; 
//            achValue[0] = '\0'; 
//            retCode = RegEnumValue(hKey, i, 
//                achValue, 
//                &cchValue, 
//                NULL, 
//                NULL,
//                NULL,
//                NULL);
//            
//            if (retCode == ERROR_SUCCESS ) 
//            { 
//                unsigned char str[MAX_PATH];
//                unsigned long ilength=MAX_PATH;
//                int error = 0;
//                if(strncmp(achValue,"ParentIdPrefix",14)==0)
//                {
//                    memset(str,0,MAX_PATH);
//                    error = RegQueryValueEx(hKey,achValue,NULL,NULL,str,&ilength);
//                    CString strPrefix = str;
//                    if(0==strPrefix.CompareNoCase(strDevice))
//                    {
//                        m_bFound = TRUE;
//                        break;
//                    }
//                }
//                if(strncmp(achValue,"SymbolicName",12) == 0)
//                {
//                    memset(str,0,MAX_PATH);
//                    error = RegQueryValueEx(hKey,achValue,NULL,NULL,str,&ilength);
//                    str[1]='\\';
//                    m_strHubName = str;
//                    
//                    
//                }
//            } 
//        }
//    }
//    
//    RegCloseKey( hKey );
//    return m_strHubName;
//}

