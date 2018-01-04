/* *** THIS LINE MUST BE THE FIRST LINE AND DO NOT MODIFY OR REMOVE IT *** */
/***************************************************************************
 *
 *               Qisda QSyncher Project
 *
 *	   Copyright (C) 2005 BenQ Corp.
 *
 ***************************************************************************
 *
 *  $Workfile:   GPRSUtil.h  $
 *  $Revision:   1.3  $
 *  $Date:   Dec 10 2007 11:45:56  $
 *
 ***************************************************************************
 *
 * File Description
 * ----------------
 * 
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 * 
 ***************************************************************************/
 /*$Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Apps/GPRSMdmSetupWiz/GPRSUtil.h.-arc  $
 * 
 *    Rev 1.3   Dec 10 2007 11:45:56   Eddy Guo
 * update
 * 
 *    Rev 1.2   Dec 07 2007 09:13:42   Eddy Guo
 * update
 * 
 *    Rev 1.1   Feb 08 2006 21:46:54   Steven CC Chen
 * [Fix]: 1. Hardware_Flow_Control Off 2. Modem Device Installation failure 3. INF file.
 * Resolution for 478: [HerC1A][GPRS Modem M580]: Release V0.1 for M580.
 * 
 *    Rev 1.0   Sep 15 2005 08:54:46   Jane Yu
 * Initial revision.
  
   */
/**************************************************************************/
/* *** <<<DO NOT MODIFY OR REMOVE THIS LINE>>> *** */

//===========================================================================//
//  Function      :   GPRS Modem Installation related functions.             //
//  Release Date  :   2005/08/16                                             //
//  Rivision Date :   2006/02/08                                             //
//                                                                           //
//  Author:  Steven CC Chen <Steven.CC.Chen@BenQ.com>                        //
//  Copyright(C) 2005-2006  All rights reserved.                             //
//===========================================================================//
#ifndef __GPRSUTIL_H__
#define __GPRSUTIL_H__

#pragma once

#include	"stdafx.h"

//use Standard Exception:
#include	<stdexcept>
using std::runtime_error;


//--[Modem Install]--
#include	"setupapi.h"					//(VC 6.0 include)
#pragma comment(lib, "setupapi")			//(VC 6.0 library)

#include	"./newdev.h"				//(DDK 2600.1106 include)
#pragma comment(lib, "./newdev")		//(DDK 2600.1106 library)
//--[Modem Install]--

//--[Dial-up Connection]--
#include	"ras.h"
#pragma comment(lib, "rasapi32")
//--[Dial-up Connection]--


//--[Modem Install]--
#ifndef				MAX_CLASS_NAME_LEN
	#define				MAX_CLASS_NAME_LEN						32
#endif

#ifndef				ERROR_NO_CATALOG_FOR_OEM_INF  //(DDK 2600.1106 include)
	#define				ERROR_NO_CATALOG_FOR_OEM_INF			(APPLICATION_ERROR_MASK | ERROR_SEVERITY_ERROR | 0x22F)
#endif
//--[Modem Install]--

//--[Dial-up Connection]--
#ifndef				ERROR_CANNOT_FIND_PHONEBOOK
	#define				ERROR_CANNOT_FIND_PHONEBOOK				0x0000026F
#endif

#ifndef				ERROR_BUFFER_INVALID
	#define				ERROR_BUFFER_INVALID					0x00000262
#endif

#ifndef				ERROR_CANNOT_OPEN_PHONEBOOK
	#define				ERROR_CANNOT_OPEN_PHONEBOOK				0x0000026D
#endif

#ifndef				ERROR_CANNOT_FIND_PHONEBOOK_ENTRY
	#define				ERROR_CANNOT_FIND_PHONEBOOK_ENTRY		0x0000026F
#endif
//--[Dial-up Connection]--

//--[Modem Install]--
#define				TAG_MODEM_CALSS_ATTACHEDTO					_T("AttachedTo")
#define				TAG_MODEM_CALSS_USERINIT					_T("Userinit")
#define				DEFAULT_MODEM_PORT_ID						_T("COM3")
#define				DEFAULT_MODEM_INIT_COMMAND					_T("AT+CGDCONT=1,\"IP\",\"emome\",\"\",0,0<cr>")
//--[Modem Install]--

//--[Dial-up Connection]--
#ifndef DEFAULT_GPRS_DIAL_NUMBER
    #define				DEFAULT_GPRS_DIAL_NUMBER					"*99#"
#endif
//--[Dial-up Connection]--


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGPRSUtil
{
public:
    CGPRSUtil() {}
    virtual ~CGPRSUtil() {}
	
	//--[Modem Install]--
public:
	static bool	GetSystemErrorMsg( CString & out_szErrorMsg, DWORD in_nErrorCode, CString in_szModuleName = _T("") );
	static bool	GetExeDir( CString & out_szDirPath, const HMODULE in_hModule = NULL );
	static bool	InstallModemDevice( const CString in_szINFFileName, const CString in_szHardwareID, const CString in_szCOMPortID, bool in_bIfForceInstall = false );
	static bool	UnInstallDevice( const CString in_szHardwareID );
    // write registry for the APN
	static bool	ChangeDeviceProperty( const CString in_szHardwareID, const CString in_szPropertyName, const CString in_szPropertyValue, DWORD in_dwRegType = REG_SZ );
	//--[Modem Install]--
	
	//--[Dial-up Connection]--
public:
	static bool CreateDialupConnection( const CString in_szConnectionName, const CString in_szDailNumber, const CString in_szModemName, const CString in_szUserName = _T(""), const CString in_szPassword = _T("") );
	static bool	DeleteDialupConnection( const CString in_szConnectionName );
	//--[Dial-up Connection]--
	
private:
	static bool	FindExistingDevice( const CString in_szHardwareID, DWORD & out_dwErrorCode );
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline bool CGPRSUtil::GetSystemErrorMsg(CString &out_szErrorMsg, DWORD in_nErrorCode, CString in_szModuleName /*=""*/) {
    LPTSTR ptszMsg = NULL;

    //To-do: Retrieve the pre-defined Error Message from the specified DLL module or form System.
    if(in_szModuleName.IsEmpty()) {
        if(::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, in_nErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &ptszMsg, 0, NULL)) {
            out_szErrorMsg = CString((LPCTSTR)ptszMsg);
        }
    } else {
        if(::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE, ::GetModuleHandle((LPCTSTR)in_szModuleName), in_nErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &ptszMsg, 0, NULL)) {
            out_szErrorMsg = CString((LPCTSTR)ptszMsg);
        }
    }

    //(clean up.)
    if(ptszMsg != NULL)
        ::LocalFree((LPVOID)ptszMsg);

    //To-do: Verify the result.
    if(out_szErrorMsg.IsEmpty())
        return false;
    else
        return true;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline bool CGPRSUtil::GetExeDir(CString &out_szDirPath, const HMODULE in_hModule /*=NULL*/) {
    CString szTemp;
    TCHAR ptszFullPath[MAX_PATH];
    ZeroMemory(ptszFullPath, sizeof(ptszFullPath));
    if(::GetModuleFileName(in_hModule, ptszFullPath, sizeof(ptszFullPath)) == 0) {
        return false;
    } else {
        szTemp = CString((LPCTSTR)ptszFullPath);
        if(szTemp.IsEmpty()) {
            return false;
        } else {
            int nDelimiterPos =  - 1;
            for(int i = szTemp.GetLength() - 1; i >= 0; i--) {
                if(szTemp.GetAt(i) == '\\') {
                    nDelimiterPos = i;
                    break;
                }
            }

            if(nDelimiterPos >= 0)
             { //(the Delimiter found.)
                out_szDirPath = szTemp.Mid(0, nDelimiterPos);

                return true;
            } else {
                return false;
            }
        }
    }
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline bool CGPRSUtil::FindExistingDevice(const CString in_szHardwareID, DWORD &out_dwErrorCode) {
    bool bIsFound = false;
    CString szErrorMsg = _T("");
    try {
        //To-do-1: Generate a Device Information Set of all installed devices on the local machine.
        HDEVINFO DeviceInfoSet = NULL;
        DeviceInfoSet = ::SetupDiGetClassDevs(NULL, NULL, 0, DIGCF_ALLCLASSES | DIGCF_PRESENT);
        if(DeviceInfoSet == INVALID_HANDLE_VALUE) {
            DWORD dwErrorCode = ::GetLastError();
            out_dwErrorCode = dwErrorCode;
            CString szSystemMsg;
            if(GetSystemErrorMsg(szSystemMsg, dwErrorCode, _T("setupapi.dll"))) {
                //szErrorMsg.Format("SetupDiGetClassDevs error -- %s", szSystemMsg);
                //throw std::runtime_error((LPCTSTR)szErrorMsg);
            } else {
                //szErrorMsg.Format("SetupDiGetClassDevs error -- 0x%8.8lX", dwErrorCode);
                //throw std::runtime_error((LPCTSTR)szErrorMsg);
            }
        }

        //To-do-2: Find the target device by HardwareID through enumerating all devices contained in the Device Information Set.
        DWORD dwErrorCode;
        SP_DEVINFO_DATA DeviceInfoData;
        ZeroMemory(&DeviceInfoData, sizeof(SP_DEVINFO_DATA));
        DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
        for(DWORD i = 0; ::SetupDiEnumDeviceInfo(DeviceInfoSet, i, &DeviceInfoData) && ( ! bIsFound); i++) {
            //by <HardwareID>:
            DWORD dwRegType;
            LPTSTR ptszBuffer = NULL;
            DWORD dwBufferSize = 0;
            while( ! ::SetupDiGetDeviceRegistryProperty(DeviceInfoSet, &DeviceInfoData, SPDRP_HARDWAREID, &dwRegType, (PBYTE)ptszBuffer, dwBufferSize, &dwBufferSize))
             { //(*使用[動態成長buffer大小]法)
                dwErrorCode = ::GetLastError();
                if(dwErrorCode == ERROR_INSUFFICIENT_BUFFER) {
                    //(use the RequiredSize and try again.)
                    if(ptszBuffer != NULL)
                        ::LocalFree((LPVOID)ptszBuffer);
                    ptszBuffer = (LPTSTR)::LocalAlloc(LPTR, (UINT)dwBufferSize);

                    continue;
                } else {
                    CString szSystemMsg;
                    if(GetSystemErrorMsg(szSystemMsg, dwErrorCode, _T("setupapi.dll"))) {
                        //szErrorMsg.Format("SetupDiGetDeviceRegistryProperty error -- %s", szSystemMsg);
                        //TRACE("[CGPRSUtil::FindExistingDevice]: %s.\n", szErrorMsg);
                    } else {
                        //szErrorMsg.Format("SetupDiGetDeviceRegistryProperty error -- 0x%8.8lX", dwErrorCode);
                        //TRACE("[CGPRSUtil::FindExistingDevice]: %s.\n", szErrorMsg);
                    }

                    break;
                }
            }

            if(dwErrorCode == ERROR_INVALID_DATA) { //(*supposed to a device w/o HardwareID.)
                //(Do nothing.)
                TRACE("ERROR_INVALID_DATA!!\n");
            } else {
                //Compare the given HardwareID with each HardwareID contained in the resulting "ptszBuffer (a multi-zero string)".
                LPTSTR ptszTempHardwareID = NULL;
                for(ptszTempHardwareID = ptszBuffer; (*ptszTempHardwareID) && (ptszTempHardwareID <  &ptszBuffer[dwBufferSize]); ptszTempHardwareID += ((_tcslen(ptszTempHardwareID) + 1+1) *sizeof(TCHAR))) {
                    if(_tcscmp(ptszTempHardwareID, (LPCTSTR)in_szHardwareID) == 0)
                     { //(found.)
                        bIsFound = true;
                        break;
                    }
                }
            }

            //(clean up.)
            if(ptszBuffer != NULL)
                ::LocalFree((LPVOID)ptszBuffer);
        }

        dwErrorCode = ::GetLastError();
        //--- free the no longer needed Device Information Set ---
        if( ! ::SetupDiDestroyDeviceInfoList(DeviceInfoSet)) {
            DWORD dwErrCode = ::GetLastError();
            CString szSystemMsg;
            if(GetSystemErrorMsg(szSystemMsg, dwErrCode, _T("setupapi.dll"))) {
                //szErrorMsg.Format("SetupDiDestroyDeviceInfoList error -- %s", szSystemMsg);
                //TRACE("[CGPRSUtil::FindExistingDevice]: %s.\n", szErrorMsg);
            } else {
                //szErrorMsg.Format("SetupDiDestroyDeviceInfoList error -- 0x%8.8lX", dwErrCode);
                //TRACE("[CGPRSUtil::FindExistingDevice]: %s.\n", szErrorMsg);
            }
        }
        //--- free the no longer needed Device Information Set ---

        //To-do: Verify the result.
        if((dwErrorCode != NO_ERROR) && (dwErrorCode != ERROR_NO_MORE_ITEMS))
         { //(error.)
            out_dwErrorCode = dwErrorCode;
            CString szSystemMsg;
            if(GetSystemErrorMsg(szSystemMsg, dwErrorCode, _T("setupapi.dll"))) {
                //szErrorMsg.Format("SetupDiEnumDeviceInfo error -- %s", szSystemMsg);
                //throw std::runtime_error((LPCTSTR)szErrorMsg);
            } else {
                //szErrorMsg.Format("SetupDiEnumDeviceInfo error -- 0x%8.8lX", dwErrorCode);
                //throw std::runtime_error((LPCTSTR)szErrorMsg);
            }
        } else {
            return bIsFound;
        }
    } catch(std::exception &e) {
        CStringA strWhat = e.what();
        TRACE("[CGPRSUtil::FindExistingDevice]: %s.\n", e.what());
        ///::MessageBox(0, e.what(), AfxGetApp()->m_pszAppName, MB_OK | MB_ICONERROR);

        return false;
    } catch(...) {
        TRACE("[CGPRSUtil::FindExistingDevice]: Unrecognized exception.\n");
        ///::MessageBox(0, (LPCTSTR)"Unrecognized exception", AfxGetApp()->m_pszAppName, MB_OK | MB_ICONERROR);

        return false;
    }
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------


inline bool CGPRSUtil::InstallModemDevice(const CString in_szINFFileName, const CString in_szHardwareID, const CString in_szCOMPortID, bool in_bIfForceInstall /*=false*/) {
    bool bIsSuccess = true;
    CString szINFFileName = CString(in_szINFFileName);
    CString szHardwareID = CString(in_szHardwareID); //(*case-insensitive; 預設為小寫英文字母.)
    CString szCOMPortID = CString(in_szCOMPortID);
    CString szINFFilePath = _T(""); //(目前是設為"跟本執行檔在同一個目錄下".)

    TCHAR ptszHardwareID[128];
    memset(ptszHardwareID, 0, 128 *sizeof(TCHAR));
    _tcscpy(ptszHardwareID, szHardwareID);
    TCHAR ptszCOMPortID[64];
    memset(ptszCOMPortID, 0, 64 *sizeof(TCHAR));
    _tcscpy(ptszCOMPortID, szCOMPortID);

    HDEVINFO DeviceInfoSet;
    SP_DEVINFO_DATA DeviceInfoData;
    bool bIsDeviceNodeExisted = false;

    DWORD dwErrCode = 0;
    CString szErrorMsg = _T("");
    BOOL bRet = TRUE;
    HRESULT hr;



    try {
        CString szINFDir;
        GetExeDir(szINFDir, ::GetModuleHandle(NULL));
        szINFFilePath = szINFDir + CString(_T("\\")) + szINFFileName;

        //To-do-1: Get the Device Class information from the given INF file.
        GUID ClassGUID;
        TCHAR ptszClassName[MAX_CLASS_NAME_LEN];
        bRet = ::SetupDiGetINFClass((LPCTSTR)szINFFilePath, &ClassGUID, ptszClassName, sizeof(ptszClassName), 0);
        if(bRet == FALSE) {
            DWORD dwErrorCode = ::GetLastError();
            CString szSystemMsg;
            if(GetSystemErrorMsg(szSystemMsg, dwErrorCode)) {
                //szErrorMsg.Format("SetupDiGetINFClass error -- %s", szSystemMsg);
                //throw std::runtime_error((LPCTSTR)szErrorMsg);
            } else {
                //szErrorMsg.Format("SetupDiGetINFClass error -- 0x%8.8lX", dwErrorCode);
                //throw std::runtime_error((LPCTSTR)szErrorMsg);
            }
        }

        //To-do-2: Check whether the specified device exists or not first.
        DWORD dwError = 0;
        if(FindExistingDevice(szHardwareID, dwError)) {
            bIsDeviceNodeExisted = true;

            TRACE("[CGPRSUtil::InstallModemDevice]: The device node '%s' exists already.\n", szHardwareID);
        }


        if( ! bIsDeviceNodeExisted)
         { //(create the new device node and register it.)
            //To-do-3: Create the Device Information Set for the Device Class.
            //(Bug) DeviceInfoSet = NULL;
            DeviceInfoSet = 0;
            DeviceInfoSet = ::SetupDiCreateDeviceInfoList(&ClassGUID, 0);
            if(DeviceInfoSet == INVALID_HANDLE_VALUE) {
                DWORD dwErrorCode = ::GetLastError();
                CString szSystemMsg;
                if(GetSystemErrorMsg(szSystemMsg, dwErrorCode, _T("setupapi.dll"))) {
                    //szErrorMsg.Format("SetupDiCreateDeviceInfoList error -- %s", szSystemMsg);
                    //throw std::runtime_error((LPCTSTR)szErrorMsg);
                } else {
                    //szErrorMsg.Format("SetupDiCreateDeviceInfoList error -- 0x%8.8lX", dwErrorCode);
                    //throw std::runtime_error((LPCTSTR)szErrorMsg);
                }
            }

            //To-do-4: Create a new Device Information Element and add it into the Device Information Set.
            //(Bug) ZeroMemory(&DeviceInfoData, sizeof(SP_DEVINFO_DATA));
            DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
            bRet = ::SetupDiCreateDeviceInfo(DeviceInfoSet, ptszClassName, &ClassGUID, NULL, 0, DICD_GENERATE_ID, &DeviceInfoData); //('DICD_GENERATE_ID'指定建立新的instance並自動numbering產生其ID.)
            if(bRet == FALSE) {
                DWORD dwErrorCode = ::GetLastError();
                CString szSystemMsg;
                if(GetSystemErrorMsg(szSystemMsg, dwErrorCode, _T("setupapi.dll"))) {
                    //szErrorMsg.Format("SetupDiCreateDeviceInfo error -- %s", szSystemMsg);
                    //throw std::runtime_error((LPCTSTR)szErrorMsg);
                } else {
                    //szErrorMsg.Format("SetupDiCreateDeviceInfo error -- 0x%8.8lX", dwErrorCode);
                    //throw std::runtime_error((LPCTSTR)szErrorMsg);
                }
            }

            //To-do-5: Set up the Device Properties of the Device Information Element.
            //Add the <HardwareID> property:
            //(Bug) bRet = ::SetupDiSetDeviceRegistryProperty(DeviceInfoSet, &DeviceInfoData, SPDRP_HARDWAREID, (CONST BYTE *)(LPCTSTR)szHardwareID, (DWORD)((szHardwareID.GetLength() + 1) * 2));
            bRet = ::SetupDiSetDeviceRegistryProperty(DeviceInfoSet, &DeviceInfoData, SPDRP_HARDWAREID, (LPBYTE)ptszHardwareID, (lstrlen(ptszHardwareID) + 1+1) *sizeof(TCHAR));
            if(bRet == FALSE) {
                DWORD dwErrorCode = ::GetLastError();
                CString szSystemMsg;
                if(GetSystemErrorMsg(szSystemMsg, dwErrorCode)) {
                    //szErrorMsg.Format("SetupDiSetDeviceRegistryProperty error -- %s", szSystemMsg);
                    //throw std::runtime_error((LPCTSTR)szErrorMsg);
                } else {
                    //szErrorMsg.Format("SetupDiSetDeviceRegistryProperty error -- 0x%8.8lX", dwErrorCode);
                    //throw std::runtime_error((LPCTSTR)szErrorMsg);
                }
            }

            //To-do-6: Manually register this new created non-PnP device instance with the Plug-and-Play Manager.
            bRet = ::SetupDiRegisterDeviceInfo(DeviceInfoSet, &DeviceInfoData, 0, NULL, NULL, NULL);
            if(bRet == FALSE) {
                DWORD dwErrorCode = ::GetLastError();
                CString szSystemMsg;
                if(GetSystemErrorMsg(szSystemMsg, dwErrorCode, _T("setupapi.dll"))) {
                    //szErrorMsg.Format("SetupDiRegisterDeviceInfo error -- %s", szSystemMsg);
                    //throw std::runtime_error((LPCTSTR)szErrorMsg);
                } else {
                    //szErrorMsg.Format("SetupDiRegisterDeviceInfo error -- 0x%8.8lX", dwErrorCode);
                    //throw std::runtime_error((LPCTSTR)szErrorMsg);
                }
            }


            //================================== Modem Installation specific codes ==============================<BEGIN>==//
            //To-do: Attach the new added Modem to the specified Serial Port via adding the "AttachedTo=COMx" key value in Registry.
            const TCHAR *ptszAttachedTo = TAG_MODEM_CALSS_ATTACHEDTO;

            //(open or create the HKey.)
            HKEY hkeyDevice = ::SetupDiOpenDevRegKey(DeviceInfoSet, &DeviceInfoData, DICS_FLAG_GLOBAL, 0, DIREG_DRV, KEY_ALL_ACCESS);
            if((hkeyDevice == INVALID_HANDLE_VALUE) && (ERROR_KEY_DOES_NOT_EXIST == (dwErrCode = ::GetLastError()))) {
                hkeyDevice = ::SetupDiCreateDevRegKey(DeviceInfoSet, &DeviceInfoData, DICS_FLAG_GLOBAL, 0, DIREG_DRV, 0, NULL);
                if(hkeyDevice == INVALID_HANDLE_VALUE) {
                    DWORD dwErrorCode = ::GetLastError();
                    CString szSystemMsg;
                    if(GetSystemErrorMsg(szSystemMsg, dwErrorCode)) {
                        //szErrorMsg.Format("SetupDiCreateDevRegKey error -- %s", szSystemMsg);
                        //throw std::runtime_error((LPCTSTR)szErrorMsg);
                    } else {
                        //szErrorMsg.Format("SetupDiCreateDevRegKey error -- 0x%8.8lX", dwErrorCode);
                        //throw std::runtime_error((LPCTSTR)szErrorMsg);
                    }
                }
            } else if(hkeyDevice == INVALID_HANDLE_VALUE) {
                CString szSystemMsg;
                if(GetSystemErrorMsg(szSystemMsg, dwErrCode)) {
                    //szErrorMsg.Format("SetupDiOpenDevRegKey error -- %s", szSystemMsg);
                    //throw std::runtime_error((LPCTSTR)szErrorMsg);
                } else {
                    //szErrorMsg.Format("SetupDiOpenDevRegKey error -- 0x%8.8lX", dwErrCode);
                    //throw std::runtime_error((LPCTSTR)szErrorMsg);
                }
            }

            //(write value to the HKey.)
            //(Bug) hr = ::RegSetValueEx(hkeyDevice, ptszAttachedTo, 0, REG_SZ, (CONST BYTE *)(LPCTSTR)szCOMPortID, (DWORD)((szCOMPortID.GetLength() + 1) * 2));
            hr = ::RegSetValueEx(hkeyDevice, ptszAttachedTo, 0, REG_SZ, (PBYTE)ptszCOMPortID, (lstrlen(ptszCOMPortID) + 1) *sizeof(TCHAR));
            if(hr != ERROR_SUCCESS) {
                DWORD dwErrorCode = ::GetLastError();
                CString szSystemMsg;
                if(GetSystemErrorMsg(szSystemMsg, dwErrorCode)) {
                    //szErrorMsg.Format("RegSetValueEx error -- %s", szSystemMsg);
                    //throw std::runtime_error((LPCTSTR)szErrorMsg);
                } else {
                    //szErrorMsg.Format("RegSetValueEx error -- 0x%8.8lX", dwErrorCode);
                    //throw std::runtime_error((LPCTSTR)szErrorMsg);
                }
            }

            //(close the HKey.)
            hr = ::RegCloseKey(hkeyDevice);
            if(hr != ERROR_SUCCESS) {
                DWORD dwErrorCode = ::GetLastError();
                CString szSystemMsg;
                if(GetSystemErrorMsg(szSystemMsg, dwErrorCode)) {
                    //szErrorMsg.Format("RegCloseKey error -- %s", szSystemMsg);
                    //throw std::runtime_error((LPCTSTR)szErrorMsg);
                } else {
                    //szErrorMsg.Format("RegCloseKey error -- 0x%8.8lX", dwErrorCode);
                    //throw std::runtime_error((LPCTSTR)szErrorMsg);
                }
            }
            //================================== Modem Installation specific codes ================================<END>==//
        }

        //To-do-7: If necessary, install the device driver for the specified (by HardwareID) device(s) with the given INF file.
        //(*如果在安裝步驟中途退出,未完成安裝程序時,記得一定要把上面步驟中新建的device node刪除,否則會變成"無法辨識的裝置".*)
        if( ! bIsDeviceNodeExisted || in_bIfForceInstall)
         { //(install it anyway.)
            bRet = ::UpdateDriverForPlugAndPlayDevices(0, (LPCTSTR)szHardwareID, (LPCTSTR)szINFFilePath, INSTALLFLAG_FORCE, NULL);
            if(bRet == FALSE) {
                dwErrCode = ::GetLastError();

                //To-do: Remove the previously created device node (Devnode) from system once the installation failed.
                bRet = ::SetupDiRemoveDevice(DeviceInfoSet, &DeviceInfoData);
                if(bRet == FALSE) {
                    DWORD dwErrorCode = ::GetLastError();
                    CString szSystemMsg;
                    if(GetSystemErrorMsg(szSystemMsg, dwErrorCode, _T("setupapi.dll"))) {
                        //szErrorMsg.Format("SetupDiRemoveDevice error -- %s", szSystemMsg);
                        //throw std::runtime_error((LPCTSTR)szErrorMsg);
                    } else {
                        //szErrorMsg.Format("SetupDiRemoveDevice error -- 0x%8.8lX", dwErrorCode);
                        //throw std::runtime_error((LPCTSTR)szErrorMsg);
                    }
                }

                if(dwErrCode == ERROR_NO_CATALOG_FOR_OEM_INF) {
                    CString szMsg = _T("Installation abort");
                    TRACE("[CGPRSUtil::InstallModemDevice]: %s.\n", szMsg);
                    ///::MessageBox(0, (LPCTSTR)szMsg, AfxGetApp()->m_pszAppName, MB_OK);
                    //throw std::runtime_error((LPCTSTR)szMsg);
                } else {
                    CString szSystemMsg;
                    if(GetSystemErrorMsg(szSystemMsg, dwErrCode, _T("newdev.dll"))) {
                        //szErrorMsg.Format("UpdateDriverForPlugAndPlayDevices error -- %s", szSystemMsg);
                        //throw std::runtime_error((LPCTSTR)szErrorMsg);
                    } else {
                        //szErrorMsg.Format("UpdateDriverForPlugAndPlayDevices error -- 0x%8.8lX", dwErrCode);
                        //throw std::runtime_error((LPCTSTR)szErrorMsg);
                    }
                }
            } else {
                CString szMsg = _T("The device driver has been installed successfully");
                TRACE("[CGPRSUtil::InstallModemDevice]: %s.\n", szMsg);
                ///::MessageBox(0, (LPCTSTR)szMsg, AfxGetApp()->m_pszAppName, MB_OK);
            }
        } else
         { //(install it only when the given driver is better (newer) than the existing one; Windows會先比較<驅動程式日期>,然後再比較<驅動程式版本>來決定.)
            bRet = ::UpdateDriverForPlugAndPlayDevices(0, (LPCTSTR)szHardwareID, (LPCTSTR)szINFFilePath, 0, NULL);
            if(bRet == FALSE) {
                DWORD dwErrorCode = ::GetLastError();
                if(dwErrorCode == ERROR_NO_MORE_ITEMS)
                 { //(ERROR_NO_MORE_ITEMS= 0x00000103)
                    CString szMsg = _T("The given device driver is older than the existing one");
                    TRACE("[CGPRSUtil::InstallModemDevice]: %s.\n", szMsg);
                    ///::MessageBox(0, (LPCTSTR)szMsg, AfxGetApp()->m_pszAppName, MB_OK);
                    //throw std::runtime_error((LPCTSTR)szMsg);
                } else if(dwErrorCode == ERROR_NO_CATALOG_FOR_OEM_INF) {
                    CString szMsg = _T("Installation abort");
                    TRACE("[CGPRSUtil::InstallModemDevice]: %s.\n", szMsg);
                    ///::MessageBox(0, (LPCTSTR)szMsg, AfxGetApp()->m_pszAppName, MB_OK);
                    //throw std::runtime_error((LPCTSTR)szMsg);
                } else
                 { //(error.)
                    CString szSystemMsg;
                    if(GetSystemErrorMsg(szSystemMsg, dwErrorCode, _T("newdev.dll"))) {
                        //szErrorMsg.Format("UpdateDriverForPlugAndPlayDevices error -- %s", szSystemMsg);
                        //throw std::runtime_error((LPCTSTR)szErrorMsg);
                    } else {
                        //szErrorMsg.Format("UpdateDriverForPlugAndPlayDevices error -- 0x%8.8lX", dwErrorCode);
                        //throw std::runtime_error((LPCTSTR)szErrorMsg);
                    }
                }
            } else {
                CString szMsg = _T("The device driver has been updated successfully");
                TRACE("[CGPRSUtil::InstallModemDevice]: %s.\n", szMsg);
                ///::MessageBox(0, (LPCTSTR)szMsg, AfxGetApp()->m_pszAppName, MB_OK);
            }
        }
    } catch(std::exception &e) {
        bIsSuccess = false;
        CStringA strWhat = e.what();
        TRACE("[CGPRSUtil::InstallModemDevice]: %s.\n", e.what());
        ///::MessageBox(0, e.what(), AfxGetApp()->m_pszAppName, MB_OK | MB_ICONERROR);
    } catch(...) {
        bIsSuccess = false;

        TRACE("[CGPRSUtil::InstallModemDevice]: Unrecognized exception.\n");
        ///::MessageBox(0, (LPCTSTR)"Unrecognized exception", AfxGetApp()->m_pszAppName, MB_OK | MB_ICONERROR);
    }


    return bIsSuccess;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------


inline bool CGPRSUtil::UnInstallDevice(const CString in_szHardwareID) {
    bool bIsFound = false, bIsRemoved = false;
    CString szHardwareID = CString(in_szHardwareID); //(*case-insensitive; 預設為小寫英文字母.)
    CString szErrorMsg = _T("");

    try {
        //To-do-1: Generate a Device Information Set of all installed devices on the local machine.
        HDEVINFO DeviceInfoSet = NULL;
        DeviceInfoSet = ::SetupDiGetClassDevs(NULL, NULL, 0, DIGCF_ALLCLASSES | DIGCF_PRESENT);
        if(DeviceInfoSet == INVALID_HANDLE_VALUE) {
            DWORD dwErrorCode = ::GetLastError();
            CString szSystemMsg;
            if(GetSystemErrorMsg(szSystemMsg, dwErrorCode, _T("setupapi.dll"))) {
                //szErrorMsg.Format("SetupDiGetClassDevs error -- %s", szSystemMsg);
                //throw std::runtime_error((LPCTSTR)szErrorMsg);
            } else {
                //szErrorMsg.Format("SetupDiGetClassDevs error -- 0x%8.8lX", dwErrorCode);
                //throw std::runtime_error((LPCTSTR)szErrorMsg);
            }
        }

        //To-do-2: Find the target device by HardwareID through enumerating all devices contained in the Device Information Set.
        DWORD dwErrorCode;
        SP_DEVINFO_DATA DeviceInfoData;
        ZeroMemory(&DeviceInfoData, sizeof(SP_DEVINFO_DATA));
        DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
        for(DWORD i = 0; ::SetupDiEnumDeviceInfo(DeviceInfoSet, i, &DeviceInfoData) && ( ! bIsFound); i++) {
            //To-do: Search by the <HardwareID>.
            DWORD dwRegType;
            LPTSTR ptszBuffer = NULL;
            DWORD dwBufferSize = 0;
            while( ! ::SetupDiGetDeviceRegistryProperty(DeviceInfoSet, &DeviceInfoData, SPDRP_HARDWAREID, &dwRegType, (PBYTE)ptszBuffer, dwBufferSize, &dwBufferSize))
             { //(*使用[動態成長buffer大小]法)
                dwErrorCode = ::GetLastError();
                if(dwErrorCode == ERROR_INSUFFICIENT_BUFFER) {
                    //(use the RequiredSize and try again.)
                    if(ptszBuffer != NULL)
                        ::LocalFree((LPVOID)ptszBuffer);
                    ptszBuffer = (LPTSTR)::LocalAlloc(LPTR, (UINT)dwBufferSize);

                    continue;
                } else {
                    CString szSystemMsg;
                    if(GetSystemErrorMsg(szSystemMsg, dwErrorCode, _T("setupapi.dll"))) {
                        //szErrorMsg.Format("SetupDiGetDeviceRegistryProperty error -- %s", szSystemMsg);
                        //TRACE("[CGPRSUtil::UnInstallDevice]: %s.\n", szErrorMsg);
                    } else {
                        //szErrorMsg.Format("SetupDiGetDeviceRegistryProperty error -- 0x%8.8lX", dwErrorCode);
                        //TRACE("[CGPRSUtil::UnInstallDevice]: %s.\n", szErrorMsg);
                    }

                    break;
                }
            }

            if(dwErrorCode == ERROR_INVALID_DATA)
             { //(*supposed to a device w/o HardwareID.)
                //(Do nothing.)
                ;
            } else {
                //Compare the given HardwareID with each HardwareID contained in the resulting "ptszBuffer (a multi-zero string)".
                LPTSTR ptszTempHardwareID = NULL;
                for(ptszTempHardwareID = ptszBuffer; (*ptszTempHardwareID) && (ptszTempHardwareID <  &ptszBuffer[dwBufferSize]); ptszTempHardwareID += ((_tcslen(ptszTempHardwareID) + 1+1) *sizeof(TCHAR))) {
                    if(_tcscmp(ptszTempHardwareID, (LPCTSTR)in_szHardwareID) == 0)
                     { //(found.)
                        bIsFound = true;

                        //===================== To-do: Try to remove the specified device ===================
                        BOOL bRet = ::SetupDiCallClassInstaller(DIF_REMOVE, DeviceInfoSet, &DeviceInfoData);
                        if(bRet == FALSE) {
                            bIsRemoved = false;

                            dwErrorCode = ::GetLastError();
                            CString szSystemMsg;
                            if(GetSystemErrorMsg(szSystemMsg, dwErrorCode, _T("setupapi.dll"))) {
                                //szErrorMsg.Format("SetupDiCallClassInstaller error -- %s", szSystemMsg);
                                //throw std::runtime_error((LPCTSTR)szErrorMsg);
                            } else {
                                //szErrorMsg.Format("SetupDiCallClassInstaller error -- 0x%8.8lX", dwErrorCode);
                                //throw std::runtime_error((LPCTSTR)szErrorMsg);
                            }
                        } else {
                            bIsRemoved = true;
                        }
                        //===================================================================================

                        break;
                    }
                }
            }

            //(clean up.)
            if(ptszBuffer != NULL)
                ::LocalFree((LPVOID)ptszBuffer);
        }

        dwErrorCode = ::GetLastError();
        //--- free the no longer needed Device Information Set ---
        if( ! ::SetupDiDestroyDeviceInfoList(DeviceInfoSet)) {
            DWORD dwErrCode = ::GetLastError();
            CString szSystemMsg;
            if(GetSystemErrorMsg(szSystemMsg, dwErrCode, _T("setupapi.dll"))) {
                //szErrorMsg.Format("SetupDiDestroyDeviceInfoList error -- %s", szSystemMsg);
                //TRACE("[CGPRSUtil::UnInstallDevice]: %s.\n", szErrorMsg);
            } else {
                //szErrorMsg.Format("SetupDiDestroyDeviceInfoList error -- 0x%8.8lX", dwErrCode);
                //TRACE("[CGPRSUtil::UnInstallDevice]: %s.\n", szErrorMsg);
            }
        }
        //--- free the no longer needed Device Information Set ---

        //To-do: Verify the result.
        if((dwErrorCode != NO_ERROR) && (dwErrorCode != ERROR_NO_MORE_ITEMS))
         { //(error.)
            CString szSystemMsg;
            if(GetSystemErrorMsg(szSystemMsg, dwErrorCode, _T("setupapi.dll"))) {
                //szErrorMsg.Format("SetupDiEnumDeviceInfo error -- %s", szSystemMsg);
                //throw std::runtime_error((LPCTSTR)szErrorMsg);
            } else {
                //szErrorMsg.Format("SetupDiEnumDeviceInfo error -- 0x%8.8lX", dwErrorCode);
                //throw std::runtime_error((LPCTSTR)szErrorMsg);
            }
        } else {
            if( ! bIsFound) {
                //szErrorMsg = _T("The device is not found in the system");
                //throw std::runtime_error((LPCTSTR)szErrorMsg);
            }
        }
    } catch(std::exception &e) {
        CStringA strWaht = e.what();
        TRACE("[CGPRSUtil::UnInstallDevice]: %s.\n", e.what());
        ///::MessageBox(0, e.what(), AfxGetApp()->m_pszAppName, MB_OK | MB_ICONERROR);
    } catch(...) {
        TRACE("[CGPRSUtil::UnInstallDevice]: Unrecognized exception.\n");
        ///::MessageBox(0, (LPCTSTR)"Unrecognized exception", AfxGetApp()->m_pszAppName, MB_OK | MB_ICONERROR);
    }


    return (bIsFound && bIsRemoved);
}


//(*ps. 目前只支援 'REG_SZ'字串型態 的鍵值)
inline bool CGPRSUtil::ChangeDeviceProperty(const CString in_szHardwareID, const CString in_szPropertyName, const CString in_szPropertyValue, DWORD in_dwRegType /*=REG_SZ*/) {
    bool bIsFound = false, bIsModified = false;
    CString szHardwareID = CString(in_szHardwareID); //(*case-insensitive; 預設為小寫英文字母.)
    CString szPropertyName = CString(in_szPropertyName);
    CString szPropertyValue = CString(in_szPropertyValue);
    CString szErrorMsg = _T("");
    HRESULT hr;

    try {
        //To-do-1: Generate a Device Information Set of all installed devices on the local machine.
        HDEVINFO DeviceInfoSet = NULL;
        DeviceInfoSet = ::SetupDiGetClassDevs(NULL, NULL, 0, DIGCF_ALLCLASSES | DIGCF_PRESENT);
        if(DeviceInfoSet == INVALID_HANDLE_VALUE) {
            DWORD dwErrorCode = ::GetLastError();
            CString szSystemMsg;
            if(GetSystemErrorMsg(szSystemMsg, dwErrorCode, _T("setupapi.dll"))) {
                //szErrorMsg.Format("SetupDiGetClassDevs error -- %s", szSystemMsg);
                //throw std::runtime_error((LPCTSTR)szErrorMsg);
            } else {
                //szErrorMsg.Format("SetupDiGetClassDevs error -- 0x%8.8lX", dwErrorCode);
                //throw std::runtime_error((LPCTSTR)szErrorMsg);
            }
        }

        //To-do-2: Find the target device by HardwareID through enumerating all devices contained in the Device Information Set.
        DWORD dwErrorCode;
        SP_DEVINFO_DATA DeviceInfoData;
        ZeroMemory(&DeviceInfoData, sizeof(SP_DEVINFO_DATA));
        DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
        for(DWORD i = 0; ::SetupDiEnumDeviceInfo(DeviceInfoSet, i, &DeviceInfoData) && ( ! bIsFound); i++) {
            //To-do: Search by the <HardwareID>.
            DWORD dwRegType;
            LPTSTR ptszBuffer = NULL;
            DWORD dwBufferSize = 0;
            while( ! ::SetupDiGetDeviceRegistryProperty(DeviceInfoSet, &DeviceInfoData, SPDRP_HARDWAREID, &dwRegType, (PBYTE)ptszBuffer, dwBufferSize, &dwBufferSize))
             { //(*使用[動態成長buffer大小]法)
                dwErrorCode = ::GetLastError();
                if(dwErrorCode == ERROR_INSUFFICIENT_BUFFER) {
                    //(use the RequiredSize and try again.)
                    if(ptszBuffer != NULL)
                        ::LocalFree((LPVOID)ptszBuffer);
                    ptszBuffer = (LPTSTR)::LocalAlloc(LPTR, (UINT)dwBufferSize);

                    continue;
                } else {
                    CString szSystemMsg;
                    if(GetSystemErrorMsg(szSystemMsg, dwErrorCode, _T("setupapi.dll"))) {
                        //szErrorMsg.Format("SetupDiGetDeviceRegistryProperty error -- %s", szSystemMsg);
                        //TRACE("[CGPRSUtil::ChangeDeviceProperty]: %s.\n", szErrorMsg);
                    } else {
                        //szErrorMsg.Format("SetupDiGetDeviceRegistryProperty error -- 0x%8.8lX", dwErrorCode);
                        //TRACE("[CGPRSUtil::ChangeDeviceProperty]: %s.\n", szErrorMsg);
                    }

                    break;
                }
            }

            if(dwErrorCode == ERROR_INVALID_DATA)
             { //(*supposed to a device w/o HardwareID.)
                //(Do nothing.)
                ;
            } else {
                //Compare the given HardwareID with each HardwareID contained in the resulting "ptszBuffer (a multi-zero string)".
                LPTSTR ptszTempHardwareID = NULL;
                for(ptszTempHardwareID = ptszBuffer; (*ptszTempHardwareID) && (ptszTempHardwareID <  &ptszBuffer[dwBufferSize]); ptszTempHardwareID += ((_tcslen(ptszTempHardwareID) + 1+1) /**sizeof(TCHAR)*/)) {
                    if(_tcscmp(ptszTempHardwareID, (LPCTSTR)in_szHardwareID) == 0)
                     { //(found.)
                        bIsFound = true;

                        //======= To-do: Try to modify the vale of the specified property in Registry =======
                        //(open/create the desired HKey.)
                        HKEY hkeyDevice = ::SetupDiOpenDevRegKey(DeviceInfoSet, &DeviceInfoData, DICS_FLAG_GLOBAL, 0, DIREG_DRV, KEY_ALL_ACCESS);
                        if((hkeyDevice == INVALID_HANDLE_VALUE) && (ERROR_KEY_DOES_NOT_EXIST == (dwErrorCode = ::GetLastError()))) {
                            hkeyDevice = ::SetupDiCreateDevRegKey(DeviceInfoSet, &DeviceInfoData, DICS_FLAG_GLOBAL, 0, DIREG_DRV, 0, NULL);
                            if(hkeyDevice == INVALID_HANDLE_VALUE) {
                                bIsModified = false;

                                DWORD dwErrCode = ::GetLastError();
                                CString szSystemMsg;
                                if(GetSystemErrorMsg(szSystemMsg, dwErrCode)) {
                                    //szErrorMsg.Format("SetupDiCreateDevRegKey error -- %s", szSystemMsg);
                                    //throw std::runtime_error((LPCTSTR)szErrorMsg);
                                } else {
                                    //szErrorMsg.Format("SetupDiCreateDevRegKey error -- 0x%8.8lX", dwErrCode);
                                    //throw std::runtime_error((LPCTSTR)szErrorMsg);
                                }
                            }
                        } else if(hkeyDevice == INVALID_HANDLE_VALUE) {
                            bIsModified = false;

                            CString szSystemMsg;
                            if(GetSystemErrorMsg(szSystemMsg, dwErrorCode)) {
                                //szErrorMsg.Format("SetupDiOpenDevRegKey error -- %s", szSystemMsg);
                                //throw std::runtime_error((LPCTSTR)szErrorMsg);
                            } else {
                                //szErrorMsg.Format("SetupDiOpenDevRegKey error -- 0x%8.8lX", dwErrorCode);
                                //throw std::runtime_error((LPCTSTR)szErrorMsg);
                            }
                        }

                        //(write the given value to the HKey.)
                        hr = ::RegSetValueEx(hkeyDevice, (LPCTSTR)szPropertyName, 0, (DWORD)in_dwRegType, (CONST BYTE*)(LPCTSTR)szPropertyValue, (DWORD)((szPropertyValue.GetLength() + 1) *sizeof(TCHAR)));
                        if(hr != ERROR_SUCCESS) {
                            bIsModified = false;

                            DWORD dwErrCode = ::GetLastError();
                            CString szSystemMsg;
                            if(GetSystemErrorMsg(szSystemMsg, dwErrCode)) {
                                //szErrorMsg.Format("RegSetValueEx error -- %s", szSystemMsg);
                                //throw std::runtime_error((LPCTSTR)szErrorMsg);
                            } else {
                                //szErrorMsg.Format("RegSetValueEx error -- 0x%8.8lX", dwErrCode);
                                //throw std::runtime_error((LPCTSTR)szErrorMsg);
                            }
                        }

                        //(close the HKey.)
                        hr = ::RegCloseKey(hkeyDevice);
                        if(hr != ERROR_SUCCESS) {
                            bIsModified = false;

                            DWORD dwErrCode = ::GetLastError();
                            CString szSystemMsg;
                            if(GetSystemErrorMsg(szSystemMsg, dwErrCode)) {
                                //szErrorMsg.Format("RegCloseKey error -- %s", szSystemMsg);
                                //throw std::runtime_error((LPCTSTR)szErrorMsg);
                            } else {
                                //szErrorMsg.Format("RegCloseKey error -- 0x%8.8lX", dwErrCode);
                                //throw std::runtime_error((LPCTSTR)szErrorMsg);
                            }
                        }

                        //(*Up till here, it's supposed to be modified successfully.)
                        bIsModified = true;
                        //===================================================================================

                        break;
                    }
                }
            }

            //(clean up.)
            if(ptszBuffer != NULL)
                ::LocalFree((LPVOID)ptszBuffer);
        }

        dwErrorCode = ::GetLastError();
        //--- free the no longer needed Device Information Set ---
        if( ! ::SetupDiDestroyDeviceInfoList(DeviceInfoSet)) {
            DWORD dwErrCode = ::GetLastError();
            CString szSystemMsg;
            if(GetSystemErrorMsg(szSystemMsg, dwErrCode, _T("setupapi.dll"))) {
                //szErrorMsg.Format("SetupDiDestroyDeviceInfoList error -- %s", szSystemMsg);
                //TRACE("[CGPRSUtil::ChangeDeviceProperty]: %s.\n", szErrorMsg);
            } else {
                //szErrorMsg.Format("SetupDiDestroyDeviceInfoList error -- 0x%8.8lX", dwErrCode);
                //TRACE("[CGPRSUtil::ChangeDeviceProperty]: %s.\n", szErrorMsg);
            }
        }
        //--- free the no longer needed Device Information Set ---

        //To-do: Verify the result.
        if((dwErrorCode != NO_ERROR) && (dwErrorCode != ERROR_NO_MORE_ITEMS))
         { //(error.)
            CString szSystemMsg;
            if(GetSystemErrorMsg(szSystemMsg, dwErrorCode, _T("setupapi.dll"))) {
                //szErrorMsg.Format("SetupDiEnumDeviceInfo error -- %s", szSystemMsg);
                //throw std::runtime_error((LPCTSTR)szErrorMsg);
            } else {
                //szErrorMsg.Format("SetupDiEnumDeviceInfo error -- 0x%8.8lX", dwErrorCode);
                //throw std::runtime_error((LPCTSTR)szErrorMsg);
            }
        } else {
            if( ! bIsFound) {
                //szErrorMsg = _T("The device is not found in the system");
                //throw std::runtime_error((LPCTSTR)szErrorMsg);
            } else if(bIsFound && ( ! bIsModified)) {
                //szErrorMsg = _T("Can not modify the specified property for the device");
                //throw std::runtime_error((LPCTSTR)szErrorMsg);
            }
        }
    } catch(std::exception &e) {
        CStringA strWaht = e.what();
        TRACE("[CGPRSUtil::ChangeDeviceProperty]: %s.\n", e.what());
        ///::MessageBox(0, e.what(), AfxGetApp()->m_pszAppName, MB_OK | MB_ICONERROR);
    } catch(...) {
        TRACE("[CGPRSUtil::ChangeDeviceProperty]: Unrecognized exception.\n");
        ///::MessageBox(0, (LPCTSTR)"Unrecognized exception", AfxGetApp()->m_pszAppName, MB_OK | MB_ICONERROR);
    }


    return (bIsFound && bIsModified);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------


inline bool CGPRSUtil::CreateDialupConnection(const CString in_szConnectionName, const CString in_szDailNumber, const CString in_szModemName, const CString in_szUserName /*=""*/, const CString in_szPassword /*=""*/) {
    bool bIsSuccess = true;
    DWORD dwRet = 0;
    CString szErrorMsg = _T("");
    CString szConnectionName = CString(in_szConnectionName);
    CString szLocalPhoneNumber = CString(in_szDailNumber);
    CString szDeviceName = CString(in_szModemName);
    CString szDeviceType = RASDT_Modem; //(RASDT_Modem= "modem")
    DWORD dwfNetProtocols = RASNP_Ip;
    DWORD dwFramingProtocol = RASFP_Ppp;
    DWORD dwfOptions = (RASEO_IpHeaderCompression | RASEO_SwCompression | RASEO_ModemLights); //(RASEO_ModemLights - "連線後,將圖示顯示在通知區域內")
    DWORD dwType = 1; //(RASET_Phone= '1'; *For WINVER >= 5 only.)
    CString szUserName = CString(in_szUserName);
    CString szPassword = CString(in_szPassword);

    try {
        //(Get the version of the current Windows platform.)
        OSVERSIONINFO OSVersion;
        ZeroMemory(&OSVersion, sizeof(OSVERSIONINFO));
        OSVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if(::GetVersionEx(&OSVersion) == 0)
         { //(failed.)
            DWORD dwErrorCode = ::GetLastError();
            CString szSystemMsg;
            if(GetSystemErrorMsg(szSystemMsg, dwErrorCode)) {
                //szErrorMsg.Format("GetVersionEx error -- %s", szSystemMsg);
                //throw std::runtime_error((LPCTSTR)szErrorMsg);
            } else {
                //szErrorMsg.Format("GetVersionEx error -- 0x%8.8lX", dwErrorCode);
                //throw std::runtime_error((LPCTSTR)szErrorMsg);
            }
        }

        //To-do-1: Validate the given Connection Entry Name.
        dwRet = ::RasValidateEntryName(NULL, (LPCTSTR)szConnectionName); //(*不指定Phonebook file,以向下相容Windows Me/98.)
        if(dwRet != ERROR_SUCCESS) {
            switch(dwRet) {
                case ERROR_CANNOT_FIND_PHONEBOOK:
                     {
                        //szErrorMsg = _T("The specified Phonebook doesn't exist");
                        //throw std::runtime_error((LPCTSTR)szErrorMsg);

                        break;
                    }
                case ERROR_ALREADY_EXISTS:
                     {
                        //szErrorMsg = _T("The name for the new connection already exists");
                        //throw std::runtime_error((LPCTSTR)szErrorMsg);

                        break;
                    }
                case ERROR_INVALID_NAME:
                     {
                        //szErrorMsg = _T("The format of the name for the new connection is invalid");
                        //throw std::runtime_error((LPCTSTR)szErrorMsg);

                        break;
                    }
                default:
                     {
                        CString szSystemMsg;
                        if(GetSystemErrorMsg(szSystemMsg, dwRet)) {
                            //szErrorMsg.Format("RasValidateEntryName error -- %s", szSystemMsg);
                            //throw std::runtime_error((LPCTSTR)szErrorMsg);
                        }
                         else {
                            //szErrorMsg.Format("RasValidateEntryName error -- 0x%8.8lX", dwRet);
                            //throw std::runtime_error((LPCTSTR)szErrorMsg);
                        }

                        break;
                    }
            }
        }

        //To-do-2: Create a new phone-book entry with the given settings.
        //--(create the RAS Entry first.)--
        RASENTRY RasEntry;
        ZeroMemory(&RasEntry, sizeof(RASENTRY));
        RasEntry.dwSize = sizeof(RASENTRY);
        _tcscpy(RasEntry.szLocalPhoneNumber, (LPCTSTR)szLocalPhoneNumber);
        _tcscpy(RasEntry.szDeviceName, (LPCTSTR)szDeviceName);
        _tcscpy(RasEntry.szDeviceType, (LPCTSTR)szDeviceType);
        RasEntry.dwfNetProtocols = dwfNetProtocols;
        RasEntry.dwFramingProtocol = dwFramingProtocol;
        RasEntry.dwfOptions = dwfOptions;
        ///if( (OSVersion.dwPlatformId == VER_PLATFORM_WIN32_NT) && (OSVersion.dwMajorVersion >= 5) )
        ///{
        /// RasEntry.dwType                 =   dwType;
        ///}else;
        //--(create the RAS Entry first.)--

        dwRet = ::RasSetEntryProperties(NULL, (LPCTSTR)szConnectionName, &RasEntry, sizeof(RASENTRY), NULL, 0); //(*For Windows98/Me, you may check "COMMPROP.dwProvSubType == PST_MODEM (szDeviceType是不是真的是Modem)?" again via the returned "lpbDeviceInfo".)
        if(dwRet != 0)
         { //(failed.)
            switch(dwRet) {
                case ERROR_BUFFER_INVALID:
                     {
                        //szErrorMsg = _T("The pointer to the specified Ras Entry is invalid");
                        //throw std::runtime_error((LPCTSTR)szErrorMsg);

                        break;
                    }
                case ERROR_INVALID_PARAMETER:
                     {
                        //szErrorMsg = _T("The specified Ras Entry does not contain adequate parameters");
                        //throw std::runtime_error((LPCTSTR)szErrorMsg);

                        break;
                    }
                case ERROR_CANNOT_OPEN_PHONEBOOK:
                     {
                        //szErrorMsg = _T("The Phonebook is corrupted or missing components");
                        //throw std::runtime_error((LPCTSTR)szErrorMsg);

                        break;
                    }
                default:
                     {
                        CString szSystemMsg;
                        if(GetSystemErrorMsg(szSystemMsg, dwRet)) {
                            //szErrorMsg.Format("RasSetEntryProperties error -- %s", szSystemMsg);
                            //throw std::runtime_error((LPCTSTR)szErrorMsg);
                        }
                         else {
                            //szErrorMsg.Format("RasSetEntryProperties error -- 0x%8.8lX", dwRet);
                            //throw std::runtime_error((LPCTSTR)szErrorMsg);
                        }

                        break;
                    }
            }
        }

        //To-do-3: Set up the related connection(dial) parameters of the new created phone-book entry.
        //--(create the RAS Dial Parameters first.)--
        RASDIALPARAMS RasDailParams;
        ZeroMemory(&RasDailParams, sizeof(RASDIALPARAMS));
        RasDailParams.dwSize = sizeof(RASDIALPARAMS);
        _tcscpy(RasDailParams.szEntryName, (LPCTSTR)szConnectionName);
        _tcscpy(RasDailParams.szUserName, (LPCTSTR)szUserName);
        _tcscpy(RasDailParams.szPassword, (LPCTSTR)szPassword);
        //--(create the RAS Dial Parameters first.)--

        dwRet = ::RasSetEntryDialParams(NULL, &RasDailParams, FALSE); //(For Windows XP or later, it's suggested to use the RasSetCredentials() API instead.)
        if(dwRet != 0)
         { //(failed.)
            switch(dwRet) {
                case ERROR_BUFFER_INVALID:
                     {
                        //szErrorMsg = _T("The pointer to the specified Ras Dail Params is invalid");
                        //throw std::runtime_error((LPCTSTR)szErrorMsg);

                        break;
                    }
                case ERROR_CANNOT_OPEN_PHONEBOOK:
                     {
                        //szErrorMsg = _T("The Phonebook is corrupted or missing components");
                        //throw std::runtime_error((LPCTSTR)szErrorMsg);

                        break;
                    }
                case ERROR_CANNOT_FIND_PHONEBOOK_ENTRY:
                     {
                        //szErrorMsg = _T("The specified Phonebook Entries are not found");
                        //throw std::runtime_error((LPCTSTR)szErrorMsg);

                        break;
                    }
                default:
                     {
                        CString szSystemMsg;
                        if(GetSystemErrorMsg(szSystemMsg, dwRet)) {
                            //szErrorMsg.Format("RasSetEntryDialParams error -- %s", szSystemMsg);
                            //throw std::runtime_error((LPCTSTR)szErrorMsg);
                        }
                         else {
                            //szErrorMsg.Format("RasSetEntryDialParams error -- 0x%8.8lX", dwRet);
                            //throw std::runtime_error((LPCTSTR)szErrorMsg);
                        }

                        break;
                    }
            }
        }
    } catch(std::exception &e) {
        bIsSuccess = false;
        CStringA strWaht = e.what();
        TRACE("[CGPRSUtil::CreateDialupConnection]: %s.\n", e.what());
        ///::MessageBox(0, e.what(), AfxGetApp()->m_pszAppName, MB_OK | MB_ICONERROR);
    } catch(...) {
        bIsSuccess = false;

        TRACE("[CGPRSUtil::CreateDialupConnection]: Unrecognized exception.\n");
        ///::MessageBox(0, (LPCTSTR)"Unrecognized exception", AfxGetApp()->m_pszAppName, MB_OK | MB_ICONERROR);
    }


    return bIsSuccess;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------


inline bool CGPRSUtil::DeleteDialupConnection(const CString in_szConnectionName) {
    bool bIsSuccess = true;
    CString szEntryName = CString(in_szConnectionName);
    DWORD dwRet = 0;
    CString szErrorMsg = _T("");

    try {
        //To-do: Delete the specified phone-book entry.
        dwRet = ::RasDeleteEntry(NULL, (LPCTSTR)szEntryName);
        if(dwRet != ERROR_SUCCESS) {
            CString szSystemMsg;
            if(GetSystemErrorMsg(szSystemMsg, dwRet)) {
                //szErrorMsg.Format("RasDeleteEntry error -- %s", szSystemMsg);
                //throw std::runtime_error((LPCTSTR)szErrorMsg);
            } else {
                //szErrorMsg.Format("RasDeleteEntry error -- 0x%8.8lX", dwRet);
                //throw std::runtime_error((LPCTSTR)szErrorMsg);
            }
        }
    } catch(std::exception &e) {
        bIsSuccess = false;
        CStringA strWaht = e.what();
        TRACE("[CGPRSUtil::DeleteDialupConnection]: %s.\n", e.what());
        ///::MessageBox(0, e.what(), AfxGetApp()->m_pszAppName, MB_OK | MB_ICONERROR);
    } catch(...) {
        bIsSuccess = false;

        TRACE("[CGPRSUtil::DeleteDialupConnection]: Unrecognized exception.\n");
        ///::MessageBox(0, (LPCTSTR)"Unrecognized exception", AfxGetApp()->m_pszAppName, MB_OK | MB_ICONERROR);
    }
    return bIsSuccess;
}

#endif // __GPRSUTIL_H__
