// ODM_Export.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ODM_Export.h"
#include "../../../Lib\EncodeSIMData\SimEncorderApi.h"
#include "..\..\..\CommonUtil\cloud.h"
#include "../../../CommonUtil/ErrorCode.h"

ODM_API bool D_NewODMExportInterface(IODMExport** piODMExport)
{
	bool bRes = false;
	if(!(*piODMExport))
	{
		*piODMExport = new CODM_Export;
		if((*piODMExport))
		{
			if((*piODMExport)->IsRSDInitiated())
				bRes = true;
			else
			{
				delete (*piODMExport);
				(*piODMExport) = NULL;
			}
		}
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


CODM_Export::CODM_Export()
{
	m_MotoDLModule.Initialize();
	m_PortMap.clear();
}
CODM_Export::~CODM_Export()
{
	for(std::map<long, CODMConf>::iterator iter=m_PortMap.begin(); iter!=m_PortMap.end(); iter++)
	{
		CSingleLock singlelock(&m_CritMap,TRUE);
		m_PortMap.erase(iter);
	}
	m_MotoDLModule.UnInitialize();
}

bool CODM_Export::IsRSDInitiated()
{
	return m_MotoDLModule.IsInitiated();
}

bool CODM_Export::CreateQisdaModule(long nPort)
{
	bool bConnect = false;
	int nComPort =0;
	AddLog(nPort, "CreateQisdaModule");
	if(CMotoDownload::MotoPortToCOMPort(nPort, nComPort))
	{
		//////////////////////////////////////////////////////////////////////////
		char szTemp[100] = {0};
		AddLog(nPort, _itot(nComPort, szTemp, 10)); // comport
		//////////////////////////////////////////////////////////////////////////
		CCFCDiagCommandPtr ptNVModule = new CCFCDiagCommand(nComPort);
		if(ptNVModule->IsConnectinOpen())
		{
			AddLog(nPort, "NVModule connected");
			bConnect = true;
			CSingleLock singlelock(&m_CritMap,TRUE);
			m_PortMap[nPort].m_nComPort = nComPort;
			m_PortMap[nPort].m_bDLMode = false;
			m_PortMap[nPort].m_ptNVModule = ptNVModule;
			m_PortMap[nPort].m_ptEFSModule = new CCFCEFs(nComPort);

			m_PortMap[nPort].m_ptQisdaMiscModule = new CQisdaMiscellaneousModule(nComPort);
			m_PortMap[nPort].m_ptQisdaSecureActivateModule = new CCFCOTPRelated(nComPort);
			m_PortMap[nPort].m_ptQisdaSimLockModule = new CCFCSimLockRelated(nComPort);
		}
		else
			AddLog(nPort, "NVModule can't send commands");
	}
	return bConnect;
}

bool CODM_Export::TryMotoConnection(long nPort)
{
	bool bRes = false;
	CStringA cstrSymLink;
	bool bDLMode = false;
	if( m_MotoDLModule.IsConnected(nPort, cstrSymLink, bDLMode) )
	{	
		{
			CSingleLock singlelock(&m_CritMap,TRUE);
			m_PortMap[nPort].m_bDLMode = bDLMode;
			m_PortMap[nPort].m_bMotoConnedcted = true; 
		}
		if(bDLMode)
		{
			bRes = true;
			CSingleLock singlelock(&m_CritMap,TRUE);
			m_PortMap[nPort].m_nComPort = 0;
		}
		else
		{
			if(m_PortMap[nPort].m_ptNVModule == NULL)
				bRes = CreateQisdaModule(nPort);
			else
				bRes = true;
		}
	}	
	return bRes;
}
bool CODM_Export::ConnectComPort(long nPort)
{
	bool bRes = false;
	if(IsRSDInitiated())
	{
		bool bNotFind = m_PortMap.find(nPort) == m_PortMap.end();
		if(bNotFind)
		{
			bRes = TryMotoConnection(nPort);
		}
		else
		{
			if(m_PortMap[nPort].m_bMotoConnedcted)
			{
				if(m_PortMap[nPort].m_nComPort == 0)
				{
					bRes = CreateQisdaModule(nPort) || m_PortMap[nPort].m_bDLMode;
				}
				else
				{
					if(m_PortMap[nPort].m_ptNVModule)
						bRes = m_PortMap[nPort].m_ptNVModule->IsConnected();
					else
						bRes = CreateQisdaModule(nPort);
				}
			}
			else
				bRes = TryMotoConnection(nPort);
		}
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
bool CODM_Export::SetLastError(long nPort, int nError)
{
	assert(0);
	CString strErrorKey;
	strErrorKey.Format(_T("%s%d"), g_szCFCErrorCodeTitle, nPort);

	CStringA cstrError;
	cstrError.Format("%d", nError);
	bool bRes = CCloud::Put((TSTRING)strErrorKey, cstrError.GetLength(), cstrError.GetBuffer());
	cstrError.ReleaseBuffer();

	return bRes;
}

bool CODM_Export::IsDiagMode(long nPort)
{
	bool bRes = false;
	
	if(m_PortMap.find(nPort) != m_PortMap.end())
	{
		if(m_PortMap[nPort].m_bDLMode)
			SetLastError(nPort, DIAG_ERROR_DLMODE);
		else
		{
			if(m_PortMap[nPort].m_ptNVModule)
			{
				bRes = 	m_PortMap[nPort].m_ptNVModule->IsConnected();
			}
			else
			{
				bRes = CreateQisdaModule(nPort);
			}
		}
	}
	return bRes;
}

bool CODM_Export::IsUnitOn(long nPort)
{
	bool bRes = false;
	if(m_PortMap[nPort].m_bMotoConnedcted)
	{
		bRes = m_PortMap[nPort].m_bDLMode || (m_PortMap[nPort].m_ptNVModule && m_PortMap[nPort].m_ptNVModule->IsUnitOn());
	}
	return bRes;
}
bool CODM_Export::IsUnitOff(long nPort)
{
	bool bRes = false;
	bRes = !IsUnitOn(nPort);
	return bRes;
}
bool CODM_Export::TurnUnitOff(long nPort)
{//Close connection, unit won't receive any command.
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
	{
		Sleep(4000); // sleep for previous command(Restart) in 71M
		if(bRes = m_PortMap[nPort].m_ptNVModule->CloseConnection(10000))
			DisconnectComPort(nPort);
	}
	return bRes;
}
bool CODM_Export::SetTestMode(long nPort)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
	{
		if(m_PortMap[nPort].m_ptNVModule->IsEMU(20000))
			bRes = true;
		else
			bRes = m_PortMap[nPort].m_ptNVModule->IsTestMode(20000);
	}
	return bRes;
}
bool CODM_Export::Restart(long nPort)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
		bRes =m_PortMap[nPort].m_ptNVModule->Restart();
	return bRes;
}
bool CODM_Export::Reset(long nPort)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
		bRes =m_PortMap[nPort].m_ptNVModule->Reset();
	return bRes;
}
bool CODM_Export::ReadIMEI(long nPort, char* szIMEI100)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
		bRes =m_PortMap[nPort].m_ptNVModule->ReadIMEI(szIMEI100, 5000);
	return bRes;
}
bool CODM_Export::ProgramIMEI(long nPort, char* szIMEI)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptQisdaSecureActivateModule && m_PortMap[nPort].m_ptNVModule)
	{
		if(szIMEI)
		{
			if(m_PortMap[nPort].m_ptQisdaSecureActivateModule->IsOTPFrozen())
			{//IMEI exists
				char szIMEI100[100] = {0};
				if(m_PortMap[nPort].m_ptNVModule->ReadIMEI(szIMEI100) && 0 == strcmp(szIMEI100, szIMEI))
					bRes = true;
				else
					SetLastError(nPort, DIAG_ERROR_IMEI_MISMATCH);
			}
			else
			{
				if(GetLastError(nPort) == DIAG_ERROR_EMPTY_OTP)
				{
					BYTE szFSN16[16] = {0};
					char szBTAddr[100] = {0};
					long nLength = 0;
					if( m_PortMap[nPort].m_ptNVModule->GenFSN(szFSN16) &&
						m_PortMap[nPort].m_ptNVModule->ReadBTAddr(szBTAddr, nLength) && strlen(szBTAddr)!=0 &&
						m_PortMap[nPort].m_ptNVModule->WriteIMEI(szIMEI, 8000) &&
						m_PortMap[nPort].m_ptNVModule->WriteFSN(szFSN16, 8000))
					{
						m_PortMap[nPort].m_ptQisdaSecureActivateModule->FreezeOTP();
						Sleep(1000);
						bRes = m_PortMap[nPort].m_ptQisdaSecureActivateModule->IsOTPFrozen();
					}
				}
			}
		}
		else
			SetLastError(nPort, DIAG_ERROR_INVALID_FUNCTION_PARAMETER);
	}
	return bRes;
}
bool CODM_Export::WriteSimData(long nPort)
{
	bool bRes = false;

	if( IsDiagMode(nPort) && m_PortMap[nPort].m_ptQisdaSecureActivateModule && m_PortMap[nPort].m_ptNVModule)
	{
		if( m_PortMap[nPort].m_ptQisdaSecureActivateModule->IsOTPFrozen())
		{
			const int nLavSimDataSize = 576;
			char szIMEI100[100] = {0};
			BYTE szFSN16[16] = {0};
			BYTE szSimData[nLavSimDataSize] = {0}; //Lavernock is 576
			if(m_PortMap[nPort].m_ptNVModule->ReadIMEI(szIMEI100) && 
				m_PortMap[nPort].m_ptNVModule->ReadFSN(szFSN16) &&
				GenSimData(nPort, szIMEI100, szFSN16, szSimData, nLavSimDataSize) &&
				m_PortMap[nPort].m_ptQisdaSimLockModule->WriteSimData(szSimData, nLavSimDataSize, 20000))
			{
				long nStatus = 0;
				for(int i=0; i<5 && !bRes; ++i)
				{
					Sleep(5000);
					m_PortMap[nPort].m_ptQisdaSimLockModule->QuerySimDataStatus(bRes);
				}
				if( !bRes )
					SetLastError(nPort, DIAG_ERROR_WRITESIMDATA);
			}
		}
		else
			SetLastError(nPort, DIAG_ERROR_OTP_UNFREEZED);
	}
	return bRes;
}
bool CODM_Export::ReadBTAddr(long nPort,char* szAddress100, long& nLength)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
		bRes = m_PortMap[nPort].m_ptNVModule->ReadBTAddr(szAddress100, nLength);
	return bRes;
}
bool CODM_Export::WriteBTAddr(long nPort,char* szAddress)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
		bRes = m_PortMap[nPort].m_ptNVModule->WriteBTAddr(szAddress);
	return bRes;
}

bool CODM_Export::ReadFlexVersion(long nPort, char* szReadValue1024, long& nLength)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule && m_PortMap[nPort].m_ptQisdaSimLockModule)
	{
		char szFlexVersion[1024] = {0};
		long nFlexVersionLength = 0;
		if (bRes = m_PortMap[nPort].m_ptNVModule->ReadFlexVersion(szFlexVersion, nFlexVersionLength))
			strncpy(szReadValue1024, szFlexVersion, nFlexVersionLength);
	}
	return bRes;
}
bool CODM_Export::ReadSWVersion(long nPort, char* szReadValue1024, long& nLength)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
		bRes = m_PortMap[nPort].m_ptNVModule->ReadSWVersion(szReadValue1024, nLength);
	return bRes;
}
bool CODM_Export::ReadTransceiverKitNumber(long nPort,  char* szTransceiverKitNumber100)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
		bRes = m_PortMap[nPort].m_ptNVModule->ReadTransceiverKitNumber(szTransceiverKitNumber100, 5000);
	return bRes;
}
bool CODM_Export::WriteTransceiverKitNumber(long nPort,  char* szTransceiverKitNumber100)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
		bRes = m_PortMap[nPort].m_ptNVModule->WriteTransceiverKitNumber(szTransceiverKitNumber100);
	return bRes;
}
bool CODM_Export::ReadTrackID(long nPort,  char * szTrackID)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
		bRes = m_PortMap[nPort].m_ptNVModule->ReadTrackID(szTrackID);
	return bRes;
}
bool CODM_Export::WriteTrackID(long nPort,  char * szTrackID)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
		bRes = m_PortMap[nPort].m_ptNVModule->WriteTrackID(szTrackID);
	return bRes;
}
bool CODM_Export::ReadFactoryInforBlock(long nPort, char* szBlock, long& nLength)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
		bRes = m_PortMap[nPort].m_ptNVModule->ReadFactoryInforBlock(szBlock, nLength, 5000); // Why 5s?
	return bRes;
}
bool CODM_Export::WriteFactoryInforBlock(long nPort, char* szBlock)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
		bRes = m_PortMap[nPort].m_ptNVModule->WriteFactoryInforBlock(szBlock);
	return bRes;
}
bool CODM_Export::SetInFactoryFlag(long nPort, bool bOpen)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
		bRes = m_PortMap[nPort].m_ptNVModule->TurnOnOffEM(bOpen);
	return bRes;
}
bool CODM_Export::MasterClear(long nPort)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
		bRes = m_PortMap[nPort].m_ptNVModule->MasterClear();
	return bRes;
}
bool CODM_Export::IsNWSCPLocked(long nPort,bool& bNWUnlocked, bool& bNSUnlocked, bool& bSPUnlocked, bool& bCPUnlocked, bool& bSIMUnlocked)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptQisdaSimLockModule)
	{
		bool bSimDataExist = true;
		if(m_PortMap[nPort].m_ptQisdaSimLockModule->QuerySimDataStatus(bSimDataExist) && bSimDataExist)
			bRes = m_PortMap[nPort].m_ptQisdaSimLockModule->IsNWSCPLocked(bNWUnlocked, bNSUnlocked, bSPUnlocked, bCPUnlocked, bSIMUnlocked);
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
bool CODM_Export::IsSPCLocked(long nPort, long& nStatus)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
		bRes = m_PortMap[nPort].m_ptNVModule->IsSPCLocked(nStatus);
	return bRes;
}
bool CODM_Export::IsUnitLocked(long nPort, long& nStatus, int nLockType/*0=NWSCP, 1=SSCP*/)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptQisdaSimLockModule)
	{
		bool bSimDataExist = true;
		if(m_PortMap[nPort].m_ptQisdaSimLockModule->QuerySimDataStatus(bSimDataExist) && bSimDataExist)
		{
			if (0 == nLockType)//NWSCP
				bRes = m_PortMap[nPort].m_ptQisdaSimLockModule->IsNWSCPLocked(nStatus);
			else if (1 == nLockType)//SSCP
				bRes = m_PortMap[nPort].m_ptQisdaSimLockModule->IsSSCPLocked(nStatus);
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

bool CODM_Export::LockSPC(long nPort)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
		bRes = m_PortMap[nPort].m_ptNVModule->LockSPC();
	return bRes;
}

bool CODM_Export::UnlockSPC(long nPort, char* szLock, bool& bUnlockOK)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
		bRes = m_PortMap[nPort].m_ptNVModule->UnLockSPC(szLock, bUnlockOK);
	return bRes;
}

bool CODM_Export::ProgramSPC(long nPort, char* szLock)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
		bRes = m_PortMap[nPort].m_ptNVModule->ProgramSPC(szLock);
	return bRes;
}

bool CODM_Export::UnLockNWSCP(long nPort, int nLockType/*0=NW, 1=NS, 2=SP, 3=CP, 4=SIM*/, char* szLock, bool& bUnlockOK)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
	{
		if (0 == nLockType) //NW
			bRes = m_PortMap[nPort].m_ptQisdaSimLockModule->UnlockNW(szLock, bUnlockOK);
		else if (1 == nLockType) //NS
			bRes = m_PortMap[nPort].m_ptQisdaSimLockModule->UnlockNS(szLock, bUnlockOK);
		else if (2 == nLockType) //SP
			bRes = m_PortMap[nPort].m_ptQisdaSimLockModule->UnlockSP(szLock, bUnlockOK);
		else if (3 == nLockType) //CP
			bRes = m_PortMap[nPort].m_ptQisdaSimLockModule->UnlockCP(szLock, bUnlockOK);
		else if (4 == nLockType) //SIM
			bRes = m_PortMap[nPort].m_ptQisdaSimLockModule->UnlockSIM(szLock, bUnlockOK);
		else //default NW
			bRes = m_PortMap[nPort].m_ptQisdaSimLockModule->UnlockNW(szLock, bUnlockOK);
	}
	return bRes;
}
bool CODM_Export::ProgramNWSCP(long nPort, char* szLock)
{//Just set NWSCP, not write into unit.
	bool bRes = false;
	if(szLock)
	{
		CSingleLock singlelock(&m_CritMap,TRUE);
		m_PortMap[nPort].m_strNWSCP = szLock;
		bRes = true;
	}
	return bRes;
}
bool CODM_Export::UnlockSSCP(long nPort, char* szLock, bool& bUnlockOK)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptQisdaSimLockModule)
	{
		long nStatus = 0;
		if(IsUnitLocked(nPort, nStatus, 1) && nStatus!=0)
			bRes = m_PortMap[nPort].m_ptQisdaSimLockModule->UnlockSSCP(szLock, bUnlockOK);
	}
	return bRes;
}
bool CODM_Export::ProgramSSCP(long nPort, char* szLock)
{
	bool bRes = true;
	if (szLock && strcmp(szLock, m_PortMap[nPort].m_strNWSCP.c_str()) == 0)
	{
		SetLastError(nPort, DIAG_ERROR_SSCP_NWSCP_SAME);
		bRes = false;
	}

	if(szLock && bRes)
	{
		CSingleLock singlelock(&m_CritMap,TRUE);
		m_PortMap[nPort].m_strSSCP = szLock;
	}

	return bRes;
}
bool CODM_Export::SetRTC(long nPort)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
		bRes = m_PortMap[nPort].m_ptNVModule->SetRTC();
	return bRes;
}

bool CODM_Export::Flash(long nPort, char* szMotPath)
{
	bool bRes = false;
	if(m_PortMap.find(nPort) != m_PortMap.end())
	{
		//bool bBackup = m_PortMap[nPort].m_ptNVModule->Backup();
		//SetLocName(nPort, szMotPath); // for test
		QSYNC_LIB::GetDeviceFinder()->StartMonitor();
		AddLog(nPort, "Start to Run Moto DL:");
		if(m_MotoDLModule.RunDLThread(nPort, szMotPath))
		{
			AddLog(nPort, "Run Moto DL Success");
			bool bGoing = false;
			if(m_PortMap[nPort].m_ptNVModule)
			{
				AddLog(nPort, "ReopenComPort");
				bGoing = m_PortMap[nPort].m_ptNVModule->ReopenComPort(30000);	
			}
			else
			{
				Sleep(30000);
				AddLog(nPort, "TryMotoConnection");
				bGoing = TryMotoConnection(nPort) && m_PortMap[nPort].m_ptNVModule;
			}
			if(bGoing && m_PortMap[nPort].m_ptNVModule)
			{
				AddLog(nPort, "TryMotoConnection/ReopenComport succeed");
				bool bCNVSuccess = false;
				bool bNVPartitionSuccess = false;
				if(	m_PortMap[nPort].m_ptNVModule->GetMelosRestoreNVStatus(bNVPartitionSuccess, bCNVSuccess))
				{
					if( bCNVSuccess && bNVPartitionSuccess)
					{
						AddLog(nPort, "GetMelosRestoreNVStatus succeed");
						bRes = SetLocName(nPort, szMotPath);
						if(bRes)
							AddLog(nPort, "SetLocName succeed");
						else
							AddLog(nPort, "SetLocName fail");
					}
					else
						AddLog(nPort, "Restore return fail message");
				}
				else
					AddLog(nPort, "GetMelosRestoreNVStatus fail");
			}
			else
			{
				if(!bGoing)
					AddLog(nPort, "TryMotoConnection/ReopenComport failed");
			}
		}
		else
		{
			AddLog(nPort, "MotoDL fail");
			TryMotoConnection(nPort);
		}
			//set last error
		QSYNC_LIB::GetDeviceFinder()->StopMonitor();
	}
	return bRes;
}
bool CODM_Export::Flex(long nComport, char* szMotPath)
{	
	return false;
}
bool CODM_Export::RestoreNV(long nPort, int nTimeout)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule)
		bRes = m_PortMap[nPort].m_ptNVModule->Restore(nTimeout);
	return bRes;
}
bool CODM_Export::SetLocName(long nPort, char* szMotPath)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptEFSModule)
	{
		char szLocFileName[200] = {0};
		std::string strMot = szMotPath;
		std::string::size_type nDot = strMot.find_last_of(".");
		std::string strLocPath = strMot.substr(0, nDot) + "\\";
		::CreateDirectory(strLocPath.c_str(), NULL);
		if(m_PortMap[nPort].m_ptEFSModule->GetLocFileName(szLocFileName))
		{
			strLocPath += szLocFileName;
			bRes = m_PortMap[nPort].m_ptEFSModule->GetLocFile(strLocPath.c_str());
			if(bRes)
				m_PortMap[nPort].m_str1ffLocPath = strLocPath.c_str();
		}
	}
	return bRes;
}
bool CODM_Export::CompareVersion(long nPort, CFileGetter& FileManager, bool& bDownloadOEMSBL)
{//Check the HWVersion to make sure this unit can download this SW,
	bool bRes = false;

	bRes = true; //need to implement later.

	return bRes;
}

bool CODM_Export::GenSimData(long nPort, char* szIMEI100, BYTE* szFSN16, BYTE *pSimDataBuf, int pSimDataBufLen)
{
	bool bRes = false;
	if(IsDiagMode(nPort) && m_PortMap[nPort].m_ptNVModule && m_PortMap[nPort].m_ptQisdaSimLockModule)
	{
		if(!m_PortMap[nPort].m_strSSCP.empty() && !m_PortMap[nPort].m_strNWSCP.empty() &&
			!m_PortMap[nPort].m_str1ffLocPath.empty() && szIMEI100 && szFSN16 && pSimDataBuf)
		{
			char szFSN33[33] = {0};
			m_PortMap[nPort].m_ptQisdaSimLockModule->HexBcdToDigit((BYTE*)szFSN16, 16, szFSN33, 33);
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
				nResult = ISimEnc_SetSimLockFile(hSimEncHandle, (char*)(m_PortMap[nPort].m_str1ffLocPath.c_str()));

			if (nResult == SIM_RET_SUCCESS)
			{
				T_SIM_UNLOCK_CODE tSimUnlockCode;

				tSimUnlockCode.szNetworkCode			= (char*)(m_PortMap[nPort].m_strNWSCP.c_str());
				tSimUnlockCode.szServiceProviderCode	= (char*)(m_PortMap[nPort].m_strNWSCP.c_str());
				tSimUnlockCode.szCorporateCode			= (char*)(m_PortMap[nPort].m_strNWSCP.c_str());
				tSimUnlockCode.szNetworkSubsetCode		= (char*)(m_PortMap[nPort].m_strNWSCP.c_str());
				tSimUnlockCode.szSimCode				= (char*)(m_PortMap[nPort].m_strNWSCP.c_str());
				tSimUnlockCode.szUsimCode				= (char*)(m_PortMap[nPort].m_strSSCP.c_str());
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
					AddLog(nPort, _T("SimData size : %d bytes, it should be 576 bytes."));
				}

				memcpy((void*)pSimDataBuf, (void*)pcSimLockData, sizeof(BYTE)*nSimLockData);
				int nFreeSimEncResult = ISimEnc_FreeSimEncObj(hSimEncHandle);

				bRes = (nResult == SIM_RET_SUCCESS);
			}

			if(!bRes)
			{
				SetLastError(nPort, DIAG_ERROR_GEN_SIMDATA);
				AddLog(nPort, _T("GenSimData fail"));
			}
			else
			{
				CString strGenSimSuccess;
				strGenSimSuccess.Format(_T("GenSimData success: %x"), *pSimDataBuf);
				AddLog(nPort, strGenSimSuccess);
			}
		}
		else
			SetLastError(nPort, DIAG_ERROR_INVALID_FUNCTION_PARAMETER);
	}
	return bRes;
}
void CODM_Export::AddLog(long nPort, const char* szMsg)
{
	CString strFileName;
	strFileName.Format(_T("LoopFlash_Port%02d.txt"),nPort);
	LogPolicy::Log(szMsg, strFileName);
}

int  CODM_Export::GetLastError(long nPort)
{
	if(m_PortMap.find(nPort) == m_PortMap.end())
		return 2; //Comport disconnect.

	char szLastError[10] = {0};

	int nSize = 0;
	FILETIME time;
	CString strErrorKey;
	//strErrorKey.Format(_T("%s%d"), g_szCFCErrorCodeTitle, nPort);
	//if(CCloud::Get((TSTRING)strErrorKey, nSize, NULL, time) && nSize > 0)
	//	CCloud::Get((TSTRING)strErrorKey, nSize, &szLastError, time);

	return atoi(szLastError);
}
bool CODM_Export::GetLastError(long nComPort,const TCHAR* szModule, char* szErrMsg500)
{
	bool bRes = false;
//	if(szModule && szErrMsg500)
//	{
//		int nError = 1; //Fail
//		if(m_PortMap.find(nComPort) == m_PortMap.end())
//		{
//			if(strcmp(szModule, "NVModule") == 0)
//				nError = 2; // Fail in connection
//			else if(strcmp(szModule, "Download") == 0)
//				nError = 70; // Fail in connection
//		}
//		else
//			nError = GetLastError(nComPort);
//
//		TSTRING strErrorContent = CErrorCode::GetErrorString(szModule, nError);
//		CString strErrorCode;
//		strErrorCode.Format(_T("COM%d:(%d)%s"), nComPort, nError, strErrorContent.c_str());
//
//#ifdef _UNICODE
//		std::string strTmp = QSYNC_LIB::widetoansi((TSTRING)strErrorCode);
//		strcpy(szErrMsg500, strTmp.c_str());
//#else
//		memset(szErrMsg500,0,strErrorCode.GetLength() + 1);
//		strcpy(szErrMsg500, strErrorCode);
//#endif
//		bRes = true;
//	}
	return bRes;
}
