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
	bool CDownloadFuncForMelos::m_bSigned = false;
	int	CDownloadFuncForMelos::m_nAmssVersion = 0;
	int CDownloadFuncForMelos::m_nOemsblVersion = 0;
//	bool CDownloadFunc::m_bCheckLoad = true;
	CStringArray CDownloadFuncForMelos::m_strCheckLoadInfoArray;
	CDownloadFuncForMelos* g_pThis = NULL;


    CDownloadFuncForMelos::CDownloadFuncForMelos(UINT unComport, UINT unButtonIndex, CString iniPath, HWND hUIWnd, WIN32IconButton *pButton, Win32ProgressBarWidge* pProgress)
        : m_unComport(unComport), m_unButtonIndex(unButtonIndex), 
        m_pButtonWnd(pButton), m_pProgressWnd(pProgress),
        m_strIni(iniPath), m_str1ff(_T("")), m_hUIWnd(hUIWnd),
        m_DownloadInfoIndex(0), m_pCallBackFunc(NULL), m_strFolderPath(_T("")), m_nPercentage(0),
        m_pImageParameter(NULL)
   
	{
		g_pThis = this;
	}

    CDownloadFuncForMelos::CDownloadFuncForMelos(UINT unComport, UINT unButtonIndex, CString iniPath,HWND hUIWnd, WIN32IconButton *pButton, Win32ProgressBarWidge* pProgress, DownloadFuncParameter* pImageParameter)
        : m_unComport(unComport), m_unButtonIndex(unButtonIndex), 
        m_pButtonWnd(pButton), m_pProgressWnd(pProgress),
        m_strIni(iniPath), m_str1ff(_T("")), m_hUIWnd(hUIWnd),
        m_DownloadInfoIndex(0), m_pCallBackFunc(NULL), m_strFolderPath(_T("")), m_nPercentage(0),
        m_pImageParameter(pImageParameter)
    {
        g_pThis = this;
    }

	CDownloadFuncForMelos::~CDownloadFuncForMelos(void) { }

    void CDownloadFuncForMelos::WriteLog(CFile& file, CStringA& info, int nLoop) {
        static char CRLN[2] = {0x0d, 0x0a};
        CStringA strTime;
        CTime t = CTime::GetCurrentTime();
        strTime.Format("[%d][%d:%d:%d]", nLoop, t.GetHour(), t.GetMinute(), t.GetSecond());
        file.Write(strTime.GetString(), strTime.GetLength());
        file.Write(info.GetString(), info.GetLength());
        file.Write(CRLN, 2);
        file.Flush();
    }

    BOOL CDownloadFuncForMelos::OpenLogFile(CFile& file) {
        TCHAR szBuf[MAX_PATH] = {0};
        GetCurrentDirectory(MAX_PATH, szBuf);
        CString strFileName;
        strFileName.Format(_T("%s\\Port%d.txt"), szBuf, m_unComport);
        BOOL bOpened =  file.Open(strFileName.GetString(), CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate);
        file.SeekToEnd();
        return bOpened;
    }

    bool CDownloadFuncForMelos::CheckIsInDownload(bool& bDownloadOK) {
        bool bIsDLMode = IsDLMode(m_unComport);
        if(bIsDLMode) {
            CString message = m_bSigned ?
                _T("The handset is in DL or Emerency DL mode.\nThe software load is SIGNED.\nAre you sure continuing to download?") :
            _T("The handset is in DL or Emerency DL mode.\nThe software load is UNSIGNED.\nAre you sure continuing to download?");
            if(AfxMessageBox(message, MB_YESNO, MB_ICONINFORMATION) == IDNO) {
                SetDLTaskInfo(_T("User cancel download procedure"));
                bDownloadOK = false;
            }
        }
        return bIsDLMode;
    }

    //////////////////////////////////////////////////////////////////////////
    // All the download checking is move out to othter function
    // This function only show the message and log the process
    //////////////////////////////////////////////////////////////////////////
    void CDownloadFuncForMelos::DownloadProcess() {
        bool bContinueDownload = true;
        bool bIsDLMode = CheckIsInDownload(bContinueDownload);

        if(!bIsDLMode)
            if(CheckBatteryStage() == IDNO)
                return;

        CFile f;
        BOOL bOpened = OpenLogFile(f);
        int i = 0;
        CStringA strWrite;
        strWrite.Format("Loop start:");
        WriteLog(f, strWrite, i);
        bool CheckVersion = false;

        DWORD dwStartCount = ::GetTickCount();

        for(int j = 0; j < m_strCheckLoadInfoArray.GetCount(); j++)
            SetDLTaskInfo(m_strCheckLoadInfoArray.GetAt(j));

        // this if condition will be entered only if the handset is not in DL mode, or forced download
        // notice that the previous if(bIsDLMode)
        if(bContinueDownload && !bIsDLMode) {
            // Read FTM NV item first, then check QFuse step 1 can be ignore
            int nFTMValue = 0;
            bool bFused = false;
            SetDLTaskInfo(_T("Check FTM mode"));
            strWrite.Format("Check FTM for QFuse...");
            WriteLog(f, strWrite, i);
            if(!CheckFTM(m_unComport, nFTMValue)) {
                SetDLTaskInfo(_T("Can not check FTM Mode"));
                strWrite.Format("Can not check FTM Mode...");
                WriteLog(f, strWrite, i);
                bContinueDownload = false;
            } else {
                strWrite.Format("Check QFuse...");
                WriteLog(f, strWrite, i);
                SetDLTaskInfo(_T("Check QFuse"));
                //bFused = (nFTMValue == 1) ? CheckQFuseState1() : (CheckQFuseState2() || CheckQFuseState3());
                SetDLTaskInfo(((nFTMValue == 1) ? CheckQFuseState1() : (CheckQFuseState2() || CheckQFuseState3())) ? _T("The handset is signed") : _T("The Handset is Unsigned"));

                if(m_bSigned != bFused)  {  // if the code is not matched the handset, stop download.
                    SetDLTaskInfo(_T("The software load and the handset are not matched..."));
                    SetDLTaskInfo(_T("Please make sure that the load and handset are all signed or all unsigned..."));
                    bContinueDownload = false;
                }
            }
            if(bContinueDownload) { //Check Version
                SetDLTaskInfo(_T("Check SWHW Version"));
                CheckVersion = CheckSWHWVersion(m_unComport, m_nAmssVersion, m_nOemsblVersion, 3000);

                SetDLTaskInfo(_T("Get ME info, Please wait......"));
                if(!GetInfoFromME())
                    SetDLTaskInfo(_T("Can not Get Info From ME!"));
            }
        } 

        if(bContinueDownload) {
            SetDLTaskInfo(_T("Start to download"));
            char cstrPath[MAX_PATH] ={0};
            UINT iUTF8CharCount = WideCharToMultiByte(CP_UTF8,FALSE,m_strIni.GetBuffer(0),m_strIni.GetLength(), cstrPath,MAX_PATH,NULL,NULL);	
            strWrite.Format("Start Download...");
            WriteLog(f, strWrite, i);
            // change to 6245 function to download, if will doing call back on Event()
            // Do all call back at there
            CShareDownload6240 download(m_unComport);
            download.Register(this, EVENT_DL_PROGRESS);
            download.SetAMSS(m_pImageParameter->pImgAMSS->pData, m_pImageParameter->pImgAMSS->nSize);
            download.SetCEFS(m_pImageParameter->pImgCEFS->pData, m_pImageParameter->pImgCEFS->nSize);
            download.SetDBL(m_pImageParameter->pImgDBL->pData, m_pImageParameter->pImgDBL->nSize);
            download.SetFSBL(m_pImageParameter->pImgFSBL->pData, m_pImageParameter->pImgFSBL->nSize);
            download.SetHex(m_pImageParameter->pImgHEX->pData, m_pImageParameter->pImgHEX->nSize);
            download.SetOSBL(m_pImageParameter->pImgOSBL->pData, m_pImageParameter->pImgOSBL->nSize);
            download.SetPartition(m_pImageParameter->pImgPartition->pData, m_pImageParameter->pImgPartition->nSize);
            download.SetPBL(m_pImageParameter->pImgPBL->pData, m_pImageParameter->pImgPBL->nSize);
            bContinueDownload = download.Download(true);
            download.Unregister(this, EVENT_DL_PROGRESS);

            strWrite.Format("End Download...");
            WriteLog(f, strWrite, i);
            ResetHandsetAfterDownload(bContinueDownload, strWrite, f, i);

        }

        DWORD dwEndCount = ::GetTickCount();
        CString szTime;
        szTime.Format(_T("Download Cost Time %d s"),(dwEndCount - dwStartCount)/1000);
        SetDLTaskInfo(szTime);
        SetDLTaskInfo(bContinueDownload ? _T("Download Successful!") : _T("Download Failed!"));
        strWrite.Format("End Loop...");
        WriteLog(f, strWrite, i);
        f.Close();
    }

    void CDownloadFuncForMelos::BackupBRTAfterDownload() {
        char szIMEI[256] = {0};
        bool bGetIMEI = GetIMEI(m_unComport, szIMEI);
        // use the csv file to backup the NV values
        bool bBackupBRT = BackupBrt(m_unComport, "", "");
    }

    void CDownloadFuncForMelos::ResetHandsetAfterDownload(bool bDownloadOK, CStringA &strWrite, CFile& f, int i) {
        if(bDownloadOK) {
            strWrite.Format("Reboot...");
            WriteLog(f, strWrite, i);
            while(true) {
                ChangeOnline(m_unComport);
                int nFTMValue = 0;
                CheckFTM(m_unComport, nFTMValue);
                if(nFTMValue == 1) {
                    strWrite.Format("Reboot failed...");
                    WriteLog(f, strWrite, i);
                }
                if(nFTMValue == 0)
                    break;
            }
            strWrite.Format("Reboot Complete...");
            WriteLog(f, strWrite, i);
            PowerOff(m_unComport);
        }
    }

	bool CDownloadFuncForMelos::GetDLTaskInfo(UINT nIndex, TCHAR* szMsg, UINT& nStep, UINT& nProgress)
	{
		nStep = m_DownloadInfo[nIndex].uStep;
		nProgress = m_DownloadInfo[nIndex].ulPercentage;
		_tcscpy(szMsg, m_DownloadInfo[nIndex].cMessage);
		return true;
	}


	void CDownloadFuncForMelos::SetDLTaskInfo(const TCHAR* szMsg, int nStep, int nProgress)
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
	bool CDownloadFuncForMelos::CallBackFunc(const char* szMsg, int nStep, int nProgress)
	{

		CString szMessage;

		TCHAR szUnicodeInfor[4096] = {0};
		UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szMsg,-1,szUnicodeInfor,4096);	
//		szMessage.Format(_T("%s, Percentage: %d"), szUnicodeInfor, nProgress);

		g_pThis->SetDLTaskInfo(szUnicodeInfor, nStep, nProgress);

		return true;
	}

	WIN32IconButton* CDownloadFuncForMelos::ReturnButtonWnd()
	{
		return m_pButtonWnd;
	}
	UINT	CDownloadFuncForMelos::ReturnComPort()
	{
		return m_unComport;
	}

	Win32ProgressBarWidge* CDownloadFuncForMelos::ReturnProgressWnd()
	{
		return m_pProgressWnd;
	}

	bool CDownloadFuncForMelos::GetInfoFromME()
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

	bool CDownloadFuncForMelos::CheckQFuseState1() 
	{
		unsigned long reg1 = 0, reg2 = 0;
		bool bOK = false;
		ReadQFuseState(m_unComport, reg1, reg2, bOK);
		return !(reg1 == 6 && reg2 == 0);
	}
	// Check QFuse after change FTM
	bool CDownloadFuncForMelos::CheckQFuseState2() 
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
	bool CDownloadFuncForMelos::CheckQFuseState3() 
	{
//		FTMMode(m_unComport);
//		SetFTM(m_unComport);
		ChangeFTM(m_unComport);
		SetOffline(m_unComport);
        ResetHandset(m_unComport);
		Sleep(25000);
		return CheckQFuseState1();
	}

	void CDownloadFuncForMelos::Set1ffFile(const TCHAR* str1ff)
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
	void CDownloadFuncForMelos::SetCallBackFunc(CallBackInfo pCallBac)
	{
		m_pCallBackFunc = pCallBac;
	}

	CString CDownloadFuncForMelos::CheckLoad()
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

	bool CDownloadFuncForMelos::CheckSignedCode(const TSTRING& strAmss, const TSTRING& strOemsbl) 
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

	bool CDownloadFuncForMelos::GetCodeVersion(const TSTRING& strFileName, char* szCheckCode, int nCheckLength, CString& strVer) {
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
	bool CDownloadFuncForMelos::GetVariantString(CString& strVariantString) 
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
	int CDownloadFuncForMelos::GetCodeSecurityVersion(const TSTRING& strFileName, char* szCheckCode, int nCheckLength) {
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
	TCHAR* CDownloadFuncForMelos::GetDLPathFromIni(const TCHAR* strIniPath, const TCHAR* strName)
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

	bool CDownloadFuncForMelos::SetDLPathToIni(const TCHAR* strIniPath, const TCHAR* strName, TCHAR* strPath)
	{
		DWORD lRetValue = ::WritePrivateProfileString(_T("Path"), strName, strPath, strIniPath);
		return true;
	}
	//void CDownloadFunc::SetLoadCheckFlag(bool bCheck)
	//{
	//	m_bCheckLoad = bCheck;
	//}

	bool CDownloadFuncForMelos::DeleteUnzippedFolder(const TCHAR* strFolderPath)
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

    int CDownloadFuncForMelos::CheckBatteryStage() {
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

    int CDownloadFuncForMelos::ReadBatteryVoltage() {
        SetDLTaskInfo(_T("Checking Battery voltage..."));
        int nVoltage = 0;
        ::ReadBatteryVoltage(m_unComport, nVoltage);
        //m_pFnReadBatteryVoltage(m_nSelectCom, nVoltage);
        TCHAR* szShowString = (nVoltage > 3600) ? _T("Battery voltage is enough to download...") : _T("Battery voltage is not enough to download...");
        SetDLTaskInfo(szShowString);
        return nVoltage;
    }
    void CDownloadFuncForMelos::Event(const string& strEvent, long nParam) {
        if(strEvent == EVENT_DL_PROGRESS) {
            int nProcentage = ((DL_PROGRESS*)nParam)->nProgress;
            if(nProcentage == m_nPercentage && nProcentage != 0) {

            } else {
                CStringA strPercent;
                DL_PROGRESS* dlMsg = (DL_PROGRESS*)nParam;
                strPercent.Format("%s %d%%",((DL_PROGRESS*)nParam)->szMsg, nProcentage);
#ifdef _UNICODE
                DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, strPercent.GetString(), -1, NULL, 0);
                wchar_t *pwText = new wchar_t[dwNum];
                MultiByteToWideChar (CP_ACP, 0, strPercent.GetString(), -1, pwText, dwNum);
                SetDLTaskInfo(pwText);
                delete[] pwText;
#else
                SetDLTaskInfo(strPercent.GetString());
#endif
            }
            m_nPercentage = nProcentage;
        }
    }
}