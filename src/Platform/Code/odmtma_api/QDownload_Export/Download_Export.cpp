// Download_Export.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Download_Export.h"
#include "..\..\CommonUtil\LogPolicy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//


// CDownload_ExportApp

BEGIN_MESSAGE_MAP(CDownload_ExportApp, CWinApp)
END_MESSAGE_MAP()


// CDownload_ExportApp construction

CQualcommDiag* g_pQualcommDiag = NULL;
CQDOperator* g_pQDownload = NULL;

CDownload_ExportApp::CDownload_ExportApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CDownload_ExportApp object

CDownload_ExportApp theApp;


// CDownload_ExportApp initialization

BOOL CDownload_ExportApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

void AddLog(int nCOMPort,const TSTRING& strMessage)
{
	CString strLogFile;
	strLogFile.Format(_T("Test_COM%02d.txt"),nCOMPort);
	
	LogPolicy::Log(strMessage.c_str(),strLogFile);
}

bool GetDLPathFromIni(const TSTRING& strIniPath, const TSTRING& strName, TSTRING& strPath)
{
	bool bRes = false;
	TCHAR szPath[500] ={0};
	DWORD lRetValue = ::GetPrivateProfileString(_T("Path"),strName.c_str(),NULL,szPath, 500, strIniPath.c_str());
	strPath = szPath;
	if(::_taccess_s(strPath.c_str(),0) == 0)
		bRes = true;
	else
		strPath = _T("");
	return bRes;
}

bool SetDLPathToIni(const TSTRING& strIniPath, const TSTRING& strName, const TSTRING& strPath)
{
	bool bRes = false;
	TCHAR szPath[500] ={0};
	DWORD lRetValue = ::WritePrivateProfileString(_T("path"), strName.c_str(), strPath.c_str(), strIniPath.c_str());
	if(lRetValue > 0)
		return true;
	else
		return false;

}
DL_API bool SetIniBy1ffFor6245(const string& strIni, const string& strFolderPath, const string& str1FF)
{
	bool bRes = false;
    CFileGetter* FileManager = new CFileGetterFor6245();
	//CFileGetterFor6245 FileManager;
	bool bSetMot = FileManager->SetMot(str1FF);
	if(bSetMot)
	{
		string strFolder;
		strFolder = strFolderPath;
		FileManager->SetMotExtractFolder(strFolder);
	}
	if(bSetMot && FileManager->SetFileName())
	{
		string strHex, strPartition, strQcsbl, strQcsblhd, strAmss, strAmsshd, strOemsbl, strOemsblhd;
		string strCEFS, strDu;

		if(FileManager->GetHex(strHex))
		{
			bRes = SetDLPathToIni(strIni, "hex", strHex);
		}	

		if(bRes && FileManager->GetPartition(strPartition))
		{
			bRes =  SetDLPathToIni(strIni, "partition", strPartition);
		}


		if(bRes && FileManager->GetOEMSBL(strOemsbl) && FileManager->GetOEMSBLHD(strOemsblhd) && 
				   FileManager->GetQCSBL(strQcsbl) && FileManager->GetQCSBLHD(strQcsblhd))
		{
			bRes = SetDLPathToIni(strIni, "qcsblhd", strQcsblhd);
			bRes = SetDLPathToIni(strIni, "qcsbl", strQcsbl);
			bRes = SetDLPathToIni(strIni, "oemsblhd", strOemsblhd);
			bRes = SetDLPathToIni(strIni, "oemsbl", strOemsbl);
		}

		if(bRes && FileManager->GetAMSS(strAmss) && FileManager->GetAMSSHD(strAmsshd))
		{
			bRes = SetDLPathToIni(strIni, "amsshd", strAmsshd);
			bRes = SetDLPathToIni(strIni, "amss", strAmss);
		}

		if(bRes &&  FileManager->GetCEFS(strCEFS) )
		{
			bRes = SetDLPathToIni(strIni, "cefs", strCEFS);
		}

		if(bRes && FileManager->GetDU(strDu))
		{
			bRes = SetDLPathToIni(strIni, "du", strDu);
		}
	}
    delete FileManager;
	return bRes;
}

DL_API bool SetIniBy1ffFor6240(const string& strIni, const string& strFolderPath, const string& str1FF) {
    bool bRes = false;
    CFileGetter* FileManager = new CFileGetterFor6240();
    //CFileGetterFor6240 FileManager;
    bool bSetMot = FileManager->SetMot(str1FF);
    if(bSetMot)
    {
        string strFolder;
        strFolder = strFolderPath;
        FileManager->SetMotExtractFolder(strFolder);
    }
    if(bSetMot && FileManager->SetFileName())
    {
        string strHex, strPartition, strPbl, strDbl, strFsbl, strOsbl, strAmss, strCefs;

        if(FileManager->GetHex(strHex))
        {
            bRes = SetDLPathToIni(strIni, "hex", strHex);
        }	

        if(bRes && FileManager->GetPartition(strPartition))
        {
            bRes =  SetDLPathToIni(strIni, "partition", strPartition);
        }


        if(bRes && FileManager->GetAMSS(strAmss) && FileManager->GetDBL(strDbl) &&
            FileManager->GetFSBL(strFsbl) && FileManager->GetOSBL(strOsbl))
        {
            bRes = SetDLPathToIni(strIni, "amss", strAmss);
            bRes = SetDLPathToIni(strIni, "dbl", strDbl);
            bRes = SetDLPathToIni(strIni, "fsbl", strFsbl);
            bRes = SetDLPathToIni(strIni, "osbl", strOsbl);
        }

        if(bRes &&  FileManager->GetCEFS(strCefs) )
        {
            bRes = SetDLPathToIni(strIni, "cefs", strCefs);
        }
    }
    delete FileManager;
    return bRes;
}

bool SetDLPathToParas(const TSTRING& strIniPath, CQDOperator& QDownload)
{
	bool bRes = false;
	TSTRING strHex, strPartition, strQcsbl, strQcsblhd, strAmss, strAmsshd, strOemsbl, strOemsblhd;
	TSTRING strCEFS, strDu;

	if(GetDLPathFromIni(strIniPath, _T("hex"), strHex) && GetDLPathFromIni(strIniPath, _T("partition"), strPartition))
	{
		QDownload.SetHex(strHex.c_str());
		QDownload.SetPartition(strPartition.c_str());
		bRes = true;
	}
	else
	{
		//AddLog(_T("HEX can't find."));
	}

	if(GetDLPathFromIni(strIniPath, _T("qcsbl"), strQcsbl) && GetDLPathFromIni(strIniPath, _T("qcsblhd"), strQcsblhd))
	{
		QDownload.SetQcsbl(strQcsbl.c_str());
		QDownload.SetQcsblhd(strQcsblhd.c_str());
	}
	else
	{
		//AddLog(_T("QCSBL can't find."));
	}

	if(GetDLPathFromIni(strIniPath, _T("oemsbl"), strOemsbl) && GetDLPathFromIni(strIniPath, _T("oemsblhd"), strOemsblhd))
	{
		QDownload.SetOemsbl(strOemsbl.c_str());
		QDownload.SetOemsblhd(strOemsblhd.c_str());
	}
	else
	{
		//AddLog(_T("OEMSBL can't find."));
	}

	if(GetDLPathFromIni(strIniPath, _T("amss"), strAmss) && GetDLPathFromIni(strIniPath, _T("amsshd"), strAmsshd))
	{
		QDownload.SetAMSS(strAmss.c_str());
		QDownload.SetAMSShd(strAmsshd.c_str());
	}
	else
	{
		//AddLog(_T("AMSS can't find."));
	}

		
	if(GetDLPathFromIni(strIniPath, _T("cefs"), strCEFS))
		QDownload.SetCEFS(strCEFS.c_str());
	else
	{
		//AddLog(_T("CEFS can't find."));
	}

	if(GetDLPathFromIni(strIniPath, _T("du"), strDu))
		QDownload.SetDu(strDu.c_str());
	else
	{
		//AddLog(_T("DU can't find."));
	}

	return bRes;
}

//bool SetFlashName(CFileGetterFor6245& FileManager, CQDOperator& QDownload, bool bDownloadOEMSBL)
//{   // at least contain AMSS and OEMSBL or only Flex file
//	bool bRes = false;
//	TSTRING strHex, strPartition, strQcsbl, strQcsblhd, strAmss, strAmsshd, strOemsbl, strOemsblhd;
//	TSTRING szCEFS, szDu;
//	if(FileManager.GetAMSS(strAmss) && FileManager.GetAMSSHD(strAmsshd))
//	{
//		QDownload.SetAMSS(strAmss.c_str());
//		QDownload.SetAMSShd(strAmsshd.c_str());
//	}
//	if( FileManager.GetHex(strHex) && FileManager.GetPartition(strPartition) 
//		&& FileManager.GetCEFS(szCEFS) && FileManager.GetDU(szDu))
//	{
//		QDownload.SetHex(strHex.c_str());
//		QDownload.SetPartition(strPartition.c_str());
//		QDownload.SetCEFS(szCEFS.c_str());
//		QDownload.SetDu(szDu.c_str());
//		bRes = true;
//	}
//	if(bDownloadOEMSBL)
//	{
//		if(FileManager.GetOEMSBL(strOemsbl) && FileManager.GetOEMSBLHD(strOemsblhd))
//		{
//			QDownload.SetOemsbl(strOemsbl.c_str());
//			QDownload.SetOemsblhd(strOemsblhd.c_str());
//		}
//		else
//			bRes = false;
//	}
//
//	return bRes;
//}

//bool SetBackupBrtPath(long nComPort, CQDOperator &QDownload )
//{
//	bool bRes = false;
//	TCHAR pszPath[MAX_PATH] = {0};
//	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, pszPath))) 
//	{
//		CString strFolder = CString(pszPath) + CString(_T("\\QDownload"));
//		::CreateDirectory(strFolder, NULL);
//		TCHAR szComPort[10] = {0};
//		_ltot(nComPort, szComPort, 10);
//		CString strBrtFilePath = CString(pszPath) + CString(_T("\\QDownload\\QDBackup")) + CString(szComPort) + CString(_T(".brt"));
//		QDownload.SetBrt(strBrtFilePath);
//		bRes = true;
//	}
//	QDownload.SetCsv(_T("BackupNV-Items.csv"));
//	return bRes;
//}

DL_API bool Extract1FF(const char* sz1FFPath, char* szIniPath, int nszIniPathBufferLength , const char* szExtractFolder) {
    bool bRes = false;

    TSTRING str1FFPath, strFolder;
#ifndef _UNICODE
    str1FFPath = sz1FFPath;
    if(szExtractFolder != NULL)
        strFolder = szExtractFolder;
#else
    str1FFPath = ansitowide(sz1FFPath);
    if(szExtractFolder != NULL)
        strFolder = ansitowide(szExtractFolder);
#endif
    CSingleLock singlelock(&g_CritUnZip,TRUE);
    ODMTMA_LIB::CQZip QZip;
    QZip.SetFolderPathFromMot((char*)sz1FFPath);
    bRes = QZip.UnzipWithoutExe(str1FFPath, strFolder);

    return bRes;
}

DL_API bool Extract1FFToINI(const char* sz1FFPath, char* szIniPath, int nszIniPathBufferLength , const char* szExtractFolder)
{
	bool bRes = false;

	TSTRING str1FFPath, strFolder;
#ifndef _UNICODE
	str1FFPath = sz1FFPath;
	if(szExtractFolder != NULL)
		strFolder = szExtractFolder;
#else
	str1FFPath = ansitowide(sz1FFPath);
	if(szExtractFolder != NULL)
		strFolder = ansitowide(szExtractFolder);
#endif
	CSingleLock singlelock(&g_CritUnZip,TRUE);
	ODMTMA_LIB::CQZip QZip;
	QZip.SetFolderPathFromMot((char*)sz1FFPath);
	bRes = QZip.UnzipWithoutExe(str1FFPath, strFolder);

	//if(bRes)
	//{
	//	bRes = SetIniBy1ffFor6245(szIniPath, szExtractFolder, str1FFPath);
	//}

	return bRes;
}

DL_API void SetComport(long nComPort) {
    g_pQualcommDiag->ReOpenComport(nComPort);
}

DL_API bool Download(long nComPort, bool(*CallBack)(const char*, int , int),const LPCSTR strIniPath, DownloadParameter *pdp)
{ // for 3GSWup
	bool bRes = false;
	if(CallBack)
		CallBack("OpenComPort", 1, 0);
	CQualcommDiag Diag(_T(""),nComPort);
    g_pQualcommDiag = &Diag;
	if(Diag.IsConnected())
	{
		if(CallBack)
			CallBack("OpenComPort", 1, 100);
		CQDOperator Operator(&Diag);
        g_pQDownload = &Operator;
        TCHAR pszPath[MAX_PATH];
        if((bRes = SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, pszPath)))) {
            CString strFolder = CString(pszPath) + CString(_T("\\QDownload"));
            BOOL bCreateFolder = ::CreateDirectory(strFolder, NULL);
            CString strBrtFilePath;
            strBrtFilePath.Format(_T("%s%s%d.brt"), pszPath, _T("\\QDownload\\QDBackup"), nComPort);
            Operator.SetBrt(strBrtFilePath);
        }
        if(bRes)
            Operator.SetCsv(_T("BackupNV-Items.csv"));
		if(strIniPath && SetDLPathToParas(strIniPath, Operator))
		{
			Operator.SetCallback(CallBack);
			bool bProcess = true;
			if(!Diag.IsDLMode())
			{
                CString strErrorNVItemList;
                if(pdp->bBackupBRT)
                    bProcess = Operator.BackupBRT(strErrorNVItemList) >= 0;
                if(pdp->bBackupNV)
                    bProcess = (Operator.BackupNV(8000) >=0);
            } else {
                if(CallBack)
                    CallBack("Backup", 2, 100);
            }
            if(bProcess) {
                Operator.SetQcsbl(_T(""));
                Operator.SetQcsblhd(_T(""));
                Operator.SetOemsbl(_T(""));
                Operator.SetOemsblhd(_T(""));
			    bProcess = Operator.SpeedUpFlash() >= 0 ;
            }
            if(bProcess) {
                if(pdp->bRestoreNV)
                    bProcess = Operator.RestoreNV(40000) >=0 ;
                if(pdp->bRestoreBRT)
                    bProcess = Operator.RestoreBRT(true) >= 0;
            }
            if(bProcess && (Operator.GetDu() != _T("")) && pdp->bDownloadDU) {
			    bProcess = Operator.DownloadDU() >=0 ;
            }
			bRes = bProcess;
		}
	}
	if(CallBack)
	{
		if(bRes)
			CallBack("Download Success", 0, 100);
		else
			CallBack("Download Fail", 0, 0);
	}
	return bRes;
}

bool SetDLPathToParas(const DLSetting& tagPaths, CQDOperator& QDownload)
{
	bool bRes = false;
	if(::_taccess_s(tagPaths.szHex,0) == 0 && ::_taccess_s(tagPaths.szPartition,0) == 0)
	{
		QDownload.SetHex(tagPaths.szHex);
		QDownload.SetPartition(tagPaths.szPartition);
		bRes = true;
	}
	if(::_taccess_s(tagPaths.szQcsbl,0) == 0 && ::_taccess_s(tagPaths.szQcsblhd,0) == 0 )
	{
		QDownload.SetQcsbl(tagPaths.szQcsbl);
		QDownload.SetQcsblhd(tagPaths.szQcsblhd);
	}
	if(::_taccess_s(tagPaths.szOemsbl,0) == 0 && ::_taccess_s(tagPaths.szOemsblhd,0) == 0 )
	{
		QDownload.SetOemsbl(tagPaths.szOemsbl);
		QDownload.SetOemsblhd(tagPaths.szOemsblhd);
	}
	if(::_taccess_s(tagPaths.szAmss,0) == 0 && ::_taccess_s(tagPaths.szAmsshd,0) == 0 )
	{
		QDownload.SetAMSS(tagPaths.szAmss);
		QDownload.SetAMSShd(tagPaths.szAmsshd);
	}

	if(::_taccess_s(tagPaths.szCefs,0) == 0)
		QDownload.SetCEFS(tagPaths.szCefs);

	return bRes;
}

DL_API bool DownloadByPath(long nComPort, bool(*CallBack)(const char*, int , int),const DLSetting& tagSetting)
{
	AddLog(nComPort,_T("DownloadByPath() - start"));

	bool bRes = false;
	if(CallBack)
		CallBack("OpenComPort", 1, 0);
	CQualcommDiag Diag(_T(""),nComPort);
	if(Diag.IsConnected())
	{
		if(CallBack)
			CallBack("OpenComPort", 1, 100);
		CQDOperator QDownload(&Diag);
		if(SetDLPathToParas(tagSetting, QDownload))
		{
			QDownload.SetCallback(CallBack);
			bRes = QDownload.SpeedUpFlash() >=0 ? true: false;
		}
		else
		{			
			AddLog(nComPort,_T("SetDLPathToParas fail"));
		}
	}
	else
	{
		AddLog(nComPort,_T("Diag.IsConnected() fail"));
	}

	if(CallBack)
	{
		if(bRes)
			CallBack("Download Success", 0, 100);
		else
			CallBack("Download Fail", 0, 0);
	}

	AddLog(nComPort,_T("DownloadByPath() - end"));

	return bRes;
}

DL_API bool BackupBrt(long nComPort, const LPCSTR szCsv, const LPCSTR szBrt)
{
	AddLog(nComPort,_T("BackupBrt() - end"));

	bool bRes = false;
	CQualcommDiag Diag(_T(""),nComPort);
	if(Diag.IsConnected())
	{
		CQDOperator QDownload(&Diag);
		bool bCsv = QDownload.SetCsv(szCsv) >=0 ? true : false;
		bool bBrt = QDownload.SetBrt(szBrt) >=0 ? true : false;
		CString strErrorNVItemList;
		if(bCsv && bBrt)
			bRes = QDownload.BackupBRT(strErrorNVItemList) >=0 ? true: false;
	}

	AddLog(nComPort,_T("BackupBrt() - end"));
	return bRes;
}


DL_API bool RestoreBrt(long nComPort, const LPCSTR szBrt,bool bFilterRealonly)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""),nComPort);
	if(Diag.IsConnected())
	{
		CQDOperator QDownload(&Diag);
		bool bBrt = QDownload.SetBrt(szBrt) >=0 ? true : false;
		if(bBrt)
			bRes = QDownload.RestoreBRT(bFilterRealonly) >= 0 ? true : false;
	}
	return bRes;
}


DL_API bool ReadFactoryArea(long nComport, WORD nNVItem, unsigned char* szFactoryArea128, int& nState, int nTimeOut/* =DEFAULT_CMD_TIME_OUT */)
{
	bool bRes = false;
	if(szFactoryArea128)
	{
		CQualcommDiag *pDiag = new CQualcommDiag(_T(""), nComport);
		if(pDiag)
		{
			DIAG_NVITEM_PACKET_F_type resp = {0};
			if(pDiag->ReadNVItem(nNVItem, resp, nTimeOut))
			{
				memcpy(szFactoryArea128, resp.item_data, 128);

				nState = resp.nv_stat;
				if(nState == 0)
					bRes = true;
			}
			else
			{
				if(resp.nv_stat == NV_NOTACTIVE_S)
				{
					nState = resp.nv_stat;
					bRes = true;
				}
			}
			delete pDiag;
		}
	}
	return bRes;
}
DL_API bool WriteFactoryArea(long nComport, WORD nNVItem, const unsigned char* szFactoryArea128, int nTimeOut/* =DEFAULT_CMD_TIME_OUT */)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""),nComport);
	if(szFactoryArea128)
	{
		if(Diag.IsConnected())
		{
			DIAG_NVITEM_PACKET_F_type req = {0};
			req.item = nNVItem;
			memcpy(req.item_data, szFactoryArea128, 128);
			if(Diag.WriteNVItem(req, nTimeOut))
				bRes = true;
		}
	}
	return bRes;
}

DL_API bool IsDLMode(long nComPort)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""),nComPort);
	bRes = Diag.IsDLMode();
	return bRes;
}

DL_API bool BackupNV(long nComPort, int nTimeOut/* =DEFAULT_CMD_TIME_OUT */)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""),nComPort);
	if(Diag.IsConnected())
	{
		bRes = Diag.Backup(nTimeOut);
	}
	return bRes;
}
DL_API bool RestoreNV(long nComPort, int nTimeOut/* =DEFAULT_CMD_TIME_OUT */)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""),nComPort);
	if(Diag.IsConnected())
	{
		bRes = Diag.Restore(nTimeOut);
	}
	return bRes;
}

DL_API bool DownloadDU(long nComPort, bool(*CallBack)(const char*, int , int), const LPCSTR szDuPath)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""),nComPort);
	bool bRetryConnection = true;
	for(int i=0; i<5 && bRetryConnection; i++)
	{
		if(Diag.IsConnected())
		{
			bRetryConnection = false;
			CQDOperator QDownload(&Diag);
			QDownload.SetCallback(CallBack);
			if(::_taccess_s(szDuPath,0) == 0)
			{
				QDownload.SetDu(szDuPath);
				bRes = QDownload.DownloadDU() >=0 ? true: false;
			}
		}
		else
		{
			if(CallBack)
				CallBack("Connection Fail", 0, 0);
		}
	}
	if(CallBack)
	{
		if(bRes)
			CallBack("Download Success", 0, 100);
		else
			CallBack("Download Fail", 0, 0);
	}
	return bRes;
}

DL_API bool Restart(long nComPort, int nTimeOut/* =DEFAULT_CMD_TIME_OUT */)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""),nComPort);
	if(Diag.IsConnected())
	{
		bRes = Diag.Restart(nTimeOut);
	}
	return bRes;
}
DL_API bool IsConnected(long nComPort)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""),nComPort);
	bRes = Diag.IsConnected();
	return bRes;
}

DL_API bool IsQfused(long nComPort, bool& bQfused, int nTimeOut/* =DEFAULT_CMD_TIME_OUT */)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""),nComPort);
	if(Diag.IsConnected())
	{
		if(Diag.SetTestMode(nTimeOut))
			bRes = Diag.IsQfused(bQfused, nTimeOut);
	}
	return bRes;
}

DL_API bool ReadQfuse(long nComPort, unsigned int& nReg1, unsigned int& nReg2, int nTimeOut/* =DEFAULT_CMD_TIME_OUT */)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""),nComPort);
	if(Diag.IsConnected())
	{
		if(Diag.SetTestMode(nTimeOut))
			bRes = Diag.ReadQfuse(nReg1, nReg2, nTimeOut);
	}
	return bRes;
}


DL_API bool Signed_CheckVersion(long nComPort, const LPCTSTR szAMSS, const LPCTSTR szOEMSBL, bool& bDownloadOEMSBL, int nTimeOut/* =DEFAULT_CMD_TIME_OUT */)
{
	bool bRes = false;
	bDownloadOEMSBL = false;
	if(szAMSS && szOEMSBL)
	{
		ODMTMA_LIB::CQVersionParser VersionParser;
		if(VersionParser.IsCodeSigned(szAMSS, szOEMSBL))
		{
			int nImagePhoneCodeVer = VersionParser.GetPhoneCodeVersion(szAMSS);
			int nImageBootloaderVer = VersionParser.GetBootloaderVersion(szOEMSBL);
			if(nImagePhoneCodeVer != -1 && nImageBootloaderVer != -1)
			{
				CQualcommDiag Diag(_T(""),nComPort);
				if(Diag.IsConnected())
				{
					int nUnitPhoneCodeVer = 0, nUnitBootloaderVer = 0;
					bool bGetUnitVersion = false;
					if(Diag.IsDLMode())
					{
						CQDOperator Operator(&Diag);
						bGetUnitVersion = Operator.GetHDVersion(nUnitPhoneCodeVer, nUnitBootloaderVer, nTimeOut);
					}
					else
					{
						bGetUnitVersion = Diag.GetHDVersion(nUnitPhoneCodeVer, nUnitBootloaderVer, nTimeOut);
					}
					if(bGetUnitVersion)
					{
						if((nImageBootloaderVer >= nUnitBootloaderVer) && (nImagePhoneCodeVer >= nUnitPhoneCodeVer))
						{
							bDownloadOEMSBL = nImageBootloaderVer > nUnitBootloaderVer;
							bRes = true;
						}
					}
				}
			}
		}
	}
	
	return bRes;
}
DL_API bool UnSigned_CheckVersion(const LPCTSTR szAMSS, const LPCTSTR szOEMSBL)
{
	bool bRes = false;
	if(szAMSS && szOEMSBL)
	{
		ODMTMA_LIB::CQVersionParser VersionParser;
		bRes = (VersionParser.IsCodeSigned(szAMSS, szOEMSBL) == false);
	}
	return bRes;
}

DL_API bool WriteQfused(long nComPort,WORD chain_select, int config_register[5], int& nQfusedStatus, int nTimeOut)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""),nComPort);
	if(Diag.IsConnected())
	{
		if(Diag.SetTestMode(nTimeOut))
			bRes = Diag.WriteQfused(chain_select, config_register, nQfusedStatus, nTimeOut);
	}
	return bRes;
}

DL_API bool CheckSWHWVersion(long nComPort, int nPhoneCodeVersion, int nBootloadVersion, int nTimeout) {
    CQualcommDiag Diag(_T(""),nComPort);
    bool bRes = false;
    if(Diag.IsConnected()) {
        int nUnitPhoneCodeVer = 0, nUnitBootloaderVer = 0;
        bool bGetUnitVersion = false;
        if(Diag.IsDLMode()) {
            CQDOperator Operator(&Diag);
            bGetUnitVersion = Operator.GetHDVersion(nUnitPhoneCodeVer, nUnitBootloaderVer, nTimeout);
        } else {
            bGetUnitVersion = Diag.GetHDVersion(nUnitPhoneCodeVer, nUnitBootloaderVer, nTimeout);
        }
        if(bGetUnitVersion) {
            if((nPhoneCodeVersion > nUnitBootloaderVer) && (nBootloadVersion > nUnitPhoneCodeVer))
                bRes = true;
        }
    }
    return bRes;
}