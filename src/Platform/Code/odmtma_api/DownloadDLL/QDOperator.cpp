#include "StdAfx.h"
#include "QDOperator.h"
#include "..\..\..\Code\CommonUtil\LogPolicy.h"
#include "..\..\..\code\Lib\RequestUtility\DownloadCommand.h"

#define BACKUP_NV_NUM	(2048)


CQDOperator::CQDOperator(CQualcommDiag* pQualcommDiag):
	m_CQualCmd(pQualcommDiag),
	m_nwaitforreadcount(0), 
	m_dwaddress(0),
	m_bSkipChecking(FALSE),
	m_CallBackFunc(NULL),
	m_CallBack(NULL),
	m_pQualcommDiag(pQualcommDiag),
	m_bBackup(false),
	m_bRestore(true)
{
	m_strLogFile = _T("QDoperator.txt");	
}

CQDOperator::CQDOperator():
	m_nwaitforreadcount(0), 
	m_dwaddress(0),
	m_bSkipChecking(TRUE),
	m_CallBackFunc(NULL),
	m_CallBack(NULL),
	m_pQualcommDiag(NULL),
	m_bBackup(false),
	m_bRestore(true)
{
	m_strLogFile = _T("ODoperator.txt");
}

CQDOperator::~CQDOperator(void)
{

}

int CQDOperator::SetCOMPort(LPCTSTR szCOM)
{
	int nRet = RUN_SUCCESSFULLY;
	_ASSERTE(szCOM != NULL);
	if(szCOM)
		nRet = m_CQualCmd.SetComPort(szCOM);
	else
		nRet = RUN_FAILED;
	return nRet;
}


int CQDOperator::SetPbl(LPCTSTR szPbl)
{
	int nRet = RUN_SUCCESSFULLY;
	_ASSERTE(szPbl != NULL);
	if(szPbl)
		m_szPbl = szPbl;
	else
		nRet = RUN_FAILED;
	return nRet;
}

int CQDOperator::SetQcsbl(LPCTSTR szQcsbl)
{
	int nRet = RUN_SUCCESSFULLY;
	_ASSERTE(szQcsbl != NULL);
	if(szQcsbl)
		m_szQcsbl = szQcsbl;
	else
		nRet = RUN_FAILED;
	return nRet;
}

int CQDOperator::SetQcsblhd(LPCTSTR szQcsblhd)
{
	int nRet = RUN_SUCCESSFULLY;
	_ASSERTE(szQcsblhd != NULL);
	if(szQcsblhd)
		m_szQcsblhd = szQcsblhd;
	else
		nRet = RUN_FAILED;
	return nRet;
}

int CQDOperator::SetOemsbl(LPCTSTR szOemsbl)
{
	int nRet = RUN_SUCCESSFULLY;
	_ASSERTE(szOemsbl != NULL);
	if(szOemsbl)
		m_szOemsbl = szOemsbl;
	else
		nRet = RUN_FAILED;
	return nRet;
}

int CQDOperator::SetOemsblhd(LPCTSTR szOemsblhd)
{
	int nRet = RUN_SUCCESSFULLY;
	_ASSERTE(szOemsblhd != NULL);
	if(szOemsblhd)
		m_szOemsblhd = szOemsblhd;
	else
		nRet = RUN_FAILED;
	return nRet;
}

int CQDOperator::SetAMSS(LPCTSTR szAmss)
{
	int nRet = RUN_SUCCESSFULLY;
	_ASSERTE(szAmss != NULL);
	if(szAmss)
		m_szAmss = szAmss;
	else
		nRet = RUN_FAILED;
	return nRet;
}

int CQDOperator::SetAMSShd(LPCTSTR szAmsshd)
{
	int nRet = RUN_SUCCESSFULLY;
	_ASSERTE(szAmsshd != NULL);
	if(szAmsshd)
		m_szAmsshd = szAmsshd;
	else
		nRet = RUN_FAILED;
	return nRet;
}

int CQDOperator::SetPartition(LPCTSTR szPartition)
{
	int nRet = RUN_SUCCESSFULLY;
	_ASSERTE(szPartition != NULL);
	if(szPartition)
		m_szPartition = szPartition;
	else
		nRet = RUN_FAILED;
	return nRet;
}

int CQDOperator::SetCEFS(LPCTSTR szCEFS)
{
	int nRet = RUN_SUCCESSFULLY;
	_ASSERTE(szCEFS != NULL);
	if(szCEFS)
		m_szCEFS = szCEFS;
	else
		nRet = RUN_FAILED;
	return nRet;
}

int CQDOperator::SetHex(LPCTSTR szHex)
{
	int nRet = RUN_SUCCESSFULLY;
	_ASSERTE(szHex != NULL);
	if(szHex)
		m_szHex = szHex;
	else
		nRet = RUN_FAILED;
	return nRet;
}

int CQDOperator::SetCsv(LPCTSTR szCsv)
{
	int nRet = RUN_SUCCESSFULLY;
	_ASSERTE(szCsv != NULL);
	if(szCsv)
		m_szCsv = szCsv;
	else
		nRet = RUN_FAILED;
	return nRet;
}

int CQDOperator::SetBrt(LPCTSTR szBrt)
{
	AddLog(szBrt);

	int nRet = RUN_SUCCESSFULLY;
	_ASSERTE(szBrt != NULL);
	if(szBrt)
		m_szBrt = szBrt;
	else
		nRet = RUN_FAILED;
	return nRet;
}

int CQDOperator::SetDu(LPCTSTR szDu)
{
	int nRet = RUN_SUCCESSFULLY;
	_ASSERTE(szDu != NULL);
	if(szDu)
		m_szDu = szDu;
	else
		nRet = RUN_FAILED;
	return nRet;
}

int CQDOperator::SetCallBackFunc(LPVOID pfunc)
{
	int nRet = RUN_FAILED;	
	_ASSERTE(pfunc != NULL);
	if(pfunc && ((nRet =m_CQualCmd.SetCallBackFunc(pfunc)) >= 0))
	{
		CallBackFunc pFunc = (CallBackFunc)pfunc;
		m_CallBackFunc = pFunc;
		nRet = RUN_SUCCESSFULLY;
	}
	return nRet;
}
bool CQDOperator::SetCallback(LPVOID pfunc)
{
	bool bRes = false;
	if(pfunc && m_CQualCmd.SetCallBack(pfunc))
	{
		m_CallBack = (CallBack)pfunc;
		bRes = true;
	}
	return bRes;
}
void CQDOperator::LogFunc(const char* cpMsg, int nRet)
{
	bool bFail = (nRet < 0);
	std::string strMsg = cpMsg;
	strMsg = bFail ? strMsg+" Fail!" : strMsg+" Success!";
	if(m_CallBackFunc)
		m_CallBackFunc(strMsg.c_str(), bFail ? -1 : 0 );
}
void CQDOperator::LogFunc(const char* cpMsg, int nRet, int nStep, int nPercent)
{
	bool bFail = (nRet < 0);
	std::string strMsg = cpMsg;
	strMsg = bFail ? strMsg+" Fail" : strMsg+" Success";
	if(m_CallBack)
		m_CallBack(strMsg.c_str(), nStep, bFail ? 0 : nPercent );
}


void CQDOperator::SetBackup(bool bBackup)
{
	m_bBackup = bBackup;
}
void CQDOperator::SetRestore(bool bRestore)
{
	m_bRestore = bRestore;
}
int CQDOperator::CFCSpeedUpFlash()
{
	int nRet = SpeedUpFlash();
	if(nRet >= 0)
	{
		nRet = RestoreNV(60000);
		if(nRet <0)
		{
			for(int i=0; i<5 && nRet<0; i++)
				nRet = RestoreNV(60000);
		}
		
	}
	if(nRet >= 0)
		nRet = DownloadDU();
	return nRet;
}

int CQDOperator::BackupNV(int nTimeout)
{
	int nRet = RUN_FAILED;	
	if(m_CallBack)
		m_CallBack("Backup", 2, 0);
	if(m_pQualcommDiag && !m_pQualcommDiag->Backup(nTimeout))
	{
		m_pQualcommDiag->SetLastError(DL_ERROR_BACKUP);
		
	}
	else
	{
		nRet = RUN_SUCCESSFULLY;
		if(m_CallBack)
			m_CallBack("Backup", 2, 100);
	}
	return nRet;
}
int CQDOperator::RestoreNV(int nTimeout)
{
	int nRet = RUN_FAILED;
	if(m_CallBack)
		m_CallBack("Restore", 7, 0);
	if(m_pQualcommDiag && !m_pQualcommDiag->Restore(nTimeout))
	{
		m_pQualcommDiag->SetLastError(DL_ERROR_Restore);	
	}
	else
	{
		nRet = RUN_SUCCESSFULLY;
		if(m_CallBack)
			m_CallBack("Restore", 7, 100);
	}
	return nRet;
}

int CQDOperator::DownloadDU()
{
	int nRet = RUN_FAILED;
	if(m_CallBack)
		m_CallBack("Download DU", 8, 0);
	if(!m_szDu.IsEmpty())
	{
		if(DuckFile2NVArray(m_szDu))
		{
			if(m_CallBack)
				m_CallBack("Download DU", 8, 50);
			nRet = NVArray2Mobile() ? RUN_SUCCESSFULLY : RUN_FAILED;
			if(nRet<0)
			{
				if(m_pQualcommDiag)
					m_pQualcommDiag->SetLastError(DL_ERROR_NVArray2Mobile);	
			}
			else
			{
				if(m_CallBack)
					m_CallBack("Download DU", 8, 100);
			}
		}
		else
		{
			if(m_pQualcommDiag)
				m_pQualcommDiag->SetLastError(DL_ERROR_DuckFile2NVArray);
		}
	}
	return nRet;
}

int CQDOperator::SpeedUpFlash()
{
	int nRet = RUN_FAILED;

	if(!m_szHex.IsEmpty() && !m_szPartition.IsEmpty() && (!m_szCEFS.IsEmpty() || (!m_szAmss.IsEmpty() && !m_szAmsshd.IsEmpty())))
	{
		if( ((nRet = m_CQualCmd.ChangeToDLMode()) >= 0))
		{
			if(DMSSDownload(nRet))
			{
				if(SpeedUpStreamingDownload(nRet))
				{
                    Sleep(25000);
                    //InstallDriver();
					Checking(nRet);
				}
			}
		}
	}
	else
		m_pQualcommDiag->SetLastError(DL_ERROR_FilePathEmpty);
	return nRet;
}

bool CQDOperator::DMSSDownload(int& nRet)
{
	bool bRes = false;
	nRet = RUN_FAILED;
	if(m_CallBack)
		m_CallBack("DMSS Download", 4, (100/4)*0);
	for(int nCount=0; nCount <10 && nRet <0 ; nCount++) // default 3
	{
		nRet = m_CQualCmd.CmdNopOpeation();
		Sleep(1000);
	}
	if(m_CallBack)
		m_CallBack("DMSS Download", 4, (100/4)*1);
	if(nRet <0)
	{
		//m_CQualCmd.CloseComPort();
		if(m_pQualcommDiag)
			m_pQualcommDiag->SetLastError(DL_ERROR_DMSS_NOP);
	}
	else
	{
		if(((nRet = m_CQualCmd.CmdParameter()	   	)>= 0 ) &&
		   ((nRet = OpenHexFile(m_szHex)		   	)>= 0 ) &&
		   ((nRet = m_CQualCmd.CmdGO(m_dwaddress,0)	)>= 0 ) )
		{
			bRes = true;
		}
	}
	return bRes;
}

bool CQDOperator::StreamingDownload(int& nRet)
{
	bool bRes = false;
	nRet = RUN_FAILED;
	for( int nCount =0;  nCount <10 && nRet < 0; nCount ++)//至少要送Hello兩次以上，因為第一次會沒反應	
		nRet = m_CQualCmd.CmdHello();

	if(nRet >= 0)
	{
		BYTE nSecurity = (m_szPbl == _T("")) ? 0X01 : 0X00 ;
		if( ((nRet = m_CQualCmd.CmdSercurity(nSecurity))    >= 0) &&
			((nRet = WritePartitiontoPhone(m_szPartition))  >= 0) &&
			  OpenandWriteAllImage(nRet))
		{
			bRes = true;
		}
		bRes = (nRet >= 0);
	}
	
		
	m_CQualCmd.CmdSReset();
	m_CQualCmd.CloseComPort();
	return bRes;
}

bool CQDOperator::SpeedUpStreamingDownload(int& nRet)
{
	bool bRes = false;
	nRet = RUN_FAILED;
	if(m_CallBack)
		m_CallBack("Streaming Download", 5, (100/6)*0);

	int nHelloSuccess = 0;
	for( int nCount =0;  nCount <15 && nHelloSuccess < 2; nCount ++)//至少要送Hello兩次以上，因為第一次會沒反應	
	{	
		nRet = m_CQualCmd.CmdSpeedUpOrgHello();
		if(nRet >= 0)
			nHelloSuccess++;
	}


	if(nRet >= 0 || nHelloSuccess>0)
	{
		if(m_CallBack)
			m_CallBack("Streaming Download", 5, (100/6)*1);
		BYTE nSecurity = (m_szPbl == _T("")) ? 0X01 : 0X00 ;
		if( ((nRet = m_CQualCmd.CmdSpeedUpSercurity(nSecurity))    >= 0) &&
			((nRet = WritePartitiontoPhone(m_szPartition, true))  >= 0) &&
			OpenandWriteAllImage(nRet,true))
		{
			bRes = true;
		}
		bRes = (nRet >= 0);
	}
	else
	{
		if(m_pQualcommDiag)
			m_pQualcommDiag->SetLastError(DL_ERROR_Streaming_Hello);
	}
	
	m_CQualCmd.CmdSReset(true);
	if(nRet >= 0)
		m_CQualCmd.CloseComPort();
	return bRes;
}

bool CQDOperator::InstallDriver() {
    TCHAR szPath[512] = {0};
    GetCurrentDirectory(512, szPath);
    CString strPath = szPath;
    strPath += _T("\\NewDriver\\X86\\DPInst.exe");
    SHELLEXECUTEINFO info = {0};
    info.cbSize = sizeof(SHELLEXECUTEINFO);
    info.fMask = SEE_MASK_NOCLOSEPROCESS;
    info.hwnd = NULL;
    info.lpVerb = NULL;
    info.lpFile = strPath.GetString();
    info.lpParameters = _T("/LM /A /SA");
    info.lpDirectory = szPath;
    info.nShow = SW_SHOWNORMAL;
    ShellExecuteEx(&info);
    WaitForSingleObject(info.hProcess, INFINITE);
    return true;
}

void CQDOperator::Checking(int& nRet)
{	
	TRACE_FILE_LINE_INFO();
	if(m_bSkipChecking)
	{
		Sleep(20000);
		if(m_CallBackFunc)
		{
			m_CallBackFunc("Skip Checking POWER_ON Status.", 0);
			m_CallBackFunc("Download Process Success.", 0);
		}	
	}
	else
	{
		nRet = RUN_FAILED;
		if(m_CallBack)
			m_CallBack("ReOpenComPort", 6, 0);
		for(int i=0; i<20 && (nRet < 0); i++)
		{
			TRACE_FILE_LINE_INFO();
			nRet = m_CQualCmd.OpenComPort();
			if(nRet <0)
			{
				Sleep(5000);
			}
			else
			{
				if( (nRet = m_CQualCmd.ChangeToDiagMode(true)) >= 0)
				{
					TRACE_FILE_LINE_INFO();
					nRet = m_CQualCmd.CmdDiagIsPhoneConnected();
					if(m_CallBack)
						m_CallBack("ReOpenComPort", 6, 100/20*i);
				}
			}
		}
		if(nRet<0 )
		{
			if(m_pQualcommDiag)
				m_pQualcommDiag->SetLastError(DL_ERROR_ReOpenComport);
			
		}
		else
		{
			if(m_CallBack)
				m_CallBack("ReOpenComPort", 6, 100);
		}
	}
}




int CQDOperator::WriteImagetoPhone(const CString& szfilename, bool bSpeedUp, int nPrograss)
{
	CFile cfimage;
	CFileException pError ;
	int nRet = RUN_FAILED;

    if(cfimage.Open(szfilename,CFile::shareDenyNone | CFile::modeRead,&pError) != TRUE)
        return RUN_FAILED;

    DWORD dwImageSize = cfimage.GetLength();
    BYTE* fileContent = new BYTE[dwImageSize];
    DWORD dwReadBytes = cfimage.Read(fileContent, dwImageSize);
    ASSERT(dwReadBytes == dwImageSize);
    long nLoopCount = dwImageSize / PAGE_SIZE + (dwImageSize % PAGE_SIZE == 0 ? 0 : 1);
    BYTE bytemp[PAGE_SIZE+32] = {0};
    DWORD dwWriteAddress = 0;
    int nCurrentPercentage = 100;
    int nProgressOption = 0;
    if(bSpeedUp) {
        for(long i = 0; i < nLoopCount - 1; ++i) {
            //memcpy(bytemp, fileContent + (i * PAGE_SIZE), PAGE_SIZE);
            nRet = m_CQualCmd.CmdSpeedUpStrWrite2(fileContent + (i * PAGE_SIZE), PAGE_SIZE, dwWriteAddress);
            dwWriteAddress += PAGE_SIZE;

            if((nCurrentPercentage != dwWriteAddress * 100 / dwImageSize) && m_CallBack && (nProgressOption = ++nProgressOption % 3) == 0) {
                m_CallBack("[Streaming Download]Write Image", 5, dwWriteAddress * 100 / dwImageSize);
                nCurrentPercentage = dwWriteAddress * 100 / dwImageSize;
                //Sleep(1);
            }
            Sleep(1);
        }
        // write last time
        DWORD nFinalSize = dwImageSize % PAGE_SIZE;
        memcpy(bytemp, fileContent + (nLoopCount-1) * PAGE_SIZE, nFinalSize == 0 ? PAGE_SIZE : nFinalSize);
        nRet = m_CQualCmd.CmdSpeedUpStrWrite2(fileContent + (nLoopCount-1) * PAGE_SIZE, nFinalSize == 0 ? PAGE_SIZE : nFinalSize, dwWriteAddress);
        if(m_CallBack)
            m_CallBack("[Streaming Download]Write Image", 5, 100);
    } else {
        for(long i = 0; i < nLoopCount - 1; ++i) {
            memcpy(bytemp, fileContent + (i * PAGE_SIZE), PAGE_SIZE);
            nRet = m_CQualCmd.CmdStrWrite2(bytemp, PAGE_SIZE, dwWriteAddress);
            dwWriteAddress += PAGE_SIZE;

            if((nCurrentPercentage != dwWriteAddress * 100 / dwImageSize) && m_CallBack && (nProgressOption = ++nProgressOption % 3) == 0) {
                m_CallBack("[Streaming Download]Write Image", 5, dwWriteAddress * 100 / dwImageSize);
                nCurrentPercentage = dwWriteAddress * 100 / dwImageSize;
            }
            Sleep(0);
        }
        // write last time
        DWORD nFinalSize = dwImageSize % PAGE_SIZE;
        memcpy(bytemp, fileContent + (nLoopCount-1) * PAGE_SIZE, nFinalSize == 0 ? PAGE_SIZE : nFinalSize);
        nRet = m_CQualCmd.CmdStrWrite2(bytemp, nFinalSize == 0 ? PAGE_SIZE : nFinalSize, dwWriteAddress);
    }
    delete [] fileContent;

	//if(cfimage.Open(szfilename,CFile::shareDenyNone | CFile::modeRead,&pError) == TRUE)
	//{
	//	DWORD dwImageSize = cfimage.GetLength();
	//	if(dwImageSize>0)
	//	{
	//		DWORD waddress = 0;
	//		WORD dwsize = 0;
	//		DWORD dwWriteSize = 0;
	//		DWORD dwPrevProgress = 0;
	//		int nTimes =0;
	//		do {
	//			BYTE bytemp[PAGE_SIZE+32] = {0};
	//			waddress+=dwsize;
	//			dwsize = cfimage.Read((void *)bytemp,PAGE_SIZE);    //利用讀多少Bytes的dwsize判斷是否到尾端
	//			if(!bSpeedUp)
	//				nRet = m_CQualCmd.CmdStrWrite2(bytemp,dwsize,waddress);
	//			else
	//				nRet = m_CQualCmd.CmdSpeedUpStrWrite2(bytemp,dwsize,waddress);
	//			nTimes++;
	//			if( nRet >= 0 ) 
	//			{
	//				DWORD dwCurProgress = 0;
	//				dwWriteSize += dwsize;
	//				dwCurProgress = (dwWriteSize*100)/dwImageSize;
	//				

	//				if ( dwCurProgress > dwPrevProgress )
	//				{
	//					CString strTemp;
	//					strTemp.Format(_T(" %3.d %% completed."), dwCurProgress);
	//					//TRACE("****************************** %3.d %% completed.******************************\n", dwCurProgress);
	//					if(m_CallBack)
	//						m_CallBack("[Streaming Download]Write Image",5, nPrograss/3+nPrograss/3*dwCurProgress/100);
	//					dwPrevProgress = dwCurProgress;
	//					if(m_CallBackFunc)
	//						m_CallBackFunc((LPCTSTR)strTemp, 0);
	//				}
	//			}
	//			else
	//				ASSERT(0);
	//			Sleep(0);
	//		} while( dwsize == PAGE_SIZE && nRet >=0 );
	//	}
	//	
	//	cfimage.Close();
	//}
	return nRet;
}



int CQDOperator::WritePartitiontoPhone(const CString& szfilename, bool bSpeedUp)
{
	int nRet = RUN_FAILED;
	CFile cfpartition;
	CFileException pError;
	if(szfilename != _T("") && cfpartition.Open(szfilename,CFile::shareDenyNone,&pError) == TRUE) 
	{
		UINT unsize = cfpartition.GetLength();
		BYTE *lpbyte = new BYTE[unsize+1];
		cfpartition.Read(lpbyte,unsize);
		if(!bSpeedUp)
			nRet = m_CQualCmd.CmdPartition(lpbyte,unsize);
		else
			nRet = m_CQualCmd.CmdSpeedUpPartition(lpbyte,unsize);
		cfpartition.Close();
		delete [] lpbyte;
		nRet = RUN_SUCCESSFULLY;
	}
	if(nRet <0)
	{
		if(m_pQualcommDiag)
			m_pQualcommDiag->SetLastError(DL_ERROR_Streaming_Partition);
	}
	else
	{
		if(m_CallBack)
			m_CallBack("Streaming Download", 5, (100/6)*3);
	}
	return nRet;
}

int CQDOperator::OpenMutilImage(BYTE byImageType, bool bSpeedUp, const CString& szfilename)
{
	UINT unsize = 0;
	BYTE* lpbyte = NULL;
	int nRet = RUN_SUCCESSFULLY;
	if(byImageType==Image_type_OEMSECBL || byImageType==Image_type_AMSSMODEL || byImageType==Image_type_AMSSAPP)
	{
		CFile cfheader;
		CFileException pError;
		if(cfheader.Open(szfilename,CFile::shareDenyNone,&pError) == TRUE)
		{
			unsize = (UINT)cfheader.GetLength();
			lpbyte = new BYTE[unsize+1];
			cfheader.Read(lpbyte,unsize);
			cfheader.Close();
		}
		else
		{
			nRet = RUN_FAILED;
		}
	}
	if(nRet == RUN_SUCCESSFULLY)
	{
		if(!bSpeedUp)
			nRet = m_CQualCmd.CmdOpenMutiImage(byImageType,lpbyte,unsize);
		else
			nRet = m_CQualCmd.CmdSpeedUpOpenMutiImage(byImageType,lpbyte,unsize);
		delete [] lpbyte;
		lpbyte = NULL;
	}
	return nRet;
}

int CQDOperator::OpenHexFile(const CString& szfilename)
{
	int nRet = RUN_FAILED;
	CFileException pError;
	if(TRUE == m_chexfile.Open((LPCTSTR)szfilename,CFile::modeRead | CFile::shareDenyNone,&pError))
	{
		m_dwaddress = m_chexfile.GetHexAddrss();//Get the first address that Hex File Setting
		DWORD waddress = m_dwaddress;
		waddress <<= 16;
		WORD dwsize = 0;
		do {
			BYTE bytemp[1024] = {0};
			waddress += dwsize;
			dwsize = m_chexfile.Read((char*)bytemp, 0X03F9); // 0x03F9 = 1017 
			nRet = m_CQualCmd.CmdWrite32bit(bytemp,dwsize,waddress);
		} while(dwsize == 0X03F9 && nRet >= 0);
		m_chexfile.Close();
	}
	LogFunc("Download HEX File", nRet);
	if(nRet <0)
	{	
		if(m_pQualcommDiag)
			m_pQualcommDiag->SetLastError(DL_ERROR_DMSS_Hex);
	}
	else
	{
		if(m_CallBack)
			m_CallBack("DMSS Download", 4, (100/4)*3);
	}
	return nRet;
}

bool CQDOperator::OpenandWriteAllImage(int& nRet,bool bSpeedUp)
{
	bool bRes = false;

	if(((nRet = OpenandWriteoneImage("QCSBLHD", Image_type_QCSECBLHD,m_szQcsblhd, bSpeedUp))							 >=0 ) &&
	   ((nRet = OpenandWriteoneImage("QCSBL",   Image_type_QCSECBL,  m_szQcsbl,   bSpeedUp))							 >=0 ) &&
	   ((nRet = OpenandWriteoneImage("OEMSBLHD & OEMSBL", Image_type_OEMSECBL,m_szOemsbl,bSpeedUp, m_szOemsblhd, true))	 >=0 ) &&
	   ((nRet = OpenandWriteoneImage("AMSSHD & AMSS", Image_type_AMSSMODEL,m_szAmss,bSpeedUp, m_szAmsshd, true))			 >=0 ) &&
	   ((nRet = OpenandWriteoneImage("CEFS", Image_type_CEFS, m_szCEFS,bSpeedUp))										 >=0 ))
	{
		bRes = true;
		if(m_CallBack)
			m_CallBack("Streaming Download", 5, 100);
	}
	
	if(!bRes)
	{
		if(m_pQualcommDiag)
			m_pQualcommDiag->SetLastError(DL_ERROR_Streaming_Image);
	}
	
	return bRes;
}

double GetProgress(const std::string& strMsg)
{
	double nProgress = 0;
	if(strMsg == "QCSBLHD")
		nProgress = (double)1/5;
	else if(strMsg == "QCSBL")
		nProgress = (double)2/5;
	else if(strMsg == "OEMSBLHD & OEMSBL")
		nProgress = (double)3/5;
	else if(strMsg == "AMSSHD & AMSS")
		nProgress = (double)4/5;
	else if(strMsg == "CEFS")
		nProgress = (double)5/5;
	return nProgress;
}

int CQDOperator::OpenandWriteoneImage(const std::string& strMsg, BYTE byImageType, const CString& szImageFile , bool bSpeedUp,const CString& szHeaderFile, bool bForce)
{
	int nRet = RUN_SUCCESSFULLY;
	TRACE_FILE_LINE_INFO();
	std::string strMsgTmp = strMsg;
	strMsgTmp += _T("###################\n");
	TRACE(strMsgTmp.c_str());
	double nProgress = 0;
	
	
	if(!szImageFile.IsEmpty())
	{
		if(!bForce || (bForce && !szHeaderFile.IsEmpty()))
		{
			TRACE(("Open " + strMsg).c_str(), nRet);
			nRet = OpenMutilImage(byImageType, bSpeedUp,szHeaderFile);
			
			LogFunc(("[Streaming Download]Open " + strMsg).c_str(), nRet, 5, (100/6)*4*GetProgress(strMsg)*1/3);
		
			if(nRet >=0)
			{
				TRACE(("Write " + strMsg).c_str(), nRet);
				nRet = WriteImagetoPhone(szImageFile, bSpeedUp, (100/6)*4*GetProgress(strMsg));
				LogFunc(("[Streaming Download]Write " + strMsg).c_str(), nRet, 5, (100/6)*4*GetProgress(strMsg)*2/3);
			}
			if(nRet >=0)
			{	
				nRet = m_CQualCmd.CmdCloseImage(bSpeedUp);
				LogFunc(("[Streaming Download]Close " + strMsg).c_str(), nRet, 5, (100/6)*4*GetProgress(strMsg));

			}
		}
	}
	return nRet;
}

int CQDOperator::BackupBRT(CString& strErrorNVItemList) //csv->brt
{
	int nRet = RUN_FAILED;
	TRACE_FILE_LINE_INFO();
	CStdioFile nvfile;
	int nNVCount = 0;
	if( TRUE == nvfile.Open(m_szCsv,CFile::shareDenyNone | CFile::modeRead) )
	{
		std::vector<CNV_ItemHolder> vNVItem;
		
		CString strLine;
		nvfile.ReadString(strLine);
		
		while ( nvfile.ReadString(strLine) )
		{
			int nNVItemID = -1;
			BYTE nStatus = -1;
			nRet = LineToNVItem(strLine, vNVItem,nNVItemID,nStatus);
			if ( nRet >= 0 )
				nNVCount++;
			else
			{
				if(nRet == RUN_FAILED)
				{
					TCHAR szTemp[50] = {0};
					::_stprintf(szTemp,_T("[NV_%04d_%02d]\r\n"),nNVItemID,nStatus);
					strErrorNVItemList += szTemp;
				}
			}
		}
		nvfile.Close();

		char cpMsg[512] = {0};
		sprintf_s( cpMsg, 512, "Total NvCounts in CSV file %d has backuped", nNVCount);
		if(m_CallBackFunc)
			m_CallBackFunc(cpMsg, 0);

		if(WriteBrt(nNVCount, vNVItem))
			nRet = RUN_SUCCESSFULLY; 
	}
	return nRet;
}

int CQDOperator::LineToNVItem(const CString& strLine, std::vector<CNV_ItemHolder>& vNVItem,int& nNVItemID,BYTE& nStatus)
{
	int nRet = -99;//Unknow

	if(!strLine.IsEmpty())
	{
		CString strToken;
		int npos = 0;	
		// Find the NV name using the delimiter ";"
		strToken = strLine.Tokenize(_T(";"),npos);
		if ( -1 != npos)
		{
			CNV_ItemHolder objCurNV;
			objCurNV.SetName(strToken);

			// Find the NV ID using the delimiter ";"
			strToken = strLine.Tokenize(_T(";"),npos);
			if ( -1 != npos)
			{
				objCurNV.SetNV_ID(strToken);
				nNVItemID = objCurNV.GetNV_ID();
				WORD wNVid = atoi(strToken.GetBuffer());

				DIAGReadNVcmdRequest Request = {0};
				DIAGNVcmdResponse Response = {0};

				Request.nDIAGcmd= DIAG_CMD_ReadNVitem;  
				memcpy( Request.nNVitem, &wNVid, sizeof(Request.nNVitem) );
				Request.nStatus[0] = 0x03;
				Request.nStatus[1] = 0x18;
				nRet = m_CQualCmd.CmdDiagTxRx((char*)&Request, sizeof(DIAGReadNVcmdRequest), (char*)&Response, sizeof(DIAGNVcmdResponse));
				nStatus = Response.nStatus[0];

				if(nRet >= 0 && Response.nStatus[0] == 0)
				{
					objCurNV.SetData(Response.nItemData, 128);
					vNVItem.push_back(objCurNV);
				}
				else
				{
					nRet = RUN_FAILED;
				}
			}
			else
			{//strToken = strLine.Tokenize(_T(";"),npos);
				nRet = -2;
			}
		}
		else
		{//strToken = strLine.Tokenize(_T(";"),npos);
			nRet = -3;
		}
	}
	return nRet;
}


bool CQDOperator::WriteBrt(int nNVCount, std::vector<CNV_ItemHolder>& vNVItem)
{
	bool bRes = false;
	CFile oFile;
	if (vNVItem.size()>0 && TRUE == oFile.Open(m_szBrt,CFile::modeCreate | CFile::modeWrite))
	{
		int iVal=0;
		oFile.Write(&nNVCount,sizeof(int));
		for (std::vector<CNV_ItemHolder>::iterator iter = vNVItem.begin(); iter!=vNVItem.end(); iter++)
		{
			iVal = (*iter).GetNV_ID();
			oFile.Write(&iVal,sizeof(int)); //nv id
			iVal = (*iter).GetSize();
			oFile.Write(&iVal,sizeof(int)); //item size
			oFile.Write( (*iter).GetData(), (*iter).GetSize() ); //item data
		}
		oFile.Close();
		bRes = true;
	}
	return bRes;
}

int CQDOperator::CountCSVLines(CFile& nvfile)
{
	int nLineCount = 0;
	CString strTmp;
	CArchive arch(&nvfile,CArchive::load);
	while (arch.ReadString(strTmp))
		nLineCount++;
	arch.Close();
	nvfile.SeekToBegin();
	return nLineCount;
}
int CQDOperator::WriteArrayItem(CNV_ItemHolder & cNV_item)
{
	int nRet = TRUE;

	DIAGWriteNVArraycmdRequest Request;
	DIAGNVArraycmdResponse Response;
	ZeroMemory(&Request, sizeof(DIAGWriteNVArraycmdRequest));
	ZeroMemory(&Response, sizeof(DIAGNVArraycmdResponse));

	Request.nDIAGcmd = DIAG_CMD_WriteNVitem;
	unsigned short NVid = static_cast<short>(cNV_item.GetNV_ID());
	unsigned char* pR = reinterpret_cast<unsigned char*>(&NVid);
	memcpy(Request.nNVitem,pR,2);
	
	int nItemSize = 0;
	unsigned char *begPtr = cNV_item.GetData();
	while( nItemSize < cNV_item.GetSize() && nRet >= 0 )
	{  
		memcpy(Request.nItemData,begPtr,128);
		// Send the SendSync command
		nRet = m_CQualCmd.CmdDiagTxRx((char*)&Request, sizeof(DIAGWriteNVArraycmdRequest), (char*)&Response, sizeof(DIAGNVArraycmdResponse));
		if(Response.nStatus[0] != 0) 
			nRet = RUN_FAILED;
		begPtr += 128;
		nItemSize+=128;
	}
	return nRet;	
}

int CQDOperator::ReadArrayItem(CNV_ItemHolder & cNV_item)
{
	int nRet = RUN_SUCCESSFULLY;
	DIAGReadNVArraycmdRequest Request = {0};
	DIAGNVArraycmdResponse Response = {0};

	Request.nDIAGcmd = DIAG_CMD_ReadNVitem;
	BYTE NVid = static_cast<short>(cNV_item.GetNV_ID());
	BYTE* pR = &NVid;
	memcpy(Request.nNVitem,pR,2);
	
	BYTE *lpcbeg = NULL;
	int nCount = 0;
	int nSize = 0;
	int nfirstdatat = 0;
	do{
		Request.nAddress = nCount;
		nRet = m_CQualCmd.CmdDiagTxRx((char*)&Request, sizeof(DIAGReadNVArraycmdRequest), (char*)&Response, sizeof(DIAGNVArraycmdResponse));
		//LogPolicy::Log(_T("ss"));
		if (( nRet >=0 )&&( Response.nStatus[0] == 0 )){
			BYTE* lpctemp = new BYTE[128+nSize];
			ZeroMemory(lpctemp,128+nSize);
			memcpy(lpctemp, lpcbeg, nSize); // 暫存lpcbeg
			memcpy(lpctemp + nSize, Response.nItemData, 127);
			nfirstdatat = Response.nAddress;
			if(lpcbeg)
			{
				delete [] lpcbeg;
				lpcbeg=NULL;
			}
			lpcbeg = new unsigned char[128+nSize];
			ZeroMemory(lpcbeg, 128 + nSize);
			memcpy(lpcbeg,lpctemp,128+nSize);
			delete [] lpctemp;
			nSize += 128;
			nCount++;
		}
	}while((nRet >= 0) && (Response.nStatus[0] == 0));

	if ( nCount == 0 && (Response.nStatus[0] == 5))  // NV_NOTACTIVE_S = 5, // Variable was not active 
		nRet= 5;
	else if (nCount == 0 &&(Response.nStatus[0] == 6))
		nRet= 0;
	else if (nCount == 1)
	{
		BYTE lpctemp[128] = {0};
		lpctemp[0]=nfirstdatat;
		memcpy(lpctemp+1,lpcbeg,127);
		cNV_item.SetData(lpctemp,128);
	}
	else
		cNV_item.SetData(lpcbeg,nSize);

	delete [] lpcbeg;
	return nRet;
}

void CQDOperator::AddLog(LPCTSTR szMsg)
{
#ifndef DISABLE_LOG
	LogPolicy::Log(QSYNC_LIB::GetAnsi(szMsg).c_str(),m_strLogFile.c_str());
#endif
}

int CQDOperator::RestoreBRT(bool bFilterRealonly)
{
	AddLog(_T("int CQDOperator::Restore(bool bFilterRealonly)"));

	int nRet = RUN_FAILED;
	int nvCount = 0;
	std::vector<CNV_ItemHolder> vNVItem;
	int nCount=0;
	if( ReadBrt(vNVItem) == false)
	{
		AddLog(_T("ReadBrt(vNVItem) == false"));
	}
	else
	{
		for(std::vector<CNV_ItemHolder>::iterator iter = vNVItem.begin(); iter!=vNVItem.end(); iter++)
		{
			if ((*iter).GetSize() > 128) // Qlib
			{
				_ASSERTE(FALSE);
				//nRet = WriteArrayItem((*iter));
				nCount++;
			}
			else
			{
				if(m_pQualcommDiag)
				{
					DIAG_NVITEM_PACKET_F_type req = {0};
					req.item = (*iter).GetNV_ID();
					memcpy(req.item_data, (*iter).GetData(), (*iter).GetSize());
					if(m_pQualcommDiag->WriteNVItem(req, 20000))
						nCount++;
					else
					{
						if(m_pQualcommDiag->WriteNVItem(req, 20000))
							nCount++;
						else
						{
							if(bFilterRealonly && DIAG_ERROR_WRITE_NVITEM_READ_ONLY == m_pQualcommDiag->GetLastError())
								nCount++;
							CString strFormat;
							strFormat.Format(_T("Write %d NV fail.Status = %d"),req.item,m_pQualcommDiag->GetLastError());
							AddLog(strFormat);
						}
					}
					
				}
				else
				{
					DIAGWriteNVcmdRequest Request = {0};
					DIAGNVcmdResponse Response = {0};

					Request.nDIAGcmd = DIAG_CMD_WriteNVitem;
					int nNVID = (*iter).GetNV_ID();
					memcpy(Request.nNVitem, &nNVID, 2);
					memcpy(Request.nItemData, (*iter).GetData(), (*iter).GetSize());
					nRet = m_CQualCmd.CmdDiagTxRx((char*)&Request, sizeof(DIAGWriteNVcmdRequest), (char*)&Response, sizeof(DIAGNVcmdResponse));
					if (nRet >= 0 && Response.nStatus[0] == 0)
						nCount++;
					else
					{
						if(bFilterRealonly && /*NV_READONLY_S*/7 == (int)Response.nStatus)
							nCount++;

						CString strFormat;
						strFormat.Format(_T("Write %d NV item fail.Staus = %d"),Response.nNVitem,Response.nStatus);
						AddLog(strFormat);
					}
				}
				
			}
		}
		char cpMsg[512] = {0};
		sprintf_s( cpMsg, 512, "Total NvCounts in BRT file = %d, %d has restored", nvCount, nCount);
		if(m_CallBackFunc)
			m_CallBackFunc(cpMsg, 0);
		nRet = nCount == vNVItem.size() ? RUN_SUCCESSFULLY : RUN_FAILED;
	}
	return nRet;
}

bool CQDOperator::ReadBrt(std::vector<CNV_ItemHolder>& vNVItem)
{	
	bool bRes = false;
	CFile oFile;
	if (oFile.Open(m_szBrt,CFile::shareDenyNone| CFile::modeRead))
	{
		int iVal;
		int nvCount = 0;
		oFile.Read(&nvCount,sizeof(int));
		for ( int i = 0; i < nvCount; i++ )
		{
			CNV_ItemHolder NVHolder;
			oFile.Read(&iVal,sizeof(int));//nvid
			NVHolder.SetNV_ID(iVal);
			oFile.Read(&iVal,sizeof(int));
			NVHolder.SetSize(iVal);
			oFile.Read(NVHolder.GetData(),iVal);//data
			vNVItem.push_back(NVHolder);
		}
		bRes = true;
	}
	return bRes;
}
bool CQDOperator::ReadBrt(CNV_ItemHolder* pNVlist, int& nvCount)
{	
	bool bRes = false;
	CFile oFile;
	if (oFile.Open(m_szBrt,CFile::shareDenyNone))
	{
		int iVal;
		oFile.Read(&nvCount,sizeof(int));
		pNVlist = new CNV_ItemHolder[nvCount];
		CString str;
		for ( int i = 0; i < nvCount; i++ )
		{
			oFile.Read(&iVal,sizeof(int));//nvid
			pNVlist[i].SetNV_ID(iVal);
			oFile.Read(&iVal,sizeof(int));
			pNVlist[i].SetSize(iVal);
			oFile.Read(pNVlist[i].GetData(),iVal);//data
		}
		bRes = true;
	}
	return bRes;
}


bool CQDOperator::NVArray2Mobile(void)
{
	bool bRes = true;
	int iCount = 0;
	unsigned short _iStatus;

	int lenght = (int)m_vNVArray.size();

	for (int i = 0; i < lenght && bRes; i++)
	{
		if( !(m_CQualCmd.CmdWriteNV(m_vNVArray[i].GetNV_ID(), m_vNVArray[i].GetData(), m_vNVArray[i].GetSize(), &_iStatus)))
		{
			bRes = false;
		}
		else
		{	
			if(_iStatus == 5)	
				m_CQualCmd.CmdWriteNV(m_NVArray[i].GetNV_ID(), m_NVArray[i].GetData(), m_NVArray[i].GetSize(), &_iStatus); 
		}
	}


	/********************************************************************************************************/
	if(bRes)
	{
		int sizeOfBuf;
		const int DATA_SIZE = 133;
		short data_s = DATA_SIZE;
		unsigned char* _Data = new unsigned char[DATA_SIZE];
		unsigned char* _DataR= new unsigned char[DATA_SIZE];

		unsigned char *buf;
		unsigned char *bufFile;
		CString sLocalFile;
		CString sEFSFile;


		lenght = (int)m_vPDPArray.size();
		for(int i=0 ; i<lenght && bRes; i++)
		{ 
			CString strNVArray = m_vPDPArray[i].GetName();
			int iNVA = 0;
			CString strIsArray = strNVArray.Tokenize("#",iNVA);

			if(strIsArray == "ARRAY")//////////////////////////////
			{
				CString strNVId = strNVArray.Tokenize("#",iNVA);

				memset(_Data,NULL,DATA_SIZE);
				memset(_DataR,NULL,DATA_SIZE);
				_Data[0] = 39;
				*((DWORD*)(_Data+1)) = atoi(strNVId);

				sizeOfBuf = m_vPDPArray[i].GetSize();
				buf = m_vPDPArray[i].GetData(); 
				for(int j=0; j<sizeOfBuf;j++)
					_Data[j+3] = buf[j];

				if(m_CQualCmd.CmdDiagTxRx((CHAR*)_Data, data_s, (CHAR*)_DataR, data_s) < 0 )  //data_s, _Data, &data_s, _DataR
				{
					delete[] _Data;
					delete[] _DataR;
					bRes = false;
				}
				else
				{
					_iStatus = _DataR[ data_s - 2 ];

					if(_iStatus == 5)
					{//Need to try again
						data_s = DATA_SIZE;
						m_CQualCmd.CmdDiagTxRx((CHAR*)_Data, data_s, (CHAR*)_DataR, data_s);						
					}
				}
			}//array
			else
			{//file	
				//create sLocalFile
				sEFSFile = m_vPDPArray[i].GetName(); // making name of...
				sLocalFile = sEFSFile.Right(sEFSFile.GetLength()-sEFSFile.ReverseFind('/')-1);  // extract file name from completed path
				sizeOfBuf = m_vPDPArray[i].GetSize();
				bufFile = m_vPDPArray[i].GetData();     // get data from PDP array	   
				i++;	

				if(!m_CQualCmd.CmdDiagEFSWrite(sEFSFile.GetBuffer(), bufFile, sizeOfBuf) )
				{	
					sEFSFile.ReleaseBuffer();				
					CString strTempPath = "\0";
					int iTemp, iTempSize = sEFSFile.GetLength(), iTempFile = sLocalFile.GetLength();

					iTemp = 0;
					while(iTemp < iTempSize - iTempFile)
					{
						strTempPath += sEFSFile.Tokenize("/",iTemp);

						m_CQualCmd.CmdDiagEFSMkDir(strTempPath.GetBuffer(1024));
						strTempPath.ReleaseBuffer();
						strTempPath += "/";
					}				

					if( !m_CQualCmd.CmdDiagEFSWrite(sEFSFile.GetBuffer(), bufFile, sizeOfBuf) )
					{
						sEFSFile.ReleaseBuffer();
					}
				}//end of qlib write
				iCount++;
				sEFSFile.ReleaseBuffer();			
			}//end of else
		}//end of for

		delete[] _Data;
		delete[] _DataR;
	}
	return bRes;
}

/**
* Command handler for command CMD__ARRAY2DUCK. Called every time this command arrives.
* @return true, if all OK, else false (currently not recognized) 
********************************************************************************************/
bool CQDOperator::NVArray2DuckFile(LPCTSTR pstrDuckFile)
{	
	bool bRes = false;
	if(pstrDuckFile == "\0")
		pstrDuckFile = "duck.du";

	FILE *hDuckFile  = fopen(pstrDuckFile,"wb");
	if(hDuckFile != NULL)  // creating a Duck File and open it to writting 
	{				
		int intData=0;
		unsigned char *pBuf;
		// the first for all NV items
		for(int i=0; i < m_NVArray.GetSize();i++) 
		{
			intData = m_NVArray[i].GetNV_ID();              
			fwrite(&intData,sizeof(int),1,hDuckFile);  //write NV ID

			intData = m_NVArray[i].GetSize();
			fwrite(&intData,sizeof(int),1,hDuckFile);  //write Sizeof of Data

			pBuf = m_NVArray[i].GetData();
			fwrite(pBuf,intData,1,hDuckFile);         // write Data

		}

		const int pdpFlag = -1;  
		CString FileName;
		unsigned char *pBufArray;
		unsigned char *pBufFile;

		CString strNVArray;
		CString strIsArray;
		char *fName;

		// the second - for all PDP files 
		for(int i = 0; i < m_PDPArray.GetSize();i++) 
		{
			fwrite(&pdpFlag,sizeof(int),1,hDuckFile);  // start of pdp's part of Duck.du file

			strNVArray = m_PDPArray[i].GetName();
			int iNVA = 0;
			strIsArray = strNVArray.Tokenize("#",iNVA);

			if(strIsArray == "ARRAY")//////////////////////////////
			{
				FileName = m_PDPArray[i].GetName(); // write PDP file's name (with path) 
				intData = static_cast<int>(strlen(FileName)); 

				fwrite(&intData,sizeof(int),1,hDuckFile);  //write size of PDP File's name

				fName = new char[intData]; 
				ZeroMemory(fName, intData);
				//fName = FileName.GetBuffer(intData);    // changing CString to char*
				CopyMemory(fName, FileName.GetBuffer(intData), intData);
				fwrite(fName,intData,1,hDuckFile);     //write File path of PDP file
				FileName.ReleaseBuffer();

				intData = m_PDPArray[i].GetSize();
				fwrite(&intData,sizeof(int),1,hDuckFile);  //write size of Data

				pBufArray = m_PDPArray[i].GetData();
				fwrite(pBufArray,intData,1,hDuckFile);  
			}
			else
			{		
				FileName =  m_PDPArray[i].GetName(); // write PDP file's name (with path) 
				intData = static_cast<int>(strlen(FileName)); 

				fwrite(&intData,sizeof(int),1,hDuckFile);  //write size of PDP File's name

				fName = new char[intData];   
				ZeroMemory(fName, intData);
				//fName = FileName.GetBuffer(intData);    // changing CString to char*
				CopyMemory(fName, FileName.GetBuffer(intData), intData);
				fwrite(fName,intData,1,hDuckFile);     //write File path of PDP file
				FileName.ReleaseBuffer();


				intData = m_PDPArray[i].GetSize();
				fwrite(&intData,sizeof(int),1,hDuckFile);  //write size of Data


				pBufFile = m_PDPArray[i].GetData();
				fwrite(pBufFile,intData,1,hDuckFile);         // write Data

				// save file *.atr to DUCK.DU file
				// every even element of PDPArray contain *.atr content

				i++;  //next PDPArray. element which contain 
				intData = m_PDPArray[i].GetSize();
				fwrite(&intData,sizeof(int),1,hDuckFile);  //write size of Data in *.atr file


				pBufFile = m_PDPArray[i].GetData();
				fwrite(pBufFile,intData,1,hDuckFile);         // write Data of *atr.file

			}
			delete [] fName;
			fName = NULL;

		}
		fclose(hDuckFile);
		bRes = true;
	}
	return bRes;
}

/**
* Command handler for command CMD__DUCK2ARRAY. Called every time this command arrives.
* @return true, if all OK, else false (currently not recognized) 
********************************************************************************************/
bool CQDOperator::DuckFile2NVArray(LPCTSTR pstrDuckFile)
{	
	bool bRes = false;
	m_NVArray.RemoveAll();   //cleaning whole Array before reading new values from file
	m_PDPArray.RemoveAll();   //cleaning whole Array before reading new values from file

	FILE* hDuckFile = fopen(pstrDuckFile,"rb");  // opening Duck File for reading only
	if(hDuckFile)  
	{	
		int intData=0;
		while(fread(&intData,sizeof(int),1,hDuckFile))      //while not end of file; reading value of NV_ID
		{	 
			if (intData != -1) // if not data of PDP file
			{ 
				//NV item
				AddArray(hDuckFile, intData, NULL, m_vNVArray);
			} 
			else // save data of PDP file to PDPArray
			{
				fread(&intData,sizeof(int),1,hDuckFile);  // read size of FileName

				char* filePath = new char[intData+1]; 
				memset(filePath, 0,(intData+1));

				fread(filePath,intData,1,hDuckFile);    // read File name
				
				CString nameOfFile;
				nameOfFile.SetString(filePath);      // change char* to CString
				delete[] filePath;
				//NV ARRAY or NV file
				AddArray(hDuckFile, -1, nameOfFile, m_vPDPArray); 
				
				int nTokenCount = 0;
				if(nameOfFile.Tokenize(_T("#"),nTokenCount) != "ARRAY")
					//ATT file
					AddArray(hDuckFile, -1, NULL, m_vPDPArray);
			}
		}

		fclose(hDuckFile);
		bRes = true;
	} 
	return bRes;
}


bool CQDOperator::AddArray(FILE* hDuckFile, int nNvId, const CString& strName, std::vector<CNV_ItemHolder>& vNVArray)
{
	bool bRes = true;

	CNV_ItemHolder itemHolder;

	itemHolder.SetNV_ID(nNvId); 
	itemHolder.SetName(strName);                // there aren't any file name in NVArray

	int nDataSize;
	fread(&nDataSize,sizeof(int),1,hDuckFile);          // reading value of data's size

	BYTE* pBuf = new BYTE[nDataSize]; 
	memset(pBuf, 0, nDataSize);

	fread(pBuf,nDataSize,1,hDuckFile);                   // reading data
	itemHolder.SetData(pBuf,nDataSize);

	delete[] pBuf;

	vNVArray.push_back(itemHolder);	// add new element to Array

	return bRes;
}

bool CQDOperator::GetHDVersion(int& nPhoneCodeVersion, int& nBootloaderVersion, int nTimeOut)
{
	bool bRes = false;
	int nStatus = -1;
    if(m_CQualCmd.CmdGetPhonecodeRevocation((UCHAR*)&nStatus, (UCHAR*)&nPhoneCodeVersion, nTimeOut)>=0 && nStatus!= 3)
	{
		if(m_CQualCmd.CmdGetBootLoaderRevocation((UCHAR*)&nStatus,(UCHAR*)&nBootloaderVersion, nTimeOut)>=0 && nStatus!= 3)
		{	
			bRes = true;
		}
	}
	return bRes;
}


//////////////////////////////////////////////////////////////////

int CQDOperator::DownloadFunc(void)
{
	int nRet = RUN_FAILED;
	if(Initiate("Start To DL Process", nRet))
	{
		if( ((nRet = m_CQualCmd.ChangeToDLMode()) >= 0) && 
			DMSSDownload(nRet)	    				   &&
			StreamingDownload(nRet))
		{
			Checking(nRet);
		}
	}
	return nRet;
}



int CQDOperator::Flash()
{
	int nRet = RUN_FAILED;
	bool bBackup = false;
	if(!m_szCsv.IsEmpty() && !m_szBrt.IsEmpty())
	{
		TRACE_FILE_LINE_INFO();
		CString strErrorNVItemList;
		bBackup = BackupBRT(strErrorNVItemList) >=0;
		TRACE_FILE_LINE_INFO();
		if(bBackup)
			TRACE("BBBBBBBBBBBackup                 OK!\n");
	}
	if(!m_szHex.IsEmpty() && !m_szPartition.IsEmpty() && !m_szAmss.IsEmpty() && !m_szAmsshd.IsEmpty())
	{
		TRACE_FILE_LINE_INFO();
		if( ((nRet = m_CQualCmd.ChangeToDLMode()) >= 0) && 
			DMSSDownload(nRet)	    				    &&
			StreamingDownload(nRet))
		{
			Checking(nRet);
		}
	}
	if(nRet>=0 && bBackup && !m_szCsv.IsEmpty() && !m_szBrt.IsEmpty())
	{
		nRet = RestoreBRT(true);
	}
	return nRet;
}

bool CQDOperator::Initiate(const char* cpMsg, int& nRet)
{
	bool bRes = false;
	nRet = RUN_FAILED;
	if(_tcscmp(m_CQualCmd.GetComPort(), _T("")) != 0 )
		nRet = m_CQualCmd.OpenComPort();

	LogFunc(cpMsg, nRet);
	bRes = (nRet >= 0);
	return bRes;
}

int CQDOperator::BackupFunc(void)
{
	int nRet = RUN_FAILED;
	if(Initiate("Start To BackupBrt Process", nRet))
	{
		CString strErrorNVItemList;
		if((nRet = m_CQualCmd.ChangeToDiagMode()) >= 0)
			nRet = BackupBRT(strErrorNVItemList);
	}
	m_CQualCmd.CloseComPort();
	return nRet;
}

int CQDOperator::RestoreFunc(bool bFilterRealonly)
{
	int nRet = RUN_FAILED;
	if(Initiate("Start To RestoreBrt Process", nRet))
	{
		if((nRet = m_CQualCmd.ChangeToDiagMode()) >= 0)
			nRet = RestoreBRT(bFilterRealonly);
	}
	m_CQualCmd.CloseComPort();
	return nRet;
}

//int CQDOperator::Restore(void)
//{
//	int nRet = FALSE;
//	CNV_ItemHolder* cNVlist = NULL;
//	int nvCount = 0;
//	if( ReadBrt(cNVlist, nvCount) )
//	{
//		int nCount=0;
//		for (int i = 0; i < nvCount && nRet >0; i++ )
//		{
//			if (cNVlist[i].GetSize() > 128) // Qlib
//			{
//				nRet = WriteArrayItem(cNVlist[i]);
//			}
//			else
//			{
//				DIAGWriteNVcmdRequest Request;
//				DIAGNVcmdResponse Response;
//				ZeroMemory(&Request,sizeof(DIAGWriteNVcmdRequest));
//				ZeroMemory(&Response,sizeof(DIAGNVcmdResponse));
//
//				Request.nDIAGcmd = DIAG_CMD_WriteNVitem;
//				int nNVID = cNVlist[i].GetNV_ID();
//				memcpy(Request.nNVitem, &nNVID, 2);
//				memcpy(Request.nItemData, cNVlist->GetData(), cNVlist->GetSize());
//				nRet = m_CQualCmd.CmdDiagTxRx((char*)&Request, sizeof(DIAGWriteNVcmdRequest), (char*)&Response, sizeof(DIAGNVcmdResponse));
//				if (nRet > 0 && Response.nStatus[0] == 0)
//					nCount++;
//			}
//		}
//		char cpMsg[512] = {0};
//		sprintf_s( cpMsg, 512, "Total NvCounts in BRT file = %d, %d has restored", nvCount, nCount);
//		if(m_CallBackFunc)
//			m_CallBackFunc(cpMsg, 0);
//
//		delete [] cNVlist;
//		cNVlist = NULL;
//	}
//	return nRet;
//}