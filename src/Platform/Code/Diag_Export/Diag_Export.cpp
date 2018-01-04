// Diag_Export.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Diag_Export.h"
#include "../odmtma_api/1FFMaker/C1FFMaker.h"
#include "../odmtma_api/QFlash/QVersionParser.h"
#include "../CommonUtil/ErrorCode.h"
#include "../CommonUtil/Win32ThreadUtility.h"
#include "../../Code\Lib\RequestUtility\DownloadCommand.h"


// CDiag_ExportApp
// See Diag_Export.cpp for the implementation of this class
//

class CDiag_ExportApp : public CWinApp
{
public:
	CDiag_ExportApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

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


// CDiag_ExportApp

BEGIN_MESSAGE_MAP(CDiag_ExportApp, CWinApp)
END_MESSAGE_MAP()


// CDiag_ExportApp construction

CDiag_ExportApp::CDiag_ExportApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CDiag_ExportApp object

CDiag_ExportApp theApp;


// CDiag_ExportApp initialization

BOOL CDiag_ExportApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

CDiag_Export::CDiag_Export()
{
	m_PortMap.clear();
	TRACE_FILE_LINE_INFO();
}
CDiag_Export::~CDiag_Export()
{
	for(std::map<long, COTPControl>::iterator iter=m_PortMap.begin(); iter!=m_PortMap.end(); iter++)
	{
		CSingleLock singlelock(&m_CritMap,TRUE);
		if(iter->second.m_pQualcommDiag)
			delete iter->second.m_pQualcommDiag;
		iter->second.m_pQualcommDiag = NULL;
		m_PortMap.erase(iter);
	}
}

void CDiag_Export::InitializeParameter(long nComport, bool bAlwaysFlash)
{
	CSingleLock singlelock(&m_CritMap,TRUE);
	m_PortMap[nComport].m_bAlwaysFlash = bAlwaysFlash;
}
void CDiag_Export::SetUsbIndex(long nComport, const char* szUSBIndex)
{
	CSingleLock singlelock(&m_CritMap,TRUE);
	m_PortMap[nComport].m_strUSBIndex = szUSBIndex;
}
bool CDiag_Export::GetUsbIndex(long nComport, CStringA& strUSBIndex)
{
	bool bRes = true;
	std::map<long, COTPControl>::iterator iter = m_PortMap.find(nComport);
	if(iter != m_PortMap.end())
	{
		strUSBIndex = m_PortMap[nComport].m_strUSBIndex.c_str();
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::ConnectComPort(long nComport)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	bool bNotFind = m_PortMap.find(nComport) == m_PortMap.end();
	if(bNotFind || (!bNotFind && !m_PortMap[nComport].m_pQualcommDiag))
	{
#ifndef DISABLE_LOG_FUNCTION
		{
			CStringA strFileName = CStringA(m_PortMap[nComport].m_strUSBIndex.c_str()) + ".txt";
			if(bNotFind)
				LogPolicy::FunctionLog("CDiag_Export::ConnectComPort: Map not find comport", strFileName);
			if((!bNotFind && m_PortMap[nComport].m_pQualcommDiag == NULL))
			{
				char szTempLog[100] = {0}; 
				sprintf(szTempLog, "CDiag_Export::ConnectComPort: QualcommDiag = %x", m_PortMap[nComport].m_pQualcommDiag);
				LogPolicy::FunctionLog(szTempLog, strFileName);
			}
		}
#endif
		CQualcommDiag* pDiag = new CQualcommDiag(_T(""),nComport);
		if(pDiag)
		{
			if(pDiag->IsConnected())
			{
				bRes = true;
				CSingleLock singlelock(&m_CritMap,TRUE);
				m_PortMap[nComport].m_pQualcommDiag = pDiag;
				if(bNotFind)
				{
					TRACE_FILE_LINE_INFO();
					m_PortMap[nComport].m_bAlwaysFlash = true;
				}
#ifndef DISABLE_LOG_FUNCTION
				{
					if(pDiag == NULL)
					{
						CStringA strFileName = CStringA(m_PortMap[nComport].m_strUSBIndex.c_str()) + ".txt";
						LogPolicy::FunctionLog("CDiag_Export::ConnectComPort: QualcommDiag = NULL but assign", strFileName);
					}
					
				}
#endif
			}
			else
			{
				delete pDiag;		
				pDiag = NULL;
#ifndef DISABLE_LOG_FUNCTION
				{
					CStringA strFileName = CStringA(m_PortMap[nComport].m_strUSBIndex.c_str()) + ".txt";
					LogPolicy::FunctionLog("CDiag_Export::ConnectComPort: QualcommDiag = NULL", strFileName);
				}
#endif
			}
		}
	}
	else
	{
		if(m_PortMap[nComport].m_pQualcommDiag->IsConnected())
			bRes = true;
		else
		{
#ifndef DISABLE_LOG_FUNCTION
			{	
				CStringA strFileName = CStringA(m_PortMap[nComport].m_strUSBIndex.c_str()) + ".txt";
				LogPolicy::FunctionLog("CDiag_Export::ConnectComPort : didn't new but connect fail", strFileName);
			}
#endif
			if(m_PortMap[nComport].m_pQualcommDiag)
			{
				delete m_PortMap[nComport].m_pQualcommDiag;
				m_PortMap[nComport].m_pQualcommDiag = NULL;
			}
		}
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}
bool CDiag_Export::DisconnectComPort(long nComport)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = true;
	std::map<long, COTPControl>::iterator iter = m_PortMap.find(nComport);
	if(iter != m_PortMap.end())
	{
		CSingleLock singlelock(&m_CritMap,TRUE);
		if(iter->second.m_pQualcommDiag)
			delete iter->second.m_pQualcommDiag;
		iter->second.m_pQualcommDiag = NULL;
#ifndef DISABLE_LOG_FUNCTION
		{
			CStringA strFileName = CStringA(m_PortMap[nComport].m_strUSBIndex.c_str()) + ".txt";
			LogPolicy::FunctionLog("CDiag_Export::DisconnectComPort : delete pQualcommDiag", strFileName);
		}
#endif
		m_PortMap.erase(iter);
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::TempDisconnectComPort(long nComport)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = true;
	std::map<long, COTPControl>::iterator iter = m_PortMap.find(nComport);
	if(iter != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag != NULL)
	{
		CSingleLock singlelock(&m_CritMap,TRUE);
		if(iter->second.m_pQualcommDiag)
			delete iter->second.m_pQualcommDiag;
		iter->second.m_pQualcommDiag = NULL;
#ifndef DISABLE_LOG_FUNCTION
		{
			CStringA strFileName = CStringA(m_PortMap[nComport].m_strUSBIndex.c_str()) + ".txt";
			LogPolicy::FunctionLog("CDiag_Export::TempDisconnectComPort : delete pQualcommDiag", strFileName);
		}
#endif
		//m_PortMap.erase(iter);
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}


bool CDiag_Export::SetTestMode(long nComport)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
#ifndef DISABLE_LOG_FUNCTION
		{
			CStringA strFileName = CStringA(m_PortMap[nComport].m_strUSBIndex.c_str()) + ".txt";
			LogPolicy::FunctionLog("Diag_Export::SetTestMode - IsDLMode?", strFileName);
		}
#endif
		if(m_PortMap[nComport].m_pQualcommDiag->IsDLMode())
		{
			 m_PortMap[nComport].m_pQualcommDiag->SetLastError(DIAG_ERROR_DLMODE);
#ifndef DISABLE_LOG_FUNCTION
			 {
				 CStringA strFileName = CStringA(m_PortMap[nComport].m_strUSBIndex.c_str()) + ".txt";
				 LogPolicy::FunctionLog("Diag_Export::SetTestMode - IsDLMode: Yes", strFileName);
			 }
#endif
		}
		else
		{
#ifndef DISABLE_LOG_FUNCTION
			{
				CStringA strFileName = CStringA(m_PortMap[nComport].m_strUSBIndex.c_str()) + ".txt";
				LogPolicy::FunctionLog("Diag_Export::SetTestMode - IsDLMode: No", strFileName);
				LogPolicy::FunctionLog("Diag_Export::SetTestMode - IsEMU?", strFileName);
			}
#endif
			if(m_PortMap[nComport].m_pQualcommDiag->IsEMU())
			{
				bRes = true;
#ifndef DISABLE_LOG_FUNCTION
				{
					CStringA strFileName = CStringA(m_PortMap[nComport].m_strUSBIndex.c_str()) + ".txt";
					LogPolicy::FunctionLog("Diag_Export::SetTestMode - IsEMU: Yes", strFileName);
				}
#endif
			}
			else
			{
#ifndef DISABLE_LOG_FUNCTION
				{
					CStringA strFileName = CStringA(m_PortMap[nComport].m_strUSBIndex.c_str()) + ".txt";
					LogPolicy::FunctionLog("Diag_Export::SetTestMode - IsEMU: No", strFileName);
					LogPolicy::FunctionLog("Diag_Export::SetTestMode - IsTestMode?", strFileName);
				}
#endif
				bRes = m_PortMap[nComport].m_pQualcommDiag->IsTestMode();
#ifndef DISABLE_LOG_FUNCTION
				{
					CStringA strFileName = CStringA(m_PortMap[nComport].m_strUSBIndex.c_str()) + ".txt";
					if(bRes)
						LogPolicy::FunctionLog("Diag_Export::SetTestMode - IsTestMode: Yes", strFileName);
					else
						LogPolicy::FunctionLog("Diag_Export::SetTestMode - IsTestMode: No", strFileName);
				}
#endif
			}
		}
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::Restart(long nComport)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		bRes = m_PortMap[nComport].m_pQualcommDiag->Restart();
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::Reset(long nComport)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		bRes = m_PortMap[nComport].m_pQualcommDiag->Reset();
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::TurnUnitOff(long nComport)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();

	/*	bRes = m_PortMap[nComport].m_pQualcommDiag->Restart();
		//Sleep(3000);
		if(bRes)*/
		Sleep(4000);
		bRes = m_PortMap[nComport].m_pQualcommDiag->CloseConnection(10000);
		if(bRes)
			DisconnectComPort(nComport);
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}
bool CDiag_Export::ReadIMEI(long nComport, char* szIMEI100)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		bRes = m_PortMap[nComport].m_pQualcommDiag->ReadIMEI(szIMEI100, 5000);
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::ReadBTAddr(long nComport, char* szAddress100, long& nLength)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		bRes = m_PortMap[nComport].m_pQualcommDiag->ReadBTAddr(szAddress100, nLength);
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::IsUnitOn(long nComport)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		bRes = m_PortMap[nComport].m_pQualcommDiag->IsUnitOn();
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}
bool CDiag_Export::IsUnitOff(long nComport)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = true;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		bRes = m_PortMap[nComport].m_pQualcommDiag->IsUnitOff();
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::ReadFlexVersion(long nComport, char* szReadValue1024, long& nLength)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		char szFlexVersion[1024] = {0};
		std::string strLocVersion;
		if(m_PortMap[nComport].m_pQualcommDiag->ReadFlexVersion(szFlexVersion, nLength))
		{
			std::string strVersion = szFlexVersion;
			if(m_PortMap[nComport].m_pQualcommDiag->ReadLockFileName(strLocVersion) && strVersion.length() != 0)
			{
				strVersion += "_";
				strVersion += strLocVersion;
			}
			memcpy(szReadValue1024, strVersion.c_str(), strVersion.length());
			bRes = true;
		}
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::ReadSWVersion(long nComport, char* szReadValue1024, long& nLength)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		bRes = m_PortMap[nComport].m_pQualcommDiag->ReadSWVersion(szReadValue1024, nLength);
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}
bool CDiag_Export::ReadTransceiverKitNumber(long nComport,  char* szTransceiverKitNumber100)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		bRes = m_PortMap[nComport].m_pQualcommDiag->ReadTransceiverKitNumber(szTransceiverKitNumber100, 5000);
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::ReadTrackID(long nComport,  char * szTrackID)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		bRes = m_PortMap[nComport].m_pQualcommDiag->ReadTrackID(szTrackID);
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::ReadFactoryInforBlock(long nComport, char* szBlock, long& nLength)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		bRes = m_PortMap[nComport].m_pQualcommDiag->ReadFactoryInforBlock(szBlock, nLength, 5000);
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}
bool CDiag_Export::ProgramIMEI(long nComport,char* szIMEI)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		if(szIMEI)
		{
			if(m_PortMap[nComport].m_pQualcommDiag->IsOTPFrozen())
			{//IMEI exists
				char szIMEI100[100] = {0};
				if(m_PortMap[nComport].m_pQualcommDiag->ReadIMEI(szIMEI100) && 0 == strcmp(szIMEI100, szIMEI))
					bRes = true;
				else
					m_PortMap[nComport].m_pQualcommDiag->SetLastError(DIAG_ERROR_IMEI_MISMATCH);
			}
			else
			{
				if(m_PortMap[nComport].m_pQualcommDiag->GetLastError() == DIAG_ERROR_EMPTY_OTP)
				{
					BYTE szFSN16[16] = {0};
					char szBTAddr[100] = {0};
					long nLength = 0;
					if( m_PortMap[nComport].m_pQualcommDiag->GenFSN(szFSN16) &&
						m_PortMap[nComport].m_pQualcommDiag->ReadBTAddr(szBTAddr, nLength) && strlen(szBTAddr)!=0 &&
						m_PortMap[nComport].m_pQualcommDiag->WriteIMEI(szIMEI, 8000) &&
						m_PortMap[nComport].m_pQualcommDiag->WriteFSN(szFSN16, 8000))
					{
						m_PortMap[nComport].m_pQualcommDiag->FreezeOTP();
						Sleep(1000);
						bRes = m_PortMap[nComport].m_pQualcommDiag->IsOTPFrozen();
					}
				}
			}
		}
		else
			m_PortMap[nComport].m_pQualcommDiag->SetLastError(DIAG_ERROR_INVALID_FUNCTION_PARAMETER);
	}
	return bRes;
}

bool CDiag_Export::WriteSimData(long nComport)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		if(m_PortMap[nComport].m_pQualcommDiag->IsOTPFrozen())
		{
			char szIMEI100[100] = {0};
			BYTE szFSN16[16] = {0};
			BYTE szSimData[512] = {0};
			if(m_PortMap[nComport].m_pQualcommDiag->ReadIMEI(szIMEI100) && 
			   m_PortMap[nComport].m_pQualcommDiag->ReadFSN(szFSN16) &&
			   GenSimData(nComport, szIMEI100, szFSN16, szSimData, 512) &&
			   m_PortMap[nComport].m_pQualcommDiag->WriteSimData(szSimData, 512, 20000))
			{
				long nStatus = 0;
				bool bCheckLock = false;
				for(int i=0; i<5 && !bCheckLock; i++)
				{
					Sleep(5000);
					if(m_PortMap[nComport].m_pQualcommDiag->IsUnitLocked(nStatus,0))
						bCheckLock = true;
				}
				if(bCheckLock)
				{
					std::string str1ffLocName = m_PortMap[nComport].m_str1ffLocPath;
					std::string::size_type nSlash = str1ffLocName.find_last_of(_T("\\"));
					str1ffLocName = str1ffLocName.substr(nSlash+1, str1ffLocName.length()-nSlash-5 );
					bRes = m_PortMap[nComport].m_pQualcommDiag->WriteLockFileName(str1ffLocName);
					/*if(bRes)
						bRes = m_PortMap[nComport].m_pQualcommDiag->Backup(120000);
					else
						m_PortMap[nComport].m_pQualcommDiag->SetLastError(DIAG_ERROR_BACKUP);*/
				}
				else
				{
					m_PortMap[nComport].m_pQualcommDiag->SetLastError(DIAG_ERROR_WRITESIMDATA);
				}
			}
		}
		else
			m_PortMap[nComport].m_pQualcommDiag->SetLastError(DIAG_ERROR_OTP_UNFREEZED);
	}
	return bRes;
}

bool CDiag_Export::GenSimData(long nComport, char* szIMEI100, BYTE* szFSN16, BYTE *pSimDataBuf, int pSimDataBufLen)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		if(!m_PortMap[nComport].m_strSSCP.empty() && !m_PortMap[nComport].m_strNWSCP.empty() &&
			!m_PortMap[nComport].m_str1ffLocPath.empty() && szIMEI100 && szFSN16 && pSimDataBuf)
		{
			char szFSN33[33] = {0};
			m_PortMap[nComport].m_pQualcommDiag->HexBcdToDigit((BYTE*)szFSN16, 16, szFSN33, 33);
			for(int i=0; i<16; i++)
			{
				char cTemp = szFSN33[i*2];
				szFSN33[i*2] = szFSN33[i*2+1];
				szFSN33[i*2+1] = cTemp;
			}
			//int nGen = Lib_ISimEnc_GenSimFile(szImei, (const BYTE*)szFSN33, szMasterCode, szSubsidyCode, szLockPath, simEncryptedDataBuf, simEncryptedDataBufLen);
			int nGen = ISimEnc_GenSimFile((const BYTE*)szIMEI100, 
										  (const BYTE*)szFSN33, 
										  (const BYTE*)m_PortMap[nComport].m_strSSCP.c_str(), 
										  (const BYTE*)m_PortMap[nComport].m_strNWSCP.c_str(),
										  (const BYTE*)m_PortMap[nComport].m_str1ffLocPath.c_str(), 
										  pSimDataBuf, pSimDataBufLen);
			bRes = nGen ==0;
		
			if(!bRes)
			{
				m_PortMap[nComport].m_pQualcommDiag->SetLastError(DIAG_ERROR_GEN_SIMDATA);
				m_PortMap[nComport].m_pQualcommDiag->AddLog("GenSimData fail");
			}
			else
			{
				char szSimData[1000] = {0};
				sprintf(szSimData, "GenSimData success: %x", *pSimDataBuf);
				m_PortMap[nComport].m_pQualcommDiag->AddLog(szSimData);
			}
		}
		else
		{
			m_PortMap[nComport].m_pQualcommDiag->SetLastError(DIAG_ERROR_INVALID_FUNCTION_PARAMETER);
		}
	}
	return bRes;
}

bool CDiag_Export::WriteFactoryInforBlock(long nComport, char* szBlock)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		bRes = m_PortMap[nComport].m_pQualcommDiag->WriteFactoryInforBlock(szBlock);
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}


bool CDiag_Export::WriteBTAddr(long nComport,char* szAddress)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		bRes = m_PortMap[nComport].m_pQualcommDiag->WriteBTAddr(szAddress);
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::WriteTrackID(long nComport,  char * szTrackID)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		bRes = m_PortMap[nComport].m_pQualcommDiag->WriteTrackID(szTrackID);
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::WriteTransceiverKitNumber(long nComport,  char* szTransceiverKitNumber100)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		bRes = m_PortMap[nComport].m_pQualcommDiag->WriteTransceiverKitNumber(szTransceiverKitNumber100);
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::SetInFactoryFlag(long nComport, bool bOpen)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		bRes = m_PortMap[nComport].m_pQualcommDiag->TurnOnOffEM(bOpen);
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::MasterClear(long nComport)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		bRes = m_PortMap[nComport].m_pQualcommDiag->MasterClear();
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::IsSPCLocked(long nComport, long& nStatus)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		bRes = m_PortMap[nComport].m_pQualcommDiag->IsSPCLocked(nStatus);
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::IsNWSCPLocked(long nComport, bool& bNWUnlocked, bool& bNSUnlocked, bool& bSPUnlocked, bool& bCPUnlocked, bool& bSIMUnlocked)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		bool bSimDataExist = true;
		if(m_PortMap[nComport].m_pQualcommDiag->QuerySimDataStatus(bSimDataExist) && bSimDataExist)
		{
			bRes = m_PortMap[nComport].m_pQualcommDiag->IsNWSCPLocked(bNWUnlocked, bNSUnlocked, bSPUnlocked, bCPUnlocked, bSIMUnlocked);
		}
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
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::IsUnitLocked(long nComport, long& nStatus, int nLockType/*0=NWSCP, 1=SSCP*/)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		bool bSimDataExist = true;
		if(m_PortMap[nComport].m_pQualcommDiag->QuerySimDataStatus(bSimDataExist) && bSimDataExist)
		{
			bRes = m_PortMap[nComport].m_pQualcommDiag->IsUnitLocked(nStatus, nLockType);
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

bool CDiag_Export::UnlockSPC(long nComport, char* szLock, bool& bUnlockOK)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		bRes = m_PortMap[nComport].m_pQualcommDiag->UnLockSPC(szLock, bUnlockOK);
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::UnLockUnit(long nComport, int nLockType/*0=NW, 1=NS, 2=SP, 3=CP, 4=SIM, 5=PROP1*/, char* szLock, bool& bUnlockOK)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		bRes = m_PortMap[nComport].m_pQualcommDiag->UnLockUnit(nLockType, szLock, bUnlockOK);
	}
	return bRes;
}

bool CDiag_Export::UnLockPhoneCode(long nComport, char* szLock, bool& bUnlockOK)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		long nStatus = 0;
		if(IsUnitLocked(nComport, nStatus, 1) && nStatus!=0)
		{
			bRes = m_PortMap[nComport].m_pQualcommDiag->UnLockPhoneCode(szLock, bUnlockOK);
		}
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::SetRTC(long nComport)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		bRes = m_PortMap[nComport].m_pQualcommDiag->SetRTC();
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::DeleteFiles()
{
	ODMTMA_LIB::CQZip QZip;
	return QZip.DeleteUnzipFiles();
}

bool CDiag_Export::Flash(long nComport, char* szMotPath)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		CQDOperator QDownload(m_PortMap[nComport].m_pQualcommDiag);
		
		bool bUnziped = true;
		bool bSetFlashFileName = false;
		bool bCompareVersion = false;
		bool bSetFlashNametoQDownload = false;
		bool bSetLockName = false;
		CFileGetter FileManager;
		if(FileManager.SetMot(szMotPath))
		{	
			CSingleLock singlelock(&m_CritUnZip,TRUE);
			bSetFlashFileName = FileManager.SetFileName();
			if(!bSetFlashFileName)
			{
				ODMTMA_LIB::CQZip QZip;
				QZip.SetFolderPathFromMot(szMotPath);
				bUnziped = QZip.UnzipWithoutExe(szMotPath, _T(""));
				bSetFlashFileName = FileManager.SetFileName(false);  //Get file name.
			}
			bool bDownloadOEMSBL = false;
			if(bUnziped && bSetFlashFileName)
			{
				bCompareVersion = CompareVersion(nComport, FileManager, QDownload, bDownloadOEMSBL);  //Compare first
				if(bCompareVersion)
				{
					bSetFlashNametoQDownload = SetFlashName(FileManager, QDownload, bDownloadOEMSBL); //SetPath secondly
					bSetLockName = SetLocName(nComport, FileManager);								  //SetLoc in the end
				}				
			}
		}
		//////////////////////////////////////////////////////////////////////////
		
		if(bUnziped && bSetFlashFileName 
			&& bCompareVersion 
			&& bSetFlashNametoQDownload
			&& bSetLockName)								
		{	
			TRACE_FILE_LINE_INFO();
			bRes = QDownload.CFCSpeedUpFlash() >=0 ? true: false;
			//bRes = true;
			TRACE_FILE_LINE_INFO();
		}
		else
		{
			if(!bUnziped)
				m_PortMap[nComport].m_pQualcommDiag->SetLastError(DL_ERROR_UNZIP);
			else if(!bSetFlashFileName)
				m_PortMap[nComport].m_pQualcommDiag->SetLastError(DL_ERROR_MISSFILES);
			else if(!bCompareVersion)
				m_PortMap[nComport].m_pQualcommDiag->SetLastError(DL_ERROR_CHECKVERSION);
			else
				m_PortMap[nComport].m_pQualcommDiag->SetLastError(DL_ERROR_SETFILENAME);
		}
		
	}
	TRACE_FILE_LINE_INFO();
 	return bRes;
}

bool CDiag_Export::CompareVersion(long nComport, CFileGetter& FileManager, CQDOperator& QDownload, bool& bDownloadOEMSBL)
{
	bool bRes = false;
	TSTRING strAMSS, strOEMSBL;
	if(FileManager.GetAMSS(strAMSS) && FileManager.GetOEMSBL(strOEMSBL) ) // premise: 1ff contains all files or only flex file.
	{
		ODMTMA_LIB::CQVersionParser VersionParser;
		if(m_PortMap[nComport].m_bAlwaysFlash)
		{
			if(VersionParser.IsCodeSigned(strAMSS, strOEMSBL))
			{
				int nImagePhoneCodeVer = VersionParser.GetPhoneCodeVersion(strAMSS);
				int nImageBootloaderVer = VersionParser.GetBootloaderVersion(strOEMSBL);
				if(nImagePhoneCodeVer != -1 && nImageBootloaderVer != -1)
				{
					int nUnitPhoneCodeVer = 0, nUnitBootloaderVer = 0;
					bool bGetUnitVersion = false;
					if(m_PortMap[nComport].m_pQualcommDiag->IsDLMode())
					{
						bGetUnitVersion = QDownload.GetHDVersion(nUnitPhoneCodeVer, nUnitBootloaderVer, 8000);
					}
					else
					{
						bGetUnitVersion = m_PortMap[nComport].m_pQualcommDiag->GetHDVersion(nUnitPhoneCodeVer, nUnitBootloaderVer, 8000);
					}
					if(bGetUnitVersion)
					{
						if((nImageBootloaderVer >= nUnitBootloaderVer) && (nImagePhoneCodeVer >= nUnitPhoneCodeVer))
						{
							bDownloadOEMSBL = (nImageBootloaderVer > nUnitBootloaderVer); 
							bRes = true;
						}
					}
				}
			}
		}
		else
		{
			bRes = (VersionParser.IsCodeSigned(strAMSS, strOEMSBL) == false);
		}
	}
	else
	{
		bRes = true; // not download mainload
		bDownloadOEMSBL = false;
	}
	return bRes;
}


bool CDiag_Export::SetFlashName(CFileGetter& FileManager, CQDOperator& QDownload, bool bDownloadOEMSBL)
{   // at least contain AMSS and OEMSBL or only Flex file
	bool bRes = false;
	TSTRING strHex, strPartition, strQcsbl, strQcsblhd, strAmss, strAmsshd, strOemsbl, strOemsblhd;
	TSTRING szCEFS, szDu;
	if(FileManager.GetAMSS(strAmss) && FileManager.GetAMSSHD(strAmsshd))
	{
		QDownload.SetAMSS(strAmss.c_str());
		QDownload.SetAMSShd(strAmsshd.c_str());
	}
	if( FileManager.GetHex(strHex) && FileManager.GetPartition(strPartition) 
		&& FileManager.GetCEFS(szCEFS) && FileManager.GetDU(szDu))
	{
		QDownload.SetHex(strHex.c_str());
		QDownload.SetPartition(strPartition.c_str());
		QDownload.SetCEFS(szCEFS.c_str());
		QDownload.SetDu(szDu.c_str());
		bRes = true;
	}
	if(bDownloadOEMSBL)
	{
		if(FileManager.GetOEMSBL(strOemsbl) && FileManager.GetOEMSBLHD(strOemsblhd))
		{
			QDownload.SetOemsbl(strOemsbl.c_str());
			QDownload.SetOemsblhd(strOemsblhd.c_str());
		}
		else
			bRes = false;
	}

	return bRes;
}
bool CDiag_Export::SetLocName(long nComport, CFileGetter& FileManager)
{
	bool bRes = false;
	TSTRING strLoc;
	if(FileManager.GetLoc(strLoc))
	{
		CSingleLock singlelock(&m_CritMap,TRUE);
		m_PortMap[nComport].m_str1ffLocPath = strLoc;
		bRes = true;
	}
	else
	{
		CSingleLock singlelock(&m_CritMap,TRUE);
		m_PortMap[nComport].m_str1ffLocPath.clear();
	}
	return bRes;
}
bool CDiag_Export::SetFlexName(CFileGetter& FileManager, CQDOperator& QDownload)
{
	bool bRes = false;
	TSTRING strHex, szPartition, szCsv, szCEFS, szDu;

	if(FileManager.GetHex(strHex) && FileManager.GetPartition(szPartition) 
		&& FileManager.GetCEFS(szCEFS) && FileManager.GetDU(szDu))
	{
		QDownload.SetHex(strHex.c_str());
		QDownload.SetPartition(szPartition.c_str());
		QDownload.SetCEFS(szCEFS.c_str());
		QDownload.SetDu(szDu.c_str());
		
		QDownload.SetAMSS(_T(""));
		QDownload.SetAMSShd(_T(""));
		QDownload.SetOemsbl(_T(""));
		QDownload.SetOemsblhd(_T(""));
		QDownload.SetQcsbl(_T(""));
		QDownload.SetQcsblhd(_T(""));
		
		bRes = true;
	}
	return bRes;
}	


bool CDiag_Export::Flex(long nComport, char* szMotPath)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		CQDOperator QDownload(m_PortMap[nComport].m_pQualcommDiag);

		bool bUnziped = true;
		bool bSetFlashFileName = true;
		CFileGetter FileManager;
		if(!FileManager.SetMot(szMotPath) && FileManager.SetFileName(false))
		{	
			CSingleLock singlelock(&m_CritUnZip,TRUE);
			ODMTMA_LIB::CQZip QZip;
			QZip.SetFolderPathFromMot(szMotPath);
			bUnziped = QZip.UnzipWithoutExe(szMotPath, _T(""));
			bSetFlashFileName = FileManager.SetFileName(false);
		}
		//////////////////////////////////////////////////////////////////////////
		if(bUnziped && bSetFlashFileName 
			&& SetFlexName(FileManager, QDownload)				//SetPath secondly
			&& SetLocName(nComport, FileManager))               //SetLoc in the end
		{	
			TRACE_FILE_LINE_INFO();
			bRes = QDownload.CFCSpeedUpFlash() >=0 ? true: false;
			//bRes = true;
			TRACE_FILE_LINE_INFO();
		}
		else
		{
			if(!bUnziped)
				m_PortMap[nComport].m_pQualcommDiag->SetLastError(DL_ERROR_UNZIP);
			else if(!bSetFlashFileName)
				m_PortMap[nComport].m_pQualcommDiag->SetLastError(DL_ERROR_MISSFILES);
		}
		
		
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::RestoreNV(long nComport, int nTimeout)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		bRes = m_PortMap[nComport].m_pQualcommDiag->Restore(nTimeout);
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

int CDiag_Export::GetLastError(long nComport)
{
	int nError = 1; //Fail
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
		nError = m_PortMap[nComport].m_pQualcommDiag->GetLastError();
	else
		nError = 2;
	return nError;
}

bool CDiag_Export::GetLastError(long nComPort, const TCHAR* szModule, char* szErrMsg500)
{
	bool bRes = false;
	if(szModule && szErrMsg500)
	{
		int nError = 1; //Fail
		if(m_PortMap.find(nComPort) != m_PortMap.end() && m_PortMap[nComPort].m_pQualcommDiag)
			nError = m_PortMap[nComPort].m_pQualcommDiag->GetLastError();
		else
		{
			if(strcmp(szModule, "NVModule") == 0)
				nError = 2; // Fail in connection
			else if(strcmp(szModule, "Download") == 0)
				nError = 70; // Fail in connection
		}

		TSTRING strErrorCode = _T("COM");
		TSTRING strComport;
		QSYNC_LIB::iToTStr(nComPort, strComport);
		TSTRING strnError;
		QSYNC_LIB::iToTStr(nError, strnError);
	
		strErrorCode += strComport;
		strErrorCode += _T(":"); 
		strErrorCode += _T(" (");
		strErrorCode += strnError;
		strErrorCode += _T(")");
		strErrorCode += CErrorCode::GetErrorString(szModule, nError);

#ifdef _UNICODE
		std::string strTmp = QSYNC_LIB::widetoansi(strErrorCode);
		strcpy(szErrMsg500, strTmp.c_str());
#else
		memset(szErrMsg500,0,strErrorCode.size() + 1);
		strcpy(szErrMsg500, strErrorCode.c_str());
#endif
		bRes = true;
	}
	return bRes;
}

bool CDiag_Export::GetLastError(long nComPort,const TCHAR* szFunctionName,const TCHAR* szModule, char* szErrMsg500)
{
	bool bRes = false;
	if(szModule && szErrMsg500)
	{
		int nError = 1; //Fail
		if(m_PortMap.find(nComPort) != m_PortMap.end() && m_PortMap[nComPort].m_pQualcommDiag)
			nError = m_PortMap[nComPort].m_pQualcommDiag->GetLastError();
		else
		{
			if(strcmp(szModule, "NVModule") == 0)
				nError = 2; // Fail in connection
			else if(strcmp(szModule, "Download") == 0)
				nError = 20; // Fail in connection
		}
		TSTRING strErrorCode = _T("COM");
		TSTRING strComport;
		QSYNC_LIB::iToTStr(nComPort, strComport);
		TSTRING strnError;
		QSYNC_LIB::iToTStr(nError, strnError);
				
		strErrorCode += strComport;
		strErrorCode += _T(":"); 
		strErrorCode += szFunctionName;
		strErrorCode += _T("> (");
		strErrorCode += strnError;
		strErrorCode += _T(")");
		strErrorCode += CErrorCode::GetErrorString(szModule, nError);
		
#ifdef _UNICODE
		std::string strTmp = QSYNC_LIB::widetoansi(strErrorCode);
		strcpy(szErrMsg500, strTmp.c_str());
#else
		strcpy(szErrMsg500, strErrorCode.c_str());
#endif
		bRes = true;
	}
	return bRes;
}

//bool CDiag_Export::WaitForPNPNotify(long nComport, int nTimeout)
//{
//	bool bRes = false;
//	HANDLE hNotify = ::CreateEvent(NULL, TRUE, FALSE, NULL);
//	if(hNotify)
//	{
//		CPNPNotify PNPNotify(hNotify, nComport);
//		if((WAIT_OBJECT_0 + 0) == QSYNC_LIB::MessageLoop(&hNotify,1, nTimeout))
//		{
//			bRes = true;
//		}
//	}
//	
//	return bRes;
//}

bool CDiag_Export::ProgramSPC(long nComport, char* szLock)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		bRes = m_PortMap[nComport].m_pQualcommDiag->ProgramSPC(szLock);
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CDiag_Export::ProgramNWSCP(long nComport, char* szLock)
{
	bool bRes = false;
	if(szLock)
	{
		CSingleLock singlelock(&m_CritMap,TRUE);
		m_PortMap[nComport].m_strNWSCP = szLock;
		bRes = true;
		TRACE_FILE_LINE_INFO();
	}
	return bRes;
}

bool CDiag_Export::ProgramPhoneCode(long nComport, char* szLock)
{
	bool bRes = false;
	if(szLock)
	{
		CSingleLock singlelock(&m_CritMap,TRUE);
		m_PortMap[nComport].m_strSSCP = szLock;
		bRes = true;
		TRACE_FILE_LINE_INFO();
	}
	return bRes;
}


bool CDiag_Export::LockSPC(long nComport)
{
	bool bRes = false;
	if(m_PortMap.find(nComport) != m_PortMap.end() && m_PortMap[nComport].m_pQualcommDiag)
	{
		TRACE_FILE_LINE_INFO();
		char szSPC[100] = {0};
		long nSPCStatus = 0;
		if(m_PortMap[nComport].m_pQualcommDiag->ReadSPC(szSPC, nSPCStatus))
		{
			if(szSPC[0] != '1')
				szSPC[0] = '1';
			else
				szSPC[0] = '2';
			bool bUnlock = true;
			if(m_PortMap[nComport].m_pQualcommDiag->UnLockSPC(szSPC, bUnlock))
			{
				bRes = !bUnlock ? true : false;
			}
		}
		else
		{
			if(nSPCStatus == 1)
				bRes = true;
		}
	}
	return bRes;
}



//////////////////////////////////////////////////////////////////////////
bool FindMotoUSB(const std::string& strUSB,std::string& strSerial)
{

	bool bRes = false;
	using namespace std;
	string::size_type nFindIndex = strUSB.find("USB");

	if(nFindIndex != string::npos)
	{

		string strUSBNumber = strUSB.substr(nFindIndex + 3);
		const string strKey = "SOFTWARE\\Motorola\\NexTest\\CUsbCSequencer";
		/*
		[HKEY_LOCAL_MACHINE\SOFTWARE\Motorola\NexTest\CUsbCSequencer]
		"PathPrefix1"="7&31718826&0&0001"
		*/
		HKEY hKey;

		LONG lRet = ::RegOpenKeyExA( HKEY_LOCAL_MACHINE,strKey.c_str(),0, KEY_READ, &hKey );

		if( lRet == ERROR_SUCCESS )
		{

			DWORD nBufferSize = 512;
			char szBuffer[512] = {0};
			string strField = "PathPrefix" + strUSBNumber;

			lRet = ::RegQueryValueExA( hKey, strField.c_str(), NULL, NULL,(LPBYTE) szBuffer, &nBufferSize);

			bRes = lRet == ERROR_SUCCESS;
			if(bRes)
			{	
				strSerial = szBuffer;

			}
			::RegCloseKey(hKey);

		}
	}
	return bRes;
}
bool TransferSerialinUSB(const std::string& strSerial,const std::string& strVID,const std::string& strPID, int& nControl, std::string& strM1Serial)
{
	/*
	HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Enum\USB\Vid_1d45&Pid_4510\"strSerial"
	"ParentIdPrefix"=strM1Serial
	*/
	bool bRes = false;
	using namespace std;

	//MI_01 always is DIAG port for now 2008/4/15
	const string strKeyFormat = "SYSTEM\\ControlSet%03d\\Enum\\USB\\Vid_%s&Pid_%s\\%s";

	for(int nControlSet=1;nControlSet<5 && !bRes;nControlSet++)
	{
		char szKey[1024] = {0};
		::sprintf(szKey,strKeyFormat.c_str(),nControlSet,strVID.c_str(),strPID.c_str(),strSerial.c_str());
		HKEY hKey;

		if(ERROR_SUCCESS == ::RegOpenKeyExA( HKEY_LOCAL_MACHINE,szKey,0, KEY_READ, &hKey ))
		{
			DWORD nBufferSize = 512;
			char szBuffer[512] = {0};
			string strField = "ParentIdPrefix";

			LONG lRet = ::RegQueryValueExA( hKey, strField.c_str(), NULL, NULL,(LPBYTE) szBuffer, &nBufferSize);

			bRes = lRet == ERROR_SUCCESS;
			if(bRes)
			{
				strM1Serial = szBuffer;
				nControl = nControlSet;
			}
			::RegCloseKey(hKey);
		}
	}

	return bRes;
}

bool FindComportFromSerial(std::string strSerial,int nControlSet,const std::string& strVID,const std::string& strPID,std::string& strCOMPort)
{
	/*
	HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Enum\USB\Vid_1d45&Pid_4510&MI_01\7&31718826&0&0001\Device Parameters
	"PortName"="COM87"
	*/
	bool bRes = false;
	using namespace std;

	//MI_01 always is DIAG port for now 2008/4/15
	const string strKeyFormat = "SYSTEM\\ControlSet%03d\\Enum\\USB\\Vid_%s&Pid_%s&MI_01\\%s\\Device Parameters";	

	char szKey[1024] = {0};
	strSerial += "&0001";
	::sprintf(szKey,strKeyFormat.c_str(),nControlSet,strVID.c_str(),strPID.c_str(),strSerial.c_str());
	HKEY hKey;

	LONG lRet = ::RegOpenKeyExA( HKEY_LOCAL_MACHINE,szKey,0, KEY_READ, &hKey );

	if( lRet == ERROR_SUCCESS )
	{
		DWORD nBufferSize = 512;
		char szBuffer[512] = {0};
		string strField = "PortName";

		lRet = ::RegQueryValueExA( hKey, strField.c_str(), NULL, NULL,(LPBYTE) szBuffer, &nBufferSize);

		bRes = lRet == ERROR_SUCCESS;
		if(bRes)
		{
			strCOMPort = szBuffer;

		}
		::RegCloseKey(hKey);

	}


	return bRes;
}

DIAG_API bool D_GetMotoXPRS(long nComPort, MotoXPRSDATA& motoXPRS,int nTimeOut)
{
	TCHAR szComport[50] = {0};
	if ( nComPort > 9 )// COM port number larger than 9, add prefix string.
		::_stprintf(szComport, _T("\\\\.\\COM%d"),nComPort);
	else	
		::_stprintf(szComport, _T("COM%d"),nComPort);
	CQualcommDiag QDiag(_T(""),nComPort);	
	bool bRes = QDiag.GetMotoXPRS((char*)&motoXPRS,sizeof(MotoXPRSDATA),nTimeOut);
	return bRes;
}

DIAG_API bool D_UnlockSPC(long nComport, char* szLock)
{
	CQualcommDiag QDiag(_T(""),nComport);
	bool bUnlockSuccess = false;
	if(szLock && strlen(szLock) == 6)
		QDiag.UnLockSPC(szLock, bUnlockSuccess);
	return bUnlockSuccess;
}
DIAG_API bool D_UnlockNW(long nComport, char* szLock)
{
	CQualcommDiag QDiag(_T(""),nComport);
	bool bUnlockSuccess = false;
	if(szLock && strlen(szLock) < 10)
		QDiag.UnLockUnit(0,szLock, bUnlockSuccess);
	return bUnlockSuccess;
}
DIAG_API bool D_UnlockNS(long nComport, char* szLock)
{
	CQualcommDiag QDiag(_T(""),nComport);
	bool bUnlockSuccess = false;
	if(szLock && strlen(szLock) < 10)
		QDiag.UnLockUnit(1,szLock, bUnlockSuccess);
	return bUnlockSuccess;
}
DIAG_API bool D_UnlockSP(long nComport, char* szLock)
{
	CQualcommDiag QDiag(_T(""),nComport);
	bool bUnlockSuccess = false;
	if(szLock && strlen(szLock) < 10)
		QDiag.UnLockUnit(2,szLock, bUnlockSuccess);
	return bUnlockSuccess;
}
DIAG_API bool D_UnlockCP(long nComport, char* szLock)
{
	CQualcommDiag QDiag(_T(""),nComport);
	bool bUnlockSuccess = false;
	if(szLock && strlen(szLock) < 10)
		QDiag.UnLockUnit(3,szLock, bUnlockSuccess);
	return bUnlockSuccess;
}
DIAG_API bool D_UnlockSIM(long nComport, char* szLock)
{
	CQualcommDiag QDiag(_T(""),nComport);
	bool bUnlockSuccess = false;
	if(szLock && strlen(szLock) < 10)
		QDiag.UnLockUnit(4,szLock, bUnlockSuccess);
	return bUnlockSuccess;
}
DIAG_API bool D_UnlockPROP(long nComport, char* szLock)
{
	CQualcommDiag QDiag(_T(""),nComport);
	bool bUnlockSuccess = false;
	if(szLock && strlen(szLock) < 10)
		QDiag.UnLockUnit(5,szLock, bUnlockSuccess);
	return bUnlockSuccess;
}
DIAG_API bool D_UnlockPhoneCode(long nComport, char* szLock)
{
	CQualcommDiag QDiag(_T(""),nComport);
	bool bUnlockSuccess = false;
	if(szLock && strlen(szLock) < 9)
		QDiag.UnLockPhoneCode(szLock,bUnlockSuccess);
	return bUnlockSuccess;
}

DIAG_API bool D_BackupNV(long nComport, int nTimeout)
{
	CQualcommDiag QDiag(_T(""),nComport);	
	bool bRes = QDiag.Backup(nTimeout);
	return bRes;
}

bool D_GetMotoUSBPort(const std::string& strUSB,const std::string& strVID,const std::string& strPID,std::string& strCOMPort)/*strUSB = "/USB3"*/
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;

	using namespace std;
	string strSerial;
	if(FindMotoUSB(strUSB,strSerial))
	{
		TRACE_FILE_LINE_INFO();
		int nControlSet = 0;
		string strM1Serial;
		if(TransferSerialinUSB(strSerial,strVID,strPID, nControlSet, strM1Serial))
			bRes = FindComportFromSerial(strM1Serial,nControlSet,strVID,strPID,strCOMPort);
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

DIAG_API bool D_GetMotoUSBPort_LONG(const std::string& strUSB,const std::string& strVID,const std::string& strPID,long& nCOMPort)
{
	using namespace std;
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	string strCOMPort;

	bRes = D_GetMotoUSBPort(strUSB,strVID,strPID,strCOMPort);
	TRACE_FILE_LINE_INFO();
	if(bRes)
	{		
		TRACE_FILE_LINE_INFO();
		string::size_type nFindIndex = strCOMPort.find("COM");
		if(nFindIndex != string::npos)
		{
			TRACE_FILE_LINE_INFO();
			string strCOMPortNumber = strCOMPort.substr(nFindIndex + 3);
			nCOMPort = ::atol(strCOMPortNumber.c_str());
			TRACE_FILE_LINE_INFO();
		}
	}
	return bRes;
}

DIAG_API bool D_SetTestMode(long nComPort)
{
	TRACE_FILE_LINE_INFO();
	CQualcommDiag QDiag(_T(""),nComPort);
	bool bRes = QDiag.SetTestMode();
	TRACE_FILE_LINE_INFO();
	return bRes;
}
DIAG_API bool D_Reset(long nComPort)
{
	TRACE_FILE_LINE_INFO();
	CQualcommDiag QDiag(_T(""),nComPort);	
	bool bRes = QDiag.Reset();
	TRACE_FILE_LINE_INFO();
	return bRes;
}
DIAG_API bool D_ReadIMEI(long nComPort,char* szIMEI100)
{
	TRACE_FILE_LINE_INFO();
	CQualcommDiag QDiag(_T(""),nComPort);	
	bool bRes = QDiag.ReadIMEI(szIMEI100);
	TRACE_FILE_LINE_INFO();
	return bRes;
}

DIAG_API bool D_NewDiagExportInterface(IDiagExport** piDiagExport)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(!(*piDiagExport))
	{
		TRACE_FILE_LINE_INFO();
		*piDiagExport = new CDiag_Export;
		bRes = true;
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}
DIAG_API bool D_DeleteDiagExportInterface(IDiagExport* iDiagExport)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	TRACE_FILE_LINE_INFO();
	if(iDiagExport)
	{
		TRACE_FILE_LINE_INFO();
		delete iDiagExport;
		TRACE_FILE_LINE_INFO();
		iDiagExport = NULL;
		bRes = true;
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

DIAG_API void D_Log(const CString& strLog)
{
	LogPolicy::Log(strLog);
}


