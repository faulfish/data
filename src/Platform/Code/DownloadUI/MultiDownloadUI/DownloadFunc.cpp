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


namespace QSYNC_LIB
{
	bool CDownloadFunc::m_bSigned = false;
	int	CDownloadFunc::m_nAmssVersion = 0;
	int CDownloadFunc::m_nOemsblVersion = 0;
//	bool CDownloadFunc::m_bCheckLoad = true;
	CStringArray CDownloadFunc::m_strCheckLoadInfoArray;
	CDownloadFunc* g_pThis = NULL;


	CDownloadFunc::CDownloadFunc(UINT unComport, UINT unButtonIndex, CString iniPath, HWND hUIWnd, WIN32IconButton *pButton, Win32ProgressBarWidge* pProgress)
	{
		m_unComport = unComport;
		m_unButtonIndex = unButtonIndex;
		m_pButtonWnd = pButton;
		m_pProgressWnd = pProgress;
		m_strIni =  iniPath;
		m_str1ff = _T("");
		m_hUIWnd = hUIWnd;

		m_DownloadInfoIndex = 0;
		g_pThis = this;
		m_pCallBackFunc = NULL;

		m_strFolderPath = _T("");

	}

	CDownloadFunc::~CDownloadFunc(void)
	{
	}

    void CDownloadFunc::WriteLog(CFile& file, CStringA& info, int nLoop) {
        char CRLN[2] = {0x0d, 0x0a};
        CStringA strTime;
        CTime t = CTime::GetCurrentTime();
        strTime.Format("[%d][%d:%d:%d]", nLoop, t.GetHour(), t.GetMinute(), t.GetSecond());
        file.Write(strTime.GetString(), strTime.GetLength());
        file.Write(info.GetString(), info.GetLength());
        file.Write(CRLN, 2);
        file.Flush();
    }

	void CDownloadFunc::DownloadProcess()
	{
        if(CheckBatteryStage() == IDNO) {
            return;
        }

        bool bDownloadOK = true;
        bool CheckVersion = false;

        DWORD dwStartCount = ::GetTickCount();

        for(int j = 0; j < m_strCheckLoadInfoArray.GetCount(); j++)
            SetDLTaskInfo(m_strCheckLoadInfoArray.GetAt(j));
        
        bool bInDownloadMode = IsDLMode(m_unComport);
        if(bInDownloadMode) {
            CString message;
            if(m_bSigned)
                message = _T("The handset is in DL or Emerency DL mode.\nThe software load is SIGNED.\nAre you sure continuing to download?");
            else
                message = _T("The handset is in DL or Emerency DL mode.\nThe software load is UNSIGNED.\nAre you sure continuing to download?");
            SetDLTaskInfo(_T("The handset is in DL or Emerency DL mode"));
            if(AfxMessageBox(message, MB_YESNO, MB_ICONINFORMATION) == IDNO) {
                SetDLTaskInfo(_T("User cancel download procedure"));
                bDownloadOK = false;
            }
        }

        if(bDownloadOK)
        {
            // Read FTM NV item first, then check QFuse step 1 can be ignore
            int nFTMValue = 0;
            bool bFused = false;
            SetDLTaskInfo(_T("Check FTM mode"));
            if(!bInDownloadMode && !CheckFTM(m_unComport, nFTMValue))
            {
                SetDLTaskInfo(_T("Can not check FTM Mode"));
                bDownloadOK = false;
            }
            else
            {
                SetDLTaskInfo(_T("Check QFuse"));
                if(nFTMValue == 1)
                    bFused = CheckQFuseState1();    ///< if the value is true, the mobile is fused
                else
                    bFused = CheckQFuseState2() || CheckQFuseState3();

                if(bFused)
                    SetDLTaskInfo(_T("The handset is signed"));
                else
                    SetDLTaskInfo(_T("The Handset is Unsigned"));

                if(m_bSigned != bFused) {
                    SetDLTaskInfo(_T("The software load and the handset are not matched..."));
                    SetDLTaskInfo(_T("Please make sure that the load and handset are all signed or all unsigned..."));
                    bDownloadOK = true; //< force downloading
                }
            }

            if(!bInDownloadMode && bDownloadOK)
            { //Check Version
                SetDLTaskInfo(_T("Check SW/HW Version"));
                CheckVersion = CheckSWHWVersion(m_unComport, m_nAmssVersion, m_nOemsblVersion, 3000);
            }
        }
        CStringA strSW, strHW;
        if(!bInDownloadMode && bDownloadOK)
        {
            //SetDLTaskInfo(_T("Get ME info, Please wait......"));
            if(!GetInfoFromME(strSW, strHW))
                SetDLTaskInfo(_T("Can not Get Info From ME!"));
        }

        if(bDownloadOK)
        {
            SetDLTaskInfo(_T("Start to download"));

            char cstrPath[MAX_PATH] ={0};
            UINT iUTF8CharCount = WideCharToMultiByte(CP_UTF8,FALSE,m_strIni.GetBuffer(0),m_strIni.GetLength(), cstrPath,MAX_PATH,NULL,NULL);	

            //ASSERT(false);
            DownloadParameter dp = {0};
            dp.bBackupBRT = true;
            dp.bBackupNV = false;
            dp.bDownloadOemsbl = false;
            dp.bRestoreBRT = false;
            dp.bRestoreNV = false;
            dp.bDownloadDU = true;
            if(m_pCallBackFunc != NULL)
                bDownloadOK = Download(m_unComport, m_pCallBackFunc, cstrPath, &dp);
            else
                bDownloadOK = Download(m_unComport,CallBackFunc, cstrPath, &dp);

            if(bDownloadOK) {
                while(true) {
                    ChangeOnline(m_unComport);
                    int nFTMValue = 0;
                    CheckFTM(m_unComport, nFTMValue);
                    if(nFTMValue == 0)
                        break;
                }
                PowerOff(m_unComport);
            }
        }

        DWORD dwEndCount = ::GetTickCount();
        CString szTime;
        szTime.Format(_T("Download Cost Time %d s"),(dwEndCount - dwStartCount)/1000);
        SetDLTaskInfo(szTime);

        if(bDownloadOK) {
            SetDLTaskInfo(_T("Download Successful!"));
        } else {
            SetDLTaskInfo(_T("Download Failed!"));
        }
	}

	bool CDownloadFunc::GetDLTaskInfo(UINT nIndex, TCHAR* szMsg, UINT& nStep, UINT& nProgress)
	{
		nStep = m_DownloadInfo[nIndex].uStep;
		nProgress = m_DownloadInfo[nIndex].ulPercentage;
		_tcscpy(szMsg, m_DownloadInfo[nIndex].cMessage);
		return true;
	}


	void CDownloadFunc::SetDLTaskInfo(const TCHAR* szMsg, int nStep, int nProgress)
	{
		if(m_pCallBackFunc != NULL)
		{
			char cstrMsg[300] ={0};
			UINT iUTF8CharCount = WideCharToMultiByte(CP_UTF8,FALSE,szMsg,-1, cstrMsg,300,NULL,NULL);	
			m_pCallBackFunc(cstrMsg, nStep, nProgress);
		}
		else
		{
			m_DownloadInfoIndex = (m_DownloadInfoIndex + 1) % DOWNLOAD_INFO_SIZE;
			m_DownloadInfo[m_DownloadInfoIndex].ulPercentage = nProgress;
			m_DownloadInfo[m_DownloadInfoIndex].uStep = nStep;
			_tcscpy(m_DownloadInfo[m_DownloadInfoIndex].cMessage,szMsg);


			if(m_hUIWnd != NULL)
			{
				::PostMessage(m_hUIWnd, WM_DL_NOTIFY, m_DownloadInfoIndex, m_unButtonIndex);
			}

		}

	}
	bool CDownloadFunc::CallBackFunc(const char* szMsg, int nStep, int nProgress)
	{

		CString szMessage;

		TCHAR szUnicodeInfor[4096] = {0};
		UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szMsg,-1,szUnicodeInfor,4096);	
//		szMessage.Format(_T("%s, Percentage: %d"), szUnicodeInfor, nProgress);

		g_pThis->SetDLTaskInfo(szUnicodeInfor, nStep, nProgress);

		return true;
	}

	WIN32IconButton* CDownloadFunc::ReturnButtonWnd()
	{
		return m_pButtonWnd;
	}
	UINT	CDownloadFunc::ReturnComPort()
	{
		return m_unComport;
	}

	Win32ProgressBarWidge* CDownloadFunc::ReturnProgressWnd()
	{
		return m_pProgressWnd;
	}

	bool CDownloadFunc::GetInfoFromME(CStringA& strSW, CStringA& strHW)
	{
		CQisdaMiscellaneousModule *pMsModule = new CQisdaMiscellaneousModule(m_unComport);

		char szSW[100] ={0};
		char szHW[100] ={0};

		if(pMsModule->GetSWHWVersion(szSW, szHW))
		{
			TCHAR szUnicodeInfor[4096] = {0};
			UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szHW,-1,szUnicodeInfor,4096);	
			CString szString;
			szString.Format( _T("Your Phone HW: %s"), szUnicodeInfor);
			SetDLTaskInfo(szString);
			iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szSW,-1,szUnicodeInfor,4096);	
			szString.Format(_T("Your Phone SW: SVN %s"), szUnicodeInfor);
			SetDLTaskInfo(szString);
            strSW = szSW;
            strHW = szHW;

		}
		else
		{
			SetDLTaskInfo(_T("Can not Get SWHW Version from ME"));
		}

		delete pMsModule;

		DIAG_NVITEM_PACKET_F_type NVResponse;
		CNVItem *pNVModule = new CNVItem(m_unComport);
		if(pNVModule->ReadNVItem(3497, NVResponse))
		{
			TCHAR szUnicodeInfor[4096] = {0};
			UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)NVResponse.item_data,-1,szUnicodeInfor,4096);	
			CString szString;
			szString.Format(_T("Your Phone Variant: %s"), szUnicodeInfor);
			SetDLTaskInfo(szString);

		}
		else
		{
			SetDLTaskInfo(_T("Can not get Variant version from ME"));
		}

		delete pNVModule;
		return true;
	}

	bool CDownloadFunc::CheckQFuseState1() 
	{
		unsigned long reg1 = 0, reg2 = 0;
		bool bOK = false;
		ReadQFuseState(m_unComport, reg1, reg2, bOK);
		return !(reg1 == 6 && reg2 == 0);
	}
	// Check QFuse after change FTM
	bool CDownloadFunc::CheckQFuseState2() 
	{
		OnLineMode(m_unComport);
//		FTMMode(m_unComport);
//		SetFTM(m_unComport);
		ChangeFTM(m_unComport);
		bool bRet = CheckQFuseState1();
		OnLineMode(m_unComport);
		return bRet;
	}
	// Check QFuse after write NV 453 (FTM)
	bool CDownloadFunc::CheckQFuseState3() 
	{
//		FTMMode(m_unComport);
//		SetFTM(m_unComport);
		ChangeFTM(m_unComport);
		SetOffline(m_unComport);
		ResetHandset(m_unComport);
		Sleep(25000);
		return CheckQFuseState1();
	}

	void CDownloadFunc::Set1ffFile(const TCHAR* str1ff)
	{
		m_str1ff = str1ff;
//		SetDLPathToIni(m_strIni, _T("1ff"), m_str1ff.GetBuffer());
	}

	//void CDownloadFunc::SetLoadInfo(const bool bSigned, const int nAmssVersion, const int nOemsblVersion)
	//{
	//	m_bSigned = bSigned;
	//	m_nAmssVersion = nAmssVersion;
	//	m_nOemsblVersion = nOemsblVersion;
	//}
	void	CDownloadFunc::SetCallBackFunc(CallBackInfo pCallBac)
	{
		m_pCallBackFunc = pCallBac;
	}

	CString CDownloadFunc::CheckLoad()
	{
		//if(!m_bCheckLoad)
		//	return true;
		bool bRes = true;

		::SetCursor(AfxGetApp()->LoadCursor(IDC_WAIT));

		TCHAR pszPath[MAX_PATH] = {0};
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, pszPath))) 
		{
			m_strFolderPath = CString(pszPath) + CString(_T("\\MultiDownload\\"));
			::CreateDirectory(m_strFolderPath, NULL);
			CString szTemp = m_str1ff.Right(m_str1ff.GetLength() - m_str1ff.ReverseFind(_T('\\')) - 1);
			szTemp.Replace(_T(".mot"), _T(""));
			m_strFolderPath = m_strFolderPath +  szTemp;
		}
		else
		{
			m_strFolderPath = m_str1ff;
			m_strFolderPath.Replace(_T(".mot"), _T(""));
		}

		char cstrIni[MAX_PATH] ={0};
		WideCharToMultiByte(CP_ACP,FALSE,m_strIni.GetBuffer(0),m_strIni.GetLength(), cstrIni,MAX_PATH,NULL,NULL);	
		char cstr1ff[MAX_PATH] ={0};
		WideCharToMultiByte(CP_ACP,FALSE,m_str1ff.GetBuffer(0),m_str1ff.GetLength(), cstr1ff,MAX_PATH,NULL,NULL);	
		char cstrFolderPath[MAX_PATH] = {0};
		WideCharToMultiByte(CP_ACP,FALSE,m_strFolderPath.GetBuffer(0),m_strFolderPath.GetLength(), cstrFolderPath,MAX_PATH,NULL,NULL);

		if(Extract1FFToINI(cstr1ff, cstrIni, 0, cstrFolderPath))
		{
			CString strVariantString;
			GetVariantString(strVariantString);
			strVariantString = CString(_T("Variant: ")) + strVariantString;
			m_strCheckLoadInfoArray.Add(strVariantString);

			TSTRING strOemsbl, strAmss;
			TCHAR szPath[500] ={0};
			DWORD lRetValue = ::GetPrivateProfileString(_T("Path"),_T("oemsbl"),NULL,szPath, 500, m_strIni.GetBuffer(0));
			strOemsbl = szPath;
			lRetValue = ::GetPrivateProfileString(_T("Path"),_T("amss"),NULL,szPath, 500, m_strIni.GetBuffer(0));
			strAmss = szPath;

			CString strCodeVersion;
			GetCodeVersion(strOemsbl, "O'zbplt", 7, strCodeVersion);
			strCodeVersion = CString(_T("Software version: ")) + strCodeVersion;
			m_strCheckLoadInfoArray.Add(strCodeVersion);

			if(CheckSignedCode(strAmss, strOemsbl))
			{
				m_bSigned = true;
				m_strCheckLoadInfoArray.Add(_T("The software loads are all signed"));
			}
			else
			{
				m_bSigned = false;
				m_strCheckLoadInfoArray.Add(_T("The Software loads are all unsigned"));
			}	
			m_nAmssVersion = GetCodeSecurityVersion(strAmss, "O'zapft", 7);
			m_nOemsblVersion = GetCodeSecurityVersion(strOemsbl, "O'zbplt", 7);

//			m_bCheckLoad = false;
		}
		else 
			bRes = false;

		::SetCursor(AfxGetApp()->LoadCursor(IDC_ARROW));

		if(bRes)
			return m_strFolderPath;
		else
			_T("");
	}

	bool CDownloadFunc::CheckSignedCode(const TSTRING& strAmss, const TSTRING& strOemsbl) 
	{
		CFile famss(strAmss.c_str(), CFile::modeRead | CFile::typeBinary| CFile::shareDenyNone );
		CFile foemsbl(strOemsbl.c_str(), CFile::modeRead | CFile::typeBinary| CFile::shareDenyNone );

		BYTE* szAmssBuf = new BYTE[4096];
		BYTE* szOemsblBuf = new BYTE[foemsbl.GetLength()];

		const char* szAttestation = "QUALCOMM Attestation CA";
		int nLength = famss.Read(szAmssBuf, 4096);
		nLength = foemsbl.Read(szOemsblBuf, foemsbl.GetLength());

		bool bAmss = false, bOemsbl = false;
		for(int i = 0; i <  4096 - 23; ++i) {
			int n = memcmp(&(szAmssBuf[i]), szAttestation, 23);
			if(n == 0) {
				bAmss = true;
				break;
			}
		}
		if(!bAmss)
			m_strCheckLoadInfoArray.Add(_T("Check Signed Code--amss..."));
		else
			m_strCheckLoadInfoArray.Add(_T("Check UnSigned Code--amss..."));
		for(int i = 0; i <  foemsbl.GetLength() - 23; ++i) {
			int n = memcmp(&(szOemsblBuf[i]), szAttestation, 23);
			if(n == 0) {
				bOemsbl = true;
				break;
			}
		}
		if(!bOemsbl)
			m_strCheckLoadInfoArray.Add(_T("Check Signed Code--oemsbl..."));
		else
			m_strCheckLoadInfoArray.Add(_T("Check UnSigned Code--oemsbl..."));

		delete[] szAmssBuf;
		delete[] szOemsblBuf;
		return bAmss && bOemsbl;
	}

	bool CDownloadFunc::GetCodeVersion(const TSTRING& strFileName, char* szCheckCode, int nCheckLength, CString& strVer) {
		CFile file;
		BOOL bOpen = file.Open(strFileName.c_str(), CFile::modeRead | CFile::typeBinary);
		if(!bOpen) return -1;
		char* szBuf = new char[file.GetLength()];
		file.Read(szBuf, file.GetLength());
		int nCheckPoint = 0;
		for(long i = 0; i < file.GetLength() - nCheckLength; ++i) {
			if(memcmp(&(szBuf[i]), szCheckCode, nCheckLength) == 0) {
				nCheckPoint = i;
				break;
			}
		}
		char szGet[100] = {0};
		memcpy(szGet, &(szBuf[nCheckPoint]), 100);
		delete [] szBuf;
		int nCount = 0, nIndex = 0;
		char szVersion[100] = {0};
		for(int i = 0; i < 100; ++i) {
			if(szGet[i] == ';') ++nCount;
			if(nCount == 2) {
				szVersion[nIndex++] = szGet[i];
			} else if(nCount == 3) {
				break;
			}
		}
		strVer.Format(_T("%S"), szVersion);
		strVer.Replace(_T(";"), _T(""));
		return true;
	}
	bool CDownloadFunc::GetVariantString(CString& strVariantString) 
	{
		CString strFileName;
		TCHAR szPath[500] ={0};
		DWORD lRetValue = ::GetPrivateProfileString(_T("Path"),_T("cefs"),NULL,szPath, 500, m_strIni.GetBuffer(0));
		if(::_taccess_s(szPath,0) == 0)
			strFileName = szPath;
		else
			return false;
		strVariantString = _T("");
		if(strFileName.GetLength() > 0)
		{
			strVariantString = strFileName.Right(strFileName.GetLength() - strFileName.ReverseFind(_T('\\')) - 1);
			strVariantString.Replace(_T(".qvcefs"), _T(""));
			strVariantString.Replace(_T("_"), _T(" "));
		}
		return true;
	}
	int CDownloadFunc::GetCodeSecurityVersion(const TSTRING& strFileName, char* szCheckCode, int nCheckLength) {
		CFile file;
		BOOL bOpen = file.Open(strFileName.c_str(), CFile::modeRead | CFile::typeBinary);
		if(!bOpen) return -1;
		char* szBuf = new char[file.GetLength()];
		file.Read(szBuf, file.GetLength());
		int nCheckPoint = 0;
		for(long i = 0; i < file.GetLength() - nCheckLength; ++i) {
			if(memcmp(&(szBuf[i]), szCheckCode, nCheckLength) == 0) {
				nCheckPoint = i;
				break;
			}
		}
		char szGet[100] = {0};
		memcpy(szGet, &(szBuf[nCheckPoint]), 100);
		delete [] szBuf;
		int nCount = 0, nIndex = 0;
		char szVersion[10] = {0};
		for(int i = 0; i < 100; ++i) {
			if(szGet[i] == ';') ++nCount;
			if(nCount == 5) {
				szVersion[nIndex++] = szGet[i];
			} else if(nCount == 6) {
				break;
			}
		}
		szVersion[0] = '0';
		int nRet = atoi((const char*)szVersion);
		return nRet;
	}
	TCHAR* CDownloadFunc::GetDLPathFromIni(const TCHAR* strIniPath, const TCHAR* strName)
	{
		bool bRes = false;
		TCHAR szPath[500] ={0};
		DWORD lRetValue = ::GetPrivateProfileString(_T("Path"),strName,NULL,szPath, 500, strIniPath);
		//		strPath = szPath;
		if(::_taccess_s(szPath,0) == 0)
			return szPath;
		else 
			return _T("");
		//			bRes = true;

		//		return szPath;
		//else
		//	strPath = _T("");
		//return bRes;
	}

	bool CDownloadFunc::SetDLPathToIni(const TCHAR* strIniPath, const TCHAR* strName, TCHAR* strPath)
	{
		DWORD lRetValue = ::WritePrivateProfileString(_T("Path"), strName, strPath, strIniPath);
		return true;
	}
	//void CDownloadFunc::SetLoadCheckFlag(bool bCheck)
	//{
	//	m_bCheckLoad = bCheck;
	//}

	bool CDownloadFunc::DeleteUnzippedFolder(const TCHAR* strFolderPath)
	{
		CString szTemp = strFolderPath;
		szTemp = szTemp + _T("\\*.*");
		CFileFind findfile;
		BOOL find=findfile.FindFile(szTemp);

		while(find)
		{
			find=findfile.FindNextFile();
			DeleteFile(findfile.GetFilePath());
		}
		findfile.Close();
		RemoveDirectory(szTemp);

		return true;
	}

    int CDownloadFunc::CheckBatteryStage() {
        int nRet = IDYES;
        if(ReadBatteryVoltage() < 3600) {
            nRet = AfxMessageBox(_T("The bettery is not enough to download.\nAre you sure to download?"), MB_YESNO, MB_ICONINFORMATION);
            if(nRet == IDNO) {
                SetDLTaskInfo(_T("User cancel download procedure."));
                SetDLTaskInfo(_T("Download Failed!"));
            }
        }
        return nRet;
    }

    int CDownloadFunc::ReadBatteryVoltage() {
        SetDLTaskInfo(_T("Checking Battery voltage..."));
        int nVoltage = 0;
        ::ReadBatteryVoltage(m_unComport, nVoltage);
        //m_pFnReadBatteryVoltage(m_nSelectCom, nVoltage);
        TCHAR* szShowString = (nVoltage > 3600) ? _T("Battery voltage is enough to download...") : _T("Battery voltage is not enough to download...");
        SetDLTaskInfo(szShowString);
        return nVoltage;
    }
}