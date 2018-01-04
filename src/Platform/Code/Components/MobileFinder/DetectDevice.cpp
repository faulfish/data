#include "stdafx.h"
#include "DetectDevice.h"
#include "..\..\CommonUtil\LogPolicy.h"
#include "..\..\CommonUtil\unicodeutility.h"

using namespace QSYNC_LIB;

const UINT WM_MODEM_FOUND = 9001 ;
const UINT WM_DIAG_FOUND = 9002 ;

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

const bool CDectectDevice::IsDiag(const TSTRING& strCOMPort)
{
    TRACE_FILE_LINE_INFO();

    CSingleLock singleLock(&m_CritSection,TRUE);

    bool bRes = false;

    //若不等一下，port剛找到時也許還問不到其他資訊，例如modal name.
    ::Sleep(300);

    CSerialPort comport;

    if(m_bEnable && comport.OpenPort(strCOMPort,CSerialPort::CSPDIAGCommandMode))
    {
        CBufferPtr ptBuffer = new CBuffer;

        /*
        000158: Bulk or Interrupt Transfer (UP), 17.06.2008 10:40:04.4389742 +11.9528955
        Pipe Handle: 0x89d458cc (Endpoint Address: 0x4)
        Send 0x24 bytes to the device:
        7B 51 43 4F 4D 20 66 61 73 74 20 64 6F 77 6E 6C   {QCOM fast downl
        6F 61 64 20 70 72 6F 74 6F 63 6F 6C 20 68 6F 73   oad protocol hos
        74 08 32 7E                                       t.2~
        000159: Bulk or Interrupt Transfer (UP), 17.06.2008 10:40:23.1417401 +18.7027659
        Pipe Handle: 0x89d458cc (Endpoint Address: 0x4)
        Send 0x24 bytes to the device:
        7B 51 43 4F 4D 20 66 61 73 74 20 64 6F 77 6E 6C   {QCOM fast downl
        6F 61 64 20 70 72 6F 74 6F 63 6F 6C 20 68 6F 73   oad protocol hos
        74 08 32 7E                                       t.2~
        000160: Bulk or Interrupt Transfer (UP), 17.06.2008 10:40:23.1417401 +0.0
        Pipe Handle: 0x89d458ac (Endpoint Address: 0x84)
        Get 0x24 bytes from the device:
        7B 51 43 4F 4D 20 66 61 73 74 20 64 6F 77 6E 6C   {QCOM fast downl
        6F 61 64 20 70 72 6F 74 6F 63 6F 6C 20 68 6F 73   oad protocol hos
        74 08 32 7E                                       t.2~
        */

        char szNOP[] = {0x7E,0x06,0x4E,0x95,0x7E};
        unsigned char emergencyBuf[] = {0x7e, 0x02, 0x6A, 0xD3, 0x7e};
        unsigned char diag1Buf[] = {0x15, 0x06, 0x58, 0x81, 0x7e};
        unsigned char diag2Buf[] = {0x13, 0x06, 0x88, 0xd5, 0x7e};
        CBufferPtr ptBufferSend = new CBuffer(5,szNOP);

        if(comport.WriteAndRead((const BYTE *)ptBufferSend->GetData(),ptBufferSend->GetLength(),ptBuffer,1000))		
        {
            bRes = ::memcmp(ptBuffer->GetData(),ptBufferSend->GetData(),26) == 0;
            if(!bRes)
                bRes = ::memcmp(ptBuffer->GetData(), emergencyBuf, 5) == 0;
            if(!bRes)
                bRes = ::memcmp(ptBuffer->GetData(), diag1Buf, 5) == 0;
            if(!bRes)
                bRes = ::memcmp(ptBuffer->GetData(), diag2Buf, 5) == 0;
            //ASSERT(bRes);
        }

        comport.ClosePort();
    }

    return bRes;
}

const bool CDectectDevice::IsMODEM(const TSTRING& strCOMPort,TSTRING& strModule)
{
    TRACE_FILE_LINE_INFO();

    CSingleLock singleLock(&m_CritSection,TRUE);

    //若不等一下，port剛找到時也許還問不到其他資訊，例如modal name.
    ::Sleep(300);

    bool bRes = false;

    CSerialPort comport;

    if(m_bEnable && comport.OpenPort(strCOMPort,CSerialPort::CSPATCommandMode))
    {
        CBufferPtr ptBuffer = new CBuffer;

        /*
        000075: Bulk or Interrupt Transfer (UP), 20.02.2008 14:51:07.7969963 +1.7811474
        Pipe Handle: 0x8a23a2d4 (Endpoint Address: 0x2)
        Send 0x9 bytes to the device:
        41 54 2B 43 50 49 4E 3F 0D                        AT+CPIN?.
        000076: Bulk or Interrupt Transfer (UP), 20.02.2008 14:51:07.7969963 +0.0
        Pipe Handle: 0x8a23a2b4 (Endpoint Address: 0x82)
        Get 0x9 bytes from the device:
        41 54 2B 43 50 49 4E 3F 0D                        AT+CPIN?.
        000077: Bulk or Interrupt Transfer (UP), 20.02.2008 14:51:07.7969963 +0.0
        Pipe Handle: 0x8a23a2b4 (Endpoint Address: 0x82)
        Get 0x16 bytes from the device:
        0D 0A 2B 43 50 49 4E 3A 20 52 45 41 44 59 0D 0A   ..+CPIN: READY..
        0D 0A 4F 4B 0D 0A                                 ..OK..
        000078: Bulk or Interrupt Transfer (UP), 20.02.2008 
        */
        /*
        000436: Bulk or Interrupt Transfer (UP), 20.02.2008 14:56:33.6688500 +0.0
        Pipe Handle: 0x8946efdc (Endpoint Address: 0x2)
        Send 0x9 bytes to the device:
        41 54 2B 43 50 49 4E 3F 0D                        AT+CPIN?.
        000437: Bulk or Interrupt Transfer (UP), 20.02.2008 14:56:33.6844741 +0.0156241
        Pipe Handle: 0x8946efbc (Endpoint Address: 0x82)
        Get 0x9 bytes from the device:
        41 54 2B 43 50 49 4E 3F 0D                        AT+CPIN?.
        000438: Bulk or Interrupt Transfer (UP), 20.02.2008 14:56:33.6844741 +0.0
        Pipe Handle: 0x8946efbc (Endpoint Address: 0x82)
        Get 0x1b bytes from the device:
        0D 0A 2B 43 4D 45 20 45 52 52 4F 52 3A 20 53 49   ..+CME ERROR: SI
        4D 20 66 61 69 6C 75 72 65 0D 0A                  M failure..
        */
        if (comport.WriteAndRead("AT+CPIN?\r",ptBuffer,1000))
            bRes = CSerialPort::IsCompleteATPackage(ptBuffer);

        if (bRes && comport.WriteAndRead("AT+CGMM\r",ptBuffer,1000))
        {
            if(CSerialPort::IsCompleteATPackage(ptBuffer))
            {								
                CBufferPtr ptDataBuffer = new CBuffer;
                CSerialPort::DecodeATPackage(ptBuffer,ptDataBuffer);
                string str;
                for(ULONGLONG i=0;i<ptDataBuffer->GetLength();i++)
                {
                    const char *pData = ptDataBuffer->GetData();
                    str += pData[i];
                }
#ifdef _UNICODE
                wchar_t* wszBuffer = new wchar_t[str.size()+1];
                memset(wszBuffer,0,sizeof(wchar_t)*(str.size()+1));
                AnsiToUnicodeBuffer(str.c_str(),wszBuffer,(str.size()+1)*sizeof(wchar_t));
                strModule = wszBuffer;
                delete [] wszBuffer;
#else
                strModule = str;
#endif
                bRes = true;
            }

        }
        comport.ClosePort();
    }

    return bRes;
}

BOOL CDectectDevice::OnDeviceChange(UINT nEventType,DWORD dwData)
{
    PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)dwData;
    TCHAR szMsg[80] = {0};

    CString strMsg;
    switch(nEventType)
    {
    case DBT_DEVICEARRIVAL:
    case DBT_DEVICEREMOVECOMPLETE:
        switch(lpdb->dbch_devicetype)
        {
#if(WINVER >= 0x040A)
    case DBT_DEVTYP_DEVICEINTERFACE:
        GetDevice((PDEV_BROADCAST_DEVICEINTERFACE)lpdb,nEventType);
        break;		
    case DBT_DEVTYP_HANDLE:
        strMsg = _T("DBT_DEVTYP_OEM");
        break;
#endif
    case DBT_DEVTYP_OEM:
        strMsg = _T("DBT_DEVTYP_HANDLE");
        break;
    case DBT_DEVTYP_PORT:
        {
            /*PDEV_BROADCAST_PORT lpdbv = (PDEV_BROADCAST_PORT)lpdb;
            if (lpdbv)
            {
            DeviceDataPt ptdevData = new DeviceData(DBT_DEVTYP_PORT);
            ptdevData->m_strPort = lpdbv->dbcp_name;
            if(nEventType == DBT_DEVICEREMOVECOMPLETE)
            {
            RemoveDevice(ptdevData);
            }
            else if(nEventType == DBT_DEVICEARRIVAL)
            {
            ptdevData->m_strPort = lpdbv->dbcp_name;
            if(IsMODEM(lpdbv->dbcp_name,ptdevData->m_strExtName))
            AddDevice(ptdevData);
            }
            }*/
        }
        break;
    case DBT_DEVTYP_VOLUME:
        {
            PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
            if (lpdbv)
            {
                //DeviceDataPt ptdevData = new DeviceData(DBT_DEVTYP_VOLUME);
                //wsprintf (szMsg, _T("%c:"),FirstDriveFromMask(lpdbv ->dbcv_unitmask));
                //ptdevData->m_strDriver = szMsg;
                //if(nEventType == DBT_DEVICEARRIVAL)
                //	AddDevice(ptdevData);
                //if(nEventType == DBT_DEVICEREMOVECOMPLETE)
                //	RemoveDevice(ptdevData);
            }
        }
        break;
    default:
        strMsg = _T("DBT_DEVICEARRIVAL or DBT_DEVICEREMOVECOMPLETE");
        break;
        };
        break;
    case DBT_DEVICEQUERYREMOVE:   
        strMsg = _T("Permission to remove a device is requested. Any application can deny this request and cancel the removal. ");
        break;
    case DBT_DEVICEQUERYREMOVEFAILED:   
        strMsg = _T("Request to remove a device has been canceled. ");
        break;
    case DBT_DEVICEREMOVEPENDING:   
        strMsg = _T("Device is about to be removed. Cannot be denied. ");
        break;
    case DBT_DEVICETYPESPECIFIC:   
        strMsg = _T("Device-specific event. ");
        break;
    case DBT_CONFIGCHANGED:   
        strMsg = _T("Current configuration has changed. ");
        break;
    case DBT_DEVNODES_CHANGED:   
        strMsg = _T("Device node has changed. ");
        break;
    default:
        strMsg = _T("Unknow");
        break;
    };
    TRACE(strMsg + CString(_T("\n")));
    return TRUE;
}

DWORD CDectectDevice::LocatePort(HANDLE hUSBHUB, HDEVINFO hDeviceInfoSet, PSP_DEVINFO_DATA pspDeviceInfoData)
{
    TRACE_FILE_LINE_INFO();

    TCHAR TraceBuffer[256] = {0};
    DWORD nPort = 0, dwType, dwRequiredSize = 0, nBytesReturned = 0;

    TSTRING sDriverClassGUID;

    // Getting SPDRP_SERVICE
    SetupDiGetDeviceRegistryProperty(hDeviceInfoSet, pspDeviceInfoData, SPDRP_DRIVER , &dwType, NULL, 0, &dwRequiredSize);
    if(dwRequiredSize > 0)
    {
        dwRequiredSize += sizeof(TCHAR);
        PBYTE pbyDriver = new BYTE[dwRequiredSize];
        if(SetupDiGetDeviceRegistryProperty(hDeviceInfoSet, pspDeviceInfoData, SPDRP_DRIVER , &dwType, pbyDriver, dwRequiredSize, &dwRequiredSize))
        {
            ::_tcsupr_s((LPTSTR) pbyDriver,dwRequiredSize/sizeof(TCHAR));
            sDriverClassGUID.assign((LPTSTR) pbyDriver);
        }
        delete [] pbyDriver;
    }

    //Get Node Information from USB HUB to know how many ports are installed
    USB_NODE_INFORMATION NodeInformation;
    ZeroMemory(&NodeInformation, sizeof(NodeInformation));
    if(!DeviceIoControl(hUSBHUB, IOCTL_USB_GET_NODE_INFORMATION, &NodeInformation, sizeof(NodeInformation), &NodeInformation, sizeof(NodeInformation), &nBytesReturned, NULL))
    {
        TRACE_FILE_LINE_INFO();
        return 0;
    }

    for(DWORD PortIndex = 1; PortIndex <= NodeInformation.u.HubInformation.HubDescriptor.bNumberOfPorts; PortIndex++)
    {
        //////////////////////////////////////////////////////////////////////////
        // Get Driver Key Name to compare with sDriverClassGUID
        //////////////////////////////////////////////////////////////////////////
        USB_NODE_CONNECTION_DRIVERKEY_NAME driverKeyName;
        ZeroMemory(&driverKeyName, sizeof(driverKeyName));
        driverKeyName.ConnectionIndex = PortIndex;

        if(!DeviceIoControl(hUSBHUB, 
            IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME, 
            &driverKeyName,	
            sizeof(driverKeyName), 
            &driverKeyName, 
            sizeof(driverKeyName), 
            &nBytesReturned, 
            NULL))
        {
            TRACE_FILE_LINE_INFO();
            continue;
        }
        else
        {
            nBytesReturned = driverKeyName.ActualLength;

            //check if necessary memory is valid
            if(nBytesReturned < sizeof(USB_NODE_CONNECTION_DRIVERKEY_NAME))
                continue;

            PUSB_NODE_CONNECTION_DRIVERKEY_NAME driverKeyNameW = (PUSB_NODE_CONNECTION_DRIVERKEY_NAME) new BYTE[nBytesReturned];
            driverKeyNameW->ConnectionIndex = PortIndex;
            if(!DeviceIoControl(hUSBHUB, 
                IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME, 
                driverKeyNameW,	
                nBytesReturned, 
                driverKeyNameW, 
                nBytesReturned, 
                &nBytesReturned, 
                NULL))
            {
                TRACE_FILE_LINE_INFO();
                delete [] driverKeyNameW;
                continue;
            }
            else
            {
                USES_CONVERSION;
                //Compare the ClassGUID to check if the devices are the same.
                if(sDriverClassGUID == W2CT(driverKeyNameW->DriverKeyName))
                {
                    nPort = PortIndex;
                    delete [] driverKeyNameW;
                    break;
                }
            }
            delete [] driverKeyNameW;
        }
    }
    return(nPort);
}

bool CDectectDevice::IsVaildVIDPID(USHORT idVendor,USHORT idProduct)
{
    bool bRes = false;

    if( idVendor== 0x1D45 
        || idVendor == 0x04a5 
        || idVendor == 4597 
        || idVendor == 0x22B8 //Motorola
        || // Siemens device Vid_11f5
        ( idVendor == 0x05C6 && idProduct == 0x6000 )  ||
        ( idVendor == 0x05C6 && idProduct == 0x9004 )  ||
        ( idVendor == 0x05C6 && idProduct == 0x9002 )  ||
        ( idVendor == 0x22B8 && idProduct == 0x2D50 )  ||
        ( idVendor == 0x22B8 && idProduct == 0x2D53 )
        ) //Qualcomm device Vid_05c6&Pid_6000
    {
        bRes =  true;
    }

    return bRes;
}

void CDectectDevice::AddPortNotify(const TSTRING& strPort,const TSTRING& strDeviceClass,const TSTRING& strEvent)
{
    if(!strPort.empty())
    {
        DeviceDataPt ptdevData = new DeviceData(DBT_DEVTYP_PORT,strPort);
        ptdevData->m_strDeviceClass = strDeviceClass;

        //Sepecially to get module name for QSyncer
        if(strEvent == DEVICE_CONNECT)
            IsMODEM(strPort,ptdevData->m_strExtName);

        AddDevice(ptdevData,strEvent);
    }
}

void CDectectDevice::ParserDBCCName(TCHAR dbcc_name[1],USHORT& idVendor,USHORT& idProduct,TSTRING& strPara)
{
    // \\?\usb#vid_22b8&pid_2d01#6&.......
    TCHAR *szToken;
    TCHAR *szNextToken;
    ::_tcstok_s(dbcc_name,_T("_&"),&szNextToken);
    szToken = ::_tcstok_s(szNextToken,_T("_&"),&szNextToken);
    idVendor = (USHORT)HEX2Ineger(szToken);
    ::_tcstok_s(szNextToken,_T("_&"),&szNextToken);
    szToken = ::_tcstok_s(szNextToken,_T("_&#"),&szNextToken);
    idProduct = (USHORT)HEX2Ineger(szToken);
    szToken = ::_tcstok_s(szNextToken,_T("#"),&szNextToken);
    strPara = szToken;
}

void CDectectDevice::AddDevice(PDEV_BROADCAST_DEVICEINTERFACE lpbv)
{
    TRACE_FILE_LINE_INFO();

    if(lpbv)
    {
        HDEVINFO  hDeviceInfoSet = ::SetupDiCreateDeviceInfoList(NULL,NULL);
        if(hDeviceInfoSet)
        {
            SP_DEVICE_INTERFACE_DATA spDeviceInterfaceData;
            ZeroMemory(&spDeviceInterfaceData, sizeof(SP_DEVICE_INTERFACE_DATA));
            spDeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

            if(::SetupDiOpenDeviceInterface(hDeviceInfoSet, lpbv->dbcc_name, 0, &spDeviceInterfaceData))
            {
                SP_DEVINFO_DATA spDeviceInfoData;
                ZeroMemory(&spDeviceInfoData, sizeof(SP_DEVINFO_DATA));
                spDeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

                DWORD dwDeviceInterfaceDetailDataSize = (offsetof( SP_DEVICE_INTERFACE_DETAIL_DATA, DevicePath) + sizeof(TCHAR));
                DWORD dwRequiredSize = 0;

                BOOL bRes = ::SetupDiGetDeviceInterfaceDetail(hDeviceInfoSet, &spDeviceInterfaceData, NULL, 0, &dwRequiredSize, &spDeviceInfoData);

                PSP_DEVICE_INTERFACE_DETAIL_DATA pDeviceInterfaceDetail = (PSP_INTERFACE_DEVICE_DETAIL_DATA) new BYTE[dwRequiredSize];
                ZeroMemory(pDeviceInterfaceDetail, dwRequiredSize);
                pDeviceInterfaceDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
                bRes = ::SetupDiGetDeviceInterfaceDetail(hDeviceInfoSet, &spDeviceInterfaceData, pDeviceInterfaceDetail, dwRequiredSize, &dwRequiredSize, &spDeviceInfoData);

                USHORT idVendor;
                USHORT idProduct;
                TSTRING strPara;
                ParserDBCCName(pDeviceInterfaceDetail->DevicePath,idVendor,idProduct,strPara);

                if(IsVaildVIDPID(idVendor,idProduct))
                {
                    //For Modem port notify	
                    TSTRING strUSBModemPort = GetUSBDeviceInfo(idProduct,idVendor,strPara,_T("Modem"),_T("Device Parameters"),_T("PortName"));
                    AddPortNotify(strUSBModemPort,lpbv->dbcc_name,DEVICE_CONNECT);

                    ////For Diag port notify
                    TSTRING strUSBDiagPort = GetUSBDeviceInfo(idProduct,idVendor,strPara,_T("Ports"),_T("Device Parameters"),_T("PortName"));
                    AddPortNotify(strUSBDiagPort,lpbv->dbcc_name,DEVICE_DIAG_CONNECT);
                }

                delete pDeviceInterfaceDetail;
            }
            ::SetupDiDestroyDeviceInfoList(hDeviceInfoSet);
        }
    }
}

void CDectectDevice::RemoveDevice(const TSTRING& strDriverClass)
{
    TRACE_FILE_LINE_INFO();

    for(DeviceMapIter Iter = m_Devices.begin();Iter != m_Devices.end();Iter++)
    {
        if(Iter->second->m_strDeviceClass == strDriverClass)
        {
            RemoveDevice(Iter->second,DEVICE_DISCONNECT);
            break;
        }
    }
}

const TSTRING CDectectDevice::GetCOMPort(HDEVINFO hDeviceInfoSet,SP_DEVINFO_DATA spDeviceInfoData,const TSTRING& sSymbolicName)
{
    //HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\USB\Vid_22b8&Pid_2d01\5&15143e4d&0&2
    TRACE_FILE_LINE_INFO();

    TSTRING sUSBPortName;

    HANDLE hUSBHUB = ::CreateFile(sSymbolicName.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if(hUSBHUB != INVALID_HANDLE_VALUE)
    {
        int nDevAddress = 0;
        if(!::SetupDiGetDeviceRegistryProperty(hDeviceInfoSet, &spDeviceInfoData, SPDRP_ADDRESS , NULL, (unsigned char *)&nDevAddress, 4, NULL))
        {
            nDevAddress = LocatePort(hUSBHUB, hDeviceInfoSet, &spDeviceInfoData);
        }											
        else
        {												
            //Get Descriptors from USB HUB on port specified in nDevAddress
            DWORD nBytesReturned;
            USB_NODE_CONNECTION_INFORMATION ConnectionInformation;
            ZeroMemory(&ConnectionInformation, sizeof(ConnectionInformation));
            ConnectionInformation.ConnectionIndex = nDevAddress;

            if(::DeviceIoControl(hUSBHUB, IOCTL_USB_GET_NODE_CONNECTION_INFORMATION, &ConnectionInformation,	sizeof(ConnectionInformation), &ConnectionInformation, sizeof(ConnectionInformation), &nBytesReturned, NULL) 
                && ConnectionInformation.ConnectionStatus == DeviceConnected)
            {
                if(IsVaildVIDPID(ConnectionInformation.DeviceDescriptor.idVendor,ConnectionInformation.DeviceDescriptor.idProduct ))
                {							
                    sUSBPortName = GetQualcommPort(ConnectionInformation.DeviceDescriptor.idProduct,ConnectionInformation.DeviceDescriptor.idVendor);
                    if(sUSBPortName.size() == 0)
                    {
                        HKEY hKey = ::SetupDiOpenDevRegKey(hDeviceInfoSet, &spDeviceInfoData, DICS_FLAG_GLOBAL, 0, DIREG_DRV, KEY_READ);
                        if(hKey != INVALID_HANDLE_VALUE)
                        {
                            DWORD dwType = 0, dwRequiredSize = 0;
                            TSTRING sSymbolicName = GetKeyValue(hKey, _T("SymbolicName"),dwType,dwRequiredSize);
                            sUSBPortName = GetKeyValue(hKey, _T("PortName"),dwType,dwRequiredSize);
                            ::RegCloseKey(hKey);
                        }
                    }
                }
            }
        }
        ::CloseHandle(hUSBHUB);
    }
    return sUSBPortName;
}

bool CDectectDevice::IsReallyCOMPort(const TSTRING& strPortName)
{	
    TRACE_FILE_LINE_INFO();

    bool bRes = false;
    TSTRING strPort;
    strPort= _TC("//./") +strPortName;
    HANDLE hComm = ::CreateFile(strPort.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        0);
    bRes = (hComm != INVALID_HANDLE_VALUE);
    if(hComm) ::CloseHandle(hComm);
    return bRes;
};

TSTRING CDectectDevice::GetQualcommPortName(HKEY hKey,const TSTRING& strSubKey)
{
    TRACE_FILE_LINE_INFO();

    TSTRING strPort;
    TSTRING strDeviceReg = strSubKey + _T("\\Device Parameters");
    HKEY hDeviceKey = NULL;
    if(::RegOpenKey(hKey,strDeviceReg.c_str(),&hDeviceKey) == ERROR_SUCCESS)
    {
#define MAX_VALUE_LENGTH 16383
        TCHAR szBuffer[MAX_VALUE_LENGTH] = {0};
        DWORD dwBufLen=MAX_VALUE_LENGTH*sizeof(TCHAR);
        LONG lRet = ::RegQueryValueEx( hDeviceKey, _T("PortName"), NULL, NULL,(LPBYTE) szBuffer, &dwBufLen);
        if( (lRet == ERROR_SUCCESS) && (dwBufLen < MAX_VALUE_LENGTH*sizeof(TCHAR)) )
            strPort = szBuffer;
        ::RegCloseKey( hDeviceKey );
    }
    return strPort;
}

bool IsWinXP()
{
    bool bIsWinXP = false;

    DWORD dwVersion = ::GetVersion();

    DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
    DWORD dwWindowsMinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));

    if (dwWindowsMajorVersion == 5)
    {
        if(dwWindowsMinorVersion == 1)
        { //Windows XP
            bIsWinXP =  true;
        }
    }
    return bIsWinXP;
}

bool IsWin2000()
{
    bool bIsWin2000 = false;

    DWORD dwVersion = ::GetVersion();

    DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
    DWORD dwWindowsMinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));

    if (dwWindowsMajorVersion == 5)
    {
        if(dwWindowsMinorVersion == 0)
        { //Windows 2000
            bIsWin2000 =  true;
        }
    }

    return bIsWin2000;
}

TSTRING CDectectDevice::GetUSBDeviceInfo(const USHORT idProduct,const USHORT idVendor,const TSTRING& strPara,const TSTRING& strClass,const TSTRING& strSubFolder,const TSTRING& strField)
{
    TSTRING strValue;

    TSTRING strReg = _T("SYSTEM\\CurrentControlSet\\Enum\\USB\\Vid_%04x&Pid_%04x");

    CString strFormat;
    strFormat.Format(strReg.c_str(),idVendor,idProduct);

    strReg = strFormat;
    strReg += _T("\\") + strPara;

    AutoHKEY hKey(HKEY_LOCAL_MACHINE,strReg.c_str(),0, KEY_READ);
    TSTRING strDeviceClass = hKey.GetValue(_T("Class"));
    TSTRING strDeviceDesc = hKey.GetValue(_T("DeviceDesc"));
    TSTRING strParentIdPrefix = hKey.GetValue(_T("ParentIdPrefix"));

    if(strDeviceClass != strClass && strDeviceDesc == _T("USB Composite Device"))
    {
        //Find other USB Composite Devices
        //USB\Vid_22b8&Pid_2d01&MI_00\7&105cb4db&0&0000
        TSTRING strComposite;
        if(IsWin2000())
            strComposite = _T("&MI_%02d\\") + strParentIdPrefix + _T("&%d");
        else if(IsWinXP())
            strComposite = _T("&MI_%02d\\") + strParentIdPrefix + _T("&%04d");
        else//Vista and latest
            strComposite = _T("&MI_%02d\\") + strParentIdPrefix + _T("&%04d");

        for(int i=0;i<5;i++)
        {
            CString strNewComposite;
            strNewComposite.Format(strComposite.c_str(),i,i);
            TSTRING strReg = strFormat + strNewComposite;

            AutoHKEY hKey(HKEY_LOCAL_MACHINE,strReg.c_str(),0, KEY_READ);
            TSTRING strDeviceClass = hKey.GetValue(_T("Class"));
            if(strDeviceClass == strClass)
            {
                if(!strSubFolder.empty())
                    strReg += _T("\\") + strSubFolder;
                AutoHKEY hKey(HKEY_LOCAL_MACHINE,strReg.c_str(),0, KEY_READ);

                strValue = hKey.GetValue(strField);
                break;
            }
        }
    }

    return strValue;
}

TSTRING CDectectDevice::GetQualcommPort(const USHORT idProduct,const USHORT idVendor)
{
    //HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\USB\Vid_22b8&Pid_2d01\5&15143e4d&0&2
    TRACE_FILE_LINE_INFO();

    ::Sleep(300);

    TSTRING strCOMPort;

    TSTRING strReg = _T("SYSTEM\\CurrentControlSet\\Enum\\USB\\Vid_%04x&Pid_%04x&MI_00");

    CString strFormat;
    strFormat.Format(strReg.c_str(),idVendor,idProduct);

    HKEY hKey;
    LONG lRet = ::RegOpenKeyEx( HKEY_LOCAL_MACHINE,strFormat,0, KEY_READ, &hKey );	
    if( lRet == ERROR_SUCCESS )
    {
        int nTestIndex = 0;
        TSTRING strSubKey = GetSubKey(hKey,nTestIndex);
        while(strSubKey.size() > 0)
        {
            TSTRING strPort;					
            strPort = GetQualcommPortName(hKey,strSubKey);
            if(strPort.size() > 0)
            {
                if(IsReallyCOMPort(strPort))
                {
                    TSTRING strModule;
                    if(IsMODEM(strPort,strModule))
                    {
                        strCOMPort = strPort;
                        break;
                    }
                }
            }
            nTestIndex++;
            strSubKey = GetSubKey(hKey,nTestIndex);
        }
        ::RegCloseKey( hKey );
    }

    return strCOMPort;
}

TSTRING CDectectDevice::GetSubKey(HKEY hKey,int nIndex)
{
    TRACE_FILE_LINE_INFO();

    TSTRING strKey;			
#define MAX_KEY_LENGTH 255
    TCHAR szBuffer[MAX_KEY_LENGTH] = {0};
    DWORD dwBufLen=MAX_KEY_LENGTH*sizeof(TCHAR);
    FILETIME ftLastWriteTime;
    if(::RegEnumKeyEx(hKey, nIndex,szBuffer,&dwBufLen,NULL,NULL,NULL,&ftLastWriteTime) == ERROR_SUCCESS)
        strKey = szBuffer;
    return strKey;
}
const TSTRING CDectectDevice::GetKeyValue(HKEY hKey,const TSTRING& strName,DWORD& dwType,DWORD& dwRequiredSize)
{
    TRACE_FILE_LINE_INFO();

    TSTRING strValue;
    if(::RegQueryValueEx(hKey, strName.c_str(), NULL, NULL, NULL, &dwRequiredSize) == ERROR_SUCCESS)
    {
        PBYTE pbyPortName = new BYTE[dwRequiredSize+2];
        ::ZeroMemory(pbyPortName,dwRequiredSize+2);
        if(::RegQueryValueEx(hKey, strName.c_str(), NULL, &dwType, pbyPortName, &dwRequiredSize) == ERROR_SUCCESS)
            strValue.assign((LPTSTR) pbyPortName);
        delete [] pbyPortName;
    }
    return strValue;
}

char CDectectDevice::FirstDriveFromMask (ULONG unitmask)
{
    TRACE_FILE_LINE_INFO();

    char i = 0;
    for (i = 0; i < 26; ++i)
    {
        if (unitmask & 0x1)
            break;
        unitmask = unitmask >> 1;
    }
    return (i + 'A');
}

bool CDectectDevice::DetectExistingPorts()
{
    TRACE_FILE_LINE_INFO();

    vector<TSTRING> vExistingPorts;

    if ( GetExistingPortNumbers(&vExistingPorts) )
    {
        for ( vector <TSTRING>::iterator v_Iter = vExistingPorts.begin() ; v_Iter != vExistingPorts.end() ; v_Iter++ )
        {			
            if(::WaitForSingleObject(m_DetectThreadEventAbort,0) == WAIT_OBJECT_0)
                break;

            TSTRING	strExtName;
            TRACE(_T("Checking %s\n"),v_Iter->c_str());
            if(IsDiag(*v_Iter))
            {
                DeviceData* ptdevData = new DeviceData(DBT_DEVTYP_PORT,*v_Iter);				
                //ptdevData->m_strExtName = strExtName;
                TRACE(_T("PostMessage WM_DIAG_FOUND\n"));				
                ::SendMessage(m_hWnd, WM_DIAG_FOUND, (WPARAM)ptdevData, 0);
            }
            else if(IsMODEM(*v_Iter,strExtName))
            {				
                DeviceData* ptdevData = new DeviceData(DBT_DEVTYP_PORT,*v_Iter);				
                ptdevData->m_strExtName = strExtName;
                TRACE(_T("PostMessage WM_MODEM_FOUND\n"));				
                ::SendMessage(m_hWnd, WM_MODEM_FOUND, (WPARAM)ptdevData, 0);
            }
        }
    }	

    return true;
}

bool CDectectDevice::GetExistingPortNumbers(vector<TSTRING> *PortIDs)
{
    TRACE_FILE_LINE_INFO();

    LONG  lRet;
    DWORD nEntryCount = 0;
    DWORD nEntryIndex = 0;
    const unsigned int maxBufLen = 256;
    TCHAR sEntryName[maxBufLen];
    DWORD nNameLen = maxBufLen;
    CString strEntryName;
    DWORD nType;
    TCHAR sEntryData[maxBufLen];
    DWORD nDataLen = maxBufLen;

    memset(sEntryName,0,maxBufLen);
    memset(sEntryData,0,maxBufLen);

    HKEY  hkeySERIALCOMM;
    GetSerialPortHKEY( hkeySERIALCOMM );

    lRet = ::RegQueryInfoKey(hkeySERIALCOMM, NULL, NULL, NULL, NULL, NULL, NULL, &nEntryCount, NULL, NULL, NULL, NULL);
    if(lRet != ERROR_SUCCESS || nEntryCount==0)
        return false;

    for(nEntryIndex = 0; nEntryIndex < nEntryCount; nEntryIndex++)
    {
        nNameLen = maxBufLen;
        nDataLen = maxBufLen;

        lRet = ::RegEnumValue(hkeySERIALCOMM, nEntryIndex, sEntryName, &nNameLen,NULL, &nType, (BYTE*)sEntryData, &nDataLen);

        // if not successfully read value, jump to next entry. more data is OK to process.
        if (! (lRet== ERROR_SUCCESS || lRet==ERROR_MORE_DATA) )
        {
            continue;
        }

        strEntryName = CString(sEntryName,nNameLen);

        // Find serial cable registry
        if ( nType==REG_SZ && strEntryName.Find(_T("\\Device"),0)==0 ) //XP,2000:Entry Type must equal to REG_SZ and Entry Name must begin with "\Device\Serial"
        {
            TSTRING strPort = sEntryData;
            PortIDs->push_back(strPort);
        }
        // Find USB cable registry
        else if( (strEntryName.Find(_T("\\Device\\AITSerial"))==0)    //For AIT USB Driver
            || (strEntryName.Find(_T("\\Device\\QCUSB_")) ==0))  //For Qualcomm Platform
        {
            TSTRING strUSBPort = sEntryData;
            PortIDs->insert( PortIDs->begin(), strUSBPort); //USB ports are more likely used, therefore the priority is higher and insert to the beginning of the port List
        }
        else; // other string we do not deal with because it may take too much time
    }

    return true;
}

bool CDectectDevice::isPortExists(const TSTRING& strPort, vector<TSTRING> &vExistingPorts)
{
    TRACE_FILE_LINE_INFO();

    bool    bExist = false;
    TSTRING strCOMPort;
    vector <TSTRING>::iterator v_Iter;

    for ( v_Iter = vExistingPorts.begin() ; v_Iter != vExistingPorts.end() ; v_Iter++ )
    {
        strCOMPort = *v_Iter;
        if ( strPort == strCOMPort)
        {
            bExist = true;
            break;
        }
    }
    return bExist;
}

bool CDectectDevice::GetSerialPortHKEY(HKEY &hkeySERIALCOMM)
{
    TRACE_FILE_LINE_INFO();

    LONG lRet;
    HKEY hkeyHARDWARE;
    HKEY hkeyDEVICEMAP;

    lRet = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE"), 0, KEY_QUERY_VALUE, &hkeyHARDWARE);
    if(lRet != ERROR_SUCCESS)  //fail to open HARDWARE Registry Entry
    {
        return false;
    }

    lRet = ::RegOpenKeyEx(hkeyHARDWARE, _T("DEVICEMAP"), 0, KEY_QUERY_VALUE, &hkeyDEVICEMAP);
    if(lRet != ERROR_SUCCESS)  //fail to open DEVICEMAP Registry Entry
    {
        return false;
    }

    lRet = ::RegOpenKeyEx(hkeyDEVICEMAP, _T("SERIALCOMM"), 0, KEY_QUERY_VALUE, &hkeySERIALCOMM);
    if(lRet != ERROR_SUCCESS)  //fail to open SERIALCOMM Registry Entry
    {
        return false;
    }

    RegCloseKey(hkeyHARDWARE);
    RegCloseKey(hkeyDEVICEMAP);

    return true;
}

const TSTRING CDectectDevice::GetDeviceProperty(HDEVINFO hDeviceInfoSet,SP_DEVINFO_DATA spDeviceInfoData, DWORD dwProperty)
{
    TRACE_FILE_LINE_INFO();

    TSTRING strTemp;
    DWORD dwType = 0, dwRequiredSize = 0;
    ::SetupDiGetDeviceRegistryProperty(hDeviceInfoSet, &spDeviceInfoData, dwProperty , &dwType, NULL, 0, &dwRequiredSize);
    if(dwRequiredSize > 0)
    {
        dwRequiredSize += sizeof(TCHAR);
        PBYTE pbyDeviceDesc = new BYTE[dwRequiredSize];
        if(::SetupDiGetDeviceRegistryProperty(hDeviceInfoSet, &spDeviceInfoData, dwProperty , &dwType, pbyDeviceDesc, dwRequiredSize, &dwRequiredSize))
        {
            ::_tcsupr_s((LPTSTR) pbyDeviceDesc,dwRequiredSize/sizeof(TCHAR));
            strTemp.assign((LPTSTR) pbyDeviceDesc);
        }
        delete [] pbyDeviceDesc;
    }
    return strTemp;
}

const TSTRING CDectectDevice::GetsSymbolicName(HDEVINFO hDeviceInfoSet,SP_DEVINFO_DATA spDeviceInfoDataHub)
{
    TRACE_FILE_LINE_INFO();

    TSTRING sSymbolicName;
    HKEY hKey = ::SetupDiOpenDevRegKey(hDeviceInfoSet, &spDeviceInfoDataHub, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);
    if(hKey != INVALID_HANDLE_VALUE)
    {
        DWORD dwType=0, dwRequiredSize=0;
        sSymbolicName = GetKeyValue(hKey, _T("SymbolicName"),dwType,dwRequiredSize);				
        TSTRING sFrom(_T("\\??\\"));
        TSTRING::size_type nPos = 0;
        while ( TSTRING::npos != (nPos = sSymbolicName.find(sFrom)) )
            sSymbolicName.replace(nPos, sFrom.size(), _T("\\\\.\\"));
        ::RegCloseKey(hKey);
    }
    return sSymbolicName;
}

void CDectectDevice::AddDevice(const DeviceDataPt ptdevData,const TSTRING& strEvent)
{
    TRACE_FILE_LINE_INFO();

    CSingleLock singleLock(&m_CritSection,TRUE);
    DeviceMapIter Iter = m_Devices.find(ptdevData->ID());
    if(Iter == m_Devices.end())
    {
        m_Devices.insert(DeviceMapPair(ptdevData->ID(),ptdevData));
        m_hDeviceConnectionStatusEvent.ResetEvent();

        ptdevData->SetEvent(true);
        if(m_bEnable)			
            Fire(strEvent,(long)&ptdevData);
    }
}

void CDectectDevice::RemoveDeviceFromMap(DeviceMapIter Iter)
{
    TRACE_FILE_LINE_INFO();

    if(Iter != m_Devices.end())
    {
        m_Devices.erase(Iter);
    }
}

void CDectectDevice::RemoveDevice(const DeviceDataPt ptdevData,const TSTRING& strEvent)
{
    TRACE_FILE_LINE_INFO();

    CSingleLock singleLock(&m_CritSection,TRUE);
    DeviceMapIter Iter = m_Devices.find(ptdevData->ID());
    while(Iter != m_Devices.end())
    {
        RemoveDeviceFromMap(Iter);
        Iter = m_Devices.find(ptdevData->ID());
    }

    m_hDeviceConnectionStatusEvent.SetEvent();

    ptdevData->SetEvent(false);
    if(m_bEnable)		
        Fire(strEvent,(long)&ptdevData);
}
int CDectectDevice::RegisterNotification()
{
    DEV_BROADCAST_DEVICEINTERFACE filter;
    memset(&filter,0,sizeof(DEV_BROADCAST_DEVICEINTERFACE));
    filter.dbcc_size         = sizeof(filter);
    filter.dbcc_devicetype   = DBT_DEVTYP_DEVICEINTERFACE;
    filter.dbcc_classguid    = GUID_CLASS_USB_DEVICE ;
    m_hDevNotify = RegisterDeviceNotification(m_hWnd, &filter, 0);
    return 0;
}
int CDectectDevice::UnregisterNotification()
{
    UnregisterDeviceNotification(m_hDevNotify);
    m_hDevNotify = NULL;
    return 0;
}

void CDectectDevice::RemovePortNotify(const TSTRING& strPort,const TSTRING& strEvent)
{
    if(!strPort.empty())
    {
        for(DeviceMapIter Iter = m_Devices.begin();Iter != m_Devices.end();Iter++)
        {
            if(Iter->second->m_strPort == strPort)
            {
                RemoveDevice(Iter->second,strEvent);
                break;
            }
        }
    }
}

void CDectectDevice::GetDevice(PDEV_BROADCAST_DEVICEINTERFACE lpbv,UINT nEventType)
{
    if(lpbv)
    {
        if(nEventType == DBT_DEVICEREMOVECOMPLETE)
        {
            USHORT idVendor;
            USHORT idProduct;
            TSTRING strPara;
            ParserDBCCName(lpbv->dbcc_name,idVendor,idProduct,strPara);

            //For Modem port notify	
            TSTRING strUSBModemPort = GetUSBDeviceInfo(idProduct,idVendor,strPara,_T("Modem"),_T("Device Parameters"),_T("PortName"));
            RemovePortNotify(strUSBModemPort,DEVICE_DISCONNECT);

            ////For Diag port notify
            TSTRING strUSBDiagPort = GetUSBDeviceInfo(idProduct,idVendor,strPara,_T("Ports"),_T("Device Parameters"),_T("PortName"));
            RemovePortNotify(strUSBDiagPort,DEVICE_DIAG_DISCONNECT);
        }
        else if(nEventType == DBT_DEVICEARRIVAL)
        {
            AddDevice(lpbv);
        }
    }
}


BOOL CDectectDevice::CreateGhostWnd()
{
    if(!IsWindow(m_hWnd))
    {
        if(RegisterWindow())
        {
            m_hWnd = ::CreateWindow(
                m_strClassName,m_strTitleName,WS_OVERLAPPEDWINDOW, 
                CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
                NULL,NULL,::AfxGetApp()->m_hInstance,(void *)this);			
        }
    }	
    return IsWindow(m_hWnd);
}

BOOL CDectectDevice::RegisterWindow()
{
    WNDCLASSEX wcx;
    memset(&wcx,0,sizeof(WNDCLASSEX));

    wcx.cbSize = sizeof(WNDCLASSEX);						// Size of structure
    wcx.style = CS_HREDRAW | CS_VREDRAW;					// Redraw if size changes
    wcx.lpfnWndProc = CDectectDevice::stWinMsgHandler;		// Points to window procedure
    wcx.cbClsExtra = 0;										// No extra class memory
    wcx.cbWndExtra = 0;										// No extra window memory
    wcx.hInstance = ::AfxGetApp()->m_hInstance;				// Handle to instance
    wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);			// Predefined app. icon
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);				// Predefined arrow
    wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);// White background brush
    wcx.lpszMenuName = NULL;								// Name of menu resource
    wcx.lpszClassName = m_strClassName.GetBuffer(0);		// Name of window class
    wcx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);			// Small class icon

    return ::RegisterClassEx(&wcx) != NULL;
}

LRESULT CALLBACK CDectectDevice::WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(uMsg == WM_DEVICECHANGE)
        OnDeviceChange(wParam, lParam);
    else if ( uMsg == WM_MODEM_FOUND )
    {
        /*
        While device connected, the default action is to sync with mobile, and pop up a dialog "Sync succeeded"
        If user click down some and hold it down before device connected, and loose it after the dialog pop
        there will be something wrong because the button captures the mouse.
        So we should release mouse capture here.*/
        ReleaseCapture();


        TRACE(_T("Receive Message WM_MODEM_FOUND\n"));
        DeviceDataPt ptdevData = (DeviceData*)(wParam);
        AddDevice(ptdevData,DEVICE_CONNECT);
    }
    else if ( uMsg == WM_DIAG_FOUND )
    {
        /*
        While device connected, the default action is to sync with mobile, and pop up a dialog "Sync succeeded"
        If user click down some and hold it down before device connected, and loose it after the dialog pop
        there will be something wrong because the button captures the mouse.
        So we should release mouse capture here.*/
        ReleaseCapture();


        TRACE(_T("Receive Message WM_DIAG_FOUND\n"));
        DeviceDataPt ptdevData = (DeviceData*)(wParam);
        AddDevice(ptdevData,DEVICE_DIAG_CONNECT);
    }

    return ::DefWindowProc(m_hWnd,uMsg, wParam, lParam);
}

CDectectDevice* CDectectDevice::GetObjectFromWindow(HWND hWnd)
{
    return reinterpret_cast<CDectectDevice *>(GetWindowLong(hWnd, GWL_USERDATA));
}

LRESULT CALLBACK CDectectDevice::stWinMsgHandler(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{			
    if (uMsg == WM_NCCREATE)
    {
        SetWindowLong(hwnd, GWL_USERDATA,(long)((LPCREATESTRUCT(lParam))->lpCreateParams));
        return TRUE;
    }
    CDectectDevice* pWnd = GetObjectFromWindow(hwnd);
    if (pWnd)
        return pWnd->WinMsgHandler(hwnd, uMsg, wParam, lParam);
    else
        return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

#pragma warning(pop)