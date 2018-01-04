// ODM_Export.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ODM_Export.h"
#include "../../../odmtma_api/QFlash/QZip.h"
#include "../../../Lib\EncodeSIMData\SimEncorderApi.h"
#include "..\..\..\CommonUtil\cloud.h"
#include "../../../CommonUtil/ErrorCode.h"

ODM_API bool D_NewODMExportInterface(IODMExport** piODMExport)
{
	bool bRes = false;
	if(!(*piODMExport))
	{
		*piODMExport = new CODM_Export;
		bRes = true;
	}
	return bRes;
}
ODM_API bool D_DeleteODMExportInterface(IODMExport* piODMExport)
{
	bool bRes = false;
	if(piODMExport)
	{
		delete piODMExport;
		piODMExport = NULL;
		bRes = true;
	}
	return bRes;
}

bool ReadData(TSTRING strPath, CBufferPtr& ptBuffer)
{
	bool bRes = false;
	if(::_taccess(strPath.c_str(), 0) == 0)
	{
		CFile f;
		CFileException pError;
		if(TRUE == f.Open(strPath.c_str(),CFile::modeRead | CFile::shareDenyNone,&pError))
		{
			BYTE* pBuffer = new BYTE[f.GetLength()];
			int nSize = f.Read(pBuffer, f.GetLength());
			ptBuffer = new CBuffer( nSize, (const char*)pBuffer );
			delete [] pBuffer;
			bRes = true;
		}
	}
	return bRes;
}

bool CFileGetter_Melos::SetMot(const TSTRING& strMot)
{
	bool bRes = false;
	if(::_taccess(strMot.c_str(), 0) == 0)
	{
		m_strMot = strMot;
		TSTRING::size_type nDot = strMot.find_last_of(_T("."));
		if(m_strFolderPath != strMot.substr(0, nDot))
		{
			m_strFolderPath = strMot.substr(0, nDot);
			m_ptPartition = NULL;
			m_ptHex = NULL;
			m_ptDBL = NULL;
			m_ptFSBL = NULL;
			m_ptOSBL = NULL;
			m_ptAmss = NULL;
			m_ptCEFS = NULL;
		}
		bRes = true;
	}
	return bRes;
}


bool CFileGetter_Melos::SetFileName(bool bFindAll)
{
	bool bRes = false;

	bool bSetHex =       FindFile(_T("*.hex"),			 		m_strHex);
	bool bSetPartition = FindFile(_T("Partition*.mbn"), 		m_strPartition);
	bool bSetOsbl =      FindFile(_T("osbl*.mbn"),				m_strOSBL);
	bool bSetfsbl =		 FindFile(_T("fsbl*.mbn"),				m_strFSBL);
	bool bSetDBL =		 FindFile(_T("dbl*.mbn"),				m_strDBL);
	bool bSetAmss =		 FindFile(_T("amss*.mbn"),				m_strAmss);

	bool bSetLoc =		 FindFile(_T("*.loc"),					m_strLoc);
	bool bSetCefs =      FindFile(_T("*.qvcefs"),				m_strCEFS);

	if(bFindAll)
	{	
		bRes = bSetHex && bSetPartition
			&& bSetfsbl
			&& bSetDBL
			&& bSetOsbl
			&& bSetAmss && bSetCefs 
			;
	}
	else
	{
		bRes = bSetHex && bSetPartition /*&& bSetAmss*//*&& bSetLoc */&& bSetCefs
			;
	}
	return bRes;
}

const bool CFileGetter_Melos::IsBufferGotten() const
{
	return m_ptHex && m_ptPartition
		&& m_ptOSBL
		&& m_ptAmss 
		&& m_ptFSBL
		&& m_ptDBL
		&& m_ptCEFS
		;
}

const bool CFileGetter_Melos::GetBuffer()
{
	TSTRING strHexPath, strPartitionPath, strOsblPath, strAmssPath, strCEFSPath, strFsblPath, strDBLPath;
	GetHex( strHexPath);
	GetPartition( strPartitionPath);
	GetOSBL(strOsblPath);
	GetAMSS( strAmssPath);
	GetCEFS( strCEFSPath);
	GetFSBL( strFsblPath);
	GetDBL( strDBLPath);
	bool bRes = ReadData(strHexPath, m_ptHex)
		&& ReadData(strPartitionPath, m_ptPartition)
		&&  ReadData(strCEFSPath, m_ptCEFS);

		ReadData(strOsblPath, m_ptOSBL);
		ReadData(strAmssPath, m_ptAmss);
		ReadData(strFsblPath, m_ptFSBL);
		ReadData(strDBLPath, m_ptDBL);
	return bRes;
}

CODM_Export::CODM_Export()
{
	m_PortMap.clear();
}
CODM_Export::~CODM_Export()
{
	for(std::map<long, CODMConf>::iterator iter=m_PortMap.begin(); iter!=m_PortMap.end(); iter++)
	{
		CSingleLock singlelock(&m_CritMap,TRUE);
		m_PortMap.erase(iter);
	}
}
bool CODM_Export::ConnectComPort(long nComport)
{
	bool bRes = false;
	bool bNotFind = m_PortMap.find(nComport) == m_PortMap.end();
	if(bNotFind)
	{
		CCFCDiagCommandPtr ptNVModule = new CCFCDiagCommand(nComport);
		if(ptNVModule->IsConnectinOpen())
		{
			bRes = true;
			CSingleLock singlelock(&m_CritMap,TRUE);
			m_PortMap[nComport].m_ptNVModule = ptNVModule;
			m_PortMap[nComport].m_ptEFSModule = new CCFCEFs(nComport);
			m_PortMap[nComport].m_ptDownloadModule = new CShareDownload6240(nComport);
			m_PortMap[nComport].m_ptQisdaMiscModule = new CQisdaMiscellaneousModule(nComport);
			m_PortMap[nComport].m_ptQisdaSecureACtivateModule = new CCFCOTPRelated(nComport);
			m_PortMap[nComport].m_ptQisdaSimLockModule = new CCFCSimLockRelated(nComport);
		}
	}
	else
	{
		if(m_PortMap[nComport].m_ptNVModule->IsConnectinOpen())
			bRes = true;
	}
	return bRes;
}

bool CODM_Export::DisconnectComPort(long nComport)
{
	bool bRes = true;
	std::map<long, CODMConf>::iterator iter = m_PortMap.find(nComport);
	if(iter != m_PortMap.end())
	{
		CSingleLock singlelock(&m_CritMap,TRUE);
		m_PortMap.erase(iter);
	}
	return bRes;
}

bool CODM_Export::TempDisconnectComPort(long nComport)
{
	bool bRes = true;
	//std::map<long, CODMConf>::iterator iter = m_PortMap.find(nComport);
	//if(iter != m_PortMap.end())
	//{
	//	CSingleLock singlelock(&m_CritMap,TRUE);
	//	m_PortMap.erase(iter);
	//}
	return bRes;
}

void CODM_Export::InitializeParameter(long nComport, bool bAlwaysFlash)
{
	//Now there is nothing to do.
}
bool CODM_Export::SetLastError(long nComport, int nError)
{
	CString strErrorKey;
	strErrorKey.Format(_T("%s%d"), g_szCFCErrorCodeTitle, nComport);

	CStringA cstrError;
	cstrError.Format("%d", nError);
	bool bRes = CCloud::Put((TSTRING)strErrorKey, cstrError.GetLength(), cstrError.GetBuffer());
	cstrError.ReleaseBuffer();

	return bRes;
}

bool CODM_Export::SetTestMode(long nComport)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
	{
		if(m_PortMap[nComport].m_ptNVModule->IsDLMode())
			SetLastError(nComport, DIAG_ERROR_DLMODE);
		else
		{
			if(m_PortMap[nComport].m_ptNVModule->IsEMU(20000))
				bRes = true;
			else
				bRes = m_PortMap[nComport].m_ptNVModule->IsTestMode(20000);
		}
	}
	return bRes;
}
bool CODM_Export::Restart(long nComport)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes =m_PortMap[nComport].m_ptNVModule->Restart();
	return bRes;
}
bool CODM_Export::Reset(long nComport)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes =m_PortMap[nComport].m_ptNVModule->Reset();
	return bRes;
}
bool CODM_Export::ReadIMEI(long nComport, char* szIMEI100)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes =m_PortMap[nComport].m_ptNVModule->ReadIMEI(szIMEI100, 5000);
	return bRes;
}
bool CODM_Export::ProgramIMEI(long nComport, char* szIMEI)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptQisdaSecureACtivateModule && m_PortMap[nComport].m_ptNVModule)
	{
		if(szIMEI)
		{
			if(m_PortMap[nComport].m_ptQisdaSecureACtivateModule->IsOTPFrozen())
			{//IMEI exists
				char szIMEI100[100] = {0};
				if(m_PortMap[nComport].m_ptNVModule->ReadIMEI(szIMEI100))
				{
					if(0 == strcmp(szIMEI100, szIMEI))
						bRes = true;
					else
						SetLastError(nComport, DIAG_ERROR_IMEI_MISMATCH);
				}
			}
			else
			{
				if(GetLastError(nComport) == DIAG_ERROR_EMPTY_OTP)
				{
					BYTE szFSN16[16] = {0};
					char szBTAddr[100] = {0};
					long nLength = 0;
					if( m_PortMap[nComport].m_ptNVModule->GenFSN(szFSN16) &&
						m_PortMap[nComport].m_ptNVModule->ReadBTAddr(szBTAddr, nLength) && strlen(szBTAddr)!=0 &&
						m_PortMap[nComport].m_ptNVModule->WriteIMEI(szIMEI, 8000) &&
						m_PortMap[nComport].m_ptNVModule->WriteFSN(szFSN16, 8000))
					{
						m_PortMap[nComport].m_ptQisdaSecureACtivateModule->FreezeOTP();
						Sleep(1000);
						bRes = m_PortMap[nComport].m_ptQisdaSecureACtivateModule->IsOTPFrozen();
					}
				}
			}
		}
		else
			SetLastError(nComport, DIAG_ERROR_INVALID_FUNCTION_PARAMETER);
	}
	return bRes;
}
bool CODM_Export::WriteSimData(long nComport)
{
	bool bRes = false;

	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptQisdaSecureACtivateModule && m_PortMap[nComport].m_ptNVModule)
	{
		if( m_PortMap[nComport].m_ptQisdaSecureACtivateModule->IsOTPFrozen())
		{
			const int nSimDataSize = 576;
			char szIMEI100[100] = {0};
			BYTE szFSN16[16] = {0};
			BYTE szSimData[nSimDataSize] = {0}; //Melos is 576
			if(m_PortMap[nComport].m_ptNVModule->ReadIMEI(szIMEI100) && 
				m_PortMap[nComport].m_ptNVModule->ReadFSN(szFSN16) &&
				GenSimData(nComport, szIMEI100, szFSN16, szSimData, nSimDataSize) &&
				m_PortMap[nComport].m_ptQisdaSimLockModule->WriteSimData(szSimData, nSimDataSize, 20000))
			{
				long nStatus = 0;
				for(int i=0; i<5 && !bRes; ++i)
				{
					Sleep(5000);
					m_PortMap[nComport].m_ptQisdaSimLockModule->QuerySimDataStatus(bRes);
				}
				if( !bRes )
					SetLastError(nComport, DIAG_ERROR_WRITESIMDATA);
			}
		}
		else
			SetLastError(nComport, DIAG_ERROR_OTP_UNFREEZED);
	}
	return bRes;
}
bool CODM_Export::ReadBTAddr(long nComport,char* szAddress100, long& nLength)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes = m_PortMap[nComport].m_ptNVModule->ReadBTAddr(szAddress100, nLength);
	return bRes;
}
bool CODM_Export::WriteBTAddr(long nComport,char* szAddress)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes = m_PortMap[nComport].m_ptNVModule->WriteBTAddr(szAddress);
	return bRes;
}
bool CODM_Export::IsUnitOn(long nComport)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes = m_PortMap[nComport].m_ptNVModule->IsUnitOn();
	return bRes;
}
bool CODM_Export::IsUnitOff(long nComport)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes = m_PortMap[nComport].m_ptNVModule->IsUnitOff();
	return bRes;
}
bool CODM_Export::TurnUnitOff(long nComport)
{//Close connection, unit won't receive any command.
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
	{
		Sleep(4000); // sleep for previous command(Restart) in 71M
		if(bRes = m_PortMap[nComport].m_ptNVModule->CloseConnection(10000))
			DisconnectComPort(nComport);
	}
	return bRes;
}
bool CODM_Export::ReadFlexVersion(long nComport, char* szReadValue1024, long& nLength)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule && m_PortMap[nComport].m_ptQisdaSimLockModule)
	{
		char szFlexVersion[1024] = {0};
		long nFlexVersionLength = 0;
		if (bRes = m_PortMap[nComport].m_ptNVModule->ReadFlexVersion(szFlexVersion, nFlexVersionLength))
			strncpy(szReadValue1024, szFlexVersion, nFlexVersionLength);
	}
	return bRes;
}
bool CODM_Export::ReadSWVersion(long nComport, char* szReadValue1024, long& nLength)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes = m_PortMap[nComport].m_ptNVModule->ReadSWVersion(szReadValue1024, nLength);
	return bRes;
}
bool CODM_Export::ReadTransceiverKitNumber(long nComport,  char* szTransceiverKitNumber100)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes = m_PortMap[nComport].m_ptNVModule->ReadTransceiverKitNumber(szTransceiverKitNumber100, 5000);
	return bRes;
}
bool CODM_Export::WriteTransceiverKitNumber(long nComport,  char* szTransceiverKitNumber100)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes = m_PortMap[nComport].m_ptNVModule->WriteTransceiverKitNumber(szTransceiverKitNumber100);
	return bRes;
}
bool CODM_Export::ReadTrackID(long nComport,  char * szTrackID)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes = m_PortMap[nComport].m_ptNVModule->ReadTrackID(szTrackID);
	return bRes;
}
bool CODM_Export::WriteTrackID(long nComport,  char * szTrackID)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes = m_PortMap[nComport].m_ptNVModule->WriteTrackID(szTrackID);
	return bRes;
}
bool CODM_Export::ReadFactoryInforBlock(long nComport, char* szBlock, long& nLength)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes = m_PortMap[nComport].m_ptNVModule->ReadFactoryInforBlock(szBlock, nLength, 5000); // Why 5s?
	return bRes;
}
bool CODM_Export::WriteFactoryInforBlock(long nComport, char* szBlock)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes = m_PortMap[nComport].m_ptNVModule->WriteFactoryInforBlock(szBlock);
	return bRes;
}
bool CODM_Export::SetInFactoryFlag(long nComport, bool bOpen)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes = m_PortMap[nComport].m_ptNVModule->TurnOnOffEM(bOpen);
	return bRes;
}
bool CODM_Export::MasterClear(long nComport)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes = m_PortMap[nComport].m_ptNVModule->MasterClear();
	return bRes;
}
bool CODM_Export::IsNWSCPLocked(long nComport,bool& bNWUnlocked, bool& bNSUnlocked, bool& bSPUnlocked, bool& bCPUnlocked, bool& bSIMUnlocked)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptQisdaSimLockModule)
	{
		bool bSimDataExist = true;
		if(m_PortMap[nComport].m_ptQisdaSimLockModule->QuerySimDataStatus(bSimDataExist) && bSimDataExist)
			bRes = m_PortMap[nComport].m_ptQisdaSimLockModule->IsNWSCPLocked(bNWUnlocked, bNSUnlocked, bSPUnlocked, bCPUnlocked, bSIMUnlocked);
		else
		{
			if(!bSimDataExist)
			{
				bNWUnlocked = true;
				bNSUnlocked = true;
				bSPUnlocked = true;
				bCPUnlocked = true;
				bSIMUnlocked = true;
				bRes = true;
			}
		}
	}
	return bRes;
}
bool CODM_Export::IsSPCLocked(long nComport, long& nStatus)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes = m_PortMap[nComport].m_ptNVModule->IsSPCLocked(nStatus);
	return bRes;
}
bool CODM_Export::IsUnitLocked(long nComport, long& nStatus, int nLockType/*0=NWSCP, 1=SSCP*/)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptQisdaSimLockModule)
	{
		bool bSimDataExist = true;
		if(m_PortMap[nComport].m_ptQisdaSimLockModule->QuerySimDataStatus(bSimDataExist) && bSimDataExist)
		{
			if (0 == nLockType)//NWSCP
				bRes = m_PortMap[nComport].m_ptQisdaSimLockModule->IsNWSCPLocked(nStatus);
			else if (1 == nLockType)//SSCP
				bRes = m_PortMap[nComport].m_ptQisdaSimLockModule->IsSSCPLocked(nStatus);
			else
				assert(0);//None defined.
		}
		else
		{
			if(!bSimDataExist)
			{
				nStatus = 0;
				bRes = true;
			}
		}
	}
	return bRes;
}

bool CODM_Export::LockSPC(long nComport)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes = m_PortMap[nComport].m_ptNVModule->LockSPC();
	return bRes;
}

bool CODM_Export::UnlockSPC(long nComport, char* szLock, bool& bUnlockOK)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes = m_PortMap[nComport].m_ptNVModule->UnLockSPC(szLock, bUnlockOK);
	return bRes;
}

bool CODM_Export::ProgramSPC(long nComport, char* szLock)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes = m_PortMap[nComport].m_ptNVModule->ProgramSPC(szLock);
	return bRes;
}

bool CODM_Export::UnLockNWSCP(long nComport, int nLockType/*0=NW, 1=NS, 2=SP, 3=CP, 4=SIM*/, char* szLock, bool& bUnlockOK)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptQisdaSimLockModule)
	{
		if (0 == nLockType) //NW
			bRes = m_PortMap[nComport].m_ptQisdaSimLockModule->UnlockNW(szLock, bUnlockOK);
		else if (1 == nLockType) //NS
			bRes = m_PortMap[nComport].m_ptQisdaSimLockModule->UnlockNS(szLock, bUnlockOK);
		else if (2 == nLockType) //SP
			bRes = m_PortMap[nComport].m_ptQisdaSimLockModule->UnlockSP(szLock, bUnlockOK);
		else if (3 == nLockType) //CP
			bRes = m_PortMap[nComport].m_ptQisdaSimLockModule->UnlockCP(szLock, bUnlockOK);
		else if (4 == nLockType) //SIM
			bRes = m_PortMap[nComport].m_ptQisdaSimLockModule->UnlockSIM(szLock, bUnlockOK);
		else //default NW
			bRes = m_PortMap[nComport].m_ptQisdaSimLockModule->UnlockNW(szLock, bUnlockOK);
	}
	return bRes;
}
bool CODM_Export::ProgramNWSCP(long nComport, char* szLock)
{//Just set NWSCP, not write into unit.
	bool bRes = false;
	if(szLock)
	{
		CSingleLock singlelock(&m_CritMap,TRUE);
		m_PortMap[nComport].m_strNWSCP = szLock;
		bRes = true;
	}
	return bRes;
}
bool CODM_Export::UnlockSSCP(long nComport, char* szLock, bool& bUnlockOK)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptQisdaSimLockModule)
	{
		long nStatus = 0;
		if(IsUnitLocked(nComport, nStatus, 1) && nStatus!=0)
			bRes = m_PortMap[nComport].m_ptQisdaSimLockModule->UnlockSSCP(szLock, bUnlockOK);
	}
	return bRes;
}
bool CODM_Export::ProgramSSCP(long nComport, char* szLock)
{
	bool bRes = true;
	if (szLock && strcmp(szLock, m_PortMap[nComport].m_strNWSCP.c_str()) == 0)
	{
		SetLastError(nComport, DIAG_ERROR_SSCP_NWSCP_SAME);
		bRes = false;
	}

	if(szLock && bRes)
	{
		CSingleLock singlelock(&m_CritMap,TRUE);
		m_PortMap[nComport].m_strSSCP = szLock;
	}

	return bRes;
}
bool CODM_Export::SetRTC(long nComport)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes = m_PortMap[nComport].m_ptNVModule->SetRTC();
	return bRes;
}

bool CODM_Export::Flash(long nComport, char* szMotPath)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptDownloadModule)
	{
		bool bUnziped = true;
		bool bIsBufferGotten = false;
		bool bCompareVersion = false;
		bool bSetLockName = false;
		if(m_FileManager.SetMot(szMotPath))
		{	
			CSingleLock singlelock(&m_CritUnZip,TRUE);
			bIsBufferGotten = m_FileManager.IsBufferGotten();
			if(bIsBufferGotten == false)
			{
				bool bSetFileName = m_FileManager.SetFileName(true);
				if(!bSetFileName)
				{
					ODMTMA_LIB::CQZip QZip;
					QZip.SetFolderPathFromMot(szMotPath);
					bUnziped = QZip.UnzipWithoutExe(szMotPath, _T(""));
					bSetFileName = m_FileManager.SetFileName(false);
				}
				if(bSetFileName)
					bIsBufferGotten = m_FileManager.GetBuffer();
				if(bIsBufferGotten == false)
					AddLog(nComport, "get buffer fail.");
			}
			bool bDownloadOEMSBL = false;
			if(bUnziped && bIsBufferGotten)
			{
				bCompareVersion = CompareVersion(nComport, m_FileManager, bDownloadOEMSBL);
			}
		}

		if (
			bIsBufferGotten
			&& m_PortMap[nComport].m_ptDownloadModule->SetHex((const BYTE*)m_FileManager.m_ptHex->GetData(), m_FileManager.m_ptHex->GetLength())
			&& m_PortMap[nComport].m_ptDownloadModule->SetPartition((const BYTE*)m_FileManager.m_ptPartition->GetData(), m_FileManager.m_ptPartition->GetLength())
			/*&& m_PortMap[nComport].m_ptDownloadModule->SetDBL((const BYTE*)m_FileManager.m_ptDBL->GetData(), m_FileManager.m_ptDBL->GetLength())
			&& m_PortMap[nComport].m_ptDownloadModule->SetFSBL((const BYTE*)m_FileManager.m_ptFSBL->GetData(), m_FileManager.m_ptFSBL->GetLength())
			&& m_PortMap[nComport].m_ptDownloadModule->SetOSBL((const BYTE*)m_FileManager.m_ptOSBL->GetData(), m_FileManager.m_ptOSBL->GetLength())
			*/&& m_PortMap[nComport].m_ptDownloadModule->SetAMSS((const BYTE*)m_FileManager.m_ptAmss->GetData(), m_FileManager.m_ptAmss->GetLength())
			&& m_PortMap[nComport].m_ptDownloadModule->SetCEFS((const BYTE*)m_FileManager.m_ptCEFS->GetData(), m_FileManager.m_ptCEFS->GetLength())
			)
		{
			bRes = m_PortMap[nComport].m_ptDownloadModule->CFCDownload();
			if(!bRes)
			{
				SetLastError(nComport,  m_PortMap[nComport].m_ptDownloadModule->GetCFCLastError());
			}
			else
			{
				bool bRestoreNV = false;
				for (int i = 0; !bRestoreNV && i < 3; ++i)
				{
					Sleep(500);
					bool bNVRestore = false;
					bool bCNVRestore = false;
					bRestoreNV = m_PortMap[nComport].m_ptNVModule->GetMelosRestoreNVStatus(bNVRestore, bCNVRestore)&& bNVRestore && bNVRestore;
				}
				bRes = bRestoreNV;
				if(bRes)
					bRes = SetLocName(nComport, m_FileManager);
			}
		}
		else
		{
			if(!bUnziped)
				SetLastError(nComport, DL_ERROR_UNZIP);
			else if(!bIsBufferGotten)
				SetLastError(nComport, DL_ERROR_MISSFILES);
			else if(!bCompareVersion)
				SetLastError(nComport, DL_ERROR_CHECKVERSION);
			else
				SetLastError(nComport, DL_ERROR_SETFILENAME);
		}
	}else
		SetLastError(nComport, DIAG_ERROR_CONNECTION_FAIL);

	return bRes;
}
bool CODM_Export::Flex(long nComport, char* szMotPath)
{	
	return false;
}
bool CODM_Export::RestoreNV(long nComport, int nTimeout)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule)
		bRes = m_PortMap[nComport].m_ptNVModule->Restore(nTimeout);
	return bRes;
}
bool CODM_Export::SetLocName(long nComport, CFileGetter& FileManager)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptEFSModule)
	{
		char szLocFileName[200] = {0};
		std::string strLocPath;
		if(m_PortMap[nComport].m_ptEFSModule->GetLocFileName(szLocFileName) && FileManager.GetFolderPath(strLocPath))
		{
			strLocPath += szLocFileName;
			bRes = m_PortMap[nComport].m_ptEFSModule->GetLocFile(strLocPath.c_str());
			if(bRes)
				m_PortMap[nComport].m_str1ffLocPath = strLocPath.c_str();
		}
	}
	if(!bRes)
		SetLastError(nComport, DL_ERROR_GETLOCFILE_FAIL);
	return bRes;
}
bool CODM_Export::CompareVersion(long nComPort, CFileGetter& FileManager, bool& bDownloadOEMSBL)
{//Check the HWVersion to make sure this unit can download this SW,
	bool bRes = false;

	bRes = true; //need to implement later.

	return bRes;
}

bool CODM_Export::GenSimData(long nComport, char* szIMEI100, BYTE* szFSN16, BYTE *pSimDataBuf, int pSimDataBufLen)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_ptNVModule && m_PortMap[nComport].m_ptQisdaSimLockModule)
	{
		if(!m_PortMap[nComport].m_strSSCP.empty() && !m_PortMap[nComport].m_strNWSCP.empty() &&
			!m_PortMap[nComport].m_str1ffLocPath.empty() && szIMEI100 && szFSN16 && pSimDataBuf)
		{
			char szFSN33[33] = {0};
			m_PortMap[nComport].m_ptQisdaSimLockModule->HexBcdToDigit((BYTE*)szFSN16, 16, szFSN33, 33);
			for(int i=0; i<16; i++)
			{//swap
				char cTemp = szFSN33[i*2];
				szFSN33[i*2] = szFSN33[i*2+1];
				szFSN33[i*2+1] = cTemp;
			}

			void *hSimEncHandle = NULL;
			int nResult = 0;

			nResult = ISimEnc_AllocSimEncObj(SIM_TYPE_LAVERNOCK , hSimEncHandle);
			if (nResult == SIM_RET_SUCCESS)
				nResult = ISimEnc_SetImei(hSimEncHandle, szIMEI100, strlen(szIMEI100));

			if (nResult == SIM_RET_SUCCESS)
				nResult = ISimEnc_SetFlashSerialNumber(hSimEncHandle, (char*)szFSN33, strlen((char*)szFSN33));

			if (nResult == SIM_RET_SUCCESS)
				nResult = ISimEnc_SetSimLockFile(hSimEncHandle, (char*)(m_PortMap[nComport].m_str1ffLocPath.c_str()));

			if (nResult == SIM_RET_SUCCESS)
			{
				T_SIM_UNLOCK_CODE tSimUnlockCode;

				tSimUnlockCode.szNetworkCode			= (char*)(m_PortMap[nComport].m_strNWSCP.c_str());
				tSimUnlockCode.szServiceProviderCode	= (char*)(m_PortMap[nComport].m_strNWSCP.c_str());
				tSimUnlockCode.szCorporateCode			= (char*)(m_PortMap[nComport].m_strNWSCP.c_str());
				tSimUnlockCode.szNetworkSubsetCode		= (char*)(m_PortMap[nComport].m_strNWSCP.c_str());
				tSimUnlockCode.szSimCode				= (char*)(m_PortMap[nComport].m_strNWSCP.c_str());
				tSimUnlockCode.szUsimCode = (char*)(m_PortMap[nComport].m_strSSCP.c_str());
				tSimUnlockCode.szReserved				= NULL;

				nResult = ISimEnc_SetUnlockCodes(hSimEncHandle, &tSimUnlockCode);
			}

			if (nResult == SIM_RET_SUCCESS)
			{
				int nSimLockData = 1024;
				BYTE pcSimLockData[1024] = {0};
				nResult = ISimEnc_GenSimLockData(hSimEncHandle, pcSimLockData, &nSimLockData);

				if (nSimLockData != 576/*Lavernock*/)
				{
					assert(0);
					AddLog(nComport, _T("SimData size : %d bytes, it should be 576 bytes."));
				}

				memcpy((void*)pSimDataBuf, (void*)pcSimLockData, sizeof(BYTE)*nSimLockData);
				int nFreeSimEncResult = ISimEnc_FreeSimEncObj(hSimEncHandle);

				bRes = (nResult == SIM_RET_SUCCESS);
			}

			if(!bRes)
			{
				SetLastError(nComport, DIAG_ERROR_GEN_SIMDATA);
				AddLog(nComport, _T("GenSimData fail"));
			}
			else
			{
				CString strGenSimSuccess;
				strGenSimSuccess.Format(_T("GenSimData success: %x"), *pSimDataBuf);
				AddLog(nComport, strGenSimSuccess);
			}
		}
		else
			SetLastError(nComport, DIAG_ERROR_INVALID_FUNCTION_PARAMETER);
	}
	return bRes;
}

void CODM_Export::AddLog(long nComport, const CString& strMessage)
{
#ifndef DISABLE_LOG
	CString strFileName;
	strFileName.Format(_T("ODM_Export_COM%02d.txt"),nComport);
	LogPolicy::Log(strMessage, strFileName);
#endif//#ifndef DISABLE_LOG
}

int  CODM_Export::GetLastError(long nComport)
{
	if(m_PortMap.find(nComport) == m_PortMap.end())
		return 2; //Comport disconnect.

	char szLastError[10] = {0};

	int nSize = 0;
	FILETIME time;
	CString strErrorKey;
	strErrorKey.Format(_T("%s%d"), g_szCFCErrorCodeTitle, nComport);
	if(CCloud::Get((TSTRING)strErrorKey, nSize, NULL, time) && nSize > 0)
		CCloud::Get((TSTRING)strErrorKey, nSize, &szLastError, time);

	return atoi(szLastError);
}
bool CODM_Export::GetLastError(long nComPort,const TCHAR* szModule, char* szErrMsg500)
{
	bool bRes = false;
	if(szModule && szErrMsg500)
	{
		int nError = 1; //Fail
		if(m_PortMap.find(nComPort) == m_PortMap.end())
		{
			if(strcmp(szModule, "NVModule") == 0)
				nError = 2; // Fail in connection
			else if(strcmp(szModule, "Download") == 0)
				nError = 70; // Fail in connection
		}
		else
			nError = GetLastError(nComPort);

		TSTRING strErrorContent = CErrorCode::GetErrorString(szModule, nError);
		CString strErrorCode;
		strErrorCode.Format(_T("COM%d:(%d)%s"), nComPort, nError, strErrorContent.c_str());

#ifdef _UNICODE
		std::string strTmp = QSYNC_LIB::widetoansi((TSTRING)strErrorCode);
		strcpy(szErrMsg500, strTmp.c_str());
#else
		memset(szErrMsg500,0,strErrorCode.GetLength() + 1);
		strcpy(szErrMsg500, strErrorCode);
#endif
		bRes = true;
	}
	return bRes;
}
