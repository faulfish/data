// RSDUSBNotify.cpp: implementation of the CRSDUSBNotify class.
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
#include "RSDUSBNotify.h"
#include "RSDInterface.h"
#include <dbt.h>
#include <Setupapi.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//PDEV_BROADCAST_DEVICEINTERFACE CRSDUSBNotify::m_pDeviceInterface = new DEV_BROADCAST_DEVICEINTERFACE();

CRSDUSBNotify::CRSDUSBNotify(CRSDInterface* pRSDInterface)
 : m_pRSDInterface(pRSDInterface)
 , m_hNotifyDev(NULL)
{
    ASSERT(m_pRSDInterface);

    // create a hidden window that waiting for the USB driver notification message.
    RECT rect = {10,10,100,100};	
    LPCTSTR pszHideWndClass = ::AfxRegisterWndClass(NULL); 
    BOOL result = CreateEx(0, pszHideWndClass, NULL, WS_OVERLAPPED, rect, NULL, 0);

	RegisterNotification();
    ASSERT(result);
}

CRSDUSBNotify::~CRSDUSBNotify()
{
    UnRegisterNotification();
}


BEGIN_MESSAGE_MAP(CRSDUSBNotify, CWnd)
    ON_WM_DEVICECHANGE()
END_MESSAGE_MAP()


//---------------------------------------------------------------------------/
// FUNCTION: RegisterNotification
//
// DESCRIPTION:
//   Start the detection for ODM USB devices
//
// PARAMETERS:
//   None
//
// RETURN: 
//   BOOL, TRUE means no error
//---------------------------------------------------------------------------/
BOOL CRSDUSBNotify::RegisterNotification()
{                                           
    INT i = 0;
    
    // Holds the GUID and device type
    DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
    
    // The list of GUIDs for which we want to register for notification
    const GUID *guidList[] = 
    {
        &GUID_CLASS_P2K,
            &GUID_CLASS_MSD,
            &GUID_CLASS_NDI,
            &GUID_CLASS_P2KMDM,
            &GUID_CLASS_HUB,
            &GUID_CLASS_ALADDIN_OBEX,
            &GUID_CLASS_ALADDIN_OMT,
            &GUID_CLASS_MCPC_AT,
            &GUID_CLASS_MCCI_FOMA_COML,
            &GUID_CLASS_MTP,
            &GUID_DEVINTERFACE_IMAGE,
            
            // List terminator -- should always be last -- add GUIDs above here
            0
    };
    
    // Loop on all GUIDs, registering for device notification
    while (guidList[i])
    {
        // Fill out the notification struct (GUID, device type, struct size)
        ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
        NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
        NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
        NotificationFilter.dbcc_classguid = *(guidList[i]);   
        
        // Attempt to register for notification
        if (INVALID_HANDLE_VALUE == (m_hNotifyDev = 
            RegisterDeviceNotification(m_hWnd, &NotificationFilter, 
            DEVICE_NOTIFY_WINDOW_HANDLE)))
        {
            // log failed -- 
            CString message;
            message.Format("RegisterDeviceNotification failed -- GetLastError "
                "returned %lx.\n", GetLastError());
        }
        
        // Move to the next GUID or finish when 0 is reached
        i++;
    }
    
    
    if (m_hNotifyDev == NULL)
    {
        DWORD dwError = ::GetLastError();
        return FALSE;
    }

    //scan for the device that already connected


    return TRUE;
}

//---------------------------------------------------------------------------/
// FUNCTION: UnRegisterNotification
//
// DESCRIPTION:
//   Stop the detection for ODM USB devices
//
// PARAMETERS:
//   None
//
// RETURN: 
//   BOOL, TRUE means no error
//---------------------------------------------------------------------------/
BOOL CRSDUSBNotify::UnRegisterNotification()
{
    if (m_hNotifyDev)
    {
        UnregisterDeviceNotification(m_hNotifyDev);
        m_hNotifyDev = NULL;
    }
    return TRUE;
}

//---------------------------------------------------------------------------/
// FUNCTION: OnDeviceChange
//
// DESCRIPTION:
//   the handler function for WM_DEVICECHANGE
//
// PARAMETERS:
//   nEventType:    the event type
//   dwData:        the address of a structure that contains event-specific data
//
// RETURN: 
//   BOOL
//---------------------------------------------------------------------------/
BOOL CRSDUSBNotify::OnDeviceChange(UINT nEventType, DWORD dwData)
{				
    if (dwData == NULL)
        return TRUE;	
    
    // header to know what our dwData really points to
    PDEV_BROADCAST_HDR pHeader = (PDEV_BROADCAST_HDR)dwData;
    PDEV_BROADCAST_PORT pDevicePort;
    PDEV_BROADCAST_DEVICEINTERFACE pDeviceInterface;
    
    
    // Notify PST
    switch (nEventType) 
    {
        case DBT_DEVICEARRIVAL:
        case DBT_DEVICEREMOVECOMPLETE:
        {
            // Get the SymbolicLink from the broadcast header.             
            CString sSymbolicLink;            
            if (pHeader->dbch_devicetype == DBT_DEVTYP_PORT)    
            {
                // Port device (serial or parallel) type  
                pDevicePort = (PDEV_BROADCAST_PORT)pHeader;
                sSymbolicLink = pDevicePort->dbcp_name;
            }
            else if (pHeader->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
            {
                // Devices type
                pDeviceInterface = (PDEV_BROADCAST_DEVICEINTERFACE) pHeader;
                sSymbolicLink = pDeviceInterface->dbcc_name;

                if(nEventType == DBT_DEVICEARRIVAL)
                {
                    memcpy(&m_DeviceInterface, pDeviceInterface, sizeof(DEV_BROADCAST_DEVICEINTERFACE));
                }
                
            }
            else
            {
                // Ignore others
                break;
            }

            // Get the "PathPrefix"
            CString strPathPrefix(GetPathPrefix(sSymbolicLink));

            if ("" == strPathPrefix)
            {
                // Composite device, can't be a Moto Flash device, just ignore it
                break;
            }

            // Check if the device is RSD interested
            UINT nDeviceId = -1;
            CSingleLock SingleLock(&m_csDeviceMap, TRUE);
            if ( m_mapDevice.Lookup(strPathPrefix, nDeviceId) )
            {
                MotDeviceNotificationStruct MessyMotDeviceNotificationStruct;
                MessyMotDeviceNotificationStruct.guid               = pDeviceInterface->dbcc_classguid;	//GUID_CLASS_ODM_DEVICE; 
                MessyMotDeviceNotificationStruct.eNotificationType  = (nEventType == DBT_DEVICEARRIVAL) ? DETECTION_ARRIVAL : DETECTION_REMOVAL;
                MessyMotDeviceNotificationStruct.eTransportType     = USB;
                MessyMotDeviceNotificationStruct.eDeviceId          = nDeviceId;
                MessyMotDeviceNotificationStruct.pStrSymLink        = (char*)(LPCSTR)sSymbolicLink;
                
                // Notify PST framework to process device
                EPSTStatus eStatus = m_pRSDInterface->PSTNotifyOnDeviceChange(&MessyMotDeviceNotificationStruct);
            }

            break;
        }

        default:
            // ignore other event types
            break;
    }

    return TRUE;
}


void CRSDUSBNotify::RegisterDevice(const CString& sPathPrefix, UINT nDeviceId)
{
    CSingleLock SingleLock(&m_csDeviceMap, TRUE);
    m_mapDevice.SetAt( sPathPrefix, nDeviceId );
}


void CRSDUSBNotify::UnRegisterDevice(const CString& sPathPrefix, UINT nDeviceId)
{
    CSingleLock SingleLock(&m_csDeviceMap, TRUE);
    m_mapDevice.RemoveKey( sPathPrefix );
}

////////////////////////////////////////////////////////////////////////
////
//// METHOD NAME: CRSDUSBNotify
//// DESCRIPTION: 
////    - scans the registry for any entries associated with the P2k device 
////    - the entries are created in the registry by the driver for all devices connected
////    - when an entry is detected, the method checks to see if it is valid by opening the handle to the interface associated with it
////    - if the handle is valid, the interface name is added to the list
//// PARAMETERS: 
////    - [IN] GUID  pDeviceClsName - Class GUID of the device to detect.
////    - [OUT] CStringArray &interfaceList - stores the entries found in the registry for the connected devices
//// RETURN: void 
////
////////////////////////////////////////////////////////////////////////
//void CRSDUSBNotify::GetUSBInterfaceList(const GUID*  pDeviceClsName, CStringArray &interfaceList)
//{
//	HDEVINFO                            hDeviceInfo = INVALID_HANDLE_VALUE;                    // handle to device information structure for the interface to our devices
//    PSP_INTERFACE_DEVICE_DETAIL_DATA    pDeviceDetail;                  // the detailed info
//    DWORD                               requiredSize;                   // size of buffer reguired to get the symbolic link name
//    SP_INTERFACE_DEVICE_DATA            interfaceData;                  // interface data for this device
//    LONG                                status = ERROR_SUCCESS ;        // return status from command
//    ULONG                               currentOffset = 0;              // the current offset into the user's buffer
//	
//    // get handle to devices that have our interface
//    hDeviceInfo = SetupDiGetClassDevs((LPGUID)pDeviceClsName, NULL, NULL,
//                                      DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);  
//	
//    if (hDeviceInfo == INVALID_HANDLE_VALUE) {
//        return;
//    }
//        // enumerate all interfaces
//    for ( int index = 0 ; status ==  ERROR_SUCCESS; index++) {
//            // reset our size
//        interfaceData.cbSize = sizeof(interfaceData);
//            // get the next interface
//        if (!SetupDiEnumDeviceInterfaces(hDeviceInfo, 
//                                         NULL, 
//                                         (LPGUID) pDeviceClsName,
//                                         index, 
//                                         &interfaceData)) {
//            status = GetLastError();
//            if (status == ERROR_NO_MORE_ITEMS) {
//                break;
//            } else {
//                break;
//            }
//        }   
//            // found our device, get the details data to obtain the symbolic
//            // link name
//		if (!SetupDiGetDeviceInterfaceDetail(hDeviceInfo, 
//                                             &interfaceData, 
//                                             NULL, 
//                                             0,
//                                             &requiredSize, 
//                                             NULL)) {
//            if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
//                break;
//            }
//        }
//        
//		// allocate a buffer to get detail
//        pDeviceDetail = (PSP_INTERFACE_DEVICE_DETAIL_DATA) LocalAlloc(LPTR, requiredSize);
//        if (pDeviceDetail == NULL) {
//            break;
//        }
//        pDeviceDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
//            // get the detail info
//        if (!SetupDiGetDeviceInterfaceDetail(hDeviceInfo, 
//                                             &interfaceData, 
//                                             pDeviceDetail, 
//                                             requiredSize,
//                                             NULL, 
//                                             NULL)) {
//            LocalFree(pDeviceDetail);
//            break;
//        }
//
//		interfaceList.Add(CString(pDeviceDetail->DevicePath));
//		// cleanup search
//        LocalFree(pDeviceDetail);
//
//	}  // end for loop
//		
//    SetupDiDestroyDeviceInfoList(hDeviceInfo);
//    SetLastError(ERROR_SUCCESS);
//}

//---------------------------------------------------------------------------/
// FUNCTION: GetPathPrefix
//
// DESCRIPTION:
//   Extract the "PathPrefix"(USB port topology) from the SymbolicLink
//
// PARAMETERS:
//   sSymbolicLink: the USB SymbolicLink
//
// RETURN: 
//   CString, the "PathPrefix"
//
// IMPORTANT NOTES:
//   Cause we only care about Moto Flash devices here, we ignored Composite devices
//   in this function, for Composite devices, it will just return ""
//---------------------------------------------------------------------------/
CString CRSDUSBNotify::GetPathPrefix(const CString& sSymbolicLink)
{
    CString sPathPrefix;
    CString localBuffer = sSymbolicLink;
    int intFirstIndex;
    int intSecondIndex;


    //************************************************************************************** //
    //WIN2K and WINXP only
    //----------------------------------------------------------------------------------------
    
    //Examples of interface names returned by the USB driver for a phone in Config. 8 connected to Port 1:
    
    //Interface 8
    // \\?\USB#Vid_22b8&Pid_0001&Mi_08#6& 2ce3fdc6 &2&8#{4e1e1e15-52d7-11d4-852d-00105aa6f80b}
    // 0          10       20        30        40        50        60        70        80  
    // 012345678901234567890123456789012345678901234567890123456789012345678901234567890123 
    //Interface 5
    // \\?\usb#vid_22b8&pid_0001&mi_05#6&2ce3fdc6&2&5#{4e1e1e15-52d7-11d4-852d-00105aa6f80b}
    //Interface 6
    // \\?\usb#vid_22b8&pid_0001&mi_06#6&2ce3fdc6&2&6#{4e1e1e15-52d7-11d4-852d-00105aa6f80b}
    // Non Composite device
    // \\##?#USB#Vid_22b8&Pid_2801#5&92832f8&0&1#{4e1e1e15-52d7-11d4-852d-00105aa6f80b}
    //Different lenghts of substring
    // \\?\USB#Vid_22b8&Pid_1001&MI_08#6&38c78cb&0&0008#{4e1e1e15-52d7-11d4-852d-00105aa6f80b}
    // \\?\USB#Vid_22b8&Pid_4901&Mi_08#6&28e14028&0&8#{4e1e1e15-52d7-11d4-852d-00105aa6f80b}
    // \\?\usb#root_hub#4&1cfa10b2&0#{f18a0e88-c30c-11d0-8815-00a0c906bed8}
    // we need to find a substring which is the same for all interfaces in the same config, in order to associate them with the same SU
    
    //Extract this substring meeting the above criteria: ie 6&2ce3fdc6
    //Find where to start, then where to end, then use those indexes to extract the substring
    localBuffer.MakeLower();  //make all lower case to ignore case differences in finds	
    
    if(localBuffer.Find("root_hub") != -1)
        intFirstIndex = localBuffer.Find("#",10)+1;
    else
        intFirstIndex = localBuffer.Find("#", 20) + 1; //Search after the PID in the string

    // ignore the composite devices
    if(localBuffer.Find("mi") != -1)
    {
        int iTempIndex= localBuffer.Find("&", intFirstIndex + 3); 
        /*Look for third ampersand in the string after the # sign*/
        intSecondIndex = localBuffer.Find("&", iTempIndex + 2); 
        //m_bComposite = true;
        m_bFound = FALSE;
        CString strKey = ENUMUSBKEY;
        CString strID = localBuffer.Mid(intFirstIndex , intSecondIndex - intFirstIndex); //extract the substring
        QueryKey(strKey,strID);
        return GetPathPrefix( m_strHubName);

    }

    
    
    //String must contain instance number also since parentIDs on some composite deivces are the same 
    intSecondIndex = localBuffer.Find("#", intFirstIndex + 3);

    //extract the substring
    sPathPrefix = localBuffer.Mid(intFirstIndex, intSecondIndex - intFirstIndex); 
    
    return sPathPrefix;
}

CString CRSDUSBNotify::QueryKey(CString& strKey,const CString& strDevice) 
{ 
    CSingleLock lockQueryRegistry(&m_csQueryRegistry, TRUE); // lock first to prevent threads conflicting
    TCHAR    achKey[MAX_PATH]= TEXT("");   // buffer for subkey name
    DWORD    cbName=0;                   // size of name string 
    TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // size of class string 
    DWORD    cSubKeys=0;               // number of subkeys 
    DWORD    cbMaxSubKey=0;              // longest subkey size 
    DWORD    cchMaxClass=0;              // longest class string 
    DWORD    cValues=0;              // number of values for key 
    DWORD    cchMaxValue=0;          // longest value name 
    DWORD    cbMaxValueData=0;       // longest value data 
    DWORD    cbSecurityDescriptor=0; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 
    
    DWORD i, retCode; 
    
    TCHAR  achValue[MAX_VALUE_NAME]= TEXT(""); 
    DWORD cchValue = MAX_VALUE_NAME; 
    HKEY hKey;
    
    long error = 0;
    if(m_bFound)
        return m_strHubName;
    error = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
        strKey,
        0, KEY_READ, &hKey );
    
    if (ERROR_SUCCESS != error)
    {
        //error = GetLastError();
        //LogPSTMessage(NO_DEVICE_ID, PST_LOG_LEVEL_ERROR, PST_LOG_TYPE_GENERAL, "CPSTSessionMgr::QueryKey - Open (%s) failed, Error = (%d)",strKey, error);
        return "";
    }
    // Get the class name and the value count. 
    retCode = RegQueryInfoKey(
        hKey,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class string 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime);       // last write time 
    
    // Enumerate the subkeys, until RegEnumKeyEx fails.
    
    if (cSubKeys)
    {
        CString strSubKey;
        for (i=0; i<cSubKeys; i++) 
        { 
            cbName = MAX_PATH;
            retCode = RegEnumKeyEx(hKey, i,
                achKey, 
                &cbName, 
                NULL, 
                NULL, 
                NULL, 
                &ftLastWriteTime); 
            if (retCode == ERROR_SUCCESS) 
            {
                if(!m_bFound)
                {
                    strSubKey= strKey+"\\"+achKey;
                    QueryKey(strSubKey,strDevice);
                }
                else
                {
                    break;
                }
            }
        }
    }
    
    
    
    // Enumerate the key values. 
    
    if (cValues) 
    {
        
        for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++) 
        { 
            cchValue = MAX_VALUE_NAME; 
            achValue[0] = '\0'; 
            retCode = RegEnumValue(hKey, i, 
                achValue, 
                &cchValue, 
                NULL, 
                NULL,
                NULL,
                NULL);
            
            if (retCode == ERROR_SUCCESS ) 
            { 
                unsigned char str[MAX_PATH];
                unsigned long ilength=MAX_PATH;
                int error = 0;
                if(strncmp(achValue,"ParentIdPrefix",14)==0)
                {
                    memset(str,0,MAX_PATH);
                    error = RegQueryValueEx(hKey,achValue,NULL,NULL,str,&ilength);
                    CString strPrefix = str;
                    if(0==strPrefix.CompareNoCase(strDevice))
                    {
                        m_bFound = TRUE;
                        break;
                    }
                }
                if(strncmp(achValue,"SymbolicName",12) == 0)
                {
                    memset(str,0,MAX_PATH);
                    error = RegQueryValueEx(hKey,achValue,NULL,NULL,str,&ilength);
                    str[1]='\\';
                    m_strHubName = str;
                    
                    
                }
            } 
        }
    }
    
    RegCloseKey( hKey );
    return m_strHubName;
}