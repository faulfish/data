// IMEIStation.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "IMEIStation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:7
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


// CIMEIStationApp

BEGIN_MESSAGE_MAP(CIMEIStationApp, CWinApp)
END_MESSAGE_MAP()


// CIMEIStationApp construction

CIMEIStationApp::CIMEIStationApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CIMEIStationApp object

CIMEIStationApp theApp;


// CIMEIStationApp initialization

BOOL CIMEIStationApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


IMEISTATION_API bool SetTestMode(long nComPort, int nTimeout)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""), nComPort);
	bRes = Diag.SetTestMode(nTimeout);
	return bRes;
}

IMEISTATION_API bool WriteIMEI(long nComPort,const char* szIMEI, int nTimeout)
{
	bool bRes = false;
	if(szIMEI && strlen(szIMEI) == 15)
	{
		CQualcommDiag Diag(_T(""), nComPort);
		bRes = Diag.WriteIMEI((char*)szIMEI, nTimeout);
	}
	return bRes;
}

IMEISTATION_API bool ReadIMEI(long nComPort, char* szIMEI, int nTimeout)
{
	bool bRes = false;
	if(szIMEI)
	{
		CQualcommDiag QDiag(_T(""),nComPort);
		bRes = QDiag.ReadIMEI(szIMEI, nTimeout);
	}
	return bRes;
}
IMEISTATION_API bool WriteBTAddress(long nComPort, const char* strBTAddr, int nTimeout)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""), nComPort);
	char szBT[100] = {0};
	memcpy(szBT, strBTAddr, 12);
	bRes = Diag.WriteBTAddr(szBT, nTimeout);
	return bRes;
}

IMEISTATION_API bool ReadBTAddress(long nComPort, char* strBTAddr, int nTimeout)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""), nComPort);
	long nLength = 0;
	bRes = Diag.ReadBTAddr(strBTAddr, nLength, nTimeout);
	return bRes;
}

IMEISTATION_API bool ReadFSN(long nComPort, BYTE* szFSN16,int nTimeout)
{
	bool bRes = false;
	if(szFSN16)
	{
		CQualcommDiag Diag(_T(""), nComPort);
		bRes = Diag.ReadFSN(szFSN16, nTimeout);
	}
	return bRes;
}

IMEISTATION_API bool WriteFSN(long nComPort,const BYTE* szFSN16,int nTimeout)
{
	bool bRes = false;
	if(szFSN16)
	{
		CQualcommDiag Diag(_T(""), nComPort);
		bRes = Diag.WriteFSN((BYTE*)szFSN16, nTimeout);
	}
	return bRes;
}

IMEISTATION_API bool WriteSPC(long nComPort,const char* szLock6,int nTimeout)
{
	bool bRes = false;
	if(szLock6 && strlen(szLock6) == 6)
	{
		CQualcommDiag Diag(_T(""), nComPort);
		bRes = Diag.ProgramSPC((char*)szLock6, nTimeout);
	}
	return bRes;
}

IMEISTATION_API bool LockSPC(long nComPort, int nTimeout)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""), nComPort);
	char szSPC[100] = {0};
	long nSPCStatus = 0;
	if(Diag.ReadSPC(szSPC, nSPCStatus, nTimeout))
	{
		if(szSPC[0] != '1')
			szSPC[0] = '1';
		else
			szSPC[0] = '2';
		bool bUnlock = true;
		if(Diag.UnLockSPC(szSPC, bUnlock, nTimeout))
		{
			bRes = !bUnlock ? true : false;
		}
	}
	else
	{
		if(nSPCStatus == 1)
			bRes = true;
	}
	return bRes;
}

IMEISTATION_API bool ReadSWVersion(long nComPort,char* szReadValue1024, long& nLength, int nTimeout)
{
	bool bRes = false;
	if(szReadValue1024)
	{
		CQualcommDiag Diag(_T(""), nComPort);
		bRes = Diag.ReadSWVersion(szReadValue1024, nLength, nTimeout);
	}
	return bRes;
}

IMEISTATION_API bool FreezeOTP(long nComPort, int nTimeout)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""), nComPort);
	bRes = Diag.FreezeOTP(nTimeout);
	return bRes;
}

IMEISTATION_API bool IsOTPFrozen(long nComPort, bool& bFrozen, int nTimeout)
{
	CQualcommDiag QDiag(_T(""),nComPort);
	bool bRes = QDiag.IsOTPFrozen(nTimeout);
	if(QDiag.GetLastError()!=DIAG_ERROR_TXRX_FAIL && QDiag.GetLastError()!=DIAG_ERROR_CONNECTION_FAIL )
	{
		bFrozen = bRes;
		bRes = true;
	}
	else
	{
		bRes = false;
	}
	return bRes;
}

IMEISTATION_API bool UnlockSPC(long nComport, char* szLock, int nTimeout)
{
	CQualcommDiag QDiag(_T(""),nComport);
	bool bUnlockSuccess = false;
	if(szLock && strlen(szLock) == 6)
		QDiag.UnLockSPC(szLock, bUnlockSuccess, nTimeout);
	return bUnlockSuccess;
}

IMEISTATION_API bool UnlockNW(long nComport, char* szLock, int nTimeout)
{
	CQualcommDiag QDiag(_T(""),nComport);
	bool bUnlockSuccess = false;
	if(szLock && (strlen(szLock) == 8 || strlen(szLock) == 16))
		QDiag.UnLockUnit(0,szLock, bUnlockSuccess, nTimeout);
	return bUnlockSuccess;
}

IMEISTATION_API bool UnlockNS(long nComport, char* szLock, int nTimeout)
{
	CQualcommDiag QDiag(_T(""),nComport);
	bool bUnlockSuccess = false;
	if(szLock && (strlen(szLock) == 8 || strlen(szLock) == 16))
		QDiag.UnLockUnit(1,szLock, bUnlockSuccess, nTimeout);
	return bUnlockSuccess;
}

IMEISTATION_API bool UnlockSP(long nComport, char* szLock, int nTimeout)
{
	CQualcommDiag QDiag(_T(""),nComport);
	bool bUnlockSuccess = false;
	if(szLock && (strlen(szLock) == 8 || strlen(szLock) == 16))
		QDiag.UnLockUnit(2,szLock, bUnlockSuccess, nTimeout);
	return bUnlockSuccess;
}

IMEISTATION_API bool UnlockCP(long nComport, char* szLock, int nTimeout)
{
	CQualcommDiag QDiag(_T(""),nComport);
	bool bUnlockSuccess = false;
	if(szLock && (strlen(szLock) == 8 || strlen(szLock) == 16))
		QDiag.UnLockUnit(3,szLock, bUnlockSuccess, nTimeout);
	return bUnlockSuccess;
}

IMEISTATION_API bool UnlockSIM(long nComport, char* szLock, int nTimeout)
{
	CQualcommDiag QDiag(_T(""),nComport);
	bool bUnlockSuccess = false;
	if(szLock && (strlen(szLock) == 8 || strlen(szLock) == 16))
		QDiag.UnLockUnit(4,szLock, bUnlockSuccess, nTimeout);
	return bUnlockSuccess;
}

IMEISTATION_API bool UnlockPhoneCode(long nComport, char* szLock, int nTimeout)
{
	CQualcommDiag QDiag(_T(""),nComport);
	bool bUnlockSuccess = false;
	if(szLock && (strlen(szLock) == 8 || strlen(szLock) == 16) )
		QDiag.UnLockPhoneCode(szLock,bUnlockSuccess, nTimeout);
	return bUnlockSuccess;
}


IMEISTATION_API bool IsSSCPLocked(long nComPort,long& nStatus, int nTimeOut)
{
	CQualcommDiag QDiag(_T(""),nComPort);
	bool bRes = QDiag.IsUnitLocked(nStatus, 1, nTimeOut);
	return bRes;
}

IMEISTATION_API bool IsNWSCPLocked(long nComPort,
								   long& nNWStatus, // Network Personalization /*0=unlocked, 1=locked*/
								   long& nNSStatus, // Network Subset Personalization /*0=unlocked, 1=locked*/
								   long& nSPStatus, // Service Provider Personalization /*0=unlocked, 1=locked*/
								   long& nCPnStatus, // Corporate Personalization /*0=unlocked, 1=locked*/
								   long& nSIMStatus,// SIM Personalization  /*0=unlocked, 1=locked*/
								   int nTimeOut)
{
	bool bNWUnlocked, bNSUnlocked, bSPUnlocked, bCPUnlocked, bSIMUnlocked;
	CQualcommDiag QDiag(_T(""),nComPort);
	bool bRes = QDiag.IsNWSCPLocked(bNWUnlocked, bNSUnlocked,bSPUnlocked,bCPUnlocked,bSIMUnlocked, nTimeOut);
	nNWStatus = bNWUnlocked ? 0 : 1;
	nNSStatus = bNSUnlocked ? 0 : 1;
	nSPStatus = bSPUnlocked ? 0 : 1;
	nCPnStatus= bCPUnlocked ? 0 : 1;
	nSIMStatus= bSIMUnlocked? 0 : 1;
	return bRes;
}



IMEISTATION_API bool BackupNV(long nComport, int nTimeout)
{
	CQualcommDiag QDiag(_T(""),nComport);	
	bool bRes = QDiag.Backup(nTimeout);
	return bRes;
}

IMEISTATION_API bool RestoreNV(long nComport, int nTimeout)
{
	CQualcommDiag QDiag(_T(""),nComport);	
	bool bRes = QDiag.Restore(nTimeout);
	return bRes;
}

IMEISTATION_API bool WriteSimData(long nComport, BYTE *pSimDataBuf, int nSimDataBufLen, int nTimeOut)
{
	bool bRes = false;
	if(pSimDataBuf)
	{
		CQualcommDiag Diag(_T(""), nComport);
		bRes = Diag.WriteSimData(pSimDataBuf,nSimDataBufLen, nTimeOut);
	}
	return bRes;
}

IMEISTATION_API bool SetOnlineMode(long nComPort, int nTimeout)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""), nComPort);
	bRes = Diag.Restart(nTimeout);
	return bRes;
}

IMEISTATION_API bool ResetHandset(long nComPort,bool bWaitForRestart, int nTimeout)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""), nComPort);
	bRes = Diag.Restart(nTimeout,bWaitForRestart);
	return bRes;
}

IMEISTATION_API bool ReadFactoryArea(long nComport, WORD nNVItem, unsigned char* szFactoryArea128, int& nState, int nTimeOut)
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
IMEISTATION_API bool WriteFactoryArea(long nComport, WORD nNVItem, const unsigned char* szFactoryArea128, int nTimeOut)
{
	bool bRes = false;
	if(szFactoryArea128)
	{
		CQualcommDiag *pDiag = new CQualcommDiag(_T(""), nComport);
		if(pDiag)
		{
			DIAG_NVITEM_PACKET_F_type req = {0};
			req.item = nNVItem;
			for(int i=0; i<128; i++)
			{
				req.item_data[i] = szFactoryArea128[i];
			}
			if(pDiag->WriteNVItem(req, nTimeOut))
				bRes = true;
			delete pDiag;
		}		
	}
	return bRes;
}

IMEISTATION_API bool BackupBrtV2(long nComport, const LPCSTR szCsv, const LPCSTR szBrt,LPSTR szErrorListBuffer,int nBufferSize)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""),nComport);
	if(Diag.IsConnected())
	{
		CQDOperator QDownload(&Diag);
		bool bCsv = QDownload.SetCsv(szCsv) >=0 ? true : false;
		bool bBrt = QDownload.SetBrt(szBrt) >=0 ? true : false;
		if(bCsv && bBrt)
		{
			CString strErrorNVItemList;
			bRes = QDownload.BackupBRT(strErrorNVItemList) >=0 ? true: false;
			if(szErrorListBuffer && nBufferSize > strErrorNVItemList.GetLength())
			{
				::_tcscpy(szErrorListBuffer,(LPCSTR)strErrorNVItemList);
			}
			else
			{
				if(szErrorListBuffer && nBufferSize <= strErrorNVItemList.GetLength())
				{
					if(nBufferSize > 16)
						::strcpy(szErrorListBuffer,"Buffer too small");
				}
			}
		}
	}
	return bRes;
}

IMEISTATION_API bool BackupBrt(long nComport, const LPCSTR szCsv, const LPCSTR szBrt)
{
	return BackupBrtV2(nComport,szCsv,szBrt);
}

IMEISTATION_API bool RestoreBrt(long nComport, const LPCSTR szBrt)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""),nComport);
	if(Diag.IsConnected())
	{
		CQDOperator QDownload(&Diag);
		bool bBrt = QDownload.SetBrt(szBrt) >=0 ? true : false;
		if(bBrt)
			bRes = QDownload.RestoreBRT(false) >=0 ? true: false;
	}
	return bRes;
}

IMEISTATION_API bool IsDLMode(long nComPort)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""),nComPort);
	bRes = Diag.IsDLMode();
	return bRes;
}

IMEISTATION_API bool IsSPCLocked(long nComPort, long& nStatus/*0=unlocked, 1=locked, 2=Unlocked by default code*/, int nTimeOut)
{
	CQualcommDiag QDiag(_T(""),nComPort);
	bool bRes = QDiag.IsSPCLocked(nStatus, nTimeOut);
	return bRes;
}

IMEISTATION_API bool ReadSPC(long nComPort, long& nStatus/*0=unlocked, 1=locked, 2=Unlocked by default code*/, char* szSPC6, int nTimeOut)
{
	bool bRes = false;
	CQualcommDiag QDiag(_T(""),nComPort);
	if(szSPC6)
	{
		bRes = QDiag.ReadSPC(szSPC6, nStatus, nTimeOut);
	}
	return bRes;
}

IMEISTATION_API bool IsConnected(long nComPort)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""),nComPort);
	bRes = Diag.IsConnected();
	return bRes;
}

IMEISTATION_API bool GetReleaseDateTime(long nComPort, char* szDate12, char* szTime9, int nTimeout)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""),nComPort);
	bRes = Diag.GetReleaseDate(szDate12, szTime9, nTimeout);
	return bRes;
}
IMEISTATION_API bool ReadNVItem(long nComport,const WORD& NVItem, BYTE  item_data[128],WORD&  nv_stat)
{
	bool bRes = false;
	CQualcommDiag Diag(_T(""),nComport);
	DIAG_NVITEM_PACKET_F_type resp = {0};
	bRes = Diag.ReadNVItem(NVItem, resp);
	if(bRes)
	{
		if(item_data)
			memcpy(item_data, resp.item_data, 128);
		nv_stat = resp.nv_stat;
	}
	
	return bRes;
}
IMEISTATION_API bool WriteNVItem(long nComport,const WORD& NVItem, const BYTE *WriteData, const size_t szWriteData)
{
	bool bRes = false;
	if(szWriteData <129)
	{
		CQualcommDiag Diag(_T(""),nComport);
		DIAG_NVITEM_PACKET_F_type req = {0};
		req.item = NVItem;
		memcpy(req.item_data, WriteData, szWriteData);
		bRes = Diag.WriteNVItem(req);
	}
	return bRes;
}