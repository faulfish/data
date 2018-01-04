#include "StdAfx.h"
#include "DownloadFunc.h"
#include "..\..\..\Code\odmtma_api\QDownload_Export\Download_Export.h"
#include "..\..\..\code\odmtma_api\\DownloadDLL\DownloadDll.h"
#include "..\..\..\Code\Modules\QisdaMiscellaneousModule\QisdaMiscellaneousModule.h"
#include "..\..\..\Code\Modules\QisdaNVModule\NVItem.h"
#include "..\..\..\code\FactoryTool\MobileToolKit\MobileToolKit.h"
#include "..\..\..\Code\odmtma_api\QFlash\QZip.h"
#include "..\..\..\Code\odmtma_api\QFlash\QVersionParser.h"
#include "..\..\..\code\odmtma_api\1FFMaker\C1FFMaker.h"
#include "..\..\..\Code\CommonUtil\LogPolicy.h"

namespace QSYNC_LIB {

	bool CDownloadFuncForAll::m_bSigned = false;
	int	CDownloadFuncForAll::m_nAmssVersion = 0;
	int CDownloadFuncForAll::m_nOemsblVersion = 0;
	CStringArray CDownloadFuncForAll::m_strCheckLoadInfoArray;
	CDownloadFuncForAll* g_pThis = NULL;

	CDownloadFuncForAll::CDownloadFuncForAll(UINT unComport, UINT unButtonIndex,
        CString iniPath, HWND hUIWnd, WIN32IconButton *pButton, Win32ProgressBarWidge* pProgress) {
		m_unComport = unComport;
		m_unButtonIndex = unButtonIndex;
		m_pButtonWnd = pButton;
		m_pProgressWnd = pProgress;
		m_strIni =  iniPath;
		m_str1ff = _T("");
		m_hUIWnd = hUIWnd;
		m_DownloadInfoIndex = 0;
		m_pCallBackFunc = NULL;
		m_strFolderPath = _T("");
        m_bPartitionDifferent = false;
        g_pThis = this;
	}

	CDownloadFuncForAll::~CDownloadFuncForAll() { }

    void CDownloadFuncForAll::SetDownload6240Information(CShareDownload6240* download) {
        static SoftwareLoad6240 load(m_strIni);
        OpenImageFile(&load);
        download->SetAMSS(load.m_imgAMSS.pData, load.m_imgAMSS.nSize);
        download->SetHex(load.m_imgHEX.pData, load.m_imgHEX.nSize);
        download->SetPartition(load.m_imgPartition.pData, load.m_imgPartition.nSize);
        download->SetFSBL(load.m_imgFSBL.pData, load.m_imgFSBL.nSize);
        download->SetDBL(load.m_imgDBL.pData, load.m_imgDBL.nSize);
        download->SetOSBL(load.m_imgOSBL.pData, load.m_imgOSBL.nSize);
        download->SetCEFS(load.m_imgCEFS.pData, load.m_imgCEFS.nSize);
    }

    void CDownloadFuncForAll::SetDownload6245Information(CShareDownload6245* download) {
        static SoftwareLoad6245 load(m_strIni);
        OpenImageFile(&load);
        download->SetAMSSHD(load.m_imgAMSSHD.pData, load.m_imgAMSSHD.nSize);
        download->SetAMSS(load.m_imgAMSS.pData, load.m_imgAMSS.nSize);
        download->SetHex(load.m_imgHEX.pData, load.m_imgHEX.nSize);
        download->SetCEFS(load.m_imgCEFS.pData, load.m_imgCEFS.nSize);
        download->SetPartition(load.m_imgPartition.pData, load.m_imgPartition.nSize);
    }

    bool CDownloadFuncForAll::Download(CDLFormatCommand* download) {
        download->Register(this, EVENT_DL_PROGRESS);
        download->Register(this, EVENT_DL_PARTITION_DIF);
        download->SetResetMode(false);
        bool bContinueDownload = download->Download(true);
        if(bContinueDownload == false && m_bPartitionDifferent) {
            download->SetIsOveridePartition(true);
            download->SetResetMode(true);
            bContinueDownload = download->StreamingDLFromPartition(true);
        }
        download->Unregister(this, EVENT_DL_PROGRESS);
        download->Unregister(this, EVENT_DL_PARTITION_DIF);
        return bContinueDownload;
    }

    bool CDownloadFuncForAll::NewDownloadStage() {
        bool bDownloadSuccess = false;
        switch (GetPlatform(m_str1ff)) {
        case PLATFORM_6240:
        case PLATFORM_6270: {
                CShareDownload6240 download(m_unComport);
                SetDownload6240Information(&download);
                bDownloadSuccess = Download(&download);
                break;
            }
        case PLATFORM_6245: {
                CShareDownload6245 download(m_unComport);
                SetDownload6245Information(&download);
                bDownloadSuccess = Download(&download);
                break;
            }
        case PLATFORM_UKNOWN:
        default:
            ASSERT(0);
            break;
        }
        return bDownloadSuccess;
    }

    void CDownloadFuncForAll::Event(const std::string& strEvent,long nParam) {
        if(strEvent == EVENT_DL_PROGRESS) {
            int nPrecentage = ((DL_PROGRESS*)nParam)->nProgress;
            CStringA strPercent;
            strPercent.Format("%s",((DL_PROGRESS*)nParam)->szMsg, nPrecentage);
            DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, strPercent.GetString(), -1, NULL, 0);
            wchar_t *pwText = new wchar_t[dwNum];
            MultiByteToWideChar(CP_ACP, 0, strPercent.GetString(), -1, pwText, dwNum);
            SetDLTaskInfo(pwText, -1, nPrecentage);
        } else if(strEvent == EVENT_DL_PARTITION_DIF) {
            m_bPartitionDifferent = true;
        }
    }


    bool CDownloadFuncForAll::WarnInDownloadMode(bool bInDownloadMode) {
        if(bInDownloadMode == false) return true;
        CString message;
        message = m_bSigned ? 
            _T("The handset is in DL or Emergency DL mode.\nThe software load is SIGNED.\nAre you sure continuing to download?") :
            _T("The handset is in DL or Emergency DL mode.\nThe software load is UNSIGNED.\nAre you sure continuing to download?");
        SetDLTaskInfo(_T("The handset is in DL or Emergency DL mode"));
        if(AfxMessageBox(message, MB_YESNO, MB_ICONINFORMATION) == IDNO) {
            SetDLTaskInfo(_T("User cancel download procedure"));
            return false;
        }
        return true;
    }

    bool CDownloadFuncForAll::CheckQFuseStage() {
        // Read FTM NV item first, then check QFuse step 1 can be ignore
        int nFTMValue = 0;
        bool bFused = false;
        SetDLTaskInfo(_T("Check FTM mode"));
        if(!CheckFTM(m_unComport, nFTMValue)) {
            SetDLTaskInfo(_T("Can not check FTM Mode"));
            return false;
        } else {
            SetDLTaskInfo(_T("Check QFuse"));
            bFused = (nFTMValue == 1) ? CheckQFuseState1() : (CheckQFuseState2() || CheckQFuseState3());
            SetDLTaskInfo(bFused ? _T("The handset is signed") : _T("The Handset is Unsigned"));

            if(m_bSigned != bFused) {
                SetDLTaskInfo(_T("The software load and the handset are not matched..."));
                SetDLTaskInfo(_T("Please make sure that the load and handset are all signed or all unsigned..."));
                //return true; //< force downloading
            }
        }
        return true;
    }
    bool CDownloadFuncForAll::CheckHandsetAndSoftwareVersion() {
        SetDLTaskInfo(_T("Check SW/HW Version"));
        return CheckSWHWVersion(m_unComport, m_nAmssVersion, m_nOemsblVersion, 3000);
    }

	void CDownloadFuncForAll::DownloadProcess() {
        // check battery
        //if(CheckBatteryStage() == IDNO) return;

        bool bDownloadOK = true;
        bool CheckVersion = false;

        DWORD dwStartCount = ::GetTickCount();

        for(int j = 0; j < m_strCheckLoadInfoArray.GetCount(); j++)
            SetDLTaskInfo(m_strCheckLoadInfoArray.GetAt(j));
        
        bool bInDownloadMode = IsDLMode(m_unComport);
        bDownloadOK = WarnInDownloadMode(bInDownloadMode);

        if(!bInDownloadMode && bDownloadOK) {
            //bDownloadOK = CheckQFuseStage();
            CheckVersion = CheckHandsetAndSoftwareVersion();
            CStringA strSW, strHW;
            if(!GetInfoFromME(strSW, strHW))
                SetDLTaskInfo(_T("Can not Get Info From ME!"));
        }

        if(bDownloadOK)
            bDownloadOK = NewDownloadStage();

        DWORD dwEndCount = ::GetTickCount();
        CString szTime;
        szTime.Format(_T("Download Cost Time %d s"),(dwEndCount - dwStartCount)/1000);
        SetDLTaskInfo(szTime);
        SetDLTaskInfo(bDownloadOK ? _T("Download Successful!") : _T("Download Failed!"));
	}

	bool CDownloadFuncForAll::GetDLTaskInfo(UINT nIndex, CString* szMsg, UINT& nStep, UINT& nProgress) {
		nStep = m_DownloadInfo[nIndex].uStep;
		nProgress = m_DownloadInfo[nIndex].ulPercentage;
        *szMsg = CString(m_DownloadInfo[nIndex].cMessage);
		return true;
	}


	void CDownloadFuncForAll::SetDLTaskInfo(const TCHAR* szMsg, int nStep, int nProgress) {
		if(m_pCallBackFunc != NULL) {
			char cstrMsg[400] ={0};
			UINT iUTF8CharCount = WideCharToMultiByte(CP_UTF8,FALSE,szMsg,-1, cstrMsg,400,NULL,NULL);	
			m_pCallBackFunc(cstrMsg, nStep, nProgress);
		} else {
			m_DownloadInfoIndex = (m_DownloadInfoIndex + 1) % DOWNLOAD_INFO_SIZE;
			m_DownloadInfo[m_DownloadInfoIndex].ulPercentage = nProgress;
			m_DownloadInfo[m_DownloadInfoIndex].uStep = nStep;
			_tcscpy(m_DownloadInfo[m_DownloadInfoIndex].cMessage,szMsg);
			if(m_hUIWnd != NULL)
				::PostMessage(m_hUIWnd, WM_DL_NOTIFY, m_DownloadInfoIndex, m_unButtonIndex);
		}
	}
	bool CDownloadFuncForAll::CallBackFunc(const char* szMsg, int nStep, int nProgress) {
		CString szMessage;
		TCHAR szUnicodeInfor[4096] = {0};
		UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szMsg,-1,szUnicodeInfor,4096);	
		g_pThis->SetDLTaskInfo(szUnicodeInfor, nStep, nProgress);
		return true;
	}

	WIN32IconButton* CDownloadFuncForAll::GetButtonWnd() {
		return m_pButtonWnd;
	}
	UINT CDownloadFuncForAll::GetComPort() {
		return m_unComport;
	}
	Win32ProgressBarWidge* CDownloadFuncForAll::GetProgressWnd() {
		return m_pProgressWnd;
	}
    void CDownloadFuncForAll::Set1ffFile(const TCHAR* str1ff) {
        m_str1ff = str1ff;
    }

    void CDownloadFuncForAll::SetCallBackFunc(CallBackInfo pCallBac) {
        m_pCallBackFunc = pCallBac;
    }

    void CDownloadFuncForAll::GetVariantVersion() {
        DIAG_NVITEM_PACKET_F_type NVResponse;
        CNVItem *pNVModule = new CNVItem(m_unComport);
        if(pNVModule->ReadNVItem(3497, NVResponse)) {
            TCHAR szUnicodeInfor[4096] = {0};
            UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)NVResponse.item_data,-1,szUnicodeInfor,4096);	
            CString szString;
            szString.Format(_T("Your Phone Variant: %s"), szUnicodeInfor);
            SetDLTaskInfo(szString);
        } else {
            SetDLTaskInfo(_T("Can not get Variant version from ME"));
        }
        delete pNVModule;
    }

    void CDownloadFuncForAll::GetHWSWVersionFromMobile( CStringA &strSW, CStringA &strHW ) {
        CQisdaMiscellaneousModule *pMsModule = new CQisdaMiscellaneousModule(m_unComport);
        char szSW[100] ={0}, szHW[100] ={0};
        if(pMsModule->GetSWHWVersion(szSW, szHW)) {
            TCHAR szUnicodeInfor[4096] = {0};
            UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szHW,-1,szUnicodeInfor,4096);	
            CString szString;
            szString.Format(_T("Your Phone HW: %s"), szUnicodeInfor);
            SetDLTaskInfo(szString);
            iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szSW,-1,szUnicodeInfor,4096);	
            szString.Format(_T("Your Phone SW: SVN %s"), szUnicodeInfor);
            SetDLTaskInfo(szString);
            strSW = szSW;
            strHW = szHW;
        } else {
            SetDLTaskInfo(_T("Can not Get SWHW Version from ME"));
        }
        delete pMsModule;
    }

	bool CDownloadFuncForAll::GetInfoFromME(CStringA& strSW, CStringA& strHW) {
        GetHWSWVersionFromMobile(strSW, strHW);
        GetVariantVersion();
		return true;
	}

	bool CDownloadFuncForAll::CheckQFuseState1()  {
		unsigned long reg1 = 0, reg2 = 0;
		bool bOK = false;
		ReadQFuseState(m_unComport, reg1, reg2, bOK);
		return !(reg1 == 6 && reg2 == 0);
	}
	// Check QFuse after change FTM
	bool CDownloadFuncForAll::CheckQFuseState2()  {
		OnLineMode(m_unComport);
		ChangeFTM(m_unComport);
		bool bRet = CheckQFuseState1();
		OnLineMode(m_unComport);
		return bRet;
	}
	// Check QFuse after write NV 453 (FTM)
	bool CDownloadFuncForAll::CheckQFuseState3()  {
		ChangeFTM(m_unComport);
		SetOffline(m_unComport);
		ResetHandset(m_unComport);
		Sleep(25000);
		return CheckQFuseState1();
	}

    void CDownloadFuncForAll::CreateTempFolder() {
        TCHAR pszPath[MAX_PATH] = {0};
        if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, pszPath)))  {
            m_strFolderPath = CString(pszPath) + CString(_T("\\MultiDownload\\"));
            ::CreateDirectory(m_strFolderPath, NULL);
            CString szTemp = m_str1ff.Right(m_str1ff.GetLength() - m_str1ff.ReverseFind(_T('\\')) - 1);
            szTemp.Replace(_T(".mot"), _T(""));
            m_strFolderPath = m_strFolderPath +  szTemp;
        } else {
            m_strFolderPath = m_str1ff;
            m_strFolderPath.Replace(_T(".mot"), _T(""));
        }
    }

	CString CDownloadFuncForAll::CheckLoad(Platform nPlatform) {
        m_nPlatform = nPlatform;
		::SetCursor(AfxGetApp()->LoadCursor(IDC_WAIT));
        CreateTempFolder();
		char cstrIni[MAX_PATH] ={0};
		WideCharToMultiByte(CP_ACP,FALSE,m_strIni.GetBuffer(0),m_strIni.GetLength(), cstrIni,MAX_PATH,NULL,NULL);	
		char cstr1ff[MAX_PATH] ={0};
		WideCharToMultiByte(CP_ACP,FALSE,m_str1ff.GetBuffer(0),m_str1ff.GetLength(), cstr1ff,MAX_PATH,NULL,NULL);	
		char cstrFolderPath[MAX_PATH] = {0};
		WideCharToMultiByte(CP_ACP,FALSE,m_strFolderPath.GetBuffer(0),m_strFolderPath.GetLength(), cstrFolderPath,MAX_PATH,NULL,NULL);

        if(!Extract1FF(cstr1ff, cstrIni, 0, cstrFolderPath)) {
            ::SetCursor(AfxGetApp()->LoadCursor(IDC_ARROW));
            return _T("");
        }
        switch (nPlatform) {
            case PLATFORM_6240:
                SetIniBy1ffFor6240(cstrIni, cstrFolderPath, cstr1ff);
                //ShowLoadInformation();
            case PLATFORM_6270:
                break;
            case PLATFORM_6245:
                SetIniBy1ffFor6245(cstrIni, cstrFolderPath, cstr1ff);
                //ShowLoadInformation();
                break;
            case PLATFORM_UKNOWN:
            default:
                ASSERT(0);
                break;
        }
        ::SetCursor(AfxGetApp()->LoadCursor(IDC_ARROW));
        return m_strFolderPath;
	}

	bool CDownloadFuncForAll::GetVariantString(CString& strVariantString)  {
		CString strFileName;
		TCHAR szPath[500] ={0};
		DWORD lRetValue = ::GetPrivateProfileString(_T("Path"),_T("cefs"),NULL,szPath, 500, m_strIni.GetBuffer(0));
		if(::_taccess_s(szPath,0) == 0)
			strFileName = szPath;
		else
			return false;
		strVariantString = _T("");
		if(strFileName.GetLength() > 0) {
			strVariantString = strFileName.Right(strFileName.GetLength() - strFileName.ReverseFind(_T('\\')) - 1);
			strVariantString.Replace(_T(".qvcefs"), _T(""));
			strVariantString.Replace(_T("_"), _T(" "));
		}
		return true;
	}

	TCHAR* CDownloadFuncForAll::GetDLPathFromIni(const TCHAR* strIniPath, const TCHAR* strName) {
		TCHAR szPath[500] ={0};
		DWORD lRetValue = ::GetPrivateProfileString(_T("Path"),strName,NULL,szPath, 500, strIniPath);
        return (::_taccess_s(szPath,0) == 0) ? szPath : _T("");
	}

	bool CDownloadFuncForAll::SetDLPathToIni(const TCHAR* strIniPath, const TCHAR* strName, TCHAR* strPath) {
		DWORD lRetValue = ::WritePrivateProfileString(_T("Path"), strName, strPath, strIniPath);
		return true;
	}

	bool CDownloadFuncForAll::DeleteUnzippedFolder(const TCHAR* strFolderPath) {
		CString szTemp = strFolderPath;
		szTemp = szTemp + _T("\\*.*");
		CFileFind findfile;
		BOOL find=findfile.FindFile(szTemp);
		while(find) {
			find=findfile.FindNextFile();
			DeleteFile(findfile.GetFilePath());
		}
		findfile.Close();
        DeleteFile(m_strIni);
		RemoveDirectory(strFolderPath);

		return true;
	}

    int CDownloadFuncForAll::CheckBatteryStage() {
        int nRet = IDYES;
        if(ReadBatteryVoltage() < 3600) {
            nRet = AfxMessageBox(_T("The battery is not enough to download.\nAre you sure to download?"), MB_YESNO, MB_ICONINFORMATION);
            if(nRet == IDNO) {
                SetDLTaskInfo(_T("User cancel download procedure."));
                SetDLTaskInfo(_T("Download Failed!"));
            }
        }
        return nRet;
    }

    int CDownloadFuncForAll::ReadBatteryVoltage() {
        SetDLTaskInfo(_T("Checking Battery voltage..."));
        int nVoltage = 0;
        ::ReadBatteryVoltage(m_unComport, nVoltage);
        TCHAR* szShowString = (nVoltage > 3600) ? _T("Battery voltage is enough to download...") : _T("Battery voltage is not enough to download...");
        SetDLTaskInfo(szShowString);
        return nVoltage;
    }

    bool CDownloadFuncForAll::OpenImageFile(SoftwareLoad* load) {
        if(load->m_bIsLoadOpened) return true;
        load->SetIniFilePath(m_strIni);
        load->GetImageFileName();
        load->GetImageContent();
        return true;
    }

    Platform CDownloadFuncForAll::GetPlatform(CString strFileName) {
        if(strFileName.GetLength() == 0) return PLATFORM_UKNOWN;
        Platform nRet = PLATFORM_UKNOWN;
        if(strFileName.Find(_T("WellFleet")) != -1) 
            nRet = PLATFORM_6245;
        else if(strFileName.Find(_T("Lavernock")) != -1)
            nRet = PLATFORM_6245;
        else if(strFileName.Find(_T("GMELB1A")) != -1)
            nRet = PLATFORM_6240;
        else if(strFileName.Find(_T("GMELB2A")) != -1)
            nRet = PLATFORM_6240;
        else if(strFileName.Find(_T("GMELB3A")) != -1)
            nRet = PLATFORM_6240;
        return nRet;
    }
}