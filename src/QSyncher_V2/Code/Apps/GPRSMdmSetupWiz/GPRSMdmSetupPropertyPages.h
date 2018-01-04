/* *** THIS LINE MUST BE THE FIRST LINE AND DO NOT MODIFY OR REMOVE IT *** */
/***************************************************************************
*
*               BenQ QSyncher Project
*
*	   Copyright (C) 2005 BenQ Corp.
*
***************************************************************************
*
*  $Workfile:   GPRSMdmSetupPropertyPages.h  $
*  $Revision:   1.6  $
*  $Date:   Dec 10 2007 11:45:56  $
*
***************************************************************************
*/

#ifndef __GPRSMDMSETUPPROPERTYPAGES_H__
#define __GPRSMDMSETUPPROPERTYPAGES_H__

#include "GPRSMdmSetupPropertyPage1.h"
#include "GPRSMdmSetupPropertyPage2.h"
#include "GPRSMdmSetupPropertyPage3.h"
#include "GPRSMdmSetupPropertyPage4.h"
#include "GPRSMdmSetupPropertyPage5.h"
#include "..\..\CommonUtil\Win32ThreadUtility.h"
#include "GPRSMdmSetupWiz.h"
#include <ras.h>
#include <stdexcept>

#pragma once
#pragma comment(lib, "rasapi32")

extern	CGPRSMdmSetupWizApp theApp;

using std::runtime_error;

typedef struct tagDEVCFGDR {
    DWORD		dwSize;
    DWORD		dwVersion;
    WORD		fwOptions;
    WORD		wWaitBong;
} DEVCFGHDR;

typedef struct tagDEVCFG {
    DEVCFGHDR	dfgHdr;
    COMMCONFIG	commconfig;
} DEVCFG, *PDEVCFG, FAR* LPDEVCFG;

class CIniReader {
public:
    CIniReader(TCHAR *szFileName) {
        memset(m_szFileName, 0, MAX_PATH+1);
        memcpy(m_szFileName, szFileName, _tcslen(szFileName)*sizeof(TCHAR));
    }
    int ReadInteger(TCHAR *szSection, TCHAR *szKey, int iDefaultValue) {
        int iResult = GetPrivateProfileInt(szSection, szKey, iDefaultValue, m_szFileName);
        return iResult;
    }
    float ReadFloat(TCHAR *szSection, TCHAR *szKey, float fltDefaultValue) {
        TCHAR szResult[255] = {0};
        TCHAR szDefault[255] = {0};
        float fltResult;
        _stprintf(szDefault, _T("%f"), fltDefaultValue);
        GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName);
        fltResult = (float)_ttoi(szResult);
        return fltResult;
    }
    bool ReadBoolean(TCHAR *szSection, TCHAR *szKey, bool bolDefaultValue) {
        TCHAR szResult[255];
        TCHAR szDefault[255];
        bool bolResult;
        _stprintf(szDefault, _T("%s"), bolDefaultValue ? _T("True") : _T("False"));
        GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName);
        bolResult = (_tcscmp(szResult, _T("True")) == 0 || _tcscmp(szResult, _T("true")) == 0) ? true : false;
        return bolResult;
    }
    CString ReadString(TCHAR *szSection, TCHAR *szKey, const TCHAR *szDefaultValue) {
        TCHAR szResult[255] = {0};
        GetPrivateProfileString(szSection, szKey, szDefaultValue, szResult, 255, m_szFileName);
        return CString(szResult);
    }
private:
    TCHAR m_szFileName[MAX_PATH+1];
};


#define				LOADSTRING(id)								(CString(LPCTSTR(id)))

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

#ifndef				RASEO_PreviewUserPw
#define				RASEO_PreviewUserPw						0x01000000
#endif

#ifndef				RASEO_ShowDialingProgress
#define				RASEO_ShowDialingProgress				0x04000000
#endif

static bool GetDir(CString & out_szDirPath, const HMODULE in_hModule = NULL)
{
    CString szTemp;
    TCHAR ptszFullPath[MAX_PATH] = {0};

    if( ::GetModuleFileName(in_hModule, ptszFullPath, sizeof(ptszFullPath)) == 0 )
    {
        return false;
    }
    else
    {
        szTemp = CString((LPCTSTR)ptszFullPath);
        if( szTemp.IsEmpty() )
        {
            return false;
        }
        else
        {
            int nDelimiterPos = -1;
            for(int i = szTemp.GetLength()-1; i >= 0; i--)
            {
                if( szTemp.GetAt(i) == '\\' )
                {
                    nDelimiterPos = i;
                    break;
                }else;
            }

            if(nDelimiterPos >= 0)  //(the Delimiter found.)
            {
                out_szDirPath = szTemp.Mid(0, nDelimiterPos);

                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

static bool GetErrorMsg(CString & out_szErrorMsg, DWORD in_nErrorCode, CString in_szModuleName = _T(""))
{
    LPTSTR ptszMsg = NULL;


    //To-do: Retrieve the pre-defined Error Message from the specified DLL module or form System.
    if( in_szModuleName.IsEmpty() )
    {
        if( ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, in_nErrorCode, MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), (LPTSTR)&ptszMsg, 0, NULL) )
        {
            out_szErrorMsg = CString((LPCTSTR)ptszMsg);
        }else;
    }
    else
    {
        if( ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE, ::GetModuleHandle((LPCTSTR)in_szModuleName), in_nErrorCode, MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), (LPTSTR)&ptszMsg, 0, NULL) )
        {
            out_szErrorMsg = CString((LPCTSTR)ptszMsg);
        }else;
    }

    //(clean up.)
    if(ptszMsg != NULL)
        ::LocalFree((LPVOID)ptszMsg);

    //To-do: Verify the result.
    if( out_szErrorMsg.IsEmpty() )
        return false;
    else
        return true;
}

static bool SetModemDevConfigure(CString strConnName, bool bHwFlowControl, bool bProtocol, bool bCompression)
{
    const   CString PBK_Path = _T("\\Documents and Settings\\All Users\\Application Data\\Microsoft\\Network\\Connections\\Pbk\\");
    const   CString RASPhone_Fname = _T("rasphone.pbk");

    TCHAR    WinDir[32] = {0}; // buffer for Windows directory
    CString Win_Path,SysDrive,RAS_PBK_Path;

    GetWindowsDirectory(WinDir,32);
    Win_Path.Format(_T("%s"),WinDir);
    SysDrive = Win_Path.Left(Win_Path.Find(_T(":"))+1);

    RAS_PBK_Path.Format(_T("%s%s%s"),SysDrive,PBK_Path,RASPhone_Fname);

    WritePrivateProfileString(strConnName, _T("HwFlowControl"), (bHwFlowControl?_T("1"):_T("0")), RAS_PBK_Path);
    WritePrivateProfileString(strConnName, _T("Protocol"), (bProtocol?_T("1"):_T("0")), RAS_PBK_Path);
    WritePrivateProfileString(strConnName, _T("Compression"), (bCompression?_T("1"):_T("0")), RAS_PBK_Path);

    return true;
}

static bool CreateDialupSettings(const CString in_szConnectionName, const CString in_szDailNumber, const CString in_szModemName, const CString in_szUserName = _T(""), const CString in_szPassword = _T(""))
{
    bool bIsSuccess = true;
    DWORD dwRet = 0;
    CString szErrorMsg = _T("");
    CString szConnectionName	= in_szConnectionName;
    CString szLocalPhoneNumber	= in_szDailNumber;
    CString szDeviceName		= in_szModemName;
    CString szDeviceType		= RASDT_Modem;  //(RASDT_Modem = "modem")
    DWORD dwfNetProtocols		= RASNP_Ip;
    DWORD dwFramingProtocol		= RASFP_Ppp;
    DWORD dwfOptions			= (RASEO_RemoteDefaultGateway | RASEO_IpHeaderCompression | RASEO_SwCompression | RASEO_ModemLights | RASEO_PreviewUserPw | RASEO_ShowDialingProgress);  //(RASEO_ModemLights - "連線後,將圖示顯示在通知區域內"; RASEO_PreviewUserPw(Win NT/Me/98/95不支援) - "提示名稱、密碼、憑證等")
    DWORD dwType				= 1;  //(RASET_Phone= '1'; *For WINVER >= 5 only.)
    CString szUserName			= CString(in_szUserName);
    CString szPassword			= CString(in_szPassword);
    try
    {
        //(Get the OS version of the current Windows platform.)
        OSVERSIONINFO ovi;
        ZeroMemory(&ovi, sizeof(OSVERSIONINFO));
        ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if(::GetVersionEx(&ovi) == 0)  //(failed.)
        {
            DWORD dwErrorCode = ::GetLastError();
            CString szSystemMsg;
            if( GetErrorMsg(szSystemMsg, dwErrorCode) )
            {
                szErrorMsg.Format(_T("GetVersionEx error - %s"), szSystemMsg);
                throw ;//std::runtime_error((LPCTSTR)szErrorMsg);
            }
            else
            {
                szErrorMsg.Format(_T("GetVersionEx error - 0x%8.8lX"), dwErrorCode);
                throw ;//std::runtime_error((LPCTSTR)szErrorMsg);
            }
        }


        //To-do-1: Validate the given Connection Entry Name.
        dwRet = ::RasValidateEntryName(NULL, (LPCTSTR)szConnectionName);  //(*不指定Phonebook file,以向下相容Windows 98/Me.)
        if(dwRet != ERROR_SUCCESS)
        {
            switch( dwRet )
            {
            case ERROR_CANNOT_FIND_PHONEBOOK:
                szErrorMsg = _T("The specified Phonebook doesn't exist");
                throw ;//std::runtime_error((LPCTSTR)szErrorMsg);

                break;
            case ERROR_ALREADY_EXISTS:
                szErrorMsg = _T("The name for the new connection already exists");
                throw ;//std::runtime_error((LPCTSTR)szErrorMsg);

                break;
            case ERROR_INVALID_NAME:
                szErrorMsg = _T("The format of the name for the new connection is invalid");
                throw ;//std::runtime_error((LPCTSTR)szErrorMsg);

                break;
            default:
                CString szSystemMsg;
                if( GetErrorMsg(szSystemMsg, dwRet) )
                {
                    szErrorMsg.Format(_T("RasValidateEntryName error - %s"), szSystemMsg);
                    throw ;//std::runtime_error((LPCTSTR)szErrorMsg);
                }
                else
                {
                    szErrorMsg.Format(_T("RasValidateEntryName error - 0x%8.8lX"), dwRet);
                    throw ;//std::runtime_error((LPCTSTR)szErrorMsg);
                }

                break;
            }
        }


        //To-do-2: Create a new phone-book entry with the given settings.
        RASENTRY RasEntry;
        //[Note]: The RASENTRY structure has different size, in WinXP is 2884 bytes, in Win2000 is 2088 bytes, in Win95/98/NT4 is 1768 bytes, and in WinNT3.51 is 1672 bytes.
        /*?
        DWORD dwEntrySize = 0;
        ::RasGetEntryProperties(NULL, "", NULL, &dwEntrySize, NULL, NULL);
        if(dwEntrySize == 0)
        {
        szErrorMsg.Format("The RasGetEntryProperties API returns an 0-size RasEntry");
        throw std::runtime_error((LPCTSTR)szErrorMsg);
        }
        else
        {
        ZeroMemory(&RasEntry, dwEntrySize);
        RasEntry.dwSize = dwEntrySize;
        }
        ?*/
        ZeroMemory(&RasEntry, sizeof(RASENTRY));
        RasEntry.dwSize = sizeof(RASENTRY);
        _tcscpy(RasEntry.szLocalPhoneNumber	,	(LPCTSTR)szLocalPhoneNumber);
        _tcscpy(RasEntry.szDeviceName		,	(LPCTSTR)szDeviceName);
        _tcscpy(RasEntry.szDeviceType		,	(LPCTSTR)szDeviceType);
        RasEntry.dwfNetProtocols			=	dwfNetProtocols;
        RasEntry.dwFramingProtocol			=	dwFramingProtocol;
        RasEntry.dwfOptions					=	dwfOptions;

        //? dwRet = ::RasSetEntryProperties(NULL, (LPCTSTR)szConnectionName, &RasEntry, dwEntrySize, NULL, 0);
        dwRet = ::RasSetEntryProperties(NULL, (LPCTSTR)szConnectionName, &RasEntry, sizeof(RASENTRY), NULL, 0);  //(*For Windows98/Me, you may check "COMMPROP.dwProvSubType == PST_MODEM (szDeviceType是不是真的是Modem)?" again via the returned "lpbDeviceInfo".)
        if(dwRet != 0)  //(failed.)
        {
            switch( dwRet )
            {
            case ERROR_BUFFER_INVALID:
                szErrorMsg = _T("The pointer to the specified Ras Entry is invalid");
                throw ;//std::runtime_error((LPCTSTR)szErrorMsg);

                break;
            case ERROR_INVALID_PARAMETER:
                szErrorMsg = _T("The specified Ras Entry does not contain adequate parameters");
                throw ;//std::runtime_error((LPCTSTR)szErrorMsg);

                break;
            case ERROR_CANNOT_OPEN_PHONEBOOK:
                szErrorMsg = _T("The Phonebook is corrupted or missing components");
                throw ;//std::runtime_error((LPCTSTR)szErrorMsg);

                break;
            default:
                CString szSystemMsg;
                if( GetErrorMsg(szSystemMsg, dwRet) )
                {
                    ;//szErrorMsg.Format("RasSetEntryProperties error -- %s", szSystemMsg);
                    throw ;//std::runtime_error((LPCTSTR)szErrorMsg);
                }
                else
                {
                    ;//szErrorMsg.Format("RasSetEntryProperties error -- 0x%8.8lX", dwRet);
                    throw ;//std::runtime_error((LPCTSTR)szErrorMsg);
                }

                break;
            }
        }


        //To-do-3: Set up the related connection(dial) parameters of the new created phone-book entry.
        RASDIALPARAMS RasDailParams;
        ZeroMemory(&RasDailParams, sizeof(RASDIALPARAMS));
        RasDailParams.dwSize = sizeof(RASDIALPARAMS);
        _tcscpy(RasDailParams.szEntryName		,	(LPCTSTR)szConnectionName);
        _tcscpy(RasDailParams.szUserName		,	(LPCTSTR)szUserName);
        _tcscpy(RasDailParams.szPassword		,	(LPCTSTR)szPassword);

        dwRet = ::RasSetEntryDialParams(NULL, &RasDailParams, FALSE);  //(For Windows XP or later, it's suggested to use the RasSetCredentials() API instead.)
        if(dwRet != 0)  //(failed.)
        {
            switch( dwRet )
            {
            case ERROR_BUFFER_INVALID:
                szErrorMsg = _T("The pointer to the specified Ras Dail Params is invalid");
                throw ;//std::runtime_error((LPCTSTR)szErrorMsg);

                break;
            case ERROR_CANNOT_OPEN_PHONEBOOK:
                szErrorMsg = _T("The Phonebook is corrupted or missing components");
                throw ;//std::runtime_error((LPCTSTR)szErrorMsg);

                break;
            case ERROR_CANNOT_FIND_PHONEBOOK_ENTRY:
                szErrorMsg = _T("The specified Phonebook Entries are not found");
                throw ;//std::runtime_error((LPCTSTR)szErrorMsg);

                break;
            default:
                CString szSystemMsg;
                if( GetErrorMsg(szSystemMsg, dwRet) )
                {
                    //szErrorMsg.Format("RasSetEntryDialParams error - %s", szSystemMsg);
                    throw ;//std::runtime_error((LPCTSTR)szErrorMsg);
                }
                else
                {
                    //szErrorMsg.Format("RasSetEntryDialParams error - 0x%8.8lX", dwRet);
                    throw ;//std::runtime_error((LPCTSTR)szErrorMsg);
                }

                break;
            }
        }


        //(*To-do-4: Set up the Flow Control.)
        if( ((ovi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && (ovi.dwMinorVersion == 0))		//(Windows 95)
            || 
            ((ovi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && (ovi.dwMinorVersion == 10))	//(Windows 98)
            )
        {
            PDEVCFG pDevCfg;
            DWORD dwEntrySize = sizeof(RASENTRY), dwDevInfo = 0;

            //To get the current Entry Size :
            dwRet = ::RasGetEntryProperties(NULL, (LPCTSTR)szConnectionName, &RasEntry, &dwEntrySize, NULL, &dwDevInfo);
            if(dwRet != 0)  //(failed.)
            {
                CString szSystemMsg;
                if( GetErrorMsg(szSystemMsg, dwRet) )
                {
                    //szErrorMsg.Format("RasGetEntryProperties error - %s", szSystemMsg);
                    throw ;//std::runtime_error((LPCTSTR)szErrorMsg);
                }
                else
                {
                    //szErrorMsg.Format("RasGetEntryProperties error - 0x%8.8lX", dwRet);
                    throw ;//std::runtime_error((LPCTSTR)szErrorMsg);
                }
            }

            if( !(pDevCfg = (LPDEVCFG)LocalAlloc(LPTR,dwDevInfo)) )
            {
                //szErrorMsg.Format("LocalAlloc failure - 0x%8.8lX", GetLastError());
                throw ;//std::runtime_error((LPCTSTR)szErrorMsg);
            }

            //To get the Entry for modification :
            dwRet = ::RasGetEntryProperties(NULL, (LPCTSTR)szConnectionName, &RasEntry, &dwEntrySize, (LPBYTE)pDevCfg, &dwDevInfo);
            if(dwRet != 0)  //(failed.)
            {
                CString szSystemMsg;
                if( GetErrorMsg(szSystemMsg, dwRet) )
                {
                    ;//szErrorMsg.Format("RasGetEntryProperties error - %s", szSystemMsg);
                    throw ;//std::runtime_error((LPCTSTR)szErrorMsg);
                }
                else
                {
                    ;//szErrorMsg.Format("RasGetEntryProperties error - 0x%8.8lX", dwRet);
                    throw ;//std::runtime_error((LPCTSTR)szErrorMsg);
                }
            }

            //To verify the retrieved Entry first :
            if( ((pDevCfg->dfgHdr).dwSize < sizeof(DEVCFG)) || 
                ((pDevCfg->commconfig).dwProviderSubType != PST_MODEM) )
            {
                ;//szErrorMsg.Format("Invalid RASENTRY type");
                throw ;//std::runtime_error((LPCTSTR)szErrorMsg);
            }

            //To configure the designated properties: (turn off all Flow Control.)
            (pDevCfg->commconfig).dcb.fOutxCtsFlow			= FALSE;
            (pDevCfg->commconfig).dcb.fOutxDsrFlow			= FALSE;
            (pDevCfg->commconfig).dcb.fDsrSensitivity		= FALSE;
            (pDevCfg->commconfig).dcb.fDtrControl			= DTR_CONTROL_DISABLE;
            (pDevCfg->commconfig).dcb.fRtsControl			= RTS_CONTROL_DISABLE;
            (pDevCfg->commconfig).dcb.fOutX					= FALSE;
            (pDevCfg->commconfig).dcb.fInX					= FALSE;
            (pDevCfg->commconfig).dcb.fTXContinueOnXoff		= FALSE;

            dwRet = ::RasSetEntryProperties(NULL, (LPCTSTR)szConnectionName, &RasEntry, dwEntrySize, (LPBYTE)pDevCfg, dwDevInfo);
            if(dwRet != 0)  //(failed.)
            {
                CString szSystemMsg;
                if( GetErrorMsg(szSystemMsg, dwRet) )
                {
                    ;//szErrorMsg.Format("RasSetEntryProperties error - %s", szSystemMsg);
                    throw ;//std::runtime_error((LPCTSTR)szErrorMsg);
                }
                else
                {
                    ;//szErrorMsg.Format("RasSetEntryProperties error - 0x%8.8lX", dwRet);
                    throw ;//std::runtime_error((LPCTSTR)szErrorMsg);
                }
            }
        }
        else
        {
            SetModemDevConfigure(szConnectionName,false,false,false);  //Add by Saffin to set the hardware flow-contro off.
        }
    }
    catch(std::exception &e)
    {
        bIsSuccess = false;
        CStringA strWaht = e.what();
        TRACE("[CreateDialupSettings]: %s.\n", e.what());
    }
    catch(...)
    {
        bIsSuccess = false;
        TRACE("[CreateDialupSettings]: Unrecognized exception.\n");
    }
    return bIsSuccess;
}

static bool DeleteDialupSettings(const CString in_szConnectionName)
{
    bool bIsSuccess = true;
    DWORD dwRet = 0;
    CString szErrorMsg = _T("");
    CString szEntryName = CString(in_szConnectionName);



    try
    {
        //To-do: Delete the specified phone-book entry.
        dwRet = ::RasDeleteEntry(NULL, (LPCTSTR)szEntryName);
        if(dwRet != ERROR_SUCCESS)
        {
            CString szSystemMsg;
            if( GetErrorMsg(szSystemMsg, dwRet) )
            {
                ;//szErrorMsg.Format("RasDeleteEntry error - %s", szSystemMsg);
                throw ;//std::runtime_error((LPCTSTR)szErrorMsg);
            }
            else
            {
                // if there is no dial up setting, this exception will be thrown
                ;//szErrorMsg.Format("RasDeleteEntry error - 0x%8.8lX", dwRet);
                //throw ;//std::runtime_error((LPCTSTR)szErrorMsg);
            }
        }
    }
    catch(std::exception &e)
    {
        bIsSuccess = false;
        CStringA strWaht = e.what();
        TRACE("[::DeleteDialupSettings]: %s.\n", e.what());
    }
    catch(...)
    {
        bIsSuccess = false;
        TRACE("[::DeleteDialupSettings]: Unrecognized exception.\n");
    }


    return bIsSuccess;
}

static CString GetHardwareDeviceID(LPCTSTR szIniFileName) {
    CString szDirPath;
    GetDir(szDirPath);
    CString szHardwareINIFile = CString(szDirPath) + CString(_T("\\ModemSetupSetting\\")) + CString(szIniFileName);
    CIniReader reader((LPTSTR)(LPCTSTR)szHardwareINIFile);
    CString szModemHardwareID = reader.ReadString(_T("Hardware.ID"), (LPTSTR)(LPCTSTR)theApp.m_szModelName, _T(""));
    return szModemHardwareID;
}

#endif // __GPRSMDMSETUPPROPERTYPAGES_H__
